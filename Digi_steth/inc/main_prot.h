/*
 * main_prot.h
 *
 *  Created on: Feb 5, 2013
 *      Author: SKN
 */

#ifndef MAIN_PROT_H_
  #define MAIN_PROT_H_

//#include "global_variables.h"

/***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
*/
void Delay(uint32_t nTime) ;

void SELECT_Screen(uint8 *u8currentScreen, uint8 *u8userButton) ;
void Display_Screen(uint8 u8currentScreen) ;

void Set_Sys_Mode(uint8 mode)                   ;
uint8 Get_Sys_Mode(void)                        ;
uint8 Get_Mains_swt_st(void)			;
//uint8 Get_Run_Mode_sel_st(void) 		;
uint8 Get_Manual_swt_st(void)			;
void Set_Manual_swt_st(uint8 man_swt_st)        ;
void Set_Dispense_swt_st (uint8, uint8)		;
void Toggle_Flush_swt_st(void)                  ;
void Toggle_Pump_swt_st(void)                   ;
void Set_Pump_swt_st(uint8 pump_st)             ;
uint8 Get_Pump_swt_st(void)                     ;
uint8 Get_Manual_Flush_swt_st(void)             ;
void Set_Manual_Flush_swt_st(uint8 flush_swt)   ;

uint8 Get_Water_lvl(void)			;
void Auto_Dispense(void)			;
void Manual_Semi_Auto_Dispense(void)	        ;
void Vending_Mode(void)				;
void Cost_Vol_Map(user_intput_st *)		;

void get_user(Customer_struct *customer)  					;
uint8 Autenticate(Customer_struct *Customers, Customer_struct *customer)	;
uint8 update_customer_data(Customer_struct *customer)  				;
void Dispense_On_Request(float request_vol)  					;
float Get_Flow_sens_Vol(uint8 mode);
void Set_Flush_Mode_St(uint8 flush_st)  					;
uint8 Get_Flush_Mode_St(void)							;
void Flush_Now(void)  								;
void Flush_Auto(void)  								;
uint8 Get_Mem_delta_P_lvl(void)							;
float ADC_Pres_Conv(uint16 adc_val, uint8 pres_sens_idx)                        ;
float Get_PreFilter_PS_upstream(void)                                           ;
float Get_PreFilter_PS_downstream(void)                                         ;
float Get_MEM_Filter_Ds_post(void)                                              ;
uint32 Get_FS_pulse_cnt(void)                                                   ;
uint8 Get_Dispense_swt_st (void)                                                ;
uint8 Get_System_Health(void)                                                   ;
void Save_System_Health(uint8 sys_health)                                       ;
uint8 Load_System_Health(void)                                                  ;
void Set_Health_Check (uint8 enbl_cond)                                         ;
uint8 Get_Health_Check(void)                                                    ;
void Set_Display_reqeust_key_st (uint8 disp_req_key)                            ;
uint8 Get_Display_reqeust_key_st(void)                                          ;
void Set_Flush_Run_Complete_st( uint8 flush_run_st)                             ;
uint8 Get_Flush_Run_Complete_st(void)                                           ;

void SYSCFG_EXTILineConfig(uint8_t , uint8_t )                                  ;
void EXTI_Interrupt_Init(void)                                                  ;
void EXTI0_IRQHandler(void)                                                     ;


void SysTick_CounterCmd (uint32 SysTick_Counter) ;
void lcd_init(void);                                    // Initial LCD
void lcd_out_data4(unsigned char);              // Strobe 4-Bit Data to LCD
void lcd_write_byte(unsigned char);             // Write 1 Byte Data to LCD
void lcd_write_control(unsigned char);  // Write Instruction
void lcd_write_ascii(unsigned char);    // Write LCD Display(ASCII)
void goto_cursor(unsigned char);                // Set Position Cursor LCD
void lcd_print(unsigned char*);                 // Print Display to LCD
void enable_lcd(void);                                  // Enable Pulse
uint8 busy_lcd(void) ;

void LCD_16x2_4Bit_Init(void) ;
void LCD_16x2_8Bit_Init(void) ;
void Send_LCD_Block_4Bit(uint8 cmd_data_fld, char* str) ;
void Send_LCD_Block_8Bit(uint8 cmd_data_fld, char* str) ;
void LCD_Print() ;
void print_on_LCD(char *str);
void lcd_write_control(unsigned char val) ;
void LCD_EN_Ctrl(uint8 EN_pin_value);
void LCD_RS_Ctrl(uint8 RS_pin_value);

void Enable_Disable_LCD(uint8 EN_pin_value) ;
void RS_LCD(uint8 RS_pin_value) ;
void Send_LCD_Cmd_Block(void) ;
void Send_LCD_Data_Block(void) ;
void Send_LCD_Block(uint8 cmd_data_fld, char* str) ;
void goto_cursor(unsigned char i) ;
void Clear_LCD(void) ;
void Scroll_LCD_Left(void) ;
void Scroll_LCD_Right(void) ;
void Cursor_to_Home(void) ;
void Cursor_shift_Left (void) ;
void Cursor_shift_Right (void) ;

uint16 Get_Steth_Analog(void) ;
float ADC_Beat_Conv(uint16 adc_val, uint8 pres_sens_idx);

#endif /* MAIN_PROT_H_ */

