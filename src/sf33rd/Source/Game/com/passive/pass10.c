/**
 * @file pass10.c
 * COM Passive: Yang
 */

#include "sf33rd/Source/Game/com/passive/pass10.h"
#include "sf33rd/Source/Game/com/passive/pass10_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive10_Tbl[138])();

void Passive10(PLW* wk) {
    Passive10_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive10_0000(PLW* wk) {
    pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive10_0001(PLW* wk) {
    pattern_lever_off_look(wk);
}

void Passive10_0002(PLW* wk) {
    pattern_wait_get_up_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 10, -1},
        &(EM_Term_Params){0x7fff, -1, 1, 1, -1}
    );
}

void Passive10_0003(PLW* wk) {
    pattern_vs_jump_guard(wk);
}

void Passive10_0004(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x70, 0x53, &(Command_Attack_Args){8, 1, 0xb, -1});
}

void Passive10_0005(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive10_0006(PLW* wk) {
    pattern_approach_walk_em_term(wk, 0x7f, &(EM_Term_Params){0x7fff, -1, 1, 1, -1});
}

void Passive10_0007(PLW* wk) {
    pattern_approach_walk_command_attack(wk, 0xbf, &(Command_Attack_Args){8, 0x1e, 9, -1});
}

void Passive10_0008(PLW* wk) {
    pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xc, 0, 0xb, -1},
        &(Command_Attack_Args){8, 0x1e, 8, -1}
    );
}

void Passive10_0009(PLW* wk) {
    pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1e, 9, -1}
    );
}

void Passive10_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, 0, 0, 2, 0});
        break;

    case 1:
        Short_Range_Attack(wk, &(Short_Range_Args){8, 0x40, 6, 0x1d});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0011(PLW* wk) {
    pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 2, -0x7FA0, -1, 0x8400});
}

void Passive10_0012(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F90, 0, 0, 2, 0}, 0x20);
}

void Passive10_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xffff, 0xffff, 0x30, 0});
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x2e, 0x2f, 0xffff, 0});
        break;

    case 3:
        Normal_Attack(wk, 0xb, 0x10);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        pattern_com_random_select_from_step_6(wk);
        break;
    }
}

void Passive10_0014(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FB0, -0x7FB0, 0, 1, -1}, 0x200);
}

void Passive10_0015(PLW* wk) {
    pattern_wait_get_up_2(wk, 0);
}

void Passive10_0016(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 2:
        Normal_Attack(wk, 0xb, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 4:
        Normal_Attack(wk, 0xc, 0x40);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1e, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive10_0017_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Normal_Attack(wk, 0xc, 0x40);
        break;

    case 7:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1e, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 6, 0x6c);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, 0x89, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 4:
        Normal_Attack(wk, 0xb, 0x100);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        passive10_0017_from_step_6(wk);
        break;
    }
}

void Passive10_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x41, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 0xb, 0x202);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0019(PLW* wk) {
    pattern_wait_get_up_2(wk, 0);
}

void Passive10_0020(PLW* wk) {
    pattern_wait_get_up_2(wk, 3);
}

void Passive10_0021(PLW* wk) {
    pattern_wait_get_up_command_attack(wk, 0, -1, &(Command_Attack_Args){8, 0x1e, 9, -1});
}

void Passive10_0022(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 0xb, 0x100, 0, -0x7FA0, -1, 0x40});
        break;

    case 2:
        Normal_Attack(wk, 0xb, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1e, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0023(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -1, 0xb, 0x8100, 0, -0x7FA0, -1, 0x40});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1e, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0024(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_4(wk, 0x70, 0x6c, 0x89);
}

void Passive10_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 6, 0x68);
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
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1e, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0026(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1}, 0x20);
}

void Passive10_0027(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1e, 8, -1});
}

void Passive10_0028(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 2, -0x7FA0, -1, 0x400});
}

void Passive10_0029(PLW* wk) {
    pattern_normal_attack_7(wk, 0x10, 0x10, 0x10);
}

void Passive10_0030(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1}, 0x400);
}

void Passive10_0031(PLW* wk) {
    pattern_walk(wk, 1);
}

void Passive10_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2e, 0x2f, 0xffff, 0});
        break;

    case 1:
        Normal_Attack(wk, 0xb, 0x22);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1f, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0033(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x110);
}

void Passive10_0034(PLW* wk) {
    pattern_normal_attack_command_attack(wk, 0xc, 0x20, &(Command_Attack_Args){8, 0x1e, 8, -1});
}

void Passive10_0035(PLW* wk) {
    pattern_normal_attack_4(wk, 8, 0xb, 0x202);
}

void Passive10_0036(PLW* wk) {
    pattern_normal_attack_command_attack_3(wk, 8, 0xb, &(Command_Attack_Args){8, 0x1e, 8, -1});
}

void Passive10_0037(PLW* wk) {
    pattern_em_term_lever_attack(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 0, 0x110);
}

void Passive10_0038(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1}, 0x110);
}

void Passive10_0039(PLW* wk) {
    pattern_etc_term_branch_unit_area(wk, 2, 0x42, &(Branch_Menu_Args){6, 0x59, 0x5a, 0x5a, 0x5b});
}

void Passive10_0040(PLW* wk) {
    pattern_em_term_normal_attack_7(wk, &(EM_Term_Params){-0x7FA0, -0x7FF8, 6, 1, -1}, 9, 0x400);
}

void Passive10_0041(PLW* wk) {
    pattern_em_term_lever_attack_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 8, 0x42);
}

void Passive10_0042(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 2, -0x7FA0, -1, -0x7C00});
}

void Passive10_0043(PLW* wk) {
    pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1e, 9, -1}
    );
}

void Passive10_0044(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x41,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1e, 10, -1}
    );
}

void Passive10_0045(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FE0, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x59, 0x5a, 0x5b, 0x5b}
    );
}

void Passive10_0046(PLW* wk) {
    pattern_keep_status(wk, -1);
}

void Passive10_0047(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xb, 0x102);
        break;

    case 1:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0x59, 0x5a, 0x5b, 0x5b});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive10_0048(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x102, 0x402);
}

void Passive10_0049(PLW* wk) {
    pattern_adjust_attack(wk, 0x10);
}

void (*const Passive10_Tbl[138])(PLW*) = {
    Passive10_0000, Passive10_0001, Passive10_0002, Passive10_0003, Passive10_0004, Passive10_0005, Passive10_0006,
    Passive10_0007, Passive10_0008, Passive10_0009, Passive10_0010, Passive10_0011, Passive10_0012, Passive10_0013,
    Passive10_0014, Passive10_0015, Passive10_0016, Passive10_0017, Passive10_0018, Passive10_0019, Passive10_0020,
    Passive10_0021, Passive10_0022, Passive10_0023, Passive10_0024, Passive10_0025, Passive10_0026, Passive10_0027,
    Passive10_0028, Passive10_0029, Passive10_0030, Passive10_0031, Passive10_0032, Passive10_0033, Passive10_0034,
    Passive10_0035, Passive10_0036, Passive10_0037, Passive10_0038, Passive10_0039, Passive10_0040, Passive10_0041,
    Passive10_0042, Passive10_0043, Passive10_0044, Passive10_0045, Passive10_0046, Passive10_0047, Passive10_0048,
    Passive10_0049, Passive10_0050, Passive10_0051, Passive10_0052, Passive10_0053, Passive10_0054, Passive10_0055,
    Passive10_0056, Passive10_0057, Passive10_0058, Passive10_0059, Passive10_0060, Passive10_0061, Passive10_0062,
    Passive10_0063, Passive10_0064, Passive10_0065, Passive10_0066, Passive10_0067, Passive10_0068, Passive10_0069,
    Passive10_0070, Passive10_0071, Passive10_0072, Passive10_0073, Passive10_0074, Passive10_0075, Passive10_0076,
    Passive10_0077, Passive10_0078, Passive10_0079, Passive10_0080, Passive10_0081, Passive10_0082, Passive10_0083,
    Passive10_0084, Passive10_0085, Passive10_0086, Passive10_0087, Passive10_0088, Passive10_0089, Passive10_0090,
    Passive10_0091, Passive10_0092, Passive10_0093, Passive10_0094, Passive10_0095, Passive10_0096, Passive10_0097,
    Passive10_0098, Passive10_0099, Passive10_0100, Passive10_0101, Passive10_0102, Passive10_0103, Passive10_0104,
    Passive10_0105, Passive10_0106, Passive10_0107, Passive10_0108, Passive10_0109, Passive10_0110, Passive10_0111,
    Passive10_0112, Passive10_0113, Passive10_0114, Passive10_0115, Passive10_0116, Passive10_0117, Passive10_0118,
    Passive10_0119, Passive10_0120, Passive10_0121, Passive10_0122, Passive10_0123, Passive10_0124, Passive10_0125,
    Passive10_0126, Passive10_0127, Passive10_0128, Passive10_0129, Passive10_0130, Passive10_0131, Passive10_0132,
    Passive10_0133, Passive10_0134, Passive10_0135, Passive10_0136, Passive10_0137
};
