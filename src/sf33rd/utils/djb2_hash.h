#ifndef DJB2_HASH_H
#define DJB2_HASH_H

#include <stdint.h>
#include <stdlib.h>

static inline uint32_t djb2_init() {
    return 5381;
}

static inline uint32_t djb2_update_mem(uint32_t hash, const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        hash *= 33;
        hash += data[i];
    }

    return hash;
}

#define djb2_update(hash, v) djb2_update_mem(hash, &v, sizeof(v))
#define djb2_updatep(hash, p) djb2_update_mem(hash, p, sizeof(*p))
#define djb2_updatea(hash, a) djb2_update_mem(hash, a, sizeof(a))

#endif
