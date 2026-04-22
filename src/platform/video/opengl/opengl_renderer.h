#if CRS_VIDEO_DRIVER_OPENGL

#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "core/render_primitives.h"
#include "platform/video/sdl_generic/sdl_render_backend.h"

#include <SDL3/SDL.h>

// Internal

extern const SDLRenderBackend opengl_render_backend;

#endif

#endif // CRS_VIDEO_DRIVER_OPENGL
