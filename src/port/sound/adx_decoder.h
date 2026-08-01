#ifndef ADX_DECODER_H
#define ADX_DECODER_H

#include <SDL3/SDL.h>

#define CRI_COPYRIGHT_STR "(c)CRI"
#define ADX_CHANNELS_MAX 2
#define ADX_SAMPLE_BITDEPTH 4
#define ADX_BLOCK_SIZE 18
#define ADX_SAMPLES_PER_BLOCK ((ADX_BLOCK_SIZE - 2) * 8 / ADX_SAMPLE_BITDEPTH)

typedef enum ADXEncoding : Uint8 {
    ADX_ENCODING_PRESET = 2,
    ADX_ENCODING_STANDARD = 3,
    ADX_ENCODING_EXPONENTIAL = 4,
} ADXEncoding;

typedef struct ADXHeader {
    Uint32 copyright_offset;
    Uint32 data_offset;
    ADXEncoding encoding;
    Uint8 block_size;
    Uint8 sample_bitdepth;
    Uint8 channel_count;
    Uint32 sample_rate;
    Uint32 total_samples;
    Uint16 highpass_frequency;
    Uint8 version;
    Uint8 flags;
    bool loop_enabled;
    Uint16 loop_alignment_samples;
    Uint32 loop_begin_sample;
    Uint32 loop_end_sample;
    Uint32 loop_begin_byte;
    Uint32 loop_end_byte;
} ADXHeader;

typedef struct ADXDecoder {
    ADXHeader header;
    Sint32 coefficients[2];
    Uint32 sample_index;
    Sint32 past_samples[ADX_CHANNELS_MAX][2];
    SDL_IOStream* file;
} ADXDecoder;

// Decodes up to samples_needed samples per channel into interleaved PCM.
// Returns the number of samples decoded per channel.
Uint32 ADXDecoder_Decode(ADXDecoder* decoder, Sint16* buffer, Uint32 samples_needed);

bool ADXDecoder_Init(ADXDecoder* decoder, SDL_IOStream* file);

#endif
