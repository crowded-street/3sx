/**
 * @file pass14_2.c
 * COM Passive: Akuma/Gouki
 * Pattern scripts from Passive14_0053 on.
 */

#include "sf33rd/Source/Game/com/passive/pass14.h"
#include "sf33rd/Source/Game/com/passive/pass14_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive14_0053(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive14_0054(PLW* wk) {
    pattern_wait_get_up(wk, 0, -1);
}

void Passive14_0055(PLW* wk) {
    pattern_wait_get_up(wk, 3, -1);
}

void Passive14_0056(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 0, 0x402);
}

void Passive14_0057(PLW* wk) {
    pattern_wait_get_up_lever_attack(wk, 0, 0, 0x20);
}

void Passive14_0058(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 3, 0x402);
}

void Passive14_0059(PLW* wk) {
    pattern_wait_get_up_lever_attack(wk, 3, 0, 0x20);
}

static void passive14_pattern_keep_away_wait_get_up(PLW* wk, s16 target_pos, u16 lever_data) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, target_pos, 0);
        break;

    case 1:
        Wait_Get_Up(wk, lever_data, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0060(PLW* wk) {
    passive14_pattern_keep_away_wait_get_up(wk, 0xBF, 0);
}

void Passive14_0061(PLW* wk) {
    passive14_pattern_keep_away_wait_get_up(wk, 0xBF, 3);
}

void Passive14_0062(PLW* wk) {
    pattern_keep_away_wait_get_up_command_attack(wk, 0xBF, -1, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive14_0063(PLW* wk) {
    pattern_etc_term_provoke_next_another_menu(wk, 0x37, 0x39);
}

void Passive14_0064(PLW* wk) {
    passive14_pattern_keep_away_wait_get_up(wk, 0x7F, 0);
}

void Passive14_0065(PLW* wk) {
    passive14_pattern_keep_away_wait_get_up(wk, 0x7F, 3);
}

void Passive14_0066(PLW* wk) {
    pattern_keep_away_wait_get_up_command_attack(wk, 0x7F, -1, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive14_0067(PLW* wk) {
    pattern_wait_get_up_normal_attack(wk, 3, 0x202);
}

void Passive14_0068(PLW* wk) {
    pattern_wait_get_up_search_back_term_jump_attack_term(
        wk,
        0x45,
        &(Jump_Term_Args){-0x7FA8, -0x7FD0, 0xB, 0x200, 0, -0x7F80, -1, 0x400},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

static void passive14_pattern_wait_get_up_branch_wait_area_command_attack(
    PLW* wk, const Branch_Wait_Args* p, const Command_Attack_Args* p_b
) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Branch_Wait_Area(wk, p);
        break;

    case 2:
        Command_Attack(wk, p_b);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0069(PLW* wk) {
    passive14_pattern_wait_get_up_branch_wait_area_command_attack(
        wk,
        &(Branch_Wait_Args){0x14, 0xF, 5, 1},
        &(Command_Attack_Args){8, 0x1F, 9, -1}
    );
}

void Passive14_0070(PLW* wk) {
    passive14_pattern_wait_get_up_branch_wait_area_command_attack(
        wk,
        &(Branch_Wait_Args){0x14, 0xF, 5, 1},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive14_0071(PLW* wk) {
    pattern_approach_walk_wait_get_up_normal_attack(wk, 0x7F, 0x402);
}

void Passive14_0072(PLW* wk) {
    pattern_approach_walk_wait_get_up_lever_attack(wk, 0x7F, 0, 0x20);
}

void Passive14_0073(PLW* wk) {
    passive14_pattern_wait_get_up_branch_wait_area_command_attack(
        wk,
        &(Branch_Wait_Args){10, 7, 3, 1},
        &(Command_Attack_Args){8, 0x21, 8, -1}
    );
}

void Passive14_0074(PLW* wk) {
    passive14_pattern_wait_get_up_branch_wait_area_command_attack(
        wk,
        &(Branch_Wait_Args){10, 7, 3, 1},
        &(Command_Attack_Args){8, 0x21, 9, -1}
    );
}

void Passive14_0075(PLW* wk) {
    passive14_pattern_wait_get_up_branch_wait_area_command_attack(
        wk,
        &(Branch_Wait_Args){10, 7, 3, 1},
        &(Command_Attack_Args){8, 0x21, 10, -1}
    );
}

void Passive14_0076(PLW* wk) {
    pattern_wait_get_up_branch_wait_area_j_command_attack(
        wk,
        &(Branch_Wait_Args){0xF, 10, 5, 1},
        &(Command_Attack_Args){8, 0x20, 10, -1}
    );
}

void Passive14_0077(PLW* wk) {
    pattern_wait_get_up_search_back_term_jump_attack_term(
        wk,
        0x45,
        &(Jump_Term_Args){-0x7FA8, -0x7FE0, 0xB, 0x200, 0, -0x7F80, -1, 0x400},
        &(Command_Attack_Args){8, 0x20, 10, -1}
    );
}

void Passive14_0078(PLW* wk) {
    pattern_etc_term_provoke_next_another_menu(wk, 0x37, 0x46);
}

void Passive14_0079(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
}

void Passive14_0080(PLW* wk) {
    active_pattern_command_attack_2(wk, &(Command_Attack_Args){8, 0, -1, -1}, &(Command_Attack_Args){8, 0, -1, -1});
}

void Passive14_0081(PLW* wk) {
    pattern_search_back_term_command_attack_sa_term(wk, 0x300, 0xE3, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive14_0082(PLW* wk) {
    pattern_search_back_term_command_attack_sa_term(wk, 0x80, 0xE3, &(Command_Attack_Args){8, 0x1D, 10, -1});
}

void Passive14_0083(PLW* wk) {
    pattern_search_back_term_command_attack_sa_term(wk, 0x200, 0xE4, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive14_0084(PLW* wk) {
    pattern_search_back_term_command_attack_sa_term(wk, 0x100, 0xE4, &(Command_Attack_Args){8, 0x1D, 8, -1});
}

void Passive14_0085(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 1:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1F, 10, -1});
        break;

    case 2:
        Wait(wk, 1);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
