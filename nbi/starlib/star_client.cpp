#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compat.h"
#include "star_client.h"

int star_client::open(cchar *host, int port)
{
	s.soc.close();

	if (s.soc.connect(host, port) != S_ERR_NONE) {
		s.wlog("%s: connect error: host=%s, port=%d\n", __FUNCTION__, host, port);
		return 0;		
	}

	s.soc.setNonblock();
	s.soc.setKeepAlive();
	return 1;	
}

void star_client::close()
{
	s.soc.close();
}

int	star_client::send(int mode, cchar *buf, int len)
{
	
	int ret = s.starWrite(mode, len, (void*)buf);
	return (ret == S_ERR_NONE) ? 1 : 0;
}


int star_client::ping(cchar *ip)
{
	char buf[1024] = {0};

	s.end();
	s.soc.setSocketType(SOC_UDP);
	if (s.soc.connect(ip, STAR_PROBE_PORT) != S_ERR_NONE) {
		s.wlog("%s: connect error\n", __FUNCTION__);
		return 0;
	}

	snprintf(buf, sizeof(buf)-1, "<star-p><identify entity=\"PROBE\"/></star-p>");

	s.soc.setBroadcast(1);
	strcpy(s.soc.udp_ip, ip);
	s.soc.udp_port = STAR_ADMIN_PORT;

	return send(PKT_TYPE_ADMIN, buf, strlen(buf));
}
	
	
int	star_client::recv()
{
	if (!s.isConnected()) 
		return 0;							

	int ret = s.starRead();
	switch(ret) {
		case S_ERR_GENERAL:	return 0;		
		case S_ERR_QUEUED:	return 1;		
		case S_ERR_NONE:
			
			if (s.packet.cnt > 0) {
				char* p = s.parseXml(s.packet.buf);
				if (p != NULL) 
					s.setPath(s.getXmlAtt("PATH"));
			}
			
			switch(s.getType()) {
			case PKT_TYPE_REQFILE:	return s.starRecvFile() ? PKT_TYPE_REQFILE : 1;			
			}

			return 2;		
	}
	return 1;		
}

int star_client::sendFile(cchar* star_path, int mode, cchar *target_dir, cchar *source)
{
	char target[512] = {0};
	char *p = strrchr((char*)source, '/');
	char *fn = (char*)source;
	
	if (p == NULL) {
		p = strrchr((char*)source, '\\');
	}

	if (p != NULL) 
		fn = p + 1;
	
	if (target_dir[strlen(target_dir)-1] == '/') 
		snprintf(target, sizeof(target)-1,"%s%s", target_dir, fn);
	else
		snprintf(target, sizeof(target)-1,"%s/%s", target_dir, fn);
		
	return s.starSendFile(star_path, mode, source, target);
}


