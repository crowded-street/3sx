/**
 * @file pass14_2.c
 * COM Passive: Akuma/Gouki
 * Pattern scripts from Passive14_0035 on.
 */

#include "sf33rd/Source/Game/com/passive/pass14.h"
#include "sf33rd/Source/Game/com/passive/pass14_internal.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

static void passive14_0035_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Wait(wk, 3);
        break;

    case 7:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 10, -1});
        break;

    case 8:
        Wait(wk, 3);
        break;

    case 9:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x31, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0035(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){0xB, 0x20, 8, -1});
        break;

    case 5:
        Pierce_On(wk);
        break;

    default:
        passive14_0035_from_step_6(wk);
        break;
    }
}

void Passive14_0036(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x30, 0x31, 0x7F});
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive14_pattern_em_term_sa_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        SA_Term(wk, p_b);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0037(PLW* wk) {
    passive14_pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FA0, -0x7FA0, 6, 6, 0x35},
        &(SA_Term_Args){0xFFFF, 0x30, 0x31, 0x7F}
    );
}

void Passive14_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x30, 0x31, 0x7F});
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0039(PLW* wk) {
    passive14_pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 6, 0x35},
        &(SA_Term_Args){0xFFFF, 0x30, 0x31, 0x7F}
    );
}

void Passive14_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 3);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7FC0, -0x7FC8, 6, 6, 0x35});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 6, 0x35});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive14_pattern_command_attack_2(PLW* wk, const Command_Attack_Args* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0042(PLW* wk) {
    passive14_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 1, -1, -1},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

static void passive14_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0043(PLW* wk) {
    passive14_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, 0x38, 8, 0x200, 2, -0x7FA0, -1, 0x200});
}

void Passive14_0044(PLW* wk) {
    passive14_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, 0x38, 8, 0x400, 2, -0x7FA0, -1, 0x400});
}

void Passive14_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x2F, 10, -1, -1, 0x10, 2, -1, -1, 0xFFFF});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x31, 0x7F});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0046(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1F, 10, -1});
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0047(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 10, -1});
        break;

    case 4:
        Wait(wk, 3);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0048(PLW* wk) {
    passive14_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F80, 0x20, 8, 0x400, 1, -0x7F80, -1, 0x400});
}

void passive14_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0049(PLW* wk) {
    passive14_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FB0, -0x7FB0, 6, 6, -1}, 8, 0x400);
}

void Passive14_0050(PLW* wk) {
    passive14_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FC0, -0x7FF0, 6, 1, -1}, 8, 0x100);
}

void Passive14_0051(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -0x7FC0, 0, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive14_pattern_approach_walk_normal_attack(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0052(PLW* wk) {
    passive14_pattern_approach_walk_normal_attack(wk, 0xB, 0x402);
}

void Passive14_0053(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Forced_Guard(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0055(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive14_pattern_wait_get_up_normal_attack(PLW* wk, u16 lever_data, u16 lever_data_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, lever_data, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0056(PLW* wk) {
    passive14_pattern_wait_get_up_normal_attack(wk, 0, 0x402);
}

void Passive14_0057(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0058(PLW* wk) {
    passive14_pattern_wait_get_up_normal_attack(wk, 3, 0x402);
}

void Passive14_0059(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive14_pattern_keep_away_wait_get_up(PLW* wk, s16 target_pos, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, target_pos, 0);
        break;

    case 1:
        Wait_Get_Up(wk, lever_data, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0060(PLW* wk) {
    passive14_pattern_keep_away_wait_get_up(wk, 0xBF, 0);
}

void Passive14_0061(PLW* wk) {
    passive14_pattern_keep_away_wait_get_up(wk, 0xBF, 3);
}

void Passive14_0062(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xBF, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 4, 6, 0x37);
        break;

    case 1:
        Provoke(wk, -1);
        break;

    case 2:
        Next_Another_Menu(wk, 6, 0x39);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0064(PLW* wk) {
    passive14_pattern_keep_away_wait_get_up(wk, 0x7F, 0);
}

void Passive14_0065(PLW* wk) {
    passive14_pattern_keep_away_wait_get_up(wk, 0x7F, 3);
}

void Passive14_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0x7F, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0067(PLW* wk) {
    passive14_pattern_wait_get_up_normal_attack(wk, 3, 0x202);
}

void Passive14_0068(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Search_Back_Term(wk, 0xE0, 6, 0x45);
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x200, 0, -0x7F80, -1, 0x400});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive14_pattern_wait_get_up_branch_wait_area_command_attack(
    PLW* wk, const Branch_Wait_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Branch_Wait_Area(wk, p);
        break;

    case 2:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0069(PLW* wk) {
    passive14_pattern_wait_get_up_branch_wait_area_command_attack(
        wk,
        &(Branch_Wait_Args){0x14, 0xF, 5, 1},
        &(Command_Attack_Args){8, 0x1F, 9, -1}
    );
}

void Passive14_0070(PLW* wk) {
    passive14_pattern_wait_get_up_branch_wait_area_command_attack(
        wk,
        &(Branch_Wait_Args){0x14, 0xF, 5, 1},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive14_0071(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0072(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0073(PLW* wk) {
    passive14_pattern_wait_get_up_branch_wait_area_command_attack(
        wk,
        &(Branch_Wait_Args){10, 7, 3, 1},
        &(Command_Attack_Args){8, 0x21, 8, -1}
    );
}

void Passive14_0074(PLW* wk) {
    passive14_pattern_wait_get_up_branch_wait_area_command_attack(
        wk,
        &(Branch_Wait_Args){10, 7, 3, 1},
        &(Command_Attack_Args){8, 0x21, 9, -1}
    );
}

void Passive14_0075(PLW* wk) {
    passive14_pattern_wait_get_up_branch_wait_area_command_attack(
        wk,
        &(Branch_Wait_Args){10, 7, 3, 1},
        &(Command_Attack_Args){8, 0x21, 10, -1}
    );
}

void Passive14_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Branch_Wait_Area(wk, &(Branch_Wait_Args){0xF, 10, 5, 1});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0077(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Search_Back_Term(wk, 0xE0, 6, 0x45);
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FE0, 0xB, 0x200, 0, -0x7F80, -1, 0x400});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0078(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 4, 6, 0x37);
        break;

    case 1:
        Provoke(wk, -1);
        break;

    case 2:
        Next_Another_Menu(wk, 6, 0x46);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive14_pattern_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0079(PLW* wk) {
    passive14_pattern_command_attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
}

void Passive14_0080(PLW* wk) {
    passive14_pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0, -1, -1}, &(Command_Attack_Args){8, 0, -1, -1});
}

void Passive14_0081(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x300, 6, 0xE3);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x47});
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 1});
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0082(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x80, 6, 0xE3);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x47});
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 1});
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0083(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x200, 6, 0xE4);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x47});
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 1});
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0084(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x100, 6, 0xE4);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x47});
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 1});
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0085(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1F, 10, -1});
        break;

    case 2:
        Wait(wk, 1);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
