/*
 * Pattern skeletons shared by every character's COM scripts, active and passive.
 * See com_patterns_1step.c and its siblings.
 *
 * The two script folders are one shape under two names - Game/com/passive
 * spells a script Passive14_0122 and Game/com/active spells it Pattern14_0122 -
 * and their skeletons were the same idiom held twice. They live here once.
 */

#ifndef COM_PATTERNS_H
#define COM_PATTERNS_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/com/patterns/com_pattern_args.h"
#include "structs.h"
#include "types.h"

void active_pattern_(PLW* wk);
void active_pattern_adjust_attack(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b);
void active_pattern_adjust_attack_2(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b);
void active_pattern_adjust_attack_3(PLW* wk, u16 lever_data);
void active_pattern_adjust_attack_4(PLW* wk, s16 reaction, s16 reaction_b, u16 lever_data);
void active_pattern_adjust_attack_branch_unit_area(PLW* wk, u16 lever_data, const Branch_Menu_Args* p);
void active_pattern_adjust_attack_command_attack(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p);
void active_pattern_adjust_attack_command_attack_2(PLW* wk, const Command_Attack_Args* p);
void active_pattern_adjust_attack_j_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void active_pattern_adjust_attack_lever_attack(PLW* wk);
void active_pattern_adjust_attack_lever_attack_2(PLW* wk, s16 reaction, u16 lever_data, u16 lever);
void active_pattern_adjust_attack_normal_attack(PLW* wk, s16 reaction, u16 lever_data, u16 lever_data_b);
void active_pattern_adjust_attack_normal_attack_branch_unit_area(PLW* wk);
void active_pattern_adjust_attack_normal_attack_command_attack(
    PLW* wk, u16 lever_data, s16 reaction, u16 lever_data_b
);
void active_pattern_adjust_attack_normal_attack_lever_attack(PLW* wk);
void active_pattern_approach_walk(PLW* wk, s16 target_pos);
void active_pattern_approach_walk_com_random_select(PLW* wk, s16 target_pos, const Branch_Menu_Args* p);
void active_pattern_approach_walk_com_random_select_2(PLW* wk, s16 target_pos, s16 option, const Branch_Menu_Args* p);
void active_pattern_approach_walk_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void active_pattern_approach_walk_jump_attack_term_normal_attack(PLW* wk, s16 option);
void active_pattern_approach_walk_jump_attack_term_normal_attack_2(PLW* wk, s16 option);
void active_pattern_approach_walk_jump_attack_term_normal_attack_3(PLW* wk, const SA_Term_Args* p);
void active_pattern_approach_walk_jump_command_attack_term_com_random_select(PLW* wk, const Branch_Menu_Args* p);
void active_pattern_approach_walk_lever_attack(PLW* wk, s16 target_pos, u16 lever);
void active_pattern_approach_walk_look_com_random_select(PLW* wk, s16 target_pos);
void active_pattern_approach_walk_normal_attack(PLW* wk, s16 target_pos, u16 lever_data);
void active_pattern_approach_walk_normal_attack_j_command_attack(
    PLW* wk, s16 target_pos, s16 reaction, u16 lever_data
);
void active_pattern_approach_walk_normal_attack_sa_term(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void active_pattern_approach_walk_pierce_on_command_attack(PLW* wk);
void active_pattern_approach_walk_sa_term_j_command_attack(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void active_pattern_approach_walk_turn_over_on_jump_attack_term(PLW* wk, const Branch_Menu_Args* p);
void active_pattern_branch_unit_area(PLW* wk, const Branch_Menu_Args* p);
void active_pattern_check_sa_command_attack(PLW* wk, s16 next_action, s16 next_menu, const Command_Attack_Args* p);
void active_pattern_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type);
void active_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void active_pattern_command_attack_2(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b);
void active_pattern_command_attack_branch_unit_area(PLW* wk, const Command_Attack_Args* p);
void active_pattern_command_attack_check_sa_command_attack(PLW* wk, const Command_Attack_Args* p);
void active_pattern_command_attack_com_random_select(
    PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b, s16 rnd_type
);
void active_pattern_command_attack_j_command_attack(
    PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b
);
void active_pattern_command_attack_jump_attack(PLW* wk, const Command_Attack_Args* p, const Jump_Attack_Args* a);
void active_pattern_command_attack_lever_attack(PLW* wk, const Command_Attack_Args* p, u16 lever, u16 lever_data);
void active_pattern_command_attack_lever_off_look(PLW* wk);
void active_pattern_command_attack_look(PLW* wk);
void active_pattern_command_attack_normal_attack(PLW* wk, const Command_Attack_Args* p, s16 reaction, u16 lever_data);
void active_pattern_command_attack_push_shot(PLW* wk, const Command_Attack_Args* p, s16 power_level);
void active_pattern_command_attack_sa_term_em_term(PLW* wk, const Command_Attack_Args* p);
void active_pattern_command_attack_turn_over_on_jump_attack_term(PLW* wk, const Branch_Menu_Args* p);
void active_pattern_em_term_adjust_attack(PLW* wk, const EM_Term_Params* p);
void active_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void active_pattern_em_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void active_pattern_em_term_lever_attack(PLW* wk, const EM_Term_Params* p, u16 lever, u16 lever_data);
void active_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void active_pattern_etc_term_command_attack(PLW* wk, u16 next_menu, const Command_Attack_Args* p);
void active_pattern_etc_term_j_command_attack(PLW* wk, u16 next_menu, const Command_Attack_Args* p);
void active_pattern_etc_term_provoke(PLW* wk, u32 next_action, u16 next_menu);
void active_pattern_hi_jump_attack(PLW* wk);
void active_pattern_hi_jump_attack_term(PLW* wk, const Hi_Jump_Term_Args* a);
void active_pattern_hi_jump_attack_term_com_random_select(PLW* wk);
void active_pattern_hi_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a);
void active_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void active_pattern_j_command_attack_2(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b);
void active_pattern_j_command_attack_from_step_6(PLW* wk);
void active_pattern_j_command_attack_from_step_6_2(PLW* wk);
void active_pattern_jump(PLW* wk);
void active_pattern_jump_attack(PLW* wk, const Jump_Attack_Args* a);
void active_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void active_pattern_jump_attack_term_command_attack(PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p);
void active_pattern_jump_attack_term_j_command_attack(PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p);
void active_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data);
void active_pattern_jump_attack_term_normal_attack_command_attack(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const SA_Term_Args* p
);
void active_pattern_jump_attack_term_normal_attack_command_attack_2(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const Command_Attack_Args* p
);
void active_pattern_jump_attack_term_normal_attack_command_attack_3(
    PLW* wk, const Command_Attack_Args* p, s16 time, const SA_Term_Args* p_b
);
void active_pattern_jump_attack_term_normal_attack_j_command_attack(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const Command_Attack_Args* p
);
void active_pattern_jump_attack_term_normal_attack_j_command_attack_2(PLW* wk, const Command_Attack_Args* p);
void active_pattern_jump_attack_term_sa_term_command_attack(
    PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void active_pattern_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a);
void active_pattern_jump_lever_off_look(PLW* wk);
void active_pattern_jump_look(PLW* wk, s16 time);
void active_pattern_keep_away(PLW* wk, s16 target_pos, s16 option);
void active_pattern_keep_away_com_random_select(PLW* wk, const Branch_Menu_Args* p);
void active_pattern_lever_attack(PLW* wk, s16 reaction, u16 lever, u16 lever_data);
void active_pattern_lever_attack_command_attack(PLW* wk, const Command_Attack_Args* p);
void active_pattern_lever_off_look(PLW* wk);
void active_pattern_lever_on_look(PLW* wk);
void active_pattern_lever_on_normal_attack_command_attack(
    PLW* wk, u16 lever_data, u16 lever_data_b, const Command_Attack_Args* p
);
void active_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void active_pattern_normal_attack_2(PLW* wk, u16 lever_data, u16 lever_data_b);
void active_pattern_normal_attack_3(PLW* wk, u16 lever_data, s16 reaction, u16 lever_data_b);
void active_pattern_normal_attack_4(PLW* wk, u16 lever_data, u16 lever_data_b);
void active_pattern_normal_attack_5(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b);
void active_pattern_normal_attack_6(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b);
void active_pattern_normal_attack_7(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b);
void active_pattern_normal_attack_8(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b);
void active_pattern_normal_attack_branch_unit_area(PLW* wk, s16 reaction, u16 lever_data, const Branch_Menu_Args* p);
void active_pattern_normal_attack_command_attack(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p);
void active_pattern_normal_attack_command_attack_2(
    PLW* wk, u16 lever_data, u16 lever_data_b, const Command_Attack_Args* p
);
void active_pattern_normal_attack_command_attack_3(PLW* wk, u16 lever_data, const Command_Attack_Args* p);
void active_pattern_normal_attack_j_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void active_pattern_normal_attack_j_command_attack_2(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void active_pattern_normal_attack_lever_attack(PLW* wk, s16 reaction, u16 lever_data, u16 lever);
void active_pattern_normal_attack_lever_off_look(PLW* wk, s16 reaction);
void active_pattern_normal_attack_pierce_on_j_command_attack(PLW* wk, u16 lever_data);
void active_pattern_normal_attack_pierce_on_j_command_attack_2(PLW* wk, s16 reaction, u16 lever_data);
void active_pattern_normal_attack_sa_term_approach_walk(PLW* wk, u16 lever_data, u16 lever);
void active_pattern_normal_attack_sp(PLW* wk);
void active_pattern_oro_ja_term(PLW* wk, const ORO_Air_Term_Args* a);
void active_pattern_oro_jca_term(PLW* wk, const ORO_JCA_Term_Args* a);
void active_pattern_pierce_on_approach_walk_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a);
void active_pattern_pierce_on_command_attack(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b);
void active_pattern_pierce_on_command_attack_2(
    PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b, const Command_Attack_Args* p_b_b
);
void active_pattern_pierce_on_command_attack_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void active_pattern_pierce_on_command_attack_wait(
    PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b, const Command_Attack_Args* p_b_b
);
void active_pattern_pierce_on_j_command_attack_normal_attack(PLW* wk, const Command_Attack_Args* p);
void active_pattern_pierce_on_jump_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type);
void active_pattern_pierce_on_search_back_term_command_attack(PLW* wk);
void active_pattern_provoke(PLW* wk);
void active_pattern_rapid_command_attack(PLW* wk, const Rapid_Command_Args* p);
void active_pattern_sa_term(PLW* wk, const SA_Term_Args* p);
void active_pattern_sa_term_branch_unit_area(PLW* wk, const SA_Term_Args* p);
void active_pattern_sa_term_com_random_select(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b);
void active_pattern_sa_term_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void active_pattern_sa_term_command_attack_2(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void active_pattern_sa_term_hi_jump_attack_term(PLW* wk);
void active_pattern_search_back_term_command_attack_sa_term(
    PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p
);
void active_pattern_search_back_term_hi_jump_attack_term_pierce_on(PLW* wk);
void active_pattern_search_back_term_jump_attack_term_pierce_on(PLW* wk);
void active_pattern_search_back_term_jump_look(PLW* wk, s16 move_value, s16 next_action, s16 next_menu);
void active_pattern_search_back_term_pierce_on_command_attack(PLW* wk, s16 move_value, s16 next_action, s16 next_menu);
void active_pattern_search_back_term_pierce_on_command_attack_2(PLW* wk);
void active_pattern_search_back_term_pierce_on_command_attack_3(PLW* wk);
void active_pattern_search_back_term_pierce_on_command_attack_4(
    PLW* wk, s16 move_value, s16 next_action, s16 next_menu
);
void active_pattern_search_back_term_walk(PLW* wk, s16 move_value, s16 next_action, s16 next_menu);
void active_pattern_search_back_term_walk_2(PLW* wk, s16 move_value, s16 next_action, s16 next_menu);
void active_pattern_search_back_term_walk_wait(PLW* wk);
void active_pattern_search_back_term_walk_wait_2(PLW* wk);
void active_pattern_turn_over_on_jump_attack_term_com_random_select(PLW* wk, const Branch_Menu_Args* p);
void active_pattern_wait(PLW* wk, s16 time);
void active_pattern_walk(PLW* wk, u16 lever, s16 time, s16 unused);
void active_pattern_walk_2(PLW* wk, s16 time, s16 time_b, s16 time_b_b);
void active_pattern_walk_com_random_select(PLW* wk, const Branch_Menu_Args* p);
void active_pattern_walk_search_back_term_walk(PLW* wk);
void pattern_adjust_attack(PLW* wk, u16 lever_data);
void pattern_adjust_attack_lever_attack(
    PLW* wk, s16 reaction, const Adjust_Attack_Step* adjust_attack, u16 lever_data
);
void pattern_approach_walk_com_random_select(PLW* wk, s16 target_pos, const Branch_Menu_Args* p, s16 rnd_type);
void pattern_approach_walk_com_random_select_2(PLW* wk, const Branch_Menu_Args* p);
void pattern_approach_walk_command_attack(
    PLW* wk, s16 target_pos, const Command_Attack_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_command_attack_com_random_select(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_approach_walk_command_attack_com_random_select_2(
    PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b
);
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
void pattern_approach_walk_em_term_command_attack_4(PLW* wk, const EM_Term_Params* p);
void pattern_approach_walk_em_term_j_command_attack(
    PLW* wk, s16 target_pos, const EM_Term_Params* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_em_term_j_command_attack_2(PLW* wk, const Command_Attack_Args* p, const SA_Term_Args* p_b);
void pattern_approach_walk_em_term_lever_attack(PLW* wk, s16 target_pos, const EM_Term_Params* p, u16 lever);
void pattern_approach_walk_em_term_lever_attack_2(PLW* wk, u16 lever);
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
void pattern_approach_walk_em_term_normal_attack_6(
    PLW* wk, const EM_Term_Params* p, u16 lever_data, const Command_Attack_Args* p_b
);
void pattern_approach_walk_em_term_normal_attack_7(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void pattern_approach_walk_em_term_normal_attack_8(PLW* wk, const EM_Term_Params* p, u16 lever_data, s16 time);
void pattern_approach_walk_em_term_normal_attack_9(
    PLW* wk, s16 target_pos, const EM_Term_Params* p, const Normal_Attack_Step* normal_attack
);
void pattern_approach_walk_em_term_sa_term(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_em_term_sa_term_2(PLW* wk);
void pattern_approach_walk_em_term_sa_term_3(PLW* wk, const SA_Term_Args* p);
void pattern_approach_walk_em_term_sa_term_4(PLW* wk, s16 target_pos, const SA_Term_Args* p, s16 reaction);
void pattern_approach_walk_em_term_sa_term_5(PLW* wk, s16 target_pos, const SA_Term_Args* p);
void pattern_approach_walk_em_term_search_back_term(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_approach_walk_etc_term_com_random_select(PLW* wk);
void pattern_approach_walk_etc_term_lever_attack(PLW* wk, s16 target_pos, u32 next_action, u16 next_menu);
void pattern_approach_walk_hi_jump_attack_term_command_attack(
    PLW* wk, s16 target_pos, const Hi_Jump_Term_Args* a, const Command_Attack_Args* p
);
void pattern_approach_walk_j_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_approach_walk_jump_attack_term_normal_attack(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p
);
void pattern_approach_walk_jump_attack_term_normal_attack_2(PLW* wk, s16 option, const Command_Attack_Args* p);
void pattern_approach_walk_jump_attack_term_normal_attack_3(PLW* wk, const SA_Term_Args* p);
void pattern_approach_walk_jump_attack_term_normal_attack_4(PLW* wk, const Command_Attack_Args* p);
void pattern_approach_walk_jump_attack_term_normal_attack_5(
    PLW* wk, s16 option, s16 reaction, const Command_Attack_Args* p
);
void pattern_approach_walk_lever_attack(PLW* wk, s16 target_pos, u16 lever, u16 lever_data);
void pattern_approach_walk_lever_attack_2(PLW* wk, s16 target_pos, u16 lever_data);
void pattern_approach_walk_lever_attack_sa_term(PLW* wk, s16 target_pos, u16 lever, const SA_Term_Args* p);
void pattern_approach_walk_lever_attack_sa_term_2(PLW* wk, s16 target_pos, u16 lever_data, const SA_Term_Args* p);
void pattern_approach_walk_look(PLW* wk, s16 option);
void pattern_approach_walk_normal_attack(PLW* wk, s16 target_pos);
void pattern_approach_walk_normal_attack_2(PLW* wk, s16 reaction, u16 lever_data);
void pattern_approach_walk_normal_attack_3(PLW* wk, s16 target_pos, s16 reaction, u16 lever_data);
void pattern_approach_walk_normal_attack_4(PLW* wk, s16 target_pos, s16 reaction, u16 lever_data);
void pattern_approach_walk_normal_attack_branch_unit_area(PLW* wk, s16 target_pos, const Branch_Menu_Args* p);
void pattern_approach_walk_normal_attack_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void pattern_approach_walk_normal_attack_j_command_attack(
    PLW* wk, s16 target_pos, u16 lever_data, const Command_Attack_Args* p
);
void pattern_approach_walk_normal_attack_j_command_attack_2(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_approach_walk_normal_attack_j_command_attack_3(
    PLW* wk, s16 target_pos, const Normal_Attack_Step* normal_attack, const Command_Attack_Args* p
);
void pattern_approach_walk_normal_attack_lever_attack(PLW* wk, s16 target_pos, u16 lever, u16 lever_data);
void pattern_approach_walk_normal_attack_lever_attack_2(PLW* wk, s16 target_pos, u16 lever);
void pattern_approach_walk_normal_attack_sa_term(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_normal_attack_sa_term_2(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_normal_attack_sa_term_3(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_sa_term(PLW* wk, s16 target_pos, const SA_Term_Args* p);
void pattern_approach_walk_sa_term_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_approach_walk_sa_term_command_attack_2(PLW* wk, s16 target_pos, const SA_Term_Args* p);
void pattern_approach_walk_turn_over_on_hi_jump_attack_term(
    PLW* wk, s16 target_pos, const Hi_Jump_Term_Args* a, const SA_Term_Args* p
);
void pattern_approach_walk_wait_get_up_command_attack(
    PLW* wk, s16 target_pos, s16 option, const Command_Attack_Args* p
);
void pattern_approach_walk_wait_get_up_em_term(PLW* wk, u16 lever_data);
void pattern_approach_walk_wait_get_up_j_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_approach_walk_wait_get_up_lever_attack(PLW* wk, s16 target_pos, u16 lever, u16 lever_data);
void pattern_approach_walk_wait_get_up_normal_attack(PLW* wk, s16 target_pos, u16 lever_data);
void pattern_approach_walk_wait_get_up_normal_attack_2(PLW* wk);
void pattern_approach_walk_wait_get_up_normal_attack_3(PLW* wk, u16 lever_data);
void pattern_approach_walk_wait_get_up_normal_attack_4(PLW* wk, s16 target_pos, s16 reaction);
void pattern_approach_walk_wait_get_up_normal_attack_5(PLW* wk);
void pattern_approach_walk_wait_get_up_sa_term(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_approach_walk_wait_get_up_sa_term_com_random_select(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Com_Random_Select_Step* com_random_select
);
void pattern_check_ex_command_attack(PLW* wk, s16 next_menu, const Command_Attack_Args* p);
void pattern_check_ex_j_command_attack(PLW* wk, s16 next_menu, const Command_Attack_Args* p);
void pattern_check_ex_wait_j_command_attack(PLW* wk, s16 time, const Command_Attack_Args* p);
void pattern_check_sa_full_em_term_command_attack(PLW* wk);
void pattern_check_store_lever_command_attack(
    PLW* wk, const Check_Store_Lever_Step* check_store_lever, const Command_Attack_Args* p
);
void pattern_com_random_select_from_step_6(PLW* wk);
void pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_command_attack_2(PLW* wk, const Command_Attack_Args* p);
void pattern_command_attack_3(PLW* wk, const Command_Attack_Args* p);
void pattern_command_attack_approach_walk_em_term(PLW* wk, const EM_Term_Params* p);
void pattern_command_attack_approach_walk_em_term_2(PLW* wk, u16 lever);
void pattern_command_attack_branch_unit_area(PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b);
void pattern_command_attack_check_sa_command_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_command_attack_check_sa_command_attack_2(PLW* wk, const Command_Attack_Args* p);
void pattern_command_attack_com_random_select(PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b);
void pattern_command_attack_em_term_command_attack(PLW* wk);
void pattern_command_attack_em_term_sa_term(PLW* wk);
void pattern_command_attack_from_step_6(PLW* wk);
void pattern_command_attack_from_step_6_2(PLW* wk);
void pattern_command_attack_normal_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_em_term_adjust_attack(PLW* wk, const EM_Term_Params* p);
void pattern_em_term_adjust_attack_2(PLW* wk, const EM_Term_Params* p, u16 lever_data);
void pattern_em_term_branch_unit_area(PLW* wk, const EM_Term_Params* p, const Branch_Menu_Args* p_b);
void pattern_em_term_com_random_select(PLW* wk, const EM_Term_Params* p, const Branch_Menu_Args* p_b);
void pattern_em_term_command_attack_wait(PLW* wk, const Command_Attack_Args* p, const SA_Term_Args* p_b);
void pattern_em_term_jump_attack_term(PLW* wk, const EM_Term_Params* p, const Jump_Term_Args* a);
void pattern_em_term_jump_command_attack_term(PLW* wk, const EM_Term_Params* p);
void pattern_em_term_lever_attack_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data);
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
void pattern_em_term_normal_attack_j_command_attack_2(
    PLW* wk, const EM_Term_Params* p, u16 lever_data, const Command_Attack_Args* p_b
);
void pattern_em_term_normal_attack_sa_term(PLW* wk);
void pattern_em_term_sa_term_command_attack(
    PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
);
void pattern_em_term_sa_term_em_term(
    PLW* wk, const EM_Term_Params* p, const EM_Term_Params* p_b, const Command_Attack_Args* p_b_b
);
void pattern_em_term_sa_term_forced_guard(PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b);
void pattern_em_term_sa_term_j_command_attack(
    PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
);
void pattern_em_term_sa_term_normal_attack(PLW* wk, const SA_Term_Args* p, s16 reaction, s16 reaction_b);
void pattern_em_term_search_back_term_command_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_em_term_short_range_attack(PLW* wk, const Short_Range_Args* p);
void pattern_etc_term_branch_unit_area(PLW* wk, u32 next_action, u16 next_menu, const Branch_Menu_Args* p);
void pattern_etc_term_check_sa_branch_unit_area(
    PLW* wk, const ETC_Term_Step* etc_term, s16 next_menu, const Branch_Menu_Args* p
);
void pattern_etc_term_normal_attack(PLW* wk);
void pattern_etc_term_normal_attack_com_random_select(
    PLW* wk, const ETC_Term_Step* etc_term, const Normal_Attack_Step* normal_attack, const Com_Random_Select_Step* com_random_select
);
void pattern_etc_term_normal_attack_lever_attack(PLW* wk, u16 next_menu, u16 lever, u16 lever_data);
void pattern_etc_term_provoke(PLW* wk, s16 exit_no, u16 next_menu);
void pattern_etc_term_provoke_2(PLW* wk, s16 exit_no, u32 next_action, u16 next_menu);
void pattern_etc_term_provoke_next_another_menu(PLW* wk, u16 next_menu, u16 next_menu_b);
void pattern_forced_guard(PLW* wk, s16 guard_type);
void pattern_hi_jump(PLW* wk, s16 pl_number);
void pattern_hi_jump_attack_normal_attack(PLW* wk, const Jump_Attack_Args* a, u16 lever_data);
void pattern_hi_jump_attack_term_command_attack(PLW* wk, const Hi_Jump_Term_Args* a, const Command_Attack_Args* p);
void pattern_hi_jump_attack_term_normal_attack(PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data);
void pattern_hi_jump_attack_term_normal_attack_2(
    PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data, u16 lever_data_b
);
void pattern_hi_jump_attack_term_normal_attack_3(PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data);
void pattern_hi_jump_attack_term_normal_attack_4(
    PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data, u16 lever_data_b
);
void pattern_hi_jump_attack_term_normal_attack_com_random_select(PLW* wk, s16 reaction, s16 reaction_b);
void pattern_hi_jump_attack_term_normal_attack_command_attack(
    PLW* wk, const Hi_Jump_Term_Args* a, const Command_Attack_Args* p
);
void pattern_hi_jump_attack_term_normal_attack_j_command_attack(
    PLW* wk, const Hi_Jump_Term_Args* a, const Command_Attack_Args* p
);
void pattern_hi_jump_attack_term_normal_attack_lever_attack(PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data);
void pattern_hi_jump_attack_term_normal_attack_lever_attack_2(PLW* wk, const Hi_Jump_Term_Args* a, s16 reaction);
void pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_j_command_attack_from_step_6(PLW* wk);
void pattern_jump(PLW* wk, s16 time);
void pattern_jump_attack_term_com_random_select(PLW* wk, const Jump_Term_Args* a, const Branch_Menu_Args* p);
void pattern_jump_attack_term_em_term_sa_term(PLW* wk, const Jump_Term_Args* a, const SA_Term_Args* p);
void pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a, s16 reaction, u16 lever_data);
void pattern_jump_attack_term_normal_attack_2(PLW* wk, const Jump_Term_Args* a);
void pattern_jump_attack_term_normal_attack_3(PLW* wk, const Jump_Term_Args* a, u16 lever_data, u16 lever_data_b);
void pattern_jump_attack_term_normal_attack_4(PLW* wk, const Jump_Term_Args* a, u16 lever_data, u16 lever_data_b);
void pattern_jump_attack_term_normal_attack_5(PLW* wk, const Jump_Term_Args* a);
void pattern_jump_attack_term_normal_attack_com_random_select(
    PLW* wk, const Jump_Term_Args* a, const Branch_Menu_Args* p, s16 rnd_type
);
void pattern_jump_attack_term_normal_attack_command_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_jump_attack_term_normal_attack_command_attack_2(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p
);
void pattern_jump_attack_term_normal_attack_command_attack_3(
    PLW* wk, s16 reaction, s16 reaction_b, const Command_Attack_Args* p
);
void pattern_jump_attack_term_normal_attack_command_attack_4(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p, s16 rnd_type
);
void pattern_jump_attack_term_normal_attack_command_attack_5(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p, const SA_Term_Args* p_b
);
void pattern_jump_attack_term_normal_attack_command_attack_6(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p
);
void pattern_jump_attack_term_normal_attack_command_attack_7(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p
);
void pattern_jump_attack_term_normal_attack_command_attack_8(
    PLW* wk, const Command_Attack_Args* p, const SA_Term_Args* p_b
);
void pattern_jump_attack_term_normal_attack_j_command_attack(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p, const SA_Term_Args* p_b
);
void pattern_jump_attack_term_normal_attack_j_command_attack_2(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const Command_Attack_Args* p
);
void pattern_jump_attack_term_normal_attack_j_command_attack_3(PLW* wk, const Command_Attack_Args* p);
void pattern_jump_attack_term_normal_attack_sa_term(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_jump_attack_term_sa_term_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_jump_attack_term_sa_term_j_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_jump_attack_term_wait_get_up_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data);
void pattern_jump_look(PLW* wk);
void pattern_keep_away_com_random_select(PLW* wk, s16 target_pos);
void pattern_keep_away_com_random_select_2(PLW* wk, s16 target_pos, s16 option, const Branch_Menu_Args* p);
void pattern_keep_away_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void pattern_keep_away_wait_get_up(PLW* wk, s16 target_pos, u16 lever_data, s16 option);
void pattern_keep_away_wait_get_up_2(PLW* wk, u16 lever_data);
void pattern_keep_away_wait_get_up_com_random_select(PLW* wk, s16 target_pos, const Branch_Menu_Args* p);
void pattern_keep_away_wait_get_up_command_attack(PLW* wk, s16 target_pos, s16 option, const Command_Attack_Args* p);
void pattern_keep_status(PLW* wk, s16 option_data);
void pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data);
void pattern_lever_attack_2(
    PLW* wk, const Lever_Attack_Step* lever_attack, const Lever_Attack_Step* lever_attack_b, u16 lever_data
);
void pattern_lever_attack_command_attack(PLW* wk, u16 lever, const Command_Attack_Args* p);
void pattern_lever_attack_j_command_attack(PLW* wk, u16 lever_data, const Command_Attack_Args* p);
void pattern_lever_attack_normal_attack(PLW* wk);
void pattern_lever_attack_normal_attack_j_command_attack(
    PLW* wk, u16 lever_data, u16 lever_data_b, const Command_Attack_Args* p
);
void pattern_lever_attack_sp(PLW* wk);
void pattern_lever_attack_sp_com_random_select(PLW* wk, const Branch_Menu_Args* p);
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
void pattern_normal_attack_6(PLW* wk, s16 reaction, u16 lever_data, s16 reaction_b);
void pattern_normal_attack_7(
    PLW* wk, const Normal_Attack_Step* normal_attack, const Normal_Attack_Step* normal_attack_b, const Normal_Attack_Step* normal_attack_b_b
);
void pattern_normal_attack_branch_unit_area(PLW* wk, const Branch_Menu_Args* p);
void pattern_normal_attack_branch_unit_area_2(PLW* wk, s16 reaction, const Branch_Menu_Args* p);
void pattern_normal_attack_com_random_select(
    PLW* wk, u16 lever_data, const Branch_Menu_Args* p, s16 rnd_type
);
void pattern_normal_attack_com_random_select_2(PLW* wk, s16 reaction, u16 lever_data);
void pattern_normal_attack_com_random_select_3(PLW* wk, u16 lever_data);
void pattern_normal_attack_com_random_select_4(PLW* wk, s16 reaction, s16 reaction_b);
void pattern_normal_attack_com_random_select_5(PLW* wk, u16 lever_data);
void pattern_normal_attack_command_attack(PLW* wk, s16 reaction, s16 reaction_b, const Command_Attack_Args* p);
void pattern_normal_attack_command_attack_2(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void pattern_normal_attack_command_attack_3(PLW* wk, s16 reaction, s16 reaction_b, const Command_Attack_Args* p);
void pattern_normal_attack_command_attack_4(PLW* wk, s16 reaction, s16 reaction_b, const Command_Attack_Args* p);
void pattern_normal_attack_command_attack_5(PLW* wk, const Command_Attack_Args* p);
void pattern_normal_attack_command_attack_6(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p);
void pattern_normal_attack_command_attack_com_random_select(
    PLW* wk, const Normal_Attack_Step* normal_attack, const Command_Attack_Args* p, const Com_Random_Select_Step* com_random_select
);
void pattern_normal_attack_command_attack_wait(PLW* wk, const Command_Attack_Args* p, const SA_Term_Args* p_b);
void pattern_normal_attack_command_attack_wait_2(PLW* wk, u16 lever_data, s16 reaction);
void pattern_normal_attack_from_step_6(PLW* wk);
void pattern_normal_attack_j_command_attack(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p);
void pattern_normal_attack_j_command_attack_2(
    PLW* wk, const Normal_Attack_Step* normal_attack, u16 lever_data, const Command_Attack_Args* p
);
void pattern_normal_attack_j_command_attack_3(PLW* wk, u16 lever_data, u16 lever_data_b, const Command_Attack_Args* p);
void pattern_normal_attack_j_command_attack_4(PLW* wk, u16 lever_data, const Command_Attack_Args* p);
void pattern_normal_attack_j_command_attack_5(PLW* wk, const Command_Attack_Args* p);
void pattern_normal_attack_j_command_attack_com_random_select(PLW* wk);
void pattern_normal_attack_j_command_attack_com_random_select_2(PLW* wk);
void pattern_normal_attack_jump_attack_term_normal_attack(PLW* wk, const SA_Term_Args* p);
void pattern_normal_attack_jump_attack_term_normal_attack_2(PLW* wk, const Command_Attack_Args* p);
void pattern_normal_attack_jump_attack_term_sa_term(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_normal_attack_jump_attack_term_sa_term_2(
    PLW* wk, s16 reaction, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_normal_attack_sa_term_branch_unit_area(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b);
void pattern_normal_attack_sa_term_com_random_select(PLW* wk, s16 reaction, const SA_Term_Args* p);
void pattern_normal_attack_sa_term_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_normal_attack_sa_term_command_attack_2(
    PLW* wk, s16 reaction, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_normal_attack_sa_term_j_command_attack(
    PLW* wk, u16 lever_data, const SA_Term_Args* p, const Command_Attack_Args* p_b
);
void pattern_normal_attack_sa_term_normal_attack(PLW* wk, const SA_Term_Args* p);
void pattern_normal_attack_sa_term_normal_attack_2(PLW* wk, const SA_Term_Args* p, u16 lever_data);
void pattern_oro_hjca_term(PLW* wk, const ORO_JCA_Term_Args* a);
void pattern_pierce_on_approach_walk_jump_attack_term(PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p);
void pattern_pierce_on_command_attack(PLW* wk, const Command_Attack_Args* p);
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
void pattern_pierce_on_command_attack_normal_attack_5(PLW* wk, const Command_Attack_Args* p);
void pattern_pierce_on_command_attack_wait(PLW* wk);
void pattern_pierce_on_command_attack_wait_2(PLW* wk, const Command_Attack_Args* p);
void pattern_pierce_on_command_attack_wait_3(PLW* wk, const Command_Attack_Args* p);
void pattern_pierce_on_em_term_j_command_attack(
    PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b, const Command_Attack_Args* p_b_b
);
void pattern_pierce_on_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
void pattern_pierce_on_em_term_normal_attack_2(PLW* wk);
void pattern_pierce_on_em_term_normal_attack_3(PLW* wk, const Command_Attack_Args* p);
void pattern_pierce_on_hi_jump_attack_term_lever_on(PLW* wk, u16 lever_data);
void pattern_pierce_on_hi_jump_attack_term_lever_on_2(PLW* wk, const Hi_Jump_Term_Args* a);
void pattern_pierce_on_j_command_attack(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b);
void pattern_pierce_on_j_command_attack_normal_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_pierce_on_lever_attack(PLW* wk);
void pattern_pierce_on_lever_on_lever_attack(PLW* wk);
void pattern_provoke(PLW* wk, s16 lever);
void pattern_provoke_com_random_select(PLW* wk, const Branch_Menu_Args* p);
void pattern_provoke_pierce_on_command_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_provoke_sa_term_com_random_select(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b);
void pattern_provoke_wait_get_up(PLW* wk);
void pattern_sa_term(PLW* wk, const SA_Term_Args* p);
void pattern_sa_term_2(PLW* wk);
void pattern_sa_term_approach_walk_jump_attack(PLW* wk);
void pattern_sa_term_approach_walk_sa_term(
    PLW* wk, const SA_Term_Args* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
);
void pattern_sa_term_approach_walk_sa_term_2(PLW* wk);
void pattern_sa_term_approach_walk_sa_term_3(PLW* wk, const SA_Term_Args* p);
void pattern_sa_term_branch_unit_area(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b);
void pattern_sa_term_com_random_select(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b, s16 rnd_type);
void pattern_sa_term_command_attack_2(
    PLW* wk, const SA_Term_Args* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
);
void pattern_sa_term_em_term_sa_term(PLW* wk);
void pattern_sa_term_forced_guard(PLW* wk, const SA_Term_Args* p);
void pattern_sa_term_from_step_6(PLW* wk);
void pattern_sa_term_j_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
void pattern_sa_term_next_another_menu(PLW* wk, const SA_Term_Args* p, u16 next_menu);
void pattern_sa_term_normal_attack(PLW* wk, const SA_Term_Args* p, s16 reaction, u16 lever_data);
void pattern_sa_term_normal_attack_com_random_select(PLW* wk, const SA_Term_Args* p, s16 reaction);
void pattern_sa_term_wait_get_up_sa_term(PLW* wk, const SA_Term_Args* p, const SA_Term_Args* p_b, s16 reaction);
void pattern_search_back_term_command_attack(PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p);
void pattern_search_back_term_command_attack_2(PLW* wk, const Command_Attack_Args* p);
void pattern_search_back_term_command_attack_from_step_6(PLW* wk);
void pattern_search_back_term_command_attack_sa_term(
    PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p
);
void pattern_search_back_term_command_attack_sa_term_2(
    PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p
);
void pattern_search_back_term_jump(PLW* wk, s16 move_value);
void pattern_search_back_term_jump_attack_term(
    PLW* wk, const Search_Back_Term_Step* search_back_term, const Jump_Term_Args* a
);
void pattern_search_back_term_pierce_on_command_attack(
    PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p
);
void pattern_search_back_term_pierce_on_command_attack_2(
    PLW* wk, s16 move_value, s16 next_menu, const Branch_Menu_Args* p
);
void pattern_search_back_term_pierce_on_command_attack_3(PLW* wk, const Command_Attack_Args* p);
void pattern_search_back_term_pierce_on_command_attack_4(PLW* wk, s16 next_menu);
void pattern_search_back_term_pierce_on_keep_away(PLW* wk, s16 next_menu, s16 target_pos, const Branch_Menu_Args* p);
void pattern_search_back_term_pierce_on_keep_away_2(PLW* wk, s16 target_pos, const Branch_Menu_Args* p);
void pattern_search_back_term_pierce_on_keep_away_3(PLW* wk, s16 target_pos);
void pattern_search_back_term_pierce_on_keep_away_4(PLW* wk, s16 move_value, s16 next_menu, s16 target_pos);
void pattern_search_back_term_pierce_on_keep_away_5(PLW* wk, s16 target_pos);
void pattern_search_back_term_pierce_on_keep_away_6(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void pattern_search_back_term_walk_em_term(PLW* wk, const Command_Attack_Args* p);
void pattern_search_back_term_walk_normal_attack(PLW* wk, s16 next_menu, u16 lever_data);
void pattern_search_back_term_walk_wait_get_up(PLW* wk, s16 move_value, s16 next_menu, s16 time);
void pattern_search_back_term_walk_wait_get_up_2(PLW* wk, s16 move_value, s16 next_menu, s16 time);
void pattern_short_range_attack(PLW* wk);
void pattern_turn_over_on_jump_attack_term(PLW* wk);
void pattern_vs_jump_guard(PLW* wk);
void pattern_wait_attack_complete(PLW* wk, s16 option);
void pattern_wait_attack_complete_normal_attack(PLW* wk);
void pattern_wait_attack_complete_sa_term_wait_attack_complete(PLW* wk, const SA_Term_Args* p);
void pattern_wait_branch_unit_area(PLW* wk, s16 time, const Branch_Menu_Args* p);
void pattern_wait_com_random_select(PLW* wk, s16 time, const Branch_Menu_Args* p);
void pattern_wait_command_attack(PLW* wk, s16 time, const Command_Attack_Args* p);
void pattern_wait_command_attack_wait_from_step_6(PLW* wk);
void pattern_wait_em_term_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_wait_em_term_normal_attack(PLW* wk, s16 time, const EM_Term_Params* p, u16 lever_data);
void pattern_wait_get_up(PLW* wk, u16 lever_data, s16 option);
void pattern_wait_get_up_2(PLW* wk, u16 lever_data);
void pattern_wait_get_up_branch_unit_area(PLW* wk, u16 lever_data, const Branch_Menu_Args* p);
void pattern_wait_get_up_branch_wait_area_command_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_wait_get_up_branch_wait_area_j_command_attack(
    PLW* wk, const Branch_Wait_Args* p, const Command_Attack_Args* p_b
);
void pattern_wait_get_up_com_random_select(PLW* wk, u16 lever_data, s16 option, const Branch_Menu_Args* p);
void pattern_wait_get_up_command_attack(PLW* wk, u16 lever_data, s16 option, const Command_Attack_Args* p);
void pattern_wait_get_up_command_attack_2(PLW* wk, const Command_Attack_Args* p);
void pattern_wait_get_up_command_attack_em_term(PLW* wk);
void pattern_wait_get_up_j_command_attack(PLW* wk, const Command_Attack_Args* p);
void pattern_wait_get_up_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void pattern_wait_get_up_jump_attack_term_j_command_attack(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p
);
void pattern_wait_get_up_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data);
void pattern_wait_get_up_jump_attack_term_normal_attack_2(
    PLW* wk, const Jump_Term_Args* a, s16 reaction, u16 lever_data
);
void pattern_wait_get_up_jump_attack_term_normal_attack_3(
    PLW* wk, const Jump_Term_Args* a, s16 reaction, const Command_Attack_Args* p
);
void pattern_wait_get_up_jump_attack_term_normal_attack_4(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p
);
void pattern_wait_get_up_lever_attack(PLW* wk, u16 lever_data, u16 lever, u16 lever_data_b);
void pattern_wait_get_up_lever_attack_2(PLW* wk, u16 lever, u16 lever_data);
void pattern_wait_get_up_normal_attack(PLW* wk, u16 lever_data, u16 lever_data_b);
void pattern_wait_get_up_normal_attack_2(PLW* wk, u16 lever_data);
void pattern_wait_get_up_normal_attack_3(
    PLW* wk, u16 lever_data, const Normal_Attack_Step* normal_attack, u16 lever_data_b
);
void pattern_wait_get_up_normal_attack_command_attack(PLW* wk, const Command_Attack_Args* p);
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
void pattern_wait_lie_approach_walk(PLW* wk, s16 target_pos);
void pattern_wait_lie_jump(PLW* wk);
void pattern_walk(PLW* wk, s16 unused);
void pattern_walk_branch_unit_area(PLW* wk, const Branch_Menu_Args* p);
void pattern_walk_com_random_select(PLW* wk, s16 time, const Branch_Menu_Args* p, s16 rnd_type);
void pattern_walk_com_random_select_2(PLW* wk);
void pattern_walk_wait_get_up_j_command_attack(PLW* wk, const Command_Attack_Args* p);

#endif
