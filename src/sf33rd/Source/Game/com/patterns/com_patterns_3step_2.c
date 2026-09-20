/**
 * @file com_patterns_3step_2.c
 * COM: pattern skeletons shared by every character
 *
 * The three-step patterns. Part 2 of 3, in name order.
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

void pattern_approach_walk_normal_attack_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_normal_attack_j_command_attack(
    PLW* wk, s16 target_pos, u16 lever_data, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xB, lever_data);
        break;

    case 2:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_normal_attack_j_command_attack_2(PLW* wk, s16 target_pos, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 2:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_normal_attack_lever_attack_2(PLW* wk, s16 target_pos, u16 lever) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 2:
        Lever_Attack(wk, 8, lever, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_sa_term_command_attack_2(PLW* wk, s16 target_pos, const SA_Term_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        SA_Term(wk, p);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1C, 0xA, -1 });
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

void pattern_check_sa_full_em_term_command_attack(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA_Full(wk, 6, 0x7C);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params) { -0x7F30, -1, 5, 2, 0 });
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0x8019, 0xA, -1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_command_attack_2(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1C, 9, -1 });
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1C, 8, -1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_command_attack_em_term_command_attack(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1F, 0xA, -1 });
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params) { 0x50, -0x7FB0, 8, 1, -1 });
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0x2E, 8, -1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_lever_attack_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Lever_Attack(wk, reaction, 0, 0x200);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_normal_attack_3(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_normal_attack_4(PLW* wk, const EM_Term_Params* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x12);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_normal_attack_5(PLW* wk, const EM_Term_Params* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_normal_attack_6(PLW* wk, const EM_Term_Params* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x102);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_normal_attack_com_random_select(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params) { -1, -0x7FB0, 6, 1, -1 });
        break;

    case 1:
        Normal_Attack(wk, 0xC, lever_data);
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 6, 0x4B, 0x36, 0x3B, 0x70 }, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_normal_attack_command_attack(
    PLW* wk, const EM_Term_Params* p, u16 lever_data, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data);
        break;

    case 2:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_normal_attack_j_command_attack(
    PLW* wk, const EM_Term_Params* p, u16 lever_data, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, 0xB, lever_data);
        break;

    case 2:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_normal_attack_j_command_attack_2(
    PLW* wk, const EM_Term_Params* p, u16 lever_data, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, 9, lever_data);
        break;

    case 2:
        J_Command_Attack(wk, p_b);
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

void pattern_em_term_sa_term_forced_guard(PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        SA_Term(wk, p_b);
        break;

    case 2:
        Forced_Guard(wk, 0);
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

void pattern_em_term_search_back_term_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params) { -0x7FA0, -1, 0, 1, -1 });
        break;

    case 1:
        Search_Back_Term(wk, 0x30, 6, 0x6D);
        break;

    case 2:
        Command_Attack(wk, p);
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

void pattern_hi_jump_attack_term_normal_attack_4(
    PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data, u16 lever_data_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 0xB, lever_data);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_hi_jump_attack_term_normal_attack_lever_attack(PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_hi_jump_attack_term_normal_attack_lever_attack_2(PLW* wk, const Hi_Jump_Term_Args* a, s16 reaction) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, reaction, 0x10);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_attack_term_normal_attack_3(PLW* wk, const Jump_Term_Args* a, u16 lever_data, u16 lever_data_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
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

void pattern_jump_attack_term_normal_attack_4(PLW* wk, const Jump_Term_Args* a, u16 lever_data, u16 lever_data_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 0xB, lever_data);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_attack_term_normal_attack_command_attack_2(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_attack_term_normal_attack_j_command_attack_2(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 9, lever_data);
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
        Jump_Attack_Term(wk, &(Jump_Term_Args) { -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40 });
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

void pattern_keep_away_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0x7F, 3);
        break;

    case 1:
        EM_Term(wk, p);
        break;

    case 2:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_keep_away_wait_get_up_com_random_select(PLW* wk, s16 target_pos, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, target_pos, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 2:
        Com_Random_Select(wk, p, 0);
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

void pattern_lever_attack_normal_attack_j_command_attack(
    PLW* wk, u16 lever_data, u16 lever_data_b, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, lever_data);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    case 2:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
