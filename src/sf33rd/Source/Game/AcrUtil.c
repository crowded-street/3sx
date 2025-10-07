#include "sf33rd/Source/Game/AcrUtil.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2math.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "structs.h"

typedef struct {
    // total size: 0x60
    MTX mtx;     // offset 0x0, size 0x40
    VPRM prm;    // offset 0x40, size 0x18
    s8 pad58[8]; // offset 0x58, size 0x8
} AppView;

AppView appView;

void appViewSetItems(VPRM* prm) {
    appView.prm = *prm;
}

void appViewGetItems(VPRM* prm) {
    *prm = appView.prm;
}

void appViewMatrix() {
    flmatMakeViewport(
        &appView.mtx, appView.prm.x0, appView.prm.y0, appView.prm.x1, appView.prm.y1, appView.prm.ne, appView.prm.fa);
}

void render_start() {
    flBeginRender();
}

void render_end() {
    flEndRender();
}

void initRenderState(s32 flag) {
    f32 fogstart = 20000.0f;
    f32 fogend = 30000.0f;

    if (flag == 0) {
        flSetRenderState(FLRENDER_AMBIENT, 0);
        flSetRenderState(FLRENDER_LIGHTING, 0x200);
        flSetRenderState(FLRENDER_MATERIAL, 2);
        flSetRenderState(FLRENDER_FOGCOLOR, 0x80FFFFFF);
        flSetRenderState(FLRENDER_FOGSTART, REINTERPRET_AS_U32(fogstart));
        flSetRenderState(FLRENDER_FOGEND, REINTERPRET_AS_U32(fogend));
        flSetRenderState(FLRENDER_ALPHATEST, 4);
        flSetRenderState(FLRENDER_ZWRITE, 1);
        flSetRenderState(FLRENDER_ZOPE, 1);
    }

    setBlendMode(0, 4, 5);
    setFilterMode(1);
    flSetRenderState(FLRENDER_ALPHAREF, 0);
}

void setZ_Operation(s32 flag) {
    static u32 ope_mode[7] = { 0, 1, 2, 3, 4, 5, 6 };
    flSetRenderState(FLRENDER_ZOPE, ope_mode[flag]);
}

void setBlendMode(s32 ope, s32 src, s32 dst) {
    setBlendSource(src, dst);
    setBlendOperation(ope);
}

void setBlendOperation(s32 ope) {
    static unsigned int ope_mode[3] = { 0, 0x400, 0x800 };
    flSetRenderState(FLRENDER_BLENDOPE, ope_mode[ope]);
}

void setBlendSource(s32 src, s32 dst) {
    static u32 src_mode[10] = { 0x00, 0x01, 0x04, 0x05, 0x02, 0x03, 0x08, 0x09, 0x06, 0x07 };
    static u32 dst_mode[10] = { 0x00, 0x10, 0x40, 0x50, 0x20, 0x30, 0x80, 0x90, 0x60, 0x70 };
    flSetRenderState(FLRENDER_ALPHABLENDMODE, src_mode[src] | dst_mode[dst]);
}

void setFilterMode(s32 mode) {
    static u32 filter_mode[2] = { 0x10000, 0 };
    flSetRenderState(FLRENDER_TEXTUREFILTER, filter_mode[mode]);
}

void setTexAdrsMode(s32 mode) {
    static u32 addressing_mode[3] = { 0x20000, 0, 0x40000 };
    flSetRenderState(FLRENDER_TEXTUREADDRESSING, addressing_mode[mode]);
}

void setBackGroundColor(u32 color) {
    flSetRenderState(FLRENDER_BACKCOLOR, color);
}
