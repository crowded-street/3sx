/**
 * @file pass19_3.c
 * COM Passive: Remy
 * Pattern scripts from Passive19_0123 on.
 */

#include "sf33rd/Source/Game/com/passive/pass19.h"
#include "sf33rd/Source/Game/com/passive/pass19_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive19_0123(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 2, -1, 0x20, 0x400});
}

void Passive19_0124(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 2, -1, 0x20, 0x20});
}

void Passive19_0125(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 2, -1, 0x20, 0x40});
}

void Passive19_0126(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 0x42);
}

void Passive19_0127(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Passive19_0128(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1F, 9, 0x700});
}

void Passive19_0129(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive19_0130(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive19_0131(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive19_0132(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x82, 0x83});
}

void Passive19_0133(PLW* wk) {
    pattern_check_ex_command_attack(wk, 0x84, &(Command_Attack_Args){8, 0x1E, 10, 0x700});
}

void Passive19_0134(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, 0x70});
}

void Passive19_0135(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 1, &(Command_Attack_Args){8, 0x1F, 10, 0x700});
}

void Passive19_0136(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1F, 10, 0x700});
}

void Passive19_0137(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7F80, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, 0x70}
    );
}

void Passive19_0138(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x22, &(Command_Attack_Args){8, 0x1F, 10, 0x700});
}

void Passive19_0139(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 0xD, 0x400, 2});
}

void Passive19_0140(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Passive19_0141(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 0x40);
}

void Passive19_0142(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive19_0143(PLW* wk) {
    pattern_provoke_com_random_select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x70});
}

void Passive19_0144(PLW* wk) {
    pattern_provoke_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0},
        &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x70}
    );
}

void Passive19_0145(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, 1);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0});
        break;

    case 2:
        Approach_Walk(wk, 0x93, 2);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x70}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0146(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, 1);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0147(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x85, &(Command_Attack_Args){8, 0x1F, 10, 0x700});
}

void Passive19_0148(PLW* wk) {
    pattern_jump_look(wk);
}

void Passive19_0149(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F70, -1, 8, 0x20, 1, -1, 0x20, 0x20});
}

void Passive19_0150(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F70, -0x7FB0, 8, 0x20, 2, -1, -0x7FB0, 0x20}
    );
}

void Passive19_0151(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x102);
}

void Passive19_0152(PLW* wk) {
    pattern_em_term_normal_attack_6(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
}

void Passive19_0153(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1}, 0x402);
}

void Passive19_0154(PLW* wk) {
    pattern_em_term_normal_attack_5(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
}

void Passive19_0155(PLW* wk) {
    pattern_wait_branch_unit_area(wk, 3, &(Branch_Menu_Args){6, 0x59, 0x59, 0x5A, 0x5B});
}

void Passive19_0156(PLW* wk) {
    pattern_check_ex_wait_j_command_attack(wk, 3, &(Command_Attack_Args){8, 0x1F, 9, 0x700});
}

void Passive19_0157(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x110);
}

void Passive19_0158(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x110);
}

void Passive19_0159(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x110);
}

void Passive19_0160(PLW* wk) {
    active_pattern_search_back_term_walk_wait(wk);
}

void Passive19_0161(PLW* wk) {
    active_pattern_search_back_term_walk_wait_2(wk);
}

void Passive19_0162(PLW* wk) {
    active_pattern_walk_search_back_term_walk(wk);
}

void Passive19_0163(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x200);
}

void Passive19_0164(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive19_0165(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive19_0166(PLW* wk) {
    pattern_em_term_sa_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1D, 8, -1}
    );
}

void Passive19_0167(PLW* wk) {
    active_pattern_command_attack_2(wk, &(Command_Attack_Args){0xC, 0x1D, 8, -1}, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive19_0168(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0x1E, 8, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive19_0169(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0x1D, 10, -1},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive19_0170(PLW* wk) {
    pattern_normal_attack_command_attack_4(wk, 8, 8, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive19_0171(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive19_0172(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive19_0173(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive19_0174(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive19_0175(PLW* wk) {
    pattern_provoke(wk, -1);
}
