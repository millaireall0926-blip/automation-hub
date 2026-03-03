#include <vector>
#include <fstream>
#include "define.h"
#include "net.h"
#include "errc.h"
#include "operation.h"
#include "STAR_TEST.h"
#include "tgc.h"
#include "dps.h"
#include "starAPI.h"
#include "ioc.h"
#include "failmem.h"
#include "pgc.h"
#include "define.h"
#include "bib.h"
#include "power.h"

extern pgc		gPG;
extern bib		gBIB;
extern failmem	gFM;
extern ioc		gIO;
extern net      gNET;
extern errctrl  gERR;
extern tgc      gTG;
extern dps      gDPS;
extern starAPI  gAPI;
extern pds      gPDS;
extern powerc   gPWR;



operation::operation()
{
    m_fSetTemp = 30.0;
    m_fReadTemp = 30.0;
    m_bSetTemp = false;

    m_nDefaultMpatDelay = SLEEP_100US;

    m_tPowerUpdate = 0;
    m_tTempCheck = 0;
    m_tScanUpdate = 0;

}

operation::~operation()
{

}

void operation::startTest(const char* pc, bool runNow)
{
    gTG.writeFormatBuf();        
    gFM.clearFmCount();
    gPG.setPC(pc);
    gNET.checkError();


    m_tPowerUpdate = 0;
    m_tTempCheck = 0;
    m_tScanUpdate = 0;
    if (runNow)
        gPG.runPG();
    m_tStart = time(NULL);    
}

void operation::readDPS()
{
    if (m_tPowerUpdate == (uint)time(NULL)) return;
    m_tPowerUpdate = (uint)time(NULL);

	gPWR.readAll();

	bool isErr = false;
	if (gPWR.readDpsFault() != 0)
    {
        gERR.addErr(ERR_POWER, __FUNCTION__, "DPS(idx:%02x) Fault Error", gPWR.readDpsFault());
		isErr = true;
	}
    else if (gPWR.readDpsAlert() != 0)
    {
        gERR.addErr(ERR_POWER, __FUNCTION__, "DPS(idx:%02x) Alert Error", gPWR.readDpsAlert());
        isErr = true;
    }

    if (isErr)
    {
        for (int i = 0; i < MAX_DPS_CH; i++)
            gERR.addErr(ERR_POWER, __FUNCTION__, "Error DPS VS%d Status(0x%04x)",
                (i + 1), gPWR.readDpsStatus(i));
    }

	for (int i = 0; i < MAX_DPS_CH; i++)
    {
        PMON* pmon = &gPWR.m_PMonArr[i];
		pmon->rv = gPWR.readDpsVolt(i);        
        pmon->rc = gPWR.readInaCurrent(i);
        //pmon->rc = gPWR.readDpsCurrent(i);
        if (pmon->rv > pmon->ov)
            gERR.addErr(ERR_POWER, __FUNCTION__, "VS%d OV(%.2f/%.2f) Error", i + 1, pmon->rv, pmon->ov);
        if ((pmon->rv < pmon->uv))
            gERR.addErr(ERR_POWER, __FUNCTION__, "VS%d UV(%.2f/%.2f) Error", i + 1, pmon->rv, pmon->uv);
        if ((pmon->rc > pmon->oc))
            gERR.addErr(ERR_POWER, __FUNCTION__, "VS%d OC(%.2f/%.2f) Error", i + 1, pmon->oc, pmon->rc);
    }

	for (int i = 0; i < 4; i++)
    {
        PMON* pmon = &gPWR.m_PMonArr[MAX_DPS_CH + i];
        pmon->rv = gPWR.getAdc(HV1 + i);
		pmon->rc = gPWR.readInaCurrent(MAX_DPS_CH + i);
    }

    if (gAPI.m_bMonDPS)
    {
        printf("name    sv      rv      oc      rc\n");
        printf("----------------------------------\n");
        char buf[128];
        int idx[5] = { 0,1,4,2,3 };
        int ch = 0;
        for (int i = 0; i <= MAX_PMON_CH; i++)
        {
            PMON* pmon = &gPWR.m_PMonArr[i];
            ch = idx[i];
			const char* name = (i < MAX_DPS_CH) ? "dps" : "hv";
			printf("%s%d   %.2f    %.2f    %.2f    %.2f\n", 
                name, (ch + 1), pmon->sv, pmon->rv, pmon->oc, pmon->rc);
            sprintf(buf, "%s%d   %.2f    %.2f    %.2f    %.2f",
                name, (ch + 1), pmon->sv, pmon->rv, pmon->oc, pmon->rc);

            gNET.send_msg(MSG_TYPE_INFO, buf);
        }
    }

    gNET.checkError();
    
}

int operation::checkSync()
{
    int syncID = SYNC_NONE;
    if (gPG.isSync())
    {
        syncID = gPG.getSyncID();
        if (syncID <= SYNC_USER_LAST)   // -> NAND PROCESS        
            SYNC(syncID);                 
        //        printf("checkSync 0x%x sc %d\n ", syncID, gFM.readScanIndex());
    }

    return syncID;
}

int operation::checkStat()
{
    if (gNET.m_bfStop)
    {
        while (1)
        {
            printf("waitting.. killed\n");
            usleep(1000 * 1000);
        }
    }
    //printf("CHECK : isRun %d\n", gPG.isRun());
    if (!gPG.isRun())
    {
        gAPI.debug("--- %s PG Stop Done", __FUNCTION__);        
        return 0;
    }
    return 1;
}

void operation::doScanProc()
{
    if (m_tScanUpdate == (uint)time(NULL)) return;
    m_tScanUpdate = (uint)time(NULL);

    int readScan = gFM.readScanIndex();
    //    printf("read sc %d\n");
    if (gBIB.m_nCurScanNo != readScan)
    {
        gBIB.m_nCurScanNo = readScan;
        //  printf("CUR Scan No %d READ %d\n", gBIB.m_nCurScanNo, readScan);
        if (gBIB.m_nCurScanNo >= gBIB.m_nScanSize) gNET.sendScanInfo(1); // all scan flag enable
        else gNET.sendScanInfo();
    }
}


int operation::waitTest(uint SleepTime, uint Waittime)
{
    uint curTime = time(NULL);
    gNET.m_nSendPower = SEND_PWR_READ;
    gBIB.m_nCurScanNo = -1;

    while (1)
    {
        if (Waittime != 0)
            if (curTime + Waittime < (uint)time(NULL))
                break;

        if (SleepTime != 0) {
            //printf("sleep %d ms\n", SleepTime);
            usleep(SleepTime);
        }

        if (checkStat() == 0) break;   // pg run / stop check

        int sync = checkSync();      // sync check / id read
        if (sync > SYNC_USER_LAST && sync < SYNC_NONE)return sync;

        readDPS();
        if (!gAPI.m_bUseHandler)                    
            doScanProc();
        
    }

    gNET.m_nSendPower = SEND_PWR_OFF;
    gBIB.m_nCurScanNo = gFM.readScanIndex();

    int all = 0;
    if ((gBIB.m_nCurScanNo > gBIB.m_nScanSize)) {
        all = 1;
        gBIB.m_nCurScanNo = gBIB.m_nScanSize;
    }
    gNET.sendScanInfo(all);


    return SYNC_END;
}

void operation::stopTest()
{
    gPG.stopPG();
}


void operation::waitTemp(int waitLimit)
{
    gNET.m_nSendPower = SEND_PWR_ZERO;
    int cnt = 0;
    while (1) {
        printf("Temp Wait SetTemp %.1f, ReadTemp %.1f\n", m_fSetTemp, m_fReadTemp);
        if ((m_fReadTemp >= (m_fSetTemp - 0.5)) && (m_fReadTemp <= (m_fSetTemp + 0.5)))
        {
            printf("Reached Temp Wait OK!!! SetTemp %.1f, ReadTemp %.1f\n", m_fSetTemp, m_fReadTemp);
            break;
        }

        if (cnt % 60 == 0)
            gNET.sendTemp(1);
        else {
            if (cnt % 2 == 0)
                gNET.reqTemp();
        }

        cnt++;
        sleep(1);

        if (waitLimit == 0) continue;
        if (cnt >= waitLimit) {
            gERR.addErr(ERR_TEMP, __FUNCTION__, "Temp Wait Limit Error");
            break;
        }

    }
    gNET.m_nSendPower = SEND_PWR_OFF;


}


