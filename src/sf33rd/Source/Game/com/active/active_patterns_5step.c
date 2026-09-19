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

void active_pattern_search_back_term_walk_wait(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x30, 2, 0xF);
        break;

    case 1:
        Walk(wk, 1, 0x20, 0);
        break;

    case 2:
        Wait(wk, 3);
        break;

    case 3:
        Walk(wk, 0, 0x30, 0);
        break;

    case 4:
        Wait(wk, 9);
        break;

    case 5:
        Walk(wk, 0, 0x20, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_search_back_term_walk_wait_2(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x20, 2, 0x1B);
        break;

    case 1:
        Walk(wk, 1, 0x18, 0);
        break;

    case 2:
        Wait(wk, 8);
        break;

    case 3:
        Search_Back_Term(wk, 0x30, 2, 0x1B);
        break;

    case 4:
        Walk(wk, 1, 0x20, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_walk_search_back_term_walk(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 0, 0x20, 0);
        break;

    case 1:
        Search_Back_Term(wk, 0x30, 2, 6);
        break;

    case 2:
        Walk(wk, 1, 0x28, 0);
        break;

    case 3:
        Wait(wk, 8);
        break;

    case 4:
        Walk(wk, 0, 0x20, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_search_back_term_hi_jump_attack_term_pierce_on(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 0x11);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x20, 1, -0x7FA0, 8, 0x200});
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 0x18, 0x18, 0x11, 0x11}, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_search_back_term_pierce_on_command_attack_2(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 0x11);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 0x18, 0x18, 0x11, 0x11}, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_search_back_term_jump_attack_term_pierce_on(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 0);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x200, 0, -1, -1, -1});
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 0x18, 0x18, 0x11, 0x11}, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
