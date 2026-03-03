using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using System.Windows.Media;
using System.Windows.Threading;
using StarBase;
using StarBase.Net;
using StarRouter.Comm;

using Serilog.Core;
using Serilog;
using System.Xml.Linq;
using System.IO;
using HandyControl.Controls;

namespace StarRouter.ViewModel {

    public class WebData {
        public WebData() { }
        public string PV { get; set; }
        public string State { get; set; }
    }

    public enum ITEM_DEF : int {
        DISCON = 0,
        NODEAMON,
        CONNECT,
        OS,
        WEB,
        MAINT,
        UNKNOWN      
    }

    public enum LOG_TYPE {
        ERROR = 0,
        DEBUG = 1
    }
    public class ItemDef {
        public ItemDef(BitmapImage image, SolidColorBrush brush) {
            Image = image;            
            Brush = brush;
        }

        public BitmapImage Image;        
        public SolidColorBrush Brush;
    }

    public class MainVM : BaseVM {
        StarServer _server;

        public LOG_TYPE LogLevel { get; set; } = (int)LOG_TYPE.ERROR;
        public Logger ErrLogger { get; set; }
        public Logger DbgLogger { get; set; }

        public WebData _webData = new WebData();

        public ObservableCollection<StarCM> _CMs = new ObservableCollection<StarCM>();
        public ObservableCollection<StarCM> CMs { get => _CMs; set { _CMs = value; OnPropertyChanged(nameof(CMs)); } }
        public ObservableCollection<StarProduct> _Products = new ObservableCollection<StarProduct>();
        public ObservableCollection<StarProduct> Products { get => _Products; set { _Products = value; OnPropertyChanged(nameof(Products)); } }

        public string PlcData { get; set; }

        public string _editName;
        public string EditName { get => _editName; set {_editName = value; OnPropertyChanged(nameof(EditName)); }}

        public string _editIP;
        public string EditIP { get => _editIP; set { _editIP = value; OnPropertyChanged(nameof(EditIP)); } }

        public int _editPort;
        public int EditPort { get => _editPort; set { _editPort = value; OnPropertyChanged(nameof(EditPort)); } }



        #region Loop Logic Timer 
        public DispatcherTimer _timerCMCon = new DispatcherTimer();
        public DispatcherTimer _timerWebData = new DispatcherTimer();
        #endregion

        public bool UpdateFlag_CMList { get; set; } = false;

        public Collection<ItemDef> _itemDefs = new Collection<ItemDef>();        

        public void addSlotDef(string uri, Color col)
            => _itemDefs.Add(new ItemDef(new BitmapImage(new Uri(uri, UriKind.RelativeOrAbsolute)), new SolidColorBrush(col)));


        public MainVM() {
            addSlotDef(@"/Resource/off.png", Colors.DimGray);
            addSlotDef(@"/Resource/nodaemon.png", Colors.LightGray);
            addSlotDef(@"/Resource/on.png", Colors.Honeydew);
            addSlotDef(@"/Resource/os.png", Colors.LightGreen);
            addSlotDef(@"/Resource/web.png", Colors.Tomato);
            addSlotDef(@"/Resource/maint.png", Colors.Violet);
            addSlotDef(@"/Resource/unkown.png", Colors.DimGray);

        }

        public void setLogger(string path) {
                        
            ErrLogger = new LoggerConfiguration()
                .WriteTo.Console()
                .WriteTo.File(path + @"\RouterError_.log", rollingInterval: RollingInterval.Month,
                      outputTemplate: "{Timestamp:[yyyy-MM-dd HH:mm:ss]} [{Level:u3}] {Message:lj}{NewLine}")
                .CreateLogger();
            DbgLogger = new LoggerConfiguration()
                .WriteTo.File(path + @"\RouterDebug_.log", rollingInterval: RollingInterval.Month,
                      outputTemplate: "{Timestamp:[yyyy-MM-dd HH:mm:ss]} [{Level:u3}] {Message:lj}{NewLine}")
                .CreateLogger();
        }

        
        
        public void addLog(LOG_TYPE type, string log, string name, string where = "") {
            if ((int)type > (int)LogLevel)
                return;
            string func = (where != "") ? ("[" + where + "]") : "";
            if (ErrLogger == null || DbgLogger == null) return;

            switch (type) {
                case LOG_TYPE.ERROR:                    
                    ErrLogger.Error($"[{name}] {log} {func}  ");
                    break;
                case LOG_TYPE.DEBUG:
                    DbgLogger.Information($"[{name}] {log} {func}  ");
                    break;
            }
        }

        public void initServer() {
            _server = new StarServer();
            _server.serverStart();
        }

        public void initTimers() {
            CheckCMCon(null, null);

            _timerCMCon.Interval = TimeSpan.FromSeconds(5);
            _timerCMCon.Tick += CheckCMCon;
            _timerCMCon.Start();

            _timerWebData.Interval = TimeSpan.FromSeconds(10);
            _timerWebData.Tick += SendTempToWeb;
            _timerWebData.Start();
        }

        public void SendTempToWeb(object obf, EventArgs e) {
            foreach( StarProduct product in Products) {
                if (product.ProductType != MSG_TYPE.SESSION_WEB)
                    continue;
                StringBuilder msg = new StringBuilder();
                msg.Append($"<web-data><temp pv=\"{_webData.PV}\" st=\"{_webData.State}\"/>" +
                    $"<slot>");
                foreach (StarCM cm in CMs) {
                    msg.Append($"<cm id=\"{cm.Name}\" " +
                        $"online=\"{cm.IsConnected}\" " +
                        $"pgm=\"{cm.IsPgm}\" " +
                        $"diag=\"{cm.IsDiag}\" " +
                        $"contact=\"{cm.IsContact}\" " +
                        $"bid=\"{cm.BibID}\" " +
                        $"lot=\"{cm.LotID}\" " +
                        $"name=\"{cm.PgmName}\" " +
                        $"step=\"{cm.StepName}\" " +
                        $"st=\"{cm.StartTime}\" />");                    
                }
                msg.Append($"</slot></web-data>");

                product._sender.writeResult(product.Index, RESULT.OK,
                    "web-data", msg.ToString());
                break;
            }
        }

        public void CheckCMCon(object obj, EventArgs e) 
        {
            // prev state save
            try {
                foreach (StarCM cm in CMs) {
                    //if (cm.IP != "192.168.50.2") continue;
                    if (!cm.IsPing) {
                        cm.doPing();
                        continue;
                    }
                    if (!cm.IsConnected) {
                        cm.doConnect();
                        continue;
                    }
                    if (cm.TimeLast + TimeSpan.FromSeconds(30) < DateTime.Now) {
                        cm.doPing();
                        cm.TimeLast = DateTime.Now;
                    }
                }
            }catch(Exception ex) {
                addLog(LOG_TYPE.ERROR, ex.Message, "MainVM", System.Reflection.MethodBase.GetCurrentMethod().Name);
            }

            if (UpdateFlag_CMList) {
                foreach (StarProduct sp in Products) {
                    sp.sendCMList();                 
                }
                UpdateFlag_CMList = false;
            }


            // compare prev, send update list
        }

    }
}
