#include <vector>
#include <fstream>
#include "define.h"
#include "Scanner.h"
#include "common.h"
#include "pgc.h"
#include "tgc.h"
#include "bib.h"
#include "failmem.h"
#include "net.h"
#include "ioc.h"
#include "operation.h"
#include "eeprom.h"
#include "errctrl.h"
#include "dps.h"
#include "STAR_HSBI.h"

extern bib		gBIB;
extern net      gNET;
extern failmem	gFM;
extern tgc		gTG;
extern pgc		gPG;
extern pds		gPDS;
extern common	gCOM;
extern eeprom   gROM;
extern errctrl     gERR;
extern dps      gDPS;
extern ioc      gIO;

operation::operation()
{
    m_fSetTemp = 30.0;
    m_fReadTemp = 30.0;
    m_bSetTemp = false;

    m_nDefaultMpatDelay = SLEEP_100US;

    //memset(m_PMonArr, -1.0, sizeof(m_PMonArr));

    m_IOVIH    = 0.0;
	m_DRVIH    = 0.0;
	m_VTH      = 0.0;
	m_DRVTT    = 0.0;
	m_IOVTT    = 0.0;
	m_DutDRVTT = 0.0;
	m_DutIOVTT = 0.0;
	m_DutVREF  = 0.0;

    

    stPowerSeq seq;
    seq.chBit = 0;
    seq.delay = 0.0;
    m_PSeq.push_back(seq);
    m_PSeq.push_back(seq);
    m_PSeq.push_back(seq);
    m_PSeq.push_back(seq);
    m_PSeq.push_back(seq);
    m_PSeq.push_back(seq);

    stCalFactor caldata;
    caldata.gain = 1.0;
    caldata.offset = caldata.offset2 = 0.0;

    for(int n = 0; n < MAX_DAC_CH; n++) {
        m_vecDacValue[0].push_back(0.0); // read setting value(float)
        m_vecDacValue[1].push_back(0.0); // read setting value(float)
    }

    ///// cal data init
    for(int idx = 0 ; idx < MAX_ADC_CH; idx++) {
        m_AdcCal[0].push_back(caldata);
        m_AdcCal[1].push_back(caldata);
    }

    for(int idx = 0; idx < MAX_DAC_CH; idx++) {
        m_DacCal[0].push_back(caldata);
        m_DacCal[1].push_back(caldata);
    }
}

operation::~operation() 
{

}

int operation::waitTest(int SleepTime, int Waittime)
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
        
        if (checkStat() == 0) break;                
        int sync = checkSync();        

        if (sync > SYNC_USER_LAST && sync < SYNC_NONE)return sync;
        
        readDPS();        
        //readSysTemp();
        doScanProc();

    }
  //  printf("END!!!\n");
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

void operation::startTest(const char* pc, bool runNow)
{
    gTG.writeFormatBuf();
    gPDS.writeSCOption();
	gFM.writeScanIndex(0);
    gFM.clearFmCount();
    gPG.setPC(pc);
	gNET.checkError();
        

    m_tPowerUpdate = 0;
    m_tTempCheck = 0;
    m_tScanUpdate = 0;
    if (runNow) {
        gPG.runPG();
    }
    m_tStart = time(NULL);
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
        gCOM.debug("--- %s PG Stop Done", __FUNCTION__);
        //printf("--- %s PG Stop Done\n", __FUNCTION__);
        return 0;
    }

    return 1;
}

int operation::checkSync()
{
    int syncID = SYNC_NONE;
    if (gPG.isSync())
    {
        syncID = gPG.getSyncID();
        if (syncID <= SYNC_USER_LAST)   // -> NAND PROCESS
        {
            SYNC(syncID);
//            gPG.nextSync();
        }        
//        printf("checkSync 0x%x sc %d\n ", syncID, gFM.readScanIndex());
    }

    return syncID;
}


void operation::readDPS()
{
    if (m_tPowerUpdate == (uint)time(NULL)) return;
    m_tPowerUpdate = (uint)time(NULL);

    for (int i = 0; i < MAX_DPS_CH; i++)
    {
        float read;
        gDPS.ReadChVout(i, read);
        m_PMonArr[i].rv = read;
        m_PMonArr[i].rc = gDPS.ReadChIout(i, 1, false);

        //if (m_PMonArr[i].rc > m_PMonArr[i].oc)
        if (m_PMonArr[i].oc != -1.0 && m_PMonArr[i].rc > m_PMonArr[i].oc)
            gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d OC(%.2f/%.2f) Error", i + 1, m_PMonArr[i].oc, m_PMonArr[i].rc);

        //if (i % 2 == 0)
        //{
        //    uint status =  gDPS.ReadStatus(i);
        //    if (status & 0x8000)
        //        gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d-%d OV Status(0x%X) Error", i ,i+1, status);
        //    else if (status & 0x4000)
        //        gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d-%d UV Status(0x%X) Error", i, i + 1, status);
        //    else if ((status & 0x0003) != 0x3)
        //        gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d-%d Fault Status(0x%X) Error", i, i + 1, status);
        //    else if ((status & 0x300) != 0x300)
        //        gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d-%d Power Not Good Status(0x%X) Error", i, i + 1, status);
        //}
    }

    //for (int i = 0; i < 2; i++)
    //{
    //    uint alert = gDPS.ReadAlertCtrl((PCI_SEL)i);
    //    if (alert != 0)
    //        gERR.addErr(ERR_POWER, __FUNCTION__, "PG%d DPS Current Fault(Alert) Read(0x%X) Error", i, alert);
    //}

    gNET.checkError();

    //printf("readDPS Done\n");
    

}

void operation::readSysTemp()
{
    if ((m_tTempCheck + 60) > (uint)time(NULL)) return;
    
    m_tTempCheck = (uint)time(NULL);
    
    for (int i = 0; i < 4; i++) {
        float fRead = gROM.read_tmp102(i);
        float fLimit = 100.0;
        if (fRead > fLimit)
        {
            gERR.addErr(ERR_POWER, "BoardTemp", "Board[%d] Temp %.2f, Limit %.2f", i, fRead, fLimit);
        }

        fLimit = 120.0;
        fRead = READ_DPS_TEMP1(i);
        if (fRead > fLimit)
        {
            gERR.addErr(ERR_POWER, "DPS Temp1", "CH[%d] Temp %.2f, Limit %.2f", i, fRead, fLimit);
        }
        fRead = READ_DPS_TEMP2(i);
        if (fRead > fLimit)
        {
            gERR.addErr(ERR_POWER, "DPS Temp2", "CH[%d] Temp %.2f, Limit %.2f", i, fRead, fLimit);
        }

    }
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


bool operation::dpsPowerSeq(int nSeq, unsigned long long chBit, float delay)
{
    if(nSeq < 0 || nSeq > 6) {
        return false;
    }

    if(nSeq == 0) {
        stPowerSeq seq;
        seq.chBit = 0;
        seq.delay = 0;
        m_PSeq.clear();
        m_PSeq.push_back(seq); // 0
        m_PSeq.push_back(seq); // 1
        m_PSeq.push_back(seq); // 2
        m_PSeq.push_back(seq); // 3
        m_PSeq.push_back(seq); // 4
        m_PSeq.push_back(seq); // 5
        m_PSeq.push_back(seq); // 6
        m_PSeq.push_back(seq); // 7
        m_PSeq.push_back(seq); // 8
        m_PSeq.push_back(seq); // 9
    }

    stPowerSeq seq;
    seq.chBit = chBit;
    seq.delay = (delay*1000000);

    m_PSeq[nSeq] = seq;

    printf("Set Seq(%d) Sel(0x%lX) Delay(%f)\n", nSeq, m_PSeq[nSeq].chBit, m_PSeq[nSeq].delay);

    return true;
}


bool operation::readCalFactor()
{
    int size = MAX_ADC_CH*MAX_ADC_NUM;
    //int rsize = 0;
    float adcData[size];
    FILE * g_pFileCalData = NULL;
    g_pFileCalData = fopen("/star/diag/cal/cal.data", "rb");

    if(g_pFileCalData == NULL){
        printf("Cal data file open fail.\n");
        return false;
    }

    fseek(g_pFileCalData , 0, SEEK_SET);

    // ADC Cal gain read
    fread(adcData, size * sizeof(float), 1, g_pFileCalData);
    //}else {
    //    gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)adcData);
    //}
    for(int nAdc = 0; nAdc < MAX_ADC_NUM; nAdc++) {
        for(int idx = 0; idx < MAX_ADC_CH; idx++) {
            if(adcData[(nAdc * MAX_ADC_CH) + idx] > 0.95 && adcData[(nAdc * MAX_ADC_CH) + idx] < 1.05)
                m_AdcCal[nAdc][idx].gain = adcData[(nAdc * MAX_ADC_CH) + idx]; 
            else
                m_AdcCal[nAdc][idx].gain = 1.0;
            //printf("adc %d %d gain : %0.5f\n", nAdc, idx, adcData[(nAdc * MAX_ADC_CH) + idx]);
        }
    }
    // ADC Cal offset read
    fread(adcData, size * sizeof(float), 1, g_pFileCalData);
    //}else {
    //    gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)adcData);
    //}
    for(int nAdc = 0; nAdc < MAX_ADC_NUM; nAdc++) {
        for(int idx = 0; idx < MAX_ADC_CH; idx++) {
            if(adcData[(nAdc * MAX_ADC_CH) + idx] < 0.02 && adcData[(nAdc * MAX_ADC_CH) + idx] > -0.02)
                m_AdcCal[nAdc][idx].offset = adcData[(nAdc * MAX_ADC_CH) + idx]; 
            else
                m_AdcCal[nAdc][idx].offset = 0.0;
            //printf("adc %d %d offset : %0.5f\n", nAdc, idx, adcData[(nAdc * MAX_ADC_CH) + idx]);
        }
    }

    size = MAX_DAC_CH*MAX_DAC_NUM;
    //rsize = 0;
    float dacData[size];

    // DAC Cal gain read
    fread(dacData, size * sizeof(float), 1, g_pFileCalData);
    //}else {
    //    gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)dacData);
    //}
    for(int nDac = 0; nDac < MAX_DAC_NUM; nDac++) {
        for(int idx = 0; idx < MAX_DAC_CH; idx++) {
            if(dacData[(nDac * MAX_DAC_CH) + idx] > 0.95 && dacData[(nDac * MAX_DAC_CH) + idx] < 1.05)
                m_DacCal[nDac][idx].gain = dacData[(nDac * MAX_DAC_CH) + idx]; 
            else
                m_DacCal[nDac][idx].gain = 1.0;
            //printf("dac %d %d gain : %0.5f\n", nDac, idx, m_DacCal[nDac][idx].gain);
        }
    }

    // DAC Cal offset read
    fread(dacData, size * sizeof(float), 1, g_pFileCalData);
    //}else {
    //    gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)dacData);
    //}
    for(int nDac = 0; nDac < MAX_DAC_NUM; nDac++) {
        for(int idx = 0; idx < MAX_DAC_CH; idx++) {
            if(dacData[(nDac * MAX_DAC_CH) + idx] < 0.02 && dacData[(nDac * MAX_DAC_CH) + idx] > -0.02)
                m_DacCal[nDac][idx].offset = dacData[(nDac * MAX_DAC_CH) + idx]; 
            else
                m_DacCal[nDac][idx].offset = 0.0;
            //printf("dac %d %d offset : %0.5f\n", nDac, idx, m_DacCal[nDac][idx].offset);
        }
    }


    size = MAX_DPS_CH;
    //rsize = 0;
    float dpsData[size];
    // DPS VS Cal gain read
    fread(dpsData, size * sizeof(float), 1, g_pFileCalData);
    //}else {
    //    gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)dpsData);
    //}
    for(int idx = 0; idx < MAX_DPS_CH; idx++) {
        if(dpsData[idx] > 0.95 && dpsData[idx] < 1.05)
            gDPS.m_VSCal[idx].gain = dpsData[idx];
        else
            gDPS.m_VSCal[idx].gain = 1.0;
        //printf("dps %d gain : %0.5f\n", idx, gDPS.m_VSCal[idx].gain);
    }

    // DPS VS Cal offset read
    fread(dpsData, size * sizeof(float), 1, g_pFileCalData);
    //}else {
    //    gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)dpsData);
    //}
    for(int idx = 0; idx < MAX_DPS_CH; idx++) {
        if(dpsData[idx] < 0.02 && dpsData[idx] > -0.02)
            gDPS.m_VSCal[idx].offset = dpsData[idx];
        else
            gDPS.m_VSCal[idx].offset = 0.0;
        //printf("dps %d offset : %0.5f\n", idx, gDPS.m_VSCal[idx].offset);
    }


    // DPS VM Cal gain read
    fread(dpsData, size * sizeof(float), 1, g_pFileCalData);
    //}else {
    //    gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)dpsData);
    //}
    for(int idx = 0; idx < MAX_DPS_CH; idx++) {
        if(dpsData[idx] > 0.95 && dpsData[idx] < 1.05)
            gDPS.m_VMCal[idx].gain = dpsData[idx];
        else
            gDPS.m_VMCal[idx].gain = 1.0;
        //printf("dps %d gain : %0.5f\n", idx, gDPS.m_VMCal[idx].gain);
    }

    // DPS VM Cal offset read
    fread(dpsData, size * sizeof(float), 1, g_pFileCalData);
    //}else {
    //    gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)dpsData);
    //}
    for(int idx = 0; idx < MAX_DPS_CH; idx++) {
        if(dpsData[idx] > 0.95 && dpsData[idx] < 1.05)
            gDPS.m_VMCal[idx].offset = dpsData[idx];
        else
            gDPS.m_VMCal[idx].offset = 0.0;
        //printf("dps %d offset : %0.5f\n", idx, gDPS.m_VMCal[idx].offset);
    }


    // DPS IM Cal gain read
    fread(dpsData, size * sizeof(float), 1, g_pFileCalData);
    //}else {
    //    gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)dpsData);
    //}
    for(int idx = 0; idx < MAX_DPS_CH; idx++) {
        if(dpsData[idx] > 0.95 && dpsData[idx] < 1.05)
            gDPS.m_IMCal[idx].gain = dpsData[idx];
        else
            gDPS.m_IMCal[idx].gain = 1.0;
        //printf("dac %d gain : %0.5f\n", idx, gDPS.m_IMCal[idx].gain);
    }

    // DPS IM Cal offset read
    fread(dpsData, size * sizeof(float), 1, g_pFileCalData);
    //}else {
    //    gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)dpsData);
    //}
    for(int idx = 0; idx < MAX_DPS_CH; idx++) {
        if(dpsData[idx] > 0.95 && dpsData[idx] < 1.05)
            gDPS.m_IMCal[idx].offset = dpsData[idx];
        else
            gDPS.m_IMCal[idx].offset = 0.0;
        //printf("dac %d offset : %0.5f\n", idx, gDPS.m_IMCal[idx].offset);
    }

    if(g_pFileCalData)
        fclose(g_pFileCalData);

    return true;

}

bool operation::readTDRFactor()
{
    std::fstream strmFile("/star/diag/cal/tdr.data", std::ios::in);
    if(strmFile.fail()){
        printf("tdr data file open fail.\n");
        gNET.send_msg(MSG_TYPE_ERR,"tdr data file open fail.");
        return false;
    }
    
    unsigned int tdrData;
    //}else {
    //    gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, address, size, (uint8_t *)adcData);
    //}
    
    uint16_t addr = 0;
    for(int nScan = 0; nScan < MAX_TDR_SCAN; nScan++) {
        for(int nPG = 0; nPG < MAX_PG; nPG++) {
            for(int idx = 0; idx < MAX_TDR_IO; idx++) {
                strmFile >> std::hex >> tdrData;//[nScan][idx];
                m_TdrFactor[nScan][nPG].push_back(tdrData);
                //printf("tdr read %d %d %d : %X\n", nScan, nPG, idx, tdrData);
            }
        }
    }

    if(strmFile.eof() ) {
        printf("cpe tune data does not exist.\n");
        return false;
    }

    //CPE TUNE SET
    int first, second;
    strmFile >> std::hex >> first >> second;
    gTG.m_iCpeFine[0][0] = first;
	gTG.m_iCpeFine[0][1] = second;

    strmFile >> std::hex >> first >> second;
    gTG.m_iCpeFine[1][0] = first;
	gTG.m_iCpeFine[1][1] = second;

    return true;
}

bool operation::applyTDRFactor() 
{
    uint16_t addr = 0;
    PCI_SEL pci;
    for(int nScan = 0; nScan < MAX_TDR_SCAN; nScan++) {
        for(int nPG = 0; nPG < MAX_PG; nPG++) {
            if(m_TdrFactor[nScan][nPG].size() == 0) continue;
            pci = (nPG>>1) == 0 ? PCI_SEL::PCI1 : PCI_SEL::PCI2;
            addr = 78 + (nScan * 129);
            for(int idx = 0; idx < MAX_TDR_IO; idx++) {
                gIO.writePG(pci, (0x1<<(nPG&0x1)), (0x1<<9) | addr, m_TdrFactor[nScan][nPG][idx], 1);
                //printf("tdr write %d %d %d : %X\n", nScan, nPG, idx, m_TdrFactor[nScan][nPG][idx]);
                addr++;
            }
        }
    }

    // TDR 값 적용
    gIO.writePG(PCI1, PG_ALL, 0x1<<9 | 77, 0x1, 1);
    gIO.writePG(PCI2, PG_ALL, 0x1<<9 | 77, 0x1, 1);

    return true;
}