#ifndef  __NET__
#define __NET__

#include <unistd.h>	
#include <pthread.h>
#include <sys/time.h>

#include "../starlib/starlib_dif.h"
#include "String.h"

enum {
	MSG_TYPE_INFO = 0,
	MSG_TYPE_WARN = 1,
	MSG_TYPE_ERR = 2,
	MSG_TYPE_REQUST = 3,
	MSG_TYPE_REQUST_COMPLETE = 4
	
	
};

enum { 
	SEND_PWR_OFF = 0 ,
	SEND_PWR_READ = 1,
	SEND_PWR_ZERO = 2,

};

class star_client
{

public:
	star_client();
	~star_client();

public:
	char star_path[256];
	HND m_hClient;
	int m_nCon;

public:
	int init();
	int open();
	int close();
	int send(char* str, int toGSLD = 0);
	int recvData();

};

void* thread_func(void* data);


class net
{
public:
	net(void);
	~net(void);


public:
	star_client m_clClient;

	pthread_t m_hThread;
	long unsigned int m_nThreadState;
	pthread_mutex_t mutex_lock;

	bool m_bThreadActive;
	bool m_bCloseEvent;

	bool m_bfStop;
	
	int  m_nClientType; // 0 = pgm, 1=diag
	SHMDEF* m_pShmDef;

	int start();	//thread start
	int stop();		//thread stop


	int setType(int type) { m_nClientType = type; } // 0 = pgm, 1=diag

	int shm_init();
	int shm_proc();

	int m_nSendPower; 

	int send_msg(int type, const char* msg);
	int send_result(cstring cmd, int ok, cstring xml, int no = 0);	

	int sendXmlText(HND h);

	int sendPowerSet();
	int sendPower();
	int sendScanInfo(int all = 0);

	int sendStartTest();

	int sendStepList();
	int sendStartStep();
	int sendEndStep();
	int sendBin();

	int sendResOption();

	bool checkError();
	int  send_fstop();
	int  sendAging(int sock, uint time);
	
	int  sendEndTest();

	int sendTemp(int mode); // 1= run, 0=stop, 2 = ambient
	int reqTemp();

};
#endif // ! __NET__