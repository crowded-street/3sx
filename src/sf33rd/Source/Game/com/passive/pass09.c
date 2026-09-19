/**
 * @file pass09.c
 * COM Passive: Oro
 */

#include "sf33rd/Source/Game/com/passive/pass09.h"
#include "sf33rd/Source/Game/com/passive/pass09_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive09_Tbl[132])();

void passive09_pattern_oro_ja_term(PLW* wk, const ORO_Air_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_JA_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive09_pattern_com_random_select(PLW* wk, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, p, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive09_pattern_em_term_adjust_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Adjust_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09(PLW* wk) {
    Passive09_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive09_0000(PLW* wk) {
    active_pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive09_0001(PLW* wk) {
    pattern_lever_on_em_term_check_store_lever_2(wk, &(Command_Attack_Args){8, 0x1C, 10, 0x70});
}

void Passive09_0002(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F10, -1, 5, 6, 0x2A},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive09_0003(PLW* wk) {
    pattern_check_store_lever_command_attack(
        wk,
        &(Check_Store_Lever_Step){ 0x1E, 1, -1 },
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive09_0004(PLW* wk) {
    pattern_check_ex_j_command_attack(wk, 6, &(Command_Attack_Args){8, 0x1F, 8, 0x700});
}

void Passive09_0005(PLW* wk) {
    pattern_check_store_lever_command_attack(
        wk,
        &(Check_Store_Lever_Step){ 0x1E, 1, -1 },
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive09_0006(PLW* wk) {
    pattern_look(wk);
}

void Passive09_0007(PLW* wk) {
    pattern_lever_on_em_term_check_store_lever_2(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive09_0008(PLW* wk) {
    pattern_lever_on_em_term_check_store_lever_2(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive09_0009(PLW* wk) {
    pattern_lever_on_em_term_check_store_lever_2(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive09_0010(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F90, -0x7FC0, 0, 6, 0x3C}, 0x20);
}

void Passive09_0011(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x20, 2, -0x7F70, -1, 0x20});
}

void Passive09_0012(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F68, -0x7FC0, 3, 2, 0}, 0x400);
}

void Passive09_0013(PLW* wk) {
    pattern_search_back_term_command_attack_2(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive09_0014(PLW* wk) {
    pattern_vs_jump_guard(wk);
}

void Passive09_0015(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6C);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0016(PLW* wk) {
    pattern_approach_walk_wait_get_up_normal_attack(wk, 0x37, 0x110);
}

void Passive09_0017(PLW* wk) {
    pattern_approach_walk_wait_get_up_normal_attack(wk, 0x7F, 0x202);
}

void Passive09_0018(PLW* wk) {
    pattern_approach_walk_wait_get_up_em_term(wk, 0x40);
}

void Passive09_0019(PLW* wk) {
    pattern_wait_get_up_2(wk, 3);
}

void Passive09_0020(PLW* wk) {
    pattern_wait_get_up_2(wk, 3);
}

void Passive09_0021(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FC0, 0, -1, -1, 0xFFFF});
}

void Passive09_0022(PLW* wk) {
    pattern_wait_get_up_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 8, 0x200, 0, -0x7F68, -1, 0x400});
}

void Passive09_0023(PLW* wk) {
    pattern_wait_get_up_jump_attack_term(wk, &(Jump_Term_Args){-0x7F98, -0x7FA8, 8, 0x40, 0, -0x7F78, -1, 0x40});
}

void Passive09_0024(PLW* wk) {
    pattern_approach_walk_wait_get_up_command_attack(wk, 0xBF, -1, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive09_0025(PLW* wk) {
    pattern_wait_get_up_command_attack_em_term(wk);
}

void Passive09_0026(PLW* wk) {
    pattern_approach_walk_em_term(wk, 0x7F, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
}

void Passive09_0027(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xB, 0x202, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive09_0028(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Passive09_0029(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x10, 0x10, 0x10);
}

void Passive09_0030(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Passive09_0031(PLW* wk) {
    pattern_walk(wk, 1);
}

void Passive09_0032(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x110);
}

void Passive09_0033(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x110);
}

void Passive09_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6C);
        break;

    case 1:
        Adjust_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x102);
}

void Passive09_0036(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Passive09_0037(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 0x110);
}

void Passive09_0038(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 1, 0x110);
}

void Passive09_0039(PLW* wk) {
    pattern_lever_on_em_term_j_command_attack(
        wk,
        0xFFFF,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, 0x70}
    );
}

void Passive09_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 6, 0x1F});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6D);
        break;

    case 1:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 6, 0x1F});
        break;

    case 3:
        Normal_Attack(wk, 9, 0x22);
        break;

    case 4:
        Check_Store_Lever(wk, 0x1C, 1, -1);
        break;

    case 5:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0042(PLW* wk) {
    pattern_look(wk);
}

void Passive09_0043(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 6, 0x1F});
        break;

    case 3:
        Check_Store_Lever(wk, 0x1C, 1, -1);
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0044(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x37, &(EM_Term_Params){-1, -0x7FF8, 3, 1, -1}, 0x110);
}

void Passive09_0045(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FF0, 6, 6, 0x1F},
        &(Command_Attack_Args){8, 0x1D, 8, -1}
    );
}

void Passive09_0046(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FA0, -0x7FC8, 0, 6, 0x3C}, 0x42);
}

void Passive09_0047(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC8, 8, 0x400, 0, -0x7F80, -1, 0x400});
}

void Passive09_0048(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x102, 0x402);
}

void Passive09_0049(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6D);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 3, 6, 0x1F});
        break;

    case 2:
        Adjust_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0050(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FB0, -1, 3, 6, 0x1F}, 0x200);
}

void (*const Passive09_Tbl[132])(PLW*) = {
    Passive09_0000, Passive09_0001, Passive09_0002, Passive09_0003, Passive09_0004, Passive09_0005, Passive09_0006,
    Passive09_0007, Passive09_0008, Passive09_0009, Passive09_0010, Passive09_0011, Passive09_0012, Passive09_0013,
    Passive09_0014, Passive09_0015, Passive09_0016, Passive09_0017, Passive09_0018, Passive09_0019, Passive09_0020,
    Passive09_0021, Passive09_0022, Passive09_0023, Passive09_0024, Passive09_0025, Passive09_0026, Passive09_0027,
    Passive09_0028, Passive09_0029, Passive09_0030, Passive09_0031, Passive09_0032, Passive09_0033, Passive09_0034,
    Passive09_0035, Passive09_0036, Passive09_0037, Passive09_0038, Passive09_0039, Passive09_0040, Passive09_0041,
    Passive09_0042, Passive09_0043, Passive09_0044, Passive09_0045, Passive09_0046, Passive09_0047, Passive09_0048,
    Passive09_0049, Passive09_0050, Passive09_0051, Passive09_0052, Passive09_0053, Passive09_0054, Passive09_0055,
    Passive09_0056, Passive09_0057, Passive09_0058, Passive09_0059, Passive09_0060, Passive09_0061, Passive09_0062,
    Passive09_0063, Passive09_0064, Passive09_0065, Passive09_0066, Passive09_0067, Passive09_0068, Passive09_0069,
    Passive09_0070, Passive09_0071, Passive09_0072, Passive09_0073, Passive09_0074, Passive09_0075, Passive09_0076,
    Passive09_0077, Passive09_0078, Passive09_0079, Passive09_0080, Passive09_0081, Passive09_0082, Passive09_0083,
    Passive09_0084, Passive09_0085, Passive09_0086, Passive09_0087, Passive09_0088, Passive09_0089, Passive09_0090,
    Passive09_0091, Passive09_0092, Passive09_0093, Passive09_0094, Passive09_0095, Passive09_0096, Passive09_0097,
    Passive09_0098, Passive09_0099, Passive09_0100, Passive09_0101, Passive09_0102, Passive09_0103, Passive09_0104,
    Passive09_0105, Passive09_0106, Passive09_0107, Passive09_0108, Passive09_0109, Passive09_0110, Passive09_0111,
    Passive09_0112, Passive09_0113, Passive09_0114, Passive09_0115, Passive09_0116, Passive09_0117, Passive09_0118,
    Passive09_0119, Passive09_0120, Passive09_0121, Passive09_0122, Passive09_0123, Passive09_0124, Passive09_0125,
    Passive09_0126, Passive09_0127, Passive09_0128, Passive09_0129, Passive09_0130, Passive09_0131
};
