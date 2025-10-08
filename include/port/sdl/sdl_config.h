#ifndef SDL_CONFIG_H
#define SDL_CONFIG_H

#include <stdbool.h>

// Opaque config structure - stores INI sections and key-value pairs
typedef struct SDLConfig SDLConfig;

// Create a new empty config
SDLConfig* SDLConfig_Create(void);

// Free config and all associated memory
void SDLConfig_Destroy(SDLConfig* config);

// Load config from file, merging with existing values
// Returns false if file doesn't exist or can't be read
bool SDLConfig_LoadFile(SDLConfig* config, const char* filepath);

// Save config to file, preserving all sections
// Returns false if file can't be written
bool SDLConfig_SaveFile(const SDLConfig* config, const char* filepath);

// Get string value from section (returns NULL if not found)
const char* SDLConfig_GetString(const SDLConfig* config, const char* section, const char* key);

// Get integer value from section (returns default_value if not found or invalid)
int SDLConfig_GetInt(const SDLConfig* config, const char* section, const char* key, int default_value);

// Get boolean value from section (returns default_value if not found or invalid)
// Accepts: true/false, yes/no, 1/0 (case insensitive)
bool SDLConfig_GetBool(const SDLConfig* config, const char* section, const char* key, bool default_value);

// Set string value in section
void SDLConfig_SetString(SDLConfig* config, const char* section, const char* key, const char* value);

// Set integer value in section
void SDLConfig_SetInt(SDLConfig* config, const char* section, const char* key, int value);

// Set boolean value in section (stored as "true"/"false")
void SDLConfig_SetBool(SDLConfig* config, const char* section, const char* key, bool value);

#endif
