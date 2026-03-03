#ifndef __COMPAT_H__
#define __COMPAT_H__

#define NAME	"Star Protocol Daemon (process alive mon off)"
#define VERSION	"1.1.2"
#define RELDATE	"2025/06/04"
#define AUTHOR		"uiop"
#define PRG_VER	NAME " " VERSION " " RELDATE " by " AUTHOR



/*
#define VERSION	"1.1.2"
#define RELDATE	"2025/06/05"
	bug : proc 종료시 no proc일때 pid clear .. 
	      check 루틴에서 해주지만 test주기가 빨라지며 바로 clear
		  pgm process 실행시 exist 감시 기능 끔. Protocal에 의존하도록.

#define VERSION	"1.1.1"
#define RELDATE	"2025/03/06"
	mod : BIB ID 8Bit -> 4Bit Gathering
	--- > �ٽ� 8Bit Gathring���� ���� ���߿� HW���� �������� �۾�

HEADER("v1.10	2024/10/18")
	L("mod : kill To react instantly. for Handler speed test")

HEADER("v1.09	2024/01/17")
	L("add : modify for MAINT")
* 
 HEADER("v1.08	2023/07/05")
	L("add : modify for new router")
 
* HEADER("v1.07	2023/6/13")
	L("add : Organize Source")
* 
* HEADER("v1.06	2022/9/08")
	L("add : stat data attribute position change for webdata")
	L("add : cfg_fpga remove pg arg. how couud it be called??")

HEADER("v1.05	2022/9/08")
	L("add : Process Run Cmd Buffer Size Increase for step arg ")

HEADER("v1.03	2022/7/25")
	L("add : first setup")

HEADER("v1.00	2021/12/15")
	L("add : first setup")
*/



#endif
