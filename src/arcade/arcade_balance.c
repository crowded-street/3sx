#include "arcade/arcade_balance.h"
#include "arcade/arcade_char_data.h"
#include "port/config/config.h"

static bool is_enabled = false;

void ArcadeBalance_Init() {
    is_enabled = Config_GetBool(CFG_ARCADE_BALANCE);

    if (is_enabled) {
        ArcadeCharData_Init();
    }
}

bool ArcadeBalance_IsEnabled() {
    return is_enabled;
}
