/**
 * @file pass11_4.c
 * COM Passive: Ken
 * Pattern scripts from Passive11_0177 on.
 */

#include "sf33rd/Source/Game/com/passive/pass11.h"
#include "sf33rd/Source/Game/com/passive/pass11_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive11_0177(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x200);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0178(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1C, 0xA, -1});
        break;

    case 3:
        Wait(wk, 5);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0179(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive11_0180(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 1, 0x200);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0181(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive11_0182(PLW* wk) {
    pattern_approach_walk(wk, 0x47);
}

void Passive11_0183(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x200},
        0x40,
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive11_0184(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1D, 10, -1},
        2
    );
}

void Passive11_0185(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1}, 8, 0x20);
}

void Passive11_0186(PLW* wk) {
    passive11_pattern_command_attack_com_random_select(
        wk,
        &(Command_Attack_Args){8, 0x1D, 8, -1},
        &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}
    );
}

void Passive11_0187(PLW* wk) {
    passive11_pattern_command_attack_com_random_select(
        wk,
        &(Command_Attack_Args){8, 0x1D, 9, -1},
        &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}
    );
}

void Passive11_0188(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Passive11_0189(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive11_0190(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive11_0191(PLW* wk) {
    pattern_approach_walk_com_random_select(wk, 0x60, &(Branch_Menu_Args){6, 0x3F, 0x40, 0x41, 0x43}, 0);
}

void Passive11_0192(PLW* wk) {
    pattern_keep_away_com_random_select(wk, 0xBF);
}

void Passive11_0193(PLW* wk) {
    pattern_walk_com_random_select(wk, 0x20, &(Branch_Menu_Args){6, 0xBA, 1, 0xAF, 0x92}, 3);
}

void Passive11_0194(PLW* wk) {
    pattern_normal_attack_com_random_select(wk, 0x200, &(Branch_Menu_Args){6, 1, 1, 0xC0, 0xC0}, 4);
}

void Passive11_0195(PLW* wk) {
    pattern_normal_attack_com_random_select(wk, 0x400, &(Branch_Menu_Args){6, 1, 1, 0xC0, 0xC0}, 4);
}

void Passive11_0196(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Passive11_0197(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1}, 8, 0x200);
}

void Passive11_0198(PLW* wk) {
    pattern_vs_jump_guard(wk);
}

void Passive11_0199(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x40, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive11_0200(PLW* wk) {
    pattern_normal_attack_j_command_attack_2(wk, 0xC, 0x40, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive11_0201(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1D, 0xA, -1});
        break;

    case 2:
        Wait(wk, 5);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0202(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0203(PLW* wk) {
    pattern_walk_com_random_select(wk, 0x14, &(Branch_Menu_Args){6, 0xC6, 0xC4, 0xC5, 0x2F}, 1);
}

void Passive11_0204(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack_2(wk, 0x60, 0x2F, &(Branch_Menu_Args){6, 0x51, 0x1C, 0x4D, 0x25});
}

void Passive11_0205(PLW* wk) {
    pattern_normal_attack_command_attack_6(wk, 0xB, 0x220, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive11_0206(PLW* wk) {
    pattern_keep_away(wk, 0xBF, 1);
}

void Passive11_0207(PLW* wk) {
    pattern_pierce_on_jump_com_random_select(wk, &(Branch_Menu_Args){6, 0x14, 0x15, 0x13, 0x25}, 4);
}

void Passive11_0208(PLW* wk) {
    pattern_pierce_on_jump_com_random_select(wk, &(Branch_Menu_Args){6, 0x14, 0x15, 0x16, 0xF1}, 3);
}

void Passive11_0209(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1}, 8, 0x200);
}

void Passive11_0210(PLW* wk) {
    pattern_pierce_on_j_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, 0x700});
}

void Passive11_0211(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_4(
        wk,
        0x10,
        &(Command_Attack_Args){0xC, 0x1C, 10, -1},
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F}
    );
}

void Passive11_0212(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1C, 0xA, -1});
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0213(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    case 1:
        Next_Another_Menu(wk, 6, 0x73);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0214(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x40});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1D, 0xA, -1});
        break;

    case 3:
        Wait(wk, 5);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0215(PLW* wk) {
    pattern_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive11_0216(PLW* wk) {
    pattern_jump_attack_term_normal_attack_j_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202,
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive11_0217(PLW* wk) {
    pattern_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive11_0218(PLW* wk) {
    pattern_jump_attack_term_normal_attack_j_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x200},
        0x40,
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive11_0219(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0220(PLW* wk) {
    pattern_pierce_on_command_attack_em_term(wk);
}

void Passive11_0221(PLW* wk) {
    passive11_pattern_approach_walk_em_term_sa_term(wk, 0);
}

void Passive11_0222(PLW* wk) {
    passive11_pattern_approach_walk_em_term_sa_term(wk, 1);
}

void Passive11_0223(PLW* wk) {
    passive11_pattern_approach_walk_em_term_sa_term(wk, 0xFFFF);
}

void Passive11_0224(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FE0, 6, 1, -1},
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive11_0225(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1}, 8, 0x402);
}

void Passive11_0226(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0227(PLW* wk) {
    passive11_pattern_em_term_sa_term_normal_attack(wk, 0x402);
}

void Passive11_0228(PLW* wk) {
    passive11_pattern_em_term_sa_term_normal_attack(wk, 0x202);
}

void Passive11_0229(PLW* wk) {
    passive11_pattern_em_term_sa_term_normal_attack(wk, 0x200);
}

void Passive11_0230(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x400);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0231(PLW* wk) {
    passive11_pattern_command_attack_com_random_select(
        wk,
        &(Command_Attack_Args){8, 0, -1, -1},
        &(Branch_Menu_Args){6, 0xF0, 1, 0xE2, 0xEC}
    );
}

void Passive11_0232(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, 0x70});
}

void Passive11_0233(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, 0x700});
}

void Passive11_0234(PLW* wk) {
    pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x1E, 0xA, 0x700, -1, 0x30, 0, -1, -1, 0xFFFF});
}

void Passive11_0235(PLW* wk) {
    pattern_hi_jump_command_attack_term(
        wk,
        &(JCA_Term_Args){8, 0x1E, 0xA, 0x700, -1, 0x30, 0, -1, -1, 0xFFFF}
    );
}

void Passive11_0236(PLW* wk) {
    pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x88, 0x8A, 0xEA, 0xEB}, 4);
}

void Passive11_0237(PLW* wk) {
    pattern_pierce_on_jump_com_random_select(wk, &(Branch_Menu_Args){6, 0xD2, 0xEC, 0xA1, 0xCF}, 5);
}

void Passive11_0238(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7FA8, -0x7FC0, 8, 0x200, 0, -0x7FB0, -1, 0x200});
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0239(PLW* wk) {
    pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x14, 0x15, 0x16, 0x16}, 4);
}

void Passive11_0240(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0241(PLW* wk) {
    pattern_com_random_select(wk, &(Branch_Menu_Args){6, 1, 1, 1, 0xF0}, 3);
}

void Passive11_0242(PLW* wk) {
    pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0xF0, 1, 1, 1}, 2);
}

void Passive11_0243(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x200);
}

void Passive11_0244(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x400);
}

void Passive11_0245(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack_SP(wk, &(Lever_Attack_SP_Args){8, 0, 0x400, 0x12});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0246(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x200);
}

void Passive11_0247(PLW* wk) {
    pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0xF3, 0xF4, 0xA5, 0xF6}, 3);
}

void Passive11_0248(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack_SP(wk, &(Lever_Attack_SP_Args){8, 0, 0x400, 0x12});
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 6, 7, 8, 8}, 4);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0249(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Lever_Attack_SP(wk, &(Lever_Attack_SP_Args){8, 0, 0x400, 0x12});
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0xEF, 0xF7, 0xFB, 0xEC});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0250(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x400);
        break;

    case 2:
        Lever_Attack(wk, 8, 1, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0251(PLW* wk) {
    pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0xF3, 0xF4, 0xA5, 0xFA}, 3);
}

void Passive11_0252(PLW* wk) {
    pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0xF3, 0x10, 0xA5, 0xFD}, 5);
}
