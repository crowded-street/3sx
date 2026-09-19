/**
 * @file pass11_5.c
 * COM Passive: Ken
 * Pattern scripts from Passive11_0253 on.
 */

#include "sf33rd/Source/Game/com/passive/pass11.h"
#include "sf33rd/Source/Game/com/passive/pass11_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void Passive11_0253(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x200);
}

void Passive11_0254(PLW* wk) {
    pattern_wait_get_up_branch_unit_area(wk, 0, &(Branch_Menu_Args){6, 0xEF, 0xF7, 0xFB, 0xEC});
}
