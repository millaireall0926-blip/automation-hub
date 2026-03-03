#ifndef __STAR_CLIENT__
#define __STAR_CLIENT__

#include "star_session.h"

class star_client
{
private:

public:
	
	star_client() { }
	~star_client() { close(); }

public:
	star_session		s;

	char			entity[128];			
	char			name[128];				
	char			ip[30];					
	int				port;					
	
	int				open(cchar *host, int port);
	void			close();
	int				ping(cchar *ip);
	int				send(int mode, cchar *buf, int len);
	int				recv();
	int				sendFile(cchar* sStarPath, int mode, cchar *target, cchar *source);	
	int				recvFile();	

	inline char*	getData() 		{ return s.packet.buf; }
	inline int		getLength() 	{ return s.packet.cnt; }
	inline int		getType() 		{ return s.getType(); }

	inline char*	getPath() 		{ return s.sStarPath; }
	inline char*	getLocalIp(int nIndex)	{ return s.soc.getLocalAddr(nIndex); }
	inline void		setPacketSize(int nSize)	{ s.setChunkSize(nSize);}			
};

#endif
