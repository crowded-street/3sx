#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/AcrSDK/ps2/flps2dma.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2math.h"
#include "sf33rd/AcrSDK/ps2/flps2ps.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#if !defined(TARGET_PS2)
#include "port/sdl/sdl_game_renderer.h"
#endif

void flPS2SetClearColor(u32 col);
s32 flPS2SendTextureRegister(u32 th);

s32 flBeginRender() {
    flDebugTrueTime[0] = DGET_T0_COUNT();
    flLoadNow = flGetSystemTime();

    if (flLoadCount-- == 0) {
        flLoadCount = 100;
        flLoadReserve = (f32)flLoadAmount / 100.0f;
        flLoadAmount = 0;
    }

    flPs2State.SystemStatus = 1;
    return 1;
}

s32 flEndRender() {
    flDebugTrueTime[1] = DGET_T0_COUNT();
    flLoadNow = flGetSystemTime() - flLoadNow;
    flLoadAmount = flLoadAmount + flLoadNow;
    flPs2State.SystemStatus = 2;

    return 1;
}

s32 flPS2InitRenderState() {
    s32 wk0;

    flInitPhaseStarted();
    flPS2SendRenderState_ALPHA(flSystemRenderOperation, 2);
    flPS2SendRenderState_TEST(flSystemRenderOperation, 2);
    flPS2SendRenderState_ZBUF(flSystemRenderOperation, 2);
    flPS2SendRenderState_SCISSOR(0, 0, flWidth, flHeight, 2);
    flPS2SendRenderState_FOGCOL(flFogColor);
    flPS2SendRenderState_TEX1(flSystemRenderOperation, 0U);
    flPs2FBA = 0;
    flInitPhaseFinished();

    for (wk0 = 0; flWidth > (1 << wk0); wk0++) {}
    flPS2FrameTexScaleX = (f32)flWidth / (f32)(1 << wk0);
    for (wk0 = 0; flHeight > (1 << wk0); wk0++) {}
    flPS2FrameTexScaleY = (f32)flHeight / (1 << wk0);
    flPS2INITMATRIX.m._11 = 1.0f;
    flPS2INITMATRIX.m._12 = 0.0f;
    flPS2INITMATRIX.m._13 = 0.0f;
    flPS2INITMATRIX.m._14 = 0.0f;
    flPS2INITMATRIX.m._21 = 0.0f;
    flPS2INITMATRIX.m._22 = 1.0f;
    flPS2INITMATRIX.m._23 = 0.0f;
    flPS2INITMATRIX.m._24 = 0.0f;
    flPS2INITMATRIX.m._31 = 0.0f;
    flPS2INITMATRIX.m._32 = 0.0f;
    flPS2INITMATRIX.m._33 = 1.0f;
    flPS2INITMATRIX.m._34 = 0.0f;
    flPS2INITMATRIX.m._41 = 0.0f;
    flPS2INITMATRIX.m._42 = 0.0f;
    flPS2INITMATRIX.m._43 = 0.0f;
    flPS2INITMATRIX.m._44 = 1.0f;

    return 1;
}

s32 flSetRenderState(enum _FLSETRENDERSTATE func, u32 value) {
    u32 work, th;
    FL_RenderViewport* viewport;

    if ((func >= FLRENDER_WORLD0) && (func < FLRENDER_MATERIAL0)) {
        if (value != 0) {
            flMATRIX[func - FLRENDER_WORLD0] = *(MTX*)value;
        }

        return 1;
    }

    if ((func >= FLRENDER_MATERIAL0) && (func < FLRENDER_LIGHT0)) {
        flMATERIAL[func - FLRENDER_MATERIAL0] = *(FLMaterial*)value;

        return 1;
    }

    if ((func >= FLRENDER_LIGHT0) && (func < FLRENDER_SHADER)) {
        flLIGHT[func - FLRENDER_LIGHT0] = *(FLLight*)value;
        flvecNormalize((Vec3*)&flLIGHT[func - FLRENDER_LIGHT0].direction);
        flLIGHT[func - FLRENDER_LIGHT0].diffuse.a = 0.0f;
        flLIGHT[func - FLRENDER_LIGHT0].specular.a = 0.0f;
        flPS2ATTENUATION1[func - FLRENDER_LIGHT0] = 1.0f / flLIGHT[func - FLRENDER_LIGHT0].Attenuation1;

        return 1;
    }

    switch (func) {
    case FLRENDER_CULL:
        flSystemRenderState &= ~0x60;
        flSystemRenderState |= value;
        break;

    case FLRENDER_WRAP:
        // Do nothing
        break;

    case FLRENDER_LIGHTING:
        flSystemRenderState &= ~0xF00;

        switch (value) {
        case 0x200:
            flSystemRenderState |= 0x200;
            break;

        case 0x100:
            flSystemRenderState |= 0x100;
            break;

        case 0x300:
            flSystemRenderState |= 0x300;
            break;

        case 0x400:
            flSystemRenderState |= 0x400;
            break;

        case 0x500:
            flSystemRenderState |= 0x500;
            break;

        case 0x600:
            flSystemRenderState |= 0x600;
            break;

        case 0x700:
            flSystemRenderState |= 0x700;
            break;

        case 0x800:
            flSystemRenderState |= 0x800;
            break;

        case 0:
            break;
        }
        break;

    case FLRENDER_SPECULAR:
        flSystemRenderState &= ~0x1C;

        switch (value) {
        case 4:
        case 8:
        case 12:
        case 16:
        case 20:
        case 24:
            flSystemRenderState |= value;
            break;

        default:
        case 0:
            (void)flSystemRenderState;
            break;
        }

        break;

    case FLRENDER_TEXSTAGE0:
    case FLRENDER_TEXSTAGE1:
    case FLRENDER_TEXSTAGE2:
    case FLRENDER_TEXSTAGE3:
        th = value;

        if (func == FLRENDER_TEXSTAGE0) {
            flReloadTexture(1, &th);
            flPS2SendTextureRegister(th);
            flCTH = th;
        }

        flTextureStage[func - 4] = th;
        break;

    case FLRENDER_TEXOPE0:
    case FLRENDER_TEXOPE1:
    case FLRENDER_TEXOPE2:
    case FLRENDER_TEXOPE3:
        // Do nothing
        break;

    case FLRENDER_SCISSOR:
        flSystemRenderState &= ~0x80;

        if (value == 1) {
            flSystemRenderState |= 0x80;
        }
        break;

    case FLRENDER_BLENDOPE:
        work = flSystemRenderOperation & 0xC00;

        if (work != value) {
            flSystemRenderOperation &= ~0xC00;
            flSystemRenderOperation |= value;
            flPS2SendRenderState_ALPHA(flSystemRenderOperation, 0);
        }
        break;

    case FLRENDER_ALPHABLENDMODE:
        work = flSystemRenderOperation & 0xFF;

        if (work != value) {
            flSystemRenderOperation &= ~0xFF;
            flSystemRenderOperation |= value;
            flPS2SendRenderState_ALPHA(flSystemRenderOperation, 0);
        }
        break;

    case FLRENDER_ALPHABLENDENABLE:
        work = flSystemRenderOperation & 0x01000000;

        if (work != value) {
            flSystemRenderOperation &= 0xFEFFFFFF;
            flSystemRenderOperation |= value;
            flPS2SendRenderState_ALPHA(flSystemRenderOperation, 0);
        }
        break;

    case FLRENDER_AMBIENT:
        flAmbient = value;
        flPS2Ambient[0] = ((flAmbient >> 0x10) & 0xFF) / 255.0f;
        flPS2Ambient[1] = ((flAmbient >> 8) & 0xFF) / 255.0f;
        flPS2Ambient[2] = (flAmbient & 0xFF) / 255.0f;
        work = (flAmbient >> 0x18) & 0xFF;

        if (work == 0xFF) {
            flPS2Ambient[3] = 128.0f;
        } else {
            if (work != 0) {
                work = work >> 1;
                if (work == 0)
                    work = 1;
                flPS2Ambient[3] = work;
            } else {
                flPS2Ambient[3] = 0.0f;
            }
        }
        break;

    case FLRENDER_FOGCOLOR:
        if (flFogColor != value) {
            flFogColor = value;
            flPS2SendRenderState_FOGCOL(flFogColor);
        }

        break;

    case FLRENDER_FOGSTART:
        flFogStart = *(f32*)&value;
        break;

    case FLRENDER_FOGEND:
        flFogEnd = *(f32*)&value;
        break;

    case FLRENDER_FOGENABLE:
        flSystemRenderState &= ~0x18000;

        if (value == 0x8000) {
            flSystemRenderState |= 0x8000;
        }
        break;

    case FLRENDER_FLIP:
        break;

    case FLRENDER_BACKCOLOR:
        flPS2SetClearColor(value);
        break;

    case FLRENDER_MATERIAL:
        flSystemRenderState &= ~3;

        if (value == 0) {
            (void)flSystemRenderState;
        } else {
            if (value == 2) {
                flSystemRenderState |= 2;
            }
        }
        break;

    case FLRENDER_VIEW:
        flMATRIX[0x21] = *(MTX*)value;
        break;

    case FLRENDER_PROJ:
        flMATRIX[0x20] = *(MTX*)value;
        flmatMul(&flPS2VIEWPROJ, &flMATRIX[0x20], &flMATRIX[0x22]);
        flmatMul(&flACRVIEWPROJ, &flMATRIX[0x20], &flACRVIEWPORT);
        break;

    case FLRENDER_VIEWPORT:
        viewport = (FL_RenderViewport*)value;
        flmatrMakeViewport(
            0x22, viewport->x, viewport->y, viewport->width, viewport->height, viewport->min_z, viewport->max_z);
        break;

    case FLRENDER_UVSCRMATRIX:
        flMATRIX[0x23] = *(MTX*)value;
        break;

    case FLRENDER_SHADER:
        flSystemRenderState = flSystemRenderState & 0x00FFFFFF;
        flSystemRenderState |= value;
        break;

    case FLRENDER_ALPHATEST:
        switch (value) {
        case 2:
            value = 0x100000;
            break;

        case 5:
            value = 0x280000;
            break;

        case 4:
            value = 0x200000;
            break;

        case 6:
            value = 0x300000;
            break;

        case 0:
            value = 0;
            break;

        case 7:
            value = 0x380000;
            break;

        case 1:
            value = 0x80000;
            break;

        case 3:
            value = 0x180000;
            break;
        }

        work = flSystemRenderOperation & 0x380000;

        if (work != value) {
            flSystemRenderOperation &= ~0x380000;
            flSystemRenderOperation |= value;
            flPS2SendRenderState_TEST(flSystemRenderOperation, 0);
        }

        break;

    case FLRENDER_ALPHAREF:
        if (value != flAlphaRefValue) {
            flAlphaRefValue = value;
            flPS2SendRenderState_TEST(flSystemRenderOperation, 0);
        }
        break;

    case FLRENDER_UVSCROLL:
        flSystemRenderState &= ~0x4000;

        if (value != 0) {
            flSystemRenderState |= 0x4000;
        }

        break;

    case FLRENDER_TEXTUREFILTER:
        flSystemRenderOperation &= ~0x10000;

        if (value == 0x10000) {
            flSystemRenderOperation |= 0x10000;
        } else {
            (void)flSystemRenderOperation;
        }

        flPS2SendRenderState_TEX1(flSystemRenderOperation, 0);
        break;

    case FLRENDER_TEXTUREADDRESSING:
        flSystemRenderOperation &= ~0x60000;

        if (value == 0) {
            (void)flSystemRenderOperation;
        } else {
            if (value == 0x20000) {
                flSystemRenderOperation |= 0x20000;
                break;
            }
        }

        break;

    case FLRENDER_RENDERTARGET:
        // Do nothing
        break;

    case FLRENDER_FADECOLORENABLE:
        flSystemRenderState &= ~0x20000;

        if (value == 1) {
            flSystemRenderState |= 0x20000;
        }

        break;

    case FLRENDER_FADECOLOR:
        flFadeColor = value;
        flPS2FadeColor[0] = ((flFadeColor >> 0x10) & 0xFF) / 255.0f;
        flPS2FadeColor[1] = ((flFadeColor >> 8) & 0xFF) / 255.0f;
        flPS2FadeColor[2] = (flFadeColor & 0xFF) / 255.0f;
        flPS2FadeColor[3] = ((flFadeColor >> 0x18) & 0xFF) / 255.0f;

        break;

    case FLRENDER_MIPMAPBIAS:
        // Do nothing
        break;

    case FLRENDER_MIPMAPARG1:
        flMipmapL = value;
        break;

    case FLRENDER_MIPMAPARG2:
        flMipmapK = value;
        break;

    case FLRENDER_MIPMAPFILTER:
        work = flSystemRenderOperation & 0xC00000;

        if (work != value) {
            flSystemRenderOperation &= ~0xC00000;
            flSystemRenderOperation |= value;
        }
        break;

    case FLRENDER_ZWRITE:
        if (value == 1) {
            value = 0x8000;
        } else {
            value = 0;
        }

        work = flSystemRenderOperation & 0x8000;

        if (work != value) {
            flSystemRenderOperation &= ~0x8000;
            flSystemRenderOperation |= value;
            flPS2SendRenderState_ZBUF(flSystemRenderOperation, 0);
        }
        break;

    case FLRENDER_ZOPE:
        switch (value) {
        case 2:
        case 4:
        case 5:
        case 6:
            break;

        case 0:
            value = 0;
            break;

        case 7:
            value = 0x7000;
            break;

        case 1:
            value = 0x1000;
            break;

        case 3:
            value = 0x3000;
            break;
        }

        work = flSystemRenderOperation & 0x7000;

        if (work != value) {
            flSystemRenderOperation &= ~0x7000;
            flSystemRenderOperation |= value;
            flPS2SendRenderState_TEST(flSystemRenderOperation, 0);
        }

        break;

    default:
        break;
    }

    return 1;
}

void flPS2SetClearColor(u32 col) {
    flPs2State.FrameClearColor = col;
}

s32 flPS2SendRenderState_SCISSOR(s32 dx, s32 dy, s32 dw, s32 dh, u32 flag) {
    s32 dx2;
    s32 dy2;
    u32 qwc;
    uintptr_t keep_ptr;
    QWORD* lpWork;

    if (dx < 0) {
        dx = 0;
    }

    if ((dx + dw) > flWidth) {
        dw = flWidth - dx;
    }

    dx2 = (dx + dw) - 1;
    if (dy < 0) {
        dy = 0;
    }

    if ((dy + dh) > flHeight) {
        dh = flHeight - dy;
    }

    dy2 = (dy + dh) - 1;
    if (flag != 2) {
        lpWork = (QWORD*)flPS2GetSystemTmpBuff(0x30, 0x10);
        qwc = 2;
    } else {
        lpWork = (QWORD*)flPS2GetSystemTmpBuff(0x40, 0x10);
        qwc = 3;
    }

    keep_ptr = (uintptr_t)lpWork;
    lpWork->UI64[0] = (qwc + 0x70000000) | 0x80000000;
    lpWork->UI32[2] = 0x13000000;
    lpWork->UI32[3] = (qwc | 0x50000000);

    lpWork++;
    lpWork->UI64[0] = SCE_GIF_SET_TAG(qwc - 1, 1, 0, 0, SCE_GIF_PACKED, 1);
    lpWork->UI64[1] = SCE_GIF_PACKED_AD;

    lpWork++;
    switch (flag) {
    case 0:
        flPs2State.RenderSCISSORValue1[0] = dx;
        flPs2State.RenderSCISSORValue1[1] = dy;
        flPs2State.RenderSCISSORValue1[2] = dx2;
        flPs2State.RenderSCISSORValue1[3] = dy2;
        flPs2State.RenderSCISSORStatus1 = SCE_GS_SET_SCISSOR(dx, dx2, dy, dy2);

        lpWork->UI64[0] = SCE_GS_SET_SCISSOR(dx, dx2, dy, dy2);
        lpWork->UI64[1] = SCE_GS_SCISSOR_1;
        break;

    case 1:
        flPs2State.RenderSCISSORValue2[0] = dx;
        flPs2State.RenderSCISSORValue2[1] = dy;
        flPs2State.RenderSCISSORValue2[2] = dx2;
        flPs2State.RenderSCISSORValue2[3] = dy2;
        flPs2State.RenderSCISSORStatus2 = SCE_GS_SET_SCISSOR(dx, dx2, dy, dy2);

        lpWork->UI64[0] = SCE_GS_SET_SCISSOR(dx, dx2, dy, dy2);
        lpWork->UI64[1] = SCE_GS_SCISSOR_2;
        break;

    case 2:
        flPs2State.RenderSCISSORValue1[0] = dx;
        flPs2State.RenderSCISSORValue1[1] = dy;
        flPs2State.RenderSCISSORValue1[2] = dx2;
        flPs2State.RenderSCISSORValue1[3] = dy2;
        flPs2State.RenderSCISSORStatus1 = SCE_GS_SET_SCISSOR(dx, dx2, dy, dy2);

        flPs2State.RenderSCISSORValue2[0] = dx;
        flPs2State.RenderSCISSORValue2[1] = dy;
        flPs2State.RenderSCISSORValue2[2] = dx2;
        flPs2State.RenderSCISSORValue2[3] = dy2;
        flPs2State.RenderSCISSORStatus2 = SCE_GS_SET_SCISSOR(dx, dx2, dy, dy2);

        lpWork->UI64[0] = SCE_GS_SET_SCISSOR(dx, dx2, dy, dy2);
        lpWork->UI64[1] = SCE_GS_SCISSOR_1;

        lpWork++;
        lpWork->UI64[0] = SCE_GS_SET_SCISSOR(dx, dx2, dy, dy2);
        lpWork->UI64[1] = SCE_GS_SCISSOR_2;
        break;
    }

    flPS2DmaAddQueue2(0, (keep_ptr & 0xFFFFFFF), keep_ptr, &flPs2VIF1Control);
    return 1;
}

s32 flPS2SendRenderState_ZBUF(u32 render_state, u32 flag) {
    u32 qwc;
    uintptr_t keep_ptr;
    s32 zbp;
    s32 psm;
    s32 zmsk;
    u_long zbuf;
    QWORD* lpWork;

    if ((render_state & 0x8000) == 0x8000) {
        zmsk = 0;
    } else {
        zmsk = 1;
    }

    zbp = flPs2State.ZBuffAdrs >> 5;
    psm = flPs2State.ZBuffForm;
    zbuf = SCE_GS_SET_ZBUF(zbp, psm, zmsk);

    if (flag != 2) {
        lpWork = (QWORD*)flPS2GetSystemTmpBuff(0x30, 0x10);
        qwc = 2;
    } else {
        lpWork = (QWORD*)flPS2GetSystemTmpBuff(0x40, 0x10);
        qwc = 3;
    }

    keep_ptr = (uintptr_t)lpWork;
    lpWork->UI64[0] = (qwc + 0x70000000) | 0x80000000;
    lpWork->UI32[2] = 0x13000000;
    lpWork->UI32[3] = (qwc | 0x50000000);

    lpWork++;
    lpWork->UI64[0] = SCE_GIF_SET_TAG(qwc - 1, 1, 0, 0, SCE_GIF_PACKED, 1);
    lpWork->UI64[1] = SCE_GIF_PACKED_AD;

    lpWork++;
    switch (flag) {
    case 0:
        lpWork->UI64[0] = flPs2State.RenderZBUFStatus1 = zbuf;
        lpWork->UI64[1] = SCE_GS_ZBUF_1;
        break;

    case 1:
        lpWork->UI64[0] = flPs2State.RenderZBUFStatus2 = zbuf;
        lpWork->UI64[1] = SCE_GS_ZBUF_2;
        break;

    case 2:
        lpWork->UI64[0] = flPs2State.RenderZBUFStatus2 = flPs2State.RenderZBUFStatus1 = zbuf;
        lpWork->UI64[1] = SCE_GS_ZBUF_1;

        lpWork++;
        lpWork->UI64[0] = zbuf;
        lpWork->UI64[1] = SCE_GS_ZBUF_2;
        break;
    }

    flPS2DmaAddQueue2(0, (keep_ptr & 0xFFFFFFF), keep_ptr, &flPs2VIF1Control);
    return 1;
}

s32 flPS2SendRenderState_TEST(u32 render_state, u32 flag) {
    u32 qwc;
    uintptr_t keep_ptr;
    QWORD* lpWork;
    u_long test;
    s32 ate;
    s32 atst;
    s32 aref;
    s32 afail;
    s32 date;
    s32 datm;
    s32 zte;
    s32 ztst;

    ate = 1;
    afail = 0;
    aref = flAlphaRefValue;

    if (aref == 0xFF) {
        aref = 0x80;
    } else {
        if (aref != 0) {
            aref = aref >> 1;
            if (aref == 0)
                aref = 1;
        }
    }

    switch (render_state & 0x380000) {
    case 0x0:
        atst = 0;
        break;

    case 0x380000:
        atst = 1;
        break;

    case 0x80000:
        atst = 2;
        break;

    case 0x180000:
        atst = 3;
        break;

    case 0x100000:
        atst = 4;
        break;

    case 0x300000:
        atst = 5;
        break;

    case 0x200000:
        atst = 6;
        break;

    case 0x280000:
        atst = 7;
        break;
    }

    date = 0;
    datm = 0;
    (void)atst;
    zte = 1;

    switch (render_state & 0x7000) {
    case 0x1000:
        ztst = 3;
        break;

    case 0x3000:
        ztst = 2;
        break;

    case 0x7000:
        ztst = 1;
        break;

    case 0x0:
        ztst = 0;
        break;

    case 0x2000:
    case 0x5000:
    case 0x4000:
    case 0x6000:
        ztst = 1;
        break;
    }

    test = SCE_GS_SET_TEST(ate, atst, aref, afail, date, datm, zte, ztst);

    if (flag != 2) {
        lpWork = (QWORD*)flPS2GetSystemTmpBuff(0x30, 0x10);
        qwc = 2;
    } else {
        lpWork = (QWORD*)flPS2GetSystemTmpBuff(0x40, 0x10);
        qwc = 3;
    }

    keep_ptr = (uintptr_t)lpWork;
    (void)test;

    lpWork->UI64[0] = (qwc + 0x70000000) | 0x80000000;
    lpWork->UI32[2] = 0x13000000;
    lpWork->UI32[3] = qwc | 0x50000000;
    lpWork++;

    lpWork->UI64[0] = SCE_GIF_SET_TAG(qwc - 1, 1, 0, 0, SCE_GIF_PACKED, 1);
    lpWork->UI64[1] = SCE_GIF_PACKED_AD;
    lpWork++;

    switch (flag) {
    case 0:
        flPs2State.RenderTESTStatus1 = test;

        lpWork->UI64[0] = test;
        lpWork->UI64[1] = SCE_GS_TEST_1;
        break;

    case 1:
        flPs2State.RenderTESTStatus2 = test;

        lpWork->UI64[0] = test;
        lpWork->UI64[1] = SCE_GS_TEST_2;
        break;

    case 2:
        flPs2State.RenderTESTStatus1 = test;
        flPs2State.RenderTESTStatus2 = test;

        lpWork->UI64[0] = test;
        lpWork->UI64[1] = SCE_GS_TEST_1;
        lpWork++;
        lpWork->UI64[0] = test;
        lpWork->UI64[1] = SCE_GS_TEST_2;
        break;
    }

    flPS2DmaAddQueue2(0, (keep_ptr & 0xFFFFFFF), keep_ptr, &flPs2VIF1Control);

    return 1;
}

s32 flPS2SendRenderState_ALPHA(u32 render_state, u32 flag) {
    u32 qwc;
    uintptr_t keep_ptr;
    u_long alpha;
    QWORD* lpWork;
    u32 blend_ope;

    if (flag != 2) {
        lpWork = (QWORD*)flPS2GetSystemTmpBuff(0x30, 0x10);
        qwc = 2;
    } else {
        lpWork = (QWORD*)flPS2GetSystemTmpBuff(0x40, 0x10);
        qwc = 3;
    }

    keep_ptr = (uintptr_t)lpWork;

    lpWork->UI64[0] = (qwc + 0x70000000) | 0x80000000;
    lpWork->UI32[2] = 0x13000000;
    lpWork->UI32[3] = (qwc | 0x50000000);
    lpWork++;

    lpWork->UI64[0] = SCE_GIF_SET_TAG(qwc - 1, 1, 0, 0, SCE_GIF_PACKED, 1);
    lpWork->UI64[1] = SCE_GIF_PACKED_AD;
    lpWork++;

    blend_ope = render_state & 0xC00;

    if (blend_ope == 0) {
        switch (render_state & 0xF) {
        case 0x1:
            switch (render_state & 0xF0) {
            case 0x10:
                alpha = SCE_GS_SET_ALPHA(0, 2, 2, 1, 128);
                break;

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(0, 2, 2, 2, 128);
                break;

            case 0x20:
                alpha = SCE_GS_SET_ALPHA(1, 2, 0, 0, 0);
                break;

            case 0x30:
                alpha = SCE_GS_SET_ALPHA(2, 1, 0, 0, 0);
                break;

            case 0x40:
                alpha = SCE_GS_SET_ALPHA(1, 2, 1, 0, 0);
                break;

            case 0x50:
                alpha = SCE_GS_SET_ALPHA(2, 1, 1, 0, 0);
                break;

            default:
                return 0;
            }
            break;

        case 0x0:
            switch (render_state & 0xF0) {
            case 0x10:
                alpha = SCE_GS_SET_ALPHA(2, 2, 2, 1, 128);
                break;

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(2, 2, 2, 2, 128);
                break;

            case 0x20:
                alpha = SCE_GS_SET_ALPHA(1, 2, 0, 2, 0);
                break;

            case 0x30:
                alpha = SCE_GS_SET_ALPHA(2, 1, 0, 1, 0);
                break;

            case 0x40:
                alpha = SCE_GS_SET_ALPHA(1, 2, 1, 2, 0);
                break;

            case 0x50:
                alpha = SCE_GS_SET_ALPHA(2, 1, 1, 1, 0);
                break;

            default:
                return 0;
            }
            break;

        case 0x2:
            switch (render_state & 0xF0) {
            case 16:
                alpha = SCE_GS_SET_ALPHA(0, 2, 0, 1, 0);
                break;

            case 0:
                alpha = SCE_GS_SET_ALPHA(0, 2, 0, 2, 0);
                break;

            case 48:
                alpha = SCE_GS_SET_ALPHA(0, 1, 0, 1, 0);
                break;

            default:
                return 0;
            }
            break;

        case 0x3:
            switch (render_state & 0xF0) {

            case 0:
                alpha = SCE_GS_SET_ALPHA(2, 0, 0, 0, 0);
                break;

            case 32:
                alpha = SCE_GS_SET_ALPHA(1, 0, 0, 0, 0);
                break;

            default:
                return 0;
            }
            break;

        case 0x4:
            switch (render_state & 0xF0) {
            case 0x10:
                alpha = SCE_GS_SET_ALPHA(0, 2, 1, 1, 0);
                break;

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(0, 2, 1, 2, 0);
                break;

            case 0x50:
                alpha = SCE_GS_SET_ALPHA(0, 1, 1, 1, 0);
                break;

            default:
                return 0;
            }
            break;

        case 0x5:
            switch (render_state & 0xF0) {

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(2, 0, 1, 0, 0);
                break;

            case 0x40:
                alpha = SCE_GS_SET_ALPHA(1, 0, 1, 0, 0);
                break;

            default:
                return 0;
            }
            break;

        default:
            return 0;
        }
    } else if (blend_ope == 0x400) {

        switch (render_state & 0xF) {
        case 0x1:
            switch (render_state & 0xF0) {
            case 16:
                alpha = SCE_GS_SET_ALPHA(0, 9, 8, 0, 128);
                break;

            case 0:
                alpha = SCE_GS_SET_ALPHA(2, 10, 2, 0, 128);
                break;

            default:
                return 0;
            }
            break;

        case 0x0:
            switch (render_state & 0xF0) {
            case 0x10:
                alpha = SCE_GS_SET_ALPHA(2, 9, 8, 0, 128);
                break;

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(2, 10, 8, 0, 128);
                break;

            case 0x20:
                alpha = SCE_GS_SET_ALPHA(2, 1, 0, 2, 0);
                break;

            case 0x40:
                alpha = SCE_GS_SET_ALPHA(2, 1, 1, 2, 0);
                break;

            default:
                return 0;
            }
            break;

        case 0x2:
            switch (render_state & 0xF0) {

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(0, 2, 0, 2, 0);
                break;

            case 0x20:
                alpha = SCE_GS_SET_ALPHA(0, 1, 0, 2, 0);
                break;

            default:
                return 0;
            }
            break;

        case 0x3:
            switch (render_state & 0xF0) {

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(2, 0, 0, 0, 0);
                break;

            default:
                return 0;
            }
            break;

        case 0x4:
            switch (render_state & 0xF0) {

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(0, 2, 1, 2, 0);
                break;

            case 0x40:
                alpha = SCE_GS_SET_ALPHA(0, 1, 1, 2, 0);
                break;

            default:
                return 0;
            }
            break;

        case 0x5:
            switch (render_state & 0xF0) {

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(2, 1, 1, 1, 0);
                break;

            default:
                return 0;
            }
            break;

        default:
            return 0;
        }
    } else if (blend_ope == 0x800) {

        switch (render_state & 0xF) {
        case 0x1:
            switch (render_state & 0xF0) {
            case 0x10:
                alpha = SCE_GS_SET_ALPHA(1, 0, 2, 2, 128);
                break;

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(2, 0, 2, 2, 128);
                break;

            default:
                return 0;
            }
            break;

        case 0x0:
            switch (render_state & 0xF0) {
            case 0x10:
                alpha = SCE_GS_SET_ALPHA(1, 2, 2, 2, 128);
                break;

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(2, 2, 2, 2, 128);
                ;
                break;

            case 0x20:
                alpha = SCE_GS_SET_ALPHA(1, 2, 0, 2, 0);
                break;

            case 0x30:
                alpha = SCE_GS_SET_ALPHA(2, 1, 0, 1, 0);
                break;

            case 0x40:
                alpha = SCE_GS_SET_ALPHA(1, 2, 1, 2, 0);
                break;

            case 0x50:
                alpha = SCE_GS_SET_ALPHA(2, 1, 1, 1, 0);
                break;

            default:
                return 0;
            }
            break;

        case 0x2:
            switch (render_state & 0xF0) {

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(2, 0, 0, 2, 0);
                break;

            case 0x20:
                alpha = SCE_GS_SET_ALPHA(1, 0, 0, 2, 0);
                break;

            default:
                return 0;
            }
            break;

        case 0x4:
            switch (render_state & 0xF0) {

            case 0x0:
                alpha = SCE_GS_SET_ALPHA(2, 0, 1, 2, 0);
                break;

            case 0x40:
                alpha = SCE_GS_SET_ALPHA(1, 0, 1, 2, 0);
                break;

            default:
                return 0;
            }
            break;

        default:
            return 0;
        }
    }

    if (!(render_state & 0x01000000)) {
        alpha = SCE_GS_SET_ALPHA(0, 2, 2, 2, 128);
    }

    switch (flag) {
    case 0:
        lpWork->UI64[0] = alpha;
        lpWork->UI64[1] = SCE_GS_ALPHA_1;
        break;

    case 1:
        lpWork->UI64[0] = alpha;
        lpWork->UI64[1] = SCE_GS_ALPHA_2;
        break;

    case 2:
        lpWork->UI64[0] = alpha;
        lpWork->UI64[1] = SCE_GS_ALPHA_1;
        lpWork++;

        lpWork->UI64[0] = alpha;
        lpWork->UI64[1] = SCE_GS_ALPHA_2;
        break;
    }

    flPS2DmaAddQueue2(0, keep_ptr & 0xFFFFFFF, keep_ptr, &flPs2VIF1Control);

    return 1;
}

s32 flPS2SendRenderState_FOGCOL(u32 fogcol) {
    u32 qwc;
    uintptr_t keep_ptr;
    QWORD* lpWork;

    lpWork = (QWORD*)flPS2GetSystemTmpBuff(0x30, 0x10);
    qwc = 2;
    keep_ptr = (uintptr_t)lpWork;

    lpWork->UI64[0] = (qwc + 0x70000000) | 0x80000000;
    lpWork->UI32[2] = 0x13000000;
    lpWork->UI32[3] = qwc | 0x51000000;
    lpWork++;

    lpWork->UI64[0] = SCE_GIF_SET_TAG(qwc - 1, 1, 0, 0, SCE_GIF_PACKED, 1);
    lpWork->UI64[1] = SCE_GIF_PACKED_AD;
    lpWork++;

    lpWork->UI64[0] = SCE_GS_SET_FOGCOL((fogcol >> 16) & 0xFF, (fogcol >> 8) & 0xFF, fogcol & 0xFF);
    lpWork->UI64[1] = SCE_GS_FOGCOL;

    flPS2DmaAddQueue2(0, (keep_ptr & 0xFFFFFFF), keep_ptr, &flPs2VIF1Control);
    return 1;
}

s32 flPS2SendRenderState_TEX1(u32 render_state, u32 flag) {
    u32 qwc;
    uintptr_t keep_ptr;
    QWORD* lpWork;

    lpWork = (QWORD*)flPS2GetSystemTmpBuff(0x30, 0x10);
    qwc = 2;
    keep_ptr = (uintptr_t)lpWork;

    lpWork->UI64[0] = (qwc + 0x70000000) | 0x80000000;
    lpWork->UI32[2] = 0x13000000;
    lpWork->UI32[3] = qwc | 0x50000000;
    lpWork++;

    lpWork->UI64[0] = SCE_GIF_SET_TAG(qwc - 1, 1, 0, 0, SCE_GIF_PACKED, 1);
    lpWork->UI64[1] = SCE_GIF_PACKED_AD;
    lpWork++;

    if ((render_state & 0x10000) == 0x10000) {
        lpWork->UI64[0] = 0;
    } else {
        lpWork->UI64[0] = 0x60;
    }

    if (flag == 0) {
        lpWork->UI64[1] = 0x14;
    } else {
        lpWork->UI64[1] = 0x15;
    }

    flPS2DmaAddQueue2(0, (keep_ptr & 0xFFFFFFF), keep_ptr, &flPs2VIF1Control);
    return 1;
}

s32 flPS2SendTextureRegister(u32 th) {
    static u64 psTexture_data[16] = {
        0x0000000070000007, 0x0000000000000000, 0x1000000000008006, 0x000000000000000E,
        0x0000000000000000, 0x000000000000003B, 0x0000000000000000, 0x0000000000000014,
        0x0000000000000000, 0x0000000000000006, 0x0000000000000000, 0x0000000000000008,
        0x0000000000000000, 0x0000000000000034, 0x0000000000000000, 0x0000000000000036,
    };

    if (!flPS2SetTextureRegister(th,
                                 &psTexture_data[4],
                                 &psTexture_data[6],
                                 &psTexture_data[8],
                                 &psTexture_data[10],
                                 &psTexture_data[12],
                                 &psTexture_data[14],
                                 flSystemRenderOperation)) {
        return 0;
    }

    flPS2psAddQueue((QWORD*)psTexture_data);
    return 1;
}

s32 flPS2SetTextureRegister(u32 th, u64* texA, u64* tex1, u64* tex0, u64* clamp, u64* miptbp1, u64* miptbp2,
                            u32 render_ope) {
    FLTexture* lpflTexture;
    FLTexture* lpflPalette;
    s16 dbw;
    s32 lp0;
    s32 dw;
    s32 mip_tbp[MIP_TBP_SIZE];
    s32 mip_dbw[MIP_DBW_SIZE];

#if !defined(TARGET_PS2)
    SDLGameRenderer_SetTexture(th);
    // return;
#endif

    lpflTexture = &flTexture[LO_16_BITS(th) - 1];

    if (!LO_16_BITS(th) || (LO_16_BITS(th) > FL_TEXTURE_MAX)) {
        flPS2SystemError(0, "ERROR flPS2SetTextureRegister flps2render.c 1");
    }

    if (lpflTexture->tbp == 0) {
        return 0;
    }

    if (lpflTexture->vram_on_flag == 0) {
        flPS2SystemError(0, "ERROR flPS2SetTextureRegister flps2render.c 1");
    }

    if (lpflTexture->tbp > 0 && lpflTexture->tbp < flPs2State.TextureStartAdrs) {
        flPS2SystemError(0, "ERROR flPS2SetTextureRegister flps2render.c 1");
    }

    if (lpflTexture->desc & 0x4) {
        lpflPalette = &flPalette[HI_16_BITS(th) - 1];

        if (!HI_16_BITS(th) || HI_16_BITS(th) > FL_PALETTE_MAX) {
            flPS2SystemError(0, "ERROR flPS2SetTextureRegister flps2render.c 2");
        }

        if (lpflPalette->tbp == 0) {
            return 0;
        }

        if (lpflPalette->vram_on_flag == 0) {
            flPS2SystemError(0, "ERROR flPS2SetTextureRegister flps2render.c 2");
        }

        if (lpflPalette->tbp > 0 && lpflPalette->tbp < flPs2State.TextureStartAdrs) {
            flPS2SystemError(0, "ERROR flPS2SetTextureRegister flps2render.c 2");
        }
    }

    if (!(render_ope & 0xC00000) || lpflTexture->tex_num == 1) {
        if ((render_ope & 0x10000) == 0x10000) {
            tex1[0] = 0;
        } else {
            tex1[0] = 0x60;
        }
    } else {
        if ((render_ope & 0xC00000) == 0x400000) {
            if ((render_ope & 0x10000) == 0x10000) {
                tex1[0] =
                    SCE_GS_SET_TEX1(0x80, (lpflTexture->tex_num - 1), 0, 0, 0, flMipmapL, (flMipmapK << 4) & 0xFFF);
            } else {
                tex1[0] =
                    SCE_GS_SET_TEX1(0x20, (lpflTexture->tex_num - 1), 8, 0, 0, flMipmapL, (flMipmapK << 4) & 0xFFF);
            }
        } else {
            if ((render_ope & 0x10000) == 0x10000) {
                tex1[0] =
                    SCE_GS_SET_TEX1(0xC0, (lpflTexture->tex_num - 1), 0, 0, 0, flMipmapL, (flMipmapK << 4) & 0xFFF);
            } else {
                tex1[0] =
                    SCE_GS_SET_TEX1(0x20, (lpflTexture->tex_num - 1), 10, 0, 0, flMipmapL, (flMipmapK << 4) & 0xFFF);
            }
        }
    }

    dbw = lpflTexture->width / 64;

    if (dbw == 0) {
        dbw = 1;
    }

    if (lpflTexture->desc & 0x4) {
        lpflPalette = &flPalette[HI_16_BITS(th) - 1];

        if (lpflPalette->bitdepth == 3) {
            texA[0] = SCE_GS_SET_TEXA(0x80, 0, 0x80);
        } else {
            texA[0] = SCE_GS_SET_TEXA(0, 0, 0x80);
        }

        tex0[0] = SCE_GS_SET_TEX0(lpflTexture->tbp,
                                  dbw,
                                  lpflTexture->format,
                                  lpflTexture->tw,
                                  lpflTexture->th,
                                  1,
                                  0,
                                  lpflPalette->tbp,
                                  lpflPalette->format,
                                  128,
                                  0,
                                  0);
    } else {
        if (lpflTexture->bitdepth == 3) {
            texA[0] = SCE_GS_SET_TEXA(0x80, 0, 0x80);
        } else {
            texA[0] = SCE_GS_SET_TEXA(0, 0, 0x80);
        }

        tex0[0] = SCE_GS_SET_TEX0(
            lpflTexture->tbp, dbw, lpflTexture->format, lpflTexture->tw, lpflTexture->th, 1, 0, 0, 0, 0, 0, 0);
    }

    switch (render_ope & 0x60000) {
    case 0:
        clamp[0] = 0;
        break;

    case 0x20000:
    default:
        clamp[0] = SCE_GS_SET_CLAMP(0xA, 0, 0, lpflTexture->width, 0, lpflTexture->height);
        break;
    }

    miptbp1[0] = 0;
    miptbp2[0] = 0;

    if (lpflTexture->tex_num > 1) {
        for (lp0 = 0; lp0 < 6; lp0++) {
            mip_tbp[lp0] = 0;
            mip_dbw[lp0] = 0;
        }

        dw = lpflTexture->width >> 1;

        for (lp0 = 0; lp0 < lpflTexture->tex_num - 1; lp0 += 1) {
            mip_tbp[lp0] = flPS2GetVramTransAdrs(lpflTexture, lp0 + 1);
            mip_dbw[lp0] = dw / 64;

            if (!mip_dbw[lp0]) {
                mip_dbw[lp0] = 1;
            }

            dw >>= 1;
        }

        miptbp1[0] = SCE_GS_SET_MIPTBP1(mip_tbp[0], mip_dbw[0], mip_tbp[1], mip_dbw[1], mip_tbp[2], mip_dbw[2]);
        miptbp2[0] = SCE_GS_SET_MIPTBP2(mip_tbp[3], mip_dbw[3], mip_tbp[4], mip_dbw[4], mip_tbp[5], mip_dbw[5]);
    }

    return 1;
}

s16 flPS2ConvScreenX(s16 x) {
    x -= flPs2State.ScreenOffsetX;

    return x;
}

s16 flPS2ConvScreenY(s16 y) {
    y -= flPs2State.ScreenOffsetY;

    if (flPs2State.InterlaceMode == 0) {
        y >>= 1;
    }

    return y;
}

f32 flPS2ConvScreenFX(f32 x) {
    x -= flPs2State.ScreenOffsetX;

    return x;
}

f32 flPS2ConvScreenFY(f32 y) {
    y -= flPs2State.ScreenOffsetY;

    if (flPs2State.InterlaceMode == 0) {
        y = y * 0.5f;
    }

    return y;
}

f32 flPS2ConvScreenFZ(f32 z) {
    z -= 1.0f;
    z = z * -0.5f;
    z *= flPs2State.ZBuffMax;

    return z;
}

u32 flPS2ConvColor(u32 c, s32 type) {
    s32 lp0;
    u16 col[4];

    col[0] = (c >> 16) & 0xFF;
    col[1] = (c >> 8) & 0xFF;
    col[2] = c & 0xFF;
    col[3] = (c >> 24) & 0xFF;

    if (type == 0) {
        for (lp0 = 0; lp0 < 4; lp0++) {
            col[lp0] = (col[lp0] + 1) >> 1;
        }
    } else {
        col[3] = (col[3] + 1) >> 1;
    }

    return (col[3] << 24) | (col[0] << 16) | (col[1] << 8) | (col[2]);
}

void flAdjustScreen(s32 x, s32 y) {
    flPs2State.ScreenAdjustX = x;
    flPs2State.ScreenAdjustY = y & -2;
}
