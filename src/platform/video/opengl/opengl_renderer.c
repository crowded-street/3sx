#if CRS_VIDEO_DRIVER_OPENGL

#include "platform/video/opengl/opengl_renderer.h"
#include "common.h"
#include "port/utils.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#include "glad.h"
#include "stb/stb_ds.h"
#include <SDL3/SDL.h>

#include <libgraph.h>

#define QUADS_MAX 1024

typedef enum GLPaletteType : Uint8 {
    PALETTE_NONE = 0,
    PALETTE_4,
    PALETTE_8,
} GLPaletteType;

typedef struct GLVec2 {
    float x;
    float y;
} GLVec2;

typedef struct GLVec3 {
    float x;
    float y;
    float z;
} GLVec3;

typedef struct GLTexCoord {
    float s;
    float t;
} GLTexCoord;

typedef struct GLColor {
    float r;
    float g;
    float b;
    float a;
} GLColor;

typedef struct GLVertex {
    GLVec3 position;
    GLTexCoord tex_coord;
    GLColor color;
} GLVertex;

typedef struct GLTexture {
    GLuint handle;
    Uint16 width;
    Uint16 height;
    GLPaletteType palette_type;
} GLTexture;

typedef struct GLTextureSpec {
    GLTexture texture;
    GLuint palette;
} GLTextureSpec;

typedef struct GLQuad {
    GLVec2 positions[4];
    GLTexCoord tex_coords[4];
    float z;
    Uint32 index;
    GLTextureSpec texture_spec;
    GLColor color;
} GLQuad;

static GLuint canvas_fbo = 0;
static GLuint canvas_color_tex = 0;
static GLuint canvas_depth_tex = 0;

static bool screen_texture_nearest_filter = true;
static int screen_texture_scale = 1;
static bool screen_texture_initialized = false;
static SDL_Rect last_viewport = { 0 };
static GLuint screen_fbo = 0;
static GLuint screen_color_tex = 0;

static GLuint solid_shader = 0;
static GLuint palette_4_shader = 0;
static GLuint palette_8_shader = 0;
static GLuint direct_shader = 0;

static GLuint vertex_array = 0;
static GLuint vertex_buffer = 0;
static GLQuad* quads = NULL;
static GLVertex* vertices = NULL;

static GLTexture textures[FL_TEXTURE_MAX] = { 0 };
static GLuint palettes[FL_PALETTE_MAX] = { 0 };
static GLTextureSpec latest_texture_spec = { 0 };

static const char* read_shader(const char* path) {
    const char* base_path = SDL_GetBasePath();
    char* full_path = NULL;
    SDL_asprintf(&full_path, "%s/%s", base_path, path);

    SDL_IOStream* io = SDL_IOFromFile(full_path, "r");
    SDL_free(full_path);

    if (io == NULL) {
        return NULL;
    }

    const Sint64 size = SDL_GetIOSize(io);
    char* buf = SDL_malloc(size + 1);
    SDL_ReadIO(io, buf, size);
    buf[size] = '\0';
    SDL_CloseIO(io);
    return buf;
}

static GLuint build_shader_program(const char* vertex_shader_path, const char* fragment_shader_path) {
    char info_log[512];
    GLint success;

    // Vertex shader

    const char* vertex_shader_source = read_shader(vertex_shader_path);
    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, sizeof(info_log), NULL, info_log);
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Error creating vertex shader: %s", info_log);
    }

    // Fragment shader

    const char* fragment_shader_source = read_shader(fragment_shader_path);
    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment_shader, sizeof(info_log), NULL, info_log);
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Error creating fragment shader: %s", info_log);
    }

    // Link shaders

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Error creating shader program: %s", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

static void configure_screen_texture(SDL_Rect viewport) {
    if (!screen_texture_initialized) {
        glGenTextures(1, &screen_color_tex);
        glBindTexture(GL_TEXTURE_2D, screen_color_tex);
        const GLint filter = screen_texture_nearest_filter ? GL_NEAREST : GL_LINEAR;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA8,
                     viewport.w * screen_texture_scale,
                     viewport.h * screen_texture_scale,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     NULL);

        glGenFramebuffers(1, &screen_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, screen_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_color_tex, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        screen_texture_initialized = true;
    } else if (last_viewport.w != viewport.w || last_viewport.h != viewport.h) {
        glBindTexture(GL_TEXTURE_2D, screen_color_tex);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA8,
                     viewport.w * screen_texture_scale,
                     viewport.h * screen_texture_scale,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     NULL);

        // Re-attach the texture just in case
        glBindFramebuffer(GL_FRAMEBUFFER, screen_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_color_tex, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    last_viewport = viewport;
}

static float convert_to_screen_x(float x) {
    x /= 384;
    x *= 2;
    x -= 1;
    return x;
}

static float convert_to_screen_y(float y) {
    y /= 224;
    y *= 2;
    y -= 1;
    y *= -1;
    return y;
}

static float convert_color_component(Uint8 component) {
    return (float)component / (float)SDL_MAX_UINT8;
}

static GLColor convert_color(Uint32 color) {
    return (GLColor) {
        .r = convert_color_component((color >> 16) & 0xFF),
        .g = convert_color_component((color >> 8) & 0xFF),
        .b = convert_color_component(color & 0xFF),
        .a = convert_color_component(color >> 24),
    };
}

static void quad_infer_positions(GLQuad* quad) {
    quad->positions[1].x = quad->positions[3].x;
    quad->positions[1].y = quad->positions[0].y;
    quad->positions[2].x = quad->positions[0].x;
    quad->positions[2].y = quad->positions[3].y;
}

static void quad_infer_tex_coords(GLQuad* quad) {
    quad->tex_coords[1].s = quad->tex_coords[3].s;
    quad->tex_coords[1].t = quad->tex_coords[0].t;
    quad->tex_coords[2].s = quad->tex_coords[0].s;
    quad->tex_coords[2].t = quad->tex_coords[3].t;
}

// Public

void OpenGLRenderer_CreateTexture(unsigned int th) {
    const int texture_index = LO_16_BITS(th) - 1;
    const FLTexture* fl_texture = &flTexture[texture_index];
    const void* pixels = flPS2GetSystemBuffAdrs(fl_texture->mem_handle);

    if (textures[texture_index].handle != 0) {
        glDeleteTextures(1, &textures[texture_index].handle);
        SDL_zero(textures[texture_index]);
    }

    GLint internal_format;
    GLenum format;
    GLenum type;
    GLPaletteType palette_type = PALETTE_NONE;
    GLsizei buffer_width = fl_texture->width;

    switch (fl_texture->format) {
    case SCE_GS_PSMT8:
        internal_format = GL_R8UI;
        format = GL_RED_INTEGER;
        type = GL_UNSIGNED_BYTE;
        palette_type = PALETTE_8;
        break;

    case SCE_GS_PSMT4:
        internal_format = GL_R8UI;
        format = GL_RED_INTEGER;
        type = GL_UNSIGNED_BYTE;
        palette_type = PALETTE_4;
        buffer_width /= 2;
        break;

    case SCE_GS_PSMCT16:
        internal_format = GL_RGB5_A1;
        format = GL_RGBA;
        type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
        palette_type = PALETTE_NONE;
        break;

    default:
        fatal_error("Unhandled pixel format: %d", fl_texture->format);
        break;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, buffer_width, fl_texture->height, 0, format, type, pixels);

    textures[texture_index] = (GLTexture) {
        .handle = texture,
        .width = fl_texture->width,
        .height = fl_texture->height,
        .palette_type = palette_type,
    };
}

void OpenGLRenderer_DestroyTexture(unsigned int texture_handle) {
    // Do nothing
}

void OpenGLRenderer_UnlockTexture(unsigned int th) {
    OpenGLRenderer_CreateTexture(th);
}

void OpenGLRenderer_CreatePalette(unsigned int ph) {
    const int palette_index = HI_16_BITS(ph) - 1;
    const FLTexture* fl_palette = &flPalette[palette_index];
    const void* pixels = flPS2GetSystemBuffAdrs(fl_palette->mem_handle);
    const int color_count = fl_palette->width * fl_palette->height;

    if (palettes[palette_index] != 0) {
        glDeleteTextures(1, &palettes[palette_index]);
        palettes[palette_index] = 0;
    }

    GLint internal_format;
    GLenum format;
    GLenum type;

    switch (fl_palette->format) {
    case SCE_GS_PSMCT32:
        internal_format = GL_RGBA8;
        format = GL_BGRA;
        type = GL_UNSIGNED_BYTE;
        break;

    case SCE_GS_PSMCT16:
        internal_format = GL_RGB5_A1;
        format = GL_RGBA;
        type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
        break;

    default:
        fatal_error("Unhandled pixel format: %d", fl_palette->format);
        break;
    }

    GLuint palette;
    glGenTextures(1, &palette);
    glBindTexture(GL_TEXTURE_1D, palette);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage1D(GL_TEXTURE_1D, 0, internal_format, color_count, 0, format, type, pixels);

    palettes[palette_index] = palette;
}

void OpenGLRenderer_DestroyPalette(unsigned int palette_handle) {
    // Do nothing
}

void OpenGLRenderer_UnlockPalette(unsigned int ph) {
    OpenGLRenderer_CreatePalette(ph << 16);
}

void OpenGLRenderer_SetTexture(unsigned int th) {
    const int texture_handle = LO_16_BITS(th);
    const int texture_index = texture_handle - 1;
    latest_texture_spec.texture = textures[texture_index];

    const int palette_handle = HI_16_BITS(th);

    if (palette_handle > 0) {
        const int palette_index = palette_handle - 1;
        latest_texture_spec.palette = palettes[palette_index];
    } else {
        latest_texture_spec.palette = 0;
    }
}

void OpenGLRenderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color) {
    SDL_assert(arrlen(quads) < QUADS_MAX);

    GLQuad* quad = arraddnptr(quads, 1);

    for (int i = 0; i < 4; i++) {
        quad->positions[i].x = convert_to_screen_x(sprite->v[i].x);
        quad->positions[i].y = convert_to_screen_y(sprite->v[i].y);
        quad->tex_coords[i].s = sprite->t[i].s;
        quad->tex_coords[i].t = sprite->t[i].t;
    }

    quad->z = sprite->v[0].z;
    quad->index = arrlen(quads);
    quad->color = convert_color(color);
    quad->texture_spec = latest_texture_spec;
}

void OpenGLRenderer_DrawSprite(const Sprite* sprite, unsigned int color) {
    SDL_assert(arrlen(quads) < QUADS_MAX);

    GLQuad* quad = arraddnptr(quads, 1);

    quad->positions[0].x = convert_to_screen_x(sprite->v[0].x);
    quad->positions[0].y = convert_to_screen_y(sprite->v[0].y);
    quad->positions[3].x = convert_to_screen_x(sprite->v[3].x);
    quad->positions[3].y = convert_to_screen_y(sprite->v[3].y);
    quad_infer_positions(quad);

    quad->tex_coords[0].s = sprite->t[0].s;
    quad->tex_coords[0].t = sprite->t[0].t;
    quad->tex_coords[3].s = sprite->t[3].s;
    quad->tex_coords[3].t = sprite->t[3].t;
    quad_infer_tex_coords(quad);

    quad->z = sprite->v[0].z;
    quad->index = arrlen(quads);
    quad->color = convert_color(color);
    quad->texture_spec = latest_texture_spec;
}

void OpenGLRenderer_DrawSprite2(const Sprite2* sprite2) {
    SDL_assert(arrlen(quads) < QUADS_MAX);

    GLQuad* quad = arraddnptr(quads, 1);

    quad->positions[0].x = convert_to_screen_x(sprite2->v[0].x);
    quad->positions[0].y = convert_to_screen_y(sprite2->v[0].y);
    quad->positions[3].x = convert_to_screen_x(sprite2->v[1].x);
    quad->positions[3].y = convert_to_screen_y(sprite2->v[1].y);
    quad_infer_positions(quad);

    quad->tex_coords[0].s = sprite2->t[0].s;
    quad->tex_coords[0].t = sprite2->t[0].t;
    quad->tex_coords[3].s = sprite2->t[1].s;
    quad->tex_coords[3].t = sprite2->t[1].t;
    quad_infer_tex_coords(quad);

    quad->z = sprite2->v[0].z;
    quad->index = arrlen(quads);
    quad->color = convert_color(sprite2->vertex_color);
    quad->texture_spec = latest_texture_spec;
}

void OpenGLRenderer_DrawSolidQuad(const Quad* quad, unsigned int color) {
    SDL_assert(arrlen(quads) < QUADS_MAX);

    GLQuad* _quad = arraddnptr(quads, 1);

    for (int i = 0; i < 4; i++) {
        _quad->positions[i].x = convert_to_screen_x(quad->v[i].x);
        _quad->positions[i].y = convert_to_screen_y(quad->v[i].y);
    }

    _quad->z = quad->v[0].z;
    _quad->index = arrlen(quads);
    _quad->color = convert_color(color);
    SDL_zero(_quad->texture_spec);
}

// Internal

bool OpenGLRenderer_Init(bool nearest_filter, int scale) {
    screen_texture_nearest_filter = nearest_filter;
    screen_texture_scale = scale;

    arrsetcap(quads, QUADS_MAX);
    vertices = SDL_calloc(QUADS_MAX * 4, sizeof(GLVertex));

    // Configure canvas

    glGenTextures(1, &canvas_color_tex);
    glBindTexture(GL_TEXTURE_2D, canvas_color_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 384, 224, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glGenTextures(1, &canvas_depth_tex);
    glBindTexture(GL_TEXTURE_2D, canvas_depth_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 384, 224, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glGenFramebuffers(1, &canvas_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, canvas_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, canvas_color_tex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, canvas_depth_tex, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Configure shaders

    solid_shader = build_shader_program("shaders/vert.glsl", "shaders/frag_solid.glsl");

    palette_4_shader = build_shader_program("shaders/vert.glsl", "shaders/frag_palette_4.glsl");
    glUseProgram(palette_4_shader);
    glUniform1i(glGetUniformLocation(palette_4_shader, "uPalette"), 0);
    glUniform1i(glGetUniformLocation(palette_4_shader, "uIndexTex"), 1);

    palette_8_shader = build_shader_program("shaders/vert.glsl", "shaders/frag_palette_8.glsl");
    glUseProgram(palette_8_shader);
    glUniform1i(glGetUniformLocation(palette_8_shader, "uPalette"), 0);
    glUniform1i(glGetUniformLocation(palette_8_shader, "uIndexTex"), 1);

    direct_shader = build_shader_program("shaders/vert.glsl", "shaders/frag_direct.glsl");
    glUseProgram(direct_shader);
    glUniform1i(glGetUniformLocation(direct_shader, "uTexture"), 0);

    // Setup vertex data and buffers

    glGenVertexArrays(1, &vertex_array);
    glGenBuffers(1, &vertex_buffer);
    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);

    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, QUADS_MAX * 4 * sizeof(GLVertex), NULL, GL_DYNAMIC_DRAW);

    // Pre-compute indices
    GLuint indices[QUADS_MAX * 6];

    for (int i = 0; i < QUADS_MAX; i++) {
        indices[i * 6 + 0] = i * 4 + 0;
        indices[i * 6 + 1] = i * 4 + 1;
        indices[i * 6 + 2] = i * 4 + 2;
        indices[i * 6 + 3] = i * 4 + 2;
        indices[i * 6 + 4] = i * 4 + 1;
        indices[i * 6 + 5] = i * 4 + 3;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // aPos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid*)offsetof(GLVertex, position));
    glEnableVertexAttribArray(0);
    // aTexCoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid*)offsetof(GLVertex, tex_coord));
    glEnableVertexAttribArray(1);
    // aColor
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid*)offsetof(GLVertex, color));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Misc

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    return true;
}

void OpenGLRenderer_Quit() {
    // TODO: Implement
}

void OpenGLRenderer_RenderFrame(SDL_Rect viewport) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    // Draw to canvas

    glBindFramebuffer(GL_FRAMEBUFFER, canvas_fbo);
    glViewport(0, 0, 384, 224);
	glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < arrlen(quads); i++) {
        const int vertex_base = i * 4;
        const GLQuad* quad = &quads[i];

        for (int i = 0; i < 4; i++) {
            vertices[vertex_base + i].position.x = quad->positions[i].x;
            vertices[vertex_base + i].position.y = quad->positions[i].y;
            vertices[vertex_base + i].position.z = quad->z;
            vertices[vertex_base + i].tex_coord = quad->tex_coords[i];
            vertices[vertex_base + i].color = quad->color;
        }
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLVertex) * arrlen(quads) * 4, vertices);

    for (int i = 0; i < arrlen(quads); i++) {
        const GLQuad* quad = &quads[i];

        if (quad->texture_spec.texture.handle == 0) {
            glUseProgram(solid_shader);
        } else {
            switch (quad->texture_spec.texture.palette_type) {
            case PALETTE_4:
                glUseProgram(palette_4_shader);
                const GLint texture_size_loc = glGetUniformLocation(palette_4_shader, "uTextureSize");
                glUniform2i(texture_size_loc, quad->texture_spec.texture.width, quad->texture_spec.texture.width);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_1D, quad->texture_spec.palette);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, quad->texture_spec.texture.handle);
                break;

            case PALETTE_8:
                SDL_assert(quad->texture_spec.palette != 0);
                glUseProgram(palette_8_shader);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_1D, quad->texture_spec.palette);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, quad->texture_spec.texture.handle);
                break;

            case PALETTE_NONE:
                glUseProgram(direct_shader);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, quad->texture_spec.texture.handle);
                break;
            }
        }

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * 6 * i));
    }

    // Draw to screen texture, then to window

    GLVertex screen_vertices[4] = {
        { .position = { .x = -1, .y = 1 }, .tex_coord = { .s = 0, .t = 1 } },
        { .position = { .x = 1, .y = 1 }, .tex_coord = { .s = 1, .t = 1 } },
        { .position = { .x = -1, .y = -1 }, .tex_coord = { .s = 0, .t = 0 } },
        { .position = { .x = 1, .y = -1 }, .tex_coord = { .s = 1, .t = 0 } },
    };

    for (int i = 0; i < SDL_arraysize(screen_vertices); i++) {
        screen_vertices[i].color = (GLColor) { .r = 1, .g = 1, .b = 1, .a = 1 };
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(screen_vertices), screen_vertices);
    glUseProgram(direct_shader);
    glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

    configure_screen_texture(viewport);
    glBindFramebuffer(GL_FRAMEBUFFER, screen_fbo);
    glViewport(0, 0, viewport.w * screen_texture_scale, viewport.h * screen_texture_scale);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, canvas_color_tex);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screen_color_tex);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);
    glEnable(GL_BLEND);

    // Cleanup

    glBindVertexArray(0);
    arrsetlen(quads, 0);
}

#endif // CRS_VIDEO_DRIVER_OPENGL
