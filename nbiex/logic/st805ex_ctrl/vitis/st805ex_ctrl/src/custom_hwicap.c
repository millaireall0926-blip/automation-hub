
#include "define.h"


static XHwIcap  HwIcap;	/* The instance of the HWICAP device */

u8 hwicap_init(void) {
	int Status;

	XHwIcap_Config *ConfigPtr;
	u32 ConfigRegData;

	/*
	 * Initialize the HwIcap driver.
	 */
#ifndef SDT
	ConfigPtr = XHwIcap_LookupConfig(HWICAP_DEVICE_ID);
#else
	ConfigPtr = XHwIcap_LookupConfig(HWICAP_BASEADDR);
#endif
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}

	Status = XHwIcap_CfgInitialize(&HwIcap, ConfigPtr, ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	
	/*
	 * Read the ID Code register inside the FPGA.
	 */
	Status = XHwIcap_GetConfigReg(&HwIcap, XHI_IDCODE, &ConfigRegData);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

static u32 IprogCmd[] = {
	XHI_DUMMY_PACKET,	/* Dummy Word */
	XHI_DUMMY_PACKET,
	XHI_SYNC_PACKET,	/* Sync Word*/
	XHI_NOOP_PACKET,	/* Type 1 NO OP */
	( XHwIcap_Type1Write( XHI_WBSTAR ) | 1 ),	/* Write WBSTAR w/ 1ea address data */
	0x00000000,			/* Addr in Flash for bitstream */
	( XHwIcap_Type1Write( XHI_CMD) | 1 ),		/* Write CMD w/ 1ea IPROG cmd */
	XHI_CMD_IPROG,		/* Write IPROG */
	XHI_NOOP_PACKET,	/* Type 1 NO OP */
};

u32 reboot_fpga(void) {
	u32 Index;
	u32 Retries;

	for (Index = 0; Index < sizeof(IprogCmd) / 4; Index++) {
		XHwIcap_WriteReg(HwIcap.HwIcapConfig.BaseAddress, XHI_WF_OFFSET, IprogCmd[Index]);
	}
	XHwIcap_WriteReg(HwIcap.HwIcapConfig.BaseAddress, XHI_CR_OFFSET, XHI_CR_WRITE_MASK);

	Retries = 0;
	while ((XHwIcap_ReadReg(HwIcap.HwIcapConfig.BaseAddress, XHI_SR_OFFSET) & XHI_SR_DONE_MASK) != XHI_SR_DONE_MASK) {
		Retries++;
		if (Retries > XHI_MAX_RETRIES) {
			printf("\r\n HwIcapReconfig : XHI_SR_DONE_MASK - retries failure. \r\n\r\n");

			return XST_FAILURE;
		}
	}

	Retries = 0;
	while ((XHwIcap_ReadReg(HwIcap.HwIcapConfig.BaseAddress, XHI_CR_OFFSET)) & XHI_CR_WRITE_MASK) {
		Retries++;
		if (Retries > XHI_MAX_RETRIES) {
			printf("\r\n HwIcapReconfig : XHI_CR_WRITE_MASK - retries failure. \r\n\r\n");

			return XST_FAILURE;
		}
	}

	return XST_SUCCESS;
}
