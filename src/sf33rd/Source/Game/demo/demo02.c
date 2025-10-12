/**
 * @file demo02.c
 * Demo Sequence 2
 */

#include "common.h"
#include "sf33rd/Source/Game/Game.h"
#include "sf33rd/Source/Game/Grade.h"
#include "sf33rd/Source/Game/MMTMCNT.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/SysDir.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"

void Demo00();
void Demo01();
void Setup_Demo_Arts();
void Setup_Select_Demo_PL();

s32 Play_Demo() {
    void (*main_jmp_tbl[2])() = { Demo00, Demo01 };

    Next_Demo = 0;
    main_jmp_tbl[D_No[0]]();
    return Next_Demo;
}

void Demo00() {
    Play_Game = 1;

    switch (D_No[1]) {
    case 0:
        Switch_Screen(1);
        Purge_texcash_of_list(3);
        Make_texcash_of_list(3);
        D_No[1] += 1;
        G_No[2] = 0;
        Game_pause = 0;
        Conclusion_Flag = 0;
        appear_type = 1;
        Control_Time = 0x800;
        Round_Level = 7;
        Weak_PL = random_16() & 1;
        break;

    case 1:
        Switch_Screen(1);
        Game02();

        if (--Cover_Timer == 0) {
            D_No[1] += 1;
            Switch_Screen_Init(0);
            return;
        }

        break;

    case 2:
        Game02();

        if (Switch_Screen_Revival(0) != 0) {
            D_No[1] += 1;
            D_Timer = 1800;
            Stop_SG = 0;
            return;
        }

        break;

    case 3:
        Game02();

        if (Debug_w[0x18] == 9) {
            D_Timer = 60;
        }

        if (--D_Timer == 1) {
            D_No[1] += 1;
            Stop_Combo = 1;
            return;
        }

        if (Conclusion_Flag) {
            D_No[1] += 1;
            Stop_Combo = 1;
            D_Timer = 90;
            return;
        }

        break;

    case 4:
        Game02();

        if (--D_Timer == 0) {
            D_No[1] += 1;
            Game_pause = 1;
            Disappear_LOGO = 1;
            D_Timer = 16;
            return;
        }

        break;

    case 5:
        Game02();

        if (--D_Timer == 0) {
            D_No[1] += 1;
            Switch_Screen_Init(0);
            SsBgmFadeOut(0x800);
            return;
        }

        break;

    case 6:
        Game02();

        if (Switch_Screen(0) != 0) {
            D_No[1] += 1;
            Demo_Flag = 0;
            Present_Mode = 0;
            Cover_Timer = 23;
            BGM_Stop();

            if (++Select_Demo_Index > 3) {
                Select_Demo_Index = 0;
                return;
            }
        }

        break;

    default:
        Switch_Screen(1);
        Next_Demo = 1;
        break;
    }
}

void Demo01() {
    if (D_No[1] >= 2) {
        Play_Game = 1;
    }

    switch (D_No[1]) {
    case 0:
        Switch_Screen(1);
        D_No[1] += 1;
        Game_pause = 0;
        Demo_Time_Stop = 0;
        Before_Select_Sub();
        Setup_Select_Demo_PL();
        Setup_Demo_Arts();
        Weak_PL = random_16() & 1;
        Clear_Break_Com(0);
        grade_check_work_1st_init(0, 0);
        grade_check_work_1st_init(0, 1);
        Clear_Break_Com(1);
        grade_check_work_1st_init(1, 0);
        grade_check_work_1st_init(1, 1);
        Game01();
        break;

    case 1:
        Game01();

        if (Demo_Time_Stop) {
            D_No[1] += 1;
            G_No[2] = 0;
            return;
        }

        break;

    case 2:
        Switch_Screen(1);
        Game02();

        if (--Cover_Timer == 0) {
            D_No[1] += 1;
            Switch_Screen_Init(0);
            return;
        }

        break;

    case 3:
        Game02();

        if (Switch_Screen_Revival(0) != 0) {
            D_No[1] += 1;
            D_Timer = 1200;
            Stop_SG = 0;
            return;
        }

        break;

    case 4:
        Game02();

        if (--D_Timer == 1) {
            Stop_Combo = 1;
            Disappear_LOGO = 1;
            return;
        }

        if (!D_Timer) {
            D_No[1] += 1;
            D_Timer = 16;
            Demo_Time_Stop = 1;
            Game_pause = 1;
            return;
        }

        break;

    case 5:
        Game02();

        if (--D_Timer == 0) {
            D_No[1] += 1;
            Switch_Screen_Init(0);
            SsBgmFadeOut(0x800);
            return;
        }

        break;

    case 6:
        Game02();

        if (Switch_Screen(0) != 0) {
            D_No[1] += 1;
            Cover_Timer = 23;
            BGM_Stop();
            return;
        }

        break;

    default:
        Next_Demo = 1;
        break;
    }
}

const s8 Demo_PL_Play_Data[4][2] = { { 15, 19 }, { 11, 18 }, { 2, 16 }, { 12, 8 } };
const u8 Arts_Rnd_Demo_Data[8] = { 0, 0, 0, 1, 1, 1, 2, 2 };
const s8 Demo_Stage_Play_Data[4][2] = { { 15, 19 }, { 11, 18 }, { 2, 16 }, { 12, 8 } };
const s8 Demo_PL_Data[4] = { 0, 1, 0, 1 };

void Setup_Demo_PL() {
    My_char[0] = Demo_PL_Play_Data[Demo_PL_Index][0];
    My_char[1] = Demo_PL_Play_Data[Demo_PL_Index][1];

    if (Debug_w[0x1D]) {
        My_char[0] = Debug_w[0x1D] - 1;
    }

    if (Debug_w[0x1E]) {
        My_char[1] = Debug_w[0x1E] - 1;
    }

    init_omop();
}

void Setup_Demo_Arts() {
    Super_Arts[0] = Arts_Rnd_Demo_Data[random_16() & 7];
    Super_Arts[1] = Arts_Rnd_Demo_Data[random_16() & 7];
    Player_Color[0] = 0;
    Player_Color[1] = 0;
}

void Setup_Demo_Stage() {
    s16 rnd = random_16() & 1;

    bg_w.area = 0;
    bg_w.stage = Demo_Stage_Play_Data[Demo_Stage_Index][rnd];
    Demo_Stage_Index += 1;

    if (++Demo_PL_Index > 3) {
        Demo_PL_Index = 0;
        Demo_Stage_Index = 0;
    }
}

void Setup_Select_Demo_PL() {
    plw[0].wu.operator = 0;
    plw[1].wu.operator = 0;
    Operator_Status[0] = 0;
    Operator_Status[1] = 0;
    plw[Demo_PL_Data[Select_Demo_Index]].wu.operator = 1;
    Operator_Status[Demo_PL_Data[Select_Demo_Index]] = 1;
}
