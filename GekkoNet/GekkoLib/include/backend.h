#pragma once

#include "gekkonet.h"

#include "gekko_types.h"
#include "net.h"
#include "event.h"

#include <memory>
#include <list>
#include <vector>
#include <queue>
#include <chrono>
#include <map>

namespace Gekko {

    enum PlayerStatus {
        Initiating,
        Connected,
        Disconnected,
    };

	class Player
	{
	public:
		Player(Handle phandle, GekkoPlayerType type, NetAddress* addr, u32 magic = 0);

        GekkoPlayerType GetType();

		PlayerStatus GetStatus();

		void SetStatus(PlayerStatus type);

        void SetChecksum(Frame frame, u32 checksum);

	public:
		Handle handle;

		u8 sync_num;

		u16 session_magic;

		NetStats stats;

		NetAddress address;

        std::map<Frame, u32> session_health;

	private:
        GekkoPlayerType _type;

		PlayerStatus _status;
	};

	struct AdvantageHistory {
	public:
		void Init();

		void Update(Frame frame);

		f32 GetAverageAdvantage();

		i8 GetLocalAdvantage();

		void SetLocalAdvantage(i8 adv);

		void AddRemoteAdvantage(i8 adv);

	private:
		static const i32 HISTORY_SIZE = 26;

        u8 _adv_index;

        i8 _local_frame_adv;

		i8 _local[HISTORY_SIZE];

		i8 _remote[HISTORY_SIZE];

        i8 _remote_frame_adv[HISTORY_SIZE];
	};

	class MessageSystem {
	public:
		MessageSystem();

		void Init(u32 input_size);

		void AddInput(Frame input_frame, u8 input[]);

		void AddSpectatorInput(Frame input_frame, u8 input[]);

		void SendPendingOutput(GekkoNetAdapter* host);

		void HandleData(GekkoNetAdapter* host, GekkoNetResult** data, u32 length);

        std::queue<std::unique_ptr<NetInputData>>& LastReceivedInputs();

		void SendInputAck(Handle player, Frame frame);

		Frame GetLastAddedInput(bool spectator = false);

		bool CheckStatusActors();

        void SendSessionHealth(Frame frame, u32 checksum);

        void SendNetworkHealth();

	public:
		std::vector<std::unique_ptr<Player>> locals;

		std::vector<std::unique_ptr<Player>> remotes;

		std::vector<std::unique_ptr<Player>> spectators;

		AdvantageHistory history;

        SessionEventSystem session_events;

        std::map<Frame, u32> local_health;

	private:
		void SendSyncRequest(NetAddress* addr);

		void SendSyncResponse(NetAddress* addr, u16 magic);

		void AddPendingInput(bool spectator = false);

		std::vector<Handle> GetHandlesForAddress(NetAddress* addr);

		Player* GetPlayerByHandle(Handle handle);

		Frame GetMinLastAckedFrame(bool spectator = false);

		void HandleTooFarBehindActors(bool spectator = false);

		u64 TimeSinceEpoch();

        void SendDataToAll(NetData* pkt, GekkoNetAdapter* host, bool spectators_only = false);

        void SendDataTo(NetData* pkt, GekkoNetAdapter* host);

        void ParsePacket(NetAddress& addr, NetPacket& pkt);

        void OnSyncRequest(NetAddress& addr, NetPacket& pkt);

        void OnSyncResponse(NetAddress& addr, NetPacket& pkt);

        void OnInputs(NetAddress& addr, NetPacket& pkt);

        void OnInputAck(NetAddress& addr, NetPacket& pkt);

        void OnSessionHealth(NetAddress& addr, NetPacket& pkt);

        void OnNetworkHealth(NetAddress& addr, NetPacket& pkt);

	private:
		const u32 MAX_PLAYER_SEND_SIZE = 64;
		const u32 MAX_SPECTATOR_SEND_SIZE = 64;
	    const u32 NUM_TO_SYNC = 4;

		u32 _input_size;

		u16 _session_magic;

		Frame _last_added_input;

		Frame _last_added_spectator_input;

		std::list<u8*> _player_input_send_list;

		std::list<u8*> _spectator_input_send_list;

		std::queue<std::unique_ptr<NetData>> _pending_output;

		std::queue<std::unique_ptr<NetInputData>> _received_inputs;

        std::vector<u8> _bin_buffer;

        struct InputSendCache {
            static const u64 INPUT_RESEND_DELAY = std::chrono::milliseconds(200).count();

            u64 last_send_time = 0;
            Frame frame = -1;
            InputMsg data;
        };

        InputSendCache _last_sent_input;

        InputSendCache _last_sent_spectator_input;

        u64 _last_sent_network_check;
	};
}
