/**
 * @file active_patterns_5step.c
 * COM Passive: pattern skeletons shared by every character
 *
 * The five-step patterns.
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

void active_pattern_jump_attack_term_normal_attack_command_attack(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const SA_Term_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 9, lever_data);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1D, 0xA, -1});
        break;

    case 3:
        Wait(wk, 5);
        break;

    case 4:
        SA_Term(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_search_back_term_command_attack_sa_term(
    PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, 2, next_menu);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x34, 0x34, 0x34, 0x47});
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 1});
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
