/**
 * @file pass13_3.c
 * COM Passive: Urien
 * Pattern scripts from Passive13_0113 on.
 */

#include "sf33rd/Source/Game/com/passive/pass13.h"
#include "sf33rd/Source/Game/com/passive/pass13_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive13_0113(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FA8, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x42);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1F, 9, -1});
        break;

    case 4:
        Lever_On(wk, 1, 2);
        break;

    case 5:
        Wait(wk, 5);
        break;

    default:
        pattern_command_attack_from_step_6(wk);
        break;
    }
}

void Passive13_0114(PLW* wk) {
    pattern_pierce_on_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, 0x30, 7, 1, -1},
        &(Command_Attack_Args){0xB, 0x1F, 9, -1},
        &(Command_Attack_Args){8, 0x1F, 8, -1}
    );
}

void Passive13_0115(PLW* wk) {
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
        J_Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1F, 9, -1});
        break;

    case 4:
        ETC_Term(wk, 2, 6, 1);
        break;

    case 5:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0116(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x10, -0x5FF8, 0x400, 1, -1, 0x10, 0x400});
}

void Passive13_0117(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x10, -0x5FF8, 0x200, 1, -1, 0x10, 0x20});
}

void Passive13_0118(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x10, -0x5FF8, 0x20, 1, -1, 0x10, 0x20});
}

void Passive13_0119(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x10, -0x5FF8, 0x400, 0, -1, 0x10, 0x400});
}

void Passive13_0120(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x10, -0x5FF8, 0x200, 0, -1, 0x10, 0x20});
}

void Passive13_0121(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x10, -0x5FF8, 0x20, 0, -1, 0x10, 0x20});
}

void Passive13_0122(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 0x42);
}

void Passive13_0123(PLW* wk) {
    pattern_pierce_on_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 9, 0x40);
}

void Passive13_0124(PLW* wk) {
    pattern_pierce_on_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 8, 0x400);
}

void Passive13_0125(PLW* wk) {
    pattern_search_back_term_walk_normal_attack(wk, 0x22, 0x400);
}

void Passive13_0126(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Passive13_0127(PLW* wk) {
    pattern_search_back_term_walk_normal_attack(wk, 0x7E, 0x40);
}

void Passive13_0128(PLW* wk) {
    pattern_em_term_normal_attack_4(wk, &(EM_Term_Params){-0x7F80, -0x7FB0, 6, 1, -1});
}

void Passive13_0129(PLW* wk) {
    passive13_pattern_lever_attack(wk, 8, 0x20);
}

void Passive13_0130(PLW* wk) {
    pattern_search_back_term_walk_em_term(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive13_0131(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x71, 0x400);
}

void Passive13_0132(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0xFFFF, 0x3A, 0x3D, 0xFFFF},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive13_0133(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x39, 0x3A, 0x3C, 0xFFFF});
        break;

    case 1:
        Check_Store_Lever(wk, 0x1C, 6, 0x86);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0134(PLW* wk) {
    active_pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 8, 0x70}, &(Command_Attack_Args){8, 0, 0xB, -1});
}

void Passive13_0135(PLW* wk) {
    pattern_provoke(wk, 3);
}

void Passive13_0136(PLW* wk) {
    pattern_etc_term_check_sa_branch_unit_area(
        wk,
        &(ETC_Term_Step){ 7, 6, 0x85 },
        0x35,
        &(Branch_Menu_Args){6, 0x89, 0x89, 0x8A, 0x8B}
    );
}

void Passive13_0137(PLW* wk) {
    pattern_command_attack_check_sa_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 9, -1});
}

void Passive13_0138(PLW* wk) {
    pattern_command_attack_check_sa_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 10, -1});
}

void Passive13_0139(PLW* wk) {
    pattern_command_attack_check_sa_command_attack_2(wk, &(Command_Attack_Args){8, 0x8016, 10, -1});
}

void Passive13_0140(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x8D, 0x8D, 0x8E, 0x8F});
}

void Passive13_0141(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, 0x700});
}

void Passive13_0142(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, 0x700});
}

void Passive13_0143(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, 0x700});
}

void Passive13_0144(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x91, 0x91, 0x92, 0x93});
}

void Passive13_0145(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, 0x700});
}

void Passive13_0146(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, 0x700});
}

void Passive13_0147(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, 0x700});
}

void Passive13_0148(PLW* wk) {
    pattern_jump(wk, 1);
}

void Passive13_0149(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FC0, -1, -0x5FF8, 0x200, 1, -1, -1, -1});
}

void Passive13_0150(PLW* wk) {
    passive13_pattern_lever_attack(wk, 8, 0x20);
}

void Passive13_0151(PLW* wk) {
    pattern_check_store_lever_command_attack(
        wk,
        &(Check_Store_Lever_Step){ 0x1F, 6, 0x96 },
        &(Command_Attack_Args){8, 0x1F, 9, 0x70}
    );
}

void Passive13_0152(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 8, 6, 0x22);
        break;

    case 1:
        Check_SA(wk, 6, 0x22);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x3A, 0xFFFF, 0xFFFF});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0153(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FC0, -1, -0x5FF8, 0x400, 2, -1, -1, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0154(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 10, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 9, -1});
        break;

    case 3:
        Check_SA(wk, 6, 0x22);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x39, 0x3A, 0x3D, 0xFFFF});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0155(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1}, 0, 0x20);
}

void Passive13_0156(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 0);
        break;

    case 1:
        Normal_Attack(wk, 0xD, 0x10);
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x9D, 0x59, 0x8C, 0x9E}, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0157(PLW* wk) {
    passive13_pattern_lever_attack(wk, 8, 0x200);
}

void Passive13_0158(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x22);
}

void Passive13_0159(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Passive13_0160(PLW* wk) {
    passive13_pattern_lever_attack(wk, 8, 0x20);
}

void Passive13_0161(PLW* wk) {
    passive13_pattern_lever_attack(wk, 8, 0x400);
}

void Passive13_0162(PLW* wk) {
    passive13_pattern_lever_attack(wk, 0xD, 0x20);
}

void Passive13_0163(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
