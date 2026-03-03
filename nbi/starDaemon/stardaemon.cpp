#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "../starlib/starlib_dif.h"
#include "star_server.h"



 
#define CONFIG_FN	"/star/test/config.xml"

starlib* lib = NULL;

int main(int argc, char *argv[])
{
	printf("stardaemon main start");
	FILE *f;
	char name[128];
	const char* cfn = CONFIG_FN;

	lib = new starlib;
	if (lib == NULL) {
		printf("error create starlib\n");
		return 0;
	}
	
	switch(lib->load()) {
		case -1:
			printf("can't open starlib.so\n");
			return 0;
		case -2:
			printf("incompatible version of starlib.so\n");
			return 0;
	}

	strcpy(name, "Slot");

	star_server svr;

	
	if ((f = fopen(STAR_INI, "rt")) != NULL) {
		if (fgets(name, sizeof(name), f) != NULL)
			name[strlen(name)-1] = '\0';
		fclose(f);
	}

	if (argc > 1) {
		if (strstr(argv[1], "-d") != NULL) {
			svr.m_nDebug = 1;
			printf("Debug Message ON\n");
		}
	}
	
	if (svr.setup((char*)cfn, name, NULL, NULL) != S_ERR_NONE) {
		printf("can't start server\n");
		exit(-1);
	}

	svr.doInfinite(0);        
}
