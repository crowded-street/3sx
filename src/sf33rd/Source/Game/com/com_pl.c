/**
 * @file com_pl.c
 * CPU Controlled Character
 */

#include "sf33rd/Source/Game/com/com_pl.h"
#include "common.h"
#include "sf33rd/Source/Game/com/active/active00.h"
#include "sf33rd/Source/Game/com/active/active01.h"
#include "sf33rd/Source/Game/com/active/active02.h"
#include "sf33rd/Source/Game/com/active/active03.h"
#include "sf33rd/Source/Game/com/active/active04.h"
#include "sf33rd/Source/Game/com/active/active05.h"
#include "sf33rd/Source/Game/com/active/active06.h"
#include "sf33rd/Source/Game/com/active/active07.h"
#include "sf33rd/Source/Game/com/active/active08.h"
#include "sf33rd/Source/Game/com/active/active09.h"
#include "sf33rd/Source/Game/com/active/active10.h"
#include "sf33rd/Source/Game/com/active/active11.h"
#include "sf33rd/Source/Game/com/active/active12.h"
#include "sf33rd/Source/Game/com/active/active13.h"
#include "sf33rd/Source/Game/com/active/active14.h"
#include "sf33rd/Source/Game/com/active/active15.h"
#include "sf33rd/Source/Game/com/active/active16.h"
#include "sf33rd/Source/Game/com/active/active17.h"
#include "sf33rd/Source/Game/com/active/active18.h"
#include "sf33rd/Source/Game/com/active/active19.h"
#include "sf33rd/Source/Game/com/ck_pass.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/com/follow/follow02.h"
#include "sf33rd/Source/Game/com/passive/pass00.h"
#include "sf33rd/Source/Game/com/passive/pass01.h"
#include "sf33rd/Source/Game/com/passive/pass02.h"
#include "sf33rd/Source/Game/com/passive/pass03.h"
#include "sf33rd/Source/Game/com/passive/pass04.h"
#include "sf33rd/Source/Game/com/passive/pass05.h"
#include "sf33rd/Source/Game/com/passive/pass06.h"
#include "sf33rd/Source/Game/com/passive/pass07.h"
#include "sf33rd/Source/Game/com/passive/pass08.h"
#include "sf33rd/Source/Game/com/passive/pass09.h"
#include "sf33rd/Source/Game/com/passive/pass10.h"
#include "sf33rd/Source/Game/com/passive/pass11.h"
#include "sf33rd/Source/Game/com/passive/pass12.h"
#include "sf33rd/Source/Game/com/passive/pass13.h"
#include "sf33rd/Source/Game/com/passive/pass14.h"
#include "sf33rd/Source/Game/com/passive/pass15.h"
#include "sf33rd/Source/Game/com/passive/pass16.h"
#include "sf33rd/Source/Game/com/passive/pass17.h"
#include "sf33rd/Source/Game/com/passive/pass18.h"
#include "sf33rd/Source/Game/com/passive/pass19.h"
#include "sf33rd/Source/Game/com/shell/shell00.h"
#include "sf33rd/Source/Game/com/shell/shell01.h"
#include "sf33rd/Source/Game/com/shell/shell03.h"
#include "sf33rd/Source/Game/com/shell/shell04.h"
#include "sf33rd/Source/Game/com/shell/shell05.h"
#include "sf33rd/Source/Game/com/shell/shell07.h"
#include "sf33rd/Source/Game/com/shell/shell11.h"
#include "sf33rd/Source/Game/com/shell/shell12.h"
#include "sf33rd/Source/Game/com/shell/shell13.h"
#include "sf33rd/Source/Game/com/shell/shell14.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/getup.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plmain.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/com/com_pl_internal.h"

void Main_Program(PLW* wk);

static u16 CPU_Sub(PLW* wk);
static s32 Check_Counter_Attack(PLW* wk);
static s16 Check_Hamari(PLW* wk);
static s32 Check_No12_Shell_Guard(PLW* wk, WORK_Other* tmw);
static s32 Ck_Exit_Guard(PLW* wk, WORK* em);
static s32 Ck_Exit_Guard_Sub(PLW* wk, WORK* em);

void Com_Initialize(PLW* wk);
void Com_Free(PLW* wk);
void Com_Active(PLW* wk);
void Com_Before_Follow(PLW* wk);
void Com_Follow(PLW* wk);
void Com_Before_Passive(PLW* wk);
void Com_Passive(PLW* wk);
void Com_Guard(PLW* wk);
void Com_VS_Shell(PLW* wk);
void Com_Guard_VS_Shell(PLW* wk);
void Com_Damage(PLW* wk);
void Com_Float(PLW* wk);
void Com_Flip(PLW* wk);
void Com_Caught(PLW* wk);
void Com_Wait_Lie(PLW* wk);
void Com_Catch(PLW* wk);

void Damage_1st(PLW* wk);
void Damage_2nd(PLW* wk);
void Damage_3rd(PLW* /* unused */);
void Damage_4th(PLW* /* unused */);
void Damage_5th(PLW* wk);
void Damage_6th(PLW* wk);
void Damage_7th(PLW* wk);
void Damage_8th(PLW* wk);

void Exit_Damage_Sub(PLW* wk);
static s32 Check_Damage(PLW* wk);

void Float_2nd(PLW* wk);
void Float_3rd(PLW* wk);
void Float_4th(PLW* wk);

void Flip_Zero(PLW* wk);
void Flip_1st(PLW* wk);
void Flip_2nd(PLW* wk);
void Flip_3rd(PLW* wk);
void Flip_4th(PLW* wk);

s32 Check_Flip(PLW* wk);
static s16 Decide_Exit_Catch(PLW* wk);
s32 Com_Rapid_Sub(PLW* wk, s16 Shot, u8* dir_step);
static s32 Check_Caught(PLW* wk);
s32 Command_Attack_SP(PLW* wk, s8 Pl_Number, s16 Tech_Number, s16 Power_Level);
void Next_Be_Float(PLW* wk);
void Clear_Com_Flag(PLW* wk);
void Check_At_Count(PLW* wk);
void Shift_Resume_Lv(PLW* wk);
void Check_Store_Lv(PLW* wk);
void Store_LR_Sub(PLW* wk);
void Setup_Bullet_Counter(PLW* wk);
void Pattern_Insurance(PLW* wk, s16 Kind_Of_Insurance, s16 Forced_Number);

const u16 Correct_Lv_Data[16] = { 0, 1, 2, 2, 4, 5, 6, 5, 8, 9, 10, 9, 8, 5, 10, 0 };
/* The guard is in its middle frames with nothing queued behind it, so there is
 * nothing to exit into yet. */
static s32 guard_is_mid_sequence(PLW* wk) {
    return wk->wu.routine_no[2] >= 4 && wk->wu.routine_no[2] < 8 && wk->wu.cmwk[0xE] == 0 &&
           Attack_Flag[wk->wu.id] == 0;
}

/* Being hit while running a script that is neither of the two the damage answer
 * leaves alone, and not already guarding. */
static s32 damage_interrupts_this_script(PLW* wk) {
    return wk->wu.routine_no[1] == 1 && CP_No[wk->wu.id][0] != 7 && CP_No[wk->wu.id][0] != 9 &&
           Guard_Flag[wk->wu.id] == 0;
}

u16 cpu_algorithm(PLW* wk) {
    u16 sw = CPU_Sub(wk);

    if (Play_Mode == 1 && Replay_Status[wk->wu.id] != 99) {
        if (wk->wu.id) {
            p2sw_0 = sw;
        } else {
            p1sw_0 = sw;
        }

        if (CPU_Time_Lag[wk->wu.id]) {
            CPU_Rec[wk->wu.id] = 1;
            return sw;
        }

        CPU_Rec[wk->wu.id] = 1;
        Check_Replay_Status(wk->wu.id, 1);
    }

    return sw;
}

static u16 CPU_Sub(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (Allow_a_battle_f == 0 || pcon_dp_flag) {
        return 0;
    }

    Lever_Buff[wk->wu.id] = 0;

    if (em->pat_status == 0x26) {
        Lie_Flag[wk->wu.id] = 1;
    } else {
        Lie_Flag[wk->wu.id] = 0;
    }

    Last_Pattern_Index[wk->wu.id] = Pattern_Index[wk->wu.id];
    Main_Program(wk);
    Lever_Buff[wk->wu.id] = check_illegal_lever_data(Lever_Buff[wk->wu.id]);
    Check_Store_Lv(wk);
    Shift_Resume_Lv(wk);
    return Lever_Buff[wk->wu.id];
}

void Main_Program(PLW* wk) {
    void (*Com_Jmp_Tbl[16])(PLW*) = { Com_Initialize, Com_Free,           Com_Active,   Com_Before_Follow,
                                      Com_Follow,     Com_Before_Passive, Com_Passive,  Com_Guard,
                                      Com_VS_Shell,   Com_Guard_VS_Shell, Com_Damage,   Com_Float,
                                      Com_Flip,       Com_Caught,         Com_Wait_Lie, Com_Catch };

    Ck_Distance(wk);
    Area_Number[wk->wu.id] = Ck_Area(wk);
    Attack_Flag[wk->wu.id] = plw[wk->wu.id ^ 1].caution_flag;
    Check_At_Count(wk);
    Disposal_Again[wk->wu.id] = 0;
    Com_Jmp_Tbl[CP_No[wk->wu.id][0]](wk);

    if (Disposal_Again[wk->wu.id]) {
        Com_Jmp_Tbl[CP_No[wk->wu.id][0]](wk);
    }
}

void Com_Initialize(PLW* wk) {
    const s16* xx;
    s16 i;

    CP_No[wk->wu.id][0] = 1;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Lever_Squat[wk->wu.id] = 0;
    Lever_Store[wk->wu.id][0] = 0;
    Lever_Store[wk->wu.id][1] = 0;
    Lever_Store[wk->wu.id][2] = 0;
    Attack_Counter[wk->wu.id] = 0;
    Bullet_No[wk->wu.id] = 0;
    Last_Attack_Counter[wk->wu.id] = -1;
    Guard_Counter[wk->wu.id] = -1;
    Turn_Over_Timer[wk->wu.id] = 1;
    Attack_Count_Index[wk->wu.id] = 0;
    Flip_Counter[wk->wu.id] = 0;
    Lever_LR[0] = 0;
    Lever_LR[1] = 0;
    xx = Area_Unit_Data[wk->player_number];
    Separate_Area[wk->wu.id][0] = xx[0];
    Separate_Area[wk->wu.id][1] = xx[1];
    Separate_Area[wk->wu.id][2] = xx[2];
    xx = Shell_Area_Unit_Data[wk->player_number];
    Shell_Separate_Area[wk->wu.id][0] = xx[0];
    Shell_Separate_Area[wk->wu.id][1] = xx[1];
    Shell_Separate_Area[wk->wu.id][2] = xx[2];
    Com_Width_Data[wk->wu.id] = PL_Body_Width_Data[wk->player_number];
    Clear_Com_Flag(wk);
    Standing_Master_Timer[wk->wu.id] = Setup_Next_Stand_Timer(wk);
    Squat_Master_Timer[wk->wu.id] = Setup_Next_Squat_Timer(wk);
    Squat_Master_Timer[wk->wu.id] = 0;
    Setup_Bullet_Counter(wk);

    for (i = 0; i < 20; i++) {
        Resume_Lever[wk->wu.id][i] = 0;
    }

    for (i = 0; i < 3; i++) {
        Attack_Count_Buff[wk->wu.id][i] = -1;
    }
}

/* Six of the CPU's modes open the same way: being hit, being caught or having
 * to turn round all take the frame, and the mode does not run. The three checks
 * are in the order they were in, and each still answers for itself. */
static s32 mode_was_interrupted(PLW* wk) {
    if (Check_Damage(wk)) {
        return 1;
    }

    if (Check_Caught(wk)) {
        return 1;
    }

    if (Check_Flip(wk)) {
        return 1;
    }

    return 0;
}

void Com_Free(PLW* wk) {
    s16 xx;

    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    if (Check_Damage(wk)) {
        return;
    }

    if (Check_Caught(wk)) {
        return;
    }

    CP_No[wk->wu.id][0] = 2;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;

    if (Before_Look[wk->wu.id]) {
        xx = Standing_Timer[wk->wu.id];
    } else {
        xx = 0;
    }

    Clear_Com_Flag(wk);
    Standing_Timer[wk->wu.id] = xx;

    for (xx = 0; xx <= 7; xx++) {
        CP_Index[wk->wu.id][xx] = 0;
    }

    Select_Active(wk);
}

void Com_Before_Follow(PLW* wk) {
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    if (Check_Damage(wk)) {
        return;
    }

    if (Check_Caught(wk)) {
        return;
    }

    if (Check_Guard(wk)) {
        return;
    }

    if (Check_Flip(wk)) {
        return;
    }

    if (--Timer_00[wk->wu.id] != 0) {
        return;
    }

    Decide_Follow_Menu(wk);
    CP_No[wk->wu.id][0] = 4;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    CP_Index[wk->wu.id][0] = 0;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;
    Clear_Com_Flag(wk);
}

void Com_Before_Passive(PLW* wk) {
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    if (mode_was_interrupted(wk)) {
        return;
    }

    if (!Limited_Flag[wk->wu.id] && !Counter_Attack[wk->wu.id]) {
        if (Check_Guard(wk)) {
            return;
        }
    }

    if (--Timer_00[wk->wu.id] != 0) {
        return;
    }

    CP_No[wk->wu.id][0] = 6;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    CP_Index[wk->wu.id][0] = 0;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;
}

void Com_Guard(PLW* wk) {
    WORK* em;

    if (mode_was_interrupted(wk)) {
        return;
    }

    if (wk->wu.routine_no[1] == 1 && PL_Blow_Off_Data[wk->wu.routine_no[2]] == 2) {
        Next_Be_Float(wk);
        return;
    }

    em = (WORK*)wk->wu.target_adrs;

    if (Ck_Exit_Guard(wk, em)) {
        Check_Guard_Type(wk, em);
        return;
    }

    Passive_Flag[wk->wu.id] = 0;
    Passive_Mode = 4;

    if (Ck_Passive_Term(wk)) {
        Select_Passive(wk);
        Counter_Attack[wk->wu.id] |= 2;
        return;
    }

    if (!Check_Counter_Attack(wk)) {
        Next_Be_Free(wk);
        return;
    }

    if (Select_Passive(wk) == -1) {
        Next_Be_Free(wk);
    }
}

static s32 Check_Counter_Attack(PLW* wk) {
    s16 xx;

    if (Area_Number[wk->wu.id] >= 3) {
        return 0;
    }

    xx = Type_of_Attack[wk->wu.id] & 0xF8;

    if (xx == 8) {
        VS_Tech[wk->wu.id] = 28;
        return 1;
    }

    if (xx == 24) {
        VS_Tech[wk->wu.id] = 14;
        return 1;
    }

    if (xx == 32) {
        VS_Tech[wk->wu.id] = 14;
        return 1;
    }

    if (xx == 48) {
        VS_Tech[wk->wu.id] = 14;
        return 1;
    }

    return Check_Hamari(wk);
}

static s16 Check_Hamari(PLW* wk) {
    u8 tech;
    s16 Rnd;
    s16 limit;
    s16 xx;

    if (Area_Number[wk->wu.id] >= 2) {
        return 0;
    }

    tech = Attack_Count_Buff[wk->wu.id][0];
    Rnd = random_32_com() & 1;
    limit = Rnd + 3;

    if (((PLW*)wk->wu.target_adrs)->player_number == 4 && tech == 3) {
        limit--;
    } else if (tech != 0 && tech != 1) {
        return 0;
    }

    for (xx = 1; xx < limit; xx++) {
        if (tech != Attack_Count_Buff[wk->wu.id][xx]) {
            return 0;
        }
    }

    return VS_Tech[wk->wu.id] = 32;
}

/* The frame the shell guard ends on: the reasons the guard is dropped, and the
 * timer that holds it for one more frame otherwise. */
static void End_Guard_VS_Shell(PLW* wk, WORK_Other* tmw) {
    if (wk->player_number != 18) {
        if (wk->wu.routine_no[1] != 1) {
            Exit_Damage_Sub(wk);
        }
    } else if (Check_No12_Shell_Guard(wk, tmw) != 0) {
        Exit_Damage_Sub(wk);
    }

    if (tmw->wu.routine_no[0] == 2) {
        Exit_Damage_Sub(wk);
    }

    if (tmw->wu.id != 13) {
        Exit_Damage_Sub(wk);
    }

    Timer_00[wk->wu.id] = 1;
}

void Com_Guard_VS_Shell(PLW* wk) {
    WORK_Other* tmw;

    if (Check_Caught(wk)) {
        return;
    }

    if (Check_Flip(wk)) {
        return;
    }

    tmw = (WORK_Other*)Shell_Address[wk->wu.id];

    Check_Guard_Type(wk, &tmw->wu);

    if (Timer_00[wk->wu.id] == 0) {
        End_Guard_VS_Shell(wk, tmw);
        return;
    }

    Timer_00[wk->wu.id]--;
}

static s32 Shell_Past_Left_Guard(PLW* wk, WORK_Other* tmw) {
    s16 pos_x;

    pos_x = wk->wu.xyz[0].disp.pos - 48;

    if (tmw->wu.xyz[0].disp.pos < pos_x) {
        return 1;
    }

    return 0;
}

static s32 Shell_Past_Right_Guard(PLW* wk, WORK_Other* tmw) {
    s16 pos_x;

    pos_x = wk->wu.xyz[0].disp.pos + 48;

    if (tmw->wu.xyz[0].disp.pos > pos_x) {
        return 1;
    }

    return 0;
}

static s32 Check_No12_Shell_Guard(PLW* wk, WORK_Other* tmw) {

    if (wk->wu.rl_flag) {
        return Shell_Past_Left_Guard(wk, tmw);
    }

    return Shell_Past_Right_Guard(wk, tmw);
}

void Check_Guard_Type(PLW* wk, WORK* em) {
    Lever_Buff[wk->wu.id] = Setup_Guard_Lever(wk, 1);

    switch (Guard_Type[wk->wu.id]) {
    case 0:
        if (em->pat_status >= 0xE && em->pat_status <= 0x1E) {
            break;
        }

        if (em->att.guard & 16 || !(em->att.guard & 8)) {
            break;
        }

        Lever_Buff[wk->wu.id] |= 2;
        break;

    case 1:
        break;

    case 2:
        Lever_Buff[wk->wu.id] |= 2;
        break;
    }
}

static s32 Ck_Exit_Guard(PLW* wk, WORK* em) {
    s16 Lv;

    if (--Timer_00[wk->wu.id]) {
        return 1;
    }

    Timer_00[wk->wu.id] = 1;

    if (Ck_Exit_Guard_Sub(wk, em)) {
        if (Guard_Counter[wk->wu.id] == Attack_Counter[wk->wu.id]) {
            return 1;
        }

        Guard_Counter[wk->wu.id] = Attack_Counter[wk->wu.id];
        Lv = Setup_Lv10(0);

        if (Break_Into_CPU == 2) {
            Lv = 10;
        }

        if (Demo_Flag == 0 && Weak_PL == wk->wu.id) {
            Lv = 2;
        }

        Lv += CC_Value[0];
        Lv = emLevelRemake(Lv, 11, 1);

        if (EM_Rank != 0) {
            Guard_Type[wk->wu.id] = Guard_Data[17][Lv][random_16_com()];
        } else {
            Guard_Type[wk->wu.id] = Guard_Data[wk->player_number][Lv][random_16_com()];
        }

        return 1;
    }

    return 0;
}

static s32 Ck_Exit_Guard_Sub(PLW* wk, WORK* em) {
    if (Attack_Flag[wk->wu.id] == 0) {
        return 0;
    }

    if (wk->wu.routine_no[1] == 1) {
        if (wk->wu.routine_no[3] == 0) {
            return 1;
        }

        if (guard_is_mid_sequence(wk)) {
            return 0;
        }

        return 1;
    }

    if (em->routine_no[1] != 4) {
        return 0;
    }

    if (Attack_Flag[wk->wu.id] == 0) {
        return 0;
    }

    return 1;
}

void Com_Active(PLW* wk) {
    void (*Char_Jmp_Tbl[20])(PLW*) = { Computer00, Computer01, Computer02, Computer03, Computer04,
                                       Computer05, Computer06, Computer07, Computer08, Computer09,
                                       Computer10, Computer11, Computer12, Computer13, Computer14,
                                       Computer15, Computer16, Computer17, Computer18, Computer19 };

    if (mode_was_interrupted(wk)) {
        return;
    }

    Pattern_Insurance(wk, 0, 0);
    Char_Jmp_Tbl[wk->player_number](wk);
}

void Com_Follow(PLW* wk) {
    void (*Follow_Jmp_Tbl[20])(PLW*) = { Follow02, Follow02, Follow02, Follow02, Follow02, Follow02, Follow02,
                                         Follow02, Follow02, Follow02, Follow02, Follow02, Follow02, Follow02,
                                         Follow02, Follow02, Follow02, Follow02, Follow02, Follow02 };

    if (mode_was_interrupted(wk)) {
        return;
    }

    Pattern_Insurance(wk, 3, 2);
    Follow_Jmp_Tbl[wk->player_number](wk);
}

void Com_Passive(PLW* wk) {
    void (*Passive_Jmp_Tbl[20])(PLW*) = { Passive00, Passive01, Passive02, Passive03, Passive04, Passive05, Passive06,
                                          Passive07, Passive08, Passive09, Passive10, Passive11, Passive12, Passive13,
                                          Passive14, Passive15, Passive16, Passive17, Passive18, Passive19 };

    if (mode_was_interrupted(wk)) {
        return;
    }

    Pattern_Insurance(wk, 1, 1);
    Passive_Jmp_Tbl[wk->player_number](wk);
}

void Com_VS_Shell(PLW* wk) {
    void (*VS_Shell_Jmp_Tbl[20])(PLW*) = { Shell00, Shell01, Shell11, Shell03, Shell04, Shell05, Shell03,
                                           Shell07, Shell03, Shell03, Shell03, Shell11, Shell12, Shell13,
                                           Shell14, Shell11, Shell11, Shell11, Shell11, Shell11 };

    if (mode_was_interrupted(wk)) {
        return;
    }

    Pattern_Insurance(wk, 2, 0);
    VS_Shell_Jmp_Tbl[wk->player_number](wk);
}

void Com_Damage(PLW* wk) {
    void (*Damage_Jmp_Tbl[10])(PLW*) = { Damage_1st, Damage_2nd, Damage_3rd, Damage_4th, Damage_5th,
                                         Damage_6th, Damage_7th, Damage_7th, Damage_7th, Damage_8th };

    if (Check_Caught(wk)) {
        return;
    }

    if (Check_Flip(wk)) {
        return;
    }

    Damage_Jmp_Tbl[CP_No[wk->wu.id][1]](wk);
}

static s32 Check_Damage(PLW* wk) {
    if (Counter_Attack[wk->wu.id] & 2) {
        return 0;
    }

    if (damage_interrupts_this_script(wk)) {
        CP_No[wk->wu.id][0] = 10;
        CP_No[wk->wu.id][1] = 0;
        CP_No[wk->wu.id][2] = 0;
        CP_No[wk->wu.id][3] = 0;
        Receive_Flag[wk->wu.id] = 0;
        Lever_Buff[wk->wu.id] = 2;
        Clear_Com_Flag(wk);
        return 1;
    }

    return 0;
}

void Com_Float(PLW* wk) {
    void (*Float_Jmp_Tbl[4])(PLW*) = { Damage_2nd, Float_2nd, Float_3rd, Float_4th };

    if (Check_Caught(wk)) {
        return;
    }

    if (Check_Flip(wk)) {
        return;
    }

    Float_Jmp_Tbl[CP_No[wk->wu.id][1]](wk);
}

void Float_2nd(PLW* wk) {
    switch (CP_No[wk->wu.id][2]) {
    case 0:
        CP_No[wk->wu.id][2]++;
        Lever_Buff[wk->wu.id] = 16;
        break;

    default:
        if (wk->wu.routine_no[1] == 0) {
            Next_Be_Free(wk);
            break;
        }

        Check_Damage(wk);
        break;
    }
}

void Com_Flip(PLW* wk) {
    void (*Flip_Jmp_Tbl[5])(PLW*) = { Flip_Zero, Flip_1st, Flip_2nd, Flip_3rd, Flip_4th };

    if (Check_Damage(wk)) {
        return;
    }

    if (Check_Caught(wk)) {
        return;
    }

    Flip_Jmp_Tbl[CP_No[wk->wu.id][1]](wk);
}

/* Escaping the one throw that is mashed out of: twelve frames to react, and a
 * random escape window whose level the demo and the weaker side override. The
 * block is Com_Caught's own. */
static void setup_rapid_escape_timers(PLW* wk) {
    s16 Rnd;
    s16 Lv;

    Timer_00[wk->wu.id] = 12;
    Lv = Setup_Lv08(0);

    if (Break_Into_CPU == 2) {
        Lv = 7;
    }

    if (Demo_Flag == 0 && Weak_PL == wk->wu.id) {
        Lv = 0;
    }

    Timer_01[wk->wu.id] = Rapid_Exit_Data[emLevelRemake(Lv, 8, 0)][(Rnd = random_16_com() & 7)];
}

/* Waiting out a throw: freed, damaged, or mashing to escape. */
static void Wait_Caught_Release(PLW* wk) {
    if (wk->wu.routine_no[1] != 3) {
        if (wk->wu.routine_no[1] == 0) {
            Next_Be_Free(wk);
            return;
        }

        Check_Damage(wk);
        return;
    }

    Lever_Buff[wk->wu.id] = Com_Rapid_Sub(wk, 0xFF0, &CP_No[wk->wu.id][2]);
}

void Com_Caught(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    switch (CP_No[wk->wu.id][1]) {
    case 0:
        CP_No[wk->wu.id][1]++;
        CP_No[wk->wu.id][2] = 0;

        if (em->sp_tech_id == 1) {
            setup_rapid_escape_timers(wk);
            break;
        }

        Timer_00[wk->wu.id] = Decide_Exit_Catch(wk);
        Timer_01[wk->wu.id] = 1;
        break;

    case 1:
        Wait_Caught_Release(wk);
        break;
    }
}

static s16 Decide_Exit_Catch(PLW* wk) {
    s16 Rnd;
    s16 xx;
    s16 Lv = Setup_Lv18(save_w[Present_Mode].Difficulty + 0);

    Lv += CC_Value[0];

    if (Break_Into_CPU == 2) {
        Lv = 17;
    }

    Rnd = (u8)random_32_com();
    xx = Setup_EM_Rank_Index(wk);

    if (Rnd >= Exit_Throw_Data[xx][emLevelRemake(Lv, 18, 0)]) {
        return 0;
    }

    return 1;
}

const u8 Rapid_Lever_Data[2] = { 8, 4 };

s32 Com_Rapid_Sub(PLW* wk, s16 Shot, u8* dir_step) {
    u16 xx;

    if (--Timer_00[wk->wu.id] == 0) {
        Timer_00[wk->wu.id] = Timer_01[wk->wu.id];
        xx = Rapid_Lever_Data[dir_step[0]];
        xx |= Shot;
        dir_step[0]++;
        dir_step[0] &= 1;
        return xx;
    }

    return 0;
}

static s32 Check_Caught(PLW* wk) {
    if (wk->wu.routine_no[1] == 3) {
        CP_No[wk->wu.id][0] = 13;
        CP_No[wk->wu.id][1] = 0;
        CP_No[wk->wu.id][2] = 0;
        CP_No[wk->wu.id][3] = 0;
        Clear_Com_Flag(wk);
        return 1;
    }

    return 0;
}

void Com_Catch(PLW* wk) {
    WORK* em;
    s16 Rnd;
    s16 Lv;

    switch (CP_No[wk->wu.id][1]) {
    case 0:
        CP_No[wk->wu.id][1]++;
        CP_No[wk->wu.id][2] = 0;
        Timer_00[wk->wu.id] = 1;
        Lv = Setup_Lv04(0);

        if (Break_Into_CPU == 2) {
            Lv = 3;
        }

        Timer_01[wk->wu.id] = Rapid_Hit_Data[emLevelRemake(Lv, 4, 0)][(Rnd = random_16_com() & 7)];
        break;

    case 1:
        em = (WORK*)wk->wu.target_adrs;

        if (wk->wu.routine_no[1] != 2 || em->routine_no[1] != 3) {
            Next_Be_Free(wk);
            break;
        }

        Lever_Buff[wk->wu.id] = Com_Rapid_Sub(wk, 0xFF0, &CP_No[wk->wu.id][2]);
        break;
    }
}

void Be_Catch(PLW* wk) {
    CP_No[wk->wu.id][0] = 15;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Clear_Com_Flag(wk);
}

void Com_Wait_Lie(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (Check_Blow_Off(wk, em, 0)) {
        return;
    }

    Exit_Damage_Sub(wk);
}

/* One step of the technique's command script: which of the two command types
 * the current entry is, and what each does to the step index. The switch is
 * Command_Attack_SP's own. */
static void run_tech_command(PLW* wk, s16 Tech_Number, s16 Power_Level) {
    switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {
    default:
    case 1:
    case 10:
        if (Command_Type_00(wk, Power_Level & 0xF, Tech_Number, -1) == -1) {
            CP_Index[wk->wu.id][1] = 99;
        }

        break;

    case 2:
        if (Command_Type_01(wk, Power_Level & 0xF, -1)) {
            CP_Index[wk->wu.id][1]++;
        }

        break;
    }
}

/* The attack ran to the end and left the character idle and unalarmed. */
static s32 attack_finished_cleanly(PLW* wk) {
    return wk->wu.routine_no[1] == 0 && plw[wk->wu.id].caution_flag == 0;
}

s32 Command_Attack_SP(PLW* wk, s8 Pl_Number, s16 Tech_Number, s16 Power_Level) {
    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);
        Tech_Address[wk->wu.id] = player_cmd[Pl_Number][Tech_Number & 0xFF];
        Tech_Index[wk->wu.id] = 0xC;
        Check_Rapid(wk, Tech_Number);
        Rapid_Index[wk->wu.id] = 0x110;
        Lever_Pool[wk->wu.id] = 0x110;
        break;

    case 1:
        run_tech_command(wk, Tech_Number, Power_Level);

        if (CP_Index[wk->wu.id][1] == 2) {
            return 1;
        }

        break;

    case 2:
        if (wk->wu.cg_type == 64) {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
            CP_Index[wk->wu.id][1]++;
        }

        /* fallthrough */

    default:
        Rapid_Sub(wk);

        if (attack_finished_cleanly(wk)) {
            return 1;
        }
    }

    return 0;
}

void Next_Be_Free(PLW* wk) {
    CP_No[wk->wu.id][0] = 1;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Clear_Com_Flag(PLW* wk) {
    Passive_Flag[wk->wu.id] = 0;
    Flip_Flag[wk->wu.id] = 0;
    Counter_Attack[wk->wu.id] = 0;
    Limited_Flag[wk->wu.id] = 0;
    Guard_Flag[wk->wu.id] = 0;
    Before_Jump[wk->wu.id] = 0;
    Shell_Ignore_Timer[wk->wu.id] = 0;
    Pierce_Menu[wk->wu.id] = 0;
    Continue_Menu[wk->wu.id] = 0;
    Standing_Timer[wk->wu.id] = 0;
    Before_Look[wk->wu.id] = 0;
    Attack_Count_No0[wk->wu.id] = 0;
    Turn_Over[wk->wu.id] = 0;
    Jump_Pass_Timer[wk->wu.id][0] = 0;
    Jump_Pass_Timer[wk->wu.id][1] = 0;
    Jump_Pass_Timer[wk->wu.id][2] = 0;
    Jump_Pass_Timer[wk->wu.id][3] = 0;
    Last_Eftype[wk->wu.id] = 0;
}

/* The rolling record of the last four attacks: an attack that has just started
 * is counted and remembered, and the flag is cleared when it ends. The block is
 * Check_At_Count's own. */
static void record_attack_in_counter(PLW* wk, WORK* em) {
    if (Attack_Count_No0[wk->wu.id] == 0) {
        if (Attack_Flag[wk->wu.id]) {
            Attack_Counter[wk->wu.id]++;
            Attack_Count_No0[wk->wu.id] = 1;
            Type_of_Attack[wk->wu.id] = em->kind_of_waza;
            Attack_Count_Buff[wk->wu.id][Attack_Count_Index[wk->wu.id]] = em->kind_of_waza;
            Attack_Count_Index[wk->wu.id]++;
            Attack_Count_Index[wk->wu.id] &= 3;
        }
    } else if (Attack_Flag[wk->wu.id] == 0) {
        Attack_Count_No0[wk->wu.id] = 0;
    }
}

void Check_At_Count(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;
    s16 ix;

    record_attack_in_counter(wk, em);

    if (Attack_Flag[wk->wu.id]) {
        Reset_Timer[wk->wu.id] = 120;
        return;
    }

    if (--Reset_Timer[wk->wu.id] == 0) {
        for (ix = 0; ix < 4; ix++) {
            Attack_Count_Buff[wk->wu.id][ix] = ix;
        }
    }
}

void Shift_Resume_Lv(PLW* wk) {
    s16 xx;

    for (xx = 18; xx >= 0; xx--) {
        Resume_Lever[wk->wu.id][xx + 1] = Resume_Lever[wk->wu.id][xx];
    }

    Resume_Lever[wk->wu.id][0] = Lever_Buff[wk->wu.id];
}

void Check_Store_Lv(PLW* wk) {
    s16 xx = Lever_Buff[wk->wu.id] & 0xF;

    switch (xx) {
    case 2:
        Lever_Store[wk->wu.id][0]++;
        break;

    case 6:
    case 10:
        Store_LR_Sub(wk);
        Lever_Store[wk->wu.id][0]++;
        break;

    case 4:
    case 8:
        Store_LR_Sub(wk);
        break;

    default:
        Lever_Store[wk->wu.id][0] = 0;
        Lever_Store[wk->wu.id][1] = 0;
        Lever_Store[wk->wu.id][2] = 0;
        break;
    }
}

static void Store_LR_Facing_Left(PLW* wk) {
    if (Lever_Buff[wk->wu.id] & 8) {
        Lever_Store[wk->wu.id][1]++;
        Lever_Store[wk->wu.id][2] = 0;
    }

    if (Lever_Buff[wk->wu.id] & 4) {
        Lever_Store[wk->wu.id][1] = 0;
        Lever_Store[wk->wu.id][2]++;
    }
}

static void Store_LR_Facing_Right(PLW* wk) {
    if (Lever_Buff[wk->wu.id] & 4) {
        Lever_Store[wk->wu.id][1]++;
        Lever_Store[wk->wu.id][2] = 0;
    }

    if (Lever_Buff[wk->wu.id] & 8) {
        Lever_Store[wk->wu.id][1] = 0;
        Lever_Store[wk->wu.id][2]++;
    }
}

void Store_LR_Sub(PLW* wk) {
    if (wk->wu.rl_waza) {
        Store_LR_Facing_Left(wk);
        return;
    }

    Store_LR_Facing_Right(wk);
}

void Setup_Bullet_Counter(PLW* wk) {
    Bullet_Counter[wk->wu.id] = 3;
    Bullet_Counter[wk->wu.id] += random_32_com() & 1;
}

const u8 Pattern_Insurance_Data[20][4] = {
    { 67, 157, 10, 3 }, { 69, 175, 9, 3 },  { 74, 132, 10, 3 }, { 71, 135, 10, 3 },  { 67, 141, 11, 3 },
    { 66, 101, 10, 3 }, { 63, 146, 10, 3 }, { 75, 213, 11, 3 }, { 70, 213, 10, 3 },  { 100, 131, 10, 3 },
    { 69, 137, 10, 3 }, { 89, 254, 13, 3 }, { 85, 230, 10, 3 }, { 80, 167, 11, 3 },  { 150, 252, 12, 3 },
    { 68, 163, 13, 3 }, { 69, 166, 13, 3 }, { 82, 181, 13, 3 }, { 108, 203, 13, 3 }, { 78, 175, 13, 3 }
};

void Pattern_Insurance(PLW* wk, s16 Kind_Of_Insurance, s16 Forced_Number) {
    if (Pattern_Insurance_Data[wk->player_number][Kind_Of_Insurance] < Pattern_Index[wk->wu.id]) {
        Pattern_Index[wk->wu.id] = Forced_Number;
    }
}

