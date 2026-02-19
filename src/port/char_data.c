#include "port/char_data.h"

void CharData_ApplyFixups(CharInitData* data, int character_id) {
    switch (character_id) {
    case 14: // Akuma/Gouki
        // Remove throw box from overhead chop
        for (int i = 0x5A; i <= 0x5D; i++) {
            data->hiit[i].cuix = 0;
        }

        break;
    }
}
