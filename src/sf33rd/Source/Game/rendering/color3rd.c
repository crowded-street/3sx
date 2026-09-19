/**
 * @file color3rd.c
 * Loading, conversion, and hardware-upload of color palettes
 */

#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "common.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/cse.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlMemMap.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlTSB.h"
#include "sf33rd/AcrSDK/common/plcommon.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/meta_col.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/system/ramcnt.h"

#include <SDL3/SDL.h>

typedef struct {
    u16 col[2][28][64];
} COL;

typedef struct {
    u16 data;
    u16 type;
    u16 apfn;
    u16 free;
} col_file_data;

typedef struct {
    u16 col[2][16][64];
} COL_x1000;

typedef struct {
    u16 col[64];
} COL_x80;

typedef struct {
    u16 col[3][64];
} COL_x180;

typedef struct {
    u16 col[2][64];
} COL_x100;

typedef struct {
    u16 col[20][16][16];
} COL_x2800;

u16 colPalBuffDC[1024];
u16 ColorRAM[512][64];
Col3rd_W col3rd_w;
COL* plcol[2];
PixelFormat palFormRam;
PixelFormat palFormSrc;
s32 palFormConv;

// forward decls
const u16 hitmark_color[128];
const col_file_data color_file[161];

// State 0: skip the read entirely when the sound bank this request names is
// already the one loaded. Returns 1 where the original fell through into state
// 1, and 0 where it broke out of the dispatch.
static s32 begin_color_file_request(LoadRequest* curr, col_file_data* cfn) {
    if (fsCheckCommandExecuting()) {
        return 0;
    }

    if (cfn->type == 10) {
        if (cfn->data + 1 == cseGetIdStoredBd(curr->id + 1)) {
            LDREQ_SetResultFlag(curr, true);
            curr->status = LDREQ_STATUS_FREE;
            return 0;
        }
    }

    curr->rno = 1;
    curr->fnum = cfn->apfn;

    if (cfn->apfn == 0xFFFF) {
        LDREQ_SetResultFlag(curr, true);
        curr->status = LDREQ_STATUS_FREE;
    }

    return 1;
}

static void request_color_file_read(LoadRequest* curr) {
    s32 err;

    err = fsRequestFileRead(Get_ramcnt_pointer(curr->key));

    if (err == 0) {
        Push_ramcnt_key(curr->key);
        fsClose();
        curr->rno = 0;
    } else {
        curr->rno = 4;
        curr->status = LDREQ_STATUS_RUNNING;
    }
}

// State 4: poll the read. A sound bank goes to the SPU and waits in state 5;
// anything else is a colour file and is converted here and then.
static void collect_color_file_read(LoadRequest* curr, col_file_data* cfn) {
    switch (fsCheckFileReaded()) {
    case FS_READ_IDLE:
        if (cfn->type == 10) {
            fsClose();

            cseSendBd2SpuWithId(
                Get_ramcnt_pointer(curr->key),
                Get_size_data_ramcnt_key(curr->key),
                curr->id + 1,
                cfn->data + 1
            );

            curr->rno = 5;
        } else {
            init_trans_color_ram(curr->id, curr->key, cfn->type, cfn->data);
            fsClose();
            LDREQ_SetResultFlag(curr, true);
            curr->status = LDREQ_STATUS_FREE;
        }

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
}

// The two states after the read has been asked for. Reached from
// q_ldreq_color_data's default, with the original state numbers kept; nothing
// falls into state 4, because state 3 ends in a break.
static void q_ldreq_color_data_read(LoadRequest* curr, col_file_data* cfn) {
    switch (curr->rno) {
    case 4:
        collect_color_file_read(curr, cfn);
        break;

    case 5:
        Push_ramcnt_key(curr->key);
        cseMemMapSetPhdAddr(curr->id + 1, csePHDDataTable[cfn->data + 1]);
        cseTsbSetBankAddr(curr->id + 1, cseTSBDataTable[cfn->data + 1]);
        sdbd[curr->id + 1] = (s8*)cseTSBDataTable[cfn->data + 1];
        LDREQ_SetResultFlag(curr, true);
        curr->status = LDREQ_STATUS_FREE;
        break;
    }
}

void q_ldreq_color_data(LoadRequest* curr) {
    col_file_data* cfn;

    cfn = (col_file_data*)&color_file[curr->ix];

    switch (curr->rno) {
    case 0:
        if (!begin_color_file_request(curr, cfn)) {
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
        Set_size_data_ramcnt_key(curr->key, curr->size);
        curr->rno = 3;
        /* fallthrough */

    case 3:
        request_color_file_read(curr);
        break;

    default:
        q_ldreq_color_data_read(curr, cfn);
        break;
    }
}

void load_any_color(u16 ix, u8 kokey) {
    col_file_data* cfn;
    s16 key;

    cfn = (col_file_data*)&color_file[ix];
    key = load_it_use_any_key(cfn->apfn, kokey, 0);

    if (key) {
        init_trans_color_ram(0, key, cfn->type, cfn->data);
    }
}

void set_hitmark_color() {
    s16 i;

    for (i = 0; i < 64; i++) {
        ColorRAM[7][i] = ColorRAM[15][i] = palConvSrcToRam(hitmark_color[i]);
        ColorRAM[23][i] = ColorRAM[31][i] = palConvSrcToRam(hitmark_color[i + 64]);
    }

    njSetPaletteData(64, 64, ColorRAM[15]);
    njSetPaletteData(576, 64, ColorRAM[31]);
    palUpdateGhostCP3(7, 1);
    palUpdateGhostCP3(15, 1);
    palUpdateGhostCP3(23, 1);
    palUpdateGhostCP3(31, 1);
}

// The default character's file carries a page per shade, so each is converted
// into its own slot and the six extra pages follow behind them.
static void spread_player_shading_pages(s16 id) {
    s16 i;
    s16 j;

    for (i = 0; i < 64; i++) {
        ColorRAM[id * 16][i] = palConvSrcToRam(plcol[id]->col[0][Player_Color[id]][i]);
        ColorRAM[(id * 16) + 8][i] = palConvSrcToRam(plcol[id]->col[1][Player_Color[id]][i]);
    }

    for (i = 0; i < 6; i++) {
        for (j = 0; j < 64; j++) {
            ColorRAM[i + ((id * 16) + 1)][j] = palConvSrcToRam(plcol[id]->col[0][i + 16][j]);
            ColorRAM[i + ((id * 16) + 9)][j] = palConvSrcToRam(plcol[id]->col[1][i + 16][j]);
        }
    }
}

// Any other character has one bank, written to both the slot and its +512
// mirror in one pass.
static void mirror_player_shading_pages(s16 id) {
    u16* ldadrs;
    u16* tradrs;
    s16 i;

    tradrs = (u16*)plcol[id]->col[0][Player_Color[id]];
    ldadrs = (u16*)ColorRAM[id * 16];
    for (i = 0; i < 64; i++) {
        ldadrs[i] = ldadrs[i + 512] = palConvSrcToRam(tradrs[i]);
    }
    ldadrs += 64;
    tradrs = (u16*)plcol[id]->col[0][16];
    for (i = 0; i < 384; i++) {
        ldadrs[i] = ldadrs[i + 512] = palConvSrcToRam(tradrs[i]);
    }
}

// Type 1 is a player's own colour file: the two shading banks, the six extra
// pages behind them, and the 256-entry tail that goes to a fixed page per side.
static void load_player_color_file(s16 id, s16 key) {
    u16* ldadrs;
    u16* tradrs;
    s16 i;

    plcol[id] = Get_ramcnt_pointer(key);
    if (My_char[id] == 0) {
        spread_player_shading_pages(id);
    } else {
        mirror_player_shading_pages(id);
    }

    tradrs = plcol[id]->col[0][22];
    if (id) {
        ldadrs = ColorRAM[506];
    } else {
        ldadrs = ColorRAM[502];
    }

    for (i = 0; i < 256; i++) {
        ldadrs[i] = palConvSrcToRam(tradrs[i]);
    }

    Push_ramcnt_key(key);
    palUpdateGhostCP3(id * 16, 16);

    if (id) {
        palUpdateGhostCP3(506, 4);
    } else {
        palUpdateGhostCP3(502, 4);
    }
}

// Type 2 is a flat run of entries dropped straight onto a page, with page 32
// also seeding the transparent-mask page behind it.
static void load_flat_color_file(s16 key, u16 data) {
    u16* ldadrs;
    u16* tradrs;
    s16 i;
    s32 size;

    size = Get_size_data_ramcnt_key(key);
    size = size / 2;
    tradrs = Get_ramcnt_pointer(key);
    ldadrs = (u16*)&ColorRAM[data];

    for (i = 0; i < size; i++) {
        ldadrs[i] = palConvSrcToRam(tradrs[i]);
    }

    Push_ramcnt_key(key);
    if (data == 32) {
        ColorRAM[511][0] = 0;
        for (i = 1; i < 64; i++) {
            ColorRAM[511][i] = 0x8000;
        }
    }

    palUpdateGhostCP3(data, size / 64);
}

// id 2 means both players at once, each taking both of its banks from source
// bank 0 at its own colour.
static void copy_both_metamor_banks(COL_x1000* dadr) {
    s16 i;

    for (i = 0; i < 64; i++) {
        hi_meta[0][0][i] = dadr->col[0][Player_Color[0]][i];
        hi_meta[0][1][i] = dadr->col[0][Player_Color[0]][i];
        hi_meta[1][0][i] = dadr->col[0][Player_Color[1]][i];
        hi_meta[1][1][i] = dadr->col[0][Player_Color[1]][i];
    }
}

// Both arms of the one-player case fill the same two metamorphosis banks and
// differ in a single value: which source bank the second one is taken from.
static void copy_metamor_banks(s16 id, COL_x1000* dadr, s16 second_bank) {
    s16 i;

    for (i = 0; i < 64; i++) {
        hi_meta[id][0][i] = dadr->col[0][Player_Color[id]][i];
        hi_meta[id][1][i] = dadr->col[second_bank][Player_Color[id]][i];
    }
}

// Type 3 fills the metamorphosis source banks. id 2 means both players at once
// off player 0's bank; otherwise the second bank is taken only when the other
// player is on the default character.
static void load_metamor_color_file(s16 id, s16 key) {
    s16 i;

    COL_x1000* dadr = Get_ramcnt_pointer(key);
    if (id == 2) {
        copy_both_metamor_banks(dadr);
        metamor_color_store(0);
        metamor_color_store(1);
    } else {
        if ((My_char[(id + 1) & 1]) == 0) {
            copy_metamor_banks(id, dadr, 1);
        } else {
            copy_metamor_banks(id, dadr, 0);
        }

        metamor_color_store(id);
    }
    Push_ramcnt_key(key);
}

// Type 7 takes one 16-entry row per player, each indexed by that player's
// character and colour, onto the adjacent pages 40 and 41.
static void load_both_players_meter_color(s16 key) {
    u16* ldadrs;
    u16* tradrs;
    s16 i;

    COL_x2800* adrs = Get_ramcnt_pointer(key);
    ldadrs = (u16*)&ColorRAM[40];
    tradrs = (u16*)&ColorRAM[41];

    for (i = 0; i < 16; i++) {
        ldadrs[i] = palConvSrcToRam(adrs->col[My_char[0]][Player_Color[0]][i]);
        tradrs[i] = palConvSrcToRam(adrs->col[My_char[1]][Player_Color[1]][i]);
    }

    Push_ramcnt_key(key);
    palUpdateGhostCP3(40, 2);
}

// Types 4, 5 and 6 all fill a colour page and its +8 mirror from the same
// source run, differing only in how many entries that is. The count stays
// unsigned at each call site so the loop comparison promotes as it did.
static void copy_palette_pair(s16 id, u16 data, const u16* src, u32 count) {
    s16 i;
    u16* dst = (u16*)&ColorRAM[data + (id * 16)][0];

    for (i = 0; i < count; i++) {
        dst[i] = palConvSrcToRam(src[i]);
    }

    dst = (u16*)&ColorRAM[data + (id * 16) + 8][0];

    for (i = 0; i < count; i++) {
        dst[i] = palConvSrcToRam(src[i]);
    }
}

// The meter row, the two sound-bank uploads, and the three types that do
// nothing. Reached from init_trans_color_ram's default so the labels below are
// the original numbers - a type named in neither switch still does nothing, as
// it did when there was no default at all.
static void init_trans_color_ram_late(s16 id, s16 key, u8 type, u16 data) {
    switch (type) {
    case 7:
        load_both_players_meter_color(key);
        break;

    case 8:
        cseSendBd2SpuWithId(Get_ramcnt_pointer(key), Get_size_data_ramcnt_key(key), 0, 0);
        Push_ramcnt_key(key);
        break;

    case 10:
        cseSendBd2SpuWithId(Get_ramcnt_pointer(key), Get_size_data_ramcnt_key(key), id + 1, data + 1);
        cseMemMapSetPhdAddr(id + 1, csePHDDataTable[data + 1]);
        cseTsbSetBankAddr(id + 1, cseTSBDataTable[data + 1]);
        sdbd[id + 1] = (s8*)cseTSBDataTable[data + 1];
        Push_ramcnt_key(key);
        break;

    case 0xb:
    case 0xc:
    case 0x61:
        break;
    }
}

void init_trans_color_ram(s16 id, s16 key, u8 type, u16 data) {
    switch (type) {
    case 1:
        load_player_color_file(id, key);
        break;

    case 2:
        load_flat_color_file(key, data);
        break;
    case 3:
        load_metamor_color_file(id, key);
        break;

    case 4: {
        COL_x80* adr = Get_ramcnt_pointer(key);
        u16* src = (&adr[Player_Color[id]])->col;
        // these unsigned constants are here intentionally, otherwise wouldn't match.
        copy_palette_pair(id, data, src, 64U);

        Push_ramcnt_key(key);
        palUpdateGhostCP3(data + (id * 16), 1);
        palUpdateGhostCP3(data + ((id * 16) + 8), 1);
        break;
    }
    case 5: {
        COL_x180* adr = Get_ramcnt_pointer(key);
        u16* src = (&adr[Player_Color[id]])->col[0];
        copy_palette_pair(id, data, src, 192U);

        Push_ramcnt_key(key);
        palUpdateGhostCP3((data) + (id * 16), 3);
        palUpdateGhostCP3((data) + ((id * 16) + 8), 3);
        break;
    }
    case 6: {
        COL_x100* adr = Get_ramcnt_pointer(key);
        u16* src = (&adr[Player_Color[id]])->col[0];
        copy_palette_pair(id, data, src, 128U);
        Push_ramcnt_key(key);
        palUpdateGhostCP3(data + (id * 16), 2);
        palUpdateGhostCP3((data) + ((id * 16) + 8), 2);
        break;
    }
    default:
        init_trans_color_ram_late(id, key, type, data);
        break;
    }
}

void init_color_trans_req() {
    s16 i;

    for (i = 0; i < 32; i++) {
        col3rd_w.req[i][0] = col3rd_w.req[i][1] = 0;
    }

    col3rd_w.reqNum = 0;
}

void push_color_trans_req(s16 from_col, s16 to_col) {
    palCopyGhostDC(to_col << 6, 64, ColorRAM[from_col]);
    palUpdateGhostDC();
}

void palCopyGhostDC(s32 ofs, s32 cnt, void* data) {
    s32 i;
    u16* srcAdrs = data;
    u16* dstAdrs = &colPalBuffDC[ofs];

    for (i = 0; i < cnt; i++) {
        *dstAdrs++ = *srcAdrs++;
    }

    col3rd_w.upBits = col3rd_w.upBits | (1 << (ofs / 64));
}

u16 palConvSrcToRam(u16 col) {
    u8 cA;
    u8 cR;
    u8 cG;
    u8 cB;

    if (palFormConv == 0) {
        return col;
    }

    cA = palFormSrc.am & (col >> palFormSrc.as);
    cR = palFormSrc.rm & (col >> palFormSrc.rs);
    cG = palFormSrc.gm & (col >> palFormSrc.gs);
    cB = palFormSrc.bm & (col >> palFormSrc.bs);
    return (cA << palFormRam.as) | (cR << palFormRam.rs) | (cG << palFormRam.gs) | (cB << palFormRam.bs);
}

void palCreateGhost() {
    PPLFileHeader ppl;
    s32 key;
    s32 size;
    s32 i;
    u8* adrs;

    palFormConv = 0;
    palFormSrc.rl = 5;
    palFormSrc.rs = 10;
    palFormSrc.rm = 31;
    palFormSrc.gl = 5;
    palFormSrc.gs = 5;
    palFormSrc.gm = 31;
    palFormSrc.bl = 5;
    palFormSrc.bs = 0;
    palFormSrc.bm = 31;
    palFormSrc.al = 1;
    palFormSrc.as = 15;
    palFormSrc.am = 1;
    palFormRam.rl = 5;
    palFormRam.rs = 10;
    palFormRam.rm = 31;
    palFormRam.gl = 5;
    palFormRam.gs = 5;
    palFormRam.gm = 31;
    palFormRam.bl = 5;
    palFormRam.bs = 0;
    palFormRam.bm = 31;
    palFormRam.al = 1;
    palFormRam.as = 15;
    palFormRam.am = 1;
    palFormRam.rs = 0;
    palFormRam.bs = 10;
    palFormRam.gl = 5;
    palFormRam.gm = 31;
    palFormConv = 1;
    col3rd_w.upBits = 0;
    ppl.magic = 0;
    ppl.fileSize = 0;
    ppl.free = 0;
    ppl.compress = 0;
    ppl.c_mode = 2;
    ppl.formARGB = 0x5515;

    ppl.palettes = 0x1000;
    size = 0x2000;
    key = Pull_ramcnt_key(size, 2, 0, 1);
    adrs = Get_ramcnt_pointer(key);

    for (i = 0; i < size; i++) {
        adrs[i] = 0;
    }

    ppgSetupPalChunkDir(&col3rd_w.palDC, &(PPGPalChunkDirArgs){&ppl, adrs, 0, 1});
    Push_ramcnt_key(key);

    ppl.palettes = 2;
    size = 0x2000;
    key = Pull_ramcnt_key(size, 2, 0, 1);
    adrs = Get_ramcnt_pointer(key);

    for (i = 0; i < size; i++) {
        adrs[i] = 0;
    }

    ppgSetupPalChunkDir(&col3rd_w.palCP3, &(PPGPalChunkDirArgs){&ppl, adrs, 0, 1});
    Push_ramcnt_key(key);
}

Palette* palGetChunkGhostDC() {
    return &col3rd_w.palDC;
}

Palette* palGetChunkGhostCP3() {
    return &col3rd_w.palCP3;
}

void palUpdateGhostDC() {
    plContext bits;
    s32 i;
    u16* srcAdrs;
    u16* dstAdrs;

    for (i = 0; i < col3rd_w.palDC.total; i++) {
        if (col3rd_w.upBits & (1 << i)) {
            flLockPalette(NULL, col3rd_w.palDC.handle[i], &bits, 2);
            dstAdrs = bits.ptr;
            srcAdrs = &colPalBuffDC[i << 6];
            SDL_memcpy(dstAdrs, srcAdrs, 0x80);
            flUnlockPalette(col3rd_w.palDC.handle[i]);
        }
    }

    col3rd_w.upBits = 0;
}

void palUpdateGhostCP3(s32 pal, s32 nums) {
    plContext bits;
    s32 i;
    u16* srcAdrs;
    u16* dstAdrs;

    for (i = pal; i < (pal + nums); i++) {
        flLockPalette(NULL, col3rd_w.palCP3.handle[i], &bits, 2);
        dstAdrs = bits.ptr;
        srcAdrs = (u16*)&ColorRAM[i];
        SDL_memcpy(dstAdrs, srcAdrs, 0x80);
        flUnlockPalette(col3rd_w.palCP3.handle[i]);
    }
}

const u16 hitmark_color[128] = {
    0,     64478, 64408, 64338, 64268, 64200, 64068, 63942, 63808, 58332, 52186, 54104, 51988, 47888, 41740, 49424,
    64478, 58270, 54174, 49950, 45662, 43550, 43486, 41310, 64478, 64346, 64214, 64148, 64016, 63884, 63752, 61506,
    36944, 64478, 64470, 64402, 64332, 64200, 63940, 61568, 55296, 32768, 32768, 32768, 32768, 32768, 32768, 32768,
    64478, 62302, 60190, 60124, 58012, 57882, 55770, 53658, 64478, 62302, 60190, 60124, 58012, 57882, 55770, 53658,
    0,     64478, 64472, 60298, 54148, 50054, 45896, 39690, 37512, 58332, 52186, 54104, 51988, 47888, 41740, 49424,
    64478, 62302, 60190, 60124, 58012, 57882, 55770, 53658, 64478, 64346, 64214, 64148, 64016, 63884, 63752, 61506,
    36944, 64478, 64470, 64402, 64332, 64200, 63940, 61568, 55296, 32768, 32768, 32768, 32768, 32768, 32768, 32768,
    64478, 58270, 54174, 49950, 45662, 43550, 43486, 41310, 64478, 58206, 56094, 53982, 53918, 51806, 49694, 47582
};

const col_file_data color_file[161] = { { .data = 0x0, .type = 0x1, .apfn = 0x5B7, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5BA, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5BE, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5C1, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5C5, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5C8, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5CC, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5D1, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5D5, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5D9, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5DC, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5E0, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5E4, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5E8, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5EC, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5F0, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5F3, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5F6, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5F9, .free = 0x0 },
                                        { .data = 0x0, .type = 0x1, .apfn = 0x5FD, .free = 0x0 },
                                        { .data = 0x20, .type = 0x2, .apfn = 0x9, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x567, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x56C, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x56F, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x572, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x575, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x578, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x57B, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x57E, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x581, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x584, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x587, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x58A, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x590, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x593, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x596, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x599, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x59C, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x59F, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x5A2, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x5A7, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x56B, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x56E, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x571, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x574, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x577, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x57A, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x57C, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x580, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x583, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x586, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x589, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x58C, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x58F, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x592, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x595, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x598, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x59B, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x59B, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x59E, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x5A1, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x5A5, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x5AA, .free = 0x0 },
                                        { .data = 0x0, .type = 0x7, .apfn = 0x5AB, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x553, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x554, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x555, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x556, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x557, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x558, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x559, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x55A, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x55B, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x55C, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x55D, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x55E, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x55F, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x560, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x561, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x562, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x563, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x564, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x565, .free = 0x0 },
                                        { .data = 0x0, .type = 0x3, .apfn = 0x566, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x4D, .free = 0x0 },
                                        { .data = 0x6, .type = 0x4, .apfn = 0x5C2, .free = 0x0 },
                                        { .data = 0x4, .type = 0x5, .apfn = 0x5C9, .free = 0x0 },
                                        { .data = 0x5, .type = 0x6, .apfn = 0x5D2, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x58D, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0xE, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0xF, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x10, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x11, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x12, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x13, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x14, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x15, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x16, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x17, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x18, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x19, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x1A, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x1B, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x1C, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x1D, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x1E, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x1F, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x20, .free = 0x0 },
                                        { .data = 0x12C, .type = 0x2, .apfn = 0x21, .free = 0x0 },
                                        { .data = 0x0, .type = 0x8, .apfn = 0x7, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0xA, .apfn = 0x5B8, .free = 0x0 },
                                        { .data = 0x1, .type = 0xA, .apfn = 0x5BB, .free = 0x0 },
                                        { .data = 0x2, .type = 0xA, .apfn = 0x5BF, .free = 0x0 },
                                        { .data = 0x3, .type = 0xA, .apfn = 0x5C3, .free = 0x0 },
                                        { .data = 0x4, .type = 0xA, .apfn = 0x5C6, .free = 0x0 },
                                        { .data = 0x5, .type = 0xA, .apfn = 0x5CA, .free = 0x0 },
                                        { .data = 0x6, .type = 0xA, .apfn = 0x5CE, .free = 0x0 },
                                        { .data = 0x7, .type = 0xA, .apfn = 0x5D3, .free = 0x0 },
                                        { .data = 0x8, .type = 0xA, .apfn = 0x5D6, .free = 0x0 },
                                        { .data = 0x9, .type = 0xA, .apfn = 0x5DA, .free = 0x0 },
                                        { .data = 0xA, .type = 0xA, .apfn = 0x5DD, .free = 0x0 },
                                        { .data = 0xB, .type = 0xA, .apfn = 0x5E1, .free = 0x0 },
                                        { .data = 0xC, .type = 0xA, .apfn = 0x5E5, .free = 0x0 },
                                        { .data = 0xD, .type = 0xA, .apfn = 0x5E9, .free = 0x0 },
                                        { .data = 0xE, .type = 0xA, .apfn = 0x5ED, .free = 0x0 },
                                        { .data = 0xF, .type = 0xA, .apfn = 0x5F1, .free = 0x0 },
                                        { .data = 0x10, .type = 0xA, .apfn = 0x5F4, .free = 0x0 },
                                        { .data = 0x11, .type = 0xA, .apfn = 0x5F7, .free = 0x0 },
                                        { .data = 0x12, .type = 0xA, .apfn = 0x5FA, .free = 0x0 },
                                        { .data = 0x13, .type = 0xA, .apfn = 0x5FE, .free = 0x0 },
                                        { .data = 0x0, .type = 0xC, .apfn = 0x548, .free = 0x0 },
                                        { .data = 0x0, .type = 0x9, .apfn = 0xFFFF, .free = 0x0 },
                                        { .data = 0x0, .type = 0x62, .apfn = 0x50, .free = 0x0 },
                                        { .data = 0x0, .type = 0xB, .apfn = 0x542, .free = 0x0 },
                                        { .data = 0x0, .type = 0x63, .apfn = 0x4C, .free = 0x0 },
                                        { .data = 0x4, .type = 0x4, .apfn = 0x5CD, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0xB, .free = 0x0 },
                                        { .data = 0x0, .type = 0x0, .apfn = 0x8, .free = 0x0 },
                                        { .data = 0x0, .type = 0x63, .apfn = 0x53, .free = 0x0 },
                                        { .data = 0x0, .type = 0x63, .apfn = 0x54, .free = 0x0 },
                                        { .data = 0x0, .type = 0x63, .apfn = 0x55, .free = 0x0 } };
