/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 1998-2002 CRI-MW
 *
 * Library  : CRI Middleware Library
 * Module   : CRI Common Header
 * File     : cri_xpts.h
 * Date     : 2002-09-18
 * Version  : 1.10
 *
 ****************************************************************************/
#ifndef CRI_XPTS_H
#define CRI_XPTS_H

#ifndef SEGA_XPT_H

#include <inttypes.h>

/*****************************************************************************
 * 定数マクロ
 *****************************************************************************/

/* NULL ポインタ型 */
#ifndef NULL
#ifdef __cplusplus
#define NULL	(0)
#else
#define NULL	((void *)0)
#endif
#endif

/* 論理定数１（偽、真） */
#ifndef FALSE
#define FALSE	(0)
#endif
#ifndef TRUE
#define TRUE	(1)
#endif

/* 論理定数２（スイッチ） */
#ifndef OFF
#define OFF		(0)
#endif
#ifndef ON
#define ON		(1)
#endif
                                                                                                        
/* 結果判定の定数（成功、失敗） */
#ifndef OK
#define OK		(0)			/* 成功 */
#endif
#ifndef NG
#define NG		(-1)		/* 失敗 */
#endif

/*****************************************************************************
 * 基本データ型宣言
 *****************************************************************************/

/*****************************************************************************
 * Uint8 Sint8 Uint16 Sint16 に関しては共通
 *****************************************************************************/

#ifndef _TYPEDEF_Uint8
#define _TYPEDEF_Uint8
typedef unsigned char			Uint8;		/* 符号なし１バイト整数 */
#endif

#ifndef _TYPEDEF_Sint8
#define _TYPEDEF_Sint8
typedef /* signed */ char				Sint8;		/* 符号つき１バイト整数 */
#endif

#ifndef _TYPEDEF_Uint16
#define _TYPEDEF_Uint16
typedef unsigned short			Uint16;		/* 符号なし２バイト整数 */
#endif

#ifndef _TYPEDEF_Sint16
#define _TYPEDEF_Sint16
typedef signed short			Sint16;		/* 符号つき２バイト整数 */
#endif

/*****************************************************************************
 * Uint32 Sint32 Uint64 Sint64 Uint128 Sint128 に関しては個々に設定
 *****************************************************************************/

#ifdef XPT_TGT_EE

#ifndef _TYPEDEF_Uint32
#define _TYPEDEF_Uint32
typedef unsigned int			Uint32;		/* 符号なし４バイト整数 */
#endif

#ifndef _TYPEDEF_Sint32
#define _TYPEDEF_Sint32
typedef signed int				Sint32;		/* 符号つき４バイト整数 */
#endif

#ifndef _TYPEDEF_Uint64
#define _TYPEDEF_Uint64
typedef uint64_t			Uint64;		/* 符号なし８バイト整数 */
#endif

#ifndef _TYPEDEF_Sint64
#define _TYPEDEF_Sint64
typedef int64_t				Sint64;		/* 符号つき８バイト整数 */
#endif

// #ifndef _TYPEDEF_Uint128
// #define _TYPEDEF_Uint128					/* 符号なし16バイト整数 */
// typedef unsigned int			Uint128 __attribute__ ((mode (TI)));
// #endif

// #ifndef _TYPEDEF_Sint128
// #define _TYPEDEF_Sint128					/* 符号つき16バイト整数 */
// typedef int						Sint128 __attribute__ ((mode (TI)));
// #endif

#endif	/* endif XPT_TGT_EE */

#ifdef XPT_TGT_GC

#ifndef _TYPEDEF_Uint64
#define _TYPEDEF_Uint64
typedef unsigned long long		Uint64;		/* 符号なし８バイト整数 */
#endif

#ifndef _TYPEDEF_Sint64
#define _TYPEDEF_Sint64
typedef signed long long		Sint64;		/* 符号つき８バイト整数 */
#endif

#ifndef _TYPEDEF_Uint128
#define _TYPEDEF_Uint128
typedef struct {							/* 符号なし16バイト整数 */
	Uint64	h;								/* 上位64ビット */
	Uint64	l;								/* 下位64ビット */
} Uint128;
#endif

#ifndef _TYPEDEF_Sint128
#define _TYPEDEF_Sint128
typedef struct {							/* 符号つき16バイト整数 */
	Uint64	h;								/* 上位64ビット */
	Uint64	l;								/* 下位64ビット */
} Sint128;
#endif

#endif	/* endif XPT_TGT_GC */

#if defined(XPT_TGT_XB) || defined(XPT_TGT_PC)

#ifndef _TYPEDEF_Uint64
#define _TYPEDEF_Uint64
typedef unsigned __int64		Uint64;		/* 符号なし８バイト整数 */
#endif

#ifndef _TYPEDEF_Sint64
#define _TYPEDEF_Sint64
typedef signed __int64			Sint64;		/* 符号つき８バイト整数 */
#endif

#ifndef _TYPEDEF_Uint128
#define _TYPEDEF_Uint128
typedef struct {							/* 符号なし16バイト整数 */
	Uint64	h;								/* 上位64ビット */
	Uint64	l;								/* 下位64ビット */
} Uint128;
#endif

#ifndef _TYPEDEF_Sint128
#define _TYPEDEF_Sint128
typedef struct {							/* 符号つき16バイト整数 */
	Sint64	h;								/* 上位64ビット */
	Uint64	l;								/* 下位64ビット */
} Sint128;
#endif

#endif	/* endif XPT_TGT_XB, XPT_TGT_PC */

#ifdef XPT_TGT_IOP

#ifndef _TYPEDEF_Uint32
#define _TYPEDEF_Uint32
typedef unsigned int			Uint32;		/* 符号なし４バイト整数 */
#endif

#ifndef _TYPEDEF_Sint32
#define _TYPEDEF_Sint32
typedef signed int				Sint32;		/* 符号つき４バイト整数 */
#endif

#endif	/* endif XPT_TGT_IOP */

#if	1	/* 他すべてのターゲット */

#ifndef _TYPEDEF_Uint32
#define _TYPEDEF_Uint32
typedef unsigned int			Uint32;		/* 符号なし４バイト整数 */
#endif

#ifndef _TYPEDEF_Sint32
#define _TYPEDEF_Sint32
typedef signed int				Sint32;		/* 符号つき４バイト整数 */
#endif

#endif /* endif 他すべてのターゲット */

/*****************************************************************************
 * Float16 Float32 に関してはほぼ共通（PowerPlant使用時のみ例外）
 *****************************************************************************/

#ifndef _TYPEDEF_Float16
#define _TYPEDEF_Float16
typedef signed short			Float16;	/* ２バイト実数 */
#endif

/* Mac環境でPowerPlant使用時はFloat32とFloat64がMacTypes.hで定義済み */
#ifndef __MACTYPES__

#ifndef _TYPEDEF_Float32
#define _TYPEDEF_Float32
typedef float					Float32;	/* ４バイト実数 */
#endif

#ifndef _TYPEDEF_Float64
#define _TYPEDEF_Float64
typedef double					Float64;	/* ８バイト実数 */
#endif

#endif	/* endif __MACTYPES__ */

/*****************************************************************************
 * Fixed32 Bool Char8 に関しては共通
 *****************************************************************************/

#ifndef _TYPEDEF_Fixed32
#define _TYPEDEF_Fixed32
typedef signed long				Fixed32;	/* 固定小数点32ビット */
#endif

#ifndef _TYPEDEF_Bool
#define _TYPEDEF_Bool
typedef Sint32					Bool;		/* 論理型（論理定数を値にとる） */
#endif

#ifndef _TYPEDEF_Char8
#define _TYPEDEF_Char8
typedef char					Char8;		/* 文字型 */
#endif


#endif
#endif	/* CRI_XPTS_H */

/* end of file */
