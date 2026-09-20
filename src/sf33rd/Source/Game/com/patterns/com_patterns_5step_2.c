/**
 * @file com_patterns_5step_2.c
 * COM: pattern skeletons shared by every character
 *
 * The five-step patterns. Part 2 of 2, in name order.
 *
 * A COM pattern script is a switch on the step counter with one engine call
 * per step, and the same step sequences recur across characters. Each skeleton
 * here is exactly the body its call sites used to hold, with the arguments of
 * its calls taken as parameters and written out in full at each call site.
 */

#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void pattern_pierce_on_hi_jump_attack_term_lever_on_2(PLW* wk, const Hi_Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, a);
        break;

    case 2:
        Lever_On(wk, 1, 0xFFFF);
        break;

    case 3:
        Lever_Attack(wk, 8, 1, 0x402);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1C, 8, -1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_provoke_pierce_on_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, 1);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args) { 0xC, 0, 0xB, -1 });
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args) { 0xC, 0, 0xB, -1 });
        break;

    case 4:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term_approach_walk_sa_term_2(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args) { 0xFFFF, 0x2F, 0x30, 0 });
        break;

    case 1:
        Approach_Walk(wk, 0x41, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args) { 0x2E, 0xFFFF, 0xFFFF, 0x41 });
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1F, 10, -1 });
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 2, 3, 0x38, 0x44, 0x45 }, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term_approach_walk_sa_term_3(PLW* wk, const SA_Term_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, p);
        break;

    case 1:
        Approach_Walk(wk, 0x59, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args) { 0x2E, 0xFFFF, 0xFFFF, 0x59 });
        break;

    case 3:
        Approach_Walk(wk, 0x4F, 2);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1D, 0xA, -1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term_normal_attack_com_random_select(PLW* wk, const SA_Term_Args* p, s16 reaction) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 3:
        Normal_Attack(wk, reaction, 0x40);
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 6, 0x37, 0x37, 0x27, 0x27 }, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_command_attack_sa_term(
    PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, 6, next_menu);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args) { 0x2F, 0x30, 0x31, 0x47 });
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params) { -0x7FB0, -1, 5, 6, 1 });
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1E, 8, -1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_command_attack_sa_term_2(
    PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, 6, next_menu);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args) { 0x34, 0x34, 0x34, 0x47 });
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params) { -0x7FB0, -1, 5, 6, 1 });
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1E, 8, -1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_pierce_on_keep_away(PLW* wk, s16 next_menu, s16 target_pos, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, next_menu);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, target_pos, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 4:
        Branch_Unit_Area(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_pierce_on_keep_away_3(PLW* wk, s16 target_pos) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, target_pos, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_pierce_on_keep_away_4(PLW* wk, s16 move_value, s16 next_menu, s16 target_pos) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, 6, next_menu);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, target_pos, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_pierce_on_keep_away_6(PLW* wk, s16 target_pos, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 6, 0x68);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, target_pos, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 4:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, a);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 4:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_jump_attack_term_normal_attack_3(
    PLW* wk, const Jump_Term_Args* a, s16 reaction, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, a);
        break;

    case 2:
        Normal_Attack(wk, reaction, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 4:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
