#include "port/sound/adx.h"

#if SOUND_ENABLED

#include "port/io/afs.h"
#include "port/sound/adx_decoder.h"
#include "port/utils.h"

#include <SDL3/SDL.h>

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

typedef struct ADXLoopInfo {
    Uint8* pcm;
    Uint32 pcm_size;
    Uint32 position;
} ADXLoopInfo;

typedef struct ADXTrack {
    size_t size;
    uint8_t* data;
    bool should_free_data_after_use;
    ADXDecoder decoder;
    SDL_IOStream* decoder_file;
    ADXLoopInfo loop_info;
} ADXTrack;

static SDL_AudioStream* stream = NULL;
static ADXTrack tracks[TRACKS_MAX] = { 0 };
static int num_tracks = 0;
static int first_track_index = 0;
static bool has_tracks = false;

static bool audio_available() {
    return stream != NULL;
}

static int stream_data_needed() {
    return MIN_QUEUED_DATA - SDL_GetAudioStreamQueued(stream);
}

static bool stream_is_empty() {
    return SDL_GetAudioStreamQueued(stream) <= 0;
}

static void* load_file(int file_id, size_t* size) {
    const size_t file_size = AFS_GetSize(file_id);
    *size = file_size;
    void* buff = SDL_malloc(file_size);

    AFSHandle handle = AFS_Open(file_id);
    AFS_ReadSync(handle, buff);
    AFS_Close(handle);

    return buff;
}

static bool track_reached_eof(ADXTrack* track) {
    return track->decoder.sample_index >= track->decoder.header.total_samples;
}

static bool track_loop_filled(ADXTrack* track) {
    if (track->loop_info.pcm != NULL) {
        return track->decoder.sample_index >= track->decoder.header.loop_end_sample;
    } else {
        return false;
    }
}

static bool track_needs_decoding(ADXTrack* track) {
    if (track->loop_info.pcm != NULL) {
        return !track_loop_filled(track);
    } else {
        return !track_reached_eof(track);
    }
}

static bool track_exhausted(ADXTrack* track) {
    if (track->loop_info.pcm != NULL) {
        return false; // Track is never exhausted, because it can be looped infinitely
    } else {
        return track_reached_eof(track);
    }
}

static Uint32 track_add_samples_to_loop(
    ADXTrack* track, const Sint16* buffer, Uint32 sample_count, Uint32 buffer_start_sample
) {
    ADXLoopInfo* loop_info = &track->loop_info;
    const ADXHeader* header = &track->decoder.header;

    if (loop_info->pcm == NULL) {
        return 0; // No need to add samples if looping is not enabled
    }

    const Uint32 buffer_end_sample = buffer_start_sample + sample_count;
    const Uint32 copy_start = SDL_max(buffer_start_sample, header->loop_begin_sample);
    const Uint32 copy_end = SDL_min(buffer_end_sample, header->loop_end_sample);

    if (copy_end > copy_start) {
        const Uint32 copy_samples = copy_end - copy_start;
        const Uint32 source_offset = (copy_start - buffer_start_sample) * N_CHANNELS;
        const Uint32 copy_size = copy_samples * N_CHANNELS * BYTES_PER_SAMPLE;
        SDL_memcpy(loop_info->pcm + loop_info->position, buffer + source_offset, copy_size);
        loop_info->position += copy_size;

        if (loop_info->position == loop_info->pcm_size) {
            loop_info->position = 0;
        }
    }

    return buffer_end_sample > header->loop_end_sample ? buffer_end_sample - header->loop_end_sample : 0;
}

static void loop_info_init(ADXLoopInfo* info, const ADXHeader* header) {
    if (!header->loop_enabled) {
        return;
    }

    info->pcm_size = (header->loop_end_sample - header->loop_begin_sample) * BYTES_PER_SAMPLE * N_CHANNELS;
    info->pcm = SDL_malloc(info->pcm_size);
    info->position = 0;
}

static void loop_info_destroy(ADXLoopInfo* info) {
    if (info->pcm) {
        SDL_free(info->pcm);
    }

    SDL_zerop(info);
}

static void process_track(ADXTrack* track) {
    if ((stream_data_needed() > 0) && track_needs_decoding(track)) {
        const Uint32 samples_needed = stream_data_needed() / (BYTES_PER_SAMPLE * N_CHANNELS);

        if (samples_needed == 0) {
            return;
        }

        const Uint32 sample_size = samples_needed * N_CHANNELS * BYTES_PER_SAMPLE;
        const Uint32 buffer_start_sample = track->decoder.sample_index;
        Sint16* buffer = SDL_malloc(sample_size);
        const Uint32 decoded_samples = ADXDecoder_Decode(&track->decoder, buffer, samples_needed);

        if (track->decoder.header.channel_count == 1) {
            for (Uint32 i = decoded_samples; i > 0; i--) {
                const Sint16 sample = buffer[i - 1];
                buffer[(i - 1) * N_CHANNELS] = sample;
                buffer[(i - 1) * N_CHANNELS + 1] = sample;
            }
        }

        const Uint32 overflow = track_add_samples_to_loop(track, buffer, decoded_samples, buffer_start_sample);
        SDL_PutAudioStreamData(stream, buffer, (decoded_samples - overflow) * BYTES_PER_SAMPLE * N_CHANNELS);
        SDL_free(buffer);
    }

    // Queue looped samples (if needed)
    while (track_loop_filled(track) && (stream_data_needed() > 0)) {
        const int available_data = track->loop_info.pcm_size - track->loop_info.position;
        const int data_to_queue = SDL_min(stream_data_needed(), available_data);
        SDL_PutAudioStreamData(stream, track->loop_info.pcm + track->loop_info.position, data_to_queue);
        track->loop_info.position += data_to_queue;

        if (track->loop_info.position == track->loop_info.pcm_size) {
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

    track->decoder_file = SDL_IOFromConstMem(track->data, track->size);

    if (track->decoder_file == NULL || !ADXDecoder_Init(&track->decoder, track->decoder_file)) {
        fatal_error("Failed to initialize ADX decoder: %s", SDL_GetError());
    }

    SDL_zerop(&track->loop_info);

    if (looping_allowed) {
        loop_info_init(&track->loop_info, &track->decoder.header);
    }

    process_track(track); // Feed first batch of data to the stream
}

static void track_destroy(ADXTrack* track) {
    loop_info_destroy(&track->loop_info);
    SDL_CloseIO(track->decoder_file);

    if (track->should_free_data_after_use) {
        SDL_free(track->data);
    }

    SDL_zerop(track);
}

static ADXTrack* alloc_track() {
    if (num_tracks >= TRACKS_MAX) {
        fatal_error("Too many queued ADX tracks.");
    }

    const int index = (first_track_index + num_tracks) % TRACKS_MAX;
    num_tracks += 1;
    has_tracks = true;
    return &tracks[index];
}

void ADX_ProcessTracks() {
    if (!audio_available()) {
        return;
    }

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
            first_track_index = (first_track_index + 1) % TRACKS_MAX;
        } else {
            first_track_index = 0;
        }
    }
}

void ADX_Init() {
    const SDL_AudioSpec spec = { .format = SDL_AUDIO_S16, .channels = N_CHANNELS, .freq = SAMPLE_RATE };
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);

    if (stream == NULL) {
        SDL_Log("Failed to create audio stream: %s; continuing without sound", SDL_GetError());
    }
}

void ADX_Exit() {
    if (!audio_available()) {
        return;
    }

    ADX_Stop();
    SDL_DestroyAudioStream(stream);
    stream = NULL;
}

void ADX_Stop() {
    if (!audio_available()) {
        return;
    }

    ADX_Pause(true);
    SDL_ClearAudioStream(stream);

    for (int i = 0; i < num_tracks; i++) {
        const int j = (first_track_index + i) % TRACKS_MAX;
        track_destroy(&tracks[j]);
    }

    num_tracks = 0;
    first_track_index = 0;
    has_tracks = false;
}

int ADX_IsPaused() {
    if (!audio_available()) {
        return 1;
    }

    return SDL_AudioStreamDevicePaused(stream);
}

void ADX_Pause(int pause) {
    if (!audio_available()) {
        return;
    }

    if (pause) {
        SDL_PauseAudioStreamDevice(stream);
    } else {
        SDL_ResumeAudioStreamDevice(stream);
    }
}

void ADX_StartMem(void* buf, size_t size) {
    if (!audio_available()) {
        return;
    }

    ADX_Stop();

    ADXTrack* track = alloc_track();
    track_init(track, -1, buf, size, true);
}

int ADX_GetNumFiles() {
    if (!audio_available()) {
        return 0;
    }

    return num_tracks;
}

void ADX_EntryAfs(int file_id) {
    if (!audio_available()) {
        return;
    }

    ADXTrack* track = alloc_track();
    track_init(track, file_id, NULL, 0, false);
}

void ADX_StartSeamless() {
    if (!audio_available()) {
        return;
    }

    ADX_Pause(false);
}

void ADX_ResetEntry() {
    // ResetEntry is always called after Stop, so we don't need to do anything here
}

void ADX_StartAfs(int file_id) {
    if (!audio_available()) {
        return;
    }

    ADX_Stop();

    ADXTrack* track = alloc_track();
    track_init(track, file_id, NULL, 0, true);
}

void ADX_SetOutVol(int volume) {
    if (!audio_available()) {
        return;
    }

    // Convert volume (dB * 10) to linear gain
    const float gain = powf(10.0f, volume / 200.0f);
    SDL_SetAudioStreamGain(stream, gain);
}

ADXState ADX_GetState() {
    if (!audio_available()) {
        return ADX_STATE_STOP;
    }

    if (!has_tracks) {
        return ADX_STATE_STOP;
    }

    if (stream_is_empty()) {
        return ADX_STATE_PLAYEND;
    } else {
        if (ADX_IsPaused()) {
            return ADX_STATE_STOP;
        } else {
            return ADX_STATE_PLAYING;
        }
    }
}

#else

void ADX_ProcessTracks() {
    // Do nothing
}

void ADX_Init() {
    // Do nothing
}

void ADX_Exit() {
    // Do nothing
}

void ADX_Stop() {
    // Do nothing
}

int ADX_IsPaused() {
    return 1;
}

void ADX_Pause(int pause) {
    // Do nothing
}

void ADX_StartSeamless() {
    // Do nothing
}

void ADX_StartMem(void* buf, size_t size) {
    // Do nothing
}

int ADX_GetNumFiles() {
    return 0;
}

void ADX_EntryAfs(int file_id) {
    // Do nothing
}

void ADX_StartAfs(int file_id) {
    // Do nothing
}

void ADX_ResetEntry() {
    // Do nothing
}

void ADX_SetOutVol(int volume) {
    // Do nothing
}

ADXState ADX_GetState() {
    return ADX_STATE_STOP;
}

#endif
