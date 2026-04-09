#ifndef PLCOMMON_H
#define PLCOMMON_H

#include "types.h"

typedef struct {
    s32 rl; 
    s32 rs; 
    s32 rm; 
    s32 gl; 
    s32 gs; 
    s32 gm; 
    s32 bl; 
    s32 bs; 
    s32 bm; 
    s32 al; 
    s32 as; 
    s32 am; 
} PixelFormat;

typedef struct {
    u32 desc;                
    s32 width;               
    s32 height;              
    s32 pitch;               
    void* ptr;               
    s32 bitdepth;            
    PixelFormat pixelformat; 
} plContext;

typedef struct {
    u32 flag;          
    u32 desc;          
    u32 size;          
    u32 bitdepth;      
    s16 width;         
    s16 height;        
    u32 format;        
    u32 mem_handle;    
    uintptr_t lock_ptr;
    u32 tex_num;       
    s8 be_flag;        
    u8 lock_flag;      
    void* wkVram;      
} FLTexture;

#endif
