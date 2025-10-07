#include "common.h"
#include <cri/private/libadxe/adx_amp.h>
#include <cri/private/libadxe/adx_crs.h>
#include <cri/private/libadxe/adx_dcd3.h>
#include <cri/private/libadxe/adx_errs.h>
#include <cri/private/libadxe/adx_inis.h>
#include <cri/private/libadxe/adx_rnap.h>
#include <cri/private/libadxe/adx_sjd.h>
#include <cri/private/libadxe/adx_stmc.h>
#include <cri/private/libadxe/lsc.h>
#include <cri/private/libadxe/ps2_rna.h>
#include <cri/private/libadxe/structs.h>

#include <cri/cri_adxt.h>
#include <cri/sj.h>

#include <string.h>

// data
Sint32 adxt_time_mode = 0;
Sint32 adxt_tsvr_enter_cnt = 0;
Sint32 adxt_def_svrfreq = 0;
void (*ahxdetachfunc)(ADXT) = NULL;
void (*ac3detachfunc)(ADXT) = NULL;
void (*ac3stopfunc)(ADXT) = NULL;
void (*pl2detachfunc)(ADXT) = NULL;
Sint32 adxt_time_adjust_cnt = 0;
Sint32 adxt_time_adjust_sw = 1;
Sint32 adxt_svrcnt = 0;
Sint32 adxt_svrcnt_sjd = 0;
Sint32 adxt_svrcnt_rna = 0;
Sint32 adxt_svrcnt_adxf = 0;
Sint32 adxt_svrcnt_adxstm = 0;
Sint32 adxt_svrcnt_hndl = 0;
Sint32 adxt_mviop_f = 0;
Sint32 adxt_mviop_d = 0;
Sint32 adxt_mvtmp_d = 0;
Sint32 adxt_time_unit = 0;
Float32 adxt_diff_av = 0;

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B460);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B480);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B4A0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", adxt_disp_rna_stat);

ADXT ADXT_Create(Sint32 maxnch, void* work, Sint32 worksize) {
    ADXT adxt;
    Sint32 _maxnch;
    Sint32 i;
    Sint32 ix;
    intptr_t aligned_work;
    ptrdiff_t size;
    ptrdiff_t ibuf_len;

    aligned_work = ((uintptr_t)work + 0x40 - 1) & ~(0x40 - 1);
    size = worksize - (aligned_work - (uintptr_t)work);

    if ((maxnch < 0) || (work == NULL) || (worksize < 0)) {
        ADXERR_CallErrFunc1("E02080804 ADXT_Create: parameter error");
        return NULL;
    }

    _maxnch = (maxnch != 4) ? maxnch : 1;

    for (ix = 0; ix < ADXT_MAX_OBJ; ix++) {
        if (!adxt_obj[ix].used) {
            break;
        }
    }

    if (ix == ADXT_MAX_OBJ) {
        return NULL;
    }

    adxt = &adxt_obj[ix];
    memset(adxt, 0, sizeof(ADX_TALK));

    adxt->maxnch = _maxnch;
    adxt->ibuf = (Sint8*)(aligned_work + ADXT_CALC_OBUFSIZE(_maxnch));
    adxt->obuf = (s16*)aligned_work;
    adxt->obufsize = ADXT_OBUF_SIZE;
    adxt->obufdist = ADXT_OBUF_DIST;
    adxt->ibufxlen = ADXT_IBUF_XLEN;
    ibuf_len = size - ADXT_CALC_OBUFSIZE(_maxnch) - 0x124;
    adxt->ibuflen = (Sint32)(ibuf_len / 0x800 * 0x800);

    adxt->sji = NULL;
    adxt->unkB0 = adxt->ibuf + (adxt->ibuflen + adxt->ibufxlen);
    adxt->sjf = SJRBF_Create(adxt->ibuf, adxt->ibuflen, adxt->ibufxlen);

    if (adxt->sjf == NULL) {
        ADXT_Destroy(adxt);
        return NULL;
    }

    adxt->stm = ADXSTM_Create(adxt->sjf, 0);

    if (adxt->stm == NULL) {
        ADXT_Destroy(adxt);
        return NULL;
    }

    for (i = 0; i < _maxnch; i++) {
        adxt->sjo[i] = SJRBF_Create(
            (Sint8*)(adxt->obuf + adxt->obufdist * i), adxt->obufsize * 2, (adxt->obufdist - adxt->obufsize) * 2);

        if (adxt->sjo[i] == NULL) {
            ADXT_Destroy(adxt);
            return NULL;
        }
    }

    adxt->sjd = ADXSJD_Create(adxt->sjf, _maxnch, adxt->sjo);

    if (adxt->sjd == NULL) {
        ADXT_Destroy(adxt);
        return NULL;
    }

    adxt->rna = ADXRNA_Create(adxt->sjo, _maxnch);

    if (adxt->rna == NULL) {
        ADXT_Destroy(adxt);
        return NULL;
    }

    adxt->lsc = LSC_Create(adxt->sjf);

    if (adxt->lsc == NULL) {
        ADXT_Destroy(adxt);
        return NULL;
    }

    LSC_SetStmHndl(adxt->lsc, adxt->stm);
    ADXCRS_Lock();

    adxt->outvol = 0;
    adxt->maxsct = adxt->ibuflen / 0x800;
    adxt->svrfreq = adxt_def_svrfreq;
    adxt->minsct = (adxt->maxsct << 16 >> 16) * 0.85f;

    for (i = 0; i < _maxnch; i++) {
        adxt->outpan[i] = ADXT_PAN_AUTO;
    }

    adxt->unk46 = 0;
    adxt->lpflg = 1;
    adxt->trp = 0;
    adxt->wpos = 0;
    adxt->mofst = 0;
    adxt->ercode = 0;
    adxt->edecpos = 0;
    adxt->edeccnt = 0;
    adxt->eshrtcnt = 0;
    adxt->autorcvr = 1;
    adxt->pause_flag = 0;
    adxt->time_ofst = 0;
    ADXT_SetLnkSw(adxt, 0);
    adxt->used = 1;
    ADXCRS_Unlock();
    return adxt;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_Create3D);

void adxt_detach_ahx(ADXT adxt) {
    if (ahxdetachfunc != NULL) {
        ahxdetachfunc(adxt);
    }
}

void adxt_detach_ac3(ADXT adxt) {
    if (ac3detachfunc != NULL) {
        ac3detachfunc(adxt);
    }
}

void adxt_detach_pl2(ADXT adxt) {
    if (pl2detachfunc != NULL) {
        pl2detachfunc(adxt);
    }
}

void ADXT_Destroy(ADXT adxt) {
    Sint32 i;
    ADXRNA rna;
    ADXSJD sjd;
    ADXSTM stm;
    LSC lsc;
    SJ sjf;
    void* amp;
    SJ sj;
    SJ ampsj;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080805 ADXT_Destroy: parameter error");
        return;
    }

    adxt_detach_ahx(adxt);
    adxt_detach_ac3(adxt);
    adxt_detach_pl2(adxt);

    if (adxt->used == 1) {
        ADXT_Stop(adxt);
    }

    rna = adxt->rna;

    if (rna != NULL) {
        adxt->rna = NULL;
        ADXRNA_Destroy(rna);
    }

    sjd = adxt->sjd;

    if (sjd != NULL) {
        adxt->sjd = NULL;
        ADXSJD_Destroy(sjd);
    }

    stm = adxt->stm;

    if (stm != NULL) {
        adxt->stm = NULL;
        ADXSTM_EntryEosFunc(stm, 0, 0);
        ADXSTM_Destroy(stm);
    }

    lsc = adxt->lsc;

    if (lsc != NULL) {
        adxt->lsc = NULL;
        LSC_Destroy(lsc);
    }

    ADXCRS_Lock();

    sjf = adxt->sjf;

    if (sjf != NULL) {
        adxt->sjf = NULL;
        SJ_Destroy(sjf);
    }

    for (i = 0; i < adxt->maxnch; i++) {
        sj = adxt->sjo[i];

        if (sj != NULL) {
            adxt->sjo[i] = NULL;
            SJ_Destroy(sj);
        }

        ampsj = adxt->ampsji[i];

        if (ampsj != NULL) {
            adxt->ampsji[i] = NULL;
            SJ_Destroy(ampsj);
        }

        ampsj = adxt->ampsjo[i];

        if (ampsj != NULL) {
            adxt->ampsjo[i] = NULL;
            SJ_Destroy(ampsj);
        }
    }

    amp = adxt->amp;

    if (amp != NULL) {
        adxt->amp = NULL;
        ADXAMP_Destroy(amp);
    }

    memset(adxt, 0, sizeof(ADX_TALK));
    adxt->used = 0;

    ADXCRS_Unlock();
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_DestroyAll);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_CloseAllHandles);

void adxt_start_sj(ADXT adxt, SJ sj) {
    Sint32 i;

    for (i = 0; i < adxt->maxnch; i++) {
        SJ_Reset(adxt->sjo[i]);
    }

    ADXSJD_SetInSj(adxt->sjd, sj);
    adxt->sji = sj;
    ADXSJD_Start(adxt->sjd);
    adxt->stat = ADXT_STAT_DECINFO;
    adxt->lesct = 0x7FFFFFFF;
    adxt->trpnsmpl = -1;
    adxt->lpcnt = 0;
    adxt->pstready_flag = 0;
    adxt->tvofst = 0;
    adxt->decofst = 0;
    adxt->svcnt = adxt_vsync_cnt;

    if (adxt->amp != NULL) {
        ADXAMP_Start(adxt->amp);
    }
}

void adxt_start_stm(ADXT adxt, const Char8* fname, void* dir, s32 arg3, Sint32 file_sct) {
    ADXSTM_SetBufSize(adxt->stm, adxt->minsct << 11, adxt->maxsct << 11);
    ADXSTM_SetEos(adxt->stm, 25);
    ADXSTM_EntryEosFunc(adxt->stm, NULL, NULL);
    ADXSTM_Seek(adxt->stm, 0);
    ADXSTM_StopNw(adxt->stm);
    ADXSTM_ReleaseFileNw(adxt->stm);
    ADXSTM_BindFileNw(adxt->stm, fname, dir, arg3, file_sct);
    ADXSTM_Start(adxt->stm);
    adxt_start_sj(adxt, adxt->sjf);
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B508);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_StartSj);

void ADXT_StopWithoutLsc(ADXT adxt) {
    SJ sj;
    ADXAMP amp;

    ADXCRS_Lock();
    ADXRNA_SetTransSw(adxt->rna, 0);
    ADXRNA_SetPlaySw(adxt->rna, 0);
    PS2RNA_SetDiscardSw(adxt->rna, 1);
    ADXSJD_Stop(adxt->sjd);

    if (adxt->pmode == 2) {
        sj = adxt->sji;

        if (sj != NULL) {
            adxt->sji = NULL;
            SJ_Destroy(sj);
        }
    }

    amp = adxt->amp;

    if (amp != NULL) {
        ADXAMP_Stop(amp);
    }

    adxt->sji = NULL;
    adxt->stat = ADXT_STAT_STOP;
    adxt->unkAC = 0;
    ADXCRS_Unlock();
}

void ADXT_Stop(ADXT adxt) {
    ADXSTM stm;
    SJ sj;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080813 ADXT_Stop: parameter error");
        return;
    }

    if (ac3stopfunc != NULL) {
        ADXSJD_Stop(adxt->sjd);
        ac3stopfunc(adxt);
    }

    stm = adxt->stm;

    if (stm != NULL) {
        ADXSTM_ReleaseFileNw(stm);
    }

    if (adxt->pmode == 4) {
        LSC_Stop(adxt->lsc);
        sj = adxt->sji;

        if (sj != NULL) {
            SJ_Reset(sj);
        }
    }

    ADXT_StopWithoutLsc(adxt);
}

Sint32 ADXT_GetStat(ADXT adxt) {
    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080814 ADXT_GetStat: parameter error");
        return ADXT_ERR_SHRTBUF;
    }

    return adxt->stat;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_SetTimeMode);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetTimeSfreq);

void ADXT_GetTimeSfreq2(ADXT adxt, Sint32* ncount, Sint32* tscale) {
    Uint8 stat = adxt->stat;

    if ((Uint32)(stat - 3) < 2) {
        *tscale = ADXSJD_GetSfreq(adxt->sjd);
        *ncount = (ADXSJD_GetDecNumSmpl(adxt->sjd) - (ADXRNA_GetNumData(adxt->rna) + ADXT_GetNumSmplObuf(adxt, 0))) +
                  adxt->decofst;
    } else if (adxt->stat == ADXT_STAT_PLAYEND) {
        *ncount = ADXSJD_GetTotalNumSmpl(adxt->sjd);
        *tscale = ADXSJD_GetSfreq(adxt->sjd);
        *ncount *= (16 / ADXSJD_GetOutBps(adxt->sjd));
        *ncount += adxt->decofst;
    } else {
        *ncount = 0;
        *tscale = 1;
    }

    *ncount += adxt->time_ofst;
}

void adxt_time_adjust_trap() {
    adxt_time_adjust_cnt += 1;
}

void ADXT_GetTime(ADX_TALK* adxt, Sint32* ncount, Sint32* tscale) {
    Sint32 sp0;
    Sint32 sp4;
    Sint32 temp_s1;
    Uint8 stat;
    Sint32 temp;

    if ((adxt == NULL) || (ncount == NULL) || (tscale == NULL)) {
        ADXERR_CallErrFunc1("E02080815 ADXT_GetTime: parameter error");
        return;
    }

    if (adxt_time_mode == 0) {
        ADXT_GetTimeSfreq2(adxt, ncount, tscale);
        return;
    }

    adxt_diff_av = 0.0f;
    stat = adxt->stat;

    if ((Uint32)(stat - 3) < 2) {
        if (adxt->pause_flag == 0) {
            *ncount = ((adxt_vsync_cnt - adxt->svcnt) * 0x64) + adxt->tvofst;
        } else {
            *ncount = adxt->tvofst;
        }

        ADXT_GetTimeSfreq2(adxt, &sp0, &sp4);
        adxt_diff_av = (((Float32)sp0 / sp4) - ((Float32)*ncount / adxt_time_unit)) * 1000.0f;

        if ((adxt_diff_av > 60.0f) || (adxt_diff_av < -60.0f)) {
            if (adxt_time_adjust_sw == 1) {
                temp_s1 = adxt_time_mode;
                adxt_time_mode = 0;
                ADXT_GetTime(adxt, &sp0, &sp4);
                adxt_time_mode = temp_s1;
                adxt_time_adjust_trap();
            }

            adxt->tvofst = (Float32)sp0 / sp4 * adxt_time_unit;
            adxt->svcnt = adxt_vsync_cnt;
        }
    } else if (adxt->stat == ADXT_STAT_PLAYEND) {
        sp0 = ADXSJD_GetTotalNumSmpl(adxt->sjd);
        sp4 = ADXSJD_GetSfreq(adxt->sjd);
        sp0 *= (0x10 / ADXSJD_GetOutBps(adxt->sjd));
        *ncount = (Float32)sp0 / sp4 * adxt_time_unit;
        temp = adxt->tvofst + 1;
        *ncount += temp;
    } else {
        *ncount = 0;
    }

    *ncount += adxt->time_ofst;
    *tscale = adxt_time_unit;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetTimeReal);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B5A8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetNumSmpl);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B5D8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetSfreq);

#if defined(TARGET_PS2)
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B608);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetNumChan);
#else
Sint32 ADXT_GetNumChan(ADXT adxt) {
    not_implemented(__func__);
}
#endif

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B638);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetHdrLen);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B668);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetFmtBps);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B698);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B6C8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_SetOutPan);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B6F8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetOutPan);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B728);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_SetOutBalance);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B758);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetOutBalance);

void ADXT_SetOutVol(ADXT adxt, Sint32 vol) {
    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080823 ADXT_SetOutVol: parameter error\0\0\0\0");
        return;
    }

    adxt->outvol = vol;
    ADXRNA_SetOutVol(adxt->rna, adxt->outvol + ADXSJD_GetDefOutVol(adxt->sjd));
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B7B8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetOutVol);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetDefOutVol);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetDefOutPan);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetDataId);

void ADXT_SetDefSvrFreq(Sint32 freq) {
    adxt_def_svrfreq = freq;
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B7E8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_SetSvrFreq);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B818);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_SetReloadTime);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B848);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_SetReloadSct);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B878);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetNumSctIbuf);

Sint32 ADXT_GetNumSmplObuf(ADXT adxt, Sint32 chno) {
    SJ sj;

    if ((adxt == NULL) || (chno < 0)) {
        ADXERR_CallErrFunc1("E02080837 ADXT_GetNumSmplObuf: parameter error");
        return ADXT_ERR_SHRTBUF;
    }

    sj = adxt->sjo[chno];

    if (sj != NULL) {
        return SJ_GetNumData(sj, 1) / 2;
    }

    return ADXT_ERR_OK;
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B8D8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetIbufRemainTime);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B910);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_IsIbufSafety);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_SetAutoRcvr);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B940);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_IsCompleted);

void ADXT_ExecServer() {
    Sint32 i;

    ADXCRS_Lock();

    if (adxt_tsvr_enter_cnt != 0) {
        ADXCRS_Unlock();
        return;
    }

    adxt_tsvr_enter_cnt = 1;
    ADXCRS_Unlock();

    ADXCRS_Lock();
    ADXSJD_ExecServer();
    adxt_tsvr_enter_cnt = 2;

    for (i = 0; i < ADXT_MAX_OBJ; i++) {
        if (adxt_obj[i].used == 1) {
            ADXT_ExecHndl(&adxt_obj[i]);
        }
    }

    adxt_tsvr_enter_cnt = 3;
    ADXRNA_ExecServer();
    adxt_tsvr_enter_cnt = 0;
    ADXCRS_Unlock();
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_ExecDecServer);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B970);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetErrCode);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B9A0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_ClearErrCode);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055B9D0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetLpCnt);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055BA00);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_SetLpFlg);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055BA30);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetInputSj);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055BA60);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_SetWaitPlayStart);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055BA98);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_IsReadyPlayStart);

void ADXT_Pause(ADXT adxt, Sint32 sw) {
    Sint32 ncount;
    Sint32 tscale;
    Sint32 time_mode_temp;
    Sint8 stat;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080846 ADXT_Pause: parameter error");
        return;
    }

    stat = adxt->stat;

    if (sw == adxt->pause_flag) {
        return;
    }

    ADXCRS_Lock();
    adxt->pause_flag = sw;

    if ((Uint8)(stat - 3) < 2) {
        if (sw == 1) {
            ADXRNA_SetPlaySw(adxt->rna, 0);
        } else {
            ADXRNA_SetPlaySw(adxt->rna, 1);
            adxt->svcnt = adxt_vsync_cnt;
        }

        time_mode_temp = adxt_time_mode;
        adxt_time_mode = 0;
        ADXT_GetTime(adxt, &ncount, &tscale);
        adxt_time_mode = time_mode_temp;
        adxt->tvofst = (Float32)ncount / tscale * adxt_time_unit;
    }

    ADXCRS_Unlock();
}

Sint32 ADXT_GetStatPause(ADXT adxt) {
    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080847 ADXT_GetStatPause: parameter error");
        return 0;
    }

    return adxt->pause_flag;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_PauseAll);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetStatPauseAll);

void ADXT_SetTranspose(ADXT adxt, Sint32 transps, Sint32 detune) {
    // Do nothing
}

void ADXT_GetTranspose(ADXT adxt, Sint32* transps, Sint32* detune) {
    // Do nothing
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetStm);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_TermSupply);

void ADXT_SetDrctLvl(ADXT adxt, Sint32 drctlvl) {
    // Do nothing
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetDrctLvl);

void ADXT_SetFx(ADXT adxt, Sint32 fxch, Sint32 fxlvl) {
    // Do nothing
}

void ADXT_GetFx(ADXT adxt, Sint32* fxch, Sint32* fxlvl) {
    // Do nothing
}

void ADXT_SetFilter(ADXT adxt, Sint32 coff, Sint32 q) {
    // Do nothing
}

void ADXT_GetFilter(ADXT adxt, Sint32* coff, Sint32* q) {
    // Do nothing
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_EntryErrFunc);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_DiscardSmpl);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_SetTimeOfst);

void ADXT_SetLnkSw(ADXT adxt, Sint32 sw) {
    ADXSJD sjd;

    adxt->lnkflg = sw;
    sjd = adxt->sjd;

    if (sjd != NULL) {
        ADXSJD_SetLnkSw(sjd, sw);
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetLnkSw);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_EntryFltFunc);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055BB28);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetDecNumSmpl);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_IsHeader);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_IsEndcode);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_InsertSilence);

void ADXT_SetOutputMono(Sint32 flag) {
    ADX_SetDecodeSteAsMonoSw(flag);
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", D_0055BB58);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_SetKeyString);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_SetDefKeyString);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk", ADXT_GetRna);
