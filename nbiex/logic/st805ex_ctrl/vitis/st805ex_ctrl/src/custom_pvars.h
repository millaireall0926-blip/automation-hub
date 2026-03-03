
#ifndef _CUSTOM_PVARS_H_
#define _CUSTOM_PVARS_H_

#include "define.h"


/* defines */
#define STS_BASE			(XPAR_AXI_BRAM_CTRL_ALL_STS_BASEADDR + 0x00)
#define BRD_INFO_BASE	(XPAR_BOARD_INFO_0_BASEADDR + 0x00)
#define CFG_C2X_BASE	(XPAR_CFGX8_C2X_0_BASEADDR + 0x00)
#define LB_C2P_BASE		(XPAR_LBX8_C2P_0_BASEADDR + 0x00)
#define ADC_BASE			(XPAR_ADC_AD4855_0_BASEADDR + 0x00)
#define DAC_BASE			(XPAR_DAC_AD5677_0_BASEADDR + 0x00)
#define DPS_BASE			(XPAR_DPS_I2C_0_BASEADDR + 0x00)
#define INA_BASE			(XPAR_INA_I2C_0_BASEADDR + 0x00)
#define EEP_BASE			(XPAR_EEP_I2C_0_BASEADDR + 0x00)
#define PROM_BASE			(XPAR_CFG_PROM_0_BASEADDR + 0x00)

#define CFG_BUF_BASE	(XPAR_CFGX8_C2X_BUF_BASEADDR + 0x00)
#define LB_BUF_BASE		(XPAR_LBX8_C2P_BUF_BASEADDR + 0x00)

#define HWICAP_BASE		(XPAR_AXI_HWICAP_0_BASEADDR + 0x00)
#define TMRCTR_BASE		(XPAR_AXI_TIMER_0_BASEADDR + 0x00)


/* struct */


/* extern vars */
/* ALL-STS */
extern  volatile  u8 *P_ALL_STATUS      ;

/* PJT-INFO */
extern  volatile  u8 *P_PJT_MINOR				;
extern  volatile  u8 *P_PJT_MAJOR				;
extern  volatile  u8 *P_PJT_DATE 				;
extern  volatile  u8 *P_PJT_MONTH				;
extern  volatile  u8 *P_PJT_YEAR				;

/* BOARD */
extern  volatile  u8 *P_BOARD_TPCB_REV  ;
extern  volatile  u8 *P_BOARD_PG_REV    ;
extern  volatile  u8 *P_BOARD_SLOT_ID   ;
extern  volatile  u8 *P_BOARD_LED_ACT   ;
extern  volatile  u8 *P_BOARD_LED_ON    ;
extern  volatile  u8 *P_BOARD_LED_ERR   ;
extern  volatile  u8 *P_BOARD_BIB_nCHK  ;
extern  volatile  u8 *P_BOARD_SENSE_SEL ;
extern  volatile  u8 *P_BOARD_RLY_DGS   ;
extern  volatile  u8 *P_BOARD_BID_DIR   ;
extern  volatile  u8 *P_BOARD_BID_OUT   ;
extern  volatile  u8 *P_BOARD_BID_IN    ;

/* CFGx8-C2X */

/* LBx8-C2P */
extern  volatile u32 *P_C2P_ADDR        ;
extern  volatile u16 *P_C2P_BL          ;
extern  volatile  u8 *P_C2P_SEL         ;
extern  volatile  u8 *P_C2P_CMD         ;
extern  volatile  u8 *P_C2P_RL          ;
extern  volatile  u8 *P_C2P_STS         ;
extern  volatile  u8 *P_C2P_BUF         ;

/* ADC-AD4855 */  
extern  volatile u32 *P_ADC_CMD         ;
extern  volatile  u8 *P_ADC_OS          ;
extern  volatile  u8 *P_ADC_PD          ;
extern  volatile u32 *P_ADC_DLY         ;
extern  volatile u32 *P_ADC_STS         ;
extern  volatile u16 *P_ADC_BUF         ;
extern  volatile u16 *P_ADC_AUTO        ;

/* DAC-AD5677 */  
extern  volatile u32 *P_DAC_CMD         ;
extern  volatile  u8 *P_DAC_LDAC        ;
extern  volatile u32 *P_DAC_STS         ;

/* DPS-I2C */ 
extern  volatile u32 *P_DPS_CMD         ;
extern  volatile u16 *P_DPS_RUN         ;
extern  volatile  u8 *P_DPS_HV_EN       ;
extern  volatile  u8 *P_DPS_CAL_EN      ;
extern  volatile  u8 *P_DPS_HV_CAL_EN   ;
extern  volatile  u8 *P_DPS_VREF_EN     ;
extern  volatile  u8 *P_DPS_ALERT       ;
extern  volatile  u8 *P_DPS_FAULT       ;
extern  volatile u32 *P_DPS_STS         ;
extern  volatile  u8 *P_DPS_BUF         ;

/* INA-I2C */ 
extern  volatile u32 *P_INA_CMD         ;
extern  volatile u32 *P_INA_STS         ;
extern  volatile u16 *P_INA_CUR         ;
extern  volatile u16 *P_INA_VOLT        ;

/* EEP-I2C */ 
extern  volatile u32 *P_EEP_CMD         ;
extern  volatile u32 *P_EEP_STS         ;
extern  volatile  u8 *P_EEP_BUF         ;

/* CFG-PROM */  
extern  volatile u32 *P_PROM_ADDR       ;
extern  volatile  u8 *P_PROM_BYTE       ;
extern  volatile  u8 *P_PROM_CMD        ;
extern  volatile  u8 *P_PROM_REBOOT     ;
extern  volatile  u8 *P_PROM_STS        ;
extern  volatile  u8 *P_PROM_BUF        ;


/* functions */


#endif
