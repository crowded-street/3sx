#ifndef NETPLAY_H
#define NETPLAY_H

#include <stdbool.h>

typedef struct NetworkStats {
    int delay;
    int ping;
    int rollback;
} NetworkStats;

typedef enum NetplaySessionState {
    NETPLAY_SESSION_IDLE,
    NETPLAY_SESSION_TRANSITIONING,
    NETPLAY_SESSION_CONNECTING,
    NETPLAY_SESSION_RUNNING,
    NETPLAY_SESSION_EXITING,
} NetplaySessionState;

void Netplay_SetParams(int player, const char* ip);
void Netplay_BeginDirectP2P();
void Netplay_TickDirectP2P();
void Netplay_SetMatchmakingParams(const char* server_ip, int server_port);
void Netplay_BeginMatchmaking();
void Netplay_TickMatchmaking();
bool Netplay_IsMatchmakingPending(); // true while searching, false once matched or idle
void Netplay_CancelMatchmaking();
void Netplay_Run();
NetplaySessionState Netplay_GetSessionState();
void Netplay_HandleMenuExit();
void Netplay_GetNetworkStats(NetworkStats* stats);

#endif
