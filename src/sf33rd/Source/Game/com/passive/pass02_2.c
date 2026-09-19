/**
 * @file pass02_2.c
 * COM Passive: Ryu
 * Pattern scripts from Passive02_0065 on.
 */

#include "sf33rd/Source/Game/com/passive/pass02.h"
#include "sf33rd/Source/Game/com/passive/pass02_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive02_0065(PLW* wk) {
    pattern_approach_walk_normal_attack_command_attack(wk, 0xB, 0x200, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive02_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0xFFFF, 0x47});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x19, 0x3C});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0067(PLW* wk) {
    pattern_wait_attack_complete(wk, 0);
}

void Passive02_0068(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Attack_Complete(wk, 3, 1);
        break;

    case 1:
        Next_Be_Passive(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0069(PLW* wk) {
    pattern_wait_attack_complete_sa_term_wait_attack_complete(wk, &(SA_Term_Args){0x35, 0xFFFF, 0x37, 0});
}

static void passive02_0070_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x40, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Wait(wk, 10);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0xFFFF, 0x47});
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x19, 0x3C});
        break;

    case 5:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        passive02_0070_from_step_6(wk);
        break;
    }
}

static void passive02_0071_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Wait(wk, 1);
        break;

    case 7:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0071(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 0, 1, -1});
        break;

    case 2:
        ETC_Term(wk, 8, 6, 7);
        break;

    case 3:
        Check_SA(wk, 6, 7);
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x8016, 10, -1});
        break;

    default:
        passive02_0071_from_step_6(wk);
        break;
    }
}

void Passive02_0072(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x35, 0xFFFF, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive02_0073(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x35, 0xFFFF, 0x37, 0});
        break;

    case 1:
        Adjust_Attack(wk, 0xB, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 0xA, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0074(PLW* wk) {
    pattern_sa_term_command_attack_2(
        wk,
        &(SA_Term_Args){0x35, 0x36, 0xFFFF, 0x47},
        &(SA_Term_Args){0xFFFF, 0xFFFF, 0x19, 0x3C},
        &(Command_Attack_Args){8, 0x1F, 8, -1}
    );
}

void Passive02_0075(PLW* wk) {
    pattern_em_term_normal_attack_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FC0, -1, 5, 6, 0x1F},
        0x12,
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive02_0076(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 0x1C}, 0, 0x20);
}

void Passive02_0077(PLW* wk) {
    pattern_wait_attack_complete_normal_attack(wk);
}

static void passive02_0078_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        EM_Term(wk, &(EM_Term_Params){-0x7F58, -1, 0, 1, -1});
        break;

    case 7:
        Wait(wk, 2);
        break;

    case 8:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0078(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Check_EX(wk, 6, 0x70);
        break;

    case 3:
        Pierce_On(wk);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, 0x700});
        break;

    case 5:
        Check_BOSS_EX(wk, 1, 0xFFFF);
        break;

    default:
        passive02_0078_from_step_6(wk);
        break;
    }
}

void Passive02_0079(PLW* wk) {
    pattern_forced_guard(wk, 2);
}

void Passive02_0080(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x42);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1C, 0xA, -1});
        break;

    case 3:
        Wait(wk, 5);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0081(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x400});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1D, 10, -1});
        break;

    case 3:
        Wait(wk, 5);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0xFFFF, 0x47});
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x19, 0x3C});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0082(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1D, 10, -1});
        break;

    case 4:
        Wait(wk, 5);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0083(PLW* wk) {
    pattern_etc_term_provoke(wk, 4, 0x2A);
}

void Passive02_0084(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 9, 0x100, 1, -0x7FB0, -1, 0x20});
}

void Passive02_0085(PLW* wk) {
    pattern_normal_attack_command_attack_5(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive02_0086(PLW* wk) {
    pattern_keep_away_wait_get_up(wk, 0xBF, 3, -1);
}

void Passive02_0087(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 8, 0x700, -0x7FA0, 0x48, 0, 0x700, 0x30, 0x20});
}

void Passive02_0088(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1D, 10, -1},
        2
    );
}

void Passive02_0089(PLW* wk) {
    pattern_pierce_on_command_attack_branch_unit_area(
        wk,
        &(Command_Attack_Args){8, 0, -1, -1},
        &(Branch_Menu_Args){6, 0x20, 0x3B, 0x36, 0x5A}
    );
}

void Passive02_0090(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive02_0091(PLW* wk) {
    pattern_em_term_command_attack_wait(
        wk,
        &(Command_Attack_Args){0xC, 0x1D, 0xA, -1},
        &(SA_Term_Args){0x35, 0xFFFF, 0x37, 0}
    );
}

void Passive02_0092(PLW* wk) {
    pattern_em_term_com_random_select(
        wk,
        &(EM_Term_Params){-1, -0x7FF0, 0, 1, -1},
        &(Branch_Menu_Args){6, 0x60, 0x61, 0x62, 99}
    );
}

void Passive02_0093(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_7(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x400},
        &(Command_Attack_Args){0xC, 0x1D, 0xA, -1}
    );
}

void Passive02_0094(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 3, 0x3C});
}

void Passive02_0095(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0xD, 0x3B});
}

void Passive02_0096(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0xF, 0x3C});
}

void Passive02_0097(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0xF, 0});
}

void Passive02_0098(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 3, 0x3C});
}

void Passive02_0099(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x11, 0x3C});
}

void Passive02_0100(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FA0, 0, 2, 0}, 0x400);
}

void Passive02_0101(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, 0x30, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive02_0102(PLW* wk) {
    pattern_em_term_jump_attack_term(
        wk,
        &(EM_Term_Params){-1, 0x30, 0, 1, -1},
        &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x200, 0, -0x7F70, -1, 0x20}
    );
}

void Passive02_0103(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F88, -0x7FC0, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive02_0104(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F88, -0x7FC0, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive02_0105(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F88, -0x7FC0, 0, 1, -1},
        &(SA_Term_Args){0xFFFF, 0x36, 0xFFFF, 0x47},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive02_0106(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack_2(wk, 0x60, 9, &(Branch_Menu_Args){6, 0xB, 2, 0xC, 0x5A});
}

void Passive02_0107(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 7, 0x67, 8, 0x68}, 1);
}

void Passive02_0108(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 9, 0x69, 9, 0x69}, 1);
}

void Passive02_0109(PLW* wk) {
    pattern_pierce_on_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, -1, -1},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive02_0110(PLW* wk) {
    passive02_pattern_check_boss_next_another_menu(wk, 0x6A);
}

void Passive02_0111(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F88, -0x7FD0, 0, 2, 0}, 0x400);
}

void Passive02_0112(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FF0, 0, 6, 0x1F},
        &(Command_Attack_Args){8, 0x1F, 8, -1}
    );
}

void Passive02_0113(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_BOSS_EX(wk, 6, 0x72);
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x4C, 0x37, 0x37, 0x30}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0114(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7FD0, -0x7FF0, 5, 6, 1});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x20);
        break;

    case 2:
        Wait(wk, 5);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0xFFFF, 0});
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x19, 0x3C});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0115(PLW* wk) {
    pattern_jump(wk, 0);
}

void Passive02_0116(PLW* wk) {
    pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F90, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200},
        8,
        0x402
    );
}
