#include "port/config/keymap.h"
#include "port/config/config_helpers.h"
#include "port/paths.h"

#include <SDL3/SDL.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

static const SDL_Scancode default_keymap[KEYMAP_BUTTON_COUNT][KEYMAP_CODES_PER_BUTTON] = {
    { SDL_SCANCODE_UP, SDL_SCANCODE_W, SDL_SCANCODE_SPACE }, // up
    { SDL_SCANCODE_DOWN, SDL_SCANCODE_S },                   // down
    { SDL_SCANCODE_LEFT, SDL_SCANCODE_A },                   // left
    { SDL_SCANCODE_RIGHT, SDL_SCANCODE_D },                  // right
    { SDL_SCANCODE_I },                                      // north
    { SDL_SCANCODE_U },                                      // west
    { SDL_SCANCODE_J },                                      // south
    { SDL_SCANCODE_K },                                      // east
    { SDL_SCANCODE_P },                                      // left shoulder
    { SDL_SCANCODE_O },                                      // right shoulder
    { SDL_SCANCODE_SEMICOLON },                              // left trigger
    { SDL_SCANCODE_L },                                      // right trigger
    { SDL_SCANCODE_9 },                                      // left stick
    { SDL_SCANCODE_0 },                                      // right stick
    { SDL_SCANCODE_BACKSPACE },                              // back
    { SDL_SCANCODE_RETURN },                                 // start
};

static SDL_Scancode keymap[KEYMAP_BUTTON_COUNT][KEYMAP_CODES_PER_BUTTON] = {};
static bool initialized_buttons[KEYMAP_BUTTON_COUNT] = { false };

static const char* get_button_name(KeymapButton button) {
    switch (button) {
    case KEYMAP_BUTTON_UP:
        return "up";
    case KEYMAP_BUTTON_DOWN:
        return "down";
    case KEYMAP_BUTTON_LEFT:
        return "left";
    case KEYMAP_BUTTON_RIGHT:
        return "right";
    case KEYMAP_BUTTON_NORTH:
        return "north";
    case KEYMAP_BUTTON_WEST:
        return "west";
    case KEYMAP_BUTTON_SOUTH:
        return "south";
    case KEYMAP_BUTTON_EAST:
        return "east";
    case KEYMAP_BUTTON_LEFT_SHOULDER:
        return "left-shoulder";
    case KEYMAP_BUTTON_RIGHT_SHOULDER:
        return "right-shoulder";
    case KEYMAP_BUTTON_LEFT_TRIGGER:
        return "left-trigger";
    case KEYMAP_BUTTON_RIGHT_TRIGGER:
        return "right-trigger";
    case KEYMAP_BUTTON_LEFT_STICK:
        return "left-stick";
    case KEYMAP_BUTTON_RIGHT_STICK:
        return "right-stick";
    case KEYMAP_BUTTON_BACK:
        return "back";
    case KEYMAP_BUTTON_START:
        return "start";
    }
}

static KeymapButton get_button(const char* name) {
    for (int i = 0; i < KEYMAP_BUTTON_COUNT; i++) {
        const char* this_name = get_button_name(i);

        if (SDL_strcmp(name, this_name) == 0) {
            return i;
        }
    }

    return -1;
}

static void write_defaults(const char* dst_path) {
    SDL_IOStream* io = SDL_IOFromFile(dst_path, "w");

    for (int i = 0; i < KEYMAP_BUTTON_COUNT; i++) {
        io_printf(io, "%s = ", get_button_name(i));

        bool is_first = true;

        for (int j = 0; j < KEYMAP_CODES_PER_BUTTON; j++) {
            const SDL_Scancode code = default_keymap[i][j];

            if (code == SDL_SCANCODE_UNKNOWN) {
                break;
            }

            if (!is_first) {
                io_printf(io, ", ");
            }

            is_first = false;
            io_printf(io, SDL_GetScancodeName(code));
        }

        io_printf(io, "\n");
    }

    SDL_CloseIO(io);
}

static bool dict_iterator(const char* key, const char* value) {
    const KeymapButton button = get_button(key);

    if (button == -1) {
        return true;
    }

    int code_index = 0;
    char val[128];
    SDL_strlcpy(val, value, sizeof(val));

    char name[32];
    char* saveptr;
    char* token = SDL_strtok_r(val, ",", &saveptr);

    while (token != NULL && code_index < KEYMAP_CODES_PER_BUTTON) {
        SDL_strlcpy(name, token, sizeof(name));
        trim(name);
        const SDL_Scancode code = SDL_GetScancodeFromName(name);

        if (code != SDL_SCANCODE_UNKNOWN) {
            keymap[button][code_index] = code;
            code_index += 1;
        }

        token = SDL_strtok_r(NULL, ",", &saveptr);
    }

    if (code_index > 0) {
        initialized_buttons[button] = true;
    }

    return true;
}

static void initialize_empty_buttons() {
    for (int i = 0; i < KEYMAP_BUTTON_COUNT; i++) {
        if (!initialized_buttons[i]) {
            SDL_memcpy(keymap[i], default_keymap[i], sizeof(default_keymap[0]));
            initialized_buttons[i] = true;
        }
    }
}

void Keymap_Init() {
    const char* pref_path = Paths_GetPrefPath();
    char* keymap_path;
    SDL_asprintf(&keymap_path, "%skeymap", pref_path);

    FILE* f = fopen(keymap_path, "r");

    if (f == NULL) {
        // Key map doesn't exist. Write defaults
        write_defaults(keymap_path);
        SDL_free(keymap_path);
        initialize_empty_buttons();
        return;
    }

    SDL_free(keymap_path);
    dict_read(f, dict_iterator);
    fclose(f);
    initialize_empty_buttons();
}

const SDL_Scancode* Keymap_GetScancodes(KeymapButton button) {
    return keymap[button];
}
