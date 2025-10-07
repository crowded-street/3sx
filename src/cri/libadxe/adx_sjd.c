#include "common.h"
#include <cri/private/libadxe/adx_bahx.h>
#include <cri/private/libadxe/adx_bsc.h>
#include <cri/private/libadxe/adx_crs.h>
#include <cri/private/libadxe/adx_dcd.h>
#include <cri/private/libadxe/adx_errs.h>
#include <cri/private/libadxe/adx_sjd.h>
#include <cri/private/libadxe/sj_rbf.h>

#include <cri/cri_xpts.h>
#include <cri/sj.h>

#include <string.h>

// data
void (*pl2setsfreqfunc)(ADXB, Sint32) = NULL;
ADXSJD_OBJ adxsjd_obj[ADXSJD_MAX_OBJ] = { 0 };

// forward decls
void* adxsjd_get_wr(ADXSJD sjd, ptrdiff_t* arg1, Sint32* arg2, Sint32* arg3);

void ADXSJD_Init() {
    ADXB_Init();
    memset(adxsjd_obj, 0, sizeof(adxsjd_obj));
}

void ADXSJD_Finish() {
    memset(adxsjd_obj, 0, sizeof(adxsjd_obj));
}

void adxsjd_clear(ADXSJD sjd) {
    sjd->trap_num_samples = -1;
    sjd->unk38 = SJCK_LEN_MAX;
    sjd->unk98 = 0;
    sjd->unk2C = 0;
    sjd->unk30 = 0;
    sjd->unk34 = 0;
    sjd->trap_count = 0;
    sjd->trap_dt_len = 0;
    sjd->unk3 = 0;
}

ADXSJD ADXSJD_Create(SJ sj, Sint32 maxnch, SJ* sjo) {
    ADXSJD sjd;
    SJ out;
    void* buf_ptr;
    Sint32 i;
    Sint32 y;
    Sint32 buf_size;
    Sint32 xtr_size;

    out = sjo[0];

    for (i = 0; i < ADXSJD_MAX_OBJ; i++) {
        if (!adxsjd_obj[i].used) {
            break;
        }
    }

    if (i == ADXSJD_MAX_OBJ) {
        return NULL;
    }

    sjd = &adxsjd_obj[i];
    buf_ptr = SJRBF_GetBufPtr(out);
    buf_size = SJRBF_GetBufSize(out) / 2;
    xtr_size = SJRBF_GetXtrSize(out) / 2;
    sjd->adxb = ADXB_Create(maxnch, buf_ptr, buf_size, buf_size + xtr_size);

    if (sjd->adxb == NULL) {
        return NULL;
    }

    ADXB_EntryGetWrFunc(sjd->adxb, adxsjd_get_wr, sjd);
    sjd->sji = sj;
    sjd->maxnch = maxnch;

    for (y = 0; y < maxnch; y++) {
        sjd->sjo[y] = sjo[y];
    }

    sjd->state = 0;
    adxsjd_clear(sjd);
    sjd->trap_func = NULL;
    sjd->trap_obj = NULL;
    sjd->flt_func = NULL;
    sjd->flt_obj = 0;
    sjd->used = 1;
    return sjd;
}

void ADXSJD_Destroy(ADXSJD sjd) {
    ADXB adxb;

    if (sjd == NULL) {
        return;
    }

    adxb = sjd->adxb;

    if (adxb != NULL) {
        sjd->adxb = NULL;
        ADXB_Destroy(adxb);
    }

    ADXCRS_Lock();
    memset(sjd, 0, sizeof(ADXSJD_OBJ));
    ADXCRS_Unlock();
}

Sint8 ADXSJD_GetStat(ADXSJD sjd) {
    return sjd->state;
}

void ADXSJD_SetInSj(ADXSJD sjd, SJ sj) {
    sjd->sji = sj;
    ADXB_SetAhxInSj(sjd->adxb, sj);
    ADXB_SetAc3InSj(sjd->adxb, sj);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_SetOutSj);

void ADXSJD_SetMaxDecSmpl(ADXSJD sjd, Sint32 max_samples) {
    sjd->unk38 = max_samples;
    ADXB_SetAhxDecSmpl(sjd->adxb, max_samples);
    ADXB_SetAc3DecSmpl(sjd->adxb, max_samples);
}

void ADXSJD_TermSupply(ADXSJD sjd) {
    ADXB_AhxTermSupply(sjd->adxb);
    ADXB_Ac3TermSupply(sjd->adxb);
}

void ADXSJD_Start(ADXSJD sjd) {
    adxsjd_clear(sjd);
    sjd->state = 1;
}

void ADXSJD_Stop(ADXSJD sjd) {
    ADXB_Stop(sjd->adxb);
    sjd->state = 0;
}

void adxsjd_decode_prep(ADXSJD sjd) {
    SJCK ck_0;
    SJCK ck_1;
    Sint32 header_len;
    Uint32 format;
    Sint32 nbyte;
    Sint8 state;
    SJ_OBJ* sji = sjd->sji;
    ADXB_OBJ* adxb = sjd->adxb;

    sjd++;
    sjd--;

    SJ_GetChunk(sji, 1, 0x1000, &ck_0);

    for (nbyte = 0; nbyte < ck_0.len; nbyte++) {
        if (ck_0.data[nbyte] != 0) {
            break;
        }
    }

    SJ_SplitChunk(&ck_0, nbyte, &ck_1, &ck_0);
    SJ_PutChunk(sji, 0, &ck_1);

    if (ck_0.len < 16) {
        SJ_UngetChunk(sji, 1, &ck_0);
        return;
    }

    header_len = ADXB_DecodeHeader(adxb, ck_0.data, ck_0.len);

    if ((header_len == 0) || (ck_0.len < header_len)) {
        SJ_UngetChunk(sji, 1, &ck_0);
        return;
    }

    if (header_len < 0) {
        SJ_UngetChunk(sji, 1, &ck_0);
        ADXERR_CallErrFunc2("E03010901 ADXB_DecodeHeader: ", "Can not decode this file format.");
        state = 4;
    } else {
        sjd->unk98 = header_len;

        if (ADXB_GetFormat(adxb) == 4) {
            sjd->unk3 = 1;
        }

        if (ADXB_GetFormat(adxb) == 2) {
            memcpy(sjd->unk58, ck_0.data, (ck_0.len > 0x40) ? 0x40 : ck_0.len);
        }

        format = ADXB_GetFormat(adxb);

        if (((format - 10) < 2) || (format == 20) || (format == 15)) {
            SJ_UngetChunk(sji, 1, &ck_0);
        } else {
            SJ_SplitChunk(&ck_0, header_len, &ck_0, &ck_1);
            SJ_PutChunk(sji, 0, &ck_0);
            SJ_UngetChunk(sji, 1, &ck_1);
        }

        if ((adxb->unkE4 != 0) && (pl2setsfreqfunc != NULL)) {
            pl2setsfreqfunc(adxb, adxb->sample_rate);
        }

        state = 2;
    }

    sjd->state = state;
}

void* adxsjd_get_wr(ADXSJD sjd, ptrdiff_t* arg1, Sint32* arg2, Sint32* arg3) {
    Sint32 temp_v0_3;
    Sint32 i;
    Sint32 var_v0;
    SJ first_out;
    SJ* sjo;
    SJCK* chunk_p;
    Sint32 a0;

    first_out = sjd->sjo[0];
    sjo = sjd->sjo;
    chunk_p = sjd->chunks;

    for (i = 0; i < ADXB_GetNumChan(sjd->adxb); i++) {
        SJ_GetChunk(sjo[i], 0, 0x4000, &chunk_p[i]);
    }

    *arg1 = ((void*)sjd->chunks[0].data - SJRBF_GetBufPtr(first_out)) / 2;

    a0 = sjd->unk38;
    temp_v0_3 = sjd->chunks[0].len / 2;

    if (temp_v0_3 < a0) {
        a0 = temp_v0_3;
    }

    *arg2 = a0;

    if (sjd->trap_num_samples >= 0) {
        var_v0 = sjd->trap_num_samples - sjd->trap_count;
    } else {
        var_v0 = 0x1FFFFFFF;
    }

    *arg3 = var_v0;
    return ADXB_GetPcmBuf(sjd->adxb);
}

#define READ_MAGIC(ptr) ((((Uint8*)(ptr))[1] | ((Uint16*)(ptr))[0] << 8) & 0xFFFF)

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", adxsjd_decexec_start);
#else
// Needs testing
void adxsjd_decexec_start(ADXSJD sjd) {
    ADXB adxb = sjd->adxb;
    SJ sji = sjd->sji;
    SJCK sp;
    Sint16 sp10;
    Sint32 blk_smpl;
    Sint32 num_data;
    Sint32 var_16;
    Sint32 var_5;

    if ((sjd->trap_num_samples >= 0) && (sjd->trap_count >= sjd->trap_num_samples)) {
        if (sjd->trap_func != NULL) {
            sjd->trap_func(sjd->trap_obj);
        }
    }

    if ((sjd->unk3 == 1) && (SJ_GetNumData(sji, 1) == 0)) {
        sjd->state = 3;
        return;
    }

    SJ_GetChunk(sji, 1, SJCK_LEN_MAX, &sjd->unk14);

    if ((ADXB_GetFormat(adxb) == 0) && (sjd->unk14.len >= 4) && (READ_MAGIC(sjd->unk14.data) == 0x8001)) {
        sjd->state = 3;

        if (ADX_DecodeFooter(sjd->unk14.data, sjd->unk14.len, &sp10) == 0) {
            if (sjd->unk14.len < sp10) {
                SJ_UngetChunk(sji, 1, &sjd->unk14);
                return;
            }

            SJ_SplitChunk(&sjd->unk14, sp10, &sjd->unk14, &sp);
            SJ_PutChunk(sji, 0, &sjd->unk14);
            SJ_UngetChunk(sji, 1, &sp);
        }

        if (sjd->lnkflg == 0) {
            return;
        }

        while (SJ_GetChunk(sji, 1, SJCK_LEN_MAX, &sjd->unk14), sjd->unk14.len != 0) {
            var_5 = 0;

            while (1) {
                var_16 = var_5 < sjd->unk14.len;

                if (!var_16 || sjd->unk14.data[var_5] != 0) {
                    break;
                }

                var_5 += 1;
            }

            SJ_SplitChunk(&sjd->unk14, var_5, &sjd->unk14, &sp);
            SJ_PutChunk(sji, 0, &sjd->unk14);
            SJ_UngetChunk(sji, 1, &sp);

            if (var_16) {
                break;
            }
        }

        return;
    }

    if (sjd->unk34 >= ADXSJD_GetTotalNumSmpl(sjd)) {
        sjd->state = 3;
        SJ_UngetChunk(sji, 1, &sjd->unk14);
        return;
    }

    blk_smpl = ADXSJD_GetBlkSmpl(sjd);
    num_data = SJ_GetNumData(sjd->sjo[0], 0);

    if (blk_smpl > num_data / 2) {
        SJ_UngetChunk(sji, 1, &sjd->unk14);
        return;
    }

    if (ADXB_GetFormat(adxb) == 0xA) {
        SJ_UngetChunk(sji, 1, &sjd->unk14);
    }

    if (ADXB_GetFormat(adxb) == 0xF) {
        SJ_UngetChunk(sji, 1, &sjd->unk14);
    }

    ADXB_EntryData(adxb, sjd->unk14.data, sjd->unk14.len);
    ADXB_Start(adxb);
}
#endif

void adxsjd_decexec_end(ADXSJD sjd) {
    ADXB adxb = sjd->adxb;
    SJ sji = sjd->sji;
    Sint32 total_num_smpl = ADXB_GetTotalNumSmpl(adxb);
    Sint32 dec_dt_len = ADXB_GetDecDtLen(adxb);
    Sint32 dec_num_smpl = ADXB_GetDecNumSmpl(adxb);
    SJCK sp;
    SJCK sp10;
    Sint32 i;

    total_num_smpl -= sjd->unk34;

    if (dec_num_smpl >= total_num_smpl) {
        dec_num_smpl = total_num_smpl;
    }

    SJ_SplitChunk(&sjd->unk14, dec_dt_len, &sp, &sp10);
    SJ_PutChunk(sji, 0, &sp);
    SJ_UngetChunk(sji, 1, &sp10);

    for (i = 0; i < ADXB_GetNumChan(sjd->adxb); i++) {
        SJ_SplitChunk(&sjd->chunks[i], dec_num_smpl * 2, &sp, &sp10);

        if (sjd->flt_func != NULL) {
            sjd->flt_func(sjd->flt_obj, i, sp.data, sp.len);
        }

        SJ_PutChunk(sjd->sjo[i], 1, &sp);
        SJ_UngetChunk(sjd->sjo[i], 0, &sp10);
    }

    sjd->unk2C += dec_num_smpl;
    sjd->unk30 += dec_dt_len;
    sjd->unk34 += dec_num_smpl;
    sjd->trap_count += dec_num_smpl;
    sjd->trap_dt_len += dec_dt_len;
    ADXB_Reset(adxb);
}

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", adxsjd_decexec_extra);
#else
void adxsjd_decexec_extra(ADXSJD sjd) {
    not_implemented(__func__);
}
#endif

void adxsjd_decode_exec(ADXSJD sjd) {
    ADXB adxb = sjd->adxb;
    Sint16 format;

    if (ADXB_GetStat(adxb) == 0) {
        adxsjd_decexec_start(sjd);
    }

    ADXB_ExecHndl(adxb);

    if (ADXB_GetStat(adxb) == 3) {
        adxsjd_decexec_end(sjd);
    }

    format = adxb->format;

    if ((format == 0xA) || (format == 0x14) || (format == 0xB) || (format == 0xF)) {
        adxsjd_decexec_extra(sjd);
    }
}

void ADXSJD_ExecHndl(ADXSJD sjd) {
    Sint8 state = sjd->state;

    if (state == 2) {
        adxsjd_decode_exec(sjd);
    } else if (state == 1) {
        adxsjd_decode_prep(sjd);
    }
}

void ADXSJD_ExecServer() {
    ADXSJD sjd;
    Sint32 i;

    for (i = 0; i < ADXSJD_MAX_OBJ; i++) {
        sjd = &adxsjd_obj[i];

        if (sjd->used == 1) {
            ADXSJD_ExecHndl(sjd);
        }
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetDecDtLen);

Sint32 ADXSJD_GetDecNumSmpl(ADXSJD sjd) {
    return sjd->unk2C;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_SetDecPos);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetDecPos);

void ADXSJD_SetLnkSw(ADXSJD sjd, Sint32 sw) {
    sjd->lnkflg = sw;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetLnkSw);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_SetExtString);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_SetDefExtString);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_EntryFltFunc);

void ADXSJD_EntryTrapFunc(ADXSJD sjd, void (*func)(void*), void* obj) {
    sjd->trap_func = func;
    sjd->trap_obj = obj;
}

void ADXSJD_SetTrapNumSmpl(ADXSJD sjd, Sint32 samples) {
    sjd->trap_num_samples = samples;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetTrapNumSmpl);

void ADXSJD_SetTrapCnt(ADXSJD sjd, Sint32 cnt) {
    sjd->trap_count = cnt;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetTrapCnt);

void ADXSJD_SetTrapDtLen(ADXSJD sjd, Sint32 dt_len) {
    sjd->trap_dt_len = dt_len;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetTrapDtLen);

Sint32 ADXSJD_GetFormat(ADXSJD sjd) {
    return ADXB_GetFormat(sjd->adxb);
}

Sint32 ADXSJD_GetSfreq(ADXSJD sjd) {
    return ADXB_GetSfreq(sjd->adxb);
}

Sint32 ADXSJD_GetNumChan(ADXSJD sjd) {
    return ADXB_GetNumChan(sjd->adxb);
}

Sint32 ADXSJD_GetOutBps(ADXSJD sjd) {
    return ADXB_GetOutBps(sjd->adxb);
}

Sint32 ADXSJD_GetBlkSmpl(ADXSJD sjd) {
    return ADXB_GetBlkSmpl(sjd->adxb);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetBlkLen);

Sint32 ADXSJD_GetTotalNumSmpl(ADXSJD sjd) {
    return ADXB_GetTotalNumSmpl(sjd->adxb);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetCof);

Sint32 ADXSJD_GetNumLoop(ADXSJD sjd) {
    return ADXB_GetNumLoop(sjd->adxb);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetLpInsNsmpl);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetLpStartPos);
#else
Sint32 ADXSJD_GetLpStartPos(ADXSJD sjd) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetLpStartOfst);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetLpEndPos);
#else
Sint32 ADXSJD_GetLpEndPos(ADXSJD sjd) {
    not_implemented(__func__);
}
#endif

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetLpEndOfst);
#else
Sint32 ADXSJD_GetLpEndOfst(ADXSJD sjd) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetAinfLen);

Sint32 ADXSJD_GetDefOutVol(ADXSJD sjd) {
    if (ADXB_GetAinfLen(sjd->adxb) > 0 && ((Uint8)sjd->state - 2) < 2U) {
        return ADXB_GetDefOutVol(sjd->adxb);
    }

    return 0;
}

Sint16 ADXSJD_GetDefPan(ADXSJD sjd, Sint32 arg1) {
    if ((ADXB_GetAinfLen(sjd->adxb) > 0) && (((u8)sjd->state - 2) < 2U)) {
        return ADXB_GetDefPan(sjd->adxb, arg1);
    } else {
        return -0x80;
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetDataId);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetHdrLen);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetFmtBps);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_GetSpsdInfo);
#else
Sint32 ADXSJD_GetSpsdInfo(ADXSJD sjd) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_TakeSnapshot);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sjd", ADXSJD_RestoreSnapshot);
