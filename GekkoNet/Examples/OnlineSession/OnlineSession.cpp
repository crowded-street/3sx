#include "SDL2/SDL.h"

#define GEKKONET_STATIC

#include "gekkonet.h"

#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <thread>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool running = false;

using micro = std::chrono::microseconds;
using gduration = std::chrono::duration<unsigned int, std::ratio<1, 60>>;
using gtime_point = std::chrono::time_point<std::chrono::steady_clock>;
using slow_frame = std::chrono::duration<unsigned int, std::ratio<1, 59>>;
using normal_frame = std::chrono::duration<unsigned int, std::ratio<1, 60>>;
using fast_frame = std::chrono::duration<unsigned int, std::ratio<1, 61>>;
using gclock = std::chrono::steady_clock;

bool init_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow(
        "GekkoNet Example: Online Session",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        300,
        300,
        0
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }
    return true;
}

void del_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

struct GInput {
    union inp {
        struct dir {
            char up : 1;
            char down : 1;
            char left : 1;
            char right : 1;
        }dir;
        unsigned char value;
    }input;
};

void process_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
                break;
            }
        }
    }
}

struct GState {
    int px[2]{ 0, 0};
    int py[2]{ 0, 0};
    char desync = 0;
};

void update_state(GState& gs, GInput inputs[2], int num_players) {
    for (int player = 0; player < num_players; player++) {
        if (inputs[player].input.dir.up) gs.py[player] -= 2;
        if (inputs[player].input.dir.down) gs.py[player] += 2;
        if (inputs[player].input.dir.left) gs.px[player] -= 2;
        if (inputs[player].input.dir.right)gs.px[player] += 2;
    }
}

// https://en.wikipedia.org/wiki/Fletcher%27s_checksum
uint32_t fletcher32(const uint16_t* data, size_t len) {
    uint32_t c0, c1;
    len = (len + 1) & ~1;      /* Round up len to words */

    /* We similarly solve for n > 0 and n * (n+1) / 2 * (2^16-1) < (2^32-1) here. */
    /* On modern computers, using a 64-bit c0/c1 could allow a group size of 23726746. */
    for (c0 = c1 = 0; len > 0; ) {
        size_t blocklen = len;
        if (blocklen > 360 * 2) {
            blocklen = 360 * 2;
        }
        len -= blocklen;
        do {
            c0 = c0 + *data++;
            c1 = c1 + c0;
        } while ((blocklen -= 2));
        c0 = c0 % 65535;
        c1 = c1 % 65535;
    }
    return (c1 << 16 | c0);
}

void save_state(GState* gs, GekkoGameEvent* ev) {
    *ev->data.save.state_len = sizeof(GState);
    *ev->data.save.checksum = fletcher32((uint16_t*)gs, sizeof(GState));
    std::memcpy(ev->data.save.state, gs, sizeof(GState));
}

void load_state(GState* gs, GekkoGameEvent* ev) {
    std::memcpy(gs, ev->data.load.state, sizeof(GState));
}

void render_state(GState& gs) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int player = 0; player < 2; player++)
    {
        SDL_Rect ball_rect = {
            gs.px[player],
            gs.py[player],
            40,
            40
        };
        SDL_RenderFillRect(renderer, &ball_rect);
    }
}

GInput get_key_inputs() {
    GInput input{};
    // reset inputs
    input.input.value = 0;
    // fetch local inputs
    auto keys = SDL_GetKeyboardState(NULL);
    // local player
    input.input.dir.up = keys[SDL_SCANCODE_W];
    input.input.dir.left = keys[SDL_SCANCODE_A];
    input.input.dir.down = keys[SDL_SCANCODE_S];
    input.input.dir.right = keys[SDL_SCANCODE_D];
    return input;
}

float GetFrameTime(float frames_ahead) {
    if (frames_ahead >= 0.75f) {
        return std::chrono::duration<float>(slow_frame(1)).count();
    }
    else {
        return std::chrono::duration<float>(normal_frame(1)).count();
    }
}

int main(int argc, char* args[])
{
    std::vector<std::string> all_args;

    if (argc > 1) {
        all_args.assign(args + 1, args + argc);
    }

    if (all_args.size() != 4) {
        printf("args found = %d\n", (int)all_args.size());
        return 0;
    }

    int localplayer = std::stoi(all_args[0]);
    const int localport = std::stoi(all_args[1]);
    std::string remote_address = std::move(all_args[2]);
    int localdelay = std::stoi(all_args[3]);

    printf("lplayer:%d, lport:%d, rem_addr:%s\n", localplayer, localport, remote_address.c_str());

    running = init_window();

    GState state = {};
    // state.desync = localplayer;

    GInput inputs[2] = {};

    int num_players = 2;

    GekkoSession* sess = nullptr;
    GekkoConfig conf {};

    conf.num_players = num_players;
    conf.input_size = sizeof(char);
    conf.state_size = sizeof(GState);
    conf.max_spectators = 0;
    conf.input_prediction_window = 10;
    conf.desync_detection = true;
    // conf.limited_saving = true;

    gekko_create(&sess);
    gekko_start(sess, &conf);
    gekko_net_adapter_set(sess, gekko_default_adapter(localport));

    // this is order dependant so we have to keep that in mind. 
    if (localplayer == 0) {
        // add local player
        localplayer = gekko_add_actor(sess, LocalPlayer, nullptr); 
        // add remote player
        auto remote = GekkoNetAddress{ (void*)remote_address.c_str(), (unsigned int)remote_address.size() };
        gekko_add_actor(sess, RemotePlayer, &remote);
    } else {
        // add remote player
        auto remote = GekkoNetAddress{ (void*)remote_address.c_str(), (unsigned int)remote_address.size() };
        gekko_add_actor(sess, RemotePlayer, &remote);
        // add local player
        localplayer = gekko_add_actor(sess, LocalPlayer, nullptr);
    }

    gekko_set_local_delay(sess, localplayer, localdelay);

    int current = 0;

    // timing
    auto curr_time = gclock::now();
    auto prev_time(gclock::now());

    float delta_time = 0.f;
    float accumulator = 0.f;
    float frame_time = 0.f;
    float frames_ahead = 0.f;

    while (running) {
        curr_time = gclock::now();

        frames_ahead = gekko_frames_ahead(sess);
        frame_time = GetFrameTime(frames_ahead);

        delta_time = std::chrono::duration<float>(curr_time - prev_time).count();
        prev_time = curr_time;

        accumulator += delta_time;

        gekko_network_poll(sess);

        while (accumulator >= frame_time) {

            GekkoNetworkStats stats{};
            gekko_network_stats(sess, localplayer == 0 ? 1 : 0, &stats);

            std::cout << "ping: " << stats.last_ping
                << " avg ping: " << stats.avg_ping
                << " jitter: " << stats.jitter
                << " ft: " << frame_time
                << " fa: " << frames_ahead
                << std::endl;

            process_events();

            //add local inputs to the session
            auto input = get_key_inputs();
            gekko_add_local_input(sess, localplayer, &input);

            int count = 0;
            auto events = gekko_session_events(sess, &count);
            for (int i = 0; i < count; i++) {
                auto event = events[i];

                printf("EV: %d\n", event->type);

                if (event->type == DesyncDetected) {
                    auto desync = event->data.desynced;
                    printf("desync detected, f:%d, rh:%d, lc:%u, rc:%u\n", desync.frame, desync.remote_handle, desync.local_checksum, desync.remote_checksum);
                }

                if (event->type == PlayerDisconnected) {
                    auto disco = event->data.disconnected;
                    printf("disconnect detected, player: %d\n", disco.handle);
                }
            }

            count = 0;
            auto updates = gekko_update_session(sess, &count);
            for (int i = 0; i < count; i++) {
                auto ev = updates[i];

                switch (ev->type) {
                case SaveEvent:
                    save_state(&state, ev);
                    break;
                case LoadEvent:
                    load_state(&state, ev);
                    break;
                case AdvanceEvent:
                    // on advance event, advance the gamestate using the given inputs
                    inputs[0].input.value = ev->data.adv.inputs[0];
                    inputs[1].input.value = ev->data.adv.inputs[1];
                    current = ev->data.adv.frame;
                    // now we can use them to update state.
                    update_state(state, inputs, num_players);
                    break;
                default:
                    printf("Unknown Event: %d\n", ev->type);
                    break;
                }
            }
            // frame done.
            accumulator -= frame_time;
        }
        // draw the state every iteration
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_state(state);
        SDL_RenderPresent(renderer);
    }

    del_window();

    gekko_destroy(sess);

    return 0;
}
