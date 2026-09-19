/**
 * @file active_patterns_1step.c
 * COM Passive: pattern skeletons shared by every character
 *
 * The one-step patterns.
 *
 * A passive pattern script is a switch on the step counter with one engine call
 * per step, and the same step sequences recur across characters. Each skeleton
 * here is exactly the body its call sites used to hold, with the arguments of
 * its calls taken as parameters and written out in full at each call site.
 */

#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void active_pattern_approach_walk(PLW* wk, s16 target_pos) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_branch_unit_area(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Branch_Unit_Area(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, p, rnd_type);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_hi_jump_attack_term(PLW* wk, const Hi_Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_hi_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Command_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_jump_attack(PLW* wk, const Jump_Attack_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_keep_away(PLW* wk, s16 target_pos, s16 option) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, target_pos, option);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_lever_attack(PLW* wk, s16 reaction, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, reaction, lever, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_oro_ja_term(PLW* wk, const ORO_Air_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_JA_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_sa_term(PLW* wk, const SA_Term_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_wait(PLW* wk, s16 time) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, time);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_walk(PLW* wk, u16 lever, s16 time, s16 unused) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, lever, time, unused);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_jump(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_hi_jump_attack(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack(wk, &(Jump_Attack_Args){8, 0xA, 0x400, 2});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_provoke(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_sp(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack_SP(wk, 8, 0x200, 0x12);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_j_command_attack_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1e, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_j_command_attack_from_step_6_2(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1e, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
