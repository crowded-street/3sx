#if CRS_VIDEO_DRIVER_SDL_GPU

#include "platform/video/sdl_gpu/sdl_gpu_renderer.h"

#include <SDL3/SDL.h>

#define QUADS_MAX 512

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

static SDL_GPUBuffer* vertex_buffer = NULL;
static SDL_GPUBuffer* index_buffer = NULL;

// Public

void SDLGPURenderer_CreateTexture(Uint32 th) {
    //
}

void SDLGPURenderer_DestroyTexture(Uint32 texture_handle) {
    //
}

void SDLGPURenderer_UnlockTexture(Uint32 th) {
    //
}

void SDLGPURenderer_CreatePalette(Uint32 ph) {
    //
}

void SDLGPURenderer_DestroyPalette(Uint32 palette_handle) {
    //
}

void SDLGPURenderer_UnlockPalette(Uint32 ph) {
    //
}

void SDLGPURenderer_SetTexture(Uint32 th) {
    //
}

void SDLGPURenderer_DrawTexturedQuad(const Sprite* sprite, Uint32 color) {
    //
}

void SDLGPURenderer_DrawSprite(const Sprite* sprite, Uint32 color) {
    //
}

void SDLGPURenderer_DrawSprite2(const Sprite2* sprite2) {
    //
}

void SDLGPURenderer_DrawSolidQuad(const Quad* quad, Uint32 color) {
    //
}

// Internal

bool SDLGPURenderer_Init(const SDLGPURendererContext* context) {
    vertex_buffer = SDL_CreateGPUBuffer(
        context->device,
        &(SDL_GPUBufferCreateInfo) {
            .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
            .size = QUADS_MAX * 4 * sizeof(_Vertex),
        }
    );

    // Init index buffer

    const Uint32 index_buffer_size = sizeof(Uint16) * 6 * QUADS_MAX;

    index_buffer = SDL_CreateGPUBuffer(
        context->device,
        &(SDL_GPUBufferCreateInfo) {
            .usage = SDL_GPU_BUFFERUSAGE_INDEX,
            .size = index_buffer_size,
        }
    );

    SDL_GPUTransferBuffer* index_transfer_buffer = SDL_CreateGPUTransferBuffer(
        context->device,
        &(SDL_GPUTransferBufferCreateInfo) {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size = index_buffer_size,
        }
    );

    Uint16* index_transfer_ptr = SDL_MapGPUTransferBuffer(context->device, index_transfer_buffer, false);

    for (int i = 0; i < QUADS_MAX; i++) {
        index_transfer_ptr[i * 6 + 0] = i * 4 + 0;
        index_transfer_ptr[i * 6 + 1] = i * 4 + 1;
        index_transfer_ptr[i * 6 + 2] = i * 4 + 2;
        index_transfer_ptr[i * 6 + 3] = i * 4 + 2;
        index_transfer_ptr[i * 6 + 4] = i * 4 + 1;
        index_transfer_ptr[i * 6 + 5] = i * 4 + 3;
    }

    SDL_UnmapGPUTransferBuffer(context->device, index_transfer_buffer);

    // Upload up-front data

    SDL_GPUCommandBuffer* upload_cmd_buf = SDL_AcquireGPUCommandBuffer(context->device);
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

    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(upload_cmd_buf);
    SDL_ReleaseGPUTransferBuffer(context->device, index_transfer_buffer);

    return true;
}

void SDLGPURenderer_Quit() {
    //
}

void SDLGPURenderer_RenderFrame(const SDLGPURendererContext* context) {
    SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(context->device);

    SDL_GPUTexture* swapchain_texture;
    Uint32 width, height;
    SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, context->window, &swapchain_texture, &width, &height);

    if (swapchain_texture == NULL) {
        SDL_SubmitGPUCommandBuffer(command_buffer);
        return;
    }

    SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(
        command_buffer,
        &(SDL_GPUColorTargetInfo) {
            .clear_color = { 1, 0, 1, 1 },
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = SDL_GPU_STOREOP_STORE,
            .texture = swapchain_texture,
        },
        1,
        NULL
    );

    SDL_EndGPURenderPass(render_pass);
    SDL_SubmitGPUCommandBuffer(command_buffer);
}

#endif
