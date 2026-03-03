#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#ifndef _WIN32
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

#endif

#ifdef _WIN32
#define MSG_NOSIGNAL 0
#define SOCK_ERROR(x) x = WSAGetLastError()
typedef int socklen_t;
#else
#define SOCK_ERROR(x) x = errno
#endif

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
		
void star_socket::wlog(char *fmt, ...)
{
	return;
	va_list	ap;
	char buf[2048] = {0};

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf)-1, fmt, ap);
	va_end(ap);

	printf("%s", buf);
#ifdef _WIN32
	OutputDebugString(buf);
#endif
}

void star_socket::msleep(int n)
{
#ifdef _WIN32
	Sleep(n);
#else
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = n * 1000;
	select(0, NULL, NULL, NULL, &tv);
#endif
}

void star_socket::dump(char *p, int sz)
{
	char s[33];
	int i, j;

	for (i = 0; i < sz; i += 32) {
		memset(s, 0, sizeof(s));
		wlog("%04x: ", i);
		for (j = i; j < i + 32 && j < sz; j++) {
			wlog("%02X ", p[j] & 0xff);
			s[j-i] = (p[j] >= (char)32 && p[j] <= (char)128) ? p[j] : '.';
		}
		wlog("%s\n", s);
	}
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
#ifdef _WIN32
	unsigned int timeout = x;
	int len = sizeof(unsigned int);

	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(unsigned int));
	setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(unsigned int));

	getsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, &len);
	getsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, &len);

#else
	struct timeval tv;
	tv.tv_sec = x;
	tv.tv_usec = 0;

	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
	setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv));
#endif
}

bool star_socket::setNonblock()
{
#ifndef _WIN32
	int val;

	val = fcntl(fd, F_GETFL, 0);
	if (-1 == fcntl(fd, F_SETFL, val | O_NONBLOCK)) {
		return false;
	}
#else

	long val = -1;

	if (ioctlsocket(fd, FIONBIO, (unsigned long*)&val) != 0)
		return false;

#endif
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
#ifdef _WIN32
	if (fd != SOCK_UNUSED)
		::closesocket(fd);
#else
	if (fd != SOCK_UNUSED)
		::close(fd);
#endif
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

#ifdef _WIN32
// from iphlpapi.h

#define MAX_ADAPTER_DESCRIPTION_LENGTH  128 // arb.
#define MAX_ADAPTER_NAME_LENGTH         256 // arb.
#define MAX_ADAPTER_ADDRESS_LENGTH      8   // arb.
#define DEFAULT_MINIMUM_ENTITIES        32  // arb.
#define MAX_HOSTNAME_LEN                128 // arb.
#define MAX_DOMAIN_NAME_LEN             128 // arb.
#define MAX_SCOPE_ID_LEN                256 // arb.

// #include <iphlpapi.h>

typedef struct {
	char String[4 * 4];
} IP_ADDRESS_STRING, *PIP_ADDRESS_STRING, IP_MASK_STRING, *PIP_MASK_STRING;

typedef struct _IP_ADDR_STRING {
	struct _IP_ADDR_STRING* Next;
	IP_ADDRESS_STRING IpAddress;
	IP_MASK_STRING IpMask;
	DWORD Context;
} IP_ADDR_STRING, *PIP_ADDR_STRING;

typedef struct _IP_ADAPTER_INFO {
	struct _IP_ADAPTER_INFO* Next;
	DWORD ComboIndex;
	char AdapterName[MAX_ADAPTER_NAME_LENGTH + 4];
	char Description[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];
	UINT AddressLength;
	BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];
	DWORD Index;
	UINT Type;
	UINT DhcpEnabled;
	PIP_ADDR_STRING CurrentIpAddress;
	IP_ADDR_STRING IpAddressList;
	IP_ADDR_STRING GatewayList;
	IP_ADDR_STRING DhcpServer;
	BOOL HaveWins;
	IP_ADDR_STRING PrimaryWinsServer;
	IP_ADDR_STRING SecondaryWinsServer;
	time_t LeaseObtained;
	time_t LeaseExpires;
} IP_ADAPTER_INFO, *PIP_ADAPTER_INFO;

extern "C" {
DWORD WINAPI GetAdaptersInfo(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen); 
}

char* star_socket::getLocalAddr(int which)
{
	unsigned long sz;
	static int firstf = 1;
	static char mip[20];
	static char mmac[30];
	int ip_count = 0;

	if (firstf) {

		firstf = 0;
		memset(mip, 0, sizeof(mip));
		memset(mmac, 0, sizeof(mmac));

		GetAdaptersInfo(NULL, &sz);
		IP_ADAPTER_INFO *adapter = (PIP_ADAPTER_INFO)new char[sz];
		if (GetAdaptersInfo(adapter, &sz) != ERROR_SUCCESS) {
			delete[] adapter;
			return NULL;
		}
		
		// first adapter, first address
		IP_ADDR_STRING *ip;

		do
		{
			if (ip_count == 0)	ip = &adapter->IpAddressList;
			else                ip = ip->Next;

			if (!strncmp(ip->IpAddress.String, "192.168.123", 11))
				break;
			ip_count++;
		} while (ip->Next != NULL);

		strcpy(mip, ip->IpAddress.String);
		
		unsigned char* p = adapter->Address;
		sprintf(mmac, "%02x:%02x:%02x:%02x:%02x:%02x", p[0], p[1], p[2], p[3], p[4], p[5]);
		delete[] adapter;
	}

	switch (which) {
		case 0: return mip;
		case 1: return mmac;
		case 2: return "255.255.255.255";
	}

	return mip;
}

#else

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

		system("/sbin/ifconfig >/tmp/mac.tmp");
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

#endif

void star_socket::printError(const char *sub)
{

#ifdef _WIN32
	LPVOID lpMsgBuf;
	if (!FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		WSAGetLastError(),
		0,
		(LPTSTR) &lpMsgBuf,
		0,
		NULL ))
	   return;
	wlog("%s: %s", sub, lpMsgBuf);
	LocalFree(lpMsgBuf);
#else
	wlog("%s: %s\n", sub, strerror(errno));
#endif
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
#ifdef _WIN32
			::closesocket(fd);
#else
			::close(fd);
#endif
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
#ifdef _WIN32
			::closesocket(fd);
#else
			::close(fd);
#endif
			fd = SOCK_UNUSED;
			return false;
		}

		setNonblock();
		setDontLinger();
		setKeepAlive();
	}
	
#ifndef _WIN32	
	setTcpBuf(64*1024, 1024*1024);
#else
#endif
	lastact = time(NULL);
	getPeerInfo();
	bConn = true;
	return true;
}

bool star_socket::connectNB(const char *host, int dest_port)
{
	struct sockaddr_in  sa;

	bConn = false;
	if (getAddr(host, dest_port, &sa) != S_ERR_NONE)
		return false;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printError(__FUNCTION__);
		return false;
	}

	setNonblock();
	::connect(fd, (struct sockaddr *)&sa, sizeof(sa));

	struct  timeval     timevalue;
	fd_set				fdset;

	FD_ZERO( &fdset );
	FD_SET( fd, &fdset );
	timevalue.tv_sec = 0;
	timevalue.tv_usec = 200000;

	::select( fd+1, NULL, &fdset, NULL, &timevalue );
	if ( !FD_ISSET( fd, &fdset ) )
	{
		printError( __FUNCTION__);
#ifdef _WIN32	
		::closesocket( fd );
#else
		::close(fd);
#endif
		fd = SOCK_UNUSED;
		return false;
	}

	setNonblock();
	setDontLinger();
	setKeepAlive();

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

//	soc_setbuf(1024 * 1024, 64 * 1024);
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
		wlog("%s: udp listen on %d\n", __FUNCTION__, port);
	}
	else {
		wlog("%s: tcp listen on %d\n", __FUNCTION__, port);
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
		wlog("%s: memory alloc error\n", __FUNCTION__);
		return;
	}
	
	if (socket_type == SOC_TCP) {
#ifdef _WIN32
		recv(fd, rd.buf, sz, 0);
#else
		read(fd, rd.buf, sz);
#endif
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

#ifdef _WIN32
	if (ioctlsocket(fd, FIONREAD, (unsigned long *)&sz) != 0)
#else
	if (ioctl(fd, FIONREAD, &sz) != 0)
#endif
		return -1;

	return sz;
}

int star_socket::bufRead2(char* buf, int sz)
{
	int	n, err;
	if (socket_type == SOC_TCP) {
#ifdef _WIN32
		if (sz == 0)
			return S_ERR_QUEUED;

		n = recv(fd, buf, sz, 0);
#else
		n = read(fd, buf, sz);
#endif
		SOCK_ERROR(err);
	}
	else {
		struct sockaddr_in  sa;
		socklen_t	salen = sizeof(sa);

		if (sz == 1)				// compensation for sz++;
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
	else if (n == 0) { // gracefully closed
		wlog("%s: remote peer close connection\n", __FUNCTION__);
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
//		wlog("%s: io buffer exceed error, cnt=%d, max=%d\n", __FUNCTION__, rd.cnt, rd.max);
		return S_ERR_QUEUED;		// prevent errors for that the remote client transmit data too fast.
//		return S_ERR_NOMEM;			// uncomment this line if you want to defense from port attack
	}
	else if (err == -2) {
//		wlog("%s: malloc failed\n", __FUNCTION__);
		return S_ERR_NOMEM;
	}
	
	// read peer

	if (socket_type == SOC_TCP) {
#ifdef _WIN32
		if (sz == 0)
			return S_ERR_QUEUED;

		n = recv(fd, rd.buf + rd.cnt, sz, 0);
#else
		n = read(fd, rd.buf + rd.cnt, sz);
#endif
		SOCK_ERROR(err);
	}
	else {
		struct sockaddr_in  sa;
		socklen_t	salen = sizeof(sa);

		if (sz == 1)				// compensation for sz++;
			return S_ERR_QUEUED;

		n = recvfrom(fd, rd.buf + rd.cnt, sz, MSG_NOSIGNAL, (struct sockaddr*)&sa, &salen);

//		wlog("recvfrom rd.cnt=%d, n=%d: ", rd.cnt, n); dump(rd.buf, n+rd.cnt);
		
		SOCK_ERROR(err);
		strcpy(udp_ip, inet_ntoa(sa.sin_addr) );
		udp_port = (int)ntohs(sa.sin_port);
	}

	if (n == SOCKET_ERROR) {
		if (err == WSAEWOULDBLOCK) {
			return S_ERR_QUEUED;
		}
		if (socket_type == SOC_TCP) {
			printError(__FUNCTION__);
			close();
		}
		return S_ERR_GENERAL;
	}
	else if (n == 0) { // gracefully closed
	//	wlog("%s: remote peer close connection\n", __FUNCTION__);
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
			wlog("%s: io buffer exceed error\n", __FUNCTION__);
			wr.cnt = 0;					// buf reset (lost old data)
			return S_ERR_GENERAL;
		}
		else if (err == -2) {
			wlog("star_session::bufwrite: malloc error\n");
			return S_ERR_NOMEM;
		}

		if (n > 0) {
			memcpy(wr.buf + wr.cnt, vbuf, n);
			wr.cnt += n;
		}
	}

	//wlog("wrbuf: "); dump(wr.buf, wr.cnt);

	if (socket_type == SOC_TCP) {
#ifdef _WIN32
		sz = send(fd, wr.buf, wr.cnt, 0);
#else
		sz = write(fd, wr.buf, wr.cnt);
#endif
		SOCK_ERROR(err);
	}
	else {
		struct sockaddr_in  sa;
		socklen_t salen = sizeof(sa);

		if (getAddr(udp_ip, udp_port, &sa) != S_ERR_NONE) {
			wlog("%s: invalid ip[%s] or port[%d]\n", __FUNCTION__, udp_ip, udp_port);
			return S_ERR_GENERAL;
		}

		sz = sendto(fd, wr.buf, wr.cnt, MSG_NOSIGNAL, (struct sockaddr*)&sa, salen);
		SOCK_ERROR(err);
	}

	if (sz <= 0) {
		if (err != WSAEWOULDBLOCK) {
			printError(__FUNCTION__);
//			printf("send: %s\n", wr.buf);
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
			wlog("%s: write error\n", __FUNCTION__);
			return S_ERR_GENERAL;
		}
		msleep(1);
	}

	return S_ERR_NONE;
}

