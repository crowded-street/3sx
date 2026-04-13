#include "port/resources.h"
#include "port/paths.h"

#include <SDL3/SDL.h>

#if CHECKSUM
#include "utils/sha256.h"
#endif

#if CRS_APP_DRIVER_SDL
#include <cdio/iso9660.h>
#endif

#define EXPECTED_AFS_SHA "f9fa50f3a124ec9fa9465aa9c8546c2d867887eb39f711a070762a0324ba5604"

static const char* afs_path = NULL;

static bool file_exists(const char* path) {
    SDL_PathInfo path_info;
    SDL_GetPathInfo(path, &path_info);
    return path_info.type == SDL_PATHTYPE_FILE;
}

char* Resources_GetPath(const char* file_path) {
    const char* base = Paths_GetPrefPath();
    char* full_path = NULL;

    if (file_path == NULL) {
        SDL_asprintf(&full_path, "%sresources/", base);
    } else {
        SDL_asprintf(&full_path, "%sresources/%s", base, file_path);
    }

    return full_path;
}

const char* Resources_GetAFSPath() {
    if (afs_path == NULL) {
        afs_path = Resources_GetPath("SF33RD.AFS");
    }

    return afs_path;
}

bool Resources_Check() {
    const char* afs_path = Resources_GetAFSPath();
    const bool afs_present = file_exists(afs_path);

    if (!afs_present) {
        return false;
    }

#if CHECKSUM
    sha256 sha;
    sha256_init(&sha);

    const size_t chunk_size = 10 * 1024;
    void* buf = SDL_malloc(chunk_size);
    SDL_IOStream* io = SDL_IOFromFile(afs_path, "rb");
    size_t bytes_read = 0;

    while (true) {
        bytes_read = SDL_ReadIO(io, buf, chunk_size);

        if (bytes_read <= 0) {
            break;
        }

        sha256_append(&sha, buf, bytes_read);
    }

    SDL_free(buf);
    SDL_CloseIO(io);

    char hex[SHA256_HEX_SIZE];
    sha256_finalize_hex(&sha, hex);

    if (SDL_strncmp(hex, EXPECTED_AFS_SHA, sizeof(hex)) == 0) {
        return true;
    } else {
        return false;
    }
#else
    return true;
#endif
}

#if CRS_APP_DRIVER_SDL

#define ERROR_LEN_MAX 512

typedef enum FlowState { INIT, DIALOG_OPENED, COPY_ERROR, COPY_SUCCESS } ResourceCopyingFlowState;

static ResourceCopyingFlowState flow_state = INIT;
static SDL_Window* dialog_owner_window = NULL;
static char error[ERROR_LEN_MAX] = { 0 };

static void create_dialog_parent_window() {
    if (dialog_owner_window != NULL) {
        return;
    }

    dialog_owner_window = SDL_CreateWindow("3SX", 1, 1, SDL_WINDOW_HIDDEN);
    SDL_ShowWindow(dialog_owner_window);
    SDL_RaiseWindow(dialog_owner_window);
}

static void destroy_dialog_owner_window() {
    if (dialog_owner_window == NULL) {
        return;
    }

    SDL_DestroyWindow(dialog_owner_window);
    dialog_owner_window = NULL;
}

static void create_resources_directory() {
    char* path = Resources_GetPath(NULL);
    SDL_CreateDirectory(path);
    SDL_free(path);
}

#define CHUNK_SECTORS 16
#define BUFFER_SIZE (ISO_BLOCKSIZE * CHUNK_SECTORS)

static void open_file_dialog_callback(void* userdata, const char* const* filelist, int filter) {
    const char* iso_path = filelist[0];

    iso9660_t* iso = iso9660_open(iso_path);

    if (iso == NULL) {
        SDL_snprintf(error, ERROR_LEN_MAX, "Failed to open iso");
        flow_state = COPY_ERROR;
        return;
    }

    iso9660_stat_t* stat = iso9660_ifs_stat(iso, "/THIRD/SF33RD.AFS;1");

    if (stat == NULL) {
        // Try a different path
        stat = iso9660_ifs_stat(iso, "/SF33RD.AFS;1");

        if (stat == NULL) {
            iso9660_close(iso);
            SDL_snprintf(error, ERROR_LEN_MAX, "AFS archive not found");
            flow_state = COPY_ERROR;
            return;
        }
    }

    create_resources_directory();
    const char* dst_path = Resources_GetAFSPath();
    SDL_IOStream* dst_io = SDL_IOFromFile(dst_path, "wb");

    uint8_t buffer[BUFFER_SIZE];
    uint64_t bytes_remaining = stat->total_size;
    lsn_t current_lsn = stat->lsn;

#if CHECKSUM
    sha256 sha;
    sha256_init(&sha);
#endif

    while (bytes_remaining > 0) {
        const uint64_t bytes_to_read = SDL_min(sizeof(buffer), bytes_remaining);
        const uint64_t sectors_to_read = (bytes_to_read + ISO_BLOCKSIZE - 1) / ISO_BLOCKSIZE;

        const long bytes_read = iso9660_iso_seek_read(iso, buffer, current_lsn, sectors_to_read);
        SDL_WriteIO(dst_io, buffer, bytes_read);

#if CHECKSUM
        sha256_append(&sha, buffer, bytes_read);
#endif

        bytes_remaining -= bytes_read;
        current_lsn += sectors_to_read;
    }

    iso9660_stat_free(stat);
    iso9660_close(iso);
    SDL_CloseIO(dst_io);

#if CHECKSUM
    char hex[SHA256_HEX_SIZE];
    sha256_finalize_hex(&sha, hex);

    if (SDL_strncmp(hex, EXPECTED_AFS_SHA, sizeof(hex)) == 0) {
        flow_state = COPY_SUCCESS;
    } else {
        SDL_snprintf(error, ERROR_LEN_MAX, "Incorrect AFS checksum – expected %s, got %s", EXPECTED_AFS_SHA, hex);
        flow_state = COPY_ERROR;
        SDL_RemovePath(dst_path);
    }
#else
    flow_state = COPY_SUCCESS;
#endif
}

static void open_dialog() {
    flow_state = DIALOG_OPENED;
    const SDL_DialogFileFilter filter = { .name = "Game iso", .pattern = "iso" };
    SDL_ShowOpenFileDialog(open_file_dialog_callback, NULL, dialog_owner_window, &filter, 1, NULL, false);
}

bool Resources_RunResourceCopyingFlow() {
    switch (flow_state) {
    case INIT:
        create_dialog_parent_window();
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "Valid resources are missing",
                                 "3SX needs resources from a copy of \"Street Fighter III: 3rd Strike\" to run. Choose "
                                 "the iso in the next dialog",
                                 dialog_owner_window);
        open_dialog();
        break;

    case DIALOG_OPENED:
        // Wait for the callback to be called
        break;

    case COPY_ERROR:
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", error, dialog_owner_window);
        open_dialog();
        break;

    case COPY_SUCCESS:
        char* resources_path = Resources_GetPath(NULL);
        char* message = NULL;
        SDL_asprintf(&message, "You can find them at:\n%s", resources_path);
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_INFORMATION, "Resources copied successfully", message, dialog_owner_window);
        SDL_free(resources_path);
        SDL_free(message);
        destroy_dialog_owner_window();
        flow_state = INIT;
        return true;
    }

    return false;
}

#endif // CRS_APP_DRIVER_SDL
