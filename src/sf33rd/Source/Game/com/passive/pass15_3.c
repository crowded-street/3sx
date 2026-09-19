/**
 * @file pass15_3.c
 * COM Passive: Chun-Li
 * Pattern scripts from Passive15_0119 on.
 */

#include "sf33rd/Source/Game/com/passive/pass15.h"
#include "sf33rd/Source/Game/com/passive/pass15_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive15_0119(PLW* wk) {
    pattern_lever_on_em_term_check_store_lever(wk, 0x1F);
}

void Passive15_0120(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 1, -1, 0x20, 0x400});
}

void Passive15_0121(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 1, -1, 0x20, 0x20});
}

void Passive15_0122(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 1, -1, 0x20, 0x40});
}

void Passive15_0123(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 2, -1, 0x20, 0x400});
}

void Passive15_0124(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 2, -1, 0x20, 0x20});
}

void Passive15_0125(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 2, -1, 0x20, 0x40});
}

void Passive15_0126(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 8, 0x402);
}

void Passive15_0127(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x40);
}

void Passive15_0128(PLW* wk) {
    pattern_check_ex_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1C, 9, 0x700});
}

void Passive15_0129(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive15_0130(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive15_0131(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive15_0132(PLW* wk) {
    pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x82, 0x83});
}

void Passive15_0133(PLW* wk) {
    pattern_check_ex_command_attack(wk, 0x84, &(Command_Attack_Args){8, 0x1C, 10, 0x700});
}

void Passive15_0134(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 10, 0x70});
}

void Passive15_0135(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 1, &(Command_Attack_Args){8, 0x1C, 10, 0x700});
}

void Passive15_0136(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1C, 10, 0x700});
}

void Passive15_0137(PLW* wk) {
    pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, 0x70}
    );
}

void Passive15_0138(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x22, &(Command_Attack_Args){8, 0x1F, 10, 0x700});
}

void Passive15_0139(PLW* wk) {
    pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 10, 0x42, 2});
}

void Passive15_0140(PLW* wk) {
    pattern_wait(wk, 0);
}

void Passive15_0141(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 8, 0x40);
}

void Passive15_0142(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive15_0143(PLW* wk) {
    pattern_provoke_com_random_select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x8A});
}

void Passive15_0144(PLW* wk) {
    pattern_provoke_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0},
        &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x8A}
    );
}

void Passive15_0145(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, 1);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 2:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x8A}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive15_0146(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, 1);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive15_0147(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x85, &(Command_Attack_Args){8, 0x1C, 10, 0x700});
}

void Passive15_0148(PLW* wk) {
    pattern_jump_look(wk);
}

void Passive15_0149(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F70, -1, 8, 0x20, 1, -1, 0x20, 0x20});
}

void Passive15_0150(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F70, -0x7FB0, 8, 0x20, 2, -1, -0x7FB0, 0x20}
    );
}

void Passive15_0151(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x102);
}

void Passive15_0152(PLW* wk) {
    pattern_em_term_normal_attack_6(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
}

void Passive15_0153(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1}, 8, 0x402);
}

void Passive15_0154(PLW* wk) {
    pattern_em_term_normal_attack_3(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1}, 8, 0x10);
}

void Passive15_0155(PLW* wk) {
    pattern_wait_branch_unit_area(wk, 6, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
}

void Passive15_0156(PLW* wk) {
    pattern_check_ex_wait_j_command_attack(wk, 4, &(Command_Attack_Args){8, 0x1F, 9, 0x700});
}

void Passive15_0157(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x110);
}

void Passive15_0158(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x110);
}

void Passive15_0159(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x110);
}

void Passive15_0160(PLW* wk) {
    pattern_search_back_term_walk_wait(wk);
}

void Passive15_0161(PLW* wk) {
    pattern_search_back_term_walk_wait_2(wk);
}

void Passive15_0162(PLW* wk) {
    pattern_walk_search_back_term_walk(wk);
}

void Passive15_0163(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 8, 0x400, 2, -1, 0x20, 0x400});
}
