#include "sf33rd/Source/Game/staff.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/effect/effh6.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include <string.h>

s16 roll_rate2;
s16 roll_rate_t2;
s16 roll_stop;
s16 name_timer;
static s32 staffroll_end;
static s16 name_ptr;

static const struct {
    s16 next;
    u16 x;
    u16 y;
    u16 atr;
    s8* name;
} sf3_staff[243] = { { -1, 48, 128, 3, "STREET FIGHTER {  3rd STRIKE" },
                     { -1, 48, 96, 3, "Fight for the Future" },
                     { -1, 48, 64, 3, "STAFF" },
                     { 160, 96, 160, 8, "ARCADE ORIGINAL STAFF" },
                     { 30, 96, 160, 8, "CHARACTER DESIGN" },
                     { 20, 96, 136, 6, "WHO" },
                     { 20, 96, 136, 6, "IKUSAN Z" },
                     { 20, 96, 136, 6, "D=Kurita" },
                     { 20, 96, 136, 6, "Chimorin SHOGUN" },
                     { 20, 104, 136, 6, "SIBAKI]" },
                     { 20, 96, 136, 6, "Chunzenji&Rute" },
                     { 20, 96, 136, 6, "Tsuyoshi" },
                     { 20, 96, 136, 6, "Jun-Ikawa" },
                     { 20, 96, 136, 6, "Toshihiro Suzuki" },
                     { 20, 96, 136, 6, "NOYA-T" },
                     { 20, 96, 136, 6, "Seiji Yano" },
                     { 20, 96, 136, 6, "Naoi\"HELL\"Ryosuke" },
                     { 20, 96, 136, 6, "TOMO" },
                     { 20, 88, 136, 6, "HIRANO=DAICHI" },
                     { 20, 96, 136, 6, "Izumi.N" },
                     { 60, 96, 136, 6, "AKIMAN" },
                     { 30, 96, 160, 8, "SCROLL DESIGN" },
                     { 20, 96, 136, 6, "IMAHORI 75%" },
                     { 20, 104, 136, 6, "SUGIYAMA 50%" },
                     { 20, 88, 136, 6, "]YOUKO FUKUMOTO]" },
                     { 20, 88, 136, 6, "[ YUKI KYOUTANI [" },
                     { 20, 96, 136, 6, "|SAWATCH" },
                     { 20, 96, 136, 6, "^ ATUSHI ^" },
                     { 20, 96, 136, 6, "RYO UNO" },
                     { 20, 88, 136, 6, "Yusuke Saiwai*" },
                     { 60, 96, 136, 6, "KENICHI YAMAHASHI" },
                     { 30, 96, 160, 8, "ORIGINAL ART WORK" },
                     { 20, 88, 136, 6, "DAIGORO" },
                     { 60, 88, 136, 6, "HARUMARU" },
                     { 30, 96, 160, 8, "TITLE LOGO DESIGN" },
                     { 60, 88, 136, 6, "Y.UCHIDA" },
                     { 30, 96, 160, 8, "INSTRUCTION DESIGN" },
                     { 60, 96, 136, 6, "SAKOMIZU" },
                     { 30, 96, 160, 8, "PROGRAMING" },
                     { 20, 88, 136, 6, "KAZUHITO NAKAI" },
                     { 20, 96, 136, 6, "NOBUYA\"LT1\"YOSHIZUMI" },
                     { 20, 88, 136, 6, "YUKO KAWAMURA(TATE)" },
                     { 20, 88, 136, 6, "HIROSHI NAKAGAWA(RAOH)" },
                     { 20, 96, 136, 6, "Y&M.INADA(INE)" },
                     { 40, 88, 136, 6, "MASAHITO Oh!Ya!" },
                     { 60, 88, 136, 6, "AKIHIRO | YOKOYAMA" },
                     { 30, 96, 160, 8, "SOUND EFFECT DESIGN" },
                     { 20, 104, 136, 6, "SATOSHI ISE" },
                     { 60, 96, 136, 6, "Y.SANDOU" },
                     { 30, 96, 160, 8, "MUSIC COMPOSE" },
                     { 60, 96, 136, 6, "H. OKUGAWA" },
                     { 30, 96, 160, 8, "SOUND EDITOR" },
                     { 60, 88, 136, 6, "KAZUYA TAKIMOTO" },
                     { 30, 96, 160, 8, "SOUND MANAGEMENT DIRECTOR" },
                     { 60, 88, 136, 6, "Yoshinori Ono" },
                     { 20, 48, 136, 8, "Third Strike    (Opening Theme Song)" },
                     { 20, 48, 136, 8, "Let's Get It On (Event Theme Song)" },
                     { 40, 48, 136, 8, "Moving On       (Ending Song)" },
                     { 20, 48, 136, 6, "Song Written by Infinite/hideki" },
                     { 60, 48, 136, 6, "Arranged by hideki" },
                     { 20, 48, 136, 8, "Vocals" },
                     { 60, 224, 136, 2, "#" },
                     { 20, 4, 136, 6, "(Courtesy of Lock Down Entertainment Inc.)" },
                     { 120, 192, 136, 1, "#" },
                     { 30, 96, 136, 8, "RECORDING ENGINEER" },
                     { 60, 80, 136, 6, "Yutaka Shimoyama(Victor St.)" },
                     { 30, 96, 136, 8, "MIX ENGINEER" },
                     { 60, 96, 136, 6, "Paul Shubat" },
                     { 30, 96, 136, 8, "ASSISTANT ENGINEER" },
                     { 60, 96, 136, 6, "Vladimir Nikolic" },
                     { 30, 96, 136, 8, "ARTIST MANAGEMENT" },
                     { 60, 64, 136, 6, "Cain(Lock Down Entertainment Inc.)" },
                     { 30, 96, 160, 8, "VOICE ACTOR" },
                     { 20, 96, 136, 6, "TOURU OHKAWA" },
                     { 20, 96, 136, 6, "YUJI KISHI" },
                     { 20, 96, 136, 6, "MITSUO IWATA" },
                     { 20, 96, 136, 6, "KENTARO ITOU" },
                     { 20, 96, 136, 6, "MASAKAZU SUZUKI" },
                     { 20, 104, 136, 6, "YURI AMANO" },
                     { 20, 104, 136, 6, "MIE MIDORI" },
                     { 20, 96, 136, 6, "TAKASHI MATSUYAMA" },
                     { 20, 96, 136, 6, "TOMOMICHI NISHIMURA" },
                     { 20, 88, 136, 6, "MAKOTO TSUMURA" },
                     { 20, 88, 136, 6, "EIJI SEKIGUCHI" },
                     { 40, 96, 136, 6, "ATSUKO TANAKA" },
                     { 20, 96, 136, 6, "LAWRENCE BAYNE" },
                     { 20, 96, 136, 6, "PATRICK GALLAGAN" },
                     { 20, 88, 136, 6, "FRANCIS DIAKEWSKY" },
                     { 60, 96, 136, 6, "LEN CARLSON" },
                     { 30, 96, 160, 8, "PLANNING" },
                     { 20, 96, 136, 6, "Neo_G H.ISHIZAWA" },
                     { 20, 96, 136, 6, "HARUO MURATA" },
                     { 20, 96, 136, 6, "NUMACH (T.O)" },
                     { 40, 88, 136, 6, "YASUNORI ICHINOSE" },
                     { 60, 96, 136, 6, "SADA 36" },
                     { 30, 88, 160, 8, "SPECIAL THANKS" },
                     { 40, 96, 136, 6, "YOUICHI EGAWA" },
                     { 20, 96, 136, 6, "T.SHIRAIWA" },
                     { 40, 96, 136, 6, "ABU" },
                     { 20, 96, 136, 6, "TOMOAKI TSUJI" },
                     { 20, 96, 136, 6, "TOBUTO(MAM2)DEZAKI" },
                     { 20, 96, 136, 6, "MITSUAKI(TIM2)ARAKI" },
                     { 20, 96, 136, 6, "TAKEchan" },
                     { 20, 96, 136, 6, "ONI-SUZUKI" },
                     { 40, 96, 136, 6, "KAERU" },
                     { 20, 96, 136, 6, "BUG(H.SUGIMOTO)" },
                     { 20, 96, 136, 6, "K.SHINANO" },
                     { 20, 96, 136, 6, "GON" },
                     { 40, 96, 136, 6, "SYACHI(S.OHOKA)" },
                     { 20, 96, 136, 6, "YNG.J.MATSUBARA" },
                     { 20, 88, 136, 6, "MIURA CUBERICK" },
                     { 20, 88, 136, 6, "TETSUYA HAZE" },
                     { 20, 88, 136, 6, "YOSHIHIRO TOMITA" },
                     { 20, 88, 136, 6, "HMX-12 HAMADA" },
                     { 40, 88, 136, 6, "MASAYASU MUKAI" },
                     { 20, 88, 136, 6, "PUNCH KUBOZONO" },
                     { 20, 88, 136, 6, "TETSUYA IIJIMA" },
                     { 20, 88, 136, 6, "LISA E. JONES" },
                     { 20, 88, 136, 6, "MASAYUKI FUKUMOTO" },
                     { 20, 88, 136, 6, "LANCE LUU" },
                     { 40, 88, 136, 6, "NATE WILLIAMS" },
                     { 30, 96, 160, 8, "MESSAGE TRANSLATOR" },
                     { 20, 96, 136, 6, "ERIK SUZUKI" },
                     { 20, 96, 136, 6, "DAN OKADA" },
                     { 80, 104, 136, 6, "CHRIS TANG" },
                     { 30, 96, 160, 8, "PRODUCER" },
                     { 40, 96, 136, 6, "NORITAKA FUNAMIZU" },
                     { 30, 88, 136, 6, "KOUJI NAKAJIMA" },
                     { 30, 88, 136, 6, "? " },
                     { 30, 96, 160, 8, "GENERAL PRODUCER" },
                     { 200, 88, 136, 6, "YOSHIKI OKAMOTO" },
                     { 80, 96, 160, 8, "CONSUMER ORIGINAL STAFF" },
                     { 80, 96, 160, 8, "` " },
                     { 30, 96, 160, 8, "CHARACTER DESIGN" },
                     { 20, 88, 136, 6, "JUN IKAWA" },
                     { 20, 88, 136, 6, "YUJI IMAMURA" },
                     { 20, 88, 136, 6, "Pu$Chimorin" },
                     { 20, 88, 136, 6, "Toshihiro Suzuki" },
                     { 20, 88, 136, 6, "TOMO" },
                     { 20, 88, 136, 6, "DAICHI=HIRANO" },
                     { 40, 88, 136, 6, "NAONEE[" },
                     { 60, 88, 136, 6, "D-KURITA" },
                     { 30, 96, 160, 8, "SCROLL DESIGN" },
                     { 20, 88, 136, 6, "SAWATCH-again" },
                     { 20, 88, 136, 6, "YUKI]KYOUTANI" },
                     { 60, 88, 136, 6, "YUSUKE-SAIWAI" },
                     { 30, 96, 160, 8, "ORIGINAL ART WORK" },
                     { 60, 88, 136, 6, "IKENO|MIE" },
                     { 30, 96, 160, 8, "INSTRUCTION DESIGN" },
                     { 40, 88, 136, 6, "M.ru" },
                     { 20, 88, 136, 6, "MIDUHO|" },
                     { 60, 88, 136, 6, "Y.UCHIDA" },
                     { 30, 96, 160, 8, "PROGRAMING" },
                     { 20, 88, 136, 6, "Kazuhito Nakai" },
                     { 20, 88, 136, 6, "Nobuya\"GENTLE\"Yoshizumi" },
                     { 20, 88, 136, 6, "HIROSHI \"RAOH\" NAKAGAWA" },
                     { 40, 88, 136, 6, "MaSaHiTo Oh!Ya! (MSHTOY)" },
                     { 20, 88, 136, 6, "Kohei Akiyama" },
                     { 20, 88, 136, 6, "Tomohiro Ueno" },
                     { 40, 88, 136, 6, "Batayon" },
                     { 60, 88, 136, 6, "Meijin" },
                     { 30, 96, 160, 8, "SOUND" },
                     { 20, 88, 136, 6, "Hiro\"X68k\"Kondo" },
                     { 60, 88, 136, 6, "Yoshiki Sandou" },
                     { 30, 96, 160, 8, "MUSIC" },
                     { 60, 88, 136, 6, "8bit@okugawa" },
                     { 30, 96, 160, 8, "RECORDED" },
                     { 60, 88, 136, 6, "Kazuya Takimoto" },
                     { 30, 96, 160, 8, "PLANNING" },
                     { 40, 88, 136, 6, "Neo_G H.ISHIZAWA" },
                     { 40, 88, 136, 6, "HARUO MURATA" },
                     { 60, 88, 136, 6, "TOMONORI OHNUMA" },
                     { 30, 96, 160, 8, "SPECIAL THANKS" },
                     { 40, 88, 136, 6, "HIDEKI OHNUKI" },
                     { 20, 88, 136, 6, "KEIKO HARAKAWA" },
                     { 20, 88, 136, 6, "MICHIYO AOKI" },
                     { 40, 88, 136, 6, "HIROMITSU FURUBEPPU" },
                     { 20, 88, 136, 6, "Toshiaki Matsuda" },
                     { 20, 88, 136, 6, "Takako Akutagawa" },
                     { 40, 88, 136, 6, "Ryosuke Hagiwara" },
                     { 40, 88, 136, 6, "ABU" },
                     { 40, 88, 136, 6, "Yoshinori Ono" },
                     { 20, 88, 136, 6, "SHIN." },
                     { 20, 88, 136, 6, "ICHIRO YUNDE" },
                     { 20, 88, 136, 6, "NET MAN" },
                     { 20, 88, 136, 6, "MINEYUKI NODA" },
                     { 40, 88, 136, 6, "KENTARO$KANEKO" },
                     { 40, 88, 136, 6, "HIDEAKI ITSUNO" },
                     { 20, 88, 136, 6, "Ika}chan" },
                     { 60, 88, 136, 6, "momo-mo" },
                     { 40, 88, 136, 6, "TAKUYA SHIRAIWA" },
                     { 80, 88, 136, 6, "ERIK(ESX)SUZUKI" },
                     { 30, 96, 160, 8, "DIRECTOR" },
                     { 60, 88, 136, 6, "HIDETOSHI ISHIZAWA" },
                     { 30, 96, 160, 8, "PRODUCER" },
                     { 60, 88, 136, 6, "YOSHIHIRO SUDO" },
                     { 30, 96, 160, 8, "GENERAL PRODUCER" },
                     { 60, 88, 136, 6, "NORITAKA  FUNAMIZU" },
                     { 30, 96, 160, 8, "EXECUTIVE PRODUCER" },
                     { 180, 88, 136, 6, "YOSHIKI  OKAMOTO" },
                     { 160, 96, 160, 8, "\"PlayStation 2\" STAFF" },
                     { 30, 96, 160, 8, "PROGRAMING" },
                     { 20, 88, 136, 6, "Nakai Kazuhito" },
                     { 60, 88, 136, 6, "A.F" },
                     { 30, 96, 160, 8, "SOUND & MUSIC EDIT" },
                     { 60, 88, 136, 6, "Hiroaki\"X68K\"Kondo" },
                     { 30, 96, 160, 8, "MUSIC REMASTER" },
                     { 60, 88, 136, 6, "Mitsuhiko Takano" },
                     { 30, 96, 160, 8, "ICON DESIGN" },
                     { 60, 88, 136, 6, "Masatoshi Matsushita" },
                     { 30, 96, 160, 8, "PACKAGE ILLUSTRATION" },
                     { 60, 88, 136, 6, "AKIMAN" },
                     { 30, 96, 160, 8, "INSTRUCTION DESIGN" },
                     { 60, 88, 136, 6, "AKEMI IWASAKI" },
                     { 30, 96, 160, 8, "PLANNING" },
                     { 20, 88, 136, 6, "Neo_G -H.Ishizawa-" },
                     { 60, 88, 136, 6, "MORIBA-" },
                     { 20, 96, 160, 8, "GAME LIBRARY SUPPLIED BY" },
                     { 60, 96, 160, 8, "ACR Project Staff" },
                     { 30, 96, 160, 8, "SPECIAL THANKS" },
                     { 20, 88, 136, 6, "N.Minomiya" },
                     { 40, 88, 136, 6, "SUGIYAMA HIROSHI" },
                     { 20, 88, 136, 6, "YOSSUI-" },
                     { 20, 88, 136, 6, "bug(SKILL SMITH)" },
                     { 20, 88, 136, 6, "TOMOHIRO-RIDER80000V" },
                     { 20, 88, 136, 6, "SARU/my" },
                     { 60, 88, 136, 6, "Hisakin]Mukai" },
                     { 30, 96, 160, 8, "DIRECTOR" },
                     { 60, 88, 136, 6, "HIDETOSHI ISHIZAWA" },
                     { 30, 96, 160, 8, "ASSISTANT PRODUCER" },
                     { 20, 88, 136, 6, "KENZY ITSUNO" },
                     { 60, 88, 136, 6, "HITOMI NISHIMOTO" },
                     { 30, 96, 160, 8, "PRODUCER" },
                     { 60, 88, 136, 6, "YOSHIHIRO SUDO" },
                     { 30, 96, 160, 8, "EXECUTIVE DIRECTOR" },
                     { 140, 88, 136, 6, "NORITAKA FUNAMIZU" },
                     { 160, 96, 112, 8, "AND CAPCOM ALL STAFF" },
                     { 128, 48, 144, 5, " " },
                     { -1, 48, 144, 5, "THANK YOU FOR PLAYING!" },
                     { -1, 48, 96, 5, "PRESENTED BY" },
                     { 20, 190, 96, 0, "#" },
                     { 20, 190, 0, 5, " " },
                     { 256, 0, 0, 6, NULL } };

static s32 check_shortcut() {
    u16 sw_w;

    if (!end_no_cut) {
        if (WINNER) {
            sw_w = p2sw_0;
        } else {
            sw_w = p1sw_0;
        }

        if (sw_w & 0xFF0) {
            return 1;
        }
    }

    return 0;
}

void set_credit_string(s32 t, s32 x, s32 y, s32 a, s8* s) {
    char* su;
    s16 xu;
    s16 yu;
    s16 mojisuu;

    su = s;
    xu = x;
    yu = y;

    if (*su == 0x3F || *su == 0x60) {
        su++;
    }

    mojisuu = strlen(su);
    xu = 192 - ((mojisuu * 9) / 2);

    xu += (bg_w.bgw[5].xy[0].disp.pos) - 192;
    yu += bg_w.bgw[5].position_y;

    effect_H6_init(t, su, xu, yu, a, -1);
}

s32 staff_credits(u32 /* unused */) {
#if defined(TARGET_PS2)
    void Scrn_Move_Set(s8 bgnm, s32 x, s32 y);
    void Family_Set_R(s8 fmnm, s32 x, s32 y);
#endif

    s16 t;
    s16 x;
    s16 y;
    s16 a;

    switch (staff_r_no) {
    case 0:
        staff_r_no++;
        staffroll_end = 0;
        BGM_Request(0x3F);
        bg_w.bgw[5].wxy[0].cal = bg_w.bgw[5].xy[0].cal = 0x01000000;
        bg_w.bgw[5].xy[1].cal = 0;
        bg_w.bgw[5].position_x = 0x100 - bg_w.pos_offset;
        bg_w.bgw[5].position_y = 0;
        x = bg_w.bgw[5].position_x;
        y = bg_w.bgw[5].position_y;
        Scrn_Move_Set(5, x, y);
        x = -x & 0xFFFF;
        y = 0x300 - (y & 0xFFFF) & 0xFFFF;
        Family_Set_R(6, x, y);
        roll_rate2 = 1;
        roll_stop = 0;
        name_ptr = 0;
        end_w.timer = 0;
        name_timer = 0;
        break;

    case 1:
        if (check_shortcut() != 0) {
            staff_r_no = 4;
            end_w.timer = 0;
        } else {
            roll_rate_t2 = 1;
        }

        if (end_w.timer >= 0) {
            end_w.timer = end_w.timer - roll_rate_t2;
        } else {
            if (sf3_staff[name_ptr].name == NULL) {
                staff_r_no = 3;
                end_w.timer = sf3_staff[name_ptr].next;
                SsBgmFadeOut(0x88);
                break;
            }

            if (*sf3_staff[name_ptr].name == 0x3F) {
                SsBgmFadeOut(0x4E);
            }

            if (*sf3_staff[name_ptr].name == 0x60) {
                BGM_Request(0x40);
            }

            name_timer = 0x7FFF;
            t = 240;
            x = sf3_staff[name_ptr].x;
            y = sf3_staff[name_ptr].y;
            a = sf3_staff[name_ptr].atr;
            set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
            end_w.timer = sf3_staff[name_ptr].next;
            name_ptr++;

            if (0 >= end_w.timer) {
                t = 240;
                x = sf3_staff[name_ptr].x;
                y = sf3_staff[name_ptr].y;
                a = sf3_staff[name_ptr].atr;
                set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
                end_w.timer = sf3_staff[name_ptr].next;
                name_ptr++;

                if (0 >= end_w.timer) {
                    t = 240;
                    x = sf3_staff[name_ptr].x;
                    y = sf3_staff[name_ptr].y;
                    a = sf3_staff[name_ptr].atr;
                    set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
                    end_w.timer = 0xF0;
                    name_ptr++;
                }
            }
        }

        if (name_timer >= 0) {
            name_timer = name_timer - roll_rate_t2;
            break;
        }

        name_timer = sf3_staff[name_ptr].next;
        t = 20;
        x = sf3_staff[name_ptr].x;
        y = sf3_staff[name_ptr].y;
        a = sf3_staff[name_ptr].atr;
        set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
        name_ptr++;
        t = 20;
        x = sf3_staff[name_ptr].x;
        y = sf3_staff[name_ptr].y;
        a = sf3_staff[name_ptr].atr;
        set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
        name_ptr++;

        if (sf3_staff[name_ptr].next == 0) {
            t = 20;
            x = sf3_staff[name_ptr].x;
            y = sf3_staff[name_ptr].y;
            a = sf3_staff[name_ptr].atr;
            set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
            name_ptr++;
            t = 20;
            x = sf3_staff[name_ptr].x;
            y = sf3_staff[name_ptr].y;
            a = sf3_staff[name_ptr].atr;
            set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
            name_ptr++;

            if (sf3_staff[name_ptr].next == 0) {
                t = 20;
                x = sf3_staff[name_ptr].x;
                y = sf3_staff[name_ptr].y;
                a = sf3_staff[name_ptr].atr;
                set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
                name_ptr++;
                t = 20;
                x = sf3_staff[name_ptr].x;
                y = sf3_staff[name_ptr].y;
                a = sf3_staff[name_ptr].atr;
                set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
                name_ptr++;

                if (sf3_staff[name_ptr].next == 0) {
                    t = 20;
                    x = sf3_staff[name_ptr].x;
                    y = sf3_staff[name_ptr].y;
                    a = sf3_staff[name_ptr].atr;
                    set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
                    name_ptr++;
                    t = 20;
                    x = sf3_staff[name_ptr].x;
                    y = sf3_staff[name_ptr].y;
                    a = sf3_staff[name_ptr].atr;
                    set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
                    name_ptr++;

                    if (sf3_staff[name_ptr].next == 0) {
                        t = 20;
                        x = sf3_staff[name_ptr].x;
                        y = sf3_staff[name_ptr].y;
                        a = sf3_staff[name_ptr].atr;
                        set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
                        name_ptr++;
                        t = 20;
                        x = sf3_staff[name_ptr].x;
                        y = sf3_staff[name_ptr].y;
                        a = sf3_staff[name_ptr].atr;
                        set_credit_string(t, x, y, a, sf3_staff[name_ptr].name);
                        name_ptr++;
                    }
                }
            }
        }

        break;

    case 2:
    case 3:
        if (end_w.timer >= 0) {
            end_w.timer = end_w.timer - roll_rate_t2;
        } else {
            staff_r_no++;
        }

        if (check_shortcut() != 0) {
            staff_r_no = 4;
            end_w.timer = 0;
        }

        break;

    default:
        staffroll_end = 1;
        break;
    }

    return staffroll_end;
}
