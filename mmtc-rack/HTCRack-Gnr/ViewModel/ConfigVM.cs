using HTCRack.Base;
using HTCRack.Model;
using HTCRack.Net;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Security.Permissions;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Threading;
using System.Xml;

using FolderBrowserDialog = System.Windows.Forms.FolderBrowserDialog;
using DialogResult = System.Windows.Forms.DialogResult;
using PropertyChanged;
using System.Xml.Linq;
using System.Collections.ObjectModel;
using System.Runtime.InteropServices;
using System.Windows.Controls;
using System.Linq.Expressions;
using Google.Protobuf.WellKnownTypes;
using System.Windows.Markup;

namespace HTCRack.ViewModel
{
    public enum FPGA_FLAG
    {
        DONE = 0,
        SEND = 1,
        OK = 2,
        ERROR = 3
    }

    public enum THREAD_ARG
    {
        STOPED = 0,
        CLEAR,
        PROGRAM
    }

    public enum CONTROL_MODE : int
    {
        NORMAL = 0,
        NOSV,
        ML,
        MANUAL,
        MMLOGIC
        

    }

    

    public class ConfigVM :BaseVM
    {

        public int ControlMode { get; set; } = (int) CONTROL_MODE.NORMAL;
        public int TempCollectMode { get; set; } = 0;
        [DoNotNotify]
        public int FPDisplay { get; set; } = 0; // 0 = pv display, 1 = dimm max display

        public int TestStartMode { get; set; } = 0;

        #region File Path - hibim, Hdiag, ...

        public string TamsSVPath { get; set; }
        public string TamsSVFolder { get; set; }
        public string TamsSVFile { get; set; }
        public string TamsEndPath { get; set; }
        public string TamsEndFolder { get; set; }
        public string TamsEndFile { get; set; }
        public bool SaveEXTemp { get; set; } // H / L file 저장 할지여부
        public bool SaveWaterTemp { get; set; }
        #endregion

        #region Log 관련

        public string SysLogPath { get; set; }
        public string TempLogPath { get; set; }
        public ObservableCollection<int> LogRetentionMonths { get; set; } = new ObservableCollection<int>();
        public int SelectRetention { get; set; } = 2;
        DispatcherTimer _retentionCheckTimer = new DispatcherTimer();
        #endregion

        #region FPGA Item

        int _curRack = 0;
        public int CurRack { get => _curRack; set { _curRack = value; selectRack(); } }
        public string EquipImg { get; set; } = "/Resource/equip.png";

        const int PDO_PROM_SIZE = (16 * 1024 * 1024);
        const int PDO_PROM_ERASE_SIZE = (16 * 1024);
        const int PDO_PROM_ADDR_SIZE = 4;
        const int PDO_PROM_CFG_SIZE = 128;
        public string FpgaVer { get; set; } = "";
        public string FpgaPath { get; set; }
        public DelegateCommand DGCmdFPGA { get; set; }
        public DelegateCommand DGCmdFile { get; set; }

        public int SendProg { get; set; } = 0;
        public SolidColorBrush ProgColor { get; set; }
        public FPGA_FLAG SendFlag { get; set; } = FPGA_FLAG.DONE;
        public THREAD_ARG ThreadArg { get; set; } = THREAD_ARG.STOPED;
        #endregion

        #region Error Condition Item

        public double MaxDimm { get; set; }
        public double MinDimm { get; set; }
        public int StableTime { get; set; }
        public double DimmLimit { get; set; }
        public int PeriodLog { get; set; }

        public double AlarmHighLimit { get; set; }
        public double AlarmLowLimit { get; set; }
        public int PeriodAlarm { get; set; }

        public double MinSetSV {  get; set; }
        public double MaxSetSV {  get; set; }

        public int NoDimmCheckCount { get; set; }


        [DoNotNotify]
        public bool UseMaxCtrl { get; set; }
        [DoNotNotify]
        public double MaxCtrlBase { get; set; }
        [DoNotNotify]
        public double MaxCtrlStep { get; set; }


        public double WaterOver { get; set; } = 0;
        public double FanDuty { get; set; }
        public double FanRpmOver { get; set; }
        public double FanRpmUnder { get; set; }
        public double WaterpumpCurUnder { get; set; }
        public double WaterpumpCurOver { get; set; }
        public double RadiatorfanCurUnder { get; set; }
        public double RadiatorfanCurOver { get; set; }
        public double PaltierCurOver { get; set; }
        public double FetTempUnder { get; set; }        
        public double FetTempWarn { get; set; }
        public double FetTempOver { get; set; }

        #endregion

        #region GPIO Item        
        public string SSRIn { get; set; }
        public string SSROut { get; set; }
        public string SSRCH { get; set; }
        public string SSREN { get; set; }
        public string SSRCold { get; set; } = "0";

        public string SSRFreq { get; set; } = "0";

        public string ExtraPump { get; set; }
        public string ExtraRad { get; set; }
        public string ExtraFan { get; set; }

        #endregion

        [DoNotNotify]
        public int NoDimmBoxStopMin { get; set; } = 10;


        #region MongoDB
        [DoNotNotify]
        public bool IsUseMongo { get; set; }
        [DoNotNotify]
        public string UIName { get; set; }

        [DoNotNotify]
        public string MongoIP { get;set; }
        [DoNotNotify]
        public int MongoPort { get; set; }
        [DoNotNotify]
        public string MongoUser { get; set; }
        [DoNotNotify]
        public string MongoPW { get; set; }
        [DoNotNotify]
        public string MongoAuth { get; set; }
        #endregion

        #region PLC
        [DoNotNotify]
        public bool IsUsePlc { get; set; }
        [DoNotNotify]
        public string PlcIP { get; set; }
        [DoNotNotify]
        public int PlcPort { get;set; }
        [DoNotNotify]
        public int PlcRackCount { get; set; } = 6;

        #endregion

        #region TCon Param
        public string TconPramText { get; set; } = "";

        public ListView ListTconResp;
        public ObservableCollection<CommandLog> TconResps { get; set; } = new ObservableCollection<CommandLog>();
        public void pushTconResp(CommandLog resp)
        {
            Application.Current.Dispatcher.BeginInvoke(new Action(() => {
                if (TconResps.Count > 100)
                    TconResps.RemoveAt(0);
                TconResps.Add(resp);

                ListTconResp.ScrollIntoView(resp);
            }));
        }

        public void clearTconResp()
        {
            Application.Current.Dispatcher.BeginInvoke(new Action(() => { TconResps.Clear(); }));   
        }


        public void loadTconParam()
        {
            try
            {
                TconPramText = File.ReadAllText("tcon.param");
            }
            catch { return; }
        }

        public void saveTconParam()
        {
            try {
                File.WriteAllText("tcon.param", TconPramText);
                  
            }catch { return; }
        }


        public void sendTconParam(RackModel rack)
        {
            char[] delims = new[] { '\r', '\n' };
            string[] strings = TconPramText.Split(delims, StringSplitOptions.RemoveEmptyEntries);

            
            foreach (var str in strings)
            {
                try
                {
                    if (str.Length == 0) continue;
                    int pos = str.IndexOf('#');
                    if (pos == 0) continue;
                    string line = str;
                    if (pos != -1)
                        line = str.Substring(0, pos - 1);

                    string[] words = line.Split();
                    int wordCnt = 0;
                    foreach (var word in words)
                        if (word != "") wordCnt++;

                    if (words.Length < 2) continue;
                    int first = Convert.ToInt32(words[0]);
                    int second = Convert.ToInt32(words[1]);
                    
                    int chan = (first & 0xF) << 4 | (second & 0xF00) >> 8;
                    int addr = second & 0xFF;
                    string type = "SET";
                    string data = "";
                    if (wordCnt > 2 ) // write 
                    {
                        data = " -> " + words[2];
                        short value = Convert.ToInt16(words[2]);
                        value *= 10;

                        HtcMessage msg = Global.newMsg(chan, addr, 0);
                        msg.data = BitConverter.GetBytes(value);
                        rack._client.pushSendQue(REG_TYPE.TCON, msg);
                    }
                    else  // read
                    {
                        type = "GET";
                        HtcMessage msg = Global.newMsg(chan, addr, 1, 2);
                        rack._client.pushSendQue(REG_TYPE.TCON, msg);
                    }


                    pushTconResp(new CommandLog(rack.RackIndex, CMD_TYPE.SEND, $"{type} CH[{first}] {second}{data}"));
                    Thread.Sleep(500);
                }catch (Exception e)
                {
                    pushTconResp(new CommandLog(rack.RackIndex, CMD_TYPE.ERROR, $"\'{str}\' - Text Format Error"));
                    break;
                }

            }
        }

        public bool isTconCmdEnable { get; set; } = true;

        public async void  pushTconParam()
        {
            RackModel rack = Global.gMainPageVM.getRack(CurRack);
            if (!(rack.RackStat == RACK_STAT.STOP || rack.RackStat == RACK_STAT.DONE))
            {
                MessageBox.Show($"Can't push TCon Param. Check {rack.Name} Stat");
                return;
            }
            clearTconResp();
            rack._client._isPushing = true;
            isTconCmdEnable = false;
            await Task.Run(() => sendTconParam(rack));
            isTconCmdEnable = true;
            rack._client._isPushing = false;
        }

        public DelegateCommand DGCTconReload { get; set; }
        public DelegateCommand DGCTconSave {get;set;}
        public DelegateCommand DGCTconPush { get; set; }
        #endregion

        #region
        [DoNotNotify]
        public bool isUseBMC { get; set; } = false;



        public List<string> BMCSubPath { get; set; } = new List<string>();
        public List<string> BMCSubPathCPU { get; set; } = new List<string>();




        #endregion

        public List<int> CoolerPVs = new List<int>();

        public string GetPassword { get; set; }
        public string SetPassword { get; set; }
        public bool IsUsePassword { get; set; }

        public bool CorrectPassword { get; set; } = false;

        public bool IsMaxLimitTarget { get; set; } = false;

        public double CtrlSVMin { get; set; } = 0;
        public double CtrlSVMax { get; set; } = 100;

        public ConfigVM() {
            for (int i = 1; i <= 12; i++)
                LogRetentionMonths.Add(i);

            DGCTconReload = new DelegateCommand(p => loadTconParam());
            DGCTconSave = new DelegateCommand(p => saveTconParam());
            DGCTconPush = new DelegateCommand(p => pushTconParam());

            try
            {
                initItems();
                loadTconParam();
            }
            catch (Exception ex)
            {
                HandyControl.Controls.MessageBox.Show("HTCRack.xml Load Error : " +
                    ex.Message);
            }

            Global.gConfigVM = this;
            Global.gMainPageVM.initLogFile();
            //Global.gDimmVM.setMinMax();

            if (Global.gMainPageVM.LayoutColumnCount == 1)
                EquipImg = "/Resource/equip_col1.png";
            else if (Global.gMainPageVM.LayoutColumnCount == 2)
                EquipImg = "/Resource/equip.png";

            DGCmdFPGA = new DelegateCommand(p => cmdFpga((string)p));
            DGCmdFile = new DelegateCommand(p => cmdFilePath((string)p));
            ProgColor = Global.gFPGAProgColors[(int)THREAD_ARG.STOPED];

            if (IsUseMongo)
                Global.gMainVM.startDBThread();
            if (IsUsePlc)
                Global.gMainVM.startPlcThread();

            _retentionCheckTimer.Interval = TimeSpan.FromHours(1);
            _retentionCheckTimer.Tick += checkRetention_tick;

        }

        public void checkRetention_tick(object sender, EventArgs e)
        {
            removeLogFiles(SysLogPath, ".log");
            removeLogFiles(TempLogPath, ".temp");
            _retentionCheckTimer.Interval = TimeSpan.FromHours(24);
        }

        

        public void initItems()
        {
            XmlDocument doc = Global.gConfigDoc;

            XmlNode node = doc.SelectSingleNode("/htc-rack/rack-map");            
            IsMaxLimitTarget = Convert.ToInt32(node.Attributes["svmax"].Value) == 1 ? true : false;
            CtrlSVMin = Convert.ToDouble(node.Attributes["ctrl-svmin"].Value);
            CtrlSVMax = Convert.ToDouble(node.Attributes["ctrl-svmax"].Value);

            ControlMode = Convert.ToInt32(node.Attributes["svmode"].Value);
            if (node.Attributes["fpmax"] != null)
                FPDisplay = Convert.ToInt32(node.Attributes["fpmax"].Value);
            // Fpga Load
            node = doc.SelectSingleNode("/htc-rack/fpga");
            FpgaPath = node.Attributes["path"].Value;

            // File Path
            node = doc.SelectSingleNode("/htc-rack/log");
            SaveEXTemp = Convert.ToInt32(node.Attributes["save-ex"].Value) == 1 ? true : false;
            SaveWaterTemp = Convert.ToInt32(node.Attributes["save-water"].Value) == 1 ? true : false;
            SelectRetention = Convert.ToInt32(node.Attributes["retention"].Value);

            node = doc.SelectSingleNode("/htc-rack/log/event");
            SysLogPath = node.Attributes["path"].Value;
            node = doc.SelectSingleNode("/htc-rack/log/temp");
            TempLogPath = node.Attributes["path"].Value;

            node = doc.SelectSingleNode("/htc-rack/tams");
            if (node.Attributes["auto"] != null)
                TestStartMode = Convert.ToInt32(node.Attributes["auto"].Value);

            node = doc.SelectSingleNode("/htc-rack/tams/set-temp");
            TamsSVPath = node.Attributes["path"].Value;
            TamsSVFolder = node.Attributes["folder"].Value;
            TamsSVFile = node.Attributes["name"].Value;

            node = doc.SelectSingleNode("/htc-rack/tams/end-flag");
            TamsEndPath = node.Attributes["path"].Value;
            TamsEndFolder = node.Attributes["folder"].Value;
            TamsEndFile = node.Attributes["name"].Value;


            // GPIO 
            node = doc.SelectSingleNode("/htc-rack/gpio/ssr");            
            SSRIn =     node.Attributes["in"].Value;
            SSROut =    node.Attributes["out"].Value;
            SSRCH =     node.Attributes["ch"].Value;
            SSREN =     node.Attributes["en"].Value;
            SSRCold =   node.Attributes["cold"].Value;
            SSRFreq =   node.Attributes["freq"].Value;

            node = doc.SelectSingleNode("/htc-rack/gpio/extra");            
            ExtraPump = node.Attributes["pump"].Value;
            ExtraRad =  node.Attributes["rad"].Value;
            ExtraFan = node.Attributes["fan"].Value;

            // Dimm
            node = doc.SelectSingleNode("/htc-rack/dimm");
            MaxDimm = Convert.ToDouble(node.Attributes["max"].Value);
            MinDimm = Convert.ToDouble(node.Attributes["min"].Value);
            StableTime = Convert.ToInt32(node.Attributes["stable"].Value);
            DimmLimit = Convert.ToDouble(node.Attributes["limit"].Value);
            PeriodLog = Convert.ToInt32(node.Attributes["per-log"].Value);
            AlarmHighLimit = Convert.ToDouble(node.Attributes["alarmH"].Value);
            AlarmLowLimit = Convert.ToDouble(node.Attributes["alarmL"].Value);
            PeriodAlarm = Convert.ToInt32(node.Attributes["per-alarm"].Value);
            NoDimmCheckCount = Convert.ToInt32(node.Attributes["no-dimm-check"].Value);
            MinSetSV = Convert.ToDouble(node.Attributes["SVMin"].Value);
            MaxSetSV = Convert.ToDouble(node.Attributes["SVMax"].Value);
            if (node.Attributes["box-stop-min"] != null)            
                NoDimmBoxStopMin = Convert.ToInt32(node.Attributes["box-stop-min"].Value);
            


            // Dimm / Limit
            node = doc.SelectSingleNode("/htc-rack/dimm/limit");
            UseMaxCtrl = Convert.ToInt32(node.Attributes["use"].Value) == 1 ? true : false;
            MaxCtrlBase = Convert.ToDouble(node.Attributes["baseline"].Value);
            MaxCtrlStep = Convert.ToDouble(node.Attributes["step"].Value);

            // etc
            node = doc.SelectSingleNode("/htc-rack/water");
            WaterOver = Convert.ToDouble(node.Attributes["over"].Value);            
            string text = node.InnerText;
            string[] buf = text.Split(',');
            foreach (string s in buf)
            {                
                string str = s.Trim();
                CoolerPVs.Add(Convert.ToInt32(str));                
            }

            node = doc.SelectSingleNode("/htc-rack/fan");
            FanDuty = Convert.ToDouble(node.Attributes["duty"].Value);
            FanRpmUnder = Convert.ToDouble(node.Attributes["under"].Value);
            FanRpmOver = Convert.ToDouble(node.Attributes["over"].Value);

            //cur
            node = doc.SelectSingleNode("/htc-rack/cur/utility-cur/Water-Pump");
            WaterpumpCurUnder = Convert.ToDouble(node.Attributes["under"].Value);
            WaterpumpCurOver = Convert.ToDouble(node.Attributes["over"].Value);
            node = doc.SelectSingleNode("/htc-rack/cur/utility-cur/Radiator-Fan");
            RadiatorfanCurUnder = Convert.ToDouble(node.Attributes["under"].Value);
            RadiatorfanCurOver = Convert.ToDouble(node.Attributes["over"].Value);
            node = doc.SelectSingleNode("/htc-rack/cur/paltier-cur");
            PaltierCurOver = Convert.ToDouble(node.Attributes["over"].Value);

            node = doc.SelectSingleNode("/htc-rack/fet-temp");
            FetTempUnder = Convert.ToDouble(node.Attributes["under"].Value);
            FetTempWarn = Convert.ToDouble(node.Attributes["warn"].Value);
            FetTempOver = Convert.ToDouble(node.Attributes["over"].Value);

            // Mongo
            node = doc.SelectSingleNode("/htc-rack/mongo");
            IsUseMongo = Convert.ToInt32(node.Attributes["use"].Value) == 1 ? true : false;
            UIName = node.Attributes["name"].Value;
            MongoIP = node.Attributes["ip"].Value;
            MongoPort = Convert.ToInt32(node.Attributes["port"].Value);
            MongoUser = node.Attributes["user"].Value;
            MongoPW = node.Attributes["pw"].Value;
            MongoAuth = node.Attributes["auth"].Value;

            // PLC
            node = doc.SelectSingleNode("/htc-rack/plc");
            IsUsePlc = Convert.ToInt32(node.Attributes["use"].Value) == 1 ? true : false;
            PlcIP = node.Attributes["ip"].Value;
            PlcPort = Convert.ToInt32(node.Attributes["port"].Value);
            PlcRackCount = Convert.ToInt32(node.Attributes["racks"].Value);


            // Password
            node = doc.SelectSingleNode("/htc-rack/password");
            IsUsePassword = Convert.ToInt32(node.Attributes["use"].Value) == 1 ? true : false;
            SetPassword = node.Attributes["value"].Value;

            // Setup Page Using
            node = doc.SelectSingleNode("/htc-rack/setup");
            if (node != null)
            {
                Global.gMainVM.IsUseSetup = Convert.ToInt32(node.Attributes["use"].Value) == 1 ? Visibility.Visible : Visibility.Hidden;
            }

            // Redfish BMC 
            node = doc.SelectSingleNode("/htc-rack/redfish");
            isUseBMC = Convert.ToInt32(node.Attributes["use"].Value) == 1 ? true : false;
            if (isUseBMC)
            {
                node = doc.SelectSingleNode("/htc-rack/redfish/auth");
                string user = node.Attributes["user"].Value;
                string pwd = node.Attributes["pw"].Value;

                node = doc.SelectSingleNode("/htc-rack/redfish/path");
                string main = node.Attributes["main"].Value;
                string login = node.Attributes["login"].Value;
                string subPath = node.InnerXml;
                string[] subBuf = subPath.Split(',');
                List<string> subPathList = new List<string>();
                List<string> subPathListcpu = new List<string>();
                foreach (string s in subBuf)
                {
                    string str = s.Trim();


                    string s1 = str.Substring(0, 1);
                    string s2 = str.Substring(2);

                    subPathList.Add(main + s1 + "1_CPU" +s2 );
                    subPathList.Add(main + s1 + "2_CPU" + s2);

                }

                foreach (RackModel rack in Global.gMainPageVM.Racks)
                {
                    rack.initBmc(user, pwd, main, login, subPathList);
                }
                
            }



        }

        public void saveSV(int index, double sv)
        {
            XmlDocument doc = Global.gConfigDoc;
            XmlNode node = doc.SelectSingleNode("/htc-rack/rack-map");
            XmlNode rackNode = node.SelectSingleNode($"//rack[@id='{index}']");
            rackNode.Attributes["prev-sv"].Value = sv.ToString();
            Global.saveConfig();
        }

        public void saveItems()
        {
            XmlDocument doc = Global.gConfigDoc;

            XmlNode node = doc.SelectSingleNode("/htc-rack/rack-map");
            node.Attributes["svmode"].Value = ControlMode.ToString();

            int index = 0;
            foreach (XmlNode rackNode in node.ChildNodes) {
                if (rackNode.Name != "rack") continue;

                RackModel rack = Global.gMainPageVM.getRack(index++);
                rackNode.Attributes["as"].Value = rack.IsAS ? "1" : "0";
                rackNode.Attributes["prev-sv"].Value = rack.SetSV.ToString();
            }


            // Fpga Load
            node = doc.SelectSingleNode("/htc-rack/fpga");
            node.Attributes["path"].Value = FpgaPath;



            // File Path
            node = doc.SelectSingleNode("/htc-rack/log");
            node.Attributes["retention"].Value = SelectRetention.ToString();
            node = doc.SelectSingleNode("/htc-rack/log/event");
            node.Attributes["path"].Value = SysLogPath;
            node = doc.SelectSingleNode("/htc-rack/log/temp");
            node.Attributes["path"].Value = TempLogPath;
            node = doc.SelectSingleNode("/htc-rack/tams");
            node.Attributes["all"].Value = Global.gMainPageVM.IsTamsAll?"1":"0";

            node = doc.SelectSingleNode("/htc-rack/tams");            
            node.Attributes["auto"].Value = TestStartMode.ToString();

            node = doc.SelectSingleNode("/htc-rack/tams/set-temp");
            node.Attributes["path"].Value = TamsSVPath;
            node.Attributes["folder"].Value = TamsSVFolder;
            node.Attributes["name"].Value = TamsSVFile;
            node = doc.SelectSingleNode("/htc-rack/tams/end-flag");
            node.Attributes["path"].Value = TamsEndPath;
            node.Attributes["folder"].Value = TamsEndFolder;
            node.Attributes["name"].Value = TamsEndFile;


            // Gpio
            node = doc.SelectSingleNode("/htc-rack/gpio/ssr");            
            node.Attributes["in"].Value     = SSRIn;
            node.Attributes["out"].Value    = SSROut;
            node.Attributes["ch"].Value     = SSRCH;
            node.Attributes["en"].Value     = SSREN;
            node.Attributes["cold"].Value   = SSRCold;
            node.Attributes["freq"].Value   = SSRFreq;

            node = doc.SelectSingleNode("/htc-rack/gpio/extra");            
            node.Attributes["pump"].Value = ExtraPump;
            node.Attributes["rad"].Value = ExtraRad;
            node.Attributes["fan"].Value = ExtraFan;

            // Dimm
            node = doc.SelectSingleNode("/htc-rack/dimm");
            node.Attributes["max"].Value = MaxDimm.ToString();
            node.Attributes["min"].Value = MinDimm.ToString();
            node.Attributes["stable"].Value    = StableTime.ToString();
            node.Attributes["limit"].Value     = DimmLimit.ToString();
            node.Attributes["per-log"].Value   = PeriodLog.ToString();
            node.Attributes["alarmH"].Value    = AlarmHighLimit.ToString();
            node.Attributes["alarmL"].Value    = AlarmLowLimit.ToString();
            node.Attributes["per-alarm"].Value = PeriodAlarm.ToString();
            node.Attributes["SVMin"].Value = MinSetSV.ToString();
            node.Attributes["SVMax"].Value = MaxSetSV.ToString();

            // etc
            node = doc.SelectSingleNode("/htc-rack/water");
            node.Attributes["over"].Value = WaterOver.ToString();

            node = doc.SelectSingleNode("/htc-rack/fan");
            node.Attributes["duty"].Value =  FanDuty.ToString();
            node.Attributes["under"].Value = FanRpmUnder.ToString();
            node.Attributes["over"].Value = FanRpmOver.ToString();

            //cur
            node = doc.SelectSingleNode("/htc-rack/cur/utility-cur/Water-Pump");
            node.Attributes["under"].Value = WaterpumpCurUnder.ToString();
            node.Attributes["over"].Value = WaterpumpCurOver.ToString();
            node = doc.SelectSingleNode("/htc-rack/cur/utility-cur/Radiator-Fan");
            node.Attributes["under"].Value = RadiatorfanCurUnder.ToString();
            node.Attributes["over"].Value = RadiatorfanCurOver.ToString();
            node = doc.SelectSingleNode("/htc-rack/cur/paltier-cur");
            node.Attributes["over"].Value = PaltierCurOver.ToString();

            node = doc.SelectSingleNode("/htc-rack/fet-temp");
            node.Attributes["over"].Value = FetTempOver.ToString();

            //Password
            node = doc.SelectSingleNode("/htc-rack/password");
            node.Attributes["use"].Value = (IsUsePassword == true ? 1 : 0).ToString();
            node.Attributes["value"].Value = SetPassword.ToString();

            Global.saveConfig();
        }

        public void changeTempCollect(DIM_TYPE type)
        {
            var node = Global.gConfigDoc.SelectSingleNode("htc-rack/rack-map/rack");

            foreach (var rack in Global.gMainPageVM.Racks)
            {

                var rackNode = node.SelectSingleNode($"//rack[@id='{rack.RackIndex}']");
                int boxIndex = 0;
                foreach (XmlNode boxNode in rackNode.ChildNodes)
                {
                    int subIndex = 0;
                    CBoxModel box = rack.CBoxs[boxIndex];
                    foreach (XmlNode subNode in boxNode.ChildNodes)
                    {
                        if (subNode.Name != "sub") continue;
                        CBoxSub sub = box.BoxSubs[subIndex];
                        sub._dimType = type;
                        var itemNode = subNode.SelectSingleNode("dim");
                        itemNode.Attributes["type"].Value = type.ToString();
                        subIndex++;
                    }
                    boxIndex++;
                }
            }
        }

        public void checkPassword()
        {
            CorrectPassword = (SetPassword == GetPassword)? true: false;
        }

        public void sendVerInfo()
        {
            RackModel rack = Global.gMainPageVM.getRack(CurRack);
            HtcMessage msg = Global.newMsg(0xff, 0, 0x10, 32);            
            rack._client.pushSendQue(REG_TYPE.PDO, msg);
        }
        public void selectRack()
        {
            FpgaVer = "";
            ThreadArg = THREAD_ARG.STOPED;
            SendFlag = FPGA_FLAG.DONE;

            // UI Thread
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => { 
                        SendProg = 0; 
                        ProgColor = Global.gFPGAProgColors[(int)THREAD_ARG.STOPED]; 
                    }));

            sendVerInfo(); 

        }

        //        public Thread _fpgaThread = new Thread(SendFPGA);

        public void SendFPGA()
        {

            RackModel rack = Global.gMainPageVM.getRack(CurRack);

            if (ThreadArg == THREAD_ARG.CLEAR)
            {
                SendFlag = FPGA_FLAG.OK;
                int index = 0;
                while (true)
                {
                    // Stoped
                    if (ThreadArg == THREAD_ARG.STOPED)
                        return;
                    // Send Fail
                    if (SendFlag == FPGA_FLAG.ERROR)
                    {
                        Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
                        {                            
                            ProgColor = Global.gFPGAProgColors[3];// Error Color
                        }));
                        ThreadArg = THREAD_ARG.STOPED;
                        return;
                    }
                    // Wait OK
                    if (SendFlag == FPGA_FLAG.SEND)
                    {
                        //Thread.Sleep(1);
                        continue;
                    }
                    // Done
                    if (index >= (PDO_PROM_SIZE / PDO_PROM_ERASE_SIZE))
                    {                        
                        break;
                    }

                    int prom_addr = 0x10000 * index;
                    HtcMessage msg = Global.newMsg(0xff, 0x00, 0x20, 4);                    
                    msg.data = BitConverter.GetBytes(prom_addr);
                    SendFlag = FPGA_FLAG.SEND;
                    rack._client.pushSendQue(REG_TYPE.PDO, msg);

                    index++;

                    if (index % 4== 0)
                    {
                        Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
                        {
                            SendProg = (int)((float)index / (PDO_PROM_SIZE / PDO_PROM_ERASE_SIZE) * 100);
                            ProgColor = Global.gFPGAProgColors[(int)THREAD_ARG.CLEAR];
                        }));
                    }

                }
            }
            else if (ThreadArg == THREAD_ARG.PROGRAM)
            {
                SendFlag = FPGA_FLAG.OK;

                using (FileStream fs = new FileStream(FpgaPath, FileMode.Open))
                {
                    int index = 0;
                    int size = (int)fs.Length;                    
                    int ofs = 0;
                    while (true)
                    {
                        // Stoped
                        if (ThreadArg == THREAD_ARG.STOPED)
                            return;
                        // Send Fail
                        if (SendFlag == FPGA_FLAG.ERROR)
                        {
                            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
                            {
                                ProgColor = Global.gFPGAProgColors[3];// Error Color
                            }));
                            ThreadArg = THREAD_ARG.STOPED;
                            return;
                            
                        }
                      //  Console.WriteLine($"size:{size} offset:{ofs}");
                        // Wait OK
                        if (SendFlag == FPGA_FLAG.SEND)
                        {                            
                            continue;
                        }
                      //  Console.WriteLine("send ok");
                        // Done
                        if (ofs >= size)
                        {
                            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
                            {
                                SendProg = 100;
                                ProgColor = Global.gFPGAProgColors[(int)THREAD_ARG.STOPED];
                            }));
                            SendFlag = FPGA_FLAG.DONE;
                            break;
                        }

                        int rdsz = 128;
                        if (ofs + 128 > size) // Last
                            rdsz = size - ofs; 
                        byte[] buf = new byte[rdsz+10];
                        fs.Read(buf, 0, rdsz);
                        byte[] adrs = BitConverter.GetBytes(ofs);

                        HtcMessage msg =  Global.newMsg(0xff, 0, 0x21);
                        msg.data = MConv.GetCombined(adrs, buf);                        

                        SendFlag = FPGA_FLAG.SEND;
                        rack._client.pushSendQue(REG_TYPE.PDO, msg);

                        ofs += rdsz;

                        if (index % 100 == 0)
                        {
                            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
                            {
                                SendProg = (int)((float)ofs / size * 100);
                                ProgColor = Global.gFPGAProgColors[(int)THREAD_ARG.PROGRAM];
                            }));
                        }
                        index++;

                    }
                }
              
            }
        }

        
        public async void cmdFpga(string name)
        {
            RackModel rack = Global.gMainPageVM.getRack(CurRack);


            if (name == "upload")         
            {
                if (rack.RackStat != RACK_STAT.STOP)
                {
                    MessageBox.Show($"Fixture Stat is Not Stop.. Check Fix Stat", "Warning", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return;
                }
                if (ThreadArg == THREAD_ARG.STOPED)
                {
                    //Clear
                    ThreadArg = THREAD_ARG.CLEAR;          
                    rack._client._isUploading = true;
                    await Task.Run(() => SendFPGA());
                    rack._client._isUploading = false;
                    if (ThreadArg == THREAD_ARG.STOPED) return;

                    //Program
                    ThreadArg = THREAD_ARG.PROGRAM;
                    rack._client._isUploading = true;
                    await Task.Run(() => SendFPGA());
                    rack._client._isUploading = false;
                    if (ThreadArg == THREAD_ARG.STOPED) return;
                   

                    //Reboot
                    HtcMessage msg = Global.newMsg(0xff, 0, 0x22);
                    //msg.data = BitConverter.GetBytes((int)0);
                    rack._client.pushSendQue(REG_TYPE.PDO, msg);
                    //n sec wait after reboot, and get Version
                    await Task.Delay(5000);
                    sendVerInfo();
                    ThreadArg = THREAD_ARG.STOPED;
                }
                
            }
            else if (name == "load")
            {
                if (ThreadArg != THREAD_ARG.STOPED)
                {
                    MessageBox.Show("Now File is Using.");
                    return;
                }

                string filter = "BIT File(*.bit)|*.bit|ALL Files(*.*)|*.*";
                string ret = getFileName(filter);
                if (ret == "") return;
                FpgaPath = ret;
            }
        }

        public void cmdFilePath(string name)
        {
            string path = "";            
            if (name == "syslog")
            {
                path = getFolderName();
                if (path != "") SysLogPath = path;
            }else if (name == "templog")
            {
                path = getFolderName();
                if (path != "") TempLogPath = path;
            }
            else if (name == "tamstemp")
            {
                path = getFolderName();
                if (path != "") TamsSVPath = path;
            }
            else if (name == "endfile")
            {
                path = getFolderName();
                if (path != "") TamsEndPath = path;
            }
        }

         public string getFileName(string Filter)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = Filter;
            ofd.FilterIndex = 1;
            ofd.RestoreDirectory = true;
            if (ofd.ShowDialog() == false) return "";
            return ofd.FileName;
        }      
        
        public string getFolderName()
        {
            FolderBrowserDialog fdb = new FolderBrowserDialog();
            if (fdb.ShowDialog() == DialogResult.OK)            
                return fdb.SelectedPath;

            return "";
        }

        public void removeLogFiles(string path, string extention)
        {                        
            DirectoryInfo dir = new DirectoryInfo(path);
            if (dir.Exists)
            {
                DirectoryInfo[] dirInfo = dir.GetDirectories();
                string IDate = DateTime.Today.AddMonths(-SelectRetention).ToString("yyyyMMdd");
                foreach (DirectoryInfo di in dirInfo)
                {
                    foreach (FileInfo file in di.GetFiles())
                    {                        
                        if (file.Extension != extention) // ex) ".log", ".csv"
                        {
                            continue;
                        }
     
                        // 해당 날짜 보다 작으면 삭제
                        if (IDate.CompareTo(file.LastWriteTime.ToString("yyyyMMdd")) > 0)
                        {
                            file.Delete();                            
                        }
                    }
                    
                }
            }
        }

    }

    
}
