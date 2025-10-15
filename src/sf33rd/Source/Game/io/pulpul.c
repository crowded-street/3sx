/**
 * @file pulpul.c
 * Vibration
 */

#include "sf33rd/Source/Game/io/pulpul.h"
#include "common.h"
#include "sdk/libvib.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/ps2/ps2PAD.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/debug/OBJTEST.h"
#include "sf33rd/Source/Game/debug/OTESTDAT.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/io/gd3rd.h"

// local declarations

const u16 comm_quay_pulpul[11];
const s16 pp_dm_shock[8];
const s16 pp_guard_shock[8];
const s16 pp_hit_shock[8];
const s8 dokidoki_ix_change_table[18];
const PPWORK_SUB_SUB pulreq_00[1];
const PPWORK_SUB_SUB pulreq_01[3];
const PPWORK_SUB_SUB pulreq_02[3];
const PPWORK_SUB_SUB pulreq_03[1];
const PPWORK_SUB_SUB pulreq_04[1];
const PPWORK_SUB_SUB pulreq_05[1];
const PPWORK_SUB_SUB pulreq_06[1];
const PPWORK_SUB_SUB pulreq_07[1];
const PPWORK_SUB_SUB pulreq_08[1];
const PPWORK_SUB_SUB pulreq_09[1];
const PPWORK_SUB_SUB pulreq_10[1];
const PPWORK_SUB_SUB pulreq_11[1];
const PPWORK_SUB_SUB pulreq_20[3];
const PPWORK_SUB_SUB pulreq_21[3];
const PPWORK_SUB_SUB pulreq_22[3];
const PPWORK_SUB_SUB pulreq_23[3];
const PPWORK_SUB_SUB pulreq_24[3];
const PPWORK_SUB_SUB pulreq_25[3];
const PPWORK_SUB_SUB pulreq_26[3];
const PPWORK_SUB_SUB pulreq_27[3];
const PPWORK_SUB_SUB pulreq_28[3];
const PPWORK_SUB_SUB pulreq_29[3];
const PPWORK_SUB_SUB pulreq_30[3];
const PPWORK_SUB_SUB pulreq_31[3];
const PPWORK_SUB_SUB pulreq_32[3];
const PPWORK_SUB_SUB pulreq_33[3];
const PPWORK_SUB_SUB pulreq_34[3];
const PPWORK_SUB_SUB pulreq_35[3];
const PPWORK_SUB_SUB pulreq_36[3];
const PPWORK_SUB_SUB pulreq_37[3];
const PPWORK_SUB_SUB pulreq_38[3];
const PPWORK_SUB_SUB pulreq_39[3];
const PPWORK_SUB_SUB pulreq_40[3];
const PPWORK_SUB_SUB pulreq_41[4];
const PPWORK_SUB_SUB pulreq_42[3];
const PPWORK_SUB_SUB pulreq_43[3];
const PPWORK_SUB_SUB pulreq_44[3];
const PPWORK_SUB_SUB pulreq_xx[6];
const PPWORK_SUB_SUB pulreq_x2[7];

// data

PULPARA pulpara[PULPARA_MAX] = {
    { 0, 0, 0, 0 },       { 3, 0, 0, 0 },       { 3, 0, 7, 0 },    { 2, 0, 1, 0x24 },    { 2, 0, 1, 0x2C },
    { 2, 0, 1, 0x20 },    { 2, 0, 1, 0x2A },    { 2, 0, 2, 0x1C }, { 2, 0, 1, 0x28 },    { 2, 0, 2, 0x18 },
    { 2, 0, 1, 0x24 },    { 2, 0, 3, 0x14 },    { 2, 0, 2, 0x20 }, { 2, 0, 4, 0x10 },    { 2, 0, 2, 0x1C },
    { 2, 0, 5, 0xC },     { 2, 0, 3, 0x18 },    { 2, 0, 6, 0xC },  { 2, 0, 3, 0x14 },    { 2, 0, 7, 8 },
    { 2, 0, 4, 0x10 },    { 2, 0, 7, 4 },       { 2, 0, 5, 0xE },  { 2, 1, 1, 0x2C },    { 2, 1, 3, 0x2C },
    { 3, 1, 5, 0x2C },    { 3, 1, 7, 0x2C },    { 1, 0, 1, 0 },    { 1, 0, 2, 0 },       { 2, 0, 3, 0 },
    { 3, 0, 4, 0 },       { 1, 1, 3, 0x18 },    { 2, 1, 6, 0 },    { 1, 0, 5, 0 },       { 2, 0, 7, 0 },
    { 1, 1, 1, 0x14 },    { 1, 1, 2, 0x20 },    { 2, 1, 3, 0x28 }, { 3, 1, 4, 0x2C },    { 2, 0, 2, 4 },
    { 2, 1, 3, 0x10 },    { 2, 1, 4, 0x18 },    { 3, 1, 5, 0x20 }, { 3, 1, 6, 0x28 },    { 3, 1, 7, 0x2C },
    { 3, 0x80, 7, 0x2C }, { 3, 1, 5, 0x2C },    { 3, 1, 7, 0x10 }, { 2, 1, 1, 0x2C },    { 0, 1, 4, 0 },
    { 0, 8, 1, 0 },       { 0, 0x80, 7, 0x2C }, { 0, 9, 1, 0 },    { 0, 0x81, 7, 0x2C },
};

PULREQ pulreq[PULREQ_MAX] = {
    { 0, 0, pulreq_00 }, { 0, 0, pulreq_01 }, { 1, 0, pulreq_01 }, { 0, 0, pulreq_02 }, { 1, 0, pulreq_03 },
    { 1, 0, pulreq_11 }, { 1, 0, pulreq_04 }, { 1, 0, pulreq_10 }, { 1, 0, pulreq_04 }, { 1, 0, pulreq_04 },
    { 1, 0, pulreq_03 }, { 1, 0, pulreq_07 }, { 1, 0, pulreq_04 }, { 1, 0, pulreq_06 }, { 1, 0, pulreq_09 },
    { 1, 0, pulreq_04 }, { 1, 0, pulreq_05 }, { 1, 0, pulreq_03 }, { 1, 0, pulreq_03 }, { 1, 0, pulreq_04 },
    { 1, 0, pulreq_08 }, { 1, 0, pulreq_03 }, { 1, 0, pulreq_03 }, { 1, 0, pulreq_04 }, { 0, 0, pulreq_20 },
    { 0, 0, pulreq_21 }, { 0, 0, pulreq_22 }, { 0, 0, pulreq_23 }, { 0, 0, pulreq_24 }, { 0, 0, pulreq_25 },
    { 0, 0, pulreq_26 }, { 0, 0, pulreq_27 }, { 0, 0, pulreq_28 }, { 0, 0, pulreq_29 }, { 0, 0, pulreq_30 },
    { 0, 0, pulreq_31 }, { 0, 0, pulreq_32 }, { 0, 0, pulreq_33 }, { 0, 0, pulreq_34 }, { 0, 0, pulreq_35 },
    { 0, 0, pulreq_36 }, { 0, 0, pulreq_37 }, { 0, 0, pulreq_38 }, { 0, 0, pulreq_39 }, { 0, 0, pulreq_40 },
    { 0, 0, pulreq_41 }, { 0, 0, pulreq_42 }, { 0, 0, pulreq_43 }, { 0, 0, pulreq_44 }, { 0, 0, pulreq_xx },
    { 1, 0, pulreq_x2 },
};

// sdata
static u16 pulpul_level[8] = { 0, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0 };

// rodata

const u16 comm_quay_pulpul[11] = { 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2A, 0x2B, 0x2B, 0x2B, 0x2B, 0x2C };

const s16 pp_dm_shock[8] = { 0x18, 0x19, 0x1A, 0x1A, 0x19, 0x1A, 0x1B, 0x1B };

const s16 pp_guard_shock[8] = { 0x1C, 0x1D, 0x1E, 0x1E, 0x1D, 0x1E, 0x1F, 0x1F };

const s16 pp_hit_shock[8] = { 0x24, 0x25, 0x26, 0x26, 0x25, 0x26, 0x27, 0x27 };

const s8 dokidoki_ix_change_table[18] = { 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 7 };

const PPWORK_SUB_SUB pulreq_00[1] = { { 0, 0 } };

const PPWORK_SUB_SUB pulreq_01[3] = { { 1, 1 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_02[3] = { { 2, 0xA }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_03[1] = { { 0, 0 } };

const PPWORK_SUB_SUB pulreq_04[1] = { { 0, 0 } };

const PPWORK_SUB_SUB pulreq_05[1] = { { 0, 0 } };

const PPWORK_SUB_SUB pulreq_06[1] = { { 0, 0 } };

const PPWORK_SUB_SUB pulreq_07[1] = { { 0, 0 } };

const PPWORK_SUB_SUB pulreq_08[1] = { { 0, 0 } };

const PPWORK_SUB_SUB pulreq_09[1] = { { 0, 0 } };

const PPWORK_SUB_SUB pulreq_10[1] = { { 0, 0 } };

const PPWORK_SUB_SUB pulreq_11[1] = { { 0, 0 } };

const PPWORK_SUB_SUB pulreq_20[3] = { { 0x17, 0x6 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_21[3] = { { 0x18, 0xA }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_22[3] = { { 0x19, 0x10 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_23[3] = { { 0x1A, 0x18 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_24[3] = { { 0x1B, 2 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_25[3] = { { 0x1C, 6 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_26[3] = { { 0x1D, 0xE }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_27[3] = { { 0x1E, 0x12 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_28[3] = { { 0x1F, 8 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_29[3] = { { 0x20, 0x12 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_30[3] = { { 0x21, 6 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_31[3] = { { 0x22, 8 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_32[3] = { { 0x23, 5 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_33[3] = { { 0x24, 8 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_34[3] = { { 0x25, 0x10 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_35[3] = { { 0x26, 0x18 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_36[3] = { { 0x27, 0xC }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_37[3] = { { 0x28, 0x14 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_38[3] = { { 0x29, 0x1C }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_39[3] = { { 0x2A, 0x28 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_40[3] = { { 0x2B, 0x3C }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_41[4] = { { 0x2C, 0x14 }, { 0x2D, 0x46 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_42[3] = { { 0x2E, 0x46 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_43[3] = { { 0x2F, 0x46 }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_44[3] = { { 0x30, 0xC }, { 1, 0 }, { 0, 0 } };

const PPWORK_SUB_SUB pulreq_xx[6] = {
    { 0x31, 0x3C }, { 0x32, 0x3C }, { 0x33, 0x3C }, { 0x34, 0x3C }, { 0x35, 0x3C }, { 0, 0 },
};

const PPWORK_SUB_SUB pulreq_x2[7] = {
    { 0x31, 0x1E }, { 0x32, 0x32 }, { 0x33, 0x46 }, { 0x34, 0x5A }, { 0x35, 0x78 }, { -1, 3 }, { 0, 0 },
};

// sbss

PPWORK ppwork[2];
PUL pul[2];
s16 vib_req[2][2];
u8 pulpul_scene;

void init_pulpul_work() {
    init_pulpul_work2(0);
    init_pulpul_work2(1);
}

void init_pulpul_work2(s16 ix) {
    ppwork[ix].ok_dev = 0;
    ppwork[ix].id = ix;
    ppwork[ix].psix = 0;
    ppwork[ix].inStop = 0;

    ppwork[ix].p[0].ppnew = 0;

    vib_req[ix][0] = vib_req[ix][1] = 0;

    init_pulpul_round2(ix);
}

void init_pulpul_round2(s16 ix) {
    ppwork[ix].vital = 0xA0;
    pul[ix].ix.cal = 0;
}

void pulpul_stop() {
    pulpul_stop2(0);
    pulpul_stop2(1);
}

void pulpul_stop2(s16 ix) {
    if (ppwork[ix].ok_dev) {
        vibParamTrans(ix, &pulpara[1]);
        ppwork[ix].inStop = 4;
    }

    ppwork[ix].ok_dev = 0;
    ppwork[ix].p[0].ppnew = 0;

    waitVsyncDummy();
}

void pulpul_request(s16 id, s16 ix) {
    PULREQ* adr;

    if (vib_sel[id] == 0) {
        return;
    }

    if (ix == 0) {
        return;
    }

    if (test_flag) {
        adr = &ot_pulreq[ix];
        adr->adrs = ot_pulreq_xx;
    } else {
        adr = &pulreq[ix];
    }

    vib_req[id][adr->prio] = ix;

    pulpul_req_copy(id, adr);
}

void pulpul_req_copy(s16 id, PULREQ* adr) {
    ppwork[id].p[adr->prio].ppnew = ppwork[id].ok_dev;
    ppwork[id].p[adr->prio].rno[0] = 0;
    ppwork[id].p[adr->prio].rno[1] = adr->rno;
    ppwork[id].p[adr->prio].padr = adr->adrs;
}

void pp_vib_on(s16 id) {
    PULREQ* adr;

    adr = &pulreq[3];
    pulpul_req_copy(id, adr);
}

void pulpul_request_again() {
    pulpul_request(0, vib_req[0][0]);
    pulpul_request(1, vib_req[1][0]);
}

void pp_operator_check_flag(u8 fl) {
    ppwork[0].opck = ppwork[1].opck = fl;
}

void move_pulpul_work() {
    move_pulpul(&ppwork[0]);
    move_pulpul(&ppwork[1]);

    // This is a comparison to zero in the decomp. Might be a programmer error
    test_flag = 0;
}

s32 chkVibUnit(s32 port) {
    if ((flpad_adr[0][port].kind == 0) || (flpad_adr[0][port].kind == 0x8000)) {
        return 0;
    }

    return flpad_adr[0][port].conn.vib;
}

void move_pulpul(PPWORK* wk) {
    s32 i;
    s32 index;
    s32 data;
    s32 result;

    if (wk->ok_dev) {
        if (chkVibUnit(wk->id) == 0) {
            wk->ok_dev = 0;
            vibParamTrans(wk->id, &pulpara[1]);
            return;
        }

        for (i = 0; i <= 0; i++) {
            switch (wk->p[i].rno[0]) {
            case 0:
                if (wk->p[i].ppnew == 0)
                    break;

                wk->p[i].rno[0] = 1;
                wk->p[i].ppnew = 0;
                wk->p[i].exix = -1;
                /* fallthrough */
            case 1:
                wk->p[i].exix += 1;

            lbl:
                index = wk->p[i].padr[wk->p[i].exix].ix;
                data = wk->p[i].padr[wk->p[i].exix].timer;

                if (index <= 0) {
                    if (index == 0) {
                        wk->p[i].rno[0] = 0;
                        vib_req[wk->id][i] = 0;
                        vibParamTrans(wk->id, &pulpara[1]);
                        break;
                    } else if (index == -1) {
                        wk->p[i].exix = data;
                        goto lbl;
                    }

                    if (index == -3) {
                        wk->p[i].rno[0] = 0;

                        if (test_flag) {
                            *ot_mot_of = data;
                            ot_make_curr_vib_data();
                        }

                        pulpul_request((s16)wk->id, data);
                        break;
                    }

                    if (index == -2) {
                        if (wk->vital >= data) {
                            wk->p[i].exix += 1;
                        } else {
                            wk->p[i].exix += 2;
                        }
                        goto lbl;
                    }

                    wk->p[i].rno[0] = 0;
                    break;
                }
                wk->p[i].rno[0] = 2;
                /* fallthrough */

            case 2:
                result = pulpul_pdVibMxStart(wk, i, wk->port, &pulpara[wk->p[i].padr[wk->p[i].exix].ix]);

                if (!result) {
                    break;
                }

                wk->p[i].rno[0] = 3;

                if (i == 1) {
                    wk->p[i].life = ((wk->p[i].padr[wk->p[i].exix].timer) * (0x20 - pul[wk->id].tim_ans)) / 0x20;
                } else {
                    wk->p[i].life = wk->p[i].padr[wk->p[i].exix].timer;
                }
                /* fallthrough */

            case 3:
                if (--wk->p[i].life < 0) {
                    wk->p[i].rno[0] = 1;
                }
                break;
            }
        }
        return;
    }

    if (chkVibUnit(wk->id) != 0) {
        wk->ok_dev = 1;
        wk->p[0].rno[0] = wk->p[1].rno[0] = 0;
        wk->p[0].ppnew = wk->p[1].ppnew = 0;
    }
}

s32 pulpul_pdVibMxStart(PPWORK* wk, s32 arg1, s32 arg2, PULPARA* param) {
    PULPARA adrs;

    adrs = *param;

    if ((Check_Menu_Task() == 0) && (test_flag == 0)) {
        if (save_w[Present_Mode].Pad_Infor[wk->id].Vibration == 0) {
            return 1;
        }

        if ((wk->opck) && (plw[wk->id].wu.operator == 0)) {
            return 1;
        }

        if (pulpul_scene == 0) {
            return 1;
        }
    }

    return vibParamTrans(wk->id, &adrs);
}

s32 vibParamTrans(s32 id, PULPARA* prm) {
    s32 vib_data_size;
    s32 rnum;
    u16 big;
    u8 vib_data[2];
    u8 profile;

    profile = ps2slot[id].vprofile & 3;
    if ((prm->power == 0) || (prm->unit == 0) || (profile == 0)) {
        switch (profile) {
        case 1:
        case 2:
            vib_data_size = 1;
            vib_data[0] = 0;
            break;

        default:
            vib_data_size = 2;
            vib_data[0] = 0;
            vib_data[1] = 0;
            break;
        }

        profile = 3;
    } else {

        big = prm->freq + pulpul_level[prm->power];

        switch (profile &= prm->unit) {
        case 1:
            vib_data_size = 1;
            vib_data[0] = 1;
            break;

        case 2:
            vib_data_size = 1;
            vib_data[0] = big;
            break;

        default:
            vib_data_size = 2;
            vib_data[0] = (((big * 2) & 0xFE) | 1);
            vib_data[1] = (big >> 7) & 1;
            break;
        }
    }

    rnum = sceVibSetActParam(ps2slot[id].socket_id, 1, &profile, vib_data_size, vib_data);

    return rnum == 1;
}

void pp_screen_quake(s16 ix) {
    ix /= 3;

    if (ix > 0xA) {
        ix = 0xA;
    }

    pulpul_request(0, comm_quay_pulpul[ix]);
    pulpul_request(1, comm_quay_pulpul[ix]);
}

s32 pp_conv_kow(u8 num) {
    s16 ix;

    if (num & 0x60) {
        if (num & 0x40) {
            ix = 0x11;
        } else {
            ix = 0x10;
        }
    } else {
        ix = num / 2 & 0xF;
    }

    return dokidoki_ix_change_table[ix];
}

void pp_pulpara_remake_at_init() {
    // do nothing
}

void pp_pulpara_remake_at_init2() {
    // do nothing
}

void pp_pulpara_remake_at_hit() {
    // do nothing
}

void pp_pulpara_remake_at() {
    // do nothing
}

void pp_pulpara_remake_dm_all(WORK* wk) {
    s16 ix;

    ix = pp_conv_kow(wk->dm_kind_of_waza);

    pulpul_request(wk->id, pp_dm_shock[ix]);
}

void pp_pulpara_guard(WORK* wk) {
    pulpul_request(wk->id, pp_guard_shock[pp_conv_kow(wk->dm_kind_of_waza)]);
}

void pp_pulpara_hit(WORK* wk) {
    pulpul_request(wk->id, pp_hit_shock[pp_conv_kow(wk->kind_of_waza)]);
}

void pp_pulpara_blocking(WORK* wk) {
    pulpul_request(wk->id, 0x20);

    if (((s16*)wk->dmg_adrs)[3] == 1) {
        pulpul_request((wk->id + 1) & 1, 0x21);
    }
}

void pp_pulpara_catch(WORK* wk) {
    pulpul_request(wk->id, 0x22);
}

void pp_pulpara_caught(WORK* wk) {
    pulpul_request(wk->id, 0x23);
}

void pp_pulpara_shungokusatsu(WORK* wk) {
    pulpul_request(wk->id, 0x2E);
    pulpul_request((wk->id + 1) & 1, 0x2F);
}
