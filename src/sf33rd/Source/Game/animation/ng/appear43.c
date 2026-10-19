/**
 * @file appear43.c
 * Players falling onto the bridge in the second area of Elena's stage from New Generation (appear routine 13 in the NG
 * arcade ROM).
 */

#include "sf33rd/Source/Game/animation/ng/appear43.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

// The jump in the air, like the fall from the bridge. NG plays its own fall (koc 9 0x3D) and landing (0x3E), which
// only NG's characters have.
#define FALL_KOC 0
#define FALL_CHAR_INDEX 15

void Appear_43000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        bg_app_stop = 1;
        set_char_move_init(&wk->wu, FALL_KOC, FALL_CHAR_INDEX);
        wk->wu.mvxy.a[1].sp = 0x78000;
        wk->wu.mvxy.d[1].sp = -0x3000;
        wk->wu.kage_flag = 0;
        break;

    case 1:
        if (bg_w.bridge.landed) {
            wk->wu.kage_flag = 1;
            wk->wu.routine_no[3]++;
        }

        /* fallthrough */

    case 2:
        char_move(&wk->wu);
        add_y_sub_ng(&wk->wu);

        if (wk->wu.xyz[1].disp.pos < 0) {
            wk->wu.routine_no[3]++;
            wk->wu.xyz[1].cal = 0;
            char_move_cmja(&wk->wu);
            Appear_end++;
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
