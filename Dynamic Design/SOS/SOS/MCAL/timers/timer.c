/*
 * CFile1.c
 *
 * Created: 5/12/2023 3:36:31 PM
 *  Author: atef
 */ 
#include "timer.h"

/*************************Pointer to functions to be assigned to ISR**************************************/
static void (*Timer0_OVF_Fptr) (void)=NULLPTR;
static void (*Timer1_OVF_Fptr) (void)=NULLPTR;
static void (*Timer1_OCA_Fptr) (void)=NULLPTR;
static void (*Timer1_OCB_Fptr) (void)=NULLPTR;
static void (*Timer1_ICU_Fptr) (void)=NULLPTR;
static void (*Timer2_OVF_Fptr) (void)=NULLPTR;
/*********************************************************************************************************/
/*********************************************************************************************************
/                                      global variables                                                   /
/***********************************************************************************************************/
 // used in TIME_0_DELAY_MS
static double g_ovfNum  ; 
static double g_time ;

 // used in TIMER_2_INT
u8 car_mode = 0;
s32 mode_ovf = 0;
static s32 ovf = 0;
u32 gu32_T1_OVF_TICKS;
u8 g_speed_flag =1  ;
/**********************************************************/
/*                       Timer-0                          */
/**********************************************************/

void TIMER0_Init(Timer0Mode_type mode)
{
	switch (mode)
	{
		case TIMER0_NORMAL_MODE:
		CLR_BIT(TCCR0,WGM00);
		CLR_BIT(TCCR0,WGM01);
		break;
		case TIMER0_PHASECORRECT_MODE:
		SET_BIT(TCCR0,WGM00);
		CLR_BIT(TCCR0,WGM01);
		break;
		case TIMER0_CTC_MODE:
		CLR_BIT(TCCR0,WGM00);
		SET_BIT(TCCR0,WGM01);
		break;
		case TIMER0_FASTPWM_MODE:
		SET_BIT(TCCR0,WGM00);
		SET_BIT(TCCR0,WGM01);
		break;
	}
}

EN_timerError_t timer_start(Timer0Scaler_type scaler)
{ 
	switch (scaler)
	{
		case TIMER0_STOP:
		case TIMER0_SCALER_1:
		case TIMER0_SCALER_8:
		case TIMER0_SCALER_64:
		case TIMER0_SCALER_256:
		case TIMER0_SCALER_1024:
		case EXTERNALl_FALLING:
		case EXTERNAL_RISING:
		TCCR0&=0XF8;//0b11111000
		TCCR0|=scaler;
		return TIMER_OK;
	
		default:
		return INVALID_PRESCALER;
	}
}

void timer_setTimerValue(u8 timerValue)
{
	TCNT0 = timerValue;
}

void timer_reset()
{
	TCCR0 &= 0x00;
	CLR_BIT(TIFR, 0);
	TCNT0 = 0;

}
void timer0_stop()
{
	TCCR0 &= 0x00;

}
/* Timer 0 Call Back functions*/

void TIMER0_OV_SetCallBack(void(*LocalFptr)(void))
{
	Timer0_OVF_Fptr=LocalFptr;
}

/*Timer-0 ISR functions*/

ISR(TIMER0_OVF_vect)
{   
	if(Timer0_OVF_Fptr!=NULLPTR)
	{
		Timer0_OVF_Fptr();
	}
}

/*Timer-0 Interrup*/

void TIMER0_OV_InterruptEnable(void)
{
	SET_BIT(TIMSK,TOIE0);
}
void TIMER0_OV_InterruptDisable(void)
{
	CLR_BIT(TIMSK,TOIE0);
}
void TIMER0_OC_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE0);
}
void TIMER0_OC_InterruptDisable(void)
{
	CLR_BIT(TIMSK,OCIE0);
}


/**********************************************************/
/*                       Timer-1                          */
/**********************************************************/

/*Timer-1 functions*/

void Timer1_Init( Timer1Mode_type mode,Timer1Scaler_type scaler)
{
	switch (mode)
	{
		case TIMER1_NORMAL_MODE:
		CLR_BIT(TCCR1A,WGM10);
		CLR_BIT(TCCR1A,WGM11);
		CLR_BIT(TCCR1B,WGM12);
		CLR_BIT(TCCR1B,WGM13);
		break;
		case TIMER1_CTC_ICR_TOP_MODE:
		CLR_BIT(TCCR1A,WGM10);
		CLR_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;

		case TIMER1_CTC_OCRA_TOP_MODE:
		CLR_BIT(TCCR1A,WGM10);
		CLR_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		CLR_BIT(TCCR1B,WGM13);
		break;

		case TIMER1_FASTPWM_ICR_TOP_MODE:
		CLR_BIT(TCCR1A,WGM10);
		SET_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;

		case TIMER1_FASTPWM_OCRA_TOP_MODE:
		SET_BIT(TCCR1A,WGM10);
		SET_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;
	}

	TCCR1B&=0XF8;
	TCCR1B|=scaler;

}
void Timer1_OCRA1Mode(OC1A_Mode_type oc1a_mode)
{
	switch (oc1a_mode)
	{
		case OCRA_DISCONNECTED:
		CLR_BIT(TCCR1A,COM1A0);
		CLR_BIT(TCCR1A,COM1A1);
		break;
		case OCRA_TOGGLE:
		SET_BIT(TCCR1A,COM1A0);
		CLR_BIT(TCCR1A,COM1A1);
		break;
		case OCRA_NON_INVERTING:
		CLR_BIT(TCCR1A,COM1A0);
		SET_BIT(TCCR1A,COM1A1);
		break;
		case OCRA_INVERTING:
		SET_BIT(TCCR1A,COM1A0);
		SET_BIT(TCCR1A,COM1A1);
		break;
	}
}
void Timer1_OCRB1Mode(OC1B_Mode_type oc1b_mode)
{
	switch (oc1b_mode)
	{
		case OCRB_DISCONNECTED:
		CLR_BIT(TCCR1A,COM1B0);
		CLR_BIT(TCCR1A,COM1B1);
		break;
		case OCRB_TOGGLE:
		SET_BIT(TCCR1A,COM1B0);
		CLR_BIT(TCCR1A,COM1B1);
		break;
		case OCRB_NON_INVERTING:
		CLR_BIT(TCCR1A,COM1B0);
		SET_BIT(TCCR1A,COM1B1);
		break;
		case OCRB_INVERTING:
		SET_BIT(TCCR1A,COM1B0);
		SET_BIT(TCCR1A,COM1B1);
		break;
	}
}
void Timer1_InputCaptureEdge(ICU_Edge_type edge)
{
	if(edge==RISING)
	SET_BIT(TCCR1B,ICES1);

	else if(edge==FALLING)
	CLR_BIT(TCCR1B,ICES1);
}
void timer1_ReadCounts(u16 *pu16_timer1Counts)
{
	*pu16_timer1Counts =TCNT1 ;

}
void timer1_setTimerValue(u8 timerValue)
{
	TCNT1 = timerValue ;

}


/*Timer-1 Interrupt functions*/

void Timer1_ICU_InterruptEnable(void)
{
	SET_BIT(TIMSK,TICIE1);
}
void Timer1_ICU_InterruptDisable(void)
{
	CLR_BIT(TIMSK,TICIE1);
}
void Timer1_OVF_InterruptEnable(void)
{
	SET_BIT(TIMSK,TOIE1);
}
void Timer1_OVF_InterruptDisable(void)
{
	CLR_BIT(TIMSK,TOIE1);
}
void Timer1_OCA_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE1A);
}
void Timer1_OCA_InterruptDisable(void)
{
	CLR_BIT(TIMSK,OCIE1A);
}
void Timer1_OCB_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE1B);
}
void Timer1_OCB_InterruptDisable(void)
{
	CLR_BIT(TIMSK,OCIE1B);
}

/*Timer-1 Call Back functions*/

void Timer1_OVF_SetCallBack(void(*LocalFptr)(void))
{
	Timer1_OVF_Fptr=LocalFptr;
}
void Timer1_OCA_SetCallBack(void(*LocalFptr)(void))
{
	Timer1_OCA_Fptr=LocalFptr;
}
void Timer1_OCB_SetCallBack(void(*LocalFptr)(void))
{
	Timer1_OCB_Fptr=LocalFptr;
}
void Timer1_ICU_SetCallBack(void(*LocalFptr)(void))
{
	Timer1_ICU_Fptr=LocalFptr;
}

/*Timer 1 ISR functions*/

ISR(TIMER1_OVF_vect)
{
	if(Timer1_OVF_Fptr!=NULLPTR)
	{
		Timer1_OVF_Fptr();
	}
}
ISR(TIMER1_OCA_vect)
{
	if(Timer1_OCA_Fptr!=NULLPTR)
	{
		Timer1_OCA_Fptr();
	}
}
ISR(TIMER1_OCB_vect)
{
	if(Timer1_OCB_Fptr!=NULLPTR)
	{
		Timer1_OCB_Fptr();
	}
}
ISR(TIMER1_ICU_vect)
{
	if(Timer1_ICU_Fptr!=NULLPTR)
	{
		Timer1_ICU_Fptr();
	}
}
/**********************************************************/
/*                       Timer-2                          */
/**********************************************************/
void TIMER2_Init(Timer2Mode_type mode)
{
	switch (mode)
	{
		case TIMER2_NORMAL_MODE:
		CLR_BIT(TCCR2,WGM20);
		CLR_BIT(TCCR2,WGM21);
		break;
		case TIMER2_PHASECORRECT_MODE:
		SET_BIT(TCCR2,WGM20);
		CLR_BIT(TCCR2,WGM21);
		break;
		case TIMER2_CTC_MODE:
		CLR_BIT(TCCR2,WGM20);
		SET_BIT(TCCR2,WGM21);
		break;
		case TIMER2_FASTPWM_MODE:
		SET_BIT(TCCR2,WGM20);
		SET_BIT(TCCR2,WGM21);
		break;
	}
}
void TIMER_2_stop(void)
{

	CLR_BIT(TCCR2,CS20);
	CLR_BIT(TCCR2,CS21);
	CLR_BIT(TCCR2,CS22);

}
EN_timerError_t TIMER_2_setIntialValue(u8 a_value)
{
	EN_timerError_t errorStatus = TIMER_OK;

	if(a_value < TIMR2_MAX_VALUE && a_value >= 0)
	{

		TCNT2 = a_value ;
	}
	else
	{
		errorStatus = INVALID_VALUE;
	}
	return errorStatus ;
}
EN_timerError_t timer2_start(Timer2Scaler_type scaler)
{
	switch (scaler)
	{
		case TIMER2_STOP:
		case TIMER2_SCALER_1:
		case TIMER2_SCALER_8:
		case TIMER2_SCALER_32:
		case TIMER2_SCALER_64:
		case TIMER2_SCALER_128:
		case TIMER2_SCALER_256:
		case TIMER2_SCALER_1024:
		TCCR2&=0XF8;//0b11111000
		TCCR2|=scaler;
		return TIMER_OK;
		default:
		return INVALID_PRESCALER;
	}
}
EN_timerError_t TIMER_2_OvfNum(double overflow)
{
	EN_timerError_t errorStatus = TIMER_OK;
	double num_ovf = 0;
	if (overflow > 0)
	{

		while(num_ovf<overflow)
		{

			while(READ_BIT(TIFR,TOV2)==0);
			SET_BIT(TIFR,TOV2);
			num_ovf++;
		}
		num_ovf = 0;
	}
	else if (overflow <= 0)
	{
		overflow = 1 ;
		while(num_ovf<overflow)
		{

			while(READ_BIT(TIFR,TOV2)==0);
			SET_BIT(TIFR,TOV2);
			num_ovf++;
		}
		num_ovf = 0;
	}

	else
	{
		errorStatus = INVALID_OVF;
	}

	return errorStatus;
}
void TIMER_2_DELAY_MS(double time_ms)
{
	double ovfNum2  ;
	double t2 ;
	t2 = time_ms/1000 ;
	ovfNum2 = ceil (t2 / 0.000256) ;
	TIMER_2_setIntialValue(0);
	TIMER_2_OvfNum(ovfNum2);

}
void TIMER_2_INT()
{
	TIMER2_OV_InterruptEnable();
	TIMER2_Init(TIMER2_NORMAL_MODE);
	TIMER_2_setIntialValue(0);
	timer2_start(TIMER2_SCALER_1);
}
/*Timer-2 Interrupt functions*/
void TIMER2_OV_InterruptEnable(void)
{
	SET_BIT(TIMSK,TOIE2);
}
void TIMER2_OV_InterruptDisable(void)
{
	CLR_BIT(TIMSK,TOIE2);
}
/*Timer-2 Call Back functions*/
void TIMER2_OV_SetCallBack(void(*LocalFptr)(void))
{
	Timer2_OVF_Fptr=LocalFptr;
}
/*Timer-2 ISR functions*/
ISR(TIMER2_OV_vect)
{
	if(Timer2_OVF_Fptr!=NULLPTR)
	{
		Timer2_OVF_Fptr();
	}
}








// ISR(TIMER2_OV_vect)
// {
// 	if(g_speed_flag == 1)
// 	{
// 		
// 		if (ovf < mode_ovf ){
// 			ovf++;
// 		}
// 		else if ( ovf == mode_ovf && mode_ovf!=0)
// 		{
// 			ovf =0 ;
// 			if (car_mode ==0)
// 			{
// 				car_mode=1;	
// 			}
// 			else
// 			{
// 				car_mode == 0;
// 			}
// 			
// 		}		
// 	}
// }
	
/*
u32 set_time (u16 PRE_SCALER,u32 DesiedTime)
{
	
	double oneTickTime = (((f64) PRE_SCALER /F_CPU ) * 1000); // in ms
	double MaxTime = oneTickTime * 256; // in ms
	u32 overflowsCount = (u32)(DesiedTime / MaxTime);
	u8 timerInitValue;
	if (overflowsCount > 1)
	{
		timerInitValue = (256 - ((DesiedTime - (MaxTime * overflowsCount) ) / oneTickTime));
	}
	else
	{
		timerInitValue = ((MaxTime - DesiedTime)/oneTickTime);
	}
	
	TCNT0 = timerInitValue;
	
	
	
	return overflowsCount;
	
}


*/
u32 set_time (u16 PRE_SCALER,f64 DesiedTime)
{
	double CPU_F = F_CPU;
	double oneTickTime = (PRE_SCALER /CPU_F *1000 ); // in ms
	double MaxTime = oneTickTime * 256; // in ms
	u32 overflowsCount = (u32)(DesiedTime / MaxTime);
	u8 timerInitValue;
	if (overflowsCount > 1)
	{
		timerInitValue = (256 - ((DesiedTime - (MaxTime * overflowsCount) ) / oneTickTime));
	}
	else
	{
		timerInitValue = ((MaxTime - DesiedTime)/oneTickTime);
	}
	
	// TCNT0 = timerInitValue;
	
	
	//LCD_SetCursor(1,0);
	//LCD_WriteNumber(timerInitValue);
	return overflowsCount;
	
}


void timer_delay(u16 Delay)
{
	double CPU_F = F_CPU;
	double oneTickTime = (8 / CPU_F ) * 1000; // in ms
	double maxDelay = oneTickTime * 256; // in ms
	int overflowsCount =(u32)((Delay / maxDelay));
	u8 timerInitValue;
	if (overflowsCount > 1)
	{
		timerInitValue = (u8)(256 - ((Delay - (maxDelay * (overflowsCount - 1))) / oneTickTime));
	}
	else
	{
		timerInitValue = (u8)((maxDelay - Delay)/oneTickTime);
	}
	TCNT0 = timerInitValue;

	timer_start(TIMER0_SCALER_8);


	while(overflowsCount--)
	{
		while((READ_BIT(TIFR, 0)) == 0);
		
		SET_BIT(TIFR, 0);
	}

	TCCR0 &= ~((1 << CS01) | (1 << CS00));
}

