#include "SDL2/SDL.h"

#define GEKKONET_STATIC

#include "gekkonet.h"

#include <chrono>
#include <iostream>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool running = false;

bool init_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}
	window = SDL_CreateWindow(
		"GekkoNet Example: Local Session",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		400,
		400,
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

void process_events(GekkoSession* sess) {
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
		case SDL_KEYUP:
			auto key = event.key.keysym.sym;
			if (key == SDLK_q) {
                gekko_set_local_delay(sess, 1, 30);
				printf("delay set to 30!\n");
				break;
			}
			if (key == SDLK_e) {
				gekko_set_local_delay(sess, 1, 1);
				printf("delay set to 1!\n");
				break;
			}
			break;
		}
	}
}

struct GState {
	int px[2];
	int py[2];
};

void update_state(GState& gs, GInput inputs[2], int num_players) {
	for (int player = 0; player < num_players; player++) {
		if (inputs[player].input.dir.up) gs.py[player] -= 2;
		if (inputs[player].input.dir.down) gs.py[player] += 2;
		if (inputs[player].input.dir.left) gs.px[player] -= 2;
		if (inputs[player].input.dir.right)gs.px[player] += 2;
	}
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

int main(int argc, char* args[])
{
	running = init_window();

	GState state = {};
	GInput inputs[2] = {};

	int num_players = 2;

    GekkoSession* sess = nullptr;
    GekkoConfig conf {};

	conf.num_players = num_players;
	conf.input_size = sizeof(char);
	conf.max_spectators = 0;
	conf.input_prediction_window = 0;

    gekko_create(&sess);
    gekko_start(sess, &conf);

    int p1 = gekko_add_actor(sess, LocalPlayer, nullptr);
    int p2 = gekko_add_actor(sess, LocalPlayer, nullptr);

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

			process_events(sess);
			get_key_inputs(inputs);

			//add local inputs to the session
            gekko_add_local_input(sess, p1, &inputs[0].input.value);
            gekko_add_local_input(sess, p2, &inputs[1].input.value);

            int count = 0;
            auto events = gekko_session_events(sess, &count);
            for (int i = 0; i < count; i++) {
                printf("s ev: %d\n", events[i]->type);
            }

            count = 0;
            auto updates = gekko_update_session(sess, &count);
			for (int i = 0; i < count; i++)
			{
                auto ev = updates[i];
                printf("g ev: %d\n", ev->type);
				switch (ev->type)
				{
				case AdvanceEvent:
					// on advance event, advance the gamestate using the given inputs
					inputs[0].input.value = ev->data.adv.inputs[0];
					inputs[1].input.value = ev->data.adv.inputs[1];
					// now we can use them to update state.
					update_state(state, inputs, num_players);
					break;
				default:
					break;
				}
			}
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
