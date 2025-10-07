#ifndef _EEREGS_H_
#define _EEREGS_H_

#include "types.h"

typedef struct {
    unsigned DIR : 1; /* Direction */
    unsigned p0 : 1;
    unsigned MOD : 2; /* Mode */
    unsigned ASP : 2; /* Address stack pointer */
    unsigned TTE : 1; /* Tag trasfer enable */
    unsigned TIE : 1; /* Tag interrupt enable */
    unsigned STR : 1; /* start */
    unsigned p1 : 7;
    unsigned TAG : 16; /* DMAtag */
} tD_CHCR;

typedef struct {
    unsigned EN1 : 1;   /* */
    unsigned EN2 : 1;   /* */
    unsigned CRTMD : 3; /* CRT mode */
    unsigned MMOD : 1;  /* */
    unsigned AMOD : 1;  /* */
    unsigned SLBG : 1;  /* */
    unsigned ALP : 8;   /* */
    unsigned p0 : 16;   /* */
    unsigned int p1;
} tGS_PMODE;

typedef struct {
    unsigned INT : 1;  /* Interlace mode */
    unsigned FFMD : 1; /* */
    unsigned DPMS : 2; /* VESA DPMS mode */
    unsigned p0 : 28;  /* */
    unsigned int p1;
} tGS_SMODE2;

typedef struct {
    unsigned FBP : 9; /* Base pointer */
    unsigned FBW : 6; /* Buffer width */
    unsigned PSM : 5; /* Pixel store mode */
    unsigned p0 : 12;
    unsigned DBX : 11; /* */
    unsigned DBY : 11; /* */
    unsigned p1 : 10;
} tGS_DISPFB2;

typedef struct {
    unsigned DX : 12;  /* */
    unsigned DY : 11;  /* */
    unsigned MAGH : 4; /* */
    unsigned MAGV : 2; /* */
    unsigned p0 : 3;
    unsigned DW : 12; /* */
    unsigned DH : 11; /* */
    unsigned p1 : 9;
} tGS_DISPLAY2;

typedef struct {
    unsigned R : 8; /* Background color Blue */
    unsigned G : 8; /* Background color Green */
    unsigned B : 8; /* Background color Red */
    unsigned p0 : 8;
    unsigned int p1;
} tGS_BGCOLOR;

/* TIMER */
#define T0_COUNT ((volatile u_int*)(0x10000000))
#define T0_MODE ((volatile u_int*)(0x10000010))
#define T0_COMP ((volatile u_int*)(0x10000020))
#define T0_HOLD ((volatile u_int*)(0x10000030))
#define T1_COUNT ((volatile u_int*)(0x10000800))
#define T1_MODE ((volatile u_int*)(0x10000810))
#define T1_COMP ((volatile u_int*)(0x10000820))
#define T1_HOLD ((volatile u_int*)(0x10000830))
#define T2_COUNT ((volatile u_int*)(0x10001000))
#define T2_MODE ((volatile u_int*)(0x10001010))
#define T2_COMP ((volatile u_int*)(0x10001020))
#define T3_COUNT ((volatile u_int*)(0x10001800))
#define T3_MODE ((volatile u_int*)(0x10001810))
#define T3_COMP ((volatile u_int*)(0x10001820))

/* GS Special */
#define GS_PMODE ((volatile u_long*)(0x12000000))
#define GS_SMODE2 ((volatile u_long*)(0x12000020))
#define GS_DISPFB1 ((volatile u_long*)(0x12000070))
#define GS_DISPLAY1 ((volatile u_long*)(0x12000080))
#define GS_DISPFB2 ((volatile u_long*)(0x12000090))
#define GS_DISPLAY2 ((volatile u_long*)(0x120000a0))
#define GS_EXTBUF ((volatile u_long*)(0x120000b0))
#define GS_EXTDATA ((volatile u_long*)(0x120000c0))
#define GS_EXTWRITE ((volatile u_long*)(0x120000d0))
#define GS_BGCOLOR ((volatile u_long*)(0x120000e0))
#define GS_CSR ((volatile u_long*)(0x12001000))
#define GS_IMR ((volatile u_long*)(0x12001010))
#define GS_BUSDIR ((volatile u_long*)(0x12001040))
#define GS_SIGLBLID ((volatile u_long*)(0x12001080))

/* VIF1 */
#define VIF1_STAT ((volatile u_int*)(0x10003c00))
#define VIF1_FIFO ((volatile __int128*)(0x10005000))

/*
 * register access macro
 */

/* TIMER */

#define DGET_T0_MODE() (0)
#define DPUT_T0_MODE(x)
#define DGET_T0_COUNT() (0)
#define DPUT_T0_COUNT(x)
#define DGET_T0_COMP() (0)
#define DPUT_T0_COMP(x)
#define DGET_T0_HOLD() (0)
#define DPUT_T0_HOLD(x)
#define DGET_T1_MODE() (0)
#define DPUT_T1_MODE(x)
#define DGET_T1_COUNT() (0)
#define DPUT_T1_COUNT(x)
#define DGET_T1_COMP() (0)
#define DPUT_T1_COMP(x)
#define DGET_T1_HOLD() (0)
#define DPUT_T1_HOLD(x)
#define DGET_T2_MODE() (0)
#define DPUT_T2_MODE(x)
#define DGET_T2_COUNT() (0)
#define DPUT_T2_COUNT(x)
#define DGET_T2_COMP() (0)
#define DPUT_T2_COMP(x)
#define DGET_T3_MODE() (0)
#define DPUT_T3_MODE(x)
#define DGET_T3_COUNT() (0)
#define DPUT_T3_COUNT(x)
#define DGET_T3_COMP() (0)
#define DPUT_T3_COMP(x)

/* GS Special */
#define DGET_GS_CSR() (*GS_CSR)
#define DGET_GS_SIGLBLID() (*GS_SIGLBLID)

#define DPUT_GS_PMODE(x)
#define DPUT_GS_SMODE2(x)
#define DPUT_GS_DISPFB1(x)
#define DPUT_GS_DISPLAY1(x)
#define DPUT_GS_DISPFB2(x)
#define DPUT_GS_DISPLAY2(x)
#define DPUT_GS_EXTBUF(x)
#define DPUT_GS_EXTDATA(x)
#define DPUT_GS_EXTWRITE(x)
#define DPUT_GS_BGCOLOR(x)
#define DPUT_GS_CSR(x)
#define DPUT_GS_IMR(x)
#define DPUT_GS_BUSDIR(x)

#endif
