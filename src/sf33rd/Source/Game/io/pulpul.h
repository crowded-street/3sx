#ifndef PULPUL_H
#define PULPUL_H

#include "structs.h"
#include "types.h"

#define PULREQ_MAX 51
#define PULPARA_MAX 54

extern s16 vib_req[2][2];
extern u8 pulpul_scene;
extern PPWORK ppwork[2];
extern PULREQ pulreq[];
extern PULPARA pulpara[];

void init_pulpul_work();
void pulpul_stop();
void pulpul_stop2(s16 ix);
void pp_vib_on(s16 id);
void pp_operator_check_flag(u8 fl);
void move_pulpul_work();
void pp_screen_quake(s16 ix);
void init_pulpul_work2(s16 ix);
void init_pulpul_round2(s16 ix);
void pulpul_request(s16 id, s16 ix);
void pulpul_req_copy(s16 id, PULREQ* adr);
void pulpul_request_again();
s32 chkVibUnit(s32 port);
void move_pulpul(PPWORK* wk);
s32 pulpul_pdVibMxStart(PPWORK* wk, s32 arg1, s32 arg2, PULPARA* param);
s32 vibParamTrans(s32 id, PULPARA* prm);
void pp_pulpara_remake_at_init();
void pp_pulpara_remake_at_init2();
void pp_pulpara_remake_at_hit();
void pp_pulpara_remake_at();
void pp_pulpara_remake_dm_all(WORK* wk);
void pp_pulpara_guard(WORK* wk);
void pp_pulpara_hit(WORK* wk);
void pp_pulpara_blocking(WORK* wk);
void pp_pulpara_catch(WORK* wk);
void pp_pulpara_caught(WORK* wk);
void pp_pulpara_shungokusatsu(WORK* wk);

#endif
