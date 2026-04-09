/**
 * @file renderer.c
 * @brief Renderer_ dispatch through the shared host render backend.
 */

#include "port/render_backend.h"
#include "rendering/game_renderer.h"

void Renderer_CreateTexture(unsigned int th) {
    RenderBackend_CreateTexture(th);
}

void Renderer_DestroyTexture(unsigned int texture_handle) {
    RenderBackend_DestroyTexture(texture_handle);
}

void Renderer_UnlockTexture(unsigned int th) {
    RenderBackend_UnlockTexture(th);
}

void Renderer_CreatePalette(unsigned int ph) {
    RenderBackend_CreatePalette(ph);
}

void Renderer_DestroyPalette(unsigned int palette_handle) {
    RenderBackend_DestroyPalette(palette_handle);
}

void Renderer_UnlockPalette(unsigned int th) {
    RenderBackend_UnlockPalette(th);
}

void Renderer_SetTexture(unsigned int th) {
    RenderBackend_SetTexture(th);
}

void Renderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color) {
    RenderBackend_DrawTexturedQuad(sprite, color);
}

void Renderer_DrawSprite(const Sprite* sprite, unsigned int color) {
    RenderBackend_DrawSprite(sprite, color);
}

void Renderer_DrawSprite2(const Sprite2* sprite2) {
    RenderBackend_DrawSprite2(sprite2);
}

void Renderer_DrawSolidQuad(const Quad* quad, unsigned int color) {
    RenderBackend_DrawSolidQuad(quad, color);
}
