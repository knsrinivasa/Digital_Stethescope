

/***************************************************************************************************
 * Includes
 ***************************************************************************************************
*/
#include "cpu.h"

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
*/
#ifndef _MAIN_MACROS_H_
  #define _MAIN_MACROS_H_

/*------------------------------------------------------------------------*/
#define		data_type	uint8

#define         ABS_Diff(a,b)   (a>b ) ? (a-b) : (b-a) 

#define         FALSE		(0)
#define         TRUE		(!FALSE)

#define         VALID           (0)
#define         NOT_VALID	(!VALID)
#define         EXIT_SUCCESS    0
#define		DISABLED				0
#define		ENABLED					1
#define		HIGH				        0
#define		LOW				        1

#define 	OS_SCHED_SLOW_DOWN_RATE		        1
#define		PROC_INIT				0
#define		INI_SCHEDULED				1

#define		TASKS_1MS				1
#define		TASKS_2MS				2
#define		TASKS_10MS      			10
#define		TASKS_20MS      			20
#define		TASKS_100MS     			100
#define		TASKS_200MS     			200
#define		TASKS_1000MS    			1000
#define		TASKS_2000MS    			2000
#define 	DT					DT_TIME         //  TASKS_10MS

#define		REFERENCE_WATER_VOL_1LTR		1000
#define		REFERENCE_FS_PULSES_MIN			60
#define		REFERENCE_FS_PULSES_PER_1LTR	        3600

#define         REFERENCE_FLOW_VOL_RATE_LTR_PER_MIN     6
#define         REFERENCE_FS_CONSTANT                   8.1
#define         REFERENCE_FS_RATE_RASTER_SEC            60

#define		MAINS_OFF				0
#define		MAINS_ON				1

#define		SWT_OFF					0
#define		SWT_ON					1

#define		AUTO_MODE				0
#define		MANUAL_MODE				1
#define		VENDING_MODE				2
#define		FLUSH_NOW_MODE				3

#define		SEMI_AUTO_DISPENSE			DISABLED
#define		WATER_SENSOR_OPENED			0
#define		WATER_SENSOR_CLOSED			1
#define		DISPENSE_STOP				0
#define		DISPENSE				1
#define		WATER_LEVEL_LOW				100
#define		WATER_LEVEL_MID				110
#define		WATER_LEVEL_HIGH			111
#define		FUALTY_WATER_LVL_SENSOR		        0x7F

#define		FLUSH_STOP				0
#define		FLUSH_START				1
#define		SEMI_AUTO_FLUSH				DISABLED
#define		FUALTY_DP_SENSOR			0x7F
#define		PREFLTR_CLOGGED				10
#define		PREFLTR_CLEAN				11
#define		MEMBRANE_CLEAN				12
#define		MEMBRANE_CLOGGED			13
#define		PREFLTR_DP_CLOG_THRES_BAR	        3
#define		MEM_DP_CLOG_THRES_BAR		        3

#define         MAN_FLUSH_START_PRESS_BAR               3
#define         MAN_FLUSH_START_PUMP_PRE_TIME_S         10
#define         FLUSH_RUN_DUR_S                         5
#define         FLUSH_RUN_INITIATED                     0
#define         FLUSH_RUN_COMPLETED                     1

#define         PREFLTR_ERROR_BIT                       1   
#define         MEMBRANE_ERROR_BIT                      2
#define         FLOW_ERROR_BIT                          3
#define         WLATER_LOW_BIT                          4
#define         SYS_HEALTHY_BIT                         5
#define         HEALTH_CHECK_TIME_AFTER_MOTOR_ON_SEC    5
 
/*--------------------------------------------------*/
//#define GPIO_PIN_0                 GPIO_Pin_0  /*!< Pin 0 selected */
//#define GPIO_PIN_1                 GPIO_Pin_1 /*!< Pin 1 selected */
//#define GPIO_PIN_2                 GPIO_Pin_2  /*!< Pin 2 selected */
//#define GPIO_PIN_3                 GPIO_Pin_3  /*!< Pin 3 selected */
//#define GPIO_PIN_4                 GPIO_Pin_4  /*!< PIN 4 selected */
//#define GPIO_PIN_5                 GPIO_Pin_5  /*!< Pin 5 selected */
//#define GPIO_PIN_6                 GPIO_Pin_6  /*!< Pin 6 selected */
//#define GPIO_PIN_7                 GPIO_Pin_7  /*!< Pin 7 selected */
//#define GPIO_PIN_8                 GPIO_Pin_8  /*!< Pin 8 selected */
//#define GPIO_PIN_9                 GPIO_Pin_9  /*!< Pin 9 selected */
//#define GPIO_PIN_10                GPIO_Pin_10  /*!< Pin 10 selected */
//#define GPIO_PIN_11                GPIO_Pin_11 /*!< Pin 11 selected */
//#define GPIO_PIN_12                GPIO_Pin_12  /*!< Pin 12 selected */
//#define GPIO_PIN_13                GPIO_Pin_13  /*!< Pin 13 selected */
//#define GPIO_PIN_14                GPIO_Pin_14  /*!< Pin 14 selected */
//#define GPIO_PIN_15                GPIO_Pin_15  /*!< Pin 15 selected */

//#define I_A_DRVMOTCURR_LT  ADC1_Channel_4      /**<ADC channel 4 */
//#define V_A_DRVMOTVLTG_LT  ADC1_Channel_5      //ADC channel 5
//#define I_A_DRVMOTCURR_RT  ADC1_Channel_8      //ADC channel 8
//#define V_A_DRVMOTVLTG_RT  ADC1_Channel_9      //ADC channel 9
/*--------------------------------------------------*/

#define		NOT_PRESSED		0
#define         KEYPAD_1                1
#define         KEYPAD_2                2
#define         KEYPAD_3                3
#define         KEYPAD_4                4
#define         KEYPAD_5                5
#define         KEYPAD_6                6
#define         KEYPAD_7                7
#define         KEYPAD_8                8
#define         DISPLAY_STATE_DATA                      0
#define         DISP_DEBUG_SENSOR_DATA                  1
#define         DISP_STATE_VAL                          3

  #define       PreFilter_Us_I_A        ADC1_Channel_1      //PA ADC channel 1 
  #define       PreFilter_Ds_I_A        ADC1_Channel_2      //PA ADC channel 2 
  #define       MemFilter_Ds_I_A        ADC1_Channel_3      //PA ADC channel 3 

  #define	Dispense_swt_O_D	GPIO_PIN_2               // GPIOB
  #define	Flush_swt_O_D		GPIO_PIN_3               // GPIOB

//Sri Pending
  #define       POWER_ON_LED_O_D        GPIO_PIN_0                // GPIOC
  #define       ERROR_LED_O_D           GPIO_PIN_5                // GPIOA
  #define       Motor_swt_O_D		GPIO_PIN_12               // GPIOA

//  #define	Mains_swt_I_D		GPIO_PIN_5              // GPIOB
//  #define	Manual_swt_I_D		GPIO_PIN_6              // GPIOB

  #define	Manual_swt_I_D		GPIO_PIN_5              // GPIOB
  #define	Pump_swt_I_D		GPIO_PIN_6              // GPIOB
  #define	Man_Flush_swt_I_D	GPIO_PIN_7              // GPIOB
  #define	Water_LL_I_D		GPIO_PIN_8              // GPIOB
  #define	Water_ML_I_D		GPIO_PIN_9              // GPIOB
  #define	Water_HL_I_D		GPIO_PIN_10              // GPIOB

  #define	KEYPAD1_I_D		GPIO_Pin_11              // GPIOB
  #define	KEYPAD2_I_D		GPIO_Pin_12              // GPIOB
  #define	KEYPAD3_I_D		GPIO_Pin_13              // GPIOB
  #define	KEYPAD4_I_D		GPIO_Pin_14              // GPIOB

#define         NO_SCAPE                0
#define         SCAPE                   1
#define         BOARD                   SCAPE

// 16x2 LCD pin Configuration
#if (BOARD == NO_SCAPE)
  #define               LCD_PORT                GPIOC
  #define		LCD_RS_PIN		GPIO_Pin_1		// GPIOC
  #define		LCD_EN_PIN		GPIO_Pin_2		// GPIOC
  #define		LCD_D0_PIN		GPIO_Pin_3		// GPIOC
  #define		LCD_D1_PIN		GPIO_Pin_6		// GPIOC
  #define		LCD_D2_PIN		GPIO_Pin_7		// GPIOC
  #define		LCD_D3_PIN		GPIO_Pin_8		// GPIOC
  #define		LCD_D4_PIN		GPIO_Pin_9		// GPIOC
  #define		LCD_D5_PIN		GPIO_Pin_10		// GPIOC
  #define		LCD_D6_PIN		GPIO_Pin_11		// GPIOC
  #define		LCD_D7_PIN		GPIO_Pin_12		// GPIOC
#elif (BOARD == SCAPE)
  #define               LCD_PORT                GPIOC
  #define		LCD_RS_PIN		GPIO_Pin_1		// GPIOC
  #define		LCD_EN_PIN		GPIO_Pin_2		// GPIOC
  #define		LCD_D0_PIN		GPIO_Pin_3		// GPIOC
  #define		LCD_D1_PIN		GPIO_Pin_12		// GPIOC
  #define		LCD_D2_PIN		GPIO_Pin_11		// GPIOC
  #define		LCD_D3_PIN		GPIO_Pin_10		// GPIOC
  #define		LCD_D4_PIN		GPIO_Pin_9		// GPIOC
  #define		LCD_D5_PIN		GPIO_Pin_8		// GPIOC
  #define		LCD_D6_PIN		GPIO_Pin_7		// GPIOC
  #define		LCD_D7_PIN		GPIO_Pin_6		// GPIOC
#endif

#define         RCC_APB2Periph_GPIO_LCD    RCC_APB2Periph_GPIOC

#define		PREFILTER_US		1
#define		PREFILTER_DS		2
#define		MEMFILTER_DS		3

#define		IDENTIFIED		TRUE
#define		UNIDENTIFIED		FALSE
#define		MIN_BALANCE		100UL

#define         DISPENSE_RESET_SELECTION                0
#define         DISPENSE_SET_ZERO                       0
#define         DISPENSE_INCR_DECR_PRE_SET_LTS          1
#define         DISPENSE_INCR_DECR_PRE_SET_PRICE        5
#define         DISPENSE_MAX_PRE_SET_LTS                50
#define         DISPENSE_MAX_PRE_SET_PRICE              500

#define		UNIT_VOL		1
#define		UNIT_RS			2

#define 	VALVE_POST_STOP_QUANT 	0UL

#define		MAX_SAVED_CUSTOMERS	2

#define		WELCOME_DISP_DUR_200MS_FACTOR	        0
#define		LCD_START_OF_1LINE                      0x80
#define		LCD_START_OF_2LINE                      0x40

/*------------------------------------------------------------------------*/

enum Screens{
        NO_SCREEN = 0 ,
	WELCOME_SCREEN,
	HOME_SCREEN,
	DIAG_INFO_SCREEN,
	ABOUT_SYSTEM_SCREEN,
	FLUSH_NOW_SCREEN,
	FLUSH_NOW_STATE_SCREEN,
	MODE_SELECTION_SCREEN,
	MANUAL_SCREEN,
	MANUAL_STATE_SCREEN,
	AUTO_SCREEN,
	VENDING_SCREEN,
	VENDING_FIELD_ENTRY_SCREEN,
	VENDING_STATE_SCREEN,
	ABOUT_SYSTEM_HEALTH_SCREEN,
	ABOUT_FLUSH_NOW_MODE_SCREEN,
	ABOUT_MANUAL_MODE_SCREEN,
	ABOUT_AUTO_MODE_SCREEN
};
                                
enum User_Buttons{
	NOT_ASSIGNED = 0,
	DIAG_INFO,
	MODE_SELECT,
	MANUAL_MODE_KEY,
	MANUAL_SWT_ON,
	MANUAL_SWT_OFF,
	AUTO_MODE_KEY,
	FILL_NOW,
	VENDING_MODE_KEY,
	QUANTITY_FIELD,
	PRICE_FIELD,
	SELECT_FIELD_UNIT,
	INCREMENT_STEP,
	DECREMENT_STEP,
	SELECT_FIELD_ENTRY_VALUE,
	FLUSH_NOW_MODE_KEY,
	FLUSH_SWT_ON,
	FLUSH_SWT_OFF,
	ABOUT_SYSTEM,
	ABOUT_SYSTEM_HEALTH_MSGS,
	ABOUT_MANUAL_MODE,
	ABOUT_AUTO_MODE,
	ABOUT_FLUSH_NOW_MODE,
	BACK_TO_DIAG_INFO_SCREEN,
	BACK_TO_HOME_SCREEN,
	BACK_TO_AUTO_SCREEN,
	BACK_TO_MODE_SELECTION_SCREEN,
	BACK_TO_MANUAL_SCREEN,
	BACK_TO_VENDING_SCREEN,
	BACK_TO_VENDING_FIELD_ENTRY_SCREEN,
	BACK_TO_FLUSH_NOW_SCREEN
};


#define   BIT_4_MODE        4
#define   BIT_8_MODE        8
#define   LCD_OP_BIT_MODE   BIT_8_MODE

#if (LCD_OP_BIT_MODE == BIT_4_MODE)
  #define   LCD_16x2_Init     LCD_16x2_4Bit_Init
  #define   Send_LCD_Block    Send_LCD_Block_4Bit
#elif (LCD_OP_BIT_MODE == BIT_8_MODE)
  #define   LCD_16x2_Init     LCD_16x2_8Bit_Init
  #define   Send_LCD_Block    Send_LCD_Block_8Bit
#endif

#endif
