/**
 * @file pass_patterns_3step.c
 * COM Passive: pattern skeletons shared by every character
 *
 * The three-step patterns.
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

void pattern_approach_walk_em_term_lever_attack(PLW* wk, s16 target_pos, const EM_Term_Params* p, u16 lever) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, p);
        break;

    case 2:
        Lever_Attack(wk, 8, lever, 0x110);
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

void pattern_em_term_sa_term_j_command_attack(
    PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        SA_Term(wk, p_b);
        break;

    case 2:
        J_Command_Attack(wk, p_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_keep_away_wait_get_up_command_attack(PLW* wk, s16 target_pos, s16 option, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, target_pos, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 0, option);
        break;

    case 2:
        Command_Attack(wk, p);
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

void pattern_hi_jump_attack_term_normal_attack_2(
    PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data, u16 lever_data_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_wait_get_up_normal_attack(PLW* wk, s16 target_pos, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_attack_term_wait_get_up_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data);
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

void pattern_em_term_normal_attack_com_random_select(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 0xC, lever_data);
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x4B, 0x36, 0x3B, 0x70}, 1);
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

void pattern_jump_attack_term_sa_term_j_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40});
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

void pattern_approach_walk_normal_attack_branch_unit_area(PLW* wk, s16 target_pos, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
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

void pattern_approach_walk_wait_get_up_lever_attack(PLW* wk, s16 target_pos, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Lever_Attack(wk, 8, lever, lever_data);
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

void pattern_check_ex_wait_j_command_attack(PLW* wk, s16 time, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0x9B);
        break;

    case 1:
        Wait(wk, time);
        break;

    case 2:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_wait_get_up_command_attack(
    PLW* wk, s16 target_pos, s16 option, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, option);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_j_command_attack(
    PLW* wk, s16 target_pos, const EM_Term_Params* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
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

void pattern_approach_walk_sa_term_j_command_attack(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
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

void pattern_etc_term_provoke_next_another_menu(PLW* wk, u16 next_menu, u16 next_menu_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 4, 6, next_menu);
        break;

    case 1:
        Provoke(wk, -1);
        break;

    case 2:
        Next_Another_Menu(wk, 6, next_menu_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_etc_term_lever_attack(PLW* wk, s16 target_pos, u32 next_action, u16 next_menu) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        ETC_Term(wk, 0, next_action, next_menu);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_etc_term_normal_attack_lever_attack(PLW* wk, u16 next_menu, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 0, 6, next_menu);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x10);
        break;

    case 2:
        Lever_Attack(wk, 8, lever, lever_data);
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

void pattern_approach_walk_wait_get_up_j_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        J_Command_Attack(wk, p);
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

void pattern_em_term_sa_term_command_attack(
    PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
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

void pattern_jump_attack_term_sa_term_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -1, -1, -1});
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
