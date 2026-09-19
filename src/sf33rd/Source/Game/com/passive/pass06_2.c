/**
 * @file pass06_2.c
 * COM Passive: Hugo
 * Pattern scripts from Passive06_0067 on.
 */

#include "sf33rd/Source/Game/com/passive/pass06.h"
#include "sf33rd/Source/Game/com/passive/pass06_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive06_0067(PLW* wk) {
    pattern_approach_walk_wait_get_up_command_attack(wk, 0x60, -1, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive06_0068(PLW* wk) {
    pattern_approach_walk_wait_get_up_normal_attack(wk, 0x44, 0x110);
}

void Passive06_0069(PLW* wk) {
    pattern_approach_walk_wait_get_up_lever_attack(wk, 0x44, 1, 0x110);
}

void Passive06_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x80, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 2:
        Wait(wk, 10);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0071(PLW* wk) {
    passive06_pattern_approach_walk_wait_get_up_wait(wk, 0x100);
}

void Passive06_0072(PLW* wk) {
    passive06_pattern_approach_walk_wait_get_up_wait(wk, 0x200);
}

void Passive06_0073(PLW* wk) {
    passive06_pattern_approach_walk_wait_get_up_wait(wk, 0x10);
}

void Passive06_0074(PLW* wk) {
    passive06_pattern_approach_walk_wait_get_up_wait(wk, 0x20);
}

void Passive06_0075(PLW* wk) {
    passive06_pattern_approach_walk_wait_get_up_wait(wk, 0x40);
}

void Passive06_0076(PLW* wk) {
    pattern_approach_walk_wait_get_up_command_attack(wk, 0x80, 0, &(Command_Attack_Args){8, 0x20, 8, -1});
}

static void passive06_0077_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0077(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x80, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 2:
        Check_EX(wk, 6, 0x4E);
        break;

    case 3:
        Wait(wk, 10);
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1F, 10, 0x70});
        break;

    default:
        passive06_0077_from_step_6(wk);
        break;
    }
}

void Passive06_0078(PLW* wk) {
    passive06_pattern_wait_get_up_com_random_select(wk, 3, &(Branch_Menu_Args){6, 0x41, 0x2D, 0x31, 0x65});
}

void Passive06_0079(PLW* wk) {
    passive06_pattern_wait_get_up_normal_attack(wk, 3, 0, 0x12);
}

void Passive06_0080(PLW* wk) {
    passive06_pattern_wait_get_up_normal_attack(wk, 3, 0, 0x22);
}

void Passive06_0081(PLW* wk) {
    passive06_pattern_wait_get_up_normal_attack(wk, 3, 0, 0x42);
}

void Passive06_0082(PLW* wk) {
    passive06_pattern_wait_get_up_normal_attack(wk, 3, 0, 0x102);
}

void Passive06_0083(PLW* wk) {
    passive06_pattern_wait_get_up_normal_attack(wk, 3, 0, 0x202);
}

void Passive06_0084(PLW* wk) {
    passive06_pattern_wait_get_up_normal_attack(wk, 3, -1, 0x402);
}

void Passive06_0085(PLW* wk) {
    pattern_wait_get_up(wk, 3, 0);
}

void Passive06_0086(PLW* wk) {
    pattern_keep_away_wait_get_up(wk, 0x9F, 0, 0);
}

void Passive06_0087(PLW* wk) {
    pattern_keep_away_wait_get_up(wk, 0xFF, 0, 0);
}

void Passive06_0088(PLW* wk) {
    pattern_wait_get_up(wk, 0, 0);
}

void Passive06_0089(PLW* wk) {
    pattern_keep_away_wait_get_up_command_attack(wk, 0x9F, 0, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Passive06_0090(PLW* wk) {
    pattern_keep_away_wait_get_up_com_random_select(wk, 0x9F, &(Branch_Menu_Args){6, 0x41, 0x2D, 0x31, 0x65});
}

void Passive06_0091(PLW* wk) {
    pattern_keep_away_wait_get_up_com_random_select(wk, 0xFF, &(Branch_Menu_Args){6, 0x3C, 0x3D, 0x3F, 0x65});
}

void Passive06_0092(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x80, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0093(PLW* wk) {
    passive06_pattern_wait_get_up_normal_attack(wk, 0, 0, 0x200);
}

void Passive06_0094(PLW* wk) {
    passive06_pattern_wait_get_up_com_random_select(wk, 0, &(Branch_Menu_Args){6, 0x41, 0x2D, 0x31, 0x65});
}

void Passive06_0095(PLW* wk) {
    passive06_pattern_wait_get_up_com_random_select(wk, 0, &(Branch_Menu_Args){6, 0x3C, 0x3D, 0x3F, 0x65});
}

void Passive06_0096(PLW* wk) {
    active_pattern_approach_walk(wk, 0x44);
}

void Passive06_0097(PLW* wk) {
    active_pattern_approach_walk(wk, 0x9F);
}

void Passive06_0098(PLW* wk) {
    active_pattern_approach_walk(wk, 0xFF);
}

void Passive06_0099(PLW* wk) {
    pattern_jump(wk, 0);
}

void Passive06_0100(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0, 8, -1});
}

void Passive06_0101(PLW* wk) {
    pattern_wait_get_up_command_attack(wk, 0, 0, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Passive06_0102(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Passive06_0103(PLW* wk) {
    passive06_pattern_approach_walk_check_ex_pierce_on(wk, 0x80, 0x36);
}

void Passive06_0104(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 5, 6, 0xF);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Provoke(wk, -1);
        break;

    case 3:
        Next_Another_Menu(wk, 6, 0xF);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0105(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, -1, -1},
        &(Command_Attack_Args){8, 0x20, 8, -1}
    );
}

void Passive06_0106(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0xFFFF, 0xFFFF, 3, 0xFFFF},
        &(Command_Attack_Args){0xC, 0, -1, -1}
    );
}

void Passive06_0107(PLW* wk) {
    pattern_etc_term_provoke_next_another_menu(wk, 0x36, 0x37);
}

void Passive06_0108(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x80,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x20, 8, -1}
    );
}

void Passive06_0109(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB8, 6, 1, -1}, 0x10);
}

void Passive06_0110(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Passive06_0111(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x16, 0x16, 0x17, 0x18});
}

void Passive06_0112(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Provoke(wk, -1);
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x65, 0x5B, 0x5A, 0x58}, 4);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0113(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0114(PLW* wk) {
    pattern_jump(wk, 1);
}

void Passive06_0115(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 8, 0x200, 1, -0x7FA8, -1, 0x40});
}

void Passive06_0116(PLW* wk) {
    pattern_jump(wk, 2);
}

void Passive06_0117(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F68, 0x40, 8, 0x40, 2, -0x7F68, -1, 0x400});
}

void Passive06_0118(PLW* wk) {
    pattern_pierce_on_command_attack_branch_unit_area(
        wk,
        &(Command_Attack_Args){0xC, 1, -1, -1},
        &(Branch_Menu_Args){6, 0x16, 0x16, 0x17, 0x18}
    );
}

void Passive06_0119(PLW* wk) {
    passive06_pattern_approach_walk_check_ex_pierce_on(wk, 0x58, 0x6F);
}

void Passive06_0120(PLW* wk) {
    passive06_pattern_approach_walk_check_ex_pierce_on(wk, 0x58, 0x79);
}

void Passive06_0121(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x16, 0x16, 0x17, 0x18}
    );
}

void Passive06_0122(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Passive06_0123(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F40, -1, 0, 1, -1}, 0x12);
}

void Passive06_0124(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F20, -1, 0, 1, -1}, 0x22);
}

void Passive06_0125(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F60, -1, 0, 1, -1}, 0x102);
}

void Passive06_0126(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F40, -1, 0, 1, -1}, 0x202);
}

void Passive06_0127(PLW* wk) {
    active_pattern_pierce_on_jump_com_random_select(wk, &(Branch_Menu_Args){6, 0x16, 0x13, 0x2C, 8}, 1);
}

void Passive06_0128(PLW* wk) {
    pattern_sa_term_branch_unit_area(
        wk,
        &(SA_Term_Args){0xFFFF, 0xFFFF, 3, 0xFFFF},
        &(Branch_Menu_Args){6, 0x16, 0x16, 0x17, 0x18}
    );
}

void Passive06_0129(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive06_0130(PLW* wk) {
    pattern_approach_walk_sa_term_command_attack_2(wk, 0x9F, &(SA_Term_Args){1, 0xFFFF, 0xFFFF, -1});
}

void Passive06_0131(PLW* wk) {
    pattern_turn_over_on_jump_attack_term(wk);
}

void Passive06_0132(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F90, -0x7FA8, 9, 0x42, 0, -0x7F68, -1, 0x400});
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x100);
        break;

    case 3:
        Wait(wk, 9);
        break;

    case 4:
        EM_Term(wk, &(EM_Term_Params){-0x7F90, -1, 4, 6, 0x89});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0133(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F90, -0x7FA8, 9, 0x42, 0, -0x7F68, -1, 0x400});
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x100);
        break;

    case 4:
        Wait(wk, 9);
        break;

    case 5:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x8A, 0x16, 0x86, 0x87}, 5);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0134(PLW* wk) {
    pattern_sa_term_branch_unit_area(
        wk,
        &(SA_Term_Args){1, 0xFFFF, 3, -1},
        &(Branch_Menu_Args){6, 0x16, 0x16, 0x17, 0x18}
    );
}

void Passive06_0135(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xB, 0x21, 0xA, -1},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Passive06_0136(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F90, -0x7FA8, 9, 0x42, 0, -0x7F68, -1, 0x400});
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x100);
        break;

    case 3:
        Wait(wk, 9);
        break;

    case 4:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x89, 0x86, 0x16, 0x87}, 5);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0137(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x16, 0x16, 0x17, 0x18});
}

void Passive06_0138(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F90, -1, 4, 6, 0x89},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive06_0139(PLW* wk) {
    pattern_hi_jump_attack_term_normal_attack_2(
        wk,
        &(Hi_Jump_Term_Args){-0x7F88, -0x7FA8, 9, 0x400, 0, -0x7F68, -1, 0x400},
        0x102,
        0x202
    );
}

void Passive06_0140(PLW* wk) {
    passive06_pattern_hi_jump_attack_term_normal_attack(wk, 0x402);
}
