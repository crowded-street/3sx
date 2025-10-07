#include "sf33rd/Source/Game/EFF37.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/EFFH2.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/aboutspr.h"

const s16 panel_pos_hosei[8][4][2] = {
    { { 25, 99 }, { 25, 100 }, { 0, 0 }, { 25, 100 } }, { { 43, 103 }, { 42, 106 }, { 0, 0 }, { 42, 106 } },
    { { 18, 115 }, { 0, 0 }, { 0, 0 }, { 18, 115 } },   { { 18, 115 }, { 0, 0 }, { 0, 0 }, { 18, 115 } },
    { { 18, 115 }, { 0, 0 }, { 0, 0 }, { 18, 115 } },   { { 30, 90 }, { 30, 89 }, { 0, 0 }, { 30, 89 } },
    { { -23, 112 }, { 0, 0 }, { 0, 0 }, { -23, 112 } }, { { -86, 60 }, { 0, 0 }, { 0, 0 }, { -86, 60 } }
};

void effect_37_move(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s16 koc, s32 index, s32 ip, s16 scf);
    s32 effect_H2_init(WORK * wk, u32 gal, u32 ohen);
#endif

    WORK* mwk = (WORK*)ewk->my_master;
    s16 ix;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = (ewk->wu.type == 1) + 0x28;
        set_char_move_init2(&ewk->wu, 0, 0, plw[ewk->wu.type].player_number + 1, 0);

        if (plw[0].player_number == plw[1].player_number) {
            effect_H2_init(&ewk->wu, ewk->wu.charset_id, ewk->wu.type);
            break;
        }

        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        if (mwk->cg_type) {
            ewk->wu.disp_flag = 1;

            if (mwk->cg_type == 0xFF) {
                ix = 3;
            } else {
                ix = mwk->cg_type - 1;
            }

            if (ewk->wu.rl_waza) {
                ewk->wu.position_x = mwk->position_x - panel_pos_hosei[ewk->wu.charset_id][ix][0];
            } else {
                ewk->wu.position_x = mwk->position_x + panel_pos_hosei[ewk->wu.charset_id][ix][0];
            }

            ewk->wu.position_y = mwk->position_y + panel_pos_hosei[ewk->wu.charset_id][ix][1];
        } else {
            ewk->wu.disp_flag = 0;
        }

        sort_push_request(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_37_init(WORK* wk, u8 gal, u8 ohen) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 37;
    ewk->wu.work_id = 16;
    ewk->wu.charset_id = gal;
    ewk->wu.type = ohen;
    ewk->my_master = (u32*)wk;
    ewk->wu.rl_waza = wk->rl_flag;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_family = 2;
    ewk->wu.my_mts = 14;
    ewk->wu.position_z = wk->position_z - 1;
    ewk->wu.char_table[0] = _ag_face_panel_table;
    return 0;
}
