/**
 * @file pass17_3.c
 * COM Passive: Q
 * Pattern scripts from Passive17_0116 on.
 */

#include "sf33rd/Source/Game/com/passive/pass17.h"
#include "sf33rd/Source/Game/com/passive/pass17_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive17_0116(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x20});
}

void Passive17_0117(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x100, 0});
}

void Passive17_0118(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Passive17_0119(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x68, 0x69, 0x69, 0x6A});
}

void Passive17_0120(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 1, -1, 0x20, 0x200});
}

void Passive17_0121(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 1, -1, 0x20, 0x20});
}

void Passive17_0122(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 1, -1, 0x20, 0x40});
}

void Passive17_0123(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 2, -1, 0x20, 0x200});
}

void Passive17_0124(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 2, -1, 0x20, 0x20});
}

void Passive17_0125(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 2, -1, 0x20, 0x40});
}

void Passive17_0126(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 8, 0x42);
}

void Passive17_0127(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Passive17_0128(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1D, 9, 0x700});
}

void Passive17_0129(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive17_0130(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive17_0131(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive17_0132(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x82, 0x83});
}

void Passive17_0133(PLW* wk) {
    pattern_check_ex_command_attack(wk, 0x84, &(Command_Attack_Args){8, 0x1C, 10, 0x700});
}

void Passive17_0134(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 10, 0x70});
}

void Passive17_0135(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1D, 10, 0x700});
}

void Passive17_0136(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1D, 10, 0x700});
}

void Passive17_0137(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, 0x70}
    );
}

void Passive17_0138(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x22, &(Command_Attack_Args){8, 0x1D, 10, 0x700});
}

void Passive17_0139(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 0xF, 0x40, 0});
}

void Passive17_0140(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Passive17_0141(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x40);
}

void Passive17_0142(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive17_0143(PLW* wk) {
    pattern_provoke_com_random_select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x87, 0x70});
}

void Passive17_0144(PLW* wk) {
    pattern_provoke_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0x55},
        &(Branch_Menu_Args){6, 0x85, 0x86, 0x87, 0x70}
    );
}

void Passive17_0145(PLW* wk) {
    pattern_provoke_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0xFFFF},
        &(Branch_Menu_Args){6, 0x85, 0x73, 0x92, 0x87}
    );
}

void Passive17_0146(PLW* wk) {
    pattern_provoke_pierce_on_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive17_0147(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x85, &(Command_Attack_Args){8, 0x1D, 10, 0x700});
}

void Passive17_0148(PLW* wk) {
    pattern_jump_look(wk);
}

void Passive17_0149(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F70, -1, 8, 0x200, 1, -1, 0x20, 0x20});
}

void Passive17_0150(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F70, -0x7FB0, 8, 0x40, 2, -1, -0x7FB0, 0x20}
    );
}

void Passive17_0151(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x102);
}

void Passive17_0152(PLW* wk) {
    pattern_em_term_normal_attack_6(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
}

void Passive17_0153(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1}, 8, 0x402);
}

void Passive17_0154(PLW* wk) {
    pattern_em_term_normal_attack_5(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
}

void Passive17_0155(PLW* wk) {
    pattern_wait_branch_unit_area(wk, 6, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5A, 0x5B});
}

void Passive17_0156(PLW* wk) {
    pattern_check_ex_wait_j_command_attack(wk, 4, &(Command_Attack_Args){8, 0x1C, 9, 0x700});
}

void Passive17_0157(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x110);
}

void Passive17_0158(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x110);
}

void Passive17_0159(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x110);
}

void Passive17_0160(PLW* wk) {
    active_pattern_search_back_term_walk_wait(wk);
}

void Passive17_0161(PLW* wk) {
    active_pattern_search_back_term_walk_wait_2(wk);
}

void Passive17_0162(PLW* wk) {
    active_pattern_walk_search_back_term_walk(wk);
}

void Passive17_0163(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x10, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive17_0164(PLW* wk) {
    pattern_pierce_on_command_attack_wait_3(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive17_0165(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0166(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0167(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x68, 0x69, 0x69, 0x6A}
    );
}

void Passive17_0168(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x10, 0);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1});
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x5E, 0x5E, 1, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0169(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    case 2:
        Walk(wk, 0, 0x10, 0);
        break;

    case 3:
        Wait(wk, 6);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0170(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA(wk, 6, 0xA5);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    case 3:
        Walk(wk, 0, 8, 0);
        break;

    case 4:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7F98, 6, 1, -1});
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0171(PLW* wk) {
    passive17_pattern_lever_on_normal_attack_command_attack(
        wk,
        0x12,
        0x12,
        &(Command_Attack_Args){8, 0x1C, 0x4008, -1}
    );
}

void Passive17_0172(PLW* wk) {
    passive17_pattern_lever_on_normal_attack_command_attack(wk, 0x102, 0x202, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive17_0173(PLW* wk) {
    pattern_pierce_on_lever_on_lever_attack(wk);
}

void Passive17_0174(PLW* wk) {
    passive17_pattern_lever_on_normal_attack_command_attack(
        wk,
        0x102,
        0x202,
        &(Command_Attack_Args){8, 0x1C, 0x4008, -1}
    );
}

void Passive17_0175(PLW* wk) {
    passive17_pattern_lever_on_normal_attack_command_attack(wk, 0x12, 0x12, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive17_0176(PLW* wk) {
    pattern_pierce_on_hi_jump_attack_term_lever_on(wk, 0x12);
}

void Passive17_0177(PLW* wk) {
    pattern_pierce_on_hi_jump_attack_term_lever_on(wk, 0x22);
}

void Passive17_0178(PLW* wk) {
    pattern_pierce_on_hi_jump_attack_term_lever_on_2(
        wk,
        &(Hi_Jump_Term_Args){-1, 0x40, 8, 0x400, 0, -0x7F90, -1, 0x200}
    );
}

void Passive17_0179(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    case 1:
        Lever_On(wk, 1, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0180(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x42);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0181(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}
