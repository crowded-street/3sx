/**
 * @file cmd_main.c
 * Command Input Parser
 */

#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "common.h"
#include "sf33rd/Source/Game/SysDir.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*chk_move_jp[28])() = { check_init, check_0,  check_1,  check_2,  check_3,  check_4,  check_5,
                              check_6,    check_7,  check_7,  check_9,  check_10, check_11, check_12,
                              check_13,   check_14, check_15, check_16, check_16, check_18, check_19,
                              check_20,   check_21, check_22, check_23, check_24, check_25, check_26 };

void waza_check(PLW* pl) {
    cmd_pl = pl;
    cmd_id = cmd_pl->wu.id;
    chk_pl = &t_pl_lvr[cmd_id];
    sw_pick_up();
    cmd_move();
}

void key_thru(PLW* pl) {
    cmd_pl = pl;
    cmd_id = cmd_pl->wu.id;
    chk_pl = &t_pl_lvr[cmd_id];
    sw_pick_up();
}

void cmd_data_set(PLW* /* unused */, s16 i) {
    u8* ptr3;
    u16* ptr4;

    wcp[cmd_id].reset[i] = *cmd_tbl_ptr++;
    waza_work[cmd_id][i].w_dead = *cmd_tbl_ptr++;
    waza_work[cmd_id][i].w_dead2 = *cmd_tbl_ptr++;

    ptr3 = &wcp[cmd_id].waza_r[i][0];
    *ptr3++ = (s8)*cmd_tbl_ptr++;
    *ptr3++ = (s8)*cmd_tbl_ptr++;
    *ptr3++ = (s8)*cmd_tbl_ptr++;
    *ptr3++ = (s8)*cmd_tbl_ptr++;

    wcp[cmd_id].btix[i] = *cmd_tbl_ptr++;

    ptr4 = &wcp[cmd_id].exdt[i][0];
    *ptr4++ = *cmd_tbl_ptr++;
    *ptr4++ = *cmd_tbl_ptr++;
    *ptr4++ = *cmd_tbl_ptr++;
    *ptr4++ = *cmd_tbl_ptr++;

    switch (i) {
    case 3:
    case 4:
    case 5:
        wcp[cmd_id].reset[i] += blok_b_omake[omop_b_block_ix[cmd_id]];
        make_red_blocking_time(cmd_id, i, wcp[cmd_id].reset[i]);
        break;

    case 6:
    case 12:
        wcp[cmd_id].reset[i] += blok_b_omake[omop_b_block_ix[cmd_id]];
        break;
    }
}

void cmd_init(PLW* pl) {
    s16 i;
    s16 j;
    s32* ptr;

    cmd_id = pl->wu.id;
    pl->cp = &wcp[cmd_id];
    ptr = (s32*)&waza_work[cmd_id][0];

    for (i = 0; i < 56; i++) {
        for (j = 0; j < 6; j++) {
            *ptr++ = 0;
        }
    }

    for (i = 0; i < 56; i++) {
        wcp[cmd_id].waza_flag[i] = 0;
        for (j = 0; j < 4; j++) {
            wcp[cmd_id].waza_r[i][j] = 0;
        }
    }

    waza_compel_all_init(pl);
}

void cmd_move() {
    s16 j;
    intptr_t* adrs;

    cmd_id = cmd_pl->wu.id;

    if (cmd_sel[cmd_id]) {
        adrs = pl_CMD[cmd_pl->player_number];
    } else {
        adrs = pl_cmd[cmd_pl->player_number];
    }

    for (j = 0; j < 56; j++) {
        if (wcp[cmd_id].waza_flag[j] != -1) {
            waza_type[cmd_id] = j;
            cmd_tbl_ptr = (s16*)adrs[j];
            waza_ptr = &waza_work[cmd_id][j];
            chk_move_jp[waza_ptr->w_type]();
        }
    }

    for (j = 0; j < 56; j++) {
        if ((wcp[cmd_id].waza_flag[j] != -1) && (wcp[cmd_id].waza_flag[j] != 0)) {
            waza_ptr = &waza_work[cmd_id][j];
            command_ok_move(j);
        }
    }
}

void check_init() {
    cmd_tbl_ptr += 12;
    waza_ptr->w_type = *cmd_tbl_ptr++;
    waza_ptr->w_int = *cmd_tbl_ptr++;
    waza_ptr->free1 = *cmd_tbl_ptr;
    waza_ptr->free2 = *cmd_tbl_ptr++;
    waza_ptr->w_lvr = *cmd_tbl_ptr++;
    waza_ptr->w_ptr = cmd_tbl_ptr;
    waza_ptr->uni0.tame.flag = 0;
    waza_ptr->uni0.tame.shot_flag = 0;
    waza_ptr->uni0.tame.shot_flag2 = 0;
    waza_ptr->shot_ok = 0;
    waza_ptr->free3 = 0;
    chk_move_jp[waza_ptr->w_type]();
}

void check_next() {
    s16* next_ptr = waza_ptr->w_ptr;

    waza_ptr->w_type = *next_ptr++;
    waza_ptr->w_int = *next_ptr++;
    waza_ptr->free1 = *next_ptr;
    waza_ptr->free2 = *next_ptr++;
    waza_ptr->w_lvr = *next_ptr++;
    waza_ptr->w_ptr = next_ptr;

    if (waza_ptr->w_type != 10) {
        chk_move_jp[waza_ptr->w_type]();
    }
}

void check_0() {
    u16 sw_lever;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    sw_lever = chk_pl->sw_lever & 0xF;

    if (dead_lvr_check() == 0) {
        if (waza_ptr->w_lvr & 0x8000) {
            sw_work = waza_ptr->w_lvr & 0xF;

            if (sw_lever == sw_work) {
                if (*waza_ptr->w_ptr == 28) {
                    command_ok();
                    return;
                }

                check_next();
            }
        } else if (waza_ptr->w_lvr == 0) {
            if (sw_lever == 0) {
                if (*waza_ptr->w_ptr == 28) {
                    command_ok();
                    return;
                }

                check_next();
            }
        } else if (chk_pl->now_lvbt & 0xF && sw_lever & waza_ptr->w_lvr) {
            if (*waza_ptr->w_ptr == 28) {
                command_ok();
                return;
            }

            check_next();
        }
    }
}

void check_1() {
    if (dead_lvr_check() == 0) {
        sw_work = waza_ptr->w_lvr & 0xF;
        if (waza_ptr->w_lvr & 0x8000) {
            if (sw_work == chk_pl->sw_lever) {
                waza_ptr->free2--;

                if (!waza_ptr->uni0.tame.flag && waza_ptr->free2 < 0) {
                    waza_ptr->uni0.tame.flag = 1;
                }
            } else {
                if (waza_ptr->uni0.tame.flag) {
                    waza_ptr->uni0.tame.flag = 0;

                    if (*waza_ptr->w_ptr == 0x1C) {
                        command_ok();
                    } else {
                        check_next();
                    }
                    return;
                }

                waza_ptr->free2 = waza_ptr->free1;
                waza_ptr->w_int--;

                if (waza_ptr->w_int < 0) {
                    waza_ptr->w_type = 0;
                }
            }
        } else {
            if (sw_work & chk_pl->sw_lever) {
                if (!waza_ptr->uni0.tame.flag) {
                    waza_ptr->free1--;

                    if (waza_ptr->free1 < 0) {
                        waza_ptr->uni0.tame.flag = 1;
                    }
                }
            } else {
                if (waza_ptr->uni0.tame.flag) {
                    waza_ptr->uni0.tame.flag = 0;

                    if (*waza_ptr->w_ptr == 0x1C) {
                        command_ok();
                    } else {
                        check_next();
                    }
                    return;
                }

                waza_ptr->free2 = waza_ptr->free1;
                waza_ptr->w_int--;

                if (waza_ptr->w_int < 0) {
                    waza_ptr->w_type = 0;
                }
            }
        }
    }
}

void check_2() {
    sw_work = chk_pl->sw_new & waza_ptr->w_lvr;

    if (waza_ptr->w_lvr == sw_work) {
        if (!waza_ptr->uni0.tame.flag) {
            waza_ptr->free2--;

            if (waza_ptr->free2 < 0) {
                waza_ptr->uni0.tame.flag = 1;
            }
        }
    } else {
        if (waza_ptr->uni0.tame.flag && sw_work == 0) {
            waza_ptr->uni0.tame.flag = 0;

            if (*waza_ptr->w_ptr == 28) {
                command_ok();
            } else {
                check_next();
            }

            return;
        }

        waza_ptr->free2 = waza_ptr->free1;
        waza_ptr->w_int--;

        if (waza_ptr->w_int < 0) {
            waza_ptr->w_type = 0;
        }
    }
}

void check_3() {
    s16 i;
    s16 w_flag;
    s16* shot_cnt_adrs;

    sw_work = chk_pl->sw_new & 0x770;
    waza_ptr->uni0.tame.shot_flag2 = waza_ptr->uni0.tame.shot_flag;
    waza_ptr->uni0.tame.shot_flag = 0;
    shot_cnt_adrs = &chk_pl->s1_cnt;
    w_flag = 0x10;

    for (i = 0; i < 6; i++) {
        if (*shot_cnt_adrs >= waza_ptr->w_int) {
            waza_ptr->uni0.tame.shot_flag |= w_flag;
        }

        shot_cnt_adrs++;

        if ((chk_pl->shot_down & w_flag) && (waza_ptr->uni0.tame.shot_flag2 & w_flag)) {
            waza_ptr->shot_ok++;
        }

        w_flag <<= 1;
    }

    if (waza_ptr->shot_ok) {
        waza_ptr->free2--;

        if (waza_ptr->free2 < 0) {
            waza_ptr->shot_ok = 0;
            waza_ptr->free2 = waza_ptr->free1;
        }
    }

    if (waza_ptr->shot_ok >= waza_ptr->w_lvr) {
        waza_ptr->shot_ok = 0;
        waza_ptr->free2 = waza_ptr->free1;

        if (*waza_ptr->w_ptr == 28) {
            command_ok();
            return;
        }

        check_next();
    }
}

void check_4() {
    if (waza_ptr->w_lvr == 0x10) {
        if (chk_pl->sw_now & 0x10) {
            waza_ptr->uni0.tame.flag++;
        }

        if (chk_pl->sw_now & 0x20) {
            waza_ptr->uni0.tame.shot_flag++;
        }

        if (chk_pl->sw_now & 0x40) {
            waza_ptr->uni0.tame.shot_flag2++;
        }
    } else {
        if (chk_pl->sw_now & 0x100) {
            waza_ptr->uni0.tame.flag++;
        }

        if (chk_pl->sw_now & 0x200) {
            waza_ptr->uni0.tame.shot_flag++;
        }

        if (chk_pl->sw_now & 0x400) {
            waza_ptr->uni0.tame.shot_flag2++;
        }
    }

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->uni0.tame.flag = 0;
        waza_ptr->uni0.tame.shot_flag = 0;
        waza_ptr->uni0.tame.shot_flag2 = 0;
        waza_ptr->w_int = waza_ptr->free1;
    }

    if (wcp[cmd_id].waza_flag[waza_type[cmd_id]]) {
        if (waza_ptr->w_int > 0 && waza_ptr->uni0.tame.shot_flag2) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
            waza_ptr->uni0.tame.shot_flag2 = 0;
            waza_ptr->w_int = 9;
            return;
        }
    } else if (waza_ptr->uni0.tame.shot_flag2 >= 5) {
        wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
        waza_ptr->uni0.tame.shot_flag2 = 0;
        waza_ptr->w_int = 9;
        chk_pl->waza_no = waza_type[cmd_id];
        return;
    }

    if (wcp[cmd_id].waza_flag[waza_type[cmd_id]]) {
        if (waza_ptr->w_int > 0 && waza_ptr->uni0.tame.shot_flag) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
            waza_ptr->uni0.tame.shot_flag = 0;
            waza_ptr->w_int = 12;
            return;
        }
    } else if (waza_ptr->uni0.tame.shot_flag >= 5) {
        wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
        waza_ptr->uni0.tame.shot_flag = 0;
        waza_ptr->w_int = 12;
        chk_pl->waza_no = waza_type[cmd_id];
        return;
    }

    if (wcp[cmd_id].waza_flag[waza_type[cmd_id]]) {
        if (waza_ptr->w_int > 0 && waza_ptr->uni0.tame.flag) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
            waza_ptr->uni0.tame.flag = 0;
            waza_ptr->w_int = 15;
        }
    } else if (waza_ptr->uni0.tame.flag >= 5) {
        wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
        waza_ptr->uni0.tame.flag = 0;
        waza_ptr->w_int = 15;
        chk_pl->waza_no = waza_type[cmd_id];
    }
}

void check_5() {
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    if (dead_lvr_check() == 0 && waza_ptr->w_lvr == chk_pl->sw_now) {
        if (*waza_ptr->w_ptr == 0x1C) {
            command_ok();
            return;
        }

        check_next();
    }
}

void check_6() {
    s16 i;
    u16 lvr_work;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        cmd_tbl_ptr += 12;
        waza_ptr->w_type = *cmd_tbl_ptr++;
        waza_ptr->w_int = *cmd_tbl_ptr++;
        waza_ptr->free2 = *cmd_tbl_ptr++;
        waza_ptr->w_lvr = *cmd_tbl_ptr++;
        waza_ptr->w_ptr = cmd_tbl_ptr;
        waza_ptr->uni0.tame.flag = 0;
        waza_ptr->uni0.tame.shot_flag = 0;
        waza_ptr->uni0.tame.shot_flag2 = 0;
        waza_ptr->free1 = 14;
        waza_ptr->shot_ok = 0;
    } else {
        waza_ptr->free1--;

        if (waza_ptr->free1 <= 0) {
            waza_ptr->free1 = 14;
            waza_ptr->shot_ok = 0;
        }
    }

    lvr_work = 1 & 0xFFFF;

    for (i = 0; i < 4; i++) {
        if (chk_pl->sw_lever == lvr_work) {
            waza_ptr->shot_ok |= (lvr_work);
            waza_ptr->free1 = 14;
        }

        lvr_work *= 2;
    }

    if (waza_ptr->shot_ok == 15) {
        if (*waza_ptr->w_ptr == 28) {
            command_ok();
            return;
        }

        waza_ptr->shot_ok = 0;
        check_next();
    }
}

void check_7() {
    s16 i;
    s16 w_flag;
    s16* shot_cnt_adrs;

    waza_ptr->w_int--;

    if (waza_ptr->w_type == 8) {
        sw_work = chk_pl->sw_new & 0x70;
        shot_cnt_adrs = &chk_pl->s1_cnt;
        w_flag = 0x10;
    } else {
        sw_work = chk_pl->sw_new & 0x780;
        shot_cnt_adrs = &chk_pl->s4_cnt;
        w_flag = 0x100;
    }

    waza_ptr->uni0.tame.shot_flag2 = waza_ptr->uni0.tame.shot_flag;
    waza_ptr->uni0.tame.shot_flag = 0;

    for (i = 0; i < 3; i++) {
        if (*shot_cnt_adrs & waza_ptr->w_lvr) {
            waza_ptr->uni0.tame.shot_flag |= w_flag;
        }

        shot_cnt_adrs++;

        if (chk_pl->shot_down & w_flag && waza_ptr->uni0.tame.shot_flag2 & w_flag) {
            waza_ptr->shot_ok += 1;
        }

        w_flag *= 2;
    }

    if (waza_ptr->shot_ok) {
        waza_ptr->free2--;

        if (waza_ptr->free2 < 0) {
            waza_ptr->shot_ok = 0;
            waza_ptr->free2 = waza_ptr->free1;
            waza_ptr->uni0.tame.shot_flag = 0;
        }
    }

    if (waza_ptr->shot_ok >= waza_ptr->w_lvr) {
        waza_ptr->shot_ok = 0;
        waza_ptr->free2 = waza_ptr->free1;

        if (*waza_ptr->w_ptr == 28) {
            command_ok();
            return;
        }

        check_next();
    }
}

void check_9() {
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    if (waza_ptr->w_lvr & 0x8000) {
        sw_work = waza_ptr->w_lvr & 0xF;

        if (waza_ptr->w_lvr == 0) {
            if (chk_pl->new_lvbt == 0) {
                if (*waza_ptr->w_ptr == 28) {
                    command_ok();
                    return;
                }

                check_next();
            }
        } else if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) {
            if (chk_pl->sw_lever == sw_work) {
                if (*waza_ptr->w_ptr == 0x1C) {
                    command_ok();
                    return;
                }

                check_next();
                return;
            }

            waza_ptr->w_type = 0;
        }
    } else if (waza_ptr->w_lvr == 0) {
        if (chk_pl->new_lvbt == 0) {
            if (*waza_ptr->w_ptr == 28) {
                command_ok();
                return;
            }

            check_next();
            return;
        }

        if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) {
            waza_ptr->w_type = 0;
        }
    } else if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) {
        if (chk_pl->sw_lever & waza_ptr->w_lvr) {
            if (*waza_ptr->w_ptr == 28) {
                command_ok();
                return;
            }

            check_next();
            return;
        }

        waza_ptr->w_type = 0;
    }
}

void paring_miss_init() {
    waza_ptr->free3 = 0;
    waza_ptr->w_type = 0;
    waza_ptr->uni0.tame.flag = 0;
    wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
}

void check_10() {
    switch (waza_ptr->shot_ok) {
    case 0:
        if (chk_pl->sw_lever == 0) {
            waza_ptr->shot_ok++;
        }
        break;

    case 1:
        if ((cmd_pl->wu.xyz[1].disp.pos > 0 || (waza_type[cmd_id] != 5 && waza_type[cmd_id] != 6)) &&
            chk_pl->now_lvbt & 0xF) {
            if (chk_pl->sw_lever == waza_ptr->w_lvr) {
                waza_ptr->shot_ok++;
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
                waza_ptr->free3 = wcp[cmd_id].reset[waza_type[cmd_id]] + 10;
                waza_ptr->w_int = 6;

                switch (waza_type[cmd_id]) {
                case 3:
                    if (wcp[cmd_id].waza_flag[3] > wcp[cmd_id].waza_flag[4]) {
                        wcp[cmd_id].waza_flag[4] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[3] > wcp[cmd_id].waza_flag[5]) {
                        wcp[cmd_id].waza_flag[5] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[3] > wcp[cmd_id].waza_flag[6]) {
                        wcp[cmd_id].waza_flag[6] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[3] > wcp[cmd_id].waza_flag[12]) {
                        wcp[cmd_id].waza_flag[12] = 0;
                    }

                    break;

                case 4:
                    if (wcp[cmd_id].waza_flag[4] > wcp[cmd_id].waza_flag[3]) {
                        wcp[cmd_id].waza_flag[3] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[4] > wcp[cmd_id].waza_flag[5]) {
                        wcp[cmd_id].waza_flag[5] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[4] > wcp[cmd_id].waza_flag[6]) {
                        wcp[cmd_id].waza_flag[6] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[4] > wcp[cmd_id].waza_flag[12]) {
                        wcp[cmd_id].waza_flag[12] = 0;
                    }

                    break;

                case 5:
                    if (wcp[cmd_id].waza_flag[5] > wcp[cmd_id].waza_flag[3]) {
                        wcp[cmd_id].waza_flag[3] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[5] > wcp[cmd_id].waza_flag[4]) {
                        wcp[cmd_id].waza_flag[4] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[5] > wcp[cmd_id].waza_flag[6]) {
                        wcp[cmd_id].waza_flag[6] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[5] > wcp[cmd_id].waza_flag[12]) {
                        wcp[cmd_id].waza_flag[12] = 0;
                    }

                    if (waza_work[cmd_id][6].free3 > 0) {
                        wcp[cmd_id].waza_flag[5] = 0;
                    }

                    break;

                case 6:
                    if (wcp[cmd_id].waza_flag[6] > wcp[cmd_id].waza_flag[3]) {
                        wcp[cmd_id].waza_flag[3] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[6] > wcp[cmd_id].waza_flag[4]) {
                        wcp[cmd_id].waza_flag[4] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[6] > wcp[cmd_id].waza_flag[5]) {
                        wcp[cmd_id].waza_flag[5] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[6] > wcp[cmd_id].waza_flag[12]) {
                        wcp[cmd_id].waza_flag[12] = 0;
                    }

                    if (waza_work[cmd_id][5].free3 > 0) {
                        wcp[cmd_id].waza_flag[6] = 0;
                    }

                    break;

                case 12:
                    if (wcp[cmd_id].waza_flag[12] > wcp[cmd_id].waza_flag[3]) {
                        wcp[cmd_id].waza_flag[3] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[12] > wcp[cmd_id].waza_flag[4]) {
                        wcp[cmd_id].waza_flag[4] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[12] > wcp[cmd_id].waza_flag[5]) {
                        wcp[cmd_id].waza_flag[5] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[12] > wcp[cmd_id].waza_flag[6]) {
                        wcp[cmd_id].waza_flag[6] = 0;
                    }

                    break;
                }
            } else {
                waza_ptr->shot_ok = 0;
                break;
            }
        }

        break;

    case 2:
        waza_ptr->w_int--;
        waza_ptr->free3--;

        if (waza_ptr->w_int > 0) {
            if (chk_pl->sw_lever == 0) {
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever & 8) {
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever != waza_ptr->w_lvr) {
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
                waza_ptr->shot_ok++;
                break;
            }
        } else {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
            waza_ptr->shot_ok++;
        }

        break;

    case 3:
        waza_ptr->free3--;

        if (waza_ptr->free3 < 0) {
            waza_ptr->w_type = 0;
            break;
        }

        if ((chk_pl->sw_now & 8) || !(chk_pl->sw_now != waza_ptr->w_lvr)) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
            break;
        }

        if (chk_pl->sw_now & 0xF) {
            waza_ptr->shot_ok++;
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
        }

        break;

    case 4:
        waza_ptr->free3--;

        if (waza_ptr->free3 < 0) {
            waza_ptr->w_type = 0;
        }

        break;
    }
}

void check_11() {
    if (dead_lvr_check()) {
        paring_miss_init();
        return;
    }

    switch (waza_ptr->uni0.tame.flag) {
    case 0:
        if (chk_pl->sw_lever & 8) {
            waza_ptr->uni0.tame.flag = 1;
            break;
        }

        waza_ptr->uni0.tame.flag = 0;
        break;

    case 1:
        if (chk_pl->sw_lever == 2) {
            check_next();
            break;
        }

        if (!(chk_pl->sw_lever & 8)) {
            waza_ptr->uni0.tame.flag = 0;
        }

        break;
    }
}

void check_12() {
    switch (waza_ptr->shot_ok) {
    case 0:
        if (chk_pl->sw_lever == 0) {
            waza_ptr->shot_ok++;
        }
        break;

    case 1:
        if (cmd_pl->wu.xyz[1].disp.pos > 0 && (chk_pl->now_lvbt & 0xF) != 0) {
            if (chk_pl->sw_lever == waza_ptr->w_lvr) {
                waza_ptr->shot_ok++;
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
                waza_ptr->free3 = wcp[cmd_id].reset[waza_type[cmd_id]] + 10;
                waza_ptr->w_int = 6;

                switch (waza_type[cmd_id]) {
                case 3:
                    if (wcp[cmd_id].waza_flag[3] > wcp[cmd_id].waza_flag[4]) {
                        wcp[cmd_id].waza_flag[4] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[3] > wcp[cmd_id].waza_flag[5]) {
                        wcp[cmd_id].waza_flag[5] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[3] > wcp[cmd_id].waza_flag[6]) {
                        wcp[cmd_id].waza_flag[6] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[3] > wcp[cmd_id].waza_flag[12]) {
                        wcp[cmd_id].waza_flag[12] = 0;
                    }

                    break;

                case 4:
                    if (wcp[cmd_id].waza_flag[4] > wcp[cmd_id].waza_flag[3]) {
                        wcp[cmd_id].waza_flag[3] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[4] > wcp[cmd_id].waza_flag[5]) {
                        wcp[cmd_id].waza_flag[5] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[4] > wcp[cmd_id].waza_flag[6]) {
                        wcp[cmd_id].waza_flag[6] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[4] > wcp[cmd_id].waza_flag[12]) {
                        wcp[cmd_id].waza_flag[12] = 0;
                    }

                    break;

                case 5:
                    if (wcp[cmd_id].waza_flag[5] > wcp[cmd_id].waza_flag[3]) {
                        wcp[cmd_id].waza_flag[3] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[5] > wcp[cmd_id].waza_flag[4]) {
                        wcp[cmd_id].waza_flag[4] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[5] > wcp[cmd_id].waza_flag[6]) {
                        wcp[cmd_id].waza_flag[6] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[5] > wcp[cmd_id].waza_flag[12]) {
                        wcp[cmd_id].waza_flag[12] = 0;
                    }

                    break;

                case 6:
                    if (wcp[cmd_id].waza_flag[6] > wcp[cmd_id].waza_flag[3]) {
                        wcp[cmd_id].waza_flag[3] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[6] > wcp[cmd_id].waza_flag[4]) {
                        wcp[cmd_id].waza_flag[4] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[6] > wcp[cmd_id].waza_flag[5]) {
                        wcp[cmd_id].waza_flag[5] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[6] > wcp[cmd_id].waza_flag[12]) {
                        wcp[cmd_id].waza_flag[12] = 0;
                    }

                    break;

                case 12:
                    if (wcp[cmd_id].waza_flag[12] > wcp[cmd_id].waza_flag[3]) {
                        wcp[cmd_id].waza_flag[3] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[12] > wcp[cmd_id].waza_flag[4]) {
                        wcp[cmd_id].waza_flag[4] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[12] > wcp[cmd_id].waza_flag[5]) {
                        wcp[cmd_id].waza_flag[5] = 0;
                    }

                    if (wcp[cmd_id].waza_flag[12] > wcp[cmd_id].waza_flag[6]) {
                        wcp[cmd_id].waza_flag[6] = 0;
                    }

                    break;
                }
            } else {
                waza_ptr->shot_ok = 0;
                break;
            }
        }

        break;

    case 2:
        waza_ptr->w_int--;
        waza_ptr->free3--;

        if (waza_ptr->w_int > 0) {
            if (chk_pl->sw_lever == 0) {
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever & 8) {
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever != waza_ptr->w_lvr) {
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
                waza_ptr->shot_ok++;
                break;
            }
        } else {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
            waza_ptr->shot_ok++;
        }

        break;

    case 3:
        waza_ptr->free3--;

        if (waza_ptr->free3 < 0) {
            waza_ptr->w_type = 0;
            break;
        }

        if ((chk_pl->sw_now & 8) || !(chk_pl->sw_now != waza_ptr->w_lvr)) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
            break;
        }

        if (chk_pl->sw_now & 0xF) {
            waza_ptr->shot_ok++;
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
        }

        break;

    case 4:
        waza_ptr->free3--;

        if (waza_ptr->free3 < 0) {
            waza_ptr->w_type = 0;
        }

        break;
    }
}

void check_13() {
    u16 sw_w;

    if (waza_ptr->free3 > 0) {
        waza_ptr->free3--;

        if (waza_ptr->free3 <= 0) {
            waza_ptr->w_type = 0;
        }
    }

    if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF) && (chk_pl->sw_lever) == 2) {
        wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0x10 - ukemi_time_tbl[wcp[cmd_id].waza_flag[waza_type[cmd_id]]];
        waza_ptr->free3 = 0x10;
        chk_pl->waza_no = waza_type[cmd_id];
    }

    sw_w = (chk_pl->sw_now | chk_pl->old_now) & 0x70;

    if (sw_w == 0x70) {
        wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0x10 - ukemi_time_tbl[wcp[cmd_id].waza_flag[waza_type[cmd_id]]];
        waza_ptr->free3 = 0x10;
        chk_pl->waza_no = waza_type[cmd_id];
    }
}

void check_14() {
    waza_ptr->w_int--;

    if (waza_ptr->w_lvr == 0x10) {
        if (chk_pl->sw_now & 0x70) {
            waza_ptr->uni0.tame.flag++;
        }
    } else if (chk_pl->sw_now & 0x700) {
        waza_ptr->uni0.tame.flag += 1;
    }

    if (wcp[cmd_id].waza_flag[waza_type[cmd_id]]) {
        if (waza_ptr->w_int <= 0) {
            if (waza_ptr->uni0.tame.flag) {
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
                waza_ptr->uni0.tame.flag = 0;

                if (waza_type[cmd_id] & 1) {
                    waza_ptr->w_int = 10;
                } else {
                    waza_ptr->w_int = 6;
                }
                return;
            }

            waza_ptr->uni0.tame.flag = 0;
            waza_ptr->w_int = waza_ptr->free1;
        }
    } else if (waza_type[cmd_id] & 1) {
        if (waza_ptr->uni0.tame.flag >= 3) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
            waza_ptr->uni0.tame.flag = 0;
            waza_ptr->w_int = 0xA;
            chk_pl->waza_no = waza_type[cmd_id];
            return;
        }

        if (waza_ptr->w_int < 0) {
            waza_ptr->uni0.tame.flag = 0;
            waza_ptr->w_int = waza_ptr->free1;
        }
    } else {
        if (waza_ptr->uni0.tame.flag >= 3) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
            waza_ptr->uni0.tame.flag = 0;
            waza_ptr->w_int = 6;
            chk_pl->waza_no = waza_type[cmd_id];
            return;
        }

        if (waza_ptr->w_int < 0) {
            waza_ptr->uni0.tame.flag = 0;
            waza_ptr->w_int = waza_ptr->free1;
        }
    }
}

void check_15() {
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
        return;
    }

    if (!dead_lvr_check()) {
        if (waza_ptr->w_lvr & 0x8000) {
            sw_work = waza_ptr->w_lvr & 0xF;

            if (chk_pl->sw_lever == sw_work) {
                waza_ptr->shot_ok++;

                if (waza_ptr->shot_ok >= waza_ptr->free1) {
                    if (*waza_ptr->w_ptr == 28) {
                        command_ok();
                        return;
                    }

                    check_next();
                }
            }
        } else if (waza_ptr->w_lvr == 0) {
            if (chk_pl->sw_lever == 0) {
                waza_ptr->shot_ok += 1;

                if (waza_ptr->shot_ok >= waza_ptr->free1) {
                    if (*waza_ptr->w_ptr == 28) {
                        command_ok();
                        return;
                    }

                    check_next();
                }
            }
        } else if (((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) && (chk_pl->sw_lever & waza_ptr->w_lvr) &&
                   (waza_ptr->shot_ok += 1, waza_ptr->shot_ok < waza_ptr->free1 == 0)) {
            if (*waza_ptr->w_ptr == 0x1C) {
                command_ok();
                return;
            }

            check_next();
        }
    }
}

void check_16() {
    s16 i;
    u16 w_flag;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
        waza_ptr->shot_ok = 0;
        return;
    }

    if (waza_ptr->w_type == 17) {
        sw_work = chk_pl->sw_now & 0x70;
        w_flag = 0x10;
    } else {
        sw_work = chk_pl->sw_now & 0x700;
        w_flag = 0x100;
    }

    waza_ptr->uni0.tame.shot_flag2 = waza_ptr->uni0.tame.shot_flag;
    waza_ptr->uni0.tame.shot_flag = 0;

    for (i = 0; i < 3; i++) {
        if (sw_work & w_flag) {
            waza_ptr->shot_ok++;
        }

        w_flag *= 2;
    }

    if (waza_ptr->shot_ok >= waza_ptr->w_lvr) {
        waza_ptr->shot_ok = 0;

        if (*waza_ptr->w_ptr == 0x1C) {
            command_ok();
            return;
        }

        check_next();
    }
}

void check_18() {
    u16 sw_lever;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
        return;
    }

    sw_lever = chk_pl->sw_lever & 0xF;

    if (!dead_lvr_check()) {
        if (waza_ptr->w_lvr & 0x8000) {
            if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) {
                sw_work = waza_ptr->w_lvr & 0xF;

                if (sw_lever == sw_work) {
                    waza_ptr->w_int = waza_ptr->free1;
                    wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
                }
            }
        } else if (waza_ptr->w_lvr == 0) {
            if (chk_pl->sw_lever == 0) {
                waza_ptr->w_int = waza_ptr->free1;
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
            }
        } else if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF) && (sw_lever & waza_ptr->w_lvr)) {
            waza_ptr->w_int = waza_ptr->free1;
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
        }
    }
}

void check_19() {
    u16 sw_lever;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    sw_lever = chk_pl->sw_lever & 0xF;

    if (!dead_lvr_check()) {
        if (waza_ptr->w_lvr & 0x8000) {
            if (chk_pl->now_lvbt & 0xF) {
                sw_work = waza_ptr->w_lvr & 0xF;
                if (sw_lever == sw_work) {
                    wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
                    check_next();
                }
            }
        } else if (waza_ptr->w_lvr == 0) {
            if (chk_pl->sw_lever == 0) {
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
                check_next();
            }
        } else if ((chk_pl->now_lvbt & 0xF) != 0 && (sw_lever & waza_ptr->w_lvr)) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
            check_next();
        }
    }
}

void check_20() {}

void check_21() {
    u16 sw_lever;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    sw_lever = chk_pl->sw_lever & 0xF;

    if (!dead_lvr_check()) {
        if (waza_ptr->w_lvr & 0x8000) {
            sw_work = waza_ptr->w_lvr & 0xF;
            if (sw_work == 0) {
                if (sw_lever == 0) {
                    if (((*waza_ptr->w_ptr)) == 0x1C) {
                        command_ok();
                        return;
                    }
                    check_next();
                }
            } else if (chk_pl->now_lvbt & 0xF) {
                if (sw_lever == sw_work) {
                    if (*waza_ptr->w_ptr == 28) {
                        command_ok();
                        return;
                    }
                    check_next();
                }
            }
        } else if (waza_ptr->w_lvr == 0) {
            if (sw_lever == 0) {
                if (*waza_ptr->w_ptr == 28) {
                    command_ok();
                    return;
                }

                check_next();
            }
        } else if ((chk_pl->now_lvbt & 0xF) && (sw_lever & waza_ptr->w_lvr)) {
            if (*waza_ptr->w_ptr == 28) {
                command_ok();
                return;
            }

            check_next();
        }
    }
}

void check_22() {
    s16 i;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_int = waza_ptr->free2;
        cmd_tbl_ptr += 12;
        waza_ptr->w_type = *cmd_tbl_ptr++;
        waza_ptr->w_int = *cmd_tbl_ptr++;
        waza_ptr->free1 = *cmd_tbl_ptr;
        waza_ptr->free2 = *cmd_tbl_ptr++;
        waza_ptr->w_lvr = *cmd_tbl_ptr++;
        waza_ptr->w_ptr = cmd_tbl_ptr;
        waza_ptr->uni0.tame.flag = 0;
        waza_ptr->uni0.tame.shot_flag = 0;
        waza_ptr->uni0.tame.shot_flag2 = 0;
        waza_ptr->shot_ok = 0;
        waza_ptr->free3 = 0;
    }

    for (i = 0; i < 8; i++) {
        if (chk_pl->sw_lever == chk22_tbl[i]) {
            waza_ptr->free3 |= 1 << i;
        }
    }

    if (waza_ptr->free3 == 0xFF) {
        if (((*waza_ptr->w_ptr)) == 0x1C) {
            command_ok();
            return;
        }

        waza_ptr->free3 = 0;
        check_next();
    }
}

void check_23() {
    switch (waza_ptr->shot_ok) {
    case 0:
        if (chk_pl->sw_lever == 0) {
            waza_ptr->shot_ok++;
            break;
        }

        break;

    case 1:
        if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF) && chk_pl->sw_lever == waza_ptr->w_lvr) {
            waza_ptr->shot_ok++;
            wcp[cmd_id].waza_flag[(waza_type[cmd_id])] = wcp[cmd_id].reset[(waza_type[cmd_id])];
            waza_ptr->free3 = (s16)(((((wcp[cmd_id].reset[(waza_type[cmd_id])])) + 3)));
            waza_ptr->w_int = 6;
        }

        break;

    case 2:
        waza_ptr->w_int -= 1;
        waza_ptr->free3 -= 1;

        if (((waza_ptr->w_int)) > 0) {
            if (chk_pl->sw_lever == 0) {
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever & 8) {
                wcp[cmd_id].waza_flag[(waza_type[cmd_id])] = 0;
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever != ((waza_ptr->w_lvr))) {
                wcp[cmd_id].waza_flag[(waza_type[cmd_id])] = 0;
                waza_ptr->w_type = 0;
                break;
            }
        } else {
            wcp[cmd_id].waza_flag[(waza_type[cmd_id])] = 0;
            waza_ptr->shot_ok++;
        }

        break;

    case 3:
        waza_ptr->free3--;

        if (waza_ptr->free3 < 0) {
            waza_ptr->w_type = 0;
            break;
        }

        if ((chk_pl->sw_now & 8) || !(chk_pl->sw_now != waza_ptr->w_lvr)) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
            break;
        }

        if (chk_pl->sw_now & 0xF) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
            waza_ptr->w_type = 0;
        }

        break;
    }
}

void check_24() {
    u16 sw_lever;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    sw_lever = chk_pl->now_lvbt & 0xF;

    if (!dead_lvr_check()) {
        if (waza_ptr->w_lvr & 0x8000) {
            sw_work = waza_ptr->w_lvr & 0xF;

            if (sw_lever == sw_work) {
                if (*waza_ptr->w_ptr == 28) {
                    command_ok();
                    return;
                }

                check_next();
            }
        } else if (waza_ptr->w_lvr == 0) {
            if (sw_lever == 0) {
                if (*waza_ptr->w_ptr == 28) {
                    command_ok();
                    return;
                }

                check_next();
            }
        } else {
            if (sw_lever & waza_ptr->w_lvr) {
                if (*waza_ptr->w_ptr == 28) {
                    command_ok();
                    return;
                }

                check_next();
            }
        }
    }
}

void check_25() {
    u16 sw_lever;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    sw_lever = chk_pl->sw_lever & 0xF;

    if (!dead_lvr_check()) {
        if (waza_ptr->w_lvr & 0x8000) {
            sw_work = waza_ptr->w_lvr & 0xF;

            if (sw_lever == sw_work) {
                if (*waza_ptr->w_ptr == 28) {
                    command_ok();
                    return;
                }

                check_next();
            }
        } else if (waza_ptr->w_lvr == 0) {
            if (sw_lever == 0) {
                if (*waza_ptr->w_ptr == 28) {
                    command_ok();
                    return;
                }

                check_next();
            }
        } else {
            if (sw_lever & waza_ptr->w_lvr) {
                if (*waza_ptr->w_ptr == 28) {
                    command_ok();
                    return;
                }

                check_next();
            }
        }
    }
}

void check_26() {
    u16 sw_lever = chk_pl->sw_now & 0xF;
    u16 sw_now_lvr = chk_pl->sw_lever & 0xF;

    if (!dead_lvr_check()) {
        sw_work = waza_ptr->w_lvr & 0xF;

        if (sw_lever != sw_work) {
            if (sw_now_lvr != sw_work && waza_ptr->uni0.tame.flag) {
                if (*waza_ptr->w_ptr == 28) {
                    command_ok();
                    return;
                }

                check_next();
            }
        } else {
            waza_ptr->uni0.tame.flag = 1;
        }
    }
}

void command_ok() {
    wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];

    if (waza_ptr->w_type != 14) {
        waza_ptr->w_type = 0;
        chk_pl->waza_no = waza_type[cmd_id];
    }
}

void command_ok_move(s16 waza_num) {
    if (dead_lvr_check()) {
        wcp[cmd_id].waza_flag[waza_num] = 0;
        return;
    }

    wcp[cmd_id].waza_flag[waza_num]--;
}

s32 dead_lvr_check() {
    if ((!waza_ptr->w_dead || waza_ptr->w_dead != chk_pl->sw_new) &&
        (!waza_ptr->w_dead2 || waza_ptr->w_dead2 != chk_pl->sw_new)) {
        return 0;
    }

    waza_ptr->w_type = 0;
    return 1;
}

void pl_lvr_set() {
    u16 sw_work;
    u16 work2;
    u16 sw_0;
    u16 sw_hana;
    u16 hana2;

    sw_0 = wcp[cmd_id].sw_lvbt;

    if (check_rl_on_car(cmd_pl)) {
        if (cmd_pl->wu.rl_flag) {
            sw_work = (sw_0 & 0xC);
            if (sw_work) {
                sw_0 &= 0xFF3;
                sw_work ^= 0xC;
                sw_0 |= sw_work;
            }
        }
    } else if (cmd_pl->wu.rl_waza) {
        sw_work = sw_0 & 0xC;

        if (sw_work) {
            sw_0 &= 0xFF3;
            sw_work ^= 0xC;
            sw_0 |= sw_work;
        }
    }

    wcp[cmd_id].old_now = chk_pl->sw_now;
    chk_pl->old_now = chk_pl->sw_now;
    chk_pl->old_lvbt = chk_pl->new_lvbt;

    sw_work = ~(chk_pl->old_lvbt) & (wcp[cmd_id].sw_lvbt);
    sw_hana = chk_pl->sw_new & ~(sw_0);

    work2 = sw_work & 0xF0;
    hana2 = sw_hana & 0xF0;

    switch (work2) {
    case 0x70:
    case 0x30:
    case 0x50:
    case 0x60:
        wcp[cmd_id].sw_lvbt |= 0x80;
        sw_0 |= 0x80;
        break;

    default:
        switch (hana2) {
        case 0x70:
        case 0x30:
        case 0x50:
        case 0x60:
            wcp[cmd_id].sw_lvbt |= 0x80;
            sw_0 |= 0x80;
            break;

        default:
            wcp[cmd_id].sw_lvbt &= 0xFF7F;
            sw_0 &= 0xFF7F;
            break;
        }
    }

    work2 = sw_work & 0xF00;
    hana2 = sw_hana & 0xF00;

    switch (work2) {
    case 0x700:
    case 0x300:
    case 0x500:
    case 0x600:
        wcp[cmd_id].sw_lvbt |= 0x800;
        sw_0 |= 0x800;
        break;

    default:
        switch (hana2) {
        case 0x700:
        case 0x300:
        case 0x500:
        case 0x600:
            wcp[cmd_id].sw_lvbt |= 0x800;
            sw_0 |= 0x800;
            break;

        default:
            wcp[cmd_id].sw_lvbt &= 0xF7FF;
            sw_0 &= 0xF7FF;
            break;
        }
    }

    chk_pl->new_lvbt = wcp[cmd_id].sw_lvbt;
    chk_pl->sw_old = chk_pl->sw_new;
    chk_pl->sw_new = sw_0;
    chk_pl->sw_now = sw_0 & ~(chk_pl->sw_old);
    chk_pl->now_lvbt = ~(chk_pl->old_lvbt) & (wcp[cmd_id].sw_lvbt);
    chk_pl->sw_chg = (chk_pl->sw_now) | (chk_pl->sw_old & ~(sw_0));
    chk_pl->sw_lever = sw_0 & 0xF;
    chk_pl->shot_up = chk_pl->sw_now & 0x770;
    chk_pl->shot_down = chk_pl->sw_old & ~(sw_0) & 0x770;
    chk_pl->shot_ud = ((chk_pl->shot_up) | (chk_pl->shot_down));
    sw_work = ((chk_pl->sw_now) | (wcp[cmd_id].old_now));

    if ((sw_work & 0x110) == 0x110) {
        wcp[cmd_id].ca14 = 1;
    } else {
        wcp[cmd_id].ca14 = 0;
    }

    if ((sw_work & 0x220) == 0x220) {
        wcp[cmd_id].ca25 = 1;
    } else {
        wcp[cmd_id].ca25 = 0;
    }
    if ((sw_work & 0x440) == 0x440) {
        wcp[cmd_id].ca36 = 1;
    } else {
        wcp[cmd_id].ca36 = 0;
    }

    wcp[cmd_id].lgp = lever_gacha_tbl[cmd_pl->cp->sw_now & 0xF] * 4;
    wcp[cmd_id].lgp += lever_gacha_tbl[cmd_pl->cp->sw_off & 0xF] * 2;
    wcp[cmd_id].lgp += lever_gacha_tbl[(cmd_pl->cp->sw_now / 16) & 7] * 2;
    wcp[cmd_id].lgp += lever_gacha_tbl[(cmd_pl->cp->sw_now / 256) & 7] * 1;
}

void sw_pick_up() {
    s16 i;
    s16* cnt_address1;

    pl_lvr_set();
    sw_work = 1;
    cnt_address1 = &chk_pl->up_cnt;

    for (i = 0; i < 10; i++) {
        if (chk_pl->sw_new & sw_work) {
            *cnt_address1 += 1;
        } else {
            *cnt_address1 = 0;
        }

        cnt_address1++;
        sw_work *= 2;
    }

    for (i = 0; i < 4; i++) {
        if (chk_pl->sw_new & lvr_chk_tbl[0][i]) {
            *cnt_address1 += 1;
        } else {
            *cnt_address1 = 0;
        }

        cnt_address1++;
    }

    wcp[cmd_id].sw_new = chk_pl->sw_new;
    wcp[cmd_id].sw_old = chk_pl->sw_old;
    wcp[cmd_id].sw_chg = chk_pl->sw_chg;
    wcp[cmd_id].sw_now = chk_pl->sw_now;
    wcp[cmd_id].sw_off = chk_pl->shot_down;

    if ((i = wcp[cmd_id].sw_lvbt & 0xC)) {
        if (cmd_pl->wu.rl_flag) {
            if (i & 8) {
                wcp[cmd_id].lever_dir = 1;
            } else {
                wcp[cmd_id].lever_dir = 2;
            }
        } else if (i & 4) {
            wcp[cmd_id].lever_dir = 1;
        } else {
            wcp[cmd_id].lever_dir = 2;
        }
    } else {
        wcp[cmd_id].lever_dir = 0;
    }

    if ((chk_pl->left_cnt != 0) && (chk_pl->left_cnt < 12)) {
        wcp[cmd_id].calf = 1;
    } else {
        wcp[cmd_id].calf = 0;
    }

    if ((chk_pl->right_cnt != 0) && (chk_pl->right_cnt < 12)) {
        wcp[cmd_id].calr = 1;
        return;
    }

    wcp[cmd_id].calr = 0;
}

void dash_flag_clear(s16 pl_id) {
    intptr_t* adrs;

    if (cmd_sel[pl_id]) {
        adrs = pl_CMD[gs.plw[pl_id].player_number];
    } else {
        adrs = pl_cmd[gs.plw[pl_id].player_number];
    }

    waza_compel_init(pl_id, 0, adrs);
    waza_compel_init(pl_id, 1, adrs);
}

void hi_jump_flag_clear(s16 pl_id) {
    intptr_t* adrs;

    if (cmd_sel[pl_id]) {
        adrs = pl_CMD[gs.plw[pl_id].player_number];
    } else {
        adrs = pl_cmd[gs.plw[pl_id].player_number];
    }

    waza_compel_init(pl_id, 2, adrs);
}

void waza_flag_clear_only_1(s16 pl_id, s16 wznum) {
    intptr_t* adrs;

    if (cmd_sel[pl_id]) {
        adrs = pl_CMD[gs.plw[pl_id].player_number];
    } else {
        adrs = pl_cmd[gs.plw[pl_id].player_number];
    }

    waza_compel_init(pl_id, wznum, adrs);
}

void waza_compel_init(s16 pl_id, s16 num, intptr_t* adrs) {
    WAZA_WORK* w_ptr;
    s16* ptr;

    ptr = (s16*)adrs[num];
    ptr += 12;
    w_ptr = &waza_work[pl_id][num];
    w_ptr->w_type = *ptr++;
    w_ptr->w_int = *ptr++;
    w_ptr->free1 = *ptr;
    w_ptr->free2 = *ptr++;
    w_ptr->w_lvr = *ptr++;
    w_ptr->w_ptr = ptr;
    w_ptr->uni0.tame.flag = 0;
    w_ptr->uni0.tame.shot_flag = 0;
    w_ptr->uni0.tame.shot_flag2 = 0;
    w_ptr->shot_ok = 0;
    w_ptr->free3 = 0;
    wcp[pl_id].waza_flag[num] = 0;
}

void waza_compel_all_init(PLW* pl) {
    s16 i;
    intptr_t* adrs;

    if (cmd_sel[pl->wu.id]) {
        adrs = pl_CMD[pl->player_number];
    } else {
        adrs = pl_cmd[pl->player_number];
    }

    for (i = 0; i < pl_cmd_num[pl->player_number][0]; i++) {
        cmd_tbl_ptr = (s16*)adrs[i];
        cmd_data_set(pl, i);
    }

    for (i = pl_cmd_num[pl->player_number][0]; i < 20; i++) {
        wcp[cmd_id].waza_flag[i] = -1;
    }

    for (i = 20; i < pl_cmd_num[pl->player_number][1]; i++) {
        cmd_tbl_ptr = (s16*)adrs[i];
        cmd_data_set(pl, i);
    }

    for (i = pl_cmd_num[pl->player_number][1]; i < 24; i++) {
        wcp[cmd_id].waza_flag[i] = -1;
    }

    for (i = 24; i < pl_cmd_num[pl->player_number][2]; i++) {
        cmd_tbl_ptr = (s16*)adrs[i];
        cmd_data_set(pl, i);
    }

    for (i = pl_cmd_num[pl->player_number][2]; i < 28; i++) {
        wcp[cmd_id].waza_flag[i] = -1;
    }

    for (i = 28; i < pl_cmd_num[pl->player_number][3]; i++) {
        cmd_tbl_ptr = (s16*)adrs[i];
        cmd_data_set(pl, i);
    }

    for (i = pl_cmd_num[pl->player_number][3]; i < 38; i++) {
        wcp[cmd_id].waza_flag[i] = -1;
    }

    for (i = 38; i < pl_cmd_num[pl->player_number][4]; i++) {
        cmd_tbl_ptr = (s16*)adrs[i];
        cmd_data_set(pl, i);
    }

    for (i = pl_cmd_num[pl->player_number][4]; i < 42; i++) {
        wcp[cmd_id].waza_flag[i] = -1;
    }

    for (i = 42; i < pl_cmd_num[pl->player_number][5]; i++) {
        cmd_tbl_ptr = (s16*)adrs[i];
        cmd_data_set(pl, i);
    }

    for (i = pl_cmd_num[pl->player_number][5]; i < 46; i++) {
        wcp[cmd_id].waza_flag[i] = -1;
    }

    for (i = 46; i < pl_cmd_num[pl->player_number][6]; i++) {
        cmd_tbl_ptr = (s16*)adrs[i];
        cmd_data_set(pl, i);
    }

    for (i = pl_cmd_num[pl->player_number][6]; i < 56; i++) {
        wcp[cmd_id].waza_flag[i] = -1;
    }
}

void waza_compel_all_init2(PLW* pl) {
    s16 j;

    for (j = 0; j < 56; j++) {
        if (wcp[pl->wu.id].waza_flag[j] != -1) {
            waza_work[pl->wu.id][j].w_type = 0;
        }
    }
}

u16 processed_lvbt(u16 lv_data) {
    return lv_data & 0xFFF;
}
