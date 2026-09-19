/**
 * @file mtrans_pool.c
 * The multi-texture pool's lifecycle and bookkeeping: setting a pool up,
 * ageing its patterns out, and collecting the tiles a released pattern frees.
 *
 * Split out of mtrans.c. Nothing here changed on the way across, and none of it
 * is reached from the transfer passes except through makeup_tpu_free, which was
 * already public.
 */

#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "structs.h"

static void collect_used_x16_tile_row(s16 i, s16 j, PatternMap* map) {
    s16 k;

    if (map->x16_map[i][j] == 0) {
        return;
    }

    for (k = 0; k < 16; k++) {
        if (!((1 << k) & map->x16_map[i][j])) {
            continue;
        }

        tpu_free->x16_used[tpu_free->x16] = (i * 256) + (j * 16) + k;
        tpu_free->x16 += 1;
    }
}

static void collect_used_x16_tiles(s32 x16, PatternMap* map) {
    s16 i;
    s16 j;

    for (i = 0; i < x16; i++) {
        for (j = 0; j < 16; j++) {
            collect_used_x16_tile_row(i, j, map);
        }
    }
}

static void collect_used_x32_tile_row(s16 i, s16 j, PatternMap* map) {
    s16 k;

    if (map->x32_map[i][j] == 0) {
        return;
    }

    for (k = 0; k < 8; k++) {
        if (!(map->x32_map[i][j] & (1 << k))) {
            continue;
        }

        tpu_free->x32_used[tpu_free->x32] = (i * 64) + (j * 8) + k;
        tpu_free->x32 += 1;
    }
}

static void collect_used_x32_tiles(s32 x32, PatternMap* map) {
    s16 i;
    s16 j;

    for (i = 0; i < x32; i++) {
        for (j = 0; j < 8; j++) {
            collect_used_x32_tile_row(i, j, map);
        }
    }
}

void makeup_tpu_free(s32 x16, s32 x32, PatternMap* map) {
    tpu_free->x16 = 0;
    tpu_free->x32 = 0;

    collect_used_x16_tiles(x16, map);
    collect_used_x32_tiles(x32, map);
}

void mlt_obj_trans_init(MultiTexture* mt, s32 mode, u8* adrs) {
    PatternState* mc;
    PPGFileHeader ppg;
    s32 i;

    ppg.width = ppg.height = 16;
    ppg.compress = 0;
    ppg.formARGB = 0x1555;
    ppg.transNums = 0;
    mt->texList.tex = &mt->tex;

    switch (mode & 7) {
    case 4:
        ppg.pixel = 0x82;
        mt->texList.pal = NULL;
        break;

    case 2:
        ppg.pixel = 0x81;
        mt->texList.pal = palGetChunkGhostCP3();
        break;

    default:
        ppg.pixel = 0x81;
        mt->texList.pal = palGetChunkGhostDC();
        break;
    }

    mt->texList.tex->be = 0;
    ppgSetupTexChunkSeqs(&mt->tex, &(PPGTexSeqsArgs){&ppg, adrs, mt->mltgidx16, mt->mltnum, mt->attribute});

    if (!(mode & 0x20)) {
        mc = mt->mltcsh16;

        for (i = 0; i < mt->mltnum16; i++) {
            mc->time = 0;
            mc->cs.code = -1;
            mc++;
        }

        mc = mt->mltcsh32;

        for (i = 0; i < mt->mltnum32; i++) {
            mc->time = 0;
            mc->cs.code = -1;
            mc++;
        }
    }
}

static void release_expired_pattern(PatternState* mc, u32 unused_code) {
    if (mc->time) {
        if (--mc->time == 0) {
            mc->cs.code = unused_code;
        }
    }
}

void mlt_obj_trans_update(MultiTexture* mt) {
    s32 i;
    PatternState* mc;

    PatternState* assign1;
    PatternState* assign2;

    for (mc = mt->mltcsh16, i = 0; i < mt->mltnum16; i++, mc += 1, assign1 = mc) {
        release_expired_pattern(mc, -1);
    }

    for (mc = mt->mltcsh32, i = 0; i < mt->mltnum32; i++, mc += 1, assign2 = mc) {
        release_expired_pattern(mc, -1U);
    }
}
