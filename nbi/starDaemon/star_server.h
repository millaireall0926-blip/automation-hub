#ifndef _STAR_SERVER_
#define _STAR_SERVER_


#define MAX_SHM_MONITOR_ITEMS	10			
#define RESULT_TEMP				"/star/test/star.tmp"
#define STAR_INI				"/star/test/stardaemon.ini"	
#define FPGA_LOG				"/star/test/FPGAUp.log"	

#include "star_session.h"
#include "proc.h"

#include "compat.h"

#include "../starlib/starlib_dif.h"
#include "../StarAPI/ioc.h"
#include "../StarAPI/eeprom.h"


class star_server
{
public:

	star_server();
	virtual ~star_server();

public:
	int			m_nDebug;

	prgctrl		procs;					// process control

	
	bool		bVmount;              
	bool		bInNotify;				
	bool		bBidCheck;				
	int			nConCount;				// connection count
	int			nSendCnt;
	int			nRecvCnt;
	int			cntProbe;	
	int			nSentLen;
	int			nRecvLen;	

	time_t		tPrevSlotStat;

public:
	SHMDEF*		pShmDef;
	int			nServerPort;
	int			nCmdSeq;
	int			nSessionMax;
	
	char		sDeamonID[256];			// ini file slot name

	star_session* pSessions;
	
	ioc			clIO;
	eeprom		clRom;
	bool		m_bIO;
	

	// ip check
	 int runFlagWaitCnt; 
	 int offWaitCnt;
					

public:


	int			setup(char *config_fn, char *sDeamonID, char*id, char* file);
	int			preProcessing(int port, int sn);
	int			doInfinite(int exitf);
	void		resetCounter();
	int			getConnectionCount();

private:
	int			acceptClient(const int idx);

	int			funcRequest(const int idx);
	int			funcFile(const int idx);
	int			processDeamon(star_session* s);
	int			processRoute(star_session* s, int dest);
	int			start();
	
	char*		cmdVersion(star_session* s, char *str);
	char*		cmdIdentify(star_session* s, char *str);
	char*		cmdControl(star_session* s, char *str);
	char*		cmdSetTime(star_session* s, char *str);
	char*		cmdBid(star_session* s, char *str);
	char*		cmdRequestFile(star_session* s, char *str);
	char*		cmdSendFile(star_session* s, char *str);
	char*		cmdDelFile(star_session* s, char *str);
	char*		cmdMap(star_session* s, char *str);
	char*		cmdStatus(star_session* s, char* str);
	char*		cmdProcess(star_session* s, char* str);
	char*		cmdFileVersion(star_session* s, char *str);
	char*		cmdSlotStatus(star_session* s, char* str, int pkt_type = PKT_TYPE_RESULT);
	char*       cmdReqTime(star_session* s, char* str, int pkt_type);
	char*		cmdRom(star_session* s, char* str);
	
	char*		cmdSetFileTransfer(star_session* s, char* str);

	int			rwCal(uint16_t command, int32_t length, uint8_t* data, FILE* pf);
	int			rwCal2(uint16_t command, int32_t length, uint8_t* data, FILE* pf);
	int			rwCal3(uint16_t command, int32_t length, uint8_t* data, FILE* pf);
	void		makeCalFile();  // PE Cal File
	void		makeCalFile2(); // DPS Cal File
	void		makeCalFile3(); // VS3 Cal File
	int			setBidRefVolt(double fVolt);

	uint		readBoardRom(uint adr);
	uint		readDPSRom(int sel, int type);
	void		writeBoardRom(uint adr, uint value);
	void		writeDPSRom(int sel, int type, word value);
	

	int			readBid();
	void		getBid(int* bid, bool bRead = true);	
	void		sendSlotInfo();
	void		alive();  
	int 		p2i(char *s);
	int			responseFile(star_session* s, int status, const char *fn);
	int			responsePing(star_session* s, char *ip, int port);

	int			checkComIP(HND h, int tryCnt);

};

#endif // _star_server_


