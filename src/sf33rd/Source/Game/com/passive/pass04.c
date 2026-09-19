/**
 * @file pass04.c
 * COM Passive: Dudley
 */

#include "sf33rd/Source/Game/com/passive/pass04.h"
#include "sf33rd/Source/Game/com/passive/pass04_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive04_Tbl[142])();

void passive04_pattern_approach_walk_wait_get_up_command_attack(PLW* wk, s16 target_pos, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive04_pattern_approach_walk_wait_get_up_lever_attack(
    PLW* wk, s16 target_pos, s16 reaction, u16 lever_data
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Lever_Attack(wk, reaction, 0, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive04(PLW* wk) {
    Passive04_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive04_0000(PLW* wk) {
    pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive04_0001(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x30, 6, 0x58);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive04_0002(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive04_0003(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive04_0004(PLW* wk) {
    pattern_command_attack(wk, &(Command_Attack_Args){8, 0, -1, -1}, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive04_0005(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){0xD, 0x20, 0x30A, -1});
}

void Passive04_0006(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x30, 6, 0x58);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive04_0007(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F98, -1, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive04_0008(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F98, -1, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive04_0009(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F98, -1, 0, 1, -1},
        &(SA_Term_Args){0x35, 0xFFFF, 0xFFFF, 0},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive04_0010(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-0x7FD0, -0x7FC0, 0, 1, -1}, 8, 0x40);
}

void Passive04_0011(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x200, 0, -0x7F70, -1, 0x20});
}

void Passive04_0012(PLW* wk) {
    pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F90, 0, 0, 2, 0},
        &(Command_Attack_Args){0xD, 0x20, 0x60A, -1}
    );
}

void Passive04_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x30, 6, 0x58);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive04_0014(PLW* wk) {
    pattern_vs_jump_guard(wk);
}

void Passive04_0015(PLW* wk) {
    pattern_normal_attack_7(wk, 0x10, 0x10, 0x10);
}

void Passive04_0016(PLW* wk) {
    passive04_pattern_approach_walk_wait_get_up_lever_attack(wk, 0x47, 0xD, 0x200);
}

void Passive04_0017(PLW* wk) {
    pattern_approach_walk_wait_get_up_normal_attack(wk, 0x47, 0x402);
}

void Passive04_0018(PLW* wk) {
    passive04_pattern_approach_walk_wait_get_up_lever_attack(wk, 0x47, 8, 0x400);
}

void Passive04_0019(PLW* wk) {
    pattern_wait_get_up_2(wk, 3);
}

void Passive04_0020(PLW* wk) {
    pattern_wait_get_up_2(wk, 3);
}

void Passive04_0021(PLW* wk) {
    passive04_pattern_approach_walk_wait_get_up_command_attack(wk, 0x9F, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive04_0022(PLW* wk) {
    pattern_wait_get_up_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 8, 0x400, 0, -0x7F68, -1, 0x400});
}

void Passive04_0023(PLW* wk) {
    pattern_wait_get_up_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x40, 0, -0x7F78, -1, 0x40});
}

void Passive04_0024(PLW* wk) {
    passive04_pattern_approach_walk_wait_get_up_lever_attack(wk, 0xDF, 8, 0x40);
}

void Passive04_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1});
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive04_0026(PLW* wk) {
    pattern_approach_walk_em_term(wk, 0x9F, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
}

void Passive04_0027(PLW* wk) {
    pattern_normal_attack_command_attack(wk, 9, 0x22, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive04_0028(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x202);
}

void Passive04_0029(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x10);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x10);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive04_0030(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x402);
}

void Passive04_0031(PLW* wk) {
    pattern_walk(wk, 1);
}

void Passive04_0032(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x47, 0, 0x110);
}

void Passive04_0033(PLW* wk) {
    pattern_approach_walk_normal_attack_2(wk, 8, 0x110);
}

void Passive04_0034(PLW* wk) {
    pattern_adjust_attack(wk, 0x20);
}

void Passive04_0035(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x102);
}

void Passive04_0036(PLW* wk) {
    pattern_wait(wk, 0);
}

void Passive04_0037(PLW* wk) {
    pattern_approach_walk_em_term_lever_attack(wk, 0x47, &(EM_Term_Params){-1, -0x7FF8, 0, 1, -1}, 0);
}

void Passive04_0038(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x47, &(EM_Term_Params){-1, -0x7FF8, 0, 1, -1}, 0x110);
}

void Passive04_0039(PLW* wk) {
    pattern_approach_walk_em_term_j_command_attack(
        wk,
        0x47,
        &(EM_Term_Params){-1, -0x7FE0, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive04_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 0, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive04_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FE8, 0, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x20);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive04_0042(PLW* wk) {
    pattern_lever_off_look(wk);
}

void Passive04_0043(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD8, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive04_0044(PLW* wk) {
    pattern_approach_walk_em_term_lever_attack(wk, 0x47, &(EM_Term_Params){-1, -0x7FF8, 3, 1, -1}, 0);
}

void Passive04_0045(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x40);
}

void Passive04_0046(PLW* wk) {
    pattern_keep_status(wk, -1);
}

void Passive04_0047(PLW* wk) {
    pattern_normal_attack_j_command_attack(wk, 0xB, 0x102, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive04_0048(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x102, 0x202);
}

void Passive04_0049(PLW* wk) {
    pattern_adjust_attack(wk, 0x20);
}

void Passive04_0050(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x40);
}

void Passive04_0051(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x12, 0x12);
}

void Passive04_0052(PLW* wk) {
    pattern_approach_walk_normal_attack_2(wk, 8, 0x202);
}

void Passive04_0053(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x400);
}

void Passive04_0054(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x22);
}

void Passive04_0055(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x200);
}

void Passive04_0056(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x40, 8, 0x400, 0, -0x7F68, -1, 0x400});
}

void Passive04_0057(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x100, 0, -0x7F78, -1, 0x40});
}

void Passive04_0058(PLW* wk) {
    pattern_keep_status(wk, 1);
}

void Passive04_0059(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive04_0060(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive04_0061(PLW* wk) {
    pattern_wait_get_up_2(wk, 0);
}

void (*const Passive04_Tbl[142])(PLW*) = {
    Passive04_0000, Passive04_0001, Passive04_0002, Passive04_0003, Passive04_0004, Passive04_0005, Passive04_0006,
    Passive04_0007, Passive04_0008, Passive04_0009, Passive04_0010, Passive04_0011, Passive04_0012, Passive04_0013,
    Passive04_0014, Passive04_0015, Passive04_0016, Passive04_0017, Passive04_0018, Passive04_0019, Passive04_0020,
    Passive04_0021, Passive04_0022, Passive04_0023, Passive04_0024, Passive04_0025, Passive04_0026, Passive04_0027,
    Passive04_0028, Passive04_0029, Passive04_0030, Passive04_0031, Passive04_0032, Passive04_0033, Passive04_0034,
    Passive04_0035, Passive04_0036, Passive04_0037, Passive04_0038, Passive04_0039, Passive04_0040, Passive04_0041,
    Passive04_0042, Passive04_0043, Passive04_0044, Passive04_0045, Passive04_0046, Passive04_0047, Passive04_0048,
    Passive04_0049, Passive04_0050, Passive04_0051, Passive04_0052, Passive04_0053, Passive04_0054, Passive04_0055,
    Passive04_0056, Passive04_0057, Passive04_0058, Passive04_0059, Passive04_0060, Passive04_0061, Passive04_0062,
    Passive04_0063, Passive04_0064, Passive04_0065, Passive04_0066, Passive04_0067, Passive04_0068, Passive04_0069,
    Passive04_0070, Passive04_0071, Passive04_0072, Passive04_0073, Passive04_0074, Passive04_0075, Passive04_0076,
    Passive04_0077, Passive04_0078, Passive04_0079, Passive04_0080, Passive04_0081, Passive04_0082, Passive04_0083,
    Passive04_0084, Passive04_0085, Passive04_0086, Passive04_0087, Passive04_0088, Passive04_0089, Passive04_0090,
    Passive04_0091, Passive04_0092, Passive04_0093, Passive04_0094, Passive04_0095, Passive04_0096, Passive04_0097,
    Passive04_0098, Passive04_0099, Passive04_0100, Passive04_0101, Passive04_0102, Passive04_0103, Passive04_0104,
    Passive04_0105, Passive04_0106, Passive04_0107, Passive04_0108, Passive04_0109, Passive04_0110, Passive04_0111,
    Passive04_0112, Passive04_0113, Passive04_0114, Passive04_0115, Passive04_0116, Passive04_0117, Passive04_0118,
    Passive04_0119, Passive04_0120, Passive04_0121, Passive04_0122, Passive04_0123, Passive04_0124, Passive04_0125,
    Passive04_0126, Passive04_0127, Passive04_0128, Passive04_0129, Passive04_0130, Passive04_0131, Passive04_0132,
    Passive04_0133, Passive04_0134, Passive04_0135, Passive04_0136, Passive04_0137, Passive04_0138, Passive04_0139,
    Passive04_0140, Passive04_0141
};
