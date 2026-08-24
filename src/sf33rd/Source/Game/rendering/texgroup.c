/**
 * @file texgroup.c
 * Texture Group Manager and Loader
 */

#include "sf33rd/Source/Game/rendering/texgroup.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "main.h"
#include "port/config/config.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Game/engine/charid.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/rendering/chren3rd.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/rendering/texgroup_data.h"
#include "sf33rd/Source/Game/system/ramcnt.h"
#include "structs.h"

#if ARCADE_ROM
#include "arcade/arcade_char_data.h"
#endif

#include <SDL3/SDL.h>

#include <stdlib.h>

typedef struct {
    s16 x;
    s16 y;
    u16 attr;
    u16 code;
} TexGroup_UNK_0;

u8 omSelObjNowOnMemoryType = 0xFF;
TEX_GRP_LD texgrplds[100];

// forward decls
s32 load_any_texture_grpnum(u8 grp, u8 kokey);

void q_ldreq_texture_group(LoadRequest* curr) {
    const TexGroupData* bsd = &texgrpdat[curr->ix];

    switch (curr->rno) {
    case 0:
        if (fsCheckCommandExecuting()) {
            break;
        }

        curr->rno = 1;
        curr->fnum = bsd->apfn;

        if (bsd->apfn == -1) {
            LDREQ_SetResultFlag(curr, true);
            curr->status = LDREQ_STATUS_FREE;
        }

        if (bsd->num_of_1st == 0) {
            curr->group = obj_group_table[bsd->num_of_1st + 1];
        } else {
            curr->group = obj_group_table[bsd->num_of_1st];
        }

        curr->lds = &texgrplds[curr->group];

        if (curr->lds->ok) {
            switch (bsd->mode) {
            case TEXGROUP_MODE_CHARACTER:
            case TEXGROUP_MODE_SHARED:
                switch (rckey_work[curr->lds->key].type) {
                case 3:
                    if (curr->id) {
                        rckey_work[curr->lds->key].type = 5;
                    }

                    break;

                case 4:
                    if (curr->id == 0) {
                        rckey_work[curr->lds->key].type = 5;
                    }

                    break;

                case 5:
                    break;
                }

                if (rckey_work[curr->lds->key].type == 5) {
                    LDREQ_SetResultFlag(curr, true);
                    curr->status = LDREQ_STATUS_FREE;
                } else {
                    fatal_error("A duplicate transfer occurred. File number: %d", bsd->apfn);
                }
                
                break;

            case TEXGROUP_MODE_NORMAL:
                rckey_work[curr->lds->key].type = curr->kokey;
                LDREQ_SetResultFlag(curr, true);
                curr->status = LDREQ_STATUS_FREE;
                break;
            }

            break;
        }

        /* fallthrough */

    case 1:
        if (!fsOpen(curr->fnum)) {
            curr->rno = 0;
            break;
        }

        curr->rno = 2;
        /* fallthrough */

    case 2:
        curr->size = fsGetFileSize(curr->fnum);
        curr->key = Pull_ramcnt_key(curr->size, curr->kokey, curr->group, curr->frre);
        curr->lds->key = curr->key;
        Set_size_data_ramcnt_key(curr->key, curr->size);
        curr->rno = 3;
        /* fallthrough */

    case 3:
        const s32 err = fsRequestFileRead(Get_ramcnt_pointer(curr->key));

        if (err == 0) {
            Push_ramcnt_key(curr->key);
            fsClose();
            curr->rno = 0;
            return;
        }

        curr->rno = 4;
        curr->status = LDREQ_STATUS_RUNNING;
        break;

    case 4:
        switch (fsCheckFileReaded()) {
        case FS_READ_IDLE:
            fsClose();
            u8* ldadr = Get_ramcnt_pointer(curr->key);
            curr->lds->texture_table = ldadr + bsd->to_tex;
            curr->lds->trans_table = ldadr;
            curr->lds->ok = 1;

            if (bsd->mode == TEXGROUP_MODE_CHARACTER) {
                u8* ldchd = ldadr + bsd->to_chd;

                // Explanation:
                //
                // The code above loads a bunch of data from the AFS partition.
                // This data includes character init data which starts at `ldchd`.
                // Data at `ldchd` starts with 25 4-byte ints which are offsets
                // from `ldchd` to the actual data.
                //
                // On PS2 it is okay to just add `ldchd` to each of these offsets
                // to turn them into pointers, because a 4-byte int can hold a pointer.
                // However on modern 64-bit platforms pointers are bigger, meaning we
                // can't add `ldchd` to the offsets inplace. That's why we have to
                // allocate a separate memory region for `cit` and compute the pointers
                // that comprise it there.
                //
                // Because 25 is the number of members in CharInitData struct, `i` goes
                // to 25 too.

                const Character character_id = plt_req[curr->id];
                CharInitData* dst = &char_init_data[plid_data[character_id]];

                if (ArcadeBalance_IsEnabled()) {
#if ARCADE_ROM
                    const size_t ps2_char_data_size = curr->size - bsd->to_chd;
                    const bool adapted =
                        ArcadeCharData_Apply3SXRenderingConventions(character_id, ldchd, ps2_char_data_size);
                    const CharInitData* arcade_data = ArcadeCharData_Get(character_id);

                    SDL_assert(adapted && arcade_data != NULL);

                    if (!adapted || arcade_data == NULL) {
                        SDL_LogCritical(
                            SDL_LOG_CATEGORY_APPLICATION,
                            "Could not adapt arcade character data for character %d",
                            character_id
                        );
                        return;
                    }

                    SDL_copyp(dst, arcade_data);
#endif
                } else {
                    for (int i = 0; i < 25; i++) {
                        ((uintptr_t*)dst)[i] = (uintptr_t)ldchd + ((u32*)ldchd)[i];
                    }

                    // Q specific code
                    if (curr->ix == 18) {
                        dst->cbca[37] = dst->cbca[3];
                    }

                    // Akuma specific code
                    if (curr->ix == 15) {
                        u16* trsbas = (u16*)(((u32*)texgrplds[15].trans_table)[166] + texgrplds[15].trans_table);
                        const int count = *trsbas - 1;
                        *trsbas = count;
                        trsbas += 1;

                        TexGroup_UNK_0* trsptr = (TexGroup_UNK_0*)trsbas;
                        trsptr[0].x += trsptr[1].x;
                        trsptr[0].y += trsptr[1].y;
                        trsptr[0].attr = trsptr[1].attr;
                        trsptr[0].code = trsptr[1].code;

                        for (int i = 1; i < count; i++) {
                            trsptr[i] = trsptr[i + 1];
                        }
                    }
                }

                parabora_own_table[character_id] = dst->prot;
            }

            LDREQ_SetResultFlag(curr, true);
            curr->status = LDREQ_STATUS_FREE;
            break;

        case FS_READ_READING:
            // Do nothing
            break;

        case FS_READ_ERROR:
            Push_ramcnt_key(curr->key);
            fsClose();
            curr->status = LDREQ_STATUS_IDLE;
            curr->rno = 0;
            break;
        }

        break;
    }
}

void Init_texgrplds_work() {
    SDL_zeroa(texgrplds);
}

void reservMemKeySelObj() {
    const int size = 1128236;
    TEX_GRP_LD* lds = &texgrplds[obj_group_table[27104]];
    lds->key = Pull_ramcnt_key(size, 13, 0, 1);

    if (lds->key < 0) {
        fatal_error("Failed to pull ramcnt key");
    }
}

void checkSelObjFileLoaded() {
    if (omSelObjNowOnMemoryType == mpp_w.language) {
        return;
    }

    const TexGroupData* bsd;

    if (mpp_w.language == LANG_JAPANESE) {
        bsd = &texgrpdat[98]; // ef02.bin
    } else {
        bsd = &texgrpdat[23]; // ef02_usa.bin
    }

    TEX_GRP_LD* lds = &texgrplds[obj_group_table[27104]];

    while (1) {
        const bool success = load_it_use_this_key(bsd->apfn, lds->key);

        if (success) {
            break;
        }
    }

    void* ldadr = Get_ramcnt_pointer(lds->key);
    lds->texture_table = (u8*)ldadr + bsd->to_tex;
    lds->trans_table = ldadr;
    lds->ok = 1;
    omSelObjNowOnMemoryType = mpp_w.language;
    Clear_texcash_work();
}

void purge_texture_group_of_this(u16 patnum) {
    purge_texture_group(obj_group_table[patnum]);
}

void purge_texture_group(u8 grp) {
    if (texgrplds[grp].ok != 0) {
        texgrplds[grp].ok = 0;
        Push_ramcnt_key(texgrplds[grp].key);
    }
}

void purge_player_texture(s16 id) {
    s16 emid;
    s16 pkey;

    emid = (id + 1) & 1;

    if ((pkey = Search_ramcnt_type(lpt_seldat[2])) != 0) {
        while (1) {
            rckey_work[pkey].type = lpt_seldat[emid];

            if (!(pkey = Search_ramcnt_type(lpt_seldat[2]))) {
                break;
            }
        }
    }

    while (1) {
        pkey = Search_ramcnt_type(lpt_seldat[id]);

        if (pkey == 0) {
            break;
        }

        Push_ramcnt_key(pkey);
    }
}

s32 load_any_texture_patnum(u16 patnum, u8 kokey, u8 _unused) {
    return load_any_texture_grpnum(obj_group_table[patnum], kokey);
}

s32 load_any_texture_grpnum(u8 grp, u8 kokey) {
    if (grp == 0) {
        return 0;
    }

    TEX_GRP_LD* lds = &texgrplds[grp];
    const TexGroupData* bsd = &texgrpdat[grp];

    if (lds->ok) {
        return 0;
    }

    lds->key = load_it_use_any_key(bsd->apfn, kokey, grp);
    void* ldadr = Get_ramcnt_pointer(lds->key);
    lds->texture_table = (u8*)ldadr + bsd->to_tex;
    lds->trans_table = ldadr;
    lds->ok = 1;
    return 1;
}
