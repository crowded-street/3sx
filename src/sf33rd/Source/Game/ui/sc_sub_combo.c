/**
 * @file sc_sub_combo.c
 * Combo and Score Readouts
 *
 * Split out of sc_sub.c: the combo message and hit counter, the score digits
 * and the naming display.
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

void combo_message_set(u8 pl, u8 kind, u8 x, u8 num, u8 hi, u8 low) {
    u8 xw;
    u8 xw2;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);

    if (num > combo_mtbl[kind][2]) {
        xw = combo_mtbl[kind][2];
    } else {
        xw = num;
    }

    if (num > combo_mtbl[kind][2]) {
        xw2 = (num - (combo_mtbl[kind][2]));
    } else {
        xw2 = 0;
    }

    switch (kind) {
    case 2:
    case 1:
    case 0:
        if (pl == 0) {
            if (hi != 0) {
                scfont_sqput(x, 7, 8, 0, hi, 6, 1, 2, 2);
            }

            if (num > 1) {
                scfont_sqput(x + 1, 7, 8, 0, low, 6, 1, 2, 2);
            }

            if (num > 3) {
                scfont_sqput(x + 3, 7, 8, 2, combo_mtbl[kind][0], combo_mtbl[kind][1], xw, 2, 2);
                return;
            }
        } else {
            scfont_sqput(xw2, 7, 8, 2, (combo_mtbl[kind][0] + combo_mtbl[kind][2]) - xw, combo_mtbl[kind][1], xw, 2, 2);

            if (xw2 > 1) {
                scfont_sqput(xw2 - 2, 7, 8, 0, low, 6, 1, 2, 2);
            }

            if ((xw2 > 2) && (hi != 0)) {
                scfont_sqput(xw2 - 3, 7, 8, 0, hi, 6, 1, 2, 2);
                return;
            }
        }

        break;

    case 3:
    case 4:
    case 5:
    case 6:
        if (pl == 0) {
            scfont_sqput(x, 7, 8, 2, combo_mtbl[kind][0], combo_mtbl[kind][1], xw, 2, 2);
        } else {
            scfont_sqput(xw2, 7, 8, 2, (combo_mtbl[kind][0] + combo_mtbl[kind][2]) - xw, combo_mtbl[kind][1], xw, 2, 2);
        }

        break;
    }
}

void combo_pts_set(u8 pl, u8 x, u8 num, s8* pts, s8 digit) {
    s8 i;
    s8 j;

    s8 assign1;
    u8 assign2;
    u8 assign3;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);

    if (pl == 0) {
        for (i = digit, assign1 = j = 1; i >= 0; i--, j++, assign2 = x += 1) {
            score8x16_put(x, 10, 8, pts[i], 2);

            if (num - j == 0) {
                return;
            }
        }

        if (num < digit + 1) {
            return;
        }

        score8x16_put(x, 10, 8, 0, 2);

        if (num < digit + 2) {
            return;
        }

        score8x16_put(x + 1, 10, 8, 0, 2);

        if (num < digit + 3) {
            return;
        }

        scfont_put(&(ScFontCell){ x + 2, 11, 8, 0, 6, 13 }, 2);

        if (num < digit + 4) {
            return;
        }

        scfont_put(&(ScFontCell){ x + 3, 11, 8, 0, 7, 13 }, 2);

    } else {
        scfont_put(&(ScFontCell){ x, 11, 8, 0, 7, 13 }, 2);

        if (num > 1) {
            scfont_put(&(ScFontCell){ x - 1, 11, 8, 0, 6, 13 }, 2);
        }

        if (num > 2) {
            score8x16_put(x - 2, 10, 8, 0, 2);
        }

        if (num > 3) {
            score8x16_put(x - 3, 10, 8, 0, 2);
        }

        if (num > 4) {
            for (i = 0; i <= digit; i++, assign3 = x -= 1) {
                score8x16_put(x - 4, 10, 8, pts[i], 2);

                if (num - i == 0) {
                    break;
                }
            }
        }
    }
}

void score8x16_put(u16 x, u16 y, u8 atr, u8 chr, u8 priority) {
    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    scfont_sqput(x, y, atr, 0, chr, 6, 1, 2, priority);
}

void score16x24_put(u16 x, u16 y, u8 atr, u8 chr) {
    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    scfont_sqput(x, y, atr, 2, chr * 2, 6, 2, 3, 2);
}

void naming_set(u8 pl, s16 place, u16 atr, u16 chr) {
    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    scfont_put(&(ScFontCell){ place + 13 + (pl * 27), 0, atr, 0, rankname_pos_tbl[chr][0], rankname_pos_tbl[chr][1] }, 2);
}
