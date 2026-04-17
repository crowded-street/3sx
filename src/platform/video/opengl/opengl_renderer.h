#if CRS_VIDEO_DRIVER_OPENGL

#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "core/render_primitives.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

// Public

void OpenGLRenderer_CreateTexture(unsigned int th);
void OpenGLRenderer_DestroyTexture(unsigned int texture_handle);
void OpenGLRenderer_UnlockTexture(unsigned int th);
void OpenGLRenderer_CreatePalette(unsigned int ph);
void OpenGLRenderer_DestroyPalette(unsigned int palette_handle);
void OpenGLRenderer_UnlockPalette(unsigned int ph);
void OpenGLRenderer_SetTexture(unsigned int th);
void OpenGLRenderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color);
void OpenGLRenderer_DrawSprite(const Sprite* sprite, unsigned int color);
void OpenGLRenderer_DrawSprite2(const Sprite2* sprite2);
void OpenGLRenderer_DrawSolidQuad(const Quad* quad, unsigned int color);

// Internal

bool OpenGLRenderer_Init(bool nearest_filter, int scale);
void OpenGLRenderer_Quit();
void OpenGLRenderer_RenderFrame(SDL_Rect viewport);

#endif

#endif // CRS_VIDEO_DRIVER_OPENGL
