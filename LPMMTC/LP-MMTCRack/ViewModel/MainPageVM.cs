using LP_MMTCRack.Base;
using LP_MMTCRack.Model;
using LP_MMTCRack.Net;
using PropertyChanged;
using Serilog;
using Serilog.Core;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;
using System.Xml;

namespace LP_MMTCRack.ViewModel
{
    public enum LOG_TYPE : int
    {
        INFO = 0,
        WARNING,
        ERROR
    }
    public class SystemLog : BaseVM
    {
        public LOG_TYPE Type { get; set; }
        public string Time { get; set; }
        public string Message { get; set; }
        public SystemLog(LOG_TYPE type, string msg)
        {
            Type = type;
            Time = DateTime.Now.ToString("yyyy-MM-dd H:mm:ss");
            Message = msg;
        }
    }
    public class MainPageVM : BaseVM
    {
        public SiteCom[] sc = new SiteCom[6];
        public CaptureCom[] cc = new CaptureCom[6];
        public int LayoutColumnCount { get; set; } = 2;
        public int MotorOffsetVal { get; set; } = 0;
        public bool IsTamsAllEnable { get; set; } = false;
        Visibility _engrmodevisibility = Visibility.Hidden;
        public Visibility EngrmodeVisibility 
        { 
            get => _engrmodevisibility;
            set
            {
                _engrmodevisibility = value;
                if(_engrmodevisibility == Visibility.Visible)
                {
                    IsTamsAllEnable = true;
                    foreach (RackModel rm in Racks)
                    {
                        rm.RackEngrmodeVisibility = Visibility.Visible;
                        rm.IsRackTamsEnable = true;
                        foreach (BoxModel bm in rm.Boxes)
                            bm.IsBoxTamsEnable = true;
                    }
                }
                else if(_engrmodevisibility == Visibility.Hidden)
                {
                    IsTamsAll = true;
                    IsTamsAllEnable = false;
                    foreach (RackModel rm in Racks)
                    {
                        rm.RackEngrmodeVisibility = Visibility.Hidden;
                        //rm.IsRackTams = true;
                        //rm.IsRackTamsEnable = false;
                        foreach (BoxModel bm in rm.Boxes)
                        {
                            bm.IsBoxTams = true;
                            bm.IsBoxTamsEnable = false;
                        }
                    }
                }
            }
        }
        public double SVAll { get; set; } = 25;
        public bool _isTamsAll = true;
        public bool IsTamsAll
        {
            get => _isTamsAll;
            set
            {
                _isTamsAll = value;
                foreach (RackModel rack in Racks)
                    foreach (BoxModel bm in rack.Boxes)
                        bm.IsBoxTams = _isTamsAll;

                if (_isTamsAll)
                    EnableSVAll = false;
                else
                    EnableSVAll = true;
            }
        }
        public bool EnableSVAll { get; set; } = true;
        int _allrackruncnt = 0;
        public int AllRackRunCnt 
        { 
            get => _allrackruncnt; 
            set
            {
                if(value > AllSlotCnt) //AllBoxCnt 12 or 18
                    _allrackruncnt = AllSlotCnt;
                else if(value < 0)
                    _allrackruncnt = 0;
                else
                    _allrackruncnt = value;
            }
        }
        /*
        int _allrackpreruncnt = 0;
        public int AllRackPreRunCnt
        {
            get => _allrackruncnt;
            set
            {
                if (value > 18)
                    _allrackpreruncnt = 18;
                else if (value < 0)
                    _allrackpreruncnt = 0;
                else
                    _allrackpreruncnt = value;
            }
        }
        */
        ObservableCollection<RackModel> _racks = new ObservableCollection<RackModel>();
        public ObservableCollection<RackModel> Racks { get => _racks; set { _racks = value; PC(nameof(Racks)); }}
        public DelegateCommand DGCmdAll { get; set; }
        [DoNotNotify]
        public Logger SysSerilog { get; set; }
        [DoNotNotify]
        public Logger FlowSerilog { get; set; } = null;
        [DoNotNotify]
        public Logger HumiditySerilog { get; set; } = null;
        [DoNotNotify]
        public Logger HumidityTempSerilog { get; set; } = null;
        [DoNotNotify]
        public Logger AllTempSerilog { get; set; } = null; //이거 사용해서 전체 LP Data 저장하는방향

        public ListView ListSysLog;
        ObservableCollection<SystemLog> _sysLogs = new ObservableCollection<SystemLog>();
        public ObservableCollection<SystemLog> SysLogs { get { return _sysLogs; } set { _sysLogs = value; PC("SysLogs"); } }
        public string logo = string.Empty;

        int _allboxcnt = 0;
        public int AllBoxCnt
        {
            get => _allboxcnt;
            set
            {
                _allboxcnt = value;
                if (_allboxcnt == 2)
                    AllSlotCnt = 12;
                else if(_allboxcnt == 3)
                    AllSlotCnt = 18;
            }
        }

        public int AllSlotCnt = 0;
        public MainPageVM()
        {
            Global.gMainPageVM = this;
            //initLogFile();
            var columnNode = Global.gConfigDoc.SelectSingleNode("/lpcam-rack/rack-map");
            logo = columnNode.Attributes["logo"].Value;
            if (logo == "star")
                Global.gMainVM.LogoPath = "/Resource/star.png";
            else if (logo == "gsi")
                Global.gMainVM.LogoPath = "/Resource/gsi.png";
            else if (logo == "cube")
                Global.gMainVM.LogoPath = "/Resource/cube.png";

            LayoutColumnCount = Convert.ToInt32(columnNode.Attributes["column"].Value);
            AllBoxCnt = Convert.ToInt32(columnNode.Attributes["Boxes"].Value);
            Global.gTconCom.TconPortName = columnNode.Attributes["tcon-port"].Value;
            MotorOffsetVal = Convert.ToInt32(columnNode.Attributes["site-offsetval"].Value);

            var nodes = Global.gConfigDoc.SelectNodes("/lpcam-rack/rack-map/rack");
            int site = 0;
            foreach (XmlNode node in nodes)
            {
                int id = Convert.ToInt32(node.Attributes["id"].Value);
                Racks.Add(new RackModel(id, AllBoxCnt));

                sc[site] = new SiteCom();
                sc[site].SitePortName = node.Attributes["site-port"].Value;
                sc[site].MotorValue = node.Attributes["site-val"].Value;
                sc[site].SitePortUse = Convert.ToInt32(node.Attributes["site-use"].Value) == 1 ? true : false;
                sc[site].MotorFinalStep = node.Attributes["site-final"].Value;
                sc[site].MotorPositon = Convert.ToInt32(node.Attributes["site-position"].Value) == 1 ? true : false;
                sc[site].UsbSiteNameing = "CP2102 USB to UART Bridge Controller S" + (site + 1);
                sc[site].UsbHwSitePort = "SITE" + (site + 1).ToString();

                cc[site] = new CaptureCom();
                cc[site].CaputurePortName = node.Attributes["capture-port"].Value;
                cc[site].CaputureUse = Convert.ToInt32(node.Attributes["capture-use"].Value) == 1 ? true : false;
                cc[site].UsbCaptureNameing = "CP2102 USB to UART Bridge Controller C" + (site + 1);
                cc[site].UsbHWPort = "CAP" + (site + 1).ToString();
                site++;
            }
            columnNode = Global.gConfigDoc.SelectSingleNode("/lpcam-rack/flowmeter");
            Global.gFlowMeterCom.FlowMeterName = columnNode.Attributes["port"].Value;

            columnNode = Global.gConfigDoc.SelectSingleNode("/lpcam-rack/alarm");
            Global.gAlarmCom.AlarmPortName = columnNode.Attributes["port"].Value;
            Global.gAlarmCom.AlarmPortFlag = Convert.ToInt32(columnNode.Attributes["flag"].Value) == 1 ? true : false;
            Global.gAlarmCom.AlarmPortUse = Convert.ToInt32(columnNode.Attributes["use"].Value) == 1 ? true : false;

            site = 0;
            foreach (RackModel rm in Racks)
            {
                rm.showAlram(ALRAM_TYPE.INIT, Global.msg_InitRack, false);
                columnNode = Global.gConfigDoc.SelectSingleNode("/lpcam-rack/flowmeter/flow" + (site + 1));
                rm.FlowMinLimit = Convert.ToInt16(columnNode.Attributes["min"].Value);
                rm.FlowMaxLimit = Convert.ToInt16(columnNode.Attributes["max"].Value);
                site++;
            }

            DGCmdAll = new DelegateCommand(p => {
                int overlapCnt = 0;
                foreach (RackModel rack in Racks)
                {

                    switch (p)
                    {
                        case "Set":

							rack.RackSetSV = SVAll;
							Task.Run(() => rack.RacksetTemp());
							break;
                        case "Start":
                            Task.Run(() => rack.RackstartTest());
                            break;
                        case "Stop":
                            Task.Run(() => rack.RackstopTestAsync());
                            break;
                        case "Select":
                            foreach(BoxModel bm in rack.Boxes)
							{
								// 안전한 상태일 때만 true 적용
								if (bm.BoxStat == Box_STAT.IDLE || bm.BoxStat == Box_STAT.STOP ||
									bm.BoxStat == Box_STAT.DONE || bm.BoxStat == Box_STAT.ERROR)
								{
									bm.IsBoxTCUnit = true;
								}
							}
							break;
                        case "None":
                            foreach (BoxModel bm in rack.Boxes)
							{
								// 안전한 상태일 때만 false 적용 (동작 중인 박스 보호)
								if (bm.BoxStat == Box_STAT.IDLE || bm.BoxStat == Box_STAT.STOP ||
									bm.BoxStat == Box_STAT.DONE || bm.BoxStat == Box_STAT.ERROR)
								{
									bm.IsBoxTCUnit = false;
								}
							}
							break;
                        case "PopUp":
                            rack.ShowBox = Visibility.Hidden;
                            if (rack.RackMotorStat == MOTOR_STAT.ERROR)
                                rack.RackMotorStat = MOTOR_STAT.NORMAL;
                            foreach (BoxModel bm in rack.Boxes)
                            {
                                bm.BoxShowBox = Visibility.Hidden;
                                if (bm.BoxStat == Box_STAT.ERROR)
                                {
                                    Task.Run(() => bm.Read_BoxTConStat());
                                    bm.BoxStat = Box_STAT.STOP;
                                    bm.BoxDefrost = DEFROST_TYPE.NORMAL;
                                    bm.DefostTime = string.Empty;
                                    bm.Sopen_Stat = true;
                                    bm.Bimetal_Stat = true;
                                }
                                else if (bm.BoxStat == Box_STAT.DONE)
                                {
                                    bm.BoxStat = Box_STAT.STOP;
                                    bm.BoxDefrost = DEFROST_TYPE.NORMAL;
                                    bm.DefostTime = string.Empty;
                                    bm.Sopen_Stat = true;
                                    bm.Bimetal_Stat = true;
                                }
                            }
                            break;
                        case "MotorDown":
                            Task.Run(() => rack.PreMotorAutoDown());
                            break;
                        case "MotorUp":
                            Task.Run(() => rack.PreMotorAutoOpen(MOTOR_TYPE.NOMAL));//수동 모터 업 true
                            break;
                        case "Clear":
                            SysLogs.Clear();
                                //ListSysLog.Items.Clear();
                            break;
                        case "HumidityOn":
                            foreach (BoxModel bm in rack.Boxes)
                            {
                                bm.isLoggingHumidity = true;
                            }
                            break;
                        case "HumidityOff":
                            foreach (BoxModel bm in rack.Boxes)
                            {
                                bm.isLoggingHumidity = false;
                                bm.HumidityValue = 0;
                                bm.HumRateColor = Global.gWarningColors[0];
                            }
                            break;
                        case "ReConnect":
                            if (overlapCnt == 0)
                            {
                                for (int i = 0; i < 6; i++)
                                {
                                    sc[i].SiteConnect(i);
                                    cc[i].Connect(i);
                                }

                                Global.gTconCom.TConConnect();
                                Global.gFlowMeterCom.FlowMeterConnect();
                                Global.gAlarmCom.AlarmConnect();
                                overlapCnt++;
                            }
                            break;
                    }

                }
            });
        }
        public void initLogFile()
        {
            SysSerilog = new LoggerConfiguration()
                //                .WriteTo.Console()
                .WriteTo.File(Global.gConfigVM.EventLogPath + "\\" + "Event_.log", rollingInterval: RollingInterval.Day,
                outputTemplate: "{Timestamp:[yyyy-MM-dd HH:mm:ss]} [{Level:u3}] {Message:lj}{NewLine}")
                .CreateLogger();
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "Start Application");
        }
        public void startHumdityLog()
        {
            if (HumiditySerilog != null)
                HumiditySerilog.Dispose();
            string file = $"{Global.gConfigVM.HumidityLogPath}\\Humidity_{DateTime.Now.ToString("yyyy_MM_dd_HHmm")}" + $".csv";
            HumiditySerilog = new LoggerConfiguration()
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite, outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();

            startHumdityTempLog();
        }
        public void startHumdityTempLog()
        {
            if (HumidityTempSerilog != null)
                HumidityTempSerilog.Dispose();
            string file = $"{Global.gConfigVM.HumidityLogPath}\\SensorTemperature_{DateTime.Now.ToString("yyyy_MM_dd_HHmm")}" + $".csv";
            HumidityTempSerilog = new LoggerConfiguration()
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite, outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();
        }
        public void startFlowLog()
        {
            if (FlowSerilog != null)
                FlowSerilog.Dispose();
            string file = $"{Global.gConfigVM.FlowMeterLogPath}\\FlowData_{DateTime.Now.ToString("yyyy_MM_dd_HHmm")}" + $".csv";
            FlowSerilog = new LoggerConfiguration()
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite, outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();
        }
        public void addHumdityLog()
        {
            if (HumiditySerilog == null) startHumdityLog();
            if (HumiditySerilog == null) return;

            if (AllRackRunCnt > 0) {
                StringBuilder strBuilder = new StringBuilder();
                strBuilder.Append(DateTime.Now.ToString("MM-dd HH:mm:ss"));

                foreach (RackModel rm in Racks)
                {
                    foreach (BoxModel bm in rm.Boxes)
                        if (bm.isLoggingHumidity)
                            strBuilder.Append("," + bm.HumidityValue.ToString());
                        else
                            strBuilder.Append(",0");
                        
                    
                }
                HumiditySerilog.Information(strBuilder.ToString());
            }
            addHumdityTempLog();

        }
        public void addHumdityTempLog()
        {
            if (HumidityTempSerilog == null) startHumdityTempLog();
            if (HumidityTempSerilog == null) return;

            if (AllRackRunCnt > 0)
            {
                StringBuilder strBuilder = new StringBuilder();
                strBuilder.Append(DateTime.Now.ToString("MM-dd HH:mm:ss"));

                foreach (RackModel rm in Racks)
                {
                    foreach (BoxModel bm in rm.Boxes)
                        if (bm.isLoggingHumidity)
                            strBuilder.Append("," + bm.Hum_TempSenorVal.ToString());
                        else
                            strBuilder.Append(",0");
                }
                HumidityTempSerilog.Information(strBuilder.ToString());
            }
        }
       
        public void endHumdityLog()
        {
            if (HumiditySerilog == null) return;
            
            StringBuilder strBuilder = new StringBuilder();

            strBuilder.Append("\r\n");

            HumiditySerilog.Information(strBuilder.ToString());

            if (HumiditySerilog == null)
                return;
            HumiditySerilog.Dispose();
            HumiditySerilog = null;

            endHumdityTempLog();
        }
        public void endHumdityTempLog()
        {
            if (HumidityTempSerilog == null) return;

            StringBuilder strBuilder = new StringBuilder();

            strBuilder.Append("\r\n");

            HumidityTempSerilog.Information(strBuilder.ToString());

            if (HumidityTempSerilog == null)
                return;
            HumidityTempSerilog.Dispose();
            HumidityTempSerilog = null;
        }
        public void addFlowLog()
        {
            if(FlowSerilog == null) startFlowLog();
            if (FlowSerilog == null) return;

            StringBuilder strBuilder = new StringBuilder();
            strBuilder.Append(DateTime.Now.ToString("MM-dd HH:mm:ss"));

            foreach(RackModel rm in Racks)
            {
                strBuilder.Append("," + rm.FlowRate.ToString());
            }
            FlowSerilog.Information(strBuilder.ToString());
        }
		private readonly object _logLock = new object();
		public void addLog(LOG_TYPE type, string msg)
        {
            try
            {
                SystemLog log = new SystemLog(type, msg);
                lock (_logLock)
                {
                    if (Application.Current != null)
                    {
                        Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                        new Action(() =>
                        {
                            try
                            {
                                SysLogs.Add(log);
                                if (SysLogs.Count > 1000)
                                    SysLogs.RemoveAt(0);
                                ListSysLog.ScrollIntoView(log);
                            }
                            catch (Exception ex) { }
                        }));
                    }
                }

                switch (type)
                {
                    case LOG_TYPE.ERROR:
                        SysSerilog.Error(msg);
                        break;
                    case LOG_TYPE.WARNING:
                        SysSerilog.Warning(msg);
                        break;
                    case LOG_TYPE.INFO:
                        SysSerilog.Information(msg);
                        break;
                }
            }
            catch
            { }
        }
        public RackModel getRack(int index)
        {
            if (index >= Racks.Count) return null;
            return Racks[index];
        }
    }
}
