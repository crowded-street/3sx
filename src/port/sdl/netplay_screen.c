#if NETPLAY_ENABLED

#include "port/sdl/netplay_screen.h"
#include "main.h"
#include "platform/netplay/fistbump.h"
#include "platform/netplay/netplay.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

#include <SDL3/SDL.h>

// Frames to hold the matchmaking message before switching to the next.
#define MM_TEXT_HOLD_FRAMES 30
// Frames to hold "Match found!" into the connecting phase before showing the game.
#define MATCH_FOUND_HOLD_FRAMES 90

static FistbumpState display_state = FISTBUMP_IDLE;
static int transition_hold = 0;
static int match_found_hold = 0;

bool display_netplay_text = false;

void NetplayScreen_Render() {
    const NetplaySessionState ns = Netplay_GetSessionState();
    const FistbumpState fs = Fistbump_GetState();

    if (!display_netplay_text) {
        return;
    }

    // While matchmaking is in progress show status text at the top of the
    // screen. This is safe at any time and doesn't require the full render pipeline.
    if (fs != FISTBUMP_IDLE && fs != FISTBUMP_GAME_START) {
        // Errors show immediately. Other state changes hold the current
        // message for MM_TEXT_HOLD_FRAMES before switching.
        if (fs == FISTBUMP_ERROR) {
            display_state = fs;
            transition_hold = 0;
        } else if (fs == display_state) {
            // Do nothing
        } else if (transition_hold > 0) {
            transition_hold--;
        } else {
            display_state = fs;
            transition_hold = MM_TEXT_HOLD_FRAMES;
        }

        switch (display_state) {
        case FISTBUMP_IDLE:
            break;

        case FISTBUMP_CONNECTING:
            SSPutStrPro(1, 384, 100, 9, 0xFFFFFFFF, "Connecting to server...");
            break;

        case FISTBUMP_SENDING_TOKEN:
            break;

        case FISTBUMP_LOGGING_IN:
        case FISTBUMP_AWAITING_LOGIN:
            DAG dag = Fistbump_GetDAG();

            SSPutStrPro(1, 384, 80, 9, 0xFFFFFFFF, "Activation Code:");
            SSPutStr_Bigger(128, 96, 9, dag.code, 2.0f, 2, 0);

            SSPutStrPro(1, 384, 140, 9, 0xFFFFFFFF, "Log-in to the WebUI at:");
            SSPutStrPro(1, 384, 150, 9, 0xFFFFFFFF, dag.activate_url);

            SSPutStrPro(1, 384, 200, 9, 0xFFFFFFFF, "Press      to go back.");
            dispButtonImage2(163, 197, 0x18, 0x13, 0xF, 0, 5);

            break;

        case FISTBUMP_AWAITING_MATCH:
            SSPutStrPro(1, 384, 140, 9, 0xFFFFFFFF, "Finding match...");
            SSPutStrPro(1, 384, 160, 9, 0xFFFFFFFF, "Press      to cancel search.");
            dispButtonImage2(143, 157, 0x18, 0x13, 0xF, 0, 5);
            break;

        case FISTBUMP_MATCHED:
            MatchResult* match = Fistbump_GetResult();

            SSPutStrPro(1, 384, 130, 9, 0xFFFFFFFF, "Matched with:");
            SSPutStrPro(1, 384, 140, 9, 0xFFFFFFFF, match->opponent_name);

            SSPutStrPro(1, 384, 160, 9, 0xFFFFFFFF, "      ACCEPT      DECLINE");
            dispButtonImage2(121, 157, 0x19, 0x13, 0xF, 0, 4);
            dispButtonImage2(193, 157, 0x19, 0x13, 0xF, 0, 5);

            break;

        case FISTBUMP_SENDING_UDP:
            SSPutStrPro(1, 384, 140, 9, 0xFFFFFFFF, "Starting match...");
            break;

        case FISTBUMP_GAME_START:
        case FISTBUMP_ERROR:
            break;
        }

        return;
    }

    display_state = FISTBUMP_IDLE;
    transition_hold = 0;

    // After a match is found, show "Match start!" during VS mode loading and
    // hold it briefly into the connecting phase before revealing the game.
    // this should maybe be replaced by actual visual effects but good for a prototype.
    if (ns == NETPLAY_SESSION_TRANSITIONING) {
        match_found_hold = MATCH_FOUND_HOLD_FRAMES;
    } else if (match_found_hold > 0) {
        match_found_hold--;
    } else {
        return;
    }

    const char* msg = "Match start!";
    SSPutStrPro(1, 384, 110, 9, 0xFFFFFFFF, msg);
}

#endif // NETPLAY_ENABLED
