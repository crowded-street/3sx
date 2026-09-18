/**
 * @file pass01_2.c
 * COM Passive: Alex
 * Pattern scripts from Passive01_0048 on.
 */

#include "sf33rd/Source/Game/com/passive/pass01.h"
#include "sf33rd/Source/Game/com/passive/pass01_internal.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive01_0048(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0049(PLW* wk) {
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

void Passive01_0050(PLW* wk) {
    passive01_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0x200);
}

void Passive01_0051(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x220);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0052(PLW* wk) {
    passive01_pattern_normal_attack(wk, 8, 0x202);
}

void Passive01_0053(PLW* wk) {
    passive01_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive01_0054(PLW* wk) {
    passive01_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x68, 0x6A, 1, 1});
}

void Passive01_0055(PLW* wk) {
    passive01_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive01_0056(PLW* wk) {
    passive01_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7FA0, -1, 0x20});
}

void Passive01_0057(PLW* wk) {
    passive01_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x20, 0, -0x7FA0, -1, 0x20});
}

void Passive01_0058(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Status(wk, 2, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0059(PLW* wk) {
    passive01_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive01_0060(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Forced_Guard(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0061(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x59, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0x59});
        break;

    case 3:
        Approach_Walk(wk, 0x4F, 2);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0062(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        ETC_Term(wk, 0, 2, 0xD);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 0, 6, 0x21);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x10);
        break;

    case 2:
        Lever_Attack(wk, 8, 1, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0064(PLW* wk) {
    passive01_pattern_normal_attack(wk, 8, 0x400);
}

void Passive01_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}, 1);
        break;

    case 3:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x4F, 2);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0067(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x83, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0068(PLW* wk) {
    passive01_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7FA0, -1, 0x400});
}

void Passive01_0069(PLW* wk) {
    passive01_pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x42, 0});
}

void Passive01_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x83, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, 1, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0071(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0072(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
        break;

    case 2:
        Approach_Walk(wk, 0x4F, 2);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0073(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xC3, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0074(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0x59});
        break;

    case 1:
        Approach_Walk(wk, 0x4F, 2);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0075(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x59, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0x59});
        break;

    case 3:
        Approach_Walk(wk, 0x4F, 2);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0xC3, 2);
        break;

    case 2:
        Jump_Attack(wk, &(Jump_Attack_Args){8, 0xC, 0x42, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0077(PLW* wk) {
    passive01_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x40, 0, -0x7FA0, -1, 0x200}
    );
}

void Passive01_0078(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x100, 0, -0x7FA0, -1, 0x20});
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

void Passive01_0079(PLW* wk) {
    passive01_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20}
    );
}

void Passive01_0080(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x400});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0081(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x100, 0, -0x7FA0, -1, 0x400});
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

void Passive01_0082(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0083(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20});
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

void Passive01_0084(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x42, 0});
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

void Passive01_0085(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 0xA);
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 5, 9, 0x35, 1}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0086(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x59, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0x59});
        break;

    case 3:
        Approach_Walk(wk, 0x4F, 2);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0087(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x59, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0x59});
        break;

    case 3:
        Approach_Walk(wk, 0x4F, 2);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0088(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FA0, 6, 1, -1});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0089(PLW* wk) {
    passive01_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive01_0090(PLW* wk) {
    passive01_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive01_0091(PLW* wk) {
    passive01_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive01_0092(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x83, 3);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FA0, 6, 1, -1});
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0093(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0094(PLW* wk) {
    passive01_pattern_lever_attack(wk, 0, 0x20);
}

void Passive01_0095(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x68, 0x69, 0x6A, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0096(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x83, 3);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -0x7FD8, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0097(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0098(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0099(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0100(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x4F, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0101(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0x59});
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0102(PLW* wk) {
    passive01_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0, 0x20);
}

void Passive01_0103(PLW* wk) {
    passive01_pattern_normal_attack(wk, 8, 0x42);
}

void Passive01_0104(PLW* wk) {
    passive01_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive01_0105(PLW* wk) {
    passive01_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive01_0106(PLW* wk) {
    passive01_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Passive01_0107(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Walk(wk, 1, 0x20, 0);
        break;

    case 2:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0108(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump(wk, 1, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0109(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xA, -1});
        break;

    case 3:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0110(PLW* wk) {
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
