/**
 * @file pass07.c
 * COM Passive: Ibuki
 */

#include "sf33rd/Source/Game/com/passive/pass07.h"
#include "sf33rd/Source/Game/com/passive/pass07_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive07_Tbl[214])();

static void passive07_pattern_approach_walk_lever_attack(PLW* wk, s16 target_pos, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive07_pattern_keep_away_wait_get_up(PLW* wk, u16 lever_data, s16 option) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xC7, 0);
        break;

    case 1:
        Wait_Get_Up(wk, lever_data, option);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive07_pattern_approach_walk_normal_attack(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive07_pattern_wait_get_up_j_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07(PLW* wk) {
    Passive07_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive07_0000(PLW* wk) {
    pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive07_0001(PLW* wk) {
    pattern_lever_off_look(wk);
}

void Passive07_0002(PLW* wk) {
    pattern_wait(wk, 0);
}

void Passive07_0003(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive07_0004(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FA0, 0x28, 7, 1, -1}, 0x400);
}

void Passive07_0005(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 1:
        Jump(wk, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0006(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        Lever_Attack(wk, 8, 1, 0x110);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0007(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0008(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0009(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive07_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 4:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0011(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB8, 6, 1, -1}, 0x402);
}

void Passive07_0012(PLW* wk) {
    pattern_search_back_term_command_attack_2(wk, &(Command_Attack_Args){8, 1, -1, -1});
}

void Passive07_0013(PLW* wk) {
    passive07_pattern_approach_walk_normal_attack(wk, 8, 0x20);
}

void Passive07_0014(PLW* wk) {
    passive07_pattern_approach_walk_normal_attack(wk, 8, 0x200);
}

void Passive07_0015(PLW* wk) {
    pattern_approach_walk_normal_attack_3(wk, 0x3B, 9, 0x100);
}

void Passive07_0016(PLW* wk) {
    passive07_pattern_approach_walk_lever_attack(wk, 0x78, 0x200);
}

void Passive07_0017(PLW* wk) {
    passive07_pattern_approach_walk_normal_attack(wk, 8, 0x402);
}

void Passive07_0018(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x20, 10, -1});
}

void Passive07_0019(PLW* wk) {
    pattern_normal_attack_j_command_attack_2(wk, 9, 0x20, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive07_0020(PLW* wk) {
    passive07_pattern_approach_walk_lever_attack(wk, 0x3B, 0x110);
}

void Passive07_0021(PLW* wk) {
    passive07_pattern_approach_walk_lever_attack(wk, 0x3B, 0x400);
}

void Passive07_0022(PLW* wk) {
    passive07_pattern_approach_walk_lever_attack(wk, 0x3B, 0x202);
}

void Passive07_0023(PLW* wk) {
    pattern_approach_walk_j_command_attack(wk, 0x3B, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive07_0024(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x100);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0025(PLW* wk) {
    pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 10, -1, -1, 0x40, 0, -1, -1, 0xFFFF});
}

void Passive07_0026(PLW* wk) {
    pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 10, -1, -1, 0x40, 1, -1, -1, 0xFFFF});
}

void Passive07_0027(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x2E);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x2E, 10, -1, -1, 0x40, 1, -1, -1, 0xFFFF});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0028(PLW* wk) {
    pattern_end_immediately(wk);
}

void Passive07_0029(PLW* wk) {
    pattern_pierce_on_j_command_attack(
        wk,
        &(Command_Attack_Args){8, 0x1E, 10, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive07_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 2:
        Lever_Attack(wk, 8, 0x1E, 10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0031(PLW* wk) {
    pattern_approach_walk_normal_attack_lever_attack(wk, 0x3B, 0, 0x400);
}

void Passive07_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3B, 2);
        break;

    case 1:
        Lever_Attack(wk, 0xB, 0, 0x200);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0033(PLW* wk) {
    passive07_pattern_approach_walk_lever_attack(wk, 0x3B, 0x200);
}

void Passive07_0034(PLW* wk) {
    pattern_approach_walk_sa_term_j_command_attack(
        wk,
        0x3B,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive07_0035(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive07_0036(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive07_0037(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive07_0038(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive07_0039(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive07_0040(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive07_0041(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(SA_Term_Args){0x35, 0xFFFF, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive07_0042(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive07_0043(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, 0x40, 8, 0x200, 0, -0x7FC0, -1, 0x200});
}

void Passive07_0044(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x400, 0, -0x7FC0, -1, 0x200});
}

void Passive07_0045(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive07_0046(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive07_0047(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive07_0048(PLW* wk) {
    pattern_em_term_normal_attack_5(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1});
}

void Passive07_0049(PLW* wk) {
    pattern_em_term_normal_attack_4(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1});
}

void Passive07_0050(PLW* wk) {
    pattern_normal_attack_j_command_attack_2(wk, 9, 0x10, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Passive07_0051(PLW* wk) {
    pattern_normal_attack_j_command_attack_3(wk, 0x100, 0x102, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive07_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 2);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, 0x700});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive07_0053(PLW* wk) {
    pattern_jump_attack_term_normal_attack_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x400,
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive07_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
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

void (*const Passive07_Tbl[214])(PLW*) = {
    Passive07_0000, Passive07_0001, Passive07_0002, Passive07_0003, Passive07_0004, Passive07_0005, Passive07_0006,
    Passive07_0007, Passive07_0008, Passive07_0009, Passive07_0010, Passive07_0011, Passive07_0012, Passive07_0013,
    Passive07_0014, Passive07_0015, Passive07_0016, Passive07_0017, Passive07_0018, Passive07_0019, Passive07_0020,
    Passive07_0021, Passive07_0022, Passive07_0023, Passive07_0024, Passive07_0025, Passive07_0026, Passive07_0027,
    Passive07_0028, Passive07_0029, Passive07_0030, Passive07_0031, Passive07_0032, Passive07_0033, Passive07_0034,
    Passive07_0035, Passive07_0036, Passive07_0037, Passive07_0038, Passive07_0039, Passive07_0040, Passive07_0041,
    Passive07_0042, Passive07_0043, Passive07_0044, Passive07_0045, Passive07_0046, Passive07_0047, Passive07_0048,
    Passive07_0049, Passive07_0050, Passive07_0051, Passive07_0052, Passive07_0053, Passive07_0054, Passive07_0055,
    Passive07_0056, Passive07_0057, Passive07_0058, Passive07_0059, Passive07_0060, Passive07_0061, Passive07_0062,
    Passive07_0063, Passive07_0064, Passive07_0065, Passive07_0066, Passive07_0067, Passive07_0068, Passive07_0069,
    Passive07_0070, Passive07_0071, Passive07_0072, Passive07_0073, Passive07_0074, Passive07_0075, Passive07_0076,
    Passive07_0077, Passive07_0078, Passive07_0079, Passive07_0080, Passive07_0081, Passive07_0082, Passive07_0083,
    Passive07_0084, Passive07_0085, Passive07_0086, Passive07_0087, Passive07_0088, Passive07_0089, Passive07_0090,
    Passive07_0091, Passive07_0092, Passive07_0093, Passive07_0094, Passive07_0095, Passive07_0096, Passive07_0097,
    Passive07_0098, Passive07_0099, Passive07_0100, Passive07_0101, Passive07_0102, Passive07_0103, Passive07_0104,
    Passive07_0105, Passive07_0106, Passive07_0107, Passive07_0108, Passive07_0109, Passive07_0110, Passive07_0111,
    Passive07_0112, Passive07_0113, Passive07_0114, Passive07_0115, Passive07_0116, Passive07_0117, Passive07_0118,
    Passive07_0119, Passive07_0120, Passive07_0121, Passive07_0122, Passive07_0123, Passive07_0124, Passive07_0125,
    Passive07_0126, Passive07_0127, Passive07_0128, Passive07_0129, Passive07_0130, Passive07_0131, Passive07_0132,
    Passive07_0133, Passive07_0134, Passive07_0135, Passive07_0136, Passive07_0137, Passive07_0138, Passive07_0139,
    Passive07_0140, Passive07_0141, Passive07_0142, Passive07_0143, Passive07_0144, Passive07_0145, Passive07_0146,
    Passive07_0147, Passive07_0148, Passive07_0149, Passive07_0150, Passive07_0151, Passive07_0152, Passive07_0153,
    Passive07_0154, Passive07_0155, Passive07_0156, Passive07_0157, Passive07_0158, Passive07_0159, Passive07_0160,
    Passive07_0161, Passive07_0162, Passive07_0163, Passive07_0164, Passive07_0165, Passive07_0166, Passive07_0167,
    Passive07_0168, Passive07_0169, Passive07_0170, Passive07_0171, Passive07_0172, Passive07_0173, Passive07_0174,
    Passive07_0175, Passive07_0176, Passive07_0177, Passive07_0178, Passive07_0179, Passive07_0180, Passive07_0181,
    Passive07_0182, Passive07_0183, Passive07_0184, Passive07_0185, Passive07_0186, Passive07_0187, Passive07_0188,
    Passive07_0189, Passive07_0190, Passive07_0191, Passive07_0192, Passive07_0193, Passive07_0194, Passive07_0195,
    Passive07_0196, Passive07_0197, Passive07_0198, Passive07_0199, Passive07_0200, Passive07_0201, Passive07_0202,
    Passive07_0203, Passive07_0204, Passive07_0205, Passive07_0206, Passive07_0207, Passive07_0208, Passive07_0209,
    Passive07_0210, Passive07_0211, Passive07_0212, Passive07_0213
};
