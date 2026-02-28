#ifndef PORT_CONFIG_H
#define PORT_CONFIG_H

#include <stdbool.h>

#define CFG_KEY_FULLSCREEN "fullscreen"
#define CFG_KEY_WINDOW_WIDTH "window-width"
#define CFG_KEY_WINDOW_HEIGHT "window-height"
#define CFG_KEY_SCALEMODE "scale-mode"
#define CFG_DRAW_PLAYER_ABOVE_HUD "draw-players-above-hud"

/// Initialize config system
void Config_Init();

/// Destroy resources used by config system
void Config_Destroy();

/// Get the value associated with the given key as a `bool`
/// @return The value associated with `key` if `key` is among entries and the value's type is `bool`, `false` otherwise
bool Config_GetBool(const char* key);

/// Get the value associated with the given key as an `int`
/// @return The value associated with `key` if `key` is among entries and the value's type is `int`, `0` otherwise
int Config_GetInt(const char* key);

/// Get the value associated with the given key as a `string`
/// @return The value associated with `key` if `key` is among entries and the value's type is `string`, `NULL` otherwise
const char* Config_GetString(const char* key);

/// Get the value that shifts the value by HUD_SHIFT is CFG_DRAW_PLAYER_ABOVE_HUD is true
/// @return The value associated with `val` plus HUD_SHIFT if `val` if CFG_DRAW_PLAYER_ABOVE_HUD is true, `val` otherwise
int Config_GetHUDShift(int val);

#endif
