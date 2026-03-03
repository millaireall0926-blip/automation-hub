#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>

#define SOCKET_ERROR	-1
#define WSAEWOULDBLOCK	EWOULDBLOCK
#define INVALID_SOCKET	-1

#ifdef _LINUX_
	#include <stdint.h>
#else
	#ifndef INADDR_NONE
		#define INADDR_NONE	0xffffffff
	#endif
#endif

#define stricmp strcasecmp

#define SOCK_ERROR(x) x = errno

#include "star_socket.h"

star_socket::star_socket()
{
	fd = (unsigned)SOCK_UNUSED;
	bConn = false;

	lastact = 0;
	ip[0] = '\0';
	udp_ip[0] = '\0';
	port = 0;
	udp_port = 0;

	rd.setup(TCPBUF_MAX, TCPBUF_GROW);
	wr.setup(TCPBUF_MAX, TCPBUF_GROW);
}

star_socket::~star_socket()
{
	if (isConnected()) 
		close();
	bufClear();
}
		

void star_socket::msleep(int n)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = n * 1000;
	select(0, NULL, NULL, NULL, &tv);
}

bool star_socket::isConnected()
{
	return (bConn && fd != INVALID_SOCKET && fd != SOCK_UNUSED);
}

bool star_socket::setReuseAddr()
{
	int	on = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) 
		return false;
	return true;
}

void star_socket::setTimeout(int x)
{
	struct timeval tv;
	tv.tv_sec = x;
	tv.tv_usec = 0;

	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
	setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv));

}

bool star_socket::setNonblock()
{

	int val;

	val = fcntl(fd, F_GETFL, 0);
	if (-1 == fcntl(fd, F_SETFL, val | O_NONBLOCK)) {
		return false;
	}

	return true;
}

bool star_socket::setDontLinger()
{
	struct linger l;

	l.l_onoff = 1;
	l.l_linger = 0;
	if (setsockopt(fd, SOL_SOCKET, SO_LINGER, (char *)&l, sizeof(l)) < 0)
		return false;

	return true;
}

bool star_socket::setTcpBuf(int s, int r)
{
	if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char*)&s, sizeof(s)) < 0) 
		return false;
	
	if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char*)&r, sizeof(r)) < 0) 
		return false;

	return true;
}

bool star_socket::setBroadcast(int n)
{
	if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, (char *)&n, sizeof(n)) < 0)
		return false;

	return true;
}

bool star_socket::setKeepAlive()
{
	int on = 1;

	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on)) < 0)
		return false;

	return true;
}

void star_socket::close()
{

	if (fd != SOCK_UNUSED)
		::close(fd);

	fd = (unsigned)SOCK_UNUSED;
	bConn = false;

	bufClear();
	lastact = 0;
	memset(ip, 0, sizeof(ip));
	memset(udp_ip, 0, sizeof(udp_ip));
	port = 0;
	udp_port = 0;
}

bool star_socket::getAddr(const char *host, const int port, struct sockaddr_in *sa)
{
	unsigned long inaddr;
	struct hostent      *hp;

	memset((char*)sa, 0, sizeof(struct sockaddr_in));
	sa->sin_family = AF_INET;
	sa->sin_port = htons((unsigned short)port);
	if (INADDR_NONE != (inaddr = inet_addr(host)) )
		memcpy((char *)&sa->sin_addr, (char *)&inaddr, sizeof(inaddr));
	else {
		if ((hp = gethostbyname(host)) == NULL)
			return false;
		memcpy((char *)&sa->sin_addr, hp->h_addr_list[0], hp->h_length);
	}
	return true;
}

char *star_socket::getLocalAddr(int which)
{
	static int firstf = 1;
	static char mmac[90];
	static char mip[90];
	static char mbc[90];
	FILE *f;
	char s[256], *p;
	int fip = 0;

	if (firstf) {

		firstf = 0;

		memset(mip, 0, sizeof(mip));
		memset(mmac, 0, sizeof(mmac));
		memset(mbc, 0, sizeof(mbc));

		if (system("/sbin/ifconfig >/tmp/mac.tmp") == -1)
			return (char*)"";


		if ((f = fopen("/tmp/mac.tmp", "rt")) != NULL) {

			while (fgets(s, sizeof(s), f) != NULL) {
				s[strlen(s)-1] = '\0';

				if ((p = strstr(s, "inet addr")) != NULL && !fip) {
					strcpy(mip, p+10);
					fip = 1;
				}

				if ((p = strstr(s, "HWaddr")) != NULL) {
					strcpy(mmac, p+7);
				}
				
				if ((p = strstr(s, "Bcast")) != NULL) {
					strcpy(mbc, p+6);
				}
			}
			fclose(f);
		}

		for (p = mip; *p; p++)
			if (*p == ' ') *p = '\0';

		for (p = mmac; *p; p++)
			if (*p == ' ') *p = '\0';
		for (p = mmac; *p; p++)
			if (*p == ' ') *p = '\0';
	}

	switch (which) {
		case 0 : return mip;
		case 1 : return mmac;
		case 2 : return mbc;
	}
	return mip;
}



void star_socket::printError(const char *sub)
{
	printf("star_socket::SocketError -> %s: %s\n", sub, strerror(errno));
}

bool star_socket::connect(const char *host, int dest_port)
{
	struct sockaddr_in  sa;

	bConn = false;
	if (socket_type == SOC_UDP) {
		memset(&sa, 0, sizeof(sa));
		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
		sa.sin_port = htons(dest_port);

		if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
			printError(__FUNCTION__);
			return false;
		}

		setReuseAddr();
		if (bind(fd, (struct sockaddr*)&sa, sizeof(sa)) != 0) {
			printError(__FUNCTION__);
			::close(fd);
			fd = SOCK_UNUSED;
			return false;
		}
	}
	else {
		if (getAddr(host, dest_port, &sa) != S_ERR_NONE)
			return false;

		if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			printError(__FUNCTION__);
			return false;
		}

		setTimeout(1);
		if (::connect(fd, (struct sockaddr *)&sa, sizeof(sa)) != 0) {
			printError(__FUNCTION__);
			::close(fd);
			fd = SOCK_UNUSED;
			return false;
		}

		setNonblock();
		setDontLinger();
		setKeepAlive();
	}

	setTcpBuf(64*1024, 1024*1024);
	lastact = time(NULL);
	getPeerInfo();
	bConn = true;
	return true;
}

void star_socket::getPeerInfo()
{
	struct sockaddr_in  sa;
	socklen_t salen = sizeof(sa);
		
	memset(&sa, 0, sizeof(sa));
	getpeername(fd, (struct sockaddr*)&sa, &salen);

	strcpy(ip, inet_ntoa(sa.sin_addr));
	port = sa.sin_port;
}

	
bool star_socket::listen(const int port)
{
	struct sockaddr_in	servaddr;
	int st = SOCK_STREAM;

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons((unsigned short)port);

	if (socket_type == SOC_UDP)
		st = SOCK_DGRAM;

	bConn = false;
	if ((fd = socket(AF_INET, st, 0)) == INVALID_SOCKET) {
		printError(__FUNCTION__);
		return false;
	}

	setReuseAddr();
	if (bind(fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		close();
		printError(__FUNCTION__);
		return false;
	}

	if (socket_type == SOC_TCP && ::listen(fd, 1024) != 0) {
		close();
		printError(__FUNCTION__);
		return false;
	}

	setNonblock();

	if (socket_type == SOC_UDP) {
		printf("star_socket::%s: UDP Server Listen Port : % d\n", __FUNCTION__, port);
	}
	else {
		printf("star_socket::%s: UDP Server Listen Port : %d\n", __FUNCTION__, port);
	}

	bConn = true;
	return true;
}

void star_socket::clearTcpBuf()
{
	int sz;

	if ((sz = bufRemains()) <= 0)
		return;

	rd.cnt = 0;
	if (rd.alloc(sz) < 0) {
		printf("star_socket::%s: alloc error\n", __FUNCTION__);
		return;
	}
	
	if (socket_type == SOC_TCP) {
		read(fd, rd.buf, sz);
	}
	else {
		struct sockaddr_in  sa;
		socklen_t	salen = sizeof(sa);
		recvfrom(fd, rd.buf, sz, MSG_NOSIGNAL, (struct sockaddr*)&sa, &salen);
	}

}

void star_socket::bufClear()
{
	rd.reset();
	wr.reset();
}

int star_socket::bufRemains()
{
	int sz;

	if (ioctl(fd, FIONREAD, &sz) != 0)
		return -1;

	return sz;
}

int star_socket::bufRead2(char* buf, int sz)
{
	int	n, err;
	if (socket_type == SOC_TCP) {
		n = read(fd, buf, sz);
		SOCK_ERROR(err);
	}
	else {
		struct sockaddr_in  sa;
		socklen_t	salen = sizeof(sa);

		if (sz == 1)				
			return S_ERR_QUEUED;

		n = recvfrom(fd, buf, sz, MSG_NOSIGNAL, (struct sockaddr*)&sa, &salen);
		SOCK_ERROR(err);
		strcpy(udp_ip, inet_ntoa(sa.sin_addr) );
		udp_port = (int)ntohs(sa.sin_port);
	}

	if (n == SOCKET_ERROR) {
		if (err != WSAEWOULDBLOCK) {
			printError(__FUNCTION__);
			close();
			return S_ERR_GENERAL;
		}
	}
	else if (n == 0) { 
		printf("star_socket::%s: close connection in client\n", __FUNCTION__);
		close();
		return 0;
	}

	lastact = time(NULL);
	return n;
}
	
	
int star_socket::bufRead()
{
	int sz, n, err;

	if ((sz = bufRemains()) < 0)
		return S_ERR_GENERAL;

	sz++;

	err = rd.alloc(sz);
	if (err == -1) {
		return S_ERR_QUEUED;
	}
	else if (err == -2) {

		return S_ERR_NOMEM;
	}

	if (socket_type == SOC_TCP) {

		n = read(fd, rd.buf + rd.cnt, sz);
		SOCK_ERROR(err);
	}
	else {
		struct sockaddr_in  sa;
		socklen_t	salen = sizeof(sa);

		if (sz == 1)				// compensation for sz++;
			return S_ERR_QUEUED;

		n = recvfrom(fd, rd.buf + rd.cnt, sz, MSG_NOSIGNAL, (struct sockaddr*)&sa, &salen);
		SOCK_ERROR(err);
		strcpy(udp_ip, inet_ntoa(sa.sin_addr) );
		udp_port = (int)ntohs(sa.sin_port);
	}

	if (n == SOCKET_ERROR) {
		if (err == WSAEWOULDBLOCK) {
			return S_ERR_QUEUED;
		}
		if (socket_type == SOC_TCP) {
			//printError(__FUNCTION__);
			close();
		}
		return S_ERR_GENERAL;
	}
	else if (n == 0) { 	
		close();
		return S_ERR_GENERAL;
	}

	rd.cnt += n;
	lastact = time(NULL);
	return S_ERR_NONE;
}

int star_socket::bufWrite(void *vbuf, int n)
{
	int sz, err;

	if (!isConnected())
		return S_ERR_GENERAL;

	if (vbuf != NULL) {		// if null, just flush buffer

		err = wr.alloc(n);
		if (err == -1) {
			printf("star_socket::%s: io buffer exceed error\n", __FUNCTION__);
			wr.cnt = 0;					// buf reset (lost old data)
			return S_ERR_GENERAL;
		}
		else if (err == -2) {
			printf("star_socket::bufwrite: malloc error\n");
			return S_ERR_NOMEM;
		}

		if (n > 0) {
			memcpy(wr.buf + wr.cnt, vbuf, n);
			wr.cnt += n;
		}
	}	

	if (socket_type == SOC_TCP) {
		sz = write(fd, wr.buf, wr.cnt);
		SOCK_ERROR(err);
	}
	else {
		struct sockaddr_in  sa;
		socklen_t salen = sizeof(sa);

		if (getAddr(udp_ip, udp_port, &sa) != S_ERR_NONE) {
			printf("star_socket::%s: invalid ip[%s] or port[%d]\n", __FUNCTION__, udp_ip, udp_port);
			return S_ERR_GENERAL;
		}
		sz = sendto(fd, wr.buf, wr.cnt, MSG_NOSIGNAL, (struct sockaddr*)&sa, salen);
		SOCK_ERROR(err);
	}

	if (sz <= 0) {
		if (err != WSAEWOULDBLOCK) {
			printError(__FUNCTION__);
			wr.cnt = 0;
			return S_ERR_GENERAL;
		}
		sz = 0;
	}

	if (sz > 0 && wr.cnt >= sz) {
		memmove(wr.buf, wr.buf + sz, wr.cnt - sz);
		wr.cnt -= sz;
	}
	
	lastact = time(NULL);
	return S_ERR_NONE;
}

int star_socket::bufFlush()
{
	while (wr.cnt > 0) {
		if (bufWrite(NULL, 0) == S_ERR_GENERAL) {
			printf("star_socket::%s: write error\n", __FUNCTION__);
			return S_ERR_GENERAL;
		}
		msleep(1);
	}

	return S_ERR_NONE;
}

