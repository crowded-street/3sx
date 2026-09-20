/**
 * @file sys_sub_ranking.c
 * Ranking Table Entry
 *
 * Split out of sys_sub.c: building a player's ranking record at the end of a
 * game and inserting it into the four ranked tables.
 */

#include "sf33rd/Source/Game/system/sys_sub.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/game.h"
#include "sf33rd/Source/Game/screen/entry.h"
#include "sf33rd/Source/Game/screen/ranking.h"
#include "sf33rd/Source/Game/system/work_sys.h"

void Check_Partners_Rank(s16 dir_step, s16 PL_id);
s32 Check_Sort_Score(s16 PL_id);
s32 Check_Sort_Wins(s16 PL_id);
s32 Check_Sort_CPU_Grade(s16 PL_id);
s32 Check_Sort_Grade(s16 PL_id);
s32 Check_CPU_Grade_Score(s16 PL_id, s16 i);
s32 Check_Grade_Score(s16 PL_id, s16 i);

static void setup_present_ranking_data(s16 PL_id) {
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
}

static s32 ranked_in_any_table(s16 PL_id) {
    return Rank_In[PL_id][0] >= 0 || Rank_In[PL_id][1] >= 0 || Rank_In[PL_id][2] >= 0 || Rank_In[PL_id][3] >= 0;
}

s32 Check_Ranking(s16 PL_id) {
    setup_present_ranking_data(PL_id);

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

    if (ranked_in_any_table(PL_id)) {
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

/* Push the rows below `i` down one and put this player's entry in the gap. The
 * three ranking tables that use it differ only in where they start, and each
 * writes both of its offsets out at its own call site. */
static void insert_ranking_row(s16 PL_id, s16 i, s16 base, s16 next) {
    s16 j;

    for (j = 3; j >= i; j--) {
        Ranking_Data[j + next] = Ranking_Data[j + base];
    }

    Ranking_Data[i + base] = Present_Data[PL_id];
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

    for (i = 0; i < 5; i++) {
        if (Ranking_Data[i + 5].wins < Present_Data[PL_id].wins) {
            insert_ranking_row(PL_id, i, 5, 6);
            return i;
        }
    }

    return -1;
}

s32 Check_Sort_CPU_Grade(s16 PL_id) {
    s16 i;

    for (i = 0; i < 5; i++) {
        if (!Check_CPU_Grade_Score(PL_id, i)) {
            continue;
        }

        insert_ranking_row(PL_id, i, 10, 11);
        return i;
    }

    return -1;
}

s32 Check_Sort_Grade(s16 PL_id) {
    s16 i;

    for (i = 0; i < 5; i++) {
        if (!Check_Grade_Score(PL_id, i)) {
            continue;
        }

        insert_ranking_row(PL_id, i, 15, 16);
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
