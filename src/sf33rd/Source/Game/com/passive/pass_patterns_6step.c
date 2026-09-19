/**
 * @file pass_patterns_6step.c
 * COM Passive: pattern skeletons shared by every character
 *
 * The six-step patterns.
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

void pattern_approach_walk_em_term_normal_attack_4(
    PLW* wk, s16 target_pos, const Command_Attack_Args* p, const SA_Term_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
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
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}, rnd_type);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void pattern_pierce_on_command_attack_jump_attack_term(PLW* wk, const SA_Term_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40});
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
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -1, -1, -1});
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
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
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

void pattern_pierce_on_command_attack_wait(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 0xb, 0x70});
        break;

    case 2:
        Wait(wk, 0xe);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 0xb, 0x70});
        break;

    case 4:
        Wait(wk, 0xe);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 0xb, 0x70});
        break;

    default:
        pattern_wait_command_attack_wait_from_step_6(wk);
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

void pattern_search_back_term_walk_wait(PLW* wk) {
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
