/**
 * @file com_patterns_3step.c
 * COM: pattern skeletons shared by every character
 *
 * The three-step patterns. Part 1 of 3, in name order.
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

void active_pattern_adjust_attack_normal_attack_branch_unit_area(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xB, 0x20);
        break;

    case 1:
        Normal_Attack(wk, 0xA, 0x202);
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args) { 2, 0x31, 0x32, 0x33, 1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_adjust_attack_normal_attack_command_attack(
    PLW* wk, u16 lever_data, s16 reaction, u16 lever_data_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xB, lever_data);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data_b);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1F, 0xA, -1 });
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

void active_pattern_approach_walk_jump_command_attack_term_com_random_select(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args) { 8, 0x2F, 0xA, -1, -0x7F60, 0x50, 0, -0x7F80, -1, 0x400 });
        break;

    case 2:
        Com_Random_Select(wk, p, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_approach_walk_look_com_random_select(PLW* wk, s16 target_pos) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Look(wk, 2);
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 2, 0x4C, 0x4D, 0x4E, 0x4F }, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_approach_walk_normal_attack_j_command_attack(
    PLW* wk, s16 target_pos, s16 reaction, u16 lever_data
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1C, 0xA, -1 });
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

void active_pattern_command_attack_lever_off_look(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0, 0xB, -1 });
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

void active_pattern_jump_attack_term_sa_term_command_attack(
    PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args) { -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -1, -1, -1 });
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

/* The four active_pattern_normal_attack_5..8 skeletons are the same switch;
 * only the three reactions differ. The steps arrive as a table of the
 * Normal_Attack_Step parameter object com_pattern_args.h already defines, which
 * keeps the helper at two parameters. */
static void run_normal_attack_3step(PLW* wk, const Normal_Attack_Step* steps) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, steps[0].Reaction, steps[0].Lever_Data);
        break;

    case 1:
        Normal_Attack(wk, steps[1].Reaction, steps[1].Lever_Data);
        break;

    case 2:
        Normal_Attack(wk, steps[2].Reaction, steps[2].Lever_Data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_5(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b) {
    run_normal_attack_3step(
        wk, (Normal_Attack_Step[]) { { 9, lever_data }, { 9, lever_data_b }, { 8, lever_data_b_b } }
    );
}

void active_pattern_normal_attack_6(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b) {
    run_normal_attack_3step(
        wk, (Normal_Attack_Step[]) { { 8, lever_data }, { 8, lever_data_b }, { 8, lever_data_b_b } }
    );
}

void active_pattern_normal_attack_7(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b) {
    run_normal_attack_3step(
        wk, (Normal_Attack_Step[]) { { 9, lever_data }, { 9, lever_data_b }, { 9, lever_data_b_b } }
    );
}

void active_pattern_normal_attack_8(PLW* wk, u16 lever_data, u16 lever_data_b, u16 lever_data_b_b) {
    run_normal_attack_3step(
        wk, (Normal_Attack_Step[]) { { 9, lever_data }, { 8, lever_data_b }, { 8, lever_data_b_b } }
    );
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

void active_pattern_pierce_on_j_command_attack_normal_attack(PLW* wk, const Command_Attack_Args* p) {
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
        Command_Attack(wk, &(Command_Attack_Args) { 8, 1, 0xB, -1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_search_back_term_pierce_on_command_attack_4(
    PLW* wk, s16 move_value, s16 next_action, s16 next_menu
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, next_action, next_menu);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 1, 0xA, -1 });
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
        Jump_Attack_Term(wk, &(Jump_Term_Args) { -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40 });
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

void pattern_approach_walk_command_attack(
    PLW* wk, s16 target_pos, const Command_Attack_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
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

void pattern_approach_walk_command_attack_com_random_select_2(
    PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x4B, 2);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    case 2:
        Com_Random_Select(wk, p_b, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_com_random_select(PLW* wk, s16 target_pos, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params) { -1, -0x7FF8, 6, 1, -1 });
        break;

    case 2:
        Com_Random_Select(wk, p, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_command_attack(
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
        Command_Attack(wk, p_b);
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

void pattern_approach_walk_em_term_normal_attack_2(PLW* wk, s16 target_pos, const EM_Term_Params* p, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, p);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_normal_attack_7(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x83, 3);
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

void pattern_approach_walk_etc_term_com_random_select(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        ETC_Term(wk, 0, 2, 0xD);
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 6, 0x9D, 0x9E, 0x9F, 0x9F }, 1);
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

void pattern_approach_walk_hi_jump_attack_term_command_attack(
    PLW* wk, s16 target_pos, const Hi_Jump_Term_Args* a, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, a);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_lever_attack_2(PLW* wk, s16 target_pos, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Lever_Attack(wk, 0xB, 0, lever_data);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_normal_attack_3(PLW* wk, s16 target_pos, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, reaction, 0x100);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_normal_attack_3(
    PLW* wk, u16 lever_data, const Normal_Attack_Step* normal_attack, u16 lever_data_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, lever_data, -1);
        break;

    case 1:
        Normal_Attack(wk, normal_attack->Reaction, normal_attack->Lever_Data);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_7(
    PLW* wk, const Normal_Attack_Step* normal_attack, const Normal_Attack_Step* normal_attack_b,
    const Normal_Attack_Step* normal_attack_b_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, normal_attack->Reaction, normal_attack->Lever_Data);
        break;

    case 1:
        Normal_Attack(wk, normal_attack_b->Reaction, normal_attack_b->Lever_Data);
        break;

    case 2:
        Normal_Attack(wk, normal_attack_b_b->Reaction, normal_attack_b_b->Lever_Data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_lever_attack_2(
    PLW* wk, const Lever_Attack_Step* lever_attack, const Lever_Attack_Step* lever_attack_b, u16 lever_data
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, lever_attack->Reaction, lever_attack->Lever, lever_attack->Lever_Data);
        break;

    case 1:
        Lever_Attack(wk, lever_attack_b->Reaction, lever_attack_b->Lever, lever_attack_b->Lever_Data);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_normal_attack_j_command_attack_3(
    PLW* wk, s16 target_pos, const Normal_Attack_Step* normal_attack, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, normal_attack->Reaction, normal_attack->Lever_Data);
        break;

    case 2:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_etc_term_normal_attack_com_random_select(
    PLW* wk, const ETC_Term_Step* etc_term, const Normal_Attack_Step* normal_attack,
    const Com_Random_Select_Step* com_random_select
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, etc_term->Exit_No, etc_term->Next_Action, etc_term->Next_Menu);
        break;

    case 1:
        Normal_Attack(wk, normal_attack->Reaction, normal_attack->Lever_Data);
        break;

    case 2:
        Com_Random_Select(wk, com_random_select->p, com_random_select->Rnd_Type);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_command_attack_com_random_select(
    PLW* wk, const Normal_Attack_Step* normal_attack, const Command_Attack_Args* p,
    const Com_Random_Select_Step* com_random_select
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, normal_attack->Reaction, normal_attack->Lever_Data);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    case 2:
        Com_Random_Select(wk, com_random_select->p, com_random_select->Rnd_Type);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_normal_attack_9(
    PLW* wk, s16 target_pos, const EM_Term_Params* p, const Normal_Attack_Step* normal_attack
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, p);
        break;

    case 2:
        Normal_Attack(wk, normal_attack->Reaction, normal_attack->Lever_Data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_adjust_attack_lever_attack(
    PLW* wk, s16 reaction, const Adjust_Attack_Step* adjust_attack, u16 lever_data
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, reaction, 0x100);
        break;

    case 1:
        Adjust_Attack(wk, adjust_attack->Reaction, adjust_attack->Lever_Data);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_j_command_attack_2(
    PLW* wk, const Normal_Attack_Step* normal_attack, u16 lever_data, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, normal_attack->Reaction, normal_attack->Lever_Data);
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

void pattern_etc_term_check_sa_branch_unit_area(
    PLW* wk, const ETC_Term_Step* etc_term, s16 next_menu, const Branch_Menu_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, etc_term->Exit_No, etc_term->Next_Action, etc_term->Next_Menu);
        break;

    case 1:
        Check_SA(wk, 2, next_menu);
        break;

    case 2:
        Branch_Unit_Area(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_shell_term_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, &(Shell_Term_Params) { 0, 2, 1, -1, -1 });
        break;

    case 1:
        Jump_Attack_Term(wk, a);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_lever_off_shell_term_next_be_flip(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Off(wk);
        break;

    case 1:
        SHELL_Term(wk, &(Shell_Term_Params) { 2, 2, 1, -1, -1 });
        break;

    case 2:
        Next_Be_Flip(wk, 8);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_shell_term_jump_attack_term_j_command_attack(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, &(Shell_Term_Params) { 0, 2, 1, -1, -1 });
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args) { -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x400 });
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1C, 0xA, -1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
