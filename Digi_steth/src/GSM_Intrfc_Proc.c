

/***************************************************************************************************
 * Includes
 ***************************************************************************************************
*/

#include "main_inf.h"
#include "global_variables.h"
//#include <shell.h>	       // include shell

/*
 ============================================================================
 Name        : WP_GSM_Intrfc_Proc.c
 Author      : SKN
 Version     :
 Copyright   :
 Description : For pumping Data on UART for Mobile phone app...
 ============================================================================
 */

   static char buff[25] ;

void WP_GSM_Intrfc_Proc(void)
{
/*---------*/
static uint8 sms_st;
//static uint8 end_var ;
//static uint8 system_health_st1 ;
        


        sms_st++ ;        
        switch(sms_st)
        {
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
            sprintf(buff, "AT\n");
            UART_TxData(buff, strlen(buff) ) ;
            break ;  
            
          case 7:
            sprintf(buff, "AT+CMGF=1\n");
            UART_TxData(buff, strlen(buff) ) ;
            break ;  
            
          case 8:
            //sprintf(buff, "AT+CMGS=\"+919449750734\"\n");
            sprintf(buff, "ATD+919449750734;\n");
            UART_TxData(buff, strlen(buff) ) ;
            break ;

/*
          case 7:
            sprintf(buff, "Test Message 1\n");
            UART_TxData(buff, strlen(buff) ) ;
            break ;

          case 8:
            end_var = 0x1A ;
            sprintf(buff, "%d\n", end_var);
            UART_TxData(buff, strlen(buff) ) ;
            break ;  
*/
          }
//          system_health_st = Get_System_Health() ;
//          if(system_health_st)
/*
            system_health_st1 = system_health_st1 ^ 1 ;
            GPIO_Set(GPIOA, GPIO_PIN_4, system_health_st1 );
            GPIO_Set(GPIOA, GPIO_PIN_12, system_health_st1 );
*/            
/*---------*/        
  
}
