/**
 * @file pass07_4.c
 * COM Passive: Ibuki
 * Pattern scripts from Passive07_0162 on.
 */

#include "sf33rd/Source/Game/com/passive/pass07.h"
#include "sf33rd/Source/Game/com/passive/pass07_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

static void passive07_0162_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    case 7:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0162(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 4:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200});
        break;

    case 5:
        Normal_Attack(wk, 9, 0x402);
        break;

    default:
        passive07_0162_from_step_6(wk);
        break;
    }
}

void Passive07_0163(PLW* wk) {
    pattern_jump_attack_term_normal_attack_com_random_select(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x200},
        &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF},
        2
    );
}

void Passive07_0164(PLW* wk) {
    pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200},
        8,
        0x402
    );
}

void Passive07_0165(PLW* wk) {
    pattern_jump_attack_term_normal_attack_j_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200},
        0x402,
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Passive07_0166(PLW* wk) {
    pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200},
        8,
        0x402
    );
}

void Passive07_0167(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x402);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    case 3:
        Search_Back_Term(wk, 0x30, 1, -1);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, -1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0168(PLW* wk) {
    pattern_walk(wk, -1);
}

void Passive07_0169(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Passive07_0170(PLW* wk) {
    passive07_pattern_keep_away_wait_get_up(wk, 3, -1);
}

void Passive07_0171(PLW* wk) {
    passive07_pattern_keep_away_wait_get_up(wk, 0, 0);
}

void Passive07_0172(PLW* wk) {
    passive07_pattern_keep_away_wait_get_up(wk, 3, -1);
}

void Passive07_0173(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0174(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1C, 0xA, -1});
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0175(PLW* wk) {
    pattern_normal_attack_j_command_attack_5(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive07_0176(PLW* wk) {
    pattern_normal_attack_j_command_attack_com_random_select(wk);
}

void Passive07_0177(PLW* wk) {
    pattern_normal_attack_j_command_attack_com_random_select_2(wk);
}

void Passive07_0178(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x40);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x20, 0xA, -1});
        break;

    case 3:
        Wait(wk, 5);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0179(PLW* wk) {
    pattern_lever_attack_normal_attack(wk);
}

void Passive07_0180(PLW* wk) {
    pattern_lever_attack_j_command_attack(wk, 0x200, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive07_0181(PLW* wk) {
    pattern_lever_attack_normal_attack_j_command_attack(wk, 0x200, 0x400, &(Command_Attack_Args){8, 0x20, 0xA, -1});
}

void Passive07_0182(PLW* wk) {
    active_pattern_approach_walk(wk, 0x3B);
}

void Passive07_0183(PLW* wk) {
    pattern_jump_attack_term_normal_attack_j_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x200},
        0x40,
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Passive07_0184(PLW* wk) {
    pattern_jump_attack_term_normal_attack_j_command_attack_3(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive07_0185(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Passive07_0186(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive07_0187(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive07_0188(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Passive07_0189(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive07_0190(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive07_0191(PLW* wk) {
    pattern_approach_walk_com_random_select(wk, 0x60, &(Branch_Menu_Args){6, 0x3F, 0x40, 0x41, 0x43}, 0);
}

void Passive07_0192(PLW* wk) {
    pattern_keep_away_com_random_select(wk, 0xC7);
}

void Passive07_0193(PLW* wk) {
    pattern_walk_com_random_select(wk, 0x20, &(Branch_Menu_Args){6, 0xBA, 1, 0xAF, 0x92}, 3);
}

void Passive07_0194(PLW* wk) {
    pattern_normal_attack_com_random_select_3(wk, 0x200);
}

void Passive07_0195(PLW* wk) {
    pattern_normal_attack_com_random_select_3(wk, 0x400);
}

void Passive07_0196(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Passive07_0197(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1}, 0x200);
}

void Passive07_0198(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1}, 0, 0x402);
}

void Passive07_0199(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 0xC, 0x40, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive07_0200(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 0xC, 0x40, &(Command_Attack_Args){8, 0x20, 0xA, -1});
}

void Passive07_0201(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1C, 0xA, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0202(PLW* wk) {
    pattern_normal_attack_sa_term_j_command_attack(
        wk,
        0x40,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Passive07_0203(PLW* wk) {
    active_pattern_normal_attack(wk, 0xD, 0x100);
}

void Passive07_0204(PLW* wk) {
    active_pattern_em_term_command_attack(wk, &(EM_Term_Params){-0x7F98, 0, 4, 2, 0}, &(Command_Attack_Args){8, 0, 0xA, -1});
}

void Passive07_0205(PLW* wk) {
    pattern_jump_look(wk);
}

void Passive07_0206(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -1, 8, 0x400, 1, -1, 0x20, 0x400});
}

void Passive07_0207(PLW* wk) {
    active_pattern_jump_attack_term_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FB0, 8, 0x40, 2, -1, -0x7FB0, 0x40},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive07_0208(PLW* wk) {
    pattern_jump_attack_term_com_random_select(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FB0, 8, 0x40, 2, -1, -0x7FB0, 0x40},
        &(Branch_Menu_Args){6, 0xAF, 0xB0, 0x7C, 0x7B}
    );
}

void Passive07_0209(PLW* wk) {
    active_pattern_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FB0, 8, 0x40, 2, -1, -0x7FB0, 0x40},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive07_0210(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive07_0211(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Passive07_0212(PLW* wk) {
    pattern_command_attack_em_term_command_attack(wk);
}

void Passive07_0213(PLW* wk) {
    pattern_command_attack_em_term_sa_term(wk);
}
