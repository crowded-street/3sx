/**
 * @file pass03_3.c
 * COM Passive: Yun
 * Pattern scripts from Passive03_0106 on.
 */

#include "sf33rd/Source/Game/com/passive/pass03.h"
#include "sf33rd/Source/Game/com/passive/pass03_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive03_0106(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x70, 0x68, 0x38);
}

void Passive03_0107(PLW* wk) {
    pattern_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0},
        &(Branch_Menu_Args){6, 0x37, 0x37, 0x27, 0x27},
        0
    );
}

void Passive03_0108(PLW* wk) {
    pattern_approach_walk(wk, 0xB7);
}

void Passive03_0109(PLW* wk) {
    pattern_sa_term_branch_unit_area(
        wk,
        &(SA_Term_Args){0x2E, 0xFFFF, 0x30, 0},
        &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 0x5B}
    );
}

void Passive03_0110(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x20);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x37, 0x37, 0x27, 0x27}, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive03_0111(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 0x200);
}

void Passive03_0112(PLW* wk) {
    pattern_em_term_normal_attack_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        0x200,
        &(Command_Attack_Args){8, 0x20, 8, -1}
    );
}

void Passive03_0113(PLW* wk) {
    pattern_em_term_j_command_attack(wk, &(EM_Term_Params){-1, 0x20, 6, 1, -1}, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive03_0114(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 1, -1, 0x20, 0x400});
}

void Passive03_0115(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x220);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive03_0116(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 0, -1, 0x20, 0x400});
}

void Passive03_0117(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x200, 1, -1, 0x20, 0x200});
}

void Passive03_0118(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x200, 0, -1, 0x20, 0x200});
}

void Passive03_0119(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x400);
}

void Passive03_0120(PLW* wk) {
    pattern_normal_attack(wk, 0xD, 0x100);
}

void Passive03_0121(PLW* wk) {
    pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x7A, 0x7B, 0x7C, 0x7D});
}

void Passive03_0122(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive03_0123(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive03_0124(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive03_0125(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 0xB, 0x70});
}

void Passive03_0126(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive03_0127(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Normal_Attack(wk, 0xD, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive03_0128(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x20, 0xB, 0x70});
}

void Passive03_0129(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1F, 0xB, 0x700});
}

void Passive03_0130(PLW* wk) {
    pattern_provoke_wait_get_up(wk);
}

void Passive03_0131(PLW* wk) {
    pattern_wait_get_up_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 0xB, 0x70});
}

void Passive03_0132(PLW* wk) {
    pattern_wait_get_up_command_attack_2(wk, &(Command_Attack_Args){8, 0x20, 0xB, 0x70});
}

void Passive03_0133(PLW* wk) {
    pattern_wait_get_up_command_attack_2(wk, &(Command_Attack_Args){8, 0x1F, 0xB, 0x700});
}

void Passive03_0134(PLW* wk) {
    pattern_jump(wk, 1);
}

void Passive03_0135(PLW* wk) {
    pattern_jump(wk, 0);
}
