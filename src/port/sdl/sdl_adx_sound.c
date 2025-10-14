#include "port/sdl/sdl_adx_sound.h"
#include "common.h"
#include "port/io/afs.h"
#include "sf33rd/Source/Game/io/gd3rd.h"

#include <SDL3/SDL.h>

#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/frame.h>
#include <libavutil/intreadwrite.h>
#include <libswresample/swresample.h>

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_RATE 48000
#define N_CHANNELS 2
#define BYTES_PER_SAMPLE 2
#define MIN_QUEUED_DATA_MS 400
#define MIN_QUEUED_DATA (int)((float)SAMPLE_RATE * MIN_QUEUED_DATA_MS / 1000 * N_CHANNELS * BYTES_PER_SAMPLE)
#define TRACKS_MAX 10

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct ADXDecoderPipeline {
    AVCodecContext* context;
    AVCodecParserContext* parser_context;
    SwrContext* swr;
    AVPacket* packet;
    AVFrame* frame;
} ADXDecoderPipeline;

typedef struct ADXLoopInfo {
    bool looping_enabled;
    int start_sample;
    int end_sample;
    uint8_t* data;
    int data_size;
    int position;
} ADXLoopInfo;

typedef struct ADXTrack {
    int size;
    uint8_t* data;
    bool should_free_data_after_use;
    int used_bytes;
    int processed_samples;
    ADXLoopInfo loop_info;
    ADXDecoderPipeline pipeline;
} ADXTrack;

static SDL_AudioStream* stream = NULL;
static ADXTrack tracks[TRACKS_MAX] = { 0 };
static int num_tracks = 0;
static int first_track_index = 0;
static bool has_tracks = false;

static int stream_data_needed() {
    return MIN_QUEUED_DATA - SDL_GetAudioStreamQueued(stream);
}

static bool stream_needs_data() {
    return stream_data_needed() > 0;
}

static bool stream_is_empty() {
    return SDL_GetAudioStreamQueued(stream) <= 0;
}

static void pipeline_init(ADXDecoderPipeline* pipeline) {
    const AVCodec* codec = avcodec_find_decoder(AV_CODEC_ID_ADPCM_ADX);
    pipeline->context = avcodec_alloc_context3(codec);
    avcodec_open2(pipeline->context, codec, NULL);
    pipeline->parser_context = av_parser_init(codec->id);

    const AVChannelLayout ch_layout = AV_CHANNEL_LAYOUT_STEREO;
    swr_alloc_set_opts2(&pipeline->swr,
                        &ch_layout,
                        AV_SAMPLE_FMT_S16,
                        SAMPLE_RATE,
                        &ch_layout,
                        AV_SAMPLE_FMT_S16P,
                        SAMPLE_RATE,
                        0,
                        NULL);
    swr_init(pipeline->swr);

    pipeline->packet = av_packet_alloc();
    pipeline->frame = av_frame_alloc();
}

static void pipeline_destroy(ADXDecoderPipeline* pipeline) {
    av_packet_free(&pipeline->packet);
    av_frame_free(&pipeline->frame);
    swr_free(&pipeline->swr);
    avcodec_free_context(&pipeline->context);
    av_parser_close(pipeline->parser_context);
}

static void* load_file(int file_id, int* size) {
    // FIXME: Remove dependency on GD3rd.h
    const unsigned int file_size = fsGetFileSize(file_id);
    *size = file_size;
    const size_t buff_size = (file_size + 2048 - 1) & ~(2048 - 1); // AFS reads data in 2048-byte chunks
    void* buff = malloc(buff_size);

    AFSHandle handle = AFS_Open(file_id);
    AFS_ReadSync(handle, fsCalSectorSize(file_size), buff);
    AFS_Close(handle);

    return buff;
}

static void print_av_error(int errnum) {
    char errbuf[AV_ERROR_MAX_STRING_SIZE] = { 0 };
    av_strerror(errnum, errbuf, sizeof(errbuf));
    fprintf(stderr, "FFmpeg error: %s\n", errbuf);
}

static bool track_reached_eof(ADXTrack* track) {
    return (track->size - track->used_bytes) <= 0;
}

static bool track_loop_filled(ADXTrack* track) {
    if (track->loop_info.looping_enabled) {
        return track->processed_samples >= track->loop_info.end_sample;
    } else {
        return false;
    }
}

static bool track_needs_decoding(ADXTrack* track) {
    if (track->loop_info.looping_enabled) {
        return !track_loop_filled(track);
    } else {
        return !track_reached_eof(track);
    }
}

static bool track_exhausted(ADXTrack* track) {
    if (track->loop_info.looping_enabled) {
        return false; // Track is never exhausted, because it can be looped infinitely
    } else {
        return track_reached_eof(track);
    }
}

static int track_add_samples_to_loop(ADXTrack* track, uint8_t* buf, int num_samples) {
    ADXLoopInfo* loop_info = &track->loop_info;

    if (!loop_info->looping_enabled) {
        return 0; // No need to add samples if looping is not enabled
    }

    const int buf_sample_start = MAX(loop_info->start_sample - track->processed_samples, 0);
    const int buf_sample_end = MIN(loop_info->end_sample - track->processed_samples, num_samples);

    if (buf_sample_end > buf_sample_start) {
        const int buf_start = buf_sample_start * N_CHANNELS * BYTES_PER_SAMPLE;
        const int buf_end = buf_sample_end * N_CHANNELS * BYTES_PER_SAMPLE;
        const int buf_len = buf_end - buf_start;
        memcpy(loop_info->data + loop_info->position, buf + buf_sample_start, buf_len);
        loop_info->position += buf_len;

        if (loop_info->position == loop_info->data_size) {
            loop_info->position = 0;
        }
    }

    const int overflow = MAX(track->processed_samples + num_samples - loop_info->end_sample, 0);
    track->processed_samples += num_samples;
    return overflow;
}

static void loop_info_init(ADXLoopInfo* info, const uint8_t* data) {
    const uint8_t version = data[0x12];

    switch (version) {
    case 3:
        const Uint16 loop_enabled_16 = AV_RB16(data + 0x16);

        if (loop_enabled_16 == 1) {
            info->looping_enabled = true;
            info->start_sample = AV_RB32(data + 0x1C);
            info->end_sample = AV_RB32(data + 0x24);
        }

        break;

    case 4:
        const Uint32 loop_enabled_32 = AV_RB32(data + 0x24);

        if (loop_enabled_32 == 1) {
            info->looping_enabled = true;
            info->start_sample = AV_RB32(data + 0x28);
            info->end_sample = AV_RB32(data + 0x30);
        }

        break;

    default:
        fatal_error("Unhandled ADX version: %d", version);
        break;
    }

    if (info->looping_enabled) {
        info->data_size = (info->end_sample - info->start_sample) * BYTES_PER_SAMPLE * N_CHANNELS;
        info->data = malloc(info->data_size);
        info->position = 0;
    }
}

static void loop_info_destroy(ADXLoopInfo* info) {
    if (info->looping_enabled) {
        free(info->data);
    }

    SDL_zerop(info);
}

static void process_track(ADXTrack* track) {
    ADXDecoderPipeline* pipeline = &track->pipeline;

    // Decode samples and queue them for playback
    while (stream_needs_data() && track_needs_decoding(track)) {
        int ret = av_parser_parse2(pipeline->parser_context,
                                   pipeline->context,
                                   &pipeline->packet->data,
                                   &pipeline->packet->size,
                                   track->data + track->used_bytes,
                                   track->size - track->used_bytes,
                                   AV_NOPTS_VALUE,
                                   AV_NOPTS_VALUE,
                                   0);

        if (ret < 0) {
            print_av_error(ret);
            break;
        }

        track->used_bytes += ret;

        if (pipeline->packet->size > 0) {
            // Send parsed packet to decoder
            ret = avcodec_send_packet(pipeline->context, pipeline->packet);

            if (ret < 0) {
                print_av_error(ret);
                break;
            }

            // Receive all available frames
            while (ret >= 0) {
                ret = avcodec_receive_frame(pipeline->context, pipeline->frame);

                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    print_av_error(ret);
                    break;
                }

                const int out_channels = pipeline->frame->ch_layout.nb_channels;
                const int out_samples = pipeline->frame->nb_samples;

                // Allocate buffer for interleaved samples
                uint8_t* out_buf = NULL;
                int out_linesize = 0;

                av_samples_alloc(&out_buf, &out_linesize, out_channels, out_samples, AV_SAMPLE_FMT_S16, 0);

                // Convert planar → interleaved
                const int samples_converted = swr_convert(
                    pipeline->swr, &out_buf, out_samples, (const uint8_t**)pipeline->frame->data, out_samples);

                const int overflow = track_add_samples_to_loop(track, out_buf, samples_converted);
                const int samples_to_queue = samples_converted - overflow;

                const int out_size =
                    av_samples_get_buffer_size(&out_linesize, out_channels, samples_to_queue, AV_SAMPLE_FMT_S16, 1);

                SDL_PutAudioStreamData(stream, out_buf, out_size);
                av_freep(&out_buf);
            }
        }
    }

    // Queue looped samples (if needed)
    while (track_loop_filled(track) && stream_needs_data()) {
        const int available_data = track->loop_info.data_size - track->loop_info.position;
        const int data_to_queue = MIN(stream_data_needed(), available_data);
        SDL_PutAudioStreamData(stream, track->loop_info.data + track->loop_info.position, data_to_queue);
        track->loop_info.position += data_to_queue;

        if (track->loop_info.position == track->loop_info.data_size) {
            track->loop_info.position = 0;
        }
    }
}

static void track_init(ADXTrack* track, int file_id, void* buf, size_t buf_size, bool looping_allowed) {
    if (file_id == -1 && buf == NULL) {
        fatal_error("One of file_id or buf must be valid.");
    }

    if (file_id != -1) {
        track->data = load_file(file_id, &track->size);
        track->should_free_data_after_use = true;
    } else {
        track->data = buf;
        track->size = buf_size;
        track->should_free_data_after_use = false;
    }

    track->used_bytes = 0;
    pipeline_init(&track->pipeline);

    if (looping_allowed) {
        loop_info_init(&track->loop_info, track->data);
    }

    process_track(track); // Feed first batch of data to the stream
}

static void track_destroy(ADXTrack* track) {
    pipeline_destroy(&track->pipeline);
    loop_info_destroy(&track->loop_info);

    if (track->should_free_data_after_use) {
        free(track->data);
    }

    SDL_zerop(track);
}

static ADXTrack* alloc_track() {
    const int index = (first_track_index + num_tracks) % TRACKS_MAX;
    num_tracks += 1;
    has_tracks = true;
    return &tracks[index];
}

void SDLADXSound_ProcessTracks() {
    const int first_track_index_old = first_track_index;
    const int num_tracks_old = num_tracks;

    for (int i = 0; i < num_tracks_old; i++) {
        const int j = (first_track_index_old + i) % TRACKS_MAX;
        ADXTrack* track = &tracks[j];
        process_track(track);

        if (!track_exhausted(track)) {
            // No need to continue if the current track is not exhausted yet
            break;
        }

        track_destroy(track);
        num_tracks -= 1;

        if (num_tracks > 0) {
            first_track_index += 1;
        } else {
            first_track_index = 0;
        }
    }
}

void SDLADXSound_Init() {
    const SDL_AudioSpec spec = { .format = SDL_AUDIO_S16, .channels = N_CHANNELS, .freq = SAMPLE_RATE };
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
}

void SDLADXSound_Exit() {
    SDLADXSound_Stop();
    SDL_DestroyAudioStream(stream);
}

void SDLADXSound_Stop() {
    SDLADXSound_Pause(true);
    SDL_ClearAudioStream(stream);

    for (int i = 0; i < num_tracks; i++) {
        const int j = (first_track_index + i) % TRACKS_MAX;
        track_destroy(&tracks[j]);
    }

    num_tracks = 0;
    first_track_index = 0;
    has_tracks = false;
}

int SDLADXSound_IsPaused() {
    return SDL_AudioStreamDevicePaused(stream);
}

void SDLADXSound_Pause(int pause) {
    if (pause) {
        SDL_PauseAudioStreamDevice(stream);
    } else {
        SDL_ResumeAudioStreamDevice(stream);
    }
}

void SDLADXSound_StartMem(void* buf, size_t size) {
    SDLADXSound_Stop();

    ADXTrack* track = alloc_track();
    track_init(track, -1, buf, size, true);
}

int SDLADXSound_GetNumFiles() {
    return num_tracks;
}

void SDLADXSound_EntryAfs(int file_id) {
    ADXTrack* track = alloc_track();
    track_init(track, file_id, NULL, 0, false);
}

void SDLADXSound_StartSeamless() {
    SDLADXSound_Pause(false);
}

void SDLADXSound_ResetEntry() {
    // ResetEntry is always called after Stop, so we don't need to do anything here
}

void SDLADXSound_StartAfs(int file_id) {
    SDLADXSound_Stop();

    ADXTrack* track = alloc_track();
    track_init(track, file_id, NULL, 0, true);
}

void SDLADXSound_SetOutVol(int volume) {
    // Convert volume (dB * 10) to linear gain
    const float gain = powf(10.0f, volume / 200.0f);
    SDL_SetAudioStreamGain(stream, gain);
}

void ADX_SetMono(bool mono) {
    // FIXME: Do we really need this?
}

ADXState SDLADXSound_GetState() {
    if (!has_tracks) {
        return ADX_STATE_STOP;
    }

    if (stream_is_empty()) {
        return ADX_STATE_PLAYEND;
    } else {
        if (SDLADXSound_IsPaused()) {
            return ADX_STATE_STOP;
        } else {
            return ADX_STATE_PLAYING;
        }
    }
}
