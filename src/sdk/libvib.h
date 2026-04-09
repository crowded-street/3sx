#ifndef _LIBVIB_H_
#define _LIBVIB_H_

#define SCE_VIB_PROFILE_SIZE (1)
#define SCE_VIB_DATA_SIZE (2)

int sceVibGetProfile(int, unsigned char*);
int sceVibSetActParam(int, int, unsigned char*, int, unsigned char*);
void* sceVibGetErxEntries(void);

#endif
