

/***************************************************************************************************
 * Includes
 ***************************************************************************************************
*/

#include "main_inf.h"
#include "global_variables.h"
//#include <shell.h>	       // include shell

/*
 ============================================================================
 Name        : System_Health_Proc.c
 Author      : SKN
 Version     :
 Copyright   :
 Description : For System Health Check...
 ============================================================================
 */

void System_Health_Proc(void)
{
  static uint8 itr_cnt ; 
  uint8 Dispens_st ;
  uint8 sys_health ;
  
   Dispens_st  = Get_Dispense_swt_st() ;       // Get Dispenser Status
  
   if( Dispens_st == DISPENSE)
   {
     itr_cnt++ ;
     
     if(itr_cnt >= HEALTH_CHECK_TIME_AFTER_MOTOR_ON_SEC )
     {
       itr_cnt = 0 ;
       Set_Health_Check (ENABLED) ;
       sys_health = Get_System_Health() ;
       Save_System_Health(sys_health) ;
       
       if( sys_health && (!(Srvc_GetBitU8_B(sys_health, SYS_HEALTHY_BIT))) )
       {
//         GPIO_Set(GPIOA, ERROR_LED_O_D, TRUE );
//         GPIO_Set(GPIOC, POWER_ON_LED_O_D, FALSE ); 
          GPIO_Set(GPIOC, POWER_ON_LED_O_D, TRUE ); 
       }
       else if ( Srvc_GetBitU8_B(sys_health, SYS_HEALTHY_BIT) )
       {
//         GPIO_Set(GPIOA, ERROR_LED_O_D, FALSE );
         GPIO_Set(GPIOC, POWER_ON_LED_O_D, TRUE ); 
       }
       
     }
     
   }
   else
     Set_Health_Check (DISABLED) ;
}
