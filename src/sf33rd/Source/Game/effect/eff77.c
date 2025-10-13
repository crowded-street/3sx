/**
 * @file eff77.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff77.h"
#include "common.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/workuser.h"

s16 chk77_flag;

const s16 eff77_data_tbl[7][2] = { { 64, 0 }, { 2, 0 }, { 1, 1 }, { 32, 1 }, { 22, 1 }, { 1, 0 }, { 1, 0 } };

const u32 eff77_col_tbl[2] = { 0xFFFFFFFF, 0xFF000000 };

void effect_77_move(WORK_Other* ewk) {
    s16 i;
    u16 bg;
    u16 mask;

    u16 assign1;
    u16 assign2;
    u16 assign3;

    another_bg[0] = another_bg[1] = 0;
    Flash_MT[0] = Flash_MT[1] = 0;

    if (akebono_flag) {
        ewk->wu.routine_no[0] = 99;
    }

    for (i = 0; i < 3; i++) {
        scr_calc(i);
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (Game_pause || EXE_flag) {
            break;
        }

        ewk->wu.routine_no[0]++;
        plw[0].wu.disp_flag = 0;
        plw[1].wu.disp_flag = 0;

        if (ewk->wu.type == 0) {
            Extra_Break = 1;
        }

        /* fallthrough */

    case 1:
        if (Game_pause || EXE_flag) {
            break;
        }

        ewk->wu.routine_no[0]++;
        sa_pa_flag = 1;
        bg = ake_bg_off[bg_w.stage];
        mask = 1;

        for (i = 0; i < 4; i++, assign1 = mask *= 2) {
            if (bg & mask) {
                Bg_Off_R(1 << i);
            }
        }

        ewk->wu.old_rno[0] = eff77_data_tbl[ewk->wu.type][0];
        ewk->wu.old_rno[1] = eff77_data_tbl[ewk->wu.type][1];
        overwrite_panel(eff77_col_tbl[ewk->wu.old_rno[1]], 0x46);
        break;

    case 2:
        if (Game_pause || EXE_flag) {
            overwrite_panel(eff77_col_tbl[ewk->wu.old_rno[1]], 0x46);
            break;
        }

        ewk->wu.routine_no[0]++;
        bg = ake_bg_off[bg_w.stage];
        mask = 1;

        for (i = 0; i < 4; i++, assign2 = mask *= 2) {
            if (bg & mask) {
                Bg_Off_R(1 << i);
            }
        }

        sa_pa_flag = 1;
        /* fallthrough */

    case 3:
        if (Suicide[6]) {
            ewk->wu.old_rno[0] = 0;
        }

        if (!Game_pause && !EXE_flag) {
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] <= 0) {
                sa_pa_flag = 0;
                ewk->wu.routine_no[0]++;
                plw[0].wu.disp_flag = 1;
                plw[1].wu.disp_flag = 1;
                overwrite_panel(eff77_col_tbl[ewk->wu.old_rno[1]], 0x46);
                bg = ake_bg_off[bg_w.stage];
                mask = 1;

                for (i = 0; i < 4; i++, assign3 = mask *= 2) {
                    if (bg & mask) {
                        Bg_On_R(1 << i);
                    }
                }

                break;
            }

            sa_pa_flag = 1;
        }

        overwrite_panel(eff77_col_tbl[ewk->wu.old_rno[1]], 0x46);
        break;

    case 4:
        if (Game_pause || EXE_flag) {
            break;
        }

        ewk->wu.routine_no[0]++;

        if (ewk->wu.type == 0) {
            Extra_Break = 0;
        }

        break;

    default:
        if (!Game_pause && !EXE_flag) {
            sa_pa_flag = 0;
            all_cgps_put_back(&ewk->wu);
            push_effect_work(&ewk->wu);
        }

        break;
    }
}

s32 effect_77_init(u8 /* unused */, u8 data) {
    WORK_Other* ewk;
    s16 ix;

    chk77_flag = 0;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 77;
    ewk->wu.type = data;
    another_bg[0] = another_bg[1] = 0;
    Flash_MT[0] = Flash_MT[1] = 0;
    sa_pa_flag = 1;
    return 0;
}
