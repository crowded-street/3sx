/**
 * @file entry.c
 * Manages all the screens
 */

#include "sf33rd/Source/Game/screen/entry.h"
#include "sf33rd/Source/Game/screen/entry_break_in.h"
#include "common.h"
#include "constants.h"
#include "main.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/effect/effa2.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/screen/n_input.h"
#include "sf33rd/Source/Game/screen/ranking.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/system/reset.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "structs.h"

const u8 Coin_Message_Data[7][2] = { { 5, 30 }, { 2, 27 }, { 7, 32 }, { 17, 37 }, { 6, 31 }, { 5, 42 }, { 0, 0 } };

void Entry_00();
void Entry_01();
void Entry_02();
void Entry_03();
void Entry_04();
void Entry_06();
void Entry_07();
void Entry_08();
void Entry_10();

void Disp_00_0();
void Entry_01_Sub(s16 PL_id);
void Exit_Title_Sub_Entry();
void Entry_Main_Sub(s16 PL_id, s16 Jump_Index);
void Entry_03_1st();
void Entry_03_2nd();
void Entry_04_1st();
void Entry_04_2nd();
void Correct_BI_Data();
void Entry_06_1st();
void Entry_06_2nd();
void Entry_07_1st();
void Entry_07_2nd();
void Entry_08_1st();
void Entry_08_2nd();
void Entry_10_1st();
void Entry_10_2nd();
void Entry_Common_Sub(s16 PL_id, s16 Jump_Index);
void Entry_Continue_Sub(s16 PL_id);
void In_Game_Sub(s16 PL_id);
void In_Over_Sub(s16 PL_id);
void Loser_Scene_Sub(s16 PL_id, s16 Jump_Index);
void Name_In_Sub(s16 PL_id);
void Name_In_Sub0(s16 PL_id, s16 xx);
s32 Credit_Continue_1P();
s32 Credit_Continue_2P();
void Naming_Cut_Sub_1P();
void Naming_Cut_Sub_2P();
void Naming_Init(s16 PL_id);
s32 Credit_Sub_1P();
s32 Credit_Sub_2P();
s32 Loser_Sub_1P();
s32 Loser_Sub_2P();
s32 Flash_Start(s16 PL_id);
s32 Flash_Please(s16 PL_id);
void Setup_Next_Step(s16 PL_id);

void Entry_Task(struct _TASK* /* unused */) {
    s16 ix;

    void (*Main_Jmp_Tbl[11])() = { Entry_00, Entry_01, Entry_02, Entry_03, Entry_04, Entry_03,
                                   Entry_06, Entry_07, Entry_08, Entry_03, Entry_10 };

    if (Pause || nowSoftReset()) {
        return;
    }

    Main_Jmp_Tbl[E_No[0]]();
}

void Entry_00() {
    switch (E_No[1]) {
    case 0:
        break;

    case 1:
        E_No[1] += 1;
        E_Timer = 50;
        Disp_00_0();
        break;

    case 2:
        if (--E_Timer == 0) {
            E_No[1] += 1;
            E_Timer = 30;
            break;
        }

        Disp_00_0();
        break;

    case 3:
        if (!--E_Timer) {
            E_No[1] -= 1;
            E_Timer = 50;
            Disp_00_0();
            break;
        }

        break;
    }
}

void Disp_00_0() {
    if (save_w[1].extra_option.contents[3][5] == 0) {
        return;
    }

    SSPutStr(&(ScStr){ 16, Insert_Y, 9, "PRESS ANY BUTTON" }, 2);

    if (!(G_No[1] == 3 || G_No[1] == 5)) {
        return;
    }

    SSPutStr(&(ScStr){ 5, 0, 9, "PRESS 1P START" }, 2);
    SSPutStr(&(ScStr){ 30, 0, 9, "PRESS 2P START" }, 2);
}

void Entry_01() {
    switch (E_No[2]) {
    case 0:
        E_No[2] += 1;
        E_No[1] = 1;
        Break_Into = 0;
        break;

    case 1:
        Entry_00();

        if (~p1sw_1 & p1sw_0 & (SWK_START | SWK_ATTACKS)) {
            Entry_01_Sub(0);
        } else if (~p2sw_1 & p2sw_0 & (SWK_START | SWK_ATTACKS)) {
            Entry_01_Sub(1);
        }

        break;

    case 2:
        if (Request_E_No) {
            E_No[2] += 1;
        }

        break;

    default:
        Exit_Title_Sub_Entry();
        break;
    }
}

void Entry_01_Sub(s16 PL_id) {
    E_No[2] += 1;
    Request_G_No = 1;
    plw[PL_id].wu.operator = 1;
    Operator_Status[PL_id] = 1;
    Champion = PL_id;
    plw[PL_id ^ 1].wu.operator = 0;
    Operator_Status[PL_id ^ 1] = 0;
    Ignore_Entry[0] = 0;
    Ignore_Entry[1] = 0;

    if (Continue_Coin[PL_id] == 0) {
        grade_check_work_1st_init(PL_id, 0);
    }
}

void Exit_Title_Sub_Entry() {
    s16 i;
    s16 j;

    E_No[0] = 2;
    E_No[1] = 0;
    E_No[2] = 0;
    E_No[3] = 0;
    F_No1[0] = F_No2[0] = F_No3[0] = 0;
    F_No1[1] = F_No2[1] = F_No3[1] = 0;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            E_Number[i][j] = 0;
        }
    }
}

void Entry_02() {
    switch (E_No[1]) {
    case 0:
        E_No[1] += 1;
        break;
    }

    Entry_Main_Sub(0, 2);
    Entry_Main_Sub(1, 2);
}

/* The five screen dispatchers - 03, 04, 06, 07 and 08 - were identical apart from
 * which pair of handlers they named. Both names travel verbatim to the call site
 * and this helper does nothing with them but call them. */
static void Entry_Screen_Step(void (*first_frame)(), void (*later_frames)()) {
    switch (E_No[1]) {
    case 0:
        first_frame();
        break;

    default:
        later_frames();
        break;
    }
}

void Entry_03() {
    Entry_Screen_Step(Entry_03_1st, Entry_03_2nd);
}

/* The first frame of a screen: advance the sub-state once, then step both players.
 * Entry_03_1st, Entry_06_1st and Entry_07_1st differed only in the jump index. */
static void Begin_Entry_Step(s16 Jump_Index) {
    switch (E_No[2]) {
    case 0:
        E_No[2] += 1;
        break;
    }

    Entry_Main_Sub(0, Jump_Index);
    Entry_Main_Sub(1, Jump_Index);
}

void Entry_03_1st() {
    Begin_Entry_Step(4);
}

/* Put the screen back to the in-game state: raise the cover and rewind both the game
 * and entry state machines. Shared by the character select, the VS screen and the
 * naming and final-grade tail. */
static void Reset_To_Game_Screen() {
    Cover_Timer = 23;
    G_No[1] = 1;
    G_No[2] = 0;
    G_No[3] = 0;
    E_No[0] = 2;
    E_No[1] = 0;
    E_No[2] = 0;
    E_No[3] = 0;
}

/* Both screens wait out the same timer before covering up, and retry a frame at a
 * time while a break-in load is still pending. */
static void Await_Break_In_Load() {
    if (--E_Timer == 0) {
        if (!Check_LDREQ_Break()) {
            E_No[2] += 1;
            Switch_Screen_Init(1);
            return;
        }

        E_Timer = 1;
        return;
    }
}

void Entry_03_2nd() {
    switch (E_No[2]) {
    case 0:
        Await_Break_In_Load();

        break;

    case 1:
        if (Switch_Screen(1) != 0) {
            Reset_To_Game_Screen();
            plw[New_Challenger].wu.operator = 1;
            Operator_Status[New_Challenger] = 1;
            Sel_Arts_Complete[Champion] = -1;

            if (Continue_Coin[New_Challenger] == 0) {
                grade_check_work_1st_init(New_Challenger, 0);
            }
        }

        break;
    }
}

void Entry_04() {
    Entry_Screen_Step(Entry_04_1st, Entry_04_2nd);
}

void Entry_04_1st() {
    switch (E_No[2]) {
    case 0:
        E_No[2] += 1;
        break;
    }

    if (Game_pause != 0x81) {
        Entry_Main_Sub(0, 5);
        Entry_Main_Sub(1, 5);
    }
}

void Entry_04_2nd() {
    switch (E_No[2]) {
    case 0:
        Await_Break_In_Load();

        break;

    case 1:
        if (Switch_Screen(0) != 0) {
            E_No[2] += 1;
            Cover_Timer = 23;
            G_No[1] = 1;
            G_No[2] = 0;
            G_No[3] = 0;

            if (E_No[3] == 0xFF) {
                E_Number[LOSER][0] = 1;
                E_Number[LOSER][1] = 0;
                E_Number[LOSER][2] = 0;
                E_Number[LOSER][3] = 0;
            } else {
                Correct_BI_Data();
            }

            E_No[0] = 2;
            E_No[1] = 0;
            E_No[2] = 0;
            E_No[3] = 0;
            Game_pause = 0;
            plw[New_Challenger].wu.operator = 1;
            Operator_Status[New_Challenger] = 1;

            if (Continue_Coin[New_Challenger] == 0) {
                grade_check_work_1st_init(New_Challenger, 0);
            }
        }

        break;
    }
}

void Entry_06() {
    Entry_Screen_Step(Entry_06_1st, Entry_06_2nd);
}

void Entry_06_1st() {
    Begin_Entry_Step(7);
}

/* One entrant takes a controller, and starts a fresh grade record unless continuing.
 * The two sides differed only in the player index. */
static void Give_Operator_To(s16 PL_id) {
    if (E_07_Flag[PL_id]) {
        plw[PL_id].wu.operator = 1;
        Operator_Status[PL_id] = 1;

        if (Continue_Coin[PL_id] == 0) {
            grade_check_work_1st_init(PL_id, 0);
        }
    }
}

/* Whoever broke into this screen takes a controller, and starts a fresh grade record
 * unless they are continuing. Shared verbatim by the three screens that hand over. */
static void Give_Operator_To_Entrants() {
    Give_Operator_To(0);
    Give_Operator_To(1);

    E_07_Flag[0] = 0;
    E_07_Flag[1] = 0;
}

void Entry_06_2nd() {
    if (E_07_Flag[0] == 0) {
        Entry_Main_Sub(0, 7);
    }

    if (E_07_Flag[1] == 0) {
        Entry_Main_Sub(1, 7);
    }

    switch (E_No[2]) {
    case 0:
        E_No[2] += 1;
        Switch_Screen_Init(1);
        break;

    case 1:
        if (Switch_Screen(1) != 0) {
            E_No[2] += 1;
            Cover_Timer = 23;
            return;
        }

        break;

    default:
        Switch_Screen(1);
        G_No[1] = 1;
        G_No[2] = 0;
        G_No[3] = 0;
        E_No[0] = 2;
        E_No[1] = 0;
        E_No[2] = 0;
        E_No[3] = 0;
        Fade_Flag = 0;

        Give_Operator_To_Entrants();

        if (E_Number[LOSER][0] == 5) {
            E_Number[LOSER][0] = 1;
        }

        break;
    }
}

void Entry_07() {
    Entry_Screen_Step(Entry_07_1st, Entry_07_2nd);
}

void Entry_07_1st() {
    Begin_Entry_Step(8);
}

void Entry_07_2nd() {
    if (E_07_Flag[0] == 0) {
        Entry_Main_Sub(0, 8);
    }

    if (E_07_Flag[1] == 0) {
        Entry_Main_Sub(1, 8);
    }

    switch (E_No[2]) {
    case 0:
        if (!--E_Timer) {
            E_No[2] += 1;
            Switch_Screen_Init(1);
        }

        break;

    default:
        if (Switch_Screen(1) != 0) {
            Reset_To_Game_Screen();

            Give_Operator_To_Entrants();
        }

        break;
    }
}

void Entry_08() {
    Entry_Screen_Step(Entry_08_1st, Entry_08_2nd);
}

void Entry_08_1st() {
    switch (E_No[2]) {
    case 0:
        E_No[2] += 1;
        /* fallthrough */

    case 1:
        Entry_Main_Sub(0, 9);
        Entry_Main_Sub(1, 9);
        break;
    }
}

/* The tail both the naming screen and the final-grade screen run: step any player who is
 * not breaking in, then cover the screen and hand the game to whoever did. Entry_08_2nd
 * and Entry_10_2nd were identical apart from the jump index. */
static void Finish_Entry_And_Start_Game(s16 Jump_Index) {
    if (E_07_Flag[0] == 0) {
        Entry_Main_Sub(0, Jump_Index);
    }

    if (E_07_Flag[1] == 0) {
        Entry_Main_Sub(1, Jump_Index);
    }

    switch (E_No[2]) {
    case 0:
        E_No[2] += 1;

        if ((E_Number[LOSER][0] == 8) && (E_Number[LOSER][1] == 1)) {
            Clear_Personal_Data(LOSER);
        }

        Switch_Screen_Init(1);
        break;

    default:
        if (Switch_Screen(1) != 0) {
            Reset_To_Game_Screen();

            Give_Operator_To_Entrants();
            Request_Disp_Rank[0][0] = -1;
            Request_Disp_Rank[0][1] = -1;
            Request_Disp_Rank[1][0] = -1;
            Request_Disp_Rank[1][1] = -1;
        }

        break;
    }
}

void Entry_08_2nd() {
    Finish_Entry_And_Start_Game(9);
}

void Entry_10() {
    if ((E_Number[0][0] == 0x63) && (E_Number[1][0] == 0x63)) {
        cpExitTask(TASK_ENTRY);
        return;
    }

    switch (E_No[1]) {
    case 0:
        Entry_10_1st();
        break;

    default:
        Entry_10_2nd();
        break;
    }
}

void Entry_10_1st() {
    switch (E_No[2]) {
    case 0:
        E_No[2] += 1;
        break;

    case 1:
        E_No[2] += 1;
        Setup_Final_Grade();

        if (Check_Ranking(WINNER) != 0) {
            E_Number[WINNER][0] = 2;
            E_Number[WINNER][1] = 0;
            E_Number[WINNER][2] = 0;
            E_Number[WINNER][3] = 0;
            Request_Disp_Rank[WINNER][0] = Rank_In[WINNER][0];
            Request_Disp_Rank[WINNER][1] = Rank_In[WINNER][1];
            Request_Disp_Rank[WINNER][2] = Rank_In[WINNER][2];
            Request_Disp_Rank[WINNER][3] = Rank_In[WINNER][3];
        } else {
            E_Number[WINNER][0] = 8;
            E_Number[WINNER][1] = 0;
        }

        /* fallthrough */

    default:
        Entry_Main_Sub(0, 10);
        Entry_Main_Sub(1, 10);
        break;
    }
}

void Entry_10_2nd() {
    Finish_Entry_And_Start_Game(10);
}

/* E_Number 0: the slot is free. Offer it to a player who is not already in, unless the
 * screen is one of the two that shut entry down instead. */
static void Offer_Free_Entry(s16 PL_id, s16 Jump_Index) {
    if (!Ignore_Entry[LOSER]) {
        if ((E_No[0] == 10) || (E_No[0] == 8)) {
            E_Number[PL_id][0] = 99;
            return;
        }

        if (plw[PL_id].wu.operator == 0) {
            Entry_Common_Sub(PL_id, Jump_Index);
            return;
        }
    }
}

/* E_Number 1: the player is on the continue countdown. Take a break-in if one is
 * pending, otherwise keep the countdown running. */
static void Run_Continue_Countdown(s16 PL_id, s16 Jump_Index) {
    if (PL_id) {
        if (Credit_Continue_2P() != 0) {
            Break_Into_Sub(PL_id, Jump_Index);
        }
    } else if (Credit_Continue_1P() != 0) {
        Break_Into_Sub(PL_id, Jump_Index);
    }

    if (Request_Break[PL_id]) {
        E_Number[PL_id][0] = 0;
        E_Number[PL_id][1] = 0;
        E_Number[PL_id][2] = 0;
        E_Number[PL_id][3] = 0;
        return;
    }

    if ((E_Number[PL_id][0] == 1) && (E_07_Flag[PL_id ^ 1] == 0)) {
        Entry_Continue_Sub(PL_id);
        return;
    }
}

/* E_Number 2 sub-state 2: the player is typing their name into the ranking table. */
static void Run_Name_Entry(s16 PL_id) {
    if (!(Forbid_Break != 1)) {
        return;
    }

    if (PL_id == 0) {
        Naming_Cut_Sub_1P();
    } else {
        Naming_Cut_Sub_2P();
    }

    if (!Name_Input(PL_id)) {
        return;
    }

    Name_In_Sub(PL_id);

    if (Naming_Cut[PL_id]) {
        Clear_Personal_Data(PL_id);
        return;
    }

    E_Number[PL_id][2] = 0;
    E_Number[PL_id][3] = 0;

    if (E_No[0] == 8) {
        E_Number[PL_id][0] = 8;
        E_Number[PL_id][1] = 1;
        return;
    }

    E_Number[PL_id][0] = 8;
    E_Number[PL_id][1] = 0;
    return;
}

/* E_Number 3: the player holds until the screen settles, then goes either to the
 * naming screen or straight to game over. */
/* Sub-state 1: once the screen settles, go to game over - or back to the head of the
 * entry list if it settled on the character select. */
static void Settle_To_Game_Over(s16 PL_id) {
    if ((E_No[0] == 8) || (E_No[0] == 2)) {
        E_Number[PL_id][0] = 8;
        E_Number[PL_id][1] = 1;
        E_Number[PL_id][2] = 0;
        E_Number[PL_id][3] = 0;

        if (E_No[0] == 2) {
            E_Number[PL_id][1] = 0;
            return;
        }
    }
}

static void Await_Screen_Settled(s16 PL_id) {
    switch (E_Number[PL_id][1]) {
    case 0:
        if ((E_No[0] == 8) || (E_No[0] == 2)) {
            E_Number[PL_id][0] = 2;
            E_Number[PL_id][1] = 2;
            E_Number[PL_id][2] = 0;
            E_Number[PL_id][3] = 0;
            Naming_Init(PL_id);
            return;
        }

        break;

    case 1:
        Settle_To_Game_Over(PL_id);

        break;
    }
}

/* E_Number 2: settle for a moment, then run the ranking-name entry. */
static void Run_Naming_Screen(s16 PL_id) {
    switch (E_Number[PL_id][1]) {
    case 0:
        E_Number[PL_id][1] += 1;
        Personal_Timer[PL_id] = 30;
        break;

    case 1:
        if (!--Personal_Timer[PL_id]) {
            E_Number[PL_id][1] += 1;
            Naming_Init(PL_id);
            return;
        }

        break;

    case 2:
        Run_Name_Entry(PL_id);
        break;
    }
}

/* E_Number 8: the game-over hold, either still counting down or already over. */
static void Run_Game_Over_Screen(s16 PL_id) {
    switch (E_Number[PL_id][1]) {
    case 0:
        In_Game_Sub(PL_id);
        break;

    case 1:
        In_Over_Sub(PL_id);
        break;
    }
}

void Entry_Main_Sub(s16 PL_id, s16 Jump_Index) {
    ENTRY_X = 0;

    switch (E_Number[PL_id][0]) {
    case 0:
        Offer_Free_Entry(PL_id, Jump_Index);
        break;

    case 1:
        Run_Continue_Countdown(PL_id, Jump_Index);
        break;

    case 2:
        Run_Naming_Screen(PL_id);

        break;

    case 3:
        Await_Screen_Settled(PL_id);
        break;

    case 8:
        Run_Game_Over_Screen(PL_id);

        break;

    case 5:
        Loser_Scene_Sub(PL_id, Jump_Index);
        break;
    }
}

void Naming_Init(s16 PL_id) {
    Naming_Cut[PL_id] = 0;
    Name_00[PL_id] = 0;
    name_wk[PL_id].r_no_0 = 0;
    name_wk[PL_id].r_no_1 = 0;
    end_name_cut[PL_id] = 0;
}

void Naming_Cut_Sub_1P() {
    if (!Naming_Cut[0] && (Ck_Break_Into_SP(p1sw_0, p1sw_1, 0) != 0)) {
        Game_pause = 0;
        Naming_Cut[0] = 1;
        Request_Break[0] = 1;
    }
}

void Naming_Cut_Sub_2P() {
    if (!Naming_Cut[1] && (Ck_Break_Into_SP(p2sw_0, p2sw_1, 1) != 0)) {
        Game_pause = 0;
        Naming_Cut[1] = 1;
        Request_Break[1] = 1;
    }
}

void Name_In_Sub(s16 PL_id) {
    if (Rank_In[PL_id][0] >= 0) {
        Name_In_Sub0(PL_id, Rank_In[PL_id][0] + 0);
    }

    if (Rank_In[PL_id][1] >= 0) {
        Name_In_Sub0(PL_id, Rank_In[PL_id][1] + 5);
    }

    if (Rank_In[PL_id][2] >= 0) {
        Name_In_Sub0(PL_id, Rank_In[PL_id][2] + 10);
    }

    if (Rank_In[PL_id][3] >= 0) {
        Name_In_Sub0(PL_id, Rank_In[PL_id][3] + 15);
    }
}

void Name_In_Sub0(s16 PL_id, s16 xx) {
    Ranking_Data[xx].name[0] = rank_name_w[PL_id].code[0];
    Ranking_Data[xx].name[1] = rank_name_w[PL_id].code[1];
    Ranking_Data[xx].name[2] = rank_name_w[PL_id].code[2];
}

void Entry_Common_Sub(s16 PL_id, s16 Jump_Index) {
    if (PL_id) {
        if (Credit_Sub_2P() != 0) {
            Break_Into_Sub(PL_id, Jump_Index);
        }
    } else if (Credit_Sub_1P() != 0) {
        Break_Into_Sub(PL_id, Jump_Index);
    }
}

void Loser_Scene_Sub(s16 PL_id, s16 Jump_Index) {
    if (PL_id) {
        if (Loser_Sub_2P() != 0) {
            Break_Into_Sub(PL_id, Jump_Index);
        }
    } else if (Loser_Sub_1P() != 0) {
        Break_Into_Sub(PL_id, Jump_Index);
    }
}

s32 Loser_Sub_1P() {
    if ((Ck_Break_Into(p1sw_0, p1sw_1, 0) == 0) && !Request_Break[0]) {
        if (LOSER == 0) {
            if (save_w[1].extra_option.contents[3][5]) {
                SSPutStr(&(ScStr){ DE_X[0], 0, 9, "     CONTINUE?" }, TopHUDPriority);
            }
        } else {
            Flash_Start(0);
        }
    }

    return ENTRY_X;
}

s32 Loser_Sub_2P() {
    if ((Ck_Break_Into(p2sw_0, p2sw_1, 1) == 0) && !Request_Break[1]) {
        if (LOSER == 1) {
            if (save_w[1].extra_option.contents[3][5]) {
                SSPutStr(&(ScStr){ DE_X[1], 0, 9, "     CONTINUE?" }, TopHUDPriority);
            }
        } else {
            Flash_Start(1);
        }
    }

    return ENTRY_X;
}

s32 Credit_Sub_1P() {
    if (Ck_Break_Into(p1sw_0, p1sw_1, 0) == 0) {
        if (Request_Break[0]) {
            Flash_Please(0);
        } else {
            Flash_Start(0);
        }
    }

    return ENTRY_X;
}

s32 Credit_Sub_2P() {
    if (Ck_Break_Into(p2sw_0, p2sw_1, 1) == 0) {
        if (Request_Break[1]) {
            Flash_Please(1);
        } else {
            Flash_Start(1);
        }
    }

    return ENTRY_X;
}

s32 Credit_Continue_1P() {
    Ck_Break_Into(p1sw_0, p1sw_1, 0);
    return ENTRY_X;
}

s32 Credit_Continue_2P() {
    Ck_Break_Into(p2sw_0, p2sw_1, 1);
    return ENTRY_X;
}

/* One tick of the continue countdown: a cut ends it early, the timer ends it when it
 * runs out, and the step after it is chosen once the credits are spent. */
static void Count_Down_Continue(s16 PL_id) {
    if (Check_Count_Cut(PL_id, 8)) {
        Continue_Cut[PL_id] = 1;
    } else if (--Personal_Timer[PL_id]) {
        return;
    }

    if (--Continue_Count[PL_id] >= 0) {
        Personal_Timer[PL_id] = 60;
        return;
    }

    Setup_Next_Step(PL_id);
}

void Entry_Continue_Sub(s16 PL_id) {
    if ((Continue_Count_Down[PL_id] == 0) && save_w[1].extra_option.contents[3][5]) {
        SSPutStr(&(ScStr){ DE_X[PL_id], 0, 9, "     CONTINUE?" }, TopHUDPriority);
        Disp_Personal_Count(PL_id, Continue_Count[PL_id]);
    }

    switch (E_Number[PL_id][1]) {
    case 0:
        if (Continue_Count_Down[PL_id] == 0) {
            E_Number[PL_id][1] += 1;
            Personal_Timer[PL_id] = 60;
            return;
        }

        break;

    case 1:
        Count_Down_Continue(PL_id);

        break;
    }
}

/* Off the last screen, a player goes to the naming screen if they ranked and to game
 * over if they did not - without waiting on the other player. */
static void Rank_Or_Game_Over(s16 PL_id) {
    Setup_Final_Grade();

    if (Check_Ranking(PL_id) != 0) {
        E_Number[PL_id][0] = 2;
        Request_Disp_Rank[PL_id][0] = Rank_In[PL_id][0];
        Request_Disp_Rank[PL_id][1] = Rank_In[PL_id][1];
        Request_Disp_Rank[PL_id][2] = Rank_In[PL_id][2];
        Request_Disp_Rank[PL_id][3] = Rank_In[PL_id][3];
        return;
    }

    E_Number[PL_id][0] = 8;
    E_Number[PL_id][1] = 0;
    return;
}

void Setup_Next_Step(s16 PL_id) {
    s16 xx;

    E_Number[PL_id][1] = 0;
    E_Number[PL_id][2] = 0;
    E_Number[PL_id][3] = 0;

    for (xx = 0; xx < 20; xx++) {
        Break_Com[PL_id][xx] = 0;
    }

    if (E_No[0] != 7) {
        Rank_Or_Game_Over(PL_id);
        return;
    }

    Setup_Final_Grade();

    if (Check_Ranking(PL_id) != 0) {
        Request_Disp_Rank[PL_id][0] = Rank_In[PL_id][0];
        Request_Disp_Rank[PL_id][1] = Rank_In[PL_id][1];
        Request_Disp_Rank[PL_id][2] = Rank_In[PL_id][2];
        Request_Disp_Rank[PL_id][3] = Rank_In[PL_id][3];

        if (E_Number[PL_id ^ 1][0] != 0) {
            E_Number[PL_id][0] = 2;
            return;
        }

        E_Number[PL_id][0] = 3;
        E_Number[PL_id][1] = 0;
        return;
    }

    if (E_Number[PL_id ^ 1][0] != 0) {
        E_Number[PL_id][0] = 8;
        E_Number[PL_id][1] = 0;
        return;
    }

    E_Number[PL_id][0] = 3;
    E_Number[PL_id][1] = 1;
}

/* The last stretch of the game-over wait: hold the player out of the entry list on
 * the two screens that end it, and otherwise release them back to it. */
static void Finish_Game_Over_Wait(s16 PL_id) {
    if (--Personal_Timer[PL_id] == 0) {
        if ((E_No[0] == 10) || (E_No[0] == 8)) {
            E_Number[PL_id][0] = 99;
            return;
        }

        Clear_Personal_Data(PL_id);
        Clear_Flash_No();
    }
}

void In_Game_Sub(s16 PL_id) {
    switch (E_Number[PL_id][2]) {
    case 0:
        E_Number[PL_id][2] += 1;
        Personal_Timer[PL_id] = 30;
        break;

    case 1:
        if (--Personal_Timer[PL_id] == 0) {
            E_Number[PL_id][2] += 1;
            Personal_Timer[PL_id] = 60;
            return;
        }

        break;

    case 2:
        if (save_w[1].extra_option.contents[3][5]) {
            SSPutStr(&(ScStr){ DE_X[PL_id], 0, 9, "     GAME OVER" }, TopHUDPriority);
        }

        if (--Personal_Timer[PL_id] == 0) {
            E_Number[PL_id][2] += 1;
            Personal_Timer[PL_id] = 30;
            return;
        }

        break;

    default:
        Finish_Game_Over_Wait(PL_id);

        break;
    }
}

void In_Over_Sub(s16 PL_id) {
    switch (E_Number[PL_id][2]) {
    case 0:
        E_Number[PL_id][2] += 1;
        break;
    }

    if (save_w[1].extra_option.contents[3][5]) {
        SSPutStr(&(ScStr){ DE_X[PL_id], 0, 9, "     GAME OVER" }, TopHUDPriority);
    }
}

static void begin_press_start_prompt(s16 PL_id) {
    if (!--F_Timer[PL_id]) {
        F_No1[PL_id] += 1;
        F_Timer[PL_id] = 50;

        if (save_w[1].extra_option.contents[3][5]) {
            if (PL_id) {
                SSPutStr(&(ScStr){ DE_X[1], 0, 9, "   PRESS 2P START" }, TopHUDPriority);
            } else {
                SSPutStr(&(ScStr){ DE_X[0], 0, 9, "   PRESS 1P START" }, TopHUDPriority);
            }
        }
    }
}

static void update_press_start_prompt(s16 PL_id) {
    if (--F_Timer[PL_id]) {
        if (save_w[1].extra_option.contents[3][5]) {
            if (PL_id) {
                SSPutStr(&(ScStr){ DE_X[1], 0, 9, "   PRESS 2P START" }, TopHUDPriority);
            } else {
                SSPutStr(&(ScStr){ DE_X[0], 0, 9, "   PRESS 1P START" }, TopHUDPriority);
            }
        }
    } else {
        F_No1[PL_id] -= 1;
        F_Timer[PL_id] = 30;
    }
}

/* Arm the press-start flasher, skipping straight to the continue prompt for the
 * loser on the continue screen. */
static void Begin_Flash_Cycle(s16 PL_id) {
    F_No1[PL_id] += 1;
    F_No0[PL_id] = 0;
    F_No2[PL_id] = 0;
    F_No3[PL_id] = 0;
    F_Timer[PL_id] = 1;

    if ((E_No[0] == 6) && (PL_id == LOSER)) {
        F_No1[PL_id] = 3;
    }
}

s32 Flash_Start(s16 PL_id) {
    switch (F_No1[PL_id]) {
    case 0:
        Begin_Flash_Cycle(PL_id);

        break;

    case 1:
        begin_press_start_prompt(PL_id);
        break;

    case 2:
        update_press_start_prompt(PL_id);
        break;

    case 3:
        F_No1[PL_id] = 99;
        /* fallthrough */

    default:
        if (save_w[1].extra_option.contents[3][5]) {
            SSPutStr(&(ScStr){ DE_X[1], 0, 9, "     CONTINUE?" }, TopHUDPriority);
        }

        break;
    }

    return 0;
}

s32 Flash_Please(s16 PL_id) {
    if (E_No[0] == 6 || E_No[0] == 8) {
        return 0;
    }

    switch (F_No3[PL_id]) {
    case 0:
        F_No3[PL_id] += 1;
        F_No1[PL_id] = 0;
        F_Timer[PL_id] = 1;
        break;

    case 1:
        if (--F_Timer[PL_id] == 0) {
            F_No3[PL_id] += 1;
            F_Timer[PL_id] = 50;
        }

        break;

    default:
        if (--F_Timer[PL_id]) {
            SSPutStr(&(ScStr){ DE_X[PL_id], 0, 9, "    PLEASE WAIT" }, TopHUDPriority);
        } else {
            F_No3[PL_id] -= 1;
            F_Timer[PL_id] = 30;
        }

        break;
    }

    return 0;
}

void Correct_BI_Data() {
    Super_Arts_Finish[Player_id] -= Stage_SA_Finish[Player_id];
    Lost_Round[Player_id] -= Stage_Lost_Round[Player_id];
    Perfect_Finish[Player_id] -= Stage_Perfect_Finish[Player_id];
    Cheap_Finish[Player_id] -= Stage_Cheap_Finish[Player_id];
}
