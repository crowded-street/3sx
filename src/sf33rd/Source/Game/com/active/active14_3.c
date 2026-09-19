/**
 * @file active14_3.c
 * COM Active: Akuma/Gouki
 * Pattern scripts from Pattern14_0138 on.
 */

#include "sf33rd/Source/Game/com/active/active14.h"
#include "sf33rd/Source/Game/com/active/active14_internal.h"
#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Pattern14_0138(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 1:
        Normal_Attack(wk, 0xc, 0x202);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){0xb, 0x20, 8, -1});
        break;

    case 4:
        Wait(wk, 5);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x2f, -1, 0x31, 0x7f});
        break;

    default:
        active_pattern_j_command_attack_from_step_6_2(wk);
        break;
    }
}

void Pattern14_0139(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 0xc, 0x202);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){0xb, 0x20, 9, -1});
        break;

    case 4:
        Wait(wk, 3);
        break;

    case 5:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1e, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0140(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){0xb, 0x20, 9, -1});
        break;

    case 4:
        Wait(wk, 3);
        break;

    case 5:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1e, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0141(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x89, 0x8A, 0x8B, 0x8C}, 0);
}

void Pattern14_0142(PLW* wk) {
    active_pattern_approach_walk_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args){2, 0x89, 0x8A, 0x8B, 0x8C});
}

void Pattern14_0143(PLW* wk) {
    active_pattern_turn_over_on_jump_attack_term_com_random_select(wk, &(Branch_Menu_Args){2, 0x89, 0x8A, 0x8B, 0x8C});
}

void Pattern14_0144(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x26, 0x27, 0x28, 0x29}, 0);
}

void Pattern14_0145(PLW* wk) {
    active_pattern_approach_walk_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args){2, 0x4C, 0x4D, 0x4E, 0x4F});
}

void Pattern14_0146(PLW* wk) {
    active_pattern_approach_walk_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args){2, 0x50, 0x51, 0x52, 0x53});
}

void Pattern14_0147(PLW* wk) {
    active_pattern_approach_walk_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args){2, 0x54, 0x55, 0x56, 0x57});
}

void Pattern14_0148(PLW* wk) {
    active_pattern_approach_walk_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args){2, 0x58, 0x59, 0x5A, 0x5B});
}

void Pattern14_0149(PLW* wk) {
    active_pattern_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){8, 0x8019, 0xA, -1},
        &(Command_Attack_Args){0xB, 0x20, 9, -1}
    );
}

void Pattern14_0150(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA_Full(wk, 6, 0x7C);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7F30, -1, 5, 2, 0});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x8019, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
