/**
 * @file pass14_3.c
 * COM Passive: Akuma/Gouki
 * Pattern scripts from Passive14_0086 on.
 */

#include "sf33rd/Source/Game/com/passive/pass14.h"
#include "sf33rd/Source/Game/com/passive/pass14_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

static void passive14_pattern_normal_attack(PLW* wk, u16 lever_data, u16 lever_data_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, lever_data);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0086(PLW* wk) {
    passive14_pattern_normal_attack(wk, 0x202, 0x202);
}

static void passive14_pattern_normal_attack_j_command_attack(PLW* wk, u16 lever_data, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, lever_data);
        break;

    case 1:
        J_Command_Attack(wk, p);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0087(PLW* wk) {
    passive14_pattern_normal_attack_j_command_attack(wk, 0x102, &(Command_Attack_Args){8, 0x20, 10, -1});
}

void Passive14_0088(PLW* wk) {
    passive14_pattern_normal_attack_j_command_attack(wk, 0x102, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive14_0089(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xB, 0x102, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive14_0090(PLW* wk) {
    passive14_pattern_normal_attack_j_command_attack(wk, 0x202, &(Command_Attack_Args){8, 0x20, 10, -1});
}

void Passive14_0091(PLW* wk) {
    pattern_normal_attack_sa_term_j_command_attack(
        wk,
        0x202,
        &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive14_0092(PLW* wk) {
    pattern_normal_attack_sa_term_j_command_attack(
        wk,
        0x202,
        &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive14_0093(PLW* wk) {
    pattern_normal_attack_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive14_0094(PLW* wk) {
    pattern_normal_attack_command_attack_6(wk, 9, 0x220, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive14_0095(PLW* wk) {
    pattern_normal_attack_command_attack_wait_2(wk, 0x220, 0xC);
}

void Passive14_0096(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 10, -1});
        break;

    case 3:
        Wait(wk, 3);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0097(PLW* wk) {
    pattern_keep_away_com_random_select_2(wk, 0xBF, 1, &(Branch_Menu_Args){6, 0x15, 0x1D, 0x1E, 0x1F});
}

void Passive14_0098(PLW* wk) {
    passive14_pattern_normal_attack(wk, 0x200, 0x202);
}

void Passive14_0099(PLW* wk) {
    passive14_pattern_normal_attack(wk, 0x200, 0x200);
}

void Passive14_0100(PLW* wk) {
    passive14_pattern_normal_attack(wk, 0x102, 0x202);
}

void Passive14_0101(PLW* wk) {
    pattern_normal_attack_4(wk, 9, 10, 0x202);
}

void Passive14_0102(PLW* wk) {
    pattern_normal_attack_command_attack_4(wk, 9, 10, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive14_0103(PLW* wk) {
    pattern_normal_attack_command_attack_3(wk, 9, 10, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive14_0104(PLW* wk) {
    pattern_normal_attack_command_attack_wait_2(wk, 0x102, 10);
}

void Passive14_0105(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 1, -1, -1},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive14_0106(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 1, -1, -1},
        &(Command_Attack_Args){8, 0x21, 8, -1}
    );
}

void Passive14_0107(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Passive14_0108(PLW* wk) {
    pattern_approach_walk_normal_attack_2(wk, 8, 0x20);
}

void Passive14_0109(PLW* wk) {
    pattern_approach_walk_normal_attack_2(wk, 8, 0x40);
}

void Passive14_0110(PLW* wk) {
    pattern_approach_walk_normal_attack_2(wk, 8, 0x400);
}

void Passive14_0111(PLW* wk) {
    pattern_approach_walk_normal_attack_2(wk, 8, 0x402);
}

void Passive14_0112(PLW* wk) {
    pattern_approach_walk_normal_attack_j_command_attack_3(
        wk,
        0x47,
        &(Normal_Attack_Step){ 0xC, 0x20 },
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive14_0113(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 8, -1, -1, 0x20, 2, -0x7F80, -1, 0x400});
}

void Passive14_0114(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 9, -1, -1, 0x20, 2, -0x7F80, -1, 0x400});
}

void Passive14_0115(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 0xA, -1, -1, 0x20, 2, -0x7F80, -1, 0x400});
}

void Passive14_0116(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 8, -1, -1, 0x20, 0, -0x7F80, -1, 0x400});
}

void Passive14_0117(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 9, -1, -1, 0x20, 0, -0x7F80, -1, 0x400});
}

void Passive14_0118(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 0xA, -1, -1, 0x20, 0, -0x7F80, -1, 0x400});
}

void Passive14_0119(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 8, -1, -1, 0x20, 1, -0x7F80, -1, 0x400});
}

void Passive14_0120(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 9, -1, -1, 0x20, 1, -0x7F80, -1, 0x400});
}

void Passive14_0121(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 0xA, -1, -1, 0x20, 1, -0x7F80, -1, 0x400});
}

void Passive14_0122(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x71, 0x71, 0x72, 0x73}, 2);
}

void Passive14_0123(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x74, 0x74, 0x75, 0x76}, 2);
}

void Passive14_0124(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x77, 0x77, 0x78, 0x79}, 2);
}

void passive14_pattern_em_term_com_random_select(PLW* wk, const Branch_Menu_Args* p, s16 rnd_type) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 1});
        break;

    case 1:
        Com_Random_Select(wk, p, rnd_type);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0125(PLW* wk) {
    passive14_pattern_em_term_com_random_select(wk, &(Branch_Menu_Args){6, 0x57, 0x57, 0x58, 0x59}, 2);
}

void Passive14_0126(PLW* wk) {
    passive14_pattern_em_term_com_random_select(wk, &(Branch_Menu_Args){6, 0x5A, 0x5B, 0x5C, 0x5D}, 2);
}

void Passive14_0127(PLW* wk) {
    pattern_approach_walk_com_random_select(wk, 0x47, &(Branch_Menu_Args){6, 0x57, 0x57, 0x58, 0x59}, 2);
}

void Passive14_0128(PLW* wk) {
    pattern_approach_walk_com_random_select(wk, 0x47, &(Branch_Menu_Args){6, 0x5A, 0x5B, 0x5C, 0x5D}, 2);
}

void Passive14_0129(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x10, 0, 0x110);
}

void Passive14_0130(PLW* wk) {
    pattern_approach_walk_lever_attack(wk, 0x10, 1, 0x110);
}

void Passive14_0131(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x81, 0x81, 0x82, 0x82}, 0);
}

void Passive14_0132(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive14_0133(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive14_0134(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive14_0135(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x21, 8, -1});
}

void Passive14_0136(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x21, 9, -1});
}

void Passive14_0137(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x21, 10, -1});
}

void Passive14_0138(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x84, 0x84, 0x85, 0x86}, 2);
}

void Passive14_0139(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x87, 0x87, 0x88, 0x89}, 2);
}

static void passive14_pattern_j_command_attack(PLW* wk, const Command_Attack_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, p);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0140(PLW* wk) {
    passive14_pattern_j_command_attack(wk, &(Command_Attack_Args){9, 0x20, 8, -1});
}

void Passive14_0141(PLW* wk) {
    passive14_pattern_j_command_attack(wk, &(Command_Attack_Args){9, 0x20, 9, -1});
}

void Passive14_0142(PLW* wk) {
    passive14_pattern_j_command_attack(wk, &(Command_Attack_Args){9, 0x20, 10, -1});
}

void Passive14_0143(PLW* wk) {
    passive14_pattern_em_term_com_random_select(wk, &(Branch_Menu_Args){6, 0x8C, 0x8C, 0x8D, 0x8E}, 2);
}

void Passive14_0144(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive14_0145(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x90, 0xFF, 0xFF, 0xFF}, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0146(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){0xC, 0x2E, 8, -1, -1, 0x34, 0, -1, -1, 0xFFFF});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0xFFFF, 0xFFFF, 0xBF});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive14_0147_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Wait(wk, 1);
        break;

    case 7:
        Only_Shot(wk, 0x100);
        break;

    case 8:
        Wait(wk, 1);
        break;

    case 9:
        Only_Shot(wk, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0147(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA_Full(wk, 6, 0x76);
        break;

    case 1:
        Only_Shot(wk, 0x10);
        break;

    case 2:
        Wait(wk, 1);
        break;

    case 3:
        Only_Shot(wk, 0x10);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        Lever_On(wk, 0, 0);
        break;

    default:
        passive14_0147_from_step_6(wk);
        break;
    }
}

void Passive14_0148(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA(wk, 6, 0x76);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x8014, 10, -1, -1, 0x20, 0, -1, -1, 0xFFFF});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0149(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x92, 0x92, 0x93, 0x94}, 2);
}

void Passive14_0150(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 1:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x81, 0x82, 0x81, 0x82}, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive14_pattern_jump_attack_term_normal_attack_command_attack(
    PLW* wk, const Jump_Term_Args* a, const Command_Attack_Args* p, const SA_Term_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, a);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        Command_Attack(wk, p);
        break;

    case 3:
        Wait(wk, 1);
        break;

    case 4:
        SA_Term(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0151(PLW* wk) {
    passive14_pattern_jump_attack_term_normal_attack_command_attack(
        wk,
        &(Jump_Term_Args){8, 0x49, 0xB, 0x202, 0, -0x7F80, -1, 0x400},
        &(Command_Attack_Args){0xB, 0x1F, 10, -1},
        &(SA_Term_Args){0x2F, 0x34, 0x34, 0x7F}
    );
}

void Passive14_0152(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-1, 0x61, 0xB, 0x202, 0, -0x7F80, -1, 0x400});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1F, 10, -1});
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x34, 0x34, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0153(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x20);
}

void Passive14_0154(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 1}, 0xD, 0x20);
}

void Passive14_0155(PLW* wk) {
    pattern_approach_walk_jump_attack_term_normal_attack_2(wk, 3, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive14_0156(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC8, 0xB, 0x400, 0, -0x7F80, -1, 0x400});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1F, 10, -1});
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x34, 0x34, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
