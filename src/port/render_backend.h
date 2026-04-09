#ifndef PORT_RENDER_BACKEND_H
#define PORT_RENDER_BACKEND_H

#include "port/host_context.h"
#include "rendering/game_renderer.h"

typedef struct RenderBackendOps {
    void (*init)(const PlatformHostContext* host_context);
    void (*shutdown)();
    void (*begin_frame)();
    void (*render_frame)();
    void (*end_frame)();
    void* (*get_canvas_handle)();

    void (*create_texture)(unsigned int th);
    void (*destroy_texture)(unsigned int texture_handle);
    void (*unlock_texture)(unsigned int th);
    void (*create_palette)(unsigned int ph);
    void (*destroy_palette)(unsigned int palette_handle);
    void (*unlock_palette)(unsigned int th);
    void (*set_texture)(unsigned int th);
    void (*draw_textured_quad)(const Sprite* sprite, unsigned int color);
    void (*draw_sprite)(const Sprite* sprite, unsigned int color);
    void (*draw_sprite2)(const Sprite2* sprite2);
    void (*draw_solid_quad)(const Quad* quad, unsigned int color);
} RenderBackendOps;

extern const RenderBackendOps g_render_backend;

#endif
