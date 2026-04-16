#if CRS_VIDEO_DRIVER_PSP

#include "platform/video/psp/psp_renderer.h"

#include "common.h"
#include "port/utils.h"
#include "sf33rd/AcrSDK/common/plcommon.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#include <libgraph.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspkernel.h>

#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272
#define BUFFER_WIDTH 512
#define GAME_WIDTH 384
#define GAME_HEIGHT 224

#define DISPLAY_AREA_WIDTH 363
#define DISPLAY_AREA_HEIGHT SCREEN_HEIGHT
#define DISPLAY_OFFSET_X ((SCREEN_WIDTH - DISPLAY_AREA_WIDTH) / 2)
#define DISPLAY_OFFSET_Y 0

typedef struct PSPVertex {
    short u;
    short v;
    unsigned int color;
    float x;
    float y;
    float z;
} PSPVertex;

typedef struct PSPColorVertex {
    unsigned int color;
    float x;
    float y;
    float z;
} PSPColorVertex;

static unsigned int __attribute__((aligned(64))) display_list[0x40000];
static void* frame_buffers[2] = { NULL, NULL };
static void* depth_buffer = NULL;
static int current_back_buffer = 0;
static bool initialized = false;

static unsigned int current_texture_code = 0;
static void* current_texture_source = NULL;
static void* current_palette_source = NULL;

static unsigned int argb_to_abgr(unsigned int color) {
    return (color & 0xFF00FF00u) | ((color >> 16) & 0xFFu) | ((color & 0xFFu) << 16);
}

static const void* texture_source_pixels(const FLTexture* texture) {
    if (texture->wkVram != NULL) {
        return texture->wkVram;
    }

    if (texture->mem_handle != 0) {
        return flPS2GetSystemBuffAdrs(texture->mem_handle);
    }

    return NULL;
}

static const void* palette_source_pixels(const FLTexture* palette) {
    if (palette->wkVram != NULL) {
        return palette->wkVram;
    }

    if (palette->mem_handle != 0) {
        return flPS2GetSystemBuffAdrs(palette->mem_handle);
    }

    return NULL;
}

static const FLTexture* current_texture(void) {
    const unsigned int texture_handle = LO_16_BITS(current_texture_code);

    if ((texture_handle == 0) || (texture_handle > FL_TEXTURE_MAX)) {
        fatal_error("Invalid PSP texture handle: %u", texture_handle);
    }

    return &flTexture[texture_handle - 1];
}

static float snap_screen_coord(float value) {
    return (float)((int)value);
}

static float game_to_screen_x(float value) {
    return (float)DISPLAY_OFFSET_X + value * ((float)DISPLAY_AREA_WIDTH / (float)GAME_WIDTH);
}

static float game_to_screen_y(float value) {
    return (float)DISPLAY_OFFSET_Y + value * ((float)DISPLAY_AREA_HEIGHT / (float)GAME_HEIGHT);
}

static int display_scissor_left(void) {
    return DISPLAY_OFFSET_X;
}

static int display_scissor_top(void) {
    return DISPLAY_OFFSET_Y;
}

static int display_scissor_right(void) {
    return DISPLAY_OFFSET_X + DISPLAY_AREA_WIDTH;
}

static int display_scissor_bottom(void) {
    return DISPLAY_OFFSET_Y + DISPLAY_AREA_HEIGHT;
}

static short texel_coord(float normalized, float extent) {
    return (short)(normalized * extent + 0.5f);
}

static unsigned int ps2_to_psp_format(int ps2_format) {
    switch (ps2_format) {
    case SCE_GS_PSMCT16:
        return GU_PSM_5551;
    case SCE_GS_PSMCT24:
    case SCE_GS_PSMCT32:
        return GU_PSM_8888;
    case SCE_GS_PSMT8:
        return GU_PSM_T8;
    case SCE_GS_PSMT4:
        return GU_PSM_T4;
    default:
        fatal_error("Unhandled PSP texture format: %d", ps2_format);
    }
}

static void setup_draw_state(bool textured) {
    const int game_left = display_scissor_left();
    const int game_top = display_scissor_top();
    const int game_right = display_scissor_right();
    const int game_bottom = display_scissor_bottom();

    sceGuScissor(game_left, game_top, game_right, game_bottom);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuEnable(GU_BLEND);
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
    sceGuEnable(GU_ALPHA_TEST);
    sceGuAlphaFunc(GU_GREATER, 0, 0xFF);
    sceGuEnable(GU_DEPTH_TEST);
    sceGuDepthFunc(GU_GEQUAL);

    if (textured) {
        sceGuEnable(GU_TEXTURE_2D);
    } else {
        sceGuDisable(GU_TEXTURE_2D);
    }
}

static void fill_textured_vertices(PSPVertex* vertices, const Sprite* sprite, unsigned int color) {
    const FLTexture* texture = current_texture();
    const float texture_width = (float)texture->width;
    const float texture_height = (float)texture->height;

    for (int i = 0; i < 4; i++) {
        vertices[i].u = texel_coord(sprite->t[i].s, texture_width);
        vertices[i].v = texel_coord(sprite->t[i].t, texture_height);
        vertices[i].color = argb_to_abgr(color);
        vertices[i].x = snap_screen_coord(game_to_screen_x(sprite->v[i].x));
        vertices[i].y = snap_screen_coord(game_to_screen_y(sprite->v[i].y));
        vertices[i].z = flPS2ConvScreenFZ(sprite->v[i].z);
    }
}

static void draw_textured_quad(const Sprite* sprite, unsigned int color) {
    PSPVertex* vertices = sceGuGetMemory(4 * sizeof(PSPVertex));

    fill_textured_vertices(vertices, sprite, color);
    setup_draw_state(true);
    sceGuDrawArray(
        GU_TRIANGLE_STRIP, GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 4, 0, vertices);
}

static void draw_textured_sprite_rect(float x0, float y0, float z0, float s0, float t0, float x1, float y1, float s1,
                                      float t1, unsigned int color) {
    const FLTexture* texture = current_texture();
    const float texture_width = (float)texture->width;
    const float texture_height = (float)texture->height;
    PSPVertex* vertices = sceGuGetMemory(2 * sizeof(PSPVertex));
    const unsigned int abgr = argb_to_abgr(color);

    vertices[0].u = texel_coord(s0, texture_width);
    vertices[0].v = texel_coord(t0, texture_height);
    vertices[0].color = abgr;
    vertices[0].x = snap_screen_coord(game_to_screen_x(x0));
    vertices[0].y = snap_screen_coord(game_to_screen_y(y0));
    vertices[0].z = flPS2ConvScreenFZ(z0);

    vertices[1].u = texel_coord(s1, texture_width);
    vertices[1].v = texel_coord(t1, texture_height);
    vertices[1].color = abgr;
    vertices[1].x = snap_screen_coord(game_to_screen_x(x1));
    vertices[1].y = snap_screen_coord(game_to_screen_y(y1));
    vertices[1].z = flPS2ConvScreenFZ(z0);

    setup_draw_state(true);
    sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, vertices);
}

static void submit_solid_quad_vertices(const PSPColorVertex* vertices, bool full_screen_scissor) {
    if (full_screen_scissor) {
        sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        sceGuEnable(GU_SCISSOR_TEST);
        sceGuDisable(GU_TEXTURE_2D);
        sceGuDisable(GU_BLEND);
        sceGuDisable(GU_ALPHA_TEST);
        sceGuDisable(GU_DEPTH_TEST);
    } else {
        setup_draw_state(false);
    }

    sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_2D, 4, 0, vertices);
}

static void draw_solid_quad_vertices(const Quad* quad, unsigned int color) {
    PSPColorVertex* vertices = sceGuGetMemory(4 * sizeof(PSPColorVertex));

    for (int i = 0; i < 4; i++) {
        vertices[i].color = color;
        vertices[i].x = snap_screen_coord(game_to_screen_x(quad->v[i].x));
        vertices[i].y = snap_screen_coord(game_to_screen_y(quad->v[i].y));
        vertices[i].z = flPS2ConvScreenFZ(quad->v[i].z);
    }

    submit_solid_quad_vertices(vertices, false);
}

static void draw_black_bar(float x0, float y0, float x1, float y1) {
    PSPColorVertex* vertices = sceGuGetMemory(4 * sizeof(PSPColorVertex));
    const unsigned int black_color = 0xFF000000;

    vertices[0].color = black_color;
    vertices[0].x = x0;
    vertices[0].y = y0;
    vertices[0].z = 0.0f;

    vertices[1].color = black_color;
    vertices[1].x = x1;
    vertices[1].y = y0;
    vertices[1].z = 0.0f;

    vertices[2].color = black_color;
    vertices[2].x = x0;
    vertices[2].y = y1;
    vertices[2].z = 0.0f;

    vertices[3].color = black_color;
    vertices[3].x = x1;
    vertices[3].y = y1;
    vertices[3].z = 0.0f;

    submit_solid_quad_vertices(vertices, true);
}

static void draw_pillarbox_bars(void) {
    const float left_width = (float)display_scissor_left();
    const float right_start = (float)display_scissor_right();

    if (left_width > 0.0f) {
        draw_black_bar(0.0f, 0.0f, left_width, (float)SCREEN_HEIGHT);
    }

    if (right_start < (float)SCREEN_WIDTH) {
        draw_black_bar(right_start, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
    }
}

void PSPRenderer_Init() {
    if (initialized) {
        return;
    }

    sceGuInit();

    frame_buffers[0] = guGetStaticVramBuffer(BUFFER_WIDTH, SCREEN_HEIGHT, GU_PSM_8888);
    frame_buffers[1] = guGetStaticVramBuffer(BUFFER_WIDTH, SCREEN_HEIGHT, GU_PSM_8888);
    depth_buffer = guGetStaticVramBuffer(BUFFER_WIDTH, SCREEN_HEIGHT, GU_PSM_4444);

    sceGuStart(GU_DIRECT, display_list);
    sceGuDrawBuffer(GU_PSM_8888, frame_buffers[0], BUFFER_WIDTH);
    sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, frame_buffers[1], BUFFER_WIDTH);
    sceGuDepthBuffer(depth_buffer, BUFFER_WIDTH);
    sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
    sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuDisable(GU_CULL_FACE);
    sceGuDisable(GU_LIGHTING);
    sceGuDisable(GU_CLIP_PLANES);
    sceGuEnable(GU_BLEND);
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
    sceGuEnable(GU_ALPHA_TEST);
    sceGuAlphaFunc(GU_GREATER, 0, 0xFF);
    sceGuEnable(GU_DEPTH_TEST);
    sceGuDepthFunc(GU_GEQUAL);
    sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
    sceGuTexFilter(GU_NEAREST, GU_NEAREST);
    sceGuTexWrap(GU_CLAMP, GU_CLAMP);
    sceGuEnable(GU_TEXTURE_2D);
    sceGuFinish();
    sceGuSync(GU_SYNC_FINISH, GU_SYNC_WHAT_DONE);
    sceDisplayWaitVblankStart();
    sceGuDisplay(GU_TRUE);

    initialized = true;
    current_back_buffer = 0;
}

void PSPRenderer_Shutdown() {
    if (!initialized) {
        return;
    }

    sceGuDisplay(GU_FALSE);
    sceGuTerm();
    initialized = false;

    current_texture_code = 0;
    current_texture_source = NULL;
    current_palette_source = NULL;
}

void PSPRenderer_BeginFrame() {
    const unsigned int clear_color = argb_to_abgr(flPs2State.FrameClearColor);
    const int game_left = display_scissor_left();
    const int game_top = display_scissor_top();
    const int game_right = display_scissor_right();
    const int game_bottom = display_scissor_bottom();

    sceGuStart(GU_DIRECT, display_list);
    sceGuDrawBufferList(GU_PSM_8888, frame_buffers[current_back_buffer], BUFFER_WIDTH);
    sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
    sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuClearColor(0xFF000000);
    sceGuClearDepth(0);
    sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

    sceGuScissor(game_left, game_top, game_right, game_bottom);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuClearColor(clear_color);
    sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

    current_texture_code = 0;
    current_texture_source = NULL;
    current_palette_source = NULL;
}

void PSPRenderer_RenderFrame() {
    // Do nothing
}

void PSPRenderer_EndFrame() {
    draw_pillarbox_bars();
    sceGuFinish();
    sceGuSync(GU_SYNC_FINISH, GU_SYNC_WHAT_DONE);
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
    current_back_buffer ^= 1;
}

void PSPRenderer_CreateTexture(unsigned int th) {
    const unsigned int texture_handle = LO_16_BITS(th);

    if ((texture_handle == 0) || (texture_handle > FL_TEXTURE_MAX)) {
        fatal_error("Invalid PSP texture handle: %u", texture_handle);
    }
}

void PSPRenderer_DestroyTexture(unsigned int texture_handle) {
    if ((texture_handle == 0) || (texture_handle > FL_TEXTURE_MAX)) {
        return;
    }
}

void PSPRenderer_UnlockTexture(unsigned int th) {
    const unsigned int texture_handle = LO_16_BITS(th);

    if ((texture_handle == 0) || (texture_handle > FL_TEXTURE_MAX)) {
        fatal_error("Invalid PSP texture handle: %u", texture_handle);
    }
}

void PSPRenderer_CreatePalette(unsigned int ph) {
    // Do nothing
}

void PSPRenderer_DestroyPalette(unsigned int palette_handle) {
    // Do nothing
}

void PSPRenderer_UnlockPalette(unsigned int ph) {
    // Do nothing
}

void PSPRenderer_SetTexture(unsigned int th) {
    int texture_handle = LO_16_BITS(th) - 1;
    FLTexture* flTex = &flTexture[texture_handle];
    int palette_handle = HI_16_BITS(th) - 1;
    FLTexture* flPal = &flPalette[palette_handle];

    void* texture_source = texture_source_pixels(flTex);
    void* palette_source = palette_source_pixels(flPal);

    if (current_palette_source != palette_source) {
        sceGuClutMode(GU_PSM_5551, 0, 255, 0);
        sceGuClutLoad(flPal->size / 16, palette_source);
        current_palette_source = palette_source;
    }

    if (current_texture_source != texture_source) {
        sceGuTexMode(ps2_to_psp_format(flTex->format), 0, 0, GU_FALSE);
        sceGuTexImage(0, flTex->width, flTex->height, flTex->width, texture_source);
        current_texture_source = texture_source;
    }

    current_texture_code = th;
}

void PSPRenderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color) {
    draw_textured_quad(sprite, color);
}

void PSPRenderer_DrawSprite(const Sprite* sprite, unsigned int color) {
    draw_textured_sprite_rect(sprite->v[0].x,
                              sprite->v[0].y,
                              sprite->v[0].z,
                              sprite->t[0].s,
                              sprite->t[0].t,
                              sprite->v[3].x,
                              sprite->v[3].y,
                              sprite->t[3].s,
                              sprite->t[3].t,
                              color);
}

void PSPRenderer_DrawSprite2(const Sprite2* sprite2) {
    draw_textured_sprite_rect(sprite2->v[0].x,
                              sprite2->v[0].y,
                              sprite2->v[0].z,
                              sprite2->t[0].s,
                              sprite2->t[0].t,
                              sprite2->v[1].x,
                              sprite2->v[1].y,
                              sprite2->t[1].s,
                              sprite2->t[1].t,
                              sprite2->vertex_color);
}

void PSPRenderer_DrawSolidQuad(const Quad* quad, unsigned int color) {
    draw_solid_quad_vertices(quad, argb_to_abgr(color));
}

#endif
