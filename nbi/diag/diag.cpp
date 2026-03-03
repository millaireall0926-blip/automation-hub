#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "STAR_TEST.h"
#include "DIAG_TEST.h"


#define DIAG_BUILD_DATE		"2025-07-25"
#define DIAG_VER		    "V0.2"
/****************************************************************************************************************************
*****************************************************************************************************************************
2025.07.15 [ rev 0.2 ] : Board Temp Feature Add

2023.12.19 [ rev 0.1 ] : vih, vil calibration negative voltage change ( -0.1 -> 0 )

*****************************************************************************************************************************
*****************************************************************************************************************************/
/****************************************************************************************************************************
    diag -diag -darg=f 
          -diag : diag option excute
          -darg : 0x1=SYS, 0x2=PG, 0x4=Power, 0x8=Cal, 0x10=Shmoo 0x20=TDR 0xff=ALL
*****************************************************************************************************************************/

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

//              dl dh   dd  x   y   z   Sc  CYP
// type BIT :   0   1   2   3   4   5   6   7
//              dl dh   dd  x   y   z   Sc  CYP
// type BIT :   0   1   2   3   4   5   6   7
int g_nCompRet = 0;

bool compFm(const char* name, int type, int size, ...)
{

    diag_readFmData();
    unsigned int* compArr = new unsigned int[size];

    va_list ap;
    va_start(ap, size);
    for(int i=0; i<size; i++)
        compArr[i] = va_arg(ap, unsigned int);
    va_end(ap);

    int fmRead = size;
    int nRet = 0;
    for (int i = 0; i<size; i++ )
    {
        nRet = 0;
        DIAG_FM* pFmH = (DIAG_FM*)diag_getFmDataH(i);
        DIAG_FM* pFmL = (DIAG_FM*)diag_getFmDataL(i);
        

        if (type & 0x3){   // type =  DATA H, L
            for (int j =0; j<4; j++){
                unsigned short readh = (pFmH->ioComp[j*2]) | (pFmH->ioComp[j*2+1]<<8);
                unsigned short readl = (pFmL->ioComp[j*2]) | (pFmL->ioComp[j*2+1]<<8);
//                unsigned short comp = compArr[i] & 0xcbfd;
                unsigned short comp = compArr[i] & 0xffff;

//                readh = readh & 0xcbfd;
//                readl = readl & 0xcbfd;
                readh = readh & 0xffff;
                readl = readl & 0xffff;

                if (type & 0x1){
                    if (comp != readh){
                        diag_log_pf(0, "[%s] ioH Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, j*2, readh, compArr[i]);
                        nRet = type;
                    }
                }

                if (type & 0x2){
                    if (comp != readl){
                        diag_log_pf(0, "[%s] ioL Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name,  i, j*2, readl, compArr[i]);
                        nRet = type;
                    }
                }
                if (nRet != 0)
                    break;
            }
        }
        if (type == 0x4)        // Drv D
        {
            unsigned short readh = (pFmH->ioDrv[0]) | (pFmH->ioDrv[1]<<8);
            unsigned short readl = (pFmL->ioDrv[0]) | (pFmL->ioDrv[1]<<8);
            unsigned short comp = compArr[i] & 0xffff;
            if (comp != readh){
                diag_log_pf(0, "[%s] drv Dh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 0, readh, compArr[i]);
                nRet = type;
            }
            if (comp != readl){
                diag_log_pf(0, "[%s] drv Dl Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 0, readl, compArr[i]);
                nRet = type;
            }
        }
        if (type & 0x18)        // X
        {
            unsigned short readh = (pFmH->adrDrv[0]) | (pFmH->adrDrv[1]<<8);
            unsigned short readl = (pFmL->adrDrv[0]) | (pFmL->adrDrv[1]<<8);
            unsigned short comp = compArr[i] & 0xffff;

            if (type & 0x8){
				if (comp != readh){
                diag_log_pf(0, "[%s] drv Xh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 0, readh, compArr[i]);
                nRet = type;
				}
			}
            if (type & 0x10){
				if (comp != readl){
                diag_log_pf(0, "[%s] drv Xl Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 0, readl, compArr[i]);
                nRet = type;
				}
			}
        }
        if (type == 0x20)        // Y
        {
            unsigned short readh = (pFmH->adrDrv[2]) | (pFmH->adrDrv[3]<<8);
            unsigned short readl = (pFmL->adrDrv[2]) | (pFmL->adrDrv[3]<<8);
            unsigned short comp = compArr[i] & 0xffff;
             if (comp != readh){
                 diag_log_pf(0, "[%s] drv Yh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 2, readh, compArr[i]);
                 nRet = type;
             }
             if (comp != readl){
                 diag_log_pf(0, "[%s] drv Yl Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 2, readl, compArr[i]);
                 nRet = type;
             }
        }
        if (type == 0x40)       // Z
        {
            unsigned short readh = (pFmH->adrDrv[4]);
            unsigned short readl = (pFmL->adrDrv[4]); 
            unsigned short comp = compArr[i] & 0xff;
             if (comp != readh){
                 diag_log_pf(0, "[%s] drv Yh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 4, readh, compArr[i]);
                 nRet = type;
             }
             if (comp != readl){
                 diag_log_pf(0, "[%s] drv Yl Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 4, readl, compArr[i]);
                 nRet = type;
             }
        }
        if (type == 0x80)       // Scan
        {
            unsigned char readh = pFmH->scan;
//            unsigned char readl = pFmL->scan;
            unsigned char comp = compArr[i] & 0xff;
             if (comp != readh){
                 diag_log_pf(0, "[%s] drv Yh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 5, readh, compArr[i]);
                 nRet = type;
             }
//             if (comp != readl){
//                 procMsg(0, "[%s] drv Yl Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 5, readl, compArr[i]);
//                 nRet = type;
//             }
        }
        if (type == 0x100)       // cyp
        {
            unsigned int readh = (pFmH->adrDrv[0]) | (pFmH->adrDrv[1]<<8) | (pFmH->adrDrv[2])<<16 | (pFmH->adrDrv[3]<<24);
            unsigned int readl = (pFmL->adrDrv[0]) | (pFmL->adrDrv[1]<<8) | (pFmL->adrDrv[2])<<16 | (pFmL->adrDrv[3]<<24);
            unsigned int comp = compArr[i] & 0xffffffff;
             if (comp != readh){
                 diag_log_pf(0, "[%s] drv CYP Comp Error adr:%d, read:%04x, comp:%04x", name, i, readh, compArr[i]);
                 nRet = type;
             }
             if (comp != readl){
                 diag_log_pf(0, "[%s] drv CYP Comp Error adr:%d, read:%04x, comp:%04x", name, i, readl, compArr[i]);
                 nRet = type;
             }
        }
        if (type & 0x600)        // addr0~32 read
        {
            unsigned int readh = (pFmH->adrDrv[0]) | (pFmH->adrDrv[1]<<8)| (pFmH->adrDrv[2]<<16)| (pFmH->adrDrv[3]<<24);
            unsigned int readl = (pFmL->adrDrv[0]) | (pFmL->adrDrv[1]<<8)| (pFmL->adrDrv[2]<<16)| (pFmL->adrDrv[3]<<24);
            unsigned int comp = compArr[i] & 0xffffffff;
  
            if (type & 0x200){
				if (comp != readh){
                 diag_log_pf(0, "[%s] drv Xh Comp Error adr:%d, pos:%d, read:%08x, comp:%08x", name, i, 0, readh, compArr[i]);
                 nRet = type;
				}
			}
            if (type & 0x400){
				if (comp != readl){
                 diag_log_pf(0, "[%s] drv Xl Comp Error adr:%d, pos:%d, read:%08x, comp:%08x", name, i, 0, readl, compArr[i]);
                 nRet = type;
				}
			}
        }
        if (nRet){
            diag_showDrv(i, 1);
            diag_showData(i, 1);
            break;
        }
    }
    delete [] compArr;
    if (nRet == 0)
    {
        diag_log_pf(1, "[%20s]", name);
    }

    g_nCompRet |= nRet;
    return nRet;
}


void pg_shmoo_chk(int stype, int cpe_cnt,  ...)
{
    
	int read_dq_high[cpe_cnt],read_dq_low[cpe_cnt];
	int expect_dq_high[cpe_cnt],expect_dq_low[cpe_cnt];
	int fail_cnt_high = 0;
	int fail_cnt_low = 0;

	va_list ap;
	va_start(ap, cpe_cnt);

	for(int i=0; i<cpe_cnt; i++)
 		expect_dq_high[i] = va_arg(ap, int);
 	for(int i=0; i<cpe_cnt; i++)
		expect_dq_low[i] = va_arg(ap, int);
    va_end(ap);
	
 //   fm_readFmData();
    int fmRead = diag_readFmData();
	
	for (int i = 0; i<fmRead; i++ )
    {
        DIAG_FM* pFmH = (DIAG_FM*)diag_getFmDataH(i);
        DIAG_FM* pFmL = (DIAG_FM*)diag_getFmDataL(i);

		for (int j =0; j<3; j++)
        {
			read_dq_high[j] = (pFmH->ioComp[j*2]) | (pFmH->ioComp[j*2+1]<<8) ;
			read_dq_low[j] = (pFmL->ioComp[j*2]) | (pFmL->ioComp[j*2+1]<<8) ;

			if(!(expect_dq_high[i] == read_dq_high[j]))
				fail_cnt_high++;
			if(!(expect_dq_low[i] == read_dq_low[j]))
				fail_cnt_low++;
		}
	}
    if( stype == 0)
    {
    	if(fail_cnt_high > 0)
    		diag_log("*");    
	    else
		    diag_log("P");
    }
    else
    {
    	if(fail_cnt_low > 0)
    		diag_log("*");    
	    else
		    diag_log("P");
    }
   // diag_clearFmMemory();
}


void pg_strb_cal(int stype, int cpe_cnt,  ...)
{
    
	int read_dq_high[cpe_cnt],read_dq_low[cpe_cnt];
	int expect_dq_high[cpe_cnt],expect_dq_low[cpe_cnt];
	int fail_cnt_high = 0;
	int fail_cnt_low = 0;

	va_list ap;
	va_start(ap, cpe_cnt);

	for(int i=0; i<cpe_cnt; i++)
 		expect_dq_high[i] = va_arg(ap, int);
 	for(int i=0; i<cpe_cnt; i++)
		expect_dq_low[i] = va_arg(ap, int);
    va_end(ap);
	
 //   fm_readFmData();
    int fmRead = diag_readFmData();
	
	for (int i = 0; i<fmRead; i++ )
    {
        DIAG_FM* pFmH = (DIAG_FM*)diag_getFmDataH(i);
        DIAG_FM* pFmL = (DIAG_FM*)diag_getFmDataL(i);

		for (int j =0; j<3; j++)
        {
			read_dq_high[j] = (pFmH->ioComp[j*2]) | (pFmH->ioComp[j*2+1]<<8) ;
			read_dq_low[j] = (pFmL->ioComp[j*2]) | (pFmL->ioComp[j*2+1]<<8) ;

            if( stype == 0)
            {
			    if(!(expect_dq_high[i] == read_dq_high[j]))
				    fail_cnt_high++;
            }
            if( stype == 1)
            {
			    if(!(expect_dq_high[i] == read_dq_low[j]))
				    fail_cnt_low++;
            }
            if( stype == 2)
            {
			    if(!(expect_dq_low[i] == read_dq_high[j]))
				    fail_cnt_high++;
            }
            if( stype == 3)
            {
			    if(!(expect_dq_low[i] == read_dq_low[j]))
				    fail_cnt_low++;
            }
		}
	}
    if(( stype == 0) || ( stype == 2))
    {
    	if(fail_cnt_high > 0)
    		diag_log("*");    
	    else
		    diag_log("P");
    }
    if(( stype == 1) || ( stype == 3))
    {
    	if(fail_cnt_low > 0)
    		diag_log("*");    
	    else
		    diag_log("P");
    }
   // diag_clearFmMemory();
}



 void load_bib(const char* bib_name)
{
/*
    BIB_SIZE(12 ,12,  8, 1);
        //     row0     row1     row2     row3     row4      row5      row6      row7      row8      row9      row10     row11
     	BIB(11,5=32-39, 5=40-47, 5=48-55, 5=56-63, 11=32-39, 11=40-47, 11=48-55, 11=56-63, 17=32-39, 17=40-47, 17=48-55, 17=56-63);    //col12
	    BIB(10,4=32-39, 4=40-47, 4=48-55, 4=56-63, 10=32-39, 10=40-47, 10=48-55, 10=56-63, 16=32-39, 16=40-47, 16=48-55, 16=56-63);    //col10
	 	BIB(9, 3=32-39, 3=40-47, 3=48-55, 3=56-63,  9=32-39,  9=40-47,  9=48-55,  9=56-63, 15=32-39, 15=40-47, 15=48-55, 15=56-63);    //col9
	    BIB(8, 2=32-39, 2=40-47, 2=48-55, 2=56-63,  8=32-39,  8=40-47,  8=48-55,  8=56-63, 14=32-39, 14=40-47, 14=48-55, 14=56-63);    //col8
	    BIB(7, 1=32-39, 1=40-47, 1=48-55, 1=56-63,  7=32-39,  7=40-47,  7=48-55,  7=56-63, 13=32-39, 13=40-47, 13=48-55, 13=56-63);    //col7
	    BIB(6, 0=32-39, 0=40-47, 0=48-55, 0=56-63,  6=32-39,  6=40-47,  6=48-55,  6=56-63, 12=32-39, 12=40-47, 12=48-55, 12=56-63);    //col6
	    
        BIB(5, 5=0-7  , 5=8-15 , 5=16-23, 5=24-31, 11=0-7  , 11=8-15 , 11=16-23, 11=24-31, 17=0-7  , 7=8-15  , 17=16-23, 17=24-31);    //col5
	    BIB(4, 4=0-7  , 4=8-15 , 4=16-23, 4=24-31, 10=0-7  , 10=8-15 , 10=16-23, 10=24-31, 16=0-7  , 6=8-15  , 16=16-23, 16=24-31);    //col4
	    BIB(3, 3=0-7  , 3=8-15 , 3=16-23, 3=24-31,  9=0-7  ,  9=8-15 ,  9=16-23,  9=24-31, 15=0-7  , 5=8-15  , 15=16-23, 15=24-31);    //col3
	    BIB(2, 2=0-7  , 2=8-15 , 2=16-23, 2=24-31,  8=0-7  ,  8=8-15 ,  8=16-23,  8=24-31, 14=0-7  , 4=8-15  , 14=16-23, 14=24-31);    //col2
	    BIB(1, 1=0-7  , 1=8-15 , 1=16-23, 1=24-31,  7=0-7  ,  7=8-15 ,  7=16-23,  7=24-31, 13=0-7  , 3=8-15  , 13=16-23, 13=24-31);    //col1
	    BIB(0, 0=0-7  , 0=8-15 , 0=16-23, 0=24-31,  6=0-7  ,  6=8-15 ,  6=16-23,  6=24-31, 12=0-7  , 2=8-15  , 12=16-23, 12=24-31);    //col0
*/
    BIB_SIZE(6 ,12,  16, 1);
        //     row0     row1     row2     row3     row4      row5      row6      row7      row8      row9      row10     row11
     	BIB(11,5=32-47 , 5=48-63,  11=32-47 , 11=48-63, 17=32-47 , 17=48-63 );    //col12
	    BIB(10,4=32-47 , 4=48-63,  10=32-47 , 10=48-63, 16=32-47 , 16=48-63 );    //col10
	 	BIB(9, 3=32-47 , 3=48-63,   9=32-47 ,  9=48-63, 15=32-47 , 15=48-63 );    //col9
	    BIB(8, 2=32-47 , 2=48-63,   8=32-47 ,  8=48-63, 14=32-47 , 14=48-63 );    //col8
	    BIB(7, 1=32-47 , 1=48-63,   7=32-47 ,  7=48-63, 13=32-47 , 13=48-63 );    //col7
	    BIB(6, 0=32-47 , 0=48-63,   6=32-47 ,  6=48-63, 12=32-47 , 12=48-63 );    //col6
	    
        BIB(5, 5=0-15  , 5=16-31,  11=0-15  , 11=16-31, 17=0-15  , 17=16-31 );    //col5
	    BIB(4, 4=0-15  , 4=16-31,  10=0-15  , 10=16-31, 16=0-15  , 16=16-31 );    //col4
	    BIB(3, 3=0-15  , 3=16-31,   9=0-15  ,  9=16-31, 15=0-15  , 15=16-31 );    //col3
	    BIB(2, 2=0-15  , 2=16-31,   8=0-15  ,  8=16-31, 14=0-15  , 14=16-31 );    //col2
	    BIB(1, 1=0-15  , 1=16-31,   7=0-15  ,  7=16-31, 13=0-15  , 13=16-31 );    //col1
	    BIB(0, 0=0-15  , 0=16-31,   6=0-15  ,  6=16-31, 12=0-15  , 12=16-31 );    //col0



}

void load_scan(const char* scan_name)
{
    if(strcmp(scan_name ,"diag_18scan") == 0 )
    {
        SCAN_TABLE(0 , 0x00000001);    //scan0  Enable
        SCAN_TABLE(1 , 0x00000002);    //scan1  Enable
        SCAN_TABLE(2 , 0x00000004);    //scan2  Enable
        SCAN_TABLE(3 , 0x00000008);    //scan3  Enable
        SCAN_TABLE(4 , 0x00000010);    //scan4  Enable
        SCAN_TABLE(5 , 0x00000020);    //scan5  Enable
        SCAN_TABLE(6 , 0x00000040);    //scan6  Enable
        SCAN_TABLE(7 , 0x00000080);    //scan7  Enable
        SCAN_TABLE(8 , 0x00000100);    //scan8  Enable
        SCAN_TABLE(9 , 0x00000200);    //scan9  Enable
        SCAN_TABLE(10, 0x00000400);    //scan10 Enable
        SCAN_TABLE(11, 0x00000800);    //scan11 Enable
        SCAN_TABLE(12, 0x00001000);    //scan11 Enable
        SCAN_TABLE(13, 0x00002000);    //scan11 Enable
        SCAN_TABLE(14, 0x00004000);    //scan14 Enable
        SCAN_TABLE(15, 0x00008000);    //scan15 Enable
        SCAN_TABLE(16, 0x00010000);    //scan16 Enable
        SCAN_TABLE(17, 0x00020000);    //scan17 Enable
        SCAN_TABLE(18, 0x00000000);    //scan0~31 Disable 
        SCAN_TABLE(19, 0xFFFFFFFF);    //scan0~31 Enable
    }
}
void load_ts(const char* ts_name)
{
        double rate   = 100.0;
	    double bedge0 = 2.0;
	    double cedge0 = (rate/2.0)+bedge0;

        double rate1  = 100.0;
	    double bedge1 = 2.0;
	    double cedge1 = (rate1/2.0)+bedge1;

	    double strb0 = 60.0;
        double strb1 = 5.0;

//                   TS1,   TS2,   TS3,   TS4,   TS5,   TS6,   TS7,   TS8,   TS9,  TS10,  TS11,  TS12,  TS13,  TS14,  TS15,  TS16,
        RATE(	    rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate);

        ECLK( BCLK1  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK1  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK2  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK2  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK3  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK3  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK4  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK4  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK5  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK5  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK6  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK6  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK7  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK7  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK8  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK8  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK9  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK9  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK10 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK10 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK11 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK11 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK12 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK12 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK13 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK13 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK14 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK14 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK15 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK15 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( BCLK16 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( CCLK16 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( DRE    ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
        ECLK( DRE_B  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
        ECLK( STRB   ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 );
        ECLK( STRB_B ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 );
}

void load_ts_of_dpscal(double set_rate)
{
    double set_strb = 100;

    double rate   = set_rate;            // default
	double bedge0 = (rate / 2.0);
	double cedge0 = (rate);

	double bedge1 = rate / 2.0;         // CK
	double cedge1 = rate;

	double bedge2 = rate / 4.0;         // DQ
	double cedge2 = rate*3.0/4.0;

	double bedge3 = 0.0;                 // CA
	double cedge3 = rate / 2.0;

	double bedge4 = rate / 2.0;         // DQS
	double cedge4 = rate;

	double bedge5 = rate/2.0;           // CKE
	double cedge5 = rate;

	double strb0 = 0;
    double strb1 = 50.0;

//                   TS0,  TS1,   TS2,   TS3,   TS4,   TS5,   TS6,   TS7,   TS8,   TS9,  TS10,  TS11,  TS12,  TS13,  TS14,  TS15,
    RATE(	        rate, rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate);

    ECLK( BCLK1  ,bedge1,bedge1,bedge1,bedge1,bedge1,bedge1,bedge1,bedge1,bedge1,bedge1,bedge1,bedge1,bedge1,bedge1,bedge1,bedge1);     // CK
    ECLK( CCLK1  ,cedge1,cedge1,cedge1,cedge1,cedge1,cedge1,cedge1,cedge1,cedge1,cedge1,cedge1,cedge1,cedge1,cedge1,cedge1,cedge1);     
    ECLK( BCLK2  ,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2);     // DQ
    ECLK( CCLK2  ,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2);
    ECLK( BCLK3  ,bedge3,bedge3,bedge3,bedge3,bedge3,bedge3,bedge3,bedge3,bedge3,bedge3,bedge3,bedge3,bedge3,bedge3,bedge3,bedge3);     // CA
    ECLK( CCLK3  ,cedge3,cedge3,cedge3,cedge3,cedge3,cedge3,cedge3,cedge3,cedge3,cedge3,cedge3,cedge3,cedge3,cedge3,cedge3,cedge3);
    ECLK( BCLK4  ,bedge4,bedge4,bedge4,bedge4,bedge4,bedge4,bedge4,bedge4,bedge4,bedge4,bedge4,bedge4,bedge4,bedge4,bedge4,bedge4);     // DQS
    ECLK( CCLK4  ,cedge4,cedge4,cedge4,cedge4,cedge4,cedge4,cedge4,cedge4,cedge4,cedge4,cedge4,cedge4,cedge4,cedge4,cedge4,cedge4);
    ECLK( BCLK5  ,bedge5,bedge5,bedge5,bedge5,bedge5,bedge5,bedge5,bedge5,bedge5,bedge5,bedge5,bedge5,bedge5,bedge5,bedge5,bedge5);     // CKE
    ECLK( CCLK5  ,cedge5,cedge5,cedge5,cedge5,cedge5,cedge5,cedge5,cedge5,cedge5,cedge5,cedge5,cedge5,cedge5,cedge5,cedge5,cedge5);
    ECLK( BCLK6  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);     //Default
    ECLK( CCLK6  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
    ECLK( BCLK7  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK7  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
    ECLK( BCLK8  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK8  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
    ECLK( BCLK9  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK9  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
    ECLK( BCLK10 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK10 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
    ECLK( BCLK11 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK11 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
    ECLK( BCLK12 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK12 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
    ECLK( BCLK13 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK13 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
    ECLK( BCLK14 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK14 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
    ECLK( BCLK15 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK15 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);
    ECLK( BCLK16 ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK16 ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);

    ECLK( DRE    ,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2,bedge2);
    ECLK( DRE_B  ,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2,cedge2);
    ECLK( STRB   ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 );
    ECLK( STRB_B ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 );

    STRB_EDGE(1,bedge1+set_strb ,cedge1+set_strb);
}

void load_feset (const char* feset_name)
{
        TSSET (  ADDR0, DNRZ, ECLK1 );     
        TSSET (  ADDR1, DNRZ, ECLK1 );     
        TSSET (  ADDR2, DNRZ, ECLK1 );     
        TSSET (  ADDR3, DNRZ, ECLK1 );     
        TSSET (  ADDR4, DNRZ, ECLK1 );     
        TSSET (  ADDR5, DNRZ, ECLK1 );     
        TSSET (  ADDR6, DNRZ, ECLK1 );     
        TSSET (  ADDR7, DNRZ, ECLK1 );     
        TSSET (  ADDR8, DNRZ, ECLK1 );     
        TSSET (  ADDR9, DNRZ, ECLK1 );     
        TSSET ( ADDR10, DNRZ, ECLK1 );     
        TSSET ( ADDR11, DNRZ, ECLK1 );     
        TSSET ( ADDR12, DNRZ, ECLK1 );     
        TSSET ( ADDR13, DNRZ, ECLK1 );     
        TSSET ( ADDR14, DNRZ, ECLK1 );     
        TSSET ( ADDR15, DNRZ, ECLK1 );     
        TSSET ( ADDR16, DNRZ, ECLK1 );     
        TSSET ( ADDR17, DNRZ, ECLK1 );     
        TSSET ( ADDR18, DNRZ, ECLK1 );     
        TSSET ( ADDR19, DNRZ, ECLK1 );     
        TSSET ( ADDR20, DNRZ, ECLK1 );     
        TSSET ( ADDR21, DNRZ, ECLK1 );     
        TSSET ( ADDR22, DNRZ, ECLK1 );     
        TSSET ( ADDR23, DNRZ, ECLK1 );     
        TSSET ( ADDR24, DNRZ, ECLK1 );     
        TSSET ( ADDR25, DNRZ, ECLK1 );     
        TSSET ( ADDR26, DNRZ, ECLK1 );     
        TSSET ( ADDR27, DNRZ, ECLK1 );     
        TSSET ( ADDR28, DNRZ, ECLK1 );     
        TSSET ( ADDR29, DNRZ, ECLK1 );     
        TSSET ( ADDR30, DNRZ, ECLK1 );     
        TSSET ( ADDR31, DNRZ, ECLK1 ); 

        TSSET (  CLK0, NRZB, ECLK1 );     
        TSSET (  CLK1, NRZB, ECLK1 );     
        TSSET (  CLK2, NRZB, ECLK1 );     
        TSSET (  CLK3, NRZB, ECLK1 );     
        TSSET (  CLK4, NRZB, ECLK1 );     
        TSSET (  CLK5, NRZB, ECLK1 );     
        TSSET (  CLK6, NRZB, ECLK1 );     
        TSSET (  CLK7, NRZB, ECLK1 );     

        TSSET (  SCAN0,NRZB, ECLK1 );   
        TSSET (  SCAN1,NRZB, ECLK1 );   
        TSSET (  SCAN2,NRZB, ECLK1 );   
        TSSET (  SCAN3,NRZB, ECLK1 );   
        TSSET (  SCAN4,NRZB, ECLK1 );   
        TSSET (  SCAN5,NRZB, ECLK1 );   
        TSSET (  SCAN6,NRZB, ECLK1 );   
        TSSET (  SCAN7,NRZB, ECLK1 );   
        TSSET (  SCAN8,NRZB, ECLK1 );   
        TSSET (  SCAN9,NRZB, ECLK1 );   
        TSSET ( SCAN10,NRZB, ECLK1 );   
        TSSET ( SCAN11,NRZB, ECLK1 );   
        TSSET ( SCAN12,NRZB, ECLK1 );   
        TSSET ( SCAN13,NRZB, ECLK1 );   
        TSSET ( SCAN14,NRZB, ECLK1 );   
        TSSET ( SCAN15,NRZB, ECLK1 );   
        TSSET ( SCAN16,NRZB, ECLK1 );   
        TSSET ( SCAN17,NRZB, ECLK1 );   
        TSSET ( SCAN18,NRZB, ECLK1 );   
        TSSET ( SCAN19,NRZB, ECLK1 );   
        TSSET ( SCAN20,NRZB, ECLK1 );   
        TSSET ( SCAN21,NRZB, ECLK1 );   
        TSSET ( SCAN22,NRZB, ECLK1 );   
        TSSET ( SCAN23,NRZB, ECLK1 );   
        TSSET ( SCAN24,NRZB, ECLK1 );   
        TSSET ( SCAN25,NRZB, ECLK1 );   
        TSSET ( SCAN26,NRZB, ECLK1 );   
        TSSET ( SCAN27,NRZB, ECLK1 );   
        TSSET ( SCAN28,NRZB, ECLK1 );   
        TSSET ( SCAN29,NRZB, ECLK1 );   
        TSSET ( SCAN30,NRZB, ECLK1 );     
        TSSET ( SCAN31,NRZB, ECLK1 );  

        TSSET (  IO0, DNRZ, ECLK1 );   
        TSSET (  IO1, DNRZ, ECLK1 );   
        TSSET (  IO2, DNRZ, ECLK1 );   
        TSSET (  IO3, DNRZ, ECLK1 );   
        TSSET (  IO4, DNRZ, ECLK1 );   
        TSSET (  IO5, DNRZ, ECLK1 );   
        TSSET (  IO6, DNRZ, ECLK1 );   
        TSSET (  IO7, DNRZ, ECLK1 );   
        TSSET (  IO8, DNRZ, ECLK1 );   
        TSSET (  IO9, DNRZ, ECLK1 );   
        TSSET ( IO10, DNRZ, ECLK1 );   
        TSSET ( IO11, DNRZ, ECLK1 );   
        TSSET ( IO12, DNRZ, ECLK1 );   
        TSSET ( IO13, DNRZ, ECLK1 );  
        TSSET ( IO14, DNRZ, ECLK1 );   
        TSSET ( IO15, DNRZ, ECLK1 );   
        TSSET ( IO16, DNRZ, ECLK1 );   
        TSSET ( IO17, DNRZ, ECLK1 );   
        TSSET ( IO18, DNRZ, ECLK1 );   
        TSSET ( IO19, DNRZ, ECLK1 );   
        TSSET ( IO20, DNRZ, ECLK1 );   
        TSSET ( IO21, DNRZ, ECLK1 );   
        TSSET ( IO22, DNRZ, ECLK1 );   
        TSSET ( IO23, DNRZ, ECLK1 );   
        TSSET ( IO24, DNRZ, ECLK1 );   
        TSSET ( IO25, DNRZ, ECLK1 );   
        TSSET ( IO26, DNRZ, ECLK1 );   
        TSSET ( IO27, DNRZ, ECLK1 );   
        TSSET ( IO28, DNRZ, ECLK1 );   
        TSSET ( IO29, DNRZ, ECLK1 );   
        TSSET ( IO30, DNRZ, ECLK1 );   
        TSSET ( IO31, DNRZ, ECLK1 );   
 
        TSSET (  DRE, NRZB);
}

void load_feset_of_dpscal (const char* feset_name)
{
        TSSET (  ADDR0, NRZB, ECLK3 );     
        TSSET (  ADDR1, NRZB, ECLK3 );     
        TSSET (  ADDR2, NRZB, ECLK3 );     
        TSSET (  ADDR3, NRZB, ECLK3 );     
        TSSET (  ADDR4, NRZB, ECLK3 );     
        TSSET (  ADDR5, NRZB, ECLK3 );     
        TSSET (  ADDR6, NRZB, ECLK3 );     
        TSSET (  ADDR7, NRZB, ECLK3 );     
        TSSET (  ADDR8, NRZB, ECLK3 );     
        TSSET (  ADDR9, NRZB, ECLK3 );     
        TSSET ( ADDR10, NRZB, ECLK3 );     
        TSSET ( ADDR11, NRZB, ECLK3 );     
        TSSET ( ADDR12, NRZB, ECLK3 );     
        TSSET ( ADDR13, NRZB, ECLK3 );     
        TSSET ( ADDR14, NRZB, ECLK3 );     
        TSSET ( ADDR15, NRZB, ECLK3 );     
        TSSET ( ADDR16, NRZB, ECLK3 );     
        TSSET ( ADDR17, NRZB, ECLK3 );     
        TSSET ( ADDR18, NRZB, ECLK3 );     
        TSSET ( ADDR19, NRZB, ECLK3 );     
        TSSET ( ADDR20, NRZB, ECLK3 );     
        TSSET ( ADDR21, NRZB, ECLK3 );     
        TSSET ( ADDR22, NRZB, ECLK3 );     
        TSSET ( ADDR23, NRZB, ECLK3 );     
        TSSET ( ADDR24, NRZB, ECLK3 );     
        TSSET ( ADDR25, NRZB, ECLK3 ); 
        TSSET ( ADDR26, NRZB, ECLK3 );     
        TSSET ( ADDR27, NRZB, ECLK3 );    
        TSSET ( ADDR28, NRZB, ECLK3 );   
        TSSET ( ADDR29, NRZB, ECLK3 );   
        TSSET ( ADDR30, NRZB, ECLK3 );   
        TSSET ( ADDR31, NRZB, ECLK3 );   

        TSSET (  CLK0, NRZB, ECLK3 );    
        TSSET (  CLK1, NRZB, ECLK5 );    
        TSSET (  CLK2, NRZB, ECLK3 );    
        TSSET (  CLK3, NRZB, ECLK1 );    
        TSSET (  CLK4, NRZB, ECLK1 );    
        TSSET (  CLK5, NRZB, ECLK3 );    
        TSSET (  CLK6, NRZB, ECLK3 );    
        TSSET (  CLK7, NRZB, ECLK5 );    

        TSSET (  SCAN0, NRZB, ECLK3 );   
        TSSET (  SCAN1, NRZB, ECLK3 );   
        TSSET (  SCAN2, NRZB, ECLK3 );   
        TSSET (  SCAN3, NRZB, ECLK3 );   
        TSSET (  SCAN4, NRZB, ECLK3 );   
        TSSET (  SCAN5, NRZB, ECLK3 );   
        TSSET (  SCAN6, NRZB, ECLK3 );   
        TSSET (  SCAN7, NRZB, ECLK3 );   
        TSSET (  SCAN8, NRZB, ECLK3 );   
        TSSET (  SCAN9, NRZB, ECLK3 );   
        TSSET ( SCAN10, NRZB, ECLK3 );   
        TSSET ( SCAN11, NRZB, ECLK3 );   
        TSSET ( SCAN12, NRZB, ECLK3 );   
        TSSET ( SCAN13, NRZB, ECLK3 );   
        TSSET ( SCAN14, NRZB, ECLK3 );   
        TSSET ( SCAN15, NRZB, ECLK3 );   
        TSSET ( SCAN16, NRZB, ECLK3 );   
        TSSET ( SCAN17, NRZB, ECLK3 );   
        TSSET ( SCAN18, NRZB, ECLK3 );   
        TSSET ( SCAN19, NRZB, ECLK3 );   
        TSSET ( SCAN20, NRZB, ECLK3 );   
        TSSET ( SCAN21, NRZB, ECLK3 );   
        TSSET ( SCAN22, NRZB, ECLK3 );   
        TSSET ( SCAN23, NRZB, ECLK3 );   
        TSSET ( SCAN24, NRZB, ECLK3 );   
        TSSET ( SCAN25, NRZB, ECLK3 );   
        TSSET ( SCAN26, NRZB, ECLK3 );   
        TSSET ( SCAN27, NRZB, ECLK3 );   
        TSSET ( SCAN28, NRZB, ECLK3 );   
        TSSET ( SCAN29, NRZB, ECLK3 );   
        TSSET ( SCAN30, NRZB, ECLK3 );     
        TSSET ( SCAN31, NRZB, ECLK3 );  

        TSSET (  IO0, NRZB, ECLK2 );   
        TSSET (  IO1, NRZB, ECLK2 );   
        TSSET (  IO2, NRZB, ECLK2 );   
        TSSET (  IO3, NRZB, ECLK2 );   
        TSSET (  IO4, NRZB, ECLK2 );   
        TSSET (  IO5, NRZB, ECLK2 );   
        TSSET (  IO6, NRZB, ECLK2 );   
        TSSET (  IO7, NRZB, ECLK2 );   
        TSSET (  IO8, NRZB, ECLK2 );   
        TSSET (  IO9, NRZB, ECLK2 );   
        TSSET ( IO10, NRZB, ECLK2 );   
        TSSET ( IO11, NRZB, ECLK2 );   
        TSSET ( IO12, NRZB, ECLK2 );   
        TSSET ( IO13, NRZB, ECLK2 );   
        TSSET ( IO14, NRZB, ECLK2 );   
        TSSET ( IO15, NRZB, ECLK2 );   
        TSSET ( IO16, NRZB, ECLK2 );   
        TSSET ( IO17, NRZB, ECLK2 );   
        TSSET ( IO18, NRZB, ECLK2 );   
        TSSET ( IO19, NRZB, ECLK2 );   
        TSSET ( IO20, NRZB, ECLK2 );   
        TSSET ( IO21, NRZB, ECLK2 );   
        TSSET ( IO22, NRZB, ECLK2 );   
        TSSET ( IO23, NRZB, ECLK2 );   
        TSSET ( IO24, NRZB, ECLK2 );   
        TSSET ( IO25, NRZB, ECLK2 );   
        TSSET ( IO26, NRZB, ECLK2 );   
        TSSET ( IO27, NRZB, ECLK2 );   
        TSSET ( IO28, NRZB, ECLK2 );   
        TSSET ( IO29, NRZB, ECLK2 );   
        TSSET ( IO30, NRZB, ECLK2 );   
        TSSET ( IO31, NRZB, ECLK2 );  	
        TSSET (  DRE, NRZB);
//      TSSET ( STRB, NRZB, ECLK2);
}


void load_feset_cal (const char* feset_name)
{
        TSSET (  ADDR0, FIXH, ECLK1 );     
        TSSET (  ADDR1, FIXH, ECLK1 );     
        TSSET (  ADDR2, FIXH, ECLK1 );     
        TSSET (  ADDR3, FIXH, ECLK1 );     
        TSSET (  ADDR4, FIXH, ECLK1 );     
        TSSET (  ADDR5, FIXH, ECLK1 );     
        TSSET (  ADDR6, FIXH, ECLK1 );     
        TSSET (  ADDR7, FIXH, ECLK1 );     
        TSSET (  ADDR8, FIXH, ECLK1 );     
        TSSET (  ADDR9, FIXH, ECLK1 );     
        TSSET ( ADDR10, FIXH, ECLK1 );     
        TSSET ( ADDR11, FIXH, ECLK1 );     
        TSSET ( ADDR12, FIXH, ECLK1 );     
        TSSET ( ADDR13, FIXH, ECLK1 );     
        TSSET ( ADDR14, FIXH, ECLK1 );     
        TSSET ( ADDR15, FIXH, ECLK1 );     
        TSSET ( ADDR16, FIXH, ECLK1 );     
        TSSET ( ADDR17, FIXH, ECLK1 );     
        TSSET ( ADDR18, FIXH, ECLK1 );     
        TSSET ( ADDR19, FIXH, ECLK1 );     
        TSSET ( ADDR20, FIXH, ECLK1 );     
        TSSET ( ADDR21, FIXH, ECLK1 );     
        TSSET ( ADDR22, FIXH, ECLK1 );     
        TSSET ( ADDR23, FIXH, ECLK1 );     
        TSSET ( ADDR24, FIXH, ECLK1 );     
        TSSET ( ADDR25, FIXH, ECLK1 );     
        TSSET ( ADDR26, FIXH, ECLK1 );     
        TSSET ( ADDR27, FIXH, ECLK1 );     
        TSSET ( ADDR28, FIXH, ECLK1 );     
        TSSET ( ADDR29, FIXH, ECLK1 );     
        TSSET ( ADDR30, FIXH, ECLK1 );     
        TSSET ( ADDR31, FIXH, ECLK1 ); 

        TSSET (  CLK0,FIXH, ECLK1 );     
        TSSET (  CLK1,FIXH, ECLK1 );     
        TSSET (  CLK2,FIXH, ECLK1 );     
        TSSET (  CLK3,FIXH, ECLK1 );     
        TSSET (  CLK4,FIXH, ECLK1 );     
        TSSET (  CLK5,FIXH, ECLK1 );     
        TSSET (  CLK6,FIXH, ECLK1 );     
        TSSET (  CLK7,FIXH, ECLK1 );     

        TSSET (  SCAN0,FIXH, ECLK1 );   
        TSSET (  SCAN1,FIXH, ECLK1 );   
        TSSET (  SCAN2,FIXH, ECLK1 );   
        TSSET (  SCAN3,FIXH, ECLK1 );   
        TSSET (  SCAN4,FIXH, ECLK1 );   
        TSSET (  SCAN5,FIXH, ECLK1 );   
        TSSET (  SCAN6,FIXH, ECLK1 );   
        TSSET (  SCAN7,FIXH, ECLK1 );   
        TSSET (  SCAN8,FIXH, ECLK1 );   
        TSSET (  SCAN9,FIXH, ECLK1 );   
        TSSET ( SCAN10,FIXH, ECLK1 );   
        TSSET ( SCAN11,FIXH, ECLK1 );   
        TSSET ( SCAN12,FIXH, ECLK1 );   
        TSSET ( SCAN13,FIXH, ECLK1 );   
        TSSET ( SCAN14,FIXH, ECLK1 );   
        TSSET ( SCAN15,FIXH, ECLK1 );   
        TSSET ( SCAN16,FIXH, ECLK1 );   
        TSSET ( SCAN17,FIXH, ECLK1 );   
        TSSET ( SCAN18,FIXH, ECLK1 );   
        TSSET ( SCAN19,FIXH, ECLK1 );   
        TSSET ( SCAN20,FIXH, ECLK1 );   
        TSSET ( SCAN21,FIXH, ECLK1 );   
        TSSET ( SCAN22,FIXH, ECLK1 );   
        TSSET ( SCAN23,FIXH, ECLK1 );   
        TSSET ( SCAN24,FIXH, ECLK1 );   
        TSSET ( SCAN25,FIXH, ECLK1 );   
        TSSET ( SCAN26,FIXH, ECLK1 );   
        TSSET ( SCAN27,FIXH, ECLK1 );   
        TSSET ( SCAN28,FIXH, ECLK1 );   
        TSSET ( SCAN29,FIXH, ECLK1 );   
        TSSET ( SCAN30,FIXH, ECLK1 );     
        TSSET ( SCAN31,FIXH, ECLK1 );  

        TSSET (  IO0, FIXH, ECLK1 );   
        TSSET (  IO1, FIXH, ECLK1 );   
        TSSET (  IO2, FIXH, ECLK1 );   
        TSSET (  IO3, FIXH, ECLK1 );   
        TSSET (  IO4, FIXH, ECLK1 );   
        TSSET (  IO5, FIXH, ECLK1 );   
        TSSET (  IO6, FIXH, ECLK1 );   
        TSSET (  IO7, FIXH, ECLK1 );   
        TSSET (  IO8, FIXH, ECLK1 );   
        TSSET (  IO9, FIXH, ECLK1 );   
        TSSET ( IO10, FIXH, ECLK1 );   
        TSSET ( IO11, FIXH, ECLK1 );   
        TSSET ( IO12, FIXH, ECLK1 );   
        TSSET ( IO13, FIXH, ECLK1 );  
        TSSET ( IO14, FIXH, ECLK1 );   
        TSSET ( IO15, FIXH, ECLK1 );   
        TSSET ( IO16, FIXH, ECLK1 );   
        TSSET ( IO17, FIXH, ECLK1 );   
        TSSET ( IO18, FIXH, ECLK1 );   
        TSSET ( IO19, FIXH, ECLK1 );   
        TSSET ( IO20, FIXH, ECLK1 );   
        TSSET ( IO21, FIXH, ECLK1 );   
        TSSET ( IO22, FIXH, ECLK1 );   
        TSSET ( IO23, FIXH, ECLK1 );   
        TSSET ( IO24, FIXH, ECLK1 );   
        TSSET ( IO25, FIXH, ECLK1 );   
        TSSET ( IO26, FIXH, ECLK1 );   
        TSSET ( IO27, FIXH, ECLK1 );   
        TSSET ( IO28, FIXH, ECLK1 );   
        TSSET ( IO29, FIXH, ECLK1 );   
        TSSET ( IO30, FIXH, ECLK1 );   
        TSSET ( IO31, FIXH, ECLK1 );   
 
        TSSET (  DRE, FIXH);
 }


void load_feset_AFIXH()
{
        TSSET (  ADDR0, FIXH, ECLK1 );     
        TSSET (  ADDR1, FIXH, ECLK1 );     
        TSSET (  ADDR2, FIXH, ECLK1 );     
        TSSET (  ADDR3, FIXH, ECLK1 );     
        TSSET (  ADDR4, FIXH, ECLK1 );     
        TSSET (  ADDR5, FIXH, ECLK1 );     
        TSSET (  ADDR6, FIXH, ECLK1 );     
        TSSET (  ADDR7, FIXH, ECLK1 );     
        TSSET (  ADDR8, FIXH, ECLK1 );     
        TSSET (  ADDR9, FIXH, ECLK1 );     
        TSSET ( ADDR10, FIXH, ECLK1 );     
        TSSET ( ADDR11, FIXH, ECLK1 );     
        TSSET ( ADDR12, FIXH, ECLK1 );     
        TSSET ( ADDR13, FIXH, ECLK1 );     
        TSSET ( ADDR14, FIXH, ECLK1 );     
        TSSET ( ADDR15, FIXH, ECLK1 );     
        TSSET ( ADDR16, FIXH, ECLK1 );     
        TSSET ( ADDR17, FIXH, ECLK1 );     
        TSSET ( ADDR18, FIXH, ECLK1 );     
        TSSET ( ADDR19, FIXH, ECLK1 );     
        TSSET ( ADDR20, FIXH, ECLK1 );     
        TSSET ( ADDR21, FIXH, ECLK1 );     
        TSSET ( ADDR22, FIXH, ECLK1 );     
        TSSET ( ADDR23, FIXH, ECLK1 );     
        TSSET ( ADDR24, FIXH, ECLK1 );     
        TSSET ( ADDR25, FIXH, ECLK1 );     
        TSSET ( ADDR26, FIXH, ECLK1 );     
        TSSET ( ADDR27, FIXH, ECLK1 );     
        TSSET ( ADDR28, FIXH, ECLK1 );     
        TSSET ( ADDR29, FIXH, ECLK1 );     
        TSSET ( ADDR30, FIXH, ECLK1 );     
        TSSET ( ADDR31, FIXH, ECLK1 );  
        
        TSSET ( CLK0, FIXH, ECLK1 );     
        TSSET ( CLK1, FIXH, ECLK1 );     
        TSSET ( CLK2, FIXH, ECLK1 );     
        TSSET ( CLK3, FIXH, ECLK1 );     
        TSSET ( CLK4, FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( CLK5, FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( CLK6, FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( CLK7, FIXH, ECLK1 );                                                                                                                  
        
        TSSET ( SCAN0 , FIXH, ECLK1 );     
        TSSET ( SCAN1 , FIXH, ECLK1 );     
        TSSET ( SCAN2 , FIXH, ECLK1 );     
        TSSET ( SCAN3 , FIXH, ECLK1 );     
        TSSET ( SCAN4 , FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN5 , FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN6 , FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN7 , FIXH, ECLK1 );                                                                                                                  
        TSSET ( SCAN8 , FIXH, ECLK1 );     
        TSSET ( SCAN9 , FIXH, ECLK1 );     
        TSSET ( SCAN10, FIXH, ECLK1 );     
        TSSET ( SCAN11, FIXH, ECLK1 );     
        TSSET ( SCAN12, FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN13, FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN14, FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN15, FIXH, ECLK1 );                                                                                                                  
        TSSET ( SCAN16, FIXH, ECLK1 );     
        TSSET ( SCAN17, FIXH, ECLK1 );     
        TSSET ( SCAN18, FIXH, ECLK1 );     
        TSSET ( SCAN19, FIXH, ECLK1 );     
        TSSET ( SCAN20, FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN21, FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN22, FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN23, FIXH, ECLK1 );                                                                                                                  
        TSSET ( SCAN24, FIXH, ECLK1 );     
        TSSET ( SCAN25, FIXH, ECLK1 );     
        TSSET ( SCAN26, FIXH, ECLK1 );     
        TSSET ( SCAN27, FIXH, ECLK1 );     
        TSSET ( SCAN28, FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN29, FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN30, FIXH, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN31, FIXH, ECLK1 );    
}
void load_feset_AFIXL()
{
        TSSET (  ADDR0, FIXL, ECLK1 );     
        TSSET (  ADDR1, FIXL, ECLK1 );     
        TSSET (  ADDR2, FIXL, ECLK1 );     
        TSSET (  ADDR3, FIXL, ECLK1 );     
        TSSET (  ADDR4, FIXL, ECLK1 );     
        TSSET (  ADDR5, FIXL, ECLK1 );     
        TSSET (  ADDR6, FIXL, ECLK1 );     
        TSSET (  ADDR7, FIXL, ECLK1 );     
        TSSET (  ADDR8, FIXL, ECLK1 );     
        TSSET (  ADDR9, FIXL, ECLK1 );     
        TSSET ( ADDR10, FIXL, ECLK1 );     
        TSSET ( ADDR11, FIXL, ECLK1 );     
        TSSET ( ADDR12, FIXL, ECLK1 );     
        TSSET ( ADDR13, FIXL, ECLK1 );     
        TSSET ( ADDR14, FIXL, ECLK1 );     
        TSSET ( ADDR15, FIXL, ECLK1 );     
        TSSET ( ADDR16, FIXL, ECLK1 );     
        TSSET ( ADDR17, FIXL, ECLK1 );     
        TSSET ( ADDR18, FIXL, ECLK1 );     
        TSSET ( ADDR19, FIXL, ECLK1 );     
        TSSET ( ADDR20, FIXL, ECLK1 );     
        TSSET ( ADDR21, FIXL, ECLK1 );     
        TSSET ( ADDR22, FIXL, ECLK1 );     
        TSSET ( ADDR23, FIXL, ECLK1 );     
        TSSET ( ADDR24, FIXL, ECLK1 );     
        TSSET ( ADDR25, FIXL, ECLK1 );     
        TSSET ( ADDR26, FIXL, ECLK1 );     
        TSSET ( ADDR27, FIXL, ECLK1 );     
        TSSET ( ADDR28, FIXL, ECLK1 );     
        TSSET ( ADDR29, FIXL, ECLK1 );     
        TSSET ( ADDR30, FIXL, ECLK1 );     
        TSSET ( ADDR31, FIXL, ECLK1 );  
        
        TSSET ( CLK0, FIXL, ECLK1 );     
        TSSET ( CLK1, FIXL, ECLK1 );     
        TSSET ( CLK2, FIXL, ECLK1 );     
        TSSET ( CLK3, FIXL, ECLK1 );     
        TSSET ( CLK4, FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( CLK5, FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( CLK6, FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( CLK7, FIXL, ECLK1 );                                                                                                                  
        
        TSSET ( SCAN0 , FIXL, ECLK1 );     
        TSSET ( SCAN1 , FIXL, ECLK1 );     
        TSSET ( SCAN2 , FIXL, ECLK1 );     
        TSSET ( SCAN3 , FIXL, ECLK1 );     
        TSSET ( SCAN4 , FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN5 , FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN6 , FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN7 , FIXL, ECLK1 );                                                                                                                  
        TSSET ( SCAN8 , FIXL, ECLK1 );     
        TSSET ( SCAN9 , FIXL, ECLK1 );     
        TSSET ( SCAN10, FIXL, ECLK1 );     
        TSSET ( SCAN11, FIXL, ECLK1 );     
        TSSET ( SCAN12, FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN13, FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN14, FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN15, FIXL, ECLK1 );                                                                                                                  
        TSSET ( SCAN16, FIXL, ECLK1 );     
        TSSET ( SCAN17, FIXL, ECLK1 );     
        TSSET ( SCAN18, FIXL, ECLK1 );     
        TSSET ( SCAN19, FIXL, ECLK1 );     
        TSSET ( SCAN20, FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN21, FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN22, FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN23, FIXL, ECLK1 );                                                                                                                  
        TSSET ( SCAN24, FIXL, ECLK1 );     
        TSSET ( SCAN25, FIXL, ECLK1 );     
        TSSET ( SCAN26, FIXL, ECLK1 );     
        TSSET ( SCAN27, FIXL, ECLK1 );     
        TSSET ( SCAN28, FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN29, FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN30, FIXL, ECLK1 );                                                                                                                                                                                                                                                                                                                                              
        TSSET ( SCAN31, FIXL, ECLK1 );      
}


void load_level (const char* level_name)
{
    if(strcmp(level_name , "diag_level") == 0)
    {
        VIH ( CLK  , 1.0, false  );    // DR pin VIH set
        VIL ( CLK  , 0.0, false  );    // DR pin VIL set
        VIH ( DR   , 1.0, false  );    // DR pin VIH set
        VIL ( DR   , 0.0, false  );    // DR pin VIL set
        VIH ( SCAN , 1.0, false  );    // DR pin VIH set
        VIL ( SCAN , 0.0, false  );    // DR pin VIL set
        VIH ( IO   , 1.0, false  );    // DR pin VIH set
        VIL ( IO   , 0.0, false  );    // DR pin VIL set
        VTH ( 0.5 );         // comparator VTH set  
        
        IMP( CLK  , 10  );    // DR pin VIH set  60 , 20, 10
        IMP( DR   , 10  );    // DR pin VIH set   50   26   16
        IMP( SCAN , 30  );    // DR pin VIH set
        IMP( IO   , 30  );    // DR pin VIH set
    }
}
void load_dps (const char* dps_name)
{
	    DPS(1, 1.0, 0.1, 5.0, 20);
        DPS(2, 1.0, 0.1, 5.0, 20);
        DPS(4, 3.0, 0.1, 5.0, 20);
        DPS(5, 1.0, 0.1, 5.0, 20);
}

void load_dps_of_dpscal (double set_vdd, double set_vpp)
{
    double set_vih = set_vdd;
    double set_vil = -0.2;
    double set_vth = set_vdd/2 ;
    
    DPS(1, set_vdd, 0.0, 5.0, 40);
    DPS(2, set_vpp, 0.0, 5.0, 40);
    DPS(4,    3.00, 0.0, 5.0, 40);
    DPS(5, set_vdd, 0.0, 5.0, 40);

    VIH ( CLK  , set_vih  );   // CLK pin VIH set
    VIL ( CLK  , set_vil  );   // CLK pin VIL set
    VIH ( DR   , set_vih  );   // ADR pin VIH set
    VIL ( DR   , set_vil  );   // ADR pin VIL set
    VIH ( SCAN , set_vih  );   // SCAN pin VIH set
    VIL ( SCAN , set_vil  );   // SCAN pin VIL set
    VIH ( IO   , set_vih  );   // IO pin VIH set
    VIL ( IO   , set_vil  );   // IO pin VIL set
    VTH ( set_vth );           // comparator VTH set  

    IMP( CLK   , 16  );        // DR pin impedance set    
    IMP( DR    , 16  );        // DR pin impedance set    
    IMP( SCAN  , 30  );        // DR pin impedance set
    IMP( IO    , 30  );        // DR pin impedance set
}

void load_pds (const char* cyp_name )
{
   //     printf("%s : %s\n", __FUNCTION__,  cyp_name);
        PDS(  CLK0,  FIXL  ,  C0  ,  C0  ,  C0  );     
        PDS(  CLK1,  FIXL  ,  C1  ,  C1  ,  C1  );     
        PDS(  CLK2,  FIXL  ,  C2  ,  C2  ,  C2  );     
        PDS(  CLK3,  FIXL  ,  C3  ,  C3  ,  C3  );     
        PDS(  CLK4,  FIXL  ,  C4  ,  C4  ,  C4  );     
        PDS(  CLK5,  FIXL  ,  C5  ,  C5  ,  C5  );     
        PDS(  CLK6,  FIXL  ,  C6  ,  C6  ,  C6  );     
        PDS(  CLK7,  FIXL  ,  C7  ,  C7  ,  C7  );     
      
    
        PDS(  SCAN0,  FIXL,  C0 , C0 ,  C0 );     
        PDS(  SCAN1,  FIXL,  C1 , C1 ,  C1 );     
        PDS(  SCAN2,  FIXL,  C2 , C2 ,  C2 );     
        PDS(  SCAN3,  FIXL,  C3 , C3 ,  C3 );     
        PDS(  SCAN4,  FIXL,  C4 , C4 ,  C4 );     
        PDS(  SCAN5,  FIXL,  C5 , C5 ,  C5 );     
        PDS(  SCAN6,  FIXL,  C6 , C6 ,  C6 );     
        PDS(  SCAN7,  FIXL,  C7 , C7 ,  C7 );     
        PDS(  SCAN8,  FIXL,  C8 , C8 ,  C8 );     
        PDS(  SCAN9,  FIXL,  C9 , C9 ,  C9 );     
        PDS( SCAN10,  FIXL,  C10, C10,  C10);    
        PDS( SCAN11,  FIXL,  C11, C11,  C11);    
        PDS( SCAN12,  FIXL,  C12, C12,  C12);    
        PDS( SCAN13,  FIXL,  C13, C13,  C13);    
        PDS( SCAN14,  FIXL,  C14, C14,  C14);    
        PDS( SCAN15,  FIXL,  C15, C15,  C15);    
        PDS( SCAN16,  FIXL,  C16, C16,  C16);    
        PDS( SCAN17,  FIXL,  C17, C17,  C17);    
        PDS( SCAN18,  FIXL,  C18, C18,  C18);    
        PDS( SCAN19,  FIXL,  C19, C19,  C19);    
        PDS( SCAN20,  FIXL,  C20, C20,  C20);    
        PDS( SCAN21,  FIXL,  C21, C21,  C21);    
        PDS( SCAN22,  FIXL,  C22, C22,  C22);    
        PDS( SCAN23,  FIXL,  C23, C23,  C23);    
        PDS( SCAN24,  FIXL,  C24, C24,  C24);    
        PDS( SCAN25,  FIXL,  C25, C25,  C25);    
        PDS( SCAN26,  FIXL,  C26, C26,  C26);    
        PDS( SCAN27,  FIXL,  C27, C27,  C27);    
        PDS( SCAN28,  FIXL,  C28, C28,  C28);    
        PDS( SCAN29,  FIXL,  C29, C29,  C29);    
        PDS( SCAN30,  FIXL,  C30, C30,  C30);    
        PDS( SCAN31,  FIXL,  C31, C31,  C31); 

        PDS(  ADDR0,    X0  ,  Y0  ,  X0  ,  Y0  );     
        PDS(  ADDR1,    X1  ,  Y1  ,  X1  ,  Y1  );     
        PDS(  ADDR2,    X2  ,  Y2  ,  X2  ,  Y2  );     
        PDS(  ADDR3,    X3  ,  Y3  ,  X3  ,  Y3  );     
        PDS(  ADDR4,    X4  ,  Y4  ,  X4  ,  Y4  );     
        PDS(  ADDR5,    X5  ,  Y5  ,  X5  ,  Y5  );     
        PDS(  ADDR6,    X6  ,  Y6  ,  X6  ,  Y6  );     
        PDS(  ADDR7,    X7  ,  Y7  ,  X7  ,  Y7  );     
        PDS(  ADDR8,    X8  ,  Y8  ,  X8  ,  Y8  );     
        PDS(  ADDR9,    X9  ,  Y9  ,  X9  ,  Y9  );     
        PDS( ADDR10,    X10 ,  Y10 ,  X10 ,  Y10 );    
        PDS( ADDR11,    X11 ,  Y11 ,  X11 ,  Y11 );    
        PDS( ADDR12,    X12 ,  Y12 ,  X12 ,  Y12 );    
        PDS( ADDR13,    X13 ,  Y13 ,  X13 ,  Y13 );    
        PDS( ADDR14,    X14 ,  Y14 ,  X14 ,  Y14 );    
        PDS( ADDR15,    X15 ,  Y15 ,  X15 ,  Y15 );    
        PDS( ADDR16,    X16 ,  Y16 ,  X16 ,  Y16 );    
        PDS( ADDR17,    X17 ,  Y17 ,  X17 ,  Y17 );    
        PDS( ADDR18,    X18 ,  Y18 ,  X18 ,  Y18 );    
        PDS( ADDR19,    X19 ,  Y19 ,  X19 ,  Y19 );    
        PDS( ADDR20,    X20 ,  Y20 ,  X20 ,  Y20 );    
        PDS( ADDR21,    X21 ,  Y21 ,  X21 ,  Y21 );    
        PDS( ADDR22,    X22 ,  Y22 ,  X22 ,  Y22 );    
        PDS( ADDR23,    X23 ,  Y23 ,  X23 ,  Y23 );    
        PDS( ADDR24,    Z0  ,  Z0  ,  Z0  ,  Z0  );    
        PDS( ADDR25,    Z1  ,  Z1  ,  Z1  ,  Z1  );    
        PDS( ADDR26,    Z2  ,  Z2  ,  Z2  ,  Z2  );    
        PDS( ADDR27,    Z3  ,  Z3  ,  Z3  ,  Z3  );    
        PDS( ADDR28,    Z4  ,  Z4  ,  Z4  ,  Z4  );    
        PDS( ADDR29,    Z5  ,  Z5  ,  Z5  ,  Z5  );    
        PDS( ADDR30,    Z6  ,  Z6  ,  Z6  ,  Z6  );    
        PDS( ADDR31,    Z7  ,  Z7  ,  Z7  ,  Z7  );    
 
        PDS(   IO0,  D0,	 D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15  );     
        PDS(   IO1,  D1,	 D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0   );     
        PDS(   IO2,  D2,	 D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1   );     
        PDS(   IO3,  D3,	 D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2   );     
        PDS(   IO4,  D4,	 D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3   );     
        PDS(   IO5,  D5,	 D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4   );     
        PDS(   IO6,  D6,	 D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5   );     
        PDS(   IO7,  D7,	 D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6   );     
        PDS(   IO8,  D8,	 D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7   );     
        PDS(   IO9,  D9,	 D10,	D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8   );     
        PDS(  IO10,  D10,  D11,	D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9   );     
        PDS(  IO11,  D11,  D12,	D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10  );     
        PDS(  IO12,  D12,  D13,	D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11  );     
        PDS(  IO13,  D13,  D14,	D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12  );     
        PDS(  IO14,  D14,  D15,	D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13  );     
        PDS(  IO15,  D15,  D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14  );     
        PDS(  IO16,  D0,	 D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15  );   
        PDS(  IO17,  D1,	 D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0   );     
        PDS(  IO18,  D2,	 D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1   );     
        PDS(  IO19,  D3,	 D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2   );     
        PDS(  IO20,  D4,	 D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3   );     
        PDS(  IO21,  D5,	 D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4   );     
        PDS(  IO22,  D6,	 D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5   );     
        PDS(  IO23,  D7,	 D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6   );     
        PDS(  IO24,  D8,	 D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7   );     
        PDS(  IO25,  D9,	 D10,	D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8   );     
        PDS(  IO26,  D10,  D11,	D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9   );     
        PDS(  IO27,  D11,  D12,	D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10  );     
        PDS(  IO28,  D12,  D13,	D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11  );     
        PDS(  IO29,  D13,  D14,	D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12  );     
        PDS(  IO30,  D14,  D15,	D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13  );     
        PDS(  IO31,  D15,  D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14  );   


    if(strcmp(cyp_name , "diag_pds_data_data16") == 0)
    {
         PDS(   IO0,  D0,	 D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15  );     
        PDS(   IO1,  D1,	 D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0   );     
        PDS(   IO2,  D2,	 D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1   );     
        PDS(   IO3,  D3,	 D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2   );     
        PDS(   IO4,  D4,	 D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3   );     
        PDS(   IO5,  D5,	 D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4   );     
        PDS(   IO6,  D6,	 D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5   );     
        PDS(   IO7,  D7,	 D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6   );     
        PDS(   IO8,  D8,	 D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7   );     
        PDS(   IO9,  D9,	 D10,	D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8   );     
        PDS(  IO10,  D10,  D11,	D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9   );     
        PDS(  IO11,  D11,  D12,	D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10  );     
        PDS(  IO12,  D12,  D13,	D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11  );     
        PDS(  IO13,  D13,  D14,	D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12  );     
        PDS(  IO14,  D14,  D15,	D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13  );     
        PDS(  IO15,  D15,  D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14  );     
        PDS(  IO16,  D0,	 D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15  );     
        PDS(  IO17,  D1,	 D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0   );     
        PDS(  IO18,  D2,	 D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1   );     
        PDS(  IO19,  D3,	 D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2   );     
        PDS(  IO20,  D4,	 D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3   );     
        PDS(  IO21,  D5,	 D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4   );     
        PDS(  IO22,  D6,	 D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5   );     
        PDS(  IO23,  D7,	 D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6   );     
        PDS(  IO24,  D8,	 D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7   );     
        PDS(  IO25,  D9,	 D10,	D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8   );     
        PDS(  IO26,  D10,  D11,	D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9   );     
        PDS(  IO27,  D11,  D12,	D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10  );     
        PDS(  IO28,  D12,  D13,	D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11  );     
        PDS(  IO29,  D13,  D14,	D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12  );     
        PDS(  IO30,  D14,  D15,	D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13  );     
        PDS(  IO31,  D15,  D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14  );         
    }
    if(strcmp(cyp_name , "diag_pds_data_data18") == 0)
    {
        PDS(   IO0,  D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,_D12,	_D13,	FIXH,	FIXL);     
        PDS(   IO1,  D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,_D13,	_D14,	FIXH,	FIXL);     
        PDS(   IO2,  D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,_D14,	_D15,	FIXH,	FIXL);     
        PDS(   IO3,  D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,_D15,	_D16,	FIXH,	FIXL);     
        PDS(   IO4,  D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,_D16,	_D17,	FIXH,	FIXL);     
        PDS(   IO5,  D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D16,_D17,	_D0,	FIXH,	FIXL);     
        PDS(   IO6,  D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D16,D17,_D0,	_D1,	FIXH,	FIXL);     
        PDS(   IO7,  D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D16,D17,D0,	_D1,	_D2,	FIXH,	FIXL);     
        PDS(   IO8,  D8,	D9,	D10,D11,D12,D13,D14,D15,D16,D17,D0,	D1,	_D2,	_D3,	FIXH,	FIXL);     
        PDS(   IO9,  D9,   D10,	D11,D12,D13,D14,D15,D16,D17,D0,	D1,	D2,	_D3,	_D4,	FIXH,	FIXL);     
        PDS(  IO10,  D10,  D11,	D12,D13,D14,D15,D16,D17,D0,	D1,	D2,	D3,	_D4,	_D5,	FIXH,	FIXL);     
        PDS(  IO11,  D11,  D12,	D13,D14,D15,D16,D17,D0,	D1,	D2,	D3,	D4,	_D5,	_D6,	FIXH,	FIXL);     
        PDS(  IO12,  D12,  D13,	D14,D15,D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	_D6,	_D7,	FIXH,	FIXL);     
        PDS(  IO13,  D13,  D14,	D15,D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	D6,	_D7,	_D8,	FIXH,	FIXL);     
        PDS(  IO14,  D14,  D15,	D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	_D8,	_D9,	FIXH,	FIXL);     
        PDS(  IO15,  D15,  D16,	D17,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	_D9,	_D10,	FIXH,	FIXL);     
        PDS(  IO16,  D0, 	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,_D12,	_D13,	FIXH,	FIXL);     
        PDS(  IO17,  D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,_D13,	_D14,	FIXH,	FIXL);     
        PDS(  IO18,  D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,_D14,	_D15,	FIXH,	FIXL);     
        PDS(  IO19,  D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,_D15,	_D16,	FIXH,	FIXL);     
        PDS(  IO20,  D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,_D16,	_D17,	FIXH,	FIXL);     
        PDS(  IO21,  D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D16,_D17,	_D0,	FIXH,	FIXL);     
        PDS(  IO22,  D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D16,D17,_D0,	_D1,	FIXH,	FIXL);     
        PDS(  IO23,  D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D16,D17,D0,	_D1,	_D2,	FIXH,	FIXL);     
        PDS(  IO24,  D8,	D9,	D10,D11,D12,D13,D14,D15,D16,D17,D0,	D1,	_D2,	_D3,	FIXH,	FIXL);     
        PDS(  IO25,  D9,	D10,D11,D12,D13,D14,D15,D16,D17,D0,	D1,	D2,	_D3,	_D4,	FIXH,	FIXL);     
        PDS(  IO26,  D10,   D11,D12,D13,D14,D15,D16,D17,D0,	D1,	D2,	D3,	_D4,	_D5,	FIXH,	FIXL);     
        PDS(  IO27,  D11,   D12,D13,D14,D15,D16,D17,D0,	D1,	D2,	D3,	D4,	_D5,	_D6,	FIXH,	FIXL);     
        PDS(  IO28,  D12,   D13,D14,D15,D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	_D6,	_D7,	FIXH,	FIXL);     
        PDS(  IO29,  D13,   D14,D15,D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	D6,	_D7,	_D8,	FIXH,	FIXL);     
        PDS(  IO30,  D14,   D15,D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	_D8,	_D9,	FIXH,	FIXL);     
        PDS(  IO31,  D15,   D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	_D9,	_D10,	FIXH,	FIXL);     

        PDS(   IO0_B,  D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,_D12,	_D13,	FIXL,	FIXH);     
        PDS(   IO1_B,  D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,_D13,	_D14,	FIXL,	FIXH);     
        PDS(   IO2_B,  D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,_D14,	_D15,	FIXL,	FIXH);     
        PDS(   IO3_B,  D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,_D15,	_D16,	FIXL,	FIXH);     
        PDS(   IO4_B,  D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,_D16,	_D17,	FIXL,	FIXH);     
        PDS(   IO5_B,  D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D16,_D17,	_D0,	FIXL,	FIXH);     
        PDS(   IO6_B,  D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D16,D17,_D0,	_D1,	FIXL,	FIXH);     
        PDS(   IO7_B,  D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D16,D17,D0,	_D1,	_D2,	FIXL,	FIXH);     
        PDS(   IO8_B,  D8,	D9,	D10,D11,D12,D13,D14,D15,D16,D17,D0,	D1,	_D2,	_D3,	FIXL,	FIXH);     
        PDS(   IO9_B,  D9,   D10,	D11,D12,D13,D14,D15,D16,D17,D0,	D1,	D2,	_D3,_D4,	FIXL,	FIXH);     
        PDS(  IO10_B,  D10,  D11,	D12,D13,D14,D15,D16,D17,D0,	D1,	D2,	D3,	_D4,_D5,	FIXL,	FIXH);     
        PDS(  IO11_B,  D11,  D12,	D13,D14,D15,D16,D17,D0,	D1,	D2,	D3,	D4,	_D5,_D6,	FIXL,	FIXH);     
        PDS(  IO12_B,  D12,  D13,	D14,D15,D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	_D6,_D7,	FIXL,	FIXH);     
        PDS(  IO13_B,  D13,  D14,	D15,D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	D6,	_D7,_D8,	FIXL,	FIXH);     
        PDS(  IO14_B,  D14,  D15,	D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	_D8,_D9,	FIXL,	FIXH);     
        PDS(  IO15_B,  D15,  D16,	D17,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	_D9,_D10,	FIXL,	FIXH);     
        PDS(  IO16_B,  D0, 	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,_D12,	_D13,	FIXL,	FIXH);     
        PDS(  IO17_B,  D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,_D13,	_D14,	FIXL,	FIXH);     
        PDS(  IO18_B,  D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,_D14,	_D15,	FIXL,	FIXH);     
        PDS(  IO19_B,  D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,_D15,	_D16,	FIXL,	FIXH);     
        PDS(  IO20_B,  D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,_D16,	_D17,	FIXL,	FIXH);     
        PDS(  IO21_B,  D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D16,_D17,	_D0,	FIXL,	FIXH);     
        PDS(  IO22_B,  D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D16,D17,_D0,	_D1,	FIXL,	FIXH);     
        PDS(  IO23_B,  D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D16,D17,D0,	_D1,	_D2,	FIXL,	FIXH);     
        PDS(  IO24_B,  D8,	D9,	D10,D11,D12,D13,D14,D15,D16,D17,D0,	D1,	_D2,	_D3,	FIXL,	FIXH);     
        PDS(  IO25_B,  D9,	D10,D11,D12,D13,D14,D15,D16,D17,D0,	D1,	D2,	_D3,	_D4,	FIXL,	FIXH);     
        PDS(  IO26_B,  D10,   D11,D12,D13,D14,D15,D16,D17,D0,	D1,	D2,	D3,	_D4,_D5,	FIXL,	FIXH);     
        PDS(  IO27_B,  D11,   D12,D13,D14,D15,D16,D17,D0,	D1,	D2,	D3,	D4,	_D5,_D6,	FIXL,	FIXH);     
        PDS(  IO28_B,  D12,   D13,D14,D15,D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	_D6,_D7,	FIXL,	FIXH);     
        PDS(  IO29_B,  D13,   D14,D15,D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	D6,	_D7,_D8,	FIXL,	FIXH);     
        PDS(  IO30_B,  D14,   D15,D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	_D8,_D9,	FIXL,	FIXH);     
        PDS(  IO31_B,  D15,   D16,D17,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	_D9,_D10,	FIXL,	FIXH);        
    }
   if(strcmp(cyp_name , "diag_pds_data_xy16") ==0)
    {
        PDS(   IO0,  X0,	 X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15  );     
        PDS(   IO1,  X1,	 X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X0   );     
        PDS(   IO2,  X2,	 X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X0,	X1   );     
        PDS(   IO3,  X3,	 X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X0,	X1,	X2   );     
        PDS(   IO4,  X4,	 X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X0,	X1,	X2,	X3   );     
        PDS(   IO5,  X5,	 X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X0,	X1,	X2,	X3,	X4   );     
        PDS(   IO6,  X6,	 X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X0,	X1,	X2,	X3,	X4,	X5   );     
        PDS(   IO7,  X7,	 X8,	X9,	X10,X11,X12,X13,X14,X15,X0,	X1,	X2,	X3,	X4,	X5,	X6   );     
        PDS(   IO8,  X8,	 X9,	X10,X11,X12,X13,X14,X15,X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7   );     
        PDS(   IO9,  X9,  X10,	X11,X12,X13,X14,X15,X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8   );     
        PDS(  IO10,  X10, X11,	X12,X13,X14,X15,X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9   );     
        PDS(  IO11,  X11, X12,	X13,X14,X15,X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10  );     
        PDS(  IO12,  X12, X13,	X14,X15,X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11  );     
        PDS(  IO13,  X13, X14,	X15,X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12  );     
        PDS(  IO14,  X14, X15,	X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13  );     
        PDS(  IO15,  X15,  X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14  );     
        PDS(  IO16,  X0,	 X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15  );     
        PDS(  IO17,  X1,	 X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X0   );     
        PDS(  IO18,  X2,	 X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X0,	X1   );     
        PDS(  IO19,  X3,	 X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X0,	X1,	X2   );     
        PDS(  IO20,  X4,	 X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X0,	X1,	X2,	X3   );     
        PDS(  IO21,  X5,	 X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X0,	X1,	X2,	X3,	X4   );     
        PDS(  IO22,  X6,	 X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X0,	X1,	X2,	X3,	X4,	X5   );     
        PDS(  IO23,  X7,	 X8,	X9,	X10,X11,X12,X13,X14,X15,X0,	X1,	X2,	X3,	X4,	X5,	X6   );     
        PDS(  IO24,  X8,	 X9,	X10,X11,X12,X13,X14,X15,X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7   );     
        PDS(  IO25,  X9,  X10,	X11,X12,X13,X14,X15,X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8   );     
        PDS(  IO26,  X10, X11,	X12,X13,X14,X15,X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9   );     
        PDS(  IO27,  X11, X12,	X13,X14,X15,X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10  );     
        PDS(  IO28,  X12, X13,	X14,X15,X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11  );     
        PDS(  IO29,  X13, X14,	X15,X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12  );     
        PDS(  IO30,  X14, X15,	X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13  );     
        PDS(  IO31,  X15,  X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14  );          

        PDS(   IO0_B,  Y0,	 Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15  );     
        PDS(   IO1_B,  Y1,	 Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0   );     
        PDS(   IO2_B,  Y2,	 Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1   );     
        PDS(   IO3_B,  Y3,	 Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2   );     
        PDS(   IO4_B,  Y4,	 Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3   );     
        PDS(   IO5_B,  Y5,	 Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4   );     
        PDS(   IO6_B,  Y6,	 Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5   );     
        PDS(   IO7_B,  Y7,	 Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6   );     
        PDS(   IO8_B,  Y8,	 Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7   );     
        PDS(   IO9_B,  Y9,	Y10,	Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8   );     
        PDS(  IO10_B,  Y10, Y11,	Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9   );     
        PDS(  IO11_B,  Y11, Y12,	Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10  );     
        PDS(  IO12_B,  Y12, Y13,	Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11  );     
        PDS(  IO13_B,  Y13, Y14,	Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12  );     
        PDS(  IO14_B,  Y14, Y15,	Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13  );     
        PDS(  IO15_B,  Y15,  Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14  );     
        PDS(  IO16_B,  Y0,	 Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15  );     
        PDS(  IO17_B,  Y1,	 Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0   );     
        PDS(  IO18_B,  Y2,	 Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1   );     
        PDS(  IO19_B,  Y3,	 Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2   );     
        PDS(  IO20_B,  Y4,	 Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3   );     
        PDS(  IO21_B,  Y5,	 Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4   );     
        PDS(  IO22_B,  Y6,	 Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5   );     
        PDS(  IO23_B,  Y7,	 Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6   );     
        PDS(  IO24_B,  Y8,	 Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7   );     
        PDS(  IO25_B,  Y9,	Y10,	Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8   );     
        PDS(  IO26_B,  Y10, Y11,	Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9   );     
        PDS(  IO27_B,  Y11, Y12,	Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10  );     
        PDS(  IO28_B,  Y12, Y13,	Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11  );     
        PDS(  IO29_B,  Y13, Y14,	Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12  );     
        PDS(  IO30_B,  Y14, Y15,	Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13  );     
        PDS(  IO31_B,  Y15,  Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14  );          

    }

   if(strcmp(cyp_name , "diag_pds_data_z08") ==0)
    {
        PDS(   IO0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0);	
        PDS(   IO1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1);	
        PDS(   IO2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2);	
        PDS(   IO3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3);	
        PDS(   IO4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4);	
        PDS(   IO5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5);	
        PDS(   IO6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6);	
        PDS(   IO7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7);	
        PDS(   IO8,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0);   	
        PDS(   IO9,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1);    
        PDS(  IO10,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2);    
        PDS(  IO11,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3);    
        PDS(  IO12,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4);    
        PDS(  IO13,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5);    
        PDS(  IO14,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6);    
        PDS(  IO15,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7);    
        PDS(  IO16,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0,   Z0);	
        PDS(  IO17,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1,   Z1);	
        PDS(  IO18,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2,   Z2);	
        PDS(  IO19,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3,   Z3);	
        PDS(  IO20,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4,   Z4);	
        PDS(  IO21,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5,   Z5);	
        PDS(  IO22,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6,   Z6);	
        PDS(  IO23,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7,   Z7);	
        PDS(  IO24,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0);
        PDS(  IO25,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1);
        PDS(  IO26,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2);
        PDS(  IO27,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3);
        PDS(  IO28,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4);
        PDS(  IO29,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5);
        PDS(  IO30,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6);
        PDS(  IO31,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7);

        PDS(   IO0_B,  Y0,	 Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15  );     
        PDS(   IO1_B,  Y1,	 Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0   );     
        PDS(   IO2_B,  Y2,	 Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1   );     
        PDS(   IO3_B,  Y3,	 Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2   );     
        PDS(   IO4_B,  Y4,	 Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3   );     
        PDS(   IO5_B,  Y5,	 Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4   );     
        PDS(   IO6_B,  Y6,	 Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5   );     
        PDS(   IO7_B,  Y7,	 Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6   );     
        PDS(   IO8_B,  Y8,	 Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7   );     
        PDS(   IO9_B,  Y9,	Y10,	Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8   );     
        PDS(  IO10_B,  Y10, Y11,	Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9   );     
        PDS(  IO11_B,  Y11, Y12,	Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10  );     
        PDS(  IO12_B,  Y12, Y13,	Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11  );     
        PDS(  IO13_B,  Y13, Y14,	Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12  );     
        PDS(  IO14_B,  Y14, Y15,	Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13  );     
        PDS(  IO15_B,  Y15,  Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14  );     
        PDS(  IO16_B,  Y0,	 Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15  );     
        PDS(  IO17_B,  Y1,	 Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0   );     
        PDS(  IO18_B,  Y2,	 Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1   );     
        PDS(  IO19_B,  Y3,	 Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2   );     
        PDS(  IO20_B,  Y4,	 Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3   );     
        PDS(  IO21_B,  Y5,	 Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4   );     
        PDS(  IO22_B,  Y6,	 Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5   );     
        PDS(  IO23_B,  Y7,	 Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6   );     
        PDS(  IO24_B,  Y8,	 Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7   );     
        PDS(  IO25_B,  Y9,	Y10,	Y11,Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8   );     
        PDS(  IO26_B,  Y10, Y11,	Y12,Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9   );     
        PDS(  IO27_B,  Y11, Y12,	Y13,Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10  );     
        PDS(  IO28_B,  Y12, Y13,	Y14,Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11  );     
        PDS(  IO29_B,  Y13, Y14,	Y15,Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12  );     
        PDS(  IO30_B,  Y14, Y15,	Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13  );     
        PDS(  IO31_B,  Y15,  Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14  );          
    }


   if(strcmp(cyp_name , "diag_pds_data_z15") ==0)
    {
        PDS(   IO0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0); 
        PDS(   IO1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1); 
        PDS(   IO2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2); 
        PDS(   IO3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3); 
        PDS(   IO4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4); 
        PDS(   IO5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5); 
        PDS(   IO6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6); 
        PDS(   IO7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7); 
        PDS(   IO8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8);   	
        PDS(   IO9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9);    
        PDS(  IO10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10);    
        PDS(  IO11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11);    
        PDS(  IO12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12);    
        PDS(  IO13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13);    
        PDS(  IO14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14);    
        PDS(  IO15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15);     
        PDS(  IO16,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0,    Z0); 	
        PDS(  IO17,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1,    Z1); 	
        PDS(  IO18,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2,    Z2); 
        PDS(  IO19,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3,    Z3); 	
        PDS(  IO20,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4,    Z4); 	
        PDS(  IO21,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5,    Z5); 	
        PDS(  IO22,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6,    Z6); 	
        PDS(  IO23,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7,    Z7); 	
        PDS(  IO24,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8,    Z8); 
        PDS(  IO25,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9,    Z9); 
        PDS(  IO26,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10,   Z10); 
        PDS(  IO27,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11,   Z11); 
        PDS(  IO28,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12,   Z12); 
        PDS(  IO29,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13,   Z13); 
        PDS(  IO30,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14,   Z14); 
        PDS(  IO31,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15,   Z15); 

        PDS(   IO0_B,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0);      
        PDS(   IO1_B,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1);      
        PDS(   IO2_B,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2);      
        PDS(   IO3_B,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3);      
        PDS(   IO4_B,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4);      
        PDS(   IO5_B,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5);      
        PDS(   IO6_B,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6);      
        PDS(   IO7_B,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7);      
        PDS(   IO8_B,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8);      
        PDS(   IO9_B,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9);      
        PDS(  IO10_B, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10);      
        PDS(  IO11_B, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11);      
        PDS(  IO12_B, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12);      
        PDS(  IO13_B, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13);      
        PDS(  IO14_B, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14);      
        PDS(  IO15_B, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15);      
        PDS(  IO16_B,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0,  _Z0);      
        PDS(  IO17_B,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1,  _Z1);      
        PDS(  IO18_B,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2,  _Z2);      
        PDS(  IO19_B,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3,  _Z3);      
        PDS(  IO20_B,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4,  _Z4);      
        PDS(  IO21_B,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5,  _Z5);      
        PDS(  IO22_B,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6,  _Z6);      
        PDS(  IO23_B,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7,  _Z7);      
        PDS(  IO24_B,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8,  _Z8);      
        PDS(  IO25_B,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9,  _Z9);      
        PDS(  IO26_B, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10, _Z10);      
        PDS(  IO27_B, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11, _Z11);      
        PDS(  IO28_B, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12, _Z12);      
        PDS(  IO29_B, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13, _Z13);      
        PDS(  IO30_B, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14, _Z14);      
        PDS(  IO31_B, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15, _Z15);           
    }

   if(strcmp(cyp_name , "diag_pds_data_xy23") == 0)
    {
        PDS(   IO0,  X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,_X14,_X15);     
        PDS(   IO1,  X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,_X15,_X16);     
        PDS(   IO2,  X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,_X16,_X17);     
        PDS(   IO3,  X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,_X17,_X18);     
        PDS(   IO4,  X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,_X18,_X19);     
        PDS(   IO5,  X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,_X19,_X20);     
        PDS(   IO6,  X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,_X20,_X21);     
        PDS(   IO7,  X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,_X21,_X22);     
        PDS(   IO8,  X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,_X22,_X23);     
        PDS(   IO9,  X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,_X23,_X0 );     
        PDS(  IO10,  X10,	X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,_X0 ,_X1 );     
        PDS(  IO11,  X11,	X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,Z0,	_X1 ,_X2 );     
        PDS(  IO12,  X12,	X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,Z0,	Z1,	_X2 ,_X3 );     
        PDS(  IO13,  X13,	X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,X0,	Z1,	Z2,	_X3 ,_X4 );     
        PDS(  IO14,  X14,	X15,X16,X17,X18,X19,X20,X21,X22,X23,X0,	X1,	Z2,	Z3,	_X4 ,_X5 );     
        PDS(  IO15,  X15,	X16,X17,X18,X19,X20,X21,X22,X23,X0,	X1,	X2,	Z3,	Z4,	_X5 ,_X6 );     
        PDS(  IO16,  X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,_X14,_X15);     
        PDS(  IO17,  X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,_X15,_X16);     
        PDS(  IO18,  X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,_X16,_X17);     
        PDS(  IO19,  X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,_X17,_X18);     
        PDS(  IO20,  X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,_X18,_X19);     
        PDS(  IO21,  X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,_X19,_X20);     
        PDS(  IO22,  X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,_X20,_X21);     
        PDS(  IO23,  X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,_X21,_X22);     
        PDS(  IO24,  X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,_X22,_X23);     
        PDS(  IO25,  X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,_X23,_X0 );     
        PDS(  IO26,  X10,	X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,_X0 ,_X1 );     
        PDS(  IO27,  X11,	X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,Z0,	_X1 ,_X2 );     
        PDS(  IO28,  X12,	X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,Z0,	Z1,	_X2 ,_X3 );     
        PDS(  IO29,  X13,	X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,X0,	Z1,	Z2,	_X3 ,_X4 );     
        PDS(  IO30,  X14,	X15,X16,X17,X18,X19,X20,X21,X22,X23,X0,	X1,	Z2,	Z3,	_X4 ,_X5 );     
        PDS(  IO31,  X15,	X16,X17,X18,X19,X20,X21,X22,X23,X0,	X1,	X2,	Z3,	Z4,	_X5 ,_X6 );          

        PDS(   IO0_B,  Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,_Y14,_Y15);     
        PDS(   IO1_B,  Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,_Y15,_Y16);     
        PDS(   IO2_B,  Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,_Y16,_Y17);     
        PDS(   IO3_B,  Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,_Y17,_Y18);     
        PDS(   IO4_B,  Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,_Y18,_Y19);     
        PDS(   IO5_B,  Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,_Y19,_Y20);     
        PDS(   IO6_B,  Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,_Y20,_Y21);     
        PDS(   IO7_B,  Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,_Y21,_Y22);     
        PDS(   IO8_B,  Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,_Y22,_Y23);     
        PDS(   IO9_B,  Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,_Y23,_Y0 );     
        PDS(  IO10_B,  Y10,	Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,_Y0 ,_Y1 );     
        PDS(  IO11_B,  Y11,	Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Z0,	_Y1 ,_Y2 );     
        PDS(  IO12_B,  Y12,	Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Z0,	Z1,	_Y2 ,_Y3 );     
        PDS(  IO13_B,  Y13,	Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Y0,	Z1,	Z2,	_Y3 ,_Y4 );     
        PDS(  IO14_B,  Y14,	Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Y0,	Y1,	Z2,	Z3,	_Y4 ,_Y5 );     
        PDS(  IO15_B,  Y15,	Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Y0,	Y1,	Y2,	Z3,	Z4,	_Y5 ,_Y6 );     
        PDS(  IO16_B,  Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,_Y14,_Y15);     
        PDS(  IO17_B,  Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,_Y15,_Y16);     
        PDS(  IO18_B,  Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,_Y16,_Y17);     
        PDS(  IO19_B,  Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,_Y17,_Y18);     
        PDS(  IO20_B,  Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,_Y18,_Y19);     
        PDS(  IO21_B,  Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,_Y19,_Y20);     
        PDS(  IO22_B,  Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,_Y20,_Y21);     
        PDS(  IO23_B,  Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,_Y21,_Y22);     
        PDS(  IO24_B,  Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,_Y22,_Y23);     
        PDS(  IO25_B,  Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,_Y23,_Y0 );     
        PDS(  IO26_B,  Y10,	Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,_Y0 ,_Y1 );     
        PDS(  IO27_B,  Y11,	Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Z0,	_Y1 ,_Y2 );     
        PDS(  IO28_B,  Y12,	Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Z0,	Z1,	_Y2 ,_Y3 );     
        PDS(  IO29_B,  Y13,	Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Y0,	Z1,	Z2,	_Y3 ,_Y4 );     
        PDS(  IO30_B,  Y14,	Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Y0,	Y1,	Z2,	Z3,	_Y4 ,_Y5 );     
        PDS(  IO31_B,  Y15,	Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Y0,	Y1,	Y2,	Z3,	Z4,	_Y5 ,_Y6 );          
    }
   if(strcmp(cyp_name , "diag_pds_addr_ctrl") == 0)
    {
        PDS(   ADDR0,  C0,  C1,   C2,  C3,  C4,  C5,  C6,	 C7,  C8,  C9, C10, C11, FIXH, FIXL, _C14, _C15	);     
        PDS(   ADDR1,  C1,  C2,   C3,  C4,  C5,  C6,  C7,	 C8,  C9, C10, C11, C12, FIXH, FIXL, _C15, _C0  );     
        PDS(   ADDR2,  C2,  C3,   C4,  C5,  C6,  C7,  C8,	 C9, C10, C11, C12, C13, FIXH, FIXL,  _C0, _C1 	);     
        PDS(   ADDR3,  C3,  C4,   C5,  C6,  C7,  C8,  C9,	C10, C11, C12, C13, C14, FIXH, FIXL,  _C1, _C2 	);     
        PDS(   ADDR4,  C4,  C5,   C6,  C7,  C8,  C9, C10, C11, C12, C13, C14, C15, FIXH, FIXL,  _C2, _C3 	);     
        PDS(   ADDR5,  C5,  C6,   C7,  C8,  C9, C10, C11, C12, C13, C14, C15,  C0, FIXH, FIXL,  _C3, _C4 	);     
        PDS(   ADDR6,  C6,  C7,   C8,  C9, C10, C11, C12, C13, C14, C15,  C0,	 C1, FIXH, FIXL,  _C4, _C5 	);     
        PDS(   ADDR7,  C7,  C8,   C9, C10, C11, C12, C13, C14, C15,  C0,	C1,	 C2, FIXH, FIXL,  _C5, _C6 	);     
        PDS(   ADDR8,  C8,  C9,  C10, C11, C12, C13, C14, C15,  C0,  C1,	C2,	 C3, FIXH, FIXL,  _C6, _C7 	);     
        PDS(   ADDR9,  C9,  C10, C11, C12, C13, C14, C15,  C0,  C1,  C2,	C3,	 C4, FIXH, FIXL,  _C7, _C8 	);     
        PDS(  ADDR10,  C10, C11, C12, C13, C14, C15,  C0,  C1,  C2,  C3,	C4,	 C5, FIXH, FIXL,  _C8, _C9 	);     
        PDS(  ADDR11,  C11, C12, C13, C14, C15,  C0,	C1,  C2,  C3,  C4,	C5,	 C6, FIXH, FIXL,  _C9,_C10 	);     
        PDS(  ADDR12,  C12, C13, C14, C15,  C0,  C1,	C2,  C3,  C4,  C5,	C6,	 C7, FIXH, FIXL, _C10,_C11 	);     
        PDS(  ADDR13,  C13, C14, C15,  C0,  C1,  C2,	C3,  C4,  C5,  C6,	C7,	 C8, FIXH, FIXL, _C11,_C12 	);     
        PDS(  ADDR14,  C14, C15,   C0,  C1,  C2,  C3,	C4,  C5,  C6,  C7,	C8,	 C9, FIXH, FIXL, _C12,_C13 	);     
        PDS(  ADDR15,  C15,  C0,   C1,  C2,  C3,  C4,	C5,  C6,  C7,  C8,	C9,	C10, FIXH, FIXL, _C13,_C14 	);     
        PDS(  ADDR16,  C0,   C1,   C2,  C3,  C4,  C5,	C6,  C7,  C8,  C9, C10, C11, FIXH, FIXL, _C14,_C15	);     
        PDS(  ADDR17,  C1,   C2,   C3,  C4,  C5,  C6,	C7,  C8,  C9, C10, C11, C12, FIXH, FIXL, _C15, _C0 	);     
        PDS(  ADDR18,  C2,   C3,   C4,  C5,  C6,  C7,	C8,  C9, C10, C11, C12, C13, FIXH, FIXL,  _C0, _C1 	);     
        PDS(  ADDR19,  C3,   C4,   C5,  C6,  C7,  C8,	C9,	C10, C11, C12, C13, C14, FIXH, FIXL,  _C1, _C2 	);     
        PDS(  ADDR20,  C4,   C5,   C6,  C7,  C8,  C9, C10, C11, C12, C13, C14, C15, FIXH, FIXL,  _C2, _C3 	);     
        PDS(  ADDR21,  C5,   C6,   C7,  C8,  C9, C10, C11, C12, C13, C14, C15,  C0, FIXH, FIXL,  _C3, _C4 	);     
        PDS(  ADDR22,  C6,   C7,   C8,  C9, C10, C11, C12, C13, C14, C15,  C0,	 C1, FIXH, FIXL,  _C4, _C5 	);     
        PDS(  ADDR23,  C7,   C8,   C9, C10, C11, C12, C13, C14, C15,  C0,	C1,	 C2, FIXH, FIXL,  _C5, _C6 	);     
        PDS(  ADDR24,  C8,   C9, C10, C11, C12, C13, C14, C15,  C0,  C1,	C2,	 C3, FIXH, FIXL,  _C6, _C7 	);     
        PDS(  ADDR25,  C9,  C10, C11, C12, C13, C14, C15,  C0,  C1,  C2,	C3,	 C4, FIXH, FIXL,  _C7, _C8 	);     
        PDS(  ADDR26,  C10, C11, C12, C13, C14, C15,  C0,	 C1,  C2,  C3,	C4,	 C5, FIXH, FIXL,  _C8, _C9 	);     
        PDS(  ADDR27,  C11, C12, C13, C14, C15,  C0,	C1,	 C2,  C3,  C4,	C5,	 C6, FIXH, FIXL,  _C9,_C10 	);     
        PDS(  ADDR28,  C12, C13, C14, C15,  C0,  C1,	C2,	 C3,  C4,  C5,	C6,	 C7, FIXH, FIXL, _C10,_C11 	);     
        PDS(  ADDR29,  C13, C14, C15,  C0,  C1,  C2,	C3,	 C4,  C5,  C6,	C7,	 C8, FIXH, FIXL, _C11,_C12 	);     
        PDS(  ADDR30,  C14, C15,  C0,  C1,  C2,  C3,	C4,	 C5,  C6,  C7,	C8,	 C9, FIXH, FIXL, _C12,_C13 	);     
        PDS(  ADDR31,  C15,  C0,  C1,  C2,  C3,  C4,	C5,	 C6,  C7,  C8,	C9,	C10, FIXH, FIXL, _C13,_C14 	);    

        PDS(   ADDR0_B,  C16, C17,C18,C19,C20,C21,C22,C23,C24,C25,C26,C27,FIXL,FIXH,_C30,_C31);     
        PDS(   ADDR1_B,  C17, C18,C19,C20,C21,C22,C23,C24,C25,C26,C27,C28,FIXL,FIXH,_C31,_C16);     
        PDS(   ADDR2_B,  C18, C19,C20,C21,C22,C23,C24,C25,C26,C27,C28,C29,FIXL,FIXH,_C16,_C17);     
        PDS(   ADDR3_B,  C19, C20,C21,C22,C23,C24,C25,C26,C27,C28,C29,C30,FIXL,FIXH,_C17,_C18);     
        PDS(   ADDR4_B,  C20, C21,C22,C23,C24,C25,C26,C27,C28,C29,C30,C31,FIXL,FIXH,_C18,_C19);     
        PDS(   ADDR5_B,  C21, C22,C23,C24,C25,C26,C27,C28,C29,C30,C31,C16,FIXL,FIXH,_C19,_C20);     
        PDS(   ADDR6_B,  C22, C23,C24,C25,C26,C27,C28,C29,C30,C31,C16,C17,FIXL,FIXH,_C20,_C21);     
        PDS(   ADDR7_B,  C23, C24,C25,C26,C27,C28,C29,C30,C31,C16,C17,C18,FIXL,FIXH,_C21,_C22);     
        PDS(   ADDR8_B,  C24, C25,C26,C27,C28,C29,C30,C31,C16,C17,C18,C19,FIXL,FIXH,_C22,_C23);     
        PDS(   ADDR9_B,  C25, C26,C27,C28,C29,C30,C31,C16,C17,C18,C19,C20,FIXL,FIXH,_C23,_C24);     
        PDS(  ADDR10_B,  C26, C27,C28,C29,C30,C31,C16,C17,C18,C19,C20,C21,FIXL,FIXH,_C24,_C25);     
        PDS(  ADDR11_B,  C27, C28,C29,C30,C31,C16,C17,C18,C19,C20,C21,C22,FIXL,FIXH,_C25,_C26);     
        PDS(  ADDR12_B,  C28, C29,C30,C31,C16,C17,C18,C19,C20,C21,C22,C23,FIXL,FIXH,_C26,_C27);     
        PDS(  ADDR13_B,  C29, C30,C31,C16,C17,C18,C19,C20,C21,C22,C23,C24,FIXL,FIXH,_C27,_C28);     
        PDS(  ADDR14_B,  C30, C31,C16,C17,C18,C19,C20,C21,C22,C23,C24,C25,FIXL,FIXH,_C28,_C29);     
        PDS(  ADDR15_B,  C31, C16,C17,C18,C19,C20,C21,C22,C23,C24,C25,C26,FIXL,FIXH,_C29,_C30);     
        PDS(  ADDR16_B,  C16, C17,C18,C19,C20,C21,C22,C23,C24,C25,C26,C27,FIXL,FIXH,_C30,_C31);     
        PDS(  ADDR17_B,  C17, C18,C19,C20,C21,C22,C23,C24,C25,C26,C27,C28,FIXL,FIXH,_C31,_C16);     
        PDS(  ADDR18_B,  C18, C19,C20,C21,C22,C23,C24,C25,C26,C27,C28,C29,FIXL,FIXH,_C16,_C17);     
        PDS(  ADDR19_B,  C19, C20,C21,C22,C23,C24,C25,C26,C27,C28,C29,C30,FIXL,FIXH,_C17,_C18);     
        PDS(  ADDR20_B,  C20, C21,C22,C23,C24,C25,C26,C27,C28,C29,C30,C31,FIXL,FIXH,_C18,_C19);     
        PDS(  ADDR21_B,  C21, C22,C23,C24,C25,C26,C27,C28,C29,C30,C31,C16,FIXL,FIXH,_C19,_C20);     
        PDS(  ADDR22_B,  C22, C23,C24,C25,C26,C27,C28,C29,C30,C31,C16,C17,FIXL,FIXH,_C20,_C21);     
        PDS(  ADDR23_B,  C23, C24,C25,C26,C27,C28,C29,C30,C31,C16,C17,C18,FIXL,FIXH,_C21,_C22);     
        PDS(  ADDR24_B,  C24, C25,C26,C27,C28,C29,C30,C31,C16,C17,C18,C19,FIXL,FIXH,_C22,_C23);     
        PDS(  ADDR25_B,  C25, C26,C27,C28,C29,C30,C31,C16,C17,C18,C19,C20,FIXL,FIXH,_C23,_C24);     
        PDS(  ADDR26_B,  C26, C27,C28,C29,C30,C31,C16,C17,C18,C19,C20,C21,FIXL,FIXH,_C24,_C25);     
        PDS(  ADDR27_B,  C27, C28,C29,C30,C31,C16,C17,C18,C19,C20,C21,C22,FIXL,FIXH,_C25,_C26);     
        PDS(  ADDR28_B,  C28, C29,C30,C31,C16,C17,C18,C19,C20,C21,C22,C23,FIXL,FIXH,_C26,_C27);     
        PDS(  ADDR29_B,  C29, C30,C31,C16,C17,C18,C19,C20,C21,C22,C23,C24,FIXL,FIXH,_C27,_C28);     
        PDS(  ADDR30_B,  C30, C31,C16,C17,C18,C19,C20,C21,C22,C23,C24,C25,FIXL,FIXH,_C28,_C29);     
        PDS(  ADDR31_B,  C31, C16,C17,C18,C19,C20,C21,C22,C23,C24,C25,C26,FIXL,FIXH,_C29,_C30);          
    }
    if(strcmp(cyp_name , "diag_pds_addr_xy23")==0)
    {
        PDS(   ADDR0,  X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,_X14,_X15);     
        PDS(   ADDR1,  X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,_X15,_X16);     
        PDS(   ADDR2,  X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,_X16,_X17);     
        PDS(   ADDR3,  X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,_X17,_X18);     
        PDS(   ADDR4,  X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,_X18,_X19);     
        PDS(   ADDR5,  X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,_X19,_X20);     
        PDS(   ADDR6,  X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,_X20,_X21);     
        PDS(   ADDR7,  X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,_X21,_X22);     
        PDS(   ADDR8,  X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,_X22,_X23);     
        PDS(   ADDR9,  X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,_X23,_X0 );     
        PDS(  ADDR10,  X10,	X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,_X0 ,_X1 );     
        PDS(  ADDR11,  X11,	X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,Z0,	_X1 ,_X2 );     
        PDS(  ADDR12,  X12,	X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,Z0,	Z1,	_X2 ,_X3 );     
        PDS(  ADDR13,  X13,	X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,X0,	Z1,	Z2,	_X3 ,_X4 );     
        PDS(  ADDR14,  X14,	X15,X16,X17,X18,X19,X20,X21,X22,X23,X0,	X1,	Z2,	Z3,	_X4 ,_X5 );     
        PDS(  ADDR15,  X15,	X16,X17,X18,X19,X20,X21,X22,X23,X0,	X1,	X2,	Z3,	Z4,	_X5 ,_X6 );     
        PDS(  ADDR16,  X0,	X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,_X14,_X15);     
        PDS(  ADDR17,  X1,	X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,_X15,_X16);     
        PDS(  ADDR18,  X2,	X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,_X16,_X17);     
        PDS(  ADDR19,  X3,	X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,_X17,_X18);     
        PDS(  ADDR20,  X4,	X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,_X18,_X19);     
        PDS(  ADDR21,  X5,	X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,_X19,_X20);     
        PDS(  ADDR22,  X6,	X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,_X20,_X21);     
        PDS(  ADDR23,  X7,	X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,_X21,_X22);     
        PDS(  ADDR24,  X8,	X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,_X22,_X23);     
        PDS(  ADDR25,  X9,	X10,X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,_X23,_X0 );     
        PDS(  ADDR26,  X10,	X11,X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,_X0 ,_X1 );     
        PDS(  ADDR27,  X11,	X12,X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,Z0,	_X1 ,_X2 );     
        PDS(  ADDR28,  X12,	X13,X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,Z0,	Z1,	_X2 ,_X3 );     
        PDS(  ADDR29,  X13,	X14,X15,X16,X17,X18,X19,X20,X21,X22,X23,X0,	Z1,	Z2,	_X3 ,_X4 );     
        PDS(  ADDR30,  X14,	X15,X16,X17,X18,X19,X20,X21,X22,X23,X0,	X1,	Z2,	Z3,	_X4 ,_X5 );     
        PDS(  ADDR31,  X15,	X16,X17,X18,X19,X20,X21,X22,X23,X0,	X1,	X2,	Z3,	Z4,	_X5 ,_X6 );    

        PDS(   ADDR0_B,  Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,_Y14,_Y15);     
        PDS(   ADDR1_B,  Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,_Y15,_Y16);     
        PDS(   ADDR2_B,  Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,_Y16,_Y17);     
        PDS(   ADDR3_B,  Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,_Y17,_Y18);     
        PDS(   ADDR4_B,  Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,_Y18,_Y19);     
        PDS(   ADDR5_B,  Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,_Y19,_Y20);     
        PDS(   ADDR6_B,  Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,_Y20,_Y21);     
        PDS(   ADDR7_B,  Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,_Y21,_Y22);     
        PDS(   ADDR8_B,  Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,_Y22,_Y23);     
        PDS(   ADDR9_B,  Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,_Y23,_Y0 );     
        PDS(  ADDR10_B,  Y10,	Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,_Y0 ,_Y1 );     
        PDS(  ADDR11_B,  Y11,	Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Z0,	_Y1 ,_Y2 );     
        PDS(  ADDR12_B,  Y12,	Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Z0,	Z1,	_Y2 ,_Y3 );     
        PDS(  ADDR13_B,  Y13,	Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Y0,	Z1,	Z2,	_Y3 ,_Y4 );     
        PDS(  ADDR14_B,  Y14,	Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Y0,	Y1,	Z2,	Z3,	_Y4 ,_Y5 );     
        PDS(  ADDR15_B,  Y15,	Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Y0,	Y1,	Y2,	Z3,	Z4,	_Y5 ,_Y6 );     
        PDS(  ADDR16_B,  Y0,	Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,_Y14,_Y15);     
        PDS(  ADDR17_B,  Y1,	Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,_Y15,_Y16);     
        PDS(  ADDR18_B,  Y2,	Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,_Y16,_Y17);     
        PDS(  ADDR19_B,  Y3,	Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,_Y17,_Y18);     
        PDS(  ADDR20_B,  Y4,	Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,_Y18,_Y19);     
        PDS(  ADDR21_B,  Y5,	Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,_Y19,_Y20);     
        PDS(  ADDR22_B,  Y6,	Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,_Y20,_Y21);     
        PDS(  ADDR23_B,  Y7,	Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,_Y21,_Y22);     
        PDS(  ADDR24_B,  Y8,	Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,_Y22,_Y23);     
        PDS(  ADDR25_B,  Y9,	Y10,Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,_Y23,_Y0 );     
        PDS(  ADDR26_B,  Y10,	Y11,Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,_Y0 ,_Y1 );     
        PDS(  ADDR27_B,  Y11,	Y12,Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Z0,	_Y1 ,_Y2 );     
        PDS(  ADDR28_B,  Y12,	Y13,Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Z0,	Z1,	_Y2 ,_Y3 );     
        PDS(  ADDR29_B,  Y13,	Y14,Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Y0,	Z1,	Z2,	_Y3 ,_Y4 );     
        PDS(  ADDR30_B,  Y14,	Y15,Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Y0,	Y1,	Z2,	Z3,	_Y4 ,_Y5 );     
        PDS(  ADDR31_B,  Y15,	Y16,Y17,Y18,Y19,Y20,Y21,Y22,Y23,Y0,	Y1,	Y2,	Z3,	Z4,	_Y5 ,_Y6 );          
    }   
 if(strcmp(cyp_name , "diag_pds_addr_z15")==0)
    {
        
        PDS(   ADDR0,  Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15);
        PDS(   ADDR1,  Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0);
        PDS(   ADDR2,  Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1);
        PDS(   ADDR3,  Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2);
        PDS(   ADDR4,  Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3);
        PDS(   ADDR5,  Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4);
        PDS(   ADDR6,  Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5);
        PDS(   ADDR7,  Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6);
        PDS(   ADDR8,  Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7);
        PDS(   ADDR9,  Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8);
        PDS(  ADDR10,  Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9);
        PDS(  ADDR11,  Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10);
        PDS(  ADDR12,  Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11);
        PDS(  ADDR13,  Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12);
        PDS(  ADDR14,  Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13);
        PDS(  ADDR15,  Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14);
        PDS(  ADDR16,  Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15);
        PDS(  ADDR17,  Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0);
        PDS(  ADDR18,  Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1);
        PDS(  ADDR19,  Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2);
        PDS(  ADDR20,  Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3);
        PDS(  ADDR21,  Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4);
        PDS(  ADDR22,  Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5);
        PDS(  ADDR23,  Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6);
        PDS(  ADDR24,  Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7);
        PDS(  ADDR25,  Z9,	Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8);
        PDS(  ADDR26,  Z10,	Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9);
        PDS(  ADDR27,  Z11,	Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10);
        PDS(  ADDR28,  Z12,	Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11);
        PDS(  ADDR29,  Z13,	Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12);
        PDS(  ADDR30,  Z14,	Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13);
        PDS(  ADDR31,  Z15,	Z0,	Z1,	Z2,	Z3,	Z4,	Z5,	Z6,	Z7,	Z8,	Z9,	Z10,	Z11,	Z12,	Z13,	Z14);

        PDS(   ADDR0_B, _Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15);
        PDS(   ADDR1_B, _Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0);
        PDS(   ADDR2_B, _Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1);
        PDS(   ADDR3_B, _Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2);
        PDS(   ADDR4_B, _Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3);
        PDS(   ADDR5_B, _Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4);
        PDS(   ADDR6_B, _Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5);
        PDS(   ADDR7_B, _Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6);
        PDS(   ADDR8_B, _Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7);
        PDS(   ADDR9_B, _Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8);
        PDS(  ADDR10_B, _Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9);
        PDS(  ADDR11_B, _Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10);
        PDS(  ADDR12_B, _Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11);
        PDS(  ADDR13_B, _Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12);
        PDS(  ADDR14_B, _Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13);
        PDS(  ADDR15_B, _Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14);
        PDS(  ADDR16_B, _Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15);
        PDS(  ADDR17_B, _Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0);
        PDS(  ADDR18_B, _Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1);
        PDS(  ADDR19_B, _Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2);
        PDS(  ADDR20_B, _Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3);
        PDS(  ADDR21_B, _Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4);
        PDS(  ADDR22_B, _Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5);
        PDS(  ADDR23_B, _Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6);
        PDS(  ADDR24_B, _Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7);
        PDS(  ADDR25_B, _Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8);
        PDS(  ADDR26_B, _Z10,	_Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9);
        PDS(  ADDR27_B, _Z11,	_Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10);
        PDS(  ADDR28_B, _Z12,	_Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11);
        PDS(  ADDR29_B, _Z13,	_Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12);
        PDS(  ADDR30_B, _Z14,	_Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13);
        PDS(  ADDR31_B, _Z15,	_Z0,	_Z1,	_Z2,	_Z3,	_Z4,	_Z5,	_Z6,	_Z7,	_Z8,	_Z9,	_Z10,	_Z11,	_Z12,	_Z13,	_Z14);
        
    }   
        PDS(  DRE ,  C19  );     
        PDS(  STRB,  C17  );     
}

void load_pds_of_dpscal (const char* cyp_name )
{ 
//---------------------------------------------------------------------------------------------------------------------------------------
//                  CYP0, CYP1, CYP2, CYP3, CYP4, CYP5, CYP6, CYP7, CYP8, CYP9,CYP10,CYP11,CYP12,CYP13,CYP14,CYP15
//---------------------------------------------------------------------------------------------------------------------------------------
        PDS(  ADDR0,   X0);   
        PDS(  ADDR1,   X1);   
        PDS(  ADDR2,   X2);   
        PDS(  ADDR3,   X3);   
        PDS(  ADDR4,   X4);   
        PDS(  ADDR5,   X5);   
        PDS(  ADDR6,   X6);   
        PDS(  ADDR7,   X7);   
        PDS(  ADDR8,   X8);   
        PDS(  ADDR9,   X9);   
        PDS( ADDR10,  X10);  
        PDS( ADDR11,  X11);  
        PDS( ADDR12,  X12);  
        PDS( ADDR13,  X13); 
        PDS( ADDR14,  X14);  
        PDS( ADDR15,  X15);  
        PDS( ADDR16,   X0);  
        PDS( ADDR17,   X1);     
        PDS( ADDR18,   X2);     
        PDS( ADDR19,   X3);     
        PDS( ADDR20,   X4);     
        PDS( ADDR21,   X5);     
        PDS( ADDR22,   X6);     
        PDS( ADDR23,   X7);     
        PDS( ADDR24,   X8);     
        PDS( ADDR25,   X9);     
        PDS( ADDR26,  X10);     
        PDS( ADDR27,  X11);     
        PDS( ADDR28,  X12);     
        PDS( ADDR29,  X13);     
        PDS( ADDR30,  X14);     
        PDS( ADDR31,  X15);     
       
        PDS(   CLK0,  C0 );    
        PDS(   CLK1,  C1 );    
        PDS(   CLK2,  C2 );    
        PDS(   CLK3,  C3 );    
        PDS(   CLK4,  C4 );    
        PDS(   CLK5,  C5 );    
        PDS(   CLK6,  C6 );    
        PDS(   CLK7,  C7 );    

        PDS(  SCAN0,  S0  );
        PDS(  SCAN1,  S1  );
        PDS(  SCAN2,  S2  );
        PDS(  SCAN3,  S3  );
        PDS(  SCAN4,  S4  );
        PDS(  SCAN5,  S5  );
        PDS(  SCAN6,  S6  );
        PDS(  SCAN7,  S7  );
        PDS(  SCAN8,  S8  );
        PDS(  SCAN9,  S9  );
        PDS( SCAN10,  S10  );
        PDS( SCAN11,  S11  );
        PDS( SCAN12,  S12  );
        PDS( SCAN13,  S13  );
        PDS( SCAN14,  S14  );
        PDS( SCAN15,  S15  );
        PDS( SCAN16,  S16  );
        PDS( SCAN17,  S17  );
        PDS( SCAN18,  S18  );
        PDS( SCAN19,  S19  );
        PDS( SCAN20,  S20  ); 
        PDS( SCAN21,  S21  ); 
        PDS( SCAN22,  S22  ); 
        PDS( SCAN23,  S23  ); 
        PDS( SCAN24,  S24  ); 
        PDS( SCAN25,  S25  ); 
        PDS( SCAN26,  S26  ); 
        PDS( SCAN27,  S27  ); 
        PDS( SCAN28,  S28  ); 
        PDS( SCAN29,  S29  ); 
        PDS( SCAN30,  S30  ); 
        PDS( SCAN31,  S31  ); 

 
        PDS(    IO0,   D8  );
        PDS(    IO1,   D9  );
        PDS(    IO2,   D10 );
        PDS(    IO3,   D11 );
        PDS(    IO4,   D12 );
        PDS(    IO5,   D5  );
        PDS(    IO6,   D6  );
        PDS(    IO7,   D7  );
        PDS(    IO8,   D0  );
        PDS(    IO9,   D1  );
        PDS(   IO10,   D2  );
        PDS(   IO11,   D3  );
        PDS(   IO12,   D4  );
        PDS(   IO13,   D13 );
        PDS(   IO14,   D14 );
        PDS(   IO15,   D15 );
        PDS(   IO16,   D0  );
        PDS(   IO17,   D1  );
        PDS(   IO18,   D2  );
        PDS(   IO19,   D3  );
        PDS(   IO20,   D4  );
        PDS(   IO21,   D5  );
        PDS(   IO22,   D6  );
        PDS(   IO23,   D7  );
        PDS(   IO24,   D8  );
        PDS(   IO25,   D9  );
        PDS(   IO26,   D10 );
        PDS(   IO27,   D11 );
        PDS(   IO28,   D12 );
        PDS(   IO29,   D13 );
        PDS(   IO30,   D14 );
        PDS(   IO31,   D15 );

        PDS(  DRE ,  C23  );     
        PDS(  STRB,  C24  );    
}

void load_scr(const char* scr_name)
{
    ENABLE_SCRAM( true, SCR_ADR);
 	ENABLE_SCRAM( true, SCR_DATA);
  	
    SCRAM(x0  = ax0 );
    SCRAM(x1  = ax1	);
    SCRAM(x2  = ax2 );
    SCRAM(x3  = ax3 );
    SCRAM(x4  = ax4	);
    SCRAM(x5  = ax5	);
    SCRAM(x6  = ax6	);
    SCRAM(x7  = ax7	);
    SCRAM(x8  = ax8	);
    SCRAM(x9  = ax9	);
    SCRAM(x10 = ax10);
    SCRAM(x11 = ax11);
    SCRAM(x12 = ax12);
    SCRAM(x13 = ax13);
    SCRAM(x14 = ax14);
    SCRAM(x15 = ax15);
    SCRAM(x16 = ax16);
    SCRAM(x17 = ax17);
    SCRAM(x18 = ax18);
    SCRAM(x19 = ax19);
    SCRAM(x20 = ax20);
    SCRAM(x21 = ax21);
    SCRAM(x22 = ax22);
    SCRAM(x23 = ax23);
  
	SCRAM(y0  = ay0 )   ;
    SCRAM(y1  = ay1 )   ;
    SCRAM(y2  = ay2 )   ;
    SCRAM(y3  = ay3 )   ;
    SCRAM(y4  = ay4 )   ;
    SCRAM(y5  = ay5 )   ;
    SCRAM(y6  = ay6 )   ;
    SCRAM(y7  = ay7 )   ;
    SCRAM(y8  = ay8 )   ;
    SCRAM(y9  = ay9 )   ;
    SCRAM(y10 = ay10)   ;
    SCRAM(y11 = ay11)   ;
    SCRAM(y12 = ay12)   ;
    SCRAM(y13 = ay13)	;
    SCRAM(y14 = ay14)	;
    SCRAM(y15 = ay15)	;
    SCRAM(y16 = ay16)	;
    SCRAM(y17 = ay17)	;
    SCRAM(y18 = ay18)	;
    SCRAM(y19 = ay19)	;
    SCRAM(y20 = ay20)	;
    SCRAM(y21 = ay21)	;
    SCRAM(y22 = ay22)	;
    SCRAM(y23 = ay23)	;

    if(strcmp(scr_name , "diag_init") == 0)
    {
        SCRAM(d0  = ax0 ^ ax0);
 	    SCRAM(d1  = ax0 ^ ax0);
 	    SCRAM(d2  = ax0 ^ ax0);
 	    SCRAM(d3  = ax0 ^ ax0);
 	    SCRAM(d4  = ax0 ^ ax0);
 	    SCRAM(d5  = ax0 ^ ax0);
 	    SCRAM(d6  = ax0 ^ ax0);
 	    SCRAM(d7  = ax0 ^ ax0);
 	    SCRAM(d8  = ax0 ^ ax0);
 	    SCRAM(d9  = ax0 ^ ax0);
 	    SCRAM(d10 = ax0 ^ ax0);
 	    SCRAM(d11 = ax0 ^ ax0);
 	    SCRAM(d12 = ax0 ^ ax0);
 	    SCRAM(d13 = ax0 ^ ax0);
 	    SCRAM(d14 = ax0 ^ ax0);
        SCRAM(d15 = ax0 ^ ax0);

    }
 
    if(strcmp(scr_name , "diag_bit_scr_00") == 0)
    {
        SCRAM(d0  = ax0  ^ ay0 );
 	    SCRAM(d1  = ax1  | ay1 );
 	    SCRAM(d2  = ax2  & ay2 );
 	    SCRAM(d3  = ax3  ^ ay3  ^ ax3 );
 	    SCRAM(d4  = ax4  | ay4  ^ ax4 );
 	    SCRAM(d5  = ax5  & ay5  ^ ax5 );
 	    SCRAM(d6  = ax6  ^ ay6  | ax6 );
 	    SCRAM(d7  = ax7  | ay7  | ax7 );
 	    SCRAM(d8  = ax8  & ay8  | ax8 );
 	    SCRAM(d9  = ax9  ^ ay9  & ax9 );
 	    SCRAM(d10 = ax10 | ay10 & ax10);
 	    SCRAM(d11 = ax11 & ay11 & ax11);      
 	    SCRAM(d12 = ax12 ^ ay12 ^ ax12 ^ ay12);
 	    SCRAM(d13 = ax13 | ay13 ^ ax13 ^ ay13);
 	    SCRAM(d14 = ax14 & ay14 ^ ax14 ^ ay14);
        SCRAM(d15 = ! ax15 ^ ay15);
    }
   if(strcmp(scr_name , "diag_bit_scr_01") == 0)
    {

         
	    SCRAM(d0  = ax0 ^ ay0 | ax0 ^ ay0  );
	    SCRAM(d1  = ax1 | ay1 | ax1 ^ ay1  );
	    SCRAM(d2  = ax2 & ay2 | ax2 ^ ay2  );
	    SCRAM(d3  = ax3 ^ ay3 & ax3 ^ ay3  );
	    SCRAM(d4  = ax4 | ay4 & ax4 ^ ay4  );
	    SCRAM(d5  = ax5 & ay5 & ax5 ^ ay5  );
	    SCRAM(d6  = ax6 ^ ay6 ^ ax6 | ay6  ); 
	    SCRAM(d7  = ax7 | ay7 ^ ax7 | ay7  ); 
	    SCRAM(d8  = ax8 & ay8 ^ ax8 | ay8  ); 
	    SCRAM(d9  = ax9 ^ ay9 | ax9 | ay9  ); 
	    SCRAM(d10 = ax10 | ay10 | ax10 | ay10 ); 
	    SCRAM(d11 = ax11 & ay11 | ax11 | ay11 ); 
	    SCRAM(d12 = ax12 ^ ay12 & ax12 | ay12 ); 
	    SCRAM(d13 = ax13 | ay13 & ax13 | ay13 ); 
	    SCRAM(d14 = ax14 & ay14 & ax14 | ay14 ); 
	    SCRAM(d15 = ! ax14 & ay14 & ax14 | ay14 ); 
    }
   if(strcmp(scr_name , "diag_bit_scr_02") ==0 )
    {
        SCRAM(d0  = ax0 ^ ay0 ^ ax0 & ay0  );
	    SCRAM(d1  = ax1 | ay1 ^ ax1 & ay1  );
	    SCRAM(d2  = ax2 & ay2 ^ ax2 & ay2  );
	    SCRAM(d3  = ax3 ^ ay3 | ax3 & ay3  );
	    SCRAM(d4  = ax4 | ay4 | ax4 & ay4  );
	    SCRAM(d5  = ax5 & ay5 | ax5 & ay5  );
	    SCRAM(d6  = ax6 ^ ay6 & ax6 & ay6  );
	    SCRAM(d7  = ax7 | ay7 & ax7 & ay7  );
	    SCRAM(d8  = ax8 & ay8 & ax8 & ay8  );
    }
    
}

void testPG_opcode()
{
    /*
	int fm_cnt;
   	double strb0 = 45;
    double strb1 = 0;

    	writeCpeDelay(1,1);
        ECLK( STRB   ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 ,strb0 );
        ECLK( STRB_B ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 ,strb1 );

*/
    diag_log("============ %s ============\n", __FUNCTION__);
	load_pds("diag_pds_data_data16");
	SEND_MPAT("diag_opcode.mpa");

 	MEAS_MPAT("OPCODE_NOP_00");
    int fm_size=diag_readFmData();
    compFm("OPCODE_NOP_00",(1<<0), 16, 0x0001,	0x0002,	0x0004,	0x0008,	0x0010,	0x0020,	0x0040,	0x0080,	0x0100,	0x0200,	0x0400,	0x0800,	0x1000,	0x2000,	0x4000,	0x8000);
    compFm("OPCODE_NOP_00",(1<<1), 16, 0x000A,	0x000B,	0x000C,	0x000D,	0x00A0,	0x00B0,	0x00C0,	0x00D0,	0x0A00,	0x0B00,	0x0C00,	0x0D00,	0xA000,	0xB000,	0xC000,	0xD000);
    diag_showDrv(0,fm_size);
    diag_showData(0,fm_size);

    MEAS_MPAT("OPCODE_JMP_00");
    compFm("OPCODE_JMP_00",(1<<0), 16, 0x0000,	0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006,	0x0007,	0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F);
    compFm("OPCODE_JMP_00",(1<<1), 16, 0xffff,	0xfffe,	0xfffd,	0xfffc,	0xfffb,	0xfffa,	0xfff9,	0xfff8,	0xfff7, 0xfff6, 0xfff5, 0xfff4, 0xfff3, 0xfff2, 0xfff1, 0xfff0);
   

    MEAS_MPAT("OPCODE_JSR_00");
    compFm("OPCODE_JSR_00",(1<<0), 16, 0x0001,  0x0002, 0x0004, 0x0008, 0x0011,	0x0022,	0x0044,	0x0088,	0x0111,	0x0222,	0x0444,	0x0888,	0x1111,	0x2222,	0x4444,	0x8888);
    compFm("OPCODE_JSR_00",(1<<1), 16, 0x000a,  0x000b, 0x000c, 0x000d, 0x00aa,	0x00bb,	0x00cc,	0x00dd,	0x0aaa,	0x0bbb,	0x0ccc,	0x0ddd,	0xaaaa,	0xbbbb,	0xcccc,	0xdddd);
   
	SET_JNCFLAG(0x1);
    MEAS_MPAT("OPCODE_JNC_00");
    compFm("OPCODE_JNC_00",(1<<0), 16, 0x1000,	0x1001,	0x1002,	0x1003, 0x2000,	0x2001,	0x2002, 0x2003,	0x4000,	0x4001,	0x4002, 0x4003,	0x8000,	0x8001,	0x8002, 0x8003);
    compFm("OPCODE_JNC_00",(1<<1), 16, 0xB000,	0xB001,	0xB002,	0xB003, 0xC000,	0xC001,	0xC002, 0xC003,	0xD000,	0xD001,	0xD002, 0xD003,	0xE000,	0xE001,	0xE002,	0xE003);	

	SET_JNCFLAG(0x2);
    MEAS_MPAT("OPCODE_JNC_00");
    compFm("OPCODE_JNC_00",(1<<0), 16, 0x0000,	0x0001,	0x0002,	0x0003, 0x2000,	0x2001,	0x2002, 0x2003,	0x4000,	0x4001,	0x4002, 0x4003,	0x8000,	0x8001,	0x8002, 0x8003);
    compFm("OPCODE_JNC_00",(1<<1), 16, 0xA000,	0xA001,	0xA002,	0xA003, 0xC000,	0xC001,	0xC002,	0xC003, 0xD000,	0xD001,	0xD002, 0xD003,	0xE000,	0xE001,	0xE002,	0xE003); 

    SET_JNCFLAG(0x20);
    MEAS_MPAT("OPCODE_JNC_01");
    compFm("OPCODE_JNC_01",(1<<0), 16, 0x1000,	0x1001,	0x1002, 0x1003, 0x2000,	0x2001,	0x2002,	0x2003, 0x4000,	0x4001,	0x4002, 0x4003,0x8000,	0x8001,	0x8002, 0x8003);
    compFm("OPCODE_JNC_01",(1<<1), 16, 0x1000,	0x1001,	0x1002, 0x1003, 0x2000,	0x2001,	0x2002,	0x2003, 0x4000,	0x4001,	0x4002, 0x4003,0x8000,	0x8001,	0x8002, 0x8003); 

    SET_JNCFLAG(0x40);
    MEAS_MPAT("OPCODE_JNC_01");
    compFm("OPCODE_JNC_01",(1<<0), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x2000,	0x2001,	0x2002, 0x2003, 0x4000,	0x4001,	0x4002, 0x4003,0x8000,	0x8001,	0x8002, 0x8003);
    compFm("OPCODE_JNC_01",(1<<1), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x2000,	0x2001,	0x2002, 0x2003, 0x4000,	0x4001,	0x4002, 0x4003,0x8000,	0x8001,	0x8002, 0x8003); 

    SET_JNCFLAG(0x80);
    MEAS_MPAT("OPCODE_JNC_01");
    compFm("OPCODE_JNC_01",(1<<0), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x1000, 0x1001, 0x1002, 0x1003, 0x4000, 0x4001, 0x4002, 0x4003, 0x8000, 0x8001,	0x8002, 0x8003);
    compFm("OPCODE_JNC_01",(1<<1), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x1000, 0x1001, 0x1002, 0x1003, 0x4000, 0x4001, 0x4002, 0x4003, 0x8000, 0x8001,	0x8002, 0x8003); 

    SET_JNCFLAG(0x100);
    MEAS_MPAT("OPCODE_JNC_01");
    compFm("OPCODE_JNC_01",(1<<0), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x1000, 0x1001, 0x1002, 0x1003, 0x2000,	0x2001,	0x2002,	0x2003, 0x8000, 0x8001,	0x8002, 0x8003);
    compFm("OPCODE_JNC_01",(1<<1), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x1000, 0x1001, 0x1002, 0x1003, 0x2000,	0x2001,	0x2002,	0x2003, 0x8000, 0x8001,	0x8002, 0x8003); 

    SET_JNCFLAG(0x200);
    MEAS_MPAT("OPCODE_JNC_01");
    compFm("OPCODE_JNC_01",(1<<0), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x1000, 0x1001, 0x1002, 0x1003, 0x2000,	0x2001,	0x2002,	0x2003, 0x4000, 0x4001, 0x4002, 0x4003);
    compFm("OPCODE_JNC_01",(1<<1), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x1000, 0x1001, 0x1002, 0x1003, 0x2000,	0x2001,	0x2002,	0x2003, 0x4000, 0x4001, 0x4002, 0x4003); 

    SET_JNCFLAG(0x400);
    MEAS_MPAT("OPCODE_JNC_02");
    compFm("OPCODE_JNC_02",(1<<0), 16, 0x1000, 0x1001, 0x1002, 0x1003, 0x2000,	0x2001,	0x2002,	0x2003, 0x4000, 0x4001, 0x4002, 0x4003, 0x8000, 0x8001, 0x8002, 0x8003);
    compFm("OPCODE_JNC_02",(1<<1), 16, 0x1000, 0x1001, 0x1002, 0x1003, 0x2000,	0x2001,	0x2002,	0x2003, 0x4000, 0x4001, 0x4002, 0x4003, 0x8000, 0x8001, 0x8002, 0x8003); 

    SET_JNCFLAG(0x800);
    MEAS_MPAT("OPCODE_JNC_02");
    compFm("OPCODE_JNC_02",(1<<0), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x2000,	0x2001,	0x2002,	0x2003, 0x4000, 0x4001, 0x4002, 0x4003, 0x8000, 0x8001, 0x8002, 0x8003);
    compFm("OPCODE_JNC_02",(1<<1), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x2000,	0x2001,	0x2002,	0x2003, 0x4000, 0x4001, 0x4002, 0x4003, 0x8000, 0x8001, 0x8002, 0x8003); 

    SET_JNCFLAG(0x1000);
    MEAS_MPAT("OPCODE_JNC_02");
    compFm("OPCODE_JNC_02",(1<<0), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x1000, 0x1001, 0x1002, 0x1003, 0x4000, 0x4001, 0x4002, 0x4003, 0x8000, 0x8001, 0x8002, 0x8003);
    compFm("OPCODE_JNC_02",(1<<1), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x1000, 0x1001, 0x1002, 0x1003, 0x4000, 0x4001, 0x4002, 0x4003, 0x8000, 0x8001, 0x8002, 0x8003); 

    SET_JNCFLAG(0x2000);
    MEAS_MPAT("OPCODE_JNC_02");
    compFm("OPCODE_JNC_02",(1<<0), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x1000, 0x1001, 0x1002, 0x1003, 0x2000, 0x2001, 0x2002, 0x2003, 0x8000, 0x8001, 0x8002, 0x8003);
    compFm("OPCODE_JNC_02",(1<<1), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x1000, 0x1001, 0x1002, 0x1003, 0x2000, 0x2001, 0x2002, 0x2003, 0x8000, 0x8001, 0x8002, 0x8003); 

    SET_JNCFLAG(0x4000);
    MEAS_MPAT("OPCODE_JNC_02");
    compFm("OPCODE_JNC_02",(1<<0), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x1000, 0x1001, 0x1002, 0x1003, 0x2000, 0x2001, 0x2002, 0x2003, 0x4000, 0x4001, 0x4002, 0x4003);
    compFm("OPCODE_JNC_02",(1<<1), 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x1000, 0x1001, 0x1002, 0x1003, 0x2000, 0x2001, 0x2002, 0x2003, 0x4000, 0x4001, 0x4002, 0x4003); 

    SET_JNCFLAG(0x8000);
    MEAS_MPAT("OPCODE_JNC_03");
     compFm("OPCODE_JNC_03",(1<<0), 16, 0x1000, 0x1001, 0x1002, 0x1003, 0x2000, 0x2001, 0x2002, 0x2003, 0x4000, 0x4001, 0x4002, 0x4003, 0x8000, 0x8001, 0x8002, 0x8003);
    compFm("OPCODE_JNC_03",(1<<1), 16, 0x1000, 0x1001, 0x1002, 0x1003, 0x2000, 0x2001, 0x2002, 0x2003, 0x4000, 0x4001, 0x4002, 0x4003, 0x8000, 0x8001, 0x8002, 0x8003); 
 
    SET_TIMER(JET_TIMER,3.2 US);
  	MEAS_MPAT("OPCODE_JET_00");
    compFm("OPCODE_JET_00",(1<<0), 16, 0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006,	0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x000f,	0x0010);
    compFm("OPCODE_JET_00",(1<<1), 16, 0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006,	0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x000f,	0x0010);

    MEAS_MPAT("OPCODE_OUT_00");
    //compFm("OPCODE_OUT_00",(1<<0), 16, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x000f);
    //compFm("OPCODE_OUT_00",(1<<1), 16, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x000f);

    SET_REG(REF_MODE,1);
    SET_TIMER(REF_TIMER, 10 US);
    MEAS_MPAT("OPCODE_REF_00");
    compFm("OPCODE_REF_00",(1<<0), 16, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x1111);
    compFm("OPCODE_REF_00",(1<<1), 16, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x2222);

    SET_TIMER(REF_TIMER,11 US);
    MEAS_MPAT("OPCODE_REF_00");
    compFm("OPCODE_REF_01",(1<<0), 16, 0x1111, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e);
    compFm("OPCODE_REF_01",(1<<1), 16, 0x2222, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e);

    SET_REG(REF_MODE,0);
    SET_TIMER(REF_TIMER,11 US);
    MEAS_MPAT("OPCODE_REF_01");
    compFm("OPCODE_REF_02",(1<<0), 16, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,0x0000);
    compFm("OPCODE_REF_02",(1<<1), 16, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,0x0000);

    MEAS_MPAT("OPCODE_JZD_00");
    compFm("OPCODE_JZD_00",(1<<0), 16, 0x1111,	0xeeee,	0xdddd,	0x2222,	0x2222,	0x3333,	0xcccc,	0xcccc,	0x3333,	0x4444,	0xbbbb,	0xbbbb,	0x5555,	0x5555,	0xaaaa,	0xaaaa);
    compFm("OPCODE_JZD_00",(1<<1), 16, 0x1111,	0xeeee,	0xdddd,	0x2222,	0x2222,	0x3333,	0xcccc,	0xcccc,	0x3333,	0x4444,	0xbbbb,	0xbbbb,	0x5555,	0x5555,	0xaaaa,	0xaaaa);

    MEAS_MPAT("OPCODE_RPT_00");
    compFm("OPCODE_RPT_00",(1<<0), 16, 0x1111,	0x1111,	0x1111,	0x1111,	0x1111,	0x2222,	0x2222,	0x2222,	0x2222,	0x2222,	0x2222,	0x3333,	0x3333,	0x3333,	0x3333,	0x3333);
    compFm("OPCODE_RPT_00",(1<<1), 16, 0xaaaa,	0xaaaa,	0xaaaa,	0xaaaa,	0xaaaa,	0xbbbb,	0xbbbb,	0xbbbb,	0xbbbb,	0xbbbb,	0xbbbb,	0xcccc,	0xcccc,	0xcccc,	0xcccc,	0xcccc);

    MEAS_MPAT("OPCODE_STI_00");
    compFm("OPCODE_STI_00",(1<<0), 16, 0x1000,	0x1000,	0x1000,	0x2000,	0x2000,	0x2000,	0x2000,	0x3000, 0x3000,	0x3000,	0x3000,	0x3000,	0x4000,	0x4000,	0x4000,	0x4000);
    compFm("OPCODE_STI_00",(1<<1), 16, 0x5000,	0x5000,	0x5000,	0x6000,	0x6000,	0x6000,	0x6000,	0x7000, 0x7000,	0x7000,	0x7000,	0x7000,	0x8000,	0x8000,	0x8000,	0x8000);   

    MEAS_MPAT("OPCODE_STI_01");
    compFm("OPCODE_STI_01",(1<<0), 16, 0x1000,	0x1000,	0x1000,	0x2000,	0x2000,	0x2000,	0x2000,	0x3000, 0x3000,	0x3000,	0x3000,	0x3000,	0x4000,	0x4000,	0x4000,	0x4000);
    compFm("OPCODE_STI_01",(1<<1), 16, 0x5000,	0x5000,	0x5000,	0x6000,	0x6000,	0x6000,	0x6000,	0x7000, 0x7000,	0x7000,	0x7000,	0x7000,	0x8000,	0x8000,	0x8000,	0x8000);   

    MEAS_MPAT("OPCODE_STI_02");
    compFm("OPCODE_STI_02",(1<<0), 16, 0x1000,	0x1000,	0x1000,	0x2000,	0x2000,	0x2000,	0x2000,	0x3000, 0x3000,	0x3000,	0x3000,	0x3000,	0x4000,	0x4000,	0x4000,	0x4000);
    compFm("OPCODE_STI_02",(1<<1), 16, 0x5000,	0x5000,	0x5000,	0x6000,	0x6000,	0x6000,	0x6000,	0x7000, 0x7000,	0x7000,	0x7000,	0x7000,	0x8000,	0x8000,	0x8000,	0x8000);   

    MEAS_MPAT("OPCODE_STI_02");
    compFm("OPCODE_STI_03",(1<<0), 16, 0x1000,	0x1000,	0x1000,	0x2000,	0x2000,	0x2000,	0x2000,	0x3000, 0x3000,	0x3000,	0x3000,	0x3000,	0x4000,	0x4000,	0x4000,	0x4000);
    compFm("OPCODE_STI_03",(1<<1), 16, 0x5000,	0x5000,	0x5000,	0x6000,	0x6000,	0x6000,	0x6000,	0x7000, 0x7000,	0x7000,	0x7000,	0x7000,	0x8000,	0x8000,	0x8000,	0x8000);   

    SET_REG(IDX0,2);           
    SET_REG(IDX1,1);
    MEAS_MPAT("OPCODE_LDI_00");
    compFm("OPCODE_LDI_00",(1<<0), 16, 0x1000,	0x1000,	0x1000,	0x1000,	0x2000,	0x2000,	0x2000,	0x4000,	0x4000,	0x4000,	0x4000,	0x4000,	0x4000,	0x5000,	0x5000,	0x5000);
    compFm("OPCODE_LDI_00",(1<<1), 16, 0xa000,	0xa000,	0xa000,	0xa000,	0xb000,	0xb000,	0xb000,	0xc000,	0xc000,	0xc000,	0xc000,	0xc000,	0xc000,	0xd000,	0xd000,	0xd000);
  
    SET_REG(IDX2,2);           
    SET_REG(IDX3,1);
    MEAS_MPAT("OPCODE_LDI_01");
    compFm("OPCODE_LDI_01",(1<<0), 16, 0x1000,	0x1000,	0x1000,	0x1000,	0x2000,	0x2000,	0x2000,	0x4000,	0x4000,	0x4000,	0x4000,	0x4000,	0x4000,	0x5000,	0x5000,	0x5000);
    compFm("OPCODE_LDI_01",(1<<1), 16, 0xa000,	0xa000,	0xa000,	0xa000,	0xb000,	0xb000,	0xb000,	0xc000,	0xc000,	0xc000,	0xc000,	0xc000,	0xc000,	0xd000,	0xd000,	0xd000);

    SET_REG(IDX4,2);            
    SET_REG(IDX5,1);
    MEAS_MPAT("OPCODE_LDI_02");
    compFm("OPCODE_LDI_02",(1<<0), 16, 0x1000,	0x1000,	0x1000,	0x1000,	0x2000,	0x2000,	0x2000,	0x4000,	0x4000,	0x4000,	0x4000,	0x4000,	0x4000,	0x5000,	0x5000,	0x5000);
    compFm("OPCODE_LDI_02",(1<<1), 16, 0xa000,	0xa000,	0xa000,	0xa000,	0xb000,	0xb000,	0xb000,	0xc000,	0xc000,	0xc000,	0xc000,	0xc000,	0xc000,	0xd000,	0xd000,	0xd000);

    SET_REG(IDX6,2);             
    SET_REG(IDX7,1);
    MEAS_MPAT("OPCODE_LDI_03");
    compFm("OPCODE_LDI_03",(1<<0), 16, 0x1000,	0x1000,	0x1000,	0x1000,	0x2000,	0x2000,	0x2000,	0x4000,	0x4000,	0x4000,	0x4000,	0x4000,	0x4000,	0x5000,	0x5000,	0x5000);
    compFm("OPCODE_LDI_03",(1<<1), 16, 0xa000,	0xa000,	0xa000,	0xa000,	0xb000,	0xb000,	0xb000,	0xc000,	0xc000,	0xc000,	0xc000,	0xc000,	0xc000,	0xd000,	0xd000,	0xd000);

    SET_REG(IDX0,1);           
    SET_REG(IDX1,2);
    SET_REG(IDX2,3);           
    SET_REG(IDX3,2);
    MEAS_MPAT("OPCODE_JNI_00");
    compFm("OPCODE_JNI_00",(1<<0), 16, 0x1111,	0x1111,	0x1111,	0x2222,	0x2222,	0x2222,	0x2222,	0x3333,	0x3333,	0x3333,	0x3333,	0x3333,	0x4444,	0x4444,	0x4444,	0x4444);
    compFm("OPCODE_JNI_00",(1<<1), 16, 0xaaaa,	0xaaaa,	0xaaaa,	0xbbbb,	0xbbbb,	0xbbbb,	0xbbbb,	0xcccc,	0xcccc,	0xcccc,	0xcccc,	0xcccc,	0xdddd,	0xdddd,	0xdddd,	0xdddd);

    SET_REG(IDX4,2);            
    SET_REG(IDX5,3);
    SET_REG(IDX6,1);             
    SET_REG(IDX7,2);
    MEAS_MPAT("OPCODE_JNI_01");
    compFm("OPCODE_JNI_01",(1<<0), 16, 0x1111,	0x1111,	0x1111,	0x1111,	0x2222,	0x2222,	0x2222,	0x2222,	0x2222,	0x3333,	0x3333,	0x3333,	0x4444,	0x4444,	0x4444,	0x4444);
    compFm("OPCODE_JNI_01",(1<<1), 16, 0xaaaa,	0xaaaa,	0xaaaa,	0xaaaa,	0xbbbb,	0xbbbb,	0xbbbb,	0xbbbb,	0xbbbb,	0xcccc,	0xcccc,	0xcccc,	0xdddd,	0xdddd,	0xdddd,	0xdddd);   
    MEAS_MPAT("OPCODE_PAS_00");
    compFm("OPCODE_PAS_00",(1<<0), 16, 0x0000,	0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006,	0x0007,	0x0008,	0x0009,	0x000A,	0x000B,	0x000C,	0x000D,	0x000E,	0x000F);
    compFm("OPCODE_PAS_00",(1<<1), 16, 0x8000,	0x8001,	0x8002,	0x8003,	0x8004,	0x8005,	0x8006,	0x8007,	0x8008,	0x8009,	0x800A,	0x800B,	0x800C,	0x800D,	0x800E,	0x800F);
	
    MEAS_MPAT("OPCODE_PAS_01");
    compFm("OPCODE_PAS_01",(1<<0), 16, 0x0000,	0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006,	0x0007,	0x0008,	0x0009,	0x000A,	0x000B,	0x000C,	0x000D,	0x000E,	0x000F);
    compFm("OPCODE_PAS_01",(1<<1), 16, 0x8000,	0x8001,	0x8002,	0x8003,	0x8004,	0x8005,	0x8006,	0x8007,	0x8008,	0x8009,	0x800A,	0x800B,	0x800C,	0x800D,	0x800E,	0x800F);


	writePG(1<<9 | 7,0x1,1);	//rb_pin enable
    MEAS_MPAT("OPCODE_JRB_00");
    compFm("OPCODE_JRB_00",(1<<0), 16, 0x002a,	0x002a,	0x002a,	0x002a,	0x002a,	0x002a,	0x002a,	0x002a, 0x002a,	0x002a,	0x002a,	0x002a,	0x002a,	0x002a,	0x002a,	0x002a);
    compFm("OPCODE_JRB_00",(1<<1), 16, 0x002a,	0x002a,	0x002a,	0x002a,	0x002a,	0x002a,	0x002a,	0x002a, 0x002a,	0x002a,	0x002a,	0x002a,	0x002a,	0x002a,	0x002a,	0x002a);   
	writePG(1<<9 | 7,0x0,1);	//rb_pin enable

    COMPARE_MODE(true);
    STRB_EDGE(1, 160, 205);

    IO_MASK(0x00000000, 0x00000000);
    MEAS_MPAT("IO_MASK");
    compFm("IO_MASK_00",(1<<0), 16, 0x1111, 0x1111, 0x3333, 0x3333, 0x5555, 0x5555, 0x7777, 0x7777, 0x9999, 0x9999, 0xbbbb, 0xbbbb, 0xdddd, 0xdddd, 0xffff, 0xffff);
    compFm("IO_MASK_00",(1<<1), 16, 0x2222, 0x2222, 0x4444, 0x4444, 0x6666, 0x6666, 0x8888, 0x8888, 0xaaaa, 0xaaaa, 0xcccc, 0xcccc, 0xeeee, 0xeeee, 0xffff, 0xffff);
   
    IO_MASK(0x55555555, 0x55555555);
    MEAS_MPAT("IO_MASK");
    compFm("IO_MASK_55",(1<<0), 16, 0x0000, 0x0000, 0x2222, 0x2222, 0x0000, 0x0000, 0x2222, 0x2222, 0x8888, 0x8888, 0xaaaa, 0xaaaa, 0x8888, 0x8888, 0xaaaa, 0xaaaa);
    compFm("IO_MASK_55",(1<<1), 16, 0x2222, 0x2222, 0x0000, 0x0000, 0x2222, 0x2222, 0x8888, 0x8888, 0xaaaa, 0xaaaa, 0x8888, 0x8888, 0xaaaa, 0xaaaa, 0xaaaa, 0xaaaa);
   
    IO_MASK(0xaaaaaaaa, 0xaaaaaaaa);
    MEAS_MPAT("IO_MASK");-
    compFm("IO_MASK_aa",(1<<0), 16, 0x1111, 0x1111, 0x1111, 0x1111, 0x5555, 0x5555, 0x5555, 0x5555, 0x1111, 0x1111, 0x1111, 0x1111, 0x5555, 0x5555, 0x5555, 0x5555);
    compFm("IO_MASK_aa",(1<<1), 16, 0x0000, 0x0000, 0x4444, 0x4444, 0x4444, 0x4444, 0x0000, 0x0000, 0x0000, 0x0000, 0x4444, 0x4444, 0x4444, 0x4444, 0x5555, 0x5555);   
   
    IO_MASK(0xffffffff, 0xffffffff);
    MEAS_MPAT("IO_MASK");
    compFm("IO_MASK_ff",(1<<0), 16, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
    compFm("IO_MASK_ff",(1<<1), 16, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
   
    DATA_MODE();
    IO_MASK(0x0000, 0x0000);
    STRB_EDGE(1, 60, 105);
    
}

void testPG_alu_xbyb()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;
	
	load_pds("diag_pds_data_xy16");
    SEND_MPAT("diag_xbyb.mpa");

    MEAS_MPAT("XB_ALU_XB_00");
    compFm("XB_ALU_XB_00"      ,(1<<0), 16, 0x0000, 0xffff, 0x0000, 0x0001, 0x0000, 0x0001, 0x0002, 0x0004, 0x0002, 0x0001, 0x8000, 0x7FFF, 0x8000, 0x7FFF, 0x8000, 0x7fff);
    compFm("XB_ALU_XB_00"      ,(1<<1), 16, 0x0000, 0xffff, 0x0000, 0x0001, 0x0000, 0x0001, 0x0002, 0x0004, 0x0002, 0x0001, 0xffff, 0xfffe, 0xffff, 0xfffe, 0xffff, 0xfffe);	

    MEAS_MPAT("XB_ALU_A_00");
    compFm("XB_ALU_A_00"       ,(1<<0), 16, 0x0001, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0xffff, 0x1111, 0x0000, 0x0001, 0x0000, 0x0001);
    compFm("XB_ALU_A_00"       ,(1<<1), 16, 0x0007, 0x0008, 0xffff, 0x1111, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0xffff, 0x1111, 0x0008, 0xffff, 0x0008, 0xffff);

    MEAS_MPAT("XB_ALU_plus_A_00");  
    compFm("XB_ALU_plus_A_00"  ,(1<<0), 16, 0x1000, 0x1001, 0x1003, 0x1007, 0x100f, 0x101f, 0x103f, 0x107f, 0x117f, 0x137f, 0x177f, 0x1f7f, 0x0000, 0xffff, 0xffff, 0x0000);
    compFm("XB_ALU_plus_A_00"  ,(1<<1), 16, 0x0100, 0x0300, 0x0700, 0x0f00, 0x8f00, 0x8f01, 0x8f03, 0x8f07, 0x8f0f, 0x8f1f, 0x8f3f, 0x8f7f, 0xffff, 0x0000, 0xffff, 0x0000);

    MEAS_MPAT("XB_ALU_minus_A_00");  
    compFm("XB_ALU_minus_A_00" ,(1<<0), 16, 0x7fff, 0x7ffe, 0x7fee, 0x7eee, 0x6eee, 0x6eee, 0x6eed, 0x6edd, 0x6ddd, 0x5ddd, 0x5ddd, 0x5ddc, 0x0000, 0xffff, 0xffff, 0x0000);
    compFm("XB_ALU_minus_A_00" ,(1<<1), 16, 0x4000, 0x3fff, 0x3fef, 0x3eef, 0x2eef, 0x2eef, 0x2eee, 0x2ede, 0x2dde, 0x1dde, 0x1dde, 0x1ddd, 0xffff, 0x0000, 0xffff, 0x0000);
  
    MEAS_MPAT("XB_ALU_XB_plus_Imm");  
    compFm("XB_ALU_XB_plus_Imm",(1<<0), 16, 0x8001,	0x8003,	0x8006,	0x800a,	0x800f,	0x8015,	0x801c,	0x8024,	0x802d,	0x8037, 0x8042, 0x804e, 0x805b, 0x8069, 0x8078, 0x8088);  
    compFm("XB_ALU_XB_plus_Imm",(1<<1), 16, 0x4001,	0x4003,	0x4006,	0x400a,	0x400f,	0x4015,	0x401c,	0x4024,	0x402d,	0x4037, 0x4042, 0x404e, 0x405b, 0x4069, 0x4078, 0x4088);
 
    MEAS_MPAT("XB_ALU_XOS_00");
    compFm("XB_ALU_XOS_00"     ,(1<<0), 16, 0x0080,	0x0081,	0x0082,	0x0083,	0x0084,	0x0083,	0x0082,	0x0081,	0x0080,	0x007f, 0x0080, 0x007f, 0x007e, 0x007d, 0x007c, 0x007b);
    compFm("XB_ALU_YOS_00"     ,(1<<1), 16, 0x0040,	0x0041,	0x0042,	0x0043,	0x0044,	0x0043,	0x0042,	0x0041,	0x0040,	0x003f, 0x0040, 0x003f, 0x003e, 0x003d, 0x003c, 0x003b);

    MEAS_MPAT("XB_ALU_Imm_00");
    compFm("XB_ALU_Imm_00"     ,(1<<0), 16, 0x0000,	0x0001,	0x0003,	0x0007,	0x000f,	0x000e,	0x000c,	0x0008,	0x0000,	0xffff,	0x0000,	0xaaaa,	0x0000,	0xbbbb,	0x0000,	0x0000);
    compFm("XB_ALU_Imm_00"     ,(1<<1), 16, 0x0000,	0x0001,	0x0003,	0x0007,	0x000f,	0x000e,	0x000c,	0x0008,	0x0000,	0xffff,	0x0000,	0xbbbb,	0x0000,	0xcccc,	0x0000,	0x0000);
}

void testPG_alu_xcyc()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;
	
	load_pds("diag_pds_data_xy16");
    SEND_MPAT("diag_xcyc.mpa");

    MEAS_MPAT("XC_ALU_A_00");
    compFm("XC_ALU_A_00",(1<<0), 16, 0x0000, 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000, 0x8000, 0x9000, 0xA000, 0xB000, 0xC000, 0xD000, 0xE000, 0xF000);
    compFm("XC_ALU_A_00",(1<<1), 16, 0x8000, 0x9000, 0xA000, 0xB000, 0xC000, 0xD000, 0xE000, 0xF000, 0x0000, 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000);

    MEAS_MPAT("XC_ALU_NA_00");
    compFm("XC_ALU_NA_00",(1<<0), 16, 0xffff, 0xfffe, 0xfffd, 0xfffb, 0xfff7, 0xffef, 0xffdf, 0xffbf, 0xffff, 0xfff5, 0xfff4, 0xfff3, 0xfff2, 0xff5f, 0xff4f, 0xff3f);
    compFm("XC_ALU_NA_00",(1<<1), 16, 0xffff, 0xfff5, 0xfff4, 0xfff3, 0xfff2, 0xff5f, 0xff4f, 0xff3f, 0xffff, 0xfffe, 0xfffd, 0xfffb, 0xfff7, 0xffef, 0xffdf, 0xffbf);
 
    MEAS_MPAT("XC_ALU_AP1_00");
    compFm("XC_ALU_AP1_00",(1<<0), 16, 0x0001,	0x0002,	0x0003,	0x0005,	0x0009,	0x0011,	0x0021,	0x0041, 0x0001,	0x000b,	0x000c,	0x000d,	0x000e,	0x00a1,	0x00b1,	0x00c1);
    compFm("XC_ALU_AP1_00",(1<<1), 16, 0x0001,	0x000b,	0x000c,	0x000d,	0x000e,	0x00a1,	0x00b1,	0x00c1, 0x0001,	0x0002,	0x0003,	0x0005,	0x0009,	0x0011,	0x0021,	0x0041);
    
    MEAS_MPAT("XC_ALU_AM1_00");
    compFm("XC_ALU_AM1_00",(1<<0), 16, 0xffff,	0x0000,	0x0001,	0x0003,	0x0007,	0x000f,	0x001f,	0x003f, 0xffff,	0x0009,	0x000a,	0x000b,	0x000c,	0x009f,	0x00af,	0x00bf);
    compFm("XC_ALU_AM1_00",(1<<1), 16, 0xffff,	0x0009,	0x000a,	0x000b,	0x000c,	0x009f,	0x00af,	0x00bf, 0xffff,	0x0000,	0x0001,	0x0003,	0x0007,	0x000f,	0x001f,	0x003f);

    MEAS_MPAT("XC_ALU_AM2_00");
    compFm("XC_ALU_AM2_00",(1<<0), 16, 0x0000,	0x0002,	0x0004,	0x0008,	0x0010,	0x0020,	0x0040,	0x0080, 0x0000,	0x0014,	0x0016,	0x0018,	0x001a,	0x0140,	0x0160,	0x0180);
    compFm("XC_ALU_AM2_00",(1<<1), 16, 0x0000,	0x0014,	0x0016,	0x0018,	0x001a,	0x0140,	0x0160,	0x0180, 0x0000,	0x0002,	0x0004,	0x0008,	0x0010,	0x0020,	0x0040,	0x0080);

    MEAS_MPAT("XC_ALU_AD2_00");
    compFm("XC_ALU_AD2_00",(1<<0), 16, 0x0000,	0x0000,	0x0001,	0x0002,	0x0004,	0x0008,	0x0010,	0x0020, 0x0000,	0x0005,	0x0005,	0x0006,	0x0006,	0x0050,	0x0058,	0x0060);
    compFm("XC_ALU_AD2_00",(1<<1), 16, 0x0000,	0x0005,	0x0005,	0x0006,	0x0006,	0x0050,	0x0058,	0x0060, 0x0000,	0x0000,	0x0001,	0x0002,	0x0004,	0x0008,	0x0010,	0x0020);
    
    MEAS_MPAT("XC_ALU_B_00");   
    compFm("XC_ALU_B_00",(1<<0), 16, 0x0000,	0x0001,	0x0002,	0x0004,	0x0008,	0x0010,	0x0020,	0x0040,	0x0080,	0x0100,	0x0200,	0x0400,	0x0800,	0x1000,	0xffff, 0x0000);
    compFm("XC_ALU_B_00",(1<<1), 16, 0x0080,	0x0100,	0x0200,	0x0400,	0x0000,	0x0001,	0x0002,	0x0004,	0x0008,	0x0010,	0x0020,	0x0040,	0x1000,	0x0800,	0xffff, 0x0080);
    
    MEAS_MPAT("XC_ALU_APB_00");
    compFm("XC_ALU_APB_00",(1<<0), 16, 0x0001,	0x0011,	0x0111,	0x0112,	0x0122,	0x0222,	0x0223,	0x0233,	0x0333,	0x0334,	0x0344,	0x0444,	0x0454,	0x0455, 0x0456, 0x0466);
    compFm("XC_ALU_APB_00",(1<<1), 16, 0x8100,	0x8101,	0x8111,	0x8211,	0x8212,	0x8222,	0x8322,	0x8323,	0x8333,	0x8433,	0x8434,	0x8444,	0x8445,	0x8455, 0x8555, 0x8556);
    
    MEAS_MPAT("XC_ALU_APB_01");
    compFm("XC_ALU_APB_01",(1<<0),16, 0x1001,	0x1010,	0x1100,	0x1001,	0x1010,	0x1100,	0x1001,	0x1010,	0x1100,	0x1001,	0x1010,	0x1100,	0x1010,	0x1001, 0x1001,	0x1010);
    compFm("XC_ALU_APB_01",(1<<1),16, 0x9100,	0x9001,	0x9010,	0x9100,	0x9001,	0x9010,	0x9100,	0x9001,	0x9010,	0x9100,	0x9001,	0x9010,	0x9001,	0x9010, 0x9100,	0x9001);
    
    MEAS_MPAT("XC_ALU_APB_02");
    compFm("XC_ALU_APB_02",(1<<0),16,0x2001,	0x2010,	0x2100,	0x2001,	0x2010,	0x2100,	0x2001,	0x2010,	0x2100,	0x2001,	0x2010,	0x2100,	0x2010,	0x2001, 0x2001,	0x2010);
    compFm("XC_ALU_APB_02",(1<<1),16,0xa100,	0xa001,	0xa010,	0xa100,	0xa001,	0xa010,	0xa100,	0xa001,	0xa010,	0xa100,	0xa001,	0xa010,	0xa001,	0xa010, 0xa100,	0xa001);
    
    MEAS_MPAT("XC_ALU_APB_03");
    compFm("XC_ALU_APB_03",(1<<0),16,0x3001,	0x3010,	0x3100,	0x3001,	0x3010,	0x3100,	0x3001,	0x3010,	0x3100,	0x3001,	0x3010,	0x3100,	0x3010,	0x3001, 0x3001,	0x3010);
    compFm("XC_ALU_APB_03",(1<<1),16,0xb100,	0xb001,	0xb010,	0xb100,	0xb001,	0xb010,	0xb100,	0xb001,	0xb010,	0xb100,	0xb001,	0xb010,	0xb001,	0xb010, 0xb100,	0xb001);
    
    MEAS_MPAT("XC_ALU_APB_04");
    compFm("XC_ALU_APB_04",(1<<0),16,0x4001,	0x4010,	0x4100,	0x4001,	0x4010,	0x4100,	0x4001,	0x4010,	0x4100,	0x4001,	0x4010,	0x4100,	0x4010,	0x4001, 0x4001,	0x4010);
    compFm("XC_ALU_APB_04",(1<<1),16,0xc100,	0xc001,	0xc010,	0xc100,	0xc001,	0xc010,	0xc100,	0xc001,	0xc010,	0xc100,	0xc001,	0xc010,	0xc001,	0xc010, 0xc100,	0xc001);
    
    MEAS_MPAT("XC_ALU_APB_05");
    compFm("XC_ALU_APB_05",(1<<0),16,0x5001,	0x5010,	0x5100,	0x5001,	0x5010,	0x5100,	0x5001,	0x5010,	0x5100,	0x5001,	0x5010,	0x5100,	0x5010,	0x5001, 0x5001,	0x5010);
    compFm("XC_ALU_APB_05",(1<<1),16,0xd100,	0xd001,	0xd010,	0xd100,	0xd001,	0xd010,	0xd100,	0xd001,	0xd010,	0xd100,	0xd001,	0xd010,	0xd001,	0xd010, 0xd100,	0xd001);
    
    MEAS_MPAT("XC_ALU_APB_06");
    compFm("XC_ALU_APB_06",(1<<0),16,0x6001,	0x6010,	0x6100,	0x6001,	0x6010,	0x6100,	0x6001,	0x6010,	0x6100,	0x6001,	0x6010,	0x6100,	0x6010,	0x6001, 0x6001,	0x6010);
    compFm("XC_ALU_APB_06",(1<<1),16,0xe100,	0xe001,	0xe010,	0xe100,	0xe001,	0xe010,	0xe100,	0xe001,	0xe010,	0xe100,	0xe001,	0xe010,	0xe001,	0xe010, 0xe100,	0xe001);
	
    MEAS_MPAT("XC_ALU_APB_07");
    compFm("XC_ALU_APB_07",(1<<0),16,0x7001,	0x7010,	0x7100,	0x7001,	0x7010,	0x7100,	0x7001,	0x7010,	0x7100,	0x7001,	0x7010,	0x7100,	0x7010,	0x7001, 0x7001,	0x7010);
    compFm("XC_ALU_APB_07",(1<<1),16,0xf100,	0xf001,	0xf010,	0xf100,	0xf001,	0xf010,	0xf100,	0xf001,	0xf010,	0xf100,	0xf001,	0xf010,	0xf001,	0xf010, 0xf100,	0xf001); 

    MEAS_MPAT("XC_ALU_AMB_00");
    compFm("XC_ALU_AMB_00",(1<<0), 16,0x0443,	0x0433,	0x0333,	0x0332,	0x0322,	0x0222,	0x0221,	0x0211,	0x0111,	0x0110,	0x0100,	0x0000,	0xfff0,	0xffef, 0xffee, 0xffde);
    compFm("XC_ALU_AMB_00",(1<<1), 16,0x0554,	0x0544,	0x0444,	0x0443,	0x0433,	0x0333,	0x0332,	0x0322,	0x0222,	0x0221,	0x0211,	0x0111,	0x0101,	0x0100, 0x00ff, 0x00ef);

    MEAS_MPAT("XC_ALU_AMB_01");
    compFm("XC_ALU_AMB_01",(1<<0), 16,0x0554,	0x0545,	0x0455,	0x0554,	0x0545,	0x0455,	0x0554,	0x0545,	0x0455,	0x0554,	0x0545,	0x0455,	0x0545,	0x0554, 0x0554,	0x0545);
    compFm("XC_ALU_AMB_01",(1<<1), 16,0x0665,	0x0656,	0x0566,	0x0665,	0x0656,	0x0566,	0x0665,	0x0656,	0x0566,	0x0665,	0x0656,	0x0566,	0x0656,	0x0665, 0x0665,	0x0656);

    MEAS_MPAT("XC_ALU_AMB_02");
    compFm("XC_ALU_AMB_02",(1<<0), 16,0x0665,	0x0656,	0x0566,	0x0665,	0x0656,	0x0566,	0x0665,	0x0656,	0x0566,	0x0665,	0x0656,	0x0566,	0x0656,	0x0665, 0x0665,	0x0656);
    compFm("XC_ALU_AMB_02",(1<<1), 16,0x0776,	0x0767,	0x0677,	0x0776,	0x0767,	0x0677,	0x0776,	0x0767,	0x0677,	0x0776,	0x0767,	0x0677,	0x0767,	0x0776, 0x0776,	0x0767);

    MEAS_MPAT("XC_ALU_AMB_03");
    compFm("XC_ALU_AMB_03",(1<<0), 16,0x0776,	0x0767,	0x0677,	0x0776,	0x0767,	0x0677,	0x0776,	0x0767,	0x0677,	0x0776,	0x0767,	0x0677,	0x0767,	0x0776, 0x0776,	0x0767);
    compFm("XC_ALU_AMB_03",(1<<1), 16,0x0887,	0x0878,	0x0788,	0x0887,	0x0878,	0x0788,	0x0887,	0x0878,	0x0788,	0x0887,	0x0878,	0x0788,	0x0878,	0x0887, 0x0887,	0x0878);

    MEAS_MPAT("XC_ALU_AMB_04");
    compFm("XC_ALU_AMB_04",(1<<0), 16,0x0887,	0x0878,	0x0788,	0x0887,	0x0878,	0x0788,	0x0887,	0x0878,	0x0788,	0x0887,	0x0878,	0x0788,	0x0878,	0x0887, 0x0887,	0x0878);
    compFm("XC_ALU_AMB_04",(1<<1), 16,0x0998,	0x0989,	0x0899,	0x0998,	0x0989,	0x0899,	0x0998,	0x0989,	0x0899,	0x0998,	0x0989,	0x0899,	0x0989,	0x0998, 0x0998,	0x0989);

    MEAS_MPAT("XC_ALU_AMB_05");
    compFm("XC_ALU_AMB_05",(1<<0), 16,0x0998,	0x0989,	0x0899,	0x0998,	0x0989,	0x0899,	0x0998,	0x0989,	0x0899,	0x0998,	0x0989,	0x0899,	0x0989,	0x0998, 0x0998,	0x0989);
    compFm("XC_ALU_AMB_05",(1<<1), 16,0x0aa9,	0x0a9a,	0x09aa,	0x0aa9,	0x0a9a,	0x09aa,	0x0aa9,	0x0a9a,	0x09aa,	0x0aa9,	0x0a9a,	0x09aa,	0x0a9a,	0x0aa9, 0x0aa9,	0x0a9a);

    MEAS_MPAT("XC_ALU_AMB_06");
    compFm("XC_ALU_AMB_06",(1<<0), 16,0x0aa9,	0x0a9a,	0x09aa,	0x0aa9,	0x0a9a,	0x09aa,	0x0aa9,	0x0a9a,	0x09aa,	0x0aa9,	0x0a9a,	0x09aa,	0x0a9a,	0x0aa9, 0x0aa9,	0x0a9a);
    compFm("XC_ALU_AMB_06",(1<<1), 16,0x0bba,	0x0bab,	0x0abb,	0x0bba,	0x0bab,	0x0abb,	0x0bba,	0x0bab,	0x0abb,	0x0bba,	0x0bab,	0x0abb,	0x0bab,	0x0bba, 0x0bba,	0x0bab);

    MEAS_MPAT("XC_ALU_AMB_07");
    compFm("XC_ALU_AMB_07",(1<<0), 16,0x0bba,	0x0bab,	0x0abb,	0x0bba,	0x0bab,	0x0abb,	0x0bba,	0x0bab,	0x0abb,	0x0bba,	0x0bab,	0x0abb,	0x0bab,	0x0bba, 0x0bba,	0x0bab);
    compFm("XC_ALU_AMB_07",(1<<1), 16,0x0ccb,	0x0cbc,	0x0bcc,	0x0ccb,	0x0cbc,	0x0bcc,	0x0ccb,	0x0cbc,	0x0bcc,	0x0ccb,	0x0cbc,	0x0bcc,	0x0cbc,	0x0ccb, 0x0ccb,	0x0cbc);
    
    MEAS_MPAT("XC_ALU_AANDB_00");
    compFm("XC_ALU_AAND_00",(1<<0), 16,0xfffe,	0xfffc,	0xfff8,	0xfff0,	0xffe0,	0xffc0,	0xff80,	0xff00,	0xfe00,	0xfc00,	0xf800,	0xf000,	0xe000,	0xc000, 0xc000, 0xc000);
    compFm("XC_ALU_AAND_00",(1<<1), 16,0xeffe,	0xeffc,	0xeff8,	0xeff0,	0xefe0,	0xefc0,	0xef80,	0xef00,	0xee00,	0xec00,	0xe800,	0xe000,	0xe000,	0xc000, 0xc000, 0xc000);

    MEAS_MPAT("XC_ALU_AANDB_01");
    compFm("XC_ALU_AAND_01",(1<<0), 16,0xfffe,	0xfffd,	0xfffb,	0xfff7,	0xffef,	0xffdf,	0xffbf,	0xff7f,	0xfeff,	0xfdff,	0xfbff,	0xf7ff,	0xefff,	0xdfff, 0xfffe,	0xfffd);
    compFm("XC_ALU_AAND_01",(1<<1), 16,0xeffe,	0xeffd,	0xeffb,	0xeff7,	0xefef,	0xefdf,	0xefbf,	0xef7f,	0xeeff,	0xedff,	0xebff,	0xe7ff,	0xefff,	0xcfff, 0xeffe,	0xeffd);

    MEAS_MPAT("XC_ALU_AANDB_02");
    compFm("XC_ALU_AAND_02",(1<<0), 16,0xeffe,	0xeffd,	0xeffb,	0xeff7,	0xefef,	0xefdf,	0xefbf,	0xef7f,	0xeeff,	0xedff,	0xebff,	0xe7ff,	0xefff,	0xcfff, 0xeffe,	0xeffd);
    compFm("XC_ALU_AAND_02",(1<<1), 16,0xdffe,	0xdffd,	0xdffb,	0xdff7,	0xdfef,	0xdfdf,	0xdfbf,	0xdf7f,	0xdeff,	0xddff,	0xdbff,	0xd7ff,	0xcfff,	0xdfff, 0xdffe,	0xdffd);

    MEAS_MPAT("XC_ALU_AANDB_03");
    compFm("XC_ALU_AAND_03",(1<<0), 16,0xdffe,	0xdffd,	0xdffb,	0xdff7,	0xdfef,	0xdfdf,	0xdfbf,	0xdf7f,	0xdeff,	0xddff,	0xdbff,	0xd7ff,	0xcfff,	0xdfff, 0xdffe,	0xdffd);
    compFm("XC_ALU_AAND_03",(1<<1), 16,0xcffe,	0xcffd,	0xcffb,	0xcff7,	0xcfef,	0xcfdf,	0xcfbf,	0xcf7f,	0xceff,	0xcdff,	0xcbff,	0xc7ff,	0xcfff,	0xcfff, 0xcffe,	0xcffd);

    MEAS_MPAT("XC_ALU_AANDB_04");
    compFm("XC_ALU_AAND_04",(1<<0), 16,0xcffe,	0xcffd,	0xcffb,	0xcff7,	0xcfef,	0xcfdf,	0xcfbf,	0xcf7f,	0xceff,	0xcdff,	0xcbff,	0xc7ff,	0xcfff,	0xcfff, 0xcffe,	0xcffd);
    compFm("XC_ALU_AAND_04",(1<<1), 16,0xbffe,	0xbffd,	0xbffb,	0xbff7,	0xbfef,	0xbfdf,	0xbfbf,	0xbf7f,	0xbeff,	0xbdff,	0xbbff,	0xb7ff,	0xafff,	0x9fff, 0xbffe,	0xbffd);

    MEAS_MPAT("XC_ALU_AANDB_05");
    compFm("XC_ALU_AAND_05",(1<<0), 16,0xbffe,	0xbffd,	0xbffb,	0xbff7,	0xbfef,	0xbfdf,	0xbfbf,	0xbf7f,	0xbeff,	0xbdff,	0xbbff,	0xb7ff,	0xafff,	0x9fff, 0xbffe,	0xbffd);
    compFm("XC_ALU_AAND_05",(1<<1), 16,0xaffe,	0xaffd,	0xaffb,	0xaff7,	0xafef,	0xafdf,	0xafbf,	0xaf7f,	0xaeff,	0xadff,	0xabff,	0xa7ff,	0xafff,	0x8fff, 0xaffe,	0xaffd);

    MEAS_MPAT("XC_ALU_AANDB_06");
    compFm("XC_ALU_AAND_06",(1<<0), 16,0xaffe,	0xaffd,	0xaffb,	0xaff7,	0xafef,	0xafdf,	0xafbf,	0xaf7f,	0xaeff,	0xadff,	0xabff,	0xa7ff,	0xafff,	0x8fff, 0xaffe,	0xaffd);
    compFm("XC_ALU_AAND_06",(1<<1), 16,0x9ffe,	0x9ffd,	0x9ffb,	0x9ff7,	0x9fef,	0x9fdf,	0x9fbf,	0x9f7f,	0x9eff,	0x9dff,	0x9bff,	0x97ff,	0x8fff,	0x9fff, 0x9ffe,	0x9ffd);

    MEAS_MPAT("XC_ALU_AANDB_07");
    compFm("XC_ALU_AAND_07",(1<<0), 16,0x9ffe,	0x9ffd,	0x9ffb,	0x9ff7,	0x9fef,	0x9fdf,	0x9fbf,	0x9f7f,	0x9eff,	0x9dff,	0x9bff,	0x97ff,	0x8fff,	0x9fff, 0x9ffe,	0x9ffd);
    compFm("XC_ALU_AAND_07",(1<<1), 16,0x8ffe,	0x8ffd,	0x8ffb,	0x8ff7,	0x8fef,	0x8fdf,	0x8fbf,	0x8f7f,	0x8eff,	0x8dff,	0x8bff,	0x87ff,	0x8fff,	0x8fff, 0x8ffe,	0x8ffd);
    
}
void testPG_alu_xcyc_2()
{
    diag_log("============ %s ============\n", __FUNCTION__);
    int fm_cnt;

    load_pds("diag_pds_data_xy16");
    SEND_MPAT("diag_xcyc_2.mpa");

    MEAS_MPAT("XC_ALU_AORB_00");
    compFm("XC_ALU_AORB_00",(1<<0), 16, 0x0000,	0x0001,	0x0003,	0x0007,	0x000f,	0x001f,	0x003f,	0x007f,	0x00ff,	0x01ff,	0x03ff,	0x07ff,	0x0fff,	0x1fff, 0x1fff, 0x1fff);
    compFm("XC_ALU_AORB_00",(1<<1), 16, 0x8000,	0x8001,	0x8003,	0x8007,	0x800f,	0x801f,	0x803f,	0x807f,	0x80ff,	0x81ff,	0x83ff,	0x87ff,	0x8fff,	0x9fff, 0x9fff, 0x9fff);

    MEAS_MPAT("XC_ALU_AORB_01");
    compFm("XC_ALU_AORB_01",(1<<0), 16, 0x1000,	0x1001,	0x1002,	0x1004,	0x1008,	0x1010,	0x1020,	0x1040,	0x1080,	0x1100,	0x1200,	0x1400,	0x1800,	0x1000, 0x1000,	0x1001);
    compFm("XC_ALU_AORB_01",(1<<1), 16, 0x9000,	0x9001,	0x9002,	0x9004,	0x9008,	0x9010,	0x9020,	0x9040,	0x9080,	0x9100,	0x9200,	0x9400,	0x9800,	0x9000, 0x9000,	0x9001);

    MEAS_MPAT("XC_ALU_AORB_02");
    compFm("XC_ALU_AORB_02",(1<<0), 16, 0x2000,	0x2001,	0x2002,	0x2004,	0x2008,	0x2010,	0x2020,	0x2040,	0x2080,	0x2100,	0x2200,	0x2400,	0x2800,	0x3000, 0x2000,	0x2001);
    compFm("XC_ALU_AORB_02",(1<<1), 16, 0xa000,	0xa001,	0xa002,	0xa004,	0xa008,	0xa010,	0xa020,	0xa040,	0xa080,	0xa100,	0xa200,	0xa400,	0xa800,	0xb000, 0xa000,	0xa001);

    MEAS_MPAT("XC_ALU_AORB_03");
    compFm("XC_ALU_AORB_03",(1<<0), 16, 0x3000,	0x3001,	0x3002,	0x3004,	0x3008,	0x3010,	0x3020,	0x3040,	0x3080,	0x3100,	0x3200,	0x3400,	0x3800,	0x3000, 0x3000,	0x3001);
    compFm("XC_ALU_AORB_03",(1<<1), 16, 0xb000,	0xb001,	0xb002,	0xb004,	0xb008,	0xb010,	0xb020,	0xb040,	0xb080,	0xb100,	0xb200,	0xb400,	0xb800,	0xb000, 0xb000,	0xb001);

    MEAS_MPAT("XC_ALU_AORB_04");
    compFm("XC_ALU_AORB_04",(1<<0), 16, 0x4000,	0x4001,	0x4002,	0x4004,	0x4008,	0x4010,	0x4020,	0x4040,	0x4080,	0x4100,	0x4200,	0x4400,	0x4800,	0x5000, 0x4000,	0x4001);
    compFm("XC_ALU_AORB_04",(1<<1), 16, 0xc000,	0xc001,	0xc002,	0xc004,	0xc008,	0xc010,	0xc020,	0xc040,	0xc080,	0xc100,	0xc200,	0xc400,	0xc800,	0xd000, 0xc000,	0xc001); 

    MEAS_MPAT("XC_ALU_AORB_05");
    compFm("XC_ALU_AORB_05",(1<<0), 16, 0x5000,	0x5001,	0x5002,	0x5004,	0x5008,	0x5010,	0x5020,	0x5040,	0x5080,	0x5100,	0x5200,	0x5400,	0x5800,	0x5000, 0x5000,	0x5001);
    compFm("XC_ALU_AORB_05",(1<<1), 16, 0xd000,	0xd001,	0xd002,	0xd004,	0xd008,	0xd010,	0xd020,	0xd040,	0xd080,	0xd100,	0xd200,	0xd400,	0xd800,	0xd000, 0xd000,	0xd001);

    MEAS_MPAT("XC_ALU_AORB_06");
    compFm("XC_ALU_AORB_06",(1<<0), 16, 0x6000,	0x6001,	0x6002,	0x6004,	0x6008,	0x6010,	0x6020,	0x6040,	0x6080,	0x6100,	0x6200,	0x6400,	0x6800,	0x7000, 0x6000,	0x6001);
    compFm("XC_ALU_AORB_06",(1<<1), 16, 0xe000,	0xe001,	0xe002,	0xe004,	0xe008,	0xe010,	0xe020,	0xe040,	0xe080,	0xe100,	0xe200,	0xe400,	0xe800,	0xf000, 0xe000,	0xe001);

    MEAS_MPAT("XC_ALU_AORB_07");
    compFm("XC_ALU_AORB_07",(1<<0), 16, 0x7000,	0x7001,	0x7002,	0x7004,	0x7008,	0x7010,	0x7020,	0x7040,	0x7080,	0x7100,	0x7200,	0x7400,	0x7800,	0x7000, 0x7000,	0x7001);
    compFm("XC_ALU_AORB_07",(1<<1), 16, 0xf000,	0xf001,	0xf002,	0xf004,	0xf008,	0xf010,	0xf020,	0xf040,	0xf080,	0xf100,	0xf200,	0xf400,	0xf800,	0xf000, 0xf000,	0xf001);  
    
    MEAS_MPAT("XC_ALU_AXORB_00");
    compFm("XC_ALU_A^B_00",(1<<0), 14, 0xffff,	0xfffe,	0xfffc,	0xfff8,	0xfff0,	0xffe0,	0xffc0,	0xff80,	0xff00,	0xfe00,	0xfc00,	0xf800,	0xf000,	0xe000);
    compFm("XC_ALU_A^B_00",(1<<1), 14, 0x0000,	0x0001,	0x0003,	0x0007,	0x000f,	0x001f,	0x003f,	0x007f,	0x00ff,	0x01ff,	0x03ff,	0x07ff,	0x0fff,	0x1fff);

    MEAS_MPAT("XC_ALU_AXORB_01");
    compFm("XC_ALU_A^B_01",(1<<0), 14, 0x1000,	0x1001,	0x1002,	0x1004,	0x1008,	0x1010,	0x1020,	0x1040,	0x1080,	0x1100,	0x1200,	0x1400,	0x1800,	0x0000);
    compFm("XC_ALU_A^B_01",(1<<1), 14, 0x9000,	0x9001,	0x9002,	0x9004,	0x9008,	0x9010,	0x9020,	0x9040,	0x9080,	0x9100,	0x9200,	0x9400,	0x9800,	0x8000);

    MEAS_MPAT("XC_ALU_AXORB_02");
    compFm("XC_ALU_A^B_02",(1<<0), 14, 0x2000,	0x2001,	0x2002,	0x2004,	0x2008,	0x2010,	0x2020,	0x2040,	0x2080,	0x2100,	0x2200,	0x2400,	0x2800,	0x3000);
    compFm("XC_ALU_A^B_02",(1<<1), 14, 0xa000,	0xa001,	0xa002,	0xa004,	0xa008,	0xa010,	0xa020,	0xa040,	0xa080,	0xa100,	0xa200,	0xa400,	0xa800,	0xb000);	

    MEAS_MPAT("XC_ALU_AXORB_03");
    compFm("XC_ALU_A^B_03",(1<<0), 14, 0x3000,	0x3001,	0x3002,	0x3004,	0x3008,	0x3010,	0x3020,	0x3040,	0x3080,	0x3100,	0x3200,	0x3400,	0x3800,	0x2000);
    compFm("XC_ALU_A^B_03",(1<<1), 14, 0xb000,	0xb001,	0xb002,	0xb004,	0xb008,	0xb010,	0xb020,	0xb040,	0xb080,	0xb100,	0xb200,	0xb400,	0xb800,	0xa000);	

    MEAS_MPAT("XC_ALU_AXORB_04");
    compFm("XC_ALU_A^B_04",(1<<0), 14, 0x4000,	0x4001,	0x4002,	0x4004,	0x4008,	0x4010,	0x4020,	0x4040,	0x4080,	0x4100,	0x4200,	0x4400,	0x4800,	0x5000);
    compFm("XC_ALU_A^B_04",(1<<1), 14, 0xc000,	0xc001,	0xc002,	0xc004,	0xc008,	0xc010,	0xc020,	0xc040,	0xc080,	0xc100,	0xc200,	0xc400,	0xc800,	0xd000);	

    MEAS_MPAT("XC_ALU_AXORB_05");
    compFm("XC_ALU_A^B_05",(1<<0), 14, 0x5000,	0x5001,	0x5002,	0x5004,	0x5008,	0x5010,	0x5020,	0x5040,	0x5080,	0x5100,	0x5200,	0x5400,	0x5800,	0x4000);
    compFm("XC_ALU_A^B_05",(1<<1), 14, 0xd000,	0xd001,	0xd002,	0xd004,	0xd008,	0xd010,	0xd020,	0xd040,	0xd080,	0xd100,	0xd200,	0xd400,	0xd800,	0xc000);	

    MEAS_MPAT("XC_ALU_AXORB_06");
    compFm("XC_ALU_A^B_06",(1<<0), 14, 0x6000,	0x6001,	0x6002,	0x6004,	0x6008,	0x6010,	0x6020,	0x6040,	0x6080,	0x6100,	0x6200,	0x6400,	0x6800,	0x7000);
    compFm("XC_ALU_A^B_06",(1<<1), 14, 0xe000,	0xe001,	0xe002,	0xe004,	0xe008,	0xe010,	0xe020,	0xe040,	0xe080,	0xe100,	0xe200,	0xe400,	0xe800,	0xf000);	

    MEAS_MPAT("XC_ALU_AXORB_07");
    compFm("XC_ALU_A^B_07",(1<<0), 14, 0x7000,	0x7001,	0x7002,	0x7004,	0x7008,	0x7010,	0x7020,	0x7040,	0x7080,	0x7100,	0x7200,	0x7400,	0x7800,	0x6000);
    compFm("XC_ALU_A^B_07",(1<<1), 14, 0xf000,	0xf001,	0xf002,	0xf004,	0xf008,	0xf010,	0xf020,	0xf040,	0xf080,	0xf100,	0xf200,	0xf400,	0xf800,	0xe000);	

    MEAS_MPAT("XC_ALU_D3D4_00");
    compFm("XC_ALU_D3D4_00",(1<<0),16, 0x1000, 0x1000, 0x0000, 0x0001, 0x0002, 0x0001, 0x0002, 0x0004, 0x0008, 0x0004, 0x0008, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800);
    compFm("XC_ALU_D3D4_00",(1<<1),16, 0xA000, 0xA000, 0xA001, 0x0000, 0x0001, 0x0002, 0x0001, 0x0002, 0x0001, 0x0002, 0x0004, 0x5000, 0x5000, 0x5000, 0x5000, 0x5000);

    MEAS_MPAT("XC_ALU_APImm_00");
    compFm("XC_ALU_APImm_00",(1<<0), 4, 0x0001,	0x0003,	0x0007,	0x000f);
    compFm("XC_ALU_APImm_00",(1<<1), 4, 0x8001,	0x8003,	0x8007,	0x800f);		

    MEAS_MPAT("XC_ALU_APImm_01");
    compFm("XC_ALU_APImm_01",(1<<0), 4, 0x0010,	0x0020,	0x0040,	0x0080);
    compFm("XC_ALU_APImm_01",(1<<1), 4, 0x0010,	0x0020,	0x0040,	0x0080); 	

    MEAS_MPAT("XC_ALU_APImm_02");
    compFm("XC_ALU_APImm_02",(1<<0), 4, 0x0100,	0x0200,	0x0400,	0x0800);
    compFm("XC_ALU_APImm_02",(1<<1), 4, 0x0100,	0x0200,	0x0400,	0x0800);	

    MEAS_MPAT("XC_ALU_APImm_03");	
    compFm("XC_ALU_APImm_03",(1<<0), 4, 0x1000,	0x2000,	0x4000,	0x8000);
    compFm("XC_ALU_APImm_03",(1<<1), 4, 0x1000,	0x2000,	0x4000,	0x8000);

    MEAS_MPAT("XC_ALU_APImm_04");
    compFm("XC_ALU_APImm_04",(1<<0), 4, 0x000a,	0x000b,	0x000c,	0x000d);
    compFm("XC_ALU_APImm_04",(1<<1), 4, 0x000a,	0x000b,	0x000c,	0x000d);	

    MEAS_MPAT("XC_ALU_APImm_05");
    compFm("XC_ALU_APImm_05",(1<<0), 4, 0x00a0,	0x00b0,	0x00c0,	0x00d0);
    compFm("XC_ALU_APImm_05",(1<<1), 4, 0x00a0,	0x00b0,	0x00c0,	0x00d0);	

    MEAS_MPAT("XC_ALU_APImm_06");
    compFm("XC_ALU_APImm_06",(1<<0), 4, 0x0a00,	0x0b00,	0x0c00,	0x0d00);
    compFm("XC_ALU_APImm_06",(1<<1), 4, 0x0a00,	0x0b00,	0x0c00,	0x0d00);	

    MEAS_MPAT("XC_ALU_APImm_07");
    compFm("XC_ALU_APImm_07",(1<<0), 4, 0xa000,	0xb000,	0xc000,	0xd000);
    compFm("XC_ALU_APImm_07",(1<<1), 4, 0xa000,	0xb000,	0xc000,	0xd000);

    MEAS_MPAT("XC_ALU_AMImm_00");
    compFm("XC_ALU_AMImm_00",(1<<0), 4, 0xfffe,	0xfffc,	0xfff8,	0xfff0);
    compFm("XC_ALU_AMImm_00",(1<<1), 4, 0xaffe,	0xaffc,	0xaff8,	0xaff0);			

    MEAS_MPAT("XC_ALU_AMImm_01");
    compFm("XC_ALU_AMImm_01",(1<<0), 4, 0xffef,	0xffdf,	0xffbf,	0xff7f);
    compFm("XC_ALU_AMImm_01",(1<<1), 4, 0xafef,	0xafdf,	0xafbf,	0xaf7f); 	

    MEAS_MPAT("XC_ALU_AMImm_02");
    compFm("XC_ALU_AMImm_02",(1<<0), 4, 0xfeff,	0xfdff,	0xfbff,	0xf7ff);
    compFm("XC_ALU_AMImm_02",(1<<1), 4, 0xaeff,	0xadff,	0xabff,	0xa7ff);	

    MEAS_MPAT("XC_ALU_AMImm_03");
    compFm("XC_ALU_AMImm_03",(1<<0), 4, 0xefff,	0xdfff,	0xbfff,	0x7fff);
    compFm("XC_ALU_AMImm_03",(1<<1), 4, 0x9fff,	0x8fff,	0x6fff,	0x2fff);

    MEAS_MPAT("XC_ALU_AMImm_04");
    compFm("XC_ALU_AMImm_04",(1<<0), 4, 0xfff5,	0xfff4,	0xfff3,	0xfff2);
    compFm("XC_ALU_AMImm_04",(1<<1), 4, 0xaff5,	0xaff4,	0xaff3,	0xaff2);	

    MEAS_MPAT("XC_ALU_AMImm_05");
    compFm("XC_ALU_AMImm_05",(1<<0), 4, 0xff5f,	0xff4f,	0xff3f,	0xff2f);
    compFm("XC_ALU_AMImm_05",(1<<1), 4, 0xaf5f,	0xaf4f,	0xaf3f,	0xaf2f);	

    MEAS_MPAT("XC_ALU_AMImm_06");
    compFm("XC_ALU_AMImm_06",(1<<0), 4, 0xf5ff,	0xf4ff,	0xf3ff,	0xf2ff);
    compFm("XC_ALU_AMImm_06",(1<<1), 4, 0xa5ff,	0xa4ff,	0xa3ff,	0xa2ff);		

    MEAS_MPAT("XC_ALU_AMImm_07");
    compFm("XC_ALU_AMImm_07",(1<<0), 4, 0x5fff,	0x4fff,	0x3fff,	0x2fff);
    compFm("XC_ALU_AMImm_07",(1<<1), 4, 0x0fff,	0xffff,	0xefff,	0xdfff);
}

void testPG_alu_xdyd()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;

    	
	load_pds("diag_pds_data_xy16");
    SEND_MPAT("diag_xdyd.mpa");

    MEAS_MPAT("XD_ALU_A_00");
    compFm("XD_ALU_A_00",(1<<0), 16, 0x0000,	0x1000,	0x2000,	0x3000,	0x3000,	0x5000,	0x6000,	0x7000,	0x8000,	0x9000,	0xa000,	0xb000,	0xb000,	0xd000,	0xe000,	0xf000);
    compFm("XD_ALU_A_00",(1<<1), 16, 0x8000,	0x9000,	0xa000,	0xb000,	0xb000,	0xd000,	0xe000,	0xf000,	0x0000,	0x1000,	0x2000,	0x3000,	0x3000,	0x5000,	0x6000,	0x7000);	

    MEAS_MPAT("XD_ALU_NA_00");
    compFm("XD_ALU_NA_00",(1<<0), 16, 0xffff,	0xfffe,	0xfffd,	0xfffb,	0x0004,	0xffef,	0xffdf,	0xffbf,	0xffff,	0xfff5,	0xfff4,	0xfff3,	0x000c,	0xff5f,	0xff4f,	0xff3f);
    compFm("XD_ALU_NA_00",(1<<1), 16, 0x0000,	0x8000,	0xc000,	0xe000,	0x1fff,	0xf800,	0xfc00,	0xfe00,	0xffff,	0xfffe,	0xfffd,	0xfffb,	0x0004,	0xffef,	0xffdf,	0xffbf);

    MEAS_MPAT("XD_ALU_AP1_00");
    compFm("XD_ALU_AP1_00",(1<<0), 16, 0x0001,	0x0002,	0x0003,	0x0005,	0x0006,	0x0011,	0x0021,	0x0041,	0xffff,	0x8000,	0x4000,	0x2000,	0x2001,	0x0800,	0x0400,	0x0200);
    compFm("XD_ALU_AP1_00",(1<<1), 16, 0xffff,	0x8000,	0x4000,	0x2000,	0x2001,	0x0800,	0x0400,	0x0200,	0x0001,	0x0002,	0x0003,	0x0005,	0x0006,	0x0011,	0x0021,	0x0041);
 
    MEAS_MPAT("XD_ALU_AM1_00");
    compFm("XD_ALU_AM1_00",(1<<0), 16, 0x0000,	0x0001,	0x0003,	0x0007,	0x0006,	0x001f,	0x003f,	0x007f,	0xfffe,	0x7ffe,	0x3ffe,	0x1ffe,	0x1ffd,	0x07fe,	0x03fe,	0x01fe);
    compFm("XD_ALU_AM1_00",(1<<1), 16, 0xfffe,	0x7ffe,	0x3ffe,	0x1ffe,	0x1ffd,	0x07fe,	0x03fe,	0x01fe,	0x0000,	0x0001,	0x0003,	0x0007,	0x0006,	0x001f,	0x003f,	0x007f);

    MEAS_MPAT("XD_ALU_AM2_00");
    compFm("XD_ALU_AM2_00",(1<<0), 16, 0x0000,	0x0002,	0x0004,	0x0008,	0x0010,	0x0020,	0x0040,	0x0080,	0x0000,	0x0014,	0x0016,	0x0018,	0x0030,	0x0140,	0x0160,	0x0180);
    compFm("XD_ALU_AM2_00",(1<<1), 16, 0x0000,	0x0014,	0x0016,	0x0018,	0x0030,	0x0140,	0x0160,	0x0180,	0x0000,	0x0002,	0x0004,	0x0008,	0x0010,	0x0020,	0x0040,	0x0080);
    
    MEAS_MPAT("XD_ALU_AD2_00");
    compFm("XD_ALU_AD2_00",(1<<0), 16, 0x0000,	0x0000,	0x0001,	0x0002,	0x0001,	0x0008,	0x0010,	0x0020,	0x0000,	0x0005,	0x0050,	0x0500,	0x0280,	0x5555,	0x0555,	0x0055);
    compFm("XD_ALU_AD2_00",(1<<1), 16, 0x0000,	0x0005,	0x0050,	0x0500,	0x0280,	0x5555,	0x0555,	0x0055,	0x0000,	0x0000,	0x0001,	0x0002,	0x0001,	0x0008,	0x0010,	0x0020);

    MEAS_MPAT("XD_ALU_B_00");   
    compFm("XD_ALU_B_00",(1<<0), 16, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0xFFFF, 0x0000);
    compFm("XD_ALU_B_00",(1<<1), 16, 0x0080, 0x0100, 0x0200, 0x0400, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x1000, 0x0800, 0xFFFF, 0x0080);
    
    MEAS_MPAT("XD_ALU_APB_00");
    compFm("XD_ALU_APB_00",(1<<0), 16, 0x0001, 0x0010, 0x0100, 0x0001, 0x0010, 0x0100, 0x0001, 0x0010, 0x0100, 0x0001, 0x0010, 0x0100, 0x0010, 0x0001, 0x0001, 0x0010);
    compFm("XD_ALU_APB_00",(1<<1), 16, 0x8100, 0x8001, 0x8010, 0x8100, 0x8001, 0x8010, 0x8100, 0x8001, 0x8010, 0x8100, 0x8001, 0x8010, 0x8001, 0x8010, 0x8100, 0x8001);
    
    MEAS_MPAT("XD_ALU_APB_01");
    compFm("XD_ALU_APB_01",(1<<0),16, 0x1001, 0x1010, 0x1100, 0x1001, 0x1010, 0x1100, 0x1001, 0x1010, 0x1100, 0x1001, 0x1010, 0x1100, 0x1010, 0x1001, 0x1001, 0x1010);
    compFm("XD_ALU_APB_01",(1<<1),16, 0x9100, 0x9001, 0x9010, 0x9100, 0x9001, 0x9010, 0x9100, 0x9001, 0x9010, 0x9100, 0x9001, 0x9010, 0x9001, 0x9010, 0x9100, 0x9001);

    MEAS_MPAT("XD_ALU_APB_02");
    compFm("XD_ALU_APB_02",(1<<0),16, 0x2001, 0x2010, 0x2100, 0x2001, 0x2010, 0x2100, 0x2001, 0x2010, 0x2100, 0x2001, 0x2010, 0x2100, 0x2010, 0x2001, 0x2001, 0x2010);
    compFm("XD_ALU_APB_02",(1<<1),16, 0xA100, 0xA001, 0xA010, 0xA100, 0xA001, 0xA010, 0xA100, 0xA001, 0xA010, 0xA100, 0xA001, 0xA010, 0xA001, 0xA010, 0xA100, 0xA001);

    MEAS_MPAT("XD_ALU_APB_03");
    compFm("XD_ALU_APB_03",(1<<0),16, 0x3001,	0x3010,	0x3100,	0x3001,	0x3010,	0x3100,	0x3001,	0x3010,	0x3100,	0x3001,	0x3010,	0x3100,	0x3010,	0x3001,	0x3001,	0x3010);
    compFm("XD_ALU_APB_03",(1<<1),16, 0xb100,	0xb001,	0xb010,	0xb100,	0xb001,	0xb010,	0xb100,	0xb001,	0xb010,	0xb100,	0xb001,	0xb010,	0xb001,	0xb010,	0xb100,	0xb001);

    MEAS_MPAT("XD_ALU_APB_04");
    compFm("XD_ALU_APB_04",(1<<0),16, 0x4001,	0x4011,	0x4111,	0x4112,	0x4122,	0x4222,	0x4223,	0x4233,	0x4333,	0x4334,	0x4344,	0x4444,	0x4454,	0x4455,	0x4456,	0x4466);
    compFm("XD_ALU_APB_04",(1<<1),16, 0xc100,	0xc101,	0xc111,	0xc211,	0xc212,	0xc222,	0xc322,	0xc323,	0xc333,	0xc433,	0xc434,	0xc444,	0xc445,	0xc455,	0xc555,	0xc556);

    MEAS_MPAT("XD_ALU_APB_05");
    compFm("XD_ALU_APB_05",(1<<0),16, 0x5001, 0x5010, 0x5100, 0x5001, 0x5010, 0x5100, 0x5001, 0x5010, 0x5100, 0x5001, 0x5010, 0x5100, 0x5010, 0x5001, 0x5001, 0x5010);
    compFm("XD_ALU_APB_05",(1<<1),16, 0xD100, 0xD001, 0xD010, 0xD100, 0xD001, 0xD010, 0xD100, 0xD001, 0xD010, 0xD100, 0xD001, 0xD010, 0xD001, 0xD010, 0xD100, 0xD001);

    MEAS_MPAT("XD_ALU_APB_06");
    compFm("XD_ALU_APB_06",(1<<0),16, 0x6001, 0x6010, 0x6100, 0x6001, 0x6010, 0x6100, 0x6001, 0x6010, 0x6100, 0x6001, 0x6010, 0x6100, 0x6010, 0x6001, 0x6001, 0x6010);
    compFm("XD_ALU_APB_06",(1<<1),16, 0xE100, 0xE001, 0xE010, 0xE100, 0xE001, 0xE010, 0xE100, 0xE001, 0xE010, 0xE100, 0xE001, 0xE010, 0xE001, 0xE010, 0xE100, 0xE001);

	MEAS_MPAT("XD_ALU_APB_07");
    compFm("XD_ALU_APB_07",(1<<0),16, 0x7001, 0x7010, 0x7100, 0x7001, 0x7010, 0x7100, 0x7001, 0x7010, 0x7100, 0x7001, 0x7010, 0x7100, 0x7010, 0x7001, 0x7001, 0x7010);
    compFm("XD_ALU_APB_07",(1<<1),16, 0xF100, 0xF001, 0xF010, 0xF100, 0xF001, 0xF010, 0xF100, 0xF001, 0xF010, 0xF100, 0xF001, 0xF010, 0xF001, 0xF010, 0xF100, 0xF001); 
   
    MEAS_MPAT("XD_ALU_AMB_00");
    compFm("XD_ALU_AMB_00",(1<<0),16 ,0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0656, 0x0665, 0x0665, 0x0656);
    compFm("XD_ALU_AMB_00",(1<<1),16 ,0x0554, 0x0545, 0x0455, 0x0554, 0x0545, 0x0455, 0x0554, 0x0545, 0x0455, 0x0554, 0x0545, 0x0455, 0x0545, 0x0554, 0x0554, 0x0545);
    
    MEAS_MPAT("XD_ALU_AMB_01");
    compFm("XD_ALU_AMB_01",(1<<0),16 ,0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0767, 0x0776, 0x0776, 0x0767);
    compFm("XD_ALU_AMB_01",(1<<1),16 ,0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0656, 0x0665, 0x0665, 0x0656);
    
    MEAS_MPAT("XD_ALU_AMB_02");
    compFm("XD_ALU_AMB_02",(1<<0),16 ,0x0887, 0x0878, 0x0788, 0x0887, 0x0878, 0x0788, 0x0887, 0x0878, 0x0788, 0x0887, 0x0878, 0x0788, 0x0878, 0x0887, 0x0887, 0x0878);
    compFm("XD_ALU_AMB_02",(1<<1),16 ,0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0767, 0x0776, 0x0776, 0x0767);
    
    MEAS_MPAT("XD_ALU_AMB_03");
    compFm("XD_ALU_AMB_03",(1<<0),16 ,0x0998,0x0989,	0x0899,	0x0998,	0x0989,	0x0899,	0x0998,	0x0989,	0x0899,	0x0998,	0x0989,	0x0899,	0x0989,	0x0998,	0x0998,	0x0989);
    compFm("XD_ALU_AMB_03",(1<<1),16 ,0x0887,0x0878,	0x0788,	0x0887,	0x0878,	0x0788,	0x0887,	0x0878,	0x0788,	0x0887,	0x0878,	0x0788,	0x0878,	0x0887,	0x0887,	0x0878);
 
    MEAS_MPAT("XD_ALU_AMB_04");
    compFm("XD_ALU_AMB_04",(1<<0),16 ,0x0aa9,	0x0a99,	0x0999,	0x0998,	0x0988,	0x0888,	0x0887,	0x0877,	0x0777,	0x0776,	0x0766,	0x0666,	0x0656,	0x0655,	0x0654,	0x0644);
    compFm("XD_ALU_AMB_04",(1<<1),16 ,0x0998,	0x0988,	0x0888,	0x0887,	0x0877,	0x0777,	0x0776,	0x0766,	0x0666,	0x0665,	0x0655,	0x0555,	0x0545,	0x0544,	0x0543,	0x0533);

    MEAS_MPAT("XD_ALU_AMB_05");
    compFm("XD_ALU_AMB_05",(1<<0),16 ,0x0BBA, 0x0BAB, 0x0ABB, 0x0BBA, 0x0BAB, 0x0ABB, 0x0BBA, 0x0BAB, 0x0ABB, 0x0BBA, 0x0BAB, 0x0ABB, 0x0BAB, 0x0BBA, 0x0BBA, 0x0BAB);
    compFm("XD_ALU_AMB_05",(1<<1),16 ,0x0AA9, 0x0A9A, 0x09AA, 0x0AA9, 0x0A9A, 0x09AA, 0x0AA9, 0x0A9A, 0x09AA, 0x0AA9, 0x0A9A, 0x09AA, 0x0A9A, 0x0AA9, 0x0AA9, 0x0A9A);
    
    MEAS_MPAT("XD_ALU_AMB_06");
    compFm("XD_ALU_AMB_06",(1<<0),16 ,0x0CCB, 0x0CBC, 0x0BCC, 0x0CCB, 0x0CBC, 0x0BCC, 0x0CCB, 0x0CBC, 0x0BCC, 0x0CCB, 0x0CBC, 0x0BCC, 0x0CBC, 0x0CCB, 0x0CCB, 0x0CBC);
    compFm("XD_ALU_AMB_06",(1<<1),16 ,0x0BBA, 0x0BAB, 0x0ABB, 0x0BBA, 0x0BAB, 0x0ABB, 0x0BBA, 0x0BAB, 0x0ABB, 0x0BBA, 0x0BAB, 0x0ABB, 0x0BAB, 0x0BBA, 0x0BBA, 0x0BAB);
    
    MEAS_MPAT("XD_ALU_AMB_07");
    compFm("XD_ALU_AMB_07",(1<<0),16 ,0x0DDC, 0x0DCD, 0x0CDD, 0x0DDC, 0x0DCD, 0x0CDD, 0x0DDC, 0x0DCD, 0x0CDD, 0x0DDC, 0x0DCD, 0x0CDD, 0x0DCD, 0x0DDC, 0x0DDC, 0x0DCD);
    compFm("XD_ALU_AMB_07",(1<<1),16 ,0x0CCB, 0x0CBC, 0x0BCC, 0x0CCB, 0x0CBC, 0x0BCC, 0x0CCB, 0x0CBC, 0x0BCC, 0x0CCB, 0x0CBC, 0x0BCC, 0x0CBC, 0x0CCB, 0x0CCB, 0x0CBC);
    
    MEAS_MPAT("XD_ALU_AANDB_00");
    compFm("XD_ALU_AANDB_00",(1<<0), 14,0xFFFE, 0xFFFD, 0xFFFB, 0xFFF7, 0xFFEF, 0xFFDF, 0xFFBF, 0xFF7F, 0xFEFF, 0xFDFF, 0xFBFF, 0xF7FF, 0xEFFF, 0xDFFF);
    compFm("XD_ALU_AANDB_00",(1<<1), 14,0xEFFE, 0xEFFD, 0xEFFB, 0xEFF7, 0xEFEF, 0xEFDF, 0xEFBF, 0xEF7F, 0xEEFF, 0xEDFF, 0xEBFF, 0xE7FF, 0xEFFF, 0xCFFF);
    
    MEAS_MPAT("XD_ALU_AANDB_01");
    compFm("XD_ALU_AANDB_01",(1<<0), 14,0xfffe,	0xfffd,	0xfffb,	0xfff7,	0xffef,	0xffdf,	0xffbf,	0xff7f,	0xfeff,	0xfdff,	0xfbff,	0xf7ff,	0xefff,	0xdfff);
    compFm("XD_ALU_AANDB_01",(1<<1), 14,0xeffe,	0xeffd,	0xeffb,	0xeff7,	0xefef,	0xefdf,	0xefbf,	0xef7f,	0xeeff,	0xedff,	0xebff,	0xe7ff,	0xefff,	0xcfff);
    
    MEAS_MPAT("XD_ALU_AANDB_02");
    compFm("XD_ALU_AANDB_02",(1<<0), 14,0xfffe,	0xfffd,	0xfffb,	0xfff7,	0xffef,	0xffdf,	0xffbf,	0xff7f,	0xfeff,	0xfdff,	0xfbff,	0xf7ff,	0xefff,	0xdfff);
    compFm("XD_ALU_AANDB_02",(1<<1), 14,0xeffe,	0xeffd,	0xeffb,	0xeff7,	0xefef,	0xefdf,	0xefbf,	0xef7f,	0xeeff,	0xedff,	0xebff,	0xe7ff,	0xefff,	0xcfff);

    MEAS_MPAT("XD_ALU_AANDB_03");
    compFm("XD_ALU_AANDB_03",(1<<0), 14,0xdffe,	0xdffd,	0xdffb,	0xdff7,	0xdfef,	0xdfdf,	0xdfbf,	0xdf7f,	0xdeff,	0xddff,	0xdbff,	0xd7ff,	0xcfff,	0xdfff);
    compFm("XD_ALU_AANDB_03",(1<<1), 14,0xcffe,	0xcffd,	0xcffb,	0xcff7,	0xcfef,	0xcfdf,	0xcfbf,	0xcf7f,	0xceff,	0xcdff,	0xcbff,	0xc7ff,	0xcfff,	0xcfff);

    MEAS_MPAT("XD_ALU_AANDB_04");
    compFm("XD_ALU_AANDB_04",(1<<0), 14,0xcffe,	0xcffc,	0xcff8,	0xcff0,	0xcfe0,	0xcfc0,	0xcf80,	0xcf00,	0xce00,	0xcc00,	0xc800,	0xc000,	0xc000,	0xc000);
    compFm("XD_ALU_AANDB_04",(1<<1), 14,0xbffe,	0xbffc,	0xbff8,	0xbff0,	0xbfe0,	0xbfc0,	0xbf80,	0xbf00,	0xbe00,	0xbc00,	0xb800,	0xb000,	0xa000,	0x8000);

    
    MEAS_MPAT("XD_ALU_AANDB_05");
    compFm("XD_ALU_AANDB_05",(1<<0), 14,0xbffe,	0xbffd,	0xbffb,	0xbff7,	0xbfef,	0xbfdf,	0xbfbf,	0xbf7f,	0xbeff,	0xbdff,	0xbbff,	0xb7ff,	0xafff,	0x9fff);
    compFm("XD_ALU_AANDB_05",(1<<1), 14,0xaffe,	0xaffd,	0xaffb,	0xaff7,	0xafef,	0xafdf,	0xafbf,	0xaf7f,	0xaeff,	0xadff,	0xabff,	0xa7ff,	0xafff,	0x8fff);
    
    MEAS_MPAT("XD_ALU_AANDB_06");
    compFm("XD_ALU_AANDB_06",(1<<0), 14,0xaffe,	0xaffd,	0xaffb,	0xaff7,	0xafef,	0xafdf,	0xafbf,	0xaf7f,	0xaeff,	0xadff,	0xabff,	0xa7ff,	0xafff,	0x8fff);
    compFm("XD_ALU_AANDB_06",(1<<1), 14,0x9ffe,	0x9ffd,	0x9ffb,	0x9ff7,	0x9fef,	0x9fdf,	0x9fbf,	0x9f7f,	0x9eff,	0x9dff,	0x9bff,	0x97ff,	0x8fff,	0x9fff);
    
    MEAS_MPAT("XD_ALU_AANDB_07");
    compFm("XD_ALU_AANDB_07",(1<<0), 14,0x9ffe,	0x9ffd,	0x9ffb,	0x9ff7,	0x9fef,	0x9fdf,	0x9fbf,	0x9f7f,	0x9eff,	0x9dff,	0x9bff,	0x97ff,	0x8fff,	0x9fff);
    compFm("XD_ALU_AANDB_07",(1<<1), 14,0x8ffe,	0x8ffd,	0x8ffb,	0x8ff7,	0x8fef,	0x8fdf,	0x8fbf,	0x8f7f,	0x8eff,	0x8dff,	0x8bff,	0x87ff,	0x8fff,	0x8fff);   


}
void testPG_alu_xdyd_2()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;
     load_pds("diag_pds_data_xy16");

    SEND_MPAT("diag_xdyd_2.mpa");

    MEAS_MPAT("XD_ALU_AORB_00");
    compFm("XD_ALU_AORB_00",(1<<0), 14, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000);
    compFm("XD_ALU_AORB_00",(1<<1), 14, 0x8000, 0x8001, 0x8002, 0x8004, 0x8008, 0x8010, 0x8020, 0x8040, 0x8080, 0x8100, 0x8200, 0x8400, 0x8800, 0x9000);
    
    MEAS_MPAT("XD_ALU_AORB_01");
    compFm("XD_ALU_AORB_01",(1<<0), 14, 0x1000,	0x1001,	0x1002,	0x1004,	0x1008,	0x1010,	0x1020,	0x1040,	0x1080,	0x1100,	0x1200,	0x1400,	0x1800,	0x1000);
    compFm("XD_ALU_AORB_01",(1<<1), 14, 0x9000,	0x9001,	0x9002,	0x9004,	0x9008,	0x9010,	0x9020,	0x9040,	0x9080,	0x9100,	0x9200,	0x9400,	0x9800,	0x9000);

    MEAS_MPAT("XD_ALU_AORB_02");
    compFm("XD_ALU_AORB_02",(1<<0), 14, 0x2000,	0x2001,	0x2002,	0x2004,	0x2008,	0x2010,	0x2020,	0x2040,	0x2080,	0x2100,	0x2200,	0x2400,	0x2800,	0x3000);
    compFm("XD_ALU_AORB_02",(1<<1), 14, 0xa000,	0xa001,	0xa002,	0xa004,	0xa008,	0xa010,	0xa020,	0xa040,	0xa080,	0xa100,	0xa200,	0xa400,	0xa800,	0xb000);

    MEAS_MPAT("XD_ALU_AORB_03");
    compFm("XD_ALU_AORB_03",(1<<0), 16, 0x3000,	0x3001,	0x3002,	0x3004,	0x3008,	0x3010,	0x3020,	0x3040,	0x3080,	0x3100,	0x3200,	0x3400,	0x3800,	0x3000, 0x3000,	0x3001);
    compFm("XD_ALU_AORB_03",(1<<1), 16, 0xb000,	0xb001,	0xb002,	0xb004,	0xb008,	0xb010,	0xb020,	0xb040,	0xb080,	0xb100,	0xb200,	0xb400,	0xb800,	0xb000, 0xb000,	0xb001);

    MEAS_MPAT("XD_ALU_AORB_04");
    compFm("XD_ALU_AORB_04",(1<<0), 14, 0x4000,	0x4001,	0x4003,	0x4007,	0x400f,	0x401f,	0x403f,	0x407f,	0x40ff,	0x41ff,	0x43ff,	0x47ff,	0x4fff,	0x5fff);
    compFm("XD_ALU_AORB_04",(1<<1), 14, 0xc000,	0xc001,	0xc003,	0xc007,	0xc00f,	0xc01f,	0xc03f,	0xc07f,	0xc0ff,	0xc1ff,	0xc3ff,	0xc7ff,	0xcfff,	0xdfff);
   
    MEAS_MPAT("XD_ALU_AORB_05");
    compFm("XD_ALU_AORB_05",(1<<0), 14, 0x5000,	0x5001,	0x5002,	0x5004,	0x5008,	0x5010,	0x5020,	0x5040,	0x5080,	0x5100,	0x5200,	0x5400,	0x5800,	0x5000);
    compFm("XD_ALU_AORB_05",(1<<1), 14, 0xd000,	0xd001,	0xd002,	0xd004,	0xd008,	0xd010,	0xd020,	0xd040,	0xd080,	0xd100,	0xd200,	0xd400,	0xd800,	0xd000);

    MEAS_MPAT("XD_ALU_AORB_06");
    compFm("XD_ALU_AORB_06",(1<<0), 14, 0x6000,	0x6001,	0x6002,	0x6004,	0x6008,	0x6010,	0x6020,	0x6040,	0x6080,	0x6100,	0x6200,	0x6400,	0x6800,	0x7000);
    compFm("XD_ALU_AORB_06",(1<<1), 14, 0xe000,	0xe001,	0xe002,	0xe004,	0xe008,	0xe010,	0xe020,	0xe040,	0xe080,	0xe100,	0xe200,	0xe400,	0xe800,	0xf000);

    MEAS_MPAT("XD_ALU_AORB_07");
    compFm("XD_ALU_AORB_07",(1<<0), 14, 0x7000,	0x7001,	0x7002,	0x7004,	0x7008,	0x7010,	0x7020,	0x7040,	0x7080,	0x7100,	0x7200,	0x7400,	0x7800,	0x7000);
    compFm("XD_ALU_AORB_07",(1<<1), 14, 0xf000,	0xf001,	0xf002,	0xf004,	0xf008,	0xf010,	0xf020,	0xf040,	0xf080,	0xf100,	0xf200,	0xf400,	0xf800,	0xf000);

    MEAS_MPAT("XD_ALU_AXORB_00");
    compFm("XD_ALU_A^B_00",(1<<0), 14, 0xFFFF, 0xFFFE, 0xFFFD, 0xFFFB, 0xFFF7, 0xFFEF, 0xFFDF, 0xFFBF, 0xFF7F, 0xFEFF, 0xFDFF, 0xFBFF, 0xF7FF, 0xEFFF);
    compFm("XD_ALU_A^B_00",(1<<1), 14, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000);

    MEAS_MPAT("XD_ALU_AXORB_01");
    compFm("XD_ALU_A^B_01",(1<<0), 14, 0x1000, 0x1001, 0x1002, 0x1004, 0x1008, 0x1010, 0x1020,0x1040,	0x1080,	0x1100,	0x1200,	0x1400,	0x1800,	0x0000);
    compFm("XD_ALU_A^B_01",(1<<1), 14, 0x9000, 0x9001, 0x9002, 0x9004, 0x9008, 0x9010, 0x9020,0x9040,	0x9080,	0x9100,	0x9200,	0x9400,	0x9800,	0x8000);

    MEAS_MPAT("XD_ALU_AXORB_02");
    compFm("XD_ALU_A^B_02",(1<<0), 14, 0x2000,	0x2001,	0x2002,	0x2004,	0x2008,	0x2010,	0x2020,	0x2040,	0x2080,	0x2100,	0x2200,	0x2400,	0x2800,	0x3000);
    compFm("XD_ALU_A^B_02",(1<<1), 14, 0xa000,	0xa001,	0xa002,	0xa004,	0xa008,	0xa010,	0xa020,	0xa040,	0xa080,	0xa100,	0xa200,	0xa400,	0xa800,	0xb000);	

    MEAS_MPAT("XD_ALU_AXORB_03");
    compFm("XD_ALU_A^B_03",(1<<0), 14, 0x3000,	0x3001,	0x3002,	0x3004,	0x3008,	0x3010,	0x3020,	0x3040,	0x3080,	0x3100,	0x3200,	0x3400,	0x3800,	0x2000);
    compFm("XD_ALU_A^B_03",(1<<1), 14, 0xb000,	0xb001,	0xb002,	0xb004,	0xb008,	0xb010,	0xb020,	0xb040,	0xb080,	0xb100,	0xb200,	0xb400,	0xb800,	0xa000);

    MEAS_MPAT("XD_ALU_AXORB_04");
    compFm("XD_ALU_A^B_04",(1<<0), 14, 0x4000,	0x4001,	0x4003,	0x4007,	0x400f,	0x401f,	0x403f,	0x407f,	0x40ff,	0x41ff,	0x43ff,	0x47ff,	0x4fff,	0x5fff);
    compFm("XD_ALU_A^B_04",(1<<1), 14, 0xc000,	0xc001,	0xc003,	0xc007,	0xc00f,	0xc01f,	0xc03f,	0xc07f,	0xc0ff,	0xc1ff,	0xc3ff,	0xc7ff,	0xcfff,	0xdfff);	

    MEAS_MPAT("XD_ALU_AXORB_05");
    compFm("XD_ALU_A^B_05",(1<<0), 14, 0x5000,	0x5001,	0x5002,	0x5004,	0x5008,	0x5010,	0x5020,	0x5040,	0x5080,	0x5100,	0x5200,	0x5400,	0x5800,	0x4000);
    compFm("XD_ALU_A^B_05",(1<<1), 14, 0xd000,	0xd001,	0xd002,	0xd004,	0xd008,	0xd010,	0xd020,	0xd040,	0xd080,	0xd100,	0xd200,	0xd400,	0xd800,	0xc000);	

    MEAS_MPAT("XD_ALU_AXORB_06");
    compFm("XD_ALU_A^B_06",(1<<0), 14, 0x6000,	0x6001,	0x6002,	0x6004,	0x6008,	0x6010,	0x6020,	0x6040,	0x6080,	0x6100,	0x6200,	0x6400,	0x6800,	0x7000);
    compFm("XD_ALU_A^B_06",(1<<1), 14, 0xe000,	0xe001,	0xe002,	0xe004,	0xe008,	0xe010,	0xe020,	0xe040,	0xe080,	0xe100,	0xe200,	0xe400,	0xe800,	0xf000);	

    MEAS_MPAT("XD_ALU_AXORB_07");
    compFm("XD_ALU_A^B_07",(1<<0), 14, 0x7000,	0x7001,	0x7002,	0x7004,	0x7008,	0x7010,	0x7020,	0x7040,	0x7080,	0x7100,	0x7200,	0x7400,	0x7800,	0x6000);
    compFm("XD_ALU_A^B_07",(1<<1), 14, 0xf000,	0xf001,	0xf002,	0xf004,	0xf008,	0xf010,	0xf020,	0xf040,	0xf080,	0xf100,	0xf200,	0xf400,	0xf800,	0xe000);	
    
    MEAS_MPAT("XD_ALU_D3D4_00");
    compFm("XD_ALU_D3D4_00",(1<<0),14 , 0x1111, 0x1111, 0x0000, 0x0001, 0x0002, 0x0001, 0x0002, 0x0004, 0x0008, 0x0004, 0x0008, 0x0888, 0x0888, 0x0888);
    compFm("XD_ALU_D3D4_00",(1<<1),14 , 0xAAAA, 0xAAAA, 0xAAAB, 0x0000, 0x0001, 0x0002, 0x0001, 0x0002, 0x0001, 0x0002, 0x0004, 0x5555, 0x5555, 0x5555);

    MEAS_MPAT("XD_ALU_APImm_00");
    compFm("XD_ALU_APImm_00",(1<<0), 4, 0x0001,	0x0002,	0x0004,	0x0008);
    compFm("XD_ALU_APImm_00",(1<<1), 4, 0x8001,	0x8002,	0x8004,	0x8008);	

    MEAS_MPAT("XD_ALU_APImm_01");
    compFm("XD_ALU_APImm_01",(1<<0), 4, 0x0010,	0x0020,	0x0040,	0x0080);
    compFm("XD_ALU_APImm_01",(1<<1), 4, 0x8010,	0x8020,	0x8040,	0x8080); 

    MEAS_MPAT("XD_ALU_APImm_02");
    compFm("XD_ALU_APImm_02",(1<<0), 4, 0x0100,	0x0200,	0x0400,	0x0800);
    compFm("XD_ALU_APImm_02",(1<<1), 4, 0x8100,	0x8200,	0x8400,	0x8800);	

    MEAS_MPAT("XD_ALU_APImm_03");	
    compFm("XD_ALU_APImm_03",(1<<0), 4, 0x1000,	0x2000,	0x4000,	0x8000);
    compFm("XD_ALU_APImm_03",(1<<1), 4, 0x1001,	0x2001,	0x4001,	0x8001);

    MEAS_MPAT("XD_ALU_APImm_04");
    compFm("XD_ALU_APImm_04",(1<<0), 4, 0x000a,	0x0015,	0x0021,	0x002e);
    compFm("XD_ALU_APImm_04",(1<<1), 4, 0x800a,	0x8015,	0x8021,	0x802e);	

    MEAS_MPAT("XD_ALU_APImm_05");
    compFm("XD_ALU_APImm_05",(1<<0), 4, 0x00a0,	0x00b0,	0x00c0,	0x00d0);
    compFm("XD_ALU_APImm_05",(1<<1), 4, 0x80a0,	0x80b0,	0x80c0,	0x80d0);	

    MEAS_MPAT("XD_ALU_APImm_06");
    compFm("XD_ALU_APImm_06",(1<<0), 4, 0x0a00,	0x0b00,	0x0c00,	0x0d00);
    compFm("XD_ALU_APImm_06",(1<<1), 4, 0x0a01,	0x0b01,	0x0c01,	0x0d01);	

    MEAS_MPAT("XD_ALU_APImm_07");
    compFm("XD_ALU_APImm_07",(1<<0), 4, 0xa000,	0x0b00,	0x00c0,	0x000d);
    compFm("XD_ALU_APImm_07",(1<<1), 4, 0xa000,	0x0b00,	0x00c0,	0x000d);

    MEAS_MPAT("XD_ALU_AMImm_00");
    compFm("XD_ALU_AMImm_00",(1<<0), 4, 0xefff,	0xeffe,	0xeffc,	0xeff8);
    compFm("XD_ALU_AMImm_00",(1<<1), 4, 0x7fff,	0x7ffe,	0x7FFC,	0x7ff8);			

    MEAS_MPAT("XD_ALU_AMImm_01");
    compFm("XD_ALU_AMImm_01",(1<<0), 4, 0xeff0,	0xefe0,	0xefc0,	0xef80);
    compFm("XD_ALU_AMImm_01",(1<<1), 4, 0x7ff0,	0x7fe0,	0x7fc0,	0x7f80); 	

    MEAS_MPAT("XD_ALU_AMImm_02");
    compFm("XD_ALU_AMImm_02",(1<<0), 4, 0xef00,	0xee00,	0xec00,	0xe800);
    compFm("XD_ALU_AMImm_02",(1<<1), 4, 0x7f00,	0x7e00,	0x7c00,	0x7800);	

    MEAS_MPAT("XD_ALU_AMImm_03");
    compFm("XD_ALU_AMImm_03",(1<<0), 4, 0xe000,	0xee00,	0xefc0,	0xeff8);
    compFm("XD_ALU_AMImm_03",(1<<1), 4, 0x7000,	0x7e00,	0x7fc0,	0x7ff8);

    MEAS_MPAT("XD_ALU_AMImm_04");
    compFm("XD_ALU_AMImm_04",(1<<0), 4, 0xeff6,	0xefeb,	0xefdf,	0xefd2);
    compFm("XD_ALU_AMImm_04",(1<<1), 4, 0x7ff6,	0x7feb,	0x7fdf,	0x7fd2);	

    MEAS_MPAT("XD_ALU_AMImm_05");
    compFm("XD_ALU_AMImm_05",(1<<0), 4, 0xef60,	0xef50,	0xef40,	0xef30);
    compFm("XD_ALU_AMImm_05",(1<<1), 4, 0x7f60,	0x7f50,	0x7f40,	0x7f30);	

    MEAS_MPAT("XD_ALU_AMImm_06");
    compFm("XD_ALU_AMImm_06",(1<<0), 4, 0xe600,	0xe500,	0xe400,	0xe300);
    compFm("XD_ALU_AMImm_06",(1<<1), 4, 0x7600,	0x7500,	0x7400,	0x7300);		

    MEAS_MPAT("XD_ALU_AMImm_07");
    compFm("XD_ALU_AMImm_07",(1<<0), 4, 0x5000,	0xe500,	0xef40,	0xeff3);
    compFm("XD_ALU_AMImm_07",(1<<1), 4, 0x4000,	0xd500,	0xdf40,	0xdff3);
}

void testPG_alu_xaya()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;
	
	load_pds("diag_pds_data_xy16");
    SEND_MPAT("diag_xaya.mpa");

    MEAS_MPAT("XA_ALU_A_00");
    compFm("XA_ALU_A_00",(1<<0), 16, 0x0000, 0x1000, 0x2000, 0x2000, 0x4000, 0x5000, 0x6000, 0x7000, 0x8000, 0x9000, 0xA000, 0xA000, 0xC000, 0xD000, 0xE000, 0xF000);
    compFm("XA_ALU_A_00",(1<<1), 16, 0x8000, 0x9000, 0xA000, 0xA000, 0xC000, 0xD000, 0xE000, 0xF000, 0x0000, 0x1000, 0x2000, 0x2000, 0x4000, 0x5000, 0x6000, 0x7000);	
    
    MEAS_MPAT("XA_ALU_NA_00");
    compFm("XA_ALU_NA_00",(1<<0), 16, 0xFFFF, 0xFFFE, 0xFFFD, 0x0002, 0xFFF7, 0xFFEF, 0xFFDF, 0xFFBF, 0xFFFF, 0xFFF5, 0xFFF4, 0x000B, 0xFFF2, 0xFF5F, 0xFF4F, 0xFF3F);
    compFm("XA_ALU_NA_00",(1<<1), 16, 0xFFFF, 0xFFF5, 0xFFF4, 0x000B, 0xFFF2, 0xFF5F, 0xFF4F, 0xFF3F, 0xFFFF, 0xFFFE, 0xFFFD, 0x0002, 0xFFF7, 0xFFEF, 0xFFDF, 0xFFBF);

    MEAS_MPAT("XA_ALU_AP1_00");
    compFm("XA_ALU_AP1_00",(1<<0), 16, 0x0001, 0x0002, 0x0003, 0x0004, 0x0009, 0x0011, 0x0021, 0x0041, 0xFFFF, 0x8000, 0x4000, 0x4001, 0x1000, 0x0800, 0x0400, 0x0200);
    compFm("XA_ALU_AP1_00",(1<<1), 16, 0xFFFF, 0x8000, 0x4000, 0x4001, 0x1000, 0x0800, 0x0400, 0x0200, 0x0001, 0x0002, 0x0003, 0x0004, 0x0009, 0x0011, 0x0021, 0x0041);
    
    MEAS_MPAT("XA_ALU_AM1_00");
    compFm("XA_ALU_AM1_00",(1<<0), 16, 0x0000, 0x0001, 0x0003, 0x0002, 0x000F, 0x001F, 0x003F, 0x007F, 0xFFFE, 0x7FFE, 0x3FFE, 0x3FFD, 0x0FFE, 0x07FE, 0x03FE, 0x01FE);
    compFm("XA_ALU_AM1_00",(1<<1), 16, 0xFFFE, 0x7FFE, 0x3FFE, 0x3FFD, 0x0FFE, 0x07FE, 0x03FE, 0x01FE, 0x0000, 0x0001, 0x0003, 0x0002, 0x000F, 0x001F, 0x003F, 0x007F);
        
    MEAS_MPAT("XA_ALU_AM2_00");
    compFm("XA_ALU_AM2_00",(1<<0), 16, 0x0000, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0000, 0x0014, 0x0016, 0x002C, 0x001A, 0x0140, 0x0160, 0x0180);
    compFm("XA_ALU_AM2_00",(1<<1), 16, 0x0000, 0x0014, 0x0016, 0x002C, 0x001A, 0x0140, 0x0160, 0x0180, 0x0000, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080);
    
    MEAS_MPAT("XA_ALU_AD2_00");
    compFm("XA_ALU_AD2_00",(1<<0), 16, 0x0000, 0x0000, 0x0001, 0x0000, 0x0004, 0x0008, 0x0010, 0x0020, 0x0000, 0x0005, 0x0050, 0x0028, 0x5000, 0x5555, 0x0555, 0x0055);
    compFm("XA_ALU_AD2_00",(1<<1), 16, 0x0000, 0x0005, 0x0050, 0x0028, 0x5000, 0x5555, 0x0555, 0x0055, 0x0000, 0x0000, 0x0001, 0x0000, 0x0004, 0x0008, 0x0010, 0x0020);

    MEAS_MPAT("XA_ALU_B_00");   
    compFm("XA_ALU_B_00",(1<<0), 16, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0xFFFF, 0x0000);
    compFm("XA_ALU_B_00",(1<<1), 16, 0x0080, 0x0100, 0x0200, 0x0400, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x1000, 0x0800, 0xFFFF, 0x0080);
    
    MEAS_MPAT("XA_ALU_APB_00");
    compFm("XA_ALU_APB_00",(1<<0), 16, 0x0001, 0x0010, 0x0100, 0x0001, 0x0010, 0x0100, 0x0001, 0x0010, 0x0100, 0x0001, 0x0010, 0x0100, 0x0010, 0x0001, 0x0001, 0x0010);
    compFm("XA_ALU_APB_00",(1<<1), 16, 0x8100, 0x8001, 0x8010, 0x8100, 0x8001, 0x8010, 0x8100, 0x8001, 0x8010, 0x8100, 0x8001, 0x8010, 0x8001, 0x8010, 0x8100, 0x8001);
    
    MEAS_MPAT("XA_ALU_APB_01");
    compFm("XA_ALU_APB_01",(1<<0),16, 0x1001, 0x1010, 0x1100, 0x1001, 0x1010, 0x1100, 0x1001, 0x1010, 0x1100, 0x1001, 0x1010, 0x1100, 0x1010, 0x1001, 0x1001, 0x1010);
    compFm("XA_ALU_APB_01",(1<<1),16, 0x9100, 0x9001, 0x9010, 0x9100, 0x9001, 0x9010, 0x9100, 0x9001, 0x9010, 0x9100, 0x9001, 0x9010, 0x9001, 0x9010, 0x9100, 0x9001);

    MEAS_MPAT("XA_ALU_APB_02");
    compFm("XA_ALU_APB_02",(1<<0),16, 0x2001, 0x2010, 0x2100, 0x2001, 0x2010, 0x2100, 0x2001, 0x2010, 0x2100, 0x2001, 0x2010, 0x2100, 0x2010, 0x2001, 0x2001, 0x2010);
    compFm("XA_ALU_APB_02",(1<<1),16, 0xA100, 0xA001, 0xA010, 0xA100, 0xA001, 0xA010, 0xA100, 0xA001, 0xA010, 0xA100, 0xA001, 0xA010, 0xA001, 0xA010, 0xA100, 0xA001);

    MEAS_MPAT("XA_ALU_APB_03");
    compFm("XA_ALU_APB_03",(1<<0),16, 0x3001, 0x3011, 0x3111, 0x3112, 0x3122, 0x3222, 0x3223, 0x3233, 0x3333, 0x3334, 0x3344, 0x3444, 0x3454, 0x3455, 0x3456, 0x3466);
    compFm("XA_ALU_APB_03",(1<<1),16, 0xB100, 0xB101, 0xB111, 0xB211, 0xB212, 0xB222, 0xB322, 0xB323, 0xB333, 0xB433, 0xB434, 0xB444, 0xB445, 0xB455, 0xB555, 0xB556);

    MEAS_MPAT("XA_ALU_APB_04");
    compFm("XA_ALU_APB_04",(1<<0),16, 0x4001, 0x4010, 0x4100, 0x4001, 0x4010, 0x4100, 0x4001, 0x4010, 0x4100, 0x4001, 0x4010, 0x4100, 0x4010, 0x4001, 0x4001, 0x4010);
    compFm("XA_ALU_APB_04",(1<<1),16, 0xC100, 0xC001, 0xC010, 0xC100, 0xC001, 0xC010, 0xC100, 0xC001, 0xC010, 0xC100, 0xC001, 0xC010, 0xC001, 0xC010, 0xC100, 0xC001);

    MEAS_MPAT("XA_ALU_APB_05");
    compFm("XA_ALU_APB_05",(1<<0),16, 0x5001, 0x5010, 0x5100, 0x5001, 0x5010, 0x5100, 0x5001, 0x5010, 0x5100, 0x5001, 0x5010, 0x5100, 0x5010, 0x5001, 0x5001, 0x5010);
    compFm("XA_ALU_APB_05",(1<<1),16, 0xD100, 0xD001, 0xD010, 0xD100, 0xD001, 0xD010, 0xD100, 0xD001, 0xD010, 0xD100, 0xD001, 0xD010, 0xD001, 0xD010, 0xD100, 0xD001);

    MEAS_MPAT("XA_ALU_APB_06");
    compFm("XA_ALU_APB_06",(1<<0),16, 0x6001, 0x6010, 0x6100, 0x6001, 0x6010, 0x6100, 0x6001, 0x6010, 0x6100, 0x6001, 0x6010, 0x6100, 0x6010, 0x6001, 0x6001, 0x6010);
    compFm("XA_ALU_APB_06",(1<<1),16, 0xE100, 0xE001, 0xE010, 0xE100, 0xE001, 0xE010, 0xE100, 0xE001, 0xE010, 0xE100, 0xE001, 0xE010, 0xE001, 0xE010, 0xE100, 0xE001);

	MEAS_MPAT("XA_ALU_APB_07");
    compFm("XA_ALU_APB_07",(1<<0),16, 0x7001, 0x7010, 0x7100, 0x7001, 0x7010, 0x7100, 0x7001, 0x7010, 0x7100, 0x7001, 0x7010, 0x7100, 0x7010, 0x7001, 0x7001, 0x7010);
    compFm("XA_ALU_APB_07",(1<<1),16, 0xF100, 0xF001, 0xF010, 0xF100, 0xF001, 0xF010, 0xF100, 0xF001, 0xF010, 0xF100, 0xF001, 0xF010, 0xF001, 0xF010, 0xF100, 0xF001); 
    
    MEAS_MPAT("XA_ALU_AMB_00");
    compFm("XA_ALU_AMB_00",(1<<0),16 ,0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0656, 0x0665, 0x0665, 0x0656);
    compFm("XA_ALU_AMB_00",(1<<1),16 ,0x0554, 0x0545, 0x0455, 0x0554, 0x0545, 0x0455, 0x0554, 0x0545, 0x0455, 0x0554, 0x0545, 0x0455, 0x0545, 0x0554, 0x0554, 0x0545);
    
    MEAS_MPAT("XA_ALU_AMB_01");
    compFm("XA_ALU_AMB_01",(1<<0),16 ,0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0767, 0x0776, 0x0776, 0x0767);
    compFm("XA_ALU_AMB_01",(1<<1),16 ,0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0656, 0x0665, 0x0665, 0x0656);
    
    MEAS_MPAT("XA_ALU_AMB_02");
    compFm("XA_ALU_AMB_02",(1<<0),16 ,0x0887, 0x0878, 0x0788, 0x0887, 0x0878, 0x0788, 0x0887, 0x0878, 0x0788, 0x0887, 0x0878, 0x0788, 0x0878, 0x0887, 0x0887, 0x0878);
    compFm("XA_ALU_AMB_02",(1<<1),16 ,0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0767, 0x0776, 0x0776, 0x0767);
    
    MEAS_MPAT("XA_ALU_AMB_03");
    compFm("XA_ALU_AMB_03",(1<<0),16 ,0x0998, 0x0988, 0x0888, 0x0887, 0x0877, 0x0777, 0x0776, 0x0766, 0x0666, 0x0665, 0x0655, 0x0555, 0x0545, 0x0544, 0x0543, 0x0533);
    compFm("XA_ALU_AMB_03",(1<<1),16 ,0x0887, 0x0877, 0x0777, 0x0776, 0x0766, 0x0666, 0x0665, 0x0655, 0x0555, 0x0554, 0x0544, 0x0444, 0x0434, 0x0433, 0x0432, 0x0422);
    
    MEAS_MPAT("XA_ALU_AMB_04");
    compFm("XA_ALU_AMB_04",(1<<0),16 ,0x0AA9, 0x0A9A, 0x09AA, 0x0AA9, 0x0A9A, 0x09AA, 0x0AA9, 0x0A9A, 0x09AA, 0x0AA9, 0x0A9A, 0x09AA, 0x0A9A, 0x0AA9, 0x0AA9, 0x0A9A);
    compFm("XA_ALU_AMB_04",(1<<1),16 ,0x0998, 0x0989, 0x0899, 0x0998, 0x0989, 0x0899, 0x0998, 0x0989, 0x0899, 0x0998, 0x0989, 0x0899, 0x0989, 0x0998, 0x0998, 0x0989);
    
    MEAS_MPAT("XA_ALU_AMB_05");
    compFm("XA_ALU_AMB_05",(1<<0),16 ,0x0BBA, 0x0BAB, 0x0ABB, 0x0BBA, 0x0BAB, 0x0ABB, 0x0BBA, 0x0BAB, 0x0ABB, 0x0BBA, 0x0BAB, 0x0ABB, 0x0BAB, 0x0BBA, 0x0BBA, 0x0BAB);
    compFm("XA_ALU_AMB_05",(1<<1),16 ,0x0AA9, 0x0A9A, 0x09AA, 0x0AA9, 0x0A9A, 0x09AA, 0x0AA9, 0x0A9A, 0x09AA, 0x0AA9, 0x0A9A, 0x09AA, 0x0A9A, 0x0AA9, 0x0AA9, 0x0A9A);
    
    MEAS_MPAT("XA_ALU_AMB_06");
    compFm("XA_ALU_AMB_06",(1<<0),16 ,0x0CCB, 0x0CBC, 0x0BCC, 0x0CCB, 0x0CBC, 0x0BCC, 0x0CCB, 0x0CBC, 0x0BCC, 0x0CCB, 0x0CBC, 0x0BCC, 0x0CBC, 0x0CCB, 0x0CCB, 0x0CBC);
    compFm("XA_ALU_AMB_06",(1<<1),16 ,0x0BBA, 0x0BAB, 0x0ABB, 0x0BBA, 0x0BAB, 0x0ABB, 0x0BBA, 0x0BAB, 0x0ABB, 0x0BBA, 0x0BAB, 0x0ABB, 0x0BAB, 0x0BBA, 0x0BBA, 0x0BAB);
    
    MEAS_MPAT("XA_ALU_AMB_07");
    compFm("XA_ALU_AMB_07",(1<<0),16 ,0x0DDC, 0x0DCD, 0x0CDD, 0x0DDC, 0x0DCD, 0x0CDD, 0x0DDC, 0x0DCD, 0x0CDD, 0x0DDC, 0x0DCD, 0x0CDD, 0x0DCD, 0x0DDC, 0x0DDC, 0x0DCD);
    compFm("XA_ALU_AMB_07",(1<<1),16 ,0x0CCB, 0x0CBC, 0x0BCC, 0x0CCB, 0x0CBC, 0x0BCC, 0x0CCB, 0x0CBC, 0x0BCC, 0x0CCB, 0x0CBC, 0x0BCC, 0x0CBC, 0x0CCB, 0x0CCB, 0x0CBC);
    
    MEAS_MPAT("XA_ALU_AANDB_00");
    compFm("XA_ALU_AANDB_00",(1<<0), 14,0xFFFE, 0xFFFD, 0xFFFB, 0xFFF7, 0xFFEF, 0xFFDF, 0xFFBF, 0xFF7F, 0xFEFF, 0xFDFF, 0xFBFF, 0xF7FF, 0xEFFF, 0xDFFF);
    compFm("XA_ALU_AANDB_00",(1<<1), 14,0xEFFE, 0xEFFD, 0xEFFB, 0xEFF7, 0xEFEF, 0xEFDF, 0xEFBF, 0xEF7F, 0xEEFF, 0xEDFF, 0xEBFF, 0xE7FF, 0xEFFF, 0xCFFF);
    
    MEAS_MPAT("XA_ALU_AANDB_01");
    compFm("XA_ALU_AANDB_01",(1<<0), 14,0xfffe,	0xfffd,	0xfffb,	0xfff7,	0xffef,	0xffdf,	0xffbf,	0xff7f,	0xfeff,	0xfdff,	0xfbff,	0xf7ff,	0xefff,	0xdfff);
    compFm("XA_ALU_AANDB_01",(1<<1), 14,0xeffe,	0xeffd,	0xeffb,	0xeff7,	0xefef,	0xefdf,	0xefbf,	0xef7f,	0xeeff,	0xedff,	0xebff,	0xe7ff,	0xefff,	0xcfff);
    
    MEAS_MPAT("XA_ALU_AANDB_02");
    compFm("XA_ALU_AANDB_02",(1<<0), 14,0xfffe,	0xfffd,	0xfffb,	0xfff7,	0xffef,	0xffdf,	0xffbf,	0xff7f,	0xfeff,	0xfdff,	0xfbff,	0xf7ff,	0xefff,	0xdfff);
    compFm("XA_ALU_AANDB_02",(1<<1), 14,0xeffe,	0xeffd,	0xeffb,	0xeff7,	0xefef,	0xefdf,	0xefbf,	0xef7f,	0xeeff,	0xedff,	0xebff,	0xe7ff,	0xefff,	0xcfff);

    MEAS_MPAT("XA_ALU_AANDB_03");
    compFm("XA_ALU_AANDB_03",(1<<0), 14,0xdffe, 0xdffc, 0xdff8, 0xdff0, 0xdfe0, 0xdfc0, 0xdf80, 0xdf00, 0xde00, 0xdc00, 0xd800, 0xd000, 0xc000, 0xc000);
    compFm("XA_ALU_AANDB_03",(1<<1), 14,0xcffe, 0xcffc, 0xcff8, 0xcff0, 0xcfe0, 0xcfc0, 0xcf80, 0xcf00, 0xce00, 0xcc00, 0xc800, 0xc000, 0xc000, 0xc000);
    
    MEAS_MPAT("XA_ALU_AANDB_04");
    compFm("XA_ALU_AANDB_04",(1<<0), 14,0xcffe,	0xcffd,	0xcffb,	0xcff7,	0xcfef,	0xcfdf,	0xcfbf,	0xcf7f,	0xceff,	0xcdff,	0xcbff,	0xc7ff,	0xcfff,	0xcfff);
    compFm("XA_ALU_AANDB_04",(1<<1), 14,0xbffe,	0xbffd,	0xbffb,	0xbff7,	0xbfef,	0xbfdf,	0xbfbf,	0xbf7f,	0xbeff,	0xbdff,	0xbbff,	0xb7ff,	0xafff,	0x9fff);
    
    MEAS_MPAT("XA_ALU_AANDB_05");
    compFm("XA_ALU_AANDB_05",(1<<0), 14,0xbffe,	0xbffd,	0xbffb,	0xbff7,	0xbfef,	0xbfdf,	0xbfbf,	0xbf7f,	0xbeff,	0xbdff,	0xbbff,	0xb7ff,	0xafff,	0x9fff, 0xbffe,	0xbffd);
    compFm("XA_ALU_AANDB_05",(1<<1), 14,0xaffe,	0xaffd,	0xaffb,	0xaff7,	0xafef,	0xafdf,	0xafbf,	0xaf7f,	0xaeff,	0xadff,	0xabff,	0xa7ff,	0xafff,	0x8fff, 0xaffe,	0xaffd);
    
    MEAS_MPAT("XA_ALU_AANDB_06");
    compFm("XA_ALU_AANDB_06",(1<<0), 14,0xaffe,	0xaffd,	0xaffb,	0xaff7,	0xafef,	0xafdf,	0xafbf,	0xaf7f,	0xaeff,	0xadff,	0xabff,	0xa7ff,	0xafff,	0x8fff, 0xaffe,	0xaffd);
    compFm("XA_ALU_AANDB_06",(1<<1), 14,0x9ffe,	0x9ffd,	0x9ffb,	0x9ff7,	0x9fef,	0x9fdf,	0x9fbf,	0x9f7f,	0x9eff,	0x9dff,	0x9bff,	0x97ff,	0x8fff,	0x9fff, 0x9ffe,	0x9ffd);
    
    MEAS_MPAT("XA_ALU_AANDB_07");
    compFm("XA_ALU_AANDB_07",(1<<0), 14,0x9ffe,	0x9ffd,	0x9ffb,	0x9ff7,	0x9fef,	0x9fdf,	0x9fbf,	0x9f7f,	0x9eff,	0x9dff,	0x9bff,	0x97ff,	0x8fff,	0x9fff, 0x9ffe,	0x9ffd);
    compFm("XA_ALU_AANDB_07",(1<<1), 14,0x8ffe,	0x8ffd,	0x8ffb,	0x8ff7,	0x8fef,	0x8fdf,	0x8fbf,	0x8f7f,	0x8eff,	0x8dff,	0x8bff,	0x87ff,	0x8fff,	0x8fff, 0x8ffe,	0x8ffd);
    
}

void testPG_alu_xaya_2()
{
    diag_log("============ %s ============\n", __FUNCTION__);
    int fm_cnt;

    load_pds("diag_pds_data_xy16");

    SEND_MPAT("diag_xaya_2.mpa");

    MEAS_MPAT("XA_ALU_AORB_00");
    compFm("XA_ALU_AORB_00",(1<<0), 14, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000);
    compFm("XA_ALU_AORB_00",(1<<1), 14, 0x8000, 0x8001, 0x8002, 0x8004, 0x8008, 0x8010, 0x8020, 0x8040, 0x8080, 0x8100, 0x8200, 0x8400, 0x8800, 0x9000);

    MEAS_MPAT("XA_ALU_AORB_01");
    compFm("XA_ALU_AORB_01",(1<<0), 14, 0x1000,	0x1001,	0x1002,	0x1004,	0x1008,	0x1010,	0x1020,	0x1040,	0x1080,	0x1100,	0x1200,	0x1400,	0x1800,	0x1000);
    compFm("XA_ALU_AORB_01",(1<<1), 14, 0x9000,	0x9001,	0x9002,	0x9004,	0x9008,	0x9010,	0x9020,	0x9040,	0x9080,	0x9100,	0x9200,	0x9400,	0x9800,	0x9000);

    MEAS_MPAT("XA_ALU_AORB_02");
    compFm("XA_ALU_AORB_02",(1<<0), 14, 0x2000,	0x2001,	0x2002,	0x2004,	0x2008,	0x2010,	0x2020,	0x2040,	0x2080,	0x2100,	0x2200,	0x2400,	0x2800,	0x3000);
    compFm("XA_ALU_AORB_02",(1<<1), 14, 0xa000,	0xa001,	0xa002,	0xa004,	0xa008,	0xa010,	0xa020,	0xa040,	0xa080,	0xa100,	0xa200,	0xa400,	0xa800,	0xb000);

    MEAS_MPAT("XA_ALU_AORB_03");
    compFm("XA_ALU_AORB_03",(1<<0), 14, 0x3000, 0x3001, 0x3003, 0x3007, 0x300F, 0x301F, 0x303F, 0x307F, 0x30FF, 0x31FF, 0x33FF, 0x37FF, 0x3FFF, 0x3FFF);
    compFm("XA_ALU_AORB_03",(1<<1), 14, 0xB000, 0xB001, 0xB003, 0xB007, 0xB00F, 0xB01F, 0xB03F, 0xB07F, 0xB0FF, 0xB1FF, 0xB3FF, 0xB7FF, 0xBFFF, 0xBFFF);

    MEAS_MPAT("XA_ALU_AORB_04");
    compFm("XA_ALU_AORB_04",(1<<0), 14, 0x4000,	0x4001,	0x4002,	0x4004,	0x4008,	0x4010,	0x4020,	0x4040,	0x4080,	0x4100,	0x4200,	0x4400,	0x4800,	0x5000);
    compFm("XA_ALU_AORB_04",(1<<1), 14, 0xc000,	0xc001,	0xc002,	0xc004,	0xc008,	0xc010,	0xc020,	0xc040,	0xc080,	0xc100,	0xc200,	0xc400,	0xc800,	0xd000); 

    MEAS_MPAT("XA_ALU_AORB_05");
    compFm("XA_ALU_AORB_05",(1<<0), 14, 0x5000,	0x5001,	0x5002,	0x5004,	0x5008,	0x5010,	0x5020,	0x5040,	0x5080,	0x5100,	0x5200,	0x5400,	0x5800,	0x5000);
    compFm("XA_ALU_AORB_05",(1<<1), 14, 0xd000,	0xd001,	0xd002,	0xd004,	0xd008,	0xd010,	0xd020,	0xd040,	0xd080,	0xd100,	0xd200,	0xd400,	0xd800,	0xd000);

    MEAS_MPAT("XA_ALU_AORB_06");
    compFm("XA_ALU_AORB_06",(1<<0), 14, 0x6000,	0x6001,	0x6002,	0x6004,	0x6008,	0x6010,	0x6020,	0x6040,	0x6080,	0x6100,	0x6200,	0x6400,	0x6800,	0x7000);
    compFm("XA_ALU_AORB_06",(1<<1), 14, 0xe000,	0xe001,	0xe002,	0xe004,	0xe008,	0xe010,	0xe020,	0xe040,	0xe080,	0xe100,	0xe200,	0xe400,	0xe800,	0xf000);

    MEAS_MPAT("XA_ALU_AORB_07");
    compFm("XA_ALU_AORB_07",(1<<0), 14, 0x7000,	0x7001,	0x7002,	0x7004,	0x7008,	0x7010,	0x7020,	0x7040,	0x7080,	0x7100,	0x7200,	0x7400,	0x7800,	0x7000);
    compFm("XA_ALU_AORB_07",(1<<1), 14, 0xf000,	0xf001,	0xf002,	0xf004,	0xf008,	0xf010,	0xf020,	0xf040,	0xf080,	0xf100,	0xf200,	0xf400,	0xf800,	0xf000);

    MEAS_MPAT("XA_ALU_AXORB_00");
    compFm("XA_ALU_A^B_00",(1<<0), 14, 0xFFFF, 0xFFFE, 0xFFFD, 0xFFFB, 0xFFF7, 0xFFEF, 0xFFDF, 0xFFBF, 0xFF7F, 0xFEFF, 0xFDFF, 0xFBFF, 0xF7FF, 0xEFFF);
    compFm("XA_ALU_A^B_00",(1<<1), 14, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000);

    MEAS_MPAT("XA_ALU_AXORB_01");
    compFm("XA_ALU_A^B_01",(1<<0), 14, 0x1000, 0x1001, 0x1002, 0x1004, 0x1008, 0x1010, 0x1020,0x1040,	0x1080,	0x1100,	0x1200,	0x1400,	0x1800,	0x0000);
    compFm("XA_ALU_A^B_01",(1<<1), 14, 0x9000, 0x9001, 0x9002, 0x9004, 0x9008, 0x9010, 0x9020,0x9040,	0x9080,	0x9100,	0x9200,	0x9400,	0x9800,	0x8000);

    MEAS_MPAT("XA_ALU_AXORB_02");
    compFm("XA_ALU_A^B_02",(1<<0), 14, 0x2000,	0x2001,	0x2002,	0x2004,	0x2008,	0x2010,	0x2020,	0x2040,	0x2080,	0x2100,	0x2200,	0x2400,	0x2800,	0x3000);
    compFm("XA_ALU_A^B_02",(1<<1), 14, 0xa000,	0xa001,	0xa002,	0xa004,	0xa008,	0xa010,	0xa020,	0xa040,	0xa080,	0xa100,	0xa200,	0xa400,	0xa800,	0xb000);	

    MEAS_MPAT("XA_ALU_AXORB_03");
    compFm("XA_ALU_A^B_03",(1<<0), 14, 0x3000, 0x3001, 0x3003, 0x3007, 0x300F, 0x301F, 0x303F, 0x307F, 0x30FF, 0x31FF, 0x33FF, 0x37FF, 0x3FFF, 0x2FFF);
    compFm("XA_ALU_A^B_03",(1<<1), 14, 0xB000, 0xB001, 0xB003, 0xB007, 0xB00F, 0xB01F, 0xB03F, 0xB07F, 0xB0FF, 0xB1FF, 0xB3FF, 0xB7FF, 0xBFFF, 0xAFFF);	

    MEAS_MPAT("XA_ALU_AXORB_04");
    compFm("XA_ALU_A^B_04",(1<<0), 14, 0x4000,	0x4001,	0x4002,	0x4004,	0x4008,	0x4010,	0x4020,	0x4040,	0x4080,	0x4100,	0x4200,	0x4400,	0x4800,	0x5000);
    compFm("XA_ALU_A^B_04",(1<<1), 14, 0xc000,	0xc001,	0xc002,	0xc004,	0xc008,	0xc010,	0xc020,	0xc040,	0xc080,	0xc100,	0xc200,	0xc400,	0xc800,	0xd000);	

    MEAS_MPAT("XA_ALU_AXORB_05");
    compFm("XA_ALU_A^B_05",(1<<0), 14, 0x5000,	0x5001,	0x5002,	0x5004,	0x5008,	0x5010,	0x5020,	0x5040,	0x5080,	0x5100,	0x5200,	0x5400,	0x5800,	0x4000);
    compFm("XA_ALU_A^B_05",(1<<1), 14, 0xd000,	0xd001,	0xd002,	0xd004,	0xd008,	0xd010,	0xd020,	0xd040,	0xd080,	0xd100,	0xd200,	0xd400,	0xd800,	0xc000);	

    MEAS_MPAT("XA_ALU_AXORB_06");
    compFm("XA_ALU_A^B_06",(1<<0), 14, 0x6000,	0x6001,	0x6002,	0x6004,	0x6008,	0x6010,	0x6020,	0x6040,	0x6080,	0x6100,	0x6200,	0x6400,	0x6800,	0x7000);
    compFm("XA_ALU_A^B_06",(1<<1), 14, 0xe000,	0xe001,	0xe002,	0xe004,	0xe008,	0xe010,	0xe020,	0xe040,	0xe080,	0xe100,	0xe200,	0xe400,	0xe800,	0xf000);	

    MEAS_MPAT("XA_ALU_AXORB_07");
    compFm("XA_ALU_A^B_07",(1<<0), 14, 0x7000,	0x7001,	0x7002,	0x7004,	0x7008,	0x7010,	0x7020,	0x7040,	0x7080,	0x7100,	0x7200,	0x7400,	0x7800,	0x6000);
    compFm("XA_ALU_A^B_07",(1<<1), 14, 0xf000,	0xf001,	0xf002,	0xf004,	0xf008,	0xf010,	0xf020,	0xf040,	0xf080,	0xf100,	0xf200,	0xf400,	0xf800,	0xe000);	
    
    MEAS_MPAT("XA_ALU_D3D4_00");
    compFm("XA_ALU_D3D4_00",(1<<0),14 , 0x1111, 0x1111, 0x0000, 0x0001, 0x0002, 0x0001, 0x0002, 0x0004, 0x0008, 0x0004, 0x0008, 0x0888, 0x0888, 0x0888);
    compFm("XA_ALU_D3D4_00",(1<<1),14 , 0xAAAA, 0xAAAA, 0xAAAB, 0x0000, 0x0001, 0x0002, 0x0001, 0x0002, 0x0001, 0x0002, 0x0004, 0x5555, 0x5555, 0x5555);

    MEAS_MPAT("XA_ALU_APImm_00");
    compFm("XA_ALU_APImm_00",(1<<0), 4, 0x0001,	0x0002,	0x0004,	0x0008);
    compFm("XA_ALU_APImm_00",(1<<1), 4, 0x8001,	0x8002,	0x8004,	0x8008);	

    MEAS_MPAT("XA_ALU_APImm_01");
    compFm("XA_ALU_APImm_01",(1<<0), 4, 0x0010,	0x0020,	0x0040,	0x0080);
    compFm("XA_ALU_APImm_01",(1<<1), 4, 0x8010,	0x8020,	0x8040,	0x8080); 

    MEAS_MPAT("XA_ALU_APImm_02");
    compFm("XA_ALU_APImm_02",(1<<0), 4, 0x0100,	0x0200,	0x0400,	0x0800);
    compFm("XA_ALU_APImm_02",(1<<1), 4, 0x8100,	0x8200,	0x8400,	0x8800);	

    MEAS_MPAT("XA_ALU_APImm_03");	
    compFm("XA_ALU_APImm_03",(1<<0), 4, 0x1000,	0x3000,	0x6000,	0xA000);
    compFm("XA_ALU_APImm_03",(1<<1), 4, 0x1001,	0x3001,	0x6001,	0xA001);

    MEAS_MPAT("XA_ALU_APImm_04");
    compFm("XA_ALU_APImm_04",(1<<0), 4, 0x000a,	0x000b,	0x000c,	0x000d);
    compFm("XA_ALU_APImm_04",(1<<1), 4, 0x800a,	0x800b,	0x800c,	0x800d);	

    MEAS_MPAT("XA_ALU_APImm_05");
    compFm("XA_ALU_APImm_05",(1<<0), 4, 0x00a0,	0x00b0,	0x00c0,	0x00d0);
    compFm("XA_ALU_APImm_05",(1<<1), 4, 0x80a0,	0x80b0,	0x80c0,	0x80d0);	

    MEAS_MPAT("XA_ALU_APImm_06");
    compFm("XA_ALU_APImm_06",(1<<0), 4, 0x0a00,	0x0b00,	0x0c00,	0x0d00);
    compFm("XA_ALU_APImm_06",(1<<1), 4, 0x0a01,	0x0b01,	0x0c01,	0x0d01);	

    MEAS_MPAT("XA_ALU_APImm_07");
    compFm("XA_ALU_APImm_07",(1<<0), 4, 0xa000,	0x0b00,	0x00c0,	0x000d);
    compFm("XA_ALU_APImm_07",(1<<1), 4, 0xa000,	0x0b00,	0x00c0,	0x000d);

    MEAS_MPAT("XA_ALU_AMImm_00");
    compFm("XA_ALU_AMImm_00",(1<<0), 4, 0xefff,	0xeffe,	0xeffc,	0xeff8);
    compFm("XA_ALU_AMImm_00",(1<<1), 4, 0x7fff,	0x7ffe,	0x7FFC,	0x7ff8);			

    MEAS_MPAT("XA_ALU_AMImm_01");
    compFm("XA_ALU_AMImm_01",(1<<0), 4, 0xeff0,	0xefe0,	0xefc0,	0xef80);
    compFm("XA_ALU_AMImm_01",(1<<1), 4, 0x7ff0,	0x7fe0,	0x7fc0,	0x7f80); 	

    MEAS_MPAT("XA_ALU_AMImm_02");
    compFm("XA_ALU_AMImm_02",(1<<0), 4, 0xef00,	0xee00,	0xec00,	0xe800);
    compFm("XA_ALU_AMImm_02",(1<<1), 4, 0x7f00,	0x7e00,	0x7c00,	0x7800);	

    MEAS_MPAT("XA_ALU_AMImm_03");
    compFm("XA_ALU_AMImm_03",(1<<0), 4, 0xe000,	0xde00,	0xddc0,	0xddb8);
    compFm("XA_ALU_AMImm_03",(1<<1), 4, 0x7000,	0x6e00,	0x6dc0,	0x6db8);

    MEAS_MPAT("XA_ALU_AMImm_04");
    compFm("XA_ALU_AMImm_04",(1<<0), 4, 0xeff6,	0xeff5,	0xeff4,	0xeff3);
    compFm("XA_ALU_AMImm_04",(1<<1), 4, 0x7ff6,	0x7ff5,	0x7ff4,	0x7ff3);	

    MEAS_MPAT("XA_ALU_AMImm_05");
    compFm("XA_ALU_AMImm_05",(1<<0), 4, 0xef60,	0xef50,	0xef40,	0xef30);
    compFm("XA_ALU_AMImm_05",(1<<1), 4, 0x7f60,	0x7f50,	0x7f40,	0x7f30);	

    MEAS_MPAT("XA_ALU_AMImm_06");
    compFm("XA_ALU_AMImm_06",(1<<0), 4, 0xe600,	0xe500,	0xe400,	0xe300);
    compFm("XA_ALU_AMImm_06",(1<<1), 4, 0x7600,	0x7500,	0x7400,	0x7300);		

    MEAS_MPAT("XA_ALU_AMImm_07");
    compFm("XA_ALU_AMImm_07",(1<<0), 4, 0x5000,	0xe500,	0xef40,	0xeff3);
    compFm("XA_ALU_AMImm_07",(1<<1), 4, 0x4000,	0xd500,	0xdf40,	0xdff3);

}

void testPG_alu_xsys()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	load_pds("diag_pds_data_xy16");
    SEND_MPAT("diag_xsys.mpa");

    MEAS_MPAT("XS_ALU_A_00");
    int fm_size=diag_readFmData();
    compFm("XS_ALU_A_00",(1<<0), 8, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040);
    compFm("XS_ALU_A_00",(1<<1), 8, 0xffff, 0x7fff, 0x3fff, 0x1fff, 0x0fff, 0x07ff, 0x03ff, 0x01ff);

    MEAS_MPAT("XS_ALU_NA_00");
    compFm("XS_ALU_NA_00",(1<<0), 8, 0xffff, 0xfffe, 0xfffd, 0xfffb, 0xfff7, 0xffef, 0xffdf, 0xffbf);
    compFm("XS_ALU_NA_00",(1<<1), 8, 0x0000, 0x8000, 0xc000, 0xe000, 0xf000, 0xf800, 0xfc00, 0xfe00);

    MEAS_MPAT("XS_ALU_AP1_00");
    compFm("XS_ALU_AP1_00",(1<<0), 8, 0x0001, 0x0002, 0x0003, 0x0005, 0x0009, 0x0011, 0x0021, 0x0041);
    compFm("XS_ALU_AP1_00",(1<<1), 8, 0xfffe, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200);

    MEAS_MPAT("XS_ALU_AM1_00");
    compFm("XS_ALU_AM1_00",(1<<0), 8, 0x0001, 0x0002, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f);
    compFm("XS_ALU_AM1_00",(1<<1), 8, 0xfffe, 0x7ffe, 0x3ffe, 0x1ffe, 0x0ffe, 0x07fe, 0x03fe, 0x01fe);

    MEAS_MPAT("XS_ALU_AM2_00");
    compFm("XS_ALU_AM2_00",(1<<0), 8, 0x0000, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080);
    compFm("XS_ALU_AM2_00",(1<<1), 8, 0x0014, 0x0016, 0x0018, 0x001a, 0x001c, 0x001E, 0x0140, 0x0160);

    MEAS_MPAT("XS_ALU_AD2_00");
    compFm("XS_ALU_AD2_00",(1<<0), 8, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080);
    compFm("XS_ALU_AD2_00",(1<<1), 8, 0x0005, 0x0050, 0x0500, 0x5000, 0x5555, 0x5550, 0x5505, 0x5055);
 
    MEAS_MPAT("XS_ALU_B_00");
    compFm("XS_ALU_B_00",(1<<0), 15, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0xffff);
    compFm("XS_ALU_B_00",(1<<1), 15, 0x0080, 0x0100, 0x0200, 0x0400, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x1000, 0X0800, 0xffff);

    MEAS_MPAT("XS_ALU_APB_00");
    compFm("XS_ALU_APB_00",(1<<0), 14, 0x0001, 0x0010, 0x0100, 0x0001, 0x0010, 0x0100, 0x0001, 0x0010, 0x0100, 0x0001, 0x0010, 0x0100, 0x0010, 0x0001);
    compFm("XS_ALU_APB_00",(1<<1), 14, 0x8100, 0x8001, 0x8010, 0x8100, 0x8001, 0x8010, 0x8100, 0x8001, 0x8010, 0x8100, 0x8001, 0x8010, 0x8001, 0X8010);

    MEAS_MPAT("XS_ALU_APB_01");
    compFm("XS_ALU_APB_01",(1<<0), 14, 0x1001, 0x1011, 0x1111, 0x1112, 0x1122, 0x1222, 0x1223, 0x1233, 0x1333, 0x1334, 0x1344, 0x1444, 0x1454, 0x1455);
    compFm("XS_ALU_APB_01",(1<<1), 14, 0x9100, 0x9101, 0x9111, 0x9211, 0x9212, 0x9222, 0x9322, 0x9323, 0x9333, 0x9433, 0x9434, 0x9444, 0x9445, 0X9455);

    MEAS_MPAT("XS_ALU_APB_02");
    compFm("XS_ALU_APB_02",(1<<0), 14, 0x2001, 0x2010, 0x2100, 0x2001, 0x2010, 0x2100, 0x2001, 0x2010, 0x2100, 0x2001, 0x2010, 0x2100, 0x2010, 0x2001);
    compFm("XS_ALU_APB_02",(1<<1), 14, 0xa100, 0xa001, 0xa010, 0xa100, 0xa001, 0xa010, 0xa100, 0xa001, 0xa010, 0xa100, 0xa001, 0xa010, 0xa001, 0Xa010);

    MEAS_MPAT("XS_ALU_APB_03");
    compFm("XS_ALU_APB_03",(1<<0), 14, 0x3001, 0x3010, 0x3100, 0x3001, 0x3010, 0x3100, 0x3001, 0x3010, 0x3100, 0x3001, 0x3010, 0x3100, 0x3010, 0x3001);
    compFm("XS_ALU_APB_03",(1<<1), 14, 0xb100, 0xb001, 0xb010, 0xb100, 0xb001, 0xb010, 0xb100, 0xb001, 0xb010, 0xb100, 0xb001, 0xb010, 0xb001, 0Xb010);

    MEAS_MPAT("XS_ALU_APB_04");
    compFm("XS_ALU_APB_04",(1<<0), 14, 0x4001, 0x4010, 0x4100, 0x4001, 0x4010, 0x4100, 0x4001, 0x4010, 0x4100, 0x4001, 0x4010, 0x4100, 0x4010, 0x4001);
    compFm("XS_ALU_APB_04",(1<<1), 14, 0xc100, 0xc001, 0xc010, 0xc100, 0xc001, 0xc010, 0xc100, 0xc001, 0xc010, 0xc100, 0xc001, 0xc010, 0xc001, 0Xc010);

    MEAS_MPAT("XS_ALU_APB_05");
    compFm("XS_ALU_APB_05",(1<<0), 14, 0x5001, 0x5010, 0x5100, 0x5001, 0x5010, 0x5100, 0x5001, 0x5010, 0x5100, 0x5001, 0x5010, 0x5100, 0x5010, 0x5001);
    compFm("XS_ALU_APB_05",(1<<1), 14, 0xd100, 0xd001, 0xd010, 0xd100, 0xd001, 0xd010, 0xd100, 0xd001, 0xd010, 0xd100, 0xd001, 0xd010, 0xd001, 0Xd010);

    MEAS_MPAT("XS_ALU_APB_06");
    compFm("XS_ALU_APB_06",(1<<0), 14, 0x6001, 0x6010, 0x6100, 0x6001, 0x6010, 0x6100, 0x6001, 0x6010, 0x6100, 0x6001, 0x6010, 0x6100, 0x6010, 0x6001);
    compFm("XS_ALU_APB_06",(1<<1), 14, 0xe100, 0xe001, 0xe010, 0xe100, 0xe001, 0xe010, 0xe100, 0xe001, 0xe010, 0xe100, 0xe001, 0xe010, 0xe001, 0Xe010);

    MEAS_MPAT("XS_ALU_APB_07");
    compFm("XS_ALU_APB_07",(1<<0), 14, 0x7001, 0x7010, 0x7100, 0x7001, 0x7010, 0x7100, 0x7001, 0x7010, 0x7100, 0x7001, 0x7010, 0x7100, 0x7010, 0x7001);
    compFm("XS_ALU_APB_07",(1<<1), 14, 0xf100, 0xf001, 0xf010, 0xf100, 0xf001, 0xf010, 0xf100, 0xf001, 0xf010, 0xf100, 0xf001, 0xf010, 0xf001, 0Xf010);

    MEAS_MPAT("XS_ALU_AMB_00");
    compFm("XS_ALU_AMB_00",(1<<0), 14, 0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0665, 0x0656, 0x0566, 0x0656, 0x0665);
    compFm("XS_ALU_AMB_00",(1<<1), 14, 0x0554, 0x0545, 0x0455, 0x0554, 0x0545, 0x0455, 0x0554, 0x0545, 0x0455, 0x0554, 0x0545, 0x0455, 0x0545, 0x0554);

    MEAS_MPAT("XS_ALU_AMB_01");
    compFm("XS_ALU_AMB_01",(1<<0), 14, 0x0776, 0x0766, 0x0666, 0x0665, 0x0655, 0x0555, 0x0554, 0x0544, 0x0444, 0x0443, 0x0433, 0x0333, 0x0323, 0x0322);
    compFm("XS_ALU_AMB_01",(1<<1), 14, 0x0665, 0x0655, 0x0555, 0x0554, 0x0544, 0x0444, 0x0443, 0x0433, 0x0333, 0x0332, 0x0322, 0x0222, 0x0212, 0x0211);

    MEAS_MPAT("XS_ALU_AMB_02");
    compFm("XS_ALU_AMB_02",(1<<0), 14, 0x0887, 0x0878, 0x0788, 0x0887, 0x0878, 0x0788, 0x0887, 0x0878, 0x0788, 0x0887, 0x0878, 0x0788, 0x0878, 0x0887);
    compFm("XS_ALU_AMB_02",(1<<1), 14, 0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0776, 0x0767, 0x0677, 0x0767, 0x0776);

    MEAS_MPAT("XS_ALU_AMB_03");
    compFm("XS_ALU_AMB_03",(1<<0), 14, 0x0998, 0x0989, 0x0899, 0x0998, 0x0989, 0x0899, 0x0998, 0x0989, 0x0899, 0x0998, 0x0989, 0x0899, 0x0989, 0x0998);
    compFm("XS_ALU_AMB_03",(1<<1), 14, 0x0887, 0x0878, 0x0788, 0x0887, 0x0878, 0x0788, 0x0887, 0x0878, 0x0788, 0x0887, 0x0878, 0x0788, 0x0878, 0x0887);

    MEAS_MPAT("XS_ALU_AMB_04");
    compFm("XS_ALU_AMB_04",(1<<0), 14, 0x0aa9, 0x0a9a, 0x09aa, 0x0aa9, 0x0a9a, 0x09aa, 0x0aa9, 0x0a9a, 0x09aa, 0x0aa9, 0x0a9a, 0x09aa, 0x0a9a, 0x0aa9);
    compFm("XS_ALU_AMB_04",(1<<1), 14, 0x0998, 0x0989, 0x0899, 0x0998, 0x0989, 0x0899, 0x0998, 0x0989, 0x0899, 0x0998, 0x0989, 0x0899, 0x0989, 0x0998);

    MEAS_MPAT("XS_ALU_AMB_05");
    compFm("XS_ALU_AMB_05",(1<<0), 14, 0x0bba, 0x0bab, 0x0abb, 0x0bba, 0x0bab, 0x0abb, 0x0bba, 0x0bab, 0x0abb, 0x0bba, 0x0bab, 0x0abb, 0x0bab, 0x0bba);
    compFm("XS_ALU_AMB_05",(1<<1), 14, 0x0aa9, 0x0a9a, 0x09aa, 0x0aa9, 0x0a9a, 0x09aa, 0x0aa9, 0x0a9a, 0x09aa, 0x0aa9, 0x0a9a, 0x09aa, 0x0a9a, 0x0aa9);

    MEAS_MPAT("XS_ALU_AMB_06");
    compFm("XS_ALU_AMB_06",(1<<0), 14, 0x0ccb, 0x0cbc, 0x0bcc, 0x0ccb, 0x0cbc, 0x0bcc, 0x0ccb, 0x0cbc, 0x0bcc, 0x0ccb, 0x0cbc, 0x0bcc, 0x0cbc, 0x0ccb);
    compFm("XS_ALU_AMB_06",(1<<1), 14, 0x0bba, 0x0bab, 0x0abb, 0x0bba, 0x0bab, 0x0abb, 0x0bba, 0x0bab, 0x0abb, 0x0bba, 0x0bab, 0x0abb, 0x0bab, 0x0bba);

    MEAS_MPAT("XS_ALU_AMB_07");
    compFm("XS_ALU_AMB_07",(1<<0), 14, 0x0ddc, 0x0dcd, 0x0cdd, 0x0ddc, 0x0dcd, 0x0cdd, 0x0ddc, 0x0dcd, 0x0cdd, 0x0ddc, 0x0dcd, 0x0cdd, 0x0dcd, 0x0ddc);
    compFm("XS_ALU_AMB_07",(1<<1), 14, 0x0ccb, 0x0cbc, 0x0bcc, 0x0ccb, 0x0cbc, 0x0bcc, 0x0ccb, 0x0cbc, 0x0bcc, 0x0ccb, 0x0cbc, 0x0bcc, 0x0cbc, 0x0ccb);

    MEAS_MPAT("XS_ALU_AANDB_00");
    compFm("XS_ALU_AANDB_00",(1<<0), 14, 0xfffe, 0xfffd, 0xfffb, 0xfff7, 0xffef, 0xffdf, 0xffbf, 0xff7f, 0xfeff, 0xfdff, 0xfbff, 0xf7ff, 0xefff, 0xdfff);
    compFm("XS_ALU_AANDB_00",(1<<1), 14, 0x8ffe, 0x8ffd, 0x8ffb, 0x8ff7, 0x8fef, 0x8fdf, 0x8fbf, 0x8f7f, 0x8eff, 0x8dff, 0x8bff, 0x87ff, 0x8fff, 0x8fff);

    MEAS_MPAT("XS_ALU_AANDB_01");
    compFm("XS_ALU_AANDB_01",(1<<0), 14, 0xfffe, 0xfffc, 0xfff8, 0xfff0, 0xffe0, 0xffc0, 0xff80, 0xff00, 0xfe00, 0xfc00, 0xf800, 0xf000, 0xe000, 0xc000);
    compFm("XS_ALU_AANDB_01",(1<<1), 14, 0x8ffe, 0x8ffc, 0x8ff8, 0x8ff0, 0x8fe0, 0x8fc0, 0x8f80, 0x8f00, 0x8e00, 0x8c00, 0x8800, 0x8000, 0x8000, 0x8000);

    MEAS_MPAT("XS_ALU_AANDB_02");
    compFm("XS_ALU_AANDB_02",(1<<0), 14, 0xfffe, 0xfffd, 0xfffb, 0xfff7, 0xffef, 0xffdf, 0xffbf, 0xff7f, 0xfeff, 0xfdff, 0xfbff, 0xf7ff, 0xefff, 0xdfff);
    compFm("XS_ALU_AANDB_02",(1<<1), 14, 0x8ffe, 0x8ffd, 0x8ffb, 0x8ff7, 0x8fef, 0x8fdf, 0x8fbf, 0x8f7f, 0x8eff, 0x8dff, 0x8bff, 0x87ff, 0x8fff, 0x8fff);

    MEAS_MPAT("XS_ALU_AANDB_03");
    compFm("XS_ALU_AANDB_03",(1<<0), 14, 0xdffe, 0xdffd, 0xdffb, 0xdff7, 0xdfef, 0xdfdf, 0xdfbf, 0xdf7f, 0xdeff, 0xddff, 0xdbff, 0xd7ff, 0xcfff, 0xdfff);
    compFm("XS_ALU_AANDB_03",(1<<1), 14, 0xcffe, 0xcffd, 0xcffb, 0xcff7, 0xcfef, 0xcfdf, 0xcfbf, 0xcf7f, 0xceff, 0xcdff, 0xcbff, 0xc7ff, 0xcfff, 0xcfff);

    MEAS_MPAT("XS_ALU_AANDB_04");
    compFm("XS_ALU_AANDB_04",(1<<0), 14, 0xcffe, 0xcffd, 0xcffb, 0xcff7, 0xcfef, 0xcfdf, 0xcfbf, 0xcf7f, 0xceff, 0xcdff, 0xcbff, 0xc7ff, 0xcfff, 0xcfff);
    compFm("XS_ALU_AANDB_04",(1<<1), 14, 0xbffe, 0xbffd, 0xbffb, 0xbff7, 0xbfef, 0xbfdf, 0xbfbf, 0xbf7f, 0xbeff, 0xbdff, 0xbbff, 0xb7ff, 0xafff, 0x9fff);

    MEAS_MPAT("XS_ALU_AANDB_05");
    compFm("XS_ALU_AANDB_05",(1<<0), 14, 0xbffe, 0xbffd, 0xbffb, 0xbff7, 0xbfef, 0xbfdf, 0xbfbf, 0xbf7f, 0xbeff, 0xbdff, 0xbbff, 0xb7ff, 0xafff, 0x9fff);
    compFm("XS_ALU_AANDB_05",(1<<1), 14, 0xaffe, 0xaffd, 0xaffb, 0xaff7, 0xafef, 0xafdf, 0xafbf, 0xaf7f, 0xaeff, 0xadff, 0xabff, 0xa7ff, 0xafff, 0x8fff);

    MEAS_MPAT("XS_ALU_AANDB_06");
    compFm("XS_ALU_AANDB_06",(1<<0), 14, 0xaffe, 0xaffd, 0xaffb, 0xaff7, 0xafef, 0xafdf, 0xafbf, 0xaf7f, 0xaeff, 0xadff, 0xabff, 0xa7ff, 0xafff, 0x8fff);
    compFm("XS_ALU_AANDB_06",(1<<1), 14, 0x9ffe, 0x9ffd, 0x9ffb, 0x9ff7, 0x9fef, 0x9fdf, 0x9fbf, 0x9f7f, 0x9eff, 0x9dff, 0x9bff, 0x97ff, 0x8fff, 0x9fff);

    MEAS_MPAT("XS_ALU_AANDB_07");
    compFm("XS_ALU_AANDB_07",(1<<0), 14, 0x9ffe, 0x9ffd, 0x9ffb, 0x9ff7, 0x9fef, 0x9fdf, 0x9fbf, 0x9f7f, 0x9eff, 0x9dff, 0x9bff, 0x97ff, 0x8fff, 0x9fff);
    compFm("XS_ALU_AANDB_07",(1<<1), 14, 0x8ffe, 0x8ffd, 0x8ffb, 0x8ff7, 0x8fef, 0x8fdf, 0x8fbf, 0x8f7f, 0x8eff, 0x8dff, 0x8bff, 0x87ff, 0x8fff, 0x8fff);

}

void testPG_alu_xsys_2()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	load_pds("diag_pds_data_xy16");
    SEND_MPAT("diag_xsys_2.mpa");

    MEAS_MPAT("XS_ALU_AORB_00");
    int fm_size=diag_readFmData();
    compFm("XS_ALU_AORB_00",(1<<0), 14, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000);
    compFm("XS_ALU_AORB_00",(1<<1), 14, 0x8000, 0x8001, 0x8002, 0x8004, 0x8008, 0x8010, 0x8020, 0x8040, 0x8080, 0x8100, 0x8200, 0x8400, 0x8800, 0x9000);

    MEAS_MPAT("XS_ALU_AORB_01");
    compFm("XS_ALU_AORB_01",(1<<0), 14, 0x1000, 0x1001, 0x1003, 0x1007, 0x100f, 0x101f, 0x103f, 0x107f, 0x10ff, 0x11ff, 0x13ff, 0x17ff, 0x1fff, 0x1fff);
    compFm("XS_ALU_AORB_01",(1<<1), 14, 0x9000, 0x9001, 0x9003, 0x9007, 0x900f, 0x901f, 0x903f, 0x907f, 0x90ff, 0x91ff, 0x93ff, 0x97ff, 0x9fff, 0x9fff);

    MEAS_MPAT("XS_ALU_AORB_02");
    compFm("XS_ALU_AORB_02",(1<<0), 14, 0x2000, 0x2001, 0x2002, 0x2004, 0x2008, 0x2010, 0x2020, 0x2040, 0x2080, 0x2100, 0x2200, 0x2400, 0x2800, 0x3000);
    compFm("XS_ALU_AORB_02",(1<<1), 14, 0xa000, 0xa001, 0xa002, 0xa004, 0xa008, 0xa010, 0xa020, 0xa040, 0xa080, 0xa100, 0xa200, 0xa400, 0xa800, 0xb000);

    MEAS_MPAT("XS_ALU_AORB_03");
    compFm("XS_ALU_AORB_03",(1<<0), 14, 0x3000, 0x3001, 0x3002, 0x3004, 0x3008, 0x3010, 0x3020, 0x3040, 0x3080, 0x3100, 0x3200, 0x3400, 0x3800, 0x3000);
    compFm("XS_ALU_AORB_03",(1<<1), 14, 0xb000, 0xb001, 0xb002, 0xb004, 0xb008, 0xb010, 0xb020, 0xb040, 0xb080, 0xb100, 0xb200, 0xb400, 0xb800, 0xb000);

    MEAS_MPAT("XS_ALU_AORB_04");
    compFm("XS_ALU_AORB_04",(1<<0), 14, 0x4000, 0x4001, 0x4002, 0x4004, 0x4008, 0x4010, 0x4020, 0x4040, 0x4080, 0x4100, 0x4200, 0x4400, 0x4800, 0x5000);
    compFm("XS_ALU_AORB_04",(1<<1), 14, 0xc000, 0xc001, 0xc002, 0xc004, 0xc008, 0xc010, 0xc020, 0xc040, 0xc080, 0xc100, 0xc200, 0xc400, 0xc800, 0xd000);

    MEAS_MPAT("XS_ALU_AORB_05");
    compFm("XS_ALU_AORB_05",(1<<0), 14, 0x5000, 0x5001, 0x5002, 0x5004, 0x5008, 0x5010, 0x5020, 0x5040, 0x5080, 0x5100, 0x5200, 0x5400, 0x5800, 0x5000);
    compFm("XS_ALU_AORB_05",(1<<1), 14, 0xd000, 0xd001, 0xd002, 0xd004, 0xd008, 0xd010, 0xd020, 0xd040, 0xd080, 0xd100, 0xd200, 0xd400, 0xd800, 0xd000);

    MEAS_MPAT("XS_ALU_AORB_06");
    compFm("XS_ALU_AORB_06",(1<<0), 14, 0x6000, 0x6001, 0x6002, 0x6004, 0x6008, 0x6010, 0x6020, 0x6040, 0x6080, 0x6100, 0x6200, 0x6400, 0x6800, 0x7000);
    compFm("XS_ALU_AORB_06",(1<<1), 14, 0xe000, 0xe001, 0xe002, 0xe004, 0xe008, 0xe010, 0xe020, 0xe040, 0xe080, 0xe100, 0xe200, 0xe400, 0xe800, 0xf000);

    MEAS_MPAT("XS_ALU_AORB_07");
    compFm("XS_ALU_AORB_07",(1<<0), 14, 0x7000, 0x7001, 0x7002, 0x7004, 0x7008, 0x7010, 0x7020, 0x7040, 0x7080, 0x7100, 0x7200, 0x7400, 0x7800, 0x7000);
    compFm("XS_ALU_AORB_07",(1<<1), 14, 0xf000, 0xf001, 0xf002, 0xf004, 0xf008, 0xf010, 0xf020, 0xf040, 0xf080, 0xf100, 0xf200, 0xf400, 0xf800, 0xf000);

    MEAS_MPAT("XS_ALU_AXORB_00");
    compFm("XS_ALU_AXORB_00",(1<<0), 14, 0xffff, 0xfffe, 0xfffd, 0xfffb, 0xfff7, 0xffef, 0xffdf, 0xffbf, 0xff7f, 0xfeff, 0xfdff, 0xfbff, 0xf7ff, 0xefff);
    compFm("XS_ALU_AXORB_00",(1<<1), 14, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000);

    MEAS_MPAT("XS_ALU_AXORB_01");
    compFm("XS_ALU_AXORB_01",(1<<0), 14, 0x1000, 0x1001, 0x1003, 0x1007, 0x100f, 0x101f, 0x103f, 0x107f, 0x10ff, 0x11ff, 0x13ff, 0x17ff, 0x1fff, 0x0fff);
    compFm("XS_ALU_AXORB_01",(1<<1), 14, 0x9000, 0x9001, 0x9003, 0x9007, 0x900f, 0x901f, 0x903f, 0x907f, 0x90ff, 0x91ff, 0x93ff, 0x97ff, 0x9fff, 0x8fff);

    MEAS_MPAT("XS_ALU_AXORB_02");
    compFm("XS_ALU_AXORB_02",(1<<0), 14, 0x2000, 0x2001, 0x2002, 0x2004, 0x2008, 0x2010, 0x2020, 0x2040, 0x2080, 0x2100, 0x2200, 0x2400, 0x2800, 0x3000);
    compFm("XS_ALU_AXORB_02",(1<<1), 14, 0xa000, 0xa001, 0xa002, 0xa004, 0xa008, 0xa010, 0xa020, 0xa040, 0xa080, 0xa100, 0xa200, 0xa400, 0xa800, 0xb000);

    MEAS_MPAT("XS_ALU_AXORB_03");
    compFm("XS_ALU_AXORB_03",(1<<0), 14, 0x3000, 0x3001, 0x3002, 0x3004, 0x3008, 0x3010, 0x3020, 0x3040, 0x3080, 0x3100, 0x3200, 0x3400, 0x3800, 0x2000);
    compFm("XS_ALU_AXORB_03",(1<<1), 14, 0xb000, 0xb001, 0xb002, 0xb004, 0xb008, 0xb010, 0xb020, 0xb040, 0xb080, 0xb100, 0xb200, 0xb400, 0xb800, 0xa000);

    MEAS_MPAT("XS_ALU_AXORB_04");
    compFm("XS_ALU_AXORB_04",(1<<0), 14, 0x4000, 0x4001, 0x4002, 0x4004, 0x4008, 0x4010, 0x4020, 0x4040, 0x4080, 0x4100, 0x4200, 0x4400, 0x4800, 0x5000);
    compFm("XS_ALU_AXORB_04",(1<<1), 14, 0xc000, 0xc001, 0xc002, 0xc004, 0xc008, 0xc010, 0xc020, 0xc040, 0xc080, 0xc100, 0xc200, 0xc400, 0xc800, 0xd000);

    MEAS_MPAT("XS_ALU_AXORB_05");
    compFm("XS_ALU_AXORB_05",(1<<0), 14, 0x5000, 0x5001, 0x5002, 0x5004, 0x5008, 0x5010, 0x5020, 0x5040, 0x5080, 0x5100, 0x5200, 0x5400, 0x5800, 0x4000);
    compFm("XS_ALU_AXORB_05",(1<<1), 14, 0xd000, 0xd001, 0xd002, 0xd004, 0xd008, 0xd010, 0xd020, 0xd040, 0xd080, 0xd100, 0xd200, 0xd400, 0xd800, 0xc000);

    MEAS_MPAT("XS_ALU_AXORB_06");
    compFm("XS_ALU_AXORB_06",(1<<0), 14, 0x6000, 0x6001, 0x6002, 0x6004, 0x6008, 0x6010, 0x6020, 0x6040, 0x6080, 0x6100, 0x6200, 0x6400, 0x6800, 0x7000);
    compFm("XS_ALU_AXORB_06",(1<<1), 14, 0xe000, 0xe001, 0xe002, 0xe004, 0xe008, 0xe010, 0xe020, 0xe040, 0xe080, 0xe100, 0xe200, 0xe400, 0xe800, 0xf000);

    MEAS_MPAT("XS_ALU_AXORB_07");
    compFm("XS_ALU_AXORB_07",(1<<0), 14, 0x7000, 0x7001, 0x7002, 0x7004, 0x7008, 0x7010, 0x7020, 0x7040, 0x7080, 0x7100, 0x7200, 0x7400, 0x7800, 0x6000);
    compFm("XS_ALU_AXORB_07",(1<<1), 14, 0xf000, 0xf001, 0xf002, 0xf004, 0xf008, 0xf010, 0xf020, 0xf040, 0xf080, 0xf100, 0xf200, 0xf400, 0xf800, 0xe000);

    MEAS_MPAT("XS_ALU_D3D4_00");
    compFm("XS_ALU_D3D4_00",(1<<0), 14, 0x1111, 0x1111, 0x0000, 0x0001, 0x0002, 0x0001, 0x0002, 0x0004, 0x0008, 0x0004, 0x0008, 0x0888, 0x0888, 0x0888);
    compFm("XS_ALU_D3D4_00",(1<<1), 14, 0xaaaa, 0xaaaa, 0xaaab, 0x0000, 0x0001, 0x0002, 0x0001, 0x0002, 0x0001, 0x0002, 0x0004, 0x5555, 0x5555, 0x5555);

    MEAS_MPAT("XS_ALU_APImm_00");
    compFm("XS_ALU_APImm_00",(1<<0), 4, 0x0001, 0x0002, 0x0004, 0x0008);
    compFm("XS_ALU_APImm_00",(1<<1), 4, 0x8001, 0x8002, 0x8004, 0x8008);

    MEAS_MPAT("XS_ALU_APImm_01");
    compFm("XS_ALU_APImm_01",(1<<0), 4, 0x0010, 0x0030, 0x0070, 0x00f0);
    compFm("XS_ALU_APImm_01",(1<<1), 4, 0x8010, 0x8030, 0x8070, 0x80f0);

    MEAS_MPAT("XS_ALU_APImm_02");
    compFm("XS_ALU_APImm_02",(1<<0), 4, 0x0100, 0x0200, 0x0400, 0x0800);
    compFm("XS_ALU_APImm_02",(1<<1), 4, 0x8100, 0x8200, 0x8400, 0x8800);

    MEAS_MPAT("XS_ALU_APImm_03");
    compFm("XS_ALU_APImm_03",(1<<0), 4, 0x1000, 0x2000, 0x4000, 0x8000);
    compFm("XS_ALU_APImm_03",(1<<1), 4, 0x1001, 0x2001, 0x4001, 0x8001);

    MEAS_MPAT("XS_ALU_APImm_04");
    compFm("XS_ALU_APImm_04",(1<<0), 4, 0x000a, 0x000b, 0x000c, 0x000d);
    compFm("XS_ALU_APImm_04",(1<<1), 4, 0x800a, 0x800b, 0x800c, 0x800d);

    MEAS_MPAT("XS_ALU_APImm_05");
    compFm("XS_ALU_APImm_05",(1<<0), 4, 0x00a0, 0x00b0, 0x00c0, 0x00d0);
    compFm("XS_ALU_APImm_05",(1<<1), 4, 0x80a0, 0x80b0, 0x80c0, 0x80d0);

    MEAS_MPAT("XS_ALU_APImm_06");
    compFm("XS_ALU_APImm_06",(1<<0), 4, 0x0a00, 0x0b00, 0x0c00, 0x0d00);
    compFm("XS_ALU_APImm_06",(1<<1), 4, 0x0a01, 0x0b01, 0x0c01, 0x0d01);

    MEAS_MPAT("XS_ALU_APImm_07");
    compFm("XS_ALU_APImm_07",(1<<0), 4, 0xa000, 0x0b00, 0x00c0, 0x000d);
    compFm("XS_ALU_APImm_07",(1<<1), 4, 0xb000, 0x1b00, 0x10c0, 0x100d);

    MEAS_MPAT("XS_ALU_AMImm_00");
    compFm("XS_ALU_AMImm_00",(1<<0), 4, 0xefff, 0xeffe, 0xeffc, 0xeff8);
    compFm("XS_ALU_AMImm_00",(1<<1), 4, 0x7fff, 0x7ffe, 0x7ffc, 0x7ff8);

    MEAS_MPAT("XS_ALU_AMImm_01");
    compFm("XS_ALU_AMImm_01",(1<<0), 4, 0xeff0, 0xefd0, 0xef90, 0xef10);
    compFm("XS_ALU_AMImm_01",(1<<1), 4, 0x7ff0, 0x7fd0, 0x7f90, 0x7f10);

    MEAS_MPAT("XS_ALU_AMImm_02");
    compFm("XS_ALU_AMImm_02",(1<<0), 4, 0xef00, 0xee00, 0xec00, 0xe800);
    compFm("XS_ALU_AMImm_02",(1<<1), 4, 0x7f00, 0x7e00, 0x7c00, 0x7800);

    MEAS_MPAT("XS_ALU_AMImm_03");
    compFm("XS_ALU_AMImm_03",(1<<0), 4, 0xe000, 0xee00, 0xefc0, 0xeff8);
    compFm("XS_ALU_AMImm_03",(1<<1), 4, 0x7000, 0x7e00, 0x7fc0, 0x7ff8);

    MEAS_MPAT("XS_ALU_AMImm_04");
    compFm("XS_ALU_AMImm_04",(1<<0), 4, 0xeff6, 0xeff5, 0xeff4, 0xeff3);
    compFm("XS_ALU_AMImm_04",(1<<1), 4, 0x7ff6, 0x7ff5, 0x7ff4, 0x7ff3);

    MEAS_MPAT("XS_ALU_AMImm_05");
    compFm("XS_ALU_AMImm_05",(1<<0), 4, 0xef60, 0xef50, 0xef40, 0xef30);
    compFm("XS_ALU_AMImm_05",(1<<1), 4, 0x7f60, 0x7f50, 0x7f40, 0x7f30);

    MEAS_MPAT("XS_ALU_AMImm_06");
    compFm("XS_ALU_AMImm_06",(1<<0), 4, 0xe600, 0xe500, 0xe400, 0xe300);
    compFm("XS_ALU_AMImm_06",(1<<1), 4, 0x7600, 0x7500, 0x7400, 0x7300);

    MEAS_MPAT("XS_ALU_AMImm_07");
    compFm("XS_ALU_AMImm_07",(1<<0), 4, 0x5000, 0xe500, 0xef40, 0xeff3);
    compFm("XS_ALU_AMImm_07",(1<<1), 4, 0x4000, 0xd500, 0xdf40, 0xdff3);

}


void testPG_alu_xyz()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;
	diag_fmDrvMode(1);

	load_pds("diag_pds_data_xy16");
    SEND_MPAT("diag_xyz.mpa");
//--XOUT--------------------------------------------------------------------------------------------------------------------
    MEAS_MPAT("XY_OUT_00");   
    compFm("XY_OUT_00",(1<<0)    , 16, 0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006,	0x0007,	0x0008,	0x8001,	0x8002,	0x8003,	0x8004,	0x8005,	0x8006,	0x8007,	0x8008);
    compFm("XY_OUT_00",(1<<1)    , 16, 0x8001,	0x8002,	0x8003,	0x8004,	0x8005,	0x8006,	0x8007,	0x8008,	0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006,	0x0007,	0x0008);	

    MEAS_MPAT("XY_OUT_01");   
    compFm("XY_OUT_01",(1<<0)    , 16, 0x0006,	0x0007,	0x0008,	0x8001,	0x8002,	0x8003,	0x8004,	0x8005,	0x8006,	0x8007,	0x8008,	0x0011,	0x8008,	0x1212,	0xaaaa,	0x0002);
    compFm("XY_OUT_01",(1<<1)    , 16, 0x8006,	0x8007,	0x8008,	0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006,	0x0007,	0x0008,	0x8011,	0x0008,	0x1212,	0xbbbb,	0xbbbb);	
   
    MEAS_MPAT("XB_ALU_XT_00");   
    compFm("XB_ALU_XT_00",(1<<0) , 16, 0x0000,  0x0001, 0x0002, 0x0001, 0x0002 ,0x0001, 0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100);
    compFm("XB_ALU_XT_00",(1<<1) , 16, 0x0000,  0x000A, 0x000B, 0x000A, 0x000B, 0x000A, 0x0000, 0x000A, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00);	
//--Z--------------------------------------------------------------------------------------------------------------------
 
    MEAS_MPAT("Z_ALU_Z_00");   
    compFm("Z_ALU_Z_00",(1<<0)   , 16, 0x0000,	0xffff,	0x0000,	0x0001,	0x0002,	0x0001,	0x0002,	0x0004,	0x0002,	0x0002, 0x0001, 0x0000, 0x0000, 0xffff, 0x0000, 0xffff);

    SET_REG(ZMAX,0xF);
    MEAS_MPAT("Z_ALU_A_00");   
    compFm("Z_ALU_A_00",(1<<0)   , 16, 0x0000,	0x000a,	0x000b,	0x000f,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x000f,	0x0000,	0x0001,	0x0002,	0x0003, 0x0004, 0x0005);
 
    SET_REG(ZMAX,0xFFFF);
    MEAS_MPAT("Z_ALU_A_01");   
    compFm("Z_ALU_A_00",(1<<0)   , 16, 0x0000,	0x1234,	0x3456,	0xffff,	0x1234,	0x1235,	0x1236,	0x1237,	0x1238,	0x1239,	0x123A,	0x123B,	0x123C,	0x123D, 0x123E, 0x123F);
 
    SET_REG(ZMAX,0xFF);
    MEAS_MPAT("Z_ALU_ZPA_00");   
    compFm("Z_ALU_ZPA_00",(1<<0) , 16, 0x0011,  0x0011, 0x0012, 0x0014, 0x0018, 0x0029, 0x0028, 0x0028, 0x0029, 0x002B, 0x002F, 0x0040, 0x003F, 0x003F, 0x0040, 0x0000);
 
    SET_REG(ZMAX,0xF);
    MEAS_MPAT("Z_ALU_ZMA_00");  
    compFm("Z_ALU_ZMA_00",(1<<0) , 16, 0x000f,	0x000e,	0x000d,	0x000e,0x000f,	0x000e,	0x000d,	0x000e,0x000f,	0x000e,	0x000d,	0x000e,0x000f,	0x000e,	0x000d,	0x000e);
}

void testPG_alu_scan()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;
	
	load_pds("diag_pds_data_xy16");

    SEND_MPAT("diag_scan.mpa");
//--SCAN--------------------------------------------------------------------------------------------------------------------

    SET_REG(S1A,0x33);
    MEAS_MPAT("S_ALU_SA_00");   
    compFm("S_ALU_SA_00",(1<<7), 16, 0x0000,0x0044, 0x0010, 0x0033, 0x0053, 0x0043, 0x0062, 0x00F2, 0x0000,0x0044, 0x0010, 0x0033, 0x0053, 0x0043, 0x0062, 0x00F2);
 
    MEAS_MPAT("S_ALU_SB_00");   
    compFm("S_ALU_SB_00",(1<<7), 16, 0x0000,0x0044, 0x0010, 0x0033, 0x0053, 0x0043, 0x0083, 0x0053, 0x0000,0x0044, 0x0010, 0x0033, 0x0053, 0x0043, 0x0083, 0x0053);

	//writePG(1<<9 | 11,0x2,1);	//rb_pin enable
	//writePG(1<<9 | 12,0x0,1);	//rb_pin enable
	//writePG(1<<9 | 12,0x1,1);	//rb_pin enable
	//writePG(1<<9 | 12,0x0,1);	//rb_pin enable

    //MEAS_MPAT("S_OUT_CNT_00");   
    //int fm_size=diag_readFmData();
    //compFm("S_ALU_SB_00",(1<<7), 16, 0x0000,0x0044, 0x0010, 0x0033, 0x0053, 0x0043, 0x0083, 0x0053, 0x0000,0x0044, 0x0010, 0x0033, 0x0053, 0x0043, 0x0083, 0x0053);
    //diag_showDrv(0,fm_size);
    //diag_showData(0,fm_size);


}

void testPG_alu_tp()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;
	
	load_pds("diag_pds_data_data16");
    SEND_MPAT("diag_tp.mpa");
//--TP--------------------------------------------------------------------------------------------------------------------
    MEAS_MPAT("TP_ALU_A_00");
    compFm("TP_ALU_A_00",(1<<0)    , 16, 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x7777, 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x7777, 0x6666, 0x7777 );
    compFm("TP_ALU_A_00",(1<<1)    , 16, 0xaaaa, 0xbbbb, 0xcccc, 0xdddd, 0xeeee, 0xffff, 0xa5a5, 0xaaaa, 0xbbbb, 0xcccc, 0xdddd, 0xeeee, 0xffff, 0xa5a5, 0xffff, 0xa5a5 );

    MEAS_MPAT("TP_ALU_TPA_00");
    compFm("TP_ALU_TPA_00",(1<<0)  , 16, 0x0001, 0x0011, 0x0111, 0x1111, 0x1112, 0x1122, 0x1222, 0x0001, 0x0011, 0x0111, 0x1111, 0x1112, 0x1122, 0x1222, 0x1232, 0x1332);
    compFm("TP_ALU_TPA_00",(1<<1)  , 16, 0x0002, 0x0022, 0x0222, 0x2222, 0x2224, 0x2244, 0x2444, 0x0002, 0x0022, 0x0222, 0x2222, 0x2224, 0x2244, 0x2444, 0x2464, 0x2664);

    MEAS_MPAT("TP_ALU_TMA_00");
    compFm("TP_ALU_TMA_00",(1<<0)  , 16, 0xfffe, 0xffee, 0xfeee, 0xeeee, 0xeeed, 0xeedd, 0xeddd, 0xfffe, 0xffee, 0xfeee, 0xeeee, 0xeeed, 0xeedd, 0xeddd, 0x0000, 0xffff);
    compFm("TP_ALU_TMA_00",(1<<1)  , 16, 0xfffd, 0xffdd, 0xfddd, 0xdddd, 0xdddb, 0xddbb, 0xdbbb, 0xfffd, 0xffdd, 0xfddd, 0xdddd, 0xdddb, 0xddbb, 0xdbbb, 0x0000, 0xffff);

    MEAS_MPAT("TP_ALU_TANDA_00");
    compFm("TP_ALU_TANDA_00",(1<<0), 16, 0xfffe, 0xfffc, 0xfff8, 0xfff0, 0xffe0, 0xffc0, 0xff80, 0xfffe, 0xfffc, 0xfff8, 0xfff0, 0xffe0, 0xffc0, 0xff80, 0x0000, 0xffff);
    compFm("TP_ALU_TANDA_00",(1<<1), 16, 0xfeff, 0xfcff, 0xf8ff, 0xf0ff, 0xe0ff, 0xc0ff, 0x80ff, 0xfeff, 0xfcff, 0xf8ff, 0xf0ff, 0xe0ff, 0xc0ff, 0x80ff, 0xffff, 0x0000);

    MEAS_MPAT("TP_ALU_TORA_00");
    compFm("TP_ALU_TORA_00",(1<<0) , 16, 0x0001, 0x0011, 0x0111, 0x1111, 0x1113, 0x1133, 0x1333, 0x0001, 0x0011, 0x0111, 0x1111, 0x1113, 0x1133, 0x1333, 0x0000, 0xffff);
    compFm("TP_ALU_TORA_00",(1<<1) , 16, 0x0002, 0x0022, 0x0222, 0x2222, 0x2226, 0x2266, 0x2666, 0x0002, 0x0022, 0x0222, 0x2222, 0x2226, 0x2266, 0x2666, 0xffff, 0x0000);

    MEAS_MPAT("TP_ALU_TMULA_00");
    compFm("TP_ALU_TMULA_00",(1<<0), 16, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0x0000);
    compFm("TP_ALU_TMULA_00",(1<<1), 16, 0x2002, 0x4004, 0x8008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0x0000);

    MEAS_MPAT("TP_ALU_TDA_00");
    compFm("TP_ALU_TDA_00",(1<<0)  , 16, 0x7fff, 0x3fff, 0x1fff, 0x0fff, 0x07ff, 0x03ff, 0x01ff, 0x00ff, 0x007f, 0x003f, 0x001f, 0x000f, 0x0007, 0x0003, 0x0001, 0x0000);
    compFm("TP_ALU_TDA_00",(1<<1)  , 16, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000);

    MEAS_MPAT("TP_ALU_Imm_00");
    compFm("TP_ALU_Imm_00",(1<<0)  , 16, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x1111, 0x2222);
    compFm("TP_ALU_Imm_00",(1<<1)  , 16, 0x8001, 0x8002, 0x8004, 0x8008, 0x8010, 0x8020, 0x8040, 0x8001, 0x8002, 0x8004, 0x8008, 0x8010, 0x8020, 0x8040, 0x3333, 0x4444);	

    MEAS_MPAT("TP_ALU_TPX_00");
    compFm("TP_ALU_TPX_00",(1<<0)  , 16, 0x1001, 0x1002, 0x1004, 0x1008, 0x1010, 0x1020, 0x1040, 0x1080, 0x2001, 0x2002, 0x2004, 0x2008, 0x2010, 0x2020, 0x2040, 0x2080);
    compFm("TP_ALU_TPX_00",(1<<1)  , 16, 0x6001, 0x6002, 0x6004, 0x6008, 0x6010, 0x6020, 0x6040, 0x6080, 0x7001, 0x7002, 0x7004, 0x7008, 0x7010, 0x7020, 0x7040, 0x7080);

    MEAS_MPAT("TP_ALU_TPH_00");    
    compFm("TP_ALU_TPH_00",(1<<0)  , 16, 0x0001, 0x0002, 0x0002, 0x0003, 0x0004, 0x0002, 0x0003, 0x0004, 0x0001, 0x0002, 0x0002, 0x0003, 0x0004, 0x0002, 0x0003, 0x0004);
    compFm("TP_ALU_TPH_00",(1<<1)  , 16, 0x7fff, 0x7ffe, 0x7ffe, 0x7ffd, 0x7ffc, 0x7ffe, 0x7ffd, 0x7ffc, 0x7fff, 0x7ffe, 0x7ffe, 0x7ffd, 0x7ffc, 0x7ffe, 0x7ffd, 0x7ffc);					

    MEAS_MPAT("TP_ALU_INV_00");
    compFm("TP_ALU_INV_00",(1<<0)  , 16, 0x0001, 0x0002, 0xfffd, 0xeeee, 0xdddd, 0xcccc, 0xbbbb, 0xaaaa, 0x9999, 0x8888, 0x0001, 0x0002, 0xfffd, 0xeeee, 0xdddd, 0xcccc);
    compFm("TP_ALU_INV_00",(1<<1)  , 16, 0x7fff, 0x7ffe, 0x8001, 0x5555, 0x4444, 0x3333, 0x2222, 0x1111, 0x7777, 0x6666, 0x7fff, 0x7ffe, 0x8001, 0x5555, 0x4444, 0x3333);

    MEAS_MPAT("TP_ALU_D7D8_00");
    compFm("TP_ALU_D7D8_00",(1<<0) , 16, 0x0000, 0x1111, 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x6667, 0x6666, 0x3333, 0x0000, 0x1111, 0x1111, 0x2222, 0x3333);
    compFm("TP_ALU_D7D8_00",(1<<1) , 16, 0x0000, 0x8000, 0xaaaa, 0xbbbb, 0xcccc, 0xdddd, 0xeeee, 0x8888, 0x8889, 0x8888, 0x4444, 0x0000, 0x8000, 0xaaaa, 0xbbbb, 0xcccc);

    MEAS_MPAT("TP_ALU_D7D8_01");
    compFm("TP_ALU_D7D8_01",(1<<0) , 16, 0x0000, 0x0001, 0x0003, 0x0006, 0x000a, 0x0010, 0x0011, 0x0012, 0x0013, 0x0001, 0x0000, 0xfffe, 0xfffb, 0xfff7, 0xfff1, 0xfff0);
    compFm("TP_ALU_D7D8_01",(1<<1) , 16, 0x8000, 0x8010, 0x8030, 0x8060, 0x80a0, 0x8100, 0x8110, 0x8120, 0x8130, 0x0010, 0x0000, 0xffe0, 0xffb0, 0xff70, 0xff10, 0xff00);

    MEAS_MPAT("TP_ALU_D7D8_02");
    compFm("TP_ALU_D7D8_02",(1<<0) , 16, 0x0003, 0x0006, 0x000a, 0x0010, 0x0011, 0x0012, 0x0013, 0x0001, 0x0000, 0xfffe, 0xfffb, 0xfff7, 0xfff1, 0xfff0, 0xffef, 0xffee);
    compFm("TP_ALU_D7D8_02",(1<<1) , 16, 0x8030, 0x8060, 0x80a0, 0x8100, 0x8110, 0x8120, 0x8130, 0x0010, 0x0000, 0xffe0, 0xffb0, 0xff70, 0xff10, 0xff00, 0xfef0, 0xfee0);

    MEAS_MPAT("TP_ALU_TPPImm_00");
    compFm("TP_ALU_TPPImm_00",(1<<0) , 16, 0x0001,	0x0003,	0x0007,	0x000f,	0x001f,	0x003f,	0x007f,	0x00ff,	0x01ff,	0x03ff,	0x07ff,	0x0fff,	0x1fff,	0x3fff,	0x7fff,	0xffff );
    compFm("TP_ALU_TPPImm_00",(1<<1) , 16, 0x1000,	0x3000,	0x7000,	0xf000,	0xf100,	0xf300,	0xf700,	0xff00,	0xff10,	0xff30,	0xff70,	0xfff0,	0xfff1,	0xfff3,	0xfff7,	0xffff );
    
    MEAS_MPAT("TP_ALU_TPMImm_00");
    compFm("TP_ALU_TPMImm_00",(1<<0) , 16, 0xfffe,	0xfffc,	0xfff8,	0xfff0,	0xffe0,	0xffc0,	0xff80,	0xff00,	0xfe00,	0xfc00,	0xf800,	0xf000,	0xe000,	0xc000,	0x8000,	0x0000);
    compFm("TP_ALU_TPMImm_00",(1<<1) , 16, 0xefff,	0xcfff,	0x8fff,	0x0fff,	0x0eff,	0x0cff,	0x08ff,	0x00ff,	0x00ef,	0x00cf,	0x008f,	0x000f,	0x000e,	0x000c,	0x0008,	0x0000);
                        
}

void testPG_alu_vec()
{
    diag_log("============ %s ============\n", __FUNCTION__);
    DDR_MODE(1);
	int fm_cnt;
	int vec[] = {0xFF,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x90,0x01,0x02,0x03,0x04,0x05,0x06,
    0x07,0x08,0x09,0x00,0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,-1};


	load_pds("diag_pds_data_data16");
    SEND_MPAT("diag_tp.mpa");
//--Array--------------------------------------------------------------------------------------------------------------------
    int idxCnt =0;
    SET_VECTOR(vec, idxCnt);
    MEAS_MPAT("TP_Vector_00");
    compFm("TP_Vector_00",(1<<0)    , 16, 0x00FF, 0x00FF, 0x00FF, 0x00FF, 0x0020, 0x0020, 0x0020, 0x0020, 0x0040, 0x0040, 0x0040, 0x0040, 0x0060, 0x0060, 0x0060, 0x0060 );
    compFm("TP_Vector_00",(1<<1)    , 16, 0x0010, 0x0010, 0x0010, 0x0010, 0x0030, 0x0030, 0x0030, 0x0030, 0x0050, 0x0050, 0x0050, 0x0050, 0x0070, 0x0070, 0x0070, 0x0070 );
 
    MEAS_MPAT("TP_Vector_01");
    compFm("TP_Vector_01",(1<<0)    , 16, 0x0001, 0x0001, 0x0001, 0x0001, 0x0045, 0x0045, 0x0045, 0x0045, 0x0089, 0x0089, 0x0089, 0x0089, 0x00CD, 0x00CD, 0x00CD, 0x00CD );
    compFm("TP_Vector_01",(1<<1)    , 16, 0x0023, 0x0023, 0x0023, 0x0023, 0x0067, 0x0067, 0x0067, 0x0067, 0x00AB, 0x00AB, 0x00AB, 0x00AB, 0x00EF, 0x00EF, 0x00EF, 0x00EF );
 
    MEAS_MPAT("TP_Vector_02");
    compFm("TP_Vector_02",(1<<0)    , 16, 0x00FF, 0x0020, 0x0040, 0x0060, 0x0080, 0x0001, 0x0003, 0x0005, 0x0007, 0x0009, 0x0001, 0x0045, 0x0089, 0x00CD, 0x0011, 0x0033 );
    compFm("TP_Vector_02",(1<<1)    , 16, 0x0010, 0x0030, 0x0050, 0x0070, 0x0090, 0x0002, 0x0004, 0x0006, 0x0008, 0x0000, 0x0023, 0x0067, 0x00AB, 0x00EF, 0x0022, 0x0044 );
 
    MEAS_MPAT("TP_Vector_03");
    compFm("TP_Vector_03",(1<<0)    , 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x0003, 0x0003, 0x0005, 0x0005, 0x0004, 0x0005, 0x0006, 0x0007, 0x0055, 0x0077, 0x0077, 0x0099 );
    compFm("TP_Vector_03",(1<<1)    , 16, 0x8000, 0x8001, 0x8002, 0x8003, 0x0004, 0x0004, 0x0006, 0x0006, 0x8004, 0x8005, 0x8006, 0x8007, 0x0066, 0x0088, 0x0088, 0x00AA );
 
//--Text--------------------------------------------------------------------------------------------------------------------
    SET_VECTOR("diag_vdata.vec", idxCnt, TEXT);
    MEAS_MPAT("TP_Vector_00");
    compFm("TP_Vector_10",(1<<0)    , 16, 0x0000, 0x0000, 0x0000, 0x0000, 0x0020, 0x0020, 0x0020, 0x0020, 0x0040, 0x0040, 0x0040, 0x0040, 0x0060, 0x0060, 0x0060, 0x0060 );
    compFm("TP_Vector_10",(1<<1)    , 16, 0x0010, 0x0010, 0x0010, 0x0010, 0x0030, 0x0030, 0x0030, 0x0030, 0x0050, 0x0050, 0x0050, 0x0050, 0x0070, 0x0070, 0x0070, 0x0070 );
 
    MEAS_MPAT("TP_Vector_01");
    compFm("TP_Vector_11",(1<<0)    , 16, 0x0001, 0x0001, 0x0001, 0x0001, 0x0045, 0x0045, 0x0045, 0x0045, 0x0089, 0x0089, 0x0089, 0x0089, 0x00CD, 0x00CD, 0x00CD, 0x00CD );
    compFm("TP_Vector_11",(1<<1)    , 16, 0x0023, 0x0023, 0x0023, 0x0023, 0x0067, 0x0067, 0x0067, 0x0067, 0x00AB, 0x00AB, 0x00AB, 0x00AB, 0x00EF, 0x00EF, 0x00EF, 0x00EF );
 
    MEAS_MPAT("TP_Vector_02");
    compFm("TP_Vector_12",(1<<0)    , 16, 0x0000, 0x0020, 0x0040, 0x0060, 0x0080, 0x0001, 0x0003, 0x0005, 0x0007, 0x0009, 0x0001, 0x0045, 0x0089, 0x00CD, 0x0011, 0x0033 );
    compFm("TP_Vector_12",(1<<1)    , 16, 0x0010, 0x0030, 0x0050, 0x0070, 0x0090, 0x0002, 0x0004, 0x0006, 0x0008, 0x0000, 0x0023, 0x0067, 0x00AB, 0x00EF, 0x0022, 0x0044 );
 
    MEAS_MPAT("TP_Vector_03");
    compFm("TP_Vector_13",(1<<0)    , 16, 0x0000, 0x0001, 0x0002, 0x0003, 0x0003, 0x0003, 0x0005, 0x0005, 0x0004, 0x0005, 0x0006, 0x0007, 0x0055, 0x0077, 0x0077, 0x0099 );
    compFm("TP_Vector_13",(1<<1)    , 16, 0x8000, 0x8001, 0x8002, 0x8003, 0x0004, 0x0004, 0x0006, 0x0006, 0x8004, 0x8005, 0x8006, 0x8007, 0x0066, 0x0088, 0x0088, 0x00AA );
}


void testPG_alu_cmd()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;
	 SEND_MPAT("diag_cmd.mpa");
    load_pds("diag_pds_data_xy16");
  
 	//SET_REG(ZMAX,0x1F);
    MEAS_MPAT("XY_CMD_00");   
    compFm("XY_CMD_00",(1<<0), 21, 0xfffe,	0x0002,	0x0003,	0x0004,	0x0005,	0xfff9,	0x0007,	0x0008,	0x8001,	0x8002,	0x8003,	0x8004,	0x8005,	0x8006,	0x8007,	0x8008,	0x0011,	0x8008,	0x0012,	0x5555,	0xfffd);
    compFm("XY_CMD_00",(1<<1), 21, 0x8001,	0x7ffd,	0x8003,	0x8004,	0x8005,	0x7ff9,	0x8007,	0x8008,	0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006,	0x0007,	0x0008,	0x8011,	0x0008,	0x0012,	0xbbbb,	0xbbbb);
/*
	load_pds("diag_pds_data_data16");
    MEAS_MPAT("TP_CMD_DBDB2_00");
    compFm("TP_CMD_DBDB2_00",(1<<0), 16, 0x0001, 0xfffd, 0x0002, 0x0003, 0xfffb, 0xfffd, 0x0003, 0x0001, 0xfffd, 0x0002, 0x0003, 0xfffb, 0xfffd, 0x0003, 0xffff, 0x0000);
    compFm("TP_CMD_DBDB2_00",(1<<1), 16, 0x7fff, 0x7ffe, 0x8001, 0x8002, 0x8003, 0x7ffe, 0x7ffd, 0x7fff, 0x7ffe, 0x8001, 0x8002, 0x8003, 0x7ffe, 0x7ffd, 0x0000, 0xffff);		
*/
}

void testPG_alu_cypn()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;

    SEND_MPAT("diag_cypn.mpa");
	//DATA Field
	load_pds("diag_pds_data_xy23");
    MEAS_MPAT("XY_CYPN_00");       //DATA X Y 0~23
    compFm("XY_CYPN_00",(1<<0),24, 0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0xfffe,	0xfffe,	0x0100,	0x0100,	0x0100,	0x0100,	0x1100,	0x1100,	0xfeff,	0xfeff);
    compFm("XY_CYPN_00",(1<<1),24, 0x1000,	0x1000,	0x1000,	0x1000,	0x0000,	0x0000,	0x0000,	0x0000,	0x0000,	0x0000,	0x0000,	0x0000,	0x0000,	0x0000,	0xffff,	0xffff,	0x1000,	0x1000,	0x1000,	0x1000,	0x1010,	0x1010,	0xffef,	0xffef);

	load_pds("diag_pds_data_data18");
	MEAS_MPAT("XY_CYPN_02");
    compFm("XY_CYPN_02",(1<<0),18, 0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0xfffe,	0xfffe,	0xffff,	0x0000,	0x2000,	0x2000);
    compFm("XY_CYPN_02",(1<<1),18, 0x1000,	0x1000,	0x1000,	0x1000,	0x4000,	0x4000,	0x4000,	0x4000,	0x4000,	0x4000,	0x4000,	0x4000,	0xbfff,	0xbfff,	0x0000,	0xffff,	0x4000,	0x1000);

	load_pds("diag_pds_data_z15");
	MEAS_MPAT("Z_CYPN_00");
    compFm("Z_CYPN_00",(1<<0),16, 0x0001,	0x0002,	0x0004,	0x0008,	0x0010,	0x0020,	0x0040,	0x0080,	0x0100,	0x0200,	0x0400,	0x0800,	0x1000,	0x2000,	0x4000,	0x8000);
    compFm("Z_CYPN_00",(1<<1),16, 0xfffe,	0xfffd,	0xfffb,	0xfff7,	0xffef,	0xffdf,	0xffbf,	0xff7f,	0xfeff,	0xfdff,	0xfbff,	0xf7ff,	0xefff,	0xdfff,	0xbfff,	0x7fff);
  
	diag_fmDrvMode(1);	
    load_pds("diag_pds_addr_xy23");
    MEAS_MPAT("XY_CYPN_00");       
    compFm("XY_CYPN_03",(1<<9), 24, 0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0xfffefffe,	0xfffefffe, 
	                                0x01000100, 0x01000100, 0x01000100, 0x01000100, 0x11001100, 0x11001100, 0xfefffeff, 0xfefffeff);
    compFm("XY_CYPN_03",(1<<10),24, 0x10001000,	0x10001000,	0x10001000,	0x10001000,	0x00000000,	0x00000000,	0x00000000,	0x00000000,	0x00000000,	0x00000000,	0x00000000,	0x00000000,	0x00000000,	0x00000000,	0xffffffff,	0xffffffff,
	                                0x10001000, 0x10001000, 0x10001000, 0x10001000, 0x10101010, 0x10101010, 0xffefffef, 0xffefffef);

    load_pds("diag_pds_addr_z15");
    MEAS_MPAT("Z_CYPN_01");       
    compFm("Z_CYPN_01",(1<<9), 16, 0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,0x00010001,0x00010001);
    compFm("Z_CYPN_01",(1<<10),16, 0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe,	0xfffefffe);

    load_pds("diag_pds_addr_ctrl");
    MEAS_MPAT("XY_CYPN_04");       
    compFm("XY_CYPN_04",(1<<9),16, 0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0x00010001,	0xffffffff,	0x00000000,	0xfffefffe,	0xfffefffe);
    compFm("XY_CYPN_04",(1<<10),16, 0x000b000b,	0x00050005,	0x80038003,	0x40014001,	0xa001a001,	0x50015001,	0x28012801,	0x14011401,	0x0a010a01,	0x05010501,	0x02810281,	0x01410141,	0x00000000,	0xffffffff,	0xffd6ffd6,	0xffeaffea);
    
	diag_fmDrvMode(0);

}

void testPG_alu_scr()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;

	diag_fmDrvMode(0);

    SEND_MPAT("diag_scr.mpa");
    load_pds("pds_data_data16");

//    MEAS_MPAT(XY_SCR_00);       
//    fm_cnt=diag_readFmData();
//    compFm("XY_SCR_00",(1<<0),16, 0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001);
//    compFm("XY_SCR_00",(1<<1),16, 0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000);

	load_scr("diag_bit_scr_00");   
    MEAS_MPAT("XY_SCR_01");       
    fm_cnt=diag_readFmData();
    compFm("XY_SCR_01",(1<<0),16, 0x8000,	0x8001,	0x8001,	0x8000,	0x8000,	0x8002,	0x8002,	0x8002,	0x8000,	0x8000,	0x8000,	0x8004,	0x8000,	0x8008,	0x8000,	0x8008);
 
    MEAS_MPAT("XY_SCR_02");       
    fm_cnt=diag_readFmData();
    compFm("XY_SCR_02",(1<<0),16, 0x8000,	0x8010,	0x8000,	0x8000,	0x8000,	0x8000,	0x8020,	0x8000,	0x8000,	0x8040,	0x8040,	0x8040,	0x8000,	0x8080,	0x8080,	0x8080);  
    MEAS_MPAT("XY_SCR_03");       
    fm_cnt=diag_readFmData();
    compFm("XY_SCR_03",(1<<0),16, 0x8000,	0x8000,	0x8100,	0x8100,	0x8000,	0x8000,	0x8200,	0x8000,	0x8000,	0x8000,	0x8400,	0x8400,	0x8000,	0x8000,	0x8000,	0x8800);
    MEAS_MPAT("XY_SCR_04");       
    fm_cnt=diag_readFmData();
    compFm("XY_SCR_04",(1<<0),16, 0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0xa000,	0x8000,	0xc000,	0xc000,	0xc000,	0x8000,	0x0000,	0x0000,	0x8000);
	load_scr("diag_bit_scr_01");   
    MEAS_MPAT("XY_SCR_01");       
    fm_cnt=diag_readFmData();
    compFm("XY_SCR_01",(1<<0),16, 0x8000,	0x8000,	0x8001,	0x8000,	0x8000,	0x8000,	0x8002,	0x8000,	0x8000,	0x8004,	0x8004,	0x8000,	0x8000,	0x8008,	0x8008,	0x8008);
    MEAS_MPAT("XY_SCR_02");       
    fm_cnt=diag_readFmData();
    compFm("XY_SCR_02",(1<<0),16, 0x8000,	0x8010,	0x8010,	0x8000,	0x8000,	0x8020,	0x8000,	0x8000,	0x8000,	0x8040,	0x8000,	0x8040,	0x8000,	0x8080,	0x8000,	0x8080);	
    MEAS_MPAT("XY_SCR_03");       
    fm_cnt=diag_readFmData();
    compFm("XY_SCR_03",(1<<0),16, 0x8000,	0x8100,	0x8100,	0x8100,	0x8000,	0x8200,	0x8200,	0x8200,	0x8000,	0x8400,	0x8400,	0x8400,	0x8000,	0x8800,	0x8800,	0x8800);	
    MEAS_MPAT("XY_SCR_04");       
    fm_cnt=diag_readFmData();
    compFm("XY_SCR_04",(1<<0),16, 0x8000,	0x9000,	0x9000,	0x9000,	0x8000,	0xa000,	0xa000,	0xa000,	0x8000,	0x4000,	0x8000,	0x4000,	0x8000,	0x8000,	0x8000,	0x8000);
	load_scr("diag_bit_scr_02");   
    MEAS_MPAT("XY_SCR_01");      
    fm_cnt=diag_readFmData();
    compFm("XY_SCR_01",(1<<0),16, 0x8000,	0x8001,	0x8000,	0x8001,	0x8000,	0x8002,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8008,	0x8000,	0x8008);
    MEAS_MPAT("XY_SCR_02");       
    fm_cnt=diag_readFmData();
    compFm("XY_SCR_02",(1<<0),16, 0x8000,	0x8010,	0x8000,	0x8010,	0x8000,	0x8000,	0x8000,	0x8020,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8080);
	ENABLE_SCRAM(false);

}


void testDR_pin_chk()
{
	    diag_log("============ %s ============\n", __FUNCTION__);
	
 	    char addrA0,addrA1,addrA2,addrA3;
 	    char addrB0,addrB1,addrB2,addrB3;
 	    char clkA0;
 	    char clkB0;
 	    char scan0,scan1,scan2,scan3;
 	    int fcnt=0;
   
        load_feset_AFIXH();
        SEND_MPAT("diag_opcode.mpa");
 	    MEAS_MPAT("OPCODE_NOP_00");
 	    

 	     	    
 	    addrA0 = readPG((1<<9) | 323,1);
 	    addrA1 = readPG((1<<9) | 324,1);
 	    addrA2 = readPG((1<<9) | 325,1);
 	    addrA3 = readPG((1<<9) | 317,1);
 	    
 	    addrB0 = readPG((1<<9) | 307,1);
 	    addrB1 = readPG((1<<9) | 308,1);
 	    addrB2 = readPG((1<<9) | 309,1);
 	    addrB3 = readPG((1<<9) | 310,1);
 	   
 	    clkA0 = readPG((1<<9) | 316,1);
 	    clkB0 = readPG((1<<9) | 318,1);
 	    
 	    scan0 = readPG((1<<9) | 329,1);
 	    scan1 = readPG((1<<9) | 330,1);
 	    scan2 = readPG((1<<9) | 331,1);
 	    scan3 = readPG((1<<9) | 332,1);
 	       
 	    for(int i=0; i<8;i++)
 	    {
 	    	if (((addrA0 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "ADDR [%d] CH A HIGH FAIL \n", i);
 	    		fcnt++;
 	    	} 
 	    	if (((addrA1 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "ADDR [%d] CH A HIGH FAIL \n", i+8);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrA2 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "ADDR [%d] CH A HIGH FAIL \n", i+16);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrA3 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "ADDR [%d] CH A HIGH FAIL \n", i+24);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrB0 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "ADDR [%d] CH B HIGH FAIL \n", i);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrB1 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "ADDR [%d] CH B HIGH FAIL \n", i+8);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrB2 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "ADDR [%d] CH B HIGH FAIL \n", i+16);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrB3 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "ADDR [%d] CH B HIGH FAIL \n", i+24);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((clkA0 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "CLK [%d] CH A HIGH FAIL \n", i);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((clkB0 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "CLK [%d] CH B HIGH FAIL \n", i);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((scan0 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "SCAN [%d] HIGH FAIL \n", i);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((scan1 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "SCAN [%d] HIGH FAIL \n", i+8);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((scan2 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "SCAN [%d] HIGH FAIL \n", i+16);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((scan3 >> i) & 0x1) == 0x0)
 	    	{
 	    		diag_log_pf ( 0, "SCAN [%d] HIGH FAIL \n", i+24);
 	    		fcnt++; 	    		
 	    	}
 	    }
 	  
      
        load_feset_AFIXL();
 	    MEAS_MPAT("OPCODE_NOP_00");
 	    
 	    addrA0 = readPG((1<<9) | 323,1);
 	    addrA1 = readPG((1<<9) | 324,1);
 	    addrA2 = readPG((1<<9) | 325,1);
 	    addrA3 = readPG((1<<9) | 317,1);
 	    
 	    addrB0 = readPG((1<<9) | 307,1);
 	    addrB1 = readPG((1<<9) | 308,1);
 	    addrB2 = readPG((1<<9) | 309,1);
 	    addrB3 = readPG((1<<9) | 310,1);
 	   
 	    clkA0 = readPG((1<<9) | 316,1);
 	    clkB0 = readPG((1<<9) | 318,1);
 	    
 	    scan0 = readPG((1<<9) | 329,1);
 	    scan1 = readPG((1<<9) | 330,1);
 	    scan2 = readPG((1<<9) | 331,1);
 	    scan3 = readPG((1<<9) | 332,1);
 	    
 	     	    
 	    for(int i=0; i<8;i++)
 	    {
 	    		
 	   if (((addrA0 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "ADDR [%d] CH A LOW FAIL \n", i);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrA1 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "ADDR [%d] CH A LOW FAIL \n", i+8);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrA2 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "ADDR [%d] CH A LOW FAIL \n", i+16);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrA3 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "ADDR [%d] CH A LOW FAIL \n", i+24);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrB0 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "ADDR [%d] CH B LOW FAIL \n", i);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrB1 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "ADDR [%d] CH B LOW FAIL \n", i+8);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrB2 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "ADDR [%d] CH B LOW FAIL \n", i+16);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((addrB3 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "ADDR [%d] CH B LOW FAIL \n", i+24);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((clkA0 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "CLK [%d] CH A LOW FAIL \n", i);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((clkB0 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "CLK [%d] CH B LOW FAIL \n", i);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((scan0 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "SCAN [%d] LOW FAIL \n", i);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((scan1 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "SCAN [%d] LOW FAIL \n", i+8);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((scan2 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "SCAN [%d] LOW FAIL \n", i+16);
 	    		fcnt++; 	    		
 	    	}
 	    	if (((scan3 >> i) & 0x1) == 0x1)
 	    	{
 	    		diag_log_pf (0, "SCAN [%d] LOW FAIL \n", i+24);
 	    		fcnt++; 	    		
 	    	}
 	    }
 	    
 	 if (fcnt == 0)
    {
        diag_log_pf(1, "[%20s]", "DR PIN CHK PASS");
    }
 	      
}
void testPG_shmoo()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;
//	tgDelaySet();
    double strbA, strbB;
		
	load_pds("diag_pds_data_data16");
	SEND_MPAT("diag_out.mpa");
    //writeCpeDelay(2,2);

diag_log("                                                                                                   11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111112\n");
diag_log("         11111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001111111111222222222233333333334444444444555555555566666666667777777777888888888899999999990\n");
diag_log("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
diag_log("+---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n");

	for(double strb0=0.0;strb0<200.0;strb0=strb0+2.0)
	{
        load_ts("diag_ts");
        strbA = strb0;

        STRB_EDGE(1,strbA, 98);
		MEAS_MPAT("SHMOO_NOP_00");
		pg_shmoo_chk(0, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f,0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);
	}

	diag_log("\n");
	for(double strb1=0.0;strb1<200.0;strb1=strb1+2.0)
	{
         strbB = strb1;

        STRB_EDGE(1,52, strbB);
		MEAS_MPAT("SHMOO_NOP_00");
		pg_shmoo_chk(1, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f,0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);
	}
	diag_log("\n");

}



void testPG_shmoo_cal()
{
    diag_log("========================================\n", __FUNCTION__);
    diag_log("              %s                     \n", __FUNCTION__);
    diag_log("========================================\n", __FUNCTION__);
	int fm_cnt;
//	tgDelaySet();
    double strbA, strbB;
	int set_cal = 0;	
	

	load_pds("diag_pds_data_data16");
	SEND_MPAT("diag_out.mpa");
    writeCpeDelay(2,2);

diag_log("                                                                                                   11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111112\n");
diag_log("         11111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001111111111222222222233333333334444444444555555555566666666667777777777888888888899999999990\n");
diag_log("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
diag_log("+---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n");
 
    for(int ical = 0; ical <1; ical++)
    {
        set_cal = ical + (ical * 16);
        diag_log("cpe skew = %d value = %x\n", ical , set_cal );
       // writeDskDelay(set_cal);

     	for(double strb0=0.0;strb0<200.0;strb0=strb0+1.0)
    	{
            load_ts("diag_ts");
            strbA = strb0;

            STRB_EDGE(1,strbA, 98);
    
    		MEAS_MPAT("SHMOO_NOP_00");
    		pg_strb_cal(0, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f,0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);
    	}

    	diag_log("\n");
    	for(double strb1=0.0;strb1<200.0;strb1=strb1+1.0)
    	{
           load_ts("diag_ts");

           strbB = strb1;

            STRB_EDGE(1,52, strbB);

    		MEAS_MPAT("SHMOO_NOP_00");
    		pg_strb_cal(1, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f,0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);
    	}
    	diag_log("\n");
    	for(double strb0=0.0;strb0<200.0;strb0=strb0+1.0)
    	{
            load_ts("diag_ts");

            strbA = strb0;

            STRB_EDGE(1,strbA, 98);
    
    		MEAS_MPAT("SHMOO_NOP_00");
    		pg_strb_cal(2, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f,0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);
    	}

    	diag_log("\n");
    	for(double strb1=0.0;strb1<200.0;strb1=strb1+1.0)
    	{
           load_ts("diag_ts");

           strbB = strb1;

           STRB_EDGE(1,52, strbB);

    		MEAS_MPAT("SHMOO_NOP_00");
    		pg_strb_cal(3, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f,0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);
    	}
    	diag_log("\n");
    }
    diag_msg(1, "[PG SHMOO CAL]");
}

void testPG_tdr()
{
    diag_log("========================================\n", __FUNCTION__);
    diag_log("              %s                     \n", __FUNCTION__);
    diag_log("========================================\n", __FUNCTION__);
	int fm_cnt;
	double rate,strb0,strb1,set_vth;
	double vth_initial;
//	tgDelaySet(); 
		
	load_pds("diag_pds_data_data16");
	SEND_MPAT("diag_out.mpa");

	int io_pf_value[8];
	int step_cnt=0;
	double x_vth[300];

	int io_cnt=64;
	int xsize=300;
	int ysize=300;
	int ymsize=0;

	char io_pf_result[xsize][ysize][io_cnt];

	double xstart_val = 1.5  ;
	double xend_val   = 0.1 ;
	double xstep_val  = 0.05 ;

	double ystart_val = 0.0 ;
	double yend_val   = 200.0 ;
	double ystep_val  = 2.0  ;
	
	
	int y_valcnt =0;
	double strbs=0.0;
	
	xsize=0;
	ysize=0;
	
diag_log("                                                                                                   11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111112\n");
diag_log("         11111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001111111111222222222233333333334444444444555555555566666666667777777777888888888899999999990\n");
diag_log("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
diag_log("+---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n");
  for(set_vth=xstart_val; set_vth > xend_val; set_vth=set_vth-xstep_val )    
  //    for(set_vth=xstart_val; set_vth < xend_val; set_vth=set_vth+xstep_val )    
	{
         VIH ( IO , 1.0 ,false  );    // DR pin VIH set
         VIL ( IO , 0.0 ,false );    // DR pin VIL set
         VTH ( set_vth);         // comparator VTH set  

		for( strb0=ystart_val;strb0<yend_val;strb0=strb0+ystep_val)
		{            
             load_ts("diag_ts");
             STRB_EDGE(1,strb0, 98);

			MEAS_MPAT("SHMOO_TDR_00", false, false);

			writePG(1<<9 | 28,0x0,1);	
			for(int io_cnt=0;io_cnt<8;io_cnt++)
				io_pf_value[io_cnt] = readPG((1<<9) | 262+io_cnt,1);
	
			for(int io_cnt=0;io_cnt<8;io_cnt++)
			{
				for(int sloop=0;sloop<8;sloop++)
				{
					if(((io_pf_value[io_cnt]>> sloop) & 0x1))
						io_pf_result[xsize][ysize][(io_cnt*8)+sloop]='P';
					else
						io_pf_result[xsize][ysize][(io_cnt*8)+sloop] = '*';
				}
			}
			ysize++;
			ymsize = ysize;
		}
		ysize=0;
		xsize++;
		x_vth[xsize-1] = set_vth;		
	}
		
    for(int iostart=0; iostart<64; iostart++)
	{
		diag_log("io pin = %d \n",iostart);
		for(double ystart=ystart_val; ystart<yend_val; ystart=ystart+ystep_val)
		{	
			if((y_valcnt+1)%5==1)
				diag_log("%3.1f ",ystart);
		y_valcnt++;
		}		
		y_valcnt=0;
		diag_log("\n");

		for(int ystart=0; ystart<ymsize; ystart++)
		{
			if((ystart+1)%5==1)
				
				diag_log("+");
			else
				diag_log("-");
		}	
		diag_log("\n");
		
		for(int xstart=0; xstart<xsize; xstart++)
		{ 	
			for(int ystart=0; ystart<ymsize; ystart++)
			{
				diag_log("%c",io_pf_result[xstart][ystart][iostart]); 
			}
			diag_log(" %.3f[%d] ",x_vth[xstart],xstart);
			diag_log("\n");
		}
		diag_log("\n");
	}
    diag_msg(1, "[PG TDR]");
}


void testPG_tdr_low()
{
    diag_log("========================================\n", __FUNCTION__);
    diag_log("              %s                     \n", __FUNCTION__);
    diag_log("========================================\n", __FUNCTION__);
	int fm_cnt;
	double rate,strb0,strb1,set_vth;
	double vth_initial;
//	tgDelaySet(); 
		
	load_pds("diag_pds_data_data16");
	SEND_MPAT("diag_out.mpa");

	int io_pf_value[8];
	int step_cnt=0;
	double x_vth[300];

	int io_cnt=64;
	int xsize=300;
	int ysize=300;
	int ymsize=0;

	char io_pf_result[xsize][ysize][io_cnt];

	double xstart_val = 1.5  ;
	double xend_val   = 0.1 ;
	double xstep_val  = 0.05 ;

	double ystart_val = 0.0 ;
	double yend_val   = 200.0 ;
	double ystep_val  = 2.0  ;
	
	
	int y_valcnt =0;
	double strbs=0.0;
	
	xsize=0;
	ysize=0;
	
diag_log("                                                                                                   11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111112\n");
diag_log("         11111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001111111111222222222233333333334444444444555555555566666666667777777777888888888899999999990\n");
diag_log("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
diag_log("+---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n");
  for(set_vth=xstart_val; set_vth > xend_val; set_vth=set_vth-xstep_val )    
  //    for(set_vth=xstart_val; set_vth < xend_val; set_vth=set_vth+xstep_val )    
	{
         VIH ( IO , 1.0 ,false  );    // DR pin VIH set
         VIL ( IO , 0.0 ,false );    // DR pin VIL set
         VTH ( set_vth);         // comparator VTH set  

		for( strb0=ystart_val;strb0<yend_val;strb0=strb0+ystep_val)
		{            
             load_ts("diag_ts");
             STRB_EDGE(1,strb0, 98);

			MEAS_MPAT("SHMOO_TDR_01", false, false);

			writePG(1<<9 | 28,0x0,1);	
			for(int io_cnt=0;io_cnt<8;io_cnt++)
				io_pf_value[io_cnt] = readPG((1<<9) | 262+io_cnt,1);
	
			for(int io_cnt=0;io_cnt<8;io_cnt++)
			{
				for(int sloop=0;sloop<8;sloop++)
				{
					if(((io_pf_value[io_cnt]>> sloop) & 0x1))
						io_pf_result[xsize][ysize][(io_cnt*8)+sloop]='P';
					else
						io_pf_result[xsize][ysize][(io_cnt*8)+sloop] = '*';
				}
			}
			ysize++;
			ymsize = ysize;
		}
		ysize=0;
		xsize++;
		x_vth[xsize-1] = set_vth;		
	}
		
    for(int iostart=0; iostart<64; iostart++)
	{
		diag_log("io pin = %d \n",iostart);
		for(double ystart=ystart_val; ystart<yend_val; ystart=ystart+ystep_val)
		{	
			if((y_valcnt+1)%5==1)
				diag_log("%3.1f ",ystart);
		y_valcnt++;
		}		
		y_valcnt=0;
		diag_log("\n");

		for(int ystart=0; ystart<ymsize; ystart++)
		{
			if((ystart+1)%5==1)
				
				diag_log("+");
			else
				diag_log("-");
		}	
		diag_log("\n");
		
		for(int xstart=0; xstart<xsize; xstart++)
		{ 	
			for(int ystart=0; ystart<ymsize; ystart++)
			{
				diag_log("%c",io_pf_result[xstart][ystart][iostart]); 
			}
			diag_log(" %.3f[%d] ",x_vth[xstart],xstart);
			diag_log("\n");
		}
		diag_log("\n");
	}
    diag_msg(1, "[PG TDR]");
}
//////////////////////////////////////////////////////
//                     PE CAL                       //
//////////////////////////////////////////////////////
#define ROM_MAX_SIZE  0x8000
#define CAL_ADC0_ADDR  0x100
#define CAL_ADC1_ADDR  0x200
#define CAL_PE_VIL_ADDR   0x1000
#define CAL_PE_VIH_ADDR   0x2000
#define CAL_PE_VT_ADDR   0x3000
#define CAL_DPS_ADDR     0x5000

float g_adc0Gain[8] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
float g_adc0Offset[8] = {0,0,0,0,0,0,0,0};
float g_adc1Gain[8] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
float g_adc1Offset[8] = {0,0,0,0,0,0,0,0};
float g_dacGain[2] = {1.0,1.0};
float g_dacOffset[2] = {0,0};
float g_RData[8] = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};

float g_dpsVsGain[4] = {1.0,1.0,1.0,1.0,};
float g_dpsVsOffset[4] = {0,0,0,0};

float g_dpsVsGain2[4] = {1.0,1.0,1.0,1.0,};
float g_dpsVsOffset2[4] = {0,0,0,0};

float g_dpsIMGain[4] = {1.0,1.0,1.0,1.0,};
float g_dpsIMOffset[4] = {0,0,0,0};

float g_dpsOVGain[4] = {1.0,1.0,1.0,1.0,};
float g_dpsOVOffset[4] = {0,0,0,0};

float g_dpsUVGain[4] = {1.0,1.0,1.0,1.0,};
float g_dpsUVOffset[4] = {0,0,0,0};

float g_dpsOCGain[4] = {1.0,1.0,1.0,1.0,};
float g_dpsOCOffset[4] = {0,0,0,0};

float g_peGain[176] = {1.0,};
float g_peOffset[176] = {0.0,};
float g_data[176] =  {0.0,};
float g_data2[176] =  {0.0,};

int  chLookup[8][4][6] =   {{{0,32,64,88,120,152}, { 8,40,72, 96,128,160},{16,48,80,104,136,168},{24,56,-1,112,144,-1}},
                            {{1,33,65,89,121,153}, { 9,41,73, 97,129,161},{17,49,81,105,137,169},{25,57,-1,113,145,-1}},
                            {{2,34,66,90,122,154}, {10,42,74, 98,130,162},{18,50,82,106,138,170},{26,58,-1,114,146,-1}},
                            {{3,35,67,91,123,155}, {11,43,75, 99,131,163},{19,51,83,107,139,171},{27,59,-1,115,147,-1}},
                            {{4,36,68,92,124,156}, {12,44,76,100,132,164},{20,52,84,108,140,172},{28,60,-1,116,148,-1}},
                            {{5,37,69,93,125,157}, {13,45,77,101,133,165},{21,53,85,109,141,173},{29,61,-1,117,149,-1}},
                            {{6,38,70,94,126,158}, {14,46,78,102,134,166},{22,54,86,110,142,174},{30,62,-1,118,150,-1}},
                            {{7,39,71,95,127,159}, {15,47,79,103,135,167},{23,55,87,111,143,175},{31,63,-1,119,151,-1}}};

FILE * g_pFileLog = NULL;
FILE * g_pFileCalData = NULL;
std::stringstream g_strLog;
bool g_bCalResult = true;
bool g_bDpsCalResult = true;

void PeCalFileOpen()
{
    char outstr[200];
    time_t t;
    struct tm *tmp;

    t = time(NULL);
    tmp = localtime(&t);
    if (tmp == NULL) {
        perror("localtime");
        exit(EXIT_FAILURE);
    }

    if (strftime(outstr, sizeof(outstr), "%Y%m%d_%H%M%S", tmp) == 0) {
        fprintf(stderr, "strftime returned 0");
        exit(EXIT_FAILURE);
    }

    std::string strFile = "/star/diag/cal/";
//    strFile += outstr;
    strFile += "pecal.log";
    printf("Result log file is %s\n", strFile.c_str());
    g_pFileLog = fopen(strFile.c_str(), "wt+");
    if(g_pFileLog == NULL) printf("Cal log file open fail.\n");
    g_pFileCalData = fopen("/star/diag/cal/pecal.data", "wb");
    if(g_pFileCalData == NULL) printf("Cal data file open fail.\n");

    SEND_MPAT("cal.mpa");
}

void PeCalFileClose()
{
    if(g_pFileLog) fclose(g_pFileLog);
    if(g_pFileCalData) fclose(g_pFileCalData);
}

void DpsCalFileOpen()
{
    char outstr[200];
    time_t t;
    struct tm *tmp;

    t = time(NULL);
    tmp = localtime(&t);
    if (tmp == NULL) {
        perror("localtime");
        exit(EXIT_FAILURE);
    }

    if (strftime(outstr, sizeof(outstr), "%Y%m%d_%H%M%S", tmp) == 0) {
        fprintf(stderr, "strftime returned 0");
        exit(EXIT_FAILURE);
    }

    std::string strFile = "/star/diag/cal/";
//    strFile += outstr;
    strFile += "dpscal.log";
    printf("Result log file is %s\n", strFile.c_str());
    g_pFileLog = fopen(strFile.c_str(), "wt+");
    if(g_pFileLog == NULL) printf("Cal log file open fail.\n");
    g_pFileCalData = fopen("/star/diag/cal/dpscal.data", "wb");
    if(g_pFileCalData == NULL) printf("Cal data file open fail.\n");

    //SEND_MPAT("cal.mpa");
}

void DpsCalFileClose()
{
    if(g_pFileLog) fclose(g_pFileLog);
    if(g_pFileCalData) fclose(g_pFileCalData);
}



void load_feset_cal_H (const char* feset_name)
{
        TSSET (  ADDR0, FIXH, ECLK1 );     
        TSSET (  ADDR1, FIXH, ECLK1 );     
        TSSET (  ADDR2, FIXH, ECLK1 );     
        TSSET (  ADDR3, FIXH, ECLK1 );     
        TSSET (  ADDR4, FIXH, ECLK1 );     
        TSSET (  ADDR5, FIXH, ECLK1 );     
        TSSET (  ADDR6, FIXH, ECLK1 );     
        TSSET (  ADDR7, FIXH, ECLK1 );     
        TSSET (  ADDR8, FIXH, ECLK1 );     
        TSSET (  ADDR9, FIXH, ECLK1 );     
        TSSET ( ADDR10, FIXH, ECLK1 );     
        TSSET ( ADDR11, FIXH, ECLK1 );     
        TSSET ( ADDR12, FIXH, ECLK1 );     
        TSSET ( ADDR13, FIXH, ECLK1 );     
        TSSET ( ADDR14, FIXH, ECLK1 );     
        TSSET ( ADDR15, FIXH, ECLK1 );     
        TSSET ( ADDR16, FIXH, ECLK1 );     
        TSSET ( ADDR17, FIXH, ECLK1 );     
        TSSET ( ADDR18, FIXH, ECLK1 );     
        TSSET ( ADDR19, FIXH, ECLK1 );     
        TSSET ( ADDR20, FIXH, ECLK1 );     
        TSSET ( ADDR21, FIXH, ECLK1 );     
        TSSET ( ADDR22, FIXH, ECLK1 );     
        TSSET ( ADDR23, FIXH, ECLK1 );     
        TSSET ( ADDR24, FIXH, ECLK1 );     
        TSSET ( ADDR25, FIXH, ECLK1 );     
        TSSET ( ADDR26, FIXH, ECLK1 );     
        TSSET ( ADDR27, FIXH, ECLK1 );     
        TSSET ( ADDR28, FIXH, ECLK1 );     
        TSSET ( ADDR29, FIXH, ECLK1 );     
        TSSET ( ADDR30, FIXH, ECLK1 );     
        TSSET ( ADDR31, FIXH, ECLK1 ); 

        TSSET (  CLK0,FIXH, ECLK1 );     
        TSSET (  CLK1,FIXH, ECLK1 );     
        TSSET (  CLK2,FIXH, ECLK1 );     
        TSSET (  CLK3,FIXH, ECLK1 );     
        TSSET (  CLK4,FIXH, ECLK1 );     
        TSSET (  CLK5,FIXH, ECLK1 );     
        TSSET (  CLK6,FIXH, ECLK1 );     
        TSSET (  CLK7,FIXH, ECLK1 );     

        TSSET (  SCAN0,FIXH, ECLK1 );   
        TSSET (  SCAN1,FIXH, ECLK1 );   
        TSSET (  SCAN2,FIXH, ECLK1 );   
        TSSET (  SCAN3,FIXH, ECLK1 );   
        TSSET (  SCAN4,FIXH, ECLK1 );   
        TSSET (  SCAN5,FIXH, ECLK1 );   
        TSSET (  SCAN6,FIXH, ECLK1 );   
        TSSET (  SCAN7,FIXH, ECLK1 );   
        TSSET (  SCAN8,FIXH, ECLK1 );   
        TSSET (  SCAN9,FIXH, ECLK1 );   
        TSSET ( SCAN10,FIXH, ECLK1 );   
        TSSET ( SCAN11,FIXH, ECLK1 );   
        TSSET ( SCAN12,FIXH, ECLK1 );   
        TSSET ( SCAN13,FIXH, ECLK1 );   
        TSSET ( SCAN14,FIXH, ECLK1 );   
        TSSET ( SCAN15,FIXH, ECLK1 );   
        TSSET ( SCAN16,FIXH, ECLK1 );   
        TSSET ( SCAN17,FIXH, ECLK1 );   
        TSSET ( SCAN18,FIXH, ECLK1 );   
        TSSET ( SCAN19,FIXH, ECLK1 );   
        TSSET ( SCAN20,FIXH, ECLK1 );   
        TSSET ( SCAN21,FIXH, ECLK1 );   
        TSSET ( SCAN22,FIXH, ECLK1 );   
        TSSET ( SCAN23,FIXH, ECLK1 );   
        TSSET ( SCAN24,FIXH, ECLK1 );   
        TSSET ( SCAN25,FIXH, ECLK1 );   
        TSSET ( SCAN26,FIXH, ECLK1 );   
        TSSET ( SCAN27,FIXH, ECLK1 );   
        TSSET ( SCAN28,FIXH, ECLK1 );   
        TSSET ( SCAN29,FIXH, ECLK1 );   
        TSSET ( SCAN30,FIXH, ECLK1 );     
        TSSET ( SCAN31,FIXH, ECLK1 );  

        TSSET (  IO0, FIXH, ECLK1 );   
        TSSET (  IO1, FIXH, ECLK1 );   
        TSSET (  IO2, FIXH, ECLK1 );   
        TSSET (  IO3, FIXH, ECLK1 );   
        TSSET (  IO4, FIXH, ECLK1 );   
        TSSET (  IO5, FIXH, ECLK1 );   
        TSSET (  IO6, FIXH, ECLK1 );   
        TSSET (  IO7, FIXH, ECLK1 );   
        TSSET (  IO8, FIXH, ECLK1 );   
        TSSET (  IO9, FIXH, ECLK1 );   
        TSSET ( IO10, FIXH, ECLK1 );   
        TSSET ( IO11, FIXH, ECLK1 );   
        TSSET ( IO12, FIXH, ECLK1 );   
        TSSET ( IO13, FIXH, ECLK1 );  
        TSSET ( IO14, FIXH, ECLK1 );   
        TSSET ( IO15, FIXH, ECLK1 );   
        TSSET ( IO16, FIXH, ECLK1 );   
        TSSET ( IO17, FIXH, ECLK1 );   
        TSSET ( IO18, FIXH, ECLK1 );   
        TSSET ( IO19, FIXH, ECLK1 );   
        TSSET ( IO20, FIXH, ECLK1 );   
        TSSET ( IO21, FIXH, ECLK1 );   
        TSSET ( IO22, FIXH, ECLK1 );   
        TSSET ( IO23, FIXH, ECLK1 );   
        TSSET ( IO24, FIXH, ECLK1 );   
        TSSET ( IO25, FIXH, ECLK1 );   
        TSSET ( IO26, FIXH, ECLK1 );   
        TSSET ( IO27, FIXH, ECLK1 );   
        TSSET ( IO28, FIXH, ECLK1 );   
        TSSET ( IO29, FIXH, ECLK1 );   
        TSSET ( IO30, FIXH, ECLK1 );   
        TSSET ( IO31, FIXH, ECLK1 );   

        TSSET (  DRE, FIXH);
        TSSET (  DRE_B, FIXH);
        //TSSET ( STRB, NRZB, ECLK1);
}

void load_feset_cal_L (const char* feset_name)
{
        TSSET (  ADDR0, FIXL, ECLK1 );     
        TSSET (  ADDR1, FIXL, ECLK1 );     
        TSSET (  ADDR2, FIXL, ECLK1 );     
        TSSET (  ADDR3, FIXL, ECLK1 );     
        TSSET (  ADDR4, FIXL, ECLK1 );     
        TSSET (  ADDR5, FIXL, ECLK1 );     
        TSSET (  ADDR6, FIXL, ECLK1 );     
        TSSET (  ADDR7, FIXL, ECLK1 );     
        TSSET (  ADDR8, FIXL, ECLK1 );     
        TSSET (  ADDR9, FIXL, ECLK1 );     
        TSSET ( ADDR10, FIXL, ECLK1 );     
        TSSET ( ADDR11, FIXL, ECLK1 );     
        TSSET ( ADDR12, FIXL, ECLK1 );     
        TSSET ( ADDR13, FIXL, ECLK1 );     
        TSSET ( ADDR14, FIXL, ECLK1 );     
        TSSET ( ADDR15, FIXL, ECLK1 );     
        TSSET ( ADDR16, FIXL, ECLK1 );     
        TSSET ( ADDR17, FIXL, ECLK1 );     
        TSSET ( ADDR18, FIXL, ECLK1 );     
        TSSET ( ADDR19, FIXL, ECLK1 );     
        TSSET ( ADDR20, FIXL, ECLK1 );     
        TSSET ( ADDR21, FIXL, ECLK1 );     
        TSSET ( ADDR22, FIXL, ECLK1 );     
        TSSET ( ADDR23, FIXL, ECLK1 );     
        TSSET ( ADDR24, FIXL, ECLK1 );     
        TSSET ( ADDR25, FIXL, ECLK1 );     
        TSSET ( ADDR26, FIXL, ECLK1 );     
        TSSET ( ADDR27, FIXL, ECLK1 );     
        TSSET ( ADDR28, FIXL, ECLK1 );     
        TSSET ( ADDR29, FIXL, ECLK1 );     
        TSSET ( ADDR30, FIXL, ECLK1 );     
        TSSET ( ADDR31, FIXL, ECLK1 ); 

        TSSET (  CLK0,FIXL, ECLK1 );     
        TSSET (  CLK1,FIXL, ECLK1 );     
        TSSET (  CLK2,FIXL, ECLK1 );     
        TSSET (  CLK3,FIXL, ECLK1 );     
        TSSET (  CLK4,FIXL, ECLK1 );     
        TSSET (  CLK5,FIXL, ECLK1 );     
        TSSET (  CLK6,FIXL, ECLK1 );     
        TSSET (  CLK7,FIXL, ECLK1 );     

        TSSET (  SCAN0,FIXL, ECLK1 );   
        TSSET (  SCAN1,FIXL, ECLK1 );   
        TSSET (  SCAN2,FIXL, ECLK1 );   
        TSSET (  SCAN3,FIXL, ECLK1 );   
        TSSET (  SCAN4,FIXL, ECLK1 );   
        TSSET (  SCAN5,FIXL, ECLK1 );   
        TSSET (  SCAN6,FIXL, ECLK1 );   
        TSSET (  SCAN7,FIXL, ECLK1 );   
        TSSET (  SCAN8,FIXL, ECLK1 );   
        TSSET (  SCAN9,FIXL, ECLK1 );   
        TSSET ( SCAN10,FIXL, ECLK1 );   
        TSSET ( SCAN11,FIXL, ECLK1 );   
        TSSET ( SCAN12,FIXL, ECLK1 );   
        TSSET ( SCAN13,FIXL, ECLK1 );   
        TSSET ( SCAN14,FIXL, ECLK1 );   
        TSSET ( SCAN15,FIXL, ECLK1 );   
        TSSET ( SCAN16,FIXL, ECLK1 );   
        TSSET ( SCAN17,FIXL, ECLK1 );   
        TSSET ( SCAN18,FIXL, ECLK1 );   
        TSSET ( SCAN19,FIXL, ECLK1 );   
        TSSET ( SCAN20,FIXL, ECLK1 );   
        TSSET ( SCAN21,FIXL, ECLK1 );   
        TSSET ( SCAN22,FIXL, ECLK1 );   
        TSSET ( SCAN23,FIXL, ECLK1 );   
        TSSET ( SCAN24,FIXL, ECLK1 );   
        TSSET ( SCAN25,FIXL, ECLK1 );   
        TSSET ( SCAN26,FIXL, ECLK1 );   
        TSSET ( SCAN27,FIXL, ECLK1 );   
        TSSET ( SCAN28,FIXL, ECLK1 );   
        TSSET ( SCAN29,FIXL, ECLK1 );   
        TSSET ( SCAN30,FIXL, ECLK1 );     
        TSSET ( SCAN31,FIXL, ECLK1 );  

        TSSET (  IO0, FIXL, ECLK1 );   
        TSSET (  IO1, FIXL, ECLK1 );   
        TSSET (  IO2, FIXL, ECLK1 );   
        TSSET (  IO3, FIXL, ECLK1 );   
        TSSET (  IO4, FIXL, ECLK1 );   
        TSSET (  IO5, FIXL, ECLK1 );   
        TSSET (  IO6, FIXL, ECLK1 );   
        TSSET (  IO7, FIXL, ECLK1 );   
        TSSET (  IO8, FIXL, ECLK1 );   
        TSSET (  IO9, FIXL, ECLK1 );   
        TSSET ( IO10, FIXL, ECLK1 );   
        TSSET ( IO11, FIXL, ECLK1 );   
        TSSET ( IO12, FIXL, ECLK1 );   
        TSSET ( IO13, FIXL, ECLK1 );  
        TSSET ( IO14, FIXL, ECLK1 );   
        TSSET ( IO15, FIXL, ECLK1 );   
        TSSET ( IO16, FIXL, ECLK1 );   
        TSSET ( IO17, FIXL, ECLK1 );   
        TSSET ( IO18, FIXL, ECLK1 );   
        TSSET ( IO19, FIXL, ECLK1 );   
        TSSET ( IO20, FIXL, ECLK1 );   
        TSSET ( IO21, FIXL, ECLK1 );   
        TSSET ( IO22, FIXL, ECLK1 );   
        TSSET ( IO23, FIXL, ECLK1 );   
        TSSET ( IO24, FIXL, ECLK1 );   
        TSSET ( IO25, FIXL, ECLK1 );   
        TSSET ( IO26, FIXL, ECLK1 );   
        TSSET ( IO27, FIXL, ECLK1 );   
        TSSET ( IO28, FIXL, ECLK1 );   
        TSSET ( IO29, FIXL, ECLK1 );   
        TSSET ( IO30, FIXL, ECLK1 );   
        TSSET ( IO31, FIXL, ECLK1 );   

        TSSET (  DRE, FIXH);
        TSSET (  DRE_B, FIXH);        //TSSET ( STRB, NRZB, ECLK1);
}

void load_pds_low ()
{
   //     printf("%s : %s\n", __FUNCTION__,  cyp_name);
        PDS(  CLK0,  FIXL  ,  FIXL  ,  FIXL  ,  FIXL  );     
        PDS(  CLK1,  FIXL  ,  FIXL  ,  FIXL  ,  FIXL  );     
        PDS(  CLK2,  FIXL  ,  FIXL  ,  FIXL  ,  FIXL  );     
        PDS(  CLK3,  FIXL  ,  FIXL  ,  FIXL  ,  FIXL  );     
        PDS(  CLK4,  FIXL  ,  FIXL  ,  FIXL  ,  FIXL  );     
        PDS(  CLK5,  FIXL  ,  FIXL  ,  FIXL  ,  FIXL  );     
        PDS(  CLK6,  FIXL  ,  FIXL  ,  FIXL  ,  FIXL  );     
        PDS(  CLK7,  FIXL  ,  FIXL  ,  FIXL  ,  FIXL  );     

        PDS(  SCAN0,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  SCAN1,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  SCAN2,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  SCAN3,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  SCAN4,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  SCAN5,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  SCAN6,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  SCAN7,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  SCAN8,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  SCAN9,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS( SCAN10,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN11,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN12,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN13,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN14,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN15,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN16,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN17,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN18,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN19,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN20,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN21,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN22,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN23,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN24,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN25,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN26,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN27,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN28,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN29,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN30,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( SCAN31,  FIXL,  FIXL, FIXL,  FIXL); 

        PDS(  ADDR0,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  ADDR1,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  ADDR2,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  ADDR3,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  ADDR4,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  ADDR5,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  ADDR6,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  ADDR7,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  ADDR8,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS(  ADDR9,  FIXL,  FIXL, FIXL,  FIXL);     
        PDS( ADDR10,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR11,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR12,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR13,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR14,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR15,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR16,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR17,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR18,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR19,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR20,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR21,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR22,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR23,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR24,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR25,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR26,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR27,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR28,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR29,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR30,  FIXL,  FIXL, FIXL,  FIXL);    
        PDS( ADDR31,  FIXL,  FIXL, FIXL,  FIXL);    
 
        PDS(   IO0,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(   IO1,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(   IO2,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(   IO3,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(   IO4,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(   IO5,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(   IO6,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(   IO7,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(   IO8,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(   IO9,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(  IO10,  FIXL,  FIXL,   FIXL,   FIXL,   FIXL,   FIXL,   FIXL  );     
        PDS(  IO11,  FIXL,  FIXL,   FIXL,   FIXL,   FIXL,   FIXL,   FIXL  );     
        PDS(  IO12,  FIXL,  FIXL,   FIXL,   FIXL,   FIXL,   FIXL,   FIXL  );     
        PDS(  IO13,  FIXL,  FIXL,   FIXL,   FIXL,   FIXL,   FIXL,   FIXL  );     
        PDS(  IO14,  FIXL,  FIXL,   FIXL,   FIXL,   FIXL,   FIXL,   FIXL  );     
        PDS(  IO15,  FIXL,  FIXL,   FIXL,   FIXL,   FIXL,   FIXL,   FIXL  );     
        PDS(  IO16,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );   
        PDS(  IO17,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(  IO18,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(  IO19,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(  IO20,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(  IO21,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(  IO22,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(  IO23,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(  IO24,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(  IO25,  FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL,	FIXL  );     
        PDS(  IO26,  FIXL,  FIXL,   FIXL,   FIXL,   FIXL,   FIXL,   FIXL  );     
        PDS(  IO27,  FIXL,  FIXL,   FIXL,   FIXL,   FIXL,   FIXL,   FIXL  );     
        PDS(  IO28,  FIXL,  FIXL,   FIXL,   FIXL,   FIXL,   FIXL,   FIXL  );     
        PDS(  IO29,  FIXL,  FIXL,   FIXL,   FIXL,   FIXL,   FIXL,   FIXL  );     
        PDS(  IO30,  FIXL,  FIXL,   FIXL,   FIXL,   FIXL,   FIXL,   FIXL  );     
        PDS(  IO31,  FIXL,  FIXL,   FIXL,   FIXL,   FIXL,   FIXL,   FIXL  );   
}

void load_pds_high ()
{
   //     printf("%s : %s\n", __FUNCTION__,  cyp_name);
        PDS(  CLK0,  FIXH  ,  FIXH  ,  FIXH  ,  FIXH  );     
        PDS(  CLK1,  FIXH  ,  FIXH  ,  FIXH  ,  FIXH  );     
        PDS(  CLK2,  FIXH  ,  FIXH  ,  FIXH  ,  FIXH  );     
        PDS(  CLK3,  FIXH  ,  FIXH  ,  FIXH  ,  FIXH  );     
        PDS(  CLK4,  FIXH  ,  FIXH  ,  FIXH  ,  FIXH  );     
        PDS(  CLK5,  FIXH  ,  FIXH  ,  FIXH  ,  FIXH  );     
        PDS(  CLK6,  FIXH  ,  FIXH  ,  FIXH  ,  FIXH  );     
        PDS(  CLK7,  FIXH  ,  FIXH  ,  FIXH  ,  FIXH  );     

        PDS(  SCAN0,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  SCAN1,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  SCAN2,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  SCAN3,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  SCAN4,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  SCAN5,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  SCAN6,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  SCAN7,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  SCAN8,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  SCAN9,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS( SCAN10,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN11,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN12,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN13,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN14,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN15,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN16,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN17,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN18,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN19,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN20,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN21,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN22,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN23,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN24,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN25,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN26,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN27,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN28,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN29,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN30,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( SCAN31,  FIXH,  FIXH, FIXH,  FIXH); 

        PDS(  ADDR0,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  ADDR1,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  ADDR2,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  ADDR3,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  ADDR4,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  ADDR5,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  ADDR6,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  ADDR7,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  ADDR8,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS(  ADDR9,  FIXH,  FIXH, FIXH,  FIXH);     
        PDS( ADDR10,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR11,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR12,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR13,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR14,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR15,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR16,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR17,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR18,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR19,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR20,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR21,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR22,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR23,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR24,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR25,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR26,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR27,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR28,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR29,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR30,  FIXH,  FIXH, FIXH,  FIXH);    
        PDS( ADDR31,  FIXH,  FIXH, FIXH,  FIXH);    
 
        PDS(   IO0,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(   IO1,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(   IO2,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(   IO3,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(   IO4,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(   IO5,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(   IO6,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(   IO7,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(   IO8,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(   IO9,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(  IO10,  FIXH,  FIXH,   FIXH,   FIXH,   FIXH,   FIXH,   FIXH  );     
        PDS(  IO11,  FIXH,  FIXH,   FIXH,   FIXH,   FIXH,   FIXH,   FIXH  );     
        PDS(  IO12,  FIXH,  FIXH,   FIXH,   FIXH,   FIXH,   FIXH,   FIXH  );     
        PDS(  IO13,  FIXH,  FIXH,   FIXH,   FIXH,   FIXH,   FIXH,   FIXH  );     
        PDS(  IO14,  FIXH,  FIXH,   FIXH,   FIXH,   FIXH,   FIXH,   FIXH  );     
        PDS(  IO15,  FIXH,  FIXH,   FIXH,   FIXH,   FIXH,   FIXH,   FIXH  );     
        PDS(  IO16,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );   
        PDS(  IO17,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(  IO18,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(  IO19,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(  IO20,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(  IO21,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(  IO22,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(  IO23,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(  IO24,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(  IO25,  FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH,	FIXH  );     
        PDS(  IO26,  FIXH,  FIXH,   FIXH,   FIXH,   FIXH,   FIXH,   FIXH  );     
        PDS(  IO27,  FIXH,  FIXH,   FIXH,   FIXH,   FIXH,   FIXH,   FIXH  );     
        PDS(  IO28,  FIXH,  FIXH,   FIXH,   FIXH,   FIXH,   FIXH,   FIXH  );     
        PDS(  IO29,  FIXH,  FIXH,   FIXH,   FIXH,   FIXH,   FIXH,   FIXH  );     
        PDS(  IO30,  FIXH,  FIXH,   FIXH,   FIXH,   FIXH,   FIXH,   FIXH  );     
        PDS(  IO31,  FIXH,  FIXH,   FIXH,   FIXH,   FIXH,   FIXH,   FIXH  );   
}

void adc_read(int nADC, int idx, float * pData, int nRepeat = 1)
{
    if(nADC > 1 || idx > 7) {
        printf("adc read error : nADC(%d) idx(%d)\n", nADC, idx);
        return;
    }

    short ref5[8]  = {0,0,0,0,0,0,0,0};
    float fref5[8] = {0,0,0,0,0,0,0,0};
    for( int repeat = 0; repeat < nRepeat; repeat++) {
        for(int i = 0; i < 8; i++) {
            diag_peReadAdc(nADC, idx, ref5); // 5.0V
            fref5[i] += ref5[i] * 10.0 / 65535.0;
        }
    }
    pData[0] = fref5[0] / nRepeat;
    pData[1] = fref5[1] / nRepeat;
    pData[2] = fref5[2] / nRepeat;
    pData[3] = fref5[3] / nRepeat;
    pData[4] = fref5[4] / nRepeat;
    pData[5] = fref5[5] / nRepeat;
    pData[6] = fref5[6] / nRepeat;
    pData[7] = fref5[7] / nRepeat;
}

void PE_ADC0_Cal()
{
    //// GND read ///////////////
    bool bResult = true;
    int idx = 0;
    float GND[8];
    g_strLog.str("");
    g_strLog << "=============== ADC0 Cal ===============\n";
    std::cout<<g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    adc_read(0, idx, GND, 1);
    adc_read(0, idx, GND, 10);
    //// 5V read ///////////////
    idx = 1;
    float REF5V[8];
    adc_read(0, idx, REF5V, 1);
    adc_read(0, idx, REF5V, 10);
    //////////////// Gain, Offset Calc //////////////////
    //printf ("==============================\n");
    for(int i = 0 ; i < 6 ; i++) {
        g_adc0Gain[i] = (REF5V[i] - GND[i])/2.5;
        g_adc0Offset[i] = (GND[i]);
        g_strLog.str("");
        g_strLog << "ADC0 CH["<< std::setw(3) << i << "]" <<" Cal gain "<< std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed << g_adc0Gain[i] <<", offset " << g_adc0Offset[i];
        if(abs(1.0 - g_adc0Gain[i]) > 0.05) {
            bResult = false;
            g_strLog << " ------- [FAIL]\n";
        }else {
            g_strLog << " ------- [PASS]\n";
        }
        std::cout << g_strLog.str();
        fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    }

    printf ("============= ADC0 Cal Data File and ROM Write =================\n");
    uint16_t addr = CAL_ADC0_ADDR;

    fwrite(g_adc0Gain, 0x20, 1, g_pFileCalData);
    int rv = diag_romWrite(addr, 0x20, (uint8_t*)g_adc0Gain);
    if(rv != 0)
        printf("status : %d\n", rv);
    addr += 0x20;

    fwrite(g_adc0Offset, 0x20, 1, g_pFileCalData);
    rv = diag_romWrite(addr, 0x20, (uint8_t*)g_adc0Offset);
    if(rv != 0)
        printf("status : %d\n", rv);

    addr = CAL_ADC0_ADDR;
    rv = diag_romRead(addr, 0x20, (uint8_t*)g_RData);
    if(rv != 0)
        printf("status : %d\n", rv);

    if( g_adc0Gain[0] != g_RData[0] || 
        g_adc0Gain[1] != g_RData[1] || 
        g_adc0Gain[2] != g_RData[2] || 
        g_adc0Gain[3] != g_RData[3] || 
        g_adc0Gain[4] != g_RData[4] || 
        g_adc0Gain[5] != g_RData[5] || 
        g_adc0Gain[6] != g_RData[6] || 
        g_adc0Gain[7] != g_RData[7] ) {
            printf ("ADC(PE) Cal Data ROM Write Fail. %0.5f %0.5f %0.5f %0.5f %0.5f %0.5f %0.5f %0.5f\n", g_RData[0], g_RData[1], g_RData[2], g_RData[3], g_RData[4], g_RData[5], g_RData[6], g_RData[7]);
    }

    if(bResult == false) {
        g_bCalResult = false;
        diag_msg(0, "ADC0 Cal");
    }else diag_msg(1, "ADC0 Cal");
}

void PE_ADC0_TEST()
{
	
    //// GND read ///////////////
    bool bResult = true;
    int idx = 0;
    float GND[8];
    g_strLog.str("");
    g_strLog << "=============== ADC0 GND Read Test ===============\n";
    std::cout<<g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    adc_read(0, idx, GND, 1);
    adc_read(0, idx, GND, 10);
    for(int i = 0 ; i < 6 ; i++) {
        g_strLog.str("");
        g_strLog << "ADC0 CH[" << std::setw(3) << i << "]" << " REF Read(GND) Test "<< std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed  << (GND[i] - g_adc0Offset[i]) / g_adc0Gain[i];
        if(fabs((GND[i] - g_adc0Offset[i]) / g_adc0Gain[i]) > 0.05) {
            bResult = false;
            g_strLog << " ------- [FAIL]\n";
        }else {
            g_strLog << " ------- [PASS]\n";
        }
        std::cout << g_strLog.str();
        fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    }
    //// 5V read ///////////////
    idx = 1;
    float REF5V[8];
    g_strLog.str("");
    g_strLog << "=============== ADC0 5V Read TEst ===============\n";
    std::cout<<g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    adc_read(0, idx, REF5V, 1);
    adc_read(0, idx, REF5V, 10);
    for(int i = 0 ; i < 6 ; i++) {
        g_strLog.str("");
        g_strLog << "ADC0 CH[" << std::setw(3) << i << "]" << " REF Read(5V) Test "<< std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed  << (REF5V[i] - g_adc0Offset[i]) / g_adc0Gain[i];
        if(fabs(2.5 - ((REF5V[i] - g_adc0Offset[i]) / g_adc0Gain[i])) > 0.05) {
            bResult = false;
            g_strLog << " ------- [FAIL]\n";
        }else {
            g_strLog << " ------- [PASS]\n";
        }
        std::cout << g_strLog.str();
        fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    }

    if(bResult == false) {
        g_bCalResult = false;
        diag_msg(0, "ADC0 Test");
    }else diag_msg(1, "ADC0 Test");
}

void PE_ADC1_Cal()
{
    //// GND read ///////////////
    bool bResult = true;
    int idx = 0;
    float GND[8];
    g_strLog.str("");
    g_strLog<<"=============== ADC1 Cal ===============\n";
    std::cout<< g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);

    adc_read(1, idx, GND, 1);
    adc_read(1, idx, GND, 10);
    //// 5V read ///////////////
    idx = 1;
    float REF5V[8];
    adc_read(1, idx, REF5V, 1);
    adc_read(1, idx, REF5V, 10);
    //////////////// Gain, Offset Calc //////////////////
    for(int i = 0 ; i < 8 ; i++) {
        if(i < 6 ) {
            g_adc1Gain[i] = (REF5V[i] - GND[i])/2.5;
            g_adc1Offset[i] = (GND[i]);
            g_strLog.str("");
            g_strLog << "ADC1 CH[" << std::setw(3) << i << "]" <<" gain " << std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed << g_adc1Gain[i] <<", offset " << g_adc1Offset[i];
            if(abs(1.0 - g_adc1Gain[i]) > 0.05) {
                bResult = false;
                g_strLog << " ------- [FAIL]\n";
            }else{
                g_strLog << " ------- [PASS]\n";
            }
            std::cout<< g_strLog.str() ;
            fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
        }
        else {
            g_adc1Gain[i] = 1.0;
            g_adc1Offset[i] = 0.0;
        }
    }

    g_strLog.str("");
    g_strLog << "=============ADC1 Cal data File and ROM Write=================\n";
    std::cout<< g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);

    uint16_t addr = CAL_ADC1_ADDR;

    fwrite(g_adc1Gain, 0x20, 1, g_pFileCalData);
    int rv = diag_romWrite(addr, 0x20, (uint8_t*)g_adc1Gain);
    if(rv != 0)
        printf("status : %d\n", rv);
    addr += 0x20;

    fwrite(g_adc1Offset, 0x20, 1, g_pFileCalData);
    rv = diag_romWrite(addr, 0x20, (uint8_t*)g_adc1Offset);
    if(rv != 0)
        printf("status : %d\n", rv);

    addr = CAL_ADC1_ADDR;
    rv = diag_romRead(addr, 0x20, (uint8_t*)g_RData);
    if(rv != 0)
        printf("status : %d\n", rv);

    if( g_adc1Gain[0] != g_RData[0] || 
        g_adc1Gain[1] != g_RData[1] || 
        g_adc1Gain[2] != g_RData[2] || 
        g_adc1Gain[3] != g_RData[3] || 
        g_adc1Gain[4] != g_RData[4] || 
        g_adc1Gain[5] != g_RData[5] || 
        g_adc1Gain[6] != g_RData[6] || 
        g_adc1Gain[7] != g_RData[7] ) {
            printf ("ADC(PE) Cal Data Rom Write Fail. %0.5f %0.5f %0.5f %0.5f %0.5f %0.5f %0.5f %0.5f\n", g_RData[0], g_RData[1], g_RData[2], g_RData[3], g_RData[4], g_RData[5], g_RData[6], g_RData[7]);
    }

    if(bResult == false) {
        g_bCalResult = false;
        diag_msg(0, "ADC1 Cal");
    }else diag_msg(1, "ADC1 Cal");
}

void PE_ADC1_TEST()
{
    //// GND read ///////////////
    bool bResult = true;
    int idx = 0;
    float GND[8];
    g_strLog.str("");
    g_strLog << "=============== ADC1 GND Read Test ===============\n";
    std::cout<<g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    adc_read(1, idx, GND, 1);
    adc_read(1, idx, GND, 10);
    for(int i = 0 ; i < 6 ; i++) {
        g_strLog.str("");
        g_strLog << "ADC1 ch" << i << " REF Read(GND) Test " << std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed << (GND[i] - g_adc1Offset[i]) / g_adc1Gain[i];
        if(fabs((GND[i] - g_adc1Offset[i]) / g_adc1Gain[i]) > 0.05) {
            bResult = false;
            g_strLog << " ------- [FAIL]\n";
        }else {
            g_strLog << " ------- [PASS]\n";
        }
        std::cout << g_strLog.str();
        fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    }
    //// 5V read ///////////////
    idx = 1;
    float REF5V[8];
    g_strLog.str("");
    g_strLog << "=============== ADC1 REF 5V Test ===============\n";
    std::cout<<g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    adc_read(0, idx, REF5V, 1);
    adc_read(0, idx, REF5V, 10);
    for(int i = 0 ; i < 6 ; i++) {
        g_strLog.str("");
        g_strLog << "ADC1 ch" << i << " REF Read(5V) Test " << std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed << (REF5V[i] - g_adc1Offset[i]) / g_adc1Gain[i];
        if(fabs(2.5 - ((REF5V[i] - g_adc1Offset[i]) / g_adc1Gain[i])) > 0.05) {
            bResult = false;
            g_strLog << " ------- [FAIL]\n";
        }else {
            g_strLog << " ------- [PASS]\n";
        }
        std::cout << g_strLog.str();
        fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    }
    
    if(bResult == false) {
        g_bCalResult = false;
        diag_msg(0, "ADC1 Test");
    }else diag_msg(1, "ADC1 Test");
}

void PE_SYS_VOLTAGE0()
{
    //// GND read ///////////////
    bool bResult = true;
    int idx[3] = {0, 1, 7};
    float read[8];
    float data[8];
    float exp[3][6] = { {0, 0, 0, 0, 0, 0},
                        {2.5, 2.5, 2.5, 2.5, 2.5, 2.5},
                        {1.25, 1.25, 1.25, 1.25, 1.25, 1.25}};
    g_strLog.str("");
    g_strLog << "=============== SYSTEM Voatage Read Test ===============\n";
    std::cout<<g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    for(int j = 0; j < 3 ; j++ ) {
        adc_read(0, idx[j], read, 1);
        adc_read(0, idx[j], read, 3);
        for(int i = 0 ; i < 6 ; i++) {
            data[i] = (read[i] - g_adc0Offset[i]) / g_adc0Gain[i];
            g_strLog.str("");
            g_strLog << "ADC0 MUX " << i << " Ch "<< idx[j] << " read : " << std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed << data[i];
            g_strLog << " exp : "<< exp[j][i];
            if(exp[j][i] != 0) {
                if(data[i] > (exp[j][i] * 1.1) || data[i] < (exp[j][i] * 0.9)) {
                    bResult = false;
                    g_strLog << " ------- [FAIL]\n";
                }else {
                    g_strLog << " ------- [PASS]\n";
                }
            }else {
                if(data[i] > (exp[j][i] + 0.05) || data[i] < (exp[j][i] - 0.05)) {
                    bResult = false;
                    g_strLog << " ------- [FAIL]\n";
                }else {
                    g_strLog << " ------- [PASS]\n";
                }
            }
            std::cout << g_strLog.str();
            fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
        }
    }

    if(bResult == false) {
        g_bCalResult = false;
        diag_msg(0, "SYSTEM Voatage Read Test");
    }else diag_msg(1, "SYSTEM Voatage Read Test");
}

void PE_SYS_VOLTAGE1()
{
    //// GND read ///////////////
    bool bResult = true;
    float read[8];
    float data[8];
    float exp[6][6] = { {2.5, 2.5, 2.5, 2.5, 2.5, 2.5},
                        {0.9, 0.9, 1.75, 2.5, 2.5, 1.65},
                        {0.5, 0.5, 0.6, 0.9, 1.65, 1.25},
                        {0.43, 0.6, 0.9, 0.3, 0.3, 1.25},
                        {3, 3, 3, 2.5, 1.5, 3},
                        {3, -1.25, 3, -1.75, -1.75, 0},};
    g_strLog.str("");
    g_strLog << "=============== SYSTEM Voatage Read Test ===============\n";
    std::cout<<g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    for(int idx = 1; idx < 7 ; idx++ ) {
        adc_read(1, idx, read, 1);
        adc_read(1, idx, read, 3);
        for(int i = 0 ; i < 6 ; i++) {
            data[i] = (read[i] - g_adc1Offset[i]) / g_adc1Gain[i];
            g_strLog.str("");
            g_strLog << "ADC1 MUX " << i << " Ch "<< idx << " read : " << std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed << data[i];
            g_strLog << " exp : "<< exp[idx-1][i];
            if(exp[idx-1][i] < 0) {
                if(data[i] < (exp[idx-1][i] * 1.03) || data[i] > (exp[idx-1][i] * 0.97)) {
                    bResult = false;
                    g_strLog << " ------- [FAIL]\n";
                }else {
                    g_strLog << " ------- [PASS]\n";
                }
            }else if(exp[idx-1][i] != 0) {
                if(data[i] > (exp[idx-1][i] * 1.03) || data[i] < (exp[idx-1][i] * 0.97)) {
                    bResult = false;
                    g_strLog << " ------- [FAIL]\n";
                }else {
                    g_strLog << " ------- [PASS]\n";
                }
            }else {
                if(data[i] > (exp[idx-1][i] + 0.05) || data[i] < (exp[idx-1][i] - 0.05)) {
                    bResult = false;
                    g_strLog << " ------- [FAIL]\n";
                }else {
                    g_strLog << " ------- [PASS]\n";
                }
            }
            std::cout << g_strLog.str();
            fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
        }
    }

    if(bResult == false) {
        g_bCalResult = false;
        diag_msg(0, "SYSTEM Voatage Read Test");
    }else diag_msg(1, "SYSTEM Voatage Read Test");
}

void PE_DAC_CAL()
{
    bool bResult = true;
    std::string strName = "DPS VCC";
    int muxCh = 5;
    int adcCh = 5;
    float range = 3.0;

    g_strLog.str("");
    g_strLog << "=============== DAC DPS VCC Cal ===============\n";
    std::cout<<g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    //// High Voltage read ///////////////
    diag_DAC(7, 8.0);// DPS VCC Level Set
    float High[8]; // idx 2(HV) , 5(DPS) 
    adc_read(0, 5, High, 1);
    adc_read(0, 5, High, 10); //DPS VCC ì¸¡ì 
    //// Low Voltage read ///////////////
    diag_DAC(7, 5.0);// DPS VCC Level Set
    float Low[8]; // idx 2(HV) , 5(DPS) ë§ ì í¨
    adc_read(0, muxCh, Low, 1);
    adc_read(0, muxCh, Low, 10);

    g_dacGain[0] = ((High[adcCh] - Low[adcCh])*2)/range;
    g_dacOffset[0] = (Low[adcCh]*2) - (g_dacGain[0]*5);

    g_adc0Gain[6] = g_dacGain[0];
    g_adc0Offset[6] = g_dacOffset[0];

    diag_DAC(7, (8.0 - g_dacOffset[0]) * g_dacGain[0]);// DPS VCC Level Set
    g_strLog.str("");
    g_strLog << "DAC DPS VCC gain " << std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed << g_dacGain[0] <<", offset " << g_dacOffset[0];
    if(abs(1.0 - g_dacGain[0]) > 0.05) {
        bResult = false;
        g_strLog << " ------- [FAIL]\n";
    }else {
        g_strLog << " ------- [PASS]\n";
    }
    std::cout << g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);

    g_strLog.str("");
    g_strLog << "=============DAC DPS VCC Cal Data File and ROM Write=================\n";
    std::cout<< g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);

    uint16_t addr = CAL_ADC0_ADDR;
    fseek(g_pFileCalData, 0, 0);
    fwrite(g_adc0Gain, 0x20, 1, g_pFileCalData);
    int rv = diag_romWrite(addr, 0x20, (uint8_t*)g_adc0Gain);
    if(rv != 0)
        printf("status : %d\n", rv);
    addr += 0x20;

    fwrite(g_adc0Offset, 0x20, 1, g_pFileCalData);
    rv = diag_romWrite(addr, 0x20, (uint8_t*)g_adc0Offset);
    if(rv != 0)
        printf("status : %d\n", rv);

    addr = CAL_ADC0_ADDR;
    rv = diag_romRead(addr, 0x20, (uint8_t*)g_RData);
    if(rv != 0)
        printf("status : %d\n", rv);

    if(bResult == false) {
        g_bCalResult = false;
        diag_msg(0, "DAC DPS VCC Cal");
    }else diag_msg(1, "DAC DPS VCC Cal");
}

void PE_DAC_TEST()
{
    bool bResult = true;
    std::string strName = "DPS VCC";
    int muxCh = 5;
    int adcCh = 5;

    g_strLog.str("");
    g_strLog << "============= DAC DPS VCC Test =================\n";
    std::cout<< g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);

    diag_DAC(7, (8.0 - g_dacOffset[0]) * g_dacGain[0]);// DPS VCC Level Set
    float read[8]; // idx 2(HV) , 5(DPS)
    adc_read(0, muxCh, read, 1);
    adc_read(0, muxCh, read, 10); //HV VCC¸
    g_strLog.str("");
    g_strLog << "DAC DPS VCC Test Voltage Read(8V) : " << std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed << read[adcCh]*2;
    if( fabs((read[adcCh]*2) - 8.0 ) > 0.08 ) {
        bResult = false;
        g_strLog << " ------- [FAIL]\n";
    }else  {
        g_strLog << " ------- [PASS]\n";
    }
    std::cout << g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);

    if(bResult == false) {
        g_bCalResult = false;
        diag_msg(0, "DAC DPS VCC Test");
    }else diag_msg(1, "DAC DPS VCC Test");
}

void PE_VIL_Cal()
{
    bool bResult = true;
    float data_h[8];
    float data_l[8];

    diag_DAC(7, 8.0);// DPS VCC Level Set
    
    load_feset_cal_L("PE_VIL_Cal");
    load_pds_low();
    float vHigh = 1.0;
    VIH ( ALL , 2.0, false ); // VIH set
    VIL ( ALL , vHigh, false);// VIL set
    VTH ( 0.5 );              // comparator VTH set  

    SRON();

    //SEND_MPAT("cal.mpa");
    //diag_peSetChDiag( 0 );
    START_MPAT("INFINITY_L");
    usleep(10000);

    uint16_t peData;

    for(int peCh = 0; peCh < 8; peCh++) {
        diag_peSetChDiag( peCh);
        for(int adcIdx = 5; adcIdx > 1; adcIdx--) { // IDX 2, 3, 4, 5
            adc_read(0, adcIdx, data_h, 1); // 최초 Read값은 제외
            adc_read(0, adcIdx, data_h, 10);
            for(int idx = 0; idx < 6; idx++) {
                if(chLookup[peCh][adcIdx-2][idx] != -1) {
                    int ch = chLookup[peCh][adcIdx-2][idx];
                    //g_peGain[ch] = (data_h[idx] * g_adcGain[idx]) + g_adcOffset[idx];
                    g_peGain[ch] = (data_h[idx] - g_adc0Offset[idx] ) / g_adc0Gain[idx];
                    g_data[ch] = g_peGain[ch] * 2;
                }
            }
        }
    }
    STOP_MPAT();
    SROF();
    usleep(5000);

    float vLow = 0.1;
    VIL ( ALL , vLow, false);       // VIL set
    SRON();

    START_MPAT("INFINITY_L");
    usleep(10000);

    for(int peCh = 0; peCh < 8; peCh++) {
        diag_peSetChDiag( peCh);
        for(int adcIdx = 2; adcIdx < 6; adcIdx++) { // IDX 2, 3, 4, 5
            adc_read(0, adcIdx, data_l, 1);
            adc_read(0, adcIdx, data_l, 10);
            for(int idx = 0; idx < 6; idx++) {
                if(chLookup[peCh][adcIdx-2][idx] != -1) {
                    int ch = chLookup[peCh][adcIdx-2][idx];
                    data_l[idx] = (data_l[idx] - g_adc0Offset[idx]) / g_adc0Gain[idx];
                    g_peGain[ch] = ((g_peGain[ch] - data_l[idx] )*2) / (vHigh - vLow);
                    g_peOffset[ch] = (data_l[idx]*2) - (g_peGain[ch]*vLow);
                    g_data2[ch] = data_l[idx]*2;
                }
            }
        }
    }

    diag_peSetDiagOff();
    STOP_MPAT();
    SROF();

    g_strLog.str("");
    g_strLog << "=================== PE VIL Cal ====================\n";
    std::cout << g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    for(int ch = 0; ch < 176; ch++) {
        g_strLog.str("");
        g_strLog <<"VIL CH["<< std::setw(3) << ch <<"] " << "gain:" << std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed << g_peGain[ch] << " offset:"<<g_peOffset[ch] << " data:"<< g_data[ch]<<" "<< g_data2[ch];
        if(g_peGain[ch] > 1.2 || g_peGain[ch] < 0.90) {
            bResult = true;
            g_strLog << " ------- [FAIL]\n";
        }else {
            g_strLog << " ------- [PASS]\n";
        }
        SET_PE_CAL(1, ch, g_peGain[ch],g_peOffset[ch]);
        std::cout<<g_strLog.str();
        fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    }

    g_strLog.str("");
    g_strLog << "=============VIL Cal Data File and ROM Write==============\n";
    std::cout << g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);

    uint16_t addr = CAL_PE_VIL_ADDR;
    uint32_t size = sizeof(g_peGain);
    
    int rv = diag_romWrite(addr, size, (uint8_t*)g_peGain);
    if(rv != 0)
        printf("status : %d\n", rv);
    addr += size;
    
    rv = diag_romWrite(addr, size, (uint8_t*)g_peOffset);
    if(rv != 0)
        printf("status : %d\n", rv);

    fwrite(g_peGain, size, 1, g_pFileCalData);
    fwrite(g_peOffset, size, 1, g_pFileCalData);

    if(bResult == false) {
        g_bCalResult = false;
        diag_msg(0, "PE VIL Cal");
    }else diag_msg(1, "PE VIL Cal");
}

void PE_VIH_Cal()
{
    bool bResult = true;
    float data_h[8];
    float data_l[8];

    //diag_DAC(6, 9.0);// HV VCC Level Set
    diag_DAC(7, 8.0);// DPS VCC Level Set

    load_feset_cal_H("PE_VIH_Cal");
    load_pds_high();

    float vHigh = 2.0;
    VIH ( ALL , vHigh, false ); // VIH set
    VIL ( ALL , 0.0, true );       // VIL set
    VTH ( 0.5 ); 
    
    SRON();

    //SEND_MPAT("cal.mpa");
    diag_peSetChDiag( 0 );
    START_MPAT("INFINITY_H");
    usleep(10000);

    for(int peCh = 0; peCh < 8; peCh++) {
        diag_peSetChDiag( peCh );
        for(int adcIdx = 2; adcIdx < 6; adcIdx++) { // IDX 2, 3, 4, 5
            adc_read(0, adcIdx, data_h, 1);
            adc_read(0, adcIdx, data_h, 10);
            for(int idx = 0; idx < 6; idx++) {
                if(chLookup[peCh][adcIdx-2][idx] != -1) {
                    int ch = chLookup[peCh][adcIdx-2][idx];
                    //g_peGain[ch] = (data_h[idx] * g_adcGain[idx]) + g_adcOffset[idx];
                    g_peGain[ch] = (data_h[idx] - g_adc0Offset[idx] ) / g_adc0Gain[idx];
                    g_data[ch] = g_peGain[ch] * 2;
                }
            }
        }
    }

    STOP_MPAT();
    SROF();
    usleep(1000000);

    float vLow = 1.0;
    VIH ( ALL , vLow, false ); // VIH set

    SRON();

    START_MPAT("INFINITY_H");
    usleep(10000);

    for(int peCh = 0; peCh < 8; peCh++) {
        diag_peSetChDiag( peCh);
        for(int adcIdx = 2; adcIdx < 6; adcIdx++) { // IDX 2, 3, 4, 5
            adc_read(0, adcIdx, data_l, 1);
            adc_read(0, adcIdx, data_l, 10);
            for(int idx = 0; idx < 6; idx++) {
                if(chLookup[peCh][adcIdx-2][idx] != -1) {
                    int ch = chLookup[peCh][adcIdx-2][idx];
                    //data_l[idx] = (data_l[idx] * g_adcGain[idx]) + g_adcOffset[idx];
                    data_l[idx] = (data_l[idx] - g_adc0Offset[idx]) / g_adc0Gain[idx];
                    //g_peGain[ch] = ((g_peGain[ch] - data_l[idx] )*2)/3.0;
                    g_peGain[ch] = ((g_peGain[ch] - data_l[idx] )*2) / (vHigh - vLow);
                    g_peOffset[ch] = (data_l[idx]*2) - (g_peGain[ch]*vLow);
                    g_data2[ch] = data_l[idx]*2;
                }
            }
        }
    }

    STOP_MPAT();
    diag_peSetDiagOff();
    SROF();

    g_strLog.str("");
    g_strLog << "=================== PE VIH Cal ====================\n";
    std::cout << g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    for(int ch = 0; ch < 176; ch++) {
        g_strLog.str("");
        g_strLog <<"VIH CH[" << std::setw(3) << ch <<"] " << "gain:" << std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed << g_peGain[ch] << " offset:"<<g_peOffset[ch] << " data:"<< g_data[ch]<<" "<< g_data2[ch];
        if(g_peGain[ch] > 1.2 || g_peGain[ch] < 0.90) {
            bResult = true;
            g_strLog << " ------- [FAIL]\n";
        }else {
            g_strLog << " ------- [PASS]\n";
        }
        SET_PE_CAL(0, ch, g_peGain[ch],g_peOffset[ch]);
        std::cout<<g_strLog.str();
        fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    }

    g_strLog.str("");
    g_strLog << "=============VIH Cal Data File and ROM Write==============\n";
    std::cout << g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);

    uint16_t addr = CAL_PE_VIH_ADDR;
    uint32_t size = sizeof(g_peGain);

    int rv = diag_romWrite(addr, size, (uint8_t*)g_peGain);
    if(rv != 0)
        printf("status : %d\n", rv);

    addr += size;

    rv = diag_romWrite(addr, size, (uint8_t*)g_peOffset);
    if(rv != 0)
        printf("status : %d\n", rv);

    fwrite(g_peGain, size, 1, g_pFileCalData);
    fwrite(g_peOffset, size, 1, g_pFileCalData);

    if(bResult == false) {
        g_bCalResult = false;
        diag_msg(0, "PE VIH Cal");
    }else diag_msg(1, "PE VIH Cal");
}

void PE_VIL_TEST()
{
    bool bResult = true;
    float data[8];

    load_feset_cal_L("PE_VIL_TEST");
    load_pds_low();

    VIH(ALL, 2, false);
    VIL(ALL, 0.0, true);
    SRON();
    usleep(10);

    g_strLog.str("");
    g_strLog << "=================== PE VIL TEST ====================\n";
    std::cout << g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
    
    //SEND_MPAT("cal.mpa");
    START_MPAT("INFINITY_L");
    usleep(10000);

    for(int peCh = 0; peCh < 8; peCh++) {
        diag_peSetChDiag( peCh );
        usleep(1000);
        for(int adcIdx = 2; adcIdx < 6; adcIdx++) { // IDX 2, 3, 4, 5
            adc_read(0, adcIdx, data, 1);
            adc_read(0, adcIdx, data, 10);
            for(int idx = 0; idx < 6; idx++) {
                if(chLookup[peCh][adcIdx-2][idx] != -1) {
                    int ch = chLookup[peCh][adcIdx-2][idx];
                    //data[idx] = ((data[idx] * g_adcGain[idx]) + g_adcOffset[idx]) * 2;
                    data[idx] = ((data[idx] - g_adc0Offset[idx]) / g_adc0Gain[idx]) * 2;
                    g_strLog.str("");
                    g_strLog <<"VIL CH[" << std::setw(3) << ch << "] " << "0 V Read : " << data[idx];
                    if(data[idx] < -0.010 || data[idx] > 0.050) {
                        bResult = false;
                        g_strLog << " ------- [FAIL]\n";
                    }else {
                        g_strLog << " ------- [PASS]\n";
                    }
                    std::cout<< g_strLog.str();
                    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
                }
            }
        }
    }

    diag_peSetDiagOff();
    STOP_MPAT();
    SROF();

    if(bResult == false) {
        g_bCalResult = false;
        diag_msg(0, "PE VIL Test");
    }else diag_msg(1, "PE VIL Test");
}

void PE_VIH_TEST()
{
    bool bResult = true;
    float data[8];

    g_strLog.str("");
    g_strLog << "=================== PE VIH TEST ====================\n";
    std::cout << g_strLog.str();
    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);

    load_feset_cal_H("PE_VIH_TEST");
    load_pds_high();
    VIL ( ALL ,  0.0, false );
    VIH ( ALL ,  1.0, true  );

    SRON();

    //SEND_MPAT("cal.mpa");
    START_MPAT("INFINITY_H");
    usleep(10000);
    for(int peCh = 0; peCh < 8; peCh++) {
        diag_peSetChDiag( peCh );
        usleep(10000);
        for(int adcIdx = 2; adcIdx < 6; adcIdx++) { // IDX 2, 3, 4, 5
            adc_read(0, adcIdx, data, 1);
            adc_read(0, adcIdx, data, 10);
            for(int idx = 0; idx < 6; idx++) {
                if(chLookup[peCh][adcIdx-2][idx] != -1) {
                    int ch = chLookup[peCh][adcIdx-2][idx];
                    //data[idx] = ((data[idx] + g_adcOffset[idx]) * g_adcGain[idx]) * 2;
                    data[idx] = ((data[idx] - g_adc0Offset[idx]) / g_adc0Gain[idx]) * 2;
                    g_strLog.str("");
                    g_strLog <<"VIH CH[" << std::setw(3) << ch << "] " << "1 V Read : " << std::setw(9) << std::setw(9) << std::setprecision(6) << std::fixed << data[idx];
                    if(data[idx] < 0.950 || data[idx] > 1.100) {
                        bResult = false;
                        g_strLog << " ------- [FAIL]\n";
                    }else {
                        g_strLog << " ------- [PASS]\n";
                    }
                    std::cout<< g_strLog.str();
                    fwrite(g_strLog.str().c_str(), g_strLog.str().size() , 1, g_pFileLog);
                }
            }
        }
    }
    STOP_MPAT();
    diag_peSetDiagOff();
    SROF();

    if(bResult == false) {
        g_bCalResult = false;
        diag_msg(0, "PE VIH Test");
    }else diag_msg(1, "PE VIH Test");
}

#if 0
void DPS_VS_Cal()
{
    float low = 1.0;
    float high = 4.0;
    float lowData[8];
    float highData[8];

    int idxDPS[4] = { 0, 1, 3, 4 };

    float gain[4];
    float offset[4];

    printf("===============================================\n");
     //low Level Set & adc read
    for(int idx = 0; idx < 4; idx++) {
        diag_DPS(idx, low, low * 0.9, low*1.1, 1.0 );
    }
    usleep(40000); // 20ms Delay 

    diag_DpsRun(0xf, true);
    usleep(40000); // 40ms Delay 
    adc_read(1, 7, lowData, 100);
    diag_DpsRun(0xf, false);

    // low Level Set & adc read
    for(int idx = 0; idx < 4; idx++) {
        diag_DPS(idx, high, high * 0.9, high*1.1, 1.0 );
    }
    usleep(40000); // 40ms Delay 

    diag_DpsRun(0xf, true);
    usleep(40000); // 20ms Delay 
    adc_read(1, 7, highData, 5);
    diag_DpsRun(0xf, false);

    printf("=================== 1st Data =================\n");
    for(int idx = 0 ; idx < 4 ; idx++) {
        gain[idx] = ((highData[idxDPS[idx]] - lowData[idxDPS[idx]])*2) / (high - low);
        offset[idx] = low - (((lowData[idxDPS[idx]]*2)*0.8) * gain[idx]);
        printf("DPS %d gain %0.5f offset %0.5f (low %0.5f high %0.5f)\n",idx , gain[idx], offset[idx], lowData[idxDPS[idx]], highData[idxDPS[idx]]);
    }

    std::string strMsg;
    std::cout << "Enter the any command to continue.\n";
    std::cin >> strMsg;

    //low Level Set & adc read
    for(int idx = 0; idx < 4; idx++) {
        diag_DPS(idx, (low-offset[idx]) / gain[idx] , ((low-offset[idx]) / gain[idx]) * 0.9, ((low-offset[idx]) / gain[idx])*1.1, 4.0 );
    }
    usleep(40000); // 40ms Delay 

    diag_DpsRun(0xf, true);
    usleep(40000); // 40ms Delay 
    adc_read(1, 7, lowData, 5);
    diag_DpsRun(0xf, false);

    // low Level Set & adc read
    for(int idx = 0; idx < 4; idx++) {
        diag_DPS(idx, (high-offset[idx]) / gain[idx], ((high-offset[idx]) / gain[idx]) * 0.9, ((high-offset[idx]) / gain[idx])*1.1, 1.0 );
    }
    usleep(40000); // 40ms Delay 

    diag_DpsRun(0xf, true);
    usleep(40000); // 20ms Delay 
    adc_read(1, 7, highData, 5);
    diag_DpsRun(0xf, false);

    printf("=================== 2nd Data =================\n");
    for(int idx = 0 ; idx < 4 ; idx++) {
        offset[idx] += low - (((lowData[idxDPS[idx]]*2)*0.8) * gain[idx]);
        printf("DPS %d gain %0.5f offset %0.5f (low %0.5f high %0.5f)\n",idx , gain[idx], offset[idx], lowData[idxDPS[idx]], highData[idxDPS[idx]]);
    }
}
#else

#define LP0 (0x01)
#define LP1 (0x02)
#define LP2 (0x04)
#define LP3 (0x08)
#define LP4 (0x10)

#define HP0 (0x0100)
#define HP1 (0x0200)
#define HP2 (0x0400)
#define HP3 (0x0800)
#define HP4 (0x1000)

void dps_cal_setup()
{
    double dRATE = 400.0;

    ENABLE_SCRAM(false,SCR_ALL);
    load_pds_of_dpscal("ddr4_pds");
    load_ts_of_dpscal(dRATE);
    load_feset_of_dpscal("ddr4_feset");

    IMP( CLK   , 16  );        // DR pin impedance set    
    IMP( DR    , 16  );        // DR pin impedance set    
    IMP( SCAN  , 30  );        // DR pin impedance set
    IMP( IO    , 30  );        // DR pin impedance set

    SCAN_TABLE(0 , 0x00000000                                                                                      );    //Default
    SCAN_TABLE(1 , (LP0<<16)|(LP0<<8)                                                                              );    //No1
    SCAN_TABLE(2 , (LP0<<16)|(LP0<<8)|(LP1<<16)|(LP1<<8)                                                           );    //No2
    SCAN_TABLE(3 , (LP0<<16)|(LP0<<8)                   |(LP2<<16)|(LP2<<8)                                        );    //No3
    SCAN_TABLE(4 , (LP0<<16)|(LP0<<8)|(LP1<<16)|(LP1<<8)|(LP2<<16)|(LP2<<8)                                        );    //No4
    SCAN_TABLE(5 , (LP0<<16)|(LP0<<8)                                      |(LP3<<16)|(LP3<<8)                     );    //No5
    SCAN_TABLE(6 , (LP0<<16)|(LP0<<8)|(LP1<<16)|(LP1<<8)                   |(LP3<<16)|(LP3<<8)                     );    //No6
    SCAN_TABLE(7 , (LP0<<16)|(LP0<<8)                   |(LP2<<16)|(LP2<<8)|(LP3<<16)|(LP3<<8)                     );    //No7
    SCAN_TABLE(8 , (LP0<<16)|(LP0<<8)|(LP1<<16)|(LP1<<8)|(LP2<<16)|(LP2<<8)|(LP3<<16)|(LP3<<8)|(LP4<<16)|(LP4<<8)  );    //No8

    SCAN_TABLE(9  , (HP0<<16)|(HP0>>8)                                                                              );    //No1
    SCAN_TABLE(10 , (HP0<<16)|(HP0>>8)|(HP1<<16)|(HP1>>8)                                                           );    //No2
    SCAN_TABLE(11 , (HP0<<16)|(HP0>>8)                   |(HP2<<16)|(HP2>>8)                                        );    //No3
    SCAN_TABLE(12 , (HP0<<16)|(HP0>>8)|(HP1<<16)|(HP1>>8)|(HP2<<16)|(HP2>>8)                                        );    //No4
    SCAN_TABLE(13 , (HP0<<16)|(HP0>>8)                                      |(HP3<<16)|(HP3>>8)                     );    //No5
    SCAN_TABLE(14 , (HP0<<16)|(HP0>>8)|(HP1<<16)|(HP1>>8)                   |(HP3<<16)|(HP3>>8)                     );    //No6
    SCAN_TABLE(15 , (HP0<<16)|(HP0>>8)                   |(HP2<<16)|(HP2>>8)|(HP3<<16)|(HP3>>8)                     );    //No7
    SCAN_TABLE(16 , (HP0<<16)|(HP0>>8)|(HP1<<16)|(HP1>>8)|(HP2<<16)|(HP2>>8)|(HP3<<16)|(HP3>>8)|(HP4<<16)|(HP4>>8)  );    //No8

    SCAN_TABLE(17 , (LP0<<16)|(LP0<<8)|(LP1<<16)|(LP1<<8)|(LP3<<16)|(LP3<<8)|(LP4<<16)|(LP4<<8) );    //No3

    SEND_MPAT("DPS_CAL_CHK_R00"); 
}

void DPS_LP_HP_TEST()
{
    dps_cal_setup();

    double set_vih = 4.5;
    double set_vdd = 1.0;

    VIH ( ALL  , set_vih  );  
    VIL ( ALL  , 0        );  
    VTH ( 0.5 );

    uint LP[8] = {   LP0, 
                    (LP0 | LP1), 
                    (LP0 | LP2), 
                    (LP0 | LP1 | LP2) , 
                    (LP0 | LP3), 
                    (LP0 | LP1 | LP3), 
                    (LP0 | LP2 | LP3), 
                    (LP0 | LP1 | LP2 | LP3 | LP4)};

    set_vdd += 0.5;
    DPS(1, set_vdd, 0.0, 5, 72);
    DPS(2, set_vdd, 0.0, 5, 72);
    DPS(4, set_vdd, 0.0, 5, 72);
    DPS(5, set_vdd, 0.0, 5, 72);

    for(int idx = 0; idx < 8; idx++ ) {

    }

    

}

void DPS_VS_Cal()
{
    std::string strMsg;
    float low = 1.0;
    float high = 4.0;
    float lowData[8];
    float highData[8];

    int idxDPS[4] = { 0, 1, 3, 4 };

    //diag_DpsRun(0xf, false);
    SROF();

    //40A setting
    double dRATE = 400.0;
    //double dVDD0 = 1.3;
    //double dVDD1 = 2.5;
    double dVDD0 = 2.0;
    double dVDD1 = 4.0;

    double lDropLevel = (0.2764 - 0.018 + 0.055);
    double hDropLevel = (0.2800 - 0.018 + 0.055);

//    ENABLE_SCRAM(false,SCR_ALL);
//    load_pds_of_dpscal("ddr4_pds");
//    load_ts_of_dpscal(dRATE);
//    
//    load_feset_of_dpscal("ddr4_feset");
//
//    int LP0 = 0x01;
//    int LP1 = 0x02;
//    int LP2 = 0x04;
//    int LP3 = 0x08;
//    int LP4 = 0x10;
//
//    int HP0 = 0x0100;
//    int HP1 = 0x0200;
//    int HP2 = 0x0400;
//    int HP3 = 0x0800;
//    int HP4 = 0x1000;
//
//    SCAN_TABLE(0 , 0x00000000                                                                                      );    //Default
//    SCAN_TABLE(1 , (LP0<<16)|(LP0<<8)                                                                              );    //No1
//    SCAN_TABLE(2 , (LP0<<16)|(LP0<<8)|(LP1<<16)|(LP1<<8)                                                           );    //No2
//    SCAN_TABLE(3 , (LP0<<16)|(LP0<<8)                   |(LP2<<16)|(LP2<<8)                                        );    //No3
//    SCAN_TABLE(4 , (LP0<<16)|(LP0<<8)|(LP1<<16)|(LP1<<8)|(LP2<<16)|(LP2<<8)                                        );    //No4
//    SCAN_TABLE(5 , (LP0<<16)|(LP0<<8)                                      |(LP3<<16)|(LP3<<8)                     );    //No5
//    SCAN_TABLE(6 , (LP0<<16)|(LP0<<8)|(LP1<<16)|(LP1<<8)                   |(LP3<<16)|(LP3<<8)                     );    //No6
//    SCAN_TABLE(7 , (LP0<<16)|(LP0<<8)                   |(LP2<<16)|(LP2<<8)|(LP3<<16)|(LP3<<8)                     );    //No7
//    SCAN_TABLE(8 , (LP0<<16)|(LP0<<8)|(LP1<<16)|(LP1<<8)|(LP2<<16)|(LP2<<8)|(LP3<<16)|(LP3<<8)|(LP4<<16)|(LP4<<8)  );    //No8
//
//    SCAN_TABLE(9  , (HP0<<16)|(HP0>>8)                                                                              );    //No1
//    SCAN_TABLE(10 , (HP0<<16)|(HP0>>8)|(HP1<<16)|(HP1>>8)                                                           );    //No2
//    SCAN_TABLE(11 , (HP0<<16)|(HP0>>8)                   |(HP2<<16)|(HP2>>8)                                        );    //No3
//    SCAN_TABLE(12 , (HP0<<16)|(HP0>>8)|(HP1<<16)|(HP1>>8)|(HP2<<16)|(HP2>>8)                                        );    //No4
//    SCAN_TABLE(13 , (HP0<<16)|(HP0>>8)                                      |(HP3<<16)|(HP3>>8)                     );    //No5
//    SCAN_TABLE(14 , (HP0<<16)|(HP0>>8)|(HP1<<16)|(HP1>>8)                   |(HP3<<16)|(HP3>>8)                     );    //No6
//    SCAN_TABLE(15 , (HP0<<16)|(HP0>>8)                   |(HP2<<16)|(HP2>>8)|(HP3<<16)|(HP3>>8)                     );    //No7
//    SCAN_TABLE(16 , (HP0<<16)|(HP0>>8)|(HP1<<16)|(HP1>>8)|(HP2<<16)|(HP2>>8)|(HP3<<16)|(HP3>>8)|(HP4<<16)|(HP4>>8)  );    //No8
//
//    SEND_MPAT("DPS_CAL_CHK_R00"); 

    dps_cal_setup();

    double set_vih = 6.5;
    double set_vdd = 1.0;

    //////////// No load 측정   ////////////
    SET_REG(TPH1A,   0); // No Load
    SET_REG(  S1A,   0); // No Load

    VIH ( ALL  , set_vih  );  

    //////////////   1V Force   ///////////
    DPS(1, 1, 0.0, 5, 72);
    DPS(2, 1, 0.0, 5, 72);
    DPS(4, 1, 0.0, 5, 72);
    DPS(5, 1, 0.0, 5, 72);

    usleep(200000);
    SRON();
    usleep(20000);
    adc_read(1, 7, lowData, 5);
    SROF();
    for(int idx = 0 ; idx < 4 ; idx++) {
        printf("DPS %d  No Load low %0.5f(%0.5f) \n",idx , (lowData[idxDPS[idx]] * 2) - lDropLevel, lowData[idxDPS[idx]]*2);
        lowData[idxDPS[idx]] = (lowData[idxDPS[idx]] * 2);
    }

    //////////////   4V Force   ///////////
    DPS(1, 4, 0.0, 5, 72);
    DPS(2, 4, 0.0, 5, 72);
    DPS(4, 4, 0.0, 5, 72);
    DPS(5, 4, 0.0, 5, 72);

    usleep(200000);
    SRON();
    usleep(20000);
    adc_read(1, 7, highData, 5);
    SROF();

    for(int idx = 0 ; idx < 4 ; idx++) {
        printf("DPS %d  No Load High %0.5f(%0.5f) \n",idx , (highData[idxDPS[idx]] * 2) - hDropLevel, highData[idxDPS[idx]]*2);
        highData[idxDPS[idx]] = (highData[idxDPS[idx]] * 2);
    }

    for(int idx = 0 ; idx < 4 ; idx++) {
        g_dpsVsGain[idx] = (highData[idxDPS[idx]] - lowData[idxDPS[idx]]) / (high - low);
        g_dpsVsOffset[idx] = lowData[idxDPS[idx]] - (low * g_dpsVsGain[idx]);
        printf("DPS %d gain %0.5f offset %0.5f (low %0.5f high %0.5f)\n",idx , g_dpsVsGain[idx], g_dpsVsOffset[idx], lowData[idxDPS[idx]], highData[idxDPS[idx]]);
    }


    std::cout << "1st gain offset Test.\n";
    std::cin >> strMsg;
     //////////////   1V Force   ///////////
    DPS(1, (1 - g_dpsVsOffset[0]) / g_dpsVsGain[0], 0.0, 5, 72);
    DPS(2, (1 - g_dpsVsOffset[1]) / g_dpsVsGain[1], 0.0, 5, 72);
    DPS(4, (1 - g_dpsVsOffset[2]) / g_dpsVsGain[2], 0.0, 5, 72);
    DPS(5, (1 - g_dpsVsOffset[3]) / g_dpsVsGain[3], 0.0, 5, 72);

    usleep(200000);
    SRON();
    usleep(20000);
    adc_read(1, 7, lowData, 5);
    std::cout << "---------  On.  \n";
    std::cin >> strMsg;
    SROF();
    for(int idx = 0 ; idx < 4 ; idx++) {
        printf("DPS %d  1st gain offset Test %0.5f(%0.5f) \n",idx , (lowData[idxDPS[idx]] * 2) - lDropLevel, lowData[idxDPS[idx]]*2);
        lowData[idxDPS[idx]] = (lowData[idxDPS[idx]] * 2) - lDropLevel;
    }
    std::cout << "---------  Off.  \n";
    

    ///////////////////////////////////////////////
    set_vdd = 1.0;
    DPS(1, (set_vdd - g_dpsVsOffset[0]) / g_dpsVsGain[0], 0.0, 5, 72);
    DPS(2, (set_vdd - g_dpsVsOffset[1]) / g_dpsVsGain[1], 0.0, 5, 72);
    DPS(4, (set_vdd - g_dpsVsOffset[2]) / g_dpsVsGain[2], 0.0, 5, 72);
    DPS(5, (set_vdd - g_dpsVsOffset[3]) / g_dpsVsGain[3], 0.0, 5, 72);

    VIH ( ALL  , set_vih  );  
    VIL ( ALL  , 0        );  
    VTH ( 0.5 );

    SET_REG(TPH1A,   LP0 | LP1 | LP2); // LP 1V시 40A 설정
    SET_REG(  S1A,            0x0004); // LP 1V시 40A 설정
    

    //SET_REG(TPH1A,    LP0 | LP1 | LP3 | LP4); // LP 1V시 40A 설정
    //SET_REG(  S1A,      17); // LP 1V시 40A 설정

    //SET_REG(TPH1A, HP0|HP3 );  // HP 4V시 40A 설정
    //SET_REG(  S1A, 13);        // HP 4V시 40A 설정
    SRON();
    //printf("============= 1st ==================================\n");
    
    //std::cout << "Enter the any command to continue.\n";
    //std::cin >> strMsg;

    START_MPAT("DPS_CAL_CHK_R00");


     //low Level Set & adc read
    //for(int idx = 0; idx < 4; idx++) {
    //    diag_DPS(idx, low, 0, 5, 72.0 );
    //}
    //usleep(100000); // 100ms Delay 
    //
    //diag_DpsRun(0xf, true);
    //usleep(100000); // 100ms Delay 
    
    usleep(20000);
    //sleep(2);
    adc_read(1, 7, lowData, 5);
    for(int idx = 0 ; idx < 4 ; idx++) {
        printf("DPS %d  low %0.5f(%0.5f) \n",idx , (lowData[idxDPS[idx]] * 2) - lDropLevel, lowData[idxDPS[idx]]*2);
        lowData[idxDPS[idx]] = (lowData[idxDPS[idx]] * 2) - lDropLevel;
    }

    //printf("=========== 1st STAR MPAT ==========\n");
    //std::cout << "Enter the any command to continue.\n";
    //std::cin >> strMsg;
    //sleep(10);
    WAIT_MPAT();
    STOP_MPAT();
    SROF();

    //////////////////////// Pattern 출력 Reset용 루틴
    SRON();
    SET_REG(TPH1A,      0);        // No1
    SET_REG(  S1A, 0x0000);        //
    //START_MPAT("DPS_CAL_CHK_R00");
    //sleep(2);
    MEAS_MPAT("DPS_CAL_CHK_R00");
    STOP_MPAT();
    SROF();
    ////////////////////////////////////////////////

    //diag_DpsRun(0xf, false);

    //printf("============= 2nd ==================================\n");
    //std::cout << "Enter the any command to continue.\n";
    //std::cin >> strMsg;

    set_vdd = 4.0;

    DPS(1, (set_vdd - g_dpsVsOffset[0]) / g_dpsVsGain[0], 0.0, 5.5, 72);
    DPS(2, (set_vdd - g_dpsVsOffset[1]) / g_dpsVsGain[1], 0.0, 5.5, 72);
    DPS(4, (set_vdd - g_dpsVsOffset[2]) / g_dpsVsGain[2], 0.0, 5.5, 72);
    DPS(5, (set_vdd - g_dpsVsOffset[3]) / g_dpsVsGain[3], 0.0, 5.5, 72);

    set_vih = 6.5;
    VIH ( ALL  , set_vih  ); 
    // low Level Set & adc read
    //for(int idx = 0; idx < 4; idx++) {
    //    diag_DPS(idx, high, 0, 5, 72.0 );
    //}

    usleep(1000000); // 100ms Delay 

    SET_REG(TPH1A, HP0|HP3 );  // 4V시 40A 설정
    SET_REG(  S1A, 13);        // 4V시 40A 설정
    SRON();
    
    usleep(100000); // 100ms Delay 
    //MEAS_MPAT("DPS_CAL_CHK_R00");
    //printf("=========== 2nd STAR MPAT ==========\n");
    //std::cout << "Enter the any command to continue.\n";
    //std::cin >> strMsg;

    START_MPAT("DPS_CAL_CHK_R00");

    //usleep(100000); // 40ms Delay 
    //diag_DpsRun(0xf, true);
    //usleep(100000); // 20ms Delay 

    usleep(20000); // 20ms Delay 
    //sleep(2);
    adc_read(1, 7, highData, 5);
    //diag_DpsRun(0xf, false);
    for(int idx = 0 ; idx < 4 ; idx++) {
        printf("DPS %d  High %0.5f(%0.5f) \n",idx , (highData[idxDPS[idx]] * 2) - hDropLevel, highData[idxDPS[idx]]*2);
    }

    WAIT_MPAT();
    STOP_MPAT();
    SROF();

    //printf("=================== 1st Data =================\n");
    //for(int idx = 0 ; idx < 4 ; idx++) {
    //    highData[idxDPS[idx]] = (highData[idxDPS[idx]]*2) - hDropLevel;
    //    g_dpsVsGain[idx] = (highData[idxDPS[idx]] - lowData[idxDPS[idx]]) / (high - low);
    //    g_dpsVsOffset[idx] = lowData[idxDPS[idx]] - (low * g_dpsVsGain[idx]);
    //    printf("DPS %d gain %0.5f offset %0.5f (low %0.5f high %0.5f)\n",idx , g_dpsVsGain[idx], g_dpsVsOffset[idx], lowData[idxDPS[idx]], highData[idxDPS[idx]]);
    //}

    printf("=================== Second Offset Data =================\n");
    for(int idx = 0 ; idx < 4 ; idx++) {
        highData[idxDPS[idx]] = (highData[idxDPS[idx]]*2) - hDropLevel;
        g_dpsVsGain2[idx] = (highData[idxDPS[idx]] - lowData[idxDPS[idx]]) / (high - low);
        g_dpsVsOffset2[idx] = lowData[idxDPS[idx]] - (low * g_dpsVsGain2[idx]);
        printf("DPS %d 2nd gain %0.5f offset %0.5f (low %0.5f high %0.5f) ",idx , g_dpsVsGain[idx], g_dpsVsOffset[idx], lowData[idxDPS[idx]], highData[idxDPS[idx]]);
        g_dpsVsOffset2[idx] = (g_dpsVsOffset[idx] + g_dpsVsOffset2[idx]) / 2;
        printf(" offset2 %0.5f\n",g_dpsVsOffset2[idx]);
    }

    std::cout << "Enter the any command to continue.\n";
    std::cin >> strMsg;

    int idxArr[4] = {1,2,4,5};
    ////  gain offset 적용 Test 
    set_vdd = 1.0;    
    for(int idx = 0 ; idx < 4 ; idx++) {
        //DPS(idx, (set_vdd - g_dpsVsOffset[idx]) / g_dpsVsGain[idx], 0.0, 5, 72);
        DPS(idxArr[idx], (set_vdd - g_dpsVsOffset[idx]) / g_dpsVsGain[idx] - g_dpsVsOffset2[idx], 0.0, 5, 80);
    }
    set_vih = 4.0;
    VIH ( ALL  , set_vih  );  
    usleep(30000);

    ////// No Load test
    SET_REG(TPH1A,   0); // LP 1V시 40A 설정
    SET_REG(  S1A,   0); // LP 1V시 40A 설정
    //SET_REG(TPH1A,   LP0 | LP1 | LP2 | LP3 | LP4); // LP 1V시 40A 설정
    //SET_REG(  S1A,   8); // LP 1V시 40A 설정

    std::cout << "1V No Load Wait.\n";
    std::cin >> strMsg;
    SRON();
    START_MPAT("DPS_CAL_CHK_R00");
    usleep(20000);
    adc_read(1, 7, lowData, 5);
    for(int idx = 0 ; idx < 4 ; idx++) {
        printf("DPS %d  1V No Load Test %0.5f(%0.5f) \n",idx , (lowData[idxDPS[idx]] * 2) - lDropLevel, lowData[idxDPS[idx]]*2);
        lowData[idxDPS[idx]] = (lowData[idxDPS[idx]] * 2) - lDropLevel;
    }
    WAIT_MPAT();
    STOP_MPAT();
    SROF();


    ////// 40A Load test
    SET_REG(TPH1A,   LP0 | LP1 | LP2); // LP 1V시 40A 설정
    SET_REG(  S1A,            0x0004); // LP 1V시 40A 설정
    //SET_REG(TPH1A,   LP0 | LP1 | LP2 | LP3 | LP4); // LP 1V시 40A 설정
    //SET_REG(  S1A,   8); // LP 1V시 40A 설정
    std::cout << "1V 40A Load Wait.\n";
    std::cin >> strMsg;
    SRON();
    START_MPAT("DPS_CAL_CHK_R00");
    usleep(20000);
    adc_read(1, 7, lowData, 5);
    for(int idx = 0 ; idx < 4 ; idx++) {
        printf("DPS %d  1V 40A Test %0.5f(%0.5f) \n",idx , (lowData[idxDPS[idx]] * 2) - lDropLevel, lowData[idxDPS[idx]]*2);
        lowData[idxDPS[idx]] = (lowData[idxDPS[idx]] * 2) - lDropLevel;
    }
    WAIT_MPAT();
    STOP_MPAT();
    SROF();




    ////// 40A Load test
    SET_REG(TPH1A,   LP0 | LP1 | LP2 | LP3 | LP4); // LP 1V시 80A 설정
    SET_REG(  S1A,                        0x0008); // LP 1V시 80A 설정
    //SET_REG(TPH1A,   LP0 | LP1 | LP2 | LP3 | LP4); // LP 1V시 40A 설정
    //SET_REG(  S1A,   8); // LP 1V시 40A 설정
    std::cout << "1V 80A Load Wait.\n";
    std::cin >> strMsg;
    SRON();
    START_MPAT("DPS_CAL_CHK_R00");
    usleep(20000);
    adc_read(1, 7, lowData, 5);
    for(int idx = 0 ; idx < 4 ; idx++) {
        printf("DPS %d  1V 80A Test %0.5f(%0.5f) \n",idx , (lowData[idxDPS[idx]] * 2) - lDropLevel, lowData[idxDPS[idx]]*2);
        lowData[idxDPS[idx]] = (lowData[idxDPS[idx]] * 2) - lDropLevel;
    }
    WAIT_MPAT();
    STOP_MPAT();
    SROF();



    diag_pe_off();
    return;

    std::cout << "Enter the any command to continue.\n";
    std::cin >> strMsg;

    //low Level Set & adc read
    for(int idx = 0; idx < 4; idx++) {
        diag_DPS(idx, (low - g_dpsVsOffset[idx]) / g_dpsVsGain[idx]  , 0, 5, 72.0 );
    }
    usleep(100000); // 20ms Delay 

    diag_DpsRun(0xf, true);
    usleep(100000); // 40ms Delay 
    adc_read(1, 7, lowData, 5);
    diag_DpsRun(0xf, false);

    // low Level Set & adc read
    for(int idx = 0; idx < 4; idx++) {
        diag_DPS(idx, (high - g_dpsVsOffset[idx]) / g_dpsVsGain[idx] , 0, 5, 72.0 );
    }
    usleep(100000); // 40ms Delay 

    diag_DpsRun(0xf, true);
    usleep(100000); // 20ms Delay 
    adc_read(1, 7, highData, 5);
    diag_DpsRun(0xf, false);

    printf("=================== 2nd Data =================\n");
    for(int idx = 0 ; idx < 4 ; idx++) {
        printf("DPS %d low %0.5f high %0.5f\n",idx , lowData[idxDPS[idx]], highData[idxDPS[idx]]);
    
    }
    diag_pe_off();  
}
#endif


void DPS_OC_Cal()
{
    float low = 8.0;
    float high = 40.0;
    float lowData[4];
    float highData[4];

    int idxDPS[4] = { 0, 1, 3, 4 };
    SROF();

    //40A setting
    double dRATE = 400.0;
    double dVDD0 = 2.0;
    double dVDD1 = 4.0;

    load_dps_of_dpscal(dVDD0, dVDD1);

    dps_cal_setup();

    double set_vih = 4.5;
    double set_vdd = 4.0;

    DPS(1, set_vdd, 0.0, 5, 72);
    DPS(2, set_vdd, 0.0, 5, 72);
    DPS(4, set_vdd, 0.0, 5, 72);
    DPS(5, set_vdd, 0.0, 5, 72);

    VIH ( ALL  , set_vih  );  
    VIL ( ALL  , 0        );  
    VTH ( 0.5 );

    printf("============= 1st ==================================\n");
    std::string strMsg;
    std::cout << "Enter the any command to continue.\n";
    std::cin >> strMsg;

    SRON();
    SET_REG(TPH1A, HP0);  // 4V 8A
    SET_REG(  S1A,   9);  // 4V 8A
    START_MPAT("DPS_CAL_CHK_R00");

    //low Level Set & adc read
    sleep(2);
    for(int idx = 0 ; idx < 4 ; idx++) {
        lowData[idx] = READ_IOUT(idx);
        printf("DPS %d  low %0.5f \n",idx , lowData[idx]); // 8A Read 
    }

    STOP_MPAT();
    SROF();

    //////////////////////// Pattern 출력 Reset용 루틴
    SET_REG(TPH1A,     0);        // PE LOW 출력
    SET_REG(  S1A,0x0000);        // PE LOW 출력
    SRON();
    START_MPAT("DPS_CAL_CHK_R00");
    sleep(1);
    STOP_MPAT();
    SROF();
    ////////////////////////////////////////////////

    printf("============= 2nd ==================================\n");
    std::cout << "Enter the any command to continue.\n";
    std::cin >> strMsg;

    //set_vdd = 4.0;
    //DPS(1, set_vdd, 0.0, 5.5, 72);
    //DPS(2, set_vdd, 0.0, 5.5, 72);
    //DPS(4, set_vdd, 0.0, 5.5, 72);
    //DPS(5, set_vdd, 0.0, 5.5, 72);

    usleep(100000); // 100ms Delay 

    SET_REG(TPH1A, HP0|HP3 );  // 4V시 40A 설정
    SET_REG(  S1A, 13);        // 4V시 40A 설정
    SRON();
    START_MPAT("DPS_CAL_CHK_R00");
    sleep(2);
    for(int idx = 0 ; idx < 4 ; idx++) {
        highData[idx] = READ_IOUT(idx);
        printf("DPS %d  low %0.5f \n",idx , highData[idx]); // 8A Read 
    }
    STOP_MPAT();
    SROF();

    printf("=================== 1st Data =================\n");
    for(int idx = 0 ; idx < 4 ; idx++) {
        g_dpsVsGain[idx] = ((highData[idxDPS[idx]] - lowData[idxDPS[idx]])*2) / (high - low);
        g_dpsVsOffset[idx] = (lowData[idxDPS[idx]]*2) - (low * g_dpsVsGain[idx]);
        printf("DPS %d gain %0.5f offset %0.5f (low %0.5f high %0.5f)\n",idx , g_dpsVsGain[idx], g_dpsVsOffset[idx], lowData[idxDPS[idx]]*2, highData[idxDPS[idx]]*2);
    }

    diag_pe_off();  
    return; 
}

void diag_boardTemp()
{
	bool ret = diag_select_temp_mux();
	if (ret == false){
        diag_msg( 0 , "[Board Temp Mux]  Interface Error");
        return;
    }
       
    for (byte i = 0; i<4; i++)
    {
    	float val = diag_bd_temp(i);    	    	
    	if (val == -1)
    	{
    		char msg[128]; 
            sprintf(msg, "[Board Temp %d] Interface Error", i);
    		diag_msg( 0 , msg);    	
    		continue;
    	}
    	
    	if (val < 16 || val > 100)
    	{
    		char msg[128]; 
            sprintf(msg, "[Board Temp %d] Temp Value(%.2f C) Error", i, val);
    		diag_msg( 0 , msg);    	
    	}    		
    	
    }
      
}
 

void BoardTempRead_Infinite()
{
    float temp;
    while(1) {
        printf("==========================================\n");
        for(int idx = 0; idx < 4; idx++) {

            //printf("=====================  %d  ===================\n", idx  );
            temp = diag_bd_temp(2);
            //printf("---------------------------------------------\n" );
            //temp = diag_bd_temp_r(idx);
            //printf("Board Temp %d: %0.3f\n", idx, temp );
        }
        sleep(3);
    }
}

void PE_OT_Read() 
{
    TIME(VS1 | VS2, 0, 0.1);
    //Board Temp
    uint16_t wdata;
    diag_peRead( 0,  0, 0x2, &wdata);
    wdata &= ~(0x3800);
    wdata |= 0x0200;
    //diag_peWrite(0x3FFFFF, 0, 0x2, wdata); // OT 70
    //diag_peWrite(0x1<<8, 0xFF, 0xD, 0); 

    uint32_t alDev = 0;
    uint16_t status = 0;

    while(1) {
        printf("==========================================\n");
        alDev = diag_pe_alarm_read();
        if(alDev) {
            for(int idx = 0; idx < 22; idx++ ) {
                status = diag_pe_alarm_read(0x1<<idx);
                status >>= 8;
                if(status){
                    printf("Dev %d Alarm : 0x%X\n", idx, status );
                    //diag_peRead( 0x1<<idx,  0, 0xD, &wdata);
                    //wdata &= ~(0x0010);
                    //wdata |= 0x0010;
                    //diag_peWrite(0x3FFFFF, 0xFF, 0xD, wdata); // OT 70
                }
            }
        }
        sleep(3);
    }
}

/////////////// 1V 40A Loader test
void dps_manual_cal_load_test()
{
    ////////// Read Cal data from ROM
    uint16_t addr = CAL_DPS_ADDR;
    int sz = sizeof(float) * 4;
    diag_romRead(addr, sz, (uint8_t*)g_dpsVsGain);
    addr += sz;
    diag_romRead(addr, sz, (uint8_t*)g_dpsVsOffset);
    addr += sz;
    diag_romRead(addr, sz, (uint8_t*)g_dpsVsOffset2);
    addr += sz;

    diag_romRead(addr, sz, (uint8_t*)g_dpsIMGain);
    addr += sz;
    diag_romRead(addr, sz, (uint8_t*)g_dpsIMOffset);
    addr += sz;

    int chIdx[4] = {0, 1, 3, 4};

    for (int idx = 0; idx < 4 ; idx++) {
        std::cout << "DPS " << idx << " gain : " << g_dpsVsGain[idx] << " offset : " << g_dpsVsOffset[idx] << " offset2 : " << g_dpsVsOffset2[idx] << std::endl;
    }

    float readLevel[8];

    float readCurrent[4];
    ////////// DPS VS SET 
    float setValue = 0;    
    int idxArr[4] = {1,2,4,5};
    for(int idx = 0 ; idx < 4; idx++) {
        //setValue = (1.0 - g_dpsVsOffset2[idx]) * g_dpsVsGain[idx] + g_dpsVsOffset[idx];
        setValue = (1.0 - g_dpsVsOffset[idx]) / g_dpsVsGain[idx] + g_dpsVsOffset2[idx];
        DPS(idxArr[idx], setValue, 0,  setValue + 1.0,  72);
    }

    std::cout << std::endl;

    std::string strMsg;
    
    diag_DpsRun(0xF, 1);
    std::cout << "Turn On Ready!\n";
    std::cin >> strMsg;
    //usleep(1000000);
    adc_read(1, 7, readLevel, 1);
    adc_read(1, 7, readLevel, 5);
    readCurrent[0] = READ_IOUT(0);
    readCurrent[1] = READ_IOUT(1);
    readCurrent[2] = READ_IOUT(2);
    readCurrent[3] = READ_IOUT(3);

    diag_DpsRun(0xF, 0);

    for (int idx = 0; idx < 4 ; idx++) {
        std::cout << "DPS " << idx << " " << std::setw(6) << (readLevel[chIdx[idx]] * 2) << "  ";
        std::cout << std::setw(6) << readCurrent[idx] * g_dpsIMGain[idx] + g_dpsIMOffset[idx] << "(" << readCurrent[idx] << ")"<< std::endl;
    }

}

void TEST() 
{
	printf("DIAG BUILD : %s. Ver : %s\n", DIAG_BUILD_DATE, DIAG_VER);

    writeFPGA("/star/test/pg_top.bit");
    setDefaultMpatDelay(1000 * 20); //Min 10MS Nessisery. Apply 20MS Delay in MeasMPAT (US Unit))
 //   PE_WRITE(0x1, 0x0B5F,false);
    
    load_bib("diag_144_bib");
    load_scan("diag_18scan");
    load_level("diag_level");
 
	load_dps("diag_dps");
    load_scr("diag_init");
	load_pds("diag_pds_data_data16");
    
    load_ts("diag_ts");
    load_feset("diag_feset");

//	ENABLE_SCRAM(true);
//  writeDskDelay(0x22);
//	writeCpeDelay(2,3);
    STRB_EDGE(1,60, 105); 

	DATA_MODE();
//	COMPARE_MODE();
    printf("||||||| vih, vil calibration negative voltage change |||||||\n");
    printf("||||||| vih, vil calibration negative voltage change |||||||\n");
     SRON();
    printf("diag_getMode() %x\n", diag_getMode());

//void adc_read(int nADC, int idx, float * pData, int nRepeat = 1)

    if (diag_getMode() & DIAG_PG)
    {
        g_nCompRet = 0;
       	 
        testPG_opcode();       
        testPG_alu_xbyb();      
        testPG_alu_xcyc();      
        testPG_alu_xcyc_2();    
        testPG_alu_xdyd();
        testPG_alu_xdyd_2();
        testPG_alu_xaya();      
        testPG_alu_xaya_2();    
        testPG_alu_xsys();      
        testPG_alu_xsys_2();    


        testPG_alu_xyz();               
        testPG_alu_scan();      
        testPG_alu_tp();       
        testPG_alu_vec();       
        testPG_alu_cmd();       
        testPG_alu_cypn();      
        testPG_alu_scr();
        
        diag_msg( (g_nCompRet == 0) ? 1 : 0 , "[PG DIAG]");

    }

    if (diag_getMode() & DIAG_SHMOO){

    //for(int k =0; k<256;k++)
   	//{
   	//	writePG(1<<9 | 25,k,1);	//rb_pin enable    	
        testPG_shmoo_cal();
   	//}
                                                
    }

    if (diag_getMode() & DIAG_TDR){
        testPG_tdr();
        testPG_tdr_low();  
    }
    if (diag_getMode() & DIAG_PE_CAL){
        PeCalFileOpen();

        PE_ADC0_Cal();
        sleep(2);
        PE_ADC0_TEST();
        sleep(2);
        PE_DAC_CAL();
        sleep(2);
        PE_ADC1_Cal();
        sleep(2);
        PE_ADC1_TEST();
        sleep(2);
        PE_SYS_VOLTAGE0();
        sleep(2);
        PE_SYS_VOLTAGE1();
        sleep(2);
        PE_VIL_Cal();    
        sleep(2);
        PE_VIL_TEST();
        sleep(2);
        PE_VIH_Cal();
        sleep(2);
        PE_VIH_TEST();

        PeCalFileClose();

        if(g_bCalResult)
            diag_msg( 1, "[PE CAL]");
        else 
            diag_msg( 0, "[PE CAL]");
    }
    if (diag_getMode() & DIAG_DPS_CAL){
        //DpsCalFileOpen();
//
        //DPS_VS_Cal();
//
        //DpsCalFileClose();
//
        //if(g_bDpsCalResult)
        //    diag_msg( 1, "[DPS CAL]");
        //else
        //    diag_msg( 0, "[DPS CAL]");

        //dps_manual_cal_load_test();
    }
    diag_boardTemp();

    //if (diag_getMode() & DIAG_TEST_MODE){
    //    //DpsCalFileOpen();
//
    //    //DPS_VS_Cal();
    //    //BoardTempRead_Infinite();
    //    PE_OT_Read();
//
    //    //DpsCalFileClose();
//
    //    //diag_msg( 1, "[DPS CAL]");
    //}
    //
    SROF();
    //writeFPGA("/star/test/pg_top.bit");
}  


