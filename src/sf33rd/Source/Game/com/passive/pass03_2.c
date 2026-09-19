/**
 * @file pass03_2.c
 * COM Passive: Yun
 * Pattern scripts from Passive03_0049 on.
 */

#include "sf33rd/Source/Game/com/passive/pass03.h"
#include "sf33rd/Source/Game/com/passive/pass03_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive03_0049(PLW* wk) {
    pattern_adjust_attack(wk, 0x10);
}

void Passive03_0050(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x12, 0x22);
}

void Passive03_0051(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x220);
}

void Passive03_0052(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Passive03_0053(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 2, -0x7FA0, -1, -0x7C00});
}

void Passive03_0054(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 2, -0x7FA0, -1, 0x8400});
}

void Passive03_0055(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Passive03_0056(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -0x7FA0, -1, 0x40});
}

void Passive03_0057(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x20, 0, -0x7FA0, -1, 0x40});
}

void Passive03_0058(PLW* wk) {
    pattern_keep_status(wk, 1);
}

void Passive03_0059(PLW* wk) {
    pattern_normal_attack_2(wk, 0xB, 0x20, 0x40);
}

void Passive03_0060(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive03_0061(PLW* wk) {
    pattern_sa_term_normal_attack(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0}, 8, 0x400);
}

void Passive03_0062(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x2F, 0, 0x110);
}

void Passive03_0063(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Passive03_0064(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x200);
}

void Passive03_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x2F, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x100);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive03_0066(PLW* wk) {
    pattern_etc_term_branch_unit_area(wk, 6, 0x40, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5A, 0x5B});
}

void Passive03_0067(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 10, -1});
}

void Passive03_0068(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -0x7FA0, -1, 0x40});
}

void Passive03_0069(PLW* wk) {
    pattern_command_attack_branch_unit_area(
        wk,
        &(Command_Attack_Args){0xC, 0, 0xB, -1},
        &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 0x5B}
    );
}

void Passive03_0070(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive03_0071(PLW* wk) {
    pattern_approach_walk_command_attack(
        wk,
        0x2F,
        &(Command_Attack_Args){8, 0x1C, 10, -1},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive03_0072(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x20, 8, -1}
    );
}

void Passive03_0073(PLW* wk) {
    pattern_approach_walk_hi_jump_attack_term_command_attack(
        wk,
        0xB7,
        &(Hi_Jump_Term_Args){-0x7FA0, -1, 0xB, 0x8400, 0, -0x7FA0, -1, 0x40},
        &(Command_Attack_Args){8, 0x20, 8, -1}
    );
}

void Passive03_0074(PLW* wk) {
    pattern_sa_term_normal_attack_com_random_select(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0}, 0xC);
}

void Passive03_0075(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x2F, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 5:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    default:
        pattern_com_random_select_from_step_6(wk);
        break;
    }
}

void Passive03_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0xB7, 2);
        break;

    case 2:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 0, -0x7FA0, -1, 0x40});
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x10);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 5:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    default:
        pattern_com_random_select_from_step_6(wk);
        break;
    }
}

void Passive03_0077(PLW* wk) {
    pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FD0, 0xB, 0x20, 0, -0x7FA0, -1, 0x40},
        8,
        0x400
    );
}

void Passive03_0078(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack_3(wk, 0xB, 0xC, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Passive03_0079(PLW* wk) {
    pattern_jump_attack_term_normal_attack(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 0xB, 0x20, 0, -1, -1, -1}, 8, 0x40);
}

void Passive03_0080(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7FA0, -0x7FD0, 0xB, 0x40, 0, -0x7FA0, -1, 0x40},
        0x400
    );
}

void Passive03_0081(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_lever_attack_2(
        wk,
        &(Hi_Jump_Term_Args){-0x7FA0, -1, 0xB, 0x8200, 0, -0x7FA0, -1, 0x40},
        0xB
    );
}

void Passive03_0082(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7FA0, -0x7FC0, 0xB, 0x40, 0, -1, -1, 0xFFFF},
        0x400
    );
}

void Passive03_0083(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FC0, 0xB, 0x40, 0, -0x7FA0, -1, 0x40});
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive03_0084(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_com_random_select(wk, 0xB, 0xC);
}

void Passive03_0085(PLW* wk) {
    pattern_wait_command_attack(wk, 10, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive03_0086(PLW* wk) {
    pattern_approach_walk_em_term_sa_term_4(wk, 0x2F, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0}, 0xB);
}

void Passive03_0087(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x2F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x10);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 5:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    default:
        pattern_com_random_select_from_step_6(wk);
        break;
    }
}

void Passive03_0088(PLW* wk) {
    pattern_em_term_sa_term_normal_attack(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0}, 0xB, 0xC);
}

void Passive03_0089(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive03_0090(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive03_0091(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive03_0092(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FE0, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 0x5B}
    );
}

void Passive03_0093(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x102, 0x102);
}

void Passive03_0094(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
}

void Passive03_0095(PLW* wk) {
    pattern_walk_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 0x5B});
}

void Passive03_0096(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1}, 0x402);
}

void Passive03_0097(PLW* wk) {
    pattern_approach_walk_em_term_lever_attack(wk, 0x2F, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 0);
}

void Passive03_0098(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x2F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 4:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive03_0099(PLW* wk) {
    pattern_normal_attack_com_random_select_4(wk, 0xB, 0xC);
}

void Passive03_0100(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x30, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive03_0101(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0x20);
}

void Passive03_0102(PLW* wk) {
    pattern_em_term_normal_attack_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, -0x7FB0, 6, 1, -1},
        0x200,
        &(Command_Attack_Args){8, 0x20, 10, -1}
    );
}

void Passive03_0103(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x70, 0x68, 0x20);
}

void Passive03_0104(PLW* wk) {
    pattern_hi_jump(wk, 3);
}

void Passive03_0105(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack(wk, 0x70, 0x68, &(Command_Attack_Args){8, 1, 10, -1});
}
