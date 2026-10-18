#ifndef BG370_H
#define BG370_H

#include "types.h"

#include <stdbool.h>

void BG370();
s16 bg370_line_scroll(s16 bgnm, s16 line);

bool bg370_is_on_bridge();

#endif
