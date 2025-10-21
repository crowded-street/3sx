#include "SDL2/SDL.h"

#define GEKKONET_STATIC

#include "gekkonet.h"

#include <chrono>
#include <iostream>
#include <vector>

SDL_Window* window1 = nullptr;
SDL_Window* window2 = nullptr;

SDL_Renderer* renderer1 = nullptr;
SDL_Renderer* renderer2 = nullptr;

bool running = false;

bool init_windows(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}
	window1 = SDL_CreateWindow(
		"GekkoNet Example: Fake Online Session 1: Playing",
		0,
		50,
		600,
		400,
		0
	);

	window2 = SDL_CreateWindow(
		"GekkoNet Example: Fake Online Session 2: Spectating",
		600,
		50,
		600,
		400,
		0
	);

	if (!window1 || !window2) {
		fprintf(stderr, "Error creating SDL Window.\n");
		return false;
	}

	renderer1 = SDL_CreateRenderer(window1, -1, 0);
	renderer2 = SDL_CreateRenderer(window2, -1, 0);

	if (!renderer1 || !renderer2) {
		fprintf(stderr, "Error creating SDL Renderer.\n");
		return false;
	}
	return true;
}

void del_windows(void) {
	SDL_DestroyRenderer(renderer1);
	SDL_DestroyRenderer(renderer2);
	SDL_DestroyWindow(window1);
	SDL_DestroyWindow(window2);
	SDL_Quit();
}

struct GInput {
	union inp {
		struct dir {
			char up : 1;
			char left : 1;
			char down : 1;
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
			break;
		}
	}
}

struct GState {
	int px[2] = { 0, 0 };
	int py[2] = { 0, 0 };
};

void save_state(GState* gs, GekkoGameEvent* ev) {
    *ev->data.save.checksum = 0;
    *ev->data.save.state_len = sizeof(GState);
    std::memcpy(ev->data.save.state, gs, sizeof(GState));
}

void load_state(GState* gs, GekkoGameEvent* ev) {
    std::memcpy(gs, ev->data.load.state, sizeof(GState));
}

void update_state(GState& gs, GInput inputs[2], int num_players) {
	for (int player = 0; player < num_players; player++) {
		if (inputs[player].input.dir.up) gs.py[player] -= 2;
		if (inputs[player].input.dir.down) gs.py[player] += 2;
		if (inputs[player].input.dir.left) gs.px[player] -= 2;
		if (inputs[player].input.dir.right) gs.px[player] += 2;
	}
}

void render_state(GState& gs, SDL_Renderer* renderer) {
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

void get_key_inputs(GInput inputs[2]) {
	// reset inputs
	inputs[0].input.value = 0;
	inputs[0].input.value = 0;
	// fetch inputs
	auto keys = SDL_GetKeyboardState(NULL);
	// P1
	inputs[0].input.dir.up = keys[SDL_SCANCODE_W];
	inputs[0].input.dir.left = keys[SDL_SCANCODE_A];
	inputs[0].input.dir.down = keys[SDL_SCANCODE_S];
	inputs[0].input.dir.right = keys[SDL_SCANCODE_D];
	// P2
	inputs[1].input.dir.up = keys[SDL_SCANCODE_UP];
	inputs[1].input.dir.left = keys[SDL_SCANCODE_LEFT];
	inputs[1].input.dir.down = keys[SDL_SCANCODE_DOWN];
	inputs[1].input.dir.right = keys[SDL_SCANCODE_RIGHT];
}

int recv_idx = 0;
std::vector<GekkoNetResult*> inbox_session1;
std::vector<GekkoNetResult*> inbox_session2;
std::vector<GekkoNetResult*> outbox;

static void fake_send_data(GekkoNetAddress* addr, const char* data, int length) {
    // gekkonet will clean this up
    auto result = new GekkoNetResult();

    auto addr_to = *(unsigned char*)addr->data;
    auto addr_from = addr_to == 1 ? 2 : 1;
    // gekkonet will clean this up 
    result->addr.data = new unsigned char;
    result->addr.size = sizeof(char);
    std::memcpy(result->addr.data, &addr_from, result->addr.size);

    // user created memory
    // gekkonet will clean this up
    result->data = new char[length];
    std::memcpy(result->data, data, length);

    result->data_len = length;

    if (addr_from == 1) {
        inbox_session2.push_back(result);
    }
    else {
        inbox_session1.push_back(result);
    }
}

static void fake_free_data(void* data_ptr) {
    // delete user created data
    delete data_ptr;
}

static GekkoNetResult** fake_receive_data(int* length) {
    outbox.clear();

    auto& curr_inbox = recv_idx % 2 == 1 ? inbox_session2 : inbox_session1;
    outbox.insert(outbox.begin(), curr_inbox.begin(), curr_inbox.end());

    *length = (int)curr_inbox.size();
    curr_inbox.clear();

    recv_idx++;

    return outbox.data();
}

int main(int argc, char* args[])
{
	running = init_windows();

	GState state1 = {};
	GState state2 = {};

	GInput inputs[2] = {};

    GekkoNetAdapter adapter {
        fake_send_data,
        fake_receive_data,
        fake_free_data
    };


	int num_players = 2;

    GekkoSession* sess1 = nullptr;
    GekkoSession* sess2 = nullptr;

    GekkoConfig conf {};

	conf.num_players = num_players;
	conf.input_size = sizeof(int);
	conf.max_spectators = 1;
	conf.input_prediction_window = 8;
	conf.state_size = sizeof(GState);
	conf.limited_saving = true;
    conf.spectator_delay = 90;

    gekko_create(&sess1);
    gekko_create(&sess2);

    gekko_start(sess1, &conf);
    gekko_start(sess2, &conf);

    gekko_net_adapter_set(sess1, &adapter);
    gekko_net_adapter_set(sess2, &adapter);

	char addrs1 = 1;
    auto addr1 = GekkoNetAddress{ &addrs1, sizeof(char) };

	char addrs2 = 2;
	auto addr2 = GekkoNetAddress{ &addrs2, sizeof(char) };

    int s1p1 = gekko_add_actor(sess1, LocalPlayer, nullptr);
    int s1p2 = gekko_add_actor(sess1, LocalPlayer, nullptr);
    int s1s1 = gekko_add_actor(sess1, Spectator, &addr2);

    int s2p1 = gekko_add_actor(sess2, RemotePlayer, &addr1);

	// timing 
	using time_point = std::chrono::time_point<std::chrono::steady_clock>;
	using frame = std::chrono::duration<unsigned int, std::ratio<1, 60>>;
	using clock = std::chrono::steady_clock;

	time_point timer(clock::now());
	frame fps = {};

	while (running) {
		fps = std::chrono::duration_cast<frame>(clock::now() - timer);

		if (fps.count() > 0) {
			timer = clock::now();

			process_events();

			get_key_inputs(inputs);

			//add local inputs to the session
            gekko_add_local_input(sess1, s1p1, &inputs[0].input.value);
            gekko_add_local_input(sess1, s1p2, &inputs[1].input.value);

			int frame = 0;

            int count = 0;
            auto events = gekko_session_events(sess1, &count);
            for (int i = 0; i < count; i++) {
                printf("S1 EV: %d\n", events[i]->type);
            }

            count = 0;
            auto updates = gekko_update_session(sess1, &count);
            for (int i = 0; i < count; i++)
            {
                auto ev = updates[i];

                switch (ev->type)
                {
                case SaveEvent:
                    printf("S1 Save frame:%d\n", ev->data.save.frame);
                    save_state(&state1, ev);
                    break;
                case LoadEvent:
                    printf("S1 Load frame:%d\n", ev->data.load.frame);
                    load_state(&state1, ev);
                    break;
                case AdvanceEvent:
                    // on advance event, advance the gamestate using the given inputs
                    inputs[0].input.value = ev->data.adv.inputs[0];
                    inputs[1].input.value = ev->data.adv.inputs[4];
                    frame = ev->data.adv.frame;
                    printf("S1, F:%d, P1:%d P2:%d\n", frame, inputs[0].input.value, inputs[1].input.value);
                    // now we can use them to update state.
                    update_state(state1, inputs, num_players);
                    break;
                default:
                    printf("S1 Unknown Event: %d\n", ev->type);
                    break;
                }
            }

            count = 0;
            events = gekko_session_events(sess2, &count);
            for (int i = 0; i < count; i++) {
                printf("S1 EV: %d\n", events[i]->type);
            }

            count = 0;
            updates = gekko_update_session(sess2, &count);
            for (int i = 0; i < count; i++)
            {
                auto ev = updates[i];

                switch (ev->type)
                {
                case SaveEvent:
                    printf("S2 Save frame:%d\n", ev->data.save.frame);
                    save_state(&state2, ev);
                    break;
                case LoadEvent:
                    printf("S2 Load frame:%d\n", ev->data.load.frame);
                    load_state(&state2, ev);
                    break;
                case AdvanceEvent:
                    // on advance event, advance the gamestate using the given inputs
                    inputs[0].input.value = ev->data.adv.inputs[0];
                    inputs[1].input.value = ev->data.adv.inputs[4];
                    frame = ev->data.adv.frame;
                    printf("S2, F:%d, P1:%d P2:%d\n", frame, inputs[0].input.value, inputs[1].input.value);
                    // now we can use them to update state.
                    update_state(state2, inputs, num_players);
                    break;
                default:
                    printf("S2 Unknown Event: %d\n", ev->type);
                    break;
                }
            }
		}

		// draw the state every iteration
		SDL_SetRenderDrawColor(renderer1, 0, 0, 0, 255);
		SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);

		SDL_RenderClear(renderer1);
		SDL_RenderClear(renderer2);

		render_state(state1, renderer1);
		render_state(state2, renderer2);

		SDL_RenderPresent(renderer1);
		SDL_RenderPresent(renderer2);
	}
	del_windows();

    gekko_destroy(sess1);
    gekko_destroy(sess2);

	return 0;
}
