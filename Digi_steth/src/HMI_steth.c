
/*
 ============================================================================
 Name        : HMI.c
 Author      : SKN
 Created on  : Jan 10, 2014
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */


#include "main_inf.h"

char first_line[] = "Digital Stethoscope " ;
char second_line[] = "        -: Sri " ;

static uint8 welcome_disp_dur = 0 ;
static uint8 Welcome_display_st = FALSE;
static uint8 u8currentScreen ;
static uint8 u8userButton ;
static uint8 disp_itr_cnt = 0 ;

static uint8 x = 1;

void HMI_Proc(void)
{
  static uint8 Init_Delay_Cnt = 0 ;
  static uint8 Init_Delay_Done= 0 ;
  
  data_type auto_flush_st         ;
  data_type man_flush_st          ;
  data_type Mains_swt_st          ;
//  data_type Run_Mode_sel_st       ;
  static uint8 scroll_idx = 0     ;
  uint8 Dispense_st               ;
  uint8 manual_on_off_swt_st      ;
  uint8 Pump_on_off_swt_st        ;
  uint8 manual_pmp_on = 0         ;
  uint8 disp_req_key = 0          ;
  uint8 flush_run_st = 0          ;

  if ( Init_Delay_Done == 0 )
    Init_Delay_Cnt++ ;
  
  if (Init_Delay_Cnt >= 3) 
  { 
      Init_Delay_Done = 1 ;
	Mains_swt_st = Get_Mains_swt_st() ;

	if (Mains_swt_st == MAINS_OFF)
	{
//LCD		puts("______________\nMains OFF\n______________\n");
	}
	else if (Mains_swt_st == MAINS_ON)
	{
//LCD		puts("______________\nMains ON\n______________\n");
          
          
          if (Welcome_display_st == FALSE)
          {
              if (!welcome_disp_dur)
//            if(scroll_idx<( (strlen(first_line)>strlen(second_line))? strlen(first_line): strlen(second_line) ))
              {
                scroll_idx++;
                Display_Screen(WELCOME_SCREEN);
              } 
              else
              {
                Welcome_display_st = TRUE ;
                u8currentScreen = VENDING_SCREEN ;
                u8userButton  = NOT_PRESSED;
              }
          }
          
            u8currentScreen = HOME_SCREEN ;
            u8userButton = NOT_PRESSED ;
            Display_Screen(u8currentScreen) ;
          
	}
  }
}

/*------------------------------------------------------------------*/

//void Display_Screen(uint8 Screen, uint8 Button)
void Display_Screen(uint8 Screen)
{
  static uint8 Screen_old = NO_SCREEN ;
  static char buff[16];
  static uint8 auto_on_off_st_old = 0x7F ;
  static uint8 flush_on_off_st_old = 0x7F ;
  static uint8 dispenser_swt_st_old = 0x7F ;
  static uint8 system_health_st_old = 0x7F ;
  static uint8 value_set_old = 0x7F ;
  static uint8 unit_select_old = 0x7F ;
  static uint8 print_idx = 0 ;
  uint16 dispensing_ltrs;
  uint16 remaining_ltrs;
  uint16 input_value ;
  uint8 auto_on_off_st;
  uint8 flush_on_off_st;
  uint8 manual_on_off_st;
  uint8 dispenser_swt_st ;
  uint8 system_health_st ;
  uint8 i, num_len ;
  uint8 sel_char ;
  uint8 Pump_swt ;
  uint8 disp_req_key    ;
  
  switch(Screen)
  {
  case WELCOME_SCREEN:
    {    
    /*----------------------------------------------------------------------------*/
    
    //          if(Screen_old != Screen)               // to prevent multiple print of the same screen

      if (!welcome_disp_dur)
      {
        Clear_LCD() ;                  // Clear Display
        
        print_on_LCD(first_line) ;
        goto_cursor(LCD_START_OF_2LINE);            // goto second row
        print_on_LCD(second_line) ;
        
        welcome_disp_dur++ ;
      }          
      for(i=0;i<( (strlen(first_line)>strlen(second_line))? strlen(first_line): strlen(second_line) ); i++)
        //          else
      {
        Delay(600); 
        Scroll_LCD_Left() ;
      }
      /*
      Clear_LCD() ;                  // Clear Display
      print_on_LCD("      BPM  ") ;
      Delay(3000); 
      */
    }
    break ;
    
  case HOME_SCREEN :
/********    
    for (i=0; i<16; i++)
    {
      if(i != 15)
      {
        wave_first_line[i] = wave_first_line[i+1] ;
        wave_first_line[i+1] = '_' ;
      }
      else
        wave_first_line[i] = '_' ;
    }
    
    //  if(bpm != bpm_old)
    {
      Clear_LCD() ;
      //          sprintf(wave_first_line, " Press BPM Measure Start " );
      print_on_LCD(wave_first_line) ;
      goto_cursor(LCD_START_OF_2LINE);            // goto second row
      
      sprintf(BPM_second_line, "Beats:%d BPM:%d ", beats, bpm );
      print_on_LCD(BPM_second_line) ;
    }
    
    //  bpm_old = bpm ;
    
    //----------------------------------------------------------------------------
    
              
//    for(i=0;i<( (strlen(wave_first_line)>strlen(BPM_second_line))? strlen(wave_first_line): strlen(BPM_second_line) ); i++)
//    {
//    Delay(600); 
//    Scroll_LCD_Left();
//  }          
    
    
//    if(beats)
    {
      
      //            bpm = beats * (60000 / DT)  ;
      
      //            bpm+= beats *  60 ;
      //            bpm/= 2 ;
      
      bpm = beats * 60 ;          
      beats = 0 ;
    }
*******/    
    break ;
    
    
    
  default:
    break ;
  }
  Screen_old = Screen ;
  
}
/*------------------------------------------------------------------*/

uint16 Get_Steth_Analog(void)
{
  static uint16 adc_val_max = 0 ;
  uint16 adc_val ;
  float Beat_val ;
  
  adc_val   = ADC1_Get(MemFilter_Ds_I_A) ;        
  Beat_val  = ADC_Beat_Conv(adc_val, MEMFILTER_DS) ;
  
  if (adc_val_max < adc_val )
    adc_val_max = adc_val ;
  
//  if(Beat_val>3)
  if(adc_val>8000)
  {
    beats++ ;
    wave_first_line[15] = '^' ;
  }
  
  return adc_val_max ; // adc_val ; // Beat_val ;
}

/*----------------------------------------------------*/

#define		PRES_SENS_MIN_SCALE_CURR_MA		4
#define		PRES_SENS_MAX_SCALE_CURR_MA		20
#define		PRES_SENS_SHUNT_RES_OHM			120
#define		ADC_MAX_14_BIT                          0x3FFF
#define         VOTL_MILI_VOLT_CONV_FACTOR              (1/1000)
#define         PRESSURE_SENS_MAX_BAR                   5

#define		PRES_SENS_MIN_SCALE_VOLT_MV		( PRES_SENS_MIN_SCALE_CURR_MA * PRES_SENS_SHUNT_RES_OHM )
#define		PRES_SENS_MAX_SCALE_VOLT_MV		( PRES_SENS_MAX_SCALE_CURR_MA * PRES_SENS_SHUNT_RES_OHM )

#define		ADC_PRES_RATIOMETRIC_FACTOR		(PRES_SENS_MAX_SCALE_VOLT_MV-PRES_SENS_MIN_SCALE_VOLT_MV)/(ADC_MAX_14_BIT)
#define		PRES_SENS_RATIOMETRIC_FACTOR		(PRES_SENS_MAX_SCALE_VOLT_MV-PRES_SENS_MIN_SCALE_VOLT_MV)/(PRESSURE_SENS_MAX_BAR)
#define		PRES_SENS_ERROR_FACTOR			PRES_SENS_MIN_SCALE_VOLT_MV

float ADC_Beat_Conv(uint16 adc_val, uint8 pres_sens_idx)
{
	float pres_val ;
        float adc_mv ;

	adc_mv =  (adc_val * ADC_PRES_RATIOMETRIC_FACTOR) + PRES_SENS_ERROR_FACTOR ;
        //pres_val = pres_val * VOTL_MILI_VOLT_CONV_FACTOR ;    //mV conversion
        
        pres_val = (adc_mv - PRES_SENS_ERROR_FACTOR) / (PRES_SENS_RATIOMETRIC_FACTOR) ;
          
	return pres_val ;

}

/*----------------------------------------------------*/
