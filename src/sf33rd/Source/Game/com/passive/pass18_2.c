/**
 * @file pass18_2.c
 * COM Passive: Twelve
 * Pattern scripts from Passive18_0040 on.
 */

#include "sf33rd/Source/Game/com/passive/pass18.h"
#include "sf33rd/Source/Game/com/passive/pass18_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive18_0040(PLW* wk) {
    pattern_em_term_normal_attack_7(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 8, 0x400);
}

void Passive18_0041(PLW* wk) {
    passive18_pattern_approach_walk_em_term_normal_attack(
        wk,
        0x6F,
        &(EM_Term_Params){-0x7F80, -0x7FC8, 6, 1, -1},
        0x40
    );
}

void Passive18_0042(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x42, 2});
}

void Passive18_0043(PLW* wk) {
    pattern_em_term_jump_command_attack_term(wk, &(EM_Term_Params){-0x7F80, -0x7FB8, 6, 1, -1});
}

void Passive18_0044(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x4B,
        &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive18_0045(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FC0, 6, 1, -1}, 1, 0x40);
}

void Passive18_0046(PLW* wk) {
    pattern_keep_status(wk, -1);
}

void Passive18_0047(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xB, 0x102, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive18_0048(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x402);
}

void Passive18_0049(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x6F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -0x7FC0, 6, 1, -1});
        break;

    case 2:
        Adjust_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0050(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0x400);
}

void Passive18_0051(PLW* wk) {
    pattern_normal_attack_3(wk, 0x420, 0xB, 0x102);
}

void Passive18_0052(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Passive18_0053(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive18_0054(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x68, 0x68, 0x69, 0x6A});
}

void Passive18_0055(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive18_0056(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7FA0, -1, 0x20});
}

void Passive18_0057(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x20, 0, -0x7FA0, -1, 0x20});
}

void Passive18_0058(PLW* wk) {
    pattern_keep_status(wk, 1);
}

void Passive18_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive18_0060(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive18_0061(PLW* wk) {
    pattern_sa_term_branch_unit_area(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0},
        &(Branch_Menu_Args){6, 0x68, 0x68, 0x69, 0x6A}
    );
}

void Passive18_0062(PLW* wk) {
    pattern_approach_walk_etc_term_lever_attack(wk, 0x37, 2, 0xD);
}

void Passive18_0063(PLW* wk) {
    pattern_etc_term_normal_attack_lever_attack(wk, 0x21, 1, 0x40);
}

void Passive18_0064(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Passive18_0065(PLW* wk) {
    pattern_approach_walk_command_attack_com_random_select(wk, 0x37, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive18_0066(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FB8, 0, -1, 0x30, 0x400});
}

void Passive18_0067(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x7F, 0x402);
}

void Passive18_0068(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7FA0, -1, 0x400});
}

void Passive18_0069(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x42, 0});
}

void Passive18_0070(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0xBF, 1, 0x402);
}

void Passive18_0071(PLW* wk) {
    pattern_approach_walk_com_random_select(wk, 0x37, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}, 1);
}

void Passive18_0072(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
        break;

    case 2:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FC0, 2, -1, 0x30, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0073(PLW* wk) {
    pattern_approach_walk_normal_attack_branch_unit_area(wk, 0xBF, &(Branch_Menu_Args){6, 0x68, 0x68, 0x69, 0x6A});
}

void Passive18_0074(PLW* wk) {
    active_pattern_sa_term_command_attack(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0}, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive18_0075(PLW* wk) {
    pattern_sa_term_branch_unit_area(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0},
        &(Branch_Menu_Args){6, 0x68, 0x68, 0x69, 0x6A}
    );
}

void Passive18_0076(PLW* wk) {
    pattern_sa_term_branch_unit_area(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0},
        &(Branch_Menu_Args){6, 0x68, 0x68, 0x69, 0x6A}
    );
}

void Passive18_0077(PLW* wk) {
    passive18_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x40, 0, -0x7FA0, -1, 0x200}
    );
}

void Passive18_0078(PLW* wk) {
    pattern_jump_attack_term_normal_attack_3(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x100, 0, -0x7FA0, -1, 0x20},
        0x10,
        0x400
    );
}

void Passive18_0079(PLW* wk) {
    passive18_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20}
    );
}

void Passive18_0080(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x400},
        0x400
    );
}

void Passive18_0081(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_2(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x100, 0, -0x7FA0, -1, 0x400},
        0x10,
        0x400
    );
}

void Passive18_0082(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20},
        0x400
    );
}

void Passive18_0083(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_3(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20},
        0x400
    );
}

void Passive18_0084(PLW* wk) {
    pattern_hi_jump_attack_normal_attack(wk, &(Jump_Attack_Args){0xC, 0x12, 0x400, 0}, 0x400);
}

void Passive18_0085(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 6);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FB8, 2, -1, 0x30, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0086(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x6F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -1, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1});
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0087(PLW* wk) {
    pattern_em_term_sa_term_em_term(
        wk,
        &(EM_Term_Params){-1, -0x7FC8, 6, 1, -1},
        &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive18_0088(PLW* wk) {
    pattern_em_term_sa_term_em_term(
        wk,
        &(EM_Term_Params){-1, -0x7FC8, 6, 1, -1},
        &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive18_0089(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x30, 8, -1, -1, -0x7FD0, 0, -1, 0x30, 0x400});
}

void Passive18_0090(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x30, 9, -1, -1, -0x7FD0, 0, -1, 0x30, 0x400});
}

void Passive18_0091(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x30, 0xA, -1, -1, -0x7FD0, 0, -1, 0x30, 0x400});
}

void Passive18_0092(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FA0, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x5A, 0x5B, 0x5B, 1}
    );
}

void Passive18_0093(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x12);
}

void Passive18_0094(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x20);
}

void Passive18_0095(PLW* wk) {
    pattern_normal_attack_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x68, 0x69, 0x6A, 0x6A});
}

void Passive18_0096(PLW* wk) {
    passive18_pattern_approach_walk_em_term_normal_attack(
        wk,
        0x7F,
        &(EM_Term_Params){-0x7F80, -0x7FD8, 6, 1, -1},
        0x402
    );
}

void Passive18_0097(PLW* wk) {
    pattern_approach_walk_em_term_com_random_select(wk, 0x37, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F});
}

void Passive18_0098(PLW* wk) {
    pattern_approach_walk_em_term_command_attack_2(
        wk,
        0x7F,
        &(Command_Attack_Args){8, 0x1C, 10, -1},
        &(Branch_Menu_Args){6, 0x42, 0x20, 0xBF, 0x8E}
    );
}

void Passive18_0099(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_lever_attack(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x200},
        0x20
    );
}

void Passive18_0100(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0});
        break;

    case 2:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x68, 0x69, 0x6A, 0x6A});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0101(PLW* wk) {
    pattern_normal_attack_sa_term_branch_unit_area(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0},
        &(Branch_Menu_Args){6, 0x68, 0x69, 0x6A, 0x6A}
    );
}

void Passive18_0102(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0, 0x20);
}

void Passive18_0103(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Passive18_0104(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive18_0105(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive18_0106(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive18_0107(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x60, 0x6C, 0x20);
}

void Passive18_0108(PLW* wk) {
    pattern_hi_jump(wk, 1);
}

void Passive18_0109(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack(wk, 0x60, 0x6C, &(Command_Attack_Args){8, 1, 10, -1});
}

void Passive18_0110(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x60, 0x6C, 0x38);
}
