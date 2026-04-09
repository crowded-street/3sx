#include "port/render_backend.h"

#include "port/sdl/sdl_game_renderer.h"

const RenderBackendOps g_render_backend = {
    .init = SDLGameRenderer_Init,
    .shutdown = SDLGameRenderer_Shutdown,
    .begin_frame = SDLGameRenderer_BeginFrame,
    .render_frame = SDLGameRenderer_RenderFrame,
    .end_frame = SDLGameRenderer_EndFrame,
    .get_canvas_handle = SDLGameRenderer_GetCanvasHandle,
    .create_texture = SDLGameRenderer_CreateTexture,
    .destroy_texture = SDLGameRenderer_DestroyTexture,
    .unlock_texture = SDLGameRenderer_UnlockTexture,
    .create_palette = SDLGameRenderer_CreatePalette,
    .destroy_palette = SDLGameRenderer_DestroyPalette,
    .unlock_palette = SDLGameRenderer_UnlockPalette,
    .set_texture = SDLGameRenderer_SetTexture,
    .draw_textured_quad = SDLGameRenderer_DrawTexturedQuad,
    .draw_sprite = SDLGameRenderer_DrawSprite,
    .draw_sprite2 = SDLGameRenderer_DrawSprite2,
    .draw_solid_quad = SDLGameRenderer_DrawSolidQuad,
};
