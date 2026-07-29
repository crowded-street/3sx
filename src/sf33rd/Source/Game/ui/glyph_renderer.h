#ifndef GLYPH_RENDERER_H
#define GLYPH_RENDERER_H

#include <SDL3/SDL.h>

typedef struct GlyphPosition {
    Uint16 x;
    Uint16 y;
} GlyphPosition;

#define GLYPH_UP (GlyphPosition) { 27, 2 }
#define GLYPH_DOWN (GlyphPosition) { 29, 2 }
#define GLYPH_LEFT (GlyphPosition) { 30, 2 }
#define GLYPH_RIGHT (GlyphPosition) { 28, 2 }
#define GLYPH_UP_LEFT (GlyphPosition) { 31, 2 }
#define GLYPH_UP_RIGHT (GlyphPosition) { 32, 2 }
#define GLYPH_DOWN_LEFT (GlyphPosition) { 34, 2 }
#define GLYPH_DOWN_RIGHT (GlyphPosition) { 33, 2 }
#define GLYPH_PUNCH (GlyphPosition) { 35, 2 }
#define GLYPH_KICK (GlyphPosition) { 36, 2 }

bool GlyphRenderer_Init();
void GlyphRenderer_DrawGlyph(GlyphPosition atlas_pos, GlyphPosition screen_pos, Uint32 color, float z);
void GlyphRenderer_DrawDigit(Uint8 digit, GlyphPosition screen_pos, Uint32 color, float z);

#endif
