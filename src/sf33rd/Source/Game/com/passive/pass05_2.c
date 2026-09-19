/**
 * @file pass05_2.c
 * COM Passive: Necro
 * Pattern scripts from Passive05_0070 on.
 */

#include "sf33rd/Source/Game/com/passive/pass05.h"
#include "sf33rd/Source/Game/com/passive/pass05_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive05_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x67, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0x60});
        break;

    case 3:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0071(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FA8, -0x7FF8, 6, 1, -1},
        &(SA_Term_Args){0xFFFF, 0x36, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive05_0072(PLW* wk) {
    pattern_em_term_sa_term_forced_guard(
        wk,
        &(EM_Term_Params){-0x7F80, -0x7FF8, 5, 1, -1},
        &(SA_Term_Args){0x35, 0xFFFF, 0x37, 0}
    );
}

void Passive05_0073(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -1, 5, 1, -1});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x35, 0xFFFF, 0x37, 0});
        break;

    case 2:
        Adjust_Attack(wk, 0xB, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 10, 0x202);
        break;

    case 4:
        Normal_Attack(wk, 10, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0074(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -0x7FF0, 5, 1, -1},
        &(SA_Term_Args){0x35, 0xFFFF, 0x37, 0},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive05_0075(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7FC0, -1, 5, 6, 0x1F});
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x12);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x41D, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0076(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 6, 0x1C}, 0, 0x20);
}

void Passive05_0077(PLW* wk) {
    pattern_wait_attack_complete_normal_attack(wk);
}

void Passive05_0078(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x3F, 0x402);
}

void Passive05_0079(PLW* wk) {
    pattern_forced_guard(wk, 2);
}

void Passive05_0080(PLW* wk) {
    active_pattern_(wk);
}

void Passive05_0081(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FA8, 9, 0x40, 0, -0x7FB0, -1, 0x400});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x20);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){9, 0x1E, 10, -1});
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0x60});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0082(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){9, 0x1C, 10, -1});
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0x60});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0083(PLW* wk) {
    pattern_jump(wk, 2);
}

void Passive05_0084(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 9, 0x202, 1, -0x7FB0, -1, 0x20});
}

void Passive05_0085(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FA0, -0x7FF8, 5, 1, -1}, 0xD, 0x100);
}

void Passive05_0086(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA(wk, 6, 0x5A);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 10, -1});
        break;

    case 2:
        Wait(wk, 5);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0087(PLW* wk) {
    active_pattern_approach_walk(wk, 0xB7);
}

void Passive05_0088(PLW* wk) {
    pattern_search_back_term_command_attack_2(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive05_0089(PLW* wk) {
    active_pattern_normal_attack(wk, 9, 0x220);
}

void Passive05_0090(PLW* wk) {
    active_pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 10, -1}, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive05_0091(PLW* wk) {
    pattern_em_term_sa_term_forced_guard(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 5, 1, -1},
        &(SA_Term_Args){0x35, 0xFFFF, 0xFFFF, 0}
    );
}

void Passive05_0092(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x50, 8, 0x202, 2, -0x7F80, -1, 0x20});
}

void Passive05_0093(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7F78, 9, 0x400, 0, -0x7FA0, -1, 0x20});
}

void Passive05_0094(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, 0x30, 0, 1, -1}, 8, 0x22);
}

void Passive05_0095(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -1, 0, 1, -1},
        &(Command_Attack_Args){8, 0x41D, 10, -1}
    );
}

void Passive05_0096(PLW* wk) {
    pattern_em_term_normal_attack_4(wk, &(EM_Term_Params){-1, -0x7FF8, 0, 1, -1});
}

void Passive05_0097(PLW* wk) {
    active_pattern_approach_walk(wk, 0x67);
}

void Passive05_0098(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1});
}

void Passive05_0099(PLW* wk) {
    pattern_jump(wk, 0);
}

void Passive05_0100(PLW* wk) {
    pattern_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0x35, 0xFFFF, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x41D, 10, -1}
    );
}

void Passive05_0101(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x22);
}
