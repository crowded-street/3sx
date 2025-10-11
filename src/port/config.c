#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "port/config.h"
#include "port/port.h"

#include <SDL3/SDL.h>

#define CONFIG_FILENAME "config"

static char* trim_whitespace(char* str) {
    char* end;

    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    end[1] = '\0';

    return str;
}

static bool parse_bool(const char* value) {
    if (SDL_strcasecmp(value, "true") == 0 || SDL_strcasecmp(value, "yes") == 0 ||
        SDL_strcasecmp(value, "1") == 0 || SDL_strcasecmp(value, "on") == 0)
        return true;
    return false;
}

static const char* bool_to_string(bool value) {
    return value ? "true" : "false";
}

typedef enum {
    CONFIG_TYPE_BOOL,
    CONFIG_TYPE_INT,
} ConfigType;

typedef struct {
    const char* key;
    ConfigType type;
    void* value_ptr;
} ConfigEntry;

#define CONFIG_ENTRY_COUNT 3

static const ConfigEntry* get_config_entries(const Config* config, ConfigEntry* entries) {
    entries[0] = (ConfigEntry){"fullscreen", CONFIG_TYPE_BOOL, (void*)&config->fullscreen};
    entries[1] = (ConfigEntry){"width", CONFIG_TYPE_INT, (void*)&config->width};
    entries[2] = (ConfigEntry){"height", CONFIG_TYPE_INT, (void*)&config->height};
    return entries;
}

static void Config_Init(Config* config) {
    if (!config)
        return;

    config->fullscreen = false;
    config->width = 640;
    config->height = 480;
}

static char* Config_GetPath(void) {
    char* pref_path = SDL_GetPrefPath(PORT_ORG_NAME, PORT_APP_NAME);
    if (!pref_path) {
        SDL_Log("Failed to get preference path: %s", SDL_GetError());
        return NULL;
    }

    size_t path_len = strlen(pref_path) + strlen(CONFIG_FILENAME) + 1;
    char* config_path = SDL_malloc(path_len);
    if (!config_path) {
        SDL_free(pref_path);
        return NULL;
    }

    SDL_snprintf(config_path, path_len, "%s%s", pref_path, CONFIG_FILENAME);
    SDL_free(pref_path);

    return config_path;
}

bool Config_Load(Config* config) {
    if (!config)
        return false;

    Config_Init(config);

    ConfigEntry entries[CONFIG_ENTRY_COUNT];
    get_config_entries(config, entries);

    char* config_path = Config_GetPath();
    if (!config_path) {
        return false;
    }

    size_t file_size;
    char* file_contents = SDL_LoadFile(config_path, &file_size);

    if (!file_contents) {
        // File doesn't exist - create it with default values
        SDL_free(config_path);
        return Config_Save(config);
    }

    SDL_free(config_path);

    char* line_start = file_contents;
    char* line_end = file_contents;
    int line_number = 0;

    while (line_end < file_contents + file_size) {
        line_number++;

        // Find end of line
        while (line_end < file_contents + file_size && *line_end != '\n' && *line_end != '\r') {
            line_end++;
        }

        // Create a null-terminated copy of the line
        size_t line_len = line_end - line_start;
        if (line_len > 0) {
            char* line = SDL_malloc(line_len + 1);
            if (!line) {
                SDL_free(file_contents);
                return false;
            }
            memcpy(line, line_start, line_len);
            line[line_len] = '\0';

            char* trimmed = trim_whitespace(line);

            // Skip empty lines and comments
            if (trimmed[0] != '\0' && trimmed[0] != '#') {
                // Parse key-value pair
                char* saveptr;
                char* key = SDL_strtok_r(trimmed, " \t=", &saveptr);
                char* value = SDL_strtok_r(NULL, " \t=", &saveptr);

                if (key && value) {
                    value = trim_whitespace(value);

                    bool found = false;
                    for (size_t i = 0; i < CONFIG_ENTRY_COUNT; i++) {
                        if (strcmp(key, entries[i].key) == 0) {
                            found = true;
                            switch (entries[i].type) {
                            case CONFIG_TYPE_BOOL:
                                *(bool*)entries[i].value_ptr = parse_bool(value);
                                break;
                            case CONFIG_TYPE_INT: {
                                char* endptr;
                                long val = strtol(value, &endptr, 10);
                                if (*endptr == '\0' && endptr != value) {
                                    *(int*)entries[i].value_ptr = (int)val;
                                } else {
                                    SDL_Log("Config warning: Invalid integer value '%s' for %s on line %d",
                                            value, key, line_number);
                                }
                                break;
                            }
                            }
                            break;
                        }
                    }

                    if (!found) {
                        SDL_Log("Config warning: Unknown config key '%s' on line %d", key, line_number);
                    }
                } else {
                    SDL_Log("Config warning: Line %d has no value, skipping", line_number);
                }
            }

            SDL_free(line);
        }

        while (line_end < file_contents + file_size && (*line_end == '\n' || *line_end == '\r')) {
            line_end++;
        }
        line_start = line_end;
    }

    SDL_free(file_contents);

    // Save config to ensure any new entries are written to disk
    return Config_Save(config);
}

bool Config_Save(const Config* config) {
    if (!config)
        return false;

    ConfigEntry entries[CONFIG_ENTRY_COUNT];
    get_config_entries(config, entries);

    char* config_path = Config_GetPath();
    if (!config_path) {
        return false;
    }

    SDL_IOStream* file = SDL_IOFromFile(config_path, "w");
    if (!file) {
        SDL_Log("Failed to open config file for writing: %s", config_path);
        SDL_free(config_path);
        return false;
    }

    SDL_free(config_path);

    for (size_t i = 0; i < CONFIG_ENTRY_COUNT; i++) {
        switch (entries[i].type) {
        case CONFIG_TYPE_BOOL:
            SDL_IOprintf(file, "%s = %s\n", entries[i].key, bool_to_string(*(bool*)entries[i].value_ptr));
            break;
        case CONFIG_TYPE_INT:
            SDL_IOprintf(file, "%s = %d\n", entries[i].key, *(int*)entries[i].value_ptr);
            break;
        }
    }

    SDL_CloseIO(file);
    return true;
}

