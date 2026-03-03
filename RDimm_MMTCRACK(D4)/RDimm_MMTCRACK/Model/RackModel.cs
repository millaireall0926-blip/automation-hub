using HandyControl.Expression.Shapes;
using HandyControl.Tools.Extension;
using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.Net;
using RDimm_MMTCRACK.View.Component;
using RDimm_MMTCRACK.ViewModel;
using PropertyChanged;
using Serilog;
using Serilog.Core;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Media;
using System.Windows.Threading;
using System.Xml;
using System.Xml.Linq;

namespace RDimm_MMTCRACK.Model
{
    public enum RACK_STAT
    {
        /*
        RUN = 0,
        STOP,
        ERROR,
        IDLE,
        DONE,
        */
        IDLE = 0,
        STOP,
        PRERUN,
        RUN,
        DONE,
        DEFROST,
        ERROR
    }
    public enum MOTOR_STAT
    {
        IDLE = 0,
        NORMAL,
        ERROR,
    }
    public enum ALRAM_TYPE
    {
        DONE = 0,
        INFO,
        WARN,
        ERROR,
        INIT
    }
    public class RackModel : BaseVM
    {
        public string Name { get; set; }
        [DoNotNotify] public int RackIndex { get; set; } = 0;
        [DoNotNotify] public int RackViewIndex { get; set; } = 0;
        public SolidColorBrush BoarderColor { get; set; }
        public SolidColorBrush RackColor { get; set; } = Global.gRackColors[0];
        public SolidColorBrush MotorStatColor { get; set; } = Global.gMotorStatColors[1];
        public SolidColorBrush FlowStatColor { get; set; } = Global.gWarningColors[0];
        public SolidColorBrush FlowRateColor { get; set; } = Global.gWarningColors[0];

        public DateTime TimeStart { get; set; }
        public DateTime TimeEnd { get; set; }
        Nullable<DateTime> TempAlarmTime { get; set; }
        Nullable<DateTime> TempErrorTime { get; set; }
        public string MaxPV { get; set; } = "";

        public bool IsRackTamsEnable { get; set; } = false;

        public Visibility RackEngrmodeVisibility { get; set; } = Visibility.Hidden;
        bool _isRacktcunit = false;
        public bool IsRackTCUnit
        {
            get => _isRacktcunit;
            set
            {
                _isRacktcunit = value;
                if (_isRacktcunit)
                    RackColor = Global.gRackColors[0];
                else
                    RackColor = Global.gRackColors[2];
            }
        }
        bool _isRackTams = true;
        public bool IsRackTams
        {
            get => _isRackTams;
            set
            {
                _isRackTams = value;
            }
        }
        bool _isRackAS = false;
        public bool IsRackAS
        {
            get => _isRackAS;
            set
            {
                _isRackAS = value;
                if (_isRackAS)
                    RackColor = Global.gRackColors[1];
                else
                    RackColor = Global.gRackColors[0];
            }
        }
        //public double RackSetSV = 0;
        public bool EnableSV { get; set; } = true;

        public double _RacksetSV = 0;
        public double RackSetSV
        {
            get => _RacksetSV;
            set
            {
                _RacksetSV = value;
                foreach (CBoxModel bm in CBoxs)
                    bm.BoxSetSV = _RacksetSV;
                //Console.WriteLine($"settemp {_setSV}");

            }
        }
        /*
        int _boxruncnt = 0;
        public int BoxRunCnt
        {
            get => _boxruncnt;
            set
            {
                if (value > 3)
                    _boxruncnt = 3;
                else if (value < 0)
                    _boxruncnt = 0;
                else
                    _boxruncnt = value;
            }
        }
        */
        public MOTOR_STAT RackMotorAlarm { get; set; } = MOTOR_STAT.NORMAL;
        //TEST용 막아놈
        string _motorstattext = string.Empty;
        public string MotorStatText
        {
            get => _motorstattext;
            set
            {
                _motorstattext = value;
                Global.gEngineerVM.ManualMotor[RackIndex].eMotorStatText = _motorstattext;
                if (_motorstattext == "OPEN")
                {
                    MotorStatColor = Global.gMotorStatColors[1];
                    Global.gEngineerVM.ManualMotor[RackIndex].eMotorStatColor = Global.gMotorStatColors[1];
                }
                else if (_motorstattext == "CLOSE")
                {
                    MotorStatColor = Global.gMotorStatColors[0];
                    Global.gEngineerVM.ManualMotor[RackIndex].eMotorStatColor = Global.gMotorStatColors[0];
                }

            }
        }
        string _flowstattext = string.Empty;
        public string FlowStatText
        {
            get => _flowstattext;
            set
            {
                _flowstattext = value;
            }
        }
        //TEST용 막아놈
        double _flowrate = 0;
        public double FlowRate
        {
            get => _flowrate;
            set
            {
                _flowrate = value;
            }
        }
        public void SetFlowRate(double value)
        {
            FlowRate = value;
            FlowRateAlarmCheck();
        }
        void FlowRateAlarmCheck()
        {
            if (_flowrate < FlowMinLimit)
            {
                FlowStatText = "저유량 경고";
                FlowStatColor = Global.gWarningColors[1];// Red
                FlowRateColor = Global.gWarningColors[1];// Red
            }
            else if (_flowrate > FlowMaxLimit)
            {
                FlowStatText = "고유량 경고";
                FlowStatColor = Global.gWarningColors[1];// Red
                FlowRateColor = Global.gWarningColors[1];// Red
            }
            else
            {
                FlowStatText = "정상 유량";
                FlowStatColor = Global.gWarningColors[0];
                FlowRateColor = Global.gWarningColors[0];
            }
        }
        public int FlowMinLimit { get; set; } = 5;
        public int FlowMaxLimit { get; set; } = 50;
        public string RackStatText { get; set; } = string.Empty;
        public string DefrostTime {  get; set; } = string.Empty;

        //보류 0320
        //public string RemainDefosrtTime { get; set; } = string.Empty;

        bool _islideopen = false;
        public bool IsSlideOpen
        {
            get => _islideopen;
            set
            {
                _islideopen = value;
            }
        }
        public void GetSlideOpen(bool isOpen)
        {
            bool wasOpen = IsSlideOpen;
			bool skipAlarm = RackStat == RACK_STAT.PRERUN || RackStat == RACK_STAT.RUN || RackStat == RACK_STAT.DEFROST;

            if(isOpen && !wasOpen)
            {
                if(!skipAlarm)
                    ShowSlideAlarm(ALRAM_TYPE.ERROR, Global.msg_SlideOpen);
                else
                    HideSlideAlarm();

                if(RackStat == RACK_STAT.DONE)
                    ResetAfterCompletionBySlideOpen();
            }

            if(!isOpen && wasOpen)
                HideSlideAlarm();

            IsSlideOpen = isOpen;

            /*
            if (getStat)
            {
                Console.WriteLine($"{RackViewIndex} : {RackStat.ToString()}");

                if (skipAlarm)
                {
					HideSlideAlarm(skipAlarm);
				}
                else
                {
                    ShowSlideAlarm(ALRAM_TYPE.ERROR, Global.msg_SlideOpen);
                }
            }
            else
				HideSlideAlarm(skipAlarm);

			IsSlideOpen = getStat;
            */
        }

		public Visibility SlideShowBox { get; set; } = Visibility.Hidden;
		public string SlideMsg { get; set; } = "";
		public SolidColorBrush SlideMsgColor { get; set; }

		public void ShowSlideAlarm(ALRAM_TYPE type, string msg)
		{
			SlideMsgColor = Global.gAlramColors[(int)type];
			SlideMsg = msg;
			SlideShowBox = Visibility.Visible;
		}
		public void HideSlideAlarm()
		{
			SlideShowBox = Visibility.Hidden;
		}
        private void ResetAfterCompletionBySlideOpen()
        {
            ShowBox = Visibility.Hidden;
            DefrostTime = string.Empty;
            Rack_Defrost = DEFROST_TYPE.NORMAL;
            RackStat = RACK_STAT.STOP;
            InitializeAllBoxes(ReadBoxState: true);
        }

        RACK_STAT _rackStat = RACK_STAT.STOP;
        public RACK_STAT RackStat
        {
            get => _rackStat;
            set
            {
                _rackStat = value;
                BoarderColor = Global.gBoarderColors[(int)_rackStat];
                if (_rackStat == RACK_STAT.DEFROST)
                    RackStatText = "제상중";
                else
                    RackStatText = _rackStat.ToString();
            }
        }

        public DEFROST_TYPE Rack_Defrost = DEFROST_TYPE.NORMAL;

        public MOTOR_STAT RackMotorStat = MOTOR_STAT.IDLE;

        Visibility _showbox = Visibility.Hidden;
        public Visibility ShowBox 
        { 
            get => _showbox; 
            set
            {
                _showbox = value;
            }
        }
        public int BoxTCUnitCnt { get; set; } = 0;
        public int BoxASCnt { get; set; } = 0;
        public int BoxTamsCnt { get; set; } = 0;
        public string RackMsg { get; set; } = "";
        public SolidColorBrush RackMsgColor { get; set; }
        public void showAlram(ALRAM_TYPE type, string msg, bool isLogging = true)
        {
            RackMsgColor = Global.gAlramColors[(int)type];
            RackMsg = msg;
            ShowBox = Visibility.Visible;


            if (isLogging)
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Fix {RackViewIndex}] {msg}");
        }
        [DoNotNotify] public int GraphIndex { get; set; } = 0;
        [DoNotNotify] public List<DimDataModel> DimDatas { get; set; } = new List<DimDataModel>();
        [DoNotNotify] public List<MarkerModel> FailMakers { get; set; } = new List<MarkerModel>();

        //ObservableCollection<BoxModel> _boxes = new ObservableCollection<BoxModel>();
        //public ObservableCollection<BoxModel> Boxes { get { return _boxes; } set { _boxes = value; PC(nameof(Boxes)); } }

        ObservableCollection<CBoxModel> _cboxs = new ObservableCollection<CBoxModel>();
        public ObservableCollection<CBoxModel> CBoxs
        {
            get { return _cboxs; }
            set { _cboxs = value; PC(nameof(CBoxs)); }
        }

        public DelegateCommand RackCmdRun { get; set; }
        public DelegateCommand RackCmdStop { get; set; }
        public DelegateCommand RackManualUp { get; set; }
        public DelegateCommand RackManualDown { get; set; }
        public DelegateCommand DGCmdSetTemp { get; set; }

		public DelegateCommand RackPopUpClear { get; set; }
		#region Log 
		[DoNotNotify]
        public Logger TempSerilog { get; set; } = null;
        #endregion
        //public SiteCom sc;
        //public CaptureCom cc;
        public RackModel(int ch)
        {
            RackViewIndex = (ch + 1);
            RackIndex = ch;

            IsRackTCUnit = true;

            var node = Global.gConfigDoc.SelectSingleNode("lpcam-rack/rack-map/rack");
            var rackNode = node.SelectSingleNode($"//rack[@id='{RackIndex}']");

            /*
            //SiteCom Set
            sc = new SiteCom();
            sc.SitePortName = node.Attributes["site-port"].Value;
            sc.MotorValue = node.Attributes["site-val"].Value;
            sc.SitePortUse = Convert.ToInt32(node.Attributes["site-use"].Value) == 1 ? true : false;
            sc.MotorFinalStep = node.Attributes["site-final"].Value;
            sc.MotorPositon = Convert.ToInt32(node.Attributes["site-position"].Value) == 1 ? true : false;

            //CaptureCom Set
            cc = new CaptureCom();
            cc.CaputurePortName = node.Attributes["capture-port"].Value;
            cc.CaputureUse = Convert.ToInt32(node.Attributes["capture-use"].Value) == 1 ? true : false;
            */
            int i = 0;
            foreach (XmlNode boxNode in rackNode.ChildNodes)
            {
                CBoxModel box = new CBoxModel(boxNode.Attributes["name"].Value, this);
                box.BoxIndex = i++;
                box.BoxViewIndex = i;

                var itemNode = boxNode.SelectSingleNode("dim");
                string dimType = itemNode.Attributes["type"].Value;
                foreach (DIM_TYPE type in System.Enum.GetValues(typeof(DIM_TYPE)))
                {
                    if (dimType.ToUpper() == type.ToString())
                    { box._dimType = type; break; }
                }

                string text = itemNode.InnerText;
                string[] buf = text.Split(',');
                foreach (string s in buf)
                {
                    if (s == "") break;
                    box.DimCtrlList.Add(Convert.ToInt32(s));
                    box.DimModels.Add(new DimModel());
                }

                box.DimColumns = 4;
                if (Global.gMainPageVM.MaxDimmCount == 24)
                    box.DimColumns = 6;
                else if (Global.gMainPageVM.MaxDimmCount == 32)
                    box.DimColumns = 8;
                else if(Global.gMainPageVM.MaxDimmCount == 12)
                    box.DimColumns = 3;

                CBoxs.Add(box);

                //int id = Convert.ToInt32(boxNode.Attributes["id"].Value);
                //Boxes.Add(new BoxModel((RackIndex * 3) + id, this));
                //Boxes[i].HumidityOffset = Convert.ToInt32(boxNode.Attributes["humidityoffset"].Value);
                //Boxes[i].IsBoxTCUnit = Convert.ToInt32(boxNode.Attributes["tc-unit"].Value) == 1 ? true : false;
                //Boxes[i].IsBoxAS = Convert.ToInt32(boxNode.Attributes["as"].Value) == 1 ? true : false;

                //Boxes[i].ISBoxTams = Convert.ToInt32(boxNode.Attributes["tams"].Value) == 1 ? true : false;

            }

            initDimData();

            RackStat = RACK_STAT.IDLE;
            RackMotorStat = MOTOR_STAT.NORMAL;
            RackCmdRun = new DelegateCommand(p => { Task.Run(() => RackstartTest()); });
            RackCmdStop = new DelegateCommand(p => { Task.Run(() => RackstopTestAsync()); });
            RackManualUp = new DelegateCommand(p => { Task.Run(() => PreMotorAutoOpen(true)); });
            RackManualDown = new DelegateCommand(p => { Task.Run(() => PreMotorAutoDown()); });
            DGCmdSetTemp = new DelegateCommand(p => { Task.Run(() => RacksetTemp()); });

			RackPopUpClear = new DelegateCommand(p => { RackPopUpClearExecute(); });
		}
        public void initDimData()
        {
            for (int i = 0; i < Global.gMainPageVM.MaxDimmCount; i++)
            {
                DimDatas.Add(new DimDataModel());
            }

        }

		private void RackPopUpClearExecute()
		{
            if (RackStat != RACK_STAT.STOP && RackStat != RACK_STAT.DONE && RackStat != RACK_STAT.ERROR && RackStat != RACK_STAT.IDLE)
                return;

            ShowBox = Visibility.Hidden;

			if (RackMotorStat == MOTOR_STAT.ERROR)
				RackMotorStat = MOTOR_STAT.NORMAL;

            if(IsRackInCompletionState())
            {
                DefrostTime = string.Empty;
                RackStat = RACK_STAT.STOP;
                Rack_Defrost = DEFROST_TYPE.NORMAL;
                InitializeAllBoxes(ReadBoxState: true);
            }
            else
            {
                ProcessIndividualBoxStates();
            }
            /*
            if (RackStat == RACK_STAT.DONE || RackStat == RACK_STAT.ERROR || RackStat == RACK_STAT.STOP)
            {
                //제상 시간 초기화
                DefrostTime = string.Empty;

                RackStat = RACK_STAT.STOP;
                Rack_Defrost = DEFROST_TYPE.NORMAL;

                foreach (CBoxModel bm in CBoxs)
                {

                    Task.Run(() => bm.Read_BoxTConStat());
                    bm.CBoxStat = CBOX_STAT.STOP;
                    bm.BoxDefrost = DEFROST_TYPE.NORMAL;
                    bm.Sopen_Stat = true;
                    bm.Bimetal_Stat = true;
                }
            }

			foreach (CBoxModel bm in CBoxs)
			{
				if (bm.CBoxStat == CBOX_STAT.ERROR)
				{
					Task.Run(() => bm.Read_BoxTConStat());
					bm.CBoxStat = CBOX_STAT.STOP;
					bm.BoxDefrost = DEFROST_TYPE.NORMAL;
					bm.Sopen_Stat = true;
					bm.Bimetal_Stat = true;
				}
				else if (bm.CBoxStat == CBOX_STAT.DEFROST)
				{
					if (bm.MasterRack.RackStat == RACK_STAT.STOP)
					{
						Task.Run(() => bm.Read_BoxTConStat());
						bm.CBoxStat = CBOX_STAT.STOP;
						bm.BoxDefrost = DEFROST_TYPE.NORMAL;
						bm.Sopen_Stat = true;
						bm.Bimetal_Stat = true;
					}
				}
				else if (bm.CBoxStat == CBOX_STAT.DONE || bm.CBoxStat == CBOX_STAT.STOP)
				{
					bm.CBoxStat = CBOX_STAT.STOP;
					bm.BoxDefrost = DEFROST_TYPE.NORMAL;
					//bm.DefostTime = string.Empty;
					bm.Sopen_Stat = true;
					bm.Bimetal_Stat = true;
				}
			}
            */
		}

        private bool IsRackInCompletionState()
        {
            return RackStat == RACK_STAT.DONE ||
                   RackStat == RACK_STAT.ERROR ||
                   RackStat == RACK_STAT.STOP;
        }
        private void InitializeAllBoxes(bool ReadBoxState = false)
        {
            foreach (CBoxModel bm in CBoxs)
            {
                if (ReadBoxState)
                    Task.Run(() => bm.Read_BoxTConStat());

                ResetBoxToInitial(bm);
            }
        }
        private void ProcessIndividualBoxStates()
        {
            foreach (CBoxModel bm in CBoxs)
            {
                switch (bm.CBoxStat)
                {
                    case CBOX_STAT.ERROR:
                        Task.Run(() => bm.Read_BoxTConStat());
                        ResetBoxToInitial(bm);
                        break;

                    case CBOX_STAT.DEFROST when bm.MasterRack.RackStat == RACK_STAT.STOP:
                        Task.Run(() => bm.Read_BoxTConStat());
                        ResetBoxToInitial(bm);
                        break;

                    case CBOX_STAT.DONE:
                    case CBOX_STAT.STOP:
                        ResetBoxToInitial(bm);
                        break;
                }
            }
        }
        private void ResetBoxToInitial(CBoxModel bm)
        {
            bm.CBoxStat = CBOX_STAT.STOP;
            bm.BoxDefrost = DEFROST_TYPE.NORMAL;
            bm.Sopen_Stat = true;
            bm.Bimetal_Stat = true;
        }

        public double[] _prevDimmTemps = new double[64];
        public async void SiteBDVersionCheck()
        {
            Global.gEngineerVM.ManualMotor[RackIndex].GetSiteVer = string.Empty;
            await Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("F");
        }
        public async void RackStatusClear()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Fix" + RackViewIndex + "] Status Clear Button Click.");

            bool finalStatus = false;

            if(RackStat == RACK_STAT.DEFROST) 
                finalStatus = true;

            ShowBox = Visibility.Hidden;
            if (RackMotorStat == MOTOR_STAT.ERROR)
                RackMotorStat = MOTOR_STAT.NORMAL;

            RackStat = RACK_STAT.STOP;
            foreach (CBoxModel bm in CBoxs)
                bm.BoxStatusClear();

            showAlram(ALRAM_TYPE.INIT, Global.msg_StatusClear, false);

            if(DefrostTime != string.Empty)
                //제상 시간 초기화
                DefrostTime = string.Empty;

            /*보류 0320
            if (RemainDefosrtTime != string.Empty)
                RemainDefosrtTime = string.Empty;
            */
            if (finalStatus)
                await Task.Delay(1000 * 60);
            else
                await Task.Delay(1000 * 20);

            ShowBox = Visibility.Hidden;

            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Fix" + RackViewIndex + "] Status Popup Clear.");

            if (Global.gMainPageVM.AllRackRunCnt == 0 && Global.gAlarmCom.AllRackStatus)
            {
                //Console.WriteLine("경광등 STOP");
                Global.gAlarmCom.AlarmWrite("S");
                Global.gAlarmCom.AllRackStatus = false;
                Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.STOP;
                Global.gMainPageVM.endHumdityLog();
                Global.gMainPageVM.endTempLog();
            }
        }
        public void RackTconRun()
        {
            foreach (CBoxModel bm in CBoxs)
                bm.TCon_BoxRun();
        }
        public void RackTconStop()
        {
            foreach (CBoxModel bm in CBoxs)
                bm.TCon_BoxStop();
        }
        public void RacksetTemp()
        {
            for (int i = 0; i < CBoxs.Count; i++)
            {
                if (!IsRackTCUnit || IsRackAS)
                    continue;
                CBoxs[i].BoxSetSV = RackSetSV;
                CBoxs[i].setTemp();
                TimeStart = DateTime.Now;
                TempErrorTime = null;
                TempAlarmTime = null;
            }
        }

        private bool CanStartTest(out string logMsg)
        {
            logMsg = string.Empty;

            if (IsRackAS) return false;

            if (RackStat != RACK_STAT.STOP && RackStat != RACK_STAT.DONE && RackStat != RACK_STAT.IDLE)
                return false;

            if (IsSlideOpen || SlideShowBox == Visibility.Visible)
            {
                logMsg = "Slide Open Alarm Active. Cannot Start Test.";
                return false;
            }

            if (ShowBox == Visibility.Visible)
            {
                bool isClearable = RackMsg == Global.msg_TamsTestCompleted ||
                                   RackMsg == Global.msg_TestCompleted ||
                                   RackMsg == Global.msg_TamsFile;

                if (isClearable) ShowBox = Visibility.Hidden;
                else
                {
                    logMsg = "Fix Error Msg Check. After RUN Click.";
                    return false;
                }
            }

            if (!IsRackTCUnit)
            {
                showAlram(ALRAM_TYPE.WARN, Global.msg_Uncheck, isLogging: false);
                RackStat = RACK_STAT.ERROR;
                return false;
            }

            var port = Global.gMainPageVM.sc[RackIndex].SitePort;
            if(port == null || !port.IsOpen)
                return false;

            return true;
        }

        public async void RackstartTest()
        {
            if(!CanStartTest(out string logMsg))
            {
                if(!string.IsNullOrEmpty(logMsg))
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Fix{RackViewIndex}] {logMsg}");
                return;
            }

            bool MotorResult = false;

			#region
			/*
            if (IsRackAS) return;
            if (!(RackStat == RACK_STAT.STOP
                || RackStat == RACK_STAT.DONE
                || RackStat == RACK_STAT.IDLE
                )) return;

            if(IsSlideOpen || SlideShowBox == Visibility.Visible)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Fix" + RackViewIndex + "] Slide Open Alarm Active. Cannot Start Test.");
                return;
            }

            if (ShowBox == Visibility.Visible && (RackMsg == Global.msg_TamsTestCompleted || RackMsg == Global.msg_TestCompleted || RackMsg == Global.msg_TamsFile))
            {
                ShowBox = Visibility.Hidden;   
            }
            else if(ShowBox == Visibility.Visible && (RackMsg != Global.msg_TamsTestCompleted && RackMsg != Global.msg_TestCompleted && RackMsg != Global.msg_TamsFile))
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Fix" + RackViewIndex + "] Fix Error Msg Check. After RUN Click.");
                return;
            }
                

            if (!IsRackTCUnit)
            {
                showAlram(ALRAM_TYPE.WARN, Global.msg_Uncheck, isLogging:false);
                RackStat = RACK_STAT.ERROR;
                return;
            }

            if (Global.gMainPageVM.sc[RackIndex].SitePort == null || Global.gMainPageVM.sc[RackIndex].SitePort.IsOpen == false)
                return;
            */
			#endregion

			//제상 시간 초기화
			DefrostTime = string.Empty;

            //보류 0320
            //RemainDefosrtTime = string.Empty;

            if (IsRackTams)
            {
                if (!setRackTamsTemp())
                {
                    showAlram(ALRAM_TYPE.ERROR, Global.msg_TamsFile);
                    RackStat = RACK_STAT.ERROR;
                    return;
                }
                else
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : Tams SetTemp {RackSetSV}℃");

            }

            if (RackSetSV < Global.gConfigVM.ControlMinTemp || RackSetSV > Global.gConfigVM.ControlMaxTemp)
            {
                showAlram(ALRAM_TYPE.ERROR, $"SetTemp Range is " + Global.gConfigVM.ControlMinTemp + "~" + Global.gConfigVM.ControlMaxTemp + ". Check TargetSV");
                return;
            }

            foreach (CBoxModel bm in CBoxs)
                bm.PreStartTest();

            if (Global.gMainPageVM.sc[RackIndex].isMotorStatus) //모터 동작중이 아니고 Box Run 이 0 일때
            {
                //Test 하기위해 막아놈 모터 다운 SKIP
                MotorResult = await MotorAutoDown();


                if (MotorResult)//&& MotorStatText== "CLOSE") //Result 값 및 모터 다운 텍스트도 확인해서 스타트하게 변경
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] AutoDown OK");

                    //로그저장 및 그래프 초기화

                    startTempLog();
                    GraphIndex = 0;

                    Array.Clear(_prevDimmTemps, 0, _prevDimmTemps.Length);

                    foreach (CBoxModel bm in CBoxs)
                    {
                        bm.startTest();
                    }
                    //Console.WriteLine("AllRackRunCnt = {0}", Global.gMainPageVM.AllRackRunCnt.ToString());                    
                }
                else
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] AutoDown Fail");
                    await ReturnDownFail(true);
                }
            }
            else
            {
                foreach (CBoxModel bm in CBoxs)
                {
                    //로그저장 및 그래프 초기화

                    startTempLog();
                    GraphIndex = 0;

                    Array.Clear(_prevDimmTemps, 0, _prevDimmTemps.Length);

                    if (bm.CBoxStat == CBOX_STAT.PRERUN)
                        bm.startTest();
                }
            }
            /*
            else if (BoxRunCnt > 0) // Box Run 이 0 보다 클때
            {
                foreach (BoxModel bm in Boxes)
                {
                    if (bm.BoxStat == Box_STAT.PRERUN)
                        bm.startTest();
                }
            }

            else
            {
                foreach (BoxModel bm in Boxes)
                {
                    if (bm.BoxStat == Box_STAT.PRERUN)
                        bm.BoxStat = Box_STAT.STOP;
                }
                //Console.WriteLine("!!!!!!!!!!!!!!!!!! RackViewIndex [{0}] PRERUN FAIL RETURN STOP !!!!!!!!!!!!!!!", RackViewIndex);
                return;
            }
            */
        }
        public bool setRackTamsTemp()
        {
            try
            {
                string fn = $"{Global.gConfigVM.TamsSVPath}\\" +
                    $"{Global.gConfigVM.TamsSVFolder}{RackViewIndex}\\" +
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
                            RackSetSV = double.Parse(buf[1]);
                            return true;
                        }
                    }
                }
            }
            catch(Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Fix {RackViewIndex}] : Tams File Find Error Ocurred");
            }
            return false;
        }
        public bool findRackEndFile()
        {
            string fn = $"{Global.gConfigVM.TamsEndPath}\\" +
                $"{Global.gConfigVM.TamsEndFolder}{RackViewIndex}\\" +
                $"{Global.gConfigVM.TamsEndFile}";

            FileInfo finfo = new FileInfo(fn);

            if (finfo.Exists)
                return true;

            return false;
        }
        public void ErrCBoxTConStop()
        {
            foreach (CBoxModel bm in CBoxs)
            {
                bm.TCon_BoxStop();
                bm.CBoxStat = CBOX_STAT.ERROR;
                //if(bm.CBoxStat != CBOX_STAT.ERROR)
                //bm.CBoxStat = CBOX_STAT.STOP;
            }
            RackStat = RACK_STAT.ERROR;
        }
        public void ErrClearTConStat()
        {
            if (RackStat == RACK_STAT.ERROR)
            {
                foreach (CBoxModel bm in CBoxs)
                {
                    bm.CBoxStat = CBOX_STAT.STOP;
                }
                RackStat = RACK_STAT.STOP;
            }
        }

        private bool _isTimerRunning = false; // 타이머 동작 여부
        private CancellationTokenSource _timeUpdateCts;
        private DateTime _startTime; // 시작 시간

        /* 보류 0320
        private TimeSpan _DefrostremainingDefrostTime;
        private bool _isDefrostTimerRunning = false;
        private CancellationTokenSource _timeDefrostUpdateCts;
        */
        public async Task RackstopTestAsync()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] RackstopTest IN");

            if (IsRackAS || !IsRackTCUnit || RackStat != RACK_STAT.RUN)
                return;

            //제상 시간 얼마나 걸린지 여기서 처리
            /*// 보류 0320
            double[] RackDefrostedTime = { 0.0, 0.0, 0.0 };

            var taskCalcList = CBoxs.Select(bm => DeforstTimeCheckAsync(bm, RackDefrostedTime)).ToList();
            await Task.WhenAll(taskCalcList);

            double MaxDefrostTime = RackDefrostedTime.Max();
            _DefrostremainingDefrostTime = TimeSpan.FromSeconds(MaxDefrostTime);
            RemainDefosrtTime = "제상완료 예상시간 " + _DefrostremainingDefrostTime.ToString(@"hh\:mm\:ss");
            */

            StartUpdatingTime();

            DEFROST_TYPE[] StopFlag = { DEFROST_TYPE.NORMAL, DEFROST_TYPE.NORMAL, DEFROST_TYPE.NORMAL };

            // 각 박스에 대해 비동기 테스트 추가
            var taskStopList = CBoxs.Select(bm => StopBoxTestAsync(bm, StopFlag)).ToList();

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Task.WhenAll IN");
            await Task.WhenAll(taskStopList);
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Task.WhenAll OUT");

            Rack_Defrost = MConv.CheckArrayDefrost(StopFlag);

            Console.WriteLine($" {RackViewIndex}, Rack {Rack_Defrost}, {StopFlag[0]}, {StopFlag[1]}, {StopFlag[2]}");

            if (Rack_Defrost == DEFROST_TYPE.FAIL)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Box No Running or Error");
            }
            else
            {
                await RackCheckStop();
            }
            StopUpdatingTime();
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $" [Fix{RackViewIndex}] Rack Defrost Time : {DefrostTime}");
        }
        private async Task DeforstTimeCheckAsync(CBoxModel bm, double[] getTime)
        {
            try
            {
                getTime[bm.BoxIndex] = await bm.CalculateDefrostedTime();
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Fix{RackViewIndex}] Box {bm.BoxIndex} Defrost Time Check 실패: {ex.Message}");
                getTime[bm.BoxIndex] = 0;
            }
        }
        // ⏱️ 1초 간격으로 시간 표시하는 메서드
        private async void StartUpdatingTime()
        {
            if (_isTimerRunning)
                return;

            _isTimerRunning = true;
            _startTime = DateTime.Now; // 시작 시간 저장
            _timeUpdateCts = new CancellationTokenSource();
            CancellationToken token = _timeUpdateCts.Token;

            try
            {
                while (!token.IsCancellationRequested)
                {
                    TimeSpan elapsedTime = DateTime.Now - _startTime; // 흐른 시간 계산
                    DefrostTime = elapsedTime.ToString(@"hh\:mm\:ss"); // "hh:mm:ss" 형식으로 업데이트
                    await Task.Delay(1000, token); // 1초 대기
                }
            }
            catch (TaskCanceledException)
            {
                // 타이머가 중지되면 예외 발생할 수 있음 (무시)
            }
        }

        // ⏹️ 시간 업데이트 중지하는 메서드
        private void StopUpdatingTime()
        {
            if (!_isTimerRunning)
                return;

            _isTimerRunning = false;
            _timeUpdateCts?.Cancel();
        }

        private async Task StopBoxTestAsync(CBoxModel bm, DEFROST_TYPE[] stopFlag)
        {
            try
            {
                stopFlag[bm.BoxIndex] = await bm.stopTest();
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Fix{RackViewIndex}] Box {bm.BoxIndex} StopTest 실패: {ex.Message}");
                stopFlag[bm.BoxIndex] = DEFROST_TYPE.FAIL;
            }
        }
        public async Task RackCheckStop()
        {
            //PreRun , Run 중일때는 모터 업 금지
            /*
            for (int j = 0; j < Boxes.Count; j++)
            {
                if (Boxes[j].BoxStat == Box_STAT.RUN || Boxes[j].BoxStat == Box_STAT.PRERUN)
                    RunCheckFalg = true;
            }
            */
            //if (!RunCheckFalg && BoxRunCnt == 0 && Global.gMainPageVM.sc[RackIndex].isMotorStatus)
            if (Global.gMainPageVM.sc[RackIndex].isMotorStatus)
            {

                for (int i = 0; i < 6; i++)
                {
                    if (RackStat == RACK_STAT.STOP)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Fix {RackViewIndex}] Fix Status No Run.");
                        return;
                    }
                    await Task.Delay(10000);
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] MotorTimer " + ((i + 1) * 10).ToString());
                }
                await MotorAutoOpen(false); // 모터 업 LowTemp는 여기서 모터업 안함

                if (RackMotorStat == MOTOR_STAT.NORMAL)
                {
                    if (RackStat == RACK_STAT.DEFROST)
                    {
                        if (Rack_Defrost == DEFROST_TYPE.NORMAL)
                        {
                            foreach(CBoxModel bm in CBoxs)
                                bm.CBoxStat = CBOX_STAT.DONE;

                            showAlram(ALRAM_TYPE.DONE, Global.msg_TestCompleted, false);
                            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {RackViewIndex}] Normal Stop Finished");
                            RackStat = RACK_STAT.DONE;
                        }
                        else if (Rack_Defrost == DEFROST_TYPE.TAMS) //msg_TamsTestCompleted
                        {
                            foreach (CBoxModel bm in CBoxs)
                                bm.CBoxStat = CBOX_STAT.DONE;

                            showAlram(ALRAM_TYPE.DONE, Global.msg_TamsTestCompleted, false);
                            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ {RackViewIndex}] Tams Stop Finished");
                            RackStat = RACK_STAT.DONE;
                        }
                        else if (Rack_Defrost == DEFROST_TYPE.HUMIDITY)
                        {
                            showAlram(ALRAM_TYPE.ERROR, Global.msg_Humidity, false);
                            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[ {RackViewIndex}]] Humidity Stop Finished");
                            RackStat = RACK_STAT.ERROR;
                        }
                        else if (Rack_Defrost == DEFROST_TYPE.LOWTEMP)
                        {
                            showAlram(ALRAM_TYPE.ERROR, Global.msg_Cbox_25_temp, false);
                            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[ {RackViewIndex}] LowTemp Stop Finished");
                            RackStat = RACK_STAT.ERROR;
                        }
                        else if (Rack_Defrost == DEFROST_TYPE.ALARM)
                        {
                            showAlram(ALRAM_TYPE.ERROR, Global.msg_Alarm, false);
                            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[ {RackViewIndex}] Out of Temp Range Stop Finished");
                            RackStat = RACK_STAT.ERROR;
                        }
                        else if (Rack_Defrost == DEFROST_TYPE.TEMPERROR)
                        {
                            showAlram(ALRAM_TYPE.ERROR, Global.msg_Unbalance, false);
                            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[ {RackViewIndex}] Temp Operation Abnormal Stop Finished");
                            RackStat = RACK_STAT.ERROR;
                        }

                        foreach (CBoxModel bm in CBoxs)
                            bm.BoxDefrost = DEFROST_TYPE.NORMAL;
                    }
                }
                //Console.WriteLine("RackViewIndex [{0}] AllRackRunCnt = {1}", RackViewIndex, Global.gMainPageVM.AllRackRunCnt.ToString());
                if (Global.gMainPageVM.AllRackRunCnt == 0 && Global.gAlarmCom.AllRackStatus)
                {
                    //Console.WriteLine("경광등 STOP");
                    Global.gAlarmCom.AlarmWrite("S");
                    Global.gAlarmCom.AllRackStatus = false;
                    Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.STOP;
                    Global.gMainPageVM.endHumdityLog();
                    Global.gMainPageVM.endTempLog();
                }
            }
            else
            {
                //Console.WriteLine("!!!!!!!!!!!!!!!!!! RackViewIndex [{0}] Already Motor UP !!!!!!!!!!!!!!!", RackViewIndex);
                //showAlram(ALRAM_TYPE.ERROR, Global.msg_MotorUP, true);
            }
            //RunCheckFalg = false;
        }
        private bool Checkoperationmotor()
        {
            if (!Global.gMainPageVM.sc[RackIndex].isMotorStatus)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Already Motor Move");
                Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Already Move");
                return false;
            }
            return true;
        }
        public async void PreManualMotorDown(string getValue)
        {
            await ManualMotorDown(getValue);
        }
        public async Task<bool> ManualMotorDown(string v)
        {
            if (!Checkoperationmotor()) return false;

            try
            {
                Global.gMainPageVM.sc[RackIndex].isMotorStatus = false;

                string value = v;
                LogMotorManualDownStart();

                string str_cmd = "L" + value;
                LogMotorCommand(value);

                WriteMotorDown = true;
                await Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
                WriteMotorDown = false;
                await Task.Delay(5000);
                await Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
                await Task.Delay(300);

                Global.gMainPageVM.sc[RackIndex].Board_SensingOK = false;

                await Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("P");
                await Task.Delay(1000);

                if (Global.gMainPageVM.sc[RackIndex].Board_SensingOK)
                {
                    LogDownSenseOK();
                }

                Global.gEngineerVM.ManualMotor[RackIndex].TotalMotorVal += int.Parse(value);
                return true;
            }
            catch (Exception ex)
            {
                HandleError(ex);
                return false;
            }
            finally
            {
                Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
            }
        }
        public async void PreMotorAutoDown()
        {
            if (IsRackTCUnit == false) return;
            await MotorAutoDown();
        }
        public bool WriteMotorDown = false;
        public async Task<bool> MotorAutoDown()
        {
            if (!Checkoperationmotor()) return false;

            try
            {
                Global.gMainPageVM.sc[RackIndex].isMotorStatus = false;

                if (MotorStatText == "CLOSE")
                {
                    LogMotorAlreadyDown();
                    Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
                    return true;
                }

                int start = int.Parse(Global.gMainPageVM.sc[RackIndex].MotorValue);
                int step = 1000;
                int total = 0;
                int value_offset = 1000;
                int max_value = Math.Min(Global.gConfigVM.MotorMaxVal, 55000);

                LogMotorAutoDownStart();

                total = start;
                string str_cmd = "L" + start.ToString();
                LogMotorCommand(start.ToString());

                WriteMotorDown = true;
                await Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
                WriteMotorDown = false;
                await Task.Delay(5000);
                await Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
                await Task.Delay(300);

                Global.gMainPageVM.sc[RackIndex].Board_SensingOK = false;

                return await ProcessMotorDownAsync(total, step, max_value, value_offset);
            }
            catch (Exception ex)
            {
                HandleError(ex);
                return false;
            }
            finally
            {
                Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
            }
        }
        public async Task<bool> ReturnDownFail(bool isFail)
        {
            Global.gMainPageVM.sc[RackIndex].isMotorStatus = false;

            try
            {
                // 1. PowerOn 요청
                bool powerOnSuccess = await RequestPowerOnAsync();
                if (!powerOnSuccess)
                {
                    return false;
                }

                // 2. 모터 업 명령 실행
                bool motorUpSuccess = await ExecuteMotorUpCommand("H");
                if (!motorUpSuccess)
                {
                    return false;
                }

                // 3. 실패 상태 처리
                if (isFail)
                {
                    HandleFailure();
                }
                else
                {
                    LogCancel();
                    RackStat = RACK_STAT.STOP;
                }

                return true;
            }
            catch (Exception ex)
            {
                HandleError(ex);
                return false;
            }
            finally
            {
                Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
            }
        }
        // 헬퍼 메서드들
        private async Task<bool> RequestPowerOnAsync()
        {
            const int MAX_RETRIES = 5;
            const int RETRY_DELAY_MS = 1000;

            for (int i = 0; i < MAX_RETRIES; i++)
            {
                await Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("R");
                await Task.Delay(RETRY_DELAY_MS);

                if (Global.gMainPageVM.sc[RackIndex].Board_MotorPowerOn)
                {
                    LogPowerOnSuccess();
                    return true;
                }

                LogRetryCount(i + 1);
            }

            LogPowerOnFailure();
            return false;
        }

        private async Task<bool> ProcessMotorDownAsync(int total, int step, int max_value, int value_offset)
        {
            while (true)
            {
                await Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("P");
                await Task.Delay(1000);

                if (Global.gMainPageVM.sc[RackIndex].Board_SensingOK)
                {
                    await ProcessSensingOK(total, value_offset);
                    return true;
                }

                total = await ProcessSensingNotOK(total, step, max_value, value_offset);
                if (total > max_value)
                {
                    LogMaxSpinOver(total);
                    return false;
                }
            }
        }

        private async Task ProcessSensingOK(int total, int value_offset)
        {
            await Task.Delay(500);
            LogDownSenseOK();
            LogOffset(value_offset);

            string str_cmd = "L" + value_offset.ToString();
            WriteMotorDown = true;
            await Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
            WriteMotorDown = false;
            await Task.Delay(100);
            await Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
            await Task.Delay(100);

            int finalTotal = total + value_offset;
            LogFinalStep(finalTotal);
            await Task.Delay(500);
        }

        private async Task<int> ProcessSensingNotOK(int total, int step, int max_value, int value_offset)
        {
            total += step;
            LogMotorCommand(total.ToString());

            Global.gMainPageVM.sc[RackIndex].Board_SensingOK = false;

            string str_cmd = "L" + value_offset.ToString();
            WriteMotorDown = true;
            await Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
            WriteMotorDown = false;

            await Task.Delay(100);
            await Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
            await Task.Delay(300);

            return total;
        }
        private async Task<bool> WaitForPowerOnAsync()
        {
            const int MAX_RETRIES = 5;
            const int RETRY_DELAY_MS = 1000;
            const int TOTAL_TIMEOUT_MS = 30000; // 30초 전체 타임아웃

            var cts = new CancellationTokenSource(TOTAL_TIMEOUT_MS);
            int retryCount = 0;

            try
            {
                // 첫 번째 시도
                await Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("R");
                await Task.Delay(RETRY_DELAY_MS, cts.Token);

                while (!cts.Token.IsCancellationRequested)
                {
                    if (Global.gMainPageVM.sc[RackIndex].Board_MotorPowerOn)
                    {
                        LogPowerOnSuccess();
                        return true;
                    }

                    if (retryCount >= MAX_RETRIES)
                    {
                        LogPowerOnFailure();
                        return false;
                    }

                    await Task.Delay(RETRY_DELAY_MS, cts.Token);
                    await Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("R");

                    LogRetryCount(retryCount + 1);
                    retryCount++;
                }
                return false;
            }
            catch (OperationCanceledException)
            {
                LogTimeout();
                return false;
            }
            finally
            {
                cts.Dispose();
            }
        }

        private async Task<bool> ExecuteMotorUpCommand(string value)
        {
            try
            {
                string str_cmd = "H" + value;
                WriteMotorDown = true;

                // 모터 업 명령 시퀀스
                await Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
                await Task.Delay(5000);
                await Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
                await Task.Delay(100);
                await Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("I");
                await Task.Delay(100);

                WriteMotorDown = false;
                LogMotorUpSuccess();
                return true;
            }
            catch (Exception ex)
            {
                LogMotorUpError(ex);
                return false;
            }
        }

        private void StopTemperatureControllers()
        {
            foreach (CBoxModel bm in CBoxs)
            {
                if (bm == null) continue;
                if (bm.TconIsRun)
                    bm.TCon_BoxStop();
            }
            LogTemperatureControllersStopped();
        }

        // 로깅 메서드들
        private void LogMotorAlreadyUp()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] Motor Already Up");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Motor Already Up");
        }

        private void LogRequestPowerOn()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] Request to PowerOn");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Request to PowerOn");
        }

        private void LogPowerOnSuccess()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] PowerOn is Success");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] PowerOn is Success");
        }

        private void LogPowerOnFailure()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] PowerOn is Fail");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] PowerOn is Fail");
            RackMotorStat = MOTOR_STAT.ERROR;
            showAlram(ALRAM_TYPE.ERROR, Global.msg_MotorError, true);
            RackStat = RACK_STAT.ERROR;
        }

        private void LogRetryCount(int count)
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] Try Count = {count}");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Try Count = {count}");
        }

        private void LogTimeout()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Fix{RackViewIndex}] Operation timeout");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Operation timeout");
            RackMotorStat = MOTOR_STAT.ERROR;
            showAlram(ALRAM_TYPE.ERROR, Global.msg_MotorError, true);
            RackStat = RACK_STAT.ERROR;
        }

        private void LogMotorUpSuccess()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] Motor UP");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Motor UP");
        }

        private void LogMotorUpError(Exception ex)
        {
            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Fix{RackViewIndex}] Motor UP error: {ex.Message}");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Motor UP error: {ex.Message}");
            RackMotorStat = MOTOR_STAT.ERROR;
            showAlram(ALRAM_TYPE.ERROR, Global.msg_MotorError, true);
            RackStat = RACK_STAT.ERROR;
        }

        private void LogTemperatureControllersStopped()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] Temperature controllers stopped");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Temperature controllers stopped");
        }

        private void HandleError(Exception ex)
        {
            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Fix{RackViewIndex}] Error: {ex.Message}");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Error: {ex.Message}");
            RackMotorStat = MOTOR_STAT.ERROR;
            showAlram(ALRAM_TYPE.ERROR, Global.msg_MotorError, true);
            RackStat = RACK_STAT.ERROR;
        }
        private void LogMotorAlreadyDown()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] Motor Already Down");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Motor Already Down");
        }

        private void LogMotorAutoDownStart()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] Motor Auto Down Start");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Motor Auto Down Start");
        }

        private void LogMotorManualDownStart()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] Motor Manual Down Start");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Motor Manual Down Start");
        }

        private void LogMotorCommand(string value)
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] {value}");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] {value}");
        }

        private void LogDownSenseOK()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] Down Sense is OK");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Down Sense is OK");
        }

        private void LogOffset(int offset)
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] Offset is {offset}");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Offset is {offset}");
        }

        private void LogFinalStep(int total)
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] Final Step is {total}");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Final Step is {total}");
        }

        private void LogMaxSpinOver(int total)
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix{RackViewIndex}] Max Spin Over {total}");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] Max Spin Over {total}");
        }

        private void LogTemperatureNotOK()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Fix{RackViewIndex}] CurTemp is not ok. Can't motor up");
            Global.gEngineerVM.addMotorLog($"[Fix{RackViewIndex}] CurTemp is not ok. Can't motor up");
        }

        private void LogCancel()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[All] Motor {RackViewIndex} Down is Cancel");
        }

        private void HandleFailure()
        {
			foreach (CBoxModel bm in CBoxs)
				bm.CBoxStat = CBOX_STAT.ERROR;
			showAlram(ALRAM_TYPE.ERROR, Global.msg_SenseError, false);
            
            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[All] Motor {RackViewIndex} Down is Fail");
            RackStat = RACK_STAT.ERROR;
        }

        public async void PreManualMotorOpen(string getVal)
        {
            await ManualMotorOpen(getVal);
        }
        public async Task<bool> ManualMotorOpen(string v)
        {
            if (!Checkoperationmotor()) return false;

            try
            {
                Global.gMainPageVM.sc[RackIndex].isMotorStatus = false;

                // 1. 이미 열려있는지 확인
                if (MotorStatText == "OPEN")
                {
                    LogMotorAlreadyUp();
                    Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
                    return false;
                }

                // 2. PowerOn 요청 및 대기
                LogRequestPowerOn();
                Global.gMainPageVM.sc[RackIndex].Board_MotorPowerOn = false;

                bool powerOnSucess = await WaitForPowerOnAsync();
                if (!powerOnSucess)
                {
                    Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
                    return false;
                }

                // 3. 온도 조절기 정지
                StopTemperatureControllers();

                // 4. 모터 업 명령 실행
                return await ExecuteMotorUpCommand(v);
            }
            catch (Exception ex)
            {
                HandleError(ex);
                return false;
            }
            finally
            {
                Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
            }
        }
        public async void PreMotorAutoOpen(bool manual)
        {
            if (IsRackTCUnit == false) return;
            await MotorAutoOpen(manual);
        }
        public async Task<bool> MotorAutoOpen(bool manual)
        {
            if (!Checkoperationmotor()) return false;

            try
            {
                Global.gMainPageVM.sc[RackIndex].isMotorStatus = false;

                if (MotorStatText == "OPEN")
                {
                    LogMotorAlreadyUp();
                    Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
                    return false;
                }

                if (Rack_Defrost != DEFROST_TYPE.LOWTEMP)
                {
                    LogRequestPowerOn();
                    Global.gMainPageVM.sc[RackIndex].Board_MotorPowerOn = false;

                    bool powerOnSuccess = await WaitForPowerOnAsync();
                    if (!powerOnSuccess)
                    {
                        return false;
                    }

                    StopTemperatureControllers();
                    return await ExecuteMotorUpCommand("");
                }
                else
                {
                    LogTemperatureNotOK();
                    return false;
                }
            }
            catch (Exception ex)
            {
                HandleError(ex);
                return false;
            }
            finally
            {
                Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
            }
        }
        public bool TconRunCheck()
        {
            int Isruncnt = 0;
            foreach(CBoxModel bm in CBoxs)
            {
                if(bm.TconIsRun)
                    Isruncnt++;
            }

            if(Isruncnt >= 3)
                return true;
            else
                return false;
        }
        public void PreprocessTest()
        {
            //Prerun && CBOX 3개 모두 Tcon Run 일때 제어 시작 
            if((RackStat == RACK_STAT.PRERUN && TconRunCheck()) || RackStat == RACK_STAT.RUN || RackStat == RACK_STAT.DEFROST)
            {
                processTest();

                int gindex = GraphIndex;
                if (Global.gMainVM.ShowGraph == Visibility.Visible && Global.gDimmVM.CurRack == RackIndex)
                    System.Windows.Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => Global.gDimmVM.processRun(gindex)));
            }
            else if( RackStat != RACK_STAT.ERROR )
            {
                    foreach (CBoxModel box in CBoxs)
                    {
                        box.processBoxDimm();
                    }
                //MaxInRack.Clear();

                if (Global.gConfigVM.GraphMode)
                {
                    double pv = 0;
                    foreach (DimDataModel dim in DimDatas)
                    {
                        if (pv < dim.Temp)
                            pv = dim.Temp;

                        dim.GraphData[GraphIndex] = dim.Temp;
                    }

                    GraphIndex++;
                    addTempLog();

                    int gindex = GraphIndex;
                    if (Global.gMainVM.ShowGraph == Visibility.Visible && Global.gDimmVM.CurRack == RackIndex)
                        System.Windows.Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => Global.gDimmVM.processRun(gindex)));
                }
            }
        }
        //public string aaaa = string.Empty;
        public void processTest()
        {

            foreach (CBoxModel box in CBoxs)
            {
                /*
                IList<double> dims = (IList<double>)Global.gMainPageVM.cc[RackIndex].RackDimmTemp;

                aaaa = string.Empty;
                foreach (double dim in dims)
                {
                    aaaa += dim;
                    aaaa += ",";
                }
                Console.WriteLine($"brfore  = {aaaa}");
                */
                box.processBoxDimm();

                /*
                dims = (IList<double>)Global.gMainPageVM.cc[RackIndex].RackDimmTemp;

                aaaa = string.Empty;
                foreach (double dim in dims)
                {
                    aaaa += dim;
                    aaaa += ",";
                }
                Console.WriteLine($"after  = {aaaa}");
                */
                
                if (RackStat == RACK_STAT.DEFROST) continue;
                if (Global.gConfigVM.ControlMode != (int)CONTROL_MODE.NOSV)
                {
                    if (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.NORMAL)
                        box.processBoxSV();
                    else if (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.SUB)
                        box.processBoxSVSub();
                    else if (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.THRID)
                        box.processBoxSVThrid();
                    else if (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.FOURTH)
                        box.processBoxSVFourth();
                    else if (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.FIFTH)
                        box.processBoxSVFifth();
                }
            }

            double max = 0;
            foreach (DimDataModel dim in DimDatas)
            {
                if (max < dim.Temp)
                    max = dim.Temp;

                dim.GraphData[GraphIndex] = dim.Temp;
            }

            GraphIndex++;
            addTempLog();
            MaxPV = max.ToString("F1");

            if (RackStat == RACK_STAT.RUN && IsRackTams == true) //Run & Tams Check 일때 탐스 파일 확인
            {
                if (findEndFile())
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix {RackViewIndex}] : Find EndFlag {Global.gConfigVM.TamsEndFile}");

                    foreach (CBoxModel cm in CBoxs)
                        cm.BoxDefrost = DEFROST_TYPE.TAMS;

                    Task.Run(() => RackstopTestAsync());
                }
            }
            /*
            if (max == 0)
            {
                _pcCheckCount++;
                if (_pcCheckCount > Global.gConfigVM.NoDimmCheckCount)
                {
                    // dimm 온도 안올라옴. 
                    // 실장기 Test 종료 이거나 꺼짐으로 간주.
                    stopTest();
                }
            }
            else
                _pcCheckCount = 0;

            // Dimm 중 Max BaseLine을 넘어가면 SV Offset으로 조절
            checkBaseLine(MaxInRack.Max());
            MaxInRack.Clear();

            if (findEndFile())
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : Find EndFlag {Global.gConfigVM.TamsEndFile}");
                stopTest();
                RackStat = RACK_STAT.DONE;
            }
            */
        }
        public void startTempLog()
        {
            if (TempSerilog != null)
                TempSerilog.Dispose();
            string file = $"{Global.gConfigVM.TempLogPath}\\Fix{RackViewIndex}@{DateTime.Now.ToString("yyyy_MM_dd_HHmm")}" +
                $"{(Global.gConfigVM.GraphMode ? "_NoRun" : "")}.csv";
            TempSerilog = new LoggerConfiguration()
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite, outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();

            //TS0 TS1 구별해서 추가 저장할지 미정 하면 여기서
        }
        public void addTempLog()
        {
            if (!Global.gConfigVM.GraphMode)
                if (RackStat != RACK_STAT.RUN) return;
            if (TempSerilog == null) startTempLog();
            if (TempSerilog == null) return;

            StringBuilder strBuilder = new StringBuilder();
            strBuilder.Append(DateTime.Now.ToString("MM-dd HH:mm:ss"));
            for (int i = 0; i < DimDatas.Count; i++)
            {
                strBuilder.Append("," + DimDatas[i].Temp.ToString());
            }

            foreach (CBoxModel box in CBoxs)
                strBuilder.Append(',' + Math.Round(box.TargetSV, 1).ToString());


            foreach (CBoxModel box in CBoxs)
                strBuilder.Append(',' + Math.Round(box.InsideBoxTemp, 1).ToString());

            TempSerilog.Information(strBuilder.ToString());
        }
        public bool findEndFile()
        {
            try
            {
                string fn = $"{Global.gConfigVM.TamsEndPath}\\" +
                    $"{Global.gConfigVM.TamsEndFolder}{RackViewIndex}\\" +
                    $"{Global.gConfigVM.TamsEndFile}";

                FileInfo finfo = new FileInfo(fn);

                if (finfo.Exists)
                    return true;
            }
            catch(Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Fix {RackViewIndex}] : End File Find Error Ocurred");
            }
            return false;
        }
    }
}
