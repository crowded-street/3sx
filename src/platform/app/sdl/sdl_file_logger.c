#if CRS_APP_DRIVER_SDL

#include "platform/app/sdl/sdl_file_logger.h"
#include "port/paths.h"

#include <SDL3/SDL.h>

#include <time.h>

#define LOG_FILENAME_MAX 128
#define LOGS_MAX 10

typedef struct SDLFileLoggerContext {
    SDL_IOStream* io;
} SDLFileLoggerContext;

static SDLFileLoggerContext context = { 0 };

static void make_log_filename(char* buffer, size_t max_size) {
    const time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, max_size, "%Y-%m-%d-%H-%M-%S.log", t);
}

static char* make_logs_path() {
    char* path = NULL;
    SDL_asprintf(&path, "%slogs/", Paths_GetPrefPath());
    return path;
}

static void log_to_file(void* userdata, int category, SDL_LogPriority priority, const char* message) {
    SDLFileLoggerContext* context = (SDLFileLoggerContext*)userdata;
    SDL_IOprintf(context->io, "%s\n", message);
    SDL_FlushIO(context->io);

    SDL_LogOutputFunction default_log_func = SDL_GetDefaultLogOutputFunction();
    default_log_func(NULL, category, priority, message);
}

typedef struct DirectoryEnumerationState {
    const char* base_path;
    const char* oldest_fname;
    SDL_Time oldest_create_time;
    int total_files;
} DirectoryEnumerationState;

static SDL_EnumerationResult enumerate_callback(void* userdata, const char* dirname, const char* fname) {
    DirectoryEnumerationState* state = (DirectoryEnumerationState*)userdata;

    if (SDL_strcmp(dirname, state->base_path) != 0) {
        return SDL_ENUM_CONTINUE;
    }

    char* path = NULL;
    SDL_asprintf(&path, "%s%s", dirname, fname);

    SDL_PathInfo path_info;
    const bool get_info_success = SDL_GetPathInfo(path, &path_info);
    SDL_free(path);

    if (!get_info_success) {
        return SDL_ENUM_CONTINUE;
    }

    if ((state->total_files == 0) || (path_info.create_time < state->oldest_create_time)) {
        state->oldest_create_time = path_info.create_time;

        if (state->oldest_fname != NULL) {
            SDL_free(state->oldest_fname);
        }

        state->oldest_fname = SDL_strdup(fname);
    }

    state->total_files += 1;
    return SDL_ENUM_CONTINUE;
}

static void cleanup_stale_logs(const char* logs_path) {
    DirectoryEnumerationState state = { 0 };
    state.base_path = logs_path;
    SDL_EnumerateDirectory(logs_path, enumerate_callback, &state);

    if (state.total_files >= LOGS_MAX) {
        char* path = NULL;
        SDL_asprintf(&path, "%s%s", logs_path, state.oldest_fname);
        SDL_RemovePath(path);
        SDL_free(path);
    }

    if (state.oldest_fname != NULL) {
        SDL_free(state.oldest_fname);
    }
}

void SDLFileLogger_Configure() {
    const char* logs_path = make_logs_path();

    if (!SDL_CreateDirectory(logs_path)) {
        return;
    }

    cleanup_stale_logs(logs_path);

    const size_t buffer_size = SDL_strlen(logs_path) + LOG_FILENAME_MAX + 1;
    char* path = SDL_malloc(buffer_size);
    SDL_strlcpy(path, logs_path, buffer_size);
    SDL_free(logs_path);

    const size_t current_length = SDL_strlen(path);
    make_log_filename(path + current_length, buffer_size - current_length);

    context.io = SDL_IOFromFile(path, "wx");
    SDL_free(path);

    if (context.io == NULL) {
        SDL_Log("Failed to create log file: %s", SDL_GetError());
        return;
    }

    SDL_SetLogOutputFunction(log_to_file, &context);
}

#endif
