#if CHECKSUM

#include "sha256.h"

#include <psa/crypto.h>

static void digest_to_hex(const uint8_t* digest, char* dst_hex65) {
    static const char hex_chars[] = "0123456789abcdef";

    for (size_t i = 0; i < SHA256_BYTES_SIZE; ++i) {
        dst_hex65[i * 2] = hex_chars[(digest[i] >> 4) & 0x0F];
        dst_hex65[i * 2 + 1] = hex_chars[digest[i] & 0x0F];
    }

    dst_hex65[SHA256_BYTES_SIZE * 2] = '\0';
}

bool sha256_init(sha256* sha) {
    const psa_status_t init_status = psa_crypto_init();

    if (init_status != PSA_SUCCESS) {
        return false;
    }

    sha->operation = (psa_hash_operation_t)PSA_HASH_OPERATION_INIT;
    sha->active = false;
    sha->finished = false;

    const psa_status_t status = psa_hash_setup(&sha->operation, PSA_ALG_SHA_256);

    if (status != PSA_SUCCESS) {
        return false;
    }

    sha->active = true;
    return true;
}

bool sha256_append(sha256* sha, const void* src, size_t n_bytes) {
    if (!sha->active || sha->finished) {
        return false;
    }

    if (n_bytes == 0) {
        return true;
    }

    const psa_status_t status = psa_hash_update(&sha->operation, src, n_bytes);
    return status == PSA_SUCCESS;
}

static bool sha256_finalize_internal(sha256* sha, uint8_t digest[SHA256_BYTES_SIZE]) {
    if (!sha->active || sha->finished) {
        return false;
    }

    size_t digest_length = 0;
    const psa_status_t status = psa_hash_finish(&sha->operation, digest, SHA256_BYTES_SIZE, &digest_length);

    if (status != PSA_SUCCESS || digest_length != SHA256_BYTES_SIZE) {
        psa_hash_abort(&sha->operation);
        sha->active = false;
        sha->finished = true;
        return false;
    }

    sha->active = false;
    sha->finished = true;
    return true;
}

bool sha256_finalize_hex(sha256* sha, char* dst_hex65) {
    uint8_t digest[SHA256_BYTES_SIZE];

    if (!sha256_finalize_internal(sha, digest)) {
        return false;
    }

    digest_to_hex(digest, dst_hex65);
    return true;
}

bool sha256_finalize_bytes(sha256* sha, void* dst_bytes32) {
    return sha256_finalize_internal(sha, dst_bytes32);
}

#endif
