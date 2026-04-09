#include "port/render_backend.h"

#include "port/sdl/sdl_game_renderer.h"

void RenderBackend_Init(const PlatformHostContext* host_context) {
    void* renderer = NULL;

    if (host_context != NULL) {
        renderer = host_context->renderer;
    }

    SDLGameRenderer_Init(renderer);
}

void RenderBackend_Shutdown() {
    SDLGameRenderer_Shutdown();
}

void RenderBackend_BeginFrame() {
    SDLGameRenderer_BeginFrame();
}

void RenderBackend_RenderFrame() {
    SDLGameRenderer_RenderFrame();
}

void RenderBackend_EndFrame() {
    SDLGameRenderer_EndFrame();
}

void* RenderBackend_GetCanvasHandle() {
    return SDLGameRenderer_GetCanvasHandle();
}

void RenderBackend_CreateTexture(unsigned int th) {
    SDLGameRenderer_CreateTexture(th);
}

void RenderBackend_DestroyTexture(unsigned int texture_handle) {
    SDLGameRenderer_DestroyTexture(texture_handle);
}

void RenderBackend_UnlockTexture(unsigned int th) {
    SDLGameRenderer_UnlockTexture(th);
}

void RenderBackend_CreatePalette(unsigned int ph) {
    SDLGameRenderer_CreatePalette(ph);
}

void RenderBackend_DestroyPalette(unsigned int palette_handle) {
    SDLGameRenderer_DestroyPalette(palette_handle);
}

void RenderBackend_UnlockPalette(unsigned int th) {
    SDLGameRenderer_UnlockPalette(th);
}

void RenderBackend_SetTexture(unsigned int th) {
    SDLGameRenderer_SetTexture(th);
}

void RenderBackend_DrawTexturedQuad(const Sprite* sprite, unsigned int color) {
    SDLGameRenderer_DrawTexturedQuad(sprite, color);
}

void RenderBackend_DrawSprite(const Sprite* sprite, unsigned int color) {
    SDLGameRenderer_DrawSprite(sprite, color);
}

void RenderBackend_DrawSprite2(const Sprite2* sprite2) {
    SDLGameRenderer_DrawSprite2(sprite2);
}

void RenderBackend_DrawSolidQuad(const Quad* quad, unsigned int color) {
    SDLGameRenderer_DrawSolidQuad(quad, color);
}
