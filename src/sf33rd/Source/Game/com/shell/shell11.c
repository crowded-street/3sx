/**
 * @file shell11.c
 * COM Shell: Ryu, Ken, Chun-Li, Makoto, Q, Twelve, Remy
 */

#include "sf33rd/Source/Game/com/shell/shell11.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Shell11_Tbl[14])(PLW*);

void Shell11(PLW* wk) {
    Shell11_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Shell11_0000(PLW* wk) {
    active_pattern_(wk);
}

void Shell11_0001(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){1, 2, 1, -1, -1}, 2);
}

void Shell11_0002(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){0, 2, 1, -1, -1}, 0);
}

void Shell11_0003(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack_3(
        wk,
        &(Jump_Term_Args){-0x7F20, -0x7FD0, 9, 0x400, 0, -0x7FB0, -1, 0x200},
        0x202
    );
}

void Shell11_0004(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F20, -0x7FD0, 0xB, 0x400, 0, -0x7FB0, -1, 0x20},
        0x202
    );
}

void Shell11_0005(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F20, -0x7FC0, 0xB, 0x400, 0, -0x7FB0, -1, 0x400},
        0x202
    );
}

void Shell11_0006(PLW* wk) {
    pattern_shell_term_jump_attack_term(
        wk,
        &(Shell_Term_Params){1, 2, 1, -1, -1},
        &(Jump_Term_Args){-1, 0x30, 8, 0x400, 2, -1, -1, -1}
    );
}

void Shell11_0007(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Shell11_0008(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x20, 0, -0x7FB0, -1, 0x200},
        0x202
    );
}

void Shell11_0009(PLW* wk) {
    pattern_lever_off_shell_term_next_be_flip(wk);
}

void Shell11_0010(PLW* wk) {
    active_pattern_(wk);
}

void Shell11_0011(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, 0x70});
}

void Shell11_0012(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x1E, 0xA, -1, -1, 0x30, 0, -1, -1, 0xFFFF});
}

void Shell11_0013(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x1E, 0xA, 0x700, -1, 0x30, 0, -1, -1, 0xFFFF});
}

void (*const Shell11_Tbl[14])(PLW*) = { Shell11_0000, Shell11_0001, Shell11_0002, Shell11_0003, Shell11_0004,
                                        Shell11_0005, Shell11_0006, Shell11_0007, Shell11_0008, Shell11_0009,
                                        Shell11_0010, Shell11_0011, Shell11_0012, Shell11_0013 };
