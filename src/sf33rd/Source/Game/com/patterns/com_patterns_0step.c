/**
 * @file com_patterns_0step.c
 * COM: pattern skeletons shared by every character
 *
 * The 0-step patterns.
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

void active_pattern_(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_approach_walk_lever_attack(PLW* wk, s16 target_pos, u16 lever) {
    pattern_approach_walk_lever_attack(wk, target_pos, lever, 0x110);
}

void active_pattern_command_attack_branch_unit_area(PLW* wk, const Command_Attack_Args* p) {
    pattern_command_attack_branch_unit_area(wk, p, &(Branch_Menu_Args) { 2, 0x31, 0x32, 0x33, 1 });
}

void active_pattern_jump(PLW* wk) {
    pattern_jump(wk, 0);
}

void active_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data) {
    pattern_jump_attack_term_normal_attack(wk, a, 8, lever_data);
}

void active_pattern_jump_attack_term_normal_attack_j_command_attack_2(PLW* wk, const Command_Attack_Args* p) {
    pattern_jump_attack_term_normal_attack_j_command_attack_2(
        wk, &(Jump_Term_Args) { -0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x200 }, 0x40, p
    );
}

void active_pattern_lever_attack_command_attack(PLW* wk, const Command_Attack_Args* p) {
    pattern_lever_attack_command_attack(wk, 0, p);
}

void active_pattern_normal_attack_2(PLW* wk, u16 lever_data, u16 lever_data_b) {
    pattern_normal_attack_2(wk, 0xB, lever_data, lever_data_b);
}

void active_pattern_normal_attack_4(PLW* wk, u16 lever_data, u16 lever_data_b) {
    pattern_normal_attack_2(wk, 8, lever_data, lever_data_b);
}

void active_pattern_pierce_on_command_attack_j_command_attack(PLW* wk, const Command_Attack_Args* p) {
    pattern_pierce_on_command_attack_j_command_attack(wk, &(Command_Attack_Args) { 0xC, 0, -1, -1 }, p);
}

void active_pattern_provoke(PLW* wk) {
    pattern_provoke(wk, -1);
}

void active_pattern_sa_term_branch_unit_area(PLW* wk, const SA_Term_Args* p) {
    pattern_sa_term_branch_unit_area(wk, p, &(Branch_Menu_Args) { 2, 0x41, 0x41, 0x42, 0x43 });
}

void active_pattern_sa_term_com_random_select(PLW* wk, const SA_Term_Args* p, const Branch_Menu_Args* p_b) {
    pattern_sa_term_com_random_select(wk, p, p_b, 1);
}

void active_pattern_sa_term_command_attack_2(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    pattern_sa_term_command_attack_2(wk, &(SA_Term_Args) { 0x35, -1, -1, 0x9F }, p, p_b);
}

void active_pattern_walk_com_random_select(PLW* wk, const Branch_Menu_Args* p) {
    pattern_walk_com_random_select(wk, 0x20, p, 3);
}

void pattern_approach_walk_com_random_select_2(PLW* wk, const Branch_Menu_Args* p) {
    active_pattern_approach_walk_com_random_select_2(wk, 0xBF, 3, p);
}

void pattern_approach_walk_jump_attack_term_normal_attack_3(PLW* wk, const SA_Term_Args* p) {
    active_pattern_approach_walk_jump_attack_term_normal_attack_3(wk, p);
}

void pattern_command_attack(PLW* wk, const Command_Attack_Args* p) {
    active_pattern_command_attack(wk, p);
}

void pattern_command_attack_check_sa_command_attack(PLW* wk, const Command_Attack_Args* p) {
    active_pattern_command_attack_check_sa_command_attack(wk, p);
}

void pattern_command_attack_com_random_select(PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b) {
    active_pattern_command_attack_com_random_select(wk, p, p_b, 1);
}

void pattern_command_attack_normal_attack(PLW* wk, const Command_Attack_Args* p) {
    active_pattern_command_attack_normal_attack(wk, p, 8, 0x402);
}

void pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data) {
    active_pattern_em_term_normal_attack(wk, p, 8, lever_data);
}

void pattern_jump_attack_term_normal_attack_2(PLW* wk, const Jump_Term_Args* a) {
    pattern_jump_attack_term_normal_attack(wk, a, 8, 0x200);
}

void pattern_jump_attack_term_sa_term_command_attack(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    active_pattern_jump_attack_term_sa_term_command_attack(wk, p, p_b);
}

void pattern_jump_look(PLW* wk) {
    active_pattern_jump_look(wk, 1);
}

void pattern_keep_away_wait_get_up_2(PLW* wk, u16 lever_data) {
    pattern_keep_away_wait_get_up(wk, 0xBF, lever_data, -1);
}

void pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data) {
    active_pattern_lever_attack(wk, 8, lever, lever_data);
}

void pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data) {
    active_pattern_normal_attack(wk, reaction, lever_data);
}

void pattern_normal_attack_branch_unit_area_2(PLW* wk, s16 reaction, const Branch_Menu_Args* p) {
    active_pattern_normal_attack_branch_unit_area(wk, reaction, 0x102, p);
}

void pattern_normal_attack_com_random_select_3(PLW* wk, u16 lever_data) {
    pattern_normal_attack_com_random_select(wk, lever_data, &(Branch_Menu_Args) { 6, 1, 1, 0xC0, 0xC0 }, 4);
}

void pattern_normal_attack_command_attack_2(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p) {
    active_pattern_normal_attack_command_attack(wk, reaction, lever_data, p);
}

void pattern_normal_attack_j_command_attack(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p) {
    active_pattern_normal_attack_j_command_attack(wk, reaction, lever_data, p);
}

void pattern_normal_attack_j_command_attack_5(PLW* wk, const Command_Attack_Args* p) {
    active_pattern_normal_attack_j_command_attack_2(wk, 0xB, 0x10, p);
}

void pattern_sa_term(PLW* wk, const SA_Term_Args* p) {
    active_pattern_sa_term(wk, p);
}

void pattern_search_back_term_pierce_on_command_attack_4(PLW* wk, s16 next_menu) {
    active_pattern_search_back_term_pierce_on_command_attack(wk, 0x30, 6, next_menu);
}

void pattern_wait_get_up_2(PLW* wk, u16 lever_data) {
    pattern_wait_get_up(wk, lever_data, -1);
}

void pattern_wait_get_up_command_attack_2(PLW* wk, const Command_Attack_Args* p) {
    pattern_wait_get_up_command_attack(wk, 0, -1, p);
}

void pattern_wait_get_up_lever_attack_2(PLW* wk, u16 lever, u16 lever_data) {
    pattern_wait_get_up_lever_attack(wk, 0, lever, lever_data);
}

void pattern_walk(PLW* wk, s16 unused) {
    active_pattern_walk(wk, 1, 0x20, unused);
}

void pattern_end_immediately(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    default:
        End_Pattern(wk);
    }
}
