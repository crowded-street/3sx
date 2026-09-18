/**
 * @file vital.c
 * Vitality Bars
 */

#include "sf33rd/Source/Game/engine/vital.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

VIT vit[2];

void vital_cont_init() {
    u8 i;

    for (i = 0; i < 2; i++) {
        vit[i].cyerw = 160;
        vit[i].cred = 160;
        vit[i].ored = 160;
        vit[i].colnum = 1;
        gauge_stop_flag[i] = 0;
        vital_stop_flag[i] = 0;
    }
}

void vital_cont_main() {
    if (omop_cockpit != 0) {
        if (!EXE_flag && !Game_pause) {
            if (vital_stop_flag[0] == 0 && gauge_stop_flag[0] == 0) {
                vital_control(0);
            }

            if (vital_stop_flag[1] == 0 && gauge_stop_flag[1] == 0) {
                vital_control(1);
            }
        } else {
            vital_parts_allwrite(0);
            vital_parts_allwrite(1);
        }
    }
}

/* The bar has already caught up with the health value, so there is nothing to
 * animate this frame. */
static s32 vital_bar_is_settled(u8 pl) {
    return (vit[pl].cyerw == plw[pl].wu.vital_new) && (vit[pl].cred == plw[pl].wu.vital_new) &&
           (vit[pl].ored != (plw[pl].wu.vital_new + 1));
}

/* Full bar, low bar, and everything between get their own colour. */
static void set_vital_colour(u8 pl) {
    if (plw[pl].wu.vital_new == 160) {
        vit[pl].colnum = 1;
    } else if (plw[pl].wu.vital_new < 49) {
        vit[pl].colnum = 3;
    } else {
        vit[pl].colnum = 2;
    }
}

void vital_control(u8 pl) {
    if (plw[pl].wu.vital_new >= 161) {
        return;
    }

    if (vital_bar_is_settled(pl)) {
        if (No_Trans == 0) {
            vital_parts_allwrite(pl);
        }
        return;
    }

    if (vit[pl].cred < plw[pl].wu.vital_new) {
        vit[pl].cred = plw[pl].wu.vital_new;
    }

    vit[pl].cyerw = plw[pl].wu.vital_new;

    if (plw[pl].wu.vital_new < 0) {
        vit[pl].cyerw = 0;
    }

    set_vital_colour(pl);

    if (No_Trans == 0) {
        vital_parts_allwrite(pl);
    }

    vit[pl].ored = vit[pl].cred;
    vit[pl].cred--;

    if (vit[pl].cred < plw[pl].wu.vital_new) {
        vit[pl].cred = plw[pl].wu.vital_new;
    }
}

void vital_parts_allwrite(u8 Pl_Num) {
    scfont_sqput(&(ScFontSquare){ Pl_Num * 27, 2, 1, 0, Pl_Num, Pl_Num + 30, 21, 1 }, TopHUDVitalPriority);

    if (omop_vt_bar_disp[Pl_Num] == 0) {
        silver_vital_put(Pl_Num);
        return;
    }

    vital_put(&(VitalBar){ Pl_Num, vit[Pl_Num].colnum, vit[Pl_Num].cyerw, 0 }, TopHUDPriority);
    vital_put(&(VitalBar){ Pl_Num, 1, vit[Pl_Num].cred, 1 }, TopHUDShadowPriority);
    vital_base_put(Pl_Num);
}
