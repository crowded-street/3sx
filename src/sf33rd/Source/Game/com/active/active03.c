/**
 * @file active03.c
 * COM Active: Yun
 */

#include "sf33rd/Source/Game/com/active/active03.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/workuser.h"

void (*const Pattern03_Tbl[72])();

void Computer03(PLW* wk) {
    Pattern03_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern03_0000(PLW* wk) {
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

void Pattern03_0001(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0002(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Branch_Unit_Area(wk, 2, 0x31, 0x32, 0x33, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0003(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 9, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0004(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0005(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0006(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 0x1E);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0007(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 1);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, 8, 1, 0xB, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0008(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0009(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 0x10);
        break;

    case 1:
        Jump(wk, 1);
        break;

    case 2:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0011(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0012(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0014(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FD0, 8, 0x20, 2, -0x7fa0, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0015(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 0, 0x30, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0016(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 0);
        break;

    case 1:
        Walk(wk, 1, 0x30, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack(wk, 8, 0xC, 0x8400, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0019(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x20, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0020(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    case 1:
        Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0021(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FD0, 8, 0x40, 0, -0x7fa0, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0022(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, -0x7FB0, -1, 8, 0x8100, 0, -0x7fa0, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0023(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x83, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0024(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, -0x7FA0, -0x7FD0, 8, 0x20, 2, -0x7FA0, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 0);
        break;

    case 1:
        Walk(wk, 1, 0x60, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0026(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, 0xB, -1);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0027(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0028(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xC3, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0029(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 9, 0x100);
        break;

    case 1:
        Adjust_Attack(wk, 0xC, 0x100);
        break;

    case 2:
        Adjust_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 9, 0x10);
        break;

    case 1:
        Adjust_Attack(wk, 0xC, 0x20);
        break;

    case 2:
        Adjust_Attack(wk, 8, 0x40);
        break;

    case 3:
        Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0031(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x20);
        break;

    case 1:
        Branch_Unit_Area(wk, 2, 0x31, 0x32, 0x33, 0x33);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xB, 0x10);
        break;

    case 1:
        Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 9, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 9, 0x102);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0035(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0036(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x42);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0037(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 1:
        Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Adjust_Attack(wk, 0xB, 0x20);
        break;

    case 1:
        Normal_Attack(wk, 0xA, 0x202);
        break;

    case 2:
        Branch_Unit_Area(wk, 2, 0x31, 0x32, 0x33, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0039(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    case 1:
        Branch_Unit_Area(wk, 2, 0x31, 0x32, 0x33, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 1:
        Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x42);
        break;

    case 1:
        Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0042(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0043(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, 8, 8, 0x8400, 0, -1, -0x7FA0, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0044(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 8, 0x20, 0, -0x7FA0, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 0, 0x14, 0);
        break;

    case 1:
        Walk(wk, 1, 0x16, 0);
        break;

    case 2:
        Walk(wk, 0, 0x18, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0046(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x8016, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0047(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x8015, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0048(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x8014, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0049(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1F, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0050(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1F, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0051(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0x2E, 0x2F, 0xFFFF, 0);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0053(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0xFFFF, 0x2F, 0x30, 0);
        break;

    case 1:
        Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0xFFFF, 0xFFFF, 0x30, 0);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, -0x7FB0, 8, 8, 0x8400, 0, -0x7FA0, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0055(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, -0x7FA0, -0x7FD0, 8, 0x40, 0, -0x7FA0, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0056(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x31, 4, 5, 1, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0057(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 6, 0x12);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 8, 0x20, 1, -0x7FA0, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0058(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 0x11);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 8, 0x20, 1, -0x7FA0, 8, 0x200);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        Command_Attack(wk, 8, 0, 0xB, -1);
        break;

    case 4:
        Com_Random_Select(wk, 2, 0x18, 0x18, 0x11, 0x11, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0059(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 0x11);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, 8, 1, 0xB, -1);
        break;

    case 3:
        Command_Attack(wk, 8, 0, 0xB, -1);
        break;

    case 4:
        Com_Random_Select(wk, 2, 0x18, 0x18, 0x11, 0x11, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0060(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 0);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 8, 0x200, 0, -1, -1, -1);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        Command_Attack(wk, 8, 0, 0xB, -1);
        break;

    case 4:
        Com_Random_Select(wk, 2, 0x18, 0x18, 0x11, 0x11, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0061(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 2, 0);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Command_Attack(wk, 8, 1, 0xB, -1);
        break;

    case 3:
        Hi_Jump_Attack_Term(wk, -0x7FA0, -1, 8, 0x8400, 0, -0x7FA0, -1, 0x8400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0062(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Hi_Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 8, 0x20, 0, -0x7FA0, 8, 0x200);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x18, 0x18, 0x11, 0x11, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0064(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x10);
        break;
    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;
    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;
    case 3:
        Com_Random_Select(wk, 6, 0x37, 0x37, 0x27, 0x27, 0);
        break;
    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x200);
        break;

    case 1:
        Command_Attack(wk, 8, 0x20, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x2D, 0x19, 0x10, 0x17, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0067(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Branch_Unit_Area(wk, 6, 0x44, 0x45, 0x46, 0x47);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0068(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0069(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1D, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1D, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern03_0071(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1D, 0xB, 0x70);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Pattern03_Tbl[72])(PLW*) = {
    Pattern03_0000, Pattern03_0001, Pattern03_0002, Pattern03_0003, Pattern03_0004, Pattern03_0005, Pattern03_0006,
    Pattern03_0007, Pattern03_0008, Pattern03_0009, Pattern03_0010, Pattern03_0011, Pattern03_0012, Pattern03_0013,
    Pattern03_0014, Pattern03_0015, Pattern03_0016, Pattern03_0017, Pattern03_0018, Pattern03_0019, Pattern03_0020,
    Pattern03_0021, Pattern03_0022, Pattern03_0023, Pattern03_0024, Pattern03_0025, Pattern03_0026, Pattern03_0027,
    Pattern03_0028, Pattern03_0029, Pattern03_0030, Pattern03_0031, Pattern03_0032, Pattern03_0033, Pattern03_0034,
    Pattern03_0035, Pattern03_0036, Pattern03_0037, Pattern03_0038, Pattern03_0039, Pattern03_0040, Pattern03_0041,
    Pattern03_0042, Pattern03_0043, Pattern03_0044, Pattern03_0045, Pattern03_0046, Pattern03_0047, Pattern03_0048,
    Pattern03_0049, Pattern03_0050, Pattern03_0051, Pattern03_0052, Pattern03_0053, Pattern03_0054, Pattern03_0055,
    Pattern03_0056, Pattern03_0057, Pattern03_0058, Pattern03_0059, Pattern03_0060, Pattern03_0061, Pattern03_0062,
    Pattern03_0063, Pattern03_0064, Pattern03_0065, Pattern03_0066, Pattern03_0067, Pattern03_0068, Pattern03_0069,
    Pattern03_0070, Pattern03_0071
};
