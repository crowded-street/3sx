#ifndef RESET_H
#define RESET_H

#include "structs.h"
#include "types.h"

extern u8 Reset_Status[2];

void Reset_Task(struct _TASK* task_ptr);
u8 nowSoftReset();

#endif
