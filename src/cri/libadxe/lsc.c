#include "common.h"
#include <cri/private/libadxe/structs.h>

#include <cri/private/libadxe/lsc.h>
#include <cri/private/libadxe/lsc_crs.h>
#include <cri/private/libadxe/lsc_err.h>
#include <cri/private/libadxe/lsc_ini.h>
#include <cri/private/libadxe/lsc_svr.h>

#include <cri/ee/cri_xpt.h>
#include <cri/sj.h>

#include <string.h>

LSC lsc_Alloc() {
    LSC lsc = NULL;
    Sint32 i;

    for (i = 0; i < LSC_MAX_OBJ; i++) {
        if (lsc_obj[i].used == 0) {
            lsc = &lsc_obj[i];
            break;
        }
    }

    return lsc;
}

LSC LSC_Create(SJ sj) {
    LSC lsc;
    Sint32 lock;
    Sint32 i;

    if (sj == NULL) {
        LSC_CallErrFunc("E0001: Illigal parameter=sj (LSC_Create)\n");
        return NULL;
    }

    LSC_LockCrs(&lock);
    lsc = lsc_Alloc();

    if (lsc == NULL) {
        LSC_CallErrFunc("E0002: Not enough instance (LSC_Create)\n");
    } else {
        lsc->stat = 0;
        lsc->sj = sj;

        lsc->unk18 = SJ_GetNumData(sj, 0) + SJ_GetNumData(sj, 1);
        lsc->flow_limit = (lsc->unk18 * 8) / 10;

        for (i = 0; i < LSC_MAX_STM; i++) {
            lsc->stm[i].stat = 0;
        }

        lsc->used = 1;
    }

    LSC_UnlockCrs(&lock);
    return lsc;
}

void LSC_Destroy(LSC lsc) {
    if (lsc != NULL) {
        LSC_Stop(lsc);
        lsc->used = 0;
        memset(lsc, 0, sizeof(LSC_OBJ));
    }
}

void LSC_SetStmHndl(LSC lsc, ADXSTM stm) {
    lsc->stm_hndl = stm;
}

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/lsc", LSC_EntryFname);
#else
Sint32 LSC_EntryFname(LSC lsc) {
    not_implemented(__func__);
}
#endif

Sint32 LSC_EntryFileRange(LSC lsc, const Char8* fname, void* dir, Sint32 ofst, Sint32 fnsct) {
    LSC_STM* stm;
    Sint32 id;
    size_t fname_length;
    Sint32 i;
    Sint32 prev_stm_index;

    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return -1;
    }

    if (lsc->num_stm >= LSC_MAX_STM) {
        return -1;
    }

    if (fname == NULL) {
        LSC_CallErrFunc("E0011: Illigal parameter fname=%s\n", fname);
        return -1;
    }

    stm = &lsc->stm[lsc->last_stm_index];
    prev_stm_index = (lsc->last_stm_index + LSC_MAX_STM - 1) % LSC_MAX_STM;
    stm->fname = fname;
    id = S32_INCREMENT_WRAPAROUND(lsc->stm[prev_stm_index].id);
    stm->id = id;

    fname_length = strlen(fname);
    stm->unk8 = 0;

    for (i = 0; i < fname_length; i++) {
        stm->unk8 += (Uint8)fname[i];
    }

    stm->unk10 = ofst;
    stm->unk14 = fnsct;
    stm->dir = dir;
    stm->stat = 0;
    stm->rd_sct = 0;
    lsc->num_stm += 1;
    lsc->last_stm_index += 1;
    lsc->last_stm_index %= LSC_MAX_STM;

    if (lsc->stat == 1) {
        lsc->stat = 2;
    }

    return id;
}

void LSC_ResetEntry(LSC lsc) {
    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return;
    }

    if (lsc->stat == 0) {
        lsc->last_stm_index = 0;
        lsc->unk20 = 0;
        lsc->num_stm = 0;
    }
}

void LSC_Start(LSC lsc) {
    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return;
    }

    if (lsc->stat != 0) {
        LSC_Stop(lsc);
    }

    if (lsc->num_stm > 0) {
        lsc->stat = 2;
    } else {
        lsc->stat = 1;
    }
}

void LSC_Stop(LSC lsc) {
    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return;
    }

    if (lsc->stat != 0) {
        lsc->stat = 0;

        if ((lsc->stm_hndl != NULL) && (lsc->unk2 == 1)) {
            ADXSTM_Stop(lsc->stm_hndl);
            lsc->unk2 = 0;
        }

        lsc->unk2C = 0;
        LSC_ResetEntry(lsc);
        lsc->unk34 = 0;
    }
}

void LSC_Pause(LSC lsc, Sint32 sw) {
    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return;
    }

    if (sw == 1) {
        lsc->unk4 = 1;
    } else {
        lsc->unk4 = 0;
    }
}

void LSC_ExecServer() {
    LSC lsc;
    Sint32 i;

    for (i = 0; i < LSC_MAX_OBJ; i++) {
        lsc = &lsc_obj[i];

        if (lsc->used == 1) {
            lsc_ExecHndl(lsc);
        }
    }
}

Sint8 LSC_GetStat(LSC lsc) {
    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return -1;
    }

    return lsc->stat;
}

Sint32 LSC_GetNumStm(LSC lsc) {
    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return -1;
    }

    return lsc->num_stm;
}

Sint32 LSC_GetStmId(LSC lsc, Sint32 no) {
    Sint32 stream_index;

    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return -1;
    }

    if (no < 0 || no >= lsc->num_stm) {
        LSC_CallErrFunc("E0009: Illigal parameter no=%d\n", no);
        return -1;
    }

    stream_index = (lsc->unk20 + no) % LSC_MAX_STM;
    return lsc->stm[stream_index].id;
}

const Char8* LSC_GetStmFname(LSC lsc, Sint32 stm_id) {
    Sint32 i;

    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return NULL;
    }

    for (i = 0; i < LSC_MAX_STM; i++) {
        if (lsc->stm[i].id == stm_id) {
            break;
        }
    }

    if (i == LSC_MAX_STM) {
        LSC_CallErrFunc("E0012: Can not find stream ID =%d\n", stm_id);
        return NULL;
    }

    return lsc->stm[i].fname;
}

Sint32 LSC_GetStmStat(LSC lsc, Sint32 stm_id) {
    Sint32 i;

    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return -1;
    }

    for (i = 0; i < LSC_MAX_STM; i++) {
        if (lsc->stm[i].id == stm_id) {
            break;
        }
    }

    if (i == LSC_MAX_STM) {
        LSC_CallErrFunc("E0012: Can not find stream ID =%d\n", stm_id);
        return -1;
    }

    return lsc->stm[i].stat;
}

Sint32 LSC_GetStmRdSct(LSC lsc, Sint32 stm_id) {
    Sint32 i;

    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return 0;
    }

    for (i = 0; i < LSC_MAX_STM; i++) {
        if (lsc->stm[i].id == stm_id) {
            break;
        }
    }

    if (i == LSC_MAX_STM) {
        LSC_CallErrFunc("E0012: Can not find stream ID =%d\n", stm_id);
        return 0;
    }

    return lsc->stm[i].rd_sct;
}

void LSC_SetFlowLimit(LSC lsc, Sint32 min) {
    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return;
    }

    if ((min < 0) || (lsc->unk18 < min)) {
        LSC_CallErrFunc("E0010: Illigal parameter min=%d\n", min);
        return;
    }

    lsc->flow_limit = min;
}

Sint32 LSC_GetFlowLimit(LSC lsc) {
    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return -1;
    }

    return lsc->flow_limit;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/lsc", LSC_EntryChgStatFunc);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/lsc", LSC_CallStatFunc);

void LSC_SetLpFlg(LSC lsc, Sint32 flg) {
    if (lsc == NULL) {
        LSC_CallErrFunc("E0003: Illigal parameter lsc=NULL\n");
        return;
    }

    lsc->lp_flg = flg;
}
