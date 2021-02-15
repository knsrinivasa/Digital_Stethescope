
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

char first_line[] = "Digital Stethescope " ;
char second_line[] = "    - Sri " ;
static uint8 welcome_disp_dur = 0 ;
static uint8 Welcome_display_st = FALSE;
static uint8 u8currentScreen ;
static uint8 u8userButton ;
static uint8 disp_itr_cnt = 0 ;

  float pre_fil_Us_pres	 ;
  float pre_fil_Ds_pres	 ;
  float post_mem_pres	 ;
  float flow_vol ;  
//  uint8 print_buff[50] ;

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
          
          // Get manual Switch Status
          manual_on_off_swt_st = Get_Manual_swt_st() ;

          
          if (manual_on_off_swt_st == SWT_ON)   //      Manual mode
          {
//            Sys_Mode = MANUAL_MODE;
            Set_Sys_Mode(MANUAL_MODE) ;

            Pump_on_off_swt_st = Get_Pump_swt_st() ;
            if (Vending_Enable_st == DISABLE)
            {
              if( Pump_on_off_swt_st == SWT_ON )
              {
                Clear_LCD() ;
                //print_on_LCD("Manual Mode: ON ") ;
                
                disp_req_key = Get_Display_reqeust_key_st() ;          
                
                if(disp_req_key == DISPLAY_STATE_DATA)
                {            
                  print_on_LCD("Manual Mode: ON ") ;
                  Set_Display_reqeust_key_st (DISPLAY_STATE_DATA) ;
                }
                else if(disp_req_key == DISP_DEBUG_SENSOR_DATA)
                {            
                  pre_fil_Us_pres	=	Get_PreFilter_PS_upstream() ;
                  pre_fil_Ds_pres	=	Get_PreFilter_PS_downstream() ;
                  post_mem_pres         =	Get_MEM_Filter_Ds_post() ;
                  
                  sprintf(print_buff, "P1-%0.2f  P2-%0.2f ", pre_fil_Us_pres, pre_fil_Ds_pres );
                  print_on_LCD(print_buff) ;
                  goto_cursor(LCD_START_OF_2LINE);            // goto second row
                  sprintf(print_buff, "P3-%0.2f  F-%0.2f ", post_mem_pres, Flow_vol_Disp) ;
                  print_on_LCD(print_buff) ;
//                  Set_Display_reqeust_key_st (DISPLAY_STATE_DATA) ;
                }
                
                //              u8currentScreen = MANUAL_STATE_SCREEN ;
                Set_Dispense_swt_st (DISPENSE, MANUAL_MODE) ;
                manual_pmp_on = SWT_ON ;
              }
              else if( Pump_on_off_swt_st == SWT_OFF )
              {
                Set_Dispense_swt_st (DISPENSE_STOP, MANUAL_MODE) ;
                if(u8userButton == NOT_PRESSED)
                  u8currentScreen = VENDING_SCREEN ;
                manual_pmp_on = SWT_OFF ;
              }
            }
          }
          else                                  //      Auto mode
          {
//            Sys_Mode = AUTO_MODE;
            Set_Sys_Mode(AUTO_MODE) ;
            u8currentScreen = AUTO_SCREEN ;
            u8userButton = NOT_PRESSED ;
            Display_Screen(u8currentScreen) ;
          }
          
          man_flush_st = Get_Manual_Flush_swt_st() ;    // Get manual Flush Status
          
          auto_flush_st = Get_Mem_delta_P_lvl() ;       // Get Auto Flush Status
          //flush_st = MEMBRANE_CLEAN ;     // sri pending

          Dispense_st = Get_Dispense_swt_st() ;         // Get Dispenser Status

        flush_run_st = Get_Flush_Run_Complete_st() ;
  
        if( flush_run_st != FLUSH_RUN_COMPLETED)
        {                 
          if (man_flush_st == SWT_OFF)                  // Sri Pending: Disabled auto flush
            Set_Flush_Mode_St(FLUSH_STOP) ;
          
          if( (Dispense_st == DISPENSE_STOP) && (man_flush_st == SWT_ON) )
          {
            Set_Flush_Mode_St(FLUSH_START) ;
            Display_Screen(FLUSH_NOW_STATE_SCREEN);
          }
          else if( (Dispense_st == DISPENSE_STOP) && (auto_flush_st == MEMBRANE_CLOGGED) )
          {
            Set_Flush_Mode_St(FLUSH_START) ;
            Display_Screen(FLUSH_NOW_STATE_SCREEN);
          }
        }
        else
        {
          Set_Manual_Flush_swt_st(SWT_OFF) ;
          Set_Flush_Mode_St(FLUSH_STOP) ;
        }
          
/*
          else if( (Dispense_st == DISPENSE_STOP) && (auto_flush_st != MEMBRANE_CLOGGED) )
          {
            Set_Flush_Mode_St(FLUSH_STOP) ;
            Display_Screen(FLUSH_NOW_STATE_SCREEN);
          }
*/
          if(manual_pmp_on == SWT_OFF)
          {
            SELECT_Screen(&u8currentScreen, &u8userButton);
//                  Display_Screen(u8currentScreen, u8userButton) ;
            Display_Screen(u8currentScreen) ;
          }
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
//          if(Screen_old != Screen)               // to prevent multiple print of the same screen
          {
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

            Clear_LCD() ;                  // Clear Display
            print_on_LCD("      BPM  ") ;
            Delay(3000); 
          }
          break ;

	case HOME_SCREEN :	//	Manual / (Semi-Auto) (Overflow stop feature)
//          if(Screen_old != Screen)               // to prevent multiple print of the same screen
          {
            Clear_LCD() ;
            print_on_LCD("MODE   DIAG-Inf "); 
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            print_on_LCD("About  Flush-Now ") ;
          }
          break ;
          
	case AUTO_SCREEN :	//	Auto / Semi-Auto (Fill Now Feature)
          Pump_swt = Get_Pump_swt_st() ;
          if (Pump_swt == SWT_ON )
          {
            Auto_Dispense() ;
            auto_on_off_st = Get_Dispense_swt_st() ;
            
            //if(auto_on_off_st_old != auto_on_off_st)               // to prevent multiple print of the same screen
            {
              Clear_LCD() ;
            #if (SEMI_AUTO_DISPENSE == ENABLED)
              (auto_on_off_st == 1) ? print_on_LCD("Auto:ON     FILL ") : print_on_LCD("Auto:OFF    FILL ") ;                      
            #else
              (auto_on_off_st == 1) ? print_on_LCD("Auto: ON      ") : print_on_LCD("Auto: OFF    ") ;          
            #endif
              
              goto_cursor(LCD_START_OF_2LINE);            // goto second row
              print_on_LCD("About       BACK ") ;
              auto_on_off_st_old = auto_on_off_st ;
            }
          }
          else
          {
            Set_Dispense_swt_st (DISPENSE_STOP, AUTO_MODE) ;
            Clear_LCD() ;
            print_on_LCD("Press Pump-Start ") ;
          }
          break ;
          
	case ABOUT_SYSTEM_SCREEN:
//          if(Screen_old != Screen)               // to prevent multiple print of the same screen
          {
            Clear_LCD() ;
            print_on_LCD("Digital Stethescope ") ;
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            print_on_LCD("            BACK ") ;
          }
          break;

	case FLUSH_NOW_SCREEN : // Flush Now
  //        if(Screen_old != Screen)               // to prevent multiple print of the same screen
          {
            Clear_LCD() ;
            print_on_LCD("ON           OFF ") ;
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            print_on_LCD("About       BACK ") ;
          }
          break;

	case FLUSH_NOW_STATE_SCREEN:
          Flush_Now() ;
          flush_on_off_st = Get_Flush_Mode_St() ;
          disp_req_key = Get_Display_reqeust_key_st() ;          
          
//          if(flush_on_off_st_old != flush_on_off_st)               // to prevent multiple print of the same screen
          {
              if(disp_req_key == DISPLAY_STATE_DATA)
              {            
                Clear_LCD() ;
                (flush_on_off_st == 1) ? print_on_LCD("Flush:  ON ") : print_on_LCD("Flush:  OFF ") ;          
                goto_cursor(LCD_START_OF_2LINE);            // goto second row
                print_on_LCD("            BACK ") ;
                flush_on_off_st_old = flush_on_off_st ;
                Set_Display_reqeust_key_st (DISPLAY_STATE_DATA) ;
              }
              else if(disp_req_key == DISP_DEBUG_SENSOR_DATA)
              {            
                pre_fil_Us_pres	=	Get_PreFilter_PS_upstream() ;
                pre_fil_Ds_pres	=	Get_PreFilter_PS_downstream() ;
                post_mem_pres	=	Get_MEM_Filter_Ds_post() ;
                
                sprintf(print_buff, "P1-%0.2f  P2-%0.2f ", pre_fil_Us_pres, pre_fil_Ds_pres );
                print_on_LCD(print_buff) ;
                goto_cursor(LCD_START_OF_2LINE);            // goto second row
                sprintf(print_buff, "P3-%0.2f  F-%0.2f ", post_mem_pres, Flow_vol_Disp) ;
                print_on_LCD(print_buff) ;
//                Set_Display_reqeust_key_st (DISPLAY_STATE_DATA) ;
              }
          }
          break;
          
	case ABOUT_FLUSH_NOW_MODE_SCREEN:
//          if(Screen_old != Screen)               // to prevent multiple print of the same screen
          {
            Clear_LCD() ;
            print_on_LCD("Auto Flush-3bar ") ;
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            print_on_LCD("            BACK ") ;
          }
          break;

	case MODE_SELECTION_SCREEN:
//          if(Screen_old != Screen)               // to prevent multiple print of the same screen
          {
            Clear_LCD() ;
            print_on_LCD("MANUAL      AUTO ") ;
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            print_on_LCD("VENDING     BACK ") ;
          }
          break;

	case MANUAL_SCREEN:
//          if(Screen_old != Screen)               // to prevent multiple print of the same screen
          {
            Clear_LCD() ;
            print_on_LCD(" ON          OFF ") ;
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            print_on_LCD("ABOUT       BACK ") ;
          }
          break;

	case MANUAL_STATE_SCREEN:
          Manual_Semi_Auto_Dispense() ;
          dispenser_swt_st = Get_Dispense_swt_st() ;          
          disp_req_key = Get_Display_reqeust_key_st() ;          

            Clear_LCD() ;
          
//          if(dispenser_swt_st_old != dispenser_swt_st )               // to prevent multiple print of the same screen
          if(disp_req_key == DISPLAY_STATE_DATA)
          {                        
            (dispenser_swt_st == 1) ? print_on_LCD("Manual Mode: ON ") : print_on_LCD("Manual Mode: OFF ") ;          
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            print_on_LCD("            BACK ") ;            
            dispenser_swt_st_old = dispenser_swt_st ;
            Set_Display_reqeust_key_st (DISPLAY_STATE_DATA) ;
          }
          else if(disp_req_key == DISP_DEBUG_SENSOR_DATA)
          {            
            pre_fil_Us_pres	=	Get_PreFilter_PS_upstream() ;
            pre_fil_Ds_pres	=	Get_PreFilter_PS_downstream() ;
            post_mem_pres	=	Get_MEM_Filter_Ds_post() ;
            
            sprintf(print_buff, "P1-%0.2f  P2-%0.2f ", pre_fil_Us_pres, pre_fil_Ds_pres );
            print_on_LCD(print_buff) ;
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            sprintf(print_buff, "P3-%0.2f  F-%0.2f ", post_mem_pres, Flow_vol_Disp) ;
            print_on_LCD(print_buff) ;
//            Set_Display_reqeust_key_st (DISPLAY_STATE_DATA) ;
          }
          break;
          
	case VENDING_SCREEN :	//	Vending         // sri pending selection indicator >
//LCD		puts("\tQuantity\tPrice\n\tSelect\t\tBack\n") ;          
//          if(unit_select_old != unit_select)               // to prevent multiple print of the same screen
          {
            Clear_LCD() ;
            
            if (unit_select == UNIT_VOL)
            {
              
              buff[0] = 0x7E ;
              buff[1] = '\0' ;
              print_on_LCD(buff) ;
              
              print_on_LCD("Volume     Cost ") ;
              
//              sel_char = 0x7E;
//              sprintf(print_buff, "%d Volume     Cost ", sel_char );
//              print_on_LCD(buff) ;
            }
            else if (unit_select == UNIT_RS)
            {
              
              print_on_LCD("Volume     ") ;
              buff[0] = 0x7E ;                          // prints '->'
              buff[1] = '\0' ;
              print_on_LCD(buff) ;
              
              print_on_LCD("Cost ") ;
              
//              sel_char = 0x7E;
//              sprintf(print_buff, "Volume     %dCost ", sel_char );
//              print_on_LCD(buff) ;
              
            }
            else
            {
              print_on_LCD("Volume      Cost ") ;
            }              
            
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            print_on_LCD("Select      BACK ") ;
            unit_select_old = unit_select ;
          }
          break ;

	case VENDING_FIELD_ENTRY_SCREEN :
//LCD		printf("\tUp\t%d\tDown\n\tSelect\t\tBack\n", value_set) ;
//          if(value_set_old != value_set)               // to prevent multiple print of the same screen
          {
            Clear_LCD() ;
/*            
            print_on_LCD("UP    ") ;         // sri pending incr/ decr steps

            input_value = value_set ;
            i = 0 ;
            do
            {
              buff[i] = (input_value % 10) + 0x30;
              input_value = input_value / 10 ;
              i++ ;
            } while(input_value) ;
            
            buff[i] = '\0' ;
            num_len = i-1 ;
            for(i=0; i<((num_len+1)/2);i++)
              buff[i]^=buff[num_len-i]^=buff[i]^=buff[num_len-i] ;
              
            print_on_LCD(buff) ;
            
            print_on_LCD("   DOWN ") ; 
*/
            if(value_set<100)
              sprintf(print_buff, "UP    %d    DOWN ", value_set );
            else
              sprintf(print_buff, "UP    %d   DOWN ", value_set );
            
            print_on_LCD(print_buff) ;

            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            print_on_LCD("Select      BACK ") ;
            value_set_old = value_set ;
          }
          break;

	case VENDING_STATE_SCREEN:
          if (Vending_Enable_st == ENABLE)
          {
            Pump_swt = Get_Pump_swt_st() ;
            if (Pump_swt == SWT_OFF)
            {
              Clear_LCD() ;
              print_on_LCD("Press Pump-Start ") ;
            }
            else if (Pump_swt == SWT_ON)
            {
//              Set_Sys_Mode(VENDING_MODE) ;
              Set_Sys_Mode(MANUAL_MODE) ;
              disp_req_key = Get_Display_reqeust_key_st() ;          
              Vending_Mode() ;        // sri pending incr/ decr steps
              Clear_LCD() ;
              
              // print_on_LCD("Dispensing: ") ;
              // input_value = flow_sensor_vol ; // dispensing_ltrs
              //          print_on_LCD("Remaining: ") ;
              input_value = user_intput.vol-flow_sensor_vol ; // remaining_ltrs

              if(disp_req_key == DISPLAY_STATE_DATA)
              {            
                sprintf(print_buff, "Disp %f Lt ", flow_sensor_vol  );
                //sprintf(print_buff, "Remaining %f Lt ", input_value  );
                print_on_LCD(print_buff) ;
                Set_Display_reqeust_key_st (DISPLAY_STATE_DATA) ;
              }
              else if(disp_req_key == DISP_DEBUG_SENSOR_DATA)
              {            
                pre_fil_Us_pres	=	Get_PreFilter_PS_upstream() ;
                pre_fil_Ds_pres	=	Get_PreFilter_PS_downstream() ;
                post_mem_pres	=	Get_MEM_Filter_Ds_post() ;
                
                sprintf(print_buff, "P1-%0.2f  P2-%0.2f ", pre_fil_Us_pres, pre_fil_Ds_pres );
                print_on_LCD(print_buff) ;
                goto_cursor(LCD_START_OF_2LINE);            // goto second row
                sprintf(print_buff, "P3-%0.2f  F-%0.2f ", post_mem_pres, Flow_vol_Disp) ;
                print_on_LCD(print_buff) ;
//                Set_Display_reqeust_key_st (DISPLAY_STATE_DATA) ;
              }
              
/*
            sprintf(print_buff, "PS1:%f   PS2:%f ", pre_fil_Us_pres, pre_fil_Ds_pres );
            print_on_LCD(print_buff) ;
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            sprintf(print_buff, "PS3:%f FS:%f", post_mem_pres, Flow_vol_Disp) ;
            print_on_LCD(print_buff) ;
*/
                
              if ( (Vending_Enable_st == DISABLE) && (input_value==0) )
              {
                Set_Pump_swt_st(SWT_OFF) ;
                Clear_LCD() ;
                print_on_LCD("    Thank You ") ;
                goto_cursor(LCD_START_OF_2LINE);            // goto second row
                sprintf(print_buff, "Disp: % 0.2f Ltrs ", Total_Dispensed_Volume) ;
                print_on_LCD(print_buff) ;
                
                Delay(3000); 
                //            u8currentScreen = VENDING_SCREEN ;
                //Toggle_Pump_swt_st() ;
                u8userButton            =  BACK_TO_VENDING_SCREEN;
                u8currentScreen         =  VENDING_SCREEN       ;
                user_intput.price       =  DISPENSE_SET_ZERO    ;
                user_intput.vol         =  DISPENSE_SET_ZERO    ;
                user_intput.unit        =  DISPENSE_SET_ZERO    ;
                value_set               =  DISPENSE_SET_ZERO    ;
                unit_select             =  DISPENSE_SET_ZERO    ;                                        
                
                FS_Pulse_Cnt_curr = 0 ;
                FS_Pulse_Cnt_Total = 0 ;
                flow_sensor_vol = 0;
                
                break;
              }
              
            }
          }
          
          
          /*          
          i = 0 ;
          do
          {
          buff[i] = (input_value % 10) + 0x30;
          input_value = input_value / 10 ;
          i++ ;
        } while(input_value) ;
          
          buff[i] = '\0' ;
          num_len = i-1 ;
          for(i=0; i<((num_len+1)/2);i++)
          buff[i]^=buff[num_len-i]^=buff[i]^=buff[num_len-i] ;
          
          print_on_LCD(buff) ;
          print_on_LCD(" Lt") ;
          */          
          //goto_cursor(LCD_START_OF_2LINE);            // goto second row
          //print_on_LCD("            BACK ") ;
          
          break;
          
	case DIAG_INFO_SCREEN :
          system_health_st = Get_System_Health() ;
          //LCD		printf("%s\n", System_Diag_Msgs[system_health_st] ) ;   // sri pending direct %s
          
          if( system_health_st && (!(Srvc_GetBitU8_B(system_health_st, SYS_HEALTHY_BIT))) )
          {
//            GPIO_Set(GPIOA, ERROR_LED_O_D, TRUE );
//            GPIO_Set(GPIOC, POWER_ON_LED_O_D, FALSE ); 
            GPIO_Set(GPIOC, POWER_ON_LED_O_D, TRUE ); 
          }
          else if ( Srvc_GetBitU8_B(system_health_st, SYS_HEALTHY_BIT) )
          {
//            GPIO_Set(GPIOA, ERROR_LED_O_D, FALSE );
            GPIO_Set(GPIOC, POWER_ON_LED_O_D, TRUE ); 
          }
          
//          if(system_health_st_old != system_health_st)               // to prevent multiple print of the same screen
          {
            Clear_LCD() ;
            do{	
              
              if ( (Srvc_GetBitU8_B(system_health_st, PREFLTR_ERROR_BIT) )&& !(Srvc_GetBitU8_B(print_idx, PREFLTR_ERROR_BIT)) )
              {
                print_on_LCD("Pre-Filter Clog   ") ;
                Srvc_SetBitU8(&print_idx, PREFLTR_ERROR_BIT) ;
                break ;
              }
              
              if ( (Srvc_GetBitU8_B(system_health_st, MEMBRANE_ERROR_BIT) ) && !(Srvc_GetBitU8_B(print_idx, MEMBRANE_ERROR_BIT)) )
              {            
                print_on_LCD("Membrane Clog  ") ;
                Srvc_SetBitU8(&print_idx, MEMBRANE_ERROR_BIT) ;
                break ;
              }
              
              if ( (Srvc_GetBitU8_B(system_health_st, FLOW_ERROR_BIT) ) && !(Srvc_GetBitU8_B(print_idx, FLOW_ERROR_BIT)) )
              {            
                print_on_LCD("No Flow  ") ;
                Srvc_SetBitU8(&print_idx, FLOW_ERROR_BIT) ;
                break ;
              }
              
              if ( (Srvc_GetBitU8_B(system_health_st, WLATER_LOW_BIT) ) && !(Srvc_GetBitU8_B(print_idx, WLATER_LOW_BIT)) )
              {
                print_on_LCD("Water Low  ") ;
                Srvc_SetBitU8(&print_idx, WLATER_LOW_BIT) ;
                break ;
              }
              
              if ( (Srvc_GetBitU8_B(system_health_st, SYS_HEALTHY_BIT) ) && !(Srvc_GetBitU8_B(print_idx, SYS_HEALTHY_BIT)) )
              {
                print_on_LCD("Filter Healthy   ") ;
                Srvc_SetBitU8(&print_idx, SYS_HEALTHY_BIT) ;
                break ;
              }
              
              print_idx = 0 ;
              break ;
              
            }while(0);
            
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            print_on_LCD("About       BACK ") ;
            system_health_st_old = system_health_st ;  
          }
          
          break;
          
	case ABOUT_SYSTEM_HEALTH_SCREEN:
//          if(Screen_old != Screen)               // to prevent multiple print of the same screen
          {
            Clear_LCD() ;
            print_on_LCD("Displays System  ") ;
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            print_on_LCD("Health      BACK ") ;
          }
          break;

	case ABOUT_MANUAL_MODE_SCREEN:
//          if(Screen_old != Screen)               // to prevent multiple print of the same screen
          {
            Clear_LCD() ;

            #if(SEMI_AUTO_DISPENSE == ENABLED)
              print_on_LCD("Fill Now Active ") ;
              goto_cursor(LCD_START_OF_2LINE);            // goto second row
              print_on_LCD("Auto off    BACK ") ;              
            #else
              print_on_LCD("Fill Now Active ") ;
              goto_cursor(LCD_START_OF_2LINE);            // goto second row
              print_on_LCD("            BACK ") ;
            #endif
            
          }
          break;

	case ABOUT_AUTO_MODE_SCREEN:
//          if(Screen_old != Screen)               // to prevent multiple print of the same screen
          {
            Clear_LCD() ;
            print_on_LCD("Auto operation ") ;
            goto_cursor(LCD_START_OF_2LINE);            // goto second row
            print_on_LCD("            BACK ") ;
          }
          break;

	default:
		break ;
	}
        Screen_old = Screen ;
  
}
/*------------------------------------------------------------------*/
