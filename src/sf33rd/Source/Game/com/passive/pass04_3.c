/**
 * @file pass04_3.c
 * COM Passive: Dudley
 * Pattern scripts from Passive04_0133 on.
 */

#include "sf33rd/Source/Game/com/passive/pass04.h"
#include "sf33rd/Source/Game/com/passive/pass04_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive04_0133(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F90, -1, 0, 1, -1},
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive04_0134(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive04_0135(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, 0x70}
    );
}

void Passive04_0136(PLW* wk) {
    pattern_jump(wk, 0);
}

void Passive04_0137(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x21, 8, -1});
}

void Passive04_0138(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x21, 10, 0x700});
}

void Passive04_0139(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xB3, 2);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FA0, 8, 0x400, 0, -0x7F78, -1, 0x40});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive04_0140(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xB3, 2);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FA0, 8, 0x400, 0, -0x7F78, -1, 0x40});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive04_0141(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xB3, 2);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FA0, 8, 0x400, 0, -0x7F78, -1, 0x40});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
