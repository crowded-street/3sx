#ifndef NETPLAY_BASE_H
#define NETPLAY_BASE_H

#if NETPLAY_ENABLED

#include "platform/netplay/game_state.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "types.h"

#define PLAYER_COUNT 2

typedef struct EffectState {
    s16 frwctr;
    s16 frwctr_min;
    s16 head_ix[8];
    s16 tail_ix[8];
    s16 exec_tm[8];
    uintptr_t frw[EFFECT_MAX][448];
    s16 frwque[EFFECT_MAX];
} EffectState;

typedef struct State {
    GameState gs;
    EffectState es;
} State;

#if DEBUG
#define STATE_BUFFER_MAX 20
#endif

void NetplayBase_SetupVsMode();
void NetplayBase_StartStressSession();

#if DEBUG
void NetplayBase_DumpDesyncPair(int frame);
#endif

#endif // NETPLAY_ENABLED

#endif
