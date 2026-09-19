/**
 * @file pass17.c
 * COM Passive: Q
 */

#include "sf33rd/Source/Game/com/passive/pass17.h"
#include "sf33rd/Source/Game/com/passive/pass17_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive17_Tbl[182])();

void passive17_pattern_lever_on_normal_attack_command_attack(
    PLW* wk, u16 lever_data, u16 lever_data_b, const Command_Attack_Args* p
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 0xFFFF);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data);
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    case 3:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17(PLW* wk) {
    Passive17_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive17_0000(PLW* wk) {
    active_pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive17_0001(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Passive17_0002(PLW* wk) {
    pattern_wait_get_up_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 0xB, -1},
        &(EM_Term_Params){0x7FFF, -1, 1, 1, -1}
    );
}

void Passive17_0003(PLW* wk) {
    pattern_vs_jump_guard(wk);
}

void Passive17_0004(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x60, 0x53, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive17_0005(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive17_0006(PLW* wk) {
    pattern_approach_walk_em_term(wk, 0x7F, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
}

void Passive17_0007(PLW* wk) {
    active_pattern_approach_walk_command_attack(wk, 0x7F, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive17_0008(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive17_0009(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive17_0010(PLW* wk) {
    pattern_short_range_attack(wk);
}

void Passive17_0011(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x200, 2, -0x7F90, -1, 0x200});
}

void Passive17_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Passive17_0013(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x60, 0x6C, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive17_0014(PLW* wk) {
    pattern_wait_command_attack(wk, 3, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive17_0015(PLW* wk) {
    pattern_wait_get_up(wk, 0, -1);
}

void Passive17_0016(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away(wk, 0x6C, 0x89, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5A, 0x5B});
}

void Passive17_0017(PLW* wk) {
    pattern_wait_get_up_normal_attack_2(wk, 0x200);
}

void Passive17_0018(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_2(wk, 0x89, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5A, 0x5B});
}

void Passive17_0019(PLW* wk) {
    pattern_wait_get_up(wk, 3, 1);
}

void Passive17_0020(PLW* wk) {
    pattern_wait_get_up(wk, 3, -1);
}

void Passive17_0021(PLW* wk) {
    pattern_wait_get_up_command_attack(wk, 0, -1, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive17_0022(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F80, -0x7FB0, 0xB, 0x100, 0, -0x7F90, -1, 0x200},
        0x200
    );
}

void Passive17_0023(PLW* wk) {
    pattern_jump_attack_term_wait_get_up_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x200, 0, -0x7F90, -1, 0x200},
        0x200
    );
}

void Passive17_0024(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_3(wk, 0x89);
}

void Passive17_0025(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_5(wk, 0x89);
}

void Passive17_0026(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 8, 0x42);
}

void Passive17_0027(PLW* wk) {
    pattern_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive17_0028(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 10, 0x400, 1});
}

void Passive17_0029(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x10, 0x10, 0x10);
}

void Passive17_0030(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FB8, 6, 1, -1}, 8, 0x400);
}

void Passive17_0031(PLW* wk) {
    pattern_walk(wk, 1);
}

void Passive17_0032(PLW* wk) {
    active_pattern_sa_term_command_attack(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0}, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive17_0033(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}, 1);
}

void Passive17_0034(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive17_0035(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x10, 0x102, 0x200);
}

void Passive17_0036(PLW* wk) {
    pattern_command_attack_com_random_select(
        wk,
        &(Command_Attack_Args){0xB, 0x1E, 9, -1},
        &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}
    );
}

void Passive17_0037(PLW* wk) {
    pattern_em_term_com_random_select(
        wk,
        &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}
    );
}

void Passive17_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0x4B, 3);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive17_0039(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive17_0040(PLW* wk) {
    pattern_em_term_normal_attack_7(wk, &(EM_Term_Params){-1, -0x7FE0, 6, 1, -1}, 8, 0x200);
}

void Passive17_0041(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FC0, 6, 1, -1}, 0xB, 0x40);
}

void Passive17_0042(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 10, 0x400, 2});
}

void Passive17_0043(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -0x7FB8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive17_0044(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x4B,
        &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 9, -1}
    );
}

void Passive17_0045(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FD0, 6, 1, -1}, 8, 0x40);
}

void Passive17_0046(PLW* wk) {
    pattern_keep_status(wk, -1);
}

void Passive17_0047(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xB, 0x102, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive17_0048(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x402);
}

void (*const Passive17_Tbl[182])(PLW*) = {
    Passive17_0000, Passive17_0001, Passive17_0002, Passive17_0003, Passive17_0004, Passive17_0005, Passive17_0006,
    Passive17_0007, Passive17_0008, Passive17_0009, Passive17_0010, Passive17_0011, Passive17_0012, Passive17_0013,
    Passive17_0014, Passive17_0015, Passive17_0016, Passive17_0017, Passive17_0018, Passive17_0019, Passive17_0020,
    Passive17_0021, Passive17_0022, Passive17_0023, Passive17_0024, Passive17_0025, Passive17_0026, Passive17_0027,
    Passive17_0028, Passive17_0029, Passive17_0030, Passive17_0031, Passive17_0032, Passive17_0033, Passive17_0034,
    Passive17_0035, Passive17_0036, Passive17_0037, Passive17_0038, Passive17_0039, Passive17_0040, Passive17_0041,
    Passive17_0042, Passive17_0043, Passive17_0044, Passive17_0045, Passive17_0046, Passive17_0047, Passive17_0048,
    Passive17_0049, Passive17_0050, Passive17_0051, Passive17_0052, Passive17_0053, Passive17_0054, Passive17_0055,
    Passive17_0056, Passive17_0057, Passive17_0058, Passive17_0059, Passive17_0060, Passive17_0061, Passive17_0062,
    Passive17_0063, Passive17_0064, Passive17_0065, Passive17_0066, Passive17_0067, Passive17_0068, Passive17_0069,
    Passive17_0070, Passive17_0071, Passive17_0072, Passive17_0073, Passive17_0074, Passive17_0075, Passive17_0076,
    Passive17_0077, Passive17_0078, Passive17_0079, Passive17_0080, Passive17_0081, Passive17_0082, Passive17_0083,
    Passive17_0084, Passive17_0085, Passive17_0086, Passive17_0087, Passive17_0088, Passive17_0089, Passive17_0090,
    Passive17_0091, Passive17_0092, Passive17_0093, Passive17_0094, Passive17_0095, Passive17_0096, Passive17_0097,
    Passive17_0098, Passive17_0099, Passive17_0100, Passive17_0101, Passive17_0102, Passive17_0103, Passive17_0104,
    Passive17_0105, Passive17_0106, Passive17_0107, Passive17_0108, Passive17_0109, Passive17_0110, Passive17_0111,
    Passive17_0112, Passive17_0113, Passive17_0114, Passive17_0115, Passive17_0116, Passive17_0117, Passive17_0118,
    Passive17_0119, Passive17_0120, Passive17_0121, Passive17_0122, Passive17_0123, Passive17_0124, Passive17_0125,
    Passive17_0126, Passive17_0127, Passive17_0128, Passive17_0129, Passive17_0130, Passive17_0131, Passive17_0132,
    Passive17_0133, Passive17_0134, Passive17_0135, Passive17_0136, Passive17_0137, Passive17_0138, Passive17_0139,
    Passive17_0140, Passive17_0141, Passive17_0142, Passive17_0143, Passive17_0144, Passive17_0145, Passive17_0146,
    Passive17_0147, Passive17_0148, Passive17_0149, Passive17_0150, Passive17_0151, Passive17_0152, Passive17_0153,
    Passive17_0154, Passive17_0155, Passive17_0156, Passive17_0157, Passive17_0158, Passive17_0159, Passive17_0160,
    Passive17_0161, Passive17_0162, Passive17_0163, Passive17_0164, Passive17_0165, Passive17_0166, Passive17_0167,
    Passive17_0168, Passive17_0169, Passive17_0170, Passive17_0171, Passive17_0172, Passive17_0173, Passive17_0174,
    Passive17_0175, Passive17_0176, Passive17_0177, Passive17_0178, Passive17_0179, Passive17_0180, Passive17_0181
};
