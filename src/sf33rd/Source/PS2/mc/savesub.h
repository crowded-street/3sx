#ifndef SAVESUB_H
#define SAVESUB_H

#include "types.h"

typedef enum SaveFileType {
    SAVE_FILE_SETTINGS,
    SAVE_FILE_SYSTEM_DIRECTION,
    SAVE_FILE_REPLAY,
} SaveFileType;

void SaveInit(SaveFileType file_type, s32 save_mode);
s32 SaveMove();

#endif
