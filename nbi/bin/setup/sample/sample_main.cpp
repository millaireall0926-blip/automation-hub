
#include "STAR_TEST.h"

void load_bib() 
{
    BIB_SIZE( 10, 8,  4, 1);
    //     row0     row1     row2     row3     row4      row5      row6      row7      row8      row9      row10     row11
    BIB(7, 0=28-31  ,1=28-31  ,2=28-31  ,3=28-31  ,4=28-31  ,5=28-31  ,6=28-31  ,7=28-31  ,8=28-31  ,9=28-31); //col7
    BIB(6, 0=24-27  ,1=24-27  ,2=24-27  ,3=24-27  ,4=24-27  ,5=24-27  ,6=24-27  ,7=24-27  ,8=24-27  ,9=24-27); //col6
    BIB(5, 0=20-23  ,1=20-23  ,2=20-23  ,3=20-23  ,4=20-23  ,5=20-23  ,6=20-23  ,7=20-23  ,8=20-23  ,9=20-23); //col5
	BIB(4, 0=16-19  ,1=16-19  ,2=16-19  ,3=16-19  ,4=16-19  ,5=16-19  ,6=16-19  ,7=16-19  ,8=16-19  ,9=16-19); //col4
	BIB(3, 0=12-15  ,1=12-15  ,2=12-15  ,3=12-15  ,4=12-15  ,5=12-15  ,6=12-15  ,7=12-15  ,8=12-15  ,9=12-15); //col3
	BIB(2, 0=8-11   ,1=8-11   ,2=8-11   ,3=8-11   ,4=8-11   ,5=8-11   ,6=8-11   ,7=8-11   ,8=8-11   ,9=8-11 ); //col2
	BIB(1, 0=4-7    ,1=4-7    ,2=4-7    ,3=4-7    ,4=4-7    ,5=4-7    ,6=4-7    ,7=4-7    ,8=4-7    ,9=4-7  ); //col1
	BIB(0, 0=0-3    ,1=0-3    ,2=0-3    ,3=0-3    ,4=0-3    ,5=0-3    ,6=0-3    ,7=0-3    ,8=0-3    ,9=0-3  ); //col0
}

void load_scan()
{
    SCAN_TABLE(0 , 0x00000001);    
    SCAN_TABLE(1 , 0x00000002);    
    SCAN_TABLE(2 , 0x00000004);    
    SCAN_TABLE(3 , 0x00000008);    
    SCAN_TABLE(4 , 0x00000010);    
    SCAN_TABLE(5 , 0x00000020);    
    SCAN_TABLE(6 , 0x00000040);    
    SCAN_TABLE(7 , 0x00000080);    
    SCAN_TABLE(8 , 0x00000100);    
    SCAN_TABLE(9 , 0x00000200);    
    SCAN_TABLE(10, 0xFFFFFFFF);    //scan0~31 Enable

}
void load_ts()
{

    double rate   = 100.0;
	double bedge0 = 0.0;
	double cedge0 = (rate/2.0)+bedge0;
    double rate1  = 100.0;
	double bedge1 = 0.0;
	double cedge1 = (rate1/2.0)+bedge1;
    double strb0 = 45.0;
    double strb1 = 95.0;
     //            TS1,   TS2,   TS3,   TS4,   TS5,   TS6,   TS7,   TS8,   TS9,  TS10,  TS11,  TS12,  TS13,  TS14,  TS15,  TS16,     RATE(	    rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate,  rate);    ECLK( BCLK1  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK1  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);     ECLK( BCLK2  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK2  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);     ECLK( BCLK3  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
    ECLK( CCLK3  ,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0,cedge0);     ECLK( BCLK4  ,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0,bedge0);
}

void load_feset()
{
    // ADDR 0~31
    TSSET (  ADDR0, NRZB, ECLK1 );     
    TSSET (  ADDR1, NRZB, ECLK1 );     
    TSSET (  ADDR2, NRZB, ECLK1 );     
    // CLK0~7
    TSSET (  CLK0, _RZO, ECLK1 );       
    TSSET (  CLK1, NRZB, ECLK1 );        
    TSSET (  CLK2, _RZO, ECLK1 );   
 
    // SCAN 0~31
    TSSET (  SCAN0,NRZB, ECLK1 );   
    TSSET (  SCAN1,NRZB, ECLK1 );   
    TSSET (  SCAN2,NRZB, ECLK1 );   
    
    // IO 0~31
    TSSET (  IO0, DNRZ, ECLK1 );   
    TSSET (  IO1, DNRZ, ECLK1 );   
    TSSET (  IO2, DNRZ, ECLK1 );   

    TSSET (  DRE, NRZB);
}

void load_level()
{

    VIH ( CLK , 1.0  ); 
    VIL ( CLK , 0.0  ); 
    VIH ( DR  , 1.0  ); 
    VIL ( DR  , 0.0  ); 
    VIH ( SCAN , 1.0  );
    VIL ( SCAN , 0.0  );
    VIH ( IO , 1.0  );  
    VIL ( IO , 0.0  );  
    VTH ( 0.6 );        

    DPS (VS1, 1.0, 0.0, 5.0, 20.0); 
    DPS (VS2, 1.2, 0.0, 5.0, 20.0);
    DPS (VS4, 1.4, 0.0, 5.0, 20.0);
    DPS (VS5, 1.6, 0.0, 5.0, 20.0);
        
    
}

void load_pds()
{
    PDS(  ADDR0,    X0  ,  Y0  ,  X0  ,  Y0  );     
    PDS(  ADDR1,    X1  ,  Y1  ,  X1  ,  Y1  );     
    PDS(  ADDR2,    X2  ,  Y2  ,  X2  ,  Y2  );     
}

void load_scram()
{
   ENABLE_SCRAM(false);
   SCRAM(x0  = ax0  );
   SCRAM(x1  = ax1	);   
}

void SETUP()
{
    load_bib();
    load_scan();

    load_ts();
    load_pds();
    load_feset();
    load_scram();
    load_level();   	
}


void SYNC(int outCMD)	
{
    if(outCMD == 1)
        printf("outCmd %d\n", outCMD);   
    NEXT_SYNC();
}




STEP(INIT)
{
 
    SEND_MPAT("sample_pattern");  
    SRON();   
    MEAS_MPAT("STEP_A");
    SROF();
}  


STEP(RUN)
{
    SEND_MPAT("sample_pattern"); 
    SRON();
    AGING("STEP_B", 1*60);
    SROF();
    
}

void CONTACT()
{        
    RUN_STEP(INIT);     
} 


void TEST() 
{   
    RUN_STEP(RUN);
}
