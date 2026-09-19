/**
 * @file active17.c
 * COM Active: Q
 */

#include "sf33rd/Source/Game/com/active/active17.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern17_Tbl[83])();

void Computer17(PLW* wk) {
    Pattern17_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern17_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern17_0001(PLW* wk) {
    active_pattern_normal_attack_4(wk, 0x10, 0x10);
}

void Pattern17_0002(PLW* wk) {
    active_pattern_approach_walk_command_attack(wk, 0x41, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Pattern17_0003(PLW* wk) {
    active_pattern_adjust_attack_normal_attack(wk, 8, 0x10, 0x200);
}

void Pattern17_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern17_0005(PLW* wk) {
    active_pattern_approach_walk_com_random_select(wk, 0x37, &(Branch_Menu_Args){2, 0x3E, 0x3F, 0x40, 0x40});
}

void Pattern17_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern17_0007(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack(wk, 0x30, 2, 1);
}

void Pattern17_0008(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Pattern17_0009(PLW* wk) {
    active_pattern_search_back_term_jump_look(wk, 0x60, 6, 0x10);
}

void Pattern17_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern17_0011(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Pattern17_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x40);
}

void Pattern17_0013(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern17_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x20, 2, -0x7FA0, -1, 0x20});
}

void Pattern17_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern17_0016(PLW* wk) {
    active_pattern_walk(wk, 1, 0x30, 0);
}

void Pattern17_0017(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern17_0018(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 8, 0x402, 0});
}

void Pattern17_0019(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x400);
}

void Pattern17_0020(PLW* wk) {
    active_pattern_command_attack_2(
        wk,
        &(Command_Attack_Args){8, 0x1C, 9, -1},
        &(Command_Attack_Args){8, 0x1E, 0xA, -1}
    );
}

void Pattern17_0021(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FA0, 0xB, 0x200, 0, -0x7FA0, -1, 0x200});
}

void Pattern17_0022(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xF, 0x40, 0});
}

void Pattern17_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Pattern17_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern17_0025(PLW* wk) {
    active_pattern_walk(wk, 1, 0x60, 0);
}

void Pattern17_0026(PLW* wk) {
    active_pattern_command_attack_look(wk);
}

void Pattern17_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern17_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0xBF);
}

void Pattern17_0029(PLW* wk) {
    active_pattern_adjust_attack_2(wk, 0x100, 0x200, 0x400);
}

void Pattern17_0030(PLW* wk) {
    active_pattern_adjust_attack_3(wk, 0x402);
}

void Pattern17_0031(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-1, 0x40, 0xB, 0x40, 0, -0x7FA0, -1, 0x200},
        0x200
    );
}

void Pattern17_0032(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 0xB, 0x10, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern17_0033(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 8, 0x10, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Pattern17_0034(PLW* wk) {
    active_pattern_adjust_attack_2(wk, 0x102, 0x102, 0x200);
}

void Pattern17_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern17_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern17_0037(PLW* wk) {
    active_pattern_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1}, 8, 0x400);
}

void Pattern17_0038(PLW* wk) {
    active_pattern_adjust_attack_normal_attack_lever_attack(wk);
}

void Pattern17_0039(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    case 1:
        Wait(wk, 6);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern17_0040(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 9, 0x202, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Pattern17_0041(PLW* wk) {
    active_pattern_normal_attack_4(wk, 0x22, 0x42);
}

void Pattern17_0042(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x12, 0x12, 0x402);
}

void Pattern17_0043(PLW* wk) {
    active_pattern_search_back_term_walk_wait(wk);
}

void Pattern17_0044(PLW* wk) {
    active_pattern_search_back_term_walk_wait_2(wk);
}

void Pattern17_0045(PLW* wk) {
    active_pattern_walk_search_back_term_walk(wk);
}

void Pattern17_0046(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8014, 0xA, -1});
}

void Pattern17_0047(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 0xA, -1});
}

void Pattern17_0048(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern17_0049(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 8, -1});
}

void Pattern17_0050(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 9, -1});
}

void Pattern17_0051(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern17_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 5, 2, 0});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern17_0053(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 5, 2, 0});
        break;

    case 1:
        SA_Term(wk, &(SA_Term_Args){0x2e, -1, -1, 0});
        break;

    case 2:
        Approach_Walk(wk, 0x7f, 2);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){-1, 0x2f, -1, 0});
        break;

    case 4:
        Approach_Walk(wk, 0x41, 2);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1f, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern17_0054(PLW* wk) {
    active_pattern_sa_term_command_attack(
        wk,
        &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0},
        &(Command_Attack_Args){8, 0x1C, 0xA, -1}
    );
}

void Pattern17_0055(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern17_0056(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern17_0057(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, -1});
}

void Pattern17_0058(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 0xA, -1});
}

void Pattern17_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1C, 9, 0x700});
}

void Pattern17_0060(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, 0x70});
}

void Pattern17_0061(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1D, 0xA, 0x700});
}

void Pattern17_0062(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x110);
}

void Pattern17_0063(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 1, 0x110);
}

void Pattern17_0064(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x110);
}

void Pattern17_0065(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x40);
}

void Pattern17_0066(PLW* wk) {
    active_pattern_approach_walk_command_attack(wk, 0x4B, &(Command_Attack_Args){8, 0x21, 0xA, -1});
}

void Pattern17_0067(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x20, 0xA, -1});
}

void Pattern17_0068(PLW* wk) {
    pattern_pierce_on_command_attack_wait_3(wk, &(Command_Attack_Args){9, 0x1F, 8, -1});
}

void Pattern17_0069(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){9, 0x1F, 8, -1});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern17_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){9, 0x1F, 8, -1});
        break;

    case 2:
        Walk(wk, 0, 8, 0);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern17_0071(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){9, 0x1F, 0xA, -1});
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern17_0072(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){9, 0x1F, 0xA, -1});
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Provoke(wk, 1);
        break;

    case 3:
        Com_Random_Select(wk, &(Branch_Menu_Args){6, 0x85, 0x86, 0x87, 0x70}, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern17_0073(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA(wk, 2, 0x44);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){9, 0x1f, 8, -1});
        break;

    case 3:
        Walk(wk, 0, 8, 0);
        break;

    case 4:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){-1, 0x2f, -1, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern17_0074(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){9, 0x1F, 8, -1});
        break;

    case 2:
        Walk(wk, 0, 8, 0);
        break;

    case 3:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1});
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern17_0075(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-1, 0x40, 8, 0x40, 0, -0x7FA0, -1, 0x200});
        break;

    case 2:
        Lever_On(wk, 1, 0xffff);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1c, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern17_0076(PLW* wk) {
    active_pattern_lever_on_normal_attack_command_attack(wk, 0x12, 0x12, &(Command_Attack_Args){8, 0x1C, 0x4008, -1});
}

void Pattern17_0077(PLW* wk) {
    active_pattern_lever_on_normal_attack_command_attack(wk, 0x102, 0x202, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Pattern17_0078(PLW* wk) {
    pattern_pierce_on_lever_on_lever_attack(wk);
}

void Pattern17_0079(PLW* wk) {
    pattern_pierce_on_hi_jump_attack_term_lever_on_2(
        wk,
        &(Hi_Jump_Term_Args){-1, 0x40, 8, 0x400, 0, -0x7FA0, -1, 0x200}
    );
}

void Pattern17_0080(PLW* wk) {
    active_pattern_lever_on_normal_attack_command_attack(wk, 0x12, 0x12, &(Command_Attack_Args){8, 0x1C, 0x4008, -1});
}

void Pattern17_0081(PLW* wk) {
    active_pattern_lever_on_normal_attack_command_attack(wk, 0x102, 0x202, &(Command_Attack_Args){8, 0x1D, 9, -1});
}

void Pattern17_0082(PLW* wk)

{
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;
    case 1:
        Hi_Jump_Attack_Term(wk, &(Hi_Jump_Term_Args){-1, 0x40, 8, 0x400, 0, -0x7FA0, -1, 0x200});
        break;
    case 2:
        Lever_On(wk, 1, 0xffff);
        break;
    case 3:
        Normal_Attack(wk, 8, 0x12);
        break;
    case 4:
        Normal_Attack(wk, 8, 0x22);
        break;
    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1c, 8, -1});
        break;
    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Pattern17_Tbl[83])(PLW*) = {
    Pattern17_0000, Pattern17_0001, Pattern17_0002, Pattern17_0003, Pattern17_0004, Pattern17_0005, Pattern17_0006,
    Pattern17_0007, Pattern17_0008, Pattern17_0009, Pattern17_0010, Pattern17_0011, Pattern17_0012, Pattern17_0013,
    Pattern17_0014, Pattern17_0015, Pattern17_0016, Pattern17_0017, Pattern17_0018, Pattern17_0019, Pattern17_0020,
    Pattern17_0021, Pattern17_0022, Pattern17_0023, Pattern17_0024, Pattern17_0025, Pattern17_0026, Pattern17_0027,
    Pattern17_0028, Pattern17_0029, Pattern17_0030, Pattern17_0031, Pattern17_0032, Pattern17_0033, Pattern17_0034,
    Pattern17_0035, Pattern17_0036, Pattern17_0037, Pattern17_0038, Pattern17_0039, Pattern17_0040, Pattern17_0041,
    Pattern17_0042, Pattern17_0043, Pattern17_0044, Pattern17_0045, Pattern17_0046, Pattern17_0047, Pattern17_0048,
    Pattern17_0049, Pattern17_0050, Pattern17_0051, Pattern17_0052, Pattern17_0053, Pattern17_0054, Pattern17_0055,
    Pattern17_0056, Pattern17_0057, Pattern17_0058, Pattern17_0059, Pattern17_0060, Pattern17_0061, Pattern17_0062,
    Pattern17_0063, Pattern17_0064, Pattern17_0065, Pattern17_0066, Pattern17_0067, Pattern17_0068, Pattern17_0069,
    Pattern17_0070, Pattern17_0071, Pattern17_0072, Pattern17_0073, Pattern17_0074, Pattern17_0075, Pattern17_0076,
    Pattern17_0077, Pattern17_0078, Pattern17_0079, Pattern17_0080, Pattern17_0081, Pattern17_0082
};
