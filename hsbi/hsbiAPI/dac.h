#pragma once

#ifndef _HSBI_DAC_BASE_LIB_
#define _HSBI_DAC_BASE_LIB_


class dac
{
public:
    dac();
    ~dac();

    void dac_reset();
    void dac_init();
	//void dac_clamp(bool bOn);

	void dac_vref_en(bool en);
	void dac_vtt_bias_en(bool en);
	void dac_pps_12_en(bool en);

	
};
#endif