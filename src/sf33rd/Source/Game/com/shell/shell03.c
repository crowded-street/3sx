/**
 * @file shell03.c
 * COM Shell: Yun, Hugo, Elena, Oro, Yang
 */

#include "sf33rd/Source/Game/com/shell/shell03.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Shell03_Tbl[11])(PLW*);

void Shell03(PLW* wk) {
    Shell03_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Shell03_0000(PLW* wk) {
    active_pattern_(wk);
}

void Shell03_0001(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){1, 2, 1, -1, -1}, 2);
}

void Shell03_0002(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){0, 2, 1, -1, -1}, 0);
}

void Shell03_0003(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack_2(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Shell03_0004(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202
    );
}

void Shell03_0005(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x400},
        0x42
    );
}

void Shell03_0006(PLW* wk) {
    pattern_shell_term_jump_attack_term(
        wk,
        &(Shell_Term_Params){1, 2, 1, -1, -1},
        &(Jump_Term_Args){-1, 0x30, 8, 0x400, 2, -1, -1, -1}
    );
}

void Shell03_0007(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Shell03_0008(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x20, 0, -0x7FB0, -1, 0x200},
        0x202
    );
}

void Shell03_0009(PLW* wk) {
    pattern_lever_off_shell_term_next_be_flip(wk);
}

void Shell03_0010(PLW* wk) {
    active_pattern_(wk);
}

void (*const Shell03_Tbl[11])(PLW*) = { Shell03_0000, Shell03_0001, Shell03_0002, Shell03_0003,
                                        Shell03_0004, Shell03_0005, Shell03_0006, Shell03_0007,
                                        Shell03_0008, Shell03_0009, Shell03_0010 };
