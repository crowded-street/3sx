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

#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

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
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Com_Random_Select(wk, p, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
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

void active_pattern_approach_walk_lever_attack(PLW* wk, s16 target_pos, u16 lever) {
    pattern_approach_walk_lever_attack(wk, target_pos, lever, 0x110);
}

void active_pattern_approach_walk_normal_attack(PLW* wk, s16 target_pos, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
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

void active_pattern_command_attack_branch_unit_area(PLW* wk, const Command_Attack_Args* p) {
    pattern_command_attack_branch_unit_area(wk, p, &(Branch_Menu_Args){2, 0x31, 0x32, 0x33, 1});
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
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1});
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
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Adjust_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
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
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 5, next_action, next_menu);
        break;

    case 1:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_hi_jump_attack_term_com_random_select(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x20, 0, -0x7FA0, 8, 0x200});
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 0x18, 0x18, 0x11, 0x11}, 0);
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

void active_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data) {
    pattern_jump_attack_term_normal_attack(wk, a, 8, lever_data);
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

void active_pattern_lever_attack_command_attack(PLW* wk, const Command_Attack_Args* p) {
    pattern_lever_attack_command_attack(wk, 0, p);
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

void active_pattern_normal_attack_2(PLW* wk, u16 lever_data, u16 lever_data_b) {
    pattern_normal_attack_2(wk, 0xB, lever_data, lever_data_b);
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

void active_pattern_normal_attack_4(PLW* wk, u16 lever_data, u16 lever_data_b) {
    pattern_normal_attack_2(wk, 8, lever_data, lever_data_b);
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

void active_pattern_sa_term_branch_unit_area(PLW* wk, const SA_Term_Args* p) {
    pattern_sa_term_branch_unit_area(wk, p, &(Branch_Menu_Args){2, 0x41, 0x41, 0x42, 0x43});
}

void active_pattern_sa_term_com_random_select(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b) {
    pattern_sa_term_com_random_select(wk, p, p_b, 1);
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
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7FB0, 8, 8, 0x8400, 0, -0x7FA0, 8, 0x20});
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

void active_pattern_walk_com_random_select(PLW* wk, const Branch_Menu_Args* p) {
    pattern_walk_com_random_select(wk, 0x20, p, 3);
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

void pattern_pierce_on_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, p);
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

void pattern_wait_com_random_select(PLW* wk, s16 time, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, time);
        break;

    case 1:
        Com_Random_Select(wk, p, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_etc_term_provoke(PLW* wk, s16 exit_no, u16 next_menu) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, exit_no, 6, next_menu);
        break;

    case 1:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_jump_attack_term(PLW* wk, const EM_Term_Params* p, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Jump_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_short_range_attack(PLW* wk, const Short_Range_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, 0, 0, 2, 0});
        break;

    case 1:
        Short_Range_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_branch_unit_area_2(PLW* wk, s16 reaction, const Branch_Menu_Args* p) {
    active_pattern_normal_attack_branch_unit_area(wk, reaction, 0x102, p);
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

void pattern_walk_branch_unit_area(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x30, 0);
        break;

    case 1:
        Branch_Unit_Area(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term_forced_guard(PLW* wk, const SA_Term_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, p);
        break;

    case 1:
        Forced_Guard(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term_next_another_menu(PLW* wk, const SA_Term_Args* p, u16 next_menu) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, p);
        break;

    case 1:
        Next_Another_Menu(wk, 6, next_menu);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_jump(PLW* wk, s16 move_value) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, 1, -1);
        break;

    case 1:
        Jump(wk, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_attack_term_com_random_select(PLW* wk, const Jump_Term_Args* a, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Com_Random_Select(wk, p, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_j_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_lie_approach_walk(PLW* wk, s16 target_pos) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Lie(wk, 0);
        break;

    case 1:
        Approach_Walk(wk, target_pos, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_lever_attack_command_attack(PLW* wk, u16 lever, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, lever, 0x200);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_jump_command_attack_term(PLW* wk, const EM_Term_Params* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, 0x40, 1, -1, 0x30, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term_2(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0xFFFF, 0x47});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x19, 0x3C});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_lever_attack_j_command_attack(PLW* wk, u16 lever_data, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, lever_data);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_turn_over_on_jump_attack_term(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F90, -0x7FA8, 8, 0x42, 0, -0x7F68, -1, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_hi_jump_attack_term_command_attack(PLW* wk, const Hi_Jump_Term_Args* a, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, a);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_lever_attack_sp_com_random_select(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack_SP(wk, &(Lever_Attack_SP_Args){8, 0, 0x400, 0x12});
        break;

    case 1:
        Com_Random_Select(wk, p, 4);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_6(PLW* wk, s16 reaction, u16 lever_data, s16 reaction_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 1:
        Normal_Attack(wk, reaction_b, 0x102);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
