#include "port/config/dict.h"

#include <SDL3/SDL.h>

static void trim(char* string) {
    char* p = string;

    // Trim leading
    while (SDL_isspace((unsigned char)*p)) {
        p++;
    }

    if (p != string) {
        SDL_memmove(string, p, SDL_strlen(p) + 1);
    }

    // Trim trailing
    char* end = string + SDL_strlen(string);

    while ((end > string) && SDL_isspace((unsigned char)end[-1])) {
        end--;
        *end = '\0';
    }
}

void Dict_Read(FILE* file, DictIterator iterator) {
    if (file == NULL) {
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        line[strcspn(line, "\r\n")] = '\0';

        char* p = line;

        // Skip leading whitespace
        while (SDL_isspace((unsigned char)*p)) {
            p++;
        }

        // Skip empty/comment lines
        if (*p == '\0' || *p == '#') {
            continue;
        }

        char key[128];
        char value[128];

        if (sscanf(p, "%127[^=]=%127s", key, value) != 2) {
            continue;
        }

        trim(key);
        trim(value);

        if (SDL_strlen(key) == 0 || SDL_strlen(value) == 0) {
            continue;
        }

        const bool should_continue = iterator(key, value);

        if (!should_continue) {
            break;
        }
    }
}
