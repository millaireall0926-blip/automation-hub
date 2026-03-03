using HTCRack.Base;
using HTCRack.Net;
using HTCRack.ViewModel;
using PropertyChanged;
using Serilog;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Documents;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Threading;
using System.Xml;
using RedfishLib;
using Application = System.Windows.Application;
using Logger = Serilog.Core.Logger;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Rebar;
using System.Threading.Tasks;

namespace HTCRack.Model {

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
        BMC_RESET
    }

    public enum ALRAM_TYPE
    {
        DONE = 0,
        WARN,
        ERROR,
    }

    public enum POWER_STAT : int
    {
        PLC_OFF = 0,
        POWER_OFF = 1, // AC, 48V,12V All Off
        MIXED = 2, // AC, 48V, 12V Mixed Stat
        POWER_ON = 3, // AC, 48V,12V All On
        WAIT_RECV = 4
    }

	//08/22 BMC Add Point
	public enum DimmCheckState
	{
		IDLE,              // 1. 시스템의 기본 대기 상태. Reset() 호출을 기다림.
		RESET_TRIGGERED,   // 2. Reset() 호출 직후, 온도가 0으로 떨어지기를 기다림.
		REBOOT,            // 3. 온도가 0이 된 후, 다시 켜지기를 기다림.
		STABILIZING,       // 4. 신호 감지 후, 안정화 카운트 진행 중.
		MONITORING         // 5. 기준 설정 후, DIMM 소실 감시 중.
	}

	public class RackModel : BaseVM {
        
        public string Name { get; set; }        
        [DoNotNotify] public int RackIndex { get; set; } = 0;
        #region Dimm View 출력용
        [DoNotNotify] public int GraphIndex { get; set; } = 0;        
        [DoNotNotify] public List<DimDataModel> DimDatas { get; set; } = new List<DimDataModel>();
        [DoNotNotify] public List<MarkerModel> FailMakers { get; set; } = new List<MarkerModel>();

        #endregion
        [DoNotNotify] public List<int> DimPoslList { get; set; } = new List<int>();

        //08/22 BMC Add Point
        private int _basedimmmountcount { get; set; } = 0;
        public int basedimmMountCount
        {
            get => _basedimmmountcount;
            set
            {
                _basedimmmountcount = value;
                bdmc = (_basedimmmountcount / 2).ToString();
            }
        }
        public string bdmc {  get; set; } = "0";

        private int _curdimmmountcount {  get; set; } = 0;
        public int CurDimmMountCount
        {
            get => _curdimmmountcount;
            set
            {
                _curdimmmountcount = value;
                cdmc = (_curdimmmountcount / 2).ToString();
            }
        }
        public string cdmc { get; set; } = "0";

		private int _resetTriggerTimeoutCounter = 0;
		private int stabilizationCounter = 0; // Stabilizing 상태에서만 사용하는 카운터
        private DimmCheckState currentState = DimmCheckState.IDLE; // 현재 상태를 관리하는 단일 변수
        private const int STABILIZATION_THRESHOLD = 60;
		private const int RESET_TRIGGER_TIMEOUT_THRESHOLD = 50;

		// Check Box

		#region Rack / innner Box / Boarder관련
		public Visibility ShowBox { get; set; } = Visibility.Hidden;
        public string BoxMsg { get; set; } = "";
        public SolidColorBrush BoxColor { get; set; }  // Done, Error Color
        public SolidColorBrush BoarderColor { get; set; } // Rack Boarder Color = State Color
        public SolidColorBrush RackColor { get; set; } = Global.gRackColors[0];

        public Visibility ShowOnlyTemp { get; set; } = Visibility.Collapsed;

        public void showAlram(ALRAM_TYPE type,string head, string msg, bool isLogging = true)
        {
            BoxColor = Global.gAlramColors[(int)type];
            BoxMsg = msg;
            ShowBox = Visibility.Visible;

            if (isLogging) 
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR,head + ":"+ msg);
        }

        [DoNotNotify]
        public string FpgaVersion { get; set; } = "";        

//      SSR is concurrent. need to manage?
//        [DoNotNotify]
//        public byte SSRCtrl { get; set; } = 0;// SSR is a synthetic bit, so save and use it.

        public RACK_STAT _rackStat;
        public RACK_STAT RackStat
        {
            get => _rackStat;
            set
            {
                _rackStat = value;
                BoarderColor = Global.gBoarderColors[(int)_rackStat];
                if (_rackStat == RACK_STAT.DONE)
                {
                    showAlram(ALRAM_TYPE.DONE, $"[Prog][{Name}]", "Test Complete", false);
                    
                }
                else if (_rackStat == RACK_STAT.ERROR) 
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : RACK_STAT.ERROR stopTest");
                    stopTest();
                }

                if (!(_rackStat == RACK_STAT.RUN || _rackStat == RACK_STAT.PREV_RUN))
                    if (MaxPV != "")
                        MaxPV = "";

                StatText = _rackStat.ToString();
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
                {
                    DGCmdRun?.RaiseCanExecuteChanged();
                    DGCmdStop?.RaiseCanExecuteChanged();
                }));
            }
        }

        public string StatText { get; set; }

        bool _isTCUnit = true;
        public bool IsTCUnit { get => _isTCUnit;
            set 
            { 
                _isTCUnit = value;
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[User][{Name}] : TCUnit {_isTCUnit}");
            } 
        }
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

                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[User][{Name}] : Tams {_IsTams}");

            }
        }
        bool _isAS = false;
        public bool IsAS { get => _isAS; 
            set { _isAS = value;
                if (!_isAS)
                {
                    RackColor = Global.gRackColors[0];
                    sendExtFan(0x1);
                }
                else
                {
                    RackColor = Global.gRackColors[1];
                    sendExtFan(0x0);
                }
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[User][{Name}] : AS {_isAS}");
            } 
        }

        public bool _isSaveOnlyTemp = false;
        public bool IsSaveOnlyTemp { get => _isSaveOnlyTemp; 
            set {
                _isSaveOnlyTemp = value;
                if (!_isSaveOnlyTemp) { endTempLog(); }
            } }

        public bool EnableSV { get; set; } = true;

        public double SetSV { get; set; } = 0;

        [DoNotNotify]
        public int SideOpenDetect { get; set; } = 0;

        public bool _sideOpenRun = false;
        [DoNotNotify]
        public bool SideOpenRun { 
            get => _sideOpenRun;
            set { _sideOpenRun = value; 
                if (_sideOpenRun)
                    
                    _openLimitTimer.Start(); 
                else
                    _openLimitTimer.Stop();
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

        public DispatcherTimer _openLimitTimer = new DispatcherTimer();

        public string MaxPV { get; set; } = "";


        public DispatcherTimer _tamsFileTimer = new DispatcherTimer();


        public DispatcherTimer _boxEmptyTimer = new DispatcherTimer();  // Dimm 없는 Box는 Stop 

        #endregion

        #region BMC 관련
        public RestBMC _bmc = null;        

        public void quitBmc()
        {
            if (_bmc != null)
                _bmc.Stop();
        }
        public void initBmc(string user, string pwd, string mainPath, string loginPath, List<string> subPath)
        {
            _bmc = new RestBMC();
            _bmc.initItems(Global.gMainPageVM.MaxDimmCount);
            XmlDocument doc = Global.gConfigDoc;
            var nodes = doc.SelectNodes("/htc-rack/redfish/bmc-group/bmc");
            string ip = "";
            foreach (XmlNode node in nodes)
            {
                if (Convert.ToInt32(node.Attributes["id"].Value) != RackIndex) continue;                
                ip = node.Attributes["ip"].Value;
                break;
            }
            _bmc.IP = ip;
            _bmc.User = user;
            _bmc.Password = pwd;
            _bmc.Path = mainPath;
            _bmc.AuthPath = loginPath;
            _bmc.setList(subPath);
            

        }
        #endregion


        //Main View 출력용
        ObservableCollection<CBoxModel> _cboxs = new ObservableCollection<CBoxModel>();
        public ObservableCollection<CBoxModel> CBoxs {
            get { return _cboxs; }
            set { _cboxs = value; PC(nameof(CBoxs)); }
        }


        public void setEachTempItem(XmlNode node, ref double[] arr)
        {
            if (node == null) return;
            
            string text = node.InnerText.Trim();
            string[] buf = text.Split(',');
            int index = 0;
            foreach (string s in buf)
            {
                if (s == "") break;
                string str = s.Trim();
                arr[index] = Convert.ToDouble(str);
                index++;
            }
     
        }

        public void openLimitTimer_Tick(object sender, EventArgs e)
        {                      
            showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]","OverTime SlideOpen TestStop");
            SideOpenRun = false;
        }


        #region BMC Test Function 
        DispatcherTimer bmcTestTimer = new DispatcherTimer();

        public void bmcTimer_Start()
        {
            RackStat = RACK_STAT.STOP;
            bmcTestTimer.Start(); }
        public void bmcTimer_Stop()
        {
            RackStat = RACK_STAT.DISCONNECT;
            bmcTestTimer.Stop();
            _bmc.Stop();
        }
        public void bmcTestTimer_Tick(object sender, EventArgs e)
        {
            

            if (_bmc.checkRun())
            {
                for (int i = 0; i < _bmc.getDimmTempSize(); i++)
                {
                    DimRegs[i].ReadValue = _bmc.getDimmTemp(i);
                    
                }
                foreach (CBoxModel box in CBoxs)
                    box.processBoxDimmInStop();
            }
            else
            {
                _bmc.Start();
            }
            
        }
        #endregion

        public List<List<int>> _tCMap;
        public void loadTCMap()
        {
            if (Global.gConfigDoc == null) return;

            _tCMap = new List<List<int>>();
            for (int i = 0; i < 3; i++)
            {
                _tCMap.Add(new List<int>());
            }

            var node = Global.gConfigDoc.SelectSingleNode("/htc-rack/rack-map");
            _openLimitTimer.Interval = TimeSpan.FromSeconds(Convert.ToInt32(node.Attributes["open-limit"].Value));
            _openLimitTimer.Tick += openLimitTimer_Tick;
            ShowOnlyTemp = Convert.ToInt32(node.Attributes["save-only"].Value) == 1 ? Visibility.Visible : Visibility.Collapsed;

            //20231023 YHS DIMM POSTION VALUE
            node = Global.gConfigDoc.SelectSingleNode("/htc-rack/rack-map/dim-pos");
            string Ditext = node.InnerText;
            string[] Dibuf = Ditext.Split(',');
            foreach (string s in Dibuf)
            {
                DimPoslList.Add(Convert.ToInt32(s));
            }

            node = Global.gConfigDoc.SelectSingleNode("htc-rack/rack-map/rack");
            var rackNode = node.SelectSingleNode($"//rack[@id='{RackIndex}']");
            SetSV =  Convert.ToDouble(rackNode.Attributes["prev-sv"].Value);
            

            IsAS = Convert.ToDouble(rackNode.Attributes["as"].Value) == 1;


            int boxIndex = 0;
            foreach(XmlNode boxNode in rackNode.ChildNodes)
            {
                CBoxModel box = new CBoxModel(boxNode.Attributes["name"].Value, this);
                box.BoxIndex = boxIndex++;

                var mmNode = boxNode.SelectSingleNode("MinBefore10m");
                setEachTempItem(mmNode, ref box.MinBefore10m);

                mmNode = boxNode.SelectSingleNode("MaxBefore10m");
                setEachTempItem(mmNode, ref box.MaxBefore10m);

                var ofsNode = boxNode.SelectSingleNode("sv-ofs");
                setEachTempItem(ofsNode, ref box.EachTempOffsets);




                var eachNode = boxNode.SelectSingleNode("each-temp");
                if (eachNode != null)
                {
                    string text = eachNode.InnerText.Trim();
                    string[] buf = text.Split(',');
                    int index = 0;
                    foreach (string s in buf)
                    {
                        if (s == "") break;
                        string str = s.Trim(); 

                        foreach (DIM_TYPE type in System.Enum.GetValues(typeof(DIM_TYPE)))
                        {
                            if (str.ToUpper() == type.ToString())
                            { box.EachTempHLs[index] = type; break; }
                        }
                        index++;
                    }
                }



                foreach (XmlNode subNode in boxNode.ChildNodes)
                {
                    if (subNode.Name != "sub") continue;

                    CBoxSub sub = new CBoxSub();
                    var itemNode = subNode.SelectSingleNode("sv");
                    string text = itemNode.InnerText;
                    string[] buf = text.Split(',');
                    foreach (string s in buf)
                    {
                        if (s == "") break;
                        sub.SVCtrlList.Add(Convert.ToInt32(s));
                    }

                    itemNode = subNode.SelectSingleNode("ssr");
                    text = itemNode.InnerText;
                    buf = text.Split(',');
                    foreach (string s in buf)
                    {
                        if (s == "") break;
                        int shift = Convert.ToInt32(s);
                        int value = 1 << shift;
                        sub.SSRValue |= value;
                    }


                    itemNode = subNode.SelectSingleNode("fp");
                    text = itemNode.InnerText;
                    sub.FPIndex = Convert.ToInt32(text);

                    itemNode = subNode.SelectSingleNode("fan");
                    text = itemNode.InnerText;
                    buf = text.Split(',');
                    foreach (string s in buf)
                    {
                        if (s == "") break;
                        sub.FanCtrlList.Add(Convert.ToInt32(s));
                    }


                    itemNode = subNode.SelectSingleNode("pv");
                    text = itemNode.InnerText;
                    buf = text.Split(',');
                    foreach (string s in buf)
                    {
                        if (s == "") break;
                        sub.PVCtrlList.Add(Convert.ToInt32(s));
                    }

                    itemNode = subNode.SelectSingleNode("dim");
                    string dimType = itemNode.Attributes["type"].Value;
                    foreach (DIM_TYPE type in System.Enum.GetValues(typeof(DIM_TYPE)))
                    {
                        if (dimType.ToUpper() == type.ToString())
                        { sub._dimType = type; break; }
                    }

                    text = itemNode.InnerText;
                    buf = text.Split(',');
                    foreach (string s in buf)
                    {
                        if (s == "") break;
                        sub.DimCtrlList.Add(Convert.ToInt32(s));
                        box.DimModels.Add(new DimModel());
                    }
                    box.BoxSubs.Add(sub);
                }

                box.DimColumns = 4;
                if (Global.gMainPageVM.MaxDimmCount == 24)
                    box.DimColumns = 6;
                else if (Global.gMainPageVM.MaxDimmCount == 32)
                    box.DimColumns = 8;

                CBoxs.Add(box);
            }

            _client._ip = rackNode.Attributes["ip"].Value;
            _client._index = RackIndex;
            _client._port = Convert.ToInt32(rackNode.Attributes["port"].Value);
            _client.funcRecv += doRecv;
            _client.funcConect += connected;

            _tamsFileTimer.Interval = TimeSpan.FromSeconds(8);
            _tamsFileTimer.Tick += tamsFileCheckTimer_Tick;
            _tamsFileTimer.Start();




        }

        public void tamsFileCheckTimer_Tick(object sender, EventArgs e)
        {
            if (Global.gConfigVM.TestStartMode != 1) return;
            if (!(RackStat == RACK_STAT.STOP || RackStat == RACK_STAT.DONE)) return;
            if (ShowBox == Visibility.Visible && BoxColor == Global.gAlramColors[(int)ALRAM_TYPE.ERROR]) return; //Error PoPup 발생 시 자동 Start 안하게
            if (!setTamsTemp(onlyFileFind : true)) return;
            if (findEndFile()) return;

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : Auto Run File Found");
            startTest();
        }

        public void boxEmptyCheckTimer_Tick(object sender, EventArgs e )
        {
            if (RackStat != RACK_STAT.RUN) return;
            if (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.NOSV) return;

            foreach (CBoxModel box in CBoxs)
            {
                if (box.BoxStat == BOX_STAT.RUN)
                {
                    if (box.MaxInBoxValue == 0) box.NoDimmCount++;
                    else box.NoDimmCount = 0;

                    if (box.NoDimmCount > Global.gConfigVM.NoDimmBoxStopMin) // 10 분 이상 nodimm이면 stop
                    {
                        box.sendStop();
                        box.sendFan(50);
                        int ssr = 0;
                        foreach (CBoxSub sub in box.BoxSubs)                        
                            ssr |= sub.SSRValue;
                        
                        removeSSR( ssr );
                    }
                }
                else if (box.BoxStat == BOX_STAT.STOP)
                {
                    if (box.MaxInBoxValue != 0)
                    {
                        int ssr = 0;
                        foreach (CBoxSub sub in box.BoxSubs)
                        {
                            sub.TargetSV = SetSV;
                            box.SVST(sub, SVC.TCON);
                            sub.SampleLongTerm.Clear();
                            sub.SampleShortTerm.Clear();
                            ssr |= sub.SSRValue;                            
                        }

                        box.sendSV(SetSV);
                        box.sendRun();
                        box.sendFan(100);

                        addSSR(ssr);
                    }
                }
            }

        }



        public HtcClient _client = new HtcClient();
        public RackModel(int index) {
            Name = "FIX" + (index + 1).ToString();
            RackIndex = index;

            loadTCMap();
            initDimData();

            RackStat = RACK_STAT.DISCONNECT;

            DGCmdRun = new DelegateCommand(p => { startTest(); }, canExcuteRun);
            DGCmdStop = new DelegateCommand(p => { SideOpenRun = false; Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : DGCmdStop stopTest"); stopTest(); }, canExcuteStop);
            DGCmdPower = new DelegateCommand(p => { clickedPower(); }, canExcutePower);

            DGCmdSetTemp = new DelegateCommand(p => { setTemp(); });

            _throttleTimer.Interval = TimeSpan.FromMinutes(10);
            _throttleTimer.Tick += throttleTimer_Tick;
            _throttleTimer.Stop();

            _boxEmptyTimer.Interval = TimeSpan.FromMinutes(1);
            _boxEmptyTimer.Tick += boxEmptyCheckTimer_Tick;
            _boxEmptyTimer.Start();


        }




        public void initDimData() { 
            for (int i = 0;i < Global.gMainPageVM.MaxDimmCount; i++) 
            {
                DimDatas.Add(new DimDataModel());
            }

        }

        #region 내부 Data용 Regs
        public List<RegModel> FanRegs = new List<RegModel>();
        public List<RegModel> CurRegs = new List<RegModel>();
        public List<RegModel> TempRegs = new List<RegModel>();        
        public List<RegModel> DimRegs = new List<RegModel>();
        public List<RegModel> TConRegs = new List<RegModel>();
        public List<RegModel> GPIORegs = new List<RegModel>();
        #endregion

        public double[] _prevDimmTemps = new double[64];


        public void initRegs()
        {            

            foreach (RegModel reg in Global.gEnginerVM.FanRegModels) 
                FanRegs.Add(MConv.ObjectDeepCopy(reg));           
            foreach (RegModel reg in FanRegs)
            {                
                if (reg.FUNC == "11") reg.funcReadVlaue += doValueFanGetDuty;
                if (reg.FUNC == "20") reg.funcReadVlaue += doValueFanGetTach;
            }

            foreach (RegModel reg in Global.gEnginerVM.CurRegModels)
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

            foreach (RegModel reg in Global.gEnginerVM.TempRegModels)
                TempRegs.Add(MConv.ObjectDeepCopy(reg));            
            foreach (RegModel reg in TempRegs)
            {
                if (reg.FUNC == "11") reg.funcReadVlaue += doValueGetTemp;
            }

            foreach (RegModel reg in Global.gEnginerVM.DimRegModels)
                DimRegs.Add(MConv.ObjectDeepCopy(reg));            
            foreach (RegModel reg in DimRegs)            
                reg.funcReadVlaue += doValueGetDim;
            

            // RegMap TCon_xxx Index
            foreach (RegModel reg in Global.gEnginerVM.TConRegModels)
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

            foreach (RegModel reg in Global.gEnginerVM.GPIORegModels)
                GPIORegs.Add(MConv.ObjectDeepCopy(reg));
            foreach (RegModel reg in GPIORegs)
                reg.funcReadVlaue += doValueDecimal;
        }

        #region Read Value Change Delegate Functions
        public double doValueFanGetDuty(string val)
        {
            byte data = Convert.ToByte(val, 16);
            double x = Convert.ToDouble(data);
            return  Math.Round((x / 256) * 100, 1);
        }

        public double doValueFanGetTach(string val)
        {
            int data = (Convert.ToInt16(val,16) >> 3) & 0x1fff;
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
            int data = (int)(Convert.ToInt16(val, 16) & 0x7fff) ;
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
            if (TempSerilog == null)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : addTempLog Step startTempLog");
                startTempLog();
            }
            if (TempSerilog == null) return;

            StringBuilder strBuilder = new StringBuilder();
            strBuilder.Append(DateTime.Now.ToString("MM-dd HH:mm:ss"));
            for (int i = 0; i < DimDatas.Count; i++)
            {
                strBuilder.Append("," + DimDatas[i].Temp.ToString());
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

            TempSerilog.Information(strBuilder.ToString());

            if (Global.gConfigVM.SaveEXTemp) addTempLogEX();
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
            for (int i = 0; i < Global.gMainPageVM.MaxDimmCount; i++)
            {
                strBuilder.Append("," + DimRegs[i*2 +1].ReadValue);
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

            TempSerilogH.Information(strBuilder.ToString());

            strBuilder.Clear();            
            strBuilder.Append(DateTime.Now.ToString("MM-dd HH:mm:ss"));
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



        public void markTempLog(MarkerModel mm)
        {
            if (RackStat != RACK_STAT.RUN) return;
            if (TempSerilog == null) startTempLog();
            if (TempSerilog == null) return;
            string msg = $"mark,{mm.X},{mm.Y},{mm.Name}";
            TempSerilog.Information(msg);
        }


        #endregion


        #region Test Operation 

        public DateTime TimeStart { get; set; }
        public DateTime TimeEnd { get; set; }

        public bool canExcuteRun(object obj)
        {
			// BMC_RESET 상태일 때는 Run 버튼 비활성화
			if (RackStat == RACK_STAT.BMC_RESET)
				return false;
			if (RackStat == RACK_STAT.STOP ||
                RackStat == RACK_STAT.DONE )
                return true;            

            return false;
        }
        public DelegateCommand DGCmdRun { get; set; }
        public bool canExcuteStop(object obj)
        {
			// BMC_RESET 상태일 때는 Stop 버튼 비활성화
			if (RackStat == RACK_STAT.BMC_RESET)
				return false;
			if (RackStat == RACK_STAT.RUN || 
                RackStat == RACK_STAT.ERROR)
                return true;

            return false;
        }
        public DelegateCommand DGCmdStop { get; set; }


        public POWER_STAT _powerStat = POWER_STAT.PLC_OFF;
        public POWER_STAT PowerStat { 
            get => _powerStat;
            set { 
                _powerStat = value;
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
                DGCmdPower?.RaiseCanExecuteChanged()));
                
            } 
        }

        public DelegateCommand DGCmdPower { get; set; }

        public DelegateCommand DGCmdSetTemp { get; set; }
     


        public async void startTest()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : Run");

            if (IsAS) return;            
            if (!(RackStat == RACK_STAT.STOP 
                || RackStat == RACK_STAT.DONE
                || RackStat == RACK_STAT.ERROR
                )) return;

            if (!IsTCUnit)
            {
                showAlram(ALRAM_TYPE.WARN, $"[Prog][{Name}]","TC Unit Uncheck", isLogging:false);                
                return;
            }

            
            if (IsTams)
            {
                if (!setTamsTemp(onlyFileFind : false))
                {
                    //showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]", "Tams Temp File Error");
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Prog][{Name}] : Tams File Disappeared or Format Discrepancies");
                    return;
                }
                else
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : Tams SetTemp {SetSV}℃");
            }
            

            if (SetSV < Global.gConfigVM.MinSetSV || SetSV > Global.gConfigVM.MaxSetSV)
            {
                showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]","SetTemp Range is " + Global.gConfigVM.MinSetSV + "~" + Global.gConfigVM.MaxSetSV + ". Check TargetSV");
                return;
            }

            ShowBox = System.Windows.Visibility.Hidden;
            //  Side Door Check 0 = Close , 1 = Open
            if (GPIORegs[RegMap.GPIO_SideClose].ReadValue == 1)
            {
                showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]","Side Door Open");
                return;
            }

			//BMC Reset 함수 호출
			if (Global.gConfigVM.isUseBMC)
            {
				RackStat = RACK_STAT.BMC_RESET;
				currentState = DimmCheckState.IDLE; // 초기화

				Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] 테스트 시작 전 BMC 리셋을 시작합니다.");

                await Task.Delay(10 * 1000); //10초대기

				Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] BMC 리셋 명령 전송.");
				var (status, responseBody) = await _bmc.Reset();

				// API 응답 결과는 참고용 로그로만 남깁니다.
				if (status != SESSION_STATUS.OK)
				{
					Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[{Name}] BMC로부터 리셋 명령 수신 응답을 받지 못했습니다. (응답: {responseBody})");
				}
				else
				{
					Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] BMC가 리셋 명령을 성공적으로 수신했습니다.");
				}

				// 2. 성공/실패 여부와 상관없이 상태 머신을 다음 단계로 진행시킵니다.
				Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] 실제 DIMM 초기화를 감시합니다.");
				_resetTriggerTimeoutCounter = 0;
				currentState = DimmCheckState.RESET_TRIGGERED;
			}
			PrevSV = SetSV;

            sendFPSV(SetSV);
            if (Global.gConfigVM.FPDisplay != 0)
                sendFPDisplay(Global.gConfigVM.FPDisplay);
            //sendFPMap(
            //  CBoxs[0].BoxSubs[0].FPIndex,
            //  CBoxs[1].BoxSubs[0].FPIndex,
            //  CBoxs[2].BoxSubs[0].FPIndex) ;

            TimeStart = DateTime.Now;
            TimeEnd = default(DateTime);
            RackStat = RACK_STAT.PREV_RUN;

            Global.gMainVM.PlcCtl.setSlotMC(RackIndex, true);
            Thread.Sleep(1000);

            foreach (CBoxModel box in CBoxs)
            {
                box.ExtSVValue = SetSV * box.ExtSVScope + box.ExtSVOffet;
                int ssr = 0;
                foreach (CBoxSub sub in box.BoxSubs)
                {
                    sub.TargetSV = SetSV;
                    box.SVST(sub, SVC.TCON);
                    sub.SampleLongTerm.Clear();
                    sub.SampleShortTerm.Clear();
                    ssr |= sub.SSRValue;

                    sub.BSVAlue = 0;
                }
                               
                box.sendSV(SetSV);
                box.sendRun();
                box.sendFan(100);
                
                addSSR(ssr);
            }



            Throttle_SV_Offset = 0;
           

            startTempLog();
            GraphIndex = 0;

            Array.Clear(_prevDimmTemps, 0, _prevDimmTemps.Length);
            


            _pcCheckCount = 0;
            FailMakers.Clear();

        }
        public void setTemp()
        {            
            if (IsAS) return;
            if (!IsTCUnit) return;

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : SetTemp {SetSV}℃");

            foreach (CBoxModel box in CBoxs)
            {
                box.ExtSVValue = SetSV * box.ExtSVScope + box.ExtSVOffet;
                box.sendSV(SetSV);
                foreach (CBoxSub sub in box.BoxSubs)
                    sub.TargetSV = SetSV;
            }
            sendFPSV(SetSV);
        }
        public void stopTest()
        {
            if (IsAS) return;
            if (!IsTCUnit) return;

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : EndTest");

            // Process Stop 

            TimeEnd = DateTime.Now;
            RackStat = RACK_STAT.PREV_STOP;

            foreach (CBoxModel box in CBoxs)
            {             
                box.sendStop();
                box.sendFan(50);
                 
            }

            clearSSR();
            endTempLog();
            if (Global.gConfigVM.FPDisplay != 0)
                sendFPDisplay(0);// FP Display PV로.

            Thread.Sleep(1000);
            Global.gMainVM.PlcCtl.setSlotMC(RackIndex, false);
            
            // if (error) remind Error
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
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[User][{Name}] : Power On");

            }
            else if (PowerStat == POWER_STAT.POWER_ON)
            {
                MessageBoxResult res = HandyControl.Controls.MessageBox.Show($"Do You Want to Shut Down Rack{RackIndex + 1}?", "Power Off", MessageBoxButton.YesNo, MessageBoxImage.Exclamation);
                if (res == MessageBoxResult.No) return;

                PowerStat = POWER_STAT.WAIT_RECV;
                Global.gMainVM.PlcCtl.setSlotPower(RackIndex, false);
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[User][{Name}] : Power Off");

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
            }else 
            {
                if (PowerStat == POWER_STAT.POWER_ON)
                {
                    PowerStat = POWER_STAT.WAIT_RECV;
                    Global.gMainVM.PlcCtl.setSlotPower(RackIndex, false);
                }
            }

        }

        public void processStat()
        {
            int runSum = 0;
            int stopSum = 0;
            int tuneSum = 0;
            int errSum = 0;

            RACK_STAT curStat = RackStat;
            foreach (CBoxModel box in CBoxs)
            {
                box.processBoxST();


                if (box.BoxStat == BOX_STAT.RUN)
                    runSum++;
                else if (box.BoxStat == BOX_STAT.STOP)
                    stopSum++;
                else if (box.BoxStat == BOX_STAT.TUNE)
                    tuneSum++;
                else if (box.BoxStat == BOX_STAT.ERR)
                    errSum++;
            }

            if (errSum > 0)
            {
                RackStat = RACK_STAT.ERROR;
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{Name}] : TCon Error. Check in Engineer Page");
                return;
            }


            if (runSum > 0)
                curStat = RACK_STAT.RUN;
            else if (tuneSum > 0)
                curStat = RACK_STAT.TUNNING;
            else if (stopSum == CBoxs.Count)
                curStat = RACK_STAT.STOP;

            if (RackStat == RACK_STAT.PREV_RUN && curStat != RACK_STAT.RUN)
            {
                if (TimeStart.AddSeconds(8) < DateTime.Now)
                {
                    RackStat = RACK_STAT.ERROR;
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{Name}] : Can't Start. CBox Is Not Change Run");
                    return;
                }
            }else if (RackStat == RACK_STAT.PREV_STOP && curStat != RACK_STAT.STOP)
            {
                if (TimeEnd.AddSeconds(8) < DateTime.Now)
                {
                    RackStat = RACK_STAT.ERROR;
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{Name}] : Can't Start. CBox Is Not Change STOP");
                    return;
                }
            }
            if (curStat == RACK_STAT.RUN && RackStat == RACK_STAT.PREV_STOP)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : PREV_STOP,  Stop is in progress");
            }
            else
            {
                // 그 외의 모든 경우에는 RackStat를 curStat으로 업데이트한다.
                RackStat = curStat;
            }

            // 처음 접속시 run 상태이면 TCon Recv SV로 SV Setting
            if (PrevSV == 0 && RackStat == RACK_STAT.RUN)
            {
                foreach (CBoxModel box in CBoxs)
                {
                    box.ExtSVValue = SetSV * box.ExtSVScope + box.ExtSVOffet;
                    
                    foreach (CBoxSub sub in box.BoxSubs)
                    {
                        int index = sub.SVCtrlList[0];
                        sub.TargetSV = TConRegs[RegMap.TCon_GetSV + index].ReadValue;
                        box.SVST(sub, SVC.STABLE);
                        sub.SampleLongTerm.Clear();
                        sub.SampleShortTerm.Clear();                        
                        sub.BSVAlue = 0;
                    }
                }
                PrevSV = SetSV;
                sendFPDisplay(Global.gConfigVM.FPDisplay);
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
            else if(!IsAS && (Convert.ToInt32(Global.gConfigVM.ExtraFan, 16) == 1))
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

        bool _prevFETWarn = false;
        public void checkSystem()
        {           
            if (RackStat == RACK_STAT.RUN || RackStat == RACK_STAT.PREV_RUN)
            {
               //  Side Door Check 0 = Close , 1 = Open
                if (GPIORegs[RegMap.GPIO_SideClose].ReadValue == 1)
                {
                    showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]","Side Door Open");
                    RackStat = RACK_STAT.ERROR;
                    SideOpenRun = true;
                    return;
                }

                 //Board Con Check 0 = Contact , 1 = Discontact
                if (GPIORegs[RegMap.GPIO_BDCon].ReadValue == 1)
                {
                    showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]","Board Contact Fail");
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
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Prog][{Name}] : Water Jacket Temp Noise " +
                            $"Limit:{Global.gConfigVM.WaterOver}, Read:{waterTemp}");
                    }
                    else if (waterTemp > Global.gConfigVM.WaterOver)
                    {
                        showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]",
                            $"Cooler{i - RegMap.TCon_GetCoolerPV +1}(1Base) Water Jacket Temp Error. " + 
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
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Prog][{Name}] : HeatThink Temp Warning Unit");
                    }else if (TempRegs[i].ReadValue < Global.gConfigVM.FetTempWarn && _prevFETWarn == true)
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
                for (int i = RegMap.Cur_GetCur; i< tempend; i++) 
                { 
                    if (CurRegs[i].ReadValue > Global.gConfigVM.PaltierCurOver)
                    {
                        showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]",
                            $"Paltier{i - RegMap.Cur_GetCur +1}(1Base) Current Over Error " +
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
            }catch (Exception e)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Prog][{Name}] : Tams File Find Error Ocurred");
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
            }catch (Exception e)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Prog][{Name}] : End File Find Error Ocurred");
            }

            return false;
        }

        int _pcCheckCount = 0;


        #region Up 제어용 Throttle SV Offset
        public DispatcherTimer _throttleTimer = new DispatcherTimer();

        public void throttleCtrlStart()
        {
            Throttle_SV_Offset = -3;
            _throttleTimer.Start();
        }


        public void throttleTimer_Tick(object sender, EventArgs e)
        {
            _throttleTimer.Stop();
            Throttle_SV_Offset = 0;
        }

        [DoNotNotify]
        public double Throttle_SV_Offset = 0;
        #endregion


        //TEST Code
        private Random rand = new Random();
        public async void SampleTest()
        {
			if (!(RackStat == RACK_STAT.STOP
				|| RackStat == RACK_STAT.DONE
				|| RackStat == RACK_STAT.ERROR || RackStat == RACK_STAT.DISCONNECT
				)) return;

			if (Global.gConfigVM.isUseBMC && Global.gConfigVM.UseBmcReset)
			{
				RackStat = RACK_STAT.BMC_RESET;
				currentState = DimmCheckState.IDLE; // 초기화

				Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] 테스트 시작 전 BMC 리셋을 시작합니다.");

				await Task.Delay(10 * 1000); //10초대기

				Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] BMC 리셋 명령 전송.");



				var stat = await _bmc.Reset();

                /*
				if (stat != SESSION_STATUS.OK)
				{
					showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}]", "BMC 리셋 실패");
					RackStat = RACK_STAT.ERROR; // 에러 상태로 전환하고
					return;                     // 함수 종료
				}
                */
                
				Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] BMC 리셋 완료.");
				currentState = DimmCheckState.RESET_TRIGGERED;
			}
            RackStat = RACK_STAT.PREV_RUN;
			await Task.Delay(10 * 1000);
			RackStat = RACK_STAT.RUN;
			await Task.Delay(10 * 1000);

			for (int j = 0; j < 1000; j++)
            {
                // --- 1. 현재 데이터 상태 파악 ---
                CurDimmMountCount = 0;
				bool anyTempIsUp = false;

				for (int i = 0; i < 48; i++)//_bmc.getDimmTempSize(); i++)
                {
                    double _temp = 0;
                    if (j < 100)
                    {
                        _temp = 50.0 + (rand.NextDouble() * 3.0);//_bmc.getDimmTemp(i);
                        DimRegs[i].ReadValue = _temp;
                    }
                    else if(j == 100)
                    {
                        if(i == 2 || i == 3)
                        {
                            DimRegs[i].ReadValue = 0;
                        }
                        else
                        {
                            _temp = 50.0 + (rand.NextDouble() * 3.0);//_bmc.getDimmTemp(i);
                            DimRegs[i].ReadValue = _temp;
                        }
                    }
                    else if(j > 100 && j < 150)
                    {
						DimRegs[i].ReadValue = 0;
					}
                    else if(j >= 150)
                    {
                        _temp = 70.0 + (rand.NextDouble() * 3.0);//_bmc.getDimmTemp(i);
                        DimRegs[i].ReadValue = _temp;
                    }

					if (_temp > 0)
					{
						CurDimmMountCount++;
						anyTempIsUp = true;
					}
				}
                if (Global.gConfigVM.UseBmcReset)
                {
                    // --- 2. 로직 리셋 조건 ---
                    switch (currentState)
                    {
                        // [상태 1: IDLE]
                        // 시스템의 기본 상태. 외부에서 Reset()을 호출하기 전까지 아무것도 하지 않고 대기합니다.
                        // (이 케이스는 의도적으로 비워둡니다)
                        case DimmCheckState.IDLE:
                            break;

                        // [상태 2: WAITING]
                        // Reset()이 호출된 직후의 상태. 첫 신호를 기다립니다.
                        case DimmCheckState.RESET_TRIGGERED:
                            if (anyTempIsUp)
                            {
                                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] 첫 신호 감지. 안정화를 시작합니다.");
                                stabilizationCounter = 0;
                                currentState = DimmCheckState.STABILIZING;
                            }
                            break;

                        // [상태 3: STABILIZING]
                        // 10번의 카운트가 진행됩니다.
                        case DimmCheckState.STABILIZING:
                            if (!anyTempIsUp)
                            {
                                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[{Name}] 안정화 중 신호 소실! 다시 신호를 대기합니다.");
                                currentState = DimmCheckState.REBOOT; // IDLE이 아닌 WAITING으로 돌아가 신호를 다시 기다림
                                break;
                            }

                            stabilizationCounter++;
                            if (stabilizationCounter >= STABILIZATION_THRESHOLD)
                            {
                                basedimmMountCount = CurDimmMountCount;
                                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] 안정화 완료. 기준({basedimmMountCount / 2}개) 설정. 감시를 시작합니다.");
                                currentState = DimmCheckState.MONITORING;
                            }
                            break;

                        // [상태 4: MONITORING]
                        // DIMM 소실을 감시합니다.
                        case DimmCheckState.MONITORING:
                            if (CurDimmMountCount < basedimmMountCount)
                            {
                                _ = HandleDimmLossAndResetAsync();// 이 함수 실행 후 기다리지 않고 진행
                            }
                            break;
                    }
                }
				else
				{
					if (currentState != DimmCheckState.IDLE)
					{
						currentState = DimmCheckState.IDLE;
						stabilizationCounter = 0; // 카운터도 초기화
						basedimmMountCount = 0;   // 기준값도 초기화
					}
				}
				// --- 3. 현재 상태에 따라 다른 작업 수행 ---

				await Task.Delay(1000);
                foreach (CBoxModel box in CBoxs)
                {
                    box.processBoxDimm();
                }
            }
			RackStat = RACK_STAT.STOP;
			/*
            RackStat = RACK_STAT.RUN;
            
            startTempLog();

            for (int a = 0; a < 1000000; a++)
            {
                for (int i = 0; i < 24; i++)
                {
                    DimDatas[i].Temp = 25.0 + (rand.NextDouble() * 70.0); ;
                }


                double max = 0;
                foreach (DimDataModel dim in DimDatas)
                {
                    if (max < dim.Temp)
                        max = dim.Temp;

                    dim.GraphData[GraphIndex] = dim.Temp;
                }
                GraphIndex++;
                if (GraphIndex >= 1_000_000)
                    GraphIndex = 0;

                addTempLog();

                int gindex = GraphIndex;
                if (Global.gMainVM.ShowDimm == Visibility.Visible && Global.gDimmVM.CurRack == RackIndex)
                    _ = Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Background, new Action(() => Global.gDimmVM.processRun(gindex)));

                await Task.Delay(5000);
            }

            endTempLog();
            */
		}
        public void SampleTestEnd()
        {
            RackStat = RACK_STAT.STOP;
        }
        public void processTest()
        {
            if (RackStat == RACK_STAT.RUN)
            {

                foreach (CBoxModel box in CBoxs)
                {
                    box.processBoxDimm();
                    if (box.InsideBoxTemp < 18)
                    {
                        showAlram(ALRAM_TYPE.ERROR, $"[Prog][{Name}][{box.Name}]","PV Temp Under 18°C");
                        RackStat = RACK_STAT.ERROR;
                    }
                    
                    box.processBoxSV();
                        
                }


                double max = 0;
                foreach (DimDataModel dim in DimDatas)
                {
                    if (max < dim.Temp)
                        max = dim.Temp;

                    dim.GraphData[GraphIndex] = dim.Temp;                    
                }
                GraphIndex++;
                if (GraphIndex >= 1_000_000)
                    GraphIndex = 0;

                addTempLog();
                MaxPV = max.ToString("F1");

                if (max == 0)
                {
                    _pcCheckCount++;
                    if (_pcCheckCount > Global.gConfigVM.NoDimmCheckCount) 
                    {
                        // dimm 온도 안올라옴. 
                        // 실장기 Test 종료 이거나 꺼짐으로 간주.
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : NoDimmCheckCount stopTest");
                        stopTest();
                    }
                } else
                    _pcCheckCount = 0;

                if (findEndFile())
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : Find EndFlag {Global.gConfigVM.TamsEndFile}");                    
                    stopTest();
                    RackStat = RACK_STAT.DONE;
                }else if (IsTams)
                {
                    if (!setTamsTemp(onlyFileFind : true))// tams file 이 없으면 종료..
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Prog][{Name}] : Tams files -{Global.gConfigVM.TamsSVFile}- missing. Test Stoped");
                        stopTest();
                    
                    }
                }
               
            }
        }

        #endregion

        public string convStr(byte[] buf, int offset, int len, bool bInv = false)
        {
            string ret = "";

            if (bInv) {
                for (int i = len - 1; i >= 0; i--)
                    ret += (buf[offset + i]).ToString("X2");
            }
            else {
                for (int i = 0; i < len; i++)
                    ret += (buf[offset + i]).ToString("X2");
            }
            return ret;
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
                    for (int i =0; i< msg.data.Length; i++) 
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
            }else
            {
                TempRegs[RegMap.Temp_GetTemp + msg.sub.Adr].Read = convStr(msg.data, 0, 2);
            }
        }

        public void recvDim(HtcMessage msg)
        {
            int regIdx = msg.sub.Adr * 2;

            DimRegs[RegMap.Dim_GetDim + (regIdx * 2)].Read = convStr(msg.data, 0, 2);
            DimRegs[RegMap.Dim_GetDim + (regIdx * 2) + 1].Read = convStr(msg.data, 2, 2);
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
            }else
            {
                if (msg.sub.Func == 0x20)
                {
                    TConRegs[RegMap.TCon_GetSV + msg.sub.Adr].Read = convStr(msg.data, 0, 2);
                }else if (msg.sub.Func == 0x21)
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

            } else if (msg.sub.Func == 0x12) // Interface State
            {


            }else if (msg.sub.Func == 0x11) // all status
            {
                _client._ackWaitCount = 0;
                int ofs = 0;
                for (int i = 0; i < 15; i++, ofs += 2)
                    FanRegs[RegMap.Fan_GetTach + i].Read = convStr(msg.data, ofs , 2, true);

                for (int i = 0; i < 11; i++, ofs += 2)
                    CurRegs[RegMap.Cur_GetCur + i].Read = convStr(msg.data, ofs, 2, true);
                
                for (int i = 0; i < 8; i++, ofs += 2)
                    TempRegs[RegMap.Temp_GetTemp + i].Read = convStr(msg.data, ofs, 2);

                //                Console.WriteLine($"----------------------------------------------");
                //BMC Read value Org
                /*
                if (Global.gConfigVM.isUseBMC)
                {
                    ofs += Global.gMainPageVM.MaxFixDimmCount * 4;
                    if (_bmc.checkRun())
                    {
                        for (int i = 0; i < _bmc.getDimmTempSize(); i++)
                        {
                            DimRegs[i].ReadValue = _bmc.getDimmTemp(i);
                        }
                    }
                    else
                    {
                        _bmc.Start();
                    }
                */
                //0822 add code 적용
                //BMC Read value Chage 기준 Dimm 추가
                
                if (Global.gConfigVM.isUseBMC)
                {
                    ofs += Global.gMainPageVM.MaxFixDimmCount * 4;
                    if (_bmc.checkRun())
                    {
                        // --- 1. 현재 데이터 상태 파악 ---
                        CurDimmMountCount = 0;
                        bool anyTempIsUp = false;

                        for (int i = 0; i < _bmc.getDimmTempSize(); i++)
                        {
                            double _temp = _bmc.getDimmTemp(i);
                            DimRegs[i].ReadValue = _temp;

                            if (_temp > 0)
                            {
                                CurDimmMountCount++;
                                anyTempIsUp = true;
                            }
                        }

                        // --- 2. 로직 리셋 조건 ---
                        switch (currentState)
                        {
                            // [상태 1: IDLE]
                            // 시스템의 기본 상태. 외부에서 Reset()을 호출하기 전까지 아무것도 하지 않고 대기합니다.
                            // (이 케이스는 의도적으로 비워둡니다)
                            case DimmCheckState.IDLE:
                                break;

                            // [상태 2: 모든 DIMM 온도가 0이 되기를 기다림]
                            // Reset()이 호출된 직후의 상태. 첫 신호를 기다립니다.
                            case DimmCheckState.RESET_TRIGGERED:
                                if (!anyTempIsUp)
                                {
                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] DIMM 초기화 확인. 시스템 재부팅 대기.");
                                    currentState = DimmCheckState.REBOOT;
                                }
                                else
                                {
                                    _resetTriggerTimeoutCounter++;
									//Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] RESET_TRIGGERED... Counter value: {_resetTriggerTimeoutCounter}");
									if (_resetTriggerTimeoutCounter >= RESET_TRIGGER_TIMEOUT_THRESHOLD)
                                    {
                                        Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[{Name}] 정해진 시간 내에 DIMM이 초기화되지 않았습니다. 리셋을 재시도.");
                                        _ = _bmc.Reset();
                                        _resetTriggerTimeoutCounter = 0;
                                    }
                                }
                                break;

                            // [재부팅 대기] 상태 3: 온도가 다시 감지되기를 기다림 (타임아웃 적용)
                            case DimmCheckState.REBOOT:
                                if (anyTempIsUp)
                                {
                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] DIMM 신호 재감지. 안정화를 시작합니다.");
                                    stabilizationCounter = 0;
                                    currentState = DimmCheckState.STABILIZING;
                                }
                                break;

                            // [상태 4: STABILIZING]
                            // 60번의 카운트가 진행됩니다.
                            case DimmCheckState.STABILIZING:
                                if (!anyTempIsUp)
                                {
                                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[{Name}] 안정화 중 신호 소실! 다시 신호를 대기합니다.");
                                    currentState = DimmCheckState.REBOOT; // IDLE이 아닌 WAITING으로 돌아가 신호를 다시 기다림
                                    break;
                                }

                                stabilizationCounter++;
                                if (stabilizationCounter >= STABILIZATION_THRESHOLD)
                                {
                                    basedimmMountCount = CurDimmMountCount;
                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] 안정화 완료. 기준({basedimmMountCount / 2}개) 설정. 감시를 시작합니다.");
                                    currentState = DimmCheckState.MONITORING;
                                }
                                break;

                            // [상태 5: MONITORING]
                            // DIMM 소실을 감시합니다.
                            case DimmCheckState.MONITORING:
                                if (CurDimmMountCount < basedimmMountCount)
                                {
                                    if (Global.gConfigVM.UseBmcReset)
                                        _ = HandleDimmLossAndResetAsync();// 이 함수 실행 후 기다리지 않고 진행
                                }
                                break;
                        }
						if (RackStat != RACK_STAT.RUN && currentState == DimmCheckState.MONITORING)
						{
							currentState = DimmCheckState.IDLE;
							stabilizationCounter = 0;
							basedimmMountCount = 0;
							Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] (MONITORING 중) 상태 강제 초기화(IDLE) 수행");
						}
					}
                    else
                    {
                        _bmc.Start();
                    }
                

                }
                else
                {
                    for (int i = 0; i < Global.gMainPageVM.MaxFixDimmCount; i++, ofs += 2)
                    {

                        int idxH = DimPoslList[i] * 2;
                        DimRegs[idxH].Read = convStr(msg.data, ofs, 2, true);
                        if (_prevDimmTemps[idxH] != 0 && DimRegs[idxH].ReadValue != 0)
                        {
                            double diff = DimRegs[idxH].ReadValue - _prevDimmTemps[idxH];
                            if (Math.Abs(diff) > 10)
                                DimRegs[idxH].ReadValue = _prevDimmTemps[idxH];
                        }
                        _prevDimmTemps[idxH] = DimRegs[idxH].ReadValue;


                        int idxL = DimPoslList[i] * 2 + 1;
                        ofs += 2;
                        DimRegs[idxL].Read = convStr(msg.data, ofs, 2, true);
                        if (_prevDimmTemps[idxL] != 0 && DimRegs[idxL].ReadValue != 0)
                        {
                            double diff = DimRegs[idxL].ReadValue - _prevDimmTemps[idxL];
                            if (Math.Abs(diff) > 10)
                                DimRegs[idxL].ReadValue = _prevDimmTemps[idxL];
                        }
                        _prevDimmTemps[idxL] = DimRegs[idxL].ReadValue;
                    }

                    //                    Console.Write($"i = {i},  ");
                    //                    Console.Write($"DimReg[{DimPoslList[i] * 2}] = {DimRegs[DimPoslList[i] * 2].ReadValue}, ");
                    //                    Console.WriteLine($"DimReg[{DimPoslList[i] * 2 + 1}] = {DimRegs[DimPoslList[i] * 2].ReadValue} ");

                }
                for (int i = 0; i < 8; i++, ofs += 2)
                    TConRegs[RegMap.TCon_GetSV + i].Read = convStr(msg.data, ofs, 2);

                for (int i = 0; i < 8; i++, ofs += 2)
                    TConRegs[RegMap.TCon_GetPV + i].Read = convStr(msg.data, ofs, 2);
                for (int i = 0; i < Global.gConfigVM.CoolerPVs.Count; i++)
                {
                    if (i >= RegMap.MaxCoolerCount) break;
                    int index = Global.gConfigVM.CoolerPVs[i];
                    TConRegs[RegMap.TCon_GetCoolerPV + i].Read = TConRegs[RegMap.TCon_GetPV + index].Read;
                }


                for (int i = 0; i < 8; i++, ofs += 2)
                    TConRegs[RegMap.TCon_GetMV + i].Read = convStr(msg.data, ofs, 2);

                for (int i = 0; i < 8; i++, ofs += 2)
                    TConRegs[RegMap.TCon_GetCHSTS + i].Read = convStr(msg.data, ofs, 2);

                for (int i = 0; i < 8; i++, ofs += 2)
                    TConRegs[RegMap.TCon_GetALM + i].Read = convStr(msg.data, ofs, 2);

                TConRegs[RegMap.TCon_GetRunST].Read = convStr(msg.data, (ofs++), 1);
                //Console.WriteLine($"run : rd {TConRegs[RegMap.TCon_GetRunST].Read}, val {Convert.ToString((int)TConRegs[RegMap.TCon_GetRunST].ReadValue, 16)}");
                TConRegs[RegMap.TCon_GetTuneST].Read = convStr(msg.data, (ofs++), 1);
                //Console.WriteLine($"tune : rd {TConRegs[RegMap.TCon_GetTuneST].Read}, val {Convert.ToString((int)TConRegs[RegMap.TCon_GetTuneST].ReadValue, 16)}");

                ////////////////////////////////////////////////////////////////
                ///// -> cooler 는 control tcon에서 설정하여 받는것으로..
                
                ofs += 16;

                //for (int i = 0; i < 8; i++, ofs += 2)
                //    TConRegs[RegMap.TCon_GetCoolerPV + i].Read = convStr(msg.data, ofs, 2);
                ////////////////////////////////////////////////////////////////




                for (int i = 0; i < 13; i++, ofs += 1)
                {
                    GPIORegs[i].Read = convStr(msg.data, ofs, 1);

                    if (i == RegMap.GPIO_SideClose)
                    {
                        if (SideOpenRun) {
                            if (GPIORegs[i].ReadValue == 0) // close
                            {
                                SideOpenRun = false;
                                startTest();
                                ShowBox = System.Windows.Visibility.Hidden;                            
                            }                            
                        }else // nomal
                        {
                            if(SideOpenDetect != (int)GPIORegs[i].ReadValue)
                            {
                                if (RackStat != RACK_STAT.RUN)
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
                                            showAlram(ALRAM_TYPE.WARN, "", "Slide Open", isLogging: false);
                                    }
                                }
                            }
                            
                        }
                        SideOpenDetect = (int)GPIORegs[i].ReadValue;

                    }
                    
                }

                checkGpio();

                //if (SSRCtrl == 0) SSRCtrl = (byte)((int)GPIORegs[RegMap.GPIO_SSRCtrl].ReadValue);

                if (Global.gMainVM.ShowEngineer == Visibility.Visible && Global.gEnginerVM.CurRack == RackIndex)                    
                    Global.gEnginerVM.setRack();

                processStat();
                checkSystem();

                if (RackStat == RACK_STAT.RUN)
                {                    
                    processTest();
                    sendFPMaxDimm(); 
                    int gindex = GraphIndex;
                    if (Global.gMainVM.ShowDimm == Visibility.Visible && Global.gDimmVM.CurRack == RackIndex)
                        Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => Global.gDimmVM.processRun(gindex)));
                }else
                {
                    foreach (CBoxModel box in CBoxs)
                        box.processBoxDimmInStop();
                   // if (IsSaveOnlyTemp) {
                   //     foreach (CBoxModel box in CBoxs) box.processBoxDimm();
                   //     MaxInRack.Clear();
                   //     double pv = 0;
                   //     foreach (DimDataModel dim in DimDatas) {
                   //         if (pv < dim.Temp) pv = dim.Temp;
                   //         dim.GraphData[GraphIndex] = dim.Temp;
                   //     }
                   //     GraphIndex++;
                   //     addTempLog();
                   //     int gindex = GraphIndex;
                   //     if (Global.gMainVM.ShowDimm == Visibility.Visible && Global.gDimmVM.CurRack == RackIndex)
                   //         Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => Global.gDimmVM.processRun(gindex)));
                   // }
                }

                if (Global.gMainVM.ShowBoard == Visibility.Visible && Global.gBoardVM.CurRack == RackIndex)
                {
                    Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => Global.gBoardVM.setRack(isRead:true)));
                }



            }
            else if (msg.sub.Func == 0x12)
            {
                if (msg.data.Length < 8) return;
                
                int read = msg.data[0] | (msg.data[1] << 8);
                if (read != 0)
                {
                    for (int i = 0; i<5; i++)
                    {
                        int comp = 1 << i;
                        if (comp != 0)
                            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{Name}] : Fan I2C CH({i}) Error");
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
                            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{Name}] : Cur I2C CH({i}) Error");
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
                            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{Name}] : Temp I2C CH({i}) Error");
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
                            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{Name}] : TCON RS485 CH({i}) Error");
                    }
                    RackStat = RACK_STAT.INSPECTION;
                }

            }
        }
		private async Task HandleDimmLossAndResetAsync()
        {
			try
			{
				// 1. 에러 로그를 기록합니다.
				Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[{Name}] DIMM 소실 감지! 시스템 리셋 절차를 시작합니다.");

				// 2. 먼저 안전한 IDLE 상태로 전환합니다.
				currentState = DimmCheckState.IDLE;

				// 리셋 명령을 딱 한 번만 보냅니다.
				var (status, responseBody) = await _bmc.Reset();
				// 로그만 남기고, 무조건 다음 상태로 진행합니다.
				if (status == SESSION_STATUS.OK)
				{
					Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] BMC가 리셋 명령을 성공적으로 수신했습니다.");
				}
				else
				{
					Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[{Name}] BMC로부터 리셋 명령 수신 응답을 받지 못했습니다. (응답: {responseBody})");
				}

				// 4. 리셋이 완료되었으므로, 이제 "RESET_TRIGGERED" 상태로 전환합니다.
				Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{Name}] 실제 DIMM 초기화를 감시합니다.");
				_resetTriggerTimeoutCounter = 0;
				currentState = DimmCheckState.RESET_TRIGGERED;
			}
			catch (Exception ex)
			{
				// 비동기 작업 중 발생하는 모든 예외를 여기서 처리하여 프로그램 전체가 죽는 것을 방지합니다.
				Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[{Name}] 리셋 작업 중 심각한 오류 발생: {ex.Message}");
				// 에러 발생 시 안전하게 IDLE 상태로 복귀
				currentState = DimmCheckState.IDLE;
			}
		}

		public void doRecv(HtcMessage msg)
        {
            REG_TYPE type = MConv.FindEnumValue<REG_TYPE>((int)msg.Header.Type);

            if (type != REG_TYPE.PDO)
            {
                if (Global.gMainVM.ShowEngineer == Visibility.Visible && Global.gEnginerVM.CurRack == RackIndex)
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
                    Global.gEnginerVM.pushCmdLog(new CommandLog(RackIndex, res, sb.ToString()));
                }else if (Global.gMainVM.ShowConfig == Visibility.Visible && Global.gConfigVM.CurRack == RackIndex)
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
            }catch (Exception e)
            {
                Console.WriteLine (e.Message);
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{Name}] : RECV Msg {e.Message}");
            }
        }

        public void connected (bool isCon)
        {
            if (isCon)
            {
                RackStat = RACK_STAT.STOP;
                HtcMessage msg = Global.newMsg(0xff, 0, 0x12, 8);
                _client.pushSendQue(REG_TYPE.PDO, msg);
                Console.WriteLine($"{Name}: Connected!");
                reqFanDuty();


                msg = Global.newMsg(0xff, 0, 0x21);
                short dimCnt = (short)Global.gMainPageVM.MaxDimmCount;
                msg.data = BitConverter.GetBytes(dimCnt);
                _client.pushSendQue(REG_TYPE.DIMM, msg);


                msg = Global.newMsg(0xff, 0, 0x21);
                byte ssrCold = Convert.ToByte(Convert.ToInt32(Global.gConfigVM.SSRCold, 16));
                msg.data = BitConverter.GetBytes(ssrCold);
                _client.pushSendQue(REG_TYPE.DIMM, msg);

                sendFPSV(SetSV);
                sendFPMap(
                  CBoxs[0].BoxSubs[0].FPIndex,
                  CBoxs[1].BoxSubs[0].FPIndex,
                  CBoxs[2].BoxSubs[0].FPIndex);


            }
            else
            {
                RackStat = RACK_STAT.DISCONNECT;
                if (Global.gConfigVM.isUseBMC)
                    _bmc.Stop();
            }

         
        }

        #endregion

        #region Send Command 
        public void sendFanDuty(int index, int percent) // 0~7 TempBox, 8~14 ETC
        {
            int value = (percent * 256) / 100;
            if (value > 0xff)value = 0xff;
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

        public void reqFanDuty()
        {
            if (Global.gEnginerVM == null) return;
            var msg = Global.newMsg(0xff, 0, 0x11, 15);
            int index = Global.gEnginerVM.CurRack;
            Global.gMainPageVM.Racks[index]._client.pushSendQue(REG_TYPE.FAN, msg);
        }

        public void reqTConBS1()
        {
            var msg = Global.newMsg(0xff, 0, 0x31, 2);
            int index = Global.gEnginerVM.CurRack;
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
        public void sendFPMaxDimm()
        {
            if (Global.gConfigVM.FPDisplay != 1) return;

            HtcMessage msg = Global.newMsg(0xff, 0, 0x21, 12);
            msg.data = new byte[12];
            int idx = 0;
            foreach (var box in CBoxs)
            {
                msg.data[idx++] = 0;                        // 0    
                msg.data[idx++] = (byte)(box.MaxInBoxValue / 10);          // 10
                msg.data[idx++] = (byte)(box.MaxInBoxValue % 10);          // 1
                msg.data[idx++] = (byte)((box.MaxInBoxValue * 10) % 10);   // 0.1
                if (idx >= 12) break;                
            }
            _client.pushSendQue(REG_TYPE.FP, msg);
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

        public void sendFPMap(int b1, int b2, int b3)
        {

            HtcMessage  msg = Global.newMsg(0xff, 0, 0x11, 4);
            msg.data = new byte[4];
            msg.data[0] = (byte)b1;
            msg.data[1] = (byte)b2;
            msg.data[2] = (byte)b3;
            msg.data[3] = (byte)0;

            _client.pushSendQue(REG_TYPE.FP, msg);
        }

        #endregion



    }
}
