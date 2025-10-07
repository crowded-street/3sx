#include "common.h"
#include <cri/private/libadxe/adx_amp.h>
#include <cri/private/libadxe/adx_crs.h>
#include <cri/sj.h>
#include <memory.h>

s32 adxsmp_init_cnt = 0;
ADXAMP_OBJ adxamp_obj[16] = { 0 };

void ADXAMP_Init() {
    if (adxsmp_init_cnt == 0) {
        memset(&adxamp_obj, 0, sizeof(adxamp_obj));
    }

    adxsmp_init_cnt += 1;
}

void ADXAMP_Finish() {
    if (--adxsmp_init_cnt == 0) {
        memset(&adxamp_obj, 0, sizeof(adxamp_obj));
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_amp", ADXAMP_Create);

void ADXAMP_Destroy(ADXAMP amp) {
    if (amp != NULL) {
        ADXCRS_Lock();
        memset(amp, 0, sizeof(ADXAMP_OBJ));
        ADXCRS_Unlock();
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_amp", ADXAMP_GetStat);

void ADXAMP_Start(ADXAMP amp) {
    s32 i;
    SJ sj;
    SJCK ck;

    for (i = 0; i < amp->unk2; i++) {
        amp->unk14[i] = 0;
    }

    amp->unk2C = 0;

    for (i = 0; i < amp->unk2; i++) {
        sj = amp->unk4[i];
        SJ_Reset(sj);
        SJ_GetChunk(sj, 0, SJ_GetNumData(sj, 0), &ck);
        memset(ck.data, 0, ck.len);
        SJ_UngetChunk(sj, 0, &ck);
    }

    for (i = 0; i < amp->unk2; i++) {
        sj = amp->unkC[i];
        SJ_Reset(sj);
        SJ_GetChunk(sj, 0, SJ_GetNumData(sj, 0), &ck);
        memset(ck.data, 0, ck.len);
        SJ_UngetChunk(sj, 0, &ck);
    }

    amp->unk1 = 2;
}

void ADXAMP_Stop(ADXAMP amp) {
    amp->unk1 = 0;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_amp", adxamp_extract);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_amp", ADXAMP_ExecHndl);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_amp", ADXAMP_ExecServer);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_amp", ADXAMP_GetExtractNumSmpl);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_amp", ADXAMP_SetSfreq);
#else
void ADXAMP_SetSfreq(ADXAMP amp, Sint32 sfreq) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_amp", ADXAMP_GetSfreq);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_amp", ADXAMP_SetFrmLen);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_amp", ADXAMP_GetFrmLen);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_amp", ADXAMP_SetFrmPrd);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_amp", ADXAMP_GetFrmPrd);
