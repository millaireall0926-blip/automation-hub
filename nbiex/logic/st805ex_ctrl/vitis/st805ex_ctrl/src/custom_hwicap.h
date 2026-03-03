
#ifndef _CUSTOM_HWICAP_H_
#define _CUSTOM_HWICAP_H_

#include "xhwicap_i.h"
#include "xhwicap_l.h"
#include "xhwicap.h"


/* defines */
#ifndef SDT
#define HWICAP_DEVICE_ID    XPAR_HWICAP_0_DEVICE_ID
#else
#define HWICAP_BASEADDR     XPAR_AXI_HWICAP_0_BASEADDR
#endif


/* struct */


/* extern vars */


/* functions */
 u8 hwicap_init(void);
u32 reboot_fpga(void);


#endif
