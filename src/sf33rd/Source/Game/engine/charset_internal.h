#ifndef CHARSET_INTERNAL_H
#define CHARSET_INTERNAL_H

#include "structs.h"
#include "types.h"

#define LO_2_BYTES(_val) (((s16*)&_val)[0])
#define HI_2_BYTES(_val) (((s16*)&_val)[1])
#define WK_AS_PLW ((PLW*)wk)

/* Shared between charset.c and charset_commands.c, which were one 2842-line
 * file with 168 functions in it. Nothing here is meant to be called from
 * outside that pair. */

/* Defined in charset.c; the script commands reach these. */
s16 decord_if_jump(WORK* wk, UNK11* cpc, s16 ix);
u16 get_comm_if_lever(WORK* wk);
u16 get_comm_if_shot(WORK* wk);
u16 get_comm_if_shot_now_off(WORK* wk);
u16 get_comm_if_shot_now(WORK* wk);
u16 get_comm_if_lvsh(WORK* wk);
u8 get_comm_djmp_lever_dir(PLW* wk);
u16 check_xcopy_filter_se_req(WORK* wk);

/* Was `static` in charset.c. It is read by the sound-effect request in both
 * halves - check_cgd_patdat kept it here, comm_se moved to the other file - so
 * the split is what widened it, and nothing else should call it. */
u16 check_xcopy_filter_se_req(WORK* wk);

/* Defined in charset_commands.c, named by decode_chcmd in charset.c. */
s32 comm_dummy(WORK* /* unused */, UNK11* /* unused */);
s32 comm_roa(WORK* wk, UNK11* /* unused */);
s32 comm_end(WORK* wk, UNK11* ctc);
s32 comm_jmp(WORK* wk, UNK11* ctc);
s32 comm_jpss(WORK* wk, UNK11* ctc);
s32 comm_jsr(WORK* wk, UNK11* ctc);
s32 comm_ret(WORK* wk, UNK11* /* unused */);
s32 comm_sps(WORK* wk, UNK11* ctc);
s32 comm_setr(WORK* wk, UNK11* ctc);
s32 comm_addr(WORK* wk, UNK11* ctc);
s32 comm_if_l(WORK* wk, UNK11* ctc);
s32 comm_djmp(WORK* wk, UNK11* ctc);
s32 comm_for(WORK* wk, UNK11* ctc);
s32 comm_nex(WORK* wk, UNK11* ctc);
s32 comm_for2(WORK* wk, UNK11* ctc);
s32 comm_nex2(WORK* wk, UNK11* ctc);
s32 comm_rja(WORK* wk, UNK11* ctc);
s32 comm_uja(WORK* wk, UNK11* ctc);
s32 comm_rja2(WORK* wk, UNK11* ctc);
s32 comm_uja2(WORK* wk, UNK11* ctc);
s32 comm_rja3(WORK* wk, UNK11* ctc);
s32 comm_uja3(WORK* wk, UNK11* ctc);
s32 comm_rja4(WORK* wk, UNK11* ctc);
s32 comm_uja4(WORK* wk, UNK11* /* unused */);
s32 comm_rja5(WORK* wk, UNK11* ctc);
s32 comm_uja5(WORK* wk, UNK11* /* unused */);
s32 comm_rja6(WORK* wk, UNK11* ctc);
s32 comm_uja6(WORK* wk, UNK11* ctc);
s32 comm_rja7(WORK* wk, UNK11* ctc);
s32 comm_uja7(WORK* wk, UNK11* ctc);
s32 comm_rmja(WORK* wk, UNK11* ctc);
s32 comm_umja(WORK* wk, UNK11* /* unused */);
s32 comm_mdat(WORK* wk, UNK11* ctc);
s32 comm_ydat(WORK* wk, UNK11* ctc);
s32 comm_mpos(WORK* wk, UNK11* ctc);
s32 comm_cafr(WORK* wk, UNK11* ctc);
s32 comm_care(WORK* wk, UNK11* ctc);
s32 comm_psxy(WORK* wk, UNK11* ctc);
s32 comm_ps_x(WORK* wk, UNK11* ctc);
s32 comm_ps_y(WORK* wk, UNK11* ctc);
s32 comm_paxy(WORK* wk, UNK11* ctc);
s32 comm_pa_x(WORK* wk, UNK11* ctc);
s32 comm_pa_y(WORK* wk, UNK11* ctc);
s32 comm_exec(WORK* wk, UNK11* ctc);
s32 comm_rngc(WORK* wk, UNK11* ctc);
s32 comm_mxyt(WORK* wk, UNK11* ctc);
s32 comm_pjmp(WORK* wk, UNK11* ctc);
s32 comm_hjmp(WORK* wk, UNK11* ctc);
s32 comm_hclr(WORK* wk, UNK11* /* unused */);
s32 comm_ixfw(WORK* wk, UNK11* ctc);
s32 comm_ixbw(WORK* wk, UNK11* ctc);
s32 comm_quax(WORK* /* unused */, UNK11* ctc);
s32 comm_quay(WORK* /* unused */, UNK11* ctc);
s32 comm_if_s(WORK* wk, UNK11* ctc);
s32 comm_rapp(WORK* wk, UNK11* ctc);
s32 comm_rapk(WORK* wk, UNK11* ctc);
s32 comm_gets(WORK* wk, UNK11* /* unused */);
s32 comm_s123(WORK* wk, UNK11* ctc);
s32 comm_s456(WORK* wk, UNK11* ctc);
s32 comm_a123(WORK* wk, UNK11* ctc);
s32 comm_a456(WORK* wk, UNK11* ctc);
s32 comm_stop(PLW* wk, UNK11* ctc);
s32 comm_smhf(WORK* wk, UNK11* ctc);
s32 comm_ngme(WORK* wk, UNK11* /* unused */);
s32 comm_ngem(WORK* wk, UNK11* /* unused */);
s32 comm_iflb(WORK* wk, UNK11* ctc);
s32 comm_asxy(WORK* wk, UNK11* ctc);
s32 comm_schx(WORK* wk, UNK11* ctc);
s32 comm_schy(WORK* wk, UNK11* ctc);
s32 comm_back(WORK* wk, UNK11* /* unused */);
s32 comm_mvix(WORK* wk, UNK11* ctc);
s32 comm_sajp(WORK* wk, UNK11* ctc);
s32 comm_ccch(WORK* wk, UNK11* ctc);
s32 comm_wset(WORK* wk, UNK11* ctc);
s32 comm_wswk(WORK* wk, UNK11* ctc);
s32 comm_wadd(WORK* wk, UNK11* ctc);
s32 comm_wceq(WORK* wk, UNK11* ctc);
s32 comm_wcne(WORK* wk, UNK11* ctc);
s32 comm_wcgt(WORK* wk, UNK11* ctc);
s32 comm_wclt(WORK* wk, UNK11* ctc);
s32 comm_wadd2(WORK* wk, UNK11* ctc);
s32 comm_wceq2(WORK* wk, UNK11* ctc);
s32 comm_wcne2(WORK* wk, UNK11* ctc);
s32 comm_wcgt2(WORK* wk, UNK11* ctc);
s32 comm_wclt2(WORK* wk, UNK11* ctc);
s32 comm_rapp2(WORK* wk, UNK11* ctc);
s32 comm_rapk2(WORK* wk, UNK11* ctc);
s32 comm_iflg(WORK* wk, UNK11* ctc);
s32 comm_mpcy(WORK* wk, UNK11* ctc);
s32 comm_epcy(WORK* wk, UNK11* ctc);
s32 comm_imgs(PLW* wk, UNK11* ctc);
s32 comm_imgc(PLW* wk, UNK11* ctc);
s32 comm_rvxy(WORK* wk, UNK11* ctc);
s32 comm_rv_x(WORK* wk, UNK11* ctc);
s32 comm_rv_y(WORK* wk, UNK11* ctc);
s32 comm_ccfl(PLW* wk, UNK11* /* unused */);
s32 comm_myhp(WORK* wk, UNK11* ctc);
s32 comm_emhp(WORK* wk, UNK11* ctc);
s32 comm_exbgs(WORK* /* unused */, UNK11* /* unused */);
s32 comm_exbgc(WORK* /* unused */, UNK11* /* unused */);
s32 comm_atmf(PLW* wk, UNK11* ctc);
s32 comm_chkwf(PLW* wk, UNK11* ctc);
s32 comm_retmj(PLW* wk, UNK11* /* unused */);
s32 comm_sstx(WORK* wk, UNK11* ctc);
s32 comm_ssty(WORK* wk, UNK11* ctc);
s32 comm_ngda(WORK* wk, UNK11* ctc);
s32 comm_flip(WORK* wk, UNK11* /* unused */);
s32 comm_kage(WORK* wk, UNK11* ctc);
s32 comm_dspf(WORK* wk, UNK11* ctc);
s32 comm_ifrlf(WORK* wk, UNK11* ctc);
s32 comm_srlf(WORK* wk, UNK11* ctc);
s32 comm_bgrlf(WORK* wk, UNK11* ctc);
s32 comm_scmd(PLW* wk, UNK11* ctc);
s32 comm_rljmp(WORK* wk, UNK11* ctc);
s32 comm_ifs2(WORK* wk, UNK11* ctc);
s32 comm_abbak(WORK* wk, UNK11* /* unused */);
s32 comm_sse(WORK* wk, UNK11* ctc);
s32 comm_s_chg(WORK* wk, UNK11* ctc);
s32 comm_schg2(WORK* wk, UNK11* ctc);
s32 comm_rhsja(PLW* wk, UNK11* ctc);
s32 comm_uhsja(PLW* wk, UNK11* /* unused */);
s32 comm_ifcom(WORK* wk, UNK11* ctc);
s32 comm_axjmp(WORK* wk, UNK11* ctc);
s32 comm_ayjmp(WORK* wk, UNK11* ctc);
s32 comm_ifs3(WORK* wk, UNK11* ctc);

#endif // CHARSET_INTERNAL_H
