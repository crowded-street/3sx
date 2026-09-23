#ifndef SE_DATA_H
#define SE_DATA_H

#include "structs.h"
#include "types.h"

typedef void (*se_request)(WORK_Other* ewk, u16 Code);

extern const se_request sound_effect_request[];
extern const u16 sdcode_conv[];
extern const u16 Bonus_Voice_Data[768];

#endif
