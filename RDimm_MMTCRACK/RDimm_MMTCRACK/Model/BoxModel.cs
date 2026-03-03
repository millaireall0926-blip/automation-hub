using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.View.Component;
using RDimm_MMTCRACK.ViewModel;
using PropertyChanged;
using Serilog;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Threading;
using System.Xml.Linq;

namespace RDimm_MMTCRACK.Model
{
    public enum Box_STAT
    {
        IDLE = 0,
        STOP,
        PRERUN,
        RUN,
        DONE,
        DEFROST,
        ERROR
    }
    /*
    public enum DEFROST_TYPE
    {
        NORMAL = 0,
        HUMIDITY,
        ALARM,
        LOWTEMP,
        TAMS,
        TEMPERROR
    }
    */
    /*
    public enum TRADING
    {
        RISE,
        FALL
    }
    
    public enum SVC  //SV Ctrl
    {
        CLEAR = 0,
        TCON = 0x1,
        STABLE = 0x2,
        ACCELERATION = 0x4,
        NEXT = 0x8,

        DOWN = 0x10,
        UP = 0x20
    }
    */
    public class BoxModel : BaseVM
    {
        [DoNotNotify] public int BoxIndex { get; set; } = 0;
        [DoNotNotify] public int BoxViewIndex { get; set; } = 0;
        public SolidColorBrush BoarderColor { get; set;}
        public SolidColorBrush BoxColor { get; set; } = Global.gCBoxColors[0];
        public SolidColorBrush HumRateColor { get; set; } = Global.gWarningColors[0];
        public bool IsBoxTamsEnable { get; set; } = false;

        public Collection<double> SampleLongTerm = new Collection<double>();

        public Collection<double> SampleShortTerm = new Collection<double>();

        public TRADING PrevTrading = TRADING.RISE;

        public int LongTermCount = 10;
        public int ShortTermCount = 5;

        public int Level = 0;
        public int SVCtrl;

        bool _isboxtcunit = false;
        public bool IsBoxTCUnit
        {
            get => _isboxtcunit;
            set
            {
                _isboxtcunit = value;
                if (_isboxtcunit)
                {
                    BoxColor = Global.gBoxColors[0];
                    lock (lockThis)
                    {
                        if (MasterRack.BoxTCUnitCnt != 3)
                            MasterRack.BoxTCUnitCnt++;
                    }
                    if (Global.gConfigVM != null)
                        Global.gConfigVM.saveitems();
                }
                else
                {
                    BoxColor = Global.gBoxColors[2];
                    lock (lockThis)
                    {
                        if (MasterRack.BoxTCUnitCnt != 0)
                            MasterRack.BoxTCUnitCnt--;
                    }
                    if (Global.gConfigVM != null)
                        Global.gConfigVM.saveitems();
                }
                if (MasterRack.BoxTCUnitCnt == 3)
                    MasterRack.IsRackTCUnit = true;
                else if(MasterRack.BoxTCUnitCnt == 0)
                    MasterRack.IsRackTCUnit = false;
            }
        }
        bool _isboxAS = false;
        public bool IsBoxAS
        {
            get => _isboxAS;
            set
            {
                _isboxAS = value;
                if (_isboxAS)
                {
                    BoxColor = Global.gBoxColors[1];
                    lock (lockThis)
                        MasterRack.BoxASCnt++;

                    if (Global.gConfigVM != null)
                        Global.gConfigVM.saveitems();
                }
                else
                {
                    BoxColor = Global.gBoxColors[0];
                    lock (lockThis)
                        MasterRack.BoxASCnt--;

                    if (Global.gConfigVM != null)
                        Global.gConfigVM.saveitems();
                }

                if(MasterRack.BoxASCnt == 3)
                    MasterRack.IsRackAS = true;
                else if(MasterRack.BoxASCnt == 0)
                    MasterRack.IsRackAS = false;
            }
        }
        bool _isboxTams = true;
        public bool ISBoxTams
        {
            get => _isboxTams;
            set
            {
                _isboxTams = value;
                if (_isboxTams)
                {
                    IsReadOnlySV = true;
                    lock (lockThis)
                        MasterRack.BoxTamsCnt++;

                    if (Global.gConfigVM != null)
                        Global.gConfigVM.saveitems();

                }
                else
                {
                    IsReadOnlySV = false;
                    lock (lockThis)
                    {
                        if(MasterRack.BoxTamsCnt > 0)
                        MasterRack.BoxTamsCnt--;
                    }
                    if (Global.gConfigVM != null)
                        Global.gConfigVM.saveitems();
                }
                if (MasterRack.BoxTamsCnt == 3)
                    MasterRack.IsRackTams = true;
                else if (MasterRack.BoxTamsCnt == 0)
                    MasterRack.IsRackTams = false;
            }
        }
        int _humidityValue = 0;
        public int HumidityValue
        {
            get => _humidityValue;
            set
            {
                _humidityValue = value;
            }
        }
        int PreHumidityValue = 0;
        public void SetHumidityValue(int value)
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
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Box" + BoxViewIndex + "]  =  HumidityValue 100 Over");
                    humidityWarningCnt = 0;
                }
                //_humidityValue = 100;
            }
            else
            {
                _humidityValue = _humidityValue + HumidityOffset;
            }

            if (_humidityValue > (Global.gConfigVM.humidityWarnval - 10))
                HumRateColor = Global.gWarningColors[1];
            else
                HumRateColor = Global.gWarningColors[0];

            if (_humidityValue > Global.gConfigVM.humidityWarnval)
            {
                if (BoxStat == Box_STAT.RUN) // humidityWarnval 넘어가면 제상스탑
                {
                    BoxDefrost = DEFROST_TYPE.HUMIDITY;
                    Thread.Sleep((BoxIndex % 6) * 100);
                    isLoggingHumidity = false;
                    //습도 제상 루틴
                    Task.Run(() => DEFROSTStep());
                }
            }
        }
        public bool isLoggingHumidity = false;
        public int Hum_TempSenorVal { get; set; } = 0;
        public int HumidityOffset { get; set; } = 0;
        double _boxsetSV = 25;
        public double BoxSetSV
        {
            get => _boxsetSV;
            set
            {
                _boxsetSV = value;
            }
        }
        Box_STAT _boxStat;
        public Box_STAT BoxStat
        {
            get => _boxStat;
            set
            {
                _boxStat = value;
                BoarderColor = Global.gBoarderColors[(int)_boxStat];
                BoxStatText = _boxStat.ToString();
            }
        }
        public DEFROST_TYPE BoxDefrost { get; set; } = DEFROST_TYPE.NORMAL;
        bool TempAlarmFlag = false;
        bool TempErrorFlag = false;
        public string BoxPVText { get; set; } = string.Empty;
        public SolidColorBrush BoxPVColor { get; set; } = Global.gWarningColors[0];
        double _boxpv = 0;
        public double BoxPV 
        { 
            get => _boxpv;
            set
            {
                _boxpv = value;
            }
        }

        public double prevPV;
        public double zeroCnt = 0;
        public double abnomalCnt = 0;
        
        public void GetBoxPV(double value)
        {
            /*
            if (prevPV != 0 && value == 0)
            {
                zeroCnt++;
                BoxPV = prevPV;
            }
            else if (value != 0)
            {
                BoxPVColor = Global.gWarningColors[0];
                zeroCnt = 0;
                BoxPV = value;
                prevPV = value;
                BoxPVCheck();
                BoxPVText = BoxPV.ToString();
            }
            */

            if (prevPV != 0 && value == 0)
            {
                zeroCnt++;
                BoxPV = prevPV;
            }
            else if (value != 0)
            {
                if (prevPV != 0)
                {
                    double diffval = prevPV - value;
                    //Test용 Log 추가 삭제필요
                    //Global.gMainPageVM.addLog(LOG_TYPE.INFO, Math.Abs(diffval).ToString() + ", prePV = " + prevPV.ToString());
                    if (Math.Abs(diffval) > Global.gConfigVM.NoiseTempDiff)
                    {
                        BoxPV = prevPV;
                        abnomalCnt++;
                    }
                    else
                    {
                        BoxPV = value;
                        prevPV = value;
                        abnomalCnt = 0;
                    }
                }
                else
                {
                    BoxPV = value;
                    prevPV = value;
                    abnomalCnt = 0;
                }
                BoxPVColor = Global.gWarningColors[0];
                zeroCnt = 0;
                BoxPVCheck();
                BoxPVText = BoxPV.ToString();

                if (abnomalCnt > 4)
                {
                    BoxPVColor = Global.gWarningColors[1];
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Box" + BoxViewIndex + "] Abnomal Dimm Temp =  " + value);
                }
            }

            if (zeroCnt > 3)
            {
                BoxPV = value;
                prevPV = value;
                BoxPVText = string.Empty;//BoxPV.ToString();
            }
        }
        int StableAlarmCnt = 0;
        public void BoxPVCheck()
        {
            if (BoxStat == Box_STAT.RUN)
            {
                if (BoxPV == 0)
                    return;
                //Stable Time after Set Temp 보다 +-10도 차이나면 이상으로 알람 후 10번이상 연속이면 멈춤
                if (TimeStart.AddMinutes(Global.gConfigVM.StableTime) < DateTime.Now)
                {
                    double stableDiff = Math.Abs(BoxSetSV - BoxPV);

                    if (stableDiff >= 10)
                        StableAlarmCnt++;
                    else
                        StableAlarmCnt = 0;

                    if (StableAlarmCnt >= 10)
                    {
                        BoxDefrost = DEFROST_TYPE.TEMPERROR;
                        Thread.Sleep((BoxIndex % 6) * 100);
                        isLoggingHumidity = false;
                        Task.Run(() => DEFROSTStep());
                    }
                }

                //High , Low Temp Error Part

                if (Global.gConfigVM.HighTempAlarm < _boxpv || Global.gConfigVM.LowTempAlarm > _boxpv)
                {
                    if (TempAlarmFlag)
                    {
                        if (TempAlarmTime == null)
                        {
                            //Console.WriteLine("{0} : Box {1} Alarm Check Start", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                            TempAlarmTime = DateTime.Now.AddMinutes(Global.gConfigVM.AlarmCondition);
                        }
                        else
                        {
                            if (TempAlarmTime < DateTime.Now)
                            {
                                //Console.WriteLine("{0} : Box {1} Alarm Check Over", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[B" + BoxViewIndex + "] HighTemp = " + Global.gConfigVM.HighTempAlarm.ToString() +
                            " , LowTemp = " + Global.gConfigVM.LowTempAlarm.ToString() + " , Cur = " + _boxpv.ToString());
                            }
                            else
                            {
                                //Console.WriteLine("{0} : Box {1} Alarm Check Waitting", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                            }
                        }
                    }
                    else
                    {
                        //Console.WriteLine("{0} : Box {1}  TempTimeStart = null", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                        TempAlarmFlag = true;
                        TempAlarmTime = null;
                    }
                }
                else
                {
                    //Console.WriteLine("{0} : Box {1} TempAlarmFlag = false", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                    TempAlarmFlag = false;
                    TempAlarmTime = null;
                }

                //Console.WriteLine("Global.gConfigVM.HighTempAlarm + Global.gConfigVM.TempErr = {0} , Global.gConfigVM.LowTempAlarm - Global.gConfigVM.TempErr = {1}", Global.gConfigVM.HighTempAlarm + Global.gConfigVM.TempErr, Global.gConfigVM.LowTempAlarm - Global.gConfigVM.TempErr);
                if ((Global.gConfigVM.HighTempAlarm + Global.gConfigVM.TempErr) < _boxpv || (Global.gConfigVM.LowTempAlarm - Global.gConfigVM.TempErr) > _boxpv)
                {
                    if (TempErrorFlag)
                    {
                        if (TempErrorTime == null)
                        {
                            //Console.WriteLine("{0} : Box {1} Error Check Start", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                            TempErrorTime = DateTime.Now.AddMinutes(Global.gConfigVM.TempErrCondition);
                        }
                        else
                        {
                            if (TempErrorTime < DateTime.Now)
                            {
                                //Console.WriteLine("{0} : Box {1} Error Check Over", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                                //Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[B" + BoxViewIndex + "] HighTemp = " + Global.gConfigVM.HighTempAlarm.ToString() + " , LowTemp = " + Global.gConfigVM.LowTempAlarm.ToString() + " , Cur = " + _getpv.ToString());
                                BoxDefrost = DEFROST_TYPE.ALARM;
                                Thread.Sleep((BoxIndex % 6) * 100);
                                isLoggingHumidity = false;
                                Task.Run(() => DEFROSTStep());
                            }
                            else
                            {
                                //Console.WriteLine("{0} : Box {1} Error Check Waitting", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                            }
                        }
                    }
                    else
                    {
                        TempErrorFlag = true;
                        TempErrorTime = null;
                    }
                }
                else
                {
                    TempErrorFlag = false;
                    TempErrorTime = null;
                }
            }
        }
        public DateTime TimeStart {  get; set; }
        public DateTime TimeEnd { get; set; }
        Nullable<DateTime> TempAlarmTime { get; set; }
        Nullable<DateTime> TempErrorTime { get; set; }
        public bool IsReadOnlySV { get; set; } = false;
        bool _bimtal_stat = true;
        public bool Bimetal_Stat 
        { 
            get => _bimtal_stat; 
            set
            {
                _bimtal_stat = value;
                if(_bimtal_stat == false)
                {
                    showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_bimetal, true , BoxIndex);
                    Task.Run(() => TCon_BoxStop());
                    if (BoxStat == Box_STAT.RUN || BoxStat == Box_STAT.DEFROST)
                    {
                        lock (lockThis)
                        {
                            //MasterRack.BoxRunCnt--;
                            Global.gMainPageVM.AllRackRunCnt --;
                            //Global.gMainPageVM.AllRackPreRunCnt--;
                        }
                    }
                    BoxStat = Box_STAT.ERROR;
                    //Console.WriteLine("BoxViewIndex [{0}] MasterRack.BoxRunCnt = {1}", BoxViewIndex, MasterRack.BoxRunCnt);
                    //바이메탈 alarm msg box 
                }
                else
                {
                    BoxStat = Box_STAT.STOP;
                    BoxShowBox = Visibility.Hidden;
                    //msg box Visibility false
                }
            }
        }
        public int Bimetal_Stat_Cnt { get; set; } = 0;
        bool _sopen_stat = true;
        public bool Sopen_Stat 
        { 
            get => _sopen_stat; 
            set
            {
                _sopen_stat = value;
                if( _sopen_stat == false)
                {
                    showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_sOpen, true, BoxIndex);
                    Task.Run(() => TCon_BoxStop());
                    if (BoxStat == Box_STAT.RUN || BoxStat == Box_STAT.DEFROST)
                    {
                        lock (lockThis)
                        {
                            //MasterRack.BoxRunCnt--;
                            Global.gMainPageVM.AllRackRunCnt --;
                            //Global.gMainPageVM.AllRackPreRunCnt--;
                        }
                    }
                    BoxStat = Box_STAT.ERROR;
                    //Console.WriteLine("BoxViewIndex [{0}] MasterRack.BoxRunCnt = {1}", BoxViewIndex, MasterRack.BoxRunCnt);
                    //sopen alarm msg box 
                }
                else
                {
                    BoxStat = Box_STAT.STOP;
                    BoxShowBox = Visibility.Hidden;
                    //msg box Visibility false
                }
            }
        }
        public Visibility BoxShowBox { get; set; } = Visibility.Hidden;
        public string BoxMsg { get; set; } = "";
        public SolidColorBrush BoxMsgColor { get; set; }
        public int Sopen_Stat_Cnt { get; set; } = 0;
        public bool TconIsRun { get; set; } = false;
        public double GetSV { get; set; } = 0.0;
        public double GetPV { get; set; } = 0.0;
        public string BoxStatText { get; set; } = string.Empty;
        public string DefostTime {  get; set; } = string.Empty;

        public bool PreRunControlTemp {  get; set; } = false;   

        public DelegateCommand BoxCmdSetTemp { get; set; }
        public RackModel MasterRack { get; set; }
        public static System.Object lockThis = new System.Object();
        public void showBoxAlarm(ALRAM_TYPE type, string msg , bool isLogging, int ch)
        {
            BoxMsgColor = Global.gAlramColors[(int)type];
            BoxMsg = msg;
            BoxShowBox = Visibility.Visible;
            if (isLogging)
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, "[B" + ch + "]" + msg);
        }
        public BoxModel(int ch, RackModel master) 
        {
            BoxViewIndex = (ch + 1);
            BoxIndex = ch;
            MasterRack = master;
            BoxStat = Box_STAT.STOP;
            IsBoxTCUnit = true;
            BoxCmdSetTemp = new DelegateCommand(p => { setTemp(); });
        }
        public void BoxStatusClear()
        {
            BoxShowBox = Visibility.Hidden;

            if (BoxStat == Box_STAT.RUN || BoxStat == Box_STAT.DEFROST)
            {
                lock (lockThis)
                {
                    //MasterRack.BoxRunCnt--;
                    Global.gMainPageVM.AllRackRunCnt--;
                    //Global.gMainPageVM.AllRackPreRunCnt--;
                }
            }
            BoxStat = Box_STAT.STOP;
            BoxDefrost = DEFROST_TYPE.NORMAL;
            DefostTime = string.Empty;
            Sopen_Stat = true;
            Bimetal_Stat = true;
        }
        public bool PreStartTest()
        {
            //Console.WriteLine("{0} : PreStartTest", BoxIndex);
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==> [B" + BoxViewIndex + "] Pre Start Test");
            if(BoxStat == Box_STAT.DONE)
            { 
                BoxShowBox = Visibility.Hidden;
                BoxStat = Box_STAT.STOP;
            }
            if (BoxStat != Box_STAT.IDLE && BoxStat != Box_STAT.STOP)
            {
                //msg 추가
                return false;
            }
            
            //slide open check 유무? 

            if (!IsBoxTCUnit || IsBoxAS || MasterRack.IsSlideOpen)
                return false;

            if (ISBoxTams)
            {
                if (!setTamsTemp())
                {
                    showBoxAlarm(ALRAM_TYPE.ERROR,Global.msg_TamsFile, true, BoxIndex);
                    return false;
                }
            }
            TempErrorTime = null;
            TempAlarmTime = null;
            BoxStat = Box_STAT.PRERUN;
            Bimetal_Stat_Cnt = 0;
            Sopen_Stat_Cnt = 0;
            HumidityValue = 0;
            DefostTime = string.Empty;
            isLoggingHumidity = false;
            //Errmsg.log" ?? 필요 유무?
            return true;
        }
        /*
        public async void startTest()
        {
            if (BoxStat != Box_STAT.PRERUN)
                return;

            lock (lockThis)
            {
                if (Global.gMainPageVM.AllRackPreRunCnt == 0 && Global.gMainPageVM.AllRackRunCnt == 0)
                {
                    MasterRack.GraphIndex = 0;
                    MasterRack.startTempLog();
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[B" + BoxViewIndex + "] Graph Loging Start");
                }
                Global.gMainPageVM.AllRackPreRunCnt++;
            }
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[B" + BoxViewIndex + "] " + BoxSetSV + " Temp Setting");
            setTemp();
            Thread.Sleep(100);
            TCon_BoxRun();

            PreRunControlTemp = true;

            if (BoxSetSV < 30)
            {
                for (int count = 0; count <= 18; count++)
                {
                    await Task.Delay(10000);
                    //Thread.Sleep(10000);
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[B" + BoxViewIndex + "] Monitoring Timer " + (count * 10).ToString());
                }
            }
            else
            {
                for (int count = 0; count < 3; count++)
                {
                    await Task.Delay(10000);
                    //Thread.Sleep(10000);
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[B" + BoxViewIndex + "] Hot Temp Set Monitoring Timer " + (count * 10).ToString());
                }
            }
            // 습도 log save & display
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[B" + BoxViewIndex + "] SetTemp : " + BoxSetSV + " Monitoring Timer End");
            BoxStat = Box_STAT.RUN;
            TimeStart = DateTime.Now;
            if (Global.gMainPageVM.AllRackRunCnt == 0 && Global.gAlarmCom.AllRackStatus == false)
            {
                //Console.WriteLine("경광등 RUN");
                Global.gAlarmCom.AlarmWrite("R"); //경광등 RUN
                Global.gAlarmCom.AllRackStatus = true;
                Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.RUN;
                PreRunControlTemp = false;
            }
            lock (lockThis)
            {
                //MasterRack.BoxRunCnt++;
                
                Global.gMainPageVM.AllRackRunCnt++;
                Global.gMainPageVM.AllRackPreRunCnt--;
            }

            //Console.WriteLine("BoxViewIndex [{0}] BoxRunCnt = {1} ,AllRackRunCnt STOP IN = {2}", BoxViewIndex, MasterRack.BoxRunCnt, Global.gMainPageVM.AllRackRunCnt.ToString());
            isLoggingHumidity = true;
        }
        */
        public async void PreStopTest()
        {
            BoxDefrost = DEFROST_TYPE.TAMS;
            await stopTest();
        }
        public async Task<bool> stopTest()
        {
            //Console.WriteLine("{0} :PrestopTest", BoxIndex);
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==>[B" + BoxViewIndex + "] Pre Stop Test");

            if (!IsBoxTCUnit || IsBoxAS)
                return false;

            if (BoxStat != Box_STAT.RUN)
            {
                //await Task.Delay(100);
                return false;
            }
            else
            {
                await DEFROSTStep();
                isLoggingHumidity = false;
                return true;
                
            }
        }
        public async Task<bool> DEFROSTStep() //DEFROST
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==>[B" + BoxViewIndex + "] DefrostStep In");
            BoxStat = Box_STAT.DEFROST;

            double finalTemp = 30.0;
            int finalWaitTime = 18;
            int Cnt = 0;

            int sub_targetwait;
            int min;
            int sec;
            int step_waittime;
            int step_temp;
            int targettemp;
            try
            {
                targettemp = Global.gConfigVM.TargetTemp;
                step_temp = Global.gConfigVM.TempStep;
                step_waittime = Global.gConfigVM.TempStepTime;
                sub_targetwait = Global.gConfigVM.TargetWaitTime * 60;
                min = sub_targetwait % 3600 / 60;
                sec = sub_targetwait % 3600 % 60;
            }
            catch
            {
                targettemp = 60;
                step_temp = 1;
                step_waittime = 60;
                sub_targetwait = 20 * 60;
                min = sub_targetwait % 3600 / 60;
                sec = sub_targetwait % 3600 % 60;
            }
            double DEFROST_val = GetPV;
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Box Temp = " + DEFROST_val.ToString());
            if (DEFROST_val <= -10.0) DEFROST_val = -9.0;

            int TVal_Int = Convert.ToInt16(Math.Truncate(DEFROST_val) * 10);
            double TVal_Dou = Math.Truncate(DEFROST_val) * 10;

            if (TVal_Int < (targettemp * 10))
            {
                while(true)
                {
                    TVal_Dou = TVal_Dou + (step_temp * 10);
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Temp = " + TVal_Dou.ToString());

                    if(TVal_Dou < Convert.ToDouble(targettemp * 10))
                    {
                        setTemp(TVal_Dou);
                        await Task.Delay(100);
                    }
                    else
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Target Temp is OK = " + TVal_Dou.ToString());

                        setTemp(TVal_Dou);
                        await Task.Delay(100);

                        while(sub_targetwait >= 0)
                        {
                            DefostTime = min.ToString() + " : " + sec.ToString();
                            await Task.Delay(1000);
                            sub_targetwait -= 1;
                            min = sub_targetwait % 3600 / 60;
                            sec = sub_targetwait % 3600 % 60;
                        }
                        break;
                    }
                    await Task.Delay(step_waittime * 1000);
                }
            }
            else
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] NO Defrost = " + TVal_Dou.ToString());
            }

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Temp Down ");

            setTemp(Convert.ToDouble(Global.gConfigVM.TargetTemp + 5) * 10);

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Waiting Time");
            while (true) // 무조건 3분 대기하게 변경
            {
                //FinmalTemp 보다 온도다 낮으면 종료
                //if((GetPV <= finalTemp) || (Cnt > finalWaitTime))
                if(Cnt > finalWaitTime)
                {
                    DefostTime = "Done";
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Waiting Finished");
                    break;
                }
                await Task.Delay(10000);
                Cnt++;
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] WaitTimer " + Cnt.ToString());
            }

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Defrost Process Done");

            //Console.WriteLine("BoxViewIndex [{0}] AllRackRunCnt STOP IN = {1}", BoxViewIndex, Global.gMainPageVM.AllRackRunCnt.ToString());

            //Console.WriteLine("BoxViewIndex [{0}] AllRackRunCnt STOP OUT = {1}", BoxViewIndex, Global.gMainPageVM.AllRackRunCnt.ToString());
            lock (lockThis)
            {
                //MasterRack.BoxRunCnt--;
                Global.gMainPageVM.AllRackRunCnt--;
                //Global.gMainPageVM.AllRackPreRunCnt--;
            }
            //Console.WriteLine("BoxViewIndex [{0}] BoxRunCnt = {1} ,AllRackRunCnt STOP IN = {2}", BoxViewIndex, MasterRack.BoxRunCnt, Global.gMainPageVM.AllRackRunCnt.ToString());

            //CenterTemp 체크하여 모터 올리지 말지 Check
            double CenterTemp = 20.0;
            double HumTempVal = Convert.ToDouble(Hum_TempSenorVal);
            //Console.WriteLine("BoxViewIndex [{0}] GetPV = {1} , HumTempVal = {2}", BoxViewIndex, GetPV , HumTempVal);
            //if (GetPV < CenterTemp|| GetPV < HumTempVal)
            if (GetPV < CenterTemp)
                BoxDefrost = DEFROST_TYPE.LOWTEMP;

            if (BoxDefrost == DEFROST_TYPE.HUMIDITY || BoxDefrost == DEFROST_TYPE.TAMS || BoxDefrost == DEFROST_TYPE.ALARM || BoxDefrost == DEFROST_TYPE.TEMPERROR)
            {
                //온도조절기 스탑만
                TCon_BoxStop();
                //if (MasterRack.BoxRunCnt == 0) //여기서 box cnt 보고 모터업할필요없음 MasterRack 나가서 모터업 유무 확인
                //    MasterRack.RackCheckStop();

                if (BoxDefrost == DEFROST_TYPE.HUMIDITY)
                {
                    showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_Humidity, true, BoxIndex);
                    BoxStat = Box_STAT.ERROR;
                }
                else if(BoxDefrost == DEFROST_TYPE.ALARM)
                {
                    showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_Alarm, true, BoxIndex);
                    BoxStat = Box_STAT.ERROR;
                }
                else if(BoxDefrost == DEFROST_TYPE.TEMPERROR)
                {
                    showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_Unbalance, true, BoxIndex);
                    BoxStat = Box_STAT.ERROR;
                }

            }
            else if (BoxDefrost == DEFROST_TYPE.LOWTEMP)
            {
                showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_Cbox_25_temp, true, BoxIndex);
                BoxStat = Box_STAT.ERROR;
            }
            //온도조절기 스탑하고 모터 업하러
            TCon_BoxStop();
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==>[B" + BoxViewIndex + "] DefrostStep Out");
            return true;
        }
        public void setTemp()
        {
            //Console.WriteLine("{0} : setTemp : {1}" , BoxIndex, BoxSetSV);
            if (BoxStat == Box_STAT.DEFROST)
                return;

            byte ch = (byte)BoxViewIndex;
            string iTempVal = Convert.ToInt16(BoxSetSV * 10).ToString("X4");
            Global.gTconCom.SetTempMsg(ch, iTempVal);
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] SetTemp : " + BoxSetSV);
            TimeStart = DateTime.Now;
            TempErrorTime = null;
            TempAlarmTime = null;
            SVST(SVC.TCON);
            SampleLongTerm.Clear();
            SampleShortTerm.Clear();
        }
        public void ControlsetTemp(double getSetSV)
        {
            byte ch = (byte)BoxViewIndex;
            string iTempVal = Convert.ToInt16(getSetSV * 10).ToString("X4");
            Global.gTconCom.SetTempMsg(ch, iTempVal);
            TimeStart = DateTime.Now;
            TempErrorTime = null;
            TempAlarmTime = null;
        }
        public void setTemp(double getSetSV)
        {
            //Console.WriteLine("{0} : setTemp : {1}", BoxIndex, getSetSV);

            byte ch = (byte)BoxViewIndex;
            string iTempVal = Convert.ToInt16(getSetSV).ToString("X4");
            Global.gTconCom.SetTempMsg(ch, iTempVal);
            TimeStart = DateTime.Now;
            TempErrorTime = null;
            TempAlarmTime = null;

        }
        public void Read_BoxTConStat()
        {
            //if (!IsBoxTCUnit)
            //    return;

            byte ch = (byte)BoxViewIndex;
            Global.gTconCom.TConStatMsg(ch);
        }
        public void TCon_BoxStop()
        {
            if (!IsBoxTCUnit)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] BoxTCUnit UnCheck, NO STOP");
                return;
            }
            byte ch = (byte)BoxViewIndex;
            Global.gTconCom.RunStopMsg(ch, 0x01);
        }
        public void TCon_BoxRun()
        {
            if (!IsBoxTCUnit)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] BoxTCUnit UnCheck, NO RUN");
                return;
            }
            byte ch = (byte)BoxViewIndex;
            Global.gTconCom.RunStopMsg(ch, 0x00);
        }
        public bool setTamsTemp()
        {
            string fn = $"{Global.gConfigVM.TamsSVPath}\\" +
                $"{Global.gConfigVM.TamsSVFolder}{BoxViewIndex}\\" +
                $"{Global.gConfigVM.TamsSVFile}";
            FileInfo info = new FileInfo(fn);

            if (!info.Exists) return false;
            string[] lines = File.ReadAllLines(@info.FullName);
            foreach (string line in lines)
            {
                if (line.Contains("TEMP"))
                {
                    string[] buf = line.Split(':');
                    if (!buf[1].Equals(""))
                    {
                        BoxSetSV = double.Parse(buf[1]);
                        return true;
                    }
                }
            }
            return false;
        }
        public bool findEndFile()
        {
            string fn = $"{Global.gConfigVM.TamsEndPath}\\" +
                $"{Global.gConfigVM.TamsEndFolder}{BoxViewIndex}\\" +
                $"{Global.gConfigVM.TamsEndFile}";

            FileInfo finfo = new FileInfo(fn);

            if (finfo.Exists)
                return true;

            return false;
        }
        public void ProcessBoxSV() // lp 온도 제어 로직
        {
            //포트가 닫혀있으면 Skip
            if (Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort == null) return;
            if (!Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort.IsOpen) return;
            if(BoxPVText == "0" || BoxPVText == "" || BoxPVText == string.Empty) return;

            double ctrl = GetSV;
            double diff = BoxSetSV - GetPV;
            double setp = Global.gConfigVM.ControlSetp;
            double offset = Global.gConfigVM.ControlOffset;
            double max = Global.gConfigVM.ControlMaxTemp;
            double min = Global.gConfigVM.ControlMinTemp;

            if (Math.Abs(diff) <= Global.gConfigVM.ControlDiff)
            {
                if (BoxPV <= BoxSetSV - offset)
                    ctrl += setp;
                else if (BoxPV >= BoxSetSV + offset)
                    ctrl -= setp;
            }
            else
                ctrl = BoxSetSV;

            //if(ctrl > BoxSetSV) ctrl = BoxSetSV;
            //else if (ctrl < min) ctrl = min;
            if(ctrl < min) ctrl = min;
            else if(ctrl > max) ctrl = max;

            //setTemp(ctrl);
            ControlsetTemp(ctrl);
        }

        double prevDimAvg = 0;
        public void preTempData()
        {
            prevDimAvg = BoxPV;
        }
        public void sampleingTempData()
        {
            SampleShortTerm.Add(BoxPV - prevDimAvg);
            if (SampleShortTerm.Count > ShortTermCount)
                SampleShortTerm.RemoveAt(0);

            SampleLongTerm.Add(BoxPV);
            if (SampleLongTerm.Count > LongTermCount)
                SampleLongTerm.RemoveAt(0);
        }
        public void onlylongsampleingTempData()
        {
            SampleLongTerm.Add(BoxPV);
            if (SampleLongTerm.Count > LongTermCount)
                SampleLongTerm.RemoveAt(0);
        }
        public (double, int, int) AnalysTrend(Collection<double> collect)
        {
            double avgSum = collect.Sum();
            int down = 0, up = 0;
            foreach (double val in collect)
            {
                if (val > 0) up++;
                if (val < 0) down++;
            }


            return (avgSum, up, down);
        }

        // 표준 편차
        public double calcSD(Collection<double> collect)
        {
            if (collect.Count == 0) return 0;

            double sum = 0, mean = 0, sd = 0;
            foreach (double d in collect)
                sum += d;
            mean = sum / collect.Count;

            foreach (double d in collect)
                sd += Math.Pow(d - mean, 2);

            return Math.Sqrt(sd / collect.Count);
        }
        public void SVST(SVC set)
        {
            SVCtrl = (int)set;
        }
        public bool checkSV(SVC check)
        {
            int ch = (int)check;
            if ((SVCtrl & ch) != 0) return true;
            return false;
        }
        public double TradingOrgSV = 0;
        public int Level1Check = 0;
        public void ProcessBoxSV_Logic_Org()
        {
            if (Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort == null) return;
            if (!Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort.IsOpen) return;
            if (BoxPVText == "0" || BoxPVText == "" || BoxPVText == string.Empty)
            {
                SampleLongTerm.Clear();
                SampleShortTerm.Clear();
                return;
            }

            //double ctrl = GetSV;
            //double diff = BoxSetSV - GetPV;
            //double setp = Global.gConfigVM.ControlSetp;
            //double offset = Global.gConfigVM.ControlOffset;
            double max = Global.gConfigVM.ControlMaxTemp;
            double min = Global.gConfigVM.ControlMinTemp;

            double prevTarget = GetSV; //RealTime SV
            double DIM_SV_Diff = BoxSetSV - BoxPV; //Org - LPCAMMTemp
            double ctrl = GetSV;

            TRADING trading = TRADING.RISE;
            if (DIM_SV_Diff < 0) trading = TRADING.FALL;
            double direction = 1;
            if (trading == TRADING.FALL) direction = -1;

            DIM_SV_Diff = Math.Abs(DIM_SV_Diff);

            if (DIM_SV_Diff < Global.gConfigVM.ControlDiff) //몇도 내 들어오는지에 따라 제어방법 상이
            {
                //////////////////////////
                //////////////  Done ACCELERATION
                /////////////////////////////
                if (checkSV(SVC.ACCELERATION))
                {
                    ctrl = BoxSetSV;
                    SVST(SVC.TCON);
                }
                /*
                if (checkSV(SVC.DOWN))
                {
                    if (SampleShortTerm.Count <= 4) goto SVDone;
                    (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                    int orgLevel = Level;
                    if (Level == 1)
                    {
                        if (up > 0) // 상승 시작
                        {
                            Console.Write($"-- 상승시작 -- ");
                            SampleShortTerm.Clear();
                            ctrl = TradingOrgSV;
                            Level++;
                        }
                        //                        else
                        //                            sub.TargetSV += 1;


                    }
                    else if (Level == 2)
                    {

                        if (down > 0) // 상승 실패
                        {
                            Console.Write($"-- 상승실패 일반제어 복귀-- ");
                            //ctrl += Math.Round((DIM_SV_Diff * 1.6) * direction, 1);
                            ctrl += Math.Round(DIM_SV_Diff * direction, 1);
                            SVCtrl ^= (int)SVC.DOWN;
                            SampleLongTerm.Clear();
                        }
                        else if (DIM_SV_Diff <= 1)
                        {
                            if (up > 0)  // 오르는중.
                            {

                                SampleShortTerm.Clear();
                                TradingOrgSV = ctrl;

                                //  85도일때 add 2.6, 40도일때 add 8
                                //  기울기 = -0.12, 옵셋 = 12.8                              
                                //double nextSV = (BoxSetSV * -0.12) + 12.8;
                                double nextSV = (BoxSetSV * -0.12) + 12.8;
                                ctrl -= nextSV;
                                Level++;
                                Console.Write($"-- 오르는중 calcSV {nextSV}-- ");
                            }
                        }
                    }
                    else if (Level == 3)
                    {
                        if (down > 0)
                        {
                            Console.Write($"--L3 잡힘 --");
                            ctrl = TradingOrgSV;
                            Level = 0;
                            SVCtrl ^= (int)SVC.DOWN;
                        }
                    }
                    goto SVDone;
                }
                //////////////////////////
                ////  UP Action
                //////////////////////////
                if (checkSV(SVC.UP))
                {
                    (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                    if (down > 1)
                    {

                        SampleShortTerm.Clear();
                        SVCtrl ^= (int)SVC.UP;
                        Console.WriteLine($"--- Up 제어완료 --- sv {ctrl} -> {TradingOrgSV}");
                        ctrl = TradingOrgSV;

                    }
                    //                    else 
                    //                        sub.TargetSV -= 4;

                    goto SVDone;
                }
                */

                //////////////////////////
                ////  DOWN Action
                //////////////////////////
                if (checkSV(SVC.DOWN))
                {
                    (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                    if (up > 0)  // 오르는중.
                    {
                        SampleShortTerm.Clear();
                        SVCtrl ^= (int)SVC.DOWN;
                        Console.WriteLine($"--- Down 제어완료 ---");
                    }
                    else if(down > 2)
                    {
                        ctrl += (DIM_SV_Diff / 2);
                    }
                    goto SVDone;
                }

                //////////////////////////
                ////  UP Action
                //////////////////////////
                if (checkSV(SVC.UP))
                {
                    (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                    if (down > 0)
                    {
                        SampleShortTerm.Clear();
                        SVCtrl ^= (int)SVC.UP;
                        Console.WriteLine($"--- Up 제어완료 ---");
                    }
                    else if(up > 2)
                    {
                        ctrl -= (DIM_SV_Diff / 2);
                    }
                    goto SVDone;
                }

                //////////////////////////
                //////////// DO LOGN Term
                /////////////////////////////
                if (SampleLongTerm.Count == LongTermCount)
                {
                    double avg = SampleLongTerm.Average();
                    double sd = calcSD(SampleLongTerm);

                    //Console.WriteLine($"{Name} SD : {sd}");

                    if (sd < 2) SVST(SVC.STABLE);

                    if (checkSV(SVC.STABLE))
                    {
                        double stable_diff = Math.Abs(BoxSetSV - avg);
                        stable_diff = Math.Round(stable_diff, 1);

                        if (stable_diff > 0.5)
                        {

                            double org = ctrl;
                            //ctrl += Math.Round((stable_diff / 2) * direction, 1);
                            ctrl += Math.Round((stable_diff / 2) * direction, 1);
                            Console.WriteLine($" LongTerm Adjust {org} -> {ctrl}");
                            SampleLongTerm.Clear();
                            goto SVDone;
                        }
                    }
                }

                //////////////////////////////
                ///////////// Prevent Down / UP Action 
                /////////////////////////////////
                if (PrevTrading != trading)
                    SampleShortTerm.Clear();
                PrevTrading = trading;

                //////////////////////////
                ////////////////  DO SortTerm
                /////////////////////////////

                if (SampleShortTerm.Count == ShortTermCount)
                {
                    if (checkSV(SVC.STABLE))
                    {
                        if (!(checkSV(SVC.DOWN) || checkSV(SVC.UP)))
                        {
                            if (DIM_SV_Diff <= 5)
                            {
                                (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                                double sd = Math.Round(calcSD(SampleShortTerm), 3);

                                avg = Math.Round(avg, 1);
                                //Console.WriteLine($"{Name} SortTerm 편차:{sd} avg:{avg} u:{up} d:{down} T:{trading.ToString()}");
                                if (avg <= -0.8 && down > 1 &&
                                    ((trading == TRADING.RISE) ||
                                    (trading == TRADING.FALL && DIM_SV_Diff < 0.6)))
                                {
                                    Level1Check = 0;
                                    Level = 1;
                                    TradingOrgSV = ctrl;
                                    //sub.TargetSV += Math.Round((setSV * 8) / 100, 1);
                                    ctrl += Math.Round(DIM_SV_Diff * 1.5);

                                    Console.WriteLine($"다운제어 시작 SV: {TradingOrgSV} -> {ctrl}");
                                    SVCtrl |= (int)SVC.DOWN;

                                    SampleShortTerm.Clear();
                                    SampleLongTerm.Clear();
                                }

                                if (avg >= 0.8 && up > 1 &&
                                    ((trading == TRADING.FALL) ||
                                    (trading == TRADING.RISE && DIM_SV_Diff < 0.6)))
                                {
                                    Level = 1;
                                    //sub.TargetSV -= Math.Round((setSV * 8) / 100, 1);
                                    ctrl -= Math.Round(DIM_SV_Diff * 1.5);
                                    TradingOrgSV = ctrl;
                                    Console.WriteLine($"업 제어 시작 SV: {TradingOrgSV} -> {ctrl}");
                                    SVCtrl |= (int)SVC.UP;

                                    SampleShortTerm.Clear();
                                    SampleLongTerm.Clear();
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                SVST(SVC.ACCELERATION);
                ctrl = BoxSetSV + (5.0 * direction);
            }

        SVDone:
            if (ctrl > max) ctrl = max;
            else if (ctrl < min) ctrl = min;

            if (prevTarget != ctrl)
                ControlsetTemp(ctrl);
        }
        public void ProcessBoxSV_Logic_Long()
        {
            if (Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort == null) return;
            if (!Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort.IsOpen) return;
            if (BoxPVText == "0" || BoxPVText == "" || BoxPVText == string.Empty)
            {
                SampleLongTerm.Clear();
                return;
            }
            double max = Global.gConfigVM.ControlMaxTemp;
            double min = Global.gConfigVM.ControlMinTemp;

            double prevTarget = GetSV; //RealTime SV
            double DIM_SV_Diff = BoxSetSV - BoxPV; //Org - LPCAMMTemp
            double ctrl = GetSV;

            TRADING trading = TRADING.RISE;
            if (DIM_SV_Diff < 0) trading = TRADING.FALL;
            double direction = 1;
            if (trading == TRADING.FALL) direction = -1;

            DIM_SV_Diff = Math.Abs(DIM_SV_Diff);

            if (DIM_SV_Diff < Global.gConfigVM.ControlDiff) //몇도 내 들어오는지에 따라 제어방법 상이
            {
                //////////////////////////
                //////////////  Done ACCELERATION
                /////////////////////////////
                if (checkSV(SVC.ACCELERATION))
                {
                    ctrl = BoxSetSV;
                    SVST(SVC.TCON);
                }

                //////////////////////////
                //////////// DO LOGN Term
                /////////////////////////////
                if (SampleLongTerm.Count == LongTermCount)
                {
                    double avg = SampleLongTerm.Average();
                    double sd = calcSD(SampleLongTerm);

                    //Console.WriteLine($"{Name} SD : {sd}");

                    if (sd < 2) SVST(SVC.STABLE);

                    if (checkSV(SVC.STABLE))
                    {
                        double stable_diff = Math.Abs(BoxSetSV - avg);
                        stable_diff = Math.Round(stable_diff, 1);

                        if (stable_diff > 0.5)
                        {
                            double org = ctrl;
                            //ctrl += Math.Round((stable_diff / 2) * direction, 1);
                            ctrl += Math.Round((stable_diff / 2) * direction, 1);
                            Console.WriteLine($" LongTerm Adjust {org} -> {ctrl}");
                            SampleLongTerm.Clear();
                            goto SVDone;
                        }
                    }
                }
            }
            else
            {
                SVST(SVC.ACCELERATION);
                ctrl = BoxSetSV + (5.0 * direction);
            }

        SVDone:
            if (ctrl > max) ctrl = max;
            else if (ctrl < min) ctrl = min;

            if (prevTarget != ctrl)
                ControlsetTemp(ctrl);
        }
    }
}
