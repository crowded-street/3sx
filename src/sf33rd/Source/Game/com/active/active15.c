/**
 * @file active15.c
 * COM Active: Chun-Li
 */

#include "sf33rd/Source/Game/com/active/active15.h"
#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern15_Tbl[69])();

void Computer15(PLW* wk) {
    Pattern15_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern15_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern15_0001(PLW* wk) {
    active_pattern_normal_attack_4(wk, 0x10, 0x40);
}

void Pattern15_0002(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){2, 0x41, 0x31, 0x32, 0x33});
}

void Pattern15_0003(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 8, 0x10, 0x200);
}

void Pattern15_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern15_0005(PLW* wk) {
    active_pattern_approach_walk_com_random_select(wk, 0x37, &(Branch_Menu_Args){2, 0x3E, 0x3E, 0x3F, 0x3F});
}

void Pattern15_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern15_0007(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack(wk, 0x30, 2, 1);
}

void Pattern15_0008(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Pattern15_0009(PLW* wk) {
    active_pattern_search_back_term_jump_look(wk, 0x30, 6, 0x12);
}

void Pattern15_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern15_0011(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Pattern15_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Pattern15_0013(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern15_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x20, 2, -0x7FA0, -1, 0x20});
}

void Pattern15_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern15_0016(PLW* wk) {
    active_pattern_search_back_term_walk(wk, 0x30, 6, 0xF);
}

void Pattern15_0017(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern15_0018(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x202, 0});
}

void Pattern15_0019(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x400);
}

void Pattern15_0020(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    case 1:
        Jump_Attack(wk, &(Jump_Attack_Args){8, 0xC, 0x202, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern15_0021(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x200});
}

void Pattern15_0022(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xF, 0x40, 0});
}

void Pattern15_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Pattern15_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern15_0025(PLW* wk) {
    active_pattern_search_back_term_walk_2(wk, 0x30, 6, 0xF);
}

void Pattern15_0026(PLW* wk) {
    active_pattern_command_attack_look(wk);
}

void Pattern15_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern15_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0xBF);
}

void Pattern15_0029(PLW* wk) {
    active_pattern_adjust_attack_2(wk, 0x100, 0x200, 0x400);
}

void Pattern15_0030(PLW* wk) {
    active_pattern_adjust_attack_3(wk, 0x402);
}

void Pattern15_0031(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x200},
        0x200
    );
}

void Pattern15_0032(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 0xB, 0x10, &(Command_Attack_Args){8, 0x1E, 8, 0x70});
}

void Pattern15_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 8, 0x100);
        break;

    case 1:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){2, 1, 0x31, 0x32, 0x33});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern15_0034(PLW* wk) {
    active_pattern_adjust_attack_2(wk, 0x102, 0x102, 0x200);
}

void Pattern15_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern15_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern15_0037(PLW* wk) {
    active_pattern_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1}, 8, 0x40);
}

void Pattern15_0038(PLW* wk) {
    active_pattern_adjust_attack_normal_attack_lever_attack(wk);
}

void Pattern15_0039(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1E, 0xA, -1},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Pattern15_0040(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 9, 0x202, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern15_0041(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xB, 0x20, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern15_0042(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x12, 0x22, 0x402);
}

void Pattern15_0043(PLW* wk) {
    active_pattern_search_back_term_walk_wait(wk);
}

void Pattern15_0044(PLW* wk) {
    active_pattern_search_back_term_walk_wait_2(wk);
}

void Pattern15_0045(PLW* wk) {
    active_pattern_walk_search_back_term_walk(wk);
}

void Pattern15_0046(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern15_0047(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 0xA, -1});
}

void Pattern15_0048(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern15_0049(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Pattern15_0050(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Pattern15_0051(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Pattern15_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0});
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 0x3E, 0x3E, 0x3F, 0x3F}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern15_0053(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Pattern15_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 1:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern15_0055(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern15_0056(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, 0x700});
}

void Pattern15_0057(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern15_0058(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Pattern15_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Pattern15_0060(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, 0x70});
}

void Pattern15_0061(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 2, 2);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern15_0062(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x110);
}

void Pattern15_0063(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 1, 0x110);
}

void Pattern15_0064(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern15_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Rapid_Command_Attack(wk, &(Rapid_Command_Args){8, 0x4D, 0x100, 0x78});
        break;
    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern15_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Rapid_Command_Attack(wk, &(Rapid_Command_Args){8, 0x4D, 0x400, 0x78});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern15_0067(PLW* wk) {
    active_pattern_normal_attack_sp(wk);
}

void Pattern15_0068(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Rapid_Command_Attack(wk, &(Rapid_Command_Args){8, 0x4D, 0x100, 0x78});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Pattern15_Tbl[69])(PLW*) = {
    Pattern15_0000, Pattern15_0001, Pattern15_0002, Pattern15_0003, Pattern15_0004, Pattern15_0005, Pattern15_0006,
    Pattern15_0007, Pattern15_0008, Pattern15_0009, Pattern15_0010, Pattern15_0011, Pattern15_0012, Pattern15_0013,
    Pattern15_0014, Pattern15_0015, Pattern15_0016, Pattern15_0017, Pattern15_0018, Pattern15_0019, Pattern15_0020,
    Pattern15_0021, Pattern15_0022, Pattern15_0023, Pattern15_0024, Pattern15_0025, Pattern15_0026, Pattern15_0027,
    Pattern15_0028, Pattern15_0029, Pattern15_0030, Pattern15_0031, Pattern15_0032, Pattern15_0033, Pattern15_0034,
    Pattern15_0035, Pattern15_0036, Pattern15_0037, Pattern15_0038, Pattern15_0039, Pattern15_0040, Pattern15_0041,
    Pattern15_0042, Pattern15_0043, Pattern15_0044, Pattern15_0045, Pattern15_0046, Pattern15_0047, Pattern15_0048,
    Pattern15_0049, Pattern15_0050, Pattern15_0051, Pattern15_0052, Pattern15_0053, Pattern15_0054, Pattern15_0055,
    Pattern15_0056, Pattern15_0057, Pattern15_0058, Pattern15_0059, Pattern15_0060, Pattern15_0061, Pattern15_0062,
    Pattern15_0063, Pattern15_0064, Pattern15_0065, Pattern15_0066, Pattern15_0067, Pattern15_0068
};
