#ifndef GD3RD_DATA_H
#define GD3RD_DATA_H

#include "sf33rd/Source/Game/io/gd3rd.h"
#include "types.h"

typedef struct LoadRequestEntry {
    LoadRequestType type;
    u8 ix;
    u8 frre;
    u8 kokey;
} LoadRequestEntry;

typedef struct Span {
    s16 start;
    s16 length;
} Span;

extern const LoadRequestEntry ldreq_tbl[];
extern const Span spans[];

#endif
