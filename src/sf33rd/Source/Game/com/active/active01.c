/**
 * @file active01.c
 * COM Active: Alex
 */

#include "sf33rd/Source/Game/com/active/active01.h"
#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern01_Tbl[70])();

void Computer01(PLW* wk) {
    Pattern01_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern01_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern01_0001(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;
    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;
    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern01_0002(PLW* wk) {
    active_pattern_approach_walk_command_attack(wk, 0x45, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Pattern01_0003(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 8, 0x10, 0x200);
}

void Pattern01_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern01_0005(PLW* wk) {
    active_pattern_approach_walk_com_random_select(wk, 0x3B, &(Branch_Menu_Args){2, 0x3E, 0x3E, 0x3F, 0x3F});
}

void Pattern01_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern01_0007(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack(wk, 0x30, 2, 1);
}

void Pattern01_0008(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Pattern01_0009(PLW* wk) {
    active_pattern_search_back_term_jump_look(wk, 0x60, 6, 0x10);
}

void Pattern01_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern01_0011(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Pattern01_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Pattern01_0013(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x20);
}

void Pattern01_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x20, 2, -0x7FA0, -1, 0x20});
}

void Pattern01_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern01_0016(PLW* wk) {
    active_pattern_walk(wk, 1, 0x30, 0);
}

void Pattern01_0017(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern01_0018(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x42, 2});
}

void Pattern01_0019(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x400);
}

void Pattern01_0020(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    case 1:
        Jump_Attack(wk, &(Jump_Attack_Args){8, 0xA, 0x42, 2});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern01_0021(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x200});
}

void Pattern01_0022(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xF, 0x40, 0});
}

void Pattern01_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x83);
}

void Pattern01_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern01_0025(PLW* wk) {
    active_pattern_walk(wk, 1, 0x60, 0);
}

void Pattern01_0026(PLW* wk) {
    active_pattern_command_attack_look(wk);
}

void Pattern01_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern01_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0xC3);
}

void Pattern01_0029(PLW* wk) {
    active_pattern_adjust_attack_2(wk, 0x100, 0x200, 0x400);
}

void Pattern01_0030(PLW* wk) {
    active_pattern_adjust_attack_3(wk, 0x402);
}

void Pattern01_0031(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x200},
        0x200
    );
}

void Pattern01_0032(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 0xB, 0x10, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern01_0033(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 8, 0x10, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Pattern01_0034(PLW* wk) {
    active_pattern_adjust_attack_2(wk, 0x102, 0x102, 0x200);
}

void Pattern01_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern01_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern01_0037(PLW* wk) {
    active_pattern_command_attack_lever_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1}, 1, 0x40);
}

void Pattern01_0038(PLW* wk) {
    active_pattern_adjust_attack_normal_attack_lever_attack(wk);
}

void Pattern01_0039(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1E, 0xA, -1},
        &(Command_Attack_Args){8, 0x1D, 8, -1}
    );
}

void Pattern01_0040(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 9, 0x202, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern01_0041(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xB, 0x40, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern01_0042(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x12, 0x12, 0x402);
}

void Pattern01_0043(PLW* wk) {
    active_pattern_search_back_term_walk_wait(wk);
}

void Pattern01_0044(PLW* wk) {
    active_pattern_search_back_term_walk_wait_2(wk);
}

void Pattern01_0045(PLW* wk) {
    active_pattern_walk_search_back_term_walk(wk);
}

void Pattern01_0046(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern01_0047(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 0xA, -1});
}

void Pattern01_0048(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){2, 0x31, 0x31, 0x32, 0x33});
}

void Pattern01_0049(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 8, -1});
}

void Pattern01_0050(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 9, -1});
}

void Pattern01_0051(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern01_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x45, 2);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0x45});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern01_0053(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x45, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0x45});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern01_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0xC3, 2);
        break;

    case 2:
        Jump_Attack(wk, &(Jump_Attack_Args){8, 0xC, 0x42, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern01_0055(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern01_0056(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Pattern01_0057(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Pattern01_0058(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 0xA, -1});
}

void Pattern01_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 9, 0x700});
}

void Pattern01_0060(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, 0x70});
}

void Pattern01_0061(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, 0x700});
}

void Pattern01_0062(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x110);
}

void Pattern01_0063(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 1, 0x110);
}

void Pattern01_0064(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x21, 8, -1});
}

void Pattern01_0065(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x21, 9, -1});
}

void Pattern01_0066(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x21, 0xA, -1});
}

void Pattern01_0067(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x40);
}

void Pattern01_0068(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){2, 0x40, 0x41, 0x42, 1});
}

void Pattern01_0069(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, -0x7FC8, 0xC, 0x40, 0, -0x7fa0, -1, 0x200});
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Pattern01_Tbl[70])(PLW*) = {
    Pattern01_0000, Pattern01_0001, Pattern01_0002, Pattern01_0003, Pattern01_0004, Pattern01_0005, Pattern01_0006,
    Pattern01_0007, Pattern01_0008, Pattern01_0009, Pattern01_0010, Pattern01_0011, Pattern01_0012, Pattern01_0013,
    Pattern01_0014, Pattern01_0015, Pattern01_0016, Pattern01_0017, Pattern01_0018, Pattern01_0019, Pattern01_0020,
    Pattern01_0021, Pattern01_0022, Pattern01_0023, Pattern01_0024, Pattern01_0025, Pattern01_0026, Pattern01_0027,
    Pattern01_0028, Pattern01_0029, Pattern01_0030, Pattern01_0031, Pattern01_0032, Pattern01_0033, Pattern01_0034,
    Pattern01_0035, Pattern01_0036, Pattern01_0037, Pattern01_0038, Pattern01_0039, Pattern01_0040, Pattern01_0041,
    Pattern01_0042, Pattern01_0043, Pattern01_0044, Pattern01_0045, Pattern01_0046, Pattern01_0047, Pattern01_0048,
    Pattern01_0049, Pattern01_0050, Pattern01_0051, Pattern01_0052, Pattern01_0053, Pattern01_0054, Pattern01_0055,
    Pattern01_0056, Pattern01_0057, Pattern01_0058, Pattern01_0059, Pattern01_0060, Pattern01_0061, Pattern01_0062,
    Pattern01_0063, Pattern01_0064, Pattern01_0065, Pattern01_0066, Pattern01_0067, Pattern01_0068, Pattern01_0069
};
