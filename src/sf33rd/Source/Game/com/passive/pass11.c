/**
 * @file pass11.c
 * COM Passive: Ken
 */

#include "sf33rd/Source/Game/com/passive/pass11.h"
#include "sf33rd/Source/Game/com/passive/pass11_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive11_Tbl[255])();

void passive11_pattern_em_term_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 1:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive11_pattern_approach_walk_em_term_normal_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive11_pattern_approach_walk_em_term_lever_attack(PLW* wk, u16 lever) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        Lever_Attack(wk, 8, lever, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive11_pattern_approach_walk_j_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive11_pattern_command_attack_com_random_select(
    PLW* wk, const Command_Attack_Args* p, const Branch_Menu_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    case 1:
        Com_Random_Select(wk, p_b, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive11_pattern_approach_walk_em_term_sa_term(PLW* wk, u16 lever) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    case 3:
        Lever_Attack(wk, 8, lever, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive11_pattern_em_term_sa_term_normal_attack(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    case 2:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11(PLW* wk) {
    Passive11_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive11_0000(PLW* wk) {
    pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive11_0001(PLW* wk) {
    pattern_lever_off_look(wk);
}

void Passive11_0002(PLW* wk) {
    pattern_etc_term_provoke_next_another_menu(wk, 0x36, 0x37);
}

void Passive11_0003(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_5(
        wk,
        0x47,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 10, 0x70}
    );
}

void Passive11_0004(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F70, 0x28, 7, 1, -1},
        &(SA_Term_Args){0xFFFF, 0x31, 0xFFFF, 0x47},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0005(PLW* wk) {
    passive11_pattern_approach_walk_em_term_normal_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, 0x70});
}

void Passive11_0006(PLW* wk) {
    passive11_pattern_approach_walk_em_term_lever_attack(wk, 0);
}

void Passive11_0007(PLW* wk) {
    passive11_pattern_approach_walk_em_term_lever_attack(wk, 1);
}

void Passive11_0008(PLW* wk) {
    passive11_pattern_approach_walk_em_term_lever_attack(wk, 0xFFFF);
}

void Passive11_0009(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FE0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0010(PLW* wk) {
    passive11_pattern_approach_walk_em_term_normal_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive11_0011(PLW* wk) {
    pattern_wait(wk, 0);
}

void Passive11_0012(PLW* wk) {
    pattern_approach_walk_normal_attack_2(wk, 8, 0x20);
}

void Passive11_0013(PLW* wk) {
    pattern_approach_walk_normal_attack_2(wk, 8, 0x40);
}

void Passive11_0014(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, 0x70}
    );
}

void Passive11_0015(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Normal_Attack_SP(wk, 8, 0x200, 0x12);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0016(PLW* wk) {
    pattern_approach_walk_normal_attack_2(wk, 8, 0x400);
}

void Passive11_0017(PLW* wk) {
    pattern_approach_walk_normal_attack_2(wk, 8, 0x402);
}

void Passive11_0018(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1}, 8, 0x402);
}

void Passive11_0019(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x20);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0020(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x37, 0, 0x110);
}

void Passive11_0021(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x37, 1, 0x110);
}

void Passive11_0022(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x37, 0xFFFF, 0x110);
}

void Passive11_0023(PLW* wk) {
    passive11_pattern_approach_walk_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive11_0024(PLW* wk) {
    passive11_pattern_approach_walk_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive11_0025(PLW* wk) {
    passive11_pattern_approach_walk_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive11_0026(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive11_0027(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive11_0028(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive11_0029(PLW* wk) {
    pattern_pierce_on_j_command_attack(
        wk,
        &(Command_Attack_Args){0xB, 0x1E, 10, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 2:
        Lever_Attack(wk, 8, 1, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0031(PLW* wk) {
    pattern_approach_walk_normal_attack_lever_attack(wk, 0x47, 1, 0x200);
}

void Passive11_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x20);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0033(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x47, 1, 0x200);
}

void Passive11_0034(PLW* wk) {
    pattern_approach_walk_sa_term_j_command_attack(
        wk,
        0x47,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0035(PLW* wk) {
    passive11_pattern_em_term_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive11_0036(PLW* wk) {
    passive11_pattern_em_term_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive11_0037(PLW* wk) {
    passive11_pattern_em_term_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive11_0038(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive11_0039(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive11_0040(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0041(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0042(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive11_0043(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, 0x70}
    );
}

void Passive11_0044(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, 0x38, 8, 0x400, 2, -1, -1, -1});
}

void Passive11_0045(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive11_0046(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive11_0047(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0048(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1}, 8, 0x10);
}

void Passive11_0049(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x100);
}

void Passive11_0050(PLW* wk) {
    pattern_normal_attack_j_command_attack_2(wk, 9, 0x10, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive11_0051(PLW* wk) {
    pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -1, -1, 0xFFFF});
}

void (*const Passive11_Tbl[255])(PLW*) = {
    Passive11_0000, Passive11_0001, Passive11_0002, Passive11_0003, Passive11_0004, Passive11_0005, Passive11_0006,
    Passive11_0007, Passive11_0008, Passive11_0009, Passive11_0010, Passive11_0011, Passive11_0012, Passive11_0013,
    Passive11_0014, Passive11_0015, Passive11_0016, Passive11_0017, Passive11_0018, Passive11_0019, Passive11_0020,
    Passive11_0021, Passive11_0022, Passive11_0023, Passive11_0024, Passive11_0025, Passive11_0026, Passive11_0027,
    Passive11_0028, Passive11_0029, Passive11_0030, Passive11_0031, Passive11_0032, Passive11_0033, Passive11_0034,
    Passive11_0035, Passive11_0036, Passive11_0037, Passive11_0038, Passive11_0039, Passive11_0040, Passive11_0041,
    Passive11_0042, Passive11_0043, Passive11_0044, Passive11_0045, Passive11_0046, Passive11_0047, Passive11_0048,
    Passive11_0049, Passive11_0050, Passive11_0051, Passive11_0052, Passive11_0053, Passive11_0054, Passive11_0055,
    Passive11_0056, Passive11_0057, Passive11_0058, Passive11_0059, Passive11_0060, Passive11_0061, Passive11_0062,
    Passive11_0063, Passive11_0064, Passive11_0065, Passive11_0066, Passive11_0067, Passive11_0068, Passive11_0069,
    Passive11_0070, Passive11_0071, Passive11_0072, Passive11_0073, Passive11_0074, Passive11_0075, Passive11_0076,
    Passive11_0077, Passive11_0078, Passive11_0079, Passive11_0080, Passive11_0081, Passive11_0082, Passive11_0083,
    Passive11_0084, Passive11_0085, Passive11_0086, Passive11_0087, Passive11_0088, Passive11_0089, Passive11_0090,
    Passive11_0091, Passive11_0092, Passive11_0093, Passive11_0094, Passive11_0095, Passive11_0096, Passive11_0097,
    Passive11_0098, Passive11_0099, Passive11_0100, Passive11_0101, Passive11_0102, Passive11_0103, Passive11_0104,
    Passive11_0105, Passive11_0106, Passive11_0107, Passive11_0108, Passive11_0109, Passive11_0110, Passive11_0111,
    Passive11_0112, Passive11_0113, Passive11_0114, Passive11_0115, Passive11_0116, Passive11_0117, Passive11_0118,
    Passive11_0119, Passive11_0120, Passive11_0121, Passive11_0122, Passive11_0123, Passive11_0124, Passive11_0125,
    Passive11_0126, Passive11_0127, Passive11_0128, Passive11_0129, Passive11_0130, Passive11_0131, Passive11_0132,
    Passive11_0133, Passive11_0134, Passive11_0135, Passive11_0136, Passive11_0137, Passive11_0138, Passive11_0139,
    Passive11_0140, Passive11_0141, Passive11_0142, Passive11_0143, Passive11_0144, Passive11_0145, Passive11_0146,
    Passive11_0147, Passive11_0148, Passive11_0149, Passive11_0150, Passive11_0151, Passive11_0152, Passive11_0153,
    Passive11_0154, Passive11_0155, Passive11_0156, Passive11_0157, Passive11_0158, Passive11_0159, Passive11_0160,
    Passive11_0161, Passive11_0162, Passive11_0163, Passive11_0164, Passive11_0165, Passive11_0166, Passive11_0167,
    Passive11_0168, Passive11_0169, Passive11_0170, Passive11_0171, Passive11_0172, Passive11_0173, Passive11_0174,
    Passive11_0175, Passive11_0176, Passive11_0177, Passive11_0178, Passive11_0179, Passive11_0180, Passive11_0181,
    Passive11_0182, Passive11_0183, Passive11_0184, Passive11_0185, Passive11_0186, Passive11_0187, Passive11_0188,
    Passive11_0189, Passive11_0190, Passive11_0191, Passive11_0192, Passive11_0193, Passive11_0194, Passive11_0195,
    Passive11_0196, Passive11_0197, Passive11_0198, Passive11_0199, Passive11_0200, Passive11_0201, Passive11_0202,
    Passive11_0203, Passive11_0204, Passive11_0205, Passive11_0206, Passive11_0207, Passive11_0208, Passive11_0209,
    Passive11_0210, Passive11_0211, Passive11_0212, Passive11_0213, Passive11_0214, Passive11_0215, Passive11_0216,
    Passive11_0217, Passive11_0218, Passive11_0219, Passive11_0220, Passive11_0221, Passive11_0222, Passive11_0223,
    Passive11_0224, Passive11_0225, Passive11_0226, Passive11_0227, Passive11_0228, Passive11_0229, Passive11_0230,
    Passive11_0231, Passive11_0232, Passive11_0233, Passive11_0234, Passive11_0235, Passive11_0236, Passive11_0237,
    Passive11_0238, Passive11_0239, Passive11_0240, Passive11_0241, Passive11_0242, Passive11_0243, Passive11_0244,
    Passive11_0245, Passive11_0246, Passive11_0247, Passive11_0248, Passive11_0249, Passive11_0250, Passive11_0251,
    Passive11_0252, Passive11_0253, Passive11_0254
};
