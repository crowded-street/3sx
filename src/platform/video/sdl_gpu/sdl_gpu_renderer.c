#if CRS_VIDEO_DRIVER_SDL_GPU

#include "platform/video/sdl_gpu/sdl_gpu_renderer.h"
#include "common.h"
#include "port/utils.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#if DEBUG && IMGUI
#include "imgui/imgui_wrapper.h"
#endif

#include "stb/stb_ds.h"
#include <SDL3/SDL.h>
#include <libgraph.h>

#define QUADS_MAX 1024
#define CANVAS_WIDTH 384
#define CANVAS_HEIGHT 224
#define CANVAS_TEXTURE_FORMAT SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM

typedef enum _PaletteType : Uint8 {
    PALETTE_NONE = 0,
    PALETTE_4,
    PALETTE_8,
} _PaletteType;

typedef struct _Vec2 {
    float x;
    float y;
} _Vec2;

typedef struct _Vec3 {
    float x;
    float y;
    float z;
} _Vec3;

typedef struct _TexCoord {
    float s;
    float t;
} _TexCoord;

typedef struct _Color {
    float r;
    float g;
    float b;
    float a;
} _Color;

typedef struct _Vertex {
    _Vec3 position;
    _TexCoord tex_coord;
    _Color color;
} _Vertex;

typedef struct _Texture {
    SDL_GPUTexture* handle;
    Uint16 width;
    Uint16 height;
    _PaletteType palette_type;
} _Texture;

typedef struct _TextureCreateInfo {
    Uint16 index;
    SDL_GPUTextureFormat format;
    Uint16 width;
    Uint16 height;
    Uint32 size;
    _PaletteType palette_type;
    bool is_palette;
    const void* pixels;
} _TextureCreateInfo;

typedef struct _TextureUploadInfo {
    SDL_GPUTexture* texture;
    SDL_GPUTransferBuffer* transfer_buffer;
    Uint16 w;
    Uint16 h;
} _TextureUploadInfo;

typedef struct _Quad {
    _Vec2 positions[4];
    _TexCoord tex_coords[4];
    float z;
    Uint16 index;
    Sint16 texture_index;
    Sint16 palette_index;
    _Color color;
} _Quad;

static SDL_Window* window = NULL;
static SDL_GPUDevice* device = NULL;
static SDL_GPUPresentMode present_mode = SDL_GPU_PRESENTMODE_IMMEDIATE;

static SDL_GPUGraphicsPipeline* solid_pipeline = NULL;
static SDL_GPUGraphicsPipeline* direct_pipeline = NULL;
static SDL_GPUGraphicsPipeline* palette_4_pipeline = NULL;
static SDL_GPUGraphicsPipeline* palette_8_pipeline = NULL;
static SDL_GPUGraphicsPipeline* screen_pipeline = NULL;
static SDL_GPUSampler* sampler = NULL;
static SDL_GPUTexture* canvas_texture = NULL;
static SDL_GPUTexture* depth_texture = NULL;
static SDL_GPUTextureFormat depth_texture_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;

static SDL_GPUBuffer* vertex_buffer = NULL;
static SDL_GPUTransferBuffer* vertex_transfer_buffer = NULL;
static SDL_GPUBuffer* index_buffer = NULL;
static SDL_GPUBuffer* screen_vertex_buffer = NULL;

static _Quad* quads = NULL;

static _Texture textures[FL_TEXTURE_MAX] = { 0 };
static SDL_GPUTexture* palettes[FL_PALETTE_MAX] = { NULL };
static Sint16 latest_texture_index = 0;
static Sint16 latest_palette_index = 0;

static _TextureCreateInfo* textures_to_create = NULL;
static SDL_GPUTexture** textures_to_delete = NULL;

static SDL_GPUShaderFormat shader_format = SDL_GPU_SHADERFORMAT_INVALID;
static const char* shader_format_path = NULL;
static const char* shader_entrypoint = "main";

// Private

static SDL_GPUShaderFormat get_shader_format(SDL_GPUDevice* device) {
    SDL_GPUShaderFormat supported_formats = SDL_GetGPUShaderFormats(device);

    if (supported_formats & SDL_GPU_SHADERFORMAT_MSL) {
        return SDL_GPU_SHADERFORMAT_MSL;
    } else if (supported_formats & SDL_GPU_SHADERFORMAT_SPIRV) {
        return SDL_GPU_SHADERFORMAT_SPIRV;
    }

    return SDL_GPU_SHADERFORMAT_INVALID;
}

static const char* get_shader_format_path(SDL_GPUShaderFormat format) {
    switch (format) {
    case SDL_GPU_SHADERFORMAT_MSL:
        return "msl";

    case SDL_GPU_SHADERFORMAT_SPIRV:
        return "spirv";

    default:
        return NULL;
    }
}

static const char* get_shader_entrypoint(SDL_GPUShaderFormat format) {
    switch (format) {
    case SDL_GPU_SHADERFORMAT_MSL:
        return "main0";

    default:
        return "main";
    }
}

static SDL_GPUShader* create_shader(
    const char* filename, SDL_GPUDevice* device, SDL_GPUShaderStage stage, Uint32 num_samplers
) {
    const char* base_path = SDL_GetBasePath();
    char* full_path = NULL;
    SDL_asprintf(&full_path, "%s/shaders/sdlgpu/%s/%s", base_path, shader_format_path, filename);

    size_t code_size = 0;
    const Uint8* code = SDL_LoadFile(full_path, &code_size);

    if (code == NULL) {
        SDL_SetError("Failed to load GPU shader at %s: %s", full_path, SDL_GetError());
        SDL_free(full_path);
        return NULL;
    }

    SDL_free(full_path);

    SDL_GPUShader* shader = SDL_CreateGPUShader(
        device,
        &(SDL_GPUShaderCreateInfo) {
            .code = code,
            .code_size = code_size,
            .entrypoint = shader_entrypoint,
            .format = shader_format,
            .stage = stage,
            .num_samplers = num_samplers,
            .num_storage_textures = 0,
            .num_storage_buffers = 0,
            .num_uniform_buffers = 0,
        }
    );

    SDL_free(code);
    return shader;
}

static SDL_GPUGraphicsPipeline* create_pipeline(
    SDL_GPUDevice* device, SDL_GPUShader* vertex_shader, SDL_GPUShader* fragment_shader,
    SDL_GPUTextureFormat target_texture_format, bool enable_depth, bool enable_blend
) {
    return SDL_CreateGPUGraphicsPipeline(
        device,
        &(SDL_GPUGraphicsPipelineCreateInfo) {
            .vertex_shader = vertex_shader,
            .fragment_shader = fragment_shader,
            .vertex_input_state = {
                .vertex_buffer_descriptions = (SDL_GPUVertexBufferDescription[]) {{
                    .slot = 0,
                    .pitch = sizeof(_Vertex),
                    .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                }},
                .num_vertex_buffers = 1,
                .vertex_attributes = (SDL_GPUVertexAttribute[]) {
                    {
                        .location = 0,
                        .buffer_slot = 0,
                        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                        .offset = offsetof(_Vertex, position),
                    },
                    {
                        .location = 1,
                        .buffer_slot = 0,
                        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
                        .offset = offsetof(_Vertex, tex_coord),
                    },
                    {
                        .location = 2,
                        .buffer_slot = 0,
                        .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
                        .offset = offsetof(_Vertex, color),
                    },
                },
                .num_vertex_attributes = 3,
            },
            .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
            .depth_stencil_state = {
                .compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL,
                .enable_depth_test = enable_depth,
                .enable_depth_write = enable_depth,
            },
            // .rasterizer_state = {
            //     .fill_mode = SDL_GPU_FILLMODE_LINE,
            // },
            .target_info = {
                .color_target_descriptions = (SDL_GPUColorTargetDescription[]) {{
                    .format = target_texture_format,
                    .blend_state = {
                        .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                        .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                        .color_blend_op = SDL_GPU_BLENDOP_ADD,
                        .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
                        .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
                        .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
                        .enable_blend = enable_blend,
                    }
                }},
                .num_color_targets = 1,
                .depth_stencil_format = depth_texture_format,
                .has_depth_stencil_target = enable_depth,
            }
        }
    );
}

static SDL_GPUTextureFormat get_supported_depth_format(SDL_GPUDevice* device) {
    if (SDL_GPUTextureSupportsFormat(
            device, SDL_GPU_TEXTUREFORMAT_D24_UNORM, SDL_GPU_TEXTURETYPE_2D, SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET
        )) {
        return SDL_GPU_TEXTUREFORMAT_D24_UNORM;
    }

    if (SDL_GPUTextureSupportsFormat(
            device, SDL_GPU_TEXTUREFORMAT_D32_FLOAT, SDL_GPU_TEXTURETYPE_2D, SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET
        )) {
        return SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
    }

    return SDL_GPU_TEXTUREFORMAT_D16_UNORM;
}

static float convert_to_screen_x(float x) {
    x /= 384;
    x *= 2;
    x -= 1;
    return x;
}

static float convert_to_screen_y(float y) {
    y /= 224;
    y *= 2;
    y -= 1;
    y *= -1;
    return y;
}

static float convert_color_component(Uint8 component) {
    return (float)component / (float)SDL_MAX_UINT8;
}

static _Color convert_color(Uint32 color) {
    return (_Color) {
        .r = convert_color_component((color >> 16) & 0xFF),
        .g = convert_color_component((color >> 8) & 0xFF),
        .b = convert_color_component(color & 0xFF),
        .a = convert_color_component(color >> 24),
    };
}

static void quad_infer_positions(_Quad* quad) {
    quad->positions[1].x = quad->positions[3].x;
    quad->positions[1].y = quad->positions[0].y;
    quad->positions[2].x = quad->positions[0].x;
    quad->positions[2].y = quad->positions[3].y;
}

static void quad_infer_tex_coords(_Quad* quad) {
    quad->tex_coords[1].s = quad->tex_coords[3].s;
    quad->tex_coords[1].t = quad->tex_coords[0].t;
    quad->tex_coords[2].s = quad->tex_coords[0].s;
    quad->tex_coords[2].t = quad->tex_coords[3].t;
}

static void SDLGPURenderer_CreateTexture(Uint32 th) {
    const int texture_index = LO_16_BITS(th) - 1;
    const FLTexture* fl_texture = &flTexture[texture_index];

    if (textures[texture_index].handle != NULL) {
        arrpush(textures_to_delete, textures[texture_index].handle);
        SDL_zero(textures[texture_index]);
    }

    _TextureCreateInfo tex_create_info = { 0 };
    tex_create_info.index = texture_index;
    tex_create_info.width = fl_texture->width;
    tex_create_info.height = fl_texture->height;
    tex_create_info.pixels = flPS2GetSystemBuffAdrs(fl_texture->mem_handle);
    tex_create_info.is_palette = false;

    switch (fl_texture->format) {
    case SCE_GS_PSMT8:
        tex_create_info.format = SDL_GPU_TEXTUREFORMAT_R8_UINT;
        tex_create_info.size = fl_texture->width * fl_texture->height;
        tex_create_info.palette_type = PALETTE_8;
        break;

    case SCE_GS_PSMT4:
        tex_create_info.width /= 2;
        tex_create_info.format = SDL_GPU_TEXTUREFORMAT_R8_UINT;
        tex_create_info.size = fl_texture->width * fl_texture->height / 2;
        tex_create_info.palette_type = PALETTE_4;
        break;

    case SCE_GS_PSMCT16:
        tex_create_info.format = SDL_GPU_TEXTUREFORMAT_B5G5R5A1_UNORM;
        tex_create_info.size = fl_texture->width * fl_texture->height * 2;
        tex_create_info.palette_type = PALETTE_NONE;
        break;

    default:
        fatal_error("Unhandled pixel format: %d", fl_texture->format);
        break;
    }

    arrpush(textures_to_create, tex_create_info);
}

static void SDLGPURenderer_DestroyTexture(Uint32 texture_handle) {
    // Do nothing
}

static void SDLGPURenderer_UnlockTexture(Uint32 th) {
    SDLGPURenderer_CreateTexture(th);
}

static void SDLGPURenderer_CreatePalette(Uint32 ph) {
    const int palette_index = HI_16_BITS(ph) - 1;
    const FLTexture* fl_palette = &flPalette[palette_index];

    if (palettes[palette_index] != NULL) {
        arrpush(textures_to_delete, palettes[palette_index]);
        palettes[palette_index] = NULL;
    }

    const int num_colors = fl_palette->width * fl_palette->height;

    _TextureCreateInfo tex_create_info = { 0 };
    tex_create_info.index = palette_index;
    tex_create_info.width = num_colors;
    tex_create_info.height = 1;
    tex_create_info.pixels = flPS2GetSystemBuffAdrs(fl_palette->mem_handle);
    tex_create_info.is_palette = true;

    switch (fl_palette->format) {
    case SCE_GS_PSMCT32:
        tex_create_info.format = SDL_GPU_TEXTUREFORMAT_B8G8R8A8_UNORM;
        tex_create_info.size = num_colors * 4;
        break;

    case SCE_GS_PSMCT16:
        tex_create_info.format = SDL_GPU_TEXTUREFORMAT_B5G5R5A1_UNORM;
        tex_create_info.size = num_colors * 2;
        break;

    default:
        fatal_error("Unhandled pixel format: %d", fl_palette->format);
        break;
    }

    arrpush(textures_to_create, tex_create_info);
}

static void SDLGPURenderer_DestroyPalette(Uint32 palette_handle) {
    // Do nothing
}

static void SDLGPURenderer_UnlockPalette(Uint32 ph) {
    SDLGPURenderer_CreatePalette(ph << 16);
}

static void SDLGPURenderer_SetTexture(Uint32 th) {
    latest_texture_index = LO_16_BITS(th) - 1;
    latest_palette_index = HI_16_BITS(th) - 1;
}

static void SDLGPURenderer_DrawTexturedQuad(const Sprite* sprite, Uint32 color) {
    SDL_assert(arrlen(quads) < QUADS_MAX);

    _Quad* quad = arraddnptr(quads, 1);

    for (int i = 0; i < 4; i++) {
        quad->positions[i].x = convert_to_screen_x(sprite->v[i].x);
        quad->positions[i].y = convert_to_screen_y(sprite->v[i].y);
        quad->tex_coords[i].s = sprite->t[i].s;
        quad->tex_coords[i].t = sprite->t[i].t;
    }

    quad->z = sprite->v[0].z;
    quad->index = arrlen(quads);
    quad->color = convert_color(color);
    quad->texture_index = latest_texture_index;
    quad->palette_index = latest_palette_index;
}

static void SDLGPURenderer_DrawSprite(const Sprite* sprite, Uint32 color) {
    SDL_assert(arrlen(quads) < QUADS_MAX);

    _Quad* quad = arraddnptr(quads, 1);

    quad->positions[0].x = convert_to_screen_x(sprite->v[0].x);
    quad->positions[0].y = convert_to_screen_y(sprite->v[0].y);
    quad->positions[3].x = convert_to_screen_x(sprite->v[3].x);
    quad->positions[3].y = convert_to_screen_y(sprite->v[3].y);
    quad_infer_positions(quad);

    quad->tex_coords[0].s = sprite->t[0].s;
    quad->tex_coords[0].t = sprite->t[0].t;
    quad->tex_coords[3].s = sprite->t[3].s;
    quad->tex_coords[3].t = sprite->t[3].t;
    quad_infer_tex_coords(quad);

    quad->z = sprite->v[0].z;
    quad->index = arrlen(quads);
    quad->color = convert_color(color);
    quad->texture_index = latest_texture_index;
    quad->palette_index = latest_palette_index;
}

static void SDLGPURenderer_DrawSprite2(const Sprite2* sprite2) {
    SDL_assert(arrlen(quads) < QUADS_MAX);

    _Quad* quad = arraddnptr(quads, 1);

    quad->positions[0].x = convert_to_screen_x(sprite2->v[0].x);
    quad->positions[0].y = convert_to_screen_y(sprite2->v[0].y);
    quad->positions[3].x = convert_to_screen_x(sprite2->v[1].x);
    quad->positions[3].y = convert_to_screen_y(sprite2->v[1].y);
    quad_infer_positions(quad);

    quad->tex_coords[0].s = sprite2->t[0].s;
    quad->tex_coords[0].t = sprite2->t[0].t;
    quad->tex_coords[3].s = sprite2->t[1].s;
    quad->tex_coords[3].t = sprite2->t[1].t;
    quad_infer_tex_coords(quad);

    quad->z = sprite2->v[0].z;
    quad->index = arrlen(quads);
    quad->color = convert_color(sprite2->vertex_color);
    quad->texture_index = latest_texture_index;
    quad->palette_index = latest_palette_index;
}

static void SDLGPURenderer_DrawSolidQuad(const Quad* quad, Uint32 color) {
    SDL_assert(arrlen(quads) < QUADS_MAX);

    _Quad* _quad = arraddnptr(quads, 1);

    for (int i = 0; i < 4; i++) {
        _quad->positions[i].x = convert_to_screen_x(quad->v[i].x);
        _quad->positions[i].y = convert_to_screen_y(quad->v[i].y);
    }

    _quad->z = quad->v[0].z;
    _quad->index = arrlen(quads);
    _quad->color = convert_color(color);
    _quad->texture_index = -1;
    _quad->palette_index = -1;
}

static SDL_Window* SDLGPURenderer_Init(const SDLRenderBackendInitInfo* init_info) {
    // Init window and GPU device

    window = SDL_CreateWindow(
        init_info->app_name, init_info->window_width, init_info->window_height, init_info->window_flags
    );

    if (window == NULL) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return NULL;
    }

    device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_MSL, false, NULL);

    if (device == NULL) {
        SDL_Log("Failed to create GPU device: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        return NULL;
    }

    SDL_ClaimWindowForGPUDevice(device, window);

    if (SDL_WindowSupportsGPUPresentMode(device, window, SDL_GPU_PRESENTMODE_MAILBOX)) {
        present_mode = SDL_GPU_PRESENTMODE_MAILBOX;
        SDL_Log("Using MAILBOX present mode");
    } else {
        present_mode = SDL_GPU_PRESENTMODE_IMMEDIATE;
        SDL_Log("Using IMMEDIATE present mode");
    }

    if (!SDL_SetGPUSwapchainParameters(device, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, present_mode)) {
        SDL_Log("Failed to set GPU swapchain parameters: %s", SDL_GetError());
        SDL_ReleaseWindowFromGPUDevice(device, window);
        SDL_DestroyGPUDevice(device);
        SDL_DestroyWindow(window);
        return NULL;
    }

    // Init common variables

    arrsetcap(quads, QUADS_MAX);
    depth_texture_format = get_supported_depth_format(device);

    // Init shaders

    shader_format = get_shader_format(device);
    shader_format_path = get_shader_format_path(shader_format);
    shader_entrypoint = get_shader_entrypoint(shader_format);

    SDL_Log("Using SDL GPU driver %s with shaders from %s", SDL_GetGPUDeviceDriver(device), shader_format_path);

    SDL_GPUShader* vertex_shader = create_shader("vert", device, SDL_GPU_SHADERSTAGE_VERTEX, 0);
    SDL_GPUShader* solid_fragment_shader = create_shader("solid.frag", device, SDL_GPU_SHADERSTAGE_FRAGMENT, 0);
    SDL_GPUShader* direct_fragment_shader = create_shader("direct.frag", device, SDL_GPU_SHADERSTAGE_FRAGMENT, 1);
    SDL_GPUShader* palette_4_fragment_shader = create_shader("palette4.frag", device, SDL_GPU_SHADERSTAGE_FRAGMENT, 2);
    SDL_GPUShader* palette_8_fragment_shader = create_shader("palette8.frag", device, SDL_GPU_SHADERSTAGE_FRAGMENT, 2);
    SDL_GPUShader* screen_fragment_shader = create_shader("screen.frag", device, SDL_GPU_SHADERSTAGE_FRAGMENT, 1);

    const SDL_GPUTextureFormat swapchain_texture_format = SDL_GetGPUSwapchainTextureFormat(device, window);

    solid_pipeline = create_pipeline(device, vertex_shader, solid_fragment_shader, CANVAS_TEXTURE_FORMAT, true, true);
    direct_pipeline = create_pipeline(device, vertex_shader, direct_fragment_shader, CANVAS_TEXTURE_FORMAT, true, true);
    palette_4_pipeline =
        create_pipeline(device, vertex_shader, palette_4_fragment_shader, CANVAS_TEXTURE_FORMAT, true, true);
    palette_8_pipeline =
        create_pipeline(device, vertex_shader, palette_8_fragment_shader, CANVAS_TEXTURE_FORMAT, true, true);
    screen_pipeline =
        create_pipeline(device, vertex_shader, screen_fragment_shader, swapchain_texture_format, false, false);

    SDL_ReleaseGPUShader(device, vertex_shader);
    SDL_ReleaseGPUShader(device, solid_fragment_shader);
    SDL_ReleaseGPUShader(device, direct_fragment_shader);
    SDL_ReleaseGPUShader(device, palette_4_fragment_shader);
    SDL_ReleaseGPUShader(device, palette_8_fragment_shader);
    SDL_ReleaseGPUShader(device, screen_fragment_shader);

    // Init canvas

    canvas_texture = SDL_CreateGPUTexture(
        device,
        &(SDL_GPUTextureCreateInfo) {
            .type = SDL_GPU_TEXTURETYPE_2D,
            .format = CANVAS_TEXTURE_FORMAT,
            .usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER,
            .width = CANVAS_WIDTH,
            .height = CANVAS_HEIGHT,
            .layer_count_or_depth = 1,
            .num_levels = 1,
        }
    );

    depth_texture = SDL_CreateGPUTexture(
        device,
        &(SDL_GPUTextureCreateInfo) {
            .type = SDL_GPU_TEXTURETYPE_2D,
            .format = depth_texture_format,
            .usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
            .width = CANVAS_WIDTH,
            .height = CANVAS_HEIGHT,
            .layer_count_or_depth = 1,
            .num_levels = 1,
        }
    );

    // Init vertex buffer

    const Uint32 vertex_buffer_max_size = QUADS_MAX * 4 * sizeof(_Vertex);

    vertex_buffer = SDL_CreateGPUBuffer(
        device,
        &(SDL_GPUBufferCreateInfo) {
            .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
            .size = vertex_buffer_max_size,
        }
    );

    vertex_transfer_buffer = SDL_CreateGPUTransferBuffer(
        device,
        &(SDL_GPUTransferBufferCreateInfo) {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size = vertex_buffer_max_size,
        }
    );

    // Init index buffer

    const Uint32 index_buffer_size = sizeof(Uint16) * 6 * QUADS_MAX;

    index_buffer = SDL_CreateGPUBuffer(
        device,
        &(SDL_GPUBufferCreateInfo) {
            .usage = SDL_GPU_BUFFERUSAGE_INDEX,
            .size = index_buffer_size,
        }
    );

    SDL_GPUTransferBuffer* index_transfer_buffer = SDL_CreateGPUTransferBuffer(
        device,
        &(SDL_GPUTransferBufferCreateInfo) {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size = index_buffer_size,
        }
    );

    Uint16* index_transfer_ptr = SDL_MapGPUTransferBuffer(device, index_transfer_buffer, false);

    for (int i = 0; i < QUADS_MAX; i++) {
        index_transfer_ptr[i * 6 + 0] = i * 4 + 0;
        index_transfer_ptr[i * 6 + 1] = i * 4 + 1;
        index_transfer_ptr[i * 6 + 2] = i * 4 + 2;
        index_transfer_ptr[i * 6 + 3] = i * 4 + 2;
        index_transfer_ptr[i * 6 + 4] = i * 4 + 1;
        index_transfer_ptr[i * 6 + 5] = i * 4 + 3;
    }

    SDL_UnmapGPUTransferBuffer(device, index_transfer_buffer);

    // Init screen quad

    const Uint32 screen_vertex_buffer_size = 4 * sizeof(_Vertex);

    screen_vertex_buffer = SDL_CreateGPUBuffer(
        device,
        &(SDL_GPUBufferCreateInfo) {
            .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
            .size = screen_vertex_buffer_size,
        }
    );

    SDL_GPUTransferBuffer* screen_vertex_transfer_buffer = SDL_CreateGPUTransferBuffer(
        device,
        &(SDL_GPUTransferBufferCreateInfo) {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size = screen_vertex_buffer_size,
        }
    );

    _Vertex screen_vertices[4] = {
        {
            .position = { .x = -1.0f, .y = 1.0f, .z = 0.0f },
            .tex_coord = { .s = 0.0f, .t = 0.0f },
        },
        {
            .position = { .x = 1.0f, .y = 1.0f, .z = 0.0f },
            .tex_coord = { .s = 1.0f, .t = 0.0f },
        },
        {
            .position = { .x = -1.0f, .y = -1.0f, .z = 0.0f },
            .tex_coord = { .s = 0.0f, .t = 1.0f },
        },
        {
            .position = { .x = 1.0f, .y = -1.0f, .z = 0.0f },
            .tex_coord = { .s = 1.0f, .t = 1.0f },
        },
    };

    for (int i = 0; i < 4; i++) {
        screen_vertices[i].color = (_Color) { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
    }

    _Vertex* screen_vertex_transfer_ptr = SDL_MapGPUTransferBuffer(device, screen_vertex_transfer_buffer, false);

    SDL_memcpy(screen_vertex_transfer_ptr, screen_vertices, sizeof(screen_vertices));
    SDL_UnmapGPUTransferBuffer(device, screen_vertex_transfer_buffer);

    // Init sampler

    sampler = SDL_CreateGPUSampler(
        device,
        &(SDL_GPUSamplerCreateInfo) {
            .min_filter = SDL_GPU_FILTER_NEAREST,
            .mag_filter = SDL_GPU_FILTER_NEAREST,
            .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
            .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
            .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
            .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        }
    );

    // Upload up-front data

    SDL_GPUCommandBuffer* upload_cmd_buf = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(upload_cmd_buf);

    SDL_UploadToGPUBuffer(
        copy_pass,
        &(SDL_GPUTransferBufferLocation) {
            .transfer_buffer = index_transfer_buffer,
            .offset = 0,
        },
        &(SDL_GPUBufferRegion) {
            .buffer = index_buffer,
            .offset = 0,
            .size = index_buffer_size,
        },
        false
    );

    SDL_UploadToGPUBuffer(
        copy_pass,
        &(SDL_GPUTransferBufferLocation) {
            .transfer_buffer = screen_vertex_transfer_buffer,
            .offset = 0,
        },
        &(SDL_GPUBufferRegion) {
            .buffer = screen_vertex_buffer,
            .offset = 0,
            .size = screen_vertex_buffer_size,
        },
        false
    );

    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(upload_cmd_buf);
    SDL_ReleaseGPUTransferBuffer(device, index_transfer_buffer);
    SDL_ReleaseGPUTransferBuffer(device, screen_vertex_transfer_buffer);

#if DEBUG && IMGUI
    ImGuiW_Init(
        window,
        &(ImGui_ImplSDLGPU3_InitInfo) {
            .Device = device,
            .ColorTargetFormat = swapchain_texture_format,
            .PresentMode = present_mode,
        }
    );
#endif

    return window;
}

static void SDLGPURenderer_Quit() {
#if DEBUG && IMGUI
    ImGuiW_Finish();
#endif

    SDL_ReleaseGPUGraphicsPipeline(device, solid_pipeline);
    SDL_ReleaseGPUGraphicsPipeline(device, direct_pipeline);
    SDL_ReleaseGPUGraphicsPipeline(device, palette_4_pipeline);
    SDL_ReleaseGPUGraphicsPipeline(device, palette_8_pipeline);
    SDL_ReleaseGPUGraphicsPipeline(device, screen_pipeline);
    SDL_ReleaseGPUBuffer(device, vertex_buffer);
    SDL_ReleaseGPUTransferBuffer(device, vertex_transfer_buffer);
    SDL_ReleaseGPUBuffer(device, index_buffer);
    SDL_ReleaseGPUBuffer(device, screen_vertex_buffer);
    SDL_ReleaseGPUSampler(device, sampler);
    SDL_ReleaseGPUTexture(device, canvas_texture);
    SDL_ReleaseGPUTexture(device, depth_texture);

    SDL_ReleaseWindowFromGPUDevice(device, window);
    SDL_DestroyGPUDevice(device);
    SDL_DestroyWindow(window);
}

static void SDLGPURenderer_RenderFrame(SDL_Rect viewport) {
    SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(device);

    // Delete stale textures

    for (int i = 0; i < arrlen(textures_to_delete); i++) {
        SDL_ReleaseGPUTexture(device, textures_to_delete[i]);
    }

    // Prepare texture and vertex data

    _TextureUploadInfo* texture_uploads = NULL;

    for (int i = 0; i < arrlen(textures_to_create); i++) {
        const _TextureCreateInfo* info = &textures_to_create[i];

        SDL_GPUTransferBuffer* texture_transfer_buffer = SDL_CreateGPUTransferBuffer(
            device,
            &(SDL_GPUTransferBufferCreateInfo) {
                .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
                .size = info->size,
            }
        );

        void* texture_transfer_ptr = SDL_MapGPUTransferBuffer(device, texture_transfer_buffer, false);
        SDL_memcpy(texture_transfer_ptr, info->pixels, info->size);
        SDL_UnmapGPUTransferBuffer(device, texture_transfer_buffer);

        SDL_GPUTexture* texture = SDL_CreateGPUTexture(
            device,
            &(SDL_GPUTextureCreateInfo) {
                .type = SDL_GPU_TEXTURETYPE_2D,
                .format = info->format,
                .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
                .width = info->width,
                .height = info->height,
                .layer_count_or_depth = 1,
                .num_levels = 1,
            }
        );

        _TextureUploadInfo* upload = arraddnptr(texture_uploads, 1);
        upload->texture = texture;
        upload->transfer_buffer = texture_transfer_buffer;
        upload->w = info->width;
        upload->h = info->height;

        if (info->is_palette) {
            palettes[info->index] = texture;
        } else {
            textures[info->index] = (_Texture) {
                .handle = texture,
                .width = info->width,
                .height = info->height,
                .palette_type = info->palette_type,
            };
        }
    }

    if (arrlen(quads) > 0) {
        _Vertex* vertex_transfer_ptr = SDL_MapGPUTransferBuffer(device, vertex_transfer_buffer, false);

        for (int i = 0; i < arrlen(quads); i++) {
            const int vertex_base = i * 4;
            const _Quad* quad = &quads[i];

            for (int i = 0; i < 4; i++) {
                vertex_transfer_ptr[vertex_base + i].position.x = quad->positions[i].x;
                vertex_transfer_ptr[vertex_base + i].position.y = quad->positions[i].y;
                vertex_transfer_ptr[vertex_base + i].position.z = quad->z;
                vertex_transfer_ptr[vertex_base + i].tex_coord = quad->tex_coords[i];
                vertex_transfer_ptr[vertex_base + i].color = quad->color;
            }
        }

        SDL_UnmapGPUTransferBuffer(device, vertex_transfer_buffer);
    }

    // Upload

    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(command_buffer);

    for (int i = 0; i < arrlen(texture_uploads); i++) {
        const _TextureUploadInfo* upload = &texture_uploads[i];

        SDL_UploadToGPUTexture(
            copy_pass,
            &(SDL_GPUTextureTransferInfo) {
                .transfer_buffer = upload->transfer_buffer,
                .offset = 0,
            },
            &(SDL_GPUTextureRegion) {
                .texture = upload->texture,
                .w = upload->w,
                .h = upload->h,
                .d = 1,
            },
            false
        );
    }

    if (arrlen(quads) > 0) {
        SDL_UploadToGPUBuffer(
            copy_pass,
            &(SDL_GPUTransferBufferLocation) {
                .transfer_buffer = vertex_transfer_buffer,
                .offset = 0,
            },
            &(SDL_GPUBufferRegion) {
                .buffer = vertex_buffer,
                .offset = 0,
                .size = arrlen(quads) * 4 * sizeof(_Vertex),
            },
            false
        );
    }

    SDL_EndGPUCopyPass(copy_pass);

    // Render

    SDL_GPUTexture* swapchain_texture = NULL;
    SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchain_texture, NULL, NULL);

    if (swapchain_texture != NULL) {
#if DEBUG && IMGUI
        ImGuiW_PrepareDrawData(command_buffer);
#endif

        SDL_GPURenderPass* canvas_pass = SDL_BeginGPURenderPass(
            command_buffer,
            &(SDL_GPUColorTargetInfo) {
                .clear_color = { 0, 0, 0, 1 },
                .load_op = SDL_GPU_LOADOP_CLEAR,
                .store_op = SDL_GPU_STOREOP_STORE,
                .texture = canvas_texture,
            },
            1,
            &(SDL_GPUDepthStencilTargetInfo) {
                .texture = depth_texture,
                .clear_depth = 1.0f,
                .load_op = SDL_GPU_LOADOP_CLEAR,
                .store_op = SDL_GPU_STOREOP_DONT_CARE,
                .stencil_load_op = SDL_GPU_LOADOP_DONT_CARE,
                .stencil_store_op = SDL_GPU_STOREOP_DONT_CARE,
            }
        );

        SDL_BindGPUVertexBuffers(
            canvas_pass,
            0,
            (SDL_GPUBufferBinding[]) {
                {
                    .buffer = vertex_buffer,
                    .offset = 0,
                },
            },
            1
        );

        SDL_BindGPUIndexBuffer(
            canvas_pass,
            (SDL_GPUBufferBinding[]) {
                {
                    .buffer = index_buffer,
                    .offset = 0,
                },
            },
            SDL_GPU_INDEXELEMENTSIZE_16BIT
        );

        for (int i = 0; i < arrlen(quads); i++) {
            const _Quad* quad = &quads[i];

            if (quad->texture_index == -1) {
                SDL_BindGPUGraphicsPipeline(canvas_pass, solid_pipeline);
            } else {
                const _Texture* texture = &textures[quad->texture_index];

                switch (texture->palette_type) {
                case PALETTE_NONE:
                    SDL_BindGPUGraphicsPipeline(canvas_pass, direct_pipeline);

                    SDL_BindGPUFragmentSamplers(
                        canvas_pass,
                        0,
                        (SDL_GPUTextureSamplerBinding[]) {
                            {
                                .texture = texture->handle,
                                .sampler = sampler,
                            },
                        },
                        1
                    );

                    break;

                case PALETTE_4:
                    SDL_BindGPUGraphicsPipeline(canvas_pass, palette_4_pipeline);

                    SDL_BindGPUFragmentSamplers(
                        canvas_pass,
                        0,
                        (SDL_GPUTextureSamplerBinding[]) {
                            {
                                .texture = texture->handle,
                                .sampler = sampler,
                            },
                            {
                                .texture = palettes[quad->palette_index],
                                .sampler = sampler,
                            },
                        },
                        2
                    );

                    break;

                case PALETTE_8:
                    SDL_BindGPUGraphicsPipeline(canvas_pass, palette_8_pipeline);

                    SDL_BindGPUFragmentSamplers(
                        canvas_pass,
                        0,
                        (SDL_GPUTextureSamplerBinding[]) {
                            {
                                .texture = texture->handle,
                                .sampler = sampler,
                            },
                            {
                                .texture = palettes[quad->palette_index],
                                .sampler = sampler,
                            },
                        },
                        2
                    );

                    break;
                }
            }

            SDL_DrawGPUIndexedPrimitives(canvas_pass, 6, 1, i * 6, 0, 0);
        }

        SDL_EndGPURenderPass(canvas_pass);

        SDL_GPURenderPass* screen_pass = SDL_BeginGPURenderPass(
            command_buffer,
            &(SDL_GPUColorTargetInfo) {
                .clear_color = { 0, 0, 0, 1 },
                .load_op = SDL_GPU_LOADOP_CLEAR,
                .store_op = SDL_GPU_STOREOP_STORE,
                .texture = swapchain_texture,
            },
            1,
            NULL
        );

        SDL_SetGPUViewport(
            screen_pass,
            &(SDL_GPUViewport) {
                .x = viewport.x,
                .y = viewport.y,
                .w = viewport.w,
                .h = viewport.h,
                .min_depth = 0,
                .max_depth = 1,
            }
        );

        SDL_BindGPUGraphicsPipeline(screen_pass, screen_pipeline);

        SDL_BindGPUVertexBuffers(
            screen_pass,
            0,
            (SDL_GPUBufferBinding[]) {
                {
                    .buffer = screen_vertex_buffer,
                    .offset = 0,
                },
            },
            1
        );

        SDL_BindGPUIndexBuffer(
            screen_pass,
            (SDL_GPUBufferBinding[]) {
                {
                    .buffer = index_buffer,
                    .offset = 0,
                },
            },
            SDL_GPU_INDEXELEMENTSIZE_16BIT
        );

        SDL_BindGPUFragmentSamplers(
            screen_pass,
            0,
            (SDL_GPUTextureSamplerBinding[]) {
                {
                    .texture = canvas_texture,
                    .sampler = sampler,
                },
            },
            1
        );

        SDL_DrawGPUIndexedPrimitives(screen_pass, 6, 1, 0, 0, 0);

#if DEBUG && IMGUI
        ImGuiW_RenderDrawData(command_buffer, screen_pass);
#endif

        SDL_EndGPURenderPass(screen_pass);
    }

    SDL_SubmitGPUCommandBuffer(command_buffer);

    // Cleanup

    for (int i = 0; i < arrlen(texture_uploads); i++) {
        SDL_ReleaseGPUTransferBuffer(device, texture_uploads[i].transfer_buffer);
    }

    arrfree(texture_uploads);
    arrsetlen(quads, 0);
    arrsetlen(textures_to_delete, 0);
    arrsetlen(textures_to_create, 0);
}

// Internal

const SDLRenderBackend sdl_gpu_render_backend = {
    .name = "SDL GPU",
    .init = SDLGPURenderer_Init,
    .quit = SDLGPURenderer_Quit,
    .render_frame = SDLGPURenderer_RenderFrame,
    .create_texture = SDLGPURenderer_CreateTexture,
    .destroy_texture = SDLGPURenderer_DestroyTexture,
    .unlock_texture = SDLGPURenderer_UnlockTexture,
    .create_palette = SDLGPURenderer_CreatePalette,
    .destroy_palette = SDLGPURenderer_DestroyPalette,
    .unlock_palette = SDLGPURenderer_UnlockPalette,
    .set_texture = SDLGPURenderer_SetTexture,
    .draw_textured_quad = SDLGPURenderer_DrawTexturedQuad,
    .draw_sprite = SDLGPURenderer_DrawSprite,
    .draw_sprite2 = SDLGPURenderer_DrawSprite2,
    .draw_solid_quad = SDLGPURenderer_DrawSolidQuad,
};

#endif // CRS_VIDEO_DRIVER_SDL_GPU
