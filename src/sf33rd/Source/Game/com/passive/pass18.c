/**
 * @file pass18.c
 * COM Passive: Twelve
 */

#include "sf33rd/Source/Game/com/passive/pass18.h"
#include "sf33rd/Source/Game/com/passive/pass18_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive18_Tbl[204])();

void passive18_pattern_wait_get_up_pierce_on_command_attack(PLW* wk, u16 lever_data, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, lever_data, -1);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive18_pattern_approach_walk_em_term_normal_attack(
    PLW* wk, s16 target_pos, const EM_Term_Params* p, u16 lever_data
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, p);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive18_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18(PLW* wk) {
    Passive18_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive18_0000(PLW* wk) {
    pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive18_0001(PLW* wk) {
    pattern_lever_off_look(wk);
}

void Passive18_0002(PLW* wk) {
    passive18_pattern_wait_get_up_pierce_on_command_attack(wk, 3, &(Command_Attack_Args){8, 0, 0xB, -1});
}

void Passive18_0003(PLW* wk) {
    pattern_vs_jump_guard(wk);
}

void Passive18_0004(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x60, 0x53, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive18_0005(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive18_0006(PLW* wk) {
    pattern_approach_walk_em_term(wk, 0x7F, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
}

void Passive18_0007(PLW* wk) {
    pattern_approach_walk_command_attack(wk, 0x6F, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive18_0008(PLW* wk) {
    pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive18_0009(PLW* wk) {
    pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
}

void Passive18_0010(PLW* wk) {
    pattern_short_range_attack(wk);
}

void Passive18_0011(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x20, 2, -0x7FA0, -1, 0x20});
}

void Passive18_0012(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x400);
}

void Passive18_0013(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x60, 0x6C, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive18_0014(PLW* wk) {
    pattern_wait_command_attack(wk, 3, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive18_0015(PLW* wk) {
    pattern_wait_get_up_2(wk, 0);
}

void Passive18_0016(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away(wk, 0x6C, 0x89, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
}

void Passive18_0017(PLW* wk) {
    pattern_wait_get_up_normal_attack_2(wk, 0x400);
}

void Passive18_0018(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_2(wk, 0x89, &(Branch_Menu_Args){6, 0x68, 0x68, 0x69, 0x6A});
}

void Passive18_0019(PLW* wk) {
    pattern_wait_get_up_2(wk, 3);
}

void Passive18_0020(PLW* wk) {
    pattern_wait_get_up_2(wk, 3);
}

void Passive18_0021(PLW* wk) {
    pattern_wait_get_up_command_attack(wk, 0, -1, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive18_0022(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F80, -0x7FB0, 0xB, 0x400, 0, -0x7FA0, -1, 0x200},
        0x20
    );
}

void Passive18_0023(PLW* wk) {
    pattern_jump_attack_term_wait_get_up_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x20, 0, -0x7FA0, -1, 0x200},
        0x400
    );
}

void Passive18_0024(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_4(wk, 0x60, 0x6C, 0x89);
}

static void passive18_0025_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, 0x89, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        passive18_0025_from_step_6(wk);
        break;
    }
}

void Passive18_0026(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 0x42);
}

void Passive18_0027(PLW* wk) {
    pattern_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive18_0028(PLW* wk) {
    pattern_jump_attack(wk, &(Jump_Attack_Args){8, 10, 0x400, 1});
}

void Passive18_0029(PLW* wk) {
    pattern_normal_attack_7(wk, 0x10, 0x10, 0x10);
}

void Passive18_0030(PLW* wk) {
    passive18_pattern_approach_walk_em_term_normal_attack(
        wk,
        0x7F,
        &(EM_Term_Params){-0x7F80, -0x7FB8, 6, 1, -1},
        0x400
    );
}

void Passive18_0031(PLW* wk) {
    pattern_walk(wk, 1);
}

void Passive18_0032(PLW* wk) {
    pattern_sa_term_command_attack(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0}, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive18_0033(PLW* wk) {
    pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}, 1);
}

void Passive18_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x10);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FB8, 1, -1, 0x30, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0035(PLW* wk) {
    pattern_normal_attack_5(wk, 8, 8, 0x102);
}

void Passive18_0036(PLW* wk) {
    pattern_command_attack_com_random_select(
        wk,
        &(Command_Attack_Args){0xB, 0x1C, 10, -1},
        &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}
    );
}

void Passive18_0037(PLW* wk) {
    pattern_approach_walk_em_term_com_random_select(wk, 0x37, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F});
}

void Passive18_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FB8, 6, 1, -1});
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, 0x40, 1, -1, 0x30, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive18_0039(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x6F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FB8, 6, 1, -1});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x35, 0x37, 0xC5, 0x86}, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Passive18_Tbl[204])(PLW*) = {
    Passive18_0000, Passive18_0001, Passive18_0002, Passive18_0003, Passive18_0004, Passive18_0005, Passive18_0006,
    Passive18_0007, Passive18_0008, Passive18_0009, Passive18_0010, Passive18_0011, Passive18_0012, Passive18_0013,
    Passive18_0014, Passive18_0015, Passive18_0016, Passive18_0017, Passive18_0018, Passive18_0019, Passive18_0020,
    Passive18_0021, Passive18_0022, Passive18_0023, Passive18_0024, Passive18_0025, Passive18_0026, Passive18_0027,
    Passive18_0028, Passive18_0029, Passive18_0030, Passive18_0031, Passive18_0032, Passive18_0033, Passive18_0034,
    Passive18_0035, Passive18_0036, Passive18_0037, Passive18_0038, Passive18_0039, Passive18_0040, Passive18_0041,
    Passive18_0042, Passive18_0043, Passive18_0044, Passive18_0045, Passive18_0046, Passive18_0047, Passive18_0048,
    Passive18_0049, Passive18_0050, Passive18_0051, Passive18_0052, Passive18_0053, Passive18_0054, Passive18_0055,
    Passive18_0056, Passive18_0057, Passive18_0058, Passive18_0059, Passive18_0060, Passive18_0061, Passive18_0062,
    Passive18_0063, Passive18_0064, Passive18_0065, Passive18_0066, Passive18_0067, Passive18_0068, Passive18_0069,
    Passive18_0070, Passive18_0071, Passive18_0072, Passive18_0073, Passive18_0074, Passive18_0075, Passive18_0076,
    Passive18_0077, Passive18_0078, Passive18_0079, Passive18_0080, Passive18_0081, Passive18_0082, Passive18_0083,
    Passive18_0084, Passive18_0085, Passive18_0086, Passive18_0087, Passive18_0088, Passive18_0089, Passive18_0090,
    Passive18_0091, Passive18_0092, Passive18_0093, Passive18_0094, Passive18_0095, Passive18_0096, Passive18_0097,
    Passive18_0098, Passive18_0099, Passive18_0100, Passive18_0101, Passive18_0102, Passive18_0103, Passive18_0104,
    Passive18_0105, Passive18_0106, Passive18_0107, Passive18_0108, Passive18_0109, Passive18_0110, Passive18_0111,
    Passive18_0112, Passive18_0113, Passive18_0114, Passive18_0115, Passive18_0116, Passive18_0117, Passive18_0118,
    Passive18_0119, Passive18_0120, Passive18_0121, Passive18_0122, Passive18_0123, Passive18_0124, Passive18_0125,
    Passive18_0126, Passive18_0127, Passive18_0128, Passive18_0129, Passive18_0130, Passive18_0131, Passive18_0132,
    Passive18_0133, Passive18_0134, Passive18_0135, Passive18_0136, Passive18_0137, Passive18_0138, Passive18_0139,
    Passive18_0140, Passive18_0141, Passive18_0142, Passive18_0143, Passive18_0144, Passive18_0145, Passive18_0146,
    Passive18_0147, Passive18_0148, Passive18_0149, Passive18_0150, Passive18_0151, Passive18_0152, Passive18_0153,
    Passive18_0154, Passive18_0155, Passive18_0156, Passive18_0157, Passive18_0158, Passive18_0159, Passive18_0160,
    Passive18_0161, Passive18_0162, Passive18_0163, Passive18_0164, Passive18_0165, Passive18_0166, Passive18_0167,
    Passive18_0168, Passive18_0169, Passive18_0170, Passive18_0171, Passive18_0172, Passive18_0173, Passive18_0174,
    Passive18_0175, Passive18_0176, Passive18_0177, Passive18_0178, Passive18_0179, Passive18_0180, Passive18_0181,
    Passive18_0182, Passive18_0183, Passive18_0184, Passive18_0185, Passive18_0186, Passive18_0187, Passive18_0188,
    Passive18_0189, Passive18_0190, Passive18_0191, Passive18_0192, Passive18_0193, Passive18_0194, Passive18_0195,
    Passive18_0196, Passive18_0197, Passive18_0198, Passive18_0199, Passive18_0200, Passive18_0201, Passive18_0202,
    Passive18_0203
};
