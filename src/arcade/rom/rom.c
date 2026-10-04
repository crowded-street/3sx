#include "arcade/rom/rom.h"
#include "arcade/rom/cps3_crypt.h"

#include <SDL3/SDL.h>
#include <minizip-ng/mz.h>
#include <minizip-ng/mz_strm.h>
#include <minizip-ng/mz_strm_os.h>
#include <minizip-ng/mz_zip.h>

#include <stdbool.h>

#define SIMM_COUNT 6
#define FIRST_GRAPHICS_SIMM 3
#define PROGRAM_SIMM_CHIPS 4
#define GRAPHICS_SIMM_CHIPS 8
#define MAX_CHIP_NAME 64

typedef struct FlashFile {
    const char* name;
    int simm;
    size_t offset; // Within the SIMM
    size_t size;
} FlashFile;

/// A ROM set comes as one file per chip (MAME) or, if `flash_files` is set, optionally as one file per flash bank
/// with the bank's chips already merged into big-endian words (FBNeo). The chip layout is used when the zip has the
/// first chip.
typedef struct RomSpec {
    const char* zip_name;
    const char* chip_name_format; // Takes SIMM number and chip number
    const FlashFile* flash_files;
    int flash_file_count;
    Uint32 key1;
    Uint32 key2;
    size_t chip_size;
    int chips[SIMM_COUNT]; // Number of chips on each SIMM, indexed by SIMM number minus one
} RomSpec;

static const FlashFile sfiii_files[] = {
    { .name = "10", .simm = 1, .offset = 0, .size = 0x800000 },
    { .name = "30", .simm = 3, .offset = 0, .size = 0x800000 },
    { .name = "31", .simm = 3, .offset = 0x800000, .size = 0x800000 },
    { .name = "40", .simm = 4, .offset = 0, .size = 0x800000 },
    { .name = "41", .simm = 4, .offset = 0x800000, .size = 0x800000 },
    { .name = "50", .simm = 5, .offset = 0, .size = 0x400000 },
};

static const RomSpec specs[ROM_GAME_COUNT] = {
    [ROM_GAME_SFIII] = {
        .zip_name = "sfiiin.zip",
        .chip_name_format = "sfiii-simm%d.%d",
        .flash_files = sfiii_files,
        .flash_file_count = SDL_arraysize(sfiii_files),
        .key1 = 0xB5FE053E,
        .key2 = 0xFC03925A,
        .chip_size = 0x200000,
        .chips = { 4, 0, 8, 8, 2, 0 },
    },
    [ROM_GAME_SFIII3] = {
        .zip_name = "sfiii3nr1.zip",
        .chip_name_format = "sfiii3-simm%d.%d",
        .key1 = 0xA55432B4,
        .key2 = 0x0C129981,
        .chip_size = 0x200000,
        .chips = { 4, 4, 8, 8, 8, 8 },
    },
};

struct Rom {
    RomGame game;
    Uint8* program;
    size_t program_size;
    Uint8* graphics;
    size_t graphics_size;
};

static bool is_graphics_simm(int simm) {
    return simm >= FIRST_GRAPHICS_SIMM;
}

static int slot_chips(int simm) {
    return is_graphics_simm(simm) ? GRAPHICS_SIMM_CHIPS : PROGRAM_SIMM_CHIPS;
}

static size_t simm_size(const RomSpec* spec, int simm) {
    return spec->chip_size * slot_chips(simm);
}

/// Offset of the SIMM within its region. Absent SIMMs still occupy their slot.
static size_t simm_offset(const RomSpec* spec, int simm) {
    return is_graphics_simm(simm) ? (simm - FIRST_GRAPHICS_SIMM) * simm_size(spec, simm)
                                  : (simm - 1) * simm_size(spec, simm);
}

static int32_t match_base_name(void* handle, void* userdata, mz_zip_file* info) {
    (void)handle;
    const char* name = SDL_strrchr(info->filename, '/');
    name = name != NULL ? name + 1 : info->filename;
    return SDL_strcmp(name, userdata);
}

static bool read_chip(void* zip, const char* name, Uint8* dst, size_t size) {
    mz_zip_file* info = NULL;

    if (mz_zip_locate_first_entry(zip, (void*)name, match_base_name) != MZ_OK) {
        return SDL_SetError("Missing ROM chip %s", name);
    }

    if (mz_zip_entry_get_info(zip, &info) != MZ_OK || info == NULL || info->uncompressed_size != (int64_t)size) {
        return SDL_SetError("Unexpected size of ROM chip %s", name);
    }

    if (mz_zip_entry_read_open(zip, false, NULL) != MZ_OK) {
        return SDL_SetError("Couldn't open ROM chip %s", name);
    }

    size_t read = 0;

    while (read < size) {
        const int32_t count = mz_zip_entry_read(zip, dst + read, (int32_t)(size - read));

        if (count <= 0) {
            break;
        }

        read += (size_t)count;
    }

    mz_zip_entry_close(zip);

    if (read != size) {
        return SDL_SetError("Couldn't read ROM chip %s", name);
    }

    return true;
}

static bool load_simm(Rom* rom, const RomSpec* spec, void* zip, int simm, Uint8* staging) {
    const Uint8* chips[GRAPHICS_SIMM_CHIPS] = { 0 };

    for (int chip = 0; chip < spec->chips[simm - 1]; chip++) {
        char name[MAX_CHIP_NAME];
        SDL_snprintf(name, sizeof(name), spec->chip_name_format, simm, chip);
        Uint8* dst = staging + chip * spec->chip_size;

        if (!read_chip(zip, name, dst, spec->chip_size)) {
            return false;
        }

        chips[chip] = dst;
    }

    const size_t offset = simm_offset(spec, simm);

    if (is_graphics_simm(simm)) {
        Cps3_DecodeGraphicsSimm(rom->graphics + offset, chips, spec->chips[simm - 1], spec->chip_size);
    } else {
        Cps3_DecodeProgramSimm(
            rom->program + offset, chips, spec->chip_size, ROM_PROGRAM_BASE + (Uint32)offset, spec->key1, spec->key2
        );
    }

    return true;
}

static bool load_flash_file(Rom* rom, const RomSpec* spec, void* zip, const FlashFile* file, Uint8* staging) {
    if (!read_chip(zip, file->name, staging, file->size)) {
        return false;
    }

    const size_t offset = simm_offset(spec, file->simm) + file->offset;

    if (is_graphics_simm(file->simm)) {
        Cps3_DecodeGraphicsFlash(rom->graphics + offset, staging, file->size);
    } else {
        Cps3_DecodeProgramFlash(
            rom->program + offset, staging, file->size, ROM_PROGRAM_BASE + (Uint32)offset, spec->key1, spec->key2
        );
    }

    return true;
}

static bool has_entry(void* zip, const char* name) {
    return mz_zip_locate_first_entry(zip, (void*)name, match_base_name) == MZ_OK;
}

static bool load_files(Rom* rom, const RomSpec* spec, void* zip, Uint8* staging) {
    char first_chip[MAX_CHIP_NAME];
    SDL_snprintf(first_chip, sizeof(first_chip), spec->chip_name_format, 1, 0);
    bool success = true;

    if (spec->flash_file_count > 0 && !has_entry(zip, first_chip)) {
        for (int i = 0; i < spec->flash_file_count && success; i++) {
            success = load_flash_file(rom, spec, zip, &spec->flash_files[i], staging);
        }
    } else {
        for (int simm = 1; simm <= SIMM_COUNT && success; simm++) {
            if (spec->chips[simm - 1] > 0) {
                success = load_simm(rom, spec, zip, simm, staging);
            }
        }
    }

    return success;
}

static bool load_simms(Rom* rom, const RomSpec* spec, const char* path) {
    void* stream = mz_stream_os_create();
    void* zip = mz_zip_create();
    Uint8* staging = SDL_malloc(spec->chip_size * GRAPHICS_SIMM_CHIPS);
    bool success = false;

    if (stream == NULL || zip == NULL || staging == NULL) {
        SDL_OutOfMemory();
    } else if (mz_stream_open(stream, path, MZ_OPEN_MODE_READ) != MZ_OK ||
               mz_zip_open(zip, stream, MZ_OPEN_MODE_READ) != MZ_OK) {
        SDL_SetError("Couldn't open ROM set %s", path);
    } else {
        success = load_files(rom, spec, zip, staging);
        mz_zip_close(zip);
    }

    SDL_free(staging);

    if (zip != NULL) {
        mz_zip_delete(&zip);
    }

    if (stream != NULL) {
        mz_stream_os_delete(&stream);
    }

    return success;
}

const char* Rom_GetZipName(RomGame game) {
    return specs[game].zip_name;
}

Rom* Rom_Create(RomGame game, const char* path) {
    const RomSpec* spec = &specs[game];
    Rom* rom = SDL_calloc(1, sizeof(Rom));

    if (rom == NULL) {
        return NULL;
    }

    rom->game = game;

    for (int simm = 1; simm <= SIMM_COUNT; simm++) {
        if (spec->chips[simm - 1] > 0) {
            const size_t end = simm_offset(spec, simm) + simm_size(spec, simm);
            size_t* region_size = is_graphics_simm(simm) ? &rom->graphics_size : &rom->program_size;
            *region_size = SDL_max(*region_size, end);
        }
    }

    // Absent SIMMs between present ones are left zeroed
    rom->program = SDL_calloc(1, rom->program_size);
    rom->graphics = SDL_calloc(1, rom->graphics_size);

    if ((rom->program_size > 0 && rom->program == NULL) || (rom->graphics_size > 0 && rom->graphics == NULL) ||
        !load_simms(rom, spec, path)) {
        Rom_Destroy(rom);
        return NULL;
    }

    return rom;
}

void Rom_Destroy(Rom* rom) {
    if (rom == NULL) {
        return;
    }

    SDL_free(rom->program);
    SDL_free(rom->graphics);
    SDL_free(rom);
}

RomGame Rom_GetGame(const Rom* rom) {
    return rom->game;
}

const Uint8* Rom_GetSimm(const Rom* rom, int simm, size_t* size) {
    const RomSpec* spec = &specs[rom->game];

    if (simm < 1 || simm > SIMM_COUNT || spec->chips[simm - 1] == 0) {
        *size = 0;
        return NULL;
    }

    const Uint8* region = is_graphics_simm(simm) ? rom->graphics : rom->program;
    *size = simm_size(spec, simm);
    return region + simm_offset(spec, simm);
}

const Uint8* Rom_GetProgram(const Rom* rom, size_t* size) {
    *size = rom->program_size;
    return rom->program;
}

const Uint8* Rom_GetGraphics(const Rom* rom, size_t* size) {
    *size = rom->graphics_size;
    return rom->graphics;
}
