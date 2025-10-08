#include "port/sdl/sdl_debug_config.h"
#include "port/sdl/sdl_config.h"

// Default debug settings
static const bool SDL_DEFAULT_SHOW_FPS = false;
static const bool SDL_DEFAULT_SHOW_INPUTS = false;

void SDLDebugConfig_Init(SDLDebugConfig* config) {
    config->show_fps = SDL_DEFAULT_SHOW_FPS;
    config->show_inputs = SDL_DEFAULT_SHOW_INPUTS;
}

void SDLDebugConfig_LoadFromConfig(SDLDebugConfig* config, const SDLConfig* cfg) {
    config->show_fps = SDLConfig_GetBool(cfg, "debug", "show_fps", SDL_DEFAULT_SHOW_FPS);
    config->show_inputs = SDLConfig_GetBool(cfg, "debug", "show_inputs", SDL_DEFAULT_SHOW_INPUTS);
}

void SDLDebugConfig_SaveToConfig(const SDLDebugConfig* config, SDLConfig* cfg) {
    SDLConfig_SetBool(cfg, "debug", "show_fps", config->show_fps);
    SDLConfig_SetBool(cfg, "debug", "show_inputs", config->show_inputs);
}
