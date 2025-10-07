#ifndef _CRI_ADXF_H_INCLUDED
#define _CRI_ADXF_H_INCLUDED
/****************************************************************************/
/*																			*/
/*			$title$ ＡＤＸファイルシステム ライブラリ						*/
/*				ADXF (ADX File System) Library */
/*																			*/
/*				1998.7.3		written by S.Hosaka
 */
/*																			*/
/****************************************************************************/

/****************************************************************************/
/*      インクルードファイル */
/*      Include file */
/****************************************************************************/

#include "types.h"

#include "cri/ee/cri_xpt.h"
#include "sj.h"

/****************************************************************************/
/*		定数マクロ
 */
/*		MACRO CONSTANT
 */
/****************************************************************************/

/*	Version number of ADX File System	*/
#define ADXF_VER "6.33"

/*	ファイル名の文字列長				*/
/*	Maximum number of file name strings	*/
#define ADXF_FNAME_MAX (256)

/*	最大パーティション数				*/
/*	Maximum number of partition			*/
#define ADXF_PART_MAX (256)

/*	最大ファイル数						*/
/*	Maximum number of file				*/
#define ADXF_FILE_MAX (65536)

/*	ADXFハンドルオブジェクトの個数		*/
/*	Maximum number of ADXF handle		*/
#define ADXF_OBJ_MAX (16)

/*	ハンドルの状態						*/
/*	Status of handle					*/
#define ADXF_STAT_STOP (1)    /*	停止中						*/
                              /*	During standstill			*/
#define ADXF_STAT_READING (2) /*	データ読み込み中			*/
                              /*	During data read-in			*/
#define ADXF_STAT_READEND (3) /*	データ読み込み終了			*/
                              /*	Data read-in end			*/
#define ADXF_STAT_ERROR (4)   /*	読み込みエラー発生			*/
                              /*	Read-in error outbreak state*/

/*	シークタイプ						*/
/*	Type of seek						*/
#define ADXF_SEEK_SET (0) /*	ファイルの先頭				*/
                          /*	The beginning of file		*/
#define ADXF_SEEK_CUR (1) /*	現在の位置					*/
                          /*	The current position		*/
#define ADXF_SEEK_END (2) /*	ファイルの終端				*/
                          /*	The end of file				*/

/*	エラーコード						*/
/*	Error code							*/
#define ADXF_ERR_OK (0)        /*	正常終了					*/
                               /*	Successful					*/
#define ADXF_ERR_FATAL (-1)    /*	外部のエラー				*/
                               /*	Fatal error					*/
#define ADXF_ERR_INTERNAL (-2) /*	内部のエラー				*/
                               /*	Internal error				*/
#define ADXF_ERR_PRM (-3)      /*	不正なパラメータ			*/
                               /*	Illegal parameter			*/
#define ADXF_ERR_AFS_FILE (-4) /*	不正なAFSファイル			*/
                               /*	Illegal AFS file			*/
#define ADXF_ERR_FSIZE (-5)    /*	ファイルサイズ未取得		*/
                               /*	Cannot get file size yet	*/

/*	既定値								*/
/*	Default value						*/
#define ADXF_DEF_SCT_SIZE (2048) /*	CD-ROMのセクタサイズ		*/
                                 /*	Sector size in CD-ROM		*/
#ifdef __EE__
#define ADXF_DEF_DMA_ALIGN (64) /*	DMAのアライメント			*/
                                /*	Alignment of DMA transfer	*/
#define ADXF_DEF_ALIGN_CALC (0xFFFFFFC0)

#else

#define ADXF_DEF_DMA_ALIGN (32) /*	DMAのアライメント			*/
/*	Alignment of DMA transfer	*/
#define ADXF_DEF_ALIGN_CALC (0xFFFFFFE0)
#endif

#define ADXF_DEF_REQ_RD_SCT (512) /*	リードリクエストサイズ		*/
                                  /*	read request size			*/

/*	デバッグ用	*/
/*	for Debug	*/
// #define	ADXF_CMD_HSTRY_MAX		(256)		/*	コマンド履歴の個数			*/
#define ADXF_CMD_HSTRY_MAX (16) /*	コマンド履歴の個数			*/
                                /*	Number of command history	*/

#define ADXF_CMD_OPEN (1)      /*	ADXF_Open					*/
#define ADXF_CMD_OPEN_AFS (2)  /*	ADXF_OpenAfs				*/
#define ADXF_CMD_CLOSE (3)     /*	ADXF_Close					*/
#define ADXF_CMD_READ_NW32 (4) /*	ADXF_ReadNw32				*/
#define ADXF_CMD_STOP (5)      /*	ADXF_Stop					*/
#define ADXF_CMD_SEEK (6)      /*	ADXF_Seek					*/
#define ADXF_CMD_STOP_NW (7)   /*	ADXF_StopNw					*/

#define ADXF_CMD_NUM_MAX (16)

/****************************************************************************/
/*		列挙定数
 */
/*		Enumarate Constants
 */
/****************************************************************************/

/****************************************************************************/
/*		処理マクロ
 */
/*		Process MACRO
 */
/****************************************************************************/

/*	パーティション情報に必要な領域の計算						*/
/*	Calculation of necessary area for partition information		*/
//	"6"は、adxf_ptinfoのメンバー数(adxf_ptinfo+size+nfile+nentry+type+rev+curdir+ofst)
#define ADXF_PTIF_CMN_SZ ((6 * sizeof(Sint32)) + (ADXF_FNAME_MAX * sizeof(Sint8)))

#define ADXF_CALC_PTINFO_REAL_SIZE(n) (ADXF_PTIF_CMN_SZ + (((n) + 1) * sizeof(Sint16)))

#define ADXF_CALC_PTINFO_SIZE(n) ((ADXF_CALC_PTINFO_REAL_SIZE(n) + sizeof(Sint16)) / 4 * 4)

#define ADXF_CALC_ADD_PTINFO_SIZE(n) (ADXF_PTIF_CMN_SZ + ((n) * (2 * sizeof(Sint16) + sizeof(Sint32))))

/*	ロードパーティション用テンポラリバッファのサイズ計算	*/
/*	Calculation of temporary buffer size for load-partition	*/
#define ADXF_CALC_TMPBUF_SIZE(n) ((((n + 1) * 8) + (ADXF_DEF_SCT_SIZE - 1)) / ADXF_DEF_SCT_SIZE * ADXF_DEF_SCT_SIZE)

/****************************************************************************/
/*		データ型
 */
/*      Data type declaration */
/****************************************************************************/

#ifndef ADXSTM_DEFINED
#define ADXSTM_DEFINED
typedef void* ADXSTM;
#endif

/*	ADXFハンドル		*/
/*	ADXF handle			*/
#ifndef ADXF_DEFINED
#define ADXF_DEFINED
typedef struct _adx_fs {
    Sint8 used;       /*	使用中か否か						*/
    Sint8 stat;       /*	ハンドルの状態						*/
    Sint8 sjflag;     /*	外部から設定されたSJか否か			*/
    Sint8 stopnw_flg; /*	停止要求フラグ						*/
    ADXSTM stm;       /*	ADXSTMハンドル						*/
    SJ sj;            /*	ストリームジョイント				*/
    Sint32 fnsct;     /*	ファイルサイズ(セクタ)				*/
    Sint32 fsize;     /*	ファイルサイズ(バイト)				*/
    Sint32 skpos;     /*	アクセスポインタの位置				*/
    Sint32 rdstpos;   /*	読み込み開始位置					*/
    Sint32 rqsct;     /*	読み込み要求データ量(セクタ)		*/
    Sint32 rdsct;     /*	読み込んだデータ量(セクタ)			*/
    Sint8* buf;       /*	バッファのアドレス(バイト)			*/
    Sint32 bsize;     /*	バッファのサイズ(バイト)			*/
    Sint32 rqrdsct;   /*	実際に読み込み要求される量(セクタ)	*/
    /*	ADXSTM_OpenRange -> ADXSTM_OpenFileRange(1999.8.19)	*/
    Sint32 ofst; /*	読み込みファイルへのオフセット		*/
                 //	Sint8	fname[ADXF_FNAME_MAX];	/*	ファイル名							*/
    void* dir;
    const Char8* unk38;
    Sint32 unk3C;
    Sint32 unk40;
} ADX_FS;
typedef ADX_FS* ADXF;
#endif

/*	パーティション情報		*/
/*	Partition information	*/
#ifndef ADXF_PTINFO_DEFINED
#define ADXF_PTINFO_DEFINED
typedef struct _adxf_ptinfo {
    struct _adxf_ptinfo* next; /*	次のパーティション(NULL:オリジナル)	*/
    Sint32 size;               /*	パーティション情報領域サイズ		*/
    Sint32 nfile;              /*	パーティション内のファイル数		*/
    Uint16 nentry;             /*	パーティション内の登録ファイル数	*/
    Sint8 type;                /*	オリジナルか追記かのフラグ			*/
    Sint8 rev;                 /*	予約領域							*/
    /*	ADXSTM_OpenRange -> ADXSTM_OpenFileRange(1999.8.19)	*/
    Sint8 fname[ADXF_FNAME_MAX]; /*	AFSファイル名			*/
    void* curdir;                /*	カレントディレクトリ(NULL:指定なし)	*/
    Sint32 ofst;                 /*	入れ子AFSファイル用					*/
    Sint32 top;                  /*	ファイル情報の先頭					*/
    Sint32 file_sizes[0];
} ADXF_PTINFO;
#endif

/*	追記ファイルの情報				*/
/*	Information of postscript file	*/
#ifndef ADXF_ADD_DEFINED
#define ADXF_ADD_DEFINED
typedef struct _adxf_add_info {
    Uint16 flid;  /*	ファイルID							*/
    Uint16 fnsct; /*	ファイルサイズ						*/
    Sint32 ofst;  /*	オフセット							*/
} ADXF_ADD_INFO;
#endif

#ifndef ADXF_CMD_HSTRY_DEFINED
#define ADXF_CMD_HSTRY_DEFINED
typedef struct _adxf_cmd_hstry {
    Uint8 cmdid;     /*	コマンド(関数)ID					*/
    Uint8 fg;        /*	関数の入り口か出口かを示すフラグ	*/
    Uint16 ncall;    /*	コマンドの呼び出し回数				*/
    intptr_t prm[3]; /*	コマンドパラメータ					*/
} ADXF_CMD_HSTRY;
#endif

/****************************************************************************/
/*		変数宣言
 */
/*		Variable Declaration */
/****************************************************************************/

/****************************************************************************/
/*		関数の宣言
 */
/*      Function Declaration */
/****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*==========================================================================
                ライブラリの初期化と終了処理
                Initialize and Finalize of Library
  ==========================================================================*/

/*  ライブラリの初期化
 * [書　式] void ADXF_Init( void );
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] ライブラリを初期化する。
 *  Initialization of library
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] Initializes library.
 */
void ADXF_Init(void);

/* ライブラリの終了処理
 * [書　式] void ADXF_Finish(void);
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] ライブラリの終了処理をする。
 *  Termination of library
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] Finalizes library.
 */
void ADXF_Finish(void);

/*  即時復帰型パーティションのロード
 * [書　式] Sint32 ADXF_LoadPartitionNw(Sint32 ptid, Sint8 *fname,
 * 													void *dir, void
 **ptinfo); [入　力] ptid	:パーティションID fname	:AFSファイルの名前 dir		:ディレクトリ情報
 *(GDFS:GDFS_DIRREC) ptinfo	:パーティション情報読み込み領域 [出　力] なし [関数値] エラーコード [機　能]
 *AFSファイルを読み込んで、パーティションIDに設定する。 Load of the partition(No wait) [Inputs  ] ptid   :Partition ID
 *			  fname  :Name of AFS file
 *            dir	 :Directory information (GDFS:GDFS_DIRREC)
 *            ptinfo :Area of partition information
 * [Outputs ] None
 * [Return  ] Error code
 * [Function] Loads AFS file and sets partition ID.
 */
Sint32 ADXF_LoadPartitionNw(Sint32 ptid, Char8* fname, void* dir, void* ptinfo);

/*  即時復帰型パーティションのロード(入れ子AFSファイル)
 * [書　式] Sint32 ADXF_LoadPartitionFromPtNw(Sint32 set_ptid, Sint32 rd_ptid,
 *											Sint32 rd_flid, void *ptinfo);
 * [入　力] set_ptid	:ロードするAFSファイルに対して設定するパーティションID
 *			rd_ptid		:読み込むAFSファイルのパーティションID
 *			rd_flid		:読み込むAFSファイルのファイルID
 *			ptinfo		:パーティション情報読み込み領域
 * [出　力] なし
 * [関数値] エラーコード
 * [機　能] AFSファイル内のAFSファイルを読み込んで、パーティションIDに設定する。
 *  Load of the partition(No wait)(nested AFS file)
 * [Inputs  ] set_ptid   :Partition ID for loading and setting
 *			  rd_ptid 	 :Partition ID reading AFS file
 *            rd_flid	 :File ID reading AFS file
 *            ptinfo :Area of partition information
 * [Outputs ] None
 * [Return  ] Error code
 * [Function] Loads AFS file in AFS file and sets partition ID.
 */
Sint32 ADXF_LoadPartitionFromAfsNw(Sint32 set_ptid, Sint32 rd_ptid, Sint32 rd_flid, void* ptinfo);

/*  即時復帰型パーティションのロード(読み込みバッファを指定する)
 * [書　式] Sint32 ADXF_LoadPtNwEx(Sint32 ptid, Char8 *fname, void *dir, void *ptinfo,
 *									void *tmpbuf, Sint32 tbsize);
 * [入　力] ptid	:パーティションID
 *			fname	:AFSファイルの名前
 *			dir		:ディレクトリ情報 (GDFS:GDFS_DIRREC)
 *			ptinfo	:パーティション情報読み込み領域
 *			tmpbuf	:AFSファイルヘッダ読み込みバッファ
 *			tbsize	:AFSファイルヘッダ読み込みバッファサイズ
 * [出　力] なし
 * [関数値] エラーコード
 * [機　能] AFSファイルを読み込んで、パーティションIDに設定する。
 *  Load of the partition(No wait)(specify reading buffer)
 * [Inputs  ] ptid		:Partition ID
 *			  fname		:Name of AFS file
 *            dir	 	:Directory information (GDFS:GDFS_DIRREC)
 *            ptinfo	:Area of partition information
 *			  tmpbuf	:Buffer reading AFS file
 *			  tbsize	:Size of buffer reading AFS file
 * [Outputs ] None
 * [Return  ] Error code
 * [Function] Loads AFS file and sets partition ID.
 */
Sint32 ADXF_LoadPtNwEx(Sint32 ptid, Char8* fname, void* dir, void* ptinfo, void* tmpbuf, Sint32 tbsize);

/*  即時復帰型パーティションのロード(入れ子AFSファイル)(読み込みバッファを指定する場合)
 * [書　式] Sint32 ADXF_LoadPtFromPtNwEx(Sint32 set_ptid, Sint32 rd_ptid, Sint32 rd_flid,
 *										void *ptinfo, void *tmpbuf, Sint32
 *tbsize); [入　力] set_ptid	:ロードするAFSファイルに対して設定するパーティションID rd_ptid
 *:読み込むAFSファイルのパーティションID rd_flid		:読み込むAFSファイルのファイルID ptinfo
 *:パーティション情報読み込み領域 tmpbuf		:AFSファイルヘッダ読み込みバッファ tbsize
 *:AFSファイルヘッダ読み込みバッファサイズ [出　力] なし [関数値] エラーコード [機　能]
 *AFSファイル名を読み込んで、パーティションIDに設定する。 Load of the partition(No wait)(nested AFS file)(specify
 *reading buffer) [Inputs  ] set_ptid	:Partition ID for loading and setting rd_ptid	:Partition ID reading AFS file
 *            rd_flid	:File ID reading AFS file
 *            ptinfo	:Area of partition information
 *			  tmpbuf	:Buffer reading AFS file
 *			  tbsize	:Size of buffer reading AFS file
 * [Outputs ] None
 * [Return  ] Error code
 * [Function] Loads AFS file and sets partition ID.
 */
Sint32 ADXF_LoadPtFromAfsNwEx(Sint32 set_ptid, Sint32 rd_ptid, Sint32 rd_flid, void* ptinfo, void* tmpbuf,
                              Sint32 tbsize);

/*  パーティション情報の読み込み状態の取得
 * [書　式] Sint32 ADXF_GetPtStat(Sint32 ptid);
 * [入　力] ptid	:パーティションID
 * [出　力] なし
 * [関数値] パーティション情報の読み込み状態
 * [機　能] AFSファイル名を読み込んで、パーティションIDに設定する。
 *  Get partition loading state
 * [Inputs  ] ptid   :Partition ID
 * [Outputs ] None
 * [Return  ] Partition loading state
 * [Function] Gets partition loading state.
 */
Sint32 ADXF_GetPtStat(Sint32 ptid);
Sint32 ADXF_GetPtStatEx(Sint32 ptid);

/*  パーティション情報読み込みの中止
 * [書　式] void ADXF_StopPtLd(void);
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] パーティション情報の読み込みを中止する。
 *  Stop to load partition information
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] Stops to load partition information.
 */
void ADXF_StopPtLd(void);

/*  パーティション情報サイズの取得
 * [書　式] Sint32 ADXF_GetPtinfoSize(Sint32 ptid);
 * [入　力] ptid	:パーティションID
 * [出　力] なし
 * [関数値] パーティション情報サイズ(単位：バイト)
 * [機　能] パーティション情報領域のサイズを取得する。
 *  Get size of partition information
 * [Inputs  ] ptid   :Partition ID
 * [Outputs ] None
 * [Return  ] Size of the partition information(byte)
 * [Function] Gets size of the partition information.
 */
Sint32 ADXF_GetPtinfoSize(Sint32 ptid);

/*==========================================================================
                ファイルの読み込み
                Read file
  ==========================================================================*/

/*	ファイルのオープン(ISO9660のファイル)
 * [書　式] ADXF ADXF_Open(Sint8 *fname, void *atr);
 * [入　力] fname	: ファイル名
 *			atr		: ファイルの属性
 * [出　力] なし
 * [関数値] ADXFハンドル、エラーの場合NULL
 * [機　能] 指定のファイルをオープンし、ADXFハンドルを返す。
 *  Open file(ISO9660 format)
 * [Inputs  ] fname  : File name
 *			  atr    : File attribute
 * [Outputs ] None
 * [Return  ] ADXF handle
 *			  (If the operation was unsuccessful, this function returns NULL.)
 * [Function] Opens a file of ISO9660 format.
 */
ADXF ADXF_Open(Char8* fname, void* atr);

/*	ファイルのオープン(AFSフォーマット)
 * [書　式] ADXF ADXF_OpenAfs(Sint32 ptid, Sint32 flid);
 * [入　力] ptid	: パーティションID
 *			flid	: ファイルID
 * [出　力] なし
 * [関数値] ADXFハンドル、エラーの場合NULL
 * [機　能] パーティションIDとファイルIDで指定されたAFSファイルをオープンし、
 *			ADXFハンドルを返す。
 *  Open file(AFS format)
 * [Inputs  ] ptid   : Partition ID
 *			  flid   : File ID
 * [Outputs ] None
 * [Return  ] ADXF handle
 *			  (If the operation was unsuccessful, this function returns NULL.)
 * [Function] Opens a file of AFS format.
 */
ADXF ADXF_OpenAfs(Sint32 ptid, Sint32 flid);

/* ファイルのクローズ
 * [書　式] void ADXF_Close(ADXF adxf);
 * [入　力] adxf	: ADXFハンドル
 * [出　力] なし
 * [関数値] なし
 * [機　能] 指定されたADXFハンドルのファイルをクローズする。
 *  Close file
 * [Inputs  ] adxf   : ADXF handle
 * [Outputs ] None
 * [Return  ] None
 * [Function] Closes a file of specific ADXF handle.
 */
void ADXF_Close(ADXF adxf);

/* 全てのファイルクローズ
 * [書　式] void ADXF_CloseAll(void);
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] ADXFハンドルのファイルを全てクローズする。
 *  Close file
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] Closes all file of ADXF handle.
 */
void ADXF_CloseAll(void);

/*  SJへのデータ読み込み開始
 * [書　式] void ADXF_ReadSj(ADXF adxf, Sint32 nsct, SJ sj);
 * [入　力] adxf	: ADXFハンドル
 *			nsct	: 読み込みデータの量(単位：セクタ)
 *			SJ		: ストリームジョイント
 * [出　力] なし
 * [関数値] 読み込みデータ量(単位：セクタ)
 * [機　能] ストリームジョイントにデータ読み込みのリクエストを発行する。
 *			ストリームジョイントのバッファサイズは、セクタの整数倍でなければ
 *			ならない。
 *			ストリームジョイントからユーザがデータを読み出すと、自動的にストリー
 *			ムジョイントにデータが読み込まれる。
 *			リクエストしたアクセス動作が完了すると、アクセスポインタは
 *			nsctセクタ進む。
 *  Read-in start to Stream Joint
 * [Inputs  ] adxf   : ADXF handle
 *			  nsct   : Number of read-in sectors to request
 *			  SJ     : Stream Joint
 * [Outputs ] None
 * [Return  ] Number of sectors to read
 * [Function] Requests data read-in to Stream Joint.
 *			  Buffer size of Stream Joint must be multiples of integer.
 *			  If you read data from Stream Joint, data are read into Stream
 *			  Joint automatically.
 *			  The access pointer moves 'nsct' sector minute when the request
 *			  was completed.
 */
Sint32 ADXF_ReadSj(ADXF adxf, Sint32 nsct, SJ sj);

/* データの読み込み開始
 * [書　式] void ADXF_ReadNw(ADXF adxf, Sint32 nsct, void *buf);
 * [入　力] adxf	: ADXFハンドル
 *			nsct	: 読み込みデータの量(単位：セクタ)
 *			buf		: 読み込み領域
 * [出　力] なし
 * [関数値] 読み込みデータ量(単位：セクタ)
 * [機　能] データ読み込みのリクエストを発行する。
 *			リクエストしたアクセス動作が完了すると、アクセスポインタは
 *			nsctセクタ進む。
 *  Read-in start
 * [Inputs  ] adxf   : ADXF handle
 *			  nsct   : Number of read-in sectors to request
 *			  buf    : Pointer to buffer into which the data is read
 * [Outputs ] None
 * [Return  ] Number of sectors to read
 * [Function] Requests data read-in to Stream Joint.
 *			  The access pointer moves 'nsct' sector minute when the request
 *			  was completed.
 */
Sint32 ADXF_ReadNw(ADXF adxf, Sint32 nsct, void* buf);

/* データの読み込み中止
 * [書　式] Sint32 ADXF_Stop(ADXF adxf);
 * [入　力] adxf	: ADXFハンドル
 * [出　力] なし
 * [関数値] 中止した時点でのアクセスポインタの位置
 * [機　能] データの読み込みを中止する。
 *  Read-in stop
 * [Inputs  ] adxf   : ADXF handle
 * [Outputs ] None
 * [Return  ] The location of access pointer when you stop to read
 * [Function] Stops to read data.
 */
Sint32 ADXF_Stop(ADXF adxf);

/* データの読み込み中止 (即時復帰)
 * [書　式] Sint32 ADXF_Stop(ADXF adxf);
 * [入　力] adxf	: ADXFハンドル
 * [出　力] なし
 * [関数値] 中止をリクエスト時点でのアクセスポインタの位置
 * [機　能] データの読み込みを中止をリクエストする。
 *			状態はすぐには変らず、読み込みを停止したときにSTOPに変化する
 *  Read-in stop
 * [Inputs  ] adxf   : ADXF handle
 * [Outputs ] None
 * [Return  ] The location of access pointer when you request to stop
 * [Function] Request to Stop.
 */
Sint32 ADXF_StopNw(ADXF adxf);

/* サーバ関数
 * [書　式] void ADXF_ExecServer(void);
 * [入　力] なし
 * [出　力] なし
 * [関数値] なし
 * [機　能] 内部状態を更新する。
 *  Server function
 * [Inputs  ] None
 * [Outputs ] None
 * [Return  ] None
 * [Function] Update the inside status of library.
 */
void ADXF_ExecServer(void);

/*==========================================================================
                アクセスポインタの制御
                Control access pointer
  ==========================================================================*/

/*  アクセスポインタの移動
 * [書　式] Sint32 ADXF_Seek(ADXF adxf, Sint32 pos, Sint32 type);
 * [入　力] adxf	: ADXFハンドル
 *			pos		: アクセスポインタの移動量(単位：セクタ)
 *			type	: 移動基準(シークタイプ：ADXF_SEEK_～)
 * [出　力] なし
 * [関数値] アクセスポインタの位置、負の値はエラーコード
 * [機　能] アクセスポインタをtypeからposセクタ離れた位置に移動させる。
 *  Move access pointer
 * [Inputs  ] adxf   : ADXF handle
 *			  pos    : Offset relative to 'type'
 *			  type   : Specified the origin for the offset
 * [Outputs ] None
 * [Return  ] Location containing new seek pointer
 * [Function] Changes the seek pointer to a new location relative to the
 *			  beginning of the file, to the end of the file, or to the current
 *			  seek pointer.
 */
Sint32 ADXF_Seek(ADXF adxf, Sint32 pos, Sint32 type);

/*  アクセスポインタの取得
 * [書　式] Sint32 ADXF_Tell(ADXF adxf);
 * [入　力] adxf	: ADXFハンドル
 * [出　力] なし
 * [関数値] アクセスポインタの位置
 * [機　能] アクセスポインタの位置を取得する。
 *  Get the access pointer
 * [Inputs  ] adxf   : ADXF handle
 * [Outputs ] None
 * [Return  ] Access pointer
 * [Function] Gets the access pointer.
 */
Sint32 ADXF_Tell(ADXF adxf);

/*==========================================================================
                情報の取得
                Get information
  ==========================================================================*/

/*  ファイルサイズの取得
 * [書　式] Sint32 ADXF_GetFsizeSct(ADXF adxf);
 * [入　力] adxf	: ADXFハンドル
 * [出　力] なし
 * [関数値] ファイルサイズ(単位：セクタ)
 * [機　能] 指定されたファイルのサイズをセクタ単位で取得する。
 *  Get file size
 * [Inputs  ] adxf   : ADXF handle
 * [Outputs ] None
 * [Return  ] File size(sector)
 * [Function] Obtains the size of the specified file.
 */
Sint32 ADXF_GetFsizeSct(ADXF adxf);

/*  ファイルサイズの取得
 * [書　式] Sint32 ADXF_GetFsizeByte(ADXF adxf);
 * [入　力] adxf	: ADXFハンドル
 * [出　力] なし
 * [関数値] ファイルサイズ(単位：バイト)
 * [機　能] 指定されたファイルのサイズをバイト単位で取得する。
 *  Get file size
 * [Inputs  ] adxf   : ADXF handle
 * [Outputs ] None
 * [Return  ] File size(byte)
 * [Function] Obtains the size of the specified file.
 */
Sint32 ADXF_GetFsizeByte(ADXF adxf);

/*  読み込み要求情報の取得
 * [書　式] Sint32 ADXF_GetNumReqSct(ADXF adxf, Sint32 *seekpos);
 * [入　力] adxf	: ADXFハンドル
 * [出　力] seekpos	: 読み込み位置
 * [関数値] 要求した読み込みデータ量(単位：セクタ)
 * [機　能] 要求した読み込み位置とデータ量を取得する。
 *  Get request information
 * [Inputs  ] adxf   : ADXF handle
 * [Outputs ] seekpos: location of start to read
 * [Return  ] Request data size(sector)
 * [Function] Obtains read-in start position and the read-in size that
 *			  demanded.
 */
Sint32 ADXF_GetNumReqSct(ADXF adxf, Sint32* seekpos);

/*  実際に読み込んだセクタ数の取得
 * [書　式] Sint32 ADXF_GetNumReadSct(ADXF adxf);
 * [入　力] adxf	: ADXFハンドル
 * [出　力] なし
 * [関数値] 読み込んだデータ量(単位：セクタ)
 * [機　能] 実際に読み込んだデータ量を取得する。
 *  Get the number of sectors read
 * [Inputs  ] adxf   : ADXF handle
 * [Outputs ] None
 * [Return  ] Size of read data(sector)
 * [Function] Obtains the size of data that read actually.
 */
Sint32 ADXF_GetNumReadSct(ADXF adxf);

/*  ハンドルの状態の取得
 * [書　式] Sint32 ADXF_GetStat(ADXF adxf);
 * [入　力] adxf	: ADXFハンドル
 * [出　力] なし
 * [関数値] ADXFハンドルの内部状態(状態：ADXF_STAT_～)
 * [機　能] ADXFハンドルの内部状態を取得する。
 *  Get status of handle
 * [Inputs  ] adxf   : ADXF handle
 * [Outputs ] None
 * [Return  ] Status of handle
 * [Function] Gets status of handle.
 */
Sint32 ADXF_GetStat(ADXF adxf);

Sint32 ADXF_GetFnameRange(Sint32 ptid, Sint32 flid, Char8* fname, Sint32* ofst, Sint32* fnsct);

Sint32 ADXF_GetFnameRangeEx(Sint32 ptid, Sint32 flid, Char8* fname, void** dir, Sint32* ofst, Sint32* fnsct);

/*　リードリクエストのサイズの設定
 * [書　式] void ADXF_SetReqRdSct(ADXF adxf, Sint32 nsct);
 * [入　力] adxf    ; ADXFハンドル
 *			nsct    ; セクタ数
 * [出　力] なし
 * [関数値] なし
 * [機　能] 一回に読み込むサイズを設定します。
 *  Set size of read request
 * [Inputs  ] adxf   : ADXF handle
 *			  nsct   : size(sector)
 * [Outputs ] None
 * [Return  ] None
 * [Function] Sets size of read request(one time).
 */
void ADXF_SetReqRdSct(ADXF adxf, Sint32 nsct);

/*	ファイル読み込みステータスの取得
 * [書　式] Sint32 ADXT_GetStatRead(ADXT adxt);
 * [入　力] adxt	: ADXTハンドル
 * [出　力] なし
 * [関数値] 1=読み込み中、0=読み込み停止中
 * [機　能] 読み込み状態を取得する
 *  Get status of data reading
 * [Inputs  ] adxf   : ADXF handle
 * [Outputs ] None
 * [Return  ] 1=Data reading in progress,  0=Data reading stopped
 * [Function] Gets status of data reading
 */
Sint32 ADXF_GetStatRead(ADXF adxf);

/*==========================================================================
                ROFS用
                for ROFS
  ==========================================================================*/

/* 最大文字数 */
#define ADXF_ROFS_VOLNAME_LEN (8)                         /* ボリューム名の文字列長 */
#define ADXF_ROFS_VOLNAME_SIZ (ADXF_ROFS_VOLNAME_LEN + 1) /* 文字列サイズ */
#define ADXF_ROFS_FNAME_LEN (31)                          /* ファイル名の文字列長 */
#define ADXF_ROFS_FNAME_SIZ (ADXF_ROFS_FNAME_LEN + 1)     /* 文字列サイズ */
#define ADXF_ROFS_DIRNAME_LEN (31)                        /* ディレクトリ名の文字列長 */
#define ADXF_RPFS_DIRNAME_SIZ (ADXF_ROFS_DIRNAME_LEN + 1) /* 文字列サイズ */

/* ライブラリ作業領域サイズ */
#define ADXF_CALC_LIBWORK_SIZ(max_open, max_volume, max_dirent)                                                        \
    (sizeof(ADXF_ROFS_WK) + ((max_open) + 1) * sizeof(ADXF_ROFS_OBJ) +                                                 \
     (ADXF_CALC_DIRREC_SIZ(max_dirent) + sizeof(ADXF_ROFS_VOL)) * ((max_volume) + 1))

/* ディレクトリレコード領域サイズ */
#define ADXF_CALC_DIRREC_SIZ(n_dirent)                                                                                 \
    (sizeof(ADXF_ROFS_DIRREC) - sizeof(ADXF_ROFS_DIRRECENT) + (n_dirent) * sizeof(ADXF_ROFS_DIRRECENT))

/* ワーク領域 */
typedef struct ADXF_ROFS_STWK ADXF_ROFS_WK;

/* プリミティブ関数 */
typedef struct ADXF_ROFS_PFSIFTBL ADXF_ROFS_PFSTBL;

/* デバイスコントロール関数 */
typedef struct ADXF_ROFS_DEVIFTBLE ADXF_ROFS_DEVTBL;

/* ライブラリインターフェース */
typedef struct ADXF_ROFS_LIBIFTBLE ADXF_ROFS_LIFTBL;

/* ディレクトリレコードエントリ */
typedef struct {
    Sint32 fad;                       /* fad */
    Sint32 fsize;                     /* file size */
    Uint8 flag;                       /* file attribute */
    Uint8 sid;                        /* filename search id */
    Char8 fname[ADXF_ROFS_FNAME_SIZ]; /* filename */
    Uint8 pad[6];
} ADXF_ROFS_DIRRECENT; /* 48 bytes */

/* ディレクトリレコード */
typedef struct {
    Sint32 dir_num;                     /* number of record */
    Sint32 max_ent;                     /* max entry of directory record */
    Sint32 dir_fad;                     /* fad of directory record */
    Char8 volname[ADXF_ROFS_FNAME_SIZ]; /* volume name */
    Sint32 pad;                         /* padding */
    ADXF_ROFS_DIRRECENT dirrec_tbl[1];  /* record table */
} ADXF_ROFS_DIRREC;
typedef ADXF_ROFS_DIRREC* ADXF_ROFS_DIRRECBUF;

/* ボリューム管理構造体 */
typedef struct {
    Char8 volname[ADXF_ROFS_VOLNAME_SIZ]; /* ボリューム名 */
    void* img_hn;                         /* イメージファイルのファイルハンドル */
    Sint32 zisb;                          /* ZONE0イメージデータ開始位置 */
    Sint32 ptbl_cba;                      /* cba of path table */
    Sint32 ptbl_size;                     /* size of path table */
    ADXF_ROFS_DIRRECBUF curdir;           /* current directory handle */
} ADXF_ROFS_VOL;

/* ROFS File Handle */
typedef struct {
    ADXF_ROFS_WK* wk;   /* pointer of lib work */
    Sint32 fid;         /* file id */
    Sint32 fad;         /* fad */
    Sint32 ofs;         /* offset */
    Sint32 fsize;       /* file size */
    Sint32 fsctsize;    /* sctor size of the file */
    ADXF_ROFS_VOL* vol; /* image file volume */
    Sint32 rsize;       /* reading size */
    Sint32 trns_seg;    /* 転送済ブロック単位 */
    Sint32 trns_ofs;    /* 転送済バイト単位 */
    Sint32 trns_unit;   /* 転送単位(ブロック) */
    Uint8* rdadr;       /* 読み込みアドレス */
    Sint16 used;        /* used flag */
    Sint16 act;         /* handle act */
    Sint16 stat;        /* handle status(ADXF_Stat) */
    Sint16 err;         /* error status */
} ADXF_ROFS_OBJ;
typedef ADXF_ROFS_OBJ* ADXF_ROFS;

/* ROFS Work Area */
struct ADXF_ROFS_STWK {
    Bool f_init;     /* 初期化フラグ */
    Sint32 max_open; /* max open files */
    Sint32 max_volume;
    Sint32 max_dirent;
    Uint32 exec_server_cnt;                          /* counter */
    ADXF_ROFS syshdl;                                /* handle for system command */
    ADXF_ROFS hndtbl;                                /* handle */
    ADXF_ROFS_VOL* vollist;                          /* ボリューム情報リスト */
    ADXF_ROFS_VOL* curvol;                           /* カレントボリューム情報 */
    ADXF_ROFS_DIRRECBUF dirbuf;                      /* ディレクトリレコードの開始位置 */
    ADXF_ROFS_PFSTBL* pfs;                           /* プリミティブ関数 */
    ADXF_ROFS_DEVTBL* dev;                           /* デバイスコントロール関数 */
    ADXF_ROFS_LIFTBL* liftbl;                        /* ライブラリインターフェース */
    void (*g_errcb)(void*, Char8*, Sint32);          /* error callback */
    void* g_errcb_1st;                               /* error callback 1st arg. */
    Uint32 sctbuf[(ADXF_DEF_SCT_SIZE * 2 + 64) / 4]; /* sector buffer */
    ADXF_ROFS_OBJ hndlist[1];                        /* handle table */
}; /* 64 + 4160 + handles */

/* ROFSセットアップ構造体  */
#ifndef ADXF_SPRM_ROFS_DEFINED
#define ADXF_SPRM_ROFS_DEFINED
typedef struct {
    Sint32 max_open;   /* 同時にオープンできる最大ファイル数 */
    Sint32 max_volume; /* 同時に登録できる最大ボリューム数 */
    Sint32 max_dirent; /* ディレクトリに格納する最大ファイル数 */
    void* rofs_work;   /* ライブラリ作業領域の先頭アドレス */
} ADXF_SPRM_ROFS;
#endif

/* ROFSのセットアップ	*/
/* Setup ROFS			*/
void ADXF_SetupRofs(void* sprmr);

/* ROFSのシャットダウン */
/* Shutdown ROFS		*/
void ADXF_ShutdownRofs(void);

/* ROFSボリュームの追加		*/
/* Addition of ROFS volume  */
Sint32 ADXF_AddRofsVol(Char8* volname, Char8* imgname);

/* ROFSボリュームの削除 */
/* Delete ROFS volume	*/
void ADXF_DelRofsVol(Char8* volname);

/* ROFSボリューム名であるか否か */
/* Check ROFS volume name		*/
Bool ADXF_IsRofsVol(Char8* volname);

/* デフォルトデバイスの設定 */
/* Setting default device	*/
void ADXF_SetDefDev(Char8* volname);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CRI_ADXF_H_INCLUDED */
