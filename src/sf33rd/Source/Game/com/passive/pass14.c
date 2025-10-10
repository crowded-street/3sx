/**
 * @file pass14.c
 * COM Passive: Akuma/Gouki
 */

#include "sf33rd/Source/Game/com/passive/pass14.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/workuser.h"

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

void Passive14_0002(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 6, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x10);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0003(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FDC, 0, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0004(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FE0, 0, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0005(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x10, 2);
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

void Passive14_0006(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 6, 1, -1);
        break;

    case 2:
        Lever_Attack(wk, 8, 1, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0007(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
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
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0009(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0010(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x1E, 10, -1);
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
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
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
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0012(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x20, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0013(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7F00, 6, 6, 0x7C);
        break;

    case 1:
        Command_Attack(wk, 8, 0x21, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0014(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FC8, 6, 6, 0x7C);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1F, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0015(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FD0, 6, 6, 0x7C);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
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
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        J_Command_Attack(wk, 9, 0x20, 8, -1);
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        Wait(wk, 3);
        break;

    case 6:
        J_Command_Attack(wk, 0xC, 0x1E, 10, -1);
        break;

    case 7:
        Wait(wk, 3);
        break;

    case 8:
        SA_Term(wk, 0xFFFF, 0xFFFF, 0x31, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        J_Command_Attack(wk, 0xC, 0x1E, 10, -1);
        break;

    case 4:
        Wait(wk, 4);
        break;

    case 5:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
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
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x20, 10, -1);
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
        EM_Term(wk, -1, -0x7FF8, 6, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x42);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0020(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF0, 0, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0021(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        EM_Term(wk, -1, -0x7F80, 6, 6, 0x7C);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0022(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FB0, 6, 6, 0x7C);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x20, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0023(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2F, 10, -1, -1, 0x30, 0, -1, -1, 0xFFFF);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0024(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 3:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF8, 6, 1, -1);
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
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF8, 6, 1, -1);
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
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
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
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        J_Command_Attack(wk, 0xC, 0x1E, 10, -1);
        break;

    case 5:
        Wait(wk, 4);
        break;

    case 6:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0029(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF0, 6, 1, -1);
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 4:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
        break;

    case 5:
        Wait(wk, 1);
        break;

    case 6:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0030(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        J_Command_Attack(wk, 0xB, 0x20, 8, -1);
        break;

    case 6:
        Wait(wk, 3);
        break;

    case 7:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0031(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 4:
        J_Command_Attack(wk, 8, 0x20, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FD0, 6, 6, 0x7C);
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
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0034(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x3F, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    case 5:
        J_Command_Attack(wk, 8, 0x1E, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0035(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        J_Command_Attack(wk, 0xB, 0x20, 8, -1);
        break;

    case 5:
        Pierce_On(wk);
        break;

    case 6:
        Wait(wk, 3);
        break;

    case 7:
        J_Command_Attack(wk, 0xC, 0x1E, 10, -1);
        break;

    case 8:
        Wait(wk, 3);
        break;

    case 9:
        SA_Term(wk, 0xFFFF, 0xFFFF, 0x31, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0036(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0xFFFF, 0x30, 0x31, 0x7F);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0037(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FA0, -0x7FA0, 6, 6, 0x35);
        break;

    case 1:
        SA_Term(wk, 0xFFFF, 0x30, 0x31, 0x7F);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0xFFFF, 0x30, 0x31, 0x7F);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0039(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FB0, 6, 6, 0x35);
        break;

    case 1:
        SA_Term(wk, 0xFFFF, 0x30, 0x31, 0x7F);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0040(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 3);
        break;

    case 1:
        EM_Term(wk, -0x7FC0, -0x7FC8, 6, 6, 0x35);
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
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FD0, 6, 6, 0x35);
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
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0043(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, 0x38, 8, 0x200, 2, -0x7FA0, -1, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0044(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA0, 0x38, 8, 0x400, 2, -0x7FA0, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Pierce_On(wk);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2F, 10, -1, -1, 0x10, 2, -1, -1, 0xFFFF);
        break;

    case 2:
        SA_Term(wk, 0xFFFF, 0xFFFF, 0x31, 0x7F);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 10, -1);
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
        EM_Term(wk, -1, -0x7FF0, 6, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
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
        EM_Term(wk, -1, -0x7FF0, 6, 1, -1);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        J_Command_Attack(wk, 0xC, 0x1E, 10, -1);
        break;

    case 4:
        Wait(wk, 3);
        break;

    case 5:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0048(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7F80, 0x20, 8, 0x400, 1, -0x7F80, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0049(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FB0, -0x7FB0, 6, 6, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0050(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FC0, -0x7FF0, 6, 1, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x100);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0051(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7FB0, -0x7FC0, 0, 1, -1);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0052(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0053(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Forced_Guard(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0054(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0055(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0056(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0057(PLW* wk) {
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

void Passive14_0058(PLW* wk) {
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

void Passive14_0059(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0060(PLW* wk) {
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

void Passive14_0061(PLW* wk) {
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

void Passive14_0062(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xBF, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0063(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 4, 6, 0x37);
        break;

    case 1:
        Provoke(wk, -1);
        break;

    case 2:
        Next_Another_Menu(wk, 6, 0x39);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0064(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0x7F, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0065(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0x7F, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0066(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0x7F, 0);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0067(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0068(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Search_Back_Term(wk, 0xE0, 6, 0x45);
        break;

    case 2:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FD0, 0xB, 0x200, 0, -0x7F80, -1, 0x400);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x1E, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0069(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Branch_Wait_Area(wk, 0x14, 0xF, 5, 1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1F, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0070(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Branch_Wait_Area(wk, 0x14, 0xF, 5, 1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0071(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0072(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x7F, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 2:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0073(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Branch_Wait_Area(wk, 10, 7, 3, 1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x21, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0074(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Branch_Wait_Area(wk, 10, 7, 3, 1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x21, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0075(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Branch_Wait_Area(wk, 10, 7, 3, 1);
        break;

    case 2:
        Command_Attack(wk, 8, 0x21, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0076(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Branch_Wait_Area(wk, 0xF, 10, 5, 1);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x20, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0077(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, 0);
        break;

    case 1:
        Search_Back_Term(wk, 0xE0, 6, 0x45);
        break;

    case 2:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FE0, 0xB, 0x200, 0, -0x7F80, -1, 0x400);
        break;

    case 3:
        J_Command_Attack(wk, 8, 0x20, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0078(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        ETC_Term(wk, 4, 6, 0x37);
        break;

    case 1:
        Provoke(wk, -1);
        break;

    case 2:
        Next_Another_Menu(wk, 6, 0x46);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0079(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0080(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0081(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x300, 6, 0xE3);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 10, -1);
        break;

    case 2:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x47);
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

void Passive14_0082(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x80, 6, 0xE3);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 10, -1);
        break;

    case 2:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x47);
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

void Passive14_0083(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x200, 6, 0xE4);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    case 2:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x47);
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

void Passive14_0084(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x100, 6, 0xE4);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 8, -1);
        break;

    case 2:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x47);
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

void Passive14_0085(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 1:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
        break;

    case 2:
        Wait(wk, 1);
        break;

    case 3:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0086(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0087(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x20, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0088(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0089(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x102);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0090(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x20, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0091(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 1:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0092(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 1:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0093(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 1:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0094(PLW* wk) {
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
        Command_Attack(wk, 8, 0x1F, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0095(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
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

void Passive14_0096(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        J_Command_Attack(wk, 0xC, 0x1E, 10, -1);
        break;

    case 3:
        Wait(wk, 3);
        break;

    case 4:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0097(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Keep_Away(wk, 0xBF, 1);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0x15, 0x1D, 0x1E, 0x1F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0098(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x202);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0099(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x200);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0100(PLW* wk) {
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

void Passive14_0101(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 10, 0x202);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0102(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 10, 0x102);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0103(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 10, 0x202);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0104(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x102);
        break;

    case 1:
        Normal_Attack(wk, 10, 0x202);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
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

void Passive14_0105(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0106(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    case 1:
        Command_Attack(wk, 8, 0x21, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0107(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0108(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0109(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0110(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0111(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x402);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0112(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x20);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0113(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 8, -1, -1, 0x20, 2, -0x7F80, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0114(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 9, -1, -1, 0x20, 2, -0x7F80, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0115(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 0xA, -1, -1, 0x20, 2, -0x7F80, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0116(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 8, -1, -1, 0x20, 0, -0x7F80, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0117(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 9, -1, -1, 0x20, 0, -0x7F80, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0118(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 0xA, -1, -1, 0x20, 0, -0x7F80, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0119(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 8, -1, -1, 0x20, 1, -0x7F80, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0120(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 9, -1, -1, 0x20, 1, -0x7F80, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0121(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 8, 0x2E, 0xA, -1, -1, 0x20, 1, -0x7F80, -1, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0122(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x71, 0x71, 0x72, 0x73, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0123(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x74, 0x74, 0x75, 0x76, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0124(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0125(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 1);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0x57, 0x57, 0x58, 0x59, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0126(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 1);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0x5A, 0x5B, 0x5C, 0x5D, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0127(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0x57, 0x57, 0x58, 0x59, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0128(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0x5A, 0x5B, 0x5C, 0x5D, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0129(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, 0, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0130(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x10, 2);
        break;

    case 1:
        Lever_Attack(wk, 8, 1, 0x110);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0131(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x81, 0x81, 0x82, 0x82, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0132(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1F, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0133(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1F, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0134(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0135(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x21, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0136(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x21, 9, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0137(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x21, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0138(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x84, 0x84, 0x85, 0x86, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0139(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x87, 0x87, 0x88, 0x89, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0140(PLW* wk) {
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

void Passive14_0141(PLW* wk) {
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

void Passive14_0142(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 9, 0x20, 10, -1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0143(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 1);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0x8C, 0x8C, 0x8D, 0x8E, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0144(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0145(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0x90, 0xFF, 0xFF, 0xFF, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0146(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Command_Attack_Term(wk, 0xC, 0x2E, 8, -1, -1, 0x34, 0, -1, -1, 0xFFFF);
        break;

    case 1:
        SA_Term(wk, 0x2F, 0xFFFF, 0xFFFF, 0xBF);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0147(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA_Full(wk, 6, 0x76);
        break;

    case 1:
        Only_Shot(wk, 0x10);
        break;

    case 2:
        Wait(wk, 1);
        break;

    case 3:
        Only_Shot(wk, 0x10);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        Lever_On(wk, 0, 0);
        break;

    case 6:
        Wait(wk, 1);
        break;

    case 7:
        Only_Shot(wk, 0x100);
        break;

    case 8:
        Wait(wk, 1);
        break;

    case 9:
        Only_Shot(wk, 0x40);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0148(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Check_SA(wk, 6, 0x76);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x8014, 10, -1, -1, 0x20, 0, -1, -1, 0xFFFF);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0149(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x92, 0x92, 0x93, 0x94, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0150(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0x81, 0x82, 0x81, 0x82, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0151(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, 8, 0x49, 0xB, 0x202, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        Command_Attack(wk, 0xB, 0x1F, 10, -1);
        break;

    case 3:
        Wait(wk, 1);
        break;

    case 4:
        SA_Term(wk, 0x2F, 0x34, 0x34, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0152(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, -1, 0x61, 0xB, 0x202, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xB, 0x1F, 10, -1);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, 0x2F, 0x34, 0x34, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0153(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Lever_Attack(wk, 8, 0, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0154(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 1);
        break;

    case 1:
        Normal_Attack(wk, 0xD, 0x20);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0155(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 0xB, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0156(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 0xB, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xB, 0x1F, 10, -1);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, 0x2F, 0x34, 0x34, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0157(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 0xB, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0158(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 0xB, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xB, 0x1F, 10, -1);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, 0x2F, 0x34, 0x34, 0x7F);
        break;

    case 6:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0159(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 0xB, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0160(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 0xB, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
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

void Passive14_0161(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 0xB, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0162(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 0xB, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
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

void Passive14_0163(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0164(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 1, -1, -1);
        break;

    case 1:
        Wait_Get_Up(wk, 3, 0);
        break;

    case 2:
        SA_Term(wk, 0x32, 0xFFFF, 0xFFFF, 0xBF);
        break;

    case 3:
        Com_Random_Select(wk, 6, 0x71, 0x71, 0x72, 0x73, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0165(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 3, 0);
        break;

    case 2:
        SA_Term(wk, 0x32, 0xFFFF, 0xFFFF, 0xBF);
        break;

    case 3:
        Com_Random_Select(wk, 6, 0x74, 0x74, 0x75, 0x76, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0166(PLW* wk) {
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

void Passive14_0167(PLW* wk) {
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
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0168(PLW* wk) {
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
        Command_Attack(wk, 0xB, 0x1F, 10, -1);
        break;

    case 5:
        Wait(wk, 1);
        break;

    case 6:
        SA_Term(wk, 0x2F, 0x34, 0x34, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0169(PLW* wk) {
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

void Passive14_0170(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 2);
        break;

    case 1:
        Turn_Over_On(wk);
        break;

    case 2:
        Hi_Jump_Attack_Term(wk, -1, 0x61, 9, 0x202, 0, -0x7F80, -1, 0x400);
        break;

    case 3:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 4:
        SA_Term(wk, 0x32, 0xFFFF, 0xFFFF, 0xBF);
        break;

    case 5:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0171(PLW* wk) {
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

void Passive14_0172(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0173(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0174(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 2:
        SA_Term(wk, 0x32, 0xFFFF, 0xFFFF, 0xBF);
        break;

    case 3:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0175(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, -1, 0x61, 9, 0x202, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x202);
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

void Passive14_0176(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x220);
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

void Passive14_0177(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x3F, 0x40, 0x41, 0x42, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0178(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0179(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xB, 0x1F, 10, -1);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, 0x2F, 0x34, 0x34, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0180(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x220);
        break;

    case 2:
        SA_Term(wk, 0x32, 0xFFFF, 0xFFFF, 0xBF);
        break;

    case 3:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0181(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, -1, 0x61, 9, 0x202, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x202);
        break;

    case 3:
        SA_Term(wk, 0x32, -1, -1, 0xBF);
        break;

    case 4:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0182(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 9, 0x220);
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

void Passive14_0183(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, -1, 0x61, 9, 0x202, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 9, 0x202);
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

void Passive14_0184(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Command_Attack(wk, 0xB, 0x1F, 10, -1);
        break;

    case 2:
        Wait(wk, 1);
        break;

    case 3:
        SA_Term(wk, 0x2F, 0xFFFF, 0xFFFF, 0xFFFF);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0185(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        Command_Attack(wk, 0xB, 0x1F, 10, -1);
        break;

    case 3:
        Wait(wk, 1);
        break;

    case 4:
        SA_Term(wk, 0x2F, 0x34, 0x34, 0x7F);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0186(PLW* wk) {
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

void Passive14_0187(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0188(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 2:
        SA_Term(wk, 0x32, 0xFFFF, 0xFFFF, 0xBF);
        break;

    case 3:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0189(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump_Attack_Term(wk, -0x7FA8, -0x7FC8, 9, 0x400, 0, -0x7F80, -1, 0x400);
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x202);
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

void Passive14_0190(PLW* wk) {
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

void Passive14_0191(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, -1, 0x61, 0xB, 0x202, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x202);
        break;

    case 3:
        SA_Term(wk, 0x2F, 0x34, 0x34, 0x7F);
        break;

    case 4:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0192(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Turn_Over_On(wk);
        break;

    case 1:
        Hi_Jump_Attack_Term(wk, -1, 0x61, 0xB, 0x202, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Pierce_On(wk);
        break;

    case 4:
        J_Command_Attack(wk, 0xB, 0x20, 8, -1);
        break;

    case 5:
        Wait(wk, 3);
        break;

    case 6:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0193(PLW* wk) {
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

void Passive14_0194(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0xB2, 0xB3, 0xB4, 0xB6, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0195(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0xB9, 0xBB, 0xBC, 0xBD, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0196(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0xB5, 0xB7, 0xBF, 0xC0, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0197(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Approach_Walk(wk, 0x47, 2);
        break;

    case 2:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 0x7C);
        break;

    case 3:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x7F);
        break;

    case 4:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 5:
        Pierce_On(wk);
        break;

    case 6:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 7:
        Wait(wk, 3);
        break;

    case 8:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0198(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x400);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0199(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Attack_Complete(wk, 1, 1);
        break;

    case 1:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0200(PLW* wk) {
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

void Passive14_0201(PLW* wk) {
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

void Passive14_0202(PLW* wk) {
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

void Passive14_0203(PLW* wk) {
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

void Passive14_0204(PLW* wk) {
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

void Passive14_0205(PLW* wk) {
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

void Passive14_0206(PLW* wk) {
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

void Passive14_0207(PLW* wk) {
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

void Passive14_0208(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xB5, 3);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2F, 10, -1, -0x7F60, 0x50, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x4C, 0x4D, 0x4E, 0x4F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0209(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2F, 10, -1, -0x7F60, 0x50, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x50, 0x51, 0x52, 0x53, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0210(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xAD, 3);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2F, 10, -1, -0x7F60, 0x50, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x54, 0x55, 0x56, 0x57, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0211(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Jump_Command_Attack_Term(wk, 8, 0x2F, 10, -1, -0x7F60, 0x50, 0, -0x7F80, -1, 0x400);
        break;

    case 2:
        Com_Random_Select(wk, 2, 0x58, 0x59, 0x5A, 0x5B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0212(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, 0x32, 0xFFFF, 0xFFFF, 0xBF);
        break;

    case 1:
        Com_Random_Select(wk, 6, 0x71, 0x71, 0x72, 0x73, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0213(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 1);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x4C, 0x4D, 0x4E, 0x4F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0214(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 1);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x50, 0x51, 0x52, 0x53, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0215(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 1);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x54, 0x55, 0x56, 0x57, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0216(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 1);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x58, 0x59, 0x5A, 0x5B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0217(PLW* wk) {
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

void Passive14_0218(PLW* wk) {
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

void Passive14_0219(PLW* wk) {
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

void Passive14_0220(PLW* wk) {
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

void Passive14_0221(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x300, 6, 0xE1);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 10, -1);
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

void Passive14_0222(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x80, 6, 0xE1);
        break;

    case 1:
        Command_Attack(wk, 8, 0x1D, 10, -1);
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

void Passive14_0223(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x200, 6, 0xE2);
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

void Passive14_0224(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x100, 6, 0xE2);
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

void Passive14_0225(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1C, 10, -1);
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

void Passive14_0226(PLW* wk) {
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

void Passive14_0227(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1C, 10, -1);
        break;

    case 1:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x47);
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

void Passive14_0228(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0x1C, 8, -1);
        break;

    case 1:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x47);
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

void Passive14_0229(PLW* wk) {
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

void Passive14_0230(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 1);
        break;

    case 1:
        Check_SA_Full(wk, 6, 0x11);
        break;

    case 2:
        SA_Term(wk, 0x34, 0x34, 0x34, 0x7F);
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 6:
        Wait(wk, 3);
        break;

    case 7:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0231(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Command_Attack(wk, 8, 0, -1, -1);
        break;

    case 1:
        Check_SA_Full(wk, 6, 0x11);
        break;

    case 2:
        SA_Term(wk, 0x34, 0x34, 0x34, 0xBF);
        break;

    case 3:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 4:
        Pierce_On(wk);
        break;

    case 5:
        J_Command_Attack(wk, 8, 0x20, 8, -1);
        break;

    case 6:
        Wait(wk, 3);
        break;

    case 7:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0232(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0xBF, 3);
        break;

    case 1:
        Look(wk, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0233(PLW* wk) {
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

void Passive14_0234(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Attack_Complete(wk, 1, 1);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 6, 6, 1);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 8, 0x1F, 10, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0235(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F80, -1, 6, 6, 1);
        break;

    case 1:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;

    case 2:
        J_Command_Attack(wk, 8, 0x1E, 8, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0236(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F68, -1, 6, 6, 1);
        break;

    case 1:
        EM_Term(wk, -1, -0x7FF8, 4, 6, 0xEB);
        break;

    case 2:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 3:
        Command_Attack(wk, 0xC, 0x1F, 10, -1);
        break;

    case 4:
        Wait(wk, 1);
        break;

    case 5:
        SA_Term(wk, 0x2F, 0x30, 0x31, 0x7F);
        break;
    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0237(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, -0x7F80, -1, 6, 6, 1);
        break;

    case 1:
        J_Command_Attack(wk, 8, 0x1E, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0238(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 9, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 0xC, 0x202);
        break;

    case 2:
        Command_Attack(wk, 8, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0239(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Jump(wk, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0240(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, 0);
        break;

    case 1:
        Com_Random_Select(wk, 2, 0x84, 0x85, 0x86, 0x87, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0241(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x57, 3);
        break;

    case 1:
        Wait_Get_Up(wk, 3, 0);
        break;

    case 2:
        Turn_Over_On(wk);
        break;

    case 3:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 4:
        Com_Random_Select(wk, 2, 0x89, 0x8A, 0x8B, 0x8C, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0242(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 3, 0);
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

void Passive14_0243(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x26, 0x27, 0x28, 0x29, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0244(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x57, 3);
        break;

    case 1:
        Wait_Get_Up(wk, 3, 0);
        break;

    case 2:
        Turn_Over_On(wk);
        break;

    case 3:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 4:
        Com_Random_Select(wk, 2, 0x4C, 0x4D, 0x4E, 0x4F, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0245(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x57, 3);
        break;

    case 1:
        Wait_Get_Up(wk, 3, 0);
        break;

    case 2:
        Turn_Over_On(wk);
        break;

    case 3:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 4:
        Com_Random_Select(wk, 2, 0x50, 0x51, 0x52, 0x53, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0246(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x57, 3);
        break;

    case 1:
        Wait_Get_Up(wk, 3, 0);
        break;

    case 2:
        Turn_Over_On(wk);
        break;

    case 3:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 4:
        Com_Random_Select(wk, 2, 0x54, 0x55, 0x56, 0x57, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0247(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x57, 3);
        break;

    case 1:
        Wait_Get_Up(wk, 3, 0);
        break;

    case 2:
        Turn_Over_On(wk);
        break;

    case 3:
        Jump_Attack_Term(wk, -1, 0x49, 9, 0x202, 0, -0x7F80, -1, 0x40);
        break;

    case 4:
        Com_Random_Select(wk, 2, 0x58, 0x59, 0x5A, 0x5B, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0248(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 2, 0x89, 0x8A, 0x8B, 0x8C, 0);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0249(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 0xC, 0x402);
        break;

    case 1:
        Command_Attack(wk, 0xC, 0x1F, 0xA, -1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0250(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Com_Random_Select(wk, 6, 0x77, 0x77, 0x78, 0x79, 2);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive14_0251(PLW* wk) {
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

void Passive14_0252(PLW* wk) {
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
