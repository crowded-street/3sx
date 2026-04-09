#ifndef PPGWORK_H
#define PPGWORK_H

#include "structs.h"
#include "types.h"

extern PPGDataList ppgCapLogoList;
extern Palette ppgCapLogoPal;
extern Texture ppgCapLogoTex;
extern PPGDataList ppgAdxList;
extern PPGDataList ppgWarList;
extern Palette ppgAdxPal;
extern Palette ppgWarPal;
extern Texture ppgWarTex;
extern PPGDataList ppgTitleList;
extern Texture ppgTitleTex;
extern PPGDataList ppgOpnBgList;
extern Texture ppgOpnBgTex;
extern PPGDataList ppgScrListOpt;
extern PPGDataList ppgScrListShot;
extern PPGDataList ppgScrListFace;
extern PPGDataList ppgScrList;
extern Palette ppgScrPalOpt;
extern Palette ppgScrPalShot;
extern Palette ppgScrPalFace;
extern Palette ppgScrPal;
extern Texture ppgScrTex;
extern PPGDataList ppgAkaneList;
extern Palette ppgAkanePal;
extern Texture ppgAkaneTex;
extern PPGDataList ppgAkeList;
extern Palette ppgAkePal;
extern Texture ppgAkeTex;
extern PPGDataList ppgRwBgList;
extern Texture ppgRwBgTex;
extern PPGDataList ppgBgList[4];
extern Texture ppgBgTex[4];

void ppgWorkInitializeApprication();
void ppgPurgeFromVRAM(s32 type);
void ppgPurgePaletteFromVRAM(Palette* pal);
void ppgPurgeTextureFromVRAM(Texture* tex);

#endif
