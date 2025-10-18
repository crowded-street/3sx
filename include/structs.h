#ifndef STRUCTS_H
#define STRUCTS_H

#include "sf33rd/AcrSDK/common/plcommon.h"
#include "types.h"

#include <stdbool.h>

typedef struct {
    s16 l;
    s16 h;
} LoHi16;

typedef union {
    s32 sp;
    LoHi16 real;
} Reg32SpReal;

typedef union {
    s32 cal;
    LoHi16 pos;
} Reg32CalPos;

typedef struct {
    u8* pFrame;
    s32 heapnum;
} FMS_FRAME;

typedef struct {
    FMS_FRAME fmsFrame;
    u8* ramcntBuff;
    bool sysStop;
    bool initTrainingData;
    bool inGame;
    s8 language;
    bool cutAnalogStickData;
    bool useAnalogStickData;
    u8 useChar[20];
} MPP;

struct _TASK {
    void (*func_adrs)();
    void (*callback_adrs)();
    u8 r_no[4];
    u16 condition;
    s16 timer;
    u8 free[4];
};

struct _disp {
    u16 size_x;
    u16 size_y;
    u16 now;
    u16 new;
    s32 cable;
};

struct _SYSTEM_W {
    struct _disp disp;
    s32 pause;
    s32 gd_error;
    s32 reset;
    u8 sound_mode;
    u8 screen_mode;

    /// Controls background music type (arranged vs original)
    u8 bgm_type;

    u8 dummy;
};

typedef struct {
    u16 boix;
    u16 bhix;
    u16 haix;
    union {
        u16 full;
        struct {
            u8 bx;
            u8 mv;
        } half;
    } mf;
    u16 caix;
    u16 cuix;
    u16 atix;
    u16 hoix;
} UNK_0;

typedef struct {
    s16 body_dm[4][4];
} UNK_1;

typedef struct {
    s16 hand_dm[4][4];
} UNK_2;

typedef struct {
    s16 cat_box[4];
} UNK_3;

typedef struct {
    s16 cau_box[4];
} UNK_4;

typedef struct {
    s16 att_box[4][4];
} UNK_5;

typedef struct {
    s16 hos_box[4];
} UNK_6;

typedef struct {
    u8 reaction;
    u8 level;
    u8 mkh_ix;
    u8 but_ix;
    u8 dipsw;
    u8 guard;
    u8 dir;
    u8 free;
    u8 pow;
    u8 impact;
    u8 piyo;
    u8 ng_type;
    s8 hs_me;
    s8 hs_you;
    u8 hit_mark;
    u8 dmg_mark;
} UNK_7;

typedef struct {
    s16 parts_hos_x;
    s16 parts_hos_y;
    u8 parts_colmd;
    u8 parts_colcd;
    u8 parts_prio;
    u8 parts_flip;
    u8 parts_timer;
    u8 parts_disp;
    s16 parts_mts;
    u16 parts_nix;
    u16 parts_char;
} UNK_8;

typedef struct {
    s16 olc_ix[4];
} UNK_9;

typedef struct {
    s16 catch_hos_x;
    s16 catch_hos_y;
    u8 catch_prio;
    u8 catch_flip;
    s16 catch_nix;
} CatchTable;

typedef struct {
    u16 code;
    s16 koc;
    s16 ix;
    s16 pat;
} UNK11;

typedef struct {
    Reg32SpReal a[2];
    Reg32SpReal d[2];
    s16 kop[2];
    u16 index;
} MVXY;

typedef union {
    s32 cal;
    struct {
        s16 low;
        s16 pos;
    } disp;
} XY;

typedef struct {
    s16 total;
    s16 new_dm;
    s16 req_f;
    s16 old_r;
    s16 kind_of[10][4][2];
} ComboType;

typedef struct {
    s8 flag;
    s16 genkai;
    s16 time;
    union {
        s32 timer;
        LoHi16 quantity;
    } now;
    s32 recover;
    s16 store;
    s16 again;
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
    s16 kind_of_arts;
    u8 nmsa_g_ix;
    u8 exsa_g_ix;
    u8 exs2_g_ix;
    u8 nmsa_a_ix;
    u8 exsa_a_ix;
    u8 exs2_a_ix;
    s8 gauge_type;
    s8 mp;
    s8 ok;
    s8 ex;
    s8 ba;
    s8 dtm_mul;
    s8 mp_rno;
    s8 mp_rno2;
    s8 sa_rno;
    s8 sa_rno2;
    s8 ex_rno;
    s8 gt2;
    s8 saeff_ok;
    s8 saeff_mp;
    s16 gauge_len;
    union {
        s32 i;
        LoHi16 s;
    } gauge;
    s32 dtm;
    s16 store_max;
    s16 store;
    s16 id_arts;
    u8 ex4th_full;
    u8 ex4th_exec;
    s16 total_gauge;
    s16 bacckup_g_h;
} SA_WORK;

typedef struct {
    u16 sw_lvbt;
    u16 sw_new;
    u16 sw_old;
    u16 sw_now;
    u16 sw_off;
    u16 sw_chg;
    u16 old_now;
    s16 lgp;
    u8 ca14;
    u8 ca25;
    u8 ca36;
    u8 calf;
    u8 calr;
    u8 lever_dir;
    s16 waza_flag[56];
    s16 reset[56];
    u8 waza_r[56][4];
    u16 btix[56];
    u16 exdt[56][4];
} WORK_CP;

typedef struct {
    s16 r_no;
    s16 char_ix;
    s16 data_ix;
} AS;

typedef struct {
    WORK wu;
    WORK_CP* cp;
    u32 spmv_ng_flag;
    u32 spmv_ng_flag2;

    /// Number of the character that the player controls.
    s16 player_number;

    s16 zuru_timer;
    u16 zuru_ix_counter;

    // Invulnerability?
    bool zuru_flag;

    s8 tsukamarenai_flag;
    u8 kizetsu_kow;
    u8 micchaku_flag;
    u8 hos_fi_flag;
    u8 hos_em_flag;

    /// Number of the character the player is throwing.
    s16 tsukami_num;

    /// Flag that's set if the player is throwing the opponent.
    bool tsukami_f;

    /// Flag that's set if the player is being thrown by the opponent.
    bool tsukamare_f;

    s8 kind_of_catch;
    u8 old_gdflag;
    u8 guard_flag;
    u8 guard_chuu;
    s16 dm_ix;
    s16 hosei_amari;
    s8 dm_hos_flag;
    u8 dm_point;
    s16 muriyari_ugoku;
    s8 scr_pos_set_flag;
    s8 hoshi_flag;
    s8 the_same_players;
    const s8* dm_step_tbl;
    s8 running_f;
    s8 cancel_timer;
    s8 jpdir;
    s8 jptim;
    s16 current_attack;
    const AS* as;
    SA_WORK* sa;
    ComboType combo_type;
    PiyoriType* py;
    s8 wkey_flag;
    s8 dead_flag;
    s16 ukemi_ok_timer;
    s16 backup_ok_timer;
    s8 uot_cd_ok_flag;
    s8 ukemi_success;
    s16 old_pos_data[8];
    s16 move_distance;
    s16 move_power;
    s16 sa_stop_sai;
    u8 saishin_lvdir;
    u8 sa_stop_lvdir;
    u8 sa_stop_flag;
    u8 kezurijini_flag;
    s16 image_setup_flag;
    s16 image_data_index;
    u8 caution_flag;
    u8 tc_1st_flag;
    ComboType remake_power;
    s16 bullet_hcnt;
    s16 bhcnt_timer;
    s8 cat_break_ok_timer;
    s8 cat_break_reserve;
    s8 hazusenai_flag;
    s8 hurimukenai_flag;
    u8 tk_success;
    u8 resurrection_resv;
    s16 tk_dageki;
    s16 tk_nage;
    s16 tk_kizetsu;
    s16 tk_konjyou;
    s16 utk_dageki;
    s16 utk_nage;
    s16 utk_kizetsu;
    u8 atemi_flag;
    u8 atemi_point;
    s16 dm_vital_backup;
    u8 dm_refrect;
    u8 dm_vital_use;
    u8 exdm_ix;
    u8 meoshi_jump_flag;
    s16 cmd_request;
    s16 rl_save;

    // Invulnerability during bonus games
    bool zettai_muteki_flag;

    u8 do_not_move;
    u16 just_sa_stop_timer;
    s16 total_att_hit_ok;
    u8 sa_healing;
    u8 auto_guard;
    u8 hsjp_ok;
    u8 high_jump_flag;
    s16 att_plus;
    s16 def_plus;
    s8 bs2_on_car;
    s8 bs2_area_car;
    s8 bs2_over_car;
    s8 bs2_area_car2;
    s8 bs2_over_car2;
    u8 micchaku_wall_time;
    u8 extra_jump;
    u8 air_jump_ok_time;
    s16 waku_ram_index;
    u16 permited_koa;
    u8 ja_nmj_rno;
    u8 ja_nmj_cnt;
    u8 kind_of_blocking;
    u8 metamorphose;
    s16 metamor_index;
    u8 metamor_over;
    u8 gill_ccch_go;
    u8 renew_attchar;
    s16 omop_vital_timer;
    s16 sfwing_pos;
    u8 init_E3_flag;
    u8 init_E4_flag;
    u16 pl09_dat_index;
    s16 reserv_add_y;
} PLW;

typedef struct {
    WORK wu;
    u32* my_master;
    s16 master_work_id;
    s16 master_id;
    s16 master_player;
    s16 master_priority;
    u8 dm_refrect;
    u8 refrected;
    s16 free;
    u32 master_ng_flag;
    u32 master_ng_flag2;
    u8 et_free[30];
} WORK_Other;

typedef struct {
    s16 nx;
    s16 ny;
    s16 col;
    u16 chr;
} CONN;

typedef struct {
    WORK wu;
    u32* my_master;
    s16 master_work_id;
    s16 master_id;
    s16 master_player;
    s16 master_priority;
    s16 prio_reverse;
    s16 num_of_conn;
    CONN conn[108];
} WORK_Other_CONN;

typedef struct {
    WORK wu;
    u32* my_master;
    s16 master_work_id;
    s16 master_id;
    s16 master_player;
    s16 master_priority;
    s8 look_up_flag;
    s8 curr_ja;
    u16 ja_disp_bit;
    u16 ja_color_bit;
    s16 fade_cja;
    s16 ja[62][2];
    s16 jx[15][4];
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
    struct _MEMMAN_CELL* prev;
    struct _MEMMAN_CELL* next;
    ssize_t size;
} _MEMMAN_CELL;

typedef struct {
    u8* memHead;
    ssize_t memSize;
    u32 ownNumber;
    s32 ownUnit;
    ssize_t remainder;
    ssize_t remainderMin;
    struct _MEMMAN_CELL* cell_1st;
    struct _MEMMAN_CELL* cell_fin;
    u8* oriHead;
    s32 oriSize;
    s32 debIndex;
} _MEMMAN_OBJ;

typedef struct {
    s8 be_flag;
    s8 disp_flag;
    s16 fam_no;
    s16 r_no_0;
    s16 r_no_1;
    s16 r_no_2;
    s16 position_x;
    s16 position_y;
    s32 speed_x;
    s32 speed_y;
    XY xy[2];
    XY wxy[2];
    u16* bg_address;
    u16* suzi_adrs;
    s16 old_pos_x;
    s32 zuubun;
    s16 no_suzi_line;
    u16* start_suzi;
    s16 u_line;
    s16 d_line;
    s16 bg_adrs_c_no;
    s16 suzi_c_no;
    s16 pos_x_work;
    s16 pos_y_work;
    s8 rewrite_flag;
    s8 suzi_base_flag;
    XY hos_xy[2];
    XY chase_xy[2];
    s16 free;
    s16 frame_deff;
    s16 r_limit;
    s16 r_limit2;
    s16 l_limit;
    s16 l_limit2;
    s16 y_limit;
    s16 y_limit2;
    u16* suzi_adrs2;
    u16* start_suzi2;
    s16 suzi_c_no2;
    s32 max_x_limit;
    s16* deff_rl;
    s16* deff_plus;
    s16* deff_minus;
    s16 abs_x;
    s16 abs_y;
} BGW;

typedef struct {
    s8 bg_routine;
    s8 bg_r_1;
    s8 bg_r_2;
    s8 stage;
    s8 area;
    s8 compel_flag;
    s32 scroll_cg_adr;
    s32 ake_cg_adr;
    u8 scno;
    u8 scrno;
    s16 bg2_sp_x;
    s16 bg2_sp_y;
    s16 scr_stop;
    s8 frame_flag;
    s8 chase_flag;
    s8 old_chase_flag;
    s8 old_frame_flag;
    s16 pos_offset;
    s16 quake_x_index;
    s16 quake_y_index;
    s16 bg_f_x;
    s16 bg_f_y;
    s16 bg2_sp_x2;
    s16 bg2_sp_y2;
    s16 frame_deff;
    s16 center_x;
    s16 center_y;
    s16 bg_index;
    s8 frame_vol;
    s16 max_x;
    u8 bg_opaque;
    BGW bgw[7];
} BG;

typedef struct {
    u8* memoryblock;
    u8* baseandcap[2];
    u8* frame[2];
    s32 align;
} FL_FMS;

typedef struct {
    u8 order;
    u8 kind_req;
    u8 kind_cnt;
    u8 request;
    u8 contents;
    u8 timer;
    s16 pos_x;
    s16 pos_y;
    s16 pos_z;
} MessageData;

typedef struct {
    s8 contents[10][7];
    u16 sum;
} SystemDir;

typedef struct {
    u8 Shot[8];
    u8 Vibration;
    u8 free[3];
} _PAD_INFOR;

typedef struct {
    s8 contents[4][8];
} _EXTRA_OPTION;

typedef struct {
    u8 name[3];
    u16 player;
    u32 score;
    s8 cpu_grade;
    s8 grade;
    u16 wins;
    u8 player_color;
    u8 all_clear;
} RANK_DATA;

struct _SAVE_W {
    _PAD_INFOR Pad_Infor[2];
    u8 Difficulty;
    s8 Time_Limit;
    u8 Battle_Number[2];
    u8 Damage_Level;
    u8 Handicap;
    u8 Partner_Type[2];
    s8 Adjust_X;
    s8 Adjust_Y;
    u8 Screen_Size;
    u8 Screen_Mode;
    u8 GuardCheck;
    u8 Auto_Save;
    u8 AnalogStick;
    u8 BgmType;
    u8 SoundMode;
    u8 BGM_Level;
    u8 SE_Level;
    u8 Extra_Option;
    u8 PL_Color[2][20];
    _EXTRA_OPTION extra_option;
    RANK_DATA Ranking[20];
    u32 sum;
};

typedef struct {
    s8*** msgAdr;
    s8* msgNum;
} MessageTable;

struct _player_infor {
    u8 my_char;
    s8 sa;
    s8 color;
    s8 player_type;
};

struct _REP_GAME_INFOR {
    struct _player_infor player_infor[2];
    s8 stage;
    s8 Direction_Working;
    s8 Vital_Handicap[2];
    s16 Random_ix16;
    s16 Random_ix32;
    s16 Random_ix16_ex;
    s16 Random_ix32_ex;
    s8* fname;
    u8 winner;
    u8 play_type;
    u16 players_timer;
    s16 old_mes_no2;
    s16 old_mes_no3;
    s16 old_mes_no_pl;
    s16 mes_already;
};

struct _MINI_SAVE_W {
    _PAD_INFOR Pad_Infor[2];
    s8 Time_Limit;
    u8 Battle_Number[2];
    u8 Damage_Level;
    _EXTRA_OPTION extra_option;
};

typedef struct {
    u8 Handle_1P[18];
    u8 Id_1P[8];
    u8 Handle_2P[18];
    u8 Id_2P[8];
    u8 Serial_No[6];
    u8 Battle_Code[14];
} _NET_INFOR;

typedef struct {
    u8 header[640];
    u8 sega_reserve[64];
    s16 Control_Time_Buff;
    u8 Difficulty;
    u8 Monitor_Type;
    u8 free_free[4];
    struct _REP_GAME_INFOR game_infor;
    struct _MINI_SAVE_W mini_save_w;
    SystemDir system_dir;
    _NET_INFOR net_infor;
    struct {
        u16 key_buff[2][7198];
    } io_unit;
    u8 lag[14];
    u32 sum;
    u8 champion;
    u8 full_data;
    u8 free;
    u8 extra_free;
} _REPLAY_W;

struct _VM_W {
    u8 r_no[4];
    u8 r_sub[4];
    s32 Timer;
    s32 FreeMem[2];
    s8 Format[2];
    s8 Find[2];
    s8 Connect[2];
    s8 CheckDrive;
    s8 AutoDrive;
    s8 Drive;
    s8 Access;
    s8 Request;
    s8 AutoLoaded;
    struct {
        u16 year;
        u8 month;
        u8 day;
        u8 hour;
        u8 minute;
        u8 second;
        u8 dayofweek;
    } curTime[2];
    s32 curSize[2];
    s16 memKey;
    u8* memAdr;
    s32 nowResult;
    s32 nowNumber;
    s32 polResult;
    s32 polNumber;
    u8 File_Type;
    u8* File_Name;
    u32 Save_Size;
    u16 Block_Size;
    u8 Icon_Type;
    u8 Comment_Type;
    u8 Target_Number;
    u8 Number;
    u8 Counter;
    u8 Save_Type;
    s8 New_File;
    s8 Header_Counter;
    s8 padding[3];
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

typedef struct {
    s16 ix;
    s16 timer;
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
    u8 ok_dev;
    u8 id;
    u8 opck;
    u8 psix;
    s16 vital;
    u8 inStop;
    u8 free;
    u32 port;
    PPWORK_SUB p[2];
} PPWORK;

typedef struct {
    s16 prio;
    s16 rno;
    const PPWORK_SUB_SUB* adrs;
} PULREQ;

typedef struct {
    s16 x;
    s16 y;
    u32 c;
} Pixel;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4;

typedef struct {
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
} Rect;

typedef struct {
    f32 _11;
    f32 _12;
    f32 _13;
    f32 _14;
    f32 _21;
    f32 _22;
    f32 _23;
    f32 _24;
    f32 _31;
    f32 _32;
    f32 _33;
    f32 _34;
    f32 _41;
    f32 _42;
    f32 _43;
    f32 _44;
} Matrix;

typedef union {
    f32 f[16];
    f32 a[4][4];
    Matrix m;
} MTX;

typedef struct {
    s8 ok;
    s8 type;
    s16 key;
    uintptr_t texture_table;
    uintptr_t trans_table;
} TEX_GRP_LD;

typedef struct {
    u8 wh;
    u8 dat[4];
} TEX;

typedef struct {
    u8 be;
    u8 type;
    s16 id;
    u8 rno;
    u8 retry;
    u8 ix;
    u8 frre;
    s16 key;
    u8 kokey;
    u8 group;
    u8* result;
    s32 size;
    s32 sect;
    u16 fnum;
    u8 free[2];
    TEX_GRP_LD* lds;
    struct {
        u32 number;
        u32 size;
    } info;
} REQ;

struct _cursor_infor {
    u8 first_x;
    u8 first_y;
};

typedef struct {
    u8 ok[20];
    struct _cursor_infor cursor_infor[2];
} Permission;

typedef struct {
    s16 data[4][6];
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
    s16 my_cm;
    s16 my_cc;
    s16 my_pr;
    s16 my_fm;
} CharInitData2;

typedef struct {
    uintptr_t adr;
    size_t size;
    u8 search_type;
    u8 group_num;
    u8 type;
    u8 use;
} RCKeyWork;

typedef struct {
    s8 type;
    s8 form;
    s8 end_flag[4];
    s8 dmm;
    s16 id;
    s16 r_no_0;
    s16 r_no_1;
    u8 rank_in;
    s16 rank_status;
    s16 rank;
    s16 status;
    s16 index;
    s16 timer;
    s16 code[4];
    s16 old_code[4];
    s16 count1[2];
    s8 count2[2];
    s16 wait_cnt;
} NAME_WK;

typedef struct {
    s8 type;
    s8 n_disp_flag;
    s16 c_cnt;
    s16 r_no_0;
    s16 r_no_1;
    s16 f_cnt;
    u8 tenmetsu_flag;
    u8 tenmetsu_place;
} SC_NAME_WK;

typedef struct {
    s8 code[4];
} RANK_NAME_W;

typedef struct {
    s8 be;
    u8 c_mode;
    u16 total;
    u16* handle;
    s32 ixNum1st;
    u8* srcAdrs;
    u32 srcSize;
} Palette;

typedef union {
    u32 b32;
    u16 b16[2];
    u8 b8[4];
} TextureHandle;

typedef struct {
    s8 be;
    u8 flags;
    s16 arCnt;
    s16 arInit;
    u16 total;
    TextureHandle* handle;
    s32 ixNum1st;
    u16 textures;
    u16 accnum;
    u32* offset;
    u8* srcAdrs;
    size_t srcSize;
} Texture;

typedef struct {
    Texture* tex;
    Palette* pal;
} PPGDataList;

typedef struct {
    u32 magic;
    u32 fileSize;
    u8 width;
    u8 height;
    u8 compress;
    u8 pixel;
    u16 formARGB;
    u16 transNums;
} PPGFileHeader;

typedef struct {
    u32 magic;
    u32 fileSize;
    u16 id;
    u8 compress;
    u8 free;
    u32 expSize;
} PPXFileHeader;

typedef struct {
    u32 magic;
    u32 fileSize;
    u16 free;
    u8 compress;
    u8 c_mode;
    u16 formARGB;
    u16 palettes;
} PPLFileHeader;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 u;
    f32 v;
    u32 col;
} Polygon;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 s;
    f32 t;
} Vertex;

typedef struct {
    Vec3 v[4];
} Quad;

typedef struct {
    f32 s;
    f32 t;
} TexCoord;

typedef struct {
    Vec3 v[4];
    TexCoord t[4];
    u32 texCode;
} Sprite;

typedef struct {
    Vec3 v[2];
    TexCoord t[2];
    u32 vtxColor;
    u32 texCode;
    u32 id;
} Sprite2;

typedef struct {
    Sprite2* chip;
    u16 sprTotal;
    u16 sprMax;
    s8 up[24];
} SpriteChipSet;

typedef struct {
    u16 num_of_1st;
    s16 apfn;
    s16 conv;
    s16 ix1st;
    u32 use;
    u32 to_tex;
    u32 to_chd;
} TexGroupData;

typedef struct {
    f32 x;
    f32 y;
} PAL_CURSOR_P;

typedef union {
    u32 color;
    struct {
        s16 u;
        s16 v;
    } tex;
    struct {
        u8 b;
        u8 g;
        u8 r;
        u8 a;
    } argb;
} PAL_CURSOR_COL;

typedef struct {
    PAL_CURSOR_P pal_cursor_p[4];
    PAL_CURSOR_COL pal_cursor_col[4];
} PAL_CURSOR_TBL;

typedef struct {
    PAL_CURSOR_P* p;
    PAL_CURSOR_COL* col;
    PAL_CURSOR_COL* tex;
    u32 num;
} PAL_CURSOR;

typedef union {
    s32 full;
    struct {
        u8 B;
        u8 G;
        u8 R;
        u8 A;
    } rgb;
} OPTW_Color;

typedef struct {
    u32 g_no;
    u16 hv;
    s16 off_x;
    s16 off_y;
    f32 zx;
    f32 zy;
    s32 prio;
    s32 trans;
    OPTW_Color col;
} OPTW;

typedef struct {
    u32 g_no;
    s32 trans;
    u16 hv;
    s16 ok;
    OPTW_Color col;
} OPTW_Small;

typedef struct {
    s8 r_no_0;
    s8 r_no_1;
    s8 dir;
    s8 ctr;
    s16 bg_no;
    u16 blk_no;
    s32 prio;
    OPTW_Small map[4][4];
} OPBW;

typedef struct {
    s8 r_no_0;
    s8 r_no_1;
    s8 r_no_2;
    s8 old_rno;
    s16 index;
    s16 mv_ctr;
    s16 free_work;
    s16 dummy;
    OPBW bgw[3];
} OP_W;

typedef struct {
    s32 x16;
    s32 x32;
    u16 x16_free[1024];
    u16 x32_free[640];
} TexturePoolFree;

typedef struct {
    s32 x16;
    s32 x32;
    u16 x16_used[1024];
    u16 x32_used[640];
} TexturePoolUsed;

typedef struct {
    u16 x16_map[4][16];
    u8 x32_map[10][8];
} PatternMap;

typedef union {
    u32 code;
    struct {
        u16 offset;
        u16 group;
    } parts;
} PatternCode;

typedef struct {
    s16 time;
    s16 state;
    PatternCode cs;
} PatternState;

typedef struct {
    s16 curr_disp;
    s16 time;
    PatternCode cg;
    s16 x16;
    s16 x32;
    PatternMap map;
} PatternInstance;

typedef struct {
    s16 kazu;
    PatternInstance* adr[64];
    PatternInstance patt[64];
} PatternCollection;

typedef struct {
    s32 mltnum16;
    s32 mltnum32;
    s32 mltnum;
    s32 mltgidx16;
    s32 mltgidx32;
    s32 mltcshtime16;
    s32 mltcshtime32;
    PatternState* mltcsh16;
    PatternState* mltcsh32;
    u8* mltbuf;
    Texture tex;
    PPGDataList texList;
    u32 attribute;
    PatternCollection* cpat;
    TexturePoolFree* tpf;
    TexturePoolUsed* tpu;
    u8 id;
    u8 ext;
    s16 mode;
} MultiTexture;

typedef struct {
    u16 bg_h_shift;
    u16 bg_v_shift;
} BackgroundParameters;

union POS_FLOAT {
    s32 long_pos;
    LoHi16 word_pos;
};

typedef struct {
    union POS_FLOAT scr_x;
    union POS_FLOAT scr_x_buff;
    union POS_FLOAT scr_y;
    union POS_FLOAT scr_y_buff;
} BG_POS;

typedef struct {
    union POS_FLOAT family_x;
    union POS_FLOAT family_x_buff;
    union POS_FLOAT family_y;
    union POS_FLOAT family_y_buff;
} FM_POS;

typedef struct {
    u8 bg_num;
    const s16* rwd_ptr;
    const s16* brw_ptr;
    s16 rw_cnt;
    s16 rwgbix;
    s16 gbix;
} RW_DATA;

typedef struct {
    s16 offence_total;
    s16 defence_total;
    s16 tech_pts_total;
    s16 ex_point_total;
    s16 em_stun;
    s16 max_combo;
    s16 clean_hits;
    s16 att_renew;
    s16 guard_succ;
    s16 vitality;
    s16 nml_blocking;
    s16 rpd_blocking;
    s16 grd_blocking;
    s16 def_free;
    s16 first_attack;
    s16 leap_attack;
    s16 target_combo;
    s16 nml_nage;
    s16 grap_def;
    s16 quick_stand;
    s16 personal_act;
    s16 reversal;
    s16 comwaza;
    s16 sa_exec;
    s16 tairyokusa;
    s16 kimarite;
    s16 renshou;
    s16 em_renshou;
    s16 app_nml_block;
    s16 app_rpd_block;
    s16 app_grd_block;
    s16 onaji_waza;
    s16 grd_miss;
    s16 grd_mcnt;
    s16 grade;
    s16 round;
    s16 win_round;
    s16 no_lose;
} GradeData;

typedef struct {
    s16 vs_cpu_result[16];
    s16 vs_cpu_grade[16];
    s16 vs_cpu_player[16];
    s16 vcr_ix;
    s16 grade;
    s16 all_clear;
    s16 keizoku;
    s16 sp_point;
    s16 fr_ix;
    u8 fr_sort_data[16][4];
} GradeFinalData;

typedef struct {
    s16 x;
    s16 y;
    u16 attr;
    u16 code;
} TileMapEntry;

typedef struct {
    s16 ptix;
    s16 bank;
    s16 port;
    s16 code;
} SoundPatchConfig;

typedef struct {
    u16 cp3code;
    u16 free;
    SoundPatchConfig rmc;
} SoundPatch;

typedef struct {
    s16 req;
    s16 kind;
    s16 data;
    s16 code;
} BGMRequest;

typedef struct {
    s16 kind;
    s16 rno;
    s16 code;
    s16 timer;
    s16 data;
    s16 volume;
    s16 state;
    u16 ownData;
    u16 nowSeamless;
    u16 exEntry;
    u16 exIndex;
} BGMExecution;

typedef struct {
    u16 numStart;
    u16 numEnd;
    u16 numLoop;
    s16 free;
} BGMExecutionData;

typedef struct {
    union {
        s32 cal;
        struct {
            s16 low;
            s16 hi;
        } dex;
    } in;
    s32 speed;
} BGMFade;

typedef struct {
    u16 data;
    s16 vol;
    s32 fnum;
} BGMTableEntry;

typedef struct {
    u8 cmd;
    u8 flags;
    u8 prog;
    u8 note;
    u8 attr;
    u8 vol;
    u8 pan;
    s16 pitch;
    u8 prio;
    u8 id1;
    u8 id2;
    u32 kofftime;
    u8 limit;
    u16 param0;
    u16 param1;
    u16 param2;
    u16 param3;
    u16 link;
} SoundEvent;

typedef struct {
    u32 tag;
    u32 chunkSize;
    u32 version;
    u32 headerSize;
    u32 bodySize;
    u32 progChunkOffset;
    u32 smplChunkOffset;
    u32 vagiChunkOffset;
} _ps2_head_chunk;

typedef struct {
    u32 vagOffset;
    u32 vagSize;
    s32 loopFlag;
    s32 sampleRate;
} _ps2_vagi_param;

typedef union {
    u8 core_0;
    u8 core_1;
    u8 core;
} _ps2_effect;

typedef struct {
    u8 prio;
    _ps2_effect effect;
    u8 lowKey;
    u8 highKey;
    u16 bendLow;
    u16 bendHigh;
    s8 vol;
    s8 pan;
    s8 trans;
    s8 fine;
    u16 sampleIndex;
} _ps2_split_block;

typedef struct {
    u8 nSplit;
    _ps2_effect effect;
    s8 vol;
    s8 pan;
    s8 trans;
    s8 fine;
    u16 reserved;
    _ps2_split_block splitBlock[0];
} _ps2_prog_param;

typedef struct {
    u32 tag;
    u32 chunkSize;
    u32 maxProgNum;
    u32 reserved;
    u32 progParamOffset[0];
} _ps2_prog_chunk;

typedef struct {
    u16 ADSR1;
    u16 ADSR2;
    _ps2_effect effect;
    u8 base;
    s8 vol;
    s8 pan;
    s8 trans;
    s8 fine;
    u16 vagiIndex;
} _ps2_smpl_param;

typedef struct {
    u32 tag;
    u32 chunkSize;
    u32 maxVagInfoNum;
    u32 reserved;
    _ps2_vagi_param vagiParam[0];
} _ps2_vagi_chunk;

typedef struct {
    u32 tag;
    u32 chunkSize;
    u32 maxSmplNum;
    u32 reserved;
    _ps2_smpl_param smplParam[0];
} _ps2_smpl_chunk;

typedef struct {
    u16 flags;
    u8 prio;
    u8 id1;
    u8 id2;
} CSE_COND;

typedef struct {
    u16 flags;
    u8 attr;
    u8 prio;
    u8 bank;
    u8 note;
    u8 id1;
    u8 id2;
    s16 vol;
    s16 pan;
    s16 pitch;
    s16 bend;
    u8 limit;
    u8 ___dummy___0;
    u8 ___dummy___1;
    u8 ___dummy___2;
    u32 kofftime;
    u32 guid;
} CSE_REQP;

typedef struct {
    u8 vol;
    u8 pan;
    s16 pitch;
    u16 bendLow;
    u16 bendHigh;
    u16 adsr1;
    u16 adsr2;
    u32 freq;
    u32 s_addr;
} CSE_PHDP;

typedef struct {
    _ps2_prog_param* pPprm;
    _ps2_split_block* pSblk;
    _ps2_smpl_param* pSprm;
    _ps2_vagi_param* pVprm;
} CSE_PHDPADDR;

typedef struct {
    s32 result;
    u32 guid;
    u8 data[0];
} CSE_RPCQUEUE_RESULT;

typedef struct {
    u16 BeFlag;
    u8 Bank;
    u8 Code;
    u16 Interval;
    u16 Times;
    u16 VolDec1st;
    u16 VolDec;
    u16 CurrInterval;
    u16 CurrTimes;
    s32 Rtpc[10];
} CSE_ECHOWORK;

typedef struct {
    u16 x;
    u16 y;
    u32 code;
    u32 col;
} RenderBuffer;

typedef struct {
    Palette palDC;
    Palette palCP3;
    s16 req[32][2];
    s16 reqNum;
    u32 upBits;
} Col3rd_W;

typedef struct {
    s16 be;
    s16 mincg;
    s16 min16;
    s16 min32;
    s16 key0;
    s16 key1;
} MTS_OK;

typedef union {
    s32 pl;
    LoHi16 ps;
} S32Split;

typedef struct {
    s32 timer;
    s32 timer2;
    S32Split x;
    S32Split y;
    s32 spx;
    s32 dlx;
    s32 spy;
    s32 dly;
    s32 amx;
    s32 amy;
    s8 swx;
    s8 swy;
} MotionState;

typedef struct {
    s8 contents[2][2][4];
} TrainingData;

typedef struct {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} FLColor;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} FLVec3;

typedef struct {
    union {
        s32 cal;
        struct {
            u16 low;
            s16 pos;
        } disp;
    } iw[2];
} Ideal_W;

typedef struct {
    u8 name[3];
    u16 player;
    u32 score;
    s8 cpu_grade;
    s8 grade;
    u16 wins;
    u8 player_color;
    u8 all_clear;
} ScoreRankingEntry;

typedef struct {
    s8 contents[10][7];
    u16 sum;
} DirData;

typedef struct {
    u8 max_hitcombo;
    u8 new_max_flag;
    u8 frash_flag;
    u8 frash_switch;
    s16 damage;
    s16 total_damage;
    s16 disp_total_damage;
} TrainingData2;

typedef struct {
    s16 pos_x;
    s16 pos_y;
    s16 pos_z;
    u16 cg_num;
    s16 renew;
    u16 hit_ix;
    s8 flip;
    u8 cg_flp;
    s16 kowaza;
} ZanzouTableEntry;

typedef struct _anon6 {
    s8 xxxx[8][2][32];
} _anon6;

typedef struct _anon13 {
    s8 zzzz[4][4];
} _anon13;

typedef struct {
    u16 sw_new;
    u16 sw_old;
    u16 sw_chg;
    u16 sw_now;
    u16 old_now;
    u16 now_lvbt;
    u16 old_lvbt;
    u16 new_lvbt;
    u16 sw_lever;
    u16 shot_up;
    u16 shot_down;
    u16 shot_ud;
    s16 lvr_status;
    s16 jaku_cnt;
    s16 chuu_cnt;
    s16 kyou_cnt;
    s16 up_cnt;
    s16 down_cnt;
    s16 left_cnt;
    s16 right_cnt;
    s16 s1_cnt;
    s16 s2_cnt;
    s16 s3_cnt;
    s16 s4_cnt;
    s16 s5_cnt;
    s16 s6_cnt;
    s16 lu_cnt;
    s16 ld_cnt;
    s16 ru_cnt;
    s16 rd_cnt;
    s16 waza_num;
    s16 waza_no;
    s16 wait_cnt;
    s16 cmd_r_no;
} T_PL_LVR;

typedef struct {
    s16 w_type;
    s16 w_int;
    s16 free1;
    s16 w_lvr;
    s16* w_ptr;
    s16 free2;
    s16 w_dead;
    s16 w_dead2;
    union {
        struct {
            s16 flag;
            s16 shot_flag;
            s16 shot_flag2;
        } tame;
        struct {
            s16 s_cnt;
            s16 m_cnt;
            s16 l_cnt;
        } shot;
    } uni0;
    s16 free3;
    s16 shot_ok;
} WAZA_WORK;

typedef struct {
    s16 r_no_0;
    s16 r_no_1;
    s16 r_no_2;
    s16 type;
    s16 end_flag;
    s16 timer;
} END_W;

typedef union {
    s32 psi;
    LoHi16 pss;
} MS;

typedef struct {
    union {
        s16 full;
        struct {
            s8 l;
            s8 h;
        } half;
    } size;
    s32 step;
} Round_Timer;

typedef struct {
    s16 x_pos_num;
    s8 routine_num;
    u8 hit_hi;
    u8 hit_low;
    s8 kind;
    u32 pts;
    s8 pts_digit[4];
    s8 pts_flag;
    s8 first_digit;
    u8 move[2];
    u8 x_posnum[2];
    s16 timer[2];
} CMST_BUFF;

typedef struct {
    union {
        u16 results;
        struct {
            s8 att_result;
            s8 cat_result;
        } ca;
    } flag;
    u8 my_att;
    u8 dm_body;
    u16 my_hit;
    u16 dm_me;
    s16* ah;
    s16* dh;
} HS;

typedef struct {
    s16 my_wkid;
    u8 waza_num;
    u8 vs_refrect;
    u16 koa;
    u8 kind_of_tama;
    u8 kage_index;
    u8 chix;
    u8 ernm;
    u8 erht;
    u8 erdf;
    u8 erex;
    u8 col_1p;
    u8 col_2p;
    u8 data00;
    u8 data01;
    u8 disp_type;
    s16 def_power;
    s16 life_time;
    s16 hos_x;
    s16 hos_y;
    u8 kz_blocking;
    u8 free;
} TAMA;

typedef struct {
    s16 cyerw;
    s16 cred;
    s16 ored;
    s8 colnum;
} VIT;

typedef struct {
    s16 offence_total;
    s16 defence_total;
    s16 tech_pts_total;
    s16 ex_point_total;
    s16 grade;
} JudgeGals;

typedef struct {
    s16 offence_total;
    s16 defence_total;
    s16 tech_pts_total;
    s16 ex_point_total;
    s16 round;
    s16 grade;
} JudgeCom;

typedef struct {
    s16 cstn;
    s8 sflag;
    s8 osflag;
    s8 g_or_s;
    s8 stimer;
    s16 slen;
    s8 proccess_dead;
} SDAT;

typedef struct {
    s16 timer;
    s16 jmplv;
    s16 kosuu;
    s16 bbdat[4][4];
} BBBSTable;

typedef union {
    s32 patl;
    LoHi16 pats;
} SST;

typedef union {
    s32 l;
    LoHi16 w;
} ST;

typedef union {
    s32 dy;
    LoHi16 ry;
} PS_DY;

typedef union {
    s32 psy;
    LoHi16 psys;
} PS_UNI;

typedef union {
    s32 dp;
    LoHi16 rp;
} PS_DP;

typedef struct {
    s16 hx;
    s16 hy;
    s16 hz;
    s8 sel_pri;
    s8 sel_rl;
    s16 color;
    s8 sel_col;
    s8 dspf;
    s8 ichi;
    s8 mts;
    s16 chix;
} PLEF;

typedef struct {
    s16 pos_x;
    s16 pos_y;
} ImageBuff;

typedef struct {
    s16 flag;
    s16 timer;
    const s16* changetbl_1p;
    const s16* changetbl_2p;
} ColorTableIndex;

typedef struct {
    u16 flag;
    s16 sour;
    s16 tm;
} I3_Data;

typedef struct {
    s16 timer;
    s16 endcode;
    const s16* adrs;
} ColorCode;

typedef struct {
    s16 data[32];
} POWER;

typedef struct {
    s16 step[9][4];
} KOATT;

typedef union {
    s32 ixl;
    LoHi16 ixs;
} TBL;

typedef struct {
    s16 rno;
    s16 cix;
    s16 hx;
    s16 hy;
    s16 hzr;
    s16 hzd;
    u8 pri_use;
    u8 bomb;
    s16 ispix;
    u8 bau;
    u8 kage_char;
    u8 doa;
    u8 init_dsp;
    s16 tmt;
    s16 gr1st;
} DADD;

typedef struct {
    s16 kosuu;
    s16 bomb;
    const DADD* dadd;
} HAHEN;

typedef struct {
    s16 hx;
    s16 hy;
    s16 hz;
    s16 chix;
} GillEffData;

typedef union {
    struct {
        s32 xx;
    } s;
    struct {
        s16 xl;
        s16 xh;
    } o;
} SEA_WORK;

#endif
