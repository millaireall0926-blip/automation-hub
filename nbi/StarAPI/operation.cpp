#include "define.h"
#include "operation.h"
#include "pgc.h"
#include "bib.h"
#include "failmem.h"
#include "net.h"
#include "ioc.h"
#include "errc.h"
#include "STAR_TEST.h"
#include "tgc.h"
#include "dps.h"
#include "starAPI.h"
#include "eeprom.h"
#include "eapi/ec.h"



extern pgc		g_clPgc;
extern bib		g_clBib;
extern failmem	g_clFm;
extern net		g_clNet;
extern ioc		g_clIO;
extern errc     g_clErr;
extern tgc      g_clTgc;
extern dps      g_clDPS;
extern starAPI  g_clAPI;
extern pds      g_clPds;
extern eeprom   g_clRom;


operation::operation()
{
    m_fSetTemp = 30.0;
    m_fReadTemp = 30.0;
    m_bSetTemp = false;
    MakeCalFactor();

    stPowerSeq seq;
    seq.chBit = 0;
    seq.delay = 0.0;
    m_PSeq.push_back(seq);
    m_PSeq.push_back(seq);
    m_PSeq.push_back(seq);
    m_PSeq.push_back(seq);
    m_PSeq.push_back(seq);
    m_PSeq.push_back(seq);

    memset(m_PMonArr, 0, sizeof(m_PMonArr));

    m_flgDpsOn = 0;

    m_nDefaultMpatDelay = SLEEP_100US;
}

operation::~operation() 
{

}


void operation::startTest(const char* pc, bool runNow)
{
    gTG.writeFormatBuf();
    gPDS.writeSCOption();
	//gFM.writeScanIndex(0);
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
    
    for (int i = 0; i <= PMON_DPS4; i++)
    {        
        if(!(m_flgDpsOn & (0x1 << i))) continue;     
        float min, max;
        gDPS.ReadChVout(i, min, max);        
        if (m_tStart + 5 < time(NULL)) { // Check Level after test Start 5 Sec 
            if (m_PMonArr[i].uv > min)
                gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d UV(%.2f/%.2f) Error", i + 1, m_PMonArr[i].uv, min);
            else if (m_PMonArr[i].ov < max)
                gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d OV(%.2f/%.2f) Error", i + 1, m_PMonArr[i].ov, max);
        }
        double ave = (min + max) / 2.0;
        m_PMonArr[i].rv = ave;
        m_PMonArr[i].rc =  gDPS.ReadChIout(i);
        
        if (m_PMonArr[i].rc > m_PMonArr[i].oc)
            gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d OC(%.2f/%.2f) Error", i + 1, m_PMonArr[i].oc, m_PMonArr[i].rc);
        
        unsigned short status = gDPS.GetLtcMfrPads(i);
        uint alert = gDPS.ReadAlertCtrl();
                
        if(status & 0x8000)
            gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d OV Status(0x%X) Error", i + 1, status);
        else if(status & 0x4000)
            gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d UV Status(0x%X) Error", i + 1, status);
        else if((status & 0x0003) != 0x3)
            gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d Fault Status(0x%X) Error", i + 1, status);
        else if (alert != 0) {
            gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d Current Fault(Alert) Read(0x%X) Error", i + 1, alert);
            uint i2c_num[4] = { 0, 2, 4, 5 };
            uint state_word =  gDPS.GetLtcStatusWord(DPS_SINGLE_SEL, i2c_num[i]);
            gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d STATUS_WORD(0x%X) Error", i + 1, state_word);

        }
        else if ((status & 0x300) != 0x300)
            gERR.addErr(ERR_POWER, __FUNCTION__, "DPS%d Power Not Good Status(0x%X) Error", i + 1, status);                
    }

    gNET.checkError();

    if(m_flgDpsOn & 0x10) {
        m_PMonArr[PMON_DPS5].rv = gDPS.ReadCvmVout();
        m_PMonArr[PMON_DPS5].rc = gDPS.ReadCvmIout();
    }
    
    if (gAPI.m_bMonDPS)
    {
        printf("name    sv      rv      oc      rc\n");
        printf("----------------------------------\n");
        char buf[128];
        int idx[5] = {0,1,4,2,3};
        int ch = 0;
        for (int i = 0; i <= PMON_DPS5; i++)
        {
            ch = idx[i];
            printf("dps%d   %.2f    %.2f    %.2f    %.2f\n",
                ch+1, m_PMonArr[ch].sv, m_PMonArr[ch].rv, m_PMonArr[ch].oc, m_PMonArr[ch].rc);
            sprintf(buf, "dps%d   %.2f    %.2f    %.2f    %.2f", i + 1, m_PMonArr[i].sv, m_PMonArr[i].rv, m_PMonArr[i].oc, m_PMonArr[i].rc);

            gNET.send_msg(MSG_TYPE_INFO, buf);
        }
        
    }
}

void operation::setPmonPE(int sel, int type, double value)
{
    switch (type)
    {
    case DR:	m_PMonArr[sel].dr = value; break;
    case IO:	m_PMonArr[sel].io = value; break;
    case SCAN:	m_PMonArr[sel].sc = value; break;
    case CLK:	m_PMonArr[sel].ck = value; break;
    case ALL:
        m_PMonArr[sel].dr = value;
        m_PMonArr[sel].io = value;
        m_PMonArr[sel].sc = value;
        m_PMonArr[sel].ck = value;
        break;
    default:    break;
    }
}

double operation::getMaxPELevel()
{
    double value = m_PMonArr[PMON_VIH].dr;
    value = fmax(value, m_PMonArr[PMON_VIH].io);
    value = fmax(value, m_PMonArr[PMON_VIH].sc);
    value = fmax(value, m_PMonArr[PMON_VIH].ck);

    return value;
}

double operation::getMinPELevel()
{
    double value = m_PMonArr[PMON_VIL].dr;
    value = fmin(value, m_PMonArr[PMON_VIL].io);
    value = fmin(value, m_PMonArr[PMON_VIL].sc);
    value = fmin(value, m_PMonArr[PMON_VIL].ck);

    return value;
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

        if (checkStat() == 0) break;   // pg run / stop check

        int sync = checkSync();      // sync check / id read
        if (sync > SYNC_USER_LAST && sync < SYNC_NONE)return sync;
            
        readDPS();
        if (!gAPI.m_bUseHandler)
        {                           
            readSysTemp();
            doScanProc();
        }
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

void operation::readSysTemp()
{
    // 10 s -> 2 Min
    if ((m_tTempCheck + 120) > (uint)time(NULL)) return;
    
    m_tTempCheck = (uint)time(NULL);
        
    if (m_flgDpsOn != 0) { // if stat of pe chip is not on, alarm flag set.  
                           // pe on flag judge dps power flag of sron
        int alarm = gIO.pe_alarm_read();
        if (alarm != 0)
        {
            for (int i = 0; i < 22; i++)
            {
                if (alarm & (1 << i))
                    gERR.addErr(ERR_POWER, "PETemp", "PE[%d] Temp Alarm Over 110C", i);
            }
        }
    }
    

    float bTempArr[4];
    
    for (int i = 0; i < 4; i++) {
        float fRead = gROM.read_temp102(i);        
        bTempArr[i] = fRead;
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

    time_t t = time(NULL);
    char ts[40];
    struct tm* tp = localtime(&t);
    strftime(ts, sizeof(ts), "%Y%m%dT%H%M%S", tp);
    gAPI.logBTemp("%s,%s,%.1f,%.1f,%.1f,%.1f", ts, gAPI.m_sCurStep,
        bTempArr[0], bTempArr[1], bTempArr[2], bTempArr[3]);
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

void operation::MakeCalFactor()
{
	stCalFactor calData;	
	calData.gain = 1.0;
	calData.offset = 0.0;
    calData.offset2 = 0.0;

    m_Adc0Cal.push_back(calData);
    m_Adc0Cal.push_back(calData);
    m_Adc0Cal.push_back(calData);
    m_Adc0Cal.push_back(calData);
    m_Adc0Cal.push_back(calData);
    m_Adc0Cal.push_back(calData);
    m_Adc0Cal.push_back(calData);

    m_Adc1Cal.push_back(calData);
    m_Adc1Cal.push_back(calData);
    m_Adc1Cal.push_back(calData);
    m_Adc1Cal.push_back(calData);
    m_Adc1Cal.push_back(calData);
    m_Adc1Cal.push_back(calData);
    m_Adc1Cal.push_back(calData);

	for(int idx = 0; idx < MAX_PE_CH ;idx++) {
		m_VIHCal.push_back(calData);
		m_VILCal.push_back(calData);
		m_VTHCal.push_back(calData);
	}

    for(int idx = 0; idx < MAX_DPS_NUM ;idx++) {
        m_DPSVSCal.push_back(calData);
        m_DPSIMCal.push_back(calData);
    }
}

void operation::ResetCalFactor()
{
	stCalFactor calData;	
	calData.gain = 1.0;
	calData.offset = 0.0;
    calData.offset2 = 0.0;

    m_Adc1Cal[0] = m_Adc0Cal[0] = calData;
    m_Adc1Cal[1] = m_Adc0Cal[1] = calData;
    m_Adc1Cal[2] = m_Adc0Cal[2] = calData;
    m_Adc1Cal[3] = m_Adc0Cal[3] = calData;
    m_Adc1Cal[4] = m_Adc0Cal[4] = calData;
    m_Adc1Cal[5] = m_Adc0Cal[5] = calData;
    m_Adc1Cal[6] = m_Adc0Cal[6] = calData;
	for(int idx = 0; idx < MAX_PE_CH ;idx++) {
		m_VIHCal[idx] = calData;
		m_VILCal[idx] = calData;
		m_VTHCal[idx] = calData;
	}
}


bool operation::ReadCalFactor()
{
    float data[MAX_PE_CH];
    uint16_t addr = CAL_ADC0_ADDR;
    uint32_t size = sizeof(float) * 8;

    FILE * g_pFileCalData = NULL;
    //g_pFileCalData = fopen("/star/diag/cal/cal.data", "rb");
    g_pFileCalData = fopen("/star/diag/cal/pecal.data", "rb");

    if(g_pFileCalData == NULL){
        printf("Cal data file open fail.\n");
    }

    // ADC Cal gain read
    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < 8; idx++) {
        if(data[idx] > 0.95 && data[idx] < 1.05)
            m_Adc0Cal[idx].gain = data[idx]; 
        else
            m_Adc0Cal[idx].gain = 1.0;
        //printf("adc 0 gain : %0.5f\n", m_Adc0Cal[idx].gain);
    }

    // ADC Cal offset read
    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        addr += size;
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < 8; idx++) {
        if(m_Adc0Cal[idx].gain != 1.0)
            m_Adc0Cal[idx].offset = data[idx]; 
        else
            m_Adc0Cal[idx].offset = 0.0;
        //printf("adc 0 offset : %0.5f\n", m_Adc0Cal[idx].offset);
    }

    // PE DPS VCC cal gain
    m_peDpsVccCal.gain = m_Adc0Cal[6].gain;
    m_peDpsVccCal.offset = m_Adc0Cal[6].offset;

    // ADC1 Cal gain read
    addr = CAL_ADC1_ADDR;
    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < 8; idx++) {
        if(data[idx] > 0.95 && data[idx] < 1.05)
            m_Adc1Cal[idx].gain = data[idx]; 
        else
            m_Adc1Cal[idx].gain = 1.0;

        //printf("adc 1 gain : %0.5f\n", m_Adc1Cal[idx].gain);
    }
    // ADC1 Cal offset read
    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        addr += size;
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < 8; idx++) {
        if(m_Adc1Cal[idx].gain != 1.0)
            m_Adc1Cal[idx].offset = data[idx]; 
        else
            m_Adc1Cal[idx].offset = 0.0;

        //printf("adc 1 offset : %0.5f\n", m_Adc1Cal[idx].offset);
    }

    // VIL Cal gain read
    size = sizeof(float) * MAX_PE_CH;
    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        addr = CAL_PE_VIL_ADDR;
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < MAX_PE_CH; idx++) {
        if(data[idx] < 1.05 && data[idx] > 0.95)
            m_VILCal[idx].gain = data[idx];
        else 
            m_VILCal[idx].gain = 1.0;

    //printf("CH %03d VIL gain : %0.5f\n", idx, data[idx]);
    }

    // VIL Cal offset read
    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        addr += size;
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < MAX_PE_CH; idx++) {
        if(m_VILCal[idx].gain != 1.0)
            m_VILCal[idx].offset = data[idx];
        else 
            m_VILCal[idx].offset = 0.0;

        //printf("CH %03d VIL offset : %0.5f\n", idx, data[idx]);
    }

    // VIH Cal gain read
    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        addr = CAL_PE_VIH_ADDR;
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < MAX_PE_CH; idx++) {
        if(data[idx] < 1.05 && data[idx] > 0.95)
            m_VIHCal[idx].gain = data[idx];
        else 
            m_VIHCal[idx].gain = 1.0;

         //pprintf("CH %03d VIH gain : %0.5f\n", idx, data[idx]);
    }

    // VIH Cal offset read
    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        addr += size;
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < MAX_PE_CH; idx++) {
        if(m_VIHCal[idx].gain != 1.0)
            m_VIHCal[idx].offset = data[idx];
        else 
            m_VIHCal[idx].offset = 0.0;

        //printf("CH %03d VIL offset : %0.5f\n", idx, data[idx]);
    }

    if(g_pFileCalData) {
        fclose(g_pFileCalData);
        g_pFileCalData = NULL;
    }

    ///////////// DPS Cal Data read //////////////////
    g_pFileCalData = fopen("/star/diag/cal/dpscal.data", "rb");

    if(g_pFileCalData == NULL){
        printf("DPS Cal data file open fail.\n");
        return false;
    }

    fseek(g_pFileCalData, 0, SEEK_END);
    long pos = ftell(g_pFileCalData);
    if(pos != 80) {
        printf("DPS Cal data file is not valid.\n");
        fclose(g_pFileCalData);
        return false;
    }

    fseek(g_pFileCalData, 0, SEEK_SET);

    addr = CAL_DPS_ADDR;
    size = sizeof(float) * 4;
    // DPS VS Cal gain read
    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < 4; idx++) {
        if(data[idx] > 0.95 && data[idx] < 1.05)
            m_DPSVSCal[idx].gain = data[idx]; 
        else
            m_DPSVSCal[idx].gain = 1.0;
        //printf("dps %03d VS gain : %0.5f\n", data[idx]);
    }

    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        addr += size;
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < 4; idx++) {
        if(m_DPSVSCal[idx].gain != 1.0)
            m_DPSVSCal[idx].offset = data[idx];
        else 
            m_DPSVSCal[idx].offset = 0.0;
        //printf("dps %03d VS offset : %0.5f\n", idx, data[idx]);
    }

    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        addr += size;
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < 4; idx++) {
        if(m_DPSVSCal[idx].gain != 1.0)
            m_DPSVSCal[idx].offset2 = data[idx];
        else 
            m_DPSVSCal[idx].offset2 = 0.0;

        //printf("CH %03d VIL offset : %0.5f\n", idx, data[idx]);
    }

    // DPS IM Cal gain read
    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < 4; idx++) {
        if(data[idx] > 0.75 && data[idx] < 1.10)
            m_DPSIMCal[idx].gain = data[idx]; 
        else
            m_DPSIMCal[idx].gain = 1.0;
        //printf("dps %03d VS gain : %0.5f\n", data[idx]);
    }

    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        addr += size;
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    for(int idx = 0; idx < 4; idx++) {
        if(m_DPSIMCal[idx].gain != 1.0)
            m_DPSIMCal[idx].offset = data[idx];
        else 
            m_DPSIMCal[idx].offset = 0.0;
        //printf("dps %03d VS offset : %0.5f\n", idx, data[idx]);
    }

    if(g_pFileCalData)
        fclose(g_pFileCalData);

    g_pFileCalData = fopen("/star/diag/cal/vs3cal.data", "rb");

    if(g_pFileCalData == NULL){
        printf("VS3 Cal data file open fail.\n");
    }

    // VS3 Cal gain read
    size = sizeof(float);
    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        addr = CAL_VS3_ADDR;
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    if(data[0] > 0.90 && data[0] < 1.1) {
        m_DPSVSCal[4].gain = data[0]; 
    }else{
        m_DPSVSCal[4].gain = 1.0;
        //printf("VS3 0 gain : %0.5f\n", m_Adc0Cal[idx].gain);
    }

    if(g_pFileCalData != NULL){
        fread(data, size, 1, g_pFileCalData);
    }else {
        addr += size;
        gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, size, (uint8_t *)data);
    }
    if(m_DPSVSCal[4].gain != 1.0)  {
        m_DPSVSCal[4].offset = data[0]; 
    }else{
        m_DPSVSCal[4].offset = 0.0;
    }

    //printf("VS3 gain : %0.5f\n", m_DPSVSCal[4].gain);
    //printf("VS3 offset : %0.5f\n", m_DPSVSCal[4].offset);

    if(g_pFileCalData)
        fclose(g_pFileCalData);

    return true;
}

void operation::pe_set_cal_data(int type, int ch, float gain, float offset )
{
	if(type == 0) {// VIH
		m_VIHCal[ch].gain = gain;
		m_VIHCal[ch].offset = offset;
	}else if(type == 1) {// VIL
		m_VILCal[ch].gain = gain;
		m_VILCal[ch].offset = offset;
	}else if(type == 2) {// VTH
		m_VTHCal[ch].gain = gain;
		m_VTHCal[ch].offset = offset;
	}
}

bool operation::dpsPowerSeq(int nSeq, unsigned short chBit, float delay)
{
    if(nSeq < 0 || nSeq > 6) {
        return false;
    }

    if(nSeq == 0) {
        stPowerSeq seq;
        seq.chBit = 0;
        seq.delay = 0;
        m_PSeq.clear();
        m_PSeq.push_back(seq);
        m_PSeq.push_back(seq);
        m_PSeq.push_back(seq);
        m_PSeq.push_back(seq);
        m_PSeq.push_back(seq);
        m_PSeq.push_back(seq);
    }

    stPowerSeq seq;
    seq.chBit = chBit;
    seq.delay = (delay*1000000);

    m_PSeq[nSeq] = seq;

    printf("Set Seq(%d) Sel(0x%X) Delay(%f)\n", nSeq, m_PSeq[nSeq].chBit, m_PSeq[nSeq].delay);

    return true;
}
