/**
 * @file pass09_2.c
 * COM Passive: Oro
 * Pattern scripts from Passive09_0051 on.
 */

#include "sf33rd/Source/Game/com/passive/pass09.h"
#include "sf33rd/Source/Game/com/passive/pass09_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive09_0051(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x12, 0x12);
}

void Passive09_0052(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Passive09_0053(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){0x58, -1, 5, 6, 0x2A},
        &(Command_Attack_Args){8, 0x1F, 8, -1}
    );
}

void Passive09_0054(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive09_0055(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F60, -1, 5, 6, 0x2A},
        &(Command_Attack_Args){8, 0x1F, 9, -1}
    );
}

void Passive09_0056(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FB0, -0x7FC0, 8, 0x400, 0, -0x7F68, -1, 0x400});
}

void Passive09_0057(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F98, -0x7FA8, 8, 0x100, 0, -0x7F78, -1, 0x20});
}

void Passive09_0058(PLW* wk) {
    pattern_keep_status(wk, 1);
}

void Passive09_0059(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 3, 6, 0x1F});
        break;

    case 2:
        Check_Store_Lever(wk, 0x1C, 1, -1);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0060(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive09_0061(PLW* wk) {
    pattern_wait_get_up_2(wk, 0);
}

void Passive09_0062(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x200);
        break;

    case 1:
        ETC_Term(wk, 1, 6, 0x6E);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0063(PLW* wk) {
    pattern_sa_term_command_attack_2(
        wk,
        &(SA_Term_Args){0x35, 0xFFFF, 0xFFFF, 0x9F},
        &(SA_Term_Args){0xFFFF, 0x64, 0x3A, 0},
        &(Command_Attack_Args){8, 0x8016, 10, -1}
    );
}

void Passive09_0064(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        ETC_Term(wk, 1, 6, 0x6E);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0065(PLW* wk) {
    pattern_approach_walk_normal_attack_command_attack(wk, 0xC, 0x200, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive09_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x64, 0xFFFF, 0});
        break;

    case 2:
        Approach_Walk(wk, 0x9F, 2);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x35, 0xFFFF, 0xFFFF, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0067(PLW* wk) {
    pattern_wait_attack_complete(wk, 0);
}

void Passive09_0068(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 0, 0x15, 0);
        break;

    case 1:
        Walk(wk, 1, 0x13, 0);
        break;

    case 2:
        Walk(wk, 0, 0x10, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0069(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Attack_Complete(wk, 3, 1);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x3A, 0});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x35, 0xFFFF, 0xFFFF, 0x9F});
        break;

    case 3:
        Wait_Attack_Complete(wk, 3, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0070(PLW* wk) {
    pattern_approach_walk_sa_term(wk, 0x7F, &(SA_Term_Args){0x35, 0x64, 0x3A, 0});
}

void Passive09_0071(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0xFFFF, 0xFFFF, 0x3A, 0},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive09_0072(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x35, 0xFFFF, 0xFFFF, 0});
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7F90, -0x7FF0, 3, 6, 0x1C});
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0073(PLW* wk) {
    pattern_normal_attack_7(
        wk,
        &(Normal_Attack_Step){ 9, 0x100 },
        &(Normal_Attack_Step){ 9, 0x10 },
        &(Normal_Attack_Step){ 9, 0x20 }
    );
}

void Passive09_0074(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0x35, 0x64, 0x3A, 0});
}

void Passive09_0075(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6C);
        break;

    case 1:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-0x7FC0, -1, 3, 6, 0x1F});
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x12);
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6E);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 0x1C});
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0077(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x40, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7FB8, -0x7FF0, 3, 6, 0x1C});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x200);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0078(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 8, 0x400, 2, -0x7F70, -1, 0x400});
}

void Passive09_0079(PLW* wk) {
    pattern_forced_guard(wk, 2);
}

void Passive09_0080(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, -0x5FF7, 0x400, 0, -0x7FB0, -1, 0x400},
        0x22,
        &(Command_Attack_Args){0xC, 0x1C, 10, -1}
    );
}

void Passive09_0081(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F98, -0x7FA8, 9, 0x40, 0, -0x7FB0, -1, 0x400},
        0x200,
        &(Command_Attack_Args){0xC, 0x1D, 8, -1}
    );
}

void Passive09_0082(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0083(PLW* wk) {
    pattern_jump(wk, 2);
}

void Passive09_0084(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 9, 0x100, 1, -0x7FB0, -1, 0x400});
}

void Passive09_0085(PLW* wk) {
    pattern_normal_attack_command_attack_5(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive09_0086(PLW* wk) {
    pattern_keep_away_wait_get_up(wk, 0xBF, 3, -1);
}

void Passive09_0087(PLW* wk) {
    pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FC0, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x400},
        0xB,
        0x400
    );
}

void Passive09_0088(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, -0x5FF7, 0x200, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1E, 8, -1},
        2
    );
}

void Passive09_0089(PLW* wk) {
    pattern_pierce_on_command_attack_branch_unit_area(
        wk,
        &(Command_Attack_Args){8, 0, -1, -1},
        &(Branch_Menu_Args){6, 0x20, 0x3B, 0x36, 0x5A}
    );
}

void Passive09_0090(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive09_0091(PLW* wk) {
    pattern_em_term_command_attack_wait(
        wk,
        &(Command_Attack_Args){0xC, 0x1E, 10, -1},
        &(SA_Term_Args){0x35, 0xFFFF, 0x3A, 0}
    );
}

void Passive09_0092(PLW* wk) {
    passive09_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x60, 0x61, 0x62, 99});
}

void Passive09_0093(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_7(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FA8, 9, 0x40, 0, -0x7FB0, -1, 0x400},
        &(Command_Attack_Args){0xC, 0x1E, 10, -1}
    );
}

void Passive09_0094(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0xFFFF, 0x64, 0xFFFF, 0x36});
}

void Passive09_0095(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0xFFFF, 0x64, 0xFFFF, 0x37});
}

void Passive09_0096(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0xFFFF, 0x64, 0xFFFF, 0x38});
}

void Passive09_0097(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0xFFFF, 0x64, 0xFFFF, 0x39});
}

void Passive09_0098(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 5, 6, 0x1F});
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x200);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0099(PLW* wk) {
    pattern_etc_term_normal_attack_com_random_select(
        wk,
        &(ETC_Term_Step){ 1, 6, 0x6D },
        &(Normal_Attack_Step){ 0xC, 0x20 },
        &(Com_Random_Select_Step){ &(Branch_Menu_Args){6, 5, 5, 100, 0x5A}, 4 }
    );
}

void Passive09_0100(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive09_0101(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FC0, 0, -1, -1, 0xFFFF});
}

void Passive09_0102(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0103(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0104(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Jump(wk, 0);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0105(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Jump(wk, 0);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0106(PLW* wk) {
    pattern_lever_on_em_term_j_command_attack(
        wk,
        0xFFFF,
        &(EM_Term_Params){-1, 0x30, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive09_0107(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7F90, 6, 2, 0}, 0x400);
}

void Passive09_0108(PLW* wk) {
    passive09_pattern_em_term_adjust_attack(wk, &(EM_Term_Params){-0x7FC8, -0x7FF8, 5, 1, -1}, 0x10);
}

void Passive09_0109(PLW* wk) {
    passive09_pattern_em_term_adjust_attack(wk, &(EM_Term_Params){-0x7FC0, -0x7FF8, 5, 1, -1}, 0x20);
}

void Passive09_0110(PLW* wk) {
    passive09_pattern_em_term_adjust_attack(wk, &(EM_Term_Params){-0x7F98, -0x7FF8, 5, 1, -1}, 0x40);
}

void Passive09_0111(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Passive09_0112(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_JCA_Term(wk, &(ORO_JCA_Term_Args){8, 1, 0x40, 0, -0x7FA0, -1, 0x2F, 0xA, -1, -0x7FB0, -0x7FD0, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0113(PLW* wk) {
    pattern_oro_hjca_term(wk, &(ORO_JCA_Term_Args){8, 1, 0x40, 0, -0x7FA0, -1, 0x2F, 0xA, -1, -0x7FB0, -0x7FD0, 0x400});
}

void Passive09_0114(PLW* wk) {
    pattern_oro_hjca_term(wk, &(ORO_JCA_Term_Args){8, 2, 0x40, 0, -0x7FA0, -1, 0x2F, 0xA, -1, -0x7FB0, -0x7FD0, 0x400});
}

void Passive09_0115(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FD0, 0x10, 8, 0x110, 0, -0x7FA0, -1, 0x20});
}
