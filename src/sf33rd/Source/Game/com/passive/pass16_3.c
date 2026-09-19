/**
 * @file pass16_3.c
 * COM Passive: Makoto
 * Pattern scripts from Passive16_0111 on.
 */

#include "sf33rd/Source/Game/com/passive/pass16.h"
#include "sf33rd/Source/Game/com/passive/pass16_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive16_0111(PLW* wk) {
    pattern_em_term_normal_attack_com_random_select(wk, 0x402);
}

void Passive16_0112(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x40);
}

void Passive16_0113(PLW* wk) {
    pattern_normal_attack_com_random_select_2(wk, 9, 0x402);
}

void Passive16_0114(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1}
    );
}

void Passive16_0115(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20});
}

void Passive16_0116(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x20});
}

void Passive16_0117(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x100, 0});
}

void Passive16_0118(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Passive16_0119(PLW* wk) {
    pattern_lever_on_em_term_check_store_lever(wk, 0x1F);
}

void Passive16_0120(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 1, -1, 0x20, 0x400});
}

void Passive16_0121(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 1, -1, 0x20, 0x20});
}

void Passive16_0122(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 1, -1, 0x20, 0x40});
}

void Passive16_0123(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 2, -1, 0x20, 0x400});
}

void Passive16_0124(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 2, -1, 0x20, 0x20});
}

void Passive16_0125(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 2, -1, 0x20, 0x40});
}

void Passive16_0126(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 8, 0x402);
}

void Passive16_0127(PLW* wk) {
    pattern_lever_attack_2(wk, &(Lever_Attack_Step){ 0xC, 0, 0x40 }, &(Lever_Attack_Step){ 0xC, 0, 0x40 }, 0x40);
}

void Passive16_0128(PLW* wk) {
    passive16_pattern_check_ex_jump_command_attack_term(
        wk,
        0x36,
        &(JCA_Term_Args){8, 0x2E, 9, 0x700, -0x7FA0, -0x7FA8, 0, -1, 0x30, 0x400}
    );
}

void Passive16_0129(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive16_0130(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive16_0131(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive16_0132(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x82, 0x83});
}

void Passive16_0133(PLW* wk) {
    pattern_check_ex_command_attack(wk, 0x84, &(Command_Attack_Args){8, 0x1D, 10, 0x70});
}

void Passive16_0134(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 10, 0x70});
}

void Passive16_0135(PLW* wk) {
    passive16_pattern_check_ex_jump_command_attack_term(
        wk,
        1,
        &(JCA_Term_Args){8, 0x2E, 0xA, 0x700, -0x7FA0, -0x7FA8, 0, -1, 0x30, 0x400}
    );
}

void Passive16_0136(PLW* wk) {
    passive16_pattern_check_ex_jump_command_attack_term(
        wk,
        0x36,
        &(JCA_Term_Args){8, 0x2E, 9, 0x700, -0x7FA0, -0x7FA8, 0, -1, 0x30, 0x400}
    );
}

void Passive16_0137(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, 0x70}
    );
}

void Passive16_0138(PLW* wk) {
    passive16_pattern_check_ex_jump_command_attack_term(
        wk,
        0x22,
        &(JCA_Term_Args){8, 0x2E, 0xA, 0x700, -0x7FA0, -0x7FA8, 2, -1, 0x30, 0x400}
    );
}

void Passive16_0139(PLW* wk) {
    active_pattern_jump_command_attack_term(
        wk,
        &(JCA_Term_Args){8, 0x2E, 0xA, 0x700, -0x7FA0, -0x7FA8, 2, -1, 0x30, 0x400}
    );
}

void Passive16_0140(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Passive16_0141(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 8, 0x40);
}

void Passive16_0142(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive16_0143(PLW* wk) {
    pattern_provoke_com_random_select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x70});
}

void Passive16_0144(PLW* wk) {
    pattern_provoke_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0},
        &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x70}
    );
}

void Passive16_0145(PLW* wk) {
    pattern_provoke_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0xFFFF},
        &(Branch_Menu_Args){6, 0x85, 0x73, 0x92, 0x93}
    );
}

void Passive16_0146(PLW* wk) {
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
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    case 5:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 3, 0x38, 0x44, 0x45}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16_0147(PLW* wk) {
    passive16_pattern_check_ex_jump_command_attack_term(
        wk,
        0x85,
        &(JCA_Term_Args){8, 0x2E, 0xA, 0x700, -0x7FA0, -0x7FA8, 0, -1, 0x30, 0x400}
    );
}

void Passive16_0148(PLW* wk) {
    pattern_jump_look(wk);
}

void Passive16_0149(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F70, -1, 8, 0x20, 1, -1, 0x20, 0x20});
}

void Passive16_0150(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F70, -0x7FB0, 8, 0x20, 2, -1, -0x7FB0, 0x20}
    );
}

void Passive16_0151(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x102);
}

void Passive16_0152(PLW* wk) {
    pattern_em_term_normal_attack_6(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
}

void Passive16_0153(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1}, 8, 0x402);
}

void Passive16_0154(PLW* wk) {
    pattern_em_term_normal_attack_5(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
}

void Passive16_0155(PLW* wk) {
    pattern_wait_branch_unit_area(wk, 6, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
}

void Passive16_0156(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0x9B);
        break;

    case 1:
        Wait(wk, 4);
        break;

    case 2:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x2E, 9, 0x700, -0x7FA0, 0x58, 2, -1, 0x30, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16_0157(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x110);
}

void Passive16_0158(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x110);
}

void Passive16_0159(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x110);
}

void Passive16_0160(PLW* wk) {
    active_pattern_search_back_term_walk_wait(wk);
}

void Passive16_0161(PLW* wk) {
    active_pattern_search_back_term_walk_wait_2(wk);
}

void Passive16_0162(PLW* wk) {
    active_pattern_walk_search_back_term_walk(wk);
}

void Passive16_0163(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Passive16_0164(PLW* wk) {
    pattern_pierce_on_command_attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
}

void Passive16_0165(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Passive16_0166(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0xA4, &(Command_Attack_Args){8, 0x1C, 10, 0x70});
}
