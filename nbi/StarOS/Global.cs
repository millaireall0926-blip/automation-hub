
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Xml;
using StarOS.Model;
using StarOS.ViewModel;
using Serilog;
using Serilog.Core;


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

            XmlNode root =  doc.SelectSingleNode("/bib-id");
            if (root.Attributes["use"] != null)
                IsUseBibMap =  Convert.ToInt32(root.Attributes["use"].Value) == 1 ? true : false;

            if (IsUseBibMap == false) return;
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


        public static string    gRouterIP;
        public static int       gRouterPort;
        public static string    gDirSystem;
        public static string    gDirResult;
        public static string    gDirSub;
        public static string    gDirSlot;
        public static string    gDirData;
        public static string    gDirPower;
        public static string    gDirDiag;
        public static string    gDirMap;


        public static string gEquipID = "ST-805";        
        public static int gChamberUse = 0;
        public static string gChamberModel = "jst";
        public static int gLampOwn = 0;
        public static int gLampCtrl = 0;

        public static int gLoaderUse = 0;
        public static string gLoaderPath = "";

        public static int gUnloaderUse = 0;
        public static string gUnloaderPath = "";

        public static bool gShowLogo = true;


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
            var node = doc.SelectSingleNode("/staros");
            if (node != null)
            {
                if (node.Attributes["logo"] != null)
                {
                    if (Convert.ToInt32(node.Attributes["logo"].Value) == 0)
                        gShowLogo = false;
                }
            }

            node = doc.SelectSingleNode("/staros/router");
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

            node = doc.SelectSingleNode("/staros/log/map");
            if (node != null)
            {
                gDirMap = node.Attributes["path"].Value;
                System.IO.Directory.CreateDirectory(gDirMap);
            }

            node = doc.SelectSingleNode("/staros/chamber");
            if (node != null)
            {
                gEquipID = node.Attributes["id"].Value;
                gChamberUse = Convert.ToInt32(node.Attributes["use"].Value);
                gChamberModel = node.Attributes["model"].Value;
                gLampOwn = Convert.ToInt32(node.Attributes["own"].Value);
                gLampCtrl = Convert.ToInt32(node.Attributes["ctrl"].Value);                
            }

            node = doc.SelectSingleNode("/staros/loader");
            if (node != null )
            {
                gLoaderUse = Convert.ToInt32(node.Attributes["use"].Value);
                gLoaderPath = node.Attributes["path"].Value;
            }

            node = doc.SelectSingleNode("/staros/unloader");
            if (node != null)
            {
                gUnloaderUse = Convert.ToInt32(node.Attributes["use"].Value);
                gUnloaderPath = node.Attributes["path"].Value;
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
        const string Program = "StarOS";
        const string Date = "2025/09/09";
        const string Ver = "V1.3.6 (HEAD Check)";        
        const string Author = "uiop";
        public static string getVer() => Program + "    " + Date + " " + Ver;
    }
    /*  
     *  
     *  const string Program = "Star OS";
        const string Date = "2025/09/09";
        const string Ver = "V1.3.6 (HEAD Check)";
     *      header magic val check 
     
        const string Program = "Star OS";
        const string Date = "2025/08/14";
        const string Ver = "V1.3.5.H (Mask Process)";
            dut Mask 관련 기능 추가, 나중에 os에서도 줄수있게...

        const string Date = "2025/07/23";
        const string Ver = "V1.3.3 (Bin Map)";
            BinMap Folder add, Bin Map Create when finish test


        const string Date = "2025/07/04";

        const string Ver = "V1.3.0 (OS Rebooting with Maintain RunStat)";
     
     *  const string Date = "2025/05/30";
        const string Ver = "V1.2.1(Force Stop by Critical Alram of PLC)";        
     
        const string Date = "2025/05/16";
        const string Ver = "V1.2.0(DB Load)";        
            DB Load Product ID로

        const string Date = "2025/03/28";
        const string Ver = "V1.2.0  ";
            Barcode 기계 Enter 들어가는것때문에 Lot입력창 Enter 입력 제거 

     *  
     *  
        const string Date = "2025/03/28";
        const string Ver = "V1.2.0  ";
         BID Parity Check추가.
     *             
        const string Date = "2025/02/27";
        const string Ver = "V1.1.9  (DB PGM)";
            add : load db pgm 기능 추가  config.xml에 <db>에 loader attribute 추가
            mod : bib info Textformat / hex value 선택 가능하도록. bibmap.xml 에 use attribute 추가
     *     
     *     
     * const string Date = "2024/11/26";
       const string Ver = "V1.1.8  (graph err mod)";    
            mod : power graph string -> double error 날수있는경우 차단. double.TryParse 사용
     *     
     * Ver 1.1.8 - Diag Flag
     *      add : DIAG Flag add, Diag Control add
     * Ver 1.1.7 - MEAS DATA ADD
     *      add : MEAS_DATA
     * 
     * * Ver 1.1.6 - loader save Task 분리 
     *      mod : Loader 쪽 연결때문에 pc쪽 다른 네트웍에도 영향주는것이 아닌지.. saveLoaderFile  Task 분리.
     * 
     * 
     * Ver 1.1.5 - tlog + unloader
     *      fix : board temp log저장 하도록 system 폴더로.
     * *
     * * * Ver 1.1.4 - tlog
     *      fix : board temp log저장 하도록 system 폴더로.
     * * Ver 1.1.3
     *      fix : plc alarm process  bug fix
     *      
     *      
     * Ver 1.1.2
     *      mod : 간헐적 죽는부분. addlog에서 slot 의 logque 동시 사용때문인듯
     *      mod : result view 항상 최상위 출력 안하도록. 더블클릭시만.
     *      add : bin 별 count list 출력.
            mod : SuperSocket -> Easy Client 
            mod : 신규 Router대응
     * 
     * Ver 1.1.1
     *      add : bin color apply
     * 
     * Ver 1.1.0
     *      mod : when contact, init step list. existing include load
     *      
     * Ver 1.0.9
     *      add : BIB Col Row index print*    
     * 
     * * Ver 1.0.8
     *      add : db process function add
     *      
     * Ver 1.0.7
     *      add : web function add. temp , web-res.
     *      
     * Ver 1.0.6
     *      add : After Diag Test, Open Diag Log File by System Log Double Click 
     *      
     * Ver 1.0.5
     *      mod : send kill msg when abnormally Error.  for pg stop, power off
     * 
     * * * Ver 1.0.4
     *      add : Cycle Test, Column Count, etc ...
     *      
     * * Ver 1.0.3
     *      add : FBlock, FBlock Count Result Process
     *      
     * Ver 1.0.2
     *      mod : UI Process Thread Priority Apply
     *      add : Nand Sub Data File Recv
     *      
     * Ver 1.0.1
     *      add : Multi Chip View Create
     * 
     */

}
