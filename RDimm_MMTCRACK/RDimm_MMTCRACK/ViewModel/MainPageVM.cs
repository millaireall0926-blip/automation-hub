using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.Model;
using RDimm_MMTCRACK.Net;
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

namespace RDimm_MMTCRACK.ViewModel
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
        public int MaxDimmCount = 16;
        public int MaxFixDimmCount = 32;
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
                    }
                }
                else if(_engrmodevisibility == Visibility.Hidden)
                {
                    IsTamsAll = true;
                    IsTamsAllEnable = false;
                    foreach (RackModel rm in Racks)
                    {
                        rm.RackEngrmodeVisibility = Visibility.Hidden;
                        rm.IsRackTams = true;
                        rm.IsRackTamsEnable = false;
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
                    rack.IsRackTams = _isTamsAll;

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
                if(value > 18)
                    _allrackruncnt = 18;
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
                    _allrackpreruncnt = valuse;
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
        public Logger TempSerilog { get; set; } = null;
        public Logger TempSerilogH { get; set; } = null;
        [DoNotNotify]
        public Logger TempSerilogL { get; set; } = null;

        public ListView ListSysLog;
        ObservableCollection<SystemLog> _sysLogs = new ObservableCollection<SystemLog>();
        public ObservableCollection<SystemLog> SysLogs { get { return _sysLogs; } set { _sysLogs = value; PC("SysLogs"); } }
        public string logo = string.Empty;
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
            MaxDimmCount = Convert.ToInt32(columnNode.Attributes["dims"].Value);
            Global.gTconCom.TconPortName = columnNode.Attributes["tcon-port"].Value;
            MotorOffsetVal = Convert.ToInt32(columnNode.Attributes["site-offsetval"].Value);

            var nodes = Global.gConfigDoc.SelectNodes("/lpcam-rack/rack-map/rack");
            int site = 0;
            foreach (XmlNode node in nodes)
            {
                int id = Convert.ToInt32(node.Attributes["id"].Value);
                Racks.Add(new RackModel(id));

                sc[site] = new SiteCom();
                sc[site].SitePortName = node.Attributes["site-port"].Value;
                sc[site].MotorValue = node.Attributes["site-val"].Value;
                sc[site].SitePortUse = Convert.ToInt32(node.Attributes["site-use"].Value) == 1 ? true : false;
                sc[site].MotorFinalStep = node.Attributes["site-final"].Value;
                sc[site].MotorPositon = Convert.ToInt32(node.Attributes["site-position"].Value) == 1 ? true : false;

                cc[site] = new CaptureCom();
                cc[site].CaputurePortName = node.Attributes["capture-port"].Value;
                cc[site].CaputureUse = Convert.ToInt32(node.Attributes["capture-use"].Value) == 1 ? true : false;
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

            DGCmdAll = new DelegateCommand( p => {
                int overlapCnt = 0;
                foreach (RackModel rack in Racks)
                {

                    switch (p)
                    {
                        case "Set":
                            //캡쳐 Data Test 용
                            /*
                            for (byte a = 0; a < 0xff; a++)
                            {
                                for (byte i = 0; i < 0xff; i++)
                                    cc[0].RackDimmTemp[0] = cc[0].ReadDimmTemperature(a, i, 0);

                                Global.gMainPageVM.Racks[0].PreprocessTest();
                                //Thread.Sleep(5000);
                            }
                            */
                            /*
                           for (int a = 0; a < 10; a++)
                           {
                               cc[0].RackDimmTemp[11] = cc[0].ReadDimmTemperature(0x3, 0xC0, 11);
                               cc[0].RackDimmTemp[13] = cc[0].ReadDimmTemperature(0x0, 0x0, 13);
                               Global.gMainPageVM.Racks[0].PreprocessTest();
                               Global.gMainPageVM.Racks[3].CBoxs[1].SopenStat(false);
                                Global.gMainPageVM.Racks[4].CBoxs[2].BimetalStat(false);
                                await Task.Delay(1000);
                           }

                           for (int a = 0; a < 100; a++)
                           {
                               Console.WriteLine("Start In");
                               cc[0].RackDimmTemp[11] = cc[0].ReadDimmTemperature(0x3, 0xC0, 11);
                               cc[0].RackDimmTemp[13] = cc[0].ReadDimmTemperature(0x10, 0xA9, 13);
                               Console.WriteLine($"data = {cc[0].RackDimmTemp[13]}");
                               Global.gMainPageVM.Racks[0].PreprocessTest();
                                Global.gMainPageVM.Racks[3].CBoxs[1].SopenStat(true);
                                Global.gMainPageVM.Racks[4].CBoxs[2].BimetalStat(true);
                                await Task.Delay(2000);
                               Console.WriteLine("Start Out");
                           }
                           for (int a = 0; a < 100; a++)
                           {
                               Console.WriteLine("Start In");
                               cc[0].RackDimmTemp[11] = cc[0].ReadDimmTemperature(0x3, 0xC0, 11);
                               cc[0].RackDimmTemp[13] = cc[0].ReadDimmTemperature(0x3, 0x21, 13);
                               Console.WriteLine($"data = {cc[0].RackDimmTemp[6]}");
                               Global.gMainPageVM.Racks[0].PreprocessTest();
                                Global.gMainPageVM.Racks[3].CBoxs[1].SopenStat(false);
                                Global.gMainPageVM.Racks[4].CBoxs[2].BimetalStat(false);
                                await Task.Delay(2000);
                               Console.WriteLine("Start Out");
                           }
                           for (int a = 0; a < 50; a++)
                           {
                               cc[0].RackDimmTemp[11] = cc[0].ReadDimmTemperature(0x3, 0xC0, 11);
                               cc[0].RackDimmTemp[13] = cc[0].ReadDimmTemperature(0x0, 0x0, 13);
                               Global.gMainPageVM.Racks[0].PreprocessTest();
                                Global.gMainPageVM.Racks[3].CBoxs[1].SopenStat(true);
                                Global.gMainPageVM.Racks[4].CBoxs[2].BimetalStat(true);
                                await Task.Delay(2000);
                           }
                           */
                            rack.RackSetSV = SVAll;
                            Task.Run(() => rack.RacksetTemp());
                            break;
                        case "Start":
                            Task.Run(() => rack.RackstartTest());
                            break;
                        case "Stop":
                            Task.Run(async () => await rack.RackstopTestAsync());
                            //Task.Run(() => rack.RackstopTest());
                            break;
                        case "Select":
                            rack.IsRackTCUnit = false;
                            rack.IsRackTCUnit = true;
                            break;
                        case "None":
                            rack.IsRackTCUnit = true;
                            rack.IsRackTCUnit = false;
                            break;
                        case "PopUp":
                            rack.RackPopUpClear?.Execute(null);
                            break;
                            /*
                            rack.ShowBox = Visibility.Hidden;
                            if (rack.RackMotorStat == MOTOR_STAT.ERROR)
                                rack.RackMotorStat = MOTOR_STAT.NORMAL;

                            if (rack.RackStat == RACK_STAT.DONE || rack.RackStat == RACK_STAT.ERROR || rack.RackStat == RACK_STAT.STOP)
                            {
                                //제상 시간 초기화
                                rack.DefrostTime = string.Empty;

                                //보류 0320
                                //rack.RemainDefosrtTime = string.Empty;
                                rack.RackStat = RACK_STAT.STOP;
                                rack.Rack_Defrost = DEFROST_TYPE.NORMAL;

								foreach (CBoxModel bm in rack.CBoxs)
								{

									Task.Run(() => bm.Read_BoxTConStat());
									bm.CBoxStat = CBOX_STAT.STOP;
									bm.BoxDefrost = DEFROST_TYPE.NORMAL;
									bm.Sopen_Stat = true;
									bm.Bimetal_Stat = true;
								}

							}

                            foreach (CBoxModel bm in rack.CBoxs)
                            {
                                if (bm.CBoxStat == CBOX_STAT.ERROR)
                                {
                                    Task.Run(() => bm.Read_BoxTConStat());
                                    bm.CBoxStat = CBOX_STAT.STOP;
                                    bm.BoxDefrost = DEFROST_TYPE.NORMAL;
                                    bm.Sopen_Stat = true;
                                    bm.Bimetal_Stat = true;
                                }
                                else if(bm.CBoxStat == CBOX_STAT.DEFROST)
                                {
                                    if(bm.MasterRack.RackStat == RACK_STAT.STOP)
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
                            break;
                            */
                        case "MotorDown":
                            Task.Run(() => rack.PreMotorAutoDown());
                            break;
                        case "MotorUp":
                            Task.Run(() => rack.PreMotorAutoOpen(true));//수동 모터 업 true
                            break;
                        case "Clear":
                            SysLogs.Clear();
                                //ListSysLog.Items.Clear();
                            break;
                        case "HumidityOn":
                            foreach (CBoxModel bm in rack.CBoxs)
                            {
                                bm.isLoggingHumidity = true;
                            }
                            break;
                        case "HumidityOff":
                            foreach (CBoxModel bm in rack.CBoxs)
                            {
                                bm.isLoggingHumidity = false;
                                bm.HumidityValue = 0;
                                bm.HumRateColor = Global.gWarningColors[0];
                            }
                            break;
                        case "ReConnect":
                            if (overlapCnt == 0)
                            {
                                if (AllRackRunCnt == 0)
                                {
                                    for (int i = 0; i < 6; i++)
                                    {
                                        sc[i].SiteConnect(i);
                                        cc[i].CaptureConnect(i);
                                    }

                                    Global.gTconCom.TConConnect();
                                    Global.gFlowMeterCom.FlowMeterConnect();
                                    Global.gAlarmCom.AlarmConnect();
                                    overlapCnt++;
                                }
                                else
                                {
                                    overlapCnt++;
                                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "All Fixture is not Stop. Before All Fixture make Stop");
                                }
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
        /*
        public void startTempLog()
        {
            if(TempSerilog != null)
                TempSerilog.Dispose();
            string file = $"{Global.gConfigVM.TempLogPath}\\TempData_{DateTime.Now.ToString("yyyy_MM_dd_HHmm")}" + 
                $"{(Global.gConfigVM.GraphMode ? "_NoRun" : "")}.csv";
            TempSerilog = new LoggerConfiguration()
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite, outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();

            //TS0 TS1 구별해서 추가 저장할지 미정 하면 여기서
        }
        */
        public void addHumdityLog()
        {
            if (HumiditySerilog == null) startHumdityLog();
            if (HumiditySerilog == null) return;

            if (AllRackRunCnt > 0) {
                StringBuilder strBuilder = new StringBuilder();
                strBuilder.Append(DateTime.Now.ToString("MM-dd HH:mm:ss"));

                foreach (RackModel rm in Racks)
                {
                    foreach (CBoxModel bm in rm.CBoxs)
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
                    foreach (CBoxModel bm in rm.CBoxs)
                        if (bm.isLoggingHumidity)
                            strBuilder.Append("," + bm.Hum_TempSenorVal.ToString());
                        else
                            strBuilder.Append(",0");
                }
                HumidityTempSerilog.Information(strBuilder.ToString());
            }
        }

        /*
        public void addTempLog()
        {
            //if (Global.gMainVM.AllRackSTAT != TOTALRACK_STAT.RUN) return;
            if(TempSerilog == null) startTempLog();
            if (TempSerilog == null) return;

            StringBuilder strBuilder = new StringBuilder();
            strBuilder.Append(DateTime.Now.ToString("MM-dd HH:mm:ss"));

            foreach (RackModel rm in Racks)
                foreach (BoxModel bm in rm.Boxes)
                    strBuilder.Append("," + bm.BoxPV);

            foreach (RackModel rm in Racks)
                foreach (BoxModel bm in rm.Boxes)
                    strBuilder.Append("," + bm.GetSV);

            foreach (RackModel rm in Racks)
                foreach (BoxModel bm in rm.Boxes)
                    strBuilder.Append("," + bm.GetPV);

            TempSerilog.Information(strBuilder.ToString());
        }
        */
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
        public void addLog(LOG_TYPE type, string msg)
        {
            SystemLog log = new SystemLog(type, msg);
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                new Action(() =>
                {
                    SysLogs.Add(log);
                    if (SysLogs.Count > 1000)
                        SysLogs.RemoveAt(0);
                    ListSysLog.ScrollIntoView(log);
                }));

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
        public void endTempLog()
        {
            if (TempSerilog == null) return;

            StringBuilder strBuilder = new StringBuilder();

            strBuilder.Append("\r\n");

            TempSerilog.Information(strBuilder.ToString());

            if (TempSerilog == null)
                return;
            TempSerilog.Dispose();
            TempSerilog = null;
        }
        public RackModel getRack(int index)
        {
            if (index >= Racks.Count) return null;
            return Racks[index];
        }
    }
}
