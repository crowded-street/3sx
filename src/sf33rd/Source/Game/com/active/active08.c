/**
 * @file active08.c
 * COM Active: Elena
 */

#include "sf33rd/Source/Game/com/active/active08.h"
#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern08_Tbl[71])();

static void pattern08_0034_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Computer08(PLW* wk) {
    Pattern08_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern08_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern08_0001(PLW* wk) {
    active_pattern_normal_attack_7(wk, 0x102, 0x102, 0x402);
}

void Pattern08_0002(PLW* wk) {
    active_pattern_normal_attack_3(wk, 0x100, 9, 0x102);
}

void Pattern08_0003(PLW* wk) {
    active_pattern_adjust_attack_4(wk, 9, 8, 0x10);
}

void Pattern08_0004(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, &(Command_Attack_Args){0xB, 0x1E, 9, -1});
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0005(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0006(PLW* wk) {
    active_pattern_wait(wk, 0);
}

void Pattern08_0007(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 3);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0008(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 8, 0x200, 0, -0x7F98, -1, 0x10});
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0009(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){9, 0x1E, 0xA, -1});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern08_0011(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){9, 0x1E, 0xA, -1});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Pattern08_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0014(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FC8, 8, 0x200, 2, -1, -0x7FF0, 0x100},
        0x402
    );
}

void Pattern08_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern08_0016(PLW* wk) {
    active_pattern_walk(wk, 1, 0x30, 0);
}

void Pattern08_0017(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Pattern08_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 1:
        Jump(wk, 1);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, 0xA, -1});
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

void Pattern08_0019(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){0x70, -1, 0, 2, 5}, 0, 0x200);
}

void Pattern08_0020(PLW* wk) {
    active_pattern_j_command_attack_2(
        wk,
        &(Command_Attack_Args){9, 0x1D, 0xA, -1},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Pattern08_0021(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
}

void Pattern08_0022(PLW* wk) {
    active_pattern_jump_attack_term_j_command_attack(
        wk,
        &(Jump_Term_Args){-0x7F98, -0x7FC0, 9, 0x40, 0, -0x7F90, -1, 0x100},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Pattern08_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7B);
}

void Pattern08_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern08_0025(PLW* wk) {
    active_pattern_walk(wk, 1, 0x60, 0);
}

void Pattern08_0026(PLW* wk) {
    active_pattern_command_attack_lever_off_look(wk);
}

void Pattern08_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern08_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0xBF);
}

void Pattern08_0029(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x40);
        break;

    case 3:
        Lever_Attack(wk, 8, 0, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0031(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F98, -0x7FC0, 8, 0x200, 0, -0x7F68, -1, 0x200},
        0x402
    );
}

void Pattern08_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xB, 0x10);
        break;

    case 1:
        Adjust_Attack(wk, 0xB, 0x20);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 9, 0, 0x20);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xb, 0x100);
        break;

    case 1:
        Normal_Attack(wk, 0xb, 0x40);
        break;

    case 2:
        Normal_Attack(wk, 0xb, 0x402);
        break;

    case 3:
        Pierce_On(wk);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 10, -1});
        break;

    case 5:
        Lever_Off(wk);
        break;

    default:
        pattern08_0034_from_step_6(wk);
        break;
    }
}

void Pattern08_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern08_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern08_0037(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 1, 0x110);
}

void Pattern08_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0});
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Adjust_Attack(wk, 8, 0x200);
        break;

    case 3:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    case 4:
        Lever_Attack(wk, 8, 0, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0039(PLW* wk) {
    active_pattern_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x1C, 9, -1}
    );
}

void Pattern08_0040(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 9, 0x202, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Pattern08_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x40);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0042(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 8, 0x200, 0, -0x7F70, -1, 0x400});
}

void Pattern08_0043(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F98, -0x7FC0, 8, 0x40, 0, -1, -0x7FE0, 0x200});
}

void Pattern08_0044(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F98, -0x7FC0, 8, 0x40, 0, -0x7F70, -1, 0x200});
        break;
    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;
    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F98, -0x7FC0, 0xB, 0x400, 0, -0x7F70, -1, 0x20});
        break;

    case 1:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0046(PLW* wk) {
    active_pattern_walk_2(wk, 0x15, 0x13, 0x10);
}

void Pattern08_0047(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x20, 0);
        break;

    case 1:
        Walk(wk, 0, 0x14, 0);
        break;

    case 2:
        Walk(wk, 1, 0x18, 0);
        break;

    case 3:
        Wait(wk, 0x14);
        break;

    case 4:
        Walk(wk, 0, 0x14, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0048(PLW* wk) {
    active_pattern_wait(wk, 0x3C);
}

void Pattern08_0049(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0, 0xA, -1});
}

void Pattern08_0050(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7F98, -0x7FB8, 8, 0x400, 2, -1, -0x7FF0, 0x10});
}

void Pattern08_0051(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7F98, -0x7FB8, 8, 0x40, 0, -1, -0x7FF0, 0x10});
}

void Pattern08_0052(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7F98, -0x7FB8, 8, 0x40, 0, -1, -0x7FF0, 0x10});
}

void Pattern08_0053(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, -1});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0054(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Pattern08_0055(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 10, -1});
        break;

    case 3:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 10, -1});
        break;

    case 5:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1d, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0056(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 10, -1});
        break;

    case 3:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 10, -1});
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){-1, -1, 0x3b, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0057(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern08_0058(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 0xA, -1});
}

void Pattern08_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern08_0060(PLW* wk) {
    active_pattern_(wk);
}

void Pattern08_0061(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 1, 0xA, -1});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x3B, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0062(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0x39, 0x3A, 0xFFFF, 0});
}

void Pattern08_0063(PLW* wk) {
    active_pattern_(wk);
}

void Pattern08_0064(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Pattern08_0065(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){9, 0x1F, 8, -1});
}

void Pattern08_0066(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){9, 0x1F, 9, -1});
}

void Pattern08_0067(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){9, 0x1F, 0xA, -1});
}

void Pattern08_0068(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){9, 0x1F, 0xA, 0x700});
}

void Pattern08_0069(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, 0);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){9, 0x1F, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern08_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, &(Jump_Term_Args){-0x7F98, -0x7FC0, 9, 0x400, 0, -0x7F90, -1, 0x100});
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){9, 0x1F, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Pattern08_Tbl[71])(PLW*) = {
    Pattern08_0000, Pattern08_0001, Pattern08_0002, Pattern08_0003, Pattern08_0004, Pattern08_0005, Pattern08_0006,
    Pattern08_0007, Pattern08_0008, Pattern08_0009, Pattern08_0010, Pattern08_0011, Pattern08_0012, Pattern08_0013,
    Pattern08_0014, Pattern08_0015, Pattern08_0016, Pattern08_0017, Pattern08_0018, Pattern08_0019, Pattern08_0020,
    Pattern08_0021, Pattern08_0022, Pattern08_0023, Pattern08_0024, Pattern08_0025, Pattern08_0026, Pattern08_0027,
    Pattern08_0028, Pattern08_0029, Pattern08_0030, Pattern08_0031, Pattern08_0032, Pattern08_0033, Pattern08_0034,
    Pattern08_0035, Pattern08_0036, Pattern08_0037, Pattern08_0038, Pattern08_0039, Pattern08_0040, Pattern08_0041,
    Pattern08_0042, Pattern08_0043, Pattern08_0044, Pattern08_0045, Pattern08_0046, Pattern08_0047, Pattern08_0048,
    Pattern08_0049, Pattern08_0050, Pattern08_0051, Pattern08_0052, Pattern08_0053, Pattern08_0054, Pattern08_0055,
    Pattern08_0056, Pattern08_0057, Pattern08_0058, Pattern08_0059, Pattern08_0060, Pattern08_0061, Pattern08_0062,
    Pattern08_0063, Pattern08_0064, Pattern08_0065, Pattern08_0066, Pattern08_0067, Pattern08_0068, Pattern08_0069,
    Pattern08_0070
};
