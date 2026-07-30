#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "common.h"
#include "port/utils.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/cse.h"
#include "sf33rd/AcrSDK/common/fbms.h"
#include "sf33rd/AcrSDK/common/memfound.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/common/prilay.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/AcrSDK/ps2/ps2PAD.h"
#include "structs.h"

#include <SDL3/SDL.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FLPS2State flPs2State;
FLTexture flTexture[FL_TEXTURE_MAX];
FLTexture flPalette[FL_PALETTE_MAX];
s32 flWidth;
s32 flHeight;
u32 flSystemRenderOperation;
FL_FMS flFMS;
s32 flVramStaticNum;

// forward decls
static s32 system_work_init();
static void flPS2InitRenderBuff();

s32 flInitialize() {
    if (system_work_init() == 0) {
        return 0;
    }

    flPS2SystemTmpBuffInit();
    flPS2InitRenderBuff();
    flPADInitialize();

    return 1;
}

static s32 system_work_init() {
    void* temp;

    SDL_zero(flPs2State);
    temp = malloc(0x1800000);

    if (temp == NULL) {
        return 0;
    }

    fmsInitialize(&flFMS, temp, 0x1800000, 0x40);
    const int system_memory_size = 0xA00000;
    temp = flAllocMemoryS(system_memory_size);
    mflInit(temp, system_memory_size, 0x40);

    return 1;
}

s32 flFlip(u32 flag) {
    flPS2SystemTmpBuffFlush();
    cseExecServer(); // FIXME: This shouldn't be called from multiple places
    return 1;
}

static void flPS2InitRenderBuff() {
    s32 width;
    s32 height;
    s32 disp_height;

    width = 512;
    height = 448;
    disp_height = 448;
    flWidth = width;
    flHeight = height;
    flPs2State.DispWidth = width;
    flPs2State.DispHeight = disp_height;
    flPs2State.ZBuffMax = (f32)65535;
}

s32 flLogOut(const char* format, ...) {
    char str[2048];

    va_list args;
    va_start(args, format);
    vsnprintf(str, sizeof(str), format, args);
    va_end(args);

    fatal_error(str);
}
