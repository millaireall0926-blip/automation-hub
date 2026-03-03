#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "String.h"
#include "STAR_TEST.h"
#include "DIAG_TEST.h"


#define DIAG_BUILD_DATE		"2026-01-25"
#define DIAG_VER		    "Ver-EX(1.0.0)"
/****************************************************************************************************************************
*****************************************************************************************************************************
#define DIAG_BUILD_DATE		"2026-01-25"
#define DIAG_VER		    "Ver-EX(1.0.0)"
 : Board Temp Feature Add

*****************************************************************************************************************************
*****************************************************************************************************************************/
/****************************************************************************************************************************
    diag -diag -darg=f 
          -diag : diag option excute
          -darg : 0x1=SYS, 0x2=PG, 0x4=Power, 0x8=Cal, 0x10=Shmoo 0x20=TDR 0xff=ALL
*****************************************************************************************************************************/


/********************************************************
    ADC TYPE Arguemnt Define
*********************************************************/
enum {   
    DPS_S1 = MAX_PWR_ADC_CH,     DPS_S2,     DPS_S3,     DPS_S4,
    DPS_S5,     DPS_S6,     DPS_S7,     DPS_S8,
    DPS_S9,     DPS_S10,    DPS_S11,    DPS_S12,
    DPS_S13,    DPS_S14,    DPS_S15,    DPS_S16,
	DPS_CAL_IV1, DPS_CAL_IV2, DPS_CAL_IV3, DPS_CAL_IV4,
    DPS_CAL_IV5,    DPS_CAL_IV6,    DPS_CAL_IV7,    DPS_CAL_IV8,
    DPS_CAL_IV9,    DPS_CAL_IV10,   DPS_CAL_IV11,   DPS_CAL_IV12,
    DPS_CAL_IV13,   DPS_CAL_IV14,   DPS_CAL_IV15,   DPS_CAL_IV16,	
	HVDPS_CAL_IV1, HVDPS_CAL_IV2,   HVDPS_CAL_IV3,  HVDPS_CAL_IV4,
	DGS1, DGS2, DGS3, DGS4,
    DIV_NAG5,   DIV18,      DIV12P,     DIV12P0, 
    DIV12P1,    DIV_12CUP,  DIV9DOT5,   NODIV5,
	V3_3,   V2_5,   V1_8,   V1_2, 
	V1_0,   V0_9,   V0_85,  V0_6,
	VREF4V1, VREF4V2, VREF4V3, VREF4V4,
	VREF4V5, VREF4V6, VREF4V7, VREF4V8,
	GND1, GND2, GND3, GND4,
	GND5, GND6, GND7, GND8	
};

/********************************************************
    CAL TYPE Argument Define
*********************************************************/
enum {
	CAL_ADC_BASE = 0, 
	CAL_HV_F, CAL_HV_V, CAL_HV_A, CAL_PWR_F, 
    CAL_DPS_F, CAL_DPS_V, CAL_DPS_A  
};

#define MAX_DPS_CH 16

//////////////////////////////////////////////////////////
//    Calibration
//////////////////////////////////////////////////////////

bool doCalAdc()
{
	double x0, x1, y0, y1, gain, offset;
	double limit = 0.01;
	diag_read_all();
	int stX0 = GND1;
	int stX1 = VREF4V1;
	for (int i = 0; i< 8; i++)
	{
		y0 = 0; 
		y1 = 4.096;
		x0 = diag_get_adc(stX0 + i);
		x1 = diag_get_adc(stX1 + i);
		
		gain = (y1 - y0) / (x1 - x0);
		offset = (-gain * x0) + y0;
		
		if ( fabs(gain - 1.0) > limit){
			diag_log_pf(0, "%s ----- [FAIL] idx:%d, gain:%.3f, ofs:%.3f", __FUNCTION__, i, gain, offset);
            return false;
        }
		
		diag_set_cal(CAL_ADC_BASE, i, gain, offset);
	}
		
	diag_log_pf( 1 , "%s Success", __FUNCTION__);   
	return true;
}

bool doCalHv()
{   
	double x0, x1, y0, y1, gain, offset;
	double limit = 0.1;
	int stX = HV1;	
	for (int i = 0; i< 4; i++)
	{
		y0 = 0;	
		y1 = 4.0;
		diag_set_dac(stX + i, y0);
		x0 = diag_get_adc(stX + i, true);
		diag_set_dac(stX + i, y1);
		x0 = diag_get_adc(stX + i, true);
		gain = (y1 - y0) / (x1 - x0);
    	offset = (-gain * x0) + y0;
    	    	
    	if ( fabs(gain - 1.0) > limit){
			diag_log_pf(0, "%s ----- [FAIL] idx:%d, gain:%.3f, ofs:%.3f", __FUNCTION__, i, gain, offset);
            return false;
        }
    	
    	diag_set_cal(CAL_HV_F, i, gain, offset);
    }
    diag_log_pf( 1 , "%s Success", __FUNCTION__);   
    return true;
}

bool doCalDac()
{
	double x0, x1, y0, y1, gain, offset;
	double limit = 0.1;
	int stX = VREF1;	
	for (int i = 0; i<4; i++)
	{
		y0 = 1.0;
		y1 = 4.0;
		diag_set_dac(stX + i, y0);
		x0 = diag_get_adc(stX + i, true);
		diag_set_dac(stX + i, y1);
		x1 = diag_get_adc(stX + i, true);
    	gain = (y1 - y0) / (x1 - x0);
    	offset = (-gain * x0) + y0;
    		    	
    	if ( fabs(gain - 1.0) > limit){
    	    diag_log_pf(0, "%s ----- [FAIL] idx:%d, gain:%.3f, ofs:%.3f", __FUNCTION__, i, gain, offset);
    	    return false;
    	}
    	//(stX + i + 4) -> //Subtraction HV 4 index
    	diag_set_cal(CAL_PWR_F, (stX + i + 4), gain, offset);
    }
    
    // VH1~16,VTH1~VTH8
    stX = VH1;	
	for (int i = 0; i<24; i++)
	{
		y0 = 1.0;
		y1 = 4.0;
		diag_set_dac(stX + i, y0);
		x0 = diag_get_adc(stX + i, true);
		diag_set_dac(stX + i, y1);
		x1 = diag_get_adc(stX + i, true);
    	gain = (y1 - y0) / (x1 - x0);
    	offset = (-gain * x0) + y0;
    		    	
    	if ( fabs(gain - 1.0) > limit){
    	    diag_log_pf(0, "%s ----- [FAIL] idx:%d, gain:%.3f, ofs:%.3f", __FUNCTION__, i, gain, offset);
    	    return false;
    	}    	
    	diag_set_cal(CAL_PWR_F, stX + i, gain, offset);
    }
	diag_log_pf( 1 , "%s Success", __FUNCTION__);   
	return true;
}

bool doCalDps()
{	
		
	float MeasAdcV[2][MAX_DPS_CH]; //ADC Read Voltage
	float MeasDpsV[2][MAX_DPS_CH]; //DPS Voltage Measure
    float SetV[2] = {1.0,4.0};  //CAL : 2-Point , DIAG : Test Condition
    
	float limit = 0.01;         //CAL : Gain Check , DIAG : Pass/Fail Limit	
	float x0, x1, y0, y1, gain, offset;
	
	
	for (int i = 0; i< 2; i++)
	{
		for (int ch = 0; ch<MAX_DPS_CH; ch++)
            DPS(ch , SetV[i], SetV[i]-0.2, SetV[i]+0.2, 1.0);
        SRON();
        usleep(200000);        
        diag_read_all();        
        
        int stX = DPS_S1;
        for (int ch = 0; ch<MAX_DPS_CH; ch++){
        	MeasAdcV[i][ch] = diag_get_adc(stX++);                                	
            MeasDpsV[i][ch] = GET_DPS_V(stX++);                        
        }
        SROF();
        usleep(100000);        
	}
	
	for (int ch = 0; ch<MAX_DPS_CH; ch++)
	{
		y0 = MeasAdcV[0][ch];
		y1 = MeasAdcV[1][ch];
		x0 = SetV[0];
		x1 = SetV[1];
		
		gain = (y1 - y0) / (x1 - x0);
		offset = (-gain * x0) + y0;		
		diag_set_cal(CAL_DPS_F, ch, gain, offset);
        if ( fabs(gain - 1.0) > limit){
    	    diag_log_pf(0, "%s ----- [FAIL]  Force: idx:%d, gain:%.3f, ofs:%.3f", __FUNCTION__, ch, gain, offset);
    	    return false;
    	}    	
	}
	
	for (int ch = 0; ch<MAX_DPS_CH; ch++)
	{
		y0 = MeasAdcV[0][ch];
		y1 = MeasAdcV[1][ch];
		x0 = MeasDpsV[0][ch];
		x1 = MeasDpsV[1][ch];
		
		gain = (y1 - y0) / (x1 - x0);
		offset = (-gain * x0) + y0;		
		diag_set_cal(CAL_DPS_V, ch, gain, offset);
		if ( fabs(gain - 1.0) > limit){
    	    diag_log_pf(0, "%s ----- [FAIL] Volt: idx:%d, gain:%.3f, ofs:%.3f", __FUNCTION__, ch, gain, offset);
    	    return false;
    	}
		
	}
	diag_log_pf( 1 , "%s Success", __FUNCTION__);   	
	return true;
	
}

bool doCal()
{
	diag_init_cal();
	bool ret = true;
    ret &= doCalAdc();
    ret &= doCalHv();
    ret &= doCalDac();
    ret &= doCalDps();
        
    if (!ret)diag_init_cal();
    diag_save_cal_file();
    return ret;
    	
}


//////////////////////////////////////////////////////////
//    Power Diag
//////////////////////////////////////////////////////////
bool checkAdcRead(const char* title, int type, double check, double limit)
{
	cstring str;
	str.Format("%s ", title);	
	
	//float limit = 0.001;   
	bool ret = true;
	
	double read = diag_get_adc(type);			
	if (read < (check-limit) || read > (check+limit)) {	
        str.AppendFormat("----- [FAIL] Read:%.3f, check:%.3f,  limit:%.3f", 
            read, check, limit);
        ret = false;
    }else {
        str.AppendFormat("----- [PASS]");        
    }
    diag_log_pf(ret ? 1 : 0, str.c_str());   
    
    return ret;     
}

bool doDiagAdc()
{
	cstring str;
	bool ret = true;
	double limit = 0.001;
         
	diag_read_all();	
	ret &= checkAdcRead("ADC V5N(-5)"   ,DIV_NAG5   , -2.5,     limit); 
	ret &= checkAdcRead("ADC V18P"      ,DIV18      , 9, 		limit); 
	ret &= checkAdcRead("ADC V12.0P"    ,DIV12P     , 6, 		limit); 
	ret &= checkAdcRead("ADC V12.0P_0"  ,DIV12P0    , 6, 		limit); 
	ret &= checkAdcRead("ADC V12.0P_1"  ,DIV12P1    , 6, 		limit); 
	ret &= checkAdcRead("ADC CPU_V12.0P",DIV_12CUP  , 6, 		limit); 
	ret &= checkAdcRead("ADC V9.5P"     ,DIV9DOT5   , (9.5 / 2),limit); 
	ret &= checkAdcRead("ADC V5.0P"     ,NODIV5     , 5, limit); 
	
	ret &= checkAdcRead("ADC V3.3P"     ,V3_3     , 3.3, 		limit); 	
	ret &= checkAdcRead("ADC V2.5P"     ,V2_5     , 2.5, 		limit); 	
	ret &= checkAdcRead("ADC V1.8P"     ,V1_8     , 1.8, 		limit); 	
	ret &= checkAdcRead("ADC V1.2P"     ,V1_2     , 1.2, 		limit); 	
	ret &= checkAdcRead("ADC V1.0P"     ,V1_0     , 1.0, 		limit);
	ret &= checkAdcRead("ADC V0.9P"     ,V0_9     , 0.9, 		limit); 	
    ret &= checkAdcRead("ADC V0.85P"    ,V0_85    , 0.85,		limit);
	ret &= checkAdcRead("ADC V0.6P"     ,V0_6     , 0.6, 		limit);
	
	ret &= checkAdcRead("ADC VREF_V4.096_CH1"     ,VREF4V1	, 4.096, limit); 	
	ret &= checkAdcRead("ADC VREF_V4.096_CH2"     ,VREF4V2	, 4.096, limit); 	
	ret &= checkAdcRead("ADC VREF_V4.096_CH3"     ,VREF4V3	, 4.096, limit); 	
	ret &= checkAdcRead("ADC VREF_V4.096_CH4"     ,VREF4V4	, 4.096, limit); 	
	ret &= checkAdcRead("ADC VREF_V4.096_CH5"     ,VREF4V5	, 4.096, limit);
	ret &= checkAdcRead("ADC VREF_V4.096_CH6"     ,VREF4V6	, 4.096, limit); 	
    ret &= checkAdcRead("ADC VREF_V4.096_CH7"     ,VREF4V7	, 4.096, limit);
	ret &= checkAdcRead("ADC VREF_V4.096_CH8"     ,VREF4V8	, 4.096, limit);
	
	ret &= checkAdcRead("ADC GND_CH1"     ,GND1	, 0, limit); 	
	ret &= checkAdcRead("ADC GND_CH2"     ,GND2	, 0, limit); 	
	ret &= checkAdcRead("ADC GND_CH3"     ,GND3	, 0, limit); 	
	ret &= checkAdcRead("ADC GND_CH4"     ,GND4	, 0, limit); 	
	ret &= checkAdcRead("ADC GND_CH5"     ,GND5	, 0, limit);
	ret &= checkAdcRead("ADC GND_CH6"     ,GND6	, 0, limit); 	
    ret &= checkAdcRead("ADC GND_CH7"     ,GND7	, 0, limit);
	ret &= checkAdcRead("ADC GND_CH8"     ,GND8	, 0, limit);
	
	 
	return ret;
}

bool doDiagVolt()
{
	bool ret = true;
	cstring str;str.Format("");
    double limit = 0.1;
    
    // Level Setting
    
    diag_set_dac(VH_ALL, 1.0);
    diag_set_dac(VTH_ALL, 0.5);
    diag_set_dac(VREF_ALL, 0.0);
    
    
    int stIndex = VH1;        
    for (int i = 0; i<16; i++)
    {    	    	
    	str.Format("VOLD VH%d", i+1);
    	ret &= checkAdcRead(str.c_str(), stIndex, 1.0, limit);
    	stIndex++;
    }    
    
	stIndex = VTH1;
	for (int i = 0; i<8; i++)
    {    	    	
    	str.Format("VOLD VTH%d", i+1);
    	ret &= checkAdcRead(str.c_str(), stIndex, 0.5, limit);
    	stIndex++;
    }
        
    stIndex = VREF1;
	for (int i = 0; i<4; i++)
    {        	
    	str.Format("VOLD VREF%d", i+1);
    	ret &= checkAdcRead(str.c_str(), stIndex, 0.0, limit);
    	stIndex++;
    }
    
    
    for (int i = 0; i<4; i++)
        diag_set_dac(HV1+ i, 8.0); // 8V Setting
    diag_enable_hv(0xf);
    stIndex = HV1;
	for (int i = 0; i<4; i++)
    {    	    	
    	str.Format("VOLD HV%d", i+1);
    	ret &= checkAdcRead(str.c_str(), stIndex, 8.0, limit);
    	stIndex++;
    }
    diag_enable_hv(0);
			
	return ret;
}

bool doDiagDps()
{    
    float MeasAdcV[4][MAX_DPS_CH]; //ADC Read Voltage
	float MeasDpsV[4][MAX_DPS_CH]; //DPS Voltage Measure
	float SetV[4] = {1.0,2.0,3.0,4.0};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.005;
	bool ret = true;
	
	cstring str;str.Format("");	
	for (int i = 0; i< 4; i++)
	{	
        for (int ch = 0; ch<MAX_DPS_CH; ch++)
            DPS(ch , SetV[i], SetV[i]-0.2, SetV[i]+0.2, 1.0);		
        SRON();
        usleep(200000);        
        diag_read_all();        
        
        int stX = DPS_S1;
        for (int ch = 0; ch<MAX_DPS_CH; ch++){
        	MeasAdcV[i][ch] = diag_get_adc(stX++);                                	
            MeasDpsV[i][ch] = GET_DPS_V(stX++);                        
        }
        SROF();
        usleep(100000);        
	}
    
    
    for (int i = 0; i< 4; i++)
	{
        for (int ch = 0; ch<MAX_DPS_CH; ch++)
        {
        	str.Format("DPS Ch[%d] ---- ");
            if( (MeasDpsV[i][ch] < (SetV[i]-limit)) || 
                (MeasDpsV[i][ch] > (SetV[i]+limit)) || 
                (MeasAdcV[i][ch] < (SetV[i]-limit)) || 
                (MeasAdcV[i][ch] > (SetV[i]+limit))) 
            {
                str.AppendFormat("[FAIL] Set:%.2f, DpsV:%.2f, AdcV:%.2f, limit:%.2f"
                    , ch, SetV[i], MeasDpsV[ch], MeasAdcV[ch], limit) ;                
                diag_log_pf(ret, str.c_str());                       
                ret = false;
            }else {
            	str.AppendFormat("[PASS]");                
            	diag_log_pf(ret, str.c_str());                       
            }
            
        }
    }
			
	return true;
}
	

bool doDiagPower()
{
	bool ret = true;
	ret &= doDiagAdc();
	ret &= doDiagVolt();
	ret &= doDiagDps();
	
	//TIME(1, 50 NS, {VS1, VS2, VS3});
	return ret;
}



void SYNC(int outCMD)	
{
    if(outCMD == 1)
        diag_log_pf(1, "[%20s]","OPCODE_OUT_00");
    else if(outCMD == 2)
        diag_log_pf(1, "[%20s]","OPCODE_OUT_01");
    else if(outCMD == 3)
        diag_log_pf(1, "[%20s]","OPCODE_OUT_02");
    else 
        diag_log_pf(1, "[%20s]","OPCODE_OUT_00");
    NEXT_SYNC();
}

void SETUP(){}

void CONTACT(){}

void TEST() 
{
	printf("DIAG BUILD : %s. Ver : %s\n", DIAG_BUILD_DATE, DIAG_VER);
    writeFPGA("/star/test/pg_top.bit");
    bool bPass = true;
    uint16_t mode  = diag_get_mode();
    
    diag_enable_hv(0xf);
    diag_enable_vref(0xf);
    diag_enable_dac(0xf);
    if(mode & DIAG_CAL ){    
        bPass = doCal();
        if (bPass)
            diag_msg(1, "[CALIBRATION]");
        else
            diag_msg(0, "[CALIBRATION]");
    }
    
    
    // VIH, VTH, VREF Init   --> lib로 넣어야.

        
    if (mode & DIAG_SYS)
    {
    	bPass = doDiagPower();
    	if (bPass)
            diag_msg(1, "[SYSTEM VOLT]");
        else
            diag_msg(0, "[SYSTEM VOLT]");
    }
}  



