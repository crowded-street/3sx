/**
 * @file pass06_3.c
 * COM Passive: Hugo
 * Pattern scripts from Passive06_0141 on.
 */

#include "sf33rd/Source/Game/com/passive/pass06.h"
#include "sf33rd/Source/Game/com/passive/pass06_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive06_0141(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F88, -0x7FA8, 9, 0x400, 0, -0x7F68, -1, 0x400});
        break;

    case 1:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x16, 0x16, 0x17, 0x18});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0142(PLW* wk) {
    passive06_pattern_hi_jump_attack_term_normal_attack(wk, 0x42);
}

void Passive06_0143(PLW* wk) {
    passive06_pattern_hi_jump_attack_term_normal_attack(wk, 0x40);
}

void Passive06_0144(PLW* wk) {
    passive06_pattern_hi_jump_attack_term_normal_attack(wk, 0x20);
}

void Passive06_0145(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F00, 0x38, 8, 0x400, 1, -1, -1, -1});
}

void Passive06_0146(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F00, 0x38, 8, 0x200, 1, -1, -1, -1});
}
