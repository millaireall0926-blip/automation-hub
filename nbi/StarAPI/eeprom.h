#ifndef __EEPROM__
#define __EEPROM__

#include "eapi/eapi_function.h"

#pragma once

#define TCB_ROM  0x53
#define BIB_ROM  0x51
#define I2C_MUX  0x70
#define IO_EXP0  0x74
#define IO_EXP1  0x75

class eeprom
{
public:
	eeprom() { init(); };
	~eeprom() {}

	void init();
	void sample();
	int write_eeprom(uint8_t bus, uint8_t addr, uint16_t command, int32_t length, uint8_t* data);
	int read_eeprom(uint8_t bus, uint8_t addr, uint16_t command, int32_t length, uint8_t* data);
	bool select_tempMux(uint8_t muxidx);
	float read_temp102(uint8_t ch, bool isDiag = false);


};

#endif