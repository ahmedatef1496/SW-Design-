/*
 * button.h
 *
 * Created: 4/14/2023 8:18:07 AM
 *  Author: atef
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_
#include "../../MCAL/dio/dio.h"
typedef enum en_ButtonError_t
{
	BUTTON_OK,
	BUTTON_ERROR,
	WRONG_BUTTON_PIN
}en_buttonError_t;


#define  button0 PINC4
#define  button1 PIND1
#define  button3 PIN
#define  led0    PINB4
#define  led1    PINB5
#define  led2    PINB6
#define  led3    PINB7

typedef enum { NPRESSED  , PRESSED} enuButtonStatus_t;


en_buttonError_t BUTTON_init( DIO_Pin_type button);
en_buttonError_t BUTTON_read(DIO_Pin_type button , DIO_PinVoltage_type *buttonState);

#endif /* BUTTON_H_ */



/*button check*/
/*
BUTTON_read(button1,&button1State);
if (button1State==LOW)
{
	
	DIO_WritePort(PB,++c);
	while(button1State==LOW)
	{
		BUTTON_read(button1,&button1State);
	}
*/	