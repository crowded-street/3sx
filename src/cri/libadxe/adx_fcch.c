#include "cri/private/libadxe/adx_fcch.h"
#include "common.h"
#include <eekernel.h>

void ADXF_Ocbi(Sint8* buf, Sint32 bsize) {
    InvalidDCache(buf, buf + bsize - 1);
}
