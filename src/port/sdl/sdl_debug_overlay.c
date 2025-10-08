#include "port/sdl/sdl_debug_overlay.h"
#include "port/sdl/sdl_pad.h"

#include <SDL3/SDL.h>

// Colors for input display
#define INPUT_ACTIVE_R 0
#define INPUT_ACTIVE_G 255
#define INPUT_ACTIVE_B 0
#define INPUT_INACTIVE_R 128
#define INPUT_INACTIVE_G 128
#define INPUT_INACTIVE_B 128

static void render_fps(SDL_Renderer* renderer, int x, int y, float fps) {
    SDL_RenderDebugTextFormat(renderer, x, y, "FPS: %.3f", fps);
}

static void render_player_input(SDL_Renderer* renderer, int x, int y, int player) {
    SDLPad_ButtonState pad_state = { 0 };
    SDLPad_GetButtonState(player, &pad_state);

    int x_pos = x;
    int y_pos = y;

    // Player label
    SDL_RenderDebugTextFormat(renderer, x_pos, y_pos, "P%d:", player + 1);
    x_pos += 24;

    // Helper to draw a WASD shape
    #define DRAW_DPAD(base_x, base_y) do { \
        int cx = base_x + 6; \
        int cy = base_y + 6; \
        int size = 3; \
        /* Up */ \
        SDL_SetRenderDrawColor(renderer, pad_state.dpad_up ? INPUT_ACTIVE_R : INPUT_INACTIVE_R, pad_state.dpad_up ? INPUT_ACTIVE_G : INPUT_INACTIVE_G, pad_state.dpad_up ? INPUT_ACTIVE_B : INPUT_INACTIVE_B, SDL_ALPHA_OPAQUE); \
        SDL_FRect up = {cx - size/2.0f, cy - size*2, size, size}; \
        SDL_RenderFillRect(renderer, &up); \
        /* Down */ \
        SDL_SetRenderDrawColor(renderer, pad_state.dpad_down ? INPUT_ACTIVE_R : INPUT_INACTIVE_R, pad_state.dpad_down ? INPUT_ACTIVE_G : INPUT_INACTIVE_G, pad_state.dpad_down ? INPUT_ACTIVE_B : INPUT_INACTIVE_B, SDL_ALPHA_OPAQUE); \
        SDL_FRect down = {cx - size/2.0f, cy - 1, size, size}; \
        SDL_RenderFillRect(renderer, &down); \
        /* Left */ \
        SDL_SetRenderDrawColor(renderer, pad_state.dpad_left ? INPUT_ACTIVE_R : INPUT_INACTIVE_R, pad_state.dpad_left ? INPUT_ACTIVE_G : INPUT_INACTIVE_G, pad_state.dpad_left ? INPUT_ACTIVE_B : INPUT_INACTIVE_B, SDL_ALPHA_OPAQUE); \
        SDL_FRect left = {cx - size*2, cy - 1, size, size}; \
        SDL_RenderFillRect(renderer, &left); \
        /* Right */ \
        SDL_SetRenderDrawColor(renderer, pad_state.dpad_right ? INPUT_ACTIVE_R : INPUT_INACTIVE_R, pad_state.dpad_right ? INPUT_ACTIVE_G : INPUT_INACTIVE_G, pad_state.dpad_right ? INPUT_ACTIVE_B : INPUT_INACTIVE_B, SDL_ALPHA_OPAQUE); \
        SDL_FRect right = {cx + size, cy - 1, size, size}; \
        SDL_RenderFillRect(renderer, &right); \
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); \
    } while(0)

    // Helper to draw a diamond (4 face buttons)
    #define DRAW_DIAMOND(base_x, base_y) do { \
        int cx = base_x + 6; \
        int cy = base_y + 6; \
        int size = 3; \
        /* North (Triangle) */ \
        SDL_SetRenderDrawColor(renderer, pad_state.north ? INPUT_ACTIVE_R : INPUT_INACTIVE_R, pad_state.north ? INPUT_ACTIVE_G : INPUT_INACTIVE_G, pad_state.north ? INPUT_ACTIVE_B : INPUT_INACTIVE_B, SDL_ALPHA_OPAQUE); \
        SDL_FRect north = {cx - size/2.0f, cy - size*2, size, size}; \
        SDL_RenderFillRect(renderer, &north); \
        /* South (Cross) */ \
        SDL_SetRenderDrawColor(renderer, pad_state.south ? INPUT_ACTIVE_R : INPUT_INACTIVE_R, pad_state.south ? INPUT_ACTIVE_G : INPUT_INACTIVE_G, pad_state.south ? INPUT_ACTIVE_B : INPUT_INACTIVE_B, SDL_ALPHA_OPAQUE); \
        SDL_FRect south = {cx - size/2.0f, cy + size, size, size}; \
        SDL_RenderFillRect(renderer, &south); \
        /* West (Square) */ \
        SDL_SetRenderDrawColor(renderer, pad_state.west ? INPUT_ACTIVE_R : INPUT_INACTIVE_R, pad_state.west ? INPUT_ACTIVE_G : INPUT_INACTIVE_G, pad_state.west ? INPUT_ACTIVE_B : INPUT_INACTIVE_B, SDL_ALPHA_OPAQUE); \
        SDL_FRect west = {cx - size*2, cy - size/2.0f, size, size}; \
        SDL_RenderFillRect(renderer, &west); \
        /* East (Circle) */ \
        SDL_SetRenderDrawColor(renderer, pad_state.east ? INPUT_ACTIVE_R : INPUT_INACTIVE_R, pad_state.east ? INPUT_ACTIVE_G : INPUT_INACTIVE_G, pad_state.east ? INPUT_ACTIVE_B : INPUT_INACTIVE_B, SDL_ALPHA_OPAQUE); \
        SDL_FRect east = {cx + size, cy - size/2.0f, size, size}; \
        SDL_RenderFillRect(renderer, &east); \
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); \
    } while(0)

    // Helper to draw labeled button boxes
    #define DRAW_LABELED_BUTTON(base_x, base_y, label, is_pressed) do { \
        SDL_SetRenderDrawColor(renderer, is_pressed ? INPUT_ACTIVE_R : INPUT_INACTIVE_R, is_pressed ? INPUT_ACTIVE_G : INPUT_INACTIVE_G, is_pressed ? INPUT_ACTIVE_B : INPUT_INACTIVE_B, SDL_ALPHA_OPAQUE); \
        SDL_FRect rect = {base_x, base_y, 8, 4}; \
        SDL_RenderFillRect(renderer, &rect); \
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); \
        SDL_RenderDebugText(renderer, base_x + 11, base_y, label); \
    } while(0)

    // D-Pad
    DRAW_DPAD(x_pos, y_pos);
    x_pos += 20;

    // Face buttons
    DRAW_DIAMOND(x_pos, y_pos);
    x_pos += 20;

    // Shoulders, triggers, and joystick clicks
    DRAW_LABELED_BUTTON(x_pos, y_pos, "L1", pad_state.left_shoulder);
    DRAW_LABELED_BUTTON(x_pos, y_pos + 10, "L2", pad_state.left_trigger > 16000);
    DRAW_LABELED_BUTTON(x_pos, y_pos + 20, "L3", pad_state.l3);
    x_pos += 32;
    DRAW_LABELED_BUTTON(x_pos, y_pos, "R1", pad_state.right_shoulder);
    DRAW_LABELED_BUTTON(x_pos, y_pos + 10, "R2", pad_state.right_trigger > 16000);
    DRAW_LABELED_BUTTON(x_pos, y_pos + 20, "R3", pad_state.r3);
    x_pos += 32;

    // Start/Select
    DRAW_LABELED_BUTTON(x_pos, y_pos, "Select", pad_state.back);
    DRAW_LABELED_BUTTON(x_pos, y_pos + 10, "Start", pad_state.start);

    #undef DRAW_DPAD
    #undef DRAW_DIAMOND
    #undef DRAW_LABELED_BUTTON
}

void SDLDebugOverlay_Render(SDL_Renderer* renderer, float fps, bool show_fps, bool show_inputs) {
    if (!show_fps && !show_inputs) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_SetRenderScale(renderer, 2, 2);

    int y_pos = 8;
    int start_y = y_pos;
    int spacing = 4;
    int total_height = 0;

    // Calculate total height needed for debug display
    if (show_fps) {
        total_height += 16;
    }
    if (show_inputs) {
        if (show_fps) {
            total_height += spacing; // spacing between FPS and inputs
        }
        total_height += 28 + spacing + 28;
    }

    // Draw background for entire debug area
    if (total_height > 0) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 192);
        SDL_FRect bg_rect = { 4, start_y - 2, 193, total_height + 4 };
        SDL_RenderFillRect(renderer, &bg_rect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    }

    // Render all elements
    if (show_fps) {
        render_fps(renderer, 8, y_pos, fps);
        y_pos += 16 + spacing;
    }

    if (show_inputs) {
        render_player_input(renderer, 8, y_pos, 0);
        y_pos += 28 + spacing;
        render_player_input(renderer, 8, y_pos, 1);
    }

    SDL_SetRenderScale(renderer, 1, 1);
}
