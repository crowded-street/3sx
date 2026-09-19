/**
 * @file pass08_3.c
 * COM Passive: Elena
 * Pattern scripts from Passive08_0120 on.
 */

#include "sf33rd/Source/Game/com/passive/pass08.h"
#include "sf33rd/Source/Game/com/passive/pass08_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive08_0120(PLW* wk) {
    pattern_normal_attack_command_attack_4(wk, 9, 9, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive08_0121(PLW* wk) {
    active_pattern_normal_attack(wk, 9, 0x102);
}

void Passive08_0122(PLW* wk) {
    pattern_normal_attack_2(wk, 9, 0x102, 0x202);
}

void Passive08_0123(PLW* wk) {
    pattern_normal_attack_4(wk, 9, 9, 0x202);
}

void Passive08_0124(PLW* wk) {
    pattern_normal_attack_command_attack_3(wk, 9, 0xB, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive08_0125(PLW* wk) {
    pattern_normal_attack_sa_term_normal_attack(wk, &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0});
}

void Passive08_0126(PLW* wk) {
    pattern_normal_attack_sa_term_normal_attack_2(wk, &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0}, 0x402);
}

void Passive08_0127(PLW* wk) {
    pattern_normal_attack_jump_attack_term_normal_attack(wk, &(SA_Term_Args){0x39, 0x3A, 0x3B, 0});
}

void Passive08_0128(PLW* wk) {
    pattern_normal_attack_jump_attack_term_sa_term(
        wk,
        &(SA_Term_Args){0x39, 0x3A, 0x3B, 0},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive08_0129(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200});
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
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0130(PLW* wk) {
    pattern_normal_attack_jump_attack_term_sa_term_2(
        wk,
        8,
        &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive08_0131(PLW* wk) {
    pattern_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive08_0132(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40},
        0x400,
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive08_0133(PLW* wk) {
    pattern_jump_attack_term_normal_attack_sa_term(
        wk,
        &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive08_0134(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0135(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive08_0136(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x3F, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1}, 0x402);
}

void Passive08_0137(PLW* wk) {
    pattern_wait_get_up_normal_attack_j_command_attack(wk, 0x40, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive08_0138(PLW* wk) {
    pattern_wait_get_up_normal_attack_j_command_attack(wk, 0x40, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive08_0139(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term_4(
        wk,
        -1,
        &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive08_0140(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term(
        wk,
        -1,
        &(SA_Term_Args){0x39, 0x3A, 0x3B, 0},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive08_0141(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 10, -1});
        break;

    case 5:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    default:
        pattern_command_attack_from_step_6_2(wk);
        break;
    }
}

void Passive08_0142(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1C, 8, -1});
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x39, 0x3A, 0x3B, 0});
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    default:
        pattern_command_attack_from_step_6_2(wk);
        break;
    }
}

void Passive08_0143(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    default:
        pattern_command_attack_from_step_6_2(wk);
        break;
    }
}

void Passive08_0144(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack(
        wk,
        0x3F,
        &(SA_Term_Args){0x39, 0x3A, 0x3B, 0},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Passive08_0145(PLW* wk) {
    pattern_approach_walk_em_term_search_back_term(wk, 0x3F, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive08_0146(PLW* wk) {
    pattern_pierce_on_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, -1, -1},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive08_0147(PLW* wk) {
    pattern_pierce_on_command_attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Passive08_0148(PLW* wk) {
    active_pattern_command_attack_2(wk, &(Command_Attack_Args){0xE, 0x20, 0xA, -1}, &(Command_Attack_Args){0xE, 0x20, 0xA, -1});
}

void Passive08_0149(PLW* wk) {
    active_pattern_command_attack_2(wk, &(Command_Attack_Args){0xE, 0x20, 9, -1}, &(Command_Attack_Args){0xE, 0x20, 0xA, -1});
}

void Passive08_0150(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0x95);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xE, 0x20, 0xA, 0x700});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0151(PLW* wk) {
    pattern_pierce_on_command_attack_normal_attack(wk, &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0});
}

void Passive08_0152(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0153(PLW* wk) {
    pattern_wait_get_up_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x39, 0x3A, 0x3B, 0},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive08_0154(PLW* wk) {
    pattern_keep_away_wait_get_up_2(wk, 0);
}

void Passive08_0155(PLW* wk) {
    pattern_keep_away_wait_get_up_2(wk, 3);
}

void Passive08_0156(PLW* wk) {
    pattern_keep_away_wait_get_up_command_attack(wk, 0xBF, -1, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Passive08_0157(PLW* wk) {
    pattern_pierce_on_command_attack_jump_attack_term(
        wk,
        &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive08_0158(PLW* wk) {
    pattern_pierce_on_command_attack_jump_attack_term_2(wk, &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0});
}

