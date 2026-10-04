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

#define CHAR_3SX_TO_ARCADE(c) ((c) > CHAR_AKUMA ? (c) + 1 : (c))
#define CHAR_ARCADE_TO_3SX(c) ((c) > CHAR_AKUMA ? (c) - 1 : (c))

/// Overarching stage
typedef enum Stage {
    STAGE_3S_GILL = 0,
    STAGE_3S_ALEX = 1,
    STAGE_3S_RYU = 2,
    STAGE_3S_YUN = 3,
    STAGE_3S_DUDLEY = 4,
    STAGE_3S_NECRO = 5,
    STAGE_3S_HUGO = 6,
    STAGE_3S_IBUKI = 7,
    STAGE_3S_ELENA = 8,
    STAGE_3S_ORO = 9,
    STAGE_3S_YANG = 10,
    STAGE_3S_KEN = 11,
    STAGE_3S_SEAN = 12,
    STAGE_3S_URIEN = 13,
    STAGE_3S_AKUMA = 14,
    STAGE_3S_SHIN_AKUMA = 15,
    STAGE_3S_CHUNLI = 16,
    STAGE_3S_MAKOTO = 17,
    STAGE_3S_Q = 18,
    STAGE_3S_TWELVE = 19,
    STAGE_3S_REMY = 20,
    STAGE_3S_BONUS_CAR = 21,
    STAGE_3S_BONUS_BALLS = 22,
    STAGE_NG_GILL = 23,
    STAGE_NG_ALEX = 24,
    STAGE_COUNT,
} Stage;

/// Concrete background. A stage is made up of one or more areas (e.g. one per round).
/// 3S uses the same area for all rounds; Q's stage reuses Dudley's area.
typedef enum Area {
    AREA_3S_GILL = 0,
    AREA_3S_ALEX = 1,
    AREA_3S_RYU = 2,
    AREA_3S_YUN = 3,
    AREA_3S_DUDLEY = 4,
    AREA_3S_NECRO = 5,
    AREA_3S_HUGO = 6,
    AREA_3S_IBUKI = 7,
    AREA_3S_ELENA = 8,
    AREA_3S_ORO = 9,
    AREA_3S_YANG = 10,
    AREA_3S_KEN = 11,
    AREA_3S_SEAN = 12,
    AREA_3S_URIEN = 13,
    AREA_3S_AKUMA = 14,
    AREA_3S_SHIN_AKUMA = 15,
    AREA_3S_CHUNLI = 16,
    AREA_3S_MAKOTO = 17,
    AREA_3S_Q = 18,
    AREA_3S_TWELVE = 19,
    AREA_3S_REMY = 20,
    AREA_3S_BONUS_CAR = 21,
    AREA_3S_BONUS_BALLS = 22,
    AREA_NG_GILL = 23,
    AREA_NG_ALEX = 24,
    AREA_COUNT,
} Area;

/// A character's home stage has the same number as the character in the arcade version
#define STAGE_OF_CHAR(c) ((Stage)CHAR_3SX_TO_ARCADE(c))

typedef enum JumpDir : uint8_t {
    JUMP_DIR_NEUTRAL = 0,
    JUMP_DIR_FORWARD = 1,
    JUMP_DIR_BACKWARD = 2,
} JumpDir;

#endif
