/**
 * @file pass08.c
 * COM Passive: Elena
 */

#include "sf33rd/Source/Game/com/passive/pass08.h"
#include "sf33rd/Source/Game/com/passive/pass08_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive08_Tbl[214])();

static void passive08_pattern_approach_walk_lever_attack(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive08_pattern_approach_walk_normal_attack(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive08_pattern_lever_attack(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive08_pattern_wait_lie_approach_walk_wait_get_up(PLW* wk, s16 target_pos, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Lie(wk, 0);
        break;

    case 1:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 2:
        Wait_Get_Up(wk, lever_data, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive08_pattern_pierce_on_j_command_attack_search_back_term(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    case 2:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, -1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08(PLW* wk) {
    Passive08_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive08_0000(PLW* wk) {
    active_pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive08_0001(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Passive08_0002(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x402);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){9, 0x1E, 8, -1});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0003(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive08_0004(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1},
        &(SA_Term_Args){0xFFFF, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive08_0005(PLW* wk) {
    pattern_search_back_term_jump(wk, 0x50);
}

void Passive08_0006(PLW* wk) {
    pattern_approach_walk_em_term_lever_attack(wk, 0x3F, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 0);
}

void Passive08_0007(PLW* wk) {
    pattern_approach_walk_em_term_lever_attack(wk, 0x3F, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 0);
}

void Passive08_0008(PLW* wk) {
    pattern_search_back_term_command_attack_2(wk, &(Command_Attack_Args){8, 1, -1, -1});
}

void Passive08_0009(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive08_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0011(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Passive08_0012(PLW* wk) {
    passive08_pattern_approach_walk_lever_attack(wk, 0x200);
}

void Passive08_0013(PLW* wk) {
    passive08_pattern_approach_walk_normal_attack(wk, 8, 0x40);
}

void Passive08_0014(PLW* wk) {
    pattern_approach_walk_normal_attack_3(wk, 0x3F, 9, 0x100);
}

void Passive08_0015(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0016(PLW* wk) {
    passive08_pattern_approach_walk_normal_attack(wk, 8, 0x400);
}

void Passive08_0017(PLW* wk) {
    passive08_pattern_approach_walk_normal_attack(wk, 8, 0x402);
}

void Passive08_0018(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB8, 6, 1, -1}, 0x402);
}

void Passive08_0019(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 9, 0x20, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive08_0020(PLW* wk) {
    passive08_pattern_approach_walk_lever_attack(wk, 0x110);
}

void Passive08_0021(PLW* wk) {
    passive08_pattern_approach_walk_lever_attack(wk, 0x400);
}

void Passive08_0022(PLW* wk) {
    passive08_pattern_lever_attack(wk, 0x402);
}

void Passive08_0023(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0024(PLW* wk) {
    pattern_approach_walk_j_command_attack(wk, 0x3F, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive08_0025(PLW* wk) {
    pattern_approach_walk_j_command_attack(wk, 0x3F, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive08_0026(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive08_0027(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive08_0028(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive08_0029(PLW* wk) {
    pattern_pierce_on_j_command_attack(
        wk,
        &(Command_Attack_Args){8, 0x1E, 9, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive08_0030(PLW* wk) {
    pattern_approach_walk_normal_attack_j_command_attack_3(
        wk,
        0x3F,
        &(Normal_Attack_Step){ 9, 0x202 },
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive08_0031(PLW* wk) {
    pattern_approach_walk_normal_attack_lever_attack(wk, 0x3F, 0, 0x20);
}

void Passive08_0032(PLW* wk) {
    pattern_approach_walk_lever_attack_2(wk, 0x3F, 0x20);
}

void Passive08_0033(PLW* wk) {
    passive08_pattern_approach_walk_lever_attack(wk, 0x20);
}

void Passive08_0034(PLW* wk) {
    active_pattern_approach_walk_sa_term_j_command_attack(
        wk,
        0x3F,
        &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive08_0035(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive08_0036(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive08_0037(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive08_0038(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive08_0039(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive08_0040(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive08_0041(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(SA_Term_Args){0x39, 0xFFFF, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive08_0042(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive08_0043(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, 0x40, 8, 0x100, 0, -0x7FC0, -1, 0x200});
}

void Passive08_0044(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, 0x40, 8, 0x400, 0, -0x7FC0, -1, 0x200});
}

void Passive08_0045(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive08_0046(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive08_0047(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive08_0048(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x10);
}

void Passive08_0049(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x100, 0x100);
}

void Passive08_0050(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 9, 0x10, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive08_0051(PLW* wk) {
    pattern_normal_attack_j_command_attack_3(wk, 0x102, 0x100, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive08_0052(PLW* wk) {
    active_pattern_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive08_0053(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x400,
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive08_0054(PLW* wk) {
    pattern_wait_get_up_normal_attack_3(wk, 3, &(Normal_Attack_Step){ 9, 0x102 }, 0x202);
}

void Passive08_0055(PLW* wk) {
    pattern_wait_get_up_lever_attack(wk, 0, 0, 0x20);
}

void passive08_0056_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, -1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Passive08_Tbl[214])(PLW*) = {
    Passive08_0000, Passive08_0001, Passive08_0002, Passive08_0003, Passive08_0004, Passive08_0005, Passive08_0006,
    Passive08_0007, Passive08_0008, Passive08_0009, Passive08_0010, Passive08_0011, Passive08_0012, Passive08_0013,
    Passive08_0014, Passive08_0015, Passive08_0016, Passive08_0017, Passive08_0018, Passive08_0019, Passive08_0020,
    Passive08_0021, Passive08_0022, Passive08_0023, Passive08_0024, Passive08_0025, Passive08_0026, Passive08_0027,
    Passive08_0028, Passive08_0029, Passive08_0030, Passive08_0031, Passive08_0032, Passive08_0033, Passive08_0034,
    Passive08_0035, Passive08_0036, Passive08_0037, Passive08_0038, Passive08_0039, Passive08_0040, Passive08_0041,
    Passive08_0042, Passive08_0043, Passive08_0044, Passive08_0045, Passive08_0046, Passive08_0047, Passive08_0048,
    Passive08_0049, Passive08_0050, Passive08_0051, Passive08_0052, Passive08_0053, Passive08_0054, Passive08_0055,
    Passive08_0056, Passive08_0057, Passive08_0058, Passive08_0059, Passive08_0060, Passive08_0061, Passive08_0062,
    Passive08_0063, Passive08_0064, Passive08_0065, Passive08_0066, Passive08_0067, Passive08_0068, Passive08_0069,
    Passive08_0070, Passive08_0071, Passive08_0072, Passive08_0073, Passive08_0074, Passive08_0075, Passive08_0076,
    Passive08_0077, Passive08_0078, Passive08_0079, Passive08_0080, Passive08_0081, Passive08_0082, Passive08_0083,
    Passive08_0084, Passive08_0085, Passive08_0086, Passive08_0087, Passive08_0088, Passive08_0089, Passive08_0090,
    Passive08_0091, Passive08_0092, Passive08_0093, Passive08_0094, Passive08_0095, Passive08_0096, Passive08_0097,
    Passive08_0098, Passive08_0099, Passive08_0100, Passive08_0101, Passive08_0102, Passive08_0103, Passive08_0104,
    Passive08_0105, Passive08_0106, Passive08_0107, Passive08_0108, Passive08_0109, Passive08_0110, Passive08_0111,
    Passive08_0112, Passive08_0113, Passive08_0114, Passive08_0115, Passive08_0116, Passive08_0117, Passive08_0118,
    Passive08_0119, Passive08_0120, Passive08_0121, Passive08_0122, Passive08_0123, Passive08_0124, Passive08_0125,
    Passive08_0126, Passive08_0127, Passive08_0128, Passive08_0129, Passive08_0130, Passive08_0131, Passive08_0132,
    Passive08_0133, Passive08_0134, Passive08_0135, Passive08_0136, Passive08_0137, Passive08_0138, Passive08_0139,
    Passive08_0140, Passive08_0141, Passive08_0142, Passive08_0143, Passive08_0144, Passive08_0145, Passive08_0146,
    Passive08_0147, Passive08_0148, Passive08_0149, Passive08_0150, Passive08_0151, Passive08_0152, Passive08_0153,
    Passive08_0154, Passive08_0155, Passive08_0156, Passive08_0157, Passive08_0158, Passive08_0159, Passive08_0160,
    Passive08_0161, Passive08_0162, Passive08_0163, Passive08_0164, Passive08_0165, Passive08_0166, Passive08_0167,
    Passive08_0168, Passive08_0169, Passive08_0170, Passive08_0171, Passive08_0172, Passive08_0173, Passive08_0174,
    Passive08_0175, Passive08_0176, Passive08_0177, Passive08_0178, Passive08_0179, Passive08_0180, Passive08_0181,
    Passive08_0182, Passive08_0183, Passive08_0184, Passive08_0185, Passive08_0186, Passive08_0187, Passive08_0188,
    Passive08_0189, Passive08_0190, Passive08_0191, Passive08_0192, Passive08_0193, Passive08_0194, Passive08_0195,
    Passive08_0196, Passive08_0197, Passive08_0198, Passive08_0199, Passive08_0200, Passive08_0201, Passive08_0202,
    Passive08_0203, Passive08_0204, Passive08_0205, Passive08_0206, Passive08_0207, Passive08_0208, Passive08_0209,
    Passive08_0210, Passive08_0211, Passive08_0212, Passive08_0213
};
