#include "port/sdl/netplay_screen.h"
#include "netplay/matchmaking.h"
#include "netplay/netplay.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

// Frames to hold the matchmaking message before switching to the next.
#define MM_TEXT_HOLD_FRAMES 30
// Frames to hold "Match found!" into the connecting phase before showing the game.
#define MATCH_FOUND_HOLD_FRAMES 90

static MatchmakingState display_state = MATCHMAKING_IDLE;
static int transition_hold = 0;
static int match_found_hold = 0;

static const char* mm_message(MatchmakingState s) {
    switch (s) {
    case MATCHMAKING_RESOLVING_DNS:
    case MATCHMAKING_CONNECTING_TCP:
    case MATCHMAKING_AWAITING_ID:
        return "Connecting to server...";

    case MATCHMAKING_SENDING_UDP:
    case MATCHMAKING_AWAITING_MATCH:
        return "Finding match...";

    case MATCHMAKING_ERROR:
        return "Matchmaking error";

    default:
        return "";
    }
}

void NetplayScreen_Render() {
    const NetplaySessionState ns = Netplay_GetSessionState();
    const MatchmakingState mm = Matchmaking_GetState();

    // While matchmaking is in progress show status text at the top of the
    // screen. This is safe at any time and doesn't require the full render pipeline.
    if (mm != MATCHMAKING_IDLE && mm != MATCHMAKING_MATCHED) {
        // Errors show immediately. Other state changes hold the current
        // message for MM_TEXT_HOLD_FRAMES before switching.
        if (mm == MATCHMAKING_ERROR) {
            display_state = mm;
            transition_hold = 0;
        } else if (mm == display_state) {
            // Do nothing
        } else if (transition_hold > 0) {
            transition_hold--;
        } else {
            display_state = mm;
            transition_hold = MM_TEXT_HOLD_FRAMES;
        }

        SSPutStrPro(1, 384, 2, 9, 0xFFFFFFFF, mm_message(display_state));
        return;
    }

    display_state = MATCHMAKING_IDLE;
    transition_hold = 0;

    // After a match is found, show "Match found!" during VS mode loading and
    // hold it briefly into the connecting phase before revealing the game.
    // this should maybe be replaced by actual visual effects but good for a prototype.
    if (ns == NETPLAY_SESSION_TRANSITIONING) {
        match_found_hold = MATCH_FOUND_HOLD_FRAMES;
    } else if (match_found_hold > 0) {
        match_found_hold--;
    } else {
        return;
    }

    const char* msg = "Match found!";
    SSPutStrPro(1, 384, 110, 9, 0xFFFFFFFF, msg);
}
