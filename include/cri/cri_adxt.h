#ifndef _ADXT_H_INCLUDED
#define _ADXT_H_INCLUDED
/****************************************************************************/
/*																			*/
/*			$title$ ＡＤＸデータ再生 ライブラリ */
/*					ADXT (ADX TALK) Library
 */
/*																			*/
/*				1997.2.25		written M.Oshimi
 */
/*																			*/
/****************************************************************************/

/*	Version number of ADXT library 		*/
#define ADXT_VER "8.34"

#include "cri/ee/cri_xpt.h"
#include "sj.h"

/*	ストリームコントローラ				*/
/*	Stream Controller					*/
#ifndef ADXSTM_DEFINED
#define ADXSTM_DEFINED
typedef void* ADXSTM;
#endif

/*	ストリームジョイントデコーダ		*/
/*	Stream Joint decoder				*/
#ifndef ADXSJD_DEFINED
#define ADXSJD_DEFINED
typedef void* ADXSJD;
#endif

/*	オーディオレンダラ					*/
/*	Audio Renderer						*/
#ifndef ADXRNA_DEFINED
#define ADXRNA_DEFINED
typedef void* ADXRNA;
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/****************************************************************************/
/*		定数マクロ
 */
/*		MACRO CONSTANT
 */
/****************************************************************************/

/*	最大同時発音数						*/
/*	Maximum number of ADXT handle		*/
//	2001.09.14	S.Hosaka
//	バンドル数を8->16
// #define	ADXT_MAX_OBJ		(8)
#define ADXT_MAX_OBJ (16)

/*	最大データサイズ					*/
/*	Maximum data size					*/
#define ADXT_MAX_DATASIZE (0x40000000)

/*	入力バッファのエキストラ領域の大きさ	*/
/*	Size of Extra area in input buffer		*/
#define ADXT_IBUF_XLEN (36)

/*	ADX Talk の出力バッファの大きさ　（サンプル単位）	*/
/*	Output buffer size of ADX Talk (unit:sample)		*/
#define ADXT_OBUF_SIZE (0x2000)
#define DECODE_AHX
#ifdef DECODE_AHX
#define ADXT_OBUF_DIST (0x2060)
#else
#define ADXT_OBUF_DIST (0x2020)
#endif

/*	最大サンプリング周波数				*/
/*	Maximum sampling frequency			*/
#define ADXT_MAX_SFREQ (48000)

/*	ADX Talkの再生方式	(ADXT_CALC_WORKマクロで使用)		*/
/*	Play method of ADX Talk (used 'ADXT_CALC_WORK' macro)	*/
#define ADXT_PLY_MEM (0) /*	メモリからの再生					*/
                         /*	Play memory data					*/
#define ADXT_PLY_STM (1) /*	CDからのストリーム再生				*/
                         /*	Stream play from CD-ROM				*/

#define ADXT_CALC_IBUFSIZE0(nch, sfreq) (25000 * (nch) * ((sfreq) / 1000) / 44)

#define ADXT_CALC_IBUFSIZE(nch, nstm, sfreq)                                                                           \
    ((ADXT_CALC_IBUFSIZE0(nch, sfreq) * ((nstm) + 1) + 2048) / 2048 * 2048 + ADXT_IBUF_XLEN)

#ifdef XPT_TGT_GC
#define ADXT_RNABUF_SIZE (0x1000)
#define ADXT_CALC_OBUFSIZE(nch) ((ADXT_OBUF_DIST + ADXT_RNABUF_SIZE) * (nch) * sizeof(Sint16))
#else
#define ADXT_CALC_OBUFSIZE(nch) (ADXT_OBUF_DIST * (nch) * sizeof(Sint16))
#endif

#define ADXT_CALC_WORK(nch, stmflg, nstm, sfreq)                                                                       \
    (ADXT_CALC_IBUFSIZE(nch, nstm, sfreq) * (stmflg) + ADXT_CALC_OBUFSIZE(nch) + 64)

/*	データ容量の計算		*/
/*	Calc ADX Data size		*/
#define ADXT_CalcDataLen(sec, nch, sfreq) ((sec) * (nch) * (sfreq) * 18 / 32 + 256)

/* AHXワーク領域サイズ		*/
/* Work size of AHX 		*/
#define ADXT_WORKSIZE_AHX (12288) /* 12KB */

/*	入力バッファへの読み込み量						*/
/*	Number of sectors to read into the input buffer	*/
#define ADXT_MAX_CDBSCT (75)
#define ADXT_MIN_CDBSCT (65)
#define ADXT_PREP_RDSCT (25)

/*	$define$ ADX Talkの動作状態(ADXT_STAT_～)	*/
/*	Status of ADX Talk							*/
#define ADXT_STAT_STOP (0)    /*	停止中 								*/
                              /*	During standstill					*/
#define ADXT_STAT_DECINFO (1) /*	ＡＤＸ のヘッダ情報取得中			*/
                              /*	Getting header information			*/
#define ADXT_STAT_PREP (2)    /*	再生準備中 							*/
                              /*	During play preparation				*/
#define ADXT_STAT_PLAYING (3) /*	デコード＆再生中 					*/
                              /*	During decode and play				*/
#define ADXT_STAT_DECEND (4)  /*	デコード終了 						*/
                              /*	Decode end							*/
#define ADXT_STAT_PLAYEND (5) /*	再生終了 							*/
                              /*	Play end							*/
#define ADXT_STAT_ERROR (6)   /*	読み込みエラー発生					*/
                              /*	Read-in error outbreak state		*/

/*	$define$ ADX Talkのエラーコード　(ADXT_ERR_～)	*/
/*	Error code of ADX Talk							*/
#define ADXT_ERR_OK (0)       /*	正常 								*/
                              /*	Normality							*/
#define ADXT_ERR_SHRTBUF (-1) /*	入力バッファエンプティ				*/
                              /*	The input buffer is empty			*/
#define ADXT_ERR_SNDBLK (-2)  /*	サウンドブロックエラー				*/
                              /*	Error of sound block				*/

/*	$define$ フィルタモード	(ADXT_FLTMODE_～)		*/
/*	Filter mode										*/
#define ADXT_FLTMODE_CPU (0)  /*	ＣＰＵによる展開モード				*/
                              /*	by CPU								*/
#define ADXT_FLTMODE_SCSP (1) /*	AICA-DSPによる展開モード			*/
                              /*	by AICA-DSP							*/

/*	$define$ 再生モード	(ADXT_PMODE_～)				*/
/*	Play mode										*/
#define ADXT_PMODE_FNAME (0)  /*	ファイル名または FID による指定		*/
                              /*	Play specified file name or file ID	*/
#define ADXT_PMODE_AFS (1)    /*	AFS による指定						*/
                              /*	Play specified AFS file				*/
#define ADXT_PMODE_MEM (2)    /*	メモリ再生							*/
                              /*	Play data from memory				*/
#define ADXT_PMODE_SJ (3)     /*	ストリームジョイント再生			*/
                              /*	Play data from StreamJoint			*/
#define ADXT_PMODE_SLFILE (4) /*	シームレス連続再生（ファイル）		*/
                              /*	Seamless continuous play from file	*/

/*	$define$ エラーリカバーモード	(ADXT_RMODE_～)	*/
/*	Mode of error recovery							*/
#define ADXT_RMODE_NOACT (0)  /*	リカバー処理しない					*/
                              /*	Do not recover						*/
#define ADXT_RMODE_STOP (1)   /*	自動停止							*/
                              /*	Automatic stop						*/
#define ADXT_RMODE_REPLAY (2) /*	自動再プレイ						*/
                              /*	Automatic replay					*/

/*	パンポットの設定値		*/
/*	Panpot parameter		*/
#define ADXT_PAN_LEFT (-15)
#define ADXT_PAN_CENTER (0)
#define ADXT_PAN_RIGHT (15)
#define ADXT_PAN_AUTO (-128) /*	MONO/STE によって自動的に切り替える	*/
                             /*	Changes automatically by data		*/

/*	ステレオ再生時のチャンネル番号		*/
/*	Channel number playing stereo data	*/
/*	ADXT_CH_L:left, ADXT_CH_R:right		*/
#define ADXT_CH_L (0)
#define ADXT_CH_R (1)

/*	最大再生チャンネル数				*/
/*	Maximum number of play channel		*/
#define ADXT_MAX_NCH (2)

/*	サーバ関数の呼び出される頻度の規定値　60（回/秒）				*/
/*	Default value of frequency called server function(times/sec)	*/
#define ADXT_DEF_SVRFREQ (60)

/*	Default value of output volume(dB)	*/
#define ADXT_DEF_OUTVOL (0)

/*	入力バッファの下限値 (エラー処理用、単位バイト）				*/
/*	Minimum data in the input buffer(byte) (for coping with error)	*/
#define ADXT_MIN_BUFDATA (64)

/*	エラー判別時間　（単位:秒)			*/
/*	Error distinction time(sec)			*/
#define ADXT_EWAIT_PLY (5)
#define ADXT_EWAIT_NOTPLY (20)

/*	インデックスの最大数				*/
/*	Maximum number of index				*/
#define ADXT_MAX_IDX (0xFFFF)

/*	ADXフォーマットタイプ				*/
/*	Format type of ADX					*/
#define ADXT_FMT_ADX (1)
#define ADXT_FMT_AHX (2)

/****************************************************************************/
/*		データ型
 */
/*      Data type declaration */
/****************************************************************************/

/*	ADX Talk オブジェクト構造体		*/
/*	Structure of ADX Talk object	*/
typedef struct _adx_talk {
    /* 0x00 */ Sint8 used;                  /*	使用中か否か					*/
    /* 0x01 */ Sint8 stat;                  /*	動作状態						*/
    /* 0x02 */ Sint8 pmode;                 /*	再生モード						*/
    /* 0x03 */ Sint8 maxnch;                /*	最大再生チャンネル数			*/
    /* 0x04 */ ADXSJD sjd;                  /*	ADXストリームジョイントデコーダ	*/
    /* 0x08 */ ADXSTM stm;                  /*	入力ストリームコントローラ		*/
    /* 0x0C */ ADXRNA rna;                  /*	オーディオレンダラ				*/
    /* 0x10 */ SJ sjf;                      /*	ファイル入力ストリームジョイント*/
    /* 0x14 */ SJ sji;                      /*	入力ストリームジョイント		*/
    /* 0x18 */ SJ sjo[ADXT_MAX_NCH];        /*	出力ストリームジョイント		*/
    /* 0x20 */ Sint8* ibuf;                 /*	入力バッファ					*/
    /* 0x24 */ Sint32 ibuflen;              /*	入力バッファサイズ（バイト単位)	*/
    /* 0x28 */ Sint32 ibufxlen;             /*	入力バッファエクストラサイズ	*/
    /* 0x2C */ Sint16* obuf;                /*	出力バッファ					*/
    /* 0x30 */ Sint32 obufsize;             /*	出力バッファサイズ（サンプル）	*/
    /* 0x34 */ Sint32 obufdist;             /*	出力バッファ間隔（サンプル）	*/
    /* 0x00 */ Sint32 svrfreq;              /*	サーバ関数呼び出し頻度			*/
    /* 0x3C */ Sint16 maxsct;               /*	入力バッファ内の最高セクタ数	*/
    /* 0x3E */ Sint16 minsct;               /*	入力バッファ内の最低セクタ数	*/
    /* 0x40 */ Sint16 outvol;               /*	出力ボリューム					*/
    /* 0x42 */ Sint16 outpan[ADXT_MAX_NCH]; /*	出力パンポットの設定値			*/
    /* 0x46 */ Sint16 unk46;
    /* 0x00 */ Sint32 maxdecsmpl;       /*	最大デコードサンプル数			*/
    /* 0x00 */ Sint32 lpcnt;            /*	ループカウンタ					*/
    /* 0x00 */ Sint32 lp_skiplen;       /*	ループスキップバイト数			*/
    /* 0x00 */ Sint32 trp;              /*	トランスポーズ量 (セント)		*/
    /* 0x00 */ Sint32 wpos;             /*	メディア上の書き込み位置		*/
    /* 0x00 */ Sint32 mofst;            /*	メディア上のオフセット			*/
    /* 0x00 */ Sint16 ercode;           /*	エラーコード					*/
    /* 0x00 */ Sint32 edecpos;          /*	エラー検出用デコード位置		*/
    /* 0x00 */ Sint16 edeccnt;          /*	デコード位置カウンタ			*/
    /* 0x00 */ Sint16 eshrtcnt;         /*	入力ﾊﾞｯﾌｧｴﾝﾌﾟﾃｨｰｶｳﾝﾀ			*/
    /* 0x00 */ Sint8 lpflg;             /*	ループするか否か				*/
    /* 0x00 */ Sint8 autorcvr;          /*	自動的にエラー復帰するか否か	*/
    /* 0x00 */ Sint8 fltmode;           /*	フィルタモード					*/
    /* 0x00 */ Sint8 execflag;          /*	サーバを実行するか否か			*/
    /* 0x00 */ Sint8 pstwait_flag;      /*	音声出力開始待ちフラグ			*/
    /* 0x00 */ Sint8 pstready_flag;     /*	音声出力準備完了フラグ			*/
    /* 0x00 */ Sint8 pause_flag;        /*	一時停止フラグ					*/
    /* 0x00 */ void* amp;               /*	振幅抽出器						*/
    /* 0x00 */ SJ ampsji[ADXT_MAX_NCH]; /*	振幅抽出用入力SJ				*/
    /* 0x00 */ SJ ampsjo[ADXT_MAX_NCH]; /*	振幅抽出用出力SJ				*/
    /* 0x00 */ Sint32 time_ofst;        /*	時刻のオフセット				*/
    /* 0x00 */ Sint32 lesct;            /*	ループ再生時の最終セクタ番号	*/
    /* 0x00 */ Sint32 trpnsmpl;         /*			トラップサンプル番号	*/
    /* 0x00 */ void* lsc;               /*	連結ストリームコントローラ		*/
    /* 0x00 */ Sint8 lnkflg;            /*	連結再生フラグ					*/
    /* 0x00 */ Sint8 rsv;               /*	予約							*/
    /* 0x00 */ Sint16 rsv2;             /*	予約２							*/
    /* 0x00 */ Uint32 tvofst;           /*	スタート時間オフセット			*/
    /* 0x00 */ Uint32 svcnt;            /*	スタートV-Syncカウント			*/
                                        // 2001.11.14 oshimi
    /* 0x00 */ Uint32 decofst;          /*	トータルデコードオフセット		*/
/* 0x00 */ #ifdef __EE__
    /* 0x00 */ Sint32 flush_nsmpl; /*	フラッシュ時に挿入したサンプル数	*/
                                   /* 0x00 */                                                                          \
#endif
    /* 0x00 */ Sint8 unkAC;
    /* 0x00 */ Sint8 padAD[1];
    /* 0x00 */ Char8* unkB0;
    /* 0x00 */ Sint32 padB4;
    /* 0x00 */ Sint32 padB8;
    /* 0x00 */ Sint32 padBC;
    /* 0x00 */ Sint32 padC0;
} ADX_TALK;
typedef ADX_TALK* ADXT;

/*	インデックスデータ	*/
/*	Index data			*/
typedef struct _adxt_idx {
    Uint16 nidx;
    Uint16 top;
} ADXT_IDX;

/*	スレッドパラメータ構造体		*/
/*	Parameter structure of Thread Param	*/
typedef struct {
    int prio_lock;   // Lock Thread priority
    int prio_safe;   // Safe Thread priority
    int prio_vsync;  // Vsync Thread priority
    int prio_main;   // Main Thread Normary priority
    int prio_mwidle; // Middleware Idle Thread priority
} ADXM_TPRM;

/*	スレッドパラメータ構造体(拡張版)				*/
/*	Parameter structure of Extended Thread Param	*/
typedef struct {
    int prio_lock;     // Lock Thread priority
    int prio_safe;     // Safe Thread priority
    int prio_usrvsync; // User Vsync Thread priority
    int prio_vsync;    // Vsync Thread priority
    int prio_main;     // Main Thread Normary priority
    int prio_mwidle;   // Middleware Idle Thread priority
    int prio_usridle;  // User Idle Thread priority
} ADXM_TPRM_EX;

/*	ADXヘッダ情報構造体					*/
/*	Parameter structure of ADX header	*/
typedef struct {
    Sint32 fmt;     // Format type (ADX/AHX)
    Sint32 sfreq;   // Sampling rate
    Sint32 nch;     // Number of channel
    Sint32 bps;     // Number of bit per sample
    Sint32 nsmpl;   // Total number of sample
    Sint32 lptype;  // Loop type
    Sint32 lpstart; // Loop start position
    Sint32 lpend;   // Loop end position
} ADXHINFO;

/****************************************************************************/
/*		関数の宣言
 */
/*      Function Declaration */
/****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* $func$ ADX Talk の初期化
 * [書　式] void ADXT_Init(void);
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] ADX Talk を初期化する。
 *			変数領域の初期化を行う。
 *  Initialization of ADX Talk
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] Initializes ADX Talk. Initializes variable.
 */
void ADXT_Init(void);

/* $func$ ADX Talkの終了
 * [書　式] void ADXT_Finish(void);
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] ADX Talk の終了処理をする。
 *  Termination of ADX Talk
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] Finalizes ADX Talk.
 */
void ADXT_Finish(void);

/*	$func$ すべてのADXTハンドルの消去
 * [書　式] void ADXT_DestroyAll(void);
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] すべてのADXTハンドルを消去する。
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] You can destroy all ADXT handles.
 */
void ADXT_DestroyAll(void);

/*	$func$ すべてのADXTとADXFハンドルの消去
 * [書　式] void ADXT_CloseAllHandles(void);
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] すべてのADXTハンドルとADXFハンドルを消去する。
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] You can destroy all ADXT handles and close all ADXF handles.
 */
void ADXT_CloseAllHandles(void);

/*	$func$ ADXTハンドル の生成
 * [書　式] ADXT ADXT_Create(Sint32 maxnch, void *work, Sint32 worksize);
 * [入　力] maxnch	: 最大再生チャンネル数（モノラルのみ１：ステレオ２）
 *			work	: ワーク領域
 *			worksize: ワーク領域のサイズ
 * [出　力] なし
 * [関数値] ADXTハンドル
 * [機　能] ADXTハンドルを生成する。
 *			work領域のサイズは、ADXT_CALC_WORKマクロによって求める。
 *  Creation of ADXT handle
 * [Inputs  ] maxnch  : Number of maximum channels(monoral:1, stereo:2)
 *			  work    : Working area
 *			  worksize: Size of working area(byte)
 * [Outputs ] None
 * [Return  ] ADXT handle
 * [Function] Creates ADXT handle.
 * [Remarks ] You calculate size of working area used 'ADXT_CALC_WORK' macro.
 */
ADXT ADXT_Create(Sint32 maxnch, void* work, Sint32 worksize);

/* $func$ ADXTハンドル の消去
 * [書　式] void ADXT_Destroy(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] なし
 * [機　能] 指定された ADXTハンドルを消去する。
 *  Destroy of ADXT handle
 * [Inputs  ] adxt   : ADXT handle
 * [Outputs ] None
 * [Return  ] None
 * [Function] Destroys specified ADXT handle.
 */
void ADXT_Destroy(ADXT adxt);

/* $func$ FID 指定による再生の開始
 * [書　式] void ADXT_StartAfs(ADXT adxt, Sint32 patid, Sint32 fid);
 * [入　力] adxt	: ADXTハンドル
 *			patid	: パーティション識別子
 *			fid		: ファイル識別子
 * [出　力] なし
 * [関数値] なし
 * [機　能] パーティション識別子とファイル識別子で指定されたＡＤＸファイルの
 *			再生を開始する。
 *  Play start of ADX data by specified file ID
 * [Inputs  ] adxt	: ADXT handl
 *			  patid	: Partition ID
 *			  fid	: File ID
 * [Outputs ] None
 * [Return  ] None
 * [Function] Starts to play ADX file when you specify partition ID and file ID.
 */
void ADXT_StartAfs(ADXT adxt, Sint32 patid, Sint32 fid);

/* $func$ ファイル名指定による再生の開始
 * [書　式] void ADXT_StartFname(ADXT adxt, Sint8 *fname);
 * [入　力] adxt	: ADXTハンドル
 *			fname	: ファイル名
 * [出　力] なし
 * [関数値] なし
 * [機　能] fname で指定されたＡＤＸファイルの再生を開始する。
 *  Play start of ADX data specified file name
 * [Inputs  ] adxt	: ADXT handle
 *			  fname	: File name
 * [Outputs ] None
 * [Return  ] None
 * [Function] Starts to play ADX file when you specify file name.
 */
void ADXT_StartFname(ADXT adxt, Char8* fname);

/* $func$ ストリームジョイントによる再生の開始
 * [書　式] void ADXT_StartSj(ADXT adxt, SJ sj);
 * [入　力] adxt	: ADXTハンドル
 *			sj		: ストリームジョイント
 * [出　力] なし
 * [関数値] なし
 * [機　能] ストリームジョイントから得られるデータを再生する。
 *  Play start of ADX data from Stream Joint
 * [Inputs  ] adxt	: ADXT handle
 *			  sj	: Stream Joint
 * [Outputs ] None
 * [Return  ] None
 * [Function] Starts to play ADX file when you specify Stream Joint.
 */
void ADXT_StartSj(ADXT adxt, SJ sj);

/* $func$ メモリ指定による再生の開始
 * [書　式] void ADXT_StartMem(ADXT adxt, void *adxdat);
 * [入　力] adxt	: ADXTハンドル
 *			adxdat	: ＡＤＸデータのアドレス
 * [出　力] なし
 * [関数値] なし
 * [機　能] adxdatで指定されたＡＤＸデータを再生する。
 *  Play start of ADX data on memory
 * [Inputs  ] adxt	: ADXT handle
 *			  adxdat: Address of ADX data
 * [Outputs ] None
 * [Return  ] None
 * [Function] Starts to play ADX data on memory when you specify address.
 */
void ADXT_StartMem(ADXT adxt, void* adxdat);

/* $func$ メモリ指定による再生の開始
 * [書　式] void ADXT_StartMem2(ADXT adxt, void *adxdat, Sint32 datlen);
 * [入　力] adxt	: ADXTハンドル
 *			adxdat	: ＡＤＸデータのアドレス
 *			datlen	: ＡＤＸデータの大きさ
 * [出　力] なし
 * [関数値] なし
 * [機　能] adxdata で指定されたＡＤＸデータを再生する。
 *  Play start of ADX data on memory (TYPE 2)
 * [Inputs  ] adxt	: ADXT handle
 *			  adxdat: Address of ADX data
 *			  datlen: Length of playing ADX data
 * [Outputs ] None
 * [Return  ] None
 * [Function] Starts to play ADX data on memory. Plays to specified length.
 */
void ADXT_StartMem2(ADXT adxt, void* adxdat, Sint32 datlen);

/* $func$ メモリ指定による再生の開始
 * [書　式] void ADXT_StartMemIdx(ADXT adxt, void *acx, Sint32 no);
 * [入　力] adxt	: ADXTハンドル
 *			acx		: ＡＣＸデータのアドレス
 *			no		: インデックス番号
 * [出　力] なし
 * [関数値] なし
 * [機　能] acx で指定されたＡＣＸデータのno番目のデータを再生する。
 *  Play start of ACX data
 * [Inputs  ] adxt	: ADXT handle
 *			  acx	: Address of ACX data
 *			  no	: Index number
 * [Outputs ] None
 * [Return  ] None
 * [Function] Starts to play a part of ACX data when you specify index number.
 */
void ADXT_StartMemIdx(ADXT adxt, void* acx, Sint32 no);

/* $func$ 再生の停止
 * [書　式] void ADXT_Stop(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] なし
 * [機　能] ＡＤＸの再生を停止する。
 *  Play stop
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] None
 * [Function] Stops to play ADX data.
 */
void ADXT_Stop(ADXT adxt);

/* $func$ 状態の取得
 * [書　式] Sint32 ADXT_GetStat( ADXT adxt );
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 現在の ADXTハンドルの状態を表す定数
 * 				ADXT_STAT_STOP	 :	停止中
 *				ADXT_STAT_DECINFO:	ＡＤＸ のヘッダ情報取得中
 *				ADXT_STAT_PREP	 :	再生準備中
 *				ADXT_STAT_PLAYING:	デコード＆再生中
 *				ADXT_STAT_DECEND :	デコード終了
 *				ADXT_STAT_PLAYEND:	再生終了
 * [機　能] 現在のADXTハンドルの状態を取得する。
 *  Get status
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] Status of ADXT handle
 * [Function] Obtains status of ADXT handle.
 */
Sint32 ADXT_GetStat(ADXT adxt);

/* $func$ サンプル単位での再生時刻の取得
 * [書　式] void ADXT_GetTime( ADXT adxt, Sint32 *ncount, Sint32 *tscale );
 * [入　力] adxt	: ADXTハンドル
 * [出　力] ncount	: 再生サンプル数
 *			tscale	: サンプリング周波数 [ Hz ]
 * [関数値] なし
 * [機　能] サンプル単位で再生時刻を取得する。
 *  Get play time by sample unit
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] ncount: Number of play sample
 *			  tscale: Sampling frequency(Hz)
 * [Return  ] None
 * [Function] Obtains play time by sample unit.
 */
void ADXT_GetTime(ADXT adxt, Sint32* ncount, Sint32* tscale);

/* $func$ 実時間での再生時刻の取得
 * [書　式] Sint32 ADXT_GetTimeReal(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 再生時刻 [ 1/100sec ]
 * [機　能] 実時間で再生時刻を取得する。
 *  Get play time in real time
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] Play time(1/100sec)
 * [Function] Obtains play time in real time.
 */
Sint32 ADXT_GetTimeReal(ADXT adxt);

/* $func$ 音声データの総サンプル数の取得
 * [書　式] Sint32 ADXT_GetNumSmpl( ADXT adxt );
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 音声データの総サンプル数
 * [機　能] 再生中の ＡＤＸ データの総サンプル数を取得する。
 * [備　考] 状態が、再生準備中(ADXT_STAT_PREP)から再生終了(ADXT_STAT_PLAYEND)
 *			までの時に取得可能。
 *  Get a general sample number of sound data
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] General sample number of sound data
 * [Function] Obtains a general sample number of sound data.
 * [Remarks ] You can obtain a time between status of preparation
 *			  (ADXT_STAT_PREP) and status of play end(ADXT_STAT_PLAYEND).
 */
Sint32 ADXT_GetNumSmpl(ADXT adxt);

/* $func$ 音声データのサンプリング周波数の取得
 * [書　式] Sint32 ADXT_GetSfreq(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 音声データのサンプリング周波数
 * [機　能] 再生中の サンプリング周波数を求める。
 * [備　考] 状態が、再生準備中(ADXT_STAT_PREP)から再生終了(ADXT_STAT_PLAYEND)
 *			までの時に取得可能。
 *  Get sampling frequecy
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] Sampling frequency
 * [Function] Obtains sampling frequency.
 * [Remarks ] You can obtain a sampling frequency between status of preparation
 *			 (ADXT_STAT_PREP) and status of play end(ADXT_STAT_PLAYEND).
 */
Sint32 ADXT_GetSfreq(ADXT adxt);

/* $func$ 音声データのチャンネル数の取得
 * [書　式] Sint32 ADXT_GetNumChan(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 音声データのチャンネル数の取得
 * [機　能] 再生中の音声のチャンネル数を求める。
 * [備　考] 状態が、再生準備中(ADXT_STAT_PREP)から再生終了(ADXT_STAT_PLAYEND)
 *			までの時に取得可能。
 */
Sint32 ADXT_GetNumChan(ADXT adxt);

/* $func$ ヘッダ長の取得
 * [書　式] Sint32 Sint32 ADXT_GetHdrLen(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 音声データのヘッダ長
 * [機　能] 再生中の音声のヘッダ長を求める。
 * [備　考] 状態が、再生準備中(ADXT_STAT_PREP)から再生終了(ADXT_STAT_PLAYEND)
 *			までの時に取得可能。
 */
Sint32 ADXT_GetHdrLen(ADXT adxt);

/* $func$ 音声データの量子化の取得
 * [書　式] Sint32 ADXT_GetFmtBps(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 音声データの量子化ビット数
 * [機　能] 再生中の音声の量子化ビット数を求める。
 * [備　考] 状態が、再生準備中(ADXT_STAT_PREP)から再生終了(ADXT_STAT_PLAYEND)
 *			までの時に取得可能。
 */
Sint32 ADXT_GetFmtBps(ADXT adxt);

/*	$func$ パンポットの設定
 * [書　式] void ADXT_SetOutPan(ADXT adxt, Sint32 ch, Sint32 pan);
 * [入　力] adxt	: ADXTハンドル
 *			ch		: チャネル番号 (0, 1)
 *						ADXT_CH_L=0, ADXT_CH_R=1
 *			pan		: パン設定値 (-15～+15, -128)
 *						ADXT_PAN_LEFT=-15, ADXT_PAN_CENTER=0
 *						ADXT_PAN_RIGHT=15, ADXT_PAN_AUTO=-128
 * [出　力] なし
 * [関数値] なし
 * [機　能] 出力パンポットを設定する。
 *			AUTO の場合は、ADX データがモノラルかステレオかによって自動的に
 *			パンが設定される。
 *  Set panpot
 * [Inputs  ] adxt	: ADXT handle
 *			  ch	: Channel number(ADXT_CH_L:0, ADXT_CH_R:1)
 *			  pan	: Panpot(from -15 to +15, -128)
 *						ADXT_PAN_LEFT=-15, ADXT_PAN_CENTER=0
 *						ADXT_PAN_RIGHT=15, ADXT_PAN_AUTO=-128
 * [Outputs ] None
 * [Return  ] None
 * [Function] Sets panpot of specified channel number.
 *			 Sets automatically panpot by data, monoral or stereo, when you
 *			 specified 'ADXT_PAN_AUTO'.
 */
void ADXT_SetOutPan(ADXT adxt, Sint32 ch, Sint32 pan);

/*	$func$ パンポットの取得
 * [書　式] Sint32 ADXT_GetOutPan(ADXT adxt, Sint32 ch);
 * [入　力] adxt	: ADXTハンドル
 *			ch		: チャネル番号 (0, 1)
 *						ADXT_CH_L=0, ADXT_CH_R=1
 * [出　力] なし
 * [関数値] パンポットの設定値
 * [機　能] 出力パンポットを取得する。
 *  Get panpot
 * [Inputs  ] adxt	: ADXT handle
 *			  ch	: Channel number(ADXT_CH_L:0, ADXT_CH_R:1)
 * [Outputs ] None
 * [Return  ] Panpot
 * [Function] Obtains panpot of specified channel number.
 */
Sint32 ADXT_GetOutPan(ADXT adxt, Sint32 ch);

/*	$func$ 出力ボリュームの設定
 * [書　式] void ADXT_SetOutVol(ADXT adxt, Sint32 vol);
 * [入　力] adxt	: ADXTハンドル
 *			vol		: 減衰レベル　(0:-0dB, -960:-96dB)
 * [出　力] なし
 * [関数値] なし
 * [機　能] 出力ボリュームを設定する。
 *			volの設定値	0    :  -0dB  減衰なし
 *						-30  :  -3dB  約70%
 *						-60  :  -6dB  約50%
 *						-960 : -96dB  最大の減衰量
 *  Set volume
 * [Inputs  ] adxt	: ADXT handle
 *			  vol	: Volume (0:-dB, -960:-96dB)
 * [Outputs ] None
 * [Return  ] None
 * [Function] Sets the volume of a ADXT handle.
 *			 'vol'value	0    :  -0dB  No attenuation
 *						-30  :  -3dB  about 70%
 *						-60  :  -6dB  about 50%
 *						-960 : -96dB  Maximum attenuation
 */
void ADXT_SetOutVol(ADXT adxt, Sint32 vol);

/*	$func$ 出力ボリュームの取得
 * [書　式] Sint32 ADXT_GetOutVol(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 出力ボリュームの設定値　(0:-0dB ～ -960:-96dB)
 * [機　能] 出力ボリュームを取得する。
 *  Get volume
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] Volume (0:-0dB, -960:-96dB)
 * [Function] Obtains the volume for a ADXT handle.
 */
Sint32 ADXT_GetOutVol(ADXT adxt);

/*	$func$ サーバ関数の呼び出し頻度の設定	(１秒当たりの呼び出し回数)
 * [書　式] void ADXT_SetSvrFreq(ADXT adxt, Sint32 freq);
 * [入　力] adxt	: ADXTハンドル
 *			freq	: サーバ関数の呼び出し頻度 (１秒当たりの呼び出し回数)
 * [出　力] なし
 * [関数値] なし
 * [機　能] サーバ関数(ADXT_ExecServer関数)の呼び出し頻度の設定する。
 *			デフォルトでは、60が設定されている。
 *			通常、設定する必要はない。
 *  Set the frequency called server function(times/sec)
 * [Inputs  ] adxt	: ADXT handle
 *			  freq	: Frequency called server function(times/sec)
 * [Outputs ] None
 * [Return  ] None
 * [Function] Sets the frequency called server function(ADXT_ExecServer).
 *			 You do not need to set it usually. Default value is 60.
 */
void ADXT_SetSvrFreq(ADXT adxt, Sint32 freq);

/*	$func$ 再読み込み開始時間の設定
 * [書　式] void ADXT_SetReloadTime(ADXT adxt,float time,Sint32 nch,Sint32 sfreq);
 * [入　力] adxt	: ADXTハンドル
 *			time	: 再読み込み開始時間
 *			nch		: チャンネル数
 *			sfreq	: サンプリング周波数
 * [出　力] なし
 * [関数値] なし
 * [機　能] 入力バッファへの再読み込み開始時間を設定する。
 *			入力バッファ内のデータ量が time 秒より少なくなるとディスクから
 *			データを読み込む。
 *  Set the time of start sector to reload
 * [Inputs  ] adxt	: ADXT handle
 *			  time  : start remain time
 *			  nch   : number of channel
 *			  sfreq : sampling frequency
 * [Outputs ] None
 * [Return  ] None
 * [Function] Sets of the time of start sector to reload into input buffer.
 *			 Reads data from disc when a quantity of data in the input
 *			 buffer becomes less than 'time' [second].
 */
void ADXT_SetReloadTime(ADXT adxt, float time, Sint32 nch, Sint32 sfreq);

/*	$func$ 再読み込み開始セクタ数の設定
 * [書　式] void ADXT_SetReloadSct(ADXT adxt, Sint32 minsct);
 * [入　力] adxt	: ADXTハンドル
 *			minsct	: 再読み込み開始セクタ数
 * [出　力] なし
 * [関数値] なし
 * [機　能] 入力バッファへの再読み込み開始セクタ数を設定する。
 *			入力バッファ内のデータ量が minsct セクタより少なくなるとディスク
 *			からデータを読み込む。
 *  Set the number of start sector to reload
 * [Inputs  ] adxt	: ADXT handle
 *			  minsct: start sector number
 * [Outputs ] None
 * [Return  ] None
 * [Function] Sets of the number of start sector to reload into input buffer.
 *			 Reads data from disc when a quantity of data in the input
 *			 buffer becomes less than 'minsct' [sector].
 */
void ADXT_SetReloadSct(ADXT adxt, Sint32 minsct);

/*	$func$ 入力バッファ内のセクタ数の取得
 * [書　式] Sint32 ADXT_GetNumSctIbuf(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] セクタ数
 * [機　能] 入力バッファに読み込まれているセクタ数を取得する。
 *  Get the amount of data stored in input buffer(sector)
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] Number of sector
 * [Function] Obtains the amount of data that is stored in input buffer.
 */
Sint32 ADXT_GetNumSctIbuf(ADXT adxt);

/*	$func$ 出力バッファ内のサンプル数の取得
 * [書　式] Sint32 ADXT_GetNumSmplObuf(ADXT adxt, Sint32 chno);
 * [入　力] adxt	: ADXTハンドル
 *			chno	: チャンネル番号
 * [出　力] なし
 * [関数値] サンプル数
 * [機　能] 出力バッファ内のサンプル数を取得する。
 */
Sint32 ADXT_GetNumSmplObuf(ADXT adxt, Sint32 chno);

/*	$func$ 入力バッファ内再生時間
 * [書　式] float ADXT_GetIbufRemainTime(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 再生可能時間　(単位：秒)
 * [機　能] 入力バッファにあるデータで再生可能な時間を取得する。
 *			ADXデータのみ対応。
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] Playable time (sec)
 * [Function] Obtains playable time using only data in input buffer.
 *			  You can use this function only playing ADX data.
 */
float ADXT_GetIbufRemainTime(ADXT adxt);

/*	$func$ 入力バッファに十分なデータがあるか否かの検査
 * [書　式] Sint32 ADXT_IsIbufSafety(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] TRUE(1):十分なデータがある、FALSE(0):データ不足
 * [機　能] 入力バッファに十分なデータがあるか否かを検査する。
 *			ADXT_SetReloadSct関数によって指定された再読み込み開始セクタ数以上の
 *			データが入力バッファ内に存在する場合、TRUEが返される。
 *  Check whether the input buffer is having enough data or not
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] TRUE(1):Has enough data, FALSE(0):Data are insufficient
 * [Function] Checks whether the input buffer is having enough data or not.
 *			  Returns 'TRUE' when data more than the number of sector appointed
 *			  exist in the input buffer.
 */
Sint32 ADXT_IsIbufSafety(ADXT adxt);

/*	$func$ エラーリカバリーモードの指定
 * [書　式] void ADXT_SetAutoRcvr(ADXT adxt, Sint32 rmode);
 * [入　力] adxt	: ADXTハンドル
 *			rmode	: エラーリカバリーの方法
 *					  ADXT_RMODE_NOACT, ADXT_RMODE_STOP, ADXT_RMODE_REPLAY
 * [出　力] なし
 * [関数値] なし
 * [機　能] エラーリカバリーの方法を設定する。
 *			ADXT_RMODE_NOACT : エラーリカバリーしない
 *			ADXT_RMODE_STOP  : 自動的に停止し、動作状態がADXT_STAT_STOPになる。
 *			ADXT_RMODE_REPLAY: CDからのデータの供給が途切れたときに、自動的に
 *							   ファイルの先頭から再生する。
 *							   その他の場合は、自動的に停止しする。
 *  Set a mode of error recovery
 * [Inputs  ] adxt	: ADXT handle
 *			  rmode	: Method of error recovery
 *					  ADXT_RMODE_NOACT, ADXT_RMODE_STOP, ADXT_RMODE_REPLAY
 * [Outputs ] None
 * [Return  ] None
 * [Function] Sets the method that recover from error.
 *			 ADXT_RMODE_NOACT : Does not recover
 *			 ADXT_RMODE_STOP  : Stops automatically
 *			 ADXT_RMODE_REPLAY: Replay from the top of a file automatically
 *							    when data supply from CD-ROM broke off.
 *							    Other cases stop automatically.
 */
void ADXT_SetAutoRcvr(ADXT adxt, Sint32 rmode);

/*	$func$ 再生が終了したか否かの検査
 * [書　式] Sint32 ADXT_IsCompleted(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] TRUE(1):再生終了、FALSE(0):再生中
 * [機　能] 再生が終了したか否かを調べる。
 *  Check whether the play finished or not
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] TRUE(1):Finished, FALSE(0):Playing
 * [Function] Checks whether the play finished or not.
 */
Sint32 ADXT_IsCompleted(ADXT adxt);

/*	$func$ 各ハンドルのサーバ関数　（内部状態の更新)
 * [書　式] void ADXT_ExecHndl(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] なし
 * [機　能] 各ハンドルの内部状態を更新する。
 *			ADXT_ExecServer内から呼び出される。
 *  Server function of each handle
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] None
 * [Function] Update the inside status.
 *			  This function is called from 'ADXT_ExecServer' function.
 */
void ADXT_ExecHndl(ADXT adxt);

/*	$func$ サーバ関数　（内部状態の更新)
 * [書　式] void ADXT_ExecServer(void);
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] ライブラリの内部状態を更新する。
 *			V-Sync 毎に呼び出さなければならない。
 *  Server function
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] Update the inside status of library.
 */
void ADXT_ExecServer(void);

/*	$func$ ファイルシステムサーバ関数　（内部状態の更新)
 * [書　式] void ADXT_ExecFsSvr(void);
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] ライブラリの内部状態を更新する。
 *			V-Sync 毎に呼び出さなければならない。
 *			優先度をADXT_ExecServerよりも低くする。
 *  File System Server function
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] Update the inside status of library.
 */
void ADXT_ExecFsSvr(void);

/*	$func$ ファイルシステムサーバが処理中か否か
 * [書　式] Sint32 ADXT_IsActiveFsSvr(void);
 * [入　力] なし
 * [出　力] なし
 * [関数値] TRUE:処理中／FALSE:処理していない
 * [機　能] ファイルシステムサーバが処理中か否かをチェックする。
 *  Check active File System Server function
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] TRUE:active/FALSE:no active
 * [Function] Checks that File System server function is active.
 */
Sint32 ADXT_IsActiveFsSvr(void);

/*	$func$ エラーコードの取得
 * [書　式] Sint32 ADXT_GetErrCode(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] なし
 * [機　能] エラーコードを取得する。
 *  Get error code
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] None
 * [Function] Obtains the error code.
 */
Sint32 ADXT_GetErrCode(ADXT adxt);

/*	$func$ エラーコードのクリア
 * [書　式] void ADXT_ClearErrCode(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] なし
 * [機　能] エラーコードをクリアする。
 *  Clear error code
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] None
 * [Function] Clears the error code.
 */
void ADXT_ClearErrCode(ADXT adxt);

/*	$func$ ループした回数の取得
 * [書　式] Sint32 ADXT_GetLpCnt(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] ループした回数
 * [機　能] ループした回数を取得する。
 *  Get the number of times played a loop
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] Number of times played a loop
 * [Function] Obtains the number of times that played a loop.
 */
Sint32 ADXT_GetLpCnt(ADXT adxt);

/*	$func$ ループフラグの設定
 * [書　式] void ADXT_SetLpFlg(ADXT adxt, Sint32 flg);
 * [入　力] adxt	: ADXTハンドル
 *			flg		: 1=ループする、0=ループしない
 * [出　力] なし
 * [関数値] なし
 * [機　能] ループするか否かを設定する。
 *			再生中は、ループを解除することのみ可能。
 *			再度、ループを設定する時は、再生開始直前に設定すること。
 *  Set the loop flag
 * [Inputs  ] adxt	: ADXT handle
 *			  flg	: 1=loop, 0=not loop
 * [Outputs ] None
 * [Return  ] None
 * [Function] Sets whether play a loop or not.
 *			  You can only release looping play while playing.
 *			  If you set this flag again,
 *			  you have to set just before start playing.
 */
void ADXT_SetLpFlg(ADXT adxt, Sint32 flg);

/*	$func$ 入力ストリームジョイントの取得
 * [書　式] SJ ADXT_GetInputSj(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] ストリームジョイントハンドル
 * [機　能] 入力用ストリームジョイントを取得する。
 *  Get input Stream Joint
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] Stream Joint
 * [Function] Obtains Stream Joint for the input.
 */
SJ ADXT_GetInputSj(ADXT adxt);

/*	$func$ 音声出力開始待ちの設定
 * [書　式] void ADXT_SetWaitPlayStart(ADXT adxt, Sint32 flg);
 * [入　力] adxt	: ADXTハンドル
 *			flg		: 1=ウェイトする、0=音声出力を即座に開始する
 * [出　力] なし
 * [関数値] なし
 * [機　能] 音声の出力の開始待ちを設定する。
 *  Set waiting for starting to output the sound
 * [Inputs  ] adxt	: ADXT handle
 *			  flg	: 1=waits, 0=starts to output sound
 * [Outputs ] None
 * [Return  ] None
 * [Function] Sets waiting for starting to output the sound.
 */
void ADXT_SetWaitPlayStart(ADXT adxt, Sint32 flg);

/*	$func$ 音声出力開始準備完了か否かの判定
 * [書　式] Sint32 ADXT_IsReadyPlayStart(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 1:準備完了／0:準備中
 * [機　能] 音声出力開始の準備が完了か否かを判定する。
 *  Check whether preparations completion
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] None
 * [Function] Checks whether ADXT handle was preparations completion.
 */
Sint32 ADXT_IsReadyPlayStart(ADXT adxt);

/*	$func$ 一時停止の設定
 * [書　式] void ADXT_Pause(ADXT adxt, Sint32 sw);
 * [入　力] adxt	: ADXTハンドル
 *			sw		: 1=一時停止、0=再開
 * [出　力] なし
 * [関数値] なし
 * [機　能] 一時停止するか否かを設定する。
 *  Pause/Continue
 * [Inputs  ] adxt	: ADXT handle
 *			  sw	: 1=pause, 0=continue
 * [Outputs ] None
 * [Return  ] None
 * [Function] Stops temporarily by a specified switch and release temporary
 *			  standstill.
 */
void ADXT_Pause(ADXT adxt, Sint32 sw);

/*	$func$ 一時停止状態の取得
 * [書　式] Sint32 ADXT_GetStatPause(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] 一時停止状態 1=一時停止、0=非一時停止
 * [関数値] なし
 * [機　能] 一時停止状態を取得する。
 *  Pause/Continue
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] 1=pause, 0=not pause
 * [Return  ] None
 * [Function] Obtains the status of pause.
 */
Sint32 ADXT_GetStatPause(ADXT adxt);

/*	$func$ ダイレクトセンドレベルの設定
 * [書　式] void ADXT_SetDrctLvl(ADXT adxt, Sint32 drctlvl)
 * [入　力] adxt	: ADXTハンドル
 *			drctlvl	: ダイレクトセンドレベル　(0dB ～ -45dB)
 * [出　力] なし
 * [関数値] なし
 * [機　能] ダイレクトセンドレベルを設定する。
 */
void ADXT_SetDrctLvl(ADXT adxt, Sint32 drctlvl);

/*	$func$ ダイレクトセンドレベルの取得
 * [書　式] Sint32 ADXT_GetDrctLvl(ADXT adxt)
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] ダイレクトセンドレベル　(0dB ～ -45dB)
 * [機　能] ダイレクトセンドレベルを取得する。
 */
Sint32 ADXT_GetDrctLvl(ADXT adxt);

/*	$func$ エフェクタの設定
 * [書　式] void ADXT_SetFx(ADXT adxt, Sint32 fxch, Sint32 fxlvl);
 * [入　力] adxt	: ADXTハンドル
 *			fxch	: エフェクタの入力チャンネル番号 (0～15)
 *			fxlvl	: エフェクタの入力レベル　(0dB ～ -45dB)
 * [出　力] なし
 * [関数値] なし
 * [機　能] エフェクタへの出力を設定する。
 *  Set the effect
 * [Inputs  ] adxt	: ADXT handle
 *			  fxch	: Input channel number to Effector (0-15)
 *			  fxlvl	: Input level to Effector(from 0 to -45[dB])
 * [Outputs ] None
 * [Return  ] None
 * [Function] Sets output to the effect.
 */
void ADXT_SetFx(ADXT adxt, Sint32 fxch, Sint32 fxlvl);

/*	$func$ エフェクタ設定値の取得
 * [書　式] void ADXT_GetFx(ADXT adxt, Sint32 *fxch, Sint32 *fxlvl);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] fxch	: エフェクタへの入力チャンネル番号 (0～15)
 *			fxlvl	: エフェクタの入力レベル　(0dB ～ -45dB)
 * [関数値] なし
 * [機　能] エフェクタの設定値を取得する。
 *  Get the effect value
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] fxch	: Input channel number to Effector(0-15)
 *			  fxlvl	: Input level to Effector(from 0 to -45[dB])
 * [Return  ] None
 * [Function] Obtains the effect value.
 */
void ADXT_GetFx(ADXT adxt, Sint32* fxch, Sint32* fxlvl);

/*	$func$ フィルタの設定
 * [書　式] void ADXT_SetFilter(ADXT adxt, Sint32 coff, Sint32 q);
 * [入　力] adxt	: ADXTハンドル
 *			coff	: カットオフ周波数
 *			q		: 尖鋭度
 * [出　力] なし
 * [関数値] なし
 * [機　能] フィルタへの出力を設定する。
 *  Set the filter
 * [Inputs  ] adxt	: ADXT handle
 *			  coff	: Cut off frequency
 *			  q		:
 * [Outputs ] None
 * [Return  ] None
 * [Function] Sets the filter.
 */
void ADXT_SetFilter(ADXT adxt, Sint32 coff, Sint32 q);

/*	$func$ フィルタ設定値の取得
 * [書　式] void ADXT_GetFilter(ADXT adxt, Sint32 *coff, Sint32 *q);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] coff	: カットオフ周波数
 *			q		: 尖鋭度
 * [関数値] なし
 * [機　能] フィルタへの出力値を取得する。
 *  Get the filter value
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] coff	: Cut off frequency
 *			  q		:
 * [Return  ] None
 * [Function] Obtains the filter value.
 */
void ADXT_GetFilter(ADXT adxt, Sint32* coff, Sint32* q);

/*	$func$ トランスポーズの設定
 * [書　式] void ADXT_SetTranspose(ADXT adxt, Sint32 transps, Sint32 detune);
 * [入　力] adxt	: ADXTハンドル
 *			transps	: トランスポーズ量
 *			detune	: デチューン量
 * [出　力] なし
 * [関数値] なし
 * [機　能] トランスポーズを設定する。
 *  Set transpose
 * [Inputs  ] adxt		: ADXT handle
 *			  transps	: Amount of transpose
 *			  detune	: Amount of detune
 * [Outputs ] None
 * [Return  ] None
 * [Function] Sets the transpose.
 */
void ADXT_SetTranspose(ADXT adxt, Sint32 transps, Sint32 detune);

/*	$func$ トランスポーズの取得
 * [書　式] void ADXT_GetTranspose(ADXT adxt, Sint32 *transps, Sint32 *detune);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] transps	: トランスポーズ量
 *			detune	: デチューン量
 * [関数値] なし
 * [機　能] トランスポーズ量を取得する。
 *  Get tanspose
 * [Inputs  ] adxt		: ADXT handle
 * [Outputs ] transps	: Amount of transpose
 *			  detune	: Amount of detune
 * [Return  ] None
 * [Function] Obtains the amount of transpose.
 */
void ADXT_GetTranspose(ADXT adxt, Sint32* transps, Sint32* detune);

/*	$func$ エラー関数の登録
 * [書　式] void ADXT_EntryErrFunc(void (*func)(void *obj, Sint8 *msg),
 *																void
 * *obj); [入　力] func	: エラー処理関数 obj		: エラー処理関数の第1引数 [出　力] なし [関数値] なし [機　能]
 * エラー処理関数を登録する。 Entry error callback function [Inputs  ] func	: Error handling function obj	: First
 * argument of error handling function [Outputs ] None [Return  ] None [Function] Entry error callback function.
 */
void ADXT_EntryErrFunc(void (*func)(void* obj, Sint8* msg), void* obj);

/*	$func$ 音声データの放棄
 * [書　式] Sint32 ADXT_DiscardSmpl(ADXT adxt, Sint32 nsmpl)
 * [入　力] adxt	: ADXTハンドル
 *			nsmpl	: サンプル数
 * [出　力] なし
 * [関数値] 捨てたサンプル数
 * [機　能] デコードした音声データを捨てる。
 *  Discard sound data
 * [Inputs  ] adxt	: ADXT handle
 *			  nsmpl	: Number of sample
 * [Outputs ] None
 * [Return  ] sample number discarded
 * [Function] Discards sound data decoded.
 */
Sint32 ADXT_DiscardSmpl(ADXT adxt, Sint32 nsmpl);

/*	リトライ回数の設定	*/
void ADXT_SetNumRetry(Sint32 num);

/* ヘッダ情報の取得 */
void ADXT_GetHdrInfo(Sint8* buf, Sint32 bsize, ADXHINFO* hinfo);

/*	$func$ ファイル読み込みステータスの取得
 * [書　式] Sint32 ADXT_GetStatRead(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 1=読み込み中/読み込み準備中、0=非読み込み中/非読み込み準備中
 * [機　能] 読み込み状態を取得する
 */
Sint32 ADXT_GetStatRead(ADXT adxt);

/***
 *			フィルタ作成用コールバック
 *
 *			デコードする毎に登録された関数が呼び出される。
 *			(*user_flt_cbf)(void *obj, Sint32 ch, void *data, Sint32 dtlen);
 *			obj:	ADXT_EntryFltFunc関数の第３引数
 *			ch:		チャンネル番号　(0:左, 1:右)
 *			data:	展開されたデータ (ADXの場合は、16bitPCM)
 *			dtlen:	展開されたデータのバイト数
 *			※　振幅抽出機能とは共用できない。
 **/

/*	フィルタコールバック関数の登録	*/
void ADXT_EntryFltFunc(ADXT adxt, void (*f)(void* obj, Sint32 ch, void* data, Sint32 dtlen), void* obj);

/* $func$ デコードサンプル数の取得
 * [書　式] Sint32 ADXT_GetDecNumSmpl(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] デコードした音声データの総サンプル数
 * [機　能] デコードした音声データの総サンプル数を取得する。
 * [備　考] 状態が、再生準備中(ADXT_STAT_PREP)から再生終了(ADXT_STAT_PLAYEND)
 *			までの時に取得可能。
 *  Get a decoded sample number of sound data
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] decoded sample number of sound data
 * [Function] Obtains a decoded sample number of sound data.
 * [Remarks ] You can obtain a time between status of preparation
 *			  (ADXT_STAT_PREP) and status of play end(ADXT_STAT_PLAYEND).
 */
Sint32 ADXT_GetDecNumSmpl(ADXT adxt);

/***
 *		シームレス連続再生機能
 *
 *		・ループ設定の無い単純なADX データのみ連続再生可能
 *		・最大16ファイルまで登録可能
 *
 ***/

/*	最大登録ファイル数									*/
/*	Number of maximum files to entry 					*/
#define ADXT_MAX_ENTRY_FILES (16)

/*	$func$ シームレス連続再生ファイルの登録
 * [書　式] void ADXT_EntryFname(ADXT adxt, Sint8 *fname)
 * [入　力] adxt	: ADXTハンドル
 *			fname	: ファイル名
 * [出　力] なし
 * [関数値] なし
 * [機　能] シームレス連続再生用のファイルを登録する。
 *			ループ設定のない単純なADXデータのみ登録できる。
 *  Entry file for seamless continuous play
 * [Inputs  ] adxt	: ADXT handle
 *			  fname	: ADX File name
 * [Outputs ] None
 * [Return  ] None
 * [Function] Entry file for seamless continuous play.
 *			  You can only simple ADX data file without looping.
 */
void ADXT_EntryFname(ADXT adxt, Char8* fname);

/*	$func$ シームレス連続再生サブファイルの登録
 * [書　式] void ADXT_EntryAfs(ADXT adxt, Sint32 patid, Sint32 fid)
 * [入　力] adxt	: ADXTハンドル
 *			patid	: パーティションID
 *			fid		: ファイルID
 * [出　力] なし
 * [関数値] なし
 * [機　能] シームレス連続再生用のサブファイルを登録する。
 *			ループ設定のない単純なADXデータのみ登録できる。
 *  Entry file for seamless continuous play
 * [Inputs  ] adxt	: ADXT handle
 *			  patid	: Partition ID
 *			  fid	: File ID
 * [Outputs ] None
 * [Return  ] None
 * [Function] Entry subfile for seamless continuous play.
 *			  You can only simple ADX data subfile without looping.
 */
void ADXT_EntryAfs(ADXT adxt, Sint32 patid, Sint32 fid);

/*	$func$ シームレス連続再生の開始
 * [書　式] void ADXT_StartSeamless(ADXT adxt)
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] なし
 * [機　能] シームレス連続再生を開始する。
 *  Start seamless continuous play
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] None
 * [Function] Start seamless continuous play.
 */
void ADXT_StartSeamless(ADXT adxt);

/*	$func$ シームレスループ再生の設定
 * [書　式] void ADXT_SetSeamlessLp(ADXT adxt, Sint32 flg)
 * [入　力] adxt	: ADXTハンドル
 *			flg		: 0-ループ再生しない,1-ループ再生する
 * [出　力] なし
 * [関数値] なし
 * [機　能] 現在、登録されているファイルを繰り返し再生する。
 *			ファイルのすべてのデータを読み終わると自動的に再度登録される。
 *  Set seamless loop play
 * [Inputs  ] adxt	: ADXT handle
 *			  flg	: 0-Off, 1-On
 * [Outputs ] None
 * [Return  ] None
 * [Function] You can play all entried files with looping.
 */
void ADXT_SetSeamlessLp(ADXT adxt, Sint32 flg);

/*	$func$ シームレスループ再生の開始
 * [書　式] void ADXT_StartFnameLp(ADXT adxt, Sint8 *fname)
 * [入　力] adxt	: ADXTハンドル
 *			fname	: ファイル名
 * [出　力] なし
 * [関数値] なし
 * [機　能] 指定されたファイルを繰り返し再生する。
 *  Start seamless loop play
 * [Inputs  ] adxt	: ADXT handle
 *			  fname	: ADX File Name
 * [Outputs ] None
 * [Return  ] None
 * [Function] Start seamless loop plaing your specified file (fname).
 */
void ADXT_StartFnameLp(ADXT adxt, Char8* fname);

/*	$func$ シームレス連続再生の解除
 * [書　式] void ADXT_ReleaseSeamless(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] なし
 * [機　能] シームレス連続再生の解除する。
 *  Release seamless continuous play
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] None
 * [Function] Release seamless continuous play.
 */
void ADXT_ReleaseSeamless(ADXT adxt);

/*	$func$ 登録ファイル数の取得
 * [書　式] Sint32 ADXT_GetNumFiles(ADXT adxt)
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 登録されているファイル数
 * [機　能] 登録されているファイル数を取得する。
 *  Get number of entried files
 * [Inputs  ] adxt	: ADXT handle
 * [Outputs ] None
 * [Return  ] Number of Entried Files
 * [Function] Get number of entried files
 */
Sint32 ADXT_GetNumFiles(ADXT adxt);

/*	連結再生の設定　(1:する、0:しない)	*/
void ADXT_SetLnkSw(ADXT adxt, Sint32 sw);

/*	連結再生の取得　(1:する、0:しない)	*/
Sint32 ADXT_GetLnkSw(ADXT adxt);

/*	ファイルの登録をリセット(STOP状態でのみ有効)	*/
void ADXT_ResetEntry(ADXT adxt);

/***
 *			振幅抽出機能
 ***/

/*	振幅取得用作業領域の大きさの計算	*/
Sint32 ADXT_CalcAmpWork(Sint32 maxnch);
/*	振幅取得用作業領域の設定	*/
void ADXT_SetAmpWork(ADXT adxt, void* work, Sint32 wksize);
/*	振幅の取得	*/
Sint32 ADXT_GetAmplitude(ADXT adxt, Sint32 ch);
/*	振幅の取得 (時刻オフセット指定付き)	*/
Sint32 ADXT_GetAmplitude2(ADXT adxt, Sint32 ch, Sint32 msec, Sint32* msec2);

/*	ヘッダ情報の強制挿入	*/
void ADXT_InsertHdrSfa(ADXT adxt, Sint32 nch, Sint32 sfreq, Sint32 nsmpl);
/*	タイムオフセットの設定	*/
void ADXT_SetTimeOfst(ADXT adxt, Sint32 ofst);

/***
 *			Sofdec用拡張関数
 ***/

/*	V-Sync割り込み用関数			*/
/*	Shinobi環境では、初期化時に割り込みに自動的に登録されるので、		*/
/*	使用しません。Naomi環境でSofdec Movieを再生する場合には、			*/
/*	この関数をV-Sync割り込みから実行してください。						*/
void ADXT_VsyncProc(void);

/*	時刻の単位の設定　(0:Sfreq(Audio), 1:V-Sync(NTSC), 2:V-sync(PAL)	*/
void ADXT_SetTimeMode(Sint32 mode);

/* ADXヘッダの判定 */
Sint32 ADXT_IsHeader(Sint8* adr, Sint32 siz, Sint32* hdrsiz);

/* ADX終了コードの判定 */
Sint32 ADXT_IsEndcode(Sint8* adr, Sint32 siz, Sint32* endsiz);
/*
        adr   ：終了判定ポインタ(ADXデータの36バイト境界)
        siz   ：終了判定サイズ (判定に必要なサイズは高々数バイト
                        かもしれませんが、ファイルリードして、バッファに
                        蓄積されているサイズを入力します)
        ndsiz：終了コードサイズ（フッタサイズ）
*/

/* 無音データの強制挿入 */
Sint32 ADXT_InsertSilence(ADXT adxt, Sint32 nch, Sint32 nsmpl);
/*
        nsmpl 　：挿入要求サンプル数 [sample/1ch]
        関数値  ：挿入実行サンプル数 (0 ～ nsmpl)
                          ADXT入力用SJに十分な空きがあれば nsmpl の端数を
                          切り捨てた値が返る。
*/

/*			強制モノラル出力スイッチの設定
[書  式]	void ADXT_SetOutputMono(Sint32 flag);
[引  数]	Sint32 flag		強制モノラル出力フラグ(OFF:0, ON:1)
[戻り値]	なし
[機  能]	ステレオデータを強制的にモノラルデータとして出力する。
[備　考]
*/
void ADXT_SetOutputMono(Sint32 flag);

/***
 *		AHX 用関数
 *		Functions for AHX
 ***/

/*	$func$ AHXの組み込み
 * [書　式] void ADXT_AttachAhx(ADXT adxt, void *work, Sint32 worksize);
 * [入　力] adxt	: ADXTハンドル
 *			work	: AHXワーク領域
 *			worksize: AHXワーク領域のサイズ
 * [出　力] なし
 * [関数値] なし
 * [機　能] AHXの組み込みを行い、AHXの利用を可能にする。
 *  Attach AHX
 * [Inputs  ] adxt    : ADXT handle
 *			  work    : Working area for AHX
 *			  worksize: Size of AHX working area(byte)
 * [Outputs ] None
 * [Return  ] None
 * [Function] Attach AHX, Use of AHX is attained.
 */
void ADXT_AttachAhx(ADXT adxt, void* work, Sint32 worksize);

/*	$func$ AHXの分離
 * [書　式] void ADXT_DetachAhx(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] なし
 * [機　能] AHXを分離し、AHXの利用をやめる。
 *  Detach AHX
 * [Inputs  ] adxt    : ADXT handle
 * [Outputs ] None
 * [Return  ] None
 * [Function] Detach AHX, Unuse of AHX is attained.
 */
void ADXT_DetachAhx(ADXT adxt);

void ADXT_SetKeyString(ADXT adxt, Char8* str);
void ADXT_SetDefKeyString(Char8* str);

void ADXT_SetKeyString(ADXT adxt, Char8* str);
void ADXT_SetDefKeyString(Char8* str);

/***
 *		ADXマネージャ関数
 *		ADX Manager Functions
 ***/

/*	ADXスレッドの初期化					*/
/*	Initialization of ADX Thread 		*/
void ADXM_SetupThrd(ADXM_TPRM* tprm);
/*	ADXスレッドの終了処理				*/
/*	Termination of ADX Thread Extended 	*/
void ADXM_ShutdownThrd(void);

/*	ADXスレッドの初期化(拡張版)				*/
/*	Initialization of ADX Thread Extended 	*/
void ADXM_SetupThrdEx(ADXM_TPRM_EX* tprm);
/*	ADXスレッドの終了処理(拡張版)			*/
/*	Termination of ADX Thread Extended 		*/
void ADXM_ShutdownThrdEx(void);

/*	ユーザVSYNCスレッドへの関数登録				*/
/*	Set callback function on UsrVsync Thread	*/
void ADXM_SetCbUsrVsync(Bool (*func)(void* obj), void* obj);
/*	ユーザアイドルスレッドへの関数登録			*/
/*	Set callback function on UsrIdle Thread		*/
void ADXM_SetCbUsrIdle(Bool (*func)(void* obj), void* obj);
/*	ユーザアイドルサーバの区切り処理						*/
/*	Execution UsrIdle Thread for termination of function	*/
void ADXM_GotoUsrIdleBdr(void);
/*	UsrVsyncサーバ関数の実行	*/
/*	Execution UsrVsync Server	*/
Sint32 ADXM_ExecSvrUsrVsync(void);
/*	UsrIdleサーバ関数の実行		*/
/*	Execution UsrIdle Server	*/
Sint32 ADXM_ExecSvrUsrIdle(void);

/*	エラー関数の登録								*/
void ADXM_SetCbErr(void (*fn)(void* obj, Char8* emsg), void* obj);
/*	負荷測定用関数	*/
void ADXM_SetCbSleepMwIdle(void (*fn)(void*), void* obj);
/*	スレッド初期化が行われたか否かの判定	*/
Bool ADXM_IsSetupThrd(void);

/*	スレッドのロック	*/
void ADXM_Lock(void);
/*	スレッドのアンロック	*/
void ADXM_Unlock(void);
/* ロックレベルの取得 */
Sint32 ADXM_GetLockLevel(void);

/*	V-Sync待ち関数	*/
void ADXM_WaitVsync(void);

/*	V-Sync割り込みハンドラ	*/
Sint32 ADXM_ExecVint(Sint32 arg);

/*	メインスレッドで呼び出す関数	*/
Sint32 ADXM_ExecMain(void);

/***
 *		ADXM_SetupThrdを実行しないときに使用する関数
 ***/
/*	Vsync毎に呼び出す関数	*/
Sint32 ADXM_ExecSvrVsync(void);
/*	メインループ内で呼び出す関数	*/
Sint32 ADXM_ExecSvrMain(void);
/*	ミドルウェア用スレッドで呼び出す関数	*/
Sint32 ADXM_ExecSvrMwIdle(void);

/*	すべてのサーバ処理を呼び出す関数	*/
Sint32 ADXM_ExecSvrAll(void);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
