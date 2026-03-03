using Serilog;
using Serilog.Core;
using StarOS.Model;
using StarOS.ViewModel;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Xml;


namespace StarOS
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
        REGISTER = 0x1000,

    }

    public static class Global
    {
        public static MainWindow    gMainWin;        
        public static MainVM        gMainVM;
        public static SlotVM        gSlotVM;
        public static ResultVM      gResultVM;
        public static PowerVM       gPowerVM;
        public static ChamberVM     gChamberVM;
        public static TestInfoVM    gTestVM;
        public static Net           gNet;

        
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


        public static bool IsUseBibMap = false;
        public static Dictionary<int, string> BidTypeMap = new Dictionary<int, string>();
        public static Dictionary<int, string> BidDevMap = new Dictionary<int, string>();
        

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

            XmlNode root =  doc.SelectSingleNode("/bib-id");
            if (root.Attributes["use"] != null)
                IsUseBibMap =  Convert.ToInt32(root.Attributes["use"].Value) == 1 ? true : false;

            if (IsUseBibMap == false) return;
            XmlNodeList list = doc.SelectNodes("/bib-id/type/item");
            foreach (XmlNode node in list)
            {
                int key = Convert.ToInt32(node.Attributes["value"].Value);
                BidTypeMap.Add(key, node.Attributes["text"].Value);
            }
            list = doc.SelectNodes("/bib-id/device/item");
            foreach (XmlNode node in list)
            {
                int key = Convert.ToInt32(node.Attributes["value"].Value);
                BidDevMap.Add(key, node.Attributes["text"].Value);
            }
            list = doc.SelectNodes("/bib-id/ball/item");            
        }


        public static string    gRouterIP;
        public static int       gRouterPort;
        public static string    gDirSystem;
        public static string    gDirResult;
        public static string    gDirSub;
        public static string    gDirSlot;
        public static string    gDirData;
        public static string    gDirPower;
        public static string    gDirDiag;


        public static string gEquipID = "ST-805";        
        public static int gChamberUse = 0;
        public static string gChamberModel = "jst";
        public static int gLampOwn = 0;
        public static int gLampCtrl = 0;
        public static bool gEnableWatchDog = false;
        public static int gWatchDocTimeout = 600; // default 10m

        public static int gLoaderUse = 0;
        public static string gLoaderPath = "";

        public static int gColDisplay = 1; // 0 = number, 1 = alpabet


        public static void loadConfig()
        {
            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load("StarOS.xml");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }
            var node = doc.SelectSingleNode("/staros/router");
            if (node != null)
            {
                gRouterIP = node.Attributes["ip"].Value;
                gRouterPort = Convert.ToInt32(node.Attributes["port"].Value);
            }

            node = doc.SelectSingleNode("/staros/log/system");
            if (node != null)
            {
                gDirSystem = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirSystem);
            }

            node = doc.SelectSingleNode("/staros/log/result");
            if (node != null) 
            {
                gDirResult = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirResult);
            }

            node = doc.SelectSingleNode("/staros/log/sub");
            if (node != null)
            {
                gDirSub = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirSub);
            }

            node = doc.SelectSingleNode("/staros/log/slot");
            if (node != null)
            {
                gDirSlot = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirSlot);
            }

            node = doc.SelectSingleNode("/staros/log/data");
            if (node != null)
            {
                gDirData = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirData);
            }

            node = doc.SelectSingleNode("/staros/log/power");
            if (node != null)
            {
                gDirPower = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirPower);
            }

            node = doc.SelectSingleNode("/staros/log/diag");
            if (node != null)
            {
                gDirDiag = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirDiag);
            }

            node = doc.SelectSingleNode("/staros/bib");
            if (node != null)
            {
                if (node.Attributes["col-display"].Value == "number")
                    gColDisplay = 0;
            }

            node = doc.SelectSingleNode("/staros/chamber");
            if (node != null)
            {
                gEquipID = node.Attributes["id"].Value;
                gChamberUse = Convert.ToInt32(node.Attributes["use"].Value);
                gChamberModel = node.Attributes["model"].Value;
                gLampOwn = Convert.ToInt32(node.Attributes["own"].Value);
                gLampCtrl = Convert.ToInt32(node.Attributes["ctrl"].Value);                

                if (node.Attributes["watchdog"] != null)
                {
                    gEnableWatchDog = Convert.ToInt32(node.Attributes["watchdog"].Value) == 1 ? true : false;
                    gWatchDocTimeout = Convert.ToInt32(node.Attributes["watchdog_timeout"].Value);
                }

            }

            node = doc.SelectSingleNode("/staros/loader");
            if (node != null )
            {
                gLoaderUse = Convert.ToInt32(node.Attributes["use"].Value);
                gLoaderPath = node.Attributes["path"].Value;
            }



        }

        public static void loadBinColor() {

            string path = "binColor.xml";
            

            XmlDocument doc = new XmlDocument();
            try {
                doc.Load(path);
            }
            catch (Exception ex) {
                HandyControl.Controls.Growl.ClearGlobal();
                HandyControl.Controls.Growl.InfoGlobal(ex.Message);
                return;
            }

            XmlNodeList list = doc.SelectNodes("/binColor/item");
            foreach (XmlNode node in list) {
                int bin = Convert.ToInt32(node.Attributes["bin"].Value);
                string sCol = node.Attributes["color"].Value;
                SolidColorBrush br = new BrushConverter().ConvertFromString(sCol) as SolidColorBrush;
                BinColorDic.Add(bin, br);
            }
        }

    }

    public static class VerInfo {
        const string Program = "Star OS";
        const string Date = "2026/02/24";
        const string Ver = "V1.1.6.EX Result Modify";
        const string Author = "uiop";
        public static string getVer() => Program + "    " + Date + " " + Ver;
    }
	/*
		const string Date = "2026/02/24";
        const string Ver = "V1.1.6.EX Result Modify";
              ResultVM -> TextMode 함수내 setDutColorPF(slot), setDutColorBin() 추가하여 색상 반영
              V1.1.4 RollBack
		
        const string Program = "Star OS";
        const string Date = "2026/02/11";
        const string Ver = "V1.1.5.EX (All SOLTS / SINGLE SELECT)";
              ResultVM -> TextMode 함수내 setDutColorPF(slot), setDutColorBin() 추가하여 색상 반영
              All Slots, Single 라디오 버튼 추가 
      
        const string Program = "Star OS";
        const string Date = "2025/09/03";
        const string Ver = "V1.1.3.H (OS Enhanced Restorartion )";
            StarClient Resolve packet 보강,
            재실행시 SendDBTime 오동작 안하도록.
     
        const string Program = "Star OS";
        const string Date = "2025/08/13";
        const string Ver = "V1.1.2.H (OS Enhanced Restorartion )";
            NowTest 생성시 5초후 make selection

        const string Date = "2025/07/14";
            sendAuthority -> sendPgmRun  할때 
            한꺼번에 실행시 실행 안되는 경우가 있음 
            두 커맨드를 한번에 보낼때 Sleep(10) 으로 시차를 좀 주는걸로.  
                -> 아니면 que 형태를 고려해바야함.

        const string Date = "2025/07/11";
        const string Ver = "V1.1.1.H (CMPack Proc Improve )";
            cm data recv try , catch 
            target temp 1번만 수행하게..
            run도 1번만 수행하게..

        const string Date = "2025/06/27";
        const string Ver = "V1.1.0.H (PLC Watchdog Option)";
     
        const string Date = "2025/06/24";
        const string Ver = "V1.0.8.H (Sorter Format Apply)";

        const string Date = "2025/06/02";
        const string Ver = "V1.0.7.H (Result BIB Map Display)";
     
        const string Date = "2025/05/30";
        const string Ver = "V1.0.6.H (Force Stop by Critical Alram of PLC)";
     
       const string Date = "2025/05/16";
       const string Ver = "V1.0.5.H  ";
         DB Load Product ID로
     
        const string Date = "2025/03/28";
        const string Ver = "V1.0.4.H  ";
         BID Parity Check추가.

        const string Date = "2025/02/27";
        const string Ver = "V1.0.3.H  ";
        DB Load 추가

        const string Date = "2024/11/26";
        const string Ver = "V1.0.2.H  ";
        regview 추가
        regrecv , 기능구현

     * Ver 1.0.1 -
     *      fix : Graph 이상하게 나오던것 수정
     * 
     
     */

}
