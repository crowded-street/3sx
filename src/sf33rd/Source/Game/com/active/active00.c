/**
 * @file active00.c
 * COM Active: Gill
 */

#include "sf33rd/Source/Game/com/active/active00.h"
#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern00_Tbl[68])();

void Computer00(PLW* wk) {
    Pattern00_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern00_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern00_0001(PLW* wk) {
    active_pattern_normal_attack_2(wk, 0x10, 0x10);
}

void Pattern00_0002(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern00_0003(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 9, 0x10, 0x20);
}

void Pattern00_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Pattern00_0005(PLW* wk) {
    active_pattern_approach_walk_lever_attack(wk, 0x3B, 0);
}

void Pattern00_0006(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Pattern00_0007(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Pattern00_0008(PLW* wk) {
    active_pattern_normal_attack_2(wk, 0x100, 0x400);
}

void Pattern00_0009(PLW* wk) {
    active_pattern_search_back_term_jump_look(wk, 0x60, 2, 0x10);
}

void Pattern00_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern00_0011(PLW* wk) {
    active_pattern_normal_attack_3(wk, 0x102, 8, 0x202);
}

void Pattern00_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Pattern00_0013(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Pattern00_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 8, 0x40, 2, -0x7F90, 8, 0x20});
}

void Pattern00_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern00_0016(PLW* wk) {
    active_pattern_search_back_term_walk(wk, 0x50, 2, 0);
}

void Pattern00_0017(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern00_0018(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Pattern00_0019(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 0, 6, 0x99);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern00_0020(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 8, 0x10, 0, 8, 8, 8});
}

void Pattern00_0021(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 8, 0x400, 0, -0x7F90, 8, 0x200});
}

void Pattern00_0022(PLW* wk) {
    active_pattern_pierce_on_search_back_term_command_attack(wk);
}

void Pattern00_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x83);
}

void Pattern00_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern00_0025(PLW* wk) {
    active_pattern_search_back_term_walk_2(wk, 0x70, 2, 0);
}

void Pattern00_0026(PLW* wk) {
    active_pattern_command_attack_look(wk);
}

void Pattern00_0027(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x20, 0, -0x7F90, 8, 0x20});
}

void Pattern00_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0xC3);
}

void Pattern00_0029(PLW* wk) {
    active_pattern_adjust_attack(wk, 0x100, 0x100, 0x200);
}

void Pattern00_0030(PLW* wk) {
    active_pattern_adjust_attack(wk, 0x10, 0x20, 0x40);
}

void Pattern00_0031(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 9, 0x20, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern00_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xB, 0x10);
        break;

    case 1:
        Lever_Attack(wk, 8, 1, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern00_0033(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 9, 0x10, 0x110);
}

void Pattern00_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 9, 0x100);
        break;

    case 1:
        ETC_Term(wk, 0, 6, 0x99);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern00_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern00_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern00_0037(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 1:
        Lever_Attack(wk, 8, 1, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern00_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xB, 0x20);
        break;

    case 1:
        Normal_Attack(wk, 0xA, 0x202);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern00_0039(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 9, 0x200, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern00_0040(PLW* wk) {
    active_pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 8, -1}, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern00_0041(PLW* wk) {
    active_pattern_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1}, 8, 0x402);
}

void Pattern00_0042(PLW* wk) {
    active_pattern_normal_attack_5(wk, 0x12, 0x12, 0x402);
}

void Pattern00_0043(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 8, 0x400, 0, -1, -1, -1});
}

void Pattern00_0044(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 8, 0x40, 0, -1, -1, -1});
}

void Pattern00_0045(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, -0x7FC0, 8, 0x400, 0, -1, -1, 0xFFFF});
}

void Pattern00_0046(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x39, 0x39, 0x41, 0x41}, 0);
}

void Pattern00_0047(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x39, 0x39, 0x41, 0x41}, 0);
}

void Pattern00_0048(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x39, 0x39, 0x41, 0x41}, 0);
}

void Pattern00_0049(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern00_0050(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern00_0051(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern00_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 0, 6, 0x9A);
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 0x31, 0x31, 0x31, 0x32}, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern00_0053(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x31, 0x31, 0x32, 0x33}, 0);
}

void Pattern00_0054(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x31, 0x32, 0x32, 0x33}, 0);
}

void Pattern00_0055(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x3B, 0x110);
}

void Pattern00_0056(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 2, 0x10);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, -0x7FB8, 8, 0x400, 1, -0x7F90, -1, 0x200});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern00_0057(PLW* wk) {
    active_pattern_check_sa_command_attack(wk, 2, 0x34, &(Command_Attack_Args){8, 0x8018, 0xA, -1});
}

void Pattern00_0058(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, 0);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern00_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Pattern00_0060(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x200);
}

void Pattern00_0061(PLW* wk) {
    active_pattern_normal_attack_3(wk, 0x12, 8, 0x22);
}

void Pattern00_0062(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x22);
}

void Pattern00_0063(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 1, 0x20);
}

void Pattern00_0064(PLW* wk) {
    active_pattern_wait(wk, 0xA);
}

void Pattern00_0065(PLW* wk) {
    active_pattern_check_sa_command_attack(wk, 2, 0x34, &(Command_Attack_Args){8, 0x8019, 0xA, -1});
}

void Pattern00_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 0, 6, 0x9A);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern00_0067(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 0, 6, 0x9A);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    case 2:
        ETC_Term(wk, 0, 6, 0x9A);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Pattern00_Tbl[68])(PLW*) = {
    Pattern00_0000, Pattern00_0001, Pattern00_0002, Pattern00_0003, Pattern00_0004, Pattern00_0005, Pattern00_0006,
    Pattern00_0007, Pattern00_0008, Pattern00_0009, Pattern00_0010, Pattern00_0011, Pattern00_0012, Pattern00_0013,
    Pattern00_0014, Pattern00_0015, Pattern00_0016, Pattern00_0017, Pattern00_0018, Pattern00_0019, Pattern00_0020,
    Pattern00_0021, Pattern00_0022, Pattern00_0023, Pattern00_0024, Pattern00_0025, Pattern00_0026, Pattern00_0027,
    Pattern00_0028, Pattern00_0029, Pattern00_0030, Pattern00_0031, Pattern00_0032, Pattern00_0033, Pattern00_0034,
    Pattern00_0035, Pattern00_0036, Pattern00_0037, Pattern00_0038, Pattern00_0039, Pattern00_0040, Pattern00_0041,
    Pattern00_0042, Pattern00_0043, Pattern00_0044, Pattern00_0045, Pattern00_0046, Pattern00_0047, Pattern00_0048,
    Pattern00_0049, Pattern00_0050, Pattern00_0051, Pattern00_0052, Pattern00_0053, Pattern00_0054, Pattern00_0055,
    Pattern00_0056, Pattern00_0057, Pattern00_0058, Pattern00_0059, Pattern00_0060, Pattern00_0061, Pattern00_0062,
    Pattern00_0063, Pattern00_0064, Pattern00_0065, Pattern00_0066, Pattern00_0067
};
