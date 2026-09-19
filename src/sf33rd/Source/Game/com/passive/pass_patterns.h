/*
 * Pattern skeletons shared by every character's passive scripts.
 * See pass_patterns_1step.c and its siblings.
 */

#ifndef PASS_PATTERNS_H
#define PASS_PATTERNS_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void pattern_adjust_attack(PLW* wk, u16 lever_data);
void pattern_approach_walk(PLW* wk, s16 target_pos);
void pattern_approach_walk_com_random_select(PLW* wk, s16 target_pos, const Branch_Menu_Args* p, s16 rnd_type);
void pattern_approach_walk_com_random_select_2(PLW* wk, const Branch_Menu_Args* p);
void pattern_approach_walk_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_approach_walk_command_attack_com_random_select(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_approach_walk_em_term(PLW* wk, s16 target_pos, const EM_Term_Params* p);
void pattern_approach_walk_em_term_com_random_select(PLW* wk, s16 target_pos, const Branch_Menu_Args* p);
void pattern_approach_walk_em_term_command_attack(
    PLW* wk, s16 target_pos, const EM_Term_Params* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_em_term_command_attack_2(
    PLW* wk, s16 target_pos, const Command_Attack_Args* p, const Branch_Menu_Args* p_b
);
void pattern_approach_walk_em_term_command_attack_3(
    PLW* wk, s16 target_pos, const Command_Attack_Args* p, const SA_Term_Args* p_b
);
void pattern_approach_walk_em_term_j_command_attack(
    PLW* wk, s16 target_pos, const EM_Term_Params* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_em_term_lever_attack(PLW* wk, s16 target_pos, const EM_Term_Params* p, u16 lever);
void pattern_approach_walk_em_term_normal_attack(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_em_term_normal_attack_2(PLW* wk, s16 target_pos, const EM_Term_Params* p, u16 lever_data);
void pattern_approach_walk_em_term_normal_attack_3(
    PLW* wk, s16 target_pos, const EM_Term_Params* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_em_term_normal_attack_4(
    PLW* wk, s16 target_pos, const Command_Attack_Args* p, const SA_Term_Args* p_b
);
void pattern_approach_walk_em_term_normal_attack_5(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_em_term_sa_term(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_em_term_sa_term_2(PLW* wk);
void pattern_approach_walk_etc_term_com_random_select(PLW* wk);
void pattern_approach_walk_etc_term_lever_attack(PLW* wk, s16 target_pos, u32 next_action, u16 next_menu);
void pattern_approach_walk_j_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_approach_walk_lever_attack(PLW* wk, s16 target_pos, u16 lever, u16 lever_data);
void pattern_approach_walk_normal_attack(PLW* wk, s16 target_pos, u16 lever_data);
void pattern_approach_walk_normal_attack_2(PLW* wk, s16 reaction, u16 lever_data);
void pattern_approach_walk_normal_attack_3(PLW* wk, s16 target_pos, s16 reaction, u16 lever_data);
void pattern_approach_walk_normal_attack_branch_unit_area(PLW* wk, s16 target_pos, const Branch_Menu_Args* p);
void pattern_approach_walk_normal_attack_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void pattern_approach_walk_normal_attack_j_command_attack(
    PLW* wk, s16 target_pos, u16 lever_data, const Command_Attack_Args* p
);
void pattern_approach_walk_normal_attack_j_command_attack_2(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_approach_walk_normal_attack_lever_attack(PLW* wk, s16 target_pos, u16 lever, u16 lever_data);
void pattern_approach_walk_normal_attack_sa_term(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_normal_attack_sa_term_2(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_normal_attack_sa_term_3(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_normal_attack_sa_term_4(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_sa_term(PLW* wk, s16 target_pos, const SA_Term_Args* p);
void pattern_approach_walk_sa_term_j_command_attack(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_wait_get_up_command_attack(
    PLW* wk, s16 target_pos, s16 option, const Command_Attack_Args* p
);
void pattern_approach_walk_wait_get_up_j_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_approach_walk_wait_get_up_lever_attack(PLW* wk, s16 target_pos, u16 lever, u16 lever_data);
void pattern_approach_walk_wait_get_up_normal_attack(PLW* wk, s16 target_pos, u16 lever_data);
void pattern_approach_walk_wait_get_up_normal_attack_2(PLW* wk);
void pattern_approach_walk_wait_get_up_sa_term(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_branch_unit_area(PLW* wk, const Branch_Menu_Args* p);
void pattern_check_ex_command_attack(PLW* wk, s16 next_menu, const Command_Attack_Args* p);
void pattern_check_ex_j_command_attack(PLW* wk, s16 next_menu, const Command_Attack_Args* p);
void pattern_check_ex_wait_j_command_attack(PLW* wk, s16 time, const Command_Attack_Args* p);
void pattern_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type);
void pattern_com_random_select_from_step_6(PLW* wk);
void pattern_command_attack(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b);
void pattern_command_attack_2(PLW* wk, const Command_Attack_Args* p);
void pattern_command_attack_com_random_select(PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b);
void pattern_command_attack_from_step_6(PLW* wk);
void pattern_command_attack_from_step_6_2(PLW* wk);
void pattern_command_attack_normal_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_em_term_adjust_attack(PLW* wk, const EM_Term_Params* p);
void pattern_em_term_branch_unit_area(PLW* wk, const EM_Term_Params* p, const Branch_Menu_Args* p_b);
void pattern_em_term_com_random_select(PLW* wk, const EM_Term_Params* p, const Branch_Menu_Args* p_b);
void pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void pattern_em_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void pattern_em_term_lever_attack(PLW* wk, const EM_Term_Params* p, u16 lever, u16 lever_data);
void pattern_em_term_lever_attack_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data);
void pattern_em_term_normal_attack_2(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void pattern_em_term_normal_attack_3(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void pattern_em_term_normal_attack_4(PLW* wk, const EM_Term_Params* p);
void pattern_em_term_normal_attack_5(PLW* wk, const EM_Term_Params* p);
void pattern_em_term_normal_attack_6(PLW* wk, const EM_Term_Params* p);
void pattern_em_term_normal_attack_7(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void pattern_em_term_normal_attack_com_random_select(PLW* wk, u16 lever_data);
void pattern_em_term_normal_attack_command_attack(
    PLW* wk, const EM_Term_Params* p, u16 lever_data, const Command_Attack_Args* p_b
);
void pattern_em_term_normal_attack_j_command_attack(
    PLW* wk, const EM_Term_Params* p, u16 lever_data, const Command_Attack_Args* p_b
);
void pattern_em_term_normal_attack_sa_term(PLW* wk);
void pattern_em_term_sa_term_command_attack(
    PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
);
void pattern_em_term_sa_term_em_term(
    PLW* wk, const EM_Term_Params* p, const EM_Term_Params* p_b, const Command_Attack_Args* p_b_b
);
void pattern_em_term_sa_term_j_command_attack(
    PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
);
void pattern_end_immediately(PLW* wk);
void pattern_etc_term_branch_unit_area(PLW* wk, u32 next_action, u16 next_menu, const Branch_Menu_Args* p);
void pattern_etc_term_normal_attack(PLW* wk);
void pattern_etc_term_normal_attack_lever_attack(PLW* wk, u16 next_menu, u16 lever, u16 lever_data);
void pattern_etc_term_provoke_next_another_menu(PLW* wk, u16 next_menu, u16 next_menu_b);
void pattern_forced_guard(PLW* wk, s16 guard_type);
void pattern_hi_jump(PLW* wk, s16 pl_number);
void pattern_hi_jump_attack_normal_attack(PLW* wk, const Jump_Attack_Args* a, u16 lever_data);
void pattern_hi_jump_attack_term(PLW* wk, const Hi_Jump_Term_Args* a);
void pattern_hi_jump_attack_term_normal_attack(PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data);
void pattern_hi_jump_attack_term_normal_attack_2(
    PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data, u16 lever_data_b
);
void pattern_hi_jump_attack_term_normal_attack_3(PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data);
void pattern_hi_jump_attack_term_normal_attack_4(
    PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data, u16 lever_data_b
);
void pattern_hi_jump_attack_term_normal_attack_lever_attack(PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data);
void pattern_hi_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a);
void pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_j_command_attack_from_step_6(PLW* wk);
void pattern_jump(PLW* wk, s16 time);
void pattern_jump_attack(PLW* wk, const Jump_Attack_Args* a);
void pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void pattern_jump_attack_term_command_attack(PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p);
void pattern_jump_attack_term_j_command_attack(PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p);
void pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a, s16 reaction, u16 lever_data);
void pattern_jump_attack_term_normal_attack_2(PLW* wk, const Jump_Term_Args* a);
void pattern_jump_attack_term_normal_attack_3(PLW* wk, const Jump_Term_Args* a, u16 lever_data, u16 lever_data_b);
void pattern_jump_attack_term_normal_attack_4(PLW* wk, const Jump_Term_Args* a, u16 lever_data, u16 lever_data_b);
void pattern_jump_attack_term_normal_attack_com_random_select(
    PLW* wk, const Jump_Term_Args* a, const Branch_Menu_Args* p, s16 rnd_type
);
void pattern_jump_attack_term_normal_attack_command_attack(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const Command_Attack_Args* p
);
void pattern_jump_attack_term_normal_attack_command_attack_2(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p
);
void pattern_jump_attack_term_normal_attack_command_attack_4(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p, s16 rnd_type
);
void pattern_jump_attack_term_normal_attack_j_command_attack(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const Command_Attack_Args* p
);
void pattern_jump_attack_term_normal_attack_j_command_attack_2(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const Command_Attack_Args* p
);
void pattern_jump_attack_term_normal_attack_sa_term(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_jump_attack_term_sa_term_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_jump_attack_term_sa_term_j_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_jump_attack_term_wait_get_up_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data);
void pattern_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a);
void pattern_jump_look(PLW* wk);
void pattern_keep_away(PLW* wk, s16 target_pos, s16 option);
void pattern_keep_away_com_random_select(PLW* wk, s16 target_pos);
void pattern_keep_away_com_random_select_2(PLW* wk, s16 target_pos, s16 option, const Branch_Menu_Args* p);
void pattern_keep_away_wait_get_up(PLW* wk, s16 target_pos, u16 lever_data, s16 option);
void pattern_keep_away_wait_get_up_2(PLW* wk, u16 lever_data);
void pattern_keep_away_wait_get_up_command_attack(PLW* wk, s16 target_pos, s16 option, const Command_Attack_Args* p);
void pattern_keep_status(PLW* wk, s16 option_data);
void pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data);
void pattern_lever_attack_normal_attack(PLW* wk);
void pattern_lever_off_look(PLW* wk);
void pattern_lever_on_em_term_check_store_lever(PLW* wk, u16 tech_number);
void pattern_lever_on_em_term_check_store_lever_2(PLW* wk, const Command_Attack_Args* p);
void pattern_lever_on_em_term_j_command_attack(
    PLW* wk, u16 lr_lever, const EM_Term_Params* p, const Command_Attack_Args* p_b
);
void pattern_look(PLW* wk);
void pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void pattern_normal_attack_2(PLW* wk, s16 reaction, u16 lever_data, u16 lever_data_b);
void pattern_normal_attack_3(PLW* wk, u16 lever_data, s16 reaction, u16 lever_data_b);
void pattern_normal_attack_4(PLW* wk, s16 reaction, s16 reaction_b, u16 lever_data);
void pattern_normal_attack_5(PLW* wk, s16 reaction, s16 reaction_b, u16 lever_data);
void pattern_normal_attack_7(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b);
void pattern_normal_attack_branch_unit_area(PLW* wk, const Branch_Menu_Args* p);
void pattern_normal_attack_com_random_select(
    PLW* wk, u16 lever_data, const Branch_Menu_Args* p, s16 rnd_type
);
void pattern_normal_attack_com_random_select_2(PLW* wk, s16 reaction, u16 lever_data);
void pattern_normal_attack_com_random_select_3(PLW* wk, u16 lever_data);
void pattern_normal_attack_command_attack(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p);
void pattern_normal_attack_command_attack_2(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void pattern_normal_attack_command_attack_3(PLW* wk, s16 reaction, s16 reaction_b, const Command_Attack_Args* p);
void pattern_normal_attack_command_attack_4(PLW* wk, s16 reaction, s16 reaction_b, const Command_Attack_Args* p);
void pattern_normal_attack_command_attack_5(PLW* wk, const Command_Attack_Args* p);
void pattern_normal_attack_command_attack_6(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p);
void pattern_normal_attack_from_step_6(PLW* wk);
void pattern_normal_attack_j_command_attack(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p);
void pattern_normal_attack_j_command_attack_2(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void pattern_normal_attack_j_command_attack_3(PLW* wk, u16 lever_data, u16 lever_data_b, const Command_Attack_Args* p);
void pattern_normal_attack_j_command_attack_4(PLW* wk, u16 lever_data, const Command_Attack_Args* p);
void pattern_normal_attack_j_command_attack_5(PLW* wk, const Command_Attack_Args* p);
void pattern_normal_attack_j_command_attack_com_random_select(PLW* wk);
void pattern_normal_attack_j_command_attack_com_random_select_2(PLW* wk);
void pattern_normal_attack_jump_attack_term_normal_attack(PLW* wk, const SA_Term_Args* p);
void pattern_normal_attack_jump_attack_term_sa_term(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_normal_attack_jump_attack_term_sa_term_2(
    PLW* wk, s16 reaction, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_normal_attack_sa_term_branch_unit_area(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b);
void pattern_normal_attack_sa_term_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_normal_attack_sa_term_command_attack_2(
    PLW* wk, s16 reaction, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_normal_attack_sa_term_j_command_attack(
    PLW* wk, u16 lever_data, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_normal_attack_sa_term_normal_attack(PLW* wk, const SA_Term_Args* p);
void pattern_normal_attack_sa_term_normal_attack_2(PLW* wk, const SA_Term_Args* p, u16 lever_data);
void pattern_pierce_on_command_attack(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b);
void pattern_pierce_on_command_attack_2(
    PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b, const Command_Attack_Args* p_b_b
);
void pattern_pierce_on_command_attack_approach_walk(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_pierce_on_command_attack_branch_unit_area(
    PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b
);
void pattern_pierce_on_command_attack_em_term(PLW* wk);
void pattern_pierce_on_command_attack_j_command_attack(
    PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b
);
void pattern_pierce_on_command_attack_jump_attack_term(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_pierce_on_command_attack_jump_attack_term_2(PLW* wk, const SA_Term_Args* p);
void pattern_pierce_on_command_attack_normal_attack(PLW* wk, const SA_Term_Args* p);
void pattern_pierce_on_command_attack_normal_attack_2(PLW* wk, s16 reaction, u16 lever_data);
void pattern_pierce_on_command_attack_normal_attack_3(PLW* wk);
void pattern_pierce_on_command_attack_normal_attack_4(PLW* wk);
void pattern_pierce_on_command_attack_wait(PLW* wk);
void pattern_pierce_on_em_term_j_command_attack(
    PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b, const Command_Attack_Args* p_b_b
);
void pattern_pierce_on_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void pattern_pierce_on_em_term_normal_attack_2(PLW* wk);
void pattern_pierce_on_j_command_attack(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b);
void pattern_pierce_on_j_command_attack_normal_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_pierce_on_jump_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type);
void pattern_provoke(PLW* wk, s16 lever);
void pattern_provoke_com_random_select(PLW* wk, const Branch_Menu_Args* p);
void pattern_provoke_sa_term_com_random_select(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b);
void pattern_provoke_wait_get_up(PLW* wk);
void pattern_sa_term(PLW* wk, const SA_Term_Args* p);
void pattern_sa_term_approach_walk_sa_term(
    PLW* wk, const SA_Term_Args* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
);
void pattern_sa_term_approach_walk_sa_term_2(PLW* wk);
void pattern_sa_term_branch_unit_area(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b);
void pattern_sa_term_com_random_select(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b, s16 rnd_type);
void pattern_sa_term_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_sa_term_command_attack_2(
    PLW* wk, const SA_Term_Args* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
);
void pattern_sa_term_em_term_sa_term(PLW* wk);
void pattern_sa_term_from_step_6(PLW* wk);
void pattern_sa_term_j_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_sa_term_normal_attack(PLW* wk, const SA_Term_Args* p, s16 reaction, u16 lever_data);
void pattern_search_back_term_command_attack(PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p);
void pattern_search_back_term_command_attack_2(PLW* wk, const Command_Attack_Args* p);
void pattern_search_back_term_command_attack_from_step_6(PLW* wk);
void pattern_search_back_term_command_attack_sa_term(
    PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p
);
void pattern_search_back_term_command_attack_sa_term_2(
    PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p
);
void pattern_search_back_term_pierce_on_command_attack(
    PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p
);
void pattern_search_back_term_pierce_on_command_attack_2(
    PLW* wk, s16 move_value, s16 next_menu, const Branch_Menu_Args* p
);
void pattern_search_back_term_pierce_on_keep_away(PLW* wk, s16 next_menu, s16 target_pos, const Branch_Menu_Args* p);
void pattern_search_back_term_pierce_on_keep_away_2(PLW* wk, s16 target_pos, const Branch_Menu_Args* p);
void pattern_search_back_term_pierce_on_keep_away_3(PLW* wk, s16 target_pos);
void pattern_search_back_term_pierce_on_keep_away_4(PLW* wk, s16 move_value, s16 next_menu, s16 target_pos);
void pattern_search_back_term_pierce_on_keep_away_5(PLW* wk, s16 target_pos);
void pattern_search_back_term_walk_normal_attack(PLW* wk, s16 next_menu, u16 lever_data);
void pattern_search_back_term_walk_wait(PLW* wk);
void pattern_search_back_term_walk_wait_2(PLW* wk);
void pattern_search_back_term_walk_wait_get_up(PLW* wk, s16 move_value, s16 next_menu, s16 time);
void pattern_search_back_term_walk_wait_get_up_2(PLW* wk, s16 move_value, s16 next_menu, s16 time);
void pattern_short_range_attack(PLW* wk);
void pattern_vs_jump_guard(PLW* wk);
void pattern_wait(PLW* wk, s16 time);
void pattern_wait_attack_complete(PLW* wk, s16 option);
void pattern_wait_attack_complete_normal_attack(PLW* wk);
void pattern_wait_attack_complete_sa_term_wait_attack_complete(PLW* wk, const SA_Term_Args* p);
void pattern_wait_branch_unit_area(PLW* wk, s16 time, const Branch_Menu_Args* p);
void pattern_wait_command_attack(PLW* wk, s16 time, const Command_Attack_Args* p);
void pattern_wait_command_attack_wait_from_step_6(PLW* wk);
void pattern_wait_get_up(PLW* wk, u16 lever_data, s16 option);
void pattern_wait_get_up_2(PLW* wk, u16 lever_data);
void pattern_wait_get_up_branch_unit_area(PLW* wk, u16 lever_data, const Branch_Menu_Args* p);
void pattern_wait_get_up_com_random_select(PLW* wk, u16 lever_data, s16 option, const Branch_Menu_Args* p);
void pattern_wait_get_up_command_attack(PLW* wk, u16 lever_data, s16 option, const Command_Attack_Args* p);
void pattern_wait_get_up_command_attack_2(PLW* wk, const Command_Attack_Args* p);
void pattern_wait_get_up_command_attack_em_term(PLW* wk);
void pattern_wait_get_up_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void pattern_wait_get_up_jump_attack_term_j_command_attack(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p
);
void pattern_wait_get_up_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data);
void pattern_wait_get_up_jump_attack_term_normal_attack_2(
    PLW* wk, const Jump_Term_Args* a, s16 reaction, u16 lever_data
);
void pattern_wait_get_up_lever_attack(PLW* wk, u16 lever_data, u16 lever, u16 lever_data_b);
void pattern_wait_get_up_lever_attack_2(PLW* wk, u16 lever, u16 lever_data);
void pattern_wait_get_up_normal_attack(PLW* wk, u16 lever_data, u16 lever_data_b);
void pattern_wait_get_up_normal_attack_2(PLW* wk, u16 lever_data);
void pattern_wait_get_up_normal_attack_j_command_attack(PLW* wk, u16 lever_data, const Command_Attack_Args* p);
void pattern_wait_get_up_normal_attack_sa_term(
    PLW* wk, s16 option, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_wait_get_up_normal_attack_sa_term_2(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_wait_get_up_normal_attack_sa_term_3(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_wait_get_up_normal_attack_sa_term_4(
    PLW* wk, s16 option, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_wait_get_up_pierce_on_command_attack(PLW* wk, const Command_Attack_Args* p, const EM_Term_Params* p_b);
void pattern_wait_get_up_sa_term_j_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_wait_get_up_search_back_term_jump_attack_term(
    PLW* wk, s16 next_menu, const Jump_Term_Args* a, const Command_Attack_Args* p
);
void pattern_wait_j_command_attack(PLW* wk, s16 time, const Command_Attack_Args* p);
void pattern_wait_j_command_attack_from_step_6(PLW* wk);
void pattern_wait_lie_jump(PLW* wk);
void pattern_walk(PLW* wk, s16 unused);
void pattern_walk_com_random_select(PLW* wk, s16 time, const Branch_Menu_Args* p, s16 rnd_type);
void pattern_walk_com_random_select_2(PLW* wk);
void pattern_walk_search_back_term_walk(PLW* wk);

#endif
