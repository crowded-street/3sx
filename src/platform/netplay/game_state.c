/**
 * @file game_state.c
 * Saving the rollback state. The restore side is in game_state_load.c.
 */

#include "platform/netplay/game_state_internal.h"

#define GS_SAVE(member)                                                                                                \
    GS_ASSERT_SAME_SIZE(member);                                                                                       \
    SDL_memcpy(&dst->member, &member, sizeof(member))

void GameState_Save(GameState* dst) {
    GS_SAVE(Scene_Cut);
    GS_SAVE(Time_Over);
    GS_SAVE(round_timer);
    GS_SAVE(flash_timer);
    GS_SAVE(flash_r_num);
    GS_SAVE(flash_col);
    GS_SAVE(math_counter_hi);
    GS_SAVE(math_counter_low);
    GS_SAVE(counter_color);
    GS_SAVE(mugen_flag);
    GS_SAVE(hoji_counter);
    GS_SAVE(Order);
    GS_SAVE(Order_Timer);
    GS_SAVE(Order_Dir);
    GS_SAVE(Score);
    GS_SAVE(Complete_Bonus);
    GS_SAVE(Stock_Score);
    GS_SAVE(Vital_Bonus);
    GS_SAVE(Time_Bonus);
    GS_SAVE(Stage_Stock_Score);
    GS_SAVE(Bonus_Score);
    GS_SAVE(Final_Bonus_Score);
    GS_SAVE(WGJ_Score);
    GS_SAVE(Bonus_Score_Plus);
    GS_SAVE(Perfect_Bonus);
    GS_SAVE(Keep_Score);
    GS_SAVE(Disp_Score_Buff);
    GS_SAVE(Winner_id);
    GS_SAVE(Loser_id);
    GS_SAVE(Break_Into);
    GS_SAVE(My_char);
    GS_SAVE(Allow_a_battle_f);
    GS_SAVE(Round_num);
    GS_SAVE(Complete_Judgement);
    GS_SAVE(Fade_Flag);
    GS_SAVE(Super_Arts);
    GS_SAVE(Forbid_Break);
    GS_SAVE(Request_Break);
    GS_SAVE(Continue_Count);
    GS_SAVE(Counter_hi);
    GS_SAVE(Counter_low);
    GS_SAVE(Unit_Of_Timer);
    GS_SAVE(Select_Timer);
    GS_SAVE(Cursor_X);
    GS_SAVE(Cursor_Y);
    GS_SAVE(Cursor_Y_Pos);
    GS_SAVE(Cursor_Timer);
    GS_SAVE(Time_Stop);
    GS_SAVE(Suicide);
    GS_SAVE(Complete_Face);
    GS_SAVE(Play_Type);
    GS_SAVE(Sel_PL_Complete);
    GS_SAVE(New_Challenger);
    GS_SAVE(S_No);
    GS_SAVE(Select_Start);
    GS_SAVE(request_message);
    GS_SAVE(judge_flag);
    GS_SAVE(WINNER);
    GS_SAVE(LOSER);
    GS_SAVE(Champion);
    GS_SAVE(Fade_Half_Flag);
    GS_SAVE(Reserve_Cut);
    GS_SAVE(Perfect_Flag);
    GS_SAVE(Next_Step);
    GS_SAVE(Switch_Type);
    GS_SAVE(Cover_Timer);
    GS_SAVE(Personal_Timer);
    GS_SAVE(Request_E_No);
    GS_SAVE(Request_G_No);
    GS_SAVE(Present_Rank);
    GS_SAVE(Best_Grade);
    GS_SAVE(Demo_Type);
    GS_SAVE(Rank_Type);
    GS_SAVE(Flash_Sign);
    GS_SAVE(Flash_Rank_Time);
    GS_SAVE(Flash_Rank_Interval);
    GS_SAVE(Ranking_X);
    GS_SAVE(Rank);
    GS_SAVE(Rank_X);
    GS_SAVE(E_07_Flag);
    GS_SAVE(Complete_Victory);
    GS_SAVE(Demo_Flag);
    GS_SAVE(Next_Demo);
    GS_SAVE(Demo_PL_Index);
    GS_SAVE(Demo_Stage_Index);
    GS_SAVE(Face_MV_Request);
    GS_SAVE(Face_Move);
    GS_SAVE(Player_id);
    GS_SAVE(Last_Player_id);
    GS_SAVE(Player_Number);
    GS_SAVE(DENJIN_Term);
    GS_SAVE(Rapid_No);
    GS_SAVE(COM_id);
    GS_SAVE(EM_id);
    GS_SAVE(Select_Status);
    GS_SAVE(Select_Demo_Index);
    GS_SAVE(Country);
    GS_SAVE(Demo_Time_Stop);
    GS_SAVE(Combo_Speed);
    GS_SAVE(Exec_Wipe);
    GS_SAVE(Passive_Mode);
    GS_SAVE(Passive_Flag);
    GS_SAVE(Flip_Flag);
    GS_SAVE(Lie_Flag);
    GS_SAVE(Counter_Attack);
    GS_SAVE(Attack_Flag);
    GS_SAVE(Limited_Flag);
    GS_SAVE(Shell_Ignore_Timer);
    GS_SAVE(Event_Judge_Gals);
    GS_SAVE(EJG_index);
    GS_SAVE(Guard_Flag);
    GS_SAVE(Pierce_Menu);
    GS_SAVE(Face_MV_Time);
    GS_SAVE(Before_Jump);
    GS_SAVE(Stop_Combo);
    GS_SAVE(Stock_Hit_Flag);
    GS_SAVE(Rolling_Flag);
    GS_SAVE(Continue_Coin);
    GS_SAVE(Ignore_Entry);
    GS_SAVE(Slide_Type);
    GS_SAVE(Moving_Plate);
    GS_SAVE(Naming_Cut);
    GS_SAVE(Moving_Plate_Counter);
    GS_SAVE(Player_Color);
    GS_SAVE(PP_Priority);
    GS_SAVE(OK_Priority);
    GS_SAVE(Stock_My_char);
    GS_SAVE(Stock_Player_Color);
    GS_SAVE(Music_Fade);
    GS_SAVE(Stop_SG);
    GS_SAVE(Operator_Status);
    GS_SAVE(Round_Operator);
    GS_SAVE(another_bg);
    GS_SAVE(Last_Super_Arts);
    GS_SAVE(Last_My_char);
    GS_SAVE(Continue_Menu);
    GS_SAVE(Timer_Freeze);
    GS_SAVE(Type_of_Attack);
    GS_SAVE(Standing_Timer);
    GS_SAVE(Before_Look);
    GS_SAVE(Attack_Count_No0);
    GS_SAVE(Standing_Master_Timer);
    GS_SAVE(PB_Music_Off);
    GS_SAVE(No_Death);
    GS_SAVE(Flash_MT);
    GS_SAVE(Squat_Timer);
    GS_SAVE(Squat_Master_Timer);
    GS_SAVE(Turn_Over);
    GS_SAVE(Turn_Over_Timer);
    GS_SAVE(Jump_Pass_Timer);
    GS_SAVE(sa_gauge_flash);
    GS_SAVE(Receive_Flag);
    GS_SAVE(Disposal_Again);
    GS_SAVE(BGM_Vol);
    GS_SAVE(Used_char);
    GS_SAVE(Break_Com);
    GS_SAVE(aiuchi_flag);
    GS_SAVE(paring_counter);
    GS_SAVE(paring_bonus_r);
    GS_SAVE(paring_ctr_vs);
    GS_SAVE(paring_ctr_ori);
    GS_SAVE(Attack_Count_Buff);
    GS_SAVE(Attack_Count_Index);
    GS_SAVE(CC_Value);
    GS_SAVE(Continue_Coin2);
    GS_SAVE(Weak_PL);
    GS_SAVE(Bullet_No);
    GS_SAVE(Bullet_Counter);
    GS_SAVE(Final_Result_id);
    GS_SAVE(Disp_Win_Name);
    GS_SAVE(Perfect_Counter);
    GS_SAVE(Straight_Counter);
    GS_SAVE(Appear_Q);
    GS_SAVE(Cut_Scroll);
    GS_SAVE(Break_Into_CPU);
    GS_SAVE(ID_of_Face);
    GS_SAVE(Cursor_Move);
    GS_SAVE(Auto_Cursor);
    GS_SAVE(Auto_No);
    GS_SAVE(Auto_Index);
    GS_SAVE(Auto_Timer);
    GS_SAVE(Explosion);
    GS_SAVE(Introduce_Break_Into);
    GS_SAVE(gouki_wins);
    GS_SAVE(EM_Rank);
    GS_SAVE(Disp_PERFECT);
    GS_SAVE(Escape_SS);
    GS_SAVE(Deley_Shot_No);
    GS_SAVE(Deley_Shot_Timer);
    GS_SAVE(Lost_Round);
    GS_SAVE(Super_Arts_Finish);
    GS_SAVE(Stage_SA_Finish);
    GS_SAVE(Perfect_Finish);
    GS_SAVE(Cheap_Finish);
    GS_SAVE(Last_My_char2);
    GS_SAVE(gouki_app);
    GS_SAVE(Bonus_Game_Complete);
    GS_SAVE(Get_Demo_Index);
    GS_SAVE(Combo_Demo_Flag);
    GS_SAVE(Stage_Continue);
    GS_SAVE(Pause_Hit_Marks);
    GS_SAVE(Extra_Break);
    GS_SAVE(Shin_Gouki_BGM);
    GS_SAVE(Stage_Lost_Round);
    GS_SAVE(Stage_Perfect_Finish);
    GS_SAVE(Stage_Cheap_Finish);
    GS_SAVE(EXE_obroll);
    GS_SAVE(End_PL);
    GS_SAVE(Stock_Com_Arts);
    GS_SAVE(PB_Status);
    GS_SAVE(Flip_Counter);
    GS_SAVE(Stage_Time_Finish);
    GS_SAVE(Bonus_Type);
    GS_SAVE(Completion_Bonus);
    GS_SAVE(ichikannkei);
    GS_SAVE(Plate_Disposal_No);
    GS_SAVE(SO_No);
    GS_SAVE(Disp_Command_Name);
    GS_SAVE(OK_Appear79);
    GS_SAVE(Extra_Counter);
    GS_SAVE(SC_No);
    GS_SAVE(BGM_No);
    GS_SAVE(BGM_Timer);
    GS_SAVE(EM_List);
    GS_SAVE(Sel_EM_Complete);
    GS_SAVE(Temporary_EM);
    GS_SAVE(OK_Moving_SA_Plate);
    GS_SAVE(Battle_Q);
    GS_SAVE(EM_History);
    GS_SAVE(GO_No);
    GS_SAVE(Aborigine);
    GS_SAVE(Continue_Count_Down);
    GS_SAVE(WGJ_Target);
    GS_SAVE(EM_Candidate);
    GS_SAVE(Last_Selected_EM);
    GS_SAVE(Q_Country);
    GS_SAVE(Continue_Cut);
    GS_SAVE(Introduce_Boss);
    GS_SAVE(Final_Play_Type);
    GS_SAVE(Rank_In);
    GS_SAVE(Request_Disp_Rank);
    GS_SAVE(Reset_Timer);
    GS_SAVE(bbbs_type);
    GS_SAVE(Straight_Flag);
    GS_SAVE(kakushi_ix);
    GS_SAVE(kakushi_op);
    GS_SAVE(RO_backup);
    GS_SAVE(PT_backup);
    GS_SAVE(E_Number);
    GS_SAVE(E_No);
    GS_SAVE(C_No);
    GS_SAVE(G_No);
    GS_SAVE(D_No);
    GS_SAVE(M_No);
    GS_SAVE(Exit_No);
    GS_SAVE(SP_No);
    GS_SAVE(Face_No);
    GS_SAVE(Stop_Cursor);
    GS_SAVE(Training_Index);
    GS_SAVE(Connect_Status);
    GS_SAVE(Menu_Suicide);
    GS_SAVE(Game_pause);
    GS_SAVE(Game_difficulty);
    GS_SAVE(Pause);
    GS_SAVE(Pause_ID);
    GS_SAVE(Exit_Menu);
    GS_SAVE(Conclusion_Flag);
    GS_SAVE(CP_No);
    GS_SAVE(CP_Index);
    GS_SAVE(Gap_Timer);
    GS_SAVE(Message_Suicide);
    GS_SAVE(Disp_Cockpit);
    GS_SAVE(Select_Arts);
    GS_SAVE(Lamp_No);
    GS_SAVE(Lamp_Index);
    GS_SAVE(Lamp_Color);
    GS_SAVE(Stop_Update_Score);
    GS_SAVE(test_flag);
    GS_SAVE(ixbfw_cut);
    GS_SAVE(Cont_No);
    GS_SAVE(PL_Wins);
    GS_SAVE(Fade_R_No0);
    GS_SAVE(Fade_R_No1);
    GS_SAVE(Conclusion_Type);
    GS_SAVE(win_type);
    GS_SAVE(message_index);
    GS_SAVE(F_No0);
    GS_SAVE(F_No1);
    GS_SAVE(F_No2);
    GS_SAVE(F_No3);
    GS_SAVE(keep_condition);
    GS_SAVE(Check_Buff);
    GS_SAVE(Convert_Buff);
    GS_SAVE(Unsubstantial_BG);
    GS_SAVE(Menu_Cursor_X);
    GS_SAVE(Menu_Cursor_Y);
    GS_SAVE(Replay_Status);
    GS_SAVE(Disappear_LOGO);
    GS_SAVE(count_end);
    GS_SAVE(Play_Game);
    GS_SAVE(Menu_Cursor_Move);
    GS_SAVE(flash_win_type);
    GS_SAVE(sync_win_type);
    GS_SAVE(Mode_Type);
    GS_SAVE(Menu_Page);
    GS_SAVE(Menu_Max);
    GS_SAVE(reset_NG_flag);
    GS_SAVE(VS_Stage);
    GS_SAVE(Present_Mode);
    GS_SAVE(Play_Mode);
    GS_SAVE(Page_Max);
    GS_SAVE(Direction_Working);
    GS_SAVE(Vital_Handicap);
    GS_SAVE(Cursor_Limit);
    GS_SAVE(Synchro_No);
    GS_SAVE(SA_shadow_on);
    GS_SAVE(Pause_Down);
    GS_SAVE(Training_ID);
    GS_SAVE(Disp_Attack_Data);
    GS_SAVE(Record_Data_Tr);
    GS_SAVE(End_Training);
    GS_SAVE(Menu_Page_Buff);
    GS_SAVE(Reset_Bootrom);
    GS_SAVE(Decide_ID);
    GS_SAVE(Training_Cursor);
    GS_SAVE(Lag_Timer);
    GS_SAVE(CPU_Time_Lag);
    GS_SAVE(Forbid_Reset);
    GS_SAVE(CPU_Rec);
    GS_SAVE(Pause_Type);
    GS_SAVE(Game_timer);
    GS_SAVE(Control_Time);
    GS_SAVE(Time_in_Time);
    GS_SAVE(Round_Level);
    GS_SAVE(Round_Result);
    GS_SAVE(Fade_Number);
    GS_SAVE(G_Timer);
    GS_SAVE(D_Timer);
    GS_SAVE(Rank_Pos_X);
    GS_SAVE(Rank_Pos_Y);
    GS_SAVE(E_Timer);
    GS_SAVE(F_Timer);
    GS_SAVE(ENTRY_X);
    GS_SAVE(C_Timer);
    GS_SAVE(S_Timer);
    GS_SAVE(Flash_Complete);
    GS_SAVE(Sel_Arts_Complete);
    GS_SAVE(Arts_Y);
    GS_SAVE(Move_Super_Arts);
    GS_SAVE(Battle_Country);
    GS_SAVE(Face_Status);
    GS_SAVE(ID);
    GS_SAVE(ID2);
    GS_SAVE(mes_already);
    GS_SAVE(Timer_00);
    GS_SAVE(Timer_01);
    GS_SAVE(PL_Distance);
    GS_SAVE(Area_Number);
    GS_SAVE(Lever_Buff);
    GS_SAVE(Lever_Pool);
    GS_SAVE(Tech_Index);
    GS_SAVE(Random_ix16);
    GS_SAVE(Random_ix32);
    GS_SAVE(M_Timer);
    GS_SAVE(VS_Tech);
    GS_SAVE(Guard_Type);
    GS_SAVE(Separate_Area);
    GS_SAVE(Free_Lever);
    GS_SAVE(Term_No);
    GS_SAVE(Com_Width_Data);
    GS_SAVE(Lever_Squat);
    GS_SAVE(M_Lv);
    GS_SAVE(Insert_Y);
    GS_SAVE(scr_req_x);
    GS_SAVE(scr_req_y);
    GS_SAVE(zoom_req_flag_old);
    GS_SAVE(zoom_request_flag);
    GS_SAVE(zoom_request_level);
    GS_SAVE(Last_Selected_ID);
    GS_SAVE(Last_Called_SE);
    GS_SAVE(VS_Index);
    GS_SAVE(Rapid_Index);
    GS_SAVE(Shell_Separate_Area);
    GS_SAVE(Attack_Counter);
    GS_SAVE(Last_Attack_Counter);
    GS_SAVE(Pattern_Index);
    GS_SAVE(Com_Color_Shot);
    GS_SAVE(Resume_Lever);
    GS_SAVE(players_timer);
    GS_SAVE(Lever_Store);
    GS_SAVE(Return_CP_No);
    GS_SAVE(Return_CP_Index);
    GS_SAVE(Return_Pattern_Index);
    GS_SAVE(Lever_LR);
    GS_SAVE(Last_Eftype);
    GS_SAVE(DENJIN_No);
    GS_SAVE(SC_Personal_Time);
    GS_SAVE(Guard_Counter);
    GS_SAVE(Limit_Time);
    GS_SAVE(Last_Pattern_Index);
    GS_SAVE(Random_ix16_ex);
    GS_SAVE(Random_ix32_ex);
    GS_SAVE(DE_X);
    GS_SAVE(Exit_Timer);
    GS_SAVE(Max_vitality);
    GS_SAVE(Bonus_Game_Flag);
    GS_SAVE(Bonus_Game_Work);
    GS_SAVE(Bonus_Game_result);
    GS_SAVE(Stock_Bonus_Game_Result);
    GS_SAVE(bs_scrrrl);
    GS_SAVE(Bonus_Stage_RNO);
    GS_SAVE(Bonus_Stage_Level);
    GS_SAVE(Bonus_Stage_Tix);
    GS_SAVE(Bonus_Game_ex_result);
    GS_SAVE(Stock_Com_Color);
    GS_SAVE(bs2_floor);
    GS_SAVE(bs2_hosei);
    GS_SAVE(bs2_current_damage);
    GS_SAVE(Win_Record);
    GS_SAVE(Stock_Win_Record);
    GS_SAVE(WGJ_Win);
    GS_SAVE(Target_BG_X);
    GS_SAVE(Offset_BG_X);
    GS_SAVE(Result_Timer);
    GS_SAVE(scrl);
    GS_SAVE(scrr);
    GS_SAVE(vital_stop_flag);
    GS_SAVE(gauge_stop_flag);
    GS_SAVE(Lamp_Timer);
    GS_SAVE(Cont_Timer);
    GS_SAVE(Plate_X);
    GS_SAVE(Plate_Y);
    GS_SAVE(Demo_Timer);
    GS_SAVE(Condense_Buff);
    GS_SAVE(Demo_Ptr);
    GS_SAVE(Keep_Grade);
    GS_SAVE(IO_Result);
    GS_SAVE(VS_Win_Record);
    GS_SAVE(PLsw);
    GS_SAVE(plsw_00);
    GS_SAVE(plsw_01);
    GS_SAVE(Flash_Synchro);
    GS_SAVE(Synchro_Level);
    GS_SAVE(Random_ix16_com);
    GS_SAVE(Random_ix32_com);
    GS_SAVE(Random_ix16_ex_com);
    GS_SAVE(Random_ix32_ex_com);
    // Random_ix16_bg is left out on purpose: it only drives stage flashing, and the
    // state deciding when to draw from it isn't saved.
    GS_SAVE(Opening_Now);
    GS_SAVE(task);

    // plcnt

    GS_SAVE(plw);
    GS_SAVE(combo_type);
    GS_SAVE(remake_power);
    GS_SAVE(zanzou_table);
    GS_SAVE(super_arts);
    GS_SAVE(piyori_type);
    GS_SAVE(appear_type);
    GS_SAVE(pcon_rno);
    GS_SAVE(round_slow_flag);
    GS_SAVE(pcon_dp_flag);
    GS_SAVE(win_sp_flag);
    GS_SAVE(dead_voice_flag);
    GS_SAVE(rambod);
    GS_SAVE(ramhan);
    GS_SAVE(vital_inc_timer);
    GS_SAVE(vital_dec_timer);
    GS_SAVE(sag_inc_timer);

    // cmd_data

    GS_SAVE(wcp);
    GS_SAVE(t_pl_lvr);
    GS_SAVE(waza_work);

    // cmb_win

    GS_SAVE(cmst_buff);
    GS_SAVE(old_cmb_flag);
    GS_SAVE(cmb_stock);
    GS_SAVE(first_attack);
    GS_SAVE(rever_attack);
    GS_SAVE(paring_attack);
    GS_SAVE(bonus_pts);
    GS_SAVE(hit_num);
    GS_SAVE(sa_kind);
    GS_SAVE(chainex_check);
    GS_SAVE(end_flag);
    GS_SAVE(calc_hit);
    GS_SAVE(score_calc);
    GS_SAVE(cmb_all_stock);
    GS_SAVE(sarts_finish_flag);
    GS_SAVE(last_hit_time);
    GS_SAVE(cmb_calc_now);
    GS_SAVE(cst_read);
    GS_SAVE(cst_write);

    // bg

    GS_SAVE(bg_w);

    // charset

    GS_SAVE(att_req);

    // slowf

    GS_SAVE(SLOW_timer);
    GS_SAVE(SLOW_flag);
    GS_SAVE(EXE_flag);

    // grade

    GS_SAVE(judge_gals);
    GS_SAVE(judge_com);
    GS_SAVE(last_judge_dada);
    GS_SAVE(judge_final);
    GS_SAVE(judge_item);
    GS_SAVE(ji_sat);

    // spgauge

    GS_SAVE(Old_Stop_SG);
    GS_SAVE(Exec_Wipe_F);
    GS_SAVE(time_clear);
    GS_SAVE(spg_number);
    GS_SAVE(spg_work);
    GS_SAVE(spg_offset);
    GS_SAVE(time_num);
    GS_SAVE(time_timer);
    GS_SAVE(time_flag);
    GS_SAVE(col);
    GS_SAVE(time_operate);
    GS_SAVE(sast_now);
    GS_SAVE(max2);
    GS_SAVE(max_rno2);
    GS_SAVE(spg_dat);

    // stun

    GS_SAVE(sdat);

    // vital

    GS_SAVE(vit);

    // win_pl

    GS_SAVE(win_free);
    GS_SAVE(win_rno);
    GS_SAVE(poison_flag);

    // ta_sub

    GS_SAVE(eff_hit_flag);

    // sc_sub

    GS_SAVE(FadeLimit);
    GS_SAVE(WipeLimit);

    // appear

    GS_SAVE(Appear_car_stop);
    GS_SAVE(Appear_hv);
    GS_SAVE(Appear_free);
    GS_SAVE(Appear_flag);
    GS_SAVE(app_counter);
    GS_SAVE(appear_work);
    GS_SAVE(Appear_end);

    // bg_data

    GS_SAVE(y_sitei_pos);
    GS_SAVE(y_sitei_flag);
    GS_SAVE(c_number);
    GS_SAVE(c_kakikae);
    GS_SAVE(g_number);
    GS_SAVE(g_kakikae);
    GS_SAVE(nosekae);
    GS_SAVE(scrn_adgjust_y);
    GS_SAVE(scrn_adgjust_x);
    GS_SAVE(zoom_add);
    GS_SAVE(ls_cnt1);
    GS_SAVE(bg_app);
    GS_SAVE(sa_pa_flag);
    GS_SAVE(aku_flag);
    GS_SAVE(seraph_flag);
    GS_SAVE(akebono_flag);
    GS_SAVE(bg_mvxy);
    GS_SAVE(chase_time_y);
    GS_SAVE(chase_time_x);
    GS_SAVE(chase_y);
    GS_SAVE(chase_x);
    GS_SAVE(demo_car_flag);
    GS_SAVE(ideal_w);
    GS_SAVE(bg_app_stop);
    GS_SAVE(bg_stop);
    GS_SAVE(base_y_pos);
    GS_SAVE(etcBgPalCnvTable);
    GS_SAVE(etcBgGixCnvTable);

    // eff56

    GS_SAVE(ci_pointer);
    GS_SAVE(ci_col);
    GS_SAVE(ci_timer);

    // effb2

    GS_SAVE(rf_b2_flag);
    GS_SAVE(b2_curr_no);

    // effb8

    GS_SAVE(test_pl_no);
    GS_SAVE(test_mes_no);
    GS_SAVE(test_in);
    GS_SAVE(old_mes_no2);
    GS_SAVE(old_mes_no3);
    GS_SAVE(old_mes_no_pl);
    GS_SAVE(mes_timer);
}
