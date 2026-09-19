/**
 * @file pass_patterns_4step_2.c
 * COM Passive: pattern skeletons shared by every character
 *
 * The four-step patterns. Part 2 of 2, in name order.
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

void pattern_wait_get_up_normal_attack_sa_term(
    PLW* wk, s16 option, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, option);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_normal_attack_sa_term_2(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x20);
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_normal_attack_sa_term_3(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x22);
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_normal_attack_sa_term_4(
    PLW* wk, s16 option, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, option);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_pierce_on_command_attack(PLW* wk, const Command_Attack_Args* p, const EM_Term_Params* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    case 3:
        EM_Term(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_search_back_term_jump_attack_term(
    PLW* wk, s16 next_menu, const Jump_Term_Args* a, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Search_Back_Term(wk, 0xE0, 6, next_menu);
        break;

    case 2:
        Jump_Attack_Term(wk, a);
        break;

    case 3:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
