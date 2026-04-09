#ifndef ZLIBAPP_H
#define ZLIBAPP_H

#include "types.h"

void zlib_Initialize(void* tempAdrs, s32 tempSize);
ssize_t zlib_Decompress(void* srcBuff, s32 srcSize, void* dstBuff, s32 dstSize);

#endif
