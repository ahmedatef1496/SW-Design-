/*
 * os.c
 *
 * Created: 5/28/2023 1:52:21 PM
 *  Author: atef
 */ 

#include "os.h"


/*-------------------------LOCAL FUNCTIONS PROTOTYPES -------------------------*/
/*************************************************************************************************
* Description     : A function to check if this Priority available or not
*************************************************************************************************/
static enuOSErrorStatus_t OS_IsPriorityEmpty(strOSConfigTask_t* strOSConfigTask);

static void OS_TimerCallback(void);
 static void OS_Start_Check_pd ();

/*-------------------------GLOBAL STATIC VARIABLES -------------------------*/
static strOSConfigTask_t gastrOSConfigTasks[NO_OF_TASKS];
static strOSConfigTask_t gstrOSEmptyTask = {0, NULLPTR};           
static u64 gu64TickCounts = 0;
static u64 gu64TaskCounter = 0;
static u8 gu8TFlag = LOW;
static u8 gu8init_flag=0;
static  gu8os_flag=LOW;
/*------------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------*/
 static enuOSErrorStatus_t OS_IsPriorityEmpty(strOSConfigTask_t* strOSConfigTask)
{
	/* Checking if the attributes of the input struct are null */
	
	if((strOSConfigTask->u8Periodicity == 0) && (strOSConfigTask->pfTask == NULLPTR))
	{
		return PRIORITY_EMPTY ;
	}
	else
	{
		return PRIORITY_FULL ;
	}
		
	
} 

static void OS_TimerCallback(void)   
{
	/* Incrementing the value of the tick count */
	gu64TickCounts++; //1ms
	/* Raising the flag to acknowledge the scheduler that an ISR has occured */
	if (gu64TickCounts==100)//100ms
	{
		gu8TFlag = HIGH;
		gu64TaskCounter++;
		gu64TickCounts=0;
	}
	
}
/*-------------------------APIS IMPLEMENTATION -------------------------*/
enuOSErrorStatus_t OS_Init()
{
	if (gu8init_flag==0)
	{
		
	       gu8init_flag++;
			
		    
		 	TIMER0_Init(TIMER0_NORMAL_MODE);
		 	TIMER0_OV_InterruptEnable();
		 	TIMER0_OV_SetCallBack(OS_TimerCallback);
		 //	timer_start(TIMER0_SCALER_64);
			 
			  return OS_INIT;
	}
	else 
	{
		return OS_ERROR;
	}
}
enuOSErrorStatus_t OS_CreateTask(u8 u8Priority, strOSConfigTask_t* strOSConfigTask)
{
	enuOSErrorStatus_t STATUS;
	
	/* Checking if the selected task struct is empty, the task priority is less than the maximum number of tasks,
		and the input struct is not empty */
	if((OS_IsPriorityEmpty(&gastrOSConfigTasks[u8Priority]) == PRIORITY_EMPTY) && 
		(u8Priority < NO_OF_TASKS) && (strOSConfigTask != NULLPTR))
	{    
		/* Assigning the input struct parameters into the selected priority struct location inside the tasks array */
		gastrOSConfigTasks[u8Priority] = *strOSConfigTask;	
				
		STATUS = OS_TASK_ADDED;
	}
	else
	{
		STATUS = OS_ERROR;
	}
	return STATUS;
}
enuOSErrorStatus_t OS_modify_task(u8 u8Priority, strOSConfigTask_t* strOSConfigTask)
{
	enuOSErrorStatus_t STATUS;
	
	/* Checking if the selected task struct is FULL, the task priority is less than the maximum number of tasks,
		and the input struct is not empty */
	if((OS_IsPriorityEmpty(&gastrOSConfigTasks[u8Priority]) == PRIORITY_FULL) && 
		(u8Priority < NO_OF_TASKS) && (strOSConfigTask != NULLPTR))
	{    
		/* Assigning the input struct parameters into the selected priority struct location inside the tasks array */
		gastrOSConfigTasks[u8Priority] = *strOSConfigTask;	
				
		STATUS = OS_TASK_MODIFED;
	}
	else
	{
		STATUS = OS_ERROR;
	}
	return STATUS;
}
void OS_Run(void)
{
	u8 u8TaskP;
	 timer_start(TIMER0_SCALER_64);
	while(1)
	{   
         /* Checking if start flag is raised */
		 OS_Start_Check_pd ();
		/* Checking if the ISR flag is raised */
		if(gu8TFlag == HIGH&&gu8os_flag==HIGH)  //100ms
		{
			/* Looping on each task to implement them based on priority */
			for(u8TaskP = 1; u8TaskP <= NO_OF_TASKS; u8TaskP++)
			{    
				
				
				/* Checking if the selected task struct is not empty and the task's periodicity has come */
				if(
				(OS_IsPriorityEmpty(&gastrOSConfigTasks[u8TaskP]) != PRIORITY_EMPTY)
				 &&
				((gu64TaskCounter % (gastrOSConfigTasks[u8TaskP].u8Periodicity) ) == 0)
				)
				{
					/* Calling the task's function */
					gastrOSConfigTasks[u8TaskP].pfTask();
// 					LCD_SetCursor(0,0);               //JUST FOR TEST 
// 					LCD_WriteNumber(u8TaskP);
					
				}
		
			}
			/* Lowering the ISR flag to detect the next tick time */
			gu8TFlag  = LOW;
		}
		else
		{
		}
	}
}
enuOSErrorStatus_t OS_DeleteTask(u8 u8Priority)
{
	if((OS_IsPriorityEmpty(&gastrOSConfigTasks[u8Priority]) !=PRIORITY_EMPTY ) && (u8Priority < NO_OF_TASKS))
	
	{
		gastrOSConfigTasks[u8Priority]= gstrOSEmptyTask;
		
		return OS_TASK_DELETED;
		
	}
	else
	{
		return OS_ERROR;
	}
}
 void OS_Start_Check ()
 { 
	  u8 states;
	 BUTTON_read(button0,&states);
	 if (states==LOW)
	 {
		 DIO_writepin(PINB2,HIGH);
		 gu8os_flag=HIGH;
		 timer_start(TIMER0_SCALER_64);
		 
		 while(gu8os_flag==LOW)
		 {
			 BUTTON_read(button0,&states);
		 }
	 }
 } 
void OS_disable()
 {
	 gu8os_flag=LOW;
	 DIO_writepin(PINB0,LOW);
	 timer0_stop();

	 
	 
 }
 
  static void OS_Start_Check_pd ()
  {
	  u8 states;
	BUTTON_read(button0,&states);
	if (states==HIGH)
	{
			 DIO_writepin(PINB0,HIGH);
			 gu8os_flag=HIGH;
			 timer_start(TIMER0_SCALER_64);

		while(states==HIGH)
		{
			BUTTON_read(button0,&states);
		}
	}
  }