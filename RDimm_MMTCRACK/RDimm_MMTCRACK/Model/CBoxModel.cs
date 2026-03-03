using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.View.Component;
using RDimm_MMTCRACK.ViewModel;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Interop;

namespace RDimm_MMTCRACK.Model
{
    public enum CBOX_STAT
    {
        IDLE = 0,
        STOP,
        PRERUN,
        RUN,
        DONE,
        DEFROST,
        ERROR
    }
    public enum DEFROST_TYPE
    {
        NORMAL = 0,
        HUMIDITY,
        ALARM,
        LOWTEMP,
        TAMS,
        TEMPERROR,
        FAIL
    }
    public enum DIM_TYPE
    {
        LOW = 0,
        HIGH,
        LH,
        HL,
        AVG,
        MAX,
        MID,
        EACH
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

    public enum TRADING
    {
        RISE,
        FALL
    }
    public class CBoxModel : BaseVM
    {
        public string Name { get; set; }

        [DoNotNotify] public int BoxIndex { get; set; } = 0;
        [DoNotNotify] public int BoxViewIndex { get; set; } = 0;

        public int DimColumns { get; set; }

        public string BoxTempText { get; set; } 
        double _boxtemp;
        public double BoxTemp 
        { 
            get => _boxtemp; 
            set
            {
                _boxtemp = value;
                BoxTempText = _boxtemp.ToString();
            }
        }
        double _insideboxtemp = 0;
        public double InsideBoxTemp //GetPV
        { 
            get => _insideboxtemp; 
            set
            {
                _insideboxtemp = value;
                Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[BoxIndex].EngrRackPV = _insideboxtemp.ToString();
            }
        } 
        [DoNotNotify]
        public double DimTempAvg { get; set; }
        public DIM_TYPE _dimType = DIM_TYPE.LOW;
        [DoNotNotify]
        public double SubInsideTemp { get; set; }
        [DoNotNotify]
        public double SubDimTempAvg { get; set; }

        double _targetsv = 0;
        public double TargetSV //GetSV
        { 
            get => _targetsv; 
            set
            {
                _targetsv = value;
                Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[BoxIndex].EngrRackSV = _targetsv.ToString();
            }
        } 
        double _boxsetSV = 25;
        public double BoxSetSV
        {
            get => _boxsetSV;
            set
            {
                _boxsetSV = value;
            }
        }
        //[DoNotNotify]
        //public double ReadPV { get; set; }

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

        CBOX_STAT _cboxStat = CBOX_STAT.IDLE;
        public CBOX_STAT CBoxStat
        {
            get => _cboxStat;
            set
            {
                _cboxStat = value;
                BoxColor = Global.gCBoxColors[(int)value];

                //if ((_cboxStat == CBOX_STAT.STOP || _cboxStat == CBOX_STAT.IDLE) && BoxTempText != "")
                if (_cboxStat == CBOX_STAT.IDLE && BoxTempText != "")
                {
                    BoxTempText = string.Empty;
                    foreach (var dm in DimModels)
                    {
                        dm.DimTemp = 0;
                        dm.Stat = DIMM_STAT.STOP;
                    }

                }
                else if(_cboxStat == CBOX_STAT.STOP)
                {
                    foreach (var dm in DimModels)
                        dm.Stat = DIMM_STAT.STOP;
                }
                else if(_cboxStat == CBOX_STAT.RUN)
                {
                    foreach (var dm in DimModels)
                    {
                        //dm.DimTemp = 0;
                        dm.Stat = DIMM_STAT.RUN;
                        MasterRack.TimeStart = DateTime.Now;
                    }
                }
                else if (_cboxStat == CBOX_STAT.DEFROST || _cboxStat == CBOX_STAT.PRERUN)
                {
                    foreach (var dm in DimModels)
                        dm.Stat = DIMM_STAT.RUN;
                }

                //-BoarderColor = Global.gBoarderColors[(int)_boxStat];
                //BoxStatText = _boxStat.ToString();
            }
        }
        public DEFROST_TYPE BoxDefrost { get; set; } = DEFROST_TYPE.NORMAL;

        [DoNotNotify]
        public List<int> DimCtrlList { get; set; } = new List<int>();
        [DoNotNotify]
        public ObservableCollection<DimModel> DimModels { get; set; } = new ObservableCollection<DimModel>();

        public Collection<double> SampleLongTerm = new Collection<double>();

        public Collection<double> SampleShortTerm = new Collection<double>();

        public int Level = 0;
        public int Level1Check = 0;
        public double OrgSV = 0;

        public double BSVAlue = 0;

        public int SVCtrl;

        public TRADING PrevTrading = TRADING.RISE;

        public List<double> MaxInBox = new List<double>();
        public SolidColorBrush BoxColor { get; set; }

        [DoNotNotify]
        public RackModel MasterRack { get; set; }

        public static System.Object lockThis = new System.Object();

        public SolidColorBrush HumRateColor { get; set; } = Global.gWarningColors[0];

        //item Config 밖으로 뺴기 ShortTerm 5,6 
        public int LongTermCount = 20;
        public int ShortTermCount = 10;

        public bool PreRunControlTemp { get; set; } = false;

        bool _tconisrun = false;
        public bool TconIsRun 
        { 
            get => _tconisrun; 
            set
            {
                _tconisrun = value;
                /*
                if (_tconisrun)
                    Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[BoxIndex].EngrRackTconstat = "RUN";
                else
                    Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[BoxIndex].EngrRackTconstat = "STOP";
                */
            }
        }
        public void GetTconStatus(bool GetStatus)
        {
            if (GetStatus)
            {
                TconIsRun = GetStatus;
                Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[BoxIndex].EngrRackTconstat = "RUN";
            }
            else
            {
                TconIsRun = GetStatus;
                Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[BoxIndex].EngrRackTconstat = "STOP";
            }
        }

        bool _bimtal_stat = true;
        public bool Bimetal_Stat
        {
            get => _bimtal_stat;
            set
            {
                _bimtal_stat = value;
                /*
                if (_bimtal_stat == false)
                {
                    if (CBoxStat == CBOX_STAT.RUN || CBoxStat == CBOX_STAT.DEFROST)
                    {
                        lock (lockThis)
                            Global.gMainPageVM.AllRackRunCnt--;
                    }

                    MasterRack.showAlram(ALRAM_TYPE.ERROR, Global.msg_bimetal, true);
                    CBoxStat = CBOX_STAT.ERROR;
                    MasterRack.ErrCBoxTConStop();
                    
                    //Console.WriteLine("BoxViewIndex [{0}] MasterRack.BoxRunCnt = {1}", BoxViewIndex, MasterRack.BoxRunCnt);
                    //바이메탈 alarm msg box 
                }
                else
                {
                    CBoxStat = CBOX_STAT.STOP;
                    MasterRack.ShowBox = Visibility.Hidden;
                    //msg box Visibility false
                }
                */
            }
        }
        public void BimetalStat(bool getStat)
        {
            if (getStat == false && MasterRack.RackStat != RACK_STAT.ERROR)
            {
                Bimetal_Stat = getStat;

                if (CBoxStat == CBOX_STAT.RUN || CBoxStat == CBOX_STAT.DEFROST)
                {
                    lock (lockThis)
                        Global.gMainPageVM.AllRackRunCnt--;
                }

                MasterRack.showAlram(ALRAM_TYPE.ERROR, Global.msg_bimetal, true);
                CBoxStat = CBOX_STAT.ERROR;
                MasterRack.ErrCBoxTConStop();
            }
            else if (getStat)
            {
                if (Bimetal_Stat == false)
                {
                    Bimetal_Stat = getStat;

                    MasterRack.ErrClearTConStat();
                    MasterRack.ShowBox = Visibility.Hidden;
                }
            }
        }

        public int Bimetal_Stat_Cnt { get; set; } = 0;
        public int Sopen_Stat_Cnt { get; set; } = 0;
        bool _sopen_stat = true;
        public bool Sopen_Stat
        {
            get => _sopen_stat;
            set
            {
                _sopen_stat = value;
                /*
                if (_sopen_stat == false)
                {
                    if (CBoxStat == CBOX_STAT.RUN || CBoxStat == CBOX_STAT.DEFROST)
                    {
                        lock (lockThis)
                            Global.gMainPageVM.AllRackRunCnt--;
                    }

                    MasterRack.showAlram(ALRAM_TYPE.ERROR, Global.msg_sOpen, true);
                    CBoxStat = CBOX_STAT.ERROR;
                    MasterRack.ErrCBoxTConStop();
                    
                    
                    //if (MasterRack.RackStat == RACK_STAT.ERROR)
                    //    MasterRack.RackStat = RACK_STAT.STOP;

                    _sopen_stat = true;
                    //Console.WriteLine("BoxViewIndex [{0}] MasterRack.BoxRunCnt = {1}", BoxViewIndex, MasterRack.BoxRunCnt);
                    //sopen alarm msg box 
                }
                else
                {
                    CBoxStat = CBOX_STAT.STOP;
                    MasterRack.ShowBox = Visibility.Hidden;
                    
                    //msg box Visibility false
                }
                */
            }
        }
        public void SopenStat(bool getStat)
        {
            if (getStat == false && MasterRack.RackStat != RACK_STAT.ERROR)
            {
                Sopen_Stat = getStat;

                if (CBoxStat == CBOX_STAT.RUN || CBoxStat == CBOX_STAT.DEFROST)
                {
                    lock (lockThis)
                        Global.gMainPageVM.AllRackRunCnt--;
                }

                MasterRack.showAlram(ALRAM_TYPE.ERROR, Global.msg_sOpen, true);
                CBoxStat = CBOX_STAT.ERROR;
                MasterRack.ErrCBoxTConStop();
            }
            else if (getStat)
            {
                if (Sopen_Stat == false)
                {
                    Sopen_Stat = getStat;

                    MasterRack.ErrClearTConStat();
                    MasterRack.ShowBox = Visibility.Hidden;
                }
            }
        }

        public bool isLoggingHumidity = false;
        public int Hum_TempSenorVal { get; set; } = 0;
        public int HumidityOffset { get; set; } = 0;

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
        public int humidityWarningCnt = 0;
        void HumidityValueCheck()
        {
            //0 ~ 99까지 습도값으로 판단 100 이상은 쓰레기값으로 
            if (_humidityValue + HumidityOffset < 0)
                _humidityValue = 0;
            else if (_humidityValue + HumidityOffset >= 100)
            {
                _humidityValue = PreHumidityValue;

                //if (humidityWarningCnt > 10) // 10번에 1번만 Warning Msg
                //{
                //    //Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Box" + BoxViewIndex + "]  =  HumidityValue 100 Over");
                //    humidityWarningCnt = 0;
                //}
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
                humidityWarningCnt++;
                if (CBoxStat == CBOX_STAT.RUN && humidityWarningCnt <= 4)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[{MasterRack.RackViewIndex} - {BoxViewIndex}] humidity = {_humidityValue} 습도 경고 Count = {humidityWarningCnt + 1} ");
                }
                else if (CBoxStat == CBOX_STAT.RUN && humidityWarningCnt > 4) // humidityWarnval 넘어가면 제상스탑
                {
                    BoxDefrost = DEFROST_TYPE.HUMIDITY;
                    Thread.Sleep((BoxIndex % 6) * 100);
                    isLoggingHumidity = false;
                    //습도 제상 루틴
                    //box 하나만 말고 Rack 으로 제상 진행
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[{MasterRack.RackViewIndex} - {BoxViewIndex}] humidity = {_humidityValue} 습도 제상 발생 ");
                    Task.Run(() => MasterRack.RackstopTestAsync());
                }
            }
            else
                humidityWarningCnt = 0;
        }

        public CBOX_STAT _boxStat = CBOX_STAT.STOP;

        public CBoxModel(string name, RackModel master)
        {
            Name = $"CBox {name}"; MasterRack = master;
            BoxColor = Global.gCBoxColors[0];//STOP or IDLE
        }
        public void BoxStatusClear()
        {
            if (CBoxStat == CBOX_STAT.RUN || CBoxStat == CBOX_STAT.DEFROST)
            {
                lock (lockThis)
                {
                    //MasterRack.BoxRunCnt--;
                    Global.gMainPageVM.AllRackRunCnt--;
                    //Global.gMainPageVM.AllRackPreRunCnt--;
                }
            }
            CBoxStat = CBOX_STAT.STOP;
            BoxDefrost = DEFROST_TYPE.NORMAL;
            Sopen_Stat = true;
            Bimetal_Stat = true;
        }
        public void setTemp()
        {
            //Console.WriteLine("{0} : setTemp : {1}" , BoxIndex, BoxSetSV);
            if (CBoxStat == CBOX_STAT.DEFROST)
                return;

            try
            {
                byte ch = (byte)(MasterRack.RackIndex * 3 + BoxViewIndex);
                string iTempVal = Convert.ToInt16(BoxSetSV * 10).ToString("X4");

                Global.gTconCom.SetTempMsg(ch, iTempVal);
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] SetTemp : " + BoxSetSV);
                SVST(SVC.TCON);
                SampleLongTerm.Clear();
                SampleShortTerm.Clear();
            }
            catch { }
        }
        public void setTemp(double getSetSV)
        {
            //Console.WriteLine("{0} : setTemp : {1}", BoxIndex, getSetSV);

            byte ch = (byte)(MasterRack.RackIndex * 3 + BoxViewIndex);
            string iTempVal = Convert.ToInt16(getSetSV).ToString("X4");

            Global.gTconCom.SetTempMsg(ch, iTempVal);
        }
        public void PreStartTest()
        {
            //Console.WriteLine("{0} : PreStartTest", BoxIndex);
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"==> [ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Pre Start Test");

            CBoxStat = CBOX_STAT.PRERUN;

            if (MasterRack.RackStat != RACK_STAT.PRERUN)
                MasterRack.RackStat = RACK_STAT.PRERUN;

            Bimetal_Stat_Cnt = 0;
            Sopen_Stat_Cnt = 0;
            HumidityValue = 0;
            humidityWarningCnt =0;
            isLoggingHumidity = false;
            //Errmsg.log" ?? 필요 유무?
        }
        public async void startTest()
        {
            if (CBoxStat != CBOX_STAT.PRERUN)
                return;

            /*
            lock (lockThis)
            {
                //MasterRack에서 그래프 시작하게
                if (Global.gMainPageVM.AllRackPreRunCnt == 0 && Global.gMainPageVM.AllRackRunCnt == 0)
                {
                    MasterRack.GraphIndex = 0;
                    MasterRack.startTempLog();
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Graph Loging Start");
                }
                
                Global.gMainPageVM.AllRackPreRunCnt++;
            }
            */

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] {BoxSetSV} Temp Setting");
            setTemp();
            Thread.Sleep(100);
            TCon_BoxRun();

            PreRunControlTemp = true;

            if (BoxSetSV <= 40)
            {
                for (int count = 0; count <= 18; count++)
                {
                    await Task.Delay(10000);
                    //Thread.Sleep(10000);
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Monitoring Timer {(count * 10).ToString()}");
                    if (CBoxStat != CBOX_STAT.PRERUN)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] CBox is not PreRun Status.");
                        return;
                    }
                }
            }
            else
            {
                for (int count = 0; count < 3; count++)
                {
                    await Task.Delay(10000);
                    //Thread.Sleep(10000);
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Hot Temp Set Monitoring Timer {(count * 10).ToString()}");
                    if(CBoxStat != CBOX_STAT.PRERUN)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] CBox is not PreRun Status.");
                        return;
                    }

                }
            }
            // 습도 log save & display
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] SetTemp :  {BoxSetSV}  Monitoring Timer End");
            CBoxStat = CBOX_STAT.RUN;
            
            if(MasterRack.RackStat != RACK_STAT.RUN)
                MasterRack.RackStat = RACK_STAT.RUN;

            if (Global.gMainPageVM.AllRackRunCnt == 0 && Global.gAlarmCom.AllRackStatus == false)
            {
                //Console.WriteLine("경광등 RUN");
                Global.gAlarmCom.AlarmWrite("R"); //경광등 RUN
                Global.gAlarmCom.AllRackStatus = true;
                Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.RUN;
                PreRunControlTemp = false;
            }
            lock (lockThis)
                Global.gMainPageVM.AllRackRunCnt++;

            //Console.WriteLine("BoxViewIndex [{0}] BoxRunCnt = {1} ,AllRackRunCnt STOP IN = {2}", BoxViewIndex, MasterRack.BoxRunCnt, Global.gMainPageVM.AllRackRunCnt.ToString());
            isLoggingHumidity = true;
        }
        public Task<double> CalculateDefrostedTime()
        {
            // 각 변수 가져오기
            double TVal_Dou = Math.Truncate(InsideBoxTemp); // 시작 온도
            int targettemp = Global.gConfigVM.TargetTemp; // 목표 온도
            int step_temp = Global.gConfigVM.TempStep; // 온도 증가량
            int step_waittime = Global.gConfigVM.TempStepTime; // 온도 증가 대기 시간 (초)
            int sub_targetwait = Global.gConfigVM.TargetWaitTime * 60; // 목표 대기 시간 (초)

            double tempDiff = 0;
            int numSteps = 0;
            double tempIncreaseTime = 0;
            double subTargetWaitTime = 0;

            if (TVal_Dou < targettemp)
            {
                // 온도 증가에 필요한 반복 횟수
                tempDiff = targettemp - TVal_Dou; // 목표 온도와 현재 온도 차이
                numSteps = (int)(tempDiff / step_temp); // 필요한 반복 횟수
                tempIncreaseTime = numSteps * step_waittime; // 온도 증가에 걸리는 시간

                // sub_targetwait는 그대로
                subTargetWaitTime = sub_targetwait;
            }
            // 최종 대기 시간은 180초 (3분)
            double finalWaitTime = 180; // 3분 대기

            //최종 모터 업 대기 60초(1분)
            double motorWaitTime = 60;

            // 총 예상 시간 계산 (초 단위)
            double totalEstimatedTime = tempIncreaseTime + subTargetWaitTime + finalWaitTime + motorWaitTime;

            return Task.FromResult(totalEstimatedTime); // 총 예상 시간 (초)
        }
        public async Task<DEFROST_TYPE> stopTest()
        {
            //Console.WriteLine("{0} :PrestopTest", BoxIndex);
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $" ==>[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Pre Stop Test");

            /*
            if (CBoxStat != CBOX_STAT.RUN)
            {
                //await Task.Delay(100);
               Global.gMainPageVM.addLog(LOG_TYPE.INFO, $" ==>[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] CBox is not Run Status now {CBoxStat.ToString()}");
                return DEFROST_TYPE.FAIL;
            }
            else
            {
                DEFROST_TYPE CboxEnd = await DEFROSTStep();
                isLoggingHumidity = false;
                return CboxEnd;
            }
            */

            try
            {
                if (CBoxStat == CBOX_STAT.RUN)
                {
                    DEFROST_TYPE CboxEnd = await DEFROSTStep();
                    isLoggingHumidity = false;
                    return CboxEnd;
                }
                else if (CBoxStat == CBOX_STAT.IDLE)
                {
                    return DEFROST_TYPE.NORMAL;
                }
                else if (CBoxStat == CBOX_STAT.DEFROST)// 수정포인트 0415
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $" ==>[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] CBox is already {CBoxStat.ToString()}");
                    return DEFROST_TYPE.NORMAL;
                }
                else
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $" ==>[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] CBox is not Run Status now {CBoxStat.ToString()}");
                    return DEFROST_TYPE.FAIL;
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"Error in stopTest: {ex.Message}");
                return DEFROST_TYPE.FAIL;
            }

        }
        public async Task<DEFROST_TYPE> DEFROSTStep() //DEFROST
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"==>[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] DefrostStep In");
            CBoxStat = CBOX_STAT.DEFROST;

            // MasterRack 상태 설정
            if (MasterRack.RackStat != RACK_STAT.DEFROST)
                MasterRack.RackStat = RACK_STAT.DEFROST;

            // 설정 값 가져오기
            int targettemp = Global.gConfigVM.TargetTemp;
            int step_temp = Global.gConfigVM.TempStep;
            int step_waittime = Global.gConfigVM.TempStepTime;
            int sub_targetwait = Global.gConfigVM.TargetWaitTime * 60;

            // Box 온도 값 가져오기
            double DEFROST_val = InsideBoxTemp;
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Box Temp =  {DEFROST_val.ToString()}");
            if (DEFROST_val <= -10.0) DEFROST_val = -9.0;

            int TVal_Int = Convert.ToInt16(Math.Truncate(DEFROST_val) * 10);
            double TVal_Dou = Math.Truncate(DEFROST_val) * 10;

            if (BoxSetSV < 30)
            {

                if (TVal_Int < (targettemp * 10))
                {
                    while (MasterRack.RackStat == RACK_STAT.DEFROST)
                    {
                        if (CBoxStat != CBOX_STAT.DEFROST)
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] DEFROST FAIL OUT");
                            return DEFROST_TYPE.FAIL;
                        }

                        // 목표 온도 증가
                        TVal_Dou = TVal_Dou + (step_temp * 10);
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Temp = {TVal_Dou.ToString()}");

                        if (TVal_Dou < Convert.ToDouble(targettemp * 10))
                        {
                            setTemp(TVal_Dou);
                            await Task.Delay(100);
                        }
                        else
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Target Temp is OK = {TVal_Dou.ToString()}");

                            setTemp(TVal_Dou);
                            await Task.Delay(100);

                            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Target Temp Waiting {sub_targetwait} Sec");

                            while (sub_targetwait > 0)
                            {
                                if (CBoxStat != CBOX_STAT.DEFROST)
                                //return DEFROST_TYPE.FAIL;
                                {
                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] DEFROST FAIL OUT");
                                    return DEFROST_TYPE.FAIL;
                                }
                                //DefostTime = min.ToString() + " : " + sec.ToString();
                                await Task.Delay(1000);
                                sub_targetwait--;
                                //min = sub_targetwait % 3600 / 60;
                                //sec = sub_targetwait % 3600 % 60;
                            }
                            break;
                        }
                        await Task.Delay(step_waittime * 1000);
                    }
                }
                else
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] NO Defrost = {TVal_Dou.ToString()}");
                }
            }
            else if (BoxSetSV >= 60)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Temp Down ");

                // 40℃로 설정
                setTemp(40 * 10);

                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Waiting Time");

                int finalWaitTime = 18;
                int Cnt = 0;

                while (MasterRack.RackStat == RACK_STAT.DEFROST) // 무조건 3분 대기하게 변경
                {
                    if (CBoxStat != CBOX_STAT.DEFROST)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] DEFROST FAIL OUT");
                        return DEFROST_TYPE.FAIL;
                    }
                    //FinmalTemp 보다 온도다 낮으면 종료
                    //if((GetPV <= finalTemp) || (Cnt > finalWaitTime))
                    if (Cnt > finalWaitTime)
                    {
                        //DefostTime = "Done";
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Waiting Finished");
                        break;
                    }
                    await Task.Delay(10000);
                    Cnt++;
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] WaitTimer  {Cnt.ToString()}");
                }
            }
            else if(BoxSetSV >= 30 && BoxSetSV < 60)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Skip Defrost Process");
            }
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {MasterRack.RackViewIndex} - {BoxViewIndex} ] Defrost Process Done");

            //Console.WriteLine("BoxViewIndex [{0}] AllRackRunCnt STOP IN = {1}", BoxViewIndex, Global.gMainPageVM.AllRackRunCnt.ToString());

            //Console.WriteLine("BoxViewIndex [{0}] AllRackRunCnt STOP OUT = {1}", BoxViewIndex, Global.gMainPageVM.AllRackRunCnt.ToString());
            lock (lockThis)
            {
                //MasterRack.BoxRunCnt--;
                Global.gMainPageVM.AllRackRunCnt--;
                //Global.gMainPageVM.AllRackPreRunCnt--;
            }
            //Console.WriteLine("BoxViewIndex [{0}] BoxRunCnt = {1} ,AllRackRunCnt STOP IN = {2}", BoxViewIndex, MasterRack.BoxRunCnt, Global.gMainPageVM.AllRackRunCnt.ToString());

            //CenterTemp 체크하여 모터 올리지 말지 Check인
            BoxDefrost = CheckBoxLowTempStatus();


            //TEST용
            /*
            if (BoxIndex == 0)
                BoxDefrost = DEFROST_TYPE.NORMAL;
            else
                BoxDefrost = DEFROST_TYPE.TAMS;
            */
            TCon_BoxStop();

            DEFROST_TYPE cboxRes;


            if (BoxDefrost == DEFROST_TYPE.HUMIDITY)
            {
                //showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_Humidity, true, BoxIndex);
                CBoxStat = CBOX_STAT.ERROR;
                cboxRes = DEFROST_TYPE.HUMIDITY;
            }
            else if (BoxDefrost == DEFROST_TYPE.ALARM)
            {
                //showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_Alarm, true, BoxIndex);
                CBoxStat = CBOX_STAT.ERROR;
                cboxRes = DEFROST_TYPE.ALARM;
            }
            else if (BoxDefrost == DEFROST_TYPE.TEMPERROR)
            {
                //showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_Unbalance, true, BoxIndex);
                CBoxStat = CBOX_STAT.ERROR;
                cboxRes = DEFROST_TYPE.TEMPERROR;
            }
            else if (BoxDefrost == DEFROST_TYPE.LOWTEMP)
            {
                //showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_Cbox_25_temp, true, BoxIndex);
                CBoxStat = CBOX_STAT.ERROR;
                cboxRes = DEFROST_TYPE.LOWTEMP;
            }
            else if (BoxDefrost == DEFROST_TYPE.TAMS)
            {
                cboxRes = DEFROST_TYPE.TAMS;
            }
            else if (BoxDefrost == DEFROST_TYPE.NORMAL)
            {
                cboxRes = DEFROST_TYPE.NORMAL;
            }
            else
                cboxRes = DEFROST_TYPE.FAIL;


            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==>[B" + BoxViewIndex + "] DefrostStep Out");
            

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"==>[B { BoxViewIndex} ] {cboxRes}");

            return cboxRes;
        }
        private DEFROST_TYPE CheckBoxLowTempStatus()
        {
            double CenterTemp = 25.0;
            if (InsideBoxTemp < CenterTemp)
                return DEFROST_TYPE.LOWTEMP;

            return BoxDefrost;
        }
        public void TCon_BoxStop()
        {
            /*tc unit
            if (!MasterRack.IsRackTCUnit)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] FIX TCUnit UnCheck, NO STOP");
                return;
            }
            */
            byte ch = (byte)(MasterRack.RackIndex * 3 + BoxViewIndex);
            Global.gTconCom.RunStopMsg(ch, 0x01);
        }
        public void TCon_BoxRun()
        {
            if (!MasterRack.IsRackTCUnit)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] FIX TCUnit UnCheck, NO RUN");
                return;
            }
            byte ch = (byte)(MasterRack.RackIndex * 3 + BoxViewIndex);
            Global.gTconCom.RunStopMsg(ch, 0x00);
        }
        public void Read_BoxTConStat()
        {
            //if (!IsBoxTCUnit)
            //    return;

            byte ch = (byte)(MasterRack.RackIndex * 3 + BoxViewIndex);
            Global.gTconCom.TConStatMsg(ch);
        }
        public (double, int, int) AnalysTrend(Collection<double> collect)
        {
            // collect의 복사본을 만들어서 안전하게 사용
            var copy = collect.ToList();

            double avgSum = copy.Sum();
            int down = 0, up = 0;
            foreach (double val in copy)
            {
                if (val > 0) up++;
                if (val < 0) down++;
            }


            return (avgSum, up, down);
        }

        // 표준 편차 수정 System.InvalidOperationException 발생
        public double calcSD(Collection<double> collect)
        {
            if (collect.Count == 0) return 0;

            // collect의 복사본을 만들어서 안전하게 사용
            var copy = collect.ToList();

            double sum = copy.Sum();
            double mean = sum / copy.Count;

            double variance = copy.Sum(value => Math.Pow(value - mean, 2));
            return Math.Sqrt(variance / copy.Count);
            /*
            if (collect.Count == 0) return 0;

            double sum = 0, sd = 0;

            // 평균 계산
            foreach (var value in collect)
            {
                sum += value;
            }

            double mean = sum / collect.Count;

            // 표준 편차 계산
            foreach (var value in collect)
            {
                sd += Math.Pow(value - mean, 2);
            }

            return Math.Sqrt(sd / collect.Count);
            */
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

        private const int MaxAllowedCount = 100;
        public void processBoxDimm()
        {
            IList<double> dims = (IList<double>)Global.gMainPageVM.cc[MasterRack.RackIndex].RackDimmTemp;
            IList<DimDataModel> dimTemps = (IList<DimDataModel>)MasterRack.DimDatas;

            DateTime afterStable = MasterRack.TimeStart.AddMinutes(Global.gConfigVM.StableTime);

            double dimAvg = 0;
            int mountCnt = 0;
            double pvAvg = 0;
            bool isError = false;
            string warnMsg = "";
            int uiIdx = 0;

            int subMountCnt = 0;
            double subDimAvg = 0;
            for (int i = 0; i < DimCtrlList.Count; i++)
            {
                int index = DimCtrlList[i];
                DimModel uiDim = DimModels[uiIdx];

                //if (index >= Global.gMainPageVM.MaxFixDimmCount) continue;
                if (index >= Global.gMainPageVM.MaxFixDimmCount || index >= dims.Count || index >= dimTemps.Count) continue;

                DIM_TYPE type = _dimType;
                if (type == DIM_TYPE.EACH)
                {
                    int eachidx = (int)MasterRack.RackSetSV / 5;
                    //type = EachTempHLs[eachidx];
                    //type = (DIM_TYPE)Enum.Parse(typeof(DIM_TYPE), EachTempHLs[eachidx]
                }

                uiDim.DimTempLow = dimTemps[index].TempLow = dims[index * 2];
                uiDim.DimTempHigh = dimTemps[index].TempHigh = dims[index * 2 + 1];


                double temp = 0;
                if (type == DIM_TYPE.HIGH)
                    temp = dims[index * 2 + 1];
                else if (type == DIM_TYPE.LOW)
                    temp = dims[index * 2];
                else if (type == DIM_TYPE.LH)
                {
                    if (i >= (DimCtrlList.Count / 2))
                        temp = dims[index * 2 + 1];
                    else
                        temp = dims[index * 2];
                }
                else if (type == DIM_TYPE.HL)
                {
                    if (i >= (DimCtrlList.Count / 2))
                        temp = dims[index * 2];
                    else
                        temp = dims[index * 2 + 1];
                }
                else if (type == DIM_TYPE.AVG
                        || type == DIM_TYPE.MAX
                        || type == DIM_TYPE.MID)
                {
                    temp += dims[index * 2];
                    temp += dims[index * 2 + 1];
                    temp /= 2;
                }


                uiDim.DimTemp = temp;
                dimTemps[index].Temp = temp;


                //MasterRack.MaxInRack.Add(uiDim.DimTempLow);
                //MasterRack.MaxInRack.Add(uiDim.DimTempHigh);
                if (MaxInBox.Count >= MaxAllowedCount)
                {
                    MaxInBox.RemoveAt(0);
                }
                try
                {
                    MaxInBox.Add(uiDim.DimTempLow);
                    MaxInBox.Add(uiDim.DimTempHigh);
                }
                catch (ArgumentOutOfRangeException ex)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"Error adding to MaxInBox: {ex.Message}. Index: {i}, DimTempLow: {uiDim.DimTempLow}");
                }
                
                if (temp != 0)
                {
                    dimAvg += temp;
                    subDimAvg += temp;
                    subMountCnt++;
                    mountCnt++;
                }

                if (DateTime.Now > afterStable)
                {
                    if (MasterRack.RackStat == RACK_STAT.RUN)
                    {
                        uiDim.doDimStat();
                        //if (uiDim.Stat == DIMM_STAT.ALRAM)
                        //{
                        //    warnMsg += $"Dim{uiIdx + 1} = {uiDim._dimTemp} ";
                        //}
                        if (uiDim.Stat == DIMM_STAT.ERROR)
                        {
                            isError = true;
                            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Fix {MasterRack.RackViewIndex}][{Name}] : Dimm{uiIdx}(Total Index{index}) " +
                                $"Temp={uiDim.DimTemp} " +
                                $" Limit={Global.gConfigVM.TempErr}" +
                                $" Max={Global.gConfigVM.HighTempAlarm}" +
                                $" Min={Global.gConfigVM.LowTempAlarm} Temp Error");
                        }
                    }
                }
                uiIdx++;
            }

            double prevDimAvg = SubDimTempAvg;
            if (subMountCnt > 0)
            {
                if (_dimType == DIM_TYPE.MAX)
                    SubDimTempAvg = MaxInBox.Max();
                else if (_dimType == DIM_TYPE.MID)
                    SubDimTempAvg = (MaxInBox.Max() + MaxInBox.Min()) / 2;
                else
                    SubDimTempAvg = Math.Round(subDimAvg / subMountCnt, 1);    
            }
            else
            {
                SubDimTempAvg = 0;
            }

            MaxInBox.Clear();

            if (SampleShortTerm.Count >= MaxAllowedCount) SampleShortTerm.RemoveAt(0);
            SampleShortTerm.Add(SubDimTempAvg - prevDimAvg);
            if (SampleShortTerm.Count > ShortTermCount)
                SampleShortTerm.RemoveAt(0);

            if (SampleLongTerm.Count >= MaxAllowedCount) SampleLongTerm.RemoveAt(0);
            SampleLongTerm.Add(SubDimTempAvg);
            if (SampleLongTerm.Count > LongTermCount)
                SampleLongTerm.RemoveAt(0);

            double subBoxAvg = 0;
            subBoxAvg += InsideBoxTemp;
            pvAvg += InsideBoxTemp;

            subBoxAvg = Math.Round(subBoxAvg , 1);
            SubInsideTemp = subBoxAvg;

            if (warnMsg != "")
            {
                warnMsg += " Temp Warning";
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"{warnMsg}");
                //팝업 안나오고 로그에만 표기하게 변경
                //MasterRack.showAlram(ALRAM_TYPE.ERROR, $"[Prog][{MasterRack.RackViewIndex}][{Name}] : {warnMsg}", true);
            }
            if (isError)
            {
                BoxDefrost = DEFROST_TYPE.TEMPERROR;
                Thread.Sleep((BoxIndex % 6) * 100);
                Task.Run(() => MasterRack.RackstopTestAsync());
                return;
            }

            InsideBoxTemp = Math.Round(pvAvg , 1);
            if (mountCnt > 0) DimTempAvg = Math.Round(dimAvg / mountCnt, 1);
            BoxTemp = DimTempAvg; // BoxTemp는 Temp Box 출력용. 
        }

        public double TradingOrgSV = 0;
        public void processBoxSV()
        {
            /*
            if (Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort == null) return;
            if (!Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort.IsOpen) return;
            if (BoxPVText == "0" || BoxPVText == "" || BoxPVText == string.Empty)
            {
                SampleLongTerm.Clear();
                SampleShortTerm.Clear();
                return;
            }
            */

            if (SubDimTempAvg == 0) return;

            double max = Global.gConfigVM.ControlMaxTemp;
            double min = Global.gConfigVM.ControlMinTemp;

            double prevTarget = TargetSV; //RealTime SV
            double DIM_SV_Diff = BoxSetSV - SubDimTempAvg; //Org - DimmTemp
            double ctrl = TargetSV;

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
                    else if (down > 2)
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
                    else if (up > 2)
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
                                if (avg <= -0.8 && down > 2 &&
                                    ((trading == TRADING.RISE) ||
                                    (trading == TRADING.FALL && DIM_SV_Diff < 0.6)))
                                {
                                    /*
                                    Level1Check = 0;
                                    Level = 1;
                                    TradingOrgSV = ctrl;
                                    //sub.TargetSV += Math.Round((setSV * 8) / 100, 1);
                                    ctrl += Math.Round(DIM_SV_Diff * 1.5);
                                    */
                                    Level1Check = 0;
                                    Level = 1;
                                    OrgSV = ctrl;
                                    //sub.TargetSV += Math.Round((setSV * 8) / 100, 1);
                                    ctrl += 8;
                                    Console.WriteLine($"다운제어 시작 SV: {OrgSV} -> {ctrl}");
                                    SVCtrl |= (int)SVC.DOWN;

                                    SampleShortTerm.Clear();
                                    SampleLongTerm.Clear();
                                }

                                if (avg >= 0.8 && up > 2 &&
                                    ((trading == TRADING.FALL) ||
                                    (trading == TRADING.RISE && DIM_SV_Diff < 0.6)))
                                {
                                    /*
                                    Level = 1;
                                    //sub.TargetSV -= Math.Round((setSV * 8) / 100, 1);
                                    ctrl -= Math.Round(DIM_SV_Diff * 1.5);
                                    TradingOrgSV = ctrl;
                                    */
                                    Level = 1;
                                    OrgSV = ctrl;
                                    //sub.TargetSV -= Math.Round((setSV * 8) / 100, 1);
                                    ctrl -= 8;
                                    
                                    Console.WriteLine($"업 제어 시작 SV: {OrgSV} -> {ctrl}");
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
                setTemp(ctrl * 10);
        }
        public void processBoxSVSub()
        {
            if (SubDimTempAvg == 0) return;

            double max = Global.gConfigVM.ControlMaxTemp;
            double min = Global.gConfigVM.ControlMinTemp;

            double prevTarget = TargetSV;
            double DIM_SV_Diff = BoxSetSV - SubDimTempAvg;
            double ctrl = TargetSV;

            TRADING trading = TRADING.RISE;
            if (DIM_SV_Diff < 0) trading = TRADING.FALL;
            double direction = 1;
            if (trading == TRADING.FALL) direction = -1;

            DIM_SV_Diff = Math.Abs(DIM_SV_Diff);

            if (DIM_SV_Diff < Global.gConfigVM.ControlDiff)
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
                //////////////  DOWN Action
                /////////////////////////////
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
                            ctrl = OrgSV;
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
                            ctrl += Math.Round((DIM_SV_Diff * 1.6) * direction, 1);
                            SVCtrl ^= (int)SVC.DOWN;
                            SampleLongTerm.Clear();
                        }
                        else if (DIM_SV_Diff <= 1)
                        {
                            if (up > 0)  // 오르는중.
                            {

                                SampleShortTerm.Clear();
                                OrgSV = TargetSV;

                                //  85도일때 add 2.6, 40도일때 add 8
                                //  기울기 = -0.12, 옵셋 = 12.8                              
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
                            ctrl = OrgSV;
                            Level = 0;
                            SVCtrl ^= (int)SVC.DOWN;
                        }
                    }

                    Console.WriteLine($"DOWN-Ctrl {Name} Level:{orgLevel} ->  {Level} " +
                        $"Diff:{Math.Round(DIM_SV_Diff, 1)} Target:{Math.Round(ctrl, 1)} " +
                        $"- org:{OrgSV} avg:{Math.Round(avg, 1)} up:{up} down:{down}");
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
                        Console.WriteLine($"--- Up 제어완료 --- sv {ctrl} -> {OrgSV}");
                        ctrl = OrgSV;

                    }
                    //                    else 
                    //                        sub.TargetSV -= 4;

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

                            double org = TargetSV;
                            ctrl += Math.Round((stable_diff / 2) * direction, 1);
                            Console.WriteLine($"{Name} LongTerm Adjust {org} -> {ctrl}");
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
                                if (avg <= -1.0 && down > 2 &&
                                    ((trading == TRADING.RISE) ||
                                    (trading == TRADING.FALL && DIM_SV_Diff < 0.8)))
                                {
                                    Level1Check = 0;
                                    Level = 1;
                                    OrgSV = ctrl;
                                    //sub.TargetSV += Math.Round((setSV * 8) / 100, 1);
                                    ctrl += 8;

                                    Console.WriteLine($"다운제어 시작 {Name} SV: {OrgSV} -> {ctrl}");
                                    SVCtrl |= (int)SVC.DOWN;

                                    SampleShortTerm.Clear();
                                    SampleLongTerm.Clear();
                                }

                                if (avg >= 1.0 && up > 2 &&
                                    ((trading == TRADING.FALL) ||
                                    (trading == TRADING.RISE && DIM_SV_Diff < 0.8)))
                                {
                                    Level = 1;
                                    //sub.TargetSV -= Math.Round((setSV * 8) / 100, 1);
                                    ctrl -= 8;
                                    OrgSV = ctrl;
                                    Console.WriteLine($"업 제어 시작 {Name} SV: {OrgSV} -> {ctrl}");
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
                ctrl = BoxSetSV + (10.0 * direction);
            }

        //   if (sub.TargetSV > 120 || sub.TargetSV < 16)
        //   {
        //       MasterRack.showAlram(ALRAM_TYPE.ERROR, $"{Name} TargetSV={sub.TargetSV} " +
        //           $"SV upper limit is 120 degrees and lower limit is 16 degrees");
        //       BoxStat = BOX_STAT.ERR;
        //   }


        SVDone:
            if (ctrl > max) ctrl = max;
            else if (ctrl < min) ctrl = min;

            if (prevTarget != ctrl)
                setTemp(ctrl * 10);
        }
        public void processBoxSVThrid()
        {
            if (SubDimTempAvg == 0) return;

            double max = Global.gConfigVM.ControlMaxTemp;
            double min = Global.gConfigVM.ControlMinTemp;

            double prevTarget = TargetSV;
            double DIM_SV_Diff = BoxSetSV - SubDimTempAvg;
            double ctrl = TargetSV;

            TRADING trading = TRADING.RISE;
            if (DIM_SV_Diff < 0) trading = TRADING.FALL;
            double direction = 1;
            if (trading == TRADING.FALL) direction = -1;

            DIM_SV_Diff = Math.Abs(DIM_SV_Diff);

            if (DIM_SV_Diff < Global.gConfigVM.ControlDiff)
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
                //////////////  DOWN Action
                /////////////////////////////
                if (checkSV(SVC.DOWN))
                {
                    if (SampleShortTerm.Count <= Global.gConfigVM.DownCatchshortmaxcount) goto SVDone;
                    (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                    int orgLevel = Level;
                    if (Level == 1)
                    {
                        if (up > 0) // 상승 시작
                        {
                            Console.Write($"-- 상승시작 -- ");
                            SampleShortTerm.Clear();
                            ctrl = OrgSV;
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
                            ctrl += Math.Round((DIM_SV_Diff * 1.6) * direction, 1);
                            SVCtrl ^= (int)SVC.DOWN;
                            SampleLongTerm.Clear();
                        }
                        else if (DIM_SV_Diff <= 1)
                        {
                            if (up > 0)  // 오르는중.
                            {

                                SampleShortTerm.Clear();
                                OrgSV = TargetSV;

                                //  85도일때 add 2.6, 40도일때 add 8
                                //  기울기 = -0.12, 옵셋 = 12.8                              
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
                            ctrl = OrgSV;
                            Level = 0;
                            SVCtrl ^= (int)SVC.DOWN;
                        }
                    }

                    Console.WriteLine($"DOWN-Ctrl {Name} Level:{orgLevel} ->  {Level} " +
                        $"Diff:{Math.Round(DIM_SV_Diff, 1)} Target:{Math.Round(ctrl, 1)} " +
                        $"- org:{OrgSV} avg:{Math.Round(avg, 1)} up:{up} down:{down}");
                    goto SVDone;
                }

                //////////////////////////
                ////  UP Action
                //////////////////////////
                if (checkSV(SVC.UP))
                {
                    (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                    if (down > Global.gConfigVM.UpActionDownValue) //default == 1
                    {

                        SampleShortTerm.Clear();
                        SVCtrl ^= (int)SVC.UP;
                        Console.WriteLine($"--- Up 제어완료 --- sv {ctrl} -> {OrgSV}");
                        ctrl = OrgSV;

                    }
                    //                    else 
                    //                        sub.TargetSV -= 4;

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

                            double org = TargetSV;
                            ctrl += Math.Round((stable_diff / 2) * direction, 1);
                            Console.WriteLine($"{Name} LongTerm Adjust {org} -> {ctrl}");
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
                            if (DIM_SV_Diff <= Global.gConfigVM.MaxDIM_SV_Diffcount)//defalut == 5
                            {
                                (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                                double sd = Math.Round(calcSD(SampleShortTerm), 3);

                                avg = Math.Round(avg, 1);
                                //Console.WriteLine($"{Name} SortTerm 편차:{sd} avg:{avg} u:{up} d:{down} T:{trading.ToString()}");
                                if (avg <= Global.gConfigVM.DownCatchAvgValue && down > Global.gConfigVM.DownCatchDownValue && //down == 1 avg == -0,8
                                    ((trading == TRADING.RISE) ||
                                    (trading == TRADING.FALL && DIM_SV_Diff < Global.gConfigVM.UPDownCatchDIMSVDiffValue))) //DimSVDiff == 0.6
                                {
                                    Level1Check = 0;
                                    Level = 1;
                                    OrgSV = ctrl;
                                    //sub.TargetSV += Math.Round((setSV * 8) / 100, 1);
                                    ctrl += Global.gConfigVM.SDupvalue; //default == 6

                                    Console.WriteLine($"다운제어 시작 {Name} SV: {OrgSV} -> {ctrl}");
                                    SVCtrl |= (int)SVC.DOWN;

                                    SampleShortTerm.Clear();
                                    SampleLongTerm.Clear();
                                }

                                if (avg >= Global.gConfigVM.UPCatchAvgValue && up > Global.gConfigVM.UPCatchUPValue && // up == 1 avg == 0.8
                                    ((trading == TRADING.FALL) ||
                                    (trading == TRADING.RISE && DIM_SV_Diff < Global.gConfigVM.UPDownCatchDIMSVDiffValue))) //DimSVDiff == 0.6
                                {
                                    Level = 1;
                                    //sub.TargetSV -= Math.Round((setSV * 8) / 100, 1);
                                    ctrl -= Global.gConfigVM.SUdownvalue; //default == 8
                                    OrgSV = ctrl;
                                    Console.WriteLine($"업 제어 시작 {Name} SV: {OrgSV} -> {ctrl}");
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
                ctrl = BoxSetSV + (10.0 * direction);
            }

        //   if (sub.TargetSV > 120 || sub.TargetSV < 16)
        //   {
        //       MasterRack.showAlram(ALRAM_TYPE.ERROR, $"{Name} TargetSV={sub.TargetSV} " +
        //           $"SV upper limit is 120 degrees and lower limit is 16 degrees");
        //       BoxStat = BOX_STAT.ERR;
        //   }


        SVDone:
            if (ctrl > max) ctrl = max;
            else if (ctrl < min) ctrl = min;

            if (prevTarget != ctrl)
                setTemp(ctrl * 10);
        }
        public void processBoxSVFourth()
        {
            if (SubDimTempAvg == 0) return;

            double max = Global.gConfigVM.ControlMaxTemp;
            double min = Global.gConfigVM.ControlMinTemp;
            double prevTarget = TargetSV;
            double DIM_SV_Diff = BoxSetSV - SubDimTempAvg;
            double ctrl = TargetSV;

            TRADING trading = TRADING.RISE;
            if (DIM_SV_Diff < 0) trading = TRADING.FALL;
            double direction = 1;
            if (trading == TRADING.FALL) direction = -1;

            DIM_SV_Diff = Math.Abs(DIM_SV_Diff);

            try
            {
                if (DIM_SV_Diff < Global.gConfigVM.ControlDiff)
                {
                    // ACCELERATION 단계
                    if (checkSV(SVC.ACCELERATION))
                    {
                        ctrl = BoxSetSV;
                        SVST(SVC.TCON);
                    }

                    //////////////////////////
                    //////////////  DOWN Action
                    /////////////////////////////
                    if (checkSV(SVC.DOWN))
                    {
                        if (SampleShortTerm.Count <= Global.gConfigVM.DownCatchshortmaxcount) goto SVDone;
                        (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                        int orgLevel = Level;
                        if (Level == 1)
                        {
                            if (up > 0) // 상승 시작
                            {
                                Console.Write($"-- 상승시작 -- ");
                                SampleShortTerm.Clear();
                                ctrl = OrgSV;
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
                                ctrl += Math.Round((DIM_SV_Diff * 1.6) * direction, 1);
                                SVCtrl ^= (int)SVC.DOWN;
                                SampleLongTerm.Clear();
                            }
                            else if (DIM_SV_Diff <= 1)
                            {
                                if (up > 0)  // 오르는중.
                                {

                                    SampleShortTerm.Clear();
                                    OrgSV = TargetSV;

                                    //  85도일때 add 2.6, 40도일때 add 8
                                    //  기울기 = -0.12, 옵셋 = 12.8                              
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
                                ctrl = OrgSV;
                                Level = 0;
                                SVCtrl ^= (int)SVC.DOWN;
                            }
                        }

                        Console.WriteLine($"DOWN-Ctrl {Name} Level:{orgLevel} ->  {Level} " +
                            $"Diff:{Math.Round(DIM_SV_Diff, 1)} Target:{Math.Round(ctrl, 1)} " +
                            $"- org:{OrgSV} avg:{Math.Round(avg, 1)} up:{up} down:{down}");
                        goto SVDone;
                    }

                    //////////////////////////
                    ////  UP Action
                    //////////////////////////
                    if (checkSV(SVC.UP))
                    {
                        (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                        if (down > Global.gConfigVM.UpActionDownValue) //default == 1
                        {

                            SampleShortTerm.Clear();
                            SVCtrl ^= (int)SVC.UP;
                            Console.WriteLine($"--- Up 제어완료 --- sv {ctrl} -> {OrgSV}");
                            ctrl = OrgSV;

                        }
                        //                    else 
                        //                        sub.TargetSV -= 4;

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

                                double org = TargetSV;
                                ctrl += Math.Round((stable_diff / 2) * direction, 1);
                                Console.WriteLine($"{Name} LongTerm Adjust {org} -> {ctrl}");
                                SampleLongTerm.Clear();
                                SampleShortTerm.Clear();
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
                                if (DIM_SV_Diff <= Global.gConfigVM.MaxDIM_SV_Diffcount)//defalut == 5
                                {
                                    (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                                    double sd = Math.Round(calcSD(SampleShortTerm), 3);

                                    avg = Math.Round(avg, 1);
                                    //Console.WriteLine($"{Name} SortTerm 편차:{sd} avg:{avg} u:{up} d:{down} T:{trading.ToString()}");
                                    if (avg <= Global.gConfigVM.DownCatchAvgValue && down > Global.gConfigVM.DownCatchDownValue && //down == 1 avg == -0,8
                                        ((trading == TRADING.RISE) ||
                                        (trading == TRADING.FALL && DIM_SV_Diff < Global.gConfigVM.UPDownCatchDIMSVDiffValue))) //DimSVDiff == 0.6
                                    {
                                        Level1Check = 0;
                                        Level = 1;
                                        OrgSV = ctrl;
                                        //sub.TargetSV += Math.Round((setSV * 8) / 100, 1);
                                        ctrl += Global.gConfigVM.SDupvalue; //default == 6

                                        Console.WriteLine($"다운제어 시작 {Name} SV: {OrgSV} -> {ctrl}");
                                        SVCtrl |= (int)SVC.DOWN;

                                        SampleShortTerm.Clear();
                                        SampleLongTerm.Clear();
                                    }

                                    if (avg >= Global.gConfigVM.UPCatchAvgValue && up > Global.gConfigVM.UPCatchUPValue && // up == 1 avg == 0.8
                                        ((trading == TRADING.FALL) ||
                                        (trading == TRADING.RISE && DIM_SV_Diff < Global.gConfigVM.UPDownCatchDIMSVDiffValue))) //DimSVDiff == 0.6
                                    {
                                        Level = 1;
                                        //sub.TargetSV -= Math.Round((setSV * 8) / 100, 1);
                                        ctrl -= Global.gConfigVM.SUdownvalue; //default == 8
                                        OrgSV = ctrl;
                                        Console.WriteLine($"업 제어 시작 {Name} SV: {OrgSV} -> {ctrl}");
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
                    ctrl = BoxSetSV + (10.0 * direction);
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[ProcessBoxSV] 예외 발생: {ex.Message}");
            }

        //   if (sub.TargetSV > 120 || sub.TargetSV < 16)
        //   {
        //       MasterRack.showAlram(ALRAM_TYPE.ERROR, $"{Name} TargetSV={sub.TargetSV} " +
        //           $"SV upper limit is 120 degrees and lower limit is 16 degrees");
        //       BoxStat = BOX_STAT.ERR;
        //   }


        SVDone:
            if (ctrl > max) ctrl = max;
            else if (ctrl < min) ctrl = min;

            if (prevTarget != ctrl)
                setTemp(ctrl * 10);
        }

        public void processBoxSVFifth()
        {
            if (SubDimTempAvg == 0) return;

            double max = Global.gConfigVM.ControlMaxTemp;
            double min = Global.gConfigVM.ControlMinTemp;

            double prevTarget = TargetSV;
            double DIM_SV_Diff = BoxSetSV - SubDimTempAvg;
            double ctrl = TargetSV;

            TRADING trading = TRADING.RISE;
            if (DIM_SV_Diff < 0) trading = TRADING.FALL;
            double direction = 1;
            if (trading == TRADING.FALL) direction = -1;

            DIM_SV_Diff = Math.Abs(DIM_SV_Diff);

            if (DIM_SV_Diff < Global.gConfigVM.ControlDiff)
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

                            double org = TargetSV;
                            ctrl += Math.Round((stable_diff / 2) * direction, 1);
                            Console.WriteLine($"{Name} LongTerm Adjust {org} -> {ctrl}");
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

            }
            else
            {
                SVST(SVC.ACCELERATION);
                ctrl = BoxSetSV + (10.0 * direction);
            }

        //   if (sub.TargetSV > 120 || sub.TargetSV < 16)
        //   {
        //       MasterRack.showAlram(ALRAM_TYPE.ERROR, $"{Name} TargetSV={sub.TargetSV} " +
        //           $"SV upper limit is 120 degrees and lower limit is 16 degrees");
        //       BoxStat = BOX_STAT.ERR;
        //   }


        SVDone:
            if (ctrl > max) ctrl = max;
            else if (ctrl < min) ctrl = min;

            if (prevTarget != ctrl)
                setTemp(ctrl * 10);
        }
        bool pos = false;
        public void TestSV_Control()
        {
            double ctrl = TargetSV;
            if (TargetSV > 100)
                pos = true;
            else if (TargetSV < 0)
                pos = false;

            if (pos)
                ctrl -= 0.5;
            else
                ctrl += 0.5;

            setTemp(ctrl * 10);

        }
    }
}
