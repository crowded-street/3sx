#include "common.h"

static s8* mcard_msg_00[] = { NULL };

static s8* mcard_msg_01[] = { "▼",
                              "ＭＥＭＯＲＹ ＣＡＲＤ差込口１",
                              "ＭＥＭＯＲＹ ＣＡＲＤ差込口２",
                              "はい",
                              "いいえ",
                              "ＬＯＡＤ",
                              "ＳＡＶＥ",
                              "ＡＵＴＯＬＯＡＤ",
                              "ＡＵＴＯＳＡＶＥ",
                              "ロード",
                              "セーブ",
                              "オートロード",
                              "オートセーブ",
                              "システムデータ",
                              "ディレクションデータ",
                              "リプレイデータ",
                              NULL };

static s8* mcard_msg_02[] = { "どちらのＭＥＭＯＲＹ ＣＡＲＤ差込口を", "使用しますか？", NULL };

static s8* mcard_msg_03[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)から",
                              "%fをロードしています…",
                              "\"PlayStation 2\"専用メモリーカード(8MB)を",
                              "抜いたり、電源を切ったりしないでください。",
                              NULL };

static s8* mcard_msg_04[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)から",
                              "%fを正しくロードできませんでした。%r",
                              NULL };

static s8* mcard_msg_05[] = {
    "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sに", "\"PlayStation 2\"専用メモリーカード(8MB)が", "差さっていません。%r", NULL
};

static s8* mcard_msg_06[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)は",
                              "フォーマットされていません。%r",
                              NULL };

static s8* mcard_msg_07[] = {
    "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの", "\"PlayStation 2\"専用メモリーカード(8MB)には", "%fはありませんでした。%r", NULL
};

static s8* mcard_msg_08[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)に",
                              "アクセスしています…",
                              "\"PlayStation 2\"専用メモリーカード(8MB)を",
                              "抜いたり、電源を切ったりしないでください。",
                              NULL };

static s8* mcard_msg_09[] = {
    "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの", "\"PlayStation 2\"専用メモリーカード(8MB)の", "アクセスに失敗しました。%r", NULL
};

static s8* mcard_msg_10[] = { "（未使用）", NULL };
static s8* mcard_msg_11[] = { "（未使用）", NULL };
static s8* mcard_msg_12[] = { "（未使用）", NULL };
static s8* mcard_msg_13[] = { "（未使用）", NULL };

static s8* mcard_msg_14[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)は",
                              "フォーマットされていません。",
                              "フォーマットしますか？",
                              NULL };

static s8* mcard_msg_15[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)を",
                              "フォーマットしています…",
                              "\"PlayStation 2\"専用メモリーカード(8MB)を",
                              "抜いたり、電源を切ったりしないでください。",
                              NULL };

static s8* mcard_msg_16[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)を",
                              "正しくフォーマットできませんでした。%r",
                              NULL };

static s8* mcard_msg_17[] = { "どの%fをロードしますか？", NULL };
static s8* mcard_msg_18[] = { "どこに%fをセーブしますか？", NULL };

static s8* mcard_msg_19[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)には",
                              "すでに%fが存在します。",
                              "上書きしてもよろしいですか？",
                              NULL };

static s8* mcard_msg_20[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)へ",
                              "%fを%eしています…",
                              "\"PlayStation 2\"専用メモリーカード(8MB)を",
                              "抜いたり、電源を切ったりしないでください。",
                              NULL };

static s8* mcard_msg_21[] = {
    "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの", "\"PlayStation 2\"専用メモリーカード(8MB)へ", "%fを%eしました。%r", NULL
};

static s8* mcard_msg_22[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)へ",
                              "%fを正しく%eできませんでした。%r",
                              NULL };

static s8* mcard_msg_23[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",    "\"PlayStation 2\"専用メモリーカード(8MB)は",
                              "空き容量が不足しています。",         "%fをセーブするには",
                              "%aＫＢ以上の空き容量が必要です。%r", NULL };

static s8* mcard_msg_24[] = { "（未使用）", NULL };
static s8* mcard_msg_25[] = { "（未使用）", NULL };

static s8* mcard_msg_26[] = {
    "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの", "\"PlayStation 2\"専用メモリーカード(8MB)から", "%fを%eしました。%r", NULL
};

static s8* mcard_msg_27[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)から",
                              "%fを正しく%eできませんでした。",
                              "このままゲームを開始してよろしいですか？",
                              NULL };

static s8* mcard_msg_28[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口<C2>１<C7>・<C2>２<C7>共に",
                              "\"PlayStation 2\"専用メモリーカード(8MB)が",
                              "差さっていません。",
                              "%fをセーブするには",
                              "%aＫＢ以上の空き容量が必要です。",
                              "このままゲームを開始してよろしいですか？",
                              NULL };

static s8* mcard_msg_29[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)は",
                              "空き容量が不足しています。",
                              "%fをセーブするには",
                              "%aＫＢ以上の空き容量が必要です。",
                              "このままゲームを開始してよろしいですか？",
                              NULL };

static s8* mcard_msg_30[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口<C2>１<C7>・<C2>２<C7>の",
                              "\"PlayStation 2\"専用メモリーカード(8MB)は",
                              "共に空き容量が不足しています。",
                              "%fをセーブするには",
                              "%aＫＢ以上の空き容量が必要です。",
                              "このままゲームを開始してよろしいですか？",
                              NULL };

static s8* mcard_msg_31[] = { "これより先、このゲームの%fは",
                              "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)に",
                              "オートセーブされます。%r",
                              NULL };

static s8* mcard_msg_32[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sに",
                              "\"PlayStation 2\"専用メモリーカード(8MB)が",
                              "差さっていません。",
                              "オートセーブを解除します。%r",
                              NULL };

static s8* mcard_msg_33[] = { "ＭＥＭＯＲＹ ＣＡＲＤ差込口%sの",
                              "\"PlayStation 2\"専用メモリーカード(8MB)は",
                              "変更されています。",
                              "オートセーブを解除します。%r",
                              NULL };

static s8* mcard_msg_34[] = { "セーブせずに終了してもよろしいですか？", NULL };

static s8** mcard_msg_tbl[] = { mcard_msg_00, mcard_msg_01, mcard_msg_02, mcard_msg_03, mcard_msg_04, mcard_msg_05,
                                mcard_msg_06, mcard_msg_07, mcard_msg_08, mcard_msg_09, mcard_msg_10, mcard_msg_11,
                                mcard_msg_12, mcard_msg_13, mcard_msg_14, mcard_msg_15, mcard_msg_16, mcard_msg_17,
                                mcard_msg_18, mcard_msg_19, mcard_msg_20, mcard_msg_21, mcard_msg_22, mcard_msg_23,
                                mcard_msg_24, mcard_msg_25, mcard_msg_26, mcard_msg_27, mcard_msg_28, mcard_msg_29,
                                mcard_msg_30, mcard_msg_31, mcard_msg_32, mcard_msg_33, mcard_msg_34 };

void* MessageTableJPN[] = { mcard_msg_tbl };
