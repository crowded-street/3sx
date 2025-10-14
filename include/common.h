#ifndef COMMON_H
#define COMMON_H

#include "types.h"

#define REINTERPRET_AS_U32(_num) (*(u32*)&(_num))
#define BOOL(_expr) ((_expr) ? 1 : 0)
#define SPR 0x70000000

#define S32_MAX 0x7FFFFFFF
#define S32_INCREMENT_WRAPAROUND(val) (((val) != S32_MAX) ? ((val) + 1) : 0)

#define ALIGN_UP(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

// Compilation with GNU C Compilier means we are compiling CRI
#if defined(__GNUC__) && !defined(M2CTX) && !defined(TARGET_SDL3)
#define INCLUDE_ASM(FOLDER, NAME)                                                                                      \
    __asm__("\t.text\n"                                                                                                \
            "\t.align\t3\n"                                                                                            \
            "\t.globl\t" #NAME "\n"                                                                                    \
            "\t.ent\t" #NAME "\n" #NAME ":\n"                                                                          \
            ".include \"" FOLDER "/" #NAME ".s\"\n"                                                                    \
            "\t.set reorder\n"                                                                                         \
            "\t.set at\n"                                                                                              \
            "\t.end\t" #NAME "\n")

#define INCLUDE_RODATA(FOLDER, NAME) __asm__("    .include \"" FOLDER "/" #NAME ".s\"\n")

__asm__(".include \"include/cri_macro.inc\"\n");
#else
#define INCLUDE_ASM(FOLDER, NAME)
#define INCLUDE_RODATA(FOLDER, NAME)
#endif

#if defined(M2CTX)

#define ATTR_ALIGNED(value)

#else

#define ATTR_ALIGNED(value) __attribute__((aligned(value)))

#endif

#ifndef __dead2
#define __dead2 __attribute__((__noreturn__))
#endif

__dead2 void fatal_error(const s8* fmt, ...);
__dead2 void not_implemented(const s8* func);
void debug_print(const char* fmt, ...);

#define LO_16_BITS(val) (val & 0xFFFF)
#define HI_16_BITS(val) ((val & 0xFFFF0000) >> 16)

#endif
