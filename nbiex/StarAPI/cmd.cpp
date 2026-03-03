#include "cmd.h"
#include "define.h"
#include "ioc.h"
#include <string.h>
#include <stdio.h>

extern command g_clCmd;
extern ioc	g_clIO;

int cmd_o(int argc, char** argv)
{
	int argNo = 2;
	if (argc -1 != argNo) {
		printf("Argument Error\n");
		return 0;
	}

	uint* nArgs = new uint(argNo);
	for (int i = 0; i < argNo; i++)
	{
		char* p, * q;
		p = argv[i+1];
		q = p + strlen(p);
		nArgs[i] = strtoul(p, &q, 16);
	}
	gIO.writePG(nArgs[0] , nArgs[1], 1);
	printf("[o] : %06x -> %02x\n", nArgs[0] , nArgs[1]);
	delete[] nArgs;
	return 1;
}

int cmd_i(int argc, char** argv)
{
	int argNo = 1;
	if (argc - 1 != argNo) {
		printf("Argument Error\n");
		return 0;
	}
	char* p, * q;
	p = argv[1];
	q = p + strlen(p);
	int adr = strtoul(p, &q, 16);
	
	uint rd = gIO.readPG(adr , 1);
	printf("[i] : %06x <- %02x\n", adr , rd);
	
	return 1;
	
}

int cmd_ia(int argc, char** argv)
{
	int argNo = 2; // adr, size
	if (argc - 1 != argNo) {
		printf("Argument Error\n");
		return 0;
	}

	uint* nArgs = new uint(argNo);
	for (int i = 0; i < argNo; i++)
	{
		char* p, * q;
		p = argv[i + 1];
		q = p + strlen(p);

		int mode = 16;
		if (i == 1)
			mode = 10;
		nArgs[i] = strtoul(p, &q, mode);
	}

	byte* pData = new byte(nArgs[1]);
	gIO.readPGData(nArgs[0] , pData, nArgs[1]);
	printf("[ia] adr 0x%06x size %d", nArgs[0], nArgs[1]);
	for (int i = 0; i < (int)nArgs[1]; i++)
	{
		if (i % 16 == 0)
			printf("\n%06x : ", nArgs[0]  + i);
		printf("%02x ", *(pData+ i));
	}
	printf("\n");

	delete[] nArgs;
	delete[] pData;
	return 1;
}

int cmd_oa(int argc, char** argv)
{
	if (argc < 3) {
		printf("Argument Error\n");
		return 0;
	}

	char* p, * q;
	p = argv[1];
	q = p + strlen(p);

	uint adr = strtoul(p, &q, 16);
	int size = argc - 2;

	byte* pData = new byte(size);
	
	for (int i = 1, j = 0; j < size; i++, j++)
	{
		char* p, * q;
		p = argv[i + 1];
		q = p + strlen(p);
		*(pData + j) = strtoul(p, &q, 16);
	}
	gIO.writePGData(adr, pData, size);
	printf("[oa] adr 0x%06x size %d\n", adr, size);
		
	delete[] pData;
	return 1;
} 

int cmd_O(int argc, char** argv)
{
	int argNo = 3;
	if (argc - 1 != argNo) {
		printf("Argument Error\n");
		return 0;
	}

	uint* nArgs = new uint(argNo);
	for (int i = 0; i < argNo; i++)
	{
		char* p, * q;
		p = argv[i + 1];
		q = p + strlen(p);
		nArgs[i] = strtoul(p, &q, 16);
	}
	gIO.writePGI(nArgs[0], nArgs[1], nArgs[2], 1);
	printf("[O]P%01x : %06x -> %02x\n",nArgs[0], nArgs[1], nArgs[2]);
	delete[] nArgs;
	return 1;
}

int cmd_I(int argc, char** argv)
{
	int argNo = 2;
	if (argc - 1 != argNo) {
		printf("Argument Error\n");
		return 0;
	}

	uint* nArgs = new uint(argNo);
	for (int i = 0; i < argNo; i++)
	{
		char* p, * q;
		p = argv[i + 1];
		q = p + strlen(p);
		nArgs[i] = strtoul(p, &q, 16);
	}

	uint rd = gIO.readPGI(nArgs[0], nArgs[1], 1);
	printf("[I]p%01x : %06x <- %02x\n", nArgs[0], nArgs[1], rd);
	delete[] nArgs;
	return 1;
}

int cmd_IA(int argc, char** argv)
{
	int argNo = 3; // adr, size
	if (argc - 1 != argNo) {
		printf("Argument Error\n");
		return 0;
	}

	uint* nArgs = new uint(argNo);
	for (int i = 0; i < argNo; i++)
	{
		char* p, * q;
		p = argv[i + 1];
		q = p + strlen(p);

		int mode = 16;
		if (i == 1)
			mode = 10;
		nArgs[i] = strtoul(p, &q, mode);
	}

	byte* pData = new byte(nArgs[1]);
	gIO.readPGDataI(nArgs[0], nArgs[1], pData, nArgs[2]);
	printf("[IA]P%01x : adr 0x%06x size %d", nArgs[0], nArgs[1], nArgs[2]);
	for (int i = 0; i < (int)nArgs[1]; i++)
	{
		if (i % 16 == 0)
			printf("\n%06x : ", nArgs[0] + i);
		printf("%02x ", *(pData + i));
	}
	printf("\n");

	delete[] nArgs;
	delete[] pData;
	return 1;
}

int cmd_OA(int argc, char** argv)
{
	if (argc < 4) {
		printf("Argument Error\n");
		return 0;
	}

	char* p, * q;
	p = argv[1];
	q = p + strlen(p);
	uint pgi = strtoul(p, &q, 16);

	p = argv[2];
	q = p + strlen(p);
	uint adr = strtoul(p, &q, 16);
	int size = argc - 3;

	byte* pData = new byte(size);
	for (int i = 1, j = 0; j < size; i++, j++)
	{
		char* p, * q;
		p = argv[i + 1];
		q = p + strlen(p);
		*(pData + j) = strtoul(p, &q, 16);
	}
	gIO.writePGDataI(pgi, adr, pData, size);
	printf("[OS]P%01x: adr 0x%06x size %d\n", pgi, adr, size);

	delete[] pData;
	return 1;
}




int cmd_help(int argc, char** argv)
{
	printf("----- CMD LIST -----\n");
	for (int i = 0; i < gCMD.m_nCmdCnt; i++)
		printf("\"%s\"	: %s\n", gCMD.m_cmdTables[i].name, gCMD.m_cmdTables[i].usage);
	printf("\n----- H/W Start ADR LIST -----\n");
	printf("PG_INSTRUCT		: 0x%06x\n", 	PG_INSTRUCTION );
	printf("PG_SYSTEM		: 0x%06x\n", 	PG_SYSTEM );
	printf("PG_PAT			: 0x%06x\n", 	PG_PAT );
	printf("PG_PDS			: 0x%06x\n", 	PG_PDS );
	printf("PG_FORMAT		: 0x%06x\n", 	PG_FORMAT );
	printf("PG_SCR			: 0x%06x\n", 	PG_SCR );
	printf("PG_SCAN			: 0x%06x\n", 	PG_SCAN );
	printf("PG_FMH			: 0x%06x\n", 	PG_FMH );
	printf("PG_FML			: 0x%06x\n", 	PG_FML );
	printf("PG_TG_REG		: 0x%06x\n", 	PG_TG_REG );
	printf("PG_TG_MEM		: 0x%06x\n", 	PG_TG_MEM );
	printf("PG_VMH			: 0x%06x\n", 	PG_VMH );
	printf("PG_VML			: 0x%06x\n",	PG_VML);
	printf("-----------------------------\n\n");
	
	return 1;
}
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
int memFD;
void* pMapping;
int cmd_map(int argc, char** argv)
{
	
	//size_t size = 1024;
	memFD = open("/dev/mem", O_RDWR);
	pMapping = mmap(0, 0x40000000, PROT_READ | PROT_WRITE, MAP_SHARED , memFD, 0xc0000000);
	printf("map %x\n", pMapping);
	return 1;
}

int cmd_umap(int argc, char** argv)
{
	munmap(pMapping, 0x20000000);
	close(memFD);
	return 1;
}

command::command()
{
	m_nCmdCnt = 0;
	m_cmdTables[m_nCmdCnt++] = CmdTable{ "o", 4, cmd_o, "Write			ARG(ADR, Val)" };
	m_cmdTables[m_nCmdCnt++] = CmdTable{ "i", 4, cmd_i, "Read			ARG(ADR)" };
	m_cmdTables[m_nCmdCnt++] = CmdTable{ "oa", 4, cmd_oa, "Write Arr		ARG(ADR, Val1, Val2, ...)" };
	m_cmdTables[m_nCmdCnt++] = CmdTable{ "ia", 4, cmd_ia, "Read Arr		ARG(ADR, size)" };	
	
	m_cmdTables[m_nCmdCnt++] = CmdTable{ "O", 4, cmd_o, "[BIB FPGA] Write	ARG(PGSel[0~3, f], ADR, Val)" };
	m_cmdTables[m_nCmdCnt++] = CmdTable{ "I", 4, cmd_i, "[BIB FPGA] Read	ARG(PGSel[0~3], ADR)" };
	m_cmdTables[m_nCmdCnt++] = CmdTable{ "OA", 4, cmd_oa, "[BIB FPGA] Write Arr	ARG(PGSel[0~3, f], ADR, Val1, Val2, ...)" };
	m_cmdTables[m_nCmdCnt++] = CmdTable{ "IA", 4, cmd_ia, "[BIB FPGA] Read Arr	ARG(PGSel[0~3], ADR, size)" };

	m_cmdTables[m_nCmdCnt++] = CmdTable{ "IA", 4, cmd_ia, "[BIB FPGA] Read Arr	ARG(PGSel[0~3], ADR, size)" };

	m_cmdTables[m_nCmdCnt++] = CmdTable{ "map", 1, cmd_map, "mmap" };
	m_cmdTables[m_nCmdCnt++] = CmdTable{ "umap", 1, cmd_umap, "unmap" };

	m_cmdTables[m_nCmdCnt++] = CmdTable{ "?", 1, cmd_help, "Command Usage / H/W ADR" };
	
	

	
}

uint16_t command::str_splitter(char* buf, char** argv) {
	unsigned short argc = 0;
	char* split;

	argc = 0;
	split = strtok(buf, " ");
	argv[argc++] = split;

	while (split != NULL ) {
		split = strtok(NULL, " ");
		argv[argc++] = split;
	}
	argc--;

	return argc;
}



void command::cmd()
{
//	gIO.m_bDebug = true;
	cmd_help(1, nullptr);
	int quit = 0;
	char cmdBuf[1024];	
	char* cmdArgv[256] = { 0 };
	while (quit == 0)
	{
		printf("cmd > ");
		fgets(cmdBuf, sizeof(cmdBuf), stdin);
		
		if (cmdBuf[0] == 0xa) continue;
		cmdBuf[strlen(cmdBuf) - 1] = '\0';
		
		if (strcmp(cmdBuf, "q") == 0)break;

		int argc = str_splitter(cmdBuf, cmdArgv);
		
		char* cmd = cmdArgv[0];
		bool bFind = false;
		for (int i = 0; i < m_nCmdCnt; i++)
		{			
			if (strcmp(m_cmdTables[i].name, cmd) == 0) {
				bFind = true;				
				m_cmdTables[i].func(argc, cmdArgv);				
				break;
			}
		}
		if (!bFind)
			printf("\'%s\'Invalid CMD\n", cmd);
	}
}


