#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "common.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/cse.h"
#include "sf33rd/AcrSDK/common/fbms.h"
#include "sf33rd/AcrSDK/common/memfound.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/common/prilay.h"
#include "sf33rd/AcrSDK/ps2/flps2asm.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/AcrSDK/ps2/ps2PAD.h"
#include "structs.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FLPS2State flPs2State;
FLTexture flTexture[256];
FLTexture flPalette[1088];
s32 flWidth;
s32 flHeight;
u32 flSystemRenderOperation;
FL_FMS flFMS;
s32 flVramStaticNum;
u32 flDebugStrHan;
u32 flDebugStrCol;
u32 flDebugStrCtr;

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

    flMemset(&flPs2State, 0, sizeof(FLPS2State));
    temp = malloc(0x01800000);

    if (temp == NULL) {
        return 0;
    }

    fmsInitialize(&flFMS, temp, 0x01800000, 0x40);
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

s32 flLogOut(s8* format, ...) {
    s8 str[2048];
    s8* lp;
    static s32 bflLogOutFirst = 1;

    va_list args;
    va_start(args, format);

    vsprintf(str, format, args);
    lp = strlen(str) + str;
    *(lp++) = '\r';
    *(lp++) = '\n';
    *lp = '\0';

    if (bflLogOutFirst != 0) {
        flFileWrite("../acrout.txt", "Debug Message Output for PS2\r\n", strlen("Debug Message Output for PS2\r\n"));
        bflLogOutFirst = 0;
    }

    flFileAppend("../acrout.txt", str, strlen(str));
    va_end(args);

    fatal_error(str);
    return 1;
}
