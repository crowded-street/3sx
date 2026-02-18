#ifndef NETPLAY_H
#define NETPLAY_H

#include <stdbool.h>

typedef struct NetworkStats {
    int delay;
    int ping;
    int rollback;
} NetworkStats;

void Netplay_SetParams(int player, const char* ip);
void Netplay_Begin();
void Netplay_Run();
bool Netplay_IsRunning();
void Netplay_HandleMenuExit();
void Netplay_GetNetworkStats(NetworkStats* stats);

#endif
