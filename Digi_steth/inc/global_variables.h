
/*
 ============================================================================
 Name        : global_variables.h
 Author      : SKN
 Version     :
 Copyright   :
 Description : 
 ============================================================================
 */

#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_

#include "main_inf.h"
#define ADC_CHANNELMAX                    0x05
extern sint16 DT_TIME;

extern Customer_struct Customers[MAX_SAVED_CUSTOMERS]  ;

extern Srvc_CurveS16_t s16_Vol_CUR ;
extern Srvc_CurveS16_t s16_Price_CUR  ;
extern char *System_Diag_Msgs[] ;

extern uint16 ADC_ConvData[ADC_CHANNELMAX+1];
extern uint32 FS_Pulse_Cnt_curr ;
extern uint32 FS_Pulse_Cnt_Total ;

extern uint8 u8currentScreen ;
extern uint8 u8userButton ;
extern uint16 value_set ;
extern uint8 unit_select;
extern uint8 value_select;

extern uint8 up_key;
extern uint8 down_key;

extern Customer_struct Customer_read ;
extern user_intput_st	user_intput ;

extern char str[16];
extern float flow_sensor_vol ;
extern uint8 Vending_Enable_st ;
extern float Flow_vol_Disp ;
extern float Total_Dispensed_Volume ;
extern uint8 pumper_buff[25]     ;

extern uint8 print_buff[50] ;
extern uint16 bpm ;
extern uint16 beats ;

extern char wave_first_line[16] ;
extern char BPM_second_line[16] ;

#endif /* _GLOBAL_VARIABLES_H_ */

