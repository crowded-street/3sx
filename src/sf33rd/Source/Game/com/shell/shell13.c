/**
 * @file shell13.c
 * COM Shell: Urien
 */

#include "sf33rd/Source/Game/com/shell/shell13.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Shell13_Tbl[12])(PLW*);

void Shell13(PLW* wk) {
    Shell13_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Shell13_0000(PLW* wk) {
    active_pattern_(wk);
}

void Shell13_0001(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){1, 2, 1, -1, -1}, 2);
}

void Shell13_0002(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){0, 2, 1, -1, -1}, 0);
}

void Shell13_0003(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack_3(
        wk,
        &(Jump_Term_Args){-0x7F78, -0x7FD0, 9, 0x200, 0, -0x7FB0, -1, 0x200},
        0x40
    );
}

void Shell13_0004(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F78, -0x7FD0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202
    );
}

void Shell13_0005(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F78, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x400},
        0x40
    );
}

void Shell13_0006(PLW* wk) {
    pattern_shell_term_jump_attack_term(
        wk,
        &(Shell_Term_Params){1, 2, 1, -1, -1},
        &(Jump_Term_Args){-1, 0x30, 8, 0x400, 2, -1, -1, -1}
    );
}

void Shell13_0007(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Shell13_0008(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F78, -0x7FD0, 0xB, 0x20, 0, -0x7FB0, -1, 0x200},
        0x202
    );
}

void Shell13_0009(PLW* wk) {
    pattern_lever_off_shell_term_next_be_flip(wk);
}

void Shell13_0010(PLW* wk) {
    active_pattern_(wk);
}

void Shell13_0011(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, 0x70});
}

void (*const Shell13_Tbl[12])(PLW*) = { Shell13_0000, Shell13_0001, Shell13_0002, Shell13_0003,
                                        Shell13_0004, Shell13_0005, Shell13_0006, Shell13_0007,
                                        Shell13_0008, Shell13_0009, Shell13_0010, Shell13_0011 };
