#ifndef TEXGROUP_H
#define TEXGROUP_H

#include "structs.h"
#include "types.h"

extern TEX_GRP_LD texgrplds[100];
extern const TexGroupData texgrpdat[100];

void q_ldreq_texture_group(REQ* curr);
void Init_texgrplds_work();
void checkSelObjFileLoaded();
s32 load_any_texture_patnum(u16 patnum, u8 kokey, u8 _unused);
void purge_texture_group_of_this(u16 patnum);
void purge_texture_group(u8 grp);
void purge_player_texture(s16 id);
void reservMemKeySelObj();

#endif
