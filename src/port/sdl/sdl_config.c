#include "port/sdl/sdl_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Key-value pair
typedef struct SDLConfig_Entry {
    char* key;
    char* value;
    struct SDLConfig_Entry* next;
} SDLConfig_Entry;

// Section containing key-value pairs
typedef struct SDLConfig_Section {
    char* name;
    SDLConfig_Entry* entries;
    struct SDLConfig_Section* next;
} SDLConfig_Section;

// Config structure
struct SDLConfig {
    SDLConfig_Section* sections;
};

// Helper function to trim whitespace
static char* trim_whitespace(char* str) {
    char* end;

    // Trim leading space
    while (*str == ' ' || *str == '\t') {
        str++;
    }

    if (*str == 0) {
        return str;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) {
        end--;
    }

    end[1] = '\0';

    return str;
}

// Helper function to duplicate a string
static char* strdup_safe(const char* str) {
    size_t len = strlen(str) + 1;
    char* dup = malloc(len);
    if (dup) {
        memcpy(dup, str, len);
    }
    return dup;
}

// Find or create section
static SDLConfig_Section* find_or_create_section(SDLConfig* config, const char* name) {
    // Find existing section
    SDLConfig_Section* section = config->sections;
    SDLConfig_Section* last = NULL;
    while (section) {
        if (strcmp(section->name, name) == 0) {
            return section;
        }
        last = section;
        section = section->next;
    }

    // Create new section and append to end
    section = calloc(1, sizeof(SDLConfig_Section));
    section->name = strdup_safe(name);
    section->next = NULL;

    if (last) {
        last->next = section;
    } else {
        config->sections = section;
    }

    return section;
}

// Find section
static SDLConfig_Section* find_section(const SDLConfig* config, const char* name) {
    SDLConfig_Section* section = config->sections;
    while (section) {
        if (strcmp(section->name, name) == 0) {
            return section;
        }
        section = section->next;
    }
    return NULL;
}

// Find entry in section
static SDLConfig_Entry* find_entry(const SDLConfig_Section* section, const char* key) {
    SDLConfig_Entry* entry = section->entries;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

// Set entry in section (creates or updates)
static void set_entry(SDLConfig_Section* section, const char* key, const char* value) {
    // Find existing entry
    SDLConfig_Entry* entry = find_entry(section, key);
    if (entry) {
        // Update existing
        free(entry->value);
        entry->value = strdup_safe(value);
        return;
    }

    // Create new entry
    entry = calloc(1, sizeof(SDLConfig_Entry));
    entry->key = strdup_safe(key);
    entry->value = strdup_safe(value);
    entry->next = NULL;

    // Append to end of list to preserve insertion order
    if (!section->entries) {
        section->entries = entry;
    } else {
        SDLConfig_Entry* last = section->entries;
        while (last->next) {
            last = last->next;
        }
        last->next = entry;
    }
}

SDLConfig* SDLConfig_Create(void) {
    return calloc(1, sizeof(SDLConfig));
}

void SDLConfig_Destroy(SDLConfig* config) {
    if (!config) {
        return;
    }

    SDLConfig_Section* section = config->sections;
    while (section) {
        SDLConfig_Section* next_section = section->next;

        // Free entries
        SDLConfig_Entry* entry = section->entries;
        while (entry) {
            SDLConfig_Entry* next_entry = entry->next;
            free(entry->key);
            free(entry->value);
            free(entry);
            entry = next_entry;
        }

        free(section->name);
        free(section);
        section = next_section;
    }

    free(config);
}

bool SDLConfig_LoadFile(SDLConfig* config, const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        return false;
    }

    char line[512];
    SDLConfig_Section* current_section = NULL;

    while (fgets(line, sizeof(line), file)) {
        char* trimmed = trim_whitespace(line);

        // Skip empty lines and comments
        if (trimmed[0] == '\0' || trimmed[0] == '#' || trimmed[0] == ';') {
            continue;
        }

        // Check for section headers
        if (trimmed[0] == '[') {
            char* end = strchr(trimmed, ']');
            if (end) {
                *end = '\0';
                char* section_name = trim_whitespace(trimmed + 1);
                current_section = find_or_create_section(config, section_name);
            }
            continue;
        }

        // Parse key=value pairs
        char* equals = strchr(trimmed, '=');
        if (!equals || !current_section) {
            continue;
        }

        *equals = '\0';
        char* key = trim_whitespace(trimmed);
        char* value = trim_whitespace(equals + 1);

        set_entry(current_section, key, value);
    }

    fclose(file);
    return true;
}

bool SDLConfig_SaveFile(const SDLConfig* config, const char* filepath) {
    FILE* file = fopen(filepath, "w");
    if (!file) {
        return false;
    }

    SDLConfig_Section* section = config->sections;
    while (section) {
        fprintf(file, "[%s]\n", section->name);

        SDLConfig_Entry* entry = section->entries;
        while (entry) {
            fprintf(file, "%s=%s\n", entry->key, entry->value);
            entry = entry->next;
        }

        fprintf(file, "\n");
        section = section->next;
    }

    fclose(file);
    return true;
}

const char* SDLConfig_GetString(const SDLConfig* config, const char* section_name, const char* key) {
    SDLConfig_Section* section = find_section(config, section_name);
    if (!section) {
        return NULL;
    }

    SDLConfig_Entry* entry = find_entry(section, key);
    if (!entry) {
        return NULL;
    }

    return entry->value;
}

int SDLConfig_GetInt(const SDLConfig* config, const char* section_name, const char* key, int default_value) {
    const char* value = SDLConfig_GetString(config, section_name, key);
    if (!value) {
        return default_value;
    }

    char* endptr;
    long result = strtol(value, &endptr, 10);
    if (*endptr != '\0') {
        return default_value; // Invalid integer
    }

    return (int)result;
}

bool SDLConfig_GetBool(const SDLConfig* config, const char* section_name, const char* key, bool default_value) {
    const char* value = SDLConfig_GetString(config, section_name, key);
    if (!value) {
        return default_value;
    }

    // Convert to lowercase for comparison
    char lower[16];
    size_t i;
    for (i = 0; i < sizeof(lower) - 1 && value[i]; i++) {
        lower[i] = tolower((unsigned char)value[i]);
    }
    lower[i] = '\0';

    if (strcmp(lower, "true") == 0 || strcmp(lower, "yes") == 0 || strcmp(lower, "1") == 0) {
        return true;
    }
    if (strcmp(lower, "false") == 0 || strcmp(lower, "no") == 0 || strcmp(lower, "0") == 0) {
        return false;
    }

    return default_value;
}

void SDLConfig_SetString(SDLConfig* config, const char* section_name, const char* key, const char* value) {
    SDLConfig_Section* section = find_or_create_section(config, section_name);
    set_entry(section, key, value);
}

void SDLConfig_SetInt(SDLConfig* config, const char* section_name, const char* key, int value) {
    char buffer[32];
    sprintf(buffer, "%d", value);
    SDLConfig_SetString(config, section_name, key, buffer);
}

void SDLConfig_SetBool(SDLConfig* config, const char* section_name, const char* key, bool value) {
    SDLConfig_SetString(config, section_name, key, value ? "true" : "false");
}
