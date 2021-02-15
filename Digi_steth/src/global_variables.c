
/*
 ============================================================================
 Name        : global_variables.c
 Author      : SKN
 Version     :
 Copyright   :
 Description : 
 ============================================================================
 */

#include "global_variables.h"
#include "main_inf.h"

sint16 DT_TIME = 0;

Customer_struct Customers[MAX_SAVED_CUSTOMERS]  ;

Srvc_CurveS16_t s16_Vol_CUR =
 {
    12,                                         	                //  No. of elements in curve
    0, 20, 40, 60, 100, 140, 200, 260, 320, 400, 450, 500,  		//  X elements	in litres
    0, 10, 20, 30, 50, 70, 100, 130, 160, 200, 225, 250   	        //  Y elements	in Rs
};

Srvc_CurveS16_t s16_Price_CUR =
 {
    12,                                         	                //  No. of elements in curve
    0, 10, 20, 30, 50, 70, 100, 130, 160, 200, 225, 250,                //  X elements	in Rs
    0, 20, 40, 60, 100, 140, 200, 260, 320, 400, 450, 500	  	//  Y elements	in litres
};

Srvc_CurveS16_t s16_mv_pres_CUR =
 {
    12,                                         	                //  No. of elements in curve
    0, 200, 400, 600, 1000, 1400, 2000, 2600, 3200, 4000, 4500, 5000,   //  X elements	in Rs
    0, 2, 40, 60, 100, 140, 200, 260, 320, 400, 450, 500	  	//  Y elements	in litres
};

char *System_Diag_Msgs[] = {
		"Pre Filter clogged" ,
		"Membrane filter clogged" ,
		"No water flow" ,
		"Filter healthy" ,
		"Water level low"
};

uint16 ADC_ConvData[ADC_CHANNELMAX+1];

uint32 FS_Pulse_Cnt_curr ;
uint32 FS_Pulse_Cnt_Total ;


uint8 u8currentScreen ;
uint8 u8userButton ;
uint16 value_set ;
uint8 unit_select;
uint8 value_select;

uint8 up_key;
uint8 down_key;

Customer_struct Customer_read ;
user_intput_st	user_intput ;

char str[16];
float flow_sensor_vol = 0.0;
uint8 Vending_Enable_st = DISABLE ;
float Flow_vol_Disp = 0.0;
float Total_Dispensed_Volume = 0;
uint8 pumper_buff[25]  = {'\0'}   ;

uint8 print_buff[50] ;
uint16 bpm ;
uint16 beats ;

char wave_first_line[16] = "________________" ;
char BPM_second_line[16] = { '\0' } ;

/*----------------------------------------------------------*/
