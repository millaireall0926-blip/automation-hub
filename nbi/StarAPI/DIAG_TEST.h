#ifndef __DIAG_TEST__
#define __DIAG_TEST__
#include "STAR_TEST.h"


#define DIAG_SYS        1
#define DIAG_PG         2
#define DIAG_POWER      4
#define DIAG_SHMOO      0x10
#define DIAG_TDR        0x20

#define DIAG_PE_CAL     0x100
#define DIAG_DPS_CAL    0x200
#define DIAG_TEST_MODE  0x8000


/*****************************************
        FM 
******************************************/

typedef struct {
    byte ioComp[8];     // 8byte
    byte ioDrv[2];      // 2byte
    byte adrDrv[5];     // 5byte
    byte scan;          // 1byte
}DIAG_FM;

int diag_readFmData();
DIAG_FM* diag_getFmDataH(int idx);
DIAG_FM* diag_getFmDataL(int idx);
void diag_clearFmMemory();

void diag_showDrv(int ofs, int size);
void diag_showData(int ofs, int size);

void diag_fmDrvMode(int mode);

/*****************************************
		PE CAL
******************************************/
void diag_peWrite(uint32_t dev_sel, uint8_t ch, uint8_t addr, uint16_t wdata);
void diag_peRead(uint32_t nChip, uint8_t nCh, uint8_t addr, uint16_t * wdata);
void diag_peReadAdc(uint8_t dev_no, int idx, short * pData);
uint32_t diag_romWrite(uint16_t addr, int32_t length, uint8_t* data);
uint32_t diag_romRead(uint16_t addr, int32_t length, uint8_t* data);
void diag_peSetChDiag(uint8_t ch);
void diag_peSetDiagOff();
void diag_VIH(uint32_t dev_sel, uint8_t ch_sel, double value);
void diag_VIL(uint32_t dev_sel, uint8_t ch_sel, double value);
void diag_VIH(int type, double value);
void diag_VIL(int type, double value);
uint8_t diag_DAC(int ch, double volt);
void diag_DAC_CLAMP(bool bOn);
float diag_peTempRead(uint16_t ch);
/*****************************************
		DPS CAL
******************************************/
void diag_DPS(int type, double level, double under, double over, double limit);
void diag_DpsRun(uint8_t dev_sel, bool bOn);

uint32_t diag_pe_alarm_read();
uint16_t diag_pe_alarm_read(uint32_t sel);
void diag_pe_off();
/*****************************************
		HW Required Debug Function
******************************************/
uint diag_GetLtcCommandR(uint cmd, uint ch);
uint diag_GetLtcMfrPads(uint ch);
uint diag_GetLtcStatusVout(uint ch);
uint diag_GetLtcStatusIout(uint ch);
uint diag_GetLtcStatusInput(uint ch);
bool diag_SetUserData0(uint ch, word data);
uint diag_GetUserData0(uint ch);
bool diag_SetUserData2(uint ch, word data);
uint diag_GetUserData2(uint ch);
bool diag_SetUserData4(uint ch, word data);
uint diag_GetUserData4(uint ch);

/*****************************************
        ETC Board Temp Unit 
******************************************/
bool  diag_select_temp_mux();
float diag_bd_temp(int ch);

/*****************************************
        DIAG
******************************************/

void diag_log_open();
void diag_log_close();
void diag_msg(int pf, const char* format, ...);
void diag_log_pf(int pf, const char* format, ...);
void diag_log(const char* format, ...);


int diag_getMode();    // darg value all = 0xf

#endif // !__DIAG_TEST__


 
