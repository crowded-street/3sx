#include "sf33rd/Source/Game/EFFE5.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFE7.h"
#include "sf33rd/Source/Game/EFFE8.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/HITCHECK.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/color3rd.h"
#include "sf33rd/Source/Game/workuser.h"

const u16 after_image_data[39][10] = {
    { 28, 14, 5, 3, 0, 80, 8, 0, 1, 4 },   { 24, 11, 6, 2, 0, 4, 10, 0, 1, 4 },  { 28, 18, 4, 2, 0, 80, 8, 0, 1, 4 },
    { 28, 22, 4, 1, 0, 40, 8, 0, 1, 4 },   { 0, 0, 2, 2, 8, 3, 2, 0, 1, 0 },     { 28, 2, 4, 1, 0, 208, 8, 0, 1, 4 },
    { 28, 2, 3, 2, 0, 80, 8, 0, 1, 4 },    { 13, 9, 4, 2, 0, 4, 10, 0, 1, 4 },   { 8, 0, 2, 4, 28, 1, 10, 0, 1, 0 },
    { 12, 14, 6, 2, 48, 209, 8, 0, 1, 4 }, { 28, 14, 4, 2, 0, 208, 8, 0, 1, 4 }, { 33, 30, 1, 2, 12, 3, 4, 0, 1, 4 },
    { 12, 26, 3, 2, 24, 81, 6, 0, 1, 4 },  { 33, 9, 1, 2, 12, 3, 4, 0, 1, 4 },   { 33, 7, 1, 2, 12, 3, 4, 0, 1, 4 },
    { 9, 0, 5, 3, 0, 82, 4, 0, 1, 0 },     { 15, 0, 10, 2, 0, 4, 10, 0, 1, 0 },  { 12, 0, 4, 1, 20, 211, 4, 0, 1, 0 },
    { 28, 2, 3, 2, 0, 80, 8, 0, 1, 4 },    { 12, 26, 3, 2, 24, 81, 6, 0, 1, 4 }, { 16, 0, 3, 3, 13, 211, 6, 0, 1, 0 },
    { 16, 0, 3, 3, 16, 211, 6, 0, 1, 0 },  { 16, 0, 3, 3, 21, 211, 6, 0, 1, 0 }, { 16, 0, 3, 3, 21, 211, 6, 0, 1, 0 },
    { 16, 0, 3, 3, 25, 211, 6, 0, 1, 0 },  { 16, 0, 3, 3, 28, 211, 6, 0, 1, 0 }, { 16, 0, 3, 3, 13, 211, 6, 0, 1, 0 },
    { 16, 0, 3, 3, 16, 211, 6, 0, 1, 0 },  { 16, 0, 3, 3, 21, 211, 6, 0, 1, 0 }, { 24, 1, 6, 3, 0, 80, 8, 0, 1, 4 },
    { 28, 1, 1, 3, 0, 80, 8, 0, 1, 4 },    { 28, 2, 3, 2, 0, 80, 8, 1, 1, 4 },   { 28, 2, 3, 2, 48, 81, 8, 1, 1, 4 },
    { 33, 32, 1, 2, 12, 3, 4, 0, 0, 0 },   { 24, 1, 6, 3, 0, 80, 8, 0, 1, 4 },   { 28, 14, 6, 3, 0, 80, 8, 0, 1, 4 },
    { 28, 18, 1, 2, 0, 80, 8, 0, 1, 4 },   { 28, 22, 1, 1, 0, 40, 8, 0, 1, 4 },  { 28, 2, 2, 3, 0, 80, 8, 0, 1, 4 },
};

const u16 after_image_color[34][2] = { { 0x0000, 0x0010 }, { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC },
                                       { 0x01F8, 0x01FC }, { 0x01F9, 0x01FD }, { 0x01F9, 0x01FD }, { 0x01F9, 0x01FD },
                                       { 0x01F9, 0x01FD }, { 0x01F9, 0x01FD }, { 0x01F9, 0x01FD }, { 0x01F8, 0x01FC },
                                       { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC },
                                       { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC },
                                       { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC },
                                       { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC }, { 0x01F7, 0x01FB }, { 0x01F7, 0x01FB },
                                       { 0x01F7, 0x01FB }, { 0x01F7, 0x01FB }, { 0x01F8, 0x01FC }, { 0x01F8, 0x01FC },
                                       { 0x0001, 0x0011 }, { 0x0001, 0x0011 } };

const u16 illusion_setup_table[13][2] = {
    { 0, 0 },  { 2, 0 },  { 0, 4 }, { 0, 5 },  { 0, 2 },  { 1, 3 },  { 0, 11 },
    { 0, 12 }, { 0, 13 }, { 0, 0 }, { 0, 15 }, { 0, 17 }, { 0, 18 },
};

s32 check_new_after_image(WORK_Other* ewk, PLW* mwk);
void setup_illusion_data(WORK_Other* ewk, PLW* mwk);

void effect_E5_move(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    s32 effect_E8_init(WORK_Other * ek, PLW * mk, s32 data);
#endif

    PLW* mwk = (PLW*)ewk->my_master;
    s16 i;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.routine_no[0] = 2;
            mwk->image_setup_flag = 0;
            break;
        }

        if (mwk->image_setup_flag == 0) {
            break;
        }

        if (mwk->image_data_index == 11 && mwk->kind_of_blocking == 2) {
            mwk->image_data_index = 33;
        }

        setup_illusion_data(ewk, mwk);

        if (ewk->wu.old_rno[2]) {
            ewk->wu.routine_no[1] = 1;
        } else {
            ewk->wu.routine_no[1] = 0;
        }

        ewk->wu.routine_no[0] = 1;
        ewk->wu.routine_no[2] = 0;
        /* fallthrough */

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.routine_no[0] = 2;
            mwk->image_setup_flag = 0;
            break;
        }

        if (check_new_after_image(ewk, mwk) != 0) {
            goto jump;
        }

        if ((ewk->wu.dir_old & 1 && EXE_flag == 0 && Game_pause == 0 && mwk->wu.hit_stop <= 0 &&
             --ewk->wu.direction == 0) ||
            (ewk->wu.dir_old & 2 &&
             (ewk->wu.routine_no[5] != mwk->wu.routine_no[1] || ewk->wu.routine_no[6] != mwk->wu.routine_no[2]) &&
             (mwk->image_data_index != 11 || mwk->wu.routine_no[1] != 4 || mwk->wu.routine_no[3] != 0)) ||
            (ewk->wu.dir_old & 4 && mwk->sa->ok != -1) ||
            (ewk->wu.dir_old & 8 && ((WORK*)mwk->wu.target_adrs)->routine_no[1] != 4 &&
             ((WORK*)mwk->wu.target_adrs)->routine_no[1] != 2) ||
            (ewk->wu.dir_old & 0x10 && mwk->wu.routine_no[1] != 4 && mwk->wu.routine_no[1] != 2) ||
            (ewk->wu.dir_old & 0x20 && ewk->wu.total_att_set != ((WORK*)mwk->wu.target_adrs)->kind_of_waza) ||
            (ewk->wu.dir_old & 0x40 && ewk->wu.total_paring != mwk->wu.kind_of_waza) ||
            (ewk->wu.dir_old & 0x80 && pcon_dp_flag) || !mwk->image_setup_flag) {
            mwk->image_setup_flag = 0;
        jump:
            ewk->wu.routine_no[0] = ewk->wu.routine_no[1] = ewk->wu.routine_no[2] = 0;
            break;
        }

        switch (ewk->wu.routine_no[1]) {
        case 0:
            if (mwk->image_setup_flag == 0) {
                ewk->wu.routine_no[0] = 0;
                ewk->wu.routine_no[1] = 0;
                break;
            }

            switch (ewk->wu.routine_no[2]) {
            case 0:
                ewk->wu.routine_no[2]++;
                effect_E7_init(ewk, mwk);
                /* fallthrough */

            case 1:
                ewk->wu.routine_no[2]++;
                ewk->wu.dir_step = ewk->wu.dmcal_m;
                /* fallthrough */

            case 2:
                if (EXE_flag == 0 && Game_pause == 0 && --ewk->wu.dir_step <= 0) {
                    effect_E7_init(ewk, mwk);
                    ewk->wu.routine_no[2] = 1;
                }

                break;
            }

            break;

        case 1:
            if (mwk->image_setup_flag == 0) {
                ewk->wu.routine_no[0] = 0;
                ewk->wu.routine_no[1] = 0;
                break;
            }

            switch (ewk->wu.routine_no[2]) {
            case 0:
                ewk->wu.routine_no[2]++;
                ewk->wu.dir_step = 0;

                if (ewk->wu.old_rno[5]) {
                    for (i = 0; i < ewk->wu.dmcal_d; i++) {
                        effect_E8_init(ewk, mwk, ewk->wu.dir_step);
                        ewk->wu.dir_step += ewk->wu.dmcal_m;
                    }
                } else {
                    for (i = 0; i < ewk->wu.dmcal_d; i++) {
                        ewk->wu.dir_step += ewk->wu.dmcal_m;
                        effect_E8_init(ewk, mwk, ewk->wu.dir_step);
                    }
                }

                break;
            }

            break;
        }

        break;

    default:
    case 2:
        push_effect_work(&ewk->wu);
        break;
    }
}

void setup_illusion_data(WORK_Other* ewk, PLW* mwk) {
#if defined(TARGET_PS2)
    void push_color_trans_req(s32 from_col, s32 to_col);
#endif

    const u16* tblh = after_image_data[mwk->image_data_index];

    if (tblh[0] & 0x10) {
        ewk->wu.disp_flag = 1;
    } else {
        ewk->wu.disp_flag = 2;
    }

    ewk->wu.old_rno[5] = tblh[0] & 0x20;
    ewk->wu.old_rno[3] = tblh[0] & 8;
    ewk->wu.old_rno[2] = tblh[0] & 4;
    ewk->wu.old_rno[1] = tblh[0] & 2;
    ewk->wu.old_rno[0] = tblh[0] & 1;
    ewk->wu.old_rno[4] = tblh[1];
    ewk->wu.dmcal_m = tblh[2];
    ewk->wu.dmcal_d = tblh[3];
    ewk->wu.direction = tblh[4];
    ewk->wu.dir_old = tblh[5];
    ewk->wu.waku_work_index = mwk->image_data_index;
    ewk->wu.olc_work_ix[0] = tblh[6] - 1;
    ewk->wu.olc_work_ix[1] = tblh[7];
    ewk->wu.olc_work_ix[2] = tblh[8];
    ewk->wu.my_bright_type = tblh[9];
    ewk->wu.routine_no[5] = mwk->wu.routine_no[1];
    ewk->wu.routine_no[6] = mwk->wu.routine_no[2];
    ewk->wu.total_paring = mwk->wu.kind_of_waza;
    ewk->wu.total_att_set = ((WORK*)mwk->wu.target_adrs)->kind_of_waza;
    push_color_trans_req(after_image_color[ewk->wu.old_rno[4]][mwk->wu.id], (mwk->wu.id * 8) + 4);
    mwk->image_setup_flag = 1;
}

s32 check_new_after_image(WORK_Other* ewk, PLW* mwk) {
    if (mwk->image_setup_flag != 2) {
        return 0;
    }

    if (after_image_data[mwk->image_data_index][6] < ewk->wu.olc_work_ix[0]) {
        return 0;
    }

    return 1;
}

s32 effect_E5_init(PLW* wk) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    wk->illusion_work = &ewk->wu;
    ewk->wu.be_flag = 1;
    ewk->wu.id = 145;
    ewk->wu.work_id = 16;
    ewk->wu.my_family = wk->wu.my_family;
    ewk->wu.cgromtype = wk->wu.cgromtype;
    ewk->wu.my_col_mode = wk->wu.my_col_mode;
    ewk->wu.my_col_code = wk->wu.my_col_code;
    ewk->my_master = (u32*)wk;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_id = wk->wu.id;
    ewk->master_player = wk->player_number;
    ewk->wu.blink_timing = ewk->master_id;
    return 0;
}

void effect_e7_e8_init_union(WORK_Other* nwk, WORK_Other* ek, PLW* mk) {
    nwk->wu.old_rno[4] = ek->wu.old_rno[4];
    nwk->wu.old_rno[3] = ek->wu.old_rno[3];
    nwk->wu.old_rno[1] = ek->wu.old_rno[1];
    nwk->wu.old_rno[0] = ek->wu.old_rno[0];
    nwk->wu.old_rno[5] = ek->wu.old_rno[5];
    nwk->wu.olc_work_ix[2] = ek->wu.olc_work_ix[2];
    nwk->wu.my_bright_type = ek->wu.my_bright_type;
    nwk->wu.my_mts = mk->wu.my_mts;
    nwk->wu.my_family = mk->wu.my_family;
    nwk->wu.cgromtype = mk->wu.cgromtype;
    nwk->wu.my_col_mode = mk->wu.my_col_mode;
    nwk->wu.my_col_code = mk->wu.my_col_code;
    nwk->wu.dir_timer = ek->wu.dmcal_d * ek->wu.dmcal_m;
    nwk->wu.dmcal_d = ek->wu.dmcal_d;
    nwk->wu.dmcal_m = ek->wu.dmcal_m;
    nwk->wu.blink_timing = mk->wu.blink_timing;
    nwk->wu.target_adrs = (u32*)ek;
    nwk->my_master = (u32*)mk;
    nwk->master_work_id = mk->wu.work_id;
    nwk->master_id = mk->wu.id;
    nwk->wu.hit_ix_table = mk->wu.hit_ix_table;
    nwk->wu.body_adrs = mk->wu.body_adrs;
    nwk->wu.hand_adrs = mk->wu.hand_adrs;
    nwk->wu.catch_adrs = mk->wu.catch_adrs;
    nwk->wu.caught_adrs = mk->wu.caught_adrs;
    nwk->wu.attack_adrs = mk->wu.attack_adrs;
    nwk->wu.hosei_adrs = mk->wu.hosei_adrs;
    nwk->wu.att_ix_table = mk->wu.att_ix_table;
    nwk->wu.weight_level = mk->wu.weight_level;
}

void get_attdata_of_illusion(WORK_Other* ewk) {
    ewk->wu.cg_hit_ix = zanzou_table[ewk->master_id][ewk->wu.type].hit_ix;
    ewk->wu.cg_att_ix = zanzou_table[ewk->master_id][ewk->wu.type].renew;
    ewk->wu.xyz[0].disp.pos = ewk->wu.position_x;
    ewk->wu.xyz[1].disp.pos = ewk->wu.position_y;
    ewk->wu.xyz[2].disp.pos = ewk->wu.position_z;
    get_char_data_zanzou(&ewk->wu);
    ewk->wu.att.guard = 0x3F;
    ewk->wu.att.dipsw = 1;
    ewk->wu.kezuri_pow = 0;
    ewk->wu.at_attribute = 0;
    ewk->wu.att.pow /= 4;

    if (ewk->wu.att.pow <= 0) {
        ewk->wu.att.pow = 1;
    }

    ewk->wu.att.piyo = 0;
    ewk->wu.att.hs_you = 0;
    ewk->wu.add_arts_point = 0;
    ewk->wu.kind_of_waza = zanzou_table[ewk->master_id][ewk->wu.type].kowaza;
    ewk->wu.at_koa = acatkoa_table[ewk->wu.kind_of_waza];

    if (ewk->wu.cg_hit_ix) {
        hit_push_request(&ewk->wu);
    }
}

s32 setup_after_images(PLW* wk, u8 ix) {
    PLW* tk = (PLW*)wk->wu.target_adrs;

    switch (illusion_setup_table[ix][0]) {
    case 0:
        wk->image_setup_flag = 2;
        wk->image_data_index = illusion_setup_table[ix][1];
        break;

    default:
        wk->image_setup_flag = 2;
        wk->image_data_index = illusion_setup_table[ix][1];
        /* fallthrough */

    case 1:
        tk->image_setup_flag = 2;
        tk->image_data_index = illusion_setup_table[ix][1];
        break;
    }

    return 0;
}

s32 erase_after_images(PLW* wk, u8 who) {
    PLW* tk = (PLW*)wk->wu.target_adrs;

    switch (who) {
    case 0:
        wk->image_setup_flag = 0;
        break;

    default:
        wk->image_setup_flag = 0;
        /* fallthrough */

    case 1:
        tk->image_setup_flag = 0;
        break;
    }

    return 0;
}
