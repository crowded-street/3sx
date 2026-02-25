#include "port/button_config.h"
#include "port/paths.h"
#include "structs.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

extern struct _SAVE_W save_w[6];

static const char* BUTTON_NAMES[8] = {
    "Square", "Triangle", "R1", "L1", "Cross", "Circle", "R2", "L2"
};

static const char* ACTION_NAMES[12] = {
    "LP", "MP", "HP", "LK", "MK", "HK",
    "6", "7", "8", "9", "10", "None"
};

static const char* INI_FILENAME = "buttons.ini";

static char* get_ini_path() {
    const char* base = Paths_GetBasePath();
    char* path = NULL;
    SDL_asprintf(&path, "%s%s", base, INI_FILENAME);
    return path;
}

static const char* action_name(u8 value) {
    if (value < 12) {
        return ACTION_NAMES[value];
    }
    return "None";
}

static u8 action_value(const char* name) {
    for (int i = 0; i < 12; i++) {
        if (SDL_strcasecmp(name, ACTION_NAMES[i]) == 0) {
            return (u8)i;
        }
    }
    return 11;
}

void ButtonConfig_Save() {
    char* path = get_ini_path();
    FILE* f = fopen(path, "w");
    SDL_free(path);

    if (f == NULL) {
        printf("ButtonConfig: Failed to save buttons.ini\n");
        return;
    }

    fprintf(f, "# 3SX Button Mappings\n");
    fprintf(f, "# Actions: LP, MP, HP, LK, MK, HK, None\n");
    fprintf(f, "# Buttons: Square, Triangle, R1, L1, Cross, Circle, R2, L2\n\n");

    for (int player = 0; player < 2; player++) {
        fprintf(f, "[Player%d]\n", player + 1);

        for (int btn = 0; btn < 8; btn++) {
            fprintf(f, "%s = %s\n", BUTTON_NAMES[btn],
                    action_name(save_w[1].Pad_Infor[player].Shot[btn]));
        }

        fprintf(f, "Vibration = %d\n\n", save_w[1].Pad_Infor[player].Vibration);
    }

    fclose(f);
    printf("ButtonConfig: Saved buttons.ini\n");
}

void ButtonConfig_Load() {
    char* path = get_ini_path();
    FILE* f = fopen(path, "r");
    SDL_free(path);

    if (f == NULL) {
        printf("ButtonConfig: No buttons.ini found, using defaults\n");
        return;
    }

    int current_player = -1;
    char line[256];

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = '\0';

        char* p = line;
        while (*p == ' ' || *p == '\t') p++;

        if (*p == '\0' || *p == '#') continue;

        if (*p == '[') {
            if (SDL_strncasecmp(p, "[Player1]", 9) == 0) {
                current_player = 0;
            } else if (SDL_strncasecmp(p, "[Player2]", 9) == 0) {
                current_player = 1;
            }
            continue;
        }

        if (current_player < 0 || current_player > 1) continue;

        char key[128];
        char value[128];

        if (sscanf(p, "%127[^=]=%127s", key, value) != 2) continue;

        // Trim trailing spaces from key
        char* end = key + strlen(key) - 1;
        while (end > key && (*end == ' ' || *end == '\t')) { *end = '\0'; end--; }

        // Trim leading spaces from value
        char* vp = value;
        while (*vp == ' ' || *vp == '\t') vp++;

        if (SDL_strcasecmp(key, "Vibration") == 0) {
            save_w[1].Pad_Infor[current_player].Vibration = (u8)SDL_atoi(vp);
            continue;
        }

        for (int btn = 0; btn < 8; btn++) {
            if (SDL_strcasecmp(key, BUTTON_NAMES[btn]) == 0) {
                save_w[1].Pad_Infor[current_player].Shot[btn] = action_value(vp);
                break;
            }
        }
    }

    fclose(f);

    // Propagate to other save_w slots (same as Save_Game_Data does)
    for (int slot = 4; slot <= 5; slot++) {
        save_w[slot].Pad_Infor[0] = save_w[1].Pad_Infor[0];
        save_w[slot].Pad_Infor[1] = save_w[1].Pad_Infor[1];
    }

    printf("ButtonConfig: Loaded buttons.ini\n");
}
