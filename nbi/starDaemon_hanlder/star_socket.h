

#ifndef _STAR_SOCKET_
#define _STAR_SOCKET_

#include <time.h>



#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "compat.h"

// general error
#define S_ERR_NONE		1
#define S_ERR_GENERAL	0
#define S_ERR_NOMEM		-1
#define	S_ERR_QUEUED	-2

// socket type
#define SOC_TCP			0
#define SOC_UDP			1

// internal io buffer 
#define TCPBUF_MAX				2048*1024	// 2MB
#define TCPBUF_GROW				128*1024	// 128KB

// session_t.fd

#define SOCK_UNUSED		-1

class star_buffer
{
public:
	star_buffer(int m, int gw)	{ setup(m, gw); clear(); }
	star_buffer()				{ setup(65536, 1024); clear(); }
	~star_buffer()				{ reset(); }

public:
	char*	buf;
	int		cnt;
	int		max;
	int		limit;
	int		csize;

	inline void	clear()					{ buf = NULL; cnt = max = 0; }
	inline void setup(int m, int gw)	{ limit = m; csize = gw; }
	inline int allocsz(int x)			{ return ( ((x) / csize + 1) * csize ); }

	int alloc(int sz)
	{
		char* tmp;
		
		if (cnt + sz > limit) 
			return -1;		// buffer full
	
		if (buf == NULL) {
			max = allocsz(sz);
			if ((buf = (char *)malloc(max)) == NULL)
				return -2;	// memory error
		}
		else if (cnt + sz > max) {
			max = allocsz(cnt + sz);
			if ((tmp = (char *)realloc(buf, max)) == NULL)
				return -2; // memory error
			buf = tmp;
		}
		return 0;	// no error
	}

	void reset()			
	{ 
		if (buf != NULL) {
			free(buf);
			buf = NULL;
			cnt = 0; 
		}
	}
};


class star_socket
{
public:
	star_socket();
	virtual ~star_socket();
		
public:

	int			fd;

	int			socket_type;	
	time_t		lastact;		
	star_buffer	rd;
	star_buffer	wr;

	char		ip[30];			
	int			port;			
	char		udp_ip[30];		
	int			udp_port;		
	bool		bConn;

	void	setSocketType(int s) { socket_type = s;  }				
	bool	setReuseAddr();
	void	setTimeout(int x);
	bool	setNonblock();
	bool	setBroadcast(int n);
	bool	setKeepAlive();
	bool	setTcpBuf(int s, int r);
	bool	setDontLinger();

	char*	getLocalAddr(int which);
	void	getPeerInfo();
	bool	getAddr(const char *host, const int port, struct sockaddr_in *sa);

	bool	connect(const char *host, const int port);	
	bool	listen(const int port);
	void	close();

	bool	isConnected();						
	void	clearTcpBuf();						
	void	printError(const char *sub);
	void	msleep(int n);		

	
	void	bufClear();
	int		bufRemains();
	int		bufRead();
	int		bufRead2(char* buf, int sz);
	int 	bufWrite(void *vbuf, int n);
	int		bufFlush();
	
};
	

#endif 
