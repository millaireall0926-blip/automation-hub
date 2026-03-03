
#ifndef _DBG_H_
#define _DBG_H_

#include "define.h"


/* defines */
#define _DEBUG_INFO_
#define _DEBUG_CTRI_

#ifdef _DEBUG_INFO_
#define DBG_INFO( fmt, ... )	\
{ \
	if (dbgFlag == 1) { \
		xil_printf( "" fmt, ##__VA_ARGS__ ); \
	} \
}
#else
#define DBG_INFO( fmt, ... )
#endif

#ifdef _DEBUG_CTRI_
#define DBG_CRIT( fmt, ... )	\
{ \
	xil_printf( "[CRIT %s:%d] " fmt, __func__, __LINE__, ##__VA_ARGS__ ); \
}
#else
#define DBG_CRIT( fmt, ... )
#endif

//#define DBG_CRIT( fmt, ... )	xil_printf( "[CRIT %s:%s:%d]" fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__ )


/* struct */


/* extern vars */


/* functions */


#endif
