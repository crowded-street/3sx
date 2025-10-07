#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2asm.h"

#include <string.h>

void flPS2_Mem_move(const void* src, void* dst, size_t len) {
    not_implemented(__func__);
}

void flPS2_Mem_move16(const void* src, void* dst, size_t len) {
    not_implemented(__func__);
}

void flPS2_Mem_move16_8A(const void* src, void* dst, size_t len) {
    not_implemented(__func__);
}

void flPS2_Mem_move16_16A(const void* src, void* dst, size_t len) {
    memcpy(dst, src, len * 16);
}

void flPS2_Mem_move32(const void* src, void* dst, size_t len) {
    not_implemented(__func__);
}

void flPS2_Mem_move32_8A(const void* src, void* dst, size_t len) {
    not_implemented(__func__);
}

void flPS2_Mem_move32_16A(const void* src, void* dst, size_t len) {
    not_implemented(__func__);
}

void flPS2_Mem_move64(const void* src, void* dst, size_t len) {
    memmove(dst, src, len * 64);
}

void flPS2_Clear_Mem(void* str, size_t len) {
    not_implemented(__func__);
}

void flPS2_Clear_Mem16_16A(void* str, size_t len) {
    memset(str, 0, len * 16);
}

void flPS2_Fill_Mem(void* str, s32 c, size_t len) {
    not_implemented(__func__);
}

void flPS2_Fill_Mem16_16A(void* str, s64 c, size_t len) {
    not_implemented(__func__);
}
