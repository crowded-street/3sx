#ifndef STRUCTS_H
#define STRUCTS_H

#include "sf33rd/AcrSDK/common/plcommon.h"
#include "types.h"
#include <libgraph.h>

typedef struct {
    // total size: 0x4
    s16 l; // offset 0x0, size 0x2
    s16 h; // offset 0x2, size 0x2
} LoHi16;

typedef union {
    s32 sp;      // offset 0x0, size 0x4
    LoHi16 real; // offset 0x0, size 0x4
} Reg32SpReal;

typedef union {
    s32 cal;    // offset 0x0, size 0x4
    LoHi16 pos; // offset 0x0, size 0x4
} Reg32CalPos;

typedef struct {
    // total size: 0x8
    u8* pFrame;  // offset 0x0, size 0x4
    s32 heapnum; // offset 0x4, size 0x4
} FMS_FRAME;

typedef struct {
    FMS_FRAME fmsFrame;
    u8* ramcntBuff;
    s8 ds_h[2];
    s8 ds_v[2];
    s8 sysStop;
    s8 initTrainingData;
    s8 inGame;
    s8 ctrDemo;
    s8 language;
    s8 langload;
    s8 cutAnalogStickData;
    s8 useAnalogStickData;
    s8 pal50Hz;
    u32 hanFbTex;
    u8 useChar[20];
} MPP;

struct _TASK {
    // total size: 0x14
    void (*func_adrs)();     // offset 0x0, size 0x4
    void (*callback_adrs)(); // offset 0x4, size 0x4
    u8 r_no[4];              // offset 0x8, size 0x4
    u16 condition;           // offset 0xC, size 0x2
    s16 timer;               // offset 0xE, size 0x2
    u8 free[4];              // offset 0x10, size 0x4
};

struct _disp {
    // total size: 0xC
    u16 size_x; // offset 0x0, size 0x2
    u16 size_y; // offset 0x2, size 0x2
    u16 now;    // offset 0x4, size 0x2
    u16 new;    // offset 0x6, size 0x2
    s32 cable;  // offset 0x8, size 0x4
};

struct _SYSTEM_W {
    // total size: 0x1C
    struct _disp disp; // offset 0x0, size 0xC
    s32 pause;         // offset 0xC, size 0x4
    s32 gd_error;      // offset 0x10, size 0x4
    s32 reset;         // offset 0x14, size 0x4
    u8 sound_mode;     // offset 0x18, size 0x1
    u8 screen_mode;    // offset 0x19, size 0x1

    /// Controls background music type (arranged vs original)
    /// offset 0x1A, size 0x1
    u8 bgm_type;

    u8 dummy; // offset 0x1B, size 0x1
};

typedef struct {
    // total size: 0x10
    u16 boix; // offset 0x0, size 0x2
    u16 bhix; // offset 0x2, size 0x2
    u16 haix; // offset 0x4, size 0x2
    union /* @anon10 */ {
        u16 full; // offset 0x0, size 0x2
        struct /* @anon12 */ {
            // total size: 0x2
            u8 bx; // offset 0x0, size 0x1
            u8 mv; // offset 0x1, size 0x1
        } half;    // offset 0x0, size 0x2
    } mf;          // offset 0x6, size 0x2
    u16 caix;      // offset 0x8, size 0x2
    u16 cuix;      // offset 0xA, size 0x2
    u16 atix;      // offset 0xC, size 0x2
    u16 hoix;      // offset 0xE, size 0x2
} UNK_0;

typedef struct {
    // total size: 0x20
    s16 body_dm[4][4]; // offset 0x0, size 0x20
} UNK_1;

typedef struct {
    // total size: 0x20
    s16 hand_dm[4][4]; // offset 0x0, size 0x20
} UNK_2;

typedef struct {
    // total size: 0x8
    s16 cat_box[4]; // offset 0x0, size 0x8
} UNK_3;

typedef struct {
    // total size: 0x8
    s16 cau_box[4]; // offset 0x0, size 0x8
} UNK_4;

typedef struct {
    // total size: 0x20
    s16 att_box[4][4]; // offset 0x0, size 0x20
} UNK_5;

typedef struct {
    // total size: 0x8
    s16 hos_box[4]; // offset 0x0, size 0x8
} UNK_6;

typedef struct {
    // total size: 0x10
    u8 reaction; // offset 0x0, size 0x1
    u8 level;    // offset 0x1, size 0x1
    u8 mkh_ix;   // offset 0x2, size 0x1
    u8 but_ix;   // offset 0x3, size 0x1
    u8 dipsw;    // offset 0x4, size 0x1
    u8 guard;    // offset 0x5, size 0x1
    u8 dir;      // offset 0x6, size 0x1
    u8 free;     // offset 0x7, size 0x1
    u8 pow;      // offset 0x8, size 0x1
    u8 impact;   // offset 0x9, size 0x1
    u8 piyo;     // offset 0xA, size 0x1
    u8 ng_type;  // offset 0xB, size 0x1
    s8 hs_me;    // offset 0xC, size 0x1
    s8 hs_you;   // offset 0xD, size 0x1
    u8 hit_mark; // offset 0xE, size 0x1
    u8 dmg_mark; // offset 0xF, size 0x1
} UNK_7;

typedef struct {
    // total size: 0x10
    s16 parts_hos_x; // offset 0x0, size 0x2
    s16 parts_hos_y; // offset 0x2, size 0x2
    u8 parts_colmd;  // offset 0x4, size 0x1
    u8 parts_colcd;  // offset 0x5, size 0x1
    u8 parts_prio;   // offset 0x6, size 0x1
    u8 parts_flip;   // offset 0x7, size 0x1
    u8 parts_timer;  // offset 0x8, size 0x1
    u8 parts_disp;   // offset 0x9, size 0x1
    s16 parts_mts;   // offset 0xA, size 0x2
    u16 parts_nix;   // offset 0xC, size 0x2
    u16 parts_char;  // offset 0xE, size 0x2
} UNK_8;

typedef struct {
    // total size: 0x8
    s16 olc_ix[4]; // offset 0x0, size 0x8
} UNK_9;

typedef struct {
    // total size: 0x8
    s16 catch_hos_x; // offset 0x0, size 0x2
    s16 catch_hos_y; // offset 0x2, size 0x2
    u8 catch_prio;   // offset 0x4, size 0x1
    u8 catch_flip;   // offset 0x5, size 0x1
    s16 catch_nix;   // offset 0x6, size 0x2
} CatchTable;

typedef struct {
    // total size: 0x8
    u16 code; // offset 0x0, size 0x2
    s16 koc;  // offset 0x2, size 0x2
    s16 ix;   // offset 0x4, size 0x2
    s16 pat;  // offset 0x6, size 0x2
} UNK11;

typedef struct {
    // total size: 0x18
    Reg32SpReal a[2]; // offset 0x0, size 0x8
    Reg32SpReal d[2]; // offset 0x8, size 0x8
    s16 kop[2];       // offset 0x10, size 0x4
    u16 index;        // offset 0x14, size 0x2
} MVXY;

typedef union {
    s32 cal; // offset 0x0, size 0x4
    struct {
        // total size: 0x4
        s16 low; // offset 0x0, size 0x2
        s16 pos; // offset 0x2, size 0x2
    } disp;      // offset 0x0, size 0x4
} XY;

typedef struct {
    // total size: 0xA8
    s16 total;             // offset 0x0, size 0x2
    s16 new_dm;            // offset 0x2, size 0x2
    s16 req_f;             // offset 0x4, size 0x2
    s16 old_r;             // offset 0x6, size 0x2
    s16 kind_of[10][4][2]; // offset 0x8, size 0xA0
} ComboType;

typedef struct {
    // total size: 0x14
    s8 flag;    // offset 0x0, size 0x1
    s16 genkai; // offset 0x2, size 0x2
    s16 time;   // offset 0x4, size 0x2
    union {
        s32 timer;       // offset 0x0, size 0x4
        LoHi16 quantity; // offset 0x0, size 0x4
    } now;               // offset 0x8, size 0x4
    s32 recover;         // offset 0xC, size 0x4
    s16 store;           // offset 0x10, size 0x2
    s16 again;           // offset 0x12, size 0x2
} PiyoriType;

typedef struct {
    s8 be_flag;
    s8 disp_flag;
    u8 blink_timing;
    u8 operator;
    u8 type;
    u8 charset_id;
    s16 work_id;
    s16 id;
    s8 rl_flag;
    s8 rl_waza;
    u32* target_adrs;
    u32* hit_adrs;
    u32* dmg_adrs;

    /// Index of the struct that is in front of this one in the list.
    s16 before;

    /// Index of this struct.
    s16 myself;

    /// Index of the struct that is behind this one in the list.
    s16 behind;

    /// Index of the list that this struct is a part of.
    s16 listix;

    s16 dead_f;
    s16 timing;
    s16 routine_no[8];
    s16 old_rno[8];
    s16 hit_stop;
    s16 hit_quake;
    s8 cgromtype;
    u8 kage_flag;
    s16 kage_hx;
    s16 kage_hy;
    s16 kage_prio;
    s16 kage_width;
    s16 kage_char;
    s16 position_x;
    s16 position_y;
    s16 position_z;
    s16 next_x;
    s16 next_y;
    s16 next_z;
    s16 scr_mv_x;
    s16 scr_mv_y;
    XY xyz[3];
    s16 old_pos[3];
    s16 sync_suzi;
    u16* suzi_offset;
    MVXY mvxy;
    s16 direction;
    s16 dir_old;
    s16 dir_step;
    s16 dir_timer;
    s16 vitality;
    s16 vital_new;
    s16 vital_old;
    s16 dm_vital;
    s16 dmcal_m;
    s16 dmcal_d;
    s8 weight_level;
    UNK11 cmoa;
    UNK11 cmsw;
    UNK11 cmlp;
    UNK11 cml2;
    UNK11 cmja;
    UNK11 cmj2;
    UNK11 cmj3;
    UNK11 cmj4;
    UNK11 cmj5;
    UNK11 cmj6;
    UNK11 cmj7;
    UNK11 cmms;
    UNK11 cmmd;
    UNK11 cmyd;
    UNK11 cmcf;
    UNK11 cmcr;
    UNK11 cmbk;
    UNK11 cmb2;
    UNK11 cmb3;
    UNK11 cmhs;
    UNK11 cmr0;
    UNK11 cmr1;
    UNK11 cmr2;
    UNK11 cmr3;
    s16 cmwk[32];
    u32* char_table[12];
    u32* se_random_table;
    s16* step_xy_table;
    s16* move_xy_table;
    UNK_8* overlap_char_tbl;
    UNK_9* olc_ix_table;
    UNK_9 cg_olc;
    CatchTable* rival_catch_tbl;
    CatchTable* curr_rca;
    u32* set_char_ad;
    s16 cg_ix;
    s16 now_koc;
    s16 char_index;
    s16 current_colcd;
    s16 cgd_type;
    u8 pat_status;
    u8 kind_of_waza;
    u8 hit_range;
    u8 total_paring;
    u8 total_att_set;
    u8 sp_tech_id;
    u8 cg_type;
    u8 cg_ctr;
    u16 cg_se;
    u16 cg_olc_ix;
    u16 cg_number;
    u16 cg_hit_ix;
    s16 cg_att_ix;
    u8 cg_extdat;
    u8 cg_cancel;
    u8 cg_effect;
    u8 cg_eftype;
    u16 cg_zoom;
    u16 cg_rival;
    u16 cg_add_xy;
    u8 cg_next_ix;
    u8 cg_status;
    s16 cg_wca_ix;
    s16 cg_jphos;
    u16 cg_meoshi;
    u8 cg_prio;
    u8 cg_flip;
    u16 old_cgnum;
    s16 floor;
    u16 ccoff;
    s16 colcd;
    s16 my_col_mode;
    s16 my_col_code;
    s16 my_priority;
    s16 my_family;
    s16 my_ext_pri;
    s16 my_bright_type;
    s16 my_bright_level;
    s16 my_clear_level;
    s16 my_mts;
    s16 my_mr_flag;
    struct {
        struct {
            s16 x;
            s16 y;
        } size;
    } my_mr;
    s16 my_trans_mode;
    s16 waku_work_index;
    s16 olc_work_ix[4];
    UNK_0* hit_ix_table;
    UNK_0 cg_ja;
    UNK_1* body_adrs;
    UNK_1* h_bod;
    UNK_2* hand_adrs;
    UNK_2* h_han;
    UNK_2* dumm_adrs;
    UNK_2* h_dumm;
    UNK_3* catch_adrs;
    UNK_3* h_cat;
    UNK_4* caught_adrs;
    UNK_4* h_cau;
    UNK_5* attack_adrs;
    UNK_5* h_att;
    UNK_5* h_eat;
    UNK_6* hosei_adrs;
    UNK_6* h_hos;
    UNK_7* att_ix_table;
    UNK_7 att;
    u16 zu_flag;
    u16 at_attribute;
    s16 kezuri_pow;
    u16 add_arts_point;
    u16 buttobi_type;
    u16 att_zuru;
    u16 at_ten_ix;
    s16 dir_atthit;
    s16 vs_id;
    u8 att_hit_ok;
    u8 meoshi_hit_flag;
    u16 at_koa;
    u8 paring_attack_flag;
    s8 no_death_attack;
    u8 jump_att_flag;
    s8 shell_vs_refrect;
    s16 renew_attack;
    u16 attack_num;
    u16 uketa_att[4];
    union {
        struct {
            u8 player;
            u8 effect;
        } hit;
        u16 hit_flag;
    } hf;
    s16 hit_mark_x;
    s16 hit_mark_y;
    s16 hit_mark_z;
    s16 kohm;
    u8 dm_fushin;
    s8 dm_weight;
    u16 dm_butt_type;
    u16 dm_zuru;
    u16 dm_attribute;
    s16 dm_guard_success;
    s16 dm_plnum;
    s16 dm_attlv;
    s16 dm_dir;
    s8 dm_rl;
    u8 dm_impact;
    s16 dm_stop;
    s16 dm_quake;
    u16 dm_piyo;
    u16 dm_ten_ix;
    u16 dm_koa;
    s16 dm_work_id;
    u16 dm_arts_point;
    u8 dm_jump_att_flag;
    u8 dm_free;
    s16 dm_count_up;
    s8 dm_nodeathattack;
    u8 dm_exdm_ix;
    u8 dm_dip;
    u8 dm_kind_of_waza;
    s16 attpow;
    s16 defpow;
    u32* my_effadrs;
    s16 shell_ix[8];
    s16 hm_dm_side;
    s16 extra_col;
    s16 extra_col_2;
    s16 original_vitality;
    u8 hit_work_id;
    u8 dmg_work_id;
    s8 K5_init_flag;
    s8 K5_exec_ok;
    u8 kow;
    u8 swallow_no_effect;
    s16 E3_work_index;
    s16 E4_work_index;
    u8 kezurare_flag;
    u8 wrd_free[53];
} WORK;

typedef struct {
    // total size: 0x2C
    s16 kind_of_arts; // offset 0x0, size 0x2
    u8 nmsa_g_ix;     // offset 0x2, size 0x1
    u8 exsa_g_ix;     // offset 0x3, size 0x1
    u8 exs2_g_ix;     // offset 0x4, size 0x1
    u8 nmsa_a_ix;     // offset 0x5, size 0x1
    u8 exsa_a_ix;     // offset 0x6, size 0x1
    u8 exs2_a_ix;     // offset 0x7, size 0x1
    s8 gauge_type;    // offset 0x8, size 0x1
    s8 mp;            // offset 0x9, size 0x1
    s8 ok;            // offset 0xA, size 0x1
    s8 ex;            // offset 0xB, size 0x1
    s8 ba;            // offset 0xC, size 0x1
    s8 dtm_mul;       // offset 0xD, size 0x1
    s8 mp_rno;        // offset 0xE, size 0x1
    s8 mp_rno2;       // offset 0xF, size 0x1
    s8 sa_rno;        // offset 0x10, size 0x1
    s8 sa_rno2;       // offset 0x11, size 0x1
    s8 ex_rno;        // offset 0x12, size 0x1
    s8 gt2;           // offset 0x13, size 0x1
    s8 saeff_ok;      // offset 0x14, size 0x1
    s8 saeff_mp;      // offset 0x15, size 0x1
    s16 gauge_len;    // offset 0x16, size 0x2
    union {
        s32 i;       // offset 0x0, size 0x4
        LoHi16 s;    // offset 0x0, size 0x4
    } gauge;         // offset 0x18, size 0x4
    s32 dtm;         // offset 0x1C, size 0x4
    s16 store_max;   // offset 0x20, size 0x2
    s16 store;       // offset 0x22, size 0x2
    s16 id_arts;     // offset 0x24, size 0x2
    u8 ex4th_full;   // offset 0x26, size 0x1
    u8 ex4th_exec;   // offset 0x27, size 0x1
    s16 total_gauge; // offset 0x28, size 0x2
    s16 bacckup_g_h; // offset 0x2A, size 0x2
} SA_WORK;

typedef struct {
    // total size: 0x406
    u16 sw_lvbt;       // offset 0x0, size 0x2
    u16 sw_new;        // offset 0x2, size 0x2
    u16 sw_old;        // offset 0x4, size 0x2
    u16 sw_now;        // offset 0x6, size 0x2
    u16 sw_off;        // offset 0x8, size 0x2
    u16 sw_chg;        // offset 0xA, size 0x2
    u16 old_now;       // offset 0xC, size 0x2
    s16 lgp;           // offset 0xE, size 0x2
    u8 ca14;           // offset 0x10, size 0x1
    u8 ca25;           // offset 0x11, size 0x1
    u8 ca36;           // offset 0x12, size 0x1
    u8 calf;           // offset 0x13, size 0x1
    u8 calr;           // offset 0x14, size 0x1
    u8 lever_dir;      // offset 0x15, size 0x1
    s16 waza_flag[56]; // offset 0x16, size 0x70
    s16 reset[56];     // offset 0x86, size 0x70
    u8 waza_r[56][4];  // offset 0xF6, size 0xE0
    u16 btix[56];      // offset 0x1D6, size 0x70
    u16 exdt[56][4];   // offset 0x246, size 0x1C0
} WORK_CP;

typedef struct {
    // total size: 0x6
    s16 r_no;    // offset 0x0, size 0x2
    s16 char_ix; // offset 0x2, size 0x2
    s16 data_ix; // offset 0x4, size 0x2
} AS;

typedef struct {
    // total size: 0x46C
    WORK wu;           // offset 0x0, size 0x388
    WORK_CP* cp;       // offset 0x388, size 0x4
    u32 spmv_ng_flag;  // offset 0x38C, size 0x4
    u32 spmv_ng_flag2; // offset 0x390, size 0x4

    /// Number of the character that the player controls.
    /// offset 0x394, size 0x2
    s16 player_number;

    s16 zuru_timer;       // offset 0x396, size 0x2
    u16 zuru_ix_counter;  // offset 0x398, size 0x2
    u8 zuru_flag;         // offset 0x39A, size 0x1
    s8 tsukamarenai_flag; // offset 0x39B, size 0x1
    u8 kizetsu_kow;       // offset 0x39C, size 0x1
    u8 micchaku_flag;     // offset 0x39D, size 0x1
    u8 hos_fi_flag;       // offset 0x39E, size 0x1
    u8 hos_em_flag;       // offset 0x39F, size 0x1

    /// Number of the character the player is throwing.
    /// offset 0x3A0, size 0x2
    s16 tsukami_num;

    /// Flag that's set if the player is throwing the opponent.
    /// offset 0x3A2, size 0x1
    s8 tsukami_f;

    /// Flag that's set if the player is being thrown by the opponent.
    /// offset 0x3A3, size 0x1
    s8 tsukamare_f;

    s8 kind_of_catch;       // offset 0x3A4, size 0x1
    u8 old_gdflag;          // offset 0x3A5, size 0x1
    u8 guard_flag;          // offset 0x3A6, size 0x1
    u8 guard_chuu;          // offset 0x3A7, size 0x1
    s16 dm_ix;              // offset 0x3A8, size 0x2
    s16 hosei_amari;        // offset 0x3AA, size 0x2
    s8 dm_hos_flag;         // offset 0x3AC, size 0x1
    u8 dm_point;            // offset 0x3AD, size 0x1
    s16 muriyari_ugoku;     // offset 0x3AE, size 0x2
    s8 scr_pos_set_flag;    // offset 0x3B0, size 0x1
    s8 hoshi_flag;          // offset 0x3B1, size 0x1
    s8 the_same_players;    // offset 0x3B2, size 0x1
    s8* dm_step_tbl;        // offset 0x3B4, size 0x4
    s8 running_f;           // offset 0x3B8, size 0x1
    s8 cancel_timer;        // offset 0x3B9, size 0x1
    s8 jpdir;               // offset 0x3BA, size 0x1
    s8 jptim;               // offset 0x3BB, size 0x1
    s16 current_attack;     // offset 0x3BC, size 0x2
    const AS* as;           // offset 0x3C0, size 0x4
    SA_WORK* sa;            // offset 0x3C4, size 0x4
    ComboType* cb;          // offset 0x3C8, size 0x4
    PiyoriType* py;         // offset 0x3CC, size 0x4
    s8 wkey_flag;           // offset 0x3D0, size 0x1
    s8 dead_flag;           // offset 0x3D1, size 0x1
    s16 ukemi_ok_timer;     // offset 0x3D2, size 0x2
    s16 backup_ok_timer;    // offset 0x3D4, size 0x2
    s8 uot_cd_ok_flag;      // offset 0x3D6, size 0x1
    s8 ukemi_success;       // offset 0x3D7, size 0x1
    s16 old_pos_data[8];    // offset 0x3D8, size 0x10
    s16 move_distance;      // offset 0x3E8, size 0x2
    s16 move_power;         // offset 0x3EA, size 0x2
    s16 sa_stop_sai;        // offset 0x3EC, size 0x2
    u8 saishin_lvdir;       // offset 0x3EE, size 0x1
    u8 sa_stop_lvdir;       // offset 0x3EF, size 0x1
    u8 sa_stop_flag;        // offset 0x3F0, size 0x1
    u8 kezurijini_flag;     // offset 0x3F1, size 0x1
    WORK* illusion_work;    // offset 0x3F4, size 0x4
    s16 image_setup_flag;   // offset 0x3F8, size 0x2
    s16 image_data_index;   // offset 0x3FA, size 0x2
    u8 caution_flag;        // offset 0x3FC, size 0x1
    u8 tc_1st_flag;         // offset 0x3FD, size 0x1
    ComboType* rp;          // offset 0x400, size 0x4
    s16 bullet_hcnt;        // offset 0x404, size 0x2
    s16 bhcnt_timer;        // offset 0x406, size 0x2
    s8 cat_break_ok_timer;  // offset 0x408, size 0x1
    s8 cat_break_reserve;   // offset 0x409, size 0x1
    s8 hazusenai_flag;      // offset 0x40A, size 0x1
    s8 hurimukenai_flag;    // offset 0x40B, size 0x1
    u8 tk_success;          // offset 0x40C, size 0x1
    u8 resurrection_resv;   // offset 0x40D, size 0x1
    s16 tk_dageki;          // offset 0x40E, size 0x2
    s16 tk_nage;            // offset 0x410, size 0x2
    s16 tk_kizetsu;         // offset 0x412, size 0x2
    s16 tk_konjyou;         // offset 0x414, size 0x2
    s16 utk_dageki;         // offset 0x416, size 0x2
    s16 utk_nage;           // offset 0x418, size 0x2
    s16 utk_kizetsu;        // offset 0x41A, size 0x2
    u8 atemi_flag;          // offset 0x41C, size 0x1
    u8 atemi_point;         // offset 0x41D, size 0x1
    s16 dm_vital_backup;    // offset 0x41E, size 0x2
    u8 dm_refrect;          // offset 0x420, size 0x1
    u8 dm_vital_use;        // offset 0x421, size 0x1
    u8 exdm_ix;             // offset 0x422, size 0x1
    u8 meoshi_jump_flag;    // offset 0x423, size 0x1
    s16 cmd_request;        // offset 0x424, size 0x2
    s16 rl_save;            // offset 0x426, size 0x2
    u8 zettai_muteki_flag;  // offset 0x428, size 0x1
    u8 do_not_move;         // offset 0x429, size 0x1
    u16 just_sa_stop_timer; // offset 0x42A, size 0x2
    s16 total_att_hit_ok;   // offset 0x42C, size 0x2
    u8 sa_healing;          // offset 0x42E, size 0x1
    u8 auto_guard;          // offset 0x42F, size 0x1
    u8 hsjp_ok;             // offset 0x430, size 0x1
    u8 high_jump_flag;      // offset 0x431, size 0x1
    s16 att_plus;           // offset 0x432, size 0x2
    s16 def_plus;           // offset 0x434, size 0x2
    s8 bs2_on_car;          // offset 0x436, size 0x1
    s8 bs2_area_car;        // offset 0x437, size 0x1
    s8 bs2_over_car;        // offset 0x438, size 0x1
    s8 bs2_area_car2;       // offset 0x439, size 0x1
    s8 bs2_over_car2;       // offset 0x43A, size 0x1
    u8 micchaku_wall_time;  // offset 0x43B, size 0x1
    u8 extra_jump;          // offset 0x43C, size 0x1
    u8 air_jump_ok_time;    // offset 0x43D, size 0x1
    s16 waku_ram_index;     // offset 0x43E, size 0x2
    u16 permited_koa;       // offset 0x440, size 0x2
    u8 ja_nmj_rno;          // offset 0x442, size 0x1
    u8 ja_nmj_cnt;          // offset 0x443, size 0x1
    u8 kind_of_blocking;    // offset 0x444, size 0x1
    u8 metamorphose;        // offset 0x445, size 0x1
    s16 metamor_index;      // offset 0x446, size 0x2
    u8 metamor_over;        // offset 0x448, size 0x1
    u8 gill_ccch_go;        // offset 0x449, size 0x1
    u8 renew_attchar;       // offset 0x44A, size 0x1
    s16 omop_vital_timer;   // offset 0x44C, size 0x2
    s16 sfwing_pos;         // offset 0x44E, size 0x2
    u8 init_E3_flag;        // offset 0x450, size 0x1
    u8 init_E4_flag;        // offset 0x451, size 0x1
    u16 pl09_dat_index;     // offset 0x452, size 0x2
    s16 reserv_add_y;       // offset 0x454, size 0x2
    u8 pt_free[20];         // offset 0x456, size 0x14
} PLW;

typedef struct {
    WORK wu;             // offset 0x0, size 0x388
    u32* my_master;      // offset 0x388, size 0x4
    s16 master_work_id;  // offset 0x38C, size 0x2
    s16 master_id;       // offset 0x38E, size 0x2
    s16 master_player;   // offset 0x390, size 0x2
    s16 master_priority; // offset 0x392, size 0x2
    u8 dm_refrect;       // offset 0x394, size 0x1
    u8 refrected;        // offset 0x395, size 0x1
    s16 free;            // offset 0x396, size 0x2
    u32 master_ng_flag;  // offset 0x398, size 0x4
    u32 master_ng_flag2; // offset 0x39C, size 0x4
    u8 et_free[30];      // offset 0x3A0, size 0x1E
} WORK_Other;

typedef struct {
    // total size: 0x8
    s16 nx;  // offset 0x0, size 0x2
    s16 ny;  // offset 0x2, size 0x2
    s16 col; // offset 0x4, size 0x2
    u16 chr; // offset 0x6, size 0x2
} CONN;

typedef struct {
    // total size: 0x6F8
    WORK wu;             // offset 0x0, size 0x388
    u32* my_master;      // offset 0x388, size 0x4
    s16 master_work_id;  // offset 0x38C, size 0x2
    s16 master_id;       // offset 0x38E, size 0x2
    s16 master_player;   // offset 0x390, size 0x2
    s16 master_priority; // offset 0x392, size 0x2
    s16 prio_reverse;    // offset 0x394, size 0x2
    s16 num_of_conn;     // offset 0x396, size 0x2
    CONN conn[108];      // offset 0x398, size 0x360
} WORK_Other_CONN;

typedef struct {
    // total size: 0x6F8
    WORK wu;             // offset 0x0, size 0x388
    u32* my_master;      // offset 0x388, size 0x4
    s16 master_work_id;  // offset 0x38C, size 0x2
    s16 master_id;       // offset 0x38E, size 0x2
    s16 master_player;   // offset 0x390, size 0x2
    s16 master_priority; // offset 0x392, size 0x2
    s8 look_up_flag;     // offset 0x394, size 0x1
    s8 curr_ja;          // offset 0x395, size 0x1
    u16 ja_disp_bit;     // offset 0x396, size 0x2
    u16 ja_color_bit;    // offset 0x398, size 0x2
    s16 fade_cja;        // offset 0x39A, size 0x2
    s16 ja[62][2];       // offset 0x39C, size 0xF8
    s16 jx[15][4];       // offset 0x494, size 0x78
} WORK_Other_JUDGE;

typedef struct {
    s32 DispWidth;
    s32 DispHeight;
    f32 ZBuffMax;
    u32 TextureStartAdrs;
    u32 FrameClearColor;
    u32 SystemStatus;
    s32 SystemIndex;
    uintptr_t SystemTmpBuffStartAdrs;
    uintptr_t SystemTmpBuffEndAdrs;
    uintptr_t SystemTmpBuffNow;
    u32 SystemTmpBuffHandle[2];
} FLPS2State;

typedef struct {
    // total size: 0x3C
    u32 channel_id;             // offset 0x0, size 0x4
    u32 queue_size;             // offset 0x4, size 0x4
    s32 handleID;               // offset 0x8, size 0x4
    u32 dma_queue_handle[2];    // offset 0xC, size 0x8
    u32 queue_ctr[2];           // offset 0x14, size 0x8
    u32 queue_ptr0[2];          // offset 0x1C, size 0x8
    u32 queue_ptr1[2];          // offset 0x24, size 0x8
    u32 now_adrs;               // offset 0x2C, size 0x4
    u32 dma_normal_mode_status; // offset 0x30, size 0x4
    uintptr_t old_queue_data;   // offset 0x34, size 0x4
    uintptr_t old_endtag;       // offset 0x38, size 0x4
} FLPS2VIF1Control;

typedef union {
    __int128 I128;  // offset 0x0, size 0x10
    s64 I64[2];     // offset 0x0, size 0x10
    s32 I32[4];     // offset 0x0, size 0x10
    s16 I16[8];     // offset 0x0, size 0x10
    s8 I8[16];      // offset 0x0, size 0x10
    __int128 UI128; // offset 0x0, size 0x10
    u64 UI64[2];    // offset 0x0, size 0x10
    u32 UI32[4];    // offset 0x0, size 0x10
    u16 UI16[8];    // offset 0x0, size 0x10
    u8 UI8[16];     // offset 0x0, size 0x10
    f32 F32[4];     // offset 0x0, size 0x10
    f64 F64[2];     // offset 0x0, size 0x10
} QWORD;

typedef struct {
    // total size: 0xA0
    QWORD dmatag;     // offset 0x0, size 0x10
    QWORD giftag;     // offset 0x10, size 0x10
    QWORD frame_1;    // offset 0x20, size 0x10
    QWORD frame_2;    // offset 0x30, size 0x10
    QWORD zbuf_1;     // offset 0x40, size 0x10
    QWORD zbuf_2;     // offset 0x50, size 0x10
    QWORD xyoffset_1; // offset 0x60, size 0x10
    QWORD xyoffset_2; // offset 0x70, size 0x10
    QWORD dthe;       // offset 0x80, size 0x10
    QWORD colclamp;   // offset 0x90, size 0x10
} FLPS2DB;

typedef struct {
    // total size: 0x140
    QWORD dmatag;         // offset 0x0, size 0x10
    QWORD giftag;         // offset 0x10, size 0x10
    QWORD clr_fba;        // offset 0x20, size 0x10
    QWORD clr_scissor;    // offset 0x30, size 0x10
    QWORD clr_test;       // offset 0x40, size 0x10
    QWORD clr_prim;       // offset 0x50, size 0x10
    QWORD clr_rgbaq;      // offset 0x60, size 0x10
    QWORD clr_xyz2_0;     // offset 0x70, size 0x10
    QWORD clr_xyz2_1;     // offset 0x80, size 0x10
    QWORD acr_scissor_1;  // offset 0x90, size 0x10
    QWORD acr_scissor_2;  // offset 0xA0, size 0x10
    QWORD acr_test_1;     // offset 0xB0, size 0x10
    QWORD acr_test_2;     // offset 0xC0, size 0x10
    QWORD acr_zbuf_1;     // offset 0xD0, size 0x10
    QWORD acr_zbuf_2;     // offset 0xE0, size 0x10
    QWORD acr_fba_1;      // offset 0xF0, size 0x10
    QWORD acr_fba_2;      // offset 0x100, size 0x10
    QWORD acr_pabe;       // offset 0x110, size 0x10
    QWORD acr_prmodecont; // offset 0x120, size 0x10
    QWORD acr_dimx;       // offset 0x130, size 0x10
} FLPS2DrawStart;

enum _FLSETRENDERSTATE {
    FLRENDER_CULL = 0,
    FLRENDER_LIGHTING = 1,
    FLRENDER_SPECULAR = 2,
    FLRENDER_WRAP = 3,
    FLRENDER_TEXSTAGE0 = 4,
    FLRENDER_TEXSTAGE1 = 5,
    FLRENDER_TEXSTAGE2 = 6,
    FLRENDER_TEXSTAGE3 = 7,
    FLRENDER_TEXOPE0 = 8,
    FLRENDER_TEXOPE1 = 9,
    FLRENDER_TEXOPE2 = 10,
    FLRENDER_TEXOPE3 = 11,
    FLRENDER_SCISSOR = 12,
    FLRENDER_BLENDOPE = 13,
    FLRENDER_AMBIENT = 14,
    FLRENDER_FOGCOLOR = 15,
    FLRENDER_FOGSTART = 16,
    FLRENDER_FOGEND = 17,
    FLRENDER_FOGENABLE = 18,
    FLRENDER_FLIP = 19,
    FLRENDER_BACKCOLOR = 20,
    FLRENDER_MATERIAL = 21,
    FLRENDER_VIEW = 22,
    FLRENDER_PROJ = 23,
    FLRENDER_VIEWPORT = 24,
    FLRENDER_UVSCRMATRIX = 25,
    FLRENDER_WORLD0 = 26,
    FLRENDER_WORLD1 = 27,
    FLRENDER_WORLD2 = 28,
    FLRENDER_WORLD3 = 29,
    FLRENDER_WORLD4 = 30,
    FLRENDER_WORLD5 = 31,
    FLRENDER_WORLD6 = 32,
    FLRENDER_WORLD7 = 33,
    FLRENDER_WORLD8 = 34,
    FLRENDER_WORLD9 = 35,
    FLRENDER_WORLD10 = 36,
    FLRENDER_WORLD11 = 37,
    FLRENDER_WORLD12 = 38,
    FLRENDER_WORLD13 = 39,
    FLRENDER_WORLD14 = 40,
    FLRENDER_WORLD15 = 41,
    FLRENDER_WORLD16 = 42,
    FLRENDER_WORLD17 = 43,
    FLRENDER_WORLD18 = 44,
    FLRENDER_WORLD19 = 45,
    FLRENDER_WORLD20 = 46,
    FLRENDER_WORLD21 = 47,
    FLRENDER_WORLD22 = 48,
    FLRENDER_WORLD23 = 49,
    FLRENDER_WORLD24 = 50,
    FLRENDER_WORLD25 = 51,
    FLRENDER_WORLD26 = 52,
    FLRENDER_WORLD27 = 53,
    FLRENDER_WORLD28 = 54,
    FLRENDER_WORLD29 = 55,
    FLRENDER_WORLD30 = 56,
    FLRENDER_WORLD31 = 57,
    FLRENDER_MATERIAL0 = 58,
    FLRENDER_MATERIAL1 = 59,
    FLRENDER_MATERIAL2 = 60,
    FLRENDER_MATERIAL3 = 61,
    FLRENDER_MATERIAL4 = 62,
    FLRENDER_MATERIAL5 = 63,
    FLRENDER_MATERIAL6 = 64,
    FLRENDER_MATERIAL7 = 65,
    FLRENDER_MATERIAL8 = 66,
    FLRENDER_MATERIAL9 = 67,
    FLRENDER_MATERIAL10 = 68,
    FLRENDER_MATERIAL11 = 69,
    FLRENDER_MATERIAL12 = 70,
    FLRENDER_MATERIAL13 = 71,
    FLRENDER_MATERIAL14 = 72,
    FLRENDER_MATERIAL15 = 73,
    FLRENDER_MATERIAL16 = 74,
    FLRENDER_MATERIAL17 = 75,
    FLRENDER_MATERIAL18 = 76,
    FLRENDER_MATERIAL19 = 77,
    FLRENDER_MATERIAL20 = 78,
    FLRENDER_MATERIAL21 = 79,
    FLRENDER_MATERIAL22 = 80,
    FLRENDER_MATERIAL23 = 81,
    FLRENDER_MATERIAL24 = 82,
    FLRENDER_MATERIAL25 = 83,
    FLRENDER_MATERIAL26 = 84,
    FLRENDER_MATERIAL27 = 85,
    FLRENDER_MATERIAL28 = 86,
    FLRENDER_MATERIAL29 = 87,
    FLRENDER_MATERIAL30 = 88,
    FLRENDER_MATERIAL31 = 89,
    FLRENDER_LIGHT0 = 90,
    FLRENDER_LIGHT1 = 91,
    FLRENDER_LIGHT2 = 92,
    FLRENDER_SHADER = 93,
    FLRENDER_ALPHABLENDMODE = 94,
    FLRENDER_ALPHATEST = 95,
    FLRENDER_ALPHAREF = 96,
    FLRENDER_ALPHABLENDENABLE = 97,
    FLRENDER_UVSCROLL = 98,
    FLRENDER_TEXTUREFILTER = 99,
    FLRENDER_TEXTUREADDRESSING = 100,
    FLRENDER_RENDERTARGET = 101,
    FLRENDER_FADECOLORENABLE = 102,
    FLRENDER_FADECOLOR = 103,
    FLRENDER_MIPMAPBIAS = 104,
    FLRENDER_MIPMAPARG1 = 105,
    FLRENDER_MIPMAPARG2 = 106,
    FLRENDER_MIPMAPFILTER = 107,
    FLRENDER_ZWRITE = 108,
    FLRENDER_ZOPE = 109,
};

typedef struct _MEMMAN_CELL {
    // total size: 0xC
    struct _MEMMAN_CELL* prev; // offset 0x0, size 0x4
    struct _MEMMAN_CELL* next; // offset 0x4, size 0x4
    ssize_t size;              // offset 0x8, size 0x4
} _MEMMAN_CELL;

typedef struct {
    // total size: 0x2C
    u8* memHead;                   // offset 0x0, size 0x4
    ssize_t memSize;               // offset 0x4, size 0x4
    u32 ownNumber;                 // offset 0x8, size 0x4
    s32 ownUnit;                   // offset 0xC, size 0x4
    ssize_t remainder;             // offset 0x10, size 0x4
    ssize_t remainderMin;          // offset 0x14, size 0x4
    struct _MEMMAN_CELL* cell_1st; // offset 0x18, size 0x4
    struct _MEMMAN_CELL* cell_fin; // offset 0x1C, size 0x4
    u8* oriHead;                   // offset 0x20, size 0x4
    s32 oriSize;                   // offset 0x24, size 0x4
    s32 debIndex;                  // offset 0x28, size 0x4
} _MEMMAN_OBJ;

typedef struct {
    // total size: 0x90
    s8 be_flag;        // offset 0x0, size 0x1
    s8 disp_flag;      // offset 0x1, size 0x1
    s16 fam_no;        // offset 0x2, size 0x2
    s16 r_no_0;        // offset 0x4, size 0x2
    s16 r_no_1;        // offset 0x6, size 0x2
    s16 r_no_2;        // offset 0x8, size 0x2
    s16 position_x;    // offset 0xA, size 0x2
    s16 position_y;    // offset 0xC, size 0x2
    s32 speed_x;       // offset 0x10, size 0x4
    s32 speed_y;       // offset 0x14, size 0x4
    XY xy[2];          // offset 0x18, size 0x8
    XY wxy[2];         // offset 0x20, size 0x8
    u16* bg_address;   // offset 0x28, size 0x4
    u16* suzi_adrs;    // offset 0x2C, size 0x4
    s16 old_pos_x;     // offset 0x30, size 0x2
    s32 zuubun;        // offset 0x34, size 0x4
    s16 no_suzi_line;  // offset 0x38, size 0x2
    u16* start_suzi;   // offset 0x3C, size 0x4
    s16 u_line;        // offset 0x40, size 0x2
    s16 d_line;        // offset 0x42, size 0x2
    s16 bg_adrs_c_no;  // offset 0x44, size 0x2
    s16 suzi_c_no;     // offset 0x46, size 0x2
    s16 pos_x_work;    // offset 0x48, size 0x2
    s16 pos_y_work;    // offset 0x4A, size 0x2
    s8 rewrite_flag;   // offset 0x4C, size 0x1
    s8 suzi_base_flag; // offset 0x4D, size 0x1
    XY hos_xy[2];      // offset 0x50, size 0x8
    XY chase_xy[2];    // offset 0x58, size 0x8
    s16 free;          // offset 0x60, size 0x2
    s16 frame_deff;    // offset 0x62, size 0x2
    s16 r_limit;       // offset 0x64, size 0x2
    s16 r_limit2;      // offset 0x66, size 0x2
    s16 l_limit;       // offset 0x68, size 0x2
    s16 l_limit2;      // offset 0x6A, size 0x2
    s16 y_limit;       // offset 0x6C, size 0x2
    s16 y_limit2;      // offset 0x6E, size 0x2
    u16* suzi_adrs2;   // offset 0x70, size 0x4
    u16* start_suzi2;  // offset 0x74, size 0x4
    s16 suzi_c_no2;    // offset 0x78, size 0x2
    s32 max_x_limit;   // offset 0x7C, size 0x4
    s16* deff_rl;      // offset 0x80, size 0x4
    s16* deff_plus;    // offset 0x84, size 0x4
    s16* deff_minus;   // offset 0x88, size 0x4
    s16 abs_x;         // offset 0x8C, size 0x2
    s16 abs_y;         // offset 0x8E, size 0x2
} BGW;

typedef struct {
    // total size: 0x428
    s8 bg_routine;     // offset 0x0, size 0x1
    s8 bg_r_1;         // offset 0x1, size 0x1
    s8 bg_r_2;         // offset 0x2, size 0x1
    s8 stage;          // offset 0x3, size 0x1
    s8 area;           // offset 0x4, size 0x1
    s8 compel_flag;    // offset 0x5, size 0x1
    s32 scroll_cg_adr; // offset 0x8, size 0x4
    s32 ake_cg_adr;    // offset 0xC, size 0x4
    u8 scno;           // offset 0x10, size 0x1
    u8 scrno;          // offset 0x11, size 0x1
    s16 bg2_sp_x;      // offset 0x12, size 0x2
    s16 bg2_sp_y;      // offset 0x14, size 0x2
    s16 scr_stop;      // offset 0x16, size 0x2
    s8 frame_flag;     // offset 0x18, size 0x1
    s8 chase_flag;     // offset 0x19, size 0x1
    s8 old_chase_flag; // offset 0x1A, size 0x1
    s8 old_frame_flag; // offset 0x1B, size 0x1
    s16 pos_offset;    // offset 0x1C, size 0x2
    s16 quake_x_index; // offset 0x1E, size 0x2
    s16 quake_y_index; // offset 0x20, size 0x2
    s16 bg_f_x;        // offset 0x22, size 0x2
    s16 bg_f_y;        // offset 0x24, size 0x2
    s16 bg2_sp_x2;     // offset 0x26, size 0x2
    s16 bg2_sp_y2;     // offset 0x28, size 0x2
    s16 frame_deff;    // offset 0x2A, size 0x2
    s16 center_x;      // offset 0x2C, size 0x2
    s16 center_y;      // offset 0x2E, size 0x2
    s16 bg_index;      // offset 0x30, size 0x2
    s8 frame_vol;      // offset 0x32, size 0x1
    s16 max_x;         // offset 0x34, size 0x2
    u8 bg_opaque;      // offset 0x36, size 0x1
    BGW bgw[7];        // offset 0x38, size 0x3F0
} BG;

typedef struct {
    // total size: 0x18
    u8* memoryblock;   // offset 0x0, size 0x4
    u8* baseandcap[2]; // offset 0x4, size 0x8
    u8* frame[2];      // offset 0xC, size 0x8
    s32 align;         // offset 0x14, size 0x4
} FL_FMS;

typedef struct {
    // total size: 0xC
    u8 order;    // offset 0x0, size 0x1
    u8 kind_req; // offset 0x1, size 0x1
    u8 kind_cnt; // offset 0x2, size 0x1
    u8 request;  // offset 0x3, size 0x1
    u8 contents; // offset 0x4, size 0x1
    u8 timer;    // offset 0x5, size 0x1
    s16 pos_x;   // offset 0x6, size 0x2
    s16 pos_y;   // offset 0x8, size 0x2
    s16 pos_z;   // offset 0xA, size 0x2
} MessageData;

typedef struct {
    // total size: 0x48
    s8 contents[10][7]; // offset 0x0, size 0x46
    u16 sum;            // offset 0x46, size 0x2
} SystemDir;

typedef struct {
    // total size: 0xC
    u8 Shot[8];   // offset 0x0, size 0x8
    u8 Vibration; // offset 0x8, size 0x1
    u8 free[3];   // offset 0x9, size 0x3
} _PAD_INFOR;

typedef struct {
    // total size: 0x20
    s8 contents[4][8]; // offset 0x0, size 0x20
} _EXTRA_OPTION;

typedef struct {
    // total size: 0x14
    u8 name[3];      // offset 0x0, size 0x3
    u16 player;      // offset 0x4, size 0x2
    u32 score;       // offset 0x8, size 0x4
    s8 cpu_grade;    // offset 0xC, size 0x1
    s8 grade;        // offset 0xD, size 0x1
    u16 wins;        // offset 0xE, size 0x2
    u8 player_color; // offset 0x10, size 0x1
    u8 all_clear;    // offset 0x11, size 0x1
} RANK_DATA;

struct _SAVE_W {
    // total size: 0x208
    _PAD_INFOR Pad_Infor[2];    // offset 0x0, size 0x18
    u8 Difficulty;              // offset 0x18, size 0x1
    s8 Time_Limit;              // offset 0x19, size 0x1
    u8 Battle_Number[2];        // offset 0x1A, size 0x2
    u8 Damage_Level;            // offset 0x1C, size 0x1
    u8 Handicap;                // offset 0x1D, size 0x1
    u8 Partner_Type[2];         // offset 0x1E, size 0x2
    s8 Adjust_X;                // offset 0x20, size 0x1
    s8 Adjust_Y;                // offset 0x21, size 0x1
    u8 Screen_Size;             // offset 0x22, size 0x1
    u8 Screen_Mode;             // offset 0x23, size 0x1
    u8 GuardCheck;              // offset 0x24, size 0x1
    u8 Auto_Save;               // offset 0x25, size 0x1
    u8 AnalogStick;             // offset 0x26, size 0x1
    u8 BgmType;                 // offset 0x27, size 0x1
    u8 SoundMode;               // offset 0x28, size 0x1
    u8 BGM_Level;               // offset 0x29, size 0x1
    u8 SE_Level;                // offset 0x2A, size 0x1
    u8 Extra_Option;            // offset 0x2B, size 0x1
    u8 PL_Color[2][20];         // offset 0x2C, size 0x28
    _EXTRA_OPTION extra_option; // offset 0x54, size 0x20
    RANK_DATA Ranking[20];      // offset 0x74, size 0x190
    u32 sum;                    // offset 0x204, size 0x4
};

typedef struct {
    // total size: 0x8
    s8*** msgAdr; // offset 0x0, size 0x4
    s8* msgNum;   // offset 0x4, size 0x4
} MessageTable;

struct _player_infor {
    // total size: 0x4
    u8 my_char;     // offset 0x0, size 0x1
    s8 sa;          // offset 0x1, size 0x1
    s8 color;       // offset 0x2, size 0x1
    s8 player_type; // offset 0x3, size 0x1
};

struct _REP_GAME_INFOR {
    // total size: 0x24
    struct _player_infor player_infor[2]; // offset 0x0, size 0x8
    s8 stage;                             // offset 0x8, size 0x1
    s8 Direction_Working;                 // offset 0x9, size 0x1
    s8 Vital_Handicap[2];                 // offset 0xA, size 0x2
    s16 Random_ix16;                      // offset 0xC, size 0x2
    s16 Random_ix32;                      // offset 0xE, size 0x2
    s16 Random_ix16_ex;                   // offset 0x10, size 0x2
    s16 Random_ix32_ex;                   // offset 0x12, size 0x2
    s8* fname;                            // offset 0x14, size 0x4
    u8 winner;                            // offset 0x18, size 0x1
    u8 play_type;                         // offset 0x19, size 0x1
    u16 players_timer;                    // offset 0x1A, size 0x2
    s16 old_mes_no2;                      // offset 0x1C, size 0x2
    s16 old_mes_no3;                      // offset 0x1E, size 0x2
    s16 old_mes_no_pl;                    // offset 0x20, size 0x2
    s16 mes_already;                      // offset 0x22, size 0x2
};

struct _MINI_SAVE_W {
    // total size: 0x3C
    _PAD_INFOR Pad_Infor[2];    // offset 0x0, size 0x18
    s8 Time_Limit;              // offset 0x18, size 0x1
    u8 Battle_Number[2];        // offset 0x19, size 0x2
    u8 Damage_Level;            // offset 0x1B, size 0x1
    _EXTRA_OPTION extra_option; // offset 0x1C, size 0x20
};

typedef struct {
    // total size: 0x48
    u8 Handle_1P[18];   // offset 0x0, size 0x12
    u8 Id_1P[8];        // offset 0x12, size 0x8
    u8 Handle_2P[18];   // offset 0x1A, size 0x12
    u8 Id_2P[8];        // offset 0x2C, size 0x8
    u8 Serial_No[6];    // offset 0x34, size 0x6
    u8 Battle_Code[14]; // offset 0x3A, size 0xE
} _NET_INFOR;

typedef struct {
    // total size: 0x7448
    u8 header[640];                    // offset 0x0, size 0x280
    u8 sega_reserve[64];               // offset 0x280, size 0x40
    s16 Control_Time_Buff;             // offset 0x2C0, size 0x2
    u8 Difficulty;                     // offset 0x2C2, size 0x1
    u8 Monitor_Type;                   // offset 0x2C3, size 0x1
    u8 free_free[4];                   // offset 0x2C4, size 0x4
    struct _REP_GAME_INFOR game_infor; // offset 0x2C8, size 0x24
    struct _MINI_SAVE_W mini_save_w;   // offset 0x2EC, size 0x3C
    SystemDir system_dir;              // offset 0x328, size 0x48
    _NET_INFOR net_infor;              // offset 0x370, size 0x48
    struct {
        // total size: 0x7078
        u16 key_buff[2][7198]; // offset 0x0, size 0x7078
    } io_unit;                 // offset 0x3B8, size 0x7078
    u8 lag[14];                // offset 0x7430, size 0xE
    u32 sum;                   // offset 0x7440, size 0x4
    u8 champion;               // offset 0x7444, size 0x1
    u8 full_data;              // offset 0x7445, size 0x1
    u8 free;                   // offset 0x7446, size 0x1
    u8 extra_free;             // offset 0x7447, size 0x1
} _REPLAY_W;

struct _VM_W {
    // total size: 0x6C
    u8 r_no[4];     // offset 0x0, size 0x4
    u8 r_sub[4];    // offset 0x4, size 0x4
    s32 Timer;      // offset 0x8, size 0x4
    s32 FreeMem[2]; // offset 0xC, size 0x8
    s8 Format[2];   // offset 0x14, size 0x2
    s8 Find[2];     // offset 0x16, size 0x2
    s8 Connect[2];  // offset 0x18, size 0x2
    s8 CheckDrive;  // offset 0x1A, size 0x1
    s8 AutoDrive;   // offset 0x1B, size 0x1
    s8 Drive;       // offset 0x1C, size 0x1
    s8 Access;      // offset 0x1D, size 0x1
    s8 Request;     // offset 0x1E, size 0x1
    s8 AutoLoaded;  // offset 0x1F, size 0x1
    struct {
        // total size: 0x8
        u16 year;      // offset 0x0, size 0x2
        u8 month;      // offset 0x2, size 0x1
        u8 day;        // offset 0x3, size 0x1
        u8 hour;       // offset 0x4, size 0x1
        u8 minute;     // offset 0x5, size 0x1
        u8 second;     // offset 0x6, size 0x1
        u8 dayofweek;  // offset 0x7, size 0x1
    } curTime[2];      // offset 0x20, size 0x10
    s32 curSize[2];    // offset 0x30, size 0x8
    s16 memKey;        // offset 0x38, size 0x2
    u8* memAdr;        // offset 0x3C, size 0x4
    s32 nowResult;     // offset 0x40, size 0x4
    s32 nowNumber;     // offset 0x44, size 0x4
    s32 polResult;     // offset 0x48, size 0x4
    s32 polNumber;     // offset 0x4C, size 0x4
    u8 File_Type;      // offset 0x50, size 0x1
    u8* File_Name;     // offset 0x54, size 0x4
    u32 Save_Size;     // offset 0x58, size 0x4
    u16 Block_Size;    // offset 0x5C, size 0x2
    u8 Icon_Type;      // offset 0x5E, size 0x1
    u8 Comment_Type;   // offset 0x5F, size 0x1
    u8 Target_Number;  // offset 0x60, size 0x1
    u8 Number;         // offset 0x61, size 0x1
    u8 Counter;        // offset 0x62, size 0x1
    u8 Save_Type;      // offset 0x63, size 0x1
    s8 New_File;       // offset 0x64, size 0x1
    s8 Header_Counter; // offset 0x65, size 0x1
    s8 padding[3];     // offset 0x66, size 0x3
};

typedef struct {
    u8 unit;
    u8 flag;
    s8 power;
    u8 freq;
} PULPARA;

typedef struct {
    u16 low;
    u16 hi;
} PUL_UNI_HILO;

typedef union {
    s32 cal;
    PUL_UNI_HILO num;
} PUL_UNION;

typedef struct {
    s16 pow_ans;
    s16 tim_ans;
    s32 rc_step;
    PUL_UNION ix;
} PUL;

typedef struct /* @anon14 */ {
    // total size: 0x4
    s16 ix;    // offset 0x0, size 0x2
    s16 timer; // offset 0x2, size 0x2
} PPWORK_SUB_SUB;

typedef struct {
    u8 ppnew;
    u8 free;
    s16 data;
    s16 rno[4];
    s16 life;
    s16 exix;
    const PPWORK_SUB_SUB* padr;
} PPWORK_SUB;

typedef struct {
    // total size: 0x34
    u8 ok_dev;       // offset 0x0, size 0x1
    u8 id;           // offset 0x1, size 0x1
    u8 opck;         // offset 0x2, size 0x1
    u8 psix;         // offset 0x3, size 0x1
    s16 vital;       // offset 0x4, size 0x2
    u8 inStop;       // offset 0x6, size 0x1
    u8 free;         // offset 0x7, size 0x1
    u32 port;        // offset 0x8, size 0x4
    PPWORK_SUB p[2]; // offset 0xC, size 0x28
} PPWORK;

typedef struct {
    s16 prio;
    s16 rno;
    const PPWORK_SUB_SUB* adrs;
} PULREQ;

typedef struct {
    // total size: 0x8
    s16 x; // offset 0x0, size 0x2
    s16 y; // offset 0x2, size 0x2
    u32 c; // offset 0x4, size 0x4
} Pixel;

typedef struct {
    // total size: 0xC
    f32 x; // offset 0x0, size 0x4
    f32 y; // offset 0x4, size 0x4
    f32 z; // offset 0x8, size 0x4
} Vec3;

typedef struct {
    // total size: 0x10
    f32 x; // offset 0x0, size 0x4
    f32 y; // offset 0x4, size 0x4
    f32 z; // offset 0x8, size 0x4
    f32 w; // offset 0xC, size 0x4
} Vec4;

typedef struct {
    // total size: 0x8
    s16 x1; // offset 0x0, size 0x2
    s16 y1; // offset 0x2, size 0x2
    s16 x2; // offset 0x4, size 0x2
    s16 y2; // offset 0x6, size 0x2
} Rect;

typedef struct {
    // total size: 0x40
    f32 _11; // offset 0x0, size 0x4
    f32 _12; // offset 0x4, size 0x4
    f32 _13; // offset 0x8, size 0x4
    f32 _14; // offset 0xC, size 0x4
    f32 _21; // offset 0x10, size 0x4
    f32 _22; // offset 0x14, size 0x4
    f32 _23; // offset 0x18, size 0x4
    f32 _24; // offset 0x1C, size 0x4
    f32 _31; // offset 0x20, size 0x4
    f32 _32; // offset 0x24, size 0x4
    f32 _33; // offset 0x28, size 0x4
    f32 _34; // offset 0x2C, size 0x4
    f32 _41; // offset 0x30, size 0x4
    f32 _42; // offset 0x34, size 0x4
    f32 _43; // offset 0x38, size 0x4
    f32 _44; // offset 0x3C, size 0x4
} Matrix;

typedef union {
    f32 f[16];   // offset 0x0, size 0x40
    f32 a[4][4]; // offset 0x0, size 0x40
    Matrix m;    // offset 0x0, size 0x40
} MTX;

typedef struct {
    // total size: 0xC
    s8 ok;                   // offset 0x0, size 0x1
    s8 type;                 // offset 0x1, size 0x1
    s16 key;                 // offset 0x2, size 0x2
    uintptr_t texture_table; // offset 0x4, size 0x4
    uintptr_t trans_table;   // offset 0x8, size 0x4
} TEX_GRP_LD;

typedef struct {
    // total size: 0x5
    u8 wh;     // offset 0x0, size 0x1
    u8 dat[4]; // offset 0x1, size 0x4
} TEX;

typedef struct {
    // total size: 0x28
    u8 be;           // offset 0x0, size 0x1
    u8 type;         // offset 0x1, size 0x1
    s16 id;          // offset 0x2, size 0x2
    u8 rno;          // offset 0x4, size 0x1
    u8 retry;        // offset 0x5, size 0x1
    u8 ix;           // offset 0x6, size 0x1
    u8 frre;         // offset 0x7, size 0x1
    s16 key;         // offset 0x8, size 0x2
    u8 kokey;        // offset 0xA, size 0x1
    u8 group;        // offset 0xB, size 0x1
    u8* result;      // offset 0xC, size 0x4
    s32 size;        // offset 0x10, size 0x4
    s32 sect;        // offset 0x14, size 0x4
    u16 fnum;        // offset 0x18, size 0x2
    u8 free[2];      // offset 0x1A, size 0x2
    TEX_GRP_LD* lds; // offset 0x1C, size 0x4
    struct {
        // total size: 0x8
        u32 number; // offset 0x0, size 0x4
        u32 size;   // offset 0x4, size 0x4
    } info;         // offset 0x20, size 0x8
} REQ;

struct _cursor_infor {
    // total size: 0x2
    u8 first_x; // offset 0x0, size 0x1
    u8 first_y; // offset 0x1, size 0x1
};

typedef struct {
    // total size: 0x18
    u8 ok[20];                            // offset 0x0, size 0x14
    struct _cursor_infor cursor_infor[2]; // offset 0x14, size 0x4
} Permission;

typedef struct {
    // total size: 0x30
    s16 data[4][6]; // offset 0x0, size 0x30
} UNK_Data;

typedef struct {
    u32* nmca;
    u32* dmca;
    u32* btca;
    u32* caca;
    u32* cuca;
    u32* atca;
    u32* saca;
    u32* exca;
    u32* cbca;
    u32* yuca;
    s16* stxy;
    s16* mvxy;
    u32* sernd;
    UNK_8* ovct;
    UNK_9* ovix;
    CatchTable* rict;
    UNK_0* hiit;
    UNK_1* boda;
    UNK_2* hana;
    UNK_3* cata;
    UNK_4* caua;
    UNK_5* atta;
    UNK_6* hosa;
    UNK_7* atit;
    UNK_Data* prot;
} CharInitData;

typedef struct {
    // total size: 0x8
    s16 my_cm; // offset 0x0, size 0x2
    s16 my_cc; // offset 0x2, size 0x2
    s16 my_pr; // offset 0x4, size 0x2
    s16 my_fm; // offset 0x6, size 0x2
} CharInitData2;

typedef struct {
    // total size: 0xC
    uintptr_t adr;  // offset 0x0, size 0x4
    size_t size;    // offset 0x4, size 0x4
    u8 search_type; // offset 0x8, size 0x1
    u8 group_num;   // offset 0x9, size 0x1
    u8 type;        // offset 0xA, size 0x1
    u8 use;         // offset 0xB, size 0x1
} RCKeyWork;

typedef struct {
    // total size: 0x32
    s8 type;         // offset 0x0, size 0x1
    s8 form;         // offset 0x1, size 0x1
    s8 end_flag[4];  // offset 0x2, size 0x4
    s8 dmm;          // offset 0x6, size 0x1
    s16 id;          // offset 0x8, size 0x2
    s16 r_no_0;      // offset 0xA, size 0x2
    s16 r_no_1;      // offset 0xC, size 0x2
    u8 rank_in;      // offset 0xE, size 0x1
    s16 rank_status; // offset 0x10, size 0x2
    s16 rank;        // offset 0x12, size 0x2
    s16 status;      // offset 0x14, size 0x2
    s16 index;       // offset 0x16, size 0x2
    s16 timer;       // offset 0x18, size 0x2
    s16 code[4];     // offset 0x1A, size 0x8
    s16 old_code[4]; // offset 0x22, size 0x8
    s16 count1[2];   // offset 0x2A, size 0x4
    s8 count2[2];    // offset 0x2E, size 0x2
    s16 wait_cnt;    // offset 0x30, size 0x2
} NAME_WK;

typedef struct {
    // total size: 0xC
    s8 type;           // offset 0x0, size 0x1
    s8 n_disp_flag;    // offset 0x1, size 0x1
    s16 c_cnt;         // offset 0x2, size 0x2
    s16 r_no_0;        // offset 0x4, size 0x2
    s16 r_no_1;        // offset 0x6, size 0x2
    s16 f_cnt;         // offset 0x8, size 0x2
    u8 tenmetsu_flag;  // offset 0xA, size 0x1
    u8 tenmetsu_place; // offset 0xB, size 0x1
} SC_NAME_WK;

typedef struct {
    // total size: 0x4
    s8 code[4]; // offset 0x0, size 0x4
} RANK_NAME_W;

typedef struct {
    // total size: 0x14
    s8 be;        // offset 0x0, size 0x1
    u8 c_mode;    // offset 0x1, size 0x1
    u16 total;    // offset 0x2, size 0x2
    u16* handle;  // offset 0x4, size 0x4
    s32 ixNum1st; // offset 0x8, size 0x4
    u8* srcAdrs;  // offset 0xC, size 0x4
    u32 srcSize;  // offset 0x10, size 0x4
} Palette;

typedef union {
    u32 b32;    // offset 0x0, size 0x4
    u16 b16[2]; // offset 0x0, size 0x4
    u8 b8[4];   // offset 0x0, size 0x4
} TextureHandle;

typedef struct {
    // total size: 0x20
    s8 be;                 // offset 0x0, size 0x1
    u8 flags;              // offset 0x1, size 0x1
    s16 arCnt;             // offset 0x2, size 0x2
    s16 arInit;            // offset 0x4, size 0x2
    u16 total;             // offset 0x6, size 0x2
    TextureHandle* handle; // offset 0x8, size 0x4
    s32 ixNum1st;          // offset 0xC, size 0x4
    u16 textures;          // offset 0x10, size 0x2
    u16 accnum;            // offset 0x12, size 0x2
    u32* offset;           // offset 0x14, size 0x4
    u8* srcAdrs;           // offset 0x18, size 0x4
    size_t srcSize;        // offset 0x1C, size 0x4
} Texture;

typedef struct {
    // total size: 0x8
    Texture* tex; // offset 0x0, size 0x4
    Palette* pal; // offset 0x4, size 0x4
} PPGDataList;

typedef struct {
    // total size: 0x10
    u32 magic;     // offset 0x0, size 0x4
    u32 fileSize;  // offset 0x4, size 0x4
    u8 width;      // offset 0x8, size 0x1
    u8 height;     // offset 0x9, size 0x1
    u8 compress;   // offset 0xA, size 0x1
    u8 pixel;      // offset 0xB, size 0x1
    u16 formARGB;  // offset 0xC, size 0x2
    u16 transNums; // offset 0xE, size 0x2
} PPGFileHeader;

typedef struct {
    // total size: 0x10
    u32 magic;    // offset 0x0, size 0x4
    u32 fileSize; // offset 0x4, size 0x4
    u16 id;       // offset 0x8, size 0x2
    u8 compress;  // offset 0xA, size 0x1
    u8 free;      // offset 0xB, size 0x1
    u32 expSize;  // offset 0xC, size 0x4
} PPXFileHeader;

typedef struct {
    // total size: 0x10
    u32 magic;    // offset 0x0, size 0x4
    u32 fileSize; // offset 0x4, size 0x4
    u16 free;     // offset 0x8, size 0x2
    u8 compress;  // offset 0xA, size 0x1
    u8 c_mode;    // offset 0xB, size 0x1
    u16 formARGB; // offset 0xC, size 0x2
    u16 palettes; // offset 0xE, size 0x2
} PPLFileHeader;

typedef struct {
    // total size: 0x18
    f32 x;   // offset 0x0, size 0x4
    f32 y;   // offset 0x4, size 0x4
    f32 z;   // offset 0x8, size 0x4
    f32 u;   // offset 0xC, size 0x4
    f32 v;   // offset 0x10, size 0x4
    u32 col; // offset 0x14, size 0x4
} Polygon;

typedef struct {
    // total size: 0x14
    f32 x; // offset 0x0, size 0x4
    f32 y; // offset 0x4, size 0x4
    f32 z; // offset 0x8, size 0x4
    f32 s; // offset 0xC, size 0x4
    f32 t; // offset 0x10, size 0x4
} Vertex;

typedef struct {
    // total size: 0x30
    Vec3 v[4]; // offset 0x0, size 0x30
} Quad;

typedef struct {
    // total size: 0x8
    f32 s; // offset 0x0, size 0x4
    f32 t; // offset 0x4, size 0x4
} TexCoord;

typedef struct {
    // total size: 0x54
    Vec3 v[4];     // offset 0x0, size 0x30
    TexCoord t[4]; // offset 0x30, size 0x20
    u32 texCode;   // offset 0x50, size 0x4
} Sprite;

typedef struct {
    // total size: 0x34
    Vec3 v[2];     // offset 0x0, size 0x18
    TexCoord t[2]; // offset 0x18, size 0x10
    u32 vtxColor;  // offset 0x28, size 0x4
    u32 texCode;   // offset 0x2C, size 0x4
    u32 id;        // offset 0x30, size 0x4
} Sprite2;

typedef struct {
    // total size: 0x20
    Sprite2* chip; // offset 0x0, size 0x4
    u16 sprTotal;  // offset 0x4, size 0x2
    u16 sprMax;    // offset 0x6, size 0x2
    s8 up[24];     // offset 0x8, size 0x18
} SpriteChipSet;

typedef struct {
    // total size: 0x14
    u16 num_of_1st; // offset 0x0, size 0x2
    s16 apfn;       // offset 0x2, size 0x2
    s16 conv;       // offset 0x4, size 0x2
    s16 ix1st;      // offset 0x6, size 0x2
    u32 use;        // offset 0x8, size 0x4
    u32 to_tex;     // offset 0xC, size 0x4
    u32 to_chd;     // offset 0x10, size 0x4
} TexGroupData;

typedef struct {
    // total size: 0x8
    f32 x; // offset 0x0, size 0x4
    f32 y; // offset 0x4, size 0x4
} PAL_CURSOR_P;

typedef union {
    u32 color; // offset 0x0, size 0x4
    struct {
        // total size: 0x4
        s16 u; // offset 0x0, size 0x2
        s16 v; // offset 0x2, size 0x2
    } tex;     // offset 0x0, size 0x4
    struct {
        // total size: 0x4
        u8 b; // offset 0x0, size 0x1
        u8 g; // offset 0x1, size 0x1
        u8 r; // offset 0x2, size 0x1
        u8 a; // offset 0x3, size 0x1
    } argb;   // offset 0x0, size 0x4
} PAL_CURSOR_COL;

typedef struct {
    // total size: 0x30
    PAL_CURSOR_P pal_cursor_p[4];     // offset 0x0, size 0x20
    PAL_CURSOR_COL pal_cursor_col[4]; // offset 0x20, size 0x10
} PAL_CURSOR_TBL;

typedef struct {
    // total size: 0x10
    PAL_CURSOR_P* p;     // offset 0x0, size 0x4
    PAL_CURSOR_COL* col; // offset 0x4, size 0x4
    PAL_CURSOR_COL* tex; // offset 0x8, size 0x4
    u32 num;             // offset 0xC, size 0x4
} PAL_CURSOR;

typedef union {
    s32 full; // offset 0x0, size 0x4
    struct {
        // total size: 0x4
        u8 B; // offset 0x0, size 0x1
        u8 G; // offset 0x1, size 0x1
        u8 R; // offset 0x2, size 0x1
        u8 A; // offset 0x3, size 0x1
    } rgb;    // offset 0x0, size 0x4
} OPTW_Color;

typedef struct {
    // total size: 0x20
    u32 g_no;       // offset 0x0, size 0x4
    u16 hv;         // offset 0x4, size 0x2
    s16 off_x;      // offset 0x6, size 0x2
    s16 off_y;      // offset 0x8, size 0x2
    f32 zx;         // offset 0xC, size 0x4
    f32 zy;         // offset 0x10, size 0x4
    s32 prio;       // offset 0x14, size 0x4
    s32 trans;      // offset 0x18, size 0x4
    OPTW_Color col; // offset 0x1C, size 0x4
} OPTW;

typedef struct {
    // total size: 0x10
    u32 g_no;       // offset 0x0, size 0x4
    s32 trans;      // offset 0x4, size 0x4
    u16 hv;         // offset 0x8, size 0x2
    s16 ok;         // offset 0xA, size 0x2
    OPTW_Color col; // offset 0xC, size 0x4
} OPTW_Small;

typedef struct {
    // total size: 0x10C
    s8 r_no_0;            // offset 0x0, size 0x1
    s8 r_no_1;            // offset 0x1, size 0x1
    s8 dir;               // offset 0x2, size 0x1
    s8 ctr;               // offset 0x3, size 0x1
    s16 bg_no;            // offset 0x4, size 0x2
    u16 blk_no;           // offset 0x6, size 0x2
    s32 prio;             // offset 0x8, size 0x4
    OPTW_Small map[4][4]; // offset 0xC, size 0x100
} OPBW;

typedef struct {
    // total size: 0x330
    s8 r_no_0;     // offset 0x0, size 0x1
    s8 r_no_1;     // offset 0x1, size 0x1
    s8 r_no_2;     // offset 0x2, size 0x1
    s8 old_rno;    // offset 0x3, size 0x1
    s16 index;     // offset 0x4, size 0x2
    s16 mv_ctr;    // offset 0x6, size 0x2
    s16 free_work; // offset 0x8, size 0x2
    s16 dummy;     // offset 0xA, size 0x2
    OPBW bgw[3];   // offset 0xC, size 0x324
} OP_W;

typedef struct {
    // total size: 0xD08
    s32 x16;            // offset 0x0, size 0x4
    s32 x32;            // offset 0x4, size 0x4
    u16 x16_free[1024]; // offset 0x8, size 0x800
    u16 x32_free[640];  // offset 0x808, size 0x500
} TexturePoolFree;

typedef struct {
    // total size: 0xD08
    s32 x16;            // offset 0x0, size 0x4
    s32 x32;            // offset 0x4, size 0x4
    u16 x16_used[1024]; // offset 0x8, size 0x800
    u16 x32_used[640];  // offset 0x808, size 0x500
} TexturePoolUsed;

typedef struct {
    // total size: 0xD0
    u16 x16_map[4][16]; // offset 0x0, size 0x80
    u8 x32_map[10][8];  // offset 0x80, size 0x50
} PatternMap;

typedef union {
    u32 code; // offset 0x0, size 0x4
    struct {
        // total size: 0x4
        u16 offset; // offset 0x0, size 0x2
        u16 group;  // offset 0x2, size 0x2
    } parts;        // offset 0x0, size 0x4
} PatternCode;

typedef struct {
    // total size: 0x8
    s16 time;       // offset 0x0, size 0x2
    s16 state;      // offset 0x2, size 0x2
    PatternCode cs; // offset 0x4, size 0x4
} PatternState;

typedef struct {
    // total size: 0xDC
    s16 curr_disp;  // offset 0x0, size 0x2
    s16 time;       // offset 0x2, size 0x2
    PatternCode cg; // offset 0x4, size 0x4
    s16 x16;        // offset 0x8, size 0x2
    s16 x32;        // offset 0xA, size 0x2
    PatternMap map; // offset 0xC, size 0xD0
} PatternInstance;

typedef struct {
    // total size: 0x3804
    s16 kazu;                 // offset 0x0, size 0x2
    PatternInstance* adr[64]; // offset 0x4, size 0x100
    PatternInstance patt[64]; // offset 0x104, size 0x3700
} PatternCollection;

typedef struct {
    // total size: 0x64
    s32 mltnum16;            // offset 0x0, size 0x4
    s32 mltnum32;            // offset 0x4, size 0x4
    s32 mltnum;              // offset 0x8, size 0x4
    s32 mltgidx16;           // offset 0xC, size 0x4
    s32 mltgidx32;           // offset 0x10, size 0x4
    s32 mltcshtime16;        // offset 0x14, size 0x4
    s32 mltcshtime32;        // offset 0x18, size 0x4
    PatternState* mltcsh16;  // offset 0x1C, size 0x4
    PatternState* mltcsh32;  // offset 0x20, size 0x4
    u8* mltbuf;              // offset 0x24, size 0x4
    Texture tex;             // offset 0x28, size 0x20
    PPGDataList texList;     // offset 0x48, size 0x8
    u32 attribute;           // offset 0x50, size 0x4
    PatternCollection* cpat; // offset 0x54, size 0x4
    TexturePoolFree* tpf;    // offset 0x58, size 0x4
    TexturePoolUsed* tpu;    // offset 0x5C, size 0x4
    u8 id;                   // offset 0x60, size 0x1
    u8 ext;                  // offset 0x61, size 0x1
    s16 mode;                // offset 0x62, size 0x2
} MultiTexture;

typedef struct {
    // total size: 0x4
    u16 bg_h_shift; // offset 0x0, size 0x2
    u16 bg_v_shift; // offset 0x2, size 0x2
} BackgroundParameters;

union POS_FLOAT {
    s32 long_pos;    // offset 0x0, size 0x4
    LoHi16 word_pos; // offset 0x0, size 0x4
};

typedef struct {
    // total size: 0x10
    union POS_FLOAT scr_x;      // offset 0x0, size 0x4
    union POS_FLOAT scr_x_buff; // offset 0x4, size 0x4
    union POS_FLOAT scr_y;      // offset 0x8, size 0x4
    union POS_FLOAT scr_y_buff; // offset 0xC, size 0x4
} BG_POS;

typedef struct {
    // total size: 0x10
    union POS_FLOAT family_x;      // offset 0x0, size 0x4
    union POS_FLOAT family_x_buff; // offset 0x4, size 0x4
    union POS_FLOAT family_y;      // offset 0x8, size 0x4
    union POS_FLOAT family_y_buff; // offset 0xC, size 0x4
} FM_POS;

typedef struct {
    // total size: 0x14
    u8 bg_num;          // offset 0x0, size 0x1
    const s16* rwd_ptr; // offset 0x4, size 0x4
    const s16* brw_ptr; // offset 0x8, size 0x4
    s16 rw_cnt;         // offset 0xC, size 0x2
    s16 rwgbix;         // offset 0xE, size 0x2
    s16 gbix;           // offset 0x10, size 0x2
} RW_DATA;

typedef struct {
    // total size: 0x4C
    s16 offence_total;  // offset 0x0, size 0x2
    s16 defence_total;  // offset 0x2, size 0x2
    s16 tech_pts_total; // offset 0x4, size 0x2
    s16 ex_point_total; // offset 0x6, size 0x2
    s16 em_stun;        // offset 0x8, size 0x2
    s16 max_combo;      // offset 0xA, size 0x2
    s16 clean_hits;     // offset 0xC, size 0x2
    s16 att_renew;      // offset 0xE, size 0x2
    s16 guard_succ;     // offset 0x10, size 0x2
    s16 vitality;       // offset 0x12, size: 0x2
    s16 nml_blocking;   // offset 0x14, size: 0x2
    s16 rpd_blocking;   // offset 0x16, size: 0x2
    s16 grd_blocking;   // offset 0x18, size: 0x2
    s16 def_free;       // offset 0x1A, size: 0x2
    s16 first_attack;   // offset 0x1C, size: 0x2
    s16 leap_attack;    // offset 0x1E, size: 0x2
    s16 target_combo;   // offset 0x20, size: 0x2
    s16 nml_nage;       // offset 0x22, size: 0x2
    s16 grap_def;       // offset 0x24, size: 0x2
    s16 quick_stand;    // offset 0x26, size: 0x2
    s16 personal_act;   // offset 0x28, size: 0x2
    s16 reversal;       // offset 0x2A, size: 0x2
    s16 comwaza;        // offset 0x2C, size: 0x2
    s16 sa_exec;        // offset 0x2E, size: 0x2
    s16 tairyokusa;     // offset 0x30, size: 0x2
    s16 kimarite;       // offset 0x32, size: 0x2
    s16 renshou;        // offset 0x34, size: 0x2
    s16 em_renshou;     // offset 0x36, size: 0x2
    s16 app_nml_block;  // offset 0x38, size: 0x2
    s16 app_rpd_block;  // offset 0x3A, size: 0x2
    s16 app_grd_block;  // offset 0x3C, size: 0x2
    s16 onaji_waza;     // offset 0x3E, size: 0x2
    s16 grd_miss;       // offset 0x40, size: 0x2
    s16 grd_mcnt;       // offset 0x42, size: 0x2
    s16 grade;          // offset 0x44, size: 0x2
    s16 round;          // offset 0x46, size: 0x2
    s16 win_round;      // offset 0x48, size: 0x2
    s16 no_lose;        // offset 0x4A, size: 0x2
} GradeData;

typedef struct {
    // total size: 0xAC
    s16 vs_cpu_result[16];  // offset 0x0, size 0x20
    s16 vs_cpu_grade[16];   // offset 0x20, size 0x20
    s16 vs_cpu_player[16];  // offset 0x40, size 0x20
    s16 vcr_ix;             // offset 0x60, size 0x2
    s16 grade;              // offset 0x62, size 0x2
    s16 all_clear;          // offset 0x64, size 0x2
    s16 keizoku;            // offset 0x66, size 0x2
    s16 sp_point;           // offset 0x68, size 0x2
    s16 fr_ix;              // offset 0x6A, size 0x2
    u8 fr_sort_data[16][4]; // offset 0x6C, size 0x40
} GradeFinalData;

typedef struct {
    // total size: 0x8
    s16 x;    // offset 0x0, size 0x2
    s16 y;    // offset 0x2, size 0x2
    u16 attr; // offset 0x4, size 0x2
    u16 code; // offset 0x6, size 0x2
} TileMapEntry;

typedef struct {
    // total size: 0x8
    s16 ptix; // offset 0x0, size 0x2
    s16 bank; // offset 0x2, size 0x2
    s16 port; // offset 0x4, size 0x2
    s16 code; // offset 0x6, size 0x2
} SoundPatchConfig;

typedef struct {
    // total size: 0xC
    u16 cp3code;          // offset 0x0, size 0x2
    u16 free;             // offset 0x2, size 0x2
    SoundPatchConfig rmc; // offset 0x4, size 0x8
} SoundPatch;             // Tentative name

typedef struct {
    // total size: 0x8
    s16 req;  // offset 0x0, size 0x2
    s16 kind; // offset 0x2, size 0x2
    s16 data; // offset 0x4, size 0x2
    s16 code; // offset 0x6, size 0x2
} BGMRequest;

typedef struct {
    // total size: 0x16
    s16 kind;        // offset 0x0, size 0x2
    s16 rno;         // offset 0x2, size 0x2
    s16 code;        // offset 0x4, size 0x2
    s16 timer;       // offset 0x6, size 0x2
    s16 data;        // offset 0x8, size 0x2
    s16 volume;      // offset 0xA, size 0x2
    s16 state;       // offset 0xC, size 0x2
    u16 ownData;     // offset 0xE, size 0x2
    u16 nowSeamless; // offset 0x10, size 0x2
    u16 exEntry;     // offset 0x12, size 0x2
    u16 exIndex;     // offset 0x14, size 0x2
} BGMExecution;

typedef struct {
    // total size: 0x8
    u16 numStart; // offset 0x0, size 0x2
    u16 numEnd;   // offset 0x2, size 0x2
    u16 numLoop;  // offset 0x4, size 0x2
    s16 free;     // offset 0x6, size 0x2
} BGMExecutionData;

typedef struct {
    // total size: 0x8
    union {
        s32 cal; // offset 0x0, size 0x4
        struct {
            // total size: 0x4
            s16 low; // offset 0x0, size 0x2
            s16 hi;  // offset 0x2, size 0x2
        } dex;       // offset 0x0, size 0x4
    } in;            // offset 0x0, size 0x4
    s32 speed;       // offset 0x4, size 0x4
} BGMFade;

typedef struct {
    // total size: 0x8
    u16 data; // offset 0x0, size 0x2
    s16 vol;  // offset 0x2, size 0x2
    s32 fnum; // offset 0x4, size 0x4
} BGMTableEntry;

typedef struct {
    // total size: 0x20
    u8 cmd;       // offset 0x0, size 0x1
    u8 flags;     // offset 0x1, size 0x1
    u8 prog;      // offset 0x2, size 0x1
    u8 note;      // offset 0x3, size 0x1
    u8 attr;      // offset 0x4, size 0x1
    u8 vol;       // offset 0x5, size 0x1
    u8 pan;       // offset 0x6, size 0x1
    s16 pitch;    // offset 0x8, size 0x2
    u8 prio;      // offset 0xA, size 0x1
    u8 id1;       // offset 0xB, size 0x1
    u8 id2;       // offset 0xC, size 0x1
    u32 kofftime; // offset 0x10, size 0x4
    u8 limit;     // offset 0x14, size 0x1
    u16 param0;   // offset 0x16, size 0x2
    u16 param1;   // offset 0x18, size 0x2
    u16 param2;   // offset 0x1A, size 0x2
    u16 param3;   // offset 0x1C, size 0x2
    u16 link;     // offset 0x1E, size 0x2
} SoundEvent;     // Tentative name

typedef struct {
    // total size: 0x20
    u32 tag;             // offset 0x0, size 0x4
    u32 chunkSize;       // offset 0x4, size 0x4
    u32 version;         // offset 0x8, size 0x4
    u32 headerSize;      // offset 0xC, size 0x4
    u32 bodySize;        // offset 0x10, size 0x4
    u32 progChunkOffset; // offset 0x14, size 0x4
    u32 smplChunkOffset; // offset 0x18, size 0x4
    u32 vagiChunkOffset; // offset 0x1C, size 0x4
} _ps2_head_chunk;

typedef struct {
    // total size: 0x10
    u32 vagOffset;  // offset 0x0, size 0x4
    u32 vagSize;    // offset 0x4, size 0x4
    s32 loopFlag;   // offset 0x8, size 0x4
    s32 sampleRate; // offset 0xC, size 0x4
} _ps2_vagi_param;

typedef union {
    u8 core_0; // offset 0x0, size 0x1
    u8 core_1; // offset 0x0, size 0x1
    u8 core;   // offset 0x0, size 0x1
} _ps2_effect;

typedef struct {
    // total size: 0xE
    u8 prio;            // offset 0x0, size 0x1
    _ps2_effect effect; // offset 0x1, size 0x1
    u8 lowKey;          // offset 0x2, size 0x1
    u8 highKey;         // offset 0x3, size 0x1
    u16 bendLow;        // offset 0x4, size 0x2
    u16 bendHigh;       // offset 0x6, size 0x2
    s8 vol;             // offset 0x8, size 0x1
    s8 pan;             // offset 0x9, size 0x1
    s8 trans;           // offset 0xA, size 0x1
    s8 fine;            // offset 0xB, size 0x1
    u16 sampleIndex;    // offset 0xC, size 0x2
} _ps2_split_block;

typedef struct {
    // total size: 0x8
    u8 nSplit;                      // offset 0x0, size 0x1
    _ps2_effect effect;             // offset 0x1, size 0x1
    s8 vol;                         // offset 0x2, size 0x1
    s8 pan;                         // offset 0x3, size 0x1
    s8 trans;                       // offset 0x4, size 0x1
    s8 fine;                        // offset 0x5, size 0x1
    u16 reserved;                   // offset 0x6, size 0x2
    _ps2_split_block splitBlock[0]; // offset 0x8, size 0x0
} _ps2_prog_param;

typedef struct {
    // total size: 0x10
    u32 tag;                // offset 0x0, size 0x4
    u32 chunkSize;          // offset 0x4, size 0x4
    u32 maxProgNum;         // offset 0x8, size 0x4
    u32 reserved;           // offset 0xC, size 0x4
    u32 progParamOffset[0]; // offset 0x10, size 0x0
} _ps2_prog_chunk;

typedef struct {
    // total size: 0xC
    u16 ADSR1;          // offset 0x0, size 0x2
    u16 ADSR2;          // offset 0x2, size 0x2
    _ps2_effect effect; // offset 0x4, size 0x1
    u8 base;            // offset 0x5, size 0x1
    s8 vol;             // offset 0x6, size 0x1
    s8 pan;             // offset 0x7, size 0x1
    s8 trans;           // offset 0x8, size 0x1
    s8 fine;            // offset 0x9, size 0x1
    u16 vagiIndex;      // offset 0xA, size 0x2
} _ps2_smpl_param;

typedef struct {
    // total size: 0x10
    u32 tag;                      // offset 0x0, size 0x4
    u32 chunkSize;                // offset 0x4, size 0x4
    u32 maxVagInfoNum;            // offset 0x8, size 0x4
    u32 reserved;                 // offset 0xC, size 0x4
    _ps2_vagi_param vagiParam[0]; // offset 0x10, size 0x0
} _ps2_vagi_chunk;

typedef struct {
    // total size: 0x10
    u32 tag;                      // offset 0x0, size 0x4
    u32 chunkSize;                // offset 0x4, size 0x4
    u32 maxSmplNum;               // offset 0x8, size 0x4
    u32 reserved;                 // offset 0xC, size 0x4
    _ps2_smpl_param smplParam[0]; // offset 0x10, size 0x0
} _ps2_smpl_chunk;

typedef struct {
    // total size: 0x6
    u16 flags; // offset 0x0, size 0x2
    u8 prio;   // offset 0x2, size 0x1
    u8 id1;    // offset 0x3, size 0x1
    u8 id2;    // offset 0x4, size 0x1
} CSE_COND;

typedef struct {
    // total size: 0x1C
    u16 flags;       // offset 0x0, size 0x2
    u8 attr;         // offset 0x2, size 0x1
    u8 prio;         // offset 0x3, size 0x1
    u8 bank;         // offset 0x4, size 0x1
    u8 note;         // offset 0x5, size 0x1
    u8 id1;          // offset 0x6, size 0x1
    u8 id2;          // offset 0x7, size 0x1
    s16 vol;         // offset 0x8, size 0x2
    s16 pan;         // offset 0xA, size 0x2
    s16 pitch;       // offset 0xC, size 0x2
    s16 bend;        // offset 0xE, size 0x2
    u8 limit;        // offset 0x10, size 0x1
    u8 ___dummy___0; // offset 0x11, size 0x1
    u8 ___dummy___1; // offset 0x12, size 0x1
    u8 ___dummy___2; // offset 0x13, size 0x1
    u32 kofftime;    // offset 0x14, size 0x4
    u32 guid;        // offset 0x18, size 0x4
} CSE_REQP;

typedef struct {
    // total size: 0x14
    u8 vol;       // offset 0x0, size 0x1
    u8 pan;       // offset 0x1, size 0x1
    s16 pitch;    // offset 0x2, size 0x2
    u16 bendLow;  // offset 0x4, size 0x2
    u16 bendHigh; // offset 0x6, size 0x2
    u16 adsr1;    // offset 0x8, size 0x2
    u16 adsr2;    // offset 0xA, size 0x2
    u32 freq;     // offset 0xC, size 0x4
    u32 s_addr;   // offset 0x10, size 0x4
} CSE_PHDP;

typedef struct {
    // total size: 0x10
    _ps2_prog_param* pPprm;  // offset 0x0, size 0x4
    _ps2_split_block* pSblk; // offset 0x4, size 0x4
    _ps2_smpl_param* pSprm;  // offset 0x8, size 0x4
    _ps2_vagi_param* pVprm;  // offset 0xC, size 0x4
} CSE_PHDPADDR;

typedef struct {
    // total size: 0x8
    s32 result; // offset 0x0, size 0x4
    u32 guid;   // offset 0x4, size 0x4
    u8 data[0]; // offset 0x8, size 0x0
} CSE_RPCQUEUE_RESULT;

typedef struct {
    // total size: 0x38
    u16 BeFlag;       // offset 0x0, size 0x2
    u8 Bank;          // offset 0x2, size 0x1
    u8 Code;          // offset 0x3, size 0x1
    u16 Interval;     // offset 0x4, size 0x2
    u16 Times;        // offset 0x6, size 0x2
    u16 VolDec1st;    // offset 0x8, size 0x2
    u16 VolDec;       // offset 0xA, size 0x2
    u16 CurrInterval; // offset 0xC, size 0x2
    u16 CurrTimes;    // offset 0xE, size 0x2
    s32 Rtpc[10];     // offset 0x10, size 0x28
} CSE_ECHOWORK;

typedef struct {
    // total size: 0xC
    u16 x;    // offset 0x0, size 0x2
    u16 y;    // offset 0x2, size 0x2
    u32 code; // offset 0x4, size 0x4
    u32 col;  // offset 0x8, size 0x4
} RenderBuffer;

typedef struct {
    // total size: 0xB0
    Palette palDC;  // offset 0x0, size 0x14
    Palette palCP3; // offset 0x14, size 0x14
    s16 req[32][2]; // offset 0x28, size 0x80
    s16 reqNum;     // offset 0xA8, size 0x2
    u32 upBits;     // offset 0xAC, size 0x4
} Col3rd_W;

typedef struct {
    // total size: 0xC
    s16 be;    // offset 0x0, size 0x2
    s16 mincg; // offset 0x2, size 0x2
    s16 min16; // offset 0x4, size 0x2
    s16 min32; // offset 0x6, size 0x2
    s16 key0;  // offset 0x8, size 0x2
    s16 key1;  // offset 0xA, size 0x2
} MTS_OK;

typedef union {
    s32 pl;    // offset 0x0, size 0x4
    LoHi16 ps; // offset 0x0, size 0x4
} S32Split;

typedef struct {
    // total size: 0x2C
    s32 timer;  // offset 0x0, size 0x4
    s32 timer2; // offset 0x4, size 0x4
    S32Split x; // offset 0x8, size 0x4
    S32Split y; // offset 0xC, size 0x4
    s32 spx;    // offset 0x10, size 0x4
    s32 dlx;    // offset 0x14, size 0x4
    s32 spy;    // offset 0x18, size 0x4
    s32 dly;    // offset 0x1C, size 0x4
    s32 amx;    // offset 0x20, size 0x4
    s32 amy;    // offset 0x24, size 0x4
    s8 swx;     // offset 0x28, size 0x1
    s8 swy;     // offset 0x29, size 0x1
} MotionState;

typedef struct {
    // total size: 0x10
    s8 contents[2][2][4]; // offset 0x0, size 0x10
} TrainingData;

typedef struct {
    // total size: 0x10
    f32 r; // offset 0x0, size 0x4
    f32 g; // offset 0x4, size 0x4
    f32 b; // offset 0x8, size 0x4
    f32 a; // offset 0xC, size 0x4
} FLColor;

typedef struct {
    // total size: 0xC
    f32 x; // offset 0x0, size 0x4
    f32 y; // offset 0x4, size 0x4
    f32 z; // offset 0x8, size 0x4
} FLVec3;

typedef struct {
    // total size: 0x4C
    u32 desc;         // offset 0x0, size 0x4
    FLColor diffuse;  // offset 0x4, size 0x10
    FLColor specular; // offset 0x14, size 0x10
    FLColor ambient;  // offset 0x24, size 0x10
    FLColor emissive; // offset 0x34, size 0x10
    u32 tex;          // offset 0x44, size 0x4
    s32 power;        // offset 0x48, size 0x4
} FLMaterial;

typedef struct {
    // total size: 0x68
    u32 desc;         // offset 0x0, size 0x4
    FLColor diffuse;  // offset 0x4, size 0x10
    FLColor specular; // offset 0x14, size 0x10
    FLColor ambient;  // offset 0x24, size 0x10
    FLVec3 direction; // offset 0x34, size 0xC
    FLVec3 position;  // offset 0x40, size 0xC
    f32 Attenuation0; // offset 0x4C, size 0x4
    f32 Attenuation1; // offset 0x50, size 0x4
    f32 Attenuation2; // offset 0x54, size 0x4
    f32 Range;        // offset 0x58, size 0x4
    f32 theta;        // offset 0x5C, size 0x4
    f32 phi;          // offset 0x60, size 0x4
    f32 Falloff;      // offset 0x64, size 0x4
} FLLight;

typedef struct {
    // total size: 0x8
    union {
        s32 cal; // offset 0x0, size 0x4
        struct {
            // total size: 0x4
            u16 low; // offset 0x0, size 0x2
            s16 pos; // offset 0x2, size 0x2
        } disp;      // offset 0x0, size 0x4
    } iw[2];         // offset 0x0, size 0x8
} Ideal_W;

typedef struct {
    // total size: 0x14
    u8 name[3];      // offset 0x0, size 0x3
    u16 player;      // offset 0x4, size 0x2
    u32 score;       // offset 0x8, size 0x4
    s8 cpu_grade;    // offset 0xC, size 0x1
    s8 grade;        // offset 0xD, size 0x1
    u16 wins;        // offset 0xE, size 0x2
    u8 player_color; // offset 0x10, size 0x1
    u8 all_clear;    // offset 0x11, size 0x1
} ScoreRankingEntry;

typedef struct {
    // total size: 0x48
    s8 contents[10][7]; // offset 0x0, size 0x46
    u16 sum;            // offset 0x46, size 0x2
} DirData;

typedef struct {
    // total size: 0xA
    u8 max_hitcombo;       // offset 0x0, size 0x1
    u8 new_max_flag;       // offset 0x1, size 0x1
    u8 frash_flag;         // offset 0x2, size 0x1
    u8 frash_switch;       // offset 0x3, size 0x1
    s16 damage;            // offset 0x4, size 0x2
    s16 total_damage;      // offset 0x6, size 0x2
    s16 disp_total_damage; // offset 0x8, size 0x2
} TrainingData2;

typedef struct {
    // total size: 0x10
    s16 pos_x;  // offset 0x0, size 0x2
    s16 pos_y;  // offset 0x2, size 0x2
    s16 pos_z;  // offset 0x4, size 0x2
    u16 cg_num; // offset 0x6, size 0x2
    s16 renew;  // offset 0x8, size 0x2
    u16 hit_ix; // offset 0xA, size 0x2
    s8 flip;    // offset 0xC, size 0x1
    u8 cg_flp;  // offset 0xD, size 0x1
    s16 kowaza; // offset 0xE, size 0x2
} ZanzouTableEntry;

typedef struct {
    s32 x;      // offset 0x0, size 0x4
    s32 y;      // offset 0x4, size 0x4
    s32 width;  // offset 0x8, size 0x4
    s32 height; // offset 0xC, size 0x4
    f32 min_z;  // offset 0x10, size 0x4
    f32 max_z;  // offset 0x14, size 0x4
} FL_RenderViewport;

typedef struct _anon6 {
    s8 xxxx[8][2][32];
} _anon6;

typedef struct _anon13 {
    s8 zzzz[4][4];
} _anon13;

typedef struct {
    // total size: 0x44
    u16 sw_new;     // offset 0x0, size 0x2
    u16 sw_old;     // offset 0x2, size 0x2
    u16 sw_chg;     // offset 0x4, size 0x2
    u16 sw_now;     // offset 0x6, size 0x2
    u16 old_now;    // offset 0x8, size 0x2
    u16 now_lvbt;   // offset 0xA, size 0x2
    u16 old_lvbt;   // offset 0xC, size 0x2
    u16 new_lvbt;   // offset 0xE, size 0x2
    u16 sw_lever;   // offset 0x10, size 0x2
    u16 shot_up;    // offset 0x12, size 0x2
    u16 shot_down;  // offset 0x14, size 0x2
    u16 shot_ud;    // offset 0x16, size 0x2
    s16 lvr_status; // offset 0x18, size 0x2
    s16 jaku_cnt;   // offset 0x1A, size 0x2
    s16 chuu_cnt;   // offset 0x1C, size 0x2
    s16 kyou_cnt;   // offset 0x1E, size 0x2
    s16 up_cnt;     // offset 0x20, size 0x2
    s16 down_cnt;   // offset 0x22, size 0x2
    s16 left_cnt;   // offset 0x24, size 0x2
    s16 right_cnt;  // offset 0x26, size 0x2
    s16 s1_cnt;     // offset 0x28, size 0x2
    s16 s2_cnt;     // offset 0x2A, size 0x2
    s16 s3_cnt;     // offset 0x2C, size 0x2
    s16 s4_cnt;     // offset 0x2E, size 0x2
    s16 s5_cnt;     // offset 0x30, size 0x2
    s16 s6_cnt;     // offset 0x32, size 0x2
    s16 lu_cnt;     // offset 0x34, size 0x2
    s16 ld_cnt;     // offset 0x36, size 0x2
    s16 ru_cnt;     // offset 0x38, size 0x2
    s16 rd_cnt;     // offset 0x3A, size 0x2
    s16 waza_num;   // offset 0x3C, size 0x2
    s16 waza_no;    // offset 0x3E, size 0x2
    s16 wait_cnt;   // offset 0x40, size 0x2
    s16 cmd_r_no;   // offset 0x42, size 0x2
} T_PL_LVR;

typedef struct /* @anon31 */ {
    // total size: 0x1C
    s16 w_type;  // offset 0x0, size 0x2
    s16 w_int;   // offset 0x2, size 0x2
    s16 free1;   // offset 0x4, size 0x2
    s16 w_lvr;   // offset 0x6, size 0x2
    s16* w_ptr;  // offset 0x8, size 0x4
    s16 free2;   // offset 0xC, size 0x2
    s16 w_dead;  // offset 0xE, size 0x2
    s16 w_dead2; // offset 0x10, size 0x2
    union {
        struct {
            // total size: 0x6
            s16 flag;       // offset 0x0, size 0x2
            s16 shot_flag;  // offset 0x2, size 0x2
            s16 shot_flag2; // offset 0x4, size 0x2
        } tame;             // offset 0x0, size 0x6
        struct {
            // total size: 0x6
            s16 s_cnt; // offset 0x0, size 0x2
            s16 m_cnt; // offset 0x2, size 0x2
            s16 l_cnt; // offset 0x4, size 0x2
        } shot;        // offset 0x0, size 0x6
    } uni0;            // offset 0x12, size 0x6
    s16 free3;         // offset 0x18, size 0x2
    s16 shot_ok;       // offset 0x1A, size 0x2
} WAZA_WORK;

typedef struct {
    // total size: 0x90
    struct {
        // total size: 0x10
        QWORD data; // offset 0x0, size 0x10
    } dmatag;       // offset 0x0, size 0x10

    QWORD vifcode;   // offset 0x10, size 0x10
    QWORD giftag;    // offset 0x20, size 0x10
    QWORD bitbltbuf; // offset 0x30, size 0x10
    QWORD trxpos;    // offset 0x40, size 0x10
    QWORD trxreg;    // offset 0x50, size 0x10
    QWORD finish;    // 0ffset 0x60, size 0x10
    QWORD trxdir;    // offset 0x70, size 0x10
    QWORD data;      // offset 0x80, size 0x10
} FLPS2StoreImageData;

typedef struct {
    // total size: 0x80
    struct {
        // total size: 0x10
        QWORD data; // offset 0x0, size 0x10
    } dmatag;       // offset 0x0, size 0x10

    struct {
        // total size: 0x10
        QWORD data; // offset 0x0, size 0x10
    } giftag;       // offset 0x10, size 0x10

    QWORD bitbltbuf; // offset 0x20, size 0x10
    QWORD trxpos;    // offset 0x30, size 0x10
    QWORD trxreg;    // offset 0x40, size 0x10
    QWORD trxdir;    // offset 0x50, size: 0x10

    struct {
        // total size: 0x10
        QWORD data; // offset 0x0, size 0x10
    } giftag1;      // offset 0x60, size 0x10

    struct {
        // total size: 0x10
        QWORD data; // offset 0x0, size 0x10
    } dmatag1;      // offset 0x70, size 0x10
} FLPS2LoadImageData;

typedef struct {
    // total size: 0x40
    QWORD dmatag;   // offset 0x0, size 0x10
    QWORD directhl; // offset 0x10, size 0x10
    QWORD giftag;   // offset 0x20, size 0x10
    QWORD texflush; // offset 0x30, size 0x10
} FLPS2LoadEndData; // Not Sure about this name

typedef struct {
    // total size: 0xC
    s16 r_no_0;   //  offset 0x0, size 0x2
    s16 r_no_1;   // offset 0x2, size 0x2
    s16 r_no_2;   // offset 0x4, size 0x2
    s16 type;     // offset 0x6, size 0x2
    s16 end_flag; // offset 0x8, size 0x2
    s16 timer;    // offset 0xA, size 0x2
} END_W;

typedef union {
    s32 psi;    // offset 0x0, size 0x4
    LoHi16 pss; // offset 0x0, size 0x4
} MS;

typedef struct {
    // total size: 0x8
    union {
        s16 full; // offset 0x0, size 0x2
        struct {
            // total size: 0x2
            s8 l; // offset 0x0, size 0x1
            s8 h; // offset 0x1, size 0x1
        } half;   // offset 0x0, size 0x2
    } size;       // offset 0x0, size 0x2
    s32 step;     // offset 0x4, size 0x4
} Round_Timer;

typedef struct {
    // total size: 0x1C
    s16 x_pos_num;   // offset 0x0, size 0x2
    s8 routine_num;  // offset 0x2, size 0x1
    u8 hit_hi;       // offset 0x3, size 0x1
    u8 hit_low;      // offset 0x4, size 0x1
    s8 kind;         // offset 0x5, size 0x1
    u32 pts;         // offset 0x8, size 0x4
    s8 pts_digit[4]; // offset 0xC, size 0x4
    s8 pts_flag;     // offset 0x10, size 0x1
    s8 first_digit;  // offset 0x11, size 0x1
    u8 move[2];      // offset 0x12, size 0x2
    u8 x_posnum[2];  // offset 0x14, size 0x2
    s16 timer[2];    // offset 0x16, size 0x4
} CMST_BUFF;

typedef struct {
    // total size: 0x10
    union {
        u16 results; // offset 0x0, size 0x2
        struct {
            // total size: 0x2
            s8 att_result; // offset 0x0, size 0x1
            s8 cat_result; // offset 0x1, size 0x1
        } ca;              // offset 0x0, size 0x2
    } flag;                // offset 0x0, size 0x2
    u8 my_att;             // offset 0x2, size 0x1
    u8 dm_body;            // offset 0x3, size 0x1
    u16 my_hit;            // offset 0x4, size 0x2
    u16 dm_me;             // offset 0x6, size 0x2
    s16* ah;               // offset 0x8, size 0x4
    s16* dh;               // offset 0xC, size 0x4
} HS;

typedef struct {
    // total size: 0x1C
    s16 my_wkid;     // offset 0x0, size 0x2
    u8 waza_num;     // offset 0x2, size 0x1
    u8 vs_refrect;   // offset 0x3, size 0x1
    u16 koa;         // offset 0x4, size 0x2
    u8 kind_of_tama; // offset 0x6, size 0x1
    u8 kage_index;   // offset 0x7, size 0x1
    u8 chix;         // offset 0x8, size 0x1
    u8 ernm;         // offset 0x9, size 0x1
    u8 erht;         // offset 0xA, size 0x1
    u8 erdf;         // offset 0xB, size 0x1
    u8 erex;         // offset 0xC, size 0x1
    u8 col_1p;       // offset 0xD, size 0x1
    u8 col_2p;       // offset 0xE, size 0x1
    u8 data00;       // offset 0xF, size 0x1
    u8 data01;       // offset 0x10, size 0x1
    u8 disp_type;    // offset 0x11, size 0x1
    s16 def_power;   // offset 0x12, size 0x2
    s16 life_time;   // offset 0x14, size 0x2
    s16 hos_x;       // offset 0x16, size 0x2
    s16 hos_y;       // offset 0x18, size 0x2
    u8 kz_blocking;  // offset 0x1A, size 0x1
    u8 free;         // offset 0x1B, size 0x1
} TAMA;

typedef struct {
    // total size: 0x8
    s16 cyerw; // offset 0x0, size 0x2
    s16 cred;  // offset 0x2, size 0x2
    s16 ored;  // offset 0x4, size 0x2
    s8 colnum; // offset 0x6, size 0x1
} VIT;

typedef struct {
    // total size: 0xA
    s16 offence_total;  // offset 0x0, size 0x2
    s16 defence_total;  // offset 0x2, size 0x2
    s16 tech_pts_total; // offset 0x4, size 0x2
    s16 ex_point_total; // offset 0x6, size 0x2
    s16 grade;          // offset 0x8, size 0x2
} JudgeGals;

typedef struct {
    // total size: 0xC
    s16 offence_total;  // offset 0x0, size 0x2
    s16 defence_total;  // offset 0x2, size 0x2
    s16 tech_pts_total; // offset 0x4, size 0x2
    s16 ex_point_total; // offset 0x6, size 0x2
    s16 round;          // offset 0x8, size 0x2
    s16 grade;          // offset 0xA, size 0x2
} JudgeCom;

typedef struct {
    s16 cstn;         // offset 0x0, size 0x2
    s8 sflag;         // offset 0x2, size 0x1
    s8 osflag;        // offset 0x3, size 0x1
    s8 g_or_s;        // offset 0x4, size 0x1
    s8 stimer;        // offset 0x5, size 0x1
    s16 slen;         // offset 0x6, size 0x2
    s8 proccess_dead; // offset 0x8, size 0x1
} SDAT;

typedef struct {
    // total size: 0x26
    s16 timer;       // offset 0x0, size 0x2
    s16 jmplv;       // offset 0x2, size 0x2
    s16 kosuu;       // offset 0x4, size 0x2
    s16 bbdat[4][4]; // offset 0x6, size 0x20
} BBBSTable;

typedef union {
    s32 patl;    // offset 0x0, size 0x4
    LoHi16 pats; // offset 0x0, size 0x4
} SST;

typedef union {
    s32 l;    // offset 0x0, size 0x4
    LoHi16 w; // offset 0x0, size 0x4
} ST;

typedef union {
    s32 dy;    // offset 0x0, size 0x4
    LoHi16 ry; // offset 0x0, size 0x4
} PS_DY;

typedef union {
    s32 psy;     // offset 0x0, size 0x4
    LoHi16 psys; // offset 0x0, size 0x4
} PS_UNI;

typedef union {
    s32 dp;    // offset 0x0, size 0x4
    LoHi16 rp; // offset 0x0, size 0x4
} PS_DP;

typedef struct {
    // total size: 0x10
    s16 hx;     // offset 0x0, size 0x2
    s16 hy;     // offset 0x2, size 0x2
    s16 hz;     // offset 0x4, size 0x2
    s8 sel_pri; // offset 0x6, size 0x1
    s8 sel_rl;  // offset 0x7, size 0x1
    s16 color;  // offset 0x8, size 0x2
    s8 sel_col; // offset 0xA, size 0x1
    s8 dspf;    // offset 0xB, size 0x1
    s8 ichi;    // offset 0xC, size 0x1
    s8 mts;     // offset 0xD, size 0x1
    s16 chix;   // offset 0xE, size 0x2
} PLEF;

typedef struct {
    // total size: 0x4
    s16 pos_x; // offset 0x0, size 0x2
    s16 pos_y; // offset 0x2, size 0x2
} ImageBuff;

typedef struct {
    // total size: 0xC
    s16 flag;                // offset 0x0, size 0x2
    s16 timer;               // offset 0x2, size 0x2
    const s16* changetbl_1p; // offset 0x4, size 0x4
    const s16* changetbl_2p; // offset 0x8, size 0x4
} ColorTableIndex;

typedef struct {
    // total size: 0x6
    u16 flag; // offset 0x0, size 0x2
    s16 sour; // offset 0x2, size 0x2
    s16 tm;   // offset 0x4, size 0x2
} I3_Data;

typedef struct {
    // total size: 0x8
    s16 timer;       // offset 0x0, size 0x2
    s16 endcode;     // offset 0x2, size 0x2
    const s16* adrs; // offset 0x4, size 0x4
} ColorCode;

typedef struct {
    // total size: 0x40
    s16 data[32]; // offset 0x0, size 0x40
} POWER;

typedef struct {
    // total size: 0x48
    s16 step[9][4]; // offset 0x0, size 0x48
} KOATT;

typedef union {
    s32 ixl;    // offset 0x0, size 0x4
    LoHi16 ixs; // offset 0x0, size 0x4
} TBL;

typedef struct {
    // total size: 0x18
    s16 rno;      // offset 0x0, size 0x2
    s16 cix;      // offset 0x2, size 0x2
    s16 hx;       // offset 0x4, size 0x2
    s16 hy;       // offset 0x6, size 0x2
    s16 hzr;      // offset 0x8, size 0x2
    s16 hzd;      // offset 0xA, size 0x2
    u8 pri_use;   // offset 0xC, size 0x1
    u8 bomb;      // offset 0xD, size 0x1
    s16 ispix;    // offset 0xE, size 0x2
    u8 bau;       // offset 0x10, size 0x1
    u8 kage_char; // offset 0x11, size 0x1
    u8 doa;       // offset 0x12, size 0x1
    u8 init_dsp;  // offset 0x13, size 0x1
    s16 tmt;      // offset 0x14, size 0x2
    s16 gr1st;    // offset 0x16, size 0x2
} DADD;

typedef struct /* @anon19 */ {
    // total size: 0x8
    s16 kosuu;        // offset 0x0, size 0x2
    s16 bomb;         // offset 0x2, size 0x2
    const DADD* dadd; // offset 0x4, size 0x4
} HAHEN;

typedef struct {
    // total size: 0x8
    s16 hx;   // offset 0x0, size 0x2
    s16 hy;   // offset 0x2, size 0x2
    s16 hz;   // offset 0x4, size 0x2
    s16 chix; // offset 0x6, size 0x2
} GillEffData;

typedef union {
    struct {
        // total size: 0x4
        s32 xx; // offset 0x0, size 0x4
    } s;        // offset 0x0, size 0x4
    struct {
        // total size: 0x4
        s16 xl; // offset 0x0, size 0x2
        s16 xh; // offset 0x2, size 0x2
    } o;        // offset 0x0, size 0x4
} SEA_WORK;

#endif
