/**
 * @file pass19_2.c
 * COM Passive: Remy
 * Pattern scripts from Passive19_0058 on.
 */

#include "sf33rd/Source/Game/com/passive/pass19.h"
#include "sf33rd/Source/Game/com/passive/pass19_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive19_0058(PLW* wk) {
    pattern_keep_status(wk, 1);
}

void Passive19_0059(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive19_0060(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive19_0061(PLW* wk) {
    pattern_sa_term_command_attack(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0}, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive19_0062(PLW* wk) {
    pattern_approach_walk_etc_term_com_random_select(wk);
}

void Passive19_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 0, 6, 0x21);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x10);
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0064(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x400);
}

void Passive19_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}, 1);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0066(PLW* wk) {
    pattern_approach_walk_j_command_attack(wk, 0x4B, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive19_0067(PLW* wk) {
    pattern_approach_walk_normal_attack(wk, 0x7F, 0x402);
}

void Passive19_0068(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7FA0, -1, 0x400});
}

void Passive19_0069(PLW* wk) {
    pattern_jump_attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x400, 0});
}

void Passive19_0070(PLW* wk) {
    pattern_approach_walk_normal_attack(wk, 0x7F, 0x402);
}

void Passive19_0071(PLW* wk) {
    pattern_approach_walk_com_random_select(wk, 0x37, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}, 1);
}

void Passive19_0072(PLW* wk) {
    pattern_pierce_on_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive19_0073(PLW* wk) {
    pattern_approach_walk_normal_attack_branch_unit_area(wk, 0xBF, &(Branch_Menu_Args){6, 0x68, 0x69, 0x69, 0x6A});
}

void Passive19_0074(PLW* wk) {
    pattern_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0},
        &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F},
        1
    );
}

void Passive19_0075(PLW* wk) {
    pattern_sa_term_command_attack(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0}, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive19_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x93, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0077(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x40, 0, -0x7FA0, -1, 0x200}
    );
}

void Passive19_0078(PLW* wk) {
    pattern_jump_attack_term_normal_attack_3(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x10, 0, -0x7FA0, -1, 0x20},
        0x10,
        0x200
    );
}

void Passive19_0079(PLW* wk) {
    pattern_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x400, 0, -0x7FA0, -1, 0x20}
    );
}

void Passive19_0080(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x400},
        0x200
    );
}

void Passive19_0081(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_2(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x10, 0, -0x7FA0, -1, 0x400},
        0x10,
        0x200
    );
}

void Passive19_0082(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(
        wk,
        &(Hi_Jump_Term_Args){-0x7F60, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x20},
        0x200
    );
}

void Passive19_0083(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_3(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20},
        0x200
    );
}

void Passive19_0084(PLW* wk) {
    pattern_hi_jump_attack_normal_attack(wk, &(Jump_Attack_Args){0xC, 0xC, 0x402, 0}, 0x200);
}

void Passive19_0085(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 2);
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 5, 5, 7, 8}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0086(PLW* wk) {
    pattern_em_term_sa_term_em_term(
        wk,
        &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1},
        &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 8, -1}
    );
}

void Passive19_0087(PLW* wk) {
    pattern_em_term_sa_term_em_term(
        wk,
        &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1},
        &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 8, -1}
    );
}

void Passive19_0088(PLW* wk) {
    pattern_em_term_sa_term_em_term(
        wk,
        &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1},
        &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 8, -1}
    );
}

void Passive19_0089(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive19_0090(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive19_0091(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive19_0092(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FA0, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 0x5B}
    );
}

void Passive19_0093(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x12);
}

void Passive19_0094(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x20);
}

void Passive19_0095(PLW* wk) {
    pattern_normal_attack_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x68, 0x69, 0x6A, 1});
}

void Passive19_0096(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0x7F, 3);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -0x7FD8, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0097(PLW* wk) {
    pattern_approach_walk_em_term_com_random_select(wk, 0x37, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F});
}

void Passive19_0098(PLW* wk) {
    pattern_approach_walk_em_term_command_attack_2(
        wk,
        0x37,
        &(Command_Attack_Args){8, 0x1D, 10, -1},
        &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}
    );
}

void Passive19_0099(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_lever_attack(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x200},
        0x20
    );
}

void Passive19_0100(PLW* wk) {
    pattern_approach_walk_wait_get_up_sa_term(
        wk,
        0x4B,
        &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1F, 8, -1}
    );
}

void Passive19_0101(PLW* wk) {
    pattern_normal_attack_sa_term_branch_unit_area(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0},
        &(Branch_Menu_Args){6, 0x68, 0x69, 0x6A, 0x6A}
    );
}

void Passive19_0102(PLW* wk) {
    pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0, 0x20);
}

void Passive19_0103(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x42);
}

void Passive19_0104(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive19_0105(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive19_0106(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive19_0107(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x60, 0x6C, 0x20);
}

void Passive19_0108(PLW* wk) {
    pattern_hi_jump(wk, 1);
}

void Passive19_0109(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack(wk, 0x60, 0x6C, &(Command_Attack_Args){8, 1, 10, -1});
}

void Passive19_0110(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up_2(wk, 0x60, 0x6C, 0x38);
}

void Passive19_0111(PLW* wk) {
    pattern_em_term_normal_attack_com_random_select(wk, 0x400);
}

void Passive19_0112(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x400);
}

void Passive19_0113(PLW* wk) {
    pattern_normal_attack_com_random_select_2(wk, 0xC, 0x400);
}

void Passive19_0114(PLW* wk) {
    pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1D, 9, -1}
    );
}

void Passive19_0115(PLW* wk) {
    pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x20});
}

void Passive19_0116(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x20});
}

void Passive19_0117(PLW* wk) {
    pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x100, 0});
}

void Passive19_0118(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x200);
}

void Passive19_0119(PLW* wk) {
    pattern_lever_on_em_term_check_store_lever(wk, 0x1F);
}

void Passive19_0120(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 1, -1, 0x20, 0x400});
}

void Passive19_0121(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x20, 1, -1, 0x20, 0x20});
}

void Passive19_0122(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x40, 1, -1, 0x20, 0x40});
}
