/**
 * @file eff79.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff79.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff80.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/screen/sel_data.h"
#include "sf33rd/Source/Game/stage/bg.h"

s32 Check_Play_Status_79(WORK_Other* ewk);
void Move_Move_79(WORK_Other* ewk);
void Setup_Move_79(WORK_Other* ewk, s32 /* unused */, s32 X_Value, s32 Y_Value, s32 Option);
void Move_79(WORK_Other* ewk);
void Check_Priority(WORK_Other* ewk);
s32 EFF79_Move_X(WORK_Other* ewk);
s32 EFF79_Move_Y(WORK_Other* ewk);
void Setup_Command_Name(WORK_Other* ewk);
s32 Select_End_Sub_79(WORK_Other* ewk);
void Setup_Pos_79(WORK_Other* ewk);
void Check_Speed_79(WORK_Other* ewk);
s32 Move_X_Sub(WORK_Other* ewk, s16 Target_X, s16 cut);
s32 Check_Depth_to_Before(WORK_Other* ewk);
s32 Move_Y_Sub(WORK_Other* ewk, s16 Target_Y, u16 cut);

u8 OK_Appear79[2];
u8 Extra_Counter[2];

const s32 Pos_Z_Data_79[3] = { 0, 5, 10 };

void effect_79_move(WORK_Other* ewk) {
    s16 xx;
    s16 arrived[2];

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (--ewk->wu.dir_timer) {
            return;
        }

        ewk->wu.disp_flag = 1;
        ewk->wu.routine_no[0]++;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 1) {
            OK_Appear79[ewk->master_id] = 1;
            ewk->wu.routine_no[0]++;
            ewk->wu.dir_timer = 1;
        }

        break;

    case 2:
        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.routine_no[0] = 5;
            Move_Super_Arts[ewk->master_id]--;
            Select_Arts[ewk->master_id]--;
            effect_80_init(ewk, ewk->master_id, ewk->master_player, ewk->wu.my_family - 1);
        }

        break;

    case 3:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            if (OK_Appear79[ewk->master_id]) {
                ewk->wu.routine_no[1]++;
                ewk->wu.routine_no[5] = 0;
                ewk->wu.routine_no[6] = 1;
                ewk->wu.disp_flag = 1;
                set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
            }

            break;

        case 1:
            arrived[0] = EFF79_Move_X(ewk);
            arrived[1] = EFF79_Move_Y(ewk);

            if (arrived[0] == 0 || arrived[1] == 0) {
                break;
            }

            ewk->wu.routine_no[0]++;
            ewk->wu.routine_no[1] = 0;
            ewk->wu.routine_no[5] = 0;
            ewk->wu.routine_no[6] = 0;
            ewk->wu.dir_timer = 1;
            break;
        }

        break;

    case 4:
        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.routine_no[0]++;
            Move_Super_Arts[ewk->master_id]--;
            Select_Arts[ewk->master_id]--;
            effect_80_init(ewk, ewk->master_id, ewk->master_player, ewk->wu.my_family - 1);
        }

        break;

    case 5:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            Plate_Disposal_No[ewk->master_id][ewk->master_player] = 0;

            if (Check_Play_Status_79(ewk)) {
                break;
            }

            switch (Moving_Plate[ewk->master_id]) {
            case 1:
                switch (ewk->wu.hit_quake) {
                case 0:
                    ewk->wu.dmcal_m = 1;
                    ewk->wu.dm_vital = 3;
                    Setup_Move_79(ewk, 2, 0x60000, -0x20000, 0);
                    break;

                case 1:
                    ewk->wu.dmcal_m = 2;
                    ewk->wu.dm_vital = 1;
                    Setup_Move_79(ewk, 2, 0x60000, -0x20000, 0);
                    break;

                default:
                    ewk->wu.dmcal_m = 0;
                    ewk->wu.dm_vital = 2;
                    Setup_Move_79(ewk, 2, 0x20000, -0x80000, 2);
                    break;
                }

                break;

            case 2:
                switch (ewk->wu.hit_quake) {
                case 0:
                    ewk->wu.dmcal_m = 2;
                    ewk->wu.dm_vital = 3;
                    Setup_Move_79(ewk, 2, 0x60000, -0x80000, 1);
                    break;

                case 1:
                    ewk->wu.dmcal_m = 0;
                    ewk->wu.dm_vital = 1;
                    Setup_Move_79(ewk, 1, -0x60000, 0x20000, 0);
                    break;

                default:
                    ewk->wu.dmcal_m = 1;
                    ewk->wu.dm_vital = 2;
                    Setup_Move_79(ewk, 1, -0x60000, 0x20000, 0);
                    break;
                }

                break;

            default:
                Select_End_Sub_79(ewk);
                break;
            }

            break;

        case 1:
            Move_79(ewk);
            break;

        case 2:
            Move_Move_79(ewk);
            break;
        }

        break;

    case 6:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            if (--ewk->wu.dir_timer != 0) {
                break;
            }

            ewk->wu.routine_no[1]++;
            ewk->wu.routine_no[5] = 0;
            ewk->wu.routine_no[6] = 1;
            ewk->wu.vital_new = Plate_X[ewk->master_id][0];
            ewk->wu.direction = Plate_Y[ewk->master_id][0];
            ewk->wu.mvxy.a[1].sp = 0x20000;
            ewk->wu.mvxy.d[1].sp = 0x2000;

            if (ewk->wu.xyz[0].disp.pos < ewk->wu.vital_new) {
                ewk->wu.mvxy.a[0].sp = 0x18000;
                ewk->wu.mvxy.d[0].sp = 0x6000;
            } else {
                ewk->wu.mvxy.a[0].sp = -0x18000;
                ewk->wu.mvxy.d[0].sp = -0x6000;
            }

            Check_Speed_79(ewk);
            break;

        case 1:
            arrived[0] = EFF79_Move_X(ewk);
            arrived[1] = EFF79_Move_Y(ewk);

            if (arrived[0] == 0 || arrived[1] == 0) {
                break;
            }

            ewk->wu.routine_no[0] = 10;
            ewk->wu.disp_flag = 0;
            Extra_Counter[ewk->master_id]--;
            return;

        default:
            break;
        }

        break;

    case 7:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            if (--ewk->wu.dir_timer == 0) {
                ewk->wu.routine_no[1]++;
            }

            break;

        case 1:
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 2) {
                ewk->wu.routine_no[1]++;
                Sel_Arts_Complete[ewk->master_id] |= 0x8000;

                if (ewk->wu.xyz[0].disp.pos ==
                    bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Plate_Pos_Data_79[1][ewk->master_id][0][0]) {
                    ewk->wu.routine_no[0] = 8;
                }
            }

            break;

        case 2:
            if (Extra_Counter[ewk->master_id] != 0) {
                break;
            }

            ewk->wu.routine_no[1]++;
            ewk->wu.routine_no[5] = 0;
            ewk->wu.routine_no[6] = 1;

            if (VS_Index[ewk->master_id] < 9) {
                xx = 0;
            } else {
                xx = 2;
            }

            ewk->wu.vital_new = Plate_Finish_Data_79[ewk->master_id + xx][0];
            ewk->wu.direction = Plate_Finish_Data_79[ewk->master_id + xx][1];
            ewk->wu.mvxy.a[1].sp = -0x8000;
            ewk->wu.mvxy.d[1].sp = -0xE000;

            if (ewk->wu.xyz[0].disp.pos < ewk->wu.vital_new) {
                ewk->wu.mvxy.a[0].sp = 0x10000;
                ewk->wu.mvxy.d[0].sp = 0x30000;
            } else {
                ewk->wu.mvxy.a[0].sp = -0x10000;
                ewk->wu.mvxy.d[0].sp = -0x30000;
            }

            break;

        case 3:
            arrived[0] = EFF79_Move_X(ewk);
            arrived[1] = EFF79_Move_Y(ewk);

            if (arrived[0] != 0 && arrived[1] != 0) {
                ewk->wu.routine_no[0]++;
            }

            break;
        }

        break;

    case 8:
        break;

    case 9:
        if (!Suicide[0]) {
            break;
        }

        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0] = 10;
        return;

    case 10:
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0]++;
        return;

    default:
        ewk->wu.disp_flag = 0;
        push_effect_work(&ewk->wu);
        return;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    ewk->wu.position_z = ewk->wu.xyz[2].disp.pos;
    PP_Priority[ewk->master_id][ewk->master_player] = ewk->wu.dmcal_m;
    sort_push_request4(&ewk->wu);
}

s32 Check_Play_Status_79(WORK_Other* ewk) {
    if (ewk->wu.dir_old == 0 && Play_Type == 1) {
        ewk->wu.routine_no[1] = 2;
        ewk->wu.routine_no[2] = 0;
        ewk->wu.routine_no[5] = 0;
        ewk->wu.routine_no[6] = 1;
        ewk->wu.routine_no[7] = 0;
        Plate_Disposal_No[ewk->master_id][ewk->master_player] = 1;
        ewk->wu.dir_timer = 1;
        ewk->wu.vital_new = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos +
                            Plate_Pos_Data_79[Play_Type][ewk->master_id][ewk->wu.dmcal_m][0];
        ewk->wu.direction = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos +
                            Plate_Pos_Data_79[Play_Type][ewk->master_id][ewk->wu.dmcal_m][1];
        OK_Moving_SA_Plate[ewk->master_id] = 3;
        ewk->wu.mvxy.a[1].sp = -0x8000;
        ewk->wu.mvxy.d[1].sp = -0xE000;

        if (ewk->wu.xyz[0].disp.pos < ewk->wu.vital_new) {
            ewk->wu.mvxy.a[0].sp = 0x10000;
            ewk->wu.mvxy.d[0].sp = 0x30000;
            Moving_Plate[ewk->master_id] = 8;
        } else {
            ewk->wu.mvxy.a[0].sp = -0x10000;
            ewk->wu.mvxy.d[0].sp = -0x30000;
            Moving_Plate[ewk->master_id] = 4;
        }

        return ewk->wu.dir_old = 99;
    }

    return 0;
}

void Move_Move_79(WORK_Other* ewk) {
    s16 arrived[2];

    switch (ewk->wu.routine_no[2]) {
    case 0:
        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.routine_no[2]++;
        }

        break;

    case 1:
        arrived[0] = EFF79_Move_X(ewk);
        arrived[1] = EFF79_Move_Y(ewk);

        if (arrived[0] != 0 && arrived[1] != 0) {
            ewk->wu.routine_no[2]++;
            OK_Moving_SA_Plate[ewk->master_id]--;
        }

        break;

    case 2:
        if (OK_Moving_SA_Plate[ewk->master_id] == 0) {
            ewk->wu.routine_no[2]++;
        }

        break;

    case 3:
        ewk->wu.routine_no[1] = 0;
        ewk->wu.routine_no[2] = 0;
        Moving_Plate[ewk->master_id] = 0;
        break;
    }

    if (ewk->wu.hit_quake == 0) {
        Plate_X[ewk->master_id][0] = ewk->wu.xyz[0].disp.pos;
        Plate_Y[ewk->master_id][0] = ewk->wu.xyz[1].disp.pos;
    }
}

void Setup_Move_79(WORK_Other* ewk, s32 /* unused */, s32 X_Value, s32 Y_Value, s32 Option) {
    ewk->wu.routine_no[1] = 1;
    ewk->wu.routine_no[2] = 0;
    ewk->wu.routine_no[5] = 0;
    ewk->wu.routine_no[6] = 1;
    ewk->wu.routine_no[7] = 0;
    ewk->wu.dir_timer = 1;
    ewk->wu.vital_new = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos +
                        Plate_Pos_Data_79[Play_Type][ewk->master_id][ewk->wu.dmcal_m][0];
    ewk->wu.direction = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos +
                        Plate_Pos_Data_79[Play_Type][ewk->master_id][ewk->wu.dmcal_m][1];
    ewk->wu.mvxy.a[1].sp = Y_Value;
    ewk->wu.mvxy.d[1].sp = 0;
    Plate_Disposal_No[ewk->master_id][ewk->master_player] = 1;
    OK_Priority[ewk->master_id] = 0;
    ewk->wu.mvxy.a[0].sp = X_Value;

    if (!Option) {
        return;
    }

    ewk->wu.routine_no[6] = 0;

    if (0 < ewk->wu.mvxy.a[1].sp) {
        ewk->wu.vital_old = ewk->wu.xyz[1].disp.pos + 48;
    } else if (ewk->wu.dmcal_m == 0) {
        ewk->wu.vital_old = ewk->wu.xyz[1].disp.pos - 40;
    } else {
        ewk->wu.vital_old = ewk->wu.xyz[1].disp.pos - 48;
    }
}

void Move_79(WORK_Other* ewk) {
    s16 arrived[2];

    Check_Priority(ewk);

    switch (ewk->wu.routine_no[2]) {
    case 0:
        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.routine_no[2]++;
        }

        break;

    case 1:
        arrived[0] = EFF79_Move_X(ewk);
        arrived[1] = EFF79_Move_Y(ewk);

        if (arrived[0] != 0 && arrived[1] != 0) {
            ewk->wu.routine_no[2]++;
            ewk->wu.hit_quake = ewk->wu.dmcal_m;
            Moving_Plate_Counter[ewk->master_id]--;
            ewk->wu.xyz[2].disp.pos = Pos_Z_Data_79[ewk->wu.hit_quake] + 35;
        }

        break;

    case 2:
        if (Moving_Plate_Counter[ewk->master_id] == 0) {
            ewk->wu.routine_no[2]++;
        }

        break;

    case 3:
        ewk->wu.routine_no[1] = 0;
        ewk->wu.routine_no[2] = 0;
        Moving_Plate[ewk->master_id] = 0;
        break;
    }
}

void Check_Priority(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[7]) {
    case 0:
        if (OK_Priority[ewk->master_id] == 0) {
            break;
        }

        ewk->wu.routine_no[7]++;
        /* fallthrough */

    case 1:
        if (--ewk->wu.dm_vital == 0) {
            ewk->wu.routine_no[7]++;
            ewk->wu.xyz[2].disp.pos = Pos_Z_Data_79[ewk->wu.dmcal_m] + 35;
        }

        break;
    }
}

s32 EFF79_Move_X(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[5]) {
    case 0:
        if (!Move_X_Sub(ewk, ewk->wu.vital_new, 1)) {
            break;
        }

        ewk->wu.routine_no[5]++;
        return 1;

    case 1:
        return 1;

    case 2:
        if (!Move_X_Sub(ewk, ewk->wu.vital_new, 1)) {
            break;
        }

        ewk->wu.routine_no[5]++;
        return 1;

    default:
        return 1;
    }

    return 0;
}

s32 EFF79_Move_Y(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[6]) {
    case 0:
        ewk->wu.xyz[1].cal += ewk->wu.mvxy.a[1].sp;
        ewk->wu.mvxy.a[1].sp += ewk->wu.mvxy.d[1].sp;

        if (0 > ewk->wu.mvxy.a[1].sp) {
            if (ewk->wu.vital_old >= ewk->wu.xyz[1].disp.pos) {
                OK_Priority[ewk->master_id] = 1;
                ewk->wu.routine_no[5] = 2;
                ewk->wu.routine_no[6]++;
                ewk->wu.routine_no[7] = 99;
                ewk->wu.mvxy.a[1].sp = -ewk->wu.mvxy.a[1].sp;
                ewk->wu.mvxy.d[1].sp = -ewk->wu.mvxy.d[1].sp;

                if (ewk->wu.dmcal_m == 0) {
                    ewk->wu.mvxy.a[0].sp = -0x60000;
                    ewk->wu.mvxy.d[0].sp = -ewk->wu.mvxy.d[0].sp;
                }

                ewk->wu.xyz[2].disp.pos = Pos_Z_Data_79[ewk->wu.dmcal_m] + 35;
                Setup_Command_Name(ewk);
            }
        } else if (ewk->wu.vital_old <= ewk->wu.xyz[1].disp.pos) {
            OK_Priority[ewk->master_id] = 1;
            ewk->wu.routine_no[5] = 2;
            ewk->wu.routine_no[6]++;
            ewk->wu.routine_no[7] = 99;
            ewk->wu.mvxy.a[1].sp = -ewk->wu.mvxy.a[1].sp;
            ewk->wu.mvxy.d[1].sp = -ewk->wu.mvxy.d[1].sp;

            if (ewk->wu.dmcal_m == 0) {
                ewk->wu.mvxy.a[0].sp = -ewk->wu.mvxy.a[0].sp;
                ewk->wu.mvxy.d[0].sp = -ewk->wu.mvxy.d[0].sp;
            }

            ewk->wu.xyz[2].disp.pos = Pos_Z_Data_79[ewk->wu.dmcal_m] + 35;
            Setup_Command_Name(ewk);
        }

        break;

    case 1:
        if (!Move_Y_Sub(ewk, ewk->wu.direction, 1)) {
            break;
        }

        ewk->wu.routine_no[6] = 99;
        return 1;

    default:
        return 1;
    }

    return 0;
}

void Setup_Command_Name(WORK_Other* ewk) {
    if (Moving_Plate[ewk->master_id] == 2) {
        Disp_Command_Name[ewk->master_id][ewk->master_player] = 0;

        switch (ewk->master_player) {
        case 0:
            Disp_Command_Name[ewk->master_id][1] = 1;
            Disp_Command_Name[ewk->master_id][2] = 0;
            break;

        case 1:
            Disp_Command_Name[ewk->master_id][2] = 1;
            Disp_Command_Name[ewk->master_id][0] = 0;
            break;

        default:
            Disp_Command_Name[ewk->master_id][0] = 1;
            Disp_Command_Name[ewk->master_id][1] = 0;
            break;
        }
    } else {
        Disp_Command_Name[ewk->master_id][0] = 0;
        Disp_Command_Name[ewk->master_id][1] = 0;
        Disp_Command_Name[ewk->master_id][2] = 0;
        Disp_Command_Name[ewk->master_id][ewk->master_player] = 1;
    }
}

s32 Select_End_Sub_79(WORK_Other* ewk) {
    if (Sel_Arts_Complete[ewk->master_id] == 0) {
        return 0;
    }

    if (ewk->wu.hit_quake == 0) {
        ewk->wu.routine_no[0] = 7;
        ewk->wu.routine_no[1] = 0;
        ewk->wu.dir_timer = 5;
        Extra_Counter[ewk->master_id] = 2;
        Plate_X[ewk->master_id][0] = ewk->wu.xyz[0].disp.pos;
        Plate_Y[ewk->master_id][0] = ewk->wu.xyz[1].disp.pos;
    } else if (Extra_Counter[ewk->master_id]) {
        ewk->wu.routine_no[0] = 6;
        ewk->wu.routine_no[1] = 0;
        ewk->wu.dir_timer = 1;
    } else {
        return 0;
    }

    return 1;
}

s32 effect_79_init(s16 pl_id, s16 plate_id, s16 pos_id, s16 time, s16 Target_BG) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->master_player = plate_id;
    ewk->master_priority = pos_id;
    ewk->wu.hit_quake = pos_id;
    ewk->wu.dmcal_m = pos_id;
    ewk->wu.be_flag = 1;
    ewk->wu.id = 79;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x2090;
    ewk->wu.my_family = Target_BG + 1;
    ewk->wu.dir_timer = time;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->master_id = pl_id;
    ewk->wu.char_index = 14;
    ewk->wu.dir_old = Play_Type;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);

    if (pos_id == 0) {
        ewk->wu.dir_step = 0;
    } else {
        ewk->wu.dir_step = 30;
    }

    Setup_Pos_79(ewk);

    if (pos_id == 0) {
        Disp_Command_Name[ewk->master_id][plate_id] = 1;
    } else {
        Disp_Command_Name[ewk->master_id][plate_id] = 0;
    }

    Plate_X[ewk->master_id][0] =
        bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Plate_Pos_Data_79[Play_Type][ewk->master_id][0][0];
    Plate_Y[ewk->master_id][0] =
        bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Plate_Pos_Data_79[Play_Type][ewk->master_id][0][1];
    return 0;
}

void Setup_Pos_79(WORK_Other* ewk) {
    if (ewk->master_priority) {
        ewk->wu.routine_no[0] = 3;
        ewk->wu.xyz[0].disp.pos =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Plate_Pos_Data_79[Play_Type][ewk->master_id][0][0];
        ewk->wu.xyz[1].disp.pos =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Plate_Pos_Data_79[Play_Type][ewk->master_id][0][1];
        ewk->wu.vital_new = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos +
                            Plate_Pos_Data_79[Play_Type][ewk->master_id][ewk->master_priority][0];
        ewk->wu.direction = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos +
                            Plate_Pos_Data_79[Play_Type][ewk->master_id][ewk->master_priority][1];
        ewk->wu.mvxy.a[1].sp = -0x20000;
        ewk->wu.mvxy.d[1].sp = -0x2000;

        if (ewk->wu.xyz[0].disp.pos < ewk->wu.vital_new) {
            ewk->wu.mvxy.a[0].sp = 0x18000;
            ewk->wu.mvxy.d[0].sp = 0x6000;
        } else {
            ewk->wu.mvxy.a[0].sp = -0x18000;
            ewk->wu.mvxy.d[0].sp = -0x6000;
        }

        Check_Speed_79(ewk);
    } else {
        ewk->wu.routine_no[0] = 0;
        OK_Appear79[ewk->master_id] = 0;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos +
                                  Plate_Pos_Data_79[Play_Type][ewk->master_id][ewk->master_priority][0];
        ewk->wu.xyz[1].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos +
                                  Plate_Pos_Data_79[Play_Type][ewk->master_id][ewk->master_priority][1];
    }

    ewk->wu.xyz[2].disp.pos = Pos_Z_Data_79[ewk->master_priority] + 35;
}

void Check_Speed_79(WORK_Other* ewk) {
    if (Play_Type == 1) {
        ewk->wu.mvxy.a[0].sp /= 3;
        ewk->wu.mvxy.d[0].sp /= 3;
        ewk->wu.mvxy.a[1].sp /= 3;
        ewk->wu.mvxy.d[1].sp /= 3;
    }
}

s32 Move_X_Sub(WORK_Other* ewk, s16 Target_X, s16 cut) {
    ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp * cut;
    ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

    if (Check_Depth_to_Before(ewk)) {
        return 0;
    }

    if (0 > ewk->wu.mvxy.a[0].sp) {
        if (Target_X >= ewk->wu.xyz[0].disp.pos) {
            ewk->wu.xyz[0].disp.pos = Target_X;
            ewk->wu.mvxy.a[0].sp = 0;
            ewk->wu.mvxy.d[0].sp = 0;
            return 1;
        }
    } else if (Target_X <= ewk->wu.xyz[0].disp.pos) {
        ewk->wu.xyz[0].disp.pos = Target_X;
        ewk->wu.mvxy.a[0].sp = 0;
        ewk->wu.mvxy.d[0].sp = 0;
        return 1;
    }

    return 0;
}

s32 Check_Depth_to_Before(WORK_Other* ewk) {
    if (0 <= ewk->wu.mvxy.a[1].sp) {
        return 0;
    }

    if (ewk->wu.hit_quake == 2 && ewk->wu.dmcal_m == 0) {
        return 1;
    }

    return 0;
}

s32 Move_Y_Sub(WORK_Other* ewk, s16 Target_Y, u16 cut) {
    ewk->wu.xyz[1].cal += ewk->wu.mvxy.a[1].sp * cut;
    ewk->wu.mvxy.a[1].sp += ewk->wu.mvxy.d[1].sp;

    if (0 > ewk->wu.mvxy.a[1].sp) {
        if (Target_Y >= ewk->wu.xyz[1].disp.pos) {
            ewk->wu.xyz[1].disp.pos = Target_Y;
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[1].sp = 0;
            return 1;
        }
    } else if (Target_Y <= ewk->wu.xyz[1].disp.pos) {
        ewk->wu.xyz[1].disp.pos = Target_Y;
        ewk->wu.mvxy.a[1].sp = 0;
        ewk->wu.mvxy.d[1].sp = 0;
        return 1;
    }

    return 0;
}
