#ifndef _PRGCTRL_H_
#define _PRGCTRL_H_

#define MAX_APPS	16

#include <string.h>
#include <time.h>
#include "../starlib/starlib.h"
#include "../starlib/starlib_dif.h"

static void sigchld_hdl(int sig);

class prgctrl  
{
public:
	typedef struct {
		char	id[32];				
		SHMITEM* pShmItem;			
		int		shmcnt;				
		char	dir[100];			
		char	cmd[2048];			
		int 	forceKill_Cnt;
		int		dead_cnt;			
		int		restart_cnt;		
		int		killed;				
		time_t	lastcheck;		

		int		pid;				

	} program_t;

	class stack 
	{
	public:
		stack() { n = 0; memset(d, 0, sizeof(d)); }
		virtual ~stack() { }

		int		n, nav, d[128];
		
		int first() { nav = 0; return (n > 0) ? d[nav] : 0; }
		int next() { return (++nav >= n) ? 0 : d[nav]; }
		int add(int pid) { if (n < 128) d[n++] = pid; return 1; }
		int remove(int pid) {
			int i;
			for (i = 0; i < n; i++) {
				if (d[i] == pid) {
					memcpy(&d[i], &d[i+1], n - i);
					n--;
					return 1;
				}
			}
			return 0;
		}
	};

	prgctrl() { max_prg = 0; init(); }
	virtual ~prgctrl() {}
	void	init();

public:
	program_t	pdb[MAX_APPS];
	int			max_prg;	
	stack		child;

	void	setup()		{  removeall(); }
	void	removeall()			{ max_prg = 0; memset(pdb, 0, sizeof(pdb)); }
	int		add(const char* id,const char* dir,const char* exec);
	int		remove(char* id);
	int		check();
	void	resetTime();

	program_t* find(const char *id);
	int		isalive(const char* id);
	
	int		pexec(char* id, char* exec);

	int		pkill(char* id);

	int		procrun( char *cmd);
	int		prockill(int pid);

	int		sethwclock();
};

#endif // _PRGCTRL_H_
