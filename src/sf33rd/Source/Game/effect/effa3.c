/**
 * @file effa3.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effa3.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

const u8 Contents_Check_Data_A3[23] = { 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0 };

s8* Letter_Data_A3[23][8] = {
    { "NORMAL MODE",
      "RECORDING MODE",
      "REPLAY",
      "DUMMY SETTING",
      "TRAINING OPTION",
      "BUTTON CONFIG.",
      "CHARACTER CHANGE",
      "EXIT" },
    { "ACTION$................",
      "GUARD$.................",
      "QUICK STANDING$........",
      "STUN$..................",
      "DEFAULT SETTING",
      "EXIT",
      NULL,
      NULL },
    { "STAND", "CROUCH", "JUMP", "CPU", "HUMAN", NULL, NULL, NULL },
    { "AUTO GUARD", "NO GUARD", "ALL GUARD", "PARRYING", "RANDOM GUARD", "RANDOM PARRYING", "ALL RANDOM", NULL },
    { "OFF", "ON", "RANDOM", NULL, NULL, NULL, NULL, NULL },
    { "OFF", "ON", NULL, NULL, NULL, NULL, NULL, NULL },
    { "S.A.GAUGE$.............",
      "ATTACK DATA$...........",
      "DAMAGE$................$L    $H",
      "DIFFICULTY$............$L        $H",
      "DEFAULT SETTING",
      "EXIT",
      NULL,
      NULL },
    { "NORMAL", "MAX START", "INFINITY", "MAXIMUM", NULL, NULL, NULL, NULL },
    { "OFF", "DATA", "DATA+HITBOXES", NULL, NULL, NULL, NULL, NULL },
    { "$ *", "$ **", "$ ***", "$ ****", NULL, NULL, NULL, NULL },
    { "$ *", "$ **", "$ ***", "$ ****", "$ *****", "$ ******", "$ *******", "$ ********" },
    { "DUMMY RECORDING",
      "PARRYING TRAINING",
      "TRAINING OPTION",
      "BUTTON CONFIG.",
      "CHARACTER CHANGE",
      "EXIT",
      NULL,
      NULL },
    { "STAND", "CROUCH", "JUMP", NULL, NULL, NULL, NULL, NULL },
    { "NORMAL", "MAX START", "INFINITY", NULL, NULL, NULL, NULL, NULL },
    { "NORMAL", "MAX START", "INFINITY", NULL, NULL, NULL, NULL, NULL },
    { "OFF", "ON", NULL, NULL, NULL, NULL, NULL, NULL },
    { "ACTION(PLAYER)$..........",
      "S.A.GAUGE(DUMMY)$........",
      "AUTO PARRYING$...........",
      "S.A.GAUGE(PLAYER)$.......",
      "DEFAULT SETTING",
      "EXIT",
      NULL,
      NULL },
    { "STAND", "CROUCH", "JUMP", NULL, NULL, NULL, NULL, NULL },
    { "NORMAL", "MAX START", "INFINITY", "MAXIMUM", NULL, NULL, NULL, NULL },
    { "OFF", "ON", NULL, NULL, NULL, NULL, NULL, NULL },
    { "NORMAL", "MAX START", "INFINITY", "MAXIMUM", NULL, NULL, NULL, NULL },
    { "RECORDING", "TRAINING", NULL, NULL, NULL, NULL, NULL, NULL },
    { "REPLAY DATA", "FINISHED$!", NULL, NULL, NULL, NULL, NULL, NULL }
};

void effect_A3_move(WORK_Other* ewk) {
    s16 color;
    s16 ix;
    s16 clear_level;
    f32 prio;

    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    if (ewk->master_priority != Menu_Cursor_Y[0]) {
        color = 9;
    } else {
        color = 5;
    }

    if (Contents_Check_Data_A3[ewk->wu.type] == 1) {
        ix = Training[2].contents[ewk->master_id][ewk->wu.dir_step][ewk->master_priority];
    } else {
        ix = ewk->wu.cg_type;
    }

    clear_level = 0;

    if (Record_Data_Tr == 0 && ewk->wu.type == 0 && ewk->master_priority == 2) {
        clear_level = 1;
    }
    if (Record_Data_Tr == 0 && ewk->wu.type == 11 && ewk->master_priority == 1) {
        clear_level = 1;
    }

    if (Training[2].contents[0][0][0] == 3 && ewk->wu.type == 0 && ewk->master_priority == 1) {
        clear_level = 1;
    }

    if (ewk->master_priority == 99) {
        color = 10;
    }

    if (ewk->wu.type < 22) {
        prio = 1.0f;
    } else {
        prio = 2.0f;
    }

    SSPutStr_Bigger(ewk->wu.xyz[0].disp.pos,
                    ewk->wu.xyz[1].disp.pos,
                    color,
                    Letter_Data_A3[ewk->wu.type][ix],
                    1.0f,
                    clear_level,
                    prio);
}

s32 effect_A3_init(s16 id, u8 Type, u8 Type_in_Type, u8 dir_step, u8 Death_Type, s16 pos_x, s16 pos_y, s16 Buff) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 103;
    ewk->wu.work_id = 16;
    ewk->master_id = id;
    ewk->wu.type = Type;
    ewk->master_priority = Type_in_Type;
    ewk->wu.cg_type = dir_step;
    ewk->master_player = Death_Type;
    ewk->wu.dir_step = Buff;
    ewk->wu.xyz[0].disp.pos = pos_x;
    ewk->wu.xyz[1].disp.pos = pos_y;
    return 0;
}
