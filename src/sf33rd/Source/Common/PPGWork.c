#include "sf33rd/Source/Common/PPGWork.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"

Texture ppgBgTex[4];
PPGDataList ppgBgList[4];

Texture ppgRwBgTex;
PPGDataList ppgRwBgList;

Texture ppgAkeTex;
Palette ppgAkePal;
PPGDataList ppgAkeList;

Texture ppgAkaneTex;
Palette ppgAkanePal;
PPGDataList ppgAkaneList;

Texture ppgScrTex;
Palette ppgScrPal;
Palette ppgScrPalFace;
Palette ppgScrPalShot;
Palette ppgScrPalOpt;
PPGDataList ppgScrList;
PPGDataList ppgScrListFace;
PPGDataList ppgScrListShot;
PPGDataList ppgScrListOpt;

Texture ppgOpnBgTex;
PPGDataList ppgOpnBgList;

Texture ppgTitleTex;
PPGDataList ppgTitleList;

Texture ppgWarTex;
Palette ppgWarPal;
Palette ppgAdxPal;
PPGDataList ppgWarList;
PPGDataList ppgAdxList;

Texture ppgCapLogoTex;
Palette ppgCapLogoPal;
PPGDataList ppgCapLogoList;

void ppgWorkInitializeApprication() {
    s32 i;

    for (i = 0; i < 4; i++) {
        ppgBgTex[i].be = 0;
    }

    ppgRwBgTex.be = 0;
    ppgAkeTex.be = ppgAkePal.be = 0;
    ppgAkaneTex.be = ppgAkanePal.be = 0;
    ppgScrTex.be = ppgScrPal.be = ppgScrPalFace.be = ppgScrPalShot.be = ppgScrPalOpt.be = 0;
    ppgOpnBgTex.be = 0;
    ppgTitleTex.be = 0;
    ppgWarTex.be = ppgWarPal.be = ppgAdxPal.be = 0;
    ppgCapLogoTex.be = ppgCapLogoPal.be = 0;

    for (i = 0; i < 24; i++) {
        mts[i].tex.be = 0;
    }

    col3rd_w.palDC.be = col3rd_w.palCP3.be = 0;
}

/* Releasing one texture or palette, where it is loaded. The purge switch wrote
 * that same `if` over and over, differing in nothing but the object, which
 * travels to the helper as its address. */
static void purge_texture_if_loaded(Texture* tex) {
    if (tex->be) {
        ppgPurgeTextureFromVRAM(tex);
    }
}

static void purge_palette_if_loaded(Palette* pal) {
    if (pal->be) {
        ppgPurgePaletteFromVRAM(pal);
    }
}

/* The two texture sets the purge walks rather than names one at a time. */
static void purge_stage_bg_textures() {
    s32 i;

    for (i = 0; i < 4; i++) {
        purge_texture_if_loaded(&ppgBgTex[i]);
    }
}

static void purge_multi_textures() {
    s32 i;

    for (i = 1; i < 24; i++) {
        if ((mts_ok[i].be) && (mts[i].tex.be)) {
            ppgPurgeTextureFromVRAM(&mts[i].tex);
        }
    }
}

void ppgPurgeFromVRAM(s32 type) {

    switch (type) {
    case 0:
        purge_texture_if_loaded(&ppgScrTex);
        purge_palette_if_loaded(&ppgScrPal);
        purge_palette_if_loaded(&ppgScrPalFace);
        purge_palette_if_loaded(&ppgScrPalShot);
        purge_palette_if_loaded(&ppgScrPalOpt);

        break;

    case 1:
        purge_texture_if_loaded(&ppgWarTex);
        purge_palette_if_loaded(&ppgWarPal);
        purge_palette_if_loaded(&ppgAdxPal);

        break;

    case 2:
        purge_texture_if_loaded(&ppgOpnBgTex);
        purge_texture_if_loaded(&ppgCapLogoTex);
        purge_palette_if_loaded(&ppgCapLogoPal);
        purge_palette_if_loaded(&col3rd_w.palDC);
        purge_palette_if_loaded(&col3rd_w.palCP3);

        break;

    case 3:
        purge_texture_if_loaded(&ppgTitleTex);

        break;

    case 4:
        purge_stage_bg_textures();

        purge_palette_if_loaded(&col3rd_w.palDC);
        purge_palette_if_loaded(&col3rd_w.palCP3);
        purge_texture_if_loaded(&ppgAkeTex);
        purge_texture_if_loaded(&ppgAkaneTex);
        purge_palette_if_loaded(&ppgAkePal);
        purge_palette_if_loaded(&ppgAkanePal);

        break;

    case 5:
        purge_multi_textures();

        purge_palette_if_loaded(&col3rd_w.palDC);
        purge_palette_if_loaded(&col3rd_w.palCP3);

        break;
    }
}

void ppgPurgeTextureFromVRAM(Texture* tex) {
    s32 i;
    s32 th;

    for (i = 0; i < tex->total; i++) {
        th = tex->handle[i].b16[0];
        if (th != 0) {
            // TODO do something here?
        }
    }
}

void ppgPurgePaletteFromVRAM(Palette* pal) {
    s32 i;
    s32 ph;

    for (i = 0; i < pal->total; i++) {
        ph = pal->handle[i];
        if (ph != 0) {
            // TODO do something here?
        }
    }
}
