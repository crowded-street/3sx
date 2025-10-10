/**
 * @file shell03.c
 * COM Shell: Yun, Hugo, Elena, Oro, Yang
 */

#include "sf33rd/Source/Game/com/shell/shell03.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/workuser.h"

void (*const Shell03_Tbl[11])(PLW*);

void Shell03(PLW* wk) {
    Shell03_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Shell03_0000(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    default:
        End_Pattern(wk);
        break;
    }
}

void Shell03_0001(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, 1, 2, 1, -1, -1);
        break;

    case 1:
        Jump(wk, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Shell03_0002(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, 0, 2, 1, -1, -1);
        break;

    case 1:
        Jump(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Shell03_0003(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, 0, 2, 1, -1, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FD0, 9, 0x200, 0, -0x7FB0, -1, 0x200);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x20);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x20, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Shell03_0004(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, 0, 2, 1, -1, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FD0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Shell03_0005(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, 0, 2, 1, -1, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x42);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Shell03_0006(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, 1, 2, 1, -1, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, -1, 0x30, 8, 0x400, 2, -1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Shell03_0007(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Shell03_0008(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SHELL_Term(wk, 0, 2, 1, -1, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FD0, 0xB, 0x20, 0, -0x7FB0, -1, 0x200);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Shell03_0009(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Off(wk);
        break;

    case 1:
        SHELL_Term(wk, 2, 2, 1, -1, -1);
        break;

    case 2:
        Next_Be_Flip(wk, 8);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Shell03_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Shell03_Tbl[11])(PLW*) = { Shell03_0000, Shell03_0001, Shell03_0002, Shell03_0003,
                                        Shell03_0004, Shell03_0005, Shell03_0006, Shell03_0007,
                                        Shell03_0008, Shell03_0009, Shell03_0010 };
