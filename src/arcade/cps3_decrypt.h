#ifndef CPS3_DECRYPT_H
#define CPS3_DECRYPT_H

#include <SDL3/SDL.h>

/* The four SIMM bytes of one word, in the order cps3_decrypt's parameter list
 * had them. */
typedef struct {
    Uint8 b0;
    Uint8 b1;
    Uint8 b2;
    Uint8 b3;
} Cps3Word;

Uint32 cps3_decrypt(const Cps3Word* w, Uint32 i);

#endif
