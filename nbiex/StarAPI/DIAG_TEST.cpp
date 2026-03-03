
#include "DIAG_TEST.h"
#include "define.h"
#include "dps.h"
#include "ioc.h"
#include "bib.h"
#include "result.h"
#include "starAPI.h"
#include "net.h"
#include "errc.h"
#include "power.h"

#define DR_SEL 0x2A80AA
#define IO_SEL 0x154055
#define SCAN_SEL 0x1D00
#define CLK_SEL 0x2200


extern ioc			gIO;		//	gIO
extern starAPI		gAPI;	//	gAPI
extern starlib		gLIB;	//  gLIB
extern net			gNET;	//	gNET
extern errctrl		gERR;	//	gERR
extern bib			gBIB;	//  gBIB
extern result		gRES;	//	gRES
extern dps			gDPS;
extern powerc		gPWR;


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

int diag_get_mode()
{
	return gAPI.m_nDiagTestSel;
}

void diag_set_dac(int ch, double val)
{
	gPWR.setDac(ch, val);
}

void diag_enable_dps(word onoff, bool isOn)
{
	gIO.dps_run(onoff, isOn);
}

void diag_enable_hv(byte onoff, bool isOn)
{
	gPWR.setEnableHv(onoff, isOn);
}

void diag_enable_vref(byte onoff, bool isOn)
{
	gPWR.setEnableVref(onoff, isOn);
}

void diag_enable_dac(int type, bool isOn)
{
	gPWR.setEnableDac(type, isOn);
}

void diag_read_all()
{
	gPWR.readAll();
}

double diag_get_adc(int ch, bool isDirect)
{
	gPWR.getAdc(ch, isDirect);
}

double diag_get_ina_volt(int ch, bool isDirect)
{
	return gPWR.getInaVolt(ch, isDirect);
}

double diag_get_ina_cur(int ch, bool isDirect)
{
	return gPWR.getInaCur(ch, isDirect);
}

void diag_init_cal()
{
	gPWR.initCalData();
}

void diag_load_cal_file()
{
	gPWR.loadCalFile();
}

void diag_save_cal_file()
{
	gPWR.saveCalFile();
}

void diag_set_cal(int type, int index, double gain, double ofs)
{
	if (type == CAL_ADC_BASE)
	{
		gPWR.m_fCalADCBase[index][0] = gain;
		gPWR.m_fCalADCBase[index][1] = ofs;
	}else if (type == CAL_HV_F)
	{
		gPWR.m_fCalHvF[index][0] = gain;
		gPWR.m_fCalHvF[index][1] = ofs;
	}
	else if (type == CAL_HV_V)
	{
		gPWR.m_fCalHvV[index][0] = gain;
		gPWR.m_fCalHvV[index][1] = ofs;
	}
	else if (type == CAL_HV_A)
	{
		gPWR.m_fCalHvA[index][0] = gain;
		gPWR.m_fCalHvA[index][1] = ofs;
	}
	else if (type == CAL_PWR_F)
	{
		gPWR.m_fCalPwrF[index][0] = gain;
		gPWR.m_fCalPwrF[index][1] = ofs;
	}
	else if (type == CAL_DPS_F)
	{
		gPWR.m_fCalDpsF[index][0] = gain;
		gPWR.m_fCalDpsF[index][1] = ofs;
	}
	else if (type == CAL_DPS_V)
	{
		gPWR.m_fCalDpsV[index][0] = gain;
		gPWR.m_fCalDpsV[index][1] = ofs;
	}
	else if (type == CAL_DPS_A)
	{
		gPWR.m_fCalDpsA[index][0] = gain;
		gPWR.m_fCalDpsA[index][1] = ofs;
	}
	
}