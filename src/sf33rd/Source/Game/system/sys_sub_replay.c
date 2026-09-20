/**
 * @file sys_sub_replay.c
 * Replay Recording and Playback
 *
 * Split out of sys_sub.c: setting up a replay to record or play back, and
 * stepping its key buffer each frame.
 */

#include "sf33rd/Source/Game/system/sys_sub.h"
#include "common.h"
#include "main.h"
#include "sf33rd/Source/Game/effect/effb8.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/game.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/menu/menu.h"

#include <SDL3/SDL.h>

#include <memory.h>

void Setup_Replay_Header();
void Get_Replay_Header();
void Get_Replay(s16 PL_id);
void Setup_Replay_Buff(s16 PL_id, u16 sw_buff);
void Replay(s16 PL_id);

static void start_replay_recording() {
    s16 ix;

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

    if (in_training_mode()) {
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
}

static void start_replay_playback() {
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
}

void Check_Replay() {
    if (!Demo_Flag) {
        return;
    }

    switch (Play_Mode) {
    case 1:
        start_replay_recording();
        break;

    case 3:
        start_replay_playback();
        break;

    default:
        return;
    }

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
        // [REPLAY AREA FULL!!]
        SDL_assert(false);
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
        return;
    }

    Demo_Timer[PL_id] = 1;
    Condense_Buff[PL_id] = sw_buff;
}

/* The end of a replay: the status both sides carry, the menu the replay mode
 * returns to, and the demo timer's stop. */
static void end_replay_playback() {
    Replay_Status[0] = 2;
    Replay_Status[1] = 2;

    if (Mode_Type == MODE_REPLAY) {
        cpExitTask(TASK_PAUSE);
        cpReadyTask(TASK_MENU, Menu_Task);
        task[TASK_MENU].r_no[0] = 13;
    }

    Demo_Time_Stop = 1;
}

/* The next condensed input word, and the frame count it is held for. */
static void pull_replay_input(s16 PL_id) {
    u16 sw;
    u16 buff;

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
}

/* Handing that input to the side it belongs to, or nothing where the side is
 * not under replay control. */
static void apply_replay_input(s16 PL_id) {
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
}

void Replay(s16 PL_id) {

    if (&Replay_w.io_unit.key_buff[PL_id][7198] < Demo_Ptr[PL_id]) {
        end_replay_playback();
        return;
    }

    if (Game_pause == 0x81) {
        return;
    }

    pull_replay_input(PL_id);

    apply_replay_input(PL_id);

    Demo_Timer[PL_id]--;
}
