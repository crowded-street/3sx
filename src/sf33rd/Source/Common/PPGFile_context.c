/**
 * @file PPGFile_context.c
 * Reading a PPG or PPL file header into a plContext.
 */

#include "sf33rd/Source/Common/PPGFile.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/plcommon.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "sf33rd/Source/Compress/Lz77/Lz77Dec.h"
#include "sf33rd/Source/Compress/zlibApp.h"
#include "structs.h"
#include "core/renderer.h"
#include <SDL3/SDL.h>
#include "sf33rd/Source/Common/PPGFile_internal.h"


void ppgSetupContextFromPPL(PPLFileHeader* ppl, plContext* bits) {
    bits->desc = 0;
    bits->width = pplColorModeWidth[ppl->c_mode & 3] < 17 ? 16 : 256;
    bits->height = 1;
    bits->bitdepth = ppl->formARGB != 0x8888 ? 2 : 4;
    bits->pitch = bits->width * bits->bitdepth;
    bits->ptr = NULL;

    switch (SDL_Swap16BE(ppl->formARGB)) {
    case 0x1555:
        bits->pixelformat.rl = 5;
        bits->pixelformat.rs = 0xA;
        bits->pixelformat.rm = 0x1F;
        bits->pixelformat.gl = 5;
        bits->pixelformat.gs = 5;
        bits->pixelformat.gm = 0x1F;
        bits->pixelformat.bl = 5;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0x1F;
        bits->pixelformat.al = 1;
        bits->pixelformat.as = 0xF;
        bits->pixelformat.am = 1;
        break;

    case 0x565:
        bits->pixelformat.rl = 5;
        bits->pixelformat.rs = 0xB;
        bits->pixelformat.rm = 0x1F;
        bits->pixelformat.gl = 6;
        bits->pixelformat.gs = 5;
        bits->pixelformat.gm = 0x3F;
        bits->pixelformat.bl = 5;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0x1F;
        bits->pixelformat.al = 0;
        bits->pixelformat.as = 0;
        bits->pixelformat.am = 0;
        break;

    case 0x4444:
        bits->pixelformat.rl = 4;
        bits->pixelformat.rs = 8;
        bits->pixelformat.rm = 0xF;
        bits->pixelformat.gl = 4;
        bits->pixelformat.gs = 4;
        bits->pixelformat.gm = 0xF;
        bits->pixelformat.bl = 4;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0xF;
        bits->pixelformat.al = 4;
        bits->pixelformat.as = 0xC;
        bits->pixelformat.am = 0xF;
        break;

    default:
        bits->pixelformat.rl = 8;
        bits->pixelformat.rs = 0x10;
        bits->pixelformat.rm = 0xFF;
        bits->pixelformat.gl = 8;
        bits->pixelformat.gs = 8;
        bits->pixelformat.gm = 0xFF;
        bits->pixelformat.bl = 8;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0xFF;
        bits->pixelformat.al = 8;
        bits->pixelformat.as = 0x18;
        bits->pixelformat.am = 0xFF;
        break;
    }
}

void ppgSetupContextFromPPG(PPGFileHeader* ppg, plContext* bits) {
    bits->desc = 0;
    bits->width = ppg->width * 16;
    bits->height = ppg->height * 16;

    switch (ppg->pixel & 3) {
    case 0:
        if (ppg->pixel & 0x20) {
            bits->desc |= 0x24;
        } else {
            bits->desc |= 0x14;
        }

        bits->bitdepth = 0;
        bits->pitch = bits->width / 2;
        break;

    case 1:
        bits->desc = bits->desc | 4;
        bits->bitdepth = 1;
        bits->pitch = bits->width;
        break;

    case 2:
        bits->bitdepth = 2;
        bits->pitch = bits->width * 2;
        break;

    default:
        bits->bitdepth = 4;
        bits->pitch = bits->width * 4;
        break;
    }

    switch (SDL_Swap16BE(ppg->formARGB)) {
    case 0x1555:
        bits->pixelformat.rl = 5;
        bits->pixelformat.rs = 0xA;
        bits->pixelformat.rm = 0x1F;
        bits->pixelformat.gl = 5;
        bits->pixelformat.gs = 5;
        bits->pixelformat.gm = 0x1F;
        bits->pixelformat.bl = 5;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0x1F;
        bits->pixelformat.al = 1;
        bits->pixelformat.as = 0xF;
        bits->pixelformat.am = 1;
        break;

    case 0x565:
        bits->pixelformat.rl = 5;
        bits->pixelformat.rs = 0xB;
        bits->pixelformat.rm = 0x1F;
        bits->pixelformat.gl = 6;
        bits->pixelformat.gs = 5;
        bits->pixelformat.gm = 0x3F;
        bits->pixelformat.bl = 5;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0x1F;
        bits->pixelformat.al = 0;
        bits->pixelformat.as = 0;
        bits->pixelformat.am = 0;
        break;

    case 0x4444:
        bits->pixelformat.rl = 4;
        bits->pixelformat.rs = 8;
        bits->pixelformat.rm = 0xF;
        bits->pixelformat.gl = 4;
        bits->pixelformat.gs = 4;
        bits->pixelformat.gm = 0xF;
        bits->pixelformat.bl = 4;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0xF;
        bits->pixelformat.al = 4;
        bits->pixelformat.as = 0xC;
        bits->pixelformat.am = 0xF;
        break;

    case 0x8888:
        bits->pixelformat.rl = 8;
        bits->pixelformat.rs = 0x10;
        bits->pixelformat.rm = 0xFF;
        bits->pixelformat.gl = 8;
        bits->pixelformat.gs = 8;
        bits->pixelformat.gm = 0xFF;
        bits->pixelformat.bl = 8;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0xFF;
        bits->pixelformat.al = 8;
        bits->pixelformat.as = 0x18;
        bits->pixelformat.am = 0xFF;
        break;

    default:
        bits->pixelformat.rl = 0;
        bits->pixelformat.rs = 0;
        bits->pixelformat.rm = 0;
        bits->pixelformat.gl = 0;
        bits->pixelformat.gs = 0;
        bits->pixelformat.gm = 0;
        bits->pixelformat.bl = 0;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0;
        bits->pixelformat.al = 0;
        bits->pixelformat.as = 0;
        bits->pixelformat.am = 0;
        break;
    }
}
