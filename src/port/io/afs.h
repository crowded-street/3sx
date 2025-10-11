#ifndef PORT_IO_AFS_H
#define PORT_IO_AFS_H

#include <stdbool.h>

typedef enum AFSReadState {
    AFS_READ_STATE_IDLE,
    AFS_READ_STATE_READING,
    AFS_READ_STATE_FINISHED,
    AFS_READ_STATE_ERROR
} AFSReadState;

typedef int AFSHandle;

#define AFS_NONE -1

bool AFS_Init(const char* file_path);
void AFS_Finish();
unsigned int AFS_GetFileCount();
unsigned int AFS_GetSize(int file_num);

void AFS_RunServer();
AFSHandle AFS_Open(int file_num);
void AFS_Read(AFSHandle handle, int sectors, void* buf);
void AFS_Stop(AFSHandle handle);
void AFS_Close(AFSHandle handle);
AFSReadState AFS_GetState(AFSHandle handle);
unsigned int AFS_GetSectorCount(AFSHandle handle);

#endif
