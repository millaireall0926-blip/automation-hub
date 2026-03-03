using HTCRack_CXL.Base;
using HTCRack_CXL.Model;
using PropertyChanged;
using Serilog;
using Serilog.Core;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;
using System.Xml;

namespace HTCRack_CXL.ViewModel
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
        public bool _isTamsAll = false;
        public bool IsTamsAll
        {
            get => _isTamsAll;
            set
            {
                _isTamsAll = value;

                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[User][ALL] : Tams {_isTamsAll}");

                foreach (RackModel rack in Racks)
                    rack.IsTams = _isTamsAll;

                if (_isTamsAll)
                    EnableSVAll = false;
                else
                    EnableSVAll = true;
            }
        }
        public bool EnableSVAll { get; set; } = true;
        public int LayoutColumnCount { get; set; } = 2;
        public int MaxFixDimmCount = 32;
        public double SVAll { get; set; }

        Visibility _engrmodevisibility = Visibility.Hidden;
        public Visibility EngrmodeVisibility
        {
            get => _engrmodevisibility;
            set
            {
                _engrmodevisibility = value;
                if (_engrmodevisibility == Visibility.Visible)
                {
                    //todo
                    //IsTamsAllEnable = true;
                    foreach (RackModel rm in Racks)
                    {
                        rm.RackEngrmodeVisibility = Visibility.Visible;
                        
                        //todo
                        //rm.IsRackTamsEnable = true;
                        //foreach (BoxModel bm in rm.Boxes)
                        //    bm.IsBoxTamsEnable = true;
                    }
                }
                else if (_engrmodevisibility == Visibility.Hidden)
                {
                    //IsTamsAll = true;

                    //todo
                    //IsTamsAllEnable = false;
                    foreach (RackModel rm in Racks)
                    {
                        rm.RackEngrmodeVisibility = Visibility.Hidden;

                        //todo
                        //foreach (BoxModel bm in rm.Boxes)
                        //{
                        //    bm.IsBoxTams = true;
                        //    bm.IsBoxTamsEnable = false;
                        //}
                    }
                }
            }
        }

        public DelegateCommand DGCmdAll { get; set; }

        ObservableCollection<RackModel> _racks = new ObservableCollection<RackModel>();
        public ObservableCollection<RackModel> Racks { get => _racks; set { _racks = value; PC(nameof(Racks)); } }

        [DoNotNotify]
        public Logger SysSerilog { get; set; }
        public ListView ListSysLog;
        ObservableCollection<SystemLog> _sysLogs = new ObservableCollection<SystemLog>();

        public ObservableCollection<SystemLog> SysLogs { get { return _sysLogs; } set { _sysLogs = value; PC("SysLogs"); } }

        public MainPageVM()
        {
            var columnNode = Global.gConfigDoc.SelectSingleNode("/cxl-rack/rack-map");

            string logo = columnNode.Attributes["logo"].Value;
            if (logo == "star")
                Global.gMainVM.LogoPath = "/Resource/star.png";
            else if (logo == "gsi")
                Global.gMainVM.LogoPath = "/Resource/gsi.png";
            else if (logo == "cube")
                Global.gMainVM.LogoPath = "/Resource/cube.png";

            LayoutColumnCount = Convert.ToInt32(columnNode.Attributes["column"].Value);

            

            var nodes = Global.gConfigDoc.SelectNodes("/cxl-rack/rack-map/rack");
            foreach (XmlNode node in nodes)
            {
                int id = Convert.ToInt32(node.Attributes["id"].Value);
                // id supply as index
                Racks.Add(new RackModel(id));
            }
            
            var tamsNode = Global.gConfigDoc.SelectSingleNode("/cxl-rack/tams");
            IsTamsAll = Convert.ToInt32(tamsNode.Attributes["all"].Value) == 1;
            
            Global.gMainPageVM = this;

            DGCmdAll = new DelegateCommand( p => {

                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[User][ALL] : {p}");
                if (p.ToString() == "Off")
                {
                    MessageBoxResult res = HandyControl.Controls.MessageBox.Show($"Do You Want to Shut Down All Unit?", "Power Off", MessageBoxButton.YesNo, MessageBoxImage.Exclamation);
                    if (res == MessageBoxResult.No) return;
                }

                foreach (RackModel rack in Racks)
                {
                    //todo

                    switch (p)
                    {
                        case "Set":
                            rack.SetSV = SVAll;
                            if (!rack.IsTams)
                                rack.setTemp();
                            break;
                        case "Start":
                            if (rack.RackStat == RACK_STAT.DISCONNECT) continue;
                            rack.startTest();
                            break;
                        case "Stop":
                            if (rack.RackStat == RACK_STAT.DISCONNECT) continue;
                            //rack.SideOpenRun = false;
                            rack.PrestopTest();
                            break;
                        case "Select":
                            rack.IsTCUnit = true;
                            break;
                        case "None":
                            rack.IsTCUnit = false;
                            break;

                        case "Off":
                            rack.clickedAllCmdPower(false);
                            break;
                        case "On":
                            rack.clickedAllCmdPower(true);
                            break;

                        case "PopUpClear":
                            rack.ShowBox = System.Windows.Visibility.Hidden;
                            if (rack.RackStat == RACK_STAT.ERROR || rack.RackStat == RACK_STAT.DONE)
                                rack.RackStat = RACK_STAT.STOP;

                            foreach (BoxModel bm in rack.Boxs)
                            {
                                bm.BoxShowBox = System.Windows.Visibility.Hidden;
                                if (bm.BoxStat == BOX_STAT.ERROR || bm.BoxStat == BOX_STAT.DONE)
                                {
                                    bm.BoxStat = BOX_STAT.STOP;
                                    foreach (SiteModel sm in bm.Sites)
                                        sm.SiteStat = SITE_STAT.STOP;
                                }
                            }
                            break;
                        case "Clear":
                            SysLogs.Clear();
                            break;
                    }
                    
                }
            });
        }

        public void initLogFile()
        {
            SysSerilog = new LoggerConfiguration()
                //                .WriteTo.Console()
                .WriteTo.File(Global.gConfigVM.SysLogPath + "\\" + "Event_.log", rollingInterval: RollingInterval.Day,
                outputTemplate: "{Timestamp:[yyyy-MM-dd HH:mm:ss]} [{Level:u3}] {Message:lj}{NewLine}")
                .CreateLogger();
            
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[Application] : Start");
        }

        public void addLog(LOG_TYPE type, string msg)
        {
            if (SysSerilog == null) return;

            SystemLog log = new SystemLog(type, msg);
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                new Action(() =>
                {
                    SysLogs.Add(log);
                    if (SysLogs.Count > 100)
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

        public RackModel getRack(int index)
        {
            if (index >= Racks.Count) return null;
            return Racks[index];
        }
    }
}
