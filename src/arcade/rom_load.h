#ifndef ROM_LOAD_H
#define ROM_LOAD_H

/// Loads decrypted arcade rom into memory.
/// @param path Path to the .zip file with the rom.
/// @return Pointer to memory containing decrypted rom bytes on success, `NULL` on failure.
void* Rom_Load(const char* path);

#endif
