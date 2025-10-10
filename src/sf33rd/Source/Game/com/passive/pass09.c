/**
 * @file pass09.c
 * COM Passive: Oro
 */

#include "sf33rd/Source/Game/com/passive/pass09.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/workuser.h"

void (*const Passive09_Tbl[132])();

void Passive09(PLW* wk) {
    Passive09_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive09_0000(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xD, M_Lv[wk->wu.id]);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0001(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 1:
        EM_Term(wk, -0x7FB0, -0x7FD8, 3, 1, -1);
        break;

    case 2:
        Check_Store_Lever(wk, 0x1C, 6, 0x3C);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1C, 10, 0x70);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0002(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F10, -1, 5, 6, 0x2A);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0003(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_Store_Lever(wk, 0x1E, 1, -1);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0004(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 6);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1F, 8, 0x700);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0005(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_Store_Lever(wk, 0x1E, 1, -1);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0006(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0007(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 1:
        EM_Term(wk, -0x7FB0, -0x7FD8, 3, 1, -1);
        break;

    case 2:
        Check_Store_Lever(wk, 0x1C, 6, 0x3C);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0008(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 1:
        EM_Term(wk, -0x7FB0, -0x7FD8, 3, 1, -1);
        break;

    case 2:
        Check_Store_Lever(wk, 0x1C, 6, 0x3C);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0009(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 1:
        EM_Term(wk, -0x7FB0, -0x7FD8, 3, 1, -1);
        break;

    case 2:
        Check_Store_Lever(wk, 0x1C, 6, 0x3C);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F90, -0x7FC0, 0, 6, 0x3C);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0011(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 8, 0x20, 2, -0x7F70, -1, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0012(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F68, -0x7FC0, 3, 2, 0);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 1:
        Command_Attack(wk, 8, 1, 0xB, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0014(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        VS_Jump_Guard(wk);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0015(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6C);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0016(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        EM_Term(wk, -0x7FB0, -1, 5, 6, 0x1C);
        break;

    case 3:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0019(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0020(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0021(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x2F, 9, -1, -1, -0x7FC0, 0, -1, -1, 0xFFFF);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0022(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7F90, -0x7FC0, 8, 0x200, 0, -0x7F68, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0023(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7F98, -0x7FA8, 8, 0x40, 0, -0x7F78, -1, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0024(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Command_Attack(wk, 8, 0, 0xB, -1);
        break;

    case 2:
        EM_Term(wk, 0x7FFF, -1, 1, 1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0026(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        EM_Term(wk, 0x7FFF, -1, 1, 1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0027(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0028(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0029(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0031(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x20, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 1, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6C);
        break;

    case 1:
        Adjust_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0035(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x102);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0036(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0037(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF8, 6, 1, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF8, 6, 1, -1);
        break;

    case 1:
        Lever_Attack(wk, 8, 1, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0039(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FD0, 6, 1, -1);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 8, 0x70);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF0, 6, 6, 0x1F);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6D);
        break;

    case 1:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF0, 6, 6, 0x1F);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x22);
        break;

    case 4:
        Check_Store_Lever(wk, 0x1C, 1, -1);
        break;

    case 5:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0042(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0043(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FB0, 6, 6, 0x1F);
        break;

    case 3:
        Check_Store_Lever(wk, 0x1C, 1, -1);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0044(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 3, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF0, 6, 6, 0x1F);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0046(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FA0, -0x7FC8, 0, 6, 0x3C);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x42);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0047(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7F90, -0x7FC8, 8, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0048(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0049(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6D);
        break;

    case 1:
        EM_Term(wk, -0x7FB0, -1, 3, 6, 0x1F);
        break;

    case 2:
        Adjust_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0050(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FB0, -1, 3, 6, 0x1F);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0051(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x12);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0053(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, 0x58, -1, 5, 6, 0x2A);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1F, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0055(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F60, -1, 5, 6, 0x2A);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1F, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0056(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FB0, -0x7FC0, 8, 0x400, 0, -0x7F68, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0057(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7F98, -0x7FA8, 8, 0x100, 0, -0x7F78, -1, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0058(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Status(wk, 2, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0059(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 1:
        EM_Term(wk, -0x7FB0, -1, 3, 6, 0x1F);
        break;

    case 2:
        Check_Store_Lever(wk, 0x1C, 1, -1);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0060(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Forced_Guard(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0061(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0062(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x200);
        break;

    case 1:
        ETC_Term(wk, 1, 6, 0x6E);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0x35, 0xFFFF, 0xFFFF, 0x9F);
        break;

    case 1:
        SA_Term(wk, 0xFFFF, 0x64, 0x3A, 0);
        break;

    case 2:
        Command_Attack(wk, 8, 0x8016, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0064(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        ETC_Term(wk, 1, 6, 0x6E);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x37, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x200);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        SA_Term(wk, 0xFFFF, 0x64, 0xFFFF, 0);
        break;

    case 2:
        Approach_Walk(wk, 0x9F, 2);
        break;

    case 3:
        SA_Term(wk, 0x35, 0xFFFF, 0xFFFF, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0067(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Attack_Complete(wk, 3, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0068(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 0, 0x15, 0);
        break;

    case 1:
        Walk(wk, 1, 0x13, 0);
        break;

    case 2:
        Walk(wk, 0, 0x10, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0069(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Attack_Complete(wk, 3, 1);
        break;

    case 1:
        SA_Term(wk, 0xFFFF, 0xFFFF, 0x3A, 0);
        break;

    case 2:
        SA_Term(wk, 0x35, 0xFFFF, 0xFFFF, 0x9F);
        break;

    case 3:
        Wait_Attack_Complete(wk, 3, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        SA_Term(wk, 0x35, 0x64, 0x3A, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0071(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0xFFFF, 0xFFFF, 0x3A, 0);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0072(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0x35, 0xFFFF, 0xFFFF, 0);
        break;

    case 1:
        EM_Term(wk, -0x7F90, -0x7FF0, 3, 6, 0x1C);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0073(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0074(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0x35, 0x64, 0x3A, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0075(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6C);
        break;

    case 1:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 2:
        EM_Term(wk, -0x7FC0, -1, 3, 6, 0x1F);
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x12);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6E);
        break;

    case 1:
        EM_Term(wk, -0x7FB0, -1, 5, 6, 0x1C);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0077(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x40, 2);
        break;

    case 1:
        EM_Term(wk, -0x7FB8, -0x7FF0, 3, 6, 0x1C);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x200);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1D, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0078(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7F90, -0x7FC0, 8, 0x400, 2, -0x7F70, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0079(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Forced_Guard(wk, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0080(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, -0x5FF7, 0x400, 0, -0x7FB0, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x22);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0081(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7F98, -0x7FA8, 9, 0x40, 0, -0x7FB0, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1D, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0082(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF0, 6, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 3:
        Command_Attack(wk, 0xC, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0083(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0084(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -1, -0x7FC0, 9, 0x100, 1, -0x7FB0, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0085(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x220);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0086(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xBF, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0087(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FC0, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0088(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, -0x5FF7, 0x200, 0, -0x7FB0, -1, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 4:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    case 5:
        Com_Random_Select(wk, 6, 0x2D, 0xFF, 0xFF, 0xFF, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0089(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 2:
        Branch_Unit_Area(wk, 6, 0x20, 0x3B, 0x36, 0x5A);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0090(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1E, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0091(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FB0, -0x7FF0, 6, 1, -1);
        break;

    case 1:
        Command_Attack(wk, 0xC, 0x1E, 10, -1);
        break;

    case 2:
        Wait(wk, 5);
        break;

    case 3:
        SA_Term(wk, 0x35, 0xFFFF, 0x3A, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0092(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x60, 0x61, 0x62, 99, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0093(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FA8, 9, 0x40, 0, -0x7FB0, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1E, 10, -1);
        break;

    case 3:
        Wait(wk, 5);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0094(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0xFFFF, 0x64, 0xFFFF, 0x36);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0095(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0xFFFF, 0x64, 0xFFFF, 0x37);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0096(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0xFFFF, 0x64, 0xFFFF, 0x38);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0097(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0xFFFF, 0x64, 0xFFFF, 0x39);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0098(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF0, 5, 6, 0x1F);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x200);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1F, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0099(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 1, 6, 0x6D);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x20);
        break;

    case 2:
        Com_Random_Select(wk, 6, 5, 5, 100, 0x5A, 4);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0100(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0101(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x2F, 9, -1, -1, -0x7FC0, 0, -1, -1, 0xFFFF);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0102(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1F, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0103(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0104(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Jump(wk, 0);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0105(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 1, 2);
        break;

    case 1:
        Jump(wk, 0);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1F, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0106(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_On(wk, 0xFFFF, 2);
        break;

    case 1:
        EM_Term(wk, -1, 0x30, 6, 1, -1);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0107(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7F90, 6, 2, 0);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0108(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FC8, -0x7FF8, 5, 1, -1);
        break;

    case 1:
        Adjust_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0109(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FC0, -0x7FF8, 5, 1, -1);
        break;

    case 1:
        Adjust_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0110(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F98, -0x7FF8, 5, 1, -1);
        break;

    case 1:
        Adjust_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0111(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0112(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_JCA_Term(wk, 8, 1, 0x40, 0, -0x7FA0, -1, 0x2F, 0xA, -1, -0x7FB0, -0x7FD0, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0113(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_HJCA_Term(wk, 8, 1, 0x40, 0, -0x7FA0, -1, 0x2F, 0xA, -1, -0x7FB0, -0x7FD0, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0114(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_HJCA_Term(wk, 8, 2, 0x40, 0, -0x7FA0, -1, 0x2F, 0xA, -1, -0x7FB0, -0x7FD0, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0115(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FD0, 0x10, 8, 0x110, 0, -0x7FA0, -1, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0116(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_JA_Term(wk, 8, 1, 0x40, 1, -1, -1, 0xFFFF, -0x7FB0, -0x7FD0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0117(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_JA_Term(wk, 8, 1, 0x50, 0, -1, -1, 0xFFFF, -0x7FB0, -0x7FD0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0118(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_JA_Term(wk, 8, 2, 0x40, 0, -1, -1, 0xFFFF, -0x7FB0, -0x7FD0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0119(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_JA_Term(wk, 8, 2, 0x40, 1, -1, -1, 0xFFFF, -0x7FB0, -0x7FD0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0120(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x74, 0x74, 0x75, 0x75, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0121(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_JA_Term(wk, 8, 0, 0x40, 2, -1, -1, 0xFFFF, -0x7FB0, -0x7FD0, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0122(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ORO_JA_Term(wk, 8, 0, 0x50, 0, -1, -1, 0xFFFF, -0x7FB0, -0x7FD0, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0123(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x79, 0x7A, 0x7A, 0x2F, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0124(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x76, 0x76, 0x77, 0x77, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0125(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FD0, 0x10, 8, 0x110, 0, -1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0126(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1F, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0127(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0128(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FA0, 8, 0x100, 0, -0x7F78, -1, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0129(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FA0, 8, 0x40, 0, -0x7F78, -1, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0130(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FA0, 8, 0x100, 0, -0x7F78, -1, 0x40);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive09_0131(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FA0, 8, 0x40, 0, -0x7F78, -1, 0x40);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1D, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Passive09_Tbl[132])(PLW*) = {
    Passive09_0000, Passive09_0001, Passive09_0002, Passive09_0003, Passive09_0004, Passive09_0005, Passive09_0006,
    Passive09_0007, Passive09_0008, Passive09_0009, Passive09_0010, Passive09_0011, Passive09_0012, Passive09_0013,
    Passive09_0014, Passive09_0015, Passive09_0016, Passive09_0017, Passive09_0018, Passive09_0019, Passive09_0020,
    Passive09_0021, Passive09_0022, Passive09_0023, Passive09_0024, Passive09_0025, Passive09_0026, Passive09_0027,
    Passive09_0028, Passive09_0029, Passive09_0030, Passive09_0031, Passive09_0032, Passive09_0033, Passive09_0034,
    Passive09_0035, Passive09_0036, Passive09_0037, Passive09_0038, Passive09_0039, Passive09_0040, Passive09_0041,
    Passive09_0042, Passive09_0043, Passive09_0044, Passive09_0045, Passive09_0046, Passive09_0047, Passive09_0048,
    Passive09_0049, Passive09_0050, Passive09_0051, Passive09_0052, Passive09_0053, Passive09_0054, Passive09_0055,
    Passive09_0056, Passive09_0057, Passive09_0058, Passive09_0059, Passive09_0060, Passive09_0061, Passive09_0062,
    Passive09_0063, Passive09_0064, Passive09_0065, Passive09_0066, Passive09_0067, Passive09_0068, Passive09_0069,
    Passive09_0070, Passive09_0071, Passive09_0072, Passive09_0073, Passive09_0074, Passive09_0075, Passive09_0076,
    Passive09_0077, Passive09_0078, Passive09_0079, Passive09_0080, Passive09_0081, Passive09_0082, Passive09_0083,
    Passive09_0084, Passive09_0085, Passive09_0086, Passive09_0087, Passive09_0088, Passive09_0089, Passive09_0090,
    Passive09_0091, Passive09_0092, Passive09_0093, Passive09_0094, Passive09_0095, Passive09_0096, Passive09_0097,
    Passive09_0098, Passive09_0099, Passive09_0100, Passive09_0101, Passive09_0102, Passive09_0103, Passive09_0104,
    Passive09_0105, Passive09_0106, Passive09_0107, Passive09_0108, Passive09_0109, Passive09_0110, Passive09_0111,
    Passive09_0112, Passive09_0113, Passive09_0114, Passive09_0115, Passive09_0116, Passive09_0117, Passive09_0118,
    Passive09_0119, Passive09_0120, Passive09_0121, Passive09_0122, Passive09_0123, Passive09_0124, Passive09_0125,
    Passive09_0126, Passive09_0127, Passive09_0128, Passive09_0129, Passive09_0130, Passive09_0131
};
