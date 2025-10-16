/**
 * @file follow02.c
 * COM Follow
 */

#include "sf33rd/Source/Game/com/follow/follow02.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Follow02_Tbl[4])(PLW*);

void Follow02(PLW* wk) {
    Follow02_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Follow02_0000(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Follow02_0001(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 1:
        Command_Attack(wk, 2, 8, 0x1C, 8);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Follow02_0002(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 1:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Follow02_0003(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 1:
        Command_Attack(wk, 2, 8, 0x1C, 8);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void (*const Follow02_Tbl[4])(PLW*) = {
    Follow02_0000,
    Follow02_0001,
    Follow02_0002,
    Follow02_0003,
};
