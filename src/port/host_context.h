#ifndef PORT_HOST_CONTEXT_H
#define PORT_HOST_CONTEXT_H

typedef enum PlatformHostBackendKind {
    PLATFORM_HOST_BACKEND_NONE = 0,
    PLATFORM_HOST_BACKEND_SDL = 1,
} PlatformHostBackendKind;

typedef struct PlatformHostContext {
    PlatformHostBackendKind backend_kind;
    void* window;
    void* renderer;
    void* graphics_context;
} PlatformHostContext;

#endif
