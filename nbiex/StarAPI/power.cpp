
#include "power.h"	
#include "ioc.h"
#include "errc.h"
#include "../starlib/starlib_dif.h"

#define CAL_FILE "/star/diag/cal/cal.xml"

extern ioc  gIO;
extern starlib gLIB;
extern errctrl gERR;

powerc::powerc()
{
	initCalData();
	loadCalFile();
}


void powerc::readAll()
{
	int len = sizeof(allst);
	gIO.read_all_status((byte*)&m_stAllStat, len);
}

short powerc::readAdc(int mux, int index, bool isDirect)
{
	short val = 0;
	if (isDirect)
	{
		int adcOffset = 39;
		int	ofs = adcOffset + (((mux * 16) + mux) * 2);		
		gIO.read_one_status((byte*)&val, adcOffset, 2);		
	}
	else
	{
		int adc_index = mux;
		int adc_mux = index;
		if (adc_index < 0 || adc_index >= 16 || adc_mux < 0 || adc_mux >= 8)
		{
			printf("%s : error invalid adc index or mux [%d][%d]!!\n", __FUNCTION__, adc_index, adc_mux);
			return 0;
		}
		val =  m_stAllStat.ADC_MUX[adc_index][adc_mux];
	}
	// something Calculation if needed

	return val;
}

byte powerc::readDpsRun(bool isDirect)
{
	byte val = 0;
	if (isDirect)
	{
		int dps_run_ofs = 295;
		word wVal = 0;
		gIO.read_one_status((byte*)&wVal, dps_run_ofs, 2);
		val = (byte)(wVal & 0xFF);
	}
	else
	{
		val = (byte)(m_stAllStat.DPS_RUN & 0xFF);
	}
	return val;
}

byte powerc::readHvRun(bool isDirect)
{
	byte val = 0;
	if (isDirect)
	{
		int hv_dps_en_ofs = 297;
		gIO.read_one_status((byte*)&val, hv_dps_en_ofs, 1);
	}
	else
	{
		val = m_stAllStat.HV_DPS_EN;
	}
	return val;
}

byte powerc::readVrefRun(bool isDirect)
{
	byte val = 0;
	if (isDirect)
	{
		int vref_en_ofs = 298;
		gIO.read_one_status((byte*)&val, vref_en_ofs, 1);
	}
	else
	{
		val = m_stAllStat.VREF_EN;
	}
	return val;
}

byte powerc::readDpsAlert(bool isDirect)
{
	byte val = 0;
	if (isDirect)
	{
		int dps_alert_ofs = 299;
		gIO.read_one_status((byte*)&val, dps_alert_ofs, 1);
	}
	else
	{
		val = m_stAllStat.DPS_ALERT;
	}
	return val;
}

byte powerc::readDpsFault(bool isDirect)
{
	byte val = 0;
	if (isDirect)
	{
		int dps_fault_ofs = 300;
		gIO.read_one_status((byte*)&val, dps_fault_ofs, 1);
	}
	else
	{
		val = m_stAllStat.DPS_FAULT;
	}
	return val;
}

word powerc::readDpsStatus(int ch, bool isDirect)
{
	word val = 0;
	if (isDirect)
	{
		int dps_status_ofs = 301;
		if (ch < 0 || ch >= 16)
		{
			printf("%s : error invalid dps status channel [%d]!!\n", __FUNCTION__, ch);
			return 0;
		}
		gIO.read_one_status((byte*)&val, dps_status_ofs + (ch * 2), 2);
	}
	else
	{
		if (ch < 0 || ch >= 16)
		{
			printf("%s : error invalid dps status channel [%d]!!\n", __FUNCTION__, ch);
			return 0;
		}
		val = m_stAllStat.DPS_STATUS[ch];
	}
	return val;
}

word powerc::readDpsVolt(int ch, bool isDirect)
{
	word val = 0;
	if (isDirect)
	{
		int dps_vout_ofs = 333;
		if (ch < 0 || ch >= 16)
		{
			printf("%s : error invalid dps volt channel [%d]!!\n", __FUNCTION__, ch);
			return 0;
		}
		gIO.read_one_status((byte*)&val, dps_vout_ofs + (ch * 2), 2);
	}
	else
	{
		if (ch < 0 || ch >= 16)
		{
			printf("%s : error invalid dps volt channel [%d]!!\n", __FUNCTION__, ch);
			return 0;
		}
		val = m_stAllStat.DPS_VOUT[ch];
	}
	return val;
}

word powerc::readDpsCurrent(int ch, bool isDirect)
{
	word val = 0;
	if (isDirect)
	{
		int dps_iout_ofs = 365;
		if (ch < 0 || ch >= 16)
		{
			printf("%s : error invalid dps current channel [%d]!!\n", __FUNCTION__, ch);
			return 0;
		}
		gIO.read_one_status((byte*)&val, dps_iout_ofs + (ch * 2), 2);
	}
	else
	{
		if (ch < 0 || ch >= 16)
		{
			printf("%s : error invalid dps current channel [%d]!!\n", __FUNCTION__, ch);
			return 0;
		}
		val = m_stAllStat.DPS_IOUT[ch];
	}
	return val;
}

short powerc::readInaVolt(int ch, bool isDirect)
{
	word val = 0;
	if (isDirect)
	{
		int ina_volt_ofs = 397;
		if (ch < 0 || ch >= 20)
		{
			printf("%s : error invalid ina volt channel [%d]!!\n", __FUNCTION__, ch);
			return 0;
		}
		gIO.read_one_status((byte*)&val, ina_volt_ofs + (ch * 2), 2);
	}
	else
	{
		if (ch < 0 || ch >= 20)
		{
			printf("%s : error invalid ina volt channel [%d]!!\n", __FUNCTION__, ch);
			return 0;
		}
		val = m_stAllStat.INA_VOLT[ch];
	}

	return val;
}

short powerc::readInaCurrent(int ch, bool isDirect)
{
	word val = 0;
	if (isDirect)
	{
		int ina_cur_ofs = 437;
		if (ch < 0 || ch >= 20)
		{
			printf("%s : error invalid ina current channel [%d]!!\n", __FUNCTION__, ch);
			return 0;
		}
		gIO.read_one_status((byte*)&val, ina_cur_ofs + (ch * 2), 2);
	}
	else
	{
		if (ch < 0 || ch >= 20)
		{
			printf("%s : error invalid ina current channel [%d]!!\n", __FUNCTION__, ch);
			return 0;
		}
		val = m_stAllStat.INA_CUR[ch];
	}
	return val;
}

void powerc::setEnableHv(byte onoff, bool isOn)
{	
	gIO.dps_hv_enable(onoff, isOn);	
}

void powerc::setEnableVref(byte onoff, bool isOn)
{	
	gIO.dps_vref_enable(onoff, isOn);
}

void powerc::setEnableDac(int type, bool isOn)
{
	byte ret =  gIO.dac_enable(type, isOn);
	if (ret == RTN_ARGC_ERR)	
		gERR.addErr(ERR_POWER, "%s Invalid DAC Type [%d]!!", __FUNCTION__, type);
	
	//gIO.dac_ldac(onoff, isOn);
}


void powerc::setDac(int ch, double val)
{
	short wdata = 0;

	if (ch == VH_ALL)
	{
		for (int i = VH1; i <= VH16; i++)		
			setDac(i, val);
		return;
	}

	if (ch == VTH_ALL)
	{
		for (int i = VTH1; i <= VH8; i++)
			setDac(i, val);
		return;
	}

	if (ch == HV_ALL)
	{
		for (int i = HV1; i <= HV4; i++)
			setDac(i, val);
		return;
	}
	if (ch == VREF_ALL)
	{
		for (int i = VREF1; i <= VREF4; i++)
			setDac(i, val);
		return;
	}
	if (ch == DPS_ALL)
	{
		gERR.addErr(ERR_POWER, "%s Not Allow DPS Set", __FUNCTION__);
	}
	
	if (ch >= HV1 && ch <= HV4)
	{
		int calIndex = ch - HV1;
		double calc = (val * m_fCalHvF[calIndex][0]) + m_fCalHvF[calIndex][1];
		wdata = (short)(calc / 5.0 * 65536.0); // Convert to DAC value

		int shift = ch - HV1;
		byte onval = (1 << shift) & 0xf;
		// Store the set value for monitor
		m_PMonArr[MAX_DPS_CH + (ch - HV1)].sv = val;
	}
	else {
		int calIndex = ch;
		if (ch >= VREF1 && ch <= VREF4) 
			calIndex = ch - 4; //Subtraction HV 4 index
		double calc = (val * m_fCalPwrF[calIndex][0]) + m_fCalPwrF[calIndex][1];
		wdata = (short)(calc / 5.0 * 65536.0); // Convert to DAC value

		if (ch == VH1)m_VMonArr[0] = val;
		else if (ch == VTH1)m_VMonArr[1] = val;
		else if (ch == VREF1)m_VMonArr[2] = val;
	}	
	
	gIO.dac_set(ch, wdata);
	
}

double powerc::getAdc(int ch, bool isDirect)
{
	adcItem item = m_adcMap.arr[ch];
	short read = readAdc(item.mux, item.index, isDirect);
	double adcVal = read * 40.0 / 65536.0; // Convert to voltage 
	adcVal = (adcVal * m_fCalADCBase[item.index][0]) + m_fCalADCBase[item.index][1];
	// HV adc로 읽을땐 보정 필요없음으로 문서화 되어있음
	//double calc = 0;
	//if (ch >= HV1 && ch <= HV4)
	//{
	//	int calIndex = ch - HV1;
	//	double calc = (adcVal * m_fCalHvV[calIndex][0]) + m_fCalHvV[calIndex][1];
	
	return adcVal;
}

double powerc::getInaVolt(int ch, bool isDirect)
{
	int index = ch;
	if (ch >= VS16)
		index = ch - HV1 + 16;
	if (index > 19)return 0;
	short code = readInaVolt(index, isDirect);
	const double Voltage_LSB = 0.00125; // (VoltageLSB = 1.25mV )
	double value = code * Voltage_LSB;
	return value;
}

double powerc::getInaCur(int ch, bool isDirect)
{
	int index = ch;
	if (ch >= VS16 )
		index = ch - HV1 + 16;
	if (index > 19)return 0;
	short code = readInaCurrent(index, isDirect);
	const double Current_LSB = 0.00512; // (CurrentLSB * rShunt )
	double value = code* Current_LSB;
	return value;
}


void powerc::initCalData()
{
	for (int i = 0; i < 8; i++)
	{
		m_fCalADCBase[i][0] = 1;
		m_fCalADCBase[i][1] = 0;
	}
	for (int i = 0; i < 4; i++)
	{
		m_fCalHvF[i][0] = 1;
		m_fCalHvF[i][1] = 0;
		m_fCalHvV[i][0] = 1;
		m_fCalHvV[i][1] = 0;
		m_fCalHvA[i][0] = 1;
		m_fCalHvA[i][1] = 0;
	}
	for (int i = 0; i < 28; i++)
	{
		m_fCalPwrF[i][0] = 1;
		m_fCalPwrF[i][1] = 0;		
	}

	for (int i = 0; i < 16; i++)
	{
		m_fCalDpsF[i][0] = 1;
		m_fCalDpsF[i][1] = 0;
		m_fCalDpsV[i][0] = 1;
		m_fCalDpsV[i][1] = 0;
		m_fCalDpsA[i][0] = 1;
		m_fCalDpsA[i][1] = 0;		
	}

}

void powerc::loadCalFile()
{
	HND doc;
	gLIB.xml_load(CAL_FILE, &doc);
	if (doc <= 0)
	{
		printf("%s : cal file not found!! \n", __FUNCTION__);
		return;
	}
	cstring str = "/cal-data/items/item.name=CAL_ADC_BASE";
	HND hItem = gLIB.xml_path_handle(doc, str);
	if (hItem <= 0)
	{
		// Gain
		HND hGain = gLIB.xml_path_handle(hItem, "gain");
		cstring sGain = gLIB.xml_text(hGain);
		vector<cstring> aGain;
		sGain.Split(aGain, ",");
		for (int i = 0; i < 8; i++)
			m_fCalADCBase[i][0] = atof(aGain[i].c_str());
				// Offset
		HND hOfs = gLIB.xml_path_handle(hItem, "ofs");
		cstring sOfs = gLIB.xml_text(hOfs);
		vector<cstring> aOfs;
		sOfs.Split(aOfs, ",");
		for (int i = 0; i < 8; i++)
			m_fCalADCBase[i][1] = atof(aOfs[i].c_str());				
	}

	str = "/cal-data/items/item.name=CAL_HV_F";
	hItem = gLIB.xml_path_handle(doc, str);
	if (hItem <= 0)
	{	// Gain
		HND hGain = gLIB.xml_path_handle(hItem, "gain");
		cstring sGain = gLIB.xml_text(hGain);
		vector<cstring> aGain;
		sGain.Split(aGain, ",");
		for (int i = 0; i < 4; i++)
			m_fCalHvF[i][0] = atof(aGain[i].c_str());
		// Offset
		HND hOfs = gLIB.xml_path_handle(hItem, "ofs");
		cstring sOfs = gLIB.xml_text(hOfs);
		vector<cstring> aOfs;
		sOfs.Split(aOfs, ",");
		for (int i = 0; i < 4; i++)
			m_fCalHvF[i][1] = atof(aOfs[i].c_str());
	}

	str = "/cal-data/items/item.name=CAL_HV_V";
	hItem = gLIB.xml_path_handle(doc, str);
	if (hItem <= 0)
	{	// Gain
		HND hGain = gLIB.xml_path_handle(hItem, "gain");
		cstring sGain = gLIB.xml_text(hGain);
		vector<cstring> aGain;
		sGain.Split(aGain, ",");
		for (int i = 0; i < 4; i++)
			m_fCalHvV[i][0] = atof(aGain[i].c_str());		
		// Offset
		HND hOfs = gLIB.xml_path_handle(hItem, "ofs");
		cstring sOfs = gLIB.xml_text(hOfs);
		vector<cstring> aOfs;
		sOfs.Split(aOfs, ",");
		for (int i = 0; i < 4; i++)
			m_fCalHvV[i][1] = atof(aOfs[i].c_str());		
	}

	str = "/cal-data/items/item.name=CAL_HV_A";
	hItem = gLIB.xml_path_handle(doc, str);
	if (hItem <= 0)
	{	// Gain
		HND hGain = gLIB.xml_path_handle(hItem, "gain");
		cstring sGain = gLIB.xml_text(hGain);
		vector<cstring> aGain;
		sGain.Split(aGain, ",");
		for (int i = 0; i < 4; i++)
			m_fCalHvA[i][0] = atof(aGain[i].c_str());	
		// Offset
		HND hOfs = gLIB.xml_path_handle(hItem, "ofs");
		cstring sOfs = gLIB.xml_text(hOfs);
		vector<cstring> aOfs;
		sOfs.Split(aOfs, ",");
		for (int i = 0; i < 4; i++)
			m_fCalHvA[i][1] = atof(aOfs[i].c_str());		
	}

	str = "/cal-data/items/item.name=CAL_DPS_F";
	hItem = gLIB.xml_path_handle(doc, str);
	if (hItem <= 0)
	{	// Gain
		HND hGain = gLIB.xml_path_handle(hItem, "gain");
		cstring sGain = gLIB.xml_text(hGain);
		vector<cstring> aGain;
		sGain.Split(aGain, ",");
		for (int i = 0; i < 16; i++)
			m_fCalDpsF[i][0] = atof(aGain[i].c_str());
		// Offset
		HND hOfs = gLIB.xml_path_handle(hItem, "ofs");
		cstring sOfs = gLIB.xml_text(hOfs);
		vector<cstring> aOfs;
		sOfs.Split(aOfs, ",");
		for (int i = 0; i < 16; i++)
			m_fCalDpsF[i][1] = atof(aOfs[i].c_str());
	}

	str = "/cal-data/items/item.name=CAL_DPS_V";
	hItem = gLIB.xml_path_handle(doc, str);
	if (hItem <= 0)
	{	// Gain
		HND hGain = gLIB.xml_path_handle(hItem, "gain");
		cstring sGain = gLIB.xml_text(hGain);
		vector<cstring> aGain;
		sGain.Split(aGain, ",");
		for (int i = 0; i < 16; i++)
			m_fCalDpsV[i][0] = atof(aGain[i].c_str());
		// Offset
		HND hOfs = gLIB.xml_path_handle(hItem, "ofs");
		cstring sOfs = gLIB.xml_text(hOfs);
		vector<cstring> aOfs;
		sOfs.Split(aOfs, ",");
		for (int i = 0; i < 16; i++)
			m_fCalDpsV[i][1] = atof(aOfs[i].c_str());
	}

	str = "/cal-data/items/item.name=CAL_DPS_A";
	hItem = gLIB.xml_path_handle(doc, str);
	if (hItem <= 0)
	{	// Gain
		HND hGain = gLIB.xml_path_handle(hItem, "gain");
		cstring sGain = gLIB.xml_text(hGain);
		vector<cstring> aGain;
		sGain.Split(aGain, ",");
		for (int i = 0; i < 16; i++)
			m_fCalDpsA[i][0] = atof(aGain[i].c_str());
		// Offset
		HND hOfs = gLIB.xml_path_handle(hItem, "ofs");
		cstring sOfs = gLIB.xml_text(hOfs);
		vector<cstring> aOfs;
		sOfs.Split(aOfs, ",");
		for (int i = 0; i < 16; i++)
			m_fCalDpsA[i][1] = atof(aOfs[i].c_str());
	}


	gLIB.xml_docfree(doc);
	

}

void powerc::saveCalFile()
{
	remove(CAL_FILE);
	HND doc = gLIB.xml_docset(CAL_FILE);
	gLIB.xml_parse(doc, "<cal-data><items/></cal-data>");	
	HND hList = gLIB.xml_path_handle(doc, "/cal-data/items");
	///// CAL_ADC_BASE
	HND hItem = gLIB.xml_create("item");
	XAS(hItem, "name", "CAL_ADC_BASE");
	// Gain
	HND hGain = gLIB.xml_create("gain");
	cstring sGain;
	sGain.Format("");
	for (int i = 0; i < 8; i++)	
		sGain.AppendFormat("%.4f,", m_fCalADCBase[i][0]);	
	XTEXT(hGain, sGain.c_str());
	// Offset
	HND hOfs = gLIB.xml_create("ofs");
	cstring sOfs; 
	sOfs.Format("");
	for (int i = 0; i < 8; i++)
		sOfs.AppendFormat("%.4f,", m_fCalADCBase[i][1]);
	XTEXT(hOfs, sOfs.c_str());
	
	gLIB.xml_add(hItem, hGain); gLIB.xml_destroy(hGain);
	gLIB.xml_add(hItem, hOfs); gLIB.xml_destroy(hOfs);
	gLIB.xml_add(hList, hItem); gLIB.xml_destroy(hItem);
	
	///// CAL_HV_F
	hItem = gLIB.xml_create("item");
	XAS(hItem, "name", "CAL_HV_F");
	// Gain
	hGain = gLIB.xml_create("gain");
	sGain.Format("");
	for (int i = 0; i < 4; i++)
		sGain.AppendFormat("%.4f,", m_fCalHvF[i][0]);
	XTEXT(hGain, sGain.c_str());
	// Offset
	hOfs = gLIB.xml_create("ofs");
	sOfs.Format("");
	for (int i = 0; i < 4; i++)
		sOfs.AppendFormat("%.4f,", m_fCalHvF[i][1]);
	XTEXT(hOfs, sOfs.c_str());

	gLIB.xml_add(hItem, hGain); gLIB.xml_destroy(hGain);
	gLIB.xml_add(hItem, hOfs); gLIB.xml_destroy(hOfs);
	gLIB.xml_add(hList, hItem); gLIB.xml_destroy(hItem);
	
	///// CAL_HV_V
	hItem = gLIB.xml_create("item");
	XAS(hItem, "name", "CAL_HV_V");
	// Gain
	hGain = gLIB.xml_create("gain");
	sGain.Format("");
	for (int i = 0; i < 4; i++)
		sGain.AppendFormat("%.4f,", m_fCalHvV[i][0]);
	XTEXT(hGain, sGain.c_str());
	// Offset
	hOfs = gLIB.xml_create("ofs");
	sOfs.Format("");
	for (int i = 0; i < 4; i++)
		sOfs.AppendFormat("%.4f,", m_fCalHvV[i][1]);
	XTEXT(hOfs, sOfs.c_str());
	
	gLIB.xml_add(hItem, hGain); gLIB.xml_destroy(hGain);
	gLIB.xml_add(hItem, hOfs); gLIB.xml_destroy(hOfs);
	gLIB.xml_add(hList, hItem); gLIB.xml_destroy(hItem);

	///// CAL_HV_A
	hItem = gLIB.xml_create("item");
	XAS(hItem, "name", "CAL_HV_A");
	// Gain
	hGain = gLIB.xml_create("gain");
	sGain.Format("");
	for (int i = 0; i < 4; i++)
		sGain.AppendFormat("%.4f,", m_fCalHvA[i][0]);
	XTEXT(hGain, sGain.c_str());
	// Offset
	hOfs = gLIB.xml_create("ofs");
	sOfs.Format("");
	for (int i = 0; i < 4; i++)
		sOfs.AppendFormat("%.4f,", m_fCalHvA[i][1]);
	XTEXT(hOfs, sOfs.c_str());
	
	gLIB.xml_add(hItem, hGain); gLIB.xml_destroy(hGain);
	gLIB.xml_add(hItem, hOfs); gLIB.xml_destroy(hOfs);
	gLIB.xml_add(hList, hItem); gLIB.xml_destroy(hItem);

	///// CAL_PWR_F
	hItem = gLIB.xml_create("item");
	XAS(hItem, "name", "CAL_PWR_F");
	// Gain
	hGain = gLIB.xml_create("gain");
	sGain.Format("");
	for (int i = 0; i < 28; i++)
		sGain.AppendFormat("%.4f,", m_fCalPwrF[i][0]);
	XTEXT(hGain, sGain.c_str());
	// Offset
	hOfs = gLIB.xml_create("ofs");
	sOfs.Format("");
	for (int i = 0; i < 28; i++)
		sOfs.AppendFormat("%.4f,", m_fCalPwrF[i][1]);
	XTEXT(hOfs, sOfs.c_str());

	///// CAL_DPS_F
	hItem = gLIB.xml_create("item");
	XAS(hItem, "name", "CAL_DPS_F");
	// Gain
	hGain = gLIB.xml_create("gain");
	sGain.Format("");
	for (int i = 0; i < 16; i++)
		sGain.AppendFormat("%.4f,", m_fCalDpsF[i][0]);
	XTEXT(hGain, sGain.c_str());
	// Offset
	hOfs = gLIB.xml_create("ofs");
	sOfs.Format("");
	for (int i = 0; i < 16; i++)
		sOfs.AppendFormat("%.4f,", m_fCalDpsF[i][1]);
	XTEXT(hOfs, sOfs.c_str());

	///// CAL_DPS_V
	hItem = gLIB.xml_create("item");
	XAS(hItem, "name", "CAL_DPS_V");
	// Gain
	hGain = gLIB.xml_create("gain");
	sGain.Format("");
	for (int i = 0; i < 16; i++)
		sGain.AppendFormat("%.4f,", m_fCalDpsV[i][0]);
	XTEXT(hGain, sGain.c_str());
	// Offset
	hOfs = gLIB.xml_create("ofs");
	sOfs.Format("");
	for (int i = 0; i < 16; i++)
		sOfs.AppendFormat("%.4f,", m_fCalDpsV[i][1]);
	XTEXT(hOfs, sOfs.c_str());

	///// CAL_DPS_A
	hItem = gLIB.xml_create("item");
	XAS(hItem, "name", "CAL_DPS_A");
	// Gain
	hGain = gLIB.xml_create("gain");
	sGain.Format("");
	for (int i = 0; i < 16; i++)
		sGain.AppendFormat("%.4f,", m_fCalDpsA[i][0]);
	XTEXT(hGain, sGain.c_str());
	// Offset
	hOfs = gLIB.xml_create("ofs");
	sOfs.Format("");
	for (int i = 0; i < 16; i++)
		sOfs.AppendFormat("%.4f,", m_fCalDpsA[i][1]);
	XTEXT(hOfs, sOfs.c_str());


	gLIB.xml_add(hItem, hGain); gLIB.xml_destroy(hGain);
	gLIB.xml_add(hItem, hOfs); gLIB.xml_destroy(hOfs);
	gLIB.xml_add(hList, hItem); gLIB.xml_destroy(hItem);

	
	gLIB.xml_save(doc);
	gLIB.xml_docfree(doc);
}


