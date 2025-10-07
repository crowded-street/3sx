#include "common.h"
#include <cri/private/libadxe/adx_crs.h>
#include <cri/private/libadxe/adx_errs.h>
#include <cri/private/libadxe/adx_stmc.h>
#include <cri/private/libadxe/cri_cvfs.h>
#include <cri/private/libadxe/svm.h>

#include <cri/cri_adxt.h>
#include <cri/sj.h>

#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ADXSTMF_MAX_OBJ 40

// data
Sint32 adxstmf_rtim_ofst = 0;
Sint32 adxstmf_rtim_num = 16;
Sint32 adxstmf_nrml_ofst = 16;
Sint32 adxstmf_nrml_num = 24;
Sint32 volatile adxstmf_execsvr_flg = 0;
Sint32 adxstmf_num_rtry = 0xFFFFFFFF;
Sint32 adxstm_sj_internal_error_cnt = 0;
ADXSTM_OBJ adxstmf_obj[ADXSTMF_MAX_OBJ] = { 0 };

// forward declarations
void ADXSTM_StopNw();

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXT_SetupRtimeNumStm);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXT_SetupNrmlNumStm);

Sint32 ADXSTM_Init() {
    memset(adxstmf_obj, 0, sizeof(adxstmf_obj));
    return 1;
}

void ADXSTM_Reset() {
    // Do nothing
}

void ADXSTM_Finish() {
    // Do nothing
}

void ADXSTMF_SetupHandleMember(ADXSTM stm, Sint32 offset, Sint32 arg2, Sint32 file_len, SJ sj) {
    Sint32 file_sct;

    ADXCRS_Lock();

    file_sct = file_len / 2048;

    // This rounds file_sct up
    if (file_len - file_sct * 2048 > 0) {
        file_sct += 1;
    }

    stm->stat = 1;
    stm->read_flg = 0;
    stm->sj = sj;
    stm->cvfs = offset;
    stm->unkC = arg2;
    stm->req_rd_size = 0x200;
    stm->unk5C = 0xFFFFF;
    stm->file_sct = file_sct;
    stm->eos = file_sct;
    stm->cur_ofst = 0;
    stm->file_len = file_len;

    if (sj != NULL) {
        stm->unk1C = stm->unk18 = stm->unk40 = SJ_GetNumData(sj, 0) + SJ_GetNumData(sj, 1);
    }

    stm->unk0 = 1;
    stm->pause = 0;
    ADXCRS_Unlock();
}

ADXSTM ADXSTMF_CreateCvfsRt(Sint32 arg0, Sint32 offset, Sint32 file_len, SJ sj) {
    ADXSTM stm = NULL;
    Sint32 i;

    for (i = 0; i < adxstmf_rtim_num; i++) {
        stm = &adxstmf_obj[adxstmf_rtim_ofst + i];

        if (stm->unk0 == 0) {
            break;
        }
    }

    if (i == adxstmf_rtim_num) {
        return NULL;
    }

    ADXSTMF_SetupHandleMember(stm, arg0, offset, file_len, sj);
    return stm;
}

ADXSTM ADXSTMF_CreateCvfs(Sint32 arg0, Sint32 offset, Sint32 file_len, SJ sj) {
    ADXSTM stm = NULL;
    Sint32 i;

    for (i = 0; i < adxstmf_nrml_num; i++) {
        stm = &adxstmf_obj[adxstmf_nrml_ofst + i];

        if (stm->unk0 == 0) {
            break;
        }
    }

    if (i == adxstmf_nrml_num) {
        return NULL;
    }

    ADXSTMF_SetupHandleMember(stm, arg0, offset, file_len, sj);
    return stm;
}

ADXSTM ADXSTM_Create(SJ sj, Sint32 offset) {
    ADXSTM stm;

    if (offset < 0x100) {
        stm = ADXSTMF_CreateCvfsRt(0, 0, 0, sj);
    } else {
        stm = ADXSTMF_CreateCvfs(0, 0, 0, sj);
    }

    return stm;
}

void ADXSTM_Destroy(ADXSTM stm) {
    if (stm != NULL) {
        ADXSTM_Stop(stm);
        ADXSTM_ReleaseFile(stm);
        stm->unk0 = 0;
        memset(stm, 0, sizeof(ADXSTM_OBJ));
    }
}

void ADXSTM_BindFileNw(ADXSTM stm, const Char8* fname, void* dir, Sint32 arg3, Sint32 file_sct) {
    SVM_Lock();
    stm->unkC = arg3;
    stm->file_len = file_sct << 11;
    stm->file_sct = file_sct;
    stm->fname = fname;
    stm->dir = dir;
    stm->unk45 = 1;
    SVM_Unlock();
}

void ADXSTM_BindFile(ADXSTM stm, const Char8* fname, void* dir, Sint32 arg3, Sint32 file_sct) {
    ADXSTM_BindFileNw(stm, fname, dir, arg3, file_sct);

    do {
        ADXT_ExecFsSvr();
    } while (stm->unk45 != 0);
}

void ADXSTM_ReleaseFileNw(ADXSTM stm) {
    ADXSTM_StopNw(stm);
    SVM_Lock();

    if (stm->unk49 == 1) {
        stm->unk46 = 1;
    }

    stm->unk45 = 0;
    SVM_Unlock();
}

void ADXSTM_ReleaseFile(ADXSTM stm) {
    ADXSTM_Stop(stm);
    ADXSTM_ReleaseFileNw(stm);

    while (stm->unk49 != 0) {
        ADXT_ExecFsSvr();
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXSTM_IsOpenedFile);

Sint32 ADXSTM_GetStat(ADXSTM stm) {
    return stm->stat;
};

Sint32 ADXSTM_Seek(ADXSTM stm, Sint32 offset) {
    stm->cur_ofst = offset;

    if (stm->file_sct < offset) {
        stm->cur_ofst = stm->file_sct;
    }

    return stm->cur_ofst;
}

Sint32 ADXSTM_Tell(ADXSTM stm) {
    if (stm->cvfs == NULL) {
        return 0;
    }

    return stm->cur_ofst;
}

void adxstm_start_sub(ADXSTM stm) {
    stm->unk34 = 0;
    stm->unk3 = 0;

    if (stm->file_sct == 0) {
        stm->stat = 3;
    } else {
        stm->stat = 2;
    }

    stm->read_flg = 0;
    stm->unk24.data = NULL;
    stm->unk24.len = 0;
    stm->unk47 = 1;
}

Sint32 ADXSTM_Start(ADXSTM stm) {
    ADXCRS_Lock();
    adxstm_start_sub(stm);
    stm->unk5C = 0xFFFFF;
    ADXCRS_Unlock();
    return 1;
}

Sint32 ADXSTM_Start2(ADXSTM stm, Sint32 arg1) {
    ADXCRS_Lock();
    adxstm_start_sub(stm);
    stm->unk5C = arg1;
    ADXCRS_Unlock();
    return 1;
}

void ADXSTM_StopNw(ADXSTM stm) {
    SVM_Lock();

    if (stm->stat == 2 && stm->read_flg == 1) {
        stm->unk48 = 1;

        if (stm->unk47 == 1) {
            stm->unk47 = 0;
        }
    } else {
        stm->stat = 1;
    }

    SVM_Unlock();
}

void ADXSTM_Stop(ADXSTM stm) {
    ADXSTM_StopNw(stm);

    do {
        ADXT_ExecFsSvr();
    } while (stm->stat != 1 || stm->unk24.data != NULL);
}

void ADXSTM_EntryEosFunc(ADXSTM stm, void (*func)(void*), void* context) {
    stm->eos_callback = func;
    stm->eos_callback_context = context;
}

void ADXSTM_SetEos(ADXSTM stm, Sint32 eos) {
    if (eos >= 0) {
        stm->eos = eos;
    } else {
        stm->eos = stm->file_sct;
    }
}

void adxstm_sj_internal_error() {
    adxstm_sj_internal_error_cnt += 1;
}

void adxstmf_stat_exec(ADXSTM stm) {
    SJCK chunk_0;
    SJCK chunk_1;
    SJ sj;
    Sint32 len;
    Sint32 cvFs_stat;
    Sint32 temp_s0_5;
    Sint32 stream_len;

    sj = stm->sj;
    cvFs_stat = cvFsGetStat(stm->cvfs);
    SVM_Lock();

    if (stm->read_flg == 1) {
        if (cvFs_stat == 1) {
            stm->read_flg = 0;
            SVM_Unlock();
            temp_s0_5 = stm->unk20 << 11;
            SJ_SplitChunk(&stm->unk24, temp_s0_5, &chunk_0, &chunk_1);
            SJ_PutChunk(sj, 1, &chunk_0);
            SJ_UngetChunk(sj, 0, &chunk_1);
            stm->cur_ofst += stm->unk20;

            stm->unk34 += temp_s0_5;
            stm->unk24.len = 0;
            stm->unk24.data = NULL;
            stream_len = stm->file_sct;

            if (stm->cur_ofst == stm->eos) {
                if (stm->eos_callback != NULL) {
                    stm->eos_callback(stm->eos_callback_context);
                }
            }

            if (stm->cur_ofst >= stream_len || ((stm->unk34 >> 11) >= stm->unk5C && stm->unk5C <= 0xFFFFE)) {
                stm->stat = 3;
            }

            stm->unk3 = 0;
            return;
        } else if (cvFs_stat == 3) {
            stm->read_flg = 0;
            SVM_Unlock();
            SJ_UngetChunk(sj, 0, &stm->unk24);
            stm->unk24.data = NULL;
            stm->unk24.len = 0;
        } else {
            SVM_Unlock();
            return;
        }

    } else {
        stm->read_flg = 1;
        stm->unk24.data = NULL;
        stm->unk24.len = 0;

        SVM_Unlock();

        if (stm->pause == 1) {
            stm->read_flg = 0;
            return;
        }

        if (stm->unk48 == 1) {
            stm->read_flg = 0;
            return;
        }
        if (stm->file_sct == 0) {
            stm->stat = 3;
            stm->unk20 = 0;
            stm->read_flg = 0;
            return;
        }

        if (sj == NULL || sj->vtbl == NULL) {
            stm->read_flg = 0;
            adxstm_sj_internal_error();
            return;
        }

        if ((stm->unk40 - SJ_GetNumData(sj, 0)) >= stm->unk1C) {
            stm->read_flg = 0;
            return;
        }

        SJ_GetChunk(sj, 0, stm->unk18, &chunk_1);
        len = chunk_1.len / 2048;
        len = MIN(len, stm->eos - stm->cur_ofst);
        len = MIN(len, stm->file_sct - stm->cur_ofst);
        len = MIN(len, stm->req_rd_size);
        cvFsSeek(stm->cvfs, stm->unkC + stm->cur_ofst, 0);
        stm->unk20 = cvFsReqRd(stm->cvfs, len, chunk_1.data);
        stm->unk24.data = chunk_1.data;
        stm->unk24.len = chunk_1.len;

        if (stm->unk20 > 0) {
            return;
        }

        SJ_UngetChunk(sj, 0, &stm->unk24);
        stm->unk24.data = NULL;
        stm->unk24.len = 0;
        stm->read_flg = 0;

        if (cvFsGetStat(stm->cvfs) != 3) {
            return;
        }
    }

    if (adxstmf_num_rtry >= 0) {
        if (stm->unk3 >= adxstmf_num_rtry) {
            stm->stat = 4;
        } else {
            stm->unk3 += 1;
        }
    }
}

void ADXSTMF_ExecHndl(ADXSTM stm) {
    void* cvfs;
    Sint32 cur_pos;

    if (stm->read_flg == 0) {
        if (stm->unk48 == 1) {
            stm->unk48 = 0;

            if (stm->unk47 == 0) {
                stm->stat = 1;
            }
        }

        if (stm->unk46 == 1) {
            cvfs = stm->cvfs;

            if (cvfs != NULL) {
                stm->cvfs = NULL;
                cvFsClose(cvfs);
            }

            stm->unk46 = 0;
            stm->unk49 = 0;
        }

        SVM_Lock();

        if (stm->unk45 == 1) {
            stm->unk49 = 1;
            SVM_Unlock();

            if (stm->cvfs == NULL) {
                stm->cvfs = cvFsOpen(stm->fname, stm->dir, 0);

                if (stm->cvfs == NULL) {
                    ADXERR_CallErrFunc2("E02110501 adxstmf_stat_exec: can't open ", stm->fname);
                    stm->stat = 4;
                    stm->unk49 = 0;
                    stm->unk45 = 0;
                    return;
                }

                cvFsSeek(stm->cvfs, 0, 2);
                cur_pos = cvFsTell(stm->cvfs);
                cvFsSeek(stm->cvfs, 0, 0);

                if (stm->file_len == 0x7FFFF800) {
                    stm->file_sct = cur_pos;
                    stm->file_len = stm->file_sct << 11;
                } else {
                    if (stm->unkC > cur_pos) {
                        stm->unkC = cur_pos;
                    }

                    if ((stm->file_sct + stm->unkC) > cur_pos) {
                        stm->file_sct = cur_pos - stm->unkC;
                        stm->file_len = stm->file_sct << 11;
                    }
                }

                ADXSTM_Seek(stm, 0);
                stm->unk45 = 0;
            }
        } else {
            SVM_Unlock();
        }

        if (stm->unk47 == 1) {
            stm->unk47 = 0;
        }
    }

    if ((stm->stat == 2) && (stm->unk49 == 1)) {
        adxstmf_stat_exec(stm);
    }
}

void ADXSTM_ExecServer() {
    ADXSTM stm;
    Sint32 i;

    if (SVM_TestAndSet(&adxstmf_execsvr_flg) == 0) {
        return;
    }

    for (i = 0; i < ADXSTMF_MAX_OBJ; i++) {
        stm = &adxstmf_obj[i];

        if (stm->unk0 == 1) {
            ADXSTMF_ExecHndl(stm);
        }
    }

    adxstmf_execsvr_flg = 0;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXSTM_GetCurOfst);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXSTM_GetBufSize);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXSTM_GetSj);

Sint32 ADXSTM_SetBufSize(ADXSTM stm, Sint32 arg1, Sint32 arg2) {
    stm->unk1C = arg1;
    stm->unk18 = arg2;
    return 1;
}

Sint32 ADXSTM_SetReqRdSize(ADXSTM stm, Sint32 size) {
    stm->req_rd_size = size;
    return 1;
}

void ADXSTM_EntryErrFunc() {
    // Do nothing
}

Sint32 ADXSTM_GetFileLen(ADXSTM stm) {
    return stm->file_len;
}

Sint32 ADXSTM_GetFileSct(ADXSTM stm) {
    return stm->file_sct;
}

void ADXSTM_SetPause(ADXSTM stm, Sint8 sw) {
    stm->pause = sw;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXSTM_GetPause);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXSTM_GetCvdfsStat);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXSTM_GetFad);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXSTM_GetFsizeSct);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXSTM_GetFsizeByte);

void ADXSTM_SetSj(ADXSTM stm, SJ sj) {
    stm->sj = sj;
    ADXCRS_Lock();
    stm->unk40 = SJ_GetNumData(sj, 0) + SJ_GetNumData(sj, 1);
    ADXCRS_Unlock();
    stm->unk18 = stm->unk40;
    stm->unk1C = stm->unk40;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXSTM_SetRdSct);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXSTM_SetOfst);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXT_SetNumRetry);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_stmc", ADXSTM_GetReadFlg);

void ADXSTM_OpenCvfs(ADXSTM stm) {
    if (stm->fname == NULL) {
        return;
    }

    if (stm->cvfs != NULL) {
        return;
    }

    stm->unk45 = 1;

    do {
        if (SVM_TestAndSet(&adxstmf_execsvr_flg) == 1) {
            ADXSTMF_ExecHndl(stm);
            adxstmf_execsvr_flg = 0;
        }
    } while (stm->unk45 != 0);
}
