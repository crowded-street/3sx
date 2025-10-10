/**
 * @file pass08.c
 * COM Passive: Elena
 */

#include "sf33rd/Source/Game/com/passive/pass08.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/workuser.h"

void (*const Passive08_Tbl[214])();

void Passive08(PLW* wk) {
    Passive08_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive08_0000(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xD, M_Lv[wk->wu.id]);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0001(PLW* wk) {
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

void Passive08_0002(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x402);
        break;

    case 2:
        J_Command_Attack(wk, 9, 0x1E, 8, -1);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0003(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FB0, 0x28, 7, 1, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0004(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F70, 0x28, 7, 1, -1);
        break;

    case 1:
        SA_Term(wk, 0xFFFF, 0x3A, 0xFFFF, 0);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0005(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x50, 1, -1);
        break;

    case 1:
        Jump(wk, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0006(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 6, 1, -1);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0007(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 6, 1, -1);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0008(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 1:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0009(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FB0, 6, 1, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FD0, 6, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0011(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0012(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0014(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x100);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0015(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0016(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0018(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FB8, 6, 1, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0019(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x20);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0020(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0021(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0022(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0023(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0024(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0026(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0027(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1D, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0028(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1D, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0029(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0031(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 2:
        Lever_Attack(wk, 9, 0, 0x20);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Lever_Attack(wk, 0xB, 0, 0x20);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0033(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0035(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0036(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0037(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1E, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F80, -1, 6, 1, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0039(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F80, -1, 6, 1, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F80, -1, 6, 1, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0041(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F80, -1, 6, 1, -1);
        break;

    case 1:
        SA_Term(wk, 0x39, 0xFFFF, 0xFFFF, 0);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0042(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Forced_Guard(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0043(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, 0x40, 8, 0x100, 0, -0x7FC0, -1, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0044(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, 0x40, 8, 0x400, 0, -0x7FC0, -1, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0046(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0047(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0048(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0049(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x100);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0050(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0051(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0053(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x400);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
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

void Passive08_0055(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0056(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0xBC);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 8, 0x700);
        break;

    case 2:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    case 3:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 4:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    case 5:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 6:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0057(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x22);
        break;

    case 2:
        SA_Term(wk, 0x39, 0xFFFF, 0x3B, 0);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0058(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Provoke(wk, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0059(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0060(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0x3A);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 10, 0x70);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0061(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Lie(wk, 0);
        break;

    case 1:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 2:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0062(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FB0, 0x28, 7, 1, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 10, 0x700);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0064(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x400);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Walk(wk, 0, 0x10, 0);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    case 4:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 5:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    case 6:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 7:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0067(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0068(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Lie(wk, 0);
        break;

    case 1:
        Approach_Walk(wk, 0x7B, 2);
        break;

    case 2:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0069(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 0xB, 0x400, 0, -0x7FB0, -1, 0x200);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0071(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Lie(wk, 0);
        break;

    case 1:
        Jump(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0072(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Lie(wk, 0);
        break;

    case 1:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 2:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0073(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0074(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FC0, -1, 6, 1, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x100);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0075(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FB0, -1, 0, 1, -1);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0077(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0078(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 2:
        Branch_Unit_Area(wk, 6, 0x55, 0x2F, 0x1B, 0x25);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0079(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 2:
        Branch_Unit_Area(wk, 6, 0x53, 0x2F, 0x24, 0x25);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0080(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1D, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0081(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40);
        break;

    case 1:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1D, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0082(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200);
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
        Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    case 5:
        Com_Random_Select(wk, 6, 0x2D, 0xFF, 0xFF, 0xFF, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0083(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0084(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0085(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0086(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0087(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0088(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0089(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, 0xC, 0, -1, -1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0090(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0091(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Lever_Attack(wk, 0xC, 0, 0x20);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0092(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Lever_Attack(wk, 0xC, 0, 0x20);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x1E, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0093(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F80, 0x28, 7, 1, -1);
        break;

    case 1:
        Lever_Attack(wk, 9, 0, 0x200);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0094(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0095(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0096(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0097(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0098(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0099(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0100(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0101(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 1:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0102(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -1, -1, -1);
        break;

    case 1:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0103(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0104(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x402);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0105(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FD0, 6, 1, -1);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0106(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0107(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FC0, 6, 1, -1);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0108(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FC8, 6, 1, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1D, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0109(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FC8, 0, 1, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0110(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x20);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0111(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x20);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0112(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7B, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0113(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0114(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0115(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x20);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1E, 10, -1);
        break;

    case 3:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0116(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Forced_Guard(wk, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0117(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0118(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1D, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0119(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0120(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1D, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0121(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0122(PLW* wk) {
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

void Passive08_0123(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0124(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1D, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0125(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 1:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0126(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0127(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x12);
        break;

    case 3:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0128(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -1, -1, -1);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0129(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x100);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 5:
        J_Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0130(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x100);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1D, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0131(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1D, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0132(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x400);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0133(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x400);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1D, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0134(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0135(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FB0, 6, 1, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0136(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FC0, 6, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0137(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0138(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0139(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0140(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0141(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1D, 10, -1);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 4:
        Command_Attack(wk, 8, 1, 10, -1);
        break;

    case 5:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 6:
        Command_Attack(wk, 8, 1, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0142(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FC0, 6, 1, -1);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1C, 8, -1);
        break;

    case 3:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 6:
        Command_Attack(wk, 8, 1, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0143(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF0, 6, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 6:
        Command_Attack(wk, 8, 1, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0144(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 6, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    case 4:
        Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0145(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FD0, 6, 1, -1);
        break;

    case 2:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 3:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x1D, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0146(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, 0xC, 0, -1, -1);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0147(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, 8, 0x20, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0148(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 0xE, 0x20, 0xA, -1);
        break;

    case 1:
        Command_Attack(wk, 0xE, 0x20, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0149(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 0xE, 0x20, 9, -1);
        break;

    case 1:
        Command_Attack(wk, 0xE, 0x20, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0150(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_EX(wk, 6, 0x95);
        break;

    case 1:
        Command_Attack(wk, 0xE, 0x20, 0xA, 0x700);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0151(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 4:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0152(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 3:
        Com_Random_Select(wk, 6, 0x2D, 0xFF, 0xFF, 0xFF, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0153(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0154(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xBF, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0155(PLW* wk) {
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

void Passive08_0156(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xBF, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0157(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 2:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40);
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x400);
        break;

    case 4:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 5:
        J_Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0158(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 2:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -1, -1, -1);
        break;

    case 3:
        Normal_Attack(wk, 0xB, 0x12);
        break;

    case 4:
        SA_Term(wk, 0x39, 0x3A, 0xFFFF, 0);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0159(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 2:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 10, -1);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x402);
        break;

    case 5:
        Pierce_On(wk);
        break;

    case 6:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 7:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0160(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x20);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0161(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 0xB, 0x200, 0, -0x7FB0, -1, 0x40);
        break;

    case 1:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0162(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x20, 0, -0x7FB0, -1, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x12);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x202);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0163(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x200);
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
        Com_Random_Select(wk, 6, 0x2D, 0xFF, 0xFF, 0xFF, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0164(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x400);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0165(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x402);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0166(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x102);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0167(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 9, 0x400, 0, -0x7FB0, -1, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x402);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0168(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x20, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0169(PLW* wk) {
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

void Passive08_0170(PLW* wk) {
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

void Passive08_0171(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xBF, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0172(PLW* wk) {
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

void Passive08_0173(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0xFFFF, 0xFFFF, 0x3B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0174(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF0, 6, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 3:
        J_Command_Attack(wk, 0xC, 0x1E, 9, -1);
        break;

    case 4:
        Wait(wk, 5);
        break;

    case 5:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0175(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x10);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0176(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x200);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    case 4:
        Com_Random_Select(wk, 6, 0x2D, 0xFF, 0xFF, 0xFF, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0177(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x200);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    case 2:
        Com_Random_Select(wk, 6, 0x2D, 0xFF, 0xFF, 0xFF, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0178(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x40);
        break;

    case 2:
        J_Command_Attack(wk, 0xC, 0x1E, 0xA, -1);
        break;

    case 3:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0179(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0180(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 9, -1);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0181(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0182(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0183(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FC0, 9, 0x40, 0, -0x7FB0, -1, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1D, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0184(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC0, 9, 0x200, 0, -0x7FB0, -1, 0x200);
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
        J_Command_Attack(wk, 8, 0x1D, 10, -1);
        break;

    case 5:
        Com_Random_Select(wk, 6, 0x2D, 0xFF, 0xFF, 0xFF, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0185(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0186(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    case 2:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 3:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0187(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1D, 9, -1);
        break;

    case 2:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 3:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0188(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1D, 0xA, -1);
        break;

    case 2:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 3:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0189(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F70, 0x28, 7, 1, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0190(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F70, 0x28, 7, 1, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0191(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x60, 2);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0x3F, 0x40, 0x41, 0x43, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0192(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xBF, 1);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0xBA, 1, 0xAF, 0x92, 3);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0193(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Walk(wk, 1, 0x20, -1);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0xBA, 1, 0xAF, 0x92, 3);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0194(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x200);
        break;

    case 1:
        Com_Random_Select(wk, 6, 1, 1, 0xC0, 0xC0, 4);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0195(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x400);
        break;

    case 1:
        Com_Random_Select(wk, 6, 1, 1, 0xC0, 0xC0, 4);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0196(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F70, 0x28, 7, 1, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1D, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0197(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F70, 0x28, 7, 1, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0198(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F70, 0x28, 7, 1, -1);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0199(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0200(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    case 2:
        Pierce_On(wk);
        break;

    case 3:
        Search_Back_Term(wk, 0x60, 1, -1);
        break;

    case 4:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0201(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x40);
        break;

    case 1:
        J_Command_Attack(wk, 0xC, 0x1C, 0xA, -1);
        break;

    case 2:
        Wait(wk, 5);
        break;

    case 3:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0202(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 1:
        SA_Term(wk, 0x39, 0x3A, 0x3B, 0);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1D, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0203(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 2);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0204(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 6);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0205(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, 1);
        break;

    case 1:
        Look(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0206(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -1, 8, 0x400, 1, -1, 0x20, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0207(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FB0, 8, 0x200, 2, -1, -0x7FB0, 0x200);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1C, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0208(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FB0, 8, 0x400, 2, -1, -0x7FB0, 0x400);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0xAF, 0xB0, 0x78, 0x7B, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0209(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, -0x7FB0, 8, 0x40, 2, -1, -0x7FB0, 0x40);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x20, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0210(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FB0, 8, 0x200, 2, -1, -0x7FB0, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0211(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0212(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 9, 0x1F, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive08_0213(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7F00, 0x38, 8, 0x200, 1, -1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Passive08_Tbl[214])(PLW*) = {
    Passive08_0000, Passive08_0001, Passive08_0002, Passive08_0003, Passive08_0004, Passive08_0005, Passive08_0006,
    Passive08_0007, Passive08_0008, Passive08_0009, Passive08_0010, Passive08_0011, Passive08_0012, Passive08_0013,
    Passive08_0014, Passive08_0015, Passive08_0016, Passive08_0017, Passive08_0018, Passive08_0019, Passive08_0020,
    Passive08_0021, Passive08_0022, Passive08_0023, Passive08_0024, Passive08_0025, Passive08_0026, Passive08_0027,
    Passive08_0028, Passive08_0029, Passive08_0030, Passive08_0031, Passive08_0032, Passive08_0033, Passive08_0034,
    Passive08_0035, Passive08_0036, Passive08_0037, Passive08_0038, Passive08_0039, Passive08_0040, Passive08_0041,
    Passive08_0042, Passive08_0043, Passive08_0044, Passive08_0045, Passive08_0046, Passive08_0047, Passive08_0048,
    Passive08_0049, Passive08_0050, Passive08_0051, Passive08_0052, Passive08_0053, Passive08_0054, Passive08_0055,
    Passive08_0056, Passive08_0057, Passive08_0058, Passive08_0059, Passive08_0060, Passive08_0061, Passive08_0062,
    Passive08_0063, Passive08_0064, Passive08_0065, Passive08_0066, Passive08_0067, Passive08_0068, Passive08_0069,
    Passive08_0070, Passive08_0071, Passive08_0072, Passive08_0073, Passive08_0074, Passive08_0075, Passive08_0076,
    Passive08_0077, Passive08_0078, Passive08_0079, Passive08_0080, Passive08_0081, Passive08_0082, Passive08_0083,
    Passive08_0084, Passive08_0085, Passive08_0086, Passive08_0087, Passive08_0088, Passive08_0089, Passive08_0090,
    Passive08_0091, Passive08_0092, Passive08_0093, Passive08_0094, Passive08_0095, Passive08_0096, Passive08_0097,
    Passive08_0098, Passive08_0099, Passive08_0100, Passive08_0101, Passive08_0102, Passive08_0103, Passive08_0104,
    Passive08_0105, Passive08_0106, Passive08_0107, Passive08_0108, Passive08_0109, Passive08_0110, Passive08_0111,
    Passive08_0112, Passive08_0113, Passive08_0114, Passive08_0115, Passive08_0116, Passive08_0117, Passive08_0118,
    Passive08_0119, Passive08_0120, Passive08_0121, Passive08_0122, Passive08_0123, Passive08_0124, Passive08_0125,
    Passive08_0126, Passive08_0127, Passive08_0128, Passive08_0129, Passive08_0130, Passive08_0131, Passive08_0132,
    Passive08_0133, Passive08_0134, Passive08_0135, Passive08_0136, Passive08_0137, Passive08_0138, Passive08_0139,
    Passive08_0140, Passive08_0141, Passive08_0142, Passive08_0143, Passive08_0144, Passive08_0145, Passive08_0146,
    Passive08_0147, Passive08_0148, Passive08_0149, Passive08_0150, Passive08_0151, Passive08_0152, Passive08_0153,
    Passive08_0154, Passive08_0155, Passive08_0156, Passive08_0157, Passive08_0158, Passive08_0159, Passive08_0160,
    Passive08_0161, Passive08_0162, Passive08_0163, Passive08_0164, Passive08_0165, Passive08_0166, Passive08_0167,
    Passive08_0168, Passive08_0169, Passive08_0170, Passive08_0171, Passive08_0172, Passive08_0173, Passive08_0174,
    Passive08_0175, Passive08_0176, Passive08_0177, Passive08_0178, Passive08_0179, Passive08_0180, Passive08_0181,
    Passive08_0182, Passive08_0183, Passive08_0184, Passive08_0185, Passive08_0186, Passive08_0187, Passive08_0188,
    Passive08_0189, Passive08_0190, Passive08_0191, Passive08_0192, Passive08_0193, Passive08_0194, Passive08_0195,
    Passive08_0196, Passive08_0197, Passive08_0198, Passive08_0199, Passive08_0200, Passive08_0201, Passive08_0202,
    Passive08_0203, Passive08_0204, Passive08_0205, Passive08_0206, Passive08_0207, Passive08_0208, Passive08_0209,
    Passive08_0210, Passive08_0211, Passive08_0212, Passive08_0213
};
