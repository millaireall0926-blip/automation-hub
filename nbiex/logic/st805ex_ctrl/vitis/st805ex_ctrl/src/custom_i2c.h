
#ifndef _CUSTOM_I2C_H_
#define _CUSTOM_I2C_H_

#include "xiic_l.h"

#include "define.h"


/* defines */


/* struct */
#pragma pack (1)
struct _I2C_TARGET {
	u32	base;
	u8	slave_addr;
	u8	device[16];
	u8	name[16];
};
#pragma pack ()


/* extern vars */


/* functions */
unsigned i2c_recv( UINTPTR BaseAddress, u8 *BufferPtr, unsigned ByteCount, u8 Option );
unsigned i2c_send( UINTPTR BaseAddress, u8 *BufferPtr, unsigned ByteCount, u8 Option );
unsigned i2c_recv_data( UINTPTR BaseAddress, u8 Address, u8 *CmdBufferPtr, unsigned CmdByteCount, u8 *BufferPtr, unsigned ByteCount, u8 Option );
unsigned i2c_send_data( UINTPTR BaseAddress, u8 Address, u8 *BufferPtr, unsigned ByteCount, u8 Option );

u8 i2c_addr_chk( UINTPTR BaseAddress );


#endif
