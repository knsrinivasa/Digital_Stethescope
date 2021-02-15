/*
 ============================================================================
 Name        : Water_purifier.c
 Author      : SKN
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "main_inf.h"


int main1(void)
{
	uint8 sched_speed = OS_SCHED_SLOW_DOWN_RATE ;

		Customers[0].Customer_ID		=	1		 ;
		strcpy (Customers[0].name , "Sri" )			 ;
		Customers[0].Balance			=	1000	 ;
		Customers[0].Consumed_Vol		=	100		 ;
		Customers[0].Consumed_Cost		=	1000	 ;
		Customers[0].Consumed_cnt		=	10		 ;

		Customers[1].Customer_ID		=	2		 ;
		strcpy (Customers[1].name , "Rama" )		 ;
		Customers[1].Balance			=	200		 ;
		Customers[1].Consumed_Vol		=	200		 ;
		Customers[1].Consumed_Cost		=	2000	 ;
		Customers[1].Consumed_cnt		=	20		 ;

	system("cls") ;
//	puts("\n/*******************************/\n\tWater_purifier\n/*******************************/\n"); /* prints Water_purifier */
//	printf("Schuduler running at speed [ 1 : %d ]\n", sched_speed) ;
	// Scheduler
	sched();

	return EXIT_SUCCESS;
}
