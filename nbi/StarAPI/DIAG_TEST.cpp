
#include "DIAG_TEST.h"

#include "define.h"
#include "../starlib/starlib_dif.h"
#include "dps.h"
#include "ioc.h"
#include "bib.h"
#include "result.h"
#include "pgc.h"
#include "tgc.h"
#include "starAPI.h"
#include "net.h"
#include "errc.h"
#include "failmem.h"
#include "operation.h"
#include "eeprom.h"
#include "eapi/ec.h"


#include "bitcharts.h"

#define DR_SEL 0x2A80AA
#define IO_SEL 0x154055
#define SCAN_SEL 0x1D00
#define CLK_SEL 0x2200


extern ioc			g_clIO;		//	gIO
extern starAPI		g_clAPI;	//	gAPI
extern starlib		g_clLib;	//  gLIB
extern net			g_clNet;	//	gNET
extern errc			g_clErr;	//	gERR
extern bib			g_clBib;	//  gBIB
extern tgc			g_clTgc;	//	gTG
extern pgc			g_clPgc;	//	gPG
extern result		g_clRes;	//	gRES
extern failmem		g_clFm;		//	gFM
extern operation	g_clOperation;		//  gOPER
extern eeprom		g_clRom;	
extern dps			g_clDPS;

/*****************************************
		FM
******************************************/


int muxidx[22] = {2,3,4,5, 2,3,4,5, 2,3,4, 2,3,4,5, 2,3,4,5, 2,3,4};
int adcch[22] = {0,0,0,0, 1,1,1,1, 2,2,2, 3,3,3,3, 4,4,4,4, 5,5,5};
int diag_readFmData()
{ 
	return gFM.readFmData();
}

DIAG_FM* diag_getFmDataH(int idx)
{
	return (DIAG_FM*)gFM.getFmDataH(idx);
}

DIAG_FM* diag_getFmDataL(int idx)
{
	return (DIAG_FM*)gFM.getFmDataL(idx);
}

void diag_clearFmMemory()
{
	gFM.clearFmMemory();
}

void diag_showDrv(int ofs, int size)
{
	gFM.showFmDrv(ofs, size);
}
void diag_showData(int ofs, int size)
{
	gFM.showFmData(ofs, size);
}

void diag_fmDrvMode(int mode)
{
	gFM.setFmDrvMode(mode);
}

/*****************************************
		PE CAL
******************************************/
void diag_peWrite(uint32_t dev_sel, uint8_t ch, uint8_t addr, uint16_t wdata)
{
	gIO.pe_write(dev_sel, ch, addr, wdata);
}

void diag_peRead(uint32_t nChip, uint8_t nCh, uint8_t addr, uint16_t * wdata)
{
	uint32_t dev_sel = (0x1 << nChip);
	uint8_t pe_ch = 0x1<<nCh;
	gIO.pe_read(dev_sel, pe_ch, addr, wdata);
}

void diag_peReadAdc(uint8_t dev_no, int idx, short * pData)
{
	gIO.adc_read(dev_no, idx, pData);
}

uint32_t diag_romWrite(uint16_t addr, int32_t length, uint8_t* data)
{
	return gROM.write_eeprom(BUSID_EC_I2C, TCB_ROM, addr, length, data );
}

uint32_t diag_romRead(uint16_t addr, int32_t length, uint8_t* data)
{
	return gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, length, data );
}

void diag_peSetChDiag(uint8_t ch)
{
	gIO.pe_write(0x3FFFFF, 0, PE_C_DIAG, (1 << 15) );
	usleep(1000);
	gIO.pe_write(0x3FFFFF, 0xFF, PE_CH_CON_DIAG, 0);
	usleep(1000);
	gIO.pe_write(0x3FFFFF, (0x1 << ch), PE_CH_CON_DIAG, (0x1<<1) ); // Con-F/S-Int-EF
	usleep(1000);
}

void diag_peSetDiagOff()
{
	gIO.pe_write(0x3FFFFF, 0, PE_C_DIAG, 0);
	gIO.pe_write(0x3FFFFF, 0xFF, PE_CH_CON_DIAG, 0);

	gIO.pe_write(0x3FFFFF, 0xFF, PE_CH_SEL_DIAG_0, 0);
	gIO.pe_write(0x3FFFFF, 0xFF, PE_CH_SEL_DIAG_1, 0);
	usleep(1000);
}

void diag_VIH(uint32_t dev_sel, uint8_t ch_sel, double value)
{
	gIO.pe_set_dvh(dev_sel, ch_sel, value);
}

void diag_VIL(uint32_t dev_sel, uint8_t ch_sel, double value)
{
	gIO.pe_set_dvl(dev_sel, ch_sel, value);
}

void diag_VIH(int type, double value)
{
	uint32_t dev_sel = 0x3FFFFF;
	uint8_t ch_sel = 0xFF; // all channels
	
	if(type == DR) 
		dev_sel = DR_SEL;
	else if(type == IO)
		dev_sel = IO_SEL;
	else if(type == SCAN)
		dev_sel = SCAN_SEL;
	else if(type == CLK)
		dev_sel = CLK_SEL;
	else if(type == ALL)
		dev_sel = 0x3FFFFF;
	else
		return;

	gIO.pe_set_dvh(dev_sel, ch_sel, value);
}

void diag_VIL(int type, double value)
{
	uint32_t dev_sel = 0x3FFFFF;
	uint8_t ch_sel = 0xFF; // all channels
	
	if(type == DR) 
		dev_sel = DR_SEL;
	else if(type == IO)
		dev_sel = IO_SEL;
	else if(type == SCAN)
		dev_sel = SCAN_SEL;
	else if(type == CLK)
		dev_sel = CLK_SEL;
	else if(type == ALL)
		dev_sel = 0x3FFFFF;
	else
		return;

	gIO.pe_set_dvl(dev_sel, ch_sel, value);
}

uint8_t diag_DAC(int ch, double volt)
{
	double setVolt = 0;
	if(ch == 2)// VS3
		setVolt = (volt == 0.0 ? 2.366 : (2.366 - ((2.18*volt)/20.0)));
	else if (ch == 5)
		setVolt = volt / 2.0;
	else
		setVolt = volt / 4.0;

	uint16_t dacCode = 0;
    dacCode = (uint16_t)(setVolt / (DAC_SPAN / DAC_RESOLUTION)) & 0xFFF;
	printf("\t DAC.Set : %s[ %01X ] %s[ %6.3f ] %s[ %03X ]\n", "ch", ch, "volt", volt, "dacCode", dacCode);
	return gIO.dac_write(DAC_C_WO_SINREG, ch, dacCode);
}

void diag_DAC_CLAMP(bool bOn)
{
	gIO.dac_clamp(bOn);
}

//// ES Diag Read
/*
float diag_peTempRead(uint16_t ch)
{
	int ndev = ch / 8;
	int dev_sel = (0x1 << ndev);
	int ch_sel = 0x1 << (ch % 8);
	int mux = muxidx[ndev];
	int nadc = adcch[ndev];
	uint16_t read;

	diag_peSetChDiag(ch_sel);

	//Con-Int-ES-ES
	gIO.pe_write(0x3FFFFF, 0, PE_C_DIAG, (1 << 14) );
	usleep(1000);

	//Con-AD-EF-Int-ES
	gIO.pe_read(dev_sel, ch_sel, PE_CH_CON_DIAG,   &read );//INT-ES
	read |= 0x40;
	gIO.pe_write(dev_sel, ch_sel, PE_CH_CON_DIAG,   read );//INT-EF

	//Sel-An-Diag-ES
	gIO.pe_read(dev_sel, ch_sel, PE_CH_SEL_DIAG_1, &read );
	read &= 0xFF0F;
	gIO.pe_write(dev_sel, ch_sel, PE_CH_SEL_DIAG_1, read );
	
	//Sel-Power-Diag
	gIO.pe_read(dev_sel, ch_sel, PE_CH_SEL_DIAG_0, &read );
	read &= 0xF0FF;
	read |= 0x0A00;
	gIO.pe_write(dev_sel, ch_sel, PE_CH_SEL_DIAG_0, read );
	
	// adc read
	short data[8];
	gIO.adc_read(0, mux, data);
	float volt = (data[nadc] * 10.0 / 65535.0) * 2;
	float temp = (volt - 2.08) * (-1/0.0056) + 24;

	diag_peSetDiagOff();
	return temp;
}
*/


float diag_peTempRead(uint16_t ch)
{
	int ndev = ch / 8;
	int dev_sel = (0x1 << ndev);
	int ch_sel = 0x1 << (ch % 8);
	int mux = muxidx[ndev];
	int nadc = adcch[ndev];
	uint16_t read;

	diag_peSetChDiag(ch_sel);

	//Con-AD-EF-Int-EF
	gIO.pe_read(dev_sel, ch_sel, PE_CH_CON_DIAG,   &read );//INT-EF
	read |= 0x80;
	gIO.pe_write(dev_sel, ch_sel, PE_CH_CON_DIAG,   read );//INT-EF

	//Sel-An-Diag-EF
	gIO.pe_write(dev_sel, ch_sel, PE_CH_SEL_DIAG_1, 0 ); // ES & EF : Power Diag Path 설정 

	//Sel-Power-Diag
	gIO.pe_read(dev_sel, ch_sel, PE_CH_SEL_DIAG_0, &read );
	read &= 0xF0FF;
	read |= 0x0A00;
	gIO.pe_write(dev_sel, ch_sel, PE_CH_SEL_DIAG_0, read );

	// adc read
	short data[8];
	gIO.adc_read(0, mux, data);
	float volt = (data[nadc] * 10.0 / 65535.0) * 2;
	float temp = (volt - 2.08) * (-1/0.0056) + 24;

	diag_peSetDiagOff();
	return temp;
}

/*****************************************
		DPS CAL
******************************************/
void diag_DPS(int type, double level, double under, double over, double limit)
{
	if(type == 4) gDPS.SetHVout(level);
	else gDPS.SetVout(type, level, under, over, limit);

	gOPER.m_PMonArr[type].sv = level;
	gOPER.m_PMonArr[type].uv = under;
	gOPER.m_PMonArr[type].ov = over;
	gOPER.m_PMonArr[type].oc = limit;
}

void diag_DpsRun(uint8_t dev_sel, bool bOn)
{
	gIO.dps_run(dev_sel,  bOn ? 1 : 0 );
	if(dev_sel >> 4) gIO.dps_vs3f(bOn ? 1 : 0 );
}


uint32_t diag_pe_alarm_read()
{
	return gIO.pe_alarm_read();
}

uint16_t diag_pe_alarm_read(uint32_t sel)
{
	return gIO.pe_alarm_read(sel);
}

void diag_pe_off()
{
	gIO.pe_channel_off();
	gIO.pe_chip_off();
}

/*****************************************
		HW Required Debug Function
******************************************/
uint diag_GetLtcCommandR(uint cmd, uint ch)
{
	if (ch > 3) return false;
	
	uint i2c_num[4] = {0, 2, 4, 5};
	return gDPS.GetLtcCommandR((DPS_PMBUS_COMMAND)cmd, i2c_num[ch]);
}

uint diag_GetLtcMfrPads(uint ch)
{
	if (ch > 3) return false;

	uint i2c_num[4] = {0, 2, 4, 5};
	return gDPS.GetLtcMfrPads(i2c_num[ch]);
}

uint diag_GetLtcStatusVout(uint ch)
{
	if (ch > 3) return false;

	uint i2c_num[4] = {0, 2, 4, 5};
	return gDPS.GetLtcStatusVout(DPS_SINGLE_SEL, i2c_num[ch]);
}

uint diag_GetLtcStatusIout(uint ch)
{
	if (ch > 3) return false;

	uint i2c_num[4] = {0, 2, 4, 5};
	return gDPS.GetLtcStatusIout(DPS_SINGLE_SEL, i2c_num[ch]);
}

uint diag_GetLtcStatusInput(uint ch)
{
	if (ch > 3) return false;

	uint i2c_num[4] = {0, 2, 4, 5};
	return gDPS.GetLtcStatusInput(DPS_SINGLE_SEL, i2c_num[ch]);
}

bool diag_SetUserData0(uint ch, word data)
{
	if (ch > 3) return false;

	uint i2c_num[4] = {0, 2, 4, 5};
	return gDPS.SetLtcUserData0(i2c_num[ch], data);
}

uint diag_GetUserData0(uint ch)
{
	if (ch > 3) return false;

	uint i2c_num[4] = {0, 2, 4, 5};
	return gDPS.GetLtcUserData0(i2c_num[ch]);
}

bool diag_SetUserData2(uint ch, word data)
{
	if (ch > 3) return false;

	uint i2c_num[4] = {0, 2, 4, 5};
	return gDPS.SetLtcUserData2(i2c_num[ch], data);
}

uint diag_GetUserData2(uint ch)
{
	if (ch > 3) return false;

	uint i2c_num[4] = {0, 2, 4, 5};
	return gDPS.GetLtcUserData2(i2c_num[ch]);
}

bool diag_SetUserData4(uint ch, word data)
{
	if (ch > 3) return false;

	uint i2c_num[4] = {0, 2, 4, 5};
	return gDPS.SetLtcUserData4(i2c_num[ch], data);
}

uint diag_GetUserData4(uint ch)
{
	if (ch > 3) return false;

	uint i2c_num[4] = {0, 2, 4, 5};
	return gDPS.GetLtcUserData4(i2c_num[ch]);
}

/*****************************************
		ETC Board Temp Unit
******************************************/

bool diag_select_temp_mux()
{
	return gROM.select_tempMux(0xf); // 0~3 all Select
}

float diag_bd_temp(int ch)
{
	return gROM.read_temp102(ch, true);
}

/*****************************************
		DIAG
******************************************/
FILE* g_fDiagLog = NULL;

void diag_log_open()
{
	g_fDiagLog = fopen("/star/diag/log/diag.log", "wt");
}

void diag_log_close()
{
	if(g_fDiagLog)	fclose(g_fDiagLog);
	g_fDiagLog = NULL;
}

void diag_log(const char* format, ...)
{
	char sMsg[4096];
	//int MsgLen;

	va_list ap;
	va_start(ap, format);
	vsprintf(sMsg, format, ap);
	va_end(ap);
	if (g_fDiagLog != NULL)fprintf(g_fDiagLog, "%s", sMsg);
	printf("%s", sMsg);
}


void diag_log_pf(int pf, const char* format, ...)
{
	char sMsg[4096];
	//int MsgLen;

	va_list ap;
	va_start(ap, format);
	vsprintf(sMsg, format, ap);
	va_end(ap);

	char buf[128];
	sprintf(buf, " ------- [%s]", pf ? "PASS " : "FAIL*");
	strcat(sMsg, buf);	
	if (g_fDiagLog != NULL)fprintf(g_fDiagLog, "%s\n", sMsg);
	printf("%s\n", sMsg);
}

void diag_msg(int pf, const char* format, ...)
{
	char sMsg[4096];
	//int MsgLen;

	va_list ap;
	va_start(ap, format);
	vsprintf(sMsg, format, ap);
	va_end(ap);

	char buf[128];
	sprintf(buf, "---[%s]", pf ? "PASS " : "FAIL*");
	strcat(sMsg, buf); 
	
	int type = MSG_TYPE_INFO;
	if (pf == 0) type = MSG_TYPE_ERR;
	gNET.send_msg(type, sMsg);

	if (g_fDiagLog != NULL)fprintf(g_fDiagLog, "%s\n", sMsg);
	printf("%s\n", sMsg);
}

int diag_getMode()
{
	return gAPI.m_nDiagTestSel;
}

