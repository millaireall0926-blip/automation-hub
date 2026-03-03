#include "define.h"
#include "dac.h"
#include "ioc.h"
#include "operation.h"

extern operation	gOPR;
extern ioc  gIO;

dac::dac()
{

}

dac::~dac()
{

}

void dac::dac_reset() {
    printf("==%s==\n", __FUNCTION__);
    gIO.dac_reset(PCI_SEL::PCI1);
    gIO.dac_reset(PCI_SEL::PCI2);
}

void dac::dac_init()
{
    printf("==%s==\n", __FUNCTION__);
	uint16_t code[2] = {0,0};
	uint8_t ch;
	// VIH 1.00
	gOPR.m_DRVIH = 1.0;
	gOPR.m_IOVIH = 1.0;
	gOPR.m_vecDacValue[0][4] = gOPR.m_vecDacValue[0][1] = 4.224/1.989 * gOPR.m_DRVIH;
	gOPR.m_vecDacValue[0][3] = gOPR.m_vecDacValue[0][0] = 4.224/1.962 * gOPR.m_IOVIH;

	gOPR.m_vecDacValue[1][4] = gOPR.m_vecDacValue[1][1] = 4.224/1.989 * gOPR.m_DRVIH;
	gOPR.m_vecDacValue[1][3] = gOPR.m_vecDacValue[1][0] = 4.224/1.962 * gOPR.m_IOVIH;

    	// VTH 0.5
	gOPR.m_VTH = 0.5;
	gOPR.m_vecDacValue[0][5] = gOPR.m_vecDacValue[0][2] = 4 * gOPR.m_VTH;
	gOPR.m_vecDacValue[1][5] = gOPR.m_vecDacValue[1][2] = 4 * gOPR.m_VTH;

	for(ch = 0; ch < 6; ch++) {
		code[0] = ((gOPR.m_vecDacValue[0][ch]*gOPR.m_DacCal[0][ch].gain + gOPR.m_DacCal[0][ch].offset)/5.0)*4095;
		code[1] = ((gOPR.m_vecDacValue[1][ch]*gOPR.m_DacCal[1][ch].gain + gOPR.m_DacCal[1][ch].offset)/5.0)*4095;
		gIO.dac_write(PCI_SEL::PCI1, DAC_REG_SEL::INPUT, ch, code[0]);
		gIO.dac_write(PCI_SEL::PCI1, DAC_REG_SEL::INPUT, ch, code[0]);
		gIO.dac_write(PCI_SEL::PCI2, DAC_REG_SEL::INPUT, ch, code[1]);
		gIO.dac_write(PCI_SEL::PCI2, DAC_REG_SEL::INPUT, ch, code[1]);
	}

    // DVTT 0.5
	gOPR.m_DutDRVTT = 0.5;
	gOPR.m_vecDacValue[0][16] = gOPR.m_DutDRVTT;
	gOPR.m_vecDacValue[0][17] = gOPR.m_DutDRVTT;
	gOPR.m_vecDacValue[0][20] = gOPR.m_DutDRVTT;
	gOPR.m_vecDacValue[0][21] = gOPR.m_DutDRVTT;

	gOPR.m_vecDacValue[1][16] = gOPR.m_DutDRVTT;
	gOPR.m_vecDacValue[1][17] = gOPR.m_DutDRVTT;
	gOPR.m_vecDacValue[1][20] = gOPR.m_DutDRVTT;
	gOPR.m_vecDacValue[1][21] = gOPR.m_DutDRVTT;

	gOPR.m_DutIOVTT = 0.5;
	gOPR.m_vecDacValue[0][18] = gOPR.m_DutIOVTT;
	gOPR.m_vecDacValue[0][19] = gOPR.m_DutIOVTT;
	gOPR.m_vecDacValue[0][22] = gOPR.m_DutIOVTT;
	gOPR.m_vecDacValue[0][23] = gOPR.m_DutIOVTT;

	gOPR.m_vecDacValue[1][18] = gOPR.m_DutIOVTT;
	gOPR.m_vecDacValue[1][19] = gOPR.m_DutIOVTT;
	gOPR.m_vecDacValue[1][22] = gOPR.m_DutIOVTT;
	gOPR.m_vecDacValue[1][23] = gOPR.m_DutIOVTT;

	for(ch = 16; ch < 24; ch++) {
		code[0] = ((gOPR.m_vecDacValue[0][ch]*gOPR.m_DacCal[0][ch].gain + gOPR.m_DacCal[0][ch].offset)/5.0)*4095;
		code[1] = ((gOPR.m_vecDacValue[1][ch]*gOPR.m_DacCal[1][ch].gain + gOPR.m_DacCal[1][ch].offset)/5.0)*4095;
		gIO.dac_write(PCI_SEL::PCI1, DAC_REG_SEL::INPUT, ch, code[0]);
		gIO.dac_write(PCI_SEL::PCI1, DAC_REG_SEL::INPUT, ch, code[0]);
		gIO.dac_write(PCI_SEL::PCI2, DAC_REG_SEL::INPUT, ch, code[1]);
		gIO.dac_write(PCI_SEL::PCI2, DAC_REG_SEL::INPUT, ch, code[1]);
	}

	// VTT 0.5
	gOPR.m_DRVTT = 0.5;
	gOPR.m_vecDacValue[0][8] = gOPR.m_DRVTT;
	gOPR.m_vecDacValue[0][9] = gOPR.m_DRVTT;
	gOPR.m_vecDacValue[0][12] = gOPR.m_DRVTT;
	gOPR.m_vecDacValue[0][13] = gOPR.m_DRVTT;

	gOPR.m_vecDacValue[1][8] = gOPR.m_DRVTT;
	gOPR.m_vecDacValue[1][9] = gOPR.m_DRVTT;
	gOPR.m_vecDacValue[1][12] = gOPR.m_DRVTT;
	gOPR.m_vecDacValue[1][13] = gOPR.m_DRVTT;

	gOPR.m_IOVTT = 0.5;
	gOPR.m_vecDacValue[0][10] = gOPR.m_IOVTT;
	gOPR.m_vecDacValue[0][11] = gOPR.m_IOVTT;
	gOPR.m_vecDacValue[0][14] = gOPR.m_IOVTT;
	gOPR.m_vecDacValue[0][15] = gOPR.m_IOVTT;

	gOPR.m_vecDacValue[1][10] = gOPR.m_IOVTT;
	gOPR.m_vecDacValue[1][11] = gOPR.m_IOVTT;
	gOPR.m_vecDacValue[1][14] = gOPR.m_IOVTT;
	gOPR.m_vecDacValue[1][15] = gOPR.m_IOVTT;

	for(ch = 8; ch < 16; ch++) {
		code[0] = ((gOPR.m_vecDacValue[0][ch]*gOPR.m_DacCal[0][ch].gain + gOPR.m_DacCal[0][ch].offset)/5.0)*4095;
		code[1] = ((gOPR.m_vecDacValue[1][ch]*gOPR.m_DacCal[1][ch].gain + gOPR.m_DacCal[1][ch].offset)/5.0)*4095;
		gIO.dac_write(PCI_SEL::PCI1, DAC_REG_SEL::INPUT, ch, code[0]);
		gIO.dac_write(PCI_SEL::PCI1, DAC_REG_SEL::INPUT, ch, code[0]);
		gIO.dac_write(PCI_SEL::PCI2, DAC_REG_SEL::INPUT, ch, code[1]);
		gIO.dac_write(PCI_SEL::PCI2, DAC_REG_SEL::INPUT, ch, code[1]);
	}

}

void dac::dac_vref_en(bool en)
{
    //printf("==%s==\n", __FUNCTION__);
    gIO.dac_vref_en(PCI_SEL::PCI1, en);
    gIO.dac_vref_en(PCI_SEL::PCI2, en);
}

void dac::dac_vtt_bias_en(bool en)
{
    //printf("==%s==\n", __FUNCTION__);
    gIO.dac_vtt_bias_en(PCI_SEL::PCI1, en);
    gIO.dac_vtt_bias_en(PCI_SEL::PCI2, en);
}

void dac::dac_pps_12_en(bool en)
{
    printf("==%s==\n", __FUNCTION__);
    gIO.dac_pps_12_en(PCI_SEL::PCI1, en);
    gIO.dac_pps_12_en(PCI_SEL::PCI2, en);
}
