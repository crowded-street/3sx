/**
 * @file pass06.c
 * COM Passive: Hugo
 */

#include "sf33rd/Source/Game/com/passive/pass06.h"
#include "sf33rd/Source/Game/com/passive/pass06_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive06_Tbl[147])();

void passive06_pattern_wait_get_up_normal_attack(PLW* wk, u16 lever_data, s16 option, u16 lever_data_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, lever_data, option);
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive06_pattern_approach_walk_em_term_command_attack_2(PLW* wk, s16 target_pos, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7F80, 6, 1, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    case 3:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive06_pattern_approach_walk_wait_get_up_wait(PLW* wk, u16 lever_data) {
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

    case 4:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive06_pattern_approach_walk_check_ex_pierce_on(PLW* wk, s16 target_pos, s16 next_menu) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, target_pos, 2);
        break;

    case 1:
        Check_EX(wk, 6, next_menu);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1F, 10, 0x70});
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive06_pattern_hi_jump_attack_term_normal_attack(PLW* wk, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-0x7F88, -0x7FA8, 9, 0x400, 0, -0x7F68, -1, 0x400});
        break;

    case 1:
        Normal_Attack(wk, 8, lever_data);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void passive06_pattern_wait_get_up_com_random_select(PLW* wk, u16 lever_data, const Branch_Menu_Args* p) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, lever_data, 0);
        break;

    case 1:
        Com_Random_Select(wk, p, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06(PLW* wk) {
    Passive06_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive06_0000(PLW* wk) {
    pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive06_0001(PLW* wk) {
    pattern_lever_off_look(wk);
}

void Passive06_0002(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB8, 6, 1, -1}, 0x20);
}

void Passive06_0003(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7F80, 6, 1, -1}, 0x40);
}

void Passive06_0004(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x12);
}

void Passive06_0005(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x22);
}

void Passive06_0006(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB8, 6, 1, -1}, 0x42);
}

void Passive06_0007(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x100);
}

void Passive06_0008(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1}, 0x200);
}

void Passive06_0009(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x400);
}

void Passive06_0010(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x102);
}

void Passive06_0011(PLW* wk) {
    pattern_normal_attack(wk, 8, 0x202);
}

void Passive06_0012(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1}, 0x402);
}

void Passive06_0013(PLW* wk) {
    pattern_approach_walk_command_attack(wk, 0x70, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Passive06_0014(PLW* wk) {
    pattern_approach_walk_command_attack(wk, 0x68, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Passive06_0015(PLW* wk) {
    pattern_approach_walk_command_attack(wk, 0x60, &(Command_Attack_Args){8, 0x1C, 10, -1});
}

void Passive06_0016(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive06_0017(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive06_0018(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive06_0019(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Passive06_0020(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Passive06_0021(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive06_0022(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Passive06_0023(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Passive06_0024(PLW* wk) {
    pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x20, 10, -1});
}

void Passive06_0025(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x70,
        &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Passive06_0026(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x68,
        &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Passive06_0027(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x60,
        &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1C, 10, -1}
    );
}

void Passive06_0028(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x80,
        &(EM_Term_Params){-1, -0x7F80, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive06_0029(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x80,
        &(EM_Term_Params){-1, -0x7F80, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 10, 0x70}
    );
}

void Passive06_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x78, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){1, 0xFFFF, 3, 0x78});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0031(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x78, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7F80, 6, 1, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1F, 10, 0x70});
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 2, 3, 0x78});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0032(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x44, &(EM_Term_Params){-1, -0x7FFC, 6, 1, -1}, 0x110);
}

void Passive06_0033(PLW* wk) {
    pattern_approach_walk_em_term_lever_attack(wk, 0x44, &(EM_Term_Params){-1, -0x7FFC, 6, 1, -1}, 0);
}

void Passive06_0034(PLW* wk) {
    passive06_pattern_approach_walk_em_term_command_attack_2(wk, 0x80, 0x100);
}

void Passive06_0035(PLW* wk) {
    passive06_pattern_approach_walk_em_term_command_attack_2(wk, 0x60, 0x200);
}

void Passive06_0036(PLW* wk) {
    passive06_pattern_approach_walk_em_term_command_attack_2(wk, 0x80, 0x10);
}

void Passive06_0037(PLW* wk) {
    passive06_pattern_approach_walk_em_term_command_attack_2(wk, 0x80, 0x20);
}

void Passive06_0038(PLW* wk) {
    passive06_pattern_approach_walk_em_term_command_attack_2(wk, 0x80, 0x40);
}

void Passive06_0039(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x80,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x20, 8, -1}
    );
}

void Passive06_0040(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x80,
        &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x20, 8, 0x700}
    );
}

void Passive06_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x80, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7F80, 6, 1, -1});
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, 0x70});
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0042(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x80, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7F80, 6, 1, -1});
        break;

    case 2:
        Check_EX(wk, 6, 0x18);
        break;

    case 3:
        Pierce_On(wk);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1F, 10, 0x70});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0043(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F60, -1, 6, 1, -1}, 0x20);
}

void Passive06_0044(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F60, -1, 6, 1, -1}, 0x40);
}

void Passive06_0045(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F60, -1, 6, 1, -1}, 0x200);
}

void Passive06_0046(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F60, -1, 6, 1, -1}, 0x400);
}

void Passive06_0047(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F60, -1, 6, 1, -1}, 0x10);
}

void Passive06_0048(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x9F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){1, 0xFFFF, 3, 0xFFFF});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0049(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F60, -1, 6, 1, -1}, 0x42);
}

void Passive06_0050(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F40, -1, 6, 1, -1}, 0x402);
}

void Passive06_0051(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F60, -1, 6, 1, -1});
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F90, 0x38, 8, 0x400, 2, -1, -1, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0052(PLW* wk) {
    pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F60, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive06_0053(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 2, 0xFFFF, 0xFFFF});
        break;

    case 1:
        Next_Another_Menu(wk, 6, 0x2F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x58, 2);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xB, 0x21, 10, -1});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0055(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x58, 2);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xB, 0x21, 10, -1});
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 2, 0xFFFF, 0xFFFF});
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0056(PLW* wk) {
    passive06_pattern_approach_walk_check_ex_pierce_on(wk, 0x58, 0x65);
}

void Passive06_0057(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FA0, 6, 1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x58, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 3:
        Pierce_On(wk);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){0xB, 0x21, 10, -1});
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0058(PLW* wk) {
    pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0xFFFF, 2, 0xFFFF, 0xFFFF},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive06_0059(PLW* wk) {
    pattern_search_back_term_pierce_on_command_attack_2(wk, 0x60, 0x2D, &(Branch_Menu_Args){6, 0x41, 0x2D, 0x31, 0x3C});
}

void Passive06_0060(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 8, 0x200, 0, -0x7FA8, -1, 0x40});
}

void Passive06_0061(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F88, -0x7FC0, 8, 0x400, 0, -0x7F88, -1, 0x40});
}

void Passive06_0062(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F68, -0x7FC0, 8, 0x200, 0, -0x7F68, -1, 0x400});
}

void Passive06_0063(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F68, -0x7FC0, 8, 0x400, 0, -0x7F68, -1, 0x400});
}

void Passive06_0064(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FA8, -1, 0x400});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive06_0065(PLW* wk) {
    pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F00, -1, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive06_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F60, -1, 6, 1, -1});
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1F, 10, 0x70});
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Passive06_Tbl[147])(PLW*) = {
    Passive06_0000, Passive06_0001, Passive06_0002, Passive06_0003, Passive06_0004, Passive06_0005, Passive06_0006,
    Passive06_0007, Passive06_0008, Passive06_0009, Passive06_0010, Passive06_0011, Passive06_0012, Passive06_0013,
    Passive06_0014, Passive06_0015, Passive06_0016, Passive06_0017, Passive06_0018, Passive06_0019, Passive06_0020,
    Passive06_0021, Passive06_0022, Passive06_0023, Passive06_0024, Passive06_0025, Passive06_0026, Passive06_0027,
    Passive06_0028, Passive06_0029, Passive06_0030, Passive06_0031, Passive06_0032, Passive06_0033, Passive06_0034,
    Passive06_0035, Passive06_0036, Passive06_0037, Passive06_0038, Passive06_0039, Passive06_0040, Passive06_0041,
    Passive06_0042, Passive06_0043, Passive06_0044, Passive06_0045, Passive06_0046, Passive06_0047, Passive06_0048,
    Passive06_0049, Passive06_0050, Passive06_0051, Passive06_0052, Passive06_0053, Passive06_0054, Passive06_0055,
    Passive06_0056, Passive06_0057, Passive06_0058, Passive06_0059, Passive06_0060, Passive06_0061, Passive06_0062,
    Passive06_0063, Passive06_0064, Passive06_0065, Passive06_0066, Passive06_0067, Passive06_0068, Passive06_0069,
    Passive06_0070, Passive06_0071, Passive06_0072, Passive06_0073, Passive06_0074, Passive06_0075, Passive06_0076,
    Passive06_0077, Passive06_0078, Passive06_0079, Passive06_0080, Passive06_0081, Passive06_0082, Passive06_0083,
    Passive06_0084, Passive06_0085, Passive06_0086, Passive06_0087, Passive06_0088, Passive06_0089, Passive06_0090,
    Passive06_0091, Passive06_0092, Passive06_0093, Passive06_0094, Passive06_0095, Passive06_0096, Passive06_0097,
    Passive06_0098, Passive06_0099, Passive06_0100, Passive06_0101, Passive06_0102, Passive06_0103, Passive06_0104,
    Passive06_0105, Passive06_0106, Passive06_0107, Passive06_0108, Passive06_0109, Passive06_0110, Passive06_0111,
    Passive06_0112, Passive06_0113, Passive06_0114, Passive06_0115, Passive06_0116, Passive06_0117, Passive06_0118,
    Passive06_0119, Passive06_0120, Passive06_0121, Passive06_0122, Passive06_0123, Passive06_0124, Passive06_0125,
    Passive06_0126, Passive06_0127, Passive06_0128, Passive06_0129, Passive06_0130, Passive06_0131, Passive06_0132,
    Passive06_0133, Passive06_0134, Passive06_0135, Passive06_0136, Passive06_0137, Passive06_0138, Passive06_0139,
    Passive06_0140, Passive06_0141, Passive06_0142, Passive06_0143, Passive06_0144, Passive06_0145, Passive06_0146
};
