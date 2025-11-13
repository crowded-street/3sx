#ifndef STUN_H
#define STUN_H

#include "types.h"

typedef struct {
    s16 cstn;
    s8 sflag;
    s8 osflag;
    s8 g_or_s;
    s8 stimer;
    s16 slen;
    s8 proccess_dead;
} SDAT;

// MARK: - Serialized

extern SDAT sdat[2];

void stngauge_cont_init();
void stngauge_cont_main();
void stngauge_control(u8 pl);
void stngauge_work_clear();

#endif
