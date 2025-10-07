#ifndef _SJ_H_INCLUDED
#define _SJ_H_INCLUDED
/****************************************************************************/
/*																			*/
/*					ストリームジョイントライブラリヘッダ					*/
/*						 SJ (Stream Joint) Library
 */
/*																			*/
/*				 1998.4.14			written by M.Oshimi
 */
/*																			*/
/****************************************************************************/

#define SJ_VER "6.10" /* Version number */

#include "cri/ee/cri_xpt.h"

/***********************************************************************
 *		処理マクロ
 *		Process MACRO
 ***********************************************************************/

/* 整数アドレスのポインタ型への変換 */
#define SJ_VOID_PTR(adr) ((void*)(adr))

/* 下位nビットをゼロにする
 * 　1,2,8,16ビットの場合、SHは、目的のレジスタだけ使用して2命令で処理
 * できるので、シフト処理にする。
 * 　3～7ビットの場合、SHは、-128までは命令コードにイミディエイトデータ
 * を格納できるので、マスク処理にする。1命令または2命令。
 * 　その他の場合、状況に応じて最適とは限らないが、シフト処理にしておく。
 */
#define SJ_OFF_LSB(val, n) ((((Uint32)(val)) >> (n)) << (n))
#define SJ_OFF_LSB1(val) ((((Uint32)(val)) >> (1)) << (1))
#define SJ_OFF_LSB2(val) ((((Uint32)(val)) >> (2)) << (2))
#define SJ_OFF_LSB3(val) (((Uint32)(val)) & (0xfffffff8))
#define SJ_OFF_LSB4(val) (((Uint32)(val)) & (0xfffffff0))
#define SJ_OFF_LSB5(val) (((Uint32)(val)) & (0xffffffe0))
#define SJ_OFF_LSB6(val) (((Uint32)(val)) & (0xffffffc0))
#define SJ_OFF_LSB7(val) (((Uint32)(val)) & (0xffffff80))
#define SJ_OFF_LSB8(val) ((((Uint32)(val)) >> (8)) << (8))
#define SJ_OFF_LSB16(val) ((((Uint32)(val)) >> (16)) << (16))

/* ポインタを2,4,8,16,32byte境界まで戻す（注意：結果は(void*)型） */
#define SJ_BACK_BND2(ptr) SJ_VOID_PTR(SJ_OFF_LSB1(ptr))
#define SJ_BACK_BND4(ptr) SJ_VOID_PTR(SJ_OFF_LSB2(ptr))
#define SJ_BACK_BND8(ptr) SJ_VOID_PTR(SJ_OFF_LSB3(ptr))
#define SJ_BACK_BND16(ptr) SJ_VOID_PTR(SJ_OFF_LSB4(ptr))
#define SJ_BACK_BND32(ptr) SJ_VOID_PTR(SJ_OFF_LSB5(ptr))

/* ポインタを2,4,8,16,32byte境界まで進める（注意：結果は(void*)型） */
#define SJ_FORE_BND2(ptr) SJ_BACK_BND2(((Uint32)(ptr)) + 1)
#define SJ_FORE_BND4(ptr) SJ_BACK_BND4(((Uint32)(ptr)) + 3)
#define SJ_FORE_BND8(ptr) SJ_BACK_BND8(((Uint32)(ptr)) + 7)
#define SJ_FORE_BND16(ptr) SJ_BACK_BND16(((Uint32)(ptr)) + 15)
#define SJ_FORE_BND32(ptr) SJ_BACK_BND32(((Uint32)(ptr)) + 31)

/* 32byte境界の領域確保のために、領域サイズを水増しする */
#define SJ_SIZ_BND32(siz) ((siz) + 32)

/* SJバッファアドレスの32バイト境界の取得 */
#define SJ_ADR_BND32(adr) SJ_FORE_BND32(adr)

/* 境界内の先端と個数から終端を求める */
#define SJ_GET_ENDPOS(bgn, num) ((bgn) + (num) - 1)

/* 境界内の個数（境界値：bgn <= end） */
#define SJ_INNER_NUM(bgn, end) ((end) - (bgn) + 1)

/* 境界内の判定（境界値：bgn <= x <= end） */
#define SJ_IS_INNER(x, bgn, end) (((x) >= (bgn)) && ((x) <= (end)))

/***********************************************************************
 *		データ型
 *		Data type declaration
 ***********************************************************************/

/*	データラインの定義		*/
/*	Data line definition	*/
#define SJ_LIN_FREE (0) /*	フリーチャンクライン				*/
                        /*	Free chunk line						*/
#define SJ_LIN_DATA (1) /*	データチャンクライン				*/
                        /*	Data chunk line						*/

#define SJ_ERR_FATAL (-1)
#define SJ_ERR_INTERNAL (-2)
#define SJ_ERR_PRM (-3)

/*	チャンクの上限サイズ	*/
/*	Maximum size of chunk	*/
#define SJCK_LEN_MAX (0x7fffffff)

#ifndef UUID_DEFINED
#define UUID_DEFINED
typedef struct _UUID {
    Uint32 Data1;
    Uint16 Data2;
    Uint16 Data3;
    Uint8 Data4[8];
} UUID;
#define uuid_t UUID
#endif

/*	データチャンク	*/
/*	Data chunk		*/
#ifndef SJCK_DEFINED
#define SJCK_DEFINED
typedef struct {
    Sint8* data; /*	開始アドレス						*/
                 /*	Begin address						*/
    Sint32 len;  /*	バイト数							*/
                 /*	Size of chunk length				*/
} SJCK;
#endif

/*	ライブラリハンドル	*/
/*	SJ handle			*/
typedef struct {
    struct _sj_vtbl* vtbl; /*	インタフェース						*/
                           /*	Interfaces							*/
} SJ_OBJ;
#ifndef SJ_DEFINED
#define SJ_DEFINED
typedef SJ_OBJ* SJ;
#endif

/*	インタフェース関数  				*/
/*	Interface functions					*/
typedef struct _sj_vtbl {
    void (*QueryInterface)(); /*		for COM compatibility			*/
    void (*AddRef)();         /*		for COM compatibility			*/
    void (*Release)();        /*		for COM compatibility			*/
    void (*Destroy)(SJ sj);   /*		ハンドルの消去					*/
                              /*		Destroys specified SJ handle	*/
    UUID* (*GetUuid)(SJ sj);  /*		UUIDの取得						*/
                              /*		Get a UUID						*/
    void (*Reset)(SJ sj);     /*		リセット						*/
                              /*		Resets SJ						*/
    /*	チャンクの取得	(FIFOの先頭から取得)								*/
    /*	Get a chunk	(Get chunk from top of FIFO)							*/
    void (*GetChunk)(SJ sj, Sint32 id, Sint32 nbyte, SJCK* ck);
    /*	チャンクを戻す　(FIFOの先頭に挿入)									*/
    /*	Unget a chunk	(Insert chunk in top of FIFO)						*/
    void (*UngetChunk)(SJ sj, Sint32 id, SJCK* ck);
    /*	チャンクを挿入	(FIFOの最後に挿入)									*/
    /*	Put a chunk	(Insert chunk in last of FIFO)							*/
    void (*PutChunk)(SJ sj, Sint32 id, SJCK* ck);
    /*	取得できるデータのバイト数の取得									*/
    /*	Get the data number which can be got								*/
    Sint32 (*GetNumData)(SJ sj, Sint32 id);
    /*	チャンクの取得可能の判定 */
    /*	Check whether can get chunk */
    Sint32 (*IsGetChunk)(SJ sj, Sint32 id, Sint32 nbyte, Sint32* rbyte);
    /*	エラー発生時に起動する関数の登録									*/
    /*	Entry error callback function										*/
    void (*EntryErrFunc)(SJ sj, void (*func)(void* obj, Sint32 ecode), void* obj);
} SJ_IF;
typedef SJ_IF* SJIF;

/*	ストリームジョイントの消去 */
/*	Destroys specified SJ handle */
#define SJ_Destroy(sj) (*(sj)->vtbl->Destroy)(sj)
/*	ストリームジョイントのリセット */
/*	Resets SJ
 */
#define SJ_Reset(sj) (*(sj)->vtbl->Reset)(sj)
/*	チャンクの取得
 */
/*	Get a chunk
 */
#define SJ_GetChunk(sj, id, nbyte, ck) (*(sj)->vtbl->GetChunk)(sj, id, nbyte, ck)
/*	チャンクを戻す
 */
/*	Unget a chunk
 */
#define SJ_UngetChunk(sj, id, ck) (*(sj)->vtbl->UngetChunk)(sj, id, ck)
/*	チャンクを挿入
 */
/*	Put a chunk
 */
#define SJ_PutChunk(sj, id, ck) (*(sj)->vtbl->PutChunk)(sj, id, ck)
/*	取得可能なデータのバイト数を求める */
/*	Get the data number which can be got									*/
#define SJ_GetNumData(sj, id) (*(sj)->vtbl->GetNumData)(sj, id)
/*	ストリームジョイントUUIDの取得 (UUIDへのアドレス)						*/
/*	Get a UUID
 */
#define SJ_GetUuid(sj) (*(sj)->vtbl->GetUuid)(sj)
/*	エラー発生時に起動するコールバック関数の登録							*/
/*	Entry error callback function */
#define SJ_EntryErrFunc(sj, f, o) (*(sj)->vtbl->EntryErrFunc)(sj, f, o)
/*	チャンクの取得可能の判定 */
/*	Check whether can get chunk */
#define SJ_IsGetChunk(sj, id, nbyte, rbyte) (*(sj)->vtbl->IsGetChunk)(sj, id, nbyte, rbyte)

/****************************************************************************
 *		関数の宣言
 *		Function Declaration
 ****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*	$func$ チャンクの分解
 * [書　式] void SJ_SplitChunk(SJCK *ck, Sint32 nbyte,
 *													SJCK *ck1, SJCK
 * *ck2); [入　力] ck		: 分解対象となるチャンク nbyte	: ck1 のバイト数 [出　力] ck1		:
 * 分解された前半のチャンク ck2		: 分解された後半のチャンク [関数値] ck2	チャンクのバイト数 [機　能]
 * ckチャンクをck1チャンクとck2チャンクに分解する。 ck1チャンクは、nbyte の長さになる。 ck1チャンクの大きさが、nbyte
 * に満たない場合は、分解されず、 ck2チャンクの長さは、0となる。 ck1とckは、同じものを指定しても良い。
 *			単に分解する場合は、以下のように記述する。
 *				SJ_SplitChunk(&ck, 100, &ck, &ck2);
 *
 * [Inputs  ] ck	: Target chunk
 *			  nbyte	: Size of ck1(byte)
 * [Outputs ] ck1	: Split chunk of the first half
 *			  ck2	: Split chunk of the latter half
 * [Return  ] None
 * [Function] Split a chunk.
 */
void SJ_SplitChunk(SJCK* ck, Sint32 nbyte, SJCK* ck1, SJCK* ck2);

/***
 *		タグ操作関数
 *			タグフォーマット
 *				0-6		タグ文字列
 *				7		0(=\0)
 *				8-14	データ長 (１６進文字列)
 *				15		0(=\0)
 ***/

/*	$func$ タグの生成
 * [書　式] void SJ_MakeTag(SJCK *ck, char *tag);
 * [入　力] ck		: タグを生成するチャンク
 *			tag		: タグ文字列
 * [出　力] なし
 * [関数値] なし
 * [機　能] ckチャンク内にタグを生成する。タグの持つ長さはck.len-16バイトとなる。
 * [Inputs  ] ck	: Target chunk
 *			  tag	: TAG strings
 * [Outputs ] none
 * [Return  ] None
 * [Function] Creating tag.
 */
void SJ_MakeTag(SJCK* ck, char* tag);

/*	$func$ タグの内容の取得
 * [書　式] void SJ_GetTagContent(Sint8 *tag, SJCK *cko);
 * [入　力] tag		: タグ文字列
 * [出　力] ck		: タグの内容となるチャンク
 * [関数値] なし
 * [機　能] タグからその内容を示すチャンクを取得する。
 * [Inputs  ] tag	: TAG strings
 * [Outputs ] ck	: chunk
 * [Return  ] None
 * [Function] Creating chunk from TAG.
 */
void SJ_GetTagContent(Sint8* tag, SJCK* cko);

/*	$func$ タグの検索
 * [書　式] Sint8 *SJ_SearchTag(SJCK *cki, Char8 *kw, Char8 *endkw, SJCK *cko);
 * [入　力] cki		: 検索対象となるチャンク
 *			kw		: 目的とするタグのキーワード
 *			endkw	: このキーワードを持つタグを見つけたときは検索を中断する。
 *					  NULLが指定されたときは、検索を中断しない。
 * [出　力] ck		: タグの内容となるチャンク。
 *					  見つからなかったときは、cko->data=NULL, cko->len=0となる。
 * [関数値] 見つかったタグのアドレス。見つからなかったときはNULLが返る。
 * [機　能] タグからその内容を示すチャンクを取得する。
 * [Inputs  ] cki	: chunk for searching
 *			　kw	: Keyword of tag
 *			  endkw : Serach end keyword
 * [Outputs ] cko	: target chunk
 * [Return  ] tag
 * [Function] Searching chunk.
 */
Sint8* SJ_SearchTag(SJCK* cki, Char8* kw, Char8* endkw, SJCK* cko);

/***
 *		リングバッファタイプのストリームジョイント
 *		Stream joint of Ring buffer types
 ***/

/*	初期化
 */
/*	Initialize
 */
void SJRBF_Init(void);
/*	終了処理
 */
/*	Finish
 */
void SJRBF_Finish(void);
/*	リングバッファの生成 */
/*	Create a Ring buffer */
SJ SJRBF_Create(Sint8* buf, Sint32 bsize, Sint32 xsize);

/***
 *		メモリタイプのストリームジョイント
 *		Stream joint of Memory buffer types
 ***/

/*	初期化
 */
/*	Initialize
 */
void SJMEM_Init(void);
/*	終了処理
 */
/*	Finish
 */
void SJMEM_Finish(void);
/*	メモリストリームの生成 */
/*	Create a Memory buffer */
SJ SJMEM_Create(Sint8* data, Sint32 bsize);

/***
 *		汎用ストリームジョイント
 *		General Stream joint
 ***/

/*	チャンクの結合モード		*/
/*	Combination mode of chunk	*/
#define SJUNI_MODE_SEPA (0)
#define SJUNI_MODE_JOIN (1)

#define SJUNI_CALC_WORK(nck) ((nck) * 16)

/*	初期化
 */
/*	Initialize
 */
void SJUNI_Init(void);
/*	終了処理
 */
/*	Finish
 */
void SJUNI_Finish(void);
/*	汎用ストリームジョイントの生成 */
/*	Create a Stream joint */
SJ SJUNI_Create(Sint32 mode, Sint8* work, Sint32 wksize);
/*	チャンク数の取得
 */
/*	Get chunk number
 */
Sint32 SJUNI_GetNumChunk(SJ sj, Sint32 id);
/*	チェインプール数の取得 */
/*	Get Chain pool	number */
Sint32 SJUNI_GetNumChainPool(SJ sj);

#if 0
/*	GetChunk関数が呼ばれたときのコールバック関数の登録						*/
void SJUNI_EntryGetFunc(SJ sj, Sint32 id, void (*f)(void *o, SJCK *ck), void *o);
/*	PutChunk関数が呼ばれたときのコールバック関数の登録						*/
void SJUNI_EntryPutFunc(SJ sj, Sint32 id, void (*f)(void *o, SJCK *ck), void *o);
/*	UngetChunk関数が呼ばれたときのコールバック関数の登録					*/
void SJUNI_EntryUngetFunc(SJ sj, Sint32 id, void (*f)(void *o, SJCK *ck), void *o);
#endif

/***
 *		共通関数
 ***/

/*	クリティカルセクションへの進入	*/
void SJCRS_Lock(void);
/*	クリティカルセクションからの脱出	*/
void SJCRS_Unlock(void);

#ifdef __cplusplus
}
#endif

#endif /*  #ifndef _SJ_H_INCLUDED	*/
