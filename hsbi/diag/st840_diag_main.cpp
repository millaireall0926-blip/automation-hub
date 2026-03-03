
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include "define.h"
#include "STAR_HSBI.h"
#include "DIAG_HSBI.h"

#include "sub/st840_calibration.cpp"

const char *GRADE = "STAR";
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
// Global Variable
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   

double dVDD0 = 1.3;
double dVDD1 = 2.5;
double dRATE = 400.0;
double BurnIn_Temp = 100;
int BurnIn_Time = 3600;
int Soak_Time = 100;
int dCAT = 0;
int dBIN = 0;

int xBIB = 12;
int yBIB = 16;

//int xBIB = 20;
//int yBIB = 16;
int result_SHM[100][100]={0,};    
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
// OUT Command  
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
void SYNC(int outCMD)	// Don't Remove 
{
    if(outCMD == 1)
        printf("Operand 1\n");
    if(outCMD ==2)
        printf("Operand 2\n");
}
 
	
void load_bib(const char* bib_name)
{
	
    BIB_SIZE(xBIB ,yBIB,  4, 1);
      
           //row0            ,row1             , row2            , row3            , row4            , row5            , row6            , row7            , rwo8            , row9            , row10            , row11        
	BIB(0,  0=5|4|3|2        ,1=5|4|3|2        ,0=1|0|11|10      ,1=1|0|11|10      ,0=9|8|7|6        ,1=9|8|7|6        ,0=17|16|15|14    ,1=17|16|15|14    ,0=13|12|23|24    ,1=13|12|23|24    ,0=21|20|19|18    ,1=21|20|19|18    );	//col 0
	BIB(1,  0=37|36|35|34    ,1=37|36|35|34    ,0=33|32|43|42    ,1=33|32|43|42    ,0=41|40|39|38    ,1=41|40|39|38    ,0=49|48|47|46    ,1=49|48|47|46    ,0=45|44|55|56    ,1=45|44|55|56    ,0=53|52|51|50    ,1=53|52|51|50    );	//col 1
	BIB(2,  0=69|68|67|66    ,1=69|68|67|66    ,0=65|64|75|74    ,1=65|64|75|74    ,0=73|72|71|70    ,1=73|72|71|70    ,0=81|80|79|78    ,1=81|80|79|78    ,0=77|76|87|88    ,1=77|76|87|88    ,0=85|84|83|82    ,1=85|84|83|82    );	//col 2
	BIB(3,  0=101|100|99|98  ,1=101|100|99|98  ,0=97|96|107|106  ,1=97|96|107|106  ,0=105|104|103|102,1=105|104|103|102,0=113|112|111|110,1=113|112|111|110,0=109|108|119|120,1=109|108|119|120,0=117|116|115|114,1=117|116|115|114);	//col 3
	 
	BIB(4,  0=133|132|131|130,1=133|132|131|130,0=129|128|139|138,1=129|128|139|138,0=137|136|135|134,1=137|136|135|134,0=145|144|143|142,1=145|144|143|142,0=141|140|151|152,1=141|140|151|152,0=149|148|147|146,1=149|148|147|146);   //col 4
	BIB(5,  0=165|164|163|162,1=165|164|163|162,0=161|160|171|170,1=161|160|171|170,0=169|168|167|166,1=169|168|167|166,0=177|176|175|174,1=177|176|175|174,0=173|172|183|184,1=173|172|183|184,0=181|180|179|178,1=181|180|179|178);   //col 5
	BIB(6,  0=197|196|195|194,1=197|196|195|194,0=193|192|203|202,1=193|192|203|202,0=201|200|199|198,1=201|200|199|198,0=209|208|207|206,1=209|208|207|206,0=205|204|215|216,1=205|204|215|216,0=213|212|211|210,1=213|212|211|210);   //col 6
	BIB(7,  0=229|228|227|226,1=229|228|227|226,0=225|224|235|234,1=225|224|235|234,0=233|232|231|230,1=233|232|231|230,0=241|240|239|238,1=241|240|239|238,0=237|236|247|248,1=237|236|247|248,0=245|244|243|242,1=245|244|243|242);   //col 7
	
	BIB(8,  0=261|260|259|258,1=261|260|259|258,0=257|256|267|266,1=257|256|267|266,0=265|264|263|262,1=265|264|263|262,0=273|272|271|270,1=273|272|271|270,0=269|268|279|280,1=269|268|279|280,0=277|276|275|274,1=277|276|275|274);   //col 8
	BIB(9,  0=293|292|291|290,1=293|292|291|290,0=289|288|299|298,1=289|288|299|298,0=297|296|295|294,1=297|296|295|294,0=305|304|303|302,1=305|304|303|302,0=301|300|311|312,1=301|300|311|312,0=309|308|307|306,1=309|308|307|306);   //col 9
	BIB(10, 0=325|324|323|322,1=325|324|323|322,0=321|320|331|330,1=321|320|331|330,0=329|328|327|326,1=329|328|327|326,0=337|336|335|334,1=337|336|335|334,0=333|332|343|344,1=333|332|343|344,0=341|340|339|338,1=341|340|339|338);   //col 10
	BIB(11, 0=357|356|355|354,1=357|356|355|354,0=353|352|363|362,1=353|352|363|362,0=361|360|359|358,1=361|360|359|358,0=369|368|367|366,1=369|368|367|366,0=365|364|375|376,1=365|364|375|376,0=373|372|371|370,1=373|372|371|370);   //col 11
	
	BIB(12, 0=389|388|387|386,1=389|388|387|386,0=385|384|395|394,1=385|384|395|394,0=393|392|391|390,1=393|392|391|390,0=401|400|399|398,1=401|400|399|398,0=397|396|407|408,1=397|396|407|408,0=405|404|403|402,1=405|404|403|402);   //col 12
	BIB(13, 0=421|420|419|418,1=421|420|419|418,0=417|416|427|426,1=417|416|427|426,0=425|424|423|422,1=425|424|423|422,0=433|432|431|430,1=433|432|431|430,0=429|428|439|440,1=429|428|439|440,0=437|436|435|434,1=437|436|435|434);   //col 13
	BIB(14, 0=453|452|451|450,1=453|452|451|450,0=449|448|459|458,1=449|448|459|458,0=457|456|455|454,1=457|456|455|454,0=465|464|463|462,1=465|464|463|462,0=461|460|471|472,1=461|460|471|472,0=469|468|467|466,1=469|468|467|466);   //col 14
	BIB(15, 0=485|484|483|482,1=485|484|483|482,0=481|480|491|490,1=481|480|491|490,0=489|488|487|486,1=489|488|487|486,0=497|496|495|494,1=497|496|495|494,0=493|492|503|504,1=493|492|503|504,0=501|500|499|498,1=501|500|499|498);   //col 15

}
void load_scan(const char* scan_name)
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
        SCAN_TABLE(18, 0x00040000);    //scan18 Enable
        SCAN_TABLE(19, 0x00080000);    //scan19 Enable

        SCAN_TABLE(20, 0x00000000);    //scan0~31 Disable
        SCAN_TABLE(21, 0xFFFFFFFF);    //scan0~31 Enable
}
int g_nCompRet = 0;
bool compFm(const char* name, int type, int size, ...)
{

    diag_readFmData(0);
    diag_readFmData(1);
    diag_readFmData(2);
    diag_readFmData(3);
    
    unsigned int* compArr = new unsigned int[size];
    unsigned short comp;
    va_list ap;
    va_start(ap, size);
    for(int i=0; i<size; i++)
        compArr[i] = va_arg(ap, unsigned int);
    va_end(ap);

    int fmRead = size;
    int nRet = 0;
    
    for (int k = 0; k<4; k++)
    {
        for (int i = 0; i<size; i++ )
        {
            nRet = 0;
            DIAG_FM* pFmH = (DIAG_FM*)diag_getFmDataH(k,i);
            DIAG_FM* pFmL = (DIAG_FM*)diag_getFmDataL(k,i);
        
            if (type & 0x3)
            {   // type =  DATA H, L
                for (int j =0; j<8; j++)
                {
                    unsigned short readh = (pFmH->ioComp[j*2]) | (pFmH->ioComp[j*2+1]<<8);
                    unsigned short readl = (pFmL->ioComp[j*2]) | (pFmL->ioComp[j*2+1]<<8);
                    
                    if(j%2 == 1)
                    {
                        comp = compArr[i] & 0x00ff;
                    
                        readh = readh & 0x00ff;
                        readl = readl & 0x00ff;	
                    }
                    else
                    {
                    
                        comp = compArr[i] & 0xffff;
                    
                        readh = readh & 0xffff;
                        readl = readl & 0xffff;
                    }
                    
                    if (type & 0x1)
                    {
                        if (comp != readh)
                        {
                            diag_log_pf(0, "[%s] ioH Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, j*2, readh, compArr[i]);
                            //printf("[%s] PG[%d] ioH Comp Error adr:%d, pos:%d, read:%04x, comp:%04x\n", name, k, i, j*2, readh, compArr[i]);    	                    
                            nRet = type;
                        }
                    }
	
                    if (type & 0x2)
                    {
                        if (comp != readl)
                        {
                            diag_log_pf(0, "[%s] ioL Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name,  i, j*2, readl, compArr[i]);
                            //printf("[%s] PG[%d] ioL Comp Error adr:%d, pos:%d, read:%04x, comp:%04x\n", name, k, i, j*2, readh, compArr[i]);    	                        	                    
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
                if (comp != readh)
                {
                    diag_log_pf(0, "[%s] drv Dh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 0, readh, compArr[i]);
                    //printf("[%s] PG[%d] drv Dh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x\n", name, k, i, 0, readh, compArr[i]);
                    nRet = type;
                }
                if (comp != readl)
                {
                    diag_log_pf(0, "[%s] drv Dl Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 0, readl, compArr[i]);
                    //printf("[%s] PG[%d] drv Dh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x\n", name, k, i, 0, readh, compArr[i]);
                    nRet = type;
                }
            }

    	    if (type & 0x18)        // X
            {
                unsigned short readh = (pFmH->adrDrv[0]) | (pFmH->adrDrv[1]<<8);
                unsigned short readl = (pFmL->adrDrv[0]) | (pFmL->adrDrv[1]<<8);
                unsigned short comp = compArr[i] & 0xffff;
                
                if (comp != readh)
                {
                    diag_log_pf(0, "[%s] drv Xh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 0, readh, compArr[i]);
    	            //printf("[%s] PG[%d] drv Xh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x\n", name, k, i, 0, readh, compArr[i]);
                    nRet = type;
				}
				
                if (comp != readl)
				{
                    diag_log_pf(0, "[%s] drv Xl Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 0, readl, compArr[i]);
    	            //printf("[%s] PG[%d] drv Xh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x\n", name, k, i, 0, readl, compArr[i]);
                    nRet = type;
				}
            }
    	    if (type == 0x20)        // Y
            {
                unsigned short readh = (pFmH->adrDrv[2]) | (pFmH->adrDrv[3]<<8);
                unsigned short readl = (pFmL->adrDrv[2]) | (pFmL->adrDrv[3]<<8);
                unsigned short comp = compArr[i] & 0xffff;
                if (comp != readh)
                {
                    diag_log_pf(0, "[%s] drv Yh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 2, readh, compArr[i]);
    	            //printf("[%s] PG[%d] drv Xh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x\n", name, k, i, 2, readh, compArr[i]);
                    nRet = type;
                }
                if (comp != readl)
                {
                    diag_log_pf(0, "[%s] drv Yl Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 2, readl, compArr[i]);
    	            //printf("[%s] PG[%d] drv Xh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x\n", name, k, i, 2, readl, compArr[i]);
                    nRet = type;
                }
            }
            
    	    if (type == 0x40)       // Z
            {   	    	
                unsigned short readh = (pFmH->adrDrv[4]);
                unsigned short readl = (pFmL->adrDrv[4]); 
                unsigned short comp = compArr[i] & 0xff;
                if (comp != readh)
                {
                    diag_log_pf(0, "[%s] drv Yh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 4, readh, compArr[i]);
                    //printf("[%s] PG[%d]  drv Yh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x\n", name, k, i, 4, readh, compArr[i]);
                    nRet = type;
                }
                if (comp != readl)
                {
                    diag_log_pf(0, "[%s] drv Yl Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 4, readl, compArr[i]);
                    //printf("[%s] PG[%d]  drv Yl Comp Error adr:%d, pos:%d, read:%04x, comp:%04x\n", name, k, i, 4, readl, compArr[i]);    	            
                    nRet = type;
                }
            }
    	    if (type == 0x80)       // Scan
            {
                unsigned char readh = pFmH->scan;
//  	          unsigned char readl = pFmL->scan;
                unsigned char comp = compArr[i] & 0xff;
                if (comp != readh)
                {
                    diag_log_pf(0, "[%s] drv Yh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 5, readh, compArr[i]);
                     //printf("[%s] PG[%d]  drv Yh Comp Error adr:%d, pos:%d, read:%04x, comp:%04x\n", name, k, i, 5, readh, compArr[i]);    	             
                    nRet = type;
                }
//  	           if (comp != readl){
//  	               procMsg(0, "[%s] drv Yl Comp Error adr:%d, pos:%d, read:%04x, comp:%04x", name, i, 5, readl, compArr[i]);
//  	               nRet = type;
//  	           }
            }
    	    if (type == 0x100)       // cyp
            {
                unsigned int readh = (pFmH->adrDrv[0]) | (pFmH->adrDrv[1]<<8) | (pFmH->adrDrv[2])<<16 | (pFmH->adrDrv[3]<<24);
                unsigned int readl = (pFmL->adrDrv[0]) | (pFmL->adrDrv[1]<<8) | (pFmL->adrDrv[2])<<16 | (pFmL->adrDrv[3]<<24);
                unsigned int comp = compArr[i] & 0xffffffff;
                if (comp != readh)
                {
                    diag_log_pf(0, "[%s] drv CYP Comp Error adr:%d, read:%04x, comp:%04x", name, i, readh, compArr[i]);
                     //printf("[%s] PG[%d]  drv CYP Comp Error adr:%d, read:%04x, comp:%04x\n", name, k, i, readh, compArr[i]);    	             
                    nRet = type;
                }
                if (comp != readl)
                {
                    diag_log_pf(0, "[%s] drv CYP Comp Error adr:%d, read:%04x, comp:%04x", name, i, readl, compArr[i]);
                     //printf("[%s] PG[%d]  drv CYP Comp Error adr:%d, read:%04x, comp:%04x\n", name, k, i, readl, compArr[i]);
                    nRet = type;
                }
            }
    	    if (type & 0x600)        // addr0~32 read
            {
                unsigned int readh = (pFmH->adrDrv[0]) | (pFmH->adrDrv[1]<<8)| (pFmH->adrDrv[2]<<16)| (pFmH->adrDrv[3]<<24);
                unsigned int readl = (pFmL->adrDrv[0]) | (pFmL->adrDrv[1]<<8)| (pFmL->adrDrv[2]<<16)| (pFmL->adrDrv[3]<<24);
                unsigned int comp = compArr[i] & 0xffffffff;
    
                if (type & 0x200)
                {
					if (comp != readh)
					{
                    diag_log_pf(0, "[%s] drv Xh Comp Error adr:%d, pos:%d, read:%08x, comp:%08x", name, i, 0, readh, compArr[i]);
    	             //printf("[%s] PG[%d] drv Xh Comp Error adr:%d, pos:%d, read:%08x, comp:%08x\n", name, k, i, 0, readh, compArr[i]);    	             
                    nRet = type;
					}
				}
                if (type & 0x400)
                {
					if (comp != readl)
					{
                    diag_log_pf(0, "[%s] drv Xl Comp Error adr:%d, pos:%d, read:%08x, comp:%08x", name, i, 0, readl, compArr[i]);
    	             //printf("[%s] PG[%d[ drv Xl Comp Error adr:%d, pos:%d, read:%08x, comp:%08x\n", name, k, i, 0, readl, compArr[i]);    	             
                    nRet = type;
					}
				}
            }
            
            if (nRet)
            {
                diag_showDrv(k, i, 1);
                diag_showData(k, i, 1);
                break;
            }
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

void pg_strb_cal(int pg_num, int stype, int cpe_cnt,  ...)
{
    
	int read_dq_high[cpe_cnt],read_dq_low[cpe_cnt];
	int expect_dq_high[cpe_cnt],expect_dq_low[cpe_cnt];
	int fail_cnt_high = 0;
	int fail_cnt_low = 0;
    unsigned short compH;
	unsigned short compL;
	va_list ap;
	va_start(ap, cpe_cnt);

	for(int i=0; i<cpe_cnt; i++)
        expect_dq_high[i] = va_arg(ap, int);
    for(int i=0; i<cpe_cnt; i++)
		expect_dq_low[i] = va_arg(ap, int);
    va_end(ap);
	
 //   fm_readFmData();
    
    int fmRead = diag_readFmData(pg_num);
    
    //diag_showDrv(3,0,fmRead);
    //diag_showData(3,0,fmRead);
    
	for (int i = 0; i<fmRead; i++ )
    {
        DIAG_FM* pFmH = (DIAG_FM*)diag_getFmDataH(pg_num,i);
        DIAG_FM* pFmL = (DIAG_FM*)diag_getFmDataL(pg_num,i);

		for (int j =0; j<8; j++)
        {
        
            unsigned short readh = (pFmH->ioComp[j*2]) | (pFmH->ioComp[j*2+1]<<8);
            unsigned short readl = (pFmL->ioComp[j*2]) | (pFmL->ioComp[j*2+1]<<8);
                    
            if(j%2 == 1)
            {
                compH = expect_dq_high[i] & 0x00ff;
                compL = expect_dq_low[i] & 0x00ff;
                
                readh = readh & 0x00ff;
                readl = readl & 0x00ff;	
            }
            else
            {
                compH = expect_dq_high[i] & 0xffff;
                compL = expect_dq_low[i] & 0xffff;
                
                readh = readh & 0xffff;
                readl = readl & 0xffff;
            }
			
           // printf("\n j = %d read  = %x expect = %x \n", j, readh, compH);
            
            if( stype == 0)
            {
                if(!(compH == readh))
                    fail_cnt_high++;
            }
            if( stype == 1)
            {
                if(!(compH == readl))
                    fail_cnt_low++;
            }
            if( stype == 2)
            {
                if(!(compL == readh))
                    fail_cnt_high++;
            }
            if( stype == 3)
            {
                if(!(compL == readl))
                    fail_cnt_low++;
            }
            if( stype == 4)
            {
                if(!(compH == readh))
                    fail_cnt_high++;
                if(!(compL == readl))
                    fail_cnt_low++;
            }
		}
	}
    if(( stype == 0) || ( stype == 2))
    {
    	if(fail_cnt_high > 0)
    		diag_log("*");   
//    		printf("*");   
	    else
		    diag_log("P");
//		    printf("P");
    }
    if(( stype == 1) || ( stype == 3))
    {
    	if(fail_cnt_low > 0)
    		diag_log("*");    
//    		printf("*");    
	    else
		    diag_log("P");
//		    printf("P");
    }
    if( stype == 4)
    {
    	if((fail_cnt_low > 0) | (fail_cnt_high> 0))
    		diag_log("*");    
//    		printf("*");    
	    else
		    diag_log("P");
//		    printf("P");
    }
    
    //diag_clearFmMemory();
}


void load_ts(double set_rate, const char* ts_sel)
{
    double TCK = set_rate;
    double TAC = 20;          
    
    double BEDGE1 = (TCK*0.5);    double CEDGE1 = (TCK); 
    double BEDGE2 = (TCK*0.5);    double CEDGE2 = (TCK);
    double BEDGE3 = (TCK*0.5);    double CEDGE3 = (TCK);
    double BEDGE4 = (TCK*0.5);    double CEDGE4 = (TCK);
    double BEDGE5 = (TCK*0.5);    double CEDGE5 = (TCK);
    double BEDGE6 = (TCK*0.5);    double CEDGE6 = (TCK);
    double BEDGE7 = (TCK*0.5);    double CEDGE7 = (TCK);
    double BEDGE8 = (TCK*0.5);    double CEDGE8 = (TCK);
    double BEDGE9 = (TCK*0.5);    double CEDGE9 = (TCK);

    double STRB1 = (TCK*0.5) + TAC;      double STRB129= (TCK)+TAC;

//                       TS0 ,   TS1 ,   TS2 ,   TS3 ,   TS4 ,   TS5 ,   TS6 ,   TS7 ,   TS8 ,   TS9 ,  TS10 ,  TS11 ,  TS12 ,  TS13 ,  TS14 ,  TS15
    RATE(	             TCK ,   TCK ,   TCK ,   TCK ,   TCK ,   TCK ,   TCK ,   TCK ,   TCK ,   TCK ,   TCK ,   TCK ,   TCK ,   TCK ,   TCK ,   TCK );
    EDGE_SET( 1, BCLK1  ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 ,BEDGE1 );     // A18~31    () 
    EDGE_SET( 1, CCLK1  ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 ,CEDGE1 );      
    EDGE_SET( 2, BCLK1  ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 ,BEDGE2 );     // A2~A17    (ADDR, CMD, DM)
    EDGE_SET( 2, CCLK1  ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 ,CEDGE2 );      
    EDGE_SET( 3, BCLK1  ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 ,BEDGE3 );     // CLK4~CLK7 (DQS_t, DQS_c) 
    EDGE_SET( 3, CCLK1  ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 ,CEDGE3 );      
    EDGE_SET( 7, BCLK1  ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 ,BEDGE7 );     // A0~A1     (CK_t, Ck_c) 
    EDGE_SET( 7, CCLK1  ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 ,CEDGE7 );      
    EDGE_SET( 9, BCLK1  ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 ,BEDGE9 );     // IO / DRE 
    EDGE_SET( 9, CCLK1  ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 ,CEDGE9 );      

    STRB_EDGE(1,STRB1 ,STRB129);
    
    CPE_TUNE(18, 25);
    CPE_TUNE2(14, 20);   
//    STRB_EDGE(1,150 ,350);

}

void load_feset (const char* feset_name)
{
        TSSET (  ADDR0, NRZB);     
        TSSET (  ADDR1, NRZB);     
        TSSET (  ADDR2, NRZB);     
        TSSET (  ADDR3, NRZB);     
        TSSET (  ADDR4, NRZB);     
        TSSET (  ADDR5, NRZB);     
        TSSET (  ADDR6, NRZB);     
        TSSET (  ADDR7, NRZB);     
        TSSET (  ADDR8, NRZB);     
        TSSET (  ADDR9, NRZB);     
        TSSET ( ADDR10, NRZB);     
        TSSET ( ADDR11, NRZB);     
        TSSET ( ADDR12, NRZB);     
        TSSET ( ADDR13, NRZB);     
        TSSET ( ADDR14, NRZB);     
        TSSET ( ADDR15, NRZB);     
        TSSET ( ADDR16, NRZB);     
        TSSET ( ADDR17, NRZB);     
        TSSET ( ADDR18, NRZB);     
        TSSET ( ADDR19, NRZB);     
        TSSET ( ADDR20, NRZB);     
        TSSET ( ADDR21, NRZB);     
        TSSET ( ADDR22, NRZB);     
        TSSET ( ADDR23, NRZB);     
        TSSET ( ADDR24, NRZB);     
        TSSET ( ADDR25, NRZB);  
        TSSET ( ADDR26, NRZB);     
        TSSET ( ADDR27, NRZB);    
        TSSET ( ADDR28, NRZB);  //   BG0 
        TSSET ( ADDR29, NRZB);  //   BG1 
        TSSET ( ADDR30, NRZB);  //  _RESET   
        TSSET ( ADDR31, NRZB);  //   C2, ODT1

        TSSET (  CLK0, NRZB);   //  _RAS, A16    
        TSSET (  CLK1, NRZB);   //   C0, CKE1     
        TSSET (  CLK2, NRZB);   //  _ACT     
        TSSET (  CLK3, NRZB);   //   CK  
        TSSET (  CLK4, NRZB);   //  _CK  
        TSSET (  CLK5, NRZB);   //  _WE, A14  
        TSSET (  CLK6, NRZB);   //  _CAS, A15   
        TSSET (  CLK7, NRZB);   //  CKE0 

        TSSET (  SCAN0, NRZB);   
        TSSET (  SCAN1, NRZB);   
        TSSET (  SCAN2, NRZB);   
        TSSET (  SCAN3, NRZB);   
        TSSET (  SCAN4, NRZB);   
        TSSET (  SCAN5, NRZB);   
        TSSET (  SCAN6, NRZB);   
        TSSET (  SCAN7, NRZB);   
        
        TSSET (  IO0, DNRZ);   
        TSSET (  IO1, DNRZ);   
        TSSET (  IO2, DNRZ);   
        TSSET (  IO3, DNRZ);   
        TSSET (  IO4, DNRZ);   
        TSSET (  IO5, DNRZ);   
        TSSET (  IO6, DNRZ);   
        TSSET (  IO7, DNRZ);   
        TSSET (  IO8, DNRZ);   
        TSSET (  IO9, DNRZ);   
        TSSET ( IO10, DNRZ);   
        TSSET ( IO11, DNRZ);   
        TSSET ( IO12, DNRZ);   
        TSSET ( IO13, DNRZ);   
        TSSET ( IO14, DNRZ);   
        TSSET ( IO15, DNRZ);   
        TSSET ( IO16, DNRZ);   
        TSSET ( IO17, DNRZ);   
        TSSET ( IO18, DNRZ);   
        TSSET ( IO19, DNRZ);   
        TSSET ( IO20, DNRZ);   
        TSSET ( IO21, DNRZ);   
        TSSET ( IO22, DNRZ);   
        TSSET ( IO23, DNRZ);   

        
        TSSET (  DRE, NRZB);
}

void load_dps (double set_vdd, double set_vpp)
{
//set_vdd = 1.2;
    double set_vih = set_vdd;
    double set_vil = 0.0;
    double set_vth = set_vih/2 ;
    DUTVTT( DR, set_vih/2 );
    DUTVTT( IO, set_vih/2 );
    VTT( DR, set_vih/2 );
    VTT( IO, set_vih/2 );    
    DUTVREF1 ( (set_vih/2)   );
    DUTVREF2 ( (set_vih/2)   );
    

    VIH ( DR   , set_vih  );   // ADR pin VIH set
    VIH ( IO   , set_vih  );   // IO pin VIH set
    VTH ( set_vth );           // comparator VTH set  
    
    DPS( 8, set_vdd, 0.1, 3.0, 40);                //0
    DPS( 9, set_vdd, 0.1, 3.0, 40);                //1
    DPS( 0, set_vdd, 0.1, 3.0, 40);                //2
    DPS( 2, set_vdd, 0.1, 3.0, 40);                //3
    DPS( 1, set_vpp, 0.1, 3.0, 40);                //8 vpp         
    DPS( 3, set_vdd, 0.1, 3.0, 40);                //4
    DPS( 5, set_vdd, 0.1, 3.0, 40);                //5      
    DPS( 6, set_vdd, 0.1, 3.0, 40);                //6
    DPS( 7, set_vdd, 0.1, 3.0, 40);                //7
    
    DPS( 4, set_vpp, 0.1, 3.0, 40);                //9 vpp
//--------------------------------------------------------------
    DPS(18, set_vdd, 0.1, 3.0, 40);                //10
    DPS(19, set_vdd, 0.1, 3.0, 40);                //11
    DPS(10, set_vdd, 0.1, 3.0, 40);                //12
    DPS(12, set_vdd, 0.1, 3.0, 40);                //13
    DPS(11, set_vpp, 0.1, 3.0, 40);                //18 vpp
    DPS(13, set_vdd, 0.1, 3.0, 40);                //14
    DPS(15, set_vdd, 0.1, 3.0, 40);                //15      
    DPS(16, set_vdd, 0.1, 3.0, 40);                //16
    DPS(17, set_vdd, 0.1, 3.0, 40);                //17
    DPS(14, set_vpp, 0.1, 3.0, 40);               //19 vpp
    
    //TIME(0, VS1|VS4|VS11|VS14, 100 MS);         // On/Off Sequence      
	//TIME(1, VS8|VS9|VS0|VS2|VS3|VS5|VS6|VS7|VS18|VS19|VS10|VS12|VS13|VS15|VS16|VS17, 100 MS);         // On/Off Sequence
    //TIME(2, DVREF1 | DVREF2, 100 MS);         // On/Off Sequence                            
    TIME(0, PE, 1 MS);         // On/Off Sequence                            
}

void load_pds (const char* cyp_name )
{ 
//---------------------------------------------------------------------------------------------------------------------------------------
//                  CYP0, CYP1, CYP2, CYP3, CYP4, CYP5, CYP6, CYP7, CYP8, CYP9,CYP10,CYP11,CYP12,CYP13,CYP14,CYP15
//---------------------------------------------------------------------------------------------------------------------------------------
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
        
        PDS(  CLK0,  FIXL  ,  C0  ,  C0  ,  C0  );     
        PDS(  CLK1,  FIXL  ,  C1  ,  C1  ,  C1  );     
        PDS(  CLK2,  FIXL  ,  C2  ,  C2  ,  C2  );     
        PDS(  CLK3,  FIXL  ,  C3  ,  C3  ,  C3  );     
        PDS(  CLK4,  FIXL  ,  C4  ,  C4  ,  C4  );     
        PDS(  CLK5,  FIXL  ,  C5  ,  C5  ,  C5  );     
        PDS(  CLK6,  FIXL  ,  C6  ,  C6  ,  C6  );     
        PDS(  CLK7,  FIXL  ,  C7  ,  C7  ,  C7  );     

//       PDS(  SCAN0,  SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2  );
//       PDS(  SCAN1,  SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2  );
//       PDS(  SCAN2,  SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2  );
//       PDS(  SCAN3,  SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2  );
//       PDS(  SCAN4,  SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2  );
//       PDS(  SCAN5,  SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2  );
//       PDS(  SCAN6,  SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2  );
//       PDS(  SCAN7,  SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2, SC2  );

        PDS(  SCAN0,  FIXL,  C0 , C0 ,  C0 );     
        PDS(  SCAN1,  FIXL,  C1 , C1 ,  C1 );     
        PDS(  SCAN2,  FIXL,  C2 , C2 ,  C2 );     
        PDS(  SCAN3,  FIXL,  C3 , C3 ,  C3 );     
        PDS(  SCAN4,  FIXL,  C4 , C4 ,  C4 );     
        PDS(  SCAN5,  FIXL,  C5 , C5 ,  C5 );     
        PDS(  SCAN6,  FIXL,  C6 , C6 ,  C6 );     
        PDS(  SCAN7,  FIXL,  C7 , C7 ,  C7 );     


        PDS(   IO0,   D0,	 D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15  );     
        PDS(   IO1,   D1,	 D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0   );     
        PDS(   IO2,   D2,	 D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1   );     
        PDS(   IO3,   D3,	 D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2   );     
        PDS(   IO4,   D4,	 D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3   );     
        PDS(   IO5,   D5,	 D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4   );     
        PDS(   IO6,   D6,	 D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5   );     
        PDS(   IO7,   D7,	 D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6   );     
        PDS(   IO8,   D8,	 D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7   );     
        PDS(   IO9,   D9,	 D10,	D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8   );     
        PDS(  IO10,  D10,    D11,	D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9   );     
        PDS(  IO11,  D11,    D12,	D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10  );     
        PDS(  IO12,  D12,    D13,	D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11  );     
        PDS(  IO13,  D13,    D14,	D15,D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12  );     
        PDS(  IO14,  D14,    D15,	D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13  );     
        PDS(  IO15,  D15,    D0,	D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14  );     
        PDS(  IO16,   D0,	 D1,	D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15  );     
        PDS(  IO17,   D1,	 D2,	D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0   );     
        PDS(  IO18,   D2,	 D3,	D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1   );     
        PDS(  IO19,   D3,	 D4,	D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2   );     
        PDS(  IO20,   D4,	 D5,	D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3   );     
        PDS(  IO21,   D5,	 D6,	D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4   );     
        PDS(  IO22,   D6,	 D7,	D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5   );     
        PDS(  IO23,   D7,	 D8,	D9,	D10,D11,D12,D13,D14,D15,D0,	D1,	D2,	D3,	D4,	D5,	D6   );     
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
    }
        PDS(  DRE ,  C19  );     
        PDS(  STRB,  C17  );    
}


void load_reg(const char* szRegSet)
{


}

void load_scram(const char* scr_name)
{
    ENABLE_SCRAM(false,SCR_ALL);

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

    SCRAM(y0  = ay0 );
    SCRAM(y1  = ay1 );
    SCRAM(y2  = ay2 );
    SCRAM(y3  = ay3 );
    SCRAM(y4  = ay4 );
    SCRAM(y5  = ay5 );
    SCRAM(y6  = ay6 );
    SCRAM(y7  = ay7 );
    SCRAM(y8  = ay8 );
    SCRAM(y9  = ay9 );
    SCRAM(y10 = ay10);
    SCRAM(y11 = ay11);
    SCRAM(y12 = ay12);
    SCRAM(y13 = ay13);
    SCRAM(y14 = ay14);
    SCRAM(y15 = ay15);
    SCRAM(y16 = ay16);
    SCRAM(y17 = ay17);
    SCRAM(y18 = ay18);
    SCRAM(y19 = ay19);
    SCRAM(y20 = ay20);
    SCRAM(y21 = ay21);
    SCRAM(y22 = ay22);
    SCRAM(y23 = ay23);
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

    diag_log("============ %s ============\n", __FUNCTION__);
	load_pds("diag_pds_data_data16");
	SEND_MPAT("diag_opcode.mpa");
	
    MEAS_MPAT("OPCODE_NOP_00");
    //int fm_size=diag_readFmData(3);
    compFm("OPCODE_NOP_00",(1<<0), 16, 0x0001,	0x0002,	0x0004,	0x0008,	0x0010,	0x0020,	0x0040,	0x0080,	0x0100,	0x0200,	0x0400,	0x0800,	0x1000,	0x2000,	0x4000,	0x8000);
    compFm("OPCODE_NOP_00",(1<<1), 16, 0x000A,	0x000B,	0x000C,	0x000D,	0x00A0,	0x00B0,	0x00C0,	0x00D0,	0x0A00,	0x0B00,	0x0C00,	0x0D00,	0xA000,	0xB000,	0xC000,	0xD000);
    //diag_showDrv(3,0,fm_size);
    //diag_showData(3,0,fm_size);
    
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

//    SET_TIMER(JET_TIMER,2.8 US);
    SET_TIMER(JET_TIMER,11.2 US);    
    MEAS_MPAT("OPCODE_JET_00");
    compFm("OPCODE_JET_00",(1<<0), 16, 0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006,	0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x000f,	0x0010);
    compFm("OPCODE_JET_00",(1<<1), 16, 0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006,	0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x000f,	0x0010);

/*
    MEAS_MPAT("OPCODE_OUT_00");
    //compFm("OPCODE_OUT_00",(1<<0), 16, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x000f);
    //compFm("OPCODE_OUT_00",(1<<1), 16, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x000f);
*/

    SET_REG(REF_MODE,1);
    SET_TIMER(REF_TIMER, 10 US);
//    SET_TIMER(REF_TIMER, 40 US);
    MEAS_MPAT("OPCODE_REF_00");
    compFm("OPCODE_REF_00",(1<<0), 16, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x1111);
    compFm("OPCODE_REF_00",(1<<1), 16, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e,	0x2222);

    SET_TIMER(REF_TIMER,11 US);
//    SET_TIMER(REF_TIMER,44 US);    
    MEAS_MPAT("OPCODE_REF_00");
    compFm("OPCODE_REF_01",(1<<0), 16, 0x1111, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e);
    compFm("OPCODE_REF_01",(1<<1), 16, 0x2222, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e);

    SET_REG(REF_MODE,0);
    SET_TIMER(REF_TIMER,11 US);
//    SET_TIMER(REF_TIMER,44 US);
    MEAS_MPAT("OPCODE_REF_01");
    compFm("OPCODE_REF_02",(1<<0), 18, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e, 0x0000, 0x0001, 0x0002);
    compFm("OPCODE_REF_02",(1<<1), 18, 0x0000,  0x0001,	0x0002,	0x0003,	0x0004,	0x0005,	0x0006, 0x0007,	0x0008,	0x0009,	0x000a,	0x000b,	0x000c,	0x000d,	0x000e, 0x0000, 0x0001, 0x0002);

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
/*
    COMPARE_MODE(true);
    STRB_EDGE(1, 40, 80);
    writePG(1<<9 | 23,0x2,1);	
    writePG(1<<9 | 24,0x2,1);	    	
    
    IO_MASK(0x00000000, 0x00000000, 0x00000000, 0x00000000);
    MEAS_MPAT("IO_MASK");
    compFm("IO_MASK_00",(1<<0), 16, 0x1111, 0x1111, 0x3333, 0x3333, 0x5555, 0x5555, 0x7777, 0x7777, 0x9999, 0x9999, 0xbbbb, 0xbbbb, 0xdddd, 0xdddd, 0xffff, 0xffff);
    compFm("IO_MASK_00",(1<<1), 16, 0x2222, 0x2222, 0x4444, 0x4444, 0x6666, 0x6666, 0x8888, 0x8888, 0xaaaa, 0xaaaa, 0xcccc, 0xcccc, 0xeeee, 0xeeee, 0xffff, 0xffff);

    IO_MASK(0x55555555, 0x55555555, 0x55555555, 0x55555555);
    MEAS_MPAT("IO_MASK");
    compFm("IO_MASK_55",(1<<0), 16, 0x0000, 0x0000, 0x2222, 0x2222, 0x0000, 0x0000, 0x2222, 0x2222, 0x8888, 0x8888, 0xaaaa, 0xaaaa, 0x8888, 0x8888, 0xaaaa, 0xaaaa);
    compFm("IO_MASK_55",(1<<1), 16, 0x2222, 0x2222, 0x0000, 0x0000, 0x2222, 0x2222, 0x8888, 0x8888, 0xaaaa, 0xaaaa, 0x8888, 0x8888, 0xaaaa, 0xaaaa, 0xaaaa, 0xaaaa);

    IO_MASK(0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa);
    MEAS_MPAT("IO_MASK");
    compFm("IO_MASK_aa",(1<<0), 16, 0x1111, 0x1111, 0x1111, 0x1111, 0x5555, 0x5555, 0x5555, 0x5555, 0x1111, 0x1111, 0x1111, 0x1111, 0x5555, 0x5555, 0x5555, 0x5555);
    compFm("IO_MASK_aa",(1<<1), 16, 0x0000, 0x0000, 0x4444, 0x4444, 0x4444, 0x4444, 0x0000, 0x0000, 0x0000, 0x0000, 0x4444, 0x4444, 0x4444, 0x4444, 0x5555, 0x5555);   

    IO_MASK(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
    MEAS_MPAT("IO_MASK");
//  fm_size=diag_readFmData(0);    
    compFm("IO_MASK_ff",(1<<0), 16, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
    compFm("IO_MASK_ff",(1<<1), 16, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);        
//  diag_showDrv(0,0,fm_size);
//  diag_showData(0,0,fm_size);   

    DATA_MODE();
    IO_MASK(0x00000000, 0x00000000, 0x00000000, 0x00000000);
    STRB_EDGE(1, 40, 80);
*/
}


void testPG_alu_xbyb()
{
    diag_log("============ %s ============\n", __FUNCTION__);

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
//    int fm_size=diag_readFmData();
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
//    int fm_size=diag_readFmData();
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
//	diag_fmDrvMode(1);

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

/*
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
*/
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
/*  diag_fmDrvMode ????
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
*/
}

void testPG_alu_scr()
{
    diag_log("============ %s ============\n", __FUNCTION__);
	int fm_cnt;

//	diag_fmDrvMode(0);

    SEND_MPAT("diag_scr.mpa");
    load_pds("diag_pds_data_data16");

//    MEAS_MPAT(XY_SCR_00);       
//    fm_cnt=diag_readFmData();
//    compFm("XY_SCR_00",(1<<0),16, 0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001,	0x0001);
//    compFm("XY_SCR_00",(1<<1),16, 0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000,	0x1000);

	load_scr("diag_bit_scr_00");   
    MEAS_MPAT("XY_SCR_01");       
    fm_cnt=diag_readFmData(0);
    compFm("XY_SCR_01",(1<<0),16, 0x8000,	0x8001,	0x8001,	0x8000,	0x8000,	0x8002,	0x8002,	0x8002,	0x8000,	0x8000,	0x8000,	0x8004,	0x8000,	0x8008,	0x8000,	0x8008);

    MEAS_MPAT("XY_SCR_02");       
    fm_cnt=diag_readFmData(0);
    compFm("XY_SCR_02",(1<<0),16, 0x8000,	0x8010,	0x8000,	0x8000,	0x8000,	0x8000,	0x8020,	0x8000,	0x8000,	0x8040,	0x8040,	0x8040,	0x8000,	0x8080,	0x8080,	0x8080);  
    MEAS_MPAT("XY_SCR_03");       
    fm_cnt=diag_readFmData(0);
    compFm("XY_SCR_03",(1<<0),16, 0x8000,	0x8000,	0x8100,	0x8100,	0x8000,	0x8000,	0x8200,	0x8000,	0x8000,	0x8000,	0x8400,	0x8400,	0x8000,	0x8000,	0x8000,	0x8800);
    MEAS_MPAT("XY_SCR_04");       
    fm_cnt=diag_readFmData(0);
    compFm("XY_SCR_04",(1<<0),16, 0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0xa000,	0x8000,	0xc000,	0xc000,	0xc000,	0x8000,	0x0000,	0x0000,	0x8000);
	load_scr("diag_bit_scr_01");   
    MEAS_MPAT("XY_SCR_01");       
    fm_cnt=diag_readFmData(0);
    compFm("XY_SCR_01",(1<<0),16, 0x8000,	0x8000,	0x8001,	0x8000,	0x8000,	0x8000,	0x8002,	0x8000,	0x8000,	0x8004,	0x8004,	0x8000,	0x8000,	0x8008,	0x8008,	0x8008);
    MEAS_MPAT("XY_SCR_02");       
    fm_cnt=diag_readFmData(0);
    compFm("XY_SCR_02",(1<<0),16, 0x8000,	0x8010,	0x8010,	0x8000,	0x8000,	0x8020,	0x8000,	0x8000,	0x8000,	0x8040,	0x8000,	0x8040,	0x8000,	0x8080,	0x8000,	0x8080);	
    MEAS_MPAT("XY_SCR_03");       
    fm_cnt=diag_readFmData(0);
    compFm("XY_SCR_03",(1<<0),16, 0x8000,	0x8100,	0x8100,	0x8100,	0x8000,	0x8200,	0x8200,	0x8200,	0x8000,	0x8400,	0x8400,	0x8400,	0x8000,	0x8800,	0x8800,	0x8800);	
    MEAS_MPAT("XY_SCR_04");       
    fm_cnt=diag_readFmData(0);
    compFm("XY_SCR_04",(1<<0),16, 0x8000,	0x9000,	0x9000,	0x9000,	0x8000,	0xa000,	0xa000,	0xa000,	0x8000,	0x4000,	0x8000,	0x4000,	0x8000,	0x8000,	0x8000,	0x8000);
	load_scr("diag_bit_scr_02");   
    MEAS_MPAT("XY_SCR_01");      
    fm_cnt=diag_readFmData(0);
    compFm("XY_SCR_01",(1<<0),16, 0x8000,	0x8001,	0x8000,	0x8001,	0x8000,	0x8002,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8008,	0x8000,	0x8008);
    MEAS_MPAT("XY_SCR_02");       
    fm_cnt=diag_readFmData(0);
    compFm("XY_SCR_02",(1<<0),16, 0x8000,	0x8010,	0x8000,	0x8010,	0x8000,	0x8000,	0x8000,	0x8020,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8000,	0x8080);
	ENABLE_SCRAM(false);

}
bool pg_shm_rlt(int rlp, int slp, int stype, int size,  ...)
{
    diag_readFmData(0);
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
        DIAG_FM* pFmH = (DIAG_FM*)diag_getFmDataH(0,i);
        DIAG_FM* pFmL = (DIAG_FM*)diag_getFmDataL(0,i);

        for (int j =0; j<4; j++){
            unsigned short readh = (pFmH->ioComp[j*2]) | (pFmH->ioComp[j*2+1]<<8);
            unsigned short readl = (pFmL->ioComp[j*2]) | (pFmL->ioComp[j*2+1]<<8);
//            unsigned short comp = compArr[i] & 0xcbfd;
            unsigned short comp = compArr[i] & 0xffff;

//            readh = readh & 0xcbfd;
//            readl = readl & 0xcbfd;
            readh = readh & 0xffff;
            readl = readl & 0xffff;

            if (stype==0){
                if (comp != readh){
                    //result_SHM[rlp][slp][stype]=0;
                    nRet = stype;
                }
               // else result_SHM[rlp][slp][stype]=1;
            }

            if (stype==1){
                if (comp != readl){
              	    //result_SHM[rlp][slp][stype]=0;
                    nRet = stype;
                }
                //else result_SHM[rlp][slp][stype]=1;                   
            }
        }
    }
    delete [] compArr;
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
    int fmRead = diag_readFmData(0);
	
	for (int i = 0; i<fmRead; i++ )
    {
        DIAG_FM* pFmH = (DIAG_FM*)diag_getFmDataH(0,i);
        DIAG_FM* pFmL = (DIAG_FM*)diag_getFmDataL(0,i);

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
            printf("*");    
        else
            printf("P");
    }
    else
    {
        if(fail_cnt_low > 0)
            printf("*");    
        else
            printf("P");
    }
   // diag_clearFmMemory();
}

void testPG_shmoo_cal()
{
    diag_log("========================================\n", __FUNCTION__);
    //printf("========================================\n");
    diag_log("              %s                     \n", __FUNCTION__);
    //printf("              PG Shmoo                     \n");
    diag_log("========================================\n", __FUNCTION__);
    //printf("========================================\n");
    int fm_cnt;
//	tgDelaySet();
    double strbA, strbB;
	int set_cal = 0;	

	load_pds("diag_pds_data_data16");
	SEND_MPAT("diag_out.mpa");
//    writeCpeDelay(2,2);

diag_log("                                                                                                   11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111112\n");
diag_log("         11111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001111111111222222222233333333334444444444555555555566666666667777777777888888888899999999990\n");
diag_log("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
diag_log("+---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n");
//printf("                                                                                                   11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111112\n");
//printf("         11111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001111111111222222222233333333334444444444555555555566666666667777777777888888888899999999990\n");
//printf("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
//printf("+---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n");  
/*400
printf("                        11111111111111111111111112222222222222222222222222333333333333333333333333344444444444444444444444445555555555555555555555555666666666666666666666666677777777777777777777777778\n");
printf("  112223344455666778889900011222334445566677888990001122233444556667788899000112223344455666778889900011222334445566677888990001122233444556667788899000112223344455666778889900011222334445566677888990\n");
printf("48260482604826048260482604826048260482604826048260482604826048260482604826048260482604826048260482604826048260482604826048260482604826048260482604826048260482604826048260482604826048260482604826048260\n");
printf("+---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n");  
*/
    for(int pg_num = 0; pg_num <4; pg_num++)
    {
        for(double strb0=0.0;strb0<200;strb0=strb0+1.0)
        {
            load_ts(100,"TSET1");                    
            strbA = strb0;
            STRB_EDGE(1,strbA, 120);
            MEAS_MPAT("SHMOO_NOP_00");
            pg_strb_cal(pg_num, 0, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f,0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);
        }    	
    	//printf(" cpe h expect h  pg_num = %d \n", pg_num );
        diag_log("cpe h expect h  pg_num = %d \n");
        for(double strb1=0.0;strb1<200.0;strb1=strb1+1.0)
        {
            load_ts(100,"TSET1");                    

            strbB = strb1;

            STRB_EDGE(1,70, strbB);
            MEAS_MPAT("SHMOO_NOP_00");
            pg_strb_cal(pg_num,1, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f,0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);
        }
        //printf(" cpe l expect h  pg_num = %d \n", pg_num );    	    	
        diag_log(" cpe l expect h  pg_num = %d \n");
        for(double strb0=0.0;strb0<200.0;strb0=strb0+1.0)
        {
            load_ts(100.0,"TSET1");                    

            strbA = strb0;

            STRB_EDGE(1,strbA, 120);
            MEAS_MPAT("SHMOO_NOP_00");
            pg_strb_cal(pg_num,2, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f,0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);
        }
    	//printf(" cpe h expect l  pg_num = %d \n", pg_num );
        diag_log(" cpe h expect l  pg_num = %d \n");
        for(double strb1=0.0;strb1<200.0;strb1=strb1+1.0)
        {
            load_ts(100.0,"TSET1");                    

            strbB = strb1;

            STRB_EDGE(1,70, strbB);
            MEAS_MPAT("SHMOO_NOP_00");
            pg_strb_cal(pg_num,3, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f,0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);
        }
    	//printf(" cpe l expect l  pg_num = %d \n", pg_num );    	
        diag_log(" cpe l expect l  pg_num = %d \n\n");
    	//printf("\n");
    }
  //  diag_msg(1, "[PG SHMOO CAL]");
}


void testPG_tdr()
{
    diag_log("========================================\n", __FUNCTION__);
    diag_log("              %s                     \n", __FUNCTION__);
    diag_log("========================================\n", __FUNCTION__);
    //printf("========================================\n");
    //printf("              TDR                     \n");
    //printf("========================================\n");
	int fm_cnt;
	double rate,strb0,strb1,set_vth;
	double vth_initial;
//	tgDelaySet(); 
		
	load_pds("diag_pds_data_data16");
	SEND_MPAT("diag_out.mpa");

	int io_pf_value[12*4];
	int step_cnt=0;
	double x_vth[300];

	int io_cnt=96*4;
	int xsize=50;
	int ysize=200;
	int ymsize=0;

	char io_pf_result[xsize][ysize][io_cnt];

	double xstart_val = 1.4  ;
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
//printf("                                                                                                   11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111112\n");
//printf("         11111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001111111111222222222233333333334444444444555555555566666666667777777777888888888899999999990\n");
//printf("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
//printf("+---+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n");
    for(set_vth=xstart_val; set_vth > xend_val; set_vth=set_vth-xstep_val )    
    {
    	VIH    ( DR   , 1.0   );   // ADR pin VIH set
        VIH    ( IO   , 1.0   );   // IO pin VIH set
        VTT    ( DR   , set_vth  );
        VTT    ( IO   , set_vth  );
        VTH ( set_vth );           // comparator VTH set  
        SRON();
		for( strb0=ystart_val;strb0<yend_val;strb0=strb0+ystep_val)
		{            
            load_ts(100.0,"TSET1");                    
            STRB_EDGE(1,strb0, 120);

			MEAS_MPAT("SHMOO_TDR_00", false, false);

            writePG(1<<9 | 28,0x0,1);	
            io_pf_value[0] = readPG(0, (1<<9) | 262,1);
            io_pf_value[1] = readPG(0, (1<<9) | 263,1);
            io_pf_value[2] = readPG(0, (1<<9) | 264,1);
            io_pf_value[3] = readPG(0, (1<<9) | 266,1);
            io_pf_value[4] = readPG(0, (1<<9) | 267,1);
            io_pf_value[5] = readPG(0, (1<<9) | 268,1);
            io_pf_value[6] = readPG(0, (1<<9) | 270,1);
            io_pf_value[7] = readPG(0, (1<<9) | 271,1);
            io_pf_value[8] = readPG(0, (1<<9) | 272,1);
            io_pf_value[9] = readPG(0, (1<<9) | 274,1);
            io_pf_value[10]= readPG(0, (1<<9) | 275,1);
            io_pf_value[11]= readPG(0, (1<<9) | 276,1);
            
            io_pf_value[0+12] = readPG(1, (1<<9) | 262,1);
            io_pf_value[1+12] = readPG(1, (1<<9) | 263,1);
            io_pf_value[2+12] = readPG(1, (1<<9) | 264,1);
            io_pf_value[3+12] = readPG(1, (1<<9) | 266,1);
            io_pf_value[4+12] = readPG(1, (1<<9) | 267,1);
            io_pf_value[5+12] = readPG(1, (1<<9) | 268,1);
            io_pf_value[6+12] = readPG(1, (1<<9) | 270,1);
            io_pf_value[7+12] = readPG(1, (1<<9) | 271,1);
            io_pf_value[8+12] = readPG(1, (1<<9) | 272,1);
            io_pf_value[9+12] = readPG(1, (1<<9) | 274,1);
            io_pf_value[10+12]= readPG(1, (1<<9) | 275,1);
            io_pf_value[11+12]= readPG(1, (1<<9) | 276,1);
            
            io_pf_value[0+24] = readPG(2, (1<<9) | 262,1);
            io_pf_value[1+24] = readPG(2, (1<<9) | 263,1);
            io_pf_value[2+24] = readPG(2, (1<<9) | 264,1);
            io_pf_value[3+24] = readPG(2, (1<<9) | 266,1);
            io_pf_value[4+24] = readPG(2, (1<<9) | 267,1);
            io_pf_value[5+24] = readPG(2, (1<<9) | 268,1);
            io_pf_value[6+24] = readPG(2, (1<<9) | 270,1);
            io_pf_value[7+24] = readPG(2, (1<<9) | 271,1);
            io_pf_value[8+24] = readPG(2, (1<<9) | 272,1);
            io_pf_value[9+24] = readPG(2, (1<<9) | 274,1);
            io_pf_value[10+24]= readPG(2, (1<<9) | 275,1);
            io_pf_value[11+24]= readPG(2, (1<<9) | 276,1);
            
            io_pf_value[0+36] = readPG(3, (1<<9) | 262,1);
            io_pf_value[1+36] = readPG(3, (1<<9) | 263,1);
            io_pf_value[2+36] = readPG(3, (1<<9) | 264,1);
            io_pf_value[3+36] = readPG(3, (1<<9) | 266,1);
            io_pf_value[4+36] = readPG(3, (1<<9) | 267,1);
            io_pf_value[5+36] = readPG(3, (1<<9) | 268,1);
            io_pf_value[6+36] = readPG(3, (1<<9) | 270,1);
            io_pf_value[7+36] = readPG(3, (1<<9) | 271,1);
            io_pf_value[8+36] = readPG(3, (1<<9) | 272,1);
            io_pf_value[9+36] = readPG(3, (1<<9) | 274,1);
            io_pf_value[10+36]= readPG(3, (1<<9) | 275,1);
            io_pf_value[11+36]= readPG(3, (1<<9) | 276,1);
            
			for(int io_cnt=0;io_cnt<48;io_cnt++)
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
        SROF();
	
		ysize=0;
		xsize++;
		x_vth[xsize-1] = set_vth;		
	}
		
    for(int iostart=0; iostart<96*4; iostart++)
	{
		diag_log("io pin = %d \n",iostart);
//		printf("io pin = %d \n",iostart);		
		for(double ystart=ystart_val; ystart<yend_val; ystart=ystart+ystep_val)
		{	
			if((y_valcnt+1)%5==1)
				diag_log("%3.1f ",ystart);
//				printf("%3.1f ",ystart);
		y_valcnt++;
		}		
		y_valcnt=0;
		diag_log("\n");
//		printf("\n");		

		for(int ystart=0; ystart<ymsize; ystart++)
		{
			if((ystart+1)%5==1)
				
				diag_log("+");
//				printf("+");				
			else
				diag_log("-");
//				printf("-");				
		}	
		diag_log("\n");
//		printf("\n");
				
		for(int xstart=0; xstart<xsize; xstart++)
		{ 	
			for(int ystart=0; ystart<ymsize; ystart++)
			{
				diag_log("%c",io_pf_result[xstart][ystart][iostart]); 
//				printf("%c",io_pf_result[xstart][ystart][iostart]); 				
			}
			diag_log(" %.3f[%d] ",x_vth[xstart],xstart);
			diag_log("\n");
//			printf(" %.3f[%d] ",x_vth[xstart],xstart);
//			printf("\n");
		}
		diag_log("\n");
//		printf("\n");		
	}
    diag_msg(1, "[PG TDR]"); 
}
#define MAX_DUT 320
#define X_ARRAY 100
#define Y_ARRAY 100
void x_axis(double x_start, double x_end, double x_step, int x_lp)
{
    char xs[100];
    char axis0[x_lp], axis1[x_lp], axis2[x_lp], axis3[x_lp];
    double x_num;
    int xprint=0;
    for(int x=0; x<=x_lp; x++)
    {
        x_num=x_start+x_step*x;
//printf("x_num=%f\n", x_num);
        sprintf(xs, "%f", x_num);

        axis0[x]=xs[0];
        axis1[x]=xs[1];
        axis2[x]=xs[2];
        axis3[x]=xs[3];
//printf("axis0[%d]=%c, axis1[%d]=%c, axis2[%d]=%c, axis3[%d]=%c, num=%f\n", x, axis0[x], x, axis1[x], x, axis2[x], x, axis3[x], x_num);
    }
    printf("        ");
    for(xprint=0; xprint<=x_lp; xprint++)
    {
        printf("%c", axis0[xprint]);
        if(xprint==x_lp) printf("\n");
    }
    printf("        ");
    for(xprint=0; xprint<=x_lp; xprint++)
    {
        printf("%c", axis1[xprint]);
        if(xprint==x_lp) printf("\n");
    }
    printf("        ");
    for(xprint=0; xprint<=x_lp; xprint++)
    {
        printf("%c", axis2[xprint]);
        if(xprint==x_lp) printf("\n");
    }
    printf("        ");
    for(xprint=0; xprint<=x_lp; xprint++)
    {
        printf("%c", axis3[xprint]);
        if(xprint==x_lp) printf("\n");
    }    
//    printf("\n");
}

void tCK_shm()
{
    double rate_s, rate_e;
    double strb_s, strb_e, strbA, strbB;
    double rate_step, strb_step;
    int rate_lp, strb_lp;
    int pf_result;
    
    rate_s     = 50.0;
    rate_e     = 900.0;
    rate_step  = 50.0;    
    rate_lp    = 0;

    strb_s    = 0.0;
    strb_e    = 500.0;
    strb_step = 5.0;
    strb_lp   = 0;
    COMPARE_MODE();
	load_pds("diag_pds_data_data16");
    SEND_MPAT("diag_out.mpa");
    printf("\nTCK SHMOO\n");    
    for(double rate_set=rate_s; rate_set<=rate_e; rate_set=rate_set+rate_step)
    {
//        printf("RATE : %.2f\n", rate_set);
        load_ts(rate_set, "TSET1");
        strb_lp=0;        
        for(double strb_set=strb_s; strb_set<=strb_e; strb_set=strb_set+strb_step)        
        {
            STRB_EDGE(1, strb_set, rate_set/2+strb_set );
            MEAS_MPAT("SHMOO_NOP_00");  
            pf_result=diag_readFmData(0);
            if(pf_result==0) result_SHM[rate_lp][strb_lp]=1;
            else result_SHM[rate_lp][strb_lp]=0;       
//            pg_shm_rlt(rate_lp, strb_lp, 0, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f);
//            pg_shm_rlt(rate_lp, strb_lp, 1, 10, 0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);            
//            printf("RATE=%4.2f, strb0=%4.2f, strb1=%4.2f, Result=%d\n", rate_v, strb_v, strb_v+rate_v/2,fmRead);
            if(strb_set==strb_e) break;
            strb_lp++;
        }
        if(rate_set==rate_e) break;        
        rate_lp++;
    }
//            printf("                                              11111111111111111111111111111111111111111\n");
//            printf("        2223333344444555556666677777888889999900000111112222233333444445555566666777778\n");
//            printf("        4680246802468024680246802468024680246802468024680246802468024680246802468024680\n");
//            printf("        0000000000000000000000000000000000000000000000000000000000000000000000000000000\n");
//            printf("        +--+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n");
    x_axis(strb_s, strb_e, strb_step, strb_lp);
//printf("rate lp:%d, strb lp:%d\n",rate_lp,strb_lp);
    for(int rate_shm=rate_lp; rate_shm>=0; rate_shm--)
//            for(int rate_shm=rate_lp_s; rate_shm<rate_lp_e; rate_shm++)            
    {
            printf("%4.1fns ", rate_s+rate_step*rate_shm);
            for(int strb_shm=0; strb_shm<strb_lp; strb_shm++)
            {
//            	printf("%4.1f : %d  ", strb_s+strb_step*strb_shm, result_SHM[rate_shm][strb_shm][0]);
//            	printf("%4.1f : %d\n", strb_s+strb_step*strb_shm, result_SHM[rate_shm][strb_shm][1]);            	
                if(result_SHM[rate_shm][strb_shm]==1) printf("P");
                else printf("*");
//                printf("       ");
            }
            printf("\n");
    }
    DATA_MODE();
}

void vth_strb_shm()
{
    double vth_s, vth_e;
    double strb_s, strb_e, strbA, strbB;
    double vth_step, strb_step;
    int vth_lp, strb_lp;
    int pf_result;
    
    vth_s     = 0.0;
    vth_e     = 1.0;
    vth_step  = 0.1;    
    vth_lp    = 0;

    strb_s    = 0.0;
    strb_e    = 100.0;
    strb_step = 5.0;
    strb_lp   = 0;
    COMPARE_MODE();
	load_pds("diag_pds_data_data16");
    SEND_MPAT("diag_out.mpa");
    printf("\nVTH STRB SHMOO\n");    
    for(double vth_set=vth_s; vth_set<=vth_e; vth_set=vth_set+vth_step)
    {
//        printf("VTH : %.2f\n", vth_set);
        VIH ( DR   , 1.0  );   // ADR pin VIH set
        VIH ( IO   , 1.0  );   // IO pin VIH set
        VTH ( vth_set );
        
        strb_lp=0;        
        for(double strb_set=strb_s; strb_set<=strb_e; strb_set=strb_set+strb_step)        
        {
            STRB_EDGE(1, strb_set, 50.0+strb_set );
            MEAS_MPAT("SHMOO_NOP_00");  
            pf_result=diag_readFmData(0);
            if(pf_result==0) result_SHM[vth_lp][strb_lp]=1;
            else result_SHM[vth_lp][strb_lp]=0;
            
//            pg_shm_rlt(rate_lp, strb_lp, 0, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f);
//            pg_shm_rlt(rate_lp, strb_lp, 1, 10, 0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);            
//            printf("RATE=%4.2f, strb0=%4.2f, strb1=%4.2f, Result=%d\n", rate_v, strb_v, strb_v+rate_v/2,fmRead);
            if(strb_set==strb_e) break;
            strb_lp++;
        }
        if(vth_set==vth_e) break;        
        vth_lp++;
    }
//            printf("                                              11111111111111111111111111111111111111111\n");
//            printf("        2223333344444555556666677777888889999900000111112222233333444445555566666777778\n");
//            printf("        4680246802468024680246802468024680246802468024680246802468024680246802468024680\n");
//            printf("        0000000000000000000000000000000000000000000000000000000000000000000000000000000\n");
//            printf("        +--+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n");
    x_axis(strb_s, strb_e, strb_step, strb_lp);

    for(int vth_shm=vth_lp; vth_shm>=0; vth_shm--)
    {
            printf("%4.1fV   ", vth_s+vth_step*vth_shm);
            for(int strb_shm=0; strb_shm<strb_lp; strb_shm++)
            {
//            	printf("%4.1f : %d  ", strb_s+strb_step*strb_shm, result_SHM[rate_shm][strb_shm][0]);
//            	printf("%4.1f : %d\n", strb_s+strb_step*strb_shm, result_SHM[rate_shm][strb_shm][1]);            	
                if(result_SHM[vth_shm][strb_shm]==1) printf("P");
                else printf("*");
//                printf("       ");
            }
            printf("\n");
    }
    DATA_MODE();
}

void vih_vth_shm()
{
    double vih_s, vih_e;
    double vth_s, vth_e;
    double vih_step, vth_step;
    int vih_lp, vth_lp;
    int pf_result;
    
    vih_s     = 0.1;
    vih_e     = 1.5;
    vih_step  = 0.1;    
    vih_lp    = 0;

    vth_s    = 0.2;
    vth_e    = 1.0;
    vth_step = 0.1;
    vth_lp   = 0;
    
	load_pds("diag_pds_data_data16");
    SEND_MPAT("diag_out.mpa");
    COMPARE_MODE();
    SROF();
    printf("\nVIH VTH SHMOO\n");    
    for(double vih_set=vih_s; vih_set<=vih_e; vih_set=vih_set+vih_step)
    {
//      printf("VTH : %.2f\n", vth_set);
        VIH ( DR   , vih_set  );   // ADR pin VIH set
        VIH ( IO   , vih_set  );   // IO pin VIH set
        DUTVTT( DR, vih_set);
        DUTVTT( IO, vih_set);
        VTT( DR, vih_set);
        VTT( IO, vih_set);    
        
        vth_lp=0;        
        for(double vth_set=vth_s; vth_set<=vth_e; vth_set=vth_set+vth_step)        
        {
            VTH ( vth_set );
            SRON();                                
            MEAS_MPAT("SHMOO_NOP_00");  
            pf_result=diag_readFmData(0);
            if(pf_result==0) result_SHM[vih_lp][vth_lp]=1;
            else result_SHM[vih_lp][vth_lp]=0;       
//            pg_shm_rlt(rate_lp, strb_lp, 0, 10, 0xf0f0, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0f0f);
//            pg_shm_rlt(rate_lp, strb_lp, 1, 10, 0x0f0f, 0x000B, 0x000C, 0x000D, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x0A00, 0xf0f0);            
//            printf("RATE=%4.2f, strb0=%4.2f, strb1=%4.2f, Result=%d\n", rate_v, strb_v, strb_v+rate_v/2,fmRead);
            if(vth_set==vth_e) break;
            vth_lp++;
            SROF();
        }
        if(vih_set==vih_e) break;        
        vih_lp++;
    }
//            printf("                                              11111111111111111111111111111111111111111\n");
//            printf("        2223333344444555556666677777888889999900000111112222233333444445555566666777778\n");
//            printf("        4680246802468024680246802468024680246802468024680246802468024680246802468024680\n");
//            printf("        0000000000000000000000000000000000000000000000000000000000000000000000000000000\n");
//            printf("        +--+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+\n");
    x_axis(vth_s, vth_e, vth_step, vth_lp);

    for(int vih_shm=vih_lp; vih_shm>=0; vih_shm--)
    {
            printf("%4.1fV   ", vih_s+vih_step*vih_shm);
            for(int vth_shm=0; vth_shm<vth_lp; vth_shm++)
            {
//            	printf("%4.1f : %d  ", strb_s+strb_step*strb_shm, result_SHM[rate_shm][strb_shm][0]);
//            	printf("%4.1f : %d\n", strb_s+strb_step*strb_shm, result_SHM[rate_shm][strb_shm][1]);            	
                if(result_SHM[vih_shm][vth_shm]==1) printf("P");
                else printf("*");
//                printf("       ");
            }
            printf("\n");
    }
    DATA_MODE();
}



// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
// Test Condition
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   

void SETUP()
{
    
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
// Contact Flow
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
void CONTACT()
{

}

// ##############################################################################################################################################################################   
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
// Run Flow
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   
void TEST() 
{
    bool bPass = true;
    

    if(diag_readInuse()) {
        diag_msg(1, "BIB is Inserted."); 
        return;
    }

    uint16_t mode  = diag_getMode();

    int bCaled = 0;

    writeFPGA("/home/pc/samba/PciOpenTest/tg_top_241204.bit",0);  // 1 = pg upload 0 = tg upload
    //writeFPGA("/home/pc/samba/PciOpenTest/pg_top_241212.bit",1);  // 1 = pg upload 0 = tg upload
    writeFPGA("/home/pc/samba/PciOpenTest/pg_top_250227_1.bit",1);  // 1 = pg upload 0 = tg upload

    //init cal data
    initRefData();
    initAdcCalData();
    initDacCalData();
    initDpsCalData();
    //read cal data
    ReadCalFactor();
    
        //if(mode & DIAG_CAL ) {
    //    printf("============= CAL DIAG ===========\n");
    //    //CALIBRATION INIT /////////////////
    //    RUN_STEP(INIT_ADC_CAL_DATA);
    //    RUN_STEP(INIT_DPS_CAL_DATA);
    //    RUN_STEP(INIT_DAC_CAL_DATA);
//
    //    //CALIBRATION START /////////////////
    //    RUN_STEP(ADC_CAL);   //SW 구조상 DIAG시에만 ADC CAL 적용함
    //    RUN_STEP(DPS_CAL);
    //    RUN_STEP(DPS_I_CAL);
    //    RUN_STEP(DUTVTT_CAL);
    //    RUN_STEP(IOVTT_CAL);
    //    RUN_STEP(DUTVREF_CAL);
    //    RUN_STEP(PG_DRIO_CAL);
    //    RUN_STEP(PG_VTH_CAL);   
//
    //    //CALIBRATION APLLY /////////////////
    //    RUN_STEP(MAKE_ADC_CAL_DATA);
    //    RUN_STEP(MAKE_DPS_CAL_DATA);
    //    RUN_STEP(MAKE_DAC_CAL_DATA);
    //}

    if(mode & DIAG_CAL ) {
        //printf("============= CAL DIAG ===========\n");
        diag_log("Calibration Start.\n");
        //CALIBRATION INIT /////////////////
        diag_log("Calibration Data Init.\n");
        initAdcCalData();
        initDpsCalData(); 
        initDacCalData();

        //diag_log("=====================  Calibration Data Read.\n");
        //ReadCalFactor();
        //diag_log("=====================  Calibration Data Read END.\n");

        //CALIBRATION START /////////////////
        diag_log("ADC Calibration.\n");
        bPass = adc_cal();   //SW 구조상 DIAG시에만 ADC CAL 적용함
        if(bPass)
            diag_msg(1, "[ADC Calibration]");
        else 
            diag_msg(0, "[ADC Calibration]");
        bPass = dps_cal();
        if(bPass)
            diag_msg(1, "[DPS V Calibration]");
        else 
            diag_msg(0, "[DPS V Calibration]");
        bPass = dps_i_cal();
        if(bPass)
            diag_msg(1, "[DPS I Calibration]");
        else 
            diag_msg(0, "[DPS I Calibration]");
        bPass = dutvtt_cal();
        if(bPass)
            diag_msg(1, "[dvtt Calibration]");
        else 
            diag_msg(0, "[dvtt Calibration]");
        bPass = iovtt_cal();
        if(bPass)
            diag_msg(1, "[iovtt Calibration]");
        else 
            diag_msg(0, "[iovtt Calibration]");
        bPass = dutvref_cal();
        if(bPass)
            diag_msg(1, "[dvref Calibration]");
        else 
            diag_msg(0, "[dvref Calibration]");
        bPass = pg_drio_cal();
        if(bPass)
            diag_msg(1, "[dr/io Calibration]");
        else 
            diag_msg(0, "[dr/io Calibration]");
        bPass = pg_vth_cal();
        if(bPass)
            diag_msg(1, "[vth Calibration]");
        else 
            diag_msg(0, "[vth Calibration]");

        //CALIBRATION APLLY /////////////////
        makeAdcCalData();
        makeDpsCalData();
        makeDacCalData();
        bCaled = 1;
        
        diag_msg(1, "Calibration Data Applied.");
        bPass = MakeCalFile();
        if(bPass)
            diag_msg(1, "Successfully created calibration file.");
        else
            diag_msg(1, "Failed to create calibration file.");
    }

    //printf("============= DIAG TEST START ===========\n");
    //if(mode & DIAG_SYS ) {
    //    RUN_STEP(INIT_ADC_CAL_DATA);
    //    RUN_STEP(ADC_CAL);
    //    RUN_STEP(SYS_VOLT_DIAG);
    //}

    if(mode & DIAG_SYS ) {
        diag_log("SYSTEM Voltage Diag Start.\n");
        //adc_cal();
        bPass = sys_volt_diag();
        if(bPass)
            diag_msg(1, "[SYSTEM Voltage Diag]");
        else 
            diag_msg(0, "[SYSTEM Voltage Diag]");
    }

    //DIAGNOSTIC /////////////////
    //if(mode & DIAG_POWER ) {
    //    printf("============= POWER DIAG ===========\n");
    //    RUN_STEP(ADC_DIAG);
    //    RUN_STEP(SYS_VOLT_DIAG);
    //    RUN_STEP(DPS_DIAG);
    //    RUN_STEP(DPS_I_DIAG);
    //    RUN_STEP(DUTVTT_DIAG);
    //    RUN_STEP(IOVTT_DIAG);
    //    RUN_STEP(DUTVREF_DIAG);
    //    RUN_STEP(PG_DRIO_DIAG);
    //    RUN_STEP(PG_VTH_DIAG);
    //}

    if(mode & DIAG_POWER ) {
        //printf("============= POWER DIAG ===========\n");
        diag_log("DPS and Level Diagnostic Start.\n");
        bPass = adc_diag();
        if(bPass)
            diag_msg(1, "[ADC Diagnostic]");
        else 
            diag_msg(0, "[ADC Diagnostic]");
        bPass = sys_volt_diag();
        if(bPass)
            diag_msg(1, "[System Voltage Diagnostic]");
        else 
            diag_msg(0, "[System Voltage Diagnostic]");
        bPass = dps_diag(!bCaled);
        if(bPass)
            diag_msg(1, "[DPS Voltage Diagnostic]");
        else 
            diag_msg(0, "[DPS Voltage Diagnostic]");
        bPass = dps_i_diag(!bCaled);
        if(bPass)
            diag_msg(1, "[DPS Current Diagnostic]");
        else 
            diag_msg(0, "[DPS Current Diagnostic]");
        bPass = dutvtt_diag();
        if(bPass)
            diag_msg(1, "[DUTVTT Diagnostic]");
        else 
            diag_msg(0, "[DUTVTT Diagnostic]");
        bPass = iovtt_diag();
        if(bPass)
            diag_msg(1, "[IOVTT Diagnostic]");
        else 
            diag_msg(0, "[IOVTT Diagnostic]");
        bPass = dutvref_diag();
        if(bPass)
            diag_msg(1, "[DUTVREF Diagnostic]");
        else 
            diag_msg(0, "[DUTVREF Diagnostic]");
        bPass = pg_drio_diag();
        if(bPass)
            diag_msg(1, "[DR/IO Diagnostic]");
        else 
            diag_msg(0, "[DR/IO Diagnostic]");
        bPass = pg_vth_diag();
        if(bPass)
            diag_msg(1, "[VTH Diagnostic]");
        else 
            diag_msg(0, "[VTH Diagnostic]");
    }


    load_scan("ddr4_scan");
    
    load_ts(100.0,"TSET1");                    
    load_pds("PDS1");
    load_feset("FESET1");
    load_dps(1.00, 1.00);
    
    IO_MASK(0xFF000000,0xFF000000,0xFF000000,0xFF000000);  

//  COMPARE_MODE();

    

    

    DATA_MODE();
    
    
    SRON();
    
    if((mode & DIAG_PG ) != 0) {
        testPG_opcode();  //pg
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
//      testPG_alu_vec();       
        testPG_alu_cmd();       
        testPG_alu_cypn();      
        testPG_alu_scr();

        diag_msg( (g_nCompRet == 0 ? 1 : 0 ), "[PG_DIAG]");
    }
    
    if((mode & DIAG_SHMOO ) != 0) {
        testPG_shmoo_cal();  //shmoo
    }

    if((mode & DIAG_TDR ) != 0) {
        testPG_tdr();         //tdr
    }
    
    SROF();
}
