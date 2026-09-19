/**
 * @file active18.c
 * COM Active: Twelve
 */

#include "sf33rd/Source/Game/com/active/active18.h"
#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern18_Tbl[109])();

void Computer18(PLW* wk) {
    Pattern18_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern18_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern18_0001(PLW* wk) {
    active_pattern_normal_attack_4(wk, 0x10, 0x10);
}

void Pattern18_0002(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FA8, 1, -1, 0x30, 0x400});
}

void Pattern18_0003(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 8, 0x10, 0x200);
}

void Pattern18_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Pattern18_0005(PLW* wk) {
    active_pattern_approach_walk_com_random_select(wk, 0x37, &(Branch_Menu_Args){2, 0x3E, 0x3F, 0x40, 0x40});
}

void Pattern18_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern18_0007(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack(wk, 0x30, 2, 1);
}

void Pattern18_0008(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Pattern18_0009(PLW* wk) {
    active_pattern_search_back_term_jump_look(wk, 0x60, 6, 0x10);
}

void Pattern18_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x402);
        break;
    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern18_0011(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Pattern18_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Pattern18_0013(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern18_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x20, 2, -0x7FA0, -1, 0x20});
}

void Pattern18_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern18_0016(PLW* wk) {
    active_pattern_walk(wk, 1, 0x30, 0);
}

void Pattern18_0017(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Pattern18_0018(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FA8, 1, -1, 0x30, 0x400});
}

void Pattern18_0019(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Pattern18_0020(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){2, 0x41, 0x41, 0x42, 0x43});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern18_0021(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x200});
}

void Pattern18_0022(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xF, 0x400, 0});
}

void Pattern18_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Pattern18_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern18_0025(PLW* wk) {
    active_pattern_walk(wk, 1, 0x60, 0);
}

void Pattern18_0026(PLW* wk) {
    active_pattern_command_attack_look(wk);
}

void Pattern18_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern18_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0xBF);
}

void Pattern18_0029(PLW* wk) {
    active_pattern_adjust_attack_2(wk, 0x100, 0x200, 0x400);
}

void Pattern18_0030(PLW* wk) {
    active_pattern_adjust_attack_3(wk, 0x402);
}

void Pattern18_0031(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x400, 0, -0x7FA0, -1, 0x200},
        0x200
    );
}

void Pattern18_0032(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 0xB, 0x10, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern18_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 8, 0x10);
        break;

    case 1:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){2, 0x41, 0x41, 0x42, 0x43});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern18_0034(PLW* wk) {
    active_pattern_adjust_attack_2(wk, 0x102, 0x102, 0x200);
}

void Pattern18_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern18_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern18_0037(PLW* wk) {
    active_pattern_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1}, 8, 0x400);
}

void Pattern18_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xB, 0x20);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern18_0039(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){2, 0x41, 0x41, 0x42, 0x43});
        break;

    case 1:
        Adjust_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern18_0040(PLW* wk) {
    active_pattern_normal_attack_branch_unit_area(wk, 9, 0x42, &(Branch_Menu_Args){2, 0x41, 0x41, 0x42, 0x43});
}

void Pattern18_0041(PLW* wk) {
    active_pattern_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1}, 0xB, 0x40);
}

void Pattern18_0042(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x12, 0x12, 0x402);
}

void Pattern18_0043(PLW* wk) {
    active_pattern_search_back_term_walk_wait(wk);
}

void Pattern18_0044(PLW* wk) {
    active_pattern_search_back_term_walk_wait_2(wk);
}

void Pattern18_0045(PLW* wk) {
    active_pattern_walk_search_back_term_walk(wk);
}

void Pattern18_0046(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern18_0047(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x8026, 0xA, -1, 0x30, -0x7FB0, 1, -1, -1, -1});
}

void Pattern18_0048(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 0xA, -1});
}

void Pattern18_0049(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x8026, 8, -1, 0x30, -0x7FB0, 0, -1, -1, -1});
}

void Pattern18_0050(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x8026, 9, -1, 0x30, -0x7FB0, 2, -1, -1, -1});
}

void Pattern18_0051(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x8026, 0xA, -1, 0x30, -0x7FB0, 1, -1, -1, -1});
}

void Pattern18_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x41, 2);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern18_0053(PLW* wk) {
    active_pattern_sa_term_branch_unit_area(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
}

void Pattern18_0054(PLW* wk) {
    active_pattern_sa_term_branch_unit_area(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
}

void Pattern18_0055(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern18_0056(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Pattern18_0057(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Pattern18_0058(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){2, 0x41, 0x41, 0x42, 0x43});
}

void Pattern18_0059(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){2, 0x48, 0x48, 0x49, 0x4A});
}

void Pattern18_0060(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, 0x70});
}

void Pattern18_0061(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 9, 0x700, -1, -0x7FA8, 1, -1, 0x30, 0x400});
}

void Pattern18_0062(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x110);
}

void Pattern18_0063(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 1, 0x110);
}

void Pattern18_0064(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x110);
}

void Pattern18_0065(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Pattern18_0066(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Pattern18_0067(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Pattern18_0068(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 9, -1, -1, 0x50, 0, -0x7FA0, -1, 0x40});
}

void Pattern18_0069(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x10, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Pattern18_0070(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x10, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern18_0071(PLW* wk) {
    active_pattern_provoke(wk);
}

void Pattern18_0072(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, 0x70});
}

void Pattern18_0073(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, 0x70});
}

void Pattern18_0074(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, 0x70});
}

void Pattern18_0075(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Pattern18_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x40, 2, 0x1B);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern18_0077(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x10);
}

void Pattern18_0078(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x100);
}

void Pattern18_0079(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x102);
}

void Pattern18_0080(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x51, 0x51, 0x52, 0x53}, 0);
}

void Pattern18_0081(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 8, -1, -1, -0x7FA8, 2, -1, 0x30, 0x400});
}

void Pattern18_0082(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FA8, 2, -1, 0x30, 0x400});
}

void Pattern18_0083(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 0xA, -1, -1, -0x7FA8, 2, -1, 0x30, 0x400});
}

void Pattern18_0084(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x55, 0x55, 0x56, 0x57}, 0);
}

void Pattern18_0085(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 8, -1, -1, -0x7FA8, 0, -1, 0x30, 0x400});
}

void Pattern18_0086(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FA8, 0, -1, 0x30, 0x400});
}

void Pattern18_0087(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 0xA, -1, -1, -0x7FA8, 0, -1, 0x30, 0x400});
}

void Pattern18_0088(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x59, 0x59, 0x5A, 0x5B}, 0);
}

void Pattern18_0089(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 8, -1, -1, -0x7FA8, 0, -1, 0x30, 0x400});
}

void Pattern18_0090(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FA8, 0, -1, 0x30, 0x400});
}

void Pattern18_0091(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 0xA, -1, -1, -0x7FA8, 0, -1, 0x30, 0x400});
}

void Pattern18_0092(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x100, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Pattern18_0093(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x200, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern18_0094(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Pattern18_0095(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern18_0096(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x10, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Pattern18_0097(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x10, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern18_0098(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x100, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Pattern18_0099(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern18_0100(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Pattern18_0101(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern18_0102(PLW* wk) {
    active_pattern_normal_attack_5(wk, 0x102, 0x12, 0x402);
}

void Pattern18_0103(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern18_0104(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern18_0105(PLW* wk) {
    active_pattern_sa_term_branch_unit_area(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0});
}

void Pattern18_0106(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x11, 0x6B, 0x6C, 0x67}, 4);
}

void Pattern18_0107(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1C, 0xA, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Pattern18_0108(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0x1C, 0xA, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void (*const Pattern18_Tbl[109])(PLW*) = {
    Pattern18_0000, Pattern18_0001, Pattern18_0002, Pattern18_0003, Pattern18_0004, Pattern18_0005, Pattern18_0006,
    Pattern18_0007, Pattern18_0008, Pattern18_0009, Pattern18_0010, Pattern18_0011, Pattern18_0012, Pattern18_0013,
    Pattern18_0014, Pattern18_0015, Pattern18_0016, Pattern18_0017, Pattern18_0018, Pattern18_0019, Pattern18_0020,
    Pattern18_0021, Pattern18_0022, Pattern18_0023, Pattern18_0024, Pattern18_0025, Pattern18_0026, Pattern18_0027,
    Pattern18_0028, Pattern18_0029, Pattern18_0030, Pattern18_0031, Pattern18_0032, Pattern18_0033, Pattern18_0034,
    Pattern18_0035, Pattern18_0036, Pattern18_0037, Pattern18_0038, Pattern18_0039, Pattern18_0040, Pattern18_0041,
    Pattern18_0042, Pattern18_0043, Pattern18_0044, Pattern18_0045, Pattern18_0046, Pattern18_0047, Pattern18_0048,
    Pattern18_0049, Pattern18_0050, Pattern18_0051, Pattern18_0052, Pattern18_0053, Pattern18_0054, Pattern18_0055,
    Pattern18_0056, Pattern18_0057, Pattern18_0058, Pattern18_0059, Pattern18_0060, Pattern18_0061, Pattern18_0062,
    Pattern18_0063, Pattern18_0064, Pattern18_0065, Pattern18_0066, Pattern18_0067, Pattern18_0068, Pattern18_0069,
    Pattern18_0070, Pattern18_0071, Pattern18_0072, Pattern18_0073, Pattern18_0074, Pattern18_0075, Pattern18_0076,
    Pattern18_0077, Pattern18_0078, Pattern18_0079, Pattern18_0080, Pattern18_0081, Pattern18_0082, Pattern18_0083,
    Pattern18_0084, Pattern18_0085, Pattern18_0086, Pattern18_0087, Pattern18_0088, Pattern18_0089, Pattern18_0090,
    Pattern18_0091, Pattern18_0092, Pattern18_0093, Pattern18_0094, Pattern18_0095, Pattern18_0096, Pattern18_0097,
    Pattern18_0098, Pattern18_0099, Pattern18_0100, Pattern18_0101, Pattern18_0102, Pattern18_0103, Pattern18_0104,
    Pattern18_0105, Pattern18_0106, Pattern18_0107, Pattern18_0108
};
