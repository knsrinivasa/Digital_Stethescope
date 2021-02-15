
/*
 ============================================================================
 Name        : DE_swt_srv.c
 Author      : SKN
 Created on	 : Jan 7, 2014
 Version     :
 Copyright   :
 Description :
 ============================================================================
*/

#include "main_inf.h"

static uint8 Dispense_swt_st            ;
static uint8 Flush_mode_swt_st          ;
static uint8 Flush_ON_OFF_swt_st        ;
static uint8 Man_Auto_Mode_Swt_st       ;
//static uint8 Pump_ON_OFF_swt_st         ;
static uint8 Pump_ON_OFF_swt_Man_st     ;
static uint8 Pump_ON_OFF_swt_Auto_st    ;
static uint8 Sys_Mode                   ;
static uint8 Sys_Health_St              ;
static uint8 Health_Check_St            ;
static uint8 Display_Request_Key_St     ;
static uint8 Flush_Run_complete_st      ;

/*----------------------------------------------------*/

void Set_Sys_Mode(uint8 mode)
{
  Sys_Mode = mode;
}

/*----------------------------------------------------*/

uint8 Get_Sys_Mode(void)
{
  return Sys_Mode ;
}

/*----------------------------------------------------*/

uint8 Get_Mains_swt_st(void)
{
//	uint8 mains_swt ;
//        mains_swt = GPIO_Get(GPIOB, Mains_swt_I_D);
//	return mains_swt ;
  
        return 1 ;
}

/*----------------------------------------------------*/

void Set_Display_reqeust_key_st (uint8 disp_req_key)
{
  Display_Request_Key_St = disp_req_key ;
}

/*----------------------------------------------------*/

uint8 Get_Display_reqeust_key_st(void)
{
  return Display_Request_Key_St ;
}
/*----------------------------------------------------*/

uint8 Get_Pump_swt_st(void)
{
  if( Sys_Mode == MANUAL_MODE)
    return Pump_ON_OFF_swt_Man_st ;
  else if(Sys_Mode == AUTO_MODE)
    return Pump_ON_OFF_swt_Auto_st ;  
}

/*----------------------------------------------------*/

void Set_Pump_swt_st(uint8 pump_st)
{  
  if( Sys_Mode == MANUAL_MODE)
    Pump_ON_OFF_swt_Man_st = pump_st ;
  else if(Sys_Mode == AUTO_MODE)
    Pump_ON_OFF_swt_Auto_st = pump_st ;
}

/*----------------------------------------------------*/

void Toggle_Pump_swt_st(void)
{
  if( Sys_Mode == MANUAL_MODE)
    Pump_ON_OFF_swt_Man_st = Pump_ON_OFF_swt_Man_st ^ 1 ;
  else if(Sys_Mode == AUTO_MODE)
    Pump_ON_OFF_swt_Auto_st = Pump_ON_OFF_swt_Auto_st ^ 1 ;
  
}

/*----------------------------------------------------*

uint8 Get_Run_Mode_sel_st(void)
{
	data_type Run_Mode_sel_st;

	puts("Enter Mode selection\n\tManual Mode\t( 1 )\n\tAuto Mode\t( 2 )\n\tVending Mode\t( 3 )\n\tFlush Now\t( 4 )\n") ;
	scanf("%d", &Run_Mode_sel_st) ;

	return Run_Mode_sel_st ;
}

*----------------------------------------------------*/

uint8 Get_Manual_swt_st(void)
{
  return Man_Auto_Mode_Swt_st ;
}

/*----------------------------------------------------*/

void Set_Manual_swt_st(uint8 man_swt_st)
{
  Man_Auto_Mode_Swt_st = man_swt_st;
}


/*----------------------------------------------------*/

uint8 Get_Manual_Flush_swt_st(void)
{
//	uint8 man_flush_swt_st ;
//	man_flush_swt_st = DIO_Get (Man_Flush_swt_I_D) ;
//        man_flush_swt_st = GPIO_Get(GPIOB, Man_Flush_swt_I_D);
//	return man_flush_swt_st ;
  
  return Flush_ON_OFF_swt_st ;
}

/*----------------------------------------------------*/

void Set_Manual_Flush_swt_st(uint8 flush_swt)
{
  Flush_ON_OFF_swt_st  = flush_swt ;
}

/*----------------------------------------------------*/

void Toggle_Flush_swt_st(void)
{
    Flush_ON_OFF_swt_st = Flush_ON_OFF_swt_st ^ 1 ;
}


/*----------------------------------------------------*/

void Set_Dispense_swt_st (uint8 dispense_st, uint8 mode)
{
  static uint8 itrn_cnt = 0 ;
  
	Dispense_swt_st = dispense_st ;

//        GPIO_Set(GPIOB, Dispense_swt_O_D, dispense_st );
        GPIO_Set(GPIOA, Motor_swt_O_D, dispense_st );

        
        if(dispense_st)
        {
          GPIO_Set(GPIOB, Flush_swt_O_D, (!dispense_st) );
        }
        
        
        if ( (mode == VENDING_MODE) && (dispense_st == DISPENSE_STOP) )
        {
          user_intput.price       =  DISPENSE_SET_ZERO    ;
          user_intput.vol         =  DISPENSE_SET_ZERO    ;
          user_intput.unit        =  DISPENSE_SET_ZERO    ;
          value_set               =  DISPENSE_SET_ZERO    ;
          unit_select             =  DISPENSE_SET_ZERO    ;                                        
          FS_Pulse_Cnt_curr = 0 ;
          FS_Pulse_Cnt_Total = 0 ;
          flow_sensor_vol = 0 ;
        }
        
        if( (dispense_st == DISPENSE_STOP) && (itrn_cnt >= 5) )
        {
          itrn_cnt = 0 ;
          Flow_vol_Disp = 0 ;
        }

        itrn_cnt++ ;
}

/*----------------------------------------------------*/

uint8 Get_Dispense_swt_st (void)
{
  return Dispense_swt_st ;
}

/*----------------------------------------------------*/

float Get_Flow_sens_Vol(uint8 mode)
{
	uint32 flow_sens_pulse_cnt;
        uint32 flow_now_count;
	static float flow_vol ;

//	Pulse to Volume conversion pending
	// count pulses
	// convert to Vol
// F(Hz)= 8.1*flow rate(Q)-6(l/min)
        
	flow_sens_pulse_cnt = Get_FS_pulse_cnt() ;
        flow_now_count = flow_sens_pulse_cnt  ;
//	flow_vol = ( REFERENCE_WATER_VOL_1LTR * flow_sens_pulse_cnt * DT_TIME ) / (REFERENCE_FS_PULSES_MIN * REFERENCE_FS_PULSES_PER_1LTR) ;
  
//        flow_vol = (DT_TIME) * ( ( flow_sens_pulse_cnt + REFERENCE_FLOW_VOL_RATE_LTR_PER_MIN) / (REFERENCE_FS_CONSTANT * REFERENCE_FS_RATE_RASTER_SEC) ) ;
        flow_vol = (1) * ( ( flow_sens_pulse_cnt + REFERENCE_FLOW_VOL_RATE_LTR_PER_MIN) / (REFERENCE_FS_CONSTANT * REFERENCE_FS_RATE_RASTER_SEC) ) ;
        flow_sens_pulse_cnt = 0;
/*        
        if(mode == 2)
          return 1 ;
        else if(mode == 1)
          return flow_now_count  ;     // Sri pending
*/       
        Flow_vol_Disp+= flow_vol ;
        
        return flow_vol  ;
}

/*----------------------------------------------------*/

uint32 Get_FS_pulse_cnt(void)           // Sri Pending
{
  
  FS_Pulse_Cnt_Total = FS_Pulse_Cnt_curr;
  FS_Pulse_Cnt_curr = 0 ;

  return FS_Pulse_Cnt_Total ;
        
}


/*----------------------------------------------------*/

void Set_Flush_Run_Complete_st( uint8 flush_run_st)
{
  Flush_Run_complete_st = flush_run_st ;
}

/*----------------------------------------------------*/

uint8 Get_Flush_Run_Complete_st(void)
{
  return Flush_Run_complete_st ;
}

/*----------------------------------------------------*/


void Set_Flush_Mode_St(uint8 flush_st)
{
  static uint8 Flush_start_delay = 0 ;
  static uint8 flush_init = 0 ;
  static uint8 Flush_Run_Dur = 0;
  float post_mem_pres ;
  uint8 flush_run_st    ;
  
	Flush_mode_swt_st = flush_st ;
        post_mem_pres = Get_MEM_Filter_Ds_post() ;
        flush_run_st = Get_Flush_Run_Complete_st() ;
  
        if( flush_run_st != FLUSH_RUN_COMPLETED)
        {
          if( (flush_init == 0) && (flush_st==TRUE) && ( ( Flush_start_delay < MAN_FLUSH_START_PUMP_PRE_TIME_S ) || (post_mem_pres <= MAN_FLUSH_START_PRESS_BAR) ) )
          {
            Flush_start_delay++ ;
            GPIO_Set(GPIOB, Flush_swt_O_D, (!flush_st) );
            GPIO_Set(GPIOB, Dispense_swt_O_D, flush_st );
            GPIO_Set(GPIOA, Motor_swt_O_D, flush_st );
          }
          
          if( (flush_st==TRUE) && ( ( Flush_start_delay >= MAN_FLUSH_START_PUMP_PRE_TIME_S ) || (post_mem_pres >= MAN_FLUSH_START_PRESS_BAR) ) )
          {
            if(Flush_Run_Dur <= FLUSH_RUN_DUR_S)
            {
              Flush_Run_Dur++ ;
              
              flush_init = 1 ;
              GPIO_Set(GPIOB, Flush_swt_O_D, flush_st );
              GPIO_Set(GPIOB, Dispense_swt_O_D, flush_st );
              GPIO_Set(GPIOA, Motor_swt_O_D, (!flush_st) );
            }
            else
            {
              Flush_Run_Dur = 0 ;
              Set_Flush_Run_Complete_st ( FLUSH_RUN_COMPLETED );
              
              GPIO_Set(GPIOB, Flush_swt_O_D, FALSE );
              GPIO_Set(GPIOB, Dispense_swt_O_D, FALSE );
              GPIO_Set(GPIOA, Motor_swt_O_D, FALSE );              
            }
            
          }
          
        }         
//          if( (flush_st==TRUE) && ( ( Flush_start_delay >= MAN_FLUSH_START_PUMP_PRE_TIME_S ) && (post_mem_pres < MAN_FLUSH_START_PRESS_BAR) ) )                    
          
            if(!flush_st)
          { 
            Flush_start_delay = 0 ;
            flush_init = 0 ;
            Set_Flush_Run_Complete_st ( FLUSH_RUN_INITIATED );
            GPIO_Set(GPIOB, Flush_swt_O_D, flush_st );
            GPIO_Set(GPIOB, Dispense_swt_O_D, flush_st );
            GPIO_Set(GPIOA, Motor_swt_O_D, flush_st );            
          }
                               
}

/*----------------------------------------------------*/

uint8 Get_Flush_Mode_St()
{
	return Flush_mode_swt_st ;
}

/*----------------------------------------------------*/


uint8 Get_Water_lvl(void)
{
	uint8 water_lvl;
	uint8 water_lvl_low;
	uint8 water_lvl_mid;
	uint8 water_lvl_high;

//	water_lvl_low	=	DIO_Get (Water_LL_I_D) ;
//	water_lvl_mid	=	DIO_Get (Water_ML_I_D) ;
//	water_lvl_high	=	DIO_Get (Water_HL_I_D) ;

	water_lvl_low	= GPIO_Get (GPIOB, Water_LL_I_D);
	water_lvl_mid	= GPIO_Get (GPIOB, Water_ML_I_D);
	water_lvl_high	= GPIO_Get (GPIOB, Water_HL_I_D);
        
        
	if( (water_lvl_low == WATER_SENSOR_OPENED) && (water_lvl_mid == WATER_SENSOR_OPENED) && (water_lvl_high == WATER_SENSOR_OPENED) )
        {
          water_lvl = WATER_LEVEL_LOW ;
        }

	else if (
                  ( (water_lvl_low == WATER_SENSOR_CLOSED) && (water_lvl_mid == WATER_SENSOR_OPENED) && (water_lvl_high == WATER_SENSOR_OPENED) ) ||
                  ( (water_lvl_low == WATER_SENSOR_CLOSED) && (water_lvl_mid == WATER_SENSOR_CLOSED) && (water_lvl_high == WATER_SENSOR_OPENED) )
		)
        {
		water_lvl = WATER_LEVEL_MID ;
        }
	else if ( (water_lvl_low == WATER_SENSOR_CLOSED) && (water_lvl_mid == WATER_SENSOR_CLOSED) && (water_lvl_high == WATER_SENSOR_CLOSED) )
        {
          water_lvl = WATER_LEVEL_HIGH ;
        }

/*	else if (
			( (water_lvl_low == WATER_SENSOR_OPENED) && (water_lvl_mid == WATER_SENSOR_CLOSED) && (water_lvl_high == WATER_SENSOR_CLOSED) ) ||
			( (water_lvl_low == WATER_SENSOR_OPENED) && (water_lvl_mid == WATER_SENSOR_CLOSED) && (water_lvl_high == WATER_SENSOR_OPENED) ) ||
			( (water_lvl_low == WATER_SENSOR_OPENED) && (water_lvl_mid == WATER_SENSOR_OPENED) && (water_lvl_high == WATER_SENSOR_CLOSED) ) ||
			( (water_lvl_low == WATER_SENSOR_CLOSED) && (water_lvl_mid == WATER_SENSOR_OPENED) && (water_lvl_high == WATER_SENSOR_CLOSED) )
			)
*/
	else
	{
//LCD	  puts("\t___________________________________\n\t!!! Fualty Water Level Sensor !!!\n\t___________________________________\n") ;
	  water_lvl = FUALTY_WATER_LVL_SENSOR ;
	}

	return water_lvl ;
}


/*----------------------------------------------------*/

void Manual_Semi_Auto_Dispense(void)
{
	data_type manual_swt_st ;
        uint8   Pump_swt_st;
        
        #if(SEMI_AUTO_DISPENSE == ENABLED)
          data_type curr_water_lvl;
        #endif
          
	manual_swt_st = Get_Manual_swt_st() ;

	if(manual_swt_st==SWT_ON)               // Manual Mode
	{
		#if(SEMI_AUTO_DISPENSE == ENABLED)
                  curr_water_lvl = Get_Water_lvl() ;

                  if(curr_water_lvl == FUALTY_WATER_LVL_SENSOR)
                          Set_Dispense_swt_st (DISPENSE_STOP, MANUAL_MODE) ;
                  else if(curr_water_lvl != WATER_LEVEL_HIGH)
                          Set_Dispense_swt_st (DISPENSE, MANUAL_MODE) ;
                  else if(curr_water_lvl == WATER_LEVEL_HIGH)
                          Set_Dispense_swt_st (DISPENSE_STOP, MANUAL_MODE) ;

		#else
                  
                  Pump_swt_st = Get_Pump_swt_st() ;
                  if (Pump_swt_st == SWT_ON)
                    Set_Dispense_swt_st (DISPENSE, MANUAL_MODE) ;
                  else
                    Set_Dispense_swt_st (DISPENSE_STOP, MANUAL_MODE) ;
		#endif
	}
	else	//	if(dispense_sw_st==SWT_OFF)     //      Auto mode
	{
		Set_Dispense_swt_st (DISPENSE_STOP, MANUAL_MODE) ;
	}

}

/*----------------------------------------------------*/

void Auto_Dispense(void)
{
	data_type curr_water_lvl;

#if(SEMI_AUTO_DISPENSE == ENABLED)
	data_type manual_swt_st ;

	manual_swt_st = Get_Manual_swt_st() ;
	if(manual_swt_st == SWT_ON)
          Manual_Semi_Auto_Dispense() ;
#endif

	curr_water_lvl = Get_Water_lvl() ;

	if(curr_water_lvl == WATER_LEVEL_LOW)
		Set_Dispense_swt_st (DISPENSE, AUTO_MODE) ;
//	else if ( (curr_water_lvl == WATER_LEVEL_MID) && (dir==OFF_ON) )
//		Set_Dispense_swt_st (DISPENSE, AUTO_MODE) ;
	else if(curr_water_lvl == WATER_LEVEL_HIGH)
		Set_Dispense_swt_st (DISPENSE_STOP, AUTO_MODE) ;
	else if(curr_water_lvl == FUALTY_WATER_LVL_SENSOR)
		Set_Dispense_swt_st (DISPENSE_STOP, AUTO_MODE) ;

}

/*----------------------------------------------------*/

void Vending_Mode(void)
{
	Customer_struct Customer_read ;
	//user_intput_st	user_intput ;
	float	dispense_volume;
	uint8	auth_st ;


	get_user(&Customer_read) ;
	auth_st = Autenticate(Customers, &Customer_read);

	if (auth_st == IDENTIFIED)
	{
		Cost_Vol_Map(&user_intput) ;

		//if(Customer_read.Balance >= user_intput.price)        // Sri pending
		{
			dispense_volume = user_intput.vol ;

			Customer_read.Balance = user_intput.price ;
			Customer_read.Consumed_Cost = user_intput.price ;
			Customer_read.Consumed_Vol = user_intput.vol ;

//			update_customer_data(Customers, &Customer_read ) ;
			update_customer_data(&Customer_read ) ;
                        Total_Dispensed_Volume = user_intput.vol ;
			Dispense_On_Request(user_intput.vol) ;
		}
//		else
//LCD			printf("!!! Insufficient Balance... Please Recharge\nMax available water for you is Rs.%d  / %d Liters\n\n", Customer_read.Balance, Srvc_IpoCurveS16(Customer_read.Balance, s16_Price_CUR) ) ;
	}

}

/*----------------------------------------------------*/

void get_user(Customer_struct *customer)
{

	customer->Customer_ID 	=	1			;
/*
	strcpy (customer->name , "\0" )		                ;
	customer->Balance		=	0		;
	customer->Consumed_Vol	=	0			;
	customer->Consumed_Cost	=	0			;
	customer->Consumed_cnt	=	0			;
*/
}

/*----------------------------------------------------*/

uint8 Autenticate(Customer_struct *Customers, Customer_struct *customer)
{
	uint8 idx;

	for(idx=0; idx<MAX_SAVED_CUSTOMERS; idx++)
	{
		if ( (Customers[idx].Customer_ID == customer->Customer_ID) ) // && !strcmp(Customers[idx].name, customer->name) )
		{
//LCD			puts("User IDENTIFIED\n");

			strcpy (customer->name , Customers[idx].name )				;
			customer->Balance	=	Customers[idx].Balance			;
			customer->Consumed_Vol	=	Customers[idx].Consumed_Vol		;
			customer->Consumed_Cost	=	Customers[idx].Consumed_Cost	;
			customer->Consumed_cnt	=	Customers[idx].Consumed_cnt		;

			return IDENTIFIED ;
		}
		else
		{
//LCD			puts("User UNIDENTIFIED\n");
			return UNIDENTIFIED ;
		}
	}
	return UNIDENTIFIED ;
}


/*----------------------------------------------------*/

void Cost_Vol_Map(user_intput_st *user_intput)
{

	if (user_intput->unit == UNIT_VOL)
		user_intput->price = Srvc_IpoCurveS16(user_intput->vol, s16_Vol_CUR) ;
	else if (user_intput->unit == UNIT_RS)
		user_intput->vol = Srvc_IpoCurveS16(user_intput->price, s16_Price_CUR);

}


/*----------------------------------------------------*/

//uint8 update_customer_data(Customer_struct *Customers, Customer_struct *customer)
uint8 update_customer_data(Customer_struct *customer)
{
	uint8 idx;

	for(idx=0; idx<MAX_SAVED_CUSTOMERS; idx++)
	{
		if ( (Customers[idx].Customer_ID == customer->Customer_ID) && !strcmp(Customers[idx].name, customer->name) )
		{
/*                  
			puts("user before deduction\n") ;
			printf("\tCustomers[idx].Customer_ID = %d\n", Customers[idx].Customer_ID) ;
			printf("\tCustomers[idx].name = %s\n", Customers[idx].name) ;
			printf("\tCustomers[idx].Balance = %d\n", Customers[idx].Balance) ;
			printf("\tCustomers[idx].Consumed_Vol = %d\n", Customers[idx].Consumed_Vol) ;
			printf("\tCustomers[idx].Consumed_Cost = %d\n", Customers[idx].Consumed_Cost) ;
			printf("\tCustomers[idx].Consumed_cnt = %d\n", Customers[idx].Consumed_cnt) ;
*/
			Customers[idx].Balance-= customer->Balance ;
			Customers[idx].Consumed_Cost+= customer->Consumed_Cost ;
			Customers[idx].Consumed_Vol+= customer->Consumed_Vol ;
			Customers[idx].Consumed_cnt++ ;
/*
			puts("after before deduction\n") ;
			printf("\tCustomers[idx].Customer_ID = %d\n", Customers[idx].Customer_ID) ;
			printf("\tCustomers[idx].name = %s\n", Customers[idx].name) ;
			printf("\tCustomers[idx].Balance = %d\n", Customers[idx].Balance) ;
			printf("\tCustomers[idx].Consumed_Vol = %d\n", Customers[idx].Consumed_Vol) ;
			printf("\tCustomers[idx].Consumed_Cost = %d\n", Customers[idx].Consumed_Cost) ;
			printf("\tCustomers[idx].Consumed_cnt = %d\n", Customers[idx].Consumed_cnt) ;
*/
		}
	}

	return TRUE ;
}

/*----------------------------------------------------*/

#define VENDING 2
void Dispense_On_Request(float request_vol)
{

/*
	if (flow_sensor_vol < (request_vol-VALVE_POST_STOP_QUANT) )
		Set_Dispense_swt_st (DISPENSE, VENDING_MODE) ;
	else
	{
		Set_Dispense_swt_st (DISPENSE_STOP, VENDING_MODE) ;
		flow_sensor_vol = 0 ;
	}
*/

//        FS_Pulse_Cnt_curr = 0 ;
//        FS_Pulse_Cnt_Total = 0 ;
//        flow_sensor_vol = 0;
        
//	do{
        if(flow_sensor_vol < (request_vol-VALVE_POST_STOP_QUANT) )
        {
          Set_Dispense_swt_st (DISPENSE, VENDING_MODE) ;
          flow_sensor_vol+= Get_Flow_sens_Vol(VENDING) ;
          //LCD		printf("\tDispensed Quantity: %d\n", flow_sensor_vol) ;
          //LCD		printf("\tRemaining: %d Liters\n", (request_vol-flow_sensor_vol) ) ;
        }
//	}while (flow_sensor_vol < (request_vol-VALVE_POST_STOP_QUANT) );
        
	if (flow_sensor_vol >= (request_vol-VALVE_POST_STOP_QUANT) )
	{
          Pump_ON_OFF_swt_Man_st = 0 ;
          Set_Dispense_swt_st (DISPENSE_STOP, VENDING_MODE) ;
          Vending_Enable_st = DISABLE ;
          //		flow_sensor_vol = 0 ;
//LCD		printf("\tDiespensed %d litres\n", request_vol) ;
	}

}


/*----------------------------------------------------*/

void Flush_Now(void)
{

	uint8 manual_flush_swt_st ;
        #if(SEMI_AUTO_FLUSH == ENABLED)
          data_type curr_mem_dP_lvl;
        #endif

	manual_flush_swt_st = Get_Manual_Flush_swt_st() ;

	if(manual_flush_swt_st==SWT_ON)
	{
		#if(SEMI_AUTO_FLUSH == ENABLED)
                  curr_mem_dP_lvl = Get_Mem_delta_P_lvl() ;

                  if(curr_mem_dP_lvl == FUALTY_DP_SENSOR)
                          Set_Flush_Mode_St(FLUSH_STOP) ;
                  else if(curr_mem_dP_lvl == MEMBRANE_CLOGGED)
                          Set_Flush_Mode_St(FLUSH_START) ;
                  else if(curr_mem_dP_lvl == MEMBRANE_CLEAN)
                          Set_Flush_Mode_St(FLUSH_STOP) ;

		#else
                  Set_Flush_Mode_St(FLUSH_START) ;
		#endif
	}
	else	//	if(manual_flush_swt_st==SWT_OFF)
	{
		Set_Flush_Mode_St(FLUSH_STOP) ;
	}

}

/*----------------------------------------------------*/

void Flush_Auto(void)
{

	data_type curr_mem_dP_lvl;

#if(SEMI_AUTO_FLUSH == ENABLED)
	data_type manual_flush_swt_st ;

	manual_flush_swt_st = Get_Manual_Flush_swt_st() ;
	if(manual_flush_swt_st == SWT_ON)
          Flush_Now() ;
#endif

	curr_mem_dP_lvl = Get_Mem_delta_P_lvl() ;

	if(curr_mem_dP_lvl == MEMBRANE_CLOGGED)
		Set_Flush_Mode_St(FLUSH_START) ;
	else if(curr_mem_dP_lvl == MEMBRANE_CLEAN)
		Set_Flush_Mode_St(FLUSH_STOP) ;
	else if(curr_mem_dP_lvl == FUALTY_DP_SENSOR)
		Set_Flush_Mode_St(FLUSH_STOP) ;

}

/*----------------------------------------------------*/

uint8 Get_PreFilter_delta_P_lvl(void)
{
	float	pre_fil_Us_pres	;
	float	pre_fil_Ds_pres	;
	float	prefltr_dP		;
	uint8	prefltr_dP_st	;
       
//	pre_fil_Us_pres	=	ADC_Get (PreFilter_Us_I_A) ;
//	pre_fil_Ds_pres	=	ADC_Get (PreFilter_Ds_I_A) ;

//     	pre_fil_Us_pres	=	ADC1_Get(PreFilter_Us_I_A); 
//	pre_fil_Ds_pres	=	ADC1_Get(PreFilter_Ds_I_A); 

	pre_fil_Us_pres	=	Get_PreFilter_PS_upstream() ;
	pre_fil_Ds_pres	=	Get_PreFilter_PS_downstream() ;
        
	prefltr_dP = ABS_Diff(pre_fil_Us_pres, pre_fil_Ds_pres ) ;
        
	if (pre_fil_Us_pres < pre_fil_Ds_pres + 3.0 )		//	Verify
	{
//LCD		puts("\t____________________________________________\n\t!!! Fualty Membrane Pressure Sensor !!!\n\t____________________________________________\n") ;
		prefltr_dP_st = FUALTY_DP_SENSOR ;
	}

	else if(prefltr_dP >= PREFLTR_DP_CLOG_THRES_BAR)
		prefltr_dP_st = PREFLTR_CLOGGED ;
	else
		prefltr_dP_st = PREFLTR_CLEAN ;

	return prefltr_dP_st ;

}

/*----------------------------------------------------*/

uint8 Get_Mem_delta_P_lvl(void)
{
	float	pre_mem_pres	;
	float	post_mem_pres	;
	float	mem_dP		;
	uint8	mem_dP_st	;

//	pre_mem_pres	=	ADC_Get (PreFilter_Ds_I_A) ;
//	post_mem_pres	=	ADC_Get (MemFilter_Ds_I_A) ;

//     	pre_mem_pres	=	ADC1_Get(PreFilter_Ds_I_A); 
//	post_mem_pres	=	ADC1_Get(MemFilter_Ds_I_A); 

	pre_mem_pres	=	Get_PreFilter_PS_downstream() ;
	post_mem_pres	=	Get_MEM_Filter_Ds_post() ;
        
	mem_dP = ABS_Diff(pre_mem_pres, post_mem_pres ) ;

	if (pre_mem_pres < post_mem_pres)		//	Verify
	{
//LCD		puts("\t____________________________________________\n\t!!! Fualty Membrane Pressure Sensor !!!\n\t____________________________________________\n") ;
		mem_dP_st = FUALTY_DP_SENSOR ;
	}

	else if(mem_dP >= MEM_DP_CLOG_THRES_BAR)
		mem_dP_st = MEMBRANE_CLOGGED ;
	else
		mem_dP_st = MEMBRANE_CLEAN ;

	return mem_dP_st ;

}

/*----------------------------------------------------*/

#define HEALTH 1
uint8 Get_System_Health(void)
{
	float flow_vol = 0;
	uint8 pre_fltr_dp_st = 0;
	uint8 mem_fltr_dp_st = 0;
	uint8 water_lvl_st = 0;
        uint8 ret_error_st = 0;
        
	pre_fltr_dp_st = Get_PreFilter_delta_P_lvl() ;
	mem_fltr_dp_st = Get_Mem_delta_P_lvl() ;

	flow_vol = Get_Flow_sens_Vol(HEALTH);
	water_lvl_st = Get_Water_lvl() ;

        ret_error_st = 0;
        
	if(pre_fltr_dp_st == PREFLTR_CLOGGED )
          Srvc_SetBitU8(&ret_error_st, PREFLTR_ERROR_BIT) ;
	if (mem_fltr_dp_st == MEMBRANE_CLOGGED )
          Srvc_SetBitU8(&ret_error_st, MEMBRANE_ERROR_BIT) ;
	if (flow_vol == FALSE )
          Srvc_SetBitU8(&ret_error_st, FLOW_ERROR_BIT) ;
	if (water_lvl_st <= WATER_LEVEL_LOW )
          Srvc_SetBitU8(&ret_error_st, WLATER_LOW_BIT) ;
	if ( (pre_fltr_dp_st == PREFLTR_CLEAN) && (mem_fltr_dp_st == MEMBRANE_CLEAN) && (water_lvl_st != WATER_LEVEL_LOW ) )
        {
          ret_error_st = 0;
          Srvc_SetBitU8(&ret_error_st, SYS_HEALTHY_BIT) ;
        }

        return ret_error_st ;
}


/*----------------------------------------------------*/

void Save_System_Health(uint8 sys_health) 
{
  Sys_Health_St = sys_health ;
}

/*----------------------------------------------------*/

uint8 Load_System_Health(void) 
{
  return Sys_Health_St ;
}

/*----------------------------------------------------*/
void Set_Health_Check (uint8 enbl_cond)
{
  Health_Check_St = enbl_cond ;
}

/*----------------------------------------------------*/

uint8 Get_Health_Check(void)
{
  return Health_Check_St ;
}

/*----------------------------------------------------*/

float Get_PreFilter_PS_upstream(void)
{
	uint16 adc_val ;
	float pres_val ;

//	adc_val = ADC_Get (PreFilter_Us_I_A) ;
       	adc_val	= ADC1_Get(PreFilter_Us_I_A) ;
	pres_val = ADC_Pres_Conv(adc_val, PREFILTER_US) ;

	return pres_val ;
}

/*----------------------------------------------------*/

float Get_PreFilter_PS_downstream(void)
{
	uint16 adc_val ;
	float pres_val ;

//	adc_val = ADC_Get (PreFilter_Ds_I_A) ;
        adc_val	= ADC1_Get(PreFilter_Ds_I_A) ;
	pres_val = ADC_Pres_Conv(adc_val, PREFILTER_DS) ;

	return pres_val ;
}

/*----------------------------------------------------*/

float Get_MEM_Filter_Ds_post(void)
{
	uint16 adc_val ;
	float pres_val ;

//	adc_val = ADC_Get (MemFilter_Ds_I_A) ;
        adc_val	= ADC1_Get(MemFilter_Ds_I_A) ;        
	pres_val = ADC_Pres_Conv(adc_val, MEMFILTER_DS) ;

	return pres_val ;
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

float ADC_Pres_Conv(uint16 adc_val, uint8 pres_sens_idx)
{
  float pres_val ;
  float adc_mv ;
  
  adc_mv =  (adc_val * ADC_PRES_RATIOMETRIC_FACTOR) + PRES_SENS_ERROR_FACTOR ;
  //pres_val = pres_val * VOTL_MILI_VOLT_CONV_FACTOR ;    //mV conversion
  
  pres_val = (adc_mv - PRES_SENS_ERROR_FACTOR) / (PRES_SENS_RATIOMETRIC_FACTOR) ;
  
  return pres_val ;
  
}

/*----------------------------------------------------*/
