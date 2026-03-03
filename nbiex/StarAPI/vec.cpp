#include "vec.h"
#include "ioc.h"
#include "errc.h"
#include "bib.h"
#include "starAPI.h"
#include "../starlib/starlib_dif.h"


extern errctrl	gERR;
extern ioc		gIO;
extern bib		gBIB;
extern starlib	gLIB;
extern starAPI	gAPI;

vec::vec()
{
	m_fpVec = NULL;
}

vec::~vec()
{

}

bool vec::loadFile(const char* name, int nMode)
{
	cstring path = "";
	
	if (m_fpVec != NULL) fclose(m_fpVec);
	m_fpVec = fopen(name, nMode == 0 ? "rb" : "rt");
	
	if (m_fpVec == NULL)
	{
		if (gAPI.m_nTestMode == TEST_MODE_DIAG)
			path.Format("/star/diag/%s", name);
		else
			path.Format("/star/pgm/%s", name);
		m_fpVec = fopen(path, nMode == 0 ? "rb" : "rt");
	}
	
	if (m_fpVec == NULL)
	{
		gERR.addErr(ERR_VEC, __FUNCTION__, "Can't Open File %s", name);
		m_bDone = true;
		return false;
	}

	m_nMode = nMode;

	fseek(m_fpVec, 0, SEEK_END);
	m_nFileSize = ftell(m_fpVec);
	fseek(m_fpVec, 0, SEEK_SET);
	m_nFileCur = 0;
	m_bRemind = false;
	m_nRemind = 0;

	m_bDone = false;


	return true;
}

void vec::fillBuffer(int& posH, int& posL, uint nData)
{
	word value = nData & 0xffffff;
	if (gBIB.m_bDDR == 0)
	{
		m_nBufH[posH++] = value;
		m_nBufL[posL++] = value;
	}
	else {
		if (posH == posL)m_nBufH[posH++] = value;
		else m_nBufL[posL++] = value;
	}
	
}

bool vec::setBinary(int& idxCnt)
{
	//fseek(m_fpVec, 0, SEEK_SET);
	printf("%s File Pos %d\n", __FUNCTION__, m_nFileCur);
	fseek(m_fpVec, m_nFileCur, SEEK_SET);
	int unitSize = 1;
	if (gBIB.m_nIoCnt == 16)unitSize = 2;
	int fSize = m_nFileSize;
	int curPos = m_nFileCur;
	int posH = 0, posL = 0;
	byte rBuf[2];

	while (1)
	{
		if (!(fread(rBuf, 1, unitSize, m_fpVec)))break;

		if (gBIB.m_nIoCnt == 16) {
			fillBuffer(posH, posL, rBuf[0] | (rBuf[1] << 8));
		}
		else if (gBIB.m_nIoCnt == 8)
		{
			fillBuffer(posH, posL, rBuf[0]);
		}
		else if (gBIB.m_nIoCnt == 4)
		{
			fillBuffer(posH, posL, rBuf[0] & 0xf);
			fillBuffer(posH, posL, (rBuf[0] >> 4) & 0xf);
		}
		else if (gBIB.m_nIoCnt == 1)
		{
			for (int i = 0; i < 8; i++)
				fillBuffer(posH, posL, (rBuf[0] & (1 << i)) ? 1 : 0);
		}
		curPos += unitSize;

		if (curPos >= m_nFileSize)break;
		if (posL >= VEC_MAX)
			break;
		
	}

	idxCnt = posH;
	return writeBuffer(posH);
}

int vec::setBuf(int ofs, int& idxCnt, int* pData)
{
	int posH = ofs, posL = ofs;
	int srcIdx = 0;		
	byte rBuf[2];
	
	while (1)
	{	
		int data = *(pData + srcIdx);
//		printf("--- value %d | 0x%X\n", data, data);
		if (data == -1)break;

		uint value = (uint)data;
		rBuf[0] = value & 0xff;
		rBuf[1] = (value >> 8) & 0xff;
//		printf("rbuf0 %x rbuf1 %x\n", rBuf[0], rBuf[1]);

		if (gBIB.m_nIoCnt == 16) {
			fillBuffer(posH, posL, rBuf[0] | (rBuf[1] << 8));
		}
		else if (gBIB.m_nIoCnt == 8)
		{
			fillBuffer(posH, posL, rBuf[0]);			
		}
		else if (gBIB.m_nIoCnt == 4)
		{
			fillBuffer(posH, posL, rBuf[0] & 0xf);
			fillBuffer(posH, posL, (rBuf[0] >> 4) & 0xf);
			
		}
		else if (gBIB.m_nIoCnt == 1)
		{
			for (int i = 0; i < 8; i++)
				fillBuffer(posH, posL, (rBuf[0] & (1 << i)) ? 1 : 0);			
		}

		if (posL >= VEC_MAX)
		{
			gERR.addErr(ERR_VEC, __FUNCTION__, "Vector File Size Over(Max 32KByte)");
			return -1;
		}
		srcIdx++;
	}

	idxCnt = posH;
	writeBuffer(posH);
	return srcIdx;
}

bool vec::writeBuffer(int sz)
{
	//printf("---VM H--- \n");	
	byte* pData = (byte*)m_nBufH;
	int size = sz * 2; // word -> byte size
	gIO.writePGData(PG_VMH, pData, size);

	//printf("---VM L--- \n");
	pData = (byte*)m_nBufL;
	size = sz * 2; // word -> byte size
	gIO.writePGData(PG_VML, pData, size);
	
	
	if (m_fpVec != NULL) {
		if (feof(m_fpVec)) {
			fclose(m_fpVec);
			m_fpVec = NULL;
			m_bDone = true;
			
		}
		else
			m_nFileCur = ftell(m_fpVec);
	}
	
	
	
	return m_bDone;
}

uint vec::getValue(char* p)
{
	char buf[16];
	char* sp = p;
	char* dp = buf;
	*dp++ = '0';
	*dp++ = 'x';	
	
	for (int i = 0; *sp != '\0' && i < 16; i++)
		*dp++ = *sp++;
	*dp = '\0';

	return gLIB.vtoi(buf);
}

bool vec::setText(int& idxCnt)
{	
	printf("%s File Pos %d\n", __FUNCTION__, m_nFileCur);

	const char sep[] = " \t\n";
	int posH = 0, posL = 0;

	if (m_bRemind)
	{
		for (int i = 0; i < m_nRemind; i++) {
			fillBuffer(posH, posL, m_nBufRem[i]);
		}
	}

	fseek(m_fpVec, m_nFileCur, SEEK_SET);
	m_bRemind = false; m_nRemind = 0;
	char rBuf[2048];
	while (fgets(rBuf, 2048, m_fpVec)) {
		char* sp = rBuf;
		while (*sp != '\0' && isspace(*sp))	
			sp++;

		char* p = sp;
		for (; *p; p++) {					
			if (*p == ';' || (*p == '/' && *(p + 1) == '/')) {
				*p = '\0';
				break;
			}
		}

		p = strtok(sp, sep);
		while (p != NULL) {

			while (*p != '\0' && isspace(*p))
				p++;

			if (*p != '\0') {
				uint v = getValue(p);
				if (!m_bRemind) {
					if (gBIB.m_nIoCnt >= 8)
					{
						fillBuffer(posH, posL, v);
					}
					else if (gBIB.m_nIoCnt == 4)
					{
						fillBuffer(posH, posL, v & 0xf);
						fillBuffer(posH, posL, (v >> 4) & 0xf);
					}
					else if (gBIB.m_nIoCnt == 1) 
					{
						for (int i = 0; i < 8; i++)
							fillBuffer(posH, posL, (v & (1 << i)) ? 1 : 0);
					}					
				}
				else {
					if (gBIB.m_nIoCnt >= 8)
					{
						m_nBufRem[m_nRemind++] = v;
					}
					else if (gBIB.m_nIoCnt == 4) 
					{
						m_nBufRem[m_nRemind++] = v & 0xf;
						m_nBufRem[m_nRemind++] = (v >> 4) & 0xf;
					}
					else if (gBIB.m_nIoCnt == 1) 
					{
						for (int i = 0; i < 8; i++)
							m_nBufRem[m_nRemind++] = (v & (1 << i)) ? 1 : 0;
					}
				}
			}

			if (posL >= VEC_MAX) {
				m_bRemind = true;
			}
			p = strtok(NULL, sep); // 1 line
		}
		if (posL >= VEC_MAX)break;
	}
	
	idxCnt = posL;
	return writeBuffer(posH);
}

