/**
 * @file end_18.c
 * Q's Ending
 */

#include "common.h"
#include "sf33rd/Source/Game/EFFF9.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effe6.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/sound/se.h"

void end_1800_move();

void end_1800_0001();
void end_1800_0005();
void end_1800_0006();
void end_1800_0008();
void end_1800_0009();
void end_1800_0010();

const s16 timer_18_tbl[11] = { 900, 240, 240, 240, 240, 420, 720, 360, 480, 240, 300 };

const s16 end_18_pos[11][2] = { { 768, 768 }, { 256, 768 }, { 256, 768 }, { 256, 768 }, { 256, 768 }, { 768, 768 },
                                { 256, 512 }, { 256, 768 }, { 256, 512 }, { 256, 256 }, { 768, 512 } };

void end_18000(s16 pl_num) {
    switch (end_w.r_no_1) {
    case 0:
        end_w.r_no_1++;
        end_w.r_no_2 = 0;
        common_end_init00(pl_num);
        common_end_init01();
        BGM_Request(0x32);
        end_w.timer = timer_18_tbl[end_w.r_no_2];
        break;

    case 1:
        end_w.timer--;

        if (end_w.timer < 0) {
            end_w.r_no_2++;

            if (end_w.r_no_2 >= 11) {
                end_w.r_no_1++;
                end_w.end_flag = 1;
                fadeout_to_staff_roll();
                end_scn_pos_set2();
                end_bg_pos_hosei2();
                end_fam_set2();
                break;
            }

            end_w.timer = timer_18_tbl[end_w.r_no_2];
            bg_w.bgw[0].r_no_1 = 0;
            bg_w.bgw[1].r_no_1 = 0;
            bg_w.bgw[2].r_no_1 = 0;
        }

        end_1800_move();
        /* fallthrough */

    case 2:
        end_scn_pos_set2();
        end_bg_pos_hosei2();
        end_fam_set2();
        break;
    }
}

void end_1800_move() {
    void (*end_1800_move_jp[11])() = { end_1800_0005, end_1800_0001, end_1800_0001, end_1800_0001,
                                       end_1800_0001, end_1800_0005, end_1800_0006, end_1800_0001,
                                       end_1800_0008, end_1800_0009, end_1800_0010 };
    bgw_ptr = &bg_w.bgw[0];
    end_1800_move_jp[end_w.r_no_2]();
}

void end_1800_0001() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_18_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_18_pos[end_w.r_no_2][1];

        switch (end_w.r_no_2) {
        case 1:
            Bg_On_W(1);
            effect_E6_init(0x6F);
            effect_E6_init(0x74);
            bgw_ptr->l_limit = 0;
            bgw_ptr->free = 3;
            Rewrite_End_Message(2);
            break;

        case 2:
            effect_E6_init(0x70);
            break;

        case 3:
            effect_E6_init(0x71);
            break;

        case 4:
            effect_E6_init(0x72);
            break;

        case 7:
            effect_E6_init(0x77);
            effect_E6_init(0x7A);
            Rewrite_End_Message(4);
            bgw_ptr->l_limit = 0;
            bgw_ptr->free = 3;
            break;
        }

        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->free = 1;
            bgw_ptr->l_limit ^= 1;
        }

        break;
    }
}

void end_1800_0005() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_On_W(1);
        bgw_ptr->xy[0].disp.pos = end_18_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_18_pos[end_w.r_no_2][1];
        effect_E6_init(0x73);
        effect_E6_init(0x75);

        if (end_w.r_no_2) {
            Rewrite_End_Message(3);
        } else {
            Rewrite_End_Message(1);
        }

        bgw_ptr->l_limit = 0;
        bgw_ptr->free = 3;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->free = 2;
            bgw_ptr->l_limit ^= 1;
        }

        break;
    }
}

void end_1800_0006() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_18_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_18_pos[end_w.r_no_2][1];
        effect_E6_init(0x76);
        Rewrite_End_Message(6);
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->free = 1;
            bgw_ptr->l_limit ^= 1;
        }

        break;
    }
}

void end_1800_0008() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_18_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_18_pos[end_w.r_no_2][1];
        effect_E6_init(0x78);
        Rewrite_End_Message(5);
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->free = 1;
            bgw_ptr->l_limit ^= 1;
        }

        break;
    }
}

void end_1800_0009() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_18_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_18_pos[end_w.r_no_2][1];
        bgw_ptr->free = 0;
        Rewrite_End_Message(0);
        break;

    case 1:
        bgw_ptr->free++;
        bgw_ptr->free &= 1;

        if (bgw_ptr->free) {
            bgw_ptr->xy[0].disp.pos = end_18_pos[end_w.r_no_2][0];
            break;
        }

        bgw_ptr->xy[0].disp.pos = end_18_pos[end_w.r_no_2][0] + 512;
        break;
    }
}

void end_1800_0010() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bgw_ptr->xy[0].disp.pos = end_18_pos[end_w.r_no_2][0];
        bgw_ptr->xy[1].disp.pos = end_18_pos[end_w.r_no_2][1];
        effect_E6_init(0x79);
        effect_E6_init(0xA2);
        bgw_ptr->free = 1;
        bgw_ptr->l_limit = 0;
        end_fade_flag = 1;
        end_fade_timer = timer_18_tbl[end_w.r_no_2] - 120;
        break;

    case 1:
        bgw_ptr->free--;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->free = 1;
            bgw_ptr->l_limit++;
            bgw_ptr->l_limit &= 1;
        }

        break;
    }
}
