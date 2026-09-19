/**
 * @file pass_patterns_1step.c
 * COM Passive: pattern skeletons shared by every character
 *
 * The one-step patterns.
 *
 * A passive pattern script is a switch on the step counter with one engine call
 * per step, and the same step sequences recur across characters. Each skeleton
 * here is exactly the body its call sites used to hold, with the arguments of
 * its calls taken as parameters and written out in full at each call site.
 */

#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void pattern_adjust_attack(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk(PLW* wk, s16 target_pos) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_branch_unit_area(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Branch_Unit_Area(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, p, rnd_type);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_com_random_select_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x37, 0x37, 0x27, 0x27}, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_command_attack_2(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_command_attack_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_command_attack_from_step_6_2(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_forced_guard(PLW* wk, s16 guard_type) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Forced_Guard(wk, guard_type);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_hi_jump(PLW* wk, s16 pl_number) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump(wk, pl_number, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_hi_jump_attack_term(PLW* wk, const Hi_Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_hi_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Command_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_j_command_attack_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump(PLW* wk, s16 time) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, time);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_attack(PLW* wk, const Jump_Attack_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_keep_away(PLW* wk, s16 target_pos, s16 option) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, target_pos, option);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_keep_status(PLW* wk, s16 option_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Status(wk, 2, option_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, lever, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_look(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_provoke(PLW* wk, s16 lever) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, lever);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term(PLW* wk, const SA_Term_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_short_range_attack(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Short_Range_Attack(wk, &(Short_Range_Args){8, 0x40, 6, 0x1D});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_vs_jump_guard(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        VS_Jump_Guard(wk);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait(PLW* wk, s16 time) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, time);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_attack_complete(PLW* wk, s16 option) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Attack_Complete(wk, 3, option);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up(PLW* wk, u16 lever_data, s16 option) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, lever_data, option);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_2(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, lever_data, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_walk(PLW* wk, s16 unused) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x20, unused);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
