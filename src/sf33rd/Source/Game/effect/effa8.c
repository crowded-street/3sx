/**
 * @file effa8.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effa8.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/texcash.h"

void Setup_A8_Sub(WORK_Other_CONN* ewk);

const s16 Pos_Data_A8[4][2] = { { 52, 81 }, { 52, 68 }, { 0, 52 }, { 192, 32 } };

const s8* Letter_Data_A8DC[69] = { "-PLAY(  )",
                                   " STOP(  ) BUTTON-",
                                   "1. Alex & Ken Stage -JAZZY NYC '99 mix1-",
                                   "2. Alex & Ken Stage -JAZZY NYC '99 mix2-",
                                   "3. Alex & Ken Stage -JAZZY NYC '99 mix3-",
                                   "4. Necro & Twelve Stage -SNOWLAND mix1-",
                                   "5. Necro & Twelve Stage -SNOWLAND mix2-",
                                   "6. Necro & Twelve Stage -SNOWLAND mix3-",
                                   "7. Hugo Stage -THE CIRCUIT mix1-",
                                   "8. Hugo Stage -THE CIRCUIT mix2-",
                                   "9. Hugo Stage -THE CIRCUIT mix3-",
                                   "10. Chun-Li Stage -CHINA VOX mix1-",
                                   "11. Chun-Li Stage -CHINA VOX mix2-",
                                   "12. Chun-Li Stage -CHINA VOX mix3-",
                                   "13. Ryu Stage -KOBU mix1-",
                                   "14. Ryu Stage -KOBU mix2-",
                                   "15. Ryu Stage -KOBU mix3-",
                                   "16. Ibuki Stage -TWILIGHT mix1-",
                                   "17. Ibuki Stage -TWILIGHT mix2-",
                                   "18. Ibuki Stage -TWILIGHT mix3-",
                                   "19. Makoto Stage -SPUNKY mix1-",
                                   "20. Makoto Stage -SPUNKY mix2-",
                                   "21. Makoto Stage -SPUNKY mix3-",
                                   "22. Akuma Stage -KILLING MOON mix1-",
                                   "23. Akuma Stage -KILLING MOON mix2-",
                                   "24. Akuma Stage -KILLING MOON mix3-",
                                   "25. Elena Stage -BEATS IN MY HEAD mix1-",
                                   "26. Elena Stage -BEATS IN MY HEAD mix2-",
                                   "27. Elena Stage -BEATS IN MY HEAD mix3-",
                                   "28. Sean & Oro Stage -THE LONGSHOREMAN mix1-",
                                   "29. Sean & Oro Stage -THE LONGSHOREMAN mix2-",
                                   "30. Sean & Oro Stage -THE LONGSHOREMAN mix3-",
                                   "31. Dudley Stage -YOU BLOW MY MIND mix1-",
                                   "32. Dudley Stage -YOU BLOW MY MIND mix2-",
                                   "33. Dudley Stage -YOU BLOW MY MIND mix3-",
                                   "34. Yun & Yang Stage -CROWDED STREET mix1-",
                                   "35. Yun & Yang Stage -CROWDED STREET mix2-",
                                   "36. Yun & Yang Stage -CROWDED STREET mix3-",
                                   "37. Remy Stage -THE BEEP mix1-",
                                   "38. Remy Stage -THE BEEP mix2-",
                                   "39. Remy Stage -THE BEEP mix3-",
                                   "40. The Theme Of Q -Q mix1-",
                                   "41. The Theme Of Q -Q mix2-",
                                   "42. The Theme Of Q -Q mix3-",
                                   "43. Urien Stage -CRAZY CHILI DOG mix1-",
                                   "44. Urien Stage -CRAZY CHILI DOG mix2-",
                                   "45. Urien Stage -CRAZY CHILI DOG mix3-",
                                   "46. Gill Stage -PSYCH OUT mix1-",
                                   "47. Gill Stage -PSYCH OUT mix2-",
                                   "48. Gill Stage -PSYCH OUT mix3-",
                                   "49. Ending1",
                                   "50. Ending2",
                                   "51. Just Before The Battle",
                                   "52. Opening Demo -THIRD STRIKE OPENING MIX-",
                                   "53. Player Select -LET'S GET IT ON(ARCADE)-",
                                   "54. Final Results",
                                   "55. Stage Results",
                                   "56. Judgement",
                                   "57. Stage Select & Score Ranking",
                                   "58. Continue",
                                   "59. We Await Your Return,Warrior!",
                                   "60. Gill Appears!",
                                   "61. Bonus Game 1",
                                   "62. Bonus Game 2",
                                   "63. Staff Roll -Moving On-",
                                   "64. Staff Roll -THIRD STRIKE REMIX-",
                                   "65. Menu -THIRD STRIKE-",
                                   "66. Player Select -LET'S GET IT ON(VERSUS)-",
                                   "67. Capcom Logo" };

const s8* Letter_Data_A8AC[69] = { "-PLAY(  )",
                                   " STOP(  ) BUTTON-",
                                   "1. Alex & Ken Stage -JAZZY NYC '99 1-",
                                   "2. Alex & Ken Stage -JAZZY NYC '99 2-",
                                   "-. ",
                                   "3. Necro & Twelve Stage -SNOWLAND 1-",
                                   "4. Necro & Twelve Stage -SNOWLAND 2-",
                                   "-. ",
                                   "5. Hugo Stage -THE CIRCUIT 1-",
                                   "6. Hugo Stage -THE CIRCUIT 2-",
                                   "-. ",
                                   "7. Chun-Li Stage -CHINA VOX 1-",
                                   "8. Chun-Li Stage -CHINA VOX 2-",
                                   "-. ",
                                   "9. Ryu Stage -KOBU 1-",
                                   "10. Ryu Stage -KOBU 2-",
                                   "-. ",
                                   "11. Ibuki Stage -TWILIGHT 1-",
                                   "12. Ibuki Stage -TWILIGHT 2-",
                                   "-. ",
                                   "13. Makoto Stage -SPUNKY 1-",
                                   "14. Makoto Stage -SPUNKY 2-",
                                   "-. ",
                                   "15. Akuma Stage -KILLING MOON 1-",
                                   "16. Akuma Stage -KILLING MOON 2-",
                                   "-. ",
                                   "17. Elena Stage -BEATS IN MY HEAD 1-",
                                   "18. Elena Stage -BEATS IN MY HEAD 2-",
                                   "-. ",
                                   "19. Sean & Oro Stage -THE LONGSHOREMAN 1-",
                                   "20. Sean & Oro Stage -THE LONGSHOREMAN 2-",
                                   "-. ",
                                   "21. Dudley Stage -YOU BLOW MY MIND 1-",
                                   "22. Dudley Stage -YOU BLOW MY MIND 2-",
                                   "-. ",
                                   "23. Yun & Yang Stage -CROWDED STREET 1-",
                                   "24. Yun & Yang Stage -CROWDED STREET 2-",
                                   "-. ",
                                   "25. Remy Stage -THE BEEP 1-",
                                   "26. Remy Stage -THE BEEP 2-",
                                   "-. ",
                                   "27. The Theme Of Q -Q 1-",
                                   "28. The Theme Of Q -Q 2-",
                                   "-. ",
                                   "29. Urien Stage -CRAZY CHILI DOG 1-",
                                   "30. Urien Stage -CRAZY CHILI DOG 2-",
                                   "-. ",
                                   "31. Gill Stage -PSYCH OUT 1-",
                                   "32. Gill Stage -PSYCH OUT 2-",
                                   "-. ",
                                   "33. Ending1",
                                   "34. Ending2",
                                   "35. Just Before The Battle",
                                   "36. Opening Demo -THIRD STRIKE OPENING MIX-",
                                   "37. Player Select -LET'S GET IT ON(ARCADE)-",
                                   "38. Final Results",
                                   "39. Stage Results",
                                   "40. Judgement",
                                   "41. Stage Select & Score Ranking",
                                   "42. Continue",
                                   "43. We Await Your Return,Warrior!",
                                   "44. Gill Appears!",
                                   "45. Bonus Game 1",
                                   "46. Bonus Game 2",
                                   "47. Staff Roll -Moving On-",
                                   "48. Staff Roll -THIRD STRIKE REMIX-",
                                   "49. Menu -THIRD STRIKE-",
                                   "50. Player Select -LET'S GET IT ON(VERSUS)-",
                                   "51. Capcom Logo" };

const s8** Letter_Data_A8[2] = { Letter_Data_A8DC, Letter_Data_A8AC };

void effect_A8_move(WORK_Other_CONN* ewk) {
    s16 ix;

    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    if (Menu_Cursor_Y[0] == ewk->wu.type) {
        ewk->wu.disp_flag = 1;
    } else {
        ewk->wu.disp_flag = 0;
    }

    switch (ewk->master_id) {
    case 2:
        ewk->wu.my_clear_level = Flash_Synchro;
        Setup_A8_Sub(ewk);
        ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Pos_Data_A8[ewk->wu.dir_step][0];
        ix = ewk->num_of_conn - 1;
        ix = ewk->conn[ix].nx;
        ewk->wu.position_x -= ix / 2;
        sort_push_request3(&ewk->wu);
        break;

    case 0:
        Setup_A8_Sub(ewk);
        sort_push_request3(&ewk->wu);

        if (ewk->wu.disp_flag) {
            if (Order_Dir[ewk->wu.dir_old]) {
                dispButtonImage(-87, 25, 68, 22, 17, 0, 4);
            } else {
                dispButtonImage(-23, 0, 68, 22, 17, 0, 5);
            }
        }

        break;

    case 3:
        sort_push_requestA(&ewk->wu);
        break;
    }
}

s32 effect_A8_init(s16 id, u8 dir_old, s16 sync_bg, s16 master_player, s16 cursor_index, s16 char_ix, s16 pos_index) {
    WORK_Other_CONN* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 108;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x1AC;
    ewk->master_id = id;
    ewk->wu.dir_old = dir_old;
    ewk->wu.my_family = sync_bg + 1;
    ewk->master_player = master_player;
    ewk->wu.type = cursor_index;
    ewk->wu.old_cgnum = char_ix;
    ewk->wu.dir_step = pos_index;
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Pos_Data_A8[ewk->wu.dir_step][0];
    ewk->wu.position_y = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Pos_Data_A8[ewk->wu.dir_step][1];
    ewk->wu.position_z = 68;

    if (ewk->master_id != 3) {
        Setup_A8_Sub(ewk);
    } else {
        ewk->wu.my_col_code = 0;
        ewk->wu.shell_ix[0] = -376;
        ewk->wu.shell_ix[1] = 400;
        ewk->wu.shell_ix[2] = 16;
        ewk->wu.shell_ix[3] = 14;
        ewk->wu.charset_id = 0;
        ewk->wu.my_clear_level = 128;
        ewk->wu.position_z = 69;
    }

    return 0;
}

void Setup_A8_Sub(WORK_Other_CONN* ewk) {
    s16 x;
    s16 ix;
    s16 offset_x;
    u8* ptr;

    if (ewk->wu.old_cgnum == 0x70A7) {
        offset_x = 8;
    } else {
        offset_x = 14;
    }

    ptr = (u8*)Letter_Data_A8[sys_w.bgm_type][Order_Dir[ewk->wu.dir_old] + ewk->master_id];
    ix = 0;
    x = 0;

    while (*ptr != '\0') {
        if (*ptr == ' ') {
            x += offset_x;
            ptr++;
            continue;
        }

        ewk->conn[ix].nx = x;
        ewk->conn[ix].ny = 0;
        ewk->conn[ix].col = 0;
        ewk->conn[ix].chr = ewk->wu.old_cgnum + *ptr;
        x += offset_x;
        ptr += 1;
        ix++;
    }

    ewk->num_of_conn = ix;
}
