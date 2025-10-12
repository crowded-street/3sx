#ifndef VM_SUB_H
#define VM_SUB_H

#include "types.h"

u8 VM_Access_Request(u8 Request, u8 Drive);
void Setup_File_Property(s16 file_type, u8 number);

#endif
