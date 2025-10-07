#include "common.h"
#include <cri/private/libadxe/adx_fini.h>

#include <cri/cri_adxf.h>
#include <cri/cri_xpts.h>

#include <string.h>

Char8* volatile adxf_build = "\nADXF/PS2EE Ver.7.13 Build:Sep 18 2003 09:59:54\n";
Sint32 adxf_init_cnt = 0;
ADX_FS adxf_obj[ADXF_OBJ_MAX] = { 0 };
ADXF_PTINFO* adxf_ptinfo[ADXF_PART_MAX] = { 0 };
Sint32 adxf_ocbi_fg = 0;
ADXF_CMD_HSTRY adxf_cmd_hstry[ADXF_CMD_HSTRY_MAX] = { 0 };
Sint16 adxf_cmd_ncall[ADXF_CMD_NUM_MAX] = { 0 };
Sint32 adxf_hstry_no = 0;
Sint32 adxf_flno = 0;
ADXF adxf_ldptnw_hn = 0;
Sint32 adxf_ldptnw_ptid = 0;

void ADXF_Init() {
    adxf_build;

    if (adxf_init_cnt == 0) {
        memset(adxf_obj, 0, sizeof(adxf_obj));
        memset(adxf_ptinfo, 0, sizeof(adxf_ptinfo));
        memset(adxf_cmd_hstry, 0xFF, sizeof(adxf_cmd_hstry));
        memset(adxf_cmd_ncall, 0, sizeof(adxf_cmd_ncall));
        adxf_hstry_no = 0;
        adxf_ocbi_fg = 0;
        adxf_flno = 0;
        adxf_ldptnw_hn = NULL;
        adxf_ldptnw_ptid = -1;
    }

    adxf_init_cnt += 1;
}

void ADXF_Finish() {
    if (--adxf_init_cnt == 0) {
        ADXF_CloseAll();
        adxf_ldptnw_ptid = -1;
        adxf_ldptnw_hn = NULL;
        adxf_flno = 0;
        adxf_ocbi_fg = 0;
        adxf_hstry_no = 0;
        memset(adxf_cmd_ncall, 0, sizeof(adxf_cmd_ncall));
        memset(adxf_cmd_hstry, 0xFF, sizeof(adxf_cmd_hstry));
        memset(adxf_ptinfo, 0, sizeof(adxf_ptinfo));
        memset(adxf_obj, 0, sizeof(adxf_obj));
    }
}
