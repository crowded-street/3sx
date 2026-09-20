/**
 * @file effe9.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effe9.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/system/work_sys.h"

static s32 should_adjust_end_panel(const WORK_Other* ewk) {
    return End_PL == 14 && ewk->wu.type < 2;
}


/* The horizontal band the first two panel types draw. Only this arm is lifted:
 * cutting the vertical one as well makes the two a duplication pair and
 * measures 9.38 against 10.00. */
static void effe9_panel_shape_band(const WORK_Other* ewk, PAL_CURSOR_P* ita_p) {
    ita_p[0].x = ita_p[1].x = 0.0f;
    ita_p[2].x = ita_p[3].x = 384.0f;

    if (ewk->wu.type == 0) {
        ita_p[0].y = ita_p[3].y = 175.0f;
        ita_p[1].y = ita_p[2].y = 224.0f;
    } else {
        ita_p[0].y = ita_p[3].y = 0.0f;
        ita_p[1].y = ita_p[2].y = 33.0f;
    }
}

static void effe9_panel_shape(const WORK_Other* ewk, PAL_CURSOR_P* ita_p) {
    if (ewk->wu.type < 2) {
        effe9_panel_shape_band(ewk, ita_p);
    } else {
        ita_p[0].y = ita_p[2].y = 0.0f;
        ita_p[1].y = ita_p[3].y = 224.0f;

        if (ewk->wu.type == 2) {
            ita_p[0].x = ita_p[1].x = 0.0f;
            ita_p[2].x = ita_p[3].x = 1.0f;
        } else {
            ita_p[0].x = ita_p[1].x = 384.0f;
            ita_p[2].x = ita_p[3].x = 385.0f;
        }
    }
}

static void effe9_draw(PAL_CURSOR* ita, const PAL_CURSOR_P* ita_p, PAL_CURSOR_P* ita_pos, f32 prio) {
    if (!No_Trans) {
        ita_pos[0] = ita_p[0];
        ita_pos[1] = ita_p[3];
        ita_pos[2] = ita_p[1];
        ita_pos[3] = ita_p[2];
        njDrawPolygon2D((PAL_CURSOR*)&ita->p, 4, prio, 0x60);
    }
}

static void effe9_scale_panel(const WORK_Other* ewk, PAL_CURSOR_P* ita_p) {
    if (ewk->wu.type) {
        ita_p[0].y = ita_p[3].y = 0.0f;
        ita_p[1].y = ita_p[2].y = (33.0f - ((33.0f * scr_sc) - 33.0f));
    } else {
        ita_p[0].y = ita_p[3].y = (224.0f - (1.0f + (48.0f - ((48.0f * scr_sc) - 48.0f))));
        ita_p[1].y = ita_p[2].y = 224.0f;
    }
}

static void effe9_final_panel(const WORK_Other* ewk, PAL_CURSOR_P* ita_p) {
    if (ewk->wu.type) {
        ita_p[0].y = ita_p[3].y = 0.0f;
        ita_p[1].y = ita_p[2].y = 16.0f;
    } else {
        ita_p[0].y = ita_p[3].y = 207.0f;
        ita_p[1].y = ita_p[2].y = 224.0f;
    }
}

void effect_E9_move(WORK_Other* ewk) {
    PAL_CURSOR ita;
    PAL_CURSOR_P ita_p[4];
    PAL_CURSOR_P ita_pos[4];
    PAL_CURSOR_COL ita_col[4];
    f32 prio;

    ita.p = &ita_pos[0];
    ita.col = &ita_col[0];
    ita.num = 4;
    ita_col[0].color = ita_col[1].color = ita_col[2].color = ita_col[3].color = 0xFF000000;
    prio = PrioBase[ewk->wu.my_priority];

    effe9_panel_shape(ewk, ita_p);

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;

        effe9_draw(&ita, ita_p, ita_pos, prio);

        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.routine_no[0] = 3;
            ewk->wu.disp_flag = 0;
            break;
        }

        if (should_adjust_end_panel(ewk)) {
            effe9_scale_panel(ewk, ita_p);
        }

        if (end_w.r_no_0 >= 6) {
            ewk->wu.routine_no[0]++;
        }

        effe9_draw(&ita, ita_p, ita_pos, prio);
        break;

    case 2:
        effe9_final_panel(ewk, ita_p);

        effe9_draw(&ita, ita_p, ita_pos, prio);

        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_E9_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 i;

    for (i = 0; i < 4; i++) {

        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }
        ewk = (WORK_Other*)frw[ix];
        ewk->wu.id = 149;
        ewk->wu.be_flag = 1;
        ewk->wu.type = i;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_family = 4;
        ewk->wu.my_priority = 19;
    }

    return 0;
}
