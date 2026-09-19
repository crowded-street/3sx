/**
 * @file active05.c
 * COM Active: Necro
 */

#include "sf33rd/Source/Game/com/active/active05.h"
#include "sf33rd/Source/Game/com/active/active_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern05_Tbl[67])();

void Computer05(PLW* wk) {
    Pattern05_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern05_0000(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Pattern05_0001(PLW* wk) {
    active_pattern_normal_attack_7(wk, 0x102, 0x102, 0x102);
}

void Pattern05_0002(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x41D, 9, -1});
}

void Pattern05_0003(PLW* wk) {
    active_pattern_adjust_attack_4(wk, 8, 0, 0x10);
}

void Pattern05_0004(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){1, 0x1C, 0xA, -1});
}

void Pattern05_0005(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 0, 0x110);
}

void Pattern05_0006(PLW* wk) {
    active_pattern_wait(wk, 0x1E);
}

void Pattern05_0007(PLW* wk) {
    active_pattern_search_back_term_pierce_on_command_attack(wk, 0x30, 2, 0xD);
}

void Pattern05_0008(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 0, 0x200, 0, -1, -1, -1});
}

void Pattern05_0009(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern05_0010(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x402);
}

void Pattern05_0011(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Pattern05_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 0, 0x40);
}

void Pattern05_0013(PLW* wk) {
    active_pattern_normal_attack(wk, 0, 0x200);
}

void Pattern05_0014(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -0x7FC8, 8, 0x200, 2, -0x7F80, -1, 0x200});
}

void Pattern05_0015(PLW* wk) {
    active_pattern_walk(wk, 0, 0x30, 0);
}

void Pattern05_0016(PLW* wk) {
    active_pattern_walk(wk, 1, 0x30, 0);
}

void Pattern05_0017(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
}

void Pattern05_0018(PLW* wk) {
    active_pattern_jump_lever_off_look(wk);
}

void Pattern05_0019(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){0x88, -1, 0, 2, 5}, 1, 0x400);
}

void Pattern05_0020(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern05_0021(PLW* wk) {
    active_pattern_j_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 0xA, -1});
}

void Pattern05_0022(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FA8, 8, 0x40, 0, -0x7F90, -1, 0x100});
}

void Pattern05_0023(PLW* wk) {
    active_pattern_approach_walk(wk, 0x7F);
}

void Pattern05_0024(PLW* wk) {
    active_pattern_walk(wk, 0, 0x60, 0);
}

void Pattern05_0025(PLW* wk) {
    active_pattern_walk(wk, 1, 0x60, 0);
}

void Pattern05_0026(PLW* wk) {
    active_pattern_command_attack_lever_off_look(wk);
}

void Pattern05_0027(PLW* wk) {
    active_pattern_jump(wk);
}

void Pattern05_0028(PLW* wk) {
    active_pattern_approach_walk(wk, 0xBF);
}

void Pattern05_0029(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 9, 0x100, &(Command_Attack_Args){8, 0x41D, 9, -1});
}

void Pattern05_0030(PLW* wk) {
    active_pattern_adjust_attack_command_attack(wk, 9, 0x100, &(Command_Attack_Args){8, 0x1F, 9, -1});
}

void Pattern05_0031(PLW* wk) {
    active_pattern_jump_attack_term_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F50, -0x7FB0, 8, 0x202, 0, -0x7F68, -1, 0x200},
        0x402
    );
}

void Pattern05_0032(PLW* wk) {
    active_pattern_adjust_attack_4(wk, 0xB, 8, 0x20);
}

void Pattern05_0033(PLW* wk) {
    active_pattern_adjust_attack_j_command_attack(wk, 0xB, 0x10, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Pattern05_0034(PLW* wk) {
    active_pattern_normal_attack_lever_off_look(wk, 0);
}

void Pattern05_0035(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x20);
}

void Pattern05_0036(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x42);
}

void Pattern05_0037(PLW* wk) {
    active_pattern_lever_attack(wk, 8, 1, 0x400);
}

void Pattern05_0038(PLW* wk) {
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
        Normal_Attack(wk, 0xA, 0x202);
        break;
    case 4:
        Lever_Attack(wk, 8, 0, 0x400);
        break;
    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern05_0039(PLW* wk) {
    active_pattern_command_attack_j_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Pattern05_0040(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 9, 0x202, &(Command_Attack_Args){8, 0x1C, 8, -1});
}

void Pattern05_0041(PLW* wk) {
    active_pattern_normal_attack_j_command_attack(wk, 0xB, 0x40, &(Command_Attack_Args){8, 0x1E, 0xA, -1});
}

void Pattern05_0042(PLW* wk) {
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

void Pattern05_0043(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FA8, 8, 0x40, 0, -1, 0x30, 0x401F});
}

void Pattern05_0044(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FA8, 8, 0x40, 0, -1, 0x30, 0x400});
}

void Pattern05_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x30, 2, 0xD);
        break;

    case 1:
        Walk(wk, 1, 0x30, 0);
        break;

    case 2:
        Walk(wk, 0, 0x40, 0);
        break;

    case 3:
        Walk(wk, 1, 0x20, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern05_0046(PLW* wk) {
    active_pattern_walk_2(wk, 0x20, 0x20, 0x30);
}

void Pattern05_0047(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(
        wk,
        &(JCA_Term_Args){8, 0x1F, 0xA, -1, -0x7FA0, 0x30, 0, -1, 0x30, 0x400}
    );
}

void Pattern05_0048(PLW* wk) {
    active_pattern_(wk);
}

void Pattern05_0049(PLW* wk) {
    active_pattern_(wk);
}

void Pattern05_0050(PLW* wk) {
    active_pattern_hi_jump_attack(wk);
}

void Pattern05_0051(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-1, 0x30, 8, 0x40, 0, -1, -1, 0xFFFF});
}

void Pattern05_0052(PLW* wk) {
    active_pattern_hi_jump_command_attack_term(wk, &(JCA_Term_Args){8, 0x1F, 0xA, -1, -1, 0x30, 0, -1, -1, -1});
}

void Pattern05_0053(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8414, 0xA, -1});
}

void Pattern05_0054(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 0xA, -1});
}

void Pattern05_0055(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 0xA, -1});
}

void Pattern05_0056(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8414, 9, -1});
}

void Pattern05_0057(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 9, -1});
}

void Pattern05_0058(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 9, -1});
}

void Pattern05_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8414, 8, -1});
}

void Pattern05_0060(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8016, 8, -1});
}

void Pattern05_0061(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x8015, 8, -1});
}

void Pattern05_0062(PLW* wk) {
    active_pattern_sa_term(wk, &(SA_Term_Args){0x35, 0x36, 0x37, 0x60});
}

void Pattern05_0063(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F90, -0x7FC0, 8, 0x200, 0, -0x7F68, -1, 0x400});
}

void Pattern05_0064(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x50, 8, 0x402, 2, -0x7F80, -1, 0x20});
}

void Pattern05_0065(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, 0x50, 8, 0x202, 2, -0x7F80, -1, 0x20});
}

void Pattern05_0066(PLW* wk) {
    active_pattern_provoke(wk);
}

void (*const Pattern05_Tbl[67])(PLW*) = {
    Pattern05_0000, Pattern05_0001, Pattern05_0002, Pattern05_0003, Pattern05_0004, Pattern05_0005, Pattern05_0006,
    Pattern05_0007, Pattern05_0008, Pattern05_0009, Pattern05_0010, Pattern05_0011, Pattern05_0012, Pattern05_0013,
    Pattern05_0014, Pattern05_0015, Pattern05_0016, Pattern05_0017, Pattern05_0018, Pattern05_0019, Pattern05_0020,
    Pattern05_0021, Pattern05_0022, Pattern05_0023, Pattern05_0024, Pattern05_0025, Pattern05_0026, Pattern05_0027,
    Pattern05_0028, Pattern05_0029, Pattern05_0030, Pattern05_0031, Pattern05_0032, Pattern05_0033, Pattern05_0034,
    Pattern05_0035, Pattern05_0036, Pattern05_0037, Pattern05_0038, Pattern05_0039, Pattern05_0040, Pattern05_0041,
    Pattern05_0042, Pattern05_0043, Pattern05_0044, Pattern05_0045, Pattern05_0046, Pattern05_0047, Pattern05_0048,
    Pattern05_0049, Pattern05_0050, Pattern05_0051, Pattern05_0052, Pattern05_0053, Pattern05_0054, Pattern05_0055,
    Pattern05_0056, Pattern05_0057, Pattern05_0058, Pattern05_0059, Pattern05_0060, Pattern05_0061, Pattern05_0062,
    Pattern05_0063, Pattern05_0064, Pattern05_0065, Pattern05_0066
};
