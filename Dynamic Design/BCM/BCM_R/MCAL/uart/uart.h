/*
 * pre.h
 *
 * Created: 5/12/2023 12:38:30 AM
 *  Author: atef
 */ 


#ifndef PRE_H_
#define PRE_H_

#include "../../SERVIES/utils.h"
#include "../../SERVIES/memmap.h"
#include "../../SERVIES/std_typs.h"
#include "uart_cfg.h"
//#include "lcd.h"

void UART_Init(void);
void UART_sendByte(u16 data);
void UART_recieveByte(u16* pu16Data);


////////////////////////////////////////////////////
void UART_sendByteNoBlock( u16 data);
void UART_RX_InterruptEnable(void);
void UART_RX_InterruptDisable(void);
void UART_RX_SetCallBack(void(*LocalFptr)(void));
void UART_recieveByteNoBlock(u16* pu16Data);
void UART_TX_InterruptEnable(void);
void UART_TX_InterruptDisable(void);
void UART_TX_SetCallBack(void(*LocalFptr)(void));



 void uart_init_noblocking();
void UART_SendStringAshync(u8*str);
void UART_ReciveStringAshync ();
void UART_ReciveStringAshync_V2 (u8 *str) ;
//void UART_ReciveStringAshync (u8 *Pu8_RX_STR);
void UART_SendStringSetter(u8*str);
void UART_SendStringRunnable();
#endif /* PRE_H_ */