/*
 * os.c
 *
 * Created: 5/28/2023 1:52:21 PM
 *  Author: atef
 */ 

#include "os.h"


/*- LOCAL FUNCTIONS PROTOTYPES -----------------------------*/
/*************************************************************************************************
* Description     : A function to check if this Priority available or not
*************************************************************************************************/
static enuErrorStatus_t OS_IsStructEmpty(strOSConfigTask_t* strOSConfigTask);

static void OS_TimerCallback(void);


/*- GLOBAL STATIC VARIABLES --------------------------------*/
static strOSConfigTask_t gastrOSConfigTaskParameters[NO_OF_TASKS];
static strOSConfigTask_t gstrOSEmptyTask = {0, NULLPTR};           
static u64 gu64TickCounts = 0;
static u8 gu8TFlag = LOW;
static u8 gu8init_flag=LOW;






static enuErrorStatus_t OS_IsStructEmpty(strOSConfigTask_t* strOSConfigTask)
{
	/* Checking if the attributes of the input struct are null */
	
	if((strOSConfigTask->u8Periodicity == 0) && (strOSConfigTask->pfTask == NULLPTR))
	{
		return OS_OK ;
	}
	else
	{
		return OS_ERROR ;
	}
		
	
} 

static void OS_TimerCallback(void)   ////
{
	/* Incrementing the value of the tick count */
	gu64TickCounts++;
	/* Raising the flag to acknowledge the scheduler that an ISR has occured */
	gu8TFlag = HIGH;
}

void OS_Init()
{
	if (gu8init_flag==0)
	{
		
	     gu8init_flag++;
		 return OS_OK;	
	}
	else 
	{
		return OS_ERROR;
	}
}

enuErrorStatus_t SOS_CreateTask(u8 u8Priority, strOSConfigTask_t* strSOSConfigTask)
{
	enuErrorStatus_t STATUS;
	
	/* Checking if the selected task struct is empty, the task priority is less than the maximum number of tasks,
		and the input struct is not empty */
	if((OS_IsStructEmpty(&gastrOSConfigTaskParameters[u8Priority]) == OS_OK) && 
		(u8Priority < NO_OF_TASKS) && (strSOSConfigTask != NULLPTR))
	{
		/* Assigning the input struct parameters into the selected priority struct location inside the tasks array */
		gastrOSConfigTaskParameters[u8Priority] = *strSOSConfigTask;			
		STATUS = OS_OK;
	}
	else
	{
		STATUS = OS_ERROR;
	}
	return STATUS;
}

void OS_Run(void)
{
	
}


void OS_DeleteTask(u8 u8Priority)
{
	if((OS_IsStructEmpty(&gastrOSConfigTaskParameters[u8Priority]) !=OS_OK ) && (u8Priority < NO_OF_TASKS))
	
	{
		
	}
}