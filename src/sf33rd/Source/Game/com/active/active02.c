/**
 * @file active02.c
 * COM Active: Ryu
 */

#include "sf33rd/Source/Game/com/active/active02.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern02_Tbl[75])();

void Computer02(PLW* wk) {
    Pattern02_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern02_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern02_0001(PLW* wk) {
    active_pattern_normal_attack_7(wk, 0x102, 0x102, 0x102);
}

void Pattern02_0002(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern02_0003(PLW* wk) {
    active_pattern_adjust_attack_4(wk, 8, 0, 0x10);
}

void Pattern02_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){1, 0x1D, 0xA, -1});
}

void Pattern02_0005(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x110);
}

void Pattern02_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern02_0007(PLW* wk) {
    pattern_pierce_on_command_attack(wk, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Pattern02_0008(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 0, 0x200, 0, -1, -1, -1});
}

void Pattern02_0009(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern02_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern02_0011(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Pattern02_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 9, 0x40);
}

void Pattern02_0013(PLW* wk) {
    active_pattern_normal_attack(wk, 9, 0x200);
}

void Pattern02_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC8, 8, 0x200, 2, -0x7F80, -1, 0x200});
}

void Pattern02_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern02_0016(PLW* wk) {
    active_pattern_walk(wk, 1, 0x30, 0);
}

void Pattern02_0017(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern02_0018(PLW* wk) {
    active_pattern_jump_lever_off_look(wk);
}

void Pattern02_0019(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){0x88, -1, 0, 2, 5},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Pattern02_0020(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x41, 0x42, 0x43, 0x44}, 1);
}

void Pattern02_0021(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern02_0022(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC8, 8, 0x40, 0, -0x7f90, -1, 0x100});
}

void Pattern02_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Pattern02_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern02_0025(PLW* wk) {
    active_pattern_walk(wk, 1, 0x60, 0);
}

void Pattern02_0026(PLW* wk) {
    active_pattern_command_attack_lever_off_look(wk);
}

void Pattern02_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern02_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Pattern02_0029(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Pattern02_0030(PLW* wk) {
    active_pattern_adjust_attack_lever_attack_2(wk, 9, 0x100, 0);
}

void Pattern02_0031(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x200, 0, -0x7f68, -1, 0x200},
        0x402
    );
}

void Pattern02_0032(PLW* wk) {
    active_pattern_adjust_attack_4(wk, 0xB, 8, 0x20);
}

void Pattern02_0033(PLW* wk) {
    active_pattern_adjust_attack_j_command_attack(wk, 9, 0x10, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern02_0034(PLW* wk) {
    active_pattern_normal_attack_lever_off_look(wk, 0);
}

void Pattern02_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern02_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern02_0037(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 1, 0x110);
}

void Pattern02_0038(PLW* wk) {
    active_pattern_adjust_attack_normal_attack_command_attack(wk, 0x20, 0xA, 0x202);
}

void Pattern02_0039(PLW* wk) {
    active_pattern_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x1C, 8, -1}
    );
}

void Pattern02_0040(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 0xC, 0x202, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Pattern02_0041(PLW* wk) {
    active_pattern_approach_walk_normal_attack_j_command_attack(wk, 0x37, 9, 0x40);
}

void Pattern02_0042(PLW* wk) {
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

void Pattern02_0043(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -1, 0x30, 0x401E});
}

void Pattern02_0044(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x40, 0, -1, 0x30, 0x400});
}

void Pattern02_0045(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x400, 0, -1, -1, -1});
}

void Pattern02_0046(PLW* wk) {
    active_pattern_walk_2(wk, 0x15, 0x13, 0x10);
}

void Pattern02_0047(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(
        wk,
        &(JCA_Term_Args){8, 0x1E, 0xA, -1, -0x7FA0, 0x30, 0, -1, 0x30, 0x400}
    );
}

void Pattern02_0048(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){0x50, -1, 5, 2, 0});
        break;

    case 1:
        Jump_Attack_Term(wk, &(Jump_Term_Args){0x8060, 0x8040, 9, 0x400, 0, 0x8080, -1, 0x200});
        break;

    case 2:
        Normal_Attack(wk, 9, 0x42);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xc, 0x1d, 10, -1});
        break;

    case 4:
        Wait(wk, 5);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0x47});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern02_0049(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FA0, -1, 0x400},
        0x40,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0x47}
    );
}

void Pattern02_0050(PLW* wk) {
    active_pattern_hi_jump_attack(wk);
}

void Pattern02_0051(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, 0x30, 8, 0x40, 0, -1, -1, 0xFFFF});
}

void Pattern02_0052(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x1E, 0xA, -1, -1, 0x30, 0, -1, -1, 0xFFFF});
}

void Pattern02_0053(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern02_0054(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern02_0055(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Setup_DENJIN_LEVEL(wk);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern02_0056(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 8, -1});
}

void Pattern02_0057(PLW* wk) {
    active_pattern_command_attack_push_shot(wk, &(Command_Attack_Args){8, 0x8015, 9, -1}, 5);
}

void Pattern02_0058(PLW* wk) {
    active_pattern_command_attack_push_shot(wk, &(Command_Attack_Args){8, 0x8015, 9, -1}, 0xB);
}

void Pattern02_0059(PLW* wk) {
    active_pattern_command_attack_push_shot(wk, &(Command_Attack_Args){8, 0x8015, 9, -1}, 0x10);
}

void Pattern02_0060(PLW* wk) {
    active_pattern_command_attack_push_shot(wk, &(Command_Attack_Args){8, 0x8015, 0xA, -1}, 0xFF);
}

void Pattern02_0061(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    default:
        End_Pattern(wk);
    }
}

void Pattern02_0062(PLW* wk) {
    pattern_sa_term_2(wk);
}

void Pattern02_0063(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 8, 0x200, 0, -0x7F68, -1, 0x400});
}

void Pattern02_0064(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack_command_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7F80, -1, 0x400},
        0x202,
        &(SA_Term_Args){0x35, 0x36, 0x37, 0}
    );
}

void Pattern02_0065(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1D, 0xA, -1},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Pattern02_0066(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1D, 0xA, -1},
        &(Command_Attack_Args){8, 0x1D, 8, -1}
    );
}

void Pattern02_0067(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1D, 9, -1},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Pattern02_0068(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1D, 8, -1},
        &(Command_Attack_Args){8, 0x1D, 0xA, -1}
    );
}

void Pattern02_0069(PLW* wk) {
    active_pattern_command_attack_lever_attack(wk, &(Command_Attack_Args){8, 0, 0xB, -1}, 0, 0x110);
}

void Pattern02_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 2, 0x14);
        break;

    case 1:
        Jump(wk, 1);
        break;

    case 2:
        Next_Another_Menu(wk, 2, 0x14);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern02_0071(PLW* wk) {
    active_pattern_etc_term_provoke(wk, 2, 0x17);
}

void Pattern02_0072(PLW* wk) {
    active_pattern_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x2E, 8, -1, -0x7FA0, 0x48, 0, -1, 0x30, 0x20});
}

void Pattern02_0073(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x40);
}

void Pattern02_0074(PLW* wk) {
    pattern_lever_attack_j_command_attack(wk, 0x40, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void (*const Pattern02_Tbl[75])(PLW*) = {
    Pattern02_0000, Pattern02_0001, Pattern02_0002, Pattern02_0003, Pattern02_0004, Pattern02_0005, Pattern02_0006,
    Pattern02_0007, Pattern02_0008, Pattern02_0009, Pattern02_0010, Pattern02_0011, Pattern02_0012, Pattern02_0013,
    Pattern02_0014, Pattern02_0015, Pattern02_0016, Pattern02_0017, Pattern02_0018, Pattern02_0019, Pattern02_0020,
    Pattern02_0021, Pattern02_0022, Pattern02_0023, Pattern02_0024, Pattern02_0025, Pattern02_0026, Pattern02_0027,
    Pattern02_0028, Pattern02_0029, Pattern02_0030, Pattern02_0031, Pattern02_0032, Pattern02_0033, Pattern02_0034,
    Pattern02_0035, Pattern02_0036, Pattern02_0037, Pattern02_0038, Pattern02_0039, Pattern02_0040, Pattern02_0041,
    Pattern02_0042, Pattern02_0043, Pattern02_0044, Pattern02_0045, Pattern02_0046, Pattern02_0047, Pattern02_0048,
    Pattern02_0049, Pattern02_0050, Pattern02_0051, Pattern02_0052, Pattern02_0053, Pattern02_0054, Pattern02_0055,
    Pattern02_0056, Pattern02_0057, Pattern02_0058, Pattern02_0059, Pattern02_0060, Pattern02_0061, Pattern02_0062,
    Pattern02_0063, Pattern02_0064, Pattern02_0065, Pattern02_0066, Pattern02_0067, Pattern02_0068, Pattern02_0069,
    Pattern02_0070, Pattern02_0071, Pattern02_0072, Pattern02_0073, Pattern02_0074
};
