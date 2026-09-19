/**
 * @file pass12_3.c
 * COM Passive: Sean
 * Pattern scripts from Passive12_0127 on.
 */

#include "sf33rd/Source/Game/com/passive/pass12.h"
#include "sf33rd/Source/Game/com/passive/pass12_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive12_0127(PLW* wk) {
    pattern_normal_attack_jump_attack_term_normal_attack(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0});
}

void Passive12_0128(PLW* wk) {
    pattern_normal_attack_jump_attack_term_sa_term(
        wk,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1C, 0x4009, -1}
    );
}

void Passive12_0129(PLW* wk) {
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
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0x4008, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0130(PLW* wk) {
    pattern_normal_attack_jump_attack_term_sa_term_2(
        wk,
        9,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0131(PLW* wk) {
    pattern_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x30, 0xFFFF, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0132(PLW* wk) {
    pattern_jump_attack_term_normal_attack_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40},
        0x400,
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0133(PLW* wk) {
    pattern_jump_attack_term_normal_attack_sa_term(
        wk,
        &(SA_Term_Args){0x30, -1, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0134(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200},
        0x40,
        &(Command_Attack_Args){8, 0x1C, 0x400A, -1}
    );
}

void Passive12_0135(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1D, 9, -1}
    );
}

void Passive12_0136(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x37, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1}, 0x402);
}

void Passive12_0137(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0138(PLW* wk) {
    pattern_wait_get_up_normal_attack_j_command_attack(wk, 0x42, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive12_0139(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term_4(
        wk,
        -1,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1C, 0x400A, -1}
    );
}

void Passive12_0140(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term(
        wk,
        -1,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 0xA, 0x70}
    );
}

void Passive12_0141(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0142(PLW* wk) {
    pattern_approach_walk_em_term_command_attack_3(
        wk,
        0x37,
        &(Command_Attack_Args){0xC, 0x1C, 0x400A, -1},
        &(SA_Term_Args){0x30, 0x31, 0x32, 0}
    );
}

void Passive12_0143(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_3(
        wk,
        0x37,
        &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive12_0144(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack(
        wk,
        0x37,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1C, 0x400A, -1}
    );
}

void Passive12_0145(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_5(
        wk,
        0x37,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive12_0146(PLW* wk) {
    pattern_pierce_on_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, -1, -1},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive12_0147(PLW* wk) {
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
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0148(PLW* wk) {
    pattern_pierce_on_command_attack_normal_attack_2(wk, 9, 0x202);
}

void Passive12_0149(PLW* wk) {
    pattern_pierce_on_command_attack_normal_attack_4(wk);
}

void Passive12_0150(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x30, 0xFFFF, 0x32, 0});
        break;

    case 4:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0151(PLW* wk) {
    pattern_pierce_on_command_attack_normal_attack(wk, &(SA_Term_Args){0x30, 0xFFFF, 0x32, 0});
}

void Passive12_0152(PLW* wk) {
    pattern_normal_attack_com_random_select(wk, 0x200, &(Branch_Menu_Args){6, 0xA9, 0xA8, 0xB9, 0x86}, 3);
}

void Passive12_0153(PLW* wk) {
    pattern_wait_get_up_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive12_0154(PLW* wk) {
    pattern_keep_away_wait_get_up_2(wk, 0);
}

void Passive12_0155(PLW* wk) {
    pattern_keep_away_wait_get_up_2(wk, 3);
}

void Passive12_0156(PLW* wk) {
    pattern_keep_away_wait_get_up_command_attack(wk, 0xBF, -1, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
}

void Passive12_0157(PLW* wk) {
    pattern_pierce_on_command_attack_jump_attack_term(
        wk,
        &(SA_Term_Args){0x30, 0xFFFF, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0158(PLW* wk) {
    pattern_pierce_on_command_attack_jump_attack_term_2(wk, &(SA_Term_Args){0x30, 0xFFFF, 0x32, 0});
}

void Passive12_0159(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term_4(
        wk,
        0x37,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive12_0160(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        &(Command_Attack_Args){8, 0x1C, 0x400A, -1}
    );
}

void Passive12_0161(PLW* wk) {
    pattern_jump_attack_term_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive12_0162(PLW* wk) {
    pattern_normal_attack_j_command_attack_2(wk, 9, 0x10, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive12_0163(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1C, 0x400A, -1},
        2
    );
}

void Passive12_0164(PLW* wk) {
    pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200},
        8,
        0x402
    );
}

void Passive12_0165(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200},
        0x402,
        &(Command_Attack_Args){8, 0x1C, 0x400A, -1}
    );
}

void Passive12_0166(PLW* wk) {
    pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200},
        8,
        0x402
    );
}

void Passive12_0167(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200},
        0x402,
        &(Command_Attack_Args){8, 0x1C, 0x400A, -1}
    );
}

void Passive12_0168(PLW* wk) {
    pattern_walk(wk, -1);
}

void Passive12_0169(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 8, 0x400, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
}

void Passive12_0170(PLW* wk) {
    pattern_keep_away_wait_get_up_2(wk, 3);
}
