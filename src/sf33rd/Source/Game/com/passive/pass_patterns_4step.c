/**
 * @file pass_patterns_4step.c
 * COM Passive: pattern skeletons shared by every character
 *
 * The four-step patterns. Part 1 of 2, in name order.
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

void pattern_approach_walk_command_attack_com_random_select(PLW* wk, s16 target_pos, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}, 1);
        break;

    case 3:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_command_attack_2(
    PLW* wk, s16 target_pos, const Command_Attack_Args* p, const Branch_Menu_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1});
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    case 3:
        Com_Random_Select(wk, p_b, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_command_attack_3(
    PLW* wk, s16 target_pos, const Command_Attack_Args* p, const SA_Term_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1});
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    case 3:
        SA_Term(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_normal_attack_3(
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
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_sa_term(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_normal_attack_lever_attack(PLW* wk, s16 target_pos, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 2:
        Lever_Attack(wk, 9, lever, lever_data);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_normal_attack_sa_term(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_normal_attack_sa_term_2(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x20);
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_normal_attack_sa_term_3(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x20);
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_normal_attack_sa_term_4(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_wait_get_up_sa_term(
    PLW* wk, s16 target_pos, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_normal_attack_7(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, reaction, 0x100);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 3:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_normal_attack_sa_term(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_hi_jump_attack_normal_attack(PLW* wk, const Jump_Attack_Args* a, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_hi_jump_attack_term_normal_attack_3(PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 3:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_attack_term_normal_attack_sa_term(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40});
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x400);
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_lever_on_em_term_check_store_lever(PLW* wk, u16 tech_number) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 2:
        Check_Store_Lever(wk, tech_number, 1, -1);
        break;

    case 3:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_lever_on_em_term_check_store_lever_2(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -0x7FD8, 3, 1, -1});
        break;

    case 2:
        Check_Store_Lever(wk, 0x1C, 6, 0x3C);
        break;

    case 3:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_command_attack_5(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x220);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 3:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_command_attack_6(PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 3:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_jump_attack_term_sa_term(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -1, -1, -1});
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_jump_attack_term_sa_term_2(
    PLW* wk, s16 reaction, const SA_Term_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, 0x100);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40});
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_sa_term_normal_attack_2(PLW* wk, const SA_Term_Args* p, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_2(
    PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b, const Command_Attack_Args* p_b_b
) {
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

    case 3:
        Command_Attack(wk, p_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_approach_walk(PLW* wk, s16 target_pos, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
        break;

    case 2:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 3:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_em_term(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 3:
        Normal_Attack(wk, 9, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_normal_attack_2(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 2:
        Normal_Attack(wk, reaction, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_em_term_j_command_attack(
    PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b, const Command_Attack_Args* p_b_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        EM_Term(wk, p);
        break;

    case 2:
        J_Command_Attack(wk, p_b);
        break;

    case 3:
        J_Command_Attack(wk, p_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_em_term_normal_attack_2(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x42);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term_approach_walk_sa_term(
    PLW* wk, const SA_Term_Args* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, p);
        break;

    case 1:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 2:
        SA_Term(wk, p_b);
        break;

    case 3:
        Command_Attack(wk, p_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term_em_term_sa_term(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_pierce_on_command_attack(
    PLW* wk, s16 move_value, s16 next_menu, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, 6, next_menu);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    case 3:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_pierce_on_command_attack_2(
    PLW* wk, s16 move_value, s16 next_menu, const Branch_Menu_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, move_value, 6, next_menu);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, -1, -1});
        break;

    case 3:
        Com_Random_Select(wk, p, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_command_attack_wait_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Wait(wk, 0xe);
        break;

    case 7:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 0xb, 0x70});
        break;

    case 8:
        Wait(wk, 0xe);
        break;

    case 9:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 0xb, 0x70});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_jump_attack_term_normal_attack_2(
    PLW* wk, const Jump_Term_Args* a, s16 reaction, u16 lever_data
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, a);
        break;

    case 2:
        Normal_Attack(wk, reaction, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_wait_get_up_normal_attack_2(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
