/**
 * @file pass18_4.c
 * COM Passive: Twelve
 * Pattern scripts from Passive18_0201 on.
 */

#include "sf33rd/Source/Game/com/passive/pass18.h"
#include "sf33rd/Source/Game/com/passive/pass18_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive18_0201(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0xC3, 0xB1, 0xA7, 0xA7}, 4);
}

void Passive18_0202(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0xC9);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x4A, 0xC4, 0x68, 0x84}, 4);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0203(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F00, -0x7FA0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}
