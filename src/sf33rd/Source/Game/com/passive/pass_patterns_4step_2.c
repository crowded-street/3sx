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
