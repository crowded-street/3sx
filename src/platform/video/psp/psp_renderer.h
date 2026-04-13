#ifndef PSP_RENDERER_H
#define PSP_RENDERER_H

#include "core/render_primitives.h"

void PSPRenderer_Init();
void PSPRenderer_Shutdown();
void PSPRenderer_BeginFrame();
void PSPRenderer_RenderFrame();
void PSPRenderer_EndFrame();

void PSPRenderer_CreateTexture(unsigned int th);
void PSPRenderer_DestroyTexture(unsigned int texture_handle);
void PSPRenderer_UnlockTexture(unsigned int th);
void PSPRenderer_CreatePalette(unsigned int ph);
void PSPRenderer_DestroyPalette(unsigned int palette_handle);
void PSPRenderer_UnlockPalette(unsigned int ph);
void PSPRenderer_SetTexture(unsigned int th);
void PSPRenderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color);
void PSPRenderer_DrawSprite(const Sprite* sprite, unsigned int color);
void PSPRenderer_DrawSprite2(const Sprite2* sprite2);
void PSPRenderer_DrawSolidQuad(const Quad* quad, unsigned int color);

#endif
