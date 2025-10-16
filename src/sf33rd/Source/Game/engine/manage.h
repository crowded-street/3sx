#ifndef MANAGE_H
#define MANAGE_H

#include "types.h"

extern u8 Disp_Bonus_Contents;

s32 Game_Management();
s32 Wait_Seek_Time();
void request_center_message(s16 Kind_of_Message);
void Control_Music_Fade(s16 Time);
void complete_victory_pause();

#endif
