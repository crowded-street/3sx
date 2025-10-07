#include "sf33rd/AcrSDK/ps2/flps2math.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "structs.h"

void flmatCopy(MTX* dst, const MTX* src);
void flPS2MakeClipViewport(s32 /* unused */, s32 /* unused */, s32 dwWidth, s32 dwHeight, f32 dvMinz, f32 dvMaxz);
void flmatInit(MTX* lpmat);

void flPS2MakeClipProjection(MTX* lppro) {
    f32 width;
    f32 height;

    flmatCopy(&flPS2CLIPPROJ, lppro);
    width = 0.5f * flViewportDW;
    height = 0.5f * flViewportDH;
    flPS2CLIPPROJ.f[0] = (2.0f * flPS2CLIPPROJ.f[0]) / (flViewportLX / width);
    flPS2CLIPPROJ.f[5] = (2.0f * flPS2CLIPPROJ.f[5]) / (flViewportLY / height);
}

void flmatrMakeViewport(s32 mdst, s32 dwx, s32 dwy, s32 dwWidth, s32 dwHeight, f32 dvMinz, f32 dvMaxz) {
    MTX* lpmat = &flMATRIX[mdst];

    flmatMakeViewport(lpmat, dwx, dwy, dwWidth, dwHeight, dvMinz, dvMaxz);
    flPS2MakeClipProjection(&flMATRIX[0x20]);
    flmatMul(&flPS2VIEWPROJ, &flMATRIX[0x20], &flMATRIX[0x22]);
    flmatMul(&flACRVIEWPROJ, &flMATRIX[0x20], &flACRVIEWPORT);

    if (flPs2State.InterlaceMode == 0) {
        dwy /= 2;
        dwHeight /= 2;
    }

    flPS2SendRenderState_SCISSOR(dwx, dwy, dwWidth, dwHeight, 2);
}

void flmatMakeViewport(MTX* lpmat, s32 dwx, s32 dwy, s32 dwWidth, s32 dwHeight, f32 dvMinz, f32 dvMaxz) {
    f32 width;
    f32 height;
    f32 cwidth;
    f32 cheight;

    width = dwWidth / 2;
    height = dwHeight / 2;
    flmatInit(&flACRVIEWPORT);
    flACRVIEWPORT.f[0] = width;
    flACRVIEWPORT.f[5] = -height;
    flACRVIEWPORT.f[0xA] = dvMaxz - dvMinz;
    flACRVIEWPORT.f[0xC] = dwx + width;
    flACRVIEWPORT.f[0xD] = dwy + height;
    flACRVIEWPORT.f[0xE] = dvMinz;
    flACRVIEWPORT.f[0xF] = 1.0f;

    if (flPs2State.InterlaceMode == 0) {
        dwy /= 2;
        dwHeight /= 2;
    }

    width = dwWidth / 2;
    height = dwHeight / 2;
    flmatInit(lpmat);
    lpmat->f[0] = width;
    lpmat->f[5] = -height;
    lpmat->f[0xA] = dvMaxz - dvMinz;
    lpmat->f[0xC] = dwx + width;
    lpmat->f[0xD] = dwy + height;
    lpmat->f[0xF] = 1.0f;
    lpmat->f[0xA] *= 0.5f * -flPs2State.ZBuffMax;
    lpmat->f[0xC] = (0x800 - (flWidth >> 1)) + lpmat->f[0xC];
    lpmat->f[0xD] = (0x800 - (flHeight >> 1)) + lpmat->f[0xD];
    lpmat->f[0xE] = 0.5f * flPs2State.ZBuffMax;
    flViewportCX = lpmat->f[0xC];
    flViewportCY = lpmat->f[0xD];
    flViewportDX = dwx;
    flViewportDY = dwy;
    flViewportDW = dwWidth;
    flViewportDH = dwHeight;
    flViewportLX = 2048.0f - flViewportCX;

    if (flViewportLX < 0.0f) {
        flViewportLX *= -1.0f;
    }

    flViewportLX = 2048.0f - flViewportLX;
    flViewportLY = 2048.0f - flViewportCY;

    if (flViewportLY < 0.0f) {
        flViewportLY *= -1.0f;
    }

    flViewportLY = 2048.0f - flViewportLY;
    cwidth = flViewportLX;
    cheight = flViewportLY;
    flPS2MakeClipViewport(0, 0, cwidth, cheight, dvMinz, dvMaxz);
}

void flPS2MakeClipViewport(s32 /* unused */, s32 /* unused */, s32 dwWidth, s32 dwHeight, f32 dvMinz, f32 dvMaxz) {
    f32 width = dwWidth / 2;
    f32 height = dwHeight / 2;

    flmatInit(&flPS2VIEWPORT);
    flPS2VIEWPORT.f[0] = width;
    flPS2VIEWPORT.f[5] = -height;
    flPS2VIEWPORT.f[0xA] = dvMaxz - dvMinz;
    flPS2VIEWPORT.f[0xF] = 1.0f;
    flPS2VIEWPORT.f[0xA] *= 0.5f * -flPs2State.ZBuffMax;
    flPS2VIEWPORT.f[0xC] = flViewportCX;
    flPS2VIEWPORT.f[0xD] = flViewportCY;
    flPS2VIEWPORT.f[0xE] = 0.5f * flPs2State.ZBuffMax;
}

void flmatInit(MTX* lpmat) {
#if !defined(TARGET_PS2)
    *lpmat = flPS2INITMATRIX;
#else
    __asm__ __volatile__("lqc2 $vf4, 0(%1)"
                         "lqc2 $vf5, 0x10(%1)"
                         "lqc2 $vf6, 0x20(%1)"
                         "lqc2 $vf7, 0x30(%1)"
                         "sqc2 $vf4, 0(%0)"
                         "sqc2 $vf5, 0x10(%0)"
                         "sqc2 $vf6, 0x20(%0)"
                         "sqc2 $vf7, 0x30(%0)"
                         :
                         : "w"(lpmat), "m"(flPS2INITMATRIX)
                         : "memory");
#endif
}

void flmatMul(MTX* lpdst, const MTX* lpsrc1, const MTX* lpsrc2) {
#if !defined(TARGET_PS2)
    not_implemented(__func__);
#else
    __asm__ __volatile__("lqc2       $vf4, 0x0(%2)"
                         "lqc2       $vf5, 0x10(%2)"
                         "lqc2       $vf6, 0x20(%2)"
                         "lqc2       $vf7, 0x30(%2)"
                         "lqc2       $vf8, 0x0(%1)"
                         "lqc2       $vf9, 0x10(%1)"
                         "lqc2       $vf10, 0x20(%1)"
                         "lqc2       $vf11, 0x30(%1)"
                         "vmulax.xyzw $ACCxyzw, $vf4xyzw, $vf8x"
                         "vmadday.xyzw $ACCxyzw, $vf5xyzw, $vf8y"
                         "vmaddaz.xyzw $ACCxyzw, $vf6xyzw, $vf8z"
                         "vmaddw.xyzw $vf8xyzw, $vf7xyzw, $vf8w"
                         "vmulax.xyzw $ACCxyzw,$vf4xyzw,$vf9x"
                         "vmadday.xyzw $ACCxyzw,$vf5xyzw,$vf9y"
                         "vmaddaz.xyzw $ACCxyzw,$vf6xyzw,$vf9z"
                         "vmaddw.xyzw $vf9xyzw,$vf7xyzw,$vf9w"
                         "vmulax.xyzw $ACCxyzw,$vf4xyzw,$vf10x"
                         "vmadday.xyzw $ACCxyzw,$vf5xyzw,$vf10y"
                         "vmaddaz.xyzw $ACCxyzw,$vf6xyzw,$vf10z"
                         "vmaddw.xyzw $vf10xyzw,$vf7xyzw,$vf10w"
                         "vmulax.xyzw $ACCxyzw,$vf4xyzw,$vf11x"
                         "vmadday.xyzw $ACCxyzw,$vf5xyzw,$vf11y"
                         "vmaddaz.xyzw $ACCxyzw,$vf6xyzw,$vf11z"
                         "vmaddw.xyzw $vf11xyzw,$vf7xyzw,$vf11w"
                         "sqc2       $vf8, 0x0(%0)"
                         "sqc2       $vf9, 0x10(%0)"
                         "sqc2       $vf10, 0x20(%0)"
                         "sqc2       $vf11, 0x30(%0)"
                         :
                         : "w"(lpdst), "r"(lpsrc1), "r"(lpsrc2)
                         : "memory");
#endif
}

void flmatCopy(MTX* dst, const MTX* src) {
#if !defined(TARGET_PS2)
    not_implemented(__func__);
#else
    __asm__ __volatile__("lq a2, 0(%1)"
                         "lq a3, 0x10(%1)"
                         "lq t0, 0x20(%1)"
                         "lq t1, 0x30(%1)"
                         "sq a2, 0(%0)"
                         "sq a3, 0x10(%0)"
                         "sq t0, 0x20(%0)"
                         "sq t1, 0x30(%0)"
                         :
                         : "w"(dst), "r"(src)
                         : "memory");
#endif
}

void flvecNormalize(Vec3* lpvec) {
#if !defined(TARGET_PS2)
    not_implemented(__func__);
#else
    Vec3 vec = *lpvec;

    __asm__ __volatile__("lqc2 $vf4, 0(%1)"
                         "vmul.xyz $vf5xyz, $vf4xyz, $vf4xyz"
                         "vaddy.x $vf5x, $vf5x, $vf5y"
                         "vaddz.x $vf5x,$vf5x,$vf5z"
                         "vrsqrt  $Q, $vf0w, $vf5x"
                         "vwaitq"
                         "vmulq.xyz $vf4xyz, $vf4xyz, $Q"
                         "sqc2 $vf4, 0(%1)"
                         :
                         : "w"(lpvec), "m"(vec)
                         : "memory");

    *lpvec = vec;
#endif
}
