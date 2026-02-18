#include "port/sdl/netstats_renderer.h"
#include "netplay/netplay.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

#include <SDL3/SDL.h>

void NetstatsRenderer_Render() {
    if (!Netplay_IsRunning()) {
        return;
    }

    NetworkStats stats = { 0 };
    Netplay_GetNetworkStats(&stats);

    char buffer[32];
    SDL_snprintf(buffer, sizeof(buffer), "R:%d P:%d", stats.rollback, stats.ping);

    SSPutStrPro(0, 2, 2, 9, 0xFFFFFFFF, buffer);
}
