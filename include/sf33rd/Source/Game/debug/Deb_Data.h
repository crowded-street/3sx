#ifndef DEB_DATA_H
#define DEB_DATA_H

#include "types.h"

typedef struct {
    u8 max;  
    s8* name;
} DEBUG_STR_DAT;

extern u8* debug_name_data[5];
extern const DEBUG_STR_DAT debug_string_data[72];

#endif
