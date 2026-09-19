/**
 * @file active19.c
 * COM Active: Remy
 */

#include "sf33rd/Source/Game/com/active/active19.h"
#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern19_Tbl[79])();

void Computer19(PLW* wk) {
    Pattern19_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern19_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern19_0001(PLW* wk) {
    active_pattern_normal_attack_4(wk, 0x10, 0x10);
}

void Pattern19_0002(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Pattern19_0003(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 8, 0x10, 0x200);
}

void Pattern19_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Pattern19_0005(PLW* wk) {
    active_pattern_approach_walk_com_random_select(wk, 0x37, &(Branch_Menu_Args){2, 0x3E, 0x3F, 0x40, 0x40});
}

void Pattern19_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern19_0007(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack(wk, 0x30, 2, 1);
}

void Pattern19_0008(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Pattern19_0009(PLW* wk) {
    active_pattern_search_back_term_jump_look(wk, 0x60, 6, 0x10);
}

void Pattern19_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern19_0011(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Pattern19_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Pattern19_0013(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x200);
}

void Pattern19_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x20, 2, -0x7FA0, -1, 0x20});
}

void Pattern19_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern19_0016(PLW* wk) {
    active_pattern_walk(wk, 1, 0x30, 0);
}

void Pattern19_0017(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Pattern19_0018(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x400, 2});
}

void Pattern19_0019(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x400);
}

void Pattern19_0020(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 8, 0x402, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern19_0021(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x200});
}

void Pattern19_0022(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xF, 0x40, 0});
}

void Pattern19_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Pattern19_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern19_0025(PLW* wk) {
    active_pattern_walk(wk, 1, 0x60, 0);
}

void Pattern19_0026(PLW* wk) {
    active_pattern_command_attack_look(wk);
}

void Pattern19_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern19_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0xBF);
}

void Pattern19_0029(PLW* wk) {
    active_pattern_adjust_attack_2(wk, 0x100, 0x200, 0x400);
}

void Pattern19_0030(PLW* wk) {
    active_pattern_adjust_attack_3(wk, 0x402);
}

void Pattern19_0031(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x200},
        0x200
    );
}

void Pattern19_0032(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1D, 0xA, -1},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Pattern19_0033(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 8, 0x10, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Pattern19_0034(PLW* wk) {
    active_pattern_adjust_attack_2(wk, 0x102, 0x102, 0x200);
}

void Pattern19_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern19_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern19_0037(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 0xB, 0x10, 0x400);
}

void Pattern19_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xB, 0x42);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x400);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern19_0039(PLW* wk) {
    active_pattern_adjust_attack_2(wk, 0x10, 0x20, 0x40);
}

void Pattern19_0040(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 9, 0x202, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Pattern19_0041(PLW* wk) {
    active_pattern_normal_attack_2(wk, 0x40, 0x40);
}

void Pattern19_0042(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x12, 0x12, 0x402);
}

void Pattern19_0043(PLW* wk) {
    active_pattern_search_back_term_walk_wait(wk);
}

void Pattern19_0044(PLW* wk) {
    active_pattern_search_back_term_walk_wait_2(wk);
}

void Pattern19_0045(PLW* wk) {
    active_pattern_walk_search_back_term_walk(wk);
}

void Pattern19_0046(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern19_0047(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern19_0048(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 0xA, -1});
}

void Pattern19_0049(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Pattern19_0050(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Pattern19_0051(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Pattern19_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0x30, 0});
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 0x3E, 0x3F, 0x40, 0x40}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern19_0053(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0},
        &(Command_Attack_Args){8, 0x1F, 8, -1}
    );
}

void Pattern19_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x93, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern19_0055(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern19_0056(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern19_0057(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
        break;
    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern19_0058(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern19_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, 0x700});
}

void Pattern19_0060(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, 0x70});
}

void Pattern19_0061(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, 0x700});
}

void Pattern19_0062(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x110);
}

void Pattern19_0063(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 1, 0x110);
}

void Pattern19_0064(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x110);
}

void Pattern19_0065(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x31, 0x32, 0x33, 0x44}, 1);
}

void Pattern19_0066(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x38, 0x39, 0x3A, 0x45}, 1);
}

void Pattern19_0067(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern19_0068(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Pattern19_0069(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Pattern19_0070(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Pattern19_0071(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Pattern19_0072(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){0xC, 0x1D, 8, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Pattern19_0073(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0x1E, 8, -1},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Pattern19_0074(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0x1D, 0xA, -1},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Pattern19_0075(PLW* wk) {
    active_pattern_normal_attack_command_attack_3(wk, 0x102, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Pattern19_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, -0x7FA0, 8, 0x402, 0, -0x7FA0, -1, 0x200});
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x102);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern19_0077(PLW* wk) {
    active_pattern_normal_attack_command_attack_3(wk, 0x202, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern19_0078(PLW* wk) {
    active_pattern_normal_attack_command_attack_3(wk, 0x102, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void (*const Pattern19_Tbl[79])(PLW*) = {
    Pattern19_0000, Pattern19_0001, Pattern19_0002, Pattern19_0003, Pattern19_0004, Pattern19_0005, Pattern19_0006,
    Pattern19_0007, Pattern19_0008, Pattern19_0009, Pattern19_0010, Pattern19_0011, Pattern19_0012, Pattern19_0013,
    Pattern19_0014, Pattern19_0015, Pattern19_0016, Pattern19_0017, Pattern19_0018, Pattern19_0019, Pattern19_0020,
    Pattern19_0021, Pattern19_0022, Pattern19_0023, Pattern19_0024, Pattern19_0025, Pattern19_0026, Pattern19_0027,
    Pattern19_0028, Pattern19_0029, Pattern19_0030, Pattern19_0031, Pattern19_0032, Pattern19_0033, Pattern19_0034,
    Pattern19_0035, Pattern19_0036, Pattern19_0037, Pattern19_0038, Pattern19_0039, Pattern19_0040, Pattern19_0041,
    Pattern19_0042, Pattern19_0043, Pattern19_0044, Pattern19_0045, Pattern19_0046, Pattern19_0047, Pattern19_0048,
    Pattern19_0049, Pattern19_0050, Pattern19_0051, Pattern19_0052, Pattern19_0053, Pattern19_0054, Pattern19_0055,
    Pattern19_0056, Pattern19_0057, Pattern19_0058, Pattern19_0059, Pattern19_0060, Pattern19_0061, Pattern19_0062,
    Pattern19_0063, Pattern19_0064, Pattern19_0065, Pattern19_0066, Pattern19_0067, Pattern19_0068, Pattern19_0069,
    Pattern19_0070, Pattern19_0071, Pattern19_0072, Pattern19_0073, Pattern19_0074, Pattern19_0075, Pattern19_0076,
    Pattern19_0077, Pattern19_0078
};
