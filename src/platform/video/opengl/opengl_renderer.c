#if CRS_VIDEO_DRIVER_OPENGL

#include "platform/video/opengl/opengl_renderer.h"

#include "stb/stb_ds.h"
#include <OpenGL/gl3.h>
#include <SDL3/SDL.h>

#define QUADS_MAX 512

typedef struct GLVec2 {
    float x;
    float y;
} GLVec2;

typedef struct GLVec3 {
    float x;
    float y;
    float z;
} GLVec3;

typedef struct GLColor {
    float r;
    float g;
    float b;
    float a;
} GLColor;

typedef struct GLVertex {
    GLVec3 position;
    GLColor color;
} GLVertex;

typedef struct GLQuad {
    float x0;
    float y0;
    float x1;
    float y1;
    float z;
    GLColor color;
} GLQuad;

static GLuint shader_program;
static GLuint vertex_array;
static GLuint vertex_buffer;
static GLQuad* quads = NULL;
static GLVertex* vertices = NULL;

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
    void* buf = SDL_malloc(size);
    SDL_ReadIO(io, buf, size);
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

static void push_quad(float x0, float y0, float x1, float y1, float z, Uint32 color) {
    SDL_assert(arrlen(quads) < QUADS_MAX - 1);
    GLQuad* quad = arraddnptr(quads, 1);
    quad->x0 = convert_to_screen_x(x0);
    quad->y0 = convert_to_screen_y(y0);
    quad->x1 = convert_to_screen_x(x1);
    quad->y1 = convert_to_screen_y(y1);
    quad->z = z;
    quad->color = convert_color(color);
}

// Public

void OpenGLRenderer_CreateTexture(unsigned int th) {
    //
}

void OpenGLRenderer_DestroyTexture(unsigned int texture_handle) {
    //
}

void OpenGLRenderer_UnlockTexture(unsigned int th) {
    //
}

void OpenGLRenderer_CreatePalette(unsigned int ph) {
    //
}

void OpenGLRenderer_DestroyPalette(unsigned int palette_handle) {
    //
}

void OpenGLRenderer_UnlockPalette(unsigned int ph) {
    //
}

void OpenGLRenderer_SetTexture(unsigned int th) {
    //
}

void OpenGLRenderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color) {
    push_quad(sprite->v[0].x, sprite->v[0].y, sprite->v[3].x, sprite->v[3].y, sprite->v[0].z, color);
}

void OpenGLRenderer_DrawSprite(const Sprite* sprite, unsigned int color) {
    push_quad(sprite->v[0].x, sprite->v[0].y, sprite->v[3].x, sprite->v[3].y, sprite->v[0].z, color);
}

void OpenGLRenderer_DrawSprite2(const Sprite2* sprite2) {
    push_quad(
        sprite2->v[0].x, sprite2->v[0].y, sprite2->v[1].x, sprite2->v[1].y, sprite2->v[0].z, sprite2->vertex_color);
}

void OpenGLRenderer_DrawSolidQuad(const Quad* quad, unsigned int color) {
    push_quad(quad->v[0].x, quad->v[0].y, quad->v[3].x, quad->v[3].y, quad->v[0].z, color);
}

// Internal

bool OpenGLRenderer_Init() {
    arrsetcap(quads, QUADS_MAX);
    vertices = SDL_calloc(QUADS_MAX * 4, sizeof(GLVertex));

    shader_program = build_shader_program("shaders/vert.glsl", "shaders/frag.glsl");

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
    // aColor
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid*)offsetof(GLVertex, color));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    return true;
}

void OpenGLRenderer_Quit() {
    //
}

void OpenGLRenderer_RenderFrame(int window_width, int window_height) {
    glViewport(0, 0, window_width, window_height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vertex_array);

    // Draw quads

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    for (int i = 0; i < arrlen(quads); i++) {
        const int vertex_base = i * 4;
        GLQuad* quad = &quads[i];

        vertices[vertex_base + 0].position.x = vertices[vertex_base + 2].position.x = quad->x0;
        vertices[vertex_base + 0].position.y = vertices[vertex_base + 1].position.y = quad->y0;
        vertices[vertex_base + 1].position.x = vertices[vertex_base + 3].position.x = quad->x1;
        vertices[vertex_base + 2].position.y = vertices[vertex_base + 3].position.y = quad->y1;

        vertices[vertex_base + 0].position.z = vertices[vertex_base + 1].position.z =
            vertices[vertex_base + 2].position.z = vertices[vertex_base + 3].position.z = quad->z;

        vertices[vertex_base + 0].color = vertices[vertex_base + 1].color = vertices[vertex_base + 2].color =
            vertices[vertex_base + 3].color = quad->color;
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLVertex) * arrlen(quads) * 4, vertices);
    glDrawElements(GL_TRIANGLES, 6 * arrlen(quads), GL_UNSIGNED_INT, NULL);

    // Cleanup

    glBindVertexArray(0);
    arrsetlen(quads, 0);
}

#endif // CRS_VIDEO_DRIVER_OPENGL
