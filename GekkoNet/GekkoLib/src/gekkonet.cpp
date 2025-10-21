#include "gekkonet.h"
#include "gekko.h"

bool gekko_create(GekkoSession** session)
{
    if (*session) {
        return false;
    }

    *session = new Gekko::Session();
    return true;
}

bool gekko_destroy(GekkoSession* session)
{
    if (session) {
        delete session;
        return true;
    }

    return false;
}

void gekko_start(GekkoSession* session, GekkoConfig* config)
{
    session->Init(config);
}

void gekko_net_adapter_set(GekkoSession* session, GekkoNetAdapter* adapter)
{
    session->SetNetAdapter(adapter);
}

int gekko_add_actor(GekkoSession* session, GekkoPlayerType player_type, GekkoNetAddress* addr)
{
    return session->AddActor(player_type, !addr ? nullptr : addr);
}

void gekko_set_local_delay(GekkoSession* session, int player, unsigned char delay)
{
    session->SetLocalDelay(player, delay);
}

void gekko_add_local_input(GekkoSession* session, int player, void* input)
{
    session->AddLocalInput(player, input);
}

GekkoGameEvent** gekko_update_session(GekkoSession* session, int* count)
{
    return session->UpdateSession(count);
}

GekkoSessionEvent** gekko_session_events(GekkoSession* session, int* count)
{
    return session->Events(count);
}

float gekko_frames_ahead(GekkoSession* session)
{
    return session->FramesAhead();
}

void gekko_network_stats(GekkoSession* session, int player, GekkoNetworkStats* stats)
{
    session->NetworkStats(player, stats);
}

void gekko_network_poll(GekkoSession* session)
{
    session->NetworkPoll();
}

#ifndef GEKKONET_NO_ASIO

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#endif // _WIN32

#define ASIO_STANDALONE

#include "asio/asio.hpp"
#include <iostream>

static char _buffer[1024];
static asio::error_code _ec;
static asio::io_context _io_ctx;
static asio::ip::udp::endpoint _remote;
static std::vector<GekkoNetResult*> _results;
static asio::ip::udp::socket* _socket = nullptr;

static asio::ip::udp::endpoint STOE(const std::string& str) {
    std::string::size_type colon_pos = str.find(':');
    if (colon_pos == std::string::npos) {
        throw std::invalid_argument("Invalid endpoint string");
    }

    std::string address = str.substr(0, colon_pos);
    u16 port = (u16)(std::stoi(str.substr(colon_pos + 1)));

    return asio::ip::udp::endpoint(asio::ip::address::from_string(address), port);
}


static void asio_send(GekkoNetAddress* addr, const char* data, int length) {
    std::string address((char*)addr->data, addr->size);

    auto endpoint = STOE(address);

    _socket->send_to(asio::buffer(data, length), endpoint, 0, _ec);

    if (_ec) {
        std::cerr << "send failed: " << _ec.message() << std::endl;
    }
}

static GekkoNetResult** asio_receive(int* length) {
    _results.clear();

    while (true) {
        const u32 len = (u32)_socket->receive_from(asio::buffer(_buffer), _remote, 0, _ec);
        if (_ec && _ec != asio::error::would_block) {
            // std::cout << "receive failed: " << _ec.message() << std::endl;
            continue;
        }
        else if (!_ec) {
            std::string endpoint = _remote.address().to_string() + ":" + std::to_string(_remote.port());

            GekkoNetResult* res = reinterpret_cast<GekkoNetResult*>(std::malloc(sizeof(*res)));
            _results.push_back(res);

            res->addr.data = std::malloc(endpoint.size());
            res->addr.size = (u32)endpoint.size();
            std::memcpy(res->addr.data, endpoint.c_str(), res->addr.size);

            res->data_len = len;
            res->data = std::malloc(len);

            std::memcpy(res->data, _buffer, len);
        }
        else {
            break;
        }
    }

    *length = (int)_results.size();

    return _results.data();
}

static void asio_free(void* data_ptr) {
    std::free(data_ptr);
}

static GekkoNetAdapter default_sock{
    asio_send,
    asio_receive,
    asio_free
};

GekkoNetAdapter* gekko_default_adapter(unsigned short port) {
    // in case this has been called before.
    if (_socket) {
        try {
            _socket->shutdown(asio::socket_base::shutdown_type::shutdown_both);
            _socket->close();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }

        delete _socket;
        _socket = nullptr;
    }

    // setup socket
    _socket = new asio::ip::udp::socket(_io_ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), port));
    _socket->non_blocking(true);

    return &default_sock;
}

#endif // GEKKONET_NO_ASIO
