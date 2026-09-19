/**
 * @file pass15_2.c
 * COM Passive: Chun-Li
 * Pattern scripts from Passive15_0060 on.
 */

#include "sf33rd/Source/Game/com/passive/pass15.h"
#include "sf33rd/Source/Game/com/passive/pass15_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive15_0060(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive15_0061(PLW* wk) {
    active_pattern_sa_term_command_attack(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0}, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive15_0062(PLW* wk) {
    pattern_approach_walk_etc_term_lever_attack(wk, 0x37, 6, 0x36);
}

void Passive15_0063(PLW* wk) {
    pattern_etc_term_normal_attack_lever_attack(wk, 0x36, 0, 0x402);
}

void Passive15_0064(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Passive15_0065(PLW* wk) {
    pattern_approach_walk_command_attack_com_random_select(wk, 0x37, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive15_0066(PLW* wk) {
    pattern_sa_term_approach_walk_sa_term(
        wk,
        &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0},
        &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive15_0067(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x7F, 0x402);
}

void Passive15_0068(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7FA0, -1, 0x400});
}

void Passive15_0069(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x42, 0});
}

void Passive15_0070(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x7F, 1, 0x40);
}

void Passive15_0071(PLW* wk) {
    pattern_approach_walk_com_random_select(wk, 0x37, &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E}, 1);
}

void Passive15_0072(PLW* wk) {
    pattern_pierce_on_command_attack_approach_walk(wk, 0x4B, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive15_0073(PLW* wk) {
    pattern_approach_walk_normal_attack_branch_unit_area(wk, 0xBF, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
}

void Passive15_0074(PLW* wk) {
    pattern_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0},
        &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E},
        1
    );
}

void Passive15_0075(PLW* wk) {
    pattern_sa_term_approach_walk_sa_term(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0},
        &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive15_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 1:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0});
        break;

    case 3:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive15_0077(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x40, 0, -0x7FA0, -1, 0x200}
    );
}

void Passive15_0078(PLW* wk) {
    pattern_jump_attack_term_normal_attack_3(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x100, 0, -0x7FA0, -1, 0x20},
        0x10,
        0x200
    );
}

void Passive15_0079(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20}
    );
}

void Passive15_0080(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x400},
        0x200
    );
}

void Passive15_0081(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_2(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x100, 0, -0x7FA0, -1, 0x400},
        0x10,
        0x200
    );
}

void Passive15_0082(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20},
        0x200
    );
}

void Passive15_0083(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_3(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20},
        0x200
    );
}

void Passive15_0084(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x202, 0});
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

void Passive15_0085(PLW* wk) {
    pattern_wait_command_attack(wk, 3, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive15_0086(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0xFFFF});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive15_0087(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0xFFFF});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive15_0088(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0xFFFF});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive15_0089(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive15_0090(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive15_0091(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive15_0092(PLW* wk) {
    pattern_em_term_com_random_select(
        wk,
        &(EM_Term_Params){-1, -0x7FA0, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1}
    );
}

void Passive15_0093(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x12);
}

void Passive15_0094(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x20);
}

void Passive15_0095(PLW* wk) {
    pattern_normal_attack_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x68, 0x69, 0x6A, 1});
}

void Passive15_0096(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FD8, 6, 1, -1}, 8, 0x402);
}

void Passive15_0097(PLW* wk) {
    pattern_approach_walk_em_term_com_random_select(wk, 0x37, &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E});
}

void Passive15_0098(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive15_0099(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_lever_attack(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x200},
        0x20
    );
}

void Passive15_0100(PLW* wk) {
    pattern_approach_walk_wait_get_up_sa_term_com_random_select(
        wk,
        0x37,
        &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0},
        &(Com_Random_Select_Step){ &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E}, 1 }
    );
}

void Passive15_0101(PLW* wk) {
    pattern_normal_attack_sa_term_command_attack_2(
        wk,
        8,
        &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive15_0102(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0, 0x20);
}

void Passive15_0103(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Passive15_0104(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive15_0105(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive15_0106(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive15_0107(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x60, 0x6C, 0x20);
}

void Passive15_0108(PLW* wk) {
    pattern_hi_jump(wk, 1);
}

void Passive15_0109(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack(wk, 0x60, 0x6C, &(Command_Attack_Args){8, 1, 10, -1});
}

void Passive15_0110(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x60, 0x6C, 0x38);
}

void Passive15_0111(PLW* wk) {
    pattern_em_term_normal_attack_com_random_select(wk, 0x20);
}

void Passive15_0112(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x402);
}

void Passive15_0113(PLW* wk) {
    pattern_normal_attack_com_random_select_2(wk, 9, 0x200);
}

void Passive15_0114(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive15_0115(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20});
}

void Passive15_0116(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x20});
}

void Passive15_0117(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x100, 0});
}

void Passive15_0118(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}
