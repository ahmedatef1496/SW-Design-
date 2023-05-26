/*
 * IncFile1.h
 *
 * Created: 5/22/2023 9:18:48 PM
 *  Author: atef
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include "../MCAL/dio/dio.h"
#include "../MCAL/uart/uart.h"
#include "../HAL/lcd/lcd.h"
#include "../MCAL/interrupt/exi.h"
typedef void (*pfTaskCompleteNotify_t) (void);

typedef enum {
	BCM_STATE_IDLE,
	BCM_STATE_TXCMD,
	BCM_STATE_TXSIZE,
	BCM_STATE_TXDATA,
	BCM_STATE_TXCS,
	BCM_STATE_RXCMD,
	BCM_STATE_RXSIZE,
	BCM_STATE_RXDATA,
	BCM_STATE_RXCS,
	BCM_STATE_FINISHED,
	BCM_STATE_ERROR
}enuBCMState_t ;



typedef enum {
	BCM_UART_PROTOCOL,
	BCM_SPI_PROTOCOL,
	BCM_I2C_PROTOCOL
}enuBCMProtocol_t;

typedef enum {
	BCM_TX,
	BCM_RX,
	BCM_TX_RX
}enuBCMMode_t;




typedef struct {
	/* Selecting communication channel (uart, spi, i2c) */
	enuBCMProtocol_t Protocol;
	/* Selecting communication mode (send, receive) */
	enuBCMMode_t    Mode;
}strBCMCfg_t;



typedef struct {
	
	/* Selecting data size to be send or received */
	 u16 			             u16_Size;
	/* Buffer to hold the data */
	u8*			                 pu8_DataBuffer;
	/* Check sum to check the data is received correctly */
	u64			                u64_CheckSum;
	/* Call back function to be called when receive or send all data is complete */
	  pfTaskCompleteNotify_t   pfTaskCompleteNotify;
	/* Selecting communication channel (uart, spi, i2c) */
	enuBCMProtocol_t 	      Protocol;
	/* Selecting communication mode (send, receive) */
	enuBCMMode_t           Mode;
}strBCMTaskCfg_t;

typedef enum {
	E_ERROR,
	E_OK
}enuErrorStatus_t;


enuErrorStatus_t BCM_Setup(strBCMTaskCfg_t *pstrBCMTaskCfg);
enuErrorStatus_t BCM_Init(strBCMCfg_t *strBCMCfg);
enuErrorStatus_t BCM_TxDispatcher(void);
enuErrorStatus_t BCM_TxDispatcher_v2(void);
enuErrorStatus_t BMC_RxDispatcher();
#endif /* INCFILE1_H_ */