
#ifndef INA_I2C_H
#define INA_I2C_H


/****************** Include Files ********************/
#include "xil_types.h"
#include "xstatus.h"

#define INA_I2C_S00_AXI_SLV_REG0_OFFSET 0
#define INA_I2C_S00_AXI_SLV_REG1_OFFSET 4
#define INA_I2C_S00_AXI_SLV_REG2_OFFSET 8
#define INA_I2C_S00_AXI_SLV_REG3_OFFSET 12
#define INA_I2C_S00_AXI_SLV_REG4_OFFSET 16
#define INA_I2C_S00_AXI_SLV_REG5_OFFSET 20
#define INA_I2C_S00_AXI_SLV_REG6_OFFSET 24
#define INA_I2C_S00_AXI_SLV_REG7_OFFSET 28
#define INA_I2C_S00_AXI_SLV_REG8_OFFSET 32
#define INA_I2C_S00_AXI_SLV_REG9_OFFSET 36
#define INA_I2C_S00_AXI_SLV_REG10_OFFSET 40
#define INA_I2C_S00_AXI_SLV_REG11_OFFSET 44
#define INA_I2C_S00_AXI_SLV_REG12_OFFSET 48
#define INA_I2C_S00_AXI_SLV_REG13_OFFSET 52
#define INA_I2C_S00_AXI_SLV_REG14_OFFSET 56
#define INA_I2C_S00_AXI_SLV_REG15_OFFSET 60
#define INA_I2C_S00_AXI_SLV_REG16_OFFSET 64
#define INA_I2C_S00_AXI_SLV_REG17_OFFSET 68
#define INA_I2C_S00_AXI_SLV_REG18_OFFSET 72
#define INA_I2C_S00_AXI_SLV_REG19_OFFSET 76
#define INA_I2C_S00_AXI_SLV_REG20_OFFSET 80
#define INA_I2C_S00_AXI_SLV_REG21_OFFSET 84
#define INA_I2C_S00_AXI_SLV_REG22_OFFSET 88
#define INA_I2C_S00_AXI_SLV_REG23_OFFSET 92
#define INA_I2C_S00_AXI_SLV_REG24_OFFSET 96
#define INA_I2C_S00_AXI_SLV_REG25_OFFSET 100
#define INA_I2C_S00_AXI_SLV_REG26_OFFSET 104
#define INA_I2C_S00_AXI_SLV_REG27_OFFSET 108
#define INA_I2C_S00_AXI_SLV_REG28_OFFSET 112
#define INA_I2C_S00_AXI_SLV_REG29_OFFSET 116
#define INA_I2C_S00_AXI_SLV_REG30_OFFSET 120
#define INA_I2C_S00_AXI_SLV_REG31_OFFSET 124
#define INA_I2C_S00_AXI_SLV_REG32_OFFSET 128
#define INA_I2C_S00_AXI_SLV_REG33_OFFSET 132
#define INA_I2C_S00_AXI_SLV_REG34_OFFSET 136
#define INA_I2C_S00_AXI_SLV_REG35_OFFSET 140
#define INA_I2C_S00_AXI_SLV_REG36_OFFSET 144
#define INA_I2C_S00_AXI_SLV_REG37_OFFSET 148
#define INA_I2C_S00_AXI_SLV_REG38_OFFSET 152
#define INA_I2C_S00_AXI_SLV_REG39_OFFSET 156
#define INA_I2C_S00_AXI_SLV_REG40_OFFSET 160
#define INA_I2C_S00_AXI_SLV_REG41_OFFSET 164
#define INA_I2C_S00_AXI_SLV_REG42_OFFSET 168
#define INA_I2C_S00_AXI_SLV_REG43_OFFSET 172
#define INA_I2C_S00_AXI_SLV_REG44_OFFSET 176
#define INA_I2C_S00_AXI_SLV_REG45_OFFSET 180
#define INA_I2C_S00_AXI_SLV_REG46_OFFSET 184
#define INA_I2C_S00_AXI_SLV_REG47_OFFSET 188
#define INA_I2C_S00_AXI_SLV_REG48_OFFSET 192
#define INA_I2C_S00_AXI_SLV_REG49_OFFSET 196
#define INA_I2C_S00_AXI_SLV_REG50_OFFSET 200
#define INA_I2C_S00_AXI_SLV_REG51_OFFSET 204
#define INA_I2C_S00_AXI_SLV_REG52_OFFSET 208
#define INA_I2C_S00_AXI_SLV_REG53_OFFSET 212
#define INA_I2C_S00_AXI_SLV_REG54_OFFSET 216
#define INA_I2C_S00_AXI_SLV_REG55_OFFSET 220
#define INA_I2C_S00_AXI_SLV_REG56_OFFSET 224
#define INA_I2C_S00_AXI_SLV_REG57_OFFSET 228
#define INA_I2C_S00_AXI_SLV_REG58_OFFSET 232
#define INA_I2C_S00_AXI_SLV_REG59_OFFSET 236
#define INA_I2C_S00_AXI_SLV_REG60_OFFSET 240
#define INA_I2C_S00_AXI_SLV_REG61_OFFSET 244
#define INA_I2C_S00_AXI_SLV_REG62_OFFSET 248
#define INA_I2C_S00_AXI_SLV_REG63_OFFSET 252


/**************************** Type Definitions *****************************/
/**
 *
 * Write a value to a INA_I2C register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the INA_I2Cdevice.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void INA_I2C_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data)
 *
 */
#define INA_I2C_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/**
 *
 * Read a value from a INA_I2C register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the INA_I2C device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	u32 INA_I2C_mReadReg(u32 BaseAddress, unsigned RegOffset)
 *
 */
#define INA_I2C_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))

/************************** Function Prototypes ****************************/
/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the INA_I2C instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus INA_I2C_Reg_SelfTest(void * baseaddr_p);

#endif // INA_I2C_H
