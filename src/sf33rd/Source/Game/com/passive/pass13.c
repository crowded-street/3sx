/**
 * @file pass13.c
 * COM Passive: Urien
 */

#include "sf33rd/Source/Game/com/passive/pass13.h"
#include "sf33rd/Source/Game/com/passive/pass13_internal.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive13_Tbl[164])();

void passive13_pattern_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive13_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive13_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive13_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data) {
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

void passive13_pattern_lever_attack(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, reaction, 0, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive13_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive13_pattern_normal_attack_2(PLW* wk, s16 reaction, u16 lever_data, u16 lever_data_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive13_pattern_wait_get_up(PLW* wk, u16 lever_data, s16 option) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, lever_data, option);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive13_pattern_pierce_on_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        EM_Term(wk, p);
        break;

    case 2:
        Normal_Attack(wk, reaction, 0x42);
        break;

    case 3:
        Wait(wk, 5);
        break;

    case 4:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive13_pattern_hi_jump_attack_term_normal_attack(
    PLW* wk, const Hi_Jump_Term_Args* a, u16 lever_data, u16 lever_data_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 0xB, lever_data);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13(PLW* wk) {
    Passive13_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive13_0000(PLW* wk) {
    passive13_pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive13_0001(PLW* wk) {
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

void Passive13_0002(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 10, -1});
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0003(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        VS_Jump_Guard(wk);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0004(PLW* wk) {
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

void Passive13_0005(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Forced_Guard(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0006(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7B, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0007(PLW* wk) {
    passive13_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive13_0008(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 1, 0xB, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0009(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, 0x38, 7, 1, -1});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Short_Range_Attack(wk, &(Short_Range_Args){8, 0x40, 6, 0x1D});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0011(PLW* wk) {
    passive13_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive13_0012(PLW* wk) {
    passive13_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, 0, 0, 2, 0}, 0x20);
}

void Passive13_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x22);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0014(PLW* wk) {
    passive13_pattern_normal_attack(wk, 8, 0x200);
}

void Passive13_0015(PLW* wk) {
    passive13_pattern_wait_get_up(wk, 0, -1);
}

void Passive13_0016(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x71, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x71, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x20);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x71, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x20);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0019(PLW* wk) {
    passive13_pattern_wait_get_up(wk, 0, 1);
}

void Passive13_0020(PLW* wk) {
    passive13_pattern_wait_get_up(wk, 3, -1);
}

void Passive13_0021(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0022(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, -0x7FC0, -0x5FF5, 0x40, 0, -0x7F90, -1, 0x20});
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x20);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0023(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, -0x7FC0, -0x5FF5, 0x400, 0, -0x7F90, -1, 0x200});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0024(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x68);
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

void Passive13_0026(PLW* wk) {
    passive13_pattern_normal_attack(wk, 8, 0x20);
}

void Passive13_0027(PLW* wk) {
    passive13_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive13_0028(PLW* wk) {
    passive13_pattern_normal_attack(wk, 8, 0x220);
}

void Passive13_0029(PLW* wk) {
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

void Passive13_0030(PLW* wk) {
    passive13_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FD0, 6, 1, -1}, 0x40);
}

void Passive13_0031(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x20, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0032(PLW* wk) {
    passive13_pattern_normal_attack_2(wk, 9, 0x100, 0x200);
}

void Passive13_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x33, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0034(PLW* wk) {
    passive13_pattern_normal_attack(wk, 8, 0x400);
}

void Passive13_0035(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0036(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 3:
        Next_Another_Menu(wk, 6, 0x8B);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0037(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x33, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FFE, 6, 1, -1});
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0038(PLW* wk) {
    passive13_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FFE, 6, 1, -1}, 0x110);
}

void Passive13_0039(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7B, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x42);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0040(PLW* wk) {
    passive13_pattern_pierce_on_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FB0, 6, 1, -1}, 9, 0x400);
}

void Passive13_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x100);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0042(PLW* wk) {
    passive13_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, -0x5FF8, 0x40, 0, -0x7F90, -1, 0x40});
}

void Passive13_0043(PLW* wk) {
    passive13_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FB0, 6, 1, -1}, 0x400);
}

void Passive13_0044(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7F90, 6, 1, -1});
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0046(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Status(wk, 2, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0047(PLW* wk) {
    passive13_pattern_normal_attack_2(wk, 8, 0x102, 0x402);
}

void Passive13_0048(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -0x7FC0, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x12);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0049(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0050(PLW* wk) {
    passive13_pattern_normal_attack_2(wk, 9, 0x12, 0x22);
}

void Passive13_0051(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, &(Command_Attack_Args){0xB, 0xE, 10, -1});
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0052(PLW* wk) {
    passive13_pattern_normal_attack(wk, 8, 0x202);
}

void Passive13_0053(PLW* wk) {
    passive13_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive13_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7B, 2);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0055(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7B, 2);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){9, 0x1F, 8, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive13_0056(PLW* wk) {
    passive13_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, -0x5FF8, 0x40, 0, -0x7F90, -1, 0x400});
}

void Passive13_0057(PLW* wk) {
    passive13_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, -0x5FF8, 0x40, 0, -0x7F90, -1, 0x20});
}

void Passive13_0058(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Status(wk, 2, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Passive13_Tbl[164])(PLW*) = {
    Passive13_0000, Passive13_0001, Passive13_0002, Passive13_0003, Passive13_0004, Passive13_0005, Passive13_0006,
    Passive13_0007, Passive13_0008, Passive13_0009, Passive13_0010, Passive13_0011, Passive13_0012, Passive13_0013,
    Passive13_0014, Passive13_0015, Passive13_0016, Passive13_0017, Passive13_0018, Passive13_0019, Passive13_0020,
    Passive13_0021, Passive13_0022, Passive13_0023, Passive13_0024, Passive13_0025, Passive13_0026, Passive13_0027,
    Passive13_0028, Passive13_0029, Passive13_0030, Passive13_0031, Passive13_0032, Passive13_0033, Passive13_0034,
    Passive13_0035, Passive13_0036, Passive13_0037, Passive13_0038, Passive13_0039, Passive13_0040, Passive13_0041,
    Passive13_0042, Passive13_0043, Passive13_0044, Passive13_0045, Passive13_0046, Passive13_0047, Passive13_0048,
    Passive13_0049, Passive13_0050, Passive13_0051, Passive13_0052, Passive13_0053, Passive13_0054, Passive13_0055,
    Passive13_0056, Passive13_0057, Passive13_0058, Passive13_0059, Passive13_0060, Passive13_0061, Passive13_0062,
    Passive13_0063, Passive13_0064, Passive13_0065, Passive13_0066, Passive13_0067, Passive13_0068, Passive13_0069,
    Passive13_0070, Passive13_0071, Passive13_0072, Passive13_0073, Passive13_0074, Passive13_0075, Passive13_0076,
    Passive13_0077, Passive13_0078, Passive13_0079, Passive13_0080, Passive13_0081, Passive13_0082, Passive13_0083,
    Passive13_0084, Passive13_0085, Passive13_0086, Passive13_0087, Passive13_0088, Passive13_0089, Passive13_0090,
    Passive13_0091, Passive13_0092, Passive13_0093, Passive13_0094, Passive13_0095, Passive13_0096, Passive13_0097,
    Passive13_0098, Passive13_0099, Passive13_0100, Passive13_0101, Passive13_0102, Passive13_0103, Passive13_0104,
    Passive13_0105, Passive13_0106, Passive13_0107, Passive13_0108, Passive13_0109, Passive13_0110, Passive13_0111,
    Passive13_0112, Passive13_0113, Passive13_0114, Passive13_0115, Passive13_0116, Passive13_0117, Passive13_0118,
    Passive13_0119, Passive13_0120, Passive13_0121, Passive13_0122, Passive13_0123, Passive13_0124, Passive13_0125,
    Passive13_0126, Passive13_0127, Passive13_0128, Passive13_0129, Passive13_0130, Passive13_0131, Passive13_0132,
    Passive13_0133, Passive13_0134, Passive13_0135, Passive13_0136, Passive13_0137, Passive13_0138, Passive13_0139,
    Passive13_0140, Passive13_0141, Passive13_0142, Passive13_0143, Passive13_0144, Passive13_0145, Passive13_0146,
    Passive13_0147, Passive13_0148, Passive13_0149, Passive13_0150, Passive13_0151, Passive13_0152, Passive13_0153,
    Passive13_0154, Passive13_0155, Passive13_0156, Passive13_0157, Passive13_0158, Passive13_0159, Passive13_0160,
    Passive13_0161, Passive13_0162, Passive13_0163
};
