/**
 * @file effe9.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effe9.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"

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

    if (ewk->wu.type < 2) {
        ita_p[0].x = ita_p[1].x = 0.0f;
        ita_p[2].x = ita_p[3].x = 384.0f * Frame_Zoom_X;

        if (ewk->wu.type == 0) {
            ita_p[0].y = ita_p[3].y = 175.0f * Frame_Zoom_Y;
            ita_p[1].y = ita_p[2].y = 224.0f * Frame_Zoom_Y;
        } else {
            ita_p[0].y = ita_p[3].y = 0.0f;
            ita_p[1].y = ita_p[2].y = 33.0f * Frame_Zoom_Y;
        }
    } else {
        ita_p[0].y = ita_p[2].y = 0.0f;
        ita_p[1].y = ita_p[3].y = 224.0f * Frame_Zoom_Y;

        if (ewk->wu.type == 2) {
            ita_p[0].x = ita_p[1].x = 0.0f;
            ita_p[2].x = ita_p[3].x = Frame_Zoom_X;
        } else {
            ita_p[0].x = ita_p[1].x = 384.0f * Frame_Zoom_X;
            ita_p[2].x = ita_p[3].x = 385.0f * Frame_Zoom_X;
        }
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;

        if (!No_Trans) {
            ita_pos[0] = ita_p[0];
            ita_pos[1] = ita_p[3];
            ita_pos[2] = ita_p[1];
            ita_pos[3] = ita_p[2];
            njDrawPolygon2D((PAL_CURSOR*)&ita.p, 4, prio, 0x60);
        }

        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.routine_no[0] = 3;
            ewk->wu.disp_flag = 0;
            break;
        }

        if (End_PL == 14 && ewk->wu.type < 2) {
            if (ewk->wu.type) {
                ita_p[0].y = ita_p[3].y = 0.0f;
                ita_p[1].y = ita_p[2].y = (33.0f - ((33.0f * scr_sc) - 33.0f)) * Frame_Zoom_Y;
            } else {
                ita_p[0].y = ita_p[3].y = (224.0f - (1.0f + (48.0f - ((48.0f * scr_sc) - 48.0f)))) * Frame_Zoom_Y;
                ita_p[1].y = ita_p[2].y = 224.0f * Frame_Zoom_Y;
            }
        }

        if (end_w.r_no_0 >= 6) {
            ewk->wu.routine_no[0]++;
        }

        if (!No_Trans) {
            ita_pos[0] = ita_p[0];
            ita_pos[1] = ita_p[3];
            ita_pos[2] = ita_p[1];
            ita_pos[3] = ita_p[2];
            njDrawPolygon2D((PAL_CURSOR*)&ita.p, 4, prio, 0x60);
            break;
        }

        break;

    case 2:
        if (ewk->wu.type) {
            ita_p[0].y = ita_p[3].y = 0.0f;
            ita_p[1].y = ita_p[2].y = 16.0f * Frame_Zoom_Y;
        } else {
            ita_p[0].y = ita_p[3].y = 207.0f * Frame_Zoom_Y;
            ita_p[1].y = ita_p[2].y = 224.0f * Frame_Zoom_Y;
        }

        if (!No_Trans) {
            ita_pos[0] = ita_p[0];
            ita_pos[1] = ita_p[3];
            ita_pos[2] = ita_p[1];
            ita_pos[3] = ita_p[2];
            njDrawPolygon2D((PAL_CURSOR*)&ita.p, 4, prio, 0x60);
        }

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
