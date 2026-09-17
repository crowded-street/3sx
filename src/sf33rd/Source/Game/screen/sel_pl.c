/**
 * @file sel_pl.c
 * Character/Super Art selection screen
 */

#include "sf33rd/Source/Game/screen/sel_pl.h"
#include "sf33rd/Source/Game/screen/sel_pl_faces.h"
#include "sf33rd/Source/Game/screen/sel_pl_exit.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
#include "sf33rd/Source/Game/demo/demo_dat.h"
#include "sf33rd/Source/Game/effect/eff38.h"
#include "sf33rd/Source/Game/effect/eff39.h"
#include "sf33rd/Source/Game/effect/eff42.h"
#include "sf33rd/Source/Game/effect/eff43.h"
#include "sf33rd/Source/Game/effect/eff50.h"
#include "sf33rd/Source/Game/effect/eff52.h"
#include "sf33rd/Source/Game/effect/eff58.h"
#include "sf33rd/Source/Game/effect/eff66.h"
#include "sf33rd/Source/Game/effect/eff69.h"
#include "sf33rd/Source/Game/effect/eff70.h"
#include "sf33rd/Source/Game/effect/eff75.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/effect/eff79.h"
#include "sf33rd/Source/Game/effect/eff93.h"
#include "sf33rd/Source/Game/effect/eff99.h"
#include "sf33rd/Source/Game/effect/effa5.h"
#include "sf33rd/Source/Game/effect/effd8.h"
#include "sf33rd/Source/Game/effect/effk6.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mmtmcnt.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/screen/next_cpu.h"
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

void Switch_Work();
void Sel_PL_Control();
void Sel_PL_Cont_1st();
void Check_Use_Gill();
void Sel_PL_Cont_2nd();
void Sel_PL_Cont_3rd();
void Sel_PL_Cont_4th();
void Setup_Face_ID();
void Setup_1st_Play_Type();
void Setup_Face_Sub();
void Setup_Select_Status();
u8 Setup_Aborigine();
void Setup_Cursor_Y();
void Player_Select_Control();
void PL_Sel_1st();
void PL_Sel_2nd();
void PL_Sel_3rd();
void PL_Sel_4th();
void PL_Sel_5th();
void Setup_Plates(s8 PL_id, s16 Time);
void Sel_PL();
void Sel_PL_1st();
void Sel_PL_2nd();
void Sel_PL_3rd();
u16 Deley_Shot_Sub(s16 PL_id);
void Sel_PL_4th();
void Sel_PL_5th();
void Sel_PL_6th();
u16 Disposal_Of_Diagonal(u16 sw);
void Sel_PL_Sub(s16 PL_id, u16 sw);
void Sel_PL_Sub_CR(s16 PL_id);
void Sel_PL_Sub_CL(s16 PL_id);
void Sel_PL_Sub_CU(s16 PL_id);
void Sel_PL_Sub_CD(s16 PL_id);
void Auto_Repeat_Sub(s16 PL_id);
u16 Auto_Repeat_Sub_Wife(s16 PL_id);
void Sel_Arts_Sub(s16 PL_id, u16 sw, u16 /* unused */);
void Correct_Control_Time(s16 PL_id);
s32 Check_Boss(s16 PL_id);
u8 Setup_Battle_Country();

u8 SEL_PL_X;
s16 Play_Type_1st;
u16 Color7[2];

const s16 Cursor_Y_Data[6] = { 80, 104, 128, 80, 104, 128 };

const u8 Arts_Y_Data[3][3] = { { 0, 1, 2 }, { 2, 0, 1 }, { 1, 2, 0 } };

const u8 Repeat_Time_Data[3] = { 26, 9, 7 };

const u8 Repeat_Time_Data_Wife[3] = { 1, 1, 1 };

/* The cursor is frozen while this player has confirmed or the grid is scrolling. */
static s32 Cursor_Is_Held() {
    return Stop_Cursor[ID] != 0 || Face_Move != 0;
}

/* Either of the two training modes. */
static s32 In_A_Training_Mode() {
    return Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING;
}

/* At least one seat has no operator, so a CPU opponent has to be chosen. */
static s32 Either_Side_Is_CPU() {
    return plw[0].wu.operator == 0 || plw[1].wu.operator == 0;
}

s16 Select_Player() {
    SEL_PL_X = 0;

    if (Break_Into) {
        return 0;
    }

    Scene_Cut = Cut_Cut_Cut();
    Sel_PL_Control();
    Switch_Work();
    ID = 0;
    Sel_PL();
    ID = 1;
    Sel_PL();
    Time_Over = false;
    return SEL_PL_X;
}

/* Entering training: reset the training work and hand the idle pad the other
 * player's switches, so both sides read the same stick. */
static void Enter_Training_Mode() {
    S_No[3]++;
    Default_Training_Data(0);
    Record_Data_Tr = 0;
    Training_Disp_Work_Clear();
    Menu_Cursor_X[0] = 0;
    Training_Cursor = 0;

    if (Champion) {
        p1sw_0 = p2sw_0;
        p1sw_1 = p2sw_0;
    } else {
        p2sw_0 = p1sw_0;
        p2sw_1 = p1sw_0;
    }
}

void Switch_Work() {
    if (Mode_Type != MODE_NORMAL_TRAINING && Mode_Type != MODE_PARRY_TRAINING) {
        return;
    }

    switch (S_No[3]) {
    case 0:
        if (Champion) {
            p1sw_0 = 0;
        } else {
            p2sw_0 = 0;
        }

        break;

    case 1:
        Enter_Training_Mode();

        break;

    case 2:
        if (Champion) {
            p1sw_0 = p2sw_0;
        } else {
            p2sw_0 = p1sw_0;
        }

        break;
    }
}

void Sel_PL_Control() {
    void (*Sel_PL_Cont_Tbl[4])() = { Sel_PL_Cont_1st, Sel_PL_Cont_2nd, Sel_PL_Cont_3rd, Sel_PL_Cont_4th };
    Setup_Select_Status();
    Sel_PL_Cont_Tbl[S_No[0]]();
    Face_Control();
    OBJ_Control();
    ID2 = 0;
    Player_Select_Control();
    ID2 = 1;
    Player_Select_Control();
    Check_Exit();
}

void Sel_PL_Cont_1st() {
    s16 xx;

    Switch_Screen(1);
    S_No[0]++;
    All_Clear_Suicide();
    SsBgmHalfVolume(0);
    Face_No[0] = 0;
    Face_No[1] = 0;
    SO_No[0] = 0;
    SO_No[1] = 0;
    Exit_No = 0;
    Fade_Flag = 0;
    judge_flag = 0;
    Game_pause = 0;

    for (xx = 0; xx < 4; xx++) {
        SP_No[0][xx] = 0;
        SP_No[1][xx] = 0;
    }

    Purge_mmtm_area(2);
    Make_texcash_of_list(2);
    bg_etc_write(2);
    Setup_Aborigine();
    Initialize_BG();
    Setup_Cursor_Y();

    if (Present_Mode == 4 || Present_Mode == 5) {
        Select_Timer = 0x20;
    } else {
        Select_Timer = 0x30;
    }

    Unit_Of_Timer = UNIT_OF_TIMER_MAX;
    Setup_Face_ID();
    Setup_1st_Play_Type();
    Setup_Face_Sub();
    Time_Stop = 1;
    effect_A5_init();
    Face_MV_Request = 0;
    Face_Status = 0;
    Face_Move = 0;
    Break_Into_CPU = 0;
    Explosion = 0;
    Time_Over = false;
    Move_Super_Arts[0] = 0;
    Move_Super_Arts[1] = 0;
    Flash_Complete[0] = 0;
    Flash_Complete[1] = 0;
    Cursor_Move[0] = 0;
    Cursor_Move[1] = 0;
    Check_Use_Gill();
    pulpul_stop();
    pp_operator_check_flag(1);
    effect_58_init(6, 20, 157);
}

void Check_Use_Gill() {
    s16 ix;

    if (Mode_Type == MODE_NETWORK) {
        return;
    }

    permission_player[PRESENT_MODE_LOCAL].ok[CHAR_GILL] = 1;
    permission_player[PRESENT_MODE_NORMAL_TRAINING].ok[CHAR_GILL] = 1;
    permission_player[PRESENT_MODE_PARRY_TRAINING].ok[CHAR_GILL] = 1;
}

void Sel_PL_Cont_2nd() {
    Switch_Screen(1);
    Switch_Screen_Init(1);
    S_No[0]++;
    Request_E_No = 1;
    Clear_Flash_No();
}

void Sel_PL_Cont_3rd() {
    if (!Switch_Screen_Revival(0)) {
        return;
    }

    S_No[0]++;
    Forbid_Break = 0;

    if (G_No[1] != 1) {
        // This is a comparison to zero in the decomp. Might be a programmer error
        Demo_Flag = 0;
    }
}

void Sel_PL_Cont_4th() {
    // Do nothing
}

void Setup_Face_ID() {
    s16 x;
    s16 y;

    for (y = 0; y < 3; y++) {
        for (x = 0; x < 8; x++) {
            ID_of_Face[y][x] = Face_Cursor_Data[y][x];
        }
    }
}

void Setup_1st_Play_Type() {
    if (Play_Type == 1) {
        Play_Type_1st = 99;
    } else {
        Play_Type_1st = Aborigine;
    }
}

void Setup_Face_Sub() {
    s16 x;

    Complete_Face = 19;

    for (x = 1; x < 20; x++) {
        effect_70_init(x);
    }
}

void Setup_Select_Status() {
    if (plw[0].wu.operator) {
        Select_Status[0] = 1;
    } else {
        Select_Status[0] = 0;
    }

    if (plw[1].wu.operator) {
        Select_Status[0] |= 2;
    }

    if (Sel_Arts_Complete[0] != -1 && plw[0].wu.operator != 0) {
        Select_Status[1] = 1;
    } else {
        Select_Status[1] = 0;
    }

    if (Sel_Arts_Complete[1] != -1 && plw[1].wu.operator != 0) {
        Select_Status[1] |= 2;
    }
}

u8 Setup_Aborigine() {
    if (Select_Status[0] == 3) {
        return Aborigine = 153;
    }

    if (Select_Status[0] == 1) {
        return Aborigine = 0;
    }

    return Aborigine = 1;
}

void Setup_Cursor_Y() {
    s16 i;
    s16 j;

    s16 a;
    s16 b;
    s16 c;
    s16 d;

    for (i = 2, a = j = 0; i >= 0; i--, b = j++) {
        Cursor_Y_Pos[0][i] = Cursor_Y_Data[j];
    }

    for (i = 2, c = j = 3; i >= 0; i--, d = j++) {
        Cursor_Y_Pos[1][i] = Cursor_Y_Data[j];
    }
}

void Player_Select_Control() {
    void (*PL_Sel_Jmp_Tbl[5])() = { PL_Sel_1st, PL_Sel_2nd, PL_Sel_3rd, PL_Sel_4th, PL_Sel_5th };

    if (plw[ID2].wu.operator != 0) {
        PL_Sel_Jmp_Tbl[SP_No[ID2][1]]();
    }
}

void PL_Sel_1st() {
    s16 ret;
    s16 ret2;

    if (Sel_PL_Complete[ID2] == -0x8000) {
        SP_No[ID2][1] = 2;
        Push_LDREQ_Queue_Player(ID2, My_char[ID2]);
        ret = check_use_all_SA();
        ret2 = check_without_SA();
        ret |= ret2;

        if (ret != 0) {
            return;
        }

        if (My_char[ID2] == 0) {
            return;
        }

        Sel_Arts_Complete[ID2] = 0;
        Setup_Plates(ID2, 0x55);
        effect_50_init(ID2, 1, 0);
        effect_50_init(ID2, 1, 1);
        effect_50_init(ID2, 2, 0);
        effect_50_init(ID2, 2, 1);

#if DEBUG
        if (debug_config.character_override[0]) {
            My_char[0] = debug_config.character_override[0] - 1;
        }

        if (!debug_config.character_override[1]) {
            return;
        }

        My_char[1] = debug_config.character_override[1] - 1;
#endif
    } else {
        SP_No[ID2][1] += 1;
    }
}

/* Once a character is chosen, either open the super-art plates or - in the modes with
 * no art to pick - clear the lines and move straight on. */
static void Begin_Arts_Selection() {
    s16 ret;
    s16 ret2;

    if (!Sel_PL_Complete[ID2]) {
        return;
    }

    ret = check_use_all_SA();
    ret2 = check_without_SA();
    ret |= ret2;

    if (ret != 0 || My_char[ID2] == 0) {
        SP_No[ID2][3]++;
        Cursor_Timer[ID2] = 40;
        Go_Away_Red_Lines();

        if (In_A_Training_Mode()) {
            S_No[3] = 1;
            return;
        }

        return;
    }

    SP_No[ID2][1]++;
    Setup_Plates(ID2, 1);
    effect_50_init(ID2, 1, 0);
    effect_50_init(ID2, 1, 1);
    effect_50_init(ID2, 2, 0);
    effect_50_init(ID2, 2, 1);
}

void PL_Sel_2nd() {
    switch (SP_No[ID2][3]) {
    case 0:
        Begin_Arts_Selection();
        break;

    case 1:
        if ((Cursor_Timer[ID2] -= 1) != 0) {
            break;
        }

        Sel_Arts_Complete[ID2] = -1;
        SP_No[ID2][1]++;
        SP_No[ID2][3] = 0;
        Setup_ID();

        if (Used_char[ID2] != My_char[ID2]) {
            Last_Player_id = ID2;
        }

        Used_char[ID2] = My_char[ID2];
        break;
    }
}

void PL_Sel_3rd() {
    if (Sel_Arts_Complete[ID2] < 0) {
        SP_No[ID2][1]++;
    }
}

void PL_Sel_4th() {
    // Do nothing
}

void PL_Sel_5th() {
    // Do nothing
}

void Setup_Plates(s8 PL_id, s16 Time) {
    Move_Super_Arts[PL_id] = 3;
    Select_Arts[PL_id] = 3;
    effect_79_init(&(Effect79InitArgs){ PL_id, 0, Arts_Y_Data[Super_Arts[PL_id]][0], Time, 2 });
    effect_79_init(&(Effect79InitArgs){ PL_id, 1, Arts_Y_Data[Super_Arts[PL_id]][1], Time, 2 });
    effect_79_init(&(Effect79InitArgs){ PL_id, 2, Arts_Y_Data[Super_Arts[PL_id]][2], Time, 2 });
}

void Sel_PL() {
    void (*Sel_PL_Jmp_Tbl[6])() = { Sel_PL_1st, Sel_PL_2nd, Sel_PL_3rd, Sel_PL_4th, Sel_PL_5th, Sel_PL_6th };

    if (plw[ID].wu.operator != 0) {
        Sel_PL_Jmp_Tbl[SP_No[ID][0]]();
    }
}

void Sel_PL_1st() {
    u16 Rnd;

    if (Exit_No) {
        return;
    }

    SP_No[ID][0]++;
    Stop_Cursor[ID] = 1;
    Auto_No[ID] = 0;
    Auto_Index[ID] = 0;
    Auto_Cursor[ID] = 0;
    Moving_Plate[ID] = 0;
    Moving_Plate_Counter[ID] = 0;
    Select_Start[ID] = 2;
    Select_Arts[ID] = -1;

    if (ID == 1) {
        effect_D8_init(1, 1);
        effect_D8_init(1, 3);
        Rnd = random_16() & 3;
        Free_Ptr[1] = Voice_Random_Data[1][Rnd];
    } else {
        effect_D8_init(0, 0);
        effect_D8_init(0, 2);
        Rnd = random_16() & 3;
        Free_Ptr[0] = Voice_Random_Data[1][Rnd];
    }

    if (Sel_PL_Complete[ID]) {
        SP_No[ID][0] = 3;
        Select_Start[ID] = 3;
        Select_Arts[ID] = 3;
        Stop_Cursor[ID] = 1;
        paring_ctr_vs[0][ID] = 0;
        paring_ctr_vs[1][ID] = 0;
        return;
    }

    Arts_Y[ID] = Super_Arts[ID] = Last_Super_Arts[ID];
}

void Sel_PL_2nd() {
    if (Select_Start[ID] > 0) {
        return;
    }

    SP_No[ID][0]++;
    Stop_Cursor[ID] = 0;
    Deley_Shot_No[ID] = 0;
    Cursor_Timer[ID] = 1;

    if (Demo_Flag == 0) {
        Demo_Timer[ID] = 0;
        Demo_Ptr[ID] = (u16*)Sel_PL_Data_Address[Select_Demo_Index];
    }
}

/* Once a character is chosen: queue it, stop the cursor, and start a fresh grade
 * record unless this player is continuing. */
static void Commit_Player_Choice() {
#if DEBUG
    if (debug_config.character_override[0]) {
        My_char[0] = debug_config.character_override[0] - 1;
    }

    if (debug_config.character_override[1]) {
        My_char[1] = debug_config.character_override[1] - 1;
    }
#endif

    Push_LDREQ_Queue_Player(ID, My_char[ID]);
    SP_No[ID][0]++;
    Stop_Cursor[ID] = 1;
    Auto_No[ID] = 0;
    paring_ctr_vs[0][ID] = 0;
    paring_ctr_vs[1][ID] = 0;

    if (Continue_Coin[ID] == 0) {
        Clear_Break_Com(ID);
        grade_check_work_1st_init(ID, 0);
        grade_check_work_1st_init(ID, 1);
        Initialize_EM_Candidate(ID);
        Best_Grade[ID] = -1;
        Result_Timer[ID] = 180;
        Request_Disp_Rank[ID][0] = -1;
        Request_Disp_Rank[ID][1] = -1;
        Request_Disp_Rank[ID][2] = -1;
        Request_Disp_Rank[ID][3] = -1;
        return;
    }

    Check_Same_CPU(ID);
}

void Sel_PL_3rd() {
    if (Cursor_Is_Held()) {
        return;
    }

    if (Demo_Flag == 0) {
        if (ID) {
            Sel_PL_Sub(1, Check_Demo_Data(1));
        } else {
            Sel_PL_Sub(0, Check_Demo_Data(0));
        }
    } else if (ID) {
        Sel_PL_Sub(1, Deley_Shot_Sub(1));
    } else {
        Sel_PL_Sub(0, Deley_Shot_Sub(0));
    }

    if (Sel_PL_Complete[ID] >= 0) {
        return;
    }

    Commit_Player_Choice();
}

/* State 0: nothing latched yet. The three-button art shortcut and the buttons that
 * cannot be combined go straight through; anything else starts the latch window.
 * The arm's `break` fell to the function's `return lever`, which is what it
 * returns here. */
static u16 Begin_Delayed_Shot(s16 PL_id, u16 sw, u16 lever) {
    if (!(sw & SWK_ATTACKS)) {
        return lever;
    }

    if (sw == (SWK_WEST | SWK_RIGHT_SHOULDER | SWK_EAST)) {
        return lever | (SWK_WEST | SWK_RIGHT_SHOULDER | SWK_EAST);
    }

    if (sw & (SWK_NORTH | SWK_SOUTH | SWK_RIGHT_TRIGGER | SWK_START)) {
        return sw | lever;
    }

    Color7[PL_id] = sw;
    Deley_Shot_No[PL_id] = 1;
    Deley_Shot_Timer[PL_id] = 3;

    return lever;
}

u16 Deley_Shot_Sub(s16 PL_id) {
    u16 sw;
    u16 lever;

    if (PL_id == 0) {
        sw = ~p1sw_1 & p1sw_0;
    } else {
        sw = ~p2sw_1 & p2sw_0;
    }

    lever = Disposal_Of_Diagonal(sw);
    sw &= SWK_ATTACKS;

    switch (Deley_Shot_No[PL_id]) {
    case 0:
        lever = Begin_Delayed_Shot(PL_id, sw, lever);
        break;

    case 1:
        Color7[PL_id] |= sw;

        if ((Deley_Shot_Timer[PL_id] -= 1) == 0) {
            return lever | Color7[PL_id];
        }

        if (Color7[PL_id] == (SWK_WEST | SWK_RIGHT_SHOULDER | SWK_EAST)) {
            return lever | (SWK_WEST | SWK_RIGHT_SHOULDER | SWK_EAST);
        }

        break;
    }

    return lever;
}

void Sel_PL_4th() {
    if (!Select_Arts[ID]) {
        SP_No[ID][0]++;
        Stop_Cursor[ID] = 0;
    }
}

void Sel_PL_5th() {
    if (Cursor_Is_Held()) {
        return;
    }

    if (Demo_Flag == 0) {
        if (ID) {
            Sel_Arts_Sub(1, Check_Demo_Data(1), 0);
        } else {
            Sel_Arts_Sub(0, Check_Demo_Data(0), 0);
        }
    } else if (ID) {
        Sel_Arts_Sub(1, ~p2sw_1 & p2sw_0, p2sw_0);
    } else {
        Sel_Arts_Sub(0, ~p1sw_1 & p1sw_0, p1sw_0);
    }

    if (!Sel_Arts_Complete[ID]) {
        return;
    }

    SP_No[ID][0]++;

    if (In_A_Training_Mode()) {
        S_No[3] = 1;
    }

    if (Either_Side_Is_CPU()) {
        Check_Boss(ID);
    }
}

void Sel_PL_6th() {
    // Do nothing
}

u16 Disposal_Of_Diagonal(u16 sw) {
    sw &= SWK_DIRECTIONS;

    if (sw == SWK_UP) {
        return SWK_UP;
    }

    if (sw == SWK_DOWN) {
        return SWK_DOWN;
    }

    if (sw == (SWK_UP | SWK_RIGHT)) {
        return SWK_UP;
    }

    if (sw == (SWK_DOWN | SWK_LEFT)) {
        return SWK_DOWN;
    }

    return sw &= (SWK_LEFT | SWK_RIGHT);
}

/* Once the cursor timer runs out, step the cursor in whichever direction is held and
 * set the hold before it may step again. */
static void Step_Face_Cursor(s16 PL_id, u16 sw) {
    if ((Cursor_Timer[PL_id] -= 1) == 0) {
        Cursor_Timer[PL_id] = 1;

        if (sw & SWK_RIGHT) {
            Cursor_Timer[PL_id] = 5;
            Sel_PL_Sub_CR(PL_id);
        } else if (sw & SWK_LEFT) {
            Cursor_Timer[PL_id] = 5;
            Sel_PL_Sub_CL(PL_id);
        } else if (sw & SWK_UP) {
            Cursor_Timer[PL_id] = 5;
            Sel_PL_Sub_CU(PL_id);
        } else if (sw & SWK_DOWN) {
            Cursor_Timer[PL_id] = 5;
            Sel_PL_Sub_CD(PL_id);
        }
    }
}

void Sel_PL_Sub(s16 PL_id, u16 sw) {
    Cursor_Move[PL_id] = 0;

    if (Sel_PL_Complete[PL_id]) {
        return;
    }

    if (Time_Over) {
        sw = SWK_WEST;
    }

    if (sw == 0) {
        Auto_Repeat_Sub(PL_id);
    }

    Step_Face_Cursor(PL_id, sw);

    if (Cursor_Move[PL_id]) {
        Sound_SE(ID + 96);
    }

    if (!(sw & SWK_ATTACKS)) {
        return;
    }

    Sel_PL_Complete[PL_id] = 1;
    My_char[PL_id] = ID_of_Face[Cursor_Y[PL_id]][Cursor_X[PL_id]];

    if (Last_My_char2[PL_id] != My_char[PL_id]) {
        Arts_Y[ID] = Super_Arts[ID] = Last_Super_Arts[ID] = 0;
        Introduce_Boss[ID][0] = 0;
    }

    Last_My_char2[PL_id] = My_char[PL_id];
    Last_Selected_ID = PL_id;
    Order[1] = 2;
    Order_Timer[1] = 1;
    Order_Dir[1] = 8;

    if (Select_Status[0] != 3) {
        Order[2] = 1;
        Order_Timer[2] = 10;
        Order_Dir[2] = 4;
    }

    Sound_SE(ID + 98);
    Sound_SE(*Free_Ptr[PL_id]++);
    Setup_PL_Color(PL_id, sw);
    Correct_Control_Time(PL_id);
}

void Sel_PL_Sub_CR(s16 PL_id) {
    if (Cursor_X[PL_id] == 7) {
        return;
    }

    Cursor_Move[PL_id] = 1;

    do {
        Cursor_Y[PL_id]++;

        switch (Cursor_X[PL_id]) {
        case 6:
            if (Cursor_Y[PL_id] > 1) {
                Cursor_Y[PL_id] = 1;
                Cursor_X[PL_id] = 0;
            }

            break;

        default:
            if (Cursor_Y[PL_id] > 2) {
                Cursor_Y[PL_id] = 0;
                Cursor_X[PL_id]++;
            }

            break;
        }
    } while (!permission_player[Present_Mode].ok[Face_Cursor_Data[Cursor_Y[PL_id]][Cursor_X[PL_id]]]);
}

/* Having stepped up a row, wrap to the bottom of the previous column when the step
 * ran off the top. Each column has its own bottom row. */
static void Wrap_Cursor_Up_Column(s16 PL_id) {
    switch (Cursor_X[PL_id]) {
    case 0:
        if (Cursor_Y[PL_id] <= 0) {
            Cursor_Y[PL_id] = 1;
            Cursor_X[PL_id] = 6;
        }
        break;

    case 1:
        if (Cursor_Y[PL_id] < 0) {
            Cursor_Y[PL_id] = 2;
            Cursor_X[PL_id] = 0;
        }
        break;

    default:
        if (Cursor_Y[PL_id] < 0) {
            Cursor_Y[PL_id] = 2;
            Cursor_X[PL_id]--;
        }

        break;
    }
}

void Sel_PL_Sub_CL(s16 PL_id) {
    if (Cursor_X[PL_id] == 7) {
        return;
    }

    Cursor_Move[PL_id] = 1;

    do {
        Cursor_Y[PL_id]--;

        Wrap_Cursor_Up_Column(PL_id);
    } while (!permission_player[Present_Mode].ok[Face_Cursor_Data[Cursor_Y[PL_id]][Cursor_X[PL_id]]]);
}

void Sel_PL_Sub_CU(s16 PL_id) {
    Cursor_Move[PL_id] = 1;

    do {
        Cursor_X[PL_id]++;

        switch (Cursor_Y[PL_id]) {
        case 0:
            if (Cursor_X[PL_id] > 6) {
                Cursor_X[PL_id] = 1;
            }

            break;

        case 1:
            if (Cursor_X[PL_id] > 7) {
                Cursor_X[PL_id] = 0;
            }

            break;

        default:
            if (Cursor_X[PL_id] > 5) {
                Cursor_X[PL_id] = 0;
            }

            break;
        }
    } while (!permission_player[Present_Mode].ok[Face_Cursor_Data[Cursor_Y[PL_id]][Cursor_X[PL_id]]]);
}

void Sel_PL_Sub_CD(s16 PL_id) {
    Cursor_Move[PL_id] = 1;

    do {
        Cursor_X[PL_id]--;

        switch (Cursor_Y[PL_id]) {
        case 0:
            if (Cursor_X[PL_id] <= 0) {
                Cursor_X[PL_id] = 6;
            }

            break;

        case 1:
            if (Cursor_X[PL_id] < 0) {
                Cursor_X[PL_id] = 7;
            }

            break;

        default:
            if (Cursor_X[PL_id] < 0) {
                Cursor_X[PL_id] = 5;
            }

            break;
        }
    } while (!permission_player[Present_Mode].ok[Face_Cursor_Data[Cursor_Y[PL_id]][Cursor_X[PL_id]]]);
}

/* Arm the auto-repeat for one lever direction, and report whether it fired - the
 * four directions differed only in the lever bit and the cursor code. */
static s32 Begin_Auto_Repeat(s16 PL_id, u16 sw, u16 direction_bit, s8 cursor) {
    if (sw & direction_bit) {
        Auto_No[PL_id] = 1;
        Auto_Cursor[PL_id] = cursor;
        Auto_Timer[PL_id] = Repeat_Time_Data[0];
        Auto_Index[PL_id] = 1;
        return 1;
    }

    return 0;
}

/* State 0: no repeat running - arm one if a lever direction is held. */
static void Arm_Auto_Repeat(s16 PL_id, u16 sw) {
    if (Begin_Auto_Repeat(PL_id, sw, SWK_RIGHT, 8)) {
        return;
    }

    if (Begin_Auto_Repeat(PL_id, sw, SWK_LEFT, 4)) {
        return;
    }

    if (Begin_Auto_Repeat(PL_id, sw, SWK_UP, 1)) {
        return;
    }

    Begin_Auto_Repeat(PL_id, sw, SWK_DOWN, 2);
}

/* State 1: a repeat is running - drop it if the lever moved, otherwise tick it and
 * step the cursor each time it fires. */
static void Tick_Auto_Repeat(s16 PL_id, u16 sw) {
    if (sw != Auto_Cursor[PL_id]) {
        Auto_No[PL_id] = 0;
        return;
    }

    if (Auto_Timer[PL_id] -= 1) {
        return;
    }

    Auto_Timer[PL_id] = Repeat_Time_Data[Auto_Index[PL_id]];
    Auto_Index[PL_id]++;

    if ((Auto_Index[PL_id]) > 2) {
        Auto_Index[PL_id] = 2;
    }

    if (sw & SWK_RIGHT) {
        Sel_PL_Sub_CR(PL_id);
    }

    if (sw & SWK_LEFT) {
        Sel_PL_Sub_CL(PL_id);
    }

    if (sw & SWK_UP) {
        Sel_PL_Sub_CU(PL_id);
    }

    if (sw & SWK_DOWN) {
        Sel_PL_Sub_CD(PL_id);
    }
}

void Auto_Repeat_Sub(s16 PL_id) {
    u16 sw;

    if (Demo_Flag == 0) {
        return;
    }

    if (Cursor_Move[PL_id]) {
        return;
    }

    if (PL_id == 0) {
        sw = p1sw_0;
    } else {
        sw = p2sw_0;
    }

    sw = Disposal_Of_Diagonal(sw);

    switch (Auto_No[PL_id]) {
    case 0:
        Arm_Auto_Repeat(PL_id, sw);
        break;

    case 1:
        Tick_Auto_Repeat(PL_id, sw);
        break;
    }
}

/* State 1 of the super-art cursor repeat: drop it if the lever left the direction it
 * started on, otherwise tick it and report the direction each time it fires. Every
 * path that fell out of the switch reached the function's `return 0`. */
static u16 Tick_Wife_Repeat(s16 PL_id, u16 sw) {
    sw &= Auto_Cursor[PL_id];

    if (sw) {
        if (Auto_Timer[PL_id] -= 1) {
            return 0;
        }

        Auto_Timer[PL_id] = Repeat_Time_Data_Wife[Auto_Index[PL_id]++];

        if (Auto_Index[PL_id] > 2) {
            Auto_Index[PL_id] = 2;
        }

        if (sw & SWK_UP) {
            return SWK_UP;
        }

        if (sw & SWK_DOWN) {
            return SWK_DOWN;
        }

        return 0;
    }

    Auto_No[PL_id] = 0;
    return 0;
}

u16 Auto_Repeat_Sub_Wife(s16 PL_id) {
    u16 sw;

    if (Cursor_Move[PL_id] || Demo_Flag == 0) {
        return 0;
    }

    if (PL_id == 0) {
        sw = p1sw_0;
    } else {
        sw = p2sw_0;
    }

    switch (Auto_No[PL_id]) {
    case 0:
        if (sw & SWK_UP) {
            Auto_No[PL_id] = 1;
            Auto_Cursor[PL_id] = 1;
            Auto_Timer[PL_id] = Repeat_Time_Data_Wife[0];
            Auto_Index[PL_id] = 1;
        } else if (sw & SWK_DOWN) {
            Auto_No[PL_id] = 1;
            Auto_Cursor[PL_id] = 2;
            Auto_Timer[PL_id] = Repeat_Time_Data_Wife[0];
            Auto_Index[PL_id] = 1;
        }

        break;

    case 1:
        return Tick_Wife_Repeat(PL_id, sw);
    }

    return 0;
}

/* Any of the three super-art plates still animating. */
static s32 Any_Plate_Is_Disposing(s16 PL_id) {
    return Plate_Disposal_No[PL_id][0] != 0 || Plate_Disposal_No[PL_id][1] != 0 ||
           Plate_Disposal_No[PL_id][2] != 0;
}

/* An attack button confirms the super art: lock it in, remember it, and note the
 * character this player took it with. */
static void Confirm_Arts_Selection(s16 PL_id, u16 sw) {
    if (sw & SWK_ATTACKS) {
        Stop_Cursor[ID] = 1;
        Slide_Type = PL_id;
        Sel_Arts_Complete[PL_id] = 1;
        Last_Super_Arts[PL_id] = Super_Arts[PL_id] = Arts_Y[PL_id];
        Sound_SE(ID + 98);
        Sound_SE(*Free_Ptr[PL_id]++);
        Setup_ID();

        if (Used_char[PL_id] != My_char[PL_id]) {
            Last_Player_id = PL_id;
        }

        Used_char[PL_id] = My_char[PL_id];
    }
}

/* The super-art cursor is locked while this player has already chosen, while a plate
 * is moving, and while any plate is still animating out. */
static s32 Arts_Cursor_Is_Locked(s16 PL_id) {
    if (Sel_Arts_Complete[PL_id]) {
        return 1;
    }

    if (Moving_Plate_Counter[PL_id]) {
        return 1;
    }

    if (Moving_Plate[PL_id]) {
        return 1;
    }

    if (Any_Plate_Is_Disposing(PL_id)) {
        return 1;
    }

    return 0;
}

/* Step the super-art cursor down one plate, wrapping to the top. */
static void Move_Arts_Cursor_Down(s16 PL_id, u16 sw) {
    if (sw & SWK_DOWN) {
        Sound_SE(ID + 96);
        Moving_Plate[PL_id] = 2;
        Moving_Plate_Counter[PL_id] = 3;
        OK_Priority[PL_id] = 0;

        if ((Arts_Y[PL_id] += 1) > 2) {
            Arts_Y[PL_id] = 0;
        }
    }
}

void Sel_Arts_Sub(s16 PL_id, u16 sw, u16 /* unused */) {
    u16 lever_sw;

    if (Arts_Cursor_Is_Locked(PL_id)) {
        return;
    }

    if (Time_Over) {
        sw = SWK_WEST;
    }

    lever_sw = sw & SWK_DIRECTIONS;

    if (lever_sw == 0) {
        sw |= Auto_Repeat_Sub_Wife(PL_id);
    }

    Move_Arts_Cursor_Down(PL_id, sw);

    if (sw & SWK_UP) {
        Sound_SE(ID + 96);
        Moving_Plate[PL_id] = 1;
        Moving_Plate_Counter[PL_id] = 3;
        OK_Priority[PL_id] = 0;

        if ((Arts_Y[PL_id] -= 1) < 0) {
            Arts_Y[PL_id] = 2;
        }
    }

    Confirm_Arts_Selection(PL_id, sw);
}

void Correct_Control_Time(s16 PL_id) {
    u8 xx;
    u8 zz;

    if (Play_Type == 1) {
        return;
    }

    if (Stage_Continue[PL_id] == 0) {
        return;
    }

    xx = Stage_Continue[PL_id];

    if (VS_Index[PL_id] >= 9) {
        zz = 1;
    } else {
        zz = 0;
    }

    if (Stage_Continue[PL_id] >= 16) {
        xx = 16;
    } else {
        xx = Stage_Continue[PL_id];
    }

    Control_Time = SC_Personal_Time[PL_id] - Correct_Cont_Time_Data[zz][xx];

    if (Control_Time < 0) {
        Control_Time = 0;
    }

    SC_Personal_Time[PL_id] = Control_Time;
}

s32 Check_Boss(s16 PL_id) {
    if (VS_Index[Player_id] >= 9 && Introduce_Boss[Player_id][1] == 0) {
        Control_Time = Limit_Time;
        SC_Personal_Time[PL_id] = Control_Time;
        return Break_Into_CPU = 1;
    }

    return Break_Into_CPU = 0;
}

u8 Setup_Battle_Country() {
    s16 Rnd32;

    if (Mode_Type == MODE_VERSUS) {
        if (VS_Stage == 20) {
            Rnd32 = random_32();
            return Random_Stage_Data[1][Rnd32];
        }

        return VS_Stage;
    }

    if (My_char[0] == 17 && My_char[1] == 17) {
        Rnd32 = random_32();
        return Random_Stage_Data[0][Rnd32];
    }

    if (My_char[New_Challenger] == 17) {
        return My_char[Champion];
    }

    return My_char[New_Challenger];
}
