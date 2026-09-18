/**
 * @file next_cpu.c
 * Next CPU character selection
 */

#include "sf33rd/Source/Game/screen/next_cpu.h"
#include "sf33rd/Source/Game/screen/next_cpu_setup.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
#include "sf33rd/Source/Game/effect/eff38.h"
#include "sf33rd/Source/Game/effect/eff39.h"
#include "sf33rd/Source/Game/effect/eff42.h"
#include "sf33rd/Source/Game/effect/eff43.h"
#include "sf33rd/Source/Game/effect/eff58.h"
#include "sf33rd/Source/Game/effect/eff75.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/effect/eff98.h"
#include "sf33rd/Source/Game/effect/effa5.h"
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

u8 SEL_CPU_X;
s16 Start_X;

/* Every scene dispatcher runs the same frame: stand aside while a break-in is
 * pending, clear the result, sample the cut button, step the scene and report.
 * Next_CPU, After_Bonus and Next_Q differed only in the table they index, and
 * each passes its own by name. The return type is SEL_CPU_X's own, so each
 * caller widens it exactly as `return SEL_CPU_X;` did. */
static u8 Run_Scene_Step(void (*const step_table[])()) {
    if (Break_Into) {
        return 0;
    }

    SEL_CPU_X = 0;
    Scene_Cut = Cut_Cut_Cut();
    step_table[SC_No[0]]();
    Time_Over = false;
    return SEL_CPU_X;
}

s16 Next_CPU() {
    void (*Next_CPU_Tbl[12])() = { Next_CPU_1st,   Next_CPU_2nd,   Next_CPU_3rd,       Next_CPU_4th,
                                   Next_CPU_5th,   Next_CPU_6th,   Next_Bonus_1st,     Next_Bonus_2nd,
                                   Next_Bonus_3rd, Next_Bonus_End, Wait_Load_Complete, Wait_Load_Complete2 };

    return Run_Scene_Step(Next_CPU_Tbl);
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
    effect_A5_init();
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

/* Sub-state 0: run the character select until both sides are settled, queue the
 * fighters, and set the pause before the VS screen. */
#if DEBUG
/* The debug character override, applied on both sides of building the fighter
 * queue - four copies of the same two tests. */
static void Apply_Character_Overrides() {
    if (debug_config.character_override[0]) {
        My_char[0] = debug_config.character_override[0] - 1;
    }

    if (debug_config.character_override[1]) {
        My_char[1] = debug_config.character_override[1] - 1;
    }
}
#endif

static void Select_Next_CPU_Character() {
    if (Player_id) {
        Sel_CPU_Sub(1, ~p2sw_1 & p2sw_0, p2sw_0);
    } else {
        Sel_CPU_Sub(0, ~p1sw_1 & p1sw_0, p1sw_0);
    }

    if (!Sel_EM_Complete[Player_id]) {
        return;
    }

    SC_No[1]++;
    SC_No[2] = 0;

#if DEBUG
    Apply_Character_Overrides();
#endif

    Push_LDREQ_Queue_Player(COM_id, My_char[COM_id]);
    Setup_Next_Fighter();

#if DEBUG
    Apply_Character_Overrides();
#endif

    if (VS_Index[Player_id] < 8) {
        S_Timer = 50;
        return;
    }

    SC_No[1] = 2;
    S_Timer = 100;
}

/* Sub-state 7: a cut shortens the tail, and the BGM fades once it is short enough. */
static void Fade_Out_Before_Cut(u16 fade_time) {
    switch (SC_No[2]) {
    case 0:
        if (Scene_Cut) {
            S_Timer = 9;
        }

        if (S_Timer < 10) {
            S_Timer = 9;
            SC_No[2]++;
            SsBgmFadeOut(fade_time);
        }

        break;
    }
}

/* Tick the scene timer and, when it runs out, move to the next scene from its top. */
static void Advance_Scene_On_Timeout() {
    if ((S_Timer -= 1) == 0) {
        SC_No[0]++;
        SC_No[1] = 0;
        SC_No[2] = 0;
    }
}

/* Sub-state 2: once the pause is nearly out, go on to the VS screen - or off to the
 * boss speech scene when this opponent has one. */
static void Branch_On_EM_Speech() {
    if ((S_Timer -= 1) < 71) {
        if (Check_EM_Speech() == 0) {
            SC_No[1]++;
        } else {
            SC_No[0] = 4;
            SC_No[1] = 0;
        }

        SC_No[2] = 0;
        return;
    }
}

void Next_CPU_3rd() {
    switch (SC_No[1]) {
    case 0:
        Select_Next_CPU_Character();
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

        Advance_Scene_On_Timeout();

        break;

    case 2:
        Branch_On_EM_Speech();

        break;

    case 3:
        Fade_Out_Before_Cut(0x1000U);

        Advance_Scene_On_Timeout();

        break;
    }
}

/* The tail of the CPU intro: a cut shortens the hold, and where it goes next depends
 * on which screen asked for the intro. */
static void Leave_CPU_Intro() {
    if (Scene_Cut) {
        S_Timer = 1;
    }

    if ((S_Timer -= 1) != 0) {
        return;
    }

    if (G_No[1] == 5 || G_No[1] == 10) {
        SC_No[0] = 10;
        return;
    }

    SC_No[0] = 6;
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
        Leave_CPU_Intro();
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

/* Sub-state 2: scroll the backgrounds into the boss VS layout, place the VS objects
 * and start the two name banners moving in. */
static void Setup_Boss_VS_Screen() {
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
}

/* Sub-state 6: when the hold runs out, fade the boss intro in and start its music. */
static void Start_Boss_Intro() {
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
}

/* Sub-state 7: run the fade to completion, then mark this boss as introduced. */
static void Finish_Boss_Intro() {
    S_Timer--;

    if (FadeIn(0, 4, 8)) {
        SC_No[1]++;

        if (S_Timer < 0) {
            S_Timer = 1;
        }

        Introduce_Boss[Player_id][VS_Index[Player_id] - 8] |= 1;
    }
}

/* The tail: a cut shortens the hold, and the scene ends when it runs out. */
static void Leave_Boss_Intro() {
    if (Scene_Cut) {
        S_Timer = 1;
    }

    if ((S_Timer -= 1) == 0) {
        SC_No[0] = 10;
    }
}

/* The boss introduction itself, from the screen revival onward. Labels unchanged, so
 * a sub-state still reads as the number the rest of the file uses. */
static void Next_CPU_5th_Boss_Intro() {
    switch (SC_No[1]) {
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
        Start_Boss_Intro();

        break;

    case 7:
        Finish_Boss_Intro();

        break;

    default:
        Leave_Boss_Intro();

        break;
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
        Setup_Boss_VS_Screen();
        break;

    case 3:
        Switch_Screen(1);

        if ((Cover_Timer -= 1) == 0) {
            SC_No[1]++;
            Switch_Screen_Init(1);
        }

        break;

    default:
        Next_CPU_5th_Boss_Intro();
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

static bool is_load_complete() {
    if (!Check_PL_Load()) {
        return false;
    }

    if (!Check_LDREQ_Queue_BG(bg_w.stage)) {
        return false;
    }

    if (!(adx_now_playend() || Scene_Cut)) {
        return false;
    }

    return true;
}

void Wait_Load_Complete() {
    if (!is_load_complete()) {
        return;
    }

    SEL_CPU_X = 1;
    init_omop();
    SC_No[0] = 5;
}

void Wait_Load_Complete2() {
    if (!is_load_complete()) {
        return;
    }

    SEL_CPU_X = 2;
    init_omop();
    SC_No[0] = 10;
}

void Wait_Load_Complete3() {
    if (!is_load_complete()) {
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

    return Run_Scene_Step(After_Bonus_Tbl);
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
        Push_LDREQ_Queue_Direct(9, LDREQ_ID_SHARED);
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

/* Sub-state 0: run the select - from the demo script when no one is playing - then
 * queue the fighters and set the pause before the VS screen. */
static void Select_CPU_Character() {
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
        return;
    }

    SC_No[1]++;

#if DEBUG
    Apply_Character_Overrides();
#endif

    Push_LDREQ_Queue_Player(COM_id, My_char[COM_id]);
    Setup_Next_Fighter();

#if DEBUG
    Apply_Character_Overrides();
#endif

    if (VS_Index[Player_id] < 8) {
        S_Timer = 50;
    } else {
        SC_No[1] = 2;
        S_Timer = 100;
    }
}

/* The super-art panel is shown only in the modes that let a super art be picked, and
 * only against a real opponent. */
static s32 Super_Art_Panel_Is_Shown() {
    return check_use_all_SA() == 0 && check_without_SA() == 0 && EM_id != 0;
}

/* A boss with an introduction to play before the fight. */
static s32 Boss_Has_Speech() {
    return 8 <= VS_Index[Player_id] && Check_EM_Speech();
}

/* Sub-state 4: bring the VS presentation up - the two fighter cards, the super-art
 * panel when one is shown, the scroll target and the background sweep. */
static void Start_VS_Presentation() {
    SC_No[1] = 6;
    Order[Player_id + 11] = 4;
    Order_Timer[Player_id + 11] = 5;
    effect_38_init(COM_id, COM_id + 11, My_char[COM_id], 1, 2);
    Order[COM_id + 11] = 1;
    Order_Timer[COM_id + 11] = 1;

    if (Super_Art_Panel_Is_Shown()) {
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

    if (Boss_Has_Speech()) {
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
}

/* Sub-state 2: hold until the player load is queued, retrying a frame at a time. */
static void Await_Player_Load() {
    if ((S_Timer -= 1) < 51) {
        if (Check_LDREQ_Queue_Direct(9)) {
            SC_No[1]++;
        } else {
            S_Timer = 1;
        }
    }
}

/* Sub-state 3: a cut shortens the pause to nothing; otherwise run it out. */
static void Hold_Before_Cards() {
    if (Scene_Cut) {
        S_Timer = 1;
    }

    if ((S_Timer -= 1) == 0) {
        SC_No[1]++;
    }
}

/* The presentation half of the VS scene, from the cards onward. The case labels are
 * the original ones, so a sub-state still reads as the number the rest of the file
 * uses, and a value matching none of them does nothing, as before. */
static void Select_CPU_3rd_Presentation() {
    switch (SC_No[1]) {
    case 4:
        Start_VS_Presentation();
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
        Fade_Out_Before_Cut(0x1000);

        Advance_Scene_On_Timeout();

        break;
    }
}

void Select_CPU_3rd() {
    switch (SC_No[1]) {
    case 0:
        Select_CPU_Character();
        break;

    case 1:
        if ((S_Timer -= 1) == 0) {
            SC_No[1] = 4;
        }

        break;

    case 2:
        Await_Player_Load();
        break;

    case 3:
        Hold_Before_Cards();
        break;

    default:
        Select_CPU_3rd_Presentation();
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
        Fade_Out_Before_Cut(0x1000);

        Advance_Scene_On_Timeout();

        break;
    }
}

/* The tail of the bonus intro: a cut shortens the hold, and the scene only ends once
 * the player load has arrived - otherwise it retries a frame at a time. */
static void Leave_Bonus_Intro() {
    if (Scene_Cut) {
        S_Timer = 1;
    }

    if ((S_Timer -= 1) == 0) {
        if (!Check_PL_Load()) {
            S_Timer = 1;
            return;
        }

        SC_No[0] = 11;
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
        Leave_Bonus_Intro();

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

    return Run_Scene_Step(Next_Q_Tbl);
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

/* One lever direction: start the plate moving, unless the selection is already
 * there - in which case the caller stops, as both arms did. */
static s32 Move_Selection_Plate(s16 PL_id, u16 lever_sw, u16 direction_bit, s8 direction) {
    if (lever_sw & direction_bit) {
        if (Temporary_EM[Player_id] == direction) {
            return 1;
        }

        Sound_SE(PL_id + 96);
        Moving_Plate[PL_id] = direction;
        Moving_Plate_Counter[PL_id] = 2;
        Temporary_EM[Player_id] = direction;
    }

    return 0;
}

/* An attack button confirms the selection: lock it in, tell the engine which
 * character it is, and play the pick-up voice unless this is a boss. */
static void Confirm_EM_Selection(s16 PL_id, u16 sw) {
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

    if (Move_Selection_Plate(PL_id, lever_sw, SWK_DOWN, 2)) {
        return;
    }

    if (Move_Selection_Plate(PL_id, lever_sw, SWK_UP, 1)) {
        return;
    }

    Confirm_EM_Selection(PL_id, sw);
}
