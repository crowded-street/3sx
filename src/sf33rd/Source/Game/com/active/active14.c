/**
 * @file active14.c
 * COM Active: Akuma/Gouki
 */

#include "sf33rd/Source/Game/com/active/active14.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Pattern14_Tbl[151])();

void Computer14(PLW* wk) {
    Pattern14_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Pattern14_0000(PLW* wk) {
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

void Pattern14_0001(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0002(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0003(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0004(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0005(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x100);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0006(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0007(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0008(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x12);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0009(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x22);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x42);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0011(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x102);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0012(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0014(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1F, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0015(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1F, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0016(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1F, 8, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0019(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0020(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x20, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0021(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x20, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0022(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 9, 0x20, 8, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0023(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 9, 0x20, 9, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0024(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0x7F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xBF, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0026(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Approach_Walk(wk, 0xbf, 3);
        break;

    case 2:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 8, -1, -1, 0x30, 0, -0x7F80, -1, 0x400);
        break;

    case 3:
        Normal_Attack(wk, 0xb, 0x202);
        break;

    case 4:
        J_Command_Attack(wk, 0xb, 0x20, 8, -1);
        break;

    case 5:
        Wait(wk, 5);
        break;

    case 6:
        SA_Term(wk, 0x2f, 0xffff, 0x31, 0x7f);
        break;

    case 7:
        J_Command_Attack(wk, 8, 0x1e, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0027(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0028(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 8, -1, -1, 0x30, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0029(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 0xB, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xC, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xbf, 3);
        break;

    case 1:
        Jump_Attack_Term(wk, 0x8058, 0x8038, 0xb, 0x400, 0, 0x8080, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xb, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xc, 0x1f, 10, -1);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, 0x2f, 0x30, 0x31, 0x7f);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0031(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 0xB, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xC, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xbf, 2);
        break;

    case 1:
        Jump_Attack_Term(wk, 0x8058, 0x8038, 0xb, 0x400, 0, 0x8080, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xb, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xc, 0x1f, 10, -1);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, 0x2f, 0x30, 0x31, 0x7f);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Approach_Walk(wk, 0xbf, 2);
        break;

    case 2:
        Jump_Command_Attack_Term(wk, 8, 0x2e, 8, -1, -1, 0x30, 0, -0x7F80, -1, 0x400);
        break;

    case 3:
        Normal_Attack(wk, 0xb, 0x202);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 5:
        Wait(wk, 3);
        break;

    case 6:
        J_Command_Attack(wk, 8, 0x1e, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 2:
        Jump_Command_Attack_Term(wk, 0xB, 0x2F, 0xA, -1, -1, 0x30, 0, -1, -1, -1);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0035(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, -1, 0x61, 8, 0x202, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0036(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, -1, 0x61, 8, 0x202, 0, 0x8080, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xb, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xc, 0x1f, 10, -1);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, 0x2f, 0x30, 0x31, 0x7f);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0037(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, -1, 0x61, 8, 0x202, 0, 0x8080, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xb, 0x202);
        break;

    case 3:
        Pierce_On(wk);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 5:
        Wait(wk, 3);
        break;

    case 6:
        J_Command_Attack(wk, 8, 0x1e, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 3, 2, 0);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0039(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 3, 2, 0);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 3, 2, 0);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 3, 2, 0);
        break;

    case 1:
        J_Command_Attack(wk, 0xB, 0x20, 8, -1);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0042(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 3, 2, 0);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1F, 0xA, -1);
        break;

    case 3:
        Wait(wk, 1);
        break;

    case 4:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0043(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 3, 2, 0);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0044(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x71, 0x71, 0x72, 0x73, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x74, 0x74, 0x75, 0x76, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0046(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0047(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x8014, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0048(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x8015, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0049(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x8016, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0050(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Branch_Unit_Area(wk, 2, 0x37, 0x37, 0x36, 0x35);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0051(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x8016, 0xA, -1, -0x7FB0, -0x7FC0, 0, -1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA_Full(wk, 6, 0x7c);
        break;

    case 1:
        EM_Term(wk, 0x80d0, -1, 5, 2, 0);
        break;

    case 2:
        Only_Shot(wk, 0x10);
        break;

    case 3:
        Wait(wk, 1);
        break;

    case 4:
        Only_Shot(wk, 0x10);
        break;

    case 5:
        Wait(wk, 1);
        break;

    case 6:
        Lever_On(wk, 0, 0);
        break;

    case 7:
        Wait(wk, 1);
        break;

    case 8:
        Only_Shot(wk, 0x100);
        break;

    case 9:
        Wait(wk, 1);
        break;

    case 10:
        Only_Shot(wk, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0053(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x8014, 8, -1, -1, 0x20, 0, -1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x8014, 8, -1, -1, 0x20, 2, -1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0055(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x8014, 8, -1, -1, 0x20, 1, -1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0056(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        SA_Term(wk, 0x32, 0xFFFF, 0xFFFF, 0xBF);
        break;

    case 2:
        Com_Random_Select(wk, 6, 0x74, 0x74, 0x75, 0x76, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0057(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0x84, 0x84, 0x85, 0x86, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0058(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 4:
        Command_Attack(wk, 0xC, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0059(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xbf, 2);
        break;

    case 1:
        Jump_Attack_Term(wk, 0x8058, 0x8038, 9, 0x400, 0, 0x8080, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 3:
        Normal_Attack(wk, 0xc, 0x202);
        break;

    case 4:
        Command_Attack(wk, 0xc, 0x1f, 10, -1);
        break;

    case 5:
        Wait(wk, 1);
        break;

    case 6:
        SA_Term(wk, 0x2f, 0x30, 0x31, 0x7f);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0060(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 3:
        SA_Term(wk, 0x32, 0xFFFF, 0xFFFF, 0xBF);
        break;

    case 4:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0061(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xbf, 2);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Hi_Jump_Attack_Term(wk, -1, 0x61, 9, 0x202, 0, 0x8080, -1, 0x400);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 4:
        SA_Term(wk, 0x32, 0xffff, 0xffff, 0xbf);
        break;

    case 5:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0062(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 3:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x7F);
        break;

    case 4:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xbf, 2);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2e, 8, -1, -1, 0x34, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 4:
        Normal_Attack(wk, 0xc, 0x202);
        break;

    case 5:
        Command_Attack(wk, 0xc, 0x1f, 10, -1);
        break;

    case 6:
        Wait(wk, 1);
        break;

    case 7:
        SA_Term(wk, 0x2f, 0x30, 0x31, 0x7f);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0064(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xbf, 2);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 8, -1, -1, 0x34, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Lever_Attack(wk, 9, 0, 0x20);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 4:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 5:
        Normal_Attack(wk, 0xc, 0x202);
        break;

    case 6:
        Command_Attack(wk, 0xc, 0x1f, 10, -1);
        break;

    case 7:
        Wait(wk, 1);
        break;

    case 8:
        SA_Term(wk, 0x2f, 0x30, 0x31, 0x7f);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xbf, 2);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2e, 8, -1, -1, 0x34, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        SA_Term(wk, 0x32, -1, -1, 0xbf);
        break;

    case 5:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 8, -1, -1, 0x34, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x7F);
        break;

    case 3:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0067(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 2:
        Jump_Command_Attack_Term(wk, 0xB, 0x2F, 0xA, -1, -1, 0x40, 0, -1, -1, -1);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0068(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0069(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0xB2, 0xB3, 0xB4, 0xB6, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0xB9, 0xBB, 0xBC, 0xBD, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0071(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0xB5, 0xB7, 0xBF, 0xC0, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0072(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0xB2, 0xB3, 0xB4, 0xB6, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0073(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0xB9, 0xBB, 0xBC, 0xBD, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0074(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0xB5, 0xB7, 0xBF, 0xC0, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0075(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 2:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0077(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0078(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0079(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0080(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0081(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0082(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xD, 0x20);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0083(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 4:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0084(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 4:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 5:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 6:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 7:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0085(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0086(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0087(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 5:
        Lever_On(wk, 1, 2);
        break;

    case 6:
        Wait(wk, 2);
        break;

    case 7:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0088(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x7F);
        break;

    case 2:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 3:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0089(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x7F);
        break;

    case 2:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 3:
        Lever_Attack(wk, 8, 1, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0090(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x7f);
        break;

    case 4:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 5:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0091(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 4:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 5:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x7f);
        break;

    case 6:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 7:
        Lever_Attack(wk, 8, 1, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0092(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x4C, 0x4D, 0x4E, 0x4F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0093(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x50, 0x51, 0x52, 0x53, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0094(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x54, 0x55, 0x56, 0x57, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0095(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x58, 0x59, 0x5A, 0x5B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0096(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x4C, 0x4D, 0x4E, 0x4F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0097(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x50, 0x51, 0x52, 0x53, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0098(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x54, 0x55, 0x56, 0x57, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0099(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x58, 0x59, 0x5A, 0x5B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0100(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2F, 0xA, -1, -0x7F60, 0x50, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x4C, 0x4D, 0x4E, 0x4F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0101(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2F, 0xA, -1, -0x7F60, 0x50, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x50, 0x51, 0x52, 0x53, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0102(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2F, 0xA, -1, -0x7F60, 0x50, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x54, 0x55, 0x56, 0x57, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0103(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2F, 0xA, -1, -0x7F60, 0x50, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x58, 0x59, 0x5A, 0x5B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0104(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x4C, 0x4D, 0x4E, 0x4F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0105(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x50, 0x51, 0x52, 0x53, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0106(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x54, 0x55, 0x56, 0x57, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0107(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x58, 0x59, 0x5A, 0x5B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0108(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 3:
        Com_Random_Select(wk, 2, 0x4C, 0x4D, 0x4E, 0x4F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0109(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 3:
        Com_Random_Select(wk, 2, 0x50, 0x51, 0x52, 0x53, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0110(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 3:
        Com_Random_Select(wk, 2, 0x54, 0x55, 0x56, 0x57, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0111(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 3:
        Com_Random_Select(wk, 2, 0x58, 0x59, 0x5A, 0x5B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0112(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0113(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x22, 8, -1);
        break;

    case 1:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x47);
        break;

    case 2:
        EM_Term(wk, -0x7FB0, -1, 5, 6, 1);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0114(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x300, 2, 0x76);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 0xA, -1);
        break;

    case 2:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x47);
        break;

    case 3:
        EM_Term(wk, -0x7FB0, -1, 5, 6, 1);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0115(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x80, 2, 0x76);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 0xA, -1);
        break;

    case 2:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x47);
        break;

    case 3:
        EM_Term(wk, -0x7FB0, -1, 5, 6, 1);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0116(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x200, 2, 0x77);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    case 2:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x47);
        break;

    case 3:
        EM_Term(wk, -0x7FB0, -1, 5, 6, 1);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0117(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x100, 2, 0x77);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    case 2:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x47);
        break;

    case 3:
        EM_Term(wk, -0x7FB0, -1, 5, 6, 1);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0118(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    case 1:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x47);
        break;

    case 2:
        EM_Term(wk, -0x7FB0, -1, 5, 6, 1);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0119(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    case 1:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x47);
        break;

    case 2:
        EM_Term(wk, -0x7FB0, -1, 5, 6, 1);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0120(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Look(wk, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0121(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x72, 2);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x4C, 0x4D, 0x4E, 0x4F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0122(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x72, 2);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x50, 0x51, 0x52, 0x53, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0123(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x72, 2);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x54, 0x55, 0x56, 0x57, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0124(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x72, 2);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x58, 0x59, 0x5A, 0x5B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0125(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x21, 0x3F, 0x40, 0x41, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0126(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x23, 0x24, 0x25, 0x3D, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0127(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x22, 0x43, 0x3C, 0x3E, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0128(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Look(wk, 2);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x4C, 0x4D, 0x4E, 0x4F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0129(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x72, 2);
        break;

    case 1:
        Look(wk, 2);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x4C, 0x4D, 0x4E, 0x4F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0130(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x71, 0x72, 0x71, 0x72, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0131(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x1A, 0x1C, 0x1D, 0x1E, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0132(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x78, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0133(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x86, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0134(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x77, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0135(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x84, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0136(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x84, 0x85, 0x86, 0x87, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0137(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 0xc, 0x202);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        J_Command_Attack(wk, 0xb, 0x20, 8, -1);
        break;

    case 4:
        Wait(wk, 5);
        break;

    case 5:
        SA_Term(wk, 0x2f, -1, 0x31, 0x7f);
        break;

    case 6:
        J_Command_Attack(wk, 8, 0x1e, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0138(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 1:
        Normal_Attack(wk, 0xc, 0x202);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        J_Command_Attack(wk, 0xb, 0x20, 8, -1);
        break;

    case 4:
        Wait(wk, 5);
        break;

    case 5:
        SA_Term(wk, 0x2f, -1, 0x31, 0x7f);
        break;

    case 6:
        J_Command_Attack(wk, 8, 0x1e, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0139(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 0xc, 0x202);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        J_Command_Attack(wk, 0xb, 0x20, 9, -1);
        break;

    case 4:
        Wait(wk, 3);
        break;

    case 5:
        J_Command_Attack(wk, 8, 0x1e, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0140(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        J_Command_Attack(wk, 0xb, 0x20, 9, -1);
        break;

    case 4:
        Wait(wk, 3);
        break;

    case 5:
        J_Command_Attack(wk, 8, 0x1e, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0141(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x89, 0x8A, 0x8B, 0x8C, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0142(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x57, 3);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 3:
        Com_Random_Select(wk, 2, 0x89, 0x8A, 0x8B, 0x8C, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0143(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x89, 0x8A, 0x8B, 0x8C, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0144(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x26, 0x27, 0x28, 0x29, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0145(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x57, 3);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 3:
        Com_Random_Select(wk, 2, 0x4C, 0x4D, 0x4E, 0x4F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0146(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x57, 3);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 3:
        Com_Random_Select(wk, 2, 0x50, 0x51, 0x52, 0x53, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0147(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x57, 3);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 3:
        Com_Random_Select(wk, 2, 0x54, 0x55, 0x56, 0x57, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0148(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x57, 3);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 3:
        Com_Random_Select(wk, 2, 0x58, 0x59, 0x5A, 0x5B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0149(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x8019, 0xA, -1);
        break;

    case 1:
        J_Command_Attack(wk, 0xB, 0x20, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Pattern14_0150(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA_Full(wk, 6, 0x7C);
        break;

    case 1:
        EM_Term(wk, -0x7F30, -1, 5, 2, 0);
        break;

    case 2:
        Command_Attack(wk, 8, 0x8019, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Pattern14_Tbl[151])(PLW*) = {
    Pattern14_0000, Pattern14_0001, Pattern14_0002, Pattern14_0003, Pattern14_0004, Pattern14_0005, Pattern14_0006,
    Pattern14_0007, Pattern14_0008, Pattern14_0009, Pattern14_0010, Pattern14_0011, Pattern14_0012, Pattern14_0013,
    Pattern14_0014, Pattern14_0015, Pattern14_0016, Pattern14_0017, Pattern14_0018, Pattern14_0019, Pattern14_0020,
    Pattern14_0021, Pattern14_0022, Pattern14_0023, Pattern14_0024, Pattern14_0025, Pattern14_0026, Pattern14_0027,
    Pattern14_0028, Pattern14_0029, Pattern14_0030, Pattern14_0031, Pattern14_0032, Pattern14_0033, Pattern14_0034,
    Pattern14_0035, Pattern14_0036, Pattern14_0037, Pattern14_0038, Pattern14_0039, Pattern14_0040, Pattern14_0041,
    Pattern14_0042, Pattern14_0043, Pattern14_0044, Pattern14_0045, Pattern14_0046, Pattern14_0047, Pattern14_0048,
    Pattern14_0049, Pattern14_0050, Pattern14_0051, Pattern14_0052, Pattern14_0053, Pattern14_0054, Pattern14_0055,
    Pattern14_0056, Pattern14_0057, Pattern14_0058, Pattern14_0059, Pattern14_0060, Pattern14_0061, Pattern14_0062,
    Pattern14_0063, Pattern14_0064, Pattern14_0065, Pattern14_0066, Pattern14_0067, Pattern14_0068, Pattern14_0069,
    Pattern14_0070, Pattern14_0071, Pattern14_0072, Pattern14_0073, Pattern14_0074, Pattern14_0075, Pattern14_0076,
    Pattern14_0077, Pattern14_0078, Pattern14_0079, Pattern14_0080, Pattern14_0081, Pattern14_0082, Pattern14_0083,
    Pattern14_0084, Pattern14_0085, Pattern14_0086, Pattern14_0087, Pattern14_0088, Pattern14_0089, Pattern14_0090,
    Pattern14_0091, Pattern14_0092, Pattern14_0093, Pattern14_0094, Pattern14_0095, Pattern14_0096, Pattern14_0097,
    Pattern14_0098, Pattern14_0099, Pattern14_0100, Pattern14_0101, Pattern14_0102, Pattern14_0103, Pattern14_0104,
    Pattern14_0105, Pattern14_0106, Pattern14_0107, Pattern14_0108, Pattern14_0109, Pattern14_0110, Pattern14_0111,
    Pattern14_0112, Pattern14_0113, Pattern14_0114, Pattern14_0115, Pattern14_0116, Pattern14_0117, Pattern14_0118,
    Pattern14_0119, Pattern14_0120, Pattern14_0121, Pattern14_0122, Pattern14_0123, Pattern14_0124, Pattern14_0125,
    Pattern14_0126, Pattern14_0127, Pattern14_0128, Pattern14_0129, Pattern14_0130, Pattern14_0131, Pattern14_0132,
    Pattern14_0133, Pattern14_0134, Pattern14_0135, Pattern14_0136, Pattern14_0137, Pattern14_0138, Pattern14_0139,
    Pattern14_0140, Pattern14_0141, Pattern14_0142, Pattern14_0143, Pattern14_0144, Pattern14_0145, Pattern14_0146,
    Pattern14_0147, Pattern14_0148, Pattern14_0149, Pattern14_0150
};
