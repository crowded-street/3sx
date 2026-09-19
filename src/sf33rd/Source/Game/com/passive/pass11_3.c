/**
 * @file pass11_3.c
 * COM Passive: Ken
 * Pattern scripts from Passive11_0123 on.
 */

#include "sf33rd/Source/Game/com/passive/pass11.h"
#include "sf33rd/Source/Game/com/passive/pass11_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive11_0123(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1}, 8, 0x202);
}

void Passive11_0124(PLW* wk) {
    pattern_normal_attack_command_attack_3(wk, 9, 0xB, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive11_0125(PLW* wk) {
    pattern_normal_attack_sa_term_normal_attack(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
}

void Passive11_0126(PLW* wk) {
    pattern_em_term_normal_attack_sa_term(wk);
}

void Passive11_0127(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x1E, 10, 0x700, -1, 0x30, 2, -1, -1, 0xFFFF});
}

void Passive11_0128(PLW* wk) {
    pattern_normal_attack_jump_attack_term_sa_term(
        wk,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1D, 9, -1}
    );
}

void Passive11_0129(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x1E, 10, -1, -1, 0x30, 2, -1, -1, 0xFFFF});
}

void Passive11_0130(PLW* wk) {
    pattern_normal_attack_jump_attack_term_sa_term_2(
        wk,
        9,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0131(PLW* wk) {
    pattern_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0132(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40},
        0x400,
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0133(PLW* wk) {
    pattern_jump_attack_term_normal_attack_sa_term(
        wk,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0134(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200},
        0x40,
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive11_0135(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(
        wk,
        &(JCA_Term_Args){8, 0x1E, 0xA, 0x700, -1, 0x30, 2, -1, -1, 0xFFFF}
    );
}

void Passive11_0136(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x1E, 0xA, -1, -1, 0x30, 0, -1, -1, 0xFFFF});
}

void Passive11_0137(PLW* wk) {
    pattern_wait_get_up_normal_attack_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive11_0138(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x1E, 0xA, -1, -1, 0x30, 0, -1, -1, 0xFFFF});
}

void Passive11_0139(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -1, 8, 0x200, 1, -1, -1, -1});
}

void Passive11_0140(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term(
        wk,
        -1,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive11_0141(PLW* wk) {
    pattern_wait_get_up_branch_wait_area_j_command_attack(
        wk,
        &(Branch_Wait_Args){0x14, 0xF, 5, 1},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Passive11_0142(PLW* wk) {
    pattern_approach_walk_em_term_command_attack_3(
        wk,
        0x47,
        &(Command_Attack_Args){0xC, 0x1D, 0xA, -1},
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F}
    );
}

void Passive11_0143(PLW* wk) {
    passive11_pattern_approach_walk_em_term_normal_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive11_0144(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack(
        wk,
        0x47,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive11_0145(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_5(
        wk,
        0x47,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive11_0146(PLW* wk) {
    pattern_pierce_on_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, -1, -1},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive11_0147(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive11_0148(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive11_0149(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 2, -1, -1, -1},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive11_0150(PLW* wk) {
    pattern_jump(wk, 1);
}

void Passive11_0151(PLW* wk) {
    pattern_pierce_on_command_attack_normal_attack(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
}

void Passive11_0152(PLW* wk) {
    pattern_normal_attack_com_random_select(wk, 0x200, &(Branch_Menu_Args){6, 0xA9, 0xA8, 0xB9, 0x86}, 3);
}

void Passive11_0153(PLW* wk) {
    pattern_etc_term_provoke(wk, 5, 0x56);
}

void Passive11_0154(PLW* wk) {
    pattern_keep_away_wait_get_up_2(wk, 0);
}

void Passive11_0155(PLW* wk) {
    pattern_keep_away_wait_get_up_2(wk, 3);
}

void Passive11_0156(PLW* wk) {
    pattern_keep_away_wait_get_up_command_attack(wk, 0xBF, -1, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive11_0157(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 5, 6, 0x9A);
        break;

    case 1:
        Provoke(wk, -1);
        break;

    case 2:
        Keep_Away(wk, 0xBF, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0158(PLW* wk) {
    pattern_normal_attack_com_random_select_5(wk, 0x100);
}

void Passive11_0159(PLW* wk) {
    active_pattern_approach_walk_normal_attack_sa_term(
        wk,
        0x47,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive11_0160(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive11_0161(PLW* wk) {
    pattern_jump_attack_term_em_term_sa_term(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40},
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F}
    );
}

void Passive11_0162(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 9, 0x10, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive11_0163(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1D, 10, -1},
        2
    );
}

void Passive11_0164(PLW* wk) {
    pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200},
        8,
        0x402
    );
}

void Passive11_0165(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack_SP(wk, 8, 0x200, 0x12);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0166(PLW* wk) {
    pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200},
        8,
        0x402
    );
}

void Passive11_0167(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200},
        0x402,
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive11_0168(PLW* wk) {
    pattern_walk(wk, -1);
}

void Passive11_0169(PLW* wk) {
    pattern_em_term_normal_attack_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        0x400,
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive11_0170(PLW* wk) {
    pattern_keep_away_wait_get_up_2(wk, 3);
}

void Passive11_0171(PLW* wk) {
    pattern_keep_away_wait_get_up_command_attack(wk, 0xBF, -1, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive11_0172(PLW* wk) {
    pattern_keep_away_com_random_select_2(wk, 0x50, 3, &(Branch_Menu_Args){6, 0x3F, 0x40, 0x41, 0x43});
}

void Passive11_0173(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack_2(wk, 0x60, 0xA5, &(Branch_Menu_Args){6, 0x51, 0x1C, 0x4D, 0x25});
}

void Passive11_0174(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1D, 10, -1});
        break;

    case 4:
        Wait(wk, 10);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0175(PLW* wk) {
    pattern_normal_attack_j_command_attack_5(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive11_0176(PLW* wk) {
    pattern_normal_attack_com_random_select_5(wk, 0x102);
}
