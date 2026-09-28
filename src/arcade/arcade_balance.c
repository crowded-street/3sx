#include "arcade/arcade_balance.h"
#include "port/config/config.h"

static bool is_enabled = false;

void ArcadeBalance_Init() {
    is_enabled = Config_GetBool(CFG_ARCADE_BALANCE);
}

bool ArcadeBalance_IsEnabled() {
    return is_enabled;
}
