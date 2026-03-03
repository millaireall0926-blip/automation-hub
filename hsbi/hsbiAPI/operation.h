#ifndef __OPERATION__
#define __OPERATION__

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
	
	stPMON() { sv = -1.0; rv = -1.0, ov = -1.0; uv = -1.0; oc = -1.0; rc = -1.0; }

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
	int waitTest(int SleepTime = 100, int Waittime = 0);
	void stopTest();

	bool dpsPowerSeq(int nSeq, unsigned long long chBit, float delay);

	void readSysTemp();
	bool readCalFactor();
	bool readTDRFactor();
	bool applyTDRFactor();

	std::vector<stCalFactor> m_DPSVSCal;
	//std::vector<stCalFactor> m_DPSIMCal;

	uint    m_tPowerUpdate;
	uint    m_tTempCheck;
	uint    m_tScanUpdate;

	PMON m_PMonArr[MAX_DPS_CH + 2]; // DPS 0~19, HV0, HV1 

	void	waitTemp(int waitLimit);
	int		m_nChamSt;
	bool	m_bSetTemp;
	double	m_fSetTemp;
	double	m_fReadTemp;
	int		m_nSoakTime;
	int		m_nDefaultMpatDelay;

	unsigned int m_tStart;

	double m_IOVIH; // dac ch 0,3
	double m_DRVIH; // dac ch 1,4
	double m_VTH;   // dac ch 2,5
	double m_DRVTT; // dac ch  8,  9, 12, 13
	double m_IOVTT; // dac ch 10, 11, 14, 15
	double m_DutDRVTT; // dac ch 16, 17, 20, 21
	double m_DutIOVTT; // dac ch 18, 19, 22, 23
	double m_DutVREF; // dac ch 24 ~ 31

	std::vector<double> m_vecDacValue[2];
	std::vector<stCalFactor> m_AdcCal[2];
	std::vector<stCalFactor> m_DacCal[2];
	
	std::vector<unsigned int> m_TdrFactor[2][4]; // [scan][PG]

	std::vector<stPowerSeq> m_PSeq; // DPS Power up/down sequence

	// 미구현 함수들
	void readDPS();
};

#endif // !1

