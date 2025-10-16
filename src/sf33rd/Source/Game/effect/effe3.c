/**
 * @file effe3.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effe3.h"
#include "common.h"
#include "sf33rd/Source/Game/SysDir.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plmain.h"
#include "sf33rd/Source/Game/engine/spgauge.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void effect_E3_move(WORK_Other* ewk) {
    PLW* mwk = (PLW*)ewk->my_master;
    s16 num;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if ((mwk->wu.E3_work_index != ewk->wu.myself || ewk->wu.dead_f != 0) ||
            (Mode_Type != MODE_NORMAL_TRAINING && Mode_Type != MODE_PARRY_TRAINING)) {
            ewk->wu.routine_no[0] = 2;
            break;
        }

        if (mwk->init_E3_flag == 0) {
            break;
        }

        mwk->init_E3_flag = 0;

        if (Mode_Type != MODE_NORMAL_TRAINING) {
            break;
        }

        if (mwk->wu.id == New_Challenger && Training[0].contents[0][0][0] != 4) {
            vib_sel[mwk->wu.id] = 0;
        }

        if (Training[0].contents[0][1][3] == 2) {
            vib_sel[mwk->wu.id] = 0;
        }

        ewk->wu.direction = 0;

        if (Training[0].contents[0][0][0] == 3 || Training[0].contents[0][0][0] == 4) {
            ewk->wu.direction = 1;
        }

        omop_vital_ix[mwk->wu.id] = 1;

        if (Training[0].contents[0][1][3] == 0) {
            omop_vital_ix[mwk->wu.id] = 3;
        }

        ewk->wu.dm_vital = 0;
        ewk->wu.vitality = 0;
        ewk->wu.dir_timer = 0;

        if (mwk->wu.id == New_Challenger) {
            mwk->py->now.quantity.h = 0;

            if (ewk->wu.direction == 0) {
                switch (Training[0].contents[0][0][1]) {
                case 0:
                case 1:
                case 6:
                    mwk->spmv_ng_flag &= 0xFFFFFFEF;
                    mwk->spmv_ng_flag |= 0xC0;
                    break;

                case 2:
                case 4:
                    mwk->spmv_ng_flag |= 0x80;
                    mwk->spmv_ng_flag &= 0xFFFFFFBF;
                    mwk->spmv_ng_flag &= 0xFFFFFFEF;
                    break;

                case 3:
                case 5:
                    mwk->spmv_ng_flag |= 0x40;
                    mwk->spmv_ng_flag &= 0xFFFFFF7F;
                    mwk->spmv_ng_flag &= 0xFFFFF0FF;
                    break;
                }
            } else {
                mwk->spmv_ng_flag |= 0xC0;
                mwk->spmv_ng_flag &= 0xFFFFFFEF;
                mwk->spmv_ng_flag &= 0xFFFFF0FF;
            }
        }

        switch (Training[0].contents[0][1][0]) {
        case 1:
            mwk->spmv_ng_flag2 &= 0xFFFBFFFF;
            mwk->spmv_ng_flag2 |= 0x90000;
            demo_set_sa_full(mwk->sa);
            tr_spgauge_cont_init2(mwk->wu.id);
            break;

        case 3:
            mwk->spmv_ng_flag2 &= 0xFFF7FFFF;
            mwk->spmv_ng_flag2 |= 0x50000;
            demo_set_sa_full(mwk->sa);
            tr_spgauge_cont_init2(mwk->wu.id);
            break;

        case 2:
            mwk->spmv_ng_flag2 &= 0xFFFEFFFF;
            mwk->spmv_ng_flag2 |= 0xC0000;
            clear_super_arts_point(mwk);
            tr_spgauge_cont_init(mwk->wu.id);
            break;

        case 0:
            mwk->spmv_ng_flag2 |= 0xD0000;
            clear_super_arts_point(mwk);
            tr_spgauge_cont_init(mwk->wu.id);
            break;
        }

        ewk->wu.routine_no[0]++;
        omop_spmv_ng_table[mwk->wu.id] = mwk->spmv_ng_flag;
        omop_spmv_ng_table2[mwk->wu.id] = mwk->spmv_ng_flag2;
        /* fallthrough */

    case 1:
        if ((mwk->wu.E3_work_index != ewk->wu.myself || ewk->wu.dead_f != 0) ||
            (Mode_Type != MODE_NORMAL_TRAINING && Mode_Type != MODE_PARRY_TRAINING)) {
            ewk->wu.routine_no[0] = 2;
            break;
        }

        if (mwk->init_E3_flag == 1) {
            ewk->wu.routine_no[0] = 0;
            mwk->spmv_ng_flag = ewk->master_ng_flag;
            mwk->spmv_ng_flag2 = ewk->master_ng_flag2;
            break;
        }

        if (New_Challenger != mwk->wu.id) {
            break;
        }

        if (ewk->wu.direction == 0) {
            mwk->cp->waza_flag[7] = 2;

            switch (Training[0].contents[0][0][2]) {
            case 0:
            sw1_case_0:
                mwk->spmv_ng_flag2 |= 0x200;
                break;

            case 1:
            sw1_case_1:
                mwk->spmv_ng_flag2 &= 0xFFFFFDFF;
                break;

            default:
                if (Game_timer & 0xF) {
                    break;
                }

                switch (mwk->spmv_ng_flag2 & 0x200) {
                case 0:
                    goto sw1_case_0;

                default:
                    goto sw1_case_1;
                }

                break;
            }

            switch (Training[0].contents[0][0][1]) {
            case 0:
                if (mwk->wu.routine_no[1] == 1 || mwk->wu.routine_no[1] == 3) {
                    ewk->wu.dm_vital = 1;
                    ewk->wu.dir_timer = 12;
                } else {
                    ewk->wu.dm_vital = 0;
                }

                if (ewk->wu.dm_vital) {
                    mwk->spmv_ng_flag &= 0xFFFFFFBF;
                } else {
                    if (ewk->wu.dir_timer == 0) {
                        mwk->spmv_ng_flag |= 0x40;
                    } else {
                        ewk->wu.dir_timer--;
                    }
                }

                break;

            case 5:
                if ((Game_timer & 2) != 0) {
                    goto sw2_case_3;
                }

            case 1:
            sw2_case_1:
                mwk->spmv_ng_flag |= 0x80;
                mwk->spmv_ng_flag |= 0x40;
                mwk->spmv_ng_flag |= 0x10;
                break;

            case 2:
            sw2_case_2:
                mwk->spmv_ng_flag |= 0x80;
                mwk->spmv_ng_flag &= 0xFFFFFFBF;
                mwk->spmv_ng_flag &= 0xFFFFFFEF;
                break;

            case 6:
                num = Game_timer % 3;

                if (num == 1) {
                    goto sw2_case_1;
                }

                if (num == 2) {
                    goto sw2_case_2;
                }

            case 3:
            sw2_case_3:
                mwk->spmv_ng_flag |= 0x40;
                mwk->spmv_ng_flag &= 0xFFFFFF7F;
                mwk->spmv_ng_flag &= 0xFFFFF0FF;
                break;

            case 4:
                if ((mwk->wu.routine_no[1] == 1 && mwk->wu.routine_no[2] >= 4) && mwk->wu.routine_no[2] < 14) {
                    ewk->wu.dm_vital = 1;
                    ewk->wu.dir_timer = 12;
                } else {
                    ewk->wu.dm_vital = 0;
                }

                if (ewk->wu.dm_vital) {
                    mwk->spmv_ng_flag &= 0xFFFFFFBF;
                } else {
                    if (ewk->wu.dir_timer == 0) {
                        if ((Game_timer & 4) != 0) {
                            mwk->spmv_ng_flag &= 0xFFFFFFBF;
                        } else {
                            mwk->spmv_ng_flag |= 0x40;
                        }
                    } else {
                        ewk->wu.dir_timer--;
                    }
                }

                break;
            }
        }

        if (Training[0].contents[0][0][3] == 1) {
            if ((mwk->wu.routine_no[1] == 1 && mwk->guard_chuu == 0) || mwk->wu.routine_no[1] == 3) {
                ewk->wu.vitality = 1;
                ewk->wu.dir_step = 20;
            } else {
                ewk->wu.vitality = 0;
            }

            if (ewk->wu.vitality == 0) {
                if (ewk->wu.dir_step == 0) {
                    mwk->py->now.quantity.h = mwk->py->genkai - 1;
                } else {
                    ewk->wu.dir_step--;
                }
            }
        }

        break;

    case 2:
    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_E3_init(PLW* wk) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 143;
    ewk->wu.work_id = 16;
    ewk->my_master = (u32*)wk;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_id = wk->wu.id;
    ewk->master_player = wk->player_number;
    ewk->master_ng_flag = wk->spmv_ng_flag;
    ewk->master_ng_flag2 = wk->spmv_ng_flag2;
    wk->init_E3_flag = 1;
    wk->wu.E3_work_index = ewk->wu.myself;
    return 0;
}
