#ifndef __OPERATION__
#define __OPERATION__




class operation
{
public:
	operation();
	~operation();

	int		m_nChamSt;
	bool	m_bSetTemp;
	double	m_fSetTemp;
	double	m_fReadTemp;
	int		m_nSoakTime;
	int		m_nDefaultMpatDelay;

	unsigned int m_tStart;

	uint    m_tPowerUpdate;
	uint    m_tTempCheck;
	uint    m_tScanUpdate;
	

	void startTest(const char* pc, bool runNow = true);
	int checkSync();
	int checkStat();	
	void doScanProc();
	int waitTest(uint SleepTime = 100, uint Waittime = 0); // return sync type
	void stopTest();
	void readDPS();	

	void	waitTemp(int waitLimit = 0);

	


};


#endif