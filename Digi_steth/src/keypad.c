

/****************************************** Inclusions ****************************************/

/*
#include <includes.h>
#include <port.h>
#include <dio.h>
#include "datcol.h"
#include <dsm.h>
#include "gui.h"
*/

#if (SHELL_KEYPAD_EN  > 0)
  #include <shell.h>
#else
  #define Shell_Printf(...)
#endif

#include "main_inf.h"
#include "keypad.h"

/* [QAC-Supress] DSM Errhandler return type */
//PRQA S 3200 EOF
/****************************************** Static Variables ****************************************/

static Srvc_DebounceParam_t      st_KeyPad_tmDebBtn1_ms; ///< Timing parameter for Button 1 debouncing
static Srvc_DebounceParam_t      st_KeyPad_tmDebBtn2_ms; ///< Timing parameter for Button 2 debouncing
static Srvc_DebounceParam_t      st_KeyPad_tmDebBtn3_ms; ///< Timing parameter for Button 3 debouncing
static Srvc_DebounceParam_t      st_KeyPad_tmDebBtn4_ms; ///< Timing parameter for Button 4 debouncing

static Srvc_DebounceParam_t      st_pump_swt_tmDeb_ms; ///< Timing parameter for pump switch debouncing
static Srvc_DebounceParam_t      st_flush_swt_tmDeb_ms; ///< Timing parameter for flush switch debouncing

/*
static Srvc_DebounceParam_t      st_water_lvl_low_swt_tmDeb_ms; ///< Timing parameter for water level low debouncing
static Srvc_DebounceParam_t      st_water_lvl_mid_swt_tmDeb_ms; ///< Timing parameter for water level mid debouncing
static Srvc_DebounceParam_t      st_water_lvl_high_swt_tmDeb_ms; ///< Timing parameter for water level high debouncing

static Srvc_DebounceParam_t      st_prefltr_us_swt_tmDeb_ms; ///< Timing parameter for water level high debouncing
static Srvc_DebounceParam_t      st_prefltr_ds_swt_tmDeb_ms; ///< Timing parameter for water level high debouncing
static Srvc_DebounceParam_t      st_memfltr_ds_swt_tmDeb_ms; ///< Timing parameter for water level high debouncing
*/

static Srvc_SWTmrU32_t           st_KeyPad_TmSinceBtnPrsd;  ///< Timer to track time since last button pressed
static Srvc_SWTmrU32_t           st_KeyPad_TmSinceBtnRlsd;  ///< Timer to track time since last button released

/**
*********************************************************************************************************
* Keypad Initialization process
*********************************************************************************************************
*/

void  KeyPad_ProcIni (void)
{
  // Code_TAG: SRC_KEYPAD_001: Update the button press and release debouncing time
  st_KeyPad_tmDebBtn1_ms.TimeLowHigh =  KEYPAD_DEBTIME_BTNPRESS;
  st_KeyPad_tmDebBtn1_ms.TimeHighLow =  KEYPAD_DEBTIME_BTNRLS;
  
  st_KeyPad_tmDebBtn2_ms.TimeLowHigh =  KEYPAD_DEBTIME_BTNPRESS;
  st_KeyPad_tmDebBtn2_ms.TimeHighLow =  KEYPAD_DEBTIME_BTNRLS;
  
  st_KeyPad_tmDebBtn3_ms.TimeLowHigh =  KEYPAD_DEBTIME_BTNPRESS;
  st_KeyPad_tmDebBtn3_ms.TimeHighLow =  KEYPAD_DEBTIME_BTNRLS;
  
  st_KeyPad_tmDebBtn4_ms.TimeLowHigh =  KEYPAD_DEBTIME_BTNPRESS;
  st_KeyPad_tmDebBtn4_ms.TimeHighLow =  KEYPAD_DEBTIME_BTNRLS;

  st_pump_swt_tmDeb_ms.TimeLowHigh =  KEYPAD_DEBTIME_BTNPRESS;
  st_pump_swt_tmDeb_ms.TimeHighLow =  KEYPAD_DEBTIME_BTNRLS;

  st_flush_swt_tmDeb_ms.TimeLowHigh =  KEYPAD_DEBTIME_BTNPRESS;
  st_flush_swt_tmDeb_ms.TimeHighLow =  KEYPAD_DEBTIME_BTNRLS;
  
}


/**
*********************************************************************************************************
* Keypad process to read the keypad button status.
*
* This process scans the keypad buttons and read the status. The digital input value is read and 
* debounced to filter out any noise if present. The read status (debounced) is updated into the 
* shared memory using data collector interface.
*********************************************************************************************************
*/

static uint8 u8_BtnPresd;        // Remember which button was pressed
static uint8 y = 1 ;
static uint8 bpm_old ;

void  KeyPad_Proc (void)
{
  static uint8 Pump_Last_key_Press_Dur = 0 ;
  static uint8 Flush_Last_key_Press_Dur = 0 ;
  static Srvc_DebounceState_t      st_KeyPad_stateDebBtn1;      ///< State variable for Button 1 debouncing
  static Srvc_DebounceState_t      st_KeyPad_stateDebBtn2;      ///< State variable for Button 2 debouncing 
  static Srvc_DebounceState_t      st_KeyPad_stateDebBtn3;      ///< State variable for Button 3 debouncing
  static Srvc_DebounceState_t      st_KeyPad_stateDebBtn4;      ///< State variable for Button 4 debouncing
    
  static Srvc_DebounceState_t      st_swt_stateDeb_pump;      ///< State variable for pump swt debouncing
  static Srvc_DebounceState_t      st_swt_stateDeb_flush;      ///< State variable for flush swt debouncing
  
  DC_KeyPad_Intfce_Struct  st_KeyPad_rawVal;           // Local structure to store the button status (raw value)
  DC_KeyPad_Intfce_Struct  st_KeyPad_debVal;           // Local structure to store the button status (debounced)

  uint8 u8_Pump_swt_st  ;
  uint8 u8_flush_swt_st ;
  uint8 u8_man_swt_st   ;

  uint8 i;

  if(Pump_Last_key_Press_Dur)
    Pump_Last_key_Press_Dur++ ;
  if(Pump_Last_key_Press_Dur>=200)
    Pump_Last_key_Press_Dur = 0 ;
  
  if(Flush_Last_key_Press_Dur)
    Flush_Last_key_Press_Dur++ ;
  if(Flush_Last_key_Press_Dur>=200)
    Flush_Last_key_Press_Dur = 0 ;

/*----------------------------------------------------------------------------*/
/*  
  if(y%33 == 0)
  {
    beats++;  
    y = 1 ;
    wave_first_line[15] = '^' ;
  }
  y++ ;
*/

/*  
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
    
    sprintf(BPM_second_line, "BPM: %d  HB: %d", bpm, HB );
    print_on_LCD(BPM_second_line) ;
  }
//  bpm_old = bpm ;
*/  
/*----------------------------------------------------------------------------*/
  
  // Set Manual/Auto switch st based on the push button 
  u8_man_swt_st = (uint8)((GPIO_Get(GPIOB, Manual_swt_I_D) == 1)? MANUAL_MODE : AUTO_MODE);  
  Set_Manual_swt_st(u8_man_swt_st);
  
  
  // Local structure to store the final button status (after button release)
  DC_KeyPad_Intfce_Struct  st_KeyPad_finalVal = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, // Buttons
                                                 FALSE,  // Multiple key-press
                                                 0};     // Timer

  // Code_TAG: SRC_KEYPAD_002: Scan the keys to get the raw value (pressed key retuns 'LOW')
  st_KeyPad_rawVal.u8_statBtn1 = (uint8)((GPIO_Get(GPIOB, KEYPAD1_I_D) == 1)? TRUE : FALSE);  
  st_KeyPad_rawVal.u8_statBtn2 = (uint8)((GPIO_Get(GPIOB, KEYPAD2_I_D) == 1)? TRUE : FALSE);  
  st_KeyPad_rawVal.u8_statBtn3 = (uint8)((GPIO_Get(GPIOB, KEYPAD3_I_D) == 1)? TRUE : FALSE);  
  st_KeyPad_rawVal.u8_statBtn4 = (uint8)((GPIO_Get(GPIOB, KEYPAD4_I_D) == 1)? TRUE : FALSE);  
  
  st_KeyPad_rawVal.u8_stat_pump_swt  = (uint8)((GPIO_Get(GPIOB, Pump_swt_I_D) == 1)? TRUE : FALSE);  
  st_KeyPad_rawVal.u8_stat_flush_swt = (uint8)((GPIO_Get(GPIOB, Man_Flush_swt_I_D) == 1)? TRUE : FALSE);  
  
  // Code_TAG: SRC_KEYPAD_003: Debounce the raw values and update into the local memory
  // Debouncing for Button 1
  st_KeyPad_debVal.u8_statBtn1 = Srvc_Debounce (st_KeyPad_rawVal.u8_statBtn1,
                                                &st_KeyPad_stateDebBtn1,
                                                &st_KeyPad_tmDebBtn1_ms,
                                                DT);

  // Debouncing for Button 2
  st_KeyPad_debVal.u8_statBtn2 = Srvc_Debounce (st_KeyPad_rawVal.u8_statBtn2,
                                                &st_KeyPad_stateDebBtn2,
                                                &st_KeyPad_tmDebBtn2_ms,
                                                DT);
  
  // Debouncing for Button 3
  st_KeyPad_debVal.u8_statBtn3 = Srvc_Debounce (st_KeyPad_rawVal.u8_statBtn3,
                                                &st_KeyPad_stateDebBtn3,
                                                &st_KeyPad_tmDebBtn3_ms,
                                                DT);

  // Debouncing for Button 4  
  st_KeyPad_debVal.u8_statBtn4 = Srvc_Debounce (st_KeyPad_rawVal.u8_statBtn4,
                                                &st_KeyPad_stateDebBtn4,
                                                &st_KeyPad_tmDebBtn4_ms,
                                                DT);

  // Debouncing for pump swt  
  st_KeyPad_debVal.u8_stat_pump_swt = Srvc_Debounce (st_KeyPad_rawVal.u8_stat_pump_swt,
                                                &st_swt_stateDeb_pump,
                                                &st_pump_swt_tmDeb_ms,
                                                DT);

  // Debouncing for flush swt 
  st_KeyPad_debVal.u8_stat_flush_swt = Srvc_Debounce (st_KeyPad_rawVal.u8_stat_flush_swt,
                                                &st_swt_stateDeb_flush,
                                                &st_flush_swt_tmDeb_ms,
                                                DT);
  
  // Check if any button is pressed and previous button is cleared (release time is stopped)
  if ((Srvc_TestSWTmrU32(&st_KeyPad_TmSinceBtnRlsd) != FALSE) &&
      (
       (st_KeyPad_debVal.u8_statBtn1 != FALSE) || 
       (st_KeyPad_debVal.u8_statBtn2 != FALSE) || 
       (st_KeyPad_debVal.u8_statBtn3 != FALSE) ||
       (st_KeyPad_debVal.u8_statBtn4 != FALSE) ||
       (st_KeyPad_debVal.u8_stat_pump_swt != FALSE) ||
       (st_KeyPad_debVal.u8_stat_flush_swt != FALSE) 
      )
     )
  {
    Srvc_StartSWTmrU32(&st_KeyPad_TmSinceBtnPrsd); // Restart the timer 

    // Remember which button is pressed
    if(st_KeyPad_debVal.u8_statBtn1 != FALSE)
    {
      u8_BtnPresd = 1;
    }
    else if (st_KeyPad_debVal.u8_statBtn2 != FALSE)
    {
      u8_BtnPresd = 2;
    }
    else if (st_KeyPad_debVal.u8_statBtn3 != FALSE)
    {
      u8_BtnPresd = 3;
    }
    else if (st_KeyPad_debVal.u8_statBtn4 != FALSE)
    {
      u8_BtnPresd = 4;
    }
    else if (st_KeyPad_debVal.u8_stat_pump_swt != FALSE)
    {
      u8_BtnPresd = 5;
      
      if(Pump_Last_key_Press_Dur == 0)
      {
        Toggle_Pump_swt_st() ;
        Pump_Last_key_Press_Dur = 1 ;
      }

    }
    else if (st_KeyPad_debVal.u8_stat_flush_swt != FALSE)
    {
      u8_BtnPresd = 6;
      Toggle_Flush_swt_st() ;

      if(Flush_Last_key_Press_Dur == 0)
      {
        Toggle_Pump_swt_st() ;
        Flush_Last_key_Press_Dur = 1 ;
      }
    }
    
  }
  else if((u8_BtnPresd != 0) && // If button pressed
          (Srvc_TestSWTmrU32(&st_KeyPad_TmSinceBtnRlsd) != FALSE) && // release timer is not started yet
          (st_KeyPad_debVal.u8_statBtn1 == FALSE) && 
          (st_KeyPad_debVal.u8_statBtn2 == FALSE) && 
          (st_KeyPad_debVal.u8_statBtn3 == FALSE) &&
          (st_KeyPad_debVal.u8_statBtn4 == FALSE) &&
          (st_KeyPad_debVal.u8_stat_pump_swt == FALSE) &&
          (st_KeyPad_debVal.u8_stat_flush_swt == FALSE)
         )
  {
    Srvc_StartSWTmrU32(&st_KeyPad_TmSinceBtnRlsd); // Start the release timer
  }
  else if ((Srvc_TestSWTmrU32(&st_KeyPad_TmSinceBtnRlsd) == FALSE) && // If button released timer is running and
           (Srvc_DiffSWTmrU32(&st_KeyPad_TmSinceBtnRlsd) >  500) )    // timer is elapsed
  {
    Srvc_StopSWTmrU32(&st_KeyPad_TmSinceBtnRlsd);   // Stop the release timer

    u8_BtnPresd = 0; // Reset button pressed status
  }
  else
  {
    // Do nothing
  }

  // Release timer is running --> Update the final button status
  if((Srvc_TestSWTmrU32(&st_KeyPad_TmSinceBtnRlsd) == FALSE))
  {
    // Activate which button is pressed
    switch(u8_BtnPresd)
    {
      case 1:
      {
        st_KeyPad_finalVal.u8_statBtn1 = TRUE;
      }
      break;

      case 2:
      {
        st_KeyPad_finalVal.u8_statBtn2 = TRUE;
      }
      break;

      case 3:
      {
        st_KeyPad_finalVal.u8_statBtn3 = TRUE;
      }
      break;

      case 4:
      {
        st_KeyPad_finalVal.u8_statBtn4 = TRUE;
      }
      break;

      case 5:
      {
        st_KeyPad_finalVal.u8_stat_pump_swt = TRUE;
      }
      break;
      
      case 6:
      {
        st_KeyPad_finalVal.u8_stat_flush_swt = TRUE;
      }
      break;

      default:
      break;
    }

  }

  // Check for simultaneous key-press of specific keys (button-1 to button-4)
  // Used for special cases (e.g. reset of PIN)
  if((st_KeyPad_debVal.u8_statBtn1 != FALSE) && (st_KeyPad_debVal.u8_statBtn2!= FALSE) &&
     (st_KeyPad_debVal.u8_statBtn3 != FALSE) && (st_KeyPad_debVal.u8_statBtn4!= FALSE))
  {
    st_KeyPad_finalVal.u8_multipleKeyPrsd = TRUE;
  }
  
  // Simulatneous key-press of buttons 1 2 3 starts ASD Logging
  if(
     (TRUE == st_KeyPad_debVal.u8_statBtn1) && 
     (TRUE == st_KeyPad_debVal.u8_statBtn2) &&
     (TRUE == st_KeyPad_debVal.u8_statBtn3) &&
     (FALSE == st_KeyPad_debVal.u8_statBtn4)
    )
  {
    st_KeyPad_finalVal.u8_ASD_Detect = TRUE;
  }

  // Update the time
  st_KeyPad_finalVal.u32_tmSinceBtnPrsd_ms = Srvc_DiffSWTmrU32(&st_KeyPad_TmSinceBtnPrsd);

  // Code_TAG: SRC_KEYPAD_004: Update the keypad status to data collector
//  DatCol_Write (DC_INTFCE_KEYPAD_1, &st_KeyPad_finalVal);

  // Detect if any button is stuck for long time and update the error vector if so
//  DSM_ErrHdlr (KEYPAD_BTN1STCK_ERR, st_KeyPad_debVal.u8_statBtn1 != FALSE);  // Update the error vector
//  DSM_ErrHdlr (KEYPAD_BTN2STCK_ERR, st_KeyPad_debVal.u8_statBtn2 != FALSE);  // Update the error vector
//  DSM_ErrHdlr (KEYPAD_BTN3STCK_ERR, st_KeyPad_debVal.u8_statBtn3 != FALSE);  // Update the error vector
//  DSM_ErrHdlr (KEYPAD_BTN4STCK_ERR, st_KeyPad_debVal.u8_statBtn4 != FALSE);  // Update the error vector

/*  
  Shell_Printf ("1: %d   %d  %d\n", st_KeyPad_rawVal.u8_statBtn1,  st_KeyPad_debVal.u8_statBtn1, st_KeyPad_finalVal.u8_statBtn1);
  Shell_Printf ("2: %d   %d  %d\n", st_KeyPad_rawVal.u8_statBtn2,  st_KeyPad_debVal.u8_statBtn2, st_KeyPad_finalVal.u8_statBtn2);
  Shell_Printf ("3: %d   %d  %d\n", st_KeyPad_rawVal.u8_statBtn3,  st_KeyPad_debVal.u8_statBtn3, st_KeyPad_finalVal.u8_statBtn3);
  Shell_Printf ("4: %d   %d  %d\n", st_KeyPad_rawVal.u8_statBtn4,  st_KeyPad_debVal.u8_statBtn4, st_KeyPad_finalVal.u8_statBtn4);

  Shell_Printf ("\n\n");
*/
  
}

/**
***************************************************************************************************
* Function to restart KeyPad timer
***************************************************************************************************
*/

void  KeyPad_ResetTmr(void)
{
  Srvc_StartSWTmrU32(&st_KeyPad_TmSinceBtnPrsd); // Restart the timer
}  

/**
***************************************************************************************************
* Function to write keypad status values to shared memory
*
* \param  *Dest   Pointer to destination data (shared memory location)
* \param  *Src    Pointer to source data (local memory location)
*
* \return None.
***************************************************************************************************
*/
/*
void DC_KeyPad_1_Wr (void *Dest, void *Src)
{
  DC_KeyPad_Intfce_Struct   *st_SharedMemPtr;  // Pointer to shared memory
  DC_KeyPad_Intfce_Struct   *st_LocaldMemPtr;  // Pointer to local memory
  
  st_SharedMemPtr = (DC_KeyPad_Intfce_Struct*)Dest;
  st_LocaldMemPtr = (DC_KeyPad_Intfce_Struct*)Src;
  
  // Copy operation from local memory to shared memory
  Mem_Copy (st_SharedMemPtr, st_LocaldMemPtr, sizeof(DC_KeyPad_Intfce_Struct));
}


/*
 ============================================================================
 Name        :
 Author      : SKN
 Created on  : Jan 11, 2014
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */

void SELECT_Screen(uint8 *u8currentScreen, uint8 *u8userButton)
{

	if(*u8currentScreen != WELCOME_SCREEN)
	{
//		SimuKey(&u8_BtnPresd) ;         // Sri Pending

		switch(u8_BtnPresd)
		{
			case KEYPAD_1:
			{
				if(*u8currentScreen == HOME_SCREEN){
					*u8userButton = MODE_SELECT;
					*u8currentScreen = MODE_SELECTION_SCREEN ;
				}
				else if(*u8currentScreen == MODE_SELECTION_SCREEN){
					*u8userButton = MANUAL_MODE_KEY;
					*u8currentScreen = MANUAL_SCREEN ;
				}
				else if(*u8currentScreen == DIAG_INFO_SCREEN){
					*u8userButton = NOT_ASSIGNED;
					*u8currentScreen = DIAG_INFO_SCREEN ;
				}
				else if(*u8currentScreen == ABOUT_SYSTEM_SCREEN){
					*u8userButton = NOT_ASSIGNED;
					*u8currentScreen = ABOUT_SYSTEM_SCREEN ;
				}
				else if(*u8currentScreen == MANUAL_SCREEN){
					*u8userButton = MANUAL_SWT_ON;
					*u8currentScreen = MANUAL_STATE_SCREEN ;
				}
				else if(*u8currentScreen == AUTO_SCREEN){
					*u8userButton = NOT_ASSIGNED;
					*u8currentScreen = AUTO_SCREEN ;
				}
				else if(*u8currentScreen == VENDING_SCREEN){
					*u8userButton = QUANTITY_FIELD;
					*u8currentScreen = VENDING_SCREEN ;
                                        unit_select   = UNIT_VOL ;
				}
				else if(*u8currentScreen == VENDING_FIELD_ENTRY_SCREEN){
					*u8userButton = INCREMENT_STEP;
					*u8currentScreen = VENDING_FIELD_ENTRY_SCREEN ;
                                        
					if( user_intput.unit == UNIT_VOL)
					{
                                          if(value_set<DISPENSE_MAX_PRE_SET_LTS)
                                            value_set+= DISPENSE_INCR_DECR_PRE_SET_LTS ;
                                          else
                                            value_set = DISPENSE_SET_ZERO ;
					}
					if( user_intput.unit == UNIT_RS )
					{
                                          if(value_set<DISPENSE_MAX_PRE_SET_PRICE)
                                            value_set+= DISPENSE_INCR_DECR_PRE_SET_PRICE ;
                                          else
                                            value_set = DISPENSE_SET_ZERO ;
					}
                                        
				}
				else if(*u8currentScreen == FLUSH_NOW_SCREEN){
					*u8userButton = FLUSH_SWT_ON;
					*u8currentScreen = FLUSH_NOW_STATE_SCREEN ;
                                        Set_Display_reqeust_key_st (DISPLAY_STATE_DATA) ;
				}
				else if(*u8currentScreen == MANUAL_STATE_SCREEN){
                                  *u8userButton = NOT_ASSIGNED;
                                  *u8currentScreen = MANUAL_STATE_SCREEN ;
                                  Set_Display_reqeust_key_st (DISPLAY_STATE_DATA) ;          
				}
				else if(*u8currentScreen == VENDING_STATE_SCREEN){
                                  *u8userButton = NOT_ASSIGNED;
                                  *u8currentScreen = VENDING_STATE_SCREEN ;
                                  Set_Display_reqeust_key_st (DISPLAY_STATE_DATA) ;          
				}
				else if(*u8currentScreen == FLUSH_NOW_STATE_SCREEN){
                                  *u8userButton = NOT_ASSIGNED;
                                  *u8currentScreen = FLUSH_NOW_STATE_SCREEN ;
                                  Set_Display_reqeust_key_st (DISPLAY_STATE_DATA) ;          
				}

				break;
			}

			case KEYPAD_2:
			{
				if(*u8currentScreen == HOME_SCREEN){
					*u8userButton = DIAG_INFO;
					*u8currentScreen = DIAG_INFO_SCREEN ;
				}
				else if(*u8currentScreen == MODE_SELECTION_SCREEN){
					*u8userButton = AUTO_MODE_KEY;
					*u8currentScreen = AUTO_SCREEN ;
				}
				else if(*u8currentScreen == DIAG_INFO_SCREEN){
					*u8userButton = NOT_ASSIGNED;
					*u8currentScreen = DIAG_INFO_SCREEN ;
				}
				else if(*u8currentScreen == ABOUT_SYSTEM_SCREEN){
					*u8userButton = NOT_ASSIGNED;
					*u8currentScreen = ABOUT_SYSTEM_SCREEN ;
				}
				else if(*u8currentScreen == MANUAL_SCREEN){
					*u8userButton = MANUAL_SWT_OFF;
					*u8currentScreen = MANUAL_STATE_SCREEN ;
				}
				else if(*u8currentScreen == AUTO_SCREEN){
					*u8userButton = FILL_NOW;
					*u8currentScreen = MANUAL_SCREEN ;
				}
				else if(*u8currentScreen == VENDING_SCREEN){
					*u8userButton = PRICE_FIELD;
					*u8currentScreen = VENDING_SCREEN ;
                                        unit_select   = UNIT_RS ;
				}
				else if(*u8currentScreen == VENDING_FIELD_ENTRY_SCREEN){
					*u8userButton = DECREMENT_STEP;
					*u8currentScreen = VENDING_FIELD_ENTRY_SCREEN ;
                                        
					if( user_intput.unit == UNIT_VOL)
					{
                                          if(value_set>DISPENSE_SET_ZERO)
                                            value_set-= DISPENSE_INCR_DECR_PRE_SET_LTS ;
                                          else
                                            value_set = DISPENSE_MAX_PRE_SET_LTS ;
					}
					if( user_intput.unit == UNIT_RS)
					{
					  if(value_set>DISPENSE_SET_ZERO)
                                            value_set-= DISPENSE_INCR_DECR_PRE_SET_PRICE ;
					  else
                                            value_set = DISPENSE_MAX_PRE_SET_PRICE ;
					}
                                        
				}
				else if(*u8currentScreen == FLUSH_NOW_SCREEN){
					*u8userButton = FLUSH_SWT_OFF;
					*u8currentScreen = FLUSH_NOW_STATE_SCREEN ;
				}
				else if(*u8currentScreen == MANUAL_STATE_SCREEN){
					*u8userButton = DISP_STATE_VAL;
					*u8currentScreen = MANUAL_STATE_SCREEN ;
                                        Set_Display_reqeust_key_st (DISP_DEBUG_SENSOR_DATA) ;          
				}
				else if(*u8currentScreen == VENDING_STATE_SCREEN){
					*u8userButton = DISP_STATE_VAL;
					*u8currentScreen = VENDING_STATE_SCREEN ;
                                        Set_Display_reqeust_key_st (DISP_DEBUG_SENSOR_DATA) ;          
				}
				else if(*u8currentScreen == FLUSH_NOW_STATE_SCREEN){
					*u8userButton = DISP_STATE_VAL;
					*u8currentScreen = FLUSH_NOW_STATE_SCREEN ;
                                        Set_Display_reqeust_key_st (DISP_DEBUG_SENSOR_DATA) ;          
				}

				break;
			}

			case KEYPAD_3:
			{
				if(*u8currentScreen == HOME_SCREEN){
					*u8userButton = ABOUT_SYSTEM;
					*u8currentScreen = ABOUT_SYSTEM_SCREEN ;
				}
				else if(*u8currentScreen == MODE_SELECTION_SCREEN){
					*u8userButton = VENDING_MODE_KEY;
					*u8currentScreen = VENDING_SCREEN ;
				}
				else if(*u8currentScreen == DIAG_INFO_SCREEN){
					*u8userButton = ABOUT_SYSTEM_HEALTH_MSGS;
					*u8currentScreen = ABOUT_SYSTEM_HEALTH_SCREEN ;
				}
				else if(*u8currentScreen == ABOUT_SYSTEM_SCREEN){
					*u8userButton = NOT_ASSIGNED;
					*u8currentScreen = ABOUT_SYSTEM_SCREEN ;
				}
				else if(*u8currentScreen == MANUAL_SCREEN){
					*u8userButton = ABOUT_MANUAL_MODE;
					*u8currentScreen = ABOUT_MANUAL_MODE_SCREEN ;
				}
				else if(*u8currentScreen == MANUAL_STATE_SCREEN){
					*u8userButton = NOT_ASSIGNED;
					*u8currentScreen = MANUAL_STATE_SCREEN ;
				}
				else if(*u8currentScreen == AUTO_SCREEN){
					*u8userButton = ABOUT_AUTO_MODE;
					*u8currentScreen = ABOUT_AUTO_MODE_SCREEN ;
				}
				else if(*u8currentScreen == VENDING_SCREEN){
                                        //if(unit_select != DISPENSE_SET_ZERO)
                                        {
                                          *u8userButton = SELECT_FIELD_UNIT;
                                          *u8currentScreen = VENDING_FIELD_ENTRY_SCREEN ;
                                          user_intput.unit   = unit_select ;
                                        }
				}
				else if(*u8currentScreen == VENDING_FIELD_ENTRY_SCREEN){
                                        //if(value_set != DISPENSE_SET_ZERO)
                                        {
                                          *u8userButton = SELECT_FIELD_ENTRY_VALUE;
                                          *u8currentScreen = VENDING_STATE_SCREEN ;
                                          if(user_intput.unit == UNIT_VOL)
                                            user_intput.vol =  value_set ;
                                          else if(user_intput.unit == UNIT_RS)
                                            user_intput.price =  value_set ;
                                          Vending_Enable_st = ENABLE ;
                                        }                                        
				}
				else if(*u8currentScreen == VENDING_STATE_SCREEN){
					*u8userButton = NOT_ASSIGNED;
					*u8currentScreen = VENDING_STATE_SCREEN ;
				}
				else if(*u8currentScreen == FLUSH_NOW_SCREEN){
					*u8userButton = ABOUT_FLUSH_NOW_MODE ;
					*u8currentScreen = ABOUT_FLUSH_NOW_MODE_SCREEN ;
				}
				else if(*u8currentScreen == FLUSH_NOW_STATE_SCREEN){
					*u8userButton = NOT_ASSIGNED;
					*u8currentScreen = FLUSH_NOW_STATE_SCREEN ;
				}

				break;
			}

			case KEYPAD_4:
			{
				if(*u8currentScreen == HOME_SCREEN){
					*u8userButton = FLUSH_NOW_MODE_KEY;
					*u8currentScreen = FLUSH_NOW_SCREEN ;
				}
				else if(*u8currentScreen == MODE_SELECTION_SCREEN){
					*u8userButton = BACK_TO_HOME_SCREEN;
					*u8currentScreen = HOME_SCREEN ;
				}
				else if(*u8currentScreen == DIAG_INFO_SCREEN){
					*u8userButton = BACK_TO_HOME_SCREEN;
					*u8currentScreen = HOME_SCREEN ;
				}
				else if(*u8currentScreen == ABOUT_SYSTEM_SCREEN){
					*u8userButton = BACK_TO_HOME_SCREEN;
					*u8currentScreen = HOME_SCREEN ;
				}
				else if(*u8currentScreen == MANUAL_SCREEN){
					*u8userButton = BACK_TO_MODE_SELECTION_SCREEN;
					*u8currentScreen = MODE_SELECTION_SCREEN ;
				}
				else if(*u8currentScreen == ABOUT_MANUAL_MODE_SCREEN){
					*u8userButton = BACK_TO_MANUAL_SCREEN;
					*u8currentScreen = MANUAL_SCREEN ;
				}
				else if(*u8currentScreen == MANUAL_STATE_SCREEN){
					*u8userButton = BACK_TO_MANUAL_SCREEN ;
					*u8currentScreen = MANUAL_SCREEN ;
				}
				else if(*u8currentScreen == AUTO_SCREEN){
					*u8userButton = BACK_TO_MODE_SELECTION_SCREEN;
					*u8currentScreen = MODE_SELECTION_SCREEN ;
				}
				else if(*u8currentScreen == VENDING_SCREEN){
					*u8userButton           =  BACK_TO_VENDING_SCREEN ;     //BACK_TO_MODE_SELECTION_SCREEN;
					*u8currentScreen        =  VENDING_SCREEN       ;       //MODE_SELECTION_SCREEN;
                                        user_intput.price       =  DISPENSE_SET_ZERO    ;
					user_intput.vol         =  DISPENSE_SET_ZERO    ;
					user_intput.unit        =  DISPENSE_SET_ZERO    ;
					value_set               =  DISPENSE_SET_ZERO    ;
					unit_select             =  DISPENSE_SET_ZERO    ;                                        
				}
				else if(*u8currentScreen == VENDING_FIELD_ENTRY_SCREEN){
					*u8userButton           =  BACK_TO_VENDING_SCREEN;
					*u8currentScreen        =  VENDING_SCREEN       ;
                                        user_intput.price       =  DISPENSE_SET_ZERO    ;
					user_intput.vol         =  DISPENSE_SET_ZERO    ;
					user_intput.unit        =  DISPENSE_SET_ZERO    ;
					value_set               =  DISPENSE_SET_ZERO    ;
					unit_select             =  DISPENSE_SET_ZERO    ;                                        
				}
				else if(*u8currentScreen == VENDING_STATE_SCREEN){
					*u8userButton = NOT_ASSIGNED;
					*u8currentScreen = VENDING_STATE_SCREEN ;
/*
                                        *u8userButton           =  BACK_TO_VENDING_FIELD_ENTRY_SCREEN;
					*u8currentScreen        =  VENDING_FIELD_ENTRY_SCREEN ;
                                        user_intput.price       =  DISPENSE_SET_ZERO    ;
					user_intput.vol         =  DISPENSE_SET_ZERO    ;
					user_intput.unit        =  DISPENSE_SET_ZERO    ;
					value_set               =  DISPENSE_SET_ZERO    ;
					unit_select             =  DISPENSE_SET_ZERO    ;                                        
                                        FS_Pulse_Cnt_curr = 0 ;
                                        FS_Pulse_Cnt_Total = 0 ;
                                        flow_sensor_vol = 0;
*/                                        
				}
				else if(*u8currentScreen == FLUSH_NOW_SCREEN){
					*u8userButton = BACK_TO_HOME_SCREEN;
					*u8currentScreen = HOME_SCREEN ;
				}
				else if(*u8currentScreen == FLUSH_NOW_STATE_SCREEN){
					*u8userButton = BACK_TO_FLUSH_NOW_SCREEN;
					*u8currentScreen = FLUSH_NOW_SCREEN ;
				}
				else if(*u8currentScreen == ABOUT_FLUSH_NOW_MODE_SCREEN){
					*u8userButton = BACK_TO_FLUSH_NOW_SCREEN;
					*u8currentScreen = FLUSH_NOW_SCREEN ;
				}
				else if(*u8currentScreen == ABOUT_SYSTEM_HEALTH_SCREEN){
					*u8userButton = BACK_TO_DIAG_INFO_SCREEN;
					*u8currentScreen = DIAG_INFO_SCREEN ;
				}
				else if(*u8currentScreen == ABOUT_AUTO_MODE_SCREEN){
					*u8userButton = BACK_TO_AUTO_SCREEN;
					*u8currentScreen = AUTO_SCREEN ;
				}

				break;
			}
		}
		u8_BtnPresd = FALSE ;
	}
}

/*------------------------------------------------------------------*/
