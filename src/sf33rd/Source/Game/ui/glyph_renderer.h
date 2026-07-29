#ifndef GLYPH_RENDERER_H
#define GLYPH_RENDERER_H

#include <SDL3/SDL.h>

typedef struct GlyphPosition {
    Uint16 x;
    Uint16 y;
} GlyphPosition;

typedef enum GlyphColor {
    GLYPH_COLOR_DEFAULT, /// Default blue color
    GLYPH_COLOR_WHITE,   /// Neutral white color
    GLYPH_COLOR_LIGHT,   /// Light blue color primarily associated with light normals
    GLYPH_COLOR_MEDIUM,  /// Yellow color primarily associated with medium normals
    GLYPH_COLOR_HEAVY,   /// Red color primarily associated with heavy normals
} GlyphColor;

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
void GlyphRenderer_DrawGlyph(GlyphPosition atlas_pos, GlyphPosition screen_pos, GlyphColor color, float z);
void GlyphRenderer_DrawDigit(Uint8 digit, GlyphPosition screen_pos, GlyphColor color, float z);

#endif
