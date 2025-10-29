#include "sf33rd/Source/Game/netplay.h"
#include "sf33rd/Source/Game/Game.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/main.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "types.h"

#include <stdbool.h>

#include "gekkonet.h"
#include <SDL3/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#define INPUT_HISTORY_MAX 120

// Uncomment to enable packet drops
// #define LOSSY_ADAPTER

typedef enum SessionState {
    SESSION_IDLE,
    SESSION_TRANSITIONING,
    SESSION_CONNECTING,
    SESSION_RUNNING,
} SessionState;

static GekkoSession* session = NULL;
static unsigned short local_port = 0;
static unsigned short remote_port = 0;
static int player_number = 0;
static int player_handle = 0;
static SessionState session_state = SESSION_IDLE;
static u16 input_history[2][INPUT_HISTORY_MAX] = { 0 };
static float frames_behind = 0;
static int frame_skip_timer = 0;

#if defined(LOSSY_ADAPTER)
static GekkoNetAdapter* base_adapter = NULL;
static GekkoNetAdapter lossy_adapter = { 0 };

static float random_float() {
    return (float)rand() / RAND_MAX;
}

static void LossyAdapter_SendData(GekkoNetAddress* addr, const char* data, int length) {
    const float number = random_float();

    // Adjust this number to change drop probability
    if (number <= 0.25) {
        return;
    }

    base_adapter->send_data(addr, data, length);
}
#endif

static void setup_vs_mode() {
    // This is pretty much a copy of logic from menu.c
    task[TASK_MENU].r_no[0] = 5; // go to idle routine (doing nothing)
    cpExitTask(TASK_SAVER);
    gs.plw[0].wu.operator = 1;
    gs.plw[1].wu.operator = 1;
    Operator_Status[0] = 1;
    Operator_Status[1] = 1;
    grade_check_work_1st_init(0, 0);
    grade_check_work_1st_init(0, 1);
    grade_check_work_1st_init(1, 0);
    grade_check_work_1st_init(1, 1);
    Setup_Training_Difficulty();

    G_No[1] = 12;
    G_No[2] = 1;
    Mode_Type = MODE_NETWORK;
    cpExitTask(TASK_MENU);

    // Stop game task. We'll run game logic manually
    task[TASK_GAME].condition = 3;
}

#if defined(LOSSY_ADAPTER)
static void configure_lossy_adapter() {
    base_adapter = gekko_default_adapter(local_port);
    lossy_adapter.send_data = LossyAdapter_SendData;
    lossy_adapter.receive_data = base_adapter->receive_data;
    lossy_adapter.free_data = base_adapter->free_data;
}
#endif

static void configure_gekko() {
    GekkoConfig config;
    SDL_zero(config);

    config.num_players = 2;
    config.input_size = sizeof(u16);
    config.state_size = sizeof(GameState);
    config.max_spectators = 0;
    config.input_prediction_window = 10;
    // config.desync_detection = true;

    gekko_create(&session);
    gekko_start(session, &config);

#if defined(LOSSY_ADAPTER)
    configure_lossy_adapter();
    gekko_net_adapter_set(session, &lossy_adapter);
#else
    gekko_net_adapter_set(session, gekko_default_adapter(local_port));
#endif

    printf("starting a session for player %d at port %hu\n", player_number, local_port);

    char remote_address_str[100];
    SDL_snprintf(remote_address_str, sizeof(remote_address_str), "127.0.0.1:%hu", remote_port);
    GekkoNetAddress remote_address = { .data = remote_address_str, .size = strlen(remote_address_str) };

    if (player_number == 0) {
        player_handle = gekko_add_actor(session, LocalPlayer, NULL);
        gekko_add_actor(session, RemotePlayer, &remote_address);
    } else {
        gekko_add_actor(session, RemotePlayer, &remote_address);
        player_handle = gekko_add_actor(session, LocalPlayer, NULL);
    }
}

static u16 get_inputs() {
    // The game doesn't differentiate between controllers and players.
    // That's why we OR the inputs of both local controllers together to get
    // local inputs.
    u16 inputs = 0;
    inputs = p1sw_buff | p2sw_buff;
    return inputs;
}

static void note_input(u16 input, int player, int frame) {
    input_history[player][frame % INPUT_HISTORY_MAX] = input;
}

static u16 recall_input(int player, int frame) {
    return input_history[player][frame % INPUT_HISTORY_MAX];
}

static void save_state(GekkoGameEvent* event) {
    *event->data.save.state_len = sizeof(GameState);
    GameState* dst = (GameState*)event->data.save.state;
    SDL_memcpy(dst, &gs, sizeof(GameState));
}

static void load_state(GekkoGameEvent* event) {
    const GameState* src = (GameState*)event->data.load.state;
    SDL_memcpy(&gs, src, sizeof(GameState));
}

static bool game_ready_to_run_character_select() {
    return G_No[1] == 1;
}

static bool need_to_catch_up() {
    return frames_behind >= 1;
}

static void step_game(bool render) {
    if (render) {
        init_color_trans_req();
    }

    No_Trans = !render;
    Play_Game = 0;

    init_texcash_before_process();
    seqsBeforeProcess();

    Game();

    seqsAfterProcess();
    texture_cash_update();
    move_pulpul_work();
    Check_LDREQ_Queue();
}

static void advance_game(GekkoGameEvent* event, bool last_advance) {
    const u16* inputs = (u16*)event->data.adv.inputs;
    const int frame = event->data.adv.frame;
    p1sw_0 = inputs[0];
    p2sw_0 = inputs[1];

    p1sw_1 = recall_input(0, frame - 1);
    p2sw_1 = recall_input(1, frame - 1);

    note_input(inputs[0], 0, frame);
    note_input(inputs[1], 1, frame);

    step_game(last_advance);
}

static void process_session() {
    frames_behind = -gekko_frames_ahead(session);

    gekko_network_poll(session);

    // GekkoNetworkStats stats;
    // gekko_network_stats(session, (player_handle == 0) ? 1 : 0, &stats);
    // printf("🛜 ping: %hu, avg ping: %.2f, jitter: %.2f\n", stats.last_ping, stats.avg_ping, stats.jitter);

    u16 local_inputs = get_inputs();
    gekko_add_local_input(session, player_handle, &local_inputs);

    int session_event_count = 0;
    GekkoSessionEvent** session_events = gekko_session_events(session, &session_event_count);

    for (int i = 0; i < session_event_count; i++) {
        const GekkoSessionEvent* event = session_events[i];

        switch (event->type) {
        case PlayerSyncing:
            printf("🔴 player syncing\n");
            // FIXME: Show status to the player
            break;

        case PlayerConnected:
            printf("🔴 player connected\n");
            break;

        case PlayerDisconnected:
            printf("🔴 player disconnected\n");
            // FIXME: Handle disconnection
            break;

        case SessionStarted:
            printf("🔴 session started\n");
            session_state = SESSION_RUNNING;
            break;

        default:
            // Do nothing
            break;
        }
    }
}

static int get_last_advance_index(GekkoGameEvent** events, int event_count) {
    for (int i = event_count - 1; i >= 0; i--) {
        if (events[i]->type == AdvanceEvent) {
            return i;
        }
    }

    return -1;
}

static void process_events() {
    int game_event_count = 0;
    GekkoGameEvent** game_events = gekko_update_session(session, &game_event_count);
    const int last_advance_index = get_last_advance_index(game_events, game_event_count);

    for (int i = 0; i < game_event_count; i++) {
        const GekkoGameEvent* event = game_events[i];

        switch (event->type) {
        case LoadEvent:
            load_state(event);
            break;

        case AdvanceEvent:
            const bool last_advance = (i == last_advance_index);
            advance_game(event, last_advance);
            break;

        case SaveEvent:
            save_state(event);
            break;

        case EmptyGameEvent:
            // Do nothing
            break;
        }
    }
}

static void step_logic() {
    process_session();
    process_events();
}

static void run_netplay() {
    step_logic();

    if (need_to_catch_up() && (frame_skip_timer == 0)) {
        step_logic();
        frame_skip_timer = 60; // Allow skipping a frame roughly every second
    }

    frame_skip_timer -= 1;

    if (frame_skip_timer < 0) {
        frame_skip_timer = 0;
    }
}

void Netplay_SetPlayer(int player) {
    if (player == 1) {
        local_port = 50000;
        remote_port = 50001;
        player_number = 0;
    } else {
        local_port = 50001;
        remote_port = 50000;
        player_number = 1;
    }
}

void Netplay_Begin() {
    setup_vs_mode();
    session_state = SESSION_TRANSITIONING;
}

void Netplay_Run() {
    switch (session_state) {
    case SESSION_TRANSITIONING:
        if (!game_ready_to_run_character_select()) {
            step_game(true);
        } else {
            configure_gekko();
            session_state = SESSION_CONNECTING;
        }

        break;

    case SESSION_CONNECTING:
    case SESSION_RUNNING:
        run_netplay();
        break;

    case SESSION_IDLE:
        // Do nothing
        break;
    }
}
