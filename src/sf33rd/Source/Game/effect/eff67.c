/**
 * @file eff67.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff67.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"

typedef struct {
    s16 wait_time;
    s16 end_routine;
    void (*initialize)(WORK_Other_CONN* ewk);
    void (*move)(WORK_Other_CONN* ewk);
    void (*display)(WORK_Other_CONN* ewk);
} MovingConnection67;

static void initialize_character_connection_67(WORK_Other_CONN* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
}

static void initialize_animated_connection_67(WORK_Other_CONN* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.old_cgnum = ewk->wu.cg_number = 0;
    ewk->wu.cg_number++;
    ewk->wu.cg_number &= 0x7FFF;
    ewk->prio_reverse = 1;
}

static void display_character_connection_67(WORK_Other_CONN* ewk) {
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    sort_push_request4(&ewk->wu);
}

static void display_moving_animated_connection_67(WORK_Other_CONN* ewk) {
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    ewk->wu.cg_number++;
    ewk->wu.cg_number &= 0x7FFF;
    sort_push_request3(&ewk->wu);
}

static void move_left_connection_67(WORK_Other_CONN* ewk) {
    ewk->wu.xyz[0].disp.pos = ewk->wu.xyz[0].disp.pos - 10;
}

static void move_right_connection_67(WORK_Other_CONN* ewk) {
    ewk->wu.xyz[0].disp.pos = ewk->wu.xyz[0].disp.pos + 10;
}

static void update_moving_connection_67(WORK_Other_CONN* ewk, const MovingConnection67* config) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        config->initialize(ewk);
        break;

    case 1:
        if (!--ewk->wu.dir_timer) {
            ewk->wu.routine_no[1]++;
            ewk->wu.dir_timer = config->wait_time;
        }
        break;

    case 2:
        if (--ewk->wu.dir_timer) {
            config->move(ewk);
        } else {
            ewk->wu.routine_no[0] = config->end_routine;
        }
        break;
    }

    config->display(ewk);
}

static const MovingConnection67 left_connection_67 = {
    40, 3, initialize_character_connection_67, move_left_connection_67, display_character_connection_67
};

static const MovingConnection67 right_connection_67 = {
    39, 3, initialize_character_connection_67, move_right_connection_67, display_character_connection_67
};

static const MovingConnection67 animated_connection_67 = {
    40, 4, initialize_animated_connection_67, move_left_connection_67, display_moving_animated_connection_67
};

static void update_left_connection_67(WORK_Other_CONN* ewk) {
    update_moving_connection_67(ewk, &left_connection_67);
}

static void update_right_connection_67(WORK_Other_CONN* ewk) {
    update_moving_connection_67(ewk, &right_connection_67);
}

static void update_animated_connection_67(WORK_Other_CONN* ewk) {
    update_moving_connection_67(ewk, &animated_connection_67);
}

static void initialize_static_connection_67(WORK_Other_CONN* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    sort_push_request4(&ewk->wu);
}

static void display_static_connection_67(WORK_Other_CONN* ewk) {
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    sort_push_request4(&ewk->wu);
}

static void display_animated_connection_67(WORK_Other_CONN* ewk) {
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    sort_push_request3(&ewk->wu);
}

static void initialize_animated_static_connection_67(WORK_Other_CONN* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.old_cgnum = ewk->wu.cg_number = 0;
        ewk->wu.cg_number++;
        ewk->wu.cg_number &= 0x7FFF;
        break;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    ewk->wu.cg_number++;
    ewk->wu.cg_number &= 0x7FFF;
    sort_push_request3(&ewk->wu);
}

void effect_67_move(WORK_Other_CONN* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        update_left_connection_67(ewk);
        break;

    case 1:
        update_right_connection_67(ewk);
        break;

    case 2:
        update_animated_connection_67(ewk);
        break;

    case 3:
        display_static_connection_67(ewk);
        break;

    case 4:
        display_animated_connection_67(ewk);
        break;

    case 5:
        initialize_static_connection_67(ewk);
        break;

    case 6:
        initialize_animated_static_connection_67(ewk);
        break;
    }
}

typedef struct {
    u16 first;
    u16 second;
    u16 third;
} ConnectionCharacters67;

static const ConnectionCharacters67 connection_characters_67[] = {
    [0] = { 0x6F38, 0x6F43, 0 },  [1] = { 0x6F38, 0x6F43, 0x6F44 },
    [2] = { 0x6F38, 0x6F43, 0x6F45 }, [3] = { 0x6F38, 0x6F42, 0 },
    [4] = { 0x6F38, 0x6F42, 0x6F44 }, [5] = { 0x6F38, 0x6F42, 0x6F45 },
    [6] = { 0x6F38, 0x6F41, 0 },  [7] = { 0x6F38, 0x6F41, 0x6F44 },
    [8] = { 0x6F38, 0x6F41, 0x6F45 }, [9] = { 0x6F37, 0x6F40, 0 },
    [10] = { 0x6F37, 0x6F40, 0x6F44 }, [11] = { 0x6F37, 0x6F40, 0x6F45 },
    [12] = { 0x6F37, 0x6F3F, 0 }, [13] = { 0x6F37, 0x6F3F, 0x6F44 },
    [14] = { 0x6F37, 0x6F3F, 0x6F45 }, [15] = { 0x6F36, 0x6F3E, 0 },
    [16] = { 0x6F36, 0x6F3E, 0x6F44 }, [17] = { 0x6F36, 0x6F3E, 0x6F45 },
    [18] = { 0x6F35, 0x6F3D, 0 }, [19] = { 0x6F35, 0x6F3D, 0x6F44 },
    [20] = { 0x6F35, 0x6F3D, 0x6F45 }, [21] = { 0x6F35, 0x6F3C, 0 },
    [22] = { 0x6F35, 0x6F3C, 0x6F44 }, [23] = { 0x6F35, 0x6F3C, 0x6F45 },
    [24] = { 0x6F34, 0x6F3B, 0 }, [25] = { 0x6F34, 0x6F3B, 0x6EE2 },
    [26] = { 0x6F34, 0x6F3B, 0x6EE1 }, [27] = { 0x6F33, 0x6F3A, 0 },
    [28] = { 0x6F33, 0x6F3A, 0x6EE3 }, [29] = { 0x6F33, 0x6F3A, 0x6EE4 },
    [30] = { 0x6F33, 0x6F3A, 0x6EE5 }, [31] = { 0x6F32, 0x6F39, 0 }
};

static void initialize_connection_slot_67(CONN* connection, u16 character) {
    connection->nx = 0;
    connection->ny = 0;
    connection->chr = character;
    connection->col = 0;
}

static void initialize_connections_67(WORK_Other_CONN* ewk, s16 char_index) {
    const ConnectionCharacters67* characters = &connection_characters_67[char_index];

    ewk->prio_reverse = 1;
    ewk->num_of_conn = 2;
    initialize_connection_slot_67(&ewk->conn[0], characters->first);
    initialize_connection_slot_67(&ewk->conn[1], characters->second);

    if (characters->third) {
        ewk->num_of_conn++;
        initialize_connection_slot_67(&ewk->conn[2], characters->third);
    }
}

static void initialize_character_67(WORK_Other_CONN* ewk, const Effect67Init* init) {
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->wu.char_index = init->id;
    ewk->wu.dir_step = init->char_index;
}

s32 effect_67_init_params(Effect67Init init) {
    WORK_Other_CONN* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 67;
    ewk->wu.work_id = 16;

    if (init.color) {
        ewk->wu.my_col_code = 0x90;
    } else {
        ewk->wu.my_col_code = 0x6A;
    }

    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.my_family = 1;
    ewk->wu.position_z = init.priority;
    ewk->wu.dir_timer = init.time;
    ewk->wu.routine_no[0] = init.routine;
    ewk->wu.routine_no[1] = 0;
    ewk->wu.xyz[0].disp.pos = init.x;
    ewk->wu.xyz[1].disp.pos = init.y;

    if (!init.id) {
        initialize_connections_67(ewk, init.char_index);
    } else {
        initialize_character_67(ewk, &init);
    }

    return 0;
}
