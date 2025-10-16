#ifndef CHARSET_H
#define CHARSET_H

#include "structs.h"

extern const u16 acatkoa_table[];

void setupCharTableData(WORK* wk, s32 clr, s32 info);
void char_move_cmhs(PLW* wk);
void char_move(WORK* wk);
void check_cm_extended_code(WORK* wk);
void setup_comm_back(WORK* wk);
void setup_comm_abbak(WORK* wk);
void check_cgd_patdat(WORK* wk);
void set_jugde_area(WORK* wk);
void set_new_attnum(WORK* wk);
void set_char_move_init(WORK* wk, s16 koc, s16 index);
void set_char_move_init2(WORK* wk, s16 koc, s16 index, s16 ip, s16 scf);
void char_move_z(WORK* wk);
void char_move_wca(WORK* wk);
void char_move_wca_init(WORK* wk);
void char_move_cmja(WORK* wk);
void exset_char_move_init(WORK* wk, s16 koc, s16 index);
void char_move_cmms(WORK* wk);
void char_move_cmms2(WORK* wk);
s32 char_move_cmms3(PLW* wk);
void char_move_index(WORK* wk, s16 ix);
void char_move_cmj4(WORK* wk);
void get_char_data_zanzou(WORK* wk);

#endif
