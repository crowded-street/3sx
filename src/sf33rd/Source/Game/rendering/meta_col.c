/**
 * @file meta_col.c
 * Palette-swapping and color-changing effects for Resurrection/X.C.O.P.Y.
 */

#include "sf33rd/Source/Game/rendering/meta_col.h"
#include "common.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"

// bss
u16 hi_meta[2][2][64];
u16 metamor_original[2][5][64];

// rodata
const s16 mcs_sel_tbl[2] = { 0x1F8, 0x1FC };

void metamor_color_trans(s16 wkid) {
    s16 i;

    for (i = 0; i < 64; i++) {
        ColorRAM[wkid * 16][i] = palConvSrcToRam(hi_meta[wkid][0][i]);
        ColorRAM[(wkid * 16) + 8][i] = palConvSrcToRam(hi_meta[wkid][1][i]);
    }

    palUpdateGhostCP3(wkid * 16, 1);
    palUpdateGhostCP3((wkid * 16) + 8, 1);
}

void metamor_color_copy(s16 wkid) {
    s16 from_id;
    s16 to_id;
    s16 i;
    s16 j;

    from_id = (((wkid == 0) * 0x10) + 1);
    to_id = (((wkid == 1) * 0x10) + 1);

    for (i = 0; i < 6; i++) {
        for (j = 0; j < 64; j++) {
            ColorRAM[to_id + i][j] = ColorRAM[from_id + i][j];
            ColorRAM[to_id + i + 8][j] = ColorRAM[from_id + i + 8][j];
        }
    }

    for (j = 0; j < 64; j++) {
        ColorRAM[mcs_sel_tbl[wkid]][j] = ColorRAM[mcs_sel_tbl[(wkid + 1) & 1]][j];
    }

    palUpdateGhostCP3(to_id, 6);
    palUpdateGhostCP3(to_id + 8, 6);
    palUpdateGhostCP3(mcs_sel_tbl[wkid], 1);
}

void metamor_color_store(s16 wkid) {
    s16 i;
    s16 j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 64; j++) {
            metamor_original[wkid][i][j] = ColorRAM[i + (wkid * 16)][j];
        }
    }

    for (j = 0; j < 64; j++) {
        metamor_original[wkid][i][j] = ColorRAM[mcs_sel_tbl[wkid]][j];
    }
}

void metamor_color_restore(s16 wkid) {
    s16 i;
    s16 j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 64; j++) {
            ColorRAM[i + (wkid * 16)][j] = metamor_original[wkid][i][j];
            ColorRAM[i + ((wkid * 16) + 8)][j] = metamor_original[wkid][i][j];
        }
    }

    for (j = 0; j < 64; j++) {
        ColorRAM[mcs_sel_tbl[wkid]][j] = metamor_original[wkid][i][j];
    }

    palUpdateGhostCP3(wkid * 16, 4);
    palUpdateGhostCP3((wkid * 16) + 8, 4);
    palUpdateGhostCP3(mcs_sel_tbl[wkid], 1);
}
