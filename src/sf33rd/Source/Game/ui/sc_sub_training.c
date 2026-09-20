/**
 * @file sc_sub_training.c
 * Training Mode Display
 *
 * Split out of sc_sub.c: the damage and frame readout drawn over training
 * mode, and the work area behind it.
 */

#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "common.h"
#include "constants.h"
#include "core/input.h"
#include "core/renderer.h"
#include "port/config/config.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/system/ramcnt.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/glyph_renderer.h"
#include "sf33rd/Source/Game/ui/sc_data.h"
#include "structs.h"
#include "core/xbox_buttons.h"

void Training_Disp_Work_Clear() {
    u8 i;

    for (i = 0; i < 2; i++) {
        tr_data[i].max_hitcombo = 0;
        tr_data[i].new_max_flag = 0;
        tr_data[i].frash_flag = 0;
        tr_data[i].frash_switch = 2;
        tr_data[i].damage = 0;
        tr_data[i].total_damage = 0;
        tr_data[i].disp_total_damage = 0;
    }
}

void Training_Damage_Set(s16 damage, s16 arg1, u8 kezuri) {
    u8 j;

    if (Training_ID == 0) {
        j = 1;
    } else {
        j = 0;
    }

    if (damage == 0) {
        return;
    }

    tr_data[j].damage = damage;

    if (tr_data[j].damage > 999) {
        tr_data[j].damage = 999;
    }

    if (kezuri) {
        tr_data[j].disp_total_damage = damage;
        tr_data[j].total_damage = 0;
    } else {
        tr_data[j].total_damage = damage + tr_data[j].total_damage;
        tr_data[j].disp_total_damage = tr_data[j].total_damage;
    }

    if (tr_data[j].disp_total_damage > 999) {
        tr_data[j].disp_total_damage = 999;
    }
}

/* The damage row and the running-total row, and the best-combo row below
 * them. Each draws both players' copies. */
static void Training_Disp_Damage_Rows(u8 j) {
    u8 i;

    for (i = 0; i < 2; i++) {
        scfont_sqput3(&(ScFontSquareWide){ i + Training_combo_pos_tbl[j], i + 48, 13, 4, 0, 176, 76, 8 }, i + 5, Training_combo_prio_tbl[i] + (sa_pa_flag * 14) * i);

        SSPutDec3(&(ScDec3){ i + (Training_combo_pos_tbl[j] + 158), i + 48, 13, tr_data[j].damage }, 3, i + 7, Training_combo_prio_tbl[i] + (sa_pa_flag * 14) * i);
    }

    for (i = 0; i < 2; i++) {
        scfont_sqput3(&(ScFontSquareWide){ i + (Training_combo_pos_tbl[j] + 1), i + 58, 13, 4, 0, 184, 134, 8 }, i + 5, Training_combo_prio_tbl[i] + (sa_pa_flag * 14) * i);

        SSPutDec3(&(ScDec3){ i + (Training_combo_pos_tbl[j] + 158), i + 58, 13, tr_data[j].disp_total_damage }, 3, i + 7, Training_combo_prio_tbl[i] + (sa_pa_flag * 14) * i);
    }
}

static void Training_Disp_Max_Combo_Row(u8 j, u8 atr, u8 gr) {
    u8 i;

    for (i = 0; i < 2; i++) {
        scfont_sqput3(&(ScFontSquareWide){ i + (Training_combo_pos_tbl[j] + 1), i + 68, 13, 4, 0, 192, 98, 8 }, i + 3, Training_combo_prio_tbl[i] + (sa_pa_flag * 14) * i);

        SSPutDec3(&(ScDec3){ i + (Training_combo_pos_tbl[j] + 158), i + 68, atr, tr_data[j].max_hitcombo }, 2, gr + i, Training_combo_prio_tbl[i] + (sa_pa_flag * 14) * i);
    }
}

void Training_Data_Disp() {
    u8 j;
    u8 atr;
    u8 gr;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);

    if (Disp_Attack_Data == 0) {
        return;
    }

    if (Training_ID == 0) {
        j = 1;
    } else {
        j = 0;
    }

    Training_Disp_Damage_Rows(j);

    if (tr_data[j].frash_flag) {
        atr = 0x1E;
        gr = 9;
    } else {
        atr = 13;
        gr = 7;
    }

    tr_data[j].frash_switch--;

    if (tr_data[j].new_max_flag != 0 && tr_data[j].frash_switch == 0) {
        tr_data[j].frash_switch = 2;
        tr_data[j].new_max_flag--;
        tr_data[j].frash_flag = ~tr_data[j].frash_flag;

        if (tr_data[j].frash_flag) {
            atr = 0x1E;
            gr = 0;
        }
    }

    Training_Disp_Max_Combo_Row(j, atr, gr);
}
