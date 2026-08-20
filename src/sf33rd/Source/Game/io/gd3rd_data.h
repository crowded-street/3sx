#ifndef GD3RD_DATA_H
#define GD3RD_DATA_H

#include "types.h"

typedef struct {
    u8 type;
    u8 ix;
    u8 frre;
    u8 kokey;
} LDREQ_TBL;

typedef struct Span {
    s16 start;
    s16 length;
} Span;

extern const LDREQ_TBL ldreq_tbl[];
extern const Span spans[];

#endif
