/**
 * @file pass10_3.c
 * COM Passive: Yang
 * Pattern scripts from Passive10_0106 on.
 */

#include "sf33rd/Source/Game/com/passive/pass10.h"
#include "sf33rd/Source/Game/com/passive/pass10_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive10_0106(PLW* wk) {
    pattern_search_back_term_walk_wait_get_up(wk, 0x70, 0x68, 0x38);
}

void Passive10_0107(PLW* wk) {
    pattern_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0x2e, 0x2f, 0x30, 0},
        &(Branch_Menu_Args){6, 0x37, 0x37, 0x27, 0x27},
        0
    );
}

void Passive10_0108(PLW* wk) {
    pattern_approach_walk(wk, 0xbf);
}

void Passive10_0109(PLW* wk) {
    pattern_sa_term_branch_unit_area(
        wk,
        &(SA_Term_Args){0x2e, 0xffff, 0x30, 0},
        &(Branch_Menu_Args){6, 0x59, 0x5a, 0x5b, 0x5b}
    );
}

void Passive10_0110(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xc, 0x20);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2e, 0x2f, 0xffff, 0});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x37, 0x37, 0x27, 0x27}, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0111(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 0x200);
}

void Passive10_0112(PLW* wk) {
    pattern_em_term_normal_attack_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        0x200,
        &(Command_Attack_Args){8, 0x1e, 8, -1}
    );
}

void Passive10_0113(PLW* wk) {
    pattern_em_term_j_command_attack(wk, &(EM_Term_Params){-1, 0x20, 6, 1, -1}, &(Command_Attack_Args){8, 0x1f, 8, -1});
}

void Passive10_0114(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 1, -1, 0x20, 0x400});
}

void Passive10_0115(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xb, 0x220);
        break;

    case 1:
        Normal_Attack(wk, 0xb, 0x102);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1f, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0116(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x400, 0, -1, 0x20, 0x400});
}

void Passive10_0117(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x200, 1, -1, 0x20, 0x200});
}

void Passive10_0118(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x20, 8, 0x200, 0, -1, 0x20, 0x200});
}

void Passive10_0119(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x400);
}

void Passive10_0120(PLW* wk) {
    pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xe, 0x1e, 10, -1},
        &(Command_Attack_Args){0xe, 0x1e, 10, -1}
    );
}

void Passive10_0121(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 10, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 10, -1});
        break;

    case 3:
        Wait(wk, 0xe);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1e, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0122(PLW* wk) {
    pattern_pierce_on_command_attack_2(
        wk,
        &(Command_Attack_Args){0xe, 0x1e, 10, -1},
        &(Command_Attack_Args){8, 0x1e, 10, -1},
        &(Command_Attack_Args){8, 0x1e, 10, -1}
    );
}

void Passive10_0123(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive10_0124(PLW* wk) {
    pattern_normal_attack(wk, 0xd, 0x100);
}

void Passive10_0125(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 10, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 10, -1});
        break;

    case 3:
        Wait(wk, 0xe);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0126(PLW* wk) {
    pattern_pierce_on_command_attack_2(
        wk,
        &(Command_Attack_Args){0xe, 0x1e, 10, -1},
        &(Command_Attack_Args){0xe, 0x1e, 10, -1},
        &(Command_Attack_Args){0xe, 0x1e, 10, -1}
    );
}

void Passive10_0127(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xB, 0x700});
}

void Passive10_0128(PLW* wk) {
    pattern_pierce_on_command_attack_wait(wk);
}

void Passive10_0129(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 10, -1});
        break;

    case 2:
        Wait(wk, 0xe);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 10, -1});
        break;

    case 4:
        Wait(wk, 0xe);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0130(PLW* wk) {
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

void Passive10_0131(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x30, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 0xB, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0132(PLW* wk) {
    pattern_pierce_on_command_attack_wait(wk);
}

void Passive10_0133(PLW* wk) {
    pattern_provoke_wait_get_up(wk);
}

void Passive10_0134(PLW* wk) {
    pattern_pierce_on_command_attack_wait(wk);
}

void Passive10_0135(PLW* wk) {
    pattern_jump(wk, 1);
}

void Passive10_0136(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0137(PLW* wk) {
    pattern_jump(wk, 0);
}
