using HandyControl.Tools.Extension;
using HTCRack_CXL.Base;
using HTCRack_CXL.Net;
using HTCRack_CXL.ViewModel;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace HTCRack_CXL.Model
{
    public enum BOX_STAT
    {
        STOP = 0,
        PREV_STOP,
        PREV_RUN,
        RUN,
        DONE,
        TUNNING,
        ERROR,
        DISCONNECT,
        INSPECTION,
        DEFROST,
    }
    public enum DEFROST_TYPE
    {
        NORMAL = 0,
        HUMIDITY,
        ALARM,
        LOWTEMP,
        TAMS,
        TEMPERROR
    }
    public enum TCON_STAT : int
    {
        COOLING = 1,
        HEATING = 2,
        TUNNING = 4,
        MANUAL = 8,
        RUN = 0x10,
        ADJUST_ERROR = 0x20,
        PLUS_BURNOUT = 0x40,    // Real Error    
        MINUS_BURNOUT = 0x80,    // Real Error
        PLUS_OVER = 0x100,      // Real Error
        MINUS_OVER = 0x200,     // Real Error
        AT_ERROR = 0x400,
        COMM_ERROR = 0x800,
        RJC_ERROR = 0x1000,
        EEPROM_ERROR = 0x2000,
        FIRMWARE_ERROR = 0x4000,
        SYSTEM_ERROR = 0x8000
    }
    public class BoxModel : BaseVM
    {
        public string Name { get; set; }
        [DoNotNotify] public int BoxIndex { get; set; } = 0;

        public SolidColorBrush BoarderColor { get; set; }
        public SolidColorBrush BoxColor { get; set; } = Global.gRackColors[0];
        public SolidColorBrush HumRateColor { get; set; } = Global.gWarningColors[0];

        ObservableCollection<SiteModel> _sites = new ObservableCollection<SiteModel>();
        public ObservableCollection<SiteModel> Sites
        {
            get { return _sites; }
            set { _sites = value; PC(nameof(Sites)); }
        }
        [DoNotNotify]
        public RackModel MasterRack { get; set; }

        BOX_STAT _boxStat;
        public BOX_STAT BoxStat
        {
            get => _boxStat;
            set
            {
                _boxStat = value;
                BoarderColor = Global.gBoarderColors[(int)_boxStat];
                if (_boxStat == BOX_STAT.PREV_RUN) //Boxs.ForEach(bm => bm.BoxStat = BOX_STAT.PREV_RUN);
                {
                    DefostTime = string.Empty;
                    if (BoxShowBox == Visibility.Visible && BoxMsgColor == Global.gBoxAlramColors[0])
                        BoxShowBox = Visibility.Hidden;
                }
                else if (_boxStat == BOX_STAT.ERROR)
                    foreach (SiteModel sm in Sites)
                        sm.SiteStat = SITE_STAT.ERROR;

            }
        }
        public Visibility BoxShowBox { get; set; } = Visibility.Hidden;
        public string BoxMsg { get; set; } = "";
        public SolidColorBrush BoxMsgColor { get; set; }
        public void showBoxAlarm(ALRAM_TYPE type, string msg, bool isLogging)
        {
            BoxMsgColor = Global.gBoxAlramColors[(int)type];
            BoxMsg = msg;
            BoxShowBox = Visibility.Visible;
            if (isLogging)
                LogHelper.Error($"[Prog][{MasterRack.Name}-{Name}]", $"{msg}");
        }
        public DEFROST_TYPE BoxDefrost { get; set; } = DEFROST_TYPE.NORMAL;
        public string DefostTime { get; set; } = string.Empty;

        public bool isLoggingHumidity = false;
        public double HumTempSensorValue { get; set; } = 0;
        public double Hum_TempSenorVal { get; set; } = 0;
        public double HumidityOffset { get; set; } = 0;
        double? _humidityValue = null;
        public double? HumidityValue
        {
            get => _humidityValue;
            set
            {
                _humidityValue = value;
            }
        }
        double? PreHumidityValue = 0;
        public void SetHumidityValue(double value)
        {
            if (isLoggingHumidity)
            {
                PreHumidityValue = _humidityValue;
                HumidityValue = value;
                HumidityValueCheck();
            }
        }
        int humidityWarningCnt = 0;
        void HumidityValueCheck()
        {
            //0 ~ 99까지 습도값으로 판단 100 이상은 쓰레기값으로 
            if (_humidityValue + HumidityOffset < 0)
                _humidityValue = 0;
            else if (_humidityValue + HumidityOffset >= 100)
            {
                humidityWarningCnt++;
                _humidityValue = PreHumidityValue;

                if (humidityWarningCnt > 10) // 10번에 1번만 Warning Msg
                {
                    //Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Box" + BoxViewIndex + "]  =  HumidityValue 100 Over");
                    humidityWarningCnt = 0;
                }
                //_humidityValue = 100;
            }
            else
            {
                _humidityValue += HumidityOffset;
            }

            if (_humidityValue > (Global.gConfigVM.humidityWarnval - 10))
                HumRateColor = Global.gWarningColors[1];
            else
                HumRateColor = Global.gWarningColors[0];

            if (_humidityValue > Global.gConfigVM.humidityWarnval)
            {
                if (BoxStat == BOX_STAT.RUN) // humidityWarnval 넘어가면 제상스탑
                {
                    LogStep($"humidityValue {_humidityValue}");
                    //todo
                    BoxDefrost = DEFROST_TYPE.HUMIDITY;
                    Thread.Sleep((BoxIndex % 6) * 100);
                    isLoggingHumidity = false;
                    //습도 제상 루틴
                    Task.Run(() => DefrostStep());
                }
            }
        }

        public BoxModel(string name, int index, RackModel master) 
        {
            Name = $"Box {name}";
            BoxIndex = index;
            MasterRack = master;

            BoxStat = BOX_STAT.DISCONNECT;
        }
        public async Task<bool> BoxStopTest()
        {
            if(BoxStat != BOX_STAT.RUN)
                return false;
            else
            {
                isLoggingHumidity = false;
                return await DefrostStep();
            }
        }
        private CancellationTokenSource _defrostCts;
        public async Task<bool> DefrostStep()
        {
            LogStep("DefrostStep In");

            _defrostCts = new CancellationTokenSource();
            CancellationToken token = _defrostCts.Token;

            bool isCanceled = false;
            bool rtnStep = false;

            BoxStat = BOX_STAT.DEFROST;
            foreach (SiteModel sm in Sites)
                sm.SiteStat = SITE_STAT.DEFROST;

            const int DefaultTargetTemp = 60;
            const int DefaultStepTemp = 1;
            const int DefaultStepWaitTimeSec = 60;
            //const int DefaultTargetWaitSec = 20 * 60;
            const int FinalCoolWaitTimeSec = 180;
            const int CoolingTargetTemp = 40;

            int targetTemp = Global.gConfigVM?.TargetTemp ?? DefaultTargetTemp;
            int stepTemp = Global.gConfigVM?.TempStep ?? DefaultStepTemp;
            int stepWaitSec = Global.gConfigVM?.TempStepTime ?? DefaultStepWaitTimeSec;
            int targetWaitSec = (Global.gConfigVM?.TargetWaitTime ?? 20) * 60;

            double currentTemp = getGetPvList().Min();
            LogStep($"Box Temp = {currentTemp}");

            if (currentTemp <= -10.0) currentTemp = -9.0;

            double tempToReach = Math.Truncate(currentTemp) * 10;

            try
            {
                if (tempToReach < targetTemp * 10)
                {
                    while (BoxStat == BOX_STAT.DEFROST)
                    {
                        tempToReach += stepTemp * 10;
                        LogStep($"Temp = {tempToReach}");

                        sendSV(tempToReach);
                        await Task.Delay(100, token);

                        if (tempToReach >= targetTemp * 10)
                        {
                            LogStep($"Target Temp is OK = {tempToReach}");
                            sendSV(tempToReach);
                            rtnStep = await WaitWithCountdown(targetWaitSec, token);
                            break;
                        }

                        await Task.Delay(stepWaitSec * 1000, token);
                    }
                    if (BoxStat != BOX_STAT.DEFROST)
                    {
                        LogStep("Defrost 중단 감지됨");
                        isCanceled = true;
                    }
                }
                else
                    LogStep($"NO Defrost = {tempToReach}");

                if (!isCanceled)
                {
                    LogStep("Temp Down = 40");
                    sendSV(CoolingTargetTemp * 10);
                    rtnStep = await WaitWithLog(FinalCoolWaitTimeSec, token);

                    LogStep("Defrost Done");

                    rtnStep = FinalizeDefrost();
                }
                else rtnStep = false;
            }
            catch(TaskCanceledException)
            {
                LogStep("Defrost 중단됨 (Canceled)");
                isCanceled = true;
                rtnStep = false;
            }
            finally
            {
                _defrostCts?.Dispose();
                _defrostCts = null;
            }

            return rtnStep;
        }
        private async Task<bool> WaitWithCountdown(int seconds, CancellationToken token)
        {
            /*
            while (totalSec > 0 && BoxStat == BOX_STAT.DEFROST)
            {
                DefostTime = $"{totalSec / 60:D2} : {totalSec % 60:D2}";
                await Task.Delay(1000);
                totalSec--;
            }
            */
            while (seconds-- > 0)
            {
                token.ThrowIfCancellationRequested();

                if (BoxStat != BOX_STAT.DEFROST)
                {
                    LogStep("Defrost canceled during countdown.");
                    return false;
                }
                DefostTime = $"{seconds / 60} : {seconds % 60}";
                await Task.Delay(1000, token);
            }
            DefostTime = "Done";
            return true;
        }
        private async Task<bool> WaitWithLog(int durationSec, CancellationToken token)
        {
            for (int i = 0; i < durationSec / 10; i++)
            {
                token.ThrowIfCancellationRequested();

                if (BoxStat != BOX_STAT.DEFROST)
                {
                    LogStep("Defrost canceled during cooling wait.");
                    return false;
                }

                await Task.Delay(10 * 1000, token);
                LogStep($"Wait Timer : {(i + 1) * 10}");
            }
            return true;
            /*
            int elapsed = 0;
            while (elapsed < durationSec && BoxStat == BOX_STAT.DEFROST)
            {
                await Task.Delay(10 * 1000);
                elapsed += 10;
                LogStep($"Wait Timer : {elapsed}");
            }
            */
        }
        private bool FinalizeDefrost()
        {
            bool isCanceled = BoxStat == BOX_STAT.DEFROST;

            if (isCanceled)
            {
#if TEST_MODE
                //BoxDefrost = DEFROST_TYPE.ALARM;
#else
                double CenterTemp = 25.0;
                if (getGetPvList().Min() < CenterTemp)
                    BoxDefrost = DEFROST_TYPE.LOWTEMP;
#endif

                if (BoxDefrost == DEFROST_TYPE.HUMIDITY || BoxDefrost == DEFROST_TYPE.ALARM || BoxDefrost == DEFROST_TYPE.TEMPERROR)
                {
                    sendStop();
                    sendFan(50);

                    string msg;
                    switch (BoxDefrost)
                    {
                        case DEFROST_TYPE.HUMIDITY:
                            msg = "습도제상";
                            break;
                        case DEFROST_TYPE.ALARM:
                            msg = "이상온도 발생";
                            break;
                        case DEFROST_TYPE.TEMPERROR:
                            msg = "온도동작 이상감지";
                            break;
                        default:
                            msg = "기타 에러";
                            break;
                    }

                    showBoxAlarm(ALRAM_TYPE.ERROR, msg, true);
                    BoxStat = BOX_STAT.ERROR;
                }
                else if (BoxDefrost == DEFROST_TYPE.LOWTEMP)
                {
                    showBoxAlarm(ALRAM_TYPE.ERROR, "온도 25도이하", true);
                    BoxStat = BOX_STAT.ERROR;
                }
            }

            sendStop();
            sendFan(50);

            LogStep("DefrostStep Out");

            return isCanceled;
        }
        private void LogStep(string message)
        {
            LogHelper.Info( $"[Prog][{MasterRack.Name}-{Name}]", $"{message}");
        }
        public async Task BoxStatusClear()
        {
            LogStep("Box Status Clear In");

            _defrostCts?.Cancel();
            showBoxAlarm(ALRAM_TYPE.INIT, "Box Status Clear", true);

            if (BoxStat == BOX_STAT.DEFROST)
            {
                BoxStat = BOX_STAT.STOP;
                await Task.Delay(60 * 1000);
            }
            else
            {
                BoxStat = BOX_STAT.STOP;
                await Task.Delay(30 * 1000);
            }
            
            foreach (SiteModel sm in Sites)
                sm.SiteStat = SITE_STAT.STOP;

            BoxShowBox = Visibility.Hidden;
            BoxDefrost = DEFROST_TYPE.NORMAL;
            isLoggingHumidity = false;
            DefostTime = string.Empty;

            LogStep("Box Status Clear Out");
        }
        public double[] getGetPvList()
        {
            double[] getSiteGetPv = { 0, 0 };

            for (int i = 0; i < 2; i++)
                getSiteGetPv[i] = (double)Sites[i].TConGetPV;

            return getSiteGetPv;
        }
        public void SampleprocessBoxST()
        {
            IList<RegModel> tcons = (IList<RegModel>)MasterRack.TConRegs;

            string warnMsg = "";
            int runSum = 0;
            int tuneSum = 0;
            int errSum = 0;
            int svCount = 0;

            foreach (SiteModel sm in Sites)
            {
                foreach (int idx in sm.SVCtrlList)
                {
                    int chStat = 0x0;
                    if (BoxIndex == 0)
                        chStat = 0x10;
                    else if (BoxIndex == 1)
                        chStat = 0x10;
                    if ((chStat & (int)TCON_STAT.RUN) != 0)
                    {
                        sm.TConIsRun = null;
                        sm.TConIsRun = true;
                        runSum++;
                    }
                    else if ((chStat & (int)TCON_STAT.RUN) == 0)
                    {
                        sm.TConIsRun = null;
                        sm.TConIsRun = false;
                    }
                    if ((chStat & (int)TCON_STAT.TUNNING) != 0)
                        tuneSum++;
                    //0619 Error ChStat 수정
                    if ((chStat & ((int)TCON_STAT.PLUS_BURNOUT
                                    | (int)TCON_STAT.MINUS_BURNOUT
                                    | (int)TCON_STAT.PLUS_OVER
                                    | (int)TCON_STAT.MINUS_OVER)) != 0)
                    {
                        if ((chStat & (int)TCON_STAT.PLUS_BURNOUT) != 0)
                            warnMsg += TCON_STAT.PLUS_BURNOUT.ToString();
                        if ((chStat & (int)TCON_STAT.MINUS_BURNOUT) != 0)
                            warnMsg += TCON_STAT.MINUS_BURNOUT.ToString();
                        if ((chStat & (int)TCON_STAT.PLUS_OVER) != 0)
                            warnMsg += TCON_STAT.PLUS_OVER.ToString();
                        if ((chStat & (int)TCON_STAT.MINUS_OVER) != 0)
                            warnMsg += TCON_STAT.MINUS_OVER.ToString();
                        errSum++;
                    }
                }
                svCount += sm.SVCtrlList.Count;
            }

            if (errSum > 0)
            {
                LogStep($"{warnMsg}");
                BoxStat = BOX_STAT.ERROR;
                return;
            }

            if (runSum == 0) BoxStat = BOX_STAT.STOP;
            else if (runSum == svCount)
            {
                if (BoxStat != BOX_STAT.RUN && BoxStat != BOX_STAT.DEFROST)
                    BoxStat = BOX_STAT.RUN;
            }
        }
        public void processBoxST()
        {
            IList<RegModel> tcons = (IList<RegModel>)MasterRack.TConRegs;

            string warnMsg = "";
            int runSum = 0;
            int tuneSum = 0;
            int errSum = 0;
            int svCount = 0;

            foreach (SiteModel sm in Sites)
            {
                foreach (int idx in sm.SVCtrlList)
                {
                    int chStat = (int)tcons[RegMap.TCon_GetCHSTS + idx].ReadValue;
                    if ((chStat & (int)TCON_STAT.RUN) != 0)
                    {
                        sm.TConIsRun = null;
                        sm.TConIsRun = true;
                        runSum++;
                    }
                    else if ((chStat & (int)TCON_STAT.RUN) == 0)
                    {
                        sm.TConIsRun = null;
                        sm.TConIsRun = false;
                    }

                    if ((chStat & (int)TCON_STAT.TUNNING) != 0)
                        tuneSum++;
                    //0619 Error ChStat 수정
                    if ((chStat & ((int)TCON_STAT.PLUS_BURNOUT
                                    | (int)TCON_STAT.MINUS_BURNOUT
                                    | (int)TCON_STAT.PLUS_OVER
                                    | (int)TCON_STAT.MINUS_OVER)) != 0)
                    {
                        if ((chStat & (int)TCON_STAT.PLUS_BURNOUT) != 0)
                            warnMsg += TCON_STAT.PLUS_BURNOUT.ToString();
                        if ((chStat & (int)TCON_STAT.MINUS_BURNOUT) != 0)
                            warnMsg += TCON_STAT.MINUS_BURNOUT.ToString();
                        if ((chStat & (int)TCON_STAT.PLUS_OVER) != 0)
                            warnMsg += TCON_STAT.PLUS_OVER.ToString();
                        if ((chStat & (int)TCON_STAT.MINUS_OVER) != 0)
                            warnMsg += TCON_STAT.MINUS_OVER.ToString();
                        errSum++;
                    }
                }
                svCount += sm.SVCtrlList.Count;
            }

            if (errSum > 0)
            {
                LogStep($"{warnMsg}");
                BoxStat = BOX_STAT.ERROR;
                return;
            }

            //todo 

            if (runSum == 0) BoxStat = BOX_STAT.STOP;
            else if (runSum == svCount)
            {
                if(BoxStat != BOX_STAT.RUN && BoxStat != BOX_STAT.DEFROST)
                    BoxStat = BOX_STAT.RUN;
            }
            /*
            else BoxStat = BOX_STAT.ERROR;

            if (tuneSum > 0) BoxStat = BOX_STAT.TUNNING;
            */
        }

        public void sendFan(SiteModel sm, int percent)
        {
            foreach (int index in sm.FanCtrlList)
            {
                MasterRack.sendFanDuty(index, percent);
            }
        }
        public void sendFan(int percent)
        {
            foreach (SiteModel sm in Sites)
            {
                sendFan(sm, percent);
            }
        }
        public void sendBS1(SiteModel sm, double set)
        {
            short pvOffset = (short)(set * 10.0);
            //Console.WriteLine($"{Name} SV : {sv}");
            foreach (int index in sm.SVCtrlList)
            {
                HtcMessage msg = Global.newMsg(0xff, index, 0x30);
                msg.data = BitConverter.GetBytes(pvOffset);
                MasterRack._client.pushSendQue(REG_TYPE.TCON, msg);
            }
        }
        public void sendSV(SiteModel sm, double set)
        {
            short sv = (short)(set * 10.0);
            foreach (int index in sm.SVCtrlList)
            {
                HtcMessage msg = Global.newMsg(0xff, index, 0x10);
                msg.data = BitConverter.GetBytes(sv);
                MasterRack._client.pushSendQue(REG_TYPE.TCON, msg);
            }
        }
        public void sendSV(double set)
        {
            if (BoxStat == BOX_STAT.DEFROST) return;
            foreach (SiteModel sm in Sites)
                foreach (int index in sm.SVCtrlList)
                    sendSV(sm, set);
        }
        public void sendRun()
        {
            short ctrl = 1;
            foreach (SiteModel sm in Sites)
            {
                foreach (int index in sm.SVCtrlList)
                {
                    HtcMessage msg = Global.newMsg(0xff, index, 0x11);
                    msg.data = BitConverter.GetBytes(ctrl);
                    MasterRack._client.pushSendQue(REG_TYPE.TCON, msg);
                }
            }
        }
        public void sendStop()
        {
            short ctrl = 0;
            foreach (SiteModel sm in Sites)
            {
                foreach (int index in sm.SVCtrlList)
                {
                    HtcMessage msg = Global.newMsg(0xff, index, 0x11);
                    msg.data = BitConverter.GetBytes(ctrl);
                    MasterRack._client.pushSendQue(REG_TYPE.TCON, msg);
                }
            }
        }      
        public void processBoxCxl()
        {
            IList<TempDataModel> cxltemp = (IList<TempDataModel>)MasterRack.TempDatas;

            Random rnd = new Random();

            foreach (SiteModel sm in Sites)
            {
                IList<RegModel> tcons = (IList<RegModel>)MasterRack.TConRegs;

                for(int i = 0; i < sm.PVCtrlList.Count; i++) 
                {
#if TEST_MODE
                    double getTconPV = Math.Round(GetSecureRandomDouble(10.0, 90.0), 1);
#else
                    int index = RegMap.TCon_GetPV;
                    if (index >= tcons.Count)
                        continue;

                    double getTconPV = tcons[index].ReadValue;
#endif

#if TEST_MODE
                    Console.WriteLine($"{BoxIndex}- {sm.SiteIndex} temp {getTconPV}");
#endif

                    sm.TConGetPV = getTconPV;
                    MasterRack.TempDatas[(BoxIndex * 2) + sm.SiteIndex].PVData[MasterRack.GraphIndex] = getTconPV;
                    cxltemp[(BoxIndex  * 2) + sm.SiteIndex].TempPV = getTconPV;
                }
            }
        }
        public double GetSecureRandomDouble(double min, double max)
        {
            byte[] bytes = new byte[8];

            using (var rng = RandomNumberGenerator.Create())
            {
                rng.GetBytes(bytes);
            }

            ulong ulongRand = BitConverter.ToUInt64(bytes, 0);
            double normalized = (ulongRand / (double)ulong.MaxValue); // 0~1
            return min + (max - min) * normalized;
        }
    }
}
