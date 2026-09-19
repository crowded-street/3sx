/**
 * @file pass_patterns_3step_2.c
 * COM Passive: pattern skeletons shared by every character
 *
 * The three-step patterns. Part 2 of 2, in name order.
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

void pattern_lever_on_em_term_j_command_attack(
    PLW* wk, u16 lr_lever, const EM_Term_Params* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, lr_lever, 2);
        break;

    case 1:
        EM_Term(wk, p);
        break;

    case 2:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_4(PLW* wk, s16 reaction, s16 reaction_b, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, 0x102);
        break;

    case 1:
        Normal_Attack(wk, reaction_b, lever_data);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_5(PLW* wk, s16 reaction, s16 reaction_b, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, 0x10);
        break;

    case 1:
        Normal_Attack(wk, reaction_b, lever_data);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_7(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, lever_data);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_branch_unit_area(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 2:
        Branch_Unit_Area(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_command_attack_3(PLW* wk, s16 reaction, s16 reaction_b, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, 0x102);
        break;

    case 1:
        Normal_Attack(wk, reaction_b, 0x202);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_command_attack_4(PLW* wk, s16 reaction, s16 reaction_b, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, 0x102);
        break;

    case 1:
        Normal_Attack(wk, reaction_b, 0x102);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_j_command_attack_3(PLW* wk, u16 lever_data, u16 lever_data_b, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, lever_data);
        break;

    case 1:
        Normal_Attack(wk, 9, lever_data_b);
        break;

    case 2:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_j_command_attack_4(PLW* wk, u16 lever_data, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, lever_data);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x42);
        break;

    case 2:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_j_command_attack_5(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x10);
        break;

    case 2:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_j_command_attack_com_random_select_2(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x200);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_sa_term_branch_unit_area(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 1:
        SA_Term(wk, p);
        break;

    case 2:
        Branch_Unit_Area(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_sa_term_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 1:
        SA_Term(wk, p);
        break;

    case 2:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_sa_term_command_attack_2(
    PLW* wk, s16 reaction, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, 0x100);
        break;

    case 1:
        SA_Term(wk, p);
        break;

    case 2:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_sa_term_j_command_attack(
    PLW* wk, u16 lever_data, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, lever_data);
        break;

    case 1:
        SA_Term(wk, p);
        break;

    case 2:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_sa_term_normal_attack(PLW* wk, const SA_Term_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 1:
        SA_Term(wk, p);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    case 2:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_branch_unit_area(
    PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    case 2:
        Branch_Unit_Area(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_j_command_attack(
    PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    case 2:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_normal_attack_3(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_j_command_attack(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    case 2:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_j_command_attack_normal_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_jump_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Jump(wk, 0);
        break;

    case 2:
        Com_Random_Select(wk, p, rnd_type);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_provoke_sa_term_com_random_select(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, 1);
        break;

    case 1:
        SA_Term(wk, p);
        break;

    case 2:
        Com_Random_Select(wk, p_b, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term_command_attack_2(
    PLW* wk, const SA_Term_Args* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, p);
        break;

    case 1:
        SA_Term(wk, p_b);
        break;

    case 2:
        Command_Attack(wk, p_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_walk_normal_attack(PLW* wk, s16 next_menu, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x48, 6, next_menu);
        break;

    case 1:
        Walk(wk, 1, 0x18, 0);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_walk_wait_get_up(PLW* wk, s16 move_value, s16 next_menu, s16 time) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, 6, next_menu);
        break;

    case 1:
        Walk(wk, 1, time, 0);
        break;

    case 2:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_walk_wait_get_up_2(PLW* wk, s16 move_value, s16 next_menu, s16 time) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, 6, next_menu);
        break;

    case 1:
        Walk(wk, 1, time, 0);
        break;

    case 2:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_attack_complete_sa_term_wait_attack_complete(PLW* wk, const SA_Term_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Attack_Complete(wk, 3, 1);
        break;

    case 1:
        SA_Term(wk, p);
        break;

    case 2:
        Wait_Attack_Complete(wk, 3, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_command_attack_em_term(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1});
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_jump_attack_term_j_command_attack(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Jump_Attack_Term(wk, a);
        break;

    case 2:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_normal_attack_j_command_attack(PLW* wk, u16 lever_data, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Normal_Attack(wk, 0xC, lever_data);
        break;

    case 2:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_sa_term_j_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        SA_Term(wk, p);
        break;

    case 2:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
