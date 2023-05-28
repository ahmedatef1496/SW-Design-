
#include "button_interface.h"
#include "button_config.h"
//static u8 button_press = 0 ; /** GLOBAL VARIABLE TO CHECK THE NUMBER OF PRESSED TIMES **/

Button_State Is_pressed(u8 BUTTON_PIN , u8* value){
	
	Button_State state = Notpressed; 
	en_dioError_t status_pin = WRONG_VALUE;
	
	u8 value_check = 0 ;
	
	status_pin =  DIO_readpin (BUTTON_PIN, &value_check);
	
	if (!status_pin)
	{
		state = pressed ; 
		*value = value_check ;
	}
	
	else
	{
		state = Notpressed ; 
		*value = value_check ;
	}
	
	return state ; 
	
} 

