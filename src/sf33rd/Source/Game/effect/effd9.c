/**
 * @file effd9.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effd9.h"
#include "common.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/workuser.h"

const s16 coltbl_000_1P[10] = { 1, 8192, 2, 8193, 1, 8195, 2, 8193, 0, 0 };
const s16 coltbl_000_2P[10] = { 1, 8208, 2, 8209, 1, 8211, 2, 8209, 0, 0 };

const s16 coltbl_001_1P[6] = { 2, 8192, 2, 8194, 0, 0 };
const s16 coltbl_001_2P[6] = { 2, 8208, 2, 8210, 0, 0 };

const s16 coltbl_002_1P[4] = { 999, 8195, 0, 0 };
const s16 coltbl_002_2P[4] = { 999, 8211, 0, 0 };

const s16 coltbl_003_1P[6] = { 2, 8192, 2, 8193, 0, 0 };
const s16 coltbl_003_2P[6] = { 2, 8208, 2, 8209, 0, 0 };

const s16 coltbl_004_1P[6] = { 2, 8192, 2, 8196, 0, 0 };
const s16 coltbl_004_2P[6] = { 2, 8208, 2, 8212, 0, 0 };

const s16 coltbl_005_1P[6] = { 1, 8694, 2, 8192, 0, 0 };
const s16 coltbl_005_2P[6] = { 1, 8698, 2, 8208, 0, 0 };

const s16 coltbl_006_1P[6] = { 99, 8198, 99, 8198, 0, 0 };
const s16 coltbl_006_2P[6] = { 99, 8214, 99, 8214, 0, 0 };

const s16 coltbl_007_1P[10] = { 1, 8195, 2, 8192, 1, 8195, 2, 8192, 0, 0 };
const s16 coltbl_007_2P[10] = { 1, 8211, 2, 8208, 1, 8211, 2, 8208, 0, 0 };

const s16 coltbl_008_1P[8] = { 2, 8192, 2, 8193, 2, 8193, 0, 0 };
const s16 coltbl_008_2P[8] = { 2, 8208, 2, 8209, 2, 8209, 0, 0 };

const s16 coltbl_009_1P[4] = { 52, 8197, 0, 0 };
const s16 coltbl_009_2P[4] = { 52, 8213, 0, 0 };

const s16 coltbl_010_1P[4] = { 999, 8197, 0, 0 };
const s16 coltbl_010_2P[4] = { 999, 8213, 0, 0 };

const ColorTableIndex color_table_index[11] = {
    { 8, 0, coltbl_000_1P, coltbl_000_2P },   { 7, 24, coltbl_003_1P, coltbl_003_2P },
    { 7, 30, coltbl_001_1P, coltbl_001_2P },  { 5, 0, coltbl_002_1P, coltbl_002_2P },
    { 7, 18, coltbl_004_1P, coltbl_004_2P },  { 18, 24, coltbl_005_1P, coltbl_005_2P },
    { 17, 24, coltbl_006_1P, coltbl_006_2P }, { 2, 10, coltbl_007_1P, coltbl_007_2P },
    { 8, 0, coltbl_008_1P, coltbl_008_2P },   { 19, 52, coltbl_009_1P, coltbl_009_2P },
    { 17, 24, coltbl_010_1P, coltbl_010_2P }
};

void effect_D9_move(WORK_Other* ewk) {
    PLW* mwk = (PLW*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        if (ewk->master_id) {
            ewk->wu.step_xy_table = (s16*)color_table_index[ewk->wu.direction].changetbl_2p;
        } else {
            ewk->wu.step_xy_table = (s16*)color_table_index[ewk->wu.direction].changetbl_1p;
        }
        ewk->wu.vital_old = color_table_index[ewk->wu.direction].flag;
        ewk->wu.dir_timer = color_table_index[ewk->wu.direction].timer;
        ewk->wu.dir_step = 0;
        ewk->wu.vitality = 0;
        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.routine_no[0]++;
            break;
        }

        if ((ewk->wu.vital_old & 2) == 0 || EXE_flag != 0 || Game_pause != 0 || mwk->wu.hit_stop > 0 ||
            (--ewk->wu.dir_timer >= 0)) {
            if ((ewk->wu.vital_old & 4) != 0) {
                if (ewk->wu.dir_old == mwk->wu.dm_count_up) {
                    if ((ewk->wu.type != 0) && (ewk->wu.type != 32)) {
                        if (mwk->wu.xyz[1].disp.pos <= 0) {
                            goto set_routine_2;
                        }
                    } else {
                        if (mwk->wu.cg_type != 0) {
                            goto set_routine_2;
                        }
                    }
                } else {
                    goto set_routine_2;
                }
            }

            if (((ewk->wu.vital_old & 8) == 0 || (mwk->sa->ok == -1)) &&
                (((ewk->wu.vital_old & 0x10) == 0) || (ewk->wu.total_paring == mwk->wu.kind_of_waza))) {
                if (--ewk->wu.vitality <= 0) {
                    ewk->wu.dir_step += 2;

                    if (ewk->wu.step_xy_table[ewk->wu.dir_step] == 0) {
                        ewk->wu.dir_step = 0;
                    }

                    ewk->wu.vitality = ewk->wu.step_xy_table[ewk->wu.dir_step];
                    ewk->wu.vital_new = ewk->wu.step_xy_table[ewk->wu.dir_step + 1];
                }

                if ((ewk->wu.vital_old & 1) != 0) {
                    mwk->wu.extra_col = ewk->wu.vital_new;
                } else {
                    mwk->wu.extra_col_2 = ewk->wu.vital_new;
                }

                break;
            }
        }

    set_routine_2:
        ewk->wu.routine_no[0] = 2;
        break;

    case 2:
    default:
        if ((ewk->wu.vital_old & 1) != 0) {
            mwk->wu.extra_col = 0;
        } else {
            mwk->wu.extra_col_2 = 0;
        }
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_D9_init(PLW* wk, u8 data) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(6)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 139;
    ewk->wu.work_id = 16;
    ewk->wu.direction = data;
    ewk->wu.dir_old = wk->wu.dm_count_up;
    ewk->wu.dm_attribute = wk->wu.dm_attribute;
    ewk->wu.type = wk->wu.pat_status;
    ewk->wu.total_paring = wk->wu.kind_of_waza;
    ewk->my_master = (u32*)wk;
    ewk->master_id = wk->wu.id;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_player = wk->player_number;
    return 0;
}
