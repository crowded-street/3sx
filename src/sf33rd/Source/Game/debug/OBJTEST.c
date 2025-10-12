#include "sf33rd/Source/Game/debug/OBJTEST.h"
#include "common.h"
#include "sf33rd/Source/Game/debug/OTESTDAT.h"
#include "sf33rd/Source/Game/io/pulpul.h"

// sbss
PPWORK_SUB_SUB ot_pulreq_xx[OT_PULREQ_XX_MAX];

// bss
PULREQ ot_pulreq[OT_PULREQ_MAX];
PULPARA ot_pulpara[OT_PULPARA_MAX];

void ot_make_curr_vib_data() {
    const PPWORK_SUB_SUB* adrs;
    s16 i;

    for (i = 0; i < (OT_PULPARA_MAX + 1); i++) {
        ot_pulpara[i] = pulpara[i];
    }

    for (i = 0; i < OT_PULREQ_MAX; i++) {
        ot_pulreq[i] = pulreq[i];
    }

    adrs = ot_pulreq[ot_mot_of[0]].adrs;
    for (i = 0; i < OT_PULREQ_XX_MAX; i++) {
        ot_pulreq_xx[i] = adrs[i];
        if (ot_pulreq_xx[i].ix == 0) {
            break;
        }
    }

    ot_pat_of[1] = i;
    ot_pat_of[0] = 0;
}
