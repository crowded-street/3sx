#include "arcade/arcade_balance.h"
#include "port/config/config.h"

#if ARCADE_ROM
#include "arcade/arcade_char_data.h"
#endif

static bool is_enabled = false;

void ArcadeBalance_Init() {
#if ARCADE_ROM
    is_enabled = Config_GetBool(CFG_ARCADE_BALANCE);

    if (is_enabled) {
        ArcadeCharData_Init();
    }
#endif
}

bool ArcadeBalance_IsEnabled() {
    return is_enabled;
}
