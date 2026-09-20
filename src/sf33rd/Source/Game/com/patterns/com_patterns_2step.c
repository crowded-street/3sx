/**
 * @file com_patterns_2step.c
 * COM: pattern skeletons shared by every character
 *
 * The two-step patterns. Part 1 of 3, in name order.
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

void pattern_approach_walk_normal_attack_4(PLW* wk, s16 target_pos, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_adjust_attack_2(PLW* wk, const EM_Term_Params* p, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Adjust_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_etc_term_provoke_2(PLW* wk, s16 exit_no, u32 next_action, u16 next_menu) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, exit_no, next_action, next_menu);
        break;

    case 1:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_adjust_attack_3(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 9, 0x100);
        break;

    case 1:
        Adjust_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_adjust_attack_4(PLW* wk, s16 reaction, s16 reaction_b, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, reaction, 0x10);
        break;

    case 1:
        Adjust_Attack(wk, reaction_b, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_adjust_attack_branch_unit_area(PLW* wk, u16 lever_data, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 8, lever_data);
        break;

    case 1:
        Branch_Unit_Area(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_adjust_attack_command_attack(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, reaction, lever_data);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_adjust_attack_j_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, reaction, lever_data);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_adjust_attack_lever_attack_2(PLW* wk, s16 reaction, u16 lever_data, u16 lever) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, reaction, lever_data);
        break;

    case 1:
        Lever_Attack(wk, 8, lever, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_adjust_attack_normal_attack(PLW* wk, s16 reaction, u16 lever_data, u16 lever_data_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, reaction, lever_data);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_approach_walk_com_random_select(PLW* wk, s16 target_pos, const Branch_Menu_Args* p) {
    pattern_approach_walk_com_random_select(wk, target_pos, p, 1);
}

void active_pattern_approach_walk_com_random_select_2(PLW* wk, s16 target_pos, s16 option, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, option);
        break;

    case 1:
        Com_Random_Select(wk, p, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_approach_walk_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_approach_walk_normal_attack(PLW* wk, s16 target_pos, u16 lever_data) {
    pattern_approach_walk_normal_attack_4(wk, target_pos, 8, lever_data);
}

void active_pattern_check_sa_command_attack(PLW* wk, s16 next_action, s16 next_menu, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA(wk, next_action, next_menu);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_command_attack_2(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_command_attack_com_random_select(
    PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b, s16 rnd_type
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        Com_Random_Select(wk, p_b, rnd_type);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_command_attack_j_command_attack(
    PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_command_attack_jump_attack(PLW* wk, const Command_Attack_Args* p, const Jump_Attack_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        Jump_Attack(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_command_attack_lever_attack(PLW* wk, const Command_Attack_Args* p, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        Lever_Attack(wk, 8, lever, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_command_attack_look(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0, 0xB, -1 });
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_command_attack_normal_attack(PLW* wk, const Command_Attack_Args* p, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_command_attack_push_shot(PLW* wk, const Command_Attack_Args* p, s16 power_level) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        Push_Shot(wk, power_level);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_em_term_adjust_attack(PLW* wk, const EM_Term_Params* p) {
    pattern_em_term_adjust_attack_2(wk, p, 0x10);
}

void active_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_em_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_em_term_lever_attack(PLW* wk, const EM_Term_Params* p, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Lever_Attack(wk, 8, lever, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_etc_term_command_attack(PLW* wk, u16 next_menu, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 0, 6, next_menu);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_etc_term_j_command_attack(PLW* wk, u16 next_menu, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 3, 2, next_menu);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_etc_term_provoke(PLW* wk, u32 next_action, u16 next_menu) {
    pattern_etc_term_provoke_2(wk, 5, next_action, next_menu);
}

void active_pattern_hi_jump_attack_term_com_random_select(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args) { -0x7FA0, -0x7FC0, 8, 0x20, 0, -0x7FA0, 8, 0x200 });
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 2, 0x18, 0x18, 0x11, 0x11 }, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_j_command_attack_2(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, p);
        break;

    case 1:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_jump_attack_term_command_attack(PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_jump_attack_term_j_command_attack(PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_jump_look(PLW* wk, s16 time) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, time);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_keep_away_com_random_select(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xBF, 1);
        break;

    case 1:
        Com_Random_Select(wk, p, 3);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_lever_off_look(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Off(wk);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_lever_on_look(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_3(PLW* wk, u16 lever_data, s16 reaction, u16 lever_data_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, lever_data);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_branch_unit_area(PLW* wk, s16 reaction, u16 lever_data, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 1:
        Branch_Unit_Area(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_command_attack(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_j_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_lever_attack(PLW* wk, s16 reaction, u16 lever_data, u16 lever) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 1:
        Lever_Attack(wk, 8, lever, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_sa_term_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, p);
        break;

    case 1:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_sa_term_hi_jump_attack_term(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args) { 0xFFFF, 0xFFFF, 0x30, 0 });
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args) { -0x7FB0, 8, 8, 0x8400, 0, -0x7FA0, 8, 0x20 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_search_back_term_walk(PLW* wk, s16 move_value, s16 next_action, s16 next_menu) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, next_action, next_menu);
        break;

    case 1:
        Walk(wk, 1, 0x30, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_search_back_term_walk_2(PLW* wk, s16 move_value, s16 next_action, s16 next_menu) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, next_action, next_menu);
        break;

    case 1:
        Walk(wk, 1, 0x60, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_com_random_select(PLW* wk, s16 target_pos, const Branch_Menu_Args* p, s16 rnd_type) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Com_Random_Select(wk, p, rnd_type);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term(PLW* wk, s16 target_pos, const EM_Term_Params* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_j_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_lever_attack(PLW* wk, s16 target_pos, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, lever, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_look(PLW* wk, s16 option) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, option);
        break;

    case 1:
        Look(wk, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_normal_attack_2(PLW* wk, s16 reaction, u16 lever_data) {
    pattern_approach_walk_normal_attack_4(wk, 0x47, reaction, lever_data);
}

void pattern_approach_walk_sa_term(PLW* wk, s16 target_pos, const SA_Term_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        SA_Term(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_check_ex_command_attack(PLW* wk, s16 next_menu, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, next_menu);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_check_ex_j_command_attack(PLW* wk, s16 next_menu, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, next_menu);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_command_attack_branch_unit_area(PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        Branch_Unit_Area(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_jump_attack_term(
    PLW* wk, const Search_Back_Term_Step* search_back_term, const Jump_Term_Args* a
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, search_back_term->Move_Value, search_back_term->Next_Action, search_back_term->Next_Menu);
        break;

    case 1:
        Jump_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_check_store_lever_command_attack(
    PLW* wk, const Check_Store_Lever_Step* check_store_lever, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_Store_Lever(
            wk, check_store_lever->Tech_Number, check_store_lever->Next_Action, check_store_lever->Next_Menu
        );
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_shell_term_jump(PLW* wk, const Shell_Term_Params* p, s16 time) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, p);
        break;

    case 1:
        Jump(wk, time);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_shell_term_jump_attack_term(PLW* wk, const Shell_Term_Params* p, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, p);
        break;

    case 1:
        Jump_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_shell_term_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, &(Shell_Term_Params) { 3, 2, 1, -1, -1 });
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
