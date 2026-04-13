#if ARCADE_ROM

#include "arcade/arcade_char_data.h"
#include "arcade/rom_load.h"
#include "constants.h"
#include "port/resources.h"
#include "structs.h"

#include <SDL3/SDL.h>

#define BASE_OFFSET 0x6000000

// Uncomment to enable parsed char data dumping to dump folder
// #define DUMP_CHAR_DATA

typedef struct Location {
    Uint32 offset;
    Uint32 size;
} Location;

typedef struct LocationData {
    Location nmca;
    Location dmca;
    Location btca;
    Location caca;
    Location cuca;
    Location atca;
    Location saca;
    Location exca;
    Location cbca;
    Location yuca;
    Location stxy;
    Location mvxy;
    Location sernd;
    Location ovct;
    Location ovix;
    Location rict;
    Location hiit;
    Location boda;
    Location hana;
    Location cata;
    Location caua;
    Location atta;
    Location hosa;
    Location atit;
    Location prot;
} LocationData;

static CharInitData data[NUM_CHARS] = { 0 };
static bool initialized = false;

// Forward decls
static const int cg_number_offsets[NUM_CHARS];
static const LocationData location_data[NUM_CHARS];

int SDLCALL compare(const void* lhs, const void* rhs) {
    const Uint32 a = *(Uint32*)lhs;
    const Uint32 b = *(Uint32*)rhs;

    if (a < b) {
        return -1;
    } else if (a > b) {
        return 1;
    } else {
        return 0;
    }
}

static Uint16 remap_cg_number(Uint16 value, Character character) {
    if (value < 0x400) {
        return value;
    }

    int adjusted = value + cg_number_offsets[character];

    switch (character) {
    case CHAR_IBUKI:
        // Ibuki uses two CPS3-only high cg-number banks that map down into her normal PS2 range.
        if (value >= 0x70BD && value <= 0x70C7) {
            adjusted = value - 18692;
        } else if (value >= 0x9BA8 && value <= 0x9C6F) {
            adjusted = value - 29904;
        }

        break;

    case CHAR_MAKOTO:
        // Makoto has a separate high cg-number bank that maps with an additional shift on PS2.
        if (value >= 0xA000) {
            adjusted -= 0x45F8;
        }

        break;

    default:
        // Do nothing
        break;
    }

    if (adjusted < 0 || adjusted > 0xFFFF) {
        return value;
    }

    return adjusted;
}

static Uint16 remap_ovct_parts_char(Uint16 value, Character character) {
    switch (character) {
    case CHAR_IBUKI:
        if (value >= 0x9660 && value <= 0x96D7) {
            return value - 28671;
        }

        if ((value >= 0x9BCE && value <= 0x9BD4) || (value >= 0x9C43 && value <= 0x9C45) ||
            (value >= 0x9C70 && value <= 0x9C7A)) {
            return value - 29903;
        }

        break;

    case CHAR_URIEN:
        if (value >= 0x4E00 && value <= 0x533F) {
            return value - 3168;
        }

        if (value >= 0x9F58 && value <= 0x9F64) {
            return value - 10809;
        }

        break;

    default:
        // Do nothing
        break;
    }

    return remap_cg_number(value, character);
}

static const void* read_char_table(SDL_IOStream* rom, Location location, Character character) {
    void* result = SDL_malloc(location.size);
    SDL_memset(result, 0, location.size);

    // Read script offsets
    Uint32* offsets = result;
    int offset_count = 0;
    SDL_SeekIO(rom, location.offset, SDL_IO_SEEK_SET);

    for (;; offset_count++) {
        Uint32 value = 0;
        SDL_ReadU32BE(rom, &value);

        if (value == 0) {
            offsets[offset_count] = 0;
            break;
        }

        offsets[offset_count] = value - BASE_OFFSET - location.offset;
    }

    // Calculate script sizes
    const size_t script_offsets_size = offset_count * sizeof(Uint32);
    Uint32* script_offsets = SDL_malloc(script_offsets_size);
    SDL_memcpy(script_offsets, offsets, script_offsets_size);
    SDL_qsort(script_offsets, offset_count, sizeof(script_offsets[0]), compare);

    // Parse data

    for (int i = 0; i < offset_count; i++) {
        const Uint32 start_offset = script_offsets[i] - 8;
        const Uint32 end_offset = (i == (offset_count - 1)) ? location.size : script_offsets[i + 1] - 8;

        SDL_SeekIO(rom, location.offset + start_offset, SDL_IO_SEEK_SET);
        Uint8* p = (Uint8*)result + start_offset;

        // Read script header
        Sint16 cgd_type = 0;
        SDL_ReadS16BE(rom, &cgd_type);
        SDL_assert(cgd_type == 1 || cgd_type == 2 || cgd_type == 4 || cgd_type == 6);
        *(Sint16*)p = cgd_type;
        p += 2;

        for (int i = 0; i < 6; i++) {
            SDL_ReadU8(rom, p); // pat_status ... sp_tech_id
            p += 1;
        }

        while (p < (Uint8*)result + end_offset) {
            Uint16 code = 0;
            SDL_ReadU16BE(rom, &code);

            if (code < 0x100) {
                *(Uint16*)p = code;
                p += 2;

                for (int i = 0; i < 3; i++) {
                    SDL_ReadS16BE(rom, p); // koc ... pat
                    p += 2;
                }

                const int left_to_move = SDL_max(cgd_type * 4 - 8, 0);
                p += left_to_move;
                SDL_SeekIO(rom, left_to_move, SDL_IO_SEEK_CUR);
            } else {
                const Uint8 cg_ctr = code >> 8;
                const Uint8 cg_type = code & 0xFF;
                *p++ = cg_type;
                *p++ = cg_ctr;

                for (int i = 0; i < 2; i++) {
                    SDL_ReadU16BE(rom, p); // cg_se ... cg_olc_ix
                    p += 2;
                }

                Uint16 cg_number = 0;
                SDL_ReadU16BE(rom, &cg_number);
                cg_number = remap_cg_number(cg_number, character);
                *(Uint16*)p = cg_number;
                p += 2;

                if (cgd_type >= 4) {
                    Sint16 cg_att_ix = 0;
                    Uint16 cg_hit_ix = 0;
                    SDL_ReadS16BE(rom, &cg_att_ix);
                    SDL_ReadU16BE(rom, &cg_hit_ix);

                    *(Uint16*)p = cg_hit_ix;
                    p += 2;
                    *(Sint16*)p = cg_att_ix;
                    p += 2;

                    for (int i = 0; i < 4; i++) {
                        SDL_ReadU8(rom, p); // cg_extdat ... cg_eftype
                        p += 1;
                    }
                }

                if (cgd_type == 6) {
                    for (int i = 0; i < 3; i++) {
                        SDL_ReadU16BE(rom, p); // cg_zoom ... cg_add_xy
                        p += 2;
                    }

                    for (int i = 0; i < 2; i++) {
                        SDL_ReadU8(rom, p); // cg_next_ix ... cg_status
                        p += 1;
                    }
                }
            }
        }
    }

    // Cleanup
    SDL_free(script_offsets);

    return result;
}

static const void* read_u8_array(SDL_IOStream* rom, Location location) {
    SDL_SeekIO(rom, location.offset, SDL_IO_SEEK_SET);
    void* result = SDL_malloc(location.size);
    SDL_ReadIO(rom, result, location.size);
    return result;
}

static const void* read_s16_array(SDL_IOStream* rom, Location location) {
    SDL_SeekIO(rom, location.offset, SDL_IO_SEEK_SET);
    void* result = SDL_malloc(location.size);
    Sint16* values = result;

    for (int i = 0; i < location.size / 2; i++) {
        SDL_ReadS16BE(rom, values);
        values++;
    }

    return result;
}

static const void* read_u16_array(SDL_IOStream* rom, Location location) {
    SDL_SeekIO(rom, location.offset, SDL_IO_SEEK_SET);
    void* result = SDL_malloc(location.size);
    Uint16* values = result;

    for (int i = 0; i < location.size / 2; i++) {
        SDL_ReadU16BE(rom, values);
        values++;
    }

    return result;
}

static const void* read_sernd(SDL_IOStream* rom, Location location) {
    SDL_SeekIO(rom, location.offset, SDL_IO_SEEK_SET);

    void* result = SDL_malloc(location.size);
    const int offset_count = location.size / 0x24;
    Uint32* offsets = result;

    for (int i = 0; i < offset_count; i++) {
        SDL_ReadU32BE(rom, offsets);
        *offsets -= (BASE_OFFSET + location.offset);
        offsets++;
    }

    Uint16* values = offsets;
    const int value_count = offset_count * 16;

    for (int i = 0; i < value_count; i++) {
        SDL_ReadU16BE(rom, values);
        values++;
    }

    return result;
}

static const void* read_ovct(SDL_IOStream* rom, Location location, Character character) {
    SDL_SeekIO(rom, location.offset, SDL_IO_SEEK_SET);

    UNK_8* result = SDL_malloc(location.size);
    const int elem_count = location.size / sizeof(UNK_8);

    for (int i = 0; i < elem_count; i++) {
        UNK_8* element = &result[i];
        SDL_ReadS16BE(rom, &element->parts_hos_x);
        SDL_ReadS16BE(rom, &element->parts_hos_y);
        SDL_ReadU8(rom, &element->parts_colmd);
        SDL_ReadU8(rom, &element->parts_colcd);
        SDL_ReadU8(rom, &element->parts_prio);
        SDL_ReadU8(rom, &element->parts_flip);
        SDL_ReadU8(rom, &element->parts_timer);
        SDL_ReadU8(rom, &element->parts_disp);
        SDL_ReadS16BE(rom, &element->parts_mts);
        SDL_ReadU16BE(rom, &element->parts_nix);
        SDL_ReadU16BE(rom, &element->parts_char);
        element->parts_char = remap_ovct_parts_char(element->parts_char, character);

        if (element->parts_mts == 0) {
            switch (character) {
            case CHAR_KEN:
                // Ken's DP overlap flame uses additive MTS in PS2 data.
                element->parts_mts = 1;
                break;

            case CHAR_URIEN:
                if ((element->parts_char >= 17854 && element->parts_char <= 17879) ||
                    (element->parts_char >= 18017 && element->parts_char <= 18037) ||
                    element->parts_char == 18060 || (element->parts_char >= 18096 && element->parts_char <= 18100) ||
                    (element->parts_char >= 18129 && element->parts_char <= 18131) ||
                    element->parts_char == 18143 || (element->parts_char >= 29983 && element->parts_char <= 29995)) {
                    element->parts_mts = 1;
                }
                break;

            default:
                break;
            }
        }
    }

    return result;
}

static const void* read_catch_table(SDL_IOStream* rom, Location location) {
    SDL_SeekIO(rom, location.offset, SDL_IO_SEEK_SET);

    CatchTable* result = SDL_malloc(location.size);
    const int elem_count = location.size / sizeof(CatchTable);

    for (int i = 0; i < elem_count; i++) {
        CatchTable* element = &result[i];
        SDL_ReadS16BE(rom, &element->catch_hos_x);
        SDL_ReadS16BE(rom, &element->catch_hos_y);
        SDL_ReadU8(rom, &element->catch_prio);
        SDL_ReadU8(rom, &element->catch_flip);
        SDL_ReadS16BE(rom, &element->catch_nix);
    }

    return result;
}

#if DEBUG && DUMP_CHAR_DATA
static void dump(const void* buf, const char* name, size_t length, Character character) {
    char* path;
    SDL_asprintf(&path, "dump/%s_pl%02d", name, character);

    SDL_IOStream* io = SDL_IOFromFile(path, "wb");
    SDL_WriteIO(io, buf, length);

    SDL_CloseIO(io);
    SDL_free(path);
}

static void dump_data(CharInitData* data, Character character) {
    const LocationData* locations = &location_data[character];
    dump(data->nmca, "nmca", locations->nmca.size, character);
    dump(data->dmca, "dmca", locations->dmca.size, character);
    dump(data->btca, "btca", locations->btca.size, character);
    dump(data->caca, "caca", locations->caca.size, character);
    dump(data->cuca, "cuca", locations->cuca.size, character);
    dump(data->atca, "atca", locations->atca.size, character);
    dump(data->saca, "saca", locations->saca.size, character);
    dump(data->exca, "exca", locations->exca.size, character);
    dump(data->cbca, "cbca", locations->cbca.size, character);
    dump(data->yuca, "yuca", locations->yuca.size, character);
    dump(data->stxy, "stxy", locations->stxy.size, character);
    dump(data->mvxy, "mvxy", locations->mvxy.size, character);
    dump(data->sernd, "sernd", locations->sernd.size, character);
    dump(data->ovct, "ovct", locations->ovct.size, character);
    dump(data->ovix, "ovix", locations->ovix.size, character);
    dump(data->rict, "rict", locations->rict.size, character);
    dump(data->hiit, "hiit", locations->hiit.size, character);
    dump(data->boda, "boda", locations->boda.size, character);
    dump(data->hana, "hana", locations->hana.size, character);
    dump(data->cata, "cata", locations->cata.size, character);
    dump(data->caua, "caua", locations->caua.size, character);
    dump(data->atta, "atta", locations->atta.size, character);
    dump(data->hosa, "hosa", locations->hosa.size, character);
    dump(data->atit, "atit", locations->atit.size, character);
    dump(data->prot, "prot", locations->prot.size, character);
}
#endif

void ArcadeCharData_Init() {
    const char* rom_path = Resources_GetPath("sfiii3nr1.zip");
    size_t rom_size = 0;
    const void* rom = Rom_Load(rom_path, &rom_size);
    SDL_free(rom_path);

    if (rom == NULL) {
        return;
    }

    SDL_IOStream* io = SDL_IOFromConstMem(rom, rom_size);

    for (int character = 0; character < NUM_CHARS; character++) {
        const LocationData* locations = &location_data[character];
        CharInitData* dst = &data[character];
        dst->nmca = read_char_table(io, locations->nmca, character);
        dst->dmca = read_char_table(io, locations->dmca, character);
        dst->btca = read_char_table(io, locations->btca, character);
        dst->caca = read_char_table(io, locations->caca, character);
        dst->cuca = read_char_table(io, locations->cuca, character);
        dst->atca = read_char_table(io, locations->atca, character);
        dst->saca = read_char_table(io, locations->saca, character);
        dst->exca = read_char_table(io, locations->exca, character);
        dst->cbca = read_char_table(io, locations->cbca, character);
        dst->yuca = read_char_table(io, locations->yuca, character);
        dst->stxy = read_s16_array(io, locations->stxy);
        dst->mvxy = read_s16_array(io, locations->mvxy);
        dst->sernd = read_sernd(io, locations->sernd);
        dst->ovct = read_ovct(io, locations->ovct, character);
        dst->ovix = read_s16_array(io, locations->ovix);
        dst->rict = read_catch_table(io, locations->rict);
        dst->hiit = read_u16_array(io, locations->hiit);
        dst->boda = read_s16_array(io, locations->boda);
        dst->hana = read_s16_array(io, locations->hana);
        dst->cata = read_s16_array(io, locations->cata);
        dst->caua = read_s16_array(io, locations->caua);
        dst->atta = read_s16_array(io, locations->atta);
        dst->hosa = read_s16_array(io, locations->hosa);
        dst->atit = read_u8_array(io, locations->atit);
        dst->prot = read_s16_array(io, locations->prot);

#if DEBUG && DUMP_CHAR_DATA
        dump_data(dst, character);
#endif
    }

    initialized = true;
    SDL_CloseIO(io);
    SDL_free(rom);
}

const CharInitData* ArcadeCharData_Get(Character character) {
    if (!initialized) {
        return NULL;
    }

    return &data[character];
}

static const int cg_number_offsets[NUM_CHARS] = {
    0x0000,  0x0020,  -0x01E0, -0x0420, -0x0480, -0x0600, -0x0720, -0x0940, -0x0820, -0x0800,
    -0x0820, -0x08C0, -0x0AA0, -0x0C60, -0x0CA0, -0x0E00, -0x0D80, -0x0C20, -0x0B80, -0x0D00,
};

static const LocationData location_data[NUM_CHARS] = {
    {
        .nmca = { .offset = 0x1CB230, .size = 0x2660 },
        .dmca = { .offset = 0x1CD890, .size = 0x25B4 },
        .btca = { .offset = 0x1CFE44, .size = 0x1554 },
        .caca = { .offset = 0x1D1398, .size = 0x578 },
        .cuca = { .offset = 0x1D1910, .size = 0x2D54 },
        .atca = { .offset = 0x1D4664, .size = 0x2358 },
        .saca = { .offset = 0x1DBC98, .size = 0x25B4 },
        .exca = { .offset = 0x1D9ECC, .size = 0x1DCC },
        .cbca = { .offset = 0x1DE24C, .size = 0x2F0 },
        .yuca = { .offset = 0x36068C, .size = 0x1388 },
        .stxy = { .offset = 0x45D664, .size = 0x1C4 },
        .mvxy = { .offset = 0x45D828, .size = 0x270 },
        .sernd = { .offset = 0x453FB0, .size = 0x24 },
        .ovct = { .offset = 0x1D6E4C, .size = 0x1880 },
        .ovix = { .offset = 0x1D69BC, .size = 0x490 },
        .rict = { .offset = 0x1D86CC, .size = 0x1800 },
        .hiit = { .offset = 0x3EE998, .size = 0xD80 },
        .boda = { .offset = 0x3EF718, .size = 0x1600 },
        .hana = { .offset = 0x3F0D18, .size = 0x320 },
        .cata = { .offset = 0x443D58, .size = 0x10 },
        .caua = { .offset = 0x443D68, .size = 0x40 },
        .atta = { .offset = 0x443798, .size = 0x5C0 },
        .hosa = { .offset = 0x3F1038, .size = 0x60 },
        .atit = { .offset = 0x453D80, .size = 0x230 },
        .prot = { .offset = 0x45DA98, .size = 0x420 },
    },
    {
        .nmca = { .offset = 0x1DE53C, .size = 0x1C58 },
        .dmca = { .offset = 0x1E0194, .size = 0x27EC },
        .btca = { .offset = 0x1E2980, .size = 0x1694 },
        .caca = { .offset = 0x1E4014, .size = 0x33E8 },
        .cuca = { .offset = 0x1E73FC, .size = 0x2D1C },
        .atca = { .offset = 0x1EA118, .size = 0x2994 },
        .saca = { .offset = 0x1F4EC4, .size = 0x3EE8 },
        .exca = { .offset = 0x1F33A4, .size = 0x1B20 },
        .cbca = { .offset = 0x1F8DAC, .size = 0x4BC },
        .yuca = { .offset = 0x361A14, .size = 0x1150 },
        .stxy = { .offset = 0x45DEB8, .size = 0x3D0 },
        .mvxy = { .offset = 0x45E288, .size = 0x45C },
        .sernd = { .offset = 0x454624, .size = 0x24 },
        .ovct = { .offset = 0x1ECC54, .size = 0x390 },
        .ovix = { .offset = 0x1ECAAC, .size = 0x1A8 },
        .rict = { .offset = 0x1ECFE4, .size = 0x63C0 },
        .hiit = { .offset = 0x3F1098, .size = 0x13F0 },
        .boda = { .offset = 0x3F2488, .size = 0x1F00 },
        .hana = { .offset = 0x3F4388, .size = 0x800 },
        .cata = { .offset = 0x444768, .size = 0xC8 },
        .caua = { .offset = 0x444830, .size = 0x90 },
        .atta = { .offset = 0x443DA8, .size = 0x9C0 },
        .hosa = { .offset = 0x3F4B88, .size = 0xE8 },
        .atit = { .offset = 0x453FD4, .size = 0x650 },
        .prot = { .offset = 0x45E6E4, .size = 0x570 },
    },
    {
        .nmca = { .offset = 0x1F9268, .size = 0x16D0 },
        .dmca = { .offset = 0x1FA938, .size = 0x217C },
        .btca = { .offset = 0x1FCAB4, .size = 0x1584 },
        .caca = { .offset = 0x1FE038, .size = 0x724 },
        .cuca = { .offset = 0x1FE75C, .size = 0x2D24 },
        .atca = { .offset = 0x201480, .size = 0x2114 },
        .saca = { .offset = 0x20669C, .size = 0x2748 },
        .exca = { .offset = 0x20476C, .size = 0x1F30 },
        .cbca = { .offset = 0x208DE4, .size = 0x9D0 },
        .yuca = { .offset = 0x362B64, .size = 0x6E8 },
        .stxy = { .offset = 0x45EC54, .size = 0x1B0 },
        .mvxy = { .offset = 0x45EE04, .size = 0x324 },
        .sernd = { .offset = 0x454C08, .size = 0x24 },
        .ovct = { .offset = 0x2035CC, .size = 0x360 },
        .ovix = { .offset = 0x203594, .size = 0x38 },
        .rict = { .offset = 0x20392C, .size = 0xE40 },
        .hiit = { .offset = 0x3F4C70, .size = 0x11B0 },
        .boda = { .offset = 0x3F5E20, .size = 0x1580 },
        .hana = { .offset = 0x3F73A0, .size = 0x4A0 },
        .cata = { .offset = 0x445120, .size = 0x10 },
        .caua = { .offset = 0x445130, .size = 0x30 },
        .atta = { .offset = 0x4448C0, .size = 0x860 },
        .hosa = { .offset = 0x3F7840, .size = 0x30 },
        .atit = { .offset = 0x454648, .size = 0x5C0 },
        .prot = { .offset = 0x45F128, .size = 0x840 },
    },
    {
        .nmca = { .offset = 0x2097B4, .size = 0x22A4 },
        .dmca = { .offset = 0x20BA58, .size = 0x275C },
        .btca = { .offset = 0x20E1B4, .size = 0x14EC },
        .caca = { .offset = 0x20F6A0, .size = 0x184C },
        .cuca = { .offset = 0x210EEC, .size = 0x2D14 },
        .atca = { .offset = 0x213C00, .size = 0x6AC4 },
        .saca = { .offset = 0x2218E0, .size = 0x7D40 },
        .exca = { .offset = 0x21E604, .size = 0x32DC },
        .cbca = { .offset = 0x229620, .size = 0xE34 },
        .yuca = { .offset = 0x36324C, .size = 0x15F8 },
        .stxy = { .offset = 0x45F968, .size = 0x3A8 },
        .mvxy = { .offset = 0x45FD10, .size = 0x42C },
        .sernd = { .offset = 0x45570C, .size = 0x90 },
        .ovct = { .offset = 0x21A764, .size = 0x60 },
        .ovix = { .offset = 0x21A6C4, .size = 0xA0 },
        .rict = { .offset = 0x21A7C4, .size = 0x3E40 },
        .hiit = { .offset = 0x3F7870, .size = 0x18B0 },
        .boda = { .offset = 0x3F9120, .size = 0x2020 },
        .hana = { .offset = 0x3FB140, .size = 0x980 },
        .cata = { .offset = 0x446060, .size = 0x18 },
        .caua = { .offset = 0x446078, .size = 0x98 },
        .atta = { .offset = 0x445160, .size = 0xF00 },
        .hosa = { .offset = 0x3FBAC0, .size = 0xB0 },
        .atit = { .offset = 0x454C2C, .size = 0xAE0 },
        .prot = { .offset = 0x46013C, .size = 0x7E0 },
    },
    {
        .nmca = { .offset = 0x22A454, .size = 0x1C0C },
        .dmca = { .offset = 0x22C060, .size = 0x2954 },
        .btca = { .offset = 0x22E9B4, .size = 0x160C },
        .caca = { .offset = 0x22FFC0, .size = 0x7B8 },
        .cuca = { .offset = 0x230778, .size = 0x2CA4 },
        .atca = { .offset = 0x23341C, .size = 0x4200 },
        .saca = { .offset = 0x23A644, .size = 0x6AE4 },
        .exca = { .offset = 0x238DC4, .size = 0x1880 },
        .cbca = { .offset = 0x241128, .size = 0xF10 },
        .yuca = { .offset = 0x364844, .size = 0xD58 },
        .stxy = { .offset = 0x46091C, .size = 0x1A0 },
        .mvxy = { .offset = 0x460ABC, .size = 0x3FC },
        .sernd = { .offset = 0x455EFC, .size = 0x24 },
        .ovct = { .offset = 0x237764, .size = 0xB20 },
        .ovix = { .offset = 0x23761C, .size = 0x148 },
        .rict = { .offset = 0x238284, .size = 0xB40 },
        .hiit = { .offset = 0x3FBB70, .size = 0x1050 },
        .boda = { .offset = 0x3FCBC0, .size = 0x17A0 },
        .hana = { .offset = 0x3FE360, .size = 0x460 },
        .cata = { .offset = 0x446A50, .size = 0x10 },
        .caua = { .offset = 0x446A60, .size = 0xB8 },
        .atta = { .offset = 0x446110, .size = 0x940 },
        .hosa = { .offset = 0x3FE7C0, .size = 0xF0 },
        .atit = { .offset = 0x45579C, .size = 0x760 },
        .prot = { .offset = 0x460EB8, .size = 0x7B0 },
    },
    {
        .nmca = { .offset = 0x242038, .size = 0x1D78 },
        .dmca = { .offset = 0x243DB0, .size = 0x278C },
        .btca = { .offset = 0x24653C, .size = 0x132C },
        .caca = { .offset = 0x247868, .size = 0x2124 },
        .cuca = { .offset = 0x24998C, .size = 0x2D04 },
        .atca = { .offset = 0x24C690, .size = 0x3A04 },
        .saca = { .offset = 0x254AE4, .size = 0x3FF8 },
        .exca = { .offset = 0x2536C4, .size = 0x1420 },
        .cbca = { .offset = 0x258ADC, .size = 0x534 },
        .yuca = { .offset = 0x36559C, .size = 0xE20 },
        .stxy = { .offset = 0x461668, .size = 0x21C },
        .mvxy = { .offset = 0x461884, .size = 0x264 },
        .sernd = { .offset = 0x4564E0, .size = 0x24 },
        .ovct = { .offset = 0x2500E4, .size = 0x2E0 },
        .ovix = { .offset = 0x250094, .size = 0x50 },
        .rict = { .offset = 0x2503C4, .size = 0x3300 },
        .hiit = { .offset = 0x3FE8B0, .size = 0x1410 },
        .boda = { .offset = 0x3FFCC0, .size = 0x1C80 },
        .hana = { .offset = 0x401940, .size = 0x940 },
        .cata = { .offset = 0x447278, .size = 0x30 },
        .caua = { .offset = 0x4472A8, .size = 0x88 },
        .atta = { .offset = 0x446B18, .size = 0x760 },
        .hosa = { .offset = 0x402280, .size = 0xD8 },
        .atit = { .offset = 0x455F20, .size = 0x5C0 },
        .prot = { .offset = 0x461AE8, .size = 0x450 },
    },
    {
        .nmca = { .offset = 0x259010, .size = 0x1A48 },
        .dmca = { .offset = 0x25AA58, .size = 0x241C },
        .btca = { .offset = 0x25CE74, .size = 0x156C },
        .caca = { .offset = 0x25E3E0, .size = 0x2C40 },
        .cuca = { .offset = 0x261020, .size = 0x2D44 },
        .atca = { .offset = 0x263D64, .size = 0x211C },
        .saca = { .offset = 0x26ECC0, .size = 0x4164 },
        .exca = { .offset = 0x26C360, .size = 0x2960 },
        .cbca = { .offset = 0x272E24, .size = 0x920 },
        .yuca = { .offset = 0x3663BC, .size = 0xD88 },
        .stxy = { .offset = 0x461F38, .size = 0xF4 },
        .mvxy = { .offset = 0x46202C, .size = 0x474 },
        .sernd = { .offset = 0x456984, .size = 0x24 },
        .ovct = { .offset = 0x266070, .size = 0x470 },
        .ovix = { .offset = 0x265E80, .size = 0x1F0 },
        .rict = { .offset = 0x2664E0, .size = 0x5E80 },
        .hiit = { .offset = 0x402358, .size = 0x1020 },
        .boda = { .offset = 0x403378, .size = 0x11C0 },
        .hana = { .offset = 0x404538, .size = 0x660 },
        .cata = { .offset = 0x447B50, .size = 0x60 },
        .caua = { .offset = 0x447BB0, .size = 0x60 },
        .atta = { .offset = 0x447330, .size = 0x820 },
        .hosa = { .offset = 0x404B98, .size = 0x80 },
        .atit = { .offset = 0x456504, .size = 0x480 },
        .prot = { .offset = 0x4624A0, .size = 0x630 },
    },
    {
        .nmca = { .offset = 0x273744, .size = 0x2458 },
        .dmca = { .offset = 0x275B9C, .size = 0x302C },
        .btca = { .offset = 0x278BC8, .size = 0x1674 },
        .caca = { .offset = 0x27A23C, .size = 0x2144 },
        .cuca = { .offset = 0x27C380, .size = 0x2CC4 },
        .atca = { .offset = 0x27F044, .size = 0x54C4 },
        .saca = { .offset = 0x296D50, .size = 0x9158 },
        .exca = { .offset = 0x293C18, .size = 0x3138 },
        .cbca = { .offset = 0x29FEA8, .size = 0xCF4 },
        .yuca = { .offset = 0x367144, .size = 0x1820 },
        .stxy = { .offset = 0x462AD0, .size = 0x354 },
        .mvxy = { .offset = 0x462E24, .size = 0x6D8 },
        .sernd = { .offset = 0x457518, .size = 0x24 },
        .ovct = { .offset = 0x288AB8, .size = 0x8EE0 },
        .ovix = { .offset = 0x284508, .size = 0x45B0 },
        .rict = { .offset = 0x291998, .size = 0x2280 },
        .hiit = { .offset = 0x404C18, .size = 0x1970 },
        .boda = { .offset = 0x406588, .size = 0x2220 },
        .hana = { .offset = 0x4087A8, .size = 0xB00 },
        .cata = { .offset = 0x448C30, .size = 0x60 },
        .caua = { .offset = 0x448C90, .size = 0x68 },
        .atta = { .offset = 0x447C10, .size = 0x1020 },
        .hosa = { .offset = 0x4092A8, .size = 0x90 },
        .atit = { .offset = 0x4569A8, .size = 0xB70 },
        .prot = { .offset = 0x4634FC, .size = 0x720 },
    },
    {
        .nmca = { .offset = 0x2A0B9C, .size = 0x2BB0 },
        .dmca = { .offset = 0x2A374C, .size = 0x2754 },
        .btca = { .offset = 0x2A5EA0, .size = 0x148C },
        .caca = { .offset = 0x2A732C, .size = 0x1DC },
        .cuca = { .offset = 0x2A7508, .size = 0x2D8C },
        .atca = { .offset = 0x2AA294, .size = 0x45EC },
        .saca = { .offset = 0x2B2F04, .size = 0x6638 },
        .exca = { .offset = 0x2AF948, .size = 0x35BC },
        .cbca = { .offset = 0x2B953C, .size = 0xA80 },
        .yuca = { .offset = 0x368964, .size = 0x2DA8 },
        .stxy = { .offset = 0x463C1C, .size = 0x2C0 },
        .mvxy = { .offset = 0x463EDC, .size = 0x39C },
        .sernd = { .offset = 0x457C8C, .size = 0x48 },
        .ovct = { .offset = 0x2AEB58, .size = 0x5B0 },
        .ovix = { .offset = 0x2AE880, .size = 0x2D8 },
        .rict = { .offset = 0x2AF108, .size = 0x840 },
        .hiit = { .offset = 0x409338, .size = 0x1D10 },
        .boda = { .offset = 0x40B048, .size = 0x3180 },
        .hana = { .offset = 0x40E1C8, .size = 0x10A0 },
        .cata = { .offset = 0x4498B8, .size = 0x10 },
        .caua = { .offset = 0x4498C8, .size = 0x50 },
        .atta = { .offset = 0x448CF8, .size = 0xBC0 },
        .hosa = { .offset = 0x40F268, .size = 0x60 },
        .atit = { .offset = 0x45753C, .size = 0x750 },
        .prot = { .offset = 0x464278, .size = 0x5A0 },
    },
    {
        .nmca = { .offset = 0x2B9FBC, .size = 0x1D88 },
        .dmca = { .offset = 0x2BBD44, .size = 0x22CC },
        .btca = { .offset = 0x2BE010, .size = 0x1474 },
        .caca = { .offset = 0x2BF484, .size = 0x24D0 },
        .cuca = { .offset = 0x2C1954, .size = 0x2D0C },
        .atca = { .offset = 0x2C4660, .size = 0x3540 },
        .saca = { .offset = 0x2CD3F8, .size = 0x4054 },
        .exca = { .offset = 0x2CB658, .size = 0x1DA0 },
        .cbca = { .offset = 0x2D144C, .size = 0xB30 },
        .yuca = { .offset = 0x36B70C, .size = 0x1880 },
        .stxy = { .offset = 0x464818, .size = 0x17C },
        .mvxy = { .offset = 0x464994, .size = 0x384 },
        .sernd = { .offset = 0x458274, .size = 0x24 },
        .ovct = { .offset = 0x2C7CE8, .size = 0x430 },
        .ovix = { .offset = 0x2C7BA0, .size = 0x148 },
        .rict = { .offset = 0x2C8118, .size = 0x3540 },
        .hiit = { .offset = 0x40F2C8, .size = 0x11F0 },
        .boda = { .offset = 0x4104B8, .size = 0x1B40 },
        .hana = { .offset = 0x411FF8, .size = 0x840 },
        .cata = { .offset = 0x44A0F8, .size = 0x50 },
        .caua = { .offset = 0x44A148, .size = 0x68 },
        .atta = { .offset = 0x449918, .size = 0x7E0 },
        .hosa = { .offset = 0x412838, .size = 0xB0 },
        .atit = { .offset = 0x457CD4, .size = 0x5A0 },
        .prot = { .offset = 0x464D18, .size = 0x570 },
    },
    {
        .nmca = { .offset = 0x2D1F7C, .size = 0x1DEC },
        .dmca = { .offset = 0x2D3D68, .size = 0x274C },
        .btca = { .offset = 0x2D64B4, .size = 0x150C },
        .caca = { .offset = 0x2D79C0, .size = 0x161C },
        .cuca = { .offset = 0x2D8FDC, .size = 0x2D14 },
        .atca = { .offset = 0x2DBCF0, .size = 0x603C },
        .saca = { .offset = 0x2E8F64, .size = 0x7054 },
        .exca = { .offset = 0x2E5ECC, .size = 0x3098 },
        .cbca = { .offset = 0x2EFFB8, .size = 0xF50 },
        .yuca = { .offset = 0x36CF8C, .size = 0x11E8 },
        .stxy = { .offset = 0x465288, .size = 0x3F4 },
        .mvxy = { .offset = 0x46567C, .size = 0x4EC },
        .sernd = { .offset = 0x458CF8, .size = 0x48 },
        .ovct = { .offset = 0x2E1DCC, .size = 0x140 },
        .ovix = { .offset = 0x2E1D2C, .size = 0xA0 },
        .rict = { .offset = 0x2E1F0C, .size = 0x3FC0 },
        .hiit = { .offset = 0x4128E8, .size = 0x1840 },
        .boda = { .offset = 0x414128, .size = 0x1EA0 },
        .hana = { .offset = 0x415FC8, .size = 0x940 },
        .cata = { .offset = 0x44B030, .size = 0x18 },
        .caua = { .offset = 0x44B048, .size = 0x98 },
        .atta = { .offset = 0x44A1B0, .size = 0xE80 },
        .hosa = { .offset = 0x416908, .size = 0xB8 },
        .atit = { .offset = 0x458298, .size = 0xA60 },
        .prot = { .offset = 0x465B68, .size = 0x630 },
    },
    {
        .nmca = { .offset = 0x2F0F08, .size = 0x16D0 },
        .dmca = { .offset = 0x2F25D8, .size = 0x217C },
        .btca = { .offset = 0x2F4754, .size = 0x1584 },
        .caca = { .offset = 0x2F5CD8, .size = 0xE38 },
        .cuca = { .offset = 0x2F6B10, .size = 0x2D24 },
        .atca = { .offset = 0x2F9834, .size = 0x2CEC },
        .saca = { .offset = 0x3001EC, .size = 0x26A8 },
        .exca = { .offset = 0x2FEA40, .size = 0x17AC },
        .cbca = { .offset = 0x302894, .size = 0x5D0 },
        .yuca = { .offset = 0x36E174, .size = 0x938 },
        .stxy = { .offset = 0x466198, .size = 0x250 },
        .mvxy = { .offset = 0x4663E8, .size = 0x360 },
        .sernd = { .offset = 0x4594B0, .size = 0x24 },
        .ovct = { .offset = 0x2FC5E0, .size = 0x2A0 },
        .ovix = { .offset = 0x2FC520, .size = 0xC0 },
        .rict = { .offset = 0x2FC880, .size = 0x21C0 },
        .hiit = { .offset = 0x4169C0, .size = 0x1040 },
        .boda = { .offset = 0x417A00, .size = 0x12C0 },
        .hana = { .offset = 0x418CC0, .size = 0x540 },
        .cata = { .offset = 0x44B9E0, .size = 0x10 },
        .caua = { .offset = 0x44B9F0, .size = 0x30 },
        .atta = { .offset = 0x44B0E0, .size = 0x900 },
        .hosa = { .offset = 0x419200, .size = 0x30 },
        .atit = { .offset = 0x458D40, .size = 0x770 },
        .prot = { .offset = 0x466748, .size = 0x750 },
    },
    {
        .nmca = { .offset = 0x302E64, .size = 0x16D0 },
        .dmca = { .offset = 0x304534, .size = 0x215C },
        .btca = { .offset = 0x306690, .size = 0x1584 },
        .caca = { .offset = 0x307C14, .size = 0x12C0 },
        .cuca = { .offset = 0x308ED4, .size = 0x2D24 },
        .atca = { .offset = 0x30BBF8, .size = 0x21AC },
        .saca = { .offset = 0x311BD0, .size = 0x2A88 },
        .exca = { .offset = 0x3101BC, .size = 0x1A14 },
        .cbca = { .offset = 0x314658, .size = 0x73C },
        .yuca = { .offset = 0x36EAAC, .size = 0xDA8 },
        .stxy = { .offset = 0x466E98, .size = 0x350 },
        .mvxy = { .offset = 0x4671E8, .size = 0x564 },
        .sernd = { .offset = 0x459B74, .size = 0x24 },
        .ovct = { .offset = 0x30DE6C, .size = 0x190 },
        .ovix = { .offset = 0x30DDA4, .size = 0xC8 },
        .rict = { .offset = 0x30DFFC, .size = 0x21C0 },
        .hiit = { .offset = 0x419230, .size = 0x1100 },
        .boda = { .offset = 0x41A330, .size = 0x1480 },
        .hana = { .offset = 0x41B7B0, .size = 0x620 },
        .cata = { .offset = 0x44CA28, .size = 0x18 },
        .caua = { .offset = 0x44CA40, .size = 0xD0 },
        .atta = { .offset = 0x44BA20, .size = 0x1008 },
        .hosa = { .offset = 0x41BDD0, .size = 0x38 },
        .atit = { .offset = 0x4594D4, .size = 0x6A0 },
        .prot = { .offset = 0x46774C, .size = 0x750 },
    },
    {
        .nmca = { .offset = 0x314D94, .size = 0x1A78 },
        .dmca = { .offset = 0x31680C, .size = 0x2594 },
        .btca = { .offset = 0x318DA0, .size = 0x148C },
        .caca = { .offset = 0x31A22C, .size = 0x568 },
        .cuca = { .offset = 0x31A794, .size = 0x2D54 },
        .atca = { .offset = 0x31D4E8, .size = 0x290C },
        .saca = { .offset = 0x324858, .size = 0x3A54 },
        .exca = { .offset = 0x32305C, .size = 0x17FC },
        .cbca = { .offset = 0x3282AC, .size = 0x47C },
        .yuca = { .offset = 0x36F854, .size = 0x1058 },
        .stxy = { .offset = 0x467E9C, .size = 0x25C },
        .mvxy = { .offset = 0x4680F8, .size = 0x330 },
        .sernd = { .offset = 0x459F28, .size = 0x24 },
        .ovct = { .offset = 0x3203CC, .size = 0x1490 },
        .ovix = { .offset = 0x31FDF4, .size = 0x5D8 },
        .rict = { .offset = 0x32185C, .size = 0x1800 },
        .hiit = { .offset = 0x41BE08, .size = 0xF30 },
        .boda = { .offset = 0x41CD38, .size = 0x16E0 },
        .hana = { .offset = 0x41E418, .size = 0x580 },
        .cata = { .offset = 0x44D1F0, .size = 0x10 },
        .caua = { .offset = 0x44D200, .size = 0x48 },
        .atta = { .offset = 0x44CB10, .size = 0x6E0 },
        .hosa = { .offset = 0x41E998, .size = 0x78 },
        .atit = { .offset = 0x459B98, .size = 0x390 },
        .prot = { .offset = 0x468428, .size = 0x480 },
    },
    {
        .nmca = { .offset = 0x328728, .size = 0x16A0 },
        .dmca = { .offset = 0x329DC8, .size = 0x217C },
        .btca = { .offset = 0x32BF44, .size = 0x1584 },
        .caca = { .offset = 0x32D4C8, .size = 0xF3C },
        .cuca = { .offset = 0x32E404, .size = 0x2D24 },
        .atca = { .offset = 0x331128, .size = 0x2810 },
        .saca = { .offset = 0x337EBC, .size = 0x3544 },
        .exca = { .offset = 0x335978, .size = 0x2544 },
        .cbca = { .offset = 0x33B400, .size = 0x788 },
        .yuca = { .offset = 0x3708AC, .size = 0xD84 },
        .stxy = { .offset = 0x4688A8, .size = 0x240 },
        .mvxy = { .offset = 0x468AE8, .size = 0x4A4 },
        .sernd = { .offset = 0x45A54C, .size = 0x24 },
        .ovct = { .offset = 0x333A48, .size = 0x730 },
        .ovix = { .offset = 0x333938, .size = 0x110 },
        .rict = { .offset = 0x334178, .size = 0x1800 },
        .hiit = { .offset = 0x41EA10, .size = 0x1190 },
        .boda = { .offset = 0x41FBA0, .size = 0x1540 },
        .hana = { .offset = 0x4210E0, .size = 0x620 },
        .cata = { .offset = 0x44DC28, .size = 0x98 },
        .caua = { .offset = 0x44DCC0, .size = 0x30 },
        .atta = { .offset = 0x44D248, .size = 0x9E0 },
        .hosa = { .offset = 0x421700, .size = 0x30 },
        .atit = { .offset = 0x459F4C, .size = 0x600 },
        .prot = { .offset = 0x468F8C, .size = 0x930 },
    },
    {
        .nmca = { .offset = 0x3723B4, .size = 0x21F8 },
        .dmca = { .offset = 0x3745AC, .size = 0x2574 },
        .btca = { .offset = 0x376B20, .size = 0x151C },
        .caca = { .offset = 0x37803C, .size = 0x684 },
        .cuca = { .offset = 0x3786C0, .size = 0x2CF4 },
        .atca = { .offset = 0x37B3B4, .size = 0x3710 },
        .saca = { .offset = 0x382940, .size = 0x57CC },
        .exca = { .offset = 0x38024C, .size = 0x26F4 },
        .cbca = { .offset = 0x38810C, .size = 0x660 },
        .yuca = { .offset = 0x3E2E00, .size = 0x1000 },
        .stxy = { .offset = 0x46A544, .size = 0x28C },
        .mvxy = { .offset = 0x46A7D0, .size = 0x234 },
        .sernd = { .offset = 0x45B294, .size = 0x24 },
        .ovct = { .offset = 0x37ED1C, .size = 0x4B0 },
        .ovix = { .offset = 0x37EAC4, .size = 0x258 },
        .rict = { .offset = 0x37F1CC, .size = 0x1080 },
        .hiit = { .offset = 0x423D50, .size = 0x1FF0 },
        .boda = { .offset = 0x425D40, .size = 0x3240 },
        .hana = { .offset = 0x428F80, .size = 0x1B00 },
        .cata = { .offset = 0x44F310, .size = 0x18 },
        .caua = { .offset = 0x44F328, .size = 0xA8 },
        .atta = { .offset = 0x44E670, .size = 0xCA0 },
        .hosa = { .offset = 0x42AA80, .size = 0x48 },
        .atit = { .offset = 0x45AAD4, .size = 0x7C0 },
        .prot = { .offset = 0x46AA04, .size = 0x780 },
    },
    {
        .nmca = { .offset = 0x38876C, .size = 0x2350 },
        .dmca = { .offset = 0x38AABC, .size = 0x230C },
        .btca = { .offset = 0x38CDC8, .size = 0x152C },
        .caca = { .offset = 0x38E2F4, .size = 0xA8C },
        .cuca = { .offset = 0x38ED80, .size = 0x2D04 },
        .atca = { .offset = 0x391A84, .size = 0x31BC },
        .saca = { .offset = 0x39D7DC, .size = 0x4E3C },
        .exca = { .offset = 0x399898, .size = 0x3F44 },
        .cbca = { .offset = 0x3A2618, .size = 0x594 },
        .yuca = { .offset = 0x3E3E00, .size = 0x1260 },
        .stxy = { .offset = 0x46B184, .size = 0x344 },
        .mvxy = { .offset = 0x46B4C8, .size = 0x2B8 },
        .sernd = { .offset = 0x45B798, .size = 0x24 },
        .ovct = { .offset = 0x395098, .size = 0xFC0 },
        .ovix = { .offset = 0x394C40, .size = 0x458 },
        .rict = { .offset = 0x396058, .size = 0x3840 },
        .hiit = { .offset = 0x42AAC8, .size = 0x1680 },
        .boda = { .offset = 0x42C148, .size = 0x2240 },
        .hana = { .offset = 0x42E388, .size = 0xBA0 },
        .cata = { .offset = 0x44FD30, .size = 0x30 },
        .caua = { .offset = 0x44FD60, .size = 0x88 },
        .atta = { .offset = 0x44F3D0, .size = 0x960 },
        .hosa = { .offset = 0x42EF28, .size = 0xA0 },
        .atit = { .offset = 0x45B2B8, .size = 0x4E0 },
        .prot = { .offset = 0x46B780, .size = 0x660 },
    },
    {
        .nmca = { .offset = 0x3A2BAC, .size = 0x2358 },
        .dmca = { .offset = 0x3A4F04, .size = 0x293C },
        .btca = { .offset = 0x3A7840, .size = 0x148C },
        .caca = { .offset = 0x3A8CCC, .size = 0x13A8 },
        .cuca = { .offset = 0x3AA074, .size = 0x2DAC },
        .atca = { .offset = 0x3ACE20, .size = 0x2F5C },
        .saca = { .offset = 0x3B41D0, .size = 0x52DC },
        .exca = { .offset = 0x3B262C, .size = 0x1BA4 },
        .cbca = { .offset = 0x3B94AC, .size = 0x5B8 },
        .yuca = { .offset = 0x3E5060, .size = 0x13B0 },
        .stxy = { .offset = 0x46BDE0, .size = 0x3FC },
        .mvxy = { .offset = 0x46C1DC, .size = 0x270 },
        .sernd = { .offset = 0x45BC8C, .size = 0x24 },
        .ovct = { .offset = 0x3AFE0C, .size = 0x120 },
        .ovix = { .offset = 0x3AFD7C, .size = 0x90 },
        .rict = { .offset = 0x3AFF2C, .size = 0x2700 },
        .hiit = { .offset = 0x42EFC8, .size = 0x1E90 },
        .boda = { .offset = 0x430E58, .size = 0x3140 },
        .hana = { .offset = 0x433F98, .size = 0x1420 },
        .cata = { .offset = 0x450848, .size = 0x30 },
        .caua = { .offset = 0x450878, .size = 0x30 },
        .atta = { .offset = 0x44FDE8, .size = 0xA60 },
        .hosa = { .offset = 0x4353B8, .size = 0x40 },
        .atit = { .offset = 0x45B7BC, .size = 0x4D0 },
        .prot = { .offset = 0x46C44C, .size = 0x870 },
    },
    {
        .nmca = { .offset = 0x3B9A64, .size = 0x1E38 },
        .dmca = { .offset = 0x3BB89C, .size = 0x297C },
        .btca = { .offset = 0x3BE218, .size = 0x134C },
        .caca = { .offset = 0x3BF564, .size = 0xDF8 },
        .cuca = { .offset = 0x3C035C, .size = 0x2CD4 },
        .atca = { .offset = 0x3C3030, .size = 0x2F94 },
        .saca = { .offset = 0x3CA60C, .size = 0x5E50 },
        .exca = { .offset = 0x3C8AFC, .size = 0x1B10 },
        .cbca = { .offset = 0x3D045C, .size = 0x89C },
        .yuca = { .offset = 0x3E6410, .size = 0x11A8 },
        .stxy = { .offset = 0x46CCBC, .size = 0x22C },
        .mvxy = { .offset = 0x46CEE8, .size = 0x2E8 },
        .sernd = { .offset = 0x45C330, .size = 0x24 },
        .ovct = { .offset = 0x3C63EC, .size = 0x850 },
        .ovix = { .offset = 0x3C5FC4, .size = 0x428 },
        .rict = { .offset = 0x3C6C3C, .size = 0x1EC0 },
        .hiit = { .offset = 0x4353F8, .size = 0x1F70 },
        .boda = { .offset = 0x437368, .size = 0x2EE0 },
        .hana = { .offset = 0x43A248, .size = 0x2640 },
        .cata = { .offset = 0x451908, .size = 0x1A8 },
        .caua = { .offset = 0x451AB0, .size = 0x98 },
        .atta = { .offset = 0x4508A8, .size = 0x1060 },
        .hosa = { .offset = 0x43C888, .size = 0xF8 },
        .atit = { .offset = 0x45BCB0, .size = 0x680 },
        .prot = { .offset = 0x46D1D0, .size = 0x4B0 },
    },
    {
        .nmca = { .offset = 0x3D0CF8, .size = 0x23A0 },
        .dmca = { .offset = 0x3D3098, .size = 0x2504 },
        .btca = { .offset = 0x3D559C, .size = 0x1394 },
        .caca = { .offset = 0x3D6930, .size = 0x3E8 },
        .cuca = { .offset = 0x3D6D18, .size = 0x2D64 },
        .atca = { .offset = 0x3D9A7C, .size = 0x227C },
        .saca = { .offset = 0x3DF2B4, .size = 0x36B4 },
        .exca = { .offset = 0x3DCB10, .size = 0x27A4 },
        .cbca = { .offset = 0x3E2968, .size = 0x498 },
        .yuca = { .offset = 0x3E75B8, .size = 0x73E0 },
        .stxy = { .offset = 0x46D680, .size = 0x400 },
        .mvxy = { .offset = 0x46DA80, .size = 0x2F4 },
        .sernd = { .offset = 0x45C844, .size = 0x24 },
        .ovct = { .offset = 0x3DBDF0, .size = 0x2A0 },
        .ovix = { .offset = 0x3DBCF8, .size = 0xF8 },
        .rict = { .offset = 0x3DC090, .size = 0xA80 },
        .hiit = { .offset = 0x43C980, .size = 0x1910 },
        .boda = { .offset = 0x43E290, .size = 0x2720 },
        .hana = { .offset = 0x4409B0, .size = 0x1080 },
        .cata = { .offset = 0x452528, .size = 0x10 },
        .caua = { .offset = 0x452538, .size = 0x1848 },
        .atta = { .offset = 0x451B48, .size = 0x9E0 },
        .hosa = { .offset = 0x441A30, .size = 0x1D68 },
        .atit = { .offset = 0x45C354, .size = 0x4F0 },
        .prot = { .offset = 0x46DD74, .size = 0x570 },
    },
};

#endif // ARCADE_ROM
