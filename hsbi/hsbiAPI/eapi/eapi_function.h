#ifndef __EAPI_FUNCTION_IMPL__
#define __EAPI_FUNCTION_IMPL__

#include "EApi.h"

uint32_t eapi_detect(uint8_t BusID, uint8_t Addr);
uint32_t eapi_read(uint8_t BusID, uint8_t Addr, uint32_t Length, uint8_t* Data);
uint32_t eapi_read(uint8_t BusID, uint8_t Addr, uint8_t Command, int32_t Length, uint8_t* Data);
uint32_t eapi_read(uint8_t BusID, uint8_t Addr, uint8_t Command1, uint8_t Command2, int32_t Length, uint8_t* Data);
uint32_t eapi_write(uint8_t BusID, uint8_t Addr, uint32_t Length, uint8_t* Data);
uint32_t eapi_write(uint8_t BusID, uint8_t Addr, uint8_t Command, int32_t Length, uint8_t* Data);
uint32_t eapi_write(uint8_t BusID, uint8_t Addr, uint8_t Command1, uint8_t Command2, int32_t Length, uint8_t* Data);
uint32_t eapi_reset();
int test_eapi();
#endif