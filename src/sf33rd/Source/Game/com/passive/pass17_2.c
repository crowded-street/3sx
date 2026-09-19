/**
 * @file pass17_2.c
 * COM Passive: Q
 * Pattern scripts from Passive17_0049 on.
 */

#include "sf33rd/Source/Game/com/passive/pass17.h"
#include "sf33rd/Source/Game/com/passive/pass17_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive17_0049(PLW* wk) {
    pattern_em_term_adjust_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1});
}

void Passive17_0050(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 8, 0x200);
}

void Passive17_0051(PLW* wk) {
    pattern_normal_attack_3(wk, 0x220, 0xB, 0x102);
}

void Passive17_0052(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Passive17_0053(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive17_0054(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5A, 0x5B});
}

void Passive17_0055(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive17_0056(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7F90, -1, 0x20});
}

void Passive17_0057(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7F90, -1, 0x20});
}

void Passive17_0058(PLW* wk) {
    pattern_keep_status(wk, 1);
}

void Passive17_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive17_0060(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive17_0061(PLW* wk) {
    pattern_sa_term_approach_walk_sa_term(
        wk,
        &(SA_Term_Args){0x2E, 0xFFFF, 0x30, 0},
        &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive17_0062(PLW* wk) {
    pattern_approach_walk_etc_term_com_random_select(wk);
}

void Passive17_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 0, 6, 0x21);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0064(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Passive17_0065(PLW* wk) {
    pattern_approach_walk_command_attack_com_random_select_2(
        wk,
        &(Command_Attack_Args){8, 0x1E, 8, -1},
        &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}
    );
}

void Passive17_0066(PLW* wk) {
    active_pattern_approach_walk_command_attack(wk, 0x4B, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive17_0067(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x7F, 0x402);
}

void Passive17_0068(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7F90, -1, 0x200});
}

void Passive17_0069(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x40, 0});
}

void Passive17_0070(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x7F, 0x402);
}

void Passive17_0071(PLW* wk) {
    pattern_approach_walk_com_random_select(wk, 0x37, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}, 1);
}

void Passive17_0072(PLW* wk) {
    pattern_pierce_on_command_attack_approach_walk(wk, 0x4B, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive17_0073(PLW* wk) {
    pattern_approach_walk_normal_attack_branch_unit_area(wk, 0xBF, &(Branch_Menu_Args){6, 0x68, 0x69, 0x69, 0x6A});
}

void Passive17_0074(PLW* wk) {
    active_pattern_sa_term_command_attack(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0}, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive17_0075(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        Approach_Walk(wk, 0x41, 2);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0076(PLW* wk) {
    pattern_sa_term_approach_walk_sa_term(
        wk,
        &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0},
        &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive17_0077(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x40, 0, -0x7FA0, -1, 0x200}
    );
}

void Passive17_0078(PLW* wk) {
    pattern_jump_attack_term_normal_attack_3(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x200, 0, -0x7F90, -1, 0x40},
        0x10,
        0x200
    );
}

void Passive17_0079(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x40, 0, -0x7F90, -1, 0x40}
    );
}

void Passive17_0080(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x200, 0, -0x7F90, -1, 0x200},
        0x200
    );
}

void Passive17_0081(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_2(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x40, 0, -0x7F90, -1, 0x40},
        0x10,
        0x200
    );
}

void Passive17_0082(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x400, 0, -0x7F90, -1, 0x40},
        0x200
    );
}

void Passive17_0083(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_3(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7F90, -1, 0x40},
        0x200
    );
}

void Passive17_0084(PLW* wk) {
    pattern_hi_jump_attack_normal_attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x40, 0}, 0x200);
}

void Passive17_0085(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 10);
        break;

    case 1:
        Hi_Jump_Attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x400, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0086(PLW* wk) {
    pattern_sa_term_em_term_sa_term(wk);
}

void Passive17_0087(PLW* wk) {
    pattern_sa_term_em_term_sa_term(wk);
}

void Passive17_0088(PLW* wk) {
    pattern_sa_term_em_term_sa_term(wk);
}

void Passive17_0089(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive17_0090(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive17_0091(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive17_0092(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5A, 0x5B}
    );
}

void Passive17_0093(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x12);
}

void Passive17_0094(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x20);
}

void Passive17_0095(PLW* wk) {
    pattern_normal_attack_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x82, 0x83});
}

void Passive17_0096(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FC8, 6, 1, -1}, 8, 0x402);
}

void Passive17_0097(PLW* wk) {
    pattern_approach_walk_em_term_com_random_select(wk, 0x37, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F});
}

void Passive17_0098(PLW* wk) {
    pattern_approach_walk_em_term_command_attack_2(
        wk,
        0x37,
        &(Command_Attack_Args){8, 0x1E, 9, -1},
        &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}
    );
}

void Passive17_0099(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_lever_attack(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7F90, -1, 0x200},
        0x40
    );
}

void Passive17_0100(PLW* wk) {
    pattern_approach_walk_wait_get_up_sa_term(
        wk,
        0x4B,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive17_0101(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA(wk, 6, 0x2F);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0});
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x82, 0x83});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0102(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0, 0x20);
}

void Passive17_0103(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x22);
}

void Passive17_0104(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0x4008, -1});
}

void Passive17_0105(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0x4009, -1});
}

void Passive17_0106(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
}

void Passive17_0107(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up_2(wk, 0x60, 0x6C, 0x20);
}

void Passive17_0108(PLW* wk) {
    pattern_hi_jump(wk, 1);
}

void Passive17_0109(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack(wk, 0x60, 0x6C, &(Command_Attack_Args){8, 1, 10, -1});
}

void Passive17_0110(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x60, 0x6C, 0x38);
}

void Passive17_0111(PLW* wk) {
    pattern_em_term_normal_attack_com_random_select(wk, 0x42);
}

void Passive17_0112(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Passive17_0113(PLW* wk) {
    pattern_normal_attack_com_random_select_2(wk, 9, 0x42);
}

void Passive17_0114(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive17_0115(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20});
}
