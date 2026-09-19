/**
 * @file pass00_2.c
 * COM Passive: Gill
 * Pattern scripts from Passive00_0051 on.
 */

#include "sf33rd/Source/Game/com/passive/pass00.h"
#include "sf33rd/Source/Game/com/passive/pass00_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive00_0051(PLW* wk) {
    pattern_normal_attack_7(
        wk,
        &(Normal_Attack_Step){ 0xB, 0x220 },
        &(Normal_Attack_Step){ 0xB, 0x12 },
        &(Normal_Attack_Step){ 8, 0x402 }
    );
}

void Passive00_0052(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Passive00_0053(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive00_0054(PLW* wk) {
    pattern_wait_get_up_normal_attack_3(wk, 0, &(Normal_Attack_Step){ 8, 0x10 }, 0x10);
}

void Passive00_0055(PLW* wk) {
    pattern_lever_attack(wk, 8, 0x40);
}

void Passive00_0056(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, -0x7FA8, 8, 0x100, 0, -1, -1, -1});
}

void Passive00_0057(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA8, 8, 0x10, 0, -1, -1, -1});
}

void Passive00_0058(PLW* wk) {
    pattern_keep_status(wk, 1);
}

void Passive00_0059(PLW* wk) {
    pattern_etc_term_normal_attack(wk);
}

void Passive00_0060(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive00_0061(PLW* wk) {
    passive00_pattern_etc_term_command_attack(wk, 0x99, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive00_0062(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x43, 0x110);
}

void Passive00_0063(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive00_0064(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x20);
}

void Passive00_0065(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x33, 0, 0x110);
}

void Passive00_0066(PLW* wk) {
    passive00_pattern_etc_term_command_attack(wk, 0x99, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive00_0067(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive00_0068(PLW* wk) {
    passive00_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x59, 0x59, 0x59, 0x5a});
}

void Passive00_0069(PLW* wk) {
    pattern_pierce_on_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Passive00_0070(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x33, 0, 0x110);
}

void Passive00_0071(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x33, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x220);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x12);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0072(PLW* wk) {
    pattern_pierce_on_command_attack_normal_attack_3(wk);
}

void Passive00_0073(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_4(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 8, 0x400, 0, -0x7F90, -1, 0x20},
        0x12,
        0x42
    );
}

void Passive00_0074(PLW* wk) {
    pattern_normal_attack_5(wk, 9, 0xB, 0x20);
}

void Passive00_0075(PLW* wk) {
    pattern_normal_attack_j_command_attack_4(wk, 0x20, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Passive00_0076(PLW* wk) {
    pattern_approach_walk_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-1, -0x7FC0, 8, 0x400, 0, -0x7F90, -1, 0x20},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Passive00_0077(PLW* wk) {
    passive00_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-1, -0x7FC0, 0xB, 0x40, 0, -0x7F90, -1, 0x200},
        0x400
    );
}

void Passive00_0078(PLW* wk) {
    pattern_jump_attack_term_normal_attack_5(wk, &(Jump_Term_Args){-1, -0x7FC0, 0xB, 0x400, 0, -0x7F90, -1, 0x20});
}

void Passive00_0079(PLW* wk) {
    passive00_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-1, -0x7FC0, 0xB, 0x40, 0, -1, -1, -1},
        0x40
    );
}

void Passive00_0080(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_4(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FB0, 0xB, 0x40, 0, -0x7F90, -1, 0x200},
        0x200,
        0x40
    );
}

void Passive00_0081(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_4(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7F90, -1, 0x20},
        0x10,
        0x200
    );
}

void Passive00_0082(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack(wk, &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x40, 0, -1, -1, -1}, 0x200);
}

void Passive00_0083(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_j_command_attack(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7F90, -1, 0x20},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Passive00_0084(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_command_attack(
        wk,
        &(Hi_Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7F90, -1, 0x200},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive00_0085(PLW* wk) {
    pattern_wait_j_command_attack(wk, 8, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive00_0086(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_BOSS(wk, 6, 0x49);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x42);
        break;

    case 4:
        Wait(wk, 5);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0087(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_BOSS(wk, 6, 0x49);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x42);
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1F, 10, -1});
        break;

    case 5:
        ETC_Term(wk, 2, 6, 1);
        break;

    default:
        pattern_command_attack_from_step_6(wk);
        break;
    }
}

void Passive00_0088(PLW* wk) {
    pattern_pierce_on_approach_walk_jump_attack_term(
        wk,
        &(Jump_Term_Args){-1, -0x7FC0, 0xB, 0x400, 0, -0x7F90, -1, 0x200},
        &(Command_Attack_Args){0xB, 0x1F, 10, -1}
    );
}

void Passive00_0089(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive00_0090(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive00_0091(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Passive00_0092(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive00_0093(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x102, 0x102);
}

void Passive00_0094(PLW* wk) {
    passive00_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5A, 0x5B});
}

void Passive00_0095(PLW* wk) {
    pattern_walk_com_random_select_2(wk);
}

void Passive00_0096(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1}, 0x402);
}

void Passive00_0097(PLW* wk) {
    passive00_pattern_approach_walk_em_term_lever_attack(wk, &(EM_Term_Params){-1, -0x7FFE, 6, 1, -1}, 0, 0x110);
}

void Passive00_0098(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x33, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x42);
        break;

    case 4:
        Check_BOSS(wk, 6, 0xe);
        break;

    case 5:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1f, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0099(PLW* wk) {
    pattern_normal_attack_j_command_attack_4(wk, 0x10, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Passive00_0100(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Check_BOSS(wk, 6, 0x88);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0101(PLW* wk) {
    pattern_wait_get_up_com_random_select(wk, 0, -1, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5A, 0x5B});
}

void Passive00_0102(PLW* wk) {
    pattern_wait_get_up_com_random_select(wk, 0, -1, &(Branch_Menu_Args){6, 0x59, 0x59, 0x59, 0x5A});
}

void Passive00_0103(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x60, 0x68, 0x20);
}

void Passive00_0104(PLW* wk) {
    pattern_hi_jump(wk, 0);
}

void Passive00_0105(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack(wk, 0x60, 0x68, &(Command_Attack_Args){8, 1, 0xA, -1});
}

void Passive00_0106(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x60, 0x68, 0x38);
}

void Passive00_0107(PLW* wk) {
    pattern_pierce_on_em_term_normal_attack_2(wk);
}

void Passive00_0108(PLW* wk) {
    active_pattern_approach_walk(wk, 0xBB);
}

void Passive00_0109(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7F90, 6, 1, -1});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1F, 0xA, -1});
        break;

    case 3:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0110(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive00_0111(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x7B, &(EM_Term_Params){-1, -0x7FA0, 6, 1, -1}, 0x42);
}
