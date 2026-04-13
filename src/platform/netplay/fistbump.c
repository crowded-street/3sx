#if NETPLAY_ENABLED

#include "platform/netplay/fistbump.h"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static FistbumpState state = FISTBUMP_IDLE;
static FistbumpConnectState connect_state = FISTBUMP_CONN_IDLE;

static NET_Address* server_addr = NULL;
static NET_StreamSocket* tcp_sock = NULL;
static NET_DatagramSocket* udp_sock = NULL;

static int saved_tcp_port = 0;
static int saved_udp_port = 0;

static char id_buf[8]; // 7-char ID + null
static char line_buf[1024];
static int line_len = 0;
static int udp_retry_timer = 0;

static const char* base_path = NULL;

static DAG dag;
static JWT refresh_token;
static Fistbump_Profile profile;
static MatchResult match_result;

static void SaveToken(const JWT* jwt) {
    if (base_path == NULL || jwt == NULL) {
        return;
    }

    char path[512];
    SDL_snprintf(path, sizeof(path), "%s/token", base_path);

    SDL_IOStream* stream = SDL_IOFromFile(path, "w");
    if (!stream) {
        return;
    }

    SDL_IOprintf(stream, "%s\n%d\n", jwt->token, jwt->expiry);
    SDL_CloseIO(stream);
}

static bool LoadToken(JWT* jwt) {
    if (base_path == NULL || jwt == NULL) {
        return false;
    }

    char path[512];
    SDL_snprintf(path, sizeof(path), "%s/token", base_path);

    SDL_IOStream* stream = SDL_IOFromFile(path, "r");
    if (!stream) {
        return false;
    }

    char line[256];

    if (!SDL_ReadIO(stream, line, sizeof(line) - 1)) {
        SDL_CloseIO(stream);
        return false;
    }

    line[sizeof(line) - 1] = '\0';

    SDL_strlcpy(jwt->token, line, sizeof(jwt->token));

    size_t len = strcspn(jwt->token, "\r\n");
    jwt->token[len] = '\0';

    if (!SDL_ReadIO(stream, line, sizeof(line) - 1)) {
        SDL_CloseIO(stream);
        return false;
    }

    line[sizeof(line) - 1] = '\0';

    char* endptr = NULL;
    long expiry = strtol(line, &endptr, 10);

    if (endptr == line) {
        SDL_CloseIO(stream);
        return false;
    }

    jwt->expiry = (time_t)expiry;

    SDL_CloseIO(stream);

    time_t now = time(NULL);
    if (jwt->expiry <= now) {
        remove(path);
        return false;
    }

    return true;
}

static void DeleteToken() {
    if (!base_path) {
        return;
    }

    char path[512];
    SDL_snprintf(path, sizeof(path), "%s/token", base_path);

    remove(path);
}

static bool pop_line(char* out, int out_size) {
    for (int i = 0; i < line_len; i++) {
        if (line_buf[i] == '\n') {

            int len = i;
            if (len >= out_size)
                len = out_size - 1;

            memcpy(out, line_buf, len);
            out[len] = '\0';

            int remaining = line_len - (i + 1);
            memmove(line_buf, line_buf + i + 1, remaining);
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

void Fistbump_Start(const char* server_ip, int tcp_port, int udp_port, const char* pref_path) {
    NET_Init();

    saved_tcp_port = tcp_port;
    saved_udp_port = udp_port;

    SDL_zeroa(id_buf);
    SDL_zeroa(line_buf);
    line_len = 0;
    udp_retry_timer = 0;
    SDL_zero(match_result);

    base_path = pref_path;

    server_addr = NET_ResolveHostname(server_ip);
    state = FISTBUMP_CONNECTING;
    connect_state = FISTBUMP_CONN_RESOLVING_DNS;
}

void Fistbump_Connect() {
    switch (connect_state) {
    case FISTBUMP_CONN_IDLE:
        break;

    case FISTBUMP_CONN_RESOLVING_DNS:
        switch (NET_GetAddressStatus(server_addr)) {
        case NET_SUCCESS:
            tcp_sock = NET_CreateClient(server_addr, (Uint16)saved_tcp_port);
            if (tcp_sock == NULL) {
                SDL_Log("Fistbump: failed to create TCP client: %s\n", SDL_GetError());
                connect_state = FISTBUMP_CONN_ERROR;
            } else {
                connect_state = FISTBUMP_CONN_CONNECTING_TCP;
            }
            break;

        case NET_FAILURE:
            SDL_Log("Fistbump: DNS resolution failed: %s\n", SDL_GetError());
            state = FISTBUMP_ERROR;
            break;

        case NET_WAITING:
            break;
        }
        break;

    case FISTBUMP_CONN_CONNECTING_TCP:
        switch (NET_GetConnectionStatus(tcp_sock)) {
        case NET_SUCCESS:
            connect_state = FISTBUMP_CONN_CONNECTED;
            break;

        case NET_FAILURE:
            SDL_Log("Fistbump: TCP connection failed: %s\n", SDL_GetError());
            connect_state = FISTBUMP_CONN_ERROR;
            break;

        case NET_WAITING:
            break;
        }
        break;

    case FISTBUMP_CONN_CONNECTED:
        break;

    case FISTBUMP_CONN_ERROR:
        state = FISTBUMP_ERROR;
        break;
    }
}

void Fistbump_Login() {
    state = FISTBUMP_LOGGING_IN;
    char buf[1024];

    if (LoadToken(&refresh_token)) {
        SDL_snprintf(buf, sizeof(buf), "REFRESH %s\n", refresh_token.token);
        NET_WriteToStreamSocket(tcp_sock, buf, SDL_strlen(buf));
    } else {
        SDL_snprintf(buf, sizeof(buf), "HELLO\n");
        NET_WriteToStreamSocket(tcp_sock, buf, SDL_strlen(buf));
    }
}

void Fistbump_Queue() {
    state = FISTBUMP_AWAITING_MATCH;

    char buf[128];
    SDL_snprintf(buf, sizeof(buf), "QUEUE add\n");
    NET_WriteToStreamSocket(tcp_sock, buf, SDL_strlen(buf));
}

void Fistbump_CancelQueue() {
    state = FISTBUMP_IDLE;

    char buf[128];
    SDL_snprintf(buf, sizeof(buf), "QUEUE remove\n");
    NET_WriteToStreamSocket(tcp_sock, buf, SDL_strlen(buf));
}

void Fistbump_BeginUDP() {
    state = FISTBUMP_SENDING_UDP;
    udp_retry_timer = 0;
}

void Fistbump_SendUDP() {
    if (udp_sock == NULL) {
        udp_sock = NET_CreateDatagramSocket(NULL, 0);

        if (udp_sock == NULL) {
            SDL_Log("Fistbump: failed to create UDP socket: %s\n", SDL_GetError());
            state = FISTBUMP_ERROR;
            return;
        }
    }

    if (udp_retry_timer <= 0) {
        char buf[128];
        SDL_snprintf(buf, sizeof(buf), "%s %s", id_buf, match_result.match_id);
        NET_SendDatagram(udp_sock, server_addr, (Uint16)saved_udp_port, buf, SDL_strlen(buf));
        udp_retry_timer = 30; // retransmit every ~0.5 seconds
    }

    udp_retry_timer--;
}

void Fistbump_AcceptMatch() {
    Fistbump_BeginUDP();
}

void Fistbump_DeclineMatch() {
    char buf[128];
    SDL_snprintf(buf, sizeof(buf), "DECLINE %s\n", match_result.match_id);
    NET_WriteToStreamSocket(tcp_sock, buf, SDL_strlen(buf));
}

void Fistbump_HandleSESSION(const char* line) {
    SDL_sscanf(line, "SESSION %7s", id_buf);
    SDL_Log("Fistbump: received ID: %s\n", id_buf);

    state = FISTBUMP_SENDING_TOKEN;
}

void Fistbump_HandleDAG(const char* line) {
    SDL_sscanf(line, "DAG %8s %127s", dag.code, dag.activate_url);
    SDL_Log("Fistbump: DAG %s, login at %s\n", dag.code, dag.activate_url);

    state = FISTBUMP_AWAITING_LOGIN;
}

void Fistbump_HandleUDP(const char* line) {
    char res[8];

    SDL_sscanf(line, "UDP %7s", res);

    if (strcmp(res, "ok") == 0) {
        SDL_Log("Fistbump: UDP ok!\n");
    }
}

void Fistbump_HandleTOKEN(const char* line) {
    char token[1024];
    int expiry;

    if (sscanf(line, "TOKEN refresh %1023s %d", token, &expiry) == 2) {
        SDL_strlcpy(refresh_token.token, token, sizeof(refresh_token.token));
        refresh_token.expiry = expiry;
        SaveToken(&refresh_token);
        state = FISTBUMP_IDLE;
    }
}

void Fistbump_HandlePROFILE(const char* line) {
    SDL_sscanf(line, "PROFILE %7s", profile.username);
    SDL_Log("Fistbump: Logged in as %s\n", profile.username);
}

void Fistbump_HandleMATCH(const char* line) {
    SDL_sscanf(line, "MATCH %36s %63s", match_result.match_id, match_result.opponent_name);
    SDL_Log("Fistbump: matched with %s\n", match_result.opponent_name);

    state = FISTBUMP_MATCHED;
}

void Fistbump_HandleCANCEL(const char* line) {
    char match_id[37];

    if (SDL_sscanf(line, "CANCEL %36s", match_id) != 1) {
        SDL_Log("Fistbump: failed to parse CANCEL\n");
        return;
    }

    if (strcmp(match_id, match_result.match_id) != 0) {
        return;
    }

    SDL_Log("Fistbump: match cancelled\n");
    SDL_zero(match_result);

    if (state == FISTBUMP_MATCHED) {
        state = FISTBUMP_IDLE;
    }
}

void Fistbump_HandleSTART(const char* line) {
    SDL_sscanf(line, "START %d %63[^:]:%d", &match_result.player, match_result.ip, &match_result.remote_port);
    SDL_Log(
        "Fistbump: player %d, opponent IP: %s:%d\n", match_result.player, match_result.ip, match_result.remote_port);

    state = FISTBUMP_GAME_START;
}

void Fistbump_ParseCommand(const char* line) {
    if (strncmp(line, "SESSION ", 8) == 0) {
        Fistbump_HandleSESSION(line);
    } else if (strncmp(line, "DAG ", 4) == 0) {
        Fistbump_HandleDAG(line);
    } else if (strncmp(line, "UDP ", 4) == 0) {
        Fistbump_HandleUDP(line);
    } else if (strncmp(line, "TOKEN ", 6) == 0) {
        Fistbump_HandleTOKEN(line);
    } else if (strncmp(line, "PROFILE ", 8) == 0) {
        Fistbump_HandlePROFILE(line);
    } else if (strncmp(line, "MATCH ", 6) == 0) {
        Fistbump_HandleMATCH(line);
    } else if (strncmp(line, "CANCEL ", 7) == 0) {
        Fistbump_HandleCANCEL(line);
    } else if (strncmp(line, "START ", 6) == 0) {
        Fistbump_HandleSTART(line);
    }
}

void Fistbump_Run() {
    char tmp[1024];

    read_into_line_buf();

    while (pop_line(tmp, sizeof(tmp))) {
        Fistbump_ParseCommand(tmp);
    }

    switch (state) {
    case FISTBUMP_IDLE:
    case FISTBUMP_CONNECTING:
        Fistbump_Connect();
        break;

    case FISTBUMP_SENDING_TOKEN:
        Fistbump_Login();
        break;

    case FISTBUMP_LOGGING_IN:
    case FISTBUMP_AWAITING_LOGIN:
    case FISTBUMP_AWAITING_MATCH:
    case FISTBUMP_MATCHED:
        break;

    case FISTBUMP_SENDING_UDP:
        Fistbump_SendUDP();
        break;

    case FISTBUMP_GAME_START:
    case FISTBUMP_ERROR:
        break;
    }
}

FistbumpState Fistbump_GetState() {
    return state;
}

FistbumpConnectState Fistbump_GetConnectState() {
    return connect_state;
}

const MatchResult* Fistbump_GetResult() {
    return &match_result;
}

NET_DatagramSocket* Fistbump_GetSocket() {
    return udp_sock;
}

DAG Fistbump_GetDAG() {
    return dag;
}

bool Fistbump_IsLoggedIn() {
    return strcmp(profile.username, "") != 0;
}

void Fistbump_Logout() {
    DeleteToken();
    Fistbump_Reset();
}

void Fistbump_Reset() {
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

    state = FISTBUMP_IDLE;
    memset(&profile, 0, sizeof(profile));
    NET_Quit();
}

#endif
