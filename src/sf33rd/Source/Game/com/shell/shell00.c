/**
 * @file shell00.c
 * COM Shell: Gill
 */

#include "sf33rd/Source/Game/com/shell/shell00.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Shell00_Tbl[11])(PLW*);

void Shell00(PLW* wk) {
    Shell00_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Shell00_0000(PLW* wk) {
    pattern_end_immediately(wk);
}

void Shell00_0001(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){1, 2, 1, -1, -1}, 2);
}

void Shell00_0002(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){0, 2, 1, -1, -1}, 0);
}

void Shell00_0003(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Shell00_0004(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F78, -0x7FD0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202
    );
}

void Shell00_0005(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F78, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x400},
        0x40
    );
}

void Shell00_0006(PLW* wk) {
    pattern_shell_term_jump_attack_term(
        wk,
        &(Shell_Term_Params){1, 2, 1, -1, -1},
        &(Jump_Term_Args){-1, 0x30, 8, 0x400, 2, -1, -1, -1}
    );
}

void Shell00_0007(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Shell00_0008(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F78, -0x7FD0, 0xB, 0x20, 0, -0x7FB0, -1, 0x200},
        0x202
    );
}

void Shell00_0009(PLW* wk) {
    pattern_lever_off_shell_term_next_be_flip(wk);
}

void Shell00_0010(PLW* wk) {
    pattern_end_immediately(wk);
}

void (*const Shell00_Tbl[11])(PLW*) = { Shell00_0000, Shell00_0001, Shell00_0002, Shell00_0003,
                                        Shell00_0004, Shell00_0005, Shell00_0006, Shell00_0007,
                                        Shell00_0008, Shell00_0009, Shell00_0010 };
