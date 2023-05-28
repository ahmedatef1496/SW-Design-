/*
 * SOS.c
 *
 * Created: 5/28/2023 1:50:30 PM
 * Author : atef
 */ 
#include "os/os.h"
u32 volatile static g_c=0,g_t_c=0;
void timer()
{ 
	 g_c++;                                //1ms
          if (g_c==100)                   //100ms
          {
			 // DIO_togglepin(PINB0);
			  g_t_c++;
			  g_c=0;
          }
		 
	
	
	
}

int main(void)
{ 
	DIO_Init_All();
	LCD_Init();
	TIMER0_Init(TIMER0_NORMAL_MODE);
	GLOBALE_ENABLE();
	TIMER0_OV_InterruptEnable();
	

	TIMER0_OV_SetCallBack(timer);
	
	timer_start(TIMER0_SCALER_64);
		 while(1)
		 { 
			 if (g_t_c%3==0)   
			 {
				   //  DIO_togglepin(PINB0);
				   LCD_SetCursor(0,0);
				   LCD_WriteString("task1");
			 }
			 			  if (g_t_c%5==0)  
			 			 {
				 			 // DIO_togglepin(PINB1);
							 LCD_SetCursor(0,0);
							 LCD_WriteString("task2");
			 			 }
      
		 }

}

// 	strOSConfigTask_t strSOSConfigTask0 = {3, Task0};
// 	strOSConfigTask_t strSOSConfigTask1 = {2, Task1};
// 	strOSConfigTask_t strSOSConfigTask2 = {1, Task2};
// 
// 
// 	SOS_CreateTask(0, &strSOSConfigTask0);
// 	SOS_CreateTask(1, &strSOSConfigTask1);
// 	SOS_CreateTask(2, &strSOSConfigTask2);

 