/**
 * @file active06.c
 * COM Active: Hugo
 */

#include "sf33rd/Source/Game/com/active/active06.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/workuser.h"

void (*const Pattern06_Tbl[64])();

void Computer06(PLW* wk) {
    Pattern06_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern06_0000(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Off(wk);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0001(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x8014, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0002(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Branch_Unit_Area(wk, 2, 4, 5, 6, 6);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0003(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x8016, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0004(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x8215, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0005(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x8215, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0006(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x8215, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0007(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x44, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FFC, 6, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0008(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x44, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FFC, 6, 1, -1);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0009(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x80, 2);
        break;

    case 1:
        Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x80, 2);
        break;

    case 1:
        Command_Attack(wk, 8, 0x20, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0011(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x80, 2);
        break;

    case 1:
        Command_Attack(wk, 8, 0x20, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0012(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x80, 2);
        break;

    case 1:
        Command_Attack(wk, 8, 0x20, 0xA, 0x700);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0014(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0015(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0016(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x12);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x22);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FB8, 6, 1, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x42);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0019(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x100);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0020(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0021(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0022(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x102);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0023(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0024(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x70, 2);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0026(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x68, 2);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0027(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x60, 2);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0028(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0029(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0031(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1F, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1F, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0035(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x20, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0036(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x20, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0037(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FA8, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7F88, -0x7FC0, 9, 0x400, 0, -0x7F88, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0039(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7F68, -0x7FC0, 9, 0x200, 0, -0x7F68, -1, 0x400);
        break;
    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7F68, -0x7FC0, 9, 0x400, 0, -0x7F68, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x58, 2);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, 0xB, 0x21, 0xA, -1);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0042(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x58, 2);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, 0xB, 0x21, 0xA, -1);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0043(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x60, 2);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1C, 0xA, 0x70);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0044(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x44, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x9F, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0046(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xFF, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0047(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1F, 8, -1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1F, 9, -1);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1F, 0xA, 0x70);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0048(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1F, 8, -1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1F, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0049(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0050(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, 0xC, 0, -1, -1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x20, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0051(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0x7F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xBF, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0053(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x40);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x40);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x40);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0055(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x2F, 0x30, 0x35, 0x36, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0056(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7F90, -0x7FA8, 8, 0x42, 0, -0x7F68, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0057(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7F90, -0x7FA8, 9, 0x42, 0, -0x7F68, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x100);
        break;

    case 3:
        Wait(wk, 0xA);
        break;

    case 4:
        Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0058(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Branch_Unit_Area(wk, 2, 0x3B, 0x3B, 0x3C, 0x3D);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0059(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0060(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1D, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0061(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1D, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0062(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Branch_Unit_Area(wk, 2, 0x21, 0x21, 0x22, 0x23);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern06_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x3A, 0x3A, 0x3A, 0x36, 5);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Pattern06_Tbl[64])(PLW*) = {
    Pattern06_0000, Pattern06_0001, Pattern06_0002, Pattern06_0003, Pattern06_0004, Pattern06_0005, Pattern06_0006,
    Pattern06_0007, Pattern06_0008, Pattern06_0009, Pattern06_0010, Pattern06_0011, Pattern06_0012, Pattern06_0013,
    Pattern06_0014, Pattern06_0015, Pattern06_0016, Pattern06_0017, Pattern06_0018, Pattern06_0019, Pattern06_0020,
    Pattern06_0021, Pattern06_0022, Pattern06_0023, Pattern06_0024, Pattern06_0025, Pattern06_0026, Pattern06_0027,
    Pattern06_0028, Pattern06_0029, Pattern06_0030, Pattern06_0031, Pattern06_0032, Pattern06_0033, Pattern06_0034,
    Pattern06_0035, Pattern06_0036, Pattern06_0037, Pattern06_0038, Pattern06_0039, Pattern06_0040, Pattern06_0041,
    Pattern06_0042, Pattern06_0043, Pattern06_0044, Pattern06_0045, Pattern06_0046, Pattern06_0047, Pattern06_0048,
    Pattern06_0049, Pattern06_0050, Pattern06_0051, Pattern06_0052, Pattern06_0053, Pattern06_0054, Pattern06_0055,
    Pattern06_0056, Pattern06_0057, Pattern06_0058, Pattern06_0059, Pattern06_0060, Pattern06_0061, Pattern06_0062,
    Pattern06_0063
};
