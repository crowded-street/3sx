#include "sf33rd/Source/Game/Message3rd/C_USA/msgTable_usa.h"
#include "common.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL00End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL00Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL00Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL01End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL01Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL01Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL02End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL02Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL02Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL03End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL03Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL03Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL04End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL04Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL04Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL05End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL05Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL05Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL06End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL06Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL06Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL07End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL07Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL07Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL08End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL08Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL08Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL09End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL09Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL09Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL10End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL10Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL10Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL11End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL11Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL11Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL12End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL12Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL12Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL13End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL13Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL13Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL14End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL14Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL15End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL15Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL15Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL16End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL16Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL16Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL17End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL17Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL18End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL18Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL18Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL19End_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL19Tlk_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/PL19Win_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/msgExtra_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/msgMenu_usa.h"
#include "sf33rd/Source/Game/Message3rd/C_USA/msgSysDir_usa.h"

MessageTable* pl_mes_tbl[20] = { &pl00win_usa_tbl, &pl01win_usa_tbl, &pl02win_usa_tbl, &pl03win_usa_tbl,
                                 &pl04win_usa_tbl, &pl05win_usa_tbl, &pl06win_usa_tbl, &pl07win_usa_tbl,
                                 &pl08win_usa_tbl, &pl09win_usa_tbl, &pl10win_usa_tbl, &pl11win_usa_tbl,
                                 &pl12win_usa_tbl, &pl13win_usa_tbl, &pl14win_usa_tbl, &pl15win_usa_tbl,
                                 &pl16win_usa_tbl, &pl17win_usa_tbl, &pl18win_usa_tbl, &pl19win_usa_tbl };

MessageTable* pl_tlk_tbl[20] = { &pl00tlk_usa_tbl, &pl01tlk_usa_tbl, &pl02tlk_usa_tbl, &pl03tlk_usa_tbl,
                                 &pl04tlk_usa_tbl, &pl05tlk_usa_tbl, &pl06tlk_usa_tbl, &pl07tlk_usa_tbl,
                                 &pl08tlk_usa_tbl, &pl09tlk_usa_tbl, &pl10tlk_usa_tbl, &pl11tlk_usa_tbl,
                                 &pl12tlk_usa_tbl, &pl13tlk_usa_tbl, &pl13tlk_usa_tbl, &pl15tlk_usa_tbl,
                                 &pl16tlk_usa_tbl, &pl16tlk_usa_tbl, &pl18tlk_usa_tbl, &pl19tlk_usa_tbl };

MessageTable* pl_end_tbl[20] = { &pl00end_usa_tbl, &pl01end_usa_tbl, &pl02end_usa_tbl, &pl03end_usa_tbl,
                                 &pl04end_usa_tbl, &pl05end_usa_tbl, &pl06end_usa_tbl, &pl07end_usa_tbl,
                                 &pl08end_usa_tbl, &pl09end_usa_tbl, &pl10end_usa_tbl, &pl11end_usa_tbl,
                                 &pl12end_usa_tbl, &pl13end_usa_tbl, &pl14end_usa_tbl, &pl15end_usa_tbl,
                                 &pl16end_usa_tbl, &pl17end_usa_tbl, &pl18end_usa_tbl, &pl19end_usa_tbl };

MessageTable* msgSysDirTbl[] = { &msgSysDirTbl_usa };

MessageTable* msgExtraTbl[] = { &msgExtraTbl_usa };

MessageTable* msgMenuTbl[] = { &msgMenuTbl_usa };
