#include "arcade/arcade_cg.h"
#include "arcade/byte_buffer.h"
#include "arcade/rom/cps3_dma.h"
#include "structs.h"

#define CG_COUNT 0x10000

typedef struct CgDescriptor {
    const Uint8* data;
    Uint16 allocation;
    Uint16 spans;
    Uint16 parts;
    Uint32 dictionary;
} CgDescriptor;

static Uint16 be16(const Uint8* p) {
    return ((Uint16)p[0] << 8) | p[1];
}

static Uint32 be32(const Uint8* p) {
    return ((Uint32)p[0] << 24) | ((Uint32)p[1] << 16) | ((Uint32)p[2] << 8) | p[3];
}

/// @return Pointer to `size` bytes of program data at `address`, or `NULL` if they are out of range.
static const Uint8* program_at(const ArcadeCgSource* source, Uint32 address, size_t size) {
    if (address < source->program_base || address - source->program_base > source->program_size ||
        size > source->program_size - (address - source->program_base)) {
        return NULL;
    }

    return source->program + (address - source->program_base);
}

static const Uint8* table_entry(const ArcadeCgSource* source, Uint16 cg) {
    return program_at(source, source->table + (Uint32)cg * source->entry_size, source->entry_size);
}

static bool descriptor_for(const ArcadeCgSource* source, Uint16 cg, CgDescriptor* result) {
    const Uint8* entry = table_entry(source, cg);

    if (entry == NULL) {
        return false;
    }

    // Descriptors follow the table
    const Uint32 address = be32(entry + source->entry_size - 4);
    const Uint8* p = program_at(source, address, 12);

    if (address < source->table + CG_COUNT * source->entry_size || p == NULL) {
        return false;
    }

    const Uint16 spans = be16(p + 4);
    const Uint16 parts = be16(p + 6);

    if (!spans || spans > 0x400 || parts > 0x400 ||
        program_at(source, address, 12 + (size_t)spans * 8 + (size_t)parts * 8) == NULL) {
        return false;
    }

    *result = (CgDescriptor) {
        .data = p, .allocation = be16(p + 2), .spans = spans, .parts = parts, .dictionary = be32(p + 8)
    };

    return true;
}

static bool graphics_offset(const ArcadeCgSource* source, Uint32 dma_word, size_t* offset) {
    const Uint64 byte_address = (Uint64)dma_word * 2;

    if (byte_address < 0x400000 || byte_address - 0x400000 >= source->graphics_size) {
        return false;
    }

    *offset = (size_t)(byte_address - 0x400000);
    return true;
}

static Uint8* decode_cram(const ArcadeCgSource* source, const CgDescriptor* descriptor, size_t* allocation) {
    *allocation = ((descriptor->allocation >> 5) + 1) * 0x1000;
    Uint8* cram = SDL_calloc(1, *allocation);
    size_t dictionary = 0;

    if (cram == NULL || !graphics_offset(source, descriptor->dictionary, &dictionary)) {
        SDL_free(cram);
        return NULL;
    }

    for (int i = 0; i < descriptor->spans; i++) {
        const Uint8* span = descriptor->data + 12 + i * 8;
        const size_t destination = (size_t)be16(span + 6) * 16;
        const size_t length = ((size_t)be16(span + 4) + 1) * 16;
        size_t offset = 0;

        if (destination > *allocation || length > *allocation - destination ||
            !graphics_offset(source, be32(span), &offset) ||
            !Cps3_DecodeDma(source->graphics, source->graphics_size, offset, dictionary, cram + destination, length)) {
            SDL_free(cram);
            return NULL;
        }
    }

    // DMA's ^3 write and the PPU's reversed four-byte pixel groups cancel;
    // this buffer is the logical row-major 16x16 tile image.
    return cram;
}

static int signed10(Uint16 value) {
    value &= 0x3ff;
    return value & 0x200 ? (int)value - 0x400 : value;
}

/// Index of a pixel within a 32x32 texture, in the PS2's swizzled order. Same as `dctex_linear`.
static int swizzle(int x, int y) {
    int index = 0;

    for (int bit = 0; bit < 5; bit++) {
        index |= ((x >> bit) & 1) << (bit * 2 + 1);
        index |= ((y >> bit) & 1) << (bit * 2);
    }

    return index;
}

/// Compresses 6bpp pixels into the format that `lz_ext_p6_fx` in mtrans.c expands.
static bool compress_p6(const Uint8* pixels, size_t length, ByteBuffer* output) {
    size_t i = 0;
    bool success = true;

    while (i < length && success) {
        // Back reference. Copies may overlap the bytes they produce.
        size_t best_length = 0;
        size_t best_distance = 0;

        for (size_t distance = 1; distance <= 256 && distance <= i; distance++) {
            size_t match = 0;

            while (match < 65 && i + match < length && pixels[i + match] == pixels[i - distance + match]) {
                match++;
            }

            if (match > best_length) {
                best_length = match;
                best_distance = distance;
            }
        }

        // Pairs of pixels that share the upper two bits
        size_t pairs = 0;

        while (pairs < 17 && i + pairs * 2 + 1 < length && (pixels[i + pairs * 2] & 0x30) == (pixels[i] & 0x30) &&
               (pixels[i + pairs * 2 + 1] & 0x30) == (pixels[i] & 0x30)) {
            pairs++;
        }

        const size_t short_length = SDL_min(best_length, 5);
        const int short_saving = (best_distance <= 16 && best_length >= 2) ? (int)short_length - 1 : 0;
        const int long_saving = best_length >= 3 ? (int)best_length - 2 : 0;
        const int pairs_saving = pairs >= 2 ? (int)pairs - 1 : 0;

        if (pairs_saving > 0 && pairs_saving >= short_saving && pairs_saving >= long_saving) {
            Uint8 code = 0xC0 | (pixels[i] & 0x30) | (Uint8)(pairs - 2);
            success = ByteBuffer_Append(output, &code, 1);

            for (size_t k = 0; k < pairs && success; k++) {
                const Uint8 packed = ((pixels[i + k * 2] & 0xF) << 4) | (pixels[i + k * 2 + 1] & 0xF);
                success = ByteBuffer_Append(output, &packed, 1);
            }

            i += pairs * 2;
        } else if (short_saving > 0 && short_saving >= long_saving) {
            const Uint8 code = 0x40 | (Uint8)((best_distance - 1) << 2) | (Uint8)(short_length - 2);
            success = ByteBuffer_Append(output, &code, 1);
            i += short_length;
        } else if (long_saving > 0) {
            const Uint16 value = (Uint16)(((best_distance - 1) << 6) | (best_length - 2));
            const Uint8 code[2] = { 0x80 | (value >> 8), value & 0xFF };
            success = ByteBuffer_Append(output, code, 2);
            i += best_length;
        } else {
            success = ByteBuffer_Append(output, &pixels[i], 1);
            i += 1;
        }
    }

    return success;
}

static bool add_cg(
    const ArcadeCgSource* source, Uint16 cg, ArcadeCgFlags flags, ByteBuffer* placements, ByteBuffer* pixels,
    Uint32* texture_offsets, int* texture_count
) {
    CgDescriptor descriptor;

    if (!descriptor_for(source, cg, &descriptor)) {
        Uint16 empty = 0;
        return ByteBuffer_Append(placements, &empty, sizeof(empty));
    }

    size_t cram_size = 0;
    Uint8* cram = decode_cram(source, &descriptor, &cram_size);

    if (cram == NULL) {
        return false;
    }

    ByteBuffer entries = { 0 };
    const Uint8* entry = table_entry(source, cg);
    const Sint16 origin_x = (Sint16)be16(entry + source->origin_offset);
    const Sint16 origin_y = (Sint16)be16(entry + source->origin_offset + 2);
    const Uint8 size_table[4] = { 8, 1, 2, 4 };
    int previous_x = 0;
    int previous_y = 0;
    int count = 0;
    bool success = true;

    for (int part_index = 0; part_index < descriptor.parts && success; part_index++) {
        const Uint8* part = descriptor.data + 12 + descriptor.spans * 8 + part_index * 8;
        const int tile_base = be16(part) / 2;
        const Uint16 attributes = be16(part + 2);
        const int shape = be16(part + 6) >> 12;
        const int tiles_x = size_table[shape & 3];
        const int tiles_y = size_table[shape >> 2];
        const int left = origin_x + signed10(be16(part + 4)) - tiles_x * 8;
        const int top = origin_y + signed10(be16(part + 6)) - tiles_y * 8;
        const bool flip_x = (attributes & 0x1000) != 0;
        const bool flip_y = (attributes & 0x0800) != 0;

        for (int block_y = 0; block_y < tiles_y && success; block_y += 2) {
            for (int block_x = 0; block_x < tiles_x && success; block_x += 2) {
                const int block_tiles_x = SDL_min(2, tiles_x - block_x);
                const int block_tiles_y = SDL_min(2, tiles_y - block_y);
                const int side = block_tiles_x == 2 || block_tiles_y == 2 ? 32 : 16;
                Uint8 tex[1 + 1024] = { 0 };
                tex[0] = (block_tiles_x == 2 ? 0x80 : 0x40) | (block_tiles_y == 2 ? 0x10 : 0x08) | (side == 32 ? 3 : 1);

                for (int y = 0; y < block_tiles_y * 16 && success; y++) {
                    for (int x = 0; x < block_tiles_x * 16; x++) {
                        const int part_x = block_x * 16 + x;
                        const int part_y = block_y * 16 + y;
                        const int source_x = flip_x ? tiles_x * 16 - 1 - part_x : part_x;
                        const int source_y = flip_y ? tiles_y * 16 - 1 - part_y : part_y;
                        const int tile = tile_base + (source_x / 16) * tiles_y + source_y / 16;
                        const size_t pixel = (size_t)tile * 256 + (source_y & 15) * 16 + (source_x & 15);

                        if (pixel >= cram_size) {
                            success = false;
                            break;
                        }

                        tex[1 + swizzle(x, y)] = cram[pixel] & 0x3f;
                    }
                }

                if (!success || *texture_count >= UINT16_MAX || pixels->size > UINT32_MAX) {
                    success = false;
                    break;
                }

                texture_offsets[*texture_count] = (Uint32)pixels->size;
                TileMapEntry placement = { .x = (Sint16)(previous_x - (left + block_x * 16)),
                                           .y = (Sint16)(-top - block_y * 16 - previous_y),
                                           .attr = (flags & ARCADE_CG_PART_PALETTES) ? attributes & 0x1FF : 0,
                                           .code = (Uint16)*texture_count };
                previous_x = left + block_x * 16;
                previous_y = -top - block_y * 16;
                if (flags & ARCADE_CG_COMPRESS) {
                    success = ByteBuffer_Append(pixels, tex, 1) && compress_p6(&tex[1], side * side, pixels);
                } else {
                    success = ByteBuffer_Append(pixels, tex, 1 + side * side);
                }

                success = success && ByteBuffer_Append(&entries, &placement, sizeof(placement));
                *texture_count += 1;
                count += 1;
            }
        }
    }

    if (success && count <= UINT16_MAX) {
        const Uint16 count16 = (Uint16)count;
        success = ByteBuffer_Append(placements, &count16, sizeof(count16)) && ByteBuffer_Append(placements, entries.data, entries.size);
    }

    SDL_free(entries.data);
    SDL_free(cram);
    return success;
}

bool ArcadeCg_Exists(const ArcadeCgSource* source, Uint16 cg) {
    CgDescriptor descriptor;
    return descriptor_for(source, cg, &descriptor);
}

bool ArcadeCg_BuildGroup(
    const ArcadeCgSource* source, const Sint32* cgs, int slot_count, ArcadeCgFlags flags, ArcadeCgGroup* result
) {
    SDL_zero(*result);

    Uint32* texture_offsets = SDL_malloc(CG_COUNT * sizeof(*texture_offsets));
    ByteBuffer placements = { 0 };
    ByteBuffer pixels = { 0 };
    int texture_count = 0;
    bool success = texture_offsets != NULL && ByteBuffer_Reserve(&placements, (size_t)slot_count * 4);

    if (success) {
        SDL_memset(placements.data, 0, (size_t)slot_count * 4);
        placements.size = (size_t)slot_count * 4;
    }

    for (int i = 0; i < slot_count && success; i++) {
        if (placements.size > UINT32_MAX) {
            success = false;
            break;
        }

        ((Uint32*)placements.data)[i] = (Uint32)placements.size;

        if (cgs[i] < 0) {
            const Uint16 empty = 0;
            success = ByteBuffer_Append(&placements, &empty, sizeof(empty));
        } else {
            success = add_cg(source, (Uint16)cgs[i], flags, &placements, &pixels, texture_offsets, &texture_count);
        }
    }

    if (success && pixels.size <= UINT32_MAX - (size_t)texture_count * 4) {
        const size_t header_size = (size_t)texture_count * 4;
        Uint8* table = SDL_malloc(header_size + pixels.size);
        success = table != NULL;

        if (success) {
            for (int i = 0; i < texture_count; i++) {
                ((Uint32*)table)[i] = (Uint32)(header_size + texture_offsets[i]);
            }

            SDL_memcpy(table + header_size, pixels.data, pixels.size);
            result->trans_table = placements.data;
            result->trans_size = placements.size;
            result->texture_table = table;
            result->texture_size = header_size + pixels.size;
            placements.data = NULL;
        }
    } else {
        success = false;
    }

    SDL_free(texture_offsets);
    SDL_free(placements.data);
    SDL_free(pixels.data);
    return success;
}

void ArcadeCg_FreeGroup(ArcadeCgGroup* group) {
    SDL_free(group->trans_table);
    SDL_free(group->texture_table);
    SDL_zero(*group);
}
