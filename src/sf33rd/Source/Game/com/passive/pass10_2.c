/**
 * @file pass10_2.c
 * COM Passive: Yang
 * Pattern scripts from Passive10_0050 on.
 */

#include "sf33rd/Source/Game/com/passive/pass10.h"
#include "sf33rd/Source/Game/com/passive/pass10_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive10_0050(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x12, 0x22);
}

void Passive10_0051(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x220);
}

void Passive10_0052(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Passive10_0053(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 2, -0x7FA0, -1, -0x7C00});
}

void Passive10_0054(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 2, -0x7FA0, -1, 0x8400});
}

void Passive10_0055(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1e, 8, -1});
}

void Passive10_0056(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -0x7FA0, -1, 0x40});
}

void Passive10_0057(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x20, 0, -0x7FA0, -1, 0x40});
}

void Passive10_0058(PLW* wk) {
    pattern_keep_status(wk, 1);
}

void Passive10_0059(PLW* wk) {
    pattern_normal_attack_2(wk, 0xb, 0x20, 0x40);
}

void Passive10_0060(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive10_0061(PLW* wk) {
    pattern_sa_term_normal_attack(wk, &(SA_Term_Args){0x2e, 0x2f, 0x30, 0}, 8, 0x400);
}

void Passive10_0062(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x37, 0x110);
}

void Passive10_0063(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xc, 0x20, &(Command_Attack_Args){8, 0x1e, 9, -1});
}

void Passive10_0064(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x200);
}

void Passive10_0065(PLW* wk) {
    pattern_approach_walk_normal_attack_command_attack(wk, 0xc, 0x100, &(Command_Attack_Args){8, 0x1e, 8, -1});
}

void Passive10_0066(PLW* wk) {
    pattern_etc_term_branch_unit_area(wk, 6, 0x40, &(Branch_Menu_Args){6, 0x59, 0x5a, 0x5a, 0x5b});
}

void Passive10_0067(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1e, 10, -1});
}

void Passive10_0068(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -0x7FA0, -1, 0x40});
}

void Passive10_0069(PLW* wk) {
    pattern_command_attack_branch_unit_area(
        wk,
        &(Command_Attack_Args){0xc, 0, 0xb, -1},
        &(Branch_Menu_Args){6, 0x59, 0x5a, 0x5b, 0x5b}
    );
}

void Passive10_0070(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 0xb, -1},
        &(Command_Attack_Args){8, 0x1e, 8, -1}
    );
}

void Passive10_0071(PLW* wk) {
    pattern_approach_walk_command_attack(
        wk,
        0x37,
        &(Command_Attack_Args){8, 0x1c, 10, -1},
        &(Command_Attack_Args){8, 0x1e, 10, -1}
    );
}

void Passive10_0072(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xc, 0, 0xb, -1},
        &(Command_Attack_Args){8, 0x1e, 8, -1}
    );
}

void Passive10_0073(PLW* wk) {
    pattern_approach_walk_hi_jump_attack_term_command_attack(
        wk,
        0xbf,
        &(Hi_Jump_Term_Args){-0x7FA0, -1, 0xb, 0x8400, 0, -0x7FA0, -1, 0x40},
        &(Command_Attack_Args){8, 0x1e, 8, -1}
    );
}

void Passive10_0074(PLW* wk) {
    pattern_sa_term_normal_attack_com_random_select(wk, &(SA_Term_Args){0x2e, 0x2f, 0x30, 0}, 0xc);
}

void Passive10_0075(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2e, 0xffff, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xffff, 0x2f, 0xffff, 0});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 5:
        Normal_Attack(wk, 0xc, 0x40);
        break;

    default:
        pattern_com_random_select_from_step_6(wk);
        break;
    }
}

void Passive10_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xffff, 0x2f, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0xbf, 2);
        break;

    case 2:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 0, -0x7FA0, -1, 0x40});
        break;

    case 3:
        Normal_Attack(wk, 0xb, 0x10);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 5:
        Normal_Attack(wk, 0xc, 0x40);
        break;

    default:
        pattern_com_random_select_from_step_6(wk);
        break;
    }
}

void Passive10_0077(PLW* wk) {
    pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FD0, 0xb, 0x20, 0, -0x7FA0, -1, 0x40},
        8,
        0x400
    );
}

void Passive10_0078(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_3(wk, 0xb, 0xc, &(Command_Attack_Args){8, 0x1e, 9, -1});
}

void Passive10_0079(PLW* wk) {
    pattern_jump_attack_term_normal_attack(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 0xb, 0x20, 0, -1, -1, -1}, 8, 0x40);
}

void Passive10_0080(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7FA0, -0x7FD0, 0xb, 0x40, 0, -0x7FA0, -1, 0x40},
        0x400
    );
}

void Passive10_0081(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_lever_attack_2(
        wk,
        &(Hi_Jump_Term_Args){-0x7FA0, -1, 0xb, 0x8200, 0, -0x7FA0, -1, 0x40},
        0xb
    );
}

void Passive10_0082(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7FA0, -0x7FC0, 0xb, 0x40, 0, -1, -1, 0xFFFF},
        0x400
    );
}

void Passive10_0083(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FC0, 0xb, 0x40, 0, -0x7FA0, -1, 0x40});
        break;

    case 1:
        Normal_Attack(wk, 0xb, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 3:
        Normal_Attack(wk, 0xc, 0x40);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1e, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0084(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_com_random_select(wk, 0xb, 0xc);
}

void Passive10_0085(PLW* wk) {
    pattern_wait_command_attack(wk, 0xa, &(Command_Attack_Args){8, 0x1e, 10, -1});
}

void Passive10_0086(PLW* wk) {
    pattern_approach_walk_em_term_sa_term_4(wk, 0x37, &(SA_Term_Args){0x2e, 0x2f, 0xffff, 0}, 0xb);
}

void Passive10_0087(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xffff, 0x2f, 0xffff, 0});
        break;

    case 3:
        Normal_Attack(wk, 0xb, 0x10);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 5:
        Normal_Attack(wk, 0xc, 0x40);
        break;

    default:
        pattern_com_random_select_from_step_6(wk);
        break;
    }
}

void Passive10_0088(PLW* wk) {
    pattern_em_term_sa_term_normal_attack(wk, &(SA_Term_Args){0xffff, 0xffff, 0x30, 0}, 0xb, 0xc);
}

void Passive10_0089(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1f, 8, -1});
}

void Passive10_0090(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1f, 9, -1});
}

void Passive10_0091(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1f, 10, -1});
}

void Passive10_0092(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FE0, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x59, 0x5a, 0x5b, 0x5b}
    );
}

void Passive10_0093(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x102, 0x102);
}

void Passive10_0094(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x59, 0x5a, 0x5b, 1});
}

void Passive10_0095(PLW* wk) {
    pattern_walk_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x59, 0x5a, 0x5b, 0x5b});
}

void Passive10_0096(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1}, 0x402);
}

void Passive10_0097(PLW* wk) {
    pattern_approach_walk_em_term_lever_attack(wk, 0x37, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 0);
}

void Passive10_0098(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xb, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 4:
        Normal_Attack(wk, 0xc, 0x40);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1e, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0099(PLW* wk) {
    pattern_normal_attack_com_random_select_4(wk, 0xb, 0xc);
}

void Passive10_0100(PLW* wk) {
    pattern_walk_wait_get_up_j_command_attack(wk, &(Command_Attack_Args){8, 0x1f, 10, -1});
}

void Passive10_0101(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0x20);
}

void Passive10_0102(PLW* wk) {
    pattern_em_term_normal_attack_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, -0x7FB0, 6, 1, -1},
        0x200,
        &(Command_Attack_Args){8, 0x1e, 10, -1}
    );
}

void Passive10_0103(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up_2(wk, 0x70, 0x68, 0x20);
}

void Passive10_0104(PLW* wk) {
    pattern_hi_jump(wk, 0xa);
}

void Passive10_0105(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack(wk, 0x70, 0x68, &(Command_Attack_Args){8, 1, 0xa, -1});
}
