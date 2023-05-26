/*
 * BCM.c
 *
 * Created: 5/22/2023 9:00:25 PM
 * Author : atef
 */ 

#include "bcm/bcm.h"


u8 au8Buffer[13]="BCM operation" ;
void Notify(void)
{ 
	
	SET_BIT(PORTB,1);
	LCD_SetCursor(0,0); 
	LCD_WriteString("I_SENT");
 	LCD_SetCursor(1,0);
 	LCD_WriteString(au8Buffer);
}

int main(void)
{
	DIO_Init_All();
	LCD_Init();

	GLOBALE_ENABLE();
	/* TX */

	
	strBCMCfg_t strBCMCfg = {BCM_UART_PROTOCOL, BCM_TX};
	strBCMTaskCfg_t strBCMTaskCfg = { 12, au8Buffer, 0x0A, Notify, BCM_UART_PROTOCOL, BCM_TX};
	BCM_Setup(&strBCMTaskCfg);
	BCM_Init(&strBCMCfg);
	
	while(1)
	{
      
		BCM_TxDispatcher_v2();
		
		

	_delay_ms(1);	
	}

}


