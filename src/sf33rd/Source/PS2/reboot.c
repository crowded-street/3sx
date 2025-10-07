#include "sf33rd/Source/PS2/reboot.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/ps2/flps2dma.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#include <eekernel.h>
#include <libcdvd.h>
#include <libgraph.h>

#include <cri/cri_adxt.h>
#include <cri/ee/adx_ps2.h>

static s8* reboot_param[2] = { "reboot_menu", "0" };

void Reboot_Program(s8* args) {
    flFlip(0);
    flFlip(0);
    flFlip(0);
    ADXPS2_LoadFcacheDvd(0);
    flPS2DmaWait();
    sceGsSyncPath(0, 0);
    flPADDestroy();
    DIntr();
    DisableDmac(flPs2VIF1Control.channel_id);
    RemoveDmacHandler(flPs2VIF1Control.channel_id, flPs2VIF1Control.handleID);
    DisableIntc(0);
    RemoveIntcHandler(0, flPs2GsHandler);
    EIntr();
    ADXM_ShutdownThrd();
    ADXT_Finish();
    sceGsSyncVCallback(0);
    sceCdInit(5);
    LoadExecPS2(args, 2, reboot_param);
}
