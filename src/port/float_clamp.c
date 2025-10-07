#include "port/float_clamp.h"

#include <float.h> // For FLT_MAX
#include <math.h>  // For isnan()
#include <stdint.h>
#include <stdio.h>

uint64_t float_to_uint64_clamped(float f) {
    if (isnan(f) || f <= 0.0f)
        return 0;
    else if (f >= (float)UINT64_MAX)
        return UINT64_MAX;
    else
        return (uint64_t)f;
}
