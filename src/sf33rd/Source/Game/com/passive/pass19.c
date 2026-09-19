/**
 * @file pass19.c
 * COM Passive: Remy
 */

#include "sf33rd/Source/Game/com/passive/pass19.h"
#include "sf33rd/Source/Game/com/passive/pass19_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive19_Tbl[176])();

void Passive19(PLW* wk) {
    Passive19_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive19_0000(PLW* wk) {
    pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive19_0001(PLW* wk) {
    pattern_lever_off_look(wk);
}

void Passive19_0002(PLW* wk) {
    pattern_wait_get_up_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 0xB, -1},
        &(EM_Term_Params){0x7FFF, -1, 1, 1, -1}
    );
}

void Passive19_0003(PLW* wk) {
    pattern_vs_jump_guard(wk);
}

void Passive19_0004(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x60, 0x53, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive19_0005(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive19_0006(PLW* wk) {
    pattern_approach_walk_em_term(wk, 0x7F, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
}

void Passive19_0007(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive19_0008(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive19_0009(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive19_0010(PLW* wk) {
    pattern_short_range_attack(wk);
}

void Passive19_0011(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x20, 2, -0x7FA0, -1, 0x20});
}

void Passive19_0012(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x400);
}

void Passive19_0013(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x60, 0x6C, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive19_0014(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive19_0015(PLW* wk) {
    pattern_wait_get_up(wk, 0, -1);
}

void Passive19_0016(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away(wk, 0x6C, 0x89, &(Branch_Menu_Args){6, 0x68, 0x69, 0x6A, 0x6A});
}

void Passive19_0017(PLW* wk) {
    pattern_wait_get_up_normal_attack_2(wk, 0x200);
}

void Passive19_0018(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_2(wk, 0x89, &(Branch_Menu_Args){6, 0x68, 0x69, 0x6A, 0x6A});
}

void Passive19_0019(PLW* wk) {
    pattern_wait_get_up(wk, 0, 1);
}

void Passive19_0020(PLW* wk) {
    pattern_wait_get_up(wk, 3, -1);
}

void Passive19_0021(PLW* wk) {
    pattern_wait_get_up_command_attack(wk, 0, -1, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive19_0022(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F80, -0x7FB0, 0xB, 0x100, 0, -0x7FA0, -1, 0x200},
        0x200
    );
}

void Passive19_0023(PLW* wk) {
    pattern_jump_attack_term_wait_get_up_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x20, 0, -0x7FA0, -1, 0x200},
        0x200
    );
}

void Passive19_0024(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_3(wk, 0x89);
}

void Passive19_0025(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_5(wk, 0x89);
}

void Passive19_0026(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 0x42);
}

void Passive19_0027(PLW* wk) {
    pattern_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive19_0028(PLW* wk) {
    pattern_jump_attack(wk, &(Jump_Attack_Args){8, 10, 0x400, 1});
}

void Passive19_0029(PLW* wk) {
    pattern_normal_attack_7(wk, 0x10, 0x10, 0x10);
}

void Passive19_0030(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FB8, 6, 1, -1}, 0x400);
}

void Passive19_0031(PLW* wk) {
    pattern_walk(wk, 1);
}

void Passive19_0032(PLW* wk) {
    pattern_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0x2E, 0x2F, 0x30, 0},
        &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F},
        1
    );
}

void Passive19_0033(PLW* wk) {
    pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}, 1);
}

void Passive19_0034(PLW* wk) {
    pattern_normal_attack_j_command_attack(wk, 0xC, 0x12, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive19_0035(PLW* wk) {
    pattern_normal_attack_7(wk, 0x12, 0x102, 0x200);
}

void Passive19_0036(PLW* wk) {
    pattern_command_attack_com_random_select(
        wk,
        &(Command_Attack_Args){0xB, 0x1D, 8, -1},
        &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}
    );
}

void Passive19_0037(PLW* wk) {
    pattern_em_term_com_random_select(
        wk,
        &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}
    );
}

void Passive19_0038(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1},
        &(Branch_Menu_Args){2, 0x4B, 0x4B, 0x4B, 0x4B}
    );
}

void Passive19_0039(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1},
        &(Branch_Menu_Args){2, 0x4D, 0x4D, 0x4D, 0x4D}
    );
}

void Passive19_0040(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1},
        &(Branch_Menu_Args){2, 0x4E, 0x4E, 0x4E, 0x4E}
    );
}

void Passive19_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0x7F, 3);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -0x7FC0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive19_0042(PLW* wk) {
    pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x400, 2});
}

void Passive19_0043(PLW* wk) {
    pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -0x7F80, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive19_0044(PLW* wk) {
    pattern_approach_walk_em_term_j_command_attack(
        wk,
        0x4B,
        &(EM_Term_Params){-1, -0x7F80, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive19_0045(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FD8, 6, 1, -1}, 0x40);
}

void Passive19_0046(PLW* wk) {
    pattern_keep_status(wk, -1);
}

void Passive19_0047(PLW* wk) {
    pattern_normal_attack_command_attack(wk, 0xB, 0x102, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive19_0048(PLW* wk) {
    pattern_normal_attack_3(wk, 0x12, 8, 0x22);
}

void Passive19_0049(PLW* wk) {
    pattern_em_term_adjust_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1});
}

void Passive19_0050(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 0x200);
}

void Passive19_0051(PLW* wk) {
    pattern_normal_attack_3(wk, 0x220, 0xB, 0x102);
}

void Passive19_0052(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x202);
}

void Passive19_0053(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive19_0054(PLW* wk) {
    pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x68, 0x69, 0x69, 0x6A});
}

void Passive19_0055(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive19_0056(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7FA0, -1, 0x20});
}

void Passive19_0057(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x20, 0, -0x7FA0, -1, 0x20});
}

void (*const Passive19_Tbl[176])(PLW*) = {
    Passive19_0000, Passive19_0001, Passive19_0002, Passive19_0003, Passive19_0004, Passive19_0005, Passive19_0006,
    Passive19_0007, Passive19_0008, Passive19_0009, Passive19_0010, Passive19_0011, Passive19_0012, Passive19_0013,
    Passive19_0014, Passive19_0015, Passive19_0016, Passive19_0017, Passive19_0018, Passive19_0019, Passive19_0020,
    Passive19_0021, Passive19_0022, Passive19_0023, Passive19_0024, Passive19_0025, Passive19_0026, Passive19_0027,
    Passive19_0028, Passive19_0029, Passive19_0030, Passive19_0031, Passive19_0032, Passive19_0033, Passive19_0034,
    Passive19_0035, Passive19_0036, Passive19_0037, Passive19_0038, Passive19_0039, Passive19_0040, Passive19_0041,
    Passive19_0042, Passive19_0043, Passive19_0044, Passive19_0045, Passive19_0046, Passive19_0047, Passive19_0048,
    Passive19_0049, Passive19_0050, Passive19_0051, Passive19_0052, Passive19_0053, Passive19_0054, Passive19_0055,
    Passive19_0056, Passive19_0057, Passive19_0058, Passive19_0059, Passive19_0060, Passive19_0061, Passive19_0062,
    Passive19_0063, Passive19_0064, Passive19_0065, Passive19_0066, Passive19_0067, Passive19_0068, Passive19_0069,
    Passive19_0070, Passive19_0071, Passive19_0072, Passive19_0073, Passive19_0074, Passive19_0075, Passive19_0076,
    Passive19_0077, Passive19_0078, Passive19_0079, Passive19_0080, Passive19_0081, Passive19_0082, Passive19_0083,
    Passive19_0084, Passive19_0085, Passive19_0086, Passive19_0087, Passive19_0088, Passive19_0089, Passive19_0090,
    Passive19_0091, Passive19_0092, Passive19_0093, Passive19_0094, Passive19_0095, Passive19_0096, Passive19_0097,
    Passive19_0098, Passive19_0099, Passive19_0100, Passive19_0101, Passive19_0102, Passive19_0103, Passive19_0104,
    Passive19_0105, Passive19_0106, Passive19_0107, Passive19_0108, Passive19_0109, Passive19_0110, Passive19_0111,
    Passive19_0112, Passive19_0113, Passive19_0114, Passive19_0115, Passive19_0116, Passive19_0117, Passive19_0118,
    Passive19_0119, Passive19_0120, Passive19_0121, Passive19_0122, Passive19_0123, Passive19_0124, Passive19_0125,
    Passive19_0126, Passive19_0127, Passive19_0128, Passive19_0129, Passive19_0130, Passive19_0131, Passive19_0132,
    Passive19_0133, Passive19_0134, Passive19_0135, Passive19_0136, Passive19_0137, Passive19_0138, Passive19_0139,
    Passive19_0140, Passive19_0141, Passive19_0142, Passive19_0143, Passive19_0144, Passive19_0145, Passive19_0146,
    Passive19_0147, Passive19_0148, Passive19_0149, Passive19_0150, Passive19_0151, Passive19_0152, Passive19_0153,
    Passive19_0154, Passive19_0155, Passive19_0156, Passive19_0157, Passive19_0158, Passive19_0159, Passive19_0160,
    Passive19_0161, Passive19_0162, Passive19_0163, Passive19_0164, Passive19_0165, Passive19_0166, Passive19_0167,
    Passive19_0168, Passive19_0169, Passive19_0170, Passive19_0171, Passive19_0172, Passive19_0173, Passive19_0174,
    Passive19_0175
};
