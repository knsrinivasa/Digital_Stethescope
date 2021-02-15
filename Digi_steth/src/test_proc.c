
/*
 ============================================================================
 Name        : test_proc.c
 Author      : SKN
 Created on	 : Jan 10, 2014
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */


#include "main_inf.h"


//static uint8 i ;
//  char buff[16];

//sprintf(buff, "ADC_Ch val = %d\n",5);    

/*
  Clear_LCD() ;
  print_on_LCD("HELLO World") ;
  goto_cursor(LCD_START_OF_2LINE);            // goto second row
  
  buff[0] = 0x34 ;
  buff[1] = '\0' ;
  print_on_LCD(buff) ;
  

  buff[0] = 0x35 ;
  buff[1] = '\0' ;
  print_on_LCD(buff) ;
  
  buff[0] = 0x20 ;
  buff[1] = '\0' ;
  print_on_LCD(buff) ;
*/



//void Proc_test_GPIO(void)
//{
//  
//uint8 val = 0;
//static uint16 adc_val ;
//uint8     gpio_get_st5  ;
//uint8     gpio_get_st6  ;
//uint8     gpio_get_st7  ;
//uint8     gpio_get_st8  ;
//uint8     gpio_get_st9  ;
//uint8     gpio_get_st10 ;
//uint8     gpio_get_st11 ;
//uint8     gpio_get_st12 ;
//uint8     gpio_get_st13 ;
//uint8     gpio_get_st14 ;
//uint8     gpio_get_st15 ;
//
//	static uint8 i, len ;
//        char buff[25], rx_buff[25]={'\0'} ;
//
////    GPIO_Toggle(GPIOB,  GPIO_Pin_6);                
////    GPIO_Toggle(GPIOB,  GPIO_Pin_7);                
//
///*
//    GPIO_Toggle(GPIOB,  GPIO_Pin_2);                
////    GPIO_Set(GPIOB, GPIO_Pin_2, TRUE);
//    GPIO_Toggle(GPIOB,  GPIO_Pin_6);                
//    GPIO_Toggle(GPIOB,  GPIO_Pin_7);                
//
//    gpio_get_st5  = GPIO_Get(GPIOB, GPIO_Pin_5);
//    gpio_get_st6  = GPIO_Get(GPIOB, GPIO_Pin_6);
//    gpio_get_st7  = GPIO_Get(GPIOB, GPIO_Pin_7);
//    gpio_get_st8  = GPIO_Get(GPIOB, GPIO_Pin_8);
//    gpio_get_st9  = GPIO_Get(GPIOB, GPIO_Pin_9);
//    gpio_get_st10 = GPIO_Get(GPIOB, GPIO_Pin_10);
//    gpio_get_st11 = GPIO_Get(GPIOB, GPIO_Pin_11);
//    gpio_get_st12 = GPIO_Get(GPIOB, GPIO_Pin_12);
//    
//    gpio_get_st13 = GPIO_Get(GPIOB, GPIO_Pin_13);
//    gpio_get_st14 = GPIO_Get(GPIOB, GPIO_Pin_14);
//    gpio_get_st15 = GPIO_Get(GPIOB, GPIO_Pin_15);
//
//	GPIO_Toggle(GPIOC,  GPIO_PIN_0  );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_1  );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_2  );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_3  );
////	GPIO_Toggle(GPIOC,  GPIO_PIN_4  );
////	GPIO_Toggle(GPIOC,  GPIO_PIN_5  );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_6  );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_7  );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_8  );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_9  );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_10 );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_11 );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_12 );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_13 );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_14 );
//	GPIO_Toggle(GPIOC,  GPIO_PIN_15 );        
//    adc_val =  ADC1_Get(1);                   // PA4 ADC1->DR ;
//*/
//
////    Shell_Printf("ADC_Ch 4 val = %d \n",adc_val);    
///*
//  print_on_LCD("In Proc Test") ;
//    val = 30 ;
//  goto_cursor(0x00); 
//  print_on_LCD("Hello World") ;
//
////  goto_cursor(0x40); 
////  print_on_LCD("Welcome ") ;
////  goto_cursor(0x80); 
////  print_on_LCD("Sri") ;
//
//  buff[0] = 0x01 ;           // Clear Display  (Clear Display,Set DD RAM Address=0)
//  buff[1] = '\0' ;
//  Send_LCD_Block(0, buff) ;
//
//  //LCD_printf("ADC = %d  ",val);
////  goto_cursor(0x40); 
//  print_on_LCD("ADC = ") ;
//
//  buff[0] = 0x1C ;           // Clear Display  (Clear Display,Set DD RAM Address=0)
//  buff[1] = '\0' ;
//  Send_LCD_Block(0, buff) ;
//goto_cursor(0x40); 
//
//  buff[0] = 0x41 ;
//  buff[1] = 0x31 ;
//  buff[2] = '\0' ;
//  print_on_LCD(buff) ;
//
//  buff[0] = 0x41 ;
//  buff[1] = '\0' ;
//  print_on_LCD("buff4h4") ;
//
////  LCD_puts() ;
////  LCD_Print() ;
//  
//*/
//        i++;
////        UART_TxByte(++i);
//
//        sprintf(buff, "ADC_Ch val = %d\n", i);
//        len = strlen(buff) ;
//        UART_TxData(buff, len ) ;
////        UART_RxData(rx_buff, len) ;
////          UART_RxByte (&i);
////        Clear_LCD() ;
////        print_on_LCD(buff) ;
//
//}


void Proc_test_GPIO(void)
{
  uint8 i ;
  uint16 val;
  
  val = Get_Steth_Analog() ;
//
//    for (i=0; i<16; i++)
//  {
//    if(i != 15)
//    {
//      wave_first_line[i] = wave_first_line[i+1] ;
//      wave_first_line[i+1] = '_' ;
//    }
//    else
//      wave_first_line[i] = '_' ;
//  }
  
    Clear_LCD() ;
    sprintf(wave_first_line, "Val:%d V ", val );
    print_on_LCD(wave_first_line) ;
    goto_cursor(LCD_START_OF_2LINE);            // goto second row
    
  sprintf(BPM_second_line, "beats: %d ", beats );
  print_on_LCD(BPM_second_line) ;
  
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
    
    sprintf(BPM_second_line, "Beats:%d BPM:%d ", beats, bpm );
    print_on_LCD(BPM_second_line) ;
  }
  
  //  bpm_old = bpm ;
*/
  
}
