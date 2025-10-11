#include "port/sdl/sdl_game_renderer.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#include <libgraph.h>

#include <SDL3/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#define RENDER_TASK_MAX 1024

typedef struct RenderTask {
    SDL_Texture* texture;
    SDL_Vertex vertices[4];
    float z;
    int index;
} RenderTask;

SDL_Texture* cps3_canvas = NULL;

static const int cps3_width = 384;
static const int cps3_height = 224;

static SDL_Renderer* _renderer = NULL;
static SDL_Surface* surfaces[FL_TEXTURE_MAX] = { NULL };
static SDL_Palette* palettes[FL_PALETTE_MAX] = { NULL };
static SDL_Texture* textures[FL_PALETTE_MAX] = { NULL };
static int texture_count = 0;
static SDL_Texture* texture_cache[FL_TEXTURE_MAX][FL_PALETTE_MAX + 1] = { { NULL } };
static SDL_Texture* textures_to_destroy[1024] = { NULL };
static int textures_to_destroy_count = 0;
static RenderTask render_tasks[RENDER_TASK_MAX] = { 0 };
static int render_task_count = 0;

// Debugging

static bool draw_rect_borders = false;
static bool dump_textures = false;

static int texture_index = 0;

static void save_texture(const SDL_Surface* surface, const SDL_Palette* palette) {
    char filename[128];
    sprintf(filename, "textures/%d.tga", texture_index);

    const Uint8* pixels = surface->pixels;
    const int width = surface->w;
    const int height = surface->h;

    FILE* f = fopen(filename, "wb");

    if (!f) {
        return;
    }

    uint8_t header[18] = { 0 };
    header[2] = 2; // uncompressed RGB
    header[12] = width & 0xFF;
    header[13] = (width >> 8) & 0xFF;
    header[14] = height & 0xFF;
    header[15] = (height >> 8) & 0xFF;
    header[16] = 32;   // bits per pixel
    header[17] = 0x20; // top-left origin

    fwrite(header, 1, 18, f);

    // Write pixels in BGRA format
    for (int i = 0; i < width * height; ++i) {
        Uint8 index = pixels[i];

        switch (palette->ncolors) {
        case 16:
            if (i & 1) {
                index >>= 4;
            } else {
                index &= 0xF;
            }

            break;

        case 256:
            break;
        }

        const SDL_Color* color = &palette->colors[index];
        const Uint8 bgr[] = { color->b, color->g, color->r, color->a };
        fwrite(bgr, 1, 4, f);
    }

    fclose(f);
    texture_index += 1;
}

// Textures

static void push_texture(SDL_Texture* texture) {
    textures[texture_count] = texture;
    texture_count += 1;
}

static SDL_Texture* get_texture() {
    if (texture_count == 0) {
        fatal_error("No textures to get");
    }

    return textures[texture_count - 1];
}

static void push_texture_to_destroy(SDL_Texture* texture) {
    textures_to_destroy[textures_to_destroy_count] = texture;
    textures_to_destroy_count += 1;
}

static void destroy_textures() {
    for (int i = 0; i < texture_count; i++) {
        textures[i] = NULL;
    }

    texture_count = 0;

    for (int i = 0; i < textures_to_destroy_count; i++) {
        SDL_DestroyTexture(textures_to_destroy[i]);
    }

    textures_to_destroy_count = 0;
}

static void push_render_task(RenderTask* task) {
    memcpy(&render_tasks[render_task_count], task, sizeof(RenderTask));
    render_task_count += 1;
}

static void clear_render_tasks() {
    SDL_zeroa(render_tasks);
    render_task_count = 0;
}

static int compare_render_tasks(const RenderTask* a, const RenderTask* b) {
    if (a->z < b->z) {
        return -1;
    } else if (a->z > b->z) {
        return 1;
    } else {
        // This eliminates z-fighting
        if (a->index < b->index) {
            return 1;
        } else if (a->index > b->index) {
            return -1;
        } else {
            return 0;
        }
    }
}

// Colors

#define clut_shuf(x) (((x) & ~0x18) | ((((x) & 0x08) << 1) | (((x) & 0x10) >> 1)))

static void read_rgba32_color(Uint32 pixel, SDL_Color* color) {
    color->b = pixel & 0xFF;
    color->g = (pixel >> 8) & 0xFF;
    color->r = (pixel >> 16) & 0xFF;
    color->a = (pixel >> 24) & 0xFF;
}

static void read_rgba32_fcolor(Uint32 pixel, SDL_FColor* fcolor) {
    SDL_Color color;

    read_rgba32_color(pixel, &color);
    fcolor->r = (float)color.r / 255;
    fcolor->g = (float)color.g / 255;
    fcolor->b = (float)color.b / 255;
    fcolor->a = (float)color.a / 255;
}

static void read_rgba16_color(Uint16 pixel, SDL_Color* color) {
    color->r = (pixel & 0x1F) * 255 / 31;
    color->g = ((pixel >> 5) & 0x1F) * 255 / 31;
    color->b = ((pixel >> 10) & 0x1F) * 255 / 31;
    color->a = (pixel & 0x8000) ? 255 : 0;
}

static void read_color(void* pixels, int index, size_t color_size, SDL_Color* color) {
    switch (color_size) {
    case 2: {
        const Uint16* rgba16_colors = (Uint16*)pixels;
        read_rgba16_color(rgba16_colors[index], color);
        break;
    }

    case 4: {
        const Uint32* rgba32_colors = (Uint32*)pixels;
        read_rgba32_color(rgba32_colors[index], color);
        break;
    }
    }
}

#define LERP_FLOAT(a, b, x) ((a) * (1 - (x)) + (b) * (x))

static void lerp_fcolors(SDL_FColor* dest, const SDL_FColor* a, const SDL_FColor* b, float x) {
    dest->r = LERP_FLOAT(a->r, b->r, x);
    dest->g = LERP_FLOAT(a->g, b->g, x);
    dest->b = LERP_FLOAT(a->b, b->b, x);
    dest->a = LERP_FLOAT(a->a, b->a, x);
}

// Lifecycle

void SDLGameRenderer_Init(SDL_Renderer* renderer) {
    _renderer = renderer;
    cps3_canvas =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, cps3_width, cps3_height);
    SDL_SetTextureScaleMode(cps3_canvas, SDL_SCALEMODE_NEAREST);
}

void SDLGameRenderer_BeginFrame() {
    // Clear canvas
    const Uint8 r = (flPs2State.FrameClearColor >> 16) & 0xFF;
    const Uint8 g = (flPs2State.FrameClearColor >> 8) & 0xFF;
    const Uint8 b = flPs2State.FrameClearColor & 0xFF;
    const Uint8 a = flPs2State.FrameClearColor >> 24;

    if (a != SDL_ALPHA_TRANSPARENT) {
        SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    } else {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    }

    SDL_SetRenderTarget(_renderer, cps3_canvas);
    SDL_RenderClear(_renderer);
}

void SDLGameRenderer_RenderFrame() {
    SDL_SetRenderTarget(_renderer, cps3_canvas);
    qsort(render_tasks, render_task_count, sizeof(RenderTask), compare_render_tasks);

    for (int i = 0; i < render_task_count; i++) {
        const RenderTask* task = &render_tasks[i];
        const int indices[] = { 0, 1, 2, 1, 2, 3 };
        SDL_RenderGeometry(_renderer, task->texture, task->vertices, 4, indices, 6);
    }

    if (draw_rect_borders) {
        const SDL_FColor red = { .r = 1, .g = 0, .b = 0, .a = SDL_ALPHA_OPAQUE_FLOAT };
        const SDL_FColor green = { .r = 0, .g = 1, .b = 0, .a = SDL_ALPHA_OPAQUE_FLOAT };
        SDL_FColor border_color;

        for (int i = 0; i < render_task_count; i++) {
            const RenderTask* task = &render_tasks[i];
            const float x0 = task->vertices[0].position.x;
            const float y0 = task->vertices[0].position.y;
            const float x1 = task->vertices[3].position.x;
            const float y1 = task->vertices[3].position.y;
            const SDL_FRect border_rect = { .x = x0, .y = y0, .w = (x1 - x0), .h = (y1 - y0) };

            const float lerp_factor = (float)i / (float)(render_task_count - 1);
            lerp_fcolors(&border_color, &red, &green, lerp_factor);

            SDL_SetRenderDrawColorFloat(_renderer, border_color.r, border_color.g, border_color.b, border_color.a);
            SDL_RenderRect(_renderer, &border_rect);
        }
    }
}

void SDLGameRenderer_EndFrame() {
    destroy_textures();
    clear_render_tasks();
}

void SDLGameRenderer_UnlockPalette(unsigned int ph) {
    const int palette_handle = ph;
    if ((palette_handle > 0) && (palette_handle < FL_PALETTE_MAX)) {
        const FLTexture* fl_palette = &flPalette[palette_handle - 1];

        SDLGameRenderer_DestroyPalette(palette_handle);
        SDLGameRenderer_CreatePalette(ph << 16);
    }
}

void SDLGameRenderer_UnlockTexture(unsigned int th) {
    const int texture_handle = th;
    if ((texture_handle > 0) && (texture_handle < FL_TEXTURE_MAX)) {
        const FLTexture* fl_texture = &flTexture[texture_handle - 1];

        SDLGameRenderer_DestroyTexture(texture_handle);
        SDLGameRenderer_CreateTexture(th);
    }
}

void SDLGameRenderer_CreateTexture(unsigned int th) {
    const int texture_index = LO_16_BITS(th) - 1;
    const FLTexture* fl_texture = &flTexture[texture_index];
    const void* pixels = flPS2GetSystemBuffAdrs(fl_texture->mem_handle);
    SDL_PixelFormat pixel_format = SDL_PIXELFORMAT_UNKNOWN;
    int pitch = 0;

    if (surfaces[texture_index] != NULL) {
        fatal_error("Overwriting an existing texture");
    }

    switch (fl_texture->format) {
    case SCE_GS_PSMT8:
        pixel_format = SDL_PIXELFORMAT_INDEX8;
        pitch = fl_texture->width;
        break;

    case SCE_GS_PSMT4:
        pixel_format = SDL_PIXELFORMAT_INDEX4LSB;
        pitch = fl_texture->width / 2;
        break;

    case SCE_GS_PSMCT16:
        pixel_format = SDL_PIXELFORMAT_ABGR1555;
        pitch = fl_texture->width * 2;
        break;

    default:
        fatal_error("Unhandled pixel format: %d", fl_texture->format);
        break;
    }

    const SDL_Surface* surface =
        SDL_CreateSurfaceFrom(fl_texture->width, fl_texture->height, pixel_format, pixels, pitch);
    surfaces[texture_index] = surface;
}

void SDLGameRenderer_DestroyTexture(unsigned int texture_handle) {
    const int texture_index = texture_handle - 1;

    for (int i = 0; i < FL_PALETTE_MAX + 1; i++) {
        SDL_Texture** texture_p = &texture_cache[texture_index][i];

        if (*texture_p == NULL) {
            continue;
        }

        push_texture_to_destroy(*texture_p);
        *texture_p = NULL;
    }

    SDL_DestroySurface(surfaces[texture_index]);
    surfaces[texture_index] = NULL;
}

void SDLGameRenderer_CreatePalette(unsigned int ph) {
    const int palette_index = HI_16_BITS(ph) - 1;
    const FLTexture* fl_palette = &flPalette[palette_index];
    const void* pixels = flPS2GetSystemBuffAdrs(fl_palette->mem_handle);
    const int color_count = fl_palette->width * fl_palette->height;
    SDL_Color colors[256];
    size_t color_size = 0;

    if (palettes[palette_index] != NULL) {
        fatal_error("Overwriting an existing palette");
    }

    switch (fl_palette->format) {
    case SCE_GS_PSMCT32:
        color_size = 4;
        break;

    case SCE_GS_PSMCT16:
        color_size = 2;
        break;

    default:
        fatal_error("Unhandled pixel format: %d", fl_palette->format);
        break;
    }

    switch (color_count) {
    case 16:
        for (int i = 0; i < 16; i++) {
            read_color(pixels, i, color_size, &colors[i]);
        }

        break;

    case 256:
        for (int i = 0; i < 256; i++) {
            const int color_index = clut_shuf(i);
            read_color(pixels, color_index, color_size, &colors[i]);
        }

        break;

    default:
        fatal_error("Unhandled palette dimensions: %dx%d", fl_palette->width, fl_palette->height);
        break;
    }

    SDL_Palette* palette = SDL_CreatePalette(color_count);
    SDL_SetPaletteColors(palette, colors, 0, color_count);
    palettes[palette_index] = palette;
}

void SDLGameRenderer_DestroyPalette(unsigned int palette_handle) {
    const int palette_index = palette_handle - 1;

    for (int i = 0; i < FL_TEXTURE_MAX; i++) {
        SDL_Texture** texture_p = &texture_cache[i][palette_handle];

        if (*texture_p == NULL) {
            continue;
        }

        push_texture_to_destroy(*texture_p);
        *texture_p = NULL;
    }

    SDL_DestroyPalette(palettes[palette_index]);
    palettes[palette_index] = NULL;
}

void SDLGameRenderer_SetTexture(unsigned int th) {
    const int texture_handle = LO_16_BITS(th);
    const SDL_Surface* surface = surfaces[texture_handle - 1];
    const int palette_handle = HI_16_BITS(th);
    const SDL_Palette* palette = palette_handle != 0 ? palettes[palette_handle - 1] : NULL;

    if (dump_textures) {
        save_texture(surface, palette);
    }

    if (palette != NULL) {
        SDL_SetSurfacePalette(surface, palette);
    }

    SDL_Texture* texture = NULL;
    const SDL_Texture* cached_texture = texture_cache[texture_handle - 1][palette_handle];

    if (cached_texture != NULL) {
        texture = cached_texture;
    } else {
        texture = SDL_CreateTextureFromSurface(_renderer, surface);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        texture_cache[texture_handle - 1][palette_handle] = texture;
    }

    push_texture(texture);
}

static void draw_quad(const SDLGameRenderer_Vertex* vertices, bool textured) {
    RenderTask task;
    task.index = render_task_count;
    task.texture = textured ? get_texture() : NULL;
    task.z = flPS2ConvScreenFZ(vertices[0].coord.z);

    SDL_zeroa(task.vertices);

    for (int i = 0; i < 4; i++) {
        task.vertices[i].position.x = vertices[i].coord.x;
        task.vertices[i].position.y = vertices[i].coord.y;

        if (textured) {
            task.vertices[i].tex_coord.x = vertices[i].tex_coord.s;
            task.vertices[i].tex_coord.y = vertices[i].tex_coord.t;
        }

        read_rgba32_fcolor(vertices[i].color, &task.vertices[i].color);
    }

    push_render_task(&task);
}

void SDLGameRenderer_DrawTexturedQuad(const SDLGameRenderer_Vertex* vertices) {
    draw_quad(vertices, true);
}

void SDLGameRenderer_DrawSolidQuad(const SDLGameRenderer_Vertex* vertices) {
    draw_quad(vertices, false);
}

void SDLGameRenderer_DrawSprite(const SDLGameRenderer_Sprite* sprite, unsigned int color) {
    SDLGameRenderer_Vertex vertices[4];
    SDL_zeroa(vertices);

    for (int i = 0; i < 4; i++) {
        vertices[i].coord.z = sprite->v[0].z;
        vertices[i].color = color;
    }

    vertices[0].coord.x = sprite->v[0].x;
    vertices[0].coord.y = sprite->v[0].y;
    vertices[3].coord.x = sprite->v[3].x;
    vertices[3].coord.y = sprite->v[3].y;
    vertices[1].coord.x = vertices[3].coord.x;
    vertices[1].coord.y = vertices[0].coord.y;
    vertices[2].coord.x = vertices[0].coord.x;
    vertices[2].coord.y = vertices[3].coord.y;

    vertices[0].tex_coord = sprite->t[0];
    vertices[3].tex_coord = sprite->t[3];
    vertices[1].tex_coord.s = vertices[3].tex_coord.s;
    vertices[1].tex_coord.t = vertices[0].tex_coord.t;
    vertices[2].tex_coord.s = vertices[0].tex_coord.s;
    vertices[2].tex_coord.t = vertices[3].tex_coord.t;

    draw_quad(vertices, true);
}

void SDLGameRenderer_DrawSprite2(const SDLGameRenderer_Sprite2* sprite2) {
    SDLGameRenderer_Sprite sprite;
    SDL_zero(sprite);

    sprite.v[0] = sprite2->v[0];
    sprite.v[1].x = sprite2->v[1].x;
    sprite.v[1].y = sprite2->v[0].y;
    sprite.v[2].x = sprite2->v[0].x;
    sprite.v[2].y = sprite2->v[1].y;
    sprite.v[3] = sprite2->v[1];

    sprite.t[0] = sprite2->t[0];
    sprite.t[1].s = sprite2->t[1].s;
    sprite.t[1].t = sprite2->t[0].t;
    sprite.t[2].s = sprite2->t[0].s;
    sprite.t[2].t = sprite2->t[1].t;
    sprite.t[3] = sprite2->t[1];

    for (int i = 0; i < 4; i++) {
        sprite.v[i].z = sprite2->v[0].z;
    }

    SDLGameRenderer_DrawSprite(&sprite, sprite2->vertex_color);
}
