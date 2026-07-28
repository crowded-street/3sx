#ifndef GLYPH_RENDERER_H
#define GLYPH_RENDERER_H

#include <SDL3/SDL.h>

typedef struct GlyphPosition {
    Uint8 x;
    Uint8 y;
} GlyphPosition;

bool GlyphRenderer_Init();
void GlyphRenderer_DrawGlyph(GlyphPosition atlas_pos, GlyphPosition screen_pos, Uint32 color, float z);

#endif
