// Linked against the Release game's objects; entry point selected by the runner.
#include "platform/netplay/netplay.h"
#include "platform/netplay/fistbump.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include <SDL3/SDL.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char* test_path;
const char* Paths_GetPrefPath(void) { return test_path; }
const char* Paths_GetBasePath(void) { return test_path; }
SDL_Storage* Paths_OpenUserStorage(SDL_PropertiesID props) { (void)props; return NULL; }

int arcade_test_main(int argc, char** argv) {
    assert(argc == 4);
    test_path = argv[3];
    assert(SDL_Init(0));
    Mode_Type = MODE_ARCADE;
    G_No[1] = 2; // An active CPU fight must not change before the reset handoff.
    Netplay_SetMatchmakingParams("127.0.0.1", atoi(argv[1]));
    Netplay_BeginArcadeMatchmaking();
    Netplay_BeginArcadeMatchmaking(); // Idempotent: must not open another socket.
    assert(Netplay_IsArcadeMatchmaking());
    const Uint64 deadline = SDL_GetTicks() + 5000;
    bool completed = false;
    while (SDL_GetTicks() < deadline) {
        Netplay_TickMatchmaking();
        if (strcmp(argv[2], "start") == 0 &&
            Netplay_GetSessionState() == NETPLAY_SESSION_ARCADE_RESETTING) {
            assert(G_No[1] == 2);
            assert(Fistbump_GetState() == FISTBUMP_GAME_START);
            assert(Fistbump_GetSocket() != NULL);
            completed = true;
            break;
        }
        if (strcmp(argv[2], "error") == 0 && !Netplay_IsArcadeMatchmaking()) {
            assert(G_No[1] == 2 && Mode_Type == MODE_ARCADE);
            assert(Netplay_GetSessionState() == NETPLAY_SESSION_IDLE);
            completed = true;
            break;
        }
        if (strcmp(argv[2], "leave") == 0 && Fistbump_GetState() == FISTBUMP_AWAITING_MATCH) {
            Mode_Type = MODE_VERSUS;
            Netplay_TickMatchmaking();
            assert(!Netplay_IsArcadeMatchmaking());
            completed = true;
            break;
        }
        SDL_Delay(2);
    }
    assert(completed);
    Netplay_CancelMatchmaking();
    assert(!Netplay_IsArcadeMatchmaking());
    if (Netplay_GetSessionState() == NETPLAY_SESSION_EXITING) {
        Netplay_Run();
    }
    assert(Netplay_GetSessionState() == NETPLAY_SESSION_IDLE);
    Netplay_SetParams(1, "127.0.0.1");
    Netplay_BeginArcadeMatchmaking();
    assert(!Netplay_IsArcadeMatchmaking());
    SDL_Quit();
    puts("ARCADE_MATCHMAKING_SMOKE_OK");
    return 0;
}
