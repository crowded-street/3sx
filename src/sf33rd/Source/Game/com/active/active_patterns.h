/*
 * Pattern skeletons shared by every character's active scripts.
 * See active_patterns_1step.c and its siblings.
 */

#ifndef ACTIVE_PATTERNS_H
#define ACTIVE_PATTERNS_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void active_pattern_(PLW* wk);
void active_pattern_adjust_attack(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b);
void active_pattern_adjust_attack_2(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b);
void active_pattern_adjust_attack_3(PLW* wk, u16 lever_data);
void active_pattern_adjust_attack_4(PLW* wk, s16 reaction, s16 reaction_b, u16 lever_data);
void active_pattern_adjust_attack_command_attack(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p);
void active_pattern_adjust_attack_j_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
);
void active_pattern_adjust_attack_lever_attack(PLW* wk);
void active_pattern_adjust_attack_normal_attack(PLW* wk, s16 reaction, u16 lever_data, u16 lever_data_b);
void active_pattern_adjust_attack_normal_attack_branch_unit_area(PLW* wk);
void active_pattern_adjust_attack_normal_attack_lever_attack(PLW* wk);
void active_pattern_approach_walk(PLW* wk, s16 target_pos);
void active_pattern_approach_walk_com_random_select(PLW* wk, s16 target_pos, const Branch_Menu_Args* p);
void active_pattern_approach_walk_com_random_select_2(PLW* wk, s16 target_pos, s16 option, const Branch_Menu_Args* p);
void active_pattern_approach_walk_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p);
void active_pattern_approach_walk_jump_command_attack_term_com_random_select(PLW* wk, const Branch_Menu_Args* p);
void active_pattern_approach_walk_lever_attack(PLW* wk, s16 target_pos, u16 lever);
void active_pattern_approach_walk_normal_attack(PLW* wk, s16 target_pos, u16 lever_data);
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
void active_pattern_command_attack_com_random_select(
    PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b, s16 rnd_type
);
void active_pattern_command_attack_j_command_attack(
    PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b
);
void active_pattern_command_attack_lever_attack(PLW* wk, const Command_Attack_Args* p, u16 lever, u16 lever_data);
void active_pattern_command_attack_lever_off_look(PLW* wk);
void active_pattern_command_attack_look(PLW* wk);
void active_pattern_command_attack_normal_attack(PLW* wk, const Command_Attack_Args* p, s16 reaction, u16 lever_data);
void active_pattern_command_attack_push_shot(PLW* wk, const Command_Attack_Args* p, s16 power_level);
void active_pattern_command_attack_sa_term_em_term(PLW* wk, const Command_Attack_Args* p);
void active_pattern_command_attack_turn_over_on_jump_attack_term(PLW* wk, const Branch_Menu_Args* p);
void active_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void active_pattern_em_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void active_pattern_em_term_lever_attack(PLW* wk, const EM_Term_Params* p, u16 lever, u16 lever_data);
void active_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data);
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
void active_pattern_jump_attack_term_j_command_attack(PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p);
void active_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data);
void active_pattern_jump_attack_term_normal_attack_command_attack(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const SA_Term_Args* p
);
void active_pattern_jump_attack_term_normal_attack_command_attack_2(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const Command_Attack_Args* p
);
void active_pattern_jump_attack_term_normal_attack_j_command_attack(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const Command_Attack_Args* p
);
void active_pattern_jump_command_attack_term(PLW* wk, const JCA_Term_Args* a);
void active_pattern_jump_lever_off_look(PLW* wk);
void active_pattern_keep_away(PLW* wk, s16 target_pos, s16 option);
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
void active_pattern_normal_attack_lever_off_look(PLW* wk, s16 reaction);
void active_pattern_normal_attack_sp(PLW* wk);
void active_pattern_oro_ja_term(PLW* wk, const ORO_Air_Term_Args* a);
void active_pattern_pierce_on_command_attack(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b);
void active_pattern_pierce_on_jump_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type);
void active_pattern_pierce_on_search_back_term_command_attack(PLW* wk);
void active_pattern_provoke(PLW* wk);
void active_pattern_sa_term(PLW* wk, const SA_Term_Args* p);
void active_pattern_sa_term_branch_unit_area(PLW* wk, const SA_Term_Args* p);
void active_pattern_sa_term_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b);
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
void active_pattern_search_back_term_walk(PLW* wk, s16 move_value, s16 next_action, s16 next_menu);
void active_pattern_search_back_term_walk_2(PLW* wk, s16 move_value, s16 next_action, s16 next_menu);
void active_pattern_search_back_term_walk_wait(PLW* wk);
void active_pattern_search_back_term_walk_wait_2(PLW* wk);
void active_pattern_turn_over_on_jump_attack_term_com_random_select(PLW* wk, const Branch_Menu_Args* p);
void active_pattern_wait(PLW* wk, s16 time);
void active_pattern_walk(PLW* wk, u16 lever, s16 time, s16 unused);
void active_pattern_walk_2(PLW* wk, s16 time, s16 time_b, s16 time_b_b);
void active_pattern_walk_search_back_term_walk(PLW* wk);

#endif
