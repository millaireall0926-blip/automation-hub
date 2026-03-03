
#include "define.h"


/* ALL-STS */
volatile  u8 *P_ALL_STATUS      = (volatile  u8 *)(STS_BASE + 0x000);

/* BOARD-INFO */
volatile  u8 *P_BOARD_TPCB_REV  = (volatile  u8 *)(BRD_INFO_BASE + 0x020);
volatile  u8 *P_BOARD_PG_REV    = (volatile  u8 *)(BRD_INFO_BASE + 0x022);
volatile  u8 *P_BOARD_SLOT_ID   = (volatile  u8 *)(BRD_INFO_BASE + 0x024);
volatile  u8 *P_BOARD_LED_ACT   = (volatile  u8 *)(BRD_INFO_BASE + 0x02C);
volatile  u8 *P_BOARD_LED_ON    = (volatile  u8 *)(BRD_INFO_BASE + 0x02D);
volatile  u8 *P_BOARD_LED_ERR   = (volatile  u8 *)(BRD_INFO_BASE + 0x02E);
volatile  u8 *P_BOARD_BIB_nCHK  = (volatile  u8 *)(BRD_INFO_BASE + 0x030);
volatile  u8 *P_BOARD_SENSE_SEL = (volatile  u8 *)(BRD_INFO_BASE + 0x031);
volatile  u8 *P_BOARD_RLY_DGS   = (volatile  u8 *)(BRD_INFO_BASE + 0x032);
volatile  u8 *P_BOARD_BID_DIR   = (volatile  u8 *)(BRD_INFO_BASE + 0x034);
volatile  u8 *P_BOARD_BID_OUT   = (volatile  u8 *)(BRD_INFO_BASE + 0x038);
volatile  u8 *P_BOARD_BID_IN    = (volatile  u8 *)(BRD_INFO_BASE + 0x03C);

/* CFGx8-C2X */

/* LBx8-C2P */
volatile u32 *P_C2P_ADDR        = (volatile u32 *)(LB_C2P_BASE + 0x020);
volatile u16 *P_C2P_BL          = (volatile u16 *)(LB_C2P_BASE + 0x024);
volatile  u8 *P_C2P_SEL         = (volatile  u8 *)(LB_C2P_BASE + 0x026);
volatile  u8 *P_C2P_CMD         = (volatile  u8 *)(LB_C2P_BASE + 0x027);
volatile  u8 *P_C2P_RL          = (volatile  u8 *)(LB_C2P_BASE + 0x02C);
volatile  u8 *P_C2P_STS         = (volatile  u8 *)(LB_C2P_BASE + 0x030);
volatile  u8 *P_C2P_BUF         = (volatile  u8 *)(LB_C2P_BASE + 0x040);

/* ADC-AD4855 */  
volatile u32 *P_ADC_CMD         = (volatile u32 *)(ADC_BASE + 0x020);
volatile  u8 *P_ADC_OS          = (volatile  u8 *)(ADC_BASE + 0x024);
volatile  u8 *P_ADC_PD          = (volatile  u8 *)(ADC_BASE + 0x025);
volatile u32 *P_ADC_DLY         = (volatile u32 *)(ADC_BASE + 0x028);
volatile u32 *P_ADC_STS         = (volatile u32 *)(ADC_BASE + 0x030);
volatile u16 *P_ADC_BUF         = (volatile u16 *)(ADC_BASE + 0x040);
volatile u16 *P_ADC_AUTO        = (volatile u16 *)(ADC_BASE + 0x100);

/* DAC-AD5677 */  
volatile u32 *P_DAC_CMD         = (volatile u32 *)(DAC_BASE + 0x020);
volatile  u8 *P_DAC_LDAC        = (volatile  u8 *)(DAC_BASE + 0x024);
volatile u32 *P_DAC_STS         = (volatile u32 *)(DAC_BASE + 0x030);

/* DPS-I2C */ 
volatile u32 *P_DPS_CMD         = (volatile u32 *)(DPS_BASE + 0x020);
volatile u16 *P_DPS_RUN         = (volatile u16 *)(DPS_BASE + 0x028);
volatile  u8 *P_DPS_HV_EN       = (volatile  u8 *)(DPS_BASE + 0x02B);
volatile  u8 *P_DPS_CAL_EN      = (volatile  u8 *)(DPS_BASE + 0x02C);
volatile  u8 *P_DPS_HV_CAL_EN   = (volatile  u8 *)(DPS_BASE + 0x02D);
volatile  u8 *P_DPS_VREF_EN     = (volatile  u8 *)(DPS_BASE + 0x02F);
volatile  u8 *P_DPS_ALERT       = (volatile  u8 *)(DPS_BASE + 0x038);
volatile  u8 *P_DPS_FAULT       = (volatile  u8 *)(DPS_BASE + 0x03A);
volatile u32 *P_DPS_STS         = (volatile u32 *)(DPS_BASE + 0x030);
volatile  u8 *P_DPS_BUF         = (volatile  u8 *)(DPS_BASE + 0x040);

/* INA-I2C */ 
volatile u32 *P_INA_CMD         = (volatile u32 *)(INA_BASE + 0x020);
volatile u32 *P_INA_STS         = (volatile u32 *)(INA_BASE + 0x030);
volatile u16 *P_INA_CUR         = (volatile u16 *)(INA_BASE + 0x040);
volatile u16 *P_INA_VOLT        = (volatile u16 *)(INA_BASE + 0x042);

/* EEP-I2C */ 
volatile u32 *P_EEP_CMD         = (volatile u32 *)(EEP_BASE + 0x020);
volatile u32 *P_EEP_STS         = (volatile u32 *)(EEP_BASE + 0x030);
volatile  u8 *P_EEP_BUF         = (volatile  u8 *)(EEP_BASE + 0x040);

/* CFG-PROM */  
volatile u32 *P_PROM_ADDR       = (volatile u32 *)(PROM_BASE + 0x020);
volatile  u8 *P_PROM_BYTE       = (volatile  u8 *)(PROM_BASE + 0x024);
volatile  u8 *P_PROM_CMD        = (volatile  u8 *)(PROM_BASE + 0x025);
volatile  u8 *P_PROM_REBOOT     = (volatile  u8 *)(PROM_BASE + 0x028);
volatile  u8 *P_PROM_STS        = (volatile  u8 *)(PROM_BASE + 0x030);
volatile  u8 *P_PROM_BUF        = (volatile  u8 *)(PROM_BASE + 0x040);
