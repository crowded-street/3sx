#include "netplay/game_state.h"
#include "sf33rd/Source/Game/engine/workuser.h"

#include <SDL3/SDL.h>

#define GS_SAVE(member) SDL_memcpy(&dst->member, &member, sizeof(member))

void GameState_Save(_GameState* dst) {
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
}

#define GS_LOAD(member) SDL_memcpy(&member, &src->member, sizeof(member))

void GameState_Load(const _GameState* src) {
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
}
