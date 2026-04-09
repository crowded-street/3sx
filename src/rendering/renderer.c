/**
 * @file renderer.c
 * @brief Renderer_ dispatch through the selected host render backend ops table.
 */

#include "port/render_backend.h"
#include "rendering/game_renderer.h"

void Renderer_CreateTexture(unsigned int th) {
    g_render_backend.create_texture(th);
}

void Renderer_DestroyTexture(unsigned int texture_handle) {
    g_render_backend.destroy_texture(texture_handle);
}

void Renderer_UnlockTexture(unsigned int th) {
    g_render_backend.unlock_texture(th);
}

void Renderer_CreatePalette(unsigned int ph) {
    g_render_backend.create_palette(ph);
}

void Renderer_DestroyPalette(unsigned int palette_handle) {
    g_render_backend.destroy_palette(palette_handle);
}

void Renderer_UnlockPalette(unsigned int th) {
    g_render_backend.unlock_palette(th);
}

void Renderer_SetTexture(unsigned int th) {
    g_render_backend.set_texture(th);
}

void Renderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color) {
    g_render_backend.draw_textured_quad(sprite, color);
}

void Renderer_DrawSprite(const Sprite* sprite, unsigned int color) {
    g_render_backend.draw_sprite(sprite, color);
}

void Renderer_DrawSprite2(const Sprite2* sprite2) {
    g_render_backend.draw_sprite2(sprite2);
}

void Renderer_DrawSolidQuad(const Quad* quad, unsigned int color) {
    g_render_backend.draw_solid_quad(quad, color);
}
