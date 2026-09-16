/**
 * @file eff09_init.c
 * Effect 09 allocation and initialization.
 */

#include "sf33rd/Source/Game/effect/eff09.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

extern const s16 eff09_data[2][9];
extern const s16 eff09_data2[43][8];

s32 effect_09_init(WORK* wk, u8 data) {
    WORK_Other* ewk;
    s16 ix;
    const s16* data_ptr;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.type = data;
    data_ptr = eff09_data[ewk->wu.type];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 9;
    ewk->wu.work_id = 16;
    ewk->master_id = wk->id;
    ewk->wu.my_priority = 64;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.char_table[0] = _eff09_char_table;
    ewk->wu.routine_no[0] = *data_ptr++;
    ewk->wu.my_family = *data_ptr++;
    ewk->wu.my_col_code = *data_ptr++;
    ewk->wu.xyz[0].disp.pos = *data_ptr++;
    ewk->wu.xyz[1].disp.pos = *data_ptr++;
    ewk->wu.my_priority = ewk->wu.position_z = *data_ptr++;
    ;
    ewk->wu.char_index = *data_ptr++;
    ewk->wu.hit_stop = *data_ptr++;
    ewk->wu.sync_suzi = *data_ptr++;
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}

static s32 effect_09_init2_is_blocked_stage(u8 data) {
    return data == 0x1F && bg_w.stage == 0xA;
}

static s32 effect_09_init2_is_blocked_test_effect(u8 data) {
    return test_flag && data >= 32 && data < 41;
}

static const s16* configure_effect_09_init2_late(WORK_Other* ewk, const WORK* wk, u8 data, const s16* data_ptr) {
    switch (data) {
    case 34:
        push_color_trans_req(0x51, 0xA);
        /* fallthrough */

    case 35:
    case 38:
    case 39:
    case 40:
        ewk->wu.my_col_code = 0xA;
        data_ptr++;
        ewk->wu.my_mts = 0x10;
        break;

    case 41:
        ewk->wu.my_col_code = *data_ptr++;
        ewk->wu.my_col_code += wk->my_col_code;
        ewk->wu.my_mr_flag = 1;
        ewk->wu.my_mr.size.x = 127;
        ewk->wu.my_mr.size.y = 127;
        break;

    default:
        ewk->wu.my_col_code = *data_ptr++;
        ewk->wu.my_col_code += wk->my_col_code;
        break;
    }

    return data_ptr;
}

static const s16* configure_effect_09_init2(WORK_Other* ewk, const WORK* wk, u8 data, const s16* data_ptr) {
    switch (data) {
    case 18:
    case 19:
    case 31:
        ewk->wu.my_col_code = 0x2020;
        data_ptr++;
        break;

    case 24:
    case 25:
    case 26:
    case 27:
        ewk->wu.my_col_code = 0x3E;
        data_ptr++;
        break;

    case 32:
        ewk->wu.my_col_code = 0x4F;
        data_ptr++;
        break;

    case 33:
        ewk->wu.my_col_code = 0x50;
        data_ptr++;
        ewk->wu.my_mts = 7;
        break;

    default:
        return configure_effect_09_init2_late(ewk, wk, data, data_ptr);
    }

    return data_ptr;
}

s32 effect_09_init2(WORK* wk, u8 data) {
    WORK_Other* ewk;
    s16 ix;
    const s16* data_ptr;

    if (effect_09_init2_is_blocked_stage(data)) {
        return 0;
    }

    if (effect_09_init2_is_blocked_test_effect(data)) {
        return 0;
    }

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.type = data;
    ewk->my_master = wk;
    ewk->wu.target_adrs = wk->target_adrs;
    ewk->wu.be_flag = 1;
    ewk->wu.id = 9;
    ewk->wu.work_id = 16;
    ewk->master_id = wk->id;
    ewk->wu.cgromtype = 1;
    *ewk->wu.char_table = _etc_char_table;
    ewk->wu.my_col_mode = wk->my_col_mode;
    data_ptr = eff09_data2[ewk->wu.type];
    ewk->wu.routine_no[0] = *data_ptr++;
    ewk->wu.my_mts = 14;
    data_ptr = configure_effect_09_init2(ewk, wk, data, data_ptr);

    ewk->wu.my_family = wk->my_family;

    if (data == 4) {
        ewk->wu.rl_flag = 0;
    } else {
        ewk->wu.rl_flag = wk->rl_flag;
    }

    ewk->wu.xyz[0].disp.pos = wk->xyz[0].disp.pos;

    if (wk->rl_flag) {
        ewk->wu.xyz[0].disp.pos -= *(s16*)data_ptr++;
    } else {
        ewk->wu.xyz[0].disp.pos += *(s16*)data_ptr++;
    }

    ewk->wu.xyz[1].disp.pos = *data_ptr++;
    ewk->wu.xyz[1].disp.pos += wk->xyz[1].disp.pos;
    ewk->wu.my_priority = wk->my_priority;
    ewk->wu.my_priority += *(s16*)data_ptr++;
    ewk->wu.position_z = ewk->wu.my_priority;
    ewk->wu.char_index = *data_ptr++;
    ewk->wu.hit_stop = *data_ptr++;
    ewk->wu.sync_suzi = *data_ptr++;
    suzi_offset_set(ewk);
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
