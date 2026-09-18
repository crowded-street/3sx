/**
 * @file mtrans_seqs.c
 * The sequence chip queue: one frame's worth of sprite fragments, and the
 * priority base the transfer passes write their depth into.
 *
 * Split out of mtrans.c, which was carrying 1813 lines. Nothing here changed
 * on the way across.
 */

#include "sf33rd/Source/Game/rendering/mtrans_seqs.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "structs.h"

#include "core/renderer.h"

#include <SDL3/SDL.h>

SpriteChipSet seqs_w;
f32 PrioBase[PRIO_BASE_SIZE];
f32 PrioBaseOriginal[PRIO_BASE_SIZE];

void appSetupBasePriority() {
    s32 i;

    for (i = 0; i < PRIO_BASE_SIZE; i++) {
        PrioBaseOriginal[i] = ((i * 512) + 1) / 65535.0f;
    }
}

void appSetupTempPriority() {
    s32 i;

    for (i = 0; i < PRIO_BASE_SIZE; i++) {
        PrioBase[i] = PrioBaseOriginal[i];
    }
}

void appRenewTempPriority_1_Chip() {
    njTranslate(NULL, 0, 0, 1.0f / 65536.0f); // 1 / 2^(-16)
}

void appRenewTempPriority(s32 z) {
    MTX mtx;
    njGetMatrix(&mtx);
    PrioBase[z] = mtx.a[3][2];
}

void seqsInitialize(void* adrs) {
    if (adrs == NULL) {
        while (1) {
            // Do nothing
        }
    }

    seqs_w.chip = (Sprite2*)adrs;
    seqs_w.sprMax = 0;
}

u16 seqsGetSprMax() {
    return seqs_w.sprMax;
}

u32 seqsGetUseMemorySize() {
    return 0xD000;
}

void seqsBeforeProcess() {
    s32 i;

    seqs_w.sprTotal = 0;

    // FIXME: Extract 24 into a define
    for (i = 0; i < 24; i++) {
        seqs_w.up[i] = 0;
    }
}

static bool should_refresh_sequence_texture(s32 i) {
    return seqs_w.up[i] && (ppgRenewTexChunkSeqs(mts[i].texList.tex) == 0);
}

static void draw_queued_sequence_chips(void) {
    s32 i;
    u32 keep = 0;
    u32 val = 0;

    for (i = 0; i < seqs_w.sprTotal; i++) {
        if (seqs_w.up[seqs_w.chip[i].id]) {
            val = seqs_w.chip[i].tex_code;

            if (keep != val) {
                keep = val;
                flSetRenderState(FLRENDER_TEXSTAGE0, val);
            }

            Renderer_DrawSprite2(&seqs_w.chip[i]);
        }
    }
}

void seqsAfterProcess() {
    s32 i;

    if (seqs_w.sprTotal == 0) {
        return;
    }

    for (i = 0; i < 24; i++) {
        if (should_refresh_sequence_texture(i)) {
            seqs_w.up[i] = 0;
        }
    }

    if (seqs_w.sprMax < seqs_w.sprTotal) {
        seqs_w.sprMax = seqs_w.sprTotal;
    }

    draw_queued_sequence_chips();
}

static bool is_sequence_chip_outside_view(Sprite2* chip) {
    return (chip->v[0].x >= 384.0f) || (chip->v[1].x < 0.0f) || (chip->v[0].y >= 224.0f) || (chip->v[1].y < 0.0f);
}

static bool is_sequence_chip_scaled(Sprite2* chip, s32 width, s32 height) {
    const f32 screen_w = SDL_fabsf(chip->v[1].x - chip->v[0].x);
    const f32 screen_h = SDL_fabsf(chip->v[0].y - chip->v[1].y);

    return SDL_fabsf(screen_w - (f32)width) > 0.001f || SDL_fabsf(screen_h - (f32)height) > 0.001f;
}

s32 seqsStoreChip(f32 x, f32 y, s32 w, s32 h, s32 gix, s32 code, s32 attr, s32 alpha, s32 id) {
    Sprite2* chip;
    s32 u;
    s32 v;

    chip = &seqs_w.chip[seqs_w.sprTotal];
    chip->v[0].x = x;
    chip->v[0].y = y;
    chip->v[1].x = x + w;
    chip->v[1].y = y - h;
    chip->v[0].z = chip->v[1].z = 0.0f;
    njCalcPoint(NULL, &chip->v[0], &chip->v[0]);
    njCalcPoint(NULL, &chip->v[1], &chip->v[1]);

    if (is_sequence_chip_outside_view(chip)) {
        return 1;
    }

    if (!(attr & 0x2000)) {
        u = (code & 0xF) * 16;
        v = code & 0xF0;
        chip->tex_code = ppgGetUsingTextureHandle(NULL, gix + (code >> 8));
    } else {
        u = (code & 7) * 32;
        v = (code & 0x38) * 4;
        chip->tex_code = ppgGetUsingTextureHandle(NULL, gix + (code >> 6));
    }

    appRenewTempPriority_1_Chip();

    const bool scaled = is_sequence_chip_scaled(chip, w, h);

    const f32 uv_dx = scaled ? 0.5f : 0.0f;
    const f32 uv_dy = scaled ? 0.5f : 0.0f;

    if (attr & 0x8000) {
        chip->t[1].s = (u + uv_dx) / 256.0f;
        chip->t[0].s = (u + w - uv_dx) / 256.0f;
    } else {
        chip->t[0].s = (u + uv_dx) / 256.0f;
        chip->t[1].s = (u + w - uv_dx) / 256.0f;
    }

    if (attr & 0x4000) {
        chip->t[1].t = (v + uv_dy) / 256.0f;
        chip->t[0].t = (v + h - uv_dy) / 256.0f;
    } else {
        chip->t[0].t = (v + uv_dy) / 256.0f;
        chip->t[1].t = (v + h - uv_dy) / 256.0f;
    }

    chip->tex_code |= ppgGetUsingPaletteHandle(NULL, attr & 0x1FF) << 16;
    chip->vertex_color = curr_bright | ((0xFF - alpha) << 24);
    chip->id = id;
    seqs_w.sprTotal += 1;

    if (seqs_w.sprTotal > 0x400) {
        // The number of OBJ fragments has exceeded the planned number
        flLogOut("ＯＢＪの破片が予定数を越えてしまいました");
        while (1) {}
    }

    return 1;
}

