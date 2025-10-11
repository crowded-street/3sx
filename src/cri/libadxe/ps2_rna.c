#include "common.h"
#include <cri/private/libadxe/dtr.h>
#include <cri/private/libadxe/dtx.h>
#include <cri/private/libadxe/ps2_rna.h>
#include <cri/private/libadxe/rna_crs.h>
#include <cri/private/libadxe/sjr_clt.h>
#include <cri/private/libadxe/sjx.h>
#include <cri/private/libadxe/structs.h>

#include <cri/cri_adxt.h>

#include <eekernel.h>
#include <sifdev.h>

#include <string.h>

#define PS2PSJ_MAX_OBJ 32
#define PS2RNA_WORK_SIZE 0x8D0
#define PS2RNA_MAX_OBJ 16
#define PS2RNA_MAX_VOICE 32

// data
Char8* volatile ps2rna_build = "\nPS2RNA Ver 1.22 Build:Sep 18 2003 10:00:13\n";

Sint32 ps2rna_dbtbl[1000] = {
    256, 253, 250, 247, 244, 241, 238, 236, 233, 230, 228, 225, 222, 220, 217, 215, 212, 210, 208, 205, 203, 201, 198,
    196, 194, 191, 189, 187, 185, 183, 181, 179, 177, 175, 173, 171, 169, 167, 165, 163, 161, 159, 157, 156, 154, 152,
    150, 149, 147, 145, 143, 142, 140, 139, 137, 135, 134, 132, 131, 129, 128, 126, 125, 123, 122, 121, 119, 118, 117,
    115, 114, 113, 111, 110, 109, 107, 106, 105, 104, 103, 101, 100, 99,  98,  97,  96,  95,  94,  92,  91,  90,  89,
    88,  87,  86,  85,  84,  83,  82,  81,  80,  80,  79,  78,  77,  76,  75,  74,  73,  72,  72,  71,  70,  69,  68,
    68,  67,  66,  65,  65,  64,  63,  62,  62,  61,  60,  60,  59,  58,  57,  57,  56,  56,  55,  54,  54,  53,  52,
    52,  51,  51,  50,  49,  49,  48,  48,  47,  47,  46,  46,  45,  45,  44,  43,  43,  42,  42,  41,  41,  41,  40,
    40,  39,  39,  38,  38,  37,  37,  37,  36,  36,  35,  35,  34,  34,  34,  33,  33,  32,  32,  32,  31,  31,  31,
    30,  30,  30,  29,  29,  29,  28,  28,  28,  27,  27,  27,  26,  26,  26,  25,  25,  25,  25,  24,  24,  24,  23,
    23,  23,  23,  22,  22,  22,  22,  21,  21,  21,  21,  20,  20,  20,  20,  19,  19,  19,  19,  18,  18,  18,  18,
    18,  17,  17,  17,  17,  17,  16,  16,  16,  16,  16,  15,  15,  15,  15,  15,  15,  14,  14,  14,  14,  14,  14,
    13,  13,  13,  13,  13,  13,  12,  12,  12,  12,  12,  12,  12,  11,  11,  11,  11,  11,  11,  11,  11,  10,  10,
    10,  10,  10,  10,  10,  10,  9,   9,   9,   9,   9,   9,   9,   9,   9,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   6,   6,   6,   6,   6,   6,   6,   6,   6,
    6,   6,   6,   6,   6,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   4,   4,   4,
    4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   3,   3,   3,   3,   3,   3,
    3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

Sint32 ps2rna_max_voice = 0;
Sint32 ps2psj_iop_wksize = 0;
Sint32 ps2rna_setupvoice_flag = 0;
void* ps2psj_iop_work0 = NULL;
void* ps2psj_iop_work = NULL;
Sint8 ps2psj_alloc_flag = 0;
Sint32 ps2rna_psm_wksize = 0;
void* ps2rna_psm_wk0 = NULL;
uintptr_t ps2rna_psm_wk = 0;
Sint32 ps2rna_psm_max_voice = 0;
Sint32 ps2rna_max_nste = 0;
Sint32 ps2rna_max_nmono = 0;
Sint32 ps2rna_dolby_flg = 0;
Sint32 ps2rna_ee_allpause = 0;
Sint32 ps2rna_iop_allpause = 0;
Sint32 ps2rna_init_cnt = 0;
void* ps2rna_eewk = NULL;
void* ps2rna_iopwk0 = NULL;
void* ps2rna_iopwk = NULL;
PS2PSJ_OBJ ps2psj_obj[PS2PSJ_MAX_OBJ] = { 0 };
Sint8 ps2psj_sjuni_eewk[PS2PSJ_MAX_OBJ][256] = { 0 };
Sint8 ps2psj_sjiop_wk[0x80] = { 0 };
Sint8 ps2psj_sjiop_buf[0x80] = { 0 };
ADXRNA_OBJ ps2rna_obj[PS2RNA_MAX_OBJ] = { 0 };
DTX ps2rna_dtx = NULL;
Sint32 ps2rna_wklen = 0;
Sint8 ps2rna_ee_work[PS2RNA_WORK_SIZE] = { 0 };

// forward decls
void PS2RNA_InitIopSnd();

void PS2RNA_SetupVoice(Sint32 nste, Sint32 nmono) {
    Sint32 voice_count;

    if (nste < 0) {
        scePrintf("PS2RNA_SetupVoice:Illigal parameter(nste=%d).", nste);
        return;
    }

    if (nmono < 0) {
        scePrintf("PS2RNA_SetupVoice:Illigal parameter(nmono=%d).", nmono);
        return;
    }

    voice_count = (nste * 2) + nmono;

    if (voice_count > PS2RNA_MAX_VOICE) {
        scePrintf("PS2RNA_SetupVoice:The set-up value exceeded maximum(MAX=%d).", PS2RNA_MAX_VOICE);
        return;
    }

    ps2rna_max_voice = voice_count;
    ps2rna_max_nste = nste;
    ps2rna_max_nmono = nmono;
    ps2psj_iop_wksize = voice_count * 0x4100;
    ps2rna_setupvoice_flag = 1;
}

void ps2rna_init_psj() {
    Sint8* wk;
    Sint32 i;
    Sint32 j;
    PS2PSJ psj;

    if (ps2rna_setupvoice_flag == 0) {
        ps2rna_max_voice = 16;
        ps2psj_iop_wksize = 0x41000;
    }

    if (ps2psj_iop_work0 == NULL) {
        ps2psj_iop_work0 = sceSifAllocIopHeap(ps2psj_iop_wksize + 0x40);

        if (ps2psj_iop_work0 == NULL) {
            scePrintf("PS2RNA: E01112903: Failed, sceSifAllocIopHeap(%d) in ps2rna_init_psj\n",
                      ps2psj_iop_wksize + 0x40);
            while (1) {}
        }

        scePrintf("PS2RNA: sceSifAllocIopHeap(%d) ret=0x%08x\n", ps2psj_iop_wksize + 0x40, ps2psj_iop_work0);
        ps2psj_iop_work = (uintptr_t)(ps2psj_iop_work0 + 0x40) & ~(0x40 - 1);
        ps2psj_alloc_flag = 1;
    }

    memset(ps2psj_obj, 0, sizeof(ps2psj_obj));
    wk = ps2psj_iop_work;

    for (i = 0, j = 0; i < ps2rna_max_voice; i++, j++) {
        psj = &ps2psj_obj[j];
        psj->used = 0;

        if ((uintptr_t)wk & 0x3F) {
            scePrintf("E0110101: ps2rna_init_psj wk size error\n");
            while (1) {}
        }

        psj->unk4 = SJUNI_CreateRmt(1, wk, 0x100);
        wk += 0x100;

        if (psj->unk4 == NULL) {
            scePrintf("E0110102: ps2rna_init_psj: can't creat SJUNI_CreaetRmt\n");
            while (1) {}
        }

        if ((uintptr_t)wk & 0x3F) {
            scePrintf("E0110103: ps2rna_init_psj: wk size error\n");
            while (1) {}
        }

        psj->unk8 = SJUNI_Create(SJUNI_MODE_JOIN, ps2psj_sjuni_eewk[i], 0x100);

        if (psj->unk8 == NULL) {
            scePrintf("E0110104: ps2rna_init_psj: can't creat SJUNI_Creaet\n");
            while (1) {}
        }

        psj->chunk.len = 0x4000;
        psj->chunk.data = wk;
        wk += 0x4000;
        SJ_PutChunk(psj->unk8, 0, &psj->chunk);
        psj->unkC = SJX_Create(psj->unk8, psj->unk4, 1);

        if (psj->unkC == NULL) {
            scePrintf("E0110105: ps2rna_init_psj: can't creat SJX_Create\n");
            while (1) {}
        }
    }
}

void ps2rna_finish_psj() {
    PS2PSJ psj;
    Sint32 i;

    for (i = 0; i < ps2rna_max_voice; i++) {
        psj = &ps2psj_obj[i];

        if (psj->unk4 != NULL) {
            SJRMT_Destroy(psj->unk4);
        }

        if (psj->unk8 != NULL) {
            SJ_Destroy(psj->unk8);
        }

        if (psj->unkC != NULL) {
            SJX_Destroy(psj->unkC);
        }

        memset(psj, 0, sizeof(PS2PSJ_OBJ));
    }

    if (ps2psj_alloc_flag == 1) {
        sceSifFreeIopHeap(ps2psj_iop_work0);
        ps2psj_iop_work0 = NULL;
        ps2psj_alloc_flag = 0;
    }
}

PS2PSJ ps2rna_get_psj() {
    PS2PSJ psj = NULL;
    Sint32 i;

    for (i = 0; i < ps2rna_max_voice; i++) {
        psj = &ps2psj_obj[i];

        if (psj->used == 0) {
            break;
        }
    }

    if (i == ps2rna_max_voice) {
        return NULL;
    }

    psj->used = 1;
    return psj;
}

void ps2rna_release_psj(PS2PSJ psj) {
    psj->used = 0;
};

void ps2rna_rcvcbf(void* object, void* arg1, Sint32 arg2) {
    Sint32 i;
    Sint32 ticks;

    if (arg1 == NULL) {
        while (1) {}
    }

    ticks = ((Sint32*)arg1)[0];

    for (i = 0; i < ticks; i++) {
        // Do nothing
    }
}

typedef struct {
    Sint16 unk0;
    void* unk4;
    Sint32 unk8;
    Sint32 unkC;
} PS2RNA_UNK_Item;

typedef struct {
    Sint32 count;
    char pad4[0xC];
    PS2RNA_UNK_Item items[0];
} PS2RNA_UNK_Header;

void ps2rna_sndcbf(void* object, void* arg1, Sint32 arg2) {
    ADXRNA rna;

    Sint32 i;
    Sint32 j;
    Sint32 k;

    PS2RNA_UNK_Header* head = (PS2RNA_UNK_Header*)arg1;
    PS2RNA_UNK_Item* items = head->items;
    PS2RNA_UNK_Item* item;

    j = 0;

    for (i = 0; i < 16; i++) {
        rna = &ps2rna_obj[i];

        if (rna->used != 1) {
            continue;
        }

        if (j == 0x80) {
            goto end;
        }

        if (ps2rna_ee_allpause != ps2rna_iop_allpause) {
            items[j].unk0 = 0xB;
            items[j].unk4 = 0;
            items[j].unk8 = ps2rna_ee_allpause;
            ps2rna_iop_allpause = ps2rna_ee_allpause;
            j += 1;
        }

        if (j == 0x80) {
            goto end;
        }

        if (rna->unk5F != rna->unk5E) {
            items[j].unk0 = 8;
            items[j].unk4 = rna->unk20;
            items[j].unk8 = rna->unk5F;
            rna->unk5E = rna->unk5F;
            j += 1;
        }

        if (j == 0x80) {
            goto end;
        }

        if (rna->unk30 == 0 && rna->unk31 == 1) {
            items[j].unk0 = 2;
            items[j].unk4 = rna->unk20;
            items[j].unk8 = rna->unk30;
            rna->unk31 = rna->unk30;
            j += 1;
        }

        if (j == 0x80) {
            goto end;
        }

        if (rna->unk54 != rna->unk58) {
            items[j].unk0 = 0xA;
            items[j].unk4 = rna->unk20;
            items[j].unk8 = rna->unk54;
            rna->unk58 = rna->unk54;
            j += 1;
        }

        if (j == 0x80) {
            goto end;
        }

        if (rna->num_chan != rna->unk33) {
            items[j].unk0 = 3;
            items[j].unk4 = rna->unk20;
            items[j].unk8 = rna->num_chan;
            rna->unk33 = rna->num_chan;
            j += 1;
        }

        if (j == 0x80) {
            goto end;
        }

        if (rna->sfreq != rna->unk38) {
            items[j].unk0 = 4;
            items[j].unk8 = rna->sfreq;
            rna->unk38 = rna->sfreq;
            items[j].unk4 = rna->unk20;
            j += 1;
        }

        if (j == 0x80) {
            goto end;
        }

        if (rna->out_vol != rna->unk40) {
            int y = ps2rna_dbtbl[-rna->out_vol];
            items[j].unk0 = 5;
            items[j].unk4 = rna->unk20;
            items[j].unk8 = 0;
            items[j].unkC = y;
            rna->unk40 = rna->out_vol;
            j += 1;
        }

        if (j == 0x80) {
            goto end;
        }

        if (j == 0x7F) {
            goto end;
        }

        for (k = 0; k < rna->num_chan; k++) {
            if (rna->pan[k] != rna->unk4C[k]) {
                items[j].unk0 = 9;
                items[j].unk4 = rna->unk20;
                items[j].unk8 = k;
                items[j].unkC = rna->pan[k];
                rna->unk4C[k] = rna->pan[k];
                j += 1;
            }
        }

        if (j == 0x80) {
            goto end;
        }

        if (rna->unk30 == 1 && rna->unk31 == 0) {
            items[j].unk0 = 2;
            items[j].unk4 = rna->unk20;
            items[j].unk8 = rna->unk30;
            rna->unk31 = rna->unk30;
            j += 1;
        }

        if (j == 0x80) {
            goto end;
        }
    }

end:
    head->count = j;
}

void PS2RNA_Init() {
    intptr_t snd_buf[3];

    if (ps2rna_init_cnt == 0) {
        SJRMT_Init();
        SJX_Init();
        DTX_Init();
        PS2RNA_InitIopSnd();
        ps2rna_init_psj();
        memset(ps2rna_ee_work, 0, sizeof(ps2rna_ee_work));
        ps2rna_wklen = 0x880;
        ps2rna_eewk = (uintptr_t)(ps2rna_ee_work + 0x40) & ~0x3F;

        if (ps2rna_iopwk0 == NULL) {
            ps2rna_iopwk0 = sceSifAllocIopHeap(PS2RNA_WORK_SIZE);

            if (ps2rna_iopwk0 == NULL) {
                scePrintf("PS2RNA: E01112902: Failed, sceSifAllocIopHeap(%d) in PS2RNA_Init.", PS2RNA_WORK_SIZE);
                while (1) {}
            }
        }

        scePrintf("PS2RNA: sceSifAllocIopHeap(%d) ret=0x%08x\n", PS2RNA_WORK_SIZE, ps2rna_iopwk0);
        ps2rna_iopwk = (uintptr_t)(ps2rna_iopwk0 + 0x40) & ~0x3F;
        ps2rna_dtx = DTX_Create(1, ps2rna_eewk, ps2rna_iopwk, ps2rna_wklen);

        if (ps2rna_dtx == NULL) {
            scePrintf("E0100302: PS2RNA_Init can't create DTX\n");
            while (1) {}
        }

        DTX_SetRcvCbf(ps2rna_dtx, ps2rna_rcvcbf, NULL);
        DTX_SetSndCbf(ps2rna_dtx, ps2rna_sndcbf, NULL);

        if (ps2rna_psm_wk0 == NULL) {
            ps2rna_psm_max_voice = ps2rna_max_voice;
            ps2rna_psm_wksize = (ps2rna_max_voice * 1024) + 0x40;
            ps2rna_psm_wk0 = sceSifAllocIopHeap(ps2rna_psm_wksize);

            if (ps2rna_psm_wk0 == NULL) {
                scePrintf("E02041101: Failed, sceSifAllocIopHeap(%d) in PS2RNA_Init.\0\0\0\0", ps2rna_psm_wksize);
                while (1) {}
            }

            scePrintf("PS2RNA: sceSifAllocIopHeap(%d) ret=0x%08x\n", ps2rna_psm_wksize, ps2rna_psm_wk0);
        }

        snd_buf[0] = ps2rna_psm_max_voice;
        ps2rna_psm_wk = snd_buf[1] = (uintptr_t)(ps2rna_psm_wk0 + 0x3F) & ~0x3F;
        snd_buf[2] = ps2rna_psm_wksize;
        DTX_CallUrpc(10, snd_buf, 3, NULL, 0);
    }

    ps2rna_init_cnt += 1;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_RestoreIopSnd);

void PS2RNA_InitIopSnd() {
    DTX_CallUrpc(12, NULL, 0, NULL, 0);
    ps2rna_ee_allpause = 0;
    ps2rna_iop_allpause = 0;
}

void PS2RNA_FinishIopSnd() {
    DTX_CallUrpc(13, NULL, 0, NULL, 0);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_SetupDolby);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_ShutdownDolby);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_SetStmType);

void PS2RNA_Finish() {
    if (--ps2rna_init_cnt == 0) {
        sceSifFreeIopHeap(ps2rna_psm_wk0);
        ps2rna_psm_wk0 = NULL;
        DTX_Destroy(ps2rna_dtx);
        ps2rna_dtx = NULL;
        sceSifFreeIopHeap(ps2rna_iopwk0);
        ps2rna_iopwk0 = NULL;
        ps2rna_finish_psj();
        PS2RNA_FinishIopSnd();
        DTX_Finish();
        SJX_Finish();
        SJRMT_Finish();
    }
}

ADXRNA PS2RNA_Create(SJ* sjo, Sint32 maxnch) {
    intptr_t snd_buf[4];
    intptr_t rcv_buf[1];
    PS2PSJ psj;
    Sint32 i;
    Sint32 j;
    Sint32 k;
    ADXRNA rna;

    for (i = 0; i < PS2RNA_MAX_OBJ; i++) {
        rna = &ps2rna_obj[i];

        if (rna->used == 0) {
            break;
        }
    }

    if (i == PS2RNA_MAX_OBJ) {
        return NULL;
    }

    memset(rna, 0, sizeof(ADXRNA_OBJ));
    rna->maxnch = maxnch;

    for (i = 0; i < maxnch; i++) {
        psj = ps2rna_get_psj();
        rna->psj[i] = psj;

        if (psj == NULL) {
            scePrintf("E1072601: not enough PSJ handle\n");

            for (j = 0; j < maxnch; j++) {
                psj = rna->psj[j];

                if (psj != NULL) {
                    ps2rna_release_psj(psj);
                }
            }

            return NULL;
        }
    }

    snd_buf[0] = maxnch;
    snd_buf[1] = 0;

    for (i = 0; i < maxnch; i++) {
        snd_buf[2 + i] = rna->psj[i]->unk4;
    }

    rna->unk20 = DTX_CallUrpc(8, snd_buf, 4, rcv_buf, 1);

    if (rna->unk20 == 0) {
        scePrintf("E0100401: can't create PS2RNA of IOP\n");
        return NULL;
    }

    for (i = 0; i < maxnch; i++) {
        rna->dtr[i] = DTR_Create(sjo[i], rna->psj[i]->unk8);

        if (rna->dtr[i] == 0) {
            scePrintf("E1072602: can't create DTR handle\n\0\0\0\0");

            for (k = 0; k < maxnch; k++) {
                if (rna->dtr[k] != 0) {
                    DTR_Destroy(rna->dtr[k]);
                }

                if (rna->unk20 != 0) {
                    snd_buf[0] = rna->unk20;
                    DTX_CallUrpc(9, snd_buf, 1, NULL, 0);
                }

                if (rna->psj[k] != NULL) {
                    ps2rna_release_psj(rna->psj[k]);
                }
            }

            return NULL;
        }
    }

    for (i = 0; i < maxnch; i++) {
        rna->sjo[i] = sjo[i];
    }

    rna->unk30 = 0;
    rna->unk31 = 0;
    rna->num_chan = maxnch;
    rna->unk33 = maxnch;
    rna->out_vol = 0;
    rna->unk40 = 0;
    rna->unk54 = 0;
    rna->unk58 = 0;
    rna->unk5F = 0;
    rna->unk5E = 0;

    for (i = 0; i < maxnch; i++) {
        rna->pan[i] = 0;
        rna->unk4C[i] = 0;
    }

    rna->unk5C = 0;
    rna->unk5D = 1;
    rna->unk24 = 0;
    rna->unk28 = SJ_GetNumData(sjo[0], 1) + SJ_GetNumData(sjo[0], 0);
    rna->unk2C = 0x4000;
    rna->used = 1;
    return rna;
}

void PS2RNA_Destroy(ADXRNA rna) {
    intptr_t snd_buf[1];
    Sint32 i;

    for (i = 0; i < rna->maxnch; i++) {
        if (rna->dtr[i] != 0) {
            DTR_Destroy(rna->dtr[i]);
        }
    }

    snd_buf[0] = rna->unk20;
    DTX_CallUrpc(9, snd_buf, 1, NULL, 0);

    for (i = 0; i < rna->maxnch; i++) {
        if (rna->psj[i] != NULL) {
            ps2rna_release_psj(rna->psj[i]);
        }
    }

    rna->used = 0;
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", D_0055E440);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_Start);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_Stop);

void PS2RNA_SetTransSw(ADXRNA rna, Sint32 sw) {
    Sint32 i;
    Sint32 v1;

    rna->unk5C = sw;
    v1 = rna->num_chan;

    if (sw == 1) {
        return;
    }

    for (i = 0; i < v1; i++) {
        DTR_Stop(rna->dtr[i]);
        rna->unk5D = 1;
        rna->unk5C = 0;
    }
}

void PS2RNA_SetPlaySw(ADXRNA rna, Sint32 sw) {
    rna->unk30 = sw;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_PauseAll);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_GetStatPauseAll);

void PS2RNA_SetDiscardSw(ADXRNA rna, Sint32 sw) {
    rna->unk5F = sw;
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", D_0055E460);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_ClearBuf);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", D_0055E488);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_SetPcmType);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_GetTime);

Sint32 PS2RNA_GetNumData(ADXRNA rna) {
    return (0x4000 - SJ_GetNumData(rna->psj[0]->unk8, 0)) / 2;
}

Sint32 PS2RNA_GetNumRoom(ADXRNA rna) {
    return SJ_GetNumData(rna->psj[0]->unk8, 0) / 2;
}

void PS2RNA_ExecHndl(ADXRNA rna) {
    SJCK chunk;
    Sint32 var_s2;
    Sint32 i;

    if (rna->unk5C != 1) {
        return;
    }

    var_s2 = 0;

    for (i = 0; i < rna->num_chan; i++) {
        if (DTR_GetStat(rna->dtr[i]) == 1) {
            continue;
        }

        if (SJ_GetNumData(rna->psj[i]->unk8, 0) != rna->unk2C) {
            continue;
        }

        var_s2 += 1;
    }

    if (var_s2 != rna->num_chan) {
        return;
    }

    i = 0;

    if (var_s2 <= 0) {
        return;
    }

    for (i = 0; i < rna->num_chan; i++) {
        SJ_GetChunk(rna->psj[i]->unk8, 0, SJCK_LEN_MAX, &chunk);
        SJ_PutChunk(rna->psj[i]->unk8, 0, &chunk);
        SJ_GetChunk(rna->psj[i]->unk8, 0, SJCK_LEN_MAX, &chunk);

        if (chunk.len != rna->unk2C) {
            while (1) {}
        }

        SJ_PutChunk(rna->psj[i]->unk8, 0, &chunk);
        rna->unk5F = 0;
        DTR_Start(rna->dtr[i]);
    }
}

void PS2RNA_ExecServer() {
    ADXRNA rna;
    Sint32 i;

    RNACRS_Lock();

    for (i = 0; i < PS2RNA_MAX_OBJ; i++) {
        rna = &ps2rna_obj[i];

        if (rna->used == 1) {
            PS2RNA_ExecHndl(rna);
        }
    }

    RNACRS_Unlock();
    DTR_ExecServer();
    SJX_ExecServer();
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", D_0055E4B0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_SetStartSmpl);

void PS2RNA_SetNumChan(ADXRNA rna, Sint32 num_chan) {
    rna->num_chan = num_chan;
}

void PS2RNA_SetSfreq(ADXRNA rna, Sint32 sfreq) {
    if ((ADXT_MAX_SFREQ % sfreq) != 0) {
        scePrintf("sampling frequency=%d[Hz]\n", sfreq);
        while (1) {}
    }

    if ((sfreq != ADXT_MAX_SFREQ) && ((ADXT_MAX_SFREQ / sfreq) % 2) != 0) {
        scePrintf("sampling frequency=%d[Hz]\n", sfreq);
        while (1) {}
    }

    rna->sfreq = sfreq;
}

void PS2RNA_SetOutVol(ADXRNA rna, Sint32 vol) {
    if (vol > 0) {
        vol = 0;
    }

    if (vol < -999) {
        vol = -999;
    }

    rna->out_vol = vol;
}

void PS2RNA_SetOutPan(ADXRNA rna, Sint32 channel, Sint32 pan) {
    Sint32 _pan = pan;

    if (_pan < ADXT_PAN_LEFT) {
        _pan = ADXT_PAN_LEFT;
    }

    if (_pan > ADXT_PAN_RIGHT) {
        _pan = ADXT_PAN_RIGHT;
    }

    rna->pan[channel] = _pan;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_SetOutBalance);

void PS2RNA_SetBitPerSmpl(ADXRNA rna, Sint32 bps) {
    if (bps != 16) {
        scePrintf("PS2RNA_SetBitPerSmpl: not support %d bps\n\0\0\0\0");
    }
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", D_0055E528);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_GetStartSmpl);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_GetSfreq);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_GetOutVol);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_GetOutPan);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_GetOutBalance);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_GetBitPerSmpl);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", D_0055E550);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_IsOverflow);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", D_0055E578);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_ClearOverflow);

void PS2RNA_Flush() {
    // Do nothing
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", D_0055E5A0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_DiscardData);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", D_0055E5C8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_SetTotalNumSmpl);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", D_0055E5F8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_SetStmHdInfo);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_GetSjtmp);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/ps2_rna", PS2RNA_GetSjiop);
