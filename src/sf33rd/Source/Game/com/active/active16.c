/**
 * @file active16.c
 * COM Active: Makoto
 */

#include "sf33rd/Source/Game/com/active/active16.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern16_Tbl[70])();

void Computer16(PLW* wk) {
    Pattern16_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern16_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern16_0001(PLW* wk) {
    active_pattern_normal_attack_4(wk, 0x10, 0x10);
}

void Pattern16_0002(PLW* wk) {
    active_pattern_approach_walk_command_attack(wk, 0x41, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Pattern16_0003(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 8, 0x100, 0x200);
}

void Pattern16_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern16_0005(PLW* wk) {
    active_pattern_approach_walk_com_random_select(wk, 0x37, &(Branch_Menu_Args){2, 0x3E, 0x3E, 0x3F, 0x43});
}

void Pattern16_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern16_0007(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack(wk, 0x30, 2, 1);
}

void Pattern16_0008(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Pattern16_0009(PLW* wk) {
    active_pattern_search_back_term_jump_look(wk, 0x60, 6, 0x10);
}

void Pattern16_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern16_0011(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Pattern16_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Pattern16_0013(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x200);
}

void Pattern16_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x20, 2, -0x7FA0, -1, 0x20});
}

void Pattern16_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern16_0016(PLW* wk) {
    active_pattern_walk(wk, 1, 0x30, 0);
}

void Pattern16_0017(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern16_0018(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 9, -1, -0x7FA0, 0x40, 0, -1, 0x30, 0x400});
}

void Pattern16_0019(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x400);
}

void Pattern16_0020(PLW* wk) {
    pattern_lever_attack_2(wk, &(Lever_Attack_Step){ 0xC, 0, 0x40 }, &(Lever_Attack_Step){ 0xC, 0, 0x40 }, 0x40);
}

void Pattern16_0021(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x200});
}

void Pattern16_0022(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xF, 0x40, 0});
}

void Pattern16_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Pattern16_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern16_0025(PLW* wk) {
    active_pattern_walk(wk, 1, 0x60, 0);
}

void Pattern16_0026(PLW* wk) {
    active_pattern_command_attack_look(wk);
}

void Pattern16_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern16_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0xBF);
}

void Pattern16_0029(PLW* wk) {
    pattern_adjust_attack_lever_attack(wk, 0xC, &(Adjust_Attack_Step){ 8, 0x200 }, 0x400);
}

void Pattern16_0030(PLW* wk) {
    active_pattern_adjust_attack_3(wk, 0x42);
}

void Pattern16_0031(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x200},
        0x200
    );
}

void Pattern16_0032(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 0xB, 0x10, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern16_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
        break;

    case 1:
        Adjust_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern16_0034(PLW* wk) {
    active_pattern_adjust_attack_2(wk, 0x102, 0x102, 0x200);
}

void Pattern16_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern16_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern16_0037(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1E, 0xA, -1},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Pattern16_0038(PLW* wk) {
    active_pattern_adjust_attack_normal_attack_lever_attack(wk);
}

void Pattern16_0039(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    case 2:
        Adjust_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern16_0040(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 9, 0x202, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern16_0041(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern16_0042(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x12, 0x12, 0x42);
}

void Pattern16_0043(PLW* wk) {
    active_pattern_search_back_term_walk_wait(wk);
}

void Pattern16_0044(PLW* wk) {
    active_pattern_search_back_term_walk_wait_2(wk);
}

void Pattern16_0045(PLW* wk) {
    active_pattern_walk_search_back_term_walk(wk);
}

void Pattern16_0046(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern16_0047(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x10, 2, 0x40);
        break;

    case 1:
        Search_Back_Term(wk, 0x40, 2, 0x41);
        break;

    case 2:
        Search_Back_Term(wk, 0x70, 2, 0x42);
        break;

    case 3:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){2, 0x32, 0x33, 0x33, 0x33});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern16_0048(PLW* wk) {
    active_pattern_check_sa_command_attack(wk, 6, 0x11, &(Command_Attack_Args){8, 0x8015, 0xA, -1});
}

void Pattern16_0049(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 8, -1});
}

void Pattern16_0050(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 9, -1});
}

void Pattern16_0051(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern16_0052(PLW* wk) {
    pattern_approach_walk_sa_term_command_attack(wk, 0x4B, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Pattern16_0053(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0});
        break;

    case 1:
        Approach_Walk(wk, 0x4B, 2);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0xFFFF, 0xFFFF, 0x41});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 3, 0x38, 0x44, 0x45}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern16_0054(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Pattern16_0055(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern16_0056(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Pattern16_0057(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Pattern16_0058(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Pattern16_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, 0x700});
}

void Pattern16_0060(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, 0x70});
}

void Pattern16_0061(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 9, 0x700, -0x7FA0, 0x40, 0, -1, 0x30, 0x400});
}

void Pattern16_0062(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x110);
}

void Pattern16_0063(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 1, 0x110);
}

void Pattern16_0064(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){2, 6, 0x31, 0x32, 0x33});
}

void Pattern16_0065(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){2, 0x31, 0x31, 0x32, 0x33});
}

void Pattern16_0066(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){2, 0x31, 0x32, 0x33, 0x33});
}

void Pattern16_0067(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x110);
}

void Pattern16_0068(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern16_0069(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xB, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Pattern16_Tbl[70])(PLW*) = {
    Pattern16_0000, Pattern16_0001, Pattern16_0002, Pattern16_0003, Pattern16_0004, Pattern16_0005, Pattern16_0006,
    Pattern16_0007, Pattern16_0008, Pattern16_0009, Pattern16_0010, Pattern16_0011, Pattern16_0012, Pattern16_0013,
    Pattern16_0014, Pattern16_0015, Pattern16_0016, Pattern16_0017, Pattern16_0018, Pattern16_0019, Pattern16_0020,
    Pattern16_0021, Pattern16_0022, Pattern16_0023, Pattern16_0024, Pattern16_0025, Pattern16_0026, Pattern16_0027,
    Pattern16_0028, Pattern16_0029, Pattern16_0030, Pattern16_0031, Pattern16_0032, Pattern16_0033, Pattern16_0034,
    Pattern16_0035, Pattern16_0036, Pattern16_0037, Pattern16_0038, Pattern16_0039, Pattern16_0040, Pattern16_0041,
    Pattern16_0042, Pattern16_0043, Pattern16_0044, Pattern16_0045, Pattern16_0046, Pattern16_0047, Pattern16_0048,
    Pattern16_0049, Pattern16_0050, Pattern16_0051, Pattern16_0052, Pattern16_0053, Pattern16_0054, Pattern16_0055,
    Pattern16_0056, Pattern16_0057, Pattern16_0058, Pattern16_0059, Pattern16_0060, Pattern16_0061, Pattern16_0062,
    Pattern16_0063, Pattern16_0064, Pattern16_0065, Pattern16_0066, Pattern16_0067, Pattern16_0068, Pattern16_0069
};
