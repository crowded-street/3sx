/**
 * @brief Cross-platform renderer interface for the game core.
 *
 * Game logic calls Renderer_* functions for all rendering operations.
 * Each platform provides implementations in its GPU backend.
 */

#ifndef CORE_RENDERER_H
#define CORE_RENDERER_H

#include "core/render_primitives.h"

void Renderer_CreateTexture(unsigned int th);
void Renderer_DestroyTexture(unsigned int texture_handle);
void Renderer_UnlockTexture(unsigned int th);
void Renderer_CreatePalette(unsigned int ph);
void Renderer_DestroyPalette(unsigned int palette_handle);
void Renderer_UnlockPalette(unsigned int th);
void Renderer_SetTexture(unsigned int th);
void Renderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color);
void Renderer_DrawSprite(const Sprite* sprite, unsigned int color);
void Renderer_DrawSprite2(const Sprite2* sprite2);
void Renderer_DrawSolidQuad(const Quad* quad, unsigned int color);

#endif
