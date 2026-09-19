/**
 * @file shell07.c
 * COM Shell: Ibuki
 */

#include "sf33rd/Source/Game/com/shell/shell07.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Shell07_Tbl[12])(PLW*);

void Shell07(PLW* wk) {
    Shell07_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Shell07_0000(PLW* wk) {
    active_pattern_(wk);
}

void Shell07_0001(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){1, 2, 1, -1, -1}, 2);
}

void Shell07_0002(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){0, 2, 1, -1, -1}, 0);
}

void Shell07_0003(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack_2(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Shell07_0004(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202
    );
}

void Shell07_0005(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x400},
        0x42
    );
}

void Shell07_0006(PLW* wk) {
    pattern_shell_term_jump_attack_term(
        wk,
        &(Shell_Term_Params){1, 2, 1, -1, -1},
        &(Jump_Term_Args){-1, 0x30, 8, 0x400, 2, -1, -1, -1}
    );
}

void Shell07_0007(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Shell07_0008(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x20, 0, -0x7FB0, -1, 0x200},
        0x202
    );
}

void Shell07_0009(PLW* wk) {
    pattern_lever_off_shell_term_next_be_flip(wk);
}

void Shell07_0010(PLW* wk) {
    active_pattern_(wk);
}

void Shell07_0011(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 9, -1, -1, 0x40, 0, -1, -1, 0xFFFF});
}

void (*const Shell07_Tbl[12])(PLW*) = { Shell07_0000, Shell07_0001, Shell07_0002, Shell07_0003,
                                        Shell07_0004, Shell07_0005, Shell07_0006, Shell07_0007,
                                        Shell07_0008, Shell07_0009, Shell07_0010, Shell07_0011 };
