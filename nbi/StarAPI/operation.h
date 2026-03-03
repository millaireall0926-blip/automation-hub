#ifndef __OPERATION__
#define __OPERATION__

#include <vector>

enum {
	OUT_SYNC_NONE = 0,
	OUT_SYNC_DATA = 201,
	OUT_SCAN_NEXT = 202 
};

enum {
	PMON_DPS1 = 0,
	PMON_DPS2,
	PMON_DPS3,
	PMON_DPS4,
	PMON_DPS5,
	PMON_VIH,
	PMON_VIL,
	PMON_VTH,
};

typedef union stPMON {
	struct {
		double sv;  // IO V
		double rv;  // CK V 
		double ov;	// DR V
		double uv;	// SC V
		double oc;  
		double rc;  
	};

	struct {
		double io;  // IO V
		double ck;  // CK V 
		double dr;	// DR V
		double sc;	// SC V
	};
	

}PMON;

class operation
{
public:
	operation(); 
	~operation();

	void startTest(const char* pc, bool runNow = true);
	int checkStat();
	int checkSync();
	void doScanProc();
	int waitTest(int SleepTime = 100, int Waittime = 0); // return sync type
	void stopTest();
	void readDPS();
	void readSysTemp();

	void setPmonPE(int sel, int type, double value);
	double getMaxPELevel();
	double getMinPELevel();

	void MakeCalFactor();
	void ResetCalFactor();
	bool ReadCalFactor();
	void pe_set_cal_data(int type, int ch, float gain, float offset );
	bool dpsPowerSeq(int nSeq, unsigned short chBit, float delay);

	PMON m_PMonArr[8]; // DPS 1~5, VIH, VTH, VIL
	unsigned int m_tStart;


	void	waitTemp(int waitLimit);
	int		m_nChamSt;
	bool	m_bSetTemp;
	double	m_fSetTemp;
	double	m_fReadTemp;
	int		m_nSoakTime;
	int		m_nDefaultMpatDelay;

	uint    m_tPowerUpdate;
	uint    m_tTempCheck;
	uint    m_tScanUpdate;

	

	std::vector<stCalFactor> m_Adc0Cal;
	std::vector<stCalFactor> m_Adc1Cal;
	std::vector<stCalFactor> m_VIHCal;
	std::vector<stCalFactor> m_VILCal;
	std::vector<stCalFactor> m_VTHCal;
	std::vector<stCalFactor> m_DPSVSCal;
	std::vector<stCalFactor> m_DPSIMCal;

	stCalFactor m_peDpsVccCal;

	std::vector<stPowerSeq> m_PSeq; // DPS Power up/down sequence
	unsigned int m_flgDpsOn;
};

#endif // !1

