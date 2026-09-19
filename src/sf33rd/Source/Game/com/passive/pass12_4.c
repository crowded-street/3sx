/**
 * @file pass12_4.c
 * COM Passive: Sean
 * Pattern scripts from Passive12_0171 on.
 */

#include "sf33rd/Source/Game/com/passive/pass12.h"
#include "sf33rd/Source/Game/com/passive/pass12_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive12_0171(PLW* wk) {
    pattern_keep_away_wait_get_up_command_attack(wk, 0xBF, -1, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
}

void Passive12_0172(PLW* wk) {
    pattern_keep_away_com_random_select_2(wk, 0x50, 3, &(Branch_Menu_Args){6, 0x3F, 0x40, 0x41, 0x43});
}

void Passive12_0173(PLW* wk) {
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

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
        break;

    case 5:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0174(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1C, 0x400A, -1});
        break;

    case 4:
        Wait(wk, 5);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0175(PLW* wk) {
    pattern_normal_attack_j_command_attack_5(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive12_0176(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x200);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0177(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x200);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0178(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1D, 0xA, -1});
        break;

    case 3:
        Wait(wk, 5);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0179(PLW* wk) {
    pattern_lever_attack_normal_attack(wk);
}

void Passive12_0180(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1C, 0x400A, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0181(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1C, 0x400A, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0182(PLW* wk) {
    pattern_approach_walk(wk, 0x37);
}

void Passive12_0183(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x200},
        0x40,
        &(Command_Attack_Args){8, 0x1C, 0x400A, -1}
    );
}

void Passive12_0184(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1C, 0x400A, -1},
        2
    );
}

void Passive12_0185(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x20);
}

void Passive12_0186(PLW* wk) {
    passive12_pattern_command_attack_com_random_select(wk, &(Command_Attack_Args){8, 0x1C, 0x4008, -1});
}

void Passive12_0187(PLW* wk) {
    passive12_pattern_command_attack_com_random_select(wk, &(Command_Attack_Args){8, 0x1C, 0x4009, -1});
}

void Passive12_0188(PLW* wk) {
    passive12_pattern_command_attack_com_random_select(wk, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
}

void Passive12_0189(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1D, 9, -1}
    );
}

void Passive12_0190(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive12_0191(PLW* wk) {
    pattern_approach_walk_com_random_select(wk, 0x60, &(Branch_Menu_Args){6, 0x3F, 0x40, 0x41, 0x43}, 0);
}

void Passive12_0192(PLW* wk) {
    pattern_keep_away_com_random_select(wk, 0xBF);
}

void Passive12_0193(PLW* wk) {
    pattern_walk_com_random_select(wk, 0x20, &(Branch_Menu_Args){6, 0xBA, 1, 0xAF, 0x92}, 3);
}

void Passive12_0194(PLW* wk) {
    pattern_normal_attack_com_random_select(wk, 0x200, &(Branch_Menu_Args){6, 1, 1, 0xC0, 0xC0}, 4);
}

void Passive12_0195(PLW* wk) {
    pattern_normal_attack_com_random_select(wk, 0x400, &(Branch_Menu_Args){6, 1, 1, 0xC0, 0xC0}, 4);
}

void Passive12_0196(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x20, 0xA, -1}
    );
}

void Passive12_0197(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1}, 0x200);
}

void Passive12_0198(PLW* wk) {
    pattern_vs_jump_guard(wk);
}

void Passive12_0199(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x40, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
}

void Passive12_0200(PLW* wk) {
    pattern_normal_attack_j_command_attack_2(wk, 0xC, 0x40, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive12_0201(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1C, 0x400A, -1});
        break;

    case 2:
        Wait(wk, 5);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0202(PLW* wk) {
    pattern_normal_attack_sa_term_j_command_attack(
        wk,
        0x40,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive12_0203(PLW* wk) {
    pattern_walk_com_random_select(wk, 0x14, &(Branch_Menu_Args){6, 0xC6, 0xC4, 0xC5, 0x2F}, 1);
}

void Passive12_0204(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack_2(wk, 0x30, 0x2F, &(Branch_Menu_Args){6, 0x51, 0x1C, 0x4D, 0x25});
}

void Passive12_0205(PLW* wk) {
    pattern_normal_attack_command_attack_5(wk, &(Command_Attack_Args){8, 0x1C, 0x4008, -1});
}

void Passive12_0206(PLW* wk) {
    pattern_keep_away(wk, 0xBF, 1);
}

void Passive12_0207(PLW* wk) {
    pattern_pierce_on_jump_com_random_select(wk, &(Branch_Menu_Args){6, 0x53, 0x55, 0x13, 0x25}, 4);
}

void Passive12_0208(PLW* wk) {
    pattern_pierce_on_jump_com_random_select(wk, &(Branch_Menu_Args){6, 0x53, 0x54, 1, 0x25}, 3);
}

void Passive12_0209(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x200);
}

void Passive12_0210(PLW* wk) {
    pattern_pierce_on_j_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x20, 0xA, -1});
}

void Passive12_0211(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_4(
        wk,
        0x37,
        &(Command_Attack_Args){0xC, 0x1D, 10, -1},
        &(SA_Term_Args){0x30, 0x31, 0x32, 0}
    );
}

void Passive12_0212(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1D, 0xA, -1});
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0213(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1D, 0xA, -1});
        break;

    case 3:
        Wait(wk, 5);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0214(PLW* wk) {
    passive12_pattern_approach_walk_j_command_attack(wk, &(Command_Attack_Args){8, 0x20, 0xB, -1});
}

void Passive12_0215(PLW* wk) {
    pattern_wait_get_up_lever_attack_2(wk, 0x1E, 8);
}

void Passive12_0216(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xBF, 0);
        break;

    case 1:
        Lever_Attack(wk, 8, 0x1E, 0xA);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0217(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x200});
        break;

    case 3:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0218(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, 0x700});
}

void Passive12_0219(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1C, 0x400B, -1});
}

void Passive12_0220(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 0xA, 0x70});
}

void Passive12_0221(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x20, 0xA, 0x700});
}

void Passive12_0222(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 5, 1, 0xFFFF);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, -1, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, -1, -1});
        break;

    case 3:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0223(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive12_0224(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x400);
}

void Passive12_0225(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x402);
}

void Passive12_0226(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive12_0227(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive12_0228(PLW* wk) {
    pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -1, -1, 0xFFFF});
}

void Passive12_0229(PLW* wk) {
    pattern_pierce_on_command_attack_em_term(wk);
}

void Passive12_0230(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F00, 0x38, 8, 0x200, 1, -1, -1, -1});
}
