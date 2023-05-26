/*
 * bcm.c
 *
 * Created: 5/22/2023 9:02:44 PM
 *  Author: atef
 */ 
#include "bcm.h"
typedef struct{
	/* task configuration */
	strBCMTaskCfg_t*	     strBCMTaskCfg;
	/* To count the numbed of bytes sent or received */
	  u16			     u16ByteCounter;
	/* bmc state to  hold the task state */
	enuBCMState_t     enuBCMState;
}strBCMHandeler_t;











static void BCM_SendByte(u16 u16Data);
static void BCM_ReceiveByte(u16* pu16Data);
static void BCM_TxHandler(void);
 void UART_SendDispatcher() ;
void BCM_UART_setter(u8*str);
  static void BCM_RxHandler(void);

/************************************************************************/
/*                                                                      */
/************************************************************************/

static strBCMHandeler_t gstrBCMHandeler;
static strBCMTaskCfg_t  gstrBCMTaskCfg;
static u8 gu8BCMInitState = LOW;
static u8 gu8TX_Flag=LOW;
static u8 gu8RX_Flag=LOW;
static u16 *gpu16RxBuffer;
static u16 gu16Buffer=LOW;
u8 *Pu8_g_TX_STR,*Pu8_g_RX_STR;

/************************************************************************/
/*                                                                      */
/************************************************************************/
enuErrorStatus_t BCM_Init(strBCMCfg_t *strBCMCfg)

{   BCM_UART_setter(gstrBCMHandeler.strBCMTaskCfg->pu8_DataBuffer);
	/* variable to hold the status of the function (E_OK, E_ERROR) and return it */
	enuErrorStatus_t enuErrorStatus;
	/* if the input pointer equals null */
	if(strBCMCfg==NULLPTR)
	{
		enuErrorStatus = E_ERROR;
	}//end if
	else
	{
		switch  (strBCMCfg->Protocol)

		{
			case BCM_UART_PROTOCOL:
			/* initialize uart */
			UART_Init();
			//BCM_UART_setter(au8Buffer);
			
			/* check for the mode (send, receive) */
			/* if send is selected */
			if(strBCMCfg->Mode==BCM_TX)
			{
				/* enable send interruot */
				UART_TX_InterruptEnable();
				/* set call back function */
				UART_TX_SetCallBack( BCM_TxHandler);

			}//end if
			/* if receive is selceted */
			else if(strBCMCfg->Mode==BCM_RX)
			{
				/* enable receive interruot */
				UART_RX_InterruptEnable();
				/* set call back function */
				UART_RX_SetCallBack( BCM_RxHandler);
			}//end else if
			else
			{
				/* enable send interruot */
				UART_TX_InterruptEnable();
				/* set call back function */
				UART_TX_SetCallBack(BCM_TxHandler);

				/* enable receive interruot */
				UART_RX_InterruptEnable();
				/* set call back function */
				UART_RX_SetCallBack(BCM_RxHandler);

			}//end else
			enuErrorStatus = E_OK;
			break;

			case BCM_SPI_PROTOCOL:

			break;


			case BCM_I2C_PROTOCOL:

			break;

			default:
			/* update function status to be E_ERROR */
			enuErrorStatus = E_ERROR;


		} //end switch of channel
	}//end else
	/* update init state to be high */
	gu8BCMInitState = HIGH;
	return enuErrorStatus;
}//end BCM_Init

enuErrorStatus_t BCM_Setup(strBCMTaskCfg_t *pstrBCMTaskCfg)
{ 
	
	/* update the global task configuration pointer struct with the input pointer struct */
	gstrBCMHandeler.strBCMTaskCfg=pstrBCMTaskCfg;
	/* initialize the BCM state machine to be idle */
	gstrBCMHandeler.enuBCMState = BCM_STATE_IDLE;
	/* initialize the byte's counter to be zero */
	gstrBCMHandeler.u16ByteCounter = 0;
	/* check for the mode */
	/* if selected mode is send */
	if(gstrBCMHandeler.strBCMTaskCfg->Mode == BCM_TX)
	{
		/* update global flag to be HIGH */
		gu8TX_Flag = HIGH;
	}//end if
	else
	{
		/* update global flag to be LOW */
		gu8TX_Flag = LOW;
	}//end else
	// BCM_UART_setter(pstrBCMTaskCfg->pu8_DataBuffer);
}
enuErrorStatus_t BCM_TxDispatcher(void)
{  
	/* variable to hold the status of the function (E_OK, E_ERROR) and return it */
	enuErrorStatus_t enuErrorStatus= E_ERROR;
	/* check if BCM not initialized and the selcted mode is receive */
	if((gu8BCMInitState == LOW) && (gstrBCMHandeler.strBCMTaskCfg->Mode==BCM_TX))
	{
		/* update function status to be E_ERROR */
		enuErrorStatus = E_ERROR;
	}//end if
	else
	{  
			if(gstrBCMHandeler.strBCMTaskCfg->Protocol==BCM_UART_PROTOCOL)
			{
				/* receive byte via uart */
				
				UART_SendDispatcher();
				
			}//end if

						
						
	}
	
	
}
enuErrorStatus_t BCM_TxDispatcher_v2(void)
{
	/* variable to hold the status of the function (E_OK, E_ERROR) and return it */
	enuErrorStatus_t enuErrorStatus= E_ERROR;
	/* check if BCM not initialized and the selcted mode is receive */
	if((gu8BCMInitState == LOW) && (gstrBCMHandeler.strBCMTaskCfg->Mode==BCM_TX))
	{
		/* update function status to be E_ERROR */
		enuErrorStatus = E_ERROR;
	}//end if
	else
	{
		if (gu8TX_Flag==HIGH)
		{

			switch(gstrBCMHandeler.enuBCMState)
			{
				/* if state is idle */
				case BCM_STATE_IDLE :

				gstrBCMHandeler.enuBCMState =BCM_STATE_TXDATA;


				break;
				case BCM_STATE_TXDATA:
				/* send data */
				BCM_SendByte(gstrBCMHandeler.strBCMTaskCfg->pu8_DataBuffer[gstrBCMHandeler.u16ByteCounter]);
				/* check if byte's counter is less than the buffer size */
				if (gstrBCMHandeler.u16ByteCounter <= gstrBCMHandeler.strBCMTaskCfg->u16_Size)
				{
					/* increase the byte's counter by 1 */
					gstrBCMHandeler.u16ByteCounter++;
				}//end if
				else
				{
					/* set the byte's counter to be zero */
					gstrBCMHandeler.u16ByteCounter = 0;
					/* update state machine to be TX complete */
					gstrBCMHandeler.enuBCMState= BCM_STATE_TXCS;
				}//end else
				break;
				case BCM_STATE_TXCS:
				gstrBCMHandeler.strBCMTaskCfg->pfTaskCompleteNotify();
				/* update state machine to be finished */
				gstrBCMHandeler.enuBCMState = BCM_STATE_FINISHED;

				enuErrorStatus = E_OK;
				break;

				case BCM_STATE_FINISHED:

				break;

			}
		}

	}


}

enuErrorStatus_t BMC_RxDispatcher()
{
	/* variable to hold the status of the function (E_OK, E_ERROR) and return it */
	enuErrorStatus_t enuErrorStatus= E_ERROR;
	/* check if BCM not initialized and the selcted mode is receive */
	if((gu8BCMInitState == LOW) && (gstrBCMHandeler.strBCMTaskCfg->Mode==BCM_RX))
	{
		/* update function status to be E_ERROR */
		enuErrorStatus = E_ERROR;
	}//end if
	else
	{
		if (gu8RX_Flag==HIGH)
		{

			switch(gstrBCMHandeler.enuBCMState)
			{

				case BCM_STATE_RXDATA:
				/* send data */
				
				/* check if byte's counter is less than the buffer size */
				if (gstrBCMHandeler.u16ByteCounter <( gstrBCMHandeler.strBCMTaskCfg->u16_Size-1))
				{
					/* increase the byte's counter by 1 */
					//LCD_WriteChar(gu16Buffer);
					gstrBCMHandeler.strBCMTaskCfg->pu8_DataBuffer[gstrBCMHandeler.u16ByteCounter]=gu16Buffer;
					gstrBCMHandeler.u16ByteCounter++;
				}//end if
				else
				{            
					/* update state machine to be TX complete */
					gstrBCMHandeler.enuBCMState= BCM_STATE_RXCS;
					
					gstrBCMHandeler.strBCMTaskCfg->pu8_DataBuffer[gstrBCMHandeler.u16ByteCounter]=gu16Buffer;
					/* set the byte's counter to be zero */
					gstrBCMHandeler.u16ByteCounter = 0;
                     
					
				}//end else
				break;
			

			}
			gu8RX_Flag=LOW;
		}
		
		if (gstrBCMHandeler.enuBCMState == BCM_STATE_RXCS)
		{
							gstrBCMHandeler.strBCMTaskCfg->pfTaskCompleteNotify();
							/* update state machine to be finished */
							gstrBCMHandeler.enuBCMState = BCM_STATE_FINISHED;
		}

	}

}




/************************************************************************/
/*                              privet fun                              */
/************************************************************************/


/* private function to send byte */
static void BCM_SendByte(u16 u16Data)
{
	/* if selected PROTOCOL is uart */
	if(gstrBCMHandeler.strBCMTaskCfg->Protocol == BCM_UART_PROTOCOL)
	{
		/* send byte via uart */
		
	UART_sendByteNoBlock(u16Data);

		
	}//end if
	/* if selected PROTOCOL is spi */
	else if(gstrBCMHandeler.strBCMTaskCfg->Protocol == BCM_SPI_PROTOCOL)
	{
		/* send byte via spi */
		
	}//end else if
	/* if selected PROTOCOL is i2c */
	else
	{
		/* send start bit */
		
		/* send slave address */
		
		/* send byte via spi */
		
		/* send stop bit */
		
	}//end else
}//end BCM_SendByte





/* private function to receive byte */
static void BCM_ReceiveByte(u16* pu16Data)
{
	/* if selected PROTOCOL is uart */
	if(gstrBCMHandeler.strBCMTaskCfg->Protocol==BCM_UART_PROTOCOL)
	{
		/* receive byte via uart */
		
		UART_recieveByteNoBlock(pu16Data);
		
	}//end if
	/* if selected PROTOCOL is spi */
	else if(gstrBCMHandeler.strBCMTaskCfg->Protocol==BCM_SPI_PROTOCOL)
	{
		/* receive byte via spi */
		
	}//end else if
	/* if selected PROTOCOL is i2c */
	else
	{
		/* send start bit */
		
		/* send slave address */
		
		/* receive byte via spi */
		
		/* send stop bit */
		
	}//end else
}//end BCM_ReceiveByte


static void BCM_TxHandler(void)
{
	/* update the TX global flag to be HIGH */
	gu8TX_Flag = HIGH;
}//end BCM_TxHandler


 void UART_SendDispatcher()    //Ashync_V2  should be in while sami asych
 { 
	 static u8 i=0;
	
	 if (gu8TX_Flag==HIGH)                     //init value of flag is 1 to send first byte
	 {   
		 if (Pu8_g_TX_STR[i]!=0)
		 {  
			  BCM_SendByte(Pu8_g_TX_STR[i]);
			 i++;
		 }
	 }
	gu8TX_Flag=LOW;
	 

 }
 
 void BCM_UART_setter(u8*str)
 {
	 Pu8_g_TX_STR=str;
	
 }
 
 
 
 /************************************/
 static void BCM_RxHandler(void)
 {
	  static u8 i=0;
		/* receive byte */
		BCM_ReceiveByte(&gu16Buffer);
		//gstrBCMHandeler.u16ByteCounter++;
		/* update the RX global flag to be HIGH */
		gu8RX_Flag = HIGH;
		gstrBCMHandeler.enuBCMState= BCM_STATE_RXDATA;
	
 }//end BCM_RxHandler