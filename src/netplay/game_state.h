#ifndef NETPLAY_GAME_STATE_H
#define NETPLAY_GAME_STATE_H

#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/select_timer.h"
#include "structs.h"
#include "types.h"

typedef struct GameState {
    PLW plw[2];
    ZanzouTableEntry zanzou_table[2][48];
    SA_WORK super_arts[2];
    PiyoriType piyori_type[2];
    AppearanceType appear_type;
    s16 pcon_rno[4];
    bool round_slow_flag;
    bool pcon_dp_flag;
    u8 win_sp_flag;
    bool dead_voice_flag;
    bool Scene_Cut;
    bool Time_Over;

    s8 round_timer;
    s8 flash_timer;
    s8 flash_r_num;
    s8 flash_col;
    s8 math_counter_hi;
    s8 math_counter_low;
    u8 counter_color;
    bool mugen_flag;
    s8 hoji_counter;

    SelectTimerState select_timer_state;

    u8 Order[148];
    u8 Order_Timer[148];
    u8 Order_Dir[148];
    u32 Score[2][3];
    u32 Complete_Bonus;
    u32 Stock_Score[2];
    u32 Vital_Bonus[2];
    u32 Time_Bonus[2];
    u32 Stage_Stock_Score[2];
    u32 Bonus_Score;
    u32 Final_Bonus_Score;
    u32 WGJ_Score;
    u32 Bonus_Score_Plus;
    u32 Perfect_Bonus[2];
    u32 Keep_Score[2];
    u32 Disp_Score_Buff[2];
    s8 Winner_id;
    s8 Loser_id;
    s8 Break_Into;
    u8 My_char[2];
    u8 Allow_a_battle_f;
    u8 Round_num;
    s8 Complete_Judgement;
    s8 Fade_Flag;
    s8 Super_Arts[2];
    s8 Forbid_Break;
    s8 Request_Break[2];
    s8 Continue_Count[2];
    s8 Counter_hi;
    s8 Counter_low;
    s16 Unit_Of_Timer;
    s8 Select_Timer;
    s8 Cursor_X[2];
    s8 Cursor_Y[2];
    s8 Cursor_Y_Pos[2][4];
    s8 Cursor_Timer[2];
    s8 Time_Stop;
    s8 Suicide[8];
    s8 Complete_Face;
    u8 Play_Type;
    s16 Sel_PL_Complete[2];
    s8 New_Challenger;
    u8 S_No[4];
    s8 Select_Start[2];
} GameState;

void GameState_Save(GameState* dst);
void GameState_Load(const GameState* src);

#endif
