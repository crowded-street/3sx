#ifndef VITAL_H
#define VITAL_H

#include "structs.h"
#include "types.h"

typedef struct {
    s16 cyerw;
    s16 cred;
    s16 ored;
    s8 colnum;
} VIT;

// MARK: - Serialized

extern VIT vit[2];

void vital_cont_init();
void vital_cont_main();
void vital_control(u8 pl);
void vital_parts_allwrite(u8 Pl_Num);

#endif
