/**
 * @file active04.c
 * COM Active: Dudley
 */

#include "sf33rd/Source/Game/com/active/active04.h"
#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern04_Tbl[68])();

void Computer04(PLW* wk) {
    Pattern04_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern04_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern04_0001(PLW* wk) {
    active_pattern_normal_attack_7(wk, 0x12, 0x12, 0x12);
}

void Pattern04_0002(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern04_0003(PLW* wk) {
    active_pattern_adjust_attack_4(wk, 8, 8, 0x10);
}

void Pattern04_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Pattern04_0005(PLW* wk) {
    active_pattern_approach_walk_normal_attack(wk, 0x47, 0x110);
}

void Pattern04_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern04_0007(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack(wk, 0x30, 2, 0x14);
}

void Pattern04_0008(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 8, 0x400, 0, -1, -1, -1});
}

void Pattern04_0009(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Pattern04_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern04_0011(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Pattern04_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Pattern04_0013(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern04_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC8, 8, 0x200, 2, -0x7F80, -1, 0x400});
}

void Pattern04_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern04_0016(PLW* wk) {
    active_pattern_walk(wk, 1, 0x30, 0);
}

void Pattern04_0017(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Pattern04_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x30, 2, 0x31);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Jump(wk, 1);
        break;

    case 3:
        Lever_Off(wk);
        break;

    case 4:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern04_0019(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){0x88, -1, 0, 2, 5}, 0, 0x40);
}

void Pattern04_0020(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1F, 8, -1},
        &(Command_Attack_Args){8, 0x1F, 0xA, -1}
    );
}

void Pattern04_0021(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Pattern04_0022(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC8, 8, 0x40, 0, -0x7F90, -1, 0x20});
}

void Pattern04_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x9F);
}

void Pattern04_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern04_0025(PLW* wk) {
    active_pattern_walk(wk, 1, 0x60, 0);
}

void Pattern04_0026(PLW* wk) {
    active_pattern_command_attack_lever_off_look(wk);
}

void Pattern04_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern04_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0xDF);
}

void Pattern04_0029(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 9, 0x102, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Pattern04_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Adjust_Attack(wk, 9, 0x100);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern04_0031(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x400, 0, -0x7F68, -1, 0x200},
        0x402
    );
}

void Pattern04_0032(PLW* wk) {
    active_pattern_adjust_attack_4(wk, 0xB, 8, 0x20);
}

void Pattern04_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Adjust_Attack(wk, 0xB, 0x10);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern04_0034(PLW* wk) {
    active_pattern_normal_attack_lever_off_look(wk, 8);
}

void Pattern04_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern04_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern04_0037(PLW* wk) {
    active_pattern_approach_walk_lever_attack(wk, 0x47, 0);
}

void Pattern04_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x35, 0xFFFF, 0x37, 0});
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Adjust_Attack(wk, 0xB, 0x20);
        break;

    case 3:
        Normal_Attack(wk, 0xA, 0x102);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern04_0039(PLW* wk) {
    active_pattern_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Pattern04_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Lever_Attack(wk, 9, 0, 0x200);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern04_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x400);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern04_0042(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern04_0043(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x40, 0, -1, 0x30, 0x4020});
}

void Pattern04_0044(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC0, 8, 0x40, 0, -1, 0x30, 0x400});
}

void Pattern04_0045(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FD0, 8, 0x400, 0, -1, -1, -1});
}

void Pattern04_0046(PLW* wk) {
    active_pattern_walk_2(wk, 0x15, 0x13, 0x10);
}

void Pattern04_0047(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(
        wk,
        &(JCA_Term_Args){8, 0x20, 0xA, -1, -0x7FA0, 0x30, 0, -1, 0x30, 0x400}
    );
}

void Pattern04_0048(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Lever_Attack(wk, 0xD, 0, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern04_0049(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern04_0050(PLW* wk) {
    active_pattern_hi_jump_attack(wk);
}

void Pattern04_0051(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, 0x30, 8, 0x40, 0, -1, -1, 0xFFFF});
}

void Pattern04_0052(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x20, 0xA, -1, -1, 0x30, 0, -1, -1, 0xFFFF});
}

void Pattern04_0053(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern04_0054(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8215, 0xA, -1});
}

void Pattern04_0055(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern04_0056(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern04_0057(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern04_0058(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        Lever_Attack(wk, 0xD, 0, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern04_0059(PLW* wk) {
    active_pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0x20, 8, -1}, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Pattern04_0060(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F90, 0, 0, 2, 0});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xD, 0x20, 0x609, -1});
        break;

    case 2:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 3:
        Normal_Attack(wk, 0xD, 0x100);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern04_0061(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x40, 0x41, 0x43, 0x43}, 1);
}

void Pattern04_0062(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){2, 0x40, 0x41, 0x42, 0x43}, 1);
}

void Pattern04_0063(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 8, 0x200, 0, 0 - 0x7F68, -1, 0x400});
}

void Pattern04_0064(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x21, 8, -1});
}

void Pattern04_0065(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x21, 9, -1});
}

void Pattern04_0066(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x21, 0xA, -1});
}

void Pattern04_0067(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x21, 0xA, 0x700});
}

void (*const Pattern04_Tbl[68])(PLW*) = {
    Pattern04_0000, Pattern04_0001, Pattern04_0002, Pattern04_0003, Pattern04_0004, Pattern04_0005, Pattern04_0006,
    Pattern04_0007, Pattern04_0008, Pattern04_0009, Pattern04_0010, Pattern04_0011, Pattern04_0012, Pattern04_0013,
    Pattern04_0014, Pattern04_0015, Pattern04_0016, Pattern04_0017, Pattern04_0018, Pattern04_0019, Pattern04_0020,
    Pattern04_0021, Pattern04_0022, Pattern04_0023, Pattern04_0024, Pattern04_0025, Pattern04_0026, Pattern04_0027,
    Pattern04_0028, Pattern04_0029, Pattern04_0030, Pattern04_0031, Pattern04_0032, Pattern04_0033, Pattern04_0034,
    Pattern04_0035, Pattern04_0036, Pattern04_0037, Pattern04_0038, Pattern04_0039, Pattern04_0040, Pattern04_0041,
    Pattern04_0042, Pattern04_0043, Pattern04_0044, Pattern04_0045, Pattern04_0046, Pattern04_0047, Pattern04_0048,
    Pattern04_0049, Pattern04_0050, Pattern04_0051, Pattern04_0052, Pattern04_0053, Pattern04_0054, Pattern04_0055,
    Pattern04_0056, Pattern04_0057, Pattern04_0058, Pattern04_0059, Pattern04_0060, Pattern04_0061, Pattern04_0062,
    Pattern04_0063, Pattern04_0064, Pattern04_0065, Pattern04_0066, Pattern04_0067
};
