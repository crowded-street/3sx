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
void Netplay_Begin();
void Netplay_Run();
NetplaySessionState Netplay_GetSessionState();
void Netplay_HandleMenuExit();
void Netplay_GetNetworkStats(NetworkStats* stats);

#endif
