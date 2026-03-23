#if DEBUG

#include "test/test_runner.h"
#include "constants.h"
#include "main.h"
#include "port/utils.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/count.h"

#include <SDL3/SDL.h>

#include <stdio.h>

#define GAME_TIMER_OFFSET 0x1136C
#define COUNTER_HI_OFFSET 0x11376
#define COUNTER_LOW_OFFSET 0x11378
#define MY_CHAR_OFFSET 0x11387
#define ALLOW_A_BATTLE_F_OFFSET 0x11389
#define SUPER_ARTS_OFFSET 0x1138B
#define GAME_ROUTINE_OFFSET 0x15438
#define C_NO_OFFSET 0x154A6
#define ROUND_TIMER_OFFSET 0x28679
#define PLW_OFFSET 0x68C6C
#define P1SW_1_OFFSET 0x6AA8E
#define P2SW_1_OFFSET 0x6AA92

#define PLW_SIZE 0x498

#define PLW_SA_STOP_FLAG_OFFSET 0x41C
#define PLW_DO_NOT_MOVE_OFFSET 0x455

#define WORK_ROUTINE_NO_OFFSET 0x24
#define WORK_HIT_STOP_OFFSET 0x44
#define WORK_XYZ_OFFSET 0x64
#define WORK_VITAL_NEW_OFFSET 0x9E
#define WORK_CG_IX_OFFSET 0x204
#define WORK_CG_ADD_XY_OFFSET 0x228
#define WORK_DM_STOP_OFFSET 0x32E

#define REPLAY_FRAMES_MAX 3 * 100 * 60

typedef enum Phase {
    PHASE_INIT,
    PHASE_TITLE,
    PHASE_MENU,
    PHASE_CHARACTER_SELECT_TRANSITION,
    PHASE_CHARACTER_SELECT,
    PHASE_GAME_TRANSITION,
    PHASE_ROUND_TRANSITION,
    PHASE_ROUND,
} Phase;

typedef struct Position {
    s16 x;
    s16 y;
} Position;

static const Uint8 character_to_cursor[20][2] = { { 7, 1 }, { 1, 0 }, { 5, 2 }, { 6, 1 }, { 3, 2 }, { 4, 0 }, { 1, 2 },
                                                  { 3, 0 }, { 2, 2 }, { 4, 2 }, { 0, 1 }, { 0, 2 }, { 2, 0 }, { 5, 0 },
                                                  { 6, 0 }, { 3, 1 }, { 2, 1 }, { 4, 1 }, { 1, 1 }, { 5, 1 } };

static Uint64 frame = 0;
static Phase phase = PHASE_INIT;
static int char_select_phase = 0;
static int wait_timer = 0;
static Sint8 selected_characters[2] = { -1, -1 };
static Sint8 selected_super_arts[2] = { -1, -1 };
static u16 inputs[REPLAY_FRAMES_MAX][2] = { 0 };
static int inputs_index = 0;
static int comparison_index = 0;

static void set_cursor(Character character, int player) {
    Cursor_X[player] = character_to_cursor[character][0];
    Cursor_Y[player] = character_to_cursor[character][1];
}

/// Repeatedly press and release a button
static void mash_button(SWKey button, int player) {
    u16* dst = player ? &p2sw_buff : &p1sw_buff;
    *dst |= (frame & 1) ? button : 0;
}

static void tap_button(SWKey button, int player) {
    u16* dst = player ? &p2sw_buff : &p1sw_buff;
    *dst |= button;
}

static u8 read_u8(SDL_IOStream* io, Sint64 offset) {
    u8 result;
    SDL_SeekIO(io, offset, SDL_IO_SEEK_SET);
    SDL_ReadIO(io, &result, 1);
    return result;
}

static u16 read_u16(SDL_IOStream* io, Sint64 offset) {
    u16 result;
    SDL_SeekIO(io, offset, SDL_IO_SEEK_SET);
    SDL_ReadIO(io, &result, sizeof(result));
    return SDL_Swap16(result);
}

static s16 read_s16(SDL_IOStream* io, Sint64 offset) {
    return (s16)read_u16(io, offset);
}

static Sint64 calc_plw_offset(int player) {
    return PLW_OFFSET + player * PLW_SIZE;
}

static Position read_position(SDL_IOStream* io, int player) {
    const Sint64 xyz_offset = calc_plw_offset(player) + WORK_XYZ_OFFSET;
    const Sint64 x_offset = xyz_offset;
    const Sint64 y_offset = x_offset + sizeof(XY);

    return (Position) { .x = read_s16(io, x_offset), .y = read_s16(io, y_offset) };
}

static Position get_position(int player) {
    const XY* xyz = plw[player].wu.xyz;
    return (Position) { .x = xyz[0].disp.pos, .y = xyz[1].disp.pos };
}

static u16 read_input_buff(SDL_IOStream* io, Sint64 offset) {
    const u16 raw_buff = read_u16(io, offset);
    u16 buff = 0;

    buff |= raw_buff & 0xF;              // directions
    buff |= raw_buff & (1 << 4);         // LP
    buff |= raw_buff & (1 << 5);         // MP
    buff |= raw_buff & (1 << 6);         // HP
    buff |= (raw_buff & (1 << 7)) << 1;  // LK
    buff |= (raw_buff & (1 << 8)) << 1;  // MK
    buff |= (raw_buff & (1 << 9)) << 1;  // HK
    buff |= (raw_buff & (1 << 12)) << 2; // start

    return buff;
}

static const char* ram_path(int index) {
    const char* base_path = configuration.test.states_path;
    const char* result = NULL;
    SDL_asprintf(&result, "%s/frame_%08d.ram", base_path, index);
    return result;
}

static void initialize_data() {
    bool in_round = false;
    bool in_round_prev = false;
    bool allow_battle_prev = false;
    bool did_set_char_data = false;

    for (int frame_num = 0;; frame_num++) {
        const char* path = ram_path(frame_num);
        bool stop = false;
        SDL_IOStream* io = SDL_IOFromFile(path, "rb");
        SDL_free(path);

        if (io == NULL) {
            break;
        }

        const bool allow_battle = read_u8(io, ALLOW_A_BATTLE_F_OFFSET);
        const u16 c_no_0 = read_u16(io, C_NO_OFFSET);
        const u16 c_no_1 = read_u16(io, C_NO_OFFSET + 2);
        const bool round_just_started = (c_no_0 == 1) && (c_no_1 == 4);

        if (round_just_started) {
            in_round = true;
        } else if (allow_battle_prev && !allow_battle) {
            in_round = false;
        }

        // Read character and SA indices until we get to game.
        // This ensures we read the latest data

        if (in_round && !did_set_char_data) {
            SDL_SeekIO(io, MY_CHAR_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadIO(io, selected_characters, 2);

            SDL_SeekIO(io, SUPER_ARTS_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadIO(io, selected_super_arts, 2);

            did_set_char_data = true;
        }

        // Parse inputs

        if (in_round && in_round_prev) {
            // We read previous inputs because CPS3 updates input buffers at the end of a frame
            inputs[inputs_index][0] = read_input_buff(io, P1SW_1_OFFSET);
            inputs[inputs_index][1] = read_input_buff(io, P2SW_1_OFFSET);
            inputs_index += 1;

            if (comparison_index == 0) {
                comparison_index = frame_num;
            }
        } else if (in_round_prev) {
            stop = true;
        }

        in_round_prev = in_round;
        allow_battle_prev = allow_battle;
        SDL_CloseIO(io);

        if (stop) {
            break;
        }
    }

    // There's no Shin Akuma in PS2 version, which is why we have to decrement character numbers after Akuma
    for (int i = 0; i < 2; i++) {
        if (selected_characters[i] > CHAR_AKUMA) {
            selected_characters[i] -= 1;
        }
    }

    inputs_index = 0;
}

static void compare_main_values(SDL_IOStream* io) {
    const u8 allow_a_battle_f_cps3 = read_u8(io, ALLOW_A_BATTLE_F_OFFSET);
    stop_if(Allow_a_battle_f != allow_a_battle_f_cps3);

    const u8 round_timer_cps3 = read_u8(io, ROUND_TIMER_OFFSET);
    stop_if(round_timer != round_timer_cps3);

    // const u16 game_timer_cps3 = read_u16(io, GAME_TIMER_OFFSET);
    // printf("⏱️ %d game_timer: %d\n", comparison_index, game_timer_cps3);

    for (int i = 0; i < 2; i++) {
        const Sint64 plw_offset = calc_plw_offset(i);

        const Position pos_3sx = get_position(i);
        const Position pos_cps3 = read_position(io, i);
        stop_if(pos_3sx.x != pos_cps3.x);
        stop_if(pos_3sx.y != pos_cps3.y);

        const s16 vital_new_3sx = plw[i].wu.vital_new;
        const s16 vital_new_cps3 = read_s16(io, plw_offset + WORK_VITAL_NEW_OFFSET);
        stop_if(vital_new_3sx != vital_new_cps3);
    }
}

static void compare_service_values(SDL_IOStream* io) {
    const s16 counter_hi_cps3 = read_s16(io, COUNTER_HI_OFFSET);
    stop_if(Counter_hi != counter_hi_cps3);

    const s16 counter_low_cps3 = read_s16(io, COUNTER_LOW_OFFSET);
    stop_if(Counter_low != counter_low_cps3);

    for (int i = 0; i < 2; i++) {
        const Sint64 plw_offset = calc_plw_offset(i);

        const u8 do_not_move_3sx = plw[i].do_not_move;
        const u8 do_not_move_cps3 = read_u8(io, plw_offset + PLW_DO_NOT_MOVE_OFFSET);
        stop_if(do_not_move_3sx != do_not_move_cps3);

        const s16 routine_no_0_3sx = plw[i].wu.routine_no[0];
        const s16 routine_no_0_cps3 = read_s16(io, plw_offset + WORK_ROUTINE_NO_OFFSET);
        stop_if(routine_no_0_3sx != routine_no_0_cps3);

        const s16 dm_stop_3sx = plw[i].wu.dm_stop;
        const s16 dm_stop_cps3 = read_s16(io, plw_offset + WORK_DM_STOP_OFFSET);
        stop_if(dm_stop_3sx != dm_stop_cps3);

        const s16 hit_stop_3sx = plw[i].wu.hit_stop;
        const s16 hit_stop_cps3 = read_s16(io, plw_offset + WORK_HIT_STOP_OFFSET);
        stop_if(hit_stop_3sx != hit_stop_cps3);

        const u8 sa_stop_flag_3sx = plw[i].sa_stop_flag;
        const u8 sa_stop_flag_cps3 = read_u8(io, plw_offset + PLW_SA_STOP_FLAG_OFFSET);
        stop_if(sa_stop_flag_3sx != sa_stop_flag_cps3);

        // const u16 cg_ix_cps3 = read_u16(io, plw_offset + WORK_CG_IX_OFFSET);
        // const u16 cg_ix_3sx = plw[i].wu.cg_ix;
        // stop_if(cg_ix_cps3 != cg_ix_3sx);

        const u16 cg_add_xy_cps3 = read_u16(io, plw_offset + WORK_CG_ADD_XY_OFFSET);
        const u16 cg_add_xy_3sx = plw[i].wu.cg_add_xy;
        stop_if(cg_add_xy_3sx != cg_add_xy_cps3);
    }
}

static void compare_values(SDL_IOStream* io) {
    compare_main_values(io);
    compare_service_values(io);
}

void TestRunner_Prologue() {
    p1sw_buff = 0;
    p2sw_buff = 0;

    switch (phase) {
    case PHASE_INIT:
        initialize_data();
        phase = PHASE_TITLE;
        // fallthrough

    case PHASE_TITLE:
        const struct _TASK* menu_task = &task[TASK_MENU];

        if (menu_task->r_no[0] == 0 && menu_task->r_no[1] == 1 && menu_task->r_no[2] == 3) {
            phase = PHASE_MENU;
            break;
        }

        mash_button(SWK_START, 0);
        break;

    case PHASE_MENU:
        if (G_No[1] == 1 && G_No[2] == 2) {
            Last_My_char2[0] = selected_characters[0];
            Last_My_char2[1] = selected_characters[1];
            Last_Super_Arts[0] = selected_super_arts[0];
            Last_Super_Arts[1] = selected_super_arts[1];
            phase = PHASE_CHARACTER_SELECT_TRANSITION;
            wait_timer = 60;
            break;
        }

        mash_button(SWK_SOUTH, 0);
        break;

    case PHASE_CHARACTER_SELECT_TRANSITION:
        wait_timer -= 1;

        if (wait_timer <= 0) {
            phase = PHASE_CHARACTER_SELECT;
        }

        break;

    case PHASE_CHARACTER_SELECT:
        switch (char_select_phase) {
        case 0:
            set_cursor(selected_characters[0], 0);
            set_cursor(selected_characters[1], 1);
            tap_button(SWK_START, 1);
            wait_timer = 20;
            char_select_phase = 1;
            break;

        case 1:
            wait_timer -= 1;

            if (wait_timer <= 0) {
                char_select_phase = 2;
            }

            break;

        case 2:
            tap_button(SWK_SOUTH, 0);
            tap_button(SWK_SOUTH, 1);
            wait_timer = 45;
            char_select_phase = 3;
            break;

        case 3:
            wait_timer -= 1;

            if (wait_timer <= 0) {
                tap_button(SWK_SOUTH, 0);
                tap_button(SWK_SOUTH, 1);
                phase = PHASE_GAME_TRANSITION;
            }

            break;
        }

        break;

    case PHASE_GAME_TRANSITION:
        if (G_No[1] == 2) {
            phase = PHASE_ROUND_TRANSITION;
        } else {
            // This skips the VS animation
            mash_button(SWK_ATTACKS, 0);
        }

        break;

    case PHASE_ROUND_TRANSITION:
        if (C_No[0] != 1 || C_No[1] != 4) {
            break;
        }

        phase = PHASE_ROUND;
        // fallthrough

    case PHASE_ROUND:
        p1sw_buff = inputs[inputs_index][0];
        p2sw_buff = inputs[inputs_index][1];
        inputs_index += 1;
        break;
    }
}

void TestRunner_Epilogue() {
    switch (phase) {
    case PHASE_ROUND:
        const char* path = ram_path(comparison_index);
        SDL_IOStream* io = SDL_IOFromFile(path, "rb");
        SDL_free(path);

        if (io == NULL) {
            break;
        }

        compare_values(io);

        SDL_CloseIO(io);
        comparison_index += 1;
        break;

    default:
        // Do nothing
        break;
    }

    frame += 1;
}

#endif
