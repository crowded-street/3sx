/**
 * @file pass_patterns_5step.c
 * COM Passive: pattern skeletons shared by every character
 *
 * The five-step patterns.
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

void pattern_approach_walk_em_term_normal_attack(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        SA_Term(wk, p);
        break;

    case 4:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_normal_attack_5(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        SA_Term(wk, p);
        break;

    case 4:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_sa_term_2(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x4B, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 3, 0x38, 0x44, 0x45}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_wait_get_up_normal_attack_2(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x71, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x20);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_sa_term_em_term(
    PLW* wk, const EM_Term_Params* p, const EM_Term_Params* p_b, const Command_Attack_Args* p_b_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 2:
        EM_Term(wk, p_b);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0});
        break;

    case 4:
        Command_Attack(wk, p_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_attack_term_normal_attack_com_random_select(
    PLW* wk, const Jump_Term_Args* a, const Branch_Menu_Args* p, s16 rnd_type
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 4:
        Com_Random_Select(wk, p, rnd_type);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_j_command_attack_com_random_select(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x200);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_jump_attack_term_normal_attack(PLW* wk, const SA_Term_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20});
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x12);
        break;

    case 3:
        SA_Term(wk, p);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_normal_attack_4(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        EM_Term(wk, p);
        break;

    case 2:
        Normal_Attack(wk, reaction, 0x42);
        break;

    case 3:
        Wait(wk, 5);
        break;

    case 4:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term_approach_walk_sa_term_2(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x41, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0x41});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 3, 0x38, 0x44, 0x45}, 1);
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
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x47});
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

void pattern_search_back_term_walk_wait_2(PLW* wk) {
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

void pattern_walk_search_back_term_walk(PLW* wk) {
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
