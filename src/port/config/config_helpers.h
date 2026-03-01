#ifndef CONFIG_HELPERS_H
#define CONFIG_HELPERS_H

#include <stdbool.h>
#include <stdio.h>

#include <SDL3/SDL.h>

typedef bool (*DictIterator)(const char* key, const char* value);

void dict_read(FILE* file, DictIterator iterator);
void trim(char* string);
void io_printf(SDL_IOStream* io, const char* format, ...);

#endif
