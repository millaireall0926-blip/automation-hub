#pragma once
#include <stdint.h>


int cmd_o(int argc, char** argv);
int cmd_i(int argc, char** argv);
int cmd_ia(int argc, char** argv);
int cmd_oa(int argc, char** argv);
int cmd_O(int argc, char** argv);
int cmd_I(int argc, char** argv);
int cmd_IA(int argc, char** argv);
int cmd_OA(int argc, char** argv);
int cmd_help(int argc, char** argv);

typedef struct cmd_table {
	char* name;	
	int	  argc;	
	int(*func)(int argc, char** argv); /* Implementation function	*/
	char* usage;		/* Usage message	(short)	*/
}CmdTable;

class command
{
public:
	command(void);
	~command(void) {}

	CmdTable m_cmdTables[16];

	uint16_t str_splitter(char* buf, char** argv);
	void cmd();

	int m_nCmdCnt;

};

