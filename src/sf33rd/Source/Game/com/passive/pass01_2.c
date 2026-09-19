/**
 * @file pass01_2.c
 * COM Passive: Alex
 * Pattern scripts from Passive01_0048 on.
 */

#include "sf33rd/Source/Game/com/passive/pass01.h"
#include "sf33rd/Source/Game/com/passive/pass01_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive01_0048(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x102, 0x402);
}

void Passive01_0049(PLW* wk) {
    pattern_em_term_adjust_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1});
}

void Passive01_0050(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0x200);
}

void Passive01_0051(PLW* wk) {
    pattern_normal_attack_6(wk, 0xB, 0x220, 0xB);
}

void Passive01_0052(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Passive01_0053(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive01_0054(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x68, 0x6A, 1, 1});
}

void Passive01_0055(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive01_0056(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7FA0, -1, 0x20});
}

void Passive01_0057(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x20, 0, -0x7FA0, -1, 0x20});
}

void Passive01_0058(PLW* wk) {
    pattern_keep_status(wk, 1);
}

void Passive01_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive01_0060(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive01_0061(PLW* wk) {
    pattern_sa_term_approach_walk_sa_term_3(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
}

void Passive01_0062(PLW* wk) {
    pattern_approach_walk_etc_term_lever_attack(wk, 0x3B, 2, 0xD);
}

void Passive01_0063(PLW* wk) {
    pattern_etc_term_normal_attack_lever_attack(wk, 0x21, 1, 0x40);
}

void Passive01_0064(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Passive01_0065(PLW* wk) {
    pattern_approach_walk_command_attack_com_random_select(wk, 0x3B, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Passive01_0066(PLW* wk) {
    active_pattern_approach_walk_command_attack(wk, 0x4F, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive01_0067(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x83, 0x402);
}

void Passive01_0068(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7FA0, -1, 0x400});
}

void Passive01_0069(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x42, 0});
}

void Passive01_0070(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x83, 1, 0x40);
}

void Passive01_0071(PLW* wk) {
    pattern_approach_walk_com_random_select(wk, 0x3B, &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E}, 1);
}

void Passive01_0072(PLW* wk) {
    pattern_pierce_on_command_attack_approach_walk(wk, 0x4F, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive01_0073(PLW* wk) {
    pattern_approach_walk_normal_attack_branch_unit_area(wk, 0xC3, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
}

void Passive01_0074(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0x59});
        break;

    case 1:
        Approach_Walk(wk, 0x4F, 2);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0075(PLW* wk) {
    pattern_sa_term_approach_walk_sa_term_3(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0});
}

void Passive01_0076(PLW* wk) {
    pattern_sa_term_approach_walk_jump_attack(wk);
}

void Passive01_0077(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x40, 0, -0x7FA0, -1, 0x200}
    );
}

void Passive01_0078(PLW* wk) {
    pattern_jump_attack_term_normal_attack_3(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x100, 0, -0x7FA0, -1, 0x20},
        0x10,
        0x200
    );
}

void Passive01_0079(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20}
    );
}

void Passive01_0080(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x400},
        0x200
    );
}

void Passive01_0081(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_2(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x100, 0, -0x7FA0, -1, 0x400},
        0x10,
        0x200
    );
}

void Passive01_0082(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20},
        0x200
    );
}

void Passive01_0083(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_3(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20},
        0x200
    );
}

void Passive01_0084(PLW* wk) {
    pattern_hi_jump_attack_normal_attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x42, 0}, 0x200);
}

void Passive01_0085(PLW* wk) {
    pattern_wait_com_random_select(wk, 0xA, &(Branch_Menu_Args){6, 5, 9, 0x35, 1});
}

void Passive01_0086(PLW* wk) {
    pattern_approach_walk_em_term_sa_term_3(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0x59});
}

void Passive01_0087(PLW* wk) {
    pattern_approach_walk_em_term_sa_term_3(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0x59});
}

void Passive01_0088(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FA0, 6, 1, -1});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0089(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive01_0090(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive01_0091(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive01_0092(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x83, 3);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FA0, 6, 1, -1});
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive01_0093(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x102, 0x12);
}

void Passive01_0094(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x20);
}

void Passive01_0095(PLW* wk) {
    pattern_normal_attack_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x68, 0x69, 0x6A, 1});
}

void Passive01_0096(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_7(wk, &(EM_Term_Params){-0x7F80, -0x7FD8, 6, 1, -1}, 8, 0x402);
}

void Passive01_0097(PLW* wk) {
    pattern_approach_walk_em_term_com_random_select(wk, 0x3B, &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E});
}

void Passive01_0098(PLW* wk) {
    pattern_approach_walk_em_term_command_attack_2(
        wk,
        0x3B,
        &(Command_Attack_Args){8, 0x1E, 0xA, -1},
        &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}
    );
}

void Passive01_0099(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_lever_attack(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x200},
        0x20
    );
}

void Passive01_0100(PLW* wk) {
    pattern_approach_walk_wait_get_up_sa_term(
        wk,
        0x4F,
        &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Passive01_0101(PLW* wk) {
    pattern_normal_attack_sa_term_branch_unit_area(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0x59},
        &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1}
    );
}

void Passive01_0102(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0, 0x20);
}

void Passive01_0103(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Passive01_0104(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive01_0105(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive01_0106(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Passive01_0107(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x60, 0x6C, 0x20);
}

void Passive01_0108(PLW* wk) {
    pattern_hi_jump(wk, 1);
}

void Passive01_0109(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack(wk, 0x60, 0x6C, &(Command_Attack_Args){8, 1, 0xA, -1});
}

void Passive01_0110(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x60, 0x6C, 0x38);
}
