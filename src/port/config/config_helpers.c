#include "port/config/config_helpers.h"

#include <SDL3/SDL.h>

void trim(char* string) {
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

void io_printf(SDL_IOStream* io, const char* format, ...) {
    va_list args;
    va_start(args, format);

    char* rendered = NULL;
    SDL_vasprintf(&rendered, format, args);
    va_end(args);

    if (rendered == NULL) {
        return;
    }

    SDL_WriteIO(io, rendered, SDL_strlen(rendered));
    SDL_free(rendered);
}

/* One configuration line: the newline removed, leading whitespace skipped, and
 * the text split at the first '='. Returns false wherever the loop body used
 * to `continue` and true where a key and a value are ready. */
static bool dict_parse_line(char* line, char* key, char* value) {
    // Remove newline
    line[strcspn(line, "\r\n")] = '\0';

    char* p = line;

    // Skip leading whitespace
    while (SDL_isspace((unsigned char)*p)) {
        p++;
    }

    // Skip empty/comment lines
    if (*p == '\0' || *p == '#') {
        return false;
    }

    if (sscanf(p, "%127[^=]=%127[^\n]", key, value) != 2) {
        return false;
    }

    trim(key);
    trim(value);

    if (SDL_strlen(key) == 0 || SDL_strlen(value) == 0) {
        return false;
    }

    return true;
}

void dict_read(FILE* file, DictIterator iterator) {
    if (file == NULL) {
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), file)) {
        char key[128];
        char value[128];

        if (!dict_parse_line(line, key, value)) {
            continue;
        }

        const bool should_continue = iterator(key, value);

        if (!should_continue) {
            break;
        }
    }
}
