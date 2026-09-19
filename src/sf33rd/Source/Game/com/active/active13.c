/**
 * @file active13.c
 * COM Active: Urien
 */

#include "sf33rd/Source/Game/com/active/active13.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern13_Tbl[81])();

void Computer13(PLW* wk) {
    Pattern13_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern13_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern13_0001(PLW* wk) {
    active_pattern_normal_attack_2(wk, 0x10, 0x10);
}

void Pattern13_0002(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern13_0003(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 0xD, 0x10, 0x20);
}

void Pattern13_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Pattern13_0005(PLW* wk) {
    active_pattern_approach_walk_lever_attack(wk, 0x33, 0);
}

void Pattern13_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern13_0007(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Pattern13_0008(PLW* wk) {
    active_pattern_normal_attack_2(wk, 0x100, 0x400);
}

void Pattern13_0009(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 2, 0x10);
        break;

    case 1:
        Jump(wk, 1);
        break;

    case 2:
        Lever_On(wk, 1, 2);
        break;

    case 3:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern13_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern13_0011(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern13_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Pattern13_0013(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Pattern13_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 8, 0x40, 2, -0x7F90, 8, 0x20});
}

void Pattern13_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern13_0016(PLW* wk) {
    active_pattern_search_back_term_walk(wk, 0x50, 2, 0);
}

void Pattern13_0017(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern13_0018(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Pattern13_0019(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Pattern13_0020(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 8, 0x10, 0, -0x7F90, 8, 0x20});
}

void Pattern13_0021(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 8, 0x400, 0, -0x7F90, 8, 0x200});
}

void Pattern13_0022(PLW* wk) {
    active_pattern_pierce_on_search_back_term_command_attack(wk);
}

void Pattern13_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7B);
}

void Pattern13_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern13_0025(PLW* wk) {
    active_pattern_search_back_term_walk_2(wk, 0x70, 2, 0);
}

void Pattern13_0026(PLW* wk) {
    active_pattern_command_attack_look(wk);
}

void Pattern13_0027(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x10, 0, -0x7F90, 8, 0x20});
}

void Pattern13_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7B);
}

void Pattern13_0029(PLW* wk) {
    active_pattern_adjust_attack(wk, 0x100, 0x100, 0x400);
}

void Pattern13_0030(PLW* wk) {
    active_pattern_adjust_attack(wk, 0x10, 0x20, 0x40);
}

void Pattern13_0031(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 9, 0x20, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern13_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Lever_On(wk, 1, 2);
        break;

    case 2:
        Adjust_Attack(wk, 0xB, 0x10);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern13_0033(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 9, 0x10, 0x110);
}

void Pattern13_0034(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern13_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern13_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern13_0037(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Lever_On(wk, 1, 2);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x42);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern13_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 0);
        break;

    case 1:
        Adjust_Attack(wk, 0xB, 0x20);
        break;

    case 2:
        Normal_Attack(wk, 0xA, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern13_0039(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 9, 0x200, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern13_0040(PLW* wk) {
    active_pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 8, -1}, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern13_0041(PLW* wk) {
    active_pattern_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1}, 8, 0x402);
}

void Pattern13_0042(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern13_0043(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 8, 0x400, 0, -1, -1, -1});
}

void Pattern13_0044(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 8, 0x40, 0, -1, -1, -1});
}

void Pattern13_0045(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, -0x7FC0, 8, 0x400, 0, -1, -1, 0xFFFF});
}

void Pattern13_0046(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Pattern13_0047(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern13_0048(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern13_0049(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern13_0050(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern13_0051(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern13_0052(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x31, 0x31, 0x31, 0x32}, 0);
}

void Pattern13_0053(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x31, 0x31, 0x32, 0x33}, 0);
}

void Pattern13_0054(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x31, 0x32, 0x32, 0x33}, 0);
}

void Pattern13_0055(PLW* wk) {
    active_pattern_approach_walk_lever_attack(wk, 0x33, 0);
}

void Pattern13_0056(PLW* wk) {
    pattern_search_back_term_jump_attack_term(
        wk,
        &(Search_Back_Term_Step){ 0x60, 2, 0x10 },
        &(Jump_Term_Args){-1, -0x7FB8, 8, 0x400, 1, -0x7F90, -1, 0x200}
    );
}

void Pattern13_0057(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern13_0058(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 0xA, -1});
}

void Pattern13_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 8, -1});
}

void Pattern13_0060(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 9, -1});
}

void Pattern13_0061(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern13_0062(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xB, -1});
}

void Pattern13_0063(PLW* wk) {
    pattern_etc_term_check_sa_branch_unit_area(
        wk,
        &(ETC_Term_Step){ 7, 2, 0x44 },
        0x44,
        &(Branch_Menu_Args){2, 0x40, 0x40, 0x41, 0x42}
    );
}

void Pattern13_0064(PLW* wk) {
    active_pattern_command_attack_check_sa_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 9, -1});
}

void Pattern13_0065(PLW* wk) {
    active_pattern_command_attack_check_sa_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern13_0066(PLW* wk) {
    pattern_command_attack_check_sa_command_attack_2(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern13_0067(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1E, 8, 0x70},
        &(Command_Attack_Args){8, 0, 0xB, -1}
    );
}

void Pattern13_0068(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x39, 0x3A, 0x3C, 0xFFFF},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Pattern13_0069(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x40, 0, -0x7F90, 8, 0x400});
}

void Pattern13_0070(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x200);
}

void Pattern13_0071(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x20);
}

void Pattern13_0072(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x400);
}

void Pattern13_0073(PLW* wk) {
    active_pattern_lever_attack(wk, 0xD, 0, 0x20);
}

void Pattern13_0074(PLW* wk) {
    active_pattern_normal_attack_lever_attack(wk, 8, 0x20, 0);
}

void Pattern13_0075(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0x4008, -1});
}

void Pattern13_0076(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0x4009, -1});
}

void Pattern13_0077(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0x400A, -1});
}

void Pattern13_0078(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x31, 0x31, 0x31, 0x4B}, 4);
}

void Pattern13_0079(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x32, 0x32, 0x31, 0x4C}, 4);
}

void Pattern13_0080(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x31, 0x32, 0x33, 0x4C}, 4);
}

void (*const Pattern13_Tbl[81])(PLW*) = {
    Pattern13_0000, Pattern13_0001, Pattern13_0002, Pattern13_0003, Pattern13_0004, Pattern13_0005, Pattern13_0006,
    Pattern13_0007, Pattern13_0008, Pattern13_0009, Pattern13_0010, Pattern13_0011, Pattern13_0012, Pattern13_0013,
    Pattern13_0014, Pattern13_0015, Pattern13_0016, Pattern13_0017, Pattern13_0018, Pattern13_0019, Pattern13_0020,
    Pattern13_0021, Pattern13_0022, Pattern13_0023, Pattern13_0024, Pattern13_0025, Pattern13_0026, Pattern13_0027,
    Pattern13_0028, Pattern13_0029, Pattern13_0030, Pattern13_0031, Pattern13_0032, Pattern13_0033, Pattern13_0034,
    Pattern13_0035, Pattern13_0036, Pattern13_0037, Pattern13_0038, Pattern13_0039, Pattern13_0040, Pattern13_0041,
    Pattern13_0042, Pattern13_0043, Pattern13_0044, Pattern13_0045, Pattern13_0046, Pattern13_0047, Pattern13_0048,
    Pattern13_0049, Pattern13_0050, Pattern13_0051, Pattern13_0052, Pattern13_0053, Pattern13_0054, Pattern13_0055,
    Pattern13_0056, Pattern13_0057, Pattern13_0058, Pattern13_0059, Pattern13_0060, Pattern13_0061, Pattern13_0062,
    Pattern13_0063, Pattern13_0064, Pattern13_0065, Pattern13_0066, Pattern13_0067, Pattern13_0068, Pattern13_0069,
    Pattern13_0070, Pattern13_0071, Pattern13_0072, Pattern13_0073, Pattern13_0074, Pattern13_0075, Pattern13_0076,
    Pattern13_0077, Pattern13_0078, Pattern13_0079, Pattern13_0080
};
