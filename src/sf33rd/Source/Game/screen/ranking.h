#ifndef RANKING_H
#define RANKING_H

#include "structs.h"
#include "types.h"

extern RANK_DATA Present_Data[2];
extern RANK_DATA Ranking_Data[20];

s32 Ranking();
void Ranking_01();
void Ranking_00();
void Ranking_00_1st();
void Ranking_00_2nd();
void Ranking_00_3rd();
void Ranking_00_4th();
void Ranking_00_5th();
void Ranking_00_6th();
void Ranking_01_1st();
void Ranking_01_2nd();
void Ranking_01_4th();
void Ranking_01_5th();
void Ranking_Sub();
void Setup_grade(s16 y);
void Setup_Name(s16 y);
void Name_Sub(s16 xx, s16 y);
void Setup_Face(s16 y);
void Setup_Score(s16 y);
void Setup_Wins(s16 y);
void Setup_Wins2(s16 y);
void Setup_Ranking_Obj();
void Setup_Score_Obj();
void Ranking_Init();

#endif
