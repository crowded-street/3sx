/**
 * @file com_patterns_6step.c
 * COM: pattern skeletons shared by every character
 *
 * The six-step patterns.
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

void active_pattern_approach_walk_jump_attack_term_normal_attack_2(PLW* wk, s16 option) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xbf, option);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args) { 0x8058, 0x8038, 0xb, 0x400, 0, 0x8080, -1, 0x400 });
        break;

    case 2:
        Normal_Attack(wk, 0xb, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args) { 0xc, 0x1f, 10, -1 });
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args) { 0x2f, 0x30, 0x31, 0x7f });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_normal_attack_pierce_on_j_command_attack(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, lever_data);
        break;

    case 1:
        Normal_Attack(wk, 0xc, 0x202);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args) { 0xb, 0x20, 8, -1 });
        break;

    case 4:
        Wait(wk, 5);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args) { 0x2f, -1, 0x31, 0x7f });
        break;

    default:
        active_pattern_j_command_attack_from_step_6_2(wk);
        break;
    }
}

void active_pattern_normal_attack_pierce_on_j_command_attack_2(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args) { 0xb, 0x20, 9, -1 });
        break;

    case 4:
        Wait(wk, 3);
        break;

    case 5:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1e, 9, -1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_pierce_on_command_attack_wait(
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
        Wait(wk, 0xe);
        break;

    case 3:
        Command_Attack(wk, p_b);
        break;

    case 4:
        Wait(wk, 0xe);
        break;

    case 5:
        Command_Attack(wk, p_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void active_pattern_search_back_term_walk_wait(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x30, 2, 0xF);
        break;

    case 1:
        Walk(wk, 1, 0x20, 0);
        break;

    case 2:
        Wait(wk, 3);
        break;

    case 3:
        Walk(wk, 0, 0x30, 0);
        break;

    case 4:
        Wait(wk, 9);
        break;

    case 5:
        Walk(wk, 0, 0x20, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_normal_attack_4(
    PLW* wk, s16 target_pos, const Command_Attack_Args* p, const SA_Term_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params) { -1, -0x7FF0, 6, 1, -1 });
        break;

    case 2:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 3:
        J_Command_Attack(wk, p);
        break;

    case 4:
        Wait(wk, 5);
        break;

    case 5:
        SA_Term(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_normal_attack_8(PLW* wk, const EM_Term_Params* p, u16 lever_data, s16 time) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, p);
        break;

    case 2:
        Normal_Attack(wk, 0xC, lever_data);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args) { 0xC, 0x1E, 10, -1 });
        break;

    case 4:
        Wait(wk, time);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args) { 0x2F, 0x30, 0x31, 0x7F });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_em_term_sa_term_4(PLW* wk, s16 target_pos, const SA_Term_Args* p, s16 reaction) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params) { -1, -0x7FF8, 6, 1, -1 });
        break;

    case 2:
        SA_Term(wk, p);
        break;

    case 3:
        Normal_Attack(wk, reaction, 0x12);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x42);
        break;

    case 5:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 6, 0x37, 0x37, 0x27, 0x27 }, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_approach_walk_turn_over_on_hi_jump_attack_term(
    PLW* wk, s16 target_pos, const Hi_Jump_Term_Args* a, const SA_Term_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Hi_Jump_Attack_Term(wk, a);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 4:
        SA_Term(wk, p);
        break;

    case 5:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 6, 0x77, 0x77, 0x78, 0x79 }, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_em_term_sa_term_normal_attack(PLW* wk, const SA_Term_Args* p, s16 reaction, s16 reaction_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params) { -1, -0x7FD8, 6, 1, -1 });
        break;

    case 1:
        SA_Term(wk, p);
        break;

    case 2:
        Normal_Attack(wk, reaction, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 4:
        Normal_Attack(wk, reaction_b, 0x40);
        break;

    case 5:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 6, 0x37, 0x37, 0x27, 0x27 }, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_attack_term_normal_attack_command_attack_4(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p, s16 rnd_type
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 4:
        Command_Attack(wk, p);
        break;

    case 5:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 6, 0x2D, 0xFF, 0xFF, 0xFF }, rnd_type);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_attack_term_normal_attack_command_attack_8(
    PLW* wk, const Command_Attack_Args* p, const SA_Term_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args) { -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400 });
        break;

    case 1:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, p);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_jump_attack_term_normal_attack_j_command_attack_3(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args) { -0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x200 });
        break;

    case 1:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 4:
        J_Command_Attack(wk, p);
        break;

    case 5:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 6, 0x2D, 0xFF, 0xFF, 0xFF }, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_normal_attack_jump_attack_term_normal_attack_2(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args) { -0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200 });
        break;

    case 2:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 5:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_approach_walk_jump_attack_term(PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Approach_Walk(wk, 0xB1, 2);
        break;

    case 2:
        Jump_Attack_Term(wk, a);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x42);
        break;

    case 4:
        J_Command_Attack(wk, p);
        break;

    case 5:
        ETC_Term(wk, 2, 6, 1);
        break;

    default:
        pattern_command_attack_from_step_6(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_jump_attack_term(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0, -1, -1 });
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args) { -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40 });
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x400);
        break;

    case 4:
        SA_Term(wk, p);
        break;

    case 5:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_jump_attack_term_2(PLW* wk, const SA_Term_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0, -1, -1 });
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args) { -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -1, -1, -1 });
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x12);
        break;

    case 4:
        SA_Term(wk, p);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_normal_attack(PLW* wk, const SA_Term_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0, -1, -1 });
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 4:
        SA_Term(wk, p);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_normal_attack_5(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0, -1, -1 });
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 5:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_wait(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args) { 0xe, 0x1e, 0xb, 0x70 });
        break;

    case 2:
        Wait(wk, 0xe);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args) { 0xe, 0x1e, 0xb, 0x70 });
        break;

    case 4:
        Wait(wk, 0xe);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args) { 0xe, 0x1e, 0xb, 0x70 });
        break;

    default:
        pattern_wait_command_attack_wait_from_step_6(wk);
        break;
    }
}

void pattern_pierce_on_hi_jump_attack_term_lever_on(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args) { -1, 0x40, 8, 0x400, 0, -0x7F90, -1, 0x200 });
        break;

    case 2:
        Lever_On(wk, 1, 0xFFFF);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 4:
        Normal_Attack(wk, 8, lever_data);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1C, 8, -1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_sa_term_wait_get_up_sa_term(PLW* wk, const SA_Term_Args* p, const SA_Term_Args* p_b, s16 reaction) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, p);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        SA_Term(wk, p_b);
        break;

    case 3:
        Normal_Attack(wk, reaction, 0x10);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        pattern_com_random_select_from_step_6(wk);
        break;
    }
}

void pattern_search_back_term_pierce_on_keep_away_2(PLW* wk, s16 target_pos, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, target_pos, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 5:
        Branch_Unit_Area(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_search_back_term_pierce_on_keep_away_5(PLW* wk, s16 target_pos) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, target_pos, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        pattern_normal_attack_from_step_6(wk);
        break;
    }
}
