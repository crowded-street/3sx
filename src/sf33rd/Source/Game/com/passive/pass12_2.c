/**
 * @file pass12_2.c
 * COM Passive: Sean
 * Pattern scripts from Passive12_0063 on.
 */

#include "sf33rd/Source/Game/com/passive/pass12.h"
#include "sf33rd/Source/Game/com/passive/pass12_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

static void passive12_0063_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0063(PLW* wk) {
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
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        passive12_0063_from_step_6(wk);
        break;
    }
}

void Passive12_0064(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 0, 0x400);
}

void Passive12_0065(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x400},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0066(PLW* wk) {
    pattern_wait_get_up_command_attack(wk, 0, 0, &(Command_Attack_Args){8, 0x1C, 0x4009, -1});
}

void Passive12_0067(PLW* wk) {
    pattern_wait_get_up_command_attack(wk, 0, 0, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
}

void Passive12_0068(PLW* wk) {
    passive12_pattern_wait_lie_approach_walk(wk, 0x7F);
}

void Passive12_0069(PLW* wk) {
    pattern_wait_get_up(wk, 0, -1);
}

void Passive12_0070(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 0xB, 0x400, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x20, 9, -1}
    );
}

void Passive12_0071(PLW* wk) {
    pattern_wait_lie_jump(wk);
}

void Passive12_0072(PLW* wk) {
    passive12_pattern_wait_lie_approach_walk(wk, 0xBF);
}

void Passive12_0073(PLW* wk) {
    pattern_wait_get_up(wk, 3, -1);
}

void Passive12_0074(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FC0, -1, 6, 1, -1}, 0x100);
}

void Passive12_0075(PLW* wk) {
    pattern_em_term_normal_attack_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, -1, 0, 1, -1},
        0x102,
        &(Command_Attack_Args){8, 0x1D, 8, -1}
    );
}

void Passive12_0076(PLW* wk) {
    passive12_pattern_approach_walk_normal_attack(wk, 0xB, 0x402);
}

void Passive12_0077(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x400);
}

void Passive12_0078(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x220);
}

void Passive12_0079(PLW* wk) {
    pattern_pierce_on_command_attack_branch_unit_area(
        wk,
        &(Command_Attack_Args){8, 0, -1, -1},
        &(Branch_Menu_Args){6, 0x53, 0x2F, 0x24, 0x25}
    );
}

void Passive12_0080(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202,
        &(Command_Attack_Args){8, 0x1C, 0x4009, -1}
    );
}

void Passive12_0081(PLW* wk) {
    pattern_jump_attack_term_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0082(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1C, 0x4008, -1},
        0
    );
}

void Passive12_0083(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x110);
}

void Passive12_0084(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x110);
}

void Passive12_0085(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x110);
}

void Passive12_0086(PLW* wk) {
    pattern_approach_walk(wk, 0xBF);
}

void Passive12_0087(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0, -1, -1});
}

void Passive12_0088(PLW* wk) {
    pattern_jump(wk, 0);
}

void Passive12_0089(PLW* wk) {
    pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, -1, -1},
        &(Command_Attack_Args){8, 0x1C, 0x4009, -1}
    );
}

void Passive12_0090(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term(
        wk,
        0x37,
        &(SA_Term_Args){0xFFFF, 0xFFFF, 0x32, 0},
        &(Command_Attack_Args){8, 0x1C, 0x4008, -1}
    );
}

void Passive12_0091(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Lever_Attack(wk, 0xC, 0, 0x200);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x32, 0});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0092(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Lever_Attack(wk, 0xC, 0, 0x200);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x30, 0xFFFF, 0x32, 0});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0093(PLW* wk) {
    pattern_em_term_lever_attack_normal_attack(wk, &(EM_Term_Params){-0x7F80, 0x28, 7, 1, -1}, 9, 0x402);
}

void Passive12_0094(PLW* wk) {
    pattern_approach_walk_normal_attack_j_command_attack_2(wk, 0x37, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive12_0095(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1C, 0x4009, -1});
}

void Passive12_0096(PLW* wk) {
    pattern_sa_term_normal_attack(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0}, 8, 0x100);
}

void Passive12_0097(PLW* wk) {
    pattern_sa_term_j_command_attack(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0}, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive12_0098(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x40);
}

void Passive12_0099(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x202);
}

void Passive12_0100(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x402);
}

void Passive12_0101(PLW* wk) {
    pattern_normal_attack_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x30, 0xFFFF, 0x32, 0},
        &(Command_Attack_Args){8, 0x1C, 0x4009, -1}
    );
}

void Passive12_0102(PLW* wk) {
    pattern_jump_attack_term_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1C, 0x4009, -1}
    );
}

void Passive12_0103(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xB, 0x202, &(Command_Attack_Args){8, 0x1C, 0x4009, -1});
}

void Passive12_0104(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 9, 0x402, &(Command_Attack_Args){8, 0x1C, 0x4009, -1});
}

void Passive12_0105(PLW* wk) {
    pattern_approach_walk_em_term_sa_term(
        wk,
        0x37,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0106(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -1, -1, -1});
}

void Passive12_0107(PLW* wk) {
    pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 0x4009, -1}
    );
}

void Passive12_0108(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x20, 9, -1}
    );
}

void Passive12_0109(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC8, 0, 1, -1},
        &(Command_Attack_Args){8, 0x20, 10, -1}
    );
}

void Passive12_0110(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term_3(
        wk,
        0x37,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0111(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term_2(
        wk,
        0x37,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1C, 0x400A, -1}
    );
}

void Passive12_0112(PLW* wk) {
    pattern_approach_walk(wk, 0x7F);
}

void Passive12_0113(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x200);
}

void Passive12_0114(PLW* wk) {
    pattern_jump_attack_term_normal_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x12,
        0x402
    );
}

void Passive12_0115(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1C, 0x400A, -1});
        break;

    case 3:
        Wait(wk, 5);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0116(PLW* wk) {
    pattern_forced_guard(wk, 2);
}

void Passive12_0117(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1C, 0x4008, -1});
}

void Passive12_0118(PLW* wk) {
    pattern_normal_attack_j_command_attack_2(wk, 9, 0x100, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Passive12_0119(PLW* wk) {
    pattern_normal_attack_sa_term_command_attack_2(
        wk,
        9,
        &(SA_Term_Args){0x30, 0xFFFF, 0x32, 0},
        &(Command_Attack_Args){8, 0x1C, 0x4008, -1}
    );
}

void Passive12_0120(PLW* wk) {
    pattern_normal_attack_command_attack_6(wk, 9, 0x102, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
}

void Passive12_0121(PLW* wk) {
    pattern_normal_attack(wk, 9, 0x102);
}

void Passive12_0122(PLW* wk) {
    pattern_normal_attack_2(wk, 9, 0x102, 0x202);
}

void Passive12_0123(PLW* wk) {
    pattern_normal_attack_4(wk, 9, 9, 0x202);
}

void Passive12_0124(PLW* wk) {
    pattern_normal_attack_command_attack_3(wk, 9, 0xB, &(Command_Attack_Args){8, 0x1C, 0x4008, -1});
}

void Passive12_0125(PLW* wk) {
    pattern_normal_attack_sa_term_normal_attack(wk, &(SA_Term_Args){0x30, 0xFFFF, 0x32, 0});
}

void Passive12_0126(PLW* wk) {
    pattern_normal_attack_sa_term_normal_attack_2(wk, &(SA_Term_Args){0x30, 0xFFFF, 0x32, 0}, 0x202);
}
