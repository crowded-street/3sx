#ifndef SLOWF_H
#define SLOWF_H

#include "types.h"

extern s16 SLOW_timer;
extern s16 SLOW_flag;
extern s16 EXE_flag;

void init_slow_flag();
void set_conclusion_slow();
void set_EXE_flag();

#endif // SLOWF_H
