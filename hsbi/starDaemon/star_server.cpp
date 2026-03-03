#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <time.h>

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#include "star_server.h"
#include "../hsbiAPI/eapi/ec.h"



extern starlib* lib;

star_server::star_server()
{
	printf("==== %s ====\n", PRG_VER);
	bInNotify	= false;
	bBidCheck 	= true;	
	nSessionMax	= 0;
	pSessions	= NULL;


	runFlagWaitCnt = 0;
	offWaitCnt = -1;
	resetCounter();

	pShmDef = NULL;
	m_bIO = false;
	m_nDebug = 0;

	tPrevSlotStat = 0;
}

star_server::~star_server()
{
	
	star_session* s;
	for (int i = 0; i < nSessionMax; i++) {
		s = pSessions + i;
		if (s->isConnected())
			s->end();
	}
	delete[] pSessions;
	pSessions = NULL;
}

void star_server::resetCounter()
{
	cntProbe = nConCount = nSendCnt = nRecvCnt = 0;
	nSentLen = nRecvLen = 0;
}

int star_server::getConnectionCount()
{
	int cnt = 0;
	star_session* s;
	for (int i = 0; i < nSessionMax; i++) {
		s = pSessions + i;
		if (s->soc.isConnected())
			cnt++;
	}
	return cnt - 2;
}

int star_server::p2i(char *s)		// period to integer time as interval
{
	char *q, *p = s, no[40];
	int val = 0;

	if (toupper(*p) == 'P')			// first caracter 'P' means Period, possible to omit
		p++;

	memset(no, 0, sizeof(no));
	for (q = no; *p; p++) {
		if (isdigit(*p)) {
			*q++ = *p;
		}
		else if (isalpha(*p)) {
			switch(toupper(*p)) {
				case 'H':	val += atoi(no) * 3600;		break;
				case 'M':	val += atoi(no) * 60;		break;
				case 'S':	val += atoi(no);			break;
			}
			memset(no, 0, sizeof(no));
			q = no;
		}
	}

	if (strlen(no) > 0)		// if missing last delimeter 'S', assume it as seconds
		val += atoi(no);

	return val;
}

// -----------------------------------------------------------------------------------

int	star_server::responseFile(star_session* s, int status, const char *fn)
{
	FILE *f;
	char *buf;
	size_t sz;

	if ((f = fopen(fn, "rt")) == NULL) {
		s->starResult(nCmdSeq, STAR_RESULT_FAIL, "system has some problem to open temp file");
		return S_ERR_GENERAL;
	}
	fseek(f, 0L, SEEK_END);
	sz = ftell(f) + 10;
	buf = new char[sz];
	memset(buf, 0, sz);
	fseek(f, 0L, SEEK_SET);
	int ret = fread(buf, sz, 1, f);	
	fclose(f);
	if (ret != 1)
		printf("star_server::responseFile: fread fail\n");
	s->starResult(nCmdSeq, status, buf, SESSION_ROUTER);
	delete buf;
	return S_ERR_NONE;
}

char* star_server::cmdIdentify(star_session* s, char *str)
{
	struct sessnames_t {
		int nSessionType;
		const char *name;
	} sessnames[] =
	{
		{ SESSION_OS,		"OS"		},
		{ SESSION_PGM,		"PGM"		},
		{ SESSION_DIAG,		"DIAG"		},		
		{ SESSION_ROUTER,	"ROUTER"	},
		{ SESSION_NULL,		""			}
	};
	char *p;
	
	if ((p = s->getXmlAtt("ME")) == NULL) {
		s->starResult(nCmdSeq, STAR_RESULT_FAIL);
		return str;
	}

	// if we found official session name, use it as multicast target
	// anyway, it's not error.

	strncpy(s->sSessionName, p, sizeof(s->sSessionName));
	s->nSessionType = SESSION_NULL;

	for (int i = 0; *sessnames[i].name; i++) {
		if (strcasecmp(p, sessnames[i].name) == 0) {
			s->nSessionType = sessnames[i].nSessionType;
		    printf("star_server::%s: %s\n", __FUNCTION__, p);
		}
	}

	cmdReqTime(s, NULL, PKT_DEST_ROUTER);     
	return str;
}

char* star_server::cmdMap(star_session* s, char *str)
{
	char *id = s->getXmlAtt("name");

	if (*id) {
		strcpy(sDeamonID, id);

		FILE *f;
		if ((f = fopen(STAR_INI, "wt")) != NULL) {
			printf( "map command: set CU's name from request\n");
			fprintf(f, "%s\n", sDeamonID);
			fclose(f);
		}
		else
			s->starResult(nCmdSeq, STAR_RESULT_FAIL, "can't set name");

	}
	else {
		s->starResult(nCmdSeq, STAR_RESULT_FAIL, "name attribute missing");
	}

	return str;
}

char* star_server::cmdControl(star_session* s, char *str)
{
	char *p, cmd[256];

	if ((p = s->getXmlAtt("cmd")) == NULL) {
		s->starResult(nCmdSeq, STAR_RESULT_FAIL);
		return str;
	}

	if (strncasecmp(p, "SH", 2) == 0) {
		sprintf(cmd, "%s >%s", p+3, RESULT_TEMP);
		int ret = system(cmd);
		if (ret== -1)
			responseFile(s, STAR_RESULT_FAIL, RESULT_TEMP);
		else
			responseFile(s, STAR_RESULT_OK, RESULT_TEMP);
		return str;
	}else if (strncasecmp(p, "NS", 2) == 0)
	{
		sprintf(cmd, "%s", p+3);
		printf("%s\n", cmd);
		int ret = system(cmd);		
		if (ret == -1)
			responseFile(s, STAR_RESULT_FAIL, RESULT_TEMP);
		else
			responseFile(s, STAR_RESULT_OK, RESULT_TEMP);
		return str;
	}
	else if (strcasecmp(p, "os-version") == 0) {

		sprintf(cmd, "uname -a >%s", RESULT_TEMP);
		int ret = system(cmd);
		if (ret == -1)
			responseFile(s, STAR_RESULT_FAIL, RESULT_TEMP);
		else
			responseFile(s, STAR_RESULT_OK, RESULT_TEMP);
		return str;
	}
	else if (strcasecmp(p, "sys-time") == 0) {
		time_t st = time(NULL);
		strftime(cmd, sizeof(cmd), "%Y-%m-%d %H:%M:%S", localtime(&st));
		s->starResult(nCmdSeq, STAR_RESULT_OK, cmd);
		return str;
	}

	s->starResult(nCmdSeq, STAR_RESULT_OK);
	return str;
}

char* star_server::cmdVersion(star_session* s, char *str)
{
	const char *ver = 
		"<version name=\"" NAME "\"" "version=\"" VERSION "\"" "reldate=\"" RELDATE "\"" "author=\"" AUTHOR "\"" ">" PRG_VER "</version>";
	s->starResult(nCmdSeq, STAR_RESULT_OK, ver);
	return str;
}

char* star_server::cmdFileVersion(star_session* s, char* str)
{
	char fn[256], fns[2048], tmp[512], buf[32768], *p;
	int ret;

	if ((p = s->getXmlAtt("files")) == NULL) {
		s->starResult(nCmdSeq, STAR_RESULT_FAIL);
		return str;
	}

	strncpy(fns, p, sizeof(fns)-1);
	buf[0] = '\0';

	for (p = strtok(fns, ","); p; p = strtok(NULL, ",")) {

		strncpy(fn, p, sizeof(fn)-1);

		struct stat sbuf;
		ret = stat(fn, &sbuf);

		if (ret == 0) {
			struct tm *tp = localtime(&sbuf.st_mtime);
			char tstr[40];
			strftime(tstr, sizeof(tstr), "%Y-%m-%d %H:%M:%S", tp);
			sprintf(tmp, "<file name=\"%s\" exist=\"1\" version=\"%s\" date=\"%s\" size=\"%d\"/>",
				fn, lib->version(0), tstr, (int)sbuf.st_size);
		}
		strcat(buf, tmp);
	}

	s->starResult(nCmdSeq, STAR_RESULT_OK, buf);
	printf( "file version request\n");
	return str;
}

char* star_server::cmdReqTime(star_session* s, char* str, int pkt_type)
{
	char buf[128];
	int l = sprintf(buf, "<star-p path=\"%s\"><req-time/></star-p>", s->sStarPath);
	s->starWrite(pkt_type, l, buf);

	return str;
}

char* star_server::cmdSlotStatus(star_session* s, char* str, int pkt_type)
{
	int bid;
	getBid(&bid, false);  // 

	char buf[1024] = { 0, };

	int pgm = procs.isalive("pgm");
	int diag = procs.isalive("diag");

	
	sprintf(buf, "<slot-stat bid=\"0x%X\" pgm=\"%d\" diag=\"%d\" ><run-step flow=\"%s\" lotid=\"%s\" con=\"%d\" path=\"%s\" name=\"%s\" st=\"%s\" step=\"%s\" /></slot-stat>", 
		bid, pgm, diag, pShmDef->sCurFlow, pShmDef->sLotID, pShmDef->bContact, pShmDef->sPgmPath, pShmDef->sPgmName, pShmDef->sStartTime, pShmDef->sCurStep);
	 
	s->starResult(nCmdSeq, STAR_RESULT_OK, buf, pkt_type);
//	printf( "%s : %s -> %s\n", __FUNCTION__ , s->soc.ip ,buf);

	return str;
}

char* star_server::cmdStatus(star_session* s, char* str)
{
	char buf[2048], r[256];
	size_t n;

	n = sprintf(buf, "<status role=\"%s\" connection=\"%d,%d\" probe=\"%d\" recv=\"%d\" sent=\"%d\">",
		sDeamonID, getConnectionCount(), nConCount, cntProbe,
		nRecvCnt, nSendCnt
	);

	for (int i = 2; i < nSessionMax; i++) {
		star_session* t = pSessions + i;
		if (t->isConnected()) {
			n += sprintf(r, "<client entity=\"%s\" ip=\"%s\"/>", t->sSessionName, t->soc.ip);
			if (n > sizeof(buf))
				break;
			strcat(buf, r);
		}
	}

	if (n > sizeof(buf)) {
		s->starResult(nCmdSeq, STAR_RESULT_FAIL, "too many client");
		return str;
	}

	strcat(buf, "</status>");
	s->starResult(nCmdSeq, STAR_RESULT_OK, buf);
	return str;
}

char* star_server::cmdRom(star_session* s, char* str)
{	
	int pgm = procs.isalive("pgm");
	int diag = procs.isalive("diag");
	if (pgm | diag)return str;
	

	char* cmd = s->getXmlAtt("cmd");
//	printf("cmd : %s\n", cmd);
	uint board;
	uint dpsArr[4][3];

	if (strcasecmp(cmd, "set") == 0) // write
	{
		char* board = s->getXmlAtt("b");
	//	char* d1 = s->getXmlAtt("d1");
	//	char* d2 = s->getXmlAtt("d2");
	//	char* d3 = s->getXmlAtt("d3");
	//	char* d4 = s->getXmlAtt("d3");
	//	printf("-- %s %s %s %s %s\n", board, d1, d2, d3, d4);
		int val = strtol(board, NULL, 16);

		writeBoardRom(0, val);
		for (int i = 0; i < 4; i++)
		{
			char att[16];
			sprintf(att, "d%d", i + 1);
			char* sSum = s->getXmlAtt(att);
//			printf("dps %d=%s : ", i+1, sSum);
			char* ss = strtok(sSum, ",");
			
			for (int j = 0; j < 3; j++)
			{
				int n = 0;
				if (ss != NULL)
					 n = strtol(ss, NULL, 16);
				writeDPSRom(i, j, n);
//				printf("%s -> 0x%x, ",ss,  n);				
				ss = strtok(NULL, ",");
			}
	//		printf("\n");
		}
		

	}
	else 
	{		
		for (int i = 0; i < 4; i++) {
			dpsArr[i][0] = readDPSRom(i, 0);
			dpsArr[i][1] = readDPSRom(i, 2);
			dpsArr[i][2] = readDPSRom(i, 4);
		}
		board = readBoardRom(0); // adr 0 : 4 byte read		

		char buf[1024] = { 0, };
		char subbuf[128] = { 0, };

		sprintf(buf, "<rom><board val=\"%x\"/>", board);
		for (int i = 0; i < 4; i++)
		{
			sprintf(subbuf, "<dps id=\"%x\" val=\"%x,%x,%x\"/>", i,
				dpsArr[i][0], dpsArr[i][1], dpsArr[i][2]);
			strcat(buf, subbuf);
		}
		strcat(buf, "</rom>");
//		printf("send : %s : %s\n", __FUNCTION__, buf);

		s->starResult(nCmdSeq, STAR_RESULT_OK, buf);
	}

	return str;
}

char* star_server::cmdProcess(star_session* s, char* str)
{
	char* cmd = s->getXmlAtt("cmd");
	char* id = s->getXmlAtt("id");

	if (strcasecmp(cmd, "add") == 0) {  // �̹� pgm�� diag�� add �Ǿ� ����
		
	}
	else if (strcasecmp(cmd, "remove") == 0) {
		s->starResult(nCmdSeq, procs.remove(id));
	}
	else if (strcasecmp(cmd, "run") == 0) {
		char* exec = s->getXmlAtt("exec");
		s->starResult(nCmdSeq, procs.pexec(id, exec), "run");
	}
	else if (strcasecmp(cmd, "kill") == 0) {

		// Process Kill
		int ret = procs.pkill(id);
		printf("kill %s\n", id);

		// PG Stop
		uint adr = PG_SYSTEM | 3;
		clIO.writePG(PCI_SEL::PCI1, PG_ALL, adr, 4, 1);
		clIO.writePG(PCI_SEL::PCI2, PG_ALL, adr, 4, 1);
	
		// DPS off	
		for (int ch = 0; ch < 32; ch++) {
			for (int pci = 0; pci < MAX_PCI; pci++) {
				clIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, 0);
				clIO.dac_write(PCI_SEL(pci), DAC_REG_SEL::INPUT, ch, 0);
			}
		}

		//for(int idx = DPS_START_CH; idx < MAX_DPS_CH; idx++)
		//	gDPS.SetVout(idx, 0.1, 0, 0.1, 5);

		clIO.dps_run(PCI1, 0x3FF, 0x1, 0);
		clIO.dps_run(PCI2, 0x3FF, 0x1, 0);

		sleep(0.1);

		//DG S3 S5 enable
		clIO.writePG(PCI1, 0x3, 0x238, 0);
		clIO.writePG(PCI2, 0x3, 0x238, 0);
		//PG S3 S5 enable
		clIO.writePG(PCI1, 0x3, 0x239, 0);
		clIO.writePG(PCI2, 0x3, 0x239, 0);



		//Send Kill Message
		s->starResult(nCmdSeq, ret, "kill");

	}
	else if (strcasecmp(cmd, "check") == 0) {
		s->starResult(nCmdSeq, procs.isalive(id),  "check");
	}
	else {
		s->starResult(nCmdSeq, STAR_RESULT_FAIL, "invalid command");
	}

	return str;
}

char* star_server::cmdDelFile(star_session* s, char* str)
{
	char *source = s->getXmlAtt("source");
	strcpy(s->sTag, "DEL-FILE");
	
	if (strlen(source) == 0) {
		s->starResult(nCmdSeq, STAR_RESULT_FAIL, "'source' sTag unspecified");
		return str;
	}

	remove(source);

	s->starResult(nCmdSeq, STAR_RESULT_OK, "");
	return (char*)"";
}

char* star_server::cmdRequestFile(star_session* s, char* str)
{
	char *source = s->getXmlAtt("source");
	char *target = s->getXmlAtt("target");
	char source_fn[256];
		
	strcpy(s->sTag, "REQ-FILE");

	if (strlen(source) == 0) {
		s->starResult(nCmdSeq, STAR_RESULT_FAIL, "'source' sTag unspecified");
		return str;
	}

	if (strlen(target) == 0) {
		s->starResult(nCmdSeq, STAR_RESULT_FAIL, "'target' sTag unspecified");
		return str;
	}

	strcpy(source_fn, source);
	
	if (!s->starSendFile(sDeamonID, PKT_TYPE_REQFILE /*| PKT_COMPRESSED*/, source_fn, target)) 
		printf( "%s: '%s' failed\n", __FUNCTION__, source_fn);
	else
		printf( "%s: '%s' send success\n", __FUNCTION__, source_fn);
		
	return (char*)"";
}

char* star_server::cmdSendFile(star_session* s, char *str)
{
	char fn[256];
	char* dir = s->getXmlAtt("dir");
	char* name = s->getXmlAtt("name");
	int sz = atoi(s->getXmlAtt("size"));

	if (*dir) {
		if (dir[strlen(dir)-1] == '/')
			sprintf(fn, "%s%s", dir, name);
		else
			sprintf(fn, "%s/%s", dir, name);
		//sprintf(fn, "%s/%s", dir, name);
	}
	else
		strcpy(fn, name);
			
	char *q = str + sz;

	if (s->starRecvFile()) {
		printf( "'%s' recv success\n", fn);
		return (char*)"";
	}
		
	return s->parseXml(q);		// skip </send-file>
}

char* star_server::cmdSetFileTransfer(star_session* s, char* str)
{
	strcpy(s->sFtFilename, s->getXmlAtt("path"));
	s->nFtSize = atoi(s->getXmlAtt("size"));

	s->pFtFileHandle = fopen(s->sFtFilename, "wb");
	if (s->pFtFileHandle == NULL) {
		s->starResult(nCmdSeq, STAR_RESULT_FAIL, "can't create file\n");
		return str;
	}

	if (s->soc.rd.cnt > 0) {
		if (fwrite(s->soc.rd.buf, s->soc.rd.cnt, 1, s->pFtFileHandle) != 1) {
			s->starResult(nCmdSeq, STAR_RESULT_FAIL, "can't write file\n");
			return str;
		}
		s->nFtSize -= s->soc.rd.cnt;
		s->soc.rd.cnt = 0;
	}
	
	s->bFtMode = 1;

	s->starResult(nCmdSeq, STAR_RESULT_OK, "");
	printf("star_server::%s: start. fn=%s, size=%d, rd.cnt=%d\n", __FUNCTION__, s->sFtFilename, s->nFtSize, s->soc.rd.cnt);
	
	return str;
}


char* star_server::cmdSetTime(star_session* s, char *str)
{
	char *p = s->getXmlAtt("date");

	if (*p) {

		struct tm tm;
		time_t st = time(NULL);

		memcpy(&tm, localtime(&st), sizeof(tm));
		int n = sscanf(p, "%04d%02d%02dT%02d%02d%02d",
				&(tm.tm_year), &(tm.tm_mon), &(tm.tm_mday),
				&(tm.tm_hour), &(tm.tm_min), &(tm.tm_sec)
		);
		
		if (n != 6) {
			s->starResult(nCmdSeq, STAR_RESULT_FAIL, "bad argument", SESSION_ROUTER);
			return str;
		}

		char buf[64];
		
		sprintf(buf, "sudo date -s \'%d-%d-%d %d:%d:%d\'", (tm.tm_year), (tm.tm_mon), (tm.tm_mday),
			(tm.tm_hour), (tm.tm_min), (tm.tm_sec));
		
		if (system(buf) == -1)
			printf("star_server::cmdSetTime: System Cmd Fail\n");
		
	}


	s->starResult(nCmdSeq, STAR_RESULT_OK, "", SESSION_ROUTER);
	procs.resetTime();

	return str;
}

char* star_server::cmdBid(star_session* s, char* str)
{
	char *p = s->getXmlAtt("check");
	
	if (*p == '1' || *p == 'y' || *p == 'Y')
		bBidCheck = true;
	else
		bBidCheck = false;
		
	return str;
}

int star_server::processDeamon(star_session* s)
{
	char *p;


//	printf("%s: RECV: %s\n", __FUNCTION__, s->packet.buf);
	p = s->parseXml(s->packet.buf);
	if (strcmp(s->sTag, "STAR-P") != 0) {
		return S_ERR_GENERAL;
	}
	
	nCmdSeq = 0;
	while (*p) {
		p = s->parseXml(p);
		if (strcmp(s->sTag, "STAR-P") == 0 && s->nTagType == STAR_XML_CLOSE)
			break;

		if (s->nTagType == STAR_XML_NOCMD) {
			return S_ERR_GENERAL;
		}

//		printf("%s: RECV tag: %s\n", __FUNCTION__, s->sTag);

		if (strcmp(s->sTag, "IDENTIFY") == 0) {
			p = cmdIdentify(s, p);
		}
		else if (strcmp(s->sTag, "CTRL-CM") == 0) {
			p = cmdControl(s, p);
		}
		else if (strcmp(s->sTag, "SET-TIME") == 0) {	// set system time
			p = cmdSetTime(s, p);
		}
		else if (strcmp(s->sTag, "GET-TIME") == 0) {
		}
		else if (strcmp(s->sTag, "BID") == 0) {
			p = cmdBid(s, p);
		}
		else if (strcmp(s->sTag, "SEND-FILE") == 0) {	
			p = cmdSendFile(s, p);
		}
		else if (strcmp(s->sTag, "REQ-FILE") == 0) {	// request file. 
			p = cmdRequestFile(s, p);
		}
		else if (strcmp(s->sTag, "DEL-FILE") == 0) {	// request file. 
			p = cmdDelFile(s, p);
		}
		else if (strcmp(s->sTag, "MAP") == 0) {			// rename this COM entity name
			p = cmdMap(s, p);
		}
		else if (strcmp(s->sTag, "VERSION") == 0) {		// version info
			p = cmdVersion(s, p);
		}
		else if (strcmp(s->sTag, "STATUS") == 0) {		// current status
			p = cmdStatus(s, p);
		}
		else if (strcmp(s->sTag, "SLOT-STAT") == 0) {
			p = cmdSlotStatus(s, p);
		}
		else if (strcmp(s->sTag, "PROCESS") == 0) {		
			printf("%s: RECV %s : %s\n", __FUNCTION__,s->soc.ip,  s->packet.buf);
			p = cmdProcess(s, p);
		}
		else if (strcmp(s->sTag, "VERSION") == 0) {		
			p = cmdFileVersion(s, p);
		}
		else if (strcmp(s->sTag, "SET-FT") == 0) {
			p = cmdSetFileTransfer(s, p);
		}
		else if (strcmp(s->sTag, "ROM") == 0) {
			p = cmdRom(s, p);
		} else {
			return S_ERR_GENERAL;
		}
		nCmdSeq++;
	}

	return S_ERR_NONE;
}

int star_server::processRoute(star_session* s, int dest)
{
	star_session *t;
	int i;

	for (i = 2; i < nSessionMax; i++) {
		t = pSessions + i; 
		if (!t->isConnected())
			continue;
		if (m_nDebug)
  			printf("star_server::%s: %s\n",__FUNCTION__, s->packet.buf);

		if (t->nSessionType == dest || t->nSessionType == SESSION_ROUTER) {
			nSentLen = s->pkt_header.len;
			if (t != s) {
				nSendCnt++;
				if (t->starWrite(s->pkt_header.mode, s->pkt_header.orglen, s->packet.buf) != S_ERR_NONE) {
					printf("star_server::%s: starWrite failed. close!\n", __FUNCTION__);
					t->end();
				}
			}
		}
	}

	return S_ERR_NONE;
}

int star_server::responsePing(star_session* s, char *ip, int port)
{
	char *p;

	p = s->parseXml(s->packet.buf);
	if (strcmp(s->sTag, "STARINK") != 0) {
		return S_ERR_NONE;					// no process, no error
	}

	if (s->soc.socket_type == SOC_UDP)
		s->soc.clearTcpBuf();

	while (*p) {
		p = s->parseXml(p);
		if (strcmp(s->sTag, "STARINK") == 0 && s->nTagType == STAR_XML_CLOSE)
			break;							// found closing sTag

		if (s->nTagType == STAR_XML_NOCMD)
			return S_ERR_NONE;

		if (strcmp(s->sTag, "IDENTIFY") == 0) {	// client request who am i
			char buf[256];
			memset(buf, 0, sizeof(buf));

			if (!bInNotify) {		// don't response during notify period
				int err;

				sprintf(buf, "<star-p path=\"%s\"><identify entity=\"CU\" name=\"%s\" ip=\"%s\" port=\"%d\"/></star-p>", 
					sDeamonID, sDeamonID, s->soc.getLocalAddr(0), nServerPort);
				err = s->starWrite(PKT_TYPE_ADMIN, strlen(buf), buf);
				if (err == S_ERR_GENERAL) {
					printf("star_server::%s: starWrite failed. close!\n", __FUNCTION__);
					s->end();
				}
				return err;
			}
		}
		else {
			return S_ERR_GENERAL;
		}
	}

	return S_ERR_NONE;		// not reach
}

int star_server::funcFile(const int idx)
{
	star_session* s = pSessions + idx;
	char buf[65536];
	
	int sz = s->soc.bufRemains();
	
	if (sz > 65535) sz = 65535;
	int rsz = (s->nFtSize > sz) ? sz : s->nFtSize;
	
	rsz = s->soc.bufRead2(buf, rsz);
	if (rsz > 0) {
		fwrite(buf, rsz, 1, s->pFtFileHandle);
		s->nFtSize -= rsz;
	}
	else {
		s->bFtMode = 0;
		if (s->pFtFileHandle)
			fclose(s->pFtFileHandle);
		s->pFtFileHandle = NULL;
		printf("star_server::%s: '%s' complete\n", __FUNCTION__, s->sFtFilename);

		return 0;
	}
	
	if (s->nFtSize == 0) {
		s->bFtMode = 0;
		fclose(s->pFtFileHandle);
		s->pFtFileHandle = NULL;
		printf("star_server::%s: file '%s' recv ok\n", __FUNCTION__, s->sFtFilename);
	}
	return 1;	
}

int star_server::funcRequest(const int idx)
{
	star_session* s = pSessions + idx;
	int ret = 0;
	int dest, kind;

	while(1) {						// process all the messages

		if (!s->isConnected()) {
			printf("star_server::funcRequest: [%d] session is not used\n", idx);
			return S_ERR_NONE;
		}

		if ((ret = s->starRead()) == S_ERR_QUEUED) {
			return ret;
		}

		if (ret == S_ERR_GENERAL) {

			s->end();			
			printf(  "star_server::funcRequest: [%d] session closed from remote\n", idx);
			return S_ERR_GENERAL;
		}
		
		kind = s->pkt_header.mode & 0xff00;
		dest = s->pkt_header.mode & 0x00ff;

		if (kind == PKT_TYPE_ADMIN) {		
			responsePing(s, s->soc.udp_ip, s->soc.udp_port);
			cntProbe++;
			continue;
		}

		nRecvCnt++;
		nRecvLen = s->pkt_header.orglen;

		if (dest == PKT_DEST_DAEMON) {
			
			ret = processDeamon(s);
			
		}
		else
			ret = processRoute(s, dest);
		
		if (ret != S_ERR_NONE) {
			printf("bad command %s\n", s->packet.buf);
			s->starResult(0, STAR_RESULT_FAIL, "bad star-p command format. clear buffer");
			printf("star_server::funcRequest: [%d] break packet clear buffer\n", idx);
			s->soc.bufClear();
			return ret;
		}

		if (s->bFtMode)
			return S_ERR_NONE;
			
		if (nRecvCnt % 10)
			s->soc.msleep(1);
	}

	return ret;
}

int star_server::acceptClient(const int idx)
{
	int				connfd = 0;
	struct sockaddr_in	cliaddr;
	star_session* s = pSessions + idx;
	
	unsigned int	clilen;
	clilen = sizeof(cliaddr);
	if ((connfd = accept(s->soc.fd, (struct sockaddr *)&cliaddr, &clilen)) < 0) {
		if (EWOULDBLOCK != errno && ECONNABORTED != errno && EINTR != errno) {
			printf("star_server::acceptClient : error : %s\n", strerror(errno));
			exit(-1);
			return S_ERR_GENERAL;
		}
	}

	for (int i = 2; i < nSessionMax; i++) { 
		s = pSessions + i;
		if (!s->isConnected()) {

			s->soc.fd = connfd;
			s->soc.bConn = true;
			s->soc.setSocketType(SOC_TCP);
			s->nSessionType = SESSION_DEFAULT;
			s->setPath(sDeamonID);

			s->soc.setNonblock();
			s->soc.setKeepAlive();
			s->soc.getPeerInfo();
			
			nConCount++;
			printf( "[%d] connect from %s\n", i, s->soc.ip);
			strcpy(s->sTag, "slot-stat");			
			cmdSlotStatus(s, NULL, PKT_TYPE_SLOTSTAT);     
			return S_ERR_NONE;
		}
	}

	return S_ERR_GENERAL;
}

void star_server::alive()
{
	static time_t st = 0;
	static time_t h = 0;	
	char s[256]; 
	if (st == time(NULL))
		return;
	
	st = time(NULL);
	
	if (h + 3600 > time(NULL)) 
		return;

	h = time(NULL);
	sprintf(s, "mark: probe(%d), connection(%d), recv(%d), send(%d)\n", cntProbe, nConCount, nRecvCnt, nSendCnt);
	printf("%s", s);
}

uint star_server::readBoardRom(uint adr)
{
	uint value = 0;
	int rv = clRom.read_eeprom(BUSID_EC_I2C, TCB_ROM, adr, 4, (uint8_t*)&value);
	if (rv != 0) value = 0;

	return value;
}

void star_server::writeBoardRom(uint adr, uint value)
{

	clRom.write_eeprom(BUSID_EC_I2C, TCB_ROM, adr, 4, (uint8_t*)&value);
	uint rd;
	clRom.read_eeprom(BUSID_EC_I2C, TCB_ROM, adr, 4, (uint8_t*)&rd);


}

uint star_server::readDPSRom(int sel, int type)
{
	uint adr = 0xb0;
	if (type == 0)adr = 0xb0; // user data 0
	if (type == 2)adr = 0xb2; // user data 2
	if (type == 4)adr = 0xb4; // user data 4


	PCI_SEL pci = (sel > 4) ? PCI2 : PCI1;
	int i2cNo = sel % 5;

	uint dps = 0;
	clIO.dps_read(pci, i2cNo, adr, 2, (byte*)&dps);
	return dps;
}

void star_server::writeDPSRom(int sel, int type, word value)
{
	uint adr = 0xb0;
	if (type == 0)adr = 0xb0; // user data 0
	if (type == 1)adr = 0xb2; // user data 2
	if (type == 2)adr = 0xb4; // user data 4

	PCI_SEL pci = (sel > 4) ? PCI2 : PCI1;
	int i2cNo = sel % 5;
	
	
	clIO.dps_write(pci, i2cNo, adr, 2, (byte*)&value);
	usleep(1000);	
	clIO.dps_send_byte(pci, i2cNo, 0x15); // Store User data 		
	
}




int star_server::readBid()
{
	int Bid = 0;

	if (!m_bIO)return Bid;
	Bid = clIO.readBID();

	return Bid;
	
}

void star_server::getBid(int *Bid, bool bHWRead)
{
	static int stcOldBid = -1;
	*Bid = 0;
	if (!bBidCheck) {
		*Bid = stcOldBid;
		return;
	}

	if (!(pShmDef->nCheckBid) || !(bHWRead)){
		*Bid = stcOldBid;

		return;
	}


	if (pShmDef->nRunStat){
		*Bid = stcOldBid;
		if (pShmDef->nCheckBid){
			pShmDef->nCheckBid = 0;
		
		}
	}else {
		if (bVmount){
			*Bid = 0x00001234;
		}else 
			*Bid = readBid();

		pShmDef->nBid = *Bid;
		
	}
	stcOldBid = *Bid;
}

void star_server::sendSlotInfo()
{

	static int oldBid = -1;	
	static int oldDiag = -1;
	static int oldPgm = 0;
	static int oldRunProc = -1;
	static time_t st = time(NULL);

	int	Bid, diag, pgm, runProc;

	if (st == time(NULL))
		return;

	st = time(NULL);
	pgm = procs.isalive("pgm");	 
	diag = procs.isalive("diag");
	
	runProc = 0;
	if (pgm || diag)runProc = 1;
	
	getBid(&Bid, (runProc == 0)); 

	bool bSendStatus = false;
	if ((oldBid != Bid) || (oldPgm != pgm) || (oldDiag != diag) || (oldRunProc != runProc))
		bSendStatus = true;
	
	if (pgm) {
		if (tPrevSlotStat < st)
		{
			bSendStatus = true;
			tPrevSlotStat = st + 2;
		}
	}

	if (bSendStatus) {
		star_session* t;
		for (int i = 2; i < nSessionMax; i++) {
			t = pSessions + i;
			if (!t->isConnected()) continue;
			strcpy(t->sTag, "slot-stat");
			cmdSlotStatus(t, NULL, PKT_TYPE_SLOTSTAT);
		}
	}
	
	if (pShmDef->nRunStat && runProc == 0)  // prg 0, flag 1
	{
		if (runFlagWaitCnt > 30) {
			runFlagWaitCnt = 0;
			pShmDef->nRunStat = 0;
			pShmDef->nCheckBid = 1;
		}
		runFlagWaitCnt++;
	}
	
	// bid read 관련 처리..
	if ( Bid == oldBid && pgm == oldPgm && diag==oldDiag ){  // bid & stat change
		if (offWaitCnt != -1 && runProc == 0)  // off wait, prg 0, flag 1
		{
			if (offWaitCnt == 0)
			{
				pShmDef->nRunStat = 0;
				pShmDef->nCheckBid = 1;
				runFlagWaitCnt = 0;
			}

			offWaitCnt--;
		}
		return;
	}
	printf("5");
	if ( runProc != oldRunProc){
		if (runProc == 0) { 
			offWaitCnt = 10; 
			printf("process \'%s\' killed Mount Check Wait : run %d, check %d\n",  
				pShmDef->sPgmName, pShmDef->nRunStat, pShmDef->nCheckBid);
		}else {
			runFlagWaitCnt = 0;
			pShmDef->nCheckBid = 0;
			printf("process \'%s\' run : run %d, check %d\n",
				pShmDef->sPgmName, pShmDef->nRunStat, pShmDef->nCheckBid);
		}
	}

	oldBid = Bid;
	oldPgm = pgm;
	oldDiag = diag;
	oldRunProc = runProc;
}




int star_server::doInfinite(int bQuit)
{
	int				i, maxfd, nready;
	fd_set			readset, writeset;
	struct timeval	tv;
	star_session		*s;

	do {
		
		alive();
		procs.check();
		sendSlotInfo();		
		usleep(5);
		maxfd =	0;
		FD_ZERO(&readset);
		FD_ZERO(&writeset);		
		for	(i = 0; i < nSessionMax; i++) {
			s =	pSessions +i ;
			if (!s->isConnected())
				continue;

			FD_SET(s->soc.fd, &readset);
			maxfd =	MAX((unsigned)maxfd, (unsigned)s->soc.fd);

			if (0 != s->soc.wr.cnt && NULL != s->soc.wr.buf) {
				FD_SET(s->soc.fd, &writeset);
				maxfd =	MAX((unsigned)maxfd, (unsigned)s->soc.fd);
			}
		}

		tv.tv_sec =	0;							
		tv.tv_usec = 2000;					              

		
		nready = select(maxfd +	1, &readset, &writeset,	NULL, &tv);
		
		if (nready < 0)	{
			printf("star_server::doInfinite:Connection Close  %s\n", strerror(errno));
			continue;
		}
		else if	(0 == nready) {				

			for	(i = 0;	i <	nSessionMax; i++) {
				s =	pSessions + i;
				if (s->isConnected() && 0 < s->soc.rd.cnt) {
					if (s->bFtMode)
						funcFile(i);
					else
						funcRequest(i);
				}

				if (s->isConnected()  && s->soc.wr.cnt > 0) {
					if (s->soc.bufFlush() == S_ERR_GENERAL) {
						s->end();

					}
				}
			}

			continue;
		}		


		for	(i = 0;	i <	nSessionMax; i++) {

			if (0 == nready)
				break;

			s =	pSessions + i;
			if (s->isConnected() && FD_ISSET(s->soc.fd, &readset)) {
				if (i == 0) {
					acceptClient(i);
				}
				else {
					if (s->bFtMode) 
						funcFile(i);
					else
						funcRequest(i);
				}
				nready--;
			}

			if (0 == nready)
				break;

			if (s->isConnected() && FD_ISSET(s->soc.fd, &writeset) && s->soc.wr.cnt != 0 && s->soc.wr.buf != NULL) {
				s->soc.bufFlush();
				nready--;
			}

		} 

	} while (!bQuit);	
						

	return S_ERR_NONE;
}

int star_server::start()
{
	printf( "Start Server.\n");

	star_session* s = pSessions + 1;
	s->soc.socket_type = SOC_UDP;

	if (!s->soc.connect("255.255.255.255", STAR_PROBE_PORT + 1)) {		// prevent conflict with gc client
		printf("star_server::%s: failed to open udp listen port:%d\n", __FUNCTION__, STAR_PROBE_PORT + 1);
		return S_ERR_GENERAL;
	}

	char buf[1024];
	int ret;

	s->soc.setBroadcast(1);

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "<star-p path=\"%s\"><identify entity=\"CU\" name=\"%s\" port=\"%d\"/></star-p>", sDeamonID, sDeamonID, nServerPort);

	strcpy(s->soc.udp_ip, s->soc.getLocalAddr(2));
	s->soc.udp_port = STAR_PROBE_PORT;
	ret = s->starWrite(PKT_TYPE_ADMIN, strlen(buf), buf);

	if (ret != S_ERR_NONE) {
		printf("star_server::%s: notify failed, but continue\n", __FUNCTION__);
		s->end();
	}
	
	bInNotify = true;
	
	doInfinite(1);

	s->end();

	bInNotify = false;
	return S_ERR_NONE;
}





int star_server::setup(char *config_fn, char *dID, char* id, char* file)
{
	
	HND h, daemon_h;	
	int port;

	// ----------- sDeamonID name setup

	if (dID == NULL) {
		FILE *f;
		if ((f = fopen(STAR_INI, "rt")) != NULL) {
			if (fgets(sDeamonID, sizeof(sDeamonID), f) == NULL)
				printf("star_server::setup: DeamonID Read Fail\n");
			fclose(f);
			sDeamonID[strlen(sDeamonID)-1] = '\0';
		}
		else
			strcpy(sDeamonID, "slot");
	}
	else
		strcpy(sDeamonID, dID);

	// ----------- config file load
	if (!lib->xml_load(config_fn, &h)) {
		printf( "config file '%s' not found\n", config_fn);
		return 0;
	}
	//	printf("%s HND %08x\n", __FUNCTION__, h);	// ----------- daemon setup

	if ((daemon_h = lib->xml_path_handle(h, "/config/daemon")) == 0) {
		
		printf("/config/daemon not found in file %s\n", config_fn);
		lib->xml_docfree(h);
		return 0;
	}

	bVmount = (atoi(lib->xml_att_find(daemon_h, "vmount")) == 1);
	port = atoi(lib->xml_att_find(daemon_h, "port"));
	int boardIP = atoi(lib->xml_att_find(daemon_h, "boardIP"));  
	int sim = atoi(lib->xml_att_find(daemon_h, "sim"));  
	if(sim == 1) clIO.m_bSim = true;
	// board ip 는 i2c(0x70,  ch 0 :site id 0-7bit site id 8~15bit LED) 에서  eeprom class 참조. 
	//                        ch 1 : power control  
	//					      ch 2 : eeprom inventory
	//						  ch 3 : cal data
	
	// IO / FPGA Check
	FILE* fpLog = fopen(FPGA_LOG, "wt");

	if(sim == 0)
		m_bIO = true;
	
	if (clIO.open_pcie() == 1) // open error
	{
		bVmount = 1;
		m_bIO = false;
		fprintf(fpLog, "PCI Open Fail!\n");
	}
	
	if (m_bIO) {
		uint pg1ID = 0;
		uint pg2ID = 0;
		uint pgClkSel = 0;
		//uint tgClkSel = 0;
		//uint tgLatency = 0;
		HND fh = lib->xml_path_handle(h, "/config/fpga");
		if (fh > 0)
		{
			char clk[64];
			char* p, * q;
			strcpy(clk, lib->xml_att_find(fh, "pg-clk-sel"));
			p = clk;
			q = p + strlen(clk);
			pgClkSel = strtoul(p, &q, 16);
			printf("Clk-Sel-Value : 0x%x\n", pgClkSel);
			clIO.c2p_lb_clk_sel(pgClkSel);
			clIO.dac_vtt_bias_en(PCI1, 1);
			clIO.dac_vtt_bias_en(PCI2, 1);

			
			char id[64];
			strcpy(id, lib->xml_att_find(fh, "pg1-id"));			
			p = id;
			q = p + strlen(id);
			pg1ID = strtoul(p, &q, 16);
			printf("PG1 FPGA : s:%s, d:%x\n", id, pg1ID);

			strcpy(id, lib->xml_att_find(fh, "pg2-id"));
			p = id;
			q = p + strlen(id);
			pg2ID = strtoul(p, &q, 16);
			printf("PG2 FPGA : s:%s, d:%x\n", id, pg2ID);

			int tgLatency = atoi(lib->xml_att_find(fh, "tg-latency"));
			clIO.c2t_read_latency(tgLatency);

			int tgClkSel = atoi(lib->xml_att_find(fh, "tg-clk-sel"));
			clIO.c2t_lb_clk_sel(tgClkSel);

		}
		
		if (!clIO.checkPG(PCI1, pg1ID)) fprintf(fpLog, "PG1 ID Read Fail!!\n");
		if (!clIO.checkPG(PCI2, pg2ID)) fprintf(fpLog, "PG2 ID Read Fail!!\n");
	}
	fclose(fpLog);

	
	printf("read flag : m_bIO %d, boardIP %d\n", m_bIO, boardIP);

	// ----------- process items for monitoring

	if (m_bIO && (boardIP == 1)) {
		FILE* pf = fopen("/star/test/BoadIP.log", "wt");
		for (int i = 0; i < 60 ; i++) { // Max 1 Min Check
			int ret = checkComIP(daemon_h, i);
			if (ret == 0) {
				fprintf(pf, "IP Matched HW SITE ID\n");
				break;
			}else if (ret == 1) {
				fprintf(pf, "Class IP Not fount\n");
				usleep(1000 * 1000);
			}
			else if (ret == 2) {
				fprintf(pf, "IP Changed\n");				
			}
		}
		if (pf != NULL)
			fclose(pf);
	}


	HND ph = lib->shm_open(1);
	pShmDef = (SHMDEF*)ph;
	procs.setup();

	//		  	name		dir			cmd	  
	procs.add(	"pgm",		""	,		""	);		// Index 0
	procs.add(	"diag",		"",			"diag -d"); // Index 1

	if (pShmDef->nRunStat == 0)
	{
		pShmDef->nCheckBid = 1;
	}

	preProcessing(port, 100);

	start();

	return S_ERR_NONE;
}

int star_server::preProcessing(int port, int sn)
{
	star_session		*s;

	// init socket


	resetCounter();
	nServerPort = port;

	
	// create sessions

	nSessionMax = sn;
	pSessions = new star_session[nSessionMax];

	if (pSessions == NULL) {
		printf( "star_server: can't create session table\n");
		return S_ERR_GENERAL;
	}

	// create server listen port

	s = pSessions + 0;
	s->soc.setSocketType(SOC_TCP);
	if (!s->soc.listen(port)) {
		printf( "star_server: failed to open listening socket on port:%d\n", port);		
		return S_ERR_GENERAL;
	}	

	return S_ERR_NONE;
} 


#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
int star_server::checkComIP(HND h, int tryCnt)
{
	char class_ip[64];
	char last_ip[16];
	char full_ip[64];
	sprintf(class_ip,"%s", lib->xml_att_find(h, "classIP"));
	//int last = clIO.readSiteID() & 0xff;
	int last = clIO.readSlotID();
	printf("Read Port : %02X\n", last);
//	int revH = (last & 0xf) << 4;
//	int revL = (last & 0xf0) >> 4;
//	int sw1 = 0;
//
//	sw1 |= (revH & 0x80) >> 3;
//	sw1 |= (revH & 0x40) >> 1;
//	sw1 |= (revH & 0x20) << 1;
//	sw1 |= (revH & 0x10) << 3;
//	printf("H %x -> %x\n", revH, sw1);
//
//	int sw2 = 0;
//	sw2 |= (revL & 8) >> 3;
//	sw2 |= (revL & 4) >> 1;
//	sw2 |= (revL & 2) << 1;
//	sw2 |= (revL & 1) << 3;
//	printf("L %x -> %x\n", revL, sw2);

//	int ten = (sw1 >> 4) * 10;
//	int il = sw2;
//	last = ten + il;

	// board id check ( 1 ~ 49 PASS)
	uint8_t low = last & 0xf;
	uint8_t high = (last >> 4) & 0xf;

	if(last == 0 || low > 9 || high > 4)  last = 0;
	else last = high * 10 + low; 

	if ((last > 99 || last < 1)) { // last ip 200
		last = atoi(lib->xml_att_find(h, "defaultIP"));
	}
	//if (last == 0)
	//	last = 50;

	sprintf(full_ip, "%s.%d", class_ip , last);
	printf("Read IP = %s . %d\n", class_ip, last);
	
	int i;
	int s = socket(PF_INET, SOCK_STREAM, 0);
	
	bool bFind = false;
	bool bIPChange = false;
	for (i = 1;; i++)
	{
		struct ifreq ifr;
		struct sockaddr_in* sin = (struct sockaddr_in*)&ifr.ifr_addr;
		char* ip;

		ifr.ifr_ifindex = i;
		if (ioctl(s, SIOCGIFNAME, &ifr) < 0)
			break;

		/* now ifr.ifr_name is set */
		if (ioctl(s, SIOCGIFADDR, &ifr) < 0)
			continue;

		ip = inet_ntoa(sin->sin_addr);
		printf("CurIP: %s, ReadIP: %s\n", ip, class_ip);

		if (strstr(ip, class_ip ) != NULL)
		{
			bFind = true;
			
			printf( "Find Class IP %s\n", class_ip);

			if (strcmp(ip, full_ip) != 0) {

				/// <summary>
				///  Slot ID Modify
				/// </summary>
				int nSlotNo = last % 100;  // Number position last 2 char
				char sSlotID[32];
				sprintf(sSlotID, "Slot%02d ", nSlotNo);
				FILE* f;
				if ((f = fopen(STAR_INI, "wt")) != NULL) {
					printf("Slot ID ReWirte : %s\n", sSlotID);
					printf("%s\n", sSlotID);					
					fprintf(f,"%s\n", sSlotID);
					fclose(f);
				}
				
				/// <summary>
				///  IP Modify
				/// </summary>
				printf("IP Change %s ->  %s\n", ip, full_ip);
				sprintf(last_ip, "%d", last);
//				usleep(1000 * 1000 * 5); // not changed case exist after reboot  wait 5sec
				execl("/star/test/stip", "stip", class_ip, last_ip, NULL);
				usleep(1000 * 1000 * 1);

				bIPChange = true;
			}
		}	
	}

	close(s);
	

	int ret = 0;
	if (!bFind)ret = 1;
	else if (bIPChange)ret = 2;
	printf("Find Class Flag %d, Change Flag %d RET:%d\n", bFind, bIPChange, ret);

	return ret;
}