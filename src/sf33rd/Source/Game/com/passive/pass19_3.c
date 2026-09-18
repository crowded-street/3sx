/**
 * @file pass19_3.c
 * COM Passive: Remy
 * Pattern scripts from Passive19_0123 on.
 */

#include "sf33rd/Source/Game/com/passive/pass19.h"
#include "sf33rd/Source/Game/com/passive/pass19_internal.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive19_0123(PLW* wk) {
    passive19_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 2, -1, 0x20, 0x400});
}

void Passive19_0124(PLW* wk) {
    passive19_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 2, -1, 0x20, 0x20});
}

void Passive19_0125(PLW* wk) {
    passive19_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 2, -1, 0x20, 0x40});
}

void Passive19_0126(PLW* wk) {
    passive19_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 6, 1, -1}, 0x42);
}

void Passive19_0127(PLW* wk) {
    passive19_pattern_normal_attack(wk, 8, 0x40);
}

void Passive19_0128(PLW* wk) {
    passive19_pattern_check_ex_j_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1F, 9, 0x700});
}

void Passive19_0129(PLW* wk) {
    passive19_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive19_0130(PLW* wk) {
    passive19_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive19_0131(PLW* wk) {
    passive19_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive19_0132(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x82, 0x83});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0133(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0x84);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0134(PLW* wk) {
    passive19_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, 0x70});
}

void Passive19_0135(PLW* wk) {
    passive19_pattern_check_ex_j_command_attack(wk, 1, &(Command_Attack_Args){8, 0x1F, 10, 0x700});
}

void Passive19_0136(PLW* wk) {
    passive19_pattern_check_ex_j_command_attack(wk, 0x36, &(Command_Attack_Args){8, 0x1F, 10, 0x700});
}

void Passive19_0137(PLW* wk) {
    passive19_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7F80, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, 0x70}
    );
}

void Passive19_0138(PLW* wk) {
    passive19_pattern_check_ex_j_command_attack(wk, 0x22, &(Command_Attack_Args){8, 0x1F, 10, 0x700});
}

void Passive19_0139(PLW* wk) {
    passive19_pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 0xD, 0x400, 2});
}

void Passive19_0140(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0141(PLW* wk) {
    passive19_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 0x40);
}

void Passive19_0142(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0143(PLW* wk) {
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

void Passive19_0144(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, 1);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x70}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0145(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, 1);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0});
        break;

    case 2:
        Approach_Walk(wk, 0x93, 2);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x7F, 0x70}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0146(PLW* wk) {
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
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0147(PLW* wk) {
    passive19_pattern_check_ex_j_command_attack(wk, 0x85, &(Command_Attack_Args){8, 0x1F, 10, 0x700});
}

void Passive19_0148(PLW* wk) {
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

void Passive19_0149(PLW* wk) {
    passive19_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F70, -1, 8, 0x20, 1, -1, 0x20, 0x20});
}

void Passive19_0150(PLW* wk) {
    passive19_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F70, -0x7FB0, 8, 0x20, 2, -1, -0x7FB0, 0x20}
    );
}

void Passive19_0151(PLW* wk) {
    passive19_pattern_normal_attack_2(wk, 0x102, 8, 0x102);
}

void Passive19_0152(PLW* wk) {
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

void Passive19_0153(PLW* wk) {
    passive19_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F70, -1, 6, 1, -1}, 0x402);
}

void Passive19_0154(PLW* wk) {
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

void Passive19_0155(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 3);
        break;

    case 1:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x59, 0x59, 0x5A, 0x5B});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0156(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0x9B);
        break;

    case 1:
        Wait(wk, 3);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0157(PLW* wk) {
    passive19_pattern_lever_attack(wk, 0, 0x110);
}

void Passive19_0158(PLW* wk) {
    passive19_pattern_lever_attack(wk, 1, 0x110);
}

void Passive19_0159(PLW* wk) {
    passive19_pattern_normal_attack(wk, 8, 0x110);
}

void Passive19_0160(PLW* wk) {
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

void Passive19_0161(PLW* wk) {
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

void Passive19_0162(PLW* wk) {
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

void Passive19_0163(PLW* wk) {
    passive19_pattern_lever_attack(wk, 0, 0x200);
}

void Passive19_0164(PLW* wk) {
    passive19_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive19_0165(PLW* wk) {
    passive19_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive19_0166(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0167(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1D, 8, -1});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0168(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0169(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0170(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0171(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, -0x7FA0, 8, 0x402, 0, -0x7FA0, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x102);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0172(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0173(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0174(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0175(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
