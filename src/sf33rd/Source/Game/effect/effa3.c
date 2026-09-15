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
    { "OFF", "1-HIT STUN", "NO STUN GAIN", NULL, NULL, NULL, NULL, NULL },
    { "S.A.GAUGE$.............",
      "ATTACK DATA$...........",
      "HITBOXES$..............",
      "INPUT HISTORY$.........",
      "DAMAGE$................$L    $H",
      "DIFFICULTY$............$L        $H",
      "DEFAULT SETTING",
      "EXIT" },
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

static s32 is_record_clear_level(const WORK_Other* ewk) {
    return Record_Data_Tr == 0 && ewk->wu.type == 0 && ewk->master_priority == 2;
}

static s32 is_recording_display(const WORK_Other* ewk) {
    return Record_Data_Tr == 0 && ewk->wu.type == 11 && ewk->master_priority == 1;
}

static s32 is_training_clear_level(const WORK_Other* ewk) {
    return Training[2].contents[0][0][0] == 3 && ewk->wu.type == 0 && ewk->master_priority == 1;
}

static s16 get_clear_level_A3(const WORK_Other* ewk) {
    if (is_record_clear_level(ewk)) {
        return 1;
    }

    if (is_recording_display(ewk)) {
        return 1;
    }

    if (is_training_clear_level(ewk)) {
        return 1;
    }

    return 0;
}

static s16 get_text_color_A3(const WORK_Other* ewk) {
    s16 color;

    if (ewk->master_priority != Menu_Cursor_Y[0]) {
        color = 9;
    } else {
        color = 5;
    }

    if (ewk->master_priority == 99) {
        color = 10;
    }

    return color;
}

static s16 get_text_index_A3(const WORK_Other* ewk) {
    if (Contents_Check_Data_A3[ewk->wu.type] == 1) {
        return Training[2].contents[ewk->master_id][ewk->wu.dir_step][ewk->master_priority];
    }

    return ewk->wu.cg_type;
}

static f32 get_text_priority_A3(const WORK_Other* ewk) {
    if (ewk->wu.type < 22) {
        return 1.0f;
    }

    return 2.0f;
}

void effect_A3_move(WORK_Other* ewk) {
    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    SSPutStr_Bigger(ewk->wu.xyz[0].disp.pos,
                    ewk->wu.xyz[1].disp.pos,
                    get_text_color_A3(ewk),
                    Letter_Data_A3[ewk->wu.type][get_text_index_A3(ewk)],
                    1.0f,
                    get_clear_level_A3(ewk),
                    get_text_priority_A3(ewk));
}

s32 effect_A3_init_params(EffectA3Init init) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 103;
    ewk->wu.work_id = 16;
    ewk->master_id = init.id;
    ewk->wu.type = init.type;
    ewk->master_priority = init.type_in_type;
    ewk->wu.cg_type = init.dir_step;
    ewk->master_player = init.death_type;
    ewk->wu.dir_step = init.buff;
    ewk->wu.xyz[0].disp.pos = init.pos_x;
    ewk->wu.xyz[1].disp.pos = init.pos_y;
    return 0;
}
