/**
 * @file pass01_3.c
 * COM Passive: Alex
 * Pattern scripts from Passive01_0111 on.
 */

#include "sf33rd/Source/Game/com/passive/pass01.h"
#include "sf33rd/Source/Game/com/passive/pass01_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive01_0111(PLW* wk) {
    pattern_em_term_normal_attack_com_random_select(wk, 0x42);
}

void Passive01_0112(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x40);
}

void Passive01_0113(PLW* wk) {
    pattern_normal_attack_com_random_select_2(wk, 9, 0x42);
}

void Passive01_0114(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive01_0115(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20});
}

void Passive01_0116(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x20});
}

void Passive01_0117(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x100, 0});
}

void Passive01_0118(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Passive01_0119(PLW* wk) {
    pattern_lever_on_em_term_check_store_lever(wk, 0x1F);
}

void Passive01_0120(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 1, -1, 0x20, 0x400});
}

void Passive01_0121(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 1, -1, 0x20, 0x20});
}

void Passive01_0122(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 1, -1, 0x20, 0x40});
}

void Passive01_0123(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 2, -1, 0x20, 0x400});
}

void Passive01_0124(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 2, -1, 0x20, 0x20});
}

void Passive01_0125(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 2, -1, 0x20, 0x40});
}

void Passive01_0126(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 0x42);
}

void Passive01_0127(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Passive01_0128(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1F, 9, 0x700});
}

void Passive01_0129(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Passive01_0130(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Passive01_0131(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 0xA, -1});
}

void Passive01_0132(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x82, 0x83});
}

void Passive01_0133(PLW* wk) {
    pattern_check_ex_command_attack(wk, 0x84, &(Command_Attack_Args){8, 0x20, 0xA, 0x700});
}

void Passive01_0134(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, 0x70});
}

void Passive01_0135(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 1, &(Command_Attack_Args){8, 0x1F, 0xA, 0x700});
}

void Passive01_0136(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1F, 0xA, 0x700});
}

void Passive01_0137(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 0xA, 0x70}
    );
}

void Passive01_0138(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x22, &(Command_Attack_Args){8, 0x1F, 0xA, 0x700});
}

void Passive01_0139(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 0xA, 0x42, 2});
}

void Passive01_0140(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Passive01_0141(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 0x40);
}

void Passive01_0142(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive01_0143(PLW* wk) {
    pattern_provoke_com_random_select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x70});
}

void Passive01_0144(PLW* wk) {
    pattern_provoke_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0x59},
        &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x70}
    );
}

void Passive01_0145(PLW* wk) {
    pattern_provoke_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0xFFFF},
        &(Branch_Menu_Args){6, 0x85, 0x73, 0x92, 0x93}
    );
}

void Passive01_0146(PLW* wk) {
    pattern_provoke_pierce_on_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive01_0147(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x85, &(Command_Attack_Args){8, 0x1F, 0xA, 0x700});
}

void Passive01_0148(PLW* wk) {
    pattern_jump_look(wk);
}

void Passive01_0149(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F70, -1, 8, 0x200, 1, -1, 0x20, 0x200});
}

void Passive01_0150(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F70, -0x7FB0, 8, 0x20, 2, -1, -0x7FB0, 0x20}
    );
}

void Passive01_0151(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x102, 0x102);
}

void Passive01_0152(PLW* wk) {
    pattern_em_term_normal_attack_6(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
}

void Passive01_0153(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1}, 0x402);
}

void Passive01_0154(PLW* wk) {
    pattern_em_term_normal_attack_5(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
}

void Passive01_0155(PLW* wk) {
    pattern_wait_branch_unit_area(wk, 6, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
}

void Passive01_0156(PLW* wk) {
    pattern_check_ex_wait_j_command_attack(wk, 4, &(Command_Attack_Args){8, 0x1F, 9, 0x700});
}

void Passive01_0157(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x110);
}

void Passive01_0158(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x110);
}

void Passive01_0159(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x110);
}

void Passive01_0160(PLW* wk) {
    active_pattern_search_back_term_walk_wait(wk);
}

void Passive01_0161(PLW* wk) {
    active_pattern_search_back_term_walk_wait_2(wk);
}

void Passive01_0162(PLW* wk) {
    active_pattern_walk_search_back_term_walk(wk);
}

void Passive01_0163(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x40);
}

void Passive01_0164(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F80, -1, 0, 1, -1}, 0, 0x40);
}

void Passive01_0165(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0xA6, 0xA7, 0xA8, 1});
}

void Passive01_0166(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x21, 8, -1});
}

void Passive01_0167(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x21, 9, -1});
}

void Passive01_0168(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x21, 0xA, -1});
}

void Passive01_0169(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x21, 8, -1}
    );
}

void Passive01_0170(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x21, 9, -1}
    );
}

void Passive01_0171(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Passive01_0172(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 8, 0x202, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Passive01_0173(PLW* wk) {
    pattern_hi_jump_attack_term_command_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x100, 0, -0x7FA0, -1, 0x400},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Passive01_0174(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20});
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0175(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
