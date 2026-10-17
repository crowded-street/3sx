/**
 * @file eff83.c
 * Clouds of Ibuki's stage from New Generation.
 */

#include "sf33rd/Source/Game/effect/ng/eff83.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define CLOUD_COUNT 3
#define CLOUD_STEPS 8
#define COLOR_CODE 0x212C // NG 0x2040
#define END_X 0x380

typedef struct Cloud {
    s16 char_index;
    s16 x;
    s16 y;
    s16 priority;
    s32 speed;
} Cloud;

static const Cloud clouds[CLOUD_COUNT][CLOUD_STEPS] = {
    {
        { 7, 200, 292, 102, 0x1800 },
        { 9, 200, 292, 102, 0x2800 },
        { 8, 200, 292, 102, 0x1A00 },
        { 7, 200, 292, 102, 0x800 },
        { 8, 200, 248, 102, 0x800 },
        { 9, 248, 288, 102, 0x1000 },
        { 10, 200, 292, 102, 0xA00 },
        { 7, 200, 292, 102, 0x2000 },
    },
    {
        { 9, 240, 284, 102, 0x1A00 },
        { 8, 240, 268, 102, 0x2C00 },
        { 7, 240, 268, 102, 0x2000 },
        { 8, 240, 268, 102, 0x1800 },
        { 7, 240, 300, 102, 0xA00 },
        { 10, 240, 268, 102, 0x1A00 },
        { 7, 240, 268, 102, 0x1000 },
        { 9, 264, 268, 102, 0x2000 },
    },
    {
        { 10, 288, 240, 100, 0x2000 },
        { 7, 290, 240, 100, 0x3000 },
        { 10, 290, 264, 100, 0x2200 },
        { 8, 288, 240, 100, 0x1000 },
        { 8, 216, 248, 102, 0x1000 },
        { 10, 288, 240, 100, 0x1800 },
        { 7, 288, 240, 100, 0xF00 },
        { 10, 288, 256, 100, 0x2800 },
    },
};

void effect_ng83_move(WORK_Other* ewk) {
    const Cloud* cloud;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;

        // NG blinks the clouds every other frame to make them look translucent instead
        ewk->wu.disp_flag = 1;
        ewk->wu.my_clear_level = 128;

        cloud = &clouds[ewk->wu.type][ewk->wu.old_rno[0]];
        ewk->wu.char_index = cloud->char_index;
        ewk->wu.xyz[0].disp.pos = cloud->x;
        ewk->wu.xyz[1].disp.pos = cloud->y;
        ewk->wu.my_priority = ewk->wu.position_z = cloud->priority;
        ewk->wu.mvxy.a[0].sp = cloud->speed;
        ewk->wu.mvxy.d[0].sp = 0;
        ewk->wu.old_rno[0] = (ewk->wu.old_rno[0] + 1) % CLOUD_STEPS;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
            add_x_sub_ng(&ewk->wu);

            if (ewk->wu.xyz[0].disp.pos > END_X) {
                ewk->wu.routine_no[0] = 0;
            }
        }

        disp_pos_trans_entry_ng(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng83_init() {
    WORK_Other* ewk;
    s16 ix;

    for (s16 i = 0; i < CLOUD_COUNT; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = EFFECT_NG83_ID;
        ewk->wu.work_id = 16;
        ewk->wu.type = i;
        ewk->wu.old_rno[0] = 0;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_family = 3; // The sky layer
        ewk->wu.my_col_code = COLOR_CODE;
        ewk->wu.char_table[0] = _ng_j11_char_table;
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
