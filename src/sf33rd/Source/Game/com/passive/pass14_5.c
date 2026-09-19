/**
 * @file pass14_5.c
 * COM Passive: Akuma/Gouki
 * Pattern scripts from Passive14_0209 on.
 */

#include "sf33rd/Source/Game/com/passive/pass14.h"
#include "sf33rd/Source/Game/com/passive/pass14_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive14_0209(PLW* wk) {
    passive14_pattern_approach_walk_jump_command_attack_term_com_random_select(
        wk,
        0xBF,
        &(Branch_Menu_Args){2, 0x50, 0x51, 0x52, 0x53}
    );
}

void Passive14_0210(PLW* wk) {
    passive14_pattern_approach_walk_jump_command_attack_term_com_random_select(
        wk,
        0xAD,
        &(Branch_Menu_Args){2, 0x54, 0x55, 0x56, 0x57}
    );
}

void Passive14_0211(PLW* wk) {
    passive14_pattern_approach_walk_jump_command_attack_term_com_random_select(
        wk,
        0xBF,
        &(Branch_Menu_Args){2, 0x58, 0x59, 0x5A, 0x5B}
    );
}

void Passive14_0212(PLW* wk) {
    pattern_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0x32, 0xFFFF, 0xFFFF, 0xBF},
        &(Branch_Menu_Args){6, 0x71, 0x71, 0x72, 0x73},
        2
    );
}

void Passive14_0213(PLW* wk) {
    passive14_pattern_em_term_com_random_select(wk, &(Branch_Menu_Args){2, 0x4C, 0x4D, 0x4E, 0x4F}, 0);
}

void Passive14_0214(PLW* wk) {
    passive14_pattern_em_term_com_random_select(wk, &(Branch_Menu_Args){2, 0x50, 0x51, 0x52, 0x53}, 0);
}

void Passive14_0215(PLW* wk) {
    passive14_pattern_em_term_com_random_select(wk, &(Branch_Menu_Args){2, 0x54, 0x55, 0x56, 0x57}, 0);
}

void Passive14_0216(PLW* wk) {
    passive14_pattern_em_term_com_random_select(wk, &(Branch_Menu_Args){2, 0x58, 0x59, 0x5A, 0x5B}, 0);
}

static void passive14_pattern_command_attack_turn_over_on_jump_attack_term(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, -1, -1});
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40});
        break;

    case 3:
        Com_Random_Select(wk, p, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0217(PLW* wk) {
    passive14_pattern_command_attack_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args){2, 0x4C, 0x4D, 0x4E, 0x4F});
}

void Passive14_0218(PLW* wk) {
    passive14_pattern_command_attack_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args){2, 0x50, 0x51, 0x52, 0x53});
}

void Passive14_0219(PLW* wk) {
    passive14_pattern_command_attack_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args){2, 0x54, 0x55, 0x56, 0x57});
}

void Passive14_0220(PLW* wk) {
    passive14_pattern_command_attack_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args){2, 0x58, 0x59, 0x5A, 0x5B});
}

void Passive14_0221(PLW* wk) {
    pattern_search_back_term_command_attack_sa_term_2(wk, 0x300, 0xE1, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive14_0222(PLW* wk) {
    pattern_search_back_term_command_attack_sa_term_2(wk, 0x80, 0xE1, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive14_0223(PLW* wk) {
    pattern_search_back_term_command_attack_sa_term_2(wk, 0x200, 0xE2, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive14_0224(PLW* wk) {
    pattern_search_back_term_command_attack_sa_term_2(wk, 0x100, 0xE2, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

static void passive14_pattern_command_attack_sa_term_em_term(
    PLW* wk, const Command_Attack_Args* p, const SA_Term_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        SA_Term(wk, p_b);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 1});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0225(PLW* wk) {
    passive14_pattern_command_attack_sa_term_em_term(
        wk,
        &(Command_Attack_Args){8, 0x1C, 10, -1},
        &(SA_Term_Args){0x34, 0x34, 0x34, 0x47}
    );
}

void Passive14_0226(PLW* wk) {
    passive14_pattern_command_attack_sa_term_em_term(
        wk,
        &(Command_Attack_Args){8, 0x1C, 8, -1},
        &(SA_Term_Args){0x34, 0x34, 0x34, 0x47}
    );
}

void Passive14_0227(PLW* wk) {
    passive14_pattern_command_attack_sa_term_em_term(
        wk,
        &(Command_Attack_Args){8, 0x1C, 10, -1},
        &(SA_Term_Args){0x2F, 0x30, 0x31, 0x47}
    );
}

void Passive14_0228(PLW* wk) {
    passive14_pattern_command_attack_sa_term_em_term(
        wk,
        &(Command_Attack_Args){8, 0x1C, 8, -1},
        &(SA_Term_Args){0x2F, 0x30, 0x31, 0x47}
    );
}

void Passive14_0229(PLW* wk) {
    passive14_pattern_command_attack_sa_term_em_term(
        wk,
        &(Command_Attack_Args){8, 0x22, 8, -1},
        &(SA_Term_Args){0x34, 0x34, 0x34, 0x47}
    );
}

void Passive14_0230(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 1});
        break;

    case 1:
        Check_SA_Full(wk, 6, 0x11);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x34, 0x34, 0x34, 0x7F});
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
        break;

    default:
        pattern_wait_j_command_attack_from_step_6(wk);
        break;
    }
}

void Passive14_0231(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Check_SA_Full(wk, 6, 0x11);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x34, 0x34, 0x34, 0xBF});
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
        break;

    default:
        pattern_wait_j_command_attack_from_step_6(wk);
        break;
    }
}

void Passive14_0232(PLW* wk) {
    pattern_approach_walk_look(wk, 3);
}

void Passive14_0233(PLW* wk) {
    pattern_approach_walk_look(wk, 2);
}

void Passive14_0234(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Attack_Complete(wk, 1, 1);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 1});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0235(PLW* wk) {
    passive14_pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 6, 1},
        &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F}
    );
}

void Passive14_0236(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F68, -1, 6, 6, 1});
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 4, 6, 0xEB});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1F, 10, -1});
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;
    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0237(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 6, 1},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Passive14_0238(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0239(PLW* wk) {
    pattern_jump(wk, 0);
}

void Passive14_0240(PLW* wk) {
    pattern_wait_get_up_com_random_select(wk, 3, 0, &(Branch_Menu_Args){2, 0x84, 0x85, 0x86, 0x87});
}

static void passive14_pattern_approach_walk_wait_get_up_turn_over_on(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x57, 3);
        break;

    case 1:
        Wait_Get_Up(wk, 3, 0);
        break;

    case 2:
        Turn_Over_On(wk);
        break;

    case 3:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40});
        break;

    case 4:
        Com_Random_Select(wk, p, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0241(PLW* wk) {
    passive14_pattern_approach_walk_wait_get_up_turn_over_on(wk, &(Branch_Menu_Args){2, 0x89, 0x8A, 0x8B, 0x8C});
}

void Passive14_0242(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, 0);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40});
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 0x89, 0x8A, 0x8B, 0x8C}, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0243(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x26, 0x27, 0x28, 0x29}, 0);
}

void Passive14_0244(PLW* wk) {
    passive14_pattern_approach_walk_wait_get_up_turn_over_on(wk, &(Branch_Menu_Args){2, 0x4C, 0x4D, 0x4E, 0x4F});
}

void Passive14_0245(PLW* wk) {
    passive14_pattern_approach_walk_wait_get_up_turn_over_on(wk, &(Branch_Menu_Args){2, 0x50, 0x51, 0x52, 0x53});
}

void Passive14_0246(PLW* wk) {
    passive14_pattern_approach_walk_wait_get_up_turn_over_on(wk, &(Branch_Menu_Args){2, 0x54, 0x55, 0x56, 0x57});
}

void Passive14_0247(PLW* wk) {
    passive14_pattern_approach_walk_wait_get_up_turn_over_on(wk, &(Branch_Menu_Args){2, 0x58, 0x59, 0x5A, 0x5B});
}

void Passive14_0248(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x89, 0x8A, 0x8B, 0x8C}, 0);
}

void Passive14_0249(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x402, &(Command_Attack_Args){0xC, 0x1F, 0xA, -1});
}

void Passive14_0250(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x77, 0x77, 0x78, 0x79}, 2);
}

void Passive14_0251(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x8019, 0xA, -1});
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){0xB, 0x20, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0252(PLW* wk) {
    pattern_check_sa_full_em_term_command_attack(wk);
}
