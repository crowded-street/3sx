/**
 * @file pass07_2.c
 * COM Passive: Ibuki
 * Pattern scripts from Passive07_0055 on.
 */

#include "sf33rd/Source/Game/com/passive/pass07.h"
#include "sf33rd/Source/Game/com/passive/pass07_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive07_0055(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Wait(wk, 4);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0056(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term_2(
        wk,
        &(SA_Term_Args){0x35, 0x36, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive07_0057(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term_3(
        wk,
        &(SA_Term_Args){0x35, 0xFFFF, 0x37, 0},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive07_0058(PLW* wk) {
    pattern_wait_get_up_lever_attack(wk, 0, 1, 0x110);
}

void Passive07_0059(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 10, 0x70, -1, 0x40, 0, -1, -1, 0xFFFF});
}

void Passive07_0060(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 10, 0x70, -1, 0x40, 1, -1, -1, 0xFFFF});
}

void Passive07_0061(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Lie(wk, 0);
        break;

    case 1:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 2:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0062(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 3, 0x402);
}

void Passive07_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x200});
        break;

    case 3:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        pattern_command_attack_from_step_6(wk);
        break;
    }
}

void Passive07_0064(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 0, 0x400);
}

void Passive07_0065(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x400},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive07_0066(PLW* wk) {
    passive07_pattern_wait_get_up_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive07_0067(PLW* wk) {
    passive07_pattern_wait_get_up_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive07_0068(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 10, 0x700});
}

void Passive07_0069(PLW* wk) {
    pattern_wait_get_up(wk, 0, -1);
}

void Passive07_0070(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 0xB, 0x400, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive07_0071(PLW* wk) {
    pattern_wait_lie_jump(wk);
}

void Passive07_0072(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 1:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0073(PLW* wk) {
    pattern_wait_get_up(wk, 3, -1);
}

void Passive07_0074(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FC0, -1, 6, 1, -1}, 0x100);
}

void Passive07_0075(PLW* wk) {
    pattern_em_term_normal_attack_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, -1, 0, 1, -1},
        0x102,
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive07_0076(PLW* wk) {
    passive07_pattern_approach_walk_normal_attack(wk, 0xB, 0x402);
}

void Passive07_0077(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Passive07_0078(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x55, 0x2F, 0x1B, 0x25});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0079(PLW* wk) {
    pattern_pierce_on_command_attack_branch_unit_area(
        wk,
        &(Command_Attack_Args){8, 0, -1, -1},
        &(Branch_Menu_Args){6, 0x53, 0x2F, 0x24, 0x25}
    );
}

void Passive07_0080(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202,
        &(Command_Attack_Args){8, 0x1F, 9, -1}
    );
}

void Passive07_0081(PLW* wk) {
    pattern_jump_attack_term_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive07_0082(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1C, 10, -1},
        0
    );
}

void Passive07_0083(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x110);
}

void Passive07_0084(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x110);
}

void Passive07_0085(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x110);
}

void Passive07_0086(PLW* wk) {
    active_pattern_approach_walk(wk, 199);
}

void Passive07_0087(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
}

void Passive07_0088(PLW* wk) {
    pattern_jump(wk, 0);
}

void Passive07_0089(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, -1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive07_0090(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term(
        wk,
        0x3B,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x20, 8, -1}
    );
}

void Passive07_0091(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0});
        break;

    case 3:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0092(PLW* wk) {
    pattern_approach_walk_lever_attack_sa_term_2(wk, 0x3B, 0x200, &(SA_Term_Args){0x35, 0x36, 0x37, 0});
}

void Passive07_0093(PLW* wk) {
    pattern_em_term_lever_attack_normal_attack(wk, &(EM_Term_Params){-0x7F80, 0x28, 7, 1, -1}, 9, 0x402);
}

void Passive07_0094(PLW* wk) {
    pattern_approach_walk_normal_attack_j_command_attack_2(wk, 0x3B, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive07_0095(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive07_0096(PLW* wk) {
    pattern_sa_term_normal_attack(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0}, 8, 0x400);
}

void Passive07_0097(PLW* wk) {
    pattern_sa_term_j_command_attack(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0}, &(Command_Attack_Args){8, 0x20, 10, -1});
}

void Passive07_0098(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Passive07_0099(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Passive07_0100(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x20, -1);
        break;

    case 1:
        Wait(wk, 0x10);
        break;

    case 2:
        Walk(wk, 0, 0x18, -1);
        break;

    case 3:
        Walk(wk, 1, 0x18, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0101(PLW* wk) {
    pattern_normal_attack_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive07_0102(PLW* wk) {
    pattern_jump_attack_term_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive07_0103(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xB, 0x202, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive07_0104(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 9, 0x402, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive07_0105(PLW* wk) {
    pattern_approach_walk_em_term_sa_term(
        wk,
        0x3B,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive07_0106(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -1, -1, -1});
}

void Passive07_0107(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive07_0108(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 9, -1}
    );
}

void Passive07_0109(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC8, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive07_0110(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term_3(
        wk,
        0x3B,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive07_0111(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term_2(
        wk,
        0x3B,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive07_0112(PLW* wk) {
    active_pattern_approach_walk(wk, 0x78);
}

void Passive07_0113(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x200);
        break;

    case 1:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 1, -1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0114(PLW* wk) {
    pattern_jump_attack_term_normal_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x12,
        0x402
    );
}

void Passive07_0115(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20},
        0x40,
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive07_0116(PLW* wk) {
    pattern_forced_guard(wk, 2);
}

void Passive07_0117(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive07_0118(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive07_0119(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x200);
}
