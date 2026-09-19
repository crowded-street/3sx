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
