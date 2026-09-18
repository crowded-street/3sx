/**
 * @file pass01_3.c
 * COM Passive: Alex
 * Pattern scripts from Passive01_0111 on.
 */

#include "sf33rd/Source/Game/com/passive/pass01.h"
#include "sf33rd/Source/Game/com/passive/pass01_internal.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive01_0111(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x42);
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x4B, 0x36, 0x3B, 0x70}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0112(PLW* wk) {
    passive01_pattern_lever_attack(wk, 1, 0x40);
}

void Passive01_0113(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x42);
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x4B, 0x36, 0x3B, 0x70}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0114(PLW* wk) {
    passive01_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive01_0115(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0116(PLW* wk) {
    passive01_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x20});
}

void Passive01_0117(PLW* wk) {
    passive01_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x100, 0});
}

void Passive01_0118(PLW* wk) {
    passive01_pattern_normal_attack(wk, 8, 0x200);
}

void Passive01_0119(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 2:
        Check_Store_Lever(wk, 0x1F, 1, -1);
        break;

    case 3:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0120(PLW* wk) {
    passive01_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 1, -1, 0x20, 0x400});
}

void Passive01_0121(PLW* wk) {
    passive01_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 1, -1, 0x20, 0x20});
}

void Passive01_0122(PLW* wk) {
    passive01_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 1, -1, 0x20, 0x40});
}

void Passive01_0123(PLW* wk) {
    passive01_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 2, -1, 0x20, 0x400});
}

void Passive01_0124(PLW* wk) {
    passive01_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 2, -1, 0x20, 0x20});
}

void Passive01_0125(PLW* wk) {
    passive01_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 2, -1, 0x20, 0x40});
}

void Passive01_0126(PLW* wk) {
    passive01_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 0x42);
}

void Passive01_0127(PLW* wk) {
    passive01_pattern_normal_attack(wk, 8, 0x40);
}

void Passive01_0128(PLW* wk) {
    passive01_pattern_check_ex_j_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1F, 9, 0x700});
}

void Passive01_0129(PLW* wk) {
    passive01_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Passive01_0130(PLW* wk) {
    passive01_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Passive01_0131(PLW* wk) {
    passive01_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 0xA, -1});
}

void Passive01_0132(PLW* wk) {
    passive01_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x82, 0x83});
}

void Passive01_0133(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0x84);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 0xA, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0134(PLW* wk) {
    passive01_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, 0x70});
}

void Passive01_0135(PLW* wk) {
    passive01_pattern_check_ex_j_command_attack(wk, 1, &(Command_Attack_Args){8, 0x1F, 0xA, 0x700});
}

void Passive01_0136(PLW* wk) {
    passive01_pattern_check_ex_j_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1F, 0xA, 0x700});
}

void Passive01_0137(PLW* wk) {
    passive01_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 0xA, 0x70}
    );
}

void Passive01_0138(PLW* wk) {
    passive01_pattern_check_ex_j_command_attack(wk, 0x22, &(Command_Attack_Args){8, 0x1F, 0xA, 0x700});
}

void Passive01_0139(PLW* wk) {
    passive01_pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 0xA, 0x42, 2});
}

void Passive01_0140(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0141(PLW* wk) {
    passive01_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 0x40);
}

void Passive01_0142(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0143(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, 1);
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x70}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0144(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, 1);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0x59});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x70}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0145(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, 1);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0xFFFF});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x85, 0x73, 0x92, 0x93}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0146(PLW* wk) {
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
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0147(PLW* wk) {
    passive01_pattern_check_ex_j_command_attack(wk, 0x85, &(Command_Attack_Args){8, 0x1F, 0xA, 0x700});
}

void Passive01_0148(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, 1);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0149(PLW* wk) {
    passive01_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F70, -1, 8, 0x200, 1, -1, 0x20, 0x200});
}

void Passive01_0150(PLW* wk) {
    passive01_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F70, -0x7FB0, 8, 0x20, 2, -1, -0x7FB0, 0x20}
    );
}

void Passive01_0151(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0152(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x102);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0153(PLW* wk) {
    passive01_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1}, 0x402);
}

void Passive01_0154(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0155(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 6);
        break;

    case 1:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0156(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0x9B);
        break;

    case 1:
        Wait(wk, 4);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0157(PLW* wk) {
    passive01_pattern_lever_attack(wk, 0, 0x110);
}

void Passive01_0158(PLW* wk) {
    passive01_pattern_lever_attack(wk, 1, 0x110);
}

void Passive01_0159(PLW* wk) {
    passive01_pattern_normal_attack(wk, 8, 0x110);
}

void Passive01_0160(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x30, 2, 0xF);
        break;

    case 1:
        Walk(wk, 1, 0x20, 0);
        break;

    case 2:
        Wait(wk, 3);
        break;

    case 3:
        Walk(wk, 0, 0x30, 0);
        break;

    case 4:
        Wait(wk, 9);
        break;

    case 5:
        Walk(wk, 0, 0x20, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0161(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x20, 2, 0x1B);
        break;

    case 1:
        Walk(wk, 1, 0x18, 0);
        break;

    case 2:
        Wait(wk, 8);
        break;

    case 3:
        Search_Back_Term(wk, 0x30, 2, 0x1B);
        break;

    case 4:
        Walk(wk, 1, 0x20, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0162(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 0, 0x20, 0);
        break;

    case 1:
        Search_Back_Term(wk, 0x30, 2, 6);
        break;

    case 2:
        Walk(wk, 1, 0x28, 0);
        break;

    case 3:
        Wait(wk, 8);
        break;

    case 4:
        Walk(wk, 0, 0x20, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0163(PLW* wk) {
    passive01_pattern_lever_attack(wk, 0, 0x40);
}

void Passive01_0164(PLW* wk) {
    passive01_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F80, -1, 0, 1, -1}, 0, 0x40);
}

void Passive01_0165(PLW* wk) {
    passive01_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0xA6, 0xA7, 0xA8, 1});
}

void Passive01_0166(PLW* wk) {
    passive01_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x21, 8, -1});
}

void Passive01_0167(PLW* wk) {
    passive01_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x21, 9, -1});
}

void Passive01_0168(PLW* wk) {
    passive01_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x21, 0xA, -1});
}

void Passive01_0169(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1});
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x21, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0170(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1});
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x21, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0171(PLW* wk) {
    passive01_pattern_normal_attack(wk, 8, 0x402);
}

void Passive01_0172(PLW* wk) {
    passive01_pattern_normal_attack_command_attack(wk, 8, 0x202, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Passive01_0173(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x100, 0, -0x7FA0, -1, 0x400});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0174(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20});
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0175(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
