/**
 * @file pass09_3.c
 * COM Passive: Oro
 * Pattern scripts from Passive09_0116 on.
 */

#include "sf33rd/Source/Game/com/passive/pass09.h"
#include "sf33rd/Source/Game/com/passive/pass09_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive09_0116(PLW* wk) {
    passive09_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 1, 0x40, 1, -1, -1, 0xFFFF, -0x7FB0, -0x7FD0, 0x20});
}

void Passive09_0117(PLW* wk) {
    passive09_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 1, 0x50, 0, -1, -1, 0xFFFF, -0x7FB0, -0x7FD0, 0x20});
}

void Passive09_0118(PLW* wk) {
    passive09_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 2, 0x40, 0, -1, -1, 0xFFFF, -0x7FB0, -0x7FD0, 0x20});
}

void Passive09_0119(PLW* wk) {
    passive09_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 2, 0x40, 1, -1, -1, 0xFFFF, -0x7FB0, -0x7FD0, 0x20});
}

void Passive09_0120(PLW* wk) {
    passive09_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x74, 0x74, 0x75, 0x75});
}

void Passive09_0121(PLW* wk) {
    passive09_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 0, 0x40, 2, -1, -1, 0xFFFF, -0x7FB0, -0x7FD0, 0x400});
}

void Passive09_0122(PLW* wk) {
    passive09_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 0, 0x50, 0, -1, -1, 0xFFFF, -0x7FB0, -0x7FD0, 0x400});
}

void Passive09_0123(PLW* wk) {
    passive09_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x79, 0x7A, 0x7A, 0x2F});
}

void Passive09_0124(PLW* wk) {
    passive09_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x76, 0x76, 0x77, 0x77});
}

void Passive09_0125(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FD0, 0x10, 8, 0x110, 0, -1, -1, -1});
}

void Passive09_0126(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive09_0127(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive09_0128(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FA0, 8, 0x100, 0, -0x7F78, -1, 0x40});
}

void Passive09_0129(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FA0, 8, 0x40, 0, -0x7F78, -1, 0x40});
}

void Passive09_0130(PLW* wk) {
    pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FA0, 8, 0x100, 0, -0x7F78, -1, 0x40},
        9,
        0x40
    );
}

void Passive09_0131(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FA0, 8, 0x40, 0, -0x7F78, -1, 0x40},
        0x200,
        &(Command_Attack_Args){0xC, 0x1D, 8, -1}
    );
}
