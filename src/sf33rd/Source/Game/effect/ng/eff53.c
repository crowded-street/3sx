/**
 * @file eff53.c
 * Vulture of Elena's stage from New Generation
 */

#include "sf33rd/Source/Game/effect/ng/eff53.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define CHAR_BASE 22
#define VULTURE_COLOR_CODE 0x143 // NG 0x57
#define VULTURE_X 0x290
#define VULTURE_Y 0x30
#define VULTURE_PRIORITY 0x43
#define TAKE_OFF_DELAY 120

static bool is_paused() {
    return EXE_flag || Game_pause;
}

static void set_size(WORK_Other* ewk, s16 size) {
    ewk->wu.my_mr.size.x = ewk->wu.my_mr.size.y = size;
}

static void grow(WORK_Other* ewk) {
    ewk->wu.my_mr.size.x++;
    ewk->wu.my_mr.size.y++;
}

static void set_motion(WORK_Other* ewk, s32 speed_x, s32 speed_y, s32 accel_x, s32 accel_y) {
    ewk->wu.mvxy.a[0].sp = speed_x;
    ewk->wu.mvxy.a[1].sp = speed_y;
    ewk->wu.mvxy.d[0].sp = accel_x;
    ewk->wu.mvxy.d[1].sp = accel_y;
}

static void set_priority(WORK_Other* ewk, s16 priority) {
    ewk->wu.my_priority = ewk->wu.position_z = priority;
}

static void reset_vulture(WORK_Other* ewk) {
    ewk->wu.routine_no[0] = 0;
    ewk->wu.disp_flag = 0;
    ewk->wu.my_mr_flag = 0;
    ewk->wu.xyz[0].cal = VULTURE_X << 16;
    ewk->wu.xyz[1].cal = VULTURE_Y << 16;
    ewk->wu.position_x = VULTURE_X;
    ewk->wu.position_y = VULTURE_Y;
    set_priority(ewk, VULTURE_PRIORITY);
}

static void draw_flying(WORK_Other* ewk) {
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0x3FF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0x3FF;
    sort_push_request4(&ewk->wu);
}

void effect_ng53_move(WORK_Other* ewk) {
    // NG removes the vulture between rounds and starts a new one with the next round. 3S keeps effects across rounds,
    // so the vulture starts over instead.
    if (pcon_rno[0] == 2 && pcon_rno[1] == 3) {
        reset_vulture(ewk);
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, CHAR_BASE + 0);
        ewk->wu.direction = TAKE_OFF_DELAY;
        return;

    case 1:
        ewk->wu.direction--;

        if (ewk->wu.direction < 0) {
            ewk->wu.routine_no[0]++;
        }

        disp_pos_trans_entry(ewk);
        return;

    case 2:
        if (!is_paused()) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 2) {
                ewk->wu.routine_no[0]++;
                set_priority(ewk, 0x4F);
            }
        }

        disp_pos_trans_entry(ewk);
        return;

    case 3:
        if (!is_paused()) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type != 0) {
                ewk->wu.routine_no[0]++;
                ewk->wu.disp_flag = 0;
            }
        }

        disp_pos_trans_entry(ewk);
        return;

    case 4:
        if (!is_round_won()) {
            return;
        }

        // Comes back small, as if far away above the valley
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.old_rno[0] = 0x43;
        set_char_move_init(&ewk->wu, 0, CHAR_BASE + 1);
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].position_x + 0x58;
        ewk->wu.xyz[1].disp.pos = bg_w.bgw[1].position_y + 0x88;
        ewk->wu.my_mr_flag = 1;
        set_size(ewk, 4);
        set_motion(ewk, 0x200, 0x200, 0x140, -0x140);
        return;

    case 5:
        // NG sets a motion here but doesn't move the vulture until the next state
        if (!is_paused()) {
            char_move(&ewk->wu);
            ewk->wu.direction = (ewk->wu.direction + 1) & 3;

            if (ewk->wu.direction == 3) {
                grow(ewk);
            }

            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] < 0) {
                ewk->wu.routine_no[0]++;
                ewk->wu.direction = 0;
                set_char_move_init(&ewk->wu, 0, CHAR_BASE + 4);
                set_motion(ewk, 0x6000, 0x10000, 0x600, -0x180);
            }
        }

        break;

    case 6:
        if (!is_paused()) {
            char_move(&ewk->wu);
            add_x_sub_ng(&ewk->wu);
            add_y_sub_ng(&ewk->wu);
            ewk->wu.direction = (ewk->wu.direction + 1) & 1;

            if (ewk->wu.direction == 1) {
                grow(ewk);

                if (ewk->wu.my_mr.size.x > 0x3E) {
                    ewk->wu.routine_no[0]++;
                    ewk->wu.direction = 0;
                    ewk->wu.old_rno[0] = 0x24;
                    set_char_move_init(&ewk->wu, 0, CHAR_BASE + 3);
                    set_priority(ewk, 10);
                    ewk->wu.mvxy.a[0].sp = 0x5000;
                    ewk->wu.mvxy.d[0].sp = 0;
                }
            }
        }

        break;

    case 7:
        if (!is_paused()) {
            add_x_sub_ng(&ewk->wu);
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] < 1) {
                ewk->wu.cg_ctr = 1;
                ewk->wu.routine_no[0]++;
                set_motion(ewk, -0x20000, -0xA0000, 0x3000, 0x8000);
            }
        }

        break;

    case 8:
        // Swoops past the camera
        if (!is_paused()) {
            char_move(&ewk->wu);
            add_x_sub_ng(&ewk->wu);
            add_y_sub_ng(&ewk->wu);
            grow(ewk);

            if (ewk->wu.cg_type == 2) {
                ewk->wu.routine_no[0]++;
                set_motion(ewk, 0, 0, 0x24000, 0x18000);
            }
        }

        break;

    case 9:
        if (!is_paused()) {
            add_x_sub_ng(&ewk->wu);
            add_y_sub_ng(&ewk->wu);

            if (ewk->wu.xyz[0].disp.pos > bg_w.bgw[1].xy[0].disp.pos + 0x120) {
                // NG removes the vulture here. We keep it, hidden, to start over in the next round.
                ewk->wu.routine_no[0]++;
                ewk->wu.disp_flag = 0;
                return;
            }

            if (ewk->wu.my_mr.size.x < 0x7F) {
                grow(ewk);
            }
        }

        break;

    default:
        return;
    }

    draw_flying(ewk);
}

s32 effect_ng53_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG53_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.charset_id = 0;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = VULTURE_COLOR_CODE;
    ewk->wu.char_table[0] = _ng_afc_a_char_table;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    reset_vulture(ewk);
    return 0;
}
