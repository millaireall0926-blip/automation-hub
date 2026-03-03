
#include "define.h"


XTmrCtr TimerCounter; /* The instance of the Tmrctr Device */
XTmrCtr *TmrCtrInstancePtr = &TimerCounter;


// TIMER_COUNTER_0 for timeout
u8 timeout_start( u32 timeout ) {
	u32 ControlStatus;

	XTmrCtr_SetControlStatusReg(TMRCTR_BASE, TIMER_COUNTER_0, 0x0);

	/*
	 * Set the value that is loaded into the timer counter and cause it to
	 * be loaded into the timer counter
	 */
	XTmrCtr_SetLoadReg(TMRCTR_BASE, TIMER_COUNTER_0, timeout);
	XTmrCtr_LoadTimerCounterReg(TMRCTR_BASE, TIMER_COUNTER_0);

	/*
	 * Clear the Load Timer bit in the Control Status Register
	 */
	ControlStatus = XTmrCtr_GetControlStatusReg(TMRCTR_BASE, TIMER_COUNTER_0) | XTC_CSR_DOWN_COUNT_MASK;
	XTmrCtr_SetControlStatusReg(TMRCTR_BASE, TIMER_COUNTER_0, ControlStatus & (~XTC_CSR_LOAD_MASK));

	XTmrCtr_Enable(TMRCTR_BASE, TIMER_COUNTER_0);

    return 0;
}

u32 timeout_value( void ) {
	u32 Value;

	Value = XTmrCtr_GetTimerCounterReg(TMRCTR_BASE, TIMER_COUNTER_0);

    return Value;
}

u8 timeout_stop( void ) {
	XTmrCtr_Disable(TMRCTR_BASE, TIMER_COUNTER_0);

    return 0;
}


// TIMER_COUNTER_1 for timer
u8 timer_count_start( u32 timeout ) {
	u32 ControlStatus;

	XTmrCtr_SetControlStatusReg(TMRCTR_BASE, TIMER_COUNTER_1, 0x0);

	/*
	 * Set the value that is loaded into the timer counter and cause it to
	 * be loaded into the timer counter
	 */
	XTmrCtr_SetLoadReg(TMRCTR_BASE, TIMER_COUNTER_1, timeout);
	XTmrCtr_LoadTimerCounterReg(TMRCTR_BASE, TIMER_COUNTER_1);

	/*
	 * Clear the Load Timer bit in the Control Status Register
	 */
	ControlStatus = XTmrCtr_GetControlStatusReg(TMRCTR_BASE, TIMER_COUNTER_1) | XTC_CSR_DOWN_COUNT_MASK;
	XTmrCtr_SetControlStatusReg(TMRCTR_BASE, TIMER_COUNTER_1, ControlStatus & (~XTC_CSR_LOAD_MASK));

	XTmrCtr_Enable(TMRCTR_BASE, TIMER_COUNTER_1);

    return 0;
}

u32 timer_count_value( void ) {
	u32 Value;

	Value = XTmrCtr_GetTimerCounterReg(TMRCTR_BASE, TIMER_COUNTER_1);

    return Value;
}

u8 timer_count_stop( void ) {
	XTmrCtr_Disable(TMRCTR_BASE, TIMER_COUNTER_1);

    return 0;
}
