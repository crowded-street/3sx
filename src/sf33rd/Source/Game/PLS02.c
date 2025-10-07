#include "sf33rd/Source/Game/PLS02.h"
#include "bin2obj/gauge.h"
#include "common.h"
#include "sf33rd/Source/Game/CALDIR.h"
#include "sf33rd/Source/Game/CHARID.h"
#include "sf33rd/Source/Game/Com_Data.h"
#include "sf33rd/Source/Game/HITCHECK.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/PLPDM.h"
#include "sf33rd/Source/Game/PLS01.h"
#include "sf33rd/Source/Game/Se_Data.h"
#include "sf33rd/Source/Game/SysDir.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/workuser.h"
#include "structs.h"

void read_adrs_store_mvxy(WORK* wk, s16* adrs);
void remake_mvxy_PoGR(WORK* wk);
s16 meri_case_switch(s16 meri);
s16 hoseishitemo_eenka(WORK* wk, s16 tx);
s32 random_32();
s32 random_32_ex();
s32 random_16_ex();
s8 get_guard_direction(WORK* as, WORK* ds);
void add_sp_arts_gauge_guard(PLW* wk);
s16 cal_sa_gauge_waribiki(PLW* wk, s16 asag);
void setup_saishin_lvdir(PLW* ds, s8 gddir);
void dead_voice_request();
void dead_voice_request2(PLW* wk);

const s16 asagh_zuru[8] = { -2, -1, 0, 0, 1, 2, 3, 4 };

const s16 sel_hosei_tbl_ix[20] = { 29, 45, 1, 5, 49, 13, 9, 21, 17, 25, 5, 1, 1, 29, 1, 33, 37, 41, 57, 53 };

const s16 satse[20] = { 32, 32, 28, 24, 32, 36, 40, 24, 28, 28, 24, 28, 28, 32, 30, 28, 28, 24, 36, 24 };

const s16 random_tbl_32[128] = { 27, 4,  17, 6,  31, 10, 29, 2,  25, 19, 11, 22, 5,  15, 6,  20, 24, 18, 4,  26, 19, 11,
                                 30, 0,  21, 7,  10, 27, 1,  24, 16, 6,  15, 5,  29, 17, 1,  23, 18, 2,  13, 22, 31, 16,
                                 13, 3,  21, 30, 17, 22, 0,  26, 3,  19, 10, 23, 14, 30, 19, 29, 13, 8,  2,  9,  9,  25,
                                 4,  12, 28, 20, 2,  14, 27, 9,  7,  18, 27, 1,  31, 14, 4,  8,  29, 8,  24, 28, 7,  16,
                                 3,  11, 15, 6,  20, 28, 25, 12, 13, 23, 25, 10, 22, 8,  18, 21, 12, 1,  24, 31, 0,  3,
                                 26, 7,  20, 5,  5,  15, 14, 0,  11, 28, 16, 26, 9,  17, 21, 30, 12, 23 };

const s16 random_tbl_16[64] = { 3,  8, 6,  9, 14, 13, 9,  5, 10, 14, 1,  7, 4,  15, 2,  0, 12, 15, 5, 13, 6,  3,
                                11, 8, 0,  3, 11, 10, 1,  7, 11, 4,  5,  4, 13, 2,  11, 9, 7,  10, 1, 6,  12, 9,
                                14, 0, 15, 2, 13, 1,  15, 8, 0,  6,  14, 3, 12, 8,  4,  5, 10, 2,  7, 12 };

const s16 random_tbl_32_ex[32] = { 16, 24, 5,  22, 10, 27, 14, 1,  20, 8,  29, 3,  26, 11, 18, 0,
                                   31, 15, 28, 7,  12, 23, 4,  30, 9,  19, 2,  25, 13, 17, 21, 6 };

const s16 random_tbl_16_ex[16] = { 9, 0, 5, 12, 3, 14, 7, 10, 1, 15, 6, 11, 2, 8, 13, 4 };

const s16 random_tbl_32_com[128] = { 20, 5,  5,  15, 14, 0,  11, 28, 16, 26, 9,  17, 21, 30, 12, 23, 13, 23, 25,
                                     10, 22, 8,  18, 21, 12, 1,  24, 31, 0,  3,  26, 7,  4,  8,  29, 8,  24, 28,
                                     7,  16, 3,  11, 15, 6,  20, 28, 25, 12, 9,  25, 4,  12, 28, 20, 2,  14, 27,
                                     9,  7,  18, 27, 1,  31, 14, 17, 22, 0,  26, 3,  19, 10, 23, 14, 30, 19, 29,
                                     13, 8,  2,  9,  15, 5,  29, 17, 1,  23, 18, 2,  13, 22, 31, 16, 13, 3,  21,
                                     30, 24, 18, 4,  26, 19, 11, 30, 0,  21, 7,  10, 27, 1,  24, 16, 6,  27, 4,
                                     17, 6,  31, 10, 29, 2,  25, 19, 11, 22, 5,  15, 6,  20 };

const s16 random_tbl_16_com[64] = { 13, 1,  15, 8, 0,  6, 14, 3, 12, 8,  4,  5,  10, 2,  7, 12, 5,  4,  13, 2, 11, 9,
                                    7,  10, 1,  6, 12, 9, 14, 0, 15, 2,  12, 15, 5,  13, 6, 3,  11, 8,  0,  3, 11, 10,
                                    1,  7,  11, 4, 3,  8, 6,  9, 14, 13, 9,  5,  10, 14, 1, 7,  4,  15, 2,  0 };

const s16 random_tbl_32_ex_com[32] = { 31, 15, 28, 7,  12, 23, 4,  30, 9,  19, 2,  25, 13, 17, 21, 6,
                                       16, 24, 5,  22, 10, 27, 14, 1,  20, 8,  29, 3,  26, 11, 18, 0 };

const s16 random_tbl_16_ex_com[16] = { 9, 0, 5, 12, 3, 14, 7, 10, 1, 15, 6, 11, 2, 8, 13, 4 };

const s16 random_tbl_16_bg[64] = { 13, 1,  15, 8, 0,  6, 14, 3, 12, 8,  4,  5,  10, 2,  7, 12, 5,  4,  13, 2, 11, 9,
                                   7,  10, 1,  6, 12, 9, 14, 0, 15, 2,  12, 15, 5,  13, 6, 3,  11, 8,  0,  3, 11, 10,
                                   1,  7,  11, 4, 3,  8, 6,  9, 14, 13, 9,  5,  10, 14, 1, 7,  4,  15, 2,  0 };

const s16 dir16_rl_conv[16] = { 0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

const s16 dir16_hddm[16] = { 35, 35, 35, 32, 32, 32, 36, 36, 36, 36, 36, 32, 32, 32, 35, 35 };

const s16 dir16_trdm[16] = { 38, 38, 38, 37, 37, 37, 36, 36, 36, 36, 36, 37, 37, 37, 38, 38 };

const s16 konjyou_tbl[20][6] = { { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 },
                                 { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 },
                                 { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 },
                                 { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 },
                                 { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 },
                                 { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 },
                                 { 26, 28, 30, 32, 32, 32 }, { 26, 28, 30, 32, 32, 32 } };

const s16 apagt_table[20] = { 5, 4, 4, 4, 4, 4, 5, 4, 5, 3, 4, 4, 4, 4, 4, 4, 5, 3, 2, 4 };

const s16 dir32_skydm[32] = { 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 97, 97, 97, 97, 98,
                              98, 98, 97, 97, 97, 97, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88 };

const s16 dir32_grddm[32] = { 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 39, 39, 39, 39, 40,
                              40, 40, 39, 39, 39, 39, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91 };

const u8 convert_saishin_lvdir[2][16] = { { 0, 0, 0, 0, 1, 1, 1, 0, 2, 2, 2, 0, 0, 0, 0, 0 },
                                          { 0, 0, 0, 0, 2, 2, 2, 0, 1, 1, 1, 0, 0, 0, 0, 0 } };

const s16 dead_voice_table[20][2] = { 864, 865, 928, 929, 512, 513, 608, 609, 896, 897, 800, 801, 832, 833,
                                      352, 353, 672, 673, 576, 577, 640, 641, 384, 385, 480, 481, 736, 737,
                                      704, 705, 416, 417, 448, 449, 768, 769, 960, 961, 544, 545 };

void add_to_mvxy_data(WORK* wk, u16 ix) {
    s16* adrs;
    s32 sp;

    wk->mvxy.index = ix;

    adrs = &wk->move_xy_table[ix * 6];
    sp = adrs[0];
    sp *= 256;
    wk->mvxy.a[0].sp += sp;
    sp = adrs[1];
    sp *= 256;
    wk->mvxy.d[0].sp += sp;
    wk->mvxy.kop[0] = adrs[2];
    sp = adrs[3];
    sp *= 256;
    wk->mvxy.a[1].sp += sp;
    sp = adrs[4];
    sp *= 256;
    wk->mvxy.d[1].sp += sp;
    wk->mvxy.kop[1] = adrs[5];
}

void setup_move_data_easy(WORK* wk, const s16* adrs, s16 prx, s16 pry) {
    wk->mvxy.a[0].sp = adrs[0];
    wk->mvxy.a[0].sp <<= 8;
    wk->mvxy.d[0].sp = adrs[1];
    wk->mvxy.d[0].sp <<= 8;
    wk->mvxy.kop[0] = prx;
    wk->mvxy.a[1].sp = adrs[2];
    wk->mvxy.a[1].sp <<= 8;
    wk->mvxy.d[1].sp = adrs[3];
    wk->mvxy.d[1].sp <<= 8;
    wk->mvxy.kop[1] = pry;
}

void setup_mvxy_data(WORK* wk, u16 ix) {
    wk->mvxy.index = ix;
    read_adrs_store_mvxy(wk, &wk->move_xy_table[ix * 6]);
}

void setup_butt_own_data(WORK* wk) {
    wk->mvxy.index = wk->dm_butt_type;
    read_adrs_store_mvxy(
        wk, (s16*)((char*)parabora_own_table[wk->dm_plnum] + wk->dm_butt_type * 48 + wk->weight_level * 12));
}

void read_adrs_store_mvxy(WORK* wk, s16* adrs) {
    wk->mvxy.a[0].sp = adrs[0];
    wk->mvxy.a[0].sp <<= 8;
    wk->mvxy.d[0].sp = adrs[1];
    wk->mvxy.d[0].sp <<= 8;
    wk->mvxy.kop[0] = adrs[2];
    wk->mvxy.a[1].sp = adrs[3];
    wk->mvxy.a[1].sp <<= 8;
    wk->mvxy.d[1].sp = adrs[4];
    wk->mvxy.d[1].sp <<= 8;
    wk->mvxy.kop[1] = adrs[5];
}

s8 get_weight_point(WORK* wk) {
    return wk->dm_weight - wk->weight_level + 3;
}

void cal_mvxy_speed(WORK* wk) {
    s16 i;

    for (i = 0; i < 2; i++) {
        switch (wk->mvxy.kop[i]) {
        case 0:
            wk->mvxy.a[i].sp += wk->mvxy.d[i].sp;
            break;

        case 1:
            if (wk->mvxy.a[i].sp >= 0) {
                wk->mvxy.a[i].sp += wk->mvxy.d[i].sp;

                if (wk->mvxy.a[i].sp < 0) {
                    wk->mvxy.d[i].sp = 0;
                    wk->mvxy.a[i].sp = 0;
                }
            } else {
                wk->mvxy.a[i].sp += wk->mvxy.d[i].sp;

                if (wk->mvxy.a[i].sp >= 0) {
                    wk->mvxy.d[i].sp = 0;
                    wk->mvxy.a[i].sp = 0;
                }
            }
            break;
        }
    }
}

void add_mvxy_speed(WORK* wk) {
    if (wk->rl_flag) {
        wk->xyz[0].cal += wk->mvxy.a[0].sp;
    } else {
        wk->xyz[0].cal -= wk->mvxy.a[0].sp;
    }

    wk->xyz[1].cal += wk->mvxy.a[1].sp;
}

void add_mvxy_speed_exp(WORK* wk, s16 dvp) {
    if (wk->rl_flag) {
        wk->xyz[0].cal += wk->mvxy.a[0].sp / dvp;
    } else {
        wk->xyz[0].cal -= wk->mvxy.a[0].sp / dvp;
    }

    wk->xyz[1].cal += wk->mvxy.a[1].sp;
}

void add_mvxy_speed_no_use_rl(WORK* wk) {
    wk->xyz[0].cal += wk->mvxy.a[0].sp;
    wk->xyz[1].cal += wk->mvxy.a[1].sp;
}

void add_mvxy_speed_direct(WORK* wk, s16 sx, s16 sy) {
    s32 ax;
    s32 ay;

    ax = sx;
    ay = sy;
    ax <<= 8;

    if (wk->rl_flag) {
        wk->xyz[0].cal += ax;
    } else {
        wk->xyz[0].cal -= ax;
    }

    wk->xyz[1].cal += ay << 8;
}

void reset_mvxy_data(WORK* wk) {
    wk->mvxy.a[0].sp = wk->mvxy.d[0].sp = wk->mvxy.kop[0] = 0;
    wk->mvxy.a[1].sp = wk->mvxy.d[1].sp = wk->mvxy.kop[1] = 0;
}

void remake_mvxy_PoSB(WORK* wk) {
    if (wk->mvxy.a[1].sp < 0) {
        wk->mvxy.a[1].sp = (wk->mvxy.a[1].sp * 30) / 100;
        wk->mvxy.a[1].sp = -wk->mvxy.a[1].sp;
    }
}

void remake_mvxy_PoGR(WORK* wk) {
    if (wk->mvxy.d[1].sp) {
        switch ((wk->mvxy.a[1].sp > 0) + ((wk->mvxy.a[1].sp < 0) * 2)) {
        case 1:
            wk->mvxy.a[1].sp = (wk->mvxy.a[1].sp * 80) / 100;
            break;

        default:
            wk->mvxy.a[1].sp = (wk->mvxy.a[1].sp * 10) / 100;
            break;
        }
    }

    switch ((wk->mvxy.a[0].sp > 0) + ((wk->mvxy.a[0].sp < 0) * 2)) {
    case 2:
        wk->mvxy.a[0].sp = (wk->mvxy.a[0].sp * 30) / 100;
        break;

    default:
        wk->mvxy.a[0].sp = (wk->mvxy.a[0].sp * 50) / 100;

        if (wk->mvxy.a[0].real.h <= 0) {
            wk->mvxy.a[0].real.h = 1;
        }

        wk->mvxy.d[0].sp = 0;
        wk->mvxy.a[0].sp = -wk->mvxy.a[0].sp;
    }
}

void check_body_touch() {
#if defined(TARGET_PS2)
    s16 meri_case_switch(s32 meri);
#endif

    PLW* p1w = &plw[0];
    PLW* p2w = &plw[1];
    s16 meri;

    if (p1w->wu.h_hos->hos_box[0] != 0 && p2w->wu.h_hos->hos_box[0] != 0) {
        meri = hit_check_subroutine(&p1w->wu, &p2w->wu, &p1w->wu.h_hos->hos_box[0], &p2w->wu.h_hos->hos_box[0]);

        if (meri != 0) {
            meri = meri_case_switch(meri);

            if (p1w->wu.old_pos[1] < 1 && p2w->wu.old_pos[1] < 1) {
                if (ichikannkei) {
                    goto one;
                }

                goto two;
            }

            if (check_work_position(&p1w->wu, &p2w->wu)) {
                goto one;
            }

            goto two;
        }
    }

    p1w->hos_em_flag = 0;
    p2w->hos_em_flag = 0;
    return;

one:
    p1w->wu.xyz[0].disp.pos += meri * (p1w->micchaku_flag != 1);
    p2w->wu.xyz[0].disp.pos -= meri * (p2w->micchaku_flag != 2);
    p1w->hos_em_flag = 2;
    p2w->hos_em_flag = 1;
    return;

two:
    p1w->wu.xyz[0].disp.pos -= meri * (p1w->micchaku_flag != 2);
    p2w->wu.xyz[0].disp.pos += meri * (p2w->micchaku_flag != 1);
    p1w->hos_em_flag = 1;
    p2w->hos_em_flag = 2;
}

s16 meri_case_switch(s16 meri) {
    switch (meri & 0xFFF8) {
    case 0:
        if (meri < 4) {
            meri /= 2;
        } else {
            meri /= 4;
        }
        break;

    case 8:
        meri = (meri * 21) / 64;
        break;

    default:
        meri = (meri * 13) / 32;
        break;
    }

    return meri;
}

void check_body_touch2() {
#if defined(TARGET_PS2)
    s16 hoseishitemo_eenka(WORK * wk, s32 tx);
    s16 meri_case_switch(s32 meri);
    s16 check_work_position_bonus(WORK * hm, s32 tx);
#endif

    PLW* hmw;
    PLW* cmw;
    WORK* efw;
    s16* dad0;
    s16* dad1;
    s16 meri;
    s16 ix;
    s16 dad2[4];
    s16 dad3[4];

    if (plw->wu.operator) {
        hmw = &plw[0];
        cmw = &plw[1];
    } else {
        hmw = &plw[1];
        cmw = &plw[0];
    }

    if (!saishin_bs2_on_car(hmw)) {
        efw = (WORK*)cmw->wu.my_effadrs;
        ix = (sel_hosei_tbl_ix[hmw->player_number]) + 1 + ((efw->dir_timer == 1) * 2);
        dad0 = &hmw->wu.hosei_adrs[1].hos_box[0];
        dad1 = &efw->hosei_adrs[ix].hos_box[0];

        if (!hoseishitemo_eenka(&hmw->wu, efw->xyz[0].disp.pos + (dad1[0] + dad1[1] / 2))) {
            dad2[0] = dad0[0];
            dad2[1] = dad0[1];
            dad2[2] = dad0[2];
            dad2[3] = dad0[3];
            dad3[0] = dad1[0];
            dad3[1] = dad1[1];
            dad3[2] = dad1[2];
            dad3[3] = dad1[3];

            if (hmw->wu.cg_jphos) {
                dad2[2] += hmw->wu.cg_jphos;
                dad2[3] -= hmw->wu.cg_jphos;
            }

            if (efw->xyz[1].disp.pos) {
                dad3[2] -= efw->xyz[1].disp.pos;
            }

            meri = hit_check_subroutine(&hmw->wu, efw, &dad2[0], &dad3[0]);

            if (meri != 0) {
                meri = meri_case_switch(meri);

                if (!check_work_position_bonus(&hmw->wu, efw->xyz[0].disp.pos + (dad1[0] + dad1[1] / 2))) {
                    goto two;
                } else {
                    goto one;
                }
            }
        }
    }

    hmw->hos_em_flag = 0;
    cmw->hos_em_flag = 0;
    return;

one:
    hmw->wu.xyz[0].disp.pos += (meri) * (hmw->micchaku_flag != 1);
    hmw->hos_em_flag = 2;
    cmw->hos_em_flag = 1;
    return;

two:
    hmw->wu.xyz[0].disp.pos -= (meri) * (hmw->micchaku_flag != 2);
    hmw->hos_em_flag = 1;
    cmw->hos_em_flag = 2;
    return;
}

s32 check_be_car_object() {
    PLW* com;

    if (pcon_rno[0] == 0) {
        return 1;
    }

    if (plw[0].wu.operator) {
        com = &plw[1];
    } else {
        com = &plw[0];
    }

    if (com->wu.routine_no[0] <= 0) {
        return 1;
    }

    return ((PLW*)com->wu.my_effadrs)->wu.be_flag != 0;
}

s16 hoseishitemo_eenka(WORK* wk, s16 tx) {
    s16 rnum = 0;

    if (wk->cg_jphos + cal_top_of_position_y(wk) > bs2_floor[2] || wk->mvxy.a[1].real.h < 0) {
        switch ((wk->xyz[0].disp.pos < tx) + (wk->rl_flag != 0) * 2) {
        case 1:
        case 2:
            if (wk->mvxy.a[1].real.h > 0 && wk->mvxy.a[0].real.h < 0) {
                rnum = 1;
            }

            if (wk->mvxy.a[1].real.h < 0 && wk->mvxy.a[0].real.h > 0) {
                rnum = 1;
            }

            break;

        default:
            if (wk->mvxy.a[1].real.h > 0 && wk->mvxy.a[0].real.h > 0) {
                rnum = 1;
            }
        }
    }

    return rnum;
}

s16 get_sel_hosei_tbl_ix(s16 plnum) {
    return sel_hosei_tbl_ix[plnum];
}

s16 check_work_position_bonus(WORK* hm, s16 tx) {
    s16 result = hm->xyz[0].disp.pos - tx;
    s16 num;

    if (result) {
        if (result > 0) {
            num = 1;
        } else {
            num = 0;
        }
    } else {
        num = hm->rl_flag == 0;
    }

    return num;
}

s32 set_field_hosei_flag(PLW* pl, s16 pos, s16 ix) {
    s16 hami;

    while (1) {
        if (ix) {
            hami = pl->wu.xyz[0].disp.pos + satse[pl->player_number] - pos;

            if (hami) {
                if (hami >= 0) {
                    pl->wu.xyz[0].disp.pos -= hami;
                    pl->micchaku_flag = 1;
                    pl->hos_fi_flag = 1;
                    pl->hosei_amari = -hami;
                } else {
                    break;
                }
            } else {
                pl->micchaku_flag = 1;
                pl->hos_fi_flag = 0;
                pl->hosei_amari = 0;
            }
        } else {
            hami = pl->wu.xyz[0].disp.pos - satse[pl->player_number] - pos;

            if (hami) {
                if (hami <= 0) {
                    pl->wu.xyz[0].disp.pos -= hami;
                    pl->micchaku_flag = 2;
                    pl->hos_fi_flag = 2;
                    pl->hosei_amari = -hami;
                } else {
                    break;
                }
            } else {
                pl->micchaku_flag = 2;
                pl->hos_fi_flag = 0;
                pl->hosei_amari = 0;
            }
        }

        return 0;
    }

    pl->micchaku_flag = 0;
    pl->hos_fi_flag = 0;
    pl->hosei_amari = 0;
    return 1;
}

s16 check_work_position(WORK* p1, WORK* p2) {
    s16 result = p1->xyz[0].disp.pos - p2->xyz[0].disp.pos;
    s16 num;

    if (result) {
        if (result > 0) {
            num = 1;
        } else {
            num = 0;
        }
    } else if (p1->rl_flag + p2->rl_flag & 1) {
        if (p1->rl_flag) {
            num = 0;
        } else {
            num = 1;
        }
    } else {
        switch ((p1->xyz[1].disp.pos == 0) + (p2->xyz[1].disp.pos == 0) * 2) {
        case 1:
            if (p1->rl_flag) {
                num = 0;
            } else {
                num = 1;
            }
            break;

        case 2:
            if (p2->rl_flag) {
                num = 1;
            } else {
                num = 0;
            }
            break;

        default:
            num = 0;
            break;
        }
    }

    return num;
}

s32 random_32() {
    Random_ix32++;

    if (Debug_w[0x3B] == 0xE0) {
        Random_ix32 = 0;
    }

    Random_ix32 &= 0x7F;
    return random_tbl_32[Random_ix32];
}

s32 random_16() {
    Random_ix16++;

    if (Debug_w[0x3B] == 0xE0) {
        Random_ix16 = 0;
    }

    Random_ix16 &= 0x3F;
    return random_tbl_16[Random_ix16];
}

s32 random_32_ex() {
    Random_ix32_ex++;

    if (Debug_w[0x3B] == 0xE0) {
        Random_ix32_ex = 0;
    }

    Random_ix32_ex &= 0x1F;
    return random_tbl_32_ex[Random_ix32_ex];
}

s32 random_16_ex() {
    Random_ix16_ex++;

    if (Debug_w[0x3B] == 0xE0) {
        Random_ix16_ex = 0;
    }

    Random_ix16_ex &= 0xF;
    return random_tbl_16_ex[Random_ix16_ex];
}

s32 random_32_com() {
    if (Play_Mode == 0) {
        return random_32();
    }

    Random_ix32_com++;

    if (Debug_w[0x3B] == 0xE0) {
        Random_ix32_com = 0;
    }

    Random_ix32_com &= 0x7F;
    return random_tbl_32_com[Random_ix32_com];
}

s32 random_16_com() {
    if (Play_Mode == 0) {
        return random_16();
    }

    Random_ix16_com++;

    if (Debug_w[0x3B] == 0xE0) {
        Random_ix16_com = 0;
    }

    Random_ix16_com &= 0x3F;
    return random_tbl_16_com[Random_ix16_com];
}

s32 random_32_ex_com() {
    if (Play_Mode == 0) {
        return random_32_ex();
    }

    Random_ix32_ex_com++;

    if (Debug_w[0x3B] == 0xE0) {
        Random_ix32_ex_com = 0;
    }

    Random_ix32_ex_com &= 0x1F;
    return random_tbl_32_ex_com[Random_ix32_ex_com];
}

s32 random_16_ex_com() {
    if (Play_Mode == 0) {
        return random_16_ex();
    }

    Random_ix16_ex_com++;

    if (Debug_w[0x3B] == 0xE0) {
        Random_ix16_ex_com = 0;
    }

    Random_ix16_ex_com &= 0xF;
    return random_tbl_16_ex_com[Random_ix16_ex_com];
}

s32 random_16_bg() {
    Random_ix16_bg++;

    if (Debug_w[0x3B] == 0xE0) {
        Random_ix16_bg = 0;
    }

    Random_ix16_bg &= 0x3F;
    return random_tbl_16_bg[Random_ix16_bg];
}

s8 get_guard_direction(WORK* as, WORK* ds) {
    s16 result;
    s8 num;

    if (as->work_id == 1) {
        result = as->xyz[0].disp.pos - ds->xyz[0].disp.pos;

        if (result) {
            if (result < 0) {
                if (ds->rl_flag) {
                    num = 1;
                } else {
                    num = 2;
                }
            } else if (ds->rl_flag) {
                num = 2;
            } else {
                num = 1;
            }
        } else {
            num = 3;
        }
    } else if (((PLW*)ds)->spmv_ng_flag & 0x04000000) {
        if (as->rl_flag + ds->rl_flag & 1) {
            if (ds->work_id != 1) {
                num = 2;
            } else if (ds->rl_flag == ds->rl_waza) {
                num = 2;
            } else {
                num = 3;
            }
        } else {
            num = 3;
        }
    } else if (as->rl_flag + ds->rl_flag & 1) {
        num = 2;
    } else {
        num = 3;
    }

    return num;
}

s16 cal_attdir(WORK* wk) {
    s16 resdir = wk->att.dir;

    if (wk->rl_flag) {
        resdir = dir16_rl_conv[resdir];
    }

    return resdir;
}

s16 cal_attdir_flip(s16 dir) {
    return dir16_rl_conv[dir];
}

s16 get_kind_of_head_dm(s16 dir, s8 drl) {
    if (drl == 0) {
        dir = dir16_rl_conv[dir];
    }

    return dir16_hddm[dir];
}

s16 get_kind_of_trunk_dm(s16 dir, s8 drl) {
    if (drl == 0) {
        dir = dir16_rl_conv[dir];
    }

    return dir16_trdm[dir];
}

void setup_vitality(WORK* wk, s16 pno) {
    s16 ix;

    if (wk->operator) {
        ix = 2;
    } else {
        ix = CC_Value[1] + save_w[Present_Mode].Difficulty;
    }

    wk->original_vitality = Com_Vital_Unit_Data[pno][save_w[Present_Mode].Damage_Level][ix];
    wk->original_vitality += (s16)base_vital_omake[omop_vital_init[wk->id]];
    wk->dmcal_m = 32;
    wk->dmcal_d = (wk->original_vitality << 5) / Max_vitality;
    wk->vitality = wk->vital_new = wk->vital_old = Max_vitality;
    wk->dm_vital = 0;

    if (Mode_Type != 0) {
        wk->vital_new = wk->vital_new * (Vital_Handicap[Present_Mode][wk->id] + 1) / 8;
        wk->vital_old = wk->vital_new;
    }
}

void cal_dm_vital_gauge_hosei(PLW* wk) {
    s16 cnjix;

    if (wk->wu.dm_vital == 0) {
        return;
    }

    if (wk->wu.vital_new < (Max_vitality * 6) / 10) {
        if (Max_vitality == 192) {
            cnjix = wk->wu.vital_new / 19;
        } else {
            cnjix = wk->wu.vital_new / 16;
        }

        if (cnjix > 5) {
            cnjix = 5;
        }

        wk->wu.dm_vital = wk->wu.dm_vital * konjyou_tbl[wk->player_number][cnjix] / wk->wu.dmcal_m;
    }

    wk->wu.dm_vital = wk->wu.dm_vital * (32 - wk->tk_konjyou) / wk->wu.dmcal_m;
    wk->wu.dm_vital = wk->wu.dm_vital * wk->wu.dmcal_m / wk->wu.dmcal_d;

    if (wk->wu.dm_vital <= 0) {
        wk->wu.dm_vital = 1;
    }
}

void set_hit_stop_hit_quake(WORK* wk) {
    if (wk->dm_stop) {
        wk->hit_stop = wk->dm_stop;
        wk->dm_stop = 0;
    }

    if (wk->dm_quake) {
        wk->hit_quake = wk->dm_quake;
        wk->dm_quake = 0;
    }
}

void add_sp_arts_gauge_init(PLW* wk) {
#if defined(TARGET_PS2)
    void add_super_arts_gauge(SA_WORK * wk, s32 ix, s32 asag, u16 mf);
#endif

    PLW* mwk;
    s16 asag;

    if (wk->wu.work_id != 1) {
        mwk = (PLW*)wk->cp;

        if ((mwk->wu.work_id == 1) && !(mwk->spmv_ng_flag2 & 0x20000000)) {
            asag = _add_arts_gauge[mwk->player_number][wk->wu.add_arts_point][0];
            add_super_arts_gauge(mwk->sa, mwk->wu.id, asag, mwk->metamorphose);
        }
    } else if (!(wk->spmv_ng_flag2 & 0x20000000)) {
        asag = _add_arts_gauge[wk->player_number][wk->wu.add_arts_point][0];
        add_super_arts_gauge(wk->sa, wk->wu.id, asag, wk->metamorphose);
    }
}

void add_sp_arts_gauge_guard(PLW* wk) {
#if defined(TARGET_PS2)
    void add_super_arts_gauge(SA_WORK * wk, s32 ix, s32 asag, u32 mf);
#endif

    PLW* mwk;
    s16 asag;

    if (wk->wu.work_id != 1) {
        mwk = (PLW*)wk->cp;

        if (mwk->wu.work_id == 1) {
            asag = _add_arts_gauge[mwk->player_number][wk->wu.add_arts_point][1];
            add_super_arts_gauge(mwk->sa, mwk->wu.id, asag, mwk->metamorphose);
        }
    } else {
        asag = _add_arts_gauge[wk->player_number][wk->wu.add_arts_point][1];
        add_super_arts_gauge(wk->sa, wk->wu.id, asag, wk->metamorphose);
    }
}

void add_sp_arts_gauge_hit_dm(PLW* wk) {
#if defined(TARGET_PS2)
    void add_super_arts_gauge(SA_WORK * wk, s32 ix, s32 asag, u32 mf);
    s16 cal_sa_gauge_waribiki(PLW * wk, s32 asag);
#endif

    PLW* emwk;
    s16 asag;

    if (wk->wu.work_id != 1) {
        return;
    }

    emwk = (PLW*)wk->wu.target_adrs;
    asag = _add_arts_gauge[emwk->player_number][wk->wu.dm_arts_point][2];

    if (asag != 0) {
        add_super_arts_gauge(wk->sa, wk->wu.id, asag / 3, wk->metamorphose);

        if (emwk->wu.operator == 0) {
            asag += asagh_zuru[save_w[Present_Mode].Difficulty];
        }

        if (asag <= 0) {
            asag = 1;
        }

        asag = cal_sa_gauge_waribiki(wk, asag);

        if (emwk->wu.operator == 0 && Break_Into_CPU == 1) {
            asag = (asag * 120) / 100;
        }

        add_super_arts_gauge(emwk->sa, emwk->wu.id, asag, emwk->metamorphose);
    }

    wk->wu.dm_arts_point = 0;
}

s16 cal_sa_gauge_waribiki(PLW* wk, s16 asag) {
    s16 num;

    if (wk->cb->total < 2) {
        return asag;
    }

    num = 32 - (wk->cb->total - 1) * 2;

    if (num <= 0) {
        num = 1;
    }

    asag = (asag * num) / 32;

    if (asag == 0) {
        asag = 1;
    }

    return asag;
}

void add_sp_arts_gauge_paring(PLW* wk) {
#if defined(TARGET_PS2)
    void add_super_arts_gauge(SA_WORK * wk, s32 ix, s32 asag, u16 mf);
#endif

    PLW* emwk;
    s16 asag;

    if (sa_stop_check() != 0) {
        return;
    }

    if (wk->wu.work_id != 1) {
        return;
    }

    emwk = (PLW*)wk->wu.target_adrs;
    asag = _add_arts_gauge[emwk->player_number][wk->wu.dm_arts_point][3];

    if (asag != 0) {
        if (wk->wu.operator == 0) {
            asag += asagh_zuru[save_w[Present_Mode].Difficulty];
        }

        if (asag <= 0) {
            asag = 1;
        }

        add_super_arts_gauge(wk->sa, wk->wu.id, asag, wk->metamorphose);
    }

    wk->wu.dm_arts_point = 0;
}

void add_sp_arts_gauge_tokushu(PLW* wk) {
#if defined(TARGET_PS2)
    void add_super_arts_gauge(SA_WORK * wk, s32 ix, s32 asag, u16 mf);
#endif

    s16 asag;

    if (wk->wu.work_id != 1) {
        return;
    }

    asag = (apagt_table[(wk->player_number)]);

    if (asag == 0) {
        return;
    }

    if (wk->wu.operator == 0) {
        asag += asagh_zuru[save_w[Present_Mode].Difficulty];
    }

    if (asag <= 0) {
        asag = 1;
    }

    add_super_arts_gauge(wk->sa, wk->wu.id, asag, wk->metamorphose);
}

void add_sp_arts_gauge_ukemi(PLW* wk) {
#if defined(TARGET_PS2)
    void add_super_arts_gauge(SA_WORK * wk, s32 ix, s32 asag, u32 mf);
#endif

    s16 asag;

    if (wk->wu.work_id != 1) {
        return;
    }

    asag = 3;

    if (asag == 0) {
        return;
    }

    if (wk->wu.operator == 0) {
        asag += asagh_zuru[save_w[Present_Mode].Difficulty];
    }

    if (asag <= 0) {
        asag = (1);
    }

    add_super_arts_gauge(wk->sa, wk->wu.id, asag, wk->metamorphose);
}

void add_sp_arts_gauge_nagenuke(PLW* wk) {
#if defined(TARGET_PS2)
    void add_super_arts_gauge(SA_WORK * wk, s32 ix, s32 asag, u16 mf);
#endif

    s16 asag;

    if (wk->wu.work_id != 1) {
        return;
    }

    asag = 6;

    if (asag == 0) {
        return;
    }

    if (wk->wu.operator == 0) {
        asag += asagh_zuru[save_w[Present_Mode].Difficulty];
    }

    if (asag <= 0) {
        asag = 1;
    }

    add_super_arts_gauge(wk->sa, wk->wu.id, asag, wk->metamorphose);
}

void add_sp_arts_gauge_maxbit(PLW* wk) {
#if defined(TARGET_PS2)
    void add_super_arts_gauge(SA_WORK * wk, s32 ix, s32 asag, u16 mf);
#endif

    if (pcon_rno[0] != 1) {
        return;
    }

    if (wk->sa->mp == -1 || wk->sa->ok == -1 || wk->sa->ex == -1) {
        return;
    }

    if (sag_inc_timer[wk->wu.id]) {
        sag_inc_timer[wk->wu.id]--;
        return;
    }

    if (!(wk->spmv_ng_flag2 & 0x80000)) {
        sag_inc_timer[wk->wu.id] = 2;
        add_super_arts_gauge(wk->sa, wk->wu.id, wk->sa->gauge_len, wk->metamorphose);
        return;
    }

    if (!(wk->spmv_ng_flag2 & 0x10000)) {
        if (sag_inc_timer[wk->wu.id]) {
            sag_inc_timer[wk->wu.id]--;
            return;
        }

        add_super_arts_gauge(wk->sa, wk->wu.id, 1, wk->metamorphose);
    }
}

void add_super_arts_gauge(SA_WORK* wk, s16 ix, s16 asag, u8 mf) {
    if (!test_flag && !mf) {
        if ((wk->mp == -1) || (wk->ok == -1) || (wk->ex == -1)) {
            return;
        }

        if (!pcon_dp_flag && !Bonus_Game_Flag && (sa_gauge_omake[omop_sa_gauge_ix[ix]] != 0) && (asag > 0) &&
            (wk->store != wk->store_max)) {
            asag = asag * 0x78 / 100;

            if (save_w[Present_Mode].Battle_Number[Play_Type] == 0) {
                asag = asag * 0x96 / 100;
            }

            asag = asag * sa_gauge_omake[omop_sa_gauge_ix[ix]] / 32;

            if (asag == 0) {
                asag = 1;
            }

            wk->gauge.s.h += asag;
            wk->gauge.s.l = -1;

            if (wk->gauge.s.h > wk->gauge_len) {
                wk->store += 1;

                if (wk->store < wk->store_max) {
                    wk->gauge.s.h -= wk->gauge_len;
                } else {
                    wk->store = wk->store_max;
                    wk->gauge.i = 0;
                }

                sa_gauge_flash[ix] |= 1;
            }
        }
    }
}

s16 check_buttobi_type(PLW* wk) {
    s16 rn;

    setup_butt_own_data(&wk->wu);
    rn = dir32_skydm[cal_move_dir_forecast(&wk->wu, 5)];
    return rn;
}

s16 check_buttobi_type2(PLW* wk) {
    s16 rn;

    setup_butt_own_data(&wk->wu);
    rn = dir32_grddm[cal_move_dir_forecast(&wk->wu, 5)];
    return rn;
}

void setup_saishin_lvdir(PLW* ds, s8 gddir) {
    if ((ds->sa_stop_flag) == 1) {
        if (ds->wu.rl_flag) {
            ds->saishin_lvdir = convert_saishin_lvdir[1][ds->sa_stop_lvdir & 0xC];
        } else {
            ds->saishin_lvdir = convert_saishin_lvdir[0][ds->sa_stop_lvdir & 0xC];
        }
    } else if (ds->wu.rl_flag) {
        ds->saishin_lvdir = convert_saishin_lvdir[1][ds->cp->sw_lvbt & 0xC];
    } else {
        ds->saishin_lvdir = convert_saishin_lvdir[0][ds->cp->sw_lvbt & 0xC];
    }

    if (!(ds->spmv_ng_flag & 0x2000) && (ds->guard_chuu != 0) && ((ds->guard_chuu) < 5)) {
        ds->saishin_lvdir = gddir;
    }
}

void setup_lvdir_after_autodir(PLW* wk) {
    if (wk->wu.rl_flag) {
        wk->cp->lever_dir = convert_saishin_lvdir[1][wk->cp->sw_lvbt & 0xC];
        return;
    }

    wk->cp->lever_dir = convert_saishin_lvdir[0][wk->cp->sw_lvbt & 0xC];
}

void dead_voice_request() {
    if (dead_voice_flag) {
        if (plw[0].dead_flag) {
            dead_voice_request2(&plw[0]);
        }

        if (plw[1].dead_flag) {
            dead_voice_request2(&plw[1]);
        }
    }

    dead_voice_flag = 0;
}

void dead_voice_request2(PLW* wk) {
    s16 secd1;
    s16 secd2;
    s16 ks = 0;

    if (wk->metamorphose != 0 && Country != 8) {
        ks = 0x600;
    }

    secd1 = dead_voice_table[wk->player_number][0];
    secd2 = dead_voice_table[wk->player_number][1];

    if ((wk->wu.routine_no[1] == 1) && atsagct[wk->wu.routine_no[2]] & 0x10) {
        sound_effect_request[secd2](wk, secd2 + ks);
        return;
    }

    sound_effect_request[secd1](wk, secd1 + ks);
}
