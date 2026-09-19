/**
 * @file active03.c
 * COM Active: Yun
 */

#include "sf33rd/Source/Game/com/active/active03.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern03_Tbl[72])();

void Computer03(PLW* wk) {
    Pattern03_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern03_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern03_0001(PLW* wk) {
    active_pattern_normal_attack_4(wk, 0x10, 0x10);
}

void Pattern03_0002(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){2, 0x31, 0x32, 0x33, 1});
}

void Pattern03_0003(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 9, 0x10, 0x20);
}

void Pattern03_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern03_0005(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x110);
}

void Pattern03_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern03_0007(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack(wk, 0x70, 2, 1);
}

void Pattern03_0008(PLW* wk) {
    active_pattern_normal_attack_3(wk, 0x100, 8, 0x400);
}

void Pattern03_0009(PLW* wk) {
    active_pattern_search_back_term_jump_look(wk, 0x70, 2, 0x10);
}

void Pattern03_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern03_0011(PLW* wk) {
    active_pattern_normal_attack_3(wk, 0x102, 8, 0x202);
}

void Pattern03_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Pattern03_0013(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x200);
}

void Pattern03_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x20, 2, -0x7fa0, 8, 0x20});
}

void Pattern03_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern03_0016(PLW* wk) {
    active_pattern_search_back_term_walk(wk, 0x70, 2, 0);
}

void Pattern03_0017(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Pattern03_0018(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x8400, 2});
}

void Pattern03_0019(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Pattern03_0020(PLW* wk) {
    active_pattern_lever_attack_command_attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Pattern03_0021(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -0x7fa0, 8, 0x200});
}

void Pattern03_0022(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FB0, -1, 8, 0x8100, 0, -0x7fa0, 8, 0x200});
}

void Pattern03_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x83);
}

void Pattern03_0024(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x20, 2, -0x7FA0, 8, 0x400});
}

void Pattern03_0025(PLW* wk) {
    active_pattern_search_back_term_walk_2(wk, 0x70, 2, 0);
}

void Pattern03_0026(PLW* wk) {
    active_pattern_command_attack_look(wk);
}

void Pattern03_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern03_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0xC3);
}

void Pattern03_0029(PLW* wk) {
    active_pattern_adjust_attack_lever_attack(wk);
}

void Pattern03_0030(PLW* wk) {
    active_pattern_adjust_attack_command_attack_2(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Pattern03_0031(PLW* wk) {
    active_pattern_normal_attack_branch_unit_area(wk, 0xC, 0x20, &(Branch_Menu_Args){2, 0x31, 0x32, 0x33, 0x33});
}

void Pattern03_0032(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 0xB, 0x10, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Pattern03_0033(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 9, 0x10, 0x110);
}

void Pattern03_0034(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 9, 0x102, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern03_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern03_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern03_0037(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x40, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Pattern03_0038(PLW* wk) {
    active_pattern_adjust_attack_normal_attack_branch_unit_area(wk);
}

void Pattern03_0039(PLW* wk) {
    active_pattern_command_attack_branch_unit_area(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern03_0040(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 9, 0x202, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Pattern03_0041(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xB, 0x42, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Pattern03_0042(PLW* wk) {
    active_pattern_normal_attack_8(wk, 0x12, 0x12, 0x402);
}

void Pattern03_0043(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, 8, 8, 0x8400, 0, -1, -0x7FA0, 0x400});
}

void Pattern03_0044(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x20, 0, -0x7FA0, 8, 0x400});
}

void Pattern03_0045(PLW* wk) {
    active_pattern_walk_2(wk, 0x14, 0x16, 0x18);
}

void Pattern03_0046(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern03_0047(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 0xA, -1});
}

void Pattern03_0048(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern03_0049(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Pattern03_0050(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Pattern03_0051(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Pattern03_0052(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Pattern03_0053(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0},
        &(Command_Attack_Args){8, 0x20, 8, -1}
    );
}

void Pattern03_0054(PLW* wk) {
    active_pattern_sa_term_hi_jump_attack_term(wk);
}

void Pattern03_0055(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -0x7FA0, 8, 0x20});
}

void Pattern03_0056(PLW* wk) {
    active_pattern_command_attack_com_random_select(
        wk,
        &(Command_Attack_Args){8, 0x1C, 0xA, -1},
        &(Branch_Menu_Args){2, 0x31, 4, 5, 1},
        0
    );
}

void Pattern03_0057(PLW* wk) {
    pattern_search_back_term_jump_attack_term(
        wk,
        &(Search_Back_Term_Step){ 0x70, 6, 0x12 },
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x20, 1, -0x7FA0, 8, 0x200}
    );
}

void Pattern03_0058(PLW* wk) {
    active_pattern_search_back_term_hi_jump_attack_term_pierce_on(wk);
}

void Pattern03_0059(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack_2(wk);
}

void Pattern03_0060(PLW* wk) {
    active_pattern_search_back_term_jump_attack_term_pierce_on(wk);
}

void Pattern03_0061(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack_3(wk);
}

void Pattern03_0062(PLW* wk) {
    active_pattern_hi_jump_attack_term_com_random_select(wk);
}

void Pattern03_0063(PLW* wk) {
    active_pattern_lever_attack_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Pattern03_0064(PLW* wk) {
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

void Pattern03_0065(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xB, 0x200, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Pattern03_0066(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x2D, 0x19, 0x10, 0x17}, 0);
}

void Pattern03_0067(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x44, 0x45, 0x46, 0x47});
}

void Pattern03_0068(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Pattern03_0069(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Pattern03_0070(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Pattern03_0071(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xB, 0x70});
}

void (*const Pattern03_Tbl[72])(PLW*) = {
    Pattern03_0000, Pattern03_0001, Pattern03_0002, Pattern03_0003, Pattern03_0004, Pattern03_0005, Pattern03_0006,
    Pattern03_0007, Pattern03_0008, Pattern03_0009, Pattern03_0010, Pattern03_0011, Pattern03_0012, Pattern03_0013,
    Pattern03_0014, Pattern03_0015, Pattern03_0016, Pattern03_0017, Pattern03_0018, Pattern03_0019, Pattern03_0020,
    Pattern03_0021, Pattern03_0022, Pattern03_0023, Pattern03_0024, Pattern03_0025, Pattern03_0026, Pattern03_0027,
    Pattern03_0028, Pattern03_0029, Pattern03_0030, Pattern03_0031, Pattern03_0032, Pattern03_0033, Pattern03_0034,
    Pattern03_0035, Pattern03_0036, Pattern03_0037, Pattern03_0038, Pattern03_0039, Pattern03_0040, Pattern03_0041,
    Pattern03_0042, Pattern03_0043, Pattern03_0044, Pattern03_0045, Pattern03_0046, Pattern03_0047, Pattern03_0048,
    Pattern03_0049, Pattern03_0050, Pattern03_0051, Pattern03_0052, Pattern03_0053, Pattern03_0054, Pattern03_0055,
    Pattern03_0056, Pattern03_0057, Pattern03_0058, Pattern03_0059, Pattern03_0060, Pattern03_0061, Pattern03_0062,
    Pattern03_0063, Pattern03_0064, Pattern03_0065, Pattern03_0066, Pattern03_0067, Pattern03_0068, Pattern03_0069,
    Pattern03_0070, Pattern03_0071
};
