/**
 * @file pass16.c
 * COM Passive: Makoto
 */

#include "sf33rd/Source/Game/com/passive/pass16.h"
#include "sf33rd/Source/Game/com/passive/pass16_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive16_Tbl[167])();

void passive16_pattern_check_ex_jump_command_attack_term(PLW* wk, s16 next_menu, const JCA_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, next_menu);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16(PLW* wk) {
    Passive16_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive16_0000(PLW* wk) {
    active_pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive16_0001(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Passive16_0002(PLW* wk) {
    pattern_wait_get_up_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 0xB, -1},
        &(EM_Term_Params){0x7FFF, -1, 1, 1, -1}
    );
}

void Passive16_0003(PLW* wk) {
    pattern_vs_jump_guard(wk);
}

void Passive16_0004(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x60, 0x53, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive16_0005(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive16_0006(PLW* wk) {
    pattern_approach_walk_em_term(wk, 0x7F, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
}

void Passive16_0007(PLW* wk) {
    active_pattern_approach_walk_command_attack(wk, 0x7F, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive16_0008(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive16_0009(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive16_0010(PLW* wk) {
    pattern_short_range_attack(wk);
}

void Passive16_0011(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x200, 2, -0x7FA0, -1, 0x20});
}

void Passive16_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Passive16_0013(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x60, 0x6C, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive16_0014(PLW* wk) {
    pattern_wait_branch_unit_area(wk, 3, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
}

void Passive16_0015(PLW* wk) {
    pattern_wait_get_up_2(wk, 0);
}

void Passive16_0016(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away(wk, 0x69, 0x89, &(Branch_Menu_Args){6, 0x6A, 0x69, 0x68, 1});
}

void Passive16_0017(PLW* wk) {
    pattern_wait_get_up_normal_attack_2(wk, 0x200);
}

void Passive16_0018(PLW* wk) {
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
        Normal_Attack(wk, 8, 0x12);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16_0019(PLW* wk) {
    pattern_wait_get_up_2(wk, 0);
}

void Passive16_0020(PLW* wk) {
    pattern_wait_get_up_2(wk, 3);
}

void Passive16_0021(PLW* wk) {
    pattern_wait_get_up_command_attack(wk, 0, -1, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive16_0022(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F80, -0x7FB0, 0xB, 0x100, 0, -0x7FA0, -1, 0x200},
        0x200
    );
}

void Passive16_0023(PLW* wk) {
    pattern_jump_attack_term_wait_get_up_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x20, 0, -0x7FA0, -1, 0x200},
        0x200
    );
}

void Passive16_0024(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_3(wk, 0x89);
}

void Passive16_0025(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_5(wk, 0x89);
}

void Passive16_0026(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 8, 0x402);
}

void Passive16_0027(PLW* wk) {
    pattern_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive16_0028(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 10, 0x200, 1});
}

void Passive16_0029(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x10, 0x10, 0x10);
}

void Passive16_0030(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FB8, 6, 1, -1}, 8, 0x200);
}

void Passive16_0031(PLW* wk) {
    pattern_walk(wk, 1);
}

void Passive16_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 3, 0x38, 0x44, 0x45}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16_0033(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}, 1);
}

void Passive16_0034(PLW* wk) {
    pattern_normal_attack_command_attack_com_random_select(
        wk,
        &(Normal_Attack_Step){ 0xC, 0x10 },
        &(Command_Attack_Args){8, 0x1F, 10, -1},
        &(Com_Random_Select_Step){ &(Branch_Menu_Args){2, 3, 0x38, 0x44, 0x45}, 1 }
    );
}

void Passive16_0035(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x10, 0x102, 0x200);
}

void Passive16_0036(PLW* wk) {
    pattern_command_attack_com_random_select(
        wk,
        &(Command_Attack_Args){0xB, 0x1E, 10, -1},
        &(Branch_Menu_Args){6, 0x42, 0x46, 0x69, 0x4A}
    );
}

void Passive16_0037(PLW* wk) {
    pattern_em_term_com_random_select(
        wk,
        &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x9D, 0x9E, 0x9F, 0x9F}
    );
}

void Passive16_0038(PLW* wk) {
    pattern_approach_walk_em_term_command_attack_4(wk, &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1});
}

void Passive16_0039(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive16_0040(PLW* wk) {
    pattern_em_term_normal_attack_7(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 8, 0x200);
}

void Passive16_0041(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FC0, 6, 1, -1}, 0xB, 0x40);
}

void Passive16_0042(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xF, 0x42, 0});
}

void Passive16_0043(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -0x7FD8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive16_0044(PLW* wk) {
    pattern_approach_walk_em_term_command_attack_4(wk, &(EM_Term_Params){-1, -0x7FE0, 6, 1, -1});
}

void Passive16_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -0x7FD8, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive16_0046(PLW* wk) {
    pattern_keep_status(wk, -1);
}

void (*const Passive16_Tbl[167])(PLW*) = {
    Passive16_0000, Passive16_0001, Passive16_0002, Passive16_0003, Passive16_0004, Passive16_0005, Passive16_0006,
    Passive16_0007, Passive16_0008, Passive16_0009, Passive16_0010, Passive16_0011, Passive16_0012, Passive16_0013,
    Passive16_0014, Passive16_0015, Passive16_0016, Passive16_0017, Passive16_0018, Passive16_0019, Passive16_0020,
    Passive16_0021, Passive16_0022, Passive16_0023, Passive16_0024, Passive16_0025, Passive16_0026, Passive16_0027,
    Passive16_0028, Passive16_0029, Passive16_0030, Passive16_0031, Passive16_0032, Passive16_0033, Passive16_0034,
    Passive16_0035, Passive16_0036, Passive16_0037, Passive16_0038, Passive16_0039, Passive16_0040, Passive16_0041,
    Passive16_0042, Passive16_0043, Passive16_0044, Passive16_0045, Passive16_0046, Passive16_0047, Passive16_0048,
    Passive16_0049, Passive16_0050, Passive16_0051, Passive16_0052, Passive16_0053, Passive16_0054, Passive16_0055,
    Passive16_0056, Passive16_0057, Passive16_0058, Passive16_0059, Passive16_0060, Passive16_0061, Passive16_0062,
    Passive16_0063, Passive16_0064, Passive16_0065, Passive16_0066, Passive16_0067, Passive16_0068, Passive16_0069,
    Passive16_0070, Passive16_0071, Passive16_0072, Passive16_0073, Passive16_0074, Passive16_0075, Passive16_0076,
    Passive16_0077, Passive16_0078, Passive16_0079, Passive16_0080, Passive16_0081, Passive16_0082, Passive16_0083,
    Passive16_0084, Passive16_0085, Passive16_0086, Passive16_0087, Passive16_0088, Passive16_0089, Passive16_0090,
    Passive16_0091, Passive16_0092, Passive16_0093, Passive16_0094, Passive16_0095, Passive16_0096, Passive16_0097,
    Passive16_0098, Passive16_0099, Passive16_0100, Passive16_0101, Passive16_0102, Passive16_0103, Passive16_0104,
    Passive16_0105, Passive16_0106, Passive16_0107, Passive16_0108, Passive16_0109, Passive16_0110, Passive16_0111,
    Passive16_0112, Passive16_0113, Passive16_0114, Passive16_0115, Passive16_0116, Passive16_0117, Passive16_0118,
    Passive16_0119, Passive16_0120, Passive16_0121, Passive16_0122, Passive16_0123, Passive16_0124, Passive16_0125,
    Passive16_0126, Passive16_0127, Passive16_0128, Passive16_0129, Passive16_0130, Passive16_0131, Passive16_0132,
    Passive16_0133, Passive16_0134, Passive16_0135, Passive16_0136, Passive16_0137, Passive16_0138, Passive16_0139,
    Passive16_0140, Passive16_0141, Passive16_0142, Passive16_0143, Passive16_0144, Passive16_0145, Passive16_0146,
    Passive16_0147, Passive16_0148, Passive16_0149, Passive16_0150, Passive16_0151, Passive16_0152, Passive16_0153,
    Passive16_0154, Passive16_0155, Passive16_0156, Passive16_0157, Passive16_0158, Passive16_0159, Passive16_0160,
    Passive16_0161, Passive16_0162, Passive16_0163, Passive16_0164, Passive16_0165, Passive16_0166
};
