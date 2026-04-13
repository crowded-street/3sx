#if ARCADE_ROM

#include "arcade/rom_load.h"
#include "arcade/cps3_decrypt.h"

#include <SDL3/SDL.h>
#include <minizip-ng/mz.h>
#include <minizip-ng/mz_strm.h>
#include <minizip-ng/mz_strm_os.h>
#include <minizip-ng/mz_zip.h>

#include <stdbool.h>

#define READ_CHUNK_SIZE (1024 * 10)

static bool is_prefix(const char* pre, const char* str) {
    return SDL_strncmp(pre, str, SDL_strlen(pre)) == 0;
}

static void read_file(SDL_IOStream* dst, void* zip, void* read_buf) {
    mz_zip_entry_read_open(zip, false, NULL);

    while (mz_zip_entry_read(zip, read_buf, READ_CHUNK_SIZE) > 0) {
        SDL_WriteIO(dst, read_buf, READ_CHUNK_SIZE);
    }

    SDL_SeekIO(dst, 0, SDL_IO_SEEK_SET);
    mz_zip_entry_close(zip);
}

static Uint8 read_byte(SDL_IOStream* src) {
    Uint8 result = 0;
    SDL_ReadIO(src, &result, 1);
    return result;
}

static void* decrypt(SDL_IOStream* simms[4], size_t* size) {
    const Sint64 simm_size = SDL_GetIOSize(simms[0]);
    const size_t buf_size = simm_size * 4;
    void* buf = SDL_malloc(buf_size);
    SDL_IOStream* dst = SDL_IOFromMem(buf, buf_size);

    for (int i = 0; i < simm_size; i++) {
        const Uint8 b0 = read_byte(simms[0]);
        const Uint8 b1 = read_byte(simms[1]);
        const Uint8 b2 = read_byte(simms[2]);
        const Uint8 b3 = read_byte(simms[3]);
        const Uint32 decrypted = cps3_decrypt(b0, b1, b2, b3, i);
        SDL_WriteIO(dst, &decrypted, sizeof(Uint32));
    }

    SDL_CloseIO(dst);

    *size = buf_size;
    return buf;
}

void* Rom_Load(const char* path, size_t* size) {
    void* stream = mz_stream_os_create();
    mz_stream_open(stream, path, MZ_OPEN_MODE_READ);

    void* zip = mz_zip_create();
    int32_t err = mz_zip_open(zip, stream, MZ_OPEN_MODE_READ);

    if (err != MZ_OK) {
        mz_zip_close(zip);
        mz_zip_delete(&zip);
        mz_stream_os_delete(&stream);
        return NULL;
    }

    err = mz_zip_goto_first_entry(zip);

    void* read_buf = SDL_malloc(READ_CHUNK_SIZE);
    SDL_IOStream* simms[4] = { 0 };
    int simm_count = 0;

    while (err == MZ_OK && simm_count < 4) {
        mz_zip_file* info = NULL;
        mz_zip_entry_get_info(zip, &info);

        if (is_prefix("sfiii3-simm1.", info->filename)) {
            SDL_IOStream* io = SDL_IOFromDynamicMem();
            read_file(io, zip, read_buf);
            simms[simm_count] = io;
            simm_count += 1;
        }

        err = mz_zip_goto_next_entry(zip);
    }

    void* result = decrypt(simms, size);

    // Cleanup

    for (int i = 0; i < 4; i++) {
        SDL_CloseIO(simms[i]);
    }

    SDL_free(read_buf);
    mz_zip_close(zip);
    mz_zip_delete(&zip);
    mz_stream_os_delete(&stream);

    return result;
}

#endif
