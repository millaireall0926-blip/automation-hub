#ifndef __DIAG_TEST__
#define __DIAG_TEST__


#define DIAG_SYS        0x1 // system level check
#define DIAG_CAL        0x2 // ADC, DAC, DPS Calibration
#define DIAG_POWER      0x4 //      DAC, DPS Check

#define DIAG_PG         0x10 // PG
#define DIAG_TG         0x20 // TG
#define DIAG_SHMOO      0x40
#define DIAG_TDR        0x80



int diag_readFmData(int nPG);
int diag_readFmData2(int nPG, uint adr, int size);
DIAG_FM* diag_getFmDataH(int nPG, int idx);
DIAG_FM* diag_getFmDataL(int nPG, int idx);
void diag_clearFmMemory();

void diag_showDrv(int nPG, int ofs, int size);
void diag_showData(int nPG, int ofs, int size);

void diag_readAdc(int nCtrl, int mux, short *pData);

void diag_writeDac(int nCtrl, int ch, uint16_t wdata);

uint8_t diag_readInuse();

/*****************************************
        DIAG
******************************************/

void diag_log_open();
void diag_log_close();
void diag_msg(int pf, const char* format, ...);
void diag_log_pf(int pf, const char* format, ...);
void diag_log(const char* format, ...);

int diag_getMode();    // darg value all = 0xf

void setAdcCalData(std::vector< stCalFactor > * calData);
void setDacCalData(std::vector< stCalFactor > * calData);
void setDpsCalData(std::vector< stCalFactor > & vsCalData, std::vector< stCalFactor > & vmCalData, std::vector< stCalFactor > & imCalData );


#endif // !__DIAG_TEST__