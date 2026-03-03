#include "ec.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

uint32_t eapi_detect(uint8_t BusID, uint8_t Addr)
{
    I2CPacket packet;
    packet.BusID = BusID,	
    packet.Addr = Addr;

	return I2C_Detect_device(&packet);
}

uint32_t eapi_read(uint8_t BusID, uint8_t Addr, uint32_t Length, uint8_t* Data)
{
	I2CPacket packet;
	packet.BusID = BusID;
	packet.Addr = Addr;
	packet.CmdType = I2C_Command_Length_Type_t::CMD_TYPE_NON;
	packet.Command = 0x0;
	packet.length = Length;
	int StatusCode = I2C_Read_Device(&packet);	
	memcpy(Data, packet.Data, Length);
	return StatusCode;
}

uint32_t eapi_read(uint8_t BusID, uint8_t Addr, uint8_t Command, int32_t Length, uint8_t* Data)
{
	I2CPacket packet;
	packet.BusID = BusID;
	packet.Addr = Addr;
	packet.CmdType = I2C_Command_Length_Type_t::CMD_TYPE_STD;
	packet.Command = Command;
	packet.length = Length;
	int StatusCode = I2C_Read_Device(&packet);		
	memcpy(Data, packet.Data, Length);
	return StatusCode;
}

uint32_t eapi_read(uint8_t BusID, uint8_t Addr, uint8_t Command1, uint8_t Command2, int32_t Length, uint8_t* Data)
{
	I2CPacket packet;
	packet.BusID = BusID;
	packet.Addr = Addr;
	packet.CmdType = I2C_Command_Length_Type_t::CMD_TYPE_EXT;
	packet.Command = (Command1 << 8) | (Command2 & 0xFF);
	packet.length = Length;
	int StatusCode =  I2C_Read_Device(&packet);	
	memcpy(Data, packet.Data, Length);
	return StatusCode;
}

uint32_t eapi_write(uint8_t BusID, uint8_t Addr, uint32_t Length, uint8_t* Data)
{
	I2CPacket packet;
	packet.BusID = BusID;
	packet.Addr = Addr;
	packet.CmdType = I2C_Command_Length_Type_t::CMD_TYPE_NON;
	packet.Command = 0x0;
	packet.length = Length;
	memcpy(packet.Data, Data, Length);
	return I2C_Write_Device(&packet);	
}

uint32_t eapi_write(uint8_t BusID, uint8_t Addr, uint8_t Command, int32_t Length, uint8_t* Data)
{
	I2CPacket packet;
	packet.BusID = BusID;
	packet.Addr = Addr;
	packet.CmdType = I2C_Command_Length_Type_t::CMD_TYPE_STD;
	packet.Command = Command;	
	packet.length = Length;
	memcpy(packet.Data, Data, Length);
	return I2C_Write_Device(&packet);	
}

uint32_t eapi_write(uint8_t BusID, uint8_t Addr, uint8_t Command1, uint8_t Command2, int32_t Length, uint8_t* Data)
{
	I2CPacket packet;
	packet.BusID = BusID;
	packet.Addr = Addr;
	packet.CmdType = I2C_Command_Length_Type_t::CMD_TYPE_EXT;
	packet.Command = (Command1 << 8) | (Command2 & 0xFF);
	packet.length = Length;
	memcpy(packet.Data, Data, Length);
	return I2C_Write_Device(&packet);	
}

uint32_t eapi_reset()
{
	//uint32_t StatusCode = 0;
	return Reset_I2c();
}

/* eapi function usage sample */
int test_eapi()
{
	// init eapi
//	uint32_t StatusCode = EApiLibInitialize();
//	if (EAPI_TEST_SUCCESS(StatusCode)) {
//		printf("eapi init ok\n");
//	} else {
//		printf("eapi init fail\n");
//	}

	uint8_t I2C_MUX = 0x70;
	uint8_t	Temp_0 = 0x48;
	//uint8_t	Temp_1 = 0x48;
	//uint8_t	Temp_2 = 0x48;
	//uint8_t	Temp_3 = 0x48;
	uint8_t cmdArr[16];

	usleep(1000 * 100);

	cmdArr[0] = 1;
	
	
//	if (EAPI_TEST_SUCCESS(eapi_write(BUSID_EC_I2C, I2C_MUX, 1, cmdArr)))
//	{
//		printf("i2c write ok %X\n", cmdArr[0]);
//	}
//	else
//		printf("i2c write error\n");
//	usleep(1000 * 20);
	
	if (EAPI_TEST_SUCCESS(eapi_read(BUSID_EC_I2C, Temp_0, 0, 2, cmdArr)))
	{
		printf("i2c read ok 0x%X 0x%X\n", cmdArr[0], cmdArr[1]);
	}
	else
		printf("i2c read error\n");
	usleep(1000 * 100);
//	printf("eapi_read 0x%x 0x%x\n", cmdArr[0], cmdArr[1]);
	

	/*

	if (EAPI_TEST_SUCCESS(eapi_detect(BUSID_EC_I2C, 0x11))) {
		printf("detect ok\n");
	} else {
		printf("detect fail\n");
	}
	
	uint8_t Dac[4];
	StatusCode = eapi_read(BUSID_EC_I2C, 0x12, 4, Dac);
	if (EAPI_TEST_SUCCESS(StatusCode)) {
		for (int n = 0; n < 4; n++) {
			printf("dac[%d] %d\n", n, Dac[n]);
		}
	} else {
		printf("read fail\n");
	}

	uint8_t CAL[10] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9 };
	StatusCode = eapi_write(BUSID_EC_I2C, 0x13, 10, CAL);
	if (EAPI_TEST_SUCCESS(StatusCode)) {
		printf("write ok\n");
	} else {
		printf("write fail\n");
	}

	uint8_t DAC[10] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9 };
	StatusCode = eapi_write(BUSID_EC_I2C, 0x13, 0x30, 10, CAL); //0x30
	if (EAPI_TEST_SUCCESS(StatusCode)) {
		printf("write ok\n");
	} else {
		printf("write fail\n");
	}
	*/
	
	return 0;
}
	
