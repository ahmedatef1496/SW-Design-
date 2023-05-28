/*
 * button_config.h
 *
 * Created: 5/16/2023 6:48:48 AM
 *  Author: A Mah
 */ 


#ifndef BUTTON_CONFIG_H_
#define BUTTON_CONFIG_H_

#include "../../MCAL/dio/dio.h"

typedef enum { Notpressed  , pressed} Button_State;
	
	
#define BUTTON_PIN   PIND6 



#endif /* BUTTON_CONFIG_H_ */