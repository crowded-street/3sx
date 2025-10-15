/**
 * @file eff45.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff45.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/eff61.h"
#include "sf33rd/Source/Game/effect/effb6.h"
#include "sf33rd/Source/Game/effect/effect.h"

void EFF45_DISP(WORK_Other_CONN* ewk);
void EFF45_SUSPEND(WORK_Other_CONN* ewk);
void EFF45_CHANGE(WORK_Other_CONN* ewk);
void Setup_Message(WORK_Other_CONN* ewk);
s16 Centering_Sub(WORK_Other_CONN* ewk, s16 dot_type);
void Check_Pig_Pig(WORK_Other_CONN* ewk);
void Convert_16_10_2(WORK_Other_CONN* ewk, u16 target);

// sbss
MessageData Message_Data[4];

void (*const EFF45_Jmp_Tbl[3])() = { EFF45_DISP, EFF45_CHANGE, EFF45_SUSPEND };

void effect_45_move(WORK_Other_CONN* ewk) {
    Check_Pos_OBJ2((WORK_Other*)ewk);

    if (Check_Die_61((WORK_Other*)ewk) != 0 || Message_Data[ewk->wu.dir_old].order == 3) {
        push_effect_work(&ewk->wu);
        return;
    }

    if (ewk->wu.routine_no[0] != Message_Data[ewk->wu.dir_old].order) {
        ewk->wu.routine_no[0] = Message_Data[ewk->wu.dir_old].order;
        ewk->wu.routine_no[1] = 0;
    }

    EFF45_Jmp_Tbl[ewk->wu.routine_no[0]](ewk);

    if (ewk->wu.be_flag != 0) {
        ewk->wu.dir_step = Message_Data[ewk->wu.dir_old].order;
        sort_push_request3(&ewk->wu);
    }
}

void EFF45_DISP(WORK_Other_CONN* ewk) {
    ewk->wu.disp_flag = 1;

    if ((ewk->wu.routine_no[0] = Message_Data[ewk->wu.dir_old].order)) {
        ewk->wu.routine_no[1] = 0;
    }
}

void EFF45_SUSPEND(WORK_Other_CONN* ewk) {
    ewk->wu.disp_flag = 0;

    if (Message_Data[ewk->wu.dir_old].order != 2) {
        ewk->wu.routine_no[0] = Message_Data[ewk->wu.dir_old].order;
        ewk->wu.routine_no[1] = 0;
    } else {
        Message_Data[ewk->wu.dir_old].request = 0xFF;
        Message_Data[ewk->wu.dir_old].contents = 0xFF;
    }
}

void EFF45_CHANGE(WORK_Other_CONN* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 0;
        break;

    case 1:
        if (--Message_Data[ewk->wu.dir_old].timer == 0) {
            ewk->wu.disp_flag = 1;
            ewk->wu.routine_no[0] = 0;
            Setup_Message(ewk);
            Message_Data[ewk->wu.dir_old].order = 0;
        }

        break;
    }
}

s32 effect_45_init(u8 id, s16 sync_bg, s16 master_player) {
    WORK_Other_CONN* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.routine_no[0] = 0;
    ewk->wu.be_flag = 1;
    ewk->wu.disp_flag = 0;
    ewk->wu.id = 45;
    ewk->wu.work_id = 16;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_code = 0x1AC;
    ewk->wu.my_family = sync_bg + 1;
    ewk->master_player = master_player;
    ewk->wu.dir_old = id;
    Message_Data[ewk->wu.dir_old].contents = 0xFF;
    ewk->wu.routine_no[0] = Message_Data[ewk->wu.dir_old].order;
    ewk->wu.my_mts = 12;
    Setup_Message(ewk);
    return 0;
}

void Setup_Message(WORK_Other_CONN* ewk) {
    Message_Data[ewk->wu.dir_old].contents = Message_Data[ewk->wu.dir_old].request;
    Message_Data[ewk->wu.dir_old].kind_cnt = Message_Data[ewk->wu.dir_old].kind_req;

    if (Debug_w[68]) {
        Message_Data[ewk->wu.dir_old].request = Debug_w[68] - 1;
        Message_Data[ewk->wu.dir_old].contents = Debug_w[68] - 1;
    }

    get_message_conn_data(ewk, Message_Data[ewk->wu.dir_old].kind_cnt, 0, Message_Data[ewk->wu.dir_old].request + 0);
    ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Message_Data[ewk->wu.dir_old].pos_x;
    ewk->wu.position_x -= Centering_Sub(ewk, 16);
    ewk->wu.position_y = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Message_Data[ewk->wu.dir_old].pos_y;
    ewk->wu.position_z = Message_Data[ewk->wu.dir_old].pos_z;
    Check_Pig_Pig(ewk);
}

s16 Centering_Sub(WORK_Other_CONN* ewk, s16 dot_type) {
    s16 i;
    s16 ix;
    s16 ny;
    s16 len;
    s16 max;

    dot_type /= 2;
    max = dot_type;
    ix = 0;
    ny = ewk->conn[0].ny;

    for (i = 0; i < ewk->num_of_conn; i++) {
        if (ny != ewk->conn[i].ny) {
            ny = ewk->conn[i].ny;
            ix = i;
        }

        len = ewk->conn[i].nx - ewk->conn[ix].nx;

        if (max < len + dot_type) {
            max = len + dot_type;
        }
    }

    return max / 2;
}

void Check_Pig_Pig(WORK_Other_CONN* ewk) {
    s16 ix;

    if (Debug_w[68]) {
        Convert_16_10_2(ewk, vm_w.Block_Size);

        for (ix = 0; ix < ewk->num_of_conn; ix++) {
            if (ewk->conn[ix].chr != 0x8020) {
                continue;
            }

            ewk->conn[ix].chr = ewk->wu.old_rno[1] + 0x80C3;
            ewk->conn[ix].col = 0x17;

            if (ewk->wu.old_rno[1] == 0) {
                ewk->conn[ix].ny -= 0x100;
            }

            ewk->conn[ix + 1].chr = ewk->wu.old_rno[0] + 0x80C3;
            ewk->conn[ix + 1].col = 0x17;
            return;
        }
    }

    switch (Message_Data[ewk->wu.dir_old].request) {
    case 22:
        for (ix = 0; ix < ewk->num_of_conn; ix++) {
            if (ewk->conn[ix].chr != 0x8020) {
                continue;
            }

            ewk->conn[ix].chr = vm_w.Block_Size + 0x7F81;
            ewk->conn[ix].col = 0x17;
            return;
        }

        break;

    case 23:
    case 25:
        Convert_16_10_2(ewk, vm_w.Block_Size);

        for (ix = 0; ix < ewk->num_of_conn; ix++) {
            if (ewk->conn[ix].chr != 0x8020) {
                continue;
            }

            ewk->conn[ix].chr = ewk->wu.old_rno[1] + 0x7F81;
            ewk->conn[ix].col = 0x17;

            if (ewk->wu.old_rno[1] == 0) {
                ewk->conn[ix].ny -= 0x100;
            }

            ewk->conn[(ix) + 1].chr = ewk->wu.old_rno[0] + 0x7F81;
            ewk->conn[(ix) + 1].col = 0x17;
            return;
        }

        break;

    case 24:
        for (ix = 0; ix < ewk->num_of_conn; ix++) {
            if (ewk->conn[ix].chr != 0x8020) {
                continue;
            }

            ewk->conn[ix].chr = ewk->wu.old_rno[2] + 0x7F81;
            ewk->conn[ix].col = 0x17;

            if (ewk->wu.old_rno[2] == 0) {
                ewk->conn[ix].ny -= 0x100;
            }

            ewk->conn[ix + 1].chr = ewk->wu.old_rno[1] + 0x7F81;
            ewk->conn[ix + 1].col = 0x17;

            if (ewk->wu.old_rno[1] == 0 && ewk->wu.old_rno[2] == 0) {
                ewk->conn[ix + 1].ny -= 0x100;
            }

            ewk->conn[ix + 2].chr = ewk->wu.old_rno[0] + 0x7F81;
            ewk->conn[ix + 2].col = 0x17;
            return;
        }

        break;
    }
}

void Convert_16_10_2(WORK_Other_CONN* ewk, u16 target) {
    ewk->wu.old_rno[1] = target / 10;
    target %= 10;
    ewk->wu.old_rno[0] = target;
}

void Convert_16_10_3(WORK_Other_CONN* ewk, u16 target) {
    ewk->wu.old_rno[2] = target / 100;
    target %= 100;
    ewk->wu.old_rno[1] = target / 10;
    target %= 10;
    ewk->wu.old_rno[0] = target;
}
