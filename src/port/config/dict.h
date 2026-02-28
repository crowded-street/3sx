#ifndef CONFIG_DICT_H
#define CONFIG_DICT_H

#include <stdbool.h>
#include <stdio.h>

typedef bool (*DictIterator)(const char* key, const char* value);

void Dict_Read(FILE* file, DictIterator iterator);

#endif
