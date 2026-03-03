#ifndef __DIAG_TEST__
#define __DIAG_TEST__
#include "STAR_TEST.h"



#define DIAG_SYS        0x1 // system level check
#define DIAG_CAL        0x2 // ADC, DAC, DPS Calibration
#define DIAG_POWER      0x4 //      DAC, DPS Check

#define DIAG_PG         0x10 // PG
#define DIAG_TG         0x20 // TG
#define DIAG_SHMOO      0x40
#define DIAG_TDR        0x80

/*****************************************
        DIAG
******************************************/

void diag_log_open();
void diag_log_close();
void diag_msg(int pf, const char* format, ...);
void diag_log_pf(int pf, const char* format, ...);
void diag_log(const char* format, ...);


int  diag_get_mode();    // darg value all = 0xf

//////////////////////////////////////////////////////////
//    Power Diag
//////////////////////////////////////////////////////////
void diag_read_all();
void diag_set_dac(int ch, double val);
void diag_enable_dps(word onoff, bool isOn = true);
void diag_enable_hv(byte onoff, bool isOn = true);
void diag_enable_vref(byte onoff, bool isOn = true);
void diag_enable_dac(int type, bool isOn = true); // type 0 = vh, 1 = vth
double diag_get_adc(int ch, bool isDirect = false);
double diag_get_ina_volt(int ch, bool isDirect = false);
double diag_get_ina_cur(int ch, bool isDirect = false);

//////////////////////////////////////////////////////////
//    Calibration
//////////////////////////////////////////////////////////
void diag_init_cal();
void diag_load_cal_file();
void diag_save_cal_file();
void diag_set_cal(int type, int index, double gain, double ofs);



#endif // !__DIAG_TEST__


 
