#include "common.h"
#include <cri/private/libadxe/structs.h>

#include <cri/private/libadxe/adx_rnap.h>
#include <cri/private/libadxe/ps2_rna.h>

#include <cri/cri_xpts.h>
#include <cri/ee/adx_ps2.h>
#include <cri/sj.h>

void ADXPS2_SetupVoice(Sint32 nste, Sint32 nmono) {
    PS2RNA_SetupVoice(nste, nmono);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXPS2_GetNumDataSjtmp);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXPS2_GetNumRoomSjtmp);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXPS2_GetNumDataSjiop);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXPS2_GetNumRoomSjiop);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXPS2_RestoreIopSnd);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXPS2_BeginDolbyDigital);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXPS2_EndDolbyDigital);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXPS2_SetStmType);

void ADXRNA_Init() {
    PS2RNA_Init();
}

void ADXRNA_Finish() {
    PS2RNA_Finish();
}

void ADXRNA_EntryErrFunc(void (*func)(void*, Char8*), void* object) {
    // Do nothing
}

ADXRNA ADXRNA_Create(SJ* sjo, Sint32 maxnch) {
    return PS2RNA_Create(sjo, maxnch);
}

void ADXRNA_Destroy(ADXRNA rna) {
    PS2RNA_SetPlaySw(rna, 0);
    PS2RNA_SetTransSw(rna, 0);
    PS2RNA_Destroy(rna);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_Start);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_Stop);

void ADXRNA_SetTransSw(ADXRNA rna, Sint32 sw) {
    PS2RNA_SetTransSw(rna, sw);
}

void ADXRNA_SetPlaySw(ADXRNA rna, Sint32 sw) {
    PS2RNA_SetPlaySw(rna, sw);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_GetTime);

Sint32 ADXRNA_GetNumData(ADXRNA rna) {
    return PS2RNA_GetNumData(rna);
}

Sint32 ADXRNA_GetNumRoom(ADXRNA rna) {
    return PS2RNA_GetNumRoom(rna);
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", D_0055B3B0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_GetStat);

void ADXRNA_ExecServer() {
    PS2RNA_ExecServer();
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_SetStartSmpl);

void ADXRNA_SetNumChan(ADXRNA rna, Sint32 num_chan) {
    PS2RNA_SetNumChan(rna, num_chan);
}

void ADXRNA_SetSfreq(ADXRNA rna, Sint32 sfreq) {
    PS2RNA_SetSfreq(rna, sfreq);
}

void ADXRNA_SetOutVol(ADXRNA rna, Sint32 vol) {
    PS2RNA_SetOutVol(rna, vol);
}

void ADXRNA_SetOutPan(ADXRNA rna, Sint32 channel, Sint32 pan) {
    PS2RNA_SetOutPan(rna, channel, pan);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_SetOutBalance);

void ADXRNA_SetBitPerSmpl(ADXRNA rna, Sint32 bps) {
    PS2RNA_SetBitPerSmpl(rna, bps);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_GetSfreq);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_GetOutVol);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_GetOutPan);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_GetOutBalance);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_GetBitPerSmpl);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_DiscardData);

void ADXRNA_ClearBuf() {
    // Do nothing
}

void ADXRNA_SetPcmType() {
    // Do nothing
}

void ADXRNA_SetTotalNumSmpl(ADXRNA rna, Sint32 samples) {
    // Do nothing
}

void ADXRNA_SetWavFname() {
    // Do nothing
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_PauseAll);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_GetStatPauseAll);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_rnap", ADXRNA_SetStmHdInfo);
#else
void ADXRNA_SetStmHdInfo(ADXRNA rna, Sint32) {
    not_implemented(__func__);
}
#endif
