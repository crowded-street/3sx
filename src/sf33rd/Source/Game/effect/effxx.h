#ifndef EFFXX_H
#define EFFXX_H

#include "structs.h"

typedef void (*EffectMoveCallback)(WORK_Other*);
typedef s32 (*EffectInitCallback)(WORK*, u8);

extern EffectMoveCallback const effmovejptbl[];
extern EffectInitCallback const effinitjptbl[];

#endif
