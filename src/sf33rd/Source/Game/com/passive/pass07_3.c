/**
 * @file pass07_3.c
 * COM Passive: Ibuki
 * Pattern scripts from Passive07_0120 on.
 */

#include "sf33rd/Source/Game/com/passive/pass07.h"
#include "sf33rd/Source/Game/com/passive/pass07_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive07_0120(PLW* wk) {
    pattern_normal_attack_command_attack_4(wk, 9, 9, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive07_0121(PLW* wk) {
    pattern_normal_attack(wk, 9, 0x102);
}

void Passive07_0122(PLW* wk) {
    pattern_normal_attack_2(wk, 9, 0x102, 0x202);
}

void Passive07_0123(PLW* wk) {
    pattern_normal_attack_4(wk, 9, 9, 0x202);
}

void Passive07_0124(PLW* wk) {
    pattern_normal_attack_command_attack_3(wk, 9, 0xB, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive07_0125(PLW* wk) {
    pattern_normal_attack_sa_term_normal_attack(wk, &(SA_Term_Args){0x35, 0xFFFF, 0x37, 0});
}

void Passive07_0126(PLW* wk) {
    pattern_normal_attack_sa_term_normal_attack_2(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0}, 0x202);
}

void Passive07_0127(PLW* wk) {
    pattern_normal_attack_jump_attack_term_normal_attack(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0});
}

void Passive07_0128(PLW* wk) {
    pattern_normal_attack_jump_attack_term_sa_term(
        wk,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive07_0129(PLW* wk) {
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
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0130(PLW* wk) {
    pattern_normal_attack_jump_attack_term_sa_term_2(
        wk,
        9,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive07_0131(PLW* wk) {
    pattern_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Passive07_0132(PLW* wk) {
    pattern_jump_attack_term_normal_attack_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40},
        0x400,
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Passive07_0133(PLW* wk) {
    pattern_jump_attack_term_normal_attack_sa_term(
        wk,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Passive07_0134(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200},
        0x40,
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive07_0135(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive07_0136(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x3B, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1}, 0x402);
}

void Passive07_0137(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0138(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x100);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x20);
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0139(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term_4(
        wk,
        0,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive07_0140(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term(
        wk,
        0,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x20, 0xA, -1}
    );
}

void Passive07_0141(PLW* wk) {
    passive07_pattern_wait_get_up_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Passive07_0142(PLW* wk) {
    pattern_approach_walk_em_term_command_attack_3(
        wk,
        0x3B,
        &(Command_Attack_Args){0xC, 0x1E, 8, -1},
        &(SA_Term_Args){0x35, 0x36, 0x37, 0}
    );
}

void Passive07_0143(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_3(
        wk,
        0x3B,
        &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive07_0144(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack(
        wk,
        0x3B,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Passive07_0145(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 2:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, -1, -1});
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0146(PLW* wk) {
    pattern_pierce_on_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, -1, -1},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive07_0147(PLW* wk) {
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
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0148(PLW* wk) {
    pattern_pierce_on_command_attack_normal_attack_2(wk, 9, 0x202);
}

void Passive07_0149(PLW* wk) {
    pattern_pierce_on_command_attack_normal_attack_4(wk);
}

void Passive07_0150(PLW* wk) {
    pattern_pierce_on_command_attack_normal_attack_2(wk, 0xB, 0x402);
}

void Passive07_0151(PLW* wk) {
    pattern_pierce_on_command_attack_normal_attack(wk, &(SA_Term_Args){0x35, 0x36, 0xFFFF, 0});
}

void Passive07_0152(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x400);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0153(PLW* wk) {
    pattern_wait_get_up_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive07_0154(PLW* wk) {
    passive07_pattern_keep_away_wait_get_up(wk, 0, -1);
}

void Passive07_0155(PLW* wk) {
    passive07_pattern_keep_away_wait_get_up(wk, 2, -1);
}

void Passive07_0156(PLW* wk) {
    pattern_keep_away_wait_get_up_command_attack(wk, 0xC7, 0, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive07_0157(PLW* wk) {
    pattern_pierce_on_command_attack_jump_attack_term(
        wk,
        &(SA_Term_Args){0x35, 0x36, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive07_0158(PLW* wk) {
    pattern_pierce_on_command_attack_jump_attack_term_2(wk, &(SA_Term_Args){0x35, 0x36, 0xFFFF, 0});
}

void Passive07_0159(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term_4(
        wk,
        0x3B,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Passive07_0160(PLW* wk) {
    pattern_jump_attack_term_normal_attack_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202,
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Passive07_0161(PLW* wk) {
    pattern_jump_attack_term_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}
