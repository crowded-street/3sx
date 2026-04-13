#ifndef PORT_IO_AFS_H
#define PORT_IO_AFS_H

#include <stdbool.h>
#include <stddef.h>

typedef enum AFSReadState {
    AFS_READ_STATE_IDLE,
    AFS_READ_STATE_READING,
    AFS_READ_STATE_FINISHED,
    AFS_READ_STATE_ERROR,
} AFSReadState;

typedef int AFSHandle;

#define AFS_NONE -1

/// Initialize AFS subsystem.
/// @param file_path Path to AFS file.
/// @param read_chunk_size Maximum size of data read per call to `AFS_RunServer`.
/// @return `true` on success, `false` on failure.
bool AFS_Init(const char* file_path, size_t read_chunk_size);

void AFS_Finish();
size_t AFS_GetFileCount();
size_t AFS_GetSize(size_t file_num);

void AFS_RunServer();
AFSHandle AFS_Open(size_t file_num);
void AFS_Read(AFSHandle handle, void* buf);
void AFS_ReadSync(AFSHandle handle, void* buf);
void AFS_Stop(AFSHandle handle);
void AFS_Close(AFSHandle handle);
AFSReadState AFS_GetState(AFSHandle handle);

#endif
