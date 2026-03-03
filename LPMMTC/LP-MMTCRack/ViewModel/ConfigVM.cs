using LP_MMTCRack.Base;
using LP_MMTCRack.Model;
using Microsoft.Win32;
using Serilog;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Media.Animation;
using System.Windows.Threading;
using System.Xml;
using System.Xml.Linq;

namespace LP_MMTCRack.ViewModel
{
    public enum CONTROL_MODE : int
    {
        NORMAL = 0,
        NOSV,
        MMLOGIC,
        ONLYLONG
    }
    public class ConfigVM : BaseVM
    {
        #region File Path - Log, hibim, Hdiag, ...
        public string EventLogPath { get; set; }
        public string TempLogPath { get; set; }
        public string HumidityLogPath { get; set; }
        public string FlowMeterLogPath { get; set; }
        public string TamsSVPath { get; set; }
        public string TamsSVFolder { get; set; }
        public string TamsSVFile { get; set; }
        public string TamsEndPath { get; set; }
        public string TamsEndFolder { get; set; }
        public string TamsEndFile { get; set; }
        #endregion

        public int ControlMode { get; set; } = (int)CONTROL_MODE.NORMAL;

        #region PW
        public string GetPassword { get; set; }
        public string SetPassword { get; set; }
        public bool IsUsePassword { get; set; }
        public bool CorrectPassword { get; set; } = false;
        #endregion
        public int StableTime { get; set; } = 0;
        public double HighTempAlarm { get; set; } = 0;
        public double LowTempAlarm { get; set; } = 0;
        public double TempErr { get; set; } = 0;
        public double TempErrCondition { get; set; } = 0;
        public double AlarmCondition { get; set; } = 0;
        public int humidityWarnval { get; set; } = 0;
        public int MotorMaxVal { get; set; } = 0;
        public double CaptureCycle { get; set; } = 1.0;
        public double ControlDiff { get; set; } = 15;
        public double ControlSetp { get; set; } = 0.5;
        public double ControlOffset { get; set; } = 0.5;
        public double ControlMaxTemp { get; set; } = 100;
        public double ControlMinTemp { get; set; } = 0;
		public double GarbageReadTemp { get; set; } = 150;
		public double NoiseTempDiff { get; set; } = 15;

        #region 온도 보정 설정
        // --- Set 1 ---
        public bool IsUseTempOffset1 { get; set; } = false;
        public double OffsetTarget1 { get; set; } = 0.0;
        public double OffsetValue1 { get; set; } = 0.0;

		// --- Set 2 ---
		public bool IsUseTempOffset2 { get; set; } = false;
		public double OffsetTarget2 { get; set; } = 0.0;
		public double OffsetValue2 { get; set; } = 0.0;
		// --- Set 3 ---
		public bool IsUseTempOffset3 { get; set; } = false;
		public double OffsetTarget3 { get; set; } = 0.0;
		public double OffsetValue3 { get; set; } = 0.0;
		#endregion

		public ObservableCollection<string> CapPortName { get; set; } = new ObservableCollection<string>();

        public ObservableCollection<string> MotorPortName { get; set; } = new ObservableCollection<string>();
        //public string[] MotorPortName { get; set; } = new string[6];
        public string TempConPortName { get; set; }
        public string FlowPortName { get; set; }
        public string ArmPortName { get; set; }

        public int ProcessBoxSVCycle { get; set; } = 0;

        bool _engrmode = false;
        public bool EngrMode 
        { 
            get => _engrmode; 
            set
            {
                _engrmode = value;
                if (_engrmode)
                    Global.gMainPageVM.EngrmodeVisibility = Visibility.Visible;
                else
                    Global.gMainPageVM.EngrmodeVisibility = Visibility.Hidden;
            }
        }
        bool _graphmode = false;
        public bool GraphMode
        {
            get => _graphmode;
            set
            { 
                _graphmode = value; 
                //if( _graphmode)
                //    Global.gGraphVM.TempGraph.Plot.AxisAuto();
            }
        }
        bool _portcheckmode = false;
        public bool PortCheckMode
        {
            get => _portcheckmode;
            set
            {
                _portcheckmode = value;
            }
        }
        public int TargetTemp { get; set; } = 0;
        public int TargetWaitTime { get; set; } = 0;
        public int TempStep { get; set; } = 0;
        public int TempStepTime { get; set; } = 0;

        /*
        public int LowTempCriteria { get; set; } = 10;
        public double LowTempDefrostStep { get; set; } = 0.5;
        public int LowTempDefrostTime { get; set; } = 60;
        */

        public DelegateCommand DGCmdFile { get; set; }
        public DelegateCommand DGCmdSave { get; set; }
        public List<string> ComPortList { get; set; } = new List<string>();

        public int LogDelMonth { get; set; } = 0;
        DispatcherTimer _retentionCheckTimer = new DispatcherTimer();

        public ConfigVM()
        {
            try
            {
                for (int i = 0; i < 6; i++)
                {
                    CapPortName.Add(""); // 빈 값으로 초기화
                    MotorPortName.Add("");
                }
                initItems();
            }
            catch (Exception ex)
            {
                HandyControl.Controls.MessageBox.Show("LPCAMRack.xml Load Error : " + ex.Message);

                Global.gMainWindow.Close();
            }
            ComPortList.Add("");
            for(int i = 1; i < 100; i++)
            {
                ComPortList.Add("COM"+ i);
            }
            Global.gConfigVM = this;
            Global.gMainPageVM.initLogFile();
            Global.gMainPageVM.startHumdityLog();
            Global.gMainPageVM.startFlowLog();

            for (int i = 0; i < 6; i++)
            {
                Global.gMainPageVM.sc[i].SiteConnect(i);
                Global.gMainPageVM.cc[i].Connect(i);
            }

            Global.gTconCom.TConConnect();
            Global.gFlowMeterCom.FlowMeterConnect();
            Global.gAlarmCom.AlarmConnect();

            Global.gGraphVM.InitGraph();

            DGCmdFile = new DelegateCommand(p => cmdFilePath((string)p));
            DGCmdSave = new DelegateCommand(p => { saveitems(); });

            _retentionCheckTimer.Interval = TimeSpan.FromHours(1);
            _retentionCheckTimer.Tick += checkRetention_tick;
        }
        public void checkRetention_tick(object sender, EventArgs e)
        {
            removeLogFiles(EventLogPath, ".log");
            removeLogFiles(TempLogPath, ".csv");
            removeLogFiles(HumidityLogPath, ".csv");
            removeLogFiles(FlowMeterLogPath, ".csv");
            _retentionCheckTimer.Interval = TimeSpan.FromHours(24);
        }
        public void removeLogFiles(string path, string extention)
        {
            try
            {
                DirectoryInfo dir = new DirectoryInfo(path);
                if (dir.Exists)
                {
                    DirectoryInfo[] dirInfo = dir.GetDirectories();
                    string IDate = DateTime.Today.AddMonths(-LogDelMonth).ToString("yyyyMMdd");
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
            catch { }
        }
        public void initItems()
        {
            XmlDocument doc = Global.gConfigDoc;

            // File Path
            XmlNode node = doc.SelectSingleNode("/lpcam-rack/log");
            LogDelMonth = Convert.ToInt32(node.Attributes["logdeletemonth"].Value);
            node = doc.SelectSingleNode("/lpcam-rack/log/event");
            EventLogPath = node.Attributes["path"].Value;
            node = doc.SelectSingleNode("/lpcam-rack/log/temp");
            TempLogPath = node.Attributes["path"].Value;
            node = doc.SelectSingleNode("/lpcam-rack/log/humidity");
            HumidityLogPath = node.Attributes["path"].Value;
            node = doc.SelectSingleNode("/lpcam-rack/log/flowmeter");
            FlowMeterLogPath = node.Attributes["path"].Value;
            node = doc.SelectSingleNode("/lpcam-rack/tams/set-temp");
            TamsSVPath = node.Attributes["path"].Value;
            TamsSVFolder = node.Attributes["folder"].Value;
            TamsSVFile = node.Attributes["name"].Value;
            node = doc.SelectSingleNode("/lpcam-rack/tams/end-flag");
            TamsEndPath = node.Attributes["path"].Value;
            TamsEndFolder = node.Attributes["folder"].Value;
            TamsEndFile = node.Attributes["name"].Value;

            node = doc.SelectSingleNode("/lpcam-rack/defrost");
            TargetTemp = Convert.ToInt32(node.Attributes["targettemp"].Value);
            TargetWaitTime = Convert.ToInt32(node.Attributes["targetwaittime"].Value);
            TempStep = Convert.ToInt32(node.Attributes["tempstep"].Value);
            TempStepTime = Convert.ToInt32(node.Attributes["steptime"].Value);

            node = doc.SelectSingleNode("/lpcam-rack/error");
            StableTime = Convert.ToInt32(node.Attributes["stable"].Value);
            TempErr = Convert.ToInt32(node.Attributes["temperr"].Value);
            TempErrCondition = Convert.ToInt32(node.Attributes["temperrcondition"].Value);
            HighTempAlarm = Convert.ToInt32(node.Attributes["highalarm"].Value);
            LowTempAlarm = Convert.ToInt32(node.Attributes["lowalarm"].Value);
            AlarmCondition = Convert.ToInt32(node.Attributes["alarmcondition"].Value);

            node = doc.SelectSingleNode("/lpcam-rack/tempcontrol");
            ControlSetp = Convert.ToDouble(node.Attributes["step"].Value);
            ControlDiff = Convert.ToDouble(node.Attributes["diff"].Value);
            ControlOffset = Convert.ToDouble(node.Attributes["offset"].Value);
            ControlMaxTemp = Convert.ToDouble(node.Attributes["maxtemp"].Value);
            ControlMinTemp = Convert.ToDouble(node.Attributes["mintemp"].Value);

            node = doc.SelectSingleNode("/lpcam-rack/etc");
            EngrMode = Convert.ToInt32(node.Attributes["engrmode"].Value) == 1 ? true: false;
            NoiseTempDiff = Convert.ToDouble(node.Attributes["NoiseTempDiff"].Value);
            GraphMode = Convert.ToInt32(node.Attributes["notempGraph"].Value) == 1 ? true : false;
            ProcessBoxSVCycle = Convert.ToInt32(node.Attributes["controlsvcycle"].Value);            
            ControlMode = Convert.ToInt32(node.Attributes["svmode"].Value);

            node = doc.SelectSingleNode("/lpcam-rack/rack-map");
            humidityWarnval = Convert.ToInt32(node.Attributes["humidityWarnval"].Value);
            MotorMaxVal = Convert.ToInt32(node.Attributes["site-maxval"].Value);
            CaptureCycle = Convert.ToDouble(node.Attributes["capture-cycle"].Value);

            node = doc.SelectSingleNode("/lpcam-rack/password");
            IsUsePassword = Convert.ToInt32(node.Attributes["use"].Value) == 1 ? true : false;
            SetPassword = node.Attributes["value"].Value;

			try
			{
				node = doc.SelectSingleNode("/lpcam-rack/tempcalibration/set1");
				if (node != null)
				{
					IsUseTempOffset1 = node.Attributes["use"].Value == "1";
					OffsetTarget1 = Convert.ToDouble(node.Attributes["target"].Value);
					OffsetValue1 = Convert.ToDouble(node.Attributes["offset"].Value);
				}

				node = doc.SelectSingleNode("/lpcam-rack/tempcalibration/set2");
				if (node != null)
				{
					IsUseTempOffset2 = node.Attributes["use"].Value == "1";
					OffsetTarget2 = Convert.ToDouble(node.Attributes["target"].Value);
					OffsetValue2 = Convert.ToDouble(node.Attributes["offset"].Value);
				}

				node = doc.SelectSingleNode("/lpcam-rack/tempcalibration/set3");
				if (node != null)
				{
					IsUseTempOffset3 = node.Attributes["use"].Value == "1";
					OffsetTarget3 = Convert.ToDouble(node.Attributes["target"].Value);
					OffsetValue3 = Convert.ToDouble(node.Attributes["offset"].Value);
				}
			}
			catch (Exception ex)
			{
				Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Config] TempCalibration Load Error: {ex.Message}");
			}

			for (int i = 0; i < 6; i++)
            {
                CapPortName[i] = Global.gMainPageVM.cc[i].CaputurePortName;
                MotorPortName[i] = Global.gMainPageVM.sc[i].SitePortName;
            }
            TempConPortName = Global.gTconCom.TconPortName;
            FlowPortName = Global.gFlowMeterCom.FlowMeterName;
            ArmPortName = Global.gAlarmCom.AlarmPortName;
        }
        public void saveitems()
        {
            for (int i = 0; i < 6; i++)
            {
                Global.gMainPageVM.cc[i].CaputurePortName = CapPortName[i];
                Global.gMainPageVM.sc[i].SitePortName = MotorPortName[i];
            }
            Global.gTconCom.TconPortName = TempConPortName;
            Global.gFlowMeterCom.FlowMeterName = FlowPortName;
            Global.gAlarmCom.AlarmPortName = ArmPortName;

            XmlDocument doc = Global.gConfigDoc;

            //Log Path
            XmlNode node = doc.SelectSingleNode("/lpcam-rack/log");
            node.Attributes["logdeletemonth"].Value = LogDelMonth.ToString();
            node = doc.SelectSingleNode("/lpcam-rack/log/event");
            node.Attributes["path"].Value = EventLogPath;
            node = doc.SelectSingleNode("/lpcam-rack/log/temp");
            node.Attributes["path"].Value = TempLogPath;
            node = doc.SelectSingleNode("/lpcam-rack/log/humidity");
            node.Attributes["path"].Value = HumidityLogPath;
            node = doc.SelectSingleNode("/lpcam-rack/log/flowmeter");
            node.Attributes["path"].Value = FlowMeterLogPath;
            node = doc.SelectSingleNode("/lpcam-rack/tams/set-temp");
            node.Attributes["path"].Value = TamsSVPath;
            node.Attributes["folder"].Value = TamsSVFolder;
            node.Attributes["name"].Value = TamsSVFile;
            node = doc.SelectSingleNode("/lpcam-rack/tams/end-flag");
            node.Attributes["path"].Value = TamsEndPath;
            node.Attributes["folder"].Value = TamsEndFolder;
            node.Attributes["name"].Value = TamsEndFile;

            //Defrost
            node = doc.SelectSingleNode("/lpcam-rack/defrost");
            node.Attributes["targettemp"].Value = TargetTemp.ToString();
            node.Attributes["targetwaittime"].Value = TargetWaitTime.ToString();
            node.Attributes["tempstep"].Value = TempStep.ToString();
            node.Attributes["steptime"].Value = TempStepTime.ToString();

            node = doc.SelectSingleNode("/lpcam-rack/error");
            node.Attributes["stable"].Value = StableTime.ToString();
            node.Attributes["temperr"].Value = TempErr.ToString();
            node.Attributes["temperrcondition"].Value = TempErrCondition.ToString();
            node.Attributes["highalarm"].Value = HighTempAlarm.ToString();
            node.Attributes["lowalarm"].Value = LowTempAlarm.ToString();
            node.Attributes["alarmcondition"].Value = AlarmCondition.ToString();

            node = doc.SelectSingleNode("/lpcam-rack/tempcontrol");
            node.Attributes["step"].Value = ControlSetp.ToString();
            node.Attributes["diff"].Value = ControlDiff.ToString();
            node.Attributes["offset"].Value = ControlOffset.ToString();
            node.Attributes["maxtemp"].Value = ControlMaxTemp.ToString();
            node.Attributes["mintemp"].Value = ControlMinTemp.ToString();

            node = doc.SelectSingleNode("/lpcam-rack/etc");
            if (EngrMode)
                node.Attributes["engrmode"].Value = "1";
            else
                node.Attributes["engrmode"].Value = "0";

            node.Attributes["NoiseTempDiff"].Value = NoiseTempDiff.ToString();
            node.Attributes["svmode"].Value = ControlMode.ToString();
            
            if (GraphMode)
                node.Attributes["notempGraph"].Value = "1";
            else
                node.Attributes["notempGraph"].Value = "0";

            node.Attributes["controlsvcycle"].Value = ProcessBoxSVCycle.ToString();

            //MainPage Config
            var columnNode = doc.SelectSingleNode("/lpcam-rack/rack-map");
            columnNode.Attributes["logo"].Value = Global.gMainPageVM.logo;
            columnNode.Attributes["column"].Value = Global.gMainPageVM.LayoutColumnCount.ToString();
            columnNode.Attributes["tcon-port"].Value = Global.gTconCom.TconPortName;
            columnNode.Attributes["humidityWarnval"].Value = humidityWarnval.ToString();
            columnNode.Attributes["site-maxval"].Value = MotorMaxVal.ToString();
            columnNode.Attributes["capture-cycle"].Value = CaptureCycle.ToString();
            columnNode.Attributes["site-offsetval"].Value = Global.gMainPageVM.MotorOffsetVal.ToString();

            var nodes = Global.gConfigDoc.SelectNodes("/lpcam-rack/rack-map/rack");
            int site = 0;
            foreach (XmlNode Rnode in nodes)
            {
                Rnode.Attributes["id"].Value = Global.gMainPageVM.Racks[site].RackIndex.ToString();
                Rnode.Attributes["site-port"].Value = Global.gMainPageVM.sc[site].SitePortName;
                Rnode.Attributes["site-val"].Value = Global.gMainPageVM.sc[site].MotorValue;
                if (Global.gMainPageVM.sc[site].SitePortUse)
                    Rnode.Attributes["site-use"].Value = "1";
                else
                    Rnode.Attributes["site-use"].Value = "0";
                Rnode.Attributes["site-final"].Value = Global.gMainPageVM.sc[site].MotorFinalStep;
                if (Global.gMainPageVM.sc[site].MotorPositon)
                    Rnode.Attributes["site-position"].Value = "1";
                else
                    Rnode.Attributes["site-position"].Value = "0";

                Rnode.Attributes["capture-port"].Value = Global.gMainPageVM.cc[site].CaputurePortName;
                if (Global.gMainPageVM.cc[site].CaputureUse)
                    Rnode.Attributes["capture-use"].Value = "1";
                else
                    Rnode.Attributes["capture-use"].Value = "0";
                site++;
            }
            node = doc.SelectSingleNode("/lpcam-rack/flowmeter");
            node.Attributes["port"].Value = Global.gFlowMeterCom.FlowMeterName;
            node = doc.SelectSingleNode("/lpcam-rack/alarm");
            node.Attributes["port"].Value = Global.gAlarmCom.AlarmPortName;
            if(Global.gAlarmCom.AlarmPortFlag)
                node.Attributes["flag"].Value = "1";
            else
                node.Attributes["flag"].Value = "0";
            if (Global.gAlarmCom.AlarmPortUse)
                node.Attributes["use"].Value = "1";
            else
                node.Attributes["use"].Value = "0";

			try
			{
				// Set 1
				node = doc.SelectSingleNode("/lpcam-rack/tempcalibration/set1");
				if (node != null)
				{
					node.Attributes["use"].Value = IsUseTempOffset1 ? "1" : "0";
					node.Attributes["target"].Value = OffsetTarget1.ToString("F1");
					node.Attributes["offset"].Value = OffsetValue1.ToString("F1");
				}

				// Set 2
				node = doc.SelectSingleNode("/lpcam-rack/tempcalibration/set2");
				if (node != null)
				{
					node.Attributes["use"].Value = IsUseTempOffset2 ? "1" : "0";
					node.Attributes["target"].Value = OffsetTarget2.ToString("F1");
					node.Attributes["offset"].Value = OffsetValue2.ToString("F1");
				}

				// Set 3
				node = doc.SelectSingleNode("/lpcam-rack/tempcalibration/set3");
				if (node != null)
				{
					node.Attributes["use"].Value = IsUseTempOffset3 ? "1" : "0";
					node.Attributes["target"].Value = OffsetTarget3.ToString("F1");
					node.Attributes["offset"].Value = OffsetValue3.ToString("F1");
				}
			}
			catch (Exception ex)
			{
				Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Config] TempCalibration Save Error: {ex.Message}");
			}

			site = 0;
            node = doc.SelectSingleNode("lpcam-rack/rack-map/rack");

            foreach (RackModel rm in Global.gMainPageVM.Racks)
            {
                columnNode = doc.SelectSingleNode("/lpcam-rack/flowmeter/flow" + (site + 1));
                columnNode.Attributes["min"].Value = rm.FlowMinLimit.ToString();
                columnNode.Attributes["max"].Value = rm.FlowMaxLimit.ToString();

                var rackNode = node.SelectSingleNode($"//rack[@id='{Global.gMainPageVM.Racks[site].RackIndex}']");
                int i = 0;
                foreach(XmlNode boxNode in rackNode.ChildNodes)
                {
                    boxNode.Attributes["id"].Value = i.ToString();
                    boxNode.Attributes["humidityoffset"].Value = rm.Boxes[i].HumidityOffset.ToString();
                    if (rm.Boxes[i].IsBoxTCUnit)
                        boxNode.Attributes["tc-unit"].Value = "1";
                    else
                        boxNode.Attributes["tc-unit"].Value = "0";
                    if (rm.Boxes[i].IsBoxAS)
                        boxNode.Attributes["as"].Value = "1";
                    else
                        boxNode.Attributes["as"].Value = "0";
                    //if (rm.Boxes[i].ISBoxTams)
                        //boxNode.Attributes["tams"].Value = "1";
                    //else
                        //boxNode.Attributes["tams"].Value = "0";

                    i++;
                }

                site++;
            }
            Global.saveConfig();

            //Global.gGraphVM.InitGraph();
            //System.Windows.MessageBox.Show("SAVE DONE");
        }
        public void checkPassword()
        {
            CorrectPassword = (SetPassword == GetPassword) ? true : false;
        }
        public void cmdFilePath(string name)
        {
            string path = "";
            if (name == "syslog")
            {
                path = getFolderName();
                if (path != "") EventLogPath = path;
            }
            else if (name == "Templog")
            {
                path = getFolderName();
                if (path != "") TempLogPath = path;
            }
            else if (name == "Humiditylog")
            {
                path = getFolderName();
                if (path != "") HumidityLogPath = path;
            }
            else if (name == "FlowMeterlog")
            {
                path = getFolderName();
                if (path != "") FlowMeterLogPath = path;
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
        public string getFolderName()
        {
            FolderBrowserDialog fdb = new FolderBrowserDialog();
            if (fdb.ShowDialog() == DialogResult.OK)
                return fdb.SelectedPath;

            return "";
        }
    }
}
