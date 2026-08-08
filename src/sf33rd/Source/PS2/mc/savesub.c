#include "sf33rd/Source/PS2/mc/savesub.h"
#include "common.h"
#include "port/paths.h"
#include "port/utils.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

#include <SDL3/SDL.h>

#define SETTINGS_VERSION 1
#define SETTINGS_SIZE_V1 367
#define SETTINGS_SIZE SETTINGS_SIZE_V1

#define SYSDIR_VERSION 1
#define SYSDIR_SIZE_V1 0
#define SYSDIR_SIZE SYSDIR_SIZE_V1

#define REPLAY_VERSION 1
#define REPLAY_SIZE_V1 0
#define REPLAY_SIZE REPLAY_SIZE_V1

#define ROOT_DIR "saves"
#define PATH_LEN_MAX 128

typedef enum SaveState {
    SAVE_STATE_IDLE,
    SAVE_STATE_INIT,
    SAVE_STATE_OPENING,
    SAVE_STATE_ERROR,
} SaveState;

typedef struct SaveOperation {
    SaveState state;
    SaveFileType file_type;
    SaveMode mode;
    SDL_Storage* storage;
    const char* error;
} SaveOperation;

typedef enum ReadResult {
    READ_SUCCESS,
    READ_NO_FILE,
    READ_SIZE_MISMATCH,
    READ_ERROR,
} ReadResult;

static const Uint64 file_type_to_expected_size[] = {
    [SAVE_FILE_SETTINGS] = SETTINGS_SIZE,
    [SAVE_FILE_SYSTEM_DIRECTION] = SYSDIR_SIZE,
    [SAVE_FILE_REPLAY] = REPLAY_SIZE,
};

static SaveOperation operation = { 0 };

static void* serialize_settings(Uint64* length) {
    Save_Game_Data();

    const struct _SAVE_W* src = &save_w[1];
    *length = SETTINGS_SIZE;
    void* buf = SDL_malloc(SETTINGS_SIZE);
    SDL_IOStream* io = SDL_IOFromMem(buf, SETTINGS_SIZE);

    SDL_WriteU8(io, SETTINGS_VERSION);

    for (int i = 0; i < SDL_arraysize(src->Pad_Infor); i++) {
        const _PAD_INFOR* pad_info = &src->Pad_Infor[i];

        for (int j = 0; j < SDL_arraysize(pad_info->Shot); j++) {
            SDL_WriteU8(io, pad_info->Shot[j]);
        }

        SDL_WriteU8(io, pad_info->Vibration);
    }

    SDL_WriteU8(io, src->Difficulty);
    SDL_WriteS8(io, src->Time_Limit);
    SDL_WriteU8(io, src->Battle_Number[0]);
    SDL_WriteU8(io, src->Battle_Number[1]);
    SDL_WriteU8(io, src->Damage_Level);
    SDL_WriteU8(io, src->Handicap);
    SDL_WriteU8(io, src->Partner_Type[0]);
    SDL_WriteU8(io, src->Partner_Type[1]);
    SDL_WriteS8(io, src->Adjust_X);
    SDL_WriteS8(io, src->Adjust_Y);
    SDL_WriteU8(io, src->Screen_Mode);
    SDL_WriteU8(io, src->GuardCheck);
    SDL_WriteU8(io, src->AnalogStick);
    SDL_WriteU8(io, src->BgmType);
    SDL_WriteU8(io, src->BGM_Level);
    SDL_WriteU8(io, src->SE_Level);
    SDL_WriteIO(io, &src->extra_option, sizeof(src->extra_option));

    for (int i = 0; i < SDL_arraysize(src->Ranking); i++) {
        const RANK_DATA* rank_data = &src->Ranking[i];

        SDL_WriteIO(io, rank_data->name, sizeof(rank_data->name));
        SDL_WriteU16BE(io, rank_data->player);
        SDL_WriteU32BE(io, rank_data->score);
        SDL_WriteS8(io, rank_data->cpu_grade);
        SDL_WriteS8(io, rank_data->grade);
        SDL_WriteU16BE(io, rank_data->wins);
        SDL_WriteU8(io, rank_data->player_color);
        SDL_WriteU8(io, rank_data->all_clear);
    }

    SDL_CloseIO(io);
    return buf;
}

static void* serialize(SaveFileType file_type, Uint64* length) {
    switch (file_type) {
    case SAVE_FILE_SETTINGS:
        return serialize_settings(length);

    case SAVE_FILE_SYSTEM_DIRECTION:
    case SAVE_FILE_REPLAY:
        fatal_error("Not implemented");
    }
}

static void deserialize_settings(const void* buffer) {
    struct _SAVE_W* dst = &save_w[1];
    SDL_IOStream* io = SDL_IOFromConstMem(buffer, SETTINGS_SIZE);
    Uint8 version;

    SDL_ReadU8(io, &version);

    if (version != SETTINGS_VERSION) {
        SDL_CloseIO(io);
        return;
    }

    for (int i = 0; i < SDL_arraysize(dst->Pad_Infor); i++) {
        _PAD_INFOR* pad_info = &dst->Pad_Infor[i];

        for (int j = 0; j < SDL_arraysize(pad_info->Shot); j++) {
            SDL_ReadU8(io, &pad_info->Shot[j]);
        }

        SDL_ReadU8(io, &pad_info->Vibration);
    }

    SDL_ReadU8(io, &dst->Difficulty);
    SDL_ReadS8(io, &dst->Time_Limit);
    SDL_ReadU8(io, &dst->Battle_Number[0]);
    SDL_ReadU8(io, &dst->Battle_Number[1]);
    SDL_ReadU8(io, &dst->Damage_Level);
    SDL_ReadU8(io, &dst->Handicap);
    SDL_ReadU8(io, &dst->Partner_Type[0]);
    SDL_ReadU8(io, &dst->Partner_Type[1]);
    SDL_ReadS8(io, &dst->Adjust_X);
    SDL_ReadS8(io, &dst->Adjust_Y);
    SDL_ReadU8(io, &dst->Screen_Mode);
    SDL_ReadU8(io, &dst->GuardCheck);
    SDL_ReadU8(io, &dst->AnalogStick);
    SDL_ReadU8(io, &dst->BgmType);
    SDL_ReadU8(io, &dst->BGM_Level);
    SDL_ReadU8(io, &dst->SE_Level);
    SDL_ReadIO(io, &dst->extra_option, sizeof(dst->extra_option));

    for (int i = 0; i < SDL_arraysize(dst->Ranking); i++) {
        RANK_DATA* rank_data = &dst->Ranking[i];

        SDL_ReadIO(io, rank_data->name, sizeof(rank_data->name));
        SDL_ReadU16BE(io, &rank_data->player);
        SDL_ReadU32BE(io, &rank_data->score);
        SDL_ReadS8(io, &rank_data->cpu_grade);
        SDL_ReadS8(io, &rank_data->grade);
        SDL_ReadU16BE(io, &rank_data->wins);
        SDL_ReadU8(io, &rank_data->player_color);
        SDL_ReadU8(io, &rank_data->all_clear);
    }

    SDL_CloseIO(io);

    Copy_Save_w();
    Copy_Check_w();
    sys_w.bgm_type = save_w[1].BgmType;
}

static void deserialize(SaveFileType file_type, const void* buffer) {
    switch (file_type) {
    case SAVE_FILE_SETTINGS:
        return deserialize_settings(buffer);

    case SAVE_FILE_SYSTEM_DIRECTION:
    case SAVE_FILE_REPLAY:
        fatal_error("Not implemented");
    }
}

static const char* file_type_to_name(SaveFileType file_type) {
    switch (file_type) {
    case SAVE_FILE_SETTINGS:
        return "settings";

    case SAVE_FILE_SYSTEM_DIRECTION:
        return "sysdir";

    case SAVE_FILE_REPLAY:
        return "replay";
    }
}

static void make_path(char* text, size_t maxlen, const char* name, bool backup) {
    SDL_snprintf(text, maxlen, backup ? "%s/%s.bak" : "%s/%s", ROOT_DIR, name);
}

static ReadResult read_file_if_exists(SDL_Storage* storage, const char* path, void* buffer, Uint64 size) {
    SDL_PathInfo info;

    if (!SDL_GetStoragePathInfo(storage, path, &info)) {
        return READ_ERROR;
    }

    if (info.type != SDL_PATHTYPE_FILE) {
        return READ_NO_FILE;
    }

    if (info.size != size) {
        return READ_SIZE_MISMATCH;
    }

    if (!SDL_ReadStorageFile(storage, path, buffer, size)) {
        return READ_ERROR;
    }

    return READ_SUCCESS;
}

static bool write_file(SDL_Storage* storage, const char* name, const void* file, Uint64 length) {
    char path[PATH_LEN_MAX];
    char backup_path[PATH_LEN_MAX];
    make_path(path, sizeof(path), name, false);
    make_path(backup_path, sizeof(backup_path), name, true);

    if (!SDL_GetStoragePathInfo(storage, ROOT_DIR, NULL)) {
        // saves directory doesn't exist, let's create it
        if (!SDL_CreateStorageDirectory(storage, ROOT_DIR)) {
            return false;
        }
    }

    if (SDL_GetStoragePathInfo(storage, path, NULL)) {
        // Save file exists, let's make a backup
        if (!SDL_CopyStorageFile(storage, path, backup_path)) {
            return false;
        }
    }

    return SDL_WriteStorageFile(storage, path, file, length);
}

void SaveInit(SaveFileType file_type, SaveMode save_mode) {
    operation.state = SAVE_STATE_INIT;
    operation.file_type = file_type;
    operation.mode = save_mode;
}

s32 SaveMove() {
    switch (operation.state) {
    case SAVE_STATE_IDLE:
        return 0;

    case SAVE_STATE_INIT:
        operation.storage = Paths_OpenUserStorage(0);

        if (operation.storage == NULL) {
            operation.state = SAVE_STATE_ERROR;
            operation.error = SDL_GetError();
            return -1;
        }

        operation.state = SAVE_STATE_OPENING;
        /* fallthrough */

    case SAVE_STATE_OPENING:
        if (!SDL_StorageReady(operation.storage)) {
            return 1;
        }

        const char* name = file_type_to_name(operation.file_type);
        bool success = false;

        switch (operation.mode) {
        case SAVE_MODE_LOAD:
        case SAVE_MODE_AUTO_LOAD:
            const Uint64 expected_size = file_type_to_expected_size[operation.file_type];
            void* buffer = SDL_malloc(expected_size);
            bool buffer_filled = false;
            char path[PATH_LEN_MAX];
            char backup_path[PATH_LEN_MAX];
            make_path(path, sizeof(path), name, false);
            make_path(backup_path, sizeof(backup_path), name, true);

            success = true; // Always return success for now

            if (read_file_if_exists(operation.storage, path, buffer, expected_size) == READ_SUCCESS) {
                buffer_filled = true;
            } else if (read_file_if_exists(operation.storage, backup_path, buffer, expected_size) == READ_SUCCESS) {
                buffer_filled = true;
            }

            if (buffer_filled) {
                deserialize(operation.file_type, buffer);
            }

            SDL_free(buffer);
            break;

        case SAVE_MODE_SAVE:
        case SAVE_MODE_AUTO_SAVE:
            Uint64 length = 0;
            const void* buf = serialize(operation.file_type, &length);
            success = write_file(operation.storage, name, buf, length);
            break;
        }

        SDL_CloseStorage(operation.storage);
        SDL_zero(operation);
        return success ? 0 : -1;

    case SAVE_STATE_ERROR:
        return -1;
    }

    return -1;
}
