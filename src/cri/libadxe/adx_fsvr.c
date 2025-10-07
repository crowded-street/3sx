#include "common.h"
#include <cri/private/libadxe/adx_crs.h>
#include <cri/private/libadxe/adx_stmc.h>
#include <cri/private/libadxe/cri_cvfs.h>
#include <cri/private/libadxe/cri_srd.h>

#include <cri/cri_adxf.h>
#include <cri/ee/cri_xpt.h>

Sint32 adxt_fssvr_enter_cnt = 0;

void ADXT_ExecFsSvr() {
    ADXCRS_Lock();

    if (adxt_fssvr_enter_cnt != 0) {
        ADXCRS_Unlock();
        return;
    }

    adxt_fssvr_enter_cnt = 1;
    ADXCRS_Unlock();
    ADXSTM_ExecServer();
    adxt_fssvr_enter_cnt = 2;
    cvFsExecServer();
    adxt_fssvr_enter_cnt = 3;
    SRD_ExecServer();
    adxt_fssvr_enter_cnt = 4;
    cvFsExecServer();
    adxt_fssvr_enter_cnt = 5;
    ADXSTM_ExecServer();
    adxt_fssvr_enter_cnt = 6;
    ADXF_ExecServer();
    adxt_fssvr_enter_cnt = 0;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_fsvr", ADXT_ExecFsServer);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_fsvr", ADXT_IsActiveFsSvr);
