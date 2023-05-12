/*
 * pre.h
 *
 * Created: 5/12/2023 12:38:30 AM
 *  Author: atef
 */ 


#ifndef PRE_H_
#define PRE_H_

#include "utils.h"
#include "memmap.h"
#include "std_typs.h"


//            UART configuration
// #define  Device_role		TRANSMITTER
// #define  SPEED_MODE         NORMAL_SPEED
// #define  CPU_F               _16_MHZ
// #define  BUAD_RATE          BAUD_9600
// #define  SYNCH_MODE         ASYNCHRONOUS
// #define  PARITY_MODE        NO_PARITY
// #define  N_DATA_BITS        _8_DATA_BITS
// #define  N_STOP_BITS        ONE_STOP_BIT



/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/*****************************_SELECT_Device_role_********************************/
/*
 *	TO  SELECT Device role for uart DEVICE
 *		option 1 			-> 	TRANSMITTER  
 *		option 2 			->      RECEIVER		
 *		option 3 			-> 	TRANSCEIVER	
 */	
#define  Device_role		TRANSMITTER
/*****************************_SELECT_SPEED_MODE_********************************/
/*
 *	TO  SELECT SPEED MODE for uart
 *		option 1 			-> 		UART_NORMAL_SPEED   
 *		option 2 			-> 		UART_DOUBLE_SPEED
 */
#define UART_TRANSMISSION_SPEED		NORMAL_SPEED

/*****************************_SELECT_CPU_F_********************************/
/*
 *	TO  SELECT CPU F
 *		option 1 			-> 		_4_MHZ   
 *		option 2 			-> 		_8_MHZ
        option 2 			-> 		_16_MHZ
 */
#define CPU_F		_16_MHZ

/******************************_SELECT_BUAD_RATE_*******************************/
/*
 *	TO  SELECT UART BUAD RATE
 *		option 1 			-> 		BAUD_2400
 *		option 2 			-> 		BAUD_4800
 *		option 3 			-> 		BAUD_9600
 *		option 4 			-> 		BAUD_14400
 *		option 5 			-> 		BAUD_19200
        option 6 			-> 		BAUD_28800
 */
#define UART_CHARACTER_SIZE			BAUD_9600

/**********************************_SELECT_MODE_SELECT**********************************/
/*
 *	TO  SELECT MODE SELECT for uart
 *		option 1 			-> 		ASYNCHRONOUS
 *		option 2 			-> 		SYNCHRONOUS
 */
#define SYNCH_MODE			ASYNCHRONOUS

/**********************************_SELECT_PARITY_MODE_**********************************/
/*
 *	TO  SELECT PARITY MODE for uart
 *		option 1 			-> 		NO_PARITY   
 *		option 2 			-> 		EVEN_PARITY    
 *		option 3 			-> 		ODD_PARITY
 */
#define PARITY_MODE      		NO_PARITY
/******************************_SELECT_CHARACTER_SIZE_*******************************/
/*
 *	TO  SELECT CHARACTER SIZE for uart
 *		option 1 			-> 		_5_DATA_BITS
 *		option 2 			-> 		_6_DATA_BITS
 *		option 3 			-> 		_7_DATA_BITS
 *		option 4 			-> 		_8_DATA_BITS
 *		option 5 			-> 		_9_DATA_BITS
 */
#define  N_DATA_BITS			_8_DATA_BITS

/******************************_SELECT_STOP_SIZE_*******************************/
/*
 *	TO  SELECT CHARACTER SIZE for uart
 *		option 1 			-> 		ONE_STOP_BIT
 *		option 2 			-> 		TWO_STOP_BIT
 */
#define   N_STOP_BITS 			ONE_STOP_BIT

/*----------------------------------------------------------------------*/




#endif /* PRE_H_ */
