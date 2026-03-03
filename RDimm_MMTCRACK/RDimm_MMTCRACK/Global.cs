using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.Net;
using RDimm_MMTCRACK.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Xml;

namespace RDimm_MMTCRACK
{
    public class Global
    {
        static public MainWindow gMainWindow;
        static public MainVM gMainVM;
        static public MainPageVM gMainPageVM;
        static public ConfigVM gConfigVM;
        static public EngineerVM gEngineerVM;
        static public GraphVM gGraphVM;
        static public DimmVM gDimmVM;

        static public TconCom gTconCom;
        static public SiteCom gSiteCom;
        static public FlowMeterCom gFlowMeterCom;
        static public AlarmCom gAlarmCom;
        static public CaptureCom gCaptureCom;

        static public int BimetalErrorCnt = 10;

        public static SolidColorBrush[] gRackColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.Transparent),      // Normal
            new SolidColorBrush(Colors.LightPink),            // as mode            
            //new SolidColorBrush(Colors.Gray)           // tcunit 
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0x3D3837)),
        };
        public static SolidColorBrush[] gBoxColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.Transparent),           // Normal
            new SolidColorBrush(Colors.LightPink),          // as mode
           // new SolidColorBrush(Colors.Gray)           // tcunit 
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0x3D3837)),
        };
        public static SolidColorBrush[] gBoarderColors = new SolidColorBrush[]
        {
            // RACK_STAT Index
            new SolidColorBrush(Colors.LightGray),  // IDEL            
            new SolidColorBrush(Colors.LightGray),  // STOP
            new SolidColorBrush(Colors.PaleGreen),     // PERV_RUN
            new SolidColorBrush(Colors.Lime),          // RUN
            new SolidColorBrush(Colors.DodgerBlue),    // Done
            new SolidColorBrush(Colors.Aquamarine),        //DEFOSET 
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFF5B79)),       // ERROR
            new SolidColorBrush(Colors.LightGray),  // NOCON
            new SolidColorBrush(Colors.Red)  // INSPECTION -> as mode
        };
        public static SolidColorBrush[] gWarningColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.Black),
            new SolidColorBrush(Colors.Red)
        };
        public static SolidColorBrush[] gMotorStatColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.Plum),    // motor close
            new SolidColorBrush(Colors.Yellow)   // motor open
        };
        public static SolidColorBrush[] gAlramColors = new SolidColorBrush[]
        {
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0x6D79FF)), // Done
            new SolidColorBrush(Colors.LightPink),
            new SolidColorBrush(Colors.LightPink),
            new SolidColorBrush(Colors.LightPink),
            new SolidColorBrush(MConv.IntToMediaColor(0xDD, 0xD3D3D3))
            /*
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFFC18C)), 
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFF5B79)), // WARAN
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFF5B79))  // ERROR
            */
        };
        public static SolidColorBrush[] gCBoxColors = new SolidColorBrush[]  {
            new SolidColorBrush(MConv.IntToMediaColor(0x44, 0xd3d3d3)),   // IDEL
            new SolidColorBrush(MConv.IntToMediaColor(0x44, 0xd3d3d3)),   // STOP
            new SolidColorBrush(MConv.IntToMediaColor(0x44, 0x98fb98)),   // PERV_RUN 
            new SolidColorBrush(MConv.IntToMediaColor(0x44, 0x00ff00)),   // RUN
            new SolidColorBrush(MConv.IntToMediaColor(0x44, 0x1E90FF)),   // Done
            new SolidColorBrush(MConv.IntToMediaColor(0x44, 0x7fffd4)),   // DEFOSET
            new SolidColorBrush(MConv.IntToMediaColor(0x44, 0xFF5B79)),   // Error

         };
        public static SolidColorBrush[] gDimStatColors = new SolidColorBrush[] {
            new SolidColorBrush(MConv.IntToMediaColor(0x99, 0xddddff)),   // STOP
            new SolidColorBrush(MConv.IntToMediaColor(0x99, 0x55aa55)),   // RUN
            new SolidColorBrush(MConv.IntToMediaColor(0x99, 0xffffff)),   // No Unit
            new SolidColorBrush(MConv.IntToMediaColor(0x99, 0x9E602D)),   // Warn
            new SolidColorBrush(MConv.IntToMediaColor(0x99, 0xAE802D)),   // Alram
            new SolidColorBrush(MConv.IntToMediaColor(0x99, 0x113311)),   // Error   
            new SolidColorBrush(MConv.IntToMediaColor(0x99, 0x1182c3)),
        };

        static public Rect[] gRackRects = new Rect[] {
            new Rect(20, 60, 60, 32),
            new Rect(88, 60, 60, 32),
            new Rect(20, 97, 60, 32),
            new Rect(80, 97, 60, 32),
            new Rect(20, 132, 60, 32),
            new Rect(80, 132, 60, 32)
        };
        static public Rect[] gRackMagins = new Rect[] {
            new Rect(23, 91, 92, 134),
            new Rect(90, 91, 25, 134),
            new Rect(23, 126, 92, 100),
            new Rect(90, 126, 25, 100),
            new Rect(23, 162, 92, 67),
            new Rect(90, 162, 25, 67),
        };
        static public Rect[] gRackRectsCol1 = new Rect[] {
            new Rect(54, 60,  60, 32),
            new Rect(54, 97,  60, 32),
            new Rect(54, 132, 60, 32),
        };

        static public Rect[] gRackMaginsCol1 = new Rect[] {
            new Rect(57, 91, 58, 134),
            new Rect(57, 126, 58, 100),
            new Rect(57, 162, 58, 67),
        };

        public static string msg_TestCompleted = "테스트 완료";
        public static string msg_TamsTestCompleted = "TAMS 테스트 완료";
        public static string msg_Unbalance = "온도동작 이상감지";
        public static string msg_Cbox_25_temp = "온도 25도이하";
        public static string msg_SlideOpen = "Slide Open";
        public static string msg_Alarm = "이상온도 발생";
        public static string msg_Humidity = "습도제상";
        public static string msg_TamsFile = "탐스파일에러";
        public static string msg_SenseError = "감압센서에러";
        public static string msg_Uncheck = "TC Unit UnCheck";
        public static string msg_bimetal = "바이메탈 감지";
        public static string msg_sOpen = "S.OPEN 감지";
        public static string msg_MotorUP = "Box 동작 중";
        public static string msg_MotorError = "모터 이상 동작";
        public static string msg_InitRack = "FIXTURE Initialize";
        public static string msg_StatusClear = "Status Clear 진행 중";

        public static string[] Motor_STAT = new string[2] { "CLOSE", "OPEN" };
        public static string[] FlowMeter_STAT = new string[3] { "정상 유량", "저유량 경고", "고유량 경고" };
        static Global()
        {
            loadConfig();
        }
        static public XmlDocument gConfigDoc = new XmlDocument();

        static public void loadConfig()
        {
            gConfigDoc = new XmlDocument();
            try
            {
                gConfigDoc.Load("RDimmRack.xml");
            }
            catch (Exception ex)
            {
                //Console.WriteLine(ex.Message);
                MessageBox.Show($"Can't Load RDimmRack.xml {ex.Message}");
                gConfigDoc = null;
                return;
            }
        }
        static public void saveConfig()
        {
            gConfigDoc.Save("RDimmRack.xml");
        }
    }
}
public static class VerInfo
{
    const string Program = "RDimm MMRACK";
    const string Date = "2026/01/30";
    const string Ver = "V1.2.3_Fixture Popup Clear Add";
    const string Author = "-";
    public static string getVer() => Program + "    " + Date + " " + Ver;
}

