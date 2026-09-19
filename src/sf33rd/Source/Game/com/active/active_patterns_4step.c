/**
 * @file active_patterns_4step.c
 * COM Passive: pattern skeletons shared by every character
 *
 * The four-step patterns.
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

void active_pattern_approach_walk_turn_over_on_jump_attack_term(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x57, 3);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40});
        break;

    case 3:
        Com_Random_Select(wk, p, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_command_attack_sa_term_em_term(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x34, 0x34, 0x34, 0x47});
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 1});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_command_attack_turn_over_on_jump_attack_term(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, -1, -1});
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40});
        break;

    case 3:
        Com_Random_Select(wk, p, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_lever_on_normal_attack_command_attack(
    PLW* wk, u16 lever_data, u16 lever_data_b, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 0xFFFF);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    case 3:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_pierce_on_search_back_term_command_attack(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Search_Back_Term(wk, 0x60, 2, 0x12);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_adjust_attack_lever_attack(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 9, 0x100);
        break;

    case 1:
        Adjust_Attack(wk, 0xC, 0x100);
        break;

    case 2:
        Adjust_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_search_back_term_pierce_on_command_attack_3(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 0);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    case 3:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 0, -0x7FA0, -1, 0x8400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_approach_walk_pierce_on_command_attack(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x58, 2);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xB, 0x21, 0xA, -1});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
