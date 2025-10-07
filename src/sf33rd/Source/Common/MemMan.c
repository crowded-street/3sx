#include "sf33rd/Source/Common/MemMan.h"
#include "common.h"

u32 mmInitialNumber;

void mmSystemInitialize() {
    mmInitialNumber = 0;
}

void mmHeapInitialize(_MEMMAN_OBJ* mmobj, u8* adrs, s32 size, s32 unit, s8* format) {
    mmobj->oriHead = adrs;
    mmobj->oriSize = size;
    mmobj->ownUnit = unit;
    mmobj->ownNumber = mmInitialNumber++;
    mmobj->memHead = (u8*)mmRoundUp(mmobj->ownUnit, (uintptr_t)adrs);
    mmobj->memSize = mmRoundOff(mmobj->ownUnit, (uintptr_t)adrs + size) - (uintptr_t)mmobj->memHead;
    mmobj->remainder = mmobj->memSize - (mmobj->ownUnit * 2);
    mmobj->remainderMin = mmobj->remainder;
    mmobj->cell_1st = (struct _MEMMAN_CELL*)mmobj->memHead;
    mmobj->cell_fin = (struct _MEMMAN_CELL*)((uintptr_t)&mmobj->memHead[mmobj->memSize] - mmobj->ownUnit);
    mmobj->cell_1st->prev = NULL;
    mmobj->cell_1st->next = mmobj->cell_fin;
    mmobj->cell_1st->size = mmobj->ownUnit;
    mmobj->cell_fin->prev = mmobj->cell_1st;
    mmobj->cell_fin->next = NULL;
    mmobj->cell_fin->size = mmobj->ownUnit;
}

uintptr_t mmRoundUp(s32 unit, uintptr_t num) {
    return ~(unit - 1) & (num + unit - 1);
}

uintptr_t mmRoundOff(s32 unit, uintptr_t num) {
    return num & ~(unit - 1);
}

void mmDebWriteTag(s8* /* unused */) {
    // Do nothing
}

ssize_t mmGetRemainder(_MEMMAN_OBJ* mmobj) {
    return mmobj->remainder;
}

ssize_t mmGetRemainderMin(_MEMMAN_OBJ* mmobj) {
    return mmobj->remainderMin;
}

u8* mmAlloc(_MEMMAN_OBJ* mmobj, ssize_t size, s32 flag) {
    struct _MEMMAN_CELL* cell = mmAllocSub(mmobj, size, flag);

    if (cell == NULL) {
        return NULL;
    }

    mmobj->remainder -= cell->size;

    if (mmobj->remainderMin > mmobj->remainder) {
        mmobj->remainderMin = mmobj->remainder;
    }

    return (u8*)cell + mmobj->ownUnit;
}

struct _MEMMAN_CELL* mmAllocSub(_MEMMAN_OBJ* mmobj, ssize_t size, s32 flag) {
    struct _MEMMAN_CELL* myself;
    struct _MEMMAN_CELL* next;
    struct _MEMMAN_CELL* cell;
    ssize_t sizeTrue;
    ptrdiff_t gap;
    ptrdiff_t gapMin;

    sizeTrue = mmobj->ownUnit + mmRoundUp(mmobj->ownUnit, size);
    gapMin = 0x7FFFFFFF;
    cell = NULL;

    if (flag != 1) {
        myself = mmobj->cell_1st;

        do {
            next = myself->next;
            gap = (intptr_t)next - (intptr_t)myself - myself->size;

            if (gap >= sizeTrue) {
                if (gap == sizeTrue) {
                    cell = myself;
                    break;
                } else {
                    if ((gap - sizeTrue) < gapMin) {
                        gapMin = gap - sizeTrue;
                        cell = myself;
                    }
                }
            }

            myself = next;
        } while (myself->next != NULL);

        if (cell == NULL) {
            return NULL;
        }

        myself = (struct _MEMMAN_CELL*)((uintptr_t)cell + cell->size);
        myself->prev = cell;
        myself->next = cell->next;
        myself->size = sizeTrue;
        cell->next->prev = myself;
        cell->next = myself;
        return myself;
    }

    myself = mmobj->cell_fin;

    do {
        next = myself->prev;
        gap = (intptr_t)myself - (intptr_t)next - next->size;

        if (gap >= sizeTrue) {
            if (gap == sizeTrue) {
                cell = myself;
                break;
            } else {
                if ((gap - sizeTrue) < gapMin) {
                    gapMin = gap - sizeTrue;
                    cell = myself;
                }
            }
        }

        myself = next;
    } while (myself->prev != NULL);

    if (cell == NULL) {
        return NULL;
    }

    myself = (struct _MEMMAN_CELL*)((uintptr_t)cell - sizeTrue);
    myself->prev = cell->prev;
    myself->next = cell;
    myself->size = sizeTrue;
    cell->prev->next = myself;
    cell->prev = myself;
    return myself;
}

void mmFree(_MEMMAN_OBJ* mmobj, u8* adrs) {
    struct _MEMMAN_CELL* cell;

    if (adrs != NULL) {
        cell = (struct _MEMMAN_CELL*)((intptr_t)adrs - mmobj->ownUnit);
        mmobj->remainder += cell->size;
        cell->prev->next = cell->next;
        cell->next->prev = cell->prev;
    } else {
        return;
    }
}
