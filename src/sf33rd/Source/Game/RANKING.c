#include "sf33rd/Source/Game/RANKING.h"
#include "common.h"
#include "sf33rd/Source/Game/EFF58.h"
#include "sf33rd/Source/Game/EFF67.h"
#include "sf33rd/Source/Game/Eff76.h"
#include "sf33rd/Source/Game/GD3rd.h"
#include "sf33rd/Source/Game/Game.h"
#include "sf33rd/Source/Game/MMTMCNT.h"
#include "sf33rd/Source/Game/SE.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/SYS_sub2.h"
#include "sf33rd/Source/Game/Sound3rd.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/demo/demo02.h"

// sbss
RANK_DATA Present_Data[2];

// bss
RANK_DATA Ranking_Data[20];

// forward declaration
const RANK_DATA Score_Ranking_Table[20];

s32 Ranking() {
    void (*main_jmp_tbl[2])() = { Ranking_01, Ranking_00 };

    Ranking_X = 0;
    main_jmp_tbl[D_No[0]]();
    BG_Draw_System();
    if ((Check_Exit_Check() == 0) && (Debug_w[24] == -1)) {
        Ranking_X = 0;
    }
    return Ranking_X;
}

void Ranking_00() {
    void (*Ranking_00_tbl[6])() = { Ranking_00_1st, Ranking_00_2nd, Ranking_00_3rd,
                                    Ranking_00_4th, Ranking_00_5th, Ranking_00_6th };

    Ranking_X = 0;
    Ranking_00_tbl[D_No[1]]();
}

void Ranking_00_1st() {
    make_texcash_work(14);
    D_No[1]++;
    Demo_Type = 0;
    Flash_Sign[0] = 1;
    Ranking_Sub();
    setup_pos_remake_key(4);
}

void Ranking_00_2nd() {
    s16 Char_Index;

    D_No[1]++;
    D_Timer = 1;
    Rank_X = 0;
    Flash_Rank_Time = 0;
    Rank_Pos_X = bg_w.bgw[0].xy[0].disp.pos - 104;
    Rank_Pos_Y = bg_w.bgw[0].xy[1].disp.pos + 160;

    if (Rank_Type >= 10) {
        Order[85] = 3;
        Order_Timer[85] = 1;
        Order_Dir[85] = (u8)Rank_Type;
        effect_76_init(85);
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos + 128, bg_w.bgw[0].xy[1].disp.pos + 80, 180, 10, 35, 5, 0);
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos + 120, bg_w.bgw[0].xy[1].disp.pos + 40, 180, 13, 35, 5, 0);
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos - 144, bg_w.bgw[0].xy[1].disp.pos + 68, 180, 7, 30, 5, 0);
        base_y_pos = 40;
        switch (Rank_Type) {
        case 10:
            if (Ranking_Data[10].cpu_grade == -1) {
                Char_Index = 0;
            } else {
                Char_Index = Ranking_Data[10].cpu_grade;
            }
            break;
        case 15:
            if (Ranking_Data[15].grade == -1) {
                Char_Index = 0;
            } else {
                Char_Index = Ranking_Data[15].grade;
            }
            break;
        }

        effect_67_init(0, bg_w.bgw[0].xy[0].disp.pos - 136, bg_w.bgw[0].xy[1].disp.pos + 60, 180, Char_Index, 10, 6, 1);
        Rank_Pos_X = bg_w.bgw[0].xy[0].disp.pos - 72;
        Rank_Pos_Y = bg_w.bgw[0].xy[1].disp.pos + 120;
        Rank_Pos_X += 16;
        Rank_Pos_Y -= 48;
        Rank = Rank_Type;
        Rank_Pos_Y -= 1;
        Setup_Name(5);
        Rank_Pos_Y += 1;
        Rank_Pos_X += 16;
        Rank_Pos_Y += 1;
        Rank_Pos_Y += 256;
        Setup_Face(5);
        Rank_Pos_Y -= 256;
        Rank_Pos_Y -= 1;
        Rank_Pos_X -= 32;
        Rank_Pos_Y -= 40;
        if (Rank_Type == 10) {
            Rank_Pos_X -= 80;
            Setup_Score(5);
        } else {
            Rank_Pos_X -= 32;
            Setup_Wins2(5);
        }
    } else {
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos - 143, bg_w.bgw[0].xy[1].disp.pos + 32, 180, 4, 30, 5, 0);
        for (Rank = Rank_Type; Rank < (Rank_Type + 5); Rank++) {
            if ((Present_Rank[0] == (Rank - Rank_Type)) || (Present_Rank[1] == (Rank - Rank_Type))) {
                Flash_Rank_Interval = 1;
            } else {
                Flash_Rank_Interval = 0;
            }

            Setup_Name(5);
            if (Rank_Type == 0) {
                Setup_Score(5);
                Rank_Pos_X += 3;
                Setup_grade(5);
                Rank_Pos_X -= 3;
            } else {
                Setup_Wins(5);
                Setup_grade(5);
            }

            Setup_Face(5);
            Rank_Pos_X = bg_w.bgw[0].xy[0].disp.pos - 104;
            Rank_Pos_Y -= 32;
            Rank_X = 0;
            Flash_Rank_Time = 0;
        }

        if ((Present_Rank[0] < 5) || (Present_Rank[1] < 5)) {
            D_No[1] += 1;
        } else {
            D_No[1] = 4;
        }
    }

    switch (Rank_Type) {
    case 0:
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos + 0, bg_w.bgw[0].xy[1].disp.pos + 200, 180, 0, 10, 5, 0);
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos + 168, bg_w.bgw[0].xy[1].disp.pos + 32, 180, 5, 20, 5, 0);
        return;
    case 5:
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos + 0, bg_w.bgw[0].xy[1].disp.pos + 200, 180, 2, 10, 5, 0);
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos + 168, bg_w.bgw[0].xy[1].disp.pos + 32, 180, 5, 20, 5, 0);
        return;
    case 10:
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos, bg_w.bgw[0].xy[1].disp.pos + 200, 180, 1, 10, 5, 0);
        return;
    case 15:
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos, bg_w.bgw[0].xy[1].disp.pos + 200, 180, 3, 10, 5, 0);
    }
}

void Ranking_00_3rd() {
    if (Flash_Sign[0] == 1) {
        D_No[1]++;
        D_Timer = 1;
    }
}

void Ranking_00_4th() {
    if (!(--D_Timer)) {
        D_No[1]++;
        D_Timer = 30;
        Flash_Sign[0] = 0;
        Flash_Sign[1] = 1;
    }
}

void Ranking_00_5th() {
    if (--D_Timer == 0) {
        D_No[1]++;
        D_Timer = 300;
        Flash_Sign[1] = 0;
    }
}

void Ranking_00_6th() {
    if (--D_Timer == 0) {
        D_Timer = 1;
        Ranking_X = 1;
        return;
    }

    if (D_Timer == 60) {
        SsBgmFadeOut(546);
    }
}

void Ranking_01() {
    void (*Ranking_01_tbl[5])() = { Ranking_01_1st, Ranking_01_2nd, Ranking_00_3rd, Ranking_01_4th, Ranking_01_5th };

    Ranking_01_tbl[D_No[1]]();
}

void Ranking_01_1st() {
    Switch_Screen(1);
    BGM_Request(57);
    Purge_mmtm_area(4);
    Make_texcash_of_list(2);
    make_texcash_work(14);
    D_No[1]++;
    Suicide[0] = 0;
    Present_Rank[0] = 99;
    Present_Rank[1] = 99;
    Ranking_Sub();
    effect_58_init(1, 1, -1);
    setup_pos_remake_key(4);
}

void Ranking_01_2nd() {
    s16 Char_Index;

    Switch_Screen(1);
    D_No[1]++;
    D_Timer = 420;
    Rank_X = 0;
    Flash_Rank_Time = 0;
    Rank_Pos_X = bg_w.bgw[0].xy[0].disp.pos - 104;
    Rank_Pos_Y = bg_w.bgw[0].xy[1].disp.pos + 160;
    Setup_Ranking_Obj();
    Setup_Score_Obj();
    if (Rank_Type == 0) {
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos + 168, bg_w.bgw[0].xy[1].disp.pos + 32, 180, 5, 20, 0, 0);
    } else {
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos + 168, bg_w.bgw[0].xy[1].disp.pos + 32, 180, 5, 20, 0, 0);
    }

    for (Rank = Rank_Type; Rank < (Rank_Type + 5); Rank++) {
        if ((Present_Rank[0] == (Rank - Rank_Type)) || (Present_Rank[1] == (Rank - Rank_Type))) {
            Flash_Rank_Interval = 1;
        } else {
            Flash_Rank_Interval = 0;
        }

        Setup_Name(0);
        if (Rank_Type == 0) {
            Setup_Score(0);
            Rank_Pos_X += 4;
            Setup_grade(0);
            Rank_Pos_X -= 4;
        } else {
            Setup_Wins(0);
            Setup_grade(0);
        }

        Setup_Face(0);
        Rank_Pos_X = bg_w.bgw[0].xy[0].disp.pos - 104;
        Rank_Pos_Y -= 32;
        Rank_X = 0;
        Flash_Rank_Time = 0;
    }
    Order[85] = 1;
    Order_Timer[85] = 180;
    Order_Dir[85] = Rank_Type + 10;
    effect_76_init(85);
    effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos + 512, bg_w.bgw[0].xy[1].disp.pos + 80, 180, 10, 35, 0, 0);
    effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos + 504, bg_w.bgw[0].xy[1].disp.pos + 40, 180, 13, 35, 0, 0);
    effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos + 240, bg_w.bgw[0].xy[1].disp.pos + 68, 180, 7, 30, 0, 0);
    base_y_pos = 40;
    switch (Rank_Type) {
    case 0:
        if (Ranking_Data[10].cpu_grade == -1) {
            Char_Index = 0;
        } else {
            Char_Index = Ranking_Data[10].cpu_grade;
        }
        break;
    case 5:
        if (Ranking_Data[15].grade == -1) {
            Char_Index = 0;
        } else {
            Char_Index = Ranking_Data[15].grade;
        }
        break;
    }
    effect_67_init(0, bg_w.bgw[0].xy[0].disp.pos + 248, bg_w.bgw[0].xy[1].disp.pos + 60, 180, Char_Index, 10, 2, 1);
    Rank_Pos_X = bg_w.bgw[0].xy[0].disp.pos + 312;
    Rank_Pos_Y = bg_w.bgw[0].xy[1].disp.pos + 120;
    Rank_Pos_X += 16;
    Rank_Pos_Y -= 48;
    Rank = (Rank_Type + 10);
    Rank_Pos_Y -= 1;
    Setup_Name(0);
    Rank_Pos_Y += 1;
    Rank_Pos_X += 16;
    Rank_Pos_Y += 1;
    Rank_Pos_Y += 256;
    Setup_Face(0);
    Rank_Pos_Y -= 256;
    Rank_Pos_Y -= 1;
    Rank_Pos_X -= 32;
    Rank_Pos_Y -= 40;
    if (Rank_Type == 0) {
        Rank_Pos_X -= 80;
        Setup_Score(0);
    } else {
        Rank_Pos_X -= 32;
        Setup_Wins2(0);
    }

    Rank -= 10;
    if ((Present_Rank[0] < 5) || (Present_Rank[1] < 5)) {
        D_No[1]++;
    } else {
        D_No[1] = 4;
    }

    if (G_No[1] < 6) {
        Setup_Demo_PL();
        Setup_Demo_Arts();
        Setup_Demo_Stage();
        Push_LDREQ_Queue_Player(0, My_char[0]);
        Push_LDREQ_Queue_Player(1, My_char[1]);
        Push_LDREQ_Queue_BG((s32)bg_w.stage);
    }
}

void Ranking_01_4th() {
    if (!(--D_Timer)) {
        D_No[1]++;
        D_Timer = 240;
    }
}

void Ranking_01_5th() {
    switch (D_No[2]) {
    case 0:
        if (!(--D_Timer)) {
            D_No[2] += 1;
            if ((Demo_Flag == 0) && (Demo_Type == 0)) {
                Clear_Personal_Data(0);
                Clear_Personal_Data(1);
            }

            Switch_Screen_Init(0);
            SsBgmFadeOut(1638);
        }
        break;

    case 1:
        if (Switch_Screen(0) != 0) {
            D_No[2] += 1;
            Game01_Sub();
            Cover_Timer = 24;
            return;
        }
        break;

    default:
        Switch_Screen(1);
        Ranking_X = 1;
        break;
    }
}

void Ranking_Sub() {
    System_all_clear_Level_B();
    if (Rank_Type == 0) {
        bg_etc_write(1);
    } else {
        bg_etc_write(6);
    }

    bg_pos_hosei2();
    Bg_Family_Set();
}

void Setup_grade(s16 y) {
    s16 Char_Index;

    switch (Rank_Type) {
    case 0:
        if (Ranking_Data[Rank].cpu_grade == -1) {
            Char_Index = 0;
        } else {
            Char_Index = Ranking_Data[Rank].cpu_grade;
        }
        break;

    case 5:
        if (Ranking_Data[Rank].grade == -1) {
            Char_Index = 0;
        } else {
            Char_Index = Ranking_Data[Rank].grade;
        }
        /* fallthrough */
    case 10:
        if (Ranking_Data[Rank].cpu_grade == -1) {
            Char_Index = 0;
        } else {
            Char_Index = Ranking_Data[Rank].cpu_grade;
        }
        /* fallthrough */
    case 15:
        if (Ranking_Data[Rank].grade == -1) {
            Char_Index = 0;
        } else {
            Char_Index = Ranking_Data[Rank].grade;
        }
        break;
    }

    effect_67_init(26, Rank_Pos_X, Rank_Pos_Y, 180, Char_Index + 98, 10, y, 0);
    if ((Rank_Type) == 0) {
        Rank_Pos_X += 16;
        if (Ranking_Data[Rank].all_clear) {
            Rank_Pos_Y += 16;
            effect_67_init(26, Rank_Pos_X - 4, Rank_Pos_Y + 3, 180, 74, 10, y, 0);
            Rank_Pos_Y -= 16;
        }

        Rank_Pos_X -= 16;
    }

    Rank_Pos_X += 48;
}

void Setup_Name(s16 y) {
    Name_Sub(0, y);
    Name_Sub(1, y);
    Name_Sub(2, y);

    if (Rank_Type != 0) {
        Rank_Pos_X += 16;
    }

    Rank_X += 3;
}

void Name_Sub(s16 xx, s16 y) {
    effect_67_init(26, Rank_Pos_X, Rank_Pos_Y, 180, Ranking_Data[Rank].name[xx], 10, y, 0);
    Flash_Rank_Time += Flash_Rank_Interval;
    Rank_Pos_X += 24;
}

void Setup_Face(s16 y) {
    Rank_Pos_Y += 16;
    effect_67_init(26, Rank_Pos_X, Rank_Pos_Y, 180, Ranking_Data[Rank].player + 47, 10, y, 1);
    Flash_Rank_Time += Flash_Rank_Interval;
    Rank_Pos_X += 40;
    Rank_Pos_Y -= 16;
    Rank_X += 2;
    Flash_Rank_Time += Flash_Rank_Interval;
}

void Setup_Score(s16 y) {
    s16 i;
    s16 First_Digit;
    u32 xx;
    u32 Score_Buff;
    s16 Digit[8];

    u32 assign1;
    u32 assign2;
    u32 assign3;
    u32 assign4;

    Score_Buff = Ranking_Data[Rank].score;
    First_Digit = -1;
    for (i = 7, assign1 = xx = 10000000; i > 0; i--, assign2 = xx /= 10) {
        Digit[i] = (Score_Buff / xx);
        Score_Buff -= xx * Digit[i];
        if ((First_Digit < 0) && (Digit[i])) {
            First_Digit = i;
        }
    }

    Digit[0] = Score_Buff;
    if (First_Digit < 0) {
        First_Digit = 0;
    }

    for (i = 0, assign3 = xx = 7; i < 8; i++, assign4 = xx--) {
        Flash_Rank_Time += Flash_Rank_Interval;
        Rank_Pos_X += 16;
        if (First_Digit >= xx) {
            effect_67_init(26, Rank_Pos_X, Rank_Pos_Y, 180, Digit[xx] + 75, 10, y, 0);
        }
    }

    Rank_Pos_X += 24;
}

void Setup_Wins(s16 y) {
    u32 Score_Buff;
    s16 Digit[3];

    Score_Buff = Ranking_Data[Rank].wins;
    Digit[2] = Score_Buff / 100;
    Score_Buff -= Digit[2] * 100;
    Digit[1] = Score_Buff / 10;
    Digit[0] = (Score_Buff -= (Digit[1] * 10));

    if (Digit[2]) {
        effect_67_init(26, Rank_Pos_X, Rank_Pos_Y, 180, Digit[2] + 130, 10, y, 0);
        Rank_Pos_X += 16;
        effect_67_init(26, Rank_Pos_X + 3, Rank_Pos_Y, 180, Digit[1] + 130, 10, y, 0);
        Rank_Pos_X += 16;
    } else {
        Rank_Pos_X += 16;
        if (Digit[1]) {
            effect_67_init(26, Rank_Pos_X + 3, Rank_Pos_Y, 180, Digit[1] + 130, 10, y, 0);
        }

        Rank_Pos_X += 16;
    }

    Rank_X += 1;
    Flash_Rank_Time += Flash_Rank_Interval;
    effect_67_init(26, Rank_Pos_X + 6, Rank_Pos_Y, 180, Digit[0] + 130, 10, y, 0);
    Rank_Pos_X += 40;
    Rank_X += 1;
    Flash_Rank_Time += Flash_Rank_Interval;
    effect_67_init(24, Rank_Pos_X, Rank_Pos_Y, 180, 6, 10, y, 0);
    Rank_Pos_X += 64;
    Rank_X += 1;
}

void Setup_Wins2(s16 y) {
    u32 Score_Buff;
    s16 Digit[3];

    Score_Buff = Ranking_Data[Rank].wins;
    Digit[2] = Score_Buff / 100;
    Score_Buff -= Digit[2] * 100;
    Digit[1] = Score_Buff / 10;
    Digit[0] = (Score_Buff -= (Digit[1] * 10));

    if (Digit[2]) {
        effect_67_init(26, Rank_Pos_X, Rank_Pos_Y, 180, Digit[2] + 75, 10, y, 0);
        Rank_Pos_X += 16;
        effect_67_init(26, Rank_Pos_X, Rank_Pos_Y, 180, Digit[1] + 75, 10, y, 0);
        Rank_Pos_X += 16;
    } else {
        Rank_Pos_X += 16;
        if (Digit[1]) {
            effect_67_init(26, Rank_Pos_X, Rank_Pos_Y, 180, Digit[1] + 75, 10, y, 0);
        }

        Rank_Pos_X += 16;
    }

    Rank_X += 1;
    Flash_Rank_Time += Flash_Rank_Interval;
    effect_67_init(26, Rank_Pos_X, Rank_Pos_Y, 180, Digit[0] + 75, 10, y, 0);
    Rank_Pos_X += 40;
    effect_67_init(24, Rank_Pos_X, Rank_Pos_Y + 8, 180, 11, 10, y, 0);
    Rank_X += 1;
    Flash_Rank_Time += Flash_Rank_Interval;
}

void Setup_Ranking_Obj() {
    effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos - 143, bg_w.bgw[0].xy[1].disp.pos + 32, 180, 4, 30, 0, 0);
}

void Setup_Score_Obj() {
    if (Rank_Type == 0) {
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos - 0, bg_w.bgw[0].xy[1].disp.pos + 200, 180, 0, 10, 1, 0);
        effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos - 384, bg_w.bgw[0].xy[1].disp.pos + 200, 180, 1, 10, 1, 0);
        return;
    }

    effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos - 0, bg_w.bgw[0].xy[1].disp.pos + 200, 180, 2, 10, 1, 0);
    effect_67_init(24, bg_w.bgw[0].xy[0].disp.pos - 384, bg_w.bgw[0].xy[1].disp.pos + 200, 180, 3, 10, 1, 0);
}

void Ranking_Init() {
    s16 ix;
    s16 ix2;

    for (ix = 0; ix < 4; ix++) {
        for (ix2 = 0; ix2 < 20; ix2++) {
            save_w[ix].Ranking[ix2] = Score_Ranking_Table[ix2];
        }
    }
}

const RANK_DATA Score_Ranking_Table[20] = { { .name = { 28, 13, 22 },
                                              .player = 15,
                                              .score = 100000,
                                              .cpu_grade = 10,
                                              .grade = 0,
                                              .wins = 10,
                                              .player_color = 0,
                                              .all_clear = 1 },
                                            { .name = { 22, 24, 24 },
                                              .player = 16,
                                              .score = 90000,
                                              .cpu_grade = 9,
                                              .grade = 0,
                                              .wins = 9,
                                              .player_color = 1,
                                              .all_clear = 0 },
                                            { .name = { 23, 14, 24 },
                                              .player = 17,
                                              .score = 80000,
                                              .cpu_grade = 8,
                                              .grade = 0,
                                              .wins = 8,
                                              .player_color = 2,
                                              .all_clear = 1 },
                                            { .name = { 24, 23, 30 },
                                              .player = 18,
                                              .score = 70000,
                                              .cpu_grade = 7,
                                              .grade = 0,
                                              .wins = 7,
                                              .player_color = 3,
                                              .all_clear = 0 },
                                            { .name = { 34, 24, 20 },
                                              .player = 19,
                                              .score = 60000,
                                              .cpu_grade = 6,
                                              .grade = 0,
                                              .wins = 6,
                                              .player_color = 4,
                                              .all_clear = 0 },
                                            { .name = { 20, 10, 18 },
                                              .player = 15,
                                              .score = 100000,
                                              .cpu_grade = 10,
                                              .grade = 10,
                                              .wins = 10,
                                              .player_color = 0,
                                              .all_clear = 1 },
                                            { .name = { 21, 29, 1 },
                                              .player = 16,
                                              .score = 90000,
                                              .cpu_grade = 9,
                                              .grade = 9,
                                              .wins = 9,
                                              .player_color = 1,
                                              .all_clear = 0 },
                                            { .name = { 34, 30, 20 },
                                              .player = 17,
                                              .score = 80000,
                                              .cpu_grade = 8,
                                              .grade = 8,
                                              .wins = 8,
                                              .player_color = 2,
                                              .all_clear = 1 },
                                            { .name = { 27, 10, 24 },
                                              .player = 18,
                                              .score = 70000,
                                              .cpu_grade = 7,
                                              .grade = 7,
                                              .wins = 7,
                                              .player_color = 3,
                                              .all_clear = 0 },
                                            { .name = { 0, 0, 8 },
                                              .player = 19,
                                              .score = 60000,
                                              .cpu_grade = 6,
                                              .grade = 6,
                                              .wins = 6,
                                              .player_color = 4,
                                              .all_clear = 0 },
                                            { .name = { 18, 12, 17 },
                                              .player = 15,
                                              .score = 100000,
                                              .cpu_grade = 11,
                                              .grade = 0,
                                              .wins = 10,
                                              .player_color = 0,
                                              .all_clear = 1 },
                                            { .name = { 28, 14, 29 },
                                              .player = 5,
                                              .score = 90000,
                                              .cpu_grade = 9,
                                              .grade = 0,
                                              .wins = 9,
                                              .player_color = 1,
                                              .all_clear = 0 },
                                            { .name = { 23, 14, 24 },
                                              .player = 4,
                                              .score = 80000,
                                              .cpu_grade = 8,
                                              .grade = 0,
                                              .wins = 8,
                                              .player_color = 2,
                                              .all_clear = 1 },
                                            { .name = { 24, 23, 30 },
                                              .player = 10,
                                              .score = 70000,
                                              .cpu_grade = 7,
                                              .grade = 0,
                                              .wins = 7,
                                              .player_color = 3,
                                              .all_clear = 0 },
                                            { .name = { 34, 24, 20 },
                                              .player = 8,
                                              .score = 60000,
                                              .cpu_grade = 6,
                                              .grade = 0,
                                              .wins = 6,
                                              .player_color = 4,
                                              .all_clear = 0 },
                                            { .name = { 18, 23, 14 },
                                              .player = 19,
                                              .score = 100000,
                                              .cpu_grade = 11,
                                              .grade = 11,
                                              .wins = 10,
                                              .player_color = 0,
                                              .all_clear = 1 },
                                            { .name = { 21, 29, 1 },
                                              .player = 1,
                                              .score = 90000,
                                              .cpu_grade = 9,
                                              .grade = 9,
                                              .wins = 9,
                                              .player_color = 1,
                                              .all_clear = 0 },
                                            { .name = { 34, 30, 20 },
                                              .player = 4,
                                              .score = 80000,
                                              .cpu_grade = 8,
                                              .grade = 8,
                                              .wins = 8,
                                              .player_color = 2,
                                              .all_clear = 1 },
                                            { .name = { 27, 10, 24 },
                                              .player = 10,
                                              .score = 70000,
                                              .cpu_grade = 7,
                                              .grade = 7,
                                              .wins = 7,
                                              .player_color = 3,
                                              .all_clear = 0 },
                                            { .name = { 18, 23, 14 },
                                              .player = 8,
                                              .score = 60000,
                                              .cpu_grade = 6,
                                              .grade = 6,
                                              .wins = 6,
                                              .player_color = 4,
                                              .all_clear = 0 } };
