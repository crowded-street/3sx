/**
 * @file shell04.c
 * COM Shell: Dudley
 */

#include "sf33rd/Source/Game/com/shell/shell04.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Shell04_Tbl[12])(PLW*);

void Shell04(PLW* wk) {
    Shell04_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Shell04_0000(PLW* wk) {
    active_pattern_(wk);
}

void Shell04_0001(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){1, 2, 1, -1, -1}, 2);
}

void Shell04_0002(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){0, 2, 1, -1, -1}, 0);
}

void Shell04_0003(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack_2(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Shell04_0004(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202
    );
}

void Shell04_0005(PLW* wk) {
    pattern_shell_term_jump_attack_term_j_command_attack(wk);
}

void Shell04_0006(PLW* wk) {
    pattern_shell_term_jump_attack_term(
        wk,
        &(Shell_Term_Params){1, 2, 1, -1, -1},
        &(Jump_Term_Args){-1, 0x30, 8, 0x400, 2, -1, -1, -1}
    );
}

void Shell04_0007(PLW* wk) {
    pattern_shell_term_command_attack(wk, &(Command_Attack_Args){0xD, 0x20, 0x30A, -1});
}

void Shell04_0008(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x20, 0, -0x7FB0, -1, 0x200},
        0x202
    );
}

void Shell04_0009(PLW* wk) {
    pattern_lever_off_shell_term_next_be_flip(wk);
}

void Shell04_0010(PLW* wk) {
    active_pattern_(wk);
}

void Shell04_0011(PLW* wk) {
    pattern_shell_term_command_attack(wk, &(Command_Attack_Args){0xD, 0x20, 0x60A, -1});
}

void (*const Shell04_Tbl[12])(PLW*) = { Shell04_0000, Shell04_0001, Shell04_0002, Shell04_0003,
                                        Shell04_0004, Shell04_0005, Shell04_0006, Shell04_0007,
                                        Shell04_0008, Shell04_0009, Shell04_0010, Shell04_0011 };
