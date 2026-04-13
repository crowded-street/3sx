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

typedef struct PSPTextureCache {
    void* pixels;
    unsigned int texture_handle;
    unsigned int palette_handle;
    size_t size;
    int render_width;
    int render_height;
} PSPTextureCache;

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
static unsigned int bound_texture_code = 0;
static PSPTextureCache texture_cache[FL_TEXTURE_MAX] = { 0 };

static unsigned int argb_to_abgr(unsigned int color) {
    return (color & 0xFF00FF00u) | ((color >> 16) & 0xFFu) | ((color & 0xFFu) << 16);
}

static int next_power_of_two(int value) {
    int result = 1;

    while (result < value) {
        result <<= 1;
    }

    return result;
}

static unsigned int rgba16_to_abgr8888(unsigned short pixel) {
    const unsigned int r = (pixel & 0x1Fu) * 255u / 31u;
    const unsigned int g = ((pixel >> 5) & 0x1Fu) * 255u / 31u;
    const unsigned int b = ((pixel >> 10) & 0x1Fu) * 255u / 31u;
    const unsigned int a = (pixel & 0x8000u) ? 0xFFu : 0x00u;
    return (a << 24) | (b << 16) | (g << 8) | r;
}

static unsigned int rgba32_to_abgr8888(unsigned int pixel) {
    return argb_to_abgr(pixel);
}

static unsigned int rgb24x_to_abgr8888(unsigned int pixel) {
    return argb_to_abgr(pixel | 0xFF000000u);
}

static int clut_shuffle(int index) {
    return (index & ~0x18) | (((index & 0x08) << 1) | ((index & 0x10) >> 1));
}

static void invalidate_texture_cache(int texture_index) {
    PSPTextureCache* cache = &texture_cache[texture_index];

    if (cache->pixels != NULL) {
        free(cache->pixels);
        cache->pixels = NULL;
    }

    cache->texture_handle = 0;
    cache->palette_handle = 0;
    cache->size = 0;
    cache->render_width = 0;
    cache->render_height = 0;

    if (LO_16_BITS(bound_texture_code) == (unsigned int)(texture_index + 1)) {
        bound_texture_code = 0;
    }
}

static void invalidate_all_texture_caches() {
    for (int i = 0; i < FL_TEXTURE_MAX; i++) {
        invalidate_texture_cache(i);
    }

    current_texture_code = 0;
    bound_texture_code = 0;
}

static void invalidate_other_texture_caches(int keep_texture_index) {
    for (int i = 0; i < FL_TEXTURE_MAX; i++) {
        if (i == keep_texture_index) {
            continue;
        }

        invalidate_texture_cache(i);
    }
}

static void invalidate_palette_caches(unsigned int palette_handle) {
    if ((palette_handle == 0) || (palette_handle > FL_PALETTE_MAX)) {
        return;
    }

    for (int i = 0; i < FL_TEXTURE_MAX; i++) {
        if (texture_cache[i].palette_handle == palette_handle) {
            invalidate_texture_cache(i);
        }
    }
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

static unsigned int palette_color_at(const FLTexture* palette, int index) {
    const void* pixels = palette_source_pixels(palette);

    if (pixels == NULL) {
        fatal_error("Missing palette pixel data");
    }

    if ((palette->width * palette->height) == 256) {
        index = clut_shuffle(index);
    }

    switch (palette->format) {
    case SCE_GS_PSMCT16:
        return rgba16_to_abgr8888(((const unsigned short*)pixels)[index]);

    case SCE_GS_PSMCT24:
        return rgb24x_to_abgr8888(((const unsigned int*)pixels)[index]);

    case SCE_GS_PSMCT32:
        return rgba32_to_abgr8888(((const unsigned int*)pixels)[index]);

    default:
        fatal_error("Unhandled PSP palette format: %u", palette->format);
    }
}

static int texture_palette_color_count(const FLTexture* palette) {
    const int color_count = palette->width * palette->height;

    if ((color_count != 16) && (color_count != 256)) {
        fatal_error("Unhandled PSP palette dimensions: %dx%d", palette->width, palette->height);
    }

    return color_count;
}

static void convert_indexed_texture(const FLTexture* texture, const FLTexture* palette, unsigned int* output) {
    const unsigned char* pixels = texture_source_pixels(texture);
    const int width = texture->width;
    const int height = texture->height;
    const int render_width = next_power_of_two(width);
    const int palette_color_count = texture_palette_color_count(palette);

    if (pixels == NULL) {
        fatal_error("Missing indexed texture pixel data");
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int color_index = 0;

            if (texture->format == SCE_GS_PSMT8) {
                color_index = pixels[y * width + x];
            } else if (texture->format == SCE_GS_PSMT4) {
                const unsigned char packed = pixels[y * (width / 2) + (x / 2)];
                color_index = (x & 1) ? (packed >> 4) : (packed & 0x0F);
            } else {
                fatal_error("Unhandled indexed PSP texture format: %u", texture->format);
            }

            if (color_index >= palette_color_count) {
                color_index = 0;
            }

            output[y * render_width + x] = palette_color_at(palette, color_index);
        }
    }
}

static void convert_direct_texture(const FLTexture* texture, unsigned int* output) {
    const void* pixels = texture_source_pixels(texture);
    const int width = texture->width;
    const int height = texture->height;
    const int render_width = next_power_of_two(width);

    if (pixels == NULL) {
        fatal_error("Missing direct texture pixel data");
    }

    switch (texture->format) {
    case SCE_GS_PSMCT16:
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                output[y * render_width + x] = rgba16_to_abgr8888(((const unsigned short*)pixels)[y * width + x]);
            }
        }
        break;

    case SCE_GS_PSMCT24:
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                output[y * render_width + x] = rgb24x_to_abgr8888(((const unsigned int*)pixels)[y * width + x]);
            }
        }
        break;

    case SCE_GS_PSMCT32:
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                output[y * render_width + x] = rgba32_to_abgr8888(((const unsigned int*)pixels)[y * width + x]);
            }
        }
        break;

    default:
        fatal_error("Unhandled PSP texture format: %u", texture->format);
    }
}

static void rebuild_texture_cache(unsigned int th) {
    const unsigned int texture_handle = LO_16_BITS(th);
    const unsigned int palette_handle = HI_16_BITS(th);
    const FLTexture* texture = &flTexture[texture_handle - 1];
    PSPTextureCache* cache = &texture_cache[texture_handle - 1];
    const int render_width = next_power_of_two(texture->width);
    const int render_height = next_power_of_two(texture->height);
    const size_t pixel_count = (size_t)render_width * (size_t)render_height;
    unsigned int* converted_pixels = NULL;

    if ((texture_handle == 0) || (texture_handle > FL_TEXTURE_MAX)) {
        fatal_error("Invalid PSP texture handle: %u", texture_handle);
    }

    invalidate_texture_cache(texture_handle - 1);

    converted_pixels = memalign(16, pixel_count * sizeof(unsigned int));

    if (converted_pixels == NULL) {
        invalidate_other_texture_caches((int)texture_handle - 1);
        converted_pixels = memalign(16, pixel_count * sizeof(unsigned int));
    }

    if (converted_pixels == NULL) {
        fatal_error("Failed to allocate PSP texture cache (%dx%d, %zu bytes)",
                    render_width,
                    render_height,
                    pixel_count * sizeof(unsigned int));
    }

    if ((texture->format == SCE_GS_PSMT4) || (texture->format == SCE_GS_PSMT8)) {
        if ((palette_handle == 0) || (palette_handle > FL_PALETTE_MAX)) {
            fatal_error("Missing palette for indexed texture");
        }

        convert_indexed_texture(texture, &flPalette[palette_handle - 1], converted_pixels);
    } else {
        convert_direct_texture(texture, converted_pixels);
    }

    sceKernelDcacheWritebackRange(converted_pixels, pixel_count * sizeof(unsigned int));

    cache->pixels = converted_pixels;
    cache->size = pixel_count * sizeof(unsigned int);
    cache->texture_handle = texture_handle;
    cache->palette_handle = palette_handle;
    cache->render_width = render_width;
    cache->render_height = render_height;
}

static void ensure_texture_ready(unsigned int th) {
    const unsigned int texture_handle = LO_16_BITS(th);
    const unsigned int palette_handle = HI_16_BITS(th);
    PSPTextureCache* cache = &texture_cache[texture_handle - 1];

    if ((cache->pixels != NULL) && (cache->palette_handle == palette_handle)) {
        return;
    }

    rebuild_texture_cache(th);
}

static void bind_current_texture() {
    const unsigned int texture_handle = LO_16_BITS(current_texture_code);
    const PSPTextureCache* cache = NULL;

    if (bound_texture_code == current_texture_code) {
        return;
    }

    if ((texture_handle == 0) || (texture_handle > FL_TEXTURE_MAX)) {
        fatal_error("Invalid PSP texture handle: %u", texture_handle);
    }

    cache = &texture_cache[texture_handle - 1];

    if (cache->pixels == NULL) {
        fatal_error("No PSP texture is currently bound");
    }

    sceGuTexMode(GU_PSM_8888, 0, 0, GU_FALSE);
    sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
    sceGuTexFilter(GU_NEAREST, GU_NEAREST);
    sceGuTexWrap(GU_CLAMP, GU_CLAMP);
    sceGuTexImage(0, cache->render_width, cache->render_height, cache->render_width, cache->pixels);
    bound_texture_code = current_texture_code;
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
        bind_current_texture();
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
        vertices[i].color = argb_to_abgr(color);
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
    invalidate_all_texture_caches();
}

void PSPRenderer_Shutdown() {
    if (!initialized) {
        return;
    }

    invalidate_all_texture_caches();
    sceGuDisplay(GU_FALSE);
    sceGuTerm();
    initialized = false;
    bound_texture_code = 0;
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
    bound_texture_code = 0;
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

    invalidate_texture_cache(texture_handle - 1);
}

void PSPRenderer_DestroyTexture(unsigned int texture_handle) {
    if ((texture_handle == 0) || (texture_handle > FL_TEXTURE_MAX)) {
        return;
    }

    invalidate_texture_cache(texture_handle - 1);
}

void PSPRenderer_UnlockTexture(unsigned int th) {
    const unsigned int texture_handle = LO_16_BITS(th);

    if ((texture_handle == 0) || (texture_handle > FL_TEXTURE_MAX)) {
        fatal_error("Invalid PSP texture handle: %u", texture_handle);
    }

    invalidate_texture_cache(texture_handle - 1);
}

void PSPRenderer_CreatePalette(unsigned int ph) {
    invalidate_palette_caches(HI_16_BITS(ph));
}

void PSPRenderer_DestroyPalette(unsigned int palette_handle) {
    invalidate_palette_caches(palette_handle);
}

void PSPRenderer_UnlockPalette(unsigned int ph) {
    invalidate_palette_caches(ph);
}

void PSPRenderer_SetTexture(unsigned int th) {
    ensure_texture_ready(th);
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
    draw_solid_quad_vertices(quad, color);
}

#endif
