#pragma once

#ifndef _WINDOWS
#define __declspec(dllexport)
#endif

typedef unsigned short WORD;
typedef unsigned char UCHAR;


__declspec(dllexport) int Read_EC_Data(unsigned int, unsigned char *);
__declspec(dllexport) int Write_EC_Data(unsigned int, unsigned char);
__declspec(dllexport) int Write_EC_Cmd(unsigned char);
__declspec(dllexport) int Reset_I2c(void);
