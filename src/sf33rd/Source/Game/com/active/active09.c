/**
 * @file active09.c
 * COM Active: Oro
 */

#include "sf33rd/Source/Game/com/active/active09.h"
#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern09_Tbl[102])();

void Computer09(PLW* wk) {
    Pattern09_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern09_0000(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x47, 0x47, 0x46, 0x46}, 1);
}

void Pattern09_0001(PLW* wk) {
    active_pattern_normal_attack_3(wk, 0x102, 9, 0x102);
}

void Pattern09_0002(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FC0, -1, 5, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Pattern09_0003(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7FA0, -1, 5, 1, -1});
        break;

    case 1:
        Adjust_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){1, 0x1E, 8, -1});
}

void Pattern09_0005(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x110);
}

void Pattern09_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern09_0007(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack(wk, 0x60, 1, -1);
}

void Pattern09_0008(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 0, 0x200, 0, -1, -1, -1});
}

void Pattern09_0009(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FC0, -1, 5, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Pattern09_0010(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FA0, -1, 5, 1, -1}, 8, 0x402);
}

void Pattern09_0011(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FA0, -1, 5, 1, -1}, 8, 0x202);
}

void Pattern09_0012(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FA0, -1, 5, 1, -1}, 9, 0x40);
}

void Pattern09_0013(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F98, -1, 5, 1, -1}, 9, 0x200);
}

void Pattern09_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC8, 8, 0x200, 2, -0x7F80, -1, 0x200});
}

void Pattern09_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern09_0016(PLW* wk) {
    active_pattern_walk(wk, 1, 0x30, 0);
}

void Pattern09_0017(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){0x58, -1, 5, 2, 0},
        &(Command_Attack_Args){8, 0x1F, 8, -1}
    );
}

void Pattern09_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, 1);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0019(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F98, -1, 0, 2, 5},
        &(Command_Attack_Args){8, 0x1D, 8, -1}
    );
}

void Pattern09_0020(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Lever_On(wk, 1, 2);
        break;

    case 2:
        Com_Random_Select(wk, &(Branch_Menu_Args){2, 0x41, 0x42, 0x43, 0x44}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0021(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F10, -1, 5, 2, 0},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Pattern09_0022(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F98, -0x7FA8, 8, 0x40, 0, -0x7F90, -1, 0x100});
}

void Pattern09_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Pattern09_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern09_0025(PLW* wk) {
    active_pattern_walk(wk, 1, 0x60, 0);
}

void Pattern09_0026(PLW* wk) {
    active_pattern_command_attack_look(wk);
}

void Pattern09_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern09_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Pattern09_0029(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 1, -1});
        break;

    case 1:
        Adjust_Attack(wk, 9, 0x100);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7FB0, -1, 5, 1, -1});
        break;

    case 1:
        Adjust_Attack(wk, 9, 0x100);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0031(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x400, 0, -0x7F68, -1, 0x200},
        0x402
    );
}

void Pattern09_0032(PLW* wk) {
    active_pattern_adjust_attack_4(wk, 9, 8, 0x20);
}

void Pattern09_0033(PLW* wk) {
    active_pattern_adjust_attack_j_command_attack(wk, 9, 0x10, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Pattern09_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0, 0x100);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0035(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FA0, -1, 5, 1, -1}, 8, 0x20);
}

void Pattern09_0036(PLW* wk) {
    active_pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7F60, -1, 5, 2, 0},
        &(Command_Attack_Args){8, 0x1F, 9, -1}
    );
}

void Pattern09_0037(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 1, 0x110);
}

void Pattern09_0038(PLW* wk) {
    active_pattern_normal_attack_7(wk, 0x100, 0x10, 0x20);
}

void Pattern09_0039(PLW* wk) {
    active_pattern_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Pattern09_0040(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 9, 0x202, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Pattern09_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7FA0, -1, 5, 1, -1});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0042(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0043(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F98, -0x7FA8, 8, 0x40, 0, -1, 0x30, 0x401F});
}

void Pattern09_0044(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FB0, 8, 0x40, 0, -1, 0x30, 0x400});
}

void Pattern09_0045(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x400, 0, -1, -1, -1});
}

void Pattern09_0046(PLW* wk) {
    active_pattern_walk_2(wk, 0x15, 0x13, 0x10);
}

void Pattern09_0047(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(
        wk,
        &(JCA_Term_Args){8, 0x1F, 0xA, -1, -0x7FA0, 0x30, 0, -1, 0x30, 0x400}
    );
}

void Pattern09_0048(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, -0x5FF7, 0x400, 0, -0x7FB0, -1, 0x400},
        0x22,
        &(Command_Attack_Args){0xC, 0x1C, 0xA, -1}
    );
}

void Pattern09_0049(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F98, -0x7FA8, 9, 0x40, 0, -0x7FB0, -1, 0x400},
        0x200,
        &(Command_Attack_Args){0xC, 0x1D, 8, -1}
    );
}

void Pattern09_0050(PLW* wk) {
    active_pattern_hi_jump_attack(wk);
}

void Pattern09_0051(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, 0x30, 8, 0x40, 0, -1, -1, -1});
}

void Pattern09_0052(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x1F, 0xA, -1, -1, 0x30, 0, -1, -1, -1});
}

void Pattern09_0053(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x39, 0x39, 0x3D, 0x3D}, 1);
}

void Pattern09_0054(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 8, -1});
}

void Pattern09_0055(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 9, -1});
}

void Pattern09_0056(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 0xA, -1});
}

void Pattern09_0057(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8018, 9, -1});
}

void Pattern09_0058(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x39, 0x39, 0x3C, 0x3C}, 1);
}

void Pattern09_0059(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F90, -1, 5, 1, -1}, 9, 0x400);
}

void Pattern09_0060(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern09_0061(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern09_0062(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){-1, 0x64, -1, 0});
}

void Pattern09_0063(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 8, 0x200, 0, -0x7F68, -1, 0x400});
}

void Pattern09_0064(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FA8, 9, 0x40, 0, -0x7FB0, -1, 0x400});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 0xA, -1});
        break;

    case 3:
        Wait(wk, 5);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x64, 0x3A, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0065(PLW* wk) {
    active_pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 8, -1}, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern09_0066(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1E, 0xA, -1},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Pattern09_0067(PLW* wk) {
    active_pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 8, -1}, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern09_0068(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1E, 8, -1},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Pattern09_0069(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2F, 9, -1, -1, -0x7FC0, 0, -1, -1, -1});
}

void Pattern09_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0071(PLW* wk) {
    active_pattern_lever_on_look(wk);
}

void Pattern09_0072(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FA0, -0x7FF0, 5, 1, -1}, 9, 0x40);
}

void Pattern09_0073(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7FC0, -0x7FF0, 5, 1, -1});
        break;

    case 1:
        Adjust_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0074(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FB8, -0x7FF0, 5, 1, -1}, 8, 0x20);
}

void Pattern09_0075(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC8, 8, 0x40, 0, -0x7FA0, -1, 0x40});
}

void Pattern09_0076(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x200, 0, -0x7FA0, -1, 0x20},
        0x402
    );
}

void Pattern09_0077(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack_2(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, -0x5FF7, 0x400, 0, -0x7FA0, -1, 0x40},
        0x22,
        &(Command_Attack_Args){0xC, 0x1C, 0xA, -1}
    );
}

void Pattern09_0078(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 2, 0x4F);
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x35, -1, -1, 0x9F});
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){-1, 100, 0x3A, 0});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x8016, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0079(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x35, -1, -1, 0x9F});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){-1, 0x64, 0x3A, 0});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0080(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0081(PLW* wk) {
    active_pattern_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FD0, 0x2008, 0x200, 0, -0x7FA0, -1, 0x20},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Pattern09_0082(PLW* wk) {
    active_pattern_etc_term_provoke(wk, 2, 0);
}

void Pattern09_0083(PLW* wk) {
    active_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 0, 0x30, 1, -1, -0x7FA0, 0x400, -0x7FB0, -0x7FD0, 0x400});
}

void Pattern09_0084(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FD0, 0x10, 8, 0x110, 0, -0x7FA0, -1, 0x20});
}

void Pattern09_0085(PLW* wk) {
    active_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 2, 0x40, 1, -1, -1, -1, -0x7FB0, -0x7FD0, 0x400});
}

void Pattern09_0086(PLW* wk) {
    active_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 0, 0x50, 1, -1, -1, -1, -0x7FB0, -0x7FD0, 0x400});
}

void Pattern09_0087(PLW* wk) {
    active_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 1, 0x40, 2, -1, -1, -1, -0x7FB0, -0x7FD0, 0x400});
}

void Pattern09_0088(PLW* wk) {
    active_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 1, 0x40, 1, -1, -1, -1, -0x7FB0, -0x7FD0, 0x400});
}

void Pattern09_0089(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x55, 0x56, 0x57, 0x58}, 1);
}

void Pattern09_0090(PLW* wk) {
    active_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 0, 0x50, 0, -1, -1, -1, -0x7FB0, -0x7FD0, 0x400});
}

void Pattern09_0091(PLW* wk) {
    active_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 0, 0x50, 2, -1, -1, -1, -0x7FB0, -0x7FD0, 0x400});
}

void Pattern09_0092(PLW* wk) {
    active_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 2, 0x40, 0, -1, -1, -1, -0x7FB0, -0x7FD0, 0x400});
}

void Pattern09_0093(PLW* wk) {
    active_pattern_oro_ja_term(wk, &(ORO_Air_Term_Args){8, 1, 0x50, 0, -1, -1, -1, -0x7FB0, -0x7FD0, 0x400});
}

void Pattern09_0094(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x5A, 0x5B, 0x5C, 0x5D}, 1);
}

void Pattern09_0095(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_JCA_Term(wk, &(ORO_JCA_Term_Args){8, 0, 0x40, 1, -1, -0x7FB0, 0x2F, 0xA, -1, -0x7FB0, -0x7FD0, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0096(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_JCA_Term(wk, &(ORO_JCA_Term_Args){8, 0, 0x40, 0, -0x7FA0, -1, 0x2F, 0xA, -1, -0x7FB0, -0x7FD0, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0097(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_HJCA_Term(wk, &(ORO_JCA_Term_Args){8, 0, 0x40, 0, -0x7FA0, -1, 0x2F, 0xA, -1, -0x7FB0, -0x7FD0, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0098(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_HJA_Term(wk, &(ORO_Air_Term_Args){8, 1, 0x50, 0, -1, -1, -1, -0x7FB0, -0x7FD0, 0x400});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0099(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x35, -1, -1, 0x9F});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){-1, -1, 0x3A, 0});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x8015, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern09_0100(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x36, 0x37, 0x38, 0x39}, 1);
}

void Pattern09_0101(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, -1, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Pattern09_Tbl[102])(PLW*) = {
    Pattern09_0000, Pattern09_0001, Pattern09_0002, Pattern09_0003, Pattern09_0004, Pattern09_0005, Pattern09_0006,
    Pattern09_0007, Pattern09_0008, Pattern09_0009, Pattern09_0010, Pattern09_0011, Pattern09_0012, Pattern09_0013,
    Pattern09_0014, Pattern09_0015, Pattern09_0016, Pattern09_0017, Pattern09_0018, Pattern09_0019, Pattern09_0020,
    Pattern09_0021, Pattern09_0022, Pattern09_0023, Pattern09_0024, Pattern09_0025, Pattern09_0026, Pattern09_0027,
    Pattern09_0028, Pattern09_0029, Pattern09_0030, Pattern09_0031, Pattern09_0032, Pattern09_0033, Pattern09_0034,
    Pattern09_0035, Pattern09_0036, Pattern09_0037, Pattern09_0038, Pattern09_0039, Pattern09_0040, Pattern09_0041,
    Pattern09_0042, Pattern09_0043, Pattern09_0044, Pattern09_0045, Pattern09_0046, Pattern09_0047, Pattern09_0048,
    Pattern09_0049, Pattern09_0050, Pattern09_0051, Pattern09_0052, Pattern09_0053, Pattern09_0054, Pattern09_0055,
    Pattern09_0056, Pattern09_0057, Pattern09_0058, Pattern09_0059, Pattern09_0060, Pattern09_0061, Pattern09_0062,
    Pattern09_0063, Pattern09_0064, Pattern09_0065, Pattern09_0066, Pattern09_0067, Pattern09_0068, Pattern09_0069,
    Pattern09_0070, Pattern09_0071, Pattern09_0072, Pattern09_0073, Pattern09_0074, Pattern09_0075, Pattern09_0076,
    Pattern09_0077, Pattern09_0078, Pattern09_0079, Pattern09_0080, Pattern09_0081, Pattern09_0082, Pattern09_0083,
    Pattern09_0084, Pattern09_0085, Pattern09_0086, Pattern09_0087, Pattern09_0088, Pattern09_0089, Pattern09_0090,
    Pattern09_0091, Pattern09_0092, Pattern09_0093, Pattern09_0094, Pattern09_0095, Pattern09_0096, Pattern09_0097,
    Pattern09_0098, Pattern09_0099, Pattern09_0100, Pattern09_0101
};
