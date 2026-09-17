/**
 * @file entry_break_in.c
 * Breaking in to a screen: the credit and continue tests that accept a challenger,
 * and the per-screen handlers that take the game apart to let them in.
 *
 * Split out of entry.c, which was over the Lines of Code threshold.
 */

#include "sf33rd/Source/Game/screen/entry_break_in.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/effect/effa2.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "main.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/screen/entry.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "structs.h"

void Break_Into_02(s16 PL_id);
void Break_Into_04(s16 PL_id);
void Break_Into_05(s16 PL_id);
void Break_Into_07(s16 PL_id);
void Break_Into_08(s16 PL_id);
void Break_Into_09(s16 PL_id);
void Break_Into_10(s16 PL_id);
void Continue_Score_Sub(s16 PL_id);

/* The four screens that hand over rather than interrupt a fight. The case labels are
 * the original ones, so a jump index still reads as the number the rest of the file
 * uses; a value matching none of them did nothing before and still does nothing. */
static void Break_Into_Handover(s16 PL_id, s16 Jump_Index) {
    switch (Jump_Index) {
    case 7:
        Break_Into_07(PL_id);
        break;

    case 8:
        Break_Into_08(PL_id);
        break;

    case 9:
        Break_Into_09(PL_id);
        break;

    case 10:
        Break_Into_10(PL_id);
        break;

    default:
        break;
    }
}

void Break_Into_Sub(s16 PL_id, s16 Jump_Index) {
    switch (Jump_Index) {
    case 0:
    case 1:
    case 2:
    case 3:
        Break_Into_02(PL_id);
        break;

    case 4:
    case 6:
        Break_Into_04(PL_id);
        break;

    case 5:
        Break_Into_05(PL_id);
        break;

    default:
        Break_Into_Handover(PL_id, Jump_Index);
        break;
    }
}

/* A break-in is held off while the round forbids it or an extra is running. */
static s32 Break_Is_Forbidden() {
    return Forbid_Break || Extra_Break;
}

/* Outside the final screen, one player's pending break-in locks the other out. */
static s32 Other_Player_Is_Breaking_In(s16 PL_id) {
    return (E_No[0] != 10) && Request_Break[PL_id ^ 1];
}

s32 Ck_Break_Into(u16 Sw_0, u16 Sw_1, s16 PL_id) {
    if (Other_Player_Is_Breaking_In(PL_id)) {
        return 0;
    }

    if (Request_Break[PL_id]) {
        if (Break_Is_Forbidden()) {
            return 0;
        }

        Game_pause = 1;
        New_Challenger = PL_id;
        Champion = New_Challenger ^ 1;
        Request_Break[PL_id] = 0;
        return ENTRY_X = 1;
    }

    if (!(~Sw_1 & Sw_0 & 0x4000)) {
        return 0;
    }

    Continue_Score_Sub(PL_id);

    if (Break_Is_Forbidden()) {
        Request_Break[PL_id] = 1;
    } else {
        Game_pause = 1;
        New_Challenger = PL_id;
        Champion = New_Challenger ^ 1;
        return ENTRY_X = 1;
    }

    return 0;
}

s32 Ck_Break_Into_SP(u16 Sw_0, u16 Sw_1, s16 PL_id) {
    if (!(~Sw_1 & Sw_0 & 0x4000)) {
        return 0;
    }

    New_Challenger = PL_id;
    Champion = New_Challenger ^ 1;
    return ENTRY_X = 1;
}

/* Every break-in path opens by clearing the incoming player's entry state. */
static void Clear_New_Challenger_Entry() {
    E_Number[New_Challenger][0] = 0;
    E_Number[New_Challenger][1] = 0;
    E_Number[New_Challenger][2] = 0;
    E_Number[New_Challenger][3] = 0;
}

void Break_Into_02(s16 /* unused */) {
    plw[New_Challenger].wu.operator = 1;
    Operator_Status[New_Challenger] = 1;
    Clear_New_Challenger_Entry();

    if (Continue_Coin[New_Challenger] == 0) {
        grade_check_work_1st_init(New_Challenger, 0);
    }

    Select_Timer = 0x30;
    Unit_Of_Timer = UNIT_OF_TIMER_MAX;
}

void Break_Into_04(s16 /* unused */) {
    Break_Into = 1;
    E_No[1] += 1;
    E_No[2] = 0;
    E_Timer = 150;
    Clear_New_Challenger_Entry();
    effect_A2_init(0);
    sound_all_off();
    Sound_SE(0xB6);
    Request_LDREQ_Break();
}

/* The break-in lands on a settled screen: normal play, the round already concluded,
 * and nobody sitting in the champion's seat. */
static s32 No_Fight_To_Interrupt() {
    return (Play_Type == 0) && (Conclusion_Flag != 0) && (plw[Champion].wu.operator == 0);
}

/* Nothing to tear down: come straight back next frame, and mark whether the
 * challenger is the one who just lost. */
static void Break_In_Without_A_Fight() {
    E_Timer = 1;

    if (LOSER != New_Challenger) {
        E_No[3] = 0xFF;
    } else {
        E_No[3] = 0;
    }
}

/* A fight is running: bank the champion's score if the round had not finished,
 * then wind the round down and ask for the break-in load. */
static void Tear_Down_The_Fight() {
    E_Timer = 150;

    if (Conclusion_Flag == 0) {
        Score[Champion][0] = Stage_Stock_Score[Champion];
    }

    effect_A2_init(0);
    sound_all_off();
    Sound_SE(0xB6);
    Request_LDREQ_Break();
}

void Break_Into_05(s16 PL_id) {
    Break_Into = 1;
    Stop_Combo = 1;
    E_No[1] += 1;
    E_No[2] = 0;
    Clear_New_Challenger_Entry();

    if (No_Fight_To_Interrupt()) {
        Break_In_Without_A_Fight();
    } else {
        Tear_Down_The_Fight();
    }

    Stop_Update_Score = 1;
    cpExitTask(TASK_PAUSE);
}

/* The opening all three break-in paths share: register this entrant, and report
 * whether that was the second one - in which case the caller does nothing more. */
static s32 Register_Break_In_Both_Ready(s16 PL_id) {
    Clear_New_Challenger_Entry();
    E_07_Flag[PL_id] = 1;

    if (E_07_Flag[0] != 0 && E_07_Flag[1] != 0) {
        return 1;
    }

    return 0;
}

void Break_Into_07(s16 PL_id) {
    if (Register_Break_In_Both_Ready(PL_id)) {
        return;
    }

    E_No[1] += 1;
    E_No[2] = 0;
    Break_Into = 1;
}

void Break_Into_08(s16 PL_id) {
    if (Register_Break_In_Both_Ready(PL_id)) {
        return;
    }

    Break_Into = 1;
    E_No[1] += 1;
    E_No[2] = 0;

    if (Continue_Count[PL_id ^ 1] >= 0) {
        E_Timer = 60;
        return;
    }

    E_Timer = 10;
}

/* Break_Into_09 and Break_Into_10 were byte-identical: the challenger takes the
 * champion's seat once both entrants are not still waiting. */
static void Break_In_As_Champion(s16 PL_id) {
    if (Register_Break_In_Both_Ready(PL_id)) {
        return;
    }

    Break_Into = 1;
    E_No[1] += 1;
    E_No[2] = 0;
    Champion = New_Challenger;
}

void Break_Into_09(s16 PL_id) {
    Break_In_As_Champion(PL_id);
}

void Break_Into_10(s16 PL_id) {
    Break_In_As_Champion(PL_id);
}

void Continue_Score_Sub(s16 PL_id) {
    if ((E_Number[PL_id][0] == 1) || (E_Number[PL_id][0] == 5)) {
        Continue_Coin[PL_id] += 1;

        if (Continue_Coin[PL_id] >= 99) {
            Continue_Coin[PL_id] = 99;
        }
    }
}
