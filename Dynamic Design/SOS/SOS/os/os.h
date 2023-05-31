/*
 * os.h
 *
 * Created: 5/28/2023 1:52:42 PM
 *  Author: atef
 */ 


#ifndef OS_H_
#define OS_H_

#include "../MCAL/dio/dio.h"
#include "../MCAL/timers/timer.h"
#include "os_cfg.h"
#include "../MCAL/Interrupt/exi.h"
#include "../HAL/button/button.h"

/*- STRUCTS AND UNIONS -------------------------------------*/
typedef struct
{
	/* Selecting the periodicity of the selected task */
	u8 u8Periodicity;
	/* Assigning the pointer to the task function */
	void (*pfTask)(void);
} strOSConfigTask_t;


typedef enum{
	 OS_OK,
	 OS_ERROR,
	 PRIORITY_EMPTY,
	 PRIORITY_FULL,
	 OS_INIT,
	 OS_TASK_ADDED,
	 OS_TASK_MODIFED,
	 OS_TASK_DELETED
	 
}enuOSErrorStatus_t ;



/*- APIS DECLARATIONS --------------------------------------*/
/*************************************************************************************************
* Description     : A function to initialize the timer used in OS
*************************************************************************************************/
enuOSErrorStatus_t OS_Init();
/*************************************************************************************************
* Description     : A function to create a certain task
*************************************************************************************************/
enuOSErrorStatus_t OS_CreateTask(u8 u8Priority, strOSConfigTask_t* strSOSConfigTask);
/*************************************************************************************************
* Description     : A function to delete a certain task
*************************************************************************************************/
enuOSErrorStatus_t OS_DeleteTask(u8 u8Priority);
/*************************************************************************************************
* Description     : A function to run task(s) using the OS
*************************************************************************************************/
void OS_Run(void);
/*************************************************************************************************
* Description     : A function to D initialize the timer used in OS
*************************************************************************************************/
enuOSErrorStatus_t OS_DInit();
/*************************************************************************************************
* Description     : A function to modify a certain task
*************************************************************************************************/
enuOSErrorStatus_t OS_modify_task(u8 u8Priority, strOSConfigTask_t* strSOSConfigTask);
/*************************************************************************************************
* Description     : A function to stop  os
*************************************************************************************************/
void OS_disable();

#endif /* OS_H_ */

