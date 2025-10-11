#ifndef PORT_IO_AFS_H
#define PORT_IO_AFS_H

#include <stdbool.h>

bool AFS_Init(const char* file_path);
void AFS_Finish();

#endif
