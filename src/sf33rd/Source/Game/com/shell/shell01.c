/**
 * @file shell01.c
 * COM Shell: Alex
 */

#include "sf33rd/Source/Game/com/shell/shell01.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Shell01_Tbl[10])(PLW*);

void Shell01(PLW* wk) {
    Shell01_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Shell01_0000(PLW* wk) {
    active_pattern_(wk);
}

void Shell01_0001(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){1, 2, 1, -1, -1}, 2);
}

void Shell01_0002(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){0, 2, 1, -1, -1}, 0);
}

void Shell01_0003(PLW* wk) {
    pattern_shell_term_jump_attack_term(
        wk,
        &(Shell_Term_Params){0, 2, 1, -1, -1},
        &(Jump_Term_Args){-0x7FA0, 0x30, 8, 0x400, 0, -1, -1, -1}
    );
}

void Shell01_0004(PLW* wk) {
    pattern_shell_term_jump_attack_term(
        wk,
        &(Shell_Term_Params){0, 2, 1, -1, -1},
        &(Jump_Term_Args){-1, 0x30, 8, 0x40, 0, -1, -1, -1}
    );
}

void Shell01_0005(PLW* wk) {
    pattern_shell_term_jump_attack_term(
        wk,
        &(Shell_Term_Params){0, 2, 1, -1, -1},
        &(Jump_Term_Args){-1, 0x30, 8, 0x400, 2, -1, -1, -1}
    );
}

void Shell01_0006(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, 0x30, 8, 0x200, 0, -1, -1, -1},
        0x202
    );
}

void Shell01_0007(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(wk, &(Jump_Term_Args){-1, 0x30, 8, 0x40, 0, -1, -1, -1}, 0x22);
}

void Shell01_0008(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-1, 0x30, 0xB, 0x200, 0, -1, -1, -1},
        0x202
    );
}

void Shell01_0009(PLW* wk) {
    pattern_lever_off_shell_term_next_be_flip(wk);
}

void (*const Shell01_Tbl[10])(PLW*) = { Shell01_0000, Shell01_0001, Shell01_0002, Shell01_0003, Shell01_0004,
                                        Shell01_0005, Shell01_0006, Shell01_0007, Shell01_0008, Shell01_0009 };
