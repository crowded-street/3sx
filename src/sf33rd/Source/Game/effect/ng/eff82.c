/**
 * @file eff82.c
 * Spectators of Ibuki's stage from New Generation (effects 77-80 and 82 in the NG arcade ROM).
 * Effect 82 starts the spectators and watches the end of each round.
 */

#include "sf33rd/Source/Game/effect/ng/eff82.h"
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
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#include <SDL3/SDL.h>

#define COLOR_CODE 0x213D      // NG 0x2051
#define EFF79_COLOR_CODE 0x13D // NG 0x51
#define DOWN_PAT_STATUS 0x26   // Pattern status of a player lying down
#define EFF77_CHARS 0          // First char of each effect in _ng_eff77_char_table
#define EFF78_CHARS 6
#define EFF79_CHARS 11
#define EFF80_CHARS 17

enum {
    SPECTATORS_WAIT,   // Round in progress
    SPECTATORS_GATHER, // Ibuki has lost a perfect round
};

static bool is_paused() {
    return EXE_flag || Game_pause;
}

static WORK_Other* get_master(WORK_Other* ewk) {
    return (WORK_Other*)ewk->my_master;
}

static WORK_Other* get_leader(WORK_Other* ewk) {
    return (WORK_Other*)ewk->wu.target_adrs;
}

static PLW* get_loser(WORK_Other* ewk) {
    return &plw[get_master(ewk)->master_id];
}

static void remove_spectator(WORK_Other* ewk) {
    all_cgps_put_back(&ewk->wu);
    push_effect_work(&ewk->wu);
}

static void set_priority(WORK_Other* ewk, s16 priority) {
    ewk->wu.my_priority = ewk->wu.position_z = priority;
}

static WORK_Other* init_spectator(s16 id, WORK_Other* master, s16 x, s16 y) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return NULL;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = id;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.sync_suzi = 0;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.char_table[0] = _ng_eff77_char_table;
    ewk->wu.xyz[0].disp.pos = x;
    ewk->wu.xyz[1].disp.pos = y;
    ewk->my_master = master;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    suzi_offset_set(ewk);
    return ewk;
}

// MARK: - Effect 77

static bool is_ibuki_knocked_down(const PLW* wk) {
    return wk->player_number == CHAR_IBUKI && wk->wu.routine_no[1] == 1 && wk->wu.routine_no[2] > 11;
}

static void eff77_wait(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, EFF77_CHARS);
        break;

    case 1:
        switch (ewk->wu.routine_no[2]) {
        case 0:
            if (is_ibuki_knocked_down(&plw[0]) || is_ibuki_knocked_down(&plw[1])) {
                ewk->wu.routine_no[2]++;
            }

            break;

        case 1:
            ewk->wu.routine_no[2]++;
            set_char_move_init(&ewk->wu, 0, EFF77_CHARS + 1);
            break;

        case 2:
            char_move(&ewk->wu);

            if (ewk->wu.cg_type != 0) {
                set_char_move_init(&ewk->wu, 0, EFF77_CHARS);
                ewk->wu.routine_no[2] = 0;
            }

            break;
        }

        break;
    }
}

static void eff77_gather(WORK_Other* ewk) {
    PLW* loser = get_loser(ewk);

    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        set_char_move_init(&ewk->wu, 0, EFF77_CHARS + 2);
        set_priority(ewk, 0x44);
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 0) {
            // Jump off
            ewk->wu.routine_no[1]++;
            ewk->wu.mvxy.a[0].sp = 0x68000;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = 0x88000;
            ewk->wu.mvxy.d[1].sp = 0x1200;
            ewk->wu.old_rno[2] = 24;
        }

        break;

    case 2:
        add_x_sub_ng(&ewk->wu);
        add_y_sub_ng(&ewk->wu);
        char_move(&ewk->wu);

        if (--ewk->wu.old_rno[2] < 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[1]++;
            ewk->wu.old_rno[2] = 32;
            ewk->wu.old_rno[0] = loser->wu.position_x;
            ewk->wu.old_rno[1] = 40;
        }

        break;

    case 3:
        if (--ewk->wu.old_rno[2] < 0) {
            // Drop in next to the loser
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 1;
            ewk->wu.old_rno[2] = 40;
            ewk->wu.rl_flag = loser->wu.rl_flag;
            ewk->wu.xyz[0].disp.pos = loser->wu.xyz[0].disp.pos + (ewk->wu.rl_flag ? -0x90 : 0x90);
            cal_all_speed_data(&ewk->wu, ewk->wu.old_rno[2], ewk->wu.old_rno[0], ewk->wu.old_rno[1], 1, 1);
            set_char_move_init(&ewk->wu, 0, EFF77_CHARS + 3);
        }

        break;

    case 4:
        add_mvxy_speed_no_use_rl(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);

        if (--ewk->wu.old_rno[2] < 1) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 5:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF && loser->wu.pat_status == DOWN_PAT_STATUS) {
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 0, EFF77_CHARS + 5);
        }

        break;

    case 6:
        char_move(&ewk->wu);
        break;
    }
}

static void eff77_stand(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, EFF77_CHARS + 4);
        ewk->wu.xyz[0].disp.pos = 320;
        ewk->wu.xyz[1].disp.pos = 52;
        set_priority(ewk, 0x46);
        break;

    case 1:
        char_move(&ewk->wu);
        disp_pos_trans_entry_ng(ewk);
        break;
    }
}

void effect_ng77_move(WORK_Other* ewk) {
    if (bg_w.area != 0) {
        eff77_stand(ewk);
        return;
    }

    switch (get_master(ewk)->wu.routine_no[0]) {
    case SPECTATORS_WAIT:
        if (!is_paused()) {
            eff77_wait(ewk);
        }

        break;

    case SPECTATORS_GATHER:
        if (!is_paused()) {
            eff77_gather(ewk);
        }

        break;

    default:
        remove_spectator(ewk);
        return;
    }

    disp_pos_trans_entry_ng(ewk);
}

// MARK: - Effect 78

static void eff78_wait(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, EFF78_CHARS);
        set_priority(ewk, 0x47);
        break;

    case 1:
        char_move(&ewk->wu);
        break;
    }
}

static void eff78_gather(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        set_char_move_init(&ewk->wu, 0, EFF78_CHARS + 2);
        set_priority(ewk, 0x45);
        ewk->wu.old_rno[2] = 40;
        break;

    case 1:
        if (--ewk->wu.old_rno[2] < 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[1]++;
            ewk->wu.old_rno[2] = 72;
        }

        if (ewk->wu.cg_type == 0) {
            char_move(&ewk->wu);
        } else {
            ewk->wu.disp_flag = 0;
        }

        break;

    case 2:
        if (--ewk->wu.old_rno[2] < 0) {
            // Drop in next to the leader's target
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 1;
            ewk->wu.old_rno[0] = ewk->wu.xyz[0].disp.pos = get_leader(ewk)->wu.old_rno[0] + 0x50;
            ewk->wu.xyz[1].disp.pos = 0x180;
            ewk->wu.old_rno[1] = 0x48;
            ewk->wu.old_rno[2] = 28;
            cal_all_speed_data(&ewk->wu, ewk->wu.old_rno[2], ewk->wu.old_rno[0], ewk->wu.old_rno[1], 1, 1);
            set_char_move_init(&ewk->wu, 0, EFF78_CHARS + 3);
        }

        break;

    case 3:
        add_x_sub_ng(&ewk->wu);
        add_y_sub_ng(&ewk->wu);

        if (--ewk->wu.old_rno[2] < 1) {
            ewk->wu.routine_no[1]++;
            set_char_move_init2(&ewk->wu, 0, EFF78_CHARS + 3, 2, 0);
        }

        break;

    case 4:
        char_move(&ewk->wu);
        break;
    }
}

static void eff78_stand(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, EFF78_CHARS + 4);
        ewk->wu.xyz[0].disp.pos = 512;
        ewk->wu.xyz[1].disp.pos = 51;
        set_priority(ewk, 0x47);
        break;

    case 1:
        char_move(&ewk->wu);
        suzi_sync_pos_set(ewk);
        sort_push_request4(&ewk->wu);
        break;
    }
}

void effect_ng78_move(WORK_Other* ewk) {
    if (bg_w.area != 0) {
        eff78_stand(ewk);
        return;
    }

    switch (get_master(ewk)->wu.routine_no[0]) {
    case SPECTATORS_WAIT:
        if (!is_paused()) {
            eff78_wait(ewk);
        }

        break;

    case SPECTATORS_GATHER:
        if (!is_paused()) {
            eff78_gather(ewk);
        }

        break;

    default:
        remove_spectator(ewk);
        return;
    }

    disp_pos_trans_entry_rs_ng(ewk);
}

// MARK: - Effect 79

static void eff79_wait(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, EFF79_CHARS);
        set_priority(ewk, 0x46);
        break;

    case 1:
        char_move(&ewk->wu);
        break;
    }
}

static void eff79_gather(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        set_char_move_init(&ewk->wu, 0, EFF79_CHARS + 1);
        set_priority(ewk, 0x44);
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 0) {
            // Jump up
            ewk->wu.routine_no[1]++;
            ewk->wu.mvxy.a[1].sp = 0x80000;
            ewk->wu.mvxy.d[1].sp = 0x2000;
            ewk->wu.old_rno[2] = 32;
        }

        break;

    case 2:
        char_move(&ewk->wu);
        add_x_sub_ng(&ewk->wu);
        add_y_sub_ng(&ewk->wu);

        if (--ewk->wu.old_rno[2] < 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[1]++;
            ewk->wu.old_rno[2] = 16;
        }

        break;

    case 3:
        if (--ewk->wu.old_rno[2] < 0) {
            // Fall next to the loser
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 0, EFF79_CHARS + 2);
            ewk->wu.disp_flag = 1;
            ewk->wu.old_rno[0] = get_loser(ewk)->wu.xyz[0].disp.pos - 0x40;
            ewk->wu.old_rno[1] = 0x40;
            ewk->wu.old_rno[2] = 24;
            cal_all_speed_data(&ewk->wu, ewk->wu.old_rno[2], ewk->wu.old_rno[0], ewk->wu.old_rno[1], 1, 1);
        }

        break;

    case 4:
        add_x_sub_ng(&ewk->wu);
        add_y_sub_ng(&ewk->wu);

        if (--ewk->wu.old_rno[2] < 1) {
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 0, EFF79_CHARS + 3);
        }

        break;

    case 5:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 2) {
            // Hop
            ewk->wu.routine_no[1]++;
            ewk->wu.old_rno[2] = 16;
            cal_all_speed_data(&ewk->wu, ewk->wu.old_rno[2], ewk->wu.xyz[0].disp.pos, 0x98, 1, 1);
        }

        break;

    case 6:
        char_move(&ewk->wu);
        add_y_sub_ng(&ewk->wu);

        if (--ewk->wu.old_rno[2] < 1) {
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 0, EFF79_CHARS + 4);
            ewk->wu.old_rno[2] = 16;
            cal_all_speed_data(&ewk->wu, ewk->wu.old_rno[2], ewk->wu.old_rno[0], ewk->wu.old_rno[1], 1, 1);
        }

        break;

    case 7:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 0) {
            add_y_sub_ng(&ewk->wu);

            if (--ewk->wu.old_rno[2] < 1) {
                ewk->wu.routine_no[1]++;
                set_char_move_init2(&ewk->wu, 0, EFF79_CHARS + 4, 6, 0);
            }
        }

        break;

    case 8:
        char_move(&ewk->wu);
        break;
    }
}

void effect_ng79_move(WORK_Other* ewk) {
    switch (get_master(ewk)->wu.routine_no[0]) {
    case SPECTATORS_WAIT:
        if (!is_paused()) {
            eff79_wait(ewk);
        }

        break;

    case SPECTATORS_GATHER:
        if (!is_paused()) {
            eff79_gather(ewk);
        }

        break;

    default:
        remove_spectator(ewk);
        return;
    }

    disp_pos_trans_entry_rs_ng(ewk);
}

// MARK: - Effect 80

static bool eff80_is_disturbed(WORK_Other* ewk) {
    const s16 x = ewk->wu.xyz[0].disp.pos;

    if (bg_w.bgw[1].xy[1].disp.pos <= 0x30) {
        return false;
    }

    return SDL_abs(x - plw[0].wu.xyz[0].disp.pos) < 0x90 || SDL_abs(x - plw[1].wu.xyz[0].disp.pos) < 0x90;
}

static void eff80_wait(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, EFF80_CHARS);
        set_priority(ewk, 0x46);
        break;

    case 1:
        switch (ewk->wu.routine_no[2]) {
        case 0:
            if (eff80_is_disturbed(ewk)) {
                ewk->wu.routine_no[2]++;
            }

            break;

        case 1:
            // Drop out of sight
            char_move(&ewk->wu);

            if (ewk->wu.cg_type != 0) {
                ewk->wu.routine_no[2]++;
                ewk->wu.disp_flag = 0;
            }

            break;
        }

        break;
    }
}

static void eff80_gather(WORK_Other* ewk) {
    WORK_Other* leader = get_leader(ewk);

    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 0;
        set_char_move_init(&ewk->wu, 0, EFF80_CHARS + 1);
        break;

    case 1:
        if (leader->wu.routine_no[1] > 2) {
            // Drop in once the leader has jumped off
            ewk->wu.routine_no[1]++;
            set_priority(ewk, 0x46);
            ewk->wu.disp_flag = 1;
            ewk->wu.old_rno[0] = ewk->wu.xyz[0].disp.pos = leader->wu.old_rno[0] + 0x80;
            ewk->wu.xyz[1].disp.pos = 0x180;
            ewk->wu.old_rno[1] = leader->wu.old_rno[1] + 0x38;
            ewk->wu.old_rno[2] = 128;
            cal_all_speed_data(&ewk->wu, ewk->wu.old_rno[2], ewk->wu.old_rno[0], ewk->wu.old_rno[1], 1, 2);
        }

        break;

    case 2:
        add_x_sub_ng(&ewk->wu);
        add_y_sub_ng(&ewk->wu);
        char_move(&ewk->wu);

        if (--ewk->wu.old_rno[2] < 1) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 3:
        char_move(&ewk->wu);
        break;
    }
}

void effect_ng80_move(WORK_Other* ewk) {
    switch (get_master(ewk)->wu.routine_no[0]) {
    case SPECTATORS_WAIT:
        if (!is_paused()) {
            eff80_wait(ewk);
        }

        disp_pos_trans_entry_s_ng(ewk);
        break;

    case SPECTATORS_GATHER:
        if (!is_paused()) {
            eff80_gather(ewk);
        }

        disp_pos_trans_entry_rs_ng(ewk);
        break;

    default:
        remove_spectator(ewk);
        break;
    }
}

// MARK: - Effect 82

void effect_ng82_move(WORK_Other* ewk) {
    if (bg_w.area != 0 || !is_round_over()) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case SPECTATORS_WAIT:
        if (Complete_Victory && plw[Loser_id].player_number == CHAR_IBUKI) {
            ewk->wu.routine_no[0] = SPECTATORS_GATHER;
            ewk->master_id = Loser_id;
        }

        break;

    case SPECTATORS_GATHER:
        break;

    default:
        remove_spectator(ewk);
        break;
    }
}

s32 effect_ng82_init() {
    WORK_Other* ewk;
    WORK_Other* leader;
    WORK_Other* follower;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG82_ID;
    ewk->wu.work_id = 16;

    if ((leader = init_spectator(EFFECT_NG77_ID, ewk, 320, 52)) == NULL) {
        return -1;
    }

    set_priority(leader, 0x46);
    leader->wu.position_x = 320;
    leader->wu.position_y = 52;

    if ((follower = init_spectator(EFFECT_NG78_ID, ewk, 512, 51)) != NULL) {
        follower->wu.target_adrs = leader;
    }

    if (bg_w.area != 0) {
        return 0;
    }

    if ((follower = init_spectator(EFFECT_NG79_ID, ewk, 480, 51)) != NULL) {
        follower->wu.target_adrs = leader;
        follower->wu.my_col_code = EFF79_COLOR_CODE;
    }

    if ((follower = init_spectator(EFFECT_NG80_ID, ewk, 496, 322)) != NULL) {
        follower->wu.target_adrs = leader;
        set_priority(follower, 0x46);
    }

    return 0;
}
