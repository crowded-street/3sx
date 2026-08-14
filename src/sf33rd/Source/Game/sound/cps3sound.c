/**
 * @file cps3sound.c
 * CPS-3 sound request setup recovered from the 3rd Strike program ROM.
 */

#include <SDL3/SDL.h>

#define SS_CHANNEL_COUNT 16
#define SS_CHANNEL_STATE_FREE 0xC0
#define SS_READ_BE16(data) ((Uint16)(((Uint16)(data)[0] << 8) | (data)[1]))
#define SS_READ_BE32(data)                                                                                             \
    (((Uint32)(data)[0] << 24) | ((Uint32)(data)[1] << 16) | ((Uint32)(data)[2] << 8) | (data)[3])

typedef struct SsSampleHeader {
    Uint32 start;
    Uint32 loop_offset;
    Uint32 end_offset;
    Uint32 key;
} SsSampleHeader;
typedef struct {
    const Uint8* stream;
    const Uint8* track;
    const Uint8* instrument;
    const SsSampleHeader* sample_header;
    Uint32 unk_10;
    Uint32 unk_14;
    Uint32 unk_18;
    Uint32 unk_1C;
    Uint32 unk_20;
    Uint32 unk_24;
    Uint32 delay;
    Uint8 unk_2C[0x10];
    Uint16 unk_3C;
    Uint16 unk_3E;
    Uint16 unk_40;
    Uint16 unk_42;
    Uint16 unk_44;
    Uint16 unk_46;
    Uint16 unk_48;
    Uint16 unk_4A;
    Uint16 unk_4C;
    Uint16 unk_4E;
    Uint16 unk_50;
    Uint16 unk_52;
    Uint16 unk_54;
    Uint16 unk_56;
    Uint16 unk_58;
    Uint8 unk_5A;
    Uint8 unk_5B;
    Uint8 unk_5C;
    Uint8 unk_5D;
    Uint8 state;
    Uint8 unk_5F;
    Uint8 unk_60;
    Uint8 unk_61;
    Uint8 unk_62;
    Uint8 unk_63;
    Uint8 unk_64;
    Uint8 unk_65;
    Uint8 unk_66;
    Uint8 unk_67;
    Uint8 unk_68;
    Uint8 unk_69;
    Uint8 unk_6A;
    Uint8 unk_6B;
    Uint8 unk_6C;
    Uint8 unk_6D;
    Uint8 unk_6E;
    Uint8 priority;
    Uint8 unk_70;
    Uint8 unk_71[3];
} SsChannelState;

typedef struct {
    Sint16 start;
    Sint16 end;
    Sint16 step;
    Sint16 control;
} SsPanDescriptor;

typedef struct SsRequestBank {
    Uint16 request_count;
    Uint8 unk_2;
    Uint8 unk_3;
    Uint32 request_offsets[];
} SsRequestBank;

Uint32 ss_unk_02007EE8;                               /* 0x02007EE8 */
SsSampleHeader* ss_sample_headers;                    /* 0x02078CFC */
Uint8** ss_instrument_banks;                          /* 0x02078D00 */
Uint32 ss_unk_02078D04;                               /* 0x02078D04 */
SsChannelState ss_bgm_channels[SS_CHANNEL_COUNT];     /* 0x02078D0C */
SsChannelState ss_sfx_channels[SS_CHANNEL_COUNT];     /* 0x0207944C */
SsPanDescriptor ss_pan_descriptors[SS_CHANNEL_COUNT]; /* 0x02079B8C */
Uint32 ss_unk_02079C14;                               /* 0x02079C14 */
Uint32 ss_channel_aux[SS_CHANNEL_COUNT];              /* 0x02079C18 */
Uint8 ss_unk_02079C58;                                /* 0x02079C58 */
SsRequestBank* ss_request_data;                       /* 0x02079C6C */
SsPanDescriptor ss_pan_tmp;                           /* 0x02079C70 */
Uint16 ss_unk_02079C78;                               /* 0x02079C78 */
Uint16 ss_request_count;                              /* 0x02079C7A */
Uint8 ss_state_flags;                                 /* 0x02079C8C */

void SsResetBgmChannels();
Sint32 SsReadDelay(const Uint8* stream, Uint32* delay);

/* 0x061394D4 */
void SsRequestCore(Uint16 req_number, Sint16 pan_control) {
    req_number %= ss_request_count;

    const Uint32 request_offset = SDL_Swap32BE(ss_request_data->request_offsets[req_number]);

    if (request_offset == 0) {
        return;
    }

    const Uint8* request = (Uint8*)ss_request_data + request_offset;
    const Uint8 type = request[0];
    const Uint8* track_offsets = request + 1;

    if (type == 0) {
        SsResetBgmChannels();

        for (int i = 0; i < SDL_arraysize(ss_bgm_channels); i++) {
            SsChannelState* channel = &ss_bgm_channels[i];
            const Uint32 track_offset = SS_READ_BE16(track_offsets + i * 2);

            if (track_offset == 0) {
                channel->state = SS_CHANNEL_STATE_FREE;
                continue;
            }

            const Uint8* track = request + track_offset;
            channel->track = track;

            Uint32 delay;
            channel->stream = track + SsReadDelay(track, &delay);
            channel->delay = delay << 8;
            channel->state = 0x20;

            const Uint8* instrument_bank = ss_instrument_banks[0];
            const Uint8* instrument = instrument_bank + SS_READ_BE16(instrument_bank);
            channel->instrument = instrument;
            channel->sample_header = &ss_sample_headers[SS_READ_BE16(instrument + 4)];

            channel->unk_24 = 0;
            channel->unk_10 = 0;
            channel->unk_14 = 0;
            channel->unk_18 = 0;
            channel->unk_1C = 0;
            channel->unk_20 = 0;
            channel->unk_3C = 0;
            channel->unk_3E = 0;
            channel->unk_40 = 0;
            channel->unk_42 = 0;
            channel->unk_44 = 0;
            channel->unk_46 = 0;
            channel->unk_48 = 0;
            channel->unk_4A = 0;
            channel->unk_4C = 0;
            channel->unk_4E = 0;
            channel->unk_50 = 0;
            channel->unk_52 = 0;
            channel->unk_54 = 0;
            channel->unk_5A = 0;
            channel->unk_5B = 0;
            channel->unk_5C = 0;
            channel->unk_5D = 0;
            channel->unk_65 = 0;
            channel->unk_6C = 0;
            channel->unk_6E = 0;
            channel->priority = type;
            channel->unk_62 = 0;
            channel->unk_63 = 0;
            channel->unk_64 = 0x40;
            channel->unk_58 = 0;
            channel->unk_56 = 0;
            channel->unk_5F = 0;
            channel->unk_60 = 0x40;
            channel->unk_66 = 0;
            channel->unk_67 = 0;
            channel->unk_69 = 0;
            channel->unk_6A = 0;
            channel->unk_68 = 0;
            channel->unk_6B = 0;
            channel->unk_6D = 0x40;
            channel->unk_70 = 0;
        }

        if ((ss_state_flags & 8) == 0) {
            ss_unk_02078D04 = 0x8000;
        }

        ss_unk_02007EE8 = 0;
        ss_unk_02079C14 = 0;
        ss_state_flags |= 2;
    } else if ((type & 0x80) == 0) {
        for (int i = 0; i < SDL_arraysize(ss_sfx_channels); i++) {
            SsPanDescriptor* pan = &ss_pan_descriptors[i];
            SsChannelState* channel = &ss_sfx_channels[i];
            const Uint32 track_offset = SS_READ_BE16(track_offsets + i * 2);

            if ((track_offset == 0) || ((channel->priority > type) && ((channel->state & 0x80) == 0))) {
                continue;
            }

            const Uint8* track = request + track_offset;

            Uint32 delay;
            channel->stream = track + SsReadDelay(track, &delay);
            channel->delay = delay << 8;
            channel->state = 0;

            const Uint8* instrument_bank = ss_instrument_banks[0];
            const Uint8* instrument = instrument_bank + SS_READ_BE16(instrument_bank);
            channel->instrument = instrument;
            channel->sample_header = &ss_sample_headers[SS_READ_BE16(instrument + 4)];

            channel->unk_10 = 0;
            channel->unk_14 = 0;
            channel->unk_18 = 0;
            channel->unk_1C = 0;
            channel->unk_20 = 0;
            channel->unk_3C = 0;
            channel->unk_3E = 0;
            channel->unk_40 = 0;
            channel->unk_42 = 0;
            channel->unk_44 = 0;
            channel->unk_46 = 0;
            channel->unk_48 = 0;
            channel->unk_4A = 0;
            channel->unk_4C = 0;
            channel->unk_4E = 0;
            channel->unk_50 = 0;
            channel->unk_52 = 0;
            channel->unk_54 = 0;
            channel->unk_5A = 0;
            channel->unk_5B = 0;
            channel->unk_5C = 0;
            channel->unk_5D = 0;
            channel->unk_65 = 0;
            channel->unk_6C = 0;
            channel->unk_6D = 0x40;
            channel->unk_6E = 0;
            channel->priority = type;
            channel->unk_62 = 0;
            channel->unk_63 = 0x7F;
            channel->unk_64 = 0x7F;
            channel->unk_58 = 0;
            channel->unk_56 = 0;
            channel->unk_5F = 0;
            channel->unk_60 = 0x40;
            channel->unk_66 = 0;
            channel->unk_67 = 0;
            channel->unk_69 = 0;
            channel->unk_6A = 0;
            channel->unk_68 = 0;
            channel->unk_6D = 0x40;
            channel->unk_70 = 0;

            if (pan_control == -1) {
                pan->start = 0;
                pan->end = 0;
                pan->step = 0;
                pan->control = -1;
            } else {
                *pan = ss_pan_tmp;
            }

            ss_channel_aux[i] = 0;
        }
    } else {
        const int channel_index = type & 0x0F;
        SsChannelState* channel = &ss_sfx_channels[channel_index];
        // In compact, fixed-channel requests track data follows type
        const Uint8* track = request + 1;

        if (((channel->priority & 0x7F) > (track[0] & 0x7F)) && ((channel->state & 0x80) == 0)) {
            return;
        }

        channel->stream = track;

        const Uint8* instrument_bank = ss_instrument_banks[0];
        const Uint8* instrument = instrument_bank + SS_READ_BE16(instrument_bank);
        channel->instrument = instrument;
        channel->sample_header = &ss_sample_headers[SS_READ_BE16(instrument + 4)];

        channel->unk_42 = 0;
        channel->unk_48 = 0;
        channel->unk_4A = 0;
        channel->unk_4C = 0;
        channel->unk_4E = 0;
        channel->unk_50 = 0;
        channel->unk_52 = 0;
        channel->unk_54 = 0;
        channel->unk_62 = 0;
        channel->unk_63 = 0x7F;
        channel->unk_64 = 0x7F;
        channel->unk_58 = 0;
        channel->unk_56 = 0;
        channel->unk_5F = 0;
        channel->unk_60 = 0x40;
        channel->priority = track[0] | 0x80;
        channel->unk_6D = 0x40;

        SsPanDescriptor* pan = &ss_pan_descriptors[channel_index];

        if (pan_control == -1) {
            pan->start = 0;
            pan->end = 0;
            pan->step = 0;
            pan->control = -1;
        } else {
            *pan = ss_pan_tmp;
        }

        channel->state = 0;
    }
}

/* 0x0613A1E8 */
void SsResetBgmChannels() {
    if ((ss_state_flags & 2) == 0) {
        return;
    }

    for (int i = 0; i < SDL_arraysize(ss_bgm_channels); i++) {
        ss_bgm_channels[i].state = SS_CHANNEL_STATE_FREE;
    }

    ss_state_flags = 1;
    ss_unk_02079C78 = 0;
}

/* 0x0613BCC0 */
Sint32 SsReadDelay(const Uint8* stream, Uint32* delay) {
    const Uint8* cursor = stream;
    Uint32 value = 0;

    while ((*cursor & 0x80) == 0) {
        value = (value * 0x80) + *cursor;
        cursor++;
    }

    *delay = value;
    return (Sint32)(cursor - stream);
}
