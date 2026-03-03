#include "ec.h"
#include <iostream>

uint32_t CommandEncoder(uint32_t InCMD, uint8_t CMDLenthType)
{
	uint32_t Command;

	if(CMDLenthType == CMD_TYPE_NON)
	{
		Command = EAPI_I2C_ENC_NO_CMD(InCMD);
	}
	else if (CMDLenthType == CMD_TYPE_STD)
	{
		Command = EAPI_I2C_ENC_STD_CMD(InCMD);
	}
	else if (CMDLenthType == CMD_TYPE_EXT)
	{
		Command = EAPI_I2C_ENC_EXT_CMD(InCMD);
	}
	return Command;
}

uint32_t I2C_Detect_device(I2CPacket* DetectDev)
{   
	uint32_t Addr = (uint32_t)(EAPI_I2C_ENC_7BIT_ADDR(DetectDev->Addr));
	return EApiI2CProbeDevice(DetectDev->BusID, Addr);
}

uint32_t I2C_Read_Device(I2CPacket* I2C_Read_Packet)
{
	//uint8_t i;
	uint32_t Addr = (uint32_t)(EAPI_I2C_ENC_7BIT_ADDR(I2C_Read_Packet->Addr));
	uint32_t Command = CommandEncoder(I2C_Read_Packet->Command, I2C_Read_Packet->CmdType);
	//printf("%s addr:0x%X cmd:0x%X data:0x%X len:0x%X\n", __FUNCTION__, Addr, Command, I2C_Read_Packet->Data[0], I2C_Read_Packet->length);
	return EApiI2CReadTransfer(I2C_Read_Packet->BusID, Addr, Command, I2C_Read_Packet->Data, I2C_Read_Packet->length, I2C_Read_Packet->length);	
}

uint32_t I2C_Write_Device(I2CPacket* I2C_Write_Packet)
{
	//uint8_t i;	
	uint32_t Addr = (uint32_t)(EAPI_I2C_ENC_7BIT_ADDR(I2C_Write_Packet->Addr));
	uint32_t Command = CommandEncoder(I2C_Write_Packet->Command, I2C_Write_Packet->CmdType);
	//printf("%s addr:0x%X cmd:0x%X data:0x%X len:0x%X\n", __FUNCTION__, Addr, Command, I2C_Write_Packet->Data[0], I2C_Write_Packet->length);
	return EApiI2CWriteTransfer(I2C_Write_Packet->BusID, Addr, Command, &I2C_Write_Packet->Data[0], I2C_Write_Packet->length);	
}

