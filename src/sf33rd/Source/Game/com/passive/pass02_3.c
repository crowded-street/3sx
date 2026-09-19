/**
 * @file pass02_3.c
 * COM Passive: Ryu
 * Pattern scripts from Passive02_0117 on.
 */

#include "sf33rd/Source/Game/com/passive/pass02.h"
#include "sf33rd/Source/Game/com/passive/pass02_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive02_0117(PLW* wk) {
    pattern_pierce_on_jump_com_random_select(wk, &(Branch_Menu_Args){6, 0x20, 0x21, 0x40, 0x78}, 4);
}

void Passive02_0118(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive02_0119(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive02_0120(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Passive02_0121(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 0xB, 0x400, 0, -0x7FB0, -1, 0x20});
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x8000, 4, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0x7F});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive02_0122(PLW* wk) {
    pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x1E, 10, -1, -1, 0x30, 0, -1, -1, 0xFFFF});
}

void Passive02_0123(PLW* wk) {
    pattern_pierce_on_j_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, 0x700});
}

void Passive02_0124(PLW* wk) {
    pattern_keep_away(wk, 0xBF, 1);
}

void Passive02_0125(PLW* wk) {
    pattern_walk_com_random_select(wk, 0x20, &(Branch_Menu_Args){6, 0x36, 0x7E, 0x37, 0x6D}, 3);
}

void Passive02_0126(PLW* wk) {
    pattern_lever_off_look(wk);
}

void Passive02_0127(PLW* wk) {
    pattern_jump_attack_term_normal_attack_j_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F90, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x200},
        0x40,
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Passive02_0128(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 8, 0x402, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive02_0129(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Passive02_0130(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive02_0131(PLW* wk) {
    pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -1, -1, 0xFFFF});
}

void Passive02_0132(PLW* wk) {
    pattern_pierce_on_command_attack_em_term(wk);
}
