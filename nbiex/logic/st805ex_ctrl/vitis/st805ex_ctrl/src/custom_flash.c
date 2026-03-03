
#include "define.h"


u8 spiRdBuf[PAGE_SIZE + READ_WRITE_EXTRA_BYTES + 4];
u8 spiWrBuf[PAGE_SIZE + READ_WRITE_EXTRA_BYTES];


/*****************************************************************************/
/**
*
* This function reads the Status register of the Numonyx Flash.
*
* @param	SpiPtr is a pointer to the instance of the Spi device.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		The status register content is stored at the second byte
*		pointed by the spiRdBuf.
*
******************************************************************************/
int SpiFlashGetStatus(XSpi *SpiPtr)
{
	int Status;

	/*
	 * Prepare the Write Buffer.
	 */
	spiWrBuf[BYTE1] = COMMAND_STATUSREG_READ;

	/*
	 * Initiate the Transfer.
	 */
	Status = XSpi_Transfer(SpiPtr, spiWrBuf, spiRdBuf,
						STATUS_READ_BYTES);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function waits till the Numonyx serial Flash is ready to accept next
* command.
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		This function reads the status register of the Buffer and waits
*.		till the WIP bit of the status register becomes 0.
*
******************************************************************************/
int SpiFlashWaitForFlashReady(XSpi *SpiPtr)
{
	int Status;
	u8 StatusReg;
	u32 timeout = SPI_TIMEOUT;

	timeout = SPI_TIMEOUT;
	while(--timeout > 0) {

		/*
		 * Get the Status Register. The status register content is
		 * stored at the second byte pointed by the spiRdBuf.
		 */
		Status = SpiFlashGetStatus(SpiPtr);
		if(Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

		/*
		 * Check if the flash is ready to accept the next command.
		 * If so break.
		 */
		StatusReg = spiRdBuf[1];
#ifdef _SIM_
		StatusReg = 0;
#endif
		if((StatusReg & FLASH_SR_IS_READY_MASK) == 0) {
			break;
		}
	}

	if ( timeout == 0 ) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function reads the data from the Numonyx Serial Flash Memory
*
* @param	SpiPtr is a pointer to the instance of the Spi device.
* @param	Addr is the starting address in the Flash Memory from which the
*		data is to be read.
* @param	ByteCount is the number of bytes to be read.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None
*
******************************************************************************/
int SpiFlashRead(XSpi *SpiPtr, u32 Addr, u32 ByteCount, u8 ReadCmd)
{
	int Status;

	/*
	 * Wait while the Flash is busy.
	 */
	Status = SpiFlashWaitForFlashReady(SpiPtr);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Prepare the spiWrBuf.
	 */
	spiWrBuf[BYTE1] = ReadCmd;
	spiWrBuf[BYTE2] = (u8) (Addr >> 16);
	spiWrBuf[BYTE3] = (u8) (Addr >> 8);
	spiWrBuf[BYTE4] = (u8) Addr;

	if (ReadCmd == COMMAND_DUAL_READ) {
		ByteCount += DUAL_READ_DUMMY_BYTES;
	} else if (ReadCmd == COMMAND_DUAL_IO_READ) {
		ByteCount += DUAL_READ_DUMMY_BYTES;
	} else if (ReadCmd == COMMAND_QUAD_IO_READ) {
		ByteCount += QUAD_IO_READ_DUMMY_BYTES;
	} else if (ReadCmd==COMMAND_QUAD_READ) {
		ByteCount += QUAD_READ_DUMMY_BYTES;
	}

	/*
	 * Initiate the Transfer.
	 */
	Status = XSpi_Transfer( SpiPtr, spiWrBuf, spiRdBuf,
				(ByteCount + READ_WRITE_EXTRA_BYTES));
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function enables writes to the Numonyx Serial Flash memory.
*
* @param	SpiPtr is a pointer to the instance of the Spi device.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None
*
******************************************************************************/
int SpiFlashWriteEnable(XSpi *SpiPtr)
{
	int Status;

	/*
	 * Wait while the Flash is busy.
	 */
	Status = SpiFlashWaitForFlashReady(SpiPtr);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Prepare the spiWrBuf.
	 */
	spiWrBuf[BYTE1] = COMMAND_WRITE_ENABLE;

	/*
	 * Initiate the Transfer.
	 */
	Status = XSpi_Transfer(SpiPtr, spiWrBuf, NULL,
				WRITE_ENABLE_BYTES);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function writes the data to the specified locations in the Numonyx Serial
* Flash memory.
*
* @param	SpiPtr is a pointer to the instance of the Spi device.
* @param	Addr is the address in the Buffer, where to write the data.
* @param	ByteCount is the number of bytes to be written.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None
*
******************************************************************************/
int SpiFlashWrite(XSpi *SpiPtr, u32 Addr, u32 ByteCount, u8 WriteCmd)
{
	//u32 Index;
	int Status;

	/*
	 * Wait while the Flash is busy.
	 */
	Status = SpiFlashWaitForFlashReady(SpiPtr);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Prepare the spiWrBuf.
	 */
	spiWrBuf[BYTE1] = WriteCmd;
	spiWrBuf[BYTE2] = (u8) (Addr >> 16);
	spiWrBuf[BYTE3] = (u8) (Addr >> 8);
	spiWrBuf[BYTE4] = (u8) Addr;


	/*
	 * Fill in the TEST data that is to be written into the Numonyx Serial
	 * Flash device.
	 */
	/*for(Index = 4; Index < ByteCount + READ_WRITE_EXTRA_BYTES; Index++) {
		spiWrBuf[Index] = (u8)((Index - 4));
	}*/

	/*
	 * Initiate the Transfer.
	 */
	Status = XSpi_Transfer(SpiPtr, spiWrBuf, NULL,
				(ByteCount + READ_WRITE_EXTRA_BYTES));
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function erases the contents of the specified Sector in the Numonyx
* Serial Flash device.
*
* @param	SpiPtr is a pointer to the instance of the Spi device.
* @param	Addr is the address within a sector of the Buffer, which is to
*		be erased.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		The erased bytes will be read back as 0xFF.
*
******************************************************************************/
int SpiFlashSectorErase(XSpi *SpiPtr, u32 Addr)
{
	int Status;

	/*
	 * Wait while the Flash is busy.
	 */
	Status = SpiFlashWaitForFlashReady(SpiPtr);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Prepare the spiWrBuf.
	 */
	spiWrBuf[BYTE1] = COMMAND_SECTOR_ERASE;
	spiWrBuf[BYTE2] = (u8) (Addr >> 16);
	spiWrBuf[BYTE3] = (u8) (Addr >> 8);
	spiWrBuf[BYTE4] = (u8) (Addr);

	/*
	 * Initiate the Transfer.
	 */
	Status = XSpi_Transfer(SpiPtr, spiWrBuf, NULL,
					SECTOR_ERASE_BYTES);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int SpiFlashPageErase(XSpi *SpiPtr, u32 Addr)
{
	int Status;

	/*
	 * Wait while the Flash is busy.
	 */
	Status = SpiFlashWaitForFlashReady(SpiPtr);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Prepare the spiWrBuf.
	 */
	spiWrBuf[BYTE1] = COMMAND_PAGE_ERASE;
	spiWrBuf[BYTE2] = (u8) (Addr >> 16);
	spiWrBuf[BYTE3] = (u8) (Addr >> 8);
	spiWrBuf[BYTE4] = 0;

	/*
	 * Initiate the Transfer.
	 */
	Status = XSpi_Transfer(SpiPtr, spiWrBuf, NULL,
					SECTOR_ERASE_BYTES);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function reads the data from the Numonyx Serial Flash Memory
*
* @param	SpiPtr is a pointer to the instance of the Spi device.
* @param	Addr is the starting address in the Flash Memory from which the
*		data is to be read.
* @param	ByteCount is the number of bytes to be read.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None
*
******************************************************************************/
int SpiFlashReadId(XSpi *SpiPtr)
{
	int Status;

	/*
	 * Wait while the Flash is busy.
	 */
	Status = SpiFlashWaitForFlashReady(SpiPtr);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Prepare the spiWrBuf.
	 */
	spiWrBuf[BYTE1] = 0x9E;

	/*
	 * Initiate the Transfer.
	 */
	Status = XSpi_Transfer( SpiPtr, spiWrBuf, spiRdBuf, 21);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

