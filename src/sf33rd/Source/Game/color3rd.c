#include "sf33rd/Source/Game/color3rd.h"
#include "common.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/cse.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlMemMap.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlTSB.h"
#include "sf33rd/AcrSDK/common/plcommon.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Game/DC_Ghost.h"
#include "sf33rd/Source/Game/RAMCNT.h"
#include "sf33rd/Source/Game/Sound3rd.h"
#include "sf33rd/Source/Game/meta_col.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/io/gd3rd.h"

typedef struct {
    // total size: 0x1C00
    u16 col[2][28][64]; // offset 0x0, size 0x1C00
} COL;

typedef struct {
    // total size: 0x8
    u16 data; // offset 0x0, size 0x2
    u16 type; // offset 0x2, size 0x2
    u16 apfn; // offset 0x4, size 0x2
    u16 free; // offset 0x6, size 0x2
} col_file_data;

typedef struct {
    // total size: 0x1000
    u16 col[2][16][64]; // offset 0x0, size 0x1000
} COL_x1000;

typedef struct {
    u16 col[64]; // offset 0x0, size 0x80
} COL_x80;

typedef struct {
    u16 col[3][64]; // offset 0x0, size 0x180
} COL_x180;

typedef struct {
    u16 col[2][64]; // offset 0x0, size 0x100
} COL_x100;

typedef struct {
    u16 col[20][16][16]; // offset 0x0, size 0x2800
} COL_x2800;

// bss
u16 colPalBuffDC[1024];
u16 ColorRAM[512][64];
Col3rd_W col3rd_w;

// sbss
COL* plcol[2];
PixelFormat palFormRam;
PixelFormat palFormSrc;
s32 palFormConv;

// forward decls
void palConvRowTim2CI8Clut(u16* src, u16* dst, s32 size);
const u16 hitmark_color[128];
const col_file_data color_file[161];

void q_ldreq_color_data(REQ* curr) {
#if defined(TARGET_PS2)
    void init_trans_color_ram(s32 id, s32 key, u32 type, u32 data);
#endif

    col_file_data* cfn;
    s32 err;

    cfn = (col_file_data*)&color_file[curr->ix];

    switch (curr->rno) {
    case 0:
        if (fsCheckCommandExecuting() != 0) {
            break;
        }
        if (cfn->type == 10) {
            if (sndCheckVTransStatus(0) == 0) {
                break;
            }
            if (cfn->data + 1 == cseGetIdStoredBd(curr->id + 1)) {
                *curr->result |= lpr_wrdata[curr->id];
                curr->be = 0;
                break;
            }
        }

        curr->rno = 1;
        curr->fnum = cfn->apfn;

        if (cfn->apfn == 0xFFFF) {
            *curr->result |= lpr_wrdata[curr->id];
            curr->be = 0;
        }
        /* fallthrough */
    case 1:
        err = fsOpen(curr);
        if (err == 0) {
            curr->rno = 0;
            break;
        }
        curr->rno = 2;
        /* fallthrough */
    case 2:
        curr->size = fsGetFileSize(curr->fnum);
        curr->sect = fsCalSectorSize(curr->size);
        curr->key = Pull_ramcnt_key(curr->sect << 11, curr->kokey, curr->group, curr->frre);
        Set_size_data_ramcnt_key(curr->key, curr->size);
        curr->rno = 3;
        /* fallthrough */
    case 3:
        err = fsRequestFileRead(curr, curr->sect, (void*)Get_ramcnt_address(curr->key));

        if (err == 0) {
            Push_ramcnt_key(curr->key);
            fsClose(curr);
            curr->rno = 0;
        } else {
            curr->rno = 4;
            curr->be = 1;
        }
        break;

    case 4:
        switch (fsCheckFileReaded(curr)) {
        case 1:
            if (cfn->type == 10) {
                fsClose(curr);
                cseSendBd2SpuWithId((void*)Get_ramcnt_address(curr->key),
                                    Get_size_data_ramcnt_key(curr->key),
                                    curr->id + 1,
                                    cfn->data + 1);
                curr->rno = 5;
            } else {
                init_trans_color_ram(curr->id, curr->key, cfn->type, cfn->data);
                fsClose(curr);
                *curr->result |= lpr_wrdata[curr->id];
                curr->be = 0;
            }
            break;
        case 0:
            break;
        default:
            Push_ramcnt_key(curr->key);
            fsClose(curr);
            curr->be = 2;
            curr->rno = 0;
            break;
        }
        break;

    case 5:
        if (sndCheckVTransStatus(1) != 0) {
            Push_ramcnt_key(curr->key);
            cseMemMapSetPhdAddr(curr->id + 1, csePHDDataTable[cfn->data + 1]);
            cseTsbSetBankAddr(curr->id + 1, cseTSBDataTable[cfn->data + 1]);
            sdbd[curr->id + 1] = (s8*)cseTSBDataTable[cfn->data + 1];
            *curr->result |= lpr_wrdata[curr->id];
            curr->be = 0;
        }
        break;
    }
}

void load_any_color(u16 ix, u8 kokey) {
#if defined(TARGET_PS2)
    void init_trans_color_ram(s16 id, s32 key, u32 type, u32 data);
#endif

    col_file_data* cfn;
    s16 key;

    cfn = (col_file_data*)&color_file[ix];
    key = load_it_use_any_key(cfn->apfn, kokey, 0);

    if (key) {
        init_trans_color_ram(0, key, cfn->type, cfn->data);
    }
}

void set_hitmark_color() {
    u16* hmcol = (u16*)&hitmark_color;
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

void init_trans_color_ram(s16 id, s16 key, u8 type, u16 data) {
#if defined(TARGET_PS2)
    void metamor_color_store(s32 wkid);
#endif

    u16* ldadrs;
    u16* tradrs;
    s16 i;
    s16 j;
    s32 size;

    switch (type) {
    case 1:
        plcol[id] = (COL*)Get_ramcnt_address(key);
        if (My_char[id] == 0) {
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
        } else {

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
        break;

    case 2:
        size = Get_size_data_ramcnt_key(key);
        size = size / 2;
        tradrs = (u16*)Get_ramcnt_address(key);
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
        break;
    case 3: {
        COL_x1000* dadr = (COL_x1000*)Get_ramcnt_address(key);
        if (id == 2) {
            for (i = 0; i < 64; i++) {
                hi_meta[0][0][i] = dadr->col[0][Player_Color[0]][i];
                hi_meta[0][1][i] = dadr->col[0][Player_Color[0]][i];
                hi_meta[1][0][i] = dadr->col[0][Player_Color[1]][i];
                hi_meta[1][1][i] = dadr->col[0][Player_Color[1]][i];
            }

            metamor_color_store(0);
            metamor_color_store(1);
        } else {
            if ((My_char[(id + 1) & 1]) == 0) {
                for (i = 0; i < 64; i++) {
                    hi_meta[id][0][i] = dadr->col[0][Player_Color[id]][i];
                    hi_meta[id][1][i] = dadr->col[1][Player_Color[id]][i];
                }
            } else {
                for (i = 0; i < 64; i++) {
                    hi_meta[id][0][i] = dadr->col[0][Player_Color[id]][i];
                    hi_meta[id][1][i] = dadr->col[0][Player_Color[id]][i];
                }
            }

            metamor_color_store(id);
        }
        Push_ramcnt_key(key);
        break;
    }
    case 4: {
        COL_x80* adr = (COL_x80*)Get_ramcnt_address(key);
        u16* src = (&adr[Player_Color[id]])->col;
        u16* dst = (u16*)&ColorRAM[data + (id * 16)][0];

        // these unsigned constants are here intentionally, otherwise wouldn't match.
        for (i = 0; i < 64U; i++) {
            dst[i] = palConvSrcToRam(src[i]);
        }

        dst = (u16*)&ColorRAM[data + (id * 16) + 8][0];
        for (i = 0; i < 64U; i++) {
            dst[i] = palConvSrcToRam(src[i]);
        }

        Push_ramcnt_key(key);
        palUpdateGhostCP3(data + (id * 16), 1);
        palUpdateGhostCP3(data + ((id * 16) + 8), 1);
        break;
    }
    case 5: {
        COL_x180* adr = (COL_x180*)Get_ramcnt_address(key);
        u16* src = (&adr[Player_Color[id]])->col[0];
        u16* dst = (u16*)&ColorRAM[data + (id * 16)][0];
        for (i = 0; i < 192U; i++) {
            dst[i] = palConvSrcToRam(src[i]);
        }

        dst = (u16*)&ColorRAM[data + (id * 16) + 8][0];
        for (i = 0; i < 192U; i++) {
            dst[i] = palConvSrcToRam(src[i]);
        }

        Push_ramcnt_key(key);
        palUpdateGhostCP3((data) + (id * 16), 3);
        palUpdateGhostCP3((data) + ((id * 16) + 8), 3);
        break;
    }
    case 6: {
        COL_x100* adr = (COL_x100*)Get_ramcnt_address(key);
        u16* src = (&adr[Player_Color[id]])->col[0];
        u16* dst = (u16*)&ColorRAM[data + (id * 16)][0];

        for (i = 0; i < 128U; i++) {
            dst[i] = palConvSrcToRam(src[i]);
        }

        dst = (u16*)&ColorRAM[data + (id * 16) + 8][0];
        for (i = 0; i < 128U; i++) {
            dst[i] = palConvSrcToRam(src[i]);
        }
        Push_ramcnt_key(key);
        palUpdateGhostCP3(data + (id * 16), 2);
        palUpdateGhostCP3((data) + ((id * 16) + 8), 2);
        break;
    }
    case 7: {
        COL_x2800* adrs = (COL_x2800*)Get_ramcnt_address(key);
        ldadrs = (u16*)&ColorRAM[40];
        tradrs = (u16*)&ColorRAM[41];

        for (i = 0; i < 16; i++) {
            ldadrs[i] = palConvSrcToRam(adrs->col[My_char[0]][Player_Color[0]][i]);
            tradrs[i] = palConvSrcToRam(adrs->col[My_char[1]][Player_Color[1]][i]);
        }

        Push_ramcnt_key(key);
        palUpdateGhostCP3(40, 2);
        break;
    }
    case 8:
        cseSendBd2SpuWithId((void*)Get_ramcnt_address(key), Get_size_data_ramcnt_key(key), 0, 0);

        while (!sndCheckVTransStatus(1)) {
            waitVsyncDummy();
        }

        Push_ramcnt_key(key);
        break;

    case 10:
        cseSendBd2SpuWithId((void*)Get_ramcnt_address(key), Get_size_data_ramcnt_key(key), id + 1, data + 1);

        while (!sndCheckVTransStatus(1)) {
            waitVsyncDummy();
        }

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
    adrs = (u8*)Get_ramcnt_address(key);

    for (i = 0; i < size; i++) {
        adrs[i] = 0;
    }

    ppgSetupPalChunkDir(&col3rd_w.palDC, &ppl, adrs, 0, 1);
    Push_ramcnt_key(key);

    ppl.palettes = 2;
    size = 0x2000;
    key = Pull_ramcnt_key(size, 2, 0, 1);
    adrs = (u8*)Get_ramcnt_address(key);

    for (i = 0; i < size; i++) {
        adrs[i] = 0;
    }

    ppgSetupPalChunkDir(&col3rd_w.palCP3, &ppl, adrs, 0, 1);
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
            palConvRowTim2CI8Clut(srcAdrs, dstAdrs, 0x40);
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
        palConvRowTim2CI8Clut(srcAdrs, dstAdrs, 0x40);
        flUnlockPalette(col3rd_w.palCP3.handle[i]);
    }
}

void palConvRowTim2CI8Clut(u16* src, u16* dst, s32 size) {
    s32 i;
    static u8 clut_tbl[32] = { 0, 1, 2,  3,  4,  5,  6,  7,  16, 17, 18, 19, 20, 21, 22, 23,
                               8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 26, 27, 28, 29, 30, 31 };

    for (i = 0; i < size; i++) {
        dst[(i & 0xE0) + clut_tbl[i & 0x1F]] = src[i];
    }
}

// rodata
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
