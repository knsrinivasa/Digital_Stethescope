
/*
 ============================================================================
 Name        : FS_pulse_Cntr.c
 Author      : SKN
 Created on	 : Jan 10, 2014
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */


#include "main_inf.h"

#define enableInterrupts()   __set_PRIMASK(0);
#define disableInterrupts()  __set_PRIMASK(1);

#define EXTI_PortSourceGPIOC       ((uint8_t)0x02)
#define EXTI_PinSource0            ((uint8_t)0x00)
#define EXT_INT_GPIO_PORT	   GPIOC
#define EXT_INT_GPIO_PIN           GPIO_Pin_0


/**************************************************************************************************/
 /*          In order to use an I/O pin as an external interrupt source, follow
  *          steps below:
  *            1- Configure the I/O in input mode using GPIO_Init()
  *            2- Select the input source pin for the EXTI line using 
  *               SYSCFG_EXTILineConfig()
  *            3- Select the mode(interrupt, event) and configure the trigger 
  *               selection (Rising, falling or both) using EXTI_Init()
  *            4- Configure NVIC IRQ channel mapped to the EXTI line using NVIC_Init()
  *   
  *@note     SYSCFG APB clock must be enabled to get write access to SYSCFG_EXTICRx
  *          registers using RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  */
/**************************************************************************************************/
/**************************************************************************************************/
/**
  * @brief  Selects the GPIO pin used as EXTI Line.
  * @param  EXTI_PortSourceGPIOx : selects the GPIO port to be used as source 
  *                                for EXTI lines where x can be (A, B, C, D, E or H).
  * @param  EXTI_PinSourcex: specifies the EXTI line to be configured.
  *         This parameter can be EXTI_PinSourcex where x can be (0..15)
  * @retval None
  */
/**************************************************************************************************/


void EXTI_Interrupt_Init(void)
{
    GPIO_InitTypeDef GPIO_EXTI_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
  
      /* Configure Port-A Pin0 pin as input *
  GPIO_EXTI_InitStructure.GPIO_Pin = EXT_INT_GPIO_PIN;
  GPIO_EXTI_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_EXTI_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_EXTI_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;

  //GPIO_Init(EXT_INT_GPIO_PORT, &GPIO_EXTI_InitStructure);
    
      /* Select Port-A Pin 0 as input source for EXTI Line */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);


  /* Configure EXT1 Line 0 in interrupt mode trigged on Rising edge */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0 ;  // PA0 for User button AND IDD_WakeUP
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        // EXTI_Trigger_Rising_Falling = 0x10   EXTI_Trigger_Falling = 0x0C,  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);


  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 

}

/**************************************************************************************************/

void EXTI0_IRQHandler(void)
{
  /* Disable general interrupts */
  disableInterrupts();

//    FS_Pulse_Cnt_curr+=1;
  
    beats++;  
    wave_first_line[15] = '^' ;
  
  /* Pulse Flow usage activated*/ 
//Shell_Printf("Pulse Flow Activate");    
  
  EXTI_ClearITPendingBit(EXTI_Line0);
  enableInterrupts();
}


/**************************************************************************************************/
