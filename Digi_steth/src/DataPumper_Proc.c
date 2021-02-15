
#include "main_inf.h"
#include "global_variables.h"
//#include <shell.h>	       // include shell

/*
 ============================================================================
 Name        : WP_DataPumper_Proc.c
 Author      : SKN
 Version     :
 Copyright   :
 Description : For pumping Data on UART for Mobile phone app...
 ============================================================================
 */


void WP_DataPumper_Proc(void)
{
  
  uint8 system_health_st    = 0 ;
  uint8 Pre_Filter_Clog_st  = 0 ;
  uint8 Membrane_Clog_st    = 0 ;
  uint8 Flow_st             = 0 ;
  uint8 Water_lvl_st        = 0 ;
  uint8 Healthy_st          = 0 ;
  uint8 Dispenser_st        = 0 ;
  uint8 man_flush_st        = 0 ;
  uint8 auto_flush_st       = 0 ;
  uint8 flush_st            = 0 ;
  uint8 mode                = 0 ;
  uint8 health_check_enb      = 0 ;
  uint8 len ;
  
//#if ( DATA_PUMPER_FORMAT == DATA_PUMPER_MOBILE )

/*
Value1 -> Dispenser Healthy
Value2 -> Water Dispensing
Value3 -> Back wash / Flush
Value4 -> Operating Mode
*/

    Pre_Filter_Clog_st  = 0 ;
    Membrane_Clog_st    = 0 ;
    Flow_st             = 0 ;
    Water_lvl_st        = 0 ;
    Healthy_st          = 0 ;
    Dispenser_st        = 0 ;
    man_flush_st        = 0 ;
    auto_flush_st       = 0 ;
    flush_st            = 0 ;
    mode                = 0 ;
    health_check_enb    = 0 ;
    
    health_check_enb    = Get_Health_Check () ;
//    system_health_st    = Get_System_Health() ;
    system_health_st    = Load_System_Health() ;
    Dispenser_st        = Get_Dispense_swt_st() ;       // Get Dispenser Status
    man_flush_st        = Get_Manual_Flush_swt_st() ;   // Get manual Flush Status
    auto_flush_st       = Get_Flush_Mode_St() ;         // Get Auto Flush Status
    flush_st            = ( auto_flush_st || man_flush_st );
    mode                = Get_Sys_Mode() ;
    
    if(health_check_enb == ENABLED)
    {
      if (Srvc_GetBitU8_B(system_health_st, PREFLTR_ERROR_BIT) )
        Pre_Filter_Clog_st = 1 ;
      
      if (Srvc_GetBitU8_B(system_health_st, MEMBRANE_ERROR_BIT) )
        Membrane_Clog_st = 1;
      
      if (Srvc_GetBitU8_B(system_health_st, FLOW_ERROR_BIT) )
        Flow_st = 1 ;
      
      if (Srvc_GetBitU8_B(system_health_st, WLATER_LOW_BIT) )
        Water_lvl_st = 1 ;
      
      if (Srvc_GetBitU8_B(system_health_st, SYS_HEALTHY_BIT) )
        Healthy_st = 1;
      
//      system_health_st    = (system_health_st==32) ? 1 : 2 ;
      system_health_st    =  1 ;
      Dispenser_st        = (Dispenser_st == 1) ? 1 : 2 ;
      flush_st            = (flush_st == 1) ? 1 : 2 ;
      mode                = (mode == AUTO_MODE) ? 2 : 1 ;
      
    }
    else
    {
      system_health_st    = 1 ;
      Dispenser_st        = 2 ;
      flush_st            = 2 ;
      mode                = (mode == AUTO_MODE) ? 2 : 1 ;
    }

    sprintf(pumper_buff, "$%d$%d$%d$%d#\n" , system_health_st, Dispenser_st, flush_st, mode) ;
    len = strlen(pumper_buff) ;
    UART_TxData(pumper_buff, len ) ;

    
//#endif
  
}
