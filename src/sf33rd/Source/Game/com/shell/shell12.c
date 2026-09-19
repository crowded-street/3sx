/**
 * @file shell12.c
 * COM Shell: Sean
 */

#include "sf33rd/Source/Game/com/shell/shell12.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Shell12_Tbl[11])(PLW*);

void Shell12(PLW* wk) {
    Shell12_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Shell12_0000(PLW* wk) {
    active_pattern_(wk);
}

void Shell12_0001(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){1, 2, 1, -1, -1}, 2);
}

void Shell12_0002(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){0, 2, 1, -1, -1}, 0);
}

void Shell12_0003(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack_2(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Shell12_0004(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202
    );
}

void Shell12_0005(PLW* wk) {
    pattern_shell_term_jump_attack_term_j_command_attack(wk);
}

void Shell12_0006(PLW* wk) {
    pattern_shell_term_jump_attack_term(
        wk,
        &(Shell_Term_Params){1, 2, 1, -1, -1},
        &(Jump_Term_Args){-1, 0x30, 8, 0x400, 2, -1, -1, -1}
    );
}

void Shell12_0007(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, &(Shell_Term_Params){3, 2, 1, -1, -1});
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Shell12_0008(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x20, 0, -0x7FB0, -1, 0x200},
        0x202
    );
}

void Shell12_0009(PLW* wk) {
    pattern_lever_off_shell_term_next_be_flip(wk);
}

void Shell12_0010(PLW* wk) {
    active_pattern_(wk);
}

void (*const Shell12_Tbl[11])(PLW*) = { Shell12_0000, Shell12_0001, Shell12_0002, Shell12_0003,
                                        Shell12_0004, Shell12_0005, Shell12_0006, Shell12_0007,
                                        Shell12_0008, Shell12_0009, Shell12_0010 };
