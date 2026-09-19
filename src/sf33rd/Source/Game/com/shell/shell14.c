/**
 * @file shell14.c
 * COM Shell: Akuma/Gouki
 */

#include "sf33rd/Source/Game/com/shell/shell14.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Shell14_Tbl[13])(PLW*);

void Shell14(PLW* wk) {
    Shell14_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Shell14_0000(PLW* wk) {
    active_pattern_(wk);
}

void Shell14_0001(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){1, 2, 1, -1, -1}, 2);
}

void Shell14_0002(PLW* wk) {
    pattern_shell_term_jump(wk, &(Shell_Term_Params){0, 2, 1, -1, -1}, 0);
}

void Shell14_0003(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack_3(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 9, 0x200, 0, -0x7FB0, -1, 0x200},
        0x202
    );
}

void Shell14_0004(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202
    );
}

void Shell14_0005(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x400},
        0x202
    );
}

void Shell14_0006(PLW* wk) {
    pattern_shell_term_jump_attack_term(
        wk,
        &(Shell_Term_Params){1, 2, 1, -1, -1},
        &(Jump_Term_Args){-1, 0x30, 8, 0x400, 2, -1, -1, -1}
    );
}

void Shell14_0007(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Shell14_0008(PLW* wk) {
    pattern_shell_term_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x20, 0, -0x7FB0, -1, 0x200},
        0x202
    );
}

void Shell14_0009(PLW* wk) {
    pattern_lever_off_shell_term_next_be_flip(wk);
}

void Shell14_0010(PLW* wk) {
    active_pattern_(wk);
}

void Shell14_0011(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x20, 0xA, -1, -1, 0x30, 0, -1, -1, 0xFFFF});
}

void Shell14_0012(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x20, 0xA, 0x700, -1, 0x30, 0, -1, -1, 0xFFFF});
}

void (*const Shell14_Tbl[13])(PLW*) = { Shell14_0000, Shell14_0001, Shell14_0002, Shell14_0003, Shell14_0004,
                                        Shell14_0005, Shell14_0006, Shell14_0007, Shell14_0008, Shell14_0009,
                                        Shell14_0010, Shell14_0011, Shell14_0012 };
