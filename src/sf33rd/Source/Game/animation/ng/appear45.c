/**
 * @file appear45.c
 * Yun's intro against Yang on Yang's stage from New Generation: NG's regular intro (Appear_01000, its record 2).
 */

#include "sf33rd/Source/Game/animation/ng/appear45.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/stage/bg_data.h"

#define APPEAR_KOC 10
#define TAP_CHAR_INDEX 0
#define BOW_CHAR_INDEX 1

static void bow(PLW* wk) {
    set_char_move_init(&wk->wu, APPEAR_KOC, BOW_CHAR_INDEX);
}

void Appear_45000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        bg_app_stop = 1;
        wk->wu.char_table[APPEAR_KOC] = _ng_yun_bow_char_table;
        set_char_move_init(&wk->wu, APPEAR_KOC, TAP_CHAR_INDEX);
        break;

    case 1:
        if (Appear_flag[wk->wu.id]) {
            wk->wu.routine_no[3]++;
            char_move(&wk->wu);
            break;
        }

        wk->wu.routine_no[3] = 3;
        bow(wk);
        break;

    case 2:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 9 && Appear_flag[wk->wu.id] == 0) {
            wk->wu.routine_no[3]++;
            bow(wk);
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
