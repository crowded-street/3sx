/**
 * @file pass18_3.c
 * COM Passive: Twelve
 * Pattern scripts from Passive18_0111 on.
 */

#include "sf33rd/Source/Game/com/passive/pass18.h"
#include "sf33rd/Source/Game/com/passive/pass18_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive18_0111(PLW* wk) {
    pattern_em_term_normal_attack_com_random_select(wk, 0x22);
}

void Passive18_0112(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Passive18_0113(PLW* wk) {
    pattern_normal_attack_com_random_select_2(wk, 9, 0x22);
}

void Passive18_0114(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive18_0115(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20});
}

void Passive18_0116(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x20});
}

void Passive18_0117(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x100, 0});
}

void Passive18_0118(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Passive18_0119(PLW* wk) {
    pattern_lever_on_em_term_check_store_lever(wk, 0x1D);
}

void Passive18_0120(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 1, -1, 0x20, 0x400});
}

void Passive18_0121(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 1, -1, 0x20, 0x20});
}

void Passive18_0122(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 1, -1, 0x20, 0x40});
}

void Passive18_0123(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 2, -1, 0x20, 0x400});
}

void Passive18_0124(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 2, -1, 0x20, 0x20});
}

void Passive18_0125(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 2, -1, 0x20, 0x40});
}

void Passive18_0126(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 0x42);
}

void Passive18_0127(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Passive18_0128(PLW* wk) {
    pattern_check_ex_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1D, 9, 0x700});
}

void Passive18_0129(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 8, -1, -1, -0x7FB8, 0, -1, -1, 0xFFFF});
}

void Passive18_0130(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FB8, 2, -1, -1, 0xFFFF});
}

void Passive18_0131(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 0xA, -1, -1, -0x7FB8, 1, -1, -1, 0xFFFF});
}

void Passive18_0132(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x82, 0x83});
}

void Passive18_0133(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0x84);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x2F, 0xA, -1, -1, -0x7FB8, 1, -1, 0x30, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0134(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, 0x70});
}

void Passive18_0135(PLW* wk) {
    pattern_check_ex_command_attack(wk, 1, &(Command_Attack_Args){8, 0x1D, 10, 0x700});
}

void Passive18_0136(PLW* wk) {
    pattern_check_ex_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1D, 10, 0x700});
}

void Passive18_0137(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, 0x70}
    );
}

void Passive18_0138(PLW* wk) {
    pattern_check_ex_command_attack(wk, 0x22, &(Command_Attack_Args){8, 0x1D, 10, 0x700});
}

void Passive18_0139(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 10, 0x42, 2});
}

void Passive18_0140(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Passive18_0141(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 0x40);
}

void Passive18_0142(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive18_0143(PLW* wk) {
    pattern_provoke_com_random_select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x70});
}

void Passive18_0144(PLW* wk) {
    pattern_provoke_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0},
        &(Branch_Menu_Args){6, 0x85, 0x86, 0x88, 0x70}
    );
}

void Passive18_0145(PLW* wk) {
    pattern_provoke_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0},
        &(Branch_Menu_Args){6, 0x85, 0x73, 0x92, 0x93}
    );
}

void Passive18_0146(PLW* wk) {
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
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0147(PLW* wk) {
    pattern_check_ex_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1D, 10, 0x700});
}

void Passive18_0148(PLW* wk) {
    pattern_jump_look(wk);
}

void Passive18_0149(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F70, -1, 8, 0x20, 1, -1, 0x20, 0x20});
}

void Passive18_0150(PLW* wk) {
    passive18_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F70, -0x7FB0, 8, 0x20, 2, -1, -0x7FB0, 0x20}
    );
}

void Passive18_0151(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x102);
}

void Passive18_0152(PLW* wk) {
    pattern_em_term_normal_attack_6(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
}

void Passive18_0153(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1}, 0x402);
}

void Passive18_0154(PLW* wk) {
    pattern_em_term_normal_attack_5(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
}

void Passive18_0155(PLW* wk) {
    pattern_wait_branch_unit_area(wk, 3, &(Branch_Menu_Args){6, 0x5B, 0x5B, 0x5A, 0x5A});
}

void Passive18_0156(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0x9B);
        break;

    case 1:
        Wait(wk, 4);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 9, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0157(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x110);
}

void Passive18_0158(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x110);
}

void Passive18_0159(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x110);
}

void Passive18_0160(PLW* wk) {
    active_pattern_search_back_term_walk_wait(wk);
}

void Passive18_0161(PLW* wk) {
    active_pattern_search_back_term_walk_wait_2(wk);
}

void Passive18_0162(PLW* wk) {
    active_pattern_walk_search_back_term_walk(wk);
}

void Passive18_0163(PLW* wk) {
    pattern_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0},
        &(Branch_Menu_Args){6, 6, 0x14, 0x16, 0x17},
        1
    );
}

void Passive18_0164(PLW* wk) {
    active_pattern_approach_walk(wk, 0xBF);
}

void Passive18_0165(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0xBF, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0166(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 8, 0x400, 0, -0x7FA0, -1, 0x200});
}

void Passive18_0167(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 9, -1, -1, 0x50, 0, -0x7FA0, -1, 0x40});
}

void Passive18_0168(PLW* wk) {
    pattern_wait_get_up_branch_unit_area(wk, 0, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
}

void Passive18_0169(PLW* wk) {
    pattern_wait_get_up_branch_unit_area(wk, 3, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
}

void Passive18_0170(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
}

void Passive18_0171(PLW* wk) {
    passive18_pattern_wait_get_up_pierce_on_command_attack(wk, 3, &(Command_Attack_Args){8, 0, 0xB, -1});
}

void Passive18_0172(PLW* wk) {
    passive18_pattern_wait_get_up_pierce_on_command_attack(wk, 0, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive18_0173(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 0, 0x42);
}

void Passive18_0174(PLW* wk) {
    active_pattern_approach_walk(wk, 0x37);
}

void Passive18_0175(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Passive18_0176(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x100, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive18_0177(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 9, -1, -1, 0x50, 2, -1, -1, 0xFFFF});
}

void Passive18_0178(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x200, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive18_0179(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive18_0180(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive18_0181(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x10, &(Command_Attack_Args){8, 0x1D, 8, 0x70});
}

void Passive18_0182(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x10, &(Command_Attack_Args){8, 0x1C, 8, 0x70});
}

void Passive18_0183(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x100, &(Command_Attack_Args){8, 0x1D, 8, 0x70});
}

void Passive18_0184(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x200, &(Command_Attack_Args){8, 0x1C, 8, 0x70});
}

void Passive18_0185(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x1D, 8, 0x70});
}

void Passive18_0186(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x1C, 8, 0x70});
}

void Passive18_0187(PLW* wk) {
    pattern_normal_attack_4(wk, 9, 9, 0x12);
}

void Passive18_0188(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive18_0189(PLW* wk) {
    active_pattern_pierce_on_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1C, 10, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive18_0190(PLW* wk) {
    pattern_sa_term_branch_unit_area(
        wk,
        &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0},
        &(Branch_Menu_Args){2, 0x41, 0x41, 0x42, 0x43}
    );
}

void Passive18_0191(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x11, 0x6B, 0x6C, 0x67}, 4);
}

void Passive18_0192(PLW* wk) {
    active_pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1C, 9, -1}, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive18_0193(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0x1C, 9, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive18_0194(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Passive18_0195(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 9, -1, -1, 0x50, 1, -1, -1, 0xFFFF});
}

void Passive18_0196(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x35, 0x37, 0xC5, 0x86}, 4);
}

void Passive18_0197(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive18_0198(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x30, 0xA, -1, -1, -0x7FD0, 2, -1, 0x30, 0x400});
}

void Passive18_0199(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 0xC6}, 4);
}

void Passive18_0200(PLW* wk) {
    pattern_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0},
        &(Branch_Menu_Args){6, 0xC, 0x67, 199, 0x26},
        1
    );
}
