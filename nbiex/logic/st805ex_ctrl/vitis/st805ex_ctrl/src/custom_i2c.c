
#include "define.h"


unsigned i2c_recv( UINTPTR BaseAddress, u8 *BufferPtr, unsigned ByteCount, u8 Option )
{
	u32 CntlReg;
	u32 IntrStatusMask;
	u32 IntrStatus;

	u32 timeout = I2C_TIMEOUT;

	while (ByteCount > 0) {
		if (ByteCount == 1) {
			IntrStatusMask = XIIC_INTR_ARB_LOST_MASK | XIIC_INTR_BNB_MASK;
		} else {
			IntrStatusMask = XIIC_INTR_ARB_LOST_MASK | XIIC_INTR_TX_ERROR_MASK | XIIC_INTR_BNB_MASK;
		}

		timeout = I2C_TIMEOUT;
		while (--timeout > 0) {
			IntrStatus = XIic_ReadIisr(BaseAddress);

			if (IntrStatus & XIIC_INTR_RX_FULL_MASK) {
				break;
			}

			if (IntrStatus & IntrStatusMask) {
				return ByteCount;
			}
		}
		if ( timeout == 0 )
			return ByteCount;

		CntlReg = XIic_ReadReg(BaseAddress,  XIIC_CR_REG_OFFSET);

		if (ByteCount == 1) {
			if (Option == XIIC_STOP) {
				XIic_WriteReg(BaseAddress,  XIIC_CR_REG_OFFSET, XIIC_CR_ENABLE_DEVICE_MASK);
			}
		}

		if (ByteCount == 2) {
			XIic_WriteReg(BaseAddress,  XIIC_CR_REG_OFFSET, CntlReg | XIIC_CR_NO_ACK_MASK);
		}

		*BufferPtr++ = (u8) XIic_ReadReg(BaseAddress, XIIC_DRR_REG_OFFSET);

		if ((ByteCount == 1) && (Option == XIIC_REPEATED_START)) {
			XIic_WriteReg(BaseAddress,  XIIC_CR_REG_OFFSET, XIIC_CR_ENABLE_DEVICE_MASK | XIIC_CR_MSMS_MASK | XIIC_CR_REPEATED_START_MASK);
		}

		XIic_ClearIisr(BaseAddress, XIIC_INTR_RX_FULL_MASK | XIIC_INTR_TX_ERROR_MASK | XIIC_INTR_ARB_LOST_MASK);
		ByteCount--;
	}

	if (Option == XIIC_STOP) {
		timeout = I2C_TIMEOUT;
		while (--timeout > 0) {
			if (XIic_ReadIisr(BaseAddress) & XIIC_INTR_BNB_MASK) {
				break;
			}
		}
		if ( timeout == 0 )
			return ByteCount;
	}

	return ByteCount;
}

unsigned i2c_send( UINTPTR BaseAddress, u8 *BufferPtr, unsigned ByteCount, u8 Option )
{
	u32 IntrStatus;

	u32 timeout = I2C_TIMEOUT;

	while (ByteCount > 0) {
		timeout = I2C_TIMEOUT;
		while (--timeout > 0) {
			IntrStatus = XIic_ReadIisr(BaseAddress);

			if (IntrStatus & (XIIC_INTR_TX_ERROR_MASK | XIIC_INTR_ARB_LOST_MASK | XIIC_INTR_BNB_MASK)) {
				return ByteCount;
			}

			if (IntrStatus & XIIC_INTR_TX_EMPTY_MASK) {
				break;
			}
		}

		if ( timeout == 0 ) {
			return 0xFFFF;
		}

		if (ByteCount > 1) {
			XIic_WriteReg(BaseAddress,  XIIC_DTR_REG_OFFSET, *BufferPtr++);
		}
		else {
			if (Option == XIIC_STOP) {
				XIic_WriteReg(BaseAddress,  XIIC_CR_REG_OFFSET, XIIC_CR_ENABLE_DEVICE_MASK | XIIC_CR_DIR_IS_TX_MASK);
			}

			XIic_WriteReg(BaseAddress,  XIIC_DTR_REG_OFFSET, *BufferPtr++);

			if (Option == XIIC_REPEATED_START) {
				XIic_ClearIisr(BaseAddress, XIIC_INTR_TX_EMPTY_MASK);
				timeout = I2C_TIMEOUT;
				while (--timeout > 0) {
					IntrStatus = XIic_ReadIisr(BaseAddress);
					if (IntrStatus & XIIC_INTR_TX_EMPTY_MASK) {
						XIic_WriteReg(BaseAddress, XIIC_CR_REG_OFFSET, XIIC_CR_REPEATED_START_MASK | XIIC_CR_ENABLE_DEVICE_MASK | XIIC_CR_DIR_IS_TX_MASK | XIIC_CR_MSMS_MASK);
						break;
					}
				}

				if ( timeout == 0 ) {
					return ByteCount;
				}
			}
		}

		XIic_ClearIisr(BaseAddress, XIIC_INTR_TX_EMPTY_MASK);
		ByteCount--;
	}

	if (Option == XIIC_STOP) {
		timeout = I2C_TIMEOUT;
		while ( --timeout > 0 ) {
			if (XIic_ReadIisr(BaseAddress) & XIIC_INTR_BNB_MASK) {
				break;
			}
		}

		if ( timeout == 0 ) {
			return 0;
		}
	}

	return ByteCount;
}

unsigned i2c_recv_data( UINTPTR BaseAddress, u8 Address, u8 *CmdBufferPtr, unsigned CmdByteCount, u8 *BufferPtr, unsigned ByteCount, u8 Option )
{
	u32 CntlReg;
	unsigned RemainingByteCount;
	volatile u32 StatusReg;

	intr_disable();

	u32 timeout = I2C_TIMEOUT;

	XIic_ClearIisr(BaseAddress, XIIC_INTR_RX_FULL_MASK | XIIC_INTR_TX_ERROR_MASK | XIIC_INTR_ARB_LOST_MASK);
	XIic_WriteReg(BaseAddress,  XIIC_RFD_REG_OFFSET, 0);
	if (XIic_WaitBusFree(BaseAddress) != XST_SUCCESS) {
		intr_enable();
		return 0;
	}

	CntlReg = XIic_ReadReg(BaseAddress,  XIIC_CR_REG_OFFSET);
	if ((CntlReg & XIIC_CR_REPEATED_START_MASK) == 0) {
		XIic_Send7BitAddress(BaseAddress, Address, XIIC_WRITE_OPERATION);
		XIic_ClearIisr(BaseAddress, XIIC_INTR_TX_EMPTY_MASK | XIIC_INTR_TX_ERROR_MASK | XIIC_INTR_ARB_LOST_MASK);
		XIic_WriteReg(BaseAddress,  XIIC_CR_REG_OFFSET, XIIC_CR_MSMS_MASK | XIIC_CR_DIR_IS_TX_MASK | XIIC_CR_ENABLE_DEVICE_MASK);

		StatusReg = XIic_ReadReg(BaseAddress,  XIIC_SR_REG_OFFSET);
		//while ((StatusReg & XIIC_SR_BUS_BUSY_MASK) == 0) {
		timeout = I2C_TIMEOUT;
		while ( ( ( StatusReg & XIIC_SR_BUS_BUSY_MASK ) == 0 ) && ( --timeout > 0 ) ) {
			StatusReg = XIic_ReadReg(BaseAddress, XIIC_SR_REG_OFFSET);
		}

		XIic_ClearIisr(BaseAddress, XIIC_INTR_BNB_MASK);

		if ( timeout == 0 ) {
			intr_enable();
			return 0;
		}
	}
	else {
		XIic_Send7BitAddress(BaseAddress, Address, XIIC_WRITE_OPERATION);
	}

	if ( i2c_send(BaseAddress, CmdBufferPtr, CmdByteCount, XIIC_REPEATED_START) == 0xFFFF ) {
		intr_enable();
		return 0xFFFF;
	}

	CntlReg = XIic_ReadReg(BaseAddress,  XIIC_CR_REG_OFFSET);
	if ((CntlReg & XIIC_CR_REPEATED_START_MASK) == 0) {
		XIic_Send7BitAddress(BaseAddress, Address, XIIC_READ_OPERATION);

		CntlReg = XIIC_CR_MSMS_MASK | XIIC_CR_ENABLE_DEVICE_MASK;
		if (ByteCount == 1) {
			CntlReg |= XIIC_CR_NO_ACK_MASK;
		}

		XIic_WriteReg(BaseAddress,  XIIC_CR_REG_OFFSET, CntlReg);
		StatusReg = XIic_ReadReg(BaseAddress,  XIIC_SR_REG_OFFSET);

		//while ((StatusReg & XIIC_SR_BUS_BUSY_MASK) == 0) {
		timeout = I2C_TIMEOUT;
		while ( ( ( StatusReg & XIIC_SR_BUS_BUSY_MASK ) == 0 ) && ( --timeout > 0 ) ) {
			StatusReg = XIic_ReadReg(BaseAddress, XIIC_SR_REG_OFFSET);
		}

		XIic_ClearIisr(BaseAddress, XIIC_INTR_BNB_MASK);

		if ( timeout == 0 ) {
			intr_enable();
			return 0;
		}
	} else {
		CntlReg &= ~XIIC_CR_DIR_IS_TX_MASK;
		if (ByteCount == 1) {
			CntlReg |= XIIC_CR_NO_ACK_MASK;
		}
		XIic_WriteReg(BaseAddress,  XIIC_CR_REG_OFFSET, CntlReg);
		XIic_Send7BitAddress(BaseAddress, Address, XIIC_READ_OPERATION);
	}

	RemainingByteCount = i2c_recv(BaseAddress, BufferPtr, ByteCount, Option);

	CntlReg = XIic_ReadReg(BaseAddress,  XIIC_CR_REG_OFFSET);
	if ((CntlReg & XIIC_CR_REPEATED_START_MASK) == 0) {
		XIic_WriteReg(BaseAddress,  XIIC_CR_REG_OFFSET, 0);
	}

	if (XIic_WaitBusFree(BaseAddress) != XST_SUCCESS) {
		intr_enable();
		return 0;
	}

	intr_enable();
	return ByteCount - RemainingByteCount;
}

unsigned i2c_send_data( UINTPTR BaseAddress, u8 Address, u8 *BufferPtr, unsigned ByteCount, u8 Option ) {
	unsigned RemainingByteCount;
	u32 ControlReg;
	volatile u32 StatusReg;

	intr_disable();

	u32 timeout = I2C_TIMEOUT;

	if (XIic_WaitBusFree(BaseAddress) != XST_SUCCESS) {
		intr_enable();
		return 0;
	}

	ControlReg = XIic_ReadReg(BaseAddress,  XIIC_CR_REG_OFFSET);
	if ((ControlReg & XIIC_CR_REPEATED_START_MASK) == 0) {
		XIic_Send7BitAddress(BaseAddress, Address, XIIC_WRITE_OPERATION);
		XIic_ClearIisr(BaseAddress, XIIC_INTR_TX_EMPTY_MASK | XIIC_INTR_TX_ERROR_MASK | XIIC_INTR_ARB_LOST_MASK);
		XIic_WriteReg(BaseAddress,  XIIC_CR_REG_OFFSET, XIIC_CR_MSMS_MASK | XIIC_CR_DIR_IS_TX_MASK | XIIC_CR_ENABLE_DEVICE_MASK);
		StatusReg = XIic_ReadReg(BaseAddress,  XIIC_SR_REG_OFFSET);

		//while ((StatusReg & XIIC_SR_BUS_BUSY_MASK) == 0) {
		timeout = I2C_TIMEOUT;
		while ( ( ( StatusReg & XIIC_SR_BUS_BUSY_MASK ) == 0 ) && ( --timeout > 0 ) ) {
			StatusReg = XIic_ReadReg(BaseAddress, XIIC_SR_REG_OFFSET);
		}

		XIic_ClearIisr(BaseAddress, XIIC_INTR_BNB_MASK);

		if ( timeout == 0 ) {
			intr_enable();
			return 0;
		}
	} else {
		XIic_Send7BitAddress(BaseAddress, Address, XIIC_WRITE_OPERATION);
	}

	RemainingByteCount = i2c_send(BaseAddress, BufferPtr, ByteCount, Option);

	if ( RemainingByteCount == 0xFFFF ) {
		intr_enable();
		return 0xFFFF;
	}

	ControlReg = XIic_ReadReg(BaseAddress,  XIIC_CR_REG_OFFSET);
	if ((ControlReg & XIIC_CR_REPEATED_START_MASK) == 0) {
		if ((ControlReg & XIIC_CR_MSMS_MASK) != 0) {
			XIic_WriteReg(BaseAddress,  XIIC_CR_REG_OFFSET, (ControlReg & ~XIIC_CR_MSMS_MASK));
			StatusReg = XIic_ReadReg(BaseAddress, XIIC_SR_REG_OFFSET);

			//while ((StatusReg & XIIC_SR_BUS_BUSY_MASK) != 0) {
			timeout = I2C_TIMEOUT;
			while ( ( ( StatusReg & XIIC_SR_BUS_BUSY_MASK ) == 0 ) && ( --timeout > 0 ) ) {
				StatusReg = XIic_ReadReg(BaseAddress, XIIC_SR_REG_OFFSET);
			}
		}

		if ( timeout == 0 ) {
			intr_enable();
			return 0;
		}

		if ((XIic_ReadReg(BaseAddress, XIIC_SR_REG_OFFSET) & XIIC_SR_ADDR_AS_SLAVE_MASK) != 0) {
			XIic_WriteReg(BaseAddress,  XIIC_CR_REG_OFFSET, 0);
		}
	}

	intr_enable();
	return ByteCount - RemainingByteCount;
}

u8 i2c_addr_chk( UINTPTR BaseAddress ) {
	u32 i = 0;
	u8 wbuf[8];
	u8 sa_cnt = 0;
	u8 sa_value[32];
	u32 sts = 0;

	wbuf[0] = 0;

	for ( i = 0; i < 0x80; i++ ) {
		sts = i2c_send_data( BaseAddress, i, wbuf, 1, XIIC_STOP );
		if ( sts == 0xFFFF )
			return 1;
		else if ( sts == 1 ) {
			sa_value[sa_cnt] = i;
			sa_cnt++;
		}
	}

	if ( sa_cnt == 0 )
		return 1;

	xil_printf( "Sacn.SA.Cnt[%d] : ", sa_cnt );
	for ( i=0; i<sa_cnt; i++ ) {
		xil_printf( "%02X ", sa_value[i] );
	}
	xil_printf( "\r\n" );

	return 0;
}
