/*
 * SOS.c
 *
 * Created: 5/28/2023 1:50:30 PM
 * Author : atef
 */ 
#include "os/os.h"

void Task3(void)
{
	 DIO_togglepin(PINB0);
}
void Task5(void)
{
	 DIO_togglepin(PINB1);
}
 
int main(void)
{  
	
	DIO_Init_All();
	LCD_Init();
	GLOBALE_ENABLE();
    OS_Init();
 	strOSConfigTask_t strOSConfigTask3 = {3, Task3};
 	strOSConfigTask_t strOSConfigTask5 = {5, Task5};
    OS_CreateTask(3, &strOSConfigTask3);
	OS_CreateTask(5, &strOSConfigTask5);
	OS_Run();

}


 