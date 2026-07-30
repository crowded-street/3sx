#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "common.h"
#include "port/utils.h"
#include "sf33rd/AcrSDK/common/memfound.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/flps2shader.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "structs.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void flPS2SystemError(s32 error_level, const char* format, ...) {
    va_list args;
    s8 str[512];

    flFlip(0);
    va_start(args, format);
    vsnprintf(str, sizeof(str), format, args);
    va_end(args);
    fatal_error("flps2debug: %s", str);
}
