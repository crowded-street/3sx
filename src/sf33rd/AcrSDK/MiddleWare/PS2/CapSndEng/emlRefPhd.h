#ifndef EML_REF_PHD_H
#define EML_REF_PHD_H

#include "structs.h"
#include "types.h"

s32 IsSafeHeadChunk(_ps2_head_chunk* pHEAD);
s32 IsSafeProgChunk(_ps2_prog_chunk* pPROG);
s32 IsSafeSmplChunk(_ps2_smpl_chunk* pSMPL);
s32 IsSafeVagiChunk(_ps2_vagi_chunk* pVAGI);
s32 GetNumSplit(_ps2_head_chunk* pHEAD, u8 prog);
s32 GetPhdParam(CSE_PHDPADDR* pHDPA, _ps2_head_chunk* pHEAD, u8 prog, u8 note, u8 index);
s32 CalcPhdParam(CSE_PHDP* pPHDP, CSE_PHDPADDR* pHDPA, u8 note, u32 SpuTopAddr);

#endif
