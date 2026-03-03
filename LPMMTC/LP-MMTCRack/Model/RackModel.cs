using HandyControl.Expression.Shapes;
using HandyControl.Tools.Extension;
using LP_MMTCRack.Base;
using LP_MMTCRack.Net;
using LP_MMTCRack.View.Component;
using LP_MMTCRack.ViewModel;
using PropertyChanged;
using ScottPlot.Drawing.Colormaps;
using ScottPlot.MarkerShapes;
using Serilog;
using Serilog.Core;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls.Primitives;
using System.Windows.Forms;
using System.Windows.Media;
using System.Xml;
using System.Xml.Linq;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TrackBar;

namespace LP_MMTCRack.Model
{
    public enum RACK_STAT
    {
        RUN = 0,
        STOP,
        ERROR,
        IDLE,
        DONE,
    }
    public enum MOTOR_STAT
    {
        IDLE = 0,
        NORMAL,
        WORKING,
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
    public enum MOTOR_TYPE
    {
        NOMAL = 0,
        REMOVE,
        FAIL
    }
    public class RackModel : BaseVM
    {
        [DoNotNotify] public int RackIndex { get; set; } = 0;
        [DoNotNotify] public int RackViewIndex { get; set; } = 0;
        [DoNotNotify] public int RackBox1ViewIndex { get; set; } = 0;
        [DoNotNotify] public int RackBox2ViewIndex { get; set; } = 0;
        public SolidColorBrush BoarderColor { get; set; }
        public SolidColorBrush RackColor { get; set; } = Global.gRackColors[0];
        public SolidColorBrush MotorStatColor { get; set; } = Global.gMotorStatColors[1];
        public SolidColorBrush FlowStatColor { get; set; } = Global.gWarningColors[0];
        public SolidColorBrush FlowRateColor { get; set; } = Global.gWarningColors[0];
        public bool IsRackTamsEnable { get; set; } = false;
        public Visibility RackEngrmodeVisibility { get; set; } = Visibility.Hidden;
        /*
        bool _isRacktcunit = true;
        public bool IsRackTCUnit
        {
            get => _isRacktcunit;
            set
            {
                _isRacktcunit = value;
            }
        }
        bool _isRackTams = true;
        public bool IsRackTams
        {
            get => _isRackTams;
            set
            {
                _isRackTams = value;
                if(_isRackTams)
                {
                    for (int i = 0; i < Boxes.Count; i++)
                    {
                        Boxes[i].IsBoxTams = true;
                    }
                }
                else
                {
                    for (int i = 0; i < Boxes.Count; i++)
                    {
                        Boxes[i].IsBoxTams = false;
                    }
                }
            }
        }
        bool _isRackAS = false;
        public bool IsRackAS
        {
            get => _isRackAS;
            set
            {
                _isRackAS = value;
                if(_isRackAS)
                {
                    for (int i = 0; i < Boxes.Count; i++)
                    {
                        Boxes[i].IsBoxAS = true;
                    }
                    RackColor = Global.gRackColors[1];
                }
                else
                {
                    for (int i = 0; i < Boxes.Count; i++)
                    {
                        Boxes[i].IsBoxAS = false;
                    }
                    RackColor = Global.gRackColors[0];
                }
            }
        }
        */
        public double RackSetSV = 0;
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
        public MOTOR_STAT RackMotorAlarm { get; set; } = MOTOR_STAT.NORMAL; 
        string _motorstattext = string.Empty;
        public string MotorStatText 
        { 
            get => _motorstattext; 
            set
            {
                _motorstattext = value;
                Global.gEngineerVM.ManualMotor[RackIndex].eMotorStatText = _motorstattext;
                if(_motorstattext == "OPEN")
                {
                    MotorStatColor = Global.gMotorStatColors[1];
                    Global.gEngineerVM.ManualMotor[RackIndex].eMotorStatColor = Global.gMotorStatColors[1];
                }
                else if( _motorstattext == "CLOSE")
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

        bool _islideopen = false;
        public bool IsSlideOpen 
        { 
            get => _islideopen; 
            set
            {
                _islideopen = value;
            }
        }
        public void GetSlideOpen(bool getStat) // true Slide Open , false Slide Close
        {
            if (getStat)
            {
				bool skipAlarm = Boxes.Any(bm => bm.BoxStat == Box_STAT.PRERUN || bm.BoxStat == Box_STAT.RUN || bm.BoxStat == Box_STAT.DEFROST);

                if(skipAlarm)
                {
                    HideSlideAlarm();
                }
                else
                {
					ShowSlideAlarm(ALRAM_TYPE.ERROR, Global.msg_SlideOpen);

					foreach (BoxModel bm in Boxes)
					{
						if (bm.BoxStat == Box_STAT.DONE)
						{
							bm.BoxShowBox = Visibility.Hidden;
							bm.BoxStat = Box_STAT.STOP;
							bm.BoxDefrost = DEFROST_TYPE.NORMAL;
							bm.DefostTime = string.Empty;
							bm.Sopen_Stat = true;
							bm.Bimetal_Stat = true;
						}
					}
				}
            }
            else
            {

				HideSlideAlarm();
			}
            IsSlideOpen = getStat;
        }

        // 박스 상태가 변경될 때 호출하여 팝업을 숨기는 함수
        public void HidePopupIfNeeded()
        {
            foreach (BoxModel bm in Boxes)
            {
                if (bm.BoxStat == Box_STAT.PRERUN || bm.BoxStat == Box_STAT.RUN || bm.BoxStat == Box_STAT.DEFROST)
                {
                    if (RackMsg != Global.msg_InitRack)
                    {
                        ShowBox = Visibility.Hidden;  // 팝업 숨김
                    }
                    return; // 하나라도 있으면 바로 리턴
                }
            }
        }

        RACK_STAT _rackStat;
        public RACK_STAT RackStat
        {
            get => _rackStat;
            set
            {
                _rackStat = value;
                BoarderColor = Global.gBoarderColors[(int)_rackStat];
            }
        }
        public MOTOR_STAT RackMotorStat = MOTOR_STAT.IDLE; 
        public Visibility ShowBox { get; set; } = Visibility.Hidden;
        public int BoxesCnt { get; set; } = 0;
        public string RackMsg { get; set; } = "";
        public SolidColorBrush RackMsgColor { get; set; }
        public void showAlram(ALRAM_TYPE type, string msg, bool isLogging)
        {
            RackMsgColor = Global.gAlramColors[(int)type];
            RackMsg = msg;
            ShowBox = Visibility.Visible;

            if (isLogging)
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, msg);
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
			SlideMsg = string.Empty;
		}


		ObservableCollection<BoxModel> _boxes = new ObservableCollection<BoxModel>();
        public ObservableCollection<BoxModel> Boxes { get { return _boxes; } set { _boxes = value; PC(nameof(Boxes)); } }

        public DelegateCommand RackPopUpClear { get; set; }
        public DelegateCommand RackBox1PopUpClear { get; set; }
        public DelegateCommand RackBox2PopUpClear { get; set; }
        public DelegateCommand RackCmdRun { get; set; }
        public DelegateCommand RackCmdStop { get; set; }
        public DelegateCommand RackManualUp {  get; set; }
        public DelegateCommand RackManualDown { get; set; }
        public DelegateCommand RackHumOn { get; set; }
        public DelegateCommand RackHumOff { get; set; }
        #region Log 
        [DoNotNotify]
        public Logger TempSerilog { get; set; } = null;
        #endregion
        //public SiteCom sc;
        //public CaptureCom cc;
        public RackModel(int ch, int cnt)
        {
            RackViewIndex = (ch + 1);
            RackIndex = ch;
            RackBox1ViewIndex = (RackIndex * 2) + 1;
            RackBox2ViewIndex = (RackIndex * 2) + 2;
            BoxesCnt = cnt;

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
                int id = Convert.ToInt32(boxNode.Attributes["id"].Value);
                if(rackNode.ChildNodes.Count == 3)
                    Boxes.Add(new BoxModel((RackIndex * 3) + id, id, this));
                else if(rackNode.ChildNodes.Count == 2)
                    Boxes.Add(new BoxModel((RackIndex * 2) + id, id, this));
                Boxes[i].HumidityOffset = Convert.ToInt32(boxNode.Attributes["humidityoffset"].Value);
                Boxes[i].IsBoxTCUnit = Convert.ToInt32(boxNode.Attributes["tc-unit"].Value) == 1 ? true : false;
                Boxes[i].IsBoxAS = Convert.ToInt32(boxNode.Attributes["as"].Value) == 1 ? true : false;
                //Boxes[i].ISBoxTams = Convert.ToInt32(boxNode.Attributes["tams"].Value) == 1 ? true : false;
               
                i++;
            }
            RackStat = RACK_STAT.STOP;
            RackMotorStat = MOTOR_STAT.NORMAL;

            RackCmdRun = new DelegateCommand(p => { Task.Run(() => RackstartTest()); });
            RackCmdStop = new DelegateCommand(p => { Task.Run(() => RackstopTestAsync()); });
            RackManualUp = new DelegateCommand(p => { Task.Run(() => PreMotorAutoOpen(MOTOR_TYPE.NOMAL)); });
            RackManualDown = new DelegateCommand(p => { Task.Run(() => PreMotorAutoDown()); });
            RackHumOn = new DelegateCommand(p => { Task.Run(() => RackHumTestOn()); });
            RackHumOff = new DelegateCommand(p => { Task.Run(() => RackHumTestOff()); });

            RackPopUpClear = new DelegateCommand(p => { RackPopUpClearExecute(); });
            RackBox1PopUpClear = new DelegateCommand(p => { RackBox1PopUpClearExecute(); });
            RackBox2PopUpClear = new DelegateCommand(p => { RackBox2PopUpClearExecute(); });
        }

        private void RackHumTestOn()
        {
            foreach(var bm in Boxes)
                bm.isLoggingHumidity = true;
        }
        private void RackHumTestOff()
        {
            foreach (var bm in Boxes)
            {
                bm.isLoggingHumidity = false;
                bm.HumidityValue = 0;
                bm.HumRateColor = Global.gWarningColors[0];
            }
        }
        private void RackPopUpClearExecute()
        {
            ShowBox = Visibility.Hidden;
            RackMsg = string.Empty;
            if (RackMotorStat == MOTOR_STAT.ERROR)
                RackMotorStat = MOTOR_STAT.NORMAL;
            if (RackStat == RACK_STAT.ERROR || RackStat == RACK_STAT.DONE)
                RackStat = RACK_STAT.STOP;
        }
        private void RackBox1PopUpClearExecute()
        {
            ClearBoxPopup(0);
        }
        private void RackBox2PopUpClearExecute()
        {
            ClearBoxPopup(1);
        }
        private void ClearBoxPopup(int boxIndex)
        {
            if (boxIndex < 0 || boxIndex >= Boxes.Count)
                return;

            var box = Boxes[boxIndex];
            box.BoxShowBox = Visibility.Hidden;
            box.BoxMsg = string.Empty;

            if (box.BoxStat == Box_STAT.ERROR)
            {
                Task.Run(() => box.Read_BoxTConStat());
            }

            if (box.BoxStat == Box_STAT.ERROR || box.BoxStat == Box_STAT.DONE)
            {
                box.BoxStat = Box_STAT.STOP;
                box.BoxDefrost = DEFROST_TYPE.NORMAL;
                box.DefostTime = string.Empty;
                box.Sopen_Stat = true;
                box.Bimetal_Stat = true;
            }
        }
        public void RackStatusClear()
        {
            ShowBox = Visibility.Hidden;
            if (RackMotorStat == MOTOR_STAT.ERROR)
                RackMotorStat = MOTOR_STAT.NORMAL;

            Overlap_PreRun = false;
            foreach (BoxModel bm in Boxes)
            {
                bm.BoxStatusClear();
            }
        }
        public void SiteBDVersionCheck()
        {
            Global.gEngineerVM.ManualMotor[RackIndex].GetSiteVer = string.Empty;
            Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("F");

            /*//Test Code
            string getPortname = Global.gMainPageVM.sc[RackIndex].SitePortName;
            string getDeviceId = string.Empty;
            Global.gMainPageVM.sc[RackIndex].SiteDisConnect();

            Thread.Sleep(1000);

            getDeviceId = MConv.GetDeviceInstanceId(getPortname);

            MConv.SetComPortState(getDeviceId, false);

            Thread.Sleep(10000);

            MConv.SetComPortState(getDeviceId, true);
            */
        }
        public void RackTconRun()
        {
            foreach (BoxModel bm in Boxes)
                bm.TCon_BoxRun();
        }
        public void RackTconStop()
        {
            foreach (BoxModel bm in Boxes)
                bm.TCon_BoxStop();
        }
        public void RacksetTemp()
        {
            for (int i = 0; i < Boxes.Count; i++)
            {
                if (!Boxes[i].IsBoxTCUnit || Boxes[i].IsBoxAS)
                    continue;
                Boxes[i].BoxSetSV = RackSetSV;
                Boxes[i].setTemp();
			}
        }
        public bool Overlap_PreRun = false;
        public async void RackstartTest()
        {
            //Test용
            /*
            foreach (BoxModel bm in Boxes)
            {
                //if (bm.BoxStat == Box_STAT.PRERUN)
                    bm.BoxStat = Box_STAT.RUN;
                if (bm.GraphIndex != 0)
                {
                    bm.GraphIndex = 0;
                    Global.gGraphVM.TempSigs.OffsetX = bm.TempDatas[0].TimeData[0].ToOADate();
                }
            }
            */
            
            if(Overlap_PreRun)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Fix" + RackViewIndex + "] Already Click Run Button. Do it again later");
                return;
            }
            Overlap_PreRun = true;
            var taskRackList = new List<Task>();
            bool[] PreStartFlag = { false, false, false };
            bool MotorResult = false;
            
            for (int i = 0; i < Boxes.Count; i++)
            {
                int index = i;
                taskRackList.Add(Task.Run(() => { PreStartFlag[index] = Boxes[index].PreStartTest(); }));
            }

            //Task.WaitAll(taskRackList.ToArray());
            //await Task.WhenAll(taskRackList);

            try
            {
                await Task.WhenAll(taskRackList);
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"Error during PreStartTest: {ex.Message}");
                Overlap_PreRun = false;
                return;
            }

            if (PreStartFlag.All(flag => !flag))
            {
                Overlap_PreRun = false;
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Fix{RackViewIndex}] PreStart Test Failed.");
                return;
            }

            if (Global.gMainPageVM.sc[RackIndex].isMotorStatus && BoxRunCnt == 0) //모터 동작중이 아니고 Box Run 이 0 일때
            {
                RackMotorStat = MOTOR_STAT.WORKING;
                Overlap_PreRun = false;

                MotorResult = await MotorAutoDown();
                RackMotorStat = MOTOR_STAT.NORMAL;

                if (MotorResult)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] AutoDown OK");

                    var tasks = new List<Task>();

                    for (int i = 0; i < Boxes.Count; i++)
                    {
                        int index = i;
                        if (PreStartFlag[i])
                            // 비동기 호출로 startTest() 실행
                            tasks.Add(Task.Run(() => {
                                if (index >= 0 && index < Boxes.Count)
                                {
                                    Boxes[index].startTest();
                                }
                            }));
                        else
                            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[B" + RackViewIndex + "-" + Boxes[i].BoxViewIndex + " ] No Start Test");
                    }
                    //Console.WriteLine("AllRackRunCnt = {0}", Global.gMainPageVM.AllRackRunCnt.ToString());

                    await Task.WhenAll(tasks);

                }
                else
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] AutoDown Fail");
                    await ReturnDownFail(true);
                }
            }
            else if (BoxRunCnt > 0) // Box Run 이 0 보다 클때
            {
                Overlap_PreRun = false;
                foreach (BoxModel bm in Boxes)
                {
                    if (bm.BoxStat == Box_STAT.PRERUN)
                        bm.startTest();
                }
            }
            else
            {
                Overlap_PreRun = false;
                foreach (BoxModel bm in Boxes)
                {
                    if (bm.BoxStat == Box_STAT.PRERUN)
                        bm.BoxStat = Box_STAT.STOP;
                }
                //Console.WriteLine("!!!!!!!!!!!!!!!!!! RackViewIndex [{0}] PRERUN FAIL RETURN STOP !!!!!!!!!!!!!!!", RackViewIndex);
                return;
            }
        }
        bool RunCheckFalg = false;
        public async Task RackstopTestAsync()
        {
            //var taskStopList = new List<Task>();
            bool[] StopFlag = {false, false, false};

            /*
            foreach(BoxModel bm in Boxes)
            {
                int index = bm.BoxIndex % 3;
                taskStopList.Add(Task.Run(async () => { StopFlag[index] = await bm.stopTest(); }));
            }
            */
            // 각 박스에 대해 비동기 테스트 추가
            var taskStopList = Boxes.Select(bm => StopBoxTestAsync(bm, StopFlag)).ToList();

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Rack" + RackViewIndex + "] Task.WaitAll IN");
            //Task.WaitAll(taskStopList.ToArray());
            await Task.WhenAll(taskStopList);
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Rack" + RackViewIndex + "] Task.WaitAll OUT");
            if (Array.IndexOf(StopFlag, true) == -1)
            {
                Console.WriteLine("RackViewIndex [{0}] !! StopPool flase !! {1}, {2}, {3} ", RackViewIndex, StopFlag[0], StopFlag[1], StopFlag[2]);
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Rack" + RackViewIndex + "] Box No Runing");
            }
            else
            {
                RackCheckStop();
            } 
            
            //RackCheckStop();
        }
        private async Task StopBoxTestAsync(BoxModel bm, bool[] stopFlag)
        {
            try
            {
                stopFlag[bm.EachBoxIndex] = await bm.stopTest();
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Fix{RackViewIndex}] Box {bm.BoxViewIndex} StopTest 실패: {ex.Message}");
                stopFlag[bm.EachBoxIndex] = false;
            }
        }
        public async void RackCheckStop()
        {
            //PreRun , Run 중일때는 모터 업 금지
            for (int j = 0; j < Boxes.Count; j++)
            {
                if (Boxes[j].BoxStat == Box_STAT.RUN || Boxes[j].BoxStat == Box_STAT.PRERUN)
                    RunCheckFalg = true;
            }

            if (!RunCheckFalg && BoxRunCnt == 0 && Global.gMainPageVM.sc[RackIndex].isMotorStatus)
            {
                
                for (int i = 0; i < 6; i++)
                {
                    await Task.Delay(10000);
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] MotorTimer " + ((i+1)*10) .ToString());
                }
                await MotorAutoOpen(MOTOR_TYPE.FAIL); // 자동 모터 업 false

                if (RackMotorStat == MOTOR_STAT.NORMAL)
                {
                    foreach (BoxModel bm in Boxes)
                    {
                        if (bm.BoxStat == Box_STAT.DEFROST)
                        {
                            if (bm.BoxDefrost == DEFROST_TYPE.NORMAL)
                            {
                                bm.BoxStat = Box_STAT.DONE;
                                bm.showBoxAlarm(ALRAM_TYPE.DONE, Global.msg_TestCompleted, false, bm.BoxViewIndex);
                                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + RackViewIndex + "-" + bm.BoxViewIndex + "] Normal Stop Finished");
                            }
                            else if (bm.BoxDefrost == DEFROST_TYPE.TAMS) //msg_TamsTestCompleted
                            {
                                bm.BoxStat = Box_STAT.DONE;
                                bm.showBoxAlarm(ALRAM_TYPE.DONE, Global.msg_TamsTestCompleted, false, bm.BoxViewIndex);
                                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + RackViewIndex + "-" + bm.BoxViewIndex + "] Normal Tams Stop Finished");
                            }
                            else if (bm.BoxDefrost == DEFROST_TYPE.HUMIDITY)
                            {
                                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + RackViewIndex + "-" + bm.BoxViewIndex + "] Humidity Stop Finished");
                            }
                            else if (bm.BoxDefrost == DEFROST_TYPE.LOWTEMP)
                            {
                                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + RackViewIndex + "-" + bm.BoxViewIndex + "] box LowTemp Stop Finished");
                            }
                            bm.BoxDefrost = DEFROST_TYPE.NORMAL;
                            bm.TimeEnd = DateTime.Now;
                        }
                    }
                }
                //Add RackStat Error로
                else if(RackMotorStat == MOTOR_STAT.ERROR)
                {
                    RackStat = RACK_STAT.ERROR;
                }

                //Console.WriteLine("RackViewIndex [{0}] AllRackRunCnt = {1}", RackViewIndex, Global.gMainPageVM.AllRackRunCnt.ToString());
                if (Global.gMainPageVM.AllRackRunCnt == 0 && Global.gAlarmCom.AllRackStatus)
                {
                    //Console.WriteLine("경광등 STOP");
                    Global.gAlarmCom.AlarmWrite("S");
                    Global.gAlarmCom.AllRackStatus = false;
                    Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.STOP;
                    Global.gMainPageVM.endHumdityLog();
                    //Global.gMainPageVM.endTempLog();
                }
            }
            else
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Motor Up Fail.  BoxRunCnt = " + BoxRunCnt);
                //Console.WriteLine("!!!!!!!!!!!!!!!!!! RackViewIndex [{0}] Already Motor UP !!!!!!!!!!!!!!!", RackViewIndex);
                //showAlram(ALRAM_TYPE.ERROR, Global.msg_MotorUP, true);
            }
            RunCheckFalg = false;
        }

        public bool SiteFWReadWrite()
        {
            bool rtnVal = false;
            uint selStienum = 0;

            uint numDevices;
            int result = MConv.CP210x_GetNumDevices(out numDevices);

            if (result != MConv.CP210x_SUCCESS || numDevices == 0)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드를 찾을수 없음.");
                return false;
            }

            // Read Reg Info
            for (uint i = 0; i < numDevices; i++)
            {
                IntPtr deviceHandle;
                result = MConv.CP210x_Open(i, out deviceHandle); // 첫 번째 장치 열기

                if (result != MConv.CP210x_SUCCESS)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드를 연결 할 수 없음. {i}");
                    return false;
                }
                byte length = 0;

                // 장치 일련번호 가져오기
                StringBuilder serialNumber = new StringBuilder(64);
                result = MConv.CP210x_GetDeviceSerialNumber(deviceHandle, serialNumber, ref length, true);

                if (result == MConv.CP210x_SUCCESS)
                {
                    // 일련번호가 정상적으로 반환되었을 경우
                    //if (serialNumber.ToString() == Global.gMainPageVM.sc[RackIndex].UsbHwSitePort)
                    //연결 안된 Port Reset 하는 방식으로
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"{i} , Site{RackIndex} BD SerialNumber Match");

                        result = MConv.CP210x_Reset(deviceHandle);

                        if (result == MConv.CP210x_SUCCESS)
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"Site{RackIndex} BD Chip Reset Done");
                        }
                        else
                        {
                            //MessageBox.Show("장치를 리셋할 수 없습니다.");
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드 칩 리셋할 수 없음. {selStienum}");
                        }

                        Thread.Sleep(1000);

                        result = MConv.CP210x_Close(deviceHandle);
                        if (result != MConv.CP210x_SUCCESS)
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드 닫을 수 없음. {i}");
                        }

                        break;
                    }
                }
                else
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드 일련번호를 가져올 수 없음. {i}");
                }

                // 장치 닫기
                result = MConv.CP210x_Close(deviceHandle);
                if (result != MConv.CP210x_SUCCESS)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드 닫을 수 없음. {i}");
                }
            }
            return rtnVal;
        }

        public async void CheckSiteConnet(string getVal)
        {
            if (getVal == "Site")
            {
                Global.gMainPageVM.sc[RackIndex].SiteDisConnect();

                await Task.Delay(3000);

                SiteFWReadWrite();

                await Task.Delay(3000);

                Global.gMainPageVM.sc[RackIndex].SiteConnect(RackIndex);
            }
            else if(getVal == "Cap")
            {
                Global.gMainPageVM.cc[RackIndex].Disconnect();

                await Task.Delay(3000);

                SiteFWReadWrite();

                await Task.Delay(3000);

                Global.gMainPageVM.cc[RackIndex].Connect(RackIndex);
            }
        }


        public async void PreManualMotorDown(string getValue)
        {
            await ManualMotorDown(getValue);
        }
        public async Task<bool>ManualMotorDown(string v)
        {
            if (!Global.gMainPageVM.sc[RackIndex].isMotorStatus)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Already Motor Move");
                Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Already Move");
                return false;
            }

            Global.gMainPageVM.sc[RackIndex].isMotorStatus = false;

            string value = v;//Global.gEngineerVM.ManualMotor[RackIndex].ManualDownVal.ToString();
            Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Manual Down Start");

            string str_cmd = "L" + value;
            Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] " + value);

            WriteMotorDown = true;
            Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
            WriteMotorDown = false;
            await Task.Delay(5000);
            Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
            await Task.Delay(300);

            Global.gMainPageVM.sc[RackIndex].Board_SensingOK = false;

            Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("P");
            
            await Task.Delay(1000);

            if (Global.gMainPageVM.sc[RackIndex].Board_SensingOK)
            {
                Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Down Sense is OK");
            }

            Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
            Global.gEngineerVM.ManualMotor[RackIndex].TotalMotorVal += int.Parse(value); 

            return true;
        }
        public async void PreMotorAutoDown()
        {
            //if(IsRackTCUnit == false) return;
            await MotorAutoDown();
        }
        public bool WriteMotorDown = false;
        public async Task<bool> MotorAutoDown()
        {
            if (!Global.gMainPageVM.sc[RackIndex].isMotorStatus)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Already Motor Move");
                Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Already Move");
                return false;
            }

            Global.gMainPageVM.sc[RackIndex].isMotorStatus = false;
            if (MotorStatText == "CLOSE")
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Motor Already Down");
                Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Already Down");
                Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
                return true;
                // 모터 감압센서 체크하고 off 면 모터 업하고 다운
                // 감압센서 on 이면 retun true
                // 모터 감압센서 체크제외
                /*
                Global.gMainPageVM.sc[RackIndex].Board_SensingOK = false;

                Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("P");
                await Task.Delay(1000);

                if (Global.gMainPageVM.sc[RackIndex].Board_SensingOK)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Motor Already Down");
                    Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Already Down");
                    Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
                    return true;
                }
                else
                {
                    Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
                    await MotorAutoOpen(MOTOR_TYPE.REMOVE);
                    Global.gMainPageVM.sc[RackIndex].isMotorStatus = false;
                }
                */
            }

            int start = int.Parse(Global.gMainPageVM.sc[RackIndex].MotorValue);
            int step = 1000;
            int total = 0;
            int value_offset = 1000;

            int max_value = Global.gConfigVM.MotorMaxVal;
            bool ret_value = false;

            if (max_value > 55000) max_value = 55000;

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Motor Auto Down Start");
            Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Auto Down Start");

            total = start;
            string str_cmd = "L" + start.ToString();
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] " + start.ToString());
            Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] " + start.ToString());

            WriteMotorDown = true;
            Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);// Motor Down 42000
            WriteMotorDown = false;
            await Task.Delay(5000);
            Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
            await Task.Delay(300);

            Global.gMainPageVM.sc[RackIndex].Board_SensingOK = false;

            while (true) //slot_start_command 비정상종료시 사용 유무??
            {
                //xml motorpositon , final motor val Check 유무??

                Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("P");
                await Task.Delay(1000);

                if(Global.gMainPageVM.sc[RackIndex].Board_SensingOK)
                {
                    await Task.Delay(500);
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Down Sense is OK");
                    //Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Offset is " + value_offset.ToString());
                    //Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Down Sense is OK");
                    //Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Offset is " + value_offset.ToString());

                    //str_cmd = "L" + value_offset.ToString();
                    //WriteMotorDown = true;
                    //Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
                    //WriteMotorDown = false;
                    //await Task.Delay(100);
                    //Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
                    //await Task.Delay(100);
                    //
                    //total = total + value_offset;
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Final Step is " + total.ToString());
                    Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Final Step is " + total.ToString());


                    await Task.Delay(500);
                    //xml motorpositon, final motor val Save 유무??
                    //await Task.Delay(1000);
                    ret_value = true;
                    break;
                }
                else
                {
                    total = total + step;

                    if (total > max_value)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Max Spin Over " + total.ToString());
                        Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Max Spin Over " + total.ToString());

                        ret_value = false;
                        break;
                    }

                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] " + total.ToString());
                    Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] " + total.ToString());

                    Global.gMainPageVM.sc[RackIndex].Board_SensingOK = false;

                    str_cmd = "L" + value_offset.ToString();
                    WriteMotorDown = true;
                    Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
                    WriteMotorDown = false;

                    await Task.Delay(100);
                    Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
                    await Task.Delay(300);

                }
            }

            Global.gEngineerVM.ManualMotor[RackIndex].TotalMotorVal += total;
            Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
            return ret_value;
        }
        public async Task<bool> ReturnDownFail(bool isFail)
        {
            Global.gMainPageVM.sc[RackIndex].isMotorStatus = false;
            int Retrycnt = 0;
            //xml RackRun Svae 유무??

            while (true)
            {
                Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("R"); //Interlock Change
                await Task.Delay(1000);
                if(Global.gMainPageVM.sc[RackIndex].Board_MotorPowerOn)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] " + "PowerOn is Success");
                    break;
                }
                Retrycnt++;
                if(Retrycnt > 5)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Fix" + RackViewIndex + "] " + "PowerOn is Fail");
                    break;
                }
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] " + "PowerOn Try Count = " + Retrycnt.ToString());
            }
            
            string str_cmd = "H";
            WriteMotorDown = true;

            Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
            WriteMotorDown = false;
            await Task.Delay(5000);
            Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
            await Task.Delay(100);

            Retrycnt = 0;

            while (true)
            {
                if(Global.gMainPageVM.sc[RackIndex].IsMotorDown == false)
                {
                    //xml Save motor Fail , motor position Save 유무??
                    break;
                }
                else
                {
                    WriteMotorDown = true;
                    Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
                    WriteMotorDown = false;
                    await Task.Delay(5000);
                    Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
                    await Task.Delay(100);
                }

                await Task.Delay(5000);
                Retrycnt++;
                if (Retrycnt > 5)
                    break;

                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] " + "Try Up Count  = " + Retrycnt.ToString());

            }
            if (isFail)
            {
				foreach (BoxModel bm in Boxes)
					bm.BoxStat = Box_STAT.ERROR;
				//RackMotorStat = MOTOR_STAT.ERROR;
				showAlram(ALRAM_TYPE.ERROR, Global.msg_SenseError, false);
                
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[All] Motor " + RackViewIndex + " Down is Fail");
            }
            else
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[All] Motor " + RackViewIndex + " Down is Cancel");
            }
            Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
            return true;
        }
        public async void PreManualMotorOpen(string getVal)
        {
            await ManualMotorOpen(getVal);
        }
        public async Task<bool> ManualMotorOpen(string v)
        {
            if(!Global.gMainPageVM.sc[RackIndex].isMotorStatus)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Already Motor Move");
                Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Already Move");
                return false;
            }
            Global.gMainPageVM.sc[RackIndex].isMotorStatus = false;
            if (MotorStatText == "OPEN")
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Motor Already Up");
                Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Already Up");
                Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
                return false;
            }
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "]  Request to PowerOn");
            Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "]  Request to PowerOn");
            Global.gMainPageVM.sc[RackIndex].Board_MotorPowerOn = false;

            Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("R");
            await Task.Delay(2000);

            int Retrycnt = 0;
            while (true)
            {
                if (Global.gMainPageVM.sc[RackIndex].Board_MotorPowerOn)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] PowerOn is Success");
                    Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] PowerOn is Success");
                    break;
                }
                else
                {
                    await Task.Delay(2000);
                    Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("R");
                }
                Retrycnt++;
                if (Retrycnt > 5)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] PowerOn is Fail");
                    Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] PowerOn is Fail");
                    RackMotorStat = MOTOR_STAT.ERROR;
                    showAlram(ALRAM_TYPE.ERROR, Global.msg_MotorError, true);
                    break;
                }
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "]Try Count = " + Retrycnt.ToString());
                Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "]Try Count = " + Retrycnt.ToString());
            }
            //온도 조절기 정지
            foreach (BoxModel bm in Boxes)
            {
                if (bm == null) continue;
                if (bm.TconIsRun)
                    bm.TCon_BoxStop();
            }
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Motor UP");
            Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor UP");

            Retrycnt = 0;
            string str_cmd = "H" + v;  //motor up
            WriteMotorDown = true;
            Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
            WriteMotorDown = false;
            await Task.Delay(5000);
            Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
            await Task.Delay(100);
            Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("I");
            await Task.Delay(100);

            Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
            return true;
        }
        public async void PreMotorAutoOpen(MOTOR_TYPE value)
        {
            await MotorAutoOpen(value);
        }
        public async Task<bool> MotorAutoOpen(MOTOR_TYPE manual)
        {
            if (!Global.gMainPageVM.sc[RackIndex].isMotorStatus)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Already Motor Move");
                Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Already Move");
                return false;
            }
            Global.gMainPageVM.sc[RackIndex].isMotorStatus = false;
            if (MotorStatText == "OPEN")
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Motor Already Up");
                Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Already Up");
                Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
                return false;
            }
            if (manual != MOTOR_TYPE.REMOVE)
            {
                RunCheckFalg = false;
                for (int j = 0; j < Boxes.Count; j++)
                {
                    if (Boxes[j].BoxStat == Box_STAT.RUN || Boxes[j].BoxStat == Box_STAT.PRERUN)
                        RunCheckFalg = true;
                }

                if (RunCheckFalg)
                {
                    Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
                    return false;
                }
            }

            
            bool IsTempOk = true;
            //double minTemp = 0;
            //int HumTempVal = 0; // 습도센서 온도값
            //int Center_Temp = 250;

            //if (!manual)
            if(manual == MOTOR_TYPE.FAIL)
            {
                for (int j = 0; j < Boxes.Count; j++)
                {
                    if (Boxes[j].BoxDefrost == DEFROST_TYPE.LOWTEMP)
                        IsTempOk = false;
                }
            }
            /*
            IsTempOk = false;

            //온도조절기 박스내 3개중 최소값
            minTemp = Math.Min(Boxes[0].GetPV , Boxes[1].GetPV );
            minTemp = Math.Min(minTemp, Boxes[2].GetPV );
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Rack" + RackViewIndex + "] Rack Min Temp = " + minTemp.ToString());

            HumTempVal = Math.Min(Boxes[0].Hum_TempSenorVal, Boxes[1].Hum_TempSenorVal );
            HumTempVal = Math.Min(HumTempVal, Boxes[2].Hum_TempSenorVal);
            //박스 내부온도 25도 체크

            minTemp *= 10;
            if (minTemp > Center_Temp || minTemp > HumTempVal)
                IsTempOk = true;
            else
                IsTempOk = false;
            */

            if (IsTempOk)
            {
                
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "]  Request to PowerOn");
                Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "]  Request to PowerOn");
                Global.gMainPageVM.sc[RackIndex].Board_MotorPowerOn = false;

                Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("R");
                await Task.Delay(2000);
                int Retrycnt = 0;
                
                while (true)
                {
                    if (Global.gMainPageVM.sc[RackIndex].Board_MotorPowerOn)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] PowerOn is Success");
                        Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] PowerOn is Success");
                        break;
                    }
                    else
                    {
                        await Task.Delay(2000);
                        Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("R");
                    }
                    Retrycnt++;
                    if (Retrycnt > 5)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] PowerOn is Fail");
                        Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] PowerOn is Fail");
                        RackMotorStat = MOTOR_STAT.ERROR;
                        showAlram(ALRAM_TYPE.ERROR, Global.msg_MotorError, true);
                        break;
                    }
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "]Try Count = " + Retrycnt.ToString());
                    Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "]Try Count = " + Retrycnt.ToString());

                }
                
                if(RackMotorStat == MOTOR_STAT.NORMAL || manual == MOTOR_TYPE.REMOVE)
                {
                    //온도 조절기 정지
                    foreach (BoxModel bm in Boxes)
                    {
                        if (bm == null) continue;
                        if(bm.TconIsRun)
                            bm.TCon_BoxStop();
                    }
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Motor UP");
                    Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor UP");

                    Retrycnt = 0;
                    string str_cmd = "H";  //motor up
                    WriteMotorDown = true;
                    Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
                    WriteMotorDown = false;
                    await Task.Delay(5000);
                    Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
                    await Task.Delay(100);
                    Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("I");
                    await Task.Delay(100);

                    while (true)
                    {
                        Global.gMainPageVM.sc[RackIndex].SiteWirteMsg("I");
                        await Task.Delay(1000);

                        if (Global.gMainPageVM.sc[RackIndex].IsMotorDown == false)
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Motor Up Done");
                            Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Up Done");
                            Global.gEngineerVM.ManualMotor[RackIndex].TotalMotorVal = 0;
                            //xml 모터 상태 저장 유무?
                            break;
                        }
                        else
                        {
                            WriteMotorDown = true;
                            Global.gMainPageVM.sc[RackIndex].SiteWirteMsg(str_cmd);
                            WriteMotorDown = false;
                            await Task.Delay(5000);
                            Global.gMainPageVM.sc[RackIndex].SiteHeightWriteMsg(17);
                            await Task.Delay(100);
                        }
                        Retrycnt++;
                        if (Retrycnt > 10)
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Motor Up is failed");
                            Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Motor Up is failed");
                            //RackMotorStat Error Add //0701
                            RackMotorStat = MOTOR_STAT.ERROR;
                            showAlram(ALRAM_TYPE.ERROR, Global.msg_MotorError, true);
                            break;
                        }
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Fix" + RackViewIndex + "] Try Up Count " + Retrycnt.ToString());
                        Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] Try Up Count " + Retrycnt.ToString());
                    }
                }
                else
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix {RackViewIndex} ] Motor UP Skip. MotorStat {RackMotorStat} , If you want to try the motor up, reset the status");
                    Global.gEngineerVM.addMotorLog($"[Fix {RackViewIndex} ] Motor UP Skip. MotorStat {RackMotorStat} , If you want to try the motor up, reset the status");
                }
            }      
            else
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Fix" + RackViewIndex + "] CurTemp is not ok. Can't motor up");
                Global.gEngineerVM.addMotorLog("[Fix" + RackViewIndex + "] CurTemp is not ok. Can't motor up");
                //showAlram(ALRAM_TYPE.ERROR, Global.msg_Cbox_25_temp, true);
            }
            Global.gMainPageVM.sc[RackIndex].isMotorStatus = true;
            return IsTempOk;
        }
        public void startTempLog()
        {
            if (TempSerilog != null)
                TempSerilog.Dispose();
            string file = $"{Global.gConfigVM.TempLogPath}\\TempData_{DateTime.Now.ToString("yyyy_MM_dd_HHmm")}" + $".csv";
            TempSerilog = new LoggerConfiguration()
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite, outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();
        }

        public bool CheckRackState(string getval)
        {
            bool retval = false;

            if (getval == "I")
            {
                retval = !Boxes.Any(bm =>
                          bm.BoxStat == Box_STAT.PRERUN ||
                          bm.BoxStat == Box_STAT.RUN ||
                          bm.BoxStat == Box_STAT.DEFROST);
            }
            else if (getval == "E")
            {
                bool bmhum = Boxes.Any(bm => bm.isLoggingHumidity);
                bool isRunningOrDefrosting = Boxes.Any(bm => bm.BoxStat == Box_STAT.RUN || bm.BoxStat == Box_STAT.DEFROST);

                retval = bmhum || isRunningOrDefrosting;
            }

            return retval;
        }
    }
}
