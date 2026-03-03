#include "net.h"
#include "define.h"
#include "errc.h"
#include "result.h"
#include "starAPI.h"
#include "bib.h"
#include "STAR_TEST.h"
#include "operation.h"
#include "ioc.h"

extern starlib	g_clLib;
extern net		g_clNet;
extern errc		g_clErr;
extern result	g_clRes;
extern starAPI	g_clAPI;
extern bib		g_clBib;
extern operation g_clOperation;
extern ioc		g_clIO;

star_client::star_client(void)
{
	init();
}

star_client::~star_client(void)
{

}

int star_client::init()
{
	m_hClient = 0;
	m_nCon = 0;

	FILE* f;

	if (!(f = fopen("/star/test/stardaemon.ini", "rt")))
		return 0;

	char s[80];
	fgets(s, 80, f);
	fclose(f);

	int i = strlen(s) - 1;
	for (; i >= 0; i--)
	{
		if (isspace(s[i]) || s[i] == '\n' || s[i] == '\r')
		{
			s[i] = '\0';
			continue;
		}
		if (s[i] >= '0' && s[i] <= '9')
			break;
	}
	star_path[0] = '\0'; strcpy(star_path, s);

	return 0;
}

int star_client::close()
{
	if (m_hClient)
	{

		gLIB.star_close(m_hClient);
		gLIB.star_destroy(m_hClient);
		m_hClient = 0;
	}
	m_nCon = 0;
	return 0;
}

int star_client::send(char* str, int toGSLD)
{
	if (m_hClient == 0)return 0;
	if (m_nCon == 0)return 0;
	cstring buf;

	buf.Format("<star-p path=\"%s\">%s</star-p>", star_path, str);

	//	printf("%s\n", buf.c_str());
	int mode = STAR_TO_OS;
	if (toGSLD)mode = STAR_TO_DAEMON;

	return gLIB.star_send(m_hClient, mode, buf, buf.GetLength());
}

int star_client::open()
{

	m_hClient = gLIB.star_create();
	
	if (m_hClient && gLIB.star_open(m_hClient, "127.0.0.1", 15267))
	{
		cstring str;
		str.Format("<star-p><identify me=\"pgm\"/></star-p>");
		if (!gLIB.star_send(m_hClient, STAR_TO_DAEMON, str, str.GetLength()))
			close();
		else
			m_nCon = true;
	}
	else
		close();
	return m_nCon;
}

int star_client::recvData()
{
	if (m_nCon == 0)
		return 0;
	HND x, e;

	int ret = gLIB.star_recv(m_hClient);
	
	switch (ret)
	{
	case STAR_RET_CLOSED:
		close();
		break;
	case STAR_RET_RECV:
		cstring sData = gLIB.star_data(m_hClient);
		x = gLIB.xml_docset("star-p");
		gLIB.xml_parse(x, sData);
		e = gLIB.xml_root(x);
//		printf("recvData = %s\n",sData.c_str());
		for (e = gLIB.xml_child(e); e; e = gLIB.xml_next(e))
		{
			cstring name = gLIB.xml_name(e);
			if (name == "pgm")
			{
				cstring sub = gLIB.xml_att_find(e, "cmd");
				if (sub == "temp")
				{
					/*
						<pgm cmd="temp">
							<temp plc="1" mode="1" sv="60.0" pv="40.0"/>
					*/

					HND ht = gLIB.xml_child(e);
					gOPER.m_nChamSt = atoi(gLIB.xml_att_find(ht, "st"));
					gOPER.m_fReadTemp = atof(gLIB.xml_att_find(ht, "pv"));

					int len = gLIB.xml_source_len(ht);
					char* p = new char[len + 10];
					gLIB.xml_source(ht, p);
//					printf("%s : %s\n", __FUNCTION__, p);					
					delete[] p;
				}
				else if (sub == "fstop") // Force Stop
				{
					gAPI.debug(" !! Abort Recv. Force Stop Process !!");
					gNET.m_bfStop = true;
//					STOP_MPAT();
//					SROF();

				}
			}
		}
		gLIB.xml_docfree(x);
		break;
	}

	return 0;
}

static time_t prev_time;
void* thread_func(void* data)
{
	time_t timer;
	while (1)
	{
		timer = time(NULL);
		if (gNET.m_bCloseEvent)
			break;
		if (gNET.m_clClient.m_nCon == 0) {
			if (prev_time != timer)
				gNET.m_clClient.open();
		}

		usleep(200 * 1000);  // 100ms

	//	printf("%s %d\n", __FUNCTION__, gNET.m_clClient.m_nCon);
		gNET.shm_proc();
		gNET.m_clClient.recvData();

		if (timer > prev_time) { // 1 Sec
			gNET.sendPower();
			prev_time = timer + 1;
		}

	}
	return 0;
}


net::net(void)
{
	m_bCloseEvent = false;
	m_bThreadActive = false;
	m_nSendPower = SEND_PWR_OFF;

	m_nClientType = 0;  // default pgm
	m_bfStop = false;
}


net::~net(void)
{
	stop();
}

int net::shm_proc()
{
	if (m_pShmDef == 0)return 0;

	m_pShmDef->stList[m_nClientType].nPID = gLIB.getprocid();
	m_pShmDef->stList[m_nClientType].nCount++;

	return 1;

}

int net::shm_init()
{

	m_pShmDef = (SHMDEF*)gLIB.shm_open(0);  // shm init flag, init execute in daemon
	if (m_pShmDef == 0)return 0;

	if (m_nClientType == 0)
		sprintf(m_pShmDef->stList[m_nClientType].sName, "pgm");
	else if (m_nClientType == 1)
		sprintf(m_pShmDef->stList[m_nClientType].sName, "diag");

	if (!gAPI.m_bUseNet) return 0;
	
	shm_proc();

	m_pShmDef->nRunStat = 1;
	int checkCnt = 0;
	while (1)
	{
		if (m_pShmDef->nCheckBid == 0)
			break;

		usleep(100 * 1000);

		checkCnt++;
		if (checkCnt > 60) {
			m_pShmDef->nCheckBid = 0;  // daemon is not exist
			break;
		}


		if ((checkCnt % 10) == 0) {
			printf(".");
			shm_proc();
		}
	}

	m_pShmDef->bContact = (gAPI.m_nTestMode == 1) ? true : false;
	if (m_pShmDef->bContact) sprintf(m_pShmDef->sLotID, gAPI.m_sLot);

	sprintf(m_pShmDef->sPgmPath, "/star/pgm");
	sprintf(m_pShmDef->sPgmName, gAPI.m_sPgm);	
	sprintf(gAPI.m_sBid, "0x%X", m_pShmDef->nBid);
	sprintf(m_pShmDef->sCurFlow, "star");

	time_t t = time(NULL);
	struct tm* tp = localtime(&t);
	strftime(m_pShmDef->sStartTime, 64, "%Y%m%dT%H%M%S", tp);


	sprintf(m_pShmDef->sEndTime, "");

	
	printf("pgm \"%s\" start\n", m_pShmDef->sPgmName);  // name , path set in daemon?
	start();

	return 1;
}

int net::start()
{
		
	pthread_mutex_init(&mutex_lock, NULL);
	pthread_create(&m_hThread, NULL, thread_func, this);

	m_bCloseEvent = false;
	m_bThreadActive = true;

	usleep(1000*10);

	return 0;
}

int net::stop()
{
	//	gRES.closeResultFile();
	m_bCloseEvent = true;

	void* rt;
	if (m_bThreadActive)
		pthread_join(m_hThread, &rt);

	m_bThreadActive = false;

	return 0;
}

int net::send_msg(int type, const char* msg)
{
	cstring str, cmd;
	str.Format("<msg type=\"%d\">%s</msg>", type, msg);
	
	if (m_nClientType == 0)
		cmd = "pgm";
	else if (m_nClientType == 1)
		cmd = "diag";
//	printf("%s : %s\n", __FUNCTION__, str.c_str());
	return send_result(cmd, 1, str);
}


/*
<result seq = \"0\" cmd=\"pgm\" return=\"1\" type=\"xml\">
	<temp cmd=\"set\" sv=\"60.0\" />
</result>
*/
int net::sendTemp(int  mode)
{
	cstring tcmd = "stop";
	if (mode == 0)
		tcmd = "stop";
	else if (mode == 1)
		tcmd = "set";
	if (mode == 2)
		tcmd = "ambient";

	cstring str;
	str.Format("<temp cmd=\"%s\" sv=\"%.1f\" />", tcmd.c_str(), gOPER.m_fSetTemp);
	return send_result("pgm", 1, str);
}

/*
<result seq = \"0\" cmd=\"pgm\" return=\"1\" type=\"xml\">
	<temp cmd=\"req\" />
</result>
*/
int net::reqTemp()
{
	cstring str;
	str.Format("<temp cmd=\"req\" />");
	return send_result("pgm", 1, str);
}


int net::send_result(cstring cmd, int ok, cstring xml, int no) 
{
	cstring str;
	
	str.Format("<result seq=\"%d\" cmd=\"%s\" return=\"%s\" type=\"xml\">", 
		no, cmd.c_str(), (ok) ? "ok":"fail");
	str.AppendFormat("%s</result>", xml.c_str());
//	printf("%s : %s\n", __FUNCTION__, str.c_str());
	return m_clClient.send(str);
}


int net::sendXmlText(HND h)
{
	if (h == 0)return 0;
	int len = gLIB.xml_source_len(h);
	char* p = new char[len + 10];
	gLIB.xml_source(h, p);

	cstring  cmd;
	if (m_nClientType == 0)
		cmd = "pgm";
	else if (m_nClientType == 1)
		cmd = "diag";

	send_result(cmd, 1, p);
	delete[]p;
	return 1;
}


int net::sendPower()
{
	if (m_nSendPower == SEND_PWR_OFF) return 0;

	cstring str;

	str.Format("<power>");
	str.AppendFormat("<dps>");

	for (int i = 0; i <= PMON_DPS5; i++)
	{
		int idxArr[5] = {0, 1, 4, 2, 3};
		int idx = idxArr[i];

		if (m_nSendPower == SEND_PWR_READ) {
			str.AppendFormat("%.2f,", gOPER.m_PMonArr[idx].sv);
			str.AppendFormat("%.2f,", gOPER.m_PMonArr[idx].rv);
			str.AppendFormat("%.2f,", gOPER.m_PMonArr[idx].oc);
			str.AppendFormat("%.2f,", gOPER.m_PMonArr[idx].rc);
		}
		else {
			str.AppendFormat("%.2f,", gOPER.m_PMonArr[idx].sv);
			str.AppendFormat("%.2f,", 0.0);
			str.AppendFormat("%.2f,", gOPER.m_PMonArr[idx].oc);
			str.AppendFormat("%.2f,", 0.0);
		}
		
	}
	str.AppendFormat("</dps>");

	str.AppendFormat("<pe>");
	for (int i = PMON_VIH; i <= PMON_VTH; i++)
	{
		if (m_nSendPower == SEND_PWR_READ) {
			str.AppendFormat("%.2f,", gOPER.m_PMonArr[i].sv); // IO V
			str.AppendFormat("%.2f,", gOPER.m_PMonArr[i].rv); // CK V
			str.AppendFormat("%.2f,", gOPER.m_PMonArr[i].uv); // DR V
			str.AppendFormat("%.2f,", gOPER.m_PMonArr[i].ov); // SC V
		}
		else
		{
			str.AppendFormat("%.2f,", 0.0); // IO V
			str.AppendFormat("%.2f,", 0.0); // CK V
			str.AppendFormat("%.2f,", 0.0); // DR V
			str.AppendFormat("%.2f,", 0.0); // SC V
		}
	}

	str.AppendFormat("</pe></power>");
	send_result("pgm", 1, str);

	return 1;
}

/*
<result seq = \"0\" cmd=\"pgm\" return=\"1\" type=\"xml\">
	<scan-proc cur=\"1\" max=\"8\" val=\"0x00000001\" all=\"0\"/>"
</result>
*/
int net::sendScanInfo(int bAll)
{
	if (gAPI.m_nTestMode == TEST_MODE_DIAG) return 1; // Diag Test Mode Not Setting Normal Seq
//	printf("%s : %d/%d all(%d)", __FUNCTION__, gBIB.m_nCurScanNo, gBIB.m_nScanSize, bAll);

	cstring str;

	uint nScanpin = 0xffffffff;
	int cur = gBIB.m_nCurScanNo;
	int sz = gBIB.m_nScanSize;
	if (cur > sz -1)cur = sz/2;
	

	if (bAll)
		nScanpin = gBIB.getScan(cur)->m_nScanValue;
	
	str.Format("<scan-proc cur=\"%d\" max=\"%d\" val=\"0x%08x\" all=\"%d\"/>",
		cur + 1, sz, nScanpin, bAll);

	
	return send_result("pgm", 1, str);

	return 1;
}


int net::sendStepList()
{
	if (gRES.m_hResult <= 0) return 0;

	cstring str;
	str.Format("/test-result/info");
	HND h = gLIB.xml_path_handle(gRES.m_hResult, str.c_str());
	if (h) sendXmlText(h);

	str.Format("/test-result/bib");
	h = gLIB.xml_path_handle(gRES.m_hResult, str.c_str());
	if (h) sendXmlText(h);
	
//	str.Format("/test-result/scanpin");
//	h = gLIB.xml_path_handle(gRES.m_hResult, str.c_str());
//	if (h) sendXmlText(h);

	str.Format("/test-result/step-list");
	h = gLIB.xml_path_handle(gRES.m_hResult, str.c_str());
	if (h) sendXmlText(h);

	usleep(1000*500);  // bit ChipSet Setting Sync!  

	return 1;

}

int net::sendStartTest()
{
	cstring str;
	str.Format("<start-test id=\"star\" con=\"%d\"/>", (gAPI.m_nTestMode == TEST_MODE_CONTACT));
	send_result("pgm", 1, str);
	return 1;

}

int net::sendStartStep()
{
	if (gRES.m_hResult <= 0) return 0;

	cstring str;
	str.Format("<start-step id=\"%s\" con=\"%d\" />",
		gRES.m_sResCurStep, (gAPI.m_nTestMode == TEST_MODE_CONTACT));

	send_result("pgm", 1, str);

	return 1;
}

int net::sendEndStep()
{
	cstring str;

	if (gRES.m_hResult != 0) {
		str.Format("/test-result/step.id=%s", gRES.m_sResCurStep);
		HND hStep = gLIB.xml_path_handle(gRES.m_hResult, str.c_str());
		if (hStep) sendXmlText(hStep);
	}

	str.Format("<end-step id=\"%s\"/>",
		gRES.m_sResCurStep);
	send_result("pgm", 1, str);

	return 1;
}

int net::sendBin()
{
	cstring str;

	if (gRES.m_hResult != 0) {		
		HND hBin = gLIB.xml_path_handle(gRES.m_hResult, "/test-result/bin");
		if (hBin) sendXmlText(hBin);
	}

	return 1;
}


int net::sendResOption()
{
	if (gRES.m_hResult == 0) return 0;

	cstring str;
	str.Format("/test-result/step.id=%s", gRES.m_sResCurStep);
	HND hStep = gLIB.xml_path_handle(gRES.m_hResult, str.c_str());
	HND hToltal = gLIB.xml_find(hStep, "total");


	if (hToltal <= 0) return 0;
	
	for (HND e = gLIB.xml_next(hToltal); e; e = gLIB.xml_next(e)) {
		sendXmlText(e);
	}

	return 1;
}
 

bool net::checkError()
{
	if (!gERR.isReady() || gIO.m_sIOErrVec.size() > 0)
	{
		for (int i = 0; i < gERR.getErrCnt(); i++)
		{
			send_msg(MSG_TYPE_ERR, gERR.getErrStr(i));
			printf("%s Error %d : %s\n", __FUNCTION__, i + 1, gERR.getErrStr(i));
		}

		for (uint i = 0; i < gIO.m_sIOErrVec.size(); i++)
		{
			send_msg(MSG_TYPE_ERR, gIO.m_sIOErrVec.at(i).s);
			printf("%s IO Error %d : %s\n", __FUNCTION__, i + 1, gIO.m_sIOErrVec.at(i).s);
		}


		printf("Wait FoceStop\n");
		
		STOP_MPAT();
		SROF();
		usleep(1000 * 1000);

		while (1) {
			send_fstop();
			printf(".");
			usleep(1000 * 1000);
		}

		return false;
	}
	

	return true;	
}

int  net::sendAging(int sock, uint time)
{
	cstring str;
	str.Format("<aging sock=\"%d\" time=\"%d\"/>", sock, time);
	send_result("pgm", 1, str);
	return 1;
}

int  net::send_fstop()
{
	cstring str;

	if (gAPI.m_nTestMode == TEST_MODE_DIAG)
		str.Format("<process cmd=\"kill\" id=\"diag\"/>");
	else 
		str.Format("<process cmd=\"kill\" id=\"pgm\"/>");

	return m_clClient.send(str, 1);
}

int  net::sendEndTest()
{
	cstring str;
	str.Format("<end-test/>");
	send_result("pgm", 1, str);

	time_t t = time(NULL);
	struct tm* tp = localtime(&t);
	strftime(m_pShmDef->sEndTime, 64, "%Y%m%dT%H%M%S", tp);

	return 1;
}
