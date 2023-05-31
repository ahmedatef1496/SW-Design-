/*
 * app.c
 *
 * Created: 5/29/2023 2:13:47 PM
 *  Author: atef
 */ 

#include "app.h"

void app_init ()
{
		DIO_Init_All();                        //initialize dio pins regarding the configuration (button included)
		LCD_Init();                           //initialize lcd as 4 bit mode
		GLOBALE_ENABLE();                    //Global interrupt Enable
		EXI_Init();                  
		EXI_Enable(EX_INT0);
		EXI_TriggerEdge(EX_INT0,RISING_EDGE);
		EXI_SetCallBack(EX_INT0,OS_disable);
}


void Task3(void)    //task to repeated each 300ms
{
	DIO_togglepin(PINB0);
}
void Task5(void)   //task to repeated each 500ms
{
	DIO_togglepin(PINB1);
}