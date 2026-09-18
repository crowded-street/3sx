/**
 * @file pass05.c
 * COM Passive: Necro
 */

#include "sf33rd/Source/Game/com/passive/pass05.h"
#include "sf33rd/Source/Game/com/passive/pass05_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive05_Tbl[102])();

void passive05_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive05_pattern_em_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        J_Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive05_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, s16 reaction, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Normal_Attack(wk, reaction, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive05_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive05_pattern_em_term_lever_attack(PLW* wk, const EM_Term_Params* p, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Lever_Attack(wk, 8, lever, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive05_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive05_pattern_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive05_pattern_em_term_sa_term_j_command_attack(
    PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b, const Command_Attack_Args* p_b_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        SA_Term(wk, p_b);
        break;

    case 2:
        J_Command_Attack(wk, p_b_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive05_pattern_wait_get_up(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, lever_data, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive05_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive05_pattern_lever_attack(PLW* wk, u16 lever, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, lever, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05(PLW* wk) {
    Passive05_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive05_0000(PLW* wk) {
    passive05_pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive05_0001(PLW* wk) {
    pattern_provoke(wk, -1);
}

void Passive05_0002(PLW* wk) {
    passive05_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive05_0003(PLW* wk) {
    passive05_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive05_0004(PLW* wk) {
    passive05_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, 0x70});
}

void Passive05_0005(PLW* wk) {
    passive05_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive05_0006(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Off(wk);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0007(PLW* wk) {
    passive05_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F88, -1, 0, 1, -1},
        &(Command_Attack_Args){8, 0x41D, 9, -1}
    );
}

void Passive05_0008(PLW* wk) {
    passive05_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F88, -1, 0, 1, -1},
        &(Command_Attack_Args){8, 0x41D, 9, -1}
    );
}

void Passive05_0009(PLW* wk) {
    passive05_pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F88, -1, 0, 1, -1},
        &(SA_Term_Args){0x35, 0xFFFF, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x41D, 10, -1}
    );
}

void Passive05_0010(PLW* wk) {
    passive05_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FD0, -0x7FC8, 0, 1, -1}, 8, 0x42);
}

void Passive05_0011(PLW* wk) {
    passive05_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F78, -0x7FB8, 0, 1, -1}, 8, 0x22);
}

void Passive05_0012(PLW* wk) {
    passive05_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-0x7F90, 0, 0, 2, 0}, 1, 0x400);
}

void Passive05_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0014(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        VS_Jump_Guard(wk);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0015(PLW* wk) {
    passive05_pattern_normal_attack(wk, 8, 0x10);
}

void Passive05_0016(PLW* wk) {
    pattern_approach_walk_wait_get_up_lever_attack(wk, 0x3F, 0, 0x110);
}

void Passive05_0017(PLW* wk) {
    pattern_approach_walk_wait_get_up_normal_attack(wk, 0x67, 0x202);
}

void Passive05_0018(PLW* wk) {
    pattern_approach_walk_wait_get_up_j_command_attack(wk, 0x67, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive05_0019(PLW* wk) {
    passive05_pattern_wait_get_up(wk, 0);
}

void Passive05_0020(PLW* wk) {
    passive05_pattern_wait_get_up(wk, 3);
}

void Passive05_0021(PLW* wk) {
    pattern_approach_walk_wait_get_up_j_command_attack(wk, 0x67, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive05_0022(PLW* wk) {
    pattern_wait_get_up_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, 0x40, 8, 0x402, 0, -0x7F68, -1, 0x10});
}

void Passive05_0023(PLW* wk) {
    pattern_wait_get_up_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FA8, 8, 0x40, 0, -0x7F78, -1, 0x10});
}

void Passive05_0024(PLW* wk) {
    pattern_approach_walk_wait_get_up_lever_attack(wk, 0xB7, 0, 0x400);
}

void Passive05_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1});
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0026(PLW* wk) {
    pattern_approach_walk_em_term(wk, 0x67, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
}

void Passive05_0027(PLW* wk) {
    pattern_normal_attack_command_attack(wk, 0xB, 0x202, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive05_0028(PLW* wk) {
    passive05_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -1, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive05_0029(PLW* wk) {
    passive05_pattern_lever_attack(wk, 1, 0x10);
}

void Passive05_0030(PLW* wk) {
    passive05_pattern_normal_attack(wk, 8, 0x400);
}

void Passive05_0031(PLW* wk) {
    pattern_walk(wk, 1);
}

void Passive05_0032(PLW* wk) {
    passive05_pattern_lever_attack(wk, 0, 0x110);
}

void Passive05_0033(PLW* wk) {
    passive05_pattern_lever_attack(wk, 1, 0x110);
}

void Passive05_0034(PLW* wk) {
    pattern_adjust_attack(wk, 0x10);
}

void Passive05_0035(PLW* wk) {
    passive05_pattern_normal_attack(wk, 8, 0x102);
}

void Passive05_0036(PLW* wk) {
    pattern_wait(wk, 0);
}

void Passive05_0037(PLW* wk) {
    passive05_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-1, -0x7FF8, 0, 1, -1}, 0, 0x110);
}

void Passive05_0038(PLW* wk) {
    passive05_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-1, -0x7FF8, 0, 1, -1}, 1, 0x110);
}

void Passive05_0039(PLW* wk) {
    passive05_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FCB, -0x7FF8, 0, 1, -1}, 8, 0x40);
}

void Passive05_0040(PLW* wk) {
    passive05_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC0, 0, 1, -1},
        &(Command_Attack_Args){8, 0x20, 10, -1}
    );
}

void Passive05_0041(PLW* wk) {
    pattern_normal_attack_j_command_attack(wk, 8, 0x42, &(Command_Attack_Args){8, 0x41D, 10, -1});
}

void Passive05_0042(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Off(wk);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0043(PLW* wk) {
    passive05_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, -1, 0, 6, 0x1F},
        &(Command_Attack_Args){8, 0x41D, 10, -1}
    );
}

void Passive05_0044(PLW* wk) {
    pattern_approach_walk_em_term_lever_attack(wk, 0x3F, &(EM_Term_Params){-1, -0x7FF8, 3, 1, -1}, 0);
}

void Passive05_0045(PLW* wk) {
    passive05_pattern_normal_attack(wk, 8, 0x400);
}

void Passive05_0046(PLW* wk) {
    pattern_keep_status(wk, -1);
}

void Passive05_0047(PLW* wk) {
    pattern_normal_attack_j_command_attack(wk, 0xB, 0x102, &(Command_Attack_Args){8, 0x1E, 8, 0x70});
}

void Passive05_0048(PLW* wk) {
    passive05_pattern_normal_attack(wk, 8, 0x402);
}

void Passive05_0049(PLW* wk) {
    passive05_pattern_normal_attack(wk, 8, 0x12);
}

void Passive05_0050(PLW* wk) {
    passive05_pattern_normal_attack(wk, 8, 0x200);
}

void Passive05_0051(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0052(PLW* wk) {
    passive05_pattern_normal_attack(wk, 8, 0x202);
}

void Passive05_0053(PLW* wk) {
    passive05_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive05_0054(PLW* wk) {
    passive05_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, 0x87, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 9, 0x70}
    );
}

void Passive05_0055(PLW* wk) {
    passive05_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive05_0056(PLW* wk) {
    passive05_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC0, 0, 1, -1},
        &(Command_Attack_Args){8, 0x20, 10, 0x700}
    );
}

void Passive05_0057(PLW* wk) {
    passive05_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x40, 8, 0x402, 2, -0x7F80, -1, 0x20});
}

void Passive05_0058(PLW* wk) {
    pattern_keep_status(wk, 1);
}

void Passive05_0059(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 10, -1});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0x60});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0060(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive05_0061(PLW* wk) {
    passive05_pattern_wait_get_up(wk, 0);
}

void Passive05_0062(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x20);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0x60});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0064(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x67, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x40);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, 0x70});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x67, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x200);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive05_0066(PLW* wk) {
    pattern_approach_walk_sa_term(wk, 0xB7, &(SA_Term_Args){0x35, 0x36, 0x37, 0x60});
}

void Passive05_0067(PLW* wk) {
    pattern_wait_attack_complete(wk, 0);
}

void Passive05_0068(PLW* wk) {
    pattern_wait_attack_complete(wk, 1);
}

void Passive05_0069(PLW* wk) {
    pattern_wait_attack_complete_sa_term_wait_attack_complete(wk, &(SA_Term_Args){0x35, 0xFFFF, 0x37, 0});
}

void (*const Passive05_Tbl[102])(PLW*) = {
    Passive05_0000, Passive05_0001, Passive05_0002, Passive05_0003, Passive05_0004, Passive05_0005, Passive05_0006,
    Passive05_0007, Passive05_0008, Passive05_0009, Passive05_0010, Passive05_0011, Passive05_0012, Passive05_0013,
    Passive05_0014, Passive05_0015, Passive05_0016, Passive05_0017, Passive05_0018, Passive05_0019, Passive05_0020,
    Passive05_0021, Passive05_0022, Passive05_0023, Passive05_0024, Passive05_0025, Passive05_0026, Passive05_0027,
    Passive05_0028, Passive05_0029, Passive05_0030, Passive05_0031, Passive05_0032, Passive05_0033, Passive05_0034,
    Passive05_0035, Passive05_0036, Passive05_0037, Passive05_0038, Passive05_0039, Passive05_0040, Passive05_0041,
    Passive05_0042, Passive05_0043, Passive05_0044, Passive05_0045, Passive05_0046, Passive05_0047, Passive05_0048,
    Passive05_0049, Passive05_0050, Passive05_0051, Passive05_0052, Passive05_0053, Passive05_0054, Passive05_0055,
    Passive05_0056, Passive05_0057, Passive05_0058, Passive05_0059, Passive05_0060, Passive05_0061, Passive05_0062,
    Passive05_0063, Passive05_0064, Passive05_0065, Passive05_0066, Passive05_0067, Passive05_0068, Passive05_0069,
    Passive05_0070, Passive05_0071, Passive05_0072, Passive05_0073, Passive05_0074, Passive05_0075, Passive05_0076,
    Passive05_0077, Passive05_0078, Passive05_0079, Passive05_0080, Passive05_0081, Passive05_0082, Passive05_0083,
    Passive05_0084, Passive05_0085, Passive05_0086, Passive05_0087, Passive05_0088, Passive05_0089, Passive05_0090,
    Passive05_0091, Passive05_0092, Passive05_0093, Passive05_0094, Passive05_0095, Passive05_0096, Passive05_0097,
    Passive05_0098, Passive05_0099, Passive05_0100, Passive05_0101
};
