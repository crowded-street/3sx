/**
 * @file next_cpu.c
 * Next CPU character selection
 */

#include "sf33rd/Source/Game/screen/next_cpu.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/eff38.h"
#include "sf33rd/Source/Game/effect/eff39.h"
#include "sf33rd/Source/Game/effect/eff42.h"
#include "sf33rd/Source/Game/effect/eff43.h"
#include "sf33rd/Source/Game/effect/eff58.h"
#include "sf33rd/Source/Game/effect/eff75.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/effect/eff98.h"
#include "sf33rd/Source/Game/effect/effa9.h"
#include "sf33rd/Source/Game/effect/effe0.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effk6.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/rendering/mmtmcnt.h"
#include "sf33rd/Source/Game/screen/sel_data.h"
#include "sf33rd/Source/Game/select_timer.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/sys_sub2.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

void Next_CPU_1st();
void Next_CPU_2nd();
void Next_CPU_3rd();
void Next_CPU_4th();
void Next_CPU_4th_0_Sub();
void Next_CPU_4th_1_Sub();
void Next_CPU_4th_2_Sub();
void Next_CPU_5th();
static u8 Check_EM_Speech();
void Next_CPU_6th();
void Wait_Load_Complete();
void Wait_Load_Complete2();
void Wait_Load_Complete3();
void After_Bonus_1st();
void After_Bonus_2nd();
void Select_CPU_1st();
void Select_CPU_2nd();
void NC_Cut_Sub();
void Select_CPU_3rd();
void Select_CPU_4th();
void Next_Bonus_1st();
void Next_Bonus_2nd();
void Next_Bonus_3rd();
void Next_Bonus_End();
void Next_Q_1st();
void Next_Q_2nd();
void Next_Q_3rd();
void Sel_CPU_Sub(s16 PL_id, u16 sw, u16 /* unused */);
void Setup_EM_List();
void Setup_Next_Fighter();
s8 Setup_Com_Arts();
void Setup_Com_Color();
void Setup_Regular_OBJ(s16 PL_id);
void Regular_OBJ_Sub(s16 PL_id, s16 Dir);
void Setup_History_OBJ();
void Setup_VS_OBJ(s16 Option);
s8 Check_Bonus_Type();
void Setup_Next_Stage(s16 dir_step);
void Check_Auto_Cut();

// sbss
u8 SEL_CPU_X;
s16 Start_X;

s16 Next_CPU() {
    void (*Next_CPU_Tbl[12])() = { Next_CPU_1st,   Next_CPU_2nd,   Next_CPU_3rd,       Next_CPU_4th,
                                   Next_CPU_5th,   Next_CPU_6th,   Next_Bonus_1st,     Next_Bonus_2nd,
                                   Next_Bonus_3rd, Next_Bonus_End, Wait_Load_Complete, Wait_Load_Complete2 };

    if (Break_Into) {
        return 0;
    }

    SEL_CPU_X = 0;
    Scene_Cut = Cut_Cut_Cut();
    Next_CPU_Tbl[SC_No[0]]();
    Time_Over = false;

    if (Check_Exit_Check() == 0 && Debug_w[0x18] == -1) {
        SEL_CPU_X = 0;
    }

    return SEL_CPU_X;
}

void Next_CPU_1st() {
    u16 Rnd;

    SC_No[0]++;
    Target_BG_X[3] = bg_w.bgw[3].wxy[0].disp.pos + 458;
    Offset_BG_X[3] = 0;
    Start_X = bg_w.bgw[3].wxy[0].disp.pos;
    bg_mvxy.a[0].sp = 0x40000;
    bg_mvxy.d[0].sp = 0;
    Sel_EM_Complete[Player_id] = 0;
    Temporary_EM[Player_id] = Last_Selected_EM[Player_id];
    Select_Timer = 0x20;
    Setup_EM_List();

    if (VS_Index[Player_id] == 0) {
        effect_A9_init(32, 0, 0, 0);
    } else {
        Setup_History_OBJ();

        if (VS_Index[Player_id] < 9) {
            Setup_Next_Stage(58);
        } else {
            Setup_Next_Stage(59);
        }
    }

    Setup_Regular_OBJ(Player_id);
    Moving_Plate[Player_id] = 0;

    if (G_No[1] == 5) {
        BGM_Request(57);
        Order[56] = 3;
        Order_Timer[56] = 1;
    }

    Time_Stop = 1;
    Unit_Of_Timer = UNIT_OF_TIMER_MAX;
    SelectTimer_Init();
    Rnd = random_16() & 3;
    effect_58_init(6, 10, EM_Select_Voice_Data[Rnd]);
    Next_Step = 0;
    Suicide[2] = 1;
    Cut_Scroll = 2;
    effect_58_init(13, 1, 3);
    effect_58_init(16, 5, 2);
}

void Next_CPU_2nd() {
    NC_Cut_Sub();
}

void Next_CPU_3rd() {
    switch (SC_No[1]) {
    case 0:
        if (Player_id) {
            Sel_CPU_Sub(1, ~p2sw_1 & p2sw_0, p2sw_0);
        } else {
            Sel_CPU_Sub(0, ~p1sw_1 & p1sw_0, p1sw_0);
        }

        if (!Sel_EM_Complete[Player_id]) {
            break;
        }

        SC_No[1]++;
        SC_No[2] = 0;

        if (Debug_w[0x1D]) {
            My_char[0] = Debug_w[0x1D] - 1;
        }

        if (Debug_w[0x1E]) {
            My_char[1] = Debug_w[0x1E] - 1;
        }

        Push_LDREQ_Queue_Player(COM_id, My_char[COM_id]);
        Setup_Next_Fighter();

        if (Debug_w[0x1D]) {
            My_char[0] = Debug_w[0x1D] - 1;
        }

        if (Debug_w[0x1E]) {
            My_char[1] = Debug_w[0x1E] - 1;
        }

        if (VS_Index[Player_id] < 8) {
            S_Timer = 50;
            break;
        }

        SC_No[1] = 2;
        S_Timer = 100;
        break;

    case 1:
        switch (SC_No[2]) {
        case 0:
            if (S_Timer < 10) {
                S_Timer = 9;
                SC_No[2]++;
                SsBgmFadeOut(0x1000);
            }

            break;
        }

        if ((S_Timer -= 1) == 0) {
            SC_No[0]++;
            SC_No[1] = 0;
            SC_No[2] = 0;
        }

        break;

    case 2:
        if ((S_Timer -= 1) < 71) {
            if (Check_EM_Speech() == 0) {
                SC_No[1]++;
            } else {
                SC_No[0] = 4;
                SC_No[1] = 0;
            }

            SC_No[2] = 0;
            break;
        }

        break;

    case 3:
        switch (SC_No[2]) {
        case 0:
            if (Scene_Cut) {
                S_Timer = 9;
            }

            if (S_Timer < 10) {
                S_Timer = 9;
                SC_No[2]++;
                SsBgmFadeOut(0x1000U);
            }

            break;
        }

        if ((S_Timer -= 1) == 0) {
            SC_No[0]++;
            SC_No[1] = 0;
            SC_No[2] = 0;
        }

        break;
    }
}

void Next_CPU_4th() {
    switch (SC_No[1]) {
    case 0:
        FadeInit();
        Next_CPU_4th_0_Sub();
        break;

    case 1:
        Next_CPU_4th_1_Sub();
        break;

    case 2:
        Next_CPU_4th_2_Sub();
        break;

    default:
        if (Scene_Cut) {
            S_Timer = 1;
        }

        if ((S_Timer -= 1) != 0) {
            break;
        }

        if (G_No[1] == 5 || G_No[1] == 10) {
            SC_No[0] = 10;
            break;
        }

        SC_No[0] = 6;
        break;
    }
}

void Next_CPU_4th_0_Sub() {
    FadeIn(0, 4, 8);
    SC_No[1]++;
    Forbid_Break = 0;
    bgPalCodeOffset[0] = 144;
    BGM_Request(51);
    S_Timer = 178;
    Exit_Timer = 2;
    bg_w.bgw[0].wxy[0].disp.pos += 512;
    bg_w.bgw[1].wxy[1].disp.pos = 512;
    bg_w.bgw[3].wxy[1].disp.pos += 512;
    Setup_BG(0, bg_w.bgw[0].wxy[0].disp.pos, bg_w.bgw[0].wxy[1].disp.pos);
    Setup_BG(1, bg_w.bgw[1].wxy[0].disp.pos + 512, bg_w.bgw[1].wxy[1].disp.pos);
    Setup_BG(3, bg_w.bgw[3].wxy[0].disp.pos, bg_w.bgw[3].wxy[1].disp.pos);
    Setup_VS_OBJ(0);
    Suicide[0] = 1;
    FadeInit();
}

void Next_CPU_4th_1_Sub() {
    FadeIn(0, 4, 8);

    if ((Exit_Timer -= 1) == 0) {
        SC_No[1]++;
    }
}

void Next_CPU_4th_2_Sub() {
    S_Timer--;

    if (!FadeIn(0, 4, 8)) {
        return;
    }

    SC_No[1]++;

    if (S_Timer < 0) {
        S_Timer = 1;
    }
}

void Next_CPU_5th() {
    switch (SC_No[1]) {
    case 0:
        SC_No[1]++;
        Switch_Screen_Init(1);
        break;

    case 1:
        if (Switch_Screen(1) != 0) {
            SC_No[1]++;
            Cover_Timer = 9;
        }

        break;

    case 2:
        Switch_Screen(1);
        SC_No[1]++;
        bgPalCodeOffset[0] = 144;
        bg_w.bgw[0].wxy[0].disp.pos += 512;
        bg_w.bgw[1].wxy[1].disp.pos = 512;
        bg_w.bgw[3].wxy[1].disp.pos += 512;
        Setup_BG(0, bg_w.bgw[0].wxy[0].disp.pos, bg_w.bgw[0].wxy[1].disp.pos);
        Setup_BG(1, bg_w.bgw[1].wxy[0].disp.pos, bg_w.bgw[1].wxy[1].disp.pos);
        Setup_BG(3, bg_w.bgw[3].wxy[0].disp.pos, bg_w.bgw[3].wxy[1].disp.pos);
        Setup_VS_OBJ(1);
        Suicide[0] = 1;
        Next_Step = 0;
        Order[67] = 1;
        Order_Timer[67] = 10;
        Order_Dir[67] = 8;
        effect_76_init(67);
        Order[68] = 1;
        Order_Timer[68] = 10;
        Order_Dir[68] = 4;
        effect_76_init(68);
        break;

    case 3:
        Switch_Screen(1);

        if ((Cover_Timer -= 1) == 0) {
            SC_No[1]++;
            Switch_Screen_Init(1);
        }

        break;

    case 4:
        if (Switch_Screen_Revival(1) != 0) {
            SC_No[1]++;
            Forbid_Break = 0;
        }

        break;

    case 5:
        if ((Next_Step & 0x80) != 0) {
            SC_No[1]++;
            S_Timer = 8;
            SsBgmFadeOut(0x1000);
        }

        break;

    case 6:
        if (!(S_Timer -= 1)) {
            FadeInit();
            FadeIn(0, 4, 8);
            SC_No[1]++;
            Forbid_Break = 0;
            Suicide[3] = 1;
            effect_43_init(1, 0);
            BGM_Request(0x33);
            S_Timer = 0xb2;
        }

        break;

    case 7:
        S_Timer--;

        if (FadeIn(0, 4, 8)) {
            SC_No[1]++;

            if (S_Timer < 0) {
                S_Timer = 1;
            }

            Introduce_Boss[Player_id][VS_Index[Player_id] - 8] |= 1;
        }

        break;

    default:
        if (Scene_Cut) {
            S_Timer = 1;
        }

        if ((S_Timer -= 1) == 0) {
            SC_No[0] = 10;
        }

        break;
    }
}

u8 Check_EM_Speech() {
    if (Introduce_Boss[Player_id][VS_Index[Player_id] - 8] & 1) {
        return 0;
    }

    return Boss_Speech_Data[My_char[Player_id]][VS_Index[Player_id] - 8];
}

void Next_CPU_6th() {
    SEL_CPU_X = 1;
}

void Wait_Load_Complete() {
    if (!Check_PL_Load()) {
        return;
    }

    if (!Check_LDREQ_Queue_BG(bg_w.stage + 0)) {
        return;
    }

    if (!adx_now_playend()) {
        return;
    }

    if (!sndCheckVTransStatus(0)) {
        return;
    }

    SEL_CPU_X = 1;
    init_omop();
    SC_No[0] = 5;
}

void Wait_Load_Complete2() {
    if (!Check_PL_Load()) {
        return;
    }

    if (!Check_LDREQ_Queue_BG(bg_w.stage + 0)) {
        return;
    }

    if (!adx_now_playend()) {
        return;
    }

    if (!sndCheckVTransStatus(0)) {
        return;
    }

    SEL_CPU_X = 2;
    init_omop();
    SC_No[0] = 10;
}

void Wait_Load_Complete3() {
    if (!Check_PL_Load()) {
        return;
    }

    if (!Check_LDREQ_Queue_BG(bg_w.stage + 0)) {
        return;
    }

    if (!adx_now_playend()) {
        return;
    }

    if (!sndCheckVTransStatus(0)) {
        return;
    }

    SEL_CPU_X = 2;
    init_omop();
    SC_No[0] = 7;
}

s32 After_Bonus() {
    void (*After_Bonus_Tbl[11])() = { After_Bonus_1st, After_Bonus_2nd, Next_CPU_1st,        Next_CPU_2nd,
                                      Next_CPU_3rd,    Next_CPU_4th,    Wait_Load_Complete2, Next_Bonus_End,
                                      Next_Bonus_End,  Next_Bonus_End,  Wait_Load_Complete3 };

    if (Break_Into) {
        return 0;
    }

    SEL_CPU_X = 0;
    Scene_Cut = Cut_Cut_Cut();
    After_Bonus_Tbl[SC_No[0]]();
    Time_Over = false;
    return SEL_CPU_X;
}

void After_Bonus_1st() {
    Switch_Screen(0);
    SC_No[0]++;
    Cover_Timer = 23;
    All_Clear_Suicide();
    System_all_clear_Level_B();
    base_y_pos = 40;
    bg_etc_write(2);
    Setup_Virtual_BG(0, 0x100, 0);
    Setup_BG(2, 0x300, 0);
    Setup_BG(1, 0x200, 0);
    Setup_BG(3, 0x2C0, 0);
    Unsubstantial_BG[0] = 1;
}

void After_Bonus_2nd() {
    switch (SC_No[1]) {
    case 0:
        Switch_Screen(0);
        Purge_mmtm_area(7);
        Purge_com_player_from_mm();
        Make_texcash_of_list(7);
        SC_No[1]++;
        effect_76_init(55);
        Order[55] = 3;
        Order_Timer[55] = 1;
        effect_76_init(56);
        Order[56] = 3;
        Order_Timer[56] = 1;
        /* fallthrough */

    case 1:
        Switch_Screen(1);

        if ((Cover_Timer -= 1) == 0) {
            SC_No[1]++;
            Clear_Flash_No();
            Switch_Screen_Init(1);
        }

        break;

    case 2:
        if (Switch_Screen_Revival(1) != 0) {
            SC_No[0]++;
            SC_No[1] = 0;
            S_Timer = 30;
            BGM_Request(57);
            Forbid_Break = 0;
            Ignore_Entry[LOSER] = 0;
        }

        break;
    }
}

s16 Select_CPU_First() {
    void (*Select_CPU_First_Tbl[4])() = { Select_CPU_1st, Select_CPU_2nd, Select_CPU_3rd, Select_CPU_4th };

    if (Break_Into) {
        return 0;
    }

    SEL_CPU_X = 0;
    Select_CPU_First_Tbl[SC_No[0]]();
    Time_Over = false;
    return SEL_CPU_X;
}

void Select_CPU_1st() {
    SC_No[0]++;
    Sel_EM_Complete[Player_id] = 0;
    Temporary_EM[Player_id] = Last_Selected_EM[Player_id];
    Select_Timer = 0x20;
    Setup_EM_List();
    Target_BG_X[3] = bg_w.bgw[3].wxy[0].disp.pos + 458;
    Offset_BG_X[3] = 0;

    if (VS_Index[Player_id] == 0) {
        bg_mvxy.a[0].sp = 0xA0000;
        bg_mvxy.d[0].sp = 0x18000;
        effect_A9_init(32, 0, 0, 1);
    } else {
        Setup_History_OBJ();
        bg_mvxy.a[0].sp = 0x40000;
        bg_mvxy.d[0].sp = 0;

        if (VS_Index[Player_id] < 9) {
            Setup_Next_Stage(58);
        } else {
            Setup_Next_Stage(59);
        }

        effect_76_init(66);
        Order[66] = 3;
        Order_Timer[66] = 1;
    }

    Setup_Regular_OBJ(Player_id);
    Moving_Plate[Player_id] = 0;

    if (VS_Index[Player_id] >= 8) {
        Push_LDREQ_Queue_Direct(9, 2);
    }
}

void Select_CPU_2nd() {
    u16 Rnd;

    switch (SC_No[1]) {
    case 0:
        SC_No[1]++;
        Order[Aborigine + 13] = 5;
        Order_Timer[Aborigine + 13] = 1;
        Order[Aborigine + 31] = 5;
        Order_Timer[Aborigine + 31] = 1;
        Order[Aborigine + 25] = 5;
        Order_Timer[Aborigine + 25] = 1;
        Order[37] = 4;
        Order_Timer[37] = 1;
        Rnd = random_16() & 3;
        effect_58_init(6, 10, EM_Select_Voice_Data[Rnd]);
        Cut_Scroll = 2;
        Next_Step = 0;
        effect_58_init(12, 1, 3);
        /* fallthrough */

    case 1:
        NC_Cut_Sub();
        break;
    }
}

void NC_Cut_Sub() {
    Check_Auto_Cut();

    if (Next_Step) {
        SC_No[0]++;
        SC_No[1] = 0;
        Time_Stop = 0;
    }
}

void Select_CPU_3rd() {
    switch (SC_No[1]) {
    case 0:
        if (Demo_Flag == 0) {
            if (Player_id) {
                Sel_CPU_Sub(1, Check_Demo_Data(1), 0);
            } else {
                Sel_CPU_Sub(0, Check_Demo_Data(0), 0);
            }
        } else if (Player_id) {
            Sel_CPU_Sub(1, ~p2sw_1 & p2sw_0, p2sw_0);
        } else {
            Sel_CPU_Sub(0, ~p1sw_1 & p1sw_0, p1sw_0);
        }

        if (!Sel_EM_Complete[Player_id]) {
            break;
        }

        SC_No[1]++;

        if (Debug_w[29]) {
            My_char[0] = Debug_w[29] - 1;
        }

        if (Debug_w[30]) {
            My_char[1] = Debug_w[30] - 1;
        }

        Push_LDREQ_Queue_Player(COM_id, My_char[COM_id]);
        Setup_Next_Fighter();

        if (Debug_w[29]) {
            My_char[0] = Debug_w[29] - 1;
        }

        if (Debug_w[30]) {
            My_char[1] = Debug_w[30] - 1;
        }

        if (VS_Index[Player_id] < 8) {
            S_Timer = 50;
        } else {
            SC_No[1] = 2;
            S_Timer = 100;
        }

        break;

    case 1:
        if ((S_Timer -= 1) == 0) {
            SC_No[1] = 4;
        }

        break;

    case 2:
        if ((S_Timer -= 1) < 51) {
            if (Check_LDREQ_Queue_Direct(9)) {
                SC_No[1]++;
            } else {
                S_Timer = 1;
            }
        }

        break;

    case 3:
        if (Scene_Cut) {
            S_Timer = 1;
        }

        if ((S_Timer -= 1) == 0) {
            SC_No[1]++;
        }

        break;

    case 4:
        SC_No[1] = 6;
        Order[Player_id + 11] = 4;
        Order_Timer[Player_id + 11] = 5;
        effect_38_init(COM_id, COM_id + 11, My_char[COM_id], 1, 2);
        Order[COM_id + 11] = 1;
        Order_Timer[COM_id + 11] = 1;

        if (check_use_all_SA() == 0 && check_without_SA() == 0 && EM_id != 0) {
            effect_98_init(COM_id, COM_id + 0x28, Super_Arts[COM_id], 2);
            Order[COM_id + 40] = 1;
            Order_Timer[COM_id + 40] = 1;
        }

        effect_75_init(42, 3, 2);
        Order[42] = 3;
        Order_Timer[42] = 1;
        Order_Dir[42] = 3;
        Target_BG_X[3] = bg_w.bgw[3].wxy[0].disp.pos + 480;
        Offset_BG_X[3] = 0;

        if (8 <= VS_Index[Player_id] && Check_EM_Speech()) {
            SC_No[1] = 5;
            Order[67] = 1;
            Order_Timer[67] = 10;
            Order_Dir[67] = 8;
            effect_76_init(67);
            Order[68] = 1;
            Order_Timer[68] = 10;
            Order_Dir[68] = 4;
            effect_76_init(68);
        }

        Next_Step = 0;
        Cut_Scroll = 2;
        bg_mvxy.a[0].sp = 0x200000;
        bg_mvxy.d[0].sp = 0x18000;
        effect_58_init(12, 1, 3);
        break;

    case 5:
        if (Next_Step & 0x80) {
            SC_No[1] = 7;
            S_Timer = 20;
            Introduce_Boss[Player_id][VS_Index[Player_id] - 8] = 1;
        }

        break;

    case 6:
        if (Next_Step & 1) {
            SC_No[1]++;
            S_Timer = 20;
        }

        break;

    case 7:
        switch (SC_No[2]) {
        case 0:
            if (Scene_Cut) {
                S_Timer = 9;
            }

            if (S_Timer < 10) {
                S_Timer = 9;
                SC_No[2]++;
                SsBgmFadeOut(0x1000);
            }

            break;
        }

        if ((S_Timer -= 1) == 0) {
            SC_No[0]++;
            SC_No[1] = 0;
            SC_No[2] = 0;
        }

        break;
    }
}

void Select_CPU_4th() {
    SEL_CPU_X = 1;
    Next_Step = 1;
    init_omop();
}

void Next_Bonus_1st() {
    u16 Rnd;

    SC_No[0]++;
    Target_BG_X[3] = bg_w.bgw[3].wxy[0].disp.pos + 458;
    Offset_BG_X[3] = 0;
    Start_X = bg_w.bgw[3].wxy[0].disp.pos;
    bg_mvxy.a[0].sp = 0x40000;
    bg_mvxy.d[0].sp = 0;
    Setup_History_OBJ();
    Setup_Next_Stage(60);
    BGM_Request(57);
    Order[56] = 3;
    Order_Timer[56] = 1;
    Rnd = random_16() & 3;
    effect_58_init(6, 10, EM_Select_Voice_Data[Rnd]);
    Suicide[2] = 1;
    Next_Step = 0;
    Cut_Scroll = 2;
    effect_58_init(13, 1, 3);
    effect_58_init(16, 5, 2);
}

void Next_Bonus_2nd() {
    switch (SC_No[1]) {
    case 0:
        Check_Auto_Cut();

        if (Next_Step) {
            SC_No[1]++;
            SC_No[2] = 0;
            S_Timer = 90;
            effect_58_init(6, 5, 160);
        }

        break;

    case 1:
        switch (SC_No[2]) {
        case 0:
            if (Scene_Cut) {
                S_Timer = 9;
            }

            if (S_Timer < 10) {
                S_Timer = 9;
                SC_No[2]++;
                SsBgmFadeOut(0x1000);
            }

            break;
        }

        if ((S_Timer -= 1) == 0) {
            SC_No[0]++;
            SC_No[1] = 0;
            SC_No[2] = 0;
        }

        break;
    }
}

void Next_Bonus_3rd() {
    switch (SC_No[1]) {
    case 0:
        My_char[COM_id] = Bonus_Type;
        Next_CPU_4th_0_Sub();
        break;

    case 1:
        Next_CPU_4th_1_Sub();
        break;

    case 2:
        Next_CPU_4th_2_Sub();
        break;

    default:
        if (Scene_Cut) {
            S_Timer = 1;
        }

        if ((S_Timer -= 1) == 0) {
            if (Check_PL_Load() == 0) {
                S_Timer = 1;
                break;
            }

            SC_No[0] = 11;
        }

        break;
    }
}

void Next_Bonus_End() {
    SEL_CPU_X = 2;
}

s16 Next_Q() {
    void (*Next_Q_Tbl[6])() = {
        Next_Q_1st, Next_Q_2nd, Next_Q_3rd, Wait_Load_Complete, Wait_Load_Complete, Next_CPU_6th
    };

    if (Break_Into) {
        return 0;
    }

    SEL_CPU_X = 0;
    Scene_Cut = Cut_Cut_Cut();
    Next_Q_Tbl[SC_No[0]]();

    if (Check_Exit_Check() == 0 && Debug_w[0x18] == -1) {
        SEL_CPU_X = 0;
    }

    Time_Over = false;
    return SEL_CPU_X;
}

void Next_Q_1st() {
    After_Bonus_1st();
    Setup_ID();
    EM_id = 17;
    Setup_Next_Fighter();
    Purge_mmtm_area(8);
    Purge_com_player_from_mm();
    Make_texcash_of_list(7);
    Push_LDREQ_Queue_Player(COM_id, 17);
}

void Next_Q_2nd() {
    switch (SC_No[1]) {
    case 0:
        SC_No[1]++;
        /* fallthrough */

    case 1:
        Switch_Screen(0);

        if ((Cover_Timer -= 1) == 5) {
            SC_No[1]++;
            effect_work_quick_init();
            bg_w.bgw[0].wxy[0].disp.pos += 512;
            Setup_BG(0, bg_w.bgw[0].wxy[0].disp.pos, bg_w.bgw[0].wxy[1].disp.pos);
            Setup_VS_OBJ(1);
        }

        break;

    case 2:
        Switch_Screen(0);

        if ((Cover_Timer -= 1) == 0) {
            SC_No[1]++;
            Clear_Flash_No();
            Switch_Screen_Init(1);
        }

        break;

    case 3:
        if (Switch_Screen_Revival(1U) != 0) {
            SC_No[0]++;
            SC_No[1] = 0;
            S_Timer = 10;
            Forbid_Break = 0;
            Ignore_Entry[LOSER] = 0;
        }

        break;
    }
}

void Next_Q_3rd() {
    switch (SC_No[1]) {
    case 0:
        if ((S_Timer -= 1) == 0) {
            SC_No[1]++;
        }

        break;

    case 1:
        FadeInit();
        FadeIn(0, 4, 8);
        SC_No[1]++;
        Forbid_Break = 0;
        effect_43_init(1, 0);
        bgPalCodeOffset[0] = 144;
        BGM_Request(51);
        S_Timer = 180;
        effect_58_init(15, 5, 0);
        return;

    case 2:
        Next_CPU_4th_2_Sub();
        return;

    default:
        if (Scene_Cut) {
            S_Timer = 1;
        }

        if ((S_Timer -= 1) == 0) {
            SC_No[0]++;
        }

        break;
    }
}

void Sel_CPU_Sub(s16 PL_id, u16 sw, u16 /* unused */) {
    u16 lever_sw;

    if (Sel_EM_Complete[PL_id]) {
        return;
    }

    if (Moving_Plate[PL_id]) {
        return;
    }

    if (Time_Over) {
        sw = SWK_WEST;
    }

    if (VS_Index[PL_id] >= 8) {
        sw = SWK_WEST;
    }

    lever_sw = sw & (SWK_UP | SWK_DOWN);

    if (lever_sw & SWK_DOWN) {
        if (Temporary_EM[Player_id] == 2) {
            return;
        }

        Sound_SE(PL_id + 96);
        Moving_Plate[PL_id] = 2;
        Moving_Plate_Counter[PL_id] = 2;
        Temporary_EM[Player_id] = 2;
    }

    if (lever_sw & SWK_UP) {
        if (Temporary_EM[Player_id] == 1) {
            return;
        }

        Sound_SE(PL_id + 96);
        Moving_Plate[PL_id] = 1;
        Moving_Plate_Counter[PL_id] = 2;
        Temporary_EM[Player_id] = 1;
    }

    if (sw & SWK_ATTACKS) {
        Sel_EM_Complete[PL_id] = 1;
        EM_id = EM_List[Player_id][Temporary_EM[Player_id] - 1];
        My_char[COM_id] = EM_id;
        Time_Stop = 2;

        if (VS_Index[PL_id] < 8) {
            Sound_SE(ID + 98);
            Sound_SE(Voice_EM_Random_Data[random_16()]);
        }

        Last_Selected_EM[PL_id] = Temporary_EM[PL_id];
    }
}

void Setup_EM_List() {
    if (My_char[Player_id] == 0) {
        EM_Candidate[Player_id][0][9] = 1;
        EM_Candidate[Player_id][1][9] = 1;
    } else {
        EM_Candidate[Player_id][0][9] = 0;
        EM_Candidate[Player_id][1][9] = 0;
    }

    EM_List[Player_id][0] = EM_Candidate[Player_id][0][VS_Index[Player_id]];
    EM_List[Player_id][1] = EM_Candidate[Player_id][1][VS_Index[Player_id]];
}

void Setup_Next_Fighter() {
    paring_counter[COM_id] = 0;
    paring_bonus_r[COM_id] = 0;
    My_char[COM_id] = EM_id;

    if (EM_id == 17) {
        Battle_Country = Q_Country;
        bg_w.stage = Q_Country;
    } else {
        Battle_Country = EM_id;

        if (My_char[Player_id] == 0 && EM_id == 1) {
            Battle_Country = 0;
        }

        bg_w.stage = Battle_Country;
    }

    if (Debug_w[31]) {
        Battle_Country = bg_w.stage = Debug_w[31] - 1;
    }

    Push_LDREQ_Queue_BG(bg_w.stage + 0);
    bg_w.area = 0;
    Super_Arts[COM_id] = Stock_Com_Arts[Player_id] = Setup_Com_Arts();

    if (Debug_w[32]) {
        Super_Arts[COM_id] = bg_w.stage = Debug_w[32] - 1;
    }

    Setup_Com_Color();
    Setup_PL_Color(COM_id, Com_Color_Shot);
}

const u8 Arts_Rnd_Data[8] = { 0, 0, 0, 1, 1, 1, 2, 2 };

s8 Setup_Com_Arts() {
    if (EM_id == 0) {
        return 1;
    }

    if (Stock_Com_Arts[Player_id] == -1) {
        return Arts_Rnd_Data[random_16() & 7];
    }

    return Stock_Com_Arts[Player_id];
}

void Setup_Com_Color() {
    Com_Color_Shot = Stock_Com_Color[Player_id];

    if (Break_Com[Player_id][EM_id]) {
        Com_Color_Shot = 1024;
        return;
    }

    Com_Color_Shot = 16;
}

void Setup_PL_Color(s16 PL_id, u16 sw) {
    s8 id_0;
    s8 id_1;
    u16 sw_new;

    sw_new = 0;

    if (plw[PL_id ^ 1].wu.operator == 0) {
        id_0 = -1;
        id_1 = 1;
    } else {
        id_0 = My_char[PL_id];
        id_1 = My_char[PL_id ^ 1];
    }

    if (Sel_PL_Complete[PL_id ^ 1] == 0) {
        id_0 = 127;
    }

    if (plw[PL_id].wu.operator != 0 && My_char[PL_id] == 0) {
        sw_new = 0;
    } else {
        if (Debug_w[53]) {
            if (PL_id == 0) {
                sw_new = p1sw_0;
            } else {
                sw_new = p2sw_0;
            }
        }

        if (save_w[Present_Mode].PL_Color[PL_id][My_char[PL_id]]) {
            if (PL_id == 0) {
                sw_new = p1sw_0;
            } else {
                sw_new = p2sw_0;
            }
        }
    }

    if (My_char[PL_id] == 0) {
        switch (sw) {
        case 16:
        case 32:
        case 64:
            if (Player_Color[PL_id ^ 1] == 0 && id_0 == id_1) {
                Player_Color[PL_id] = 1;
            } else {
                Player_Color[PL_id] = 0;
            }

            break;

        default:
            if (Player_Color[PL_id ^ 1] == 1 && id_0 == id_1) {
                Player_Color[PL_id] = 0;
            } else {
                Player_Color[PL_id] = 1;
            }

            break;
        }

        return;
    }

    if (sw_new & 0x4000) {
        switch (sw) {
        case 16:
            if (Player_Color[PL_id ^ 1] == 7 && id_0 == id_1) {
                Player_Color[PL_id] = 10;
                break;
            }

            Player_Color[PL_id] = 7;
            break;

        case 32:
            if (Player_Color[PL_id ^ 1] == 8 && id_0 == id_1) {
                Player_Color[PL_id] = 11;
                break;
            }

            Player_Color[PL_id] = 8;
            break;

        case 64:
            if (Player_Color[PL_id ^ 1] == 9 && id_0 == id_1) {
                Player_Color[PL_id] = 12;
                break;
            }

            Player_Color[PL_id] = 9;
            break;

        case 256:
            if (Player_Color[PL_id ^ 1] == 10 && id_0 == id_1) {
                Player_Color[PL_id] = 7;
                break;
            }

            Player_Color[PL_id] = 10;
            break;

        case 512:
            if (Player_Color[PL_id ^ 1] == 11 && id_0 == id_1) {
                Player_Color[PL_id] = 8;
                break;
            }

            Player_Color[PL_id] = 11;
            break;

        default:
            if (Player_Color[PL_id ^ 1] == 12 && id_0 == id_1) {
                Player_Color[PL_id] = 9;
                break;
            }

            Player_Color[PL_id] = 12;
            break;
        }
    } else {
        switch (sw) {
        case 592:
            if (Player_Color[PL_id ^ 1] == 6 && id_0 == id_1) {
                Player_Color[PL_id] = 0;
                break;
            }

            Player_Color[PL_id] = 6;
            break;

        case 16:
            if (Player_Color[PL_id ^ 1] == 0 && id_0 == id_1) {
                Player_Color[PL_id] = 3;
                break;
            }

            Player_Color[PL_id] = 0;
            break;

        case 32:
            if (Player_Color[PL_id ^ 1] == 1 && id_0 == id_1) {
                Player_Color[PL_id] = 4;
                break;
            }

            Player_Color[PL_id] = 1;
            break;

        case 64:
            if (Player_Color[PL_id ^ 1] == 2 && id_0 == id_1) {
                Player_Color[PL_id] = 5;
                break;
            }

            Player_Color[PL_id] = 2;
            break;

        case 256:
            if (Player_Color[PL_id ^ 1] == 3 && id_0 == id_1) {
                Player_Color[PL_id] = 0;
                break;
            }

            Player_Color[PL_id] = 3;
            break;

        case 512:
            if (Player_Color[PL_id ^ 1] == 4 && id_0 == id_1) {
                Player_Color[PL_id] = 1;
                break;
            }

            Player_Color[PL_id] = 4;
            break;

        default:
            if (Player_Color[PL_id ^ 1] == 5 && id_0 == id_1) {
                Player_Color[PL_id] = 2;
                break;
            }

            Player_Color[PL_id] = 5;
            break;
        }
    }
}

void Setup_Regular_OBJ(s16 PL_id) {
    s16 em_id;

    if (VS_Index[Player_id] < 8) {
        Regular_OBJ_Sub(PL_id, 2);
        Regular_OBJ_Sub(PL_id, 1);
        effect_A9_init(16, 5, 10, 0);
        effect_42_init(9);
        effect_42_init(10);
        Order[9] = 0;
        Order[10] = 0;
        Order_Timer[9] = 1;
        Order_Timer[10] = 1;
        return;
    }

    effect_A9_init(33, EM_List[PL_id][1], 5, 0);
    effect_A9_init(12, EM_List[PL_id][1], 21, 0);
    effect_A9_init(57, 0, 22, 0);
    em_id = EM_List[PL_id][1];

    if (chkNameAkuma(em_id, 1)) {
        em_id = 23;
    }

    effect_A9_init(34, em_id, 20, 0);
}

void Regular_OBJ_Sub(s16 PL_id, s16 Dir) {
    s16 ix = Dir - 1;
    s16 x;

    effect_A9_init(33, EM_List[PL_id][ix], ix + 4, 0);
    x = chkNameAkuma(EM_List[PL_id][ix], 9);
    effect_A9_init(34, x + EM_List[PL_id][ix], ix + 6, 0);
    effect_A9_init(12, EM_List[PL_id][ix], ix + 8, 0);
    effect_E0_init(Dir, 0, 0);
    effect_E0_init(Dir, 1, 0);
}

void Setup_History_OBJ() {
    s16 q_index = Break_Com[Player_id][17];
    s16 xx;
    s16 ix;
    s16 grade;

    effect_A9_init(79, 12, 11, 0);
    Offset_BG_X[3] = 88;
    effect_A9_init(79, 13, 12, 0);
    Offset_BG_X[3] += 80;

    for (xx = 0; xx < VS_Index[Player_id]; xx++) {
        effect_A9_init(79, 13, 12, 0);
        effect_A9_init(79, xx, 13, 0);
        effect_A9_init(79, 10, 14, 0);
        ix = chkNameAkuma(EM_History[Player_id][xx], 6);
        effect_A9_init(81, ix + EM_History[Player_id][xx], 15, 0);
        effect_A9_init(12, EM_History[Player_id][xx], 16, 0);
        grade = judge_final[Player_id][0].vs_cpu_grade[xx];

        if (grade == -1) {
            grade = 0;
        }

        effect_A9_init(80, grade, 17, 0);
        Offset_BG_X[3] += 88;

        if (q_index == 0 || (q_index - 1) != xx) {
            continue;
        }

        effect_A9_init(79, 13, 12, 0);
        effect_A9_init(81, 17, 15, 0);
        effect_A9_init(12, 17, 16, 0);
        grade = judge_final[Player_id]->vs_cpu_grade[15];

        if (grade == -1) {
            grade = 0;
        }

        effect_A9_init(80, grade, 17, 0);
        Offset_BG_X[3] += 88;
    }

    Offset_BG_X[3] -= 40;
}

void Setup_VS_OBJ(s16 Option) {
    effect_38_init(0, 11, My_char[0], 1, 0);
    Order[11] = 3;
    Order_Timer[11] = 1;
    effect_38_init(1, 12, My_char[1], 1, 0);
    Order[12] = 3;
    Order_Timer[12] = 1;
    effect_K6_init(0, 35, 35, 0);
    Order[35] = 3;
    Order_Timer[35] = 1;
    effect_K6_init(1, 36, 35, 0);
    Order[36] = 3;
    Order_Timer[36] = 1;
    effect_39_init(0, 17, My_char[0], 0, 0);
    Order[17] = 3;
    Order_Timer[17] = 1;
    effect_39_init(1, 18, My_char[1], 0, 0);
    Order[18] = 3;
    Order_Timer[18] = 1;
    effect_K6_init(0, 29, 29, 0);
    Order[29] = 3;
    Order_Timer[29] = 1;
    effect_K6_init(1, 30, 29, 0);
    Order[30] = 3;
    Order_Timer[30] = 1;

    if (My_char[0] != 20) {
        effect_75_init(42, 3, 0);
    }

    Order[42] = 3;
    Order_Timer[42] = 1;
    Order_Dir[42] = 5;

    if (Option == 0) {
        effect_43_init(1, 0);
    }
}

s8 Check_Bonus_Stage() {
    Setup_ID();
    Bonus_Type = Check_Bonus_Type();

    if (Bonus_Type == 0) {
        return 0;
    }

    bg_w.stage = Bonus_Type;
    bg_w.area = 0;

    if (Bonus_Type == 21) {
        My_char[COM_id] = 0xC;
    } else {
        My_char[COM_id] = My_char[Player_id];
    }

    Setup_Com_Color();
    Setup_PL_Color(COM_id, Com_Color_Shot);
    Push_LDREQ_Queue_Player(COM_id, My_char[COM_id]);
    Push_LDREQ_Queue_BG(Bonus_Type + 0);
    return Completion_Bonus[Player_id][Bonus_Type - 20] = 1;
}

s8 Check_Bonus_Type() {
    if (Debug_w[0x2E] != 0) {
        if (Debug_w[0x2E] == 1) {
            Completion_Bonus[Player_id][0] = 0;
            return 20;
        }

        if (Debug_w[0x2E] == 2) {
            Completion_Bonus[Player_id][1] = 0;
            return 21;
        }

        return 0;
    }

    if (save_w[Present_Mode].extra_option.contents[0][5] == 0) {
        return 0;
    }

    if (VS_Index[Player_id] >= 6) {
        if (Completion_Bonus[Player_id][1] & 0x80) {
            return 0;
        }

        return 21;
    }

    if (VS_Index[Player_id] >= 3) {
        if (Completion_Bonus[Player_id][0] & 0x80) {
            return 0;
        }

        return 20;
    }

    return 0;
}

void Setup_Next_Stage(s16 dir_step) {
    s16 ix;

    for (ix = 0; ix < 4; ix++) {
        effect_A9_init(dir_step, ix, ix + 23, 0);
    }
}

void Check_Auto_Cut() {
    if (!Auto_Cut_Sub()) {
        return;
    }

    if ((Cut_Scroll -= 1) < 0) {
        Cut_Scroll = 0;
    }
}

s32 Auto_Cut_Sub() {
    if (plw[0].wu.operator && ~p1sw_1 & p1sw_0 & 0xFF0) {
        return 1;
    }

    if (plw[1].wu.operator && ~p2sw_1 & p2sw_0 & 0xFF0) {
        return 1;
    }

    return 0;
}
