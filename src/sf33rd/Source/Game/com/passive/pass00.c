/**
 * @file pass00.c
 * COM Passive: Gill
 */

#include "sf33rd/Source/Game/com/passive/pass00.h"
#include "sf33rd/Source/Game/com/passive/pass00_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive00_Tbl[158])();

void passive00_pattern_com_random_select(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, p, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive00_pattern_approach_walk_em_term_lever_attack(PLW* wk, const EM_Term_Params* p, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x33, 2);
        break;

    case 1:
        EM_Term(wk, p);
        break;

    case 2:
        Lever_Attack(wk, 8, lever, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive00_pattern_etc_term_command_attack(PLW* wk, u16 next_menu, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 0, 6, next_menu);
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive00_pattern_jump_attack_term_normal_attack(PLW* wk, const Jump_Term_Args* a, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00(PLW* wk) {
    Passive00_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive00_0000(PLW* wk) {
    pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive00_0001(PLW* wk) {
    pattern_lever_off_look(wk);
}

void Passive00_0002(PLW* wk) {
    pattern_wait_get_up_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 0xA, -1},
        &(EM_Term_Params){0x7FFF, -1, 1, 1, -1}
    );
}

void Passive00_0003(PLW* wk) {
    pattern_vs_jump_guard(wk);
}

void Passive00_0004(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x60, 0x53, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive00_0005(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive00_0006(PLW* wk) {
    pattern_approach_walk_em_term(wk, 0x7B, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
}

void Passive00_0007(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive00_0008(PLW* wk) {
    pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive00_0009(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, 0x38, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Passive00_0010(PLW* wk) {
    pattern_short_range_attack(wk);
}

void Passive00_0011(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Passive00_0012(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, 0, 0, 2, 0}, 0x20);
}

void Passive00_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x20);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0014(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x200);
}

void Passive00_0015(PLW* wk) {
    pattern_wait_get_up_2(wk, 0);
}

void Passive00_0016(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x71, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0017(PLW* wk) {
    pattern_approach_walk_wait_get_up_normal_attack_2(wk);
}

void Passive00_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x71, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x20);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0019(PLW* wk) {
    pattern_wait_get_up_2(wk, 0);
}

void Passive00_0020(PLW* wk) {
    pattern_wait_get_up_2(wk, 3);
}

void Passive00_0021(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 2:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0022(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Check_BOSS(wk, 6, 0x17);
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-1, -0x7FC0, 0xB, 0x40, 0, -0x7F90, -1, 0x20});
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x20);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0023(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-1, -0x7FC0, 0xB, 0x10, 0, -1, -1, -1},
        9,
        0x200
    );
}

void Passive00_0024(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 0, 0x400);
}

void Passive00_0025(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack(wk, 0x60, 0x68, &(Command_Attack_Args){8, 1, 0xA, -1});
}

void Passive00_0026(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x20);
}

void Passive00_0027(PLW* wk) {
    passive00_pattern_approach_walk_em_term_lever_attack(wk, &(EM_Term_Params){-1, -0x7FA0, 6, 1, -1}, 1, 0x20);
}

void Passive00_0028(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x220);
}

void Passive00_0029(PLW* wk) {
    pattern_normal_attack_7(wk, 0x10, 0x10, 0x10);
}

void Passive00_0030(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FD0, 6, 1, -1}, 0x40);
}

void Passive00_0031(PLW* wk) {
    pattern_walk(wk, 1);
}

void Passive00_0032(PLW* wk) {
    pattern_normal_attack_2(wk, 9, 0x100, 0x200);
}

void Passive00_0033(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x33, 0, 0x110);
}

void Passive00_0034(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x400);
}

void Passive00_0035(PLW* wk) {
    pattern_normal_attack_4(wk, 9, 0xB, 0x202);
}

void Passive00_0036(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 3:
        ETC_Term(wk, 0, 6, 0x99);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0037(PLW* wk) {
    passive00_pattern_approach_walk_em_term_lever_attack(wk, &(EM_Term_Params){-1, -0x7FFE, 6, 1, -1}, 0, 0x110);
}

void Passive00_0038(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FFE, 6, 1, -1}, 0x110);
}

void Passive00_0039(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x7B, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1}, 0x42);
}

void Passive00_0040(PLW* wk) {
    pattern_pierce_on_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FB0, 6, 1, -1}, 9, 0x40);
}

void Passive00_0041(PLW* wk) {
    pattern_em_term_normal_attack_3(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1}, 0xB, 0x100);
}

void Passive00_0042(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FC0, 8, 0x40, 0, -0x7F90, -1, 0x40});
}

void Passive00_0043(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_BOSS(wk, 6, 0x1B);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -0x7FB0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0044(PLW* wk) {
    pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive00_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_BOSS(wk, 6, 0x1B);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7F90, 6, 1, -1});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive00_0046(PLW* wk) {
    pattern_keep_status(wk, -1);
}

void Passive00_0047(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x102, 0x402);
}

void Passive00_0048(PLW* wk) {
    pattern_em_term_normal_attack_4(wk, &(EM_Term_Params){-0x7F80, -0x7FC0, 6, 1, -1});
}

void Passive00_0049(PLW* wk) {
    pattern_adjust_attack(wk, 0x10);
}

void Passive00_0050(PLW* wk) {
    pattern_normal_attack_2(wk, 9, 0x12, 0x22);
}

void (*const Passive00_Tbl[158])(PLW*) = {
    Passive00_0000, Passive00_0001, Passive00_0002, Passive00_0003, Passive00_0004, Passive00_0005, Passive00_0006,
    Passive00_0007, Passive00_0008, Passive00_0009, Passive00_0010, Passive00_0011, Passive00_0012, Passive00_0013,
    Passive00_0014, Passive00_0015, Passive00_0016, Passive00_0017, Passive00_0018, Passive00_0019, Passive00_0020,
    Passive00_0021, Passive00_0022, Passive00_0023, Passive00_0024, Passive00_0025, Passive00_0026, Passive00_0027,
    Passive00_0028, Passive00_0029, Passive00_0030, Passive00_0031, Passive00_0032, Passive00_0033, Passive00_0034,
    Passive00_0035, Passive00_0036, Passive00_0037, Passive00_0038, Passive00_0039, Passive00_0040, Passive00_0041,
    Passive00_0042, Passive00_0043, Passive00_0044, Passive00_0045, Passive00_0046, Passive00_0047, Passive00_0048,
    Passive00_0049, Passive00_0050, Passive00_0051, Passive00_0052, Passive00_0053, Passive00_0054, Passive00_0055,
    Passive00_0056, Passive00_0057, Passive00_0058, Passive00_0059, Passive00_0060, Passive00_0061, Passive00_0062,
    Passive00_0063, Passive00_0064, Passive00_0065, Passive00_0066, Passive00_0067, Passive00_0068, Passive00_0069,
    Passive00_0070, Passive00_0071, Passive00_0072, Passive00_0073, Passive00_0074, Passive00_0075, Passive00_0076,
    Passive00_0077, Passive00_0078, Passive00_0079, Passive00_0080, Passive00_0081, Passive00_0082, Passive00_0083,
    Passive00_0084, Passive00_0085, Passive00_0086, Passive00_0087, Passive00_0088, Passive00_0089, Passive00_0090,
    Passive00_0091, Passive00_0092, Passive00_0093, Passive00_0094, Passive00_0095, Passive00_0096, Passive00_0097,
    Passive00_0098, Passive00_0099, Passive00_0100, Passive00_0101, Passive00_0102, Passive00_0103, Passive00_0104,
    Passive00_0105, Passive00_0106, Passive00_0107, Passive00_0108, Passive00_0109, Passive00_0110, Passive00_0111,
    Passive00_0112, Passive00_0113, Passive00_0114, Passive00_0115, Passive00_0116, Passive00_0117, Passive00_0118,
    Passive00_0119, Passive00_0120, Passive00_0121, Passive00_0122, Passive00_0123, Passive00_0124, Passive00_0125,
    Passive00_0126, Passive00_0127, Passive00_0128, Passive00_0129, Passive00_0130, Passive00_0131, Passive00_0132,
    Passive00_0133, Passive00_0134, Passive00_0135, Passive00_0136, Passive00_0137, Passive00_0138, Passive00_0139,
    Passive00_0140, Passive00_0141, Passive00_0142, Passive00_0143, Passive00_0144, Passive00_0145, Passive00_0146,
    Passive00_0147, Passive00_0148, Passive00_0149, Passive00_0150, Passive00_0151, Passive00_0152, Passive00_0153,
    Passive00_0154, Passive00_0155, Passive00_0156, Passive00_0157
};
