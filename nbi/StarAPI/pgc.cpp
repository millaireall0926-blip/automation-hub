#include "pgc.h"
#include "errc.h"
#include "ioc.h"
#include "define.h"
#include "bib.h"
#include "failmem.h"
#include "STAR_TEST.h"
#include "net.h"
#include "starAPI.h"
#include <float.h>


#define TIMER_RES (0.00000001 - DBL_EPSILON)

extern ioc		g_clIO;
extern errc		g_clErr;
extern bib		g_clBib;
extern failmem	g_clFm;
extern net		g_clNet;
extern starAPI	g_clAPI;			//	gAPI

int pds::getNum(char* p, int &num)
{
	//////////  Number Read ////////
	if (!isdigit(*p))
	{
		gERR.addErr(ERR_PDS, __FUNCTION__, "Context Error \"%s\"", p);
		return 0;
	}
	num = atoi(p);
	return 1;
	
}

int pds::getSource(cstring str, int dest, int& value, int& SCOption)
{
	//int ret = 0;
	char* p = str;
	bool bInvert = false;
	//int nInvertNum = 88;
	int nInvertNum = 96; // Z ADDR
	//if (dest & PDS_IO)nInvertNum = 74;
	if (dest & PDS_IO)nInvertNum = 82; // Z ADDR
	int num = 0;
	SCOption = 0;
	
	////////// IMPRINT MODE IO pin ////////
	if (str == "IMP") 
	{
		if (!(dest & (PDS_IO)))
		{
			gERR.addErr(ERR_PDS, __FUNCTION__, "Only IO Enable Imprint");
			return 0;
		}
		//value = 148;
		value = 164; // Z ADDR
		return 1;
	}

	/////////// Invert Check ////////////
	if (*p == '_') {
		bInvert = true;
		p++;
	}

	switch(*p) {
	/////////// FIXH or FIXL ////////////
	case 'F' :
	{
		if (dest & (PDS_STRB | PDS_DRE))
		{
			gERR.addErr(ERR_PDS, __FUNCTION__, "IO DRE Pin Can\'t Use \'FIX H/L\'");
			return 0;
		}
		//if (str == "FIXH")
		//	value = 178;
		//else if (str == "FIXL")
		//	value = 177;
		if (str == "FIXH") // Z ADDR
			value = 194; // Z ADDR
		else if (str == "FIXL") // Z ADDR
			value = 193;	 // Z ADDR
		if (dest == PDS_IO)
			value -= 28;		
		
		break;
	}

	//////////// C ///////////
	case 'C': 
	{
		if (getNum(p + 1, num) == 0)return 0;
		if (dest & PDS_IO)
		{ 
			gERR.addErr(ERR_PDS, __FUNCTION__, "IO Pin Can\'t Use \'C\'");
			return 0;
		}
		if (num > 31)
		{
			gERR.addErr(ERR_PDS, __FUNCTION__, "C\"%d\" (0~31) Enable", num);
			return 0;
		}

		value = 1;
		if (dest & (PDS_STRB | PDS_DRE))
			value = 0;
		value += num;		

		if (bInvert)
			value += nInvertNum;
		break;
	}
	//////////// S, SCn ///////////
	case 'S': // SCAN
	{
		if (!(dest & (PDS_SCAN | PDS_ADDR)))
		{
			//gERR.addErr(ERR_PDS, __FUNCTION__, "IO DRE CLK ADDR Pin Can\'t Use \'S\'");
			gERR.addErr(ERR_PDS, __FUNCTION__, "IO DRE CLK Pin Can\'t Use \'S\'");
			return 0;
		}
		if (*(p + 1) == 'C')
		{
			if (getNum(p + 2, num) == 0)return 0;
			if (num > 31)
			{
				gERR.addErr(ERR_PDS, __FUNCTION__, "SC\"%d\" (0~31) Enable", num);
				return 0;
			}
			value = 1 + num;

			if(dest & PDS_SCAN)
				SCOption = 1; // SC Flag Setting

			if(dest & PDS_ADDR)
				SCOption = 2; // ASC Flag Setting
		}
		else {
			value = 0;
		}
		
		break;
	}
	//////////// X Y D Z ///////////
	case 'X':
	{
		if (getNum(p + 1, num) == 0)return 0;
		if (num > 23)
		{
			gERR.addErr(ERR_PDS, __FUNCTION__, "SET X\"%d\" (0~23) Enable", num);
			return 0;
		}
		value = 33;
		if (dest & PDS_IO)
			value = 18;
		value += num;
		if (bInvert)value += nInvertNum;

		break;
	}
	case 'Y':
	{
		if (getNum(p + 1, num) == 0)return 0;
		if (num > 23)
		{
			gERR.addErr(ERR_PDS, __FUNCTION__, "SET Y\"%d\" (0~23) Enable", num);
			return 0;
		}
		value = 57;
		if (dest & PDS_IO)
			value = 42;
		value += num;
		if (bInvert)value += nInvertNum;

		break;
	}
	case 'D':
	{
		if (getNum(p + 1, num) == 0)return 0;
		if (num > 17)
		{
			gERR.addErr(ERR_PDS, __FUNCTION__, "SET D\"%d\" (0~17) Enable", num);
			return 0;
		}
		if (!(dest & (PDS_IO)))
		{
			gERR.addErr(ERR_PDS, __FUNCTION__, "Only IO Enable \'D\'");
			return 0;
		}
		value = 0;
		value += num;
		if (bInvert)value += nInvertNum;

		break;
	}
	case 'Z':
	{
		if (getNum(p + 1, num) == 0)return 0;
		//if (num > 7)
		if (num > 15) // Z ADDR
		{
			gERR.addErr(ERR_PDS, __FUNCTION__, "SET Z\"%d\" (0~15) Enable", num);
			return 0;
		}
		value = 81;
		if (dest & PDS_IO)
			value = 66;
		value += num;
		if (bInvert)value += nInvertNum;
		
		break;
	}
	default:
	{
		gERR.addErr(ERR_PDS, __FUNCTION__, "Context Error. %s is Unknown Keyword.", p);
		return 0;
	}
	}
	
	return 1;
}


void pds::setPDS(int dest, const char* source)
{
	m_Scanner.setup(source);
	token_t tok;
	int cnt = 0;
	int cypindex = 0;
	int setVal = 0;
	int SCOption = 0;
	int type = PDS_ADDR;
//	cstring sdebug;
	if (dest >= ADDR0 && dest <= ADDR31_B) {
		type = PDS_ADDR; 
//		sdebug.Format("ADR%d", (dest - ADDR0)/2 );
	}else if (dest >= CLK0 && dest <= CLK7_B){
		type = PDS_CLK;
//		sdebug.Format("CLK%d", (dest - CLK0) / 2);
	}else if (dest >= IO0 && dest <= IO31_B) {
		type = PDS_IO;
//		sdebug.Format("IO%d", (dest - IO0) / 2);
	}else if (dest >= SCAN0 && dest <= SCAN31_B) {
		type = PDS_SCAN; 
//		sdebug.Format("SCAN%d", (dest - SCAN0) / 2);
	}else if (dest >= STRB && dest <= STRB_B) {
		type = PDS_STRB;
//		sdebug.Format("STRB%d", (dest - STRB) / 2);
	}else if (dest == DRE) {
		type = PDS_DRE;
//		sdebug.Format("DRE%d", (dest - DRE) / 2);
	}else {
		gERR.addErr(ERR_PDS, __FUNCTION__, "DEST SET Context Error.");
	}
	
//	printf("PDS SET %s(%d) = %s\n", sdebug.c_str() ,dest,  source);
//	gIO.m_bDebug = true;
	while (1)
	{
		tok = m_Scanner.get();
		//	clerr.print("s:%s cnt:%d\n", tok.s.c_str(), cnt);
		if (tok.t == TOK_EOF) break;
		if (tok.t == TOK_EOL) continue;

		if (cnt % 2 == 0) {
			int ret = getSource(tok.s, type, setVal, SCOption);
			if (ret == 0) break;										// error

//			printf("setVal %s  %d\n", tok.s.c_str(), setVal);
			
			uint adr = PG_PDS | cypindex << 8 | dest;

			if (type == PDS_STRB)
			{ 
				if (dest % 2 == 0) gIO.writePG(PG_SYSTEM | 16, setVal, 1);	//SDR
				gIO.writePG(PG_SYSTEM | 14, setVal, 1);						//DDR
				
			}
			else if (type == PDS_DRE)
			{ 
				gIO.writePG(PG_SYSTEM | 17, setVal, 1);
			}
			else {
				
				gIO.writePG(adr, setVal, 1);			
				if (dest % 2 == 0) 
					gIO.writePG(adr + 1, setVal, 1);	

				/// <summary>
				///  SCOption
				/// </summary>
				if (SCOption == 1)
				{					
					int pos = (dest - SCAN0) / 2; //-> /2 = H,L
					m_nSCOption[cypindex] |= (1 << pos);										
				}else if (SCOption == 2)
				{
					int pos = (dest - ADDR0) / 2; //-> /2 = H,L
					m_nASCOption[cypindex] |= (1 << pos);					
				}
				else {
					if (type == PDS_SCAN) {
						int pos = (dest - SCAN0) / 2; //-> /2 = H,L
						m_nSCOption[cypindex] &= (~(1 << pos));

					}
					else if (type == PDS_ADDR)
					{
						int pos = (dest - ADDR0) / 2; //-> /2 = H,L
						m_nASCOption[cypindex] &= (~(1 << pos));
					}
				}
				/// <summary>
				///  SCOption
				/// </summary>				
			}

			cypindex++;
		}
		else
		{
			if (tok.s != ",") {
				gERR.addErr(ERR_PDS, __FUNCTION__, "\"%s\" Context Error.", source);
				break;
			}			
		}		

		cnt++;
	}
	//gIO.m_bDebug = false;
}

void pds::writeSCOption()
{	
	for (int i = 0; i < 16; i++) {
		gIO.writePG(PG_PDS | i << 8 | 208, m_nSCOption[i]);
		gIO.writePG(PG_PDS | i << 8 | 212, m_nASCOption[i]);
	}	
}

int scramble::getLogic(cstring log) {
	int val = 0;
	if (log == "^")
		val = 0x1;
	else if (log == "|")
		val = 0x2;
	else if (log == "&")
		val = 0x3;
	else if (log == "=")
		val = 0x4;

	return val;
}

int scramble::getReg(cstring reg, int& num, int sel) //sel 0 = Source, 1 = Dest
{
	char* p = reg;
	if (sel == 0) {
		if (*p != 'A')
			gERR.addErr(ERR_SCR, __FUNCTION__, "\"%s\" Source Possible AX, AY Only", reg.c_str());
		p++;
	}
	

	int max = 23;
	int ret = REG_NONE;
	if (*p == 'X') ret = REG_X;	
	else if (*p == 'Y') ret = REG_Y;
	else if (*p == 'D') {
		ret = REG_D;
		max = 17;		
		if (sel == 0) ret = REG_NONE;
	}
	p++;

	if (!isdigit(*p))ret = REG_NONE;
	num = atoi(p);
	if (num > max) ret = REG_NONE;
//	if (sel == 0) num++; // ADR 0 is No User 
	if (ret == REG_NONE)
		gERR.addErr(ERR_SCR, __FUNCTION__, "\"%s\" %s Set Error", reg.c_str(), (sel == 0)?"Source":"Dest");

	return ret;
}


int scramble::setDest()
{
	token_t tok = m_Scanner.get();
	if (tok.t != TOK_ID) {
		gERR.addErr(ERR_SCR, __FUNCTION__, "\"%s\" input x, y, d", tok.s.c_str());
		return 0;
	}

	int num;
	int type = getReg(tok.s, num, 1);
	if (type == REG_NONE) return 0;				
	m_nDestAdr = PG_SCR | (type << 14) | (num << 4);

	tok = m_Scanner.get();
	if (tok.s != "=") {
		gERR.addErr(ERR_SCR, __FUNCTION__, "missing \'=\' ");
		return 0;
	}
	return 1;

}

void scramble::setScr(const char* foramt)
{
	//printf("set : %s\n", foramt);
	m_Scanner.setup(foramt);
	if (setDest() == 0)return;

	int num;
	int type;

	int cnt = 0;
	//bool bErr = false;
	bool bInvert = false;

	token_t tok;

	while (1)
	{
		tok = m_Scanner.get();
//		printf("s:%s cnt:%d\n", tok.s.c_str(), cnt);
		if (tok.t == TOK_EOF) break;
		if (tok.t == TOK_EOL) continue;

		if (cnt == 0) {
			if (tok.s == "!") {
				bInvert = 1;
				continue;
			}
		}

		if (cnt % 2 == 0) {
			if (tok.t != TOK_ID) {
				gERR.addErr(ERR_SCR, __FUNCTION__, "\"%s\" context error", tok.s.c_str());
				break;
			}
			type = getReg(tok.s, num);
			if (type == REG_NONE) break;
			if (type == REG_Y) num += 24;
			m_nSrcArr[cnt / 2] = num;
		}
		else {
			if (tok.t != TOK_PUNC) {
				gERR.addErr(ERR_SCR, __FUNCTION__, "\"%s\"(%d) context error", tok.s.c_str(), cnt);
				break;
			}
			num = getLogic(tok.s);  // logic number  = ^ | &
			if (num == 0) {
				gERR.addErr(ERR_SCR, __FUNCTION__, "\"%s\"(%d) context error", tok.s.c_str(), cnt);
				break;
			}
			m_nLogArr[cnt / 2] = num;
		}
		cnt++;

		if (cnt > 7) {
			gERR.addErr(ERR_SCR, __FUNCTION__, "\"%s\"(%d) too many word", tok.s.c_str(), cnt);
			break;
		}
	}

	if (cnt > 0 && (gERR.getErrCnt() == 0))
	{
		if (cnt % 2 != 0)
			writeScr(bInvert, cnt);
		else
			gERR.addErr(ERR_SCR, __FUNCTION__, "argument, logic not matched");
	}
}

void scramble::writeScr(bool invert, int cnt)
{
	uint adr = m_nDestAdr;
	uint a = 0, b = 0, c = 0, d = 0, log = 0;
	if (cnt == 1) {
		a = m_nSrcArr[0];
		log = 0;
	}
	else if (cnt == 3) {
		a = m_nSrcArr[0];
		b = m_nSrcArr[1];
		log = 3;
		log += m_nLogArr[0];
	}
	else  if (cnt == 5)
	{
		a = m_nSrcArr[0];
		b = m_nSrcArr[1];
		c = m_nSrcArr[2];
		log = 6;
		log += m_nLogArr[0] + ((m_nLogArr[1] - 1) * 3);
	}
	else  if (cnt == 7)
	{
		a = m_nSrcArr[0];
		b = m_nSrcArr[1];
		c = m_nSrcArr[2];
		d = m_nSrcArr[3];
		log = 15;
		log += m_nLogArr[0] + ((m_nLogArr[1] - 1) * 3) + ((m_nLogArr[2] - 1) * 9);
	}
//	printf("SCR_adr %x :inv=%d log=%d a=%d b=%d c=%d d=%d \n", adr, invert, log, a, b, c, d);
	uint val = (invert << 31) | ((log & 0x3f) << 24) | ((a & 0x3f) << 18)
		| ((b & 0x3f) << 12) | ((c & 0x3f) << 6) | (d & 0x3f);
//	printf("adr:0x%08x val:0x%08x\n\n", adr, val);
	gIO.writePG(adr, val);
}

void scramble::enableScr(bool enable, int type)
{
	uint val = 0x11;
	if (type == SCR_DATA)
		val = 0x1;
	else if (type == SCR_ADR)
		val = 0x10;	
	
	uint wr = gIO.readPG(PG_SYSTEM | 5, 1);
	if (enable)
		wr |= val;
	else
		wr &= (~val & 0xff);
	gIO.writePG(PG_SYSTEM | 5, wr, 1);

}

void pgc::writeReg(registerset_test_t* pReg)
{
	if (gIO.m_bSim)return;
	//uint starAdr;
	byte* p = (byte*)pReg;
	//starAdr = PG_INSTRUCTION;
	//int wsz;

	for (uint inc = 0; inc < 238; inc+=2)
	{
		writePG(inc, *((uint*)p), 2);
		p += 2;
	}

	//for (uint inc = 0; inc <= 236; )
	//{
	//	uint adr = starAdr + inc;
	//	
//
	//	if (inc == 54 || inc == 73 || inc == 185 ||
	//		inc == 200 || inc == 216 || inc == 217 || inc == 218 ||
	//		inc == 219 || inc == 220 || inc == 227 || inc == 228 || inc == 229)
	//	{			
	//		wsz = 1;
	//		writePG(adr, *((uint*)p), wsz);
//	//		printf("%s %08x(%d):%02x(%d)\n", __FUNCTION__, adr, inc, *p, wsz);
	//		inc++;
	//		p++;
	//		continue;
	//	}
	//	else if (inc == 170 || inc == 178 || inc == 183 || inc == 192 ||
	//			 inc == 230 || inc == 232 || inc == 234 || inc == 236 )
	//	{
	//		wsz = 2;
	//		writePG(adr, *((uint*)p), 2);
//	//		printf("%s %08x(%d):%04x(%d)\n", __FUNCTION__, adr, inc, *((word*)p), wsz);
	//		inc +=2;
	//		p += 2;
	//		continue;
	//	}
	//	else
	//	{
	//		wsz = 3;
	//		writePG(adr, *((uint*)p), 3);
//	//		if (adr != 186) // Scan Register. Skip!
//	//		{
//	//			writePG(adr, *((uint*)p), 3);
//	//		}
//	//		else {
//	//			printf("skip scan Reg\n");
//	//		}
//	//		printf("%s %08x(%d):%06x(%d)\n", __FUNCTION__, adr, inc, *((uint*)p) & 0xffffff, wsz);
	//		inc += 3;
	//		p += 3;
	//	}
//
	//}

//	for (int i = 0; i < 16; i++)
//	{		
//		adr += i * 3;
//		memcpy(&val, &pReg->idx[i], 3);
//		writePG(adr, val, 3);		
//		printf("%s IDX:	%08x:%08x\n", __FUNCTION__, adr, val);
//	}
//
//	memcpy(&val, &pReg->xmax, 3);
//	writePG(PG_INSTRUCTION | 48, val, 3);


}


void pgc::writePattern(const char* file)
{

	uint fSize = 0;
	m_nWritePC = 0;  // prev writen pc counter. for multi upload.. not use now
	FILE* pFile = NULL;

	char fn[256];
	if (strstr(file, ".mpa") == NULL)
		sprintf(fn, "%s.mpa", file);
	else
		sprintf(fn, "%s", file);

	pFile = fopen(fn, "rb");

	cstring staticPath = "";
	if (pFile == NULL) {
		if (gAPI.m_nTestMode == TEST_MODE_DIAG)
			staticPath.Format("/star/diag/%s", fn);
		else
			staticPath.Format("/star/pgm/%s", fn);
		pFile = fopen(staticPath.c_str(), "rb");
	}
	
	if (pFile == NULL) {
		gERR.addErr(ERR_PG, __FUNCTION__, "Pattern File Open Error %s\n", fn);
		return;
	}
	
	fseek(pFile, 0, SEEK_END);
	fSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

//	printf("%s fsize %d \n", file, fSize);
	
	byte* pBuf = new byte[fSize + 10];
	byte* rbuf = pBuf;
//	printf("rbuf  %x\n", fSize);

	memset(rbuf, 0x0, fSize + 10);
	fread(rbuf, fSize, 1, pFile);
	
	fileheader_t header;
	memcpy(&header, rbuf, sizeof(fileheader_t));

	rbuf += sizeof(fileheader_t);
	uint nPC = header.pat_size;
//	printf("pattern \"%s\" Size %d(%dbyte), pos %x(%d) sz %d \n", file, nPC / MAX_MC_BYTE, nPC, rbuf - pBuf, rbuf - pBuf, sizeof(fileheader_t));
//	printf("pcnt %d(%d), buf %x -> %x sz %d \n", nPC / MAX_MC_BYTE, nPC, pdpg, rbuf, sizeof(fileheader_t));
	nPC /= MAX_MC_BYTE;
	if (nPC > PATTERN_MAX) {
		gERR.addErr(ERR_PG, __FUNCTION__, "Pattern Over %d. Pattern Depth %d", nPC, PATTERN_MAX);
		goto OutWirtePattern;
	}
	
	registerset_test_t reg;
	memcpy(&reg, rbuf, sizeof(registerset_test_t));
	writeReg(&reg);
	rbuf += sizeof(registerset_test_t);

//	printf("regread pos %x(%d) sz %d \n", rbuf - pBuf, rbuf - pBuf, sizeof(registerset_test_t));
	m_sPatternName.Format("%s", file);

	for (uint i = 0; i < nPC; i++) {
		uint adr = (PG_PAT | (i * MAX_MC_BYTE)) + (m_nWritePC * MAX_MC_BYTE); // Multi pattern Write Addr
//		printf("adr %08x : rbuf %x\n", adr, rbuf);
		gIO.writePGData(adr, rbuf, MAX_MC_BYTE);		
		rbuf += MAX_MC_BYTE;
//		printf("pat write %d : pos %x(%d) sz %d\n", i, rbuf - pBuf, rbuf - pBuf, MAX_MC_BYTE);

	}


	uint funcCnt;
	memcpy(&funcCnt, rbuf, sizeof(uint));	
	rbuf += sizeof(uint);	
//	printf("funcCnt %d, pos %x(%d) sz %d\n", funcCnt, rbuf - pBuf, rbuf - pBuf, sizeof(uint));
	
	for (uint i = 0; i < funcCnt; i++) {
		// Func type, pc 
		
		word read;
		memcpy(&read, rbuf, sizeof(word));
		rbuf += sizeof(word);
		byte type = read >> 14;
		word pc = read & 0x3fff;
		pc += m_nWritePC;
		
		// Func Name size
		word nameSz;
		memcpy(&nameSz, rbuf, sizeof(word));
		if (nameSz < 0) {
			gERR.addErr(ERR_PG, __FUNCTION__, "Pattern Function Symbol Loading Fial");
			goto OutWirtePattern;
		}
		rbuf += sizeof(word);
		
		
		// Func Name		
		char* name = new char[nameSz + 10];
		memcpy(name, rbuf, nameSz);
		name[nameSz] = '\0';
		rbuf += nameSz;
		
		
		cstring funcName = name;
		m_mapLabelPC[funcName] = pc;
		if (type == 0)m_nDefualtPC = pc;
//		printf("Function[%s] = %d, type %s\n", funcName.c_str(), pc, (type == 0) ? "start" : "func");


		delete[] name;

	}


	uint labelCnt;
	memcpy(&labelCnt, rbuf, sizeof(uint));	
//	printf("labelCnt %d, pos %x(%d) sz %d\n", labelCnt, rbuf - pBuf, rbuf - pBuf, sizeof(uint));
	rbuf += sizeof(uint);
	for (uint i = 0; i < labelCnt; i++) {
		// label PC
		word pc;
		memcpy(&pc, rbuf, sizeof(word));
		rbuf += sizeof(word);
		 
		// label Name Size
		word nameSz;
		memcpy(&nameSz, rbuf, sizeof(word));
		rbuf += sizeof(word);
		if (nameSz < 0) {
			gERR.addErr(ERR_PG, __FUNCTION__, "Pattern Function Symbol Loading Fial");
			goto OutWirtePattern;
		}
		// Label Name
		char* name = new char[nameSz + 10];
		memcpy(name, rbuf, nameSz);
		name[nameSz] = 0;
		rbuf += nameSz;

		cstring labelName;
		labelName.Format("%s", name);
		m_mapLabelPC[labelName] = pc;
		//printf("Label[%s] = %d\n", labelName.c_str(), pc);

		delete[] name;
	}
	
	m_nWritePC += nPC;
	if (m_nWritePC > PATTERN_MAX)gERR.addErr(ERR_PG, __FUNCTION__, "Pattern Over %d. Pattern Depth %d", m_nWritePC, PATTERN_MAX);

OutWirtePattern:
	if (pFile != NULL)
		fclose(pFile);
	delete[] pBuf;
}

void pgc::runPG()
{	
	uint adr = PG_SYSTEM | 3;
	gIO.writePG(adr, 1, 1);	
	gAPI.debug("		%s", __FUNCTION__);

	//printf("RUN : isRun %d\n", isRun());

}

void pgc::stopPG()
{	
	uint adr = PG_SYSTEM | 3;
	gIO.writePG(adr, 2, 1);
	gAPI.debug("		%s", __FUNCTION__);
	//printf("STOP : isRun %d\n", isRun());
}

bool pgc::isRun()
{
	bool bRun = false;
	if (gIO.readPG(PG_SYSTEM | 0x146, 1) & 0x10)bRun = true;
	return bRun;
}

bool pgc::isSync()
{
	bool bSyn = false;	
	if (gIO.readPG(PG_SYSTEM | 0x146, 1) & 1)bSyn = true;
	return bSyn;
}

uint pgc::getSyncID()
{
	return gIO.readPG(PG_SYSTEM | 0x147, 1);
}

void pgc::nextSync()
{
	gIO.writePG( PG_SYSTEM | 15, 1, 1);
}

void pgc::writeScan(int idx, unsigned int val)
{
	unsigned int  adr = PG_SCAN	| (idx << 2);
	gIO.writePG(adr, val);
	gBIB.getScan(idx)->m_nScanValue = val;
}

void pgc::writeAScan(int idx, unsigned int val)
{
	unsigned int  adr = PG_SCAN | (1 << 10) | (idx << 2);
	gIO.writePG(adr, val);	
}


uint pgc::readScan(int idx)
{	
	unsigned int  adr = PG_SCAN | (idx << 2);
	return gIO.readPG(adr);
}

void pgc::setPC(const char* spc)
{
	int pc = m_nDefualtPC;
	m_sRunPC.Format("%s", spc);

	if (gNET.m_nClientType == 0) {
		cstring sMsg;
		sMsg.Format("MPAT %s - %s", m_sRunPC.c_str(), m_sPatternName.c_str());
		gNET.send_msg(MSG_TYPE_INFO, sMsg.c_str());
	}
	
	if (strlen(spc) == 0)
	{
		pc = m_nDefualtPC;
	}
	else if (isdigit(*spc))
	{
		pc = atoi(spc);		
	}
	else
	{
		//cstring str;
		auto iter = m_mapLabelPC.find(spc);
		if (iter == m_mapLabelPC.end())
		{
			gERR.addErr(ERR_PG, __FUNCTION__, "Not Fount PC %s", spc);
			return;
		}
		pc = m_mapLabelPC[spc];		
		
	}	
//	printf("RUN %s %d\n", spc, pc);

	setPC(pc);

}

void pgc::setPC(int npc)
{
	gIO.writePG(PG_SYSTEM + 1, npc, 2);
}


void pgc::setJnc(unsigned int jnc)
{
	gIO.writePG(PG_INSTRUCTION | 183, jnc & 0xffff, 2);
}

typedef struct st_regadr {
	uint adr;
	int	 sz;
}REG_ADR;

REG_ADR _setRegMap[] =
{
	{0	 ,3},{3	  ,3},{6   ,3},{9   ,3},{12	 ,3},{15  ,3},{18  ,3},{21  ,3},{24	 ,3},{27  ,3},
	{30	 ,3},{33  ,3},{36  ,3},{39  ,3},{42	 ,3},{45  ,3},{48  ,3},{51  ,3},{54	 ,1},
	{55	 ,3},{58  ,3},{61  ,3},{64  ,3},{67	 ,3},{70  ,3},{73  ,1},{74  ,3},{77	 ,3},
	{80	 ,3},{83  ,3},{86  ,3},{89  ,3},{92	 ,3},{95  ,3},{98  ,3},{101 ,3},
	{104 ,3},{107 ,3},{110 ,3},{113 ,3},{116 ,3},{119 ,3},{122 ,3},{125 ,3},{128 ,3},{131 ,3},{134 ,3},
	{137 ,3},{140 ,3},{143 ,3},{146 ,3},{149 ,3},{152 ,3},{155 ,3},{158 ,3},{161 ,3},{164 ,3},{167 ,3},
	{170 ,2},{172 ,3},{175 ,3},{178 ,2},{180 ,3},{183 ,2},{185 ,1},{186 ,3},{189 ,3},{192 ,2},{194 ,3},
	{197 ,3},{200 ,1},{201 ,3},{204 ,3},{207 ,3},{210 ,3},{213 ,3},{216 ,1},{217 ,1},{218 ,1},{219 ,1},
	{220 ,1},{221 ,3},{224 ,3},{227 ,1},{228 ,1},{229 ,1},{230 ,2},{232 ,2},{234 ,2},{236 ,2},
};

void pgc::setReg(int idx, uint value)
{
	if (idx > 90)
		gERR.addErr(ERR_PG, __FUNCTION__, "Register Setting [%d]Index Error. SetValue [0x%x]", idx, value);
	
	if (idx == JET_TIMER || idx == REF_TIMER)
	{
		gERR.addErr(ERR_PG, __FUNCTION__, "[JET_TIMER] [REF_TIMER] Setting Use SET_TIMER Function");
		return;
	}

	uint adr = _setRegMap[idx].adr;
	int size = _setRegMap[idx].sz;

	if (idx <= IDX15)
		value += 1;	

	gIO.writePG(PG_INSTRUCTION | adr, value  , size); 
}


void pgc::setTimer(int sel, double dTime)
{
	if (!(sel == JET_TIMER || sel == REF_TIMER))
	{
		gERR.addErr(ERR_PG, __FUNCTION__, "SET_TIMER Enable Only [JET_TIMER] [REF_TIMER]");
		return;
	}

	int cTime = 0x0;
	uint iOperand = 0;
	if ((dTime < 300.0 * 1.0e-9) || (dTime > 100.0 * 1.0e-0))
	{
		gERR.addErr(ERR_PG, __FUNCTION__, "TIMER: time value should be between 300 NS ~ 100 S, but use %f", dTime);
		return;
	}
	int T_BIT = 22;
	if (dTime <= (41.0 * 1.0e-3)) // ~ 41 MS : NS
	{
		// dTime -= (1.0e-9 * 100.0);
		//  cTime = static_cast<int>(dTime / ((10.0 - DBL_EPSILON) * 1.0e-9));
		cTime = static_cast<int>(dTime / TIMER_RES );
		iOperand = (0 << T_BIT) | (cTime & 0x3FFFFF);
	}
	else if (dTime <= (41.0 * 1.0e-0)) //~ 41 S : US
	{
		// dTime *= 0.976;
		cTime = static_cast<int>(dTime / (TIMER_RES * 1000));
		iOperand = (1 << T_BIT) | (cTime & 0x3FFFFF);
	}
	else if (dTime <= (41000.0)) //~ 41000 S : MS
	{
		cTime = static_cast<int>(dTime / (TIMER_RES * 1000000));
		iOperand = (2 << T_BIT) | (cTime & 0x3FFFFF);
	}
	else //  41S ~
	{
		cTime = static_cast<int>(dTime / (TIMER_RES * 1000000000));
		iOperand = (3 << T_BIT) | (cTime & 0x3FFFFF);
	}

	if (cTime > 0x3FFFFF)
	{
		gERR.addErr(ERR_PG, __FUNCTION__, "TIMER/PAUSE generated value '%f (0x%X) is bigger than 22bit(0x3FFFFF)", dTime, cTime);
		return;
	}

	uint adr = _setRegMap[sel].adr;
	int size = _setRegMap[sel].sz;

	gIO.writePG(PG_INSTRUCTION | adr, iOperand, size);
}

void pgc::writeIOMask(uint mask0_31, uint mask32_63)
{
	gIO.writePG(PG_SYSTEM | 40, mask0_31);
	gIO.writePG(PG_SYSTEM | 44, mask32_63);
}
