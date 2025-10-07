#include "common.h"
#include <cri/private/libadxe/lsc_svr.h>

void lsc_StatWait(LSC lsc) {
    not_implemented(__func__);
}

void lsc_StatRead(LSC lsc) {
    not_implemented(__func__);
}

void lsc_StatEnd(LSC lsc) {
    not_implemented(__func__);
}

void lsc_ExecHndl(LSC lsc) {
    if ((lsc->unk4 == 1) || (lsc->stat != 2) || (lsc->num_stm <= 0)) {
        return;
    }

    if (lsc->stm[lsc->unk20].stat == 1) {
        lsc_StatRead(lsc);
    }

    if (lsc->stm[lsc->unk20].stat == 2) {
        lsc_StatEnd(lsc);
    }

    if (lsc->stm[lsc->unk20].stat != 0) {
        return;
    }

    lsc_StatWait(lsc);
}
