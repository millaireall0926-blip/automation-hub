#ifndef __DEFINE__
#define __DEFINE__

typedef unsigned int uint;
typedef unsigned long long int uint64;
typedef unsigned short int	word;
typedef unsigned char byte;

#define ROM_MAX_SIZE  0x8000
#define CAL_ADC0_ADDR  0x100
#define CAL_ADC1_ADDR  0x200
#define CAL_PE_VIL_ADDR   0x1000
#define CAL_PE_VIH_ADDR   0x2000
#define CAL_PE_VT_ADDR   0x3000
#define CAL_DPS_ADDR  0x5000
#define CAL_VS3_ADDR  0x5500

#define NO_SLEEP        0
#define SLEEP_1US       1
#define SLEEP_10US      10
#define SLEEP_100US     100

#define NO_RUN      false    


#define SYNC_USER_LAST    0xEF

#define SYNC_CYCLE_PROG_START   0xF0
#define SYNC_CYCLE_PROG_GATHER   0xF1
#define SYNC_CYCLE_ERASE_START   0xF2
#define SYNC_CYCLE_ERASE_GATHER   0xF3


#define SYNC_DATA   0xFE
#define SYNC_SCAN   0xFF
#define SYNC_NONE   0x100
#define SYNC_END    0x110

#define gIO		g_clIO
#define gAPI	g_clAPI
#define gLIB	g_clLib
#define gNET	g_clNet
#define gERR	g_clErr
#define gBIB	g_clBib
#define gTG		g_clTgc
#define gPG		g_clPgc
#define gPDS	g_clPds
#define gSCR	g_clScr
#define gRES	g_clRes
#define gFM		g_clFm
#define gOPER	g_clOperation
#define gROM	g_clRom
#define gDPS	g_clDPS
#define gVEC	g_clVec
#define gNAND	g_clNand
#define gCMD	g_clCmd



#define PG_INSTRUCTION		(0)
#define PG_SYSTEM			(1<<9)
#define PG_PAT				(1<<18)
#define PG_PDS				(12<<16)
#define PG_FORMAT			(13<<16)
#define PG_SCR				(14<<16)
#define PG_SCAN				(59<<14)
#define PG_FMH				(4<<17)
#define PG_FML				(5<<17)
#define PG_TG_REG			((1<<20)|(1<<18)) 
#define PG_TG_MEM			((1<<20)|(1<<18)|(1<<16))
#define PG_VMH				(1<<20)
#define PG_VML				((1<<20)|(1<<17))


#define XS(h, id) gLIB.xml_att_find(h, id)
#define XI(h, id) atoi(gLIB.xml_att_find(h, id))
#define XB(h, id) (atoi(gLIB.xml_att_find(h, id)) == 1)
#define XSI(h, id, def) gLIB.csi(gLib->xml_att_find(h, id), def)
#define XF(h, id) atof(gLIB.xml_att_find(h, id))

#define XAS(h, id, v) gLIB.xml_att_sets(h, id, v)
#define XAI(h, id, v) gLIB.xml_att_setn(h, id, v)
#define XTEXT(h, v) gLIB.xml_set_text(h, v)


typedef struct
{
  float gain;
  float offset;
  float offset2;
} stCalFactor;

typedef struct
{
    unsigned short chBit;
    double delay;
}stPowerSeq;


#endif


