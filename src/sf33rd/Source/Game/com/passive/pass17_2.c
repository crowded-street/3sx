/**
 * @file pass17_2.c
 * COM Passive: Q
 * Pattern scripts from Passive17_0049 on.
 */

#include "sf33rd/Source/Game/com/passive/pass17.h"
#include "sf33rd/Source/Game/com/passive/pass17_internal.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive17_0049(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1});
        break;

    case 1:
        Adjust_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0050(PLW* wk) {
    passive17_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 8, 0x200);
}

void Passive17_0051(PLW* wk) {
    passive17_pattern_normal_attack_2(wk, 0x220, 0xB, 0x102);
}

void Passive17_0052(PLW* wk) {
    passive17_pattern_normal_attack(wk, 8, 0x202);
}

void Passive17_0053(PLW* wk) {
    passive17_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive17_0054(PLW* wk) {
    passive17_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5A, 0x5B});
}

void Passive17_0055(PLW* wk) {
    passive17_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive17_0056(PLW* wk) {
    passive17_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7F90, -1, 0x20});
}

void Passive17_0057(PLW* wk) {
    passive17_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7F90, -1, 0x20});
}

void Passive17_0058(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Status(wk, 2, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0059(PLW* wk) {
    passive17_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive17_0060(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Forced_Guard(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0061(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0062(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        ETC_Term(wk, 0, 2, 0xD);
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 0, 6, 0x21);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0064(PLW* wk) {
    passive17_pattern_normal_attack(wk, 8, 0x400);
}

void Passive17_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x4B, 2);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x4B, 2);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0067(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0068(PLW* wk) {
    passive17_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7F90, -1, 0x200});
}

void Passive17_0069(PLW* wk) {
    passive17_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x40, 0});
}

void Passive17_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0071(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0072(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
        break;

    case 2:
        Approach_Walk(wk, 0x4B, 2);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0073(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x68, 0x69, 0x69, 0x6A});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0074(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0075(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        Approach_Walk(wk, 0x41, 2);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0077(PLW* wk) {
    passive17_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x40, 0, -0x7FA0, -1, 0x200}
    );
}

void Passive17_0078(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x200, 0, -0x7F90, -1, 0x40});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0079(PLW* wk) {
    passive17_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x40, 0, -0x7F90, -1, 0x40}
    );
}

void Passive17_0080(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x200, 0, -0x7F90, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0081(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x40, 0, -0x7F90, -1, 0x40});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0082(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x400, 0, -0x7F90, -1, 0x40});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0083(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7F90, -1, 0x40});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0084(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x40, 0});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0085(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 10);
        break;

    case 1:
        Hi_Jump_Attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x400, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0086(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0087(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0088(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0089(PLW* wk) {
    passive17_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive17_0090(PLW* wk) {
    passive17_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive17_0091(PLW* wk) {
    passive17_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive17_0092(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1});
        break;

    case 1:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5A, 0x5B});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0093(PLW* wk) {
    passive17_pattern_normal_attack_2(wk, 0x102, 8, 0x12);
}

void Passive17_0094(PLW* wk) {
    passive17_pattern_lever_attack(wk, 1, 0x20);
}

void Passive17_0095(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x82, 0x83});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0096(PLW* wk) {
    passive17_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FC8, 6, 1, -1}, 8, 0x402);
}

void Passive17_0097(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0098(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0099(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7F90, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0100(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x4B, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0101(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA(wk, 6, 0x2F);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0});
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x82, 0x83});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0102(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1});
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0103(PLW* wk) {
    passive17_pattern_normal_attack(wk, 8, 0x22);
}

void Passive17_0104(PLW* wk) {
    passive17_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0x4008, -1});
}

void Passive17_0105(PLW* wk) {
    passive17_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0x4009, -1});
}

void Passive17_0106(PLW* wk) {
    passive17_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
}

void Passive17_0107(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Walk(wk, 1, 0x20, 0);
        break;

    case 2:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0108(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump(wk, 1, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0109(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 10, -1});
        break;

    case 3:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0110(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Walk(wk, 1, 0x38, 0);
        break;

    case 2:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0111(PLW* wk) {
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

void Passive17_0112(PLW* wk) {
    passive17_pattern_normal_attack(wk, 8, 0x40);
}

void Passive17_0113(PLW* wk) {
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

void Passive17_0114(PLW* wk) {
    passive17_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive17_0115(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
