#include "netplay/netplay.h"
#include "main.h"
#include "netplay/game_state.h"
#include "netplay/matchmaking.h"
#include "netplay/sdl_net_adapter.h"
#include "port/sdl/sdl_app.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/game.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/utils/djb2_hash.h"
#include "types.h"

#include <stdbool.h>

#include "gekkonet.h"
#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

#include <stdio.h>
#include <stdlib.h>

#define INPUT_HISTORY_MAX 120
#define FRAME_SKIP_TIMER_MAX 60 // Allow skipping a frame roughly every second
#define STATS_UPDATE_TIMER_MAX 60
#define DELAY_FRAMES 1
#define PLAYER_COUNT 2

// Uncomment to enable packet drops
// #define LOSSY_ADAPTER

typedef struct EffectState {
    s16 frwctr;
    s16 frwctr_min;
    s16 head_ix[8];
    s16 tail_ix[8];
    s16 exec_tm[8];
    uintptr_t frw[EFFECT_MAX][448];
    s16 frwque[EFFECT_MAX];
} EffectState;

typedef struct State {
    GameState gs;
    EffectState es;
} State;

static GekkoSession* session = NULL;
static unsigned short local_port = 0;
static unsigned short remote_port = 0;
static const char* remote_ip = NULL;
static int player_number = 0;
static int player_handle = 0;
static NetplaySessionState session_state = NETPLAY_SESSION_IDLE;
static char matched_ip[64];
static const char* matchmaking_server_ip = NULL;
static int matchmaking_server_port = 9000;
static bool matchmaking_pending = false;
static bool direct_p2p_pending = false;
static NET_DatagramSocket* p2p_sock = NULL;
static u16 input_history[2][INPUT_HISTORY_MAX] = { 0 };
static float frames_behind = 0;
static int frame_skip_timer = 0;
static int transition_ready_frames = 0;

static int stats_update_timer = 0;
static int frame_max_rollback = 0;
static NetworkStats network_stats = { 0 };

#if defined(DEBUG)
#define STATE_BUFFER_MAX 20

static State state_buffer[STATE_BUFFER_MAX] = { 0 };
#endif

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

static void clean_input_buffers() {
    p1sw_0 = 0;
    p2sw_0 = 0;
    p1sw_1 = 0;
    p2sw_1 = 0;
    p1sw_buff = 0;
    p2sw_buff = 0;
    SDL_zeroa(PLsw);
    SDL_zeroa(plsw_00);
    SDL_zeroa(plsw_01);
}

static void setup_vs_mode() {
    task[TASK_MENU].r_no[0] = 5; // go to idle routine (doing nothing)
    cpExitTask(TASK_SAVER);

    plw[0].wu.operator = 1;
    plw[1].wu.operator = 1;
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

    E_Timer = 0; // E_Timer can have different values depending on when the session was initiated

    Deley_Shot_No[0] = 0;
    Deley_Shot_No[1] = 0;
    Deley_Shot_Timer[0] = 15;
    Deley_Shot_Timer[1] = 15;
    Random_ix16 = 0;
    Random_ix32 = 0;
    Clear_Flash_Init(4);

    clean_input_buffers();
}

#if defined(LOSSY_ADAPTER)
static void configure_lossy_adapter(NET_DatagramSocket* sock) {
    base_adapter = SDLNetAdapter_Create(sock);
    lossy_adapter.send_data = LossyAdapter_SendData;
    lossy_adapter.receive_data = base_adapter->receive_data;
    lossy_adapter.free_data = base_adapter->free_data;
}
#endif

static void configure_gekko() {
    GekkoConfig config;
    SDL_zero(config);

    config.num_players = PLAYER_COUNT;
    config.input_size = sizeof(u16);
    config.state_size = sizeof(State);
    config.max_spectators = 0;
    config.input_prediction_window = 10;

#if defined(DEBUG)
    config.desync_detection = true;
#endif

    if (gekko_create(&session, GekkoGameSession)) {
        gekko_start(session, &config);
    } else {
        printf("Session is already running! probably incorrect.\n");
    }

    NET_DatagramSocket* mm_sock = Matchmaking_GetSocket();
    NET_DatagramSocket* active_sock;
    if (mm_sock != NULL) {
        // Matchmaking path: reuse the socket that was registered with the server.
        active_sock = mm_sock;
    } else {
        // Direct P2P path: create a dedicated UDP socket on local_port.
        NET_Init();
        p2p_sock = NET_CreateDatagramSocket(NULL, local_port);
        active_sock = p2p_sock;
    }

#if defined(LOSSY_ADAPTER)
    configure_lossy_adapter(active_sock);
    gekko_net_adapter_set(session, &lossy_adapter);
#else
    gekko_net_adapter_set(session, SDLNetAdapter_Create(active_sock));
#endif

    printf("starting a session for player %d at port %hu\n", player_number, local_port);

    char remote_address_str[100];
    SDL_snprintf(remote_address_str, sizeof(remote_address_str), "%s:%hu", remote_ip, remote_port);
    GekkoNetAddress remote_address = { .data = remote_address_str, .size = strlen(remote_address_str) };

    for (int i = 0; i < PLAYER_COUNT; i++) {
        const bool is_local_player = (i == player_number);

        if (is_local_player) {
            player_handle = gekko_add_actor(session, GekkoLocalPlayer, NULL);
            gekko_set_local_delay(session, player_handle, DELAY_FRAMES);
        } else {
            gekko_add_actor(session, GekkoRemotePlayer, &remote_address);
        }
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
    if (frame < 0) {
        return;
    }

    input_history[player][frame % INPUT_HISTORY_MAX] = input;
}

static u16 recall_input(int player, int frame) {
    if (frame < 0) {
        return 0;
    }

    return input_history[player][frame % INPUT_HISTORY_MAX];
}

#if defined(DEBUG)
static uint32_t calculate_checksum(const State* state) {
    uint32_t hash = djb2_init();
    hash = djb2_updatep(hash, state);
    return hash;
}

/// Zero out all pointers in WORK for dumping
static void clean_work_pointers(WORK* work) {
    work->target_adrs = NULL;
    work->hit_adrs = NULL;
    work->dmg_adrs = NULL;
    work->suzi_offset = NULL;
    SDL_zeroa(work->char_table);
    work->se_random_table = NULL;
    work->step_xy_table = NULL;
    work->move_xy_table = NULL;
    work->overlap_char_tbl = NULL;
    work->olc_ix_table = NULL;
    work->rival_catch_tbl = NULL;
    work->curr_rca = NULL;
    work->set_char_ad = NULL;
    work->hit_ix_table = NULL;
    work->body_adrs = NULL;
    work->h_bod = NULL;
    work->hand_adrs = NULL;
    work->h_han = NULL;
    work->dumm_adrs = NULL;
    work->h_dumm = NULL;
    work->catch_adrs = NULL;
    work->h_cat = NULL;
    work->caught_adrs = NULL;
    work->h_cau = NULL;
    work->attack_adrs = NULL;
    work->h_att = NULL;
    work->h_eat = NULL;
    work->hosei_adrs = NULL;
    work->h_hos = NULL;
    work->att_ix_table = NULL;
    work->my_effadrs = NULL;

    work->current_colcd = 0;
    work->colcd = 0;
    work->extra_col = 0;
    work->extra_col_2 = 0;
}

static void clean_plw_pointers(PLW* plw) {
    clean_work_pointers(&plw->wu);
    plw->cp = NULL;
    plw->dm_step_tbl = NULL;
    plw->as = NULL;
    plw->sa = NULL;
    plw->py = NULL;
}

static void clean_state_pointers(State* state) {
    for (int i = 0; i < 2; i++) {
        clean_plw_pointers(&state->gs.plw[i]);

        for (int j = 0; j < 56; j++) {
            state->gs.waza_work[i][j].w_ptr = NULL;
        }

        state->gs.spg_dat[i].spgtbl_ptr = NULL;
        state->gs.spg_dat[i].spgptbl_ptr = NULL;
    }

    for (int i = 0; i < EFFECT_MAX; i++) {
        WORK* work = (WORK*)state->es.frw[i];
        clean_work_pointers(work);

        WORK_Other* work_big = (WORK_Other*)state->es.frw[i];
        work_big->my_master = NULL;
    }

    for (int i = 0; i < SDL_arraysize(state->gs.bg_w.bgw); i++) {
        state->gs.bg_w.bgw[i].bg_address = NULL;
        state->gs.bg_w.bgw[i].suzi_adrs = NULL;
        state->gs.bg_w.bgw[i].start_suzi = NULL;
        state->gs.bg_w.bgw[i].suzi_adrs2 = NULL;
        state->gs.bg_w.bgw[i].start_suzi2 = NULL;
        state->gs.bg_w.bgw[i].deff_rl = NULL;
        state->gs.bg_w.bgw[i].deff_plus = NULL;
        state->gs.bg_w.bgw[i].deff_minus = NULL;
    }

    state->gs.ci_pointer = NULL;

    for (int i = 0; i < SDL_arraysize(state->gs.task); i++) {
        state->gs.task[i].func_adrs = NULL;
    }
}

/// Save state in state buffer.
/// @return Pointer to state as it has been saved.
static const State* note_state(const State* state, int frame) {
    if (frame < 0) {
        frame += STATE_BUFFER_MAX;
    }

    State* dst = &state_buffer[frame % STATE_BUFFER_MAX];
    SDL_memcpy(dst, state, sizeof(State));
    clean_state_pointers(dst);
    return dst;
}

static void dump_state(const State* src, const char* filename) {
    SDL_IOStream* io = SDL_IOFromFile(filename, "w");
    SDL_WriteIO(io, src, sizeof(State));
    SDL_CloseIO(io);
}

static void dump_saved_state(int frame) {
    const State* src = &state_buffer[frame % STATE_BUFFER_MAX];

    char filename[100];
    SDL_snprintf(filename, sizeof(filename), "states/%d_%d", player_handle, frame);

    dump_state(src, filename);
}
#endif

#define SDL_copya(dst, src) SDL_memcpy(dst, src, sizeof(src))

static void gather_state(State* dst) {
    // GameState
    GameState* gs = &dst->gs;
    GameState_Save(gs);

    // EffectState
    EffectState* es = &dst->es;
    SDL_copya(es->frw, frw);
    SDL_copya(es->exec_tm, exec_tm);
    SDL_copya(es->frwque, frwque);
    SDL_copya(es->head_ix, head_ix);
    SDL_copya(es->tail_ix, tail_ix);
    es->frwctr = frwctr;
    es->frwctr_min = frwctr_min;
}

static void save_state(GekkoGameEvent* event) {
    *event->data.save.state_len = sizeof(State);
    State* dst = (State*)event->data.save.state;

    gather_state(dst);

#if defined(DEBUG)
    const int frame = event->data.save.frame;
    const State* saved_state = note_state(dst, frame);
    *event->data.save.checksum = calculate_checksum(saved_state);
#endif
}

static void load_state(const State* src) {
    // GameState
    const GameState* gs = &src->gs;
    GameState_Load(gs);

    // EffectState
    const EffectState* es = &src->es;
    SDL_copya(frw, es->frw);
    SDL_copya(exec_tm, es->exec_tm);
    SDL_copya(frwque, es->frwque);
    SDL_copya(head_ix, es->head_ix);
    SDL_copya(tail_ix, es->tail_ix);
    frwctr = es->frwctr;
    frwctr_min = es->frwctr_min;
}

static void load_state_from_event(GekkoGameEvent* event) {
    const State* src = (State*)event->data.load.state;
    load_state(src);
}

static bool game_ready_to_run_character_select() {
    return G_No[1] == 1;
}

static bool need_to_catch_up() {
    return frames_behind >= 1;
}

static void step_game(bool render) {
    No_Trans = !render;

    njUserMain();
    seqsBeforeProcess();
    njdp2d_draw();
    seqsAfterProcess();
}

static void advance_game(GekkoGameEvent* event, bool render) {
    const u16* inputs = (u16*)event->data.adv.inputs;
    const int frame = event->data.adv.frame;

    p1sw_0 = PLsw[0][0] = inputs[0];
    p2sw_0 = PLsw[1][0] = inputs[1];
    p1sw_1 = PLsw[0][1] = recall_input(0, frame - 1);
    p2sw_1 = PLsw[1][1] = recall_input(1, frame - 1);

    note_input(inputs[0], 0, frame);
    note_input(inputs[1], 1, frame);

    step_game(render);
}

static void handle_disconnection() {
    if (session_state == NETPLAY_SESSION_EXITING || session_state == NETPLAY_SESSION_IDLE) {
        return;
    }

    clean_input_buffers();
    Soft_Reset_Sub();
    session_state = NETPLAY_SESSION_EXITING;
}

static void process_session() {
    frames_behind = -gekko_frames_ahead(session);

    gekko_network_poll(session);

    u16 local_inputs = get_inputs();
    gekko_add_local_input(session, player_handle, &local_inputs);

    int session_event_count = 0;
    GekkoSessionEvent** session_events = gekko_session_events(session, &session_event_count);

    for (int i = 0; i < session_event_count; i++) {
        const GekkoSessionEvent* event = session_events[i];

        switch (event->type) {
        case GekkoPlayerSyncing:
            printf("🔴 player syncing\n");
            // FIXME: Show status to the player
            break;

        case GekkoPlayerConnected:
            printf("🔴 player connected\n");
            break;

        case GekkoPlayerDisconnected:
            printf("🔴 player disconnected\n");
            handle_disconnection();
            break;

        case GekkoSessionStarted:
            printf("🔴 session started\n");
            session_state = NETPLAY_SESSION_RUNNING;
            break;

        case GekkoDesyncDetected:
            const int frame = event->data.desynced.frame;
            printf("⚠️ desync detected at frame %d\n", frame);

#if defined(DEBUG)
            dump_saved_state(frame);
#endif
            break;

        case GekkoEmptySessionEvent:
        case GekkoSpectatorPaused:
        case GekkoSpectatorUnpaused:
            // Do nothing
            break;
        }
    }
}

static void process_events(bool drawing_allowed) {
    int game_event_count = 0;
    GekkoGameEvent** game_events = gekko_update_session(session, &game_event_count);
    int frames_rolled_back = 0;

    for (int i = 0; i < game_event_count; i++) {
        const GekkoGameEvent* event = game_events[i];

        switch (event->type) {
        case GekkoLoadEvent:
            load_state_from_event(event);
            break;

        case GekkoAdvanceEvent:
            const bool rolling_back = event->data.adv.rolling_back;
            advance_game(event, drawing_allowed && !rolling_back);
            frames_rolled_back += rolling_back ? 1 : 0;
            break;

        case GekkoSaveEvent:
            save_state(event);
            break;

        case GekkoEmptyGameEvent:
            // Do nothing
            break;
        }
    }

    frame_max_rollback = SDL_max(frame_max_rollback, frames_rolled_back);
}

static void step_logic(bool drawing_allowed) {
    process_session();
    process_events(drawing_allowed);
}

static void update_network_stats() {
    if (stats_update_timer == 0) {
        GekkoNetworkStats net_stats;
        gekko_network_stats(session, player_handle ^ 1, &net_stats);

        network_stats.ping = net_stats.avg_ping;
        network_stats.delay = DELAY_FRAMES;

        if (frame_max_rollback < network_stats.rollback) {
            // Don't decrease the reading by more than a frame to account for
            // the opponent not pressing buttons for 1-2 seconds
            network_stats.rollback -= 1;
        } else {
            network_stats.rollback = frame_max_rollback;
        }

        frame_max_rollback = 0;
        stats_update_timer = STATS_UPDATE_TIMER_MAX;
    }

    stats_update_timer -= 1;
    stats_update_timer = SDL_max(stats_update_timer, 0);
}

static void run_netplay() {
    // Step

    const bool catch_up = need_to_catch_up() && (frame_skip_timer == 0);
    step_logic(!catch_up);

    if (catch_up) {
        step_logic(true);
        frame_skip_timer = FRAME_SKIP_TIMER_MAX;
    }

    frame_skip_timer -= 1;
    frame_skip_timer = SDL_max(frame_skip_timer, 0);

    // Update stats

    update_network_stats();
}

void Netplay_SetParams(int player, const char* ip) {
    SDL_assert(player == 1 || player == 2);
    player_number = player - 1;
    remote_ip = ip;

    if (SDL_strcmp(ip, "127.0.0.1") == 0) {
        switch (player_number) {
        case 0:
            local_port = 50000;
            remote_port = 50001;
            break;

        case 1:
            local_port = 50001;
            remote_port = 50000;
            break;
        }
    } else {
        local_port = 50000;
        remote_port = 50000;
    }
}

void Netplay_BeginDirectP2P() {
    if (remote_ip == NULL) {
        return;
    }
    direct_p2p_pending = true;
}

void Netplay_TickDirectP2P() {
    if (!direct_p2p_pending) {
        return;
    }

    direct_p2p_pending = false;
    setup_vs_mode();

    SDL_zeroa(input_history);
    frames_behind = 0;
    frame_skip_timer = 0;
    transition_ready_frames = 0;

    session_state = NETPLAY_SESSION_TRANSITIONING;
}

void Netplay_SetMatchmakingParams(const char* server_ip, int server_port) {
    matchmaking_server_ip = server_ip;
    matchmaking_server_port = server_port;
}

void Netplay_BeginMatchmaking() {
    if (matchmaking_server_ip == NULL) {
        return;
    }
    // session_state stays IDLE so the menu keeps running normally.
    // setup_vs_mode() and the session transition happen in Netplay_TickMatchmaking.
    Matchmaking_Start(matchmaking_server_ip, matchmaking_server_port, 9001);
    matchmaking_pending = true;
}

void Netplay_TickMatchmaking() {
    if (!matchmaking_pending) {
        return;
    }

    Matchmaking_Run();

    const MatchmakingState mm = Matchmaking_GetState();

    if (mm == MATCHMAKING_MATCHED) {
        const MatchResult* r = Matchmaking_GetResult();
        player_number = r->player - 1;
        SDL_strlcpy(matched_ip, r->ip, sizeof(matched_ip));
        remote_ip = matched_ip;
        remote_port = (unsigned short)r->remote_port;
        SDL_zeroa(input_history);
        frames_behind = 0;
        frame_skip_timer = 0;
        transition_ready_frames = 0;
        matchmaking_pending = false;
        setup_vs_mode();
        session_state = NETPLAY_SESSION_TRANSITIONING;
    } else if (mm == MATCHMAKING_ERROR) {
        matchmaking_pending = false;
        Soft_Reset_Sub();
    }
}

bool Netplay_IsMatchmakingPending() {
    // Returns false once matched so cancel is ignored during the display countdown.
    return matchmaking_pending && Matchmaking_GetState() != MATCHMAKING_MATCHED;
}

void Netplay_CancelMatchmaking() {
    if (Matchmaking_GetState() != MATCHMAKING_IDLE) {
        Matchmaking_Reset();
    }
    matchmaking_pending = false;
}

void Netplay_Run() {
    switch (session_state) {
    case NETPLAY_SESSION_TRANSITIONING:
        if (game_ready_to_run_character_select()) {
            transition_ready_frames += 1;
        } else {
            transition_ready_frames = 0;
            clean_input_buffers();
            step_game(true);
        }

        if (transition_ready_frames >= 2) {
            configure_gekko();
            session_state = NETPLAY_SESSION_CONNECTING;
        }

        break;

    case NETPLAY_SESSION_CONNECTING:
    case NETPLAY_SESSION_RUNNING:
        run_netplay();
        break;

    case NETPLAY_SESSION_EXITING:
        if (session != NULL) {
            gekko_destroy(&session);
            SDLNetAdapter_Destroy();
        }

        if (p2p_sock != NULL) {
            NET_DestroyDatagramSocket(p2p_sock);
            p2p_sock = NULL;
            NET_Quit();
        }

        Netplay_CancelMatchmaking();
        session_state = NETPLAY_SESSION_IDLE;
        break;

    case NETPLAY_SESSION_IDLE:
        break;
    }
}

NetplaySessionState Netplay_GetSessionState() {
    return session_state;
}

void Netplay_HandleMenuExit() {
    Netplay_CancelMatchmaking();

    switch (session_state) {
    case NETPLAY_SESSION_IDLE:
    case NETPLAY_SESSION_EXITING:
        break;

    case NETPLAY_SESSION_TRANSITIONING:
    case NETPLAY_SESSION_CONNECTING:
    case NETPLAY_SESSION_RUNNING:
        session_state = NETPLAY_SESSION_EXITING;
        break;
    }
}

void Netplay_GetNetworkStats(NetworkStats* stats) {
    SDL_copyp(stats, &network_stats);
}
