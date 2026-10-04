#ifndef ARCADE_CG_H
#define ARCADE_CG_H

#include <SDL3/SDL.h>

#include <stdbool.h>
#include <stddef.h>

/// Where a CPS3 ROM set keeps its CGs (sprite frames)
typedef struct ArcadeCgSource {
    const Uint8* program; // Big-endian SH-2 words
    size_t program_size;
    Uint32 program_base;   // SH-2 address of `program[0]`
    Uint32 table;          // SH-2 address of the CG table, whose entries end with the descriptor address
    Uint32 entry_size;     // Size of a CG table entry
    Uint32 origin_offset;  // Offset of the s16 origin x within an entry. Origin y follows it.
    const Uint8* graphics; // Graphics region, laid out as `Rom_GetGraphics` returns it
    size_t graphics_size;
} ArcadeCgSource;

/// The two tables of a texture group that mtrans.c consumes
typedef struct ArcadeCgGroup {
    void* trans_table;
    size_t trans_size;
    void* texture_table;
    size_t texture_size;
} ArcadeCgGroup;

typedef enum ArcadeCgFlags {
    /// Keep the palette of each CG part in its placement. Stage objects need it. Characters draw every part with the
    /// character's palette.
    ARCADE_CG_PART_PALETTES = 1 << 0,

    /// Compress textures the way PS2 files do. Uncompressed textures are valid too, but take 4–10 times the memory.
    ARCADE_CG_COMPRESS = 1 << 1,
} ArcadeCgFlags;

bool ArcadeCg_Exists(const ArcadeCgSource* source, Uint16 cg);

/// Builds a texture group from CGs.
/// @param cgs CG for each slot of the group, or -1 to leave the slot empty.
/// @return `false` on failure. Free the result with `ArcadeCg_FreeGroup`.
bool ArcadeCg_BuildGroup(
    const ArcadeCgSource* source, const Sint32* cgs, int slot_count, ArcadeCgFlags flags, ArcadeCgGroup* result
);

void ArcadeCg_FreeGroup(ArcadeCgGroup* group);

#endif
