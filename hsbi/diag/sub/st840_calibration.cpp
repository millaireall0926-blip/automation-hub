//#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
//#include <string.h>
//#include <stdlib.h>
//#include <time.h>
#include <math.h>
//#include <iostream>
#include <sstream>
#include <iomanip>
//#include "define.h"
//#include "STAR_HSBI.h"
//#include "DIAG_HSBI.h"

#define MAX_DPS 20
#define MAX_VTT 16
#define MAX_VREF 16
#define MAX_PGPWR 4

#define MAKE_CODE2VOLT(CODE) (CODE *  20.0 / 65535.0)

#define FFixed(A)  std::fixed<<std::setprecision(A)

//const char *GRADE = "STAR";

float adc_s_gain[2][8];     //ADC Sense
float adc_s_offset[2][8];   //ADC Sense

float dps_f_gain[20];       //DPS Voltage Force
float dps_f_offset[20];     //DPS Voltage Force

float dps_s_gain[20];       //DPS Voltage Sense
float dps_s_offset[20];     //DPS Voltage Sense

float dps_i_gain[20];       //DPS Current Sense
float dps_i_offset[20];     //DPS Current Sense

float dutvtt_f_gain[16];    //DUTVTT Voltage Force
float dutvtt_f_offset[16];  //DUTVTT Voltage Force

float iovtt_f_gain[16];     //IOVTT Voltage Force
float iovtt_f_offset[16];   //IOVTT Voltage Force

float dutvref_f_gain[16];   //DUTVREF Voltage Force
float dutvref_f_offset[16]; //DUTVREF Voltage Force

float pgio_f_gain[4];       //PG-IO Voltage Force
float pgio_f_offset[4];     //PG-IO Voltage Force

float pgdr_f_gain[4];       //PG-DR Voltage Force
float pgdr_f_offset[4];     //PG-DR Voltage Force

float pgvth_f_gain[4];      //PG-VTH Voltage Force
float pgvth_f_offset[4];    //PG-VTH Voltage Force

std::vector<stCalFactor> adc_calData[2];
std::vector<stCalFactor> dac_calData[2];

std::vector<stCalFactor> dps_vscalData;
std::vector<stCalFactor> dps_vmcalData;
std::vector<stCalFactor> dps_imcalData;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
// Global Variable
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   


std::stringstream g_strLog;

//void SYNC(int outCMD)	// Don't Remove 
//{
//    if(outCMD == 1)
//        printf("Operand 1\n");
//    if(outCMD ==2)
//        printf("Operand 2\n");
//}
//
//void SETUP()
//{
//}
//
//void CONTACT()
//{                                                                     	                                                                                                                                                                                                                                                                      
//}


bool adc_cal()
{
	diag_log("==== ADC CALIBRATION START =======================================\n");	
	bool bResult = true;	
	short gnd[2][8]; // 0.0
	short ref[2][8]; // 5.0
	float fgnd[2][8]; // 0.0
	float fref[2][8]; // 5.0
	
	float limit = 0.01;

    //ADC Cal Factor Init
    for(int ch = 0; ch < 8; ch++)
    {
        adc_s_gain[0][ch] = 1.0;
        adc_s_offset[0][ch] = 0.0;
        
        adc_s_gain[1][ch] = 1.0;
        adc_s_offset[1][ch] = 0.0;
    }
    
	//ADC Read
	int nCtrl = 0;
	int mux = 0;
	diag_readAdc( nCtrl, mux, gnd[0]); // GND
	mux = 1;
	diag_readAdc( nCtrl, mux, ref[0]); // 5V
	
	nCtrl = 1;
	mux = 0;
	diag_readAdc( nCtrl, mux, gnd[1]); // GND
	mux = 1;
	diag_readAdc( nCtrl, mux, ref[1]); // 5V
	
	// Convert & Calculate
	for(int idx = 0; idx < 8; idx++) {
		fgnd[0][idx] = MAKE_CODE2VOLT(gnd[0][idx]);
		fgnd[1][idx] = MAKE_CODE2VOLT(gnd[1][idx]);
		
		fref[0][idx] = MAKE_CODE2VOLT(ref[0][idx]);
		fref[1][idx] = MAKE_CODE2VOLT(ref[1][idx]);
		
		adc_s_gain[0][idx] = 5 / (fref[0][idx] - fgnd[0][idx]);
    	adc_s_offset[0][idx] = (-adc_s_gain[0][idx]) * fgnd[0][idx];   	
        
        adc_s_gain[1][idx] = 5 / (fref[1][idx] - fgnd[1][idx]);
    	adc_s_offset[1][idx] = (-adc_s_gain[1][idx]) * fgnd[1][idx];	
	}
	
	// Log
    for(int idx = 0; idx < 8; idx++)
    {    	
        g_strLog.str("");
        g_strLog << "ADC Ch"<< idx << "  CAL : " << FFixed(4) << fgnd[0][idx] << FFixed(4) << " " << fref[0][idx];
        g_strLog << "  Gain:"<< adc_s_gain[0][idx] << "  Offset:"<< adc_s_offset[0][idx];
		if((adc_s_gain[0][idx] < 1.0-limit) || (adc_s_gain[0][idx] > 1.0+limit)) {
            bResult = false;
            g_strLog << " ------- [FAIL]\n";
        }else  {
            g_strLog << " ------- [PASS]\n";
        }
        std::cout << g_strLog.str();
    }
    
    for(int idx = 0; idx < 8; idx++)
    {  
        g_strLog.str("");
        g_strLog << "ADC Ch"<< idx + 8<< "  CAL : " << FFixed(4) << fgnd[1][idx] << FFixed(4) << " " << fref[1][idx];
        g_strLog << "  Gain:"<< adc_s_gain[1][idx] << "  Offset:"<< adc_s_offset[1][idx];
		if((adc_s_gain[1][idx] < 1.0-limit) || (adc_s_gain[1][idx] > 1.0+limit)) {
            bResult = false;
            g_strLog << " ------- [FAIL]\n";
        }else  {
            g_strLog << " ------- [PASS]\n";
        }
        std::cout << g_strLog.str();
    }
    
    diag_log("==== ADC CALIBRATION END =========================================\n\n");	
    //std::cout<<std::endl;		
	return bResult;
}

STEP(ADC_CAL) {
	adc_cal();
}



bool adc_diag()
{
    diag_log("==== ADC DIAGNOSTIC START ========================================\n");	
	bool bResult = true;
	short gnd[2][8]; // 0.0
	short ref[2][8]; // 5.0
	float fgnd[2][8]; // 0.0
	float fref[2][8]; // 5.0
    float AGain[2][8];  //ADC Sense Gain
    float AOffs[2][8];  //ADC Sense Offset
    
	float limit = 0.001;

	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    } 
    
	//ADC Read
	int nCtrl = 0;
	int mux = 0;
	diag_readAdc( nCtrl, mux, gnd[0]); // GND
	mux = 1;
	diag_readAdc( nCtrl, mux, ref[0]); // 5V
	
	nCtrl = 1;
	mux = 0;
	diag_readAdc( nCtrl, mux, gnd[1]); // GND
	mux = 1;
	diag_readAdc( nCtrl, mux, ref[1]); // 5V
	
	// Convert
    for(int idx = 0; idx < 8; idx++)
    {
        fgnd[0][idx] = MAKE_CODE2VOLT(gnd[0][idx]) * AGain[0][idx] + AOffs[0][idx];
        fgnd[1][idx] = MAKE_CODE2VOLT(gnd[1][idx]) * AGain[1][idx] + AOffs[1][idx];
        
        fref[0][idx] = MAKE_CODE2VOLT(ref[0][idx]) * AGain[0][idx] + AOffs[0][idx];
        fref[1][idx] = MAKE_CODE2VOLT(ref[1][idx]) * AGain[1][idx] + AOffs[1][idx];
    }

	// Pass/Fail Check (GND)
    for(int idx = 0; idx < 8; idx++) {
        g_strLog.str("");
        g_strLog << "ADC Ch"<< idx << " GND Test Read : "<< FFixed(4) << fgnd[0][idx];
		if((fgnd[0][idx] < -limit) || (fgnd[0][idx] > limit)) {
            bResult = false;
            g_strLog << " ------- [FAIL]\n";
        }else  {
            g_strLog << " ------- [PASS]\n";
        }
        diag_log_pf(bResult ? 1 : 0, g_strLog.str().c_str());
        //std::cout << g_strLog.str();
    }
    
    for(int idx = 0; idx < 8; idx++) {
        g_strLog.str("");
        g_strLog << "ADC Ch"<< idx + 8<< " GND Test Read : " << FFixed(4) << fgnd[1][idx];
		if((fgnd[1][idx] < -limit) || (fgnd[1][idx] > limit)) {
            bResult = false;
            g_strLog << " ------- [FAIL]\n";
        }else  {
            g_strLog << " ------- [PASS]\n";
        }
        diag_log_pf(bResult ? 1 : 0, g_strLog.str().c_str());
        //std::cout << g_strLog.str();
    }
    
    std::cout<<std::endl;
    
    // Pass/Fail Check (5V)
    for(int idx = 0; idx < 8; idx++) {
        g_strLog.str("");
        g_strLog << "ADC Ch"<< idx << " 5V Test Read : " << FFixed(4) << fref[0][idx];
		if((fref[0][idx] < 5.0-limit) || (fref[0][idx] > 5.0+limit)) {
            bResult = false;
            g_strLog << " ------- [FAIL]\n";
        }else  {
            g_strLog << " ------- [PASS]\n";
        }
        diag_log_pf(bResult ? 1 : 0, g_strLog.str().c_str());
        //std::cout << g_strLog.str();
    }
    
    for(int idx = 0; idx < 8; idx++) {
        g_strLog.str("");
        g_strLog << "ADC Ch"<< idx + 8<< " 5V Test Read : " << FFixed(4) << fref[1][idx];
		if((fref[1][idx] < 5.0-limit) || (fref[1][idx] > 5.0+limit)) {
            bResult = false;
            g_strLog << " ------- [FAIL]\n";
        }else  {
            g_strLog << " ------- [PASS]\n";
        }
        diag_log_pf(bResult ? 1 : 0, g_strLog.str().c_str());
        //std::cout << g_strLog.str();
    }
    
    diag_log("==== ADC DIAGNOSTIC END ==========================================\n");	
	std::cout<<std::endl;
	return bResult;
}

STEP(ADC_DIAG) {
	adc_diag();
}








bool sys_volt_diag()
{
	diag_log("==== SYSTEM VOLTAGE DIAGNOSTIC START =============================\n");
	bool bResult = true;	
	int mux = 2;
	short data[2][8];
	float volt[2];
	float abs_value[2];
    float AGain[2][8];  //ADC Sense Gain
    float AOffs[2][8];  //ADC Sense Offset
    
	float limit = 0.1;	
	
	float sys_volt[14][4] = { {5.0 , 1.0, 0.85, 0.85}, //2
                              {3.3 , 1.0, 0.9, 0.9},   //3
                              {0.035 , 1.2, 1.2, 1.2},   //4
                              {6.0 , 3.3, 3.3, 3.3},   //5
                              {2.5 , 1.8, 2.5, 2.5},   //6
                              {-5.0 , 1.0, 1.8, 1.8},   //7
                              {-5.0 , 1.8, 1.2, 1.2},   //8
                              {-5.0 , 2.0, 0.6, 0.6},   //9
                              {6.0 , 1.8, 1.0, 1.0},   //A
                              {5.0 , 3.3, 0.9, 0.9},   //B
                              {3.3 , 6.0, 0.9, 0.9},   //C
                              {3.3 , 5.0, 0, 0},   //D
                              {2.5 , 3.0, 0, 0},   //E
                              {2.5 , 100, 0, 0}    //F
                            }; 
	
	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    } 
	
	for(int idx = 0 ; idx < 14; idx++,mux++) {
        diag_readAdc( 0, mux, data[0]); 
        diag_readAdc( 1, mux, data[1]);
        for(int n = 0; n < 4; n++) {
        	volt[0] = MAKE_CODE2VOLT(data[0][n]) * AGain[0][n] + AOffs[0][n];
        	volt[1] = MAKE_CODE2VOLT(data[1][n]) * AGain[1][n] + AOffs[1][n];
            abs_value[0] = fabs(volt[0]-sys_volt[idx][n]);
            abs_value[1] = fabs(volt[1]-sys_volt[idx][n]);
            if(abs_value[0] > limit && sys_volt[idx][n] < 99) {
                bResult = false;
                diag_log_pf(0, " Ctrl 0 MUX:%02d CH%02d Read:%0.4fV----Exp:%0.4fV", mux, n, volt[0], sys_volt[idx][n]);
            }
            else 
                diag_log_pf(1, " Ctrl 0 MUX:%02d CH%02d Read:%0.4fV----Exp:%0.4fV", mux, n, volt[0], sys_volt[idx][n]);
                
            if(abs_value[1] > 0.1 && sys_volt[idx][n] < 99) {
                bResult = false;
                diag_log_pf(0, " Ctrl 1 MUX:%02d CH%02d Read:%0.4fV----Exp:%0.4fV", mux, n, volt[1], sys_volt[idx][n]);
            }
            else 
                diag_log_pf(1, " Ctrl 1 MUX:%02d CH%02d Read:%0.4fV----Exp:%0.4fV", mux, n, volt[1], sys_volt[idx][n]);
        }
        
        if(mux == 2 || mux == 3) {
        	volt[0] = MAKE_CODE2VOLT(data[0][7]) * AGain[0][7] + AOffs[0][7];
        	volt[1] = MAKE_CODE2VOLT(data[1][7]) * AGain[1][7] + AOffs[1][7];
            abs_value[0] = fabs(volt[0] - 6.0);
            abs_value[1] = fabs(volt[1] - 6.0);
            if(abs_value[0] > limit) {
                bResult = false;
                diag_log_pf(0, " Ctrl 0 MUX:%02d CH07 Read:%0.4fV----Exp:%0.4fV", mux, volt[0], 6.0);
            }
            else 
                diag_log_pf(1, " Ctrl 0 MUX:%02d CH07 Read:%0.4fV----Exp:%0.4fV", mux, volt[0], 6.0);
                
            if(abs_value[1] > 0.1){
                bResult = false;
                diag_log_pf(0, " Ctrl 1 MUX:%02d CH07 Read:%0.4fV----Exp:%0.4fV", mux, volt[1], 6.0);
            }
            else 
                diag_log_pf(1, " Ctrl 1 MUX:%02d CH07 Read:%0.4fV----Exp:%0.4fV", mux, volt[1], 6.0);
        }
    }
	diag_log("==== SYSTEM VOLTAGE DIAGNOSTIC END ===============================\n");

    if(bResult)
        diag_msg(1, "[SYSTEM VOLTAGE]"); 
    else
        diag_msg(0, "[SYSTEM VOLTAGE]");

	return bResult;
}

STEP(SYS_VOLT_DIAG)
{
	sys_volt_diag();
}








bool dps_cal()
{
	diag_log("==== DPS CALIBRATION START =======================================\n");
	bool bResult = true;
	short data[8] = {0};
	
	float MeasR[2][20]; //ADC Read Voltage
	float MeasV[2][20]; //DPS Voltage Measure
    float AGain[2][8];  //ADC Sense Gain
    float AOffs[2][8];  //ADC Sense Offset
    float FGain[20];    //Voltage Force Gain
    float FOffs[20];    //Voltage Force Offset
    float SGain[20];    //Voltage Sense Gain
    float SOffs[20];    //Voltage Sense Offset
    
	float SetV[2] = {1.0,4.0};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.01;         //CAL : Gain Check , DIAG : Pass/Fail Limit

	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    }
    
    //DPS Cal Factor Init
	for(int ch = 0; ch < MAX_DPS; ch++)
    {
        FGain[ch] = 1.0;
        FOffs[ch] = 0.0;
        SGain[ch] = 1.0;
        SOffs[ch] = 0.0;
		dps_f_gain[ch]      = 1.0;
		dps_f_offset[ch]    = 0.0;
		dps_s_gain[ch]      = 1.0;
		dps_s_offset[ch]    = 0.0;
	}
    
    //Measure
	for(int i = 0; i < 2; i++)
    {	
        for(int ch = 0; ch < MAX_DPS; ch++)
        {
            DPS(ch , SetV[i], SetV[i]-0.2, SetV[i]+0.2, 1.0);
        }
        SRON();
        usleep(200000);
        
        for(int mux = 4; mux < 0xE; mux++)
        {
            //DPS Channel 0 ~ 9
            diag_readAdc( 0, mux, data); 
            MeasR[i][mux - 4     ] = MAKE_CODE2VOLT(data[6]) * AGain[0][6] + AOffs[0][6];
            MeasV[i][mux - 4     ] = READ_VOUT(mux - 4 ,      10);
        
            //DPS Channel 10 ~ 19
            diag_readAdc( 1, mux, data); 
            MeasR[i][mux - 4 + 10] = MAKE_CODE2VOLT(data[6]) * AGain[1][6] + AOffs[1][6];
            MeasV[i][mux - 4 + 10] = READ_VOUT(mux - 4 + 10 , 10);            
        }

        SROF();
        usleep(100000);
    }

    //Calculate Force
	for(int ch = 0; ch < 20; ch++)
    {
        float X0 = MeasR[0][ch];
        float X1 = MeasR[1][ch];    	
        float Y0 = SetV[0];
        float Y1 = SetV[1];    	    	
        FGain[ch] = ( ( Y1 - Y0 ) / ( X1 - X0 ) );
    	FOffs[ch] = ( ( -FGain[ch] * X0 ) + Y0 ) ;    	
        dps_f_gain[ch]      = FGain[ch];
        dps_f_offset[ch]    = FOffs[ch];
    }
    
    //Calculate Sense
	for(int ch = 0; ch < 20; ch++)
    {
        float X0 = MeasV[0][ch];
        float X1 = MeasV[1][ch];    	
        float Y0 = MeasR[0][ch];
        float Y1 = MeasR[1][ch];     	
        SGain[ch] = ( ( Y1 - Y0 ) / ( X1 - X0 ) );
    	SOffs[ch] = ( ( -SGain[ch] * X0 ) + Y0 ) ;    	
        dps_s_gain[ch]      = SGain[ch];
        dps_s_offset[ch]    = SOffs[ch];
    }  
    
    //Log
	for(int ch = 0; ch < 20; ch++)
    {
        g_strLog.str("DPS_CAL ");
        g_strLog << "DPS Ch"<< ch << "  CAL : "<< FFixed(4) << MeasR[0][ch] << " " << MeasV[0][ch] << " " << MeasR[1][ch]<< " " << MeasV[1][ch];
        g_strLog << "  FGain : "<< FGain[ch] << "  FOffset : "<< FOffs[ch]<< "  SGain : "<< SGain[ch] << "  SOffset : "<< SOffs[ch];       
		if((FGain[ch] < 1.0-limit) || (FGain[ch] > 1.0+limit) || (SGain[ch] < 1.0-limit) || (SGain[ch] > 1.0+limit))  {
            bResult = false;
    		//g_strLog << " ------- [FAIL]\n";
            diag_log_pf(0, g_strLog.str().c_str());
        }else  {
    		//g_strLog << " ------- [PASS]\n";
            diag_log_pf(1, g_strLog.str().c_str());
        }
    	//std::cout << g_strLog.str();
    }  
    
    //initial
    for(int ch = 0; ch < MAX_DPS; ch++) {
        DPS(ch , 0.6, 0.6-0.2, 0.6+0.2, 1.0);
    }
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);		
	
	diag_log("==== DPS CALIBRATION END =========================================\n\n");
	//std::cout<<std::endl;	
	return bResult;	
}

STEP(DPS_CAL) {
	dps_cal();
}




bool dps_diag(int Mode = 0) //Mode "0" = Normal , "1" = PGM CalFactor Apply
{
	diag_log("==== DPS DIAGNOSTIC START ========== MODE %d ======================\n", Mode);
	bool bResult = true;
	short data[8] = {0};
	
	float MeasR[4][20]; //ADC Read Voltage
	float MeasV[4][20]; //DPS Voltage Measure
    float AGain[2][8];  //ADC Sense Gain
    float AOffs[2][8];  //ADC Sense Offset
    float FGain[20];    //Voltage Force Gain
    float FOffs[20];    //Voltage Force Offset
    float SGain[20];    //Voltage Sense Gain
    float SOffs[20];    //Voltage Sense Offset
    
	float SetV[4] = {1.0,2.0,3.0,4.0};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.005;                //CAL : Gain Check , DIAG : Pass/Fail Limit	

	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    }  
    
 	//DPS Cal Factor Select           
    if (Mode == 0)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_DPS; ch++)
        {
            FGain[ch] = 1.0;
            FOffs[ch] = 0.0;
            SGain[ch] = 1.0;
            SOffs[ch] = 0.0;
        }
    }
    else if (Mode == 1)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_DPS; ch++)
        {
            FGain[ch] = dps_f_gain  [ch];
            FOffs[ch] = dps_f_offset[ch];
            SGain[ch] = dps_s_gain  [ch];
            SOffs[ch] = dps_s_offset[ch];
        }
    } 

	//Measure											
	for(int i = 0; i < 4; i++)
    {
        for(int ch = 0; ch < MAX_DPS; ch++)
        {
            DPS(ch , SetV[i] * FGain[ch] + FOffs[ch], SetV[i]-0.2, SetV[i]+0.2, 2.0);
        }
        SRON();
        usleep(200000);
        
        for(int mux = 4; mux < 0xE; mux++)
        {
            //DPS Channel 0 ~ 9
            diag_readAdc( 0, mux, data); 
            MeasR[i][mux - 4     ] = MAKE_CODE2VOLT(data[6]) * AGain[0][6] + AOffs[0][6];
            MeasV[i][mux - 4     ] = READ_VOUT(mux - 4      , 10, true) * SGain[mux - 4     ] + SOffs[mux - 4     ]; 
        
            //DPS Channel 10 ~ 19
            diag_readAdc( 1, mux, data); 
            MeasR[i][mux - 4 + 10] = MAKE_CODE2VOLT(data[6]) * AGain[1][6] + AOffs[1][6];
            MeasV[i][mux - 4 + 10] = READ_VOUT(mux - 4 + 10 , 10, true) * SGain[mux - 4 + 10] + SOffs[mux - 4 + 10];           
        }
    
        SROF();
        usleep(100000);
    }
    
    //Log	
    for(int i = 0; i < 4; i++)
    {    
        // Pass/Fail Check
        for(int idx = 0; idx < MAX_DPS; idx++) {
            g_strLog.str("DPS_DIAG ");
            g_strLog << "DPS Ch"<< idx << "  Set : "<< FFixed(4) << SetV[i] << "  READ : " << MeasR[i][idx]<< "  VM : " << MeasV[i][idx];
            if((MeasV[i][idx] < (SetV[i]-limit)) || (MeasV[i][idx] > (SetV[i]+limit)) || (MeasR[i][idx] < (SetV[i]-limit)) || (MeasR[i][idx] > (SetV[i]+limit))) {
                bResult = false;
                //g_strLog << " ------- [FAIL]\n";
                diag_log_pf(0, g_strLog.str().c_str());
            }else  {
                //g_strLog << " ------- [PASS]\n";
                diag_log_pf(1, g_strLog.str().c_str());
            }
            //std::cout << g_strLog.str();
        }
        //std::cout<<std::endl;
        diag_log("\n");
    }
    
    //initial
    for(int ch = 0; ch < MAX_DPS; ch++) {
        DPS(ch , 0.6, 0.6-0.2, 0.6+0.2, 1.0);
    }
    
    //initial
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);			
	
	diag_log("==== DPS DIAGNOSTIC END ==========================================\n\n");
	//std::cout<<std::endl;
	return bResult;
}

STEP(DPS_DIAG) {
	dps_diag(0);
}
STEP(DPS_DIAG_TEST) {
	dps_diag(1);
}








bool dps_i_cal()
{
	diag_log("==== DPS CURRENT CALIBRATION START ===============================\n");
	bool bResult = true;
	short data[8] = {0};
	
	float MeasR[2][20]; //ADC Read Voltage
	float MeasN[2][20]; //ADC Read Voltage Negative (Use Only Current CAL) 	
	float MeasV[2][20]; //DPS Voltage Measure
	float MeasI[2][20]; //DPS Current Measure
	float MeasC[2][20]; //Calibration Resistor Value (ADC Read)
    float AGain[2][8];  //ADC Sense Gain
    float AOffs[2][8];  //ADC Sense Offset
    float FGain[20];    //Voltage Force Gain
    float FOffs[20];    //Voltage Force Offset
    float SGain[20];    //Voltage Sense Gain
    float SOffs[20];    //Voltage Sense Offset		
    float IGain[20];    //Current Sense Gain
    float IOffs[20];    //Current Sense Offset
    
	float SetV[2] = {1.0,4.0};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.2;          //CAL : Gain Check , DIAG : Pass/Fail Limit	
	float Rcal = 2.0;           //Calibration Resistor
	
	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    }  
    
 	//DPS Cal Factor Apply           
    for(int ch = 0; ch < MAX_DPS; ch++)
    {
        FGain[ch] = dps_f_gain  [ch];
        FOffs[ch] = dps_f_offset[ch];
        SGain[ch] = dps_s_gain  [ch];
        SOffs[ch] = dps_s_offset[ch];
    }
    
 	//DPS-Current Cal Factor Init           
	for(int ch = 0; ch < MAX_DPS; ch++)
    {
        IGain[ch]           = 1.0;
        IOffs[ch]           = 0.0;
        dps_i_gain[ch]      = 1.0;
		dps_i_offset[ch]    = 0.0;
	}
    
    //Measure
    writeIO(0xB03C, 0x100); //DPS CAL RES ENABLE
    usleep(100000);  
	for(int i = 0; i < 2; i++)
    {
        for(int ch = 0; ch < MAX_DPS; ch++)
        {
            DPS(ch , SetV[i] * FGain[ch] + FOffs[ch], SetV[i]-0.2, SetV[i]+0.2, 5.0);
        }
        SRON();
        usleep(200000);
                
        for(int ch = 0; ch < 10; ch++)
        {
            int mux = ch + 4;
            //DPS Channel 0 ~ 9
            diag_readAdc( 0, mux, data); 
            MeasR[i][ch] = MAKE_CODE2VOLT(data[6]) * AGain[0][6] + AOffs[0][6];
            MeasN[i][ch] = MAKE_CODE2VOLT(data[7]) * AGain[0][7] + AOffs[0][7];
            MeasV[i][ch] = READ_VOUT(ch , 10) * SGain[ch] + SOffs[ch]; 
            MeasI[i][ch] = READ_IOUT(ch , 10);
            MeasC[i][ch] = (MeasR[i][ch] - MeasN[i][ch]) / Rcal;

            //DPS Channel 10 ~ 19
            diag_readAdc( 1, mux, data); 
            MeasR[i][ch + 10] = MAKE_CODE2VOLT(data[6]) * AGain[1][6] + AOffs[1][6];
            MeasN[i][ch + 10] = MAKE_CODE2VOLT(data[7]) * AGain[1][7] + AOffs[1][7];
            MeasV[i][ch + 10] = READ_VOUT(ch + 10 , 10) * SGain[ch + 10] + SOffs[ch + 10]; 
            MeasI[i][ch + 10] = READ_IOUT(ch + 10 , 10);
            MeasC[i][ch + 10] = (MeasR[i][ch + 10] - MeasN[i][ch + 10]) / Rcal;         
        }

        SROF();
        usleep(100000);
    }
    writeIO(0xB03C, 0x000); //DPS CAL RES DISABLE
    usleep(100000);     
    
    //Calculate
	for(int ch = 0; ch < MAX_DPS; ch++)
    {
        float X0 = MeasI[0][ch];
        float X1 = MeasI[1][ch];    	
        float Y0 = MeasC[0][ch];
        float Y1 = MeasC[1][ch];     	
        IGain[ch] = ( ( Y1 - Y0 ) / ( X1 - X0 ) );
    	IOffs[ch] = ( ( -IGain[ch] * X0 ) + Y0 ) ;    	
        dps_i_gain[ch]      = IGain[ch];
        dps_i_offset[ch]    = IOffs[ch];
    } 
    
    //Log
	for(int ch = 0; ch < MAX_DPS; ch++)
    {
        g_strLog.str("DPS_CAL ");
        g_strLog << "DPS Ch"<< ch << "  CAL : "<< FFixed(4) << MeasC[0][ch] << " " << MeasI[0][ch] << " " << MeasC[1][ch]<< " " << MeasI[1][ch];
        g_strLog << "  Gain : "<< IGain[ch] << "  Offset : "<< IOffs[ch];       
		if((IGain[ch] < 1.0-limit) || (IGain[ch] > 1.0+limit))  {
            bResult = false;
    		//g_strLog << " ------- [FAIL]\n";
            diag_log_pf(0, g_strLog.str().c_str());
        }else  {
    		//g_strLog << " ------- [PASS]\n";
            diag_log_pf(1, g_strLog.str().c_str());
        }
    	//std::cout << g_strLog.str();
    }  
    
    //initial
    for(int ch = 0; ch < MAX_DPS; ch++) {
        DPS(ch , 0.6, 0.6-0.2, 0.6+0.2, 1.0);
    }
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);

	diag_log("==== DPS CURRENT CALIBRATION END =================================\n\n");
	//std::cout<<std::endl;	
	return bResult;	
}

STEP(DPS_I_CAL) {
	dps_i_cal();
}




bool dps_i_diag(int Mode = 0) //Mode "0" = Normal , "1" = PGM CalFactor Apply
{
	printf("==== DPS CURRENT DIAGNOSTIC START ================================\n");
	bool bResult = true;
	short data[8] = {0};
	
    float MeasR[4][20]; //ADC Read Voltage
	float MeasN[4][20]; //ADC Read Voltage Negative (Use Only Current CAL) 	
	float MeasV[4][20]; //DPS Voltage Measure
	float MeasI[4][20]; //DPS Current Measure
	float MeasC[4][20]; //Calibration Resistor Value (ADC Read)
    float AGain[2][8];  //ADC Sense Gain
    float AOffs[2][8];  //ADC Sense Offset
    float FGain[20];    //Voltage Force Gain
    float FOffs[20];    //Voltage Force Offset
    float SGain[20];    //Voltage Sense Gain
    float SOffs[20];    //Voltage Sense Offset		
    float IGain[20];    //Current Sense Gain
    float IOffs[20];    //Current Sense Offset
            
	float SetV[4] = {1.0,2.0,3.0,4.0};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.1;                 //CAL : Gain Check , DIAG : Pass/Fail Limit	
	float Rcal = 2.0;                   //Calibration Resistor

	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    } 

    //DPS Cal Factor Select           
    if (Mode == 0)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_DPS; ch++)
        {
            FGain[ch] = 1.0;
            FOffs[ch] = 0.0;
            SGain[ch] = 1.0;
            SOffs[ch] = 0.0;
            IGain[ch] = 1.0;
            IOffs[ch] = 0.0;           
        }
    }
    else if (Mode == 1)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_DPS; ch++)
        {
            FGain[ch] = dps_f_gain  [ch];
            FOffs[ch] = dps_f_offset[ch];
            SGain[ch] = dps_s_gain  [ch];
            SOffs[ch] = dps_s_offset[ch];
            IGain[ch] = dps_i_gain  [ch];
            IOffs[ch] = dps_i_offset[ch];    
        }
    } 
    
	//Measure
    writeIO(0xB03C, 0x100); //DPS CAL RES ENABLE
    usleep(100000);
    for(int i = 0; i < 4; i++)
    {
        for(int ch = 0; ch < MAX_DPS; ch++)
        {
            DPS(ch , SetV[i] * FGain[ch] + FOffs[ch], SetV[i]-0.2, SetV[i]+0.2, 5.0);
        }
        SRON();
        usleep(200000);
        
        for(int mux = 4; mux < 0xE; mux++)
        {
            //DPS Channel 0 ~ 9
            diag_readAdc( 0, mux, data); 
            MeasR[i][mux - 4     ] = MAKE_CODE2VOLT(data[6]) * AGain[0][6] + AOffs[0][6];
            MeasN[i][mux - 4     ] = MAKE_CODE2VOLT(data[7]) * AGain[0][7] + AOffs[0][7];
            MeasV[i][mux - 4     ] = READ_VOUT(mux - 4      , 10) * SGain[mux - 4     ] + SOffs[mux - 4     ]; 
            MeasI[i][mux - 4     ] = READ_IOUT(mux - 4      , 10) * IGain[mux - 4     ] + IOffs[mux - 4     ];
            MeasC[i][mux - 4     ] = (MeasR[i][mux - 4     ] - MeasN[i][mux - 4     ]) / Rcal;

            //DPS Channel 10 ~ 19
            diag_readAdc( 1, mux, data); 
            MeasR[i][mux - 4 + 10] = MAKE_CODE2VOLT(data[6]) * AGain[1][6] + AOffs[1][6];
            MeasN[i][mux - 4 + 10] = MAKE_CODE2VOLT(data[7]) * AGain[1][7] + AOffs[1][7];
            MeasV[i][mux - 4 + 10] = READ_VOUT(mux - 4 + 10 , 10) * SGain[mux - 4 + 10] + SOffs[mux - 4 + 10]; 
            MeasI[i][mux - 4 + 10] = READ_IOUT(mux - 4 + 10 , 10) * IGain[mux - 4 + 10] + IOffs[mux - 4 + 10];
            MeasC[i][mux - 4 + 10] = (MeasR[i][mux - 4 + 10] - MeasN[i][mux - 4 + 10]) / Rcal;  
        }
        SROF();
        usleep(100000);
    }
    writeIO(0xB03C, 0x000); //DPS CAL RES DISABLE
    usleep(100000);     
    
    //Log	
    for(int i = 0; i < 4; i++)
    {    
        // Pass/Fail Check
        for(int idx = 0; idx < MAX_DPS; idx++) {
            g_strLog.str("DPS_I_DIAG ");
            g_strLog << "DPS Ch"<< idx << "  Set : "<< FFixed(4) << SetV[i] << "V" << "  READ : " << MeasC[i][idx] << "A" << "  IM : " << MeasI[i][idx] << "A";
            if((MeasI[i][idx] < (MeasC[i][idx]-limit)) || (MeasI[i][idx] > (MeasC[i][idx]+limit))) {
                bResult = false;
                //g_strLog << " ------- [FAIL]\n";
                diag_log_pf(0, g_strLog.str().c_str());
            }else  {
                //g_strLog << " ------- [PASS]\n";
                diag_log_pf(1, g_strLog.str().c_str());
            }
            //std::cout << g_strLog.str();
        }
        //std::cout<<std::endl;
        diag_log("\n");
    }
    
    //initial
    for(int ch = 0; ch < MAX_DPS; ch++) {
        DPS(ch , 0.6, 0.6-0.2, 0.6+0.2, 1.0);
    }
    
    //initial
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);
    
	diag_log("==== DPS CURRENT DIAGNOSTIC END ==================================\n\n");
	//std::cout<<std::endl;
	return bResult;
}

STEP(DPS_I_DIAG) {
	dps_i_diag(0);
}

STEP(DPS_I_DIAG_TEST) {
	dps_i_diag(1);
}








bool dutvtt_cal()
{
	diag_log("==== DUT VTT CALIBRATION START ===================================\n");
	bool bResult = true;
	short data[8] = {0};
	
	float MeasR[2][16];   //ADC Read Voltage
    float AGain[2][8];    //ADC Sense Gain
    float AOffs[2][8];    //ADC Sense Offset		
    float FGain[16];      //Voltage Force Gain
    float FOffs[16];      //Voltage Force Offset	
    
	float SetV[2] = {0.6,1.8};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.01;         //CAL : Gain Check , DIAG : Pass/Fail Limit	

	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    } 
    
    //DUTVTT Cal Factor Init	
	for(int ch = 0; ch < MAX_VTT; ch++)
    {
		dutvtt_f_gain[ch] = 1.0;
		dutvtt_f_offset[ch] = 0.0;
		FGain[ch] = 1.0;
        FOffs[ch] = 0.0;		
	}
    
    //Measure
	for(int i = 0; i < 2; i++)
    {	
        DUTVTT(ALL, SetV[i]);        
        SRON();
        usleep(200000);        
        for(int mux = 2; mux < 10; mux++)
        {
            diag_readAdc( 0, mux, data); 
            MeasR[i][mux - 2    ] = MAKE_CODE2VOLT(data[4]) * AGain[0][4] + AOffs[0][4];
            diag_readAdc( 1, mux, data); 
            MeasR[i][mux - 2 + 8] = MAKE_CODE2VOLT(data[4]) * AGain[1][4] + AOffs[1][4];
        }        
        SROF();
        usleep(100000);
    }

    //Calculate
	for(int ch = 0; ch < MAX_VTT; ch++)
    {
        float X0 = MeasR[0][ch];
        float X1 = MeasR[1][ch];    	
        float Y0 = SetV[0];
        float Y1 = SetV[1];    	    	
        FGain[ch] = ( ( Y1 - Y0 ) / ( X1 - X0 ) );
    	FOffs[ch] = ( ( -FGain[ch] * X0 ) + Y0 ) ;    	
        dutvtt_f_gain[ch]      = FGain[ch];
        dutvtt_f_offset[ch]    = FOffs[ch];
    }    
    
    //Log
	for(int ch = 0; ch < MAX_VTT; ch++)
    {
        g_strLog.str("DUTVTT_CAL ");
        g_strLog << "DUTVTT Ch"<< ch << "  CAL : "<< FFixed(4) << MeasR[0][ch] << " " << FFixed(4) << MeasR[1][ch];
        g_strLog << "  Gain : "<< FGain[ch] << "  Offset : "<< FOffs[ch];       
		if((FGain[ch] < 1.0-limit) || (FGain[ch] > 1.0+limit))  {
            bResult = false;
    		//g_strLog << " ------- [FAIL]\n";
            diag_log_pf(0, g_strLog.str().c_str());
        }else  {
    		//g_strLog << " ------- [PASS]\n";
            diag_log_pf(1, g_strLog.str().c_str());
        }
    	//std::cout << g_strLog.str();
        //diag_log("\n");
    }  
    
    //initial
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);		
	
	diag_log("==== DUT VTT CALIBRATION END =====================================\n\n");
	//std::cout<<std::endl;	
	return bResult;	
}

STEP(DUTVTT_CAL) {
	dutvtt_cal();
}




bool dutvtt_diag(int Mode = 0) //Mode "0" = Normal , "1" = PGM CalFactor Apply
{
	diag_log("==== DUT VTT DIAGNOSTIC START ====================================\n");
	bool bResult = true;
	short data[8] = {0};
	
	float MeasR[4][16];   //ADC Read Voltage
    float AGain[2][8];    //ADC Sense Gain
    float AOffs[2][8];    //ADC Sense Offset		
    float FGain[16];      //Voltage Force Gain
    float FOffs[16];      //Voltage Force Offset	
    
	float SetV[4] = {0.5,1.0,1.5,2.0};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.005;                //CAL : Gain Check , DIAG : Pass/Fail Limit	

	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    }  
    
    //DUTVTT Cal Factor Select           
    if (Mode == 0)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_VTT; ch++)
        {
            FGain[ch] = 1.0;
            FOffs[ch] = 0.0;
        }
    }
    else if (Mode == 1)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_VTT; ch++)
        {
            FGain[ch] = dutvtt_f_gain[ch];
            FOffs[ch] = dutvtt_f_offset[ch];
        }
    }

	//Measure	
	for(int i = 0; i < 4; i++)
    {	
        DUTVTT(ALL, SetV[i]);
        SRON();
        usleep(200000);        
        for(int mux = 2; mux < 10; mux++)
        {
            if (Mode == 1)
            {
                DUTVTT(ALL, SetV[i] * FGain[mux - 2    ] + FOffs[mux - 2    ] );
                SRON();
                usleep(100000);
            }
            
            diag_readAdc( 0, mux, data); 
            MeasR[i][mux - 2    ] = MAKE_CODE2VOLT(data[4]) * AGain[0][4] + AOffs[0][4];

            if (Mode == 1)
            {
                DUTVTT(ALL, SetV[i] * FGain[mux - 2 + 8] + FOffs[mux - 2 + 8] );
                SRON();
                usleep(100000);
            }
            
            diag_readAdc( 1, mux, data); 
            MeasR[i][mux - 2 + 8] = MAKE_CODE2VOLT(data[4]) * AGain[1][4] + AOffs[1][4];
        }        
        SROF();
        usleep(100000);
    }
	
	//Log	
	for(int i = 0; i < 4; i++)
    {
        for(int idx = 0; idx < MAX_VTT; idx++)
        {
            g_strLog.str("DUTVTT_DIAG ");
            g_strLog << "DUTVTT Ch"<< idx << "  Set : "<< FFixed(4) << SetV[i] << "  Read : " << MeasR[i][idx];
            if((MeasR[i][idx] < (SetV[i]-limit)) || (MeasR[i][idx] > (SetV[i]+limit))) {
                bResult = false;
                //g_strLog << " ------- [FAIL]\n";
                diag_log_pf(0, g_strLog.str().c_str());
            }else  {
                //g_strLog << " ------- [PASS]\n";
                diag_log_pf(1, g_strLog.str().c_str());;
            }
            //std::cout << g_strLog.str();
        }
        //std::cout<<std::endl;
        diag_log("\n");
    }
	
    //initial
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);			
	
	diag_log("==== DUT VTT DIAGNOSTIC END ======================================\n\n");
	//std::cout<<std::endl;
	return bResult;
}

STEP (DUTVTT_DIAG)  {
	dutvtt_diag(0);
}

STEP (DUTVTT_DIAG_TEST) {
	dutvtt_diag(1);
}








bool iovtt_cal()
{
	printf("==== IO VTT CALIBRATION START ====================================\n");	
	bool bResult = true;
	short data[8] = {0};
	
	float MeasR[2][16];   //ADC Read Voltage
    float AGain[2][8];    //ADC Sense Gain
    float AOffs[2][8];    //ADC Sense Offset		
    float FGain[16];      //Voltage Force Gain
    float FOffs[16];      //Voltage Force Offset		
    
	float SetV[2] = {0.6,1.8};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.01;         //CAL : Gain Check , DIAG : Pass/Fail Limit	
    
	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    }     
    
    //IOVTT Cal Factor Init		
	for(int ch = 0; ch < MAX_VTT; ch++)
	{
		iovtt_f_gain[ch] = 1.0;
		iovtt_f_offset[ch] = 0.0;
		FGain[ch] = 1.0;
        FOffs[ch] = 0.0;
	}
    
    //Measure
	for(int i = 0; i < 2; i++)
    {	
        VTT(ALL, SetV[i]);
        SRON();
        usleep(200000);        
        for(int mux = 2; mux < 10; mux++)
        {
            diag_readAdc( 0, mux, data); 
            MeasR[i][mux - 2    ] = MAKE_CODE2VOLT(data[5]) * AGain[0][5] + AOffs[0][5];
            diag_readAdc( 1, mux, data); 
            MeasR[i][mux - 2 + 8] = MAKE_CODE2VOLT(data[5]) * AGain[1][5] + AOffs[1][5];
        }
        SROF();
        usleep(100000);
    }

    //Calculate
	for(int ch = 0; ch < MAX_VTT; ch++)
    {
    float X0 = MeasR[0][ch];
    float X1 = MeasR[1][ch];    	
    float Y0 = SetV[0];
    float Y1 = SetV[1];    	    	
        FGain[ch] = ( ( Y1 - Y0 ) / ( X1 - X0 ) );
    	FOffs[ch] = ( ( -FGain[ch] * X0 ) + Y0 ) ;    	
        iovtt_f_gain[ch]      = FGain[ch];
        iovtt_f_offset[ch]    = FOffs[ch];
    }   
    
    //Log
	for(int ch = 0; ch < MAX_VTT; ch++)
    {
        g_strLog.str("IOVTT_CAL ");
        g_strLog << "IOVTT Ch"<< ch << "  CAL : "<< FFixed(4) << MeasR[0][ch] << " " << FFixed(4) << MeasR[1][ch];
        g_strLog << "  Gain:"<< FGain[ch] << "  Offset:"<< FOffs[ch];       
		if((FGain[ch] < 1.0-limit) || (FGain[ch] > 1.0+limit)) {
            bResult = false;
    		//g_strLog << " ------- [FAIL]\n";
            diag_log_pf(0, g_strLog.str().c_str());
        }else  {
    		//g_strLog << " ------- [PASS]\n";
            diag_log_pf(1, g_strLog.str().c_str());
        }
    	//std::cout << g_strLog.str();
    }  
    
    //initial
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);		
	
	diag_log("==== IO VTT CALIBRATION END ======================================\n\n");
	//std::cout<<std::endl;
	return bResult;	
}

STEP(IOVTT_CAL) {
	iovtt_cal();
}




bool iovtt_diag(int Mode = 0) //Mode "0" = Normal , "1" = PGM CalFactor Apply
{
	diag_log("==== IO VTT DIAGNOSTIC START =====================================\n");
	bool bResult = true;
	short data[8] = {0};
	
	float MeasR[4][16];   //ADC Read Voltage
    float AGain[2][8];    //ADC Sense Gain
    float AOffs[2][8];    //ADC Sense Offset		
    float FGain[16];      //Voltage Force Gain
    float FOffs[16];      //Voltage Force Offset	
    
	float SetV[4] = {0.5,1.0,1.5,2.0};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.005;                //CAL : Gain Check , DIAG : Pass/Fail Limit	

	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    } 
    
    //IOVTT Cal Factor Select           
    if (Mode == 0)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_VTT; ch++)
        {
            FGain[ch] = 1.0;
            FOffs[ch] = 0.0;
        }
    }
    else if (Mode == 1)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_VTT; ch++)
        {
            FGain[ch] = iovtt_f_gain[ch];
            FOffs[ch] = iovtt_f_offset[ch];
        }
    }
    
 	//Measure	   
	for(int i = 0; i < 4; i++)
    {	
        VTT(ALL, SetV[i]);
        SRON();
        usleep(200000);
        for(int mux = 2; mux < 10; mux++)
        {
            if (Mode == 1)
            {
                VTT(ALL, SetV[i] * FGain[mux - 2    ] + FOffs[mux - 2    ] );
                SRON();   	
                usleep(100000);
            }
            
            diag_readAdc( 0, mux, data); 
            MeasR[i][mux - 2    ] = MAKE_CODE2VOLT(data[5]) * AGain[0][5] + AOffs[0][5];
            
            if (Mode == 1)
            {         
                VTT(ALL, SetV[i] * FGain[mux - 2 + 8] + FOffs[mux - 2 + 8] );
                SRON();
                usleep(100000);
            }
            
            diag_readAdc( 1, mux, data); 
            MeasR[i][mux - 2 + 8] = MAKE_CODE2VOLT(data[5]) * AGain[1][5] + AOffs[1][5];
        }        
        SROF();
        usleep(100000);
    }


	//Log	
	for(int i = 0; i < 4; i++)
    {    
        for(int idx = 0; idx < MAX_VTT; idx++)
        {
            g_strLog.str("IOVTT_DIAG ");
            g_strLog << "IOVTT Ch"<< idx << "  Set : "<< FFixed(4) << SetV[i]  << "  Read : " << MeasR[i][idx];
            if((MeasR[i][idx] < (SetV[i]-limit)) || (MeasR[i][idx] > (SetV[i]+limit))) {
                bResult = false;
                //g_strLog << " ------- [FAIL]\n";
                diag_log_pf(0, g_strLog.str().c_str());
            }else  {
                //g_strLog << " ------- [PASS]\n";
                diag_log_pf(1, g_strLog.str().c_str());
            }
            //std::cout << g_strLog.str();
        } 
        //std::cout<<std::endl;
        diag_log("\n");
    }       

    //initial
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);		
    
	diag_log("==== IO VTT DIAGNOSTIC END =======================================\n\n");
	//std::cout<<std::endl;
    return bResult;
}

STEP (IOVTT_DIAG)   {
	iovtt_diag(0);
}

STEP (IOVTT_DIAG_TEST)   {
	iovtt_diag(1);
}








bool dutvref_cal()
{
	diag_log("==== DUTVREF CALIBRATION START ===================================\n");	
	bool bResult = true;
	short data[8] = {0};
	
	float MeasR[2][16];   //ADC Read Voltage
    float AGain[2][8];    //ADC Sense Gain
    float AOffs[2][8];    //ADC Sense Offset		
    float FGain[16];      //Voltage Force Gain
    float FOffs[16];      //Voltage Force Offset	
    
	float SetV[2] = {2.0,5.5};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.01;         //CAL : Gain Check , DIAG : Pass/Fail Limit	

	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    } 
    
    //DUTVREF Cal Factor Init		
	for(int ch = 0; ch < MAX_VREF; ch++)
	{
		dutvref_f_gain[ch] = 1.0;
		dutvref_f_offset[ch] = 0.0;
		FGain[ch] = 1.0;
        FOffs[ch] = 0.0;
	}
    
    //Measure
	for(int i = 0; i < 2; i++)
    {	
        DUTVREF1(SetV[i]);        
        DUTVREF2(SetV[i]);    
        SRON();
        usleep(200000);        
        for(int mux = 10; mux < 14; mux++)
        {
            diag_readAdc( 0, mux, data); 
            MeasR[i][mux - 10] = MAKE_CODE2VOLT(data[4]) * AGain[0][4] + AOffs[0][4];   //VREF1
            MeasR[i][mux -  6] = MAKE_CODE2VOLT(data[5]) * AGain[0][5] + AOffs[0][5];   //VREF2
            diag_readAdc( 1, mux, data); 
            MeasR[i][mux -  2] = MAKE_CODE2VOLT(data[4]) * AGain[1][4] + AOffs[1][4];   //VREF1
            MeasR[i][mux +  2] = MAKE_CODE2VOLT(data[5]) * AGain[1][5] + AOffs[1][5];   //VREF2
        }
        SROF();
        usleep(100000);
    }

    //Calculate
	for(int ch = 0; ch < MAX_VREF; ch++)
    {
        float X0 = MeasR[0][ch];
        float X1 = MeasR[1][ch];    	
        float Y0 = SetV[0];
        float Y1 = SetV[1];    	    	
        FGain[ch] = ( ( Y1 - Y0 ) / ( X1 - X0 ) );
    	FOffs[ch] = ( ( -FGain[ch] * X0 ) + Y0 ) ;    	
        dutvref_f_gain[ch]      = FGain[ch];
        dutvref_f_offset[ch]    = FOffs[ch];
    }    
    
    //Log
	for(int ch = 0; ch < MAX_VREF; ch++)
    {
        g_strLog.str("DUTVREF_CAL ");
        g_strLog << "DUTVREF Ch"<< ch << "  CAL : "<< FFixed(4) << MeasR[0][ch] << " " << FFixed(4) << MeasR[1][ch];
        g_strLog << "  Gain:"<< FGain[ch] << "  Offset:"<< FOffs[ch];       
		if((FGain[ch] < 1.0-limit) || (FGain[ch] > 1.0+limit)) {
            bResult = false;
    		//g_strLog << " ------- [FAIL]\n";
            diag_log_pf(0, g_strLog.str().c_str());
        }else  {
    		//g_strLog << " ------- [PASS]\n";
            diag_log_pf(1, g_strLog.str().c_str());
        }
    	//std::cout << g_strLog.str();
    }  
    
    //initial
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);		

	diag_log("==== DUTVREF CALIBRATION END =====================================\n\n");
	//std::cout<<std::endl;
	return bResult;	
}

STEP(DUTVREF_CAL) {
	dutvref_cal();
}





bool dutvref_diag(int Mode = 0) //Mode "0" = Normal , "1" = PGM CalFactor Apply
{
	diag_log("==== DUT VREF DIAGNOSTIC START ===================================\n");
	bool bResult = true;
	short data[8] = {0};
	
	float MeasR[4][16];   //ADC Read Voltage
    float AGain[2][8];    //ADC Sense Gain
    float AOffs[2][8];    //ADC Sense Offset		
    float FGain[16];      //Voltage Force Gain
    float FOffs[16];      //Voltage Force Offset
    
	float SetV[4] = {1.5,3.0,4.5,6.0};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.005;                //CAL : Gain Check , DIAG : Pass/Fail Limit	

	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    }
    
    //DUTVREF Cal Factor Select           
    if (Mode == 0)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_VREF; ch++)
        {
            FGain[ch] = 1.0;
            FOffs[ch] = 0.0;
        }
    }
    else if (Mode == 1)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_VREF; ch++)
        {
            FGain[ch] = dutvref_f_gain[ch];
            FOffs[ch] = dutvref_f_offset[ch];
        }
    }
    
	//Measure	        
	for(int i = 0; i < 4; i++)
    {	
        DUTVREF1(SetV[i]);        
        DUTVREF2(SetV[i]);      
        SRON();
        usleep(200000);        
        for(int mux = 10; mux < 14; mux++)
        {
            if (Mode == 1)
            {
                DUTVREF1(SetV[i] * FGain[mux - 10] + FOffs[mux - 10] );
                DUTVREF2(SetV[i] * FGain[mux -  6] + FOffs[mux -  6] );
                SRON();
                usleep(100000);
            }
        
            diag_readAdc( 0, mux, data); 
            MeasR[i][mux - 10] = MAKE_CODE2VOLT(data[4]) * AGain[0][4] + AOffs[0][4];   //VREF1
            MeasR[i][mux -  6] = MAKE_CODE2VOLT(data[5]) * AGain[0][5] + AOffs[0][5];   //VREF2
            
            if (Mode == 1)
            {
                DUTVREF1(SetV[i] * FGain[mux -  2] + FOffs[mux -  2] );
                DUTVREF2(SetV[i] * FGain[mux +  2] + FOffs[mux +  2] );
                SRON();
                usleep(100000);
            }
                
            diag_readAdc( 1, mux, data); 
            MeasR[i][mux -  2] = MAKE_CODE2VOLT(data[4]) * AGain[1][4] + AOffs[1][4];   //VREF1
            MeasR[i][mux +  2] = MAKE_CODE2VOLT(data[5]) * AGain[1][5] + AOffs[1][5];   //VREF2
        }        
        SROF();
        usleep(100000);
    }
	
	//Log	
	for(int i = 0; i < 4; i++)
    {        
        for(int idx = 0; idx < MAX_VREF; idx++) {
            g_strLog.str("DUTVREF_DIAG ");
            g_strLog << "DUTVREF Ch"<< idx << "  Set : "<< FFixed(4) << SetV[i] << "  Read : " << MeasR[i][idx];
            if((MeasR[i][idx] < (SetV[i]-limit)) || (MeasR[i][idx] > (SetV[i]+limit))) {
                bResult = false;
                //g_strLog << " ------- [FAIL]\n";
                diag_log_pf(0, g_strLog.str().c_str());
            }else  {
                //g_strLog << " ------- [PASS]\n";
                diag_log_pf(1, g_strLog.str().c_str());
            }
            //std::cout << g_strLog.str();
        }        
        //std::cout<<std::endl;        
        diag_log("\n");
    } 
    
    //initial
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);		 
	
	diag_log("==== DUT VREF DIAGNOSTIC END =====================================\n");
	//std::cout<<std::endl;
	return bResult;
}

STEP (DUTVREF_DIAG) {
	dutvref_diag(0);
}

STEP (DUTVREF_DIAG_TEST) {
	dutvref_diag(1);
}








bool pg_drio_cal()
{
	diag_log("==== PG DR/IO CALIBRATION START ==================================\n");	
	bool    bResult = true;
	short   data[8] = {0};
	
	float   MeasVio[2][4];    //ADC Read Voltage
	float   MeasVdr[2][4];    //ADC Read Voltage
    float   AGain[2][8];      //ADC Sense Gain
    float   AOffs[2][8];      //ADC Sense Offset	
    float   FGainIO[4];       //Voltage Force Gain
    float   FOffsIO[4];       //Voltage Force Offset	
	float   FGainDR[4];       //Voltage Force Gain
    float   FOffsDR[4];       //Voltage Force Offset	
    
	float SetV[2] = {0.8,1.5};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.01;         //CAL : Gain Check , DIAG : Pass/Fail Limit
	
	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    } 
    
    //PG-IO/DR Cal Factor Init		
	for(int ch = 0; ch < MAX_PGPWR; ch++)
	{
		pgio_f_gain[ch]     = 1.0;
		pgio_f_offset[ch]   = 0.0;
		pgdr_f_gain[ch]     = 1.0;
		pgdr_f_offset[ch]   = 0.0;
		FGainIO[ch] = 1.0;
        FOffsIO[ch] = 0.0;
        FGainDR[ch] = 1.0;
        FOffsDR[ch] = 0.0;
	}
    
    //Measure
	for(int i = 0; i < 2; i++)
    {	
        VIH(ALL, SetV[i]);
        VTH(SetV[i]/2);
        SRON();
        usleep(200000);
    
        diag_readAdc( 0, 10, data); 
        MeasVio[i][0] = MAKE_CODE2VOLT(data[2]) * AGain[0][2] + AOffs[0][2];
        MeasVio[i][1] = MAKE_CODE2VOLT(data[3]) * AGain[0][3] + AOffs[0][3];
        
        diag_readAdc( 0, 11, data); 
        MeasVdr[i][0] = MAKE_CODE2VOLT(data[2]) * AGain[0][2] + AOffs[0][2];
        MeasVdr[i][1] = MAKE_CODE2VOLT(data[3]) * AGain[0][3] + AOffs[0][3];
                
        diag_readAdc( 1, 10, data); 
        MeasVio[i][2] = MAKE_CODE2VOLT(data[2]) * AGain[1][2] + AOffs[1][2];
        MeasVio[i][3] = MAKE_CODE2VOLT(data[3]) * AGain[1][3] + AOffs[1][3];
        
        diag_readAdc( 1, 11, data); 
        MeasVdr[i][2] = MAKE_CODE2VOLT(data[2]) * AGain[1][2] + AOffs[1][2];
        MeasVdr[i][3] = MAKE_CODE2VOLT(data[3]) * AGain[1][3] + AOffs[1][3];

        SROF();
        usleep(100000);
    }

    //Calculate IO
	for(int ch = 0; ch < MAX_PGPWR; ch++)
    {
        float X0 = MeasVio[0][ch];
        float X1 = MeasVio[1][ch];    	
        float Y0 = SetV[0];
        float Y1 = SetV[1];    	    	
        FGainIO[ch] = ( ( Y1 - Y0 ) / ( X1 - X0 ) );
    	FOffsIO[ch] = ( ( -FGainIO[ch] * X0 ) + Y0 ) ;    	
        pgio_f_gain[ch]      = FGainIO[ch];
        pgio_f_offset[ch]    = FOffsIO[ch];
    }    
    
    //Calculate DR
	for(int ch = 0; ch < MAX_PGPWR; ch++)
    {
        float X0 = MeasVdr[0][ch];
        float X1 = MeasVdr[1][ch];    	
        float Y0 = SetV[0];
        float Y1 = SetV[1];    	    	
        FGainDR[ch] = ( ( Y1 - Y0 ) / ( X1 - X0 ) );
    	FOffsDR[ch] = ( ( -FGainDR[ch] * X0 ) + Y0 ) ;    	
        pgdr_f_gain[ch]      = FGainDR[ch];
        pgdr_f_offset[ch]    = FOffsDR[ch];
    }   
    
    //Log
	for(int ch = 0; ch < MAX_PGPWR; ch++)
    {
        g_strLog.str("PG_IO_CAL ");
        g_strLog << "PG_IO Ch"<< ch << "  CAL : "<< FFixed(4) << MeasVio[0][ch] << " " << FFixed(4) << MeasVio[1][ch];
        g_strLog << "  Gain:"<< FGainIO[ch] << "  Offset:"<< FOffsIO[ch];       
		if((FGainIO[ch] < 1.0-limit) || (FGainIO[ch] > 1.0+limit)) {
            bResult = false;
    		//g_strLog << " ------- [FAIL]\n";
            diag_log_pf(0, g_strLog.str().c_str());
        }else  {
    		//g_strLog << " ------- [PASS]\n";
            diag_log_pf(1, g_strLog.str().c_str());
        }
    	//std::cout << g_strLog.str();
        
        g_strLog.str("PG_DR_CAL ");
        g_strLog << "PG_DR Ch"<< ch << "  CAL : "<< FFixed(4) << MeasVdr[0][ch] << " " << FFixed(4) << MeasVdr[1][ch];
        g_strLog << "  Gain:"<< FGainDR[ch] << "  Offset:"<< FOffsDR[ch];       
		if((FGainDR[ch] < 1.0-limit) || (FGainDR[ch] > 1.0+limit)) {
            bResult = false;
    	    //g_strLog << " ------- [FAIL]\n";
            diag_log_pf(0, g_strLog.str().c_str());
        }else  {
            //g_strLog << " ------- [PASS]\n";
            diag_log_pf(1, g_strLog.str().c_str());
        }
    	//std::cout << g_strLog.str();
    }
    
    //initial
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);		
	
	diag_log("==== PG DR/IO CALIBRATION END ====================================\n\n");
	//std::cout<<std::endl;
	return bResult;	
}

STEP(PG_DRIO_CAL) {
	pg_drio_cal();
}




bool pg_drio_diag(int Mode = 0) //Mode "0" = Normal , "1" = PGM CalFactor Apply
{
	diag_log("==== PG DR/IO DIAGNOSTIC START ===================================\n");
	bool bResult = true;
	short data[8]       = {0};
	
	float   MeasVio[4][4];    //ADC Read Voltage
	float   MeasVdr[4][4];    //ADC Read Voltage
    float   AGain[2][8];      //ADC Sense Gain
    float   AOffs[2][8];      //ADC Sense Offset	
    float   FGainIO[4];       //Voltage Force Gain
    float   FOffsIO[4];       //Voltage Force Offset	
	float   FGainDR[4];       //Voltage Force Gain
    float   FOffsDR[4];       //Voltage Force Offset	
    
	float SetV[4] = {0.7,1.0,1.3,1.6};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.005;                //CAL : Gain Check , DIAG : Pass/Fail Limit	

	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    } 
    
    //PG-IO/DR Cal Factor Select           
    if (Mode == 0)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_PGPWR; ch++)
        {
            FGainIO[ch] = 1.0;
            FOffsIO[ch] = 0.0;
            FGainDR[ch] = 1.0;
            FOffsDR[ch] = 0.0;
        }
    }
    else if (Mode == 1)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_PGPWR; ch++)
        {
            FGainIO[ch] = pgio_f_gain[ch];
            FOffsIO[ch] = pgio_f_offset[ch];
            FGainDR[ch] = pgdr_f_gain[ch];
            FOffsDR[ch] = pgdr_f_offset[ch];
        }
    }
            
  	//Measure                               
    for(int i = 0; i < 4; i++)
    {
        VIH(ALL, SetV[i]);
        VTH(SetV[i]/2);
        SRON();
        usleep(200000);
        
        if (Mode == 1)
        {
            VIH(IO, SetV[i] * FGainIO[0] + FOffsIO[0] );
            VIH(DR, SetV[i] * FGainDR[0] + FOffsDR[0] );
            SRON();
            usleep(100000);
        } 
        
        diag_readAdc( 0, 10, data); 
        MeasVio[i][0] = MAKE_CODE2VOLT(data[2]) * AGain[0][2] + AOffs[0][2];
        diag_readAdc( 0, 11, data); 
        MeasVdr[i][0] = MAKE_CODE2VOLT(data[2]) * AGain[0][2] + AOffs[0][2];     
        
        if (Mode == 1)
        {
            VIH(IO, SetV[i] * FGainIO[1] + FOffsIO[1] );
            VIH(DR, SetV[i] * FGainDR[1] + FOffsDR[1] );
            SRON();
            usleep(100000);
        }
        
        diag_readAdc( 0, 10, data); 
        MeasVio[i][1] = MAKE_CODE2VOLT(data[3]) * AGain[0][3] + AOffs[0][3];
        diag_readAdc( 0, 11, data); 
        MeasVdr[i][1] = MAKE_CODE2VOLT(data[3]) * AGain[0][3] + AOffs[0][3];
        
        if (Mode == 1)
        {
            VIH(IO, SetV[i] * FGainIO[2] + FOffsIO[2] );
            VIH(DR, SetV[i] * FGainDR[2] + FOffsDR[2] );
            SRON();
            usleep(100000);
        }
        
        diag_readAdc( 1, 10, data); 
        MeasVio[i][2] = MAKE_CODE2VOLT(data[2]) * AGain[1][2] + AOffs[1][2];
        diag_readAdc( 1, 11, data); 
        MeasVdr[i][2] = MAKE_CODE2VOLT(data[2]) * AGain[1][2] + AOffs[1][2];
        
        if (Mode == 1)
        {
            VIH(IO, SetV[i] * FGainIO[3] + FOffsIO[3] );
            VIH(DR, SetV[i] * FGainDR[3] + FOffsDR[3] );
            SRON();
            usleep(100000);
        }
        
        diag_readAdc( 1, 10, data); 
        MeasVio[i][3] = MAKE_CODE2VOLT(data[3]) * AGain[1][3] + AOffs[1][3];       
        diag_readAdc( 1, 11, data); 
        MeasVdr[i][3] = MAKE_CODE2VOLT(data[3]) * AGain[1][3] + AOffs[1][3];
        
        SROF();
        usleep(100000);
    }   
    
  	//Log  
    for(int i = 0; i < MAX_PGPWR; i++)
    {
        for(int idx = 0; idx < 4; idx++)
        {
            g_strLog.str("PG_IO_DIAG ");
            g_strLog << "PG_IO Ch"<< idx << "  Set : "<< FFixed(4) << SetV[i] << "  Read : " << MeasVio[i][idx];
            if((MeasVio[i][idx] < (SetV[i]-limit)) || (MeasVio[i][idx] > (SetV[i]+limit))) {
                bResult = false;
                //g_strLog << " ------- [FAIL]\n";
                diag_log_pf(0, g_strLog.str().c_str());
            }else  {
                //g_strLog << " ------- [PASS]\n";
                diag_log_pf(1, g_strLog.str().c_str());
            }
            //std::cout << g_strLog.str();

            g_strLog.str("PG_DR_DIAG ");
            g_strLog << "PG_DR Ch"<< idx << "  Set : "<< FFixed(4) << SetV[i] << "  Read : " << MeasVdr[i][idx];
            if((MeasVdr[i][idx] < (SetV[i]-limit)) || (MeasVdr[i][idx] > (SetV[i]+limit))) {
                bResult = false;
                //g_strLog << " ------- [FAIL]\n";
                diag_log_pf(0, g_strLog.str().c_str());
            }else  {
                //g_strLog << " ------- [PASS]\n";
                diag_log_pf(1, g_strLog.str().c_str());
            }
            //std::cout << g_strLog.str();
        }
        //std::cout<<std::endl;
        diag_log("\n");
    }   
    
    //initial
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);		
    
	diag_log("==== PG DR/IO DIAGNOSTIC END =====================================\n\n");
	//std::cout<<std::endl;
    return bResult;
}

STEP (PG_DRIO_DIAG) {
	pg_drio_diag(0);
}

STEP (PG_DRIO_DIAG_TEST) {
	pg_drio_diag(1);
}








bool pg_vth_cal()
{
	diag_log("==== PG VTH CALIBRATION START ====================================\n");
	bool bResult = true;
	short data[8] = {0};
	
	float MeasR[2][4];    //ADC Read Voltage
    float AGain[2][8];    //ADC Sense Gain
    float AOffs[2][8];    //ADC Sense Offset	
    float FGain[4];       //Voltage Force Gain
    float FOffs[4];       //Voltage Force Offset	
    
	float SetV[2] = {0.8,1.4};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.01;         //CAL : Gain Check , DIAG : Pass/Fail Limit	
	
	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    } 
    
    //PG-VTH Cal Factor Init		
	for(int ch = 0; ch < MAX_PGPWR; ch++)
    {
		pgvth_f_gain[ch] = 1.0;
		pgvth_f_gain[ch] = 0.0;
		FGain[ch] = 1.0;
        FOffs[ch] = 0.0;
	}
    
    //Measure
	for(int i = 0; i < 2; i++)
    {	
        VIH(ALL, 1.8);
        VTH(SetV[i]);        
        SRON();
        usleep(200000);       
        
        diag_readAdc( 0, 12, data); 
        MeasR[i][0] = MAKE_CODE2VOLT(data[2]) * AGain[0][2] + AOffs[0][2];
        MeasR[i][1] = MAKE_CODE2VOLT(data[3]) * AGain[0][3] + AOffs[0][3];
        
        diag_readAdc( 1, 12, data); 
        MeasR[i][2] = MAKE_CODE2VOLT(data[2]) * AGain[1][2] + AOffs[1][2];
        MeasR[i][3] = MAKE_CODE2VOLT(data[3]) * AGain[1][3] + AOffs[1][3];
        
        SROF();
        usleep(100000);
    }

    //Calculate
	for(int ch = 0; ch < MAX_PGPWR; ch++)
    {
        float X0 = MeasR[0][ch];
        float X1 = MeasR[1][ch];    	
        float Y0 = SetV[0];
        float Y1 = SetV[1];    	    	
        FGain[ch] = ( ( Y1 - Y0 ) / ( X1 - X0 ) );
    	FOffs[ch] = ( ( -FGain[ch] * X0 ) + Y0 ) ;    	
        pgvth_f_gain[ch]    = FGain[ch];
        pgvth_f_offset[ch]  = FOffs[ch];
    }    
    
    //Log
	for(int ch = 0; ch < MAX_PGPWR; ch++)
    {
        g_strLog.str("PG_VTH_CAL ");
        g_strLog << "PG_VTH Ch"<< ch << "  CAL : "<< FFixed(4) << MeasR[0][ch] << " " << FFixed(4) << MeasR[1][ch];
        g_strLog << "  Gain : "<< FGain[ch] << "  Offset : "<< FOffs[ch];       
		if((FGain[ch] < 1.0-limit) || (FGain[ch] > 1.0+limit))  {
            bResult = false;
            //g_strLog << " ------- [FAIL]\n";
            diag_log_pf(0, g_strLog.str().c_str());
        }else  {
            //g_strLog << " ------- [PASS]\n";
            diag_log_pf(1, g_strLog.str().c_str());
        }
    	//std::cout << g_strLog.str();
    }  
    
    //initial
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);		
	
	diag_log("==== PG VTH CALIBRATION END ======================================\n\n");
	//std::cout<<std::endl;
	return bResult;	
}

STEP(PG_VTH_CAL) {
	pg_vth_cal();
}




bool pg_vth_diag(int Mode = 0) //Mode "0" = Normal , "1" = PGM CalFactor Apply
{
	diag_log("==== PG VTH DIAGNOSTIC START =====================================\n");
	bool bResult = true;
	short data[8] = {0};
	
	float MeasR[4][4];    //ADC Read Voltage
    float AGain[2][8];    //ADC Sense Gain
    float AOffs[2][8];    //ADC Sense Offset	
    float FGain[4];       //Voltage Force Gain
    float FOffs[4];       //Voltage Force Offset	
    
	float SetV[4] = {0.8,1.0,1.2,1.4};  //CAL : 2-Point , DIAG : Test Condition
	float limit = 0.02;                //CAL : Gain Check , DIAG : Pass/Fail Limit	

	//ADC Cal Factor Apply
    for(int ch = 0; ch < 8; ch++)
    {
        AGain[0][ch] = adc_s_gain  [0][ch];
        AOffs[0][ch] = adc_s_offset[0][ch];
        AGain[1][ch] = adc_s_gain  [1][ch];
        AOffs[1][ch] = adc_s_offset[1][ch];
    }
    
    //PG-VTH Cal Factor Select           
    if (Mode == 0)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_PGPWR; ch++)
        {
            FGain[ch] = 1.0;
            FOffs[ch] = 0.0;
        }
    }
    else if (Mode == 1)    //Mode "0" = Normal , "1" = PGM CalFactor Apply
    {
        for(int ch = 0; ch < MAX_PGPWR; ch++)
        {
            FGain[ch] = pgvth_f_gain[ch];
            FOffs[ch] = pgvth_f_offset[ch];
        }
    }
    
	//Measure	 
	for(int i = 0; i < 4; i++)
    {	
        VIH(ALL, 1.8);
        VTH(SetV[i]);        
        SRON();
        usleep(200000);
                
        if (Mode == 1)
        {
            VTH( SetV[i] * FGain[0] + FOffs[0] );
            SRON();
            usleep(100000);               
        }
        
        diag_readAdc( 0, 12, data); 
        MeasR[i][0] = MAKE_CODE2VOLT(data[2]) * AGain[0][2] + AOffs[0][2];
        
        if (Mode == 1)
        {
            VTH( SetV[i] * FGain[1] + FOffs[1] );
            SRON();
            usleep(100000);               
        }
        
        diag_readAdc( 0, 12, data); 
        MeasR[i][1] = MAKE_CODE2VOLT(data[3]) * AGain[0][3] + AOffs[0][3];
        
        if (Mode == 1)
        {
            VTH( SetV[i] * FGain[2] + FOffs[2] );
            SRON();
            usleep(100000);               
        }
        
        diag_readAdc( 1, 12, data); 
        MeasR[i][2] = MAKE_CODE2VOLT(data[2]) * AGain[1][2] + AOffs[1][2];
            
        if (Mode == 1)
        {
            VTH( SetV[i] * FGain[3] + FOffs[3] );
            SRON();
            usleep(100000);               
        }
        
        diag_readAdc( 1, 12, data); 
        MeasR[i][3] = MAKE_CODE2VOLT(data[3]) * AGain[1][3] + AOffs[1][3];
                                                
        SROF();
        usleep(100000);
    }

 	//Log	
    for(int i = 0; i < MAX_PGPWR; i++)
    {
        for(int idx = 0; idx < 4; idx++)
        {
            g_strLog.str("IO VTH Voltage Test ");
            g_strLog << "VTH Ch"<< idx << "  Set : "<< FFixed(4) << SetV[i] << "  Read : " << MeasR[i][idx];
            if((MeasR[i][idx] < (SetV[i]-limit)) || (MeasR[i][idx] > (SetV[i]+limit))) {
                bResult = false;
                //g_strLog << " ------- [FAIL]\n";
                diag_log_pf(0, g_strLog.str().c_str());
            }else  {
                //g_strLog << " ------- [PASS]\n";
                diag_log_pf(1, g_strLog.str().c_str());
            }
            //std::cout << g_strLog.str();
        }        
        //std::cout<<std::endl;
        diag_log("\n");
    }	
	
    //initial
    VIH     (ALL, 1.0);
    DUTVTT  (ALL, 0.5);
    VTT     (ALL, 0.5);
    VTH     (     0.5);
    DUTVREF1(     0.0);
    DUTVREF2(     0.0);
    SRON();
    usleep(100000);
    SROF();
    usleep(100000);		
    
	diag_log("==== PG VTH DIAGNOSTIC END =======================================\n\n");
	//std::cout<<std::endl;
    return bResult;
}

STEP (PG_VTH_DIAG)  {
	pg_vth_diag(0);
}

STEP (PG_VTH_DIAG_TEST)  {
	pg_vth_diag(1);
}


void initRefData()
{
    for(int ch = 0; ch < 8; ch++)
    {
        adc_s_gain  [0][ch] = 1.0;
        adc_s_offset[0][ch] = 0.0;
        adc_s_gain  [1][ch] = 1.0;
        adc_s_offset[1][ch] = 0.0;
    } 
	

    for(int ch = 0; ch < 20; ch++) {
        dps_f_gain[ch] = 1.0;       //DPS Voltage Force
        dps_f_offset[ch] = 0.0;     //DPS Voltage Force

        dps_s_gain[ch] = 1.0;       //DPS Voltage Sense
        dps_s_offset[ch] = 0.0;     //DPS Voltage Sense

        dps_i_gain[ch] = 1.0;       //DPS Current Sense
        dps_i_offset[ch] = 0.0;     //DPS Current Sense
    }


    for(int ch = 0; ch < 16; ch++) {
        dutvtt_f_gain[ch] = 1.0;    //DUTVTT Voltage Force
        dutvtt_f_offset[ch] = 0.0;  //DUTVTT Voltage Force

        iovtt_f_gain[ch] = 1.0;     //IOVTT Voltage Force
        iovtt_f_offset[ch] = 0.0;   //IOVTT Voltage Force

        dutvref_f_gain[ch] = 1.0;   //DUTVREF Voltage Force
        dutvref_f_offset[ch] = 0.0; //DUTVREF Voltage Force
    }

    for(int ch = 0; ch < 4; ch++) {
        pgio_f_gain[ch] = 1.0;       //PG-IO Voltage Force
        pgio_f_offset[ch] = 0.0;     //PG-IO Voltage Force

        pgdr_f_gain[ch] = 1.0;       //PG-DR Voltage Force
        pgdr_f_offset[ch] = 0.0;     //PG-DR Voltage Force

        pgvth_f_gain[ch] = 1.0;      //PG-VTH Voltage Force
        pgvth_f_offset[ch] = 0.0;    //PG-VTH Voltage Force
    }

}




void initAdcCalData()
{
    diag_log("============= %s ===========\n", __FUNCTION__);
	stCalFactor calData;
	calData.gain = 1.0;
	calData.offset = 0.0;
	calData.offset2 = 0.0;
	
	adc_calData[0].clear();
	adc_calData[1].clear();
		
	//// ADC Cal Data Make
	for(int idx = 0; idx < 8; idx++) {
		calData.gain   = 1.0;
		calData.offset = 0.0;
        adc_calData[0].push_back(calData);
        
        calData.gain   = 1.0;
		calData.offset = 0.0;
        adc_calData[1].push_back(calData);
    }
    setAdcCalData(adc_calData); 
}
    
STEP (INIT_ADC_CAL_DATA)  {
    //printf("============= %s ===========\n", __FUNCTION__);
	initAdcCalData();
}





void makeAdcCalData()
{
	stCalFactor calData;
	calData.gain = 1.0;
	calData.offset = 0.0;
	calData.offset2 = 0.0;
	
	adc_calData[0].clear();
	adc_calData[1].clear();
		
	//// ADC Cal Data Make
	for(int idx = 0; idx < 8; idx++) {
		calData.gain   = adc_s_gain[0][idx];
		calData.offset = adc_s_offset[0][idx];
        adc_calData[0].push_back(calData);
        
        calData.gain   = adc_s_gain[1][idx];
		calData.offset = adc_s_offset[1][idx];
        adc_calData[1].push_back(calData);
    }
    setAdcCalData(adc_calData); 
}
    
STEP (MAKE_ADC_CAL_DATA)  {
	makeAdcCalData();
}
        
    
    
    


void initDacCalData()
{   
	stCalFactor calData;
	calData.gain = 1.0;
	calData.offset = 0.0;
	calData.offset2 = 0.0;
		
	dac_calData[0].clear();
	dac_calData[1].clear();    
    
    //// DAC Cal Data Make
    dac_calData[0].push_back(calData); //PG0 IO
    dac_calData[0].push_back(calData); //PG0 DR
    dac_calData[0].push_back(calData); //PG0 VTH
    dac_calData[0].push_back(calData); //PG1 IO
    dac_calData[0].push_back(calData); //PG1 DR
    dac_calData[0].push_back(calData); //PG1 VTH
    dac_calData[0].push_back(calData); //Not Used
    dac_calData[0].push_back(calData); //Not Used
    
    for(int idx = 0 ; idx < 8; idx++) {
        dac_calData[0].push_back(calData); //iovtt
    }
    
    for(int idx = 0 ; idx < 8; idx++) {
        dac_calData[0].push_back(calData); //vtt
    }
    
    for(int idx = 0 ; idx < 8; idx++) {
        dac_calData[0].push_back(calData); //vtt
    }
    
    //// DAC Cal Data Make
    dac_calData[1].push_back(calData); //PG0 IO
    dac_calData[1].push_back(calData); //PG0 DR
    dac_calData[1].push_back(calData); //PG0 VTH
    dac_calData[1].push_back(calData); //PG1 IO
    dac_calData[1].push_back(calData); //PG1 DR
    dac_calData[1].push_back(calData); //PG1 VTH
    dac_calData[1].push_back(calData); //Not Used
    dac_calData[1].push_back(calData); //Not Used
    
    for(int idx = 0 ; idx < 8; idx++) {
        dac_calData[1].push_back(calData); //iovtt
    }
    
    for(int idx = 0 ; idx < 8; idx++) {
        dac_calData[1].push_back(calData); //vtt
    }
    
    for(int idx = 0 ; idx < 8; idx++) {
        dac_calData[1].push_back(calData); //vtt
    }
    
    setDacCalData(dac_calData);	
    
}

STEP (INIT_DAC_CAL_DATA)  {
	initDacCalData();
}
    
    
void makeDacCalData()
{   
	stCalFactor calData;
	calData.gain = 1.0;
	calData.offset = 0.0;
	calData.offset2 = 0.0;
		
	dac_calData[0].clear();
	dac_calData[1].clear();    
    
    //// DAC Cal Data Make
    calData.gain   = pgio_f_gain[0];
    calData.offset = pgio_f_offset[0];
    dac_calData[0].push_back(calData); //PG0 IO
    
    calData.gain   = pgdr_f_gain[0];
    calData.offset = pgdr_f_offset[0];
    dac_calData[0].push_back(calData); //PG0 DR
    
    calData.gain   = pgvth_f_gain[0];
    calData.offset = pgvth_f_offset[0];
    dac_calData[0].push_back(calData); //PG0 VTH
    
    
    calData.gain   = pgio_f_gain[1];
    calData.offset = pgio_f_offset[1];
    dac_calData[0].push_back(calData); //PG1 IO
    
    calData.gain   = pgdr_f_gain[1];
    calData.offset = pgdr_f_offset[1];
    dac_calData[0].push_back(calData); //PG1 DR
    
    calData.gain   = pgvth_f_gain[1];
    calData.offset = pgvth_f_offset[1];
    dac_calData[0].push_back(calData); //PG1 VTH


    calData.gain   = 1.0;
    calData.offset = 0.0;
    dac_calData[0].push_back(calData); //Not Used
    
    calData.gain   = 1.0;
    calData.offset = 0.0;
    dac_calData[0].push_back(calData); //Not Used
    
    for(int idx = 0 ; idx < 8; idx++) {
        calData.gain   = iovtt_f_gain[idx];
        calData.offset = iovtt_f_offset[idx];
        dac_calData[0].push_back(calData); //iovtt
    }
    
    for(int idx = 0 ; idx < 8; idx++) {
        calData.gain   = dutvtt_f_gain[idx];
        calData.offset = dutvtt_f_offset[idx];
        dac_calData[0].push_back(calData); //vtt
    }
    
    for(int idx = 0 ; idx < 8; idx++) {
        calData.gain   = dutvref_f_gain[idx];
        calData.offset = dutvref_f_offset[idx];
        dac_calData[0].push_back(calData); //vtt
    }
    

    //// DAC Cal Data Make
    calData.gain   = pgio_f_gain[2];
    calData.offset = pgio_f_offset[2];
    dac_calData[1].push_back(calData); //PG0 IO
    
    calData.gain   = pgdr_f_gain[2];
    calData.offset = pgdr_f_offset[2];
    dac_calData[1].push_back(calData); //PG0 DR
    
    calData.gain   = pgvth_f_gain[2];
    calData.offset = pgvth_f_offset[2];
    dac_calData[1].push_back(calData); //PG0 VTH
    
    
    calData.gain   = pgio_f_gain[3];
    calData.offset = pgio_f_offset[3];
    dac_calData[1].push_back(calData); //PG1 IO
    
    calData.gain   = pgdr_f_gain[3];
    calData.offset = pgdr_f_offset[3];
    dac_calData[1].push_back(calData); //PG1 DR
    
    calData.gain   = pgvth_f_gain[3];
    calData.offset = pgvth_f_offset[3];
    dac_calData[1].push_back(calData); //PG1 VTH

    calData.gain   = 1.0;
    calData.offset = 0.0;
    dac_calData[1].push_back(calData); //Not Used
    
    calData.gain   = 1.0;
    calData.offset = 0.0;
    dac_calData[1].push_back(calData); //Not Used
    
    for(int idx = 0 ; idx < 8; idx++) {
        calData.gain   = iovtt_f_gain[idx+8];
        calData.offset = iovtt_f_offset[idx+8];
        dac_calData[1].push_back(calData); //iovtt
    }
    
    for(int idx = 0 ; idx < 8; idx++) {
        calData.gain   = dutvtt_f_gain[idx+8];
        calData.offset = dutvtt_f_offset[idx+8];
        dac_calData[1].push_back(calData); //vtt
    }
    
    for(int idx = 0 ; idx < 8; idx++) {
        calData.gain   = dutvref_f_gain[idx+8];
        calData.offset = dutvref_f_offset[idx+8];
        dac_calData[1].push_back(calData); //vtt
    }

    setDacCalData(dac_calData);
    
}

STEP (MAKE_DAC_CAL_DATA)  {
	makeDacCalData();
}








void initDpsCalData()
{
	stCalFactor calData;
	calData.gain = 1.0;
	calData.offset = 0.0;
	
	dps_vscalData.clear();
	dps_vmcalData.clear();
	dps_imcalData.clear();
	for(int idx = 0 ; idx < MAX_DPS; idx++) {
        dps_vmcalData.push_back(calData);
        dps_imcalData.push_back(calData);
        dps_vscalData.push_back(calData);
    }
    setDpsCalData(dps_vscalData, dps_vmcalData, dps_imcalData); 
}

STEP (INIT_DPS_CAL_DATA)  {
	initDpsCalData();
}







void makeDpsCalData()
{
	stCalFactor calData;
	calData.gain = 1.0;
	calData.offset = 0.0;
	calData.offset2 = 0.0;
	
	dps_vscalData.clear();
	dps_vmcalData.clear();
	dps_imcalData.clear();
	for(int idx = 0 ; idx < MAX_DPS; idx++) {
        calData.gain   = dps_s_gain[idx];
        calData.offset = dps_s_offset[idx];
        dps_vmcalData.push_back(calData);
        
        calData.gain   = dps_i_gain[idx];
        calData.offset = dps_i_offset[idx];
        dps_imcalData.push_back(calData);
        
        calData.gain   = dps_f_gain[idx];
        calData.offset = dps_f_offset[idx];
        dps_vscalData.push_back(calData);
    }
    setDpsCalData(dps_vscalData, dps_vmcalData, dps_imcalData); 
}

STEP (MAKE_DPS_CAL_DATA)  {
	makeDpsCalData();
}


bool MakeCalFile()
{
    FILE * g_pFileCalData = NULL;
    g_pFileCalData = fopen("/star/diag/cal/cal.data", "wb");

    if(g_pFileCalData == NULL){
        diag_log("Cal data file open fail.\n");
        return false;
    }

    int size = 8*2;
    float adcData[8*2];

    // ADC Cal gain write
    for(int nAdc = 0; nAdc < 2; nAdc++) {
        for(int idx = 0; idx < 8; idx++) {
            adcData[(nAdc*8) +idx] = adc_calData[nAdc][idx].gain;
            //printf("ADC %d %d Gain:%f\n", nAdc, idx, adcData[(nAdc*8) +idx]);
        }
        //printf("\n");
    }
    fwrite(adcData, sizeof(adcData), 1, g_pFileCalData);

    printf("\n");

    // ADC Cal offset write
    for(int nAdc = 0; nAdc < 2; nAdc++) {
        for(int idx = 0; idx < 8; idx++) {
            adcData[(nAdc*8) + idx] = adc_calData[nAdc][idx].offset;
            //printf("ADC %d %d offset:%f\n", nAdc, idx, adcData[(nAdc*8) +idx]);
        }
        //printf("\n");
    }
    fwrite(adcData, sizeof(adcData), 1, g_pFileCalData);

    //printf("\n");

    size = 32*2;
    float dacData[32*2];
    // DAC Cal gain write
    for(int nDac = 0; nDac < 2; nDac++) {
        for(int idx = 0; idx < 32; idx++) {
            dacData[(nDac * 32) + idx] = dac_calData[nDac][idx].gain; 
        }
    }
    fwrite(dacData, size * sizeof(float), 1, g_pFileCalData);
    // DAC Cal offset write
    for(int nDac = 0; nDac < 2; nDac++) {
        for(int idx = 0; idx < 32; idx++) {
            dacData[(nDac * 32) + idx] = dac_calData[nDac][idx].offset; 
        }
    }
    fwrite(dacData, size * sizeof(float), 1, g_pFileCalData);


    size = MAX_DPS;
    float dpsData[MAX_DPS];
    // DPS VS Cal gain write
    for(int nDps = 0; nDps < MAX_DPS; nDps++) {
        dpsData[nDps] = dps_vscalData[nDps].gain; 
    }
    fwrite(dpsData, size * sizeof(float), 1, g_pFileCalData);
    // DPS VS Cal offset write
    for(int nDps = 0; nDps < MAX_DPS; nDps++) {
        dpsData[nDps] = dps_vscalData[nDps].offset; 
    }
    fwrite(dpsData, size * sizeof(float), 1, g_pFileCalData);

    // DPS VM Cal gain write
    for(int nDps = 0; nDps < MAX_DPS; nDps++) {
        dpsData[nDps] = dps_vmcalData[nDps].gain; 
    }
    fwrite(dpsData, size * sizeof(float), 1, g_pFileCalData);
    // DPS VM Cal offset write
    for(int nDps = 0; nDps < MAX_DPS; nDps++) {
        dpsData[nDps] = dps_vmcalData[nDps].offset; 
    }
    fwrite(dpsData, size * sizeof(float), 1, g_pFileCalData);

    // DPS VM Cal gain write
    for(int nDps = 0; nDps < MAX_DPS; nDps++) {
        dpsData[nDps] = dps_imcalData[nDps].gain; 
    }
    fwrite(dpsData, size * sizeof(float), 1, g_pFileCalData);
    // DPS VM Cal offset write
    for(int nDps = 0; nDps < MAX_DPS; nDps++) {
        dpsData[nDps] = dps_imcalData[nDps].offset; 
    }
    fwrite(dpsData, size * sizeof(float), 1, g_pFileCalData);

    if(g_pFileCalData)
        fclose(g_pFileCalData);

    diag_log("Successfully created calibration file.\n");
    return true;
}


bool ReadCalFactor()
{
    int size = 8*2;
    float adcData[size];
    FILE * g_pFileCalData = NULL;
    g_pFileCalData = fopen("/star/diag/cal/cal.data", "rb");

    if(g_pFileCalData == NULL){
        printf("Cal data file open fail.\n");
        diag_msg(0, "Cal data file open fail.");
        return false;
    }

    fseek(g_pFileCalData , 0, SEEK_SET);

    // ADC Cal gain read
    fread(adcData, size * sizeof(float), 1, g_pFileCalData);
    for(int nAdc = 0; nAdc < 2; nAdc++) {
        for(int idx = 0; idx < 8; idx++) {
            if(adcData[(nAdc * 8) + idx] > 0.95 && adcData[(nAdc * 8) + idx] < 1.05)
                adc_calData[nAdc][idx].gain = adcData[(nAdc * 8) + idx]; 
            else
                adc_calData[nAdc][idx].gain = 1.0;

            adc_s_gain[nAdc][idx] = adc_calData[nAdc][idx].gain;
            //printf("read cal adc %d %d gain : %0.5f\n", nAdc, idx, adc_calData[nAdc][idx].gain);
        }
    }
    // ADC Cal offset read
    fread(adcData, size * sizeof(float), 1, g_pFileCalData);
    for(int nAdc = 0; nAdc < 2; nAdc++) {
        for(int idx = 0; idx < 8; idx++) {
            if(adcData[(nAdc * 8) + idx] < 0.01 && adcData[(nAdc * 8) + idx] > -0.01)
                adc_calData[nAdc][idx].offset = adcData[(nAdc * 8) + idx]; 
            else
                adc_calData[nAdc][idx].offset = 0.0;

            adc_s_offset[nAdc][idx] = adc_calData[nAdc][idx].offset;
            //printf("read cal adc %d %d offset : %0.5f\n", nAdc, idx, adc_calData[nAdc][idx].offset);
        }
    }

    size = 32*2;
    float dacData[size];
    // DAC Cal gain read
    fread(dacData, size * sizeof(float), 1, g_pFileCalData);
    for(int nDac = 0; nDac < 2; nDac++) {
        for(int idx = 0; idx < 32; idx++) {
            if(dacData[(nDac * 32) + idx] > 0.95 && dacData[(nDac * 32) + idx] < 1.05)
                dac_calData[nDac][idx].gain = dacData[(nDac * 32) + idx]; 
            else
                dac_calData[nDac][idx].gain = 1.0;
            //printf("read dac %d %d gain : %0.5f\n", nDac, idx, dac_calData[nDac][idx].gain);
        }
    }

    // DAC Cal offset read
    fread(dacData, size * sizeof(float), 1, g_pFileCalData);
    for(int nDac = 0; nDac < 2; nDac++) {
        for(int idx = 0; idx < 32; idx++) {
            if(dacData[(nDac * 32) + idx] < 0.02 && dacData[(nDac * 32) + idx] > -0.02)
                dac_calData[nDac][idx].offset = dacData[(nDac * 32) + idx]; 
            else
                dac_calData[nDac][idx].offset = 0.0;
            //printf("read dac %d %d offset : %0.5f\n", nDac, idx, dac_calData[nDac][idx].offset);
        }
    }

    /////////////////// DPS DATA READ ///////////////////////////////
    size = 20;
    float dpsData[size];
    // DPS FV Cal gain read
    fread(dpsData, size * sizeof(float), 1, g_pFileCalData);
    for(int idx = 0; idx < 20; idx++) {
        if(dpsData[idx] > 0.95 && dpsData[idx] < 1.05)
            dps_f_gain[idx] = dpsData[idx];
        else
            dps_f_gain[idx] = 1.0;
        //printf("dps %d gain : %0.5f\n", idx, dps_f_gain[idx]);
    }

    // DPS FV Cal offset read
    fread(dpsData, size * sizeof(float), 1, g_pFileCalData);
    for(int idx = 0; idx < 20; idx++) {
        if(dpsData[idx] < 0.02 && dpsData[idx] > -0.02)
            dps_f_offset[idx] = dpsData[idx];
        else
            dps_f_offset[idx] = 0.0;
        //printf("dps %d offset : %0.5f\n", idx, dps_f_offset[idx]);
    }


    // DPS VM Cal gain read
    fread(dpsData, size * sizeof(float), 1, g_pFileCalData);
    for(int idx = 0; idx < 20; idx++) {
        if(dpsData[idx] > 0.95 && dpsData[idx] < 1.05)
            dps_s_gain[idx] = dpsData[idx];
        else
            dps_s_gain[idx] = 1.0;
        //printf("dps %d gain : %0.5f\n", idx, dps_s_gain[idx]);
    }

    // DPS VM Cal offset read
    fread(dpsData, size * sizeof(float), 1, g_pFileCalData);
    for(int idx = 0; idx < 20; idx++) {
        if(dpsData[idx] < 0.02 && dpsData[idx] > -0.02)
            dps_s_offset[idx] = dpsData[idx];
        else
            dps_s_offset[idx] = 0.0;
        //printf("dps %d offset : %0.5f\n", idx, dps_s_offset[idx]);
    }


    // DPS IM Cal gain read
    fread(dpsData, size * sizeof(float), 1, g_pFileCalData);
    for(int idx = 0; idx < 20; idx++) {
        if(dpsData[idx] > 0.8 && dpsData[idx] < 1.2)
            dps_i_gain[idx] = dpsData[idx];
        else
            dps_i_gain[idx] = 1.0;
        //printf("read dps %d gain : %0.5f\n", idx, dps_i_gain[idx]);
    }

    // DPS IM Cal offset read
    fread(dpsData, size * sizeof(float), 1, g_pFileCalData);
    for(int idx = 0; idx < 20; idx++) {
        if(dpsData[idx] < 0.1 && dpsData[idx] > -0.2)
            dps_i_offset[idx] = dpsData[idx];
        else
            dps_i_offset[idx] = 0.0;
        //printf("read dpc %d offset : %0.5f\n", idx, dps_i_offset[idx]);
    }

    if(g_pFileCalData)
        fclose(g_pFileCalData);



    // DAC Cal Data Channel match
    pgio_f_gain[0]   = dac_calData[0][0].gain;
    pgio_f_offset[0] = dac_calData[0][0].offset;
    //printf("pgio_f  PG0 gain %f offset %f\n", pgio_f_gain[0], pgio_f_offset[0]);
        
    pgdr_f_gain[0]   = dac_calData[0][1].gain;
    pgdr_f_offset[0] = dac_calData[0][1].offset;
    //printf("pgdr_f  PG0 gain %f offset %f\n", pgdr_f_gain[0], pgdr_f_offset[0]);
        
    pgvth_f_gain[0]   = dac_calData[0][2].gain;
    pgvth_f_offset[0] = dac_calData[0][2].offset;
    //printf("pgvth_f PG0 gain %f offset %f\n", pgvth_f_gain[0], pgvth_f_offset[0]);
        
    pgio_f_gain[1]   = dac_calData[0][3].gain;
    pgio_f_offset[1] = dac_calData[0][3].offset;
    //printf("pgio_f  PG1 gain %f offset %f\n", pgio_f_gain[1], pgio_f_offset[1]);
        
    pgdr_f_gain[1]   = dac_calData[0][4].gain;
    pgdr_f_offset[1] = dac_calData[0][4].offset;
    //printf("pgdr_f  PG1 gain %f offset %f\n", pgdr_f_gain[1], pgdr_f_offset[1]);
        
    pgvth_f_gain[1]  = dac_calData[0][5].gain;
    pgvth_f_offset[1]= dac_calData[0][5].offset;
    //printf("pgvth_f PG1 gain %f offset %f\n", pgvth_f_gain[1], pgvth_f_offset[1]);

    //dac_calData[0].push_back(calData); //Not Used 6
    
    //dac_calData[0].push_back(calData); //Not Used 7
    
    for(int idx = 0 ; idx < 8; idx++) {
        iovtt_f_gain[idx]   = dac_calData[0][8 + idx].gain;
        iovtt_f_offset[idx] = dac_calData[0][8 + idx].offset;
        //printf("iovtt_f %d %f %f\n", idx, iovtt_f_gain[idx], iovtt_f_offset[idx]);
    }
    
    for(int idx = 0 ; idx < 8; idx++) {
        dutvtt_f_gain[idx]   = dac_calData[0][16 + idx].gain;
        dutvtt_f_offset[idx] = dac_calData[0][16 + idx].offset;
        //printf("dutvtt_f %d %f %f\n", idx, dutvtt_f_gain[idx], dutvtt_f_offset[idx]);
    }
    
    for(int idx = 0 ; idx < 8; idx++) {
        dutvref_f_gain[idx]   = dac_calData[0][24 + idx].gain;
        dutvref_f_offset[idx] = dac_calData[0][24 + idx].offset;
        //printf("dutvref_f %d %f %f\n", idx, dutvref_f_gain[idx], dutvref_f_offset[idx]);
    }


    pgio_f_gain[2]   = dac_calData[1][0].gain;
    pgio_f_offset[2] = dac_calData[1][0].offset;
    //printf("pgio_f  PG2 gain %f offset %f\n", pgio_f_gain[2], pgio_f_offset[2]);
    
    pgdr_f_gain[2]   = dac_calData[1][1].gain;
    pgdr_f_offset[2] = dac_calData[1][1].offset;
    //printf("pgdr_f  PG2 gain %f offset %f\n", pgdr_f_gain[2], pgdr_f_offset[2]);
    
    pgvth_f_gain[2]   = dac_calData[1][2].gain;
    pgvth_f_offset[2] = dac_calData[1][2].offset;
    //printf("pgvth_f PG2 gain %f offset %f\n", pgvth_f_gain[2], pgvth_f_offset[2]);
    
    
    pgio_f_gain[3]   = dac_calData[1][3].gain;
    pgio_f_offset[3] = dac_calData[1][3].offset;
    //printf("pgio_f  PG3 gain %f offset %f\n", pgio_f_gain[3], pgio_f_offset[3]);
    
    pgdr_f_gain[3]   = dac_calData[1][4].gain;
    pgdr_f_offset[3] = dac_calData[1][4].offset;
    //printf("pgdr_f  PG3 gain %f offset %f\n", pgdr_f_gain[3], pgdr_f_offset[3]);
    
    pgvth_f_gain[3]   = dac_calData[1][5].gain;;
    pgvth_f_offset[3] = dac_calData[1][5].offset;
    //printf("pgvth_f PG2 gain %f offset %f\n", pgvth_f_gain[3], pgvth_f_offset[3]);
    //dac_calData[1].push_back(calData); //PG1 VTH

    //dac_calData[1].push_back(calData); //Not Used 6
    
    //dac_calData[1].push_back(calData); //Not Used 7

    for(int idx = 0 ; idx < 8; idx++) {
        iovtt_f_gain[idx + 8]   = dac_calData[1][8 + idx].gain;
        iovtt_f_offset[idx + 8] = dac_calData[1][8 + idx].offset;
        //printf("iovtt_f %d %f %f\n", idx+8, iovtt_f_gain[idx + 8], iovtt_f_offset[idx + 8]);
    }
    
    for(int idx = 0 ; idx < 8; idx++) {
        dutvtt_f_gain[idx + 8]   = dac_calData[1][16 + idx].gain;
        dutvtt_f_offset[idx + 8] = dac_calData[1][16 + idx].offset;
        //printf("dutvtt_f %d %f %f\n", idx+8, dutvtt_f_gain[idx+8], dutvtt_f_offset[idx+8]);
    }
    
    for(int idx = 0 ; idx < 8; idx++) {
        dutvref_f_gain[idx + 8]   = dac_calData[1][24 + idx].gain;
        dutvref_f_offset[idx + 8] = dac_calData[1][24 + idx].offset;
        //printf("dutvref_f %d %f %f\n", idx+8, dutvref_f_gain[idx+8], dutvref_f_offset[idx+8]);
    }

    setAdcCalData(adc_calData);
    setDacCalData(dac_calData);
    setDacCalData(dac_calData);

    return true;
}



void slot_id_read()
{
	uint id = GET_SLOT_ID();
	printf("=================== SLOT ID ==================\n");
	printf("Slot ID : %d(%X)\n", id,id);
	printf("==============================================\n");
}








void bib_id_read()
{
	printf("=================== BIB ID ==================\n");
	
	printf("BIB ID : ");
	
	/*
	writeIO(0x024 , 0x1);  //Select
	usleep(1000000);
	printf(" %08x ", readIO(0, 0x24));
	
	writeIO(0x024 , 0x2);  //Select
	usleep(1000000);
	printf(" %08x ", readIO(0, 0x24));
	
	writeIO(0x024 , 0x4);  //Select
	usleep(1000000);
	printf(" %08x", readIO(0, 0x24));
	
	writeIO(0x024 , 0x8);  //Select
	usleep(1000000);
	printf(" %08x ", readIO(0, 0x24));
	
	writeIO(0x024 , 0x10);  //Select
	usleep(1000000);
	printf(" %08x ", readIO(0, 0x24));
	
	writeIO(0x024 , 0x20);  //Select
	usleep(1000000);
	printf(" %08x ", readIO(0, 0x24));
	
	writeIO(0x024 , 0x40);  //Select
	usleep(1000000);
	printf(" %08x", readIO(0, 0x24));
	
	writeIO(0x024 , 0x80);  //Select
	usleep(1000000);
	printf(" %08x ", readIO(0, 0x24));
	*/
			
	writeIO(0x024 , 0x1);  //Select
	usleep(1000000);
	printf(" %08x ", readIO(0, 0x28));
	writeIO(0x024 , 0x0);  //Select
		
	writeIO(0x024 , 0x2);  //Select
	usleep(1000000);
	printf(" %08x ", readIO(0, 0x28));
	writeIO(0x024 , 0x0);  //Select
	
	writeIO(0x024 , 0x4);  //Select
	usleep(1000000);
	printf(" %08x", readIO(0, 0x28));
	writeIO(0x024 , 0x0);  //Select
	
	writeIO(0x024 , 0x8);  //Select
	usleep(1000000);
	printf(" %08x ", readIO(0, 0x28));
	writeIO(0x024 , 0x0);  //Select
	
	writeIO(0x024 , 0x10);  //Select
	usleep(1000000);
	printf(" %08x ", readIO(0, 0x28));
	writeIO(0x024 , 0x0);  //Select
	
	writeIO(0x024 , 0x20);  //Select
	usleep(1000000);
	printf(" %08x ", readIO(0, 0x28));
	writeIO(0x024 , 0x40);  //Select
	
	writeIO(0x024 , 0x40);  //Select
	usleep(1000000);
	printf(" %08x", readIO(0, 0x28));
	writeIO(0x024 , 0x0);  //Select
	
	writeIO(0x024 , 0x80);  //Select
	usleep(1000000);
	printf(" %08x ", readIO(0, 0x28));
	writeIO(0x024 , 0x0);  //Select	
	
	
	
	printf("\n");
    printf("==============================================\n");	
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
// Test Condition
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   

#if 0
void TEST() 
{	
	//writeFPGA("/home/pc/samba/PciOpenTest/pg_top_240223.bit",1);  // 1 = pg upload 0 = tg upload
    //writeFPGA("/home/pc/samba/PciOpenTest/pg_top_241212.bit",1);  // 1 = pg upload 0 = tg upload
    //writeFPGA("/home/pc/samba/PciOpenTest/pg_top_250226.bit",1);  // 1 = pg upload 0 = tg upload
    
    /*
	//ADC Reset
	writeIO(0x4020, 0x020); //ADC Chip Reset (Clear)
	usleep(100000);
	writeIO(0x4020, 0x100); //ADC IP Reset
	usleep(100000);                                                                                                            
	writeIO(0x402C, 0x6);   //OS MAX
	usleep(100000);                                                                                                            
	writeIO(0x4020, 0x010); //ADC Conversion Operate
	usleep(100000);
	*/
	
	/*
	//DAC Reset
	writeIO(0xA020, 0x5000000); //DAC Chip Reset
	usleep(100000);
	writeIO(0xA020, 0x9000000); //DAC IP Reset
	usleep(100000);
	*/
	
	

	// NORMAL MODE ///////////////
	//CALIBRATION INIT /////////////////
    RUN_STEP(INIT_ADC_CAL_DATA);
    RUN_STEP(INIT_DPS_CAL_DATA);
    RUN_STEP(INIT_DAC_CAL_DATA);
    
    //CALIBRATION START /////////////////
	RUN_STEP(ADC_CAL);   //SW 구조상 DIAG시에만 ADC CAL 적용함
    RUN_STEP(DPS_CAL);
    RUN_STEP(DPS_I_CAL);
    RUN_STEP(DUTVTT_CAL);
    RUN_STEP(IOVTT_CAL);
    RUN_STEP(DUTVREF_CAL);
    RUN_STEP(PG_DRIO_CAL);
    RUN_STEP(PG_VTH_CAL);   
    
    //CALIBRATION APLLY /////////////////
    RUN_STEP(MAKE_ADC_CAL_DATA);
    RUN_STEP(MAKE_DPS_CAL_DATA);    
    RUN_STEP(MAKE_DAC_CAL_DATA);    
    
            
    //DIAGNOSTIC /////////////////
    RUN_STEP(ADC_DIAG);
    RUN_STEP(SYS_VOLT_DIAG);
    RUN_STEP(DPS_DIAG);
    RUN_STEP(DPS_I_DIAG);
    RUN_STEP(DUTVTT_DIAG);
    RUN_STEP(IOVTT_DIAG);
    RUN_STEP(DUTVREF_DIAG);
    RUN_STEP(PG_DRIO_DIAG);
    RUN_STEP(PG_VTH_DIAG);
    
    
    
    
    //TEST MODE /////////////////
    //RUN_STEP(ADC_DIAG);
    //RUN_STEP(SYS_VOLT_DIAG);   
    //RUN_STEP(DPS_DIAG_TEST);    
    //RUN_STEP(DPS_I_DIAG_TEST);   
    //RUN_STEP(DUTVTT_DIAG_TEST); 
    //RUN_STEP(IOVTT_DIAG_TEST);
    //RUN_STEP(DUTVREF_DIAG_TEST);
    //RUN_STEP(PG_DRIO_DIAG_TEST);
    //RUN_STEP(PG_VTH_DIAG_TEST);



    //slot_id_read();
    
    //bib_id_read();
    
    //PYD_TEST();

}

#endif



