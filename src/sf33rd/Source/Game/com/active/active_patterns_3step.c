/**
 * @file active_patterns_3step.c
 * COM Passive: pattern skeletons shared by every character
 *
 * The three-step patterns.
 *
 * A passive pattern script is a switch on the step counter with one engine call
 * per step, and the same step sequences recur across characters. Each skeleton
 * here is exactly the body its call sites used to hold, with the arguments of
 * its calls taken as parameters and written out in full at each call site.
 */

#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void active_pattern_adjust_attack(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xC, lever_data);
        break;

    case 1:
        Adjust_Attack(wk, 0xC, lever_data_b);
        break;

    case 2:
        Adjust_Attack(wk, 8, lever_data_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_adjust_attack_2(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 8, lever_data);
        break;

    case 1:
        Adjust_Attack(wk, 8, lever_data_b);
        break;

    case 2:
        Adjust_Attack(wk, 8, lever_data_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_approach_walk_jump_command_attack_term_com_random_select(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x2F, 0xA, -1, -0x7F60, 0x50, 0, -0x7F80, -1, 0x400});
        break;

    case 2:
        Com_Random_Select(wk, p, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_approach_walk_sa_term_j_command_attack(
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

void active_pattern_jump_attack_term_normal_attack_command_attack_2(
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
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_jump_attack_term_normal_attack_j_command_attack(
    PLW* wk, const Jump_Term_Args* a, u16 lever_data, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
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

void active_pattern_normal_attack_5(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, lever_data);
        break;

    case 1:
        Normal_Attack(wk, 9, lever_data_b);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_6(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b) {
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

void active_pattern_normal_attack_7(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, lever_data);
        break;

    case 1:
        Normal_Attack(wk, 9, lever_data_b);
        break;

    case 2:
        Normal_Attack(wk, 9, lever_data_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_8(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, lever_data);
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

void active_pattern_normal_attack_command_attack_2(
    PLW* wk, u16 lever_data, u16 lever_data_b, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, lever_data);
        break;

    case 1:
        Normal_Attack(wk, 0xB, lever_data_b);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_command_attack_3(PLW* wk, u16 lever_data, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_j_command_attack_2(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 2:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_lever_off_look(PLW* wk, s16 reaction) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, 0x100);
        break;

    case 1:
        Lever_Off(wk);
        break;

    case 2:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_pierce_on_command_attack(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b) {
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

void active_pattern_pierce_on_jump_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type) {
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

void active_pattern_search_back_term_jump_look(PLW* wk, s16 move_value, s16 next_action, s16 next_menu) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, next_action, next_menu);
        break;

    case 1:
        Jump(wk, 1);
        break;

    case 2:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_search_back_term_pierce_on_command_attack(PLW* wk, s16 move_value, s16 next_action, s16 next_menu) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, next_action, next_menu);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_turn_over_on_jump_attack_term_com_random_select(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40});
        break;

    case 2:
        Com_Random_Select(wk, p, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_walk_2(PLW* wk, s16 time, s16 time_b, s16 time_b_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 0, time, 0);
        break;

    case 1:
        Walk(wk, 1, time_b, 0);
        break;

    case 2:
        Walk(wk, 0, time_b_b, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_adjust_attack_normal_attack_lever_attack(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xB, 0x20);
        break;

    case 1:
        Normal_Attack(wk, 0xA, 0x202);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_command_attack_lever_off_look(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1});
        break;

    case 1:
        Lever_Off(wk);
        break;

    case 2:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_jump_lever_off_look(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, 1);
        break;

    case 1:
        Lever_Off(wk);
        break;

    case 2:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_adjust_attack_normal_attack_branch_unit_area(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xB, 0x20);
        break;

    case 1:
        Normal_Attack(wk, 0xA, 0x202);
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){2, 0x31, 0x32, 0x33, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
