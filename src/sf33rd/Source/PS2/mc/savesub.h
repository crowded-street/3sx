#ifndef SAVESUB_H
#define SAVESUB_H

#include "types.h"

typedef enum SaveFileType {
    SAVE_FILE_SETTINGS,
    SAVE_FILE_SYSTEM_DIRECTION,
    SAVE_FILE_REPLAY,
} SaveFileType;

typedef enum SaveMode {
    SAVE_MODE_LOAD,
    SAVE_MODE_SAVE,
} SaveMode;

void SaveInit(SaveFileType file_type, SaveMode save_mode);
s32 SaveMove();

#endif
