/**
 * @file active10.c
 * COM Active: Yang
 */

#include "sf33rd/Source/Game/com/active/active10.h"
#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern10_Tbl[70])();

static void pattern10_0041_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Computer10(PLW* wk) {
    Pattern10_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern10_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern10_0001(PLW* wk) {
    active_pattern_normal_attack_4(wk, 0x10, 0x10);
}

void Pattern10_0002(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){2, 0x31, 0x32, 0x33, 1});
}

void Pattern10_0003(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 9, 0x10, 0x20);
}

void Pattern10_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 0xA, -1});
}

void Pattern10_0005(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x110);
}

void Pattern10_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern10_0007(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack(wk, 0x70, 2, 1);
}

void Pattern10_0008(PLW* wk) {
    active_pattern_normal_attack_3(wk, 0x100, 8, 0x400);
}

void Pattern10_0009(PLW* wk) {
    active_pattern_search_back_term_jump_look(wk, 0x70, 2, 0x10);
}

void Pattern10_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern10_0011(PLW* wk) {
    active_pattern_normal_attack_3(wk, 0x102, 8, 0x202);
}

void Pattern10_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Pattern10_0013(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x200);
}

void Pattern10_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x20, 2, -0x7FA0, 8, 0x20});
}

void Pattern10_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern10_0016(PLW* wk) {
    active_pattern_search_back_term_walk(wk, 0x70, 2, 0);
}

void Pattern10_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 8, -1});
        break;

    case 2:
        Wait(wk, 0xe);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 8, -1});
        break;

    case 4:
        Wait(wk, 0xe);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern10_0018(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x8400, 2});
}

void Pattern10_0019(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 9, -1});
        break;

    case 2:
        Wait(wk, 0xe);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 9, -1});
        break;

    case 4:
        Wait(wk, 0xe);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern10_0020(PLW* wk) {
    active_pattern_lever_attack_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern10_0021(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -0x7FA0, 8, 0x200});
}

void Pattern10_0022(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FB0, -1, 8, 0x8100, 0, -0x7FA0, 8, 0x200});
}

void Pattern10_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x83);
}

void Pattern10_0024(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x20, 2, -0x7FA0, 8, 0x400});
}

void Pattern10_0025(PLW* wk) {
    active_pattern_search_back_term_walk_2(wk, 0x70, 2, 0);
}

void Pattern10_0026(PLW* wk) {
    active_pattern_command_attack_look(wk);
}

void Pattern10_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern10_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0xC3);
}

void Pattern10_0029(PLW* wk) {
    active_pattern_adjust_attack_lever_attack(wk);
}

void Pattern10_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 9, 0x10);
        break;

    case 1:
        Adjust_Attack(wk, 0xC, 0x20);
        break;

    case 2:
        Adjust_Attack(wk, 8, 0x40);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern10_0031(PLW* wk) {
    active_pattern_normal_attack_branch_unit_area(wk, 0xC, 0x20, &(Branch_Menu_Args){2, 0x31, 0x32, 0x33, 0x33});
}

void Pattern10_0032(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 0xB, 0x10, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern10_0033(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 9, 0x10, 0x110);
}

void Pattern10_0034(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 9, 0x102, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Pattern10_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern10_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern10_0037(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x40, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern10_0038(PLW* wk) {
    active_pattern_adjust_attack_normal_attack_branch_unit_area(wk);
}

void Pattern10_0039(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
        break;

    case 1:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){2, 0x31, 0x32, 0x33, 1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern10_0040(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 9, 0x202, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern10_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xb, 0x42);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 8, -1});
        break;

    case 3:
        Wait(wk, 0xe);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){0xe, 0x1e, 8, -1});
        break;

    case 5:
        Wait(wk, 0xe);
        break;

    default:
        pattern10_0041_from_step_6(wk);
        break;
    }
}

void Pattern10_0042(PLW* wk) {
    active_pattern_normal_attack_8(wk, 0x12, 0x12, 0x402);
}

void Pattern10_0043(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, 8, 8, 0x8400, 0, -1, -0x7FA0, 0x400});
}

void Pattern10_0044(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x20, 0, -0x7FA0, 8, 0x400});
}

void Pattern10_0045(PLW* wk) {
    active_pattern_walk_2(wk, 0x14, 0x16, 0x18);
}

void Pattern10_0046(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 0xA, -1});
}

void Pattern10_0047(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern10_0048(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern10_0049(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Pattern10_0050(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Pattern10_0051(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Pattern10_0052(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x20, 0xA, -1}
    );
}

void Pattern10_0053(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0x8040},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Pattern10_0054(PLW* wk) {
    active_pattern_sa_term_hi_jump_attack_term(wk);
}

void Pattern10_0055(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -0x7FA0, 8, 0x20});
}

void Pattern10_0056(PLW* wk) {
    active_pattern_command_attack_com_random_select(
        wk,
        &(Command_Attack_Args){8, 0x1C, 0xA, -1},
        &(Branch_Menu_Args){2, 0x31, 4, 5, 1},
        0
    );
}

void Pattern10_0057(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 6, 0x12);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x20, 1, -0x7FA0, 8, 0x200});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern10_0058(PLW* wk) {
    active_pattern_search_back_term_hi_jump_attack_term_pierce_on(wk);
}

void Pattern10_0059(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack_2(wk);
}

void Pattern10_0060(PLW* wk) {
    active_pattern_search_back_term_jump_attack_term_pierce_on(wk);
}

void Pattern10_0061(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack_3(wk);
}

void Pattern10_0062(PLW* wk) {
    active_pattern_hi_jump_attack_term_com_random_select(wk);
}

void Pattern10_0063(PLW* wk) {
    active_pattern_lever_attack_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Pattern10_0064(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x37, 0x37, 0x27, 0x27}, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern10_0065(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xB, 0x200, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern10_0066(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x2D, 0x19, 0x10, 0x17}, 0);
}

void Pattern10_0067(PLW* wk) {
    active_pattern_normal_attack(wk, 0xD, 0x100);
}

void Pattern10_0068(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Pattern10_0069(PLW* wk) {
    active_pattern_command_attack_lever_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1}, 0, 0x20);
}

void (*const Pattern10_Tbl[70])(PLW*) = {
    Pattern10_0000, Pattern10_0001, Pattern10_0002, Pattern10_0003, Pattern10_0004, Pattern10_0005, Pattern10_0006,
    Pattern10_0007, Pattern10_0008, Pattern10_0009, Pattern10_0010, Pattern10_0011, Pattern10_0012, Pattern10_0013,
    Pattern10_0014, Pattern10_0015, Pattern10_0016, Pattern10_0017, Pattern10_0018, Pattern10_0019, Pattern10_0020,
    Pattern10_0021, Pattern10_0022, Pattern10_0023, Pattern10_0024, Pattern10_0025, Pattern10_0026, Pattern10_0027,
    Pattern10_0028, Pattern10_0029, Pattern10_0030, Pattern10_0031, Pattern10_0032, Pattern10_0033, Pattern10_0034,
    Pattern10_0035, Pattern10_0036, Pattern10_0037, Pattern10_0038, Pattern10_0039, Pattern10_0040, Pattern10_0041,
    Pattern10_0042, Pattern10_0043, Pattern10_0044, Pattern10_0045, Pattern10_0046, Pattern10_0047, Pattern10_0048,
    Pattern10_0049, Pattern10_0050, Pattern10_0051, Pattern10_0052, Pattern10_0053, Pattern10_0054, Pattern10_0055,
    Pattern10_0056, Pattern10_0057, Pattern10_0058, Pattern10_0059, Pattern10_0060, Pattern10_0061, Pattern10_0062,
    Pattern10_0063, Pattern10_0064, Pattern10_0065, Pattern10_0066, Pattern10_0067, Pattern10_0068, Pattern10_0069
};
