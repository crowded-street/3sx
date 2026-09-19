/**
 * @file pass08_2.c
 * COM Passive: Elena
 * Pattern scripts from Passive08_0056 on.
 */

#include "sf33rd/Source/Game/com/passive/pass08.h"
#include "sf33rd/Source/Game/com/passive/pass08_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive08_0056(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0xBC);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 8, 0x700});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, -1, -1});
        break;

    case 3:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, -1, -1});
        break;

    case 5:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    default:
        passive08_0056_from_step_6(wk);
        break;
    }
}

void Passive08_0057(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term_3(
        wk,
        &(SA_Term_Args){0x39, 0xFFFF, 0x3B, 0},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive08_0058(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive08_0059(PLW* wk) {
    pattern_wait_get_up_lever_attack(wk, 0, 0, 0x110);
}

void Passive08_0060(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 0x3A, &(Command_Attack_Args){8, 0x1E, 10, 0x70});
}

void Passive08_0061(PLW* wk) {
    passive08_pattern_wait_lie_approach_walk_wait_get_up(wk, 0x3F, 3);
}

void Passive08_0062(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 3, 0x402);
}

void Passive08_0063(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, 0x700}
    );
}

void Passive08_0064(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 0, 0x400);
}

void Passive08_0065(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x400},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive08_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Walk(wk, 0, 0x10, 0);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
        break;

    case 4:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, -1, -1});
        break;

    default:
        pattern_search_back_term_command_attack_from_step_6(wk);
        break;
    }
}

void Passive08_0067(PLW* wk) {
    pattern_wait_get_up_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive08_0068(PLW* wk) {
    passive08_pattern_wait_lie_approach_walk_wait_get_up(wk, 0x7B, 3);
}

void Passive08_0069(PLW* wk) {
    pattern_wait_get_up(wk, 0, -1);
}

void Passive08_0070(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 0xB, 0x400, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive08_0071(PLW* wk) {
    pattern_wait_lie_jump(wk);
}

void Passive08_0072(PLW* wk) {
    passive08_pattern_wait_lie_approach_walk_wait_get_up(wk, 0xBF, 0);
}

void Passive08_0073(PLW* wk) {
    pattern_wait_get_up(wk, 3, -1);
}

void Passive08_0074(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FC0, -1, 6, 1, -1}, 0x100);
}

void Passive08_0075(PLW* wk) {
    pattern_em_term_normal_attack_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, -1, 0, 1, -1},
        0x102,
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive08_0076(PLW* wk) {
    passive08_pattern_approach_walk_normal_attack(wk, 0xB, 0x402);
}

void Passive08_0077(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Passive08_0078(PLW* wk) {
    pattern_pierce_on_command_attack_branch_unit_area(
        wk,
        &(Command_Attack_Args){8, 0, -1, -1},
        &(Branch_Menu_Args){6, 0x55, 0x2F, 0x1B, 0x25}
    );
}

void Passive08_0079(PLW* wk) {
    pattern_pierce_on_command_attack_branch_unit_area(
        wk,
        &(Command_Attack_Args){8, 0, -1, -1},
        &(Branch_Menu_Args){6, 0x53, 0x2F, 0x24, 0x25}
    );
}

void Passive08_0080(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202,
        &(Command_Attack_Args){8, 0x1D, 9, -1}
    );
}

void Passive08_0081(PLW* wk) {
    pattern_jump_attack_term_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x39, 0x3A, 0x3B, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive08_0082(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1C, 10, -1},
        0
    );
}

void Passive08_0083(PLW* wk) {
    passive08_pattern_lever_attack(wk, 0x40);
}

void Passive08_0084(PLW* wk) {
    passive08_pattern_lever_attack(wk, 0x200);
}

void Passive08_0085(PLW* wk) {
    passive08_pattern_lever_attack(wk, 0x400);
}

void Passive08_0086(PLW* wk) {
    active_pattern_approach_walk(wk, 0xBF);
}

void Passive08_0087(PLW* wk) {
    pattern_command_attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
}

void Passive08_0088(PLW* wk) {
    pattern_jump(wk, 0);
}

void Passive08_0089(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, -1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive08_0090(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term(
        wk,
        0x3F,
        &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive08_0091(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Lever_Attack(wk, 0xC, 0, 0x20);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0092(PLW* wk) {
    pattern_approach_walk_lever_attack_sa_term_2(wk, 0x3F, 0x20, &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0});
}

void Passive08_0093(PLW* wk) {
    pattern_em_term_lever_attack_normal_attack(wk, &(EM_Term_Params){-0x7F80, 0x28, 7, 1, -1}, 9, 0x402);
}

void Passive08_0094(PLW* wk) {
    pattern_approach_walk_normal_attack_j_command_attack_2(wk, 0x3F, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive08_0095(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive08_0096(PLW* wk) {
    pattern_sa_term_normal_attack(wk, &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0}, 8, 0x400);
}

void Passive08_0097(PLW* wk) {
    pattern_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive08_0098(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Passive08_0099(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Passive08_0100(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Passive08_0101(PLW* wk) {
    pattern_normal_attack_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive08_0102(PLW* wk) {
    pattern_jump_attack_term_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive08_0103(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xB, 0x202, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive08_0104(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 9, 0x402, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive08_0105(PLW* wk) {
    pattern_approach_walk_em_term_sa_term(
        wk,
        0x3F,
        &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive08_0106(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20});
}

void Passive08_0107(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive08_0108(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1D, 9, -1}
    );
}

void Passive08_0109(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC8, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive08_0110(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term_3(
        wk,
        0x3F,
        &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive08_0111(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term_2(
        wk,
        0x3F,
        &(SA_Term_Args){0x39, 0x3A, 0x3B, 0},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive08_0112(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7B);
}

void Passive08_0113(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Passive08_0114(PLW* wk) {
    pattern_jump_attack_term_normal_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x12,
        0x402
    );
}

void Passive08_0115(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 10, -1});
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0116(PLW* wk) {
    pattern_forced_guard(wk, 2);
}

void Passive08_0117(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive08_0118(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive08_0119(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1D, 8, -1});
}
