#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

#define UNIT_OF_TIMER_MAX 50
#define HUD_SHIFT 64

#if CPS3
#define NUM_CHARS 21

typedef enum Character {
    CHAR_GILL = 0,
    CHAR_ALEX = 1,
    CHAR_RYU = 2,
    CHAR_YUN = 3,
    CHAR_DUDLEY = 4,
    CHAR_NECRO = 5,
    CHAR_HUGO = 6,
    CHAR_IBUKI = 7,
    CHAR_ELENA = 8,
    CHAR_ORO = 9,
    CHAR_YANG = 10,
    CHAR_KEN = 11,
    CHAR_SEAN = 12,
    CHAR_URIEN = 13,
    CHAR_AKUMA = 14,
    CHAR_SHIN_AKUMA = 15,
    CHAR_CHUNLI = 16,
    CHAR_MAKOTO = 17,
    CHAR_Q = 18,
    CHAR_TWELVE = 19,
    CHAR_REMY = 20,
} Character;
#else
#define NUM_CHARS 20

typedef enum Character : uint16_t {
    CHAR_GILL = 0,
    CHAR_ALEX = 1,
    CHAR_RYU = 2,
    CHAR_YUN = 3,
    CHAR_DUDLEY = 4,
    CHAR_NECRO = 5,
    CHAR_HUGO = 6,
    CHAR_IBUKI = 7,
    CHAR_ELENA = 8,
    CHAR_ORO = 9,
    CHAR_YANG = 10,
    CHAR_KEN = 11,
    CHAR_SEAN = 12,
    CHAR_URIEN = 13,
    CHAR_AKUMA = 14,
    CHAR_CHUNLI = 15,
    CHAR_MAKOTO = 16,
    CHAR_Q = 17,
    CHAR_TWELVE = 18,
    CHAR_REMY = 19,
} Character;
#endif

typedef enum JumpDir : uint8_t {
    JUMP_DIR_NEUTRAL = 0,
    JUMP_DIR_FORWARD = 1,
    JUMP_DIR_BACKWARD = 2,
} JumpDir;

#endif
