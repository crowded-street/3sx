/**
 * @file pass11_2.c
 * COM Passive: Ken
 * Pattern scripts from Passive11_0052 on.
 */

#include "sf33rd/Source/Game/com/passive/pass11.h"
#include "sf33rd/Source/Game/com/passive/pass11_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive11_0052(PLW* wk) {
    pattern_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive11_0053(PLW* wk) {
    pattern_jump_attack_term_normal_attack_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x20,
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive11_0054(PLW* wk) {
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

void Passive11_0055(PLW* wk) {
    pattern_wait_get_up_lever_attack_2(wk, 1, 0x200);
}

void Passive11_0056(PLW* wk) {
    pattern_wait_get_up_normal_attack_sa_term_2(
        wk,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive11_0057(PLW* wk) {
    passive11_pattern_em_term_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 10, 0x70});
}

void Passive11_0058(PLW* wk) {
    pattern_wait_get_up_lever_attack_2(wk, 0, 0x110);
}

void Passive11_0059(PLW* wk) {
    pattern_wait_get_up_lever_attack_2(wk, 1, 0x110);
}

void Passive11_0060(PLW* wk) {
    pattern_wait_get_up_lever_attack_2(wk, 0xFFFF, 0x110);
}

void Passive11_0061(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Lie(wk, 0);
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0062(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 3, 0x402);
}

void Passive11_0063(PLW* wk) {
    pattern_pierce_on_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, -1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, 0x70}
    );
}

void Passive11_0064(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 0, 0x400);
}

void Passive11_0065(PLW* wk) {
    pattern_wait_get_up_search_back_term_jump_attack_term(
        wk,
        0x42,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x400},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Branch_Wait_Area(wk, &(Branch_Wait_Args){0x14, 0xF, 5, 1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0067(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Branch_Wait_Area(wk, &(Branch_Wait_Args){0x14, 0xF, 5, 1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0068(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Lie(wk, 0);
        break;

    case 1:
        Approach_Walk(wk, 0x7F, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0069(PLW* wk) {
    pattern_wait_get_up(wk, 0, -1);
}

void Passive11_0070(PLW* wk) {
    pattern_wait_get_up_search_back_term_jump_attack_term(
        wk,
        0x42,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 0xB, 0x400, 0, -0x7FB0, -1, 0x200},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive11_0071(PLW* wk) {
    pattern_wait_lie_jump(wk);
}

void Passive11_0072(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1D, 10, 0x70});
}

void Passive11_0073(PLW* wk) {
    pattern_wait_get_up(wk, 3, -1);
}

void Passive11_0074(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-0x7FC0, -0x7FF0, 6, 1, -1}, 8, 0x100);
}

void Passive11_0075(PLW* wk) {
    pattern_em_term_normal_attack_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, -0x7FC0, 0, 1, -1},
        0x102,
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive11_0076(PLW* wk) {
    pattern_approach_walk_normal_attack_2(wk, 0xB, 0x402);
}

void Passive11_0077(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1}, 8, 0x400);
}

void Passive11_0078(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x220);
}

void Passive11_0079(PLW* wk) {
    pattern_pierce_on_command_attack_branch_unit_area(
        wk,
        &(Command_Attack_Args){8, 0, -1, -1},
        &(Branch_Menu_Args){6, 0x53, 0x2F, 0x24, 0x25}
    );
}

void Passive11_0080(PLW* wk) {
    pattern_jump_attack_term_normal_attack_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202,
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive11_0081(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40});
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7FA9, -0x8000, 4, 6, 0x94});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0082(PLW* wk) {
    pattern_jump_attack_term_normal_attack_com_random_select(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200},
        &(Branch_Menu_Args){2, 0x2D, 0x94, 0x94, 0xFF},
        0
    );
}

void Passive11_0083(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x110);
}

void Passive11_0084(PLW* wk) {
    pattern_lever_attack(wk, 0xFFFF, 0x110);
}

void Passive11_0085(PLW* wk) {
    pattern_lever_attack(wk, 1, 0x110);
}

void Passive11_0086(PLW* wk) {
    pattern_approach_walk(wk, 0xBF);
}

void Passive11_0087(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0, -1, -1});
}

void Passive11_0088(PLW* wk) {
    pattern_jump(wk, 0);
}

void Passive11_0089(PLW* wk) {
    pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, -1, -1},
        &(Command_Attack_Args){8, 0x1D, 9, -1}
    );
}

void Passive11_0090(PLW* wk) {
    pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 10, 0x700});
}

void Passive11_0091(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Lever_Attack(wk, 0xC, 1, 0x200);
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0092(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0, -1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0093(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, 0x28, 7, 1, -1});
        break;

    case 1:
        Lever_Attack(wk, 9, 1, 0x200);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0094(PLW* wk) {
    pattern_approach_walk_normal_attack_j_command_attack_2(wk, 0x47, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive11_0095(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive11_0096(PLW* wk) {
    pattern_sa_term_normal_attack(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F}, 8, 0x400);
}

void Passive11_0097(PLW* wk) {
    pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1},
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0098(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1}, 8, 0x40);
}

void Passive11_0099(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x202);
}

void Passive11_0100(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x402);
}

void Passive11_0101(PLW* wk) {
    pattern_em_term_normal_attack_sa_term(wk);
}

void Passive11_0102(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -1, -1, -1});
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7FA9, -0x8000, 4, 6, 0x94});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0103(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 0xB, 0x202, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive11_0104(PLW* wk) {
    pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x77, 0x7F, 0x81, 0x87}, 0);
}

void Passive11_0105(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0106(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 8, 0x200, 0, -1, -1, -1});
}

void Passive11_0107(PLW* wk) {
    passive11_pattern_em_term_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Passive11_0108(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive11_0109(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC8, 0, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive11_0110(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term_3(
        wk,
        0x47,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive11_0111(PLW* wk) {
    pattern_approach_walk_normal_attack_sa_term_2(
        wk,
        0x47,
        &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F},
        &(Command_Attack_Args){8, 0x1D, 10, -1}
    );
}

void Passive11_0112(PLW* wk) {
    pattern_approach_walk(wk, 0x7F);
}

void Passive11_0113(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 1:
        Normal_Attack_SP(wk, 8, 0x200, 0x12);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0114(PLW* wk) {
    pattern_jump_attack_term_normal_attack_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20},
        0x202,
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive11_0115(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1D, 10, -1});
        break;

    case 3:
        Wait(wk, 0xA);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x30, 0x31, 0x32, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive11_0116(PLW* wk) {
    pattern_forced_guard(wk, 2);
}

void Passive11_0117(PLW* wk) {
    pattern_normal_attack_command_attack_2(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive11_0118(PLW* wk) {
    pattern_normal_attack_j_command_attack_2(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive11_0119(PLW* wk) {
    pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x1E, 10, -1, -1, 0x30, 2, -1, -1, 0xFFFF});
}

void Passive11_0120(PLW* wk) {
    pattern_normal_attack(wk, 9, 0x12);
}

void Passive11_0121(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-1, -0x7FE8, 6, 1, -1}, 9, 0x102);
}

void Passive11_0122(PLW* wk) {
    pattern_normal_attack_2(wk, 9, 0x102, 0x202);
}
