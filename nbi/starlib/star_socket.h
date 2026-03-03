

#ifndef _STAR_SOCKET_
#define _STAR_SOCKET_

#include <time.h>


#ifdef _WIN32
	#pragma once
	#include <winsock2.h>
#else
	#include <stdlib.h>
	#include <sys/types.h>
	#include <sys/socket.h>
#endif

// general error

#define S_ERR_NONE		1
#define S_ERR_GENERAL	0
#define S_ERR_NOMEM		-1
#define	S_ERR_QUEUED	-2

// socket type

#define SOC_TCP			0
#define SOC_UDP			1


// internal io buffer 

#define TCPBUF_MAX				2097152	// 2MB
#define TCPBUF_GROW				131072	// 128KB

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
#ifdef _WIN32
	SOCKET		fd;
#else
	int			fd;
#endif
	int			socket_type;	// 0=TCP, 1=UDP
	time_t		lastact;		// 마지막으로 패킷을 주고받은 시점
	star_buffer	rd;
	star_buffer	wr;

	char		ip[30];			// tcp peer ip 
	int			port;			// tcp peer port 
	char		udp_ip[30];		// udp recieve ip address
	int			udp_port;		// udp recieve port
	bool		bConn;

	void	setSocketType(int s) { socket_type = s;  }				// set socket type
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
	bool	connectNB(const char *host, int dest_port);
	bool	listen(const int port);
	void	close();

	bool	isConnected();								// socket status
	void	clearTcpBuf();						// TCP read buffer clear (for UDP cleanup)
	void	printError(const char *sub);
	void	msleep(int n);
	void	dump(char* p, int sz);
	void	wlog(char *fmt, ...);

	// buf function

	void	bufClear();
	int		bufRemains();
	int		bufRead();
	int		bufRead2(char* buf, int sz);
	int 	bufWrite(void *vbuf, int n);
	int		bufFlush();
	
};
	

#endif 
