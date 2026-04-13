#if NETPLAY_ENABLED

#ifndef FISTBUMP_H
#define FISTBUMP_H

#include <stdbool.h>

typedef enum {
    FISTBUMP_IDLE,
    FISTBUMP_CONNECTING,
    FISTBUMP_SENDING_TOKEN,
    FISTBUMP_LOGGING_IN,
    FISTBUMP_AWAITING_LOGIN,
    FISTBUMP_AWAITING_MATCH,
    FISTBUMP_MATCHED,
    FISTBUMP_SENDING_UDP,
    FISTBUMP_GAME_START,
    FISTBUMP_ERROR,
} FistbumpState;

typedef enum {
    FISTBUMP_CONN_IDLE,
    FISTBUMP_CONN_RESOLVING_DNS,
    FISTBUMP_CONN_CONNECTING_TCP,
    FISTBUMP_CONN_CONNECTED,
    FISTBUMP_CONN_ERROR,
} FistbumpConnectState;

typedef struct NET_DatagramSocket NET_DatagramSocket;

typedef struct {
    char match_id[37];
    int player; // 1 or 2
    char opponent_name[64];
    char ip[64];     // remote peer IP string
    int remote_port; // remote peer game port (parsed from "ip:port")
} MatchResult;

typedef struct {
    char code[9];
    char activate_url[128];
} DAG;

typedef struct {
    char token[1024];
    int expiry;
} JWT;

typedef struct {
    char username[64];
} Fistbump_Profile;

void Fistbump_Start(const char* server_ip, int tcp_port, int udp_port, const char* pref_path);
void Fistbump_Connect();
void Fistbump_Queue();
void Fistbump_CancelQueue();
void Fistbump_AcceptMatch();
void Fistbump_DeclineMatch();
void Fistbump_Run();
FistbumpState Fistbump_GetState();
FistbumpConnectState Fistbump_GetConnectState();
const MatchResult* Fistbump_GetResult();  // valid when MATCHED
NET_DatagramSocket* Fistbump_GetSocket(); // ephemeral UDP socket, valid when MATCHED
DAG Fistbump_GetDAG();
bool Fistbump_IsLoggedIn();
void Fistbump_Logout();
void Fistbump_Reset();

#endif

#endif // NETPLAY_ENABLED
