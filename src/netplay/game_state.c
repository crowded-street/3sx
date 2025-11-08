#include "netplay/game_state.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/select_timer.h"
#include "sf33rd/Source/Game/ui/count.h"

#include <SDL3/SDL.h>

#define GS_SAVE(member) SDL_memcpy(&dst->member, &member, sizeof(member))

void GameState_Save(GameState* dst) {
    GS_SAVE(plw);
    GS_SAVE(zanzou_table);
    GS_SAVE(super_arts);
    GS_SAVE(piyori_type);
    GS_SAVE(appear_type);
    GS_SAVE(pcon_rno);
    GS_SAVE(round_slow_flag);
    GS_SAVE(pcon_dp_flag);
    GS_SAVE(win_sp_flag);
    GS_SAVE(dead_voice_flag);
    GS_SAVE(Scene_Cut);
    GS_SAVE(Time_Over);
    GS_SAVE(round_timer);
    GS_SAVE(flash_timer);
    GS_SAVE(flash_r_num);
    GS_SAVE(flash_col);
    GS_SAVE(math_counter_hi);
    GS_SAVE(math_counter_low);
    GS_SAVE(counter_color);
    GS_SAVE(mugen_flag);
    GS_SAVE(hoji_counter);
    GS_SAVE(select_timer_state);
    GS_SAVE(Order);
    GS_SAVE(Order_Timer);
    GS_SAVE(Order_Dir);
    GS_SAVE(Score);
    GS_SAVE(Complete_Bonus);
    GS_SAVE(Stock_Score);
    GS_SAVE(Vital_Bonus);
    GS_SAVE(Time_Bonus);
    GS_SAVE(Stage_Stock_Score);
    GS_SAVE(Bonus_Score);
    GS_SAVE(Final_Bonus_Score);
    GS_SAVE(WGJ_Score);
    GS_SAVE(Bonus_Score_Plus);
    GS_SAVE(Perfect_Bonus);
    GS_SAVE(Keep_Score);
    GS_SAVE(Disp_Score_Buff);
    GS_SAVE(Winner_id);
    GS_SAVE(Loser_id);
    GS_SAVE(Break_Into);
    GS_SAVE(My_char);
    GS_SAVE(Allow_a_battle_f);
    GS_SAVE(Round_num);
    GS_SAVE(Complete_Judgement);
    GS_SAVE(Fade_Flag);
    GS_SAVE(Super_Arts);
    GS_SAVE(Forbid_Break);
    GS_SAVE(Request_Break);
    GS_SAVE(Continue_Count);
    GS_SAVE(Counter_hi);
    GS_SAVE(Counter_low);
    GS_SAVE(Unit_Of_Timer);
    GS_SAVE(Select_Timer);
    GS_SAVE(Cursor_X);
    GS_SAVE(Cursor_Y);
    GS_SAVE(Cursor_Y_Pos);
    GS_SAVE(Cursor_Timer);
    GS_SAVE(Time_Stop);
    GS_SAVE(Suicide);
    GS_SAVE(Complete_Face);
    GS_SAVE(Play_Type);
    GS_SAVE(Sel_PL_Complete);
    GS_SAVE(New_Challenger);
    GS_SAVE(S_No);
    GS_SAVE(Select_Start);
}

#define GS_LOAD(member) SDL_memcpy(&member, &src->member, sizeof(member))

void GameState_Load(const GameState* src) {
    GS_LOAD(plw);
    GS_LOAD(zanzou_table);
    GS_LOAD(super_arts);
    GS_LOAD(piyori_type);
    GS_LOAD(appear_type);
    GS_LOAD(pcon_rno);
    GS_LOAD(round_slow_flag);
    GS_LOAD(pcon_dp_flag);
    GS_LOAD(win_sp_flag);
    GS_LOAD(dead_voice_flag);
    GS_LOAD(Scene_Cut);
    GS_LOAD(Time_Over);
    GS_LOAD(round_timer);
    GS_LOAD(flash_timer);
    GS_LOAD(flash_r_num);
    GS_LOAD(flash_col);
    GS_LOAD(math_counter_hi);
    GS_LOAD(math_counter_low);
    GS_LOAD(counter_color);
    GS_LOAD(mugen_flag);
    GS_LOAD(hoji_counter);
    GS_LOAD(select_timer_state);
    GS_LOAD(Order);
    GS_LOAD(Order_Timer);
    GS_LOAD(Order_Dir);
    GS_LOAD(Score);
    GS_LOAD(Complete_Bonus);
    GS_LOAD(Stock_Score);
    GS_LOAD(Vital_Bonus);
    GS_LOAD(Time_Bonus);
    GS_LOAD(Stage_Stock_Score);
    GS_LOAD(Bonus_Score);
    GS_LOAD(Final_Bonus_Score);
    GS_LOAD(WGJ_Score);
    GS_LOAD(Bonus_Score_Plus);
    GS_LOAD(Perfect_Bonus);
    GS_LOAD(Keep_Score);
    GS_LOAD(Disp_Score_Buff);
    GS_LOAD(Winner_id);
    GS_LOAD(Loser_id);
    GS_LOAD(Break_Into);
    GS_LOAD(My_char);
    GS_LOAD(Allow_a_battle_f);
    GS_LOAD(Round_num);
    GS_LOAD(Complete_Judgement);
    GS_LOAD(Fade_Flag);
    GS_LOAD(Super_Arts);
    GS_LOAD(Forbid_Break);
    GS_LOAD(Request_Break);
    GS_LOAD(Continue_Count);
    GS_LOAD(Counter_hi);
    GS_LOAD(Counter_low);
    GS_LOAD(Unit_Of_Timer);
    GS_LOAD(Select_Timer);
    GS_LOAD(Cursor_X);
    GS_LOAD(Cursor_Y);
    GS_LOAD(Cursor_Y_Pos);
    GS_LOAD(Cursor_Timer);
    GS_LOAD(Time_Stop);
    GS_LOAD(Suicide);
    GS_LOAD(Complete_Face);
    GS_LOAD(Play_Type);
    GS_LOAD(Sel_PL_Complete);
    GS_LOAD(New_Challenger);
    GS_LOAD(S_No);
    GS_LOAD(Select_Start);
}
