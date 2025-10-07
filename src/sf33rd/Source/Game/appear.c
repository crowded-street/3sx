#include "sf33rd/Source/Game/appear.h"
#include "common.h"
#include "sf33rd/Source/Game/CALDIR.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFF09.h"
#include "sf33rd/Source/Game/EFF15.h"
#include "sf33rd/Source/Game/EFF46.h"
#include "sf33rd/Source/Game/EFF86.h"
#include "sf33rd/Source/Game/EFFC5.h"
#include "sf33rd/Source/Game/EFFC8.h"
#include "sf33rd/Source/Game/EFFM1.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/SE.h"
#include "sf33rd/Source/Game/Sound3rd.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/app_data.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/eff97.h"
#include "sf33rd/Source/Game/effM0.h"
#include "sf33rd/Source/Game/effM5.h"
#include "sf33rd/Source/Game/effM7.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/workuser.h"

s8 Appear_car_stop[] = { 0, 0 };
s8 Appear_hv[] = { 0, 0 };
s8 Appear_free[] = { 0, 0 };
s8 Appear_flag[] = { 0, 0 };
s16 app_counter[] = { 0, 0 };
s16 appear_work[] = { 0, 0 };
s16 Appear_end;

void appear_work_clear() {
    Appear_end = 0;
    Appear_flag[0] = 0;
    Appear_flag[1] = 0;
    Appear_free[0] = 0;
    Appear_free[1] = 0;
    bg_stop = 0;
    bg_app_stop = 0;
}

s32 home_visitor_check(PLW* wk) {
    s8 hv_type;
    s16 pl_num;

    hv_type = 0;

    if (wk->wu.id) {
        pl_num = plw[0].player_number;
    } else {
        pl_num = plw[1].player_number;
    }

    if (Play_Type) {
        if (Champion == wk->wu.id && wk->player_number == pl_num && pl_num != 8) {
            hv_type = 1;
        }
    } else if (wk->wu.operator && wk->player_number == pl_num && pl_num != 8) {
        hv_type = 1;
    }

    return hv_type;
}

void appear_data_set(PLW* wk, APPEAR_DATA* dtbl) {
    if (wk->wu.id) {
        wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work - dtbl->hx;
        wk->wu.xyz[1].disp.pos = dtbl->hy;
        wk->wu.rl_flag = (s8)((dtbl->rl + 1) & 1);
        wk->wu.routine_no[4] = dtbl->rno;
        Appear_flag[0] = dtbl->ixod;
        wk->wu.char_index = dtbl->char_index;

    } else {
        wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + dtbl->hx;
        wk->wu.xyz[1].disp.pos = dtbl->hy;
        wk->wu.rl_flag = dtbl->rl;
        wk->wu.routine_no[4] = dtbl->rno;
        Appear_flag[1] = dtbl->ixod;
        wk->wu.char_index = dtbl->char_index;
    }
}

void appear_data_init_set(PLW* wk) {
    APPEAR_DATA* dtbl;
    s8 ap_work;
    s16 id_work;

    Appear_hv[wk->wu.id] = home_visitor_check(wk);

    id_work = wk->wu.id ^ 1;

    if (bg_w.area) {
        ap_work = 0;
    } else if (Appear_hv[wk->wu.id]) {
        ap_work = app_type_tbl2[wk->player_number][plw[id_work].player_number][bg_w.stage];
    } else {
        ap_work = app_type_tbl[wk->player_number][plw[id_work].player_number][bg_w.stage];
    }

    dtbl = (APPEAR_DATA*)&appear_data[ap_work];

    appear_data_set(wk, dtbl);
}

void appear_player(PLW* wk) {
    void (*appear_jmp_tbl[42])(
        PLW* wk) = { Appear_00000, Appear_01000, Appear_01000, Appear_03000, Appear_04000, Appear_05000, Appear_06000,
                     Appear_07000, Appear_08000, Appear_09000, Appear_10000, Appear_11000, Appear_12000, Appear_13000,
                     Appear_14000, Appear_15000, Appear_16000, Appear_17000, Appear_18000, Appear_19000, Appear_20000,
                     Appear_21000, Appear_22000, Appear_23000, Appear_24000, Appear_25000, Appear_26000, Appear_06000,
                     Appear_28000, Appear_29000, Appear_30000, Appear_31000, Appear_32000, Appear_33000, Appear_34000,
                     Appear_01000, Appear_36000, Appear_37000, Appear_38000, Appear_39000, Appear_06000, Appear_41000 };
    appear_jmp_tbl[(short)wk->wu.routine_no[4]](wk);
}

void Appear_00000(PLW* wk) {
    Appear_end++;
    wk->wu.routine_no[2] = 1;
    wk->wu.routine_no[3] = 0;
}

void Appear_01000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    s16 work;
    s16 id_w;

    id_w = wk->wu.id ^ 1;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        bg_app_stop = 1;
        work = random_16();

        switch (wk->wu.routine_no[4]) {
        case 1:
            work &= 3;
            set_char_move_init(&wk->wu, 9, work);
            break;

        case 2:
            work &= 3;
            set_char_move_init(&wk->wu, 9, work + 4);
            break;

        case 35:
            work &= 7;
            set_char_move_init(&wk->wu, 9, work);
            break;
        }
        break;

    case 1:
        if (Appear_flag[wk->wu.id]) {
            wk->wu.routine_no[3]++;
            char_move(&wk->wu);
            return;
        }
        wk->wu.routine_no[3] = 3;
        set_char_move_init(&wk->wu, 9, wk->wu.char_index + 8);
        break;

    case 2:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 9 && Appear_flag[wk->wu.id] == 0) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, 9, wk->wu.char_index + 8);
        }
        break;

    case 3:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }
        break;
    }
}

void Appear_03000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, wk->wu.char_index);
        bg_app_stop = 1;
        return;

    case 1:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }
    }
}

void Appear_04000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        bg_app_stop = 1;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0x10);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 9) {
            wk->wu.routine_no[3]++;
            app_counter[wk->wu.id] = 0x1C;

            if (wk->wu.id) {
                cal_all_speed_data(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work + 0x58, 0, 0, 1);
                return;
            }

            cal_all_speed_data(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work - 0x58, 0, 0, 1);
            return;
        }
        break;

    case 2:
        char_move(&wk->wu);
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] <= 0) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, 9, 0x11);

            if (wk->wu.id) {
                wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 0x58;
            } else {
                wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work - 0x58;
            }

            wk->wu.xyz[0].disp.low = 0;
            wk->wu.xyz[1].cal = 0;
            return;
        }

        add_x_sub((WORK_Other*)wk);
        add_y_sub((WORK_Other*)wk);
        break;

    case 3:
        char_move(&wk->wu);

        if ((wk->wu.cg_type) == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }

        break;
    }
}

void Appear_05000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0x17);
        bg_app_stop = 1;
        appear_work[wk->wu.id] = 0x1C;
        break;

    case 1:
        appear_work[wk->wu.id]--;

        if (appear_work[wk->wu.id] < 0) {
            wk->wu.routine_no[3]++;
            appear_work[wk->wu.id] = 0x1B;

            if (wk->wu.id) {
                cal_all_speed_data(&wk->wu, appear_work[wk->wu.id], bg_w.bgw[1].pos_x_work + 0x60, 0, 2, 0);
                return;
            }

            cal_all_speed_data(&wk->wu, appear_work[wk->wu.id], bg_w.bgw[1].pos_x_work - 0x60, 0, 2, 0);
        }
        break;

    case 2:
        char_move(&wk->wu);
        appear_work[wk->wu.id]--;

        if (appear_work[wk->wu.id] <= 0) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 0x17, 9, 0);

            if (wk->wu.id) {
                wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 0x58;
                return;
            }
            wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work - 0x58;
            return;
        }

        add_x_sub((WORK_Other*)&wk->wu);
        break;

    case 3:
        char_move(&wk->wu);

        if ((wk->wu.cg_type) == 9) {
            wk->wu.routine_no[3]++;
            wk->wu.rl_flag ^= 1;
            return;
        }

        break;

    case 4:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }

        break;
    }
}

void Appear_06000(PLW* wk) {
    s16 work;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        switch (wk->wu.routine_no[4]) {
        case 6:
            effect_C5_init(wk, 0);
            break;
        case 27:
            effect_C5_init(wk, 1);
            break;
        case 40:
            effect_M5_init(wk);
            break;
        }
        wk->wu.disp_flag = 0;
        set_char_move_init(&wk->wu, 9, 0x13);
        bg_app_stop = 1;
        break;

    case 1:
        if (demo_car_flag[wk->wu.id]) {
            wk->wu.routine_no[3]++;
            wk->wu.disp_flag = 1;
            wk->wu.my_mr_flag = 0;
            set_char_move_init(&wk->wu, 9, 0x13);
            wk->wu.position_z = wk->wu.next_z = 0x6E;
            wk->wu.mvxy.d[0].sp = 0;
            wk->wu.mvxy.d[1].sp = -0x8000;

            if (wk->wu.routine_no[4] == 0x1B) {
                appear_work[wk->wu.id] = 0x34;
            } else {
                appear_work[wk->wu.id] = 0x2A;
            }

            work = 88;
            if (wk->wu.id) {
                cal_initial_speed(&wk->wu, appear_work[wk->wu.id], bg_w.bgw[1].pos_x_work + work, 0);
                break;
            }
            cal_initial_speed(&wk->wu, appear_work[wk->wu.id], bg_w.bgw[1].pos_x_work - work, 0);
        }
        break;

    case 2:
        char_move(&wk->wu);
        appear_work[wk->wu.id]--;

        if (*&appear_work[wk->wu.id] <= 0) {
            wk->wu.routine_no[3]++;
            wk->wu.xyz[1].cal = 0;
            set_char_move_init(&wk->wu, 9, 0x10);
            Appear_end++;
        } else {
            add_x_sub((WORK_Other*)&wk->wu);
            add_y_sub((WORK_Other*)&wk->wu);
        }
        break;

    case 3:
        char_move(&wk->wu);
        if (wk->wu.cg_type) {
            wk->wu.routine_no[3]++;
            if (wk->wu.routine_no[4] == 0x1B) {
                wk->wu.rl_flag ^= 1;
                set_char_move_init(&wk->wu, 0, 1);
            } else {
                wk->wu.routine_no[2] = 1;
                wk->wu.routine_no[3] = 0;
            }
        }
        break;

    case 4:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
        }
        break;
    }

    wk->wu.position_x = wk->wu.xyz[0].disp.pos;
    wk->wu.position_y = wk->wu.xyz[1].disp.pos;
}

const APPEAR_DATA appear_data[] = {
    { -88, 0, -88, 0, 1, 0, 0 },    { -88, 0, -88, 0, 1, 1, 0 },     { -88, 0, -88, 0, 1, 2, 0 },
    { -124, 0, -124, 0, 1, 3, 12 }, { -256, 144, -256, 0, 1, 4, 0 }, { 288, 0, 288, 1, 0, 5, 0 },
    { -248, 0, -248, 0, 1, 6, 0 },  { -88, 0, -88, 0, 1, 7, 0 },     { -62, 0, -62, 0, 1, 8, 0 },
    { -88, 200, -88, 0, 1, 9, 16 }, { -216, 0, -224, 0, 1, 10, 16 }, { -102, 0, -102, 0, 1, 1, 0 },
    { -88, 0, -88, 0, 1, 11, 0 },   { -17, 0, -17, 0, 1, 1, 0 },     { -88, 384, -88, 0, 1, 13, 0 },
    { -88, 0, -88, 0, 1, 14, 0 },   { -128, 0, -128, 0, 1, 15, 0 },  { -272, 0, -304, 0, 1, 12, 0 },
    { -88, 0, -88, 0, 1, 16, 0 },   { -88, 0, -88, 0, 1, 17, 0 },    { -88, 0, -88, 0, 1, 21, 0 },
    { -48, 0, -48, 0, 1, 18, 0 },   { -120, 0, -120, 0, 1, 1, 0 },   { -88, 0, -88, 0, 1, 3, 23 },
    { 256, 0, 256, 0, 1, 20, 0 },   { -88, 0, -88, 0, 1, 22, 0 },    { -88, 0, -88, 0, 1, 23, 0 },
    { -96, 0, -96, 0, 1, 24, 0 },   { -96, 0, -96, 0, 1, 25, 0 },    { 168, 80, 168, 0, 0, 26, 0 },
    { 248, 0, 248, 0, 0, 27, 0 },   { -112, 0, -112, 0, 1, 28, 0 },  { -111, 0, -111, 0, 1, 1, 0 },
    { -48, 0, -48, 0, 1, 3, 18 },   { -96, 0, -96, 0, 1, 3, 23 },    { -53, 0, -53, 0, 1, 3, 21 },
    { -53, 0, -53, 0, 1, 3, 22 },   { -72, 0, -72, 0, 1, 38, 20 },   { -84, 0, -84, 0, 1, 3, 22 },
    { -84, 0, -84, 0, 1, 3, 22 },   { -256, 0, -256, 0, 1, 29, 0 },  { -88, 0, -88, 0, 1, 30, 0 },
    { -64, 0, -64, 0, 1, 31, 0 },   { -258, 0, -258, 0, 1, 32, 0 },  { -256, 0, -256, 0, 1, 33, 0 },
    { -88, 0, -88, 0, 1, 34, 0 },   { -88, 0, -88, 0, 1, 35, 0 },    { -88, 0, -88, 0, 1, 3, 16 },
    { -24, 0, -24, 0, 1, 36, 0 },   { -24, 0, -24, 0, 1, 37, 0 },    { -448, 0, -448, 0, 1, 39, 0 },
    { -72, 0, -72, 0, 1, 3, 17 },   { -248, 0, -248, 0, 1, 40, 0 },  { -88, 0, -88, 0, 1, 41, 0 },
    { -88, 0, -88, 0, 1, 3, 17 },
};

void Appear_07000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.disp_flag = 1;
        bg_app_stop = 1;

        if (plw[wk->wu.id ^ 1].player_number == 12 && bg_w.stage == 12 && bg_w.area == 0) {
            wk->wu.routine_no[4] = 1;
            set_char_move_init(&wk->wu, 9, 17);
            wk->wu.routine_no[3] = 3;
        } else {
            set_char_move_init(&wk->wu, 9, 8);
            effect_C8_init(wk);

            if (Appear_flag[wk->wu.id]) {
                wk->wu.routine_no[3]++;
            } else {
                wk->wu.routine_no[3] = 2;
            }
        }

        break;

    case 1:
        if (Appear_flag[wk->wu.id] != 0) {
            break;
        }
        wk->wu.routine_no[3]++;
        /* fallthrough */

    case 2:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 9) {
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
        }
        break;

    case 3:
    case 4:
        switch (wk->wu.cg_type) {
        case 1:
            wk->wu.mvxy.a[1].sp = 0x30000;
            wk->wu.mvxy.d[1].sp = 0xffffa000;
            wk->wu.cg_type = 0;
            char_move_z(&wk->wu);
            break;

        case 2:
        case 3:
            char_move(&wk->wu);
            add_y_sub((WORK_Other*)wk);
            if (wk->wu.xyz[1].disp.pos < 0) {
                wk->wu.xyz[1].disp.pos = wk->wu.position_y = 0;
                wk->wu.xyz[1].disp.low = 0;
                char_move_z(&wk->wu);
            }
            break;

        case 4:
            wk->wu.cg_type = 0;
            char_move_z(&wk->wu);
            wk->wu.routine_no[3]++;
            break;

        default:
            char_move(&wk->wu);
            break;
        }

        break;

    case 5:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }

        break;
    }

    wk->wu.position_x = wk->wu.xyz[0].disp.pos;
    wk->wu.position_y = wk->wu.xyz[1].disp.pos;
}

void Appear_08000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;

        if (sean_appear_check(wk, wk->wu.id)) {
            set_char_move_init(&wk->wu, 9, 0x11);
            Appear_free[wk->wu.id] = 0;
            bg_app_stop = 1;
            break;
        }

        appear_data_set(wk, (APPEAR_DATA*)appear_data);
        wk->wu.routine_no[4] = 2;
        break;

    case 1:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }
    }
}

s32 sean_appear_check(PLW* wk, s16 id) {
    if (plw[id].player_number == 12 && bg_w.stage == 12) {
        return 1;
    }

    return 0;
}

void Appear_09000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0x10);
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[3]++;
            Appear_free[wk->wu.id] = 1;
            app_counter[wk->wu.id] = 0x20;
            return;
        }

        break;

    case 2:
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] < 0) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, 9, 0x11);
            wk->wu.mvxy.a[1].sp = -0xB0000;
            wk->wu.mvxy.d[1].sp = -0x6000;
            return;
        }

        break;

    case 3:
        char_move(&wk->wu);
        add_y_sub((WORK_Other*)&wk->wu);

        if (wk->wu.xyz[1].disp.pos <= 0) {
            wk->wu.routine_no[3]++;
            wk->wu.xyz[1].cal = 0;
            set_char_move_init(&wk->wu, 9, 0x12);
            Appear_end++;
            return;
        }

        break;

    case 4:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
        }

        break;
    }
}

void Appear_10000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0x13);
        bg_app_stop = 1;
        wk->wu.mvxy.d[0].sp = 0;
        if (wk->wu.id) {
            wk->wu.mvxy.a[0].sp = -0x18000;
            break;
        }
        wk->wu.mvxy.a[0].sp = 0x18000;
        break;

    case 1:
        char_move(&wk->wu);
        add_x_sub((WORK_Other*)&wk->wu);

        if (wk->wu.id) {
            if (!(wk->wu.xyz[0].disp.pos <= bg_w.bgw[1].pos_x_work + 0x58)) {
                return;
            }
        } else if (!(wk->wu.xyz[0].disp.pos >= bg_w.bgw[1].pos_x_work - 0x58)) {
            return;
        }
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 9, 0x14);
        Appear_end++;
        break;

    case 2:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
        }
        break;
    }
}

void Appear_11000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        bg_app_stop = 1;
        app_counter[wk->wu.id] = 0x50;
        set_char_move_init(&wk->wu, 0, 0);
        break;

    case 1:
        char_move(&wk->wu);
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] < 0) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 1;
            Appear_end++;

            if (Demo_Flag != 0) {
                SsRequestPan(0x2A9, 0x40, 0x40, 0, 2);
            }
        }
    }
}

void Appear_12000(PLW* wk) {
    s16 work;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        bg_app_stop = 1;
        set_char_move_init(&wk->wu, 9, 12);
        effect_46_init(&wk->wu, 0);
        work = 88;
        wk->wu.mvxy.d[0].sp = 0;
        wk->wu.mvxy.d[1].sp = -0x8000;
        app_counter[wk->wu.id] = 0x30;

        if (wk->wu.id) {
            cal_initial_speed(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work + work, 0);
            return;
        }
        cal_initial_speed(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work - work, 0);
        return;

    case 1:
        char_move(&wk->wu);
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] <= 0) {
            wk->wu.routine_no[3]++;
            wk->wu.xyz[1].cal = 0;
            set_char_move_init2(&wk->wu, 9, 12, 19, 0);
            Appear_end++;
            return;
        }

        add_x_sub((WORK_Other*)&wk->wu);
        add_y_sub((WORK_Other*)&wk->wu);
        return;

    case 2:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
        }
    }
}

void Appear_13000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        bg_app_stop = 1;
        set_char_move_init2(&wk->wu, 9, 0x3D, 4, 0);
        wk->wu.mvxy.a[1].sp = 0x78000;
        wk->wu.mvxy.d[1].sp = -0x3000;
        wk->wu.kage_flag = 0;
        break;

    case 1:
        // Do nothing

    case 2:
        char_move(&wk->wu);
        add_y_sub((WORK_Other*)&wk->wu);

        if (wk->wu.xyz[1].disp.pos < 0) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, 9, 0x3E);
            wk->wu.xyz[1].cal = 0;
            Appear_end++;
            return;
        }

        break;

    case 3:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
        }

        break;
    }
}

void Appear_14000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s16 koc, s32 index, s32 ip, s16 scf);
#endif

    s16 work;
    s16 id_w = wk->wu.id ^ 1;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] += 1;
        wk->wu.disp_flag = 1;
        wk->gill_ccch_go = 1;

        if (sean_appear_check(wk, id_w)) {
            set_char_move_init(&wk->wu, 9, 0x3C);
            return;
        }

        wk->wu.routine_no[2] = 1;
        wk->wu.routine_no[3] = 0;
        Appear_end++;
        break;

    case 1:
        switch (Appear_free[id_w]) {
        case 0:
            char_move(&wk->wu);
            break;

        case 1:
            work = wk->wu.cg_ix / wk->wu.cgd_type;
            set_char_move_init2(&wk->wu, 0, 0, work + 1, 0);
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 1;
            Appear_end += 1;
            break;

        case 2:
            char_move(&wk->wu);
            if (wk->wu.cg_type == 0xFF) {
                wk->wu.routine_no[2] = 1;
                wk->wu.routine_no[3] = 0;
                Appear_end += 1;
            }
        }
    }
}

void Appear_15000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        bg_app_stop = 1;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 8);
        effect_97_init(wk);
        break;

    case 1:
        char_move(&wk->wu);

        switch (wk->wu.cg_type) {
        case 0x2:
            wk->wu.cg_type = 0;
            Sound_SE(0x10A);
            break;

        case 0x3:
            wk->wu.cg_type = 0;
            Sound_SE(0x10B);
            break;

        case 0xFF:
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
            break;
        }
    }
}

void Appear_16000(PLW* wk) {
    s16 id_w;

    id_w = wk->wu.id ^ 1;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        bg_app_stop = 1;

        if (smoke_check[bg_w.bg_index]) {
            set_char_move_init(&wk->wu, 9, 0xE);
            return;
        }

        set_char_move_init(&wk->wu, 9, 0xC);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }
    }
}

s16 gill_appear_check() {
    s16 id_w;

    if (bg_w.stage && bg_w.area) {
        return 1;
    }

    id_w = -1;

    if (plw[0].player_number == 0) {
        id_w = 1;
    }

    if (plw[1].player_number == 0) {
        id_w = 0;
    }

    if (id_w < 0) {
        return 1;
    }

    if (Play_Type == 1) {
        return 1;
    }

    if (Introduce_Boss[Player_id][1] & 0x80) {
        return 1;
    }

    return 0;
}

void Appear_17000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;

        if (Gill_Appear_Flag) {
            appear_data_set(wk, (APPEAR_DATA*)appear_data);
            Appear_00000(wk);
            return;
        }

        set_char_move_init(&wk->wu, 9, 0);
        bg_app_stop = 1;
        gSeqStatus[0] = 0;
        SsRequest(0x3C);
        set_char_move_init(&wk->wu, 9, 8);
        break;

    case 1:
        if (!bg_app) {
            wk->wu.routine_no[3]++;
        }
        break;

    case 2:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 2) {
            wk->wu.routine_no[3]++;
            wk->gill_ccch_go = 1;
            return;
        }

        break;

    case 3:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 3) {
            wk->wu.cg_type = 0;
            SsBgmFadeOut(0xAA);
        }

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
            Standby_BGM(0x2E);
        }

        break;
    }
}

void Appear_18000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    s16 work;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;

        if (plw[0].player_number == 8 && plw[1].player_number == 8) {
            Appear_free[wk->wu.id] = 0;

            if (wk->wu.id) {
                wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work - 0x3B;
            } else {
                wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 0x3B;
            }

            set_char_move_init(&wk->wu, 9, 0x10);
            goto one;
        }

        Appear_free[wk->wu.id] = 1;
        work = random_16();
        work &= 3;
        set_char_move_init(&wk->wu, 9, work + 8);

        wk->wu.mvxy.a[0].sp = 0;
        wk->wu.mvxy.a[1].sp = 0x80000;

        appear_work[wk->wu.id] = 0x1F;

        if (wk->wu.id) {
            cal_delta_speed(&wk->wu, appear_work[wk->wu.id], (bg_w.bgw[1].pos_x_work + 0x58), 0, 0, 1);
            goto one;
        }
        cal_delta_speed(&wk->wu, appear_work[wk->wu.id], (bg_w.bgw[1].pos_x_work - 0x58), 0, 0, 1);

    one:
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 9) {
            if (!Appear_free[wk->wu.id]) {
                wk->wu.mvxy.a[0].sp = 0;
                wk->wu.mvxy.a[1].sp = 0x80000;

                appear_work[wk->wu.id] = 0x1F;

                if (wk->wu.id) {
                    cal_delta_speed(&wk->wu, appear_work[wk->wu.id], bg_w.bgw[1].pos_x_work + 0x58, 0, 0, 1);
                } else {
                    cal_delta_speed(&wk->wu, appear_work[wk->wu.id], bg_w.bgw[1].pos_x_work - 0x58, 0, 0, 1);
                }
            }

            wk->wu.cg_type = 0;
            wk->wu.routine_no[3]++;
        }
        break;

    case 2:
        char_move(&wk->wu);

        appear_work[wk->wu.id]--;

        if (appear_work[wk->wu.id] <= 0) {
            wk->wu.routine_no[3]++;
            wk->wu.xyz[1].cal = 0;
            return;
        }

        add_x_sub((WORK_Other*)&wk->wu);
        add_y_sub((WORK_Other*)&wk->wu);
        return;

    case 3:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }

        break;
    }
    return;
}

void Appear_19000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        bg_app_stop = 1;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0);
        wk->wu.rl_flag = 0;
        wk->wu.xyz[0].disp.pos = 0x275;
        wk->wu.xyz[1].disp.pos = 0x154;
        wk->wu.next_z = 0x56;
        wk->wu.my_mr_flag = 1;
        wk->wu.my_mr.size.x = 0x35;
        wk->wu.my_mr.size.y = 0x35;
        appear_work[wk->wu.id] = 0x82;
        break;

    case 1:
        appear_work[wk->wu.id]--;

        wk->wu.next_z = 0x56;

        if (appear_work[wk->wu.id] < 0) {
            wk->wu.routine_no[3]++;
            wk->wu.my_mr_flag = 0;
            wk->wu.my_mr.size.x = 0x3F;
            wk->wu.my_mr.size.y = 0x3F;

            set_char_move_init2(&wk->wu, 9, 0x3D, 4, 0);

            appear_work[wk->wu.id] = 0x20;

            if (wk->wu.id) {
                cal_all_speed_data(&wk->wu, appear_work[wk->wu.id], bg_w.bgw[1].pos_x_work + 0x58, 0, 1, 1);
            } else {
                cal_all_speed_data(&wk->wu, appear_work[wk->wu.id], bg_w.bgw[1].pos_x_work - 0x58, 0, 1, 1);
            }

            if (wk->wu.id == 0) {
                wk->wu.rl_flag = 1;
            }

            wk->wu.next_z = wk->wu.my_priority;

            effect_15_init(&wk->wu, 0);
            effect_15_init(&wk->wu, 1);
            effect_15_init(&wk->wu, 2);
        }
        break;

    case 2:
        appear_work[wk->wu.id]--;

        if (appear_work[wk->wu.id] <= 0) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, 9, 0x3E);
            wk->wu.xyz[1].cal = 0;

            if (wk->wu.id) {
                Appear_flag[0] = 0;
                return;
            }

            Appear_flag[1] = 0;
            return;
        }

        add_x_sub((WORK_Other*)&wk->wu);
        add_y_sub((WORK_Other*)&wk->wu);
        break;

    case 3:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }

        break;
    }
}

void Appear_20000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0x15);
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }
    }
}

void Appear_21000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif
    s16 work;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] += 1;
        wk->wu.disp_flag = 1;
        work = random_16();
        work &= 7;
        if (work == 6 || work == 7) {
            appear_data_set(wk, (APPEAR_DATA*)appear_data + 24);
            set_char_move_init(&wk->wu, 9, 0xE);
        } else {
            set_char_move_init(&wk->wu, 9, work + 8);
        }
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end += 1;
        }
    }
}

void Appear_22000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        wk->wu.cmwk[1] = 0;
        set_char_move_init(&wk->wu, 9, 0);
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 9, 8);
        break;

    case 2:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }
    }
}

void Appear_23000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    s16 work;
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] += 1;
        wk->wu.disp_flag = 1;
        work = random_16();
        work &= 3;
        wk->wu.cmwk[1] = 0;
        set_char_move_init(&wk->wu, 9, work + 4);
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);
        if ((wk->wu.cmwk[1]) && wk->wu.cg_type == 9) {
            wk->wu.routine_no[3] += 1;
            set_char_move_init(&wk->wu, 9, wk->wu.char_index + 8);
            return;
        } else {
            break;
        }
    case 2:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end += 1;
        }
        break;
    }
}

void Appear_24000(PLW* wk) {
    if (!wk->wu.operator) {
        if (wk->wu.id) {
            wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 0xA8;
        } else {
            wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work - 0x90;
        }
    }

    wk->wu.routine_no[2] = 1;
    wk->wu.routine_no[3] = 0;
}

void Appear_25000(PLW* wk) {
    if (!wk->wu.operator) {
        wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work;
    }

    wk->wu.routine_no[2] = 1;
    wk->wu.routine_no[3] = 0;
}

const s16 smoke_check[] = { 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 };

void Appear_26000(PLW* wk) {
    // s32 effect_86_init(s16 type86);

    switch (wk->wu.routine_no[3]) {
    case 0:
        bg_app_stop = 1;
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0x10);
        appear_work[wk->wu.id] = 0x14;
        Appear_free[wk->wu.id] = 0;
        break;

    case 1:
        appear_work[wk->wu.id]--;

        if (appear_work[wk->wu.id] < 1) {
            wk->wu.routine_no[3]++;
            appear_work[wk->wu.id] = 0x14;

            if (wk->wu.id) {
                cal_all_speed_data(&wk->wu, appear_work[wk->wu.id], bg_w.bgw[1].pos_x_work + 88, 0, 0, 1);
            } else {
                cal_all_speed_data(&wk->wu, appear_work[wk->wu.id], bg_w.bgw[1].pos_x_work - 88, 0, 0, 1);
            }
        }

        break;

    case 2:
        char_move(&wk->wu);
        appear_work[wk->wu.id]--;
        add_x_sub((WORK_Other*)wk);
        add_y_sub((WORK_Other*)wk);

        if (wk->wu.xyz[1].disp.pos < 0x41) {
            wk->wu.routine_no[3]++;
            wk->wu.hit_quake = 0x18;

            if (wk->wu.id == 0) {
                effect_86_init(0);
            }
            Sound_SE(0x109);
        }

        break;

    case 3:
        char_move(&wk->wu);

        if (wk->wu.hit_quake < 1) {
            wk->wu.routine_no[3]++;
            Appear_free[wk->wu.id] = 1;
        }

        break;

    case 4:
        char_move(&wk->wu);
        appear_work[wk->wu.id]--;

        if (appear_work[wk->wu.id] < 1) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 0x10, 3, 0);
            wk->wu.xyz[1].cal = 0;
        } else {
            add_x_sub((WORK_Other*)wk);
            add_y_sub((WORK_Other*)wk);
        }

        break;

    case 5:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 9) {
            wk->wu.rl_flag ^= 1;
            wk->wu.routine_no[3]++;
        }

        break;

    case 6:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }

        break;
    }
}

void Appear_28000(PLW* wk) {
    s16 id_w = wk->wu.id ^ 1;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        Appear_car_stop[id_w] = 0;
        set_char_move_init(&wk->wu, 9, 17);
        bg_app_stop = 1;
        break;

    case 1:
        if (Appear_car_stop[id_w]) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 17, 2, 0);
        }

        break;

    case 2:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 9) {
            wk->wu.routine_no[3]++;
        }

        break;

    case 3:
        if (plw[id_w].wu.routine_no[3] >= 3) {
            wk->wu.routine_no[3]++;
            appear_work[wk->wu.id] = 20;
        }

        break;

    case 4:
        appear_work[wk->wu.id]--;

        if (appear_work[wk->wu.id] < 1) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 17, 15, 0);
        }

        break;

    case 5:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }

        break;
    }
}

void Appear_29000(PLW* wk) {
    s16 work;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        bg_app_stop = 1;
        wk->wu.disp_flag = 1;
        wk->wu.cmwk[1] = 0;
        wk->wu.cmwk[2] = 0;
        work = random_16();
        work &= 3;
        wk->wu.cmwk[2] = work;
        set_char_move_init(&wk->wu, 9, 0);
        work = random_16();

        if (work & 1) {
            effect_09_init2(&wk->wu, 0x19);
        }
        if (8 < work) {
            effect_09_init2(&wk->wu, 0x1b);
        }

        animal_decide(wk);
        break;

    case 1:
        if (wk->wu.cmwk[1]) {
            switch (wk->wu.cmwk[2]) {
            case 0:
            case 1:
                wk->wu.routine_no[3] = 2;

                if (wk->wu.id) {
                    wk->wu.mvxy.a[0].sp = 0xffff0000;
                } else {
                    wk->wu.mvxy.a[0].sp = 0x10000;
                }

                set_char_move_init(&wk->wu, 9, 8);
                break;

            case 2:
                wk->wu.routine_no[3] = 3;

                if (wk->wu.id) {
                    wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 0xd8;
                } else {
                    wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + -0xd8;
                }

                set_char_move_init(&wk->wu, 9, 10);
                break;

            case 3:
                wk->wu.routine_no[3] = 4;
                set_char_move_init(&wk->wu, 9, 0xb);
                wk->wu.mvxy.d[0].sp = 0;
                wk->wu.mvxy.d[1].sp = 0xffff8000;
                wk->wu.xyz[1].disp.pos = 0xb0;
                app_counter[wk->wu.id] = 0x20;

                if (wk->wu.id) {
                    cal_initial_speed(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work + 0x58, 0);
                } else {
                    cal_initial_speed(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work - 0x58, 0);
                }
            }
        }

        break;

    case 2:
        char_move(&wk->wu);
        add_x_sub((WORK_Other*)&wk->wu);

        if (wk->wu.id) {
            if (!(wk->wu.xyz[0].disp.pos <= (bg_w.bgw[1].pos_x_work + 0x58))) {
                return;
            }
        } else if (wk->wu.xyz[0].disp.pos < (bg_w.bgw[1].pos_x_work - 0x58)) {
            return;
        }

        wk->wu.routine_no[2] = 1;
        wk->wu.routine_no[3] = 0;
        Appear_end++;
        break;

    case 3:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }
        break;

    case 4:
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] < 1) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 0xb, 5, 0);
            wk->wu.xyz[1].disp.pos = 0;
        } else {
            add_x_sub((WORK_Other*)&wk->wu);
            add_y_sub((WORK_Other*)&wk->wu);
        }

        break;

    case 5:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }

        break;
    }
}

void animal_decide(PLW* wk) {
#if defined(TARGET_PS2)
    s32 effect_M0_init(u8 pl_rl, u8 animal_type);
#endif

    u8 work2;
    s16 work = random_16();

    work2 = animal_decide_tbl[work];
    don_appear_check(wk);

    switch (work2) {
    case 0:
        break;

    case 1:
        effect_M0_init(wk->wu.rl_flag, 0);
        effect_M0_init(wk->wu.rl_flag, 1);
        break;

    default:
        effect_M0_init(wk->wu.rl_flag, work2);
        break;
    }

    return;
}

void don_appear_check(PLW* wk) {
#if defined(TARGET_PS2)
    s32 effect_M0_init(u8 pl_rl, u8 animal_type);
#endif

    s16 id_w = wk->wu.id ^ 1;
    if (plw[id_w].player_number == 7) {
        effect_M0_init(wk->wu.rl_flag, 6);
    }
}

void Appear_30000(PLW* wk) {
    s16 work;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        bg_app_stop = 1;
        work = random_16();
        work &= 3;
        appear_data_set(wk, (APPEAR_DATA*)appear_data + 24);
        set_char_move_init(&wk->wu, 9, 0xE);
        break;

    case 1:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end += 1;
        }
    }
}

void Appear_31000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;

        if (Appear_flag[wk->wu.id]) {
            appear_data_set(wk, (APPEAR_DATA*)appear_data);
            Appear_00000(wk);
        } else {
            set_char_move_init(&wk->wu, 9, 8);
        }

        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }

        break;
    }
}

void Appear_32000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    s16 work;
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] += 1;
        wk->wu.disp_flag = 1;
        work = random_16();
        work &= 7;
        set_char_move_init(&wk->wu, 9, work + 8);
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end += 1;
        }
    }
}

void Appear_33000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 9, 0xC);
        wk->wu.mvxy.d[0].sp = 0;
        wk->wu.mvxy.d[1].sp = -0x8000;
        wk->wu.xyz[1].disp.pos = 0x50;
        app_counter[wk->wu.id] = 0x2A;

        if (wk->wu.id) {
            cal_initial_speed(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work + 0x58, 0);
        } else {
            cal_initial_speed(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work - 0x58, 0);
        }

        bg_app_stop = 1;
        don_appear_check(wk);
        break;

    case 1:
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] <= 0) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 0xC, 2, 0);
            wk->wu.xyz[1].disp.pos = 0;
            return;
        }

        add_x_sub((WORK_Other*)wk);
        add_y_sub((WORK_Other*)wk);
        break;

    case 2:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }
    }
}

void Appear_34000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
#endif

    s16 work;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] += 1;
        wk->wu.disp_flag = 1;

        work = random_16();
        work &= 7;
        set_char_move_init(&wk->wu, 9, work);

        switch (work) {
        case 0:
        case 2:
        case 6:
        case 7:
            if (wk->wu.id) {
                wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 0x71;
            } else {
                wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work - 0x71;
            }
        }
        bg_app_stop = 1;
        break;

    default:
        Appear_01000(wk);
    }
}

void Appear_36000(PLW* wk) {
    s16 id_w = wk->wu.id ^ 1;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0x10);
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);

        if (plw[id_w].wu.cmwk[0] == 3) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, 9, 0x11);
            app_counter[wk->wu.id] = 0x10;
            wk->wu.next_z = plw[id_w].wu.my_priority;
        }

        break;

    case 2:
        char_move(&wk->wu);
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] <= 0) {
            wk->wu.routine_no[3]++;
            app_counter[wk->wu.id] = 0x16;

            if (wk->wu.id) {
                cal_all_speed_data(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work + 0x58, 0, 2, 0);
            } else {
                cal_all_speed_data(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work - 0x58, 0, 2, 0);
            }
        }

        wk->wu.next_z = plw[id_w].wu.my_priority;
        break;

    case 3:
        char_move(&wk->wu);
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] <= 0) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 0x11, 0x0A, 0);
            wk->wu.next_z = wk->wu.my_priority;
        } else {
            add_x_sub((WORK_Other*)wk);
        }

        break;

    case 4:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }

        break;
    }
}

const u8 animal_decide_tbl[] = { 0, 1, 2, 3, 4, 5, 0, 2, 0, 1, 2, 3, 4, 5, 0, 0 };

void Appear_37000(PLW* wk) {
    s16 id_w = wk->wu.id ^ 1;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0x11);
        bg_app_stop = 1;
        wk->wu.cmwk[0] = 0;
        effect_M1_init(&wk->wu);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[3]++;
            wk->wu.cmwk[0] = 1;
        }
        break;

    case 2:
        char_move(&wk->wu);

        if (wk->wu.cmwk[0] == 2) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, 9, 0x12);
        }

        break;

    case 3:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, 9, 0x13);
        }

        break;

    case 4:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 9) {
            wk->wu.routine_no[3]++;
            wk->wu.cmwk[0] = 3;
            wk->wu.next_z = plw[id_w].wu.my_priority;
        }

        break;

    case 5:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, 0, 3);
            app_counter[wk->wu.id] = 0x2a;

            if (wk->wu.id) {
                cal_all_speed_data(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work + 0x58, 0, 0, 0);
            } else {
                cal_all_speed_data(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work - 0x58, 0, 0, 0);
            }

            wk->wu.next_z = wk->wu.my_priority;
        } else {
            wk->wu.next_z = plw[id_w].wu.my_priority;
        }

        break;

    case 6:
        char_move(&wk->wu);
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] < 1) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        } else {
            add_x_sub((WORK_Other*)&wk->wu);
        }

        break;
    }
}

void Appear_38000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        bg_app_stop = 1;
        wk->wu.disp_flag = 1;

        if (wk->wu.id) {
            set_char_move_init(&wk->wu, 9, 0x14);
            return;
        }

        set_char_move_init(&wk->wu, 9, 0x13);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }
    }
}

void Appear_39000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;

        if (Gill_Appear_Flag) {
            appear_data_set(wk, (APPEAR_DATA*)appear_data);
            Appear_00000(wk);
            return;
        }

        bg_app_stop = 1;
        set_char_move_init(&wk->wu, 0, 2);

        if (wk->wu.id) {
            wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 0x200;
        }

        setup_mvxy_data(&wk->wu, 0);
        wk->wu.mvxy.a[0].sp >>= 1;
        add_mvxy_speed(&wk->wu);
        wk->wu.mvxy.a[0].sp *= 2;
        break;

    case 1:
        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);
        char_move(&wk->wu);

        if (wk->wu.id) {
            if (wk->wu.xyz[0].disp.pos < (bg_w.bgw[1].pos_x_work + 88)) {
                wk->wu.routine_no[3]++;
                wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 88;
                return;
            }
        } else {
            if (wk->wu.xyz[0].disp.pos > (bg_w.bgw[1].pos_x_work - 88)) {
                wk->wu.routine_no[3] += 1;
                wk->wu.xyz[0].disp.pos = (bg_w.bgw[1].pos_x_work - 88);
                return;
            }
        }

        break;

    case 2:
        wk->wu.routine_no[2] = 1;
        wk->wu.routine_no[3] = 0;
        Appear_end += 1;
        break;
    }
}

void Appear_41000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        bg_app_stop = 1;
        set_char_move_init(&wk->wu, 0, 0);
        app_counter[wk->wu.id] = 0x78;
        effect_M7_init(wk);
        break;

    case 1:
        char_move(&wk->wu);
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] < 0) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }

        break;
    }
}

void gouki_appear(PLW* wk) {
    if (!wk->wu.cmwk[0]) {
        char_move(&wk->wu);
        return;
    }

    switch (wk->wu.routine_no[6]) {
    case 0:
        wk->wu.routine_no[6]++;
        set_char_move_init(&wk->wu, 1, 0x3C);
        char_move_z(&wk->wu);
        wk->wu.xyz[1].disp.pos = -6;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[6]++;
        }

        break;

    case 2:
        break;
    }
}
