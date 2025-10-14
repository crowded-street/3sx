#include "sf33rd/Source/Game/bg_sub.h"
#include "common.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/workuser.h"
#include "structs.h"

void (*scr_x_mv_jp[35])() = { scr_10_20,   scr_10_21,   scr_10_22,   scr_x_dummy, scr_x_dummy, scr_x_dummy, scr_x_dummy,
                              scr_x_dummy, scr_x_dummy, scr_x_dummy, scr_x_dummy, scr_x_dummy, scr_x_dummy, scr_x_dummy,
                              scr_x_dummy, scr_x_dummy, scr_11_20,   scr_11_21,   scr_11_22,   scr_x_dummy, scr_x_dummy,
                              scr_x_dummy, scr_x_dummy, scr_x_dummy, scr_x_dummy, scr_x_dummy, scr_x_dummy, scr_x_dummy,
                              scr_x_dummy, scr_x_dummy, scr_x_dummy, scr_x_dummy, scr_12_20,   scr_12_21,   scr_12_22 };

// Forward decls
static s16 remake_x_mvstep(s16 mvstep);
static s32 suzi_offset_set_sub(WORK_Other* ewk);

void check_cg_zoom() {
    s16 i;
    s16 zoom_wk;
    u16 p1zoom;
    u16 p2zoom;
    u16 zmlv;
    u16 lookp1;
    u16 lookp2;

    p1zoom = plw[0].wu.cg_zoom;
    p2zoom = plw[1].wu.cg_zoom;

    if (bg_stop != 0 && !((p1zoom | p2zoom) & 0x4000)) {
        zmlv = p1zoom & 0xFF;

        if (zmlv < (p2zoom & 0xFF)) {
            zmlv = p2zoom & 0xFF;
        }

        lookp1 = p1zoom >> 8 & 3;
        lookp2 = p2zoom >> 8 & 3;
        p1zoom = zmlv | (lookp2 << 12 | lookp1 << 8);
        p2zoom = zmlv | (lookp1 << 12 | lookp2 << 8);
    }

    zoom_req_flag_old = zoom_request_flag;
    zoom_request_flag = 0;

    for (i = 0; i < 2; i++) {
        if (plw[i].scr_pos_set_flag) {
            plw[i].wu.scr_mv_x = plw[i].wu.xyz[0].disp.pos;
            plw[i].wu.scr_mv_y = plw[i].wu.xyz[1].disp.pos;
        } else if (plw[i].tsukamare_f) {
            plw[i].wu.scr_mv_x = plw[i + 1 & 1].wu.xyz[0].disp.pos;
            plw[i].wu.scr_mv_y = plw[i + 1 & 1].wu.xyz[1].disp.pos;
        }
    }

    zoom_wk = p2zoom & 0xE200;

    switch (p1zoom & 0xE200) {
    case 0x4000:
        break;

    case 0x2000:
        switch (zoom_wk) {
        case 0x4000:
            zoom_request_flag = 0x100;
            scr_req_x = plw[0].wu.xyz[0].disp.pos;
            break;

        case 0x2000:
            zoom_request_flag = 0x100;
            scr_req_x = (plw[0].wu.xyz[0].disp.pos + plw[1].wu.xyz[0].disp.pos) >> 1;
            break;

        case 0x200:
        case 0x0:
        case 0x2200:
            zoom_request_flag = 0x100;
            scr_req_x = plw[1].wu.xyz[0].disp.pos;
            break;
        }
        break;

    case 0x200:
        switch (zoom_wk) {
        case 0x2000:
        case 0x0:
        case 0x4000:
        case 0x2200:
            zoom_request_flag = 0x100;
            scr_req_x = plw[0].wu.xyz[0].disp.pos;
            break;

        case 0x200:
            zoom_request_flag = 0x100;
            scr_req_x = (plw[0].wu.xyz[0].disp.pos + plw[1].wu.xyz[0].disp.pos) >> 1;
            break;
        }
        break;

    case 0x0:
        switch (zoom_wk) {
        case 0x2200:
            zoom_request_flag = 0x100;
            scr_req_x = (plw[0].wu.xyz[0].disp.pos + plw[1].wu.xyz[0].disp.pos) >> 1;
            break;

        case 0x2000:
            zoom_request_flag = 0x100;
            scr_req_x = plw[0].wu.xyz[0].disp.pos;
            break;

        case 0x200:
            zoom_request_flag = 0x100;
            scr_req_x = plw[1].wu.xyz[0].disp.pos;
            break;

        case 0x4000:
        case 0x0:
            break;
        }
        break;

    case 0x2200:
        switch (zoom_wk) {
        case 0x0:
        case 0x2200:
        case 0x4000:
            zoom_request_flag = 0x100;
            scr_req_x = (plw[0].wu.xyz[0].disp.pos + plw[1].wu.xyz[0].disp.pos) >> 1;
            break;
        case 0x2000:
            zoom_request_flag = 0x100;
            scr_req_x = plw[0].wu.xyz[0].disp.pos;
            break;
        case 0x200:
            zoom_request_flag = 0x100;
            scr_req_x = plw[1].wu.xyz[0].disp.pos;
            break;

            break;
        }
        break;
    }

    zoom_wk = p2zoom & 0xD100;

    switch (p1zoom & 0xD100) {
    case 0x4000:
        zoom_request_flag |= 0x1000;
        scr_req_y = 0;
        break;

    case 0x1000:
        switch (zoom_wk) {
        case 0x1000:
            zoom_request_flag |= 0x1000;
            scr_req_y = (plw[0].wu.xyz[1].disp.pos + plw[1].wu.xyz[1].disp.pos) >> 1;
            break;

        case 0x100:
        case 0x0:
        case 0x1100:
            zoom_request_flag |= 0x1000;
            scr_req_y = plw[1].wu.xyz[1].disp.pos;
            break;

        case 0x4000:
            zoom_request_flag |= 0x1000;
            scr_req_y = 0;
            break;
        }
        break;

    case 0x100:
        switch (zoom_wk) {
        case 0x1000:
        case 0x0:
        case 0x1100:
            zoom_request_flag |= 0x1000;
            scr_req_y = plw[0].wu.xyz[1].disp.pos;
            break;

        case 0x100:
            zoom_request_flag |= 0x1000;
            scr_req_y = (plw[0].wu.xyz[1].disp.pos + plw[1].wu.xyz[1].disp.pos) >> 1;
            break;

        case 0x4000:
            zoom_request_flag |= 0x1000;
            scr_req_y = 0;
            break;
        }
        break;

    case 0x0:
        switch (zoom_wk) {
        case 0x1000:
            zoom_request_flag |= 0x1000;
            scr_req_y = plw[0].wu.xyz[1].disp.pos;
            break;

        case 0x100:
            zoom_request_flag |= 0x1000;
            scr_req_y = plw[1].wu.xyz[1].disp.pos;
            break;

        case 0x1100:
            zoom_request_flag |= 0x1000;
            scr_req_y = (plw[0].wu.xyz[1].disp.pos + plw[1].wu.xyz[1].disp.pos) >> 1;
            break;

        case 0x0:
            break;

        case 0x4000:
            zoom_request_flag |= 0x1000;
            scr_req_y = 0;
            break;
        }
        break;

    case 0x1100:
        switch (zoom_wk) {
        case 0x1000:
            zoom_request_flag |= 0x1000;
            scr_req_y = plw[0].wu.xyz[1].disp.pos;
            break;

        case 0x100:
            zoom_request_flag |= 0x1000;
            scr_req_y = plw[1].wu.xyz[1].disp.pos;
            break;

        case 0x1100:
        case 0x0:
            zoom_request_flag |= 0x1000;
            scr_req_y = (plw[0].wu.xyz[1].disp.pos + plw[1].wu.xyz[1].disp.pos) >> 1;
            break;

        case 0x4000:
            zoom_request_flag |= 0x1000;
            scr_req_y = 0;
            break;
        }
        break;
    }

    zoom_request_level = p1zoom & 0xFF;

    if (zoom_request_level < (p2zoom & 0xFF)) {
        zoom_request_level = p2zoom & 0xFF;
    }

    if (zoom_request_level) {
        zoom_request_flag |= 1;
    }
}

void bg_chase_move() {
    if (!Bonus_Game_Flag) {
        chase_start_check();

        if (bg_w.chase_flag) {
            chase_xy_move();
        }
    }
}

void chase_start_check() {
#if defined(TARGET_PS2)
    void cal_bg_speed_data_x(s32 /* unused */, s32 bg_num, s32 tm);
    void cal_bg_speed_data_y(s32 /* unused */, s32 bg_num, s32 tm);
#endif

    s16 work;
    s16 work2;

    if (zoom_request_flag & 0xF00) {
        if (chase_x != scr_req_x) {
            chase_x = scr_req_x;

            if (bgw_ptr->zuubun) {
                bgw_ptr->chase_xy[0].disp.pos = bgw_ptr->abs_x + bg_w.pos_offset;
            } else {
                bgw_ptr->chase_xy[0].disp.pos = bgw_ptr->position_x + bg_w.pos_offset;
            }

            chase_time_x = 6;
            cal_bg_speed_data_x(bgw_ptr->fam_no, chase_time_x, chase_x);
            bg_w.chase_flag |= 1;
            bg_w.chase_flag &= ~2;
            bg_w.old_chase_flag = 1;
        }
    } else {
        work = zoom_req_flag_old & 0xF00;
        work2 = ~(zoom_request_flag & 0xF00);
        work &= work2;

        if (work) {
            bg_w.chase_flag |= 2;
            bg_w.chase_flag &= ~1;
            chase_x = bgw_ptr->wxy[0].disp.pos;
            chase_time_x = 6;
            cal_bg_speed_data_x(bgw_ptr->fam_no, chase_time_x, chase_x);
        }
    }

    if (zoom_request_flag & 0xF000) {
        bg_w.chase_flag |= 0x10;
        bg_w.chase_flag &= ~0x20;
        bg_w.old_chase_flag |= 0x10;
        bg_w.old_chase_flag &= ~0x20;

        if (chase_y != scr_req_y) {
            chase_y = scr_req_y;

            if (bgw_ptr->abs_y < 0) {
                bgw_ptr->chase_xy[1].disp.pos = 0;
            } else {
                bgw_ptr->chase_xy[1].disp.pos = bgw_ptr->abs_y;
            }

            chase_time_y = 6;
            cal_bg_speed_data_y(bgw_ptr->fam_no, chase_time_y, chase_y);
        }
    } else {
        work = zoom_req_flag_old & 0xF000;
        work2 = ~(zoom_request_flag & 0xF000);
        work &= work2;

        if (work) {
            bg_w.chase_flag |= 0x20;
            bg_w.chase_flag &= ~0x10;
            chase_y = bgw_ptr->xy[1].disp.pos;
            chase_time_y = 6;
            cal_bg_speed_data_y(bgw_ptr->fam_no, chase_time_y, chase_y);
        }
    }
}

void chase_xy_move() {
    if (bg_w.chase_flag & 0xF) {
        bg_w.bg2_sp_x2 = bg_w.bg2_sp_x = 0;

        if (bg_w.chase_flag & 1) {
            chase_time_x -= 1;

            if (chase_time_x > 0) {
                bg_mvxy.a[0].sp += bg_mvxy.d[0].sp;
                bgw_ptr->chase_xy[0].cal += bg_mvxy.a[0].sp;
            }
        }

        if (bg_w.chase_flag & 2) {
            chase_time_x -= 1;

            if (chase_time_x > 0) {
                bg_mvxy.a[0].sp += bg_mvxy.d[0].sp;
                bgw_ptr->chase_xy[0].cal += bg_mvxy.a[0].sp;
            } else {
                bg_w.chase_flag &= ~0xF;
                bg_w.old_chase_flag &= ~0xF;
                bgw_ptr->chase_xy[0].disp.pos = chase_x;
            }
        }

        if (bgw_ptr->chase_xy[0].disp.pos > bgw_ptr->r_limit2) {
            bgw_ptr->chase_xy[0].disp.pos = bgw_ptr->r_limit2;
            bgw_ptr->chase_xy[0].disp.low = 0;
        }

        if (bgw_ptr->chase_xy[0].disp.pos < bgw_ptr->l_limit2) {
            bgw_ptr->chase_xy[0].disp.pos = bgw_ptr->l_limit2;
            bgw_ptr->chase_xy[0].disp.low = 0;
        }

        bg_w.bg2_sp_x = bg_w.bg2_sp_x2 = bgw_ptr->chase_xy[0].disp.pos - bgw_ptr->pos_x_work;
    }

    if (bg_w.chase_flag & 0xF0) {
        if (bg_w.chase_flag & 0x10) {
            chase_time_y -= 1;

            if (chase_time_y > 0) {
                bg_mvxy.a[1].sp += bg_mvxy.d[1].sp;
                bgw_ptr->chase_xy[1].cal += bg_mvxy.a[1].sp;
            }
        }

        if (bg_w.chase_flag & 0x20) {
            chase_time_y -= 1;

            if (chase_time_y > 0) {
                bg_mvxy.a[1].sp += bg_mvxy.d[1].sp;
                bgw_ptr->chase_xy[1].cal += bg_mvxy.a[1].sp;
            } else {
                bg_w.chase_flag &= 0xF;
                bg_w.old_chase_flag &= 0xF;
                bgw_ptr->chase_xy[1].disp.pos = chase_y;
            }
        }

        if (bgw_ptr->chase_xy[1].disp.pos > bgw_ptr->y_limit2) {
            bgw_ptr->chase_xy[1].disp.pos = bgw_ptr->y_limit2;
            bgw_ptr->chase_xy[1].disp.low = 0;
        }

        bg_w.bg2_sp_y = bgw_ptr->chase_xy[1].disp.pos - bgw_ptr->pos_y_work;
    }
}

void Bg_mv_tw(s32 value_x, s32 value_y) {
    bgw_ptr->xy[0].cal += value_x;
    bgw_ptr->xy[1].cal += value_y;
    bgw_ptr->wxy[0].cal += value_x;
    bgw_ptr->wxy[1].cal += value_y;
}

void x_right_check(s16 d1) {
    s32 speed_w;

    bg_w.scr_stop &= ~3;
    speed_w = bgw_ptr->speed_x * d1;
    ideal_w.iw[0].cal += speed_w;
}

void x_left_check(s16 d0) {
    s32 speed_w;

    bg_w.scr_stop &= ~3;
    speed_w = bgw_ptr->speed_x * d0;
    ideal_w.iw[0].cal += speed_w;
}

void scr_x_dummy() {}

void scr_10_20() {}

void scr_10_21() {
    s16 meri;
    meri = plw[1].wu.scr_mv_x - satse[plw[1].player_number];
    meri = meri - (ideal_w.iw[0].disp.pos - bg_w.pos_offset + 0x40);

    x_left_check(meri);
}

void scr_10_22() {
    s16 meri;
    meri = plw[1].wu.scr_mv_x + satse[plw[1].player_number];
    meri = meri - (ideal_w.iw[0].disp.pos + bg_w.pos_offset - 0x3F);

    x_right_check(meri);
}

void scr_11_20() {
    s16 meri;
    meri = plw[0].wu.scr_mv_x - satse[plw[0].player_number];
    meri = meri - (ideal_w.iw[0].disp.pos - bg_w.pos_offset + 0x40);

    x_left_check(meri);
}

void scr_11_21() {
    s16 meri;

    if (plw[0].wu.scr_mv_x < plw[1].wu.scr_mv_x) {
        meri = plw[0].wu.scr_mv_x - satse[plw[0].player_number];
    } else {
        meri = plw[1].wu.scr_mv_x - satse[plw[1].player_number];
    }

    meri = meri - (ideal_w.iw[0].disp.pos - bg_w.pos_offset + 0x40);

    x_left_check(meri);
}

void scr_11_22() {
    s16 meri;
    s16 meri2;

    meri = (satse[plw[1].player_number] - satse[plw[0].player_number]);
    meri >>= 1;
    meri2 = plw[0].wu.scr_mv_x + plw[1].wu.scr_mv_x;
    meri2 >>= 1;
    meri2 += meri;
    meri2 -= ideal_w.iw[0].disp.pos;

    if (meri2 < 0) {
        if (plw[1].micchaku_flag != 1) {
            x_left_check(meri2);
        }
    } else if (plw[0].micchaku_flag != 2) {
        x_right_check(meri2);
    }
}

void scr_12_20() {
    s16 meri;
    meri = plw[0].wu.scr_mv_x + satse[plw[0].player_number];
    meri = meri - (ideal_w.iw[0].disp.pos + bg_w.pos_offset - 0x3F);

    x_right_check(meri);
}

void scr_12_21() {
    s16 meri;
    s16 meri2;

    meri = (satse[plw[0].player_number] - satse[plw[1].player_number]);
    meri >>= 1;
    meri2 = plw[0].wu.scr_mv_x + plw[1].wu.scr_mv_x;
    meri2 >>= 1;
    meri2 += meri;
    meri2 -= ideal_w.iw[0].disp.pos;

    if (meri2 < 0) {
        if (plw[0].micchaku_flag != 1) {
            x_left_check(meri2);
        }
    } else if (plw[1].micchaku_flag != 2) {
        x_right_check(meri2);
    }
}

void scr_12_22() {
    s16 meri;

    if (plw[0].wu.scr_mv_x > plw[1].wu.scr_mv_x) {
        meri = plw[0].wu.scr_mv_x + satse[plw[0].player_number];
    } else {
        meri = plw[1].wu.scr_mv_x + satse[plw[1].player_number];
    }

    meri = meri - (ideal_w.iw[0].disp.pos + bg_w.pos_offset - 0x3F);

    x_right_check(meri);
}

void bg_base_x_move_sub() {
    s16 work[2];
    s8 st[2];
    s8 i;
    s16 bg_pos;

    bg_pos = ideal_w.iw[0].disp.pos - bg_w.pos_offset;

    if (plw[0].wu.scr_mv_x < ideal_w.iw[0].disp.pos) {
        work[0] = plw[0].wu.scr_mv_x - *&satse[plw[0].player_number];
    } else {
        work[0] = plw[0].wu.scr_mv_x + *&satse[plw[0].player_number];
    }

    work[0] -= bg_pos;

    if (plw[1].wu.scr_mv_x < ideal_w.iw[0].disp.pos) {
        work[1] = plw[1].wu.scr_mv_x - *&satse[plw[1].player_number];
    } else {
        work[1] = plw[1].wu.scr_mv_x + *&satse[plw[1].player_number];
    }

    work[1] -= bg_pos;

    if (work[0] < 0) {
        work[0] = 0;
    } else if (work[0] > 0x17F) {
        work[0] = 0x17F;
    }

    if (work[1] < 0) {
        work[1] = 0;
    } else if (work[1] > 0x17F) {
        work[1] = 0x17F;
    }

    for (i = 0; i < 2; i++) {
        if (0 <= work[i] && work[i] < 0x40) {
            st[i] = 1;
        } else if (work[i] >= 0x140 && work[i] < 0x180) {
            st[i] = 2;
        } else {
            st[i] = 0;
        }
    }

    scr_x_mv_jp[(st[0] << 4) + st[1]]();
}

void bg_base_x_move_check() {
    s16 mvstep, old_work;

    bg_w.bg2_sp_x2 = bg_w.bg2_sp_x = 0;

    if (!bg_stop && !bg_app_stop) {
        if (bg_w.chase_flag & 0xF) {
            bgw_ptr->old_pos_x = bgw_ptr->chase_xy[0].disp.pos;
        } else {
            bgw_ptr->old_pos_x = bgw_ptr->wxy[0].disp.pos;
        }

        old_work = bgw_ptr->wxy[0].disp.pos;
        ideal_w.iw[0].cal = bgw_ptr->wxy[0].cal;
        bg_base_x_move_sub();
        mvstep = ideal_w.iw[0].disp.pos;
        mvstep -= old_work;
        ideal_w.iw[0].cal = 0;
        ideal_w.iw[0].disp.pos = mvstep;

        if (mvstep) {
            if (mvstep < 0) {
                if (mvstep < -bg_w.max_x) {
                    mvstep = -bg_w.max_x;
                }
                mvstep = -remake_x_mvstep(-mvstep);
            } else {
                if (mvstep > bg_w.max_x) {
                    mvstep = bg_w.max_x;
                }
                mvstep = remake_x_mvstep(mvstep);
            }
        }

        ideal_w.iw[0].disp.pos = mvstep;
        Bg_mv_tw(ideal_w.iw[0].cal, 0);

        if (bgw_ptr->wxy[0].disp.pos < bgw_ptr->l_limit2) {
            bgw_ptr->wxy[0].disp.pos = bgw_ptr->l_limit2;
            bgw_ptr->wxy[0].disp.low = 0;
            bgw_ptr->xy[0].disp.pos = bgw_ptr->l_limit2;
            bgw_ptr->xy[0].disp.low = 0;
        }

        if (bgw_ptr->wxy[0].disp.pos > bgw_ptr->r_limit2) {
            bgw_ptr->wxy[0].disp.pos = bgw_ptr->r_limit2;
            bgw_ptr->wxy[0].disp.low = 0;
            bgw_ptr->xy[0].disp.pos = bgw_ptr->r_limit2;
            bgw_ptr->xy[0].disp.low = 0;
        }
    }

    bg_w.bg2_sp_x = bgw_ptr->xy[0].disp.pos - bgw_ptr->pos_x_work;
    bg_w.bg2_sp_x2 = bgw_ptr->wxy[0].disp.pos - bgw_ptr->pos_x_work;

    if (!(bg_w.chase_flag & 0xF)) {
        bgw_ptr->chase_xy[0].disp.pos = bgw_ptr->wxy[0].disp.pos;
    }
}

s16 remake_x_mvstep(s16 mvstep) {
    return mvstep * 0x50 / 100;
}

void Bg_Y_Sitei(u8 on_off, s16 pos) {
    y_sitei_flag = on_off;

    if (on_off == 0) {
        y_sitei_pos = 0;
        return;
    }

    y_sitei_pos = pos;
}

void bg_base_y_move_check() {
    s32 pos_w, kake;
    s16 hi_pos;

    if (y_sitei_flag == 1) {
        hi_pos = y_sitei_pos;
    } else {
        if (bg_stop)
            goto end;
        if (bg_app_stop)
            goto end;

        if (plw[0].wu.scr_mv_y > plw[1].wu.scr_mv_y) {
            hi_pos = plw[0].wu.scr_mv_y;
        } else {
            hi_pos = plw[1].wu.scr_mv_y;
        }
    }

    hi_pos -= 0x58;

    if (hi_pos <= 0) {
        bgw_ptr->wxy[1].cal = 0;
        bgw_ptr->xy[1].cal = 0;
    } else {
        kake = 0x1C000;
        pos_w = kake * hi_pos;

        bgw_ptr->xy[1].cal = 0;
        bgw_ptr->wxy[1].cal = 0;

        bgw_ptr->xy[1].cal += pos_w;
        bgw_ptr->wxy[1].cal += pos_w;

        if (bgw_ptr->xy[1].disp.pos > bgw_ptr->y_limit2) {
            bgw_ptr->xy[1].disp.pos = bgw_ptr->y_limit2;
            bgw_ptr->xy[1].disp.low = 0;
            bgw_ptr->wxy[1].disp.pos = bgw_ptr->y_limit2;
            bgw_ptr->wxy[1].disp.low = 0;
            bg_w.scr_stop &= 0x7FFF;
        }
    }
end:
    bg_w.bg2_sp_y = bgw_ptr->xy[1].disp.pos - bgw_ptr->pos_y_work;
}

void bg_x_move_check() {
    if (bg_w.chase_flag & 0xF) {
        bgw_ptr->old_pos_x = bgw_ptr->chase_xy[0].disp.pos;
    } else {
        bgw_ptr->old_pos_x = bgw_ptr->wxy[0].disp.pos;
    }

    if (bg_w.chase_flag & 0xF) {
        bgw_ptr->chase_xy[0].cal = bgw_ptr->speed_x * bg_w.bg2_sp_x2;
        bgw_ptr->chase_xy[0].disp.pos += bgw_ptr->pos_x_work;
    } else {
        bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal = bgw_ptr->speed_x * bg_w.bg2_sp_x2;
        bgw_ptr->xy[0].disp.pos += bgw_ptr->pos_x_work;
        bgw_ptr->wxy[0].disp.pos = bgw_ptr->xy[0].disp.pos;
    }

    if (!(bg_w.chase_flag & 0xF)) {
        bgw_ptr->chase_xy[0].disp.pos = bgw_ptr->wxy[0].disp.pos;
    }
}

void bg_y_move_check() {
    if (bg_w.chase_flag & 0xF0) {
        bgw_ptr->chase_xy[1].cal = bgw_ptr->speed_y * bg_w.bg2_sp_y;

        if (bgw_ptr->y_limit2 < bgw_ptr->chase_xy[1].disp.pos) {
            bgw_ptr->chase_xy[1].disp.pos = bgw_ptr->y_limit2;
            bgw_ptr->chase_xy[1].disp.low = 0;
        }

        bgw_ptr->chase_xy[1].disp.pos += bgw_ptr->pos_y_work;

        return;
    }

    bgw_ptr->xy[1].cal = bgw_ptr->speed_y * bg_w.bg2_sp_y;

    if (bgw_ptr->y_limit2 < bgw_ptr->xy[1].disp.pos) {
        bgw_ptr->xy[1].disp.pos = bgw_ptr->y_limit2;
        bgw_ptr->xy[1].disp.low = 0;
    }

    bgw_ptr->xy[1].disp.pos += bgw_ptr->pos_y_work;
    bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal;
}

void zoom_ud_check() {
#if defined(TARGET_PS2)
    void Frame_Up(u32 x, u32 y, u16 add);
    void Frame_Down(u32 x, u32 y, u16 add);
#endif

    s16 work;
    s16 work2;
    s16 pos_w;

    if (bg_app) {
        return;
    }

    if (Bonus_Game_Flag) {
        return;
    }

    if (bg_app_stop && bg_w.bg_f_x == 64) {
        return;
    }

    work2 = zoom_request_flag & 0xFF;
    bg_w.frame_deff = 64 - zoom_request_level;
    work = (~(zoom_req_flag_old) & (zoom_request_flag) & 0xFF);

    if (work && !bg_w.frame_flag) {
        bg_w.frame_flag = 1;
        bg_w.old_frame_flag = 1;
        bg_w.center_y = 224 - scr_req_y;

        if (bg_w.center_y < 0) {
            bg_w.center_y = 0;
        }

        if (scr_req_x < bg_w.bgw[1].l_limit2) {
            if (bg_w.bgw[1].zuubun != 0) {
                bg_w.center_x = scr_req_x + 512;
                pos_w = bg_w.bgw[1].wxy[0].disp.pos + 512;
            } else {
                bg_w.center_x = scr_req_x;
                pos_w = bg_w.bgw[1].wxy[0].disp.pos;
            }

            pos_w -= bg_w.pos_offset;
            bg_w.center_x -= pos_w;
        } else if (bg_w.bgw[1].r_limit2 < scr_req_x) {
            if (bg_w.bgw[1].zuubun != 0) {
                bg_w.center_x = scr_req_x + 512;
                pos_w = bg_w.bgw[1].wxy[0].disp.pos + 512;
            } else {
                bg_w.center_x = scr_req_x;
                pos_w = bg_w.bgw[1].wxy[0].disp.pos;
            }

            pos_w -= bg_w.pos_offset;
            bg_w.center_x -= pos_w;
        } else {
            bg_w.center_x = 192;
            bg_w.center_y = 224;
        }
    }

    if (work2) {
        if (bg_w.bg_f_x > bg_w.frame_deff) {
            Frame_Up((u16)bg_w.center_x, (u16)bg_w.center_y, 1);
            bg_w.bg_f_x--;
            bg_w.bg_f_y--;
            return;
        }

        if (bg_w.bg_f_x < bg_w.frame_deff) {
            Frame_Down((u16)bg_w.center_x, (u16)bg_w.center_y, 1);
            bg_w.bg_f_x++;
            bg_w.bg_f_y++;

            if (bg_w.bg_f_x == 64) {
                bg_w.frame_flag = 0;
                Zoomf_Init();
            }
        }
    } else if (bg_w.bg_f_x < 64) {
        Frame_Down((u16)bg_w.center_x, (u16)bg_w.center_y, 1);
        bg_w.bg_f_x++;
        bg_w.bg_f_y++;

        if (bg_w.bg_f_x == 64) {
            bg_w.frame_flag = 0;
            Zoomf_Init();
        }
    }
}

void suzi_offset_set(WORK_Other* ewk) {
    if (ewk->wu.sync_suzi == 1) {
        suzi_offset_set_sub(ewk);
    }
}

static s32 suzi_offset_set_sub(WORK_Other* ewk) {
    s16 work, work2;

    work = ewk->wu.xyz[1].disp.pos & 0x300;
    work = 0x300 - work;

    work2 = ewk->wu.xyz[1].disp.pos & 0xFF;
    work2 = 0x100 - work2;

    work += work2;

    return 0;
}

void suzi_sync_pos_set(WORK_Other* ewk) {
    s16 work;

    if (ewk->wu.sync_suzi) {
        if (ewk->wu.sync_suzi == 2) {
            suzi_offset_set_sub(ewk);
        }

        work = *ewk->wu.suzi_offset;
        work -= 0x200;
    } else {
        work = 0;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos - work & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
}

void Bg_Family_Set() {
#if defined(TARGET_PS2)
    void Scrn_Move_Set(s32 bgnm, s32 x, s32 y);
    void Family_Set_W(s32 fmnm, s32 x, s32 y);
#endif

    s8 i;
    s16 x;
    s16 y;

    for (i = 0; i < bg_w.scno; i++) {
        x = bg_w.bgw[i].position_x;
        y = bg_w.bgw[i].position_y;
        Scrn_Move_Set(i, x, y);
        x = -x & 0xFFFF;
        y = (768 - (y & 0xFFFF)) & 0xFFFF;
        Family_Set_W(i + 1, x, y);
    }
}

void Bg_Family_Set_appoint(s32 num_of_bg) {
#if defined(TARGET_PS2)
    void Scrn_Move_Set(s32 bgnm, s32 x, s32 y);
    void Family_Set_W(s32 fmnm, s32 x, s32 y);
#endif

    s16 x = bg_w.bgw[num_of_bg].position_x;
    s16 y = bg_w.bgw[num_of_bg].position_y;

    Scrn_Move_Set(num_of_bg, x, y);
    x = -x & 0xFFFF;
    y = (768 - (y & 0xFFFF)) & 0xFFFF;
    Family_Set_W(num_of_bg + 1, x, y);
}

void Bg_Family_Set_2() {
#if defined(TARGET_PS2)
    void Scrn_Move_Set(s32 bgnm, s32 x, s32 y);
    void Family_Set_W(s32 fmnm, s32 x, s32 y);
#endif

    s8 i;
    s16 x;
    s16 y;

    for (i = 0; i < bg_w.scno; i++) {
        x = bg_w.bgw[i].position_x;
        y = bg_w.bgw[i].position_y;
        y += 8;
        Scrn_Move_Set(i, x, y);
        x = -x & 0xFFFF;
        y = (768 - (y & 0xFFFF)) & 0xFFFF;
        Family_Set_W(i + 1, x, y);
    }
}

void Bg_Family_Set_2_appoint(s32 num_of_bg) {
#if defined(TARGET_PS2)
    void Scrn_Move_Set(s32 bgnm, s32 x, s32 y);
    void Family_Set_W(s32 fmnm, s32 x, s32 y);
#endif

    s16 x;
    s16 y;

    x = bg_w.bgw[num_of_bg].position_x;
    y = bg_w.bgw[num_of_bg].position_y;
    y += 8;
    Scrn_Move_Set(num_of_bg, x, y);
    x = -x & 0xFFFF;
    y = (768 - (y & 0xFFFF)) & 0xFFFF;
    Family_Set_W(num_of_bg + 1, x, y);
}

void ake_Family_Set2() {
#if defined(TARGET_PS2)
    void Scrn_Move_Set(s32 bgnm, s32 x, s32 y);
    void Family_Set_W(s32 fmnm, s32 x, s32 y);
#endif

    s16 x = bg_w.bgw[3].position_x;
    s16 y = bg_w.bgw[3].position_y;

    Scrn_Move_Set(3, x, y);
    x = 512 - bg_w.pos_offset;
    y = 0;
    x = -x & 0xFFFF;
    y = (768 - (y & 0xFFFF)) & 0xFFFF;
    Family_Set_W(4, x, y);
}

void bg_pos_hosei_sub2(s16 bg_no) {
    u16 pos;
    s16 pos2;

    pos2 = bg_w.bgw[bg_no].wxy[0].disp.pos;
    pos = pos2 & 0xFFFF;

    pos -= bg_w.pos_offset;
    pos2 -= bg_w.pos_offset;
    pos += quake_x_tbl[bg_w.quake_x_index];
    pos2 += quake_x_tbl[bg_w.quake_x_index];

    bg_w.bgw[bg_no].position_x = pos & 0xFFFF;
    bg_w.bgw[bg_no].abs_x = pos2;

    pos2 = bg_w.bgw[bg_no].xy[1].disp.pos;
    pos = pos2 & 0xFFFF;

    pos += quake_y_tbl[bg_w.quake_y_index];
    pos2 += quake_y_tbl[bg_w.quake_y_index];

    bg_w.bgw[bg_no].position_y = pos & 0xFFFF;
    bg_w.bgw[bg_no].abs_y = pos2;
}

void bg_pos_hosei_sub3(s16 bg_no) {
    u16 pos;
    s16 pos2;

    pos2 = bg_w.bgw[bg_no].wxy[0].disp.pos;
    pos = pos2 & 0xFFFF;

    pos -= bg_w.pos_offset;
    pos2 -= bg_w.pos_offset;

    bg_w.bgw[bg_no].position_x = pos & 0xFFFF;
    bg_w.bgw[bg_no].abs_x = pos2;

    pos2 = bg_w.bgw[bg_no].xy[1].disp.pos;
    pos = pos2 & 0xFFFF;

    bg_w.bgw[bg_no].position_y = pos & 0xFFFF;
    bg_w.bgw[bg_no].abs_y = pos2;
}

void bg_pos_hosei2() {
    s16 bg_no = 0;
    u16 pos;
    u16 pos2;

    while (bg_no < bg_w.scno) {
        if ((bg_w.chase_flag & 0xF) != 0) {
            pos2 = bg_w.bgw[bg_no].chase_xy[0].disp.pos;
        } else {
            pos2 = bg_w.bgw[bg_no].wxy[0].disp.pos;
        }

        pos = pos2 & 0xFFFF;
        pos -= bg_w.pos_offset;
        pos += quake_x_tbl[bg_w.quake_x_index];

        bg_w.bgw[bg_no].position_x = pos & 0xFFFF;

        pos2 -= bg_w.pos_offset;
        pos2 += quake_x_tbl[bg_w.quake_x_index];

        bg_w.bgw[bg_no].abs_x = pos2;

        if ((bg_w.chase_flag & 0xF0) != 0) {
            pos2 = bg_w.bgw[bg_no].chase_xy[1].disp.pos;
        } else {
            pos2 = bg_w.bgw[bg_no].xy[1].disp.pos;
        }

        pos = pos2 & 0xFFFF;
        pos += quake_y_tbl[bg_w.quake_y_index];
        pos2 += quake_y_tbl[bg_w.quake_y_index];

        bg_w.bgw[bg_no].position_y = pos & 0xFFFF;
        bg_w.bgw[bg_no].abs_y = pos2;

        bg_no++;
    }
}

s16 get_center_position() {
    if (Bonus_Game_Flag == 0x15) {
        return 0x200;
    }

    return bg_w.bgw[1].wxy[0].disp.pos;
}

s16 get_height_position() {
    return bg_w.bgw[1].xy[1].disp.pos;
}

void bg_work_clear() {
    s16 i;

    bg_w.bg_routine = 0;
    bg_w.bg_r_1 = 0;
    bg_w.bg_r_2 = 0;
    bg_w.compel_flag = 0;
    win_sp_flag = 0;
    bg_stop = 0;
    akebono_flag = 0;
    aku_flag = 0;
    sa_pa_flag = 0;
    bg_app = 0;
    bg_app_stop = 0;

    for (i = 0; i < 7; i++) {
        bg_w.bgw[i].r_no_0 = 0;
        bg_w.bgw[i].r_no_1 = 0;
        bg_w.bgw[i].r_no_2 = 0;
    }
}

void compel_bg_init_position() {
    s16 i;

    bg_w.compel_flag = 0;
    Zoomf_Init();
    bg_w.bg_f_x = 0x40;
    bg_w.bg_f_y = 0x40;
    bg_w.scr_stop = 0;
    bg_w.frame_flag = 0;
    bg_w.bg2_sp_x2 = bg_w.bg2_sp_x = 0;

    for (i = 0; i < 7; i++) {
        bg_w.bgw[(i)].xy[0].disp.pos = bg_w.bgw[(i)].wxy[0].disp.pos = bg_w.bgw[(i)].pos_x_work;
        bg_w.bgw[(i)].xy[1].disp.pos = bg_w.bgw[(i)].wxy[1].disp.pos = bg_w.bgw[(i)].pos_y_work;
        bg_w.bgw[(i)].xy[0].disp.low = bg_w.bgw[(i)].wxy[0].disp.low = 0;
        bg_w.bgw[(i)].xy[0].disp.low = bg_w.bgw[(i)].wxy[0].disp.low = 0;
        bg_w.bgw[(i)].old_pos_x = bg_w.bgw[(i)].wxy[0].disp.pos;
    }
}

void bg_base_move_common() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

void bg_move_common() {
    bg_x_move_check();
    bg_y_move_check();
}

void bg_initialize() {
    const s16* ptr;
    u8 i;

    Bg_Off_R(7);
    Family_Init();
    Scrn_Pos_Init();
    Zoomf_Init();
    bg_w.bg_opaque = stage_opaque[bg_w.stage];
    Screen_Switch = 0;
    Screen_Switch_Buffer = 0;
    bg_disp_off = 0;
    bg_w.bg_index = bg_index_tbl[bg_w.stage][bg_w.area];
    bg_w.scno = use_scr[bg_w.bg_index];
    bg_w.scrno = use_real_scr[bg_w.bg_index];
    y_sitei_flag = 0;
    y_sitei_pos = 0;

    if (G_No[0] != 2 || G_No[1] != 2 || G_No[2] != 2) {
        Bg_Texture_Load_EX();
    }

    Bg_Kakikae_Set();
    bg_w.pos_offset = 0xC0;

    for (i = 0; i < 7; i++) {
        bg_w.bgw[i].pos_x_work = bg_w.bgw[i].pos_y_work = 0;
        bg_w.bgw[i].zuubun = 0;
        bg_w.bgw[i].xy[0].cal = 0;
        bg_w.bgw[i].xy[1].cal = 0;
        bg_w.bgw[i].wxy[0].cal = 0;
        bg_w.bgw[i].wxy[1].cal = 0;
        bg_w.bgw[i].hos_xy[0].cal = 0;
        bg_w.bgw[i].hos_xy[1].cal = 0;
        bg_w.bgw[i].speed_x = 0;
        bg_w.bgw[i].speed_y = 0;
        bg_w.bgw[i].rewrite_flag = 0;
        bg_w.bgw[i].fam_no = i;
        bg_w.bgw[i].r_no_1 = bg_w.bgw[i].r_no_2 = 0;
        bg_w.bgw[i].speed_x = 0;
    }

    bg_w.scr_stop = 0;
    bg_w.frame_flag = 0;
    bg_w.bg_f_x = 0x40;
    bg_w.bg_f_y = 0x40;
    bg_w.bg2_sp_x2 = bg_w.bg2_sp_x = 0;
    bg_w.max_x = 8;
    bg_w.old_chase_flag = bg_w.chase_flag = 0;
    bg_w.quake_x_index = 0;
    bg_w.quake_y_index = 0;
    bg_w.frame_deff = 0x40;

    for (i = 0; i < bg_w.scno; i++) {
        bg_w.bgw[i].speed_x = *msp[bg_w.bg_index][i];
        bg_w.bgw[i].speed_y = msp[bg_w.bg_index][i][1];
        bg_w.bgw[i].rewrite_flag = 0;
        bg_w.bgw[i].xy[1].disp.pos = bg_w.bgw[i].wxy[1].disp.pos = bg_w.bgw[i].pos_y_work = 0;
        ptr = limit_tbl3[bg_w.bg_index][i];
        bg_w.bgw[i].l_limit2 = *ptr++;
        bg_w.bgw[i].r_limit2 = *ptr++;
        bg_w.bgw[i].y_limit = *ptr++;
        bg_w.bgw[i].y_limit2 = *ptr++;
        bg_w.bgw[i].frame_deff = 0;
        bg_w.bgw[i].max_x_limit = bg_w.bgw[i].speed_x * bg_w.max_x;
    }

    if (bg_w.stage != 4) {
        base_y_pos = 0x28;
    } else {
        base_y_pos = 0x30;
    }

    if (bg_w.stage > 19) {
        bg_pos_hosei_sub3(2);
        Bg_Family_Set_appoint(2);
    }

    bg_pos_hosei2();
    Bg_Family_Set();
}

void akebono_initialize() {
    bg_w.bgw[3].xy[0].cal = bg_w.bgw[3].wxy[0].cal = 0x100000;
    bg_w.bgw[3].xy[1].cal = bg_w.bgw[3].wxy[1].cal = 0;
    bg_w.bgw[3].position_x = 0xC0 - bg_w.pos_offset;
    bg_w.bgw[3].position_y = 0;
    Bg_Family_Set_appoint(3);
    bg_w.bgw[3].r_no_1 = bg_w.bgw[3].r_no_2 = 0;
    bg_w.bgw[3].fam_no = 3;
    Bg_Off_R(8);
}

void bg_etc_write(s16 type) {
#if defined(TARGET_PS2)
    void Bg_Texture_Load2(u32 type);
#endif

    u8 i;

    Family_Init();
    Scrn_Pos_Init();
    Zoomf_Init();
    bg_w.bg_opaque = 224;
    Screen_Switch = 0;
    Screen_Switch_Buffer = 0;
    bg_disp_off = 0;
    bg_w.scno = bg_w.scrno = use_scr2[type];
    bg_w.pos_offset = 192;

    Bg_Texture_Load2((u8)type);

    for (i = 0; i < 7; i++) {
        bg_w.bgw[i].pos_x_work = 0;
        bg_w.bgw[i].pos_y_work = 0;
        bg_w.bgw[i].hos_xy[1].cal = bg_w.bgw[i].hos_xy[0].cal = bg_w.bgw[i].wxy[1].cal = bg_w.bgw[i].wxy[0].cal =
            bg_w.bgw[i].xy[1].cal = bg_w.bgw[i].xy[0].cal = bg_w.bgw[i].zuubun = 0;
        bg_w.bgw[i].rewrite_flag = 0;
        bg_w.bgw[i].fam_no = i;
        bg_w.bgw[i].speed_y = bg_w.bgw[i].speed_x = 0;
        bg_w.bgw[i].r_no_1 = bg_w.bgw[i].r_no_2 = 0;
    }

    bg_w.scr_stop = 0;
    bg_w.frame_flag = 0;
    bg_w.old_chase_flag = bg_w.chase_flag = 0;
    bg_w.bg_f_x = 64;
    bg_w.bg_f_y = 64;
    bg_w.bg2_sp_x2 = bg_w.bg2_sp_x = 0;
    bg_w.max_x = 8;
    bg_w.quake_x_index = 0;
    bg_w.quake_y_index = 0;

    for (i = 0; i < bg_w.scno; i++) {
        bg_w.bgw[i].hos_xy[0].cal = bg_w.bgw[i].wxy[0].cal = bg_w.bgw[i].xy[0].cal = bg_pos_tbl2[type][i][0];
        bg_w.bgw[i].hos_xy[1].cal = bg_w.bgw[i].wxy[1].cal = bg_w.bgw[i].xy[1].cal = bg_pos_tbl2[type][i][1];
        bg_w.bgw[i].pos_y_work = bg_w.bgw[i].xy[1].disp.pos;
        bg_w.bgw[i].old_pos_x = bg_w.bgw[i].pos_x_work = bg_w.bgw[i].xy[0].disp.pos;
        bg_w.bgw[i].speed_x = msp2[type][i][0];
        bg_w.bgw[i].speed_y = msp2[type][i][1];
        bg_w.bgw[i].rewrite_flag = 0;
        bg_w.bgw[i].zuubun = 0;
        bg_w.bgw[i].frame_deff = 64;
        bg_w.bgw[i].max_x_limit = bg_w.bgw[i].speed_x * bg_w.max_x;
    }

    base_y_pos = 40;
}

s32 Ck_Range_Out_S(WORK_Other* ewk, s16 BG_No, s16 R) {
    s16 x;

    x = ewk->wu.xyz[0].disp.pos - bg_w.bgw[BG_No].wxy[0].disp.pos;

    if (x < 0) {
        x = -x;
    }

    if (x - R > 192) {
        return 1;
    }

    return 0;
}
