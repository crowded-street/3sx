#include "sf33rd/AcrSDK/ps2/ps2PAD.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/ps2/flPADUSR.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "structs.h"

#include <SDL3/SDL.h>

#include <libpad2.h>
#include <libvib.h>

#include <math.h>

#define PI 3.14159275f

static u32 ps2pad_hard_to_soft_ds2[16][2] = { { 4096, 1 }, { 16384, 1 }, { 32768, 1 }, { 8192, 1 },
                                              { 64, 2 },   { 32, 2 },    { 2, 3 },     { 1, 3 },
                                              { 128, 2 },  { 16, 2 },    { 8, 3 },     { 4, 3 },
                                              { 512, 0 },  { 1024, 0 },  { 256, 0 },   { 2048, 0 } };

static u32 ps2pad_hard_to_soft_dg[16][2] = { { 1024, 0 },  { 4096, 0 },  { 8192, 0 }, { 2048, 0 }, { 16, 0 }, { 8, 0 },
                                             { 32768, 0 }, { 16384, 0 }, { 32, 0 },   { 4, 0 },    { 2, 0 },  { 1, 0 },
                                             { 0, 0 },     { 0, 0 },     { 256, 0 },  { 512, 0 } };

static u8 ps2pad_an_rm_map[12] = { 2, 3, 1, 0, 6, 5, 11, 10, 7, 4, 9, 8 };
static PS2PAD_CONFIG ps2PadShotConf_Basic = { 1, 1, 1, 0 };
static u8 etclever_wrong_data[16] = { 0, 1, 2, 0, 4, 5, 6, 4, 8, 9, 10, 8, 0, 1, 2, 0 };

u32 ps2pad_hard_to_soft[2][16][2];
TARPAD tarpad_root[2];
PS2Slot ps2slot[2];
PS2PAD_CONFIG ps2pad_config[2];
PS2PAD_STATE ps2pad_state[2];
PS2PAD_STATE ps2pad_backup[2];
PS2PAD_STATE ps2pad_clear;
u8 flPadFixedAnalogSelectSwitch[2];
u8 flPadFASS[2];

static s32 PADRead_for_PS2(s32 i);
static void update_pad_stick_dir(PAD_STICK* st, s16 depth);
static u8 lever_analog_to_digital(PAD_STICK* st);
static void PADReadSub(s32 i);
s32 flPADShockSet(s32 pad_id, u32 level, u32 time);
static void ps2PADWorkClear();
static void PADPortOpen(s32 port, s32 slot, PS2Slot* adrs);

s32 tarPADInit() {
    s32 i;

    ps2PADWorkClear();
    ps2pad_clear.pad_buffer[0] = 0xFF;
    ps2pad_clear.ix.sw = 0xFFFF;

    for (i = 0; i < 2; i++) {
        ps2pad_backup[i] = ps2pad_clear;
        ps2slot[i].state = 0;
        ps2slot[i].phase = 0;
        ps2slot[i].port = 0;
        ps2slot[i].slot = 0;
        ps2slot[i].vib = 0;
    }

    PADPortOpen(0, 0, &ps2slot[0]);
    PADPortOpen(1, 0, &ps2slot[1]);

    for (i = 0; i < 2; i++) {
        ps2pad_config[i] = ps2PadShotConf_Basic;
        tarpad_root[i].conn.gc.vib = ps2slot[i].port;
        tarpad_root[i].conn.gc.etc0 = ps2slot[i].slot;
        flPadFixedAnalogSelectSwitch[i] = flPadFASS[i] = 0;
    }

    return 1;
}

void tarPADDestroy() {
    ps2PADWorkClear();
}

void flPADConfigSetACRtoXX(s32 padnum, s16 a, s16 b, s16 c) {
    ps2pad_config[padnum].abut_on = a;
    ps2pad_config[padnum].ast1_on = b;
    ps2pad_config[padnum].ast2_on = c;
}

void tarPADRead() {
    s32 i;
    TARPAD* pad;
    u32 ast1;
    u32 ast2;

    for (i = 0; i < 2; i++) {
        if (PADRead_for_PS2(i) != 0) {
            pad = &tarpad_root[i];
            pad->conn.gc.etc1 = ps2slot[i].vib;
            pad->sw = (pad->sw & 0xFFF0) | (etclever_wrong_data[pad->sw & 0xF]);
            update_pad_stick_dir(&pad->stick[0], ps2pad_config[i].ast1_on);
            update_pad_stick_dir(&pad->stick[1], ps2pad_config[i].ast2_on);
            ast1 = lever_analog_to_digital(&pad->stick[0]);
            ast2 = lever_analog_to_digital(&pad->stick[1]);
            pad->sw |= ast1 << 16;
            pad->sw |= ast2 << 20;
        }

        flPadFASS[i] = flPadFixedAnalogSelectSwitch[i];
    }
}

void ps2PADWorkClear() {
    SDL_zeroa(tarpad_root);
    SDL_zeroa(ps2pad_state);
    SDL_zeroa(ps2pad_backup);
    SDL_zero(ps2pad_clear);
}

static s32 PADRead_for_PS2(s32 i) {
    s32 j;
    u32 io;
    u8 kan[12];

    if (ps2slot[i].state == 0) {
        return 0;
    }

    PADReadSub(i);

    if (tarpad_root[i].kind != 0 && !(tarpad_root[i].kind & 3)) {
        tarpad_root[i].kind = 0x8000;
    }

    for (j = 0; j < 16; j++) {
        tarpad_root[i].anshot.pow[j] = 0;
    }

    for (j = 0; j < 12; j++) {
        kan[j] = ps2pad_state[i].ix.depth[j] / 2;
    }

    ps2pad_state[i].ix.sw = ~ps2pad_state[i].ix.sw;

    switch (tarpad_root[i].kind) {
    case 1:
    case 2:
    case 4:
    case 8:
    case 16:
    case 32:
        io = 0;

        for (j = 0; j < 16; j++) {
            if (ps2pad_state[i].ix.sw & ps2pad_hard_to_soft[i][j][0]) {
                io |= flpad_io_map[j];
                tarpad_root[i].anshot.pow[j] = 0x7F;
            }

            switch (ps2pad_hard_to_soft[i][j][1]) {
            case 1:
                if (tarpad_root[i].anstate & 0x10) {
                    if (kan[ps2pad_an_rm_map[j]] < ps2pad_config[i].abut_on) {
                        tarpad_root[i].anshot.pow[j] = 0;
                    } else {
                        tarpad_root[i].anshot.pow[j] = kan[ps2pad_an_rm_map[j]];
                    }
                }

                break;

            case 2:
                if (tarpad_root[i].anstate & 1) {
                    if (kan[ps2pad_an_rm_map[j]] < ps2pad_config[i].abut_on) {
                        tarpad_root[i].anshot.pow[j] = 0;
                    } else {
                        tarpad_root[i].anshot.pow[j] = kan[ps2pad_an_rm_map[j]];
                    }
                }

                break;

            case 3:
                if (tarpad_root[i].anstate & 2) {
                    if (kan[ps2pad_an_rm_map[j]] < ps2pad_config[i].abut_on) {
                        tarpad_root[i].anshot.pow[j] = 0;
                    } else {
                        tarpad_root[i].anshot.pow[j] = kan[ps2pad_an_rm_map[j]];
                    }
                }

                break;
            }
        }

        tarpad_root[i].sw = io;

        if (tarpad_root[i].anstate & 0x20) {
            tarpad_root[i].stick[0].x = ps2pad_state[i].pad_buffer[6] - 0x80;
            tarpad_root[i].stick[0].y = ps2pad_state[i].pad_buffer[7] - 0x80;

            if (tarpad_root[i].stick[0].x < -0x7F) {
                tarpad_root[i].stick[0].x = -0x7F;
            }

            if (tarpad_root[i].stick[0].y < -0x7F) {
                tarpad_root[i].stick[0].y = -0x7F;
            }
        }

        if (tarpad_root[i].anstate & 0x40) {
            tarpad_root[i].stick[1].x = ps2pad_state[i].pad_buffer[4] - 0x80;
            tarpad_root[i].stick[1].y = ps2pad_state[i].pad_buffer[5] - 0x80;

            if (tarpad_root[i].stick[1].x < -0x7F) {
                tarpad_root[i].stick[1].x = -0x7F;
            }

            if (tarpad_root[i].stick[1].y < -0x7F) {
                tarpad_root[i].stick[1].y = -0x7F;
            }
        }

        ps2pad_backup[i] = ps2pad_state[i];
        ps2pad_backup[i].ix.sw = ~ps2pad_backup[i].ix.sw;
        break;

    default:
        tarpad_root[i].sw = 0;
        tarpad_root[i].stick[0].x = 0;
        tarpad_root[i].stick[0].y = 0;
        tarpad_root[i].stick[1].x = 0;
        tarpad_root[i].stick[1].y = 0;
        break;
    }

    return 1;
}

void update_pad_stick_dir(PAD_STICK* st, s16 depth) {
    f32 radian;

    if ((st->y | st->x) == 0) {
        radian = 0.0f;
    } else {
        radian = atan2(-st->y, st->x);

        if (radian < 0.0f) {
            radian += PI * 2;
        }
    }

    st->rad = radian;
    st->ang = 360.0f * (st->rad / (PI * 2));
    st->pow = sqrt((f64)(st->x * st->x) + (f64)(st->y * st->y));

    if (st->pow > 0x7F) {
        st->pow = 0x7F;
    }

    if (st->pow < depth) {
        st->x = 0;
        st->y = 0;
        st->rad = 0.0f;
        st->ang = 0;
        st->pow = 0;
    }
}

u8 lever_analog_to_digital(PAD_STICK* st) {
    static u8 ps2lever_analog_to_digital[16] = { 8, 9, 9, 1, 1, 5, 5, 4, 4, 6, 6, 2, 2, 10, 10, 8 };

    if (st->pow < 0x40) {
        return 0;
    }

    return ps2lever_analog_to_digital[(u32)(st->ang / 22.5f)];
}

// FIXME: this doesn't actually open anything, better remove it entirely
void PADPortOpen(s32 port, s32 slot, PS2Slot* adrs) {
    adrs->socket_id = port;
    adrs->state = 1;
    adrs->phase = 0;
    adrs->port = port;
    adrs->slot = slot;
}

void PADReadSub(s32 i) {
    s32 lp0;
    s32 pstate;
    s32 len;
    u8 bprofile[4];
    u8 vprofile[4];
    u8 rdata[32];

    ps2pad_state[i] = ps2pad_backup[i];
    pstate = scePad2GetState(ps2slot[i].socket_id);

    switch (pstate) {
    case scePad2StateStable:
        ps2slot[i].state = 2;
        break;

    case scePad2StateNoLink:
        ps2slot[i].state = 1;
        ps2slot[i].phase = 0;
        ps2slot[i].kind = 0;
        ps2slot[i].vib = 0;
        ps2slot[i].bprofile = 0;
        ps2slot[i].vprofile = 0;
        ps2pad_state[i] = ps2pad_clear;
        tarpad_root[i].kind = 0;
        tarpad_root[i].anstate = 0;
        tarpad_root[i].state = ps2slot[i].state;
        return;

    case scePad2StateExecCmd:
    case scePad2StateError:
    default:
        ps2slot[i].state = 2;
        ps2slot[i].phase = 0;
        ps2slot[i].kind = 0;
        ps2slot[i].vib = 0;
        ps2slot[i].bprofile = 0;
        ps2slot[i].vprofile = 0;
        ps2pad_state[i] = ps2pad_clear;
        tarpad_root[i].kind = 0;
        tarpad_root[i].anstate = 0;
        tarpad_root[i].state = ps2slot[i].state;
        return;
    }

    if (ps2slot[i].phase == 0) {
        len = scePad2GetButtonProfile(ps2slot[i].socket_id, bprofile);

        if (len < 0) {
            return;
        }

        ps2slot[i].bprofile = (bprofile[3] << 24) | (bprofile[2] << 16) | (bprofile[1] << 8) | bprofile[0];

        if (sceVibGetProfile(ps2slot[i].socket_id, vprofile) >= 0) {
            ps2slot[i].vprofile = vprofile[0];
        }

        if (ps2slot[i].bprofile & 0xFFFF0000) {
            if (ps2slot[i].bprofile & 0xFFF00000) {
                ps2slot[i].pad_id = 2;
            } else {
                ps2slot[i].pad_id = 1;
            }
        } else {
            ps2slot[i].pad_id = 0;
        }

        switch (ps2slot[i].pad_id) {
        case 0:
            tarpad_root[i].kind = 1;
            tarpad_root[i].anstate = 0;

            for (lp0 = 0; lp0 < 16; lp0++) {
                ps2pad_hard_to_soft[i][lp0][0] = ps2pad_hard_to_soft_dg[lp0][0];
                ps2pad_hard_to_soft[i][lp0][1] = ps2pad_hard_to_soft_dg[lp0][1];
            }

            break;

        case 1:
            tarpad_root[i].kind = 1;
            tarpad_root[i].anstate = 0x60;

            for (lp0 = 0; lp0 < 16; lp0++) {
                ps2pad_hard_to_soft[i][lp0][0] = ps2pad_hard_to_soft_ds2[lp0][0];
                ps2pad_hard_to_soft[i][lp0][1] = ps2pad_hard_to_soft_ds2[lp0][1];
            }

            break;

        case 2:
            tarpad_root[i].kind = 1;
            tarpad_root[i].anstate = 0x73;

            for (lp0 = 0; lp0 < 16; lp0++) {
                ps2pad_hard_to_soft[i][lp0][0] = ps2pad_hard_to_soft_ds2[lp0][0];
                ps2pad_hard_to_soft[i][lp0][1] = ps2pad_hard_to_soft_ds2[lp0][1];
            }

            break;
        }

        if (!(ps2slot[i].vprofile & 3)) {
            ps2slot[i].vib = 0;
        } else {
            ps2slot[i].vib = 1;
        }

        ps2slot[i].phase += 1;
    } else {
        if ((len = scePad2Read(ps2slot[i].socket_id, (scePad2ButtonState*)rdata)) < 0) {
            return;
        }

        ps2slot[i].state = 1;
        ps2pad_state[i].pad_buffer[0] = 0;
        ps2pad_state[i].pad_buffer[1] = 0;
        ps2pad_state[i].ix.sw = rdata[0] << 8 | rdata[1];

        switch (ps2slot[i].pad_id) {
        case 0:
            break;

        case 1:
            ps2pad_state[i].ix.pos.stick.r_ax = ((scePad2ButtonState*)rdata)->rJoyH;
            ps2pad_state[i].ix.pos.stick.r_ay = ((scePad2ButtonState*)rdata)->rJoyV;
            ps2pad_state[i].ix.pos.stick.l_ax = ((scePad2ButtonState*)rdata)->lJoyH;
            ps2pad_state[i].ix.pos.stick.l_ay = ((scePad2ButtonState*)rdata)->lJoyV;
            break;

        case 2:
            ps2pad_state[i].ix.pos.stick.r_ax = ((scePad2ButtonState*)rdata)->rJoyH;
            ps2pad_state[i].ix.pos.stick.r_ay = ((scePad2ButtonState*)rdata)->rJoyV;
            ps2pad_state[i].ix.pos.stick.l_ax = ((scePad2ButtonState*)rdata)->lJoyH;
            ps2pad_state[i].ix.pos.stick.l_ay = ((scePad2ButtonState*)rdata)->lJoyV;
            ps2pad_state[i].ix.depth[0] = ((scePad2ButtonState*)rdata)->rightP;
            ps2pad_state[i].ix.depth[1] = ((scePad2ButtonState*)rdata)->leftP;
            ps2pad_state[i].ix.depth[2] = ((scePad2ButtonState*)rdata)->upP;
            ps2pad_state[i].ix.depth[3] = ((scePad2ButtonState*)rdata)->downP;
            ps2pad_state[i].ix.depth[4] = ((scePad2ButtonState*)rdata)->triangleP;
            ps2pad_state[i].ix.depth[5] = ((scePad2ButtonState*)rdata)->circleP;
            ps2pad_state[i].ix.depth[6] = ((scePad2ButtonState*)rdata)->crossP;
            ps2pad_state[i].ix.depth[7] = ((scePad2ButtonState*)rdata)->squareP;
            ps2pad_state[i].ix.depth[8] = ((scePad2ButtonState*)rdata)->l1P;
            ps2pad_state[i].ix.depth[9] = ((scePad2ButtonState*)rdata)->r1P;
            ps2pad_state[i].ix.depth[10] = ((scePad2ButtonState*)rdata)->l2P;
            ps2pad_state[i].ix.depth[11] = ((scePad2ButtonState*)rdata)->r2P;
            break;

        default:
            ps2pad_state[i] = ps2pad_clear;
            break;
        }

        if (ps2slot[i].vib_timer != 0) {
            if (--ps2slot[i].vib_timer == 0) {
                flPADShockSet(i, 0, 0);
            }
        }
    }

    tarpad_root[i].state = ps2slot[i].state;
}

s32 flPADShockSet(s32 pad_id, u32 level, u32 time) {
    s32 vib_data_size;
    u8 vib_data[2];
    u8 profile;
    u8 big;
    u8 small;

    if (ps2slot[pad_id].vprofile == 0) {
        return 0;
    }

    ps2slot[pad_id].vib_timer = time;

    if (time == 0) {
        if ((ps2slot[pad_id].vprofile = 3) != 0) {
            profile = 3;
            vib_data_size = 2;
            vib_data[0] = 0;
            vib_data[1] = 0;
        } else {
            profile = 2;
            vib_data_size = 1;
            vib_data[0] = 0;
        }
    } else {
        level &= 7;
        small = 1;
        big = (level << 5) | 0xF;

        if ((ps2slot[pad_id].vprofile = 3) != 0) {
            profile = 3;
            vib_data_size = 2;
            vib_data[0] = ((big & 0x7F) << 1) | (small & 1);
            vib_data[1] = (big & 0x80) >> 7;
        } else {
            profile = 2;
            vib_data_size = 1;
            vib_data[0] = big;
        }
    }

    sceVibSetActParam(ps2slot[pad_id].socket_id, 1, &profile, vib_data_size, vib_data);
    return 1;
}
