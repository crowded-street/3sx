#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/memfound.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/ps2/flps2asm.h"
#include "sf33rd/AcrSDK/ps2/flps2dma.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2ps.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/flps2shader.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "structs.h"

#include <eestruct.h>
#include <libgraph.h>

#include <stdio.h>
#include <string.h>

#if defined(TARGET_PS2)
#include "mw_stdarg.h"
#else
#include <stdarg.h>
#endif

void flPS2DebugStrClear();
static void flPS2DrawProbar();
void flPS2LoadCheckFlush();

s32 flSetDebugMode(u32 flag) {
    flDebugFlag = flag;
    return 1;
}

f64 flGetSystemTime() {
    f32 sys_time = DGET_T0_COUNT();
    sys_time = ((1000.0f * sys_time) / 60.0f) / 240.0f;
    return sys_time;
}

void flPS2DebugInit() {
    s32 len;
    s32 i;

    flSetDebugMode(3);
    flDebugSysMemHandleNum = 0;
    flDebugSysMemEtc = 0;
    flDebugSysMemTexture = 0;
    flDebugSysMemClay = 0;
    flDebugSysMemMotion = 0;

    for (i = 0; i < DEBUG_SYS_MEM_SIZE; i++) {
        flDebugSysMem[i] = 0;
    }

    len = 0xE100;
    flDebugStrHan = flPS2GetSystemMemoryHandle(len, 1);
    flhDebugStr = flCreateTextureFromFile("IOP\\FONT8_8.TM2", 4);

    if (flhDebugStr == 0) {
        while (1) {}
    }

    flPrintColor(-1);
    flPS2DebugStrClear();
}

void flPS2DebugStrClear() {
    flDebugStrX = 0;
    flDebugStrY = 0;
    flDebugStrCtr = 0;
}

void flPS2DebugStrDisp() {
    RenderBuffer* buff_ptr;
    u64* work_ptr;
    u64* keep_ptr;
    u64* buff_end_ptr;
    u64* giftag_keep_ptr;
    u32 code;
    u32 disp_ctr;
    ptrdiff_t length;
    s32 lp0;
    u32 col;
    u32 colold;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 u1;
    s16 v1;
    s16 u2;
    s16 v2;
    u64 texA;
    u64 tex1;
    u64 miptbp1;
    u64 miptbp2;
    u64 rgbaq;

    buff_ptr = flPS2GetSystemBuffAdrs(flDebugStrHan);
    colold = 0;
    flReloadTexture(1, &flhDebugStr);
    flPS2SendRenderState_TEX1(0x10000, 1);
    work_ptr = (u64*)((flPs2State.SystemTmpBuffNow + 0xF) & ~0xF);
    keep_ptr = work_ptr;
    buff_end_ptr = (u64*)flPs2State.SystemTmpBuffEndAdrs;
    length = buff_end_ptr - work_ptr;

    if (length >= 16) {
        work_ptr += 2;
        *work_ptr++ = SCE_GIF_SET_TAG(6, 1, 0, 0, SCE_GIF_PACKED, 1);
        *work_ptr++ = SCE_GIF_PACKED_AD;
        *work_ptr++ = SCE_GS_SET_TEX1_2(0, 0, 0, 0, 0, 0, 0);
        *work_ptr++ = SCE_GS_TEX1_2;
        *work_ptr++ = SCE_GS_SET_TEST_2(1, 7, 0, 0, 0, 0, 1, 1);
        *work_ptr++ = SCE_GS_TEST_2;
        *work_ptr++ = SCE_GS_SET_SCISSOR_2(0, flWidth - 1, 0, flHeight - 1);
        *work_ptr++ = SCE_GS_SCISSOR_2;

        flPS2SetTextureRegister(flhDebugStr, &texA, &tex1, work_ptr, work_ptr + 2, &miptbp1, &miptbp2, 0x20000);

        work_ptr[1] = SCE_GS_TEX0_2;
        work_ptr[3] = SCE_GS_CLAMP_2;
        work_ptr += 4;
        *work_ptr++ = SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 0, 0, 1, 1, 0);
        *work_ptr++ = SCE_GS_PRIM;
        giftag_keep_ptr = work_ptr;
        disp_ctr = 0;
        work_ptr += 2;
        length = buff_end_ptr - work_ptr;

        for (lp0 = 0; lp0 < flDebugStrCtr; lp0++) {
            length -= 6;

            if (length < 0) {
                break;
            }

            disp_ctr += 1;
            code = buff_ptr->code;
            col = buff_ptr->col;
            buff_ptr->code = 0;

            if (colold != col) {
                rgbaq = SCE_GS_SET_RGBAQ((col >> 16) & 0xFF, (col >> 8) & 0xFF, col & 0xFF, (col >> 0x18) & 0xFF, 1);
                colold = col;
            }

            x1 = buff_ptr->x;
            y1 = buff_ptr->y;
            x2 = x1 + 8;
            y2 = y1 + 8;

            if (flPs2State.InterlaceMode == 0) {
                y1 >>= 1;
                y2 >>= 1;
            }

            u1 = (code % 16) * 8;
            v1 = (code / 16) * 8;
            u2 = u1 + 8;
            v2 = v1 + 8;

            *work_ptr++ = rgbaq;
            *work_ptr++ = SCE_GS_SET_UV(u1 * 16, v1 * 16);
            *work_ptr++ = SCE_GS_SET_XYZ2(
                (flPs2State.D2dOffsetX + x1) * 16, (flPs2State.D2dOffsetY + y1) * 16, (u32)flPs2State.ZBuffMax);
            *work_ptr++ = SCE_GS_SET_UV(u2 * 16, v2 * 16);
            *work_ptr++ = SCE_GS_SET_XYZ2(
                (flPs2State.D2dOffsetX + x2) * 16, (flPs2State.D2dOffsetY + y2) * 16, (u32)flPs2State.ZBuffMax);

            *work_ptr++ = 0;
            buff_ptr++;
        }

        flPs2State.SystemTmpBuffNow = (uintptr_t)work_ptr;

        if (disp_ctr != 0) {
            *giftag_keep_ptr++ = SCE_GIF_SET_TAG(disp_ctr, 1, 0, 0, SCE_GIF_REGLIST, 6);
            *giftag_keep_ptr++ =
                SCE_GS_RGBAQ | SCE_GS_UV << 4 | SCE_GS_XYZ2 << 8 | SCE_GS_UV << 12 | SCE_GS_XYZ2 << 16 | 0xE << 20;
        } else {
            work_ptr = giftag_keep_ptr;
        }

        length = (work_ptr - keep_ptr) >> 1;
        work_ptr = keep_ptr;

        *work_ptr = length + 0x6FFFFFFF;
        ((u32*)work_ptr)[0] |= 0x80000000;
        ((u32*)work_ptr)[2] = 0x13000000;
        ((u32*)work_ptr)[3] = (u32)((length - 1) | 0x51000000);
        flPS2DmaAddQueue2(0, ((uintptr_t)keep_ptr & 0xFFFFFFF) | 0x40000000, (uintptr_t)keep_ptr, &flPs2VIF1Control);
    }

    flPS2DebugStrClear();
}

s32 flPrintL(s32 posi_x, s32 posi_y, const s8* format, ...) {
    s8* va_ptr;
    s8 code;
    s8 str[512];
    strlen_t len;
    s32 i;
    RenderBuffer* buff_ptr;

    va_list args;

    buff_ptr = flPS2GetSystemBuffAdrs(flDebugStrHan);
    buff_ptr += flDebugStrCtr;

    va_start(args, format);
    vsprintf(str, format, args);
    len = strlen(str);

    if (flDebugStrCtr + len >= 0x12C0) {
        len = 0x12C0 - flDebugStrCtr;
    }

    for (i = 0; i < len; i++) {
        code = str[i];

        if ((code >= 0x10) && (code < 0x80) && (code != ' ')) {
            buff_ptr->x = posi_x * 8;
            buff_ptr->y = posi_y * 8;
            buff_ptr->code = code;
            buff_ptr->col = flDebugStrCol;
            buff_ptr++;
            flDebugStrCtr += 1;
        }

        posi_x += 1;
    }

    return 1;
}

s32 flPrintColor(u32 color) {
    u8 r = (color >> 16) & 0xFF;
    u8 g = (color >> 8) & 0xFF;
    u8 b = (color) & 0xFF;
    u8 a = (color >> 24) & 0xFF;

    r >>= 1;
    g >>= 1;
    b >>= 1;

    if (a == 0xFF) {
        a = 0x80;
    } else {
        a >>= 1;
    }

    flDebugStrCol = (a << 24) | (r << 16) | (g << 8) | b;
    return 1;
}

void flPS2DispSystemInfo(s32 x, s32 y) {
    u32 max_size;
    u32 now_size;
    s32 i;

    if (flPs2State.InterlaceMode == 2) {
        for (i = 0; i < DEBUG_TRUE_TIME_SIZE; i++) {
            flDebugTrueTime[i] >>= 1;
            flDebugTrueTimeFree[i] >>= 1;
        }

        flDebugEndRenderTime >>= 1;

        while ((u32)i < flLoadCheckCtr && i < LOAD_CHECK_TIME_SIZE) {
            flLoadCheckTime[i] >>= 1;
            i += 1;
        }
    }

    if (flDebugFlag & 1) {
        flPrintColor(0xFFFFFFFF);
        flPrintL(x, y++, "USR MEM = %fMB", (flGetSpace() / 1024.0f) / 1024.0f);
        flPrintL(x,
                 y++,
                 "SYS MEM = %fMB / %fMB / %08x",
                 (mflGetFreeSpace() / 1024.0f) / 1024.0f,
                 (mflGetSpace() / 1024.0f) / 1024.0f,
                 flDebugSysMemHandleNum);
        flPrintL(x,
                 y++,
                 "0x%08x, 0x%08x, 0x%08x, 0x%08x",
                 flDebugSysMemEtc,
                 flDebugSysMemTexture,
                 flDebugSysMemClay,
                 flDebugSysMemMotion);
        max_size = 0x4000 - flPs2State.TextureStartAdrs;
        now_size = flPS2GetVramSize();
        flPrintL(x,
                 y++,
                 "VRAM = %d / %d, DMA = %d/%d, FPS = %d",
                 now_size,
                 max_size,
                 flDebugDINum,
                 flDebugAQNum,
                 flPs2State.FrameCount);
        flPrintL(x, y++, "TH = %d, %d", flCTNum, flVramNum);
        flPrintL(x, y++, "PH = %d", flPTNum);
        flPrintL(x, y++, "CLAY = %d", flClayNum);
        flPrintL(x, y++, "EC = %d, %d", flDebugECNum, flDebugErrECNum);
        flPrintL(x, y++, "RT = %d", flDebugRTNum);
        flPrintL(x, y++, "UT = %d", flDebugUTNum);
        flPrintL(x, y++, "MC = %d", flDebugMCNum);
        flPrintL(x, y++, "VERT = %d", flDebugVERTNum);
        flPrintL(x, y++, "POLY = %d", flDebugPOLYNum);
        flPrintL(x, y++, "MATERIAL = %d", flDebugMATERIALNum);

        if (flPs2State.NowVu1Code == -1) {
            flPrintL(x, y++, "MICRO CODE = %d SIZE = %d", flPs2State.NowVu1Code, flPs2State.NowVu1Size);
        } else {
            flPrintL(x, y++, "MICRO CODE = %s SIZE = %d", vu1_debug_str[flPs2State.NowVu1Code], flPs2State.NowVu1Size);
        }

        flPrintL(x,
                 y++,
                 "TIME = %d, %d, %d, %d, %d",
                 flDebugTrueTime[0],
                 flDebugTrueTime[1],
                 flDebugTrueTime[2],
                 flDebugTrueTime[3],
                 flDebugEndRenderTime);
        flPrintL(x,
                 y++,
                 "FREE TIME = %d, %d, %d, %d",
                 flDebugTrueTimeFree[0],
                 flDebugTrueTimeFree[1],
                 flDebugTrueTimeFree[2],
                 flDebugTrueTimeFree[3]);
        flPrintL(x, y++, "RENDER SIZE = 0x%08x", flPs2State.SystemTmpBuffNow - flPs2State.SystemTmpBuffStartAdrs);

        flPrintColor(0xFFFFFFFF);
        i = 0;
        y += 1;

        while (i < (u32)flLoadCheckCtr && i < LOAD_CHECK_TIME_SIZE) {
            flPrintL(x + ((i >> 4) * 10), y + (i & 0xF), "%04d", flLoadCheckTime[i]);
            i += 1;
        }
    }

    if (flDebugFlag & 2) {
        flPS2DrawProbar();
    }

    flDebugECNum = 0;
    flDebugErrECNum = 0;
    flDebugUTNum = 0;
    flDebugRTNum = 0;
    flDebugMCNum = 0;
    flDebugVERTNum = 0;
    flDebugPOLYNum = 0;
    flDebugMATERIALNum = 0;
    flDebugAQNum = 0;
    flDebugDINum = 0;
    flDebugTrueTimeFree[0] = 0;
    flDebugTrueTimeFree[1] = 0;
    flDebugTrueTimeFree[2] = 0;
    flDebugTrueTimeFree[3] = 0;
    flPS2LoadCheckFlush();
}

static void flPS2DrawProbar() {
    u32 work;
    s16 x;
    s16 y;
    u64* data_ptr;
    u32 i;
    u32 keep_y;
    u32 col;

    static u64 LoadProbar_data[8] = { 0x70000003,
                                      0x0,
                                      SCE_GIF_SET_TAG(1, 1, 0, 0, SCE_GIF_REGLIST, 4),
                                      SCE_GS_PRIM | SCE_GS_RGBAQ << 4 | SCE_GS_XYZ2 << 8 | SCE_GS_XYZ2 << 12,
                                      SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 2, 0, 0, 2, 0, 0),
                                      0x0,
                                      0x0,
                                      0x0 };

    static u64 DrawProbar_data[17] = { 0x70000008,
                                       0x0,
                                       SCE_GIF_SET_TAG(2, 1, 0, 0, SCE_GIF_PACKED, 1),
                                       SCE_GIF_PACKED_AD,
                                       0x3000F,
                                       SCE_GS_TEST_2,
                                       0x0,
                                       SCE_GS_SCISSOR_2,
                                       SCE_GIF_SET_TAG(1, 1, 0, 0, SCE_GIF_REGLIST, 7),
                                       SCE_GS_PRIM | SCE_GS_RGBAQ << 4 | SCE_GS_XYZ2 << 8 | SCE_GS_XYZ2 << 12 |
                                           SCE_GS_RGBAQ << 16 | SCE_GS_XYZ2 << 20 | SCE_GS_XYZ2 << 24,
                                       SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 2, 0, 0, 2, 0, 0),
                                       SCE_GS_SET_RGBAQ(0xFF, 0, 0, 0xFF, 1),
                                       0x0,
                                       0x0,
                                       SCE_GS_SET_RGBAQ(0, 0, 0xFF, 0xFF, 1),
                                       0x0,
                                       0x0 };

    data_ptr = (u64*)SPR;
    flPS2_Mem_move16_16A(&DrawProbar_data, (void*)SPR, 9);
    *(data_ptr + 6) = SCE_GS_SET_SCISSOR_2(0, flWidth - 1, 0, flHeight - 1);
    work = (flDebugTrueTime[3] * 2) / (flPs2State.FrameCount + 1);
    x = flPS2ConvScreenX(0);
    y = flPS2ConvScreenY(0);
    *(data_ptr + 12) =
        SCE_GS_SET_XYZ2((flPs2State.D2dOffsetX + x) * 16, (flPs2State.D2dOffsetY + y) * 16, (u32)flPs2State.ZBuffMax);
    x = flPS2ConvScreenX(8);
    y = flPS2ConvScreenY(work);
    *(data_ptr + 13) =
        SCE_GS_SET_XYZ2((flPs2State.D2dOffsetX + x) * 16, (flPs2State.D2dOffsetY + y) * 16, (u32)flPs2State.ZBuffMax);
    work = (flDebugEndRenderTime * 2) / (flPs2State.FrameCount + 1);
    x = flPS2ConvScreenX(8);
    y = flPS2ConvScreenY(0);
    *(data_ptr + 15) =
        SCE_GS_SET_XYZ2((flPs2State.D2dOffsetX + x) * 16, (flPs2State.D2dOffsetY + y) * 16, (u32)flPs2State.ZBuffMax);
    x = flPS2ConvScreenX(16);
    y = flPS2ConvScreenY(work);
    *(data_ptr + 16) =
        SCE_GS_SET_XYZ2((flPs2State.D2dOffsetX + x) * 16, (flPs2State.D2dOffsetY + y) * 16, (u32)flPs2State.ZBuffMax);
    flPS2psAddQueue((QWORD*)data_ptr);

    if (!flLoadCheckCtr) {
        return;
    }

    flPS2_Mem_move16_16A(&LoadProbar_data, (void*)SPR, 4);
    i = 0;
    keep_y = 0;

    do {
        col = flPS2ConvColor(flLoadCheckColor[i], 1);
        *(data_ptr + 5) = SCE_GS_SET_RGBAQ((col >> 16) & 0xFF, (col >> 8) & 0xFF, col & 0xFF, (col >> 0x18) & 0xFF, 1);
        x = flPS2ConvScreenX(16);
        y = flPS2ConvScreenY(keep_y);
        *(data_ptr + 6) = SCE_GS_SET_XYZ2(
            (flPs2State.D2dOffsetX + x) * 16, (flPs2State.D2dOffsetY + y) * 16, (u32)flPs2State.ZBuffMax);
        x = flPS2ConvScreenX(24);
        keep_y += (flLoadCheckTime[i] * 2) / (flPs2State.FrameCount + 1);
        y = flPS2ConvScreenY(keep_y);
        *(data_ptr + 7) = SCE_GS_SET_XYZ2(
            (flPs2State.D2dOffsetX + x) * 16, (flPs2State.D2dOffsetY + y) * 16, (u32)flPs2State.ZBuffMax);
        flPS2psAddQueue((QWORD*)data_ptr);
        i += 1;
    } while (i < (u32)flLoadCheckCtr);
}

void flPS2LoadCheckFlush() {
    s32 i;
    flLoadCheckCtr = 0;
    flLoadCheckTimeOld = 0;

    for (i = 0; i < LOAD_CHECK_TIME_SIZE; i++) {
        flLoadCheckTime[i] = 0;
    }
}

void flPS2SystemError(s32 error_level, s8* format, ...) {
    va_list args;
    s8 str[512];
    strlen_t len;

    flFlip(0);
    va_start(args, format);
    vsprintf(str, format, args);
    len = strlen(str);

    while (1) {
        flPrintL(10, 20, "%s", str);

        if (error_level == 0) {
            flSetRenderState(FLRENDER_BACKCOLOR, 0xFF0000);
        } else {
            flSetRenderState(FLRENDER_BACKCOLOR, 0xFF);
            flPrintL(10, 40, "PRESS 1P START BUTTON TO EXIT");

            if (flpad_adr[0][0].sw_new & 0x8000) {
                break;
            }
        }

        flFlip(0);
        flPADGetALL();
    }
}
