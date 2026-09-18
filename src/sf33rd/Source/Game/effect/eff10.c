/**
 * @file eff10.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff10.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

const u8 Contents_Check_Data[8] = { 0, 0, 1, 1, 0, 0, 0, 0 };

const char* button_string_data[8][12] = {
    { "CONTINUE",
      "BUTTON CONFIG.",
      "EXIT",
      "QUIT GAME ?",
      "RESTART",
      "CHARACTER CHANGE",
      "QUIT TRAINING?",
      NULL,
      NULL,
      NULL,
      NULL,
      NULL },
    { "YES", "NO", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "L.PUNCH",
      "M.PUNCH",
      "H.PUNCH",
      "L.KICK",
      "M.KICK",
      "H.KICK",
      "L.P+L.K",
      "M.P+M.K",
      "H.P+H.K",
      "PUNCH*3",
      "KICK*3",
      "NONE" },
    { "VIBRATION OFF", "VIBRATION ON", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "DEFAULT SETTING", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "0", "1", "2", "3", "4", "5", "6", "7", NULL, NULL, NULL, NULL },
    { "CONTINUE", "TRAINING MENU", "EXIT", "QUIT GAME ?", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "PLAYER 1", "PLAYER 2", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
};

static s32 pause_input_is_blocked(void) {
    return Game_pause == 0x81 && Pause_Down == 0;
}


/* Button-image rows 2 and 5 swap places on the second interface layout, so the
 * image index needs shifting for those two rows only. */
static s16 e10_row_correction(const WORK_Other* ewk) {
    if (ewk->wu.type == 5 && Interface_Type[ewk->master_id] == 1) {
        if (ewk->master_priority == 2) {
            return 4;
        }

        if (ewk->master_priority == 5) {
            return 2;
        }
    }

    return 0;
}

void effect_10_move(WORK_Other* ewk) {
    s16 color;
    s16 correct_index;
    s16 ix;

    if (pause_input_is_blocked()) {
        return;
    }

    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    correct_index = e10_row_correction(ewk);

    if (ewk->master_priority != Menu_Cursor_Y[ewk->master_id]) {
        color = 9;
    } else {
        color = 5;
    }

    if (Contents_Check_Data[ewk->wu.type] == 1) {
        ix = Convert_Buff[1][ewk->master_id][ewk->master_priority];
    } else {
        ix = ewk->wu.cg_type;
    }

    if (ewk->wu.type == 5) {
        dispButtonImage2(&(ButtonImage){ (ewk->wu.xyz[0].disp.pos * 8) - 6,
                                        (ewk->wu.xyz[1].disp.pos * 8) - 5,
                                        1,
                                        22,
                                        17,
                                        0,
                                        ix + correct_index,
                                        ewk->master_id });
    } else {
        SSPutStr2(
            ewk->wu.xyz[0].disp.pos,
            ewk->wu.xyz[1].disp.pos,
            color,
            button_string_data[ewk->wu.type][ix + correct_index]
        );
    }
}

s32 effect_10_init(const Effect10Init* p) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 10;
    ewk->wu.work_id = 16;
    ewk->master_id = p->id;
    ewk->wu.type = p->Type;
    ewk->master_priority = p->Type_in_Type;
    ewk->wu.cg_type = p->dir_step;
    ewk->master_player = p->Death_Type;
    ewk->wu.xyz[0].disp.pos = p->pos_x;
    ewk->wu.xyz[1].disp.pos = p->pos_y;
    return 0;
}
