using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Xml;
using StarQMOS.Model;
using StarQMOS.ViewModel;
using Serilog;
using Serilog.Core;


namespace StarQMOS
{
    public enum UI : int
    {
        // Priority 1
        DONE = 0,
        INFO = 1,
        MENU = 2,
        SLOTLOG = 4,
        STEPLIST = 8,
        CHAM = 0x10,
        POWER = 0x20,

        // Priority 2
        BIB = 0x100,
        BIB_SIZE = 0x200,
        RESULT = 0x400,
        RESULT_SUB = 0x800,

    }

    public static class Global
    {
        public static MainWindow gMainWin;
        public static MainVM gMainVM;
        public static SlotVM gSlotVM;
        public static ResultVM gResultVM;
        public static PowerVM gPowerVM;
        public static ChamberVM gChamberVM;
        public static TestInfoVM gTestVM;
        public static Net gNet;
        public static HandlerVM gHandlerVM;


        public static Logger gSystemLog;



        static Global()
        {
            initResource();
            loadConfig();
            loadBibMap();
            loadBinColor();

            gSystemLog = new LoggerConfiguration()
                //                .WriteTo.Console()
                .WriteTo.File(gDirSystem + "\\" + "SystemLog_.log", rollingInterval: RollingInterval.Month,
                outputTemplate: "{Timestamp:[yyyy-MM-dd HH:mm:ss]} [{Level:u3}] {Message:lj}{NewLine}")
                .CreateLogger();
        }



        public static Collection<SlotDef> gStatDefs = new Collection<SlotDef>();
        public static void addSlotDef(STAT_TYPE type, string uri, Color col)
            => gStatDefs.Add(new SlotDef(new BitmapImage(new Uri(uri, UriKind.RelativeOrAbsolute)), type.ToString(), new SolidColorBrush(col)));
        public static void initResource()
        {
            addSlotDef(STAT_TYPE.DISCONNECT, @"/Resource/discon.bmp", Colors.Gray);
            addSlotDef(STAT_TYPE.CONNECT, @"/Resource/con1.bmp", Colors.White);
            addSlotDef(STAT_TYPE.INSERT, @"/Resource/bib.bmp", Colors.White);
            addSlotDef(STAT_TYPE.LOAD, @"/Resource/ready.bmp", Colors.Yellow);
            addSlotDef(STAT_TYPE.CONTACT, @"/Resource/ready.bmp", Colors.LightSkyBlue);
            addSlotDef(STAT_TYPE.STOP, @"/Resource/ready.bmp", Colors.Yellow);
            addSlotDef(STAT_TYPE.MASK, @"/Resource/discon.bmp", Colors.Gray);
            addSlotDef(STAT_TYPE.LOADING, @"/Resource/ready.bmp", Colors.Yellow);
            addSlotDef(STAT_TYPE.CONTACTING, @"/Resource/contact.bmp", Colors.LightBlue);
            addSlotDef(STAT_TYPE.RUNNING, @"/Resource/run.bmp", Colors.LightGreen);
            addSlotDef(STAT_TYPE.ABORT, @"/Resource/ready.bmp", Colors.DarkOrange);
            addSlotDef(STAT_TYPE.ERROR, @"/Resource/error.bmp", Colors.IndianRed);
            addSlotDef(STAT_TYPE.DIAG, @"/Resource/diag.bmp", Colors.Violet);
        }



        public static Dictionary<int, string> BidFuncMap = new Dictionary<int, string>();
        public static Dictionary<int, string> BidDevMap = new Dictionary<int, string>();
        public static Dictionary<int, string> BidBallMap = new Dictionary<int, string>();

        public static Dictionary<int, SolidColorBrush> BinColorDic = new Dictionary<int, SolidColorBrush>();

        public static void loadBibMap()
        {
            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load("bidmap.xml");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }

            XmlNodeList list = doc.SelectNodes("/bib-id/function/item");
            foreach (XmlNode node in list)
            {
                int key = Convert.ToInt32(node.Attributes["value"].Value);
                BidFuncMap.Add(key, node.Attributes["text"].Value);
            }
            list = doc.SelectNodes("/bib-id/device/item");
            foreach (XmlNode node in list)
            {
                int key = Convert.ToInt32(node.Attributes["value"].Value);
                BidDevMap.Add(key, node.Attributes["text"].Value);
            }
            list = doc.SelectNodes("/bib-id/ball/item");
            foreach (XmlNode node in list)
            {
                int key = Convert.ToInt32(node.Attributes["value"].Value);
                BidBallMap.Add(key, node.Attributes["text"].Value);
            }
        }


        public static string gRouterIP;
        public static int gRouterPort;
        public static string gHandlerIP;
        public static int gHandlerPort;

        public static string gDirSystem;
        public static string gDirResult;
        public static string gDirSub;
        public static string gDirSlot;
        public static string gDirData;
        public static string gDirPower;
        public static string gDirDiag;


        public static string gEquipID = "ST-805";
        public static int gChamberUse = 0;
        public static string gChamberModel = "jst";
        public static int gLampOwn = 0;
        public static int gLampCtrl = 0;

        public static int gLoaderUse = 0;
        public static string gLoaderPath = "";

        public static bool gShowLogo = true;


        public static void loadConfig()
        {
            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load("StarQMOS.xml");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }
            var node = doc.SelectSingleNode("/StarQMOS");
            if (node != null)
            {
                if (node.Attributes["logo"] != null)
                {
                    if (Convert.ToInt32(node.Attributes["logo"].Value) == 0)
                        gShowLogo = false;                       
                }
            }

            node = doc.SelectSingleNode("/StarQMOS/router");
            if (node != null)
            {
                gRouterIP = node.Attributes["ip"].Value;
                gRouterPort = Convert.ToInt32(node.Attributes["port"].Value);
            }

            node = doc.SelectSingleNode("/StarQMOS/log/system");
            if (node != null)
            {
                gDirSystem = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirSystem);
            }

            node = doc.SelectSingleNode("/StarQMOS/log/result");
            if (node != null)
            {
                gDirResult = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirResult);
            }

            node = doc.SelectSingleNode("/StarQMOS/log/sub");
            if (node != null)
            {
                gDirSub = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirSub);
            }

            node = doc.SelectSingleNode("/StarQMOS/log/slot");
            if (node != null)
            {
                gDirSlot = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirSlot);
            }

            node = doc.SelectSingleNode("/StarQMOS/log/data");
            if (node != null)
            {
                gDirData = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirData);
            }

            node = doc.SelectSingleNode("/StarQMOS/log/power");
            if (node != null)
            {
                gDirPower = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirPower);
            }

            node = doc.SelectSingleNode("/StarQMOS/log/diag");
            if (node != null)
            {
                gDirDiag = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirDiag);
            }

            node = doc.SelectSingleNode("/StarQMOS/chamber");
            if (node != null)
            {
                gEquipID = node.Attributes["id"].Value;
                gChamberUse = Convert.ToInt32(node.Attributes["use"].Value);
                gChamberModel = node.Attributes["model"].Value;
                gLampOwn = Convert.ToInt32(node.Attributes["own"].Value);
                gLampCtrl = Convert.ToInt32(node.Attributes["ctrl"].Value);
            }

            node = doc.SelectSingleNode("/StarQMOS/loader");
            if (node != null)
            {
//                gLoaderUse = Convert.ToInt32(node.Attributes["use"].Value);
//                gLoaderPath = node.Attributes["path"].Value;
                gHandlerIP = node.Attributes["ip"].Value;
                gHandlerPort = Convert.ToInt32(node.Attributes["port"].Value);

            }



        }

        public static void loadBinColor()
        {

            string path = "binColor.xml";


            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load(path);
            }
            catch (Exception ex)
            {
                HandyControl.Controls.Growl.ClearGlobal();
                HandyControl.Controls.Growl.InfoGlobal(ex.Message);
                return;
            }

            XmlNodeList list = doc.SelectNodes("/binColor/item");
            foreach (XmlNode node in list)
            {
                int bin = Convert.ToInt32(node.Attributes["bin"].Value);
                string sCol = node.Attributes["color"].Value;
                SolidColorBrush br = new BrushConverter().ConvertFromString(sCol) as SolidColorBrush;
                BinColorDic.Add(bin, br);
            }
        }

    }

    public static class VerInfo
    {
        const string Program = "QMOS (pgm endtest mon off)";
        const string Date = "2025/06/23";
        const string Ver = "V1.0.2 ";
        const string Author = "uiop";
        public static string getVer() => Program + "    " + Date + " " + Ver;
    }
}
/*     
const string Date = "2025/06/23";
const string Ver = "V1.0.2 ";
    mod : pgm process run/endtest감지 끄는걸로.
    mod : handle run 시 running됬는지 체크 안함. protocal에 맞김.

const string Program = "StarQMOS (Log Total Count Add)";
const string Date = "2025/03/27";
const string Ver = "V1.0.1 ";    
    mod : add Total Count in Log


 2024/09/24

 const string Date = "2024/11/26";
 const string Ver = "V1.0.0 ";    
    mod : power graph string -> double error 날수있는경우 차단. double.TryParse 사용

 Ver 1.0.0 - OSQM Ver 
      add : Release First


*/