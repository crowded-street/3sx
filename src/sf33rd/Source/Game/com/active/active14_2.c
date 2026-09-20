/**
 * @file active14_2.c
 * COM Active: Akuma/Gouki
 * Pattern scripts from Pattern14_0063 on.
 */

#include "common.h"
#include "sf33rd/Source/Game/com/active/active14.h"
#include "sf33rd/Source/Game/com/active/active14_internal.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Pattern14_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xbf, 2);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args) { 8, 0x2e, 8, -1, -1, 0x34, 0, -0x7F80, -1, 0x400 });
        break;

    case 2:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 4:
        Normal_Attack(wk, 0xc, 0x202);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args) { 0xc, 0x1f, 10, -1 });
        break;

    default:
        pattern14_0063_from_step_6(wk);
        break;
    }
}

static void pattern14_0064_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Command_Attack(wk, &(Command_Attack_Args) { 0xc, 0x1f, 10, -1 });
        break;

    case 7:
        Wait(wk, 1);
        break;

    case 8:
        SA_Term(wk, &(SA_Term_Args) { 0x2f, 0x30, 0x31, 0x7f });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0064(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xbf, 2);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args) { 8, 0x2E, 8, -1, -1, 0x34, 0, -0x7F80, -1, 0x400 });
        break;

    case 2:
        Lever_Attack(wk, 9, 0, 0x20);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 4:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 5:
        Normal_Attack(wk, 0xc, 0x202);
        break;

    default:
        pattern14_0064_from_step_6(wk);
        break;
    }
}

void Pattern14_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xbf, 2);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args) { 8, 0x2e, 8, -1, -1, 0x34, 0, -0x7F80, -1, 0x400 });
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args) { 0x32, -1, -1, 0xbf });
        break;

    case 5:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 6, 0x77, 0x77, 0x78, 0x79 }, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args) { 8, 0x2E, 8, -1, -1, 0x34, 0, -0x7F80, -1, 0x400 });
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args) { 0x34, 0x34, 0x34, 0x7F });
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args) { 6, 0x77, 0x77, 0x78, 0x79 }, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0067(PLW* wk) {
    active_pattern_pierce_on_approach_walk_jump_command_attack_term(
        wk, &(JCA_Term_Args) { 0xB, 0x2F, 0xA, -1, -1, 0x40, 0, -1, -1, -1 }
    );
}

void Pattern14_0068(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args) { 8, 0, -1, -1 });
}

void Pattern14_0069(PLW* wk) {
    active_pattern_approach_walk_com_random_select_2(wk, 0xBF, 3, &(Branch_Menu_Args) { 6, 0xB2, 0xB3, 0xB4, 0xB6 });
}

void Pattern14_0070(PLW* wk) {
    active_pattern_approach_walk_com_random_select_2(wk, 0xBF, 3, &(Branch_Menu_Args) { 6, 0xB9, 0xBB, 0xBC, 0xBD });
}

void Pattern14_0071(PLW* wk) {
    active_pattern_approach_walk_com_random_select_2(wk, 0xBF, 3, &(Branch_Menu_Args) { 6, 0xB5, 0xB7, 0xBF, 0xC0 });
}

void Pattern14_0072(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 6, 0xB2, 0xB3, 0xB4, 0xB6 }, 0);
}

void Pattern14_0073(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 6, 0xB9, 0xBB, 0xBC, 0xBD }, 0);
}

void Pattern14_0074(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 6, 0xB5, 0xB7, 0xBF, 0xC0 }, 0);
}

void Pattern14_0075(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Pattern14_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args) { 0x34, 0x34, 0x34, 0x7F });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0077(PLW* wk) {
    active_pattern_normal_attack_5(wk, 0x12, 0x102, 0x202);
}

void Pattern14_0078(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args) { 8, 0x1F, 0xA, -1 });
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0079(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x20, 8, -1 });
        break;

    case 3:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0080(PLW* wk) {
    active_pattern_normal_attack_3(wk, 0x102, 8, 0x400);
}

void Pattern14_0081(PLW* wk) {
    active_pattern_normal_attack_5(wk, 0x102, 0x202, 0x40);
}

void Pattern14_0082(PLW* wk) {
    pattern_normal_attack_6(wk, 0xD, 0x20, 8);
}

void Pattern14_0083(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x20, 8, -1 });
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x20, 8, -1 });
        break;

    case 4:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void pattern14_0084_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 7:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0084(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x20, 8, -1 });
        break;

    case 4:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 5:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x20, 8, -1 });
        break;

    default:
        pattern14_0084_from_step_6(wk);
        break;
    }
}

/* Pattern14_0085 and Pattern14_0086 are the same four Normal_Attack steps.
 * Four lever values as parameters would put the helper one over the
 * argument-count threshold, so the steps arrive as a table of the
 * Normal_Attack_Step parameter object com_pattern_args.h defines. */
static void run_pattern14_normal_attack_4step(PLW* wk, const Normal_Attack_Step* steps) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, steps[0].Reaction, steps[0].Lever_Data);
        break;

    case 1:
        Normal_Attack(wk, steps[1].Reaction, steps[1].Lever_Data);
        break;

    case 2:
        Normal_Attack(wk, steps[2].Reaction, steps[2].Lever_Data);
        break;

    case 3:
        Normal_Attack(wk, steps[3].Reaction, steps[3].Lever_Data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0085(PLW* wk) {
    run_pattern14_normal_attack_4step(
        wk, (Normal_Attack_Step[]) { { 9, 0x220 }, { 9, 0x102 }, { 9, 0x202 }, { 8, 0x40 } }
    );
}

void Pattern14_0086(PLW* wk) {
    run_pattern14_normal_attack_4step(
        wk, (Normal_Attack_Step[]) { { 9, 0x12 }, { 9, 0x220 }, { 9, 0x102 }, { 8, 0x202 } }
    );
}

static void pattern14_0087_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Wait(wk, 2);
        break;

    case 7:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0087(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x20, 8, -1 });
        break;

    case 3:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 5:
        Lever_On(wk, 1, 2);
        break;

    default:
        pattern14_0087_from_step_6(wk);
        break;
    }
}

void Pattern14_0088(PLW* wk) {
    active_pattern_normal_attack_sa_term_approach_walk(wk, 0x102, 0);
}

void Pattern14_0089(PLW* wk) {
    active_pattern_normal_attack_sa_term_approach_walk(wk, 0x100, 1);
}

void Pattern14_0090(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x20, 8, -1 });
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args) { 0x34, 0x34, 0x34, 0x7f });
        break;

    case 4:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 5:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void pattern14_0091_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 7:
        Lever_Attack(wk, 8, 1, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0091(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x20, 8, -1 });
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args) { 8, 0x20, 8, -1 });
        break;

    case 4:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args) { 0x34, 0x34, 0x34, 0x7f });
        break;

    default:
        pattern14_0091_from_step_6(wk);
        break;
    }
}

void Pattern14_0092(PLW* wk) {
    active_pattern_turn_over_on_jump_attack_term_com_random_select(
        wk, &(Branch_Menu_Args) { 2, 0x4C, 0x4D, 0x4E, 0x4F }
    );
}

void Pattern14_0093(PLW* wk) {
    active_pattern_turn_over_on_jump_attack_term_com_random_select(
        wk, &(Branch_Menu_Args) { 2, 0x50, 0x51, 0x52, 0x53 }
    );
}

void Pattern14_0094(PLW* wk) {
    active_pattern_turn_over_on_jump_attack_term_com_random_select(
        wk, &(Branch_Menu_Args) { 2, 0x54, 0x55, 0x56, 0x57 }
    );
}

void Pattern14_0095(PLW* wk) {
    active_pattern_turn_over_on_jump_attack_term_com_random_select(
        wk, &(Branch_Menu_Args) { 2, 0x58, 0x59, 0x5A, 0x5B }
    );
}

void Pattern14_0096(PLW* wk) {
    active_pattern_command_attack_com_random_select(
        wk, &(Command_Attack_Args) { 8, 0, -1, -1 }, &(Branch_Menu_Args) { 2, 0x4C, 0x4D, 0x4E, 0x4F }, 0
    );
}

void Pattern14_0097(PLW* wk) {
    active_pattern_command_attack_com_random_select(
        wk, &(Command_Attack_Args) { 8, 0, -1, -1 }, &(Branch_Menu_Args) { 2, 0x50, 0x51, 0x52, 0x53 }, 0
    );
}

void Pattern14_0098(PLW* wk) {
    active_pattern_command_attack_com_random_select(
        wk, &(Command_Attack_Args) { 8, 0, -1, -1 }, &(Branch_Menu_Args) { 2, 0x54, 0x55, 0x56, 0x57 }, 0
    );
}

void Pattern14_0099(PLW* wk) {
    active_pattern_command_attack_com_random_select(
        wk, &(Command_Attack_Args) { 8, 0, -1, -1 }, &(Branch_Menu_Args) { 2, 0x58, 0x59, 0x5A, 0x5B }, 0
    );
}

void Pattern14_0100(PLW* wk) {
    active_pattern_approach_walk_jump_command_attack_term_com_random_select(
        wk, &(Branch_Menu_Args) { 2, 0x4C, 0x4D, 0x4E, 0x4F }
    );
}

void Pattern14_0101(PLW* wk) {
    active_pattern_approach_walk_jump_command_attack_term_com_random_select(
        wk, &(Branch_Menu_Args) { 2, 0x50, 0x51, 0x52, 0x53 }
    );
}

void Pattern14_0102(PLW* wk) {
    active_pattern_approach_walk_jump_command_attack_term_com_random_select(
        wk, &(Branch_Menu_Args) { 2, 0x54, 0x55, 0x56, 0x57 }
    );
}

void Pattern14_0103(PLW* wk) {
    active_pattern_approach_walk_jump_command_attack_term_com_random_select(
        wk, &(Branch_Menu_Args) { 2, 0x58, 0x59, 0x5A, 0x5B }
    );
}

void Pattern14_0104(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 2, 0x4C, 0x4D, 0x4E, 0x4F }, 0);
}

void Pattern14_0105(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 2, 0x50, 0x51, 0x52, 0x53 }, 0);
}

void Pattern14_0106(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 2, 0x54, 0x55, 0x56, 0x57 }, 0);
}

void Pattern14_0107(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 2, 0x58, 0x59, 0x5A, 0x5B }, 0);
}

void Pattern14_0108(PLW* wk) {
    active_pattern_command_attack_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args) { 2, 0x4C, 0x4D, 0x4E, 0x4F });
}

void Pattern14_0109(PLW* wk) {
    active_pattern_command_attack_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args) { 2, 0x50, 0x51, 0x52, 0x53 });
}

void Pattern14_0110(PLW* wk) {
    active_pattern_command_attack_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args) { 2, 0x54, 0x55, 0x56, 0x57 });
}

void Pattern14_0111(PLW* wk) {
    active_pattern_command_attack_turn_over_on_jump_attack_term(wk, &(Branch_Menu_Args) { 2, 0x58, 0x59, 0x5A, 0x5B });
}

void Pattern14_0112(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args) { 8, 1, -1, -1 });
}

void Pattern14_0113(PLW* wk) {
    active_pattern_command_attack_sa_term_em_term(wk, &(Command_Attack_Args) { 8, 0x22, 8, -1 });
}

void Pattern14_0114(PLW* wk) {
    active_pattern_search_back_term_command_attack_sa_term(
        wk, 0x300, 0x76, &(Command_Attack_Args) { 8, 0x1D, 0xA, -1 }
    );
}

void Pattern14_0115(PLW* wk) {
    active_pattern_search_back_term_command_attack_sa_term(wk, 0x80, 0x76, &(Command_Attack_Args) { 8, 0x1D, 0xA, -1 });
}

void Pattern14_0116(PLW* wk) {
    active_pattern_search_back_term_command_attack_sa_term(wk, 0x200, 0x77, &(Command_Attack_Args) { 8, 0x1D, 8, -1 });
}

void Pattern14_0117(PLW* wk) {
    active_pattern_search_back_term_command_attack_sa_term(wk, 0x100, 0x77, &(Command_Attack_Args) { 8, 0x1D, 8, -1 });
}

void Pattern14_0118(PLW* wk) {
    active_pattern_command_attack_sa_term_em_term(wk, &(Command_Attack_Args) { 8, 0x1C, 0xA, -1 });
}

void Pattern14_0119(PLW* wk) {
    active_pattern_command_attack_sa_term_em_term(wk, &(Command_Attack_Args) { 8, 0x1C, 8, -1 });
}

void Pattern14_0120(PLW* wk) {
    pattern_approach_walk_look(wk, 2);
}

void Pattern14_0121(PLW* wk) {
    active_pattern_approach_walk_com_random_select_2(wk, 0x72, 2, &(Branch_Menu_Args) { 2, 0x4C, 0x4D, 0x4E, 0x4F });
}

void Pattern14_0122(PLW* wk) {
    active_pattern_approach_walk_com_random_select_2(wk, 0x72, 2, &(Branch_Menu_Args) { 2, 0x50, 0x51, 0x52, 0x53 });
}

void Pattern14_0123(PLW* wk) {
    active_pattern_approach_walk_com_random_select_2(wk, 0x72, 2, &(Branch_Menu_Args) { 2, 0x54, 0x55, 0x56, 0x57 });
}

void Pattern14_0124(PLW* wk) {
    active_pattern_approach_walk_com_random_select_2(wk, 0x72, 2, &(Branch_Menu_Args) { 2, 0x58, 0x59, 0x5A, 0x5B });
}

void Pattern14_0125(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 2, 0x21, 0x3F, 0x40, 0x41 }, 0);
}

void Pattern14_0126(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 2, 0x23, 0x24, 0x25, 0x3D }, 0);
}

void Pattern14_0127(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 2, 0x22, 0x43, 0x3C, 0x3E }, 0);
}

void Pattern14_0128(PLW* wk) {
    active_pattern_approach_walk_look_com_random_select(wk, 0xBF);
}

void Pattern14_0129(PLW* wk) {
    active_pattern_approach_walk_look_com_random_select(wk, 0x72);
}

void Pattern14_0130(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 2, 0x71, 0x72, 0x71, 0x72 }, 0);
}

void Pattern14_0131(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 2, 0x1A, 0x1C, 0x1D, 0x1E }, 0);
}

void Pattern14_0132(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x78, 0x202);
}

void Pattern14_0133(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x86, 0x402);
}

void Pattern14_0134(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x77, 0x200);
}

void Pattern14_0135(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x84, 0x400);
}

void Pattern14_0136(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args) { 2, 0x84, 0x85, 0x86, 0x87 }, 0);
}

void Pattern14_0137(PLW* wk) {
    active_pattern_normal_attack_pierce_on_j_command_attack(wk, 0x200);
}
