#pragma once
#ifndef _LTC388N_BASE_LIB_
#define _LTC388N_BASE_LIB_

#include <map>
#include <list>
#include <math.h>
#include "ioc.h"
#include "String.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DPS - LTC3884 or LTC3886 or LTC3889
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define VOUT_OV_FAULT_LIMIT_FACTOR 1.1	 //+10%
#define VOUT_OV_WARN_LIMIT_FACTOR 1.075	 //+7.5%
#define VOUT_OV_MAX_FACTOR 1.075				 //+7.5%
#define VOUT_MARGIN_HIGH_FACTOR 1.05		 //+5%
#define VOUT_MARGIN_LOW_FACTOR 0.95			 //-5%
#define VOUT_UV_WARN_LIMIT_FACTOR 0.935	 //-6.5%
#define VOUT_UV_FAULT_LIMIT_FACTOR 0.930 //-7%

enum eLTC3886_Freq
{
	// Only these descrete frequencies are supported
	eEXT_FREQ = 0x0,
	e100K_FREQ = 0xEB20,
	e125K_FREQ = 0xFBE8,
	e150K_FREQ = 0xF258,
	e175K_FREQ = 0xF2BC,
	e200K_FREQ = 0xF320,
	e225K_FREQ = 0xF384,
	e250K_FREQ = 0xF3E8,
	e300K_FREQ = 0xFA58,
	e350K_FREQ = 0xFABC,
	e425K_FREQ = 0xFB52,
	e500K_FREQ = 0xFBE8,
	e575K_FREQ = 0x023F,
	e650K_FREQ = 0x028A,
	e750K_FREQ = 0x02EE
};

enum DPS_GROUP_SINGLE
{
	DPS_SINGLE_SEL = (0 << 7), // SINGLE CH
	DPS_GROUP_SEL = (1 << 7),	 // DPS.GROUP
};

enum LTC_PAGE
{
	LTC_PAGE0 = 0,
	LTC_PAGE1 = 1,
	LTC_PAGE_ALL = 0xFF,
};

enum LTC_PMBUS_OPPERATION
{
	OPR_IMMD_OFF = 0x0,
	OPR_SOFT_OFF = 0x40,
	OPR_ON = 0x80,
	OPR_MARGIN_LOW = 0x98,
	OPR_MARGIN_HIGH = 0xA8,
}; // CMD : 0x1

enum LTC_PMBUS_ON_OFF_CONFIG
{
	ON_OFF_TOFF_IGR = 0x16,
	ON_OFF_NONE_IGR = 0x17,
	ON_OFF_CMD_TOFF = 0x1E, // Default
	ON_OFF_CMD_IGR = 0x1F
}; // CMD : 0x2

enum LTC_PMBUS_COMMAND_TYPE
{
	TYPE_SENDBYTE = 0,
	TYPE_RW_BYTE,
	TYPE_RW_WORD,
	TYPE_RW_BLOCK,
	TYPE_R_BYTE,
	TYPE_R_WORD,
	TYPE_R_STRING,
	TYPE_R_BLOCK,
	TYPE_W_BLOCK,
};

enum LTC_PMBUS_DATA_FORMAT
{
	FORMAT_L11 = 0x0,
	FORMAT_L16,
	FORMAT_REG,
	FORMAT_I16,
	FORMAT_CF,
	FORMAT_ASC,
	FORMAT_NONE
};

enum LTC_VOUT_RANGE
{
	VOUT_HIGH_RANGE = 0,
	VOUT_LOW_RANGE = 2,
};

enum DPS_PMBUS_COMMAND
{
	I2C_CMD_PAGE = 0x00,
	I2C_CMD_OPERATION = 0x01,
	I2C_CMD_ON_OFF_CONFIG = 0x02,
	I2C_CMD_CLEAR_FAULTS = 0x03,
	I2C_CMD_PAGE_PLUS_WRITE = 0x05,
	I2C_CMD_PAGE_PLUS_READ = 0x06,
	I2C_CMD_WRITE_PROTECT = 0x10,
	I2C_CMD_STORE_USER_ALL = 0x15,
	I2C_CMD_RESTORE_USER_ALL = 0x16,
	I2C_CMD_CAPABILITY = 0x19,
	I2C_CMD_SMBALERT_MASK = 0x1B,

	I2C_CMD_VOUT_MODE = 0x20,
	I2C_CMD_VOUT_COMMAND = 0x21,
	I2C_CMD_VOUT_MAX = 0x24,
	I2C_CMD_VOUT_MARGIN_HIGH = 0x25,
	I2C_CMD_VOUT_MARGIN_LOW = 0x26,
	I2C_CMD_VOUT_TRANSITION_RATE = 0x27,

	I2C_CMD_FREQUENCY_SWITCH = 0x33,
	I2C_CMD_VIN_ON = 0x35,
	I2C_CMD_VIN_OFF = 0x36,
	I2C_CMD_IOUT_CAL_GAIN = 0x38,

	I2C_CMD_VOUT_OV_FAULT_LIMIT = 0x40,
	I2C_CMD_VOUT_OV_FAULT_RESPONSE = 0x41,
	I2C_CMD_VOUT_OV_WARN_LIMIT = 0x42,
	I2C_CMD_VOUT_UV_WARN_LIMIT = 0x43,
	I2C_CMD_VOUT_UV_FAULT_LIMIT = 0x44,
	I2C_CMD_VOUT_UV_FAULT_RESPONSE = 0x45,
	I2C_CMD_IOUT_OC_FAULT_LIMIT = 0x46,
	I2C_CMD_IOUT_OC_FAULT_RESPONSE = 0x47,
	I2C_CMD_IOUT_OC_WARN_LIMIT = 0x4A,
	I2C_CMD_OT_FAULT_LIMIT = 0x4F,

	I2C_CMD_OT_FAULT_RESPONSE = 0x50,
	I2C_CMD_OT_WARN_LIMIT = 0x51,
	I2C_CMD_UT_FAULT_LIMIT = 0x53,
	I2C_CMD_UT_FAULT_RESPONSE = 0x54,
	I2C_CMD_VIN_OV_FAULT_LIMIT = 0x55,
	I2C_CMD_VIN_OV_FAULT_RESPONSE = 0x56,
	I2C_CMD_VIN_UV_WARN_LIMIT = 0x58,
	I2C_CMD_IIN_OC_WARN_LIMIT = 0x5D,

	I2C_CMD_TON_DELAY = 0x60,
	I2C_CMD_TON_RISE = 0x61,
	I2C_CMD_TON_MAX_FAULT_LIMIT = 0x62,
	I2C_CMD_TON_MAX_FAULT_RESPONSE = 0x63,
	I2C_CMD_TOFF_DELAY = 0x64,
	I2C_CMD_TOFF_FALL = 0x65,
	I2C_CMD_TOFF_MAX_WARN_LIMIT = 0x66,

	I2C_CMD_STATUS_BYTE = 0x78,
	I2C_CMD_STATUS_WORD = 0x79,
	I2C_CMD_STATUS_VOUT = 0x7A,
	I2C_CMD_STATUS_IOUT = 0x7B,
	I2C_CMD_STATUS_INPUT = 0x7C,
	I2C_CMD_STATUS_TEMPERATURE = 0x7D,
	I2C_CMD_STATUS_CML = 0x7E,

	I2C_CMD_STATUS_MFR_SPECEFIC = 0x80,
	I2C_CMD_READ_VIN = 0x88,
	I2C_CMD_READ_IIN = 0x89,
	I2C_CMD_READ_VOUT = 0x8B,
	I2C_CMD_READ_IOUT = 0x8C,
	I2C_CMD_READ_TEMP_1 = 0x8D,
	I2C_CMD_READ_TEMP_2 = 0x8E,

	I2C_CMD_READ_FREQUENCY = 0x95,
	I2C_CMD_READ_POUT = 0x96,
	I2C_CMD_READ_PIN = 0x97,
	I2C_CMD_PMBUS_REV = 0x98,
	I2C_CMD_MFR_ID = 0x99,
	I2C_CMD_MFR_MODEL = 0x9A,

	I2C_CMD_MFR_VOUT_MAX = 0xA5,
	I2C_CMD_MFR_PIN_ACCURACY = 0xAC,

	I2C_CMD_USER_DATA_00 = 0xB0,
	I2C_CMD_USER_DATA_01 = 0xB1,
	I2C_CMD_USER_DATA_02 = 0xB2,
	I2C_CMD_USER_DATA_03 = 0xB3,
	I2C_CMD_USER_DATA_04 = 0xB4,
	I2C_CMD_MFR_INFO = 0xB6,

	I2C_CMD_MFR_CHAN_CONFIG = 0xD0,
	I2C_CMD_MFR_CONFIG_ALL = 0xD1,
	I2C_CMD_MFR_FAULT_PROPAGATE = 0xD2,
	I2C_CMD_MFR_PWM_COMP = 0xD3,
	I2C_CMD_MFR_PWM_MODE = 0xD4,
	I2C_CMD_MFR_FAULT_RESPONSE = 0xD5,
	I2C_CMD_MFR_OT_FAULT_RESPONSE = 0xD6,
	I2C_CMD_MFR_IOUT_PEAK = 0xD7,
	I2C_CMD_MFR_ADC_CONTROL = 0xD8,
	I2C_CMD_MFR_RETRY_DELAY = 0xDB,
	I2C_CMD_MFR_RESTART_DELAY = 0xDC,
	I2C_CMD_MFR_VOUT_PEAK = 0xDD,
	I2C_CMD_MFR_VIN_PEAK = 0xDE,
	I2C_CMD_MFR_TEMPERATUR_1_PEAK = 0xDF,

	I2C_CMD_MFR_READ_IIN_PEAK = 0xE1,
	I2C_CMD_MFR_CLEAR_PEAKS = 0xE3,
	I2C_CMD_MFR_READ_ICHIP = 0xE4,
	I2C_CMD_MFR_PADS = 0xE5,
	I2C_CMD_MFR_ADDRESS = 0xE6,
	I2C_CMD_MFR_SPECIAL_ID = 0xE7,
	I2C_CMD_MFR_IIN_CAL_GAIN = 0xE8,
	I2C_CMD_MFR_FAULT_LOG_STORE = 0xEA,
	I2C_CMD_MFR_FAULT_LOG_CLEAR = 0xEC,
	I2C_CMD_MFR_FAULT_LOG = 0xEE,
	I2C_CMD_MFR_COMMON = 0xEF,

	I2C_CMD_MFR_COMPARE_USER_ALL = 0xF0,
	I2C_CMD_MFR_TEMPERATUR_2_PEAK = 0xF4,
	I2C_CMD_MFR_PWM_CONFIG = 0xF5,
	I2C_CMD_MFR_IOUT_CAL_GAIN_TC = 0xF6,
	I2C_CMD_MFR_RVIN = 0xF7,
	I2C_CMD_MFR_TEMP_1_GAIN = 0xF8,
	I2C_CMD_MFR_TEMP_1_OFFSET = 0xF9,
	I2C_CMD_MFR_RAIL_ADDRESS = 0xFA,
	I2C_CMD_MFR_REAL_TIME = 0xFB,
	I2C_CMD_MFR_RESET = 0xFD,
};

typedef struct _stPMBusCommand
{
	cstring Command;
	LTC_PMBUS_DATA_FORMAT DataFormat;
	LTC_PMBUS_COMMAND_TYPE Type;
	unsigned int DefaultValue;
} stPMBusCommand;

typedef struct _stLTCModule
{
	int nType;	// 0 = 3884, 1 = 3886
	bool bPoly; // 0 = sparate, 1 = poly
	int nI2cNo;
	unsigned int nDevAdr;
	unsigned int nRailAdr0;
	unsigned int nRailAdr1;

} stLTCModule;

typedef union
{
	struct
	{
		unsigned char NONE_OF_THE_ABOVE : 1;
		unsigned char CML : 1;
		unsigned char TEMPERATURE : 1;
		unsigned char VIN_UV : 1;
		unsigned char IOUT_OC : 1;
		unsigned char VOUT_OV : 1;
		unsigned char OFF : 1;
		unsigned char BUSY : 1;
	} contents;
	unsigned char BYTE_STATUS;
} stStatusByte;

typedef union
{
	struct
	{
		unsigned char NONE_OF_THE_ABOVE : 1;
		unsigned char CML : 1;
		unsigned char TEMPERATURE : 1;
		unsigned char VIN_UV : 1;

		unsigned char IOUT_OC : 1;
		unsigned char VOUT_OV : 1;
		unsigned char OFF : 1;
		unsigned char BUSY : 1;

		unsigned char UNKNOWN : 1;
		unsigned char OTHER : 1;
		unsigned char FANS : 1;
		unsigned char POWER_GOOD : 1;

		unsigned char MFR_SPECIFIC : 1;
		unsigned char INPUT : 1;
		unsigned char IOUT_WARN : 1;
		unsigned char VOUT_WARN : 1;
	} contents;
	unsigned short WORD_STATUS;
} stStatusWord;

typedef union
{
	struct
	{
		unsigned char NOT_SUPPORTED : 1;
		unsigned char TOFF_MAX_FAULT : 1;
		unsigned char TON_MAX_FAULT : 1;
		unsigned char VOUT_MAX_WARN : 1;
		unsigned char VOUT_UV_FAULT : 1;
		unsigned char VOUT_UV_WARN : 1;
		unsigned char VOUT_OV_WARN : 1;
		unsigned char VOUT_OV_FAULT : 1;
	} contents;
	unsigned char VOUT_STATUS;
} stStatusVout;

typedef union
{
	struct
	{
		unsigned char NOT_SUPPORTED0 : 4;
		unsigned char UT_FAULT : 1;
		unsigned char NOT_SUPPORTED1 : 1;
		unsigned char OT_WARN : 1;
		unsigned char OT_FAULT : 1;
	} contents;
	unsigned char TEMP_STATUS;
} stStatusTemperature;

typedef union
{
	struct
	{
		unsigned char NOT_SUPPORTED0 : 5;
		unsigned char IOUT_OC_WARN : 1;
		unsigned char NOT_SUPPORTED1 : 1;
		unsigned char IOUT_OC_FAULT : 1;
	} contents;
	unsigned char IOUT_STATUS;
} stStatusIout;

typedef union
{
	struct
	{
		unsigned char NOT_SUPPORTED0 : 1;
		unsigned char IIN_OC_WARN : 1;
		unsigned char NOT_SUPPORTED1 : 1;
		unsigned char UOFF_FOR_INSUF : 1;
		unsigned char NOT_SUPPORTED2 : 1;
		unsigned char VIN_UV_WARN : 1;
		unsigned char NOT_SUPPORTED3 : 1;
		unsigned char VIN_OV_FAULT : 1;
	} contents;
	unsigned char INPUT_STATUS;
} stStatusInput;

typedef union
{
	struct
	{
		unsigned char FAULT_PULLED_LOW_BY_ERTERNAL_DEVICE : 1;
		unsigned char VOUT_SHORT_CYCLED : 1;
		unsigned char VDD33_UV_OR_OV_FAULT : 1;
		unsigned char FAULT_LOG_PRESENT : 1;
		unsigned char INTERNAL_PLL_UNLOCKED : 1;
		unsigned char EEPROM_CRC_ERROR : 1;
		unsigned char INTERNAL_TEMPERATURE_WARNING : 1;
		unsigned char INTERNAL_TEMPERATUR_FAULT : 1;
	} contents;
	unsigned char MFR_SPECFIC_STATUS;
} stStatusMfrSpecific;

typedef union
{
	struct
	{
		unsigned char OTHER_MEMORY_OR_LOGIC_FAULT : 1;
		unsigned char OTHER_COMMUNICATION_FAULT : 1;
		unsigned char RESERVED_LTC388N_RETURNS_0 : 1;
		unsigned char PROCESSOR_FAULT_DETECTED : 1;
		unsigned char MEMORY_FAULT_DETECTED : 1;
		unsigned char PACKET_ERROR_CHECK_FAILED : 1;
		unsigned char INVALID_OR_UNSUPPORTED_DATA_RECEIVED : 1;
		unsigned char INVALID_OR_UNSUPPORTED_CMD_RECEIVED : 1;
	} contents;
	unsigned char CML_STATUS;
} stStatusCml;

typedef union
{
	struct
	{
		unsigned char WP_PIN_HIGH : 1;
		unsigned char SHARE_CLK_LOW : 1;
		unsigned char READS_0 : 1;
		unsigned char EEPROM_INITIALIZED : 1;
		unsigned char OUTPUT_NOT_IN_TRANSITION : 1;
		unsigned char INTERNAL_CALCULATION_NOT_PENDING : 1;
		unsigned char CHIP_NOT_BUSY : 1;
		unsigned char CHIP_NOT_DRIVING_ALERT_LOW : 1;
	} contents;
	unsigned char MFR_COMMON;
} stStatusMfrCommon;

typedef union
{
	struct
	{
		unsigned char FAULT_PIN_STATE0 : 1;
		unsigned char FAULT_PIN_STATE1 : 1;
		unsigned char LTC_FORCING_FAULT0_LOW : 1;
		unsigned char LTC_FORCING_FAULT1_LOW : 1;
		unsigned char RUN0_PIN_STATE : 1;
		unsigned char RUN1_PIN_STATE : 1;
		unsigned char LTC_FORCING_RUN0_LOW : 1;
		unsigned char LTC_FORCING_RUN1_LOW : 1;
		unsigned char CHANNEL0_POWER_GOOD : 1;
		unsigned char CHANNEL1_POWER_GOOD : 1;
		unsigned char SYNC_CLOCKED_BY_EXTERN_SRC : 1;
		unsigned char INVALID_ADC_RESULT : 1;
		unsigned char READ0_0 : 1;
		unsigned char READ0_1 : 1;
		unsigned char VDD33_OV_FAULT : 1;
		unsigned char VDD33_UV_FAULT : 1;
	} contents;
	unsigned short MFR_PADS;
} stStatusMfrPads;

typedef struct
{
	float gain;
	float offset;
} stCalFactor;

class dps {

public:
	dps();
	~dps();

protected:
	uint DPS_RD;
	uint DPS_WR;
	uint DPS_BUSY;
	uint DPS_AUTO;
	uint DPS_EOS;
	uint DPS_ERR;
	uint DPS_IPNAME_ADDR;
	uint DPS_PARTNAME_ADDR;
	uint DPS_STAT_ADDR;
	uint DPS_TARGET_ADDR;
	uint DPS_OP_ADDR;
	uint DPS_I2C_BYTE_ADDR;
	uint DPS_I2C_CMD_ADDR;
	uint DPS_I2C_DATA_ADDR;	
	uint LTC388N_VOUT_MODE;

protected:
	std::map<DPS_PMBUS_COMMAND, stPMBusCommand> m_mapCmdLtc;
	std::list<uint> m_listGroups;

public:
	uint m_uMaxLtcGroup;
	uint m_uMaxLtc;
	uint m_uMaxLtcPage;
	uint m_uMfrPwmMode;
	float m_fLtcVoutMax;
	float m_fLtcVoutMin;
	float m_fLtcVin;
	float m_fLtcVinOn;
	float m_fLtcIoutMax;
	float m_fLtcVoutRange;
	cstring m_sLtcPart;

	bool m_nDebug; // 0 = nomarl, 1 = show reg, 2 = simmode

	void printe(const char* str, ...);

public:
	bool GetDpsCommandInfo(DPS_PMBUS_COMMAND, int &, int &, int &, string &);
	void MakePmBusCommands();
	void MakeCalFactor();

public:
	float GetLtcCommandL(DPS_PMBUS_COMMAND, int);				 // L11, L16
	uint GetLtcCommandR(DPS_PMBUS_COMMAND, int);				 // REG, I16
	bool GetLtcCommandA(DPS_PMBUS_COMMAND, int, char *); // REG, I16	
	bool SetLtcCommandL(DPS_PMBUS_COMMAND, int, float);				// L11, L16	
	bool SetLtcCommandR(DPS_PMBUS_COMMAND, int, uint);				// REG, I16

public:	
	bool I2cWaitEos();	
	bool I2cWrite(int, int, int, LTC_PMBUS_COMMAND_TYPE);	
	uint I2cRead(int, int, LTC_PMBUS_COMMAND_TYPE);
	bool I2cReadA(int, int, uint, char *);
	bool isValidGrp(uint);
	bool isValidPage(uint);
	bool isValidVout(float);
	float L11ToFloat(int);
	float L16ToFloat(uint16_t input_val);
	uint16_t FloatToL11(float);
	uint16_t FloatToL16(float);
	uint16_t FloatToL16Mode(uint8_t vout_mode, float input_val);

	uint I2cSendByte(int, int);
	uint I2cSendByteAll(int);

public:
	bool StatusCheck(uint, uint, cstring &); // Default Status Word.
	bool StatusByte(uint, uint, cstring &);
	bool StatusInput(uint, uint, cstring &);
	bool StatusMfrSpecific(uint, uint, cstring &);
	bool StatusVout(uint, uint, cstring &);
	bool StatusIout(uint, uint, cstring &);
	bool StatusTemperature(uint, uint, cstring &);
	bool StatusCml(uint, uint, cstring &);
	bool StatusMfrCommon(uint, uint, cstring &);
	bool StatusMfrPads(uint, uint, cstring &);

	bool ltcDumpCh(uint, uint);
	uint DumpCommand(uint, uint, DPS_PMBUS_COMMAND);

public:
	bool SetLtcPage(uint, uint);
	bool SetLtcPageAll(uint);
	bool SetLtcOperation(uint, LTC_PMBUS_OPPERATION);
	bool SetLtcOnOffConfig(uint, LTC_PMBUS_ON_OFF_CONFIG);
	bool SetLtcClearFaults(uint, uint d = 0x0);
	bool SetLtcMfrClearPeaks(uint, uint d = 0x0);
	bool SetLtcMfrReset(uint, uint d = 0x0);
	bool SetLtcStoreUserAll(uint, uint d = 0x0);
	bool SetLtcRestoreUserAll(uint, uint d = 0x0);

	bool setLtcMFRFaultPropagate(uint, uint);

	bool SetLtcFreqSwitch(uint, uint);

	bool SetLtcVoutCommand(uint, float);
	bool SetLtcVoutMax(uint, float);
	bool SetLtcVoutMarginHigh(uint, float);
	bool SetLtcVoutMarginLow(uint, float);
	bool SetLtcVoutTransRate(uint, float);
	bool SetLtcVoutOvFaultLimit(uint, float);
	bool SetLtcVoutOvFaultResponse(uint, uint);
	bool SetLtcVoutOvWarnLimit(uint, float);
	bool SetLtcVoutUvWarnLimit(uint, float);
	bool SetLtcVoutUvFaultLimit(uint, float);
	bool SetLtcVoutUvFaultResponse(uint, uint);

	bool SetLtcVinOn(uint, float);
	bool SetLtcVinOff(uint, float); // khz
	bool SetLtcVinOvFaultLimit(uint, float);
	bool SetLtcVinOvFaultResponse(uint, uint);
	bool SetLtcVinUvWarnLimit(uint, float);

	bool SetLtcIinOcWarnLimit(uint, float);

	bool SetLtcIOutCalGain(uint, float); // mohm
	bool SetLtcIoutOcFaultLimit(uint, float);
	bool SetLtcIoutOcFaultResponse(uint, uint);
	bool SetLtcIoutOcWarnLimit(uint, float);

	bool SetLtcOtFaultLimit(uint, float);
	bool SetLtcOtFaultResponse(uint, uint);
	bool SetLtcOtWarnLimit(uint, float);

	bool SetLtcUtFaultLimit(uint, float);
	bool SetLtcUtFaultResponse(uint, uint);

	bool SetLtcTonDelay(uint, float);					// ms
	bool SetLtcTonRise(uint, float);					// ms
	bool SetLtcTonMaxFaultLimit(uint, float); // ms
	bool SetLtcToffDelay(uint, float);				// ms
	bool SetLtcToffFall(uint, float);					// ms
	bool SetLtcToffMaxWarnLimit(uint, float); // ms

	bool setMfrIinCalGain(uint, float);

	bool SetLtcMfrRVin(uint, float);		// mohm
	bool SetLtcMfrPwmComp(uint, uint);	// REG
	bool SetLtcMfrPwmMode(uint, uint);	// REG
	bool SetLtcRailAddress(uint, uint); // REG
	bool SetLtcPolyMode(uint, uint);

	bool SetLtcUserData0(uint, word);
	bool SetLtcUserData2(uint, word);
	bool SetLtcUserData4(uint, word);

	uint getLtcMfrPwmMode(uint); // REG
	uint GetDpsVoutMode(DPS_GROUP_SINGLE, uint);
	uint GetLtcStatusByte(DPS_GROUP_SINGLE, uint);
	uint GetLtcStatusWord(DPS_GROUP_SINGLE, uint);
	uint GetLtcStatusVout(DPS_GROUP_SINGLE, uint);
	uint GetLtcStatusIout(DPS_GROUP_SINGLE, uint);
	uint GetLtcStatusInput(DPS_GROUP_SINGLE, uint);
	uint GetLtcStatusTemperature(DPS_GROUP_SINGLE, uint);
	uint GetLtcStatusCml(DPS_GROUP_SINGLE, uint);
	uint GetLtcStatusMfrSpecific(DPS_GROUP_SINGLE, uint);
	uint GetLtcMfrCommon(DPS_GROUP_SINGLE, uint);	
	uint GetLtcMfrPads(uint ch);

	uint GetLtcUserData0(uint);
	uint GetLtcUserData2(uint);
	uint GetLtcUserData4(uint);

	float GetMfrVoutMax(DPS_GROUP_SINGLE, uint);
	float ReadVin(DPS_GROUP_SINGLE, uint);
	float ReadIin(DPS_GROUP_SINGLE, uint);
	float ReadVout(DPS_GROUP_SINGLE, uint, uint sampleing = 1);
	float ReadIout(DPS_GROUP_SINGLE, uint, uint sampleing = 1);
	float ReadTemperature_1(DPS_GROUP_SINGLE, uint);
	float ReadTemperature_2(DPS_GROUP_SINGLE, uint);
	float ReadMfrTemperature_1_Peak(DPS_GROUP_SINGLE, uint);
	float ReadMfrTemperature_2_Peak(DPS_GROUP_SINGLE, uint);
	float ReadFrequency(DPS_GROUP_SINGLE, uint);
	float ReadPout(uint);
	float ReadPin(uint);
	uint ReadPmbusRev(DPS_GROUP_SINGLE, uint);
	bool ReadMfrId(DPS_GROUP_SINGLE, uint, char *);	;
	float ReadMfrVoutPeak(DPS_GROUP_SINGLE, uint);
	float ReadMfrIoutPeak(DPS_GROUP_SINGLE, uint);
	float ReadMfrVinPeak(DPS_GROUP_SINGLE, uint);
	float ReadMfrIinPeak(DPS_GROUP_SINGLE, uint);
	float ReadMfrReadIChip(DPS_GROUP_SINGLE, uint);
	float ReadMfrIinCalGain(DPS_GROUP_SINGLE, uint);
	uint ReadMfrAddress(DPS_GROUP_SINGLE, uint);
	uint ReadMfrSpecialId(DPS_GROUP_SINGLE, uint);

public:
	bool AllChannelInit();
	bool SetVout(int ch, float fVolt, float under, float over, float limit, bool bCalApply = true);
	bool ReadChVout(int ch, float &Volt, int cnt = 1, bool bCalApply = true);
	float ReadChIout(int ch, int cnt = 1, bool bCalApply = true);
	uint ReadStatus(int ch);
	uint ReadAlertCtrl();		
	
	bool ReadAllreg(int nDps);

};

#endif
