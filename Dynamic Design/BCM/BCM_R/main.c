/*
 * BCM.c
 *
 * Created: 5/22/2023 9:00:25 PM
 * Author : atef
 */ 

#include "bcm/bcm.h"


u8 au8Buffer[6] ;
void Notify(void)
{ 
	
	SET_BIT(PORTB,0);
	LCD_SetCursor(0,0); _delay_ms(100);
	LCD_WriteString("I_received");
 	LCD_SetCursor(1,0);
 	LCD_WriteString(au8Buffer);
}

int main(void)
{
	DIO_Init_All();
	LCD_Init();

	GLOBALE_ENABLE();
	/* TX */

	
	strBCMCfg_t strBCMCfg = {BCM_UART_PROTOCOL, BCM_RX};
	strBCMTaskCfg_t strBCMTaskCfg = { 13, au8Buffer, 0x0A, Notify, BCM_UART_PROTOCOL, BCM_RX};
	BCM_Setup(&strBCMTaskCfg);
	BCM_Init(&strBCMCfg);
	
	while(1)
	{
      
		BMC_RxDispatcher();
		
		_delay_ms(1);
		

		
	}

}


