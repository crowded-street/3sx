/**
 * @file effj9.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effj9.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff00.h"
#include "sf33rd/Source/Game/effect/effc2.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charid.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"

void effJ9_trans(WORK* wk);
s16 get_c2_quake(WORK* c2wk);

const s16 c2quake_table[19] = { 0, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, -1, -1, -1, -2, -2, -3, -3, 0 };

void effect_J9_move(WORK_Other* ewk) {
    WORK* c2wk = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.charset_id = 17;
        set_char_base_data(&ewk->wu);
        ewk->wu.my_col_mode = 0x4400;
        ewk->wu.my_col_code = 0x21FF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;
        ewk->wu.position_z = ewk->wu.my_priority = 68;
        effect_00_init(&ewk->wu);
        ewk->wu.next_x = 0;
        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            break;
        }

        switch (ewk->wu.routine_no[1]) {
        case 0:
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 1;
            set_char_move_init(&ewk->wu, 0, 0x44);
            break;

        case 1:
            ewk->wu.next_x = get_c2_quake(c2wk);

            if (c2wk->char_index == 0x47) {
                ewk->wu.next_x = 0;
                ewk->wu.routine_no[1]++;
                set_char_move_init(&ewk->wu, 0, 0x45);
            }

            break;

        default:
            ewk->wu.xyz[0].disp.pos = c2wk->xyz[0].disp.pos;
            break;
        }

        player_hosei_data(ewk, c2wk->dir_timer, 0);
        effJ9_trans(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effJ9_trans(WORK* wk) {
    wk->position_x = wk->xyz[0].disp.pos + wk->next_x;
    sort_push_request(wk);
}

s16 get_c2_quake(WORK* c2wk) {
    u16 c2cg;

    if ((c2cg = c2wk->cg_number) > 18) {
        return 0;
    }

    return c2quake_table[c2cg];
}

s32 effect_J9_init(WORK_Other* wk, u8 data) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 199;
    ewk->wu.work_id = 16;
    ewk->wu.type = data;
    ewk->wu.my_mts = 14;
    ewk->my_master = wk;
    ewk->master_player = wk->master_player;
    ewk->master_id = wk->master_id;
    ewk->master_work_id = wk->master_work_id;
    ewk->wu.xyz[0].disp.pos = wk->wu.xyz[0].disp.pos;
    ewk->wu.xyz[1].disp.pos = wk->wu.xyz[1].disp.pos;
    return 0;
}
