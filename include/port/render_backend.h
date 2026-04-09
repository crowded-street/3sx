#ifndef PORT_RENDER_BACKEND_H
#define PORT_RENDER_BACKEND_H

#include "port/host_context.h"
#include "rendering/game_renderer.h"

void RenderBackend_Init(const PlatformHostContext* host_context);
void RenderBackend_Shutdown();
void RenderBackend_BeginFrame();
void RenderBackend_RenderFrame();
void RenderBackend_EndFrame();
void* RenderBackend_GetCanvasHandle();

void RenderBackend_CreateTexture(unsigned int th);
void RenderBackend_DestroyTexture(unsigned int texture_handle);
void RenderBackend_UnlockTexture(unsigned int th);
void RenderBackend_CreatePalette(unsigned int ph);
void RenderBackend_DestroyPalette(unsigned int palette_handle);
void RenderBackend_UnlockPalette(unsigned int th);
void RenderBackend_SetTexture(unsigned int th);
void RenderBackend_DrawTexturedQuad(const Sprite* sprite, unsigned int color);
void RenderBackend_DrawSprite(const Sprite* sprite, unsigned int color);
void RenderBackend_DrawSprite2(const Sprite2* sprite2);
void RenderBackend_DrawSolidQuad(const Quad* quad, unsigned int color);

#endif
