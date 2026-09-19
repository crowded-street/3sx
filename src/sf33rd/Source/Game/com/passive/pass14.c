/**
 * @file pass14.c
 * COM Passive: Akuma/Gouki
 */

#include "sf33rd/Source/Game/com/passive/pass14.h"
#include "sf33rd/Source/Game/com/passive/pass14_internal.h"
#include "sf33rd/Source/Game/com/passive/pass_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive14_Tbl[253])();

void Passive14(PLW* wk) {
    Passive14_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive14_0000(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xD, M_Lv[wk->wu.id]);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0001(PLW* wk) {
    pattern_lever_off_look(wk);
}

void Passive14_0002(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x47, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 0x10);
}

void Passive14_0003(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x47, &(EM_Term_Params){-1, -0x7FDC, 0, 1, -1}, 0x40);
}

void Passive14_0004(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x47, &(EM_Term_Params){-1, -0x7FE0, 0, 1, -1}, 0x400);
}

void Passive14_0005(PLW* wk) {
    pattern_approach_walk_em_term_lever_attack(wk, 0x10, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 0);
}

void Passive14_0006(PLW* wk) {
    pattern_approach_walk_em_term_lever_attack(wk, 0x10, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 1);
}

void Passive14_0007(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 2:
        Normal_Attack(wk, 0xD, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0008(PLW* wk) {
    pattern_approach_walk_j_command_attack(wk, 0x47, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive14_0009(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_3(
        wk,
        0x47,
        &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive14_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0011(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        Pierce_On(wk);
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 8, -1});
        break;

    case 5:
        Wait(wk, 3);
        break;

    default:
        pattern_j_command_attack_from_step_6(wk);
        break;
    }
}

void Passive14_0012(PLW* wk) {
    pattern_approach_walk_em_term_j_command_attack(
        wk,
        0x47,
        &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C},
        &(Command_Attack_Args){8, 0x20, 10, -1}
    );
}

void Passive14_0013(PLW* wk) {
    pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7F00, 6, 6, 0x7C},
        &(Command_Attack_Args){8, 0x21, 10, -1}
    );
}

void Passive14_0014(PLW* wk) {
    pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC8, 6, 6, 0x7C},
        &(Command_Attack_Args){8, 0x1F, 8, -1}
    );
}

void Passive14_0015(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x7F, &(EM_Term_Params){-1, -0x7FD0, 6, 6, 0x7C}, 0x402);
}

static void passive14_0016_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 10, -1});
        break;

    case 7:
        Wait(wk, 3);
        break;

    case 8:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x31, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0016(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){9, 0x20, 8, -1});
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        Wait(wk, 3);
        break;

    default:
        passive14_0016_from_step_6(wk);
        break;
    }
}

void Passive14_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 10, -1});
        break;

    case 4:
        Wait(wk, 4);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0019(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x42);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0020(PLW* wk) {
    pattern_approach_walk_em_term_normal_attack_2(wk, 0x7F, &(EM_Term_Params){-1, -0x7FF0, 0, 1, -1}, 0x40);
}

void Passive14_0021(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7F80, 6, 6, 0x7C});
        break;

    case 2:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0022(PLW* wk) {
    pattern_em_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 6, 0x7C},
        &(Command_Attack_Args){8, 0x20, 10, -1}
    );
}

void Passive14_0023(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x2F, 10, -1, -1, 0x30, 0, -1, -1, 0xFFFF});
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0024(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 3:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 3:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0026(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1});
        break;

    case 3:
        Lever_Attack(wk, 8, 1, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0027(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 3:
        Normal_Attack(wk, 0xD, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0028(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 10, -1});
        break;

    case 5:
        Wait(wk, 4);
        break;

    default:
        pattern_sa_term_from_step_6(wk);
        break;
    }
}

void Passive14_0029(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1F, 10, -1});
        break;

    case 5:
        Wait(wk, 1);
        break;

    default:
        pattern_sa_term_from_step_6(wk);
        break;
    }
}

void Passive14_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        J_Command_Attack(wk, &(Command_Attack_Args){0xB, 0x20, 8, -1});
        break;

    default:
        pattern_wait_j_command_attack_from_step_6(wk);
        break;
    }
}

void Passive14_0031(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 6, 0x7C});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;

    case 5:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive14_0035_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Wait(wk, 3);
        break;

    case 7:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 10, -1});
        break;

    case 8:
        Wait(wk, 3);
        break;

    case 9:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x31, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0035(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 6, 0x7C});
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        J_Command_Attack(wk, &(Command_Attack_Args){0xB, 0x20, 8, -1});
        break;

    case 5:
        Pierce_On(wk);
        break;

    default:
        passive14_0035_from_step_6(wk);
        break;
    }
}

void Passive14_0036(PLW* wk) {
    pattern_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0xFFFF, 0x30, 0x31, 0x7F},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void passive14_pattern_em_term_sa_term_j_command_attack(PLW* wk, const EM_Term_Params* p, const SA_Term_Args* p_b) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, p);
        break;

    case 1:
        SA_Term(wk, p_b);
        break;

    case 2:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0037(PLW* wk) {
    passive14_pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FA0, -0x7FA0, 6, 6, 0x35},
        &(SA_Term_Args){0xFFFF, 0x30, 0x31, 0x7F}
    );
}

void Passive14_0038(PLW* wk) {
    pattern_sa_term_j_command_attack(
        wk,
        &(SA_Term_Args){0xFFFF, 0x30, 0x31, 0x7F},
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive14_0039(PLW* wk) {
    passive14_pattern_em_term_sa_term_j_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FB0, 6, 6, 0x35},
        &(SA_Term_Args){0xFFFF, 0x30, 0x31, 0x7F}
    );
}

void Passive14_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 3);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-0x7FC0, -0x7FC8, 6, 6, 0x35});
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, &(Command_Attack_Args){8, 0, -1, -1});
        break;

    case 1:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 2:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 6, 0x35});
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0042(PLW* wk) {
    pattern_command_attack(
        wk,
        &(Command_Attack_Args){8, 1, -1, -1},
        &(Command_Attack_Args){8, 0x1F, 10, -1}
    );
}

void Passive14_0043(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, 0x38, 8, 0x200, 2, -0x7FA0, -1, 0x200});
}

void Passive14_0044(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, 0x38, 8, 0x400, 2, -0x7FA0, -1, 0x400});
}

void Passive14_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, &(JCA_Term_Args){8, 0x2F, 10, -1, -1, 0x10, 2, -1, -1, 0xFFFF});
        break;

    case 2:
        SA_Term(wk, &(SA_Term_Args){0xFFFF, 0xFFFF, 0x31, 0x7F});
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){8, 0x1E, 10, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0046(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1F, 10, -1});
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0047(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1});
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        J_Command_Attack(wk, &(Command_Attack_Args){0xC, 0x1E, 10, -1});
        break;

    case 4:
        Wait(wk, 3);
        break;

    case 5:
        SA_Term(wk, &(SA_Term_Args){0x2F, 0x30, 0x31, 0x7F});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0048(PLW* wk) {
    pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F80, 0x20, 8, 0x400, 1, -0x7F80, -1, 0x400});
}

void Passive14_0049(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-0x7FB0, -0x7FB0, 6, 6, -1}, 8, 0x400);
}

void Passive14_0050(PLW* wk) {
    pattern_em_term_normal_attack_2(wk, &(EM_Term_Params){-0x7FC0, -0x7FF0, 6, 1, -1}, 8, 0x100);
}

void Passive14_0051(PLW* wk) {
    pattern_em_term_normal_attack_j_command_attack(
        wk,
        &(EM_Term_Params){-0x7FB0, -0x7FC0, 0, 1, -1},
        0x102,
        &(Command_Attack_Args){8, 0x1E, 8, -1}
    );
}

void Passive14_0052(PLW* wk) {
    pattern_approach_walk_normal_attack_2(wk, 0xB, 0x402);
}



void (*const Passive14_Tbl[253])(PLW*) = {
    Passive14_0000, Passive14_0001, Passive14_0002, Passive14_0003, Passive14_0004, Passive14_0005, Passive14_0006,
    Passive14_0007, Passive14_0008, Passive14_0009, Passive14_0010, Passive14_0011, Passive14_0012, Passive14_0013,
    Passive14_0014, Passive14_0015, Passive14_0016, Passive14_0017, Passive14_0018, Passive14_0019, Passive14_0020,
    Passive14_0021, Passive14_0022, Passive14_0023, Passive14_0024, Passive14_0025, Passive14_0026, Passive14_0027,
    Passive14_0028, Passive14_0029, Passive14_0030, Passive14_0031, Passive14_0032, Passive14_0033, Passive14_0034,
    Passive14_0035, Passive14_0036, Passive14_0037, Passive14_0038, Passive14_0039, Passive14_0040, Passive14_0041,
    Passive14_0042, Passive14_0043, Passive14_0044, Passive14_0045, Passive14_0046, Passive14_0047, Passive14_0048,
    Passive14_0049, Passive14_0050, Passive14_0051, Passive14_0052, Passive14_0053, Passive14_0054, Passive14_0055,
    Passive14_0056, Passive14_0057, Passive14_0058, Passive14_0059, Passive14_0060, Passive14_0061, Passive14_0062,
    Passive14_0063, Passive14_0064, Passive14_0065, Passive14_0066, Passive14_0067, Passive14_0068, Passive14_0069,
    Passive14_0070, Passive14_0071, Passive14_0072, Passive14_0073, Passive14_0074, Passive14_0075, Passive14_0076,
    Passive14_0077, Passive14_0078, Passive14_0079, Passive14_0080, Passive14_0081, Passive14_0082, Passive14_0083,
    Passive14_0084, Passive14_0085, Passive14_0086, Passive14_0087, Passive14_0088, Passive14_0089, Passive14_0090,
    Passive14_0091, Passive14_0092, Passive14_0093, Passive14_0094, Passive14_0095, Passive14_0096, Passive14_0097,
    Passive14_0098, Passive14_0099, Passive14_0100, Passive14_0101, Passive14_0102, Passive14_0103, Passive14_0104,
    Passive14_0105, Passive14_0106, Passive14_0107, Passive14_0108, Passive14_0109, Passive14_0110, Passive14_0111,
    Passive14_0112, Passive14_0113, Passive14_0114, Passive14_0115, Passive14_0116, Passive14_0117, Passive14_0118,
    Passive14_0119, Passive14_0120, Passive14_0121, Passive14_0122, Passive14_0123, Passive14_0124, Passive14_0125,
    Passive14_0126, Passive14_0127, Passive14_0128, Passive14_0129, Passive14_0130, Passive14_0131, Passive14_0132,
    Passive14_0133, Passive14_0134, Passive14_0135, Passive14_0136, Passive14_0137, Passive14_0138, Passive14_0139,
    Passive14_0140, Passive14_0141, Passive14_0142, Passive14_0143, Passive14_0144, Passive14_0145, Passive14_0146,
    Passive14_0147, Passive14_0148, Passive14_0149, Passive14_0150, Passive14_0151, Passive14_0152, Passive14_0153,
    Passive14_0154, Passive14_0155, Passive14_0156, Passive14_0157, Passive14_0158, Passive14_0159, Passive14_0160,
    Passive14_0161, Passive14_0162, Passive14_0163, Passive14_0164, Passive14_0165, Passive14_0166, Passive14_0167,
    Passive14_0168, Passive14_0169, Passive14_0170, Passive14_0171, Passive14_0172, Passive14_0173, Passive14_0174,
    Passive14_0175, Passive14_0176, Passive14_0177, Passive14_0178, Passive14_0179, Passive14_0180, Passive14_0181,
    Passive14_0182, Passive14_0183, Passive14_0184, Passive14_0185, Passive14_0186, Passive14_0187, Passive14_0188,
    Passive14_0189, Passive14_0190, Passive14_0191, Passive14_0192, Passive14_0193, Passive14_0194, Passive14_0195,
    Passive14_0196, Passive14_0197, Passive14_0198, Passive14_0199, Passive14_0200, Passive14_0201, Passive14_0202,
    Passive14_0203, Passive14_0204, Passive14_0205, Passive14_0206, Passive14_0207, Passive14_0208, Passive14_0209,
    Passive14_0210, Passive14_0211, Passive14_0212, Passive14_0213, Passive14_0214, Passive14_0215, Passive14_0216,
    Passive14_0217, Passive14_0218, Passive14_0219, Passive14_0220, Passive14_0221, Passive14_0222, Passive14_0223,
    Passive14_0224, Passive14_0225, Passive14_0226, Passive14_0227, Passive14_0228, Passive14_0229, Passive14_0230,
    Passive14_0231, Passive14_0232, Passive14_0233, Passive14_0234, Passive14_0235, Passive14_0236, Passive14_0237,
    Passive14_0238, Passive14_0239, Passive14_0240, Passive14_0241, Passive14_0242, Passive14_0243, Passive14_0244,
    Passive14_0245, Passive14_0246, Passive14_0247, Passive14_0248, Passive14_0249, Passive14_0250, Passive14_0251,
    Passive14_0252
};
