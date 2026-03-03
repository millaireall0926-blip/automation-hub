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
#define gPWR    g_clPower



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


/// PE Address 
#define PE_C_CHIP_ID         0x00
#define PE_C_EXT_INT_ALARM   0x01
#define PE_C_PDO_RESET       0x02
#define PE_C_SP_LATCH_EN     0x03
#define PE_C_DIAG            0x04
#define PE_C_IREF_CAL        0x05

#define PE_CH_FLAG_STATUS    0x00
#define PE_CH_POWER_ON_CTRL  0x01
#define PE_CH_DATA_EN_CONF   0x02
#define PE_CH_COMP_CONF      0x03
#define PE_CH_ROSC_CTRL      0x04
#define PE_CH_DRV_SLEW       0x05
#define PE_CH_DRV_CTRL       0x06
#define PE_CH_DATA_EN_PULSE  0x07
#define PE_CH_COMP_PULSE     0x08
#define PE_CH_DATA_FINE_DLY  0x09
#define PE_CH_EN_FINE_DLY    0x0A
#define PE_CH_COMP_FINE_DLY  0x0B
#define PE_CH_CON_DIAG       0x0C
#define PE_CH_OT_CTRL        0x0D
#define PE_CH_LOW_FORCE_LVL  0x0E
#define PE_CH_HIGH_FORCE_LVL 0x0F

#define PE_CH_COMP_TH        0x10
#define PE_CH_SEL_DIAG_0     0x11
#define PE_CH_SEL_DIAG_1     0x12



#define XS(h, id) gLIB.xml_att_find(h, id)
#define XI(h, id) atoi(gLIB.xml_att_find(h, id))
#define XB(h, id) (atoi(gLIB.xml_att_find(h, id)) == 1)
#define XSI(h, id, def) gLIB.csi(gLib->xml_att_find(h, id), def)
#define XF(h, id) atof(gLIB.xml_att_find(h, id))

#define XAS(h, id, v) gLIB.xml_att_sets(h, id, v)
#define XAI(h, id, v) gLIB.xml_att_setn(h, id, v)
#define XTEXT(h, v) gLIB.xml_set_text(h, v)






#endif


