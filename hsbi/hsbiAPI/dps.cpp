#include "define.h"
#include "dps.h"
#include "operation.h"
//#include "DIAG_TEST.h"

extern operation	gOPR;
extern ioc  gIO;

dps::dps()
{
	MakePmBusCommands();

	// default
	m_uMaxLtcGroup = 5;
	m_uMaxLtc = 20;
	m_uMaxLtcPage = 2;
	m_uMfrPwmMode = 0xC1;
	m_fLtcVoutMax = 5.5;
	m_fLtcVoutMin = 0.5;
	m_fLtcVin = 38.0;
	m_fLtcVoutRange = 0.0;

	LTC388N_VOUT_MODE = (0x14);

	m_nDebug = 0;
	m_bHideDPSErr = false;

	m_VOutVal.assign(22, 0.0);
	m_UVFVal.assign(22, 0.0);
	m_OVFVal.assign(22, 5.0);

	MakeCalFactor();
}

dps::~dps()
{
}

void dps::printe(const char* str, ...) {
	if (m_bHideDPSErr)return;
	va_list	ap;
	char buf[2048];
	va_start(ap, str);
	vsprintf(buf, str, ap);
	va_end(ap);


	printf("%s\n", buf);

}

void dps::MakePmBusCommands()
{
	m_mapCmdLtc.clear();
	m_mapCmdLtc[I2C_CMD_PAGE] = stPMBusCommand{"I2C_CMD_PAGE                    ", FORMAT_REG, TYPE_RW_BYTE, 0x0};
	m_mapCmdLtc[I2C_CMD_OPERATION] = stPMBusCommand{"I2C_CMD_OPERATION               ", FORMAT_REG, TYPE_RW_BYTE, 0x40};
	m_mapCmdLtc[I2C_CMD_ON_OFF_CONFIG] = stPMBusCommand{"I2C_CMD_ON_OFF_CONFIG           ", FORMAT_REG, TYPE_RW_BYTE, 0x1E};
	m_mapCmdLtc[I2C_CMD_CLEAR_FAULTS] = stPMBusCommand{"I2C_CMD_CLEAR_FAULTS            ", FORMAT_NONE, TYPE_SENDBYTE, 0x0};
	m_mapCmdLtc[I2C_CMD_PAGE_PLUS_WRITE] = stPMBusCommand{"I2C_CMD_PAGE_PLUS_WRITE         ", FORMAT_NONE, TYPE_W_BLOCK, 0x0};
	m_mapCmdLtc[I2C_CMD_PAGE_PLUS_READ] = stPMBusCommand{"I2C_CMD_PAGE_PLUS_READ          ", FORMAT_NONE, TYPE_RW_BLOCK, 0x0};
	m_mapCmdLtc[I2C_CMD_WRITE_PROTECT] = stPMBusCommand{"I2C_CMD_WRITE_PROTECT           ", FORMAT_REG, TYPE_RW_BYTE, 0x0};
	m_mapCmdLtc[I2C_CMD_STORE_USER_ALL] = stPMBusCommand{"I2C_CMD_STORE_USER_ALL          ", FORMAT_NONE, TYPE_SENDBYTE, 0x0};
	m_mapCmdLtc[I2C_CMD_RESTORE_USER_ALL] = stPMBusCommand{"I2C_CMD_RESTORE_USER_ALL        ", FORMAT_NONE, TYPE_SENDBYTE, 0x0};
	m_mapCmdLtc[I2C_CMD_CAPABILITY] = stPMBusCommand{"I2C_CMD_CAPABILITY              ", FORMAT_REG, TYPE_R_BYTE, 0x0};
	//m_mapCmdLtc[I2C_CMD_SMBALERT_MASK] = stPMBusCommand{"I2C_CMD_SMBALERT_MASK           ", FORMAT_REG, TYPE_RW_BLOCK, 0x0};
	m_mapCmdLtc[I2C_CMD_SMBALERT_MASK] = stPMBusCommand{"I2C_CMD_SMBALERT_MASK           ", FORMAT_REG, TYPE_RW_WORD, 0x0};

	m_mapCmdLtc[I2C_CMD_VOUT_MODE] = stPMBusCommand{"I2C_CMD_VOUT_MODE               ", FORMAT_REG, TYPE_R_BYTE, 0x14};
	m_mapCmdLtc[I2C_CMD_VOUT_COMMAND] = stPMBusCommand{"I2C_CMD_VOUT_COMMAND            ", FORMAT_L16, TYPE_RW_WORD, 0x1000};
	m_mapCmdLtc[I2C_CMD_VOUT_MAX] = stPMBusCommand{"I2C_CMD_VOUT_MAX                ", FORMAT_L16, TYPE_RW_WORD, 0xE000};
	m_mapCmdLtc[I2C_CMD_VOUT_MARGIN_HIGH] = stPMBusCommand{"I2C_CMD_VOUT_MARGIN_HIGH        ", FORMAT_L16, TYPE_RW_WORD, 0x10CD};
	m_mapCmdLtc[I2C_CMD_VOUT_MARGIN_LOW] = stPMBusCommand{"I2C_CMD_VOUT_MARGIN_LOW         ", FORMAT_L16, TYPE_RW_WORD, 0x0F33};
	m_mapCmdLtc[I2C_CMD_VOUT_TRANSITION_RATE] = stPMBusCommand{"I2C_CMD_VOUT_TRANSITION_RATE    ", FORMAT_L11, TYPE_RW_WORD, 0xAA00};

	m_mapCmdLtc[I2C_CMD_FREQUENCY_SWITCH] = stPMBusCommand{"I2C_CMD_FREQUENCY_SWITCH         ", FORMAT_REG, TYPE_RW_WORD, 0xFB52};
	m_mapCmdLtc[I2C_CMD_VIN_ON] = stPMBusCommand{"I2C_CMD_VIN_ON                   ", FORMAT_L11, TYPE_RW_WORD, 0xCB40};
	m_mapCmdLtc[I2C_CMD_VIN_OFF] = stPMBusCommand{"I2C_CMD_VIN_OFF                  ", FORMAT_L11, TYPE_RW_WORD, 0xCB00};
	m_mapCmdLtc[I2C_CMD_IOUT_CAL_GAIN] = stPMBusCommand{"I2C_CMD_IOUT_CAL_GAIN            ", FORMAT_L11, TYPE_RW_WORD, 0xBB9A};

	m_mapCmdLtc[I2C_CMD_VOUT_OV_FAULT_LIMIT] = stPMBusCommand{"I2C_CMD_VOUT_OV_FAULT_LIMIT     ", FORMAT_L16, TYPE_RW_WORD, 0x119A};
	m_mapCmdLtc[I2C_CMD_VOUT_OV_FAULT_RESPONSE] = stPMBusCommand{"I2C_CMD_VOUT_OV_FAULT_RESPONSE  ", FORMAT_REG, TYPE_RW_BYTE, 0x00B8};
	m_mapCmdLtc[I2C_CMD_VOUT_OV_WARN_LIMIT] = stPMBusCommand{"I2C_CMD_VOUT_OV_WARN_LIMIT      ", FORMAT_L16, TYPE_RW_WORD, 0x1133};
	m_mapCmdLtc[I2C_CMD_VOUT_UV_WARN_LIMIT] = stPMBusCommand{"I2C_CMD_VOUT_UV_WARN_LIMIT      ", FORMAT_L16, TYPE_RW_WORD, 0x0ECD};
	m_mapCmdLtc[I2C_CMD_VOUT_UV_FAULT_LIMIT] = stPMBusCommand{"I2C_CMD_VOUT_UV_FAULT_LIMIT     ", FORMAT_L16, TYPE_RW_WORD, 0x0E66};
	m_mapCmdLtc[I2C_CMD_VOUT_UV_FAULT_RESPONSE] = stPMBusCommand{"I2C_CMD_VOUT_UV_FAULT_RESPONSE  ", FORMAT_REG, TYPE_RW_BYTE, 0x00B8};
	m_mapCmdLtc[I2C_CMD_IOUT_OC_FAULT_LIMIT] = stPMBusCommand{"I2C_CMD_IOUT_OC_FAULT_LIMIT     ", FORMAT_L11, TYPE_RW_WORD, 0xDBB8};
	m_mapCmdLtc[I2C_CMD_IOUT_OC_FAULT_RESPONSE] = stPMBusCommand{"I2C_CMD_IOUT_OC_FAULT_RESPONSE  ", FORMAT_REG, TYPE_RW_BYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_IOUT_OC_WARN_LIMIT] = stPMBusCommand{"I2C_CMD_IOUT_OC_WARN_LIMIT      ", FORMAT_L11, TYPE_RW_WORD, 0xDA80};
	m_mapCmdLtc[I2C_CMD_OT_FAULT_LIMIT] = stPMBusCommand{"I2C_CMD_OT_FAULT_LIMIT          ", FORMAT_L11, TYPE_RW_WORD, 0xEB20};

	m_mapCmdLtc[I2C_CMD_OT_FAULT_RESPONSE] = stPMBusCommand{"I2C_CMD_OT_FAULT_RESPONSE       ", FORMAT_REG, TYPE_RW_BYTE, 0x00B8};
	m_mapCmdLtc[I2C_CMD_OT_WARN_LIMIT] = stPMBusCommand{"I2C_CMD_OT_WARN_LIMIT           ", FORMAT_L11, TYPE_RW_WORD, 0xEAA8};
	m_mapCmdLtc[I2C_CMD_UT_FAULT_LIMIT] = stPMBusCommand{"I2C_CMD_UT_FAULT_LIMIT          ", FORMAT_L11, TYPE_RW_WORD, 0xE580};
	m_mapCmdLtc[I2C_CMD_UT_FAULT_RESPONSE] = stPMBusCommand{"I2C_CMD_UT_FAULT_RESPONSE       ", FORMAT_REG, TYPE_RW_BYTE, 0x00B8};
	m_mapCmdLtc[I2C_CMD_VIN_OV_FAULT_LIMIT] = stPMBusCommand{"I2C_CMD_VIN_OV_FAULT_LIMIT      ", FORMAT_L11, TYPE_RW_WORD, 0xE300};
	m_mapCmdLtc[I2C_CMD_VIN_OV_FAULT_RESPONSE] = stPMBusCommand{"I2C_CMD_VIN_OV_FAULT_RESPONSE   ", FORMAT_REG, TYPE_RW_BYTE, 0x0080};
	m_mapCmdLtc[I2C_CMD_VIN_UV_WARN_LIMIT] = stPMBusCommand{"I2C_CMD_VIN_UV_WARN_LIMIT       ", FORMAT_L11, TYPE_RW_WORD, 0xE580};
	m_mapCmdLtc[I2C_CMD_IIN_OC_WARN_LIMIT] = stPMBusCommand{"I2C_CMD_IIN_OC_WARN_LIMIT       ", FORMAT_L11, TYPE_RW_WORD, 0xE580};

	m_mapCmdLtc[I2C_CMD_TON_DELAY] = stPMBusCommand{"I2C_CMD_TON_DELAY               ", FORMAT_L11, TYPE_RW_WORD, 0x8000};
	m_mapCmdLtc[I2C_CMD_TON_RISE] = stPMBusCommand{"I2C_CMD_TON_RISE                ", FORMAT_L11, TYPE_RW_WORD, 0xD200};
	m_mapCmdLtc[I2C_CMD_TON_MAX_FAULT_LIMIT] = stPMBusCommand{"I2C_CMD_TON_MAX_FAULT_LIMIT     ", FORMAT_L11, TYPE_RW_WORD, 0xD280};
	m_mapCmdLtc[I2C_CMD_TON_MAX_FAULT_RESPONSE] = stPMBusCommand{"I2C_CMD_TON_MAX_FAULT_RESPONSE  ", FORMAT_REG, TYPE_RW_BYTE, 0x00B8};
	m_mapCmdLtc[I2C_CMD_TOFF_DELAY] = stPMBusCommand{"I2C_CMD_TOFF_DELAY              ", FORMAT_L11, TYPE_RW_WORD, 0x8000};
	m_mapCmdLtc[I2C_CMD_TOFF_FALL] = stPMBusCommand{"I2C_CMD_TOFF_FALL               ", FORMAT_L11, TYPE_RW_WORD, 0xD200};
	m_mapCmdLtc[I2C_CMD_TOFF_MAX_WARN_LIMIT] = stPMBusCommand{"I2C_CMD_TOFF_MAX_WARN_LIMIT     ", FORMAT_L11, TYPE_RW_WORD, 0xF258};
	m_mapCmdLtc[I2C_CMD_STATUS_BYTE] = stPMBusCommand{"I2C_CMD_STATUS_BYTE             ", FORMAT_REG, TYPE_RW_BYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_STATUS_WORD] = stPMBusCommand{"I2C_CMD_STATUS_WORD             ", FORMAT_REG, TYPE_RW_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_STATUS_VOUT] = stPMBusCommand{"I2C_CMD_STATUS_VOUT             ", FORMAT_REG, TYPE_RW_BYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_STATUS_IOUT] = stPMBusCommand{"I2C_CMD_STATUS_IOUT             ", FORMAT_REG, TYPE_RW_BYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_STATUS_INPUT] = stPMBusCommand{"I2C_CMD_STATUS_INPUT            ", FORMAT_REG, TYPE_RW_BYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_STATUS_TEMPERATURE] = stPMBusCommand{"I2C_CMD_STATUS_TEMPERATURE      ", FORMAT_REG, TYPE_RW_BYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_STATUS_CML] = stPMBusCommand{"I2C_CMD_STATUS_CML              ", FORMAT_REG, TYPE_RW_BYTE, 0x0000};

	m_mapCmdLtc[I2C_CMD_STATUS_MFR_SPECEFIC] = stPMBusCommand{"I2C_CMD_STATUS_MFR_SPECEFIC     ", FORMAT_REG, TYPE_RW_BYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_READ_VIN] = stPMBusCommand{"I2C_CMD_READ_VIN                ", FORMAT_L11, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_READ_IIN] = stPMBusCommand{"I2C_CMD_READ_IIN                ", FORMAT_L11, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_READ_VOUT] = stPMBusCommand{"I2C_CMD_READ_VOUT               ", FORMAT_L16, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_READ_IOUT] = stPMBusCommand{"I2C_CMD_READ_IOUT               ", FORMAT_L11, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_READ_TEMP_1] = stPMBusCommand{"I2C_CMD_READ_TEMP_1             ", FORMAT_L11, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_READ_TEMP_2] = stPMBusCommand{"I2C_CMD_READ_TEMP_2             ", FORMAT_L11, TYPE_R_WORD, 0x0000};

	m_mapCmdLtc[I2C_CMD_READ_FREQUENCY] = stPMBusCommand{"I2C_CMD_READ_FREQUENCY          ", FORMAT_L11, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_READ_POUT] = stPMBusCommand{"I2C_CMD_READ_POUT               ", FORMAT_L11, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_READ_PIN] = stPMBusCommand{"I2C_CMD_READ_PIN                ", FORMAT_L11, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_PMBUS_REV] = stPMBusCommand{"I2C_CMD_PMBUS_REV               ", FORMAT_REG, TYPE_R_BYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_ID] = stPMBusCommand{"I2C_CMD_MFR_ID                  ", FORMAT_ASC, TYPE_R_STRING, 0x0003};		// LTC
	m_mapCmdLtc[I2C_CMD_MFR_MODEL] = stPMBusCommand{"I2C_CMD_MFR_MODEL               ", FORMAT_ASC, TYPE_R_STRING, 0x0008}; // LTC3886

	m_mapCmdLtc[I2C_CMD_MFR_VOUT_MAX] = stPMBusCommand{"I2C_CMD_MFR_VOUT_MAX            ", FORMAT_L16, TYPE_R_WORD, 0xE000};
	m_mapCmdLtc[I2C_CMD_MFR_PIN_ACCURACY] = stPMBusCommand{"I2C_CMD_MFR_PIN_ACCURACY        ", FORMAT_NONE, TYPE_R_BYTE, 0xE000};

	m_mapCmdLtc[I2C_CMD_USER_DATA_00] = stPMBusCommand{"I2C_CMD_USER_DATA_00            ", FORMAT_REG, TYPE_RW_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_USER_DATA_01] = stPMBusCommand{"I2C_CMD_USER_DATA_01            ", FORMAT_REG, TYPE_RW_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_USER_DATA_02] = stPMBusCommand{"I2C_CMD_USER_DATA_02            ", FORMAT_REG, TYPE_RW_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_USER_DATA_03] = stPMBusCommand{"I2C_CMD_USER_DATA_03            ", FORMAT_REG, TYPE_RW_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_USER_DATA_04] = stPMBusCommand{"I2C_CMD_USER_DATA_04            ", FORMAT_REG, TYPE_RW_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_INFO] = stPMBusCommand{"I2C_CMD_MFR_INFO                ", FORMAT_REG, TYPE_R_WORD, 0x0000};

	m_mapCmdLtc[I2C_CMD_MFR_CHAN_CONFIG] = stPMBusCommand{"I2C_CMD_MFR_CHAN_CONFIG         ", FORMAT_REG, TYPE_RW_BYTE, 0x001D};
	m_mapCmdLtc[I2C_CMD_MFR_CONFIG_ALL] = stPMBusCommand{"I2C_CMD_MFR_CONFIG_ALL          ", FORMAT_REG, TYPE_RW_BYTE, 0x0021};
	m_mapCmdLtc[I2C_CMD_MFR_FAULT_PROPAGATE] = stPMBusCommand{"I2C_CMD_MFR_FAULT_PROPAGATE     ", FORMAT_REG, TYPE_RW_WORD, 0x6993};
	m_mapCmdLtc[I2C_CMD_MFR_PWM_COMP] = stPMBusCommand{"I2C_CMD_MFR_PWM_COMP            ", FORMAT_REG, TYPE_RW_BYTE, 0x00AE};
	m_mapCmdLtc[I2C_CMD_MFR_PWM_MODE] = stPMBusCommand{"I2C_CMD_MFR_PWM_MODE            ", FORMAT_REG, TYPE_RW_BYTE, 0x00C1};
	m_mapCmdLtc[I2C_CMD_MFR_FAULT_RESPONSE] = stPMBusCommand{"I2C_CMD_MFR_FAULT_RESPONSE      ", FORMAT_REG, TYPE_RW_BYTE, 0x00C0};
	m_mapCmdLtc[I2C_CMD_MFR_OT_FAULT_RESPONSE] = stPMBusCommand{"I2C_CMD_MFR_OT_FAULT_RESPONSE   ", FORMAT_REG, TYPE_R_BYTE, 0x00C0};
	m_mapCmdLtc[I2C_CMD_MFR_IOUT_PEAK] = stPMBusCommand{"I2C_CMD_MFR_IOUT_PEAK           ", FORMAT_L11, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_ADC_CONTROL] = stPMBusCommand{"I2C_CMD_MFR_ADC_CONTROL         ", FORMAT_REG, TYPE_RW_BYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_RETRY_DELAY] = stPMBusCommand{"I2C_CMD_MFR_RETRY_DELAY         ", FORMAT_L11, TYPE_RW_WORD, 0xFABC};
	m_mapCmdLtc[I2C_CMD_MFR_RESTART_DELAY] = stPMBusCommand{"I2C_CMD_MFR_RESTART_DELAY       ", FORMAT_L11, TYPE_RW_WORD, 0xFBE8};
	m_mapCmdLtc[I2C_CMD_MFR_VOUT_PEAK] = stPMBusCommand{"I2C_CMD_MFR_VOUT_PEAK           ", FORMAT_L16, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_VIN_PEAK] = stPMBusCommand{"I2C_CMD_MFR_VIN_PEAK            ", FORMAT_L11, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_TEMPERATUR_1_PEAK] = stPMBusCommand{"I2C_CMD_MFR_TEMPERATUR_1_PEAK   ", FORMAT_L11, TYPE_R_WORD, 0x0000};

	m_mapCmdLtc[I2C_CMD_MFR_READ_IIN_PEAK] = stPMBusCommand{"I2C_CMD_MFR_READ_IIN_PEAK       ", FORMAT_L11, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_CLEAR_PEAKS] = stPMBusCommand{"I2C_CMD_MFR_CLEAR_PEAKS         ", FORMAT_NONE, TYPE_SENDBYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_READ_ICHIP] = stPMBusCommand{"I2C_CMD_MFR_READ_ICHIP          ", FORMAT_L11, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_PADS] = stPMBusCommand{"I2C_CMD_MFR_PADS                ", FORMAT_REG, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_ADDRESS] = stPMBusCommand{"I2C_CMD_MFR_ADDRESS             ", FORMAT_REG, TYPE_RW_BYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_SPECIAL_ID] = stPMBusCommand{"I2C_CMD_MFR_SPECIAL_ID          ", FORMAT_REG, TYPE_R_WORD, 0x04C0};
	m_mapCmdLtc[I2C_CMD_MFR_IIN_CAL_GAIN] = stPMBusCommand{"I2C_CMD_MFR_IIN_CAL_GAIN        ", FORMAT_L11, TYPE_RW_WORD, 0xCA80};
	m_mapCmdLtc[I2C_CMD_MFR_FAULT_LOG_STORE] = stPMBusCommand{"I2C_CMD_MFR_FAULT_LOG_STORE     ", FORMAT_NONE, TYPE_SENDBYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_FAULT_LOG_CLEAR] = stPMBusCommand{"I2C_CMD_MFR_FAULT_LOG_CLEAR     ", FORMAT_NONE, TYPE_SENDBYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_FAULT_LOG] = stPMBusCommand{"I2C_CMD_MFR_FAULT_LOG           ", FORMAT_REG, TYPE_R_BLOCK, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_COMMON] = stPMBusCommand{"I2C_CMD_MFR_COMMON              ", FORMAT_REG, TYPE_R_BYTE, 0x0000};

	m_mapCmdLtc[I2C_CMD_MFR_COMPARE_USER_ALL] = stPMBusCommand{"I2C_CMD_MFR_COMPARE_USER_ALL    ", FORMAT_NONE, TYPE_SENDBYTE, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_TEMPERATUR_2_PEAK] = stPMBusCommand{"I2C_CMD_MFR_TEMPERATUR_2_PEAK   ", FORMAT_L11, TYPE_R_WORD, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_PWM_CONFIG] = stPMBusCommand{"I2C_CMD_MFR_PWM_CONFIG          ", FORMAT_REG, TYPE_RW_BYTE, 0x0010};
	m_mapCmdLtc[I2C_CMD_MFR_IOUT_CAL_GAIN_TC] = stPMBusCommand{"I2C_CMD_MFR_IOUT_CAL_GAIN_TC    ", FORMAT_CF, TYPE_RW_WORD, 0x0F3C};
	m_mapCmdLtc[I2C_CMD_MFR_RVIN] = stPMBusCommand{"I2C_CMD_MFR_RVIN                ", FORMAT_L11, TYPE_RW_WORD, 0x03E8};
	m_mapCmdLtc[I2C_CMD_MFR_TEMP_1_GAIN] = stPMBusCommand{"I2C_CMD_MFR_TEMP_1_GAIN         ", FORMAT_CF, TYPE_RW_WORD, 0x4000};
	m_mapCmdLtc[I2C_CMD_MFR_TEMP_1_OFFSET] = stPMBusCommand{"I2C_CMD_MFR_TEMP_1_OFFSET       ", FORMAT_L11, TYPE_RW_WORD, 0x8000};
	m_mapCmdLtc[I2C_CMD_MFR_RAIL_ADDRESS] = stPMBusCommand{"I2C_CMD_MFR_RAIL_ADDRESS        ", FORMAT_REG, TYPE_RW_BYTE, 0x0080};
	m_mapCmdLtc[I2C_CMD_MFR_REAL_TIME] = stPMBusCommand{"I2C_CMD_MFR_REAL_TIME           ", FORMAT_CF, TYPE_R_BLOCK, 0x0000};
	m_mapCmdLtc[I2C_CMD_MFR_RESET] = stPMBusCommand{"I2C_CMD_MFR_RESET               ", FORMAT_NONE, TYPE_SENDBYTE, 0x0000};
}

void dps::MakeCalFactor() {
	stCalFactor calData;
	calData.gain = 1.0;
	calData.offset = 0.0;
	calData.offset2 = 0.0;

	for (int idx = 0; idx < MAX_DPS_CH; idx++) {
		m_VSCal.push_back(calData);
		m_VMCal.push_back(calData);
		m_IMCal.push_back(calData);
	}
}

bool dps::GetDpsCommandInfo(DPS_PMBUS_COMMAND command, int &format, int &type, int &defaullt, string &sCmd)
{
	std::map<DPS_PMBUS_COMMAND, stPMBusCommand>::iterator it = m_mapCmdLtc.find(command);
	if (it == m_mapCmdLtc.end())
	{
		printe("%s : error DPS I2C COMMAND 0x%X\n", __FUNCTION__, command);
		return false;
	}
	format = (int)it->second.DataFormat;
	type = (int)it->second.Type;
	defaullt = (int)it->second.DefaultValue;
	sCmd = it->second.Command;
	return true;
}

float dps::L11ToFloat(int input_val)
{
	// Extract exponent as MS 5 bits
	int8_t exponent = (int8_t)(input_val >> 11);
	// Extract mantissa as LS 11 bits
	int16_t mantissa = input_val & 0x7ff;
	// Sign extend exponent from 5 to 8 bits
	if (exponent > 0x0F)
		exponent |= 0xE0;
	// Sign extend mantissa from 11 to 16 bits
	if (mantissa > 0x03FF)
		mantissa |= 0xF800;
	// Compute value as mantissa * 2^(exponent)
	return mantissa * pow(2.0, exponent);
}

uint16_t dps::FloatToL11(float input_val)
{
	unsigned short int uExponent;
	unsigned short int uMantissa;

	// Set exponent to -16
	int exponent = -16;

	// Extract mantissa from input value
	int mantissa = (int)(input_val / pow(2.0, exponent));
	// Search for an exponent that produces
	// a valid 11-bit mantissa
	do
	{
		if ((mantissa >= -1024) && (mantissa <= +1023))
		{
			break; // stop if mantissa valid
		}
		exponent++;
		mantissa = (int)(input_val / pow(2.0, exponent));
	} while (exponent < +15);

	// Format the exponent of the L11
	uExponent = exponent << 11; // Format the mantissa of the L11
	uMantissa = mantissa & 0x07FF;

	// Compute value as exponent | mantissa
	return uExponent | uMantissa;
}

float dps::L16ToFloat(uint16_t input_val)
{
	float mantissa = 0.0;
	float value = 0.0;

	// Assume Linear 16, pull out 5 bits of exponent, and use signed value.
	int8_t exponent = (int8_t)(LTC388N_VOUT_MODE & 0x1F);
	// Sign extend exponent from 5 to 8 bits
	if (exponent > 0x0F)
		exponent |= 0xE0;
	// Convert mantissa to a float so we can do math.
	mantissa = (float)input_val;
	value = mantissa * pow(2.0, exponent);
	return (float)value;
}

uint16_t dps::FloatToL16(float fVal)
{
	// int32_t rc = -1;
	uint8_t vout_mode = 0x0;
	// Get the mode from the device.
	// rc = ReadRegByte(channel, address, VOUT_MODE, &nByte);
	// if(rc) TRETURN(rc);
	vout_mode = (LTC388N_VOUT_MODE & 0x1F);
	uint16_t nVal = FloatToL16Mode(vout_mode, fVal);
	return nVal;
}

uint16_t dps::FloatToL16Mode(uint8_t vout_mode, float input_val)
{
	// Assume Linear 16, pull out 5 bits of exponent, and use signed value.
	int8_t exponent = vout_mode & 0x1F;
	// Sign extend exponent from 5 to 8 bits
	if (exponent > 0x0F)
		exponent |= 0xE0;
	// Scale the value to a mantissa based on the exponent
	uint16_t mantissa = (uint16_t)(input_val / pow(2.0, exponent));
	return mantissa;
}

bool dps::SetGangMode(int idx, bool bOn)
{
	// Gang Mode is not exist
	/*
	uint status = g_clmio.readLB8(DPS_I2C_GANG_ADDR, 1) & 0xff;
	uint val = 1 << idx;
	if (bOn)
		status |= val;
	else {
		val = ~val;
		status &= val;
	}
	g_clmio.writeLB8(DPS_I2C_GANG_ADDR, status, 1);
	*/
	return true;
}

uint dps::I2cSendByte(PCI_SEL pci, int i2c_num, int command)
{
	//printf("Fault clear Start (i2cNo : %d)\n", i2c_num);
	gIO.dps_send_byte(pci, i2c_num, command);
	printf("I2cSendByte (pci : %d, i2cNo : %d, cmd : %d)\n", pci, i2c_num, command);
	usleep(1000);
	//if (!I2cWaitEos())
	//{
	//	printf("%s(0x%08X)-i2c%02d : error DPS I2C EOS Check!!\n", __FUNCTION__, command, i2c_num);
	//	return false;
	//}
	return 0;
}

uint dps::I2cSendByteAll(int command)
{
	//gIO.dps_send_byte(PCI2, 4, command);
	for(int nPCI = DBG_PCI_NU; nPCI < MAX_PCI; nPCI++) {
		for(int nDev = DPS_START_NU; nDev < MAX_DPS_DEVICE; nDev++) {
			gIO.dps_send_byte(PCI_SEL(nPCI), nDev, command);
		}
	}
	printf("All dps Send Byte Done \n");
	usleep(1000);

	return 0;
}

bool dps::I2cWaitEos(PCI_SEL pci)
{
	// !!check!!
	if (gIO.m_bSim)
		return true;
	byte ret = gIO.dps_wait_eos(pci);
	return (ret == RTN_OK);
}

// bool dps::I2cWrite( DPS_GROUP_SINGLE sel, int i2c_num, int command, int data, LTC_PMBUS_COMMAND_TYPE type )
bool dps::I2cWrite(PCI_SEL pci, int i2c_num, int command, int data, LTC_PMBUS_COMMAND_TYPE type)
{
	if (m_nDebug > 0)
	{
		printf("%s i2c(%d)\tcmd:%x\t%x\n", __FUNCTION__, i2c_num, command, data);
	}

	int iLen = 0x0;
	uint mask = 0xff;
	if (type == TYPE_RW_WORD)
	{
		mask = 0xFFFF;
		iLen = 0x2;
	}
	else if (type == TYPE_RW_BYTE)
	{
		mask = 0x00FF;
		iLen = 0x1;
	}
	else
	{
		mask = 0x00FF;
		iLen = 0x0;
	} // SEND BYTE & ETC

	data = (data & mask);

	uint8_t rv = gIO.dps_write(pci, i2c_num, command, iLen, (byte *)(&data));
	
	if(rv != RTN_OK) {
	//	printf("%s(0x%08X)-i2c%02d : error DPS I2C EOS Check!!\n", __FUNCTION__, command, i2c_num);
		return false;
	}

	//if (!I2cWaitEos(pci))
	//{
	//	printf("%s(0x%08X)-i2c%02d : error DPS I2C EOS Check!!\n", __FUNCTION__, command, i2c_num);
	//	return false;
	//}
	return true;
}

bool dps::I2cWriteGlobal(PCI_SEL pci, int i2c_num, int command, int data, LTC_PMBUS_COMMAND_TYPE type)
{
	if (m_nDebug > 0)
	{
		printf("%s i2c(%d)\tcmd:%x\t%x\n", __FUNCTION__, i2c_num, command, data);
	}

	int iLen = 0x0;
	uint mask = 0xff;
	if (type == TYPE_RW_WORD)
	{
		mask = 0xFFFF;
		iLen = 0x2;
	}
	else if (type == TYPE_RW_BYTE)
	{
		mask = 0x00FF;
		iLen = 0x1;
	}
	else
	{
		mask = 0x00FF;
		iLen = 0x0;
	} // SEND BYTE & ETC

	data = (data & mask);

	gIO.dps_write_global(pci, i2c_num, command, iLen, (byte *)(&data));

	if (!I2cWaitEos(pci))
	{
		printe("%s(0x%08X)-i2c%02d : error DPS I2C EOS Check!!\n", __FUNCTION__, command, i2c_num);
		return false;
	}
	return true;
}

uint dps::I2cRead(PCI_SEL pci, int i2c_num, int command, LTC_PMBUS_COMMAND_TYPE type)
{
	// printf("%s sim%d\n", __FUNCTION__, m_bSim);

	uint uMask = 0xff;
	uint uData = 0x0;
	uint uLen = 0x0;

	if (type == TYPE_RW_WORD)
	{
		uMask = 0xFFFF;
		uLen = 0x2;
	}
	else if (type == TYPE_R_WORD)
	{
		uMask = 0xFFFF;
		uLen = 0x2;
	}
	else if (type == TYPE_RW_BYTE)
	{
		uMask = 0x00FF;
		uLen = 0x1;
	}
	else if (type == TYPE_R_BYTE)
	{
		uMask = 0x00FF;
		uLen = 0x1;
	}
	else
	{
		uMask = 0x00FF;
		uLen = 0x1;
	} // SEND_BYTE & ETC

	gIO.dps_read(pci, i2c_num, command, uLen, (byte *)(&uData));

	if (m_nDebug == 1)
	{
		printf("%s i2c(%d)\tcmd:%x\t%x\n", __FUNCTION__, i2c_num, command, uData);
	}

	return uData & uMask;
}

bool dps::I2cReadA(PCI_SEL pci, int i2c_num, int command, uint uLen, char *pData)
{

	byte ret = gIO.dps_read(pci, i2c_num, command, uLen, (byte *)(&pData));

	return (ret == RTN_OK);
}

// Float to L11/L16
// bool dps::SetLtcCommandL( DPS_PMBUS_COMMAND command, DPS_GROUP_SINGLE sel, int i2c_num, float fValue)
bool dps::SetLtcCommandL(PCI_SEL pci, DPS_PMBUS_COMMAND command, int i2c_num, float fValue)
{
	cstring sCmd = "";
	uint16_t uData = 0x0;
	int format, type, value;
	if (!GetDpsCommandInfo(command, format, type, value, sCmd))
	{
		printe("%s : error I2C Command:0x%X!!\n", __FUNCTION__, (int)command);
		return false;
	}
	switch (format)
	{
	case FORMAT_L11:
		uData = FloatToL11(fValue);
		// printe("%s : L11 [%5.3f][0x%04X]\n", sCmd.c_str(), fValue, uData );
		break;
	case FORMAT_L16:
		uData = FloatToL16(fValue);
		// printe("%s : L16 [%5.3f][0x%04X]\n", sCmd.c_str(), fValue, uData );
		break;
	default:
		printf("%s : error invalid i2c data format expect L11 or L16\n", __FUNCTION__);
		return false;
		break;
	}
	// return I2cWrite( sel, i2c_num, command, uData, (LTC_PMBUS_COMMAND_TYPE)type );
	return I2cWrite(pci, i2c_num, command, uData, (LTC_PMBUS_COMMAND_TYPE)type);
}

bool dps::SetLtcCommandLGlobal(PCI_SEL pci, DPS_PMBUS_COMMAND command, int i2c_num, float fValue)
{
	cstring sCmd = "";
	uint16_t uData = 0x0;
	int format, type, value;
	if (!GetDpsCommandInfo(command, format, type, value, sCmd))
	{
		printe("%s : error I2C Command:0x%X!!\n", __FUNCTION__, (int)command);
		return false;
	}
	switch (format)
	{
	case FORMAT_L11:
		uData = FloatToL11(fValue);
		// printe("%s : L11 [%5.3f][0x%04X]\n", sCmd.c_str(), fValue, uData );
		break;
	case FORMAT_L16:
		uData = FloatToL16(fValue);
		// printe("%s : L16 [%5.3f][0x%04X]\n", sCmd.c_str(), fValue, uData );
		break;
	default:
		printf("%s : error invalid i2c data format expect L11 or L16\n", __FUNCTION__);
		return false;
		break;
	}
	// return I2cWrite( sel, i2c_num, command, uData, (LTC_PMBUS_COMMAND_TYPE)type );
	return I2cWriteGlobal(pci, i2c_num, command, uData, (LTC_PMBUS_COMMAND_TYPE)type);
}

// REG
// bool dps::SetLtcCommandR( DPS_PMBUS_COMMAND command, DPS_GROUP_SINGLE sel, int i2c_num, uint uValue)
bool dps::SetLtcCommandR(PCI_SEL pci, DPS_PMBUS_COMMAND command, int i2c_num, uint uValue)
{
	cstring sCmd = "";
	uint16_t uData = 0x0;
	int format, type, value;
	if (!GetDpsCommandInfo(command, format, type, value, sCmd))
	{
		printe("%s : error I2C Command:0x%X!!\n", __FUNCTION__, (int)command);
		return false;
	}
	switch (format)
	{
	case FORMAT_REG:
	case FORMAT_I16:
	case FORMAT_NONE:
		uData = uValue;
		// printe("%s : R [0x%04X]\n", sCmd.c_str(), uData );
		break;
	default:
		printe("%s : error invalid i2c data format expect REG or I16\n", sCmd.c_str());
		return false;
		break;
	}
	// return I2cWrite( sel, i2c_num, command, uData, (LTC_PMBUS_COMMAND_TYPE)type );
	return I2cWrite(pci, i2c_num, command, uData, (LTC_PMBUS_COMMAND_TYPE)type);
}

float dps::GetLtcCommandL(PCI_SEL pci, DPS_PMBUS_COMMAND command, int i2c_num) // i2c command, ch
{
	cstring sCmd = "";
	uint16_t uData = 0x0;
	float fData = 0.0;
	int format, type, value;
	if (!GetDpsCommandInfo(command, format, type, value, sCmd))
	{
		printe("%s : error I2C Command:0x%X!!\n", __FUNCTION__, (int)command);
		return -1;
	}
	uData = I2cRead(pci, i2c_num, (uint)command, (LTC_PMBUS_COMMAND_TYPE)type);
	switch (format)
	{
	case FORMAT_L11:
		fData = L11ToFloat(uData);
		break;
	case FORMAT_L16:
		fData = L16ToFloat(uData);
		break;
	default:
		printf("%s : error invalid i2c data format expect L11 or L16\n", sCmd.c_str());
		return 0.0;
		break;
	}
	return fData;
}

uint dps::GetLtcCommandR(PCI_SEL pci, DPS_PMBUS_COMMAND command, int i2c_num) // i2c command, ch
{
	cstring sCmd = "";
	uint16_t uData = 0x0;
	int format, type, value;
	if (!GetDpsCommandInfo(command, format, type, value, sCmd))
	{
		printe("%s : error I2C Command:0x%X!!\n", __FUNCTION__, (int)command);
		return -1;
	}
	// printf("%s\n", __FUNCTION__ );
	uData = I2cRead(pci, i2c_num, (uint)command, (LTC_PMBUS_COMMAND_TYPE)type);
	return uData;
}

bool dps::GetLtcCommandA(PCI_SEL pci, DPS_PMBUS_COMMAND command, int i2c_num, char *pDest) // ASC
{
	cstring sCmd = "";
	int format, type, len;
	if (!GetDpsCommandInfo(command, format, type, len, sCmd))
	{
		printe("%s : error I2C Command:0x%X!!\n", __FUNCTION__, (int)command);
		return false;
	}
	return I2cReadA(pci, i2c_num, (uint)command, len, pDest);
}

bool dps::isValidGrp(uint i2c_grp)
{
	if (i2c_grp < 0 || i2c_grp > m_uMaxLtcGroup)
	{
		printe("%s : error Dps I2C_GROUP Set Error Expect 0 or 1, but %d\n", __FUNCTION__, i2c_grp);
		return false;
	}
	return true;
}

bool dps::isValidPage(uint page)
{
	if (page == LTC_PAGE_ALL)
		return true;
	if (page < 0 || page > 1)
	{
		printe("%s : error Dps I2C_PAGE Set Error Expect 0 or 1, but %d\n", __FUNCTION__, page);
		return false;
	}
	return true;
}

bool dps::isValidVout(float fVolt)
{
	if (fVolt < m_fLtcVoutMin || fVolt > m_fLtcVoutMax)
	{
		printe("%s : error Dps VOUT force value between [ %f ~ %f], but %f set\n", __FUNCTION__, m_fLtcVoutMin, m_fLtcVoutMax, fVolt);
		return false;
	}
	return true;
}

// VOUT MODE READ
uint dps::GetDpsVoutMode(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_VOUT_MODE, i2c_num);
}

bool dps::SetLtcPageAll(uint page)
{
	for(int npci = DBG_PCI_NU ; npci < MAX_PCI ; npci++) {
		for(int nDev = DPS_START_NU; nDev < MAX_DPS_DEVICE; nDev++) {
			SetLtcPage(PCI_SEL(npci), nDev, page);
		}
	}
}

// PAGE0 or PAGE1 or ALL
bool dps::SetLtcPage(PCI_SEL pci, uint i2c_num, uint page)
{
	if (!isValidPage(page))
	{
		printe("%s : error LTC%02d I2C Page use 0x0/0x1/0xFF, but %d\n", __FUNCTION__, i2c_num, page);
		return false;
	}
	return SetLtcCommandR(pci, I2C_CMD_PAGE, i2c_num, page);
}

// ON, OFF, MARGIN HIGH/LOW
bool dps::SetLtcOperation(PCI_SEL pci, uint i2c_num, LTC_PMBUS_OPPERATION opr)
{
	return SetLtcCommandR(pci, I2C_CMD_OPERATION, i2c_num, (uint)opr);
}

// ON_OFF_CONFIG
bool dps::SetLtcOnOffConfig(PCI_SEL pci, uint i2c_num, LTC_PMBUS_ON_OFF_CONFIG cfg)
{
	return SetLtcCommandR(pci, I2C_CMD_ON_OFF_CONFIG, i2c_num, (uint)cfg);
}

// clear Faults
bool dps::SetLtcClearFaults(PCI_SEL pci, uint i2c_num, uint uData)
{
	return SetLtcCommandR(pci, I2C_CMD_CLEAR_FAULTS, i2c_num, uData);
}

// clear Faults
bool dps::SetLtcMfrClearPeaks(PCI_SEL pci, uint i2c_num, uint uData)
{
	return SetLtcCommandR(pci, I2C_CMD_MFR_CLEAR_PEAKS, i2c_num, uData);
}

// RESET
bool dps::SetLtcMfrReset(PCI_SEL pci, uint i2c_num, uint uData)
{
	bool bRtn = false;
	bRtn = SetLtcCommandR(pci, I2C_CMD_MFR_RESET, i2c_num, uData);
	usleep(1);
	return bRtn;
}

bool dps::SetLtcStoreUserAll(PCI_SEL pci, uint i2c_num, uint uData)
{
	bool bRtn = false;
	bRtn = SetLtcCommandR(pci, I2C_CMD_STORE_USER_ALL, i2c_num, uData);
	usleep(1);
	return bRtn;
}

bool dps::SetLtcRestoreUserAll(PCI_SEL pci, uint i2c_num, uint uData)
{
	bool bRtn = false;
	bRtn = SetLtcCommandR(pci, I2C_CMD_RESTORE_USER_ALL, i2c_num, uData);
	usleep(1);
	return bRtn;
}

bool dps::setLtcMFRFaultPropagate(PCI_SEL pci, uint i2c_num, uint uData)
{
	bool bRtn = false;
	bRtn = SetLtcCommandR(pci, I2C_CMD_MFR_FAULT_PROPAGATE, i2c_num, uData);
	usleep(1);
	return bRtn;
}

// Force Voltage Set
bool dps::SetLtcVoutCommand(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VOUT_COMMAND, i2c_num, fVolt);
}

// Upper limit Vout command output voltage
bool dps::SetLtcVoutMax(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VOUT_MAX, i2c_num, fVolt);
}

// Upper limit Vout command output voltage
bool dps::SetLtcVoutMarginHigh(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VOUT_MARGIN_HIGH, i2c_num, fVolt);
}

// Upper limit Vout command output voltage
bool dps::SetLtcVoutMarginLow(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VOUT_MARGIN_LOW, i2c_num, fVolt);
}

bool dps::SetLtcVoutTransRate(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VOUT_TRANSITION_RATE, i2c_num, fVolt);
}

// Freq KHZ
bool dps::SetLtcFreqSwitch(PCI_SEL pci, uint i2c_num, uint freq)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandR(pci, I2C_CMD_FREQUENCY_SWITCH, i2c_num, freq);
}

bool dps::SetLtcVinOn(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VIN_ON, i2c_num, fVolt);
}

bool dps::SetLtcVinOff(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VIN_OFF, i2c_num, fVolt);
}

bool dps::SetLtcIOutCalGain(PCI_SEL pci, uint i2c_num, float fmOhm) // mili ohm
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_IOUT_CAL_GAIN, i2c_num, fmOhm);
}

bool dps::SetLtcVoutOvFaultLimit(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VOUT_OV_FAULT_LIMIT, i2c_num, fVolt);
}

bool dps::SetLtcVoutOvFaultResponse(PCI_SEL pci, uint i2c_num, uint uRes)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandR(pci, I2C_CMD_VOUT_OV_FAULT_RESPONSE, i2c_num, uRes & 0xFFFF);
}

bool dps::SetLtcVoutOvWarnLimit(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VOUT_OV_WARN_LIMIT, i2c_num, fVolt);
}

bool dps::SetLtcVoutUvWarnLimit(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VOUT_UV_WARN_LIMIT, i2c_num, fVolt);
}

bool dps::SetLtcVoutUvFaultLimit(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VOUT_UV_FAULT_LIMIT, i2c_num, fVolt);
}

bool dps::SetLtcVoutUvFaultResponse(PCI_SEL pci, uint i2c_num, uint uRes)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandR(pci, I2C_CMD_VOUT_UV_FAULT_RESPONSE, i2c_num, uRes & 0xFFFF);
}

bool dps::SetLtcIoutOcFaultLimit(PCI_SEL pci, uint i2c_num, float fCurr)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_IOUT_OC_FAULT_LIMIT, i2c_num, fCurr);
}

bool dps::SetLtcIoutOcFaultResponse(PCI_SEL pci, uint i2c_num, uint uRes)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandR(pci, I2C_CMD_IOUT_OC_FAULT_RESPONSE, i2c_num, uRes & 0xFFFF);
}

bool dps::SetLtcIoutOcWarnLimit(PCI_SEL pci, uint i2c_num, float fCurr)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_IOUT_OC_WARN_LIMIT, i2c_num, fCurr);
}

bool dps::SetLtcOtFaultLimit(PCI_SEL pci, uint i2c_num, float fTemp)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_OT_FAULT_LIMIT, i2c_num, fTemp);
}

bool dps::SetLtcOtFaultResponse(PCI_SEL pci, uint i2c_num, uint uRes)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_OT_FAULT_RESPONSE, i2c_num, uRes);
}

bool dps::SetLtcOtWarnLimit(PCI_SEL pci, uint i2c_num, float fTemp)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_OT_WARN_LIMIT, i2c_num, fTemp);
}

bool dps::SetLtcUtFaultLimit(PCI_SEL pci, uint i2c_num, float fTemp)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_UT_FAULT_LIMIT, i2c_num, fTemp);
}

bool dps::SetLtcUtFaultResponse(PCI_SEL pci, uint i2c_num, uint uRes)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_UT_FAULT_RESPONSE, i2c_num, uRes);
}

bool dps::SetLtcVinOvFaultLimit(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VIN_OV_FAULT_LIMIT, i2c_num, fVolt);
}

bool dps::SetLtcVinOvFaultResponse(PCI_SEL pci, uint i2c_num, uint uRes)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandR(pci, I2C_CMD_VIN_OV_FAULT_RESPONSE, i2c_num, uRes & 0xFFFF);
}

bool dps::SetLtcVinUvWarnLimit(PCI_SEL pci, uint i2c_num, float fVolt)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_VIN_UV_WARN_LIMIT, i2c_num, fVolt);
}

bool dps::SetLtcIinOcWarnLimit(PCI_SEL pci, uint i2c_num, float fCurr)
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_IIN_OC_WARN_LIMIT, i2c_num, fCurr);
}

bool dps::SetLtcTonDelay(PCI_SEL pci, uint i2c_num, float fTime) // ms
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_TON_DELAY, i2c_num, fTime);
}

bool dps::SetLtcTonRise(PCI_SEL pci, uint i2c_num, float fTime) // ms
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_TON_RISE, i2c_num, fTime);
}

bool dps::SetLtcTonMaxFaultLimit(PCI_SEL pci, uint i2c_num, float fTime) // ms
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_TON_MAX_FAULT_LIMIT, i2c_num, fTime);
}

bool dps::SetLtcToffDelay(PCI_SEL pci, uint i2c_num, float fTime) // ms
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_TOFF_DELAY, i2c_num, fTime);
}

bool dps::SetLtcToffFall(PCI_SEL pci, uint i2c_num, float fTime) // ms
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_TOFF_FALL, i2c_num, fTime);
}

bool dps::SetLtcToffMaxWarnLimit(PCI_SEL pci, uint i2c_num, float fTime) // ms
{
	// if( !isValidVout(fVolt) ) return false;
	// return SetLtcCommandL( I2C_CMD_TON_MAX_FAULT_LIMIT, sel, i2c_num, fTime );
	return SetLtcCommandL(pci, I2C_CMD_TOFF_MAX_WARN_LIMIT, i2c_num, fTime);
}

bool dps::setMfrIinCalGain(PCI_SEL pci, uint i2c_num, float fGain)
{
	return SetLtcCommandL(pci, I2C_CMD_MFR_IIN_CAL_GAIN, i2c_num, fGain);
}

bool dps::SetLtcMfrRVin(PCI_SEL pci, uint i2c_num, float fTime) // ohm
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandL(pci, I2C_CMD_MFR_RVIN, i2c_num, fTime);
}

bool dps::SetLtcMfrPwmComp(PCI_SEL pci, uint i2c_num, uint pwm_comp) // REG
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandR(pci, I2C_CMD_MFR_PWM_COMP, i2c_num, pwm_comp);
}

bool dps::SetLtcMfrPwmMode(PCI_SEL pci, uint i2c_num, uint pwm_mode) // REG
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandR(pci, I2C_CMD_MFR_PWM_MODE, i2c_num, pwm_mode);
}

uint dps::getLtcMfrPwmMode(PCI_SEL pci, uint i2c_num)
{
	return GetLtcCommandR(pci, I2C_CMD_MFR_PWM_MODE, i2c_num);
}

bool dps::SetLtcRailAddress(PCI_SEL pci, uint i2c_num, uint nRail) // REG
{
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandR(pci, I2C_CMD_MFR_RAIL_ADDRESS, i2c_num, nRail);
}

bool dps::SetLtcPolyMode(PCI_SEL pci, uint i2c_num, uint nEnable) // REG
{
	uint8_t nData = 0x10;
	if (nEnable)
		nData |= 0x80;
	// if( !isValidVout(fVolt) ) return false;
	return SetLtcCommandR(pci, I2C_CMD_MFR_PWM_CONFIG, i2c_num, nData);
}

// STATUS BYTE
uint dps::GetLtcStatusByte(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_STATUS_BYTE, i2c_num);
}

uint dps::GetLtcStatusWord(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_STATUS_WORD, i2c_num);
}

uint dps::GetLtcStatusVout(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_STATUS_VOUT, i2c_num);
}

uint dps::GetLtcStatusIout(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_STATUS_IOUT, i2c_num);
}

uint dps::GetLtcStatusInput(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_STATUS_INPUT, i2c_num);
}

uint dps::GetLtcStatusTemperature(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_STATUS_TEMPERATURE, i2c_num);
}

uint dps::GetLtcStatusCml(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_STATUS_CML, i2c_num);
}

uint dps::GetLtcStatusMfrSpecific(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_STATUS_MFR_SPECEFIC, i2c_num);
}

uint dps::GetLtcMfrCommon(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_MFR_COMMON, i2c_num);
}

//uint dps::GetLtcMfrPads(DPS_GROUP_SINGLE sel, uint i2c_num)
//{
//	if (sel == DPS_GROUP_SEL)
//	{
//		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
//		return false;
//	}
//
//	return GetLtcCommandR(I2C_CMD_MFR_PADS, i2c_num);
//}

uint dps::GetLtcMfrPads(PCI_SEL pci, uint ch)
{
	return GetLtcCommandR(pci, I2C_CMD_MFR_PADS, ch);
}

float dps::GetMfrVoutMax(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_MFR_VOUT_MAX, i2c_num);
}

float dps::ReadVin(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_READ_VIN, i2c_num);
}

float dps::ReadIin(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_READ_IIN, i2c_num);
}

float dps::ReadVout(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num, uint sampleing)
{
	float fMeas = 0.0;
	float fMeasSum = 0.0;
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	for (uint nCnt = 0; nCnt < sampleing; nCnt++)
	{
		fMeasSum += GetLtcCommandL(pci, I2C_CMD_READ_VOUT, i2c_num);
	}
	fMeas = fMeasSum / (float)sampleing;
	return fMeas;
}

float dps::ReadIout(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num, uint sampleing)
{
	float fMeas = 0.0;
	float fMeasSum = 0.0;
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	for (uint nCnt = 0; nCnt < sampleing; nCnt++)
	{
		float fTemp = 0.0;
		fTemp = GetLtcCommandL(pci, I2C_CMD_READ_IOUT, i2c_num);
		// printf( "ReadIout : %7.3lf \n", fTemp );
		if (nCnt > 0)
			usleep(16000); // ltc adc conversion delay
		fMeasSum += fTemp;
		// fMeasSum += GetLtcCommandL( I2C_CMD_READ_IOUT, i2c_num );
	}
	fMeas = fMeasSum / (float)sampleing;
	return fMeas;
}

float dps::ReadTemperature_1(DPS_GROUP_SINGLE sel, uint ch)
{
	PCI_SEL pci = ch / 10 == 0 ? PCI1 : PCI2 ;
	int i2c_num = (ch % 10) / 2;
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}
//	SetLtcCommandR(I2C_CMD_MFR_ADC_CONTROL, i2c_num, 0x0c);
//	usleep(8000);
	
	return GetLtcCommandL(pci, I2C_CMD_READ_TEMP_1, i2c_num);

}

float dps::ReadTemperature_2(DPS_GROUP_SINGLE sel, uint ch)
{
	PCI_SEL pci = ch / 10 == 0 ? PCI1 : PCI2 ;
	int i2c_num = (ch % 10) / 2;
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

//	SetLtcCommandR(I2C_CMD_MFR_ADC_CONTROL, i2c_num, 0x04);
//	usleep(8000);
	
	return GetLtcCommandL(pci, I2C_CMD_READ_TEMP_2, i2c_num);

}

float dps::ReadMfrTemperature_1_Peak(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_MFR_TEMPERATUR_1_PEAK, i2c_num);
}

float dps::ReadMfrTemperature_2_Peak(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_MFR_TEMPERATUR_2_PEAK, i2c_num);
}

float dps::ReadFrequency(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_READ_FREQUENCY, i2c_num);
}

float dps::ReadPout(PCI_SEL pci,DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_READ_POUT, i2c_num);
}

float dps::ReadPin(PCI_SEL pci,DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_READ_PIN, i2c_num);
}

float dps::ReadMfrVoutPeak(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_MFR_VOUT_PEAK, i2c_num);
}

float dps::ReadMfrIoutPeak(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_MFR_IOUT_PEAK, i2c_num);
}

float dps::ReadMfrIinPeak(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_MFR_READ_IIN_PEAK, i2c_num);
}

float dps::ReadMfrReadIChip(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_MFR_READ_ICHIP, i2c_num);
}

float dps::ReadMfrVinPeak(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_MFR_VIN_PEAK, i2c_num);
}

float dps::ReadMfrIinCalGain(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandL(pci, I2C_CMD_MFR_IIN_CAL_GAIN, i2c_num);
}

uint dps::ReadMfrAddress(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_MFR_ADDRESS, i2c_num);
}

uint dps::ReadMfrSpecialId(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_MFR_SPECIAL_ID, i2c_num);
}

uint dps::ReadPmbusRev(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandR(pci, I2C_CMD_PMBUS_REV, i2c_num);
}

bool dps::ReadMfrId(PCI_SEL pci, DPS_GROUP_SINGLE sel, uint i2c_num, char *dest)
{
	if (sel == DPS_GROUP_SEL)
	{
		printe("%s : error Dps I2C channel sel, use DPS_CHANNEL_SEL\n", __FUNCTION__);
		return false;
	}

	return GetLtcCommandA(pci, I2C_CMD_PMBUS_REV, i2c_num, dest);
}

// Default Status Word Check. Include Byte Status.
bool dps::StatusCheck(PCI_SEL pci, uint ltc, uint page, cstring &msg)
{
	bool bOk = true;
	stStatusWord status;
	cstring strMsg = "";

	SetLtcPage(pci, ltc, page);
	status.WORD_STATUS = GetLtcStatusWord(pci, DPS_SINGLE_SEL, ltc);
	if (m_nDebug)
		printf("%s CH:%02d PAGE%d STATUS WORD : 0x%04X\n", m_sLtcPart.c_str(), ltc, page, status.WORD_STATUS);

	if (status.contents.VOUT_OV)
	{
		msg += strMsg;
		bOk = false;
	}
	if (status.contents.IOUT_OC) // STATUS_IOUT
	{
		StatusIout(pci, ltc, page, strMsg);
		msg += strMsg;
		bOk = false;
	}

	// 	if( status.contents.VOUT_WARN || status.contents.VOUT_OV  )		//STATUS_VOUT
	// 	{
	// 		StatusVout(ltc,page,strMsg);
	// 		msg += strMsg;
	// 		bOk = false;
	// 	}
	// 	if( status.contents.IOUT_WARN || status.contents.IOUT_OC  )		//STATUS_IOUT
	// 	{
	// 		StatusIout(ltc,page,strMsg);
	// 		msg += strMsg;
	// 		bOk = false;
	// 	}
	// 	if( status.contents.INPUT )	//STATUS_INPUT
	// 	{
	// 		StatusInput(ltc,page,strMsg);
	// 		msg += strMsg;
	// 		bOk = false;
	// 	}
	// 	if( status.contents.MFR_SPECIFIC )	//STATUS_MFR_SPECIFIC
	// 	{
	// 		StatusMfrSpecific(ltc,page,strMsg);
	// 		msg += strMsg;
	// 		bOk = false;
	// 	}
	// 	if( !status.contents.POWER_GOOD )	//POWER_GOOD#
	// 	{
	// 		StatusMfrPads(ltc,page,strMsg);
	// 		msg += strMsg;
	// 		bOk = false;
	// 	}
	// 	if( status.contents.CML )		//STATUS_CML
	// 	{
	// 		StatusCml(ltc,page,strMsg);
	// 		msg += strMsg;
	// 		bOk = false;
	// 	}
	// 	if( status.contents.TEMPERATURE )	//STATUS_TEMPERATURE
	// 	{
	// 		StatusTemperature(ltc,page,strMsg);
	// 		msg += strMsg;
	// 		bOk = false;
	// 	}
	// 	if( status.contents.BUSY )		//MFR_COMMON
	// 	{
	// 		StatusMfrCommon(ltc,page,strMsg);
	// 		msg += strMsg;
	// 		bOk = false;
	// 	}
	if (bOk)
		msg = "";
	else
		msg += "\n";
	return bOk;
}

bool dps::StatusMfrCommon(PCI_SEL pci, uint ltc, uint page, cstring &msg)
{
	bool bOk = true;
	stStatusMfrCommon status;

	SetLtcPage(pci, ltc, page);
	status.MFR_COMMON = GetLtcStatusWord(pci, DPS_SINGLE_SEL, ltc);
	// printf( "%s %02d PAGE%d STATUS MFR COMMON: 0x%04X\n", m_sLtcPart.c_str(), ltc, page, status.MFR_COMMON );
	if (status.contents.WP_PIN_HIGH)
	{
		bOk = false;
		msg += "WP_PIN_HIGH, ";
	}
	if (status.contents.SHARE_CLK_LOW)
	{
		msg += "SHARE_CLK_LOW, ";
		bOk = false;
	}
	if (status.contents.READS_0)
	{
		msg += "READS_0, ";
		bOk = false;
	}
	if (status.contents.EEPROM_INITIALIZED)
	{
		msg += "EEPROM_INITIALIZED, ";
		bOk = false;
	}
	if (status.contents.OUTPUT_NOT_IN_TRANSITION)
	{
		msg += "OUTPUT_NOT_IN_TRANSITION, ";
		bOk = false;
	}
	if (status.contents.INTERNAL_CALCULATION_NOT_PENDING)
	{
		msg += "INTERNAL_CALCULATION_NOT_PENDING, ";
		bOk = false;
	}
	if (status.contents.CHIP_NOT_BUSY)
	{
		msg += "CHIP_NOT_BUSY, ";
		bOk = false;
	}
	if (status.contents.CHIP_NOT_DRIVING_ALERT_LOW)
	{
		msg += "CHIP_NOT_DRIVING_ALERT_LOW, ";
		bOk = false;
	}
	if (bOk)
		msg = "";
	else
		msg += "\n";
	return bOk;
}

bool dps::StatusMfrPads(PCI_SEL pci, uint ltc, uint page, cstring &msg)
{
	bool bOk = true;
	stStatusMfrPads status;

	SetLtcPage(pci, ltc, page);
	status.MFR_PADS = GetLtcStatusWord(pci, DPS_SINGLE_SEL, ltc);
	printf("%s %02d PAGE%d STATUS MFR PADS: 0x%04X\n", m_sLtcPart.c_str(), ltc, page, status.MFR_PADS);
	if (status.contents.FAULT_PIN_STATE0)
	{
		msg += "FAULT_PIN_STATE0, ";
		bOk = false;
	}
	if (status.contents.FAULT_PIN_STATE1)
	{
		msg += "FAULT_PIN_STATE1, ";
		bOk = false;
	}
	if (status.contents.LTC_FORCING_FAULT0_LOW)
	{
		msg += "LTC_FORCING_FAULT0_LOW, ";
		bOk = false;
	}
	if (status.contents.LTC_FORCING_FAULT1_LOW)
	{
		msg += "LTC_FORCING_FAULT1_LOW, ";
		bOk = false;
	}
	if (status.contents.RUN0_PIN_STATE)
	{
		msg += "RUN0_PIN_STATE, ";
		bOk = false;
	}
	if (status.contents.RUN1_PIN_STATE)
	{
		msg += "RUN1_PIN_STATE, ";
		bOk = false;
	}
	if (status.contents.LTC_FORCING_RUN0_LOW)
	{
		msg += "LTC_FORCING_RUN0_LOW, ";
		bOk = false;
	}
	if (status.contents.LTC_FORCING_RUN1_LOW)
	{
		msg += "LTC_FORCING_RUN1_LOW, ";
		bOk = false;
	}

	if (status.contents.CHANNEL0_POWER_GOOD)
	{
		msg += "CHANNEL0_POWER_GOOD, ";
		bOk = false;
	}
	if (status.contents.CHANNEL1_POWER_GOOD)
	{
		msg += "CHANNEL1_POWER_GOOD, ";
		bOk = false;
	}
	if (status.contents.SYNC_CLOCKED_BY_EXTERN_SRC)
	{
		msg += "SYNC_CLOCKED_BY_EXTERN_SRC, ";
		bOk = false;
	}
	if (status.contents.INVALID_ADC_RESULT)
	{
		msg += "INVALID_ADC_RESULT, ";
		bOk = false;
	}
	if (status.contents.READ0_0)
	{
		msg += "READ0_0, ";
		bOk = false;
	}
	if (status.contents.READ0_1)
	{
		msg += "READ0_1, ";
		bOk = false;
	}
	if (status.contents.VDD33_OV_FAULT)
	{
		msg += "VDD33_OV_FAULT, ";
		bOk = false;
	}
	if (status.contents.VDD33_UV_FAULT)
	{
		msg += "VDD33_UV_FAULT, ";
		bOk = false;
	}
	if (bOk)
		msg = "";
	else
		msg += "\n";
	return bOk;
}

// Default Status Word Check. Include Byte Status.
bool dps::StatusByte(PCI_SEL pci, uint ltc, uint page, cstring &msg)
{
	bool bOk = true;
	stStatusByte status;

	SetLtcPage(pci, ltc, page);
	status.BYTE_STATUS = GetLtcStatusWord(pci, DPS_SINGLE_SEL, ltc);
	printf("%s %02d PAGE%d STATUS WORD : 0x%04X\n", m_sLtcPart.c_str(), ltc, page, status.BYTE_STATUS);
	if (status.contents.NONE_OF_THE_ABOVE) // STATUS_VOUT
	{
		msg += "NONE_OF_THE_ABOVE, ";
		bOk = false;
	}
	if (status.contents.CML) // STATUS_IOUT
	{
		msg += "CML, ";
		bOk = false;
	}
	if (status.contents.TEMPERATURE) // STATUS_INPUT
	{
		msg += "TEMPERATURE, ";
		bOk = false;
	}
	if (status.contents.IOUT_OC) // STATUS_MFR_SPECIFIC
	{
		msg += "IOUT_OC, ";
		bOk = false;
	}
	if (status.contents.VOUT_OV) // MFR_PADS
	{
		msg += "VOUT_OV, ";
		bOk = false;
	}
	if (status.contents.OFF) // STATUS_CML
	{
		msg += "OFF, ";
		bOk = false;
	}
	if (status.contents.BUSY) // STATUS_TEMPERATURE
	{
		msg += "BUSY, ";
		bOk = false;
	}
	if (bOk)
		msg = "";
	else
		msg += "\n";
	return bOk;
}

bool dps::StatusInput(PCI_SEL pci, uint ltc, uint page, cstring &msg)
{
	bool bOk = true;
	stStatusInput status;
	SetLtcPage(pci, ltc, page);

	status.INPUT_STATUS = GetLtcStatusInput(pci, DPS_SINGLE_SEL, ltc);
	printf("readvin %5.3f\n", ReadVin(pci, DPS_SINGLE_SEL, ltc));
	printf("%s %02d PAGE%d STATUS INPUT : 0x%04X\n", m_sLtcPart.c_str(), ltc, page, status.INPUT_STATUS);

	if (status.contents.VIN_OV_FAULT)
	{
		msg += "VIN_OV_FAULT, ";
		bOk = false;
	}
	if (status.contents.VIN_UV_WARN)
	{
		msg += "VIN_UV_WARN, ";
		bOk = false;
	}
	if (status.contents.IIN_OC_WARN)
	{
		msg += "IIN_OC_WARN, ";
		bOk = false;
	}
	if (bOk)
		msg = "";
	else
		msg += "\n";
	return bOk;
}

bool dps::StatusVout(PCI_SEL pci, uint ltc, uint page, cstring &msg)
{
	bool bOk = true;
	stStatusVout status;
	SetLtcPage(pci, ltc, page);

	status.VOUT_STATUS = GetLtcStatusVout(pci, DPS_SINGLE_SEL, ltc);
	printf("%s %02d PAGE%d STATUS VOUT : 0x%04X\n", m_sLtcPart.c_str(), ltc, page, status.VOUT_STATUS);
	if (status.contents.VOUT_OV_FAULT)
	{
		msg += "VOUT_OV_FAULT, ";
		bOk = false;
	}
	if (status.contents.VOUT_OV_WARN)
	{
		msg += "VOUT_OV_WARN, ";
		bOk = false;
	}
	if (status.contents.VOUT_UV_WARN)
	{
		msg += "VOUT_UV_WARN, ";
		bOk = false;
	}
	if (status.contents.VOUT_UV_FAULT)
	{
		msg += "VOUT_UV_FAULT, ";
		bOk = false;
	}
	if (bOk)
		msg = "";
	else
		msg += "\n";
	return bOk;
}

bool dps::StatusIout(PCI_SEL pci, uint ltc, uint page, cstring &msg)
{
	bool bOk = true;
	stStatusIout status;
	SetLtcPage(pci, ltc, page);

	status.IOUT_STATUS = GetLtcStatusVout(pci, DPS_SINGLE_SEL, ltc);
	printf("%s %02d PAGE%d STATUS IOUT : 0x%04X\n", m_sLtcPart.c_str(), ltc, page, status.IOUT_STATUS);
	if (status.contents.IOUT_OC_FAULT)
	{
		msg += "IOUT_OC_FAULT, ";
		bOk = false;
	}
	if (status.contents.IOUT_OC_WARN)
	{
		msg += "IOUT_OC_WARN, ";
		bOk = false;
	}
	if (bOk)
		msg = "";
	else
		msg += "\n";
	return bOk;
}

bool dps::StatusTemperature(PCI_SEL pci, uint ltc, uint page, cstring &msg)
{
	bool bOk = true;
	stStatusTemperature status;
	SetLtcPage(pci, ltc, page);

	status.TEMP_STATUS = GetLtcStatusVout(pci, DPS_SINGLE_SEL, ltc);
	printf("%s %02d PAGE%d STATUS TEMPERATUR : 0x%04X\n", m_sLtcPart.c_str(), ltc, page, status.TEMP_STATUS);
	if (status.contents.OT_FAULT)
	{
		msg += "OT_FAULT, ";
		bOk = false;
	}
	if (status.contents.OT_WARN)
	{
		msg += "OT_WARN, ";
		bOk = false;
	}
	if (status.contents.UT_FAULT)
	{
		msg += "UT_FAULT, ";
		bOk = false;
	}
	if (bOk)
		msg = "";
	else
		msg += "\n";
	return bOk;
}

bool dps::StatusCml(PCI_SEL pci, uint ltc, uint page, cstring &msg)
{
	bool bOk = true;
	stStatusCml status;
	SetLtcPage(pci, ltc, page);

	status.CML_STATUS = GetLtcStatusVout(pci, DPS_SINGLE_SEL, ltc);
	printf("%s %02d PAGE%d STATUS CML : 0x%04X\n", m_sLtcPart.c_str(), ltc, page, status.CML_STATUS);
	if (status.contents.OTHER_MEMORY_OR_LOGIC_FAULT)
	{
		msg += "OT_FAULT, ";
		bOk = false;
	}
	if (status.contents.OTHER_COMMUNICATION_FAULT)
	{
		msg += "OTHER_COMMUNICATION_FAULT, ";
		bOk = false;
	}
	if (status.contents.RESERVED_LTC388N_RETURNS_0)
	{
		msg += "RESERVED_LTC388N_RETURNS_0, ";
		bOk = false;
	}
	if (status.contents.PROCESSOR_FAULT_DETECTED)
	{
		msg += "PROCESSOR_FAULT_DETECTED, ";
		bOk = false;
	}
	if (status.contents.MEMORY_FAULT_DETECTED)
	{
		msg += "MEMORY_FAULT_DETECTED, ";
		bOk = false;
	}
	if (status.contents.PACKET_ERROR_CHECK_FAILED)
	{
		msg += "PACKET_ERROR_CHECK_FAILED, ";
		bOk = false;
	}
	if (status.contents.INVALID_OR_UNSUPPORTED_DATA_RECEIVED)
	{
		msg += "INVALID_OR_UNSUPPORTED_DATA_RECEIVED, ";
		bOk = false;
	}
	if (status.contents.INVALID_OR_UNSUPPORTED_CMD_RECEIVED)
	{
		msg += "INVALID_OR_UNSUPPORTED_CMD_RECEIVED, ";
		bOk = false;
	}
	if (bOk)
		msg = "";
	else
		msg += "\n";
	return bOk;
}

bool dps::StatusMfrSpecific(PCI_SEL pci, uint ltc, uint page, cstring &msg)
{
	bool bOk = true;
	stStatusMfrSpecific status;
	SetLtcPage(pci, ltc, page);

	status.MFR_SPECFIC_STATUS = GetLtcStatusMfrSpecific(pci, DPS_SINGLE_SEL, ltc);
	printf("%s %02d PAGE%d STATUS MFR SPECIFIC : 0x%04X\n", m_sLtcPart.c_str(), ltc, page, status.MFR_SPECFIC_STATUS);
	if (status.contents.FAULT_PULLED_LOW_BY_ERTERNAL_DEVICE)
	{
		msg += "FAULT_PULLED_LOW_BY_ERTERNAL_DEVICE, ";
		bOk = false;
	}
	if (status.contents.VOUT_SHORT_CYCLED)
	{
		msg += "VOUT_SHORT_CYCLED, ";
		bOk = false;
	}
	if (status.contents.VDD33_UV_OR_OV_FAULT)
	{
		msg += "VDD33_UV_OR_OV_FAULT, ";
		bOk = false;
	}
	if (status.contents.FAULT_LOG_PRESENT)
	{
		msg += "FAULT_LOG_PRESENT, ";
		bOk = false;
	}
	if (status.contents.INTERNAL_PLL_UNLOCKED)
	{
		msg += "INTERNAL_PLL_UNLOCKED, ";
		bOk = false;
	}
	if (status.contents.EEPROM_CRC_ERROR)
	{
		msg += "EEPROM_CRC_ERROR, ";
		bOk = false;
	}
	if (status.contents.INTERNAL_TEMPERATURE_WARNING)
	{
		msg += "INTERNAL_TEMPERATURE_WARNING, ";
		bOk = false;
	}
	if (status.contents.INTERNAL_TEMPERATUR_FAULT)
	{
		msg += "INTERNAL_TEMPERATUR_FAULT, ";
		bOk = false;
	}
	if (bOk)
		msg = "";
	else
		msg += "\n";
	return bOk;
}

uint dps::DumpCommand(PCI_SEL pci, uint ltc, uint page, DPS_PMBUS_COMMAND cmd)
{
	int nFmt, nType, nDefault;
	char szId[32] = {
			0x0,
	};
	uint rData = 0x0;
	cstring sCmd = "";

	if (GetDpsCommandInfo(cmd, nFmt, nType, nDefault, sCmd))
	{
		SetLtcPage(pci, ltc, page);
		if ((nFmt == FORMAT_L11) || (nFmt == FORMAT_L16))
		{
			rData = GetLtcCommandR(pci, cmd, ltc);
			printf("\t[CH%02d] %-35s 0x%02X : %7.3f\n", ltc, sCmd.c_str(), (int)cmd, GetLtcCommandL(pci, cmd, ltc));
		}
		else if (nFmt == FORMAT_ASC)
		{
			memset(szId, 0x0, sizeof(szId));
			GetLtcCommandA(pci, cmd, ltc, szId);
			printf("\t[CH%02d] %-35s 0x%02X :  %s\n", ltc, sCmd.c_str(), (int)cmd, szId);
		}
		else
		{
			rData = GetLtcCommandR(pci, cmd, ltc);
			printf("\t[CH%02d] %-35s 0x%02X :  0x%04X\n", ltc, sCmd.c_str(), (int)cmd, rData);
		}
	}
	else
	{
		printf("%s : error LTC Command map is empty!!\n", __FUNCTION__);
	}
	return rData;
}

bool dps::ltcDumpCh(PCI_SEL pci, uint ltc, uint page)
{
	int nFmt = 0x0;
	int nType = 0x0;
	char szId[32] = {
			0x0,
	};
	cstring sCmd = "";
	DPS_PMBUS_COMMAND cmd;

	SetLtcPage(pci, ltc, page);
	printf("LTC%d [PAGE%d] -%30s %-8s  %-10s\n", ltc, page, "COMMAND NAME", "CMD CODE", "VALUE");
	printf("-------------------------------------------------------------------------------\n");

	if (m_mapCmdLtc.size() < 1)
	{
		printf("%s : error LTC Command map is empty!!\n", __FUNCTION__);
		return false;
	}
	std::map<DPS_PMBUS_COMMAND, stPMBusCommand>::iterator it;
	for (it = m_mapCmdLtc.begin(); it != m_mapCmdLtc.end(); it++)
	{
		cmd = it->first;
		sCmd = it->second.Command;
		nFmt = it->second.DataFormat;
		nType = it->second.Type;

		if (cmd == I2C_CMD_MFR_INFO)
			continue;
		if ((nType == TYPE_SENDBYTE))
			continue;
		if ((nFmt == FORMAT_L11) || (nFmt == FORMAT_L16))
		{

			printf("\t%-35s 0x%02X : %7.3f\n",
						sCmd.c_str(), (int)cmd, GetLtcCommandL(pci, cmd, ltc));
		}
		else if (nFmt == FORMAT_ASC)
		{
			memset(szId, 0x0, sizeof(szId));
			GetLtcCommandA(pci, cmd, ltc, szId);
			printf("\t%-35s 0x%02X :  %s\n", sCmd.c_str(), (int)cmd, szId);
		}
		else
		{
			printf("\t%-35s 0x%02X :  0x%04X\n", sCmd.c_str(), (int)cmd, GetLtcCommandR(pci, cmd, ltc));
		}
	}

	SetLtcPage(pci, ltc, 0x0);
	return true;
}

//bool dps::AllChannelInit()
//{
//	gIO.dps_run(PCI1, 0x3FF, 0x1, 0);
//	gIO.dps_run(PCI2, 0x3FF, 0x1, 0);
//	usleep(10000);
//	//Sleep(0.01);
//	std::vector<std::pair<DPS_PMBUS_COMMAND, unsigned int>> vecInitCmd, vecInitCmd2;
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_SMBALERT_MASK, 0xFF7A));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_SMBALERT_MASK, 0xFF7C));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_SMBALERT_MASK, 0xFF7D));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_SMBALERT_MASK, 0xFF7E));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_SMBALERT_MASK, 0xFF80));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_WRITE_PROTECT, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_FREQUENCY_SWITCH, 0xFBE8));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_VIN_ON, 0xCB40));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_VIN_OFF, 0xCB00));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_VIN_OV_FAULT_LIMIT, 0xD3F0));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_VIN_UV_WARN_LIMIT, 0xCB26));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_IIN_OC_WARN_LIMIT, 0xD3C0));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_CONFIG_ALL, 0x00A1)); //0x00A3)); //0x00A1)); ///////
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_ADDRESS, 0x004F));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_IIN_CAL_GAIN, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_PWM_CONFIG, 0x0010));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_RVIN, 0x03E8));
//
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_OPERATION, 0x0080));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_ON_OFF_CONFIG, 0x001E));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_TRANSITION_RATE, 0xAA00));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_IOUT_CAL_GAIN, 0xBA00));// 0xBAA0)); //0xBA00));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_OV_FAULT_RESPONSE, 0x00BF));//0x00B8));//0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_UV_FAULT_RESPONSE, 0x00BF));//0x00B8));//0x0040));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_IOUT_OC_FAULT_RESPONSE, 0x00C7));//0x00C0));//0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_OT_FAULT_RESPONSE, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_UT_FAULT_RESPONSE, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_VIN_OV_FAULT_RESPONSE, 0x0080));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_TON_DELAY, 0x8000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_TON_RISE, 0xD200));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_TON_MAX_FAULT_LIMIT, 0xD280));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_TON_MAX_FAULT_RESPONSE, 0x00B8));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_TOFF_DELAY, 0x8000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_TOFF_FALL, 0xE200));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_TOFF_MAX_WARN_LIMIT, 0xF258));
//
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_CHAN_CONFIG, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_FAULT_PROPAGATE, 0x6993));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_PWM_COMP, 0x001A));//0x0019));//0x0019));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_PWM_MODE, 0x0081));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_FAULT_RESPONSE, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_RETRY_DELAY, 0xFABC));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_RESTART_DELAY, 0xFBE8));
//
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_IOUT_CAL_GAIN_TC, 0x0048));//0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_TEMP_1_GAIN, 0x4000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_TEMP_1_OFFSET, 0x8000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_RAIL_ADDRESS, 0x0030));
//
//	// Sleep(0.01);
//	// vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_ADC_CONTROL, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_BYTE, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_WORD, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_VOUT, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_IOUT, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_TEMPERATURE, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_INPUT, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_CML, 0x0000));
//	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_MFR_SPECEFIC, 0x0000));
//
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_VOUT_COMMAND, 0x1000));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_VOUT_MAX, 0x5b33));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_VOUT_MARGIN_HIGH, 0x10cd));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_VOUT_MARGIN_LOW, 0x0f33));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_VOUT_OV_FAULT_LIMIT, 0x119a));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_VOUT_OV_WARN_LIMIT, 0x1133));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_VOUT_UV_FAULT_LIMIT, 0x0ecd));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_VOUT_UV_WARN_LIMIT, 0x0e66));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_IOUT_OC_FAULT_LIMIT, 0xea80));//0xe380));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_IOUT_OC_WARN_LIMIT, 0xe2c0));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_OT_FAULT_LIMIT, 0x2271));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_OT_WARN_LIMIT, 0xeaa8));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_UT_FAULT_LIMIT, 0xfdda));
//
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_STATUS_BYTE, 0x0000));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_STATUS_WORD, 0x0000));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_STATUS_VOUT, 0x0000));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_STATUS_IOUT, 0x0000));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_STATUS_TEMPERATURE, 0x0000));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_STATUS_INPUT, 0x0000));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_STATUS_CML, 0x0000));
//	vecInitCmd2.push_back(std::make_pair(I2C_CMD_STATUS_MFR_SPECEFIC, 0x0000));
//	// vecInitCmd2.push_back(std::make_pair(I2C_CMD_MFR_CLEAR_PEAKS, 0x0000));
//
//	cstring sCmd = "";
//	DPS_PMBUS_COMMAND cmd;
//	uint16_t uData = 0;
//	int type;
//
//	for(int nPCI = DBG_PCI_NU; nPCI < MAX_PCI; nPCI++) {
//		//I2cSendByteAll(I2C_CMD_CLEAR_FAULTS);
//
//		for (unsigned int num = DPS_START_NU; num < MAX_DPS_DEVICE; num++)
//		{
//			int cnt = 0; 
//			for (unsigned int idx = 0; idx < vecInitCmd.size(); idx++)
//			{
//				printf("0 count %d\n", cnt++);
//				SetLtcPage(PCI_SEL(nPCI), num, LTC_PAGE_ALL);
//				uData = vecInitCmd[idx].second;
//				std::map<DPS_PMBUS_COMMAND, stPMBusCommand>::iterator it = m_mapCmdLtc.find(vecInitCmd[idx].first);
//				cmd = it->first;
//				sCmd = it->second.Command;
//				type = it->second.Type;
//				// printf("%s\n", m_mapCmdLtc[cmd].Command.c_str());
//				I2cWrite(PCI_SEL(nPCI), num, cmd, uData, (LTC_PMBUS_COMMAND_TYPE)type);
//
//			}
//			for (unsigned int idx = 0; idx < vecInitCmd2.size(); idx++)
//			{
//				printf("1 count %d\n", cnt++);
//				SetLtcPage(PCI_SEL(nPCI), num, LTC_PAGE_ALL);
//				uData = vecInitCmd2[idx].second;
//				std::map<DPS_PMBUS_COMMAND, stPMBusCommand>::iterator it = m_mapCmdLtc.find(vecInitCmd2[idx].first);
//				cmd = it->first;
//				sCmd = it->second.Command;
//				type = it->second.Type;
//				I2cWrite(PCI_SEL(nPCI), num, cmd, uData, (LTC_PMBUS_COMMAND_TYPE)type);
//
//			}
//			usleep(1000);
//			//Sleep(0.001);
//		}
//	}
//
//	SetHVout(0.0);
//	return true;
//
//}

bool dps::AllChannelInit()
{
	gIO.dps_run(PCI1, 0x3FF, 0x1, 0);
	gIO.dps_run(PCI2, 0x3FF, 0x1, 0);
	usleep(10000);

	std::vector<std::pair<DPS_PMBUS_COMMAND, unsigned int>> vecInitCmd;
	
	vecInitCmd.push_back(std::make_pair(I2C_CMD_SMBALERT_MASK, 0xFF7A));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_SMBALERT_MASK, 0xFF7C));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_SMBALERT_MASK, 0xFF7D));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_SMBALERT_MASK, 0xFF7E));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_SMBALERT_MASK, 0xFF80));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_WRITE_PROTECT, 0x0000));
	//vecInitCmd.push_back(std::make_pair(I2C_CMD_CLEAR_FAULTS,  0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_FREQUENCY_SWITCH, 0xFBE8));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VIN_ON, 0xCB40));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VIN_OFF, 0xCB00));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VIN_OV_FAULT_LIMIT, 0xD3F0));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VIN_UV_WARN_LIMIT, 0xCB26));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_IIN_OC_WARN_LIMIT, 0xD3C0));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_CONFIG_ALL, 0x0021));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_ADDRESS, 0x004F));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_IIN_CAL_GAIN, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_PWM_CONFIG, 0x0010));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_RVIN, 0x03E8));

	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_COMMAND, 0x1000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_MAX, 0x5b33));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_MARGIN_HIGH, 0x10cd));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_MARGIN_LOW, 0x0f33));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_OV_FAULT_LIMIT, 0x119a));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_OV_WARN_LIMIT, 0x1133));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_UV_FAULT_LIMIT, 0x0ecd));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_UV_WARN_LIMIT, 0x0e66));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_IOUT_OC_FAULT_LIMIT, 0xea80));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_IOUT_OC_WARN_LIMIT, 0xe2c0));
	//vecInitCmd2.push_back(std::make_pair(I2C_CMD_OT_FAULT_LIMIT, 0x2271));
	//vecInitCmd2.push_back(std::make_pair(I2C_CMD_OT_WARN_LIMIT, 0xeaa8));
	//vecInitCmd2.push_back(std::make_pair(I2C_CMD_UT_FAULT_LIMIT, 0xfdda));

	vecInitCmd.push_back(std::make_pair(I2C_CMD_OPERATION, 0x0080));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_ON_OFF_CONFIG, 0x001E));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_TRANSITION_RATE, 0xAA00));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_IOUT_CAL_GAIN, 0xBA00));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_OV_FAULT_RESPONSE, 0x00BF));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VOUT_UV_FAULT_RESPONSE, 0x00BF));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_IOUT_OC_FAULT_RESPONSE, 0x00C7));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_OT_FAULT_LIMIT, 0x2271));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_OT_FAULT_RESPONSE, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_OT_WARN_LIMIT, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_UT_FAULT_LIMIT, 0xFDDA));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_UT_FAULT_RESPONSE, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_VIN_OV_FAULT_RESPONSE, 0x0080));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_TON_DELAY, 0x8000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_TON_RISE, 0xD200));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_TON_MAX_FAULT_LIMIT, 0xD280));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_TON_MAX_FAULT_RESPONSE, 0x00B8));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_TOFF_DELAY, 0x8000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_TOFF_FALL, 0xD200));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_TOFF_MAX_WARN_LIMIT, 0xF258));

	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_CHAN_CONFIG, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_FAULT_PROPAGATE, 0x6993));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_PWM_COMP, 0x001A));//0x0019))
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_PWM_MODE, 0x0081));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_FAULT_RESPONSE, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_RETRY_DELAY, 0xFABC));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_RESTART_DELAY, 0xFBE8));

	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_IOUT_CAL_GAIN_TC, 0x0048));//0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_TEMP_1_GAIN, 0x4000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_TEMP_1_OFFSET, 0x8000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_MFR_RAIL_ADDRESS, 0x0030));//0x0080));"

	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_BYTE, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_WORD, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_VOUT, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_IOUT, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_TEMPERATURE, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_INPUT, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_CML, 0x0000));
	vecInitCmd.push_back(std::make_pair(I2C_CMD_STATUS_MFR_SPECEFIC, 0x0011));

	cstring sCmd = "";
	DPS_PMBUS_COMMAND cmd;
	uint16_t uData = 0;
	int type;

	SetLtcPageAll(LTC_PAGE_ALL);
	//I2cSendByteAll(I2C_CMD_CLEAR_FAULTS);
	I2cSendByteAll(I2C_CMD_MFR_RESET);
	printf("Fault Clear All\n");
	usleep(1000000);

	uint nTry = 5;

	for(int nPCI = DBG_PCI_NU; nPCI < MAX_PCI; nPCI++) {

		for (unsigned int num = DPS_START_NU; num < MAX_DPS_DEVICE; num++)
		{
			//SetLtcPage(PCI_SEL(nPCI), num, LTC_PAGE_ALL);
			//I2cSendByte(PCI_SEL(nPCI), num, I2C_CMD_CLEAR_FAULTS);
			//I2cRead(PCI_SEL(nPCI), num, I2C_CMD_PAGE, LTC_PMBUS_COMMAND_TYPE::TYPE_RW_BYTE);
			//usleep(1000);
			int cnt = 0; 
			for (unsigned int idx = 0; idx < vecInitCmd.size(); idx++)
			{
				uData = vecInitCmd[idx].second;
				std::map<DPS_PMBUS_COMMAND, stPMBusCommand>::iterator it = m_mapCmdLtc.find(vecInitCmd[idx].first);
				cmd = it->first;
				sCmd = it->second.Command;
				type = it->second.Type;
				// printf("%s\n", m_mapCmdLtc[cmd].Command.c_str());

				//if(cmd == I2C_CMD_CLEAR_FAULTS) {
				//	SetLtcClearFaults(PCI_SEL(nPCI), num, 0);
				//	continue;
				//}
				while(nTry--) {
					if(I2cWrite(PCI_SEL(nPCI), num, cmd, uData, (LTC_PMBUS_COMMAND_TYPE)type) == false)
						printe("init.err pci : %d, device %d , cmd %s data 0x%X\n", nPCI, num, sCmd.c_str(), uData );
				}

				nTry = 5;
			}
			usleep(1000);
			//Sleep(0.001);
		}
	}

	SetHVout(0, 0.0);
	SetHVout(1, 0.0);
	return true;

}

bool dps::SetVout(int ch, float fVolt, float under, float over, float limit, bool bCalApply) /// 0(0,1), 1(2,3), 2(4), 3(5)
{
	if (fVolt > 5.5) {
		printe("%s : error Dps Out Voltage MAX is 5.5, but setting is %f\n", __FUNCTION__, fVolt);
		return false;
	}

	if (ch >= MAX_DPS_CH) {
		printe("%s : error Dps channel number is invalid, but setting is %d\n", __FUNCTION__, ch);
		return false;
	}

	//int i2c_num = ch % 10;
	int i2c_num = (ch % 10) / 2;
	int nPCI = ch /10;
	int nPage = (ch & 0x1) == 0 ? LTC_PAGE0 : LTC_PAGE1;

	m_VOutVal[ch] = fVolt;
	if(bCalApply)
		fVolt = (fVolt * m_VSCal[ch].gain) + m_VSCal[ch].offset;

	float flimit = limit;
	if(bCalApply) {
		//float flimit = (limit/m_IMCal[ch].gain) - m_IMCal[ch].offset;
		flimit = (limit*m_IMCal[ch].gain) + m_IMCal[ch].offset;
	}

	if (!SetLtcPage(PCI_SEL(nPCI), i2c_num, nPage))
		printe("==== Error(ch%d) : I2C_CMD_PAGE(0x%X) ====\n", ch, I2C_CMD_PAGE);

	uint rPage = GetLtcCommandR(PCI_SEL(nPCI), I2C_CMD_PAGE, i2c_num);
	//printf("PCI(%d) DPS Set ch:%d, Volt:%f(%f)    ", nPCI, ch, m_VOutVal[ch], fVolt);
	//printf("Read LTC Page(ch%d) : %d\n", ch, rPage);

	if (!SetLtcCommandL(PCI_SEL(nPCI), I2C_CMD_VOUT_OV_FAULT_LIMIT, i2c_num, over))
		printe("==== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ====\n", ch, I2C_CMD_VOUT_OV_FAULT_LIMIT, over);
	if (!SetLtcCommandL(PCI_SEL(nPCI), I2C_CMD_VOUT_OV_WARN_LIMIT, i2c_num, over))
		printe("==== Error(ch%d) : I2C_CMD_VOUT_OV_WARN_LIMIT(0x%X) : Value(%f) ====\n", ch, I2C_CMD_VOUT_OV_FAULT_LIMIT, over);
	if (!SetLtcCommandL(PCI_SEL(nPCI), I2C_CMD_VOUT_MARGIN_HIGH, i2c_num, (over + fVolt) / 2.0))
		printe("==== Error(ch%d) : I2C_CMD_VOUT_MARGIN_HIGH(0x%X) :    Value(%f) ====\n", ch, I2C_CMD_VOUT_MARGIN_HIGH, (over + fVolt) / 2.0);
	if (!SetLtcCommandL(PCI_SEL(nPCI), I2C_CMD_VOUT_MARGIN_LOW, i2c_num, (under + fVolt) / 2.0))
		printe("==== Error(ch%d) : I2C_CMD_VOUT_MARGIN_LOW(0x%X) :     Value(%f) ====\n", ch, I2C_CMD_VOUT_MARGIN_LOW, (under + fVolt) / 2.0);
	if (!SetLtcCommandL(PCI_SEL(nPCI), I2C_CMD_VOUT_UV_FAULT_LIMIT, i2c_num, under))
		printe("==== Error(ch%d) : I2C_CMD_VOUT_UV_FAULT_LIMIT(0x%X) : Value(%f) ====\n", ch, I2C_CMD_VOUT_UV_FAULT_LIMIT, under);
	if (!SetLtcCommandL(PCI_SEL(nPCI), I2C_CMD_VOUT_UV_WARN_LIMIT, i2c_num, under))
		printe("==== Error(ch%d) : I2C_CMD_VOUT_UV_WARN_LIMIT(0x%X) : Value(%f) ====\n", ch, I2C_CMD_VOUT_UV_FAULT_LIMIT, under);
	if (!SetLtcCommandL(PCI_SEL(nPCI), I2C_CMD_IOUT_OC_WARN_LIMIT, i2c_num, flimit))
		printe("==== Error(ch%d) : I2C_CMD_IOUT_OC_FAULT_LIMIT(0x%X) : Value(%f) ====\n", ch, I2C_CMD_IOUT_OC_WARN_LIMIT, flimit);
	if (!SetLtcCommandL(PCI_SEL(nPCI), I2C_CMD_VOUT_COMMAND, i2c_num, fVolt))
		printe("==== Error(ch%d) : I2C_CMD_VOUT_COMMAND(0x%X) :        Value(%f) ====\n", ch, I2C_CMD_VOUT_COMMAND, fVolt);

	//float fread = GetLtcCommandL(PCI_SEL(nPCI),I2C_CMD_VOUT_COMMAND, i2c_num);
	//printe("VOUT READ %f\n", fread);

	//ReadAllreg(ch);

	return true;
}

bool dps::SetHVout(int ch, float fVolt)
{
	if (fVolt > 15.0) {
		printf("%s : error High Voltage MAX is 15.0, but setting is %f\n", __FUNCTION__, fVolt);
		return false;
	}

	PCI_SEL pci = ch == 0 ? PCI1 : PCI2;
	int idx = 20 + ch;
	m_VOutVal[idx] = fVolt;
	
	gIO.set_hv_dac(pci, fVolt);
}

bool dps::ReadChVout(int ch, float &Volt, int cnt, bool bCalApply)  // ch = 0~19
{
	PCI_SEL pci = ch / 10 == 0 ? PCI1 : PCI2 ;
	int i2c_num = (ch % 10) / 2;
	int ltc_page = ch%2;

	Volt = -1.0;
	SetLtcPage(pci, i2c_num, ltc_page);
	Volt = ReadVout(pci, DPS_GROUP_SINGLE::DPS_SINGLE_SEL, i2c_num, cnt);
	if(bCalApply)
		Volt = (Volt * m_VMCal[ch].gain) + m_VMCal[ch].offset;
	return true;
}

float dps::ReadChIout(int ch, int cnt, bool bCalApply)   
{
	PCI_SEL pci = ch / 10 == 0 ? PCI1 : PCI2 ;
	int i2c_num = (ch % 10) / 2;
	int ltc_page = ch%2;

	float fValue = 0.0;

	SetLtcPage(pci, i2c_num, ltc_page);
	fValue = ReadIout(pci, DPS_GROUP_SINGLE::DPS_SINGLE_SEL, i2c_num, cnt);

	if(bCalApply) {
		//return ((fValue * gOPR.m_DPSIMCal[ch].gain) + gOPR.m_DPSIMCal[ch].offset);
		return ((fValue * m_IMCal[ch].gain) + m_IMCal[ch].offset);
	}
	else
		return fValue;
}

uint dps::ReadStatus(int ch)
{
	PCI_SEL pci = ch / 10 == 0 ? PCI1 : PCI2;
	uint i2c_num = (ch % 10) / 2;

	return  GetLtcMfrPads(pci, i2c_num);
	
}

uint dps::ReadAlertCtrl(PCI_SEL pci)
{
	return gIO.dps_read_alert(pci);
}

float dps::ReadInaVout(int ch)
{
	PCI_SEL pci = ch / 10 == 0 ? PCI1 : PCI2 ;

	short code;
	gIO.ina_read_voltage(pci, (ch % 10), code);
	float volt = 0.00125 * code;
	return volt;
}

float dps::ReadInaIout(int ch)
{
	PCI_SEL pci = ch / 10 == 0 ? PCI1 : PCI2 ;
	short code;
	float gain = 1.27;
	// 임시 gain 적용 추 후 calibration 가능해지면 cal 에서 적용 예정
	gIO.ina_read_current(pci, (ch % 10), code);
	float current = INA_CUR_LSB * code * gain;
	//float current = ((0.0000025 * code) / (0.003)) * gain;
	return current;
}


bool dps::CommitVoutFaultLimit() 
{
	// Over Voltage
	if (!SetLtcCommandLGlobal(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 0, m_OVFVal[0]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 0, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_OVFVal[0]);
	if (!SetLtcCommandLGlobal(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 0, m_OVFVal[0]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 0, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_OVFVal[0]);

	if (!SetLtcCommandLGlobal(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 1, m_OVFVal[1]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 1, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_OVFVal[1]);
	if (!SetLtcCommandLGlobal(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 1, m_OVFVal[1]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 1, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_OVFVal[1]);

	if (!SetLtcCommandL(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 2, m_OVFVal[2]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 2, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_OVFVal[2]);
	if (!SetLtcCommandL(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 2, m_OVFVal[2]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 2, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_OVFVal[2]);

	if (!SetLtcCommandL(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 3, m_OVFVal[3]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 3, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_OVFVal[3]);
	if (!SetLtcCommandL(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 3, m_OVFVal[3]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 3, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_OVFVal[3]);

  // Under Voltage
	if (!SetLtcCommandLGlobal(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 0, m_UVFVal[0]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 0, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_UVFVal[0]);
	if (!SetLtcCommandLGlobal(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 0, m_UVFVal[0]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 0, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_UVFVal[0]);

	if (!SetLtcCommandLGlobal(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 1, m_UVFVal[1]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 1, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_UVFVal[1]);
	if (!SetLtcCommandLGlobal(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 1, m_UVFVal[1]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 1, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_UVFVal[1]);

	if (!SetLtcCommandL(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 2, m_UVFVal[2]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 2, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_UVFVal[2]);
	if (!SetLtcCommandL(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 2, m_UVFVal[2]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 2, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_UVFVal[2]);

	if (!SetLtcCommandL(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 3, m_UVFVal[3]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 3, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_UVFVal[3]);
	if (!SetLtcCommandL(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 3, m_UVFVal[3]))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 3, I2C_CMD_VOUT_OV_FAULT_LIMIT, m_UVFVal[3]);

	return true;
}

bool dps::ReleaseVoutFaultLimit() 
{
	if (!SetLtcCommandLGlobal(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 0, 5.5))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 0, I2C_CMD_VOUT_OV_FAULT_LIMIT, 5.5);
	if (!SetLtcCommandLGlobal(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 0, 5.5))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 0, I2C_CMD_VOUT_OV_FAULT_LIMIT, 5.5);

	if (!SetLtcCommandLGlobal(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 1, 5.5))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 1, I2C_CMD_VOUT_OV_FAULT_LIMIT, 5.5);
	if (!SetLtcCommandLGlobal(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 1, 5.5))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 1, I2C_CMD_VOUT_OV_FAULT_LIMIT, 5.5);

	if (!SetLtcCommandL(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 2, 5.5))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 2, I2C_CMD_VOUT_OV_FAULT_LIMIT, 5.5);
	if (!SetLtcCommandL(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 2, 5.5))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 2, I2C_CMD_VOUT_OV_FAULT_LIMIT, 5.5);

	if (!SetLtcCommandL(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 3, 5.5))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 3, I2C_CMD_VOUT_OV_FAULT_LIMIT, 5.5);
	if (!SetLtcCommandL(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 3, 5.5))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 3, I2C_CMD_VOUT_OV_FAULT_LIMIT, 5.5);

	// Under Voltage
	if (!SetLtcCommandLGlobal(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 0, 0.0))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 0, I2C_CMD_VOUT_OV_FAULT_LIMIT, 0.0);
	if (!SetLtcCommandLGlobal(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 0, 0.0))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 0, I2C_CMD_VOUT_OV_FAULT_LIMIT, 0.0);

	if (!SetLtcCommandLGlobal(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 1, 0.0))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 1, I2C_CMD_VOUT_OV_FAULT_LIMIT, 0.0);
	if (!SetLtcCommandLGlobal(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 1, 0.0))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 1, I2C_CMD_VOUT_OV_FAULT_LIMIT, 0.0);

	if (!SetLtcCommandL(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 2, 0.0))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 2, I2C_CMD_VOUT_OV_FAULT_LIMIT, 0.0);
	if (!SetLtcCommandL(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 2, 0.0))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 2, I2C_CMD_VOUT_OV_FAULT_LIMIT, 0.0);

	if (!SetLtcCommandL(PCI_SEL(0), I2C_CMD_VOUT_OV_FAULT_LIMIT, 3, 0.0))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 3, I2C_CMD_VOUT_OV_FAULT_LIMIT, 0.0);
	if (!SetLtcCommandL(PCI_SEL(1), I2C_CMD_VOUT_OV_FAULT_LIMIT, 3, 0.0))
		printe("=== Error(ch%d) : I2C_CMD_VOUT_OV_FAULT_LIMIT(0x%X) : Value(%f) ===\n", 3, I2C_CMD_VOUT_OV_FAULT_LIMIT, 0.0);

	return true;
}

bool dps::SetLtcUserData0(PCI_SEL pci, uint i2c_num, word data)
{
	return SetLtcCommandR(pci, I2C_CMD_USER_DATA_00, i2c_num, data);
}

uint dps::GetLtcUserData0(PCI_SEL pci, uint i2c_num)
{
	return GetLtcCommandR(pci, I2C_CMD_USER_DATA_00, i2c_num);
}

bool dps::SetLtcUserData2(PCI_SEL pci, uint i2c_num, word data)
{
	return SetLtcCommandR(pci, I2C_CMD_USER_DATA_02, i2c_num, data);
}

uint dps::GetLtcUserData2(PCI_SEL pci, uint i2c_num)
{
	return GetLtcCommandR(pci, I2C_CMD_USER_DATA_02, i2c_num);
}

bool dps::SetLtcUserData4(PCI_SEL pci, uint i2c_num, word data)
{
	return SetLtcCommandR(pci, I2C_CMD_USER_DATA_04, i2c_num, data);
}

uint dps::GetLtcUserData4(PCI_SEL pci, uint i2c_num)
{
	return GetLtcCommandR(pci, I2C_CMD_USER_DATA_04, i2c_num);
}


bool dps::ReadAllreg(int nDps)
{
	std::vector<DPS_PMBUS_COMMAND> vecReadCmd;
#if 0	
	vecReadCmd.push_back(I2C_CMD_PAGE            );
	vecReadCmd.push_back(I2C_CMD_OPERATION       );
	vecReadCmd.push_back(I2C_CMD_ON_OFF_CONFIG   );
	vecReadCmd.push_back(I2C_CMD_CLEAR_FAULTS    );
	vecReadCmd.push_back(I2C_CMD_PAGE_PLUS_WRITE );
	vecReadCmd.push_back(I2C_CMD_PAGE_PLUS_READ  );
	vecReadCmd.push_back(I2C_CMD_WRITE_PROTECT   );
	vecReadCmd.push_back(I2C_CMD_STORE_USER_ALL  );
	vecReadCmd.push_back(I2C_CMD_RESTORE_USER_ALL);
	vecReadCmd.push_back(I2C_CMD_CAPABILITY      );
	vecReadCmd.push_back(I2C_CMD_SMBALERT_MASK   );
	vecReadCmd.push_back(I2C_CMD_VOUT_MODE           );
	vecReadCmd.push_back(I2C_CMD_VOUT_COMMAND        );
	vecReadCmd.push_back(I2C_CMD_VOUT_MAX            );
	vecReadCmd.push_back(I2C_CMD_VOUT_MARGIN_HIGH    );
	vecReadCmd.push_back(I2C_CMD_VOUT_MARGIN_LOW     );
	vecReadCmd.push_back(I2C_CMD_VOUT_TRANSITION_RATE);
	vecReadCmd.push_back(I2C_CMD_FREQUENCY_SWITCH    );
	vecReadCmd.push_back(I2C_CMD_VIN_ON              );
	vecReadCmd.push_back(I2C_CMD_VIN_OFF             );
	vecReadCmd.push_back(I2C_CMD_IOUT_CAL_GAIN);
	vecReadCmd.push_back(I2C_CMD_VOUT_OV_FAULT_LIMIT   );
	vecReadCmd.push_back(I2C_CMD_VOUT_OV_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_VOUT_OV_WARN_LIMIT    );
	vecReadCmd.push_back(I2C_CMD_VOUT_UV_WARN_LIMIT    );
	vecReadCmd.push_back(I2C_CMD_VOUT_UV_FAULT_LIMIT   );
	vecReadCmd.push_back(I2C_CMD_VOUT_UV_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_IOUT_OC_FAULT_LIMIT   );
	vecReadCmd.push_back(I2C_CMD_IOUT_OC_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_IOUT_OC_WARN_LIMIT    );
	vecReadCmd.push_back(I2C_CMD_OT_FAULT_LIMIT   );
	vecReadCmd.push_back(I2C_CMD_OT_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_OT_WARN_LIMIT    );
	vecReadCmd.push_back(I2C_CMD_UT_FAULT_LIMIT   );
	vecReadCmd.push_back(I2C_CMD_UT_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_VIN_OV_FAULT_LIMIT   );
	vecReadCmd.push_back(I2C_CMD_VIN_OV_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_VIN_UV_WARN_LIMIT    );
	vecReadCmd.push_back(I2C_CMD_IIN_OC_WARN_LIMIT    );
	vecReadCmd.push_back(I2C_CMD_TON_DELAY             );
	vecReadCmd.push_back(I2C_CMD_TON_RISE              );
	vecReadCmd.push_back(I2C_CMD_TON_MAX_FAULT_LIMIT   );
	vecReadCmd.push_back(I2C_CMD_TON_MAX_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_TOFF_DELAY            );
	vecReadCmd.push_back(I2C_CMD_TOFF_FALL             );
	vecReadCmd.push_back(I2C_CMD_TOFF_MAX_WARN_LIMIT   );
	vecReadCmd.push_back(I2C_CMD_STATUS_BYTE        );
	vecReadCmd.push_back(I2C_CMD_STATUS_WORD        );
	vecReadCmd.push_back(I2C_CMD_STATUS_VOUT        );
	vecReadCmd.push_back(I2C_CMD_STATUS_IOUT        );
	vecReadCmd.push_back(I2C_CMD_STATUS_INPUT       );
	vecReadCmd.push_back(I2C_CMD_STATUS_TEMPERATURE );
	vecReadCmd.push_back(I2C_CMD_STATUS_CML         );
	//vecReadCmd.push_back(I2C_CMD_STATUS_MFR_SPECIFIC);
	vecReadCmd.push_back(I2C_CMD_READ_VIN      );
	vecReadCmd.push_back(I2C_CMD_READ_IIN      );
	vecReadCmd.push_back(I2C_CMD_READ_VOUT     );
	vecReadCmd.push_back(I2C_CMD_READ_IOUT     );
	vecReadCmd.push_back(I2C_CMD_READ_TEMP_1   );
	vecReadCmd.push_back(I2C_CMD_READ_TEMP_2   );
	vecReadCmd.push_back(I2C_CMD_READ_FREQUENCY);
	vecReadCmd.push_back(I2C_CMD_READ_POUT     );
	vecReadCmd.push_back(I2C_CMD_READ_PIN      );
	//vecReadCmd.push_back(I2C_CMD_REVISION);
	vecReadCmd.push_back(I2C_CMD_MFR_ID        );
	vecReadCmd.push_back(I2C_CMD_MFR_MODEL     );
	vecReadCmd.push_back(I2C_CMD_MFR_VOUT_MAX    );
	vecReadCmd.push_back(I2C_CMD_MFR_PIN_ACCURACY);
	vecReadCmd.push_back(I2C_CMD_USER_DATA_00);
	vecReadCmd.push_back(I2C_CMD_USER_DATA_01);
	vecReadCmd.push_back(I2C_CMD_USER_DATA_02);
	vecReadCmd.push_back(I2C_CMD_USER_DATA_03);
	vecReadCmd.push_back(I2C_CMD_USER_DATA_04);
	vecReadCmd.push_back(I2C_CMD_MFR_INFO             );
	vecReadCmd.push_back(I2C_CMD_MFR_CHAN_CONFIG      );
	vecReadCmd.push_back(I2C_CMD_MFR_CONFIG_ALL       );
	vecReadCmd.push_back(I2C_CMD_MFR_FAULT_PROPAGATE  );
	vecReadCmd.push_back(I2C_CMD_MFR_PWM_COMP         );
	vecReadCmd.push_back(I2C_CMD_MFR_PWM_MODE         );
	vecReadCmd.push_back(I2C_CMD_MFR_FAULT_RESPONSE   );
	vecReadCmd.push_back(I2C_CMD_MFR_OT_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_MFR_IOUT_PEAK        );
	vecReadCmd.push_back(I2C_CMD_MFR_ADC_CONTROL      );
	vecReadCmd.push_back(I2C_CMD_MFR_RETRY_DELAY      );
	vecReadCmd.push_back(I2C_CMD_MFR_RESTART_DELAY    );
	vecReadCmd.push_back(I2C_CMD_MFR_VOUT_PEAK        );
	vecReadCmd.push_back(I2C_CMD_MFR_VIN_PEAK         );
	vecReadCmd.push_back(I2C_CMD_MFR_TEMPERATUR_1_PEAK);
	vecReadCmd.push_back(I2C_CMD_MFR_READ_IIN_PEAK    );
	vecReadCmd.push_back(I2C_CMD_MFR_CLEAR_PEAKS      );
	vecReadCmd.push_back(I2C_CMD_MFR_READ_ICHIP       );
	vecReadCmd.push_back(I2C_CMD_MFR_PADS             );
	vecReadCmd.push_back(I2C_CMD_MFR_ADDRESS          );
	vecReadCmd.push_back(I2C_CMD_MFR_SPECIAL_ID       );
	vecReadCmd.push_back(I2C_CMD_MFR_IIN_CAL_GAIN     );
	vecReadCmd.push_back(I2C_CMD_MFR_FAULT_LOG_STORE  );
	vecReadCmd.push_back(I2C_CMD_MFR_FAULT_LOG_CLEAR  );
	vecReadCmd.push_back(I2C_CMD_MFR_FAULT_LOG        );
	vecReadCmd.push_back(I2C_CMD_MFR_COMMON           );
	vecReadCmd.push_back(I2C_CMD_MFR_COMPARE_USER_ALL );
	vecReadCmd.push_back(I2C_CMD_MFR_TEMPERATUR_2_PEAK);
	vecReadCmd.push_back(I2C_CMD_MFR_PWM_CONFIG       );
	vecReadCmd.push_back(I2C_CMD_MFR_IOUT_CAL_GAIN_TC );
	vecReadCmd.push_back(I2C_CMD_MFR_RVIN             );
	vecReadCmd.push_back(I2C_CMD_MFR_TEMP_1_GAIN      );
	vecReadCmd.push_back(I2C_CMD_MFR_TEMP_1_OFFSET    );
	vecReadCmd.push_back(I2C_CMD_MFR_RAIL_ADDRESS     );
	vecReadCmd.push_back(I2C_CMD_MFR_RESET            );
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
	// vecReadCmd.push_back(I2C_CMD_);
#else
	vecReadCmd.push_back(I2C_CMD_MFR_RESET            );

	vecReadCmd.push_back(I2C_CMD_SMBALERT_MASK);
	vecReadCmd.push_back(I2C_CMD_WRITE_PROTECT);
	vecReadCmd.push_back(I2C_CMD_FREQUENCY_SWITCH);
	vecReadCmd.push_back(I2C_CMD_VIN_ON);
	vecReadCmd.push_back(I2C_CMD_VIN_OFF);
	vecReadCmd.push_back(I2C_CMD_VIN_OV_FAULT_LIMIT);
	vecReadCmd.push_back(I2C_CMD_VIN_UV_WARN_LIMIT);
	vecReadCmd.push_back(I2C_CMD_IIN_OC_WARN_LIMIT);
	vecReadCmd.push_back(I2C_CMD_MFR_CONFIG_ALL);
	vecReadCmd.push_back(I2C_CMD_MFR_ADDRESS);
	vecReadCmd.push_back(I2C_CMD_MFR_IIN_CAL_GAIN);
	vecReadCmd.push_back(I2C_CMD_MFR_PWM_CONFIG);
	vecReadCmd.push_back(I2C_CMD_MFR_RVIN);
	vecReadCmd.push_back(I2C_CMD_VOUT_COMMAND);
	vecReadCmd.push_back(I2C_CMD_VOUT_MAX);
	vecReadCmd.push_back(I2C_CMD_VOUT_MARGIN_HIGH);
	vecReadCmd.push_back(I2C_CMD_VOUT_MARGIN_LOW);
	vecReadCmd.push_back(I2C_CMD_VOUT_OV_FAULT_LIMIT);
	vecReadCmd.push_back(I2C_CMD_VOUT_OV_WARN_LIMIT);
	vecReadCmd.push_back(I2C_CMD_VOUT_UV_FAULT_LIMIT);
	vecReadCmd.push_back(I2C_CMD_VOUT_UV_WARN_LIMIT);
	vecReadCmd.push_back(I2C_CMD_IOUT_OC_FAULT_LIMIT);
	vecReadCmd.push_back(I2C_CMD_IOUT_OC_WARN_LIMIT);
	vecReadCmd.push_back(I2C_CMD_OPERATION);
	vecReadCmd.push_back(I2C_CMD_ON_OFF_CONFIG);
	vecReadCmd.push_back(I2C_CMD_VOUT_TRANSITION_RATE);
	vecReadCmd.push_back(I2C_CMD_IOUT_CAL_GAIN);
	vecReadCmd.push_back(I2C_CMD_VOUT_OV_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_VOUT_UV_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_IOUT_OC_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_OT_FAULT_LIMIT);
	vecReadCmd.push_back(I2C_CMD_OT_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_OT_WARN_LIMIT);
	vecReadCmd.push_back(I2C_CMD_UT_FAULT_LIMIT);
	vecReadCmd.push_back(I2C_CMD_UT_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_VIN_OV_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_TON_DELAY);
	vecReadCmd.push_back(I2C_CMD_TON_RISE);
	vecReadCmd.push_back(I2C_CMD_TON_MAX_FAULT_LIMIT);
	vecReadCmd.push_back(I2C_CMD_TON_MAX_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_TOFF_DELAY);
	vecReadCmd.push_back(I2C_CMD_TOFF_FALL);
	vecReadCmd.push_back(I2C_CMD_TOFF_MAX_WARN_LIMIT);

	vecReadCmd.push_back(I2C_CMD_MFR_CHAN_CONFIG);
	vecReadCmd.push_back(I2C_CMD_MFR_FAULT_PROPAGATE);
	vecReadCmd.push_back(I2C_CMD_MFR_PWM_COMP);
	vecReadCmd.push_back(I2C_CMD_MFR_PWM_MODE);
	vecReadCmd.push_back(I2C_CMD_MFR_FAULT_RESPONSE);
	vecReadCmd.push_back(I2C_CMD_MFR_RETRY_DELAY);
	vecReadCmd.push_back(I2C_CMD_MFR_RESTART_DELAY);

	vecReadCmd.push_back(I2C_CMD_MFR_IOUT_CAL_GAIN_TC);
	vecReadCmd.push_back(I2C_CMD_MFR_TEMP_1_GAIN);
	vecReadCmd.push_back(I2C_CMD_MFR_TEMP_1_OFFSET);
	vecReadCmd.push_back(I2C_CMD_MFR_RAIL_ADDRESS);

	vecReadCmd.push_back(I2C_CMD_STATUS_BYTE);
	vecReadCmd.push_back(I2C_CMD_STATUS_WORD);
	vecReadCmd.push_back(I2C_CMD_STATUS_VOUT);
	vecReadCmd.push_back(I2C_CMD_STATUS_IOUT);
	vecReadCmd.push_back(I2C_CMD_STATUS_TEMPERATURE);
	vecReadCmd.push_back(I2C_CMD_STATUS_INPUT);
	vecReadCmd.push_back(I2C_CMD_STATUS_CML);
	vecReadCmd.push_back(I2C_CMD_STATUS_MFR_SPECEFIC);
#endif


	cstring sCmd = "";
	DPS_PMBUS_COMMAND cmd;
	uint16_t uData = 0;
	int type;

	int nPCI = nDps / 10;
	int num = (nDps % 10) / 2;

	for( int nPage = 0; nPage < 2; nPage++) {
		
		SetLtcPage(PCI_SEL(nPCI), num, nPage);
		printf("LTC REG READ pci : %d, device %d , page : %d\n",nPCI, num, nPage);
		int cnt = 0; 
		for (unsigned int idx = 0; idx < vecReadCmd.size(); idx++)
		{
			std::map<DPS_PMBUS_COMMAND, stPMBusCommand>::iterator it = m_mapCmdLtc.find(vecReadCmd[idx]);
			cmd = it->first;
			sCmd = it->second.Command;
			type = it->second.Type;

			uData = I2cRead(PCI_SEL(nPCI), num, cmd, (LTC_PMBUS_COMMAND_TYPE)type);
			printf("Reg cmd %s data 0x%X\n", sCmd.c_str(), uData );
		}
	}

}