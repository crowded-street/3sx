#ifndef MATCHMAKING_H
#define MATCHMAKING_H

typedef enum {
    MATCHMAKING_IDLE,
    MATCHMAKING_RESOLVING_DNS,
    MATCHMAKING_CONNECTING_TCP,
    MATCHMAKING_AWAITING_ID,
    MATCHMAKING_SENDING_UDP,
    MATCHMAKING_AWAITING_MATCH,
    MATCHMAKING_MATCHED,
    MATCHMAKING_ERROR,
} MatchmakingState;

typedef struct NET_DatagramSocket NET_DatagramSocket;

typedef struct {
    int player;        // 1 or 2
    char ip[64];       // remote peer IP string
    int remote_port;   // remote peer game port (parsed from "ip:port")
} MatchResult;

void Matchmaking_Start(const char* server_ip, int tcp_port, int udp_port);
void Matchmaking_Run();
MatchmakingState Matchmaking_GetState();
const MatchResult* Matchmaking_GetResult();  // valid when MATCHED
NET_DatagramSocket* Matchmaking_GetSocket(); // ephemeral UDP socket, valid when MATCHED
void Matchmaking_Reset();

#endif
