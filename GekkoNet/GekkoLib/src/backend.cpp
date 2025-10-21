#include "backend.h"
#include "input.h"
#include "event.h"
#include "compression.h"

#include <chrono>
#include <cassert>
#include <climits>

// register poly types.
namespace
{
    zpp::serializer::register_types<
        zpp::serializer::make_type<Gekko::SyncMsg, zpp::serializer::make_id("Gekko::SyncMsg")>,
        zpp::serializer::make_type<Gekko::InputMsg, zpp::serializer::make_id("Gekko::InputMsg")>,
        zpp::serializer::make_type<Gekko::InputAckMsg, zpp::serializer::make_id("Gekko::InputAckMsg")>,
        zpp::serializer::make_type<Gekko::SessionHealthMsg, zpp::serializer::make_id("Gekko::SessionHealthMsg")>,
        zpp::serializer::make_type<Gekko::NetworkHealthMsg, zpp::serializer::make_id("Gekko::NetworkHealthMsg")>
    > _;
}

Gekko::MessageSystem::MessageSystem()
{
	_input_size = 0;
	_last_added_input = GameInput::NULL_FRAME;
	_last_added_spectator_input = GameInput::NULL_FRAME;
    _last_sent_network_check = 0;

	// gen magic for session
	std::srand((unsigned int)std::time(nullptr));
	_session_magic = std::rand();

	history = AdvantageHistory();
    session_events = SessionEventSystem();
}

void Gekko::MessageSystem::Init(u32 input_size)
{
	_input_size = input_size;
	_last_added_input = GameInput::NULL_FRAME;
	_last_added_spectator_input = GameInput::NULL_FRAME;

	history.Init();
}


void Gekko::MessageSystem::AddInput(Frame input_frame, u8 input[])
{
	if (_last_added_input + 1 == input_frame) {
		_last_added_input++;
		_player_input_send_list.push_back(new u8[_input_size * locals.size()]);
		std::memcpy(_player_input_send_list.back(), input, _input_size * locals.size());

		// update history
		history.Update(input_frame);
	}

	const Frame min_ack = GetMinLastAckedFrame(false);
	const u32 diff = _last_added_input - min_ack;

	if (_player_input_send_list.size() > std::min(MAX_PLAYER_SEND_SIZE, diff)) {
		delete _player_input_send_list.front();
		_player_input_send_list.pop_front();
	}
}

void Gekko::MessageSystem::AddSpectatorInput(Frame input_frame, u8 input[])
{
	if (_last_added_spectator_input + 1 == input_frame) {
		_last_added_spectator_input++;
		_spectator_input_send_list.push_back(new u8[_input_size * (locals.size() + remotes.size())]);
		std::memcpy(_spectator_input_send_list.back(), input, _input_size * (locals.size() + remotes.size()));
	}

	const Frame min_ack = GetMinLastAckedFrame(true);
	const u32 diff = _last_added_spectator_input - min_ack;

	if (_spectator_input_send_list.size() > std::min(MAX_SPECTATOR_SEND_SIZE, diff)) {
		delete _spectator_input_send_list.front();
		_spectator_input_send_list.pop_front();
	}
}

void Gekko::MessageSystem::SendPendingOutput(GekkoNetAdapter* host)
{
	// add input packet
	if (!_player_input_send_list.empty() && !remotes.empty()) {
		AddPendingInput(false);
        // check for disconnects
        HandleTooFarBehindActors(false);
	}

	// add spectator input packet
	if (!_spectator_input_send_list.empty() && !spectators.empty()) {
		AddPendingInput(true);
        // check for disconnects
        HandleTooFarBehindActors(true);
	}

	// handle messages
	for (u32 i = 0; i < _pending_output.size(); i++) {
		auto& pkt = _pending_output.front();
		if (pkt->pkt.header.type == Inputs || pkt->pkt.header.type == SpectatorInputs) {
            if (pkt->pkt.header.type == Inputs) {
                SendDataToAll(pkt.get(), host);
            } else {
                SendDataToAll(pkt.get(), host, true);
            }
		}
        else if ((pkt->pkt.header.type == SessionHealth || pkt->pkt.header.type == NetworkHealth) && pkt->addr.GetSize() == 0) {
            // send to remotes
            SendDataToAll(pkt.get(), host);
            // send to spectators
            SendDataToAll(pkt.get(), host, true);
        }
		else {
			SendDataTo(pkt.get(), host);
		}
		// housekeeping
		_pending_output.pop();
	}
}

void Gekko::MessageSystem::HandleData(GekkoNetAdapter* host, GekkoNetResult** data, u32 length)
{
    for (u32 i = 0; i < length; i++) {
        auto res = data[i];
        auto addr = NetAddress(res->addr.data, res->addr.size);

        _bin_buffer.clear();

        try {
            _bin_buffer.insert(_bin_buffer.begin(), (u8*)res->data, (u8*)res->data + res->data_len);

            NetPacket pkt;
            zpp::serializer::memory_input_archive in(_bin_buffer);
            in(pkt.header, pkt.body);

            ParsePacket(addr, pkt);
        }
        catch (const std::exception&) {
            printf("failed to deserialize packet\n");
        }

        // cleanup :)
        host->free_data(res->addr.data);
        host->free_data(res->data);
        host->free_data(res);
    }
}

void Gekko::MessageSystem::SendSyncRequest(NetAddress* addr)
{
    if (!addr) {
        return;
    }

    _pending_output.push(std::make_unique<NetData>());
	auto& message = _pending_output.back();

	message->addr.Copy(addr);

	message->pkt.header.type = SyncRequest;
	message->pkt.header.magic = 0;

    auto body = std::make_unique<SyncMsg>();
    body->rng_data = _session_magic;

    message->pkt.body = std::move(body);
}

void Gekko::MessageSystem::SendSyncResponse(NetAddress* addr, u16 magic)
{
    if (!addr || magic == 0) {
        return;
    }

    _pending_output.push(std::make_unique<NetData>());
    auto& message = _pending_output.back();

	message->addr.Copy(addr);
	message->pkt.header.type = SyncResponse;
	message->pkt.header.magic = magic;

    auto body = std::make_unique<SyncMsg>();
    body->rng_data = _session_magic;

    message->pkt.body = std::move(body);
}

std::queue<std::unique_ptr<Gekko::NetInputData>>& Gekko::MessageSystem::LastReceivedInputs()
{
	return _received_inputs;
}

void Gekko::MessageSystem::SendInputAck(Handle player, Frame frame)
{
	auto plyr = GetPlayerByHandle(player);

    if (!plyr) {
        return;
    }

    _pending_output.push(std::make_unique<NetData>());
    auto& message = _pending_output.back();

	message->addr.Copy(&plyr->address);
	message->pkt.header.magic = plyr->session_magic;
	message->pkt.header.type = InputAck;

    auto body = std::make_unique<InputAckMsg>();
	body->ack_frame = frame;
	body->frame_advantage = history.GetLocalAdvantage();

    message->pkt.body = std::move(body);
}

std::vector<Handle> Gekko::MessageSystem::GetHandlesForAddress(NetAddress* addr)
{
	auto result = std::vector<Handle>();
	for (auto& player: remotes) {
		if (player->address.Equals(*addr)) {
			result.push_back(player->handle);
		}
	}
	return result;
}

Gekko::Player* Gekko::MessageSystem::GetPlayerByHandle(Handle handle) 
{
	for (auto& player: remotes) {
		if (player->handle == handle) {
			return player.get();
		}
	}
	return nullptr;
}

Frame Gekko::MessageSystem::GetMinLastAckedFrame(bool spectator) 
{
	Frame min = INT_MAX;
	for (auto& player : spectator ? spectators : remotes) {
		if (player->GetStatus() == Connected) {
			min = std::min(player->stats.last_acked_frame, min);
		}
	}
	return min;
}

Frame Gekko::MessageSystem::GetLastAddedInput(bool spectator)
{
	return spectator ? _last_added_spectator_input : _last_added_input;
}

bool Gekko::MessageSystem::CheckStatusActors()
{
	i32 result = 0;
	u64 now = TimeSinceEpoch();

    std::vector<std::unique_ptr<Player>>* current = &remotes;

    for (u32 i = 0; i < 2; i++)
    {
        if (i == 1) {
            current = &spectators;
        }
        for (auto& player : *current) {
            if (player->GetStatus() == Initiating) {
                if (player->stats.last_sent_sync_message + NetStats::SYNC_MSG_DELAY < now) {
                    if (player->sync_num == 0) {
                        SendSyncRequest(&player->address);
                        player->stats.last_sent_sync_message = now;
                    }
                    else if (player->sync_num < NUM_TO_SYNC) {
                        SendSyncResponse(&player->address, player->session_magic);
                        player->stats.last_sent_sync_message = now;
                    }
                    else {
                        player->SetStatus(Connected);
                        session_events.AddPlayerConnectedEvent(player->handle);
                        result++;
                    }
                }
                result--;
            }
        }
    }

	return result == 0;
}

void Gekko::MessageSystem::SendSessionHealth(Frame frame, u32 checksum)
{
    _pending_output.push(std::make_unique<NetData>());
    auto& message = _pending_output.back();

    // the address and magic is set later so dont worry about it now
    message->pkt.header.type = SessionHealth;

    auto body = std::make_unique<SessionHealthMsg>();
    body->frame = frame;
    body->checksum = checksum;

    message->pkt.body = std::move(body);
}

void Gekko::MessageSystem::SendNetworkHealth()
{
    u64 now = TimeSinceEpoch();

    // dont want to spam the network with network health packets
    if (_last_sent_network_check + NetStats::NET_CHECK_DELAY > now) {
        return;
    }

    _pending_output.push(std::make_unique<NetData>());
    auto& message = _pending_output.back();

    // the address and magic is set later so dont worry about it now
    message->pkt.header.type = NetworkHealth;

    auto body = std::make_unique<NetworkHealthMsg>();
    body->send_time = now;
    body->received = false;

    message->pkt.body = std::move(body);

    _last_sent_network_check = now;
}

void Gekko::MessageSystem::HandleTooFarBehindActors(bool spectator)
{
    const u64 now = TimeSinceEpoch();

	const u32 max_diff = spectator ? MAX_SPECTATOR_SEND_SIZE : MAX_PLAYER_SEND_SIZE;
	const Frame last_added = spectator ? _last_added_spectator_input : _last_added_input;

	for (auto& player : spectator ? spectators : remotes) {
		if (player->GetStatus() == Connected) {
            const u64 input_diff = now - player->stats.last_received_frame;

            if (!spectator && input_diff > NetStats::DISCONNECT_TIMEOUT) {
                // give them one chance to redeem themselves
                if (player->stats.last_received_frame == 0) {
                    player->stats.last_received_frame = now;
                    return;
                }
                session_events.AddPlayerDisconnectedEvent(player->handle);
                player->SetStatus(Disconnected);
                player->sync_num = 0;
                return;
            }

			const u32 ack_diff = last_added - player->stats.last_acked_frame;
            const u64 msg_diff = now - player->stats.last_received_message;

			if (ack_diff > max_diff || msg_diff > NetStats::DISCONNECT_TIMEOUT) {
                session_events.AddPlayerDisconnectedEvent(player->handle);
                player->SetStatus(Disconnected);
                player->sync_num = 0;
			}
		}
	}
}

u64 Gekko::MessageSystem::TimeSinceEpoch()
{
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void Gekko::MessageSystem::SendDataToAll(NetData* pkt, GekkoNetAdapter* host, bool spectators_only)
{
    auto& actors = spectators_only ? spectators : remotes;

    std::vector<u8> body_buffer;

    try {
        zpp::serializer::memory_output_archive out(body_buffer);
        out(pkt->pkt.body);
    }
    catch (const std::exception&)
    {
        printf("failed to serialize packet body\n");
        return;
    }


    for (auto& actor : actors) {
        _bin_buffer.clear();
        if (actor->address.GetSize() != 0 && actor->GetStatus() != Disconnected) {

            pkt->addr.Copy(&actor->address);
            pkt->pkt.header.magic = actor->session_magic;

            try {
                zpp::serializer::memory_output_archive out(_bin_buffer);
                out(pkt->pkt.header);
            }
            catch (const std::exception&)
            {
                printf("failed to serialize packet header\n");
                continue;
            }

            _bin_buffer.insert(
                _bin_buffer.end(),
                body_buffer.begin(),
                body_buffer.end()
            );

            auto addr = GekkoNetAddress();
            addr.data = actor->address.GetAddress();
            addr.size = actor->address.GetSize();

            host->send_data(&addr, (char*)_bin_buffer.data(), (int)_bin_buffer.size());
        }
    }
}

void Gekko::MessageSystem::SendDataTo(NetData* pkt, GekkoNetAdapter* host)
{
    _bin_buffer.clear();

    try {
        zpp::serializer::memory_output_archive out(_bin_buffer);
        out(pkt->pkt.header, pkt->pkt.body);
    }
    catch (const std::exception&)
    {
        printf("failed to serialize packet\n");
        return;
    }

    auto addr = GekkoNetAddress();
    addr.data = pkt->addr.GetAddress();
    addr.size = pkt->addr.GetSize();

    host->send_data(&addr, (char*)_bin_buffer.data(), (int)_bin_buffer.size());
}

void Gekko::MessageSystem::ParsePacket(NetAddress& addr, NetPacket& pkt)
{
    u64 now = TimeSinceEpoch();
    // update receive timers.
    std::vector<std::unique_ptr<Player>>* current = &remotes;
    for (u32 i = 0; i < 2; i++)
    {
        if (i == 1) {
            current = &spectators;
        }

        for (auto& player : *current) {
            if (player->address.Equals(addr)) {
                player->stats.last_received_message = now;
            }
        }
    }

    // handle packet.
    if (pkt.header.magic != _session_magic) {
        if (pkt.header.type == SyncRequest) {
            OnSyncRequest(addr, pkt);
        }
        else {
            printf("dropped packet!\n");
        }
    }
    else {
        switch (pkt.header.type)
        {
        case SyncResponse:
            OnSyncResponse(addr, pkt);
            return;
        case Inputs:
        case SpectatorInputs:
            OnInputs(addr, pkt);
            return;
        case InputAck:
            OnInputAck(addr, pkt);
            return;
        case SessionHealth:
            OnSessionHealth(addr, pkt);
            return;
        case NetworkHealth:
            OnNetworkHealth(addr, pkt);
            return;
        default:
            printf("cannot process an unknown event!\n");
            return;
        }
    }
}

void Gekko::MessageSystem::OnSyncRequest(NetAddress& addr, NetPacket& pkt)
{
    i32 should_send = 0;
    u64 now = TimeSinceEpoch();
    auto body = (SyncMsg*)pkt.body.get();

    // handle requests and set the peer its session magic for both remotes and spectators
    std::vector<std::unique_ptr<Player>>* current = &remotes;
    for (u32 i = 0; i < 2; i++)
    {
        if (i == 1) {
            current = &spectators;
        }

        for (auto& player : *current) {
            if (player->address.Equals(addr)) {
                player->session_magic = body->rng_data;
                if (player->sync_num == 0) {
                    player->stats.last_sent_sync_message = now;
                    should_send++;
                }
            }
        }
    }

    if (should_send > 0) {
	    // send a packet containing the local session magic
	    SendSyncResponse(&addr, body->rng_data);
    }
}

void Gekko::MessageSystem::OnSyncResponse(NetAddress& addr, NetPacket& pkt)
{
    i32 should_send = 0;
    u64 now = TimeSinceEpoch();
    auto body = (SyncMsg*)pkt.body.get();

    // handle sync responses for both remotes and spectators
    std::vector<std::unique_ptr<Player>>* current = &remotes;
    for (u32 i = 0; i < 2; i++)
    {
        if (i == 1) {
            current = &spectators;
        }

        for (auto& player : *current) {
            if (player->GetStatus() == Connected) continue;

            if (player->address.Equals(addr)) {
                player->session_magic = body->rng_data;
                if (player->sync_num < NUM_TO_SYNC) {
                    player->sync_num++;
                    should_send++;
                    player->stats.last_sent_sync_message = now;
                    session_events.AddPlayerSyncingEvent(
                        player->handle,
                        player->sync_num,
                        NUM_TO_SYNC
                    );
                    continue;
                }

                if (player->sync_num >= NUM_TO_SYNC) {
                    player->SetStatus(Connected);
                    session_events.AddPlayerConnectedEvent(player->handle);
                    continue;
                }
            }
        }
    }

    if (should_send > 0) {
    	// send a packet containing the local session magic
    	SendSyncResponse(&addr, body->rng_data);
    }
}

void Gekko::MessageSystem::OnInputs(NetAddress& addr, NetPacket& pkt)
{
    auto body = (InputMsg*)pkt.body.get();
    auto net_input = std::make_unique<NetInputData>();

    net_input->handles = GetHandlesForAddress(&addr);
    net_input->input.inputs = Compression::RLEDecode(body->inputs.data(), (u32)body->inputs.size());

    net_input->input.input_count = body->input_count;
    net_input->input.start_frame = body->start_frame;
    net_input->input.total_size = (u16)net_input->input.inputs.size();

    for (auto handle : net_input->handles) {
        auto player = GetPlayerByHandle(handle);
        if (player) {
            player->stats.last_received_frame = TimeSinceEpoch();
        }
    }

    _received_inputs.push(std::move(net_input));
}

void Gekko::MessageSystem::OnInputAck(NetAddress& addr, NetPacket& pkt)
{
    auto body = (InputAckMsg*)pkt.body.get();
    // we should just update the ack frame for all handles where the address matches
	const Frame ack_frame = body->ack_frame;
    const i32 remote_advantage = body->frame_advantage;
    bool added_advantage = false;

    std::vector<std::unique_ptr<Player>>* current = &remotes;
    for (u32 i = 0; i < 2; i++)
    {
        if (i == 1) {
            current = &spectators;
        }

        for (auto& player : *current) {
            if (player->address.Equals(addr)) {
                if (player->stats.last_acked_frame < ack_frame) {
                    player->stats.last_acked_frame = ack_frame;
                    // only add remote advantages once
                    if (!added_advantage && i == 0) {
                        history.AddRemoteAdvantage(remote_advantage);
                        added_advantage = true;
                    }
                }
            }
        }
    }
}

void Gekko::MessageSystem::OnSessionHealth(NetAddress& addr, NetPacket& pkt)
{
    auto body = (SessionHealthMsg*)pkt.body.get();

    const Frame frame = body->frame;
    const u32 checksum = body->checksum;

    for (auto& player : remotes) {
        if (player->address.Equals(addr)) {
            player->SetChecksum(frame, checksum);

            for (auto iter = player->session_health.begin();
                iter != player->session_health.end(); ) {
                if (iter->first < (_last_added_input - 100)) {
                    iter = player->session_health.erase(iter);
                } else {
                    ++iter;
                }
            }
            break;
        }
    }
}

void Gekko::MessageSystem::OnNetworkHealth(NetAddress& addr, NetPacket& pkt)
{
    auto body = (NetworkHealthMsg*)pkt.body.get();

    // ok if its not a returned packet then update it and send it back to its specifc peer.
    if (!body->received) {
        _pending_output.push(std::make_unique<NetData>());
        auto& message = _pending_output.back();

        auto player = GetPlayerByHandle(GetHandlesForAddress(&addr).at(0));

        message->pkt.header.magic = player->session_magic;
        message->pkt.header.type = NetworkHealth;

        auto new_body = std::make_unique<NetworkHealthMsg>();
        new_body->send_time = body->send_time;
        new_body->received = true;

        message->pkt.body = std::move(new_body);
        message->addr.Copy(&addr);
        return;
    }

    // else update network stats
    u16 rtt_ms = (u16)(TimeSinceEpoch() - body->send_time);
    std::vector<std::unique_ptr<Player>>* current = &remotes;

    for (u32 i = 0; i < 2; i++)
    {
        if (i == 1) {
            current = &spectators;
        }

        for (auto& actor : *current) {
            // add rtt times to a list 
            if (addr.Equals(actor->address)) {
                actor->stats.rtt.push_back(rtt_ms);
            }
            // cleanup
            if (actor->stats.rtt.size() > 10) {
                actor->stats.rtt.erase(actor->stats.rtt.begin());
            }
        }
    }
}

void Gekko::MessageSystem::AddPendingInput(bool spectator)
{
    u64 now = TimeSinceEpoch();

	const auto& send_list = spectator ? _spectator_input_send_list : _player_input_send_list;

    auto& cache = spectator ? _last_sent_spectator_input : _last_sent_input;
	const Frame last_added = spectator ? _last_added_spectator_input : _last_added_input;

    // just copy and resend the same data instead of doing calculations
    if (cache.frame != GameInput::NULL_FRAME && last_added == cache.frame) {
        // only resend if enough time has passed. we dont want to spam.
        if (cache.last_send_time + InputSendCache::INPUT_RESEND_DELAY > now) {
            return;
        }

        _pending_output.push(std::make_unique<NetData>());
        auto& message = _pending_output.back();

        message->pkt.header.type = spectator ? SpectatorInputs : Inputs;

        auto body = std::make_unique<InputMsg>();
        body->Copy(&cache.data);

        message->pkt.body = std::move(body);

        cache.last_send_time = now;
        return;
    }

	const u32 num_players = spectator ? (u32)(locals.size() + remotes.size()) : (u32)locals.size();
	const u32 total_input_size = _input_size * num_players;
	const u32 input_count = (u32)send_list.size();
	const u32 total_size = total_input_size * input_count;

    auto inputs = std::make_unique<u8[]>(total_size);

    const u32 offset_per_player = total_size / num_players;

	u32 idx = 0;
	for (auto& input : send_list) {
        // line up all players input in series.
        // this should make RLE encoding more efficent in the end.
        // before P1|P2|P1|P2 now P1|P1|P2|P2
        // i should probably not be copying this much. fix this later TODO
        if (num_players > 1) {
            for (u32 i = 0; i < num_players; i++) {
                auto dst = &inputs[(idx * _input_size) + (i * offset_per_player)];
                auto src = &input[i * _input_size];
                std::memcpy(dst, src, _input_size);
            }
        }
        else {
            std::memcpy(&inputs[idx * _input_size], input, _input_size);
        }
		idx++;
	}

    auto comp = Compression::RLEEncode(inputs.get(), total_size);

    _pending_output.push(std::make_unique<NetData>());
	auto& message = _pending_output.back();

    message->pkt.header.type = spectator ? SpectatorInputs : Inputs;

    auto body = std::make_unique<InputMsg>();

    body->total_size = (u16)comp.size();
	body->input_count = (u8)send_list.size();
	body->start_frame = last_added - (u32)send_list.size();

    body->inputs = std::move(comp);

    // save to the cache for later use.
    cache.frame = last_added;
    cache.data.Copy(body.get());
    cache.last_send_time = now;

    message->pkt.body = std::move(body);
}

void Gekko::AdvantageHistory::Init()
{
    _adv_index = 0;
	_local_frame_adv = 0;

    std::memset(_remote_frame_adv, 0, HISTORY_SIZE * sizeof(i8));
	std::memset(_local, 0, HISTORY_SIZE * sizeof(i8));
	std::memset(_remote, 0, HISTORY_SIZE * sizeof(i8));
}

void Gekko::AdvantageHistory::Update(Frame frame)
{
	const u32 update_frame = std::max(frame, 0);

	_local[update_frame % HISTORY_SIZE] = _local_frame_adv;

	i32 sum = 0;
	for (i8 num : _remote_frame_adv) {
        sum += num;
	}

    sum /= HISTORY_SIZE;
    _remote[update_frame % HISTORY_SIZE] = sum;
}

f32 Gekko::AdvantageHistory::GetAverageAdvantage()
{
	f32 sum_local = 0.f;
	f32 sum_remote = 0.f;

	for (i32 i = 0; i < HISTORY_SIZE; i++) {
		sum_local += _local[i];
		sum_remote += _remote[i];
	}

	f32 avg_local = sum_local / HISTORY_SIZE;
	f32 avg_remote = sum_remote / HISTORY_SIZE;

	// return the frames ahead
	return (avg_local - avg_remote);
}

void Gekko::AdvantageHistory::SetLocalAdvantage(i8 adv) {
	_local_frame_adv = adv;
}

void Gekko::AdvantageHistory::AddRemoteAdvantage(i8 adv) {
    _remote_frame_adv[_adv_index % HISTORY_SIZE] = adv;
    _adv_index++;
}

i8 Gekko::AdvantageHistory::GetLocalAdvantage() {
	return _local_frame_adv;
}
