#include "common.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/cse.h"

#include "port/sdl/sdl_app.h"

void flmwFlip(u32 /* unused */) {
    cseExecServer(); // FIXME: This shouldn't be called from multiple places
}
