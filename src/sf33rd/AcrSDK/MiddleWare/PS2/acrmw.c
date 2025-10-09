#include "sf33rd/AcrSDK/MiddleWare/PS2/acrmw.h"
#include "common.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/ADX/flADX.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/cse.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#include "port/sdl/sdl_app.h"

#include <cri_mw.h>
#include <eeregs.h>
#include <libgraph.h>

void flmwVSyncCallback() {
    flAdxControll(0);
}

static void flmwFlipSub() {
    if (flPS2FlipCancelFlag > 0) {
        flPS2FlipCancelFlag -= 1;
        return;
    }

    flPs2State.Db_change_enable = 1;
}

void flmwFlip(u32 /* unused */) {
    ADXM_ExecMain();
    cseExecServer();
}
