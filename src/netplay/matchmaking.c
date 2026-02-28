#include "netplay/matchmaking.h"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>
#include <stdbool.h>
#include <stdio.h>

static MatchmakingState state = MATCHMAKING_IDLE;

static NET_Address* server_addr = NULL;
static NET_StreamSocket* tcp_sock = NULL;
static NET_DatagramSocket* udp_sock = NULL;

static int saved_tcp_port = 0;
static int saved_udp_port = 0;

static char id_buf[8]; // 7-char ID + null
static char line_buf[128];
static int line_len = 0;
static int udp_retry_timer = 0;

static MatchResult match_result;

static bool pop_line(char* out, int out_size) {
    for (int i = 0; i < line_len; i++) {
        if (line_buf[i] == '\n') {
            int copy_len = (i < out_size - 1) ? i : out_size - 1;

            SDL_strlcpy(out, line_buf, copy_len + 1);

            int remaining = line_len - i - 1;
            SDL_memmove(line_buf, line_buf + i + 1, remaining);
            line_len = remaining;
            return true;
        }
    }
    return false;
}

static void read_into_line_buf() {
    int space = (int)sizeof(line_buf) - line_len - 1;

    if (space <= 0) {
        return;
    }

    int n = NET_ReadFromStreamSocket(tcp_sock, line_buf + line_len, space);

    if (n > 0) {
        line_len += n;
    }
}

void Matchmaking_Start(const char* server_ip, int tcp_port, int udp_port) {
    NET_Init();

    saved_tcp_port = tcp_port;
    saved_udp_port = udp_port;

    SDL_zeroa(id_buf);
    SDL_zeroa(line_buf);
    line_len = 0;
    udp_retry_timer = 0;
    SDL_zero(match_result);

    server_addr = NET_ResolveHostname(server_ip);
    state = MATCHMAKING_RESOLVING_DNS;
}

void Matchmaking_Run() {
    char tmp[128];

    switch (state) {
    case MATCHMAKING_RESOLVING_DNS:
        switch (NET_GetAddressStatus(server_addr)) {
        case NET_SUCCESS:
            tcp_sock = NET_CreateClient(server_addr, (Uint16)saved_tcp_port);
            if (tcp_sock == NULL) {
                printf("Matchmaking: failed to create TCP client: %s\n", SDL_GetError());
                state = MATCHMAKING_ERROR;
            } else {
                state = MATCHMAKING_CONNECTING_TCP;
            }
            break;

        case NET_FAILURE:
            printf("Matchmaking: DNS resolution failed: %s\n", SDL_GetError());
            state = MATCHMAKING_ERROR;
            break;

        case NET_WAITING:
            break;
        }
        break;

    case MATCHMAKING_CONNECTING_TCP:
        switch (NET_GetConnectionStatus(tcp_sock)) {
        case NET_SUCCESS:
            state = MATCHMAKING_AWAITING_ID;
            break;

        case NET_FAILURE:
            printf("Matchmaking: TCP connection failed: %s\n", SDL_GetError());
            state = MATCHMAKING_ERROR;
            break;

        case NET_WAITING:
            break;
        }
        break;

    case MATCHMAKING_AWAITING_ID:
        read_into_line_buf();

        if (pop_line(tmp, sizeof(tmp))) {
            SDL_strlcpy(id_buf, tmp, sizeof(id_buf));
            printf("Matchmaking: received ID: %s\n", id_buf);
            state = MATCHMAKING_SENDING_UDP;
        }
        break;

    case MATCHMAKING_SENDING_UDP:
        if (udp_sock == NULL) {
            udp_sock = NET_CreateDatagramSocket(NULL, 0);

            if (udp_sock == NULL) {
                printf("Matchmaking: failed to create UDP socket: %s\n", SDL_GetError());
                state = MATCHMAKING_ERROR;
                break;
            }
        }

        if (udp_retry_timer <= 0) {
            NET_SendDatagram(udp_sock, server_addr, (Uint16)saved_udp_port, id_buf, 7);
            udp_retry_timer = 30; // retransmit every ~0.5 seconds
        }

        udp_retry_timer--;

        // Advance when the server responds via TCP (confirms it received our UDP).
        read_into_line_buf();

        if (line_len > 0) {
            state = MATCHMAKING_AWAITING_MATCH;
        }
        break;

    case MATCHMAKING_AWAITING_MATCH:
        read_into_line_buf();

        if (pop_line(tmp, sizeof(tmp))) {
            SDL_sscanf(tmp, "%d %63[^:]:%d", &match_result.player, match_result.ip, &match_result.remote_port);
            printf("Matchmaking: player %d, matched with %s:%d\n",
                   match_result.player,
                   match_result.ip,
                   match_result.remote_port);
            state = MATCHMAKING_MATCHED;
        }
        break;

    case MATCHMAKING_MATCHED:
    case MATCHMAKING_IDLE:
    case MATCHMAKING_ERROR:
        break;
    }
}

MatchmakingState Matchmaking_GetState() {
    return state;
}

const MatchResult* Matchmaking_GetResult() {
    return &match_result;
}

NET_DatagramSocket* Matchmaking_GetSocket() {
    return udp_sock;
}

void Matchmaking_Reset() {
    if (state == MATCHMAKING_IDLE) {
        return;
    }

    if (tcp_sock != NULL) {
        NET_DestroyStreamSocket(tcp_sock);
        tcp_sock = NULL;
    }

    if (udp_sock != NULL) {
        NET_DestroyDatagramSocket(udp_sock);
        udp_sock = NULL;
    }

    if (server_addr != NULL) {
        NET_UnrefAddress(server_addr);
        server_addr = NULL;
    }

    SDL_zeroa(id_buf);
    SDL_zeroa(line_buf);
    line_len = 0;
    udp_retry_timer = 0;
    SDL_zero(match_result);

    state = MATCHMAKING_IDLE;
    NET_Quit();
}
