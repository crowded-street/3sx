/**
 * @file eff94.c
 * Ibuki's tanuki from New Generation (effect 94 in the NG arcade ROM)
 *
 * Variant 0 stays at the stage centre and reacts to the end of the round. Other variants follow Ibuki around.
 */

#include "sf33rd/Source/Game/effect/ng/eff94.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#include <SDL3/SDL.h>

#define PRIORITY 0x43

/// ColorRAM rows of each player's tanuki. The tanuki's CGs use Ibuki's palette at +0 and the tanuki palette at +5.
#define PLAYER_ROWS 0x1C0
#define PLAYER_ROW_STRIDE 8
#define TANUKI_ROW_OFFSET 5

/// NG color of Ibuki whose palette is closest to each 3S color, which picks the tanuki's palette
static const u8 ng_color_tbl[13] = { 0, 3, 0, 5, 3, 1, 4, 2, 5, 0, 4, 3, 5 };

/// Ibuki's win animation that the tanuki celebrates
#define IBUKI_WIN_ANIMATION 0x27

/// Waits between the tanuki's actions
static const s16 wait_tbl[16] = { 8, 34, 278, 40, 42, 140, 238, 0, 240, 18, 6, 72, 26, 124, 80, 32 };

/// Whether the tanuki reacts to an attack nearby
static const u8 react_tbl[8] = { 0, 1, 0, 0, 1, 0, 0, 1 };

/// Frames to run to Ibuki, indexed by distance / 32
static const s16 run_time_tbl[32] = { 16, 16, 16, 32, 32, 32, 48, 48, 48, 56, 56, 56, 64,  64,  64,  72,
                                      72, 72, 72, 80, 80, 80, 88, 88, 88, 96, 96, 96, 104, 104, 104, 104 };

static PLW* master(WORK_Other* ewk) {
    return &plw[ewk->master_id];
}

/// Copies Ibuki's palette and the tanuki palette for her color into the tanuki's rows
static void set_palette(WORK_Other* ewk) {
    const s16 row = PLAYER_ROWS + ewk->master_id * PLAYER_ROW_STRIDE;
    const s8 color = Player_Color[ewk->master_id];
    const s16 ng_color = (color >= 0 && color < SDL_arraysize(ng_color_tbl)) ? ng_color_tbl[color] : 0;

    SDL_memcpy(ColorRAM[row], ColorRAM[ewk->master_id * 16], sizeof(ColorRAM[0]));
    SDL_memcpy(ColorRAM[row + TANUKI_ROW_OFFSET], ColorRAM[EFF94_PALETTE_ROW + ng_color], sizeof(ColorRAM[0]));
    palUpdateGhostCP3(row, 1);
    palUpdateGhostCP3(row + TANUKI_ROW_OFFSET, 1);
}

static bool is_paused() {
    return EXE_flag || Game_pause || EXE_obroll;
}

static void reset_wait(WORK_Other* ewk) {
    ewk->wu.old_rno[0] = wait_tbl[random_16() & 0xF];
}

static void set_state(WORK_Other* ewk, s16 state) {
    ewk->wu.routine_no[1] = state;
    ewk->wu.routine_no[2] = 0;
}

static void start_idle(WORK_Other* ewk) {
    set_state(ewk, 0);
    reset_wait(ewk);
}

static void start_run(WORK_Other* ewk) {
    set_state(ewk, 1);
    reset_wait(ewk);
}

static void start_turn(WORK_Other* ewk) {
    set_state(ewk, 2);
}

static void start_sit(WORK_Other* ewk) {
    ewk->wu.dir_step = 0;
    set_state(ewk, 6);
    reset_wait(ewk);
}

/// Like add_x_sub, but accelerates before moving
static void add_x(WORK_Other* ewk) {
    ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;
    ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
}

static bool is_round_over() {
    return !Allow_a_battle_f && Conclusion_Flag == 1 && C_No[0] > 1;
}

/// @return Whether the tanuki has run past Ibuki
static bool passed_master(WORK_Other* ewk) {
    const s16 master_x = master(ewk)->wu.xyz[0].disp.pos;

    if (ewk->wu.rl_flag == 0) {
        return ewk->wu.xyz[0].disp.pos < master_x;
    }

    return master_x < ewk->wu.xyz[0].disp.pos;
}

static bool is_master_attacking(WORK_Other* ewk) {
    const WORK* wk = &master(ewk)->wu;
    return wk->routine_no[1] == 1 && wk->routine_no[2] > 11;
}

/// Decides what to do next: react to the end of the round, run after Ibuki or turn towards her when she's far, or react
/// to her attacks when she's close.
/// @return Whether the tanuki should idle, which is when it's close to Ibuki and she isn't attacking
static bool check_master(WORK_Other* ewk) {
    if (is_round_over()) {
        set_state(ewk, 4);
        return true;
    }

    if (ewk->wu.old_rno[6] > 0x60) {
        const s16 master_x = master(ewk)->wu.xyz[0].disp.pos;
        const bool facing_master =
            ewk->wu.rl_flag == 0 ? master_x <= ewk->wu.xyz[0].disp.pos : ewk->wu.xyz[0].disp.pos <= master_x;

        if (facing_master) {
            start_run(ewk);
        } else {
            start_turn(ewk);
        }

        return false;
    }

    const s16 random = random_16();

    if (!is_master_attacking(ewk)) {
        return true;
    }

    if (react_tbl[random & 7]) {
        set_state(ewk, 3);
        ewk->wu.old_rno[0] = 8;
    }

    return false;
}

// MARK: - Following Ibuki

static void eff94_idle(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        set_char_move_init(&ewk->wu, 0, 0x11);
        break;

    case 1:
        char_move(&ewk->wu);

        if (check_master(ewk) && ++ewk->wu.dir_step > 60) {
            start_sit(ewk);
        }

        break;
    }
}

static void eff94_run(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        set_char_move_init(&ewk->wu, 0, 0x11);
        break;

    case 1:
        if (--ewk->wu.old_rno[0] >= 0) {
            char_move(&ewk->wu);
            break;
        }

        ewk->wu.dir_step = 0;
        ewk->wu.routine_no[2]++;
        set_char_move_init(&ewk->wu, 0, 0x12);
        ewk->wu.old_rno[4] = master(ewk)->wu.xyz[0].disp.pos;
        ewk->wu.old_rno[0] = run_time_tbl[(ewk->wu.old_rno[6] >> 5) & 0x1F];
        cal_all_speed_data(&ewk->wu, ewk->wu.old_rno[0], ewk->wu.old_rno[4], ewk->wu.xyz[1].disp.pos, 0, 0);
        break;

    case 2:
        char_move(&ewk->wu);

        if (!passed_master(ewk)) {
            if (--ewk->wu.old_rno[0] < 1) {
                ewk->wu.routine_no[2]++;
            } else {
                add_x(ewk);
            }

            break;
        }

        if (check_master(ewk)) {
            start_idle(ewk);
        }

        ewk->wu.old_rno[0] = 0;
        break;

    case 3:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0) {
            add_x(ewk);
            break;
        }

        if (check_master(ewk)) {
            start_idle(ewk);
        }

        ewk->wu.old_rno[0] = 0;
        break;
    }
}

static void eff94_turn(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        set_char_move_init(&ewk->wu, 0, 0x13);
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            start_run(ewk);
            ewk->wu.rl_flag ^= 1;
        }

        break;
    }
}

static void eff94_react(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        if (--ewk->wu.old_rno[0] < 0) {
            ewk->wu.routine_no[2]++;
            set_char_move_init(&ewk->wu, 0, 0x14);
        }

        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type && check_master(ewk)) {
            start_idle(ewk);
        }

        break;
    }
}

static void eff94_round_over(WORK_Other* ewk) {
    if (pcon_rno[0] == 2 && pcon_rno[1] == 3) {
        ewk->wu.old_rno[0] = 60;
        set_state(ewk, 7);
        return;
    }

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        set_char_move_init(&ewk->wu, 0, 0x17);
        break;

    case 1:
        char_move(&ewk->wu);

        if (master(ewk)->wu.char_index == IBUKI_WIN_ANIMATION) {
            set_state(ewk, 5);
        }

        break;
    }
}

static void eff94_celebrate(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.rl_flag = 0;
        set_char_move_init(&ewk->wu, 0, 0x18);
        break;

    case 1:
        char_move(&ewk->wu);
        break;
    }
}

static void eff94_sit(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        set_char_move_init(&ewk->wu, 0, 0x1C);
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type && check_master(ewk)) {
            start_idle(ewk);
        }

        break;
    }
}

static void eff94_hide(WORK_Other* ewk) {
    if (ewk->wu.routine_no[2] == 0) {
        ewk->wu.routine_no[2]++;
        ewk->wu.disp_flag = 0;
    }
}

// MARK: - Staying at the stage centre

static void eff94_stay_idle(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, 0x1A);
        break;

    case 1:
        char_move(&ewk->wu);

        if (is_round_over()) {
            set_state(ewk, 1);
        }

        break;
    }
}

static void eff94_stay_judge(WORK_Other* ewk) {
    if (Winner_id != ewk->master_id) {
        set_state(ewk, 2);
    } else if (master(ewk)->wu.char_index == IBUKI_WIN_ANIMATION) {
        // Indexes the shared state table, so this is eff94_run
        set_state(ewk, 5);
    }
}

static void eff94_stay_lose(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        set_char_move_init(&ewk->wu, 0, 0x1D);
        break;

    case 1:
        if (pcon_rno[0] == 2 && pcon_rno[1] == 3) {
            ewk->wu.disp_flag = 0;
        }

        char_move(&ewk->wu);
        break;
    }
}

static void eff94_stay_done(WORK_Other* /* unused */) {
    // Do nothing
}

/// Variant 0 indexes this table from the start. The others index it from `eff94_idle`.
static void (*const eff94_states[12])(WORK_Other*) = {
    eff94_stay_idle, eff94_stay_judge, eff94_stay_lose,  eff94_stay_done, eff94_idle, eff94_run,
    eff94_turn,      eff94_react,      eff94_round_over, eff94_celebrate, eff94_sit,  eff94_hide,
};

#define FOLLOWING_STATES 4

static bool is_in_round_end_state(WORK_Other* ewk) {
    const s16 state = ewk->wu.routine_no[1];

    if (ewk->wu.old_rno[3] == 0) {
        return state != 0;
    }

    return state == 4 || state == 5 || state == 7;
}

static void restart_if_new_round(WORK_Other* ewk) {
    if (ewk->wu.routine_no[0] == 0 || !is_in_round_end_state(ewk) || is_round_over()) {
        return;
    }

    ewk->wu.routine_no[0] = 0;
    ewk->wu.routine_no[1] = 0;
    ewk->wu.routine_no[2] = 0;
    ewk->wu.dir_step = 0;
    ewk->wu.rl_flag = (ewk->master_id == 0);
}

void effect_ng94_move(WORK_Other* ewk) {
    restart_if_new_round(ewk);

    if (ewk->wu.routine_no[0] == 0) {
        // Ibuki's palette is loaded by now
        set_palette(ewk);
    }

    if (ewk->wu.old_rno[3] == 0) {
        switch (ewk->wu.routine_no[0]) {
        case 0:
            ewk->wu.routine_no[0]++;
            // TODO: NG places it at x 0x1A0 in area 1 of its stage 7
            ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work;
            break;

        case 1:
            if (!is_paused()) {
                eff94_states[ewk->wu.routine_no[1]](ewk);
            }

            disp_pos_trans_entry_s(ewk);
            break;
        }

        return;
    }

    const s16 master_x = master(ewk)->wu.xyz[0].disp.pos;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        break;

    case 1:
        ewk->wu.routine_no[0]++;
        ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[1] = master_x;
        ewk->wu.old_rno[0] = 0;
        ewk->wu.disp_flag = 1;
        break;

    case 2:
        ewk->wu.rl_waza = (s8)ewk->wu.old_rno[5];
        ewk->wu.old_rno[5] = master_x - ewk->wu.xyz[0].disp.pos;
        ewk->wu.old_rno[6] = SDL_abs(ewk->wu.old_rno[5]);

        if (!is_paused()) {
            eff94_states[FOLLOWING_STATES + ewk->wu.routine_no[1]](ewk);
        }

        disp_pos_trans_entry_s(ewk);
        break;
    }
}

s32 effect_ng94_init_for_player(s16 player_index, s16 variant) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG94_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->my_master = &plw[player_index];
    ewk->master_id = player_index;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 0x2000 | (PLAYER_ROWS + player_index * PLAYER_ROW_STRIDE);
    ewk->wu.my_family = 2;
    ewk->wu.old_rno[3] = variant;
    ewk->wu.my_priority = ewk->wu.position_z = PRIORITY;
    ewk->wu.sync_suzi = 0;
    ewk->wu.kage_flag = 1;
    ewk->wu.kage_hx = -2;
    ewk->wu.kage_char = 9;
    ewk->wu.kage_prio = PRIORITY + 1;
    ewk->wu.char_table[0] = _ng_eff94_char_table;
    ewk->wu.xyz[1].disp.pos = ewk->wu.kage_hy = (bg_w.stage == STAGE_NG_ALEX) ? 0x2B : 0x2E;
    suzi_offset_set(ewk);
    ewk->wu.rl_flag = (player_index == 0);
    ewk->wu.dir_step = 0;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}

void effect_ng94_init_for_players(s16 variant) {
    // The tanuki stays at the stage centre when Elena is around
    if (variant != 0 && (plw[0].player_number == CHAR_ELENA || plw[1].player_number == CHAR_ELENA)) {
        variant = 0;
    }

    if (plw[0].player_number == CHAR_IBUKI && plw[1].player_number == CHAR_IBUKI) {
        effect_ng94_init_for_player(0, 0);
        return;
    }

    if (plw[0].player_number == CHAR_IBUKI) {
        effect_ng94_init_for_player(0, variant);
    }

    if (plw[1].player_number == CHAR_IBUKI) {
        effect_ng94_init_for_player(1, variant);
    }
}
