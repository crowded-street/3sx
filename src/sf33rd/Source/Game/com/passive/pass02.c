/**
 * @file pass02.c
 * COM Passive: Ryu
 */

#include "sf33rd/Source/Game/com/passive/pass02.h"
#include "sf33rd/Source/Game/com/passive/pass02_internal.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive02_Tbl[133])();

void passive02_pattern_em_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive02_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive02_pattern_sa_term(PLW* wk, const SA_Term_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive02_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive02_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data) {
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

void passive02_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive02_pattern_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive02_pattern_check_boss_next_another_menu(PLW* wk, u16 next_menu) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_BOSS(wk, 6, next_menu);
        break;

    case 1:
        Next_Another_Menu(wk, 6, 3);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive02_pattern_wait_get_up(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, lever_data, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive02_pattern_normal_attack_command_attack(
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

void passive02_pattern_em_term_lever_attack(PLW* wk, const EM_Term_Params* p, u16 lever, u16 lever_data) {
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

void Passive02(PLW* wk) {
    Passive02_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive02_0000(PLW* wk) {
    passive02_pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive02_0001(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0002(PLW* wk) {
    passive02_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Passive02_0003(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F88, -1, 0, 1, -1});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, 0x70});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0004(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0005(PLW* wk) {
    passive02_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, 0x700});
}

void Passive02_0006(PLW* wk) {
    passive02_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, 0x70});
}

void Passive02_0007(PLW* wk) {
    passive02_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F88, -1, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive02_0008(PLW* wk) {
    passive02_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F88, -1, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive02_0009(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F88, -1, 0, 1, -1});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x36, 0xFFFF, 0x47});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0010(PLW* wk) {
    passive02_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FB0, -0x7FA8, 0, 1, -1}, 0x40);
}

void Passive02_0011(PLW* wk) {
    passive02_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x200, 0, -0x7F70, -1, 0x20});
}

void Passive02_0012(PLW* wk) {
    passive02_pattern_check_boss_next_another_menu(wk, 0x6F);
}

void Passive02_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0014(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        VS_Jump_Guard(wk);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0015(PLW* wk) {
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

void Passive02_0016(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 0x1C});
        break;

    case 3:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0019(PLW* wk) {
    passive02_pattern_wait_get_up(wk, 0);
}

void Passive02_0020(PLW* wk) {
    passive02_pattern_wait_get_up(wk, 3);
}

void Passive02_0021(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0022(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 8, 0x400, 0, -0x7F68, -1, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0023(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC8, 8, 0x40, 0, -0x7F78, -1, 0x40});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0024(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1});
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0026(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0027(PLW* wk) {
    passive02_pattern_normal_attack_command_attack(wk, 0xB, 0x202, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive02_0028(PLW* wk) {
    passive02_pattern_normal_attack(wk, 8, 0x402);
}

void Passive02_0029(PLW* wk) {
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

void Passive02_0030(PLW* wk) {
    passive02_pattern_normal_attack(wk, 8, 0x400);
}

void Passive02_0031(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x20, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 1, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0035(PLW* wk) {
    passive02_pattern_normal_attack(wk, 8, 0x102);
}

void Passive02_0036(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0037(PLW* wk) {
    passive02_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-1, -0x7FF8, 0, 1, -1}, 0, 0x110);
}

void Passive02_0038(PLW* wk) {
    passive02_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-1, -0x7FF8, 0, 1, -1}, 1, 0x110);
}

void Passive02_0039(PLW* wk) {
    passive02_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD0, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive02_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 0, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 0, 6, 0x1F});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0042(PLW* wk) {
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

void Passive02_0043(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 0, 6, 0x1F});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0044(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 3, 1, -1});
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0045(PLW* wk) {
    passive02_pattern_check_boss_next_another_menu(wk, 0x70);
}

void Passive02_0046(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Status(wk, 2, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0047(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0048(PLW* wk) {
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

void Passive02_0049(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 0x1F});
        break;

    case 1:
        Adjust_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0050(PLW* wk) {
    passive02_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 0x1F}, 0x200);
}

void Passive02_0051(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0052(PLW* wk) {
    passive02_pattern_normal_attack(wk, 8, 0x202);
}

void Passive02_0053(PLW* wk) {
    passive02_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive02_0054(PLW* wk) {
    passive02_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive02_0055(PLW* wk) {
    passive02_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive02_0056(PLW* wk) {
    passive02_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x40, 8, 0x400, 0, -0x7F68, -1, 0x400});
}

void Passive02_0057(PLW* wk) {
    passive02_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC8, 8, 0x100, 0, -0x7F78, -1, 0x20});
}

void Passive02_0058(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Status(wk, 2, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0059(PLW* wk) {
    passive02_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, -1, 5, 6, 0x1F},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive02_0060(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Forced_Guard(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0061(PLW* wk) {
    passive02_pattern_wait_get_up(wk, 0);
}

void Passive02_0062(PLW* wk) {
    passive02_pattern_normal_attack_command_attack(wk, 0xB, 0x200, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive02_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0xFFFF, 0x47});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x19, 0x3C});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0064(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Passive02_Tbl[133])(PLW*) = {
    Passive02_0000, Passive02_0001, Passive02_0002, Passive02_0003, Passive02_0004, Passive02_0005, Passive02_0006,
    Passive02_0007, Passive02_0008, Passive02_0009, Passive02_0010, Passive02_0011, Passive02_0012, Passive02_0013,
    Passive02_0014, Passive02_0015, Passive02_0016, Passive02_0017, Passive02_0018, Passive02_0019, Passive02_0020,
    Passive02_0021, Passive02_0022, Passive02_0023, Passive02_0024, Passive02_0025, Passive02_0026, Passive02_0027,
    Passive02_0028, Passive02_0029, Passive02_0030, Passive02_0031, Passive02_0032, Passive02_0033, Passive02_0034,
    Passive02_0035, Passive02_0036, Passive02_0037, Passive02_0038, Passive02_0039, Passive02_0040, Passive02_0041,
    Passive02_0042, Passive02_0043, Passive02_0044, Passive02_0045, Passive02_0046, Passive02_0047, Passive02_0048,
    Passive02_0049, Passive02_0050, Passive02_0051, Passive02_0052, Passive02_0053, Passive02_0054, Passive02_0055,
    Passive02_0056, Passive02_0057, Passive02_0058, Passive02_0059, Passive02_0060, Passive02_0061, Passive02_0062,
    Passive02_0063, Passive02_0064, Passive02_0065, Passive02_0066, Passive02_0067, Passive02_0068, Passive02_0069,
    Passive02_0070, Passive02_0071, Passive02_0072, Passive02_0073, Passive02_0074, Passive02_0075, Passive02_0076,
    Passive02_0077, Passive02_0078, Passive02_0079, Passive02_0080, Passive02_0081, Passive02_0082, Passive02_0083,
    Passive02_0084, Passive02_0085, Passive02_0086, Passive02_0087, Passive02_0088, Passive02_0089, Passive02_0090,
    Passive02_0091, Passive02_0092, Passive02_0093, Passive02_0094, Passive02_0095, Passive02_0096, Passive02_0097,
    Passive02_0098, Passive02_0099, Passive02_0100, Passive02_0101, Passive02_0102, Passive02_0103, Passive02_0104,
    Passive02_0105, Passive02_0106, Passive02_0107, Passive02_0108, Passive02_0109, Passive02_0110, Passive02_0111,
    Passive02_0112, Passive02_0113, Passive02_0114, Passive02_0115, Passive02_0116, Passive02_0117, Passive02_0118,
    Passive02_0119, Passive02_0120, Passive02_0121, Passive02_0122, Passive02_0123, Passive02_0124, Passive02_0125,
    Passive02_0126, Passive02_0127, Passive02_0128, Passive02_0129, Passive02_0130, Passive02_0131, Passive02_0132
};
