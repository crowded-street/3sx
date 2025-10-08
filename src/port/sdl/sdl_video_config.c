#include "port/sdl/sdl_video_config.h"
#include "port/sdl/sdl_config.h"

// Default window dimensions
static const int SDL_DEFAULT_WINDOW_WIDTH = 640;
static const int SDL_DEFAULT_WINDOW_HEIGHT = 480;

void SDLVideoConfig_Init(SDLVideoConfig* config) {
    config->fullscreen = false;
    config->window_width = SDL_DEFAULT_WINDOW_WIDTH;
    config->window_height = SDL_DEFAULT_WINDOW_HEIGHT;
}

void SDLVideoConfig_LoadFromConfig(SDLVideoConfig* config, const SDLConfig* cfg) {
    config->fullscreen = SDLConfig_GetBool(cfg, "video", "fullscreen", false);
    config->window_width = SDLConfig_GetInt(cfg, "video", "window_width", SDL_DEFAULT_WINDOW_WIDTH);
    config->window_height = SDLConfig_GetInt(cfg, "video", "window_height", SDL_DEFAULT_WINDOW_HEIGHT);
}

void SDLVideoConfig_SaveToConfig(const SDLVideoConfig* config, SDLConfig* cfg) {
    SDLConfig_SetBool(cfg, "video", "fullscreen", config->fullscreen);
    SDLConfig_SetInt(cfg, "video", "window_width", config->window_width);
    SDLConfig_SetInt(cfg, "video", "window_height", config->window_height);
}
