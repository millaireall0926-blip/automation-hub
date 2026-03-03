using HandyControl.Tools.Extension;
using HTCRack_CXL.Base;
using HTCRack_CXL.Net;
using HTCRack_CXL.ViewModel;
using Org.BouncyCastle.Bcpg;
using PropertyChanged;
using ScottPlot.MarkerShapes;
using Serilog;
using Serilog.Core;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Threading;
using System.Xml;

namespace HTCRack_CXL.Model
{
    public enum RACK_STAT
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
    public enum ALRAM_TYPE
    {
        DONE = 0,
        WARN,
        ERROR,
        INIT
    }
    public enum MOTOR_STAT
    {
        CLOSE = 0,
        CLOSING,
        OPEN,
        OPENING,
        NID
    }

    public enum POWER_STAT : int
    {
        PLC_OFF = 0,
        POWER_OFF = 1, // AC, 48V,12V All Off
        MIXED = 2, // AC, 48V, 12V Mixed Stat
        POWER_ON = 3, // AC, 48V,12V All On
        WAIT_RECV = 4
    }

    public class RackModel : BaseVM
    {
        public string Name { get; set; }
        [DoNotNotify] public int RackIndex { get; set; } = 0;
        [DoNotNotify] public int GraphIndex { get; set; } = 0;
        [DoNotNotify] public List<TempDataModel> TempDatas { get; set; } = new List<TempDataModel>();
        public Visibility ShowBox { get; set; } = Visibility.Hidden;
        public SolidColorBrush BoxColor { get; set; }  // Done, Error Color
        public string BoxMsg { get; set; } = "";
        public void showAlram(ALRAM_TYPE type, string head, string msg, bool isLogging = true)
        {
            BoxColor = Global.gAlramColors[(int)type];
            BoxMsg = msg;
            ShowBox = Visibility.Visible;

            if (isLogging)
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, head + ":" + msg);
        }
        public SolidColorBrush BoarderColor { get; set; } // Rack Boarder Color = State Color
        public SolidColorBrush RackColor { get; set; } = Global.gRackColors[0];
        public SolidColorBrush MotorColor { get; set; }

        public bool MotorStopFlag { get; set; } = false;

        public DelegateCommand DGRackCmdAll { get; set; }
        public bool canExcuteRun(object obj)
        {
            if (RackStat == RACK_STAT.STOP ||
                RackStat == RACK_STAT.DONE)
                return true; // 버튼 활성화

            return false; //버튼 비활성화
        }
        public DelegateCommand DGCmdRun { get; set; }
        public bool canExcuteStop(object obj)
        {
            if (RackStat == RACK_STAT.RUN ||
                RackStat == RACK_STAT.ERROR)
                return true;

            return false;
        }
        public DelegateCommand DGCmdStop { get; set; }
        public DelegateCommand DGCmdSetTemp { get; set; }

        ObservableCollection<BoxModel> _boxs = new ObservableCollection<BoxModel>();
        public ObservableCollection<BoxModel> Boxs
        {
            get { return _boxs; }
            set { _boxs = value; PC(nameof(Boxs)); }
        }
        public bool _isSaveOnlyTemp = false;
        public bool IsSaveOnlyTemp
        {
            get => _isSaveOnlyTemp;
            set
            {
                _isSaveOnlyTemp = value;
                if (!_isSaveOnlyTemp) { endTempLog(); }
            }
        }
        public bool EnableSV { get; set; } = true;
        public double SetSV { get; set; } = 0;
        [DoNotNotify]
        public int SideOpenDetect { get; set; } = 0;

        bool _IsTams = false;
        public bool IsTams
        {
            get => _IsTams;
            set
            {
                _IsTams = value;
                if (_IsTams)
                {
                    EnableSV = false;
                }
                else
                {
                    EnableSV = true;
                }

                LogHelper.Info($"[User][{Name}]", $"Tams {_IsTams}");
            }
        }
        bool _isTCUnit = true;
        public bool IsTCUnit
        {
            get => _isTCUnit;
            set
            {
                _isTCUnit = value;

                LogHelper.Info($"[User][{Name}]", $"TCUnit {_isTCUnit}");
            }
        }
        bool _isAS = false;
        public bool IsAS
        {
            get => _isAS;
            set
            {
                _isAS = value;
                if (!_isAS)
                {
                    RackColor = Global.gRackColors[0];

                    Boxs.ForEach(bm => bm.BoxColor = RackColor);
                    
                    foreach(BoxModel bm in Boxs)
                        bm.Sites.ForEach(st => st.SiteColor = RackColor);
                    
                    sendExtFan(0x1);
                }
                else
                {
                    RackColor = Global.gRackColors[1];

                    Boxs.ForEach(bm => bm.BoxColor = RackColor);

                    foreach (BoxModel bm in Boxs)
                        bm.Sites.ForEach(st => st.SiteColor = RackColor);

                    sendExtFan(0x0);
                }

                LogHelper.Info($"[User][{Name}]", $"AS {_isAS}");
            }
        }
        public double _prevSV = 0;
        public double PrevSV  // 재접속시 온도 setting용
        {
            get => _prevSV;
            set
            {
                _prevSV = value;
            }
        }

        public POWER_STAT _powerStat = POWER_STAT.PLC_OFF;
        public POWER_STAT PowerStat
        {
            get => _powerStat;
            set
            {
                _powerStat = value;
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
                DGCmdPower?.RaiseCanExecuteChanged()));

            }
        }
        public DelegateCommand DGCmdPower { get; set; }

        public string StatText {  get; set; }
        public string MotorText { get; set; }
        public Visibility RackEngrmodeVisibility { get; set; } = Visibility.Hidden;

        [DoNotNotify]
        public string FpgaVersion { get; set; } = "";

        RACK_STAT _rackStat;
        public RACK_STAT RackStat
        {
            get => _rackStat;
            set
            {
                _rackStat = value;
                BoarderColor = Global.gBoarderColors[(int)_rackStat];

                DGCmdRun?.RaiseCanExecuteChanged();
                DGCmdStop?.RaiseCanExecuteChanged();

                StatText = _rackStat.ToString();

                //Rack - Box Stat 연동 할지 말지 
                //foreach (BoxModel bm in Boxs)
                //    bm.BoxStat = (BOX_STAT)_rackStat;
            }
        }

        MOTOR_STAT _motorStat;
        public MOTOR_STAT MotorStat
        {
            get => _motorStat;
            set
            {
#if TEST_MODE
                Console.WriteLine($"MotorStat set called: {value}");
#endif
                _motorStat = value;
                MotorColor = Global.gMotorStatColors[(int)_motorStat];

                MotorText = _motorStat.ToString();
            }
        }

        public DateTime TimeStart { get; set; }
        public DateTime TimeEnd { get; set; }

        public HtcClient _client = new HtcClient();
        public RackModel(int index)
        {
            Name = "FIX" + (index + 1).ToString();
            RackIndex = index;

            loadMap();


#if TEST_MODE
            MotorStat = MOTOR_STAT.OPEN;
            RackStat = RACK_STAT.STOP;
            //MotorStat = MOTOR_STAT.CLOSE;
#else
            MotorStat = MOTOR_STAT.NID;
            RackStat = RACK_STAT.DISCONNECT;
#endif
            // 4개 Temp Value Save
            for (int i = 0; i < 4; i++)
                TempDatas.Add(new TempDataModel());

            DGRackCmdAll = new DelegateCommand(async p =>
            {
                LogHelper.Info($"[User][{Name}]", $"{p}");
                
                switch (p)
                {
                    case "RackStatClear":
                        RackStatusClear();
                        break;
                    case "RackHumOn":
#if TEST_MODE
                        RackStat = RACK_STAT.RUN;
                        foreach (BoxModel bm in Boxs)
                        {
                            bm.BoxStat = BOX_STAT.RUN;
                            foreach (SiteModel sm in bm.Sites)
                                sm.SiteStat = SITE_STAT.RUN;
                        }
#else
                        foreach(BoxModel bm in Boxs)
                            bm.isLoggingHumidity = true;
#endif
                        break;
                    case "RackHumOff":
#if TEST_MODE
                        RackStat = RACK_STAT.DEFROST;
                        foreach (BoxModel bm in Boxs)
                        {
                            bm.BoxStat = BOX_STAT.DEFROST;
                            foreach (SiteModel sm in bm.Sites)
                                sm.SiteStat = SITE_STAT.DEFROST;
                        }
#else
                        foreach (BoxModel bm in Boxs)
                            bm.isLoggingHumidity = false;
#endif
                        break;
                    case "RackMotorUp":
#if TEST_MODE
                        SampleprocessStat();
#else
                        await MotorAutoUp();
#endif
                        break;
                    case "RackMotorDown":
#if TEST_MODE
                        await SampleTest();
#else
                        await MotorAutoDown();
#endif
                        break;
                    case "RackMotorStop":
                        await MotorStop();
                        break;
                }
            });

            DGCmdRun = new DelegateCommand(p => { startTest(); }, canExcuteRun);
            DGCmdStop = new DelegateCommand(p => { PrestopTest(); }, canExcuteStop);
            DGCmdPower = new DelegateCommand(p => { clickedPower(); }, canExcutePower);

            DGCmdSetTemp = new DelegateCommand(p => { setTemp(); });
        }
        public void loadMap()
        {
            if (Global.gConfigDoc == null) return;

            var node = Global.gConfigDoc.SelectSingleNode("cxl-rack/rack-map/rack");
            var rackNode = node.SelectSingleNode($"//rack[@id='{RackIndex}']");

            //int boxindex = 0;
            foreach(XmlNode boxNode in rackNode.ChildNodes)
            {
                BoxModel box = new BoxModel(boxNode.Attributes["name"].Value, Convert.ToInt32(boxNode.Attributes["id"].Value) , this);

                foreach(XmlNode subNode in boxNode.ChildNodes)
                {
                    if (subNode.Name != "site") continue;
                    SiteModel site = new SiteModel(Convert.ToInt32(subNode.Attributes["id"].Value), box);
                    var itemNode = subNode.SelectSingleNode("sv");
                    string text = itemNode.InnerText;
                    string[] buf = text.Split(',');
                    foreach(string s in buf)
                    {
                        if (s == "") break;
                        site.SVCtrlList.Add(Convert.ToInt32(s));
                    }

                    itemNode = subNode.SelectSingleNode("ssr");
                    text = itemNode.InnerText;
                    buf = text.Split(',');
                    foreach(string s in buf)
                    {
                        if (s == "") break;
                        int shift = Convert.ToInt32(s);
                        int value = 1 << shift;
                        site.SSRValue |= value;
                    }

                    itemNode = subNode.SelectSingleNode("fp");
                    text = itemNode.InnerText;
                    site.FPIndex = Convert.ToInt32(text);

                    itemNode = subNode.SelectSingleNode("fan");
                    text = itemNode.InnerText;
                    buf = text.Split(',');
                    foreach (string s in buf)
                    {
                        if (s == "") break;
                        site.FanCtrlList.Add(Convert.ToInt32(s));
                    }


                    itemNode = subNode.SelectSingleNode("pv");
                    text = itemNode.InnerText;
                    buf = text.Split(',');
                    foreach (string s in buf)
                    {
                        if (s == "") break;
                        site.PVCtrlList.Add(Convert.ToInt32(s));
                    }
                    box.Sites.Add(site);
                }
                Boxs.Add(box);
            }
            /*
            //Test Code
            if (RackIndex == 0)
            {
                _client._ip = "127.0.0.1";
                _client._index = RackIndex;
                _client._port = 2020;

                _client.funcRecv += doRecv;
                _client.funcConect += connected;
            }
            else
            {
                _client._ip = rackNode.Attributes["ip"].Value;
                _client._index = RackIndex;
                _client._port = Convert.ToInt32(rackNode.Attributes["port"].Value);

                _client.funcRecv += doRecv;
                _client.funcConect += connected;
            }
            */
            
            _client._ip = rackNode.Attributes["ip"].Value;
            _client._index = RackIndex;
            _client._port = Convert.ToInt32(rackNode.Attributes["port"].Value);

            _client.funcRecv += doRecv;
            _client.funcConect += connected;
            
        }
        #region 내부 Data용 Regs
        public List<RegModel> FanRegs = new List<RegModel>();
        public List<RegModel> CurRegs = new List<RegModel>();
        public List<RegModel> TempRegs = new List<RegModel>();
        public List<RegModel> DimRegs = new List<RegModel>();
        public List<RegModel> TConRegs = new List<RegModel>();
        public List<RegModel> GPIORegs = new List<RegModel>();
        public List<RegModel> MOTORRegs = new List<RegModel>();
        #endregion
        public void initRegs()
        {
            foreach (RegModel reg in Global.gEngineerVM.FanRegModels)
                FanRegs.Add(MConv.ObjectDeepCopy<RegModel>(reg));
            foreach (RegModel reg in FanRegs)
            {
                if (reg.FUNC == "11") reg.funcReadVlaue += doValueFanGetDuty;
                if (reg.FUNC == "20") reg.funcReadVlaue += doValueFanGetTach;
            }
            
            foreach (RegModel reg in Global.gEngineerVM.CurRegModels)
                CurRegs.Add(MConv.ObjectDeepCopy(reg));


            foreach (RegModel reg in CurRegs)
            {
                if (reg.FUNC == "11")
                {
                    if (Convert.ToInt32(reg.ADR) < 8)
                        reg.funcReadVlaue += doValueGetCurrentBox;
                    else
                        reg.funcReadVlaue += doValueGetCurrentEtc;
                }

            }

            foreach (RegModel reg in Global.gEngineerVM.TempRegModels)
                TempRegs.Add(MConv.ObjectDeepCopy(reg));
            foreach (RegModel reg in TempRegs)
            {
                if (reg.FUNC == "11") reg.funcReadVlaue += doValueGetTemp;
            }

            //Dimm 사용안함
            /*
            foreach (RegModel reg in Global.gEnginerVM.DimRegModels)
                DimRegs.Add(MConv.ObjectDeepCopy(reg));
            foreach (RegModel reg in DimRegs)
                reg.funcReadVlaue += doValueGetDim;
            */

            // RegMap TCon_xxx Index
            foreach (RegModel reg in Global.gEngineerVM.TConRegModels)
                TConRegs.Add(MConv.ObjectDeepCopy(reg));
            foreach (RegModel reg in TConRegs)
            {
                // Temp Value
                if (reg.FUNC == "10" || reg.FUNC == "20" || reg.FUNC == "21"
                    || reg.FUNC == "30" || reg.FUNC == "31")
                    reg.funcReadVlaue += doValueTconTemp;
                else
                    reg.funcReadVlaue += doValueDecimal;


            }

            foreach (RegModel reg in Global.gEngineerVM.GPIORegModels)
                GPIORegs.Add(MConv.ObjectDeepCopy(reg));
            foreach (RegModel reg in GPIORegs)
                reg.funcReadVlaue += doValueDecimal;

            //MotorRegModels Add
            foreach (RegModel reg in Global.gEngineerVM.MOTORRegModels)
                MOTORRegs.Add(MConv.ObjectDeepCopy(reg));
            foreach (RegModel reg in MOTORRegs)
            {
                if (reg.Discription == "Get Motor Step ")
                    reg.funcReadVlaue += doValueDecimal32;
                else
                    reg.funcReadVlaue += doValueDecimal;
            }
        }

        public async void startTest()
        {
            LogHelper.Info($"[User][{Name}]", "Run");
            
            if (IsAS) return;
            if (!(RackStat == RACK_STAT.STOP || RackStat == RACK_STAT.DONE || RackStat == RACK_STAT.ERROR)) return;

            if (!IsTCUnit)
            {
                showAlram(ALRAM_TYPE.WARN, $"[Prog][{Name}]", "TC Unit Uncheck", isLogging: false);
                return;
            }

            if (IsTams)
            {
                if (!setTamsTemp(onlyFileFind: false))
                {
                    //showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]", "Tams Temp File Error");
                    LogHelper.Warn($"[Prog][{Name}]", "Tams File Disappeared or Format Discrepancies");
                    return;
                }
                else
                    LogHelper.Info($"[Prog][{Name}]", $"Tams SetTemp {SetSV}℃");
            }

            if (SetSV < Global.gConfigVM.MinSetSV || SetSV > Global.gConfigVM.MaxSetSV)
            {
                showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]", "SetTemp Range is " + Global.gConfigVM.MinSetSV + "~" + Global.gConfigVM.MaxSetSV + ". Check TargetSV");
                return;
            }

            ShowBox = System.Windows.Visibility.Hidden;
            //  Side Door Check 0 = Close , 1 = Open
            if (GPIORegs[RegMap.GPIO_SideClose].ReadValue == 1)
            {
                showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]", "Side Door Open");
                return;
            }

            RackStat = RACK_STAT.PREV_RUN;
            foreach (BoxModel bm in Boxs)
            {
                bm.BoxStat = BOX_STAT.PREV_RUN;
                foreach (SiteModel sm in bm.Sites)
                    sm.SiteStat = SITE_STAT.PREV_RUN;
            }

#if TEST_MODE
            await Task.Delay(5000);
            MotorStat = MOTOR_STAT.CLOSE;
#else
            //Motor Down ,Up Stat Check
            if (MotorStat == MOTOR_STAT.OPEN)
            {
                bool MotorRes = await MotorAutoDown();
                if (!MotorRes)
                {
                    RackStat = RACK_STAT.STOP;
                    foreach (BoxModel bm in Boxs)
                        bm.BoxStat = BOX_STAT.STOP;
                    return;
                }
            }
            else if (MotorStat == MOTOR_STAT.CLOSE)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : Motor is already down");
            }
            else
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : Motor is {MotorStat}.Check position");
                return;
            }
            
#endif

            PrevSV = SetSV;

            sendFPSV(SetSV);
            //if (Global.gConfigVM.FPDisplay != 0)
            //    sendFPDisplay(Global.gConfigVM.FPDisplay);

            TimeStart = DateTime.Now;
            TimeEnd = default(DateTime);

            Global.gMainVM.PlcCtl.setSlotMC(RackIndex, true);
            Thread.Sleep(1000);

            foreach(BoxModel bm in Boxs)
            {
                int ssr = 0;
                foreach(SiteModel sm in bm.Sites)
                {
                    ssr |= sm.SSRValue;
                    sm.SiteStat = SITE_STAT.RUN;
                }
                bm.sendSV(SetSV);
                bm.sendRun();
                bm.sendFan(100);

                bm.isLoggingHumidity = true;

                addSSR(ssr);
                bm.BoxStat = BOX_STAT.RUN;
            }

            RackStat = RACK_STAT.RUN;

            startTempLog();
            GraphIndex = 0;

        }
        public void setTemp()
        {
            if (IsAS) return;
            if (!IsTCUnit) return;

            LogHelper.Info($"[Prog][{Name}]", $"SetTemp {SetSV}℃");

            foreach (BoxModel box in Boxs)
            {
                box.sendSV(SetSV);
            }
            sendFPSV(SetSV);
        }

        public async Task<bool> MotorAutoDown()
        {
            //Motor Operating
            //1. Motor Down 상태 체크 (Hi.Limit = 0 만 할지 Box.Conatct 1할지) 후 Motor Auto Ctrl
            //2. Motor Auto Ctrl 2 : Down , MotorStat = Closing 변경
            //3. Step 으로 Step1 - 모터 Auto , Step2 - 모터 stop , Step1 -> step2 로 10초내에 안넘어가면 Error Step1 -> step2 로 최대 20초 내 안남어가면 Error
            //4. Motor.Run 0 (Stop) 이면 CBOX.CONTACT 4bit 1 인지 체크 4bit 중에 사용하는거만 1로 나오는지 확인후 Contact 확인
            //5. Contact Fail 이면 모터 up시키고 contact Pass 이면 다음 스텝 진행

            MotorStopFlag = false;

            // Step1: 모터 Auto Down 명령
            sendMotorCtrl(0x02); // Motor Auto Down

            // Step1 대기: 최대 15초 동안 Run 상태가 "0"이 될 때까지 대기
            int timeoutMs = 15000;
            int intervalMs = 1000;
            int elapsed = 0;

            while(elapsed < timeoutMs)
            {
                if (MotorStopFlag)
                {
                    LogHelper.Info($"[Prog][{Name}]", "Motor Stop Flag");
                    return false;
                }
                if (MOTORRegs.Count == 0)
                {
                    LogHelper.Warn($"[Prog][{Name}]", "MOTORRegs Count 0. Check Communication");
                    return false;
                }
                
                //모터가 Stop 이면 빠져나가게
                if (MOTORRegs[RegMap.MOTOR_Run].Read == "0")
                    break;

                await Task.Delay(intervalMs);
                elapsed += intervalMs;
                //Console.WriteLine($"Down Wait");
            }

            // 15초 초과해도 "0" 안됐으면 실패 "1" 이면 모터 동작중
            if (MOTORRegs[RegMap.MOTOR_Run].Read != "0")
            {
                LogHelper.Info($"[Prog][{Name}]", $"Motor Stat is {MOTORRegs[RegMap.MOTOR_Run].Read}");
                return false;
            }

            // Step2: HI_Limit 및 CBOX_Contact 확인
            bool hiLimit = MOTORRegs[RegMap.HI_Limit].Read == "0";
            var result = CompareBitPattern(MOTORRegs[RegMap.CBOX_Contact].Read, Global.gConfigVM.UseBoxContact);

            //bool contactOK = MOTORRegs[RegMap.CBOX_Contact].Read == "0xF";//all Pos 0xF //4 POS 0x8 ,3 POS 0x4, 2 POS 0x2, 1 POS 0x1
                                                                            
            if (hiLimit)
            {
                if (result.isMatch)
                {
                    LogHelper.Info($"[Prog][{Name}]", $"Contact OK, Moter Step {MOTORRegs[RegMap.MOTOR_Step].Read}");
                    return true;
                }
                else
                {
                    LogHelper.Info($"[Prog][{Name}]", $"Contact Fail 위치: {string.Join(", ", result.MismatchedPositions)}");
                    await MotorAutoUp();
                    showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]", "Motor Down 동작 Error");
                    return false;
                }
            }
            else
            {
                LogHelper.Info($"[Prog][{Name}]", $"Motor HI.LIMIT Value {hiLimit} Error");
                showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]", "Motor Down 동작 Error");
                return false;
            }
        }
        public async Task<bool> MotorAutoUp()
        {
            MotorStopFlag = false;
            // Step1: 모터 Auto Up 명령
            sendMotorCtrl(0x01); // Motor Auto Up

            // Step1 대기: 최대 15초 동안 Run 상태가 "0"이 될 때까지 대기
            int timeoutMs = 15000;
            int intervalMs = 1000;
            int elapsed = 0;

            while (elapsed < timeoutMs)
            {
                if(MotorStopFlag)
                {
                    LogHelper.Info($"[Prog][{Name}]", "Motor Stop Flag");
                    return false;
                }
                if (MOTORRegs.Count == 0)
                {
                    LogHelper.Warn($"[Prog][{Name}]", "MOTORRegs Count 0. Check Communication");
                    return false;
                }
                if (MOTORRegs[RegMap.HI_Limit].Read == "1")
                {
                    LogHelper.Info($"[Prog][{Name}]", "Motor Up Ok");
                    break;
                }

                await Task.Delay(intervalMs);
                elapsed += intervalMs;
                //Console.WriteLine($"UP Wait");
            }
            if(MOTORRegs[RegMap.HI_Limit].Read != "1")
            {
                LogHelper.Info($"[Prog][{Name}]", $"Motor HI.LIMIT Value {MOTORRegs[RegMap.HI_Limit].Read} Error");
                showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]", "Motor UP 동작 Error");
                return false;
            }

            return false;
        }
        public async Task MotorStop()
        {
            sendMotorCtrl(0x0); // Motor Stop

            MotorStopFlag = true;

            await Task.Delay(2000);

            LogHelper.Info($"[Prog][{Name}]", "Motor Stop Ok");
        }
        public async void PrestopTest()
        {
            if (IsAS) return;
            if (!IsTCUnit) return;
            if (RackStat != RACK_STAT.RUN) return;

            LogHelper.Info($"[Prog][{Name}]", "Pre Stop Test");

            bool[] DefrostDoneFlag = { false, false};

            RackStat = RACK_STAT.DEFROST;

            //var taskStopList = Boxs.Select(bm => StopBoxTest(bm, DefrostDoneFlag)).ToList();
            var dict = new Dictionary<BoxModel, bool>();
            var taskStopList = Boxs.Select(async bm =>
            {
                bool res = await bm.BoxStopTest();
                dict[bm] = res;
                return res;
            });

            LogHelper.Info($"[Prog][{Name}]", "Task.WaitAll IN");
            try
            {
                DefrostDoneFlag = await Task.WhenAll(taskStopList);
            }
            catch (Exception ex)
            {
                LogHelper.Error($"[Prog][{Name}]", $"Exception during StopBoxTest - {ex.Message}");
            }
            LogHelper.Info($"[Prog][{Name}]", "Task.WaitAll OUT");

            if (DefrostDoneFlag.Any(r => r)) await stopTest();
            else LogHelper.Info($"[Prog][{Name}]", "Box No Runing. Step Skip");
            
            //if(Array.IndexOf(DefrostDoneFlag, true) == -1)
            //    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : Box No Runing. Step Skip");
            //else
            //    await stopTest();

        }
        private CancellationTokenSource _ctsMotorWait;
        private async Task stopTest()
        {
            _ctsMotorWait?.Cancel();
            _ctsMotorWait = new CancellationTokenSource();
            var token = _ctsMotorWait.Token;

            if (MotorStat == MOTOR_STAT.CLOSE)
            {
                try
                {
                    for (int i = 0; i < 6; i++)
                    {
                        await Task.Delay(TimeSpan.FromSeconds(10), token);
                        //await Task.Delay(1000 * 10);
                        LogHelper.Info($"[Prog][{Name}]", $"MotorUp WaitTimer {((i + 1) * 10)}");
                    }
                }
                catch (TaskCanceledException)
                {
                    LogHelper.Info($"[Prog][{Name}]", "MotorUp Wait Canceled");
                }

                bool isMotorUP = CheckMotorUpStat();

                if (isMotorUP)
                {
#if TEST_MODE
                    MotorStat = MOTOR_STAT.OPEN;
#else
                    //모터 업 동작 진행
                    bool MotorRes = await MotorAutoUp();
                    if(!MotorRes)
                        LogHelper.Warn($"[Prog][{Name}]", "Motor up Fail");
#endif
                }
                else
                {
                    LogHelper.Info($"[Prog][{Name}]", "CurTemp is not ok. Can't motor up");
                }

                foreach (BoxModel bm in Boxs)
                {
                    if (bm.BoxStat == BOX_STAT.DEFROST)
                    {
                        if (bm.BoxDefrost == DEFROST_TYPE.NORMAL)
                        {
                            bm.BoxStat = BOX_STAT.DONE;
                            foreach (SiteModel sm in bm.Sites)
                                sm.SiteStat = SITE_STAT.DONE;
                            bm.showBoxAlarm(ALRAM_TYPE.DONE, "테스트 완료", true);
                        }
                        else if (bm.BoxDefrost == DEFROST_TYPE.TAMS)
                        {
                            bm.BoxStat = BOX_STAT.DONE;
                            foreach (SiteModel sm in bm.Sites)
                                sm.SiteStat = SITE_STAT.DONE;
                            bm.showBoxAlarm(ALRAM_TYPE.DONE, "TAMS 테스트 완료", true);
                        }
                        else if (bm.BoxDefrost == DEFROST_TYPE.HUMIDITY)
                        {
                            LogHelper.Info($"[Prog][{Name}]", "Humidity Stop Finished");
                        }
                        else if (bm.BoxDefrost == DEFROST_TYPE.TEMPERROR)
                        {
                            LogHelper.Info($"[Prog][{Name}]", "Box TempError Stop Finished");
                        }
                        bm.BoxDefrost = DEFROST_TYPE.NORMAL;

                    }
                }
                clearSSR();
                endTempLog();

                if (Global.gConfigVM.FPDisplay != 0)
                    sendFPDisplay(0);// FP Display PV로.

                await Task.Delay(1000);
                //Thread.Sleep(1000);
                Global.gMainVM.PlcCtl.setSlotMC(RackIndex, false);

                if (Boxs.Any(bm => bm.BoxStat == BOX_STAT.ERROR))
                    RackStat = RACK_STAT.ERROR;
                else 
                    RackStat = RACK_STAT.DONE;
            }
            else if (MotorStat == MOTOR_STAT.OPEN)
                LogHelper.Info($"[Prog][{Name}]", "Motor Already Up");
        }
        /*
        private async Task StopBoxTest(BoxModel bm, bool[] stopFlag)
        {
            try
            {
                stopFlag[bm.BoxIndex] = await bm.BoxStopTest();
            }
            catch(Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{Name}] : box{bm.BoxIndex + 1} StopTest 실패 {ex.Message}");
                stopFlag[bm.BoxIndex] = false;
            }
        }
        */

        private bool CheckMotorUpStat()
        {
            foreach(BoxModel bm in Boxs)
            {
                if (bm.BoxDefrost == DEFROST_TYPE.LOWTEMP)
                    return false;
            }
            return true;
        }
        public async void RackStatusClear()
        {
            ShowBox = Visibility.Hidden;

            var tasks = Boxs.Select(bm => bm.BoxStatusClear());
            await Task.WhenAll(tasks);

            RackStat = RACK_STAT.STOP;
        }

        #region Process Recieve Packet
        public void recvFan(HtcMessage msg)
        {
            if (msg.sub.Chanel != 0xff) // Multi Register Data
            {
                if (msg.sub.Func == 0x0) return; // Write Reg

                FanRegs[1].Read = "";
                for (int i = 0; i < msg.data.Length; i++)
                    FanRegs[1].Read += convStr(msg.data, i, 1);  // msg.data[i].ToString("X2") + " ";


            }
            else  // Single Register
            {
                if (msg.sub.Func == 0x10) return; //write Reg

                if (msg.sub.Func == 0x11)
                {
                    for (int i = 0; i < msg.data.Length; i++)
                        FanRegs[RegMap.Fan_GetDuty + msg.sub.Adr + i].Read = convStr(msg.data, i, 1);

                    // Sim Test Code
                    //if (Global.gEnginerVM.CurRack == RackIndex)
                    //    Global.gEnginerVM.setRack();
                }
                else if (msg.sub.Func == 0x20)
                    FanRegs[RegMap.Fan_GetTach + msg.sub.Adr].Read = convStr(msg.data, 0, 2);
            }
        }

        public void recvCur(HtcMessage msg)
        {
            if (msg.sub.Chanel != 0xff) // Multi Register Data
            {
                CurRegs[1].Read = "";
                for (int i = 0; i < msg.data.Length; i += 2)
                    CurRegs[1].Read += convStr(msg.data, i, 2);  // msg.data[i].ToString("X2") + " ";                    
            }
            else  // Single Register
            {
                CurRegs[RegMap.Cur_GetCur + msg.sub.Adr].Read = convStr(msg.data, 0, 2);
            }
        }

        public void recvTemp(HtcMessage msg)
        {
            if (msg.sub.Chanel != 0xff) // Multi Register Data
            {
                TempRegs[1].Read = "";
                for (int i = 0; i < msg.data.Length; i += 2)
                {
                    TempRegs[1].Read += convStr(msg.data, i, 2);  // msg.data[i].ToString("X2") + " ";                    
                }
            }
            else
            {
                TempRegs[RegMap.Temp_GetTemp + msg.sub.Adr].Read = convStr(msg.data, 0, 2);
            }
        }

        public void recvDim(HtcMessage msg)
        {
            //Dimm 사용안함
            /*
            int regIdx = msg.sub.Adr * 2;

            DimRegs[RegMap.Dim_GetDim + (regIdx * 2)].Read = convStr(msg.data, 0, 2);
            DimRegs[RegMap.Dim_GetDim + (regIdx * 2) + 1].Read = convStr(msg.data, 2, 2);
            */
        }

        public void recvTCon(HtcMessage msg)
        {
            if (msg.sub.Chanel != 0xff)
            {
                TConRegs[1].Read = "";
                for (int i = 0; i < msg.data.Length; i += 2)
                {
                    TConRegs[1].Read += convStr(msg.data, i, 2);  // msg.data[i].ToString("X2") + " ";                    
                }
            }
            else
            {
                if (msg.sub.Func == 0x20)
                {
                    TConRegs[RegMap.TCon_GetSV + msg.sub.Adr].Read = convStr(msg.data, 0, 2);
                }
                else if (msg.sub.Func == 0x21)
                {
                    TConRegs[RegMap.TCon_GetPV + msg.sub.Adr].Read = convStr(msg.data, 0, 2);
                }
                else if (msg.sub.Func == 0x22)
                {
                    TConRegs[RegMap.TCon_GetMV + msg.sub.Adr].Read = convStr(msg.data, 0, 2);
                }
                else if (msg.sub.Func == 0x23)
                {
                    TConRegs[RegMap.TCon_GetCHSTS + msg.sub.Adr].Read = convStr(msg.data, 0, 2);
                }
                else if (msg.sub.Func == 0x24)
                {
                    TConRegs[RegMap.TCon_GetALM + msg.sub.Adr].Read = convStr(msg.data, 0, 2);
                }
                else if (msg.sub.Func == 0x31)
                {
                    TConRegs[RegMap.TCon_GetBS1 + msg.sub.Adr].Read = convStr(msg.data, 0, 2);
                }

            }
        }

        public void recvPDO(HtcMessage msg)
        {
            if (msg.sub.Func == 0x10) // Version
            {
                FpgaVersion = Encoding.Default.GetString(msg.data);
                if (Global.gMainVM.ShowConfig == Visibility.Visible && Global.gConfigVM.CurRack == RackIndex)
                    Global.gConfigVM.FpgaVer = FpgaVersion;

            }
            else if (msg.sub.Func == 0x12) // Interface State
            {


            }
            else if (msg.sub.Func == 0x11) // all status
            {
                _client._ackWaitCount = 0;
                //######################   TOTAL Byte 334 Byte ##########################
                int ofs = 0;
                //Console.WriteLine($"Start {ofs}");
                for (int i = 0; i < 15; i++, ofs += 2) //30 Byte FAN TACH
                    FanRegs[RegMap.Fan_GetTach + i].Read = convStr(msg.data, ofs, 2, true);

                //Console.WriteLine($"FanRegs Fan_GetTach End {ofs}");
                for (int i = 0; i < 11; i++, ofs += 2) //22 Byte CUR
                    CurRegs[RegMap.Cur_GetCur + i].Read = convStr(msg.data, ofs, 2, true);

                //Console.WriteLine($"CurRegs Cur_GetCur End {ofs}");
                for (int i = 0; i < 8; i++, ofs += 2) //16 Byte BL TEMP
                    TempRegs[RegMap.Temp_GetTemp + i].Read = convStr(msg.data, ofs, 2);

                //Console.WriteLine($"TempRegs Temp_GetTemp End {ofs}");

                //Console.WriteLine($"MaxFixDimmCount {Global.gMainPageVM.MaxFixDimmCount}");
                //                Console.WriteLine($"----------------------------------------------");
                for (int i = 0; i < Global.gMainPageVM.MaxFixDimmCount; i++, ofs += 2) //128 Byte DIMM TEMP
                {
                    ofs += 2;
                    //DimmTemp Read 안함
                    /*
                    int idxH = DimPoslList[i] * 2;
                    DimRegs[idxH].Read = convStr(msg.data, ofs, 2, true);
                    if (_prevDimmTemps[idxH] != 0 && DimRegs[idxH].ReadValue != 0)
                    {
                        double diff = DimRegs[idxH].ReadValue - _prevDimmTemps[idxH];
                        if (Math.Abs(diff) > 10)
                        {
                            _prevDimmEqualCounts[idxH]++;
                            if (_prevDimmEqualCounts[idxH] < 10)  // 값차이가 10번 이상반복될경우 맞는값으로 사용 
                                DimRegs[idxH].ReadValue = _prevDimmTemps[idxH];
                        }
                        else
                        {
                            _prevDimmEqualCounts[idxH] = 0;
                        }
                    }
                    _prevDimmTemps[idxH] = DimRegs[idxH].ReadValue;


                    int idxL = DimPoslList[i] * 2 + 1;
                    ofs += 2;
                    DimRegs[idxL].Read = convStr(msg.data, ofs, 2, true);
                    if (_prevDimmTemps[idxL] != 0 && DimRegs[idxL].ReadValue != 0)
                    {
                        double diff = DimRegs[idxL].ReadValue - _prevDimmTemps[idxL];
                        if (Math.Abs(diff) > 10)
                        {
                            _prevDimmEqualCounts[idxL]++;
                            if (_prevDimmEqualCounts[idxL] < 10)  // 값차이가 10번 이상반복될경우 맞는값으로 사용 
                                DimRegs[idxL].ReadValue = _prevDimmTemps[idxL];
                        }
                        else
                        {
                            _prevDimmEqualCounts[idxL] = 0;
                        }

                    }
                    _prevDimmTemps[idxL] = DimRegs[idxL].ReadValue;
                    */

                    //                    Console.Write($"i = {i},  ");
                    //                    Console.Write($"DimReg[{DimPoslList[i] * 2}] = {DimRegs[DimPoslList[i] * 2].ReadValue}, ");
                    //                    Console.WriteLine($"DimReg[{DimPoslList[i] * 2 + 1}] = {DimRegs[DimPoslList[i] * 2].ReadValue} ");

                }
                //Console.WriteLine($"DimRegs Dimm Temp End {ofs}");

                for (int i = 0; i < 8; i++, ofs += 2) //16 Byte TCON-N.SV
                    TConRegs[RegMap.TCon_GetSV + i].Read = convStr(msg.data, ofs, 2);

                //Console.WriteLine($"TConRegs TCon_GetSV End {ofs}");

                for (int i = 0; i < 8; i++, ofs += 2) //16 Byte TCON-N.PV
                    TConRegs[RegMap.TCon_GetPV + i].Read = convStr(msg.data, ofs, 2);

                //Console.WriteLine($"TConRegs TCon_GetPV End {ofs}");

                for (int i = 0; i < Global.gConfigVM.CoolerPVs.Count; i++) //16Byte COOLER-N.PV
                {
                    if (i >= RegMap.MaxCoolerCount) break;
                    int index = Global.gConfigVM.CoolerPVs[i];
                    TConRegs[RegMap.TCon_GetCoolerPV + i].Read = TConRegs[RegMap.TCon_GetPV + index].Read;
                }


                for (int i = 0; i < 8; i++, ofs += 2) //16 Byte TCON-MV
                    TConRegs[RegMap.TCon_GetMV + i].Read = convStr(msg.data, ofs, 2);

                //Console.WriteLine($"TConRegs TCon_GetMV End {ofs}");

                for (int i = 0; i < 8; i++, ofs += 2) //16 Byte TCON-CH.STS
                    TConRegs[RegMap.TCon_GetCHSTS + i].Read = convStr(msg.data, ofs, 2);

                //Console.WriteLine($"TConRegs TCon_GetCHSTS End {ofs}");

                for (int i = 0; i < 8; i++, ofs += 2) //16 Byte TCON-ALM.STS
                    TConRegs[RegMap.TCon_GetALM + i].Read = convStr(msg.data, ofs, 2);

                //Console.WriteLine($"TConRegs TCon_GetALM End {ofs}");

                TConRegs[RegMap.TCon_GetRunST].Read = convStr(msg.data, (ofs++), 1); //1 Byte TCON-RUN

                //Console.WriteLine($"TConRegs TCon_GetRunST End {ofs}");
                //Console.WriteLine($"run : rd {TConRegs[RegMap.TCon_GetRunST].Read}, val {Convert.ToString((int)TConRegs[RegMap.TCon_GetRunST].ReadValue, 16)}");
                TConRegs[RegMap.TCon_GetTuneST].Read = convStr(msg.data, (ofs++), 1); //1 Byte TCON-TUNING

                //Console.WriteLine($"TConRegs TCon_GetTuneST End {ofs}");
                //Console.WriteLine($"tune : rd {TConRegs[RegMap.TCon_GetTuneST].Read}, val {Convert.ToString((int)TConRegs[RegMap.TCon_GetTuneST].ReadValue, 16)}");

                ////////////////////////////////////////////////////////////////
                ///// -> cooler 는 control tcon에서 설정하여 받는것으로..

                ofs += 16;

                //for (int i = 0; i < 8; i++, ofs += 2)
                //    TConRegs[RegMap.TCon_GetCoolerPV + i].Read = convStr(msg.data, ofs, 2);
                ////////////////////////////////////////////////////////////////


                //Console.WriteLine($"COOLER-N.PV End {ofs}");

                for (int i = 0; i < 13; i++, ofs += 1) // 13 Byte GPIO
                {
                    GPIORegs[i].Read = convStr(msg.data, ofs, 1);

                    if (i == RegMap.GPIO_SideClose)
                    {
                        if (RackStat != RACK_STAT.RUN || RackStat != RACK_STAT.PREV_RUN || RackStat != RACK_STAT.DEFROST)
                        {
                            if (GPIORegs[i].ReadValue == 0) // close
                            {
                                if (BoxColor != Global.gAlramColors[(int)ALRAM_TYPE.ERROR])
                                    ShowBox = System.Windows.Visibility.Hidden;
                            }
                            else // open
                            {
                                if (!((BoxColor == Global.gAlramColors[(int)ALRAM_TYPE.ERROR])
                                    && (ShowBox == Visibility.Visible)))
                                    showAlram(ALRAM_TYPE.WARN, "", "Side Open", isLogging: false);
                            }
                        }

                        SideOpenDetect = (int)GPIORegs[i].ReadValue;

                    }
                }
                //Console.WriteLine($"GPIORegs {ofs}");
                //for (int i = 0; i < 13; i++, ofs += 1) // 13 Byte GPIO
                //H.SENSOR HUM 2 Byte x 4 , 8 Byte
                for (int i = 0; i < 4; i++, ofs += 2)
                {
                    MOTORRegs[RegMap.HSENSOR_Hum + i].Read = convStr(msg.data, ofs, 2, true);
                }
                //H.SENSOR TEMP 2 Byte x 4 , 8 Byte
                for (int i = 0; i < 4; i++, ofs += 2)
                {
                    MOTORRegs[RegMap.HSENSOR_Temp + i].Read = convStr(msg.data, ofs, 2, true);
                }

                CheckHumidity();

                //Console.WriteLine($"MOTORRegs HSENSOR_Temp End {ofs}");
                //HI.LIMIT 1 Byte, LO.LIMIT 1 Byte, BOX.CONTACT 1 Byte, MOTOR.LOCK 1 Byte, MOTOR.EN 1 Byte, MOTOR.DIR 1 Byte, MOTOR.RUN 1 Byte
                MOTORRegs[RegMap.HI_Limit].Read = convStr(msg.data, (ofs++), 1);
                //Console.WriteLine($"MOTORRegs HI_Limit End {ofs}");
                //Motor Stat Check
                if (MOTORRegs[RegMap.HI_Limit].ReadValue == 0)
                {
                    if (MotorStat != MOTOR_STAT.CLOSING || MotorStat != MOTOR_STAT.OPENING)
                        MotorStat = MOTOR_STAT.CLOSE;
                }
                else if(MOTORRegs[RegMap.HI_Limit].ReadValue == 1)
                {
                    if (MotorStat != MOTOR_STAT.CLOSING || MotorStat != MOTOR_STAT.OPENING)
                        MotorStat = MOTOR_STAT.OPEN;
                }

                MOTORRegs[RegMap.LO_Limit].Read = convStr(msg.data, (ofs++), 1);
                MOTORRegs[RegMap.CBOX_Contact].Read = convStr(msg.data, (ofs++), 1);
                MOTORRegs[RegMap.MOTOR_Lock].Read = convStr(msg.data, (ofs++), 1);
                MOTORRegs[RegMap.MOTOR_En].Read = convStr(msg.data, (ofs++), 1);
                MOTORRegs[RegMap.MOTOR_Dir].Read = convStr(msg.data, (ofs++), 1);
                MOTORRegs[RegMap.MOTOR_Run].Read = convStr(msg.data, (ofs++), 1);
                //Console.WriteLine($"MOTORRegs MOTOR_Run End {ofs}");
                //MOTOR.STEP 4 Byte
                MOTORRegs[RegMap.MOTOR_Step].Read = convStr(msg.data, ofs, 4);
                //Console.WriteLine($"5555 {ofs}");
                checkGpio();

                //if (SSRCtrl == 0) SSRCtrl = (byte)((int)GPIORegs[RegMap.GPIO_SSRCtrl].ReadValue);

                if (Global.gMainVM.ShowEngineer == Visibility.Visible && Global.gEngineerVM.CurRack == RackIndex)
                    Global.gEngineerVM.setRack();

                //org
                processStat();
                checkSystem();

                //todo
                
                if (RackStat == RACK_STAT.RUN)
                {
                    processTest();
                    //todo
                    //sendFPMaxDimm();
                    int gindex = GraphIndex;
                    if (Global.gMainVM.ShowDimm == Visibility.Visible && Global.gTempVM.CurRack == RackIndex)
                        Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => Global.gTempVM.UpdateGraph(gindex, this)));
                }
                else
                {
                    //foreach (BoxModel bm in Boxs)
                    //    box.processBoxDimmInStop();
                    if (IsSaveOnlyTemp)
                    {
                        //foreach (BoxModel box in Boxs) box.processBoxDimm();
                        //MaxInRack.Clear();
                        double pv = 0;
                        foreach (TempDataModel cxldim in TempDatas)
                        {
                            if (pv < cxldim.Temp) pv = cxldim.Temp;
                            cxldim.PVData[GraphIndex] = cxldim.Temp;
                        }
                        GraphIndex++;
                        addTempLog();
                        int gindex = GraphIndex;
                        if (Global.gMainVM.ShowDimm == Visibility.Visible && Global.gTempVM.CurRack == RackIndex)
                            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => Global.gTempVM.UpdateGraph(gindex, this)));
                    }
                }

                if (Global.gMainVM.ShowBoard == Visibility.Visible && Global.gBoardVM.CurRack == RackIndex)
                {
                    Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => Global.gBoardVM.setRack(isRead: true)));
                }

            }
            else if (msg.sub.Func == 0x12)
            {
                if (msg.data.Length < 8) return;

                int read = msg.data[0] | (msg.data[1] << 8);
                if (read != 0)
                {
                    for (int i = 0; i < 5; i++)
                    {
                        int comp = 1 << i;
                        if (comp != 0)
                            LogHelper.Error($"[Prog][{Name}]", $"Fan I2C CH({i}) Error");
                    }
                    RackStat = RACK_STAT.INSPECTION;
                }

                read = msg.data[2] | (msg.data[3] << 8);
                if (read != 0)
                {
                    for (int i = 0; i < 11; i++)
                    {
                        int comp = 1 << i;
                        if (comp != 0)
                            LogHelper.Error($"[Prog][{Name}]", $"Cur I2C CH({i}) Error");
                    }
                    RackStat = RACK_STAT.INSPECTION;
                }

                read = msg.data[2] | (msg.data[3] << 8);
                if (read != 0)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        int comp = 1 << i;
                        if (comp != 0)
                            LogHelper.Error($"[Prog][{Name}]", $"Temp I2C CH({i}) Error");
                    }
                    RackStat = RACK_STAT.INSPECTION;
                }

                read = msg.data[2] | (msg.data[3] << 8);
                if (read != 0)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        int comp = 1 << i;
                        if (comp != 0)
                            LogHelper.Error($"[Prog][{Name}]", $"TCON RS485 I2C CH({i}) Error");
                    }
                    RackStat = RACK_STAT.INSPECTION;
                }

            }
        }
        public(bool isMatch, List<int> MismatchedPositions) CompareBitPattern(string sourceHex, string pattern)
        {
            int value = Convert.ToInt32(sourceHex, 16);
            int org  = Convert.ToInt32(pattern, 16);
            List<int> mismatchedPositions = new List<int>();

            // XOR와 AND 연산으로 비교 패턴과 관련된 불일치 비트만 찾기
            int relevantXorResult = (value ^ org) & org;
            for (int i = 0; i < 32; i++)
            {
                if (((relevantXorResult >> i) & 1) == 1)
                {
                    mismatchedPositions.Add(i);
                }
            }

            // 불일치 목록이 비어 있으면(Count == 0) 전체가 일치하는 것
            bool isMatch = mismatchedPositions.Count == 0;

            // bool 값과 리스트를 튜플로 묶어서 반환
            return (isMatch, mismatchedPositions);
        }
        public async Task SampleTest()
        {
            startTempLog();
            //Test code for 문
            for (int i = 0; i < 5000; i++)
            {
                processTest();

                
                int gindex = GraphIndex;
                if (Global.gMainVM.ShowDimm == Visibility.Visible && Global.gTempVM.CurRack == RackIndex)
                    await Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => Global.gTempVM.UpdateGraph(gindex, this)));

                await Task.Delay(1000);
            }
            endTempLog();
        }
        public void processTest()
        {
            if(RackStat == RACK_STAT.RUN)
            {
                foreach(BoxModel bm in Boxs)
                {
                    bm.processBoxCxl();
                }

                
                GraphIndex++;
                if(GraphIndex >= 1_000_000)
                    GraphIndex = 0;

                addTempLog();

                if (findEndFile())
                {
                    LogHelper.Info($"[Prog][{Name}]", $"Find EndFlag {Global.gConfigVM.TamsEndFile}");
                    foreach (BoxModel bm in Boxs)
                    {
                        bm.BoxDefrost = DEFROST_TYPE.TAMS;
                    }
                    PrestopTest();
                }
            }
        }
        public void doRecv(HtcMessage msg)
        {
            REG_TYPE type = MConv.FindEnumValue<REG_TYPE>((int)msg.Header.Type);

            StringBuilder sb = new StringBuilder();
            sb.Append($"T:{msg.Header.Type.ToString("X2")} " +
                $"L:{msg.Header.Len.ToString("X2")} " +
                $"R:{msg.Header.Res.ToString("X2")} <- ");

            if (msg.Header.Len != 0)
            {
                Array.ForEach(msg.data, num => sb.Append(num.ToString("X2") + ", "));
                sb.Append($"{msg.sub.Chanel.ToString("X2")} " +
                    $"{msg.sub.Adr.ToString("X2")} " +
                    $"{msg.sub.Func.ToString("X2")} " +
                    $"{msg.sub.Len.ToString("X2")} : ");
            }
            Console.WriteLine($"doRecv = {sb}");
            //Org
            /*
            if (type != REG_TYPE.PDO)
            {
                if (Global.gMainVM.ShowEngineer == Visibility.Visible && Global.gEngineerVM.CurRack == RackIndex)
                {
                    StringBuilder sb = new StringBuilder();
                    sb.Append($"T:{msg.Header.Type.ToString("X2")} " +
                        $"L:{msg.Header.Len.ToString("X2")} " +
                        $"R:{msg.Header.Res.ToString("X2")} <- ");

                    if (msg.Header.Len != 0)
                    {
                        Array.ForEach(msg.data, num => sb.Append(num.ToString("X2") + ", "));
                        sb.Append($"{msg.sub.Chanel.ToString("X2")} " +
                            $"{msg.sub.Adr.ToString("X2")} " +
                            $"{msg.sub.Func.ToString("X2")} " +
                            $"{msg.sub.Len.ToString("X2")} : ");
                    }
                    CMD_TYPE res = CMD_TYPE.RECV;
                    if (msg.Header.Res == 0xe) res = CMD_TYPE.ERROR;

                    Console.WriteLine($"doRecv = {sb}");
                    Global.gEngineerVM.pushCmdLog(new CommandLog(RackIndex, res, sb.ToString()));
                }
                else if (Global.gMainVM.ShowConfig == Visibility.Visible && Global.gConfigVM.CurRack == RackIndex)
                {
                    if (_client._isPushing)
                    {
                        CMD_TYPE res = CMD_TYPE.RECV;
                        string text = "Write";
                        if (msg.Header.Res == 0xe)
                        {
                            res = CMD_TYPE.ERROR;
                            text = "Error";
                        }

                        if (msg.data != null)
                        {
                            if (msg.data.Length >= 2)
                            {
                                text = "Read <- ";
                                text += doValueTconTemp(convStr(msg.data, 0, 2, true)).ToString();
                            }
                        }

                        Global.gConfigVM.pushTconResp(new CommandLog(RackIndex, res, text));
                    }
                }
            }
            */
            if (msg.Header.Len == 0)
            {
                if (msg.Header.Res == 0xe)
                {
                    //Console.WriteLine($"Packet [{type.ToString()}] Reponse Error!");

                }

                if (type == REG_TYPE.PDO)
                {
                    if (Global.gConfigVM.SendFlag == FPGA_FLAG.SEND)
                    {
                        if (msg.Header.Res == 0xe)
                            Global.gConfigVM.SendFlag = FPGA_FLAG.ERROR;
                        else
                            Global.gConfigVM.SendFlag = FPGA_FLAG.OK;
                    }
                }
                return; // Recv OK
            }

            try
            {

                switch (type)
                {
                    case REG_TYPE.FAN: recvFan(msg); break;
                    case REG_TYPE.CUR: recvCur(msg); break;
                    case REG_TYPE.TEMP: break;
                    case REG_TYPE.PDO: recvPDO(msg); break;


                }
            }
            catch (Exception e)
            {
#if TEST_MODE
                Console.WriteLine(e.Message);
#endif
                LogHelper.Error($"[Prog][{Name}]", $"RECV Msg {e.Message}");
            }
        }
        private DispatcherTimer _configCheckTimer;
        public void connected(bool isCon)
        {
            if (isCon)
            {
                // gConfigVM이 아직 null이라면, 타이머를 설정하여 확인 시작
                if (Global.gConfigVM == null)
                {
                    _configCheckTimer = new DispatcherTimer();
                    _configCheckTimer.Interval = TimeSpan.FromMilliseconds(200); // 0.2초마다 확인
                    _configCheckTimer.Tick += ConfigCheckTimer_Tick; // 타이머가 실행할 메서드 연결
                    _configCheckTimer.Start();
                }
                else
                {
                    // gConfigVM이 이미 존재하면, 즉시 초기화 로직 실행
                    InitializeAfterConnected();
                }
            }
            else
            {
                RackStat = RACK_STAT.DISCONNECT;
                MotorStat = MOTOR_STAT.NID;
                ShowBox = System.Windows.Visibility.Hidden;
                foreach (BoxModel bm in Boxs)
                {
                    bm.BoxStat = BOX_STAT.DISCONNECT;
                    foreach (SiteModel sm in bm.Sites)
                        sm.SiteStat = SITE_STAT.DISCONNECT;
                }
            }
        }
        /*
        if (isCon)
        {
            RackStat = RACK_STAT.STOP;
            foreach (BoxModel bm in Boxs)
            {
                bm.BoxStat = BOX_STAT.STOP;
                foreach (SiteModel sm in bm.Sites)
                    sm.SiteStat = SITE_STAT.STOP;
            }
            // 연결 후 초기설정
            //HI/LO Limit Polarity, CBox Contact Polarity, Motor Direction Polarity, Motor Stepping Freq, Motor Steping Max Count



            HtcMessage msg = Global.newMsg(0xff, 0, 0x12, 8);
            _client.pushSendQue(REG_TYPE.PDO, msg);
#if TEST_MODE
            Console.WriteLine($"{Name}: Connected!");
#endif
            reqFanDuty();


            msg = Global.newMsg(0xff, 0, 0x21);

            //todo Dimm count 어떻게 할지 결정해야됌 dimm temp 사용안해서
            short dimCnt = (short)Global.gMainPageVM.MaxFixDimmCount;
            msg.data = BitConverter.GetBytes(dimCnt);
            _client.pushSendQue(REG_TYPE.DIMM, msg);


            msg = Global.newMsg(0xff, 0, 0x21);
            byte ssrCold = Convert.ToByte(Convert.ToInt32(Global.gConfigVM.SSRCold, 16));
            msg.data = BitConverter.GetBytes(ssrCold);
            _client.pushSendQue(REG_TYPE.DIMM, msg);

            sendFPSV(SetSV);
            sendFPMap(
              Boxs[0].Sites[0].FPIndex,
              Boxs[0].Sites[1].FPIndex,
              Boxs[1].Sites[0].FPIndex,
              Boxs[1].Sites[1].FPIndex);

        }
        else
        {
            RackStat = RACK_STAT.DISCONNECT;
            foreach (BoxModel bm in Boxs)
            {
                bm.BoxStat = BOX_STAT.DISCONNECT;
                foreach (SiteModel sm in bm.Sites)
                    sm.SiteStat = SITE_STAT.DISCONNECT;
            }
        }
        */
        private void InitializeAfterConnected()
        {
            try
            {
                RackStat = RACK_STAT.STOP;
                foreach (BoxModel bm in Boxs)
                {
                    bm.BoxStat = BOX_STAT.STOP;
                    foreach (SiteModel sm in bm.Sites)
                        sm.SiteStat = SITE_STAT.STOP;
                }

                // 이제 이 코드는 Global.gConfigVM이 null이 아님이 보장된 상태에서,
                // 그리고 UI 스레드에서 안전하게 실행됩니다.
                HtcMessage msg = Global.newMsg(0xff, 0, 0x12, 8);
                _client.pushSendQue(REG_TYPE.PDO, msg);
#if TEST_MODE
        Console.WriteLine($"{Name}: Connected!");
#endif
                reqMotorSet();
                reqFanDuty();

                msg = Global.newMsg(0xff, 0, 0x21);
                short dimCnt = (short)Global.gMainPageVM.MaxFixDimmCount;
                msg.data = BitConverter.GetBytes(dimCnt);
                _client.pushSendQue(REG_TYPE.DIMM, msg);

                msg = Global.newMsg(0xff, 0, 0x21);
                byte ssrCold = Convert.ToByte(Convert.ToInt32(Global.gConfigVM.SSRCold, 16));
                msg.data = BitConverter.GetBytes(ssrCold);
                _client.pushSendQue(REG_TYPE.DIMM, msg);

                sendFPSV(SetSV);
                sendFPMap(
                  Boxs[0].Sites[0].FPIndex,
                  Boxs[0].Sites[1].FPIndex,
                  Boxs[1].Sites[0].FPIndex,
                  Boxs[1].Sites[1].FPIndex);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"연결 후 초기화 중 오류 발생: {ex.Message}");
            }
        }
        // DispatcherTimer가 0.2초마다 UI 스레드에서 실행하는 메서드
        private void ConfigCheckTimer_Tick(object sender, EventArgs e)
        {
            // gConfigVM이 null이 아닌지 확인
            if (Global.gConfigVM != null)
            {
                // 조건이 만족되었으므로 타이머를 멈춤
                _configCheckTimer.Stop();
                _configCheckTimer.Tick -= ConfigCheckTimer_Tick; // 이벤트 핸들러 정리
                _configCheckTimer = null;

                // 실제 연결 후 초기화 로직 실행
                InitializeAfterConnected();
            }
        }
        public bool canExcutePower(object obj)
        {
            if (PowerStat == POWER_STAT.WAIT_RECV ||
                PowerStat == POWER_STAT.PLC_OFF) return false;


            return true;
        }
        public void clickedPower()
        {
            if (PowerStat == POWER_STAT.WAIT_RECV ||
                PowerStat == POWER_STAT.PLC_OFF) return;


            if (PowerStat == POWER_STAT.POWER_OFF)
            {
                PowerStat = POWER_STAT.WAIT_RECV;
                Global.gMainVM.PlcCtl.setSlotPower(RackIndex, true);
                LogHelper.Info($"[Prog][{Name}]", "Power On");
            }
            else if (PowerStat == POWER_STAT.POWER_ON)
            {
                MessageBoxResult res = HandyControl.Controls.MessageBox.Show($"Do You Want to Shut Down Rack{RackIndex + 1}?", "Power Off", MessageBoxButton.YesNo, MessageBoxImage.Exclamation);
                if (res == MessageBoxResult.No) return;

                PowerStat = POWER_STAT.WAIT_RECV;
                Global.gMainVM.PlcCtl.setSlotPower(RackIndex, false);
                LogHelper.Info($"[Prog][{Name}]", "Power Off");
            }
            else if (PowerStat == POWER_STAT.MIXED)
            {
                PowerStat = POWER_STAT.WAIT_RECV;
                Global.gMainVM.PlcCtl.setSlotPower(RackIndex, true);

            }
        }

        public void clickedAllCmdPower(bool onoff)
        {
            if (onoff)
            {
                if (PowerStat == POWER_STAT.POWER_OFF)
                {
                    PowerStat = POWER_STAT.WAIT_RECV;
                    Global.gMainVM.PlcCtl.setSlotPower(RackIndex, true);

                }
            }
            else
            {
                if (PowerStat == POWER_STAT.POWER_ON)
                {
                    PowerStat = POWER_STAT.WAIT_RECV;
                    Global.gMainVM.PlcCtl.setSlotPower(RackIndex, false);
                }
            }

        }
        public void SampleprocessStat()
        {
            int runSum = 0;
            int stopSum = 0;
            int errSum = 0;

            RACK_STAT curStat = RackStat;

            foreach (BoxModel box in Boxs)
            {
                box.SampleprocessBoxST();


                if (box.BoxStat == BOX_STAT.RUN)
                    runSum++;
                else if (box.BoxStat == BOX_STAT.STOP)
                    stopSum++;
                //else if (box.BoxStat == BOX_STAT.TUNNING)
                //    tuneSum++;
                else if (box.BoxStat == BOX_STAT.ERROR)
                    errSum++;
            }

            if (errSum > 0)
            {
                RackStat = RACK_STAT.ERROR;
                LogHelper.Error($"[Prog][{Name}]", "TCon Error. Check in Engineer Page");
                return;
            }

            if (runSum > 0)
            {
                if (curStat != RACK_STAT.RUN || curStat != RACK_STAT.DEFROST)
                    curStat = RACK_STAT.RUN;
            }
            //else if (tuneSum > 0)
            //    curStat = RACK_STAT.TUNNING;
            else if (stopSum == Boxs.Count)
                curStat = RACK_STAT.STOP;

            RackStat = curStat;

            // 처음 접속시 run 상태이면 TCon Recv SV로 SV Setting
            if (PrevSV == 0 && RackStat == RACK_STAT.RUN && MotorStat == MOTOR_STAT.CLOSE)
            {
                foreach (BoxModel bm in Boxs)
                {
                    bm.BoxStat = BOX_STAT.RUN;
                    foreach (SiteModel sm in bm.Sites)
                        sm.SiteStat = SITE_STAT.RUN;
                }

                PrevSV = SetSV;
                sendFPDisplay(0);
            }
            //처음 접속시 Tcon Run 인데 모터 업이면 정지상태로
            else if (PrevSV == 0 && RackStat == RACK_STAT.RUN && MotorStat == MOTOR_STAT.OPEN)
            {
                foreach (BoxModel bm in Boxs)
                {
                    bm.sendStop();
                    bm.sendFan(50);
                }

                clearSSR();

                Thread.Sleep(1000);
                Global.gMainVM.PlcCtl.setSlotMC(RackIndex, false);

                RackStat = RACK_STAT.STOP;
                foreach(BoxModel bm in Boxs)
                {
                    bm.BoxStat = BOX_STAT.STOP;
                    foreach (SiteModel sm in bm.Sites)
                        sm.SiteStat = SITE_STAT.STOP;
                }    
                PrevSV = SetSV;
                sendFPDisplay(0);
            }
        }
        public void processStat()
        {
            int runSum = 0;
            int stopSum = 0;
            //int tuneSum = 0;
            int errSum = 0;

            RACK_STAT curStat = RackStat;
            foreach (BoxModel box in Boxs)
            {
                box.processBoxST();


                if (box.BoxStat == BOX_STAT.RUN)
                    runSum++;
                else if (box.BoxStat == BOX_STAT.STOP)
                    stopSum++;
                //else if (box.BoxStat == BOX_STAT.TUNNING)
                //    tuneSum++;
                else if (box.BoxStat == BOX_STAT.ERROR)
                    errSum++;
            }

            if (errSum > 0)
            {
                RackStat = RACK_STAT.ERROR;
                LogHelper.Error($"[Prog][{Name}]", "TCon Error. Check in Engineer Page");
                return;
            }

            if (runSum > 0)
            {
                if(curStat != RACK_STAT.RUN || curStat != RACK_STAT.DEFROST)
                    curStat = RACK_STAT.RUN;
            }
            //else if (tuneSum > 0)
            //    curStat = RACK_STAT.TUNNING;
            else if (stopSum == Boxs.Count)
                curStat = RACK_STAT.STOP;

            //todo
            /*
            if (RackStat == RACK_STAT.PREV_RUN && curStat != RACK_STAT.RUN)
            {
                if (TimeStart.AddSeconds(8) < DateTime.Now)
                {
                    RackStat = RACK_STAT.ERROR;
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{Name}] : Can't Start. CBox Is Not Change Run");
                    return;
                }
            }
            else if (RackStat == RACK_STAT.PREV_STOP && curStat != RACK_STAT.STOP)
            {
                if (TimeEnd.AddSeconds(8) < DateTime.Now)
                {
                    RackStat = RACK_STAT.ERROR;
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{Name}] : Can't Start. CBox Is Not Change STOP");
                    return;
                }
            }
            */
            RackStat = curStat;

            // 처음 접속시 run 상태이면 TCon Recv SV로 SV Setting
            if (PrevSV == 0 && RackStat == RACK_STAT.RUN && MotorStat == MOTOR_STAT.CLOSE)
            {
                foreach (BoxModel bm in Boxs)
                {
                    bm.BoxStat = BOX_STAT.RUN;
                    foreach (SiteModel sm in bm.Sites)
                        sm.SiteStat = SITE_STAT.RUN;
                    //foreach (SiteModel sm in bm.Sites)
                    //{
                    //    int index = sm.SVCtrlList[0];
                    //    sm.TargetSV = TConRegs[RegMap.TCon_GetSV + index].ReadValue;
                    //    box.SVST(sub, SVC.STABLE);
                    //    sub.SampleLongTerm.Clear();
                    //    sub.SampleShortTerm.Clear();
                    //    sub.BSVAlue = 0;
                    //}
                }
                
                PrevSV = SetSV;
                sendFPDisplay(0);
            }
            //처음 접속시 Tcon Run 인데 모터 업이면 정지상태로
            else if(PrevSV == 0 && RackStat == RACK_STAT.RUN && MotorStat == MOTOR_STAT.OPEN)
            {
                foreach(BoxModel bm in Boxs)
                {
                    bm.sendStop();
                    bm.sendFan(50);
                }

                clearSSR();

                Thread.Sleep(1000);
                Global.gMainVM.PlcCtl.setSlotMC(RackIndex, false);

                RackStat = RACK_STAT.STOP;
                PrevSV = SetSV;
                sendFPDisplay(0);
            }
        }
        public void checkGpioSub(RegModel reg, int value)
        {
            if ((int)reg.ReadValue != value)
            {
                int func = Convert.ToInt32(reg.FUNC, 16);
                HtcMessage msg = Global.newMsg(0xff, 0, func);
                msg.data = new byte[1];
                msg.data[0] = (byte)((int)value & 0xff);
                _client.pushSendQue(REG_TYPE.GPIO, msg);
            }
        }
        public void checkGpio()
        {
            int value = Convert.ToInt32(Global.gConfigVM.SSRIn, 16);
            checkGpioSub(GPIORegs[1], value);

            value = Convert.ToInt32(Global.gConfigVM.SSROut, 16);
            checkGpioSub(GPIORegs[2], value);

            value = Convert.ToInt32(Global.gConfigVM.SSRCH, 16);
            checkGpioSub(GPIORegs[3], value);

            value = Convert.ToInt32(Global.gConfigVM.SSREN, 16);
            checkGpioSub(GPIORegs[4], value);

            if (IsAS || (Convert.ToInt32(Global.gConfigVM.ExtraFan, 16) == 0))
                value = 0;
            else if (!IsAS && (Convert.ToInt32(Global.gConfigVM.ExtraFan, 16) == 1))
                value = 1;

            checkGpioSub(GPIORegs[5], value);

            value = Convert.ToInt32(Global.gConfigVM.ExtraPump, 16);
            checkGpioSub(GPIORegs[6], value);

            value = Convert.ToInt32(Global.gConfigVM.ExtraRad, 16);
            checkGpioSub(GPIORegs[7], value);

            value = Convert.ToInt32(Global.gConfigVM.SSRFreq, 16);
            checkGpioSub(GPIORegs[11], value);

            value = Convert.ToInt32(Global.gConfigVM.SSRCold, 16);
            checkGpioSub(GPIORegs[12], value);
        }
        public void CheckHumidity()
        {
            //H.SENSOR HUM 2 Byte x 4 , 8 Byte
            //for (int i = 0; i < 4; ofs += 1)
            //{
            //    MOTORRegs[RegMap.HSENSOR_Hum + i].Read = convStr(msg.data, ofs, 2);
            //}
            foreach(BoxModel bm in Boxs)
            {
                bm.SetHumidityValue(MOTORRegs[RegMap.HSENSOR_Hum + bm.BoxIndex].ReadValue);
                bm.HumTempSensorValue = MOTORRegs[RegMap.HSENSOR_Temp + bm.BoxIndex].ReadValue;
            }
        }
        bool _prevFETWarn = false;
        public void checkSystem()
        {
            if (RackStat == RACK_STAT.RUN || RackStat == RACK_STAT.PREV_RUN)
            {
                /* //RUN , PREV_RUN , DEFROST 일때는 Side Door Check 안하게
                //  Side Door Check 0 = Close , 1 = Open
                if (GPIORegs[RegMap.GPIO_SideClose].ReadValue == 1)
                {
                    showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]", "Side Door Open");
                    RackStat = RACK_STAT.ERROR;
                    return;
                }
                */
                //Board Con Check 0 = Contact , 1 = Discontact
                if (GPIORegs[RegMap.GPIO_BDCon].ReadValue == 1)
                {
                    showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]", "Board Contact Fail");
                    RackStat = RACK_STAT.ERROR;
                    return;
                }
                string msg = "";
                // Water
                int end = RegMap.TCon_GetCoolerPV + RegMap.MaxCoolerCount;
                for (int i = RegMap.TCon_GetCoolerPV; i < end; i++)
                {
                    double waterTemp = TConRegs[i].ReadValue;

                    if (waterTemp > 120) // 120도 이상은 노이즈 
                    {
                        LogHelper.Warn($"[Prog][{Name}]", $"Water Jacket Temp Noise Limit:{Global.gConfigVM.WaterOver}, Read:{waterTemp}");
                    }
                    else if (waterTemp > Global.gConfigVM.WaterOver)
                    {
                        showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]",
                            $"Cooler{i - RegMap.TCon_GetCoolerPV + 1}(1Base) Water Jacket Temp Error. " +
                            $"Limit : {Global.gConfigVM.WaterOver}°C, Read : {TConRegs[i].ReadValue}°C");
                        RackStat = RACK_STAT.ERROR;
                    }
                }


                // Fet-Temp
                int typeErr = 0;
                end = RegMap.Temp_GetTemp + 8;
                for (int i = RegMap.Temp_GetTemp; i < end; i++)
                {

                    if (TempRegs[i].ReadValue > Global.gConfigVM.FetTempWarn && _prevFETWarn == false)
                    {
                        _prevFETWarn = true;
                        LogHelper.Warn($"[Prog][{Name}]", "HeatThink Temp Warning Unit");
                    }
                    else if (TempRegs[i].ReadValue < Global.gConfigVM.FetTempWarn && _prevFETWarn == true)
                    {
                        _prevFETWarn = false;

                    }

                    typeErr = 0;
                    if (TempRegs[i].ReadValue < Global.gConfigVM.FetTempUnder) // Error
                    {
                        msg = "FET Under Temp Error";
                        typeErr = 1;
                    }
                    else if (TempRegs[i].ReadValue > Global.gConfigVM.FetTempOver) // Error
                    {
                        msg = "FET Over Temp Error";
                        typeErr = 2;
                    }


                    if (typeErr > 0)
                    {
                        showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]",
                            $"FET{i - RegMap.Temp_GetTemp + 1}(1Base) {msg} " +
                            $"{(typeErr == 1 ? "Under" : "over")} " +
                            $"Limit : {(typeErr == 1 ? Global.gConfigVM.FetTempUnder : Global.gConfigVM.FetTempOver)}°C, " +
                            $"Read : {TempRegs[i].ReadValue}°C");

                        RackStat = RACK_STAT.ERROR;
                    }
                }

                //HeatSink Block

                // FAN
                int[] fanRpmCheckMap = { 0, 2, 4, 6, 9, 10, 11, 12, 13, 14 };


                typeErr = 0;
                end = RegMap.Fan_GetTach + 15;
                for (int i = 0; i < fanRpmCheckMap.Length; i++)
                {
                    int index = RegMap.Fan_GetTach + fanRpmCheckMap[i];
                    typeErr = 0;
                    if (FanRegs[index].ReadValue < Global.gConfigVM.FanRpmUnder) // Error
                    {
                        msg = $"Fan[{i}] Under RPM Error";
                        typeErr = 1;
                    }
                    else if (FanRegs[index].ReadValue > Global.gConfigVM.FanRpmOver) // Error
                    {
                        msg = $"Fan[{i}] Over RPM Error";
                        typeErr = 2;
                    }

                    if (typeErr > 0)
                    {
                        showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]",
                            msg +
                            $"{(typeErr == 1 ? "Under" : "over")} " +
                            $"Limit : {(typeErr == 1 ? Global.gConfigVM.FanRpmUnder : Global.gConfigVM.FanRpmOver)}, Read : {FanRegs[index].ReadValue}");
                        RackStat = RACK_STAT.ERROR;
                    }


                }

                // Cur

                //end = RegMap.Cur_GetCur + 11;
                int tempend = RegMap.Cur_GetCur + 8;
                int waterend = tempend + 1;

                //tempbox cur 
                for (int i = RegMap.Cur_GetCur; i < tempend; i++)
                {
                    if (CurRegs[i].ReadValue > Global.gConfigVM.PaltierCurOver)
                    {
                        showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]",
                            $"Paltier{i - RegMap.Cur_GetCur + 1}(1Base) Current Over Error " +
                            $"Limit : {Global.gConfigVM.PaltierCurOver}A, Read : {FanRegs[i].ReadValue}A");
                        RackStat = RACK_STAT.ERROR;
                    }
                }

                if (Global.gConfigVM.ExtraPump == "1")
                {
                    //Waterpump cur
                    if (CurRegs[tempend].ReadValue > Global.gConfigVM.WaterpumpCurOver)
                    {
                        showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]",
                                $"[Water Pump] Current Over Error " +
                                $"Limit : {Global.gConfigVM.WaterpumpCurOver}A, Read : {CurRegs[tempend].ReadValue}A");
                        RackStat = RACK_STAT.ERROR;
                    }
                    else if (CurRegs[tempend].ReadValue < Global.gConfigVM.WaterpumpCurUnder)
                    {
                        showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]",
                            $"[Water Pump] Current Under Error " +
                                $"Limit : {Global.gConfigVM.WaterpumpCurUnder}A, Read : {CurRegs[tempend].ReadValue}A");
                        RackStat = RACK_STAT.ERROR;
                    }
                }

                if (Global.gConfigVM.ExtraRad == "1")
                {
                    //Radiator Fan
                    if (CurRegs[waterend].ReadValue > Global.gConfigVM.RadiatorfanCurOver)
                    {
                        showAlram(ALRAM_TYPE.ERROR, $"[Prog][ {Name} ]",
                                $"[Radiator Fan] Current Over Error " +
                                $"Limit : {Global.gConfigVM.RadiatorfanCurOver}A, Read : {CurRegs[waterend].ReadValue}A");
                        RackStat = RACK_STAT.ERROR;
                    }
                    else if (CurRegs[waterend].ReadValue < Global.gConfigVM.RadiatorfanCurUnder)
                    {
                        showAlram(ALRAM_TYPE.ERROR, $"[Prog][ {Name} ]",
                                $"[Radiator Fan] Current Under Error " +
                                $"Limit : {Global.gConfigVM.RadiatorfanCurUnder}A, Read : {CurRegs[waterend].ReadValue}A");
                        RackStat = RACK_STAT.ERROR;
                    }
                }

            }
        }
        public bool setTamsTemp(bool onlyFileFind)
        {
            try
            {
                string fn = $"{Global.gConfigVM.TamsSVPath}\\" +
                    $"{Global.gConfigVM.TamsSVFolder}{RackIndex + 1}\\" +
                    $"{Global.gConfigVM.TamsSVFile}";
                FileInfo info = new FileInfo(fn);

                if (!info.Exists) return false;
                if (onlyFileFind) return true;


                string[] lines = File.ReadAllLines(@info.FullName);
                foreach (string line in lines)
                {
                    if (line.Contains("TEMP"))
                    {
                        string[] buf = line.Split(':');
                        if (!buf[1].Equals(""))
                        {
                            SetSV = double.Parse(buf[1]);
                            return true;
                        }

                    }
                }
            }
            catch (Exception e)
            {
                LogHelper.Warn($"[Prog][{Name}]", "Tams File Find Error Ocurred");
            }
            return false;
        }
        public bool findEndFile()
        {
            try
            {
                string fn = $"{Global.gConfigVM.TamsEndPath}\\" +
                    $"{Global.gConfigVM.TamsEndFolder}{RackIndex + 1}\\" +
                    $"{Global.gConfigVM.TamsEndFile}";

                FileInfo finfo = new FileInfo(fn);

                if (finfo.Exists)
                    return true;
            }
            catch (Exception e)
            {
                LogHelper.Warn($"[Prog][{Name}]", "End File Find Error Ocurred");
            }

            return false;
        }

        #endregion

        #region Read Value Change Delegate Functions
        public double doValueFanGetDuty(string val)
        {
            byte data = Convert.ToByte(val, 16);
            double x = Convert.ToDouble(data);
            return Math.Round((x / 256) * 100, 1);
        }

        public double doValueFanGetTach(string val)
        {
            int data = (Convert.ToInt16(val, 16) >> 3) & 0x1fff;
            if (data == 0) return 0;
            return Math.Round((double)(3932160 * 2) / data);
        }

        public double doValueGetCurrentBox(string val)
        {
            short data = Convert.ToInt16(val, 16);
            double value = (data * 0.0625 * 2 - 2500) / 44.4;
            //double value = data * 0.000916; 
            //double value = data * 0.001; // 30A CurrentLSB            
            return Math.Round(value, 1);
        }

        public double doValueGetCurrentEtc(string val)
        {
            short data = Convert.ToInt16(val, 16);
            double value = data * 0.0005;
            //double value = data * 0.000305; // 30A CurrentLSB            
            return Math.Round(value, 1);

        }

        public double doValueGetTemp(string val)
        {
            int data = (int)(Convert.ToInt16(val, 16) & 0x7fff);
            return Math.Round((data / 16) * 0.0625, 1);
        }

        //        Random rand = new Random();
        public double doValueGetDim(string val)
        {
            //return rand.Next(0, 2);
            int data = Convert.ToInt32(val, 16);
            int value = data & 0xFFF;
            if ((data & 0x1000) != 0)
                return Math.Round((0xFFFFF000 + value) * 0.0625, 1);
            return Math.Round(value * 0.0625, 1);
        }

        public double doValueTconTemp(string val)
        {
            short value = Convert.ToInt16(val, 16);
            double ret = value / 10.0;
            return ret;
        }

        public double doValueDecimal(string val)
        {
            return Convert.ToInt16(val, 16);
        }
        public double doValueDecimal32(string val)
        {
            return Convert.ToInt32(val, 16);
        }
        #endregion
        public string convStr(byte[] buf, int offset, int len, bool isLSB = false)
        {
            string ret = "";

            if (isLSB) 
            {
                for (int i = len - 1; i >= 0; i--)
                    ret += (buf[offset + i]).ToString("X2");
            }
            else
            {
                for (int i = 0; i < len; i++)
                    ret += (buf[offset + i]).ToString("X2");
            }
            return ret;
        }
        #region Send Command 
        public void sendFanDuty(int index, int percent) // 0~7 TempBox, 8~14 ETC
        {
            int value = (percent * 256) / 100;
            if (value > 0xff) value = 0xff;
            HtcMessage msg = Global.newMsg(0xff, index, 0x10);
            msg.data = new byte[1];
            msg.data[0] = (byte)(value & 0xff);
            _client.pushSendQue(REG_TYPE.FAN, msg);
        }


        [DoNotNotify]
        public int RackSSRValue { get; set; } = 0;
        public void addSSR(int data)
        {
            RackSSRValue |= data;
            HtcMessage msg = Global.newMsg(0xff, 0, 0x10);
            msg.data = new byte[1];
            msg.data[0] = (byte)(RackSSRValue & 0xff);

            _client.pushSendQue(REG_TYPE.GPIO, msg);
        }

        public void removeSSR(int data)
        {
            RackSSRValue &= ~data;
            HtcMessage msg = Global.newMsg(0xff, 0, 0x10);
            msg.data = new byte[1];
            msg.data[0] = (byte)(RackSSRValue & 0xff);

            _client.pushSendQue(REG_TYPE.GPIO, msg);
        }

        public void clearSSR()
        {
            RackSSRValue = 0;
            addSSR(0);
        }

        public void reqMotorSet()
        {
            if (Global.gEngineerVM == null) return;

            //static public HtcMessage newMsg(int ch, int adr, int func, int Len = 0) // if Len is Not 0, It is Request Byte Count
            //HI/LO LIMIT POLARITY Set
            HtcMessage msg = Global.newMsg(0xff, 0, 0x40, 1);
            msg.data = new byte[1];
            msg.data[0] = Convert.ToByte(Global.gConfigVM.PolarityLimit, 16);
            
            _client.pushSendQue(REG_TYPE.PDO, msg);
            //CBOX CONTACT POLARITY Set
            msg = Global.newMsg(0xff, 0, 0x41, 1);
            msg.data = new byte[1];
            msg.data[0] = Convert.ToByte(Global.gConfigVM.PolarityContact, 16);

            _client.pushSendQue(REG_TYPE.PDO, msg);
            //MOTOR DIRECTION POLARITY Set
            msg = Global.newMsg(0xff, 0, 0x42, 1);
            msg.data = new byte[1];
            msg.data[0] = Convert.ToByte(Global.gConfigVM.PolarityDirection, 16);

            _client.pushSendQue(REG_TYPE.PDO, msg);
            //MOTOR STEPPING FREQ Set
            msg = Global.newMsg(0xff, 0, 0x43, 4);
            msg.data = new byte[4];
            int freqInt;
            if(int.TryParse(Global.gConfigVM.SteppingFreq, out freqInt))
            {
                msg.data = BitConverter.GetBytes(freqInt);

                //빅 엔디안(Big-Endian) 겨우
                //if (BitConverter.IsLittleEndian)
                //{
                //    Array.Reverse(msg.data);
                //}
            }

            _client.pushSendQue(REG_TYPE.PDO, msg);
            //MOTOR STEPPING MAX COUNT Set
            msg = Global.newMsg(0xff, 0, 0x44, 4);
            msg.data = new byte[4];
            
            if (int.TryParse(Global.gConfigVM.SteppingMaxCount, out freqInt))
            {
                msg.data = BitConverter.GetBytes(freqInt);

                //빅 엔디안(Big-Endian) 겨우
                //if (BitConverter.IsLittleEndian)
                //{
                //    Array.Reverse(msg.data);
                //}
            }

            _client.pushSendQue(REG_TYPE.PDO, msg);
        }

        public void reqFanDuty()
        {
            if (Global.gEngineerVM == null) return;
            var msg = Global.newMsg(0xff, 0, 0x11, 15);
            int index = Global.gEngineerVM.CurRack;

            Global.gMainPageVM.Racks[index]._client.pushSendQue(REG_TYPE.FAN, msg);
        }

        public void reqTConBS1()
        {
            var msg = Global.newMsg(0xff, 0, 0x31, 2);
            int index = Global.gEngineerVM.CurRack;

            Global.gMainPageVM.Racks[index]._client.pushSendQue(REG_TYPE.TCON, msg);
        }

        public void sendExtFan(byte data) // same etc power on / off
        {
            HtcMessage msg = Global.newMsg(0xff, 0, 0x15);
            msg.data = new byte[1];
            msg.data[0] = data;

            _client.pushSendQue(REG_TYPE.GPIO, msg);
        }
        public void sendPump(byte data)
        {
            HtcMessage msg = Global.newMsg(0xff, 0, 0x16);
            msg.data = new byte[1];
            msg.data[0] = data;

            _client.pushSendQue(REG_TYPE.GPIO, msg);
        }

        public void sendadFan(byte data)
        {
            HtcMessage msg = Global.newMsg(0xff, 0, 0x17);
            msg.data = new byte[1];
            msg.data[0] = data;

            _client.pushSendQue(REG_TYPE.GPIO, msg);
        }

        public void sendFPDisplay(int mode) // 0 = pv, 1 = max
        {
            HtcMessage msg = Global.newMsg(0xff, 0, 0x20);
            msg.data = new byte[1];
            msg.data[0] = (byte)mode;

            _client.pushSendQue(REG_TYPE.FP, msg);

        }
        public void sendMotorCtrl(byte data) // 0 - STOP, 1 - UP, 2 - DOWN
        {
            HtcMessage msg = Global.newMsg(0xff, 0, 0x50);
            msg.data = new byte[1];
            msg.data[0] = data;

            _client.pushSendQue(REG_TYPE.PDO, msg);
        }
        public void sendMotorManualUp(uint data)
        {
            HtcMessage msg = Global.newMsg(0xff, 0, 0x51, 4);
            msg.data = BitConverter.GetBytes(data);

            _client.pushSendQue(REG_TYPE.PDO, msg);
        }
        public void sendMotorManualDown(uint data)
        {
            HtcMessage msg = Global.newMsg(0xff, 0, 0x52, 4);
            msg.data = BitConverter.GetBytes(data);

            _client.pushSendQue(REG_TYPE.PDO, msg);
        }

        public void sendFPMaxDimm()
        {
            if (Global.gConfigVM.FPDisplay != 1) return;

            //todo fp 에 data 어떻게 할지 결정 필요
            /*
            HtcMessage msg = Global.newMsg(0xff, 0, 0x21, 12);
            msg.data = new byte[12];
            int idx = 0;
            foreach (var box in Boxs)
            {
                msg.data[idx++] = 0;                        // 0    
                msg.data[idx++] = (byte)(box.MaxInBoxValue / 10);          // 10
                msg.data[idx++] = (byte)(box.MaxInBoxValue % 10);          // 1
                msg.data[idx++] = (byte)((box.MaxInBoxValue * 10) % 10);   // 0.1
                if (idx >= 12) break;
            }
            _client.pushSendQue(REG_TYPE.FP, msg);
            */
        }
        
        public void sendFPSV(double sv)
        {
            HtcMessage msg = Global.newMsg(0xff, 0, 0x10, 4);
            msg.data = new byte[4];
            msg.data[0] = 0;                        // 0    
            msg.data[1] = (byte)(sv / 10);          // 10
            msg.data[2] = (byte)(sv % 10);          // 1
            msg.data[3] = (byte)((sv * 10) % 10);   // 0.1

            _client.pushSendQue(REG_TYPE.FP, msg);

        }

        public void sendFPMap(int b1, int b2, int b3, int b4)
        {

            HtcMessage msg = Global.newMsg(0xff, 0, 0x11, 4);
            msg.data = new byte[4];
            msg.data[0] = (byte)b1;
            msg.data[1] = (byte)b2;
            msg.data[2] = (byte)b3;
            msg.data[3] = (byte)b4;

            _client.pushSendQue(REG_TYPE.FP, msg);
        }

        #endregion
        #region Log 



        [DoNotNotify]
        public Logger TempSerilog { get; set; } = null;
        [DoNotNotify]
        public Logger TempSerilogH { get; set; } = null;
        [DoNotNotify]
        public Logger TempSerilogL { get; set; } = null;
        [DoNotNotify]
        public Logger WaterTempSerilog { get; set; } = null;

        public void startTempLog()
        {
            if (TempSerilog != null)
                TempSerilog.Dispose();
            string file = $"{Global.gConfigVM.TempLogPath}\\{Name}_{DateTime.Now.ToString("yyyy_MM_dd_HHmm")}" +
                $"{(IsSaveOnlyTemp ? "_NoRun" : "")}.csv";
            TempSerilog = new LoggerConfiguration()
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite, outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();

            if (Global.gConfigVM.SaveEXTemp) startTempLogEX();
            if (Global.gConfigVM.SaveWaterTemp) startWaterTempLog();
        }

        public void startTempLogEX()
        {
            if (TempSerilogH != null)
                TempSerilogH.Dispose();
            string file = $"{Global.gConfigVM.TempLogPath}\\{Name}_{DateTime.Now.ToString("yyyy_MM_dd_HHmm")}" + "_H" +
                $"{(IsSaveOnlyTemp ? "_NoRun" : "")}.csv";
            TempSerilogH = new LoggerConfiguration()
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite, outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();

            if (TempSerilogL != null)
                TempSerilogL.Dispose();
            file = $"{Global.gConfigVM.TempLogPath}\\{Name}_{DateTime.Now.ToString("yyyy_MM_dd_HHmm")}" + "_L" +
                $"{(IsSaveOnlyTemp ? "_NoRun" : "")}.csv";
            TempSerilogL = new LoggerConfiguration()
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite, outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();
        }

        public void startWaterTempLog()
        {
            if (WaterTempSerilog != null)
                WaterTempSerilog.Dispose();
            string file = $"{Global.gConfigVM.TempLogPath}\\{Name}_{DateTime.Now.ToString("yyyy_MM_dd_HHmm")}" +
                $"_Water.csv";
            WaterTempSerilog = new LoggerConfiguration()
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite, outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();
        }

        public void addTempLog()
        {
            if (!IsSaveOnlyTemp)
                if (RackStat != RACK_STAT.RUN) return;
            if (TempSerilog == null) startTempLog();
            if (TempSerilog == null) return;

            StringBuilder strBuilder = new StringBuilder();
            strBuilder.Append(DateTime.Now.ToString("MM-dd HH:mm:ss"));

#if TEST_MODE
            foreach (BoxModel bm in Boxs)
                foreach (SiteModel sm in bm.Sites)
                    foreach (int idx in sm.PVCtrlList)
                        strBuilder.Append(',' + Math.Round((double)sm.TConGetPV).ToString());

            foreach (BoxModel bm in Boxs)
                foreach (SiteModel sm in bm.Sites)
                    foreach (int idx in sm.SVCtrlList)
                        strBuilder.Append(',' + Math.Round((double)sm.TConGetSV).ToString());
#else
            foreach (BoxModel bm in Boxs)
                foreach (SiteModel sm in bm.Sites)
                    foreach (int idx in sm.PVCtrlList)
                        strBuilder.Append(',' + Math.Round(TConRegs[RegMap.TCon_GetPV + idx].ReadValue, 1).ToString());

            foreach (BoxModel bm in Boxs)
                foreach (SiteModel sm in bm.Sites)
                    foreach (int idx in sm.SVCtrlList)
                        strBuilder.Append(',' + Math.Round(TConRegs[RegMap.TCon_GetSV + idx].ReadValue, 1).ToString());
#endif

            TempSerilog.Information(strBuilder.ToString());

            //if (Global.gConfigVM.SaveEXTemp) addTempLogEX();
            if (Global.gConfigVM.SaveWaterTemp) addWaterTempLog();
        }

        public void addTempLogEX()
        {
            if (!IsSaveOnlyTemp)
                if (RackStat != RACK_STAT.RUN) return;
            if (TempSerilogH == null || TempSerilogL == null) startTempLogEX();
            if (TempSerilogH == null || TempSerilogL == null) return;

            StringBuilder strBuilder = new StringBuilder();
            strBuilder.Append(DateTime.Now.ToString("MM-dd HH:mm:ss"));

            //todo
            /*
            for (int i = 0; i < Global.gMainPageVM.MaxDimmCount; i++)
            {
                strBuilder.Append("," + DimRegs[i * 2 + 1].ReadValue);
            }

            foreach (CBoxModel box in CBoxs)
                foreach (CBoxSub sub in box.BoxSubs)
                    foreach (int idx in sub.SVCtrlList)
                        strBuilder.Append(','
                            + Math.Round(TConRegs[RegMap.TCon_GetSV + idx].ReadValue, 1).ToString());

            foreach (CBoxModel box in CBoxs)
                foreach (CBoxSub sub in box.BoxSubs)
                    foreach (int idx in sub.PVCtrlList)
                        strBuilder.Append(','
                            + Math.Round(TConRegs[RegMap.TCon_GetPV + idx].ReadValue, 1).ToString());
            */
            TempSerilogH.Information(strBuilder.ToString());

            strBuilder.Clear();
            strBuilder.Append(DateTime.Now.ToString("MM-dd HH:mm:ss"));
            
            //todo
            /*
            for (int i = 0; i < Global.gMainPageVM.MaxDimmCount; i++)
            {
                strBuilder.Append("," + DimRegs[i * 2].ReadValue);
            }

            foreach (CBoxModel box in CBoxs)
                foreach (CBoxSub sub in box.BoxSubs)
                    foreach (int idx in sub.SVCtrlList)
                        strBuilder.Append(','
                            + Math.Round(TConRegs[RegMap.TCon_GetSV + idx].ReadValue, 1).ToString());


            foreach (CBoxModel box in CBoxs)
                foreach (CBoxSub sub in box.BoxSubs)
                    foreach (int idx in sub.PVCtrlList)
                        strBuilder.Append(','
                            + Math.Round(TConRegs[RegMap.TCon_GetPV + idx].ReadValue, 1).ToString());
            */
            TempSerilogL.Information(strBuilder.ToString());

        }

        public void addWaterTempLog()
        {
            if (WaterTempSerilog == null) startWaterTempLog();
            if (WaterTempSerilog == null) return;

            StringBuilder strBuilder = new StringBuilder();
            strBuilder.Append(DateTime.Now.ToString("MM-dd HH:mm:ss"));

            int end = RegMap.TCon_GetCoolerPV + RegMap.MaxCoolerCount;
            for (int i = RegMap.TCon_GetCoolerPV; i < end; i++)
            {
                strBuilder.Append("," + TConRegs[i].ReadValue.ToString());
            }

            WaterTempSerilog.Information(strBuilder.ToString());
        }

        public void endTempLog()
        {
            //if (TempSerilog == null) startTempLog();
            if (TempSerilog == null) return;

            StringBuilder strBuilder = new StringBuilder();

            strBuilder.Append("\r\n");

            TempSerilog.Information(strBuilder.ToString());

            if (TempSerilog == null)
                return;
            TempSerilog.Dispose();
            TempSerilog = null;

            if (Global.gConfigVM.SaveEXTemp) endTempLogEX();
            if (Global.gConfigVM.SaveWaterTemp) endWaterTempLog();
        }

        public void endTempLogEX()
        {
            //  if (TempSerilogH == null || TempSerilogL == null) startTempLogEX();
            if (TempSerilogH == null || TempSerilogL == null) return;

            if (TempSerilogH != null)
            {
                TempSerilogH.Dispose();
                TempSerilogH = null;
            }

            if (TempSerilogL != null)
            {
                TempSerilogL.Dispose();
                TempSerilogL = null;
            }

        }

        public void endWaterTempLog()
        {
            //if (TempSerilog == null) startTempLog();
            if (WaterTempSerilog == null) return;

            StringBuilder strBuilder = new StringBuilder();

            strBuilder.Append("\r\n");

            WaterTempSerilog.Information(strBuilder.ToString());

            if (WaterTempSerilog == null)
                return;
            WaterTempSerilog.Dispose();
            WaterTempSerilog = null;

        }


        //todo
        /*
        public void markTempLog(MarkerModel mm)
        {
            if (RackStat != RACK_STAT.RUN) return;
            if (TempSerilog == null) startTempLog();
            if (TempSerilog == null) return;
            string msg = $"mark,{mm.X},{mm.Y},{mm.Name}";
            TempSerilog.Information(msg);
        }
        */

#endregion
    }
}
