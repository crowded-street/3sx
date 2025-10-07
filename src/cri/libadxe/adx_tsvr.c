#include "common.h"
#include <cri/private/libadxe/structs.h>

#include <cri/private/libadxe/adx_amp.h>
#include <cri/private/libadxe/adx_errs.h>
#include <cri/private/libadxe/adx_rnap.h>
#include <cri/private/libadxe/adx_sjd.h>
#include <cri/private/libadxe/adx_stmc.h>
#include <cri/private/libadxe/adx_tlk.h>
#include <cri/private/libadxe/lsc.h>

#include <cri/cri_adxt.h>
#include <string.h>

// Have to declare this one locally, because other files expect it to be non-volatile
extern volatile Sint32 adxt_vsync_cnt;

// data
Sint32 adxt_dbg_nch = 0;
Sint32 adxt_dbg_ndt = 0;
Sint32 adxt_dbg_rna_ndata = 0;

#if defined(TARGET_PS2)
void adxt_trap_entry_lps(ADXT adxt);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tsvr", adxt_trap_entry_lps);
#else
void adxt_trap_entry_lps(ADXT adxt) {
    not_implemented(__func__);
}
#endif

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tsvr", D_0055BCD0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tsvr", adxt_trap_entry);

#if defined(TARGET_PS2)
void adxt_eos_entry(ADXT adxt);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tsvr", adxt_eos_entry);
#else
void adxt_eos_entry(ADXT adxt) {
    not_implemented(__func__);
}
#endif

void adxt_set_outpan(ADXT adxt) {
    Sint32 i;
    Sint32 num_chan;
    Sint32 def_pan[2];

    num_chan = ADXSJD_GetNumChan(adxt->sjd);

    for (i = 0; i < 2; i++) {
        def_pan[i] = ADXSJD_GetDefPan(adxt->sjd, i);
    }

    if (num_chan == 1) {
        if (adxt->outpan[0] == -0x80) {
            if (def_pan[0] == -0x80) {
                ADXRNA_SetOutPan(adxt->rna, 0, 0);
            } else {
                goto lbl1;
            lbl2:
                ADXRNA_SetOutPan(adxt->rna, 0, adxt->outpan[0]);
            }
        } else {
            if (def_pan[0] == -0x80) {
                goto lbl2;
            lbl1:
                ADXRNA_SetOutPan(adxt->rna, 0, def_pan[0]);
            } else {
                ADXRNA_SetOutPan(adxt->rna, 0, (Sint16)(adxt->outpan[0] & 0xFFFF) + def_pan[0]);
            }
        }
    } else {
        if (adxt->outpan[0] == -0x80) {
            if (def_pan[0] == -0x80) {
                ADXRNA_SetOutPan(adxt->rna, 0, -0xF);
            } else {
                goto lbl3;
            lbl4:
                ADXRNA_SetOutPan(adxt->rna, 0, adxt->outpan[0]);
            }
        } else {
            if (def_pan[0] == -0x80) {
                goto lbl4;
            lbl3:
                ADXRNA_SetOutPan(adxt->rna, 0, def_pan[0]);
            } else {
                ADXRNA_SetOutPan(adxt->rna, 0, adxt->outpan[0] + def_pan[0]);
            }
        }

        if (adxt->outpan[1] == -0x80) {
            if (def_pan[1] == -0x80) {
                ADXRNA_SetOutPan(adxt->rna, 1, 0xF);
            } else {
                goto lbl5;
            lbl6:
                ADXRNA_SetOutPan(adxt->rna, 1, adxt->outpan[1]);
            }
        } else {
            if (def_pan[1] == -0x80) {
                goto lbl6;
            lbl5:
                ADXRNA_SetOutPan(adxt->rna, 1, def_pan[1]);
            } else {
                ADXRNA_SetOutPan(adxt->rna, 1, (Sint16)(adxt->outpan[1] & 0xFFFF) + def_pan[1]);
            }
        }
    }
}

#if defined(TARGET_PS2)
void adxt_nlp_trap_entry(ADXT adxt);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tsvr", adxt_nlp_trap_entry);
#else
void adxt_nlp_trap_entry(ADXT adxt) {
    not_implemented(__func__);
}
#endif

void adxt_stat_decinfo(ADXT adxt) {
    Sint32 transpose = 0;
    Sint32 detune = 0;
    ADXSJD sjd = adxt->sjd;
    Sint32 div = 2048;
    Char8 num_chan_str[32];
    Sint32 num_smpl;
    Sint32 sfreq;
    Sint32 num_chan;
    Sint32 num_loop;
    Sint32 blk_smpl;
    Sint32 max_dec_smpl;
    Sint32 lp_end_ofst;
    Sint32 lp_skiplen;

    if ((adxt->pmode < 2U) && (adxt->unkAC == 1)) {
        if (ADXSTM_GetStat(adxt->stm) == 2) {
            return;
        }

        if (adxt->sjf != NULL) {
            SJ_Reset(adxt->sjf);
        }

        adxt_start_stm(adxt, adxt->padB4, adxt->padB8, adxt->padBC, adxt->padC0);
        adxt->unkAC = 0;
    }

    if (ADXSJD_GetStat(sjd) != 2) {
        return;
    }

    num_chan = ADXSJD_GetNumChan(sjd);

    if (adxt->maxnch < num_chan) {
        ADXERR_ItoA2(num_chan, adxt->maxnch, num_chan_str, 16);
        ADXERR_CallErrFunc2("E9081001 adxt_stat_decinfo: can't play this number of channels", num_chan_str);
        ADXT_Stop(adxt);
        return;
    }

    sfreq = ADXSJD_GetSfreq(sjd);
    num_loop = ADXSJD_GetNumLoop(sjd);

    if (num_loop > 0) {
        adxt->maxdecsmpl = (sfreq / adxt->svrfreq) * 3;
    } else {
        adxt->maxdecsmpl = ((sfreq / adxt->svrfreq) * 3) / 2;
    }

    blk_smpl = ADXSJD_GetBlkSmpl(sjd) * 2;
    max_dec_smpl = ((adxt->maxdecsmpl + blk_smpl) / blk_smpl) * blk_smpl;
    adxt->maxdecsmpl = max_dec_smpl;
    ADXSJD_SetMaxDecSmpl(sjd, max_dec_smpl);

    if (num_loop > 0) {
        if (adxt->pmode == 2) {
            adxt->lp_skiplen = 0;
        } else {
            lp_end_ofst = ADXSJD_GetLpEndOfst(sjd);
            adxt->lp_skiplen = div - (lp_end_ofst % div);
            lp_end_ofst += 0x7FF;
            adxt->lp_skiplen %= div;
            adxt->lesct = lp_end_ofst / div;
            ADXSTM_SetEos(adxt->stm, adxt->lesct);
            ADXSTM_EntryEosFunc(adxt->stm, adxt_eos_entry, adxt);
        }

        ADXSJD_GetLpEndPos(sjd);
        adxt->trpnsmpl = ADXSJD_GetLpStartPos(sjd);
        ADXSJD_SetTrapNumSmpl(sjd, adxt->trpnsmpl);
        ADXSJD_SetTrapDtLen(sjd, 0);
        ADXSJD_SetTrapCnt(sjd, 0);
        ADXSJD_EntryTrapFunc(sjd, adxt_trap_entry_lps, adxt);
    } else {
        if (adxt->stm != NULL) {
            ADXSTM_SetEos(adxt->stm, SJCK_LEN_MAX);
        }

        ADXSJD_SetTrapNumSmpl(sjd, ADXSJD_GetTotalNumSmpl(sjd));
        ADXSJD_SetTrapDtLen(sjd, 0);
        ADXSJD_SetTrapCnt(sjd, 0);
        ADXSJD_EntryTrapFunc(sjd, adxt_nlp_trap_entry, adxt);
    }

    sfreq = ADXSJD_GetSfreq(sjd);
    num_chan = ADXSJD_GetNumChan(sjd);
    num_smpl = ADXSJD_GetTotalNumSmpl(sjd);
    ADXRNA_SetBitPerSmpl(adxt->rna, ADXSJD_GetOutBps(sjd));
    ADXRNA_SetSfreq(adxt->rna, sfreq);
    ADXRNA_SetNumChan(adxt->rna, num_chan);
    ADXRNA_SetTotalNumSmpl(adxt->rna, num_smpl);
    ADXRNA_SetOutVol(adxt->rna, adxt->outvol + ADXSJD_GetDefOutVol(adxt->sjd));
    ADXT_GetTranspose(adxt, &transpose, &detune);

    if ((transpose != 0) || (detune != 0)) {
        ADXT_SetTranspose(adxt, transpose, detune);
    }

    adxt_set_outpan(adxt);

    if (adxt->amp != NULL) {
        ADXAMP_SetSfreq(adxt->amp, sfreq);
    }

    if (ADXSJD_GetFormat(sjd) == 2) {
        ADXRNA_SetStmHdInfo(adxt->rna, ADXSJD_GetSpsdInfo(sjd));
    }

    ADXRNA_SetTransSw(adxt->rna, 1);
    adxt->stat = ADXT_STAT_PREP;
}

void adxt_stat_prep(ADXT adxt) {
    ADXRNA rna;
    ADXSJD sjd;
    Sint32 num_data;
    Sint32 num_room;
    Sint32 num_chan;
    Sint32 i;
    Sint32 size;
    SJCK chunk_info;

    rna = adxt->rna;
    sjd = adxt->sjd;

    num_data = ADXRNA_GetNumData(rna);
    num_room = ADXRNA_GetNumRoom(rna);

    if ((num_data >= (adxt->maxdecsmpl << 1)) || (ADXSJD_GetBlkSmpl(sjd) >= num_room) ||
        (ADXSJD_GetStat(adxt->sjd) == 3)) {
        if (adxt->pstwait_flag == 0) {
            if (adxt->pause_flag == 0) {
                ADXRNA_SetPlaySw(rna, 1);
                adxt->tvofst = 0;
                adxt->svcnt = adxt_vsync_cnt;
            }

            adxt->stat = ADXT_STAT_PLAYING;
        }

        adxt->pstready_flag = 1;
    }

    if (ADXSJD_GetStat(adxt->sjd) != 3) {
        return;
    }

    num_chan = ADXT_GetNumChan(adxt);
    size = (adxt->maxdecsmpl * num_chan) << 1;

    for (i = 0; i < num_chan; i++) {
        SJ sj_obj = adxt->sjo[i];
        SJ_GetChunk(sj_obj, 0, size, &chunk_info);
        memset(chunk_info.data, 0, chunk_info.len);
        SJ_PutChunk(sj_obj, 1, &chunk_info);
    }
}

void adxt_stat_playing(ADXT adxt) {
    Sint32 i;
    Sint32 num_chan;

    if (ADXSJD_GetStat(adxt->sjd) != 3) {
        return;
    }

    num_chan = ADXSJD_GetNumChan(adxt->sjd);
    adxt_dbg_nch = num_chan;

    for (i = 0; i < num_chan; i++) {
        SJ sj_obj = adxt->sjo[i];
        adxt_dbg_ndt = SJ_GetNumData(sj_obj, 1);

        if (adxt_dbg_ndt >= 64) {
            break;
        }
    }

    if (i == num_chan) {
        adxt->flush_nsmpl = 0;
        adxt->stat = ADXT_STAT_DECEND;
    }
}

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tsvr", adxt_stat_decend);
#else
void adxt_stat_decend(ADXT adxt) {
    not_implemented(__func__);
}
#endif

void adxt_stat_playend(ADXT adxt) {
    // Do nothing
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tsvr", adxt_RcvrReplay);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tsvr", ADXT_ExecErrChk);

void ADXT_ExecRdErrChk(ADXT adxt) {
    if ((adxt->stm != NULL) && (ADXSTM_GetStat(adxt->stm) == 4)) {
        adxt->ercode = -1;
        adxt->stat = ADXT_STAT_ERROR;
    }

    if ((adxt->lsc != NULL) && (LSC_GetStat(adxt->lsc) == 3)) {
        adxt->ercode = -1;
        adxt->stat = ADXT_STAT_ERROR;
    }
}

void ADXT_ExecRdCompChk(ADXT adxt) {
    Sint8 pmode;

    if ((adxt->stm == NULL) || (ADXT_GetStat(adxt) == 0)) {
        return;
    }

    pmode = adxt->pmode;

    switch (pmode) {
    case 0:
    case 1:
        if (ADXSTM_GetStat(adxt->stm) == 3) {
            ADXSJD_TermSupply(adxt->sjd);
        }

        break;

    case 2:
        ADXSJD_TermSupply(adxt->sjd);
        break;

    case 3:
    case 4:
    default:
        // Do nothing
        break;
    }
}

void ADXT_ExecHndl(ADXT adxt) {
    Sint8 stat;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080842 ADXT_ExecHndl: parameter error");
        return;
    }

    stat = adxt->stat;

    if (stat == 3) {
        adxt_stat_playing(adxt);
    } else if (stat == 1) {
        adxt_stat_decinfo(adxt);
    } else if (stat == 2) {
        adxt_stat_prep(adxt);
    } else if (stat == 4) {
        adxt_stat_decend(adxt);
    } else if (stat == 5) {
        adxt_stat_playend(adxt);
    }

    ADXT_ExecRdCompChk(adxt);
    ADXT_ExecRdErrChk(adxt);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tsvr", ADXT_GetStatRead);
