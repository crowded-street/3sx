/**
 * @file pass16_2.c
 * COM Passive: Makoto
 * Pattern scripts from Passive16_0047 on.
 */

#include "sf33rd/Source/Game/com/passive/pass16.h"
#include "sf33rd/Source/Game/com/passive/pass16_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive16_0047(PLW* wk) {
    pattern_normal_attack_command_attack(wk, 0xB, 0x102, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive16_0048(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x402);
}

void Passive16_0049(PLW* wk) {
    pattern_em_term_adjust_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1});
}

void Passive16_0050(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 8, 0x200);
}

void Passive16_0051(PLW* wk) {
    pattern_normal_attack_3(wk, 0x220, 0xB, 0x102);
}

void Passive16_0052(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x202);
}

void Passive16_0053(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive16_0054(PLW* wk) {
    pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x6A, 0x69, 0x68, 1});
}

void Passive16_0055(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive16_0056(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7FA0, -1, 0x20});
}

void Passive16_0057(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x20, 0, -0x7FA0, -1, 0x20});
}

void Passive16_0058(PLW* wk) {
    pattern_keep_status(wk, 1);
}

void Passive16_0059(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive16_0060(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive16_0061(PLW* wk) {
    pattern_sa_term_approach_walk_sa_term_2(wk);
}

void Passive16_0062(PLW* wk) {
    pattern_approach_walk_etc_term_com_random_select(wk);
}

void Passive16_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 0, 6, 0x21);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    case 3:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16_0064(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x400);
}

void Passive16_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x42, 0x46, 0x69, 0x4A}, 1);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x4B, 2);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 3, 0x38, 0x44, 0x45}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16_0067(PLW* wk) {
    pattern_approach_walk_normal_attack(wk, 0x7F, 0x402);
}

void Passive16_0068(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7FA0, -1, 0x400});
}

void Passive16_0069(PLW* wk) {
    pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 0xF, 0, 0x40});
}

void Passive16_0070(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x7F, 0, 0x400);
}

void Passive16_0071(PLW* wk) {
    pattern_approach_walk_com_random_select(wk, 0x37, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}, 1);
}

void Passive16_0072(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0, 0xB, -1});
        break;

    case 2:
        Approach_Walk(wk, 0x4B, 2);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 3, 0x38, 0x44, 0x45}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16_0073(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x6A, 0x69, 0x68, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16_0074(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x41, 2);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 3, 0x38, 0x44, 0x45}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16_0075(PLW* wk) {
    pattern_sa_term_approach_walk_sa_term_2(wk);
}

void Passive16_0076(PLW* wk) {
    pattern_sa_term_command_attack(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0}, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive16_0077(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x40, 0, -0x7FA0, -1, 0x200}
    );
}

void Passive16_0078(PLW* wk) {
    pattern_jump_attack_term_normal_attack_3(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x100, 0, -0x7FA0, -1, 0x20},
        0x10,
        0x200
    );
}

void Passive16_0079(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20}
    );
}

void Passive16_0080(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x400},
        0x200
    );
}

void Passive16_0081(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_2(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x100, 0, -0x7FA0, -1, 0x400},
        0x10,
        0x200
    );
}

void Passive16_0082(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x20, 0, -0x7FA0, -1, 0x20},
        0x400
    );
}

void Passive16_0083(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_3(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20},
        0x200
    );
}

void Passive16_0084(PLW* wk) {
    pattern_hi_jump_attack_normal_attack(wk, &(Jump_Attack_Args){0xC, 0xF, 0x40, 0}, 0x200);
}

void Passive16_0085(PLW* wk) {
    pattern_wait_branch_unit_area(wk, 3, &(Branch_Menu_Args){6, 0x6A, 0x69, 0x68, 1});
}

void Passive16_0086(PLW* wk) {
    pattern_approach_walk_em_term_sa_term_2(wk);
}

void Passive16_0087(PLW* wk) {
    pattern_approach_walk_em_term_sa_term_2(wk);
}

void Passive16_0088(PLW* wk) {
    pattern_approach_walk_em_term_sa_term_2(wk);
}

void Passive16_0089(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive16_0090(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive16_0091(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive16_0092(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-0x7FF0, -0x7FA0, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1}
    );
}

void Passive16_0093(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x12);
}

void Passive16_0094(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x20);
}

void Passive16_0095(PLW* wk) {
    pattern_normal_attack_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x69, 0x69, 0x68, 1});
}

void Passive16_0096(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-0x7F80, -0x7FC8, 6, 1, -1}, 8, 0x402);
}

void Passive16_0097(PLW* wk) {
    pattern_approach_walk_em_term_com_random_select(wk, 0x37, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F});
}

void Passive16_0098(PLW* wk) {
    pattern_approach_walk_em_term_command_attack_2(
        wk,
        0x37,
        &(Command_Attack_Args){8, 0x1E, 10, -1},
        &(Branch_Menu_Args){6, 0x42, 0x46, 0x69, 0x4A}
    );
}

void Passive16_0099(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_lever_attack(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x200},
        0x20
    );
}

void Passive16_0100(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x4B, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 3, 0x38, 0x44, 0x45}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16_0101(PLW* wk) {
    pattern_normal_attack_sa_term_command_attack_2(
        wk,
        8,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive16_0102(PLW* wk) {
    pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0, 0x20);
}

void Passive16_0103(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x402);
}

void Passive16_0104(PLW* wk) {
    pattern_jump_command_attack_term(
        wk,
        &(JCA_Term_Args){8, 0x2E, 8, -1, -0x7FA0, -0x7FA8, 0, -1, 0x30, 0x400}
    );
}

void Passive16_0105(PLW* wk) {
    pattern_jump_command_attack_term(
        wk,
        &(JCA_Term_Args){8, 0x2E, 9, -1, -0x7FA0, -0x7FA8, 0, -1, 0x30, 0x400}
    );
}

void Passive16_0106(PLW* wk) {
    pattern_jump_command_attack_term(
        wk,
        &(JCA_Term_Args){8, 0x2E, 0xA, -1, -0x7FA0, -0x7FA8, 0, -1, 0x30, 0x400}
    );
}

void Passive16_0107(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x60, 0x6C, 0x20);
}

void Passive16_0108(PLW* wk) {
    pattern_hi_jump(wk, 1);
}

void Passive16_0109(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack(wk, 0x60, 0x6C, &(Command_Attack_Args){8, 1, 10, -1});
}

void Passive16_0110(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up_2(wk, 0x60, 0x6C, 0x38);
}
