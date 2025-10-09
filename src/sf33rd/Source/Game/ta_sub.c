#include "sf33rd/Source/Game/ta_sub.h"
#include "common.h"
#include "sf33rd/Source/Game/HITCHECK.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/workuser.h"
#include "structs.h"

#include "sf33rd/Source/Game/animation/lose_pl.h"
#include "sf33rd/Source/Game/animation/win_pl.h"

// sdata
s16 eff_hit_data[4][4] = { { -67, 59, 13, 29 }, { 31, 95, 24, 15 }, { 4, 123, 28, 15 }, { 20, 15, 67, 37 } };

// sbss
s16 eff_hit_flag[11];

s32 eff_hit_check_sub(WORK_Other* ewk, PLW* pl);
s32 eff_hit_check_sub2(WORK_Other* ewk, PLW* pl, s16 where_type);
static s16 hit_check_subroutine_yu(WORK* tpl, WORK* tef, s16* hd1, s16* hd2);

void sync_fam_set3(s16 my_fam) {
#if defined(TARGET_PS2)
    void Scrn_Move_Set(s32 bgnm, s32 x, s32 y);
    void Family_Set_W(s32 fmnm, s32 x, s32 y);
#endif

    s16 pos_work_x;
    s16 pos_work_y;
    s16 pos_x_w;
    s16 pos_y_w;

    if (bg_w.chase_flag & 0xF) {
        pos_x_w = bg_w.bgw[my_fam].chase_xy[0].disp.pos;
    } else {
        pos_x_w = bg_w.bgw[my_fam].wxy[0].disp.pos;
    }

    if (bg_w.chase_flag & 0xF0) {
        pos_y_w = bg_w.bgw[my_fam].chase_xy[1].disp.pos;
    } else {
        pos_y_w = bg_w.bgw[my_fam].xy[1].disp.pos;
    }

    pos_work_x = pos_x_w & 0xFFFF;
    pos_work_x -= bg_w.pos_offset;
    pos_x_w -= bg_w.pos_offset;

    if ((bg_w.quake_x_index) > 0) {
        pos_work_x += quake_x_tbl[bg_w.quake_x_index];
        pos_x_w += quake_x_tbl[bg_w.quake_x_index];
    }

    bg_w.bgw[my_fam].position_x = pos_work_x & 0xFFFF;
    bg_w.bgw[my_fam].abs_x = pos_x_w;
    pos_work_y = pos_y_w & 0xFFFF;
    pos_work_y += quake_y_tbl[bg_w.quake_y_index];
    pos_y_w += quake_y_tbl[bg_w.quake_y_index];
    bg_w.bgw[my_fam].position_y = pos_work_y & 0xFFFF;
    bg_w.bgw[my_fam].abs_y = pos_y_w;
    Scrn_Move_Set(my_fam, bg_w.bgw[my_fam].position_x, bg_w.bgw[my_fam].position_y);
    pos_work_x = -pos_work_x & 0xFFFF;
    pos_work_x &= 0xFFFF;
    pos_work_y = (768 - (pos_work_y & 0xFFFF)) & 0xFFFF;
    Family_Set_W(my_fam + 1, pos_work_x, pos_work_y);
}

s32 range_x_check(WORK_Other* ewk) {
    s16 pos_x_work;
    s16 work2;
    s16 work3;

    if (bg_w.chase_flag & 0xF) {
        pos_x_work = bg_w.bgw[ewk->wu.my_family - 1].chase_xy[0].disp.pos;
    } else {
        pos_x_work = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos;
    }

    if (ewk->wu.hit_stop == -77) {
        work2 = pos_x_work - 193;
        work3 = pos_x_work + 225;
    } else {
        work2 = pos_x_work - 193;
        work3 = pos_x_work + 193;
    }

    if (work2 > ewk->wu.xyz[0].disp.pos) {
        return 0;
    } else if (ewk->wu.xyz[0].disp.pos > work3) {
        return 0;
    }

    return 1;
}

s32 range_x_check3(WORK_Other* ewk, s16 optional_range) {
    s16 pos_x_work;
    s16 work2;
    s16 work3;

    if (bg_w.chase_flag & 0xF) {
        pos_x_work = bg_w.bgw[ewk->wu.my_family - 1].chase_xy[0].disp.pos;
    } else {
        pos_x_work = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos;
    }

    work2 = pos_x_work - 192 - optional_range;
    work3 = 192 + pos_x_work + optional_range;

    if (work2 > ewk->wu.xyz[0].disp.pos) {
        return 0;
    } else if (ewk->wu.xyz[0].disp.pos > work3) {
        return 0;
    }

    return 1;
}

s32 range_y_check(WORK_Other* ewk) {
    s16 pos_y_work;
    s16 work2;
    s16 work3;

    if (bg_w.chase_flag & 0xF) {
        pos_y_work = bg_w.bgw[ewk->wu.my_family - 1].chase_xy[1].disp.pos;
    } else {
        pos_y_work = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos;
    }

    work2 = pos_y_work + 256;
    work3 = pos_y_work - 32;

    if (work2 < ewk->wu.xyz[1].disp.pos) {
        return 0;
    } else if (work3 > ewk->wu.xyz[1].disp.pos) {
        return 0;
    }

    return 1;
}

void add_x_sub(WORK_Other* ewk) {
    ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
    ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;
}

void add_x_sub2(WORK_Other* ewk) {
    ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
    ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;
}

void add_y_sub(WORK_Other* ewk) {
    ewk->wu.xyz[1].cal += ewk->wu.mvxy.a[1].sp;
    ewk->wu.mvxy.a[1].sp += ewk->wu.mvxy.d[1].sp;
}

void add_y_sub2(WORK_Other* ewk) {
    ewk->wu.xyz[1].cal += ewk->wu.mvxy.a[1].sp;
    ewk->wu.mvxy.a[1].sp += ewk->wu.mvxy.d[1].sp;
}

s32 obr_no_disp_check() {
    if (aku_flag | akebono_flag | sa_pa_flag | seraph_flag) {
        return 1;
    }

    return 0;
}

void disp_pos_trans_entry(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    sort_push_request4(&ewk->wu);
}

void disp_pos_trans_entry5(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    sort_push_request4(&ewk->wu);
}

void disp_pos_trans_entry_r(WORK_Other* ewk) {
    if ((obr_no_disp_check() == 0) && (range_x_check(ewk) != 0)) {
        ewk->wu.position_x = (ewk->wu.xyz[0].disp.pos & 0xFFFF) + 1;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
        sort_push_request4(&ewk->wu);
    }
}

void disp_pos_trans_entry_r4(WORK_Other* ewk) {
    if ((obr_no_disp_check() == 0) && (range_y_check(ewk) != 0)) {
        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
        sort_push_request4(&ewk->wu);
    }
}

void disp_pos_trans_entry_s(WORK_Other* ewk) {
    if (obr_no_disp_check() == 0) {
        suzi_sync_pos_set(ewk);
        ewk->wu.position_x++;
        sort_push_request4(&ewk->wu);
    }
}

void disp_pos_trans_entry_rs(WORK_Other* ewk) {
    if ((obr_no_disp_check() == 0) && (range_x_check(ewk) != 0)) {
        suzi_sync_pos_set(ewk);
        ewk->wu.position_x++;
        sort_push_request4(&ewk->wu);
    }
}

void pl_eff_trans_entry(WORK_Other* ewk) {
    if (obr_no_disp_check() == 0) {
        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
        sort_push_request(&ewk->wu);
    }
}

s16 eff_hit_check(WORK_Other* ewk, s16 type) {
    if (!EXE_obroll) {
        if (type) {
            if (pcon_dp_flag) {
                eff_hit_flag[ewk->wu.type] += eff_hit_check_sub(ewk, &plw[0]);
                eff_hit_flag[ewk->wu.type] += eff_hit_check_sub(ewk, &plw[1]);
            }
        } else {
            eff_hit_flag[ewk->wu.type] += eff_hit_check_sub(ewk, &plw[0]);
            eff_hit_flag[ewk->wu.type] += eff_hit_check_sub(ewk, &plw[1]);
        }
    }

    return eff_hit_flag[ewk->wu.type];
}

const s16 pl_hit_eff[25][4] = { { -11, 56, 33, 38 }, { -11, 56, 35, 53 }, { -13, 47, 50, 38 }, { -18, 42, 36, 32 },
                                { -24, 48, 40, 48 }, { -21, 48, 37, 42 }, { -13, 47, 50, 38 }, { -22, 38, 36, 36 },
                                { -13, 47, 50, 38 }, { -28, 50, 28, 34 }, { -18, 42, 36, 32 }, { -13, 47, 50, 38 },
                                { -13, 47, 50, 38 }, { -11, 56, 33, 38 }, { -13, 47, 50, 38 }, { -13, 47, 50, 38 },
                                { -13, 47, 50, 38 }, { -13, 47, 50, 38 }, { -21, 48, 37, 42 }, { -11, 56, 33, 38 },
                                { -11, 56, 33, 38 }, { -11, 56, 33, 38 }, { -11, 56, 33, 38 }, { -11, 56, 33, 38 },
                                { -11, 56, 33, 38 } };

s32 eff_hit_check_sub(WORK_Other* ewk, PLW* pl) {
    if (pl->wu.routine_no[1] == 1) {
        if (pl->wu.routine_no[2] < 14 || pl->wu.routine_no[2] >= 24) {
            return 0;
        }

        if (hit_check_subroutine(
                &pl->wu, &ewk->wu, &pl_hit_eff[pl->player_number][0], &eff_hit_data[ewk->wu.type][0])) {
            return 1;
        }
    }

    return 0;
}

s16 eff_hit_check2(WORK_Other* ewk, s16 type, s16 where_type) {
    if (!EXE_obroll) {
        if (type) {
            if (pcon_dp_flag) {
                eff_hit_flag[ewk->wu.type] += eff_hit_check_sub2(ewk, &plw[0], where_type);
                eff_hit_flag[ewk->wu.type] += eff_hit_check_sub2(ewk, &plw[1], where_type);
            }
        } else {
            eff_hit_flag[ewk->wu.type] += eff_hit_check_sub2(ewk, &plw[0], where_type);
            eff_hit_flag[ewk->wu.type] += eff_hit_check_sub2(ewk, &plw[1], where_type);
        }
    }

    return eff_hit_flag[ewk->wu.type];
}

s32 eff_hit_check_sub2(WORK_Other* ewk, PLW* pl, s16 where_type) {
    s16* hd1 = pl->wu.h_bod->body_dm[where_type];

    if (hit_check_subroutine_yu(&pl->wu, &ewk->wu, hd1, eff_hit_data[ewk->wu.type])) {
        return 1;
    }

    return 0;
}

static s16 hit_check_subroutine_yu(WORK* tpl, WORK* tef, s16* hd1, s16* hd2) {
    s16 d0 = *hd1++;
    s16 d1 = *hd1++;
    s16 d2;
    s16 d3;
    s16 flag;

    if (tpl->rl_flag) {
        d0 = -d0;
        d0 -= d1;
    }

    d0 += tpl->xyz[0].disp.pos;
    d2 = *hd2++;
    d3 = *hd2++;

    if (tef->rl_flag) {
        d2 = -d2;
        d2 -= d3;
    }

    d2 += tef->xyz[0].disp.pos;
    flag = (d0 < d2);
    d2 += (d3 - d0);
    d3 += d1;

    if ((u32)d2 >= d3) {
        return 0;
    }

    d0 = (tpl->xyz[1].disp.pos + *hd1++) - (tef->xyz[1].disp.pos + *hd2++ - 40);
    d0 += d1 = *hd1;
    d1 += *hd2;

    if ((u32)d0 >= d1) {
        return 0;
    } else if (flag) {
        d2 = d3 - d2;
    }

    return d2;
}

void eff_hit_flag_clear() {
    s16 i;
    s16* ptr;

    ptr = &eff_hit_flag[0];

    for (i = 0; i < 0xB; i++) {
        *ptr++ = 0;
    }
}

s32 compel_dead_check(WORK_Other* ewk) {
    s32 var_s0 = 0;

    if (bg_w.compel_flag) {
        if (ewk->wu.dead_f) {
            var_s0 = 1;
        }
    }

    return var_s0;
}

void win_lose_work_clear() {
    a_rno = 0;
    lose_rno[2] = 0;
    win_rno[0] = 0;
    win_free[0] = 0;
    lose_rno[0] = 0;
    lose_free[0] = 0;
    win_rno[1] = 0;
    win_free[1] = 0;
    lose_rno[1] = 0;
    lose_free[1] = 0;
}

void cal_bg_speed_data_x(s16 bg_num, s16 tm, s16 unk) {
    MotionState ms;

    bg_w.bgw[bg_num].chase_xy[0].disp.low = 0;
    ms.timer = tm;
    ms.timer2 = ((ms.timer * (ms.timer - 1)) / 2) + ms.timer;
    ms.x.ps.h = chase_x - bg_w.bgw[bg_num].chase_xy[0].disp.pos;
    ms.x.ps.l = 0;

    if (!ms.timer) {
        ms.amx = 0;
        ms.dlx = 0;
        ms.spx = 0;
    } else {
        ms.amx = ms.x.pl % ms.timer2;
        ms.spx = ms.dlx = ms.x.pl / ms.timer2;
    }

    bg_mvxy.a[0].sp = ms.spx;
    bg_mvxy.d[0].sp = ms.dlx;
    bg_w.bgw[bg_num].chase_xy[0].cal += ms.amx;
    bg_mvxy.kop[0] = 0;
}

void cal_bg_speed_data_y(s16 bg_num, s16 tm, s16 unk) {
    MotionState ms;

    bg_w.bgw[bg_num].chase_xy[1].disp.low = 0;
    ms.timer = tm;
    ms.timer2 = ((ms.timer * (ms.timer - 1)) / 2) + ms.timer;
    ms.y.ps.h = chase_y - bg_w.bgw[bg_num].chase_xy[1].disp.pos;
    ms.y.ps.l = 0;

    if (!ms.timer) {
        ms.amy = 0;
        ms.dly = 0;
        ms.spy = 0;
    } else {
        ms.amy = ms.y.pl % ms.timer2;
        ms.spy = ms.dly = ms.y.pl / ms.timer2;
    }

    bg_mvxy.a[1].sp = ms.spy;
    bg_mvxy.d[1].sp = ms.dly;
    bg_w.bgw[bg_num].chase_xy[1].cal += ms.amy;
    bg_mvxy.kop[1] = 0;
}
