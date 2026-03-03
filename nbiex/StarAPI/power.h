#ifndef __POWER_H__
#define __POWER_H__
#pragma once


#include "define.h"
#include "STAR_TEST.h"
#include "String.h"
#include <stdint.h>


enum {
    DPS_S1 = MAX_PWR_ADC_CH, DPS_S2, DPS_S3, DPS_S4,
    DPS_S5, DPS_S6, DPS_S7, DPS_S8,
    DPS_S9, DPS_S10, DPS_S11, DPS_S12,
    DPS_S13, DPS_S14, DPS_S15, DPS_S16,
    DPS_CAL_IV1, DPS_CAL_IV2, DPS_CAL_IV3, DPS_CAL_IV4,
    DPS_CAL_IV5, DPS_CAL_IV6, DPS_CAL_IV7, DPS_CAL_IV8,
    DPS_CAL_IV9, DPS_CAL_IV10, DPS_CAL_IV11, DPS_CAL_IV12,
    DPS_CAL_IV13, DPS_CAL_IV14, DPS_CAL_IV15, DPS_CAL_IV16,
    HVDPS_CAL_IV1, HVDPS_CAL_IV2, HVDPS_CAL_IV3, HVDPS_CAL_IV4,
    DGS1, DGS2, DGS3, DGS4,
    DIV_NAG5, DIV18, DIV12P, DIV12P0,
    DIV12P1, DIV_12CUP, DIV9DOT5, NODIV5,
    V3_3, V2_5, V1_8, V1_2,
    V1_0, V0_9, V0_85, V0_6,
    VREF4V1, VREF4V2, VREF4V3, VREF4V4,
    VREF4V5, VREF4V6, VREF4V7, VREF4V8,
    GND1, GND2, GND3, GND4,
    GND5, GND6, GND7, GND8
};

enum {
	CAL_ADC_BASE = 0, 
	CAL_HV_F, CAL_HV_V, CAL_HV_A, CAL_PWR_F, 
    CAL_DPS_F, CAL_DPS_V, CAL_DPS_A  
};


#pragma pack(push, 1)
typedef struct
{
    
    char  PJT_VER[32];          // 0~31    
    byte  TPCB_REV;             // 32
    byte  SLOT_ID;              // 33    
    byte  BIB_nBD_CHK;          // 34    
    uint  BIB_BID[4];           // 35~38  (uint32 의미라면 변환해서 사용)    
    short ADC_MUX[16][8];      // 39~294    
    word  DPS_RUN;           // 295~296    
    byte  HV_DPS_EN;            // 297
    byte  VREF_EN;              // 298
    byte  DPS_ALERT;            // 299
    byte  DPS_FAULT;            // 300
    word  DPS_STATUS[16];    // 301~332
    word  DPS_VOUT[16];      // 333~364
    word  DPS_IOUT[16];      // 365~396
    short  INA_VOLT[20];      // 397~436
    short  INA_CUR[20];       // 437~476

} allst;
#pragma pack(pop)


typedef struct
{
    int mux;
    int index;
}adcItem;

typedef struct
{
    adcItem arr[128];
}adcItems;

typedef union stPMON {
    struct {
        double sv;  // IO V
        double rv;  // CK V 
        double ov;	// DR V
        double uv;	// SC V
        double oc;
        double rc;
    };


    stPMON() { sv = -1.0; rv = -1.0, ov = -1.0; uv = -1.0; oc = -1.0; rc = -1.0; }

}PMON;

typedef struct
{
    uint64 chBit;
    double delay;
}stPowerSeq;

#define MAX_PMON_CH 20 // -> DPS 16CH + HV 4CH


class powerc{

public:
    powerc();
    ~powerc() {};

    allst m_stAllStat;
    adcItems m_adcMap = {
        .arr = {
			//////// OPERAION POWER ////////
            // VH1~16
            {9,0},  {9,1},  {9,2},  {9,3},
            {9,4},  {9,5},  {9,6},  {9,7},
            {10,0}, {10,1}, {10,2}, {10,3},
            {10,4}, {10,5}, {10,6}, {10,7},                         
            // VTH1~8
            {11,0}, {11,1}, {11,2}, {11,3},
            {11,4}, {11,5}, {11,6}, {11,7},             
            // HV1~4
            {6,4},  {6,5},  {6,6},	{6,7},
            // VREF1~4
            {8,4},  {8,5},  {8,6},  {8,7},

			///////// DIAG POWER ///////////                        
            //DPS_S1~16
            {2,0},  {2,1},  {2,2},  {2,3},
            {2,4},  {2,5},  {2,6},  {2,7},
            {3,0},  {3,1},  {3,2},  {3,3},
			{3,4},  {3,5},  {3,6},  {3,7},
			//DPS_CAL_IV1~16
            {4,0}, {4,1},   {4,2},  {4,3},
            {4,4},  {4,5},  {4,6},  {4,7},
            {5,0},  {5,1},  {5,2},  {5,3},
			{5,4},  {5,5},  {5,6},  {5,7},
			//HVDPS_CAL_IV1~4
		    {7,4},  {7,5},  {7,6},  {7,7},
            //DGS1~4
			{7,0},  { 7,0},  {7,1},  {7,2},  {7,3},
            //DIV_NAG5 //DIV18      //DIV12P    //DIV12P0
            {12,0},     {12,1},     {12,2},     {12,3},
            //DIV12P1   //DIV_12CUP //DIV9DOT5  //NODIV5
            {12,4},     {12,5},     {12,6},     {12,7},
            //V3_3      //V2_5      //V1_8      //V1_2
            {13,0},     {13,1},     {13,2},     {13,3},
            //V1_0      //V0_9      //V0_85     //V0_6
            {13,4},     {13,5},     {13,6},     {13,7},
		    //REF4V1~8
			{14,0}, {14,1}, {14,2}, {14,3},
			{14,4}, {14,5}, {14,6}, {14,7},
		    //GND1~8
		    {15,0}, {15,1}, {15,2}, {15,3},
			{15,4}, {15,5}, {15,6}, {15,7}
        }
    };

	PMON m_PMonArr[MAX_PMON_CH]; // DPS 16CH + HV 4CH
    vector<stPowerSeq> m_PSeq;
    double m_VMonArr[3]; // VH, VTH, VREF

    //////////////////////////
	// All Status Memory Read
    //////////////////////////
    void  readAll();
    short readAdc(int mux, int index, bool isDirect = false);
    byte  readDpsRun(bool isDirect = false);
	byte  readHvRun(bool isDirect = false);
	byte  readVrefRun(bool isDirect = false);
	byte  readDpsAlert(bool isDirect = false);  
	byte  readDpsFault(bool isDirect = false);
	word  readDpsStatus(int ch, bool isDirect = false);
	word  readDpsVolt(int ch, bool isDirect = false);
	word  readDpsCurrent(int ch, bool isDirect = false);
    short readInaVolt(int ch, bool isDirect = false);
    short readInaCurrent(int ch, bool isDirect = false);

    /////////////////////////
    // Power Control
    /////////////////////////
    
    void setEnableHv(byte onoff, bool isOn = true);    
    void setEnableVref(byte onoff, bool isOn = true);
	void setEnableDac(int type, bool isOn = true); // type 0 = vh, 1 = vth
    void setDac(int ch, double val);
	
    double getAdc(int ch, bool isDirect = false);
    double getInaVolt(int ch, bool isDirect = false);
    double getInaCur(int ch, bool isDirect = false); 


    //////////////////////////
    // Calibration Data
    // 0 = gain, 1 = offset    
    //////////////////////////
    //     
    // ADC Base
    double m_fCalADCBase[8][2];
    
    // HVDPS
    double m_fCalHvF[4][2];
    double m_fCalHvV[4][2];
    double m_fCalHvA[4][2];

    // Etc - VH16, VTH8, VREF4 
    double m_fCalPwrF[28][2];    

	double m_fCalDpsF[16][2];  
    double m_fCalDpsV[16][2];
    double m_fCalDpsA[16][2];

    void initCalData();
	void loadCalFile();
	void saveCalFile();

};  




#endif
