/**
 * @file sc_sub_transition.c
 * Screen Transitions
 *
 * Split out of sc_sub.c: the wipes, fades and tone-downs that cover the screen
 * between scenes, and the fade controller that drives them.
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

void WipeInit() {
    WipeLimit = 0;
}

/* The wipe-out's two directions: bands closing up the screen, and bands
 * sweeping across it. Each owns its counter and writes only the caller's quad. */
static void wipe_out_vertical(PAL_CURSOR* wipe_pc, PAL_CURSOR_P* wipe_p, s32 dmylim) {
    s32 i;

    wipe_p[0].x = wipe_p[2].x = 0.0f;
    wipe_p[1].x = wipe_p[3].x = 384.0f;

    for (i = 224; i > 0; i -= 8) {
        wipe_p[0].y = wipe_p[1].y = i;
        wipe_p[2].y = wipe_p[3].y = (i - (dmylim + 1));
        njDrawPolygon2D(wipe_pc, 4, PrioBase[0], 32);
    }
}

static void wipe_out_horizontal(PAL_CURSOR* wipe_pc, PAL_CURSOR_P* wipe_p, s32 dmylim) {
    s32 i;

    wipe_p[0].y = wipe_p[1].y = 0.0f;
    wipe_p[2].y = wipe_p[3].y = 224.0f;

    for (i = -224; i < 384; i += 8) {
        wipe_p[0].x = i;
        wipe_p[1].x = (i + dmylim + 1);
        wipe_p[2].x = 224.0f + wipe_p[0].x;
        wipe_p[3].x = 224.0f + wipe_p[1].x;
        njDrawPolygon2D(wipe_pc, 4, PrioBase[0], 32);
    }
}

static void draw_wipe_out(u8 type) {
    PAL_CURSOR wipe_pc;
    PAL_CURSOR_P wipe_p[4];
    PAL_CURSOR_COL wipe_col[4];
    s32 dmylim;

    if (WipeLimit > 7) {
        dmylim = 7;
    } else {
        dmylim = WipeLimit;
    }

    wipe_pc.p = wipe_p;
    wipe_pc.col = wipe_col;
    wipe_pc.tex = 0;
    wipe_pc.num = 4;
    wipe_col[0].color = wipe_col[1].color = wipe_col[2].color = wipe_col[3].color = 0xFF000000;

    if (type == 0) {
        wipe_out_vertical(&wipe_pc, wipe_p, dmylim);
    } else if (WipeLimit != 8) {
        wipe_out_horizontal(&wipe_pc, wipe_p, dmylim);
    }
}

s32 WipeOut(u8 type) {
    if (WipeLimit > 7) {
        overwrite_panel(0xFF000000, 0);
    }

    if (WipeLimit == 9) {
        overwrite_panel(0xFF000000, 0);
        return 1;
    }

    if (!No_Trans) {
        draw_wipe_out(type);
    }

    WipeLimit += 1;
    return (WipeLimit < 8) ? 0 : 1;
}

s32 WipeIn(u8 type) {
    PAL_CURSOR wipe_pc;
    PAL_CURSOR_P wipe_p[4];
    PAL_CURSOR_COL wipe_col[4];
    s32 i;

    if ((WipeLimit < 8) && !No_Trans) {
        wipe_pc.p = &wipe_p[0];
        wipe_pc.col = &wipe_col[0];
        wipe_pc.tex = 0;
        wipe_pc.num = 4;
        wipe_col[0].color = wipe_col[1].color = wipe_col[2].color = wipe_col[3].color = 0xFF000000;

        if (type == 0) {
            wipe_p[0].x = wipe_p[2].x = 0.0f;
            wipe_p[1].x = wipe_p[3].x = 384.0f;

            for (i = 0; i < 224; i += 8) {
                wipe_p[0].y = wipe_p[1].y = i;
                wipe_p[2].y = wipe_p[3].y = ((i + 8) - (WipeLimit + 1));
                njDrawPolygon2D(&wipe_pc, 4, PrioBase[0], 32);
            }
        } else {
            wipe_p[0].y = wipe_p[1].y = 0.0f;
            wipe_p[2].y = wipe_p[3].y = 224.0f;

            for (i = -224; i < 384; i += 8) {
                wipe_p[0].x = i;
                wipe_p[1].x = ((i + 8) - (WipeLimit + 1));
                wipe_p[2].x = 224.0f + wipe_p[0].x;
                wipe_p[3].x = 224.0f + wipe_p[1].x;
                njDrawPolygon2D(&wipe_pc, 4, PrioBase[0], 32);
            }
        }
    }

    WipeLimit += 1;
    return (WipeLimit < 8) ? 0 : 1;
}

void FadeInit() {
    FadeLimit = 1;
}

/* The full-screen quad the fades, the tone-down and the panel all draw: the
 * four corners from Fade_Pos_tbl and one colour on every vertex. The four
 * differ in nothing but that colour. */
static void fill_fade_quad(PAL_CURSOR_P* p, PAL_CURSOR_COL* col, u32 color) {
    u8 i;

    for (i = 0; i < 4; i++) {
        p[i].x = Fade_Pos_tbl[i * 2];
        p[i].y = Fade_Pos_tbl[i * 2 + 1];
        col[i].color = color;
    }
}

s32 FadeOut(u8 type, u8 step, u8 priority) {
    PAL_CURSOR fade_pc;
    PAL_CURSOR_P fade_p[4];
    PAL_CURSOR_COL fade_col[4];
    u32 Alpha;
    u8 flag;

    Alpha = 0xFF000000;
    flag = 0;

    if (No_Trans) {
        return 0;
    }

    njColorBlendingMode(0, 1);
    fade_pc.p = fade_p;
    fade_pc.col = fade_col;
    fade_pc.num = 4;

    if ((FadeLimit * step) < 255) {
        Alpha = (FadeLimit * step) << 24;
    } else {
        flag = 1;
    }

    if (type == 0) {
        Alpha |= 0x00FFFFFF;
    }

    fill_fade_quad(fade_p, fade_col, Alpha);

    njDrawPolygon2D(&fade_pc, 4, PrioBase[priority], 0x60);

    if (flag) {
        return 1;
    }

    FadeLimit += 1;
    return 0;
}

s32 FadeIn(u8 type, u8 step, u8 priority) {
    PAL_CURSOR fade_pc;
    PAL_CURSOR_P fade_p[4];
    PAL_CURSOR_COL fade_col[4];
    u32 Alpha;
    u8 flag;

    Alpha = 0;
    flag = 0;

    njColorBlendingMode(0, 1);
    fade_pc.p = fade_p;
    fade_pc.col = fade_col;
    fade_pc.num = 4;

    if (FadeLimit * step < 255) {
        Alpha = (255 - FadeLimit * step) << 24;
    } else {
        flag = 1;
    }

    if (type == 0) {
        Alpha |= 0x00FFFFFF;
    }

    fill_fade_quad(fade_p, fade_col, Alpha);

    if (!No_Trans) {
        njDrawPolygon2D(&fade_pc, 4, PrioBase[priority], 0x60);
    }

    if (flag) {
        return 1;
    }

    FadeLimit += 1;
    return 0;
}

/* The run ToneDown and overwrite_panel share, from the blending mode to the
 * draw. Only the colour differs inside it; overwrite_panel's own
 * ppgSetupCurrentDataList stays at its call site. */
static void draw_full_screen_quad(u32 color, u8 priority) {
    PAL_CURSOR quad_pc;
    PAL_CURSOR_P quad_p[4];
    PAL_CURSOR_COL quad_col[4];

    njColorBlendingMode(0, 1);
    quad_pc.p = quad_p;
    quad_pc.col = quad_col;
    quad_pc.num = 4;

    fill_fade_quad(quad_p, quad_col, color);

    njDrawPolygon2D(&quad_pc, 4, PrioBase[priority], 0x60);
}

void ToneDown(u8 tone, u8 priority) {
    if (No_Trans) {
        return;
    }

    draw_full_screen_quad(tone << 24, priority);
}

void overwrite_panel(u32 color, u8 priority) {
    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);

    draw_full_screen_quad(color, priority);
}

void fade_cont_init() {
    FadeInit();
    fd_dat.fade_kind = fade_data_tbl[Fade_Number][0];
    fd_dat.fade = fade_data_tbl[Fade_Number][1];
    fd_dat.fade_prio = fade_data_tbl[Fade_Number][2];
}

void fade_cont_main() {
    u8 flag = 0;

    switch (fd_dat.fade_kind) {
    case 0:
        flag = FadeIn(1, fd_dat.fade, fd_dat.fade_prio);
        break;

    case 1:
        flag = FadeOut(1, fd_dat.fade, fd_dat.fade_prio);
        break;

    case 2:
        flag = FadeIn(0, fd_dat.fade, fd_dat.fade_prio);
        break;

    case 3:
        flag = FadeOut(0, fd_dat.fade, fd_dat.fade_prio);
        break;
    }

    if (flag == 1) {
        Fade_Flag = 0;
    }
}

void Akaobi() {
    PAL_CURSOR apc;
    PAL_CURSOR_P ap[4];
    PAL_CURSOR_COL acol[4];
    u8 i;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    njColorBlendingMode(0, 1);
    apc.p = ap;
    apc.col = acol;
    apc.num = 4;

    for (i = 0; i < 4; i++) {
        ap[i].x = Akaobi_Pos_tbl[i * 2];
        ap[i].y = Akaobi_Pos_tbl[(i * 2) + 1];
        acol[i].color = 0xA0D00000;
    }

    njDrawPolygon2D(&apc, 4, PrioBase[2], 0x60);
}
