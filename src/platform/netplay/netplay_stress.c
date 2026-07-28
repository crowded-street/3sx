#if NETPLAY_ENABLED

#include "platform/netplay/netplay_stress.h"
#include "core/app.h"
#include "main.h"
#include "platform/netplay/netplay_base.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "types.h"

#include <SDL3/SDL.h>

#include <stdarg.h>
#include <stdbool.h>

#define STRESS_CHECK_DISTANCE_DEFAULT 8
#define GAME_STATE_FIGHT 2     // G_No[1] while a round is being played
#define STRESS_PAD_CONNECTED 2 // Interface_Type value keyConvert() uses for a present pad

static bool stress_pending = false;
static bool stress_running = false;
static bool stress_finished = false;
static bool stress_waiting_for_match = false;
static u32 stress_rng = 0;
static int stress_check_distance = 0;
static int stress_frame_limit = 0;
static int stress_frames_run = 0;
static int stress_desyncs = 0;
static int stress_boot_timer = 0;
static const char* stress_out_dir = ".";

// Only the first re-simulation that disagrees is worth keeping.
static bool stress_pair_dumped = false;

void Stress_Path(char* dst, size_t size, const char* relative) {
    SDL_snprintf(dst, size, "%s/%s", stress_out_dir, relative);
}

/// A stress run is unattended, so its trace goes to a file rather than stdout.
void Stress_Trace(const char* fmt, ...) {
    char path[512];
    Stress_Path(path, sizeof(path), "stress-trace.log");

    SDL_IOStream* io = SDL_IOFromFile(path, "a");

    if (io == NULL) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    SDL_IOvprintf(io, fmt, args);
    va_end(args);

    SDL_IOprintf(io, "\n");
    SDL_CloseIO(io);
}

static u32 stress_random() {
    // xorshift32, so a seed reproduces the exact same run.
    stress_rng ^= stress_rng << 13;
    stress_rng ^= stress_rng >> 17;
    stress_rng ^= stress_rng << 5;
    return stress_rng;
}

/// Holds presses for a few frames at a time, which exercises moves and cancels
/// rather than one-frame noise.
u16 Stress_NextInput(int player) {
    static const u16 directions[] = { 0,
                                      SWK_UP,
                                      SWK_DOWN,
                                      SWK_LEFT,
                                      SWK_RIGHT,
                                      SWK_UP | SWK_LEFT,
                                      SWK_UP | SWK_RIGHT,
                                      SWK_DOWN | SWK_LEFT,
                                      SWK_DOWN | SWK_RIGHT };

    static const u16 attacks[] = { SWK_WEST,          SWK_NORTH, SWK_RIGHT_SHOULDER,
                                   SWK_LEFT_SHOULDER, SWK_SOUTH, SWK_RIGHT_TRIGGER };

    static u16 held[PLAYER_COUNT] = { 0, 0 };
    static int hold_frames[PLAYER_COUNT] = { 0, 0 };

    if (hold_frames[player] > 0) {
        hold_frames[player] -= 1;
        return held[player];
    }

    u16 input = directions[stress_random() % SDL_arraysize(directions)];

    if (stress_random() % 3 == 0) {
        input |= attacks[stress_random() % SDL_arraysize(attacks)];
    }

    held[player] = input;
    hold_frames[player] = (int)(stress_random() % 6);

    return input;
}

void Stress_SetOutputDir(const char* directory) {
    if (directory != NULL) {
        stress_out_dir = directory;
    }
}

void Stress_Begin(int seed, int check_distance, int frames) {
    // A zero seed would make xorshift produce nothing but zeroes.
    stress_rng = seed != 0 ? (u32)seed : 1;

    stress_check_distance = check_distance > 0 ? check_distance : STRESS_CHECK_DISTANCE_DEFAULT;
    stress_frame_limit = frames;
    stress_pending = true;

#if DEBUG
    // Both simulations of a frame have to still be in the buffer when the desync
    // is reported, which is check_distance frames after the fact.
    if (stress_check_distance >= STATE_BUFFER_MAX) {
        stress_check_distance = STATE_BUFFER_MAX - 1;
        SDL_Log("Clamped stress check distance to %d.", stress_check_distance);
    }
#endif
}

bool Stress_IsRequested() {
    return stress_pending || stress_waiting_for_match || stress_running;
}

bool Stress_IsRunning() {
    return stress_running;
}

bool Stress_IsFinished() {
    return stress_finished;
}

void Stress_CreateSession(GekkoConfig* config, GekkoSession** session, int* player_handle) {
    config->check_distance = stress_check_distance;

    if (!gekko_create(session, GekkoStressSession)) {
        SDL_Log("Session is already running! probably incorrect.");
        return;
    }

    gekko_start(*session, config);

    for (int i = 0; i < PLAYER_COUNT; i++) {
        gekko_add_actor(*session, GekkoLocalPlayer, NULL);
    }

    *player_handle = 0;

    Stress_Trace(
        "session created (seed %u, check distance %d, frame limit %d)",
        stress_rng,
        stress_check_distance,
        stress_frame_limit
    );
}

static bool game_is_in_a_fight() {
    return G_No[1] == GAME_STATE_FIGHT;
}

void Stress_InjectBootInput() {
    if (!Stress_IsRequested()) {
        return;
    }

    // Both players are simulated, but only port 1 has real hardware behind it.
    // keyConvert() clears the other port every frame, which makes the game ask
    // for a controller to be reconnected mid-match.
    Interface_Type[0] = STRESS_PAD_CONNECTED;
    Interface_Type[1] = STRESS_PAD_CONNECTED;

    if (stress_running) {
        // The session drives the inputs from here on.
        return;
    }

    stress_boot_timer += 1;

    if (stress_boot_timer % 60 == 0) {
        Stress_Trace(
            "driving menus, frame %d: menu cond %d, G_No %d/%d/%d",
            stress_boot_timer,
            task[TASK_MENU].condition,
            G_No[0],
            G_No[1],
            G_No[2]
        );
    }

    if (stress_pending) {
        // Tap rather than hold: the screens react to a press, not to the button
        // being down, and holding it would only ever produce one edge.
        if ((stress_boot_timer / 4) % 2 == 0) {
            p1sw_buff |= SWK_START;
        }

        return;
    }

    p1sw_buff |= Stress_NextInput(0);
    p2sw_buff |= Stress_NextInput(1);

    if ((stress_boot_timer / 8) % 2 == 0) {
        p1sw_buff |= SWK_START;
        p2sw_buff |= SWK_START;
    }
}

void Stress_Tick() {
    if (stress_pending) {
        if (task[TASK_MENU].condition != 1) {
            return;
        }

        Stress_Trace("entering versus mode from G_No %d/%d/%d", G_No[0], G_No[1], G_No[2]);

        stress_pending = false;
        stress_waiting_for_match = true;
        NetplayBase_SetupVsMode();

        // Character select is driven by the pads, not by the session, so run it
        // as a local versus match.
        Mode_Type = MODE_VERSUS;
        return;
    }

    if (!stress_waiting_for_match || !game_is_in_a_fight()) {
        return;
    }

    // Starting at the fight keeps the rollback away from character select and
    // stage loading, which are far too expensive to re-simulate every frame.
    Stress_Trace("fight reached, creating session at G_No %d/%d/%d", G_No[0], G_No[1], G_No[2]);

    stress_waiting_for_match = false;
    stress_running = true;

    // Both sides have to read their lever from the session. A CPU-controlled
    // player derives it from cpu_algorithm() instead, whose state isn't part of
    // the saved State, so it would diverge on every rollback.
    for (int i = 0; i < PLAYER_COUNT; i++) {
        Operator_Status[i] = 1;
        plw[i].wu.operator = 1;
    }

    NetplayBase_StartStressSession();
}

void Stress_OnFrameAdvanced() {
    stress_frames_run += 1;

    if (!game_is_in_a_fight()) {
        // The texture cache is not part of the saved State, and the screens after
        // a match release and reload it. Rolling back across that boundary makes
        // the game draw a player from an MTS slot that has since been freed.
        Stress_Trace("exiting: match ended after %d frames, %d desync(s)", stress_frames_run, stress_desyncs);
        stress_finished = true;
        App_Exit();
        return;
    }

    if (stress_frames_run % 60 == 0) {
        Stress_Trace("stress frame %d (%d desync(s))", stress_frames_run, stress_desyncs);
    }

    if (stress_frame_limit > 0 && stress_frames_run >= stress_frame_limit) {
        Stress_Trace("exiting: frame limit reached after %d frames, %d desync(s)", stress_frames_run, stress_desyncs);
        stress_finished = true;
        App_Exit();
    }
}

void Stress_OnDesync(int frame) {
    stress_desyncs += 1;
    Stress_Trace("desync at frame %d after %d frames", frame, stress_frames_run);

#if DEBUG
    if (!stress_pair_dumped) {
        // The divergence was outside the window the save hook watches.
        stress_pair_dumped = true;
        NetplayBase_DumpDesyncPair(frame);
    }
#endif

    // The dumps are left for compare_states.py.
    Stress_Trace("exiting: desync found after %d frames", stress_frames_run);
    stress_finished = true;
    App_Exit();
}

void Stress_OnResimulationDiverged(int frame, u32 first_checksum, u32 resim_checksum) {
#if DEBUG
    if (stress_pair_dumped) {
        return;
    }

    // Catch the divergence here rather than when the session reports it: by then
    // this frame has been re-simulated several more times and the buffer may
    // hold a pass that happens to agree.
    stress_pair_dumped = true;
    NetplayBase_DumpDesyncPair(frame);
    Stress_Trace("re-simulation of frame %d diverged (0x%X vs 0x%X)", frame, first_checksum, resim_checksum);
#endif
}

#endif // NETPLAY_ENABLED
