/**
 * @file pass01.c
 * COM Passive: Alex
 */

#include "sf33rd/Source/Game/com/passive/pass01.h"
#include "sf33rd/Source/Game/com/passive/pass01_internal.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive01_Tbl[176])();

void passive01_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive01_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive01_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive01_pattern_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive01_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive01_pattern_jump_attack(PLW* wk, const Jump_Attack_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive01_pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, lever, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive01_pattern_check_ex_j_command_attack(PLW* wk, s16 next_menu, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, next_menu);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive01_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive01_pattern_wait_get_up(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, lever_data, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive01_pattern_normal_attack_command_attack(
    PLW* wk, s16 reaction, u16 lever_data, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive01_pattern_em_term_lever_attack(PLW* wk, const EM_Term_Params* p, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Lever_Attack(wk, 8, lever, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive01_pattern_branch_unit_area(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Branch_Unit_Area(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive01_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01(PLW* wk) {
    Passive01_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive01_0000(PLW* wk) {
    passive01_pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive01_0001(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Off(wk);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0002(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1});
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0003(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        VS_Jump_Guard(wk);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0004(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x53);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0005(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Forced_Guard(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0006(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x83, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0007(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x83, 2);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0008(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0009(PLW* wk) {
    passive01_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Passive01_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Short_Range_Attack(wk, &(Short_Range_Args){8, 0x40, 6, 0x1D});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0011(PLW* wk) {
    passive01_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x20, 2, -0x7FA0, -1, 0x20});
}

void Passive01_0012(PLW* wk) {
    passive01_pattern_normal_attack(wk, 8, 0x400);
}

void Passive01_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0014(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 3);
        break;

    case 1:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0015(PLW* wk) {
    passive01_pattern_wait_get_up(wk, 0);
}

void Passive01_0016(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, 0x8D, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 4:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
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

void Passive01_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, 0x8D, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 5:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0019(PLW* wk) {
    passive01_pattern_wait_get_up(wk, 3);
}

void Passive01_0020(PLW* wk) {
    passive01_pattern_wait_get_up(wk, 3);
}

void Passive01_0021(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0022(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F80, -0x7FB0, 0xB, 0x100, 0, -0x7FA0, -1, 0x200});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0023(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x20, 0, -0x7FA0, -1, 0x200});
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0024(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, 0x8D, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive01_0025_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, 0x8D, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        passive01_0025_from_step_6(wk);
        break;
    }
}

void Passive01_0026(PLW* wk) {
    passive01_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 0x42);
}

void Passive01_0027(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0028(PLW* wk) {
    passive01_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xA, 0x400, 1});
}

void Passive01_0029(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
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

void Passive01_0030(PLW* wk) {
    passive01_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FB8, 6, 1, -1}, 0x400);
}

void Passive01_0031(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x20, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0x59});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0034(PLW* wk) {
    passive01_pattern_normal_attack_command_attack(wk, 0xC, 0x10, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive01_0035(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0036(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1E, 0xA, -1});
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0037(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x4F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0039(PLW* wk) {
    passive01_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive01_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
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

void Passive01_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x83, 3);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -0x7FC0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0042(PLW* wk) {
    passive01_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x42, 2});
}

void Passive01_0043(PLW* wk) {
    passive01_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Passive01_0044(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x4F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0045(PLW* wk) {
    passive01_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FD8, 6, 1, -1}, 1, 0x40);
}

void Passive01_0046(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Status(wk, 2, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0047(PLW* wk) {
    passive01_pattern_normal_attack_command_attack(wk, 0xB, 0x102, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void (*const Passive01_Tbl[176])(PLW*) = {
    Passive01_0000, Passive01_0001, Passive01_0002, Passive01_0003, Passive01_0004, Passive01_0005, Passive01_0006,
    Passive01_0007, Passive01_0008, Passive01_0009, Passive01_0010, Passive01_0011, Passive01_0012, Passive01_0013,
    Passive01_0014, Passive01_0015, Passive01_0016, Passive01_0017, Passive01_0018, Passive01_0019, Passive01_0020,
    Passive01_0021, Passive01_0022, Passive01_0023, Passive01_0024, Passive01_0025, Passive01_0026, Passive01_0027,
    Passive01_0028, Passive01_0029, Passive01_0030, Passive01_0031, Passive01_0032, Passive01_0033, Passive01_0034,
    Passive01_0035, Passive01_0036, Passive01_0037, Passive01_0038, Passive01_0039, Passive01_0040, Passive01_0041,
    Passive01_0042, Passive01_0043, Passive01_0044, Passive01_0045, Passive01_0046, Passive01_0047, Passive01_0048,
    Passive01_0049, Passive01_0050, Passive01_0051, Passive01_0052, Passive01_0053, Passive01_0054, Passive01_0055,
    Passive01_0056, Passive01_0057, Passive01_0058, Passive01_0059, Passive01_0060, Passive01_0061, Passive01_0062,
    Passive01_0063, Passive01_0064, Passive01_0065, Passive01_0066, Passive01_0067, Passive01_0068, Passive01_0069,
    Passive01_0070, Passive01_0071, Passive01_0072, Passive01_0073, Passive01_0074, Passive01_0075, Passive01_0076,
    Passive01_0077, Passive01_0078, Passive01_0079, Passive01_0080, Passive01_0081, Passive01_0082, Passive01_0083,
    Passive01_0084, Passive01_0085, Passive01_0086, Passive01_0087, Passive01_0088, Passive01_0089, Passive01_0090,
    Passive01_0091, Passive01_0092, Passive01_0093, Passive01_0094, Passive01_0095, Passive01_0096, Passive01_0097,
    Passive01_0098, Passive01_0099, Passive01_0100, Passive01_0101, Passive01_0102, Passive01_0103, Passive01_0104,
    Passive01_0105, Passive01_0106, Passive01_0107, Passive01_0108, Passive01_0109, Passive01_0110, Passive01_0111,
    Passive01_0112, Passive01_0113, Passive01_0114, Passive01_0115, Passive01_0116, Passive01_0117, Passive01_0118,
    Passive01_0119, Passive01_0120, Passive01_0121, Passive01_0122, Passive01_0123, Passive01_0124, Passive01_0125,
    Passive01_0126, Passive01_0127, Passive01_0128, Passive01_0129, Passive01_0130, Passive01_0131, Passive01_0132,
    Passive01_0133, Passive01_0134, Passive01_0135, Passive01_0136, Passive01_0137, Passive01_0138, Passive01_0139,
    Passive01_0140, Passive01_0141, Passive01_0142, Passive01_0143, Passive01_0144, Passive01_0145, Passive01_0146,
    Passive01_0147, Passive01_0148, Passive01_0149, Passive01_0150, Passive01_0151, Passive01_0152, Passive01_0153,
    Passive01_0154, Passive01_0155, Passive01_0156, Passive01_0157, Passive01_0158, Passive01_0159, Passive01_0160,
    Passive01_0161, Passive01_0162, Passive01_0163, Passive01_0164, Passive01_0165, Passive01_0166, Passive01_0167,
    Passive01_0168, Passive01_0169, Passive01_0170, Passive01_0171, Passive01_0172, Passive01_0173, Passive01_0174,
    Passive01_0175
};
