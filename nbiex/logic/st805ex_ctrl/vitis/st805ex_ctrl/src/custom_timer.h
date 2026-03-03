
#ifndef _CUSTOM_TIMER_H_
#define _CUSTOM_TIMER_H_

#include "xtmrctr.h"
#include "xtmrctr_l.h"

#include "define.h"


/* defines */
#define TMRCTR_DEVICE_ID  XPAR_TMRCTR_0_DEVICE_ID
#define TIMER_COUNTER_0   0
#define TIMER_COUNTER_1   1


#define TIMER_1US				  100
#define TIMER_1MS				  (1000 * TIMER_1US)
#define TIMER_1S				  (1000 * TIMER_1MS)

#define ADC_TIMEOUT			  (  1 * TIMER_1MS)
#define I2C_TIMEOUT			  (  1 * TIMER_1MS)
#define	SPI_TIMEOUT			  (  1 * TIMER_1MS)


/* struct */


/* extern vars */


/* functions */
 u8 timeout_start( u32 timeout );
u32 timeout_value( void );
 u8 timeout_stop( void );

 u8 timer_count_start( u32 timeout );
u32 timer_count_value( void );
 u8 timer_count_stop( void );


#endif
