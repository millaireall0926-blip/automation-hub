#include <vector>
#include <map>
#include "string.h"
#include "ioc.h"
#include "Scanner.h"
#include "bib.h"
#include "net.h"
#include "failmem.h"
#include "common.h"
#include "STAR_HSBI.h"
#include "DIAG_HSBI.h"
#include "operation.h"
#include "dps.h"


extern failmem		gFM;
extern ioc			gIO;
extern net			gNET;
extern common		gCOM;
extern operation	gOPR;  // Operation
extern dps			gDPS;  // dps
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

int diag_readFmData(int nPG)
{ 
	return gFM.readFmData(nPG);
}

int diag_readFmData2(int nPG, uint adr, int size)
{ 
	byte *pData = new byte[size];
	int pci = (nPG >> 1);
	int pg = (nPG & 1);
	gIO.readPGData(PCI_SEL(pci), pg, adr, pData, size);
}

DIAG_FM* diag_getFmDataH(int nPG, int idx)
{
	return (DIAG_FM*)gFM.getFmDataH(nPG, idx);
}

DIAG_FM* diag_getFmDataL(int nPG, int idx)
{
	return (DIAG_FM*)gFM.getFmDataL(nPG, idx);
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

void diag_showDrv(int nPG, int ofs, int size)
{
	gFM.showFmDrv(nPG, ofs, size);
}

void diag_showData(int nPG, int ofs, int size)
{
	gFM.showFmData(nPG, ofs, size);
}

void diag_readAdc(int nCtrl, int mux, short *pData)
{
	PCI_SEL pci = nCtrl == 0 ? PCI_SEL::PCI1 : PCI_SEL::PCI2;
	gIO.adc_read(pci, mux, pData);
}

void diag_writeDac(int nCtrl, int ch, uint16_t wdata)
{
	PCI_SEL pci = nCtrl == 0 ? PCI_SEL::PCI1 : PCI_SEL::PCI2;
	gIO.dac_write(pci, DAC_REG_SEL::INPUT, ch, wdata);
}

uint8_t diag_readInuse()
{
	return gIO.readInuse();
}

int diag_getMode()
{
	return gCOM.m_nDiagTestSel;
}

void setAdcCalData(std::vector< stCalFactor >* calData)
{
	for (int nAdc = 0; nAdc < MAX_ADC_NUM; nAdc++) {
		for (int nCh = 0; nCh < MAX_ADC_CH; nCh++) {
			gOPR.m_AdcCal[nAdc][nCh] = calData[nAdc][nCh];
			//printf("set ADC %d cal data : %0.4f, %0.4f, %0.4f\n", nCh ,gOPR.m_AdcCal[nAdc][nCh].gain, gOPR.m_AdcCal[nAdc][nCh].offset, gOPR.m_AdcCal[nAdc][nCh].offset2 );
		}
	}
}

void setDacCalData(std::vector< stCalFactor >* calData)
{
	for (int nCtrl = 0; nCtrl < MAX_DAC_NUM; nCtrl++) {
		for (int nCh = 0; nCh < MAX_DAC_CH; nCh++) {
			gOPR.m_DacCal[nCtrl][nCh].gain = calData[nCtrl][nCh].gain;
			gOPR.m_DacCal[nCtrl][nCh].offset = calData[nCtrl][nCh].offset;
			gOPR.m_DacCal[nCtrl][nCh].offset2 = calData[nCtrl][nCh].offset2;
			//printf("set DAC %d cal data : %0.4f, %0.4f, %0.4f\n", nCh ,gOPR.m_DacCal[nCtrl][nCh].gain, gOPR.m_DacCal[nCtrl][nCh].offset, gOPR.m_DacCal[nCtrl][nCh].offset2 );
		}
	}
}

void setDpsCalData(std::vector< stCalFactor >& vsCalData, std::vector< stCalFactor >& vmCalData, std::vector< stCalFactor >& imCalData)
{
	for (int nCh = 0; nCh < MAX_DPS_CH; nCh++) {
		gDPS.m_VSCal[nCh].gain = vsCalData[nCh].gain;
		gDPS.m_VSCal[nCh].offset = vsCalData[nCh].offset;
		gDPS.m_VSCal[nCh].offset2 = vsCalData[nCh].offset2;

		//printf("set DPS %d VS cal data : %0.4f, %0.4f, %0.4f\n", nCh ,gDPS.m_VSCal[nCh].gain, gDPS.m_VSCal[nCh].offset, gDPS.m_VSCal[nCh].offset2 );

		gDPS.m_VMCal[nCh].gain = vmCalData[nCh].gain;
		gDPS.m_VMCal[nCh].offset = vmCalData[nCh].offset;
		gDPS.m_VMCal[nCh].offset2 = vmCalData[nCh].offset2;

		//printf("set DPS %d VM cal data : %0.4f, %0.4f, %0.4f\n", nCh ,gDPS.m_VMCal[nCh].gain, gDPS.m_VMCal[nCh].offset, gDPS.m_VMCal[nCh].offset2 );

		gDPS.m_IMCal[nCh].gain = imCalData[nCh].gain;
		gDPS.m_IMCal[nCh].offset = imCalData[nCh].offset;
		gDPS.m_IMCal[nCh].offset2 = imCalData[nCh].offset2;

		//printf("set DPS %d IM cal data : %0.4f, %0.4f, %0.4f\n", nCh ,gDPS.m_IMCal[nCh].gain, gDPS.m_IMCal[nCh].offset, gDPS.m_IMCal[nCh].offset2 );
	}
}