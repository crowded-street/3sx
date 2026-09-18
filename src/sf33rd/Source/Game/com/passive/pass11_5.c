/**
 * @file pass11_5.c
 * COM Passive: Ken
 * Pattern scripts from Passive11_0253 on.
 */

#include "sf33rd/Source/Game/com/passive/pass11.h"
#include "sf33rd/Source/Game/com/passive/pass11_internal.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive11_0253(PLW* wk) {
    passive11_pattern_normal_attack(wk, 8, 0x200);
}

void Passive11_0254(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 1:
        Branch_Unit_Area(wk, &(Branch_Menu_Args){6, 0xEF, 0xF7, 0xFB, 0xEC});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}
