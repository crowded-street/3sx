/**
 * @file game_state_load.c
 * Restoring the rollback state saved by game_state.c. The two files walk the
 * same member list in the same order, in opposite directions.
 */

#include "platform/netplay/game_state_internal.h"

#define GS_LOAD(member)                                                                                                \
    GS_ASSERT_SAME_SIZE(member);                                                                                       \
    SDL_memcpy(&member, &src->member, sizeof(member))

void GameState_Load(const GameState* src) {
    GS_LOAD(Scene_Cut);
    GS_LOAD(Time_Over);
    GS_LOAD(round_timer);
    GS_LOAD(flash_timer);
    GS_LOAD(flash_r_num);
    GS_LOAD(flash_col);
    GS_LOAD(math_counter_hi);
    GS_LOAD(math_counter_low);
    GS_LOAD(counter_color);
    GS_LOAD(mugen_flag);
    GS_LOAD(hoji_counter);
    GS_LOAD(Order);
    GS_LOAD(Order_Timer);
    GS_LOAD(Order_Dir);
    GS_LOAD(Score);
    GS_LOAD(Complete_Bonus);
    GS_LOAD(Stock_Score);
    GS_LOAD(Vital_Bonus);
    GS_LOAD(Time_Bonus);
    GS_LOAD(Stage_Stock_Score);
    GS_LOAD(Bonus_Score);
    GS_LOAD(Final_Bonus_Score);
    GS_LOAD(WGJ_Score);
    GS_LOAD(Bonus_Score_Plus);
    GS_LOAD(Perfect_Bonus);
    GS_LOAD(Keep_Score);
    GS_LOAD(Disp_Score_Buff);
    GS_LOAD(Winner_id);
    GS_LOAD(Loser_id);
    GS_LOAD(Break_Into);
    GS_LOAD(My_char);
    GS_LOAD(Allow_a_battle_f);
    GS_LOAD(Round_num);
    GS_LOAD(Complete_Judgement);
    GS_LOAD(Fade_Flag);
    GS_LOAD(Super_Arts);
    GS_LOAD(Forbid_Break);
    GS_LOAD(Request_Break);
    GS_LOAD(Continue_Count);
    GS_LOAD(Counter_hi);
    GS_LOAD(Counter_low);
    GS_LOAD(Unit_Of_Timer);
    GS_LOAD(Select_Timer);
    GS_LOAD(Cursor_X);
    GS_LOAD(Cursor_Y);
    GS_LOAD(Cursor_Y_Pos);
    GS_LOAD(Cursor_Timer);
    GS_LOAD(Time_Stop);
    GS_LOAD(Suicide);
    GS_LOAD(Complete_Face);
    GS_LOAD(Play_Type);
    GS_LOAD(Sel_PL_Complete);
    GS_LOAD(New_Challenger);
    GS_LOAD(S_No);
    GS_LOAD(Select_Start);
    GS_LOAD(request_message);
    GS_LOAD(judge_flag);
    GS_LOAD(WINNER);
    GS_LOAD(LOSER);
    GS_LOAD(Champion);
    GS_LOAD(Fade_Half_Flag);
    GS_LOAD(Reserve_Cut);
    GS_LOAD(Perfect_Flag);
    GS_LOAD(Next_Step);
    GS_LOAD(Switch_Type);
    GS_LOAD(Cover_Timer);
    GS_LOAD(Personal_Timer);
    GS_LOAD(Request_E_No);
    GS_LOAD(Request_G_No);
    GS_LOAD(Present_Rank);
    GS_LOAD(Best_Grade);
    GS_LOAD(Demo_Type);
    GS_LOAD(Rank_Type);
    GS_LOAD(Flash_Sign);
    GS_LOAD(Flash_Rank_Time);
    GS_LOAD(Flash_Rank_Interval);
    GS_LOAD(Ranking_X);
    GS_LOAD(Rank);
    GS_LOAD(Rank_X);
    GS_LOAD(E_07_Flag);
    GS_LOAD(Complete_Victory);
    GS_LOAD(Demo_Flag);
    GS_LOAD(Next_Demo);
    GS_LOAD(Demo_PL_Index);
    GS_LOAD(Demo_Stage_Index);
    GS_LOAD(Face_MV_Request);
    GS_LOAD(Face_Move);
    GS_LOAD(Player_id);
    GS_LOAD(Last_Player_id);
    GS_LOAD(Player_Number);
    GS_LOAD(DENJIN_Term);
    GS_LOAD(Rapid_No);
    GS_LOAD(COM_id);
    GS_LOAD(EM_id);
    GS_LOAD(Select_Status);
    GS_LOAD(Select_Demo_Index);
    GS_LOAD(Country);
    GS_LOAD(Demo_Time_Stop);
    GS_LOAD(Combo_Speed);
    GS_LOAD(Exec_Wipe);
    GS_LOAD(Passive_Mode);
    GS_LOAD(Passive_Flag);
    GS_LOAD(Flip_Flag);
    GS_LOAD(Lie_Flag);
    GS_LOAD(Counter_Attack);
    GS_LOAD(Attack_Flag);
    GS_LOAD(Limited_Flag);
    GS_LOAD(Shell_Ignore_Timer);
    GS_LOAD(Event_Judge_Gals);
    GS_LOAD(EJG_index);
    GS_LOAD(Guard_Flag);
    GS_LOAD(Pierce_Menu);
    GS_LOAD(Face_MV_Time);
    GS_LOAD(Before_Jump);
    GS_LOAD(Stop_Combo);
    GS_LOAD(Stock_Hit_Flag);
    GS_LOAD(Rolling_Flag);
    GS_LOAD(Continue_Coin);
    GS_LOAD(Ignore_Entry);
    GS_LOAD(Slide_Type);
    GS_LOAD(Moving_Plate);
    GS_LOAD(Naming_Cut);
    GS_LOAD(Moving_Plate_Counter);
    GS_LOAD(Player_Color);
    GS_LOAD(PP_Priority);
    GS_LOAD(OK_Priority);
    GS_LOAD(Stock_My_char);
    GS_LOAD(Stock_Player_Color);
    GS_LOAD(Music_Fade);
    GS_LOAD(Stop_SG);
    GS_LOAD(Operator_Status);
    GS_LOAD(Round_Operator);
    GS_LOAD(another_bg);
    GS_LOAD(Last_Super_Arts);
    GS_LOAD(Last_My_char);
    GS_LOAD(Continue_Menu);
    GS_LOAD(Timer_Freeze);
    GS_LOAD(Type_of_Attack);
    GS_LOAD(Standing_Timer);
    GS_LOAD(Before_Look);
    GS_LOAD(Attack_Count_No0);
    GS_LOAD(Standing_Master_Timer);
    GS_LOAD(PB_Music_Off);
    GS_LOAD(No_Death);
    GS_LOAD(Flash_MT);
    GS_LOAD(Squat_Timer);
    GS_LOAD(Squat_Master_Timer);
    GS_LOAD(Turn_Over);
    GS_LOAD(Turn_Over_Timer);
    GS_LOAD(Jump_Pass_Timer);
    GS_LOAD(sa_gauge_flash);
    GS_LOAD(Receive_Flag);
    GS_LOAD(Disposal_Again);
    GS_LOAD(BGM_Vol);
    GS_LOAD(Used_char);
    GS_LOAD(Break_Com);
    GS_LOAD(aiuchi_flag);
    GS_LOAD(paring_counter);
    GS_LOAD(paring_bonus_r);
    GS_LOAD(paring_ctr_vs);
    GS_LOAD(paring_ctr_ori);
    GS_LOAD(Attack_Count_Buff);
    GS_LOAD(Attack_Count_Index);
    GS_LOAD(CC_Value);
    GS_LOAD(Continue_Coin2);
    GS_LOAD(Weak_PL);
    GS_LOAD(Bullet_No);
    GS_LOAD(Bullet_Counter);
    GS_LOAD(Final_Result_id);
    GS_LOAD(Disp_Win_Name);
    GS_LOAD(Perfect_Counter);
    GS_LOAD(Straight_Counter);
    GS_LOAD(Appear_Q);
    GS_LOAD(Cut_Scroll);
    GS_LOAD(Break_Into_CPU);
    GS_LOAD(ID_of_Face);
    GS_LOAD(Cursor_Move);
    GS_LOAD(Auto_Cursor);
    GS_LOAD(Auto_No);
    GS_LOAD(Auto_Index);
    GS_LOAD(Auto_Timer);
    GS_LOAD(Explosion);
    GS_LOAD(Introduce_Break_Into);
    GS_LOAD(gouki_wins);
    GS_LOAD(EM_Rank);
    GS_LOAD(Disp_PERFECT);
    GS_LOAD(Escape_SS);
    GS_LOAD(Deley_Shot_No);
    GS_LOAD(Deley_Shot_Timer);
    GS_LOAD(Lost_Round);
    GS_LOAD(Super_Arts_Finish);
    GS_LOAD(Stage_SA_Finish);
    GS_LOAD(Perfect_Finish);
    GS_LOAD(Cheap_Finish);
    GS_LOAD(Last_My_char2);
    GS_LOAD(gouki_app);
    GS_LOAD(Bonus_Game_Complete);
    GS_LOAD(Get_Demo_Index);
    GS_LOAD(Combo_Demo_Flag);
    GS_LOAD(Stage_Continue);
    GS_LOAD(Pause_Hit_Marks);
    GS_LOAD(Extra_Break);
    GS_LOAD(Shin_Gouki_BGM);
    GS_LOAD(Stage_Lost_Round);
    GS_LOAD(Stage_Perfect_Finish);
    GS_LOAD(Stage_Cheap_Finish);
    GS_LOAD(EXE_obroll);
    GS_LOAD(End_PL);
    GS_LOAD(Stock_Com_Arts);
    GS_LOAD(PB_Status);
    GS_LOAD(Flip_Counter);
    GS_LOAD(Stage_Time_Finish);
    GS_LOAD(Bonus_Type);
    GS_LOAD(Completion_Bonus);
    GS_LOAD(ichikannkei);
    GS_LOAD(Plate_Disposal_No);
    GS_LOAD(SO_No);
    GS_LOAD(Disp_Command_Name);
    GS_LOAD(OK_Appear79);
    GS_LOAD(Extra_Counter);
    GS_LOAD(SC_No);
    GS_LOAD(BGM_No);
    GS_LOAD(BGM_Timer);
    GS_LOAD(EM_List);
    GS_LOAD(Sel_EM_Complete);
    GS_LOAD(Temporary_EM);
    GS_LOAD(OK_Moving_SA_Plate);
    GS_LOAD(Battle_Q);
    GS_LOAD(EM_History);
    GS_LOAD(GO_No);
    GS_LOAD(Aborigine);
    GS_LOAD(Continue_Count_Down);
    GS_LOAD(WGJ_Target);
    GS_LOAD(EM_Candidate);
    GS_LOAD(Last_Selected_EM);
    GS_LOAD(Q_Country);
    GS_LOAD(Continue_Cut);
    GS_LOAD(Introduce_Boss);
    GS_LOAD(Final_Play_Type);
    GS_LOAD(Rank_In);
    GS_LOAD(Request_Disp_Rank);
    GS_LOAD(Reset_Timer);
    GS_LOAD(bbbs_type);
    GS_LOAD(Straight_Flag);
    GS_LOAD(kakushi_ix);
    GS_LOAD(kakushi_op);
    GS_LOAD(RO_backup);
    GS_LOAD(PT_backup);
    GS_LOAD(E_Number);
    GS_LOAD(E_No);
    GS_LOAD(C_No);
    GS_LOAD(G_No);
    GS_LOAD(D_No);
    GS_LOAD(M_No);
    GS_LOAD(Exit_No);
    GS_LOAD(SP_No);
    GS_LOAD(Face_No);
    GS_LOAD(Stop_Cursor);
    GS_LOAD(Training_Index);
    GS_LOAD(Connect_Status);
    GS_LOAD(Menu_Suicide);
    GS_LOAD(Game_pause);
    GS_LOAD(Game_difficulty);
    GS_LOAD(Pause);
    GS_LOAD(Pause_ID);
    GS_LOAD(Exit_Menu);
    GS_LOAD(Conclusion_Flag);
    GS_LOAD(CP_No);
    GS_LOAD(CP_Index);
    GS_LOAD(Gap_Timer);
    GS_LOAD(Message_Suicide);
    GS_LOAD(Disp_Cockpit);
    GS_LOAD(Select_Arts);
    GS_LOAD(Lamp_No);
    GS_LOAD(Lamp_Index);
    GS_LOAD(Lamp_Color);
    GS_LOAD(Stop_Update_Score);
    GS_LOAD(test_flag);
    GS_LOAD(ixbfw_cut);
    GS_LOAD(Cont_No);
    GS_LOAD(PL_Wins);
    GS_LOAD(Fade_R_No0);
    GS_LOAD(Fade_R_No1);
    GS_LOAD(Conclusion_Type);
    GS_LOAD(win_type);
    GS_LOAD(message_index);
    GS_LOAD(F_No0);
    GS_LOAD(F_No1);
    GS_LOAD(F_No2);
    GS_LOAD(F_No3);
    GS_LOAD(keep_condition);
    GS_LOAD(Check_Buff);
    GS_LOAD(Convert_Buff);
    GS_LOAD(Unsubstantial_BG);
    GS_LOAD(Menu_Cursor_X);
    GS_LOAD(Menu_Cursor_Y);
    GS_LOAD(Replay_Status);
    GS_LOAD(Disappear_LOGO);
    GS_LOAD(count_end);
    GS_LOAD(Play_Game);
    GS_LOAD(Menu_Cursor_Move);
    GS_LOAD(flash_win_type);
    GS_LOAD(sync_win_type);
    GS_LOAD(Mode_Type);
    GS_LOAD(Menu_Page);
    GS_LOAD(Menu_Max);
    GS_LOAD(reset_NG_flag);
    GS_LOAD(VS_Stage);
    GS_LOAD(Present_Mode);
    GS_LOAD(Play_Mode);
    GS_LOAD(Page_Max);
    GS_LOAD(Direction_Working);
    GS_LOAD(Vital_Handicap);
    GS_LOAD(Cursor_Limit);
    GS_LOAD(Synchro_No);
    GS_LOAD(SA_shadow_on);
    GS_LOAD(Pause_Down);
    GS_LOAD(Training_ID);
    GS_LOAD(Disp_Attack_Data);
    GS_LOAD(Record_Data_Tr);
    GS_LOAD(End_Training);
    GS_LOAD(Menu_Page_Buff);
    GS_LOAD(Reset_Bootrom);
    GS_LOAD(Decide_ID);
    GS_LOAD(Training_Cursor);
    GS_LOAD(Lag_Timer);
    GS_LOAD(CPU_Time_Lag);
    GS_LOAD(Forbid_Reset);
    GS_LOAD(CPU_Rec);
    GS_LOAD(Pause_Type);
    GS_LOAD(Game_timer);
    GS_LOAD(Control_Time);
    GS_LOAD(Time_in_Time);
    GS_LOAD(Round_Level);
    GS_LOAD(Round_Result);
    GS_LOAD(Fade_Number);
    GS_LOAD(G_Timer);
    GS_LOAD(D_Timer);
    GS_LOAD(Rank_Pos_X);
    GS_LOAD(Rank_Pos_Y);
    GS_LOAD(E_Timer);
    GS_LOAD(F_Timer);
    GS_LOAD(ENTRY_X);
    GS_LOAD(C_Timer);
    GS_LOAD(S_Timer);
    GS_LOAD(Flash_Complete);
    GS_LOAD(Sel_Arts_Complete);
    GS_LOAD(Arts_Y);
    GS_LOAD(Move_Super_Arts);
    GS_LOAD(Battle_Country);
    GS_LOAD(Face_Status);
    GS_LOAD(ID);
    GS_LOAD(ID2);
    GS_LOAD(mes_already);
    GS_LOAD(Timer_00);
    GS_LOAD(Timer_01);
    GS_LOAD(PL_Distance);
    GS_LOAD(Area_Number);
    GS_LOAD(Lever_Buff);
    GS_LOAD(Lever_Pool);
    GS_LOAD(Tech_Index);
    GS_LOAD(Random_ix16);
    GS_LOAD(Random_ix32);
    GS_LOAD(M_Timer);
    GS_LOAD(VS_Tech);
    GS_LOAD(Guard_Type);
    GS_LOAD(Separate_Area);
    GS_LOAD(Free_Lever);
    GS_LOAD(Term_No);
    GS_LOAD(Com_Width_Data);
    GS_LOAD(Lever_Squat);
    GS_LOAD(M_Lv);
    GS_LOAD(Insert_Y);
    GS_LOAD(scr_req_x);
    GS_LOAD(scr_req_y);
    GS_LOAD(zoom_req_flag_old);
    GS_LOAD(zoom_request_flag);
    GS_LOAD(zoom_request_level);
    GS_LOAD(Last_Selected_ID);
    GS_LOAD(Last_Called_SE);
    GS_LOAD(VS_Index);
    GS_LOAD(Rapid_Index);
    GS_LOAD(Shell_Separate_Area);
    GS_LOAD(Attack_Counter);
    GS_LOAD(Last_Attack_Counter);
    GS_LOAD(Pattern_Index);
    GS_LOAD(Com_Color_Shot);
    GS_LOAD(Resume_Lever);
    GS_LOAD(players_timer);
    GS_LOAD(Lever_Store);
    GS_LOAD(Return_CP_No);
    GS_LOAD(Return_CP_Index);
    GS_LOAD(Return_Pattern_Index);
    GS_LOAD(Lever_LR);
    GS_LOAD(Last_Eftype);
    GS_LOAD(DENJIN_No);
    GS_LOAD(SC_Personal_Time);
    GS_LOAD(Guard_Counter);
    GS_LOAD(Limit_Time);
    GS_LOAD(Last_Pattern_Index);
    GS_LOAD(Random_ix16_ex);
    GS_LOAD(Random_ix32_ex);
    GS_LOAD(DE_X);
    GS_LOAD(Exit_Timer);
    GS_LOAD(Max_vitality);
    GS_LOAD(Bonus_Game_Flag);
    GS_LOAD(Bonus_Game_Work);
    GS_LOAD(Bonus_Game_result);
    GS_LOAD(Stock_Bonus_Game_Result);
    GS_LOAD(bs_scrrrl);
    GS_LOAD(Bonus_Stage_RNO);
    GS_LOAD(Bonus_Stage_Level);
    GS_LOAD(Bonus_Stage_Tix);
    GS_LOAD(Bonus_Game_ex_result);
    GS_LOAD(Stock_Com_Color);
    GS_LOAD(bs2_floor);
    GS_LOAD(bs2_hosei);
    GS_LOAD(bs2_current_damage);
    GS_LOAD(Win_Record);
    GS_LOAD(Stock_Win_Record);
    GS_LOAD(WGJ_Win);
    GS_LOAD(Target_BG_X);
    GS_LOAD(Offset_BG_X);
    GS_LOAD(Result_Timer);
    GS_LOAD(scrl);
    GS_LOAD(scrr);
    GS_LOAD(vital_stop_flag);
    GS_LOAD(gauge_stop_flag);
    GS_LOAD(Lamp_Timer);
    GS_LOAD(Cont_Timer);
    GS_LOAD(Plate_X);
    GS_LOAD(Plate_Y);
    GS_LOAD(Demo_Timer);
    GS_LOAD(Condense_Buff);
    GS_LOAD(Demo_Ptr);
    GS_LOAD(Keep_Grade);
    GS_LOAD(IO_Result);
    GS_LOAD(VS_Win_Record);
    GS_LOAD(PLsw);
    GS_LOAD(plsw_00);
    GS_LOAD(plsw_01);
    GS_LOAD(Flash_Synchro);
    GS_LOAD(Synchro_Level);
    GS_LOAD(Random_ix16_com);
    GS_LOAD(Random_ix32_com);
    GS_LOAD(Random_ix16_ex_com);
    GS_LOAD(Random_ix32_ex_com);
    GS_LOAD(Opening_Now);
    GS_LOAD(task);

    // plcnt

    GS_LOAD(plw);
    GS_LOAD(combo_type);
    GS_LOAD(remake_power);
    GS_LOAD(zanzou_table);
    GS_LOAD(super_arts);
    GS_LOAD(piyori_type);
    GS_LOAD(appear_type);
    GS_LOAD(pcon_rno);
    GS_LOAD(round_slow_flag);
    GS_LOAD(pcon_dp_flag);
    GS_LOAD(win_sp_flag);
    GS_LOAD(dead_voice_flag);
    GS_LOAD(rambod);
    GS_LOAD(ramhan);
    GS_LOAD(vital_inc_timer);
    GS_LOAD(vital_dec_timer);
    GS_LOAD(sag_inc_timer);

    // cmd_data

    GS_LOAD(wcp);
    GS_LOAD(t_pl_lvr);
    GS_LOAD(waza_work);

    // cmb_win

    GS_LOAD(cmst_buff);
    GS_LOAD(old_cmb_flag);
    GS_LOAD(cmb_stock);
    GS_LOAD(first_attack);
    GS_LOAD(rever_attack);
    GS_LOAD(paring_attack);
    GS_LOAD(bonus_pts);
    GS_LOAD(hit_num);
    GS_LOAD(sa_kind);
    GS_LOAD(chainex_check);
    GS_LOAD(end_flag);
    GS_LOAD(calc_hit);
    GS_LOAD(score_calc);
    GS_LOAD(cmb_all_stock);
    GS_LOAD(sarts_finish_flag);
    GS_LOAD(last_hit_time);
    GS_LOAD(cmb_calc_now);
    GS_LOAD(cst_read);
    GS_LOAD(cst_write);

    // bg

    GS_LOAD(bg_w);

    // charset

    GS_LOAD(att_req);

    // slowf

    GS_LOAD(SLOW_timer);
    GS_LOAD(SLOW_flag);
    GS_LOAD(EXE_flag);

    // grade

    GS_LOAD(judge_gals);
    GS_LOAD(judge_com);
    GS_LOAD(last_judge_dada);
    GS_LOAD(judge_final);
    GS_LOAD(judge_item);
    GS_LOAD(ji_sat);

    // spgauge

    GS_LOAD(Old_Stop_SG);
    GS_LOAD(Exec_Wipe_F);
    GS_LOAD(time_clear);
    GS_LOAD(spg_number);
    GS_LOAD(spg_work);
    GS_LOAD(spg_offset);
    GS_LOAD(time_num);
    GS_LOAD(time_timer);
    GS_LOAD(time_flag);
    GS_LOAD(col);
    GS_LOAD(time_operate);
    GS_LOAD(sast_now);
    GS_LOAD(max2);
    GS_LOAD(max_rno2);
    GS_LOAD(spg_dat);

    // stun

    GS_LOAD(sdat);

    // vital

    GS_LOAD(vit);

    // win_pl

    GS_LOAD(win_free);
    GS_LOAD(win_rno);
    GS_LOAD(poison_flag);

    // ta_sub

    GS_LOAD(eff_hit_flag);

    // sc_sub

    GS_LOAD(FadeLimit);
    GS_LOAD(WipeLimit);

    // appear

    GS_LOAD(Appear_car_stop);
    GS_LOAD(Appear_hv);
    GS_LOAD(Appear_free);
    GS_LOAD(Appear_flag);
    GS_LOAD(app_counter);
    GS_LOAD(appear_work);
    GS_LOAD(Appear_end);

    // bg_data

    GS_LOAD(y_sitei_pos);
    GS_LOAD(y_sitei_flag);
    GS_LOAD(c_number);
    GS_LOAD(c_kakikae);
    GS_LOAD(g_number);
    GS_LOAD(g_kakikae);
    GS_LOAD(nosekae);
    GS_LOAD(scrn_adgjust_y);
    GS_LOAD(scrn_adgjust_x);
    GS_LOAD(zoom_add);
    GS_LOAD(ls_cnt1);
    GS_LOAD(bg_app);
    GS_LOAD(sa_pa_flag);
    GS_LOAD(aku_flag);
    GS_LOAD(seraph_flag);
    GS_LOAD(akebono_flag);
    GS_LOAD(bg_mvxy);
    GS_LOAD(chase_time_y);
    GS_LOAD(chase_time_x);
    GS_LOAD(chase_y);
    GS_LOAD(chase_x);
    GS_LOAD(demo_car_flag);
    GS_LOAD(ideal_w);
    GS_LOAD(bg_app_stop);
    GS_LOAD(bg_stop);
    GS_LOAD(base_y_pos);
    GS_LOAD(etcBgPalCnvTable);
    GS_LOAD(etcBgGixCnvTable);

    // eff56

    GS_LOAD(ci_pointer);
    GS_LOAD(ci_col);
    GS_LOAD(ci_timer);

    // effb2

    GS_LOAD(rf_b2_flag);
    GS_LOAD(b2_curr_no);

    // effb8

    GS_LOAD(test_pl_no);
    GS_LOAD(test_mes_no);
    GS_LOAD(test_in);
    GS_LOAD(old_mes_no2);
    GS_LOAD(old_mes_no3);
    GS_LOAD(old_mes_no_pl);
    GS_LOAD(mes_timer);
}
