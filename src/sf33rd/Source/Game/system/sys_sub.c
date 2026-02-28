/**
 * @file sys_sub.c
 * System State and Management Hub
 */

#include "sf33rd/Source/Game/system/sys_sub.h"
#include "common.h"
#include "main.h"
#include "netplay/netplay.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_datu.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/eff93.h"
#include "sf33rd/Source/Game/effect/effb8.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/game.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/menu/ex_data.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/rendering/mmtmcnt.h"
#include "sf33rd/Source/Game/screen/entry.h"
#include "sf33rd/Source/Game/screen/ranking.h"
#include "sf33rd/Source/Game/select_timer.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/system/sys_sub2.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include <memory.h>

u8 Candidate_Buff[16];
static bool training_hitbox_display_enabled;

// forward decls
void Disp_Win_Record_Sub(u16 win_record, s16 zz);
s32 Setup_Target_PL();
void Reset_Sub0();
void Setup_Replay_Header();
void Get_Replay_Header();
void Get_Replay(s16 PL_id);
void Setup_Replay_Buff(s16 PL_id, u16 sw_buff);
void Replay(s16 PL_id);
void Check_Partners_Rank(s16 dir_step, s16 PL_id);
s32 Check_Sort_Score(s16 PL_id);
s32 Check_Sort_Wins(s16 PL_id);
s32 Check_Sort_CPU_Grade(s16 PL_id);
s32 Check_Sort_Grade(s16 PL_id);
s32 Check_CPU_Grade_Score(s16 PL_id, s16 i);
s32 Check_Grade_Score(s16 PL_id, s16 i);
void Setup_Candidate_Buff(s16 PL_id);
s16 Check_EM_Buff(s16 ix, s16 ok_urien);
s32 Check_EM_Sub(s16 ix, s16 ok_urien, s16 Rnd);

const u16 Convert_Data[12] = { 16, 32, 64, 256, 512, 1024, 272, 544, 1088, 112, 1792, 0 };

void Switch_Screen_Init(s32 /* unused */) {
    WipeInit();
    Forbid_Break = 1;
    Exec_Wipe = 1;
    Gap_Timer = 4;
    Stop_SG = 1;
    Escape_SS = 1;
}

s32 Switch_Screen(u8 Wipe_Type) {
    if (WipeOut(Wipe_Type) && --Gap_Timer <= 0) {
        Exec_Wipe = 0;
        Stop_Combo = 0;
        return 1;
    }

    return 0;
}

s32 Switch_Screen_Revival(u8 Wipe_Type) {
    if (WipeIn(Wipe_Type) && --Gap_Timer <= 0) {
        Exec_Wipe = 0;
        Stop_Combo = 0;
        return 1;
    }

    return 0;
}

u16 Convert_User_Setting(s16 PL_id) {
    u16 sw;
    u16 answer;

    if (Debug_w[70] == 16) {
        if (PL_id == 0) {
            return p1sw_0;
        }

        return p2sw_0;
    }

    if (PL_id == 0) {
        sw = p1sw_0;
    } else {
        sw = p2sw_0;
    }

    answer = sw & (SWK_DIRECTIONS | SWK_START);

    if (sw & SWK_WEST) {
        answer |= Convert_Data[save_w[Present_Mode].Pad_Infor[PL_id].Shot[0]];
    }

    if (sw & SWK_NORTH) {
        answer |= Convert_Data[save_w[Present_Mode].Pad_Infor[PL_id].Shot[1]];
    }

    if (sw & SWK_RIGHT_SHOULDER) {
        answer |= Convert_Data[save_w[Present_Mode].Pad_Infor[PL_id].Shot[2]];
    }

    if (sw & SWK_LEFT_SHOULDER) {
        answer |= Convert_Data[save_w[Present_Mode].Pad_Infor[PL_id].Shot[3]];
    }

    if (sw & SWK_SOUTH) {
        answer |= Convert_Data[save_w[Present_Mode].Pad_Infor[PL_id].Shot[4]];
    }

    if (sw & SWK_EAST) {
        answer |= Convert_Data[save_w[Present_Mode].Pad_Infor[PL_id].Shot[5]];
    }

    if (sw & SWK_RIGHT_TRIGGER) {
        answer |= Convert_Data[save_w[Present_Mode].Pad_Infor[PL_id].Shot[6]];
    }

    if (sw & SWK_LEFT_TRIGGER) {
        answer |= Convert_Data[save_w[Present_Mode].Pad_Infor[PL_id].Shot[7]];
    }

    return answer;
}

void Clear_Personal_Data(s16 PL_id) {
    s16 xx;

    Lost_Round[PL_id] = 0;
    Super_Arts_Finish[PL_id] = 0;
    Perfect_Finish[PL_id] = 0;
    Cheap_Finish[PL_id] = 0;
    Completion_Bonus[PL_id][0] = 0;
    Completion_Bonus[PL_id][1] = 0;
    Stage_Continue[PL_id] = 0;
    Introduce_Boss[PL_id][0] = 0;
    Introduce_Boss[PL_id][1] = 0;
    Introduce_Break_Into[PL_id] = 0;
    Score[PL_id][0] = 0;
    Stock_Score[PL_id] = 0;
    Stage_Stock_Score[PL_id] = 0;
    Continue_Coin[PL_id] = 0;
    Win_Record[PL_id] = 0;
    VS_Win_Record[PL_id] = 0;
    Stock_Win_Record[PL_id] = 0;
    VS_Index[PL_id] = 0;
    Used_char[PL_id] = 0xFF;
    Arts_Y[PL_id] = 0;
    Continue_Count[PL_id] = 0;
    Continue_Coin2[PL_id] = 0;
    Sel_PL_Complete[PL_id] = 0;
    Sel_Arts_Complete[PL_id] = 0;
    Sel_EM_Complete[PL_id] = 0;
    Last_Player_id = -1;
    Last_Super_Arts[PL_id] = 0;
    Last_My_char[PL_id] = -1;
    Last_My_char2[PL_id] = -1;
    Last_Selected_EM[PL_id] = 1;
    Select_Start[PL_id] = 0;
    paring_ctr_vs[0][PL_id] = 0;
    Straight_Counter[PL_id] = 0;
    Straight_Flag[PL_id] = 0;
    SC_Personal_Time[PL_id] = 481;
    E_Number[PL_id][0] = 0;
    E_Number[PL_id][1] = 0;
    E_Number[PL_id][2] = 0;
    E_Number[PL_id][3] = 0;
    E_07_Flag[PL_id] = 0;
    Request_Break[PL_id] = 0;

    if (PL_id == 0) {
        Cursor_X[0] = permission_player[Present_Mode].cursor_infor[0].first_x;
        Cursor_Y[0] = permission_player[Present_Mode].cursor_infor[0].first_y;
    } else {
        Cursor_X[1] = permission_player[Present_Mode].cursor_infor[1].first_x;
        Cursor_Y[1] = permission_player[Present_Mode].cursor_infor[1].first_y;
    }

    for (xx = 0; xx < 10; xx++) {
        EM_History[PL_id][xx] = 0;
    }
}

s16 Check_Count_Cut(s16 PL_id, s16 Limit) {
    s16 xx;

    Continue_Cut[PL_id] = 0;

    if (Continue_Count[PL_id] >= (Limit)) {
        return 0;
    }

    if (PL_id) {
        xx = p2sw_0 & ~p2sw_1;
    } else {
        xx = p1sw_0 & ~p1sw_1;
    }

    return xx & 0xFF0;
}

void Disp_Personal_Count(s16 PL_id, s8 counter) {
    SSPutDec(DE_X[PL_id] + 14, 0, 9, counter, 0);
}

void Setup_Play_Type() {
    if (Operator_Status[0] & 0x7F && Operator_Status[1] & 0x7F) {
        Play_Type = 1;
    } else {
        Play_Type = 0;
    }
}

void Clear_Flash_No() {
    F_No0[0] = F_No1[0] = F_No2[0] = F_No3[0] = 0;
    F_No0[1] = F_No1[1] = F_No2[1] = F_No3[1] = 0;
}

void Set_Training_Hitbox_Display(bool enabled) {
    training_hitbox_display_enabled = enabled;
}

bool Is_Training_Hitbox_Display_Enabled() {
    return training_hitbox_display_enabled;
}

bool Cut_Cut_Cut() {
    if (Demo_Flag == 0) {
        return false;
    }

    if (plw[0].wu.operator && (p1sw_0 & SWK_ATTACKS)) {
        return true;
    }

    if (plw[1].wu.operator && (p2sw_0 & SWK_ATTACKS)) {
        return true;
    }

    return false;
}

void Score_Sub() {
    u32 Score_Buff;
    s8 i;
    s8 j;
    s32 xx;
    s8 First_Digit;
    s8 Digit[8];
    s16 PL_id;

    s8 assign1;
    s32 assign2;
    s8 assign3;

    if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
        return;
    }

    if (omop_cockpit == 0) {
        return;
    }

    for (PL_id = 0; PL_id < 2; PL_id++) {
        if ((Mode_Type != MODE_VERSUS && Mode_Type != MODE_REPLAY) && plw[PL_id].wu.operator == 0) {
            continue;
        }

        if (Stop_Update_Score) {
            Score_Buff = Keep_Score[PL_id];
        } else {
            Score_Buff = Score[PL_id][Play_Type];
            Score_Buff += Continue_Coin[PL_id];
            Keep_Score[PL_id] = Score_Buff;
        }

        for (i = 7, xx = 10000000, assign1 = First_Digit = -1; i > 0; i--, assign2 = xx /= 10) {
            Digit[i] = Score_Buff / xx;
            Score_Buff -= Digit[i] * xx;

            if (First_Digit < 0 && Digit[i]) {
                First_Digit = i;
            }
        }

        Digit[0] = Score_Buff;

        if (First_Digit < 0) {
            First_Digit = 1;
        }

        for (i = Coin_Message_Data[3][PL_id] - First_Digit, j = First_Digit; j >= 0; j--, assign3 = i++) {
            score8x16_put(i, 0, 8, Digit[j]);
        }
    }
}

void Disp_Win_Record() {
    s16 PL_id;
    s16 zz;

    if (omop_cockpit == 0) {
        return;
    }

    switch (Mode_Type) {
    case MODE_ARCADE:
        if (Play_Type == 1) {
            if (Win_Record[0] != 0 || Win_Record[1] != 0) {
                if (Win_Record[0]) {
                    PL_id = 0;
                    zz = 5;
                } else {
                    PL_id = 1;
                    zz = 43;
                }
            } else {
                break;
            }
        } else if (Win_Record[Player_id] == 0) {
            break;
        } else {
            PL_id = Player_id;

            if (Player_id == 0) {
                zz = 5;
            } else {
                zz = 43;
            }
        }

        Disp_Win_Record_Sub(Win_Record[PL_id], zz);
        break;

    case MODE_VERSUS:
    case MODE_NETWORK:
        if (VS_Win_Record[0] > 0) {
            Disp_Win_Record_Sub(VS_Win_Record[0], 5);
        }

        if (VS_Win_Record[1] > 0) {
            Disp_Win_Record_Sub(VS_Win_Record[1], 43);
        }

        break;

    default:
        // Do nothing
        break;
    }
}

void Disp_Win_Record_Sub(u16 win_record, s16 zz) {
    s16 xx;
    s16 Wins_Buff;
    s16 First_Digit;

    switch (win_record) {
    case 1:
        SSPutStr(zz, 0, 9, "WIN");
        break;

    default:
        SSPutStr(zz, 0, 9, "WINS");
        break;
    }

    First_Digit = 0;
    Wins_Buff = win_record;
    xx = Wins_Buff / 100;

    if (xx > 0) {
        First_Digit = 1;
        SSPutDec(zz - 4, 0, 9, xx, 1);
    }

    Wins_Buff -= xx * 100;
    xx = Wins_Buff / 10;

    if (First_Digit != 0 || xx > 0) {
        SSPutDec(zz - 3, 0, 9, xx, 1);
    }

    Wins_Buff -= xx * 10;

    SSPutDec(zz - 2, 0, 9, Wins_Buff, 1);
}

s32 Button_Cut_EX(s16* Timer, s16 Limit_Time) {
    s16 PL_id = Setup_Target_PL();
    u16 xx;

    if (PL_id) {
        xx = p2sw_0;
    } else {
        xx = p1sw_0;
    }

    --*Timer;

    if (*Timer == 0) {
        return 1;
    }

    if ((xx & 0xFF0) && Limit_Time >= *Timer) {
        return 1;
    }

    return 0;
}

s32 Setup_Target_PL() {
    if (Play_Type == 1) {
        return Winner_id;
    }

    if (Round_Operator[0]) {
        return 0;
    }

    if (plw[0].wu.operator) {
        return 0;
    }

    return 1;
}

void Setup_Final_Grade() {
    if (Break_Com[Player_id][0] == 0) {
        Final_Result_id = LOSER;
        WGJ_Target = LOSER;
        WGJ_Win = Win_Record[LOSER];
        WGJ_Score = Continue_Coin[LOSER] + Score[LOSER][0];
    }
}

void Clear_Win_Type() {
    s16 i;

    for (i = 0; i < 4; i++) {
        win_type[0][i] = 0;
        win_type[1][i] = 0;
        flash_win_type[0][i] = 0;
        flash_win_type[1][i] = 0;
        sync_win_type[0][i] = 0;
        sync_win_type[1][i] = 0;
    }
}

void Clear_Disp_Ranking(s16 PL_id) {
    s16 ix;

    for (ix = 0; ix <= 3; ix++) {
        Request_Disp_Rank[PL_id][ix] = -1;
        Rank_In[PL_id][ix] = -1;
    }
}

void Meltw(u16* s, u16* d, s32 file_ptr) {
    s32 flag;
    s32 i;
    u32 s_cnt;
    u32 s_len;
    u16* s_ptr;

    while (1) {
        flag = *s++ * 0x10000;
        file_ptr--;
        i = 16;

        do {
            if (flag >= 0) {
                *d++ = *s++;
                file_ptr--;
            } else {
                s_len = *s++;
                s_cnt = s_len >> 11;

                if (s_cnt != 0) {
                    s_len = s_len & 0x7FF;
                    file_ptr--;
                } else {
                    s_cnt = *s++;
                    file_ptr -= 2;
                }

                if (s_len == 0 && s_cnt == 0) {
                    return;
                }

                if (s_len == 0) {
                    do {
                        *d++ = 0;
                    } while (--s_cnt);
                } else {
                    s_ptr = d - s_len;

                    do {
                        *d++ = *s_ptr++;
                    } while (--s_cnt);
                }
            }

            flag <<= 1;
        } while (--i);
    }
}

void Setup_ID() {
    if (Operator_Status[0] == 0) {
        COM_id = 0;
        Player_id = 1;
    } else {
        COM_id = 1;
        Player_id = 0;
    }
}

void Game_Data_Init() {
    s32 ix;

    Setup_Default_Game_Option();
    mpp_w.cutAnalogStickData = false;

    if ((flpad_adr[0][0].sw & 0x330) == 0x330) {
        mpp_w.cutAnalogStickData = true;
    } else if ((flpad_adr[0][1].sw & 0x330) == 0x330) {
        mpp_w.cutAnalogStickData = true;
    }

    if (mpp_w.cutAnalogStickData) {
        for (ix = 0; ix < 6; ix++) {
            save_w[ix].AnalogStick = 0;
        }
    }

    Ranking_Init();
    Copy_Save_w();
}

void Setup_IO_ConvDataDefault(s32 id) {
    const u8 ioConvInitData[12] = { 0, 1, 2, 11, 3, 4, 5, 11, 0, 0, 0, 0 };
    s32 ix;

    for (ix = 0; ix < 12; ix++) {
        Convert_Buff[1][id][ix] = ioConvInitData[ix];
    }
}

const s8 Time_Limit_Data[4] = { 30, 60, 99, -1 };

const s8 Battle_Number_Data[4] = { 0, 1, 2, 3 };

void Save_Game_Data() {
    s16 ix;

    save_w[1].Difficulty = Convert_Buff[0][0][0];
    save_w[1].Time_Limit = Time_Limit_Data[Convert_Buff[0][0][1]];
    save_w[1].Battle_Number[0] = Battle_Number_Data[Convert_Buff[0][0][2]];
    save_w[1].Battle_Number[1] = Battle_Number_Data[Convert_Buff[0][0][3]];
    save_w[1].Damage_Level = Convert_Buff[0][0][4];
    save_w[1].GuardCheck = Convert_Buff[0][0][5];
    save_w[1].AnalogStick = Convert_Buff[0][0][6];
    save_w[1].Handicap = Convert_Buff[0][0][7];
    save_w[1].Partner_Type[0] = Convert_Buff[0][0][8];
    save_w[1].Partner_Type[1] = Convert_Buff[0][0][9];
    mpp_w.useAnalogStickData = save_w[1].AnalogStick;
    save_w[4].GuardCheck = save_w[1].GuardCheck;
    save_w[5].GuardCheck = save_w[1].GuardCheck;

    for (ix = 0; ix < 8; ix++) {
        save_w[1].Pad_Infor[0].Shot[ix] = Convert_Buff[1][0][ix];
        save_w[1].Pad_Infor[1].Shot[ix] = Convert_Buff[1][1][ix];
    }

    save_w[1].Pad_Infor[0].Vibration = Convert_Buff[1][0][8];
    save_w[1].Pad_Infor[1].Vibration = Convert_Buff[1][1][8];
    save_w[4].Pad_Infor[0] = save_w[1].Pad_Infor[0];
    save_w[4].Pad_Infor[1] = save_w[1].Pad_Infor[1];
    save_w[5].Pad_Infor[0] = save_w[1].Pad_Infor[0];
    save_w[5].Pad_Infor[1] = save_w[1].Pad_Infor[1];
    save_w[1].Adjust_X = Convert_Buff[2][0][0];
    save_w[1].Adjust_Y = Convert_Buff[2][0][1];
    save_w[1].Screen_Size = Convert_Buff[2][0][2];
    save_w[1].Screen_Mode = Convert_Buff[2][0][3];
    save_w[1].Auto_Save = Convert_Buff[3][0][2];
    save_w[1].SoundMode = Convert_Buff[3][1][0];
    save_w[1].BGM_Level = Convert_Buff[3][1][1];
    save_w[1].SE_Level = Convert_Buff[3][1][2];
    save_w[1].BgmType = Convert_Buff[3][1][3];
}

void Copy_Save_w() {
    s16 ix;

    Convert_Buff[0][0][0] = save_w[1].Difficulty;
    Convert_Buff[0][0][2] = save_w[1].Battle_Number[0];
    Convert_Buff[0][0][3] = save_w[1].Battle_Number[1];
    Convert_Buff[0][0][4] = save_w[1].Damage_Level;
    Convert_Buff[0][0][5] = save_w[1].GuardCheck;
    Convert_Buff[0][0][6] = save_w[1].AnalogStick;
    Convert_Buff[0][0][7] = save_w[1].Handicap;
    Convert_Buff[0][0][8] = save_w[1].Partner_Type[0];
    Convert_Buff[0][0][9] = save_w[1].Partner_Type[1];
    mpp_w.useAnalogStickData = save_w[1].AnalogStick;

    switch (save_w[1].Time_Limit) {
    case 30:
        Convert_Buff[0][0][1] = 0;
        break;
    case 60:
        Convert_Buff[0][0][1] = 1;
        break;
    case 99:
        Convert_Buff[0][0][1] = 2;
        break;
    default:
        Convert_Buff[0][0][1] = 3;
        break;
    }

    for (ix = 0; ix < 8; ix++) {
        Convert_Buff[1][0][ix] = save_w[1].Pad_Infor[0].Shot[ix];
        Convert_Buff[1][1][ix] = save_w[1].Pad_Infor[1].Shot[ix];
    }

    Convert_Buff[1][0][8] = save_w[1].Pad_Infor[0].Vibration;
    Convert_Buff[1][1][8] = save_w[1].Pad_Infor[1].Vibration;
    Convert_Buff[2][0][0] = save_w[1].Adjust_X;
    Convert_Buff[2][0][1] = save_w[1].Adjust_Y;
    Convert_Buff[2][0][2] = save_w[1].Screen_Size;
    Convert_Buff[2][0][3] = save_w[1].Screen_Mode;
    sys_w.screen_mode = save_w[1].Screen_Mode;
    Convert_Buff[3][0][2] = save_w[1].Auto_Save;
    Convert_Buff[3][1][0] = save_w[1].SoundMode;
    Convert_Buff[3][1][1] = save_w[1].BGM_Level;
    Convert_Buff[3][1][2] = save_w[1].SE_Level;
    Convert_Buff[3][1][3] = save_w[1].BgmType;
    for (ix = 0; ix < 20; ix++) {
        Ranking_Data[ix] = save_w[1].Ranking[ix];
    }
}

void Copy_Check_w() {
    s16 ix;
    s16 ix2;

    for (ix = 0; ix < 4; ix++) {
        for (ix2 = 0; ix2 < 12; ix2++) {
            Check_Buff[ix][0][ix2] = Convert_Buff[ix][0][ix2];
            Check_Buff[ix][1][ix2] = Convert_Buff[ix][1][ix2];
        }
    }

    ck_ex_option = save_w[1].extra_option;
}

const struct _SAVE_W Game_Default_Data = {
    { { { 0, 1, 2, 11, 3, 4, 5, 11 }, 0, { 0, 0, 0 } }, { { 0, 1, 2, 11, 3, 4, 5, 11 }, 0, { 0, 0, 0 } } },
    2,
    99,
    { 1, 1 },
    1,
    1,
    { 0, 0 },
    0,
    0,
    0,
    1,
    0,
    0,
    1,
    0,
    0,
    15,
    15,
    0,
    { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
    { { { 1, 3, 3, 0, 0, 1, 0, 0 },
        { 0, 0, 2, 2, 8, 8, 2, 0 },
        { 2, 2, 2, 2, 0, 0, 0, 0 },
        { 1, 1, 1, 1, 1, 1, 0, 0 } } },
    { { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 },
      { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 },
      { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 },
      { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 },
      { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 },
      { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 },
      { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 }, { { 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0 } },
    0
};

void Setup_Default_Game_Option() {
    s16 ix;

    for (ix = 0; ix < 6; ix++) {
        save_w[ix] = Game_Default_Data;
        save_w[ix].sum = 0;
    }
}

s32 Check_Change_Contents() {
    s16 ix;
    s16 ix2;
    s16 page;

    Check_Buff[3][0][0] = Convert_Buff[3][0][0];

    for (ix = 4; ix < 12; ix++) {
        Check_Buff[3][1][ix] = Convert_Buff[3][1][ix];
    }

    for (ix = 0; ix < 4; ix++) {
        for (ix2 = 0; ix2 < 12; ix2++) {
            if (Convert_Buff[ix][0][ix2] != Check_Buff[ix][0][ix2]) {
                return 1;
            }

            if (Convert_Buff[ix][1][ix2] != Check_Buff[ix][1][ix2]) {
                return 1;
            }
        }
    }

    for (page = 0; page < 4; page++) {
        for (ix = Ex_Page_Data[page]; ix < 8; ix++) {
            ck_ex_option.contents[page][ix] = save_w[1].extra_option.contents[page][ix];
        }
    }

    for (ix = 0; ix < 4; ix++) {
        for (ix2 = 0; ix2 < 8; ix2++) {
            if (ck_ex_option.contents[ix][ix2] != save_w[1].extra_option.contents[ix][ix2]) {
                return 1;
            }
        }
    }

    return 0;
}

void cpRevivalTask() {
    struct _TASK* task_ptr;
    s16 ix;

    for (task_ptr = task, ix = 0; ix < 11; task_ptr++, ix++) {
        task_ptr->condition = keep_condition[ix];
    }
}

s32 Check_Menu_Task() {
    struct _TASK* task_ptr = &task[TASK_MENU];

    if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
        if (task[TASK_MENU].r_no[0] == 7 && task[TASK_MENU].r_no[1] == 7) {
            return 1;
        }

        return 0;
    }

    if (task_ptr->condition == 1) {
        return 1;
    }

    return 0;
}

void Setup_Limit_Time() {
    s16 limit;

    limit = Level_18_Data[save_w[Present_Mode].Difficulty][16];
    limit += 20;
    if (Country == 1) {
        Limit_Time = 1241;
    } else {
        Limit_Time = 1061;
    }

    if (limit > Limit_Time) {
        Limit_Time = limit;
    }
}

void Setup_Training_Difficulty() {
    s16 unit_time;
    s16 min_time;

    unit_time = Limit_Time - 481;
    unit_time = unit_time / 5;
    min_time = 481 - (unit_time * 2);

    if (min_time < 0) {
        min_time = 0;
    }

    Control_Time = min_time + (unit_time * save_w[Present_Mode].Difficulty);
}

void Setup_BG(s16 BG_INDEX, s16 X, s16 Y) {
    Unsubstantial_BG[BG_INDEX] = 1;
    bg_w.bgw[BG_INDEX].xy[0].disp.pos = X;
    bg_w.bgw[BG_INDEX].xy[1].disp.pos = Y;
    bg_w.bgw[BG_INDEX].wxy[0].disp.pos = X;
    bg_w.bgw[BG_INDEX].wxy[1].disp.pos = Y;
    bg_w.bgw[BG_INDEX].xy[0].disp.low = 0;
    bg_w.bgw[BG_INDEX].xy[1].disp.low = 0;
    bg_w.bgw[BG_INDEX].position_x = X;
    bg_w.bgw[BG_INDEX].position_y = Y;
    Bg_Family_Set_Ex(BG_INDEX);
}

void Setup_Virtual_BG(s16 BG_INDEX, s16 X, s16 Y) {
    bg_w.bgw[BG_INDEX].xy[0].disp.pos = X;
    bg_w.bgw[BG_INDEX].xy[1].disp.pos = Y;
    bg_w.bgw[BG_INDEX].wxy[0].disp.pos = X;
    bg_w.bgw[BG_INDEX].wxy[1].disp.pos = Y;
    bg_w.bgw[BG_INDEX].xy[0].disp.low = 0;
    bg_w.bgw[BG_INDEX].xy[1].disp.low = 0;
    bg_w.bgw[BG_INDEX].position_x = X;
    bg_w.bgw[BG_INDEX].position_y = Y;
    Bg_Family_Set_Ex(BG_INDEX);
}

void BG_move() {
    s16 ix;

    for (ix = 0; ix < 4; ix++) {
        if (Unsubstantial_BG[ix]) {
            bg_pos_hosei_sub2(ix);
            Bg_Family_Set_appoint(ix);
        }
    }
}

void BG_move_Ex(u8 ix) {
    scr_calc(ix);
}

void Basic_Sub() {
    bg_w.bgw[0].old_pos_x = bg_w.bgw[0].xy[0].disp.pos;
    move_effect_work(0);
    move_effect_work(1);
    move_effect_work(2);
    move_effect_work(3);
    move_effect_work(4);
    move_effect_work(5);
}

void Basic_Sub_Ex() {
    move_effect_work(0);
    move_effect_work(1);
    move_effect_work(2);
    move_effect_work(3);
    move_effect_work(4);
    move_effect_work(5);
}

s32 Check_PL_Load() {
    if (!Check_LDREQ_Queue_Player(0) || !Check_LDREQ_Queue_Player(1)) {
        return 0;
    }

    return 1;
}

void BG_Draw_System() {
    u8 i;
    u16 mask = 1 & 0xFFFF;
    u16 s2;
    u16 s3;

    if (bg_disp_off == 0) {
        for (i = 0; i < 4; i++, s2 = mask *= 2) {
            if (Screen_Switch_Buffer & mask) {
                scr_trans(i);
            }
        }
    } else {
        for (i = 0; i < 4; i++, s3 = mask *= 2) {
            if (Screen_Switch_Buffer & mask) {
                scr_calc(i);
            }
        }
    }

    if (Play_Game == 0) {
        for (i = 0; i < 4; i++) {
            if (Unsubstantial_BG[i]) {
                scr_calc(i);
            }
        }
    } else if (Play_Game == 1) {
        Family_Move();
    } else {
        Ending_Family_Move();
    }
}

u16 Check_Demo_Data(s16 PL_id) {
    u16 ans;

    if (Demo_Timer[PL_id] == 0) {
        ans = *Demo_Ptr[PL_id];
        Demo_Ptr[PL_id]++;
    } else {
        Demo_Timer[PL_id]--;
        return 0;
    }

    if (ans & 0x8000) {
        Demo_Timer[PL_id] = ans & 0x7FFF;
        return 0;
    }

    return ans;
}

void System_all_clear_Level_B() {
    Bg_Close();
    effect_work_init();
    SelectTimer_Finish();
}

s16 Cut_Cut_C_Timer() {
    C_Timer--;

    if (!Cut_Cut_Cut()) {
        return C_Timer;
    }

    return C_Timer = 0;
}

void Switch_Priority_76() {
    Order[56] = 7;
    Order_Timer[56] = 1;
}

s32 Cut_Cut_Sub(s16 xx) {
    if (Demo_Flag == 0) {
        return 1;
    }

    if (plw[0].wu.operator && (p1sw_0 & SWK_ATTACKS)) {
        return xx;
    }

    if (plw[1].wu.operator && (p2sw_0 & SWK_ATTACKS)) {
        return xx;
    }

    return 1;
}

bool Cut_Cut_Loser() {
    if (Round_Operator[0] && (p1sw_0 & SWK_ATTACKS)) {
        return true;
    }

    if (Round_Operator[1] && (p2sw_0 & SWK_ATTACKS)) {
        return true;
    }

    return false;
}

void njWaitVSync_with_N() {
    while (1) {}
}

void Soft_Reset_Sub() {
    FadeOut(1, 0xFF, 8);
    sound_all_off();
    SsBgmHalfVolume(0);

    if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
        Set_Training_Hitbox_Display(false);
    }

    if (task[TASK_GAME].condition == 0) {
        cpReadyTask(TASK_GAME, Game_Task);
    }

    if (task[TASK_DEBUG].condition == 0) {
        cpReadyTask(TASK_DEBUG, Debug_Task);
    }

    Next_Title_Sub();
    Bg_TexInit();
    Purge_mmtm_area(6);
    Make_texcash_of_list(6);
    pulpul_stop();
    init_pulpul_work();
    pp_operator_check_flag(1);
    Init_Load_Request_Queue_1st();
    Netplay_CancelMatchmaking();
    cpExitTask(TASK_MENU);
    cpExitTask(TASK_SAVER);
    cpExitTask(TASK_PAUSE);
    Reset_Sub0();
    task[TASK_INIT].r_no[0] = 1;
    task[TASK_INIT].r_no[1] = 0;
    task[TASK_INIT].r_no[2] = 0;
    task[TASK_INIT].r_no[3] = 0;
    vm_w.Request = 0;
    vm_w.Access = 0;
}

void Reset_Sub0() {
    Pause = 0;
    Game_pause = 0;
    Play_Game = 0;
    Forbid_Break = 0;
    Extra_Break = 0;
    Mode_Type = MODE_ARCADE;
    Present_Mode = 1;
    Play_Mode = 0;
    Replay_Status[0] = 0;
    Replay_Status[1] = 0;
}

void Check_Replay() {
    s16 ix;

    if (!Demo_Flag) {
        return;
    }

    switch (Play_Mode) {
    case 1:
        Replay_Status[0] = 1;
        Replay_Status[1] = 1;

        if (plw[0].wu.operator == 0) {
            Replay_Status[0] = 0;
            CP_No[0][0] = 0;
        }

        if (plw[1].wu.operator == 0) {
            Replay_Status[1] = 0;
            CP_No[1][0] = 0;
        }

        Condense_Buff[0] = 0xFFFF;
        Condense_Buff[1] = 0xFFFF;
        memset(&Replay_w, 0, sizeof(Replay_w));

        if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
            for (ix = 0; ix < 0x1C1E; ix++) {
                Replay_w.io_unit.key_buff[0][ix] = 0xF000;
                Replay_w.io_unit.key_buff[1][ix] = 0xF000;
            }
        }

        Setup_Replay_Header();

        for (ix = 0; ix < 14; ix++) {
            Replay_w.lag[ix] = 1;
        }

        Lag_Ptr = Replay_w.lag;
        Lag_Timer = 1;
        Bg_Kakikae_Set();
        break;

    case 3:
        Replay_Status[0] = 3;
        Replay_Status[1] = 3;
        CP_No[0][0] = 0;
        CP_No[1][0] = 0;
        Vital_Handicap[Present_Mode][0] = Rep_Game_Infor[10].Vital_Handicap[0];
        Vital_Handicap[Present_Mode][1] = Rep_Game_Infor[10].Vital_Handicap[1];
        Get_Replay_Header();
        Lag_Ptr = Replay_w.lag;
        Lag_Timer = (s8)*Lag_Ptr;
        Lag_Ptr += 1;
        Bg_Kakikae_Set();
        break;

    default:
        return;
    }

    Record_Timer = 0;
    Demo_Timer[0] = 0;
    Demo_Timer[1] = 0;
    Demo_Ptr[0] = Replay_w.io_unit.key_buff[0];
    Demo_Ptr[1] = Replay_w.io_unit.key_buff[1];
}

void Setup_Replay_Header() {
    s16 ix;

    Rep_Game_Infor[10].stage = bg_w.stage;
    Rep_Game_Infor[10].Direction_Working = Direction_Working[Present_Mode];
    Rep_Game_Infor[10].Vital_Handicap[0] = Vital_Handicap[Present_Mode][0];
    Rep_Game_Infor[10].Vital_Handicap[1] = Vital_Handicap[Present_Mode][1];

    for (ix = 0; ix < 2; ix++) {
        Rep_Game_Infor[10].player_infor[ix].my_char = My_char[ix];
        Rep_Game_Infor[10].player_infor[ix].sa = Super_Arts[ix];
        Rep_Game_Infor[10].player_infor[ix].color = Player_Color[ix];
        Rep_Game_Infor[10].player_infor[ix].player_type = plw[ix].wu.operator;
        Rep_Game_Infor[10].Vital_Handicap[ix] = Vital_Handicap[Present_Mode][ix];
    }

    Rep_Game_Infor[10].Random_ix16 = Random_ix16;
    Rep_Game_Infor[10].Random_ix32 = Random_ix32;
    Rep_Game_Infor[10].Random_ix16_ex = Random_ix16_ex;
    Rep_Game_Infor[10].Random_ix32_ex = Random_ix32_ex;
    Rep_Game_Infor[10].players_timer = players_timer;
    Random_ix16_com = Random_ix16;
    Random_ix32_com = Random_ix32;
    Random_ix16_ex_com = Random_ix16_ex;
    Random_ix32_ex_com = Random_ix32_ex;
    Random_ix16_bg = Random_ix16;
    Rep_Game_Infor[10].old_mes_no2 = old_mes_no2;
    Rep_Game_Infor[10].old_mes_no3 = old_mes_no3;
    Rep_Game_Infor[10].old_mes_no_pl = old_mes_no_pl;
    Rep_Game_Infor[10].mes_already = mes_already;
    Replay_w.champion = Champion;
    Replay_w.full_data = 0;
}

void Get_Replay_Header() {
    Random_ix16 = Rep_Game_Infor[10].Random_ix16;
    Random_ix32 = Rep_Game_Infor[10].Random_ix32;
    Random_ix16_ex = Rep_Game_Infor[10].Random_ix16_ex;
    Random_ix32_ex = Rep_Game_Infor[10].Random_ix32_ex;
    players_timer = Rep_Game_Infor[10].players_timer;
    old_mes_no2 = Rep_Game_Infor[10].old_mes_no2;
    old_mes_no3 = Rep_Game_Infor[10].old_mes_no3;
    old_mes_no_pl = Rep_Game_Infor[10].old_mes_no_pl;
    mes_already = Rep_Game_Infor[10].mes_already;
    Random_ix16_com = Random_ix16;
    Random_ix32_com = Random_ix32;
    Random_ix16_ex_com = Random_ix16_ex;
    Random_ix32_ex_com = Random_ix32_ex;
    Random_ix16_bg = Random_ix16;
    Champion = Replay_w.champion;
    New_Challenger = Champion ^ 1;
    Control_Time = Replay_w.Control_Time_Buff;
    save_w[Present_Mode].Difficulty = Replay_w.Difficulty;
}

void Check_Replay_Status(s16 PL_id, u8 Status) {
    if (Demo_Flag == 0) {
        return;
    }

    switch (Status) {
    case 1:
        Get_Replay(PL_id);

        if ((Game_pause != 0x81) && Debug_w[0x21]) {
            flPrintColor(0xFFFFFFFF);
            flPrintL(16, 8, "HUMAN REC!");
            break;
        }

        break;

    case 3:
        Replay(PL_id);
        break;

    case 2:
        if (PL_id) {
            p2sw_0 = 0;
            break;
        }

        p1sw_0 = 0;
        break;

    case 99:
        flPrintColor(0xFFFFFF00);
        flPrintL(12, 20, "[REPLAY AREA FULL!!]");
        Disp_Rec_Time(PL_id, Rec_Time[PL_id]);
        break;
    }
}

void Get_Replay(s16 PL_id) {
    u16 sw_buff;

    if (Game_pause == 0x81) {
        return;
    }

    if (PL_id) {
        sw_buff = p2sw_0;
    } else {
        sw_buff = p1sw_0;
    }

    if (sw_buff == Condense_Buff[PL_id]) {
        if (Demo_Timer[PL_id] >= 16) {
            Setup_Replay_Buff(PL_id, sw_buff);
        } else {
            Demo_Timer[PL_id]++;
        }
    } else {
        Setup_Replay_Buff(PL_id, sw_buff);
    }

    if (PL_id == 0) {
        Disp_Rec_Time(PL_id, Record_Timer);
    }
}

void Setup_Replay_Buff(s16 PL_id, u16 sw_buff) {
    u16 buff;
    u16 timer;

    if (Condense_Buff[PL_id] == 0xFFFF) {
        Demo_Timer[PL_id] = 1;
        Condense_Buff[PL_id] = sw_buff;
        return;
    }

    timer = Demo_Timer[PL_id] - 1;
    timer <<= 12;
    buff = Condense_Buff[PL_id] & 0xFFF;
    buff |= timer;
    *Demo_Ptr[PL_id] = buff;
    Demo_Ptr[PL_id]++;

    if (&Replay_w.io_unit.key_buff[PL_id][7197] < Demo_Ptr[PL_id]) {
        Replay_Status[PL_id] = 99;
        Replay_w.full_data |= PL_id + 1;
        Rec_Time[PL_id] = Record_Timer;
        return;
    }

    Demo_Timer[PL_id] = 1;
    Condense_Buff[PL_id] = sw_buff;
}

void Replay(s16 PL_id) {
    u16 sw;
    u16 buff;

    if (&Replay_w.io_unit.key_buff[PL_id][7198] < Demo_Ptr[PL_id]) {
        Replay_Status[0] = 2;
        Replay_Status[1] = 2;

        if (Mode_Type == MODE_REPLAY) {
            cpExitTask(TASK_PAUSE);
            cpReadyTask(TASK_MENU, Menu_Task);
            task[TASK_MENU].r_no[0] = 13;
        }

        Demo_Time_Stop = 1;
        return;
    }

    if (Game_pause == 0x81) {
        return;
    }

    if (Demo_Timer[PL_id] == 0) {
        sw = *Demo_Ptr[PL_id];
        Demo_Ptr[PL_id]++;
        buff = sw;
        sw &= 0xFFF;
        Condense_Buff[PL_id] = sw;
        buff &= 0xF000;
        buff >>= 12;
        Demo_Timer[PL_id] = buff + 1;
    }

    if (plw[PL_id].wu.operator == 0) {
        if (PL_id) {
            p2sw_0 = 0;
        } else {
            p1sw_0 = 0;
        }
    } else if (PL_id) {
        p2sw_0 = Condense_Buff[PL_id];
    } else {
        p1sw_0 = Condense_Buff[PL_id];
    }

    Demo_Timer[PL_id]--;
}

s16 Check_SysDir_Page() {
    s16 ix;
    s16 count;

    if (Debug_w[52]) {
        return Debug_w[52] + 6;
    }

    for (count = 0, ix = 0; ix < 20; ix++) {
        if (save_w[1].PL_Color[0][ix]) {
            count++;
        }
    }

    count /= 5;

    if (count > 3) {
        count = 3;
    }

    return count + 6;
}

void Clear_Flash_Init(s16 level) {
    Synchro_No = 0;
    Flash_Synchro = 0;
    Synchro_Level = level;
}

s16 Clear_Flash_Sub() {
    switch (Synchro_No) {
    case 0:
        Flash_Synchro -= Synchro_Level;

        if (Flash_Synchro <= 0) {
            Synchro_No = 1;
            Flash_Synchro = 1;
        }

        break;

    case 1:
        Flash_Synchro += Synchro_Level;

        if (Flash_Synchro > 127) {
            Synchro_No = 0;
            Flash_Synchro = 127;
        }

        break;
    }

    return Flash_Synchro;
}

void Copy_Key_Disp_Work() {
    s16 ix;

    for (ix = 0; ix < 8; ix++) {
        Convert_Buff[1][0][ix] = save_w[1].Pad_Infor[0].Shot[ix];
        Convert_Buff[1][1][ix] = save_w[1].Pad_Infor[1].Shot[ix];
    }

    Convert_Buff[1][0][8] = save_w[1].Pad_Infor[0].Vibration;
    Convert_Buff[1][1][8] = save_w[1].Pad_Infor[1].Vibration;
}

s32 Check_Extra_Setting() {
    s16 ix;
    s16 page;

    for (page = 0; page < 4; page++) {
        for (ix = Ex_Page_Data[page]; ix < 8; ix++) {
            save_w[1].extra_option.contents[page][ix] = save_w[0].extra_option.contents[page][ix];
        }
    }

    for (page = 0; page < 4; page++) {
        for (ix = 0; ix < 4; ix++) {
            if (save_w[1].extra_option.contents[page][ix] != save_w[0].extra_option.contents[page][ix]) {
                return 1;
            }
        }
    }

    return 0;
}

void All_Clear_Random_ix() {
    Random_ix16 = 0;
    Random_ix32 = 0;
    Random_ix16_ex = 0;
    Random_ix32_ex = 0;
}

void All_Clear_Timer() {
    system_timer = 0;
    Game_timer = 0;
    players_timer = 0;
}

void All_Clear_ETC() {
    old_mes_no2 = 0;
    old_mes_no3 = 0;
    old_mes_no_pl = 0;
    mes_already = 0;
}

void Setup_Net_Random_ix() {
    u8 ix = 0;

    Random_ix16 = ix;
    Random_ix32 = ix;
    Random_ix16_ex = ix;
    Random_ix32_ex = ix;
}

s32 Request_Fade(u16 fade_code) {
    if (Fade_Flag == 0) {
        Fade_Flag = 1;
        Fade_R_No0 = Fade_R_No1 = 0;
        Fade_Number = fade_code;
        Forbid_Break = 1;
        fade_cont_init();
        return 1;
    }

    return 0;
}

s32 Check_Fade_Complete_SP() {
    fade_cont_main();
    return Fade_Flag ^ 1;
}

s32 Check_Fade_Complete() {
    if (Fade_Flag) {
        fade_cont_main();
        return 0;
    }

    Forbid_Break = 1;
    return 1;
}

s32 Check_Ranking(s16 PL_id) {
    Present_Data[PL_id].name[0] = 12;
    Present_Data[PL_id].name[1] = 10;
    Present_Data[PL_id].name[2] = 25;
    Present_Data[PL_id].player = Stock_My_char[PL_id];
    Present_Data[PL_id].player_color = Stock_Player_Color[PL_id];
    Present_Data[PL_id].score = Continue_Coin[PL_id] + Score[PL_id][0];
    Present_Data[PL_id].wins = Stock_Win_Record[PL_id];
    Present_Data[PL_id].cpu_grade = judge_final[PL_id]->vs_cpu_grade[12];
    Present_Data[PL_id].grade = Best_Grade[PL_id];

    if (Break_Com[PL_id][0]) {
        Present_Data[PL_id].all_clear = 1;
    } else {
        Present_Data[PL_id].all_clear = 0;
    }

    Rank_In[PL_id][0] = Check_Sort_Score(PL_id);

    if (Rank_In[PL_id][0] >= 0 && Rank_In[PL_id ^ 1][0] >= 0) {
        Check_Partners_Rank(0, PL_id);
    }

    Rank_In[PL_id][1] = Check_Sort_Wins(PL_id);

    if (Rank_In[PL_id][1] >= 0 && Rank_In[PL_id ^ 1][1] >= 0) {
        Check_Partners_Rank(1, PL_id);
    }

    Rank_In[PL_id][2] = Check_Sort_CPU_Grade(PL_id);

    if (Rank_In[PL_id][2]) {
        Rank_In[PL_id][2] = -1;
    } else {
        Rank_In[PL_id ^ 1][2] = -1;
    }

    Rank_In[PL_id][3] = Check_Sort_Grade(PL_id);

    if (Rank_In[PL_id][3]) {
        Rank_In[PL_id][3] = -1;
    } else {
        Rank_In[PL_id ^ 1][3] = -1;
    }

    if (Rank_In[PL_id][0] >= 0 || Rank_In[PL_id][1] >= 0 || Rank_In[PL_id][2] >= 0 || Rank_In[PL_id][3] >= 0) {
        return 1;
    }

    return 0;
}

void Check_Partners_Rank(s16 dir_step, s16 PL_id) {
    if (Rank_In[PL_id][dir_step] > Rank_In[PL_id ^ 1][dir_step]) {
        return;
    }

    Rank_In[PL_id ^ 1][dir_step]++;

    if (Rank_In[PL_id ^ 1][dir_step] > 4) {
        Rank_In[PL_id ^ 1][dir_step] = -1;
    }
}

s32 Check_Sort_Score(s16 PL_id) {
    s16 i;
    s16 j;

    for (i = 0; i < 5; i++) {
        if (Ranking_Data[i].score < Present_Data[PL_id].score) {
            for (j = 3; j >= i; j--) {
                Ranking_Data[j + 1] = Ranking_Data[j];
            }

            Ranking_Data[i] = Present_Data[PL_id];
            return i;
        }
    }

    return -1;
}

s32 Check_Sort_Wins(s16 PL_id) {
    s16 i;
    s16 j;

    for (i = 0; i < 5; i++) {
        if (Ranking_Data[i + 5].wins < Present_Data[PL_id].wins) {
            for (j = 3; j >= i; j--) {
                Ranking_Data[j + 6] = Ranking_Data[j + 5];
            }

            Ranking_Data[i + 5] = Present_Data[PL_id];
            return i;
        }
    }

    return -1;
}

s32 Check_Sort_CPU_Grade(s16 PL_id) {
    s16 i;
    s16 j;

    for (i = 0; i < 5; i++) {
        if (!Check_CPU_Grade_Score(PL_id, i)) {
            continue;
        }

        for (j = 3; j >= i; j--) {
            Ranking_Data[j + 11] = Ranking_Data[j + 10];
        }

        Ranking_Data[i + 10] = Present_Data[PL_id];
        return i;
    }

    return -1;
}

s32 Check_Sort_Grade(s16 PL_id) {
    s16 i;
    s16 j;

    for (i = 0; i < 5; i++) {
        if (!Check_Grade_Score(PL_id, i)) {
            continue;
        }

        for (j = 3; j >= i; j--) {
            Ranking_Data[j + 16] = Ranking_Data[j + 15];
        }

        Ranking_Data[i + 15] = Present_Data[PL_id];
        return i;
    }

    return -1;
}

s32 Check_CPU_Grade_Score(s16 PL_id, s16 i) {
    if (Ranking_Data[i + 10].cpu_grade > Present_Data[PL_id].cpu_grade) {
        return 0;
    }

    if (Ranking_Data[i + 10].cpu_grade < Present_Data[PL_id].cpu_grade) {
        return 1;
    }

    if (Ranking_Data[i + 10].score >= Present_Data[PL_id].score) {
        return 0;
    }

    return 1;
}

s32 Check_Grade_Score(s16 PL_id, s16 i) {
    if (Ranking_Data[i + 15].grade > Present_Data[PL_id].grade) {
        return 0;
    }

    if (Ranking_Data[i + 15].grade < Present_Data[PL_id].grade) {
        return 1;
    }

    if (Ranking_Data[i + 15].wins >= Present_Data[PL_id].wins) {
        return 0;
    }

    return 1;
}

void Disp_Digit16x24(u32 Score_Buff, s16 Disp_X, s16 Disp_Y, s16 Color) {
    s16 i;
    s16 j;
    s32 xx;
    s16 First_Digit;
    s16 Digit[8];

    s16 s6;
    s32 s5;
    s16 s4;

    if (Score_Buff == 0) {
        score16x24_put(Disp_X, Disp_Y, 15, 0);
    }

    for (i = 7, xx = 10000000, s6 = First_Digit = -1; i > 0; i--, s5 = xx /= 10) {
        Digit[i] = Score_Buff / xx;
        Score_Buff -= xx * Digit[i];

        if (First_Digit < 0 && Digit[i]) {
            First_Digit = i;
        }
    }

    Digit[0] = Score_Buff;
    i = Disp_X - (First_Digit * 2);

    for (j = First_Digit; j >= 0; j--, s4 = i += 2) {
        score16x24_put(i, Disp_Y, Color, Digit[j]);
    }
}

void Disp_Copyright() {
    s32 xres;

    switch (Country) {
    case 1:
    case 2:
    case 3:
    case 7:
    case 8:
        SSPutStrPro(1, 386, 208, 9, -1, "@CAPCOM CO., LTD. 1999, 2004 ALL RIGHTS RESERVED.");
        break;

    case 4:
    case 5:
    case 6:
        xres = SSPutStrPro(1, 386, 212, 9, -1, "@CAPCOM U.S.A., INC. 1999, 2004 ALL RIGHTS RESERVED.");
        SSPutStrPro(0, xres, 202, 9, -1, "@CAPCOM CO., LTD. 1999, 2004,");
        break;

    default:
        break;
    }
}

void Initialize_EM_Candidate(s16 PL_id) {
    s16 ix;
    s16 ok_urien = random_16();

    for (ix = 0; ix < 16; ix++) {
        Candidate_Buff[ix] = 0xFF;
    }

    Setup_Candidate_Buff(PL_id);

    for (ix = 0; ix < 8; ix++) {
        EM_Candidate[PL_id][0][ix] = Check_EM_Buff(ix, ok_urien);
        EM_Candidate[PL_id][1][ix] = Check_EM_Buff(ix, ok_urien);
    }

    EM_Candidate[PL_id][0][8] = Middle_Class_Boss_Data[My_char[PL_id]];
    EM_Candidate[PL_id][1][8] = Middle_Class_Boss_Data[My_char[PL_id]];

    if (My_char[PL_id] != 0) {
        EM_Candidate[PL_id][0][9] = 0;
        EM_Candidate[PL_id][1][9] = 0;
    } else {
        EM_Candidate[PL_id][0][9] = 1;
        EM_Candidate[PL_id][1][9] = 1;
    }
}

void Setup_Candidate_Buff(s16 PL_id) {
    s16 em;
    s16 ix;
    s16 s2;

    for (em = 0, s2 = ix = 1; ix <= 19; ix++) {
        if (My_char[PL_id] == 0 && ix == 1) {
            continue;
        }

        if (ix == My_char[PL_id]) {
            continue;
        }

        if (ix == 17) {
            continue;
        }

        if (ix == Middle_Class_Boss_Data[My_char[PL_id]]) {
            continue;
        }

        if (Break_Com[PL_id][ix]) {
            continue;
        }

        Candidate_Buff[em] = ix;
        em++;

        if (em >= 16) {
            break;
        }
    }
}

s16 Check_EM_Buff(s16 ix, s16 ok_urien) {
    s16 em;
    s16 Rnd = random_16();
    s16 Next;

    if (Check_EM_Sub(ix, ok_urien, Rnd)) {
        em = Candidate_Buff[Rnd];
        Candidate_Buff[Rnd] = 0xFF;
        return em;
    }

    Next = random_16() & 1;

    if (Next == 0) {
        Next = -1;
    }

    while (1) {
        if (Check_EM_Sub(ix, ok_urien, Rnd)) {
            em = Candidate_Buff[Rnd];
            Candidate_Buff[Rnd] = 0xFF;
            return em;
        }

        Rnd += Next;

        if (Rnd < 0) {
            Rnd = 15;
        }

        if (Rnd > 15) {
            Rnd = 0;
        }
    }
}

s32 Check_EM_Sub(s16 ix, s16 ok_urien, s16 Rnd) {
    s16 em;

    if (Candidate_Buff[Rnd] == 0xFF) {
        return 0;
    }

    em = Candidate_Buff[Rnd];

    switch (em) {
    case 2:
    case 11:
    case 6:
    case 8:
        if (ix < 4) {
            return 0;
        }

        return 1;

    case 14:
        if (ix < 6) {
            return 0;
        }

        return 1;

    case 13:
        if (ok_urien != 0 && ix < 4) {
            return 0;
        }

        return 1;

    default:
        return 1;
    }
}

void Check_Same_CPU(s16 PL_id) {
    s16 ix;
    s16 ok_urien;

    if (VS_Index[PL_id] >= 9) {
        return;
    }

    if (Last_My_char[PL_id] == My_char[PL_id]) {
        return;
    }

    ok_urien = random_16();

    for (ix = 0; ix < 16; ix++) {
        Candidate_Buff[ix] = 0xFF;
    }

    Setup_Candidate_Buff(PL_id);

    for (ix = VS_Index[PL_id]; ix < 8; ix++) {
        EM_Candidate[PL_id][0][ix] = Check_EM_Buff(ix, ok_urien);
        EM_Candidate[PL_id][1][ix] = Check_EM_Buff(ix, ok_urien);
    }

    EM_Candidate[PL_id][0][8] = Middle_Class_Boss_Data[My_char[PL_id]];
    EM_Candidate[PL_id][1][8] = Middle_Class_Boss_Data[My_char[PL_id]];
}

void All_Clear_Suicide() {
    s16 ix;

    for (ix = 0; ix < 8; ix++) {
        Suicide[ix] = 0;
    }

    for (ix = 0; ix < 4; ix++) {
        Menu_Suicide[ix] = 0;
    }
}

void Clear_Break_Com(s16 PL_id) {
    s16 x;

    for (x = 0; x <= 19; x++) {
        Break_Com[PL_id][x] = 0;
    }
}

s32 Flash_Violent(WORK_Other* /* unused */, s32 /* unused */) {
    return 1;
}
