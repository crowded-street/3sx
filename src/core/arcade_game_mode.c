#include "core/arcade_game_mode.h"

static bool is_enabled = false;

bool ArcadeGameMode_IsEnabled() {
    return is_enabled;
}

void ArcadeGameMode_Toggle() {
    is_enabled = !is_enabled;
}
