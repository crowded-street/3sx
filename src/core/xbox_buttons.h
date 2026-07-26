#ifndef XBOX_BUTTONS_H
#define XBOX_BUTTONS_H

#include "core/render_primitives.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

#include <stdbool.h>

bool XboxButtons_Init();
void XboxButtons_SetTextureParams(Sprite* sprite, ButtonIcon icon);

#endif
