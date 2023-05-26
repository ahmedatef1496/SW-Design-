/*
 * uart.c
 *
 * Created: 5/11/2023 11:13:32 PM
 *  Author: atef
 */

#include "uart.h"


u8 *Pu8_g_TX_STR,*Pu8_g_RX_STR;
volatile u8 u8_g_RX_STR[20],u8_g_TX_STR[20],RX_flag=0,TX_falg=1;

/*************************Pointer to functions to be assigned to ISR**************************/
static void (*UART_RX_Fptr) (void)=NULLPTR;
static void (*UART_TX_Fptr) (void)=NULLPTR;



extern const ST_UART_cfg UART_cfg[TOTAL_DEVICES];
 
void UART_Init(void)
{
	/*Set the Baud rate*/
	u16 UBRRVAL=((F_CPU/(16L*UART_BAUDRATE))-1);
	UBRRH=(u8)(UBRRVAL>>8);
	UBRRL=(u8)(UBRRVAL);
	
	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	/*UART Transmitter and Receiver Enable*/
	//UCSRB =(1<<TXEN )|(1<<RXEN );
	        switch(UART_cfg[DEVICE_0].Device_role)
	        {
		        case TRANSMITTER:
		        SET_BIT( UCSRB,TXEN);
		        break;
		        case RECEIVER:
		        SET_BIT( UCSRB,RXEN);
		        break;
		        case TRANSCEIVER:
                UCSRB =(1<<TXEN )|(1<<RXEN );
		        break;
	        }
	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = 00 Disable parity bit
	 * USBS    = 0 One stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/ 	
	/* DATA BITS*/
	u8 data=UART_DATA_BITS;
	if(data==5)
	{
		CLR_BIT(UCSRC ,UCSZ1 );
		CLR_BIT(UCSRC ,UCSZ0 );
	}else if(data==6)
	{
		CLR_BIT(UCSRC ,UCSZ1 );
		SET_BIT(UCSRC ,UCSZ0 );
	}else if(data==7)
	{
		SET_BIT(UCSRC ,UCSZ1 );
		CLR_BIT(UCSRC ,UCSZ0 );
	}else if(data==8)
	{
		SET_BIT(UCSRC ,UCSZ1 );
		SET_BIT(UCSRC ,UCSZ0 );
	}else if (data==9)
	{
		SET_BIT(UCSRB ,UCSZ2 );
		SET_BIT(UCSRC ,UCSZ1 );
		SET_BIT(UCSRC ,UCSZ0 );
	}
	/*PARITY*/
	u8 parity=UART_PARITY_BIT;
	if(parity==0)
	{
		CLR_BIT(UCSRC ,UPM1 );
		CLR_BIT(UCSRC ,UPM0 );
	}else if(parity==1)
	{
		SET_BIT(UCSRC ,UPM1 );
		SET_BIT(UCSRC ,UPM0 );
	}else if(parity==2)
	{
		SET_BIT(UCSRC ,UPM1 );
		CLR_BIT(UCSRC ,UPM0 );
	}
	/*STOP BITS*/
	u8 stop = UART_STOP_BITS;
	if(stop==1)
	{
		CLR_BIT(UCSRC ,USBS );
	}else if(stop==2)
	{
		SET_BIT(UCSRC ,USBS );
	}

} 



void UART_sendByte( u16 data)
{
	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for 
	 * transmitting a new byte so wait until this flag is set to one */
	while(!READ_BIT(UCSRA,UDRE));
	/* Put the required data in the UDR register and it also clear the UDRE flag as 
	 * the UDR register is not empty now */	 
	UDR = data;
	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transimission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/	
}


void UART_recieveByte(u16* pu16Data)
{
	/* RXC flag is set when the UART receive data so wait until this 
	 * flag is set to one */
	while(!READ_BIT(UCSRA,RXC));
	/* Read the received data from the Rx buffer (UDR) and the RXC flag 
	   will be cleared after read this data */	 
    *pu16Data=UDR;		
}


void UART_sendByteNoBlock( u16 data)
{
	 
	UDR = data;

}


void UART_recieveByteNoBlock(u16* pu16Data)
{
    *pu16Data=UDR;		
}

void UART_RX_InterruptEnable(void)
{
	SET_BIT(UCSRB,RXCIE);
}
void UART_RX_InterruptDisable(void)
{
	CLR_BIT(UCSRB,RXCIE);
}

void UART_TX_InterruptEnable(void)
{
	SET_BIT(UCSRB,TXCIE);
}
void UART_TX_InterruptDisable(void)
{
	CLR_BIT(UCSRB,TXCIE);
}

void UART_RX_SetCallBack(void(*LocalFptr)(void))
{
	 UART_RX_Fptr=LocalFptr;
}

void UART_TX_SetCallBack(void(*LocalFptr)(void))
{
	UART_TX_Fptr=LocalFptr;
}

ISR(UART_RXC_vect)
{   
	if(UART_RX_Fptr!=NULLPTR)
	{
		UART_RX_Fptr();
	}
}


ISR(UART_TX_vect)
{
	if(UART_TX_Fptr!=NULLPTR)
	{
		UART_TX_Fptr();
	}
}


/***************************************************************************************/

void UART_RX_FUN ()  //using g_string
{
	static u8 i=0;
	UART_recieveByteNoBlock(&u8_g_RX_STR[i]);
	i++;
	RX_flag++;
}



void UART_ReciveStringAshync (u8 *str) //just for testing 
{ 
	static u8 i=0;
	if (RX_flag>0)
	{
		LCD_SetCursor(1,i);
		LCD_WriteChar(u8_g_RX_STR[i]);
		
		i++;
		RX_flag--;
	} 
/*	Pu8_RX_STR=u8_g_RX_STR;*/
	
}
void UART_RX_FUN_V2 ()
{
	static u8 i=0;
	UART_recieveByteNoBlock(&Pu8_g_RX_STR[i]);
	i++;

}
void UART_ReciveStringAshync_V2 (u8 *str) 
{
	Pu8_g_RX_STR=str;	
}

 void UART_TX_FUN (void)
 {
	 static u8 j=1;
	 static u8 i=1;
	 if (Pu8_g_TX_STR[i]!=0)
	 {
		 UART_sendByteNoBlock(Pu8_g_TX_STR[i]);
		 i++;  //big problem if no stop if
	 }
	 else
	 {
		 i=1;
		 //we could disable int here for safty
	 }
	 

 }
 void UART_SendStringAshync(u8*str)
 {
	 static u8 i=0;
	 

	 //FIRST byte
	 UART_sendByteNoBlock(str[0]);
	 // COPY address of string
	 Pu8_g_TX_STR=str;
 }
  void UART_TX_FUN_V2 (void)
  {
	  TX_falg=1;

  }
 void UART_SendStringRunnable()    //Ashync_V2  should be in while sami asych
 {
	 static u8 i=0;
	 
  if (TX_falg==1)                     //init value of flag is 1 to send first byte
  {
 	if (Pu8_g_TX_STR[i]!=0)
	  	{
	        UART_sendByteNoBlock(Pu8_g_TX_STR[i]);
	        i++;
	    }
  } 
  TX_falg=0;
	 

 }
void UART_SendStringSetter(u8*str)
{
	Pu8_g_TX_STR=str;
}


 void uart_init_noblocking()
 {
	 UART_TX_SetCallBack(UART_TX_FUN_V2);
	 UART_TX_InterruptEnable();
	 
	 UART_RX_SetCallBack(UART_RX_FUN_V2);
	 UART_RX_InterruptEnable();
 }
 