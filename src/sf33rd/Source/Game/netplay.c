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

typedef struct Inputs {
    u16 current_inputs;
    u16 previous_inputs;
} Inputs;

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

static void configure_gekko() {
    GekkoConfig config;
    SDL_zero(config);

    config.num_players = 2;
    config.input_size = sizeof(Inputs);
    config.state_size = sizeof(GameState);
    config.max_spectators = 0;
    config.input_prediction_window = 10;
    // config.desync_detection = true;

    gekko_create(&session);
    gekko_start(session, &config);
    gekko_net_adapter_set(session, gekko_default_adapter(local_port));
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

static Inputs get_inputs() {
    // The game doesn't differentiate between controllers and players.
    // That's why we OR the inputs of both local controllers together to get
    // local inputs.
    Inputs inputs;
    inputs.current_inputs = p1sw_0 | p2sw_0;
    inputs.previous_inputs = p1sw_1 | p2sw_1;
    return inputs;
}

static void save_state(GekkoGameEvent* event) {
    *event->data.save.state_len = sizeof(GameState);
    SDL_memcpy(event->data.save.state, &gs, sizeof(GameState));
}

static void load_state(GekkoGameEvent* event) {
    SDL_memcpy(&gs, event->data.load.state, sizeof(GameState));
}

static bool game_ready_to_run_character_select() {
    return G_No[1] == 1;
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
    const Inputs* inputs = (Inputs*)event->data.adv.inputs;
    p1sw_0 = inputs[0].current_inputs;
    p1sw_1 = inputs[0].previous_inputs;
    p2sw_0 = inputs[1].current_inputs;
    p2sw_1 = inputs[1].previous_inputs;

    step_game(last_advance);
}

static void process_session() {
    gekko_network_poll(session);

    Inputs local_inputs = get_inputs();
    gekko_add_local_input(session, player_handle, &local_inputs);

    int session_event_count = 0;
    GekkoSessionEvent** session_events = gekko_session_events(session, &session_event_count);

    for (int i = 0; i < session_event_count; i++) {
        const GekkoSessionEvent* event = session_events[i];

        switch (event->type) {
        case PlayerSyncing:
            printf("🔴 player syncing\n");
            break;

        case PlayerConnected:
            printf("🔴 player connected\n");
            break;

        case PlayerDisconnected:
            printf("🔴 player disconnected\n");
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
        process_session();
        process_events();
        break;

    case SESSION_RUNNING:
        process_session();
        process_events();
        break;

    case SESSION_IDLE:
        // Do nothing
        break;
    }
}
