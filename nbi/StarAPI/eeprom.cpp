#include "eeprom.h"
#include "eapi/ec.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "errc.h"
#include "define.h"

extern errc g_clErr;


#include "STAR_TEST.h"

void eeprom::init()
{
	unsigned int StatusCode = EApiLibInitialize();
	if (!EAPI_TEST_SUCCESS(StatusCode))
		printf("EAPI Init Fail!\n");
	else 
		printf("EAPI Init Success!\n");
		
//	sample();

	
	select_tempMux(0xf);
}

int eeprom::write_eeprom(uint8_t bus, uint8_t addr, uint16_t command, int32_t length, uint8_t* data)
{
	uint16_t buffersize = 0x10;
	uint32_t StatusCode = 0;
	uint32_t count = length / buffersize;
	uint32_t rest = length % buffersize;
	 
	int n = 0;
	while (count--) {
		StatusCode = eapi_write(bus, addr, (command & 0xFF00) >> 8, command & 0xFF, buffersize, data + n * buffersize);
		command += buffersize;
		n++;
	}
	if (rest) {
		StatusCode = eapi_write(bus, addr, (command & 0xFF00) >> 8, command & 0xFF, rest, data + n * buffersize);
	}
	return StatusCode;
}

//used 100ms duration after reading (in script)
int eeprom::read_eeprom(uint8_t bus, uint8_t addr, uint16_t command, int32_t length, uint8_t* data)
{
	uint16_t buffersize = 0x10;
	uint32_t StatusCode = 0;
	uint32_t count = length / buffersize;
	uint32_t rest = length % buffersize;
	int n = 0;
	while (count--) {
		StatusCode = eapi_read(bus, addr, (command & 0xFF00) >> 8, command & 0xFF, buffersize, data + n * buffersize);		
		command += buffersize;
		n++;
	}
	if (rest) {
		StatusCode = eapi_read(bus, addr, (command & 0xFF00) >> 8, command & 0xFF, rest, data + n * buffersize);
	}
	return StatusCode; 
}
#include <stdlib.h>
#define TEST_SIZE 1
void eeprom::sample()
{ 
	uint16_t adr = 0x200;
	uint8_t pBuf[TEST_SIZE];
	uint8_t pCheck[TEST_SIZE];
	int sz = TEST_SIZE;
	uint32_t ret;
	ret = read_eeprom(BUSID_EC_I2C, TCB_ROM, adr, TEST_SIZE, pBuf);
	if (!EAPI_TEST_SUCCESS(ret))printf("read fail\n");
	else {
		printf("read 0x%x : ", adr);
		for (int i = 0; i < sz; i++)
			printf("%02x ", pBuf[i]);
		printf("\n");
	}

	for (int ti = 0; ti < 2; ti++)
	{
				  
		for (int i = 0; i < TEST_SIZE; i++)
			pBuf[i] = i & 0xff;
		ret = write_eeprom(BUSID_EC_I2C, TCB_ROM, adr, TEST_SIZE, pBuf);
		if (!EAPI_TEST_SUCCESS(ret))printf("write fail\n");


		ret = read_eeprom(BUSID_EC_I2C, TCB_ROM, adr, TEST_SIZE, pCheck);
		if (!EAPI_TEST_SUCCESS(ret))printf("read fail\n");
		printf("adr %x :", adr);
		for (int i = 0; i < 16; i++)
			printf("%02x ", pCheck[i]);
		printf("\n");
		 
		bool bFail = false;
		for (int i = 0; i < TEST_SIZE; i++) {
			if (pBuf[i] != pCheck[i])
			{
				bFail = true;
				printf("ADR:%02x: W=%02x R=%02x\n", adr+i, pBuf[i], pCheck[i]);
			}
		}
		if (bFail)
			printf("EEPROM TEST %d FAIL!\n", ti);
		else
			printf("EEPROM TEST %d PASS\n", ti);

		adr += 0x100;

	}

/*
	uint8_t	Temp_0 = 0x48;
	uint8_t	Temp_1 = 0x48;
	uint8_t	Temp_2 = 0x48;
	uint8_t	Temp_3 = 0x48;
	uint8_t cmdArr[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

	cmdArr[0] = 1;
	if (!EAPI_TEST_SUCCESS(eapi_write(BUSID_EC_I2C, I2C_MUX, 1, cmdArr)))
		printf("i2c write error\n");
	usleep(1000 * 10);
	memset(cmdArr, 0, sizeof(cmdArr));
	printf("cmdarr %x %x\n", cmdArr[0], cmdArr[1]);
	if (!EAPI_TEST_SUCCESS(eapi_read(BUSID_EC_I2C, Temp_0, 0, 2, cmdArr)))
		printf("i2c read error\n");
	usleep(1000 * 100);
	printf("eapi_read 0x%x 0x%x ,0x %x %x\n", cmdArr[0], cmdArr[1], cmdArr[0], cmdArr[1]);
*/
}


bool eeprom::select_tempMux(uint8_t muxidx)
{	
	printf("====== Mux Select 0x%x =====\n", muxidx);
	//MEASURE_TIME(START);
	if (!EAPI_TEST_SUCCESS(eapi_write(BUSID_EC_I2C, I2C_MUX, 1, &muxidx))) {
		printf("Mux Select i2c write error\n");		
    //		MEASURE_TIME(END);
		return false;
	}
	//long long nano = MEASURE_TIME(END);
	//long long sec = nano / (1000 * 1000 * 1000);
	//if (sec > 1)
	//{
	//	printf("I2c Board Temp Mux Interface Error");
	//	return false;
	//}
	return true;

}


float eeprom::read_temp102(uint8_t ch, bool isDiag) // 0~3 
{
	uint8_t	tmp102 = 0x48 + ch;	
	uint8_t readCode[2] = {0, 0};
	uint16_t Code;
	int16_t Conv;
	
//	MEASURE_TIME(START);
//	printf("- Read IDX\n");
//	if (!EAPI_TEST_SUCCESS(eapi_read(BUSID_EC_I2C, tmp102, 1, &rdidx))) {
//		printf("Read IDX i2c read error\n");		
//		usleep(1000 * 100);
//	}
//	printf("Read IDX  0x%X\n", rdidx);
//	MEASURE_TIME(END);
	
	if (isDiag) {
		//MEASURE_TIME(START);
		printf("-- TEMP Read idx=%d\n", ch);
	}
	if (!EAPI_TEST_SUCCESS(eapi_read(BUSID_EC_I2C, tmp102, 0, 2, readCode)))
	{
		printf("Temp Read i2c read error\n");
		return -1;
	}
	//if (isDiag) {
	//	//long long nano = MEASURE_TIME(END);
	//	//long long sec = nano / (1000 * 1000 * 1000);
	//	if (sec > 1)
	//	{
	//		printf("I2c Board Temp Mux Interface Error");
	//		return -1;
	//	}
	//}

	Code = readCode[0];
	Code <<= 8;
	Code += readCode[1];

	memcpy(&Conv, &Code, 2);

//	printf("Read : 0x%X%X   Conv : 0x%X\n", readCode[0], readCode[1], Code);
	float tempValue = (Conv / 16) * 0.0625;
	printf("Test Board temp %d : %0.3f(0x%X)\n", ch, tempValue, Conv);

	return tempValue;
}
