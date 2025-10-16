/**
 * @file eff00.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff00.h"
#include "common.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"

const u16 jdb[16] = { 0x8000, 0x80FF, 0xBC00, 0xBCFF, 0x8300, 0x83FF, 0xBF00, 0xBFFF,
                      0xC000, 0xC0FF, 0xFC00, 0xFCFF, 0xC300, 0xC3FF, 0xFF00, 0xFFFF };

s32 get_dip_modoki(s16 from, s8 fl);
s32 get_dip_modoki2(s16 from, s8 fl);
void renewal_table_address(WORK_Other_JUDGE* ewk, WORK* twk);
void renewal_table_data(WORK_Other_JUDGE* ewk);

void effect_00_move(WORK_Other_JUDGE* ewk) {
    u16 dip;
    u16 dip2;

    ewk->fade_cja += 2;
    ewk->fade_cja &= 0xFF;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.type = ewk->master_work_id < 16;
        renewal_table_address(ewk, (WORK*)ewk->my_master);
        ewk->wu.my_priority = ewk->wu.position_z = 1;
        ewk->look_up_flag = 0;
        ewk->curr_ja = 0;
        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        if (((WORK*)ewk->my_master)->waku_work_index != ewk->wu.myself) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        dip = get_dip_modoki(18, ewk->wu.type);
        dip2 = get_dip_modoki2(18, ewk->wu.type);
        ewk->ja_disp_bit = 0;
        ewk->ja_color_bit = 0;

        if (ewk->master_work_id != 1) {
            switch (dip & 0x2000) {
            default:
                goto jump;

            case 0:
                break;
            }
        } else if (dip & 0x1000) {
        jump:
            dip = (dip / 256) & 0xF;
            dip2 = (dip2 / 256) & 0xF;
            ewk->ja_disp_bit = jdb[dip];
            ewk->ja_color_bit = jdb[dip2];
            ewk->curr_ja = Debug_w[17];
        }

        renewal_table_address(ewk, (WORK*)ewk->my_master);

        if (ewk->wu.type) {
            renewal_table_data(ewk);
        }

        sort_push_request2((WORK_Other*)ewk);
        break;

    default:
    case 2:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 get_dip_modoki(s16 from, s8 fl) {
    s16 rnum = 0;

    rnum += (Debug_w[from] != 0) << 12;
    rnum += (Debug_w[from + 5] != 0) << 13;

    if (fl) {
        rnum += (Debug_w[from + 1] != 0) << 8;
        rnum += (Debug_w[from + 2] != 0) << 9;
        rnum += (Debug_w[from + 3] != 0) << 10;
        rnum += (Debug_w[from + 4] != 0) << 11;
    }

    return rnum;
}

s32 get_dip_modoki2(s16 from, s8 fl) {
    s16 rnum = 0;

    rnum += (Debug_w[from] == 2) << 12;
    rnum += (Debug_w[from + 5] == 2) << 13;

    if (fl) {
        rnum += (Debug_w[from + 1] == 2) << 8;
        rnum += (Debug_w[from + 2] == 2) << 9;
        rnum += (Debug_w[from + 3] == 2) << 10;
        rnum += (Debug_w[from + 4] == 2) << 11;
    }

    return rnum;
}

void renewal_table_address(WORK_Other_JUDGE* ewk, WORK* twk) {
    ewk->wu.my_family = twk->my_family;
    ewk->wu.rl_flag = twk->rl_flag;

    if (twk->disp_flag) {
        ewk->wu.disp_flag = 1;
    } else {
        ewk->wu.disp_flag = 0;
    }

    ewk->wu.h_bod = twk->h_bod;
    ewk->wu.h_han = twk->h_han;
    ewk->wu.h_cat = twk->h_cat;
    ewk->wu.h_cau = twk->h_cau;
    ewk->wu.h_att = twk->h_att;
    ewk->wu.h_hos = twk->h_hos;
    ewk->wu.position_x = twk->xyz[0].disp.pos;
    ewk->wu.position_y = twk->xyz[1].disp.pos;
}

void renewal_table_data(WORK_Other_JUDGE* ewk) {
    u16* mm;
    s16 i;
    s16 j;

    for (mm = (u16*)ewk->wu.h_bod, i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            ewk->jx[i][j] = *mm++;
        }
    }

    for (mm = (u16*)ewk->wu.h_han, i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            ewk->jx[i + 4][j] = *mm++;
        }
    }

    for (mm = (u16*)ewk->wu.h_cat, j = 0; j < 4; j++) {
        ewk->jx[8][j] = *mm++;
    }

    for (mm = (u16*)ewk->wu.h_cau, j = 0; j < 4; j++) {
        ewk->jx[9][j] = *mm++;
    }

    for (mm = (u16*)ewk->wu.h_att, i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            ewk->jx[i + 10][j] = *mm++;
        }
    }

    for (mm = (u16*)ewk->wu.h_hos, j = 0; j < 4; j++) {
        ewk->jx[14][j] = *mm++;
    }
}

s32 effect_00_init(WORK* wk) {
    WORK_Other_JUDGE* ewk;
    s16 ix;

    if (Debug_w[18] == 0 && Debug_w[23] == 0) {
        return 0;
    }

    if ((ix = pull_effect_work(0)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_JUDGE*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 0;
    ewk->wu.work_id = 128;
    ewk->wu.my_family = wk->my_family;
    ewk->my_master = (u32*)wk;
    ewk->master_work_id = wk->work_id;
    ewk->master_id = wk->id;
    wk->waku_work_index = ewk->wu.myself;
    return 0;
}
