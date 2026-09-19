/**
 * @file pass00_3.c
 * COM Passive: Gill
 * Pattern scripts from Passive00_0112 on.
 */

#include "sf33rd/Source/Game/com/passive/pass00.h"
#include "sf33rd/Source/Game/com/passive/pass00_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive00_0112(PLW* wk) {
    pattern_pierce_on_em_term_normal_attack_3(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Passive00_0113(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_BOSS(wk, 6, 0x70);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FA8, 6, 1, -1});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x42);
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1F, 10, -1});
        break;

    case 5:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0114(PLW* wk) {
    pattern_pierce_on_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, 0x30, 7, 1, -1},
        &(Command_Attack_Args){0xB, 0x1F, 0xA, -1},
        &(Command_Attack_Args){0xB, 0x1F, 0xA, -1}
    );
}

void Passive00_0115(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x42);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){0xb, 0x1f, 10, -1});
        break;

    case 4:
        ETC_Term(wk, 2, 6, 1);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x42);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0116(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x10, 8, 0x400, 1, -1, 0x10, 0x400});
}

void Passive00_0117(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x10, 8, 0x200, 1, -1, 0x10, 0x20});
}

void Passive00_0118(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x10, 8, 0x20, 1, -1, 0x10, 0x20});
}

void Passive00_0119(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x10, 8, 0x400, 0, -1, 0x10, 0x400});
}

void Passive00_0120(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x10, 8, 0x200, 0, -1, 0x10, 0x20});
}

void Passive00_0121(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x10, 8, 0x20, 0, -1, 0x10, 0x20});
}

void Passive00_0122(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 0x42);
}

void Passive00_0123(PLW* wk) {
    pattern_pierce_on_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 9, 0x40);
}

void Passive00_0124(PLW* wk) {
    pattern_pierce_on_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 8, 0x400);
}

void Passive00_0125(PLW* wk) {
    pattern_search_back_term_walk_normal_attack(wk, 0x22, 0x400);
}

void Passive00_0126(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Passive00_0127(PLW* wk) {
    pattern_search_back_term_walk_normal_attack(wk, 0x7E, 0x40);
}

void Passive00_0128(PLW* wk) {
    pattern_em_term_normal_attack_4(wk, &(EM_Term_Params){-0x7F80, -0x7FB0, 6, 1, -1});
}

void Passive00_0129(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x40, 6, 0x40);
        break;

    case 1:
        Walk(wk, 1, 0x10, 0);
        break;

    case 2:
        Lever_Attack(wk, 8, 1, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0130(PLW* wk) {
    pattern_search_back_term_walk_em_term(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive00_0131(PLW* wk) {
    pattern_approach_walk_normal_attack_3(wk, 0x71, 8, 0x400);
}

void Passive00_0132(PLW* wk) {
    active_pattern_sa_term_command_attack(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0}, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive00_0133(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Passive00_0134(PLW* wk) {
    pattern_wait_attack_complete(wk, 0);
}

void Passive00_0135(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive00_0136(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x200);
}

void Passive00_0137(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x22);
}

void Passive00_0138(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 5);
        break;

    case 1:
        Lever_Attack(wk, 8, 1, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0139(PLW* wk) {
    pattern_jump_look(wk);
}

void Passive00_0140(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F70, -1, 8, 0x40, 1, -1, 0x20, 0x40});
}

void Passive00_0141(PLW* wk) {
    passive00_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F70, -0x7FC0, 8, 0x40, 2, -1, -0x7FB0, 0x40},
        0x200
    );
}

void Passive00_0142(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F70, -0x7FC0, 9, 0x40, 2, -1, -1, -1},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive00_0143(PLW* wk) {
    active_pattern_wait(wk, 0xA);
}

void Passive00_0144(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0x42);
}

void Passive00_0145(PLW* wk) {
    pattern_em_term_normal_attack_5(wk, &(EM_Term_Params){-0x7F90, -1, 6, 1, -1});
}

void Passive00_0146(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1}, 0x200);
}

void Passive00_0147(PLW* wk) {
    pattern_em_term_normal_attack_4(wk, &(EM_Term_Params){-0x7F90, -1, 6, 1, -1});
}

void Passive00_0148(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 0xB, 0x400, 1, -0x7F90, -1, 0x400});
}

void Passive00_0149(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Passive00_0150(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F90, -1, 6, 1, -1}, 0x42);
}

void Passive00_0151(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F90, -1, 6, 1, -1}, 0x400);
}

void Passive00_0152(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x47, 0x400);
}

void Passive00_0153(PLW* wk) {
    passive00_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x59, 0x6E, 0x89, 0x98});
}

void Passive00_0154(PLW* wk) {
    passive00_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x6C, 0x9B, 0x5A, 0x98});
}

void Passive00_0155(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
}

void Passive00_0156(PLW* wk) {
    pattern_wait_command_attack(wk, 5, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive00_0157(PLW* wk) {
    passive00_pattern_etc_term_command_attack(wk, 0x9A, &(Command_Attack_Args){8, 0x1E, 8, -1});
}
