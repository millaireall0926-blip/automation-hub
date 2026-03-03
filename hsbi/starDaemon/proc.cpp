#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define _chdir chdir

#include "compat.h"
#include "proc.h"


extern starlib* lib;
#define stricmp strcasecmp


int prgctrl::remove(char* id)
{
	printf( "remove is not implemented yet\n");
	return 1;
}

int prgctrl::add(const char* id, const char* dir,const char* exec)
{
	if (max_prg >= MAX_APPS) {
		printf( "too many program to monitor\n");
		return 0;
	}

	int idx = max_prg;
	strcpy(pdb[idx].id, id);
	strcpy(pdb[idx].dir, dir);
	strcpy(pdb[idx].cmd, exec);
	SHMDEF* pDef = (SHMDEF * )lib->shm_get();
	pdb[idx].pShmItem = &pDef->stList[idx];
	pdb[idx].shmcnt = pDef->stList[idx].nCount;
	pdb[idx].lastcheck = time(NULL);
	pdb[idx].killed = 0;
	pdb[idx].forceKill_Cnt = 0;
	printf( "monitoring(%d) %d:%s \n",idx,  max_prg, pdb[idx].id);
	max_prg++;
	return 1;
}

void prgctrl::resetTime()
{
	for (int i = 0; i < max_prg; i++) {
		pdb[i].lastcheck = time(NULL);
	}
}

int prgctrl::check()
{
	time_t ct;
	int i, x;


	ct = time(NULL);
	for (i = 0; i < max_prg; i++) {

		if (ct < pdb[i].lastcheck + 2) 
				continue;		
		pdb[i].lastcheck = time(NULL);

		SHMDEF* pShm = (SHMDEF * )lib->shm_get();

		x = pShm->stList[i].nCount;
		if (pdb[i].forceKill_Cnt > 0)
		{
			printf("%s : %s ForceKill %d, pid %d\n", __FUNCTION__, pdb[i].id
				, pdb[i].forceKill_Cnt, pdb[i].pid);
			pdb[i].forceKill_Cnt--;
			pdb[i].shmcnt = x;
			continue;
		}

		if (x != pdb[i].shmcnt) {				
			pdb[i].shmcnt = x;
			pdb[i].pid = pShm->stList[i].nPID;			
			continue;
		} else {			
			pdb[i].pid = 0;
			pShm->stList[i].nPID = 0;
			continue;			
		}

		if (pdb[i].killed) {
			procrun( pdb[i].cmd);
			pdb[i].restart_cnt++;
			pdb[i].killed = 0;
		}
		else {
			pdb[i].pid = pShm->stList[i].nPID;			
			if (pdb[i].pid) {
				prockill(pdb[i].pid);
				pdb[i].dead_cnt++;	
			}
			pdb[i].killed++;		
		}
	}

	return 1;
}

prgctrl::program_t* prgctrl::find(const char* id) 
{
	for (int i = 0; i < max_prg; i++) {
		if (stricmp(pdb[i].id, id) == 0)
			return &pdb[i];
	}
	return NULL;
}

int prgctrl::isalive(const char* id)
{
	prgctrl::program_t* p = find(id);
	if (p == NULL) {
		return 0;
	} 
	//printf("%s : %s pid %d\n", __FUNCTION__, id, p->pid, p->shmcnt);
	return (p->pid != 0);
}

int prgctrl::pexec(char* id, char* exec)
{
	if (isalive(id)){
		printf("%s : %s Alive, Cant Excute\n", __FUNCTION__, id);
		return 0;
	}

	prgctrl::program_t* p = find(id);
	if (p == NULL) {
		printf( "exec:process '%s' is not found.\n", id);
		return 0;
	}

	p->forceKill_Cnt = 0;	
	sprintf(p->cmd, "%s", exec);

	printf("id %s, exec %s\n", p->id, p->cmd);

	return procrun( p->cmd);
}

int prgctrl::pkill(char* id)
{
	int nAlive = isalive(id);

	prgctrl::program_t* p = find(id);
	p->forceKill_Cnt = 5;

	if (!nAlive) {
		printf( "kill: process %s is not alive\n", id);
		return 1;
	}
	
	if (p == NULL) {
		printf( "kill: process '%s' not found\n", id);
		return 0;
	}
	
	if (prockill(p->pid)) {
		printf("%s : kill %s\n", __FUNCTION__ ,id);
		p->pShmItem->nPID = 0;
		p->pid = 0;
		
	}
	

	return 0;
}

int prgctrl::sethwclock()
{
	char cmd[128]={0,};
	sprintf(cmd, "hwclock -w"); 
	return procrun(cmd);
}

int prgctrl::procrun(char *tcmd)
{
	char cmd[2048];
	char *args[40];
	char *p;
	int i; 


	strcpy(cmd, tcmd);
	for (i = 0, p = strtok(cmd, " "); p; p = strtok(NULL, " "), i++)
		args[i] = p;
	args[i] = NULL;

	for (i = 0; i < 40; i++) {
		printf( "procrun: args[%d]: %s\n", i, args[i]);
		if (args[i] == NULL)
			break;
	}

	int pid;
	if ((pid = fork()) == 0) {
		execv(args[0], args);		
		_exit(0);
	}
	child.add(pid);


	return 1;
}

int prgctrl::prockill(int pid)
{

	int x = (kill(pid, 9) == 0);

	return x;
}


#include <sys/wait.h>

static void sigchld_hdl(int sig) {


	while (waitpid(-1, NULL, WNOHANG) > 0) {
		printf(".");
	}

}

void prgctrl::init()
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = sigchld_hdl;

	if (sigaction(SIGCHLD, &act, 0)) {
		printf("Sigaction Regist Error!!\n");
		return;
	}

}

