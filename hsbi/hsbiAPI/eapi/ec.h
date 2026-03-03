#ifndef __EAPI_EC_IMPL__
#define __EAPI_EC_IMPL__
#include "EApi.h"
#include "I2CReset.h"

// MACRO : BUSID
#define BUSID_EC_I2C		EAPI_ID_I2C_EXTERNAL_2
#define BUSID_EC_SMBUS		EAPI_ID_I2C_EXTERNAL_3
#define BUSID_I2C_SMBUS		EAPI_ID_I2C_EXTERNAL

// Definition : Type enumerated
typedef enum {
	CMD_TYPE_NON,
	CMD_TYPE_STD,
	CMD_TYPE_EXT,
} I2C_Command_Length_Type_t;

// Definition : Type Struct
typedef struct{
    uint8_t     BusID;
    uint8_t     Addr;
    uint32_t    Command;
    uint8_t     CmdType;
    uint8_t    	Data[0x10];
    uint32_t    length;
} I2CPacket;

uint32_t I2C_Detect_device(I2CPacket* DetectDev);
uint32_t I2C_Read_Device(I2CPacket* I2C_Read_Packet);
uint32_t I2C_Write_Device(I2CPacket* I2C_Write_Packet);
uint32_t CommandEncoder(uint32_t InCMD, uint8_t CMDLenthType);

#endif