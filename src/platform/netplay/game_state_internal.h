/*
 * Shared between game_state.c and game_state_load.c.
 *
 * The rollback state is saved in one file and restored in the other; both need
 * the same globals in scope and the same size assertion, so the includes and
 * GS_ASSERT_SAME_SIZE live here. Nothing else moves, and no global is widened.
 */

#ifndef GAME_STATE_INTERNAL_H
#define GAME_STATE_INTERNAL_H

#include "platform/netplay/game_state.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/animation/win_pl.h"
#include "sf33rd/Source/Game/effect/eff56.h"
#include "sf33rd/Source/Game/effect/eff79.h"
#include "sf33rd/Source/Game/effect/effb2.h"
#include "sf33rd/Source/Game/effect/effb8.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/spgauge.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/count.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

#include <SDL3/SDL.h>

// Copies are sized by the global, so a mismatched field would spill into its neighbours.
#define GS_ASSERT_SAME_SIZE(member)                                                                                    \
    _Static_assert(sizeof(((GameState*)0)->member) == sizeof(member), #member " does not match its global")

#endif
