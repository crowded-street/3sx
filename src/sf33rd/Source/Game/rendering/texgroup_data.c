/**
 * @file texgroup_data.c
 * Texture group file and offset data.
 */

#include "sf33rd/Source/Game/rendering/texgroup_data.h"

#define TexGroupData_Empty ((TexGroupData) { 0, -1, 0, 0, 0 })

const TexGroupData texgrpdat[100] = {
    [0] = TexGroupData_Empty,
    [1] = { .num_of_1st = 0,
            .apfn = 1460,
            .mode = TEXGROUP_MODE_CHARACTER,
            .to_tex = 0x33784,
            .to_chd = 0x2CF158 }, // pl00.bin
    [2] = { .num_of_1st = 1568,
            .apfn = 1465,
            .mode = TEXGROUP_MODE_CHARACTER,
            .to_tex = 0x1C6D0,
            .to_chd = 0x24EDE0 }, // pl01.bin
    [3] = { .num_of_1st = 2592,
            .apfn = 1468,
            .mode = TEXGROUP_MODE_CHARACTER,
            .to_tex = 0x11C7C,
            .to_chd = 0x178F38 }, // pl02.bin
    [4] = { .num_of_1st = 3552,
            .apfn = 1472,
            .mode = TEXGROUP_MODE_CHARACTER,
            .to_tex = 0x1C080,
            .to_chd = 0x1FAA0C }, // pl03.bin
    [5] = { .num_of_1st = 4992,
            .apfn = 1476,
            .mode = TEXGROUP_MODE_CHARACTER,
            .to_tex = 0x1B088,
            .to_chd = 0x1E1D54 }, // pl04.bin
    [6] = { .num_of_1st = 6144,
            .apfn = 1479,
            .mode = TEXGROUP_MODE_CHARACTER,
            .to_tex = 0x1C79C,
            .to_chd = 0x2BCE28 }, // pl05.bin
    [7] = { .num_of_1st = 7392,
            .apfn = 1483,
            .mode = TEXGROUP_MODE_CHARACTER,
            .to_tex = 0x26AC0,
            .to_chd = 0x31392C }, // pl06.bin
    [8] = { .num_of_1st = 8384,
            .apfn = 1487,
            .mode = TEXGROUP_MODE_CHARACTER,
            .to_tex = 0x250C0,
            .to_chd = 0x25D908 }, // pl07.bin
    [9] = { .num_of_1st = 10208,
            .apfn = 1492,
            .mode = TEXGROUP_MODE_CHARACTER,
            .to_tex = 0x22BD4,
            .to_chd = 0x1A0CCC }, // pl08.bin
    [10] = { .num_of_1st = 11776,
             .apfn = 1495,
             .mode = TEXGROUP_MODE_CHARACTER,
             .to_tex = 0x219D0,
             .to_chd = 0x205694 }, // pl09.bin
    [11] = { .num_of_1st = 13280,
             .apfn = 1499,
             .mode = TEXGROUP_MODE_CHARACTER,
             .to_tex = 0x1C89C,
             .to_chd = 0x216E64 }, // pl10.bin
    [12] = { .num_of_1st = 14656,
             .apfn = 1502,
             .mode = TEXGROUP_MODE_CHARACTER,
             .to_tex = 0x118DC,
             .to_chd = 0x17C0F8 }, // pl11.bin
    [13] = { .num_of_1st = 15712,
             .apfn = 1506,
             .mode = TEXGROUP_MODE_CHARACTER,
             .to_tex = 0x13AD4,
             .to_chd = 0x18B378 }, // pl12.bin
    [14] = { .num_of_1st = 16800,
             .apfn = 1510,
             .mode = TEXGROUP_MODE_CHARACTER,
             .to_tex = 0x21104,
             .to_chd = 0x215C68 }, // pl13.bin
    [15] = { .num_of_1st = 18272,
             .apfn = 1514,
             .mode = TEXGROUP_MODE_CHARACTER,
             .to_tex = 0x1C594,
             .to_chd = 0x26DFC8 }, // pl14.bin
    [16] = { .num_of_1st = 19456,
             .apfn = 1518,
             .mode = TEXGROUP_MODE_CHARACTER,
             .to_tex = 0x234C8,
             .to_chd = 0x26C73C }, // pl16.bin
    [17] = { .num_of_1st = 21120,
             .apfn = 1522,
             .mode = TEXGROUP_MODE_CHARACTER,
             .to_tex = 0x2B63C,
             .to_chd = 0x2837C4 }, // pl17.bin
    [18] = { .num_of_1st = 23008,
             .apfn = 1525,
             .mode = TEXGROUP_MODE_CHARACTER,
             .to_tex = 0x363AC,
             .to_chd = 0x37F0B4 }, // pl18.bin
    [19] = { .num_of_1st = 24704,
             .apfn = 1528,
             .mode = TEXGROUP_MODE_CHARACTER,
             .to_tex = 0x20114,
             .to_chd = 0x21B47C }, // pl19.bin
    [20] = { .num_of_1st = 25856,
             .apfn = 1531,
             .mode = TEXGROUP_MODE_CHARACTER,
             .to_tex = 0x1E9EC,
             .to_chd = 0x1A4F50 }, // pl20.bin
    [21] = { .num_of_1st = 27040,
             .apfn = 1452,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x1B4,
             .to_chd = 0x0 }, // ef00.bin
    [22] = TexGroupData_Empty,
    [23] = { .num_of_1st = 27104,
             .apfn = 1454,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x20CA4,
             .to_chd = 0x0 }, // ef02_usa.bin
    [24] = TexGroupData_Empty,
    [25] = { .num_of_1st = 29152,
             .apfn = 1455,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x3764,
             .to_chd = 0x0 }, // ef05.bin
    [26] = { .num_of_1st = 29344,
             .apfn = 1456,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x168D0,
             .to_chd = 0x0 }, // ef06.bin
    [27] = { .num_of_1st = 30640,
             .apfn = 1461,
             .mode = TEXGROUP_MODE_SHARED,
             .to_tex = 0x169C,
             .to_chd = 0x0 }, // ef07_A.bin
    [28] = TexGroupData_Empty,
    [29] = TexGroupData_Empty,
    [30] = { .num_of_1st = 30896,
             .apfn = 1457,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0xD78,
             .to_chd = 0x0 }, // ef25.bin
    [31] = TexGroupData_Empty,
    [32] = TexGroupData_Empty,
    [33] = { .num_of_1st = 31152,
             .apfn = 1446,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x1D77C,
             .to_chd = 0x0 }, // ef32.bin
    [34] = { .num_of_1st = 32432,
             .apfn = 1444,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0xA14,
             .to_chd = 0x0 }, // ef34_A.bin
    [35] = { .num_of_1st = 36896,
             .apfn = 1462,
             .mode = TEXGROUP_MODE_SHARED,
             .to_tex = 0x1074,
             .to_chd = 0x0 }, // ef36.bin
    [36] = TexGroupData_Empty,
    [37] = TexGroupData_Empty,
    [38] = { .num_of_1st = 32560,
             .apfn = 1458,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x6200,
             .to_chd = 0x0 }, // ef40.bin
    [39] = TexGroupData_Empty,
    [40] = TexGroupData_Empty,
    [41] = TexGroupData_Empty,
    [42] = { .num_of_1st = 34352,
             .apfn = 1401,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x6A8,
             .to_chd = 0x0 }, // bg00_A.bin
    [43] = { .num_of_1st = 34384,
             .apfn = 1410,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x884,
             .to_chd = 0x0 }, // bg01.bin
    [44] = { .num_of_1st = 34448,
             .apfn = 1389,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x4D3C,
             .to_chd = 0x0 }, // bg02_A.bin
    [45] = { .num_of_1st = 34576,
             .apfn = 1395,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0xCB8,
             .to_chd = 0x0 }, // bg03_A.bin
    [46] = { .num_of_1st = 34672,
             .apfn = 1428,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x688,
             .to_chd = 0x0 }, // bg04.bin
    [47] = { .num_of_1st = 34704,
             .apfn = 1405,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x728,
             .to_chd = 0x0 }, // bg05.bin
    [48] = { .num_of_1st = 34736,
             .apfn = 1413,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x8AC,
             .to_chd = 0x0 }, // bg06_A.bin
    [49] = { .num_of_1st = 34832,
             .apfn = 1425,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x128,
             .to_chd = 0x0 }, // bg07.bin
    [50] = { .num_of_1st = 34864,
             .apfn = 1398,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x2FB0,
             .to_chd = 0x0 }, // bg08.bin
    [51] = { .num_of_1st = 34960,
             .apfn = 1434,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0xFB0,
             .to_chd = 0x0 }, // bg09.bin
    [52] = { .num_of_1st = 35024,
             .apfn = 1386,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x11D8,
             .to_chd = 0x0 }, // bg11.bin
    [53] = { .num_of_1st = 35120,
             .apfn = 1407,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x44C,
             .to_chd = 0x0 }, // bg12.bin
    [54] = { .num_of_1st = 35152,
             .apfn = 1443,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x40,
             .to_chd = 0x0 }, // bg13.bin
    [55] = { .num_of_1st = 35184,
             .apfn = 1440,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x1CCC,
             .to_chd = 0x0 }, // bg14.bin
    [56] = { .num_of_1st = 35328,
             .apfn = 1431,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x448C,
             .to_chd = 0x0 }, // bg15.bin
    [57] = TexGroupData_Empty,
    [58] = { .num_of_1st = 35648,
             .apfn = 1392,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x834,
             .to_chd = 0x0 }, // bg17.bin
    [59] = { .num_of_1st = 35744,
             .apfn = 1448,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x2130,
             .to_chd = 0x0 }, // bg18.bin
    [60] = TexGroupData_Empty,
    [61] = { .num_of_1st = 35904,
             .apfn = 74,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0xD4DC,
             .to_chd = 0x0 }, // bg20.bin
    [62] = { .num_of_1st = 36096,
             .apfn = 34,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x1998,
             .to_chd = 0x0 }, // bg21.bin
    [63] = { .num_of_1st = 36160,
             .apfn = 35,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x604,
             .to_chd = 0x0 }, // bg22.bin
    [64] = { .num_of_1st = 36192,
             .apfn = 36,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x2CD8,
             .to_chd = 0x0 }, // bg23.bin
    [65] = { .num_of_1st = 36288,
             .apfn = 37,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x8FC,
             .to_chd = 0x0 }, // bg24.bin
    [66] = { .num_of_1st = 36320,
             .apfn = 38,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x6B0,
             .to_chd = 0x0 }, // bg25.bin
    [67] = { .num_of_1st = 36352,
             .apfn = 39,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x193C,
             .to_chd = 0x0 }, // bg26.bin
    [68] = { .num_of_1st = 36384,
             .apfn = 40,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x91C,
             .to_chd = 0x0 }, // bg27.bin
    [69] = { .num_of_1st = 36416,
             .apfn = 41,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0xD54,
             .to_chd = 0x0 }, // bg28.bin
    [70] = { .num_of_1st = 36448,
             .apfn = 42,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x1BC,
             .to_chd = 0x0 }, // bg29.bin
    [71] = { .num_of_1st = 36480,
             .apfn = 43,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x430,
             .to_chd = 0x0 }, // bg30.bin
    [72] = { .num_of_1st = 36512,
             .apfn = 44,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0xB10,
             .to_chd = 0x0 }, // bg31.bin
    [73] = { .num_of_1st = 36544,
             .apfn = 45,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x1244,
             .to_chd = 0x0 }, // bg32.bin
    [74] = { .num_of_1st = 36576,
             .apfn = 46,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x1768,
             .to_chd = 0x0 }, // bg33.bin
    [75] = { .num_of_1st = 36608,
             .apfn = 47,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x22C8,
             .to_chd = 0x0 }, // bg34.bin
    [76] = { .num_of_1st = 36640,
             .apfn = 48,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x38AC,
             .to_chd = 0x0 }, // bg35.bin
    [77] = { .num_of_1st = 36704,
             .apfn = 49,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x600,
             .to_chd = 0x0 }, // bg36.bin
    [78] = { .num_of_1st = 36736,
             .apfn = 50,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0xB98,
             .to_chd = 0x0 }, // bg37.bin
    [79] = { .num_of_1st = 36768,
             .apfn = 51,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x19E4,
             .to_chd = 0x0 }, // bg38.bin
    [80] = { .num_of_1st = 36800,
             .apfn = 52,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0xB34,
             .to_chd = 0x0 }, // bg39.bin
    [81] = { .num_of_1st = 36864,
             .apfn = 53,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x12A8,
             .to_chd = 0x0 }, // bg40.bin
    [82] = { .num_of_1st = 37024,
             .apfn = 1459,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0xA534,
             .to_chd = 0x0 }, // ef65.bin
    [83] = { .num_of_1st = 37408,
             .apfn = 1384,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x9B8,
             .to_chd = 0x0 }, // ef70.bin
    [84] = { .num_of_1st = 37536,
             .apfn = 1385,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x32B8,
             .to_chd = 0x0 }, // ef71.bin
    [85] = { .num_of_1st = 34576,
             .apfn = 1416,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0xCB8,
             .to_chd = 0x0 }, // bg03_B.bin
    [86] = { .num_of_1st = 34448,
             .apfn = 1419,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x4D3C,
             .to_chd = 0x0 }, // bg02_B.bin
    [87] = { .num_of_1st = 34736,
             .apfn = 1422,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x8AC,
             .to_chd = 0x0 }, // bg06_B.bin
    [88] = { .num_of_1st = 34352,
             .apfn = 1437,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x6A8,
             .to_chd = 0x0 }, // bg00_B.bin
    [89] = { .num_of_1st = 30640,
             .apfn = 1469,
             .mode = TEXGROUP_MODE_SHARED,
             .to_tex = 0x169C,
             .to_chd = 0x0 }, // ef07_B.bin
    [90] = { .num_of_1st = 30640,
             .apfn = 1488,
             .mode = TEXGROUP_MODE_SHARED,
             .to_tex = 0x169C,
             .to_chd = 0x0 }, // ef07_C.bin
    [91] = { .num_of_1st = 30640,
             .apfn = 1496,
             .mode = TEXGROUP_MODE_SHARED,
             .to_tex = 0x169C,
             .to_chd = 0x0 }, // ef07_D.bin
    [92] = { .num_of_1st = 30640,
             .apfn = 1503,
             .mode = TEXGROUP_MODE_SHARED,
             .to_tex = 0x169C,
             .to_chd = 0x0 }, // ef07_E.bin
    [93] = { .num_of_1st = 30640,
             .apfn = 1507,
             .mode = TEXGROUP_MODE_SHARED,
             .to_tex = 0x169C,
             .to_chd = 0x0 }, // ef07_F.bin
    [94] = { .num_of_1st = 30640,
             .apfn = 1511,
             .mode = TEXGROUP_MODE_SHARED,
             .to_tex = 0x169C,
             .to_chd = 0x0 }, // ef07_G.bin
    [95] = { .num_of_1st = 30640,
             .apfn = 1515,
             .mode = TEXGROUP_MODE_SHARED,
             .to_tex = 0x169C,
             .to_chd = 0x0 }, // ef07_H.bin
    [96] = { .num_of_1st = 30640,
             .apfn = 1519,
             .mode = TEXGROUP_MODE_SHARED,
             .to_tex = 0x169C,
             .to_chd = 0x0 }, // ef07_I.bin
    [97] = { .num_of_1st = 30640,
             .apfn = 1532,
             .mode = TEXGROUP_MODE_SHARED,
             .to_tex = 0x169C,
             .to_chd = 0x0 }, // ef07_J.bin
    [98] = { .num_of_1st = 27104,
             .apfn = 1453,
             .mode = TEXGROUP_MODE_NORMAL,
             .to_tex = 0x20B9C,
             .to_chd = 0x0 }, // ef02.bin
    [99] = TexGroupData_Empty,
};
