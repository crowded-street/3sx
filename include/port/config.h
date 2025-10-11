#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// Config structure
typedef struct {
    bool fullscreen;
    int width;
    int height;
} Config;

// Load config from file, using defaults if file doesn't exist
// Returns true on success, false on failure
bool Config_Load(Config* config);

// Save config to file
// Returns true on success, false on failure
bool Config_Save(const Config* config);

#endif // CONFIG_H
