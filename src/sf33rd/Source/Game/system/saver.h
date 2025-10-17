#ifndef SAVER_H
#define SAVER_H

#include "structs.h"

void Saver_Task(struct _TASK* task_ptr);
void Saver_Init(struct _TASK* task_ptr);
void Saver_Check(struct _TASK* task_ptr);
void Saver_Move(struct _TASK* task_ptr);
void Saver_Exit(struct _TASK* task_ptr);

#endif
