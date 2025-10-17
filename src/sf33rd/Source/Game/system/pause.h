#ifndef PAUSE_H
#define PAUSE_H

#include "structs.h"
#include "types.h"

void dispControllerWasRemovedMessage(s32 x, s32 y, s32 step);

extern void Pause_Task(struct _TASK*);

#endif
