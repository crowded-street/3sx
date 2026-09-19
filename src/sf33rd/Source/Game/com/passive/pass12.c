/**
 * @file pass12.c
 * COM Passive: Sean
 */

#include "sf33rd/Source/Game/com/passive/pass12.h"
#include "sf33rd/Source/Game/com/passive/pass12_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive12_Tbl[231])();

void passive12_pattern_approach_walk_normal_attack(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive12_pattern_approach_walk_j_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive12_pattern_approach_walk_lever_attack(PLW* wk, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, lever, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive12_pattern_wait_lie_approach_walk(PLW* wk, s16 target_pos) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Lie(wk, 0);
        break;

    case 1:
        Approach_Walk(wk, target_pos, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive12_pattern_command_attack_com_random_select(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x2D, 0xFF, 0xFF, 0xFF}, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12(PLW* wk) {
    Passive12_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive12_0000(PLW* wk) {
    active_pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive12_0001(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Passive12_0002(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 0xB, 0x220, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive12_0003(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, 0x28, 7, 1, -1},
        &(Command_Attack_Args){8, 0x1D, 8, -1}
    );
}

void Passive12_0004(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1},
        &(SA_Term_Args){0xFFFF, 0x31, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0005(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x220);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0006(PLW* wk) {
    pattern_approach_walk_em_term_lever_attack(wk, 0x37, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 0);
}

void Passive12_0007(PLW* wk) {
    pattern_approach_walk_em_term_lever_attack(wk, 0x37, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 1);
}

void Passive12_0008(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x37, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 0x110);
}

void Passive12_0009(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0010(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_3(
        wk,
        0x37,
        &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 0x4008, -1}
    );
}

void Passive12_0011(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Passive12_0012(PLW* wk) {
    passive12_pattern_approach_walk_normal_attack(wk, 8, 0x20);
}

void Passive12_0013(PLW* wk) {
    passive12_pattern_approach_walk_normal_attack(wk, 8, 0x40);
}

void Passive12_0014(PLW* wk) {
    passive12_pattern_approach_walk_normal_attack(wk, 8, 0x100);
}

void Passive12_0015(PLW* wk) {
    passive12_pattern_approach_walk_normal_attack(wk, 8, 0x200);
}

void Passive12_0016(PLW* wk) {
    passive12_pattern_approach_walk_normal_attack(wk, 8, 0x400);
}

void Passive12_0017(PLW* wk) {
    passive12_pattern_approach_walk_normal_attack(wk, 8, 0x402);
}

void Passive12_0018(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1}, 0x402);
}

void Passive12_0019(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 9, 0x20, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive12_0020(PLW* wk) {
    passive12_pattern_approach_walk_lever_attack(wk, 0, 0x110);
}

void Passive12_0021(PLW* wk) {
    passive12_pattern_approach_walk_lever_attack(wk, 1, 0x110);
}

void Passive12_0022(PLW* wk) {
    passive12_pattern_approach_walk_normal_attack(wk, 8, 0x110);
}

void Passive12_0023(PLW* wk) {
    passive12_pattern_approach_walk_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive12_0024(PLW* wk) {
    passive12_pattern_approach_walk_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive12_0025(PLW* wk) {
    passive12_pattern_approach_walk_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive12_0026(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Passive12_0027(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Passive12_0028(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x20, 10, -1});
}

void Passive12_0029(PLW* wk) {
    pattern_pierce_on_j_command_attack(
        wk,
        &(Command_Attack_Args){0xB, 0x20, 10, -1},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0030(PLW* wk) {
    pattern_approach_walk_normal_attack_lever_attack_2(wk, 0x37, 0);
}

void Passive12_0031(PLW* wk) {
    pattern_approach_walk_normal_attack_lever_attack(wk, 0x37, 0, 0x200);
}

void Passive12_0032(PLW* wk) {
    pattern_approach_walk_normal_attack(wk, 0x37);
}

void Passive12_0033(PLW* wk) {
    passive12_pattern_approach_walk_lever_attack(wk, 0, 0x200);
}

void Passive12_0034(PLW* wk) {
    active_pattern_approach_walk_sa_term_j_command_attack(
        wk,
        0x37,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0035(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0x4008, -1});
}

void Passive12_0036(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0x4009, -1});
}

void Passive12_0037(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0x400A, -1});
}

void Passive12_0038(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1D, 8, -1}
    );
}

void Passive12_0039(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1D, 9, -1}
    );
}

void Passive12_0040(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0041(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(SA_Term_Args){0x30, 0x31, 0x32, 0},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive12_0042(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive12_0043(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x20, 10, -1, -1, 0x30, 0, -1, -1, 0xFFFF});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive12_0044(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, 0x38, 8, 0x400, 2, -1, -1, -1});
}

void Passive12_0045(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive12_0046(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive12_0047(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive12_0048(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x10);
}

void Passive12_0049(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x100);
}

void Passive12_0050(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 9, 0x10, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive12_0051(PLW* wk) {
    pattern_normal_attack_j_command_attack_3(wk, 0x100, 0x100, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive12_0052(PLW* wk) {
    active_pattern_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1D, 9, -1}
    );
}

void Passive12_0053(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x20,
        &(Command_Attack_Args){8, 0x20, 9, -1}
    );
}

void Passive12_0054(PLW* wk) {
    pattern_wait_get_up_normal_attack_3(wk, 3, &(Normal_Attack_Step){ 9, 0x102 }, 0x202);
}

void Passive12_0055(PLW* wk) {
    pattern_wait_get_up_lever_attack_2(wk, 0, 0x200);
}

void Passive12_0056(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term_2(
        wk,
        &(SA_Term_Args){0x30, 0xFFFF, 0x32, 0},
        &(Command_Attack_Args){8, 0x20, 0xA, -1}
    );
}

void Passive12_0057(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term_3(
        wk,
        &(SA_Term_Args){0x30, 0xFFFF, 0x32, 0},
        &(Command_Attack_Args){8, 0x20, 0xA, -1}
    );
}

void Passive12_0058(PLW* wk) {
    pattern_wait_get_up_lever_attack_2(wk, 0, 0x110);
}

void Passive12_0059(PLW* wk) {
    pattern_wait_get_up_lever_attack_2(wk, 1, 0x110);
}

void Passive12_0060(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 0, 0x110);
}

void Passive12_0061(PLW* wk) {
    passive12_pattern_wait_lie_approach_walk(wk, 0x37);
}

void Passive12_0062(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 3, 0x402);
}

void (*const Passive12_Tbl[231])(PLW*) = {
    Passive12_0000, Passive12_0001, Passive12_0002, Passive12_0003, Passive12_0004, Passive12_0005, Passive12_0006,
    Passive12_0007, Passive12_0008, Passive12_0009, Passive12_0010, Passive12_0011, Passive12_0012, Passive12_0013,
    Passive12_0014, Passive12_0015, Passive12_0016, Passive12_0017, Passive12_0018, Passive12_0019, Passive12_0020,
    Passive12_0021, Passive12_0022, Passive12_0023, Passive12_0024, Passive12_0025, Passive12_0026, Passive12_0027,
    Passive12_0028, Passive12_0029, Passive12_0030, Passive12_0031, Passive12_0032, Passive12_0033, Passive12_0034,
    Passive12_0035, Passive12_0036, Passive12_0037, Passive12_0038, Passive12_0039, Passive12_0040, Passive12_0041,
    Passive12_0042, Passive12_0043, Passive12_0044, Passive12_0045, Passive12_0046, Passive12_0047, Passive12_0048,
    Passive12_0049, Passive12_0050, Passive12_0051, Passive12_0052, Passive12_0053, Passive12_0054, Passive12_0055,
    Passive12_0056, Passive12_0057, Passive12_0058, Passive12_0059, Passive12_0060, Passive12_0061, Passive12_0062,
    Passive12_0063, Passive12_0064, Passive12_0065, Passive12_0066, Passive12_0067, Passive12_0068, Passive12_0069,
    Passive12_0070, Passive12_0071, Passive12_0072, Passive12_0073, Passive12_0074, Passive12_0075, Passive12_0076,
    Passive12_0077, Passive12_0078, Passive12_0079, Passive12_0080, Passive12_0081, Passive12_0082, Passive12_0083,
    Passive12_0084, Passive12_0085, Passive12_0086, Passive12_0087, Passive12_0088, Passive12_0089, Passive12_0090,
    Passive12_0091, Passive12_0092, Passive12_0093, Passive12_0094, Passive12_0095, Passive12_0096, Passive12_0097,
    Passive12_0098, Passive12_0099, Passive12_0100, Passive12_0101, Passive12_0102, Passive12_0103, Passive12_0104,
    Passive12_0105, Passive12_0106, Passive12_0107, Passive12_0108, Passive12_0109, Passive12_0110, Passive12_0111,
    Passive12_0112, Passive12_0113, Passive12_0114, Passive12_0115, Passive12_0116, Passive12_0117, Passive12_0118,
    Passive12_0119, Passive12_0120, Passive12_0121, Passive12_0122, Passive12_0123, Passive12_0124, Passive12_0125,
    Passive12_0126, Passive12_0127, Passive12_0128, Passive12_0129, Passive12_0130, Passive12_0131, Passive12_0132,
    Passive12_0133, Passive12_0134, Passive12_0135, Passive12_0136, Passive12_0137, Passive12_0138, Passive12_0139,
    Passive12_0140, Passive12_0141, Passive12_0142, Passive12_0143, Passive12_0144, Passive12_0145, Passive12_0146,
    Passive12_0147, Passive12_0148, Passive12_0149, Passive12_0150, Passive12_0151, Passive12_0152, Passive12_0153,
    Passive12_0154, Passive12_0155, Passive12_0156, Passive12_0157, Passive12_0158, Passive12_0159, Passive12_0160,
    Passive12_0161, Passive12_0162, Passive12_0163, Passive12_0164, Passive12_0165, Passive12_0166, Passive12_0167,
    Passive12_0168, Passive12_0169, Passive12_0170, Passive12_0171, Passive12_0172, Passive12_0173, Passive12_0174,
    Passive12_0175, Passive12_0176, Passive12_0177, Passive12_0178, Passive12_0179, Passive12_0180, Passive12_0181,
    Passive12_0182, Passive12_0183, Passive12_0184, Passive12_0185, Passive12_0186, Passive12_0187, Passive12_0188,
    Passive12_0189, Passive12_0190, Passive12_0191, Passive12_0192, Passive12_0193, Passive12_0194, Passive12_0195,
    Passive12_0196, Passive12_0197, Passive12_0198, Passive12_0199, Passive12_0200, Passive12_0201, Passive12_0202,
    Passive12_0203, Passive12_0204, Passive12_0205, Passive12_0206, Passive12_0207, Passive12_0208, Passive12_0209,
    Passive12_0210, Passive12_0211, Passive12_0212, Passive12_0213, Passive12_0214, Passive12_0215, Passive12_0216,
    Passive12_0217, Passive12_0218, Passive12_0219, Passive12_0220, Passive12_0221, Passive12_0222, Passive12_0223,
    Passive12_0224, Passive12_0225, Passive12_0226, Passive12_0227, Passive12_0228, Passive12_0229, Passive12_0230
};
