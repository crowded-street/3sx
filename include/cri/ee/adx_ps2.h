#ifndef _ADX_PS2_H_INCLUDED
#define _ADX_PS2_H_INCLUDED
/************************************************************************/
/*                                                                      */
/*              ADX function for Playstation2(PS2)                      */
/*                                                                      */
/*                  Copyright (c) 2001 CRI-MW                           */
/*                  2001.7.9 written by O.Sato                          */
/*                                                                      */
/************************************************************************/

#include "cri/ee/cri_xpt.h"
#include "cri_adxt.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/*      定数マクロ                                                      */
/*      Macro Constant                                                  */
/************************************************************************/
#define ADXPS2_VER "2.33" // "ADXPS2" Version number

#define ADXPS2_RDMODE_NOWAIT (0) /* ノーウェイト読み込みモード	*/
#define ADXPS2_RDMODE_WAIT (1)   /* ウェイト読み込みモード		*/

#define ADXPS2_PRIO_LOCK (1)    /* ロック時のスレッドプライオリティ	*/
#define ADXPS2_PRIO_SAFE (8)    /* セーフスレッドプライオリティ    	*/
#define ADXPS2_PRIO_VSYNC (16)  /* Vsyncサーバ用スレッドプライオリティ	*/
#define ADXPS2_PRIO_MAIN (24)   /* 通常時メインスレッドプライオリティ	*/
#define ADXPS2_PRIO_MWIDLE (25) /*アイドルスレッドプライオリティ*/

#define ADXPS2_DEBUG_LEVEL_ALL (0)     /* 全てのデバッグ出力を行う 	*/
#define ADXPS2_DEBUG_LEVEL_ERROR (1)   /* エラーのみデバッグ出力を行う	*/
#define ADXPS2_DEBUG_LEVEL_NOTHING (2) /* デバッグ出力を行わない		*/

/*	ファイルキャッシュ情報サイズの計算									*/
/*	Calc size of file cache information									*/
#define ADXPS2_CALC_FCSIZE_DVD(nfile, maxflen) (nfile * sizeof(ADXPS2_FC_DVD) + nfile * (maxflen + 1))
#define ADXPS2_CALC_FCSIZE_HOST(nfile, maxflen) (nfile * sizeof(ADXPS2_FC_HOST) + nfile * (maxflen + 1))

/*	ファイルキャッシュ情報に関する規定値								*/
/*	Default value for file cache information							*/
#define ADXPS2_DEF_NUM_FILE_DVD (40)  /* Number of file(for DVD)		*/
#define ADXPS2_DEF_NUM_FILE_HOST (13) /* Number of file(for Host)	  	*/
#define ADXPS2_DEF_MAXFLEN_DVD (128)  /* File length of file(for DVD)	*/
#define ADXPS2_DEF_MAXFLEN_HOST (128) /* File length of file(for Host)*/

#define ADXPS2_DEF_FCSIZE_DVD (ADXPS2_CALC_FCSIZE_DVD(ADXPS2_DEF_NUM_FILE_DVD, ADXPS2_DEF_MAXFLEN_DVD))
#define ADXPS2_DEF_FCSIZE_HOST (ADXPS2_CALC_FCSIZE_HOST(ADXPS2_DEF_NUM_FILE_HOST, ADXPS2_DEF_MAXFLEN_HOST))

/************************************************************************/
/*      データ型                                                        */
/*      Data type declaration                                           */
/************************************************************************/
/* DVDファイル情報キャッシュ											*/
/* DVD File information cache											*/
typedef struct {
    Sint32 lsn;  /* Logical Sector num 	*/
    Sint32 size; /* File size          	*/
} ADXPS2_FC_DVD;

/* HTCIファイル情報キャッシュ											*/
/* HTCI File information cache											*/
typedef struct {
    Sint32 fd;   /* File descripter		*/
    Sint32 size; /* File size      		*/
} ADXPS2_FC_HOST;

/*	DVDファイルシステムのセットアップパラメータ構造体					*/
/*	Parameter structure DVD setup function								*/
typedef struct _adxps2_sprm_dvd {
    Sint8* rtdir;  /*	ルートディレクトリ名		*/
    Sint32 rdmode; /*	読み込みモード				*/
    Sint32 nrtry;  /*	リトライ回数(0:255回)		*/
    Sint32 speed;  /*	回転速度					*/
    Sint32 dtype;  /*	データタイプ(2048byteなど)	*/
} ADXPS2_SPRM_DVD;

/*	ホストファイルシステムのセットアップパラメータ構造体				*/
/*	Parameter structure Host setup function								*/
typedef struct _adxps2_sprm_host {
    Sint8* rtdir;  /*	ルートディレクトリ名		*/
    Sint32 rdmode; /*	読み込みモード				*/
    char pad[8];
} ADXPS2_SPRM_HOST;

/*** 以下、旧バージョン互換用 ***/
/*	DVD/ホストファイルシステムのファイル情報キャッシュパラメータ構造体	*/
/*	Parameter structure DVD/Host file cache								*/
typedef struct _adxps2_fcprm {
    Sint8* flist;   /*	ファイルリストファイル名	*/
    Sint32 maxflen; /*	ファイル名の最大長			*/
    Sint8* fcbuf;   /*	ファイルキャッシュ領域		*/
    Sint32 fcsize;  /*	ファイルキャッシュサイズ	*/
} ADXPS2_FCPRM;

/*	PS2用スレッドパラメータ構造体		*/
/*	Parameter structure of Thread Param	*/
typedef struct {
    int prio_lock; // Main Thread Lock priority
    int prio_safe; // Safe Thread priority
    int prio_adx;  // ADX Thread priority
    int prio_main; // Main Thread Normary priority
} ADXPS2_TPRM;

/***
 *       PS2 専用関数
 *       Functions for PS2
 ***/

/* $func$ DVDファイルシステムのセットアップ
 * [書　式] void ADXT_SetupDvdFs(ADXT_SPRM *sprm);
 * [入　力] sprm : DVD/Hostファイルシステムセットアップパラメータ構造体
 * [出　力] なし
 * [関数値] なし
 * [機　能] DVDファイルシステムのセットアップする。
 *			ファイルをキャッシングする。
 *			引数にNULLを指定するとキャッシングしない。
 *			ADX_Init関数よりも前に1回だけ呼ぶこと。
 *  Setup of Dvd File Sysytem
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] Setup DVD file system.
 *			  This function shuld be called before ADXT_Init().
 */
void ADXPS2_SetupDvdFs(ADXPS2_SPRM_DVD* sprmd);

/* $func$ ホストファイルシステムのセットアップ
 * [書　式] void ADXT_SetupHostFs(ADXT_SPRM *sprm);
 * [入　力] sprm : DVD/Hostファイルシステムセットアップパラメータ構造体
 * [出　力] なし
 * [関数値] なし
 * [機　能] ホストファイルシステムのセットアップする。
 *			ファイルをキャッシングする。
 *			引数にNULLを指定するとキャッシングしない。
 *			ADX_Init関数よりも前に1回だけ呼ぶこと。
 *  Setup of Host File Sysytem
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] Setup Host file system.
 * 			  This function shuld be called before ADXT_Init().
 */
void ADXPS2_SetupHostFs(ADXPS2_SPRM_HOST* sprmh);

/* $func$ メモリファイルシステムのセットアップ
 * [書　式] void ADXPS2_SetupMemFs(void *sprm);
 * [入　力] sprm : 未使用 (NULL)
 * [出　力] なし
 * [関数値] なし
 * [機　能] メモリファイルシステムのセットアップ。
 *  Setup of Memory File Sysytem
 * [Inputs  ] sprm : unused. (NULL)
 * [Outputs ] None
 * [Return  ] None
 * [Function] Setup Memory file system.
 * 			  This function shuld be called before ADXT_Init().
 */
void ADXPS2_SetupMemFs(void* prm);

/*	ファイルポインタのキャッシング(DVD用)								*/
Sint32 ADXPS2_LoadFcacheDvd(ADXPS2_FCPRM* fcprm);

/*	読み込みモードの設定(DVD用) */
void ADXPS2_SetRdModeDvd(ADXPS2_SPRM_DVD* sprmd);

/*	ファイル情報キャッシュ存在の判定(DVD用)								*/
Sint32 ADXPS2_IsExistFcacheDvd(Sint8* fname);

/*	ファイルオープンモードの設定(Host用)								*/
void ADXPS2_SetRdModeHost(ADXPS2_SPRM_HOST* sprmh);

/*	ファイルポインタのキャッシング(Host用)								*/
Sint32 ADXPS2_LoadFcacheHost(ADXPS2_FCPRM* fcprm);

/*	ファイル情報キャッシュ存在の判定(Host用)							*/
Sint32 ADXPS2_IsExistFcacheHost(Sint8* fname);

/*	デバッグ出力レベルの設定 */
void ADXPS2_SetOutputDebugLevel(Sint32 lv);

/*	V-SYNC割り込み処理 */
/*	ユーザーのV-Sync割り込み処理から呼び出すためのエントリ。			*/
/*	通常使用しない。 */
int ADXPS2_VsyncCallback(int arg);

/*	割り込みハンドラを登録しなおす										*/
void ADXPS2_RestoreVsyncCallback(void);

/*	サーバーの実行 */
/*	通常は使用しない。 */
/*	ADXPS2_SetupUsvrを使用後、メインスレッドから定期的に実行する。		*/
void ADXPS2_ExecServer(void);

/*** 以下、旧バージョン互換用 ***/
/*	スレッドのセットアップ */
/*	ADXサーバ関数をV-Sync割り込みで呼び出すためのスレッドの初期化		*/
void ADXPS2_SetupThrd(ADXPS2_TPRM* tprm);

/*	スレッドのシャットダウン */
void ADXPS2_ShutdownThrd(void);

/*	スレッドのロック */
void ADXPS2_Lock(void);

/*	スレッドのロック解除 */
void ADXPS2_Unlock(void);

/* Unused */
void ADXPS2_SetupUsvr(void);
void ADXPS2_Shutdown(void);

/* $func$ ボイスのセットアップ
 * [書　式] void ADXPS2_SetupVoice(Sint32 nste, Sint32 nmono);
 * [入　力] nste  : 再生するステレオの数
 *          nmono : 再生するモノラルの数
 * [出　力] なし
 * [関数値] なし
 * [機　能] 入力されたステレオ・モノラルの数に合わせて使用するボイスを
 *			セットアップする。
 *  Setup voice resource
 * [Inputs  ] nste  : Number of stereo
 *            nmono : Number of monoral
 * [Outputs ] None
 * [Return  ] None
 * [Function] Setup voice resource from input number of stereo and monoral.
 */
void ADXPS2_SetupVoice(Sint32 nste, Sint32 nmono);

/***
 *			PS2専用関数(ADXM)
 ***/

/*	V-Sync割り込みハンドラ */
int ADXPS2_ExecVint(int arg);
/*	V-Sync待ち関数 */
void ADXPS2_WaitVsync(void);
/*	V-Sync毎に呼ばれる関数 */
Bool ADXPS2_ExecServerEx(void* obj);

/* メインスレッドのスリープ */
void ADXPS2_SleepMainThrd(void);
/* メインスレッドの起床 */
void ADXPS2_WakeupMainThrd(void);
/*	アイドルスレッドのレジューム	*/
void ADXPS2_ResumeIdleThrd(void);

/***
 *			デバッグ用
 ***/

/*	EE-IOP間のデータ流量チェック関数	*/
Sint32 ADXPS2_GetNumDataSjtmp(ADXT adxt, Sint32 chno);
Sint32 ADXPS2_GetNumRoomSjtmp(ADXT adxt, Sint32 chno);
Sint32 ADXPS2_GetNumDataSjiop(ADXT adxt, Sint32 chno);
Sint32 ADXPS2_GetNumRoomSjiop(ADXT adxt, Sint32 chno);

/* Dual Layer DVD */
void ADXPS2_SetDvdLayer(Sint32 layer);
Sint32 ADXPS2_GetDvdLayer(void);

#ifdef __cplusplus
}
#endif

#endif

/* end of file */
