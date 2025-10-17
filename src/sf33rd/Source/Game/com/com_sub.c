/**
 * @file com_sub.c
 * CPU Controlled Character Subroutines
 */

#include "sf33rd/Source/Game/com/com_sub.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/Com_Data.h"
#include "sf33rd/Source/Game/com/active/ac0000.h"
#include "sf33rd/Source/Game/com/active/ac0001.h"
#include "sf33rd/Source/Game/com/active/ac0002.h"
#include "sf33rd/Source/Game/com/active/ac0003.h"
#include "sf33rd/Source/Game/com/active/ac0004.h"
#include "sf33rd/Source/Game/com/ck_pass.h"
#include "sf33rd/Source/Game/com/com_datu.h"
#include "sf33rd/Source/Game/com/com_pl.h"
#include "sf33rd/Source/Game/com/follow/fl_com00.h"
#include "sf33rd/Source/Game/com/follow/fl_com02.h"
#include "sf33rd/Source/Game/com/passive/pass0000.h"
#include "sf33rd/Source/Game/com/passive/pass0001.h"
#include "sf33rd/Source/Game/com/passive/pass0002.h"
#include "sf33rd/Source/Game/com/passive/pass0003.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pls03.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/screen/vs_shell.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "structs.h"

// sbss
s8 Lv;
s8 Rnd;

// forward decls
void End_Pattern(PLW* wk);
void Next_Be_Passive(PLW* wk, s32);
void Turn_Over_On(PLW* wk);
void Only_Shot(PLW* wk, s16 Lever_Data);
void Lever_On(PLW* wk, u16 LR_Lever, u16 UD_Lever);
void Lever_Off(PLW* wk);
void Pierce_On(PLW* wk);
void Setup_DENJIN_LEVEL(PLW* wk);
void Push_Shot(PLW* wk, s16 Power_Level);
s32 Check_Exit_DENJIN(PLW* wk);
void Keep_Away(PLW* wk, s16 Target_Pos, s16 Option);
void Setup_KA_Jump(PLW* wk);
void Setup_KA_Walk(PLW* wk);
void Search_Back_Term(PLW* wk, s16 Move_Value, s16 Next_Action, s16 Next_Menu);
void Approach_Walk(PLW* wk, s16 Target_Pos, s16 Option);
s32 Check_Arrival(PLW* wk, s16 Target_Pos, s16 Option);
void Walk(PLW* wk, u16 Lever, s16 Time, s16 unused);
void Forced_Guard(PLW* wk, s16 Guard_Type);
void Provoke(PLW* wk, s16 Lever);
void Normal_Attack(PLW* wk, s16 Reaction, u16 Lever_Data);
s32 Small_Jump_Measure(PLW* wk);
void Normal_Attack_SP(PLW* wk, s16 Reaction, u16 Lever_Data, s16 Time);
void Adjust_Attack(PLW* wk, s16 Reaction, u16 Lever_Data);
s32 Check_Squat(PLW* wk);
s32 Check_Start_Normal_Attack(PLW* wk, s16 Reaction, u16 Lever_Data); // extra arg unused
void Lever_Attack(PLW* wk, s16 Reaction, u16 Lever, u16 Lever_Data);
void Lever_Attack_SP(PLW* wk, s16 Reaction, u16 Lever, u16 Lever_Data, s16 Time);
s32 Setup_Guard_Lever(PLW* wk, u16 Lever);
s32 Check_Start_Lever_Attack(PLW* wk, u16 Lever, u16 Lever_Data); // extra args
void SA_Term(PLW* wk, u16 SA0, u16 SA1, u16 SA2, u16 Term_No);
s32 DENJIN_Check(PLW* wk, u16 SA2, u16* xx, u16 Term_No);
s32 YAGYOU_Check(PLW* wk, s16* xx, u16 Term_No);
s32 SA_Range_Check(PLW* wk, s16 SA_No, u16 Range);
void Check_SA(PLW* wk, s16 Next_Action, s16 Next_Menu);
void Check_EX(PLW* wk, s16 Next_Action, s16 Next_Menu);
void Check_SA_Full(PLW* wk, s16 Next_Action, s16 Next_Menu);
void Branch_Unit_Area(PLW* wk, s16 Next_Action, s16 Menu_00, s16 Menu_01, s16 Menu_02, s16 Menu_03);
void Com_Random_Select(PLW* wk, s16 Next_Action, s16 Menu_00, s16 Menu_01, s16 Menu_02, s16 Menu_03, s16 Rnd_Type);
void Branch_Wait_Area(PLW* wk, s16 Time_00, s16 Time_01, s16 Time_02, s16 Time_03);
void Wait(PLW* wk, s16 Time); // unused arg
void Look(PLW* wk, s16 Time);
void Keep_Status(PLW* wk, u16 Lever_Data, s16 Option_Data);
void VS_Jump_Guard(PLW* wk);
void Wait_Lie(PLW* wk, u16 Lever_Data);
void Wait_Get_Up(PLW* wk, u16 Lever_Data, s16 Option);
s32 Check_Wait_Term(PLW* wk, s16 Option);
void Wait_Attack_Complete(PLW* wk, u16 Lever_Data, s16 Option);
s32 Check_Exit_Guard(PLW* wk, s16 Option);
void Short_Range_Attack(PLW* wk, s16 Reaction, u16 Lever_Data, s16 Next_Action, s16 Next_Menu);
void EM_Term(PLW* wk, s16 Range_X, s16 Range_Y, s16 Exit_Number, s16 Next_Action, s16 Next_Menu);
void SHELL_Term(PLW* wk, s16 Next_Command, s16 Exit_Number, s16 Next_Action, s16 Next_Menu, s16 unused); // unused arg
s32 Check_Term_Sub_Air(PLW* wk, s16 Distance, s16 Range);
s32 Check_Term_Sub(PLW* wk, s16 Distance, s16 Range);
s32 Correct_Unit_PL(PLW* wk);
s32 Check_Term_Sub_Y(PLW* wk, s16 Distance, s16 Range);
void Jump(PLW* wk, s16 Jump_Dir);
void Hi_Jump(PLW* wk, s16 Pl_Number, s16 Jump_Dir);
s32 Check_Start_Hi_Jump(PLW* wk);
s32 Check_Air_Guard(PLW* wk);
void Jump_Attack(PLW* wk, s16 Reaction, s16 Time_Data, u16 Lever_Data, s16 Jump_Dir);
void Jump_Attack_Term(PLW* wk, s16 Range_X, s16 Range_Y, s16 Reaction, u16 Lever_Data, s16 Jump_Dir, s16 Range_JX,
                      s16 Range_JY, s16 J_Lever_Data);
s32 Check_SP_Jump_Attack(PLW* wk, s16 Lever_Data);
s32 Check_VS_Air_Attack(PLW* wk, s16 Range_JX, s16 Range_JY, s16 J_Lever_Data);
void Hi_Jump_Attack(PLW* wk, s16 Reaction, s16 Time_Data, u16 Lever_Data, s16 Jump_Dir);
void Hi_Jump_Attack_Term(PLW* wk, s16 Range_X, s16 Range_Y, s16 Reaction, u16 Lever_Data, s16 Jump_Dir, s16 Range_JX,
                         s16 Range_JY, u16 J_Lever_Data);
s32 Check_Term_ABS_Distance(PLW* wk);
s32 Check_Com_Add_Y(PLW* wk, s16 Pos_Y, s16 Range);
void ORO_JA_Term(PLW* wk, s16 Reaction, s16 Jump_Dir, s16 JY, s16 Jump_Dir2, s16 RX, s16 RY, u16 Lever_Data, s16 RJX,
                 s16 RJY, u16 JLD);
void ORO_HJA_Term(PLW* wk, s16 Reaction, s16 Jump_Dir, s16 JY, s16 Jump_Dir2, s16 RX, s16 RY, u16 Lever_Data, s16 RJX,
                  s16 RJY, u16 JLD);
void Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot);
s32 Hadou_Check(PLW* wk, u16 Tech_Number);
s32 Check_Resume_Lever(PLW* wk);
void J_Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot);
void Rapid_Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Shot, u16 Time);
void Check_Rapid(PLW* wk, u16 Tech_Number);
void Setup_Rapid_End_Term(PLW* wk, s16 Tech_Number);
s32 Setup_Rapid_Time(PLW* wk, u16 Tech_Number); // unused all args
void Rapid_Sub(PLW* wk);
s32 Check_Rapid_End(PLW* wk);
s32 Check_Start_Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number);
void ORO_JCA_Term(PLW* wk, s16 Reaction, s16 Jump_Dir, s16 JY, s16 Jump_Dir2, s16 RX, s16 RY, u16 Tech_Number,
                  s16 Power_Level, s16 Ex_Shot, s16 RJX, s16 RJY, u16 JLD);
void ORO_HJCA_Term(PLW* wk, s16 Reaction, s16 Jump_Dir, s16 JY, s16 Jump_Dir2, s16 RX, s16 RY, u16 Tech_Number,
                   s16 Power_Level, s16 Ex_Shot, s16 RJX, s16 RJY, u16 JLD);
void Jump_Command_Attack_Term(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot, s16 RX, s16 RY,
                              s16 Jump_Dir, s16 JRX, s16 JRY, u16 JLD);
void Hi_Jump_Command_Attack_Term(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot, s16 RX, s16 RY,
                                 s16 Jump_Dir, s16 JRX, s16 JRY, u16 JLD);
s32 Check_Landed(PLW* wk, s16 Reaction);
s32 Check_Dash_Hit(PLW* wk, u16 Tech_Number);
s32 Setup_Front_or_Back(PLW* wk, s16 xx);
s32 Check_Hit_Shell(PLW* wk, WORK_Other* tmw, u16 Tech_Number);
void Jump_Init(PLW* wk, s16 Jump_Dir);
s32 Command_Type_00(PLW* wk, s16 Power_Level, u16 Tech_Number, s16 Ex_Shot);
s32 Command_Type_06(PLW* wk, s16 Power_Level, u16 Tech_Number, s16 Ex_Shot); // unused last arg
s32 Command_Type_01(PLW* wk, s16 Power_Level, s16 Ex_Shot);                  // unused args
void Setup_Command_01(PLW* wk);
void Check_Store_Lever(PLW* wk, u16 Tech_Number, s16 Next_Action, s16 Next_Menu);
s32 Check_Store_Direction(PLW* wk, u16 lever, s16 time);
s32 Select_Combo_Speed(PLW* wk);
s32 Select_Reflection_Time(PLW* wk);
s32 Setup_Lv04(s16 xx);
s32 Setup_Lv08(s16 xx);
s32 Setup_Lv10(s16 xx);
s32 Setup_Lv18(s16 xx);
s32 Setup_VS_Catch_Data(PLW* wk);
s32 Setup_LP_Data(PLW* wk);
s32 Setup_WT_Data(PLW* wk);
void Ck_Distance(PLW* wk);
s32 Ck_Distance_Height(PLW* wk);
s32 Ck_Area(PLW* wk);
s32 Ck_Area_Shell(PLW* wk);
void Ck_Distance_Lv(PLW* wk);
void Ck_Distance_LvJ(PLW* wk);
void Next_End(PLW* wk);
void Next_Another_Menu(PLW* wk, s16 Next_Action, u16 Next_Menu);
void Reaction_Sub(PLW* wk, s16 Reaction, s16 Power_Level);
s32 Check_Meoshi_Attack(PLW* wk, s16 Reaction, s16 Power_Level); // unused last 2 args
s32 Get_Meoshi_Data(PLW* wk);
void Reaction_Exit_Sub(PLW* wk);
void Check_First_Menu(PLW* wk);
void Select_Active(PLW* wk);
s32 Check_SA_Active(PLW* wk, s16* pl_id);
void Setup_Follow(PLW* wk, s16 Follow_Type);
void Decide_Follow_Menu(PLW* wk);
s32 Select_Passive(PLW* wk);
void Devide_Level(s16 xx);
void Setup_Random(PLW* wk);
s32 Check_Dramatic(PLW* wk, s16 PL_id);
s32 Check_Passive(PLW* wk);
s32 Check_Guard(PLW* wk);
s32 Check_Makoto(PLW* wk);
s32 Check_Flip_Term(PLW* wk, WORK* tmw);
s32 Setup_EM_Rank_Index(PLW* wk);
s32 Flip_Term_Correct(PLW* wk);
void Next_Be_Guard(PLW* wk, WORK* em, s16 Type_Of_Guard);
s32 Check_Flip_Tech(WORK* em);
void Next_Be_Flip(PLW* wk, s16 xx);
s32 Check_Diagonal_Shell(PLW* wk);
s32 Check_Ignore_Shell2(WORK_Other* tmw);
s32 Check_Shell(PLW* wk);
s32 Check_Shell_Another_in_Flip(PLW* wk);
s32 Check_Ignore_Shell(WORK_Other* tmw);
s32 Compute_Hit_Time(PLW* wk, WORK_Other* tmw);
s32 Decide_Shell_Guard(PLW* wk, WORK_Other* tmw); // unused second arg
void Guard_or_Jump_VS_Shell(PLW* wk, WORK_Other* tmw, s16 xx);
void Setup_Shell_Disposal(PLW* wk, WORK_Other* tmw);
void Next_Be_Shell_Guard(PLW* wk, WORK* tmw);
s32 Decide_Shell_Reaction(PLW* wk, WORK_Other* tmw, u16 dir_step); // unused second arg
s32 Ck_Distance_XX(s16 x1, s16 x2);
s32 Check_Behind(PLW* wk, WORK_Other* tmw);
void Setup_Lever_LR(PLW* wk, s16 PL_id, s16 Lever);
s32 Check_Exit_Term(PLW* wk, WORK* em, s16 Exit_No);
s32 VS_Jump_Term(PLW* wk, WORK* em, s16* xx);
s32 Exit_Term_0000(PLW* wk, WORK* em);
s32 Exit_Term_0001(PLW* wk, WORK* em);
s32 Exit_Term_0002(PLW* wk, WORK* em);
s32 Exit_Term_0003(PLW* wk, WORK* em);
s32 Exit_Term_0004(PLW* wk, WORK* em);
s32 Exit_Term_0005(PLW* wk, WORK* em);
s32 Exit_Term_0006(PLW* wk, WORK* em);
s32 Exit_Term_0007(PLW* wk, WORK* em);
s32 Exit_Term_0008(PLW* wk, WORK* em);
s32 Check_Drop_Term(WORK* em, s16 Y);
s32 Check_SHINRYU(PLW* wk);
void Check_BOSS(PLW* wk, u32 Next_Action, u16 Next_Menu);
void Check_BOSS_EX(PLW* wk, u32 Next_Action, u16 Next_Menu);
void ETC_Term(PLW* wk, s16 Exit_No, u32 Next_Action, u16 Next_Menu);
s32 ETC_Term_0000(PLW* wk, WORK* em);
s32 ETC_Term_0001(PLW* wk, WORK* em);
s32 ETC_Term_0002(PLW* wk, WORK* em);
s32 ETC_Term_0003(PLW* wk, WORK* em);
s32 ETC_Term_0004(PLW* wk, WORK* em);
s32 ETC_Term_0005(PLW* wk, WORK* em);
s32 ETC_Term_0006(PLW* wk, WORK* em);
s32 ETC_Term_0007(PLW* wk, WORK* em);
s32 ETC_Term_0008(PLW* wk, WORK* em);
s32 ETC_Term_0009(PLW* wk, WORK* em);
s32 emLevelRemake(s32 now, s32 max, s32 exd);
s32 emGetMaxBlocking();

void End_Pattern(PLW* wk) {
    Next_Be_Free(wk);
}

void Next_Be_Passive(PLW* wk, s32 unused) {
    Next_Be_Free(wk);
}

void Turn_Over_On(PLW* wk) {
    Disposal_Again[wk->wu.id] = 1;
    Turn_Over[wk->wu.id] = 1;
    CP_Index[wk->wu.id][0]++;
}

void Only_Shot(PLW* wk, s16 Lever_Data) {
    Lever_Buff[wk->wu.id] = Lever_Data;
    CP_Index[wk->wu.id][0]++;
}

void Lever_On(PLW* wk, u16 LR_Lever, u16 UD_Lever) {
    CP_Index[wk->wu.id][0]++;
    Disposal_Again[wk->wu.id] = 1;
    if ((LR_Lever == 0) || (LR_Lever == 1)) {
        Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, LR_Lever);
    } else {
        Lever_LR[wk->wu.id] = 0;
    }
    Lever_LR[wk->wu.id] |= UD_Lever;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Lever_Off(PLW* wk) {
    CP_Index[wk->wu.id][0]++;
    Disposal_Again[wk->wu.id] = 1;
    Lever_LR[wk->wu.id] = 0;
}

void Pierce_On(PLW* wk) {
    Disposal_Again[wk->wu.id] = 1;
    CP_Index[wk->wu.id][0]++;
    Pierce_Menu[wk->wu.id] = 1;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Setup_DENJIN_LEVEL(PLW* wk) {
    u16 xx;

    Disposal_Again[wk->wu.id] = 1;
    if ((xx = DENJIN_No[wk->wu.id])) {
        Next_Another_Menu(wk, 2, xx);
    } else {
        Next_Another_Menu(wk, 2, Denjin_Data[Area_Number[wk->wu.id]][random_16_com()]);
    }
}

void Push_Shot(PLW* wk, s16 Power_Level) {
    s16 xx;

    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if ((wk->wu.cg_type == 0x40) || (wk->wu.routine_no[1] == 0)) {
            Reaction_Exit_Sub(wk);
        } else {
            Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
            if ((wk->wu.now_koc == 8) && (wk->wu.char_index == 0xD)) {

                xx = wk->wu.cg_ix / wk->wu.cgd_type;
                if (xx >= Power_Level) {
                    CP_Index[wk->wu.id][1] = 0x63;
                }
            }
            if (Check_Exit_DENJIN(wk) != 0) {
                CP_Index[wk->wu.id][1] = 0x63;
            }
        }
        break;
    case 1:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (Check_Exit_DENJIN(wk) != 0) {
            CP_Index[wk->wu.id][1] = 0x63;
        }
        /* fallthrough */
    default:
        if ((wk->wu.cg_type == 0x40) || (wk->wu.routine_no[1] == 0)) {
            Reaction_Exit_Sub(wk);
        } else {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
            Reaction_Sub(wk, 8, Power_Level);
        }
        break;
    }
}

s32 Check_Exit_DENJIN(PLW* wk) {
    s16 xx;
    WORK* em;

    if (!(DENJIN_Term[wk->wu.id] & 1)) {
        if (CP_Index[wk->wu.id][1] == 0) {
            return 0;
        }
    }

    if ((DENJIN_Term[wk->wu.id] & 8)) {
        if (Attack_Flag[wk->wu.id]) {
            return 1;
        }
    }
    em = (WORK*)wk->wu.target_adrs;

    xx = 0;
    if ((em->xyz[0].disp.pos) != (em->old_pos[0])) {
        if (Check_Attack_Direction(wk, em) != 0) {
            xx = -1;
        } else {
            xx = 1;
        }
    }

    if ((DENJIN_Term[wk->wu.id] & 1) && (em->xyz[0].disp.pos != 0)) {
        if (CP_Index[wk->wu.id][2] == 0) {
            CP_Index[wk->wu.id][2]++;
            CP_Index[wk->wu.id][3] = Area_Number[wk->wu.id];
        }
        switch (CP_Index[wk->wu.id][3]) {
        case 0:
        case 1:
        case 2:
            if (em->mvxy.a[1].real.h > 0) {
                return 1;
            }
            if (em->mvxy.a[1].real.h < 0) {
                if (em->xyz[1].disp.pos < 0x29) {
                    return 1;
                }
            }
            break;
        default:
            if ((em->mvxy.a[1].real.h > 0) && (xx == -1)) {
                return 1;
            }
            if (em->mvxy.a[1].real.h < 0) {
                if (em->xyz[1].disp.pos < 0x29) {
                    return 1;
                }
            }
            break;
        }
    }
    if (xx == 0) {
        return 0;
    }

    if ((DENJIN_Term[wk->wu.id] & 2) && (xx == 1)) {
        return 1;
    }

    if ((DENJIN_Term[wk->wu.id] & 4) && (xx == -1)) {
        return 1;
    }

    if ((DENJIN_Term[wk->wu.id] & 0x20) && (Lie_Flag[wk->wu.id] == 0)) {
        return 1;
    }
    return 0;
}

void Keep_Away(PLW* wk, s16 Target_Pos, s16 Option) {
    switch (CP_Index[wk->wu.id][3]) {

    case 0:
        if (Option == 0) {
            if (random_16_com() < 4) {
                Setup_KA_Jump(wk);
            } else {
                Setup_KA_Walk(wk);
            }
        } else {
            if (Option == 1) {
                Setup_KA_Jump(wk);
            } else {
                CP_Index[wk->wu.id][3] = Option + 1;
            }
        }
        /* fallthrough */

    case 1:
    case 2:
        Jump(wk, CP_Index[wk->wu.id][3] - 1);
        break;

    case 3:
    case 4:
        Approach_Walk(wk, Target_Pos, CP_Index[wk->wu.id][3] - 1);
        break;
    }
}

void Setup_KA_Jump(PLW* wk) {
    s16 xx;

    CP_Index[wk->wu.id][3] = 2;
    xx = Back_Jump_Data[wk->player_number];

    if (wk->wu.rl_waza) {
        xx = wk->wu.xyz[0].disp.pos - Back_Jump_Data[wk->player_number];
        if ((bg_w.bgw[1].l_limit2 - bg_w.pos_offset) > xx) {
            CP_Index[wk->wu.id][3] = 1;
        }
    } else {
        xx = wk->wu.xyz[0].disp.pos + Back_Jump_Data[wk->player_number];
        if ((bg_w.bgw[1].r_limit2 + bg_w.pos_offset) < xx) {
            CP_Index[wk->wu.id][3] = 1;
        }
    }
}

void Setup_KA_Walk(PLW* wk) {
    CP_Index[wk->wu.id][3] = 4;
}

void Search_Back_Term(PLW* wk, s16 Move_Value, s16 Next_Action, s16 Next_Menu) {
    if (wk->wu.rl_waza) {
        Move_Value = wk->wu.xyz[0].disp.pos - Move_Value;
        if ((bg_w.bgw[1].l_limit2 - bg_w.pos_offset) > Move_Value) {
            Next_Another_Menu(wk, Next_Action, Next_Menu);
        } else {
            CP_Index[wk->wu.id][0]++;
        }
    } else {
        Move_Value = wk->wu.xyz[0].disp.pos + Move_Value;
        if (((bg_w.bgw[1].r_limit2) + (bg_w.pos_offset)) < (Move_Value)) {
            Next_Another_Menu(wk, Next_Action, Next_Menu);
        } else {
            CP_Index[wk->wu.id][0]++;
        }
    }
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Approach_Walk(PLW* wk, s16 Target_Pos, s16 Option) {
    s16 xx;

    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);
        Timer_00[wk->wu.id] = 0x78;
        /* fallthrough */

    case 1:
        xx = Standing_Timer[wk->wu.id];

        if (Lie_Flag[wk->wu.id] == 0) {
            if (Check_Passive(wk) != 0) {
                break;
            }
        }
        Standing_Timer[wk->wu.id] = xx;

        if (--Timer_00[wk->wu.id] == 0) {
            Next_Be_Free(wk);
        }

        else if (Check_Arrival(wk, Target_Pos, Option) != 0) {
            Disposal_Again[wk->wu.id] = 1;
            CP_Index[wk->wu.id][0]++;
            CP_Index[wk->wu.id][1] = 0;
            CP_Index[wk->wu.id][2] = 0;
            CP_Index[wk->wu.id][3] = 0;

            Flip_Flag[wk->wu.id] = 0;
            Limited_Flag[wk->wu.id] = 0;

            if (CP_No[wk->wu.id][0] != 6) {
                Passive_Flag[wk->wu.id] = 0;
            }
        } else {
            Ck_Distance_Lv(wk);
            if (Option == 3) {
                Lever_Buff[wk->wu.id] ^= 0xC;
            }
        }
    }
}

s32 Check_Arrival(PLW* wk, s16 Target_Pos, s16 Option) {
    if (Option == 3) {
        if (Target_Pos <= PL_Distance[wk->wu.id]) {
            return 1;
        }
        return wk->micchaku_flag;
    }

    if (wk->hos_em_flag) {
        return 1;
    }
    if (Target_Pos >= PL_Distance[wk->wu.id]) {
        return 1;
    }

    return 0;
}

void Walk(PLW* wk, u16 Lever, s16 Time, s16 unused) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);
        Timer_00[wk->wu.id] = Time;
        Timer_01[wk->wu.id] = wk->wu.rl_flag;
        Free_Lever[wk->wu.id] = Setup_Guard_Lever(wk, Lever);
        /* fallthrough */

    case 1:
        if (Lie_Flag[wk->wu.id] == 0) {
            if (Check_Passive(wk) != 0) {
                break;
            }
        }

        if (--Timer_00[wk->wu.id] == 0) {
            CP_Index[wk->wu.id][0]++;
            CP_Index[wk->wu.id][1] = 0;
            CP_Index[wk->wu.id][2] = 0;
            CP_Index[wk->wu.id][3] = 0;

            Flip_Flag[wk->wu.id] = 0;
            Limited_Flag[wk->wu.id] = 0;

            if (*CP_No[wk->wu.id] != 6) {
                Passive_Flag[wk->wu.id] = 0;
            }
        } else {
            if ((Timer_01[wk->wu.id] != (s16)wk->wu.rl_flag) || (wk->micchaku_flag != 0) || (wk->hos_em_flag != 0)) {
                Next_Be_Free(wk);
            }
            Lever_Buff[wk->wu.id] = Free_Lever[wk->wu.id];
        }
        break;
    }
}

void Forced_Guard(PLW* wk, s16 Guard_Type) {
    WORK* em;
    s16 xx;

    em = (WORK*)wk->wu.target_adrs;

    if (Attack_Flag[wk->wu.id] == 0) {
        Next_Be_Free(wk);
    }
    xx = Hit_Range_Data[em->hit_range];
    xx += Com_Width_Data[wk->wu.id];

    if (PL_Distance[wk->wu.id] > xx) {
        Next_Be_Free(wk);
    }

    Next_Be_Guard(wk, em, Guard_Type);
    Lever_Buff[wk->wu.id] |= Lever_Squat[wk->wu.id];
}

void Provoke(PLW* wk, s16 Lever) {
    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (wk->spmv_ng_flag & 1) {
            Next_Be_Free(wk);
            break;
        }

        CP_Index[wk->wu.id][1]++;
        if (Lever != -1) {
            Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, Lever & 1);
            Lever_LR[wk->wu.id] |= Lever & 2;
        }
        /* fallthrough */
    case 1:

        if (wk->permited_koa & 0x80) {
            CP_Index[wk->wu.id][1]++;
            Lever_Buff[wk->wu.id] = 0x440;
        }
        break;

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if ((wk->wu.routine_no[1] != 4) || (wk->wu.routine_no[2] != 0x1E)) {
            Reaction_Exit_Sub(wk);
        }
        break;
    }
}

void Normal_Attack(PLW* wk, s16 Reaction, u16 Lever_Data) {
    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (Lever_Data & 2) {
            Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, 1);
        } else {
            Lever_LR[wk->wu.id] = 0;
        }

        Lever_LR[wk->wu.id] |= Lever_Data & 2;
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

        if (Check_Start_Normal_Attack(wk, Reaction, Lever_Data) != 0) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id] == 0) {
            CP_Index[wk->wu.id][1]++;
            Lever_Buff[wk->wu.id] = Lever_Data;
            Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
        } else {
            Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
        }
        break;

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Reaction_Sub(wk, Reaction, 0);
        break;
    }
}

s32 Small_Jump_Measure(PLW* wk) {
    if (Lever_Squat[wk->wu.id] & 2) {
        return Setup_Guard_Lever(wk, 1);
    }
    return 0;
}

void Normal_Attack_SP(PLW* wk, s16 Reaction, u16 Lever_Data, s16 Time) {
    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (Check_Start_Normal_Attack(wk, Reaction, Lever_Data) != 0) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        Timer_00[wk->wu.id] = Time;
        Check_First_Menu(wk);
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id] == 0) {
            Lever_Buff[wk->wu.id] = Lever_Data;
            Lever_Squat[wk->wu.id] = Lever_Data & 2;
            CP_Index[wk->wu.id][1]++;
            Timer_00[wk->wu.id]--;
        } else {
            Lever_Buff[wk->wu.id] = Lever_Squat[wk->wu.id];
        }
        break;

    case 2:
        if (--Timer_00[wk->wu.id]) {
            Lever_Buff[wk->wu.id] = Lever_Data;
            Lever_Squat[wk->wu.id] = Lever_Data & 2;
        } else {
            CP_Index[wk->wu.id][1]++;
        }
        break;

    default:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Reaction_Sub(wk, Reaction, 0);
        break;
    }
}

void Adjust_Attack(PLW* wk, s16 Reaction, u16 Lever_Data) {
    u16 xx;

    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (Check_Start_Normal_Attack(wk, Reaction, Lever_Data) != 0) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id] == 0) {
            xx = Check_Squat(wk);
            Lever_Buff[wk->wu.id] = Lever_Data | xx;
            Lever_LR[wk->wu.id] = xx;
            Lever_Buff[wk->wu.id] |= Small_Jump_Measure(wk);
            CP_Index[wk->wu.id][1]++;
        } else {
            Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
            Lever_Buff[wk->wu.id] |= Small_Jump_Measure(wk);
        }
        break;

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Reaction_Sub(wk, Reaction, 0);
        break;
    }
}

s32 Check_Squat(PLW* wk) {
    if (((WORK*)wk->wu.target_adrs)->pat_status == 0x20) {
        return 0;
    }

    return 2;
}

s32 Check_Start_Normal_Attack(PLW* wk, s16 Reaction, u16 Lever_Data) {
    if (((wk->wu.routine_no[1]) != 4) || ((wk->wu.cg_type) == 0x40)) {
        return 0;
    }

    if (wk->wu.cg_cancel & 4) {
        return 0;
    }

    if (wk->permited_koa & 0x10) {
        return 0;
    }

    if ((wk->wu.cg_cancel & 8) && (Reaction == 0xE)) {
        return 0;
    }

    return 1;
}

void Lever_Attack(PLW* wk, s16 Reaction, u16 Lever, u16 Lever_Data) {
    s16 xx;

    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (Check_Start_Lever_Attack(wk, Lever, Lever_Data) != 0) {
            break;
        }
        dash_flag_clear(wk->wu.id);

        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        /* falltrhough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id]) {
            break;
        }

        xx = Setup_Guard_Lever(wk, Lever);
        Lever_Buff[wk->wu.id] = (Lever_Data | xx);
        CP_Index[wk->wu.id][1]++;
        break;

    default:
        if (wk->wu.routine_no[1] == 2) {
            Be_Catch(wk);
        } else {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
            Reaction_Sub(wk, Reaction, 0);
        }
        break;
    }
}

void Lever_Attack_SP(PLW* wk, s16 Reaction, u16 Lever, u16 Lever_Data, s16 Time) {
    s16 xx;

    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (Check_Start_Lever_Attack(wk, Lever, Lever_Data) != 0) {
            break;
        }
        dash_flag_clear(wk->wu.id);

        Timer_00[wk->wu.id] = Time;
        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id]) {
            break;
        }

        xx = Setup_Guard_Lever(wk, Lever);
        Lever_Buff[wk->wu.id] = (Lever_Data | xx);

        Timer_00[wk->wu.id]--;
        CP_Index[wk->wu.id][1]++;
        break;

    case 2:
        if (--Timer_00[wk->wu.id]) {
            Lever_Buff[wk->wu.id] = Lever_Data;
            Lever_Squat[wk->wu.id] = Lever_Data & 2;
        } else {
            CP_Index[wk->wu.id][1]++;
        }
        break;

    default:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Reaction_Sub(wk, Reaction, 0);
        break;
    }
}

s32 Setup_Guard_Lever(PLW* wk, u16 Lever) {
    s32 rnum = 0;

    switch (Lever) {
    case 0:
        if (wk->wu.rl_waza == 0) {
            rnum = 4;
        } else {
            rnum = 8;
        }
        break;
    case 1:
        if (wk->wu.rl_waza == 1) {
            rnum = 4;
        } else {
            rnum = 8;
        }
        break;
    }
    return rnum;
}

s32 Check_Start_Lever_Attack(PLW* wk, u16 Lever, u16 Lever_Data) {
    if ((wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40)) {
        return 0;
    }

    if (wk->wu.cg_cancel & 4) {
        return 0;
    }

    if (wk->wu.cg_cancel & 8) {
        return 0;
    }

    return 1;
}

void SA_Term(PLW* wk, u16 SA0, u16 SA1, u16 SA2, u16 Term_No) {
    s16 xx[3];

    if (((Passive_Flag[wk->wu.id]) == 0) && (Check_Passive(wk) != 0)) {
        return;
    }

    xx[0] = SA0;
    xx[1] = SA1;
    xx[2] = SA2;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    if ((xx[gs.plw[wk->wu.id].sa->kind_of_arts] == -1) || gs.plw[wk->wu.id].metamorphose) {
        CP_Index[wk->wu.id][0]++;
    } else if ((gs.plw[wk->wu.id].sa->ok) || (gs.plw[wk->wu.id].sa->mp)) {
        Disposal_Again[wk->wu.id] = 1;

        if ((Term_No != 0xFFFF) || (Term_No != 0)) {
            switch (wk->player_number) {
            case 2:
                if (SA_Range_Check(wk, 1, Term_No) != 0) {
                    return;
                }
                DENJIN_Check(wk, SA2, (u16*)&xx[2], Term_No);
                Next_Another_Menu(wk, 2, xx[gs.plw[wk->wu.id].sa->kind_of_arts]);
                return;

            case 11:
                if (SA_Range_Check(wk, 1, Term_No) != 0) {
                    return;
                }
                Next_Another_Menu(wk, 2, xx[gs.plw[wk->wu.id].sa->kind_of_arts]);
                return;

            case 1:
                if (SA_Range_Check(wk, 1, Term_No) != 0) {
                    return;
                }
                Next_Another_Menu(wk, 2, xx[gs.plw[wk->wu.id].sa->kind_of_arts]);
                return;

            case 5:
                if (((WORK*)wk->wu.target_adrs)->xyz[1].disp.pos >= 0x10) {
                    CP_Index[wk->wu.id][0]++;
                    return;
                }
                if (SA_Range_Check(wk, 1, Term_No) != 0) {
                    return;
                }
                Next_Another_Menu(wk, 2, xx[gs.plw[wk->wu.id].sa->kind_of_arts]);
                return;

            case 6:
                if (SA_Range_Check(wk, 0, Term_No) != 0) {
                    return;
                }
                Next_Another_Menu(wk, 2, xx[gs.plw[wk->wu.id].sa->kind_of_arts]);
                return;

            case 8:
                if ((gs.plw[wk->wu.id].sa->kind_of_arts == 2) &&
                    (gs.plw[wk->wu.id].wu.vital_new <= (Max_vitality / 2))) {
                    break;
                }
                CP_Index[wk->wu.id][0]++;
                return;

            case 9:
                YAGYOU_Check(wk, &xx[1], Term_No);
                Next_Another_Menu(wk, 2, xx[gs.plw[wk->wu.id].sa->kind_of_arts]);
                return;

            case 14:
                if (SA_Range_Check(wk, 1, Term_No) != 0) {
                    return;
                }
                if (SA_Range_Check(wk, 2, Term_No) != 0) {
                    return;
                }
                Next_Another_Menu(wk, 2, xx[gs.plw[wk->wu.id].sa->kind_of_arts]);
                return;

            default:
                Next_Another_Menu(wk, 2, xx[gs.plw[wk->wu.id].sa->kind_of_arts]);
                return;
            }
        }

        Next_Another_Menu(wk, 2, xx[gs.plw[wk->wu.id].sa->kind_of_arts]);
    } else {
        CP_Index[wk->wu.id][0]++;
    }
}

s32 DENJIN_Check(PLW* wk, u16 SA2, u16* xx, u16 Term_No) {
    if (gs.plw[wk->wu.id].sa->kind_of_arts != 2) {
        return 0;
    }

    DENJIN_No[wk->wu.id] = Term_No;
    DENJIN_Term[wk->wu.id] = SA2;
    xx[0] = 0x37;
    return 1;
}

const u8 YAGYOU_Data[0x10] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3 };

s32 YAGYOU_Check(PLW* wk, s16* xx, u16 Term_No) {
    if (gs.plw[wk->wu.id].sa->kind_of_arts != 1) {
        return 0;
    }

    if (Term_No == 0) {
        Term_No = YAGYOU_Data[random_16_com()];
        Term_No += 0x64;
    }
    xx[0] = Term_No;
    return 1;
}

s32 SA_Range_Check(PLW* wk, s16 SA_No, u16 Range) {
    if (SA_No != gs.plw[wk->wu.id].sa->kind_of_arts) {
        return 0;
    }

    if (Range & 0x8000) {
        if ((PL_Distance[wk->wu.id]) < (Range & 0x7FFF)) {
            CP_Index[wk->wu.id][0]++;
            return 1;
        }

    }

    else if (PL_Distance[wk->wu.id] > Range) {
        CP_Index[wk->wu.id][0]++;
        return 1;
    }

    return 0;
}

void Check_SA(PLW* wk, s16 Next_Action, s16 Next_Menu) {
    if (gs.plw[wk->wu.id].sa->ok) {
        CP_Index[wk->wu.id][0]++;
    } else {
        CP_No[wk->wu.id][0] = Next_Action;
        Disposal_Again[wk->wu.id] = 1;
        Next_Another_Menu(wk, Next_Action, Next_Menu);
    }
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Check_EX(PLW* wk, s16 Next_Action, s16 Next_Menu) {
    if (gs.plw[wk->wu.id].sa->ex) {
        CP_Index[wk->wu.id][0]++;
    } else {
        CP_No[wk->wu.id][0] = Next_Action;
        Disposal_Again[wk->wu.id] = 1;
        Next_Another_Menu(wk, Next_Action, Next_Menu);
    }
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Check_SA_Full(PLW* wk, s16 Next_Action, s16 Next_Menu) {
    Disposal_Again[wk->wu.id] = 1;

    if (wk->permited_koa & 0x40) {
        CP_Index[wk->wu.id][0]++;
    } else {
        CP_No[wk->wu.id][0] = Next_Action;
        Next_Another_Menu(wk, Next_Action, Next_Menu);
    }
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Branch_Unit_Area(PLW* wk, s16 Next_Action, s16 Menu_00, s16 Menu_01, s16 Menu_02, s16 Menu_03) {
    s16 xx[4];

    CP_No[wk->wu.id][0] = Next_Action;
    xx[0] = Menu_00;
    xx[1] = Menu_01;
    xx[2] = Menu_02;
    xx[3] = Menu_03;

    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    Disposal_Again[wk->wu.id] = 1;
    Next_Another_Menu(wk, Next_Action, xx[Area_Number[wk->wu.id]]);
}

void Com_Random_Select(PLW* wk, s16 Next_Action, s16 Menu_00, s16 Menu_01, s16 Menu_02, s16 Menu_03, s16 Rnd_Type) {
    s16 xx[4];
    s16 zz;

    zz = Com_Rnd_Select_Data[Rnd_Type][random_16_com()];

    xx[0] = Menu_00;
    xx[1] = Menu_01;
    xx[2] = Menu_02;
    xx[3] = Menu_03;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    if (xx[zz] == 0xFF) {
        Next_End(wk);
    } else {
        Disposal_Again[wk->wu.id] = 1;
        Next_Another_Menu(wk, Next_Action, xx[zz]);
    }
}

void Branch_Wait_Area(PLW* wk, s16 Time_00, s16 Time_01, s16 Time_02, s16 Time_03) {
    s16 xx[4];

    switch (CP_Index[wk->wu.id][1]) {
    case 0:

        CP_Index[wk->wu.id][1]++;

        xx[0] = Time_00;
        xx[1] = Time_01;
        xx[2] = Time_02;
        xx[3] = Time_03;
        Timer_00[wk->wu.id] = xx[Area_Number[wk->wu.id]];
        break;

    default:
        if (--Timer_00[wk->wu.id]) {
            break;
        }
        CP_Index[wk->wu.id][0]++;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;

        Flip_Flag[wk->wu.id] = 0;
        Limited_Flag[wk->wu.id] = 0;

        if (CP_No[wk->wu.id][0] != 6) {
            Passive_Flag[wk->wu.id] = 0;
        }
        break;
    }
}

void Wait(PLW* wk, s16 Time) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;

        if (Time == 0) {
            Timer_00[wk->wu.id] = Setup_WT_Data(wk);
        } else {
            Timer_00[wk->wu.id] = Time;
        }

        break;

    default:
        if (--Timer_00[wk->wu.id]) {
            break;
        }
        CP_Index[wk->wu.id][0]++;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;

        Flip_Flag[wk->wu.id] = 0;
        Limited_Flag[wk->wu.id] = 0;

        if (CP_No[wk->wu.id][0] != 6) {
            Passive_Flag[wk->wu.id] = 0;
        }
        break;
    }
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Look(PLW* wk, s16 Time) {
    Passive_Flag[wk->wu.id] = 0;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        CP_Index[wk->wu.id][1]++;

        if (Time == 0) {
            Timer_00[wk->wu.id] = Setup_LP_Data(wk);
        } else {
            Timer_00[wk->wu.id] = Time;
        }

        if (Lever_LR[wk->wu.id] & 2) {
            Timer_00[wk->wu.id] += 0x32;
        }

        break;

    default:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Timer_00[wk->wu.id] != 0) {
            break;
        }

        CP_Index[wk->wu.id][0]++;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;

        Flip_Flag[wk->wu.id] = 0;
        Limited_Flag[wk->wu.id] = 0;
        Before_Look[wk->wu.id] = 1;
        break;
    }
}

void Keep_Status(PLW* wk, u16 Lever_Data, s16 Option_Data) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);
        Timer_00[wk->wu.id] = 0xA;

        Free_Lever[wk->wu.id] = Lever_Data;
        if (Option_Data != -1) {
            Free_Lever[wk->wu.id] |= Setup_Guard_Lever(wk, Option_Data);
        }
        Lever_Buff[wk->wu.id] = Free_Lever[wk->wu.id];

        break;

    default:
        Lever_Buff[wk->wu.id] = Free_Lever[wk->wu.id];
        if (--Timer_00[wk->wu.id]) {
            break;
        }
        Timer_00[wk->wu.id] = 1;

        if (Attack_Flag[wk->wu.id] == 0) {
            CP_Index[wk->wu.id][0]++;
            CP_Index[wk->wu.id][1] = 0;
            CP_Index[wk->wu.id][2] = 0;
            CP_Index[wk->wu.id][3] = 0;

            Flip_Flag[wk->wu.id] = 0;
            Limited_Flag[wk->wu.id] = 0;

            if (CP_No[wk->wu.id][0] != 6) {
                Passive_Flag[wk->wu.id] = 0;
            }
        }
        break;
    }
}

void VS_Jump_Guard(PLW* wk) {
    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Guard(wk) == 0) {
            dash_flag_clear(wk->wu.id);
            CP_Index[wk->wu.id][1]++;
        }

        break;

    default:
        if (Check_Guard(wk) != 0) {
            break;
        }

        if (((WORK*)wk->wu.target_adrs)->xyz[1].disp.pos < 0x19) {
            CP_Index[wk->wu.id][0]++;
            CP_Index[wk->wu.id][1] = 0;
            CP_Index[wk->wu.id][2] = 0;
            CP_Index[wk->wu.id][3] = 0;

            Passive_Flag[wk->wu.id] = 0;
            Flip_Flag[wk->wu.id] = 0;
            Limited_Flag[wk->wu.id] = 0;
        }

        break;
    }
}

void Wait_Lie(PLW* wk, u16 Lever_Data) {
    WORK* em;

    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);
        Rolling_Flag[wk->wu.id] = 0;

        if (Lever_Data != 0) {
            Free_Lever[wk->wu.id] = Setup_Guard_Lever(wk, 1);
            Free_Lever[wk->wu.id] |= Lever_Data & 2;
        } else {
            Free_Lever[wk->wu.id] = 0;
        }
        /* fallthrough */

    default:
        Lever_Buff[wk->wu.id] = Free_Lever[wk->wu.id];

        em = (WORK*)wk->wu.target_adrs;
        if ((Check_Blow_Off(wk, em, 0) == 0) || (Lie_Flag[wk->wu.id] != 0)) {
            CP_Index[wk->wu.id][0]++;
            CP_Index[wk->wu.id][1] = 0;
            CP_Index[wk->wu.id][2] = 0;
            CP_Index[wk->wu.id][3] = 0;

            Flip_Flag[wk->wu.id] = 0;

            Limited_Flag[wk->wu.id] = 0;
        }
        break;
    }
}

void Wait_Get_Up(PLW* wk, u16 Lever_Data, s16 Option) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);
        Rolling_Flag[wk->wu.id] = 0;

        if (Lever_Data != 0) {
            Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, 1);
            Lever_LR[wk->wu.id] |= Lever_Data & 2;
        } else {
            Lever_LR[wk->wu.id] = 0;
        }

        /* fallthrough */

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

        if (Check_Wait_Term(wk, Option) != 0) {
            CP_Index[wk->wu.id][0]++;
            CP_Index[wk->wu.id][1] = 0;
            CP_Index[wk->wu.id][2] = 0;
            CP_Index[wk->wu.id][3] = 0;

            Disposal_Again[wk->wu.id] = 1;
            Passive_Flag[wk->wu.id] = 1;
            Flip_Flag[wk->wu.id] = 0;
            Limited_Flag[wk->wu.id] = 0;
        }
        break;
    }
}

s32 Check_Wait_Term(PLW* wk, s16 Option) {
    WORK* em;

    em = (WORK*)wk->wu.target_adrs;

    if ((em->routine_no[1] == 1) && (em->pat_status == 0x18)) {
        return 0;
    }
    if (Lie_Flag[wk->wu.id] == 0) {
        return 1;
    }
    if (Option != 0) {
        return 0;
    }
    if (em->cg_type == 0xB) {
        return 1;
    }
    return 0;
}

void Wait_Attack_Complete(PLW* wk, u16 Lever_Data, s16 Option) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);

        if (Lever_Data != 0) {
            Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, 1);
            Lever_LR[wk->wu.id] |= Lever_Data & 2;
            Guard_Flag[wk->wu.id] = 1;
        } else {
            Lever_LR[wk->wu.id] = 0;
        }

        /* fallthrough */

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

        if (Check_Exit_Guard(wk, Option) == 0) {
            CP_Index[wk->wu.id][0]++;
            CP_Index[wk->wu.id][1] = 0;
            CP_Index[wk->wu.id][2] = 0;
            CP_Index[wk->wu.id][3] = 0;
            Guard_Flag[wk->wu.id] = 0;

            Flip_Flag[wk->wu.id] = 0;
            Limited_Flag[wk->wu.id] = 0;
            if (Option == 0) {
                Passive_Flag[wk->wu.id] = 0;
            }
        }
        break;
    }
}

s32 Check_Exit_Guard(PLW* wk, s16 Option) {
    WORK* em;

    if (wk->wu.routine_no[1] == 1) {
        return 1;
    }

    if (Option == 0) {
        em = (WORK*)wk->wu.target_adrs;
        if (em->routine_no[1] != 4) {
            return 0;
        }
        return 1;
    }
    return Attack_Flag[wk->wu.id];
}

void Short_Range_Attack(PLW* wk, s16 Reaction, u16 Lever_Data, s16 Next_Action, s16 Next_Menu) {
    u16 xx;

    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (Check_Start_Normal_Attack(wk, Reaction, Lever_Data) == 0) {
            CP_Index[wk->wu.id][1]++;
            Check_First_Menu(wk);

            Ck_Distance_LvJ(wk);
            xx = get_nearing_range(wk->player_number, xx = Lever_Data & 0xFF0);
            if (PL_Distance[wk->wu.id] > xx) {
                Next_Another_Menu(wk, Next_Action, Next_Menu);
            }
        }

        break;

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id]) {
            break;
        }
        Lever_Buff[wk->wu.id] = Lever_Data;
        CP_Index[wk->wu.id][1]++;
        break;

    default:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Reaction_Sub(wk, Reaction, 0);
        break;
    }
}

void EM_Term(PLW* wk, s16 Range_X, s16 Range_Y, s16 Exit_Number, s16 Next_Action, s16 Next_Menu) {
    WORK* em;

    em = (WORK*)wk->wu.target_adrs;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        CP_Index[wk->wu.id][1]++;
        Term_No[wk->wu.id] = 0;
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }

        switch (Check_Exit_Term(wk, em, Exit_Number)) {
        case 0:
            if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], Range_X) == 0) {
                break;
            }

            if (Exit_Number != 8) {
                if (Check_Term_Sub_Y(wk, em->xyz[1].disp.pos, Range_Y) == 0) {
                    break;
                }
            } else {
                if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, Range_Y) == 0) {
                    break;
                }
            }

            Disposal_Again[wk->wu.id] = 1;
            CP_Index[wk->wu.id][0]++;
            CP_Index[wk->wu.id][1] = 0;
            CP_Index[wk->wu.id][2] = 0;
            CP_Index[wk->wu.id][3] = 0;

            Flip_Flag[wk->wu.id] = 0;
            Limited_Flag[wk->wu.id] = 0;
            break;

        case 1:
            Disposal_Again[wk->wu.id] = 1;
            Next_Another_Menu(wk, Next_Action, Next_Menu);
            break;

        case 2:
            break;

        case 3:
            Select_Passive(wk);
            break;

        default:
            Counter_Attack[wk->wu.id] = 1;
            Select_Passive(wk);
            break;
        }

        break;
    }
}

void SHELL_Term(PLW* wk, s16 Next_Command, s16 Exit_Number, s16 Next_Action, s16 Next_Menu, s16 unused) {
    WORK* em;
    WORK_Other* tmw;
    s16 xx;

    em = (WORK*)Shell_Address[wk->wu.id];
    tmw = (WORK_Other*)Shell_Address[wk->wu.id];

    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;
        Term_No[wk->wu.id] = 0;
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (Check_Exit_Term(wk, em, Exit_Number) == 1) {
            Next_Another_Menu(wk, Next_Action, Next_Menu);
        } else {
            xx = Compute_Hit_Time(wk, tmw);
            if (xx < Shell_Dodge_Data[Next_Command][wk->player_number]) {
                Disposal_Again[wk->wu.id] = 1;
                CP_Index[wk->wu.id][0]++;
                CP_Index[wk->wu.id][1] = 0;
                CP_Index[wk->wu.id][2] = 0;
                CP_Index[wk->wu.id][3] = 0;

                Flip_Flag[wk->wu.id] = 0;
                Limited_Flag[wk->wu.id] = 0;
            }
        }
        break;
    }
}

s32 Check_Term_Sub_Air(PLW* wk, s16 Distance, s16 Range) {
    if (Range == -1) {
        return 1;
    }
    if (!(Range & 0x8000)) {
        if (Distance >= Range) {
            return 1;
        }
        return 0;
    } else {
        Range += Correct_Unit_PL(wk);

        if (Distance <= (Range & 0x7FFF)) {
            return 1;
        }
        return 0;
    }
}

s32 Check_Term_Sub(PLW* wk, s16 Distance, s16 Range) {
    if (Range == -1) {
        return 1;
    }
    if (!(Range & 0x8000)) {
        if (Distance >= Range) {
            return 1;
        }
        return 0;
    } else {
        if (Distance <= (Range & 0x7FFF)) {
            return 1;
        }
        return 0;
    }
}

const s16 Correct_VS_Air_Data[0x14] = { 0, 0x20, 0, 0, 0, 0x20, 0x20, 0, 0x20, 0, 0, 0, 0, 0x20, 0, 0, 0, 0, 0, 0 };

s32 Correct_Unit_PL(PLW* wk) {
    return Correct_VS_Air_Data[My_char[Player_id]];
}

s32 Check_Term_Sub_Y(PLW* wk, s16 Distance, s16 Range) {
    WORK* em;

    if (Range == -1) {
        return 1;
    }

    em = (WORK*)wk->wu.target_adrs;
    if (!(Range & 0x8000)) {
        if (Distance >= Range) {
            return 1;
        }
        return 0;
    } else {
        if (em->mvxy.a[1].real.h > 0) {
            return 0;
        }
        if (Distance <= (Range & 0x7FFF)) {
            return 1;
        }
        return 0;
    }
}

void Jump(PLW* wk, s16 Jump_Dir) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }

        if ((wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40)) {
            CP_Index[wk->wu.id][1]++;
            hi_jump_flag_clear(wk->wu.id);
            Check_First_Menu(wk);
        }

        break;

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (--Combo_Speed[wk->wu.id] != 0) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        Jump_Init(wk, Jump_Dir);
        if (Check_Diagonal_Shell(wk) != 0) {
            Next_Be_Free(wk);
        }

        break;

    case 2:
        Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];

        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
            Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
            Check_Air_Guard(wk);
        }
        break;

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (wk->wu.xyz[1].disp.pos) {
            break;
        }

        CP_Index[wk->wu.id][0]++;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;
        break;
    }
}

void Hi_Jump(PLW* wk, s16 Pl_Number, s16 Jump_Dir) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }

        if (Check_Start_Hi_Jump(wk) == 0) {
            CP_Index[wk->wu.id][1]++;
            if (cmd_sel[wk->wu.id]) {
                Tech_Address[wk->wu.id] = player_CMD[Pl_Number][2];
            } else {
                Tech_Address[wk->wu.id] = player_cmd[Pl_Number][2];
            }
            Check_First_Menu(wk);
        }

        break;

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id] != 0) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        Tech_Index[wk->wu.id] = 0xC;

        Jump_Init(wk, Jump_Dir);
        if (Check_Diagonal_Shell(wk) != 0) {
            Next_Be_Free(wk);
        }

        Lever_Buff[wk->wu.id] = 0;

        break;

    case 2:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (Command_Type_00(wk, 8, 0xFFFF, -1) == -1) {
            CP_Index[wk->wu.id][1]++;
            Lever_Buff[wk->wu.id] |= Lever_Pool[wk->wu.id];
            break;
        }

        if (!(Lever_Buff[wk->wu.id] & 2)) {
            Lever_Buff[wk->wu.id] |= Lever_Pool[wk->wu.id];
        }
        break;

    case 3:
        Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
            Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
            Check_Air_Guard(wk);
        }
        break;

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        Check_Air_Guard(wk);

        if (wk->wu.xyz[1].disp.pos) {
            break;
        }
        CP_Index[wk->wu.id][0]++;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;
        break;
    }
}

s32 Check_Start_Hi_Jump(PLW* wk) {
    if ((wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40)) {
        return 0;
    }

    if (wk->wu.cg_cancel & 4) {
        return 0;
    }

    if (wk->wu.cg_cancel & 1) {
        return 0;
    }

    return 1;
}

s32 Check_Air_Guard(PLW* wk) {
    WORK* em;
    s16 xx;
    s16 zz;

    em = (WORK*)wk->wu.target_adrs;

    if (Lever_LR[wk->wu.id]) {
        return Lever_LR[wk->wu.id];
    }
    if (Guard_Counter[wk->wu.id] == Attack_Counter[wk->wu.id]) {
        return Lever_LR[wk->wu.id];
    }
    if (Attack_Flag[wk->wu.id] == 0) {
        return Lever_LR[wk->wu.id];
    }

    xx = Hit_Range_Data[em->hit_range] + 0x20;
    xx += Com_Width_Data[wk->wu.id];
    if (PL_Distance[wk->wu.id] > xx) {
        return 0;
    }

    Guard_Counter[wk->wu.id] = Attack_Counter[wk->wu.id];
    Lv = Setup_Lv10(0);
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Rnd = random_16_com();
    Lv += CC_Value[0];

    if (Lv >= 7) {
        Lv = 0xA;
    }

    Lv = emLevelRemake(Lv, 0xB, 1);

    zz = Setup_EM_Rank_Index(wk);

    if (Guard_Data[zz][Lv][Rnd] == 3) {
        return Lever_LR[wk->wu.id] = 0;
    }
    Guard_Type[wk->wu.id] = Guard_Data[zz][Lv][random_16_ex_com()];

    Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, 1);
    Lever_LR[wk->wu.id] |= 2;
    return Guard_Type[wk->wu.id] |= 0x8000;
}

void Jump_Attack(PLW* wk, s16 Reaction, s16 Time_Data, u16 Lever_Data, s16 Jump_Dir) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }

        if ((wk->wu.routine_no[1] == 4) && (wk->wu.cg_type != 0x40)) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        Timer_00[wk->wu.id] = Time_Data;
        Continue_Menu[wk->wu.id] = 0;
        wk->wu.hf.hit.player = 0;
        hi_jump_flag_clear(wk->wu.id);
        Check_First_Menu(wk);
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id] != 0) {
            break;
        }

        Timer_00[wk->wu.id] = Time_Data;
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);
        Jump_Init(wk, Jump_Dir);
        if (Check_Diagonal_Shell(wk) != 0) {
            Next_Be_Free(wk);
        }

        break;

    case 2:
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
        }

        else {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        }

        break;

    case 3:
        Check_Air_Guard(wk);

        if (--Timer_00[wk->wu.id] != 0) {
            break;
        }

        Lever_Data = Check_SP_Jump_Attack(wk, Lever_Data);
        Lever_Buff[wk->wu.id] = Lever_Data;
        CP_Index[wk->wu.id][1]++;

        break;

    default:
        Check_Air_Guard(wk);
        if (wk->wu.hf.hit.player) {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        }
        Check_Landed(wk, Reaction & 0xFFF);
        break;
    }
    if (CP_Index[wk->wu.id][1] >= 3) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}

void Jump_Attack_Term(PLW* wk, s16 Range_X, s16 Range_Y, s16 Reaction, u16 Lever_Data, s16 Jump_Dir, s16 Range_JX,
                      s16 Range_JY, s16 J_Lever_Data) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);

        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }
        if ((wk->wu.routine_no[1] == 4) && (wk->wu.cg_type != 0x40)) {
            break;
        }

        hi_jump_flag_clear(wk->wu.id);
        Continue_Menu[wk->wu.id] = 0;

        wk->wu.hf.hit.player = 0;
        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        /* Fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (--Combo_Speed[wk->wu.id] != 0) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);

        Jump_Init(wk, Jump_Dir);
        if (Check_Diagonal_Shell(wk) != 0) {
            Next_Be_Free(wk);
        }

        break;

    case 2:
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
        } else {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        }
        break;

    case 3:
        Check_Air_Guard(wk);
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        if (Check_VS_Air_Attack(wk, Range_JX, Range_JY, J_Lever_Data) != 0) {
            break;
        }
        Check_Term_ABS_Distance(wk);

        if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], Range_X) == 0) {
            break;
        }
        if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, Range_Y) == 0) {
            break;
        }
        if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, Range_Y) == 0) {
            break;
        }

        Lever_Data = Check_SP_Jump_Attack(wk, Lever_Data);
        Lever_Buff[wk->wu.id] = Lever_Data;

        CP_Index[wk->wu.id][1]++;
        Stock_Hit_Flag[wk->wu.id] = 0;
        break;

    case 4:
        Check_Air_Guard(wk);
        if (wk->wu.hf.hit.player) {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        }
        Check_Landed(wk, Reaction & 0x7F);
        break;

    case 5:
        if (Check_Landed(wk, Reaction & 0x7F) == 0) {
            switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {
            default:
            case 1:
            case 10:
                if (Command_Type_00(wk, 8, 0xFFFF, -1) == -1) {
                    CP_Index[wk->wu.id][1] = 0x63;
                }
            }
        }
        break;
    default:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Check_Landed(wk, Reaction & 0xFFF);
        break;
    }
    if (CP_Index[wk->wu.id][1] >= 3) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}

s32 Check_SP_Jump_Attack(PLW* wk, s16 Lever_Data) {
    u16 xx;

    if (!(Lever_Data & 0x8000)) {
        return Lever_Data;
    }

    xx = Setup_Guard_Lever(wk, 0);
    xx |= Lever_Data & 0x7FFF;
    return xx | 2;
}

s32 Check_VS_Air_Attack(PLW* wk, s16 Range_JX, s16 Range_JY, s16 J_Lever_Data) {
    WORK* em;

    if ((Range_JX == -1) && (Range_JY == -1)) {
        return 0;
    }
    if (J_Lever_Data == -1) {
        return 0;
    }

    em = (WORK*)wk->wu.target_adrs;
    if ((em->pat_status != 0xE) && (em->pat_status != 0x14) && (em->pat_status != 0x1A) && (em->xyz[1].disp.pos <= 0)) {
        return 0;
    }

    if (Check_Term_Sub_Air(wk, PL_Distance[wk->wu.id], Range_JX) == 0) {
        return 0;
    }
    if (Check_Term_Sub(wk, Ck_Distance_Height(wk), Range_JY) != 0) {
        switch (CP_Index[wk->wu.id][2]) {
        case 0:
            CP_Index[wk->wu.id][2]++;
            Timer_01[wk->wu.id] = Select_Reflection_Time(wk);
            Timer_01[wk->wu.id]++;
            break;
        default:
            if (--Timer_01[wk->wu.id] != 0) {
                break;
            }

            if (J_Lever_Data & 0x4000) {
                CP_Index[wk->wu.id][1] += 2;
                if (cmd_sel[wk->wu.id]) {
                    Tech_Address[wk->wu.id] = player_CMD[wk->player_number][J_Lever_Data & 0x3FFF];
                } else {
                    Tech_Address[wk->wu.id] = player_cmd[wk->player_number][J_Lever_Data & 0x3FFF];
                }
                Continue_Menu[wk->wu.id] = 1;
                return -1;
            }

            Lever_Buff[wk->wu.id] = J_Lever_Data;
            CP_Index[wk->wu.id][1]++;
            Continue_Menu[wk->wu.id] = 1;
            return 1;
        }
    }

    return 0;
}

void Hi_Jump_Attack(PLW* wk, s16 Reaction, s16 Time_Data, u16 Lever_Data, s16 Jump_Dir) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }
        if (Check_Start_Hi_Jump(wk) == 0) {
            Continue_Menu[wk->wu.id] = 0;
            wk->wu.hf.hit.player = 0;
            CP_Index[wk->wu.id][1]++;
            if (cmd_sel[wk->wu.id]) {
                Tech_Address[wk->wu.id] = player_CMD[wk->player_number][2];
            } else {
                Tech_Address[wk->wu.id] = player_cmd[wk->player_number][2];
            }
            Timer_00[wk->wu.id] = Time_Data;
            Check_First_Menu(wk);
        }

        break;

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (--Combo_Speed[wk->wu.id] != 0) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        Tech_Index[wk->wu.id] = 0xC;

        dash_flag_clear(wk->wu.id);
        Jump_Init(wk, Jump_Dir);
        Lever_Pool[wk->wu.id] &= 0xC;
        Lever_Buff[wk->wu.id] = 0;
        Check_Air_Guard(wk);
        if (Check_Diagonal_Shell(wk) != 0) {
            Next_Be_Free(wk);
        }

        break;

    case 2:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (Command_Type_00(wk, 8, 0xFFFF, -1) == -1) {
            CP_Index[wk->wu.id][1]++;
            Lever_Buff[wk->wu.id] |= Lever_Pool[wk->wu.id];
            break;
        }

        if (Lever_Buff[wk->wu.id] & 2) {
            return;
        }
        Lever_Buff[wk->wu.id] |= Lever_Pool[wk->wu.id];

        break;

    case 3:
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
        }

        else {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id] | 1;
        }

        break;

    case 4:
        Check_Air_Guard(wk);

        if (--Timer_00[wk->wu.id] != 0) {
            break;
        }

        Lever_Data = Check_SP_Jump_Attack(wk, Lever_Data);
        Lever_Buff[wk->wu.id] = Lever_Data;
        CP_Index[wk->wu.id][1] += 2;
        if (Reaction & 0x80) {
            CP_Index[wk->wu.id][1]++;
        }

        break;

    case 6:
        Check_Air_Guard(wk);
        if (Attack_Flag[wk->wu.id]) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        if (wk->wu.hf.hit.player == 0) {
            break;
        }

        if (!(wk->wu.cg_cancel & 8)) {
            break;
        }

        Lever_Buff[wk->wu.id] = Get_Meoshi_Data(wk);

        break;

    default:
        if (wk->wu.hf.hit.player) {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        }
        Check_Landed(wk, Reaction & 0xFFF);
        break;
    }
    if (CP_Index[wk->wu.id][1] >= 4) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}

void Hi_Jump_Attack_Term(PLW* wk, s16 Range_X, s16 Range_Y, s16 Reaction, u16 Lever_Data, s16 Jump_Dir, s16 Range_JX,
                         s16 Range_JY, u16 J_Lever_Data) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }
        if (Check_Start_Hi_Jump(wk) != 0) {
            break;
        }

        Continue_Menu[wk->wu.id] = 0;
        wk->wu.hf.hit.player = 0;
        CP_Index[wk->wu.id][1]++;
        if (cmd_sel[wk->wu.id]) {
            Tech_Address[wk->wu.id] = player_CMD[wk->player_number][2];
        } else {
            Tech_Address[wk->wu.id] = player_cmd[wk->player_number][2];
        }
        Check_First_Menu(wk);

        break;

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id] != 0) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        Tech_Index[wk->wu.id] = 0xC;

        dash_flag_clear(wk->wu.id);
        Jump_Init(wk, Jump_Dir);
        Lever_Pool[wk->wu.id] &= 0xC;
        Lever_Buff[wk->wu.id] = 0;
        if (Check_Diagonal_Shell(wk) != 0) {
            Next_Be_Free(wk);
        }

        break;

    case 2:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (Command_Type_00(wk, 8, 0xFFFF, -1) == -1) {
            CP_Index[wk->wu.id][1]++;
            Lever_Buff[wk->wu.id] |= Lever_Pool[wk->wu.id];
        } else {
            if (Lever_Buff[wk->wu.id] & 2) {
                return;
            }
            Lever_Buff[wk->wu.id] |= Lever_Pool[wk->wu.id];
        }
        break;

    case 3:
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
        }

        else {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id] | 1;
        }

        break;

    case 4:
        Check_Air_Guard(wk);
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        if (Check_VS_Air_Attack(wk, Range_JX, Range_JY, J_Lever_Data) != 0) {
            break;
        }

        if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], Range_X) == 0) {
            break;
        }
        if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, Range_Y) == 0) {
            break;
        }
        if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, Range_Y) == 0) {
            break;
        }

        Lever_Data = Check_SP_Jump_Attack(wk, Lever_Data);
        Lever_Buff[wk->wu.id] = Lever_Data;
        CP_Index[wk->wu.id][1]++;
        if (Reaction & 0x80) {
            CP_Index[wk->wu.id][1] = 8;
        }
        break;

    case 5:
        if (wk->wu.hf.hit.player) {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        }
        Check_Landed(wk, Reaction & 0xFFF);
        break;

    case 6:
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {
        default:
        case 1:
        case 10:

            if (Command_Type_00(wk, 8, 0xFFFF, -1) == -1) {
                CP_Index[wk->wu.id][1] = 0x63;
            }
        }

        break;

    case 7:
        if (--Combo_Speed[wk->wu.id]) {
            break;
        }
        Lever_Buff[wk->wu.id] = Tech_Address[wk->wu.id][8];
        CP_Index[wk->wu.id][1]++;
        break;

    case 8:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Check_Landed(wk, Reaction & 0xFFF);
        break;

    default:
        if (Attack_Flag[wk->wu.id]) {
            break;
        }

        CP_Index[wk->wu.id][1] = 8;
        if (wk->wu.hf.hit.player == 0) {
            break;
        }

        if (wk->wu.cg_cancel & 8) {
            Lever_Buff[wk->wu.id] = Get_Meoshi_Data(wk);
        }
        break;
    }
    if (CP_Index[wk->wu.id][1] >= 4) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}

s32 Check_Term_ABS_Distance(PLW* wk) {
    if (Turn_Over[wk->wu.id]) {
        return 1;
    }

    if (My_char[wk->wu.id] == 5) {
        return 0;
    }

    if (PL_Distance[wk->wu.id] < 0x31) {
        return 1;
    }

    if (wk->wu.mvxy.a[1].real.h >= 0) {
        return 0;
    }

    if (wk->wu.xyz[1].disp.pos < 0x31) {
        return 1;
    }

    return 0;
}

s32 Check_Com_Add_Y(PLW* wk, s16 Pos_Y, s16 Range) {
    if (Range == -1) {
        return 1;
    }
    if (!(Range & 0x8000)) {
        if (Pos_Y >= Range) {
            return 1;
        }
        return 0;
    } else {
        if (wk->wu.mvxy.a[1].real.h >= 0) {
            return 0;
        }
        if (Pos_Y <= (Range & 0x7FFF)) {
            return 1;
        }
        return 0;
    }
}

void ORO_JA_Term(PLW* wk, s16 Reaction, s16 Jump_Dir, s16 JY, s16 Jump_Dir2, s16 RX, s16 RY, u16 Lever_Data, s16 RJX,
                 s16 RJY, u16 JLD) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }

        Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);

        if ((wk->wu.routine_no[1] == 4) && (wk->wu.cg_type != 0x40)) {
            break;
        }

        hi_jump_flag_clear(wk->wu.id);
        Continue_Menu[wk->wu.id] = 0;

        wk->wu.hf.hit.player = 0;
        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id] != 0) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);

        Jump_Init(wk, Jump_Dir);
        Check_Air_Guard(wk);
        if (Check_Diagonal_Shell(wk) != 0) {
            Next_Be_Free(wk);
        }

        break;

    case 2:
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
        } else {

            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
            Timer_00[wk->wu.id] = 2;
        }
        break;

    case 3:
        Check_Air_Guard(wk);

        if (Lever_Data != 0xFFFF) {
            if (Check_Landed(wk, Reaction) != 0) {
                break;
            }
            if (Check_VS_Air_Attack(wk, RJX, RJY, JLD) != 0) {
                break;
            }
            if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, JY) == 0) {
                break;
            }
        }
        if ((wk->air_jump_ok_time == 0) && (wk->wu.position_y >= 0x30)) {
            Jump_Init(wk, Jump_Dir2);

            if ((Lever_Data) == 0xFFFF) {
                CP_Index[wk->wu.id][1] += 2;
            } else {
                CP_Index[wk->wu.id][1]++;
            }
        }

        break;

    case 4:
        Check_Air_Guard(wk);
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        if (Check_VS_Air_Attack(wk, RJX, RJY, JLD) != 0) {
            break;
        }
        if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], RX) == 0) {
            break;
        }
        if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
            break;
        }
        if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
            break;
        }

        Lever_Buff[wk->wu.id] = Lever_Data;

        CP_Index[wk->wu.id][1]++;
        Stock_Hit_Flag[wk->wu.id] = 0;
        break;

    case 5:
        if (wk->wu.hf.hit.player) {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        }
        Check_Landed(wk, Reaction & 0x7F);
        break;

    case 6:
        if (Check_Landed(wk, Reaction & 0x7F) != 0) {
            break;
        }

        switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {
        default:
        case 1:
        case 10:
            if (Command_Type_00(wk, 8, 0xFFFF, -1) == -1) {
                CP_Index[wk->wu.id][1] = 0x63;
            }
        }

        break;
    default:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Check_Landed(wk, Reaction & 0xFFF);
        break;
    }
    if (CP_Index[wk->wu.id][1] >= 3) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}

void ORO_HJA_Term(PLW* wk, s16 Reaction, s16 Jump_Dir, s16 JY, s16 Jump_Dir2, s16 RX, s16 RY, u16 Lever_Data, s16 RJX,
                  s16 RJY, u16 JLD) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }
        if (Check_Start_Hi_Jump(wk) == 0) {
            Continue_Menu[wk->wu.id] = 0;
            CP_Index[wk->wu.id][1]++;
            if (cmd_sel[wk->wu.id]) {
                Tech_Address[wk->wu.id] = player_CMD[wk->player_number][2];
            } else {
                Tech_Address[wk->wu.id] = player_cmd[wk->player_number][2];
            }
            Check_First_Menu(wk);
        }
        break;

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id == 0]) {
            CP_Index[wk->wu.id][1]++;
            Tech_Index[wk->wu.id] = 0xC;

            Jump_Init(wk, Jump_Dir);
            Lever_Pool[wk->wu.id] &= 0xC;
            Lever_Buff[wk->wu.id] = 0;
            Check_Air_Guard(wk);
            if (Check_Diagonal_Shell(wk) != 0) {
                Next_Be_Free(wk);
            }
        }
        break;

    case 2:
        if (Check_Passive(wk) != 0) {
            break;
        }
        CP_Index[wk->wu.id][1]++;
        Lever_Buff[wk->wu.id] = 2;
        Lever_Pool[wk->wu.id] |= 1;
        break;

    case 3:
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
        } else {

            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
            Timer_00[wk->wu.id] = 2;
        }
        break;

    case 4:
        Check_Air_Guard(wk);

        if (Lever_Data != 0xFFFF) {
            if (Check_Landed(wk, Reaction) != 0) {
                break;
            }
            if (Check_VS_Air_Attack(wk, RJX, RJY, JLD) != 0) {
                break;
            }
            if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, JY) == 0) {
                break;
            }
        }
        if ((wk->air_jump_ok_time == 0) && (wk->wu.position_y >= 0x30)) {
            Jump_Init(wk, Jump_Dir2);

            if ((Lever_Data) == 0xFFFF) {
                CP_Index[wk->wu.id][1] += 2;
            } else {
                CP_Index[wk->wu.id][1]++;
            }
        }

        break;

    case 5:
        Check_Air_Guard(wk);
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        if (Check_VS_Air_Attack(wk, RJX, RJY, JLD) != 0) {
            break;
        }
        if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], RX) == 0) {
            break;
        }
        if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
            break;
        }
        if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
            break;
        }

        Lever_Buff[wk->wu.id] = Lever_Data;

        CP_Index[wk->wu.id][1]++;
        Stock_Hit_Flag[wk->wu.id] = 0;
        break;

    case 6:
        if (wk->wu.hf.hit.player) {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        }
        Check_Landed(wk, Reaction & 0x7F);
        break;

    case 7:
        if (Check_Landed(wk, Reaction & 0x7F) != 0) {
            break;
        }

        switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {
        default:
        case 1:
        case 10:
            if (Command_Type_00(wk, 8, 0xFFFF, -1) == -1) {
                CP_Index[wk->wu.id][1] = 0x63;
            }
        }

        break;
    default:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Check_Landed(wk, Reaction & 0xFFF);
        break;
    }
    if (CP_Index[wk->wu.id][1] >= 3) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}

void Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        dash_flag_clear(wk->wu.id);
        if (cmd_sel[wk->wu.id]) {
            Tech_Address[wk->wu.id] = player_CMD[wk->player_number][Tech_Number & 0xFF];
        } else {
            Tech_Address[wk->wu.id] = player_cmd[wk->player_number][Tech_Number & 0xFF];
        }
        Tech_Index[wk->wu.id] = 0xC;
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

        if (Check_Start_Command_Attack(wk, Reaction, Tech_Number & 0x80FF) != 0) {
            break;
        }
        if (Check_Dash_Hit(wk, Tech_Number & 0x80FF) != 0) {
            Next_Be_Free(wk);
        }

        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);

        if (Power_Level & 0x4000) {
            Free_Lever[wk->wu.id] = Power_Lv_Data[(Power_Level & 0xF) - 8];
        } else {
            Free_Lever[wk->wu.id] = 0;
        }
        /* Fallthrough */

    case 1:
        if (--Combo_Speed[wk->wu.id]) {
            Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
            break;
        }

        if (Hadou_Check(wk, Tech_Number & 0x80FF) != 0) {
            if (Check_Passive(wk) == 0) {
                Combo_Speed[wk->wu.id] = 1;
            }
            break;
        } else {
            CP_Index[wk->wu.id][1]++;
            Check_Rapid(wk, Tech_Number);
        }
        /* Fallthrough */
    case 2:
        if (Check_Passive(wk) != 0) {
            break;
        }
        switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {

        default:
        case 1:

            if (Command_Type_00(wk, Power_Level & 0xF, Tech_Number, Ex_Shot) == -1) {
                if ((Tech_Number & 0xF) == 0 || (Tech_Number & 0xF) == 1) {
                    if (Reaction == 0xC) {
                        CP_Index[wk->wu.id][1] = 0x63;
                        Timer_00[wk->wu.id] = Dash_Time_Data[wk->player_number][Tech_Number];
                    } else {
                        CP_Index[wk->wu.id][1] = 4;
                    }
                } else {
                    CP_Index[wk->wu.id][1] = 3;
                }
            }
            break;

        case 2:
            if (Command_Type_01(wk, Power_Level & 0xF, Ex_Shot) != 0) {
                CP_Index[wk->wu.id][1]++;
            }
            break;

        case 7:
            if (Command_Type_06(wk, Power_Level & 0xF, Tech_Number, Ex_Shot) != 0) {
                CP_Index[wk->wu.id][1]++;
            }
            break;
        }
        break;

    case 3:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (gs.plw[wk->wu.id].tsukami_f) {
            break;
        }
        if (((wk->wu.cg_type) == 0x40) || (wk->wu.routine_no[1] == 0)) {
            Reaction_Exit_Sub(wk);
        } else {
            Lever_Buff[wk->wu.id] = Free_Lever[wk->wu.id];
            Rapid_Sub(wk);
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
            Reaction_Sub(wk, Reaction, Power_Level);
        }
        break;

    case 4:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (((wk->wu.cg_type) == 0x40) || (wk->running_f == 0)) {
            Reaction_Exit_Sub(wk);
        }
        break;

    case 5:
        if (PL_Distance[wk->wu.id] > 0x70) {
            Lever_Buff[wk->wu.id] = 0x40;
        }
        if (((wk->wu.cg_type) == 0x40) || (wk->wu.routine_no[1] == 0)) {
            Reaction_Exit_Sub(wk);
        }
        break;

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (--Timer_00[wk->wu.id] == 0) {
            Reaction_Exit_Sub(wk);
        }
        break;
    }
}

const s32 Hadou_Check_Data[20][2] = {
    { 0, 0 },    { 0, 0 }, { 1, 0x1D }, { 0, 0 },    { 1, 0x1F }, { 0, 0 }, { 1, 0x20 },
    { 1, 0x21 }, { 0, 0 }, { 0, 0 },    { 1, 0x1F }, { 1, 0x1D }, { 0, 0 }, { 0, 0 },
    { 1, 0x1F }, { 0, 0 }, { 1, 0x1E }, { 0, 0 },    { 0, 0 },    { 0, 0 },
};

s32 Hadou_Check(PLW* wk, u16 Tech_Number) {
    if (Hadou_Check_Data[wk->player_number][0] == 0) {
        return 0;
    }

    if (Tech_Number != Hadou_Check_Data[wk->player_number][1]) {
        return 0;
    }

    return Check_Resume_Lever(wk);
}

s32 Check_Resume_Lever(PLW* wk) {
    u16 Target_Lever;
    s16 xx;

    if (wk->wu.rl_waza) {
        Target_Lever = 8;
    } else {
        Target_Lever = 4;
    }

    for (xx = 0; xx <= 8; xx++) {
        if (Target_Lever == Resume_Lever[wk->wu.id][xx]) {
            return 1;
        }
    }
    return 0;
}

void J_Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }

        dash_flag_clear(wk->wu.id);

        if (cmd_sel[wk->wu.id]) {
            Tech_Address[wk->wu.id] = player_CMD[wk->player_number][Tech_Number & 0xFF];
        } else {
            Tech_Address[wk->wu.id] = player_cmd[wk->player_number][Tech_Number & 0xFF];
        }

        Tech_Index[wk->wu.id] = 0xC;
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

        if (Check_Start_Command_Attack(wk, Reaction, Tech_Number & 0x80FF) != 0) {
            break;
        }
        if (Check_Dash_Hit(wk, Tech_Number & 0x80FF) != 0) {
            Next_Be_Free(wk);
        }

        Continue_Menu[wk->wu.id] = 0;
        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        /* Fallthough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (--Combo_Speed[wk->wu.id]) {
            Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
            break;
        }
        if (Check_Diagonal_Shell(wk) != 0) {
            Next_Be_Free(wk);
            break;
        }
        CP_Index[wk->wu.id][1]++;
        /* Fallthough */
    case 2:
        if (Check_Passive(wk) != 0) {
            break;
        }
        switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {

        case 2:
            if (Command_Type_01(wk, Power_Level & 0xF, Ex_Shot) != 0) {
                CP_Index[wk->wu.id][1]++;
            }
            break;

        default:
        case 1:
        case 10:
            if (Command_Type_00(wk, Power_Level & 0xF, Tech_Number, Ex_Shot) == -1) {
                CP_Index[wk->wu.id][1] = 0x63;
            }
            break;
        }
        break;

    case 3:
        Check_Rapid(wk, Tech_Number);
        CP_Index[wk->wu.id][1]++;
        return;

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        if ((wk->wu.cg_type == 0x40) || (wk->wu.routine_no[1] == 0)) {
            Reaction_Exit_Sub(wk);
        } else {
            Rapid_Sub(wk);
            if (Reaction == 0xC) {
                Reaction_Sub(wk, Reaction, Power_Level);
                break;
            }
            Check_Landed(wk, Reaction & 0xFFF);
        }
        break;
    }
}

void Rapid_Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Shot, u16 Time) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        dash_flag_clear(wk->wu.id);
        if (cmd_sel[wk->wu.id]) {
            Tech_Address[wk->wu.id] = player_CMD[wk->player_number][Tech_Number & 0xFF];
        } else {
            Tech_Address[wk->wu.id] = player_cmd[wk->player_number][Tech_Number & 0xFF];
        }
        Tech_Index[wk->wu.id] = 0xC;
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

        if (Check_Start_Command_Attack(wk, Reaction, Tech_Number & 0x80FF) != 0) {
            break;
        }
        if (Check_Dash_Hit(wk, Tech_Number & 0x80FF) != 0) {
            Next_Be_Free(wk);
        }

        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        Free_Lever[wk->wu.id] = 0;
        /* Fallthough */

    case 1:
        if (--Combo_Speed[wk->wu.id]) {
            Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
            break;
        }
        CP_Index[wk->wu.id][1]++;

        Rapid_No[wk->wu.id][0] = 0;
        Rapid_No[wk->wu.id][1] = 0;
        Timer_00[wk->wu.id] = Time;

    case 2:
        switch (Rapid_No[wk->wu.id][0]) {

        case 0:
            Rapid_No[wk->wu.id][0] = 1;
            Lever_Buff[wk->wu.id] = Shot;
            break;
        case 1:
            Rapid_No[wk->wu.id][0] = 0;
            Lever_Buff[wk->wu.id] = 0;
            break;
        }
        if (wk->wu.sp_tech_id == Tech_Number) {
            CP_Index[wk->wu.id][1] = 3;
        }
        break;

    case 3:
        if (--Timer_00[wk->wu.id] == 0) {
            CP_Index[wk->wu.id][1] = 4;
        }

        else {
            switch (Rapid_No[wk->wu.id][0]) {

            case 0:
                Rapid_No[wk->wu.id][0] = 1;
                Lever_Buff[wk->wu.id] = Shot;
                break;
            case 1:
                Rapid_No[wk->wu.id][0] = 0;
                Lever_Buff[wk->wu.id] = 0;
                break;
            }
            if (wk->wu.sp_tech_id != Tech_Number) {
                CP_Index[wk->wu.id][1] = 4;
            }
        }
        break;

    case 4:
        if (wk->wu.sp_tech_id == Tech_Number) {
            break;
        }
        if ((wk->wu.cg_type == 0x40) || (wk->wu.routine_no[1] == 0)) {
            Reaction_Exit_Sub(wk);
        }
        break;
    }
}

void Check_Rapid(PLW* wk, u16 Tech_Number) {
    if (!(Tech_Number & 0xF00)) {
        Rapid_No[wk->wu.id][0] = 0;
    } else {
        Lv = Setup_Lv08(0);
        if (Break_Into_CPU == 2) {
            Lv = 7;
        }
        Rapid_No[wk->wu.id][0] = Rapid_SA_Data[emLevelRemake(Lv, 8, 0)][random_32_com() & 7];
        Rapid_No[wk->wu.id][1] = 0;
        Setup_Rapid_End_Term(wk, Tech_Number);
        Rapid_Index[wk->wu.id] = 0xFF0;
    }
}

void Setup_Rapid_End_Term(PLW* wk, s16 Tech_Number) {
    Rapid_No[wk->wu.id][2] = (Tech_Number & 0xF00) >> 8;
    if ((Tech_Number & 0xF00) == 0x400) {
        Rapid_No[wk->wu.id][3] = Setup_Rapid_Time(wk, Tech_Number);
    }
}

s32 Setup_Rapid_Time(PLW* wk, u16 Tech_Number) {
    return 60;
}

void Rapid_Sub(PLW* wk) {
    if (Check_Rapid_End(wk) != 0) {
        return;
    }

    switch (Rapid_No[wk->wu.id][0]) {
    case 0:
        break;
    case 2:
        switch (Rapid_No[wk->wu.id][1]) {
        case 0:
            Rapid_No[wk->wu.id][1]++;
            Timer_00[wk->wu.id] = 1;
            Timer_01[wk->wu.id] = 3;
            return;
        case 1:
            if (--Timer_00[wk->wu.id] == 0) {
                Lever_Buff[wk->wu.id] = Rapid_Index[wk->wu.id];
                Timer_00[wk->wu.id] = 2;

                if (--Timer_01[wk->wu.id] == 0) {
                    Rapid_No[wk->wu.id][1]++;
                    Timer_01[wk->wu.id] = 0x18;
                }
            }
            break;
        case 2:
            if (--Timer_01[wk->wu.id] == 0) {
                Rapid_No[wk->wu.id][1]++;
                Timer_00[wk->wu.id] = 1;
                Timer_01[wk->wu.id] = 2;
            }
            break;
        default:
            if (--Timer_00[wk->wu.id] == 0) {
                Lever_Buff[wk->wu.id] = Rapid_Index[wk->wu.id];
                Timer_00[wk->wu.id] = Timer_01[wk->wu.id];
            }
            break;
        }
        break;
    default:
        switch (Rapid_No[wk->wu.id][1]) {
        case 0:
            Rapid_No[wk->wu.id][1]++;
            Timer_00[wk->wu.id] = 1;
            Timer_01[wk->wu.id] = 2;
            break;
        default:
            if (--Timer_00[wk->wu.id] == 0) {
                Lever_Buff[wk->wu.id] = Rapid_Index[wk->wu.id];
                Timer_00[wk->wu.id] = Timer_01[wk->wu.id];
            }
            break;
        }
        break;
    }
}

s32 Check_Rapid_End(PLW* wk) {
    switch (Rapid_No[wk->wu.id][2]) {
    case 1:
        if (wk->wu.mvxy.a[1].real.h < 0) {
            Rapid_No[wk->wu.id][0] = 0;
            return 1;
        }
        break;
    case 2:
        switch (Rapid_No[wk->wu.id][3]) {
        case 0:
            if (gs.plw[wk->wu.id].caution_flag) {
                Rapid_No[wk->wu.id][3]++;
            }
            break;
        case 1:
            if (gs.plw[wk->wu.id].caution_flag == 0) {
                Rapid_No[wk->wu.id][0] = 0;
                return 1;
            }
            break;
        }
        break;
    case 4:
        switch (Rapid_No[wk->wu.id][3]) {
        case 0:
            if (wk->wu.cg_ja.atix) {
                Rapid_No[wk->wu.id][3]++;
            }
            break;
        case 1:
            if (--Rapid_No[wk->wu.id][3] == 0) {
                Rapid_No[wk->wu.id][0] = 0;
                return 1;
            }
            break;
        }
        break;
    }
    return 0;
}

s32 Check_Start_Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number) {
    if (Before_Jump[wk->wu.id]) {
        return Before_Jump[wk->wu.id] = 0;
    }

    if (wk->wu.routine_no[1] == 2) {
        return 1;
    }
    if ((wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40)) {
        return 0;
    }

    if ((Tech_Number == 0) || (Tech_Number == 1)) {
        if (wk->wu.cg_cancel & 2) {
            return 0;
        }
        return 1;
    }
    if ((Tech_Number & 0x8000) && (wk->wu.cg_cancel & 0x40)) {
        return 0;
    }
    if (wk->wu.cg_cancel & 0x20) {
        return 0;
    }
    if ((wk->wu.cg_cancel & 8) && (Reaction == 0xE)) {
        return 0;
    }
    return 1;
}

void ORO_JCA_Term(PLW* wk, s16 Reaction, s16 Jump_Dir, s16 JY, s16 Jump_Dir2, s16 RX, s16 RY, u16 Tech_Number,
                  s16 Power_Level, s16 Ex_Shot, s16 RJX, s16 RJY, u16 JLD) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if ((wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40)) {
            Continue_Menu[wk->wu.id] = 0;
            CP_Index[wk->wu.id][1]++;
            if (cmd_sel[wk->wu.id]) {
                Tech_Address[wk->wu.id] = player_CMD[wk->player_number][Tech_Number & 0xFF];
            } else {
                Tech_Address[wk->wu.id] = player_cmd[wk->player_number][Tech_Number & 0xFF];
            }
            Check_First_Menu(wk);
        }

        break;

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id] == 0) {
            CP_Index[wk->wu.id][1]++;
            Tech_Index[wk->wu.id] = 0xC;
            dash_flag_clear(wk->wu.id);

            Jump_Init(wk, Jump_Dir);
            Check_Air_Guard(wk);
            if (Check_Diagonal_Shell(wk) != 0) {
                Next_Be_Free(wk);
            }
        }
        break;

    case 2:
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
        }

        else {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
            Timer_00[wk->wu.id] = 2;
        }
        break;

    case 3:
        Check_Air_Guard(wk);

        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }
        if (Check_VS_Air_Attack(wk, RJX, RJY, JLD) != 0) {
            break;
        }
        if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, JY) == 0) {
            break;
        }

        Jump_Init(wk, Jump_Dir2);
        if (--Timer_00[wk->wu.id] == 0) {
            CP_Index[wk->wu.id][1]++;
        }
        break;

    case 4:
        Check_Air_Guard(wk);
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        if (Check_VS_Air_Attack(wk, RJX, RJY, JLD) != 0) {
            break;
        }
        if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], RX) == 0) {
            break;
        }
        if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
            break;
        }
        if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
            break;
        }
        CP_Index[wk->wu.id][1] += 2;
        break;

    case 5:
        if (wk->wu.hf.hit.player) {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        }
        Check_Landed(wk, Reaction & 0x7F);
        break;

    case 6:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {
        default:
        case 1:
        case 10:
            if (Command_Type_00(wk, Power_Level & 0xF, Tech_Number, Ex_Shot) == -1) {
                CP_Index[wk->wu.id][1] = 0x63;
            }
            break;
        }
        break;
    default:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Rapid_Sub(wk);
        Check_Landed(wk, Reaction & 0xFFF);
        break;
    }
}

void ORO_HJCA_Term(PLW* wk, s16 Reaction, s16 Jump_Dir, s16 JY, s16 Jump_Dir2, s16 RX, s16 RY, u16 Tech_Number,
                   s16 Power_Level, s16 Ex_Shot, s16 RJX, s16 RJY, u16 JLD) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }
        if (Check_Start_Hi_Jump(wk) != 0) {
            break;
        }

        Continue_Menu[wk->wu.id] = 0;
        CP_Index[wk->wu.id][1]++;
        if (cmd_sel[wk->wu.id]) {
            Tech_Address[wk->wu.id] = player_CMD[wk->player_number][Tech_Number & 0x7FFF];
        } else {
            Tech_Address[wk->wu.id] = player_cmd[wk->player_number][Tech_Number & 0x7FFF];
        }
        Check_First_Menu(wk);

        break;

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id == 0]) {
            CP_Index[wk->wu.id][1]++;
            Tech_Index[wk->wu.id] = 0xC;

            Jump_Init(wk, Jump_Dir);
            Lever_Pool[wk->wu.id] &= 0xC;
            Lever_Buff[wk->wu.id] = 0;
            Check_Air_Guard(wk);
            if (Check_Diagonal_Shell(wk) != 0) {
                Next_Be_Free(wk);
            }
        }
        break;

    case 2:
        if (Check_Passive(wk) != 0) {
            break;
        }
        CP_Index[wk->wu.id][1]++;
        Lever_Buff[wk->wu.id] = 2;
        Lever_Pool[wk->wu.id] |= 1;
        break;

    case 3:
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
        }

        else {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
            Timer_00[wk->wu.id] = 2;
        }
        break;

    case 4:
        Check_Air_Guard(wk);

        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }
        if (Check_VS_Air_Attack(wk, RJX, RJY, JLD) != 0) {
            break;
        }
        if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, JY) == 0) {
            break;
        }

        Jump_Init(wk, Jump_Dir2);
        if (--Timer_00[wk->wu.id] == 0) {
            CP_Index[wk->wu.id][1]++;
        }
        break;

    case 5:
        Check_Air_Guard(wk);
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        if (Check_VS_Air_Attack(wk, RJX, RJY, JLD) != 0) {
            break;
        }
        if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], RX) == 0) {
            break;
        }
        if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
            break;
        }
        if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
            break;
        }
        CP_Index[wk->wu.id][1] += 2;
        break;

    case 6:
        if (wk->wu.hf.hit.player) {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        }
        Check_Landed(wk, Reaction & 0x7F);
        return;

    case 7:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {
        default:
        case 1:
        case 10:
            if (Command_Type_00(wk, Power_Level & 0xF, Tech_Number, Ex_Shot) == -1) {
                CP_Index[wk->wu.id][1] = 0x63;
            }
            break;
        }
        break;
    default:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Rapid_Sub(wk);
        Check_Landed(wk, Reaction & 0xFFF);
        break;
    }
}

void Jump_Command_Attack_Term(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot, s16 RX, s16 RY,
                              s16 Jump_Dir, s16 JRX, s16 JRY, u16 JLD) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if ((wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40)) {
            Continue_Menu[wk->wu.id] = 0;
            CP_Index[wk->wu.id][1]++;
            if (cmd_sel[wk->wu.id]) {
                Tech_Address[wk->wu.id] = player_CMD[wk->player_number][Tech_Number & 0xFF];
            } else {
                Tech_Address[wk->wu.id] = player_cmd[wk->player_number][Tech_Number & 0xFF];
            }
            Check_First_Menu(wk);
        }

        break;

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id] == 0) {
            CP_Index[wk->wu.id][1]++;
            Tech_Index[wk->wu.id] = 0xC;

            Jump_Init(wk, Jump_Dir);
            Check_Rapid(wk, Tech_Number);

            if (Check_Diagonal_Shell(wk) != 0) {
                Next_Be_Free(wk);
            }
        }
        break;

    case 2:
        Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
        }
        break;

    case 3:
        Check_Air_Guard(wk);
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        if (Check_VS_Air_Attack(wk, JRX, JRY, JLD) != 0) {
            break;
        }
        if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], RX) == 0) {
            break;
        }
        if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
            break;
        }
        if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
            break;
        }
        CP_Index[wk->wu.id][1] += 2;
        break;

    case 4:
        if (wk->wu.hf.hit.player) {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        }
        Check_Landed(wk, Reaction & 0xFFF);
        break;

    case 5:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {
        default:
        case 1:
        case 10:
            if (Command_Type_00(wk, Power_Level & 0xF, Tech_Number, Ex_Shot) == -1) {
                CP_Index[wk->wu.id][1] = 0x63;
            }
            break;
        }
        break;
    default:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Rapid_Sub(wk);
        Check_Landed(wk, Reaction & 0xFFF);
        break;
    }
}

void Hi_Jump_Command_Attack_Term(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot, s16 RX, s16 RY,
                                 s16 Jump_Dir, s16 JRX, s16 JRY, u16 JLD) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }
        if (Check_Start_Hi_Jump(wk) == 0) {
            Continue_Menu[wk->wu.id] = 0;
            CP_Index[wk->wu.id][1]++;
            if (cmd_sel[wk->wu.id]) {
                Tech_Address[wk->wu.id] = player_CMD[wk->player_number][Tech_Number & 0x7FFF];
            } else {
                Tech_Address[wk->wu.id] = player_cmd[wk->player_number][Tech_Number & 0x7FFF];
            }
            Check_First_Menu(wk);
        }

        break;

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id == 0]) {
            CP_Index[wk->wu.id][1]++;
            Tech_Index[wk->wu.id] = 0xC;

            Jump_Init(wk, Jump_Dir);
            Lever_Pool[wk->wu.id] &= 0xC;
            Lever_Buff[wk->wu.id] = 0;
            Check_Air_Guard(wk);
            if (Check_Diagonal_Shell(wk) != 0) {
                Next_Be_Free(wk);
            }
        }
        break;

    case 2:
        if (Check_Passive(wk) != 0) {
            break;
        }
        CP_Index[wk->wu.id][1]++;
        Lever_Buff[wk->wu.id] = 2;
        Lever_Pool[wk->wu.id] |= 1;
        break;

    case 3:
        Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id] | 1;
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
        }
        break;

    case 4:
        Check_Air_Guard(wk);
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        if (Check_VS_Air_Attack(wk, JRX, JRY, JLD) != 0) {
            break;
        }
        if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], RX) == 0) {
            break;
        }
        if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
            break;
        }
        if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
            break;
        }
        CP_Index[wk->wu.id][1] += 2;
        break;

    case 5:
        if (wk->wu.hf.hit.player) {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        }
        Check_Landed(wk, Reaction & 0xFFF);
        break;

    case 6:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (Check_Landed(wk, Reaction) != 0) {
            break;
        }

        switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {
        default:
        case 1:
        case 10:
            if (Command_Type_00(wk, Power_Level & 0xF, Tech_Number, Ex_Shot) == -1) {
                CP_Index[wk->wu.id][1] = 0x63;
            }
            break;
        }
        break;
    default:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Check_Landed(wk, Reaction & 0xFFF);
        break;
    }
}

s32 Check_Landed(PLW* wk, s16 Reaction) {
    if ((wk->wu.old_pos[1] != 0) && (wk->wu.xyz[1].disp.pos == 0)) {
        Lever_Buff[wk->wu.id] = 0;
        if (Continue_Menu[wk->wu.id]) {
            Next_End(wk);
            Before_Jump[wk->wu.id] = 1;
            return 1;
        } else {
            Reaction_Sub(wk, Reaction, 0);

            Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
            Check_Guard(wk);

            Before_Jump[wk->wu.id] = 1;
            return 1;
        }
    }
    if ((wk->wu.old_pos[1] == 0) && (wk->wu.xyz[1].disp.pos == 0) && (wk->wu.routine_no[1] != 4)) {
        Lever_Buff[wk->wu.id] = 0;
        if (Continue_Menu[wk->wu.id]) {
            Next_End(wk);
            Before_Jump[wk->wu.id] = 1;
            return 1;
        } else {
            Reaction_Sub(wk, Reaction, 0);
            if (Check_Guard(wk) != 0) {
                return 1;
            }
            Before_Jump[wk->wu.id] = 1;
            return 1;
        }
    }
    return 0;
}

s32 Check_Dash_Hit(PLW* wk, u16 Tech_Number) {
    WORK_Other* tmw;
    WORK* em;
    s16 i;
    s16 xx;
    s16 zz;

    if ((Tech_Number != 0) && (Tech_Number != 1)) {
        return 0;
    }

    em = (WORK*)wk->wu.target_adrs;
    for (i = 0; i < 8; i++) {
        if ((get_vs_shell_adrs(em, em->id, i, &tmw) == 0) && (get_vs_shell_adrs((WORK*)wk, em->id, i, &tmw) == 0)) {
            return 0;
        }
        if (tmw->wu.routine_no[1] == 2) {
            continue;
        }

        xx = wk->wu.xyz[0].disp.pos - tmw->wu.xyz[0].disp.pos;
        zz = Setup_Front_or_Back(wk, xx);

        if (Tech_Number == 0) {
            if (zz != 1) {
                if (Check_Hit_Shell(wk, tmw, Tech_Number) != 0) {
                    return 1;
                }
            }
        } else {
            if (zz != 0) {
                if (Check_Hit_Shell(wk, tmw, Tech_Number) != 0) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

s32 Setup_Front_or_Back(PLW* wk, s16 xx) {
    if (wk->wu.rl_waza == 0) {
        if (xx >= 0) {
            return 0;
        }
        return 1;
    } else {
        if (xx >= 0) {
            return 1;
        }
        return 0;
    }
}

s32 Check_Hit_Shell(PLW* wk, WORK_Other* tmw, u16 Tech_Number) {
    s16 xx;

    if (wk->wu.rl_waza == 1) {
        Tech_Number ^= 1;
    }

    if (Tech_Number == 0) {
        xx = wk->wu.xyz[0].disp.pos - Dash_Distance_Data[wk->player_number][Tech_Number];
        if (xx <= tmw->wu.xyz[0].disp.pos) {
            return 1;
        }
        return 0;
    } else {
        xx = wk->wu.xyz[0].disp.pos + Dash_Distance_Data[wk->player_number][Tech_Number];
        if (xx >= tmw->wu.xyz[0].disp.pos) {
            return 1;
        }
        return 0;
    }
}

void Jump_Init(PLW* wk, s16 Jump_Dir) {
    switch (Jump_Dir) {
    case 0:
        Ck_Distance_LvJ(wk);
        Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        break;
    case 2:
        Lever_Pool[wk->wu.id] = 1;
        Lever_Buff[wk->wu.id] = 1;
        break;

    default:
        Ck_Distance_LvJ(wk);
        Lever_Pool[wk->wu.id] ^= 0xC;
        Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        break;
    }
}

s32 Command_Type_00(PLW* wk, s16 Power_Level, u16 Tech_Number, s16 Ex_Shot) {
    if (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id] + 4] != 0x1C) {
        Lever_Buff[wk->wu.id] = Tech_Address[wk->wu.id][Tech_Index[wk->wu.id] + 3] & 0x7FFF;
        Lever_Buff[wk->wu.id] = datacmd_conpanecmd(Lever_Buff[wk->wu.id]);

        if (wk->wu.rl_waza) {
            if (Lever_Buff[wk->wu.id] & 0xC) {
                Lever_Buff[wk->wu.id] ^= 0xC;
            }
        }
        Tech_Index[wk->wu.id] += 4;
        return 1;
    } else {
        Lever_Buff[wk->wu.id] = Tech_Address[wk->wu.id][Tech_Index[wk->wu.id] + 3] & 0x7FFF;
        if (wk->wu.rl_waza) {
            if (Lever_Buff[wk->wu.id] & 0xC) {
                Lever_Buff[wk->wu.id] ^= 0xC;
            }
        }
        if (Tech_Address[wk->wu.id][7] == 0x80) {
            return -1;
        }
        Tech_Index[wk->wu.id] = 7;

        if ((gs.plw[wk->wu.id].sa->ex) && ((Ex_Shot == 0x70) || (Ex_Shot == 0x700))) {
            Lever_Buff[wk->wu.id] |= Ex_Shot;
        } else {
            Lever_Buff[wk->wu.id] |= renbanshot_conpaneshot(Tech_Address[wk->wu.id], Power_Level);
        }

        if ((My_char[wk->wu.id] == 2) && ((Tech_Number) == 0x8015) && (Power_Level != 8)) {
            CP_Index[wk->wu.id][0]++;
            Lever_LR[wk->wu.id] = Lever_Buff[wk->wu.id] & 0xFF0;

            if (Power_Level == 0xA) {
                CP_Index[wk->wu.id][1] = 1;
            } else {
                CP_Index[wk->wu.id][1] = 0;
            }
        } else {
            if (CP_No[wk->wu.id][0] == 0xA) {
                Rapid_Index[wk->wu.id] = Lever_Buff[wk->wu.id] & 0xFF0;
                Lever_Pool[wk->wu.id] = Lever_Buff[wk->wu.id] & 0xFF0;
            }
            if ((wk->player_number == 6) && ((Tech_Number) == 0x8016)) {
                CP_Index[wk->wu.id][1] = 5;
            } else {
                if ((wk->player_number == 7) && ((Tech_Number) == 0x1F)) {
                    Reaction_Exit_Sub(wk);
                } else {
                    CP_Index[wk->wu.id][1]++;
                }
            }
        }
        return 0;
    }
}

const u16 Rolling_Lv_Data[2][9] = {
    { 4, 1, 8, 2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF },
    { 4, 2, 8, 1, 4, 2, 8, 1, 0xFFFF },
};

s32 Command_Type_06(PLW* wk, s16 Power_Level, u16 Tech_Number, s16 Ex_Shot) {
    s16 xx;

    xx = 0;

    if (Tech_Number & 0x8000) {
        if ((My_char[wk->wu.id] == 6) && (Super_Arts[wk->wu.id] == 0)) {
            xx = 1;
        }
    }
    Lever_Buff[wk->wu.id] = Rolling_Lv_Data[xx][CP_Index[wk->wu.id][2]];
    CP_Index[wk->wu.id][2]++;

    if ((Rolling_Lv_Data[xx][CP_Index[wk->wu.id][2]]) == 0xFFFF) {
        Lever_Buff[wk->wu.id] |= renbanshot_conpaneshot(Tech_Address[wk->wu.id], Power_Level);
        return 1;
    }

    return 0;
}

s32 Command_Type_01(PLW* wk, s16 Power_Level, s16 Ex_Shot) {
    switch (CP_Index[wk->wu.id][2]) {
    case 0:
        CP_Index[wk->wu.id][2]++;
        Timer_01[wk->wu.id] = Tech_Address[wk->wu.id][Tech_Index[wk->wu.id] + 1] + 2;
        Lever_Pool[wk->wu.id] = Tech_Address[wk->wu.id][Tech_Index[wk->wu.id] + 3];
        Setup_Command_01(wk);

        if (wk->wu.rl_waza) {
            if (Lever_Pool[wk->wu.id] & 0xC) {
                Lever_Pool[wk->wu.id] ^= 0xC;
            }
        }
        /* fallthrough */

    case 1:
        if (wk->permited_koa & 2) {
            CP_Index[wk->wu.id][2]++;
        } else {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
            Timer_00[wk->wu.id]++;
            break;
        }

    default:
        if (++Timer_00[wk->wu.id] < Timer_01[wk->wu.id]) {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        } else {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
            Tech_Index[wk->wu.id] += 4;
            if (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]] == 0x1C) {
                return 1;
            }
        }
        break;
    }
    return 0;
}

void Setup_Command_01(PLW* wk) {
    switch (Lever_Pool[wk->wu.id]) {
    case 2:
        Timer_00[wk->wu.id] = Lever_Store[wk->wu.id][0];
        break;
    default:
        Timer_00[wk->wu.id] = Lever_Store[wk->wu.id][2];
        break;
    }
}

void Check_Store_Lever(PLW* wk, u16 Tech_Number, s16 Next_Action, s16 Next_Menu) {
    s16 time;
    u16 lever;

    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    if (cmd_sel[wk->wu.id]) {
        Tech_Address[wk->wu.id] = player_CMD[wk->player_number][Tech_Number & 0xFF];
    } else {
        Tech_Address[wk->wu.id] = player_cmd[wk->player_number][Tech_Number & 0xFF];
    }
    time = Tech_Address[wk->wu.id][13] + 2;
    lever = Tech_Address[wk->wu.id][15];

    if (Check_Store_Direction(wk, lever, time) != 0) {
        CP_Index[wk->wu.id][0]++;
    } else {
        Next_Another_Menu(wk, Next_Action, Next_Menu);
    }
}

s32 Check_Store_Direction(PLW* wk, u16 lever, s16 time) {
    if (wk->wu.rl_waza) {
        if (lever & (SWK_LEFT | SWK_RIGHT)) {
            lever ^= (SWK_LEFT | SWK_RIGHT);
        }
    }

    switch (lever) {
    case SWK_DOWN:
        if (time <= Lever_Store[wk->wu.id][0]) {
            return 1;
        }

        break;

    case SWK_RIGHT:
        if (time <= Lever_Store[wk->wu.id][1]) {
            return 1;
        }

        break;

    case SWK_LEFT:
        if (time <= Lever_Store[wk->wu.id][2]) {
            return 1;
        }

        break;
    }

    return 0;
}

s32 Select_Combo_Speed(PLW* wk) {
    s8 xx;
    s8 zz;

    xx = (u8)random_32_com();

    Lv = Setup_Lv18(8);
    Lv += CC_Value[0];

    if (Break_Into_CPU == 2) {
        Lv = 0x13;
    }

    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Lv = emLevelRemake(Lv, 0x14, 2);

    if ((Break_Into_CPU == 1) || (Break_Into_CPU == 2)) {
        return zz = Combo_Speed_Unit_Data[17][Lv][xx];
    }
    return zz = Combo_Speed_Unit_Data[wk->player_number][Lv][xx];
}

s32 Select_Reflection_Time(PLW* wk) {
    s8 Lv;
    s8 xx;
    s8 zz;

    xx = (u8)random_32_com();
    Lv = Setup_Lv18(save_w[Present_Mode].Difficulty + 0);
    Lv += CC_Value[0];
    if (Break_Into_CPU == 2) {
        Lv = 0x13;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Lv = emLevelRemake(Lv, 0x14, 2);

    time_check[time_check_ix] = xx;
    time_check_ix++;
    time_check_ix &= 3;

    if ((Break_Into_CPU == 1) || (Break_Into_CPU == 2)) {
        return zz = Reflection_Speed_Unit_Data[17][Lv][xx];
    }
    return zz = Reflection_Speed_Unit_Data[My_char[wk->wu.id]][Lv][xx];
}

s32 Setup_Lv04(s16 xx) {
    s16 i;
    s16* zz;

    zz = (s16*)&Level_04_Data[xx];

    for (i = 0; i < 3; i++) {
        if (Control_Time <= zz[i]) {
            return i;
        }
    }
    return 3;
}

s32 Setup_Lv08(s16 xx) {
    s16 i;
    s16* zz;

    zz = (s16*)&Level_08_Data[xx];

    for (i = 0; i < 7; i++) {
        if (Control_Time <= zz[i]) {
            break;
        }
    }
    return i;
}

s32 Setup_Lv10(s16 xx) {
    s16 i;
    s16* zz;

    zz = (s16*)&Level_10_Data[xx];

    for (i = 0; i < 9; i++) {
        if (Control_Time <= zz[i]) {
            break;
        }
    }
    return i;
}

s32 Setup_Lv18(s16 xx) {
    s16 i;
    s16* zz;

    zz = (s16*)&Level_18_Data[xx];

    for (i = 0; i < 17; i++) {
        if (Control_Time <= zz[i]) {
            break;
        }
    }
    return i;
}

s32 Setup_VS_Catch_Data(PLW* wk) {
    Lv = Setup_Lv08(0);
    if (Break_Into_CPU == 2) {
        Lv = 7;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }
    return VS_Catch_Data[emLevelRemake(Lv, 8, 0)];
}

s32 Setup_LP_Data(PLW* wk) {
    Lv = Setup_Lv08(0);
    if (Break_Into_CPU == 2) {
        Lv = 7;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }
    return LOOK_POSITION_Data[emLevelRemake(Lv, 8, 0)][random_32_com()];
}

s32 Setup_WT_Data(PLW* wk) {
    Lv = Setup_Lv04(0);
    if (Break_Into_CPU == 2) {
        Lv = 3;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }
    return Wait_Time_Data[emLevelRemake(Lv, 4, 0)][random_16_com() & 7];
}

void Ck_Distance(PLW* wk) {
    PL_Distance[wk->wu.id] = ((WORK*)wk->wu.target_adrs)->xyz[0].disp.pos - wk->wu.xyz[0].disp.pos;
    if (PL_Distance[wk->wu.id] < 0) {
        PL_Distance[wk->wu.id] = PL_Distance[wk->wu.id] * -1;
    }
}

s32 Ck_Distance_Height(PLW* wk) {
    s16 xx;

    xx = ((WORK*)wk->wu.target_adrs)->xyz[1].disp.pos - wk->wu.xyz[1].disp.pos;
    if (xx < 0) {
        xx = xx * -1;
    }
    return xx;
}

s32 Ck_Area(PLW* wk) {
    s16 i;

    for (i = 0; i < 3; i++) {
        if (PL_Distance[wk->wu.id] <= Separate_Area[wk->wu.id][i]) {
            return i;
        }
    }
    return 3;
}

s32 Ck_Area_Shell(PLW* wk) {
    s16 i;

    for (i = 0; i < 3; i++) {
        if (PL_Distance[wk->wu.id] <= Shell_Separate_Area[wk->wu.id][i]) {
            return i;
        }
    }

    return 3;
}

void Ck_Distance_Lv(PLW* wk) {
    PL_Distance[wk->wu.id] = ((WORK*)wk->wu.target_adrs)->xyz[0].disp.pos - wk->wu.xyz[0].disp.pos;

    if (PL_Distance[wk->wu.id] > 0) {
        Lever_Buff[wk->wu.id] = 8;
    } else {
        Lever_Buff[wk->wu.id] = 4;
        PL_Distance[wk->wu.id] = PL_Distance[wk->wu.id] * -1;
    }
}

void Ck_Distance_LvJ(PLW* wk) {
    PL_Distance[wk->wu.id] = ((WORK*)wk->wu.target_adrs)->xyz[0].disp.pos - wk->wu.xyz[0].disp.pos;

    if (PL_Distance[wk->wu.id] > 0) {
        Lever_Pool[wk->wu.id] = 9;
    } else {
        Lever_Pool[wk->wu.id] = 5;
        PL_Distance[wk->wu.id] = PL_Distance[wk->wu.id] * -1;
    }
}

void Next_End(PLW* wk) {
    if (Check_Guard(wk) != 0) {
        return;
    }

    CP_Index[wk->wu.id][0] = 0xFF;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;
}

void Next_Another_Menu(PLW* wk, s16 Next_Action, u16 Next_Menu) {
    if (Next_Action != 1) {
        CP_No[wk->wu.id][0] = Next_Action;
        Pattern_Index[wk->wu.id] = Next_Menu;
        CP_Index[wk->wu.id][0] = 0;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;
    } else {
        Next_Be_Free(wk);
    }
}

void Reaction_Sub(PLW* wk, s16 Reaction, s16 Power_Level) {
    switch (Reaction & 0x7F) {
    case 9:
        if (Stock_Hit_Flag[wk->wu.id]) {
            Reaction_Exit_Sub(wk);
        } else if ((wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40)) {
            Next_End(wk);
        }
        break;

    case 10:
        if ((Stock_Hit_Flag[wk->wu.id] >> 2) != 0) {
            Next_End(wk);
            break;
        }
        if (Stock_Hit_Flag[wk->wu.id]) {
            Reaction_Exit_Sub(wk);
        } else {
            if ((wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40)) {
                Reaction_Exit_Sub(wk);
            }
        }
        break;

    case 11:
        if (gs.plw[wk->wu.id].caution_flag) {
            break;
        }
        if (gs.plw[wk->wu.id].tsukami_f) {
            break;
        }

        if ((Stock_Hit_Flag[wk->wu.id] >> 2) != 0) {
            Next_End(wk);
            break;
        }
        if (Stock_Hit_Flag[wk->wu.id]) {
            Reaction_Exit_Sub(wk);
        } else if ((wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40)) {
            Next_End(wk);
        }
        break;

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        if ((Stock_Hit_Flag[wk->wu.id] >> 2) != 0) {
            Setup_Follow(wk, Reaction & 0xFFF);
            break;
        }
        if (Stock_Hit_Flag[wk->wu.id]) {
            Reaction_Exit_Sub(wk);
        } else if ((wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40)) {
            Setup_Follow(wk, Reaction & 0xFFF);
        }
        break;

    case 12:
        Reaction_Exit_Sub(wk);
        Counter_Attack[wk->wu.id] = 1;
        break;

    case 14:
        Reaction_Exit_Sub(wk);
        Counter_Attack[wk->wu.id] = 1;
        break;

    case 13:

        if ((Stock_Hit_Flag[wk->wu.id] >> 2) != 0) {
            Next_End(wk);
            break;
        }
        if (wk->permited_koa & 0x10) {
            if (Check_Meoshi_Attack(wk, Reaction, Power_Level) != 0) {
                break;
            }
        }
        Last_Eftype[wk->wu.id] = -1;

        if ((wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40)) {
            if (Stock_Hit_Flag[wk->wu.id]) {
                if ((CP_No[wk->wu.id][0] == 6) && (Pattern_Index[wk->wu.id] == 0)) {
                    CP_No[wk->wu.id][0] = Return_CP_No[wk->wu.id];
                    CP_Index[wk->wu.id][0] = Return_CP_Index[wk->wu.id];
                    CP_Index[wk->wu.id][1] = 0;
                    CP_Index[wk->wu.id][2] = 0;
                    CP_Index[wk->wu.id][3] = 0;
                    Pattern_Index[wk->wu.id] = Return_Pattern_Index[wk->wu.id];
                } else {
                    Reaction_Exit_Sub(wk);
                }
            } else {
                Next_End(wk);
            }
        }
        break;

    default:

        if (gs.plw[wk->wu.id].caution_flag) {
            break;
        }
        if (gs.plw[wk->wu.id].tsukami_f) {
            break;
        }

        if (Stock_Hit_Flag[wk->wu.id]) {
            Reaction_Exit_Sub(wk);
        }

        else if ((wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40)) {
            Reaction_Exit_Sub(wk);
        }

        break;
    }
}

s32 Check_Meoshi_Attack(PLW* wk, s16 Reaction, s16 Power_Level) {
    if (wk->wu.cg_meoshi == Last_Eftype[wk->wu.id]) {
        return 0;
    }

    if (wk->permited_koa & 0x10) {
        Last_Eftype[wk->wu.id] = wk->wu.cg_meoshi;

        M_Lv[wk->wu.id] = Get_Meoshi_Data(wk);

        if (Pattern_Index[wk->wu.id] != 0) {
            Return_CP_No[wk->wu.id] = CP_No[wk->wu.id][0];
            Return_CP_Index[wk->wu.id] = CP_Index[wk->wu.id][0] + 1;
            CP_Index[wk->wu.id][1] = 0;
            CP_Index[wk->wu.id][2] = 0;
            CP_Index[wk->wu.id][3] = 0;
            Return_Pattern_Index[wk->wu.id] = Pattern_Index[wk->wu.id];
        }
        CP_No[wk->wu.id][0] = 6;
        CP_Index[wk->wu.id][0] = 0;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;
        Pattern_Index[wk->wu.id] = 0;
        return 1;
    }
    return 0;
}

s32 Get_Meoshi_Data(PLW* wk) {
    u16 lever;
    u16 shot;

    lever = get_meoshi_lever(wk->wu.cg_meoshi);
    shot = get_meoshi_shot(wk->wu.cg_meoshi);

    if (wk->wu.rl_flag) {
        lever ^= 0xC;
    }

    return shot | lever;
}

void Reaction_Exit_Sub(PLW* wk) {
    CP_Index[wk->wu.id][0]++;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;

    Flip_Flag[wk->wu.id] = 0;
    Counter_Attack[wk->wu.id] = 0;
    Limited_Flag[wk->wu.id] = 0;
    Before_Jump[wk->wu.id] = 0;
    if (CP_No[wk->wu.id][0] != 6) {
        Passive_Flag[wk->wu.id] = 0;
    }
}

void Check_First_Menu(PLW* wk) {
    if (CP_Index[wk->wu.id][0] == 0) {
        Combo_Speed[wk->wu.id] = 1;
    } else {
        Combo_Speed[wk->wu.id] = Select_Combo_Speed(wk);
    }
}

void Select_Active(PLW* wk) {
    s16 pl_id;

    Lv = Setup_Lv08(0);
    if (Break_Into_CPU == 2) {
        Lv = 7;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Lv = emLevelRemake(Lv, 8, 0);

    Rnd = (u8)random_32_ex_com();

    if (Check_SA_Active(wk, &pl_id) != 0) {
        Lv = Setup_Lv04(0);
        if (Break_Into_CPU == 2) {
            Lv = 3;
        }
        if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
            Lv = 2;
        }

        Lv = emLevelRemake(Lv, 4, 0);

        switch (Area_Number[wk->wu.id]) {
        case 0:
            Pattern_Index[wk->wu.id] = SA_Active_A_Unit_Data[pl_id - 1][Lv][Rnd];
            break;
        case 1:
            Pattern_Index[wk->wu.id] = SA_Active_B_Unit_Data[pl_id - 1][Lv][Rnd];
            break;
        case 2:
            Pattern_Index[wk->wu.id] = SA_Active_C_Unit_Data[pl_id - 1][Lv][Rnd];
            break;
        default:
            Pattern_Index[wk->wu.id] = SA_Active_D_Unit_Data[pl_id - 1][Lv][Rnd];
            break;
        }
    } else {
        switch (Area_Number[wk->wu.id]) {
        case 0:
            Pattern_Index[wk->wu.id] = Active_A_Unit_Data[wk->player_number][Lv][Rnd];
            break;

        case 1:
            Pattern_Index[wk->wu.id] = Active_B_Unit_Data[wk->player_number][Lv][Rnd];
            break;

        case 2:
            Pattern_Index[wk->wu.id] = Active_C_Unit_Data[wk->player_number][Lv][Rnd];
            break;

        default:
            Pattern_Index[wk->wu.id] = Active_D_Unit_Data[wk->player_number][Lv][Rnd];
            break;
        }
    }

    if (Debug_w[0x36]) {
        Pattern_Index[wk->wu.id] = (u16)Debug_w[0x36] - 1;
    }
}

s32 Check_SA_Active(PLW* wk, s16* pl_id) {
    if (wk->sa->ok != -1) {
        return 0;
    }
    if (My_char[wk->wu.id] == 9) {
        if (gs.plw[wk->wu.id].sa->kind_of_arts == 0) {
            return *pl_id = 3;
        }
        return *pl_id = 2;
    }
    if ((My_char[wk->wu.id] == 3) && (gs.plw[wk->wu.id].sa->kind_of_arts == 2)) {
        return *pl_id = 1;
    }
    if ((My_char[wk->wu.id] == 0xA) && (gs.plw[wk->wu.id].sa->kind_of_arts == 2)) {
        return *pl_id = 1;
    }
    if ((My_char[wk->wu.id] == 0x11) && (gs.plw[wk->wu.id].sa->kind_of_arts == 2)) {
        return *pl_id = 4;
    }
    return 0;
}

void Setup_Follow(PLW* wk, s16 Follow_Type) {
    CP_No[wk->wu.id][0] = 3;
    CP_No[wk->wu.id][1] = Follow_Type;

    if (wk->wu.hf.hit.player == 0) {
        CP_No[wk->wu.id][2] = 0;
    } else {
        CP_No[wk->wu.id][2] = 1;
    }

    CP_No[wk->wu.id][3] = 0;
    Timer_00[wk->wu.id] = Select_Reflection_Time(wk);
    Timer_00[wk->wu.id]++;
}

// sdata
typedef const _anon6* const_anon6_p;

static const_anon6_p Follow_Menu_1st_Unit_Data[13] = {
    &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data, &COM02_Flollow_1st_Unit_Data,
    &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data,
    &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data,
    &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data,
    &COM00_Flollow_1st_Unit_Data,
};

typedef const _anon13* const_anon13_p;

static const_anon13_p Follow_Menu_2nd_Unit_Data[13] = {
    &Com00_Follow_Menu, &Com00_Follow_Menu, &Com02_Follow_Menu, &Com00_Follow_Menu, &Com00_Follow_Menu,
    &Com00_Follow_Menu, &Com00_Follow_Menu, &Com00_Follow_Menu, &Com00_Follow_Menu, &Com00_Follow_Menu,
    &Com00_Follow_Menu, &Com00_Follow_Menu, &Com00_Follow_Menu,
};

void Decide_Follow_Menu(PLW* wk) {
    s8 xx;
    const _anon6* Menu_Add_Ptr0;
    const _anon13* Menu_Add_Ptr1;

    Menu_Add_Ptr0 = Follow_Menu_1st_Unit_Data[wk->player_number];
    Rnd = (u8)random_32_com();
    xx = Menu_Add_Ptr0->xxxx[CP_No[wk->wu.id][1]][CP_No[wk->wu.id][2]][Rnd];

    Menu_Add_Ptr1 = Follow_Menu_2nd_Unit_Data[wk->player_number];
    Pattern_Index[wk->wu.id] = Menu_Add_Ptr1->zzzz[xx][Area_Number[wk->wu.id]];
}

s32 Select_Passive(PLW* wk) {
    u16 xx;

    if (VS_Tech[wk->wu.id] == 0xB) {
        Area_Number[wk->wu.id] = Ck_Area_Shell(wk);
    }
    if (VS_Tech[wk->wu.id] == 0x1E) {
        Area_Number[wk->wu.id] = Ck_Area_Shell(wk);
    }

    Last_Attack_Counter[wk->wu.id] = Attack_Counter[wk->wu.id];
    Standing_Timer[wk->wu.id] = 0;
    Devide_Level(VS_Tech[wk->wu.id] >> 0xC);
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 1;
    }
    VS_Tech[wk->wu.id] &= 0xFFF;

    Setup_Random(wk);

    switch (Area_Number[wk->wu.id]) {
    case 0:
        xx = Passive_A_Unit_Data_04[wk->player_number][VS_Tech[wk->wu.id]][Lv][Rnd];

        if (xx == 0xFF) {
            Counter_Attack[wk->wu.id] = 0;
            Passive_Flag[wk->wu.id] = 0;
            Jump_Pass_Timer[wk->wu.id][0] = 0x78;

            return -1;
        }
        Pattern_Index[wk->wu.id] = xx;
        Jump_Pass_Timer[wk->wu.id][0] = 0;
        break;

    case 1:
        xx = Passive_B_Unit_Data_04[wk->player_number][VS_Tech[wk->wu.id]][Lv][Rnd];

        if (xx == 0xFF) {
            Counter_Attack[wk->wu.id] = 0;
            Passive_Flag[wk->wu.id] = 0;
            Jump_Pass_Timer[wk->wu.id][1] = 0x78;

            return -1;
        }
        Pattern_Index[wk->wu.id] = xx;
        Jump_Pass_Timer[wk->wu.id][1] = 0;
        break;

    case 2:
        xx = Passive_C_Unit_Data_04[wk->player_number][VS_Tech[wk->wu.id]][Lv][Rnd];

        if (xx == 0xFF) {
            Counter_Attack[wk->wu.id] = 0;
            Passive_Flag[wk->wu.id] = 0;
            Jump_Pass_Timer[wk->wu.id][2] = 0x78;

            return -1;
        }
        Pattern_Index[wk->wu.id] = xx;
        Jump_Pass_Timer[wk->wu.id][2] = 0;
        break;

    default:
        xx = Passive_D_Unit_Data_04[wk->player_number][VS_Tech[wk->wu.id]][Lv][Rnd];

        if (xx == 0xFF) {
            Counter_Attack[wk->wu.id] = 0;
            Passive_Flag[wk->wu.id] = 0;
            Jump_Pass_Timer[wk->wu.id][3] = 0x78;

            return -1;
        }
        Pattern_Index[wk->wu.id] = xx;
        Jump_Pass_Timer[wk->wu.id][3] = 0;
        break;
    }

    Passive_Flag[wk->wu.id] = 1;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Timer_00[wk->wu.id] = Select_Reflection_Time(wk);

    if (Debug_w[0x37]) {
        Pattern_Index[wk->wu.id] = (u16)Debug_w[0x37] - 1;
    }

    if ((VS_Tech[wk->wu.id] == 0x19) || (VS_Tech[wk->wu.id] == 0x13) || (Timer_00[wk->wu.id] == 0)) {
        CP_No[wk->wu.id][0] = 6;
        CP_Index[wk->wu.id][0] = 0;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;
    } else {
        CP_No[wk->wu.id][0] = 5;
    }
    return 1;
}

void Devide_Level(s16 xx) {
    switch (xx) {
    case 0:
        Lv = Setup_Lv04(0);
        if (Break_Into_CPU == 2) {
            Lv = 3;
        }
        break;
    default:
        Lv = Setup_Lv08(2);
        if (Break_Into_CPU == 2) {
            Lv = 7;
        }
        break;
    }
}

void Setup_Random(PLW* wk) {
    if (VS_Tech[wk->wu.id] == 0x20) {
        Rnd = (u8)random_16_com() & 7;
        VS_Tech[wk->wu.id] = 0x1C;
    } else {
        Rnd = (u8)random_16_com();
        Rnd = Check_Dramatic(wk, wk->wu.id);
    }
}

s32 Check_Dramatic(PLW* wk, s16 PL_id) {
    if (gs.plw[wk->wu.id].sa->ok) {
        return Rnd | 8;
    }

    if ((gs.plw[PL_id].wu.vital_new <= 0x30) || (gs.plw[PL_id ^ 1].wu.vital_new <= 0x30)) {
        return Rnd | 8;
    }
    return Rnd;
}

const s8 PL_Status[0xA] = { 1, 0, 0, 0, 1, 1, 0, 0, 0, 0 };

s32 Check_Passive(PLW* wk) {
    WORK* em;

    if ((Counter_Attack[wk->wu.id] != 0) || (Pierce_Menu[wk->wu.id] != 0)) {
        return 0;
    }

    em = (WORK*)wk->wu.target_adrs;

    if (Check_Blow_Off(wk, em, 0) != 0) {
        *CP_No[wk->wu.id] = 0xE;
        CP_No[wk->wu.id][1] = 0;
        CP_No[wk->wu.id][2] = 0;
        CP_No[wk->wu.id][3] = 0;
        return -1;
    }
    if (Check_Thrown(wk, em) != 0) {
        if (Select_Passive(wk) != -1) {
            return 1;
        }
    }

    if (Check_Shell(wk) != 0) {
        return 1;
    }
    if ((Passive_Flag[wk->wu.id]) || (Flip_Flag[wk->wu.id])) {
        return Check_Guard(wk);
    }
    if (Check_Lie(wk) == 1) {
        return 1;
    }

    if (PL_Status[em->routine_no[1]] == 0) {
        return Check_Shell(wk);
    }

    Passive_Mode = 4;

    if (Ck_Passive_Term(wk) != 0) {
        if (Select_Passive(wk) != -1) {
            return 1;
        }
    }

    if (Check_Guard(wk) != 0) {
        return 1;
    }
    if ((Passive_Flag[wk->wu.id]) || (Flip_Flag[wk->wu.id])) {
        return 0;
    }

    Passive_Mode = 0;

    if (Ck_Passive_Term(wk) != 0) {
        return Select_Passive(wk);
    }

    return 0;
}

s32 Check_Guard(PLW* wk) {
    WORK* em;
    s16 xx;
    s16 zz;

    em = (WORK*)wk->wu.target_adrs;

    if (Attack_Flag[wk->wu.id] == 0) {
        return 0;
    }

    if (Guard_Counter[wk->wu.id] == Attack_Counter[wk->wu.id]) {
        return 0;
    }

    xx = Hit_Range_Data[em->hit_range];
    xx += Com_Width_Data[wk->wu.id];

    if (PL_Distance[wk->wu.id] > xx) {
        return 0;
    }

    Lv = Setup_Lv10(0);
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }
    Lv += CC_Value[0];
    if (Break_Into_CPU == 2) {
        Lv = 0xA;
    }

    Rnd = random_16_com();

    zz = Setup_EM_Rank_Index(wk);

    Lv = emLevelRemake(Lv, 0xB, 1);

    if (Guard_Data[zz][Lv][Rnd] == 3) {
        Guard_Counter[wk->wu.id] = Attack_Counter[wk->wu.id];
        return 0;
    }

    if (Check_Flip_Term(wk, NULL) != 0) {
        Next_Be_Flip(wk, 0);
    } else {
        Next_Be_Guard(wk, em, Guard_Data[zz][Lv][random_16_ex_com()]);
    }

    return 1;
}

s32 Check_Makoto(PLW* wk) {
    if (wk->player_number != 0x10) {
        return 0;
    }
    if (wk->sa->ok != -1) {
        return 0;
    }
    if (gs.plw[wk->wu.id].sa->kind_of_arts == 2) {
        return 1;
    }

    return 0;
}

s32 Check_Flip_Term(PLW* wk, WORK* tmw) {
    WORK* em;
    s16 xx;

    if (tmw != NULL) {
        em = tmw;
    } else {
        em = (WORK*)wk->wu.target_adrs;
    }

    if (Check_Flip_Tech(em) == 0) {
        return 0;
    }

    Lv = Setup_Lv08(0);

    if (Break_Into_CPU == 2) {
        Lv = 7;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Rnd = random_32_com();
    Rnd -= Flip_Term_Correct(wk);

    xx = Setup_EM_Rank_Index(wk);

    if (Rnd >= (Flip_Data[xx][emLevelRemake(Lv, 8, 0)])) {
        return 0;
    }

    return 1;
}

s32 Setup_EM_Rank_Index(PLW* wk) {
    if (EM_Rank != 0) {
        return 0x11;
    }

    return wk->player_number;
}

s32 Flip_Term_Correct(PLW* wk) {
    s16 xx = 0;

    if (gs.plw[wk->wu.id].wu.vital_new < 0x31) {
        xx += 1;
    }
    if ((PL_Wins[wk->wu.id]) < (PL_Wins[wk->wu.id ^ 1])) {
        xx += 2;
    }
    if (Counter_hi < 0xF) {
        xx += 1;
    }
    if (Check_Makoto(wk) != 0) {
        xx += 20;
    }

    return xx;
}

void Next_Be_Guard(PLW* wk, WORK* em, s16 Type_Of_Guard) {
    CP_No[wk->wu.id][0] = 7;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Timer_00[wk->wu.id] = 10;

    dash_flag_clear(wk->wu.id);
    Guard_Type[wk->wu.id] = Type_Of_Guard;
    Check_Guard_Type(wk, em);
}

s32 Check_Flip_Tech(WORK* em) {
    s32 rnum = 1;

    switch (save_w[Present_Mode].Difficulty) {
    case 0:
        rnum = 0;
        break;
    default:
        if (em->kind_of_waza & 0xF8) {
            rnum = 0;
        }
        /* fallthrough */
    case 6:
        if (em->kind_of_waza == 0) {
            rnum = 0;
        }
        if (em->kind_of_waza == 1) {
            rnum = 0;
        }
        break;
    case 7:
        break;
    }

    return rnum;
}

void Next_Be_Flip(PLW* wk, s16 xx) {
    WORK* em;

    em = (WORK*)wk->wu.target_adrs;

    CP_No[wk->wu.id][0] = 0xC;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Timer_00[wk->wu.id] = 9;

    Flip_Counter[wk->wu.id] = 0;

    if (xx) {
        if (xx == 8) {
            SetShellFlipLever(wk);
        } else {
            if ((em->pat_status == 0x21) || (em->pat_status == 0x20)) {
                Lever_Buff[wk->wu.id] = 2;
            } else {
                Lever_Buff[wk->wu.id] = Setup_Guard_Lever(wk, 0);
            }
        }
        CP_No[wk->wu.id][2] = 1;

        Timer_01[wk->wu.id] = xx;
    } else {
        Check_Flip_GO(wk, 0);
    }
}

s32 Check_Diagonal_Shell(PLW* wk) {
    WORK_Other* tmw;
    WORK* em;
    s16 i;

    Lv = Setup_Lv08(0);
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Rnd = random_16_com();
    Lv += *CC_Value;

    if (Break_Into_CPU == 2) {
        Lv = 7;
    }
    if (Rnd > VS_Diagonal_Shell_Data[emLevelRemake(Lv, 8, 0)]) {
        return 0;
    }

    em = (WORK*)wk->wu.target_adrs;

    for (i = 0; i < 8; i++) {
        if ((get_vs_shell_adrs(em, em->id, i, &tmw) == 0) && (get_vs_shell_adrs(&wk->wu, em->id, i, &tmw) == 0)) {
            return 0;
        }

        if (tmw->wu.routine_no[1] == 2) {
            continue;
        }
        if (wk->wu.rl_waza == tmw->wu.rl_flag) {
            continue;
        }

        if (Check_Behind(wk, tmw) != 0) {
            continue;
        }

        if (tmw->wu.charset_id == 2) {
            continue;
        }
        if (Check_Ignore_Shell2(tmw) != 0) {
            return 1;
        }
    }
    return 0;
}

s32 Check_Ignore_Shell2(WORK_Other* tmw) {
    if (tmw->wu.type == 0xDE) {
        return 1;
    }
    if ((tmw->wu.type >= 0x24) && (tmw->wu.type < 0x28)) {
        return 1;
    }
    if ((tmw->wu.type >= 0xD) && (tmw->wu.type < 0x10)) {
        return 1;
    }
    if ((tmw->wu.type == 0x54) || (tmw->wu.type == 0x55)) {
        return 1;
    }
    if ((tmw->wu.type >= 0x4D) && (tmw->wu.type < 0x51)) {
        return 1;
    }
    if ((tmw->wu.type >= 0x7A) && (tmw->wu.type < 0x7F)) {
        return 1;
    }

    return 0;
}

s32 Check_Shell(PLW* wk) {
    WORK_Other* tmw;
    WORK* em;
    s16 i;
    s16 xx;

    if (Shell_Ignore_Timer[wk->wu.id]) {
        Shell_Ignore_Timer[wk->wu.id]--;
        return 0;
    }
    if (CP_No[wk->wu.id][0] == 8) {
        return 0;
    }

    em = (WORK*)wk->wu.target_adrs;

    for (i = 0; i < 8; i++) {
        if ((get_vs_shell_adrs(em, em->id, i, &tmw) == 0) && (get_vs_shell_adrs(&wk->wu, em->id, i, &tmw) == 0)) {
            return 0;
        }

        if (tmw->wu.routine_no[1] == 2) {
            continue;
        }
        if (wk->wu.rl_waza == tmw->wu.rl_flag) {
            continue;
        }
        if (tmw->wu.routine_no[0] != 1) {
            continue;
        }
        if (Check_Behind(wk, tmw) == 0) {
            if (tmw->wu.charset_id == 2) {
                continue;
            }
            if (Check_Ignore_Shell(tmw) == 0) {
                xx = Compute_Hit_Time(wk, tmw);

                if (Decide_Shell_Guard(wk, tmw) != 0) {
                    return 0;
                }

                CP_No[wk->wu.id][0] = 8;
                CP_No[wk->wu.id][1] = 0;
                CP_No[wk->wu.id][2] = 0;
                CP_No[wk->wu.id][3] = 0;

                CP_Index[wk->wu.id][0] = 0;
                CP_Index[wk->wu.id][1] = 0;
                CP_Index[wk->wu.id][2] = 0;
                CP_Index[wk->wu.id][3] = 0;

                Shell_Address[wk->wu.id] = (u32*)tmw;

                Guard_or_Jump_VS_Shell(wk, tmw, xx);

                return 1;
            }
        }
    }

    return 0;
}

s32 Check_Shell_Another_in_Flip(PLW* wk) {
    WORK_Other* tmw;
    WORK* em;
    s32 i;
    s32 xx = 0;

    em = (WORK*)wk->wu.target_adrs;

    for (i = 0; i < 8; i++) {
        if ((get_vs_shell_adrs(em, em->id, i, &tmw) == 0) && (get_vs_shell_adrs(&wk->wu, em->id, i, &tmw) == 0)) {
            return 0;
        }

        if (tmw->wu.routine_no[1] == 2) {
            continue;
        }
        if (wk->wu.rl_waza == tmw->wu.rl_flag) {
            continue;
        }
        if (tmw->wu.routine_no[0] != 1) {
            continue;
        }
        if (Check_Behind(wk, tmw) != 0) {
            continue;
        }

        if (tmw->wu.charset_id == 2) {
            continue;
        }
        if (Check_Ignore_Shell(tmw) != 0) {
            continue;
        }

        xx = Compute_Hit_Time(wk, tmw);

        Shell_Address[wk->wu.id] = (u32*)tmw;

        break;
    }
    return xx;
}

s32 Check_Ignore_Shell(WORK_Other* tmw) {
    if (tmw->wu.type == 0xDE) {
        return 1;
    }
    if ((tmw->wu.type >= 0x24) && (tmw->wu.type < 0x28)) {
        return 1;
    }
    if ((tmw->wu.type >= 0xD) && (tmw->wu.type < 0x10)) {
        return 1;
    }
    if ((tmw->wu.type == 0x54) || (tmw->wu.type == 0x55)) {
        return 1;
    }
    if ((tmw->wu.type >= 0x4D) && (tmw->wu.type < 0x51)) {
        return 1;
    }
    if ((tmw->wu.type >= 0x7A) && (tmw->wu.type < 0x7F)) {
        return 1;
    }

    return 0;
}

s32 Compute_Hit_Time(PLW* wk, WORK_Other* tmw) {
    s32 lx1;
    s32 divsp;
    s16 x2;

    lx1 = get_att_head_position(&tmw->wu);
    lx1 <<= 16;

    lx1 -= wk->wu.xyz[0].cal;

    if (tmw->wu.mvxy.a[0].sp != 0) {
        divsp = tmw->wu.mvxy.a[0].sp;
    } else {
        divsp = 0x48000;
    }

    x2 = lx1 / divsp;

    if (x2 < 0) {
        x2 = x2 * -1;
    }

    return x2;
}

s32 Decide_Shell_Guard(PLW* wk, WORK_Other* tmw) {
    s16 xx;

    Lv = Setup_Lv10(0);

    if (Break_Into_CPU == 2) {
        Lv = 9;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Rnd = random_32_com();

    xx = Setup_EM_Rank_Index(wk);
    if (Shell_Guard_Data[xx][emLevelRemake(Lv, 0xA, 0)] > Rnd) {
        return 0;
    }
    Shell_Ignore_Timer[wk->wu.id] = 0x3C;
    return 1;
}

void Guard_or_Jump_VS_Shell(PLW* wk, WORK_Other* tmw, s16 xx) {
    if (xx <= Shell_Dodge_Data[0][wk->player_number]) {
        if (Check_Flip_Term(wk, &tmw->wu) != 0) {
            Pattern_Index[wk->wu.id] = 9;
        } else {
            Pattern_Index[wk->wu.id] = 0;
        }
    } else {
        switch (save_w[Present_Mode].Difficulty) {
        case 7:
            if (wk->wu.vital_new < 4) {
                if (!(random_32_com() & 0xF)) {
                    Pattern_Index[wk->wu.id] = 9;
                    break;
                }
            }
            /* fallthrough */
        case 6:
            if (wk->wu.vital_new < 2) {
                if (!(random_32_com() & 7)) {
                    Pattern_Index[wk->wu.id] = 9;
                    break;
                }
            }
            /* fallthrough */
        default:
            Pattern_Index[wk->wu.id] = Decide_Shell_Reaction(wk, tmw, Shell_Change_Data_For_Reaction[tmw->wu.type]);
            break;
        }
    }

    Setup_Shell_Disposal(wk, tmw);
}

void Setup_Shell_Disposal(PLW* wk, WORK_Other* tmw) {
    switch (Pattern_Index[wk->wu.id]) {
    case 0:
        Next_Be_Shell_Guard(wk, &tmw->wu);
        break;
    case 9:
        Passive_Flag[wk->wu.id] = 1;
        break;
    case 10:
        Pattern_Index[wk->wu.id] = Decide_Shell_Reaction(wk, tmw, 0);

        if (Pattern_Index[wk->wu.id] == 0) {
            Next_Be_Shell_Guard(wk, &tmw->wu);
        }

        break;
    }
}

void Next_Be_Shell_Guard(PLW* wk, WORK* tmw) {
    CP_No[wk->wu.id][0] = 9;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Timer_00[wk->wu.id] = 0xA;

    dash_flag_clear(wk->wu.id);
    Guard_Type[wk->wu.id] = 0;
    Check_Guard_Type(wk, tmw);
}

s32 Decide_Shell_Reaction(PLW* wk, WORK_Other* tmw, u16 dir_step) {
    if (dir_step == 0xFF) {
        return 0;
    }

    Lv = Setup_Lv08(0);

    if (Break_Into_CPU == 2) {
        Lv = 7;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 4;
    }

    Rnd = random_16_com();

    return VS_Shell_Active_Data[wk->player_number][dir_step][emLevelRemake(Lv, 8, 0)][Rnd];
}

s32 Ck_Distance_XX(s16 x1, s16 x2) {
    s16 xx;

    xx = x1 - x2;
    if (xx < 0) {
        xx = xx * -1;
    }

    return xx;
}

s32 Check_Behind(PLW* wk, WORK_Other* tmw) {
    if (wk->wu.rl_waza == 0) {
        if (wk->wu.xyz[0].disp.pos < tmw->wu.xyz[0].disp.pos) {
            return 1;
        }
        return 0;
    } else {
        if (wk->wu.xyz[0].disp.pos > tmw->wu.xyz[0].disp.pos) {
            return 1;
        }
        return 0;
    }
}

typedef s32 (*Term_Tbl_t)(PLW* wk, WORK* em);
const Term_Tbl_t Exit_Term_Tbl[9] = { Exit_Term_0000, Exit_Term_0001, Exit_Term_0002, Exit_Term_0003, Exit_Term_0004,
                                      Exit_Term_0005, Exit_Term_0006, Exit_Term_0007, Exit_Term_0008 };

void Setup_Lever_LR(PLW* wk, s16 PL_id, s16 Lever) {
    if (Lever == 0) {
        Lever_LR[PL_id] = 0;
    } else {
        if (Lever & 0x1000) {
            Lever_LR[PL_id] = 1;
        }

        else if (Lever & 0x2000) {
            Lever_LR[PL_id] |= 2;
        }

        if (Lever & 0x4000) {
            Lever_LR[PL_id] |= Setup_Guard_Lever(wk, 0);
        } else {
            if (Lever & 0x8000) {
                Lever_LR[PL_id] |= Setup_Guard_Lever(wk, 1);
            }
        }
    }
}

s32 Check_Exit_Term(PLW* wk, WORK* em, s16 Exit_No) {
    s16 xx;

    if (VS_Jump_Term(wk, em, &xx) != 0) {
        return xx;
    }
    return Exit_Term_Tbl[Exit_No](wk, em);
}

s32 VS_Jump_Term(PLW* wk, WORK* em, s16* xx) {
    if (Attack_Flag[wk->wu.id] == 0) {
        return 0;
    }

    switch (My_char[em->id]) {
    case 1:
        if (Check_F_Cross_Chop(wk, em, 0xF) != 0) {
            return *xx = 3;
        }
        break;
    case 3:
    case 10:
        if (Check_Special_Technique(wk, em, 0, 0, 0x2b, -1, -1) != 0) {
            return *xx = 2;
        }
        break;
    case 5:
        if (Check_Special_Technique(wk, em, 0, 0, 0x2A, -1, -1) != 0) {
            return *xx = 2;
        }
        if (Check_Limited_Jump_Attack(wk, em, 0x14, 4) != 0) {
            VS_Tech[wk->wu.id] = 0xF;
            return *xx = 3;
        }
        break;
    case 8:
        if (Check_Limited_Jump_Attack(wk, em, 0x14, 5) != 0) {
            VS_Tech[wk->wu.id] = 0xF;
            return *xx = 3;
        }
        break;
    case 9:
        if (Check_Special_Technique(wk, em, 0xF, 8, 0x2C, 1, -1) != 0) {
            return *xx = 3;
        }
        break;
    }
    return 0;
}

s32 Exit_Term_0000(PLW* wk, WORK* em) {
    switch (Term_No[wk->wu.id]) {
    case 0:
        Term_No[wk->wu.id]++;
        Timer_00[wk->wu.id] = 0x1E;
        break;
    default:
        if (--Timer_00[wk->wu.id]) {
            break;
        }
        Timer_00[wk->wu.id] = 1;
        return 1;
    }

    return 0;
}

s32 Exit_Term_0001(PLW* wk, WORK* em) {
    return 1;
}

s32 Exit_Term_0002(PLW* wk, WORK* em) {
    if (em->routine_no[1] == 2) {
        return 1;
    }
    if (em->id != 0xD) {
        return 1;
    }
    return 0;
}

s32 Exit_Term_0003(PLW* wk, WORK* em) {
    switch (Term_No[wk->wu.id]) {
    case 0:
        Term_No[wk->wu.id]++;
        Timer_00[wk->wu.id] = 0xA;
        break;
    default:
        if (--Timer_00[wk->wu.id]) {
            break;
        }
        if (Check_SHINRYU(wk)) {
            return 0;
        }
        Timer_00[wk->wu.id] = 1;
        return 1;
    }

    return 0;
}

s32 Exit_Term_0004(PLW* wk, WORK* em) {
    switch (Term_No[wk->wu.id]) {
    case 0:
        Term_No[wk->wu.id]++;
        Timer_00[wk->wu.id] = 1;
        break;
    default:
        if (--Timer_00[wk->wu.id]) {
            break;
        }
        if (Check_SHINRYU(wk)) {
            return 0;
        }
        Timer_00[wk->wu.id] = 1;
        return 1;
    }

    return 0;
}

s32 Exit_Term_0005(PLW* wk, WORK* em) {
    switch (Term_No[wk->wu.id]) {
    case 0:
        Term_No[wk->wu.id]++;
        Timer_00[wk->wu.id] = 5;
        break;
    default:
        if (--Timer_00[wk->wu.id]) {
            break;
        }
        if (Check_SHINRYU(wk)) {
            return 0;
        }
        Timer_00[wk->wu.id] = 1;
        return 1;
    }

    return 0;
}

s32 Exit_Term_0006(PLW* wk, WORK* em) {
    switch (Term_No[wk->wu.id]) {
    case 0:
        Term_No[wk->wu.id]++;
        Timer_00[wk->wu.id] = 0x3C;
        break;
    default:
        if (--Timer_00[wk->wu.id]) {
            break;
        }
        if (Check_SHINRYU(wk)) {
            return 0;
        }
        Timer_00[wk->wu.id] = 1;
        return 1;
    }

    return 0;
}

s32 Exit_Term_0007(PLW* wk, WORK* em) {
    switch (Term_No[wk->wu.id]) {
    case 0:
        Term_No[wk->wu.id]++;
        Timer_00[wk->wu.id] = 0x1E;
        /* fallthrough */
    default:
        if (Check_Drop_Term(em, 0x28) != 0) {
            if (Area_Number[wk->wu.id] >= 3) {
                return 1;
            }
            VS_Tech[wk->wu.id] = 0x1A;
            return -1;
        }
        if (--Timer_00[wk->wu.id]) {
            break;
        }
        Timer_00[wk->wu.id] = 1;
        return 1;
    }

    return 0;
}

s32 Exit_Term_0008(PLW* wk, WORK* em) {
    switch (Term_No[wk->wu.id]) {
    case 0:
        Term_No[wk->wu.id]++;
        Timer_00[wk->wu.id] = 0xb4;
        /* fallthough */
    default:
        if ((wk->wu.old_pos[1] == 0) && (wk->wu.xyz[1].disp.pos == 0) && (wk->wu.routine_no[1] != 4)) {
            return 1;
        }
        if (--Timer_00[wk->wu.id] == 0) {
            return 1;
        }
    }

    return 0;
}

s32 Check_Drop_Term(WORK* em, s16 Y) {
    if (em->mvxy.a[1].real.h >= 0) {
        return 0;
    }
    if (em->xyz[1].disp.pos >= Y) {
        return 0;
    }
    return 1;
}

s32 Check_SHINRYU(PLW* wk) {
    PLW* em;

    em = (PLW*)wk->wu.target_adrs;

    if (em->sa->ok != -1) {
        return 0;
    }
    if (My_char[em->wu.id] != 0xB) {
        return 0;
    }
    if (gs.plw[em->wu.id].sa->kind_of_arts != 1) {
        return 0;
    }
    return 1;
}

const Term_Tbl_t ETC_Term_Tbl[10] = { ETC_Term_0000, ETC_Term_0001, ETC_Term_0002, ETC_Term_0003, ETC_Term_0004,
                                      ETC_Term_0005, ETC_Term_0006, ETC_Term_0007, ETC_Term_0008, ETC_Term_0009 };

void Check_BOSS(PLW* wk, u32 Next_Action, u16 Next_Menu) {
    if (Break_Into_CPU == 1) {
        Disposal_Again[wk->wu.id] = 1;
        CP_Index[wk->wu.id][0]++;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;

        Flip_Flag[wk->wu.id] = 0;
        Limited_Flag[wk->wu.id] = 0;
    } else {
        Disposal_Again[wk->wu.id] = 1;
        Next_Another_Menu(wk, Next_Action, Next_Menu);
    }
}

void Check_BOSS_EX(PLW* wk, u32 Next_Action, u16 Next_Menu) {
    if (Break_Into_CPU != 1) {
        Disposal_Again[wk->wu.id] = 1;
        CP_Index[wk->wu.id][0]++;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;

        Flip_Flag[wk->wu.id] = 0;
        Limited_Flag[wk->wu.id] = 0;
    } else {
        Disposal_Again[wk->wu.id] = 1;
        Next_Another_Menu(wk, Next_Action, Next_Menu);
    }
}

void ETC_Term(PLW* wk, s16 Exit_No, u32 Next_Action, u16 Next_Menu) {
    s16 xx;
    WORK* em = (WORK*)wk->wu.target_adrs;

    xx = ETC_Term_Tbl[Exit_No](wk, em);

    if (xx == -1) {
        return;
    }

    if (xx) {
        Disposal_Again[wk->wu.id] = 1;
        CP_Index[wk->wu.id][0]++;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;

        Flip_Flag[wk->wu.id] = 0;
        Limited_Flag[wk->wu.id] = 0;
    } else {
        Disposal_Again[wk->wu.id] = 1;
        Next_Another_Menu(wk, Next_Action, Next_Menu);
    }
}

s32 ETC_Term_0000(PLW* wk, WORK* em) {
    if (Check_VS_Squat(wk, em, 0x1D, 0x21, 0x20) != 0) {
        return 1;
    }
    return 0;
}

s32 ETC_Term_0001(PLW* wk, WORK* em) {
    if (wk->sa->ok != -1) {
        return 1;
    }
    if (My_char[wk->wu.id] != 9) {
        return 1;
    }
    if (gs.plw[wk->wu.id].sa->kind_of_arts) {
        return 1;
    }
    return 0;
}

s32 ETC_Term_0002(PLW* wk, WORK* em) {
    if (gs.plw[wk->wu.id].wu.vital_new < 0x31) {
        return 1;
    }
    return 0;
}

s32 ETC_Term_0003(PLW* wk, WORK* em) {
    if ((em->pat_status != 0x20) && (em->pat_status != 0x21) && (em->pat_status != 2)) {
        return 1;
    }
    return 0;
}

s32 ETC_Term_0004(PLW* wk, WORK* em) {
    if (em->vital_new >= 0x50) {
        return 0;
    }
    if (gs.plw[wk->wu.id].wu.vital_new < 0x78) {
        return 0;
    }
    return 1;
}

s32 ETC_Term_0005(PLW* wk, WORK* em) {
    if (em->vital_new < gs.plw[wk->wu.id].wu.vital_new) {
        return 1;
    }
    return 0;
}

s32 ETC_Term_0006(PLW* wk, WORK* em) {
    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        CP_Index[wk->wu.id][1]++;
        Timer_00[wk->wu.id] = 0x78;
        /* fallthrough */
    case 1:
        if (--Timer_00[wk->wu.id] == 0) {
            return 1;
        }
        if (PL_Distance[wk->wu.id] < 0x70) {
            return 1;
        }
        Lever_Buff[wk->wu.id] = 0x40;
        return -1;
    }
    return -1;
}

s32 ETC_Term_0007(PLW* wk, WORK* em) {
    if (gs.plw[wk->wu.id].sa->kind_of_arts == 2) {
        return 1;
    }

    return 0;
}

s32 ETC_Term_0008(PLW* wk, WORK* em) {
    if (gs.plw[wk->wu.id].sa->kind_of_arts == 1) {
        return 1;
    }

    return 0;
}

s32 ETC_Term_0009(PLW* wk, WORK* em) {
    if (gs.plw[wk->wu.id].sa->kind_of_arts == 0) {
        return 1;
    }

    return 0;
}

s32 emLevelRemake(s32 now, s32 max, s32 exd) {
    s32 RemakeLevelForDifficulty[8] = { -30, -10, 0, 0, 0, 0, 20, 60 };

    now += (max - exd) * RemakeLevelForDifficulty[save_w[Present_Mode].Difficulty] / 100;

    if (now < 0) {
        now = 0;
    }
    if (now >= max) {
        now = max - 1;
    }

    return now;
}

s32 emGetMaxBlocking() {
    s32 RapidBlockingTimes[8] = { 2, 2, 3, 3, 3, 4, 6, 10 };

    return RapidBlockingTimes[save_w[Present_Mode].Difficulty];
}
