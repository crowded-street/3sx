/**
 * @file op_sub.c
 * Opening Subroutines
 */

#include "sf33rd/Source/Game/opening/op_sub.h"
#include "common.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Game/RAMCNT.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/rendering/texcash.h"

#include "sf33rd/Source/Game/opening/opening.h"

void TexRelease(u32 G_Num) {
    if (G_Num == 601) {
        title_tex_flag = 0;
    }

    switch (G_Num) {
    case 590:
        ppgReleaseTextureHandle(&ppgWarTex, -1);
        ppgReleasePaletteHandle(&ppgWarPal, -1);
        ppgReleasePaletteHandle(&ppgAdxPal, -1);
        break;

    case 600:
        ppgReleaseTextureHandle(&ppgCapLogoTex, -1);
        ppgReleasePaletteHandle(&ppgCapLogoPal, -1);
        break;

    case 601:
        ppgReleaseTextureHandle(&ppgTitleTex, -1);
        break;
    }
}

void TexRelease_OP() {
    Purge_memory_of_kind_of_key(0x1D);
    ppgSourceDataReleased(&ppgOpnBgList);
    ppgReleaseTextureHandle(&ppgOpnBgTex, -1);
    reset_dma_group(0x8C40);
    purge_texcash_work(9);
}

void put_chr2(OPTW* optw) {
    Vertex tex[4];

    if (No_Trans) {
        return;
    }

    tex[0].x = Frame_Zoom_X * ((s32)optw->off_x * optw->zx);
    tex[0].y = Frame_Zoom_Y * ((s32)optw->off_y * optw->zy);
    tex[3].x = Frame_Zoom_X * ((optw->off_x + 0x100) * optw->zx);
    tex[3].y = Frame_Zoom_Y * ((optw->off_y + 0x100) * optw->zy);
    tex[0].z = tex[3].z = PrioBase[optw->prio];
    ppgWriteQuadUseTrans(tex, optw->col.full, NULL, optw->g_no, -1, optw->hv, 300);
}

void opbg_trans(OPBW* opbw, s16 x, s16 y) {
    OPTW optw;
    s16 i;
    s16 i0;
    s16 i1;
    s16 j;
    s16 j0;
    s16 j1;
    s16 xx;
    s16 yy;

    xx = (x < 0) ? 0 : x;
    yy = (y < 0) ? 0 : y;
    xx = (x > 1023) ? 1023 : xx;
    yy = (y > 1023) ? 1023 : yy;

    i0 = xx >> 8;
    i1 = ((xx + 383) >> 8) & 3;
    j1 = ((yy + 223) >> 8) & 3;
    j0 = yy >> 8;

    for (i = i0; i <= i1; i++) {
        for (j = j0; j <= j1; j++) {
            if (opbw->map[i][j].g_no != 0) {
                optw.g_no = opbw->map[i][j].g_no;
                optw.hv = opbw->map[i][j].hv;
                optw.off_x = (i << 8) - x;
                optw.off_y = (y + 224) - ((j << 8) + 256);
                optw.zx = optw.zy = scr_sc;
                optw.prio = opbw->prio;
                optw.trans = opbw->map[i][j].trans;
                optw.col.full = opbw->map[i][j].col.full;
                put_chr2(&optw);
            }
        }
    }
}

void COLOR_COPYn(s16 dst, s16 colcd, s16 n) {
    s16* colram = (s16*)ColorRAM;
    njSetPaletteData(dst * 16, n * 16, &colram[colcd * 16]);
}
