using HTCRack_CXL.Base;
using HTCRack_CXL.Model;
using HTCRack_CXL.Net;
using HTCRack_CXL.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Xml;

namespace HTCRack_CXL
{
    public class Global
    {
        static public MainVM gMainVM;
        static public MainPageVM gMainPageVM;
        static public EngineerVM gEngineerVM;
        static public ConfigVM gConfigVM;
        static public BoardVM gBoardVM;
        static public TempVM gTempVM;

        public static SolidColorBrush[] gBoarderColors = new SolidColorBrush[]
        {
            // RACK_STAT Index
            new SolidColorBrush(Colors.AntiqueWhite),    // STOP            
            new SolidColorBrush(Colors.AntiqueWhite),      // PREV_STOP
            new SolidColorBrush(Colors.PaleGreen),      // PERV_RUN
            new SolidColorBrush(Colors.SpringGreen),      // RUN
            new SolidColorBrush(Colors.DeepSkyBlue),    // Done
            new SolidColorBrush(Colors.Orange),  // TUNING
            //new SolidColorBrush(Colors.Plum),       // ERROR
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFF5B79)), // ERROR
            new SolidColorBrush(Colors.LightGray),  // NOCON
            new SolidColorBrush(Colors.Red),  // INSPECTION -> as mode
            new SolidColorBrush(Colors.Aquamarine),        //DEFOSET 
        };

        public static SolidColorBrush[] gMotorStatColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.MediumPurple),        // Motor Close
            new SolidColorBrush(Colors.Plum),        // Motor Closing
            new SolidColorBrush(Colors.Gold),      // Motor Open
            new SolidColorBrush(Colors.Yellow),      // Motor Opening
            new SolidColorBrush(Colors.LightGray),   // Motor NID
        };

        public static SolidColorBrush[] gWarningColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.Black),
            new SolidColorBrush(Colors.Red)
        };

        public static SolidColorBrush[] gRackColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.Transparent),    // Normal
            new SolidColorBrush(Colors.IndianRed),            // as mode            
        };

        public static SolidColorBrush[] gFPGAProgColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.LightGray),    // STOP            
            new SolidColorBrush(Colors.Green),    // Erase
            new SolidColorBrush(Colors.Blue),    // Program
            new SolidColorBrush(Colors.Red),      // Alarm
            new SolidColorBrush(Colors.Red)      // Error
        };
        public static SolidColorBrush[] gAlramColors = new SolidColorBrush[]
        {
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFFC18C)), 
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFF5B79)), // WARAN
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFF5B79))  // ERROR
        };
        public static SolidColorBrush[] gBoxAlramColors = new SolidColorBrush[]
        {
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0x6D79FF)), // Done
            new SolidColorBrush(Colors.LightPink),
            new SolidColorBrush(Colors.LightPink),
            new SolidColorBrush(MConv.IntToMediaColor(0xDD, 0xD3D3D3)),
            new SolidColorBrush(Colors.LightPink),
            /*
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFFC18C)), 
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFF5B79)), // WARAN
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFF5B79))  // ERROR
            */
        };
        public static Color[] GraphColors = new Color[] {
                Colors.Coral,
                Colors.DarkKhaki,
                Colors.Violet,
                Colors.DarkSlateBlue,
        };
        
        #region Equip Selection Rect
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
        #endregion

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
                gConfigDoc.Load("CxlRack.xml");
            }
            catch (Exception ex)
            {
                //Console.WriteLine(ex.Message);
                MessageBox.Show($"Can't Load CxlRack.xml {ex.Message}");
                gConfigDoc = null;
                return;
            }
        }

        static public void saveConfig()
        {
            gConfigDoc.Save("CxlRack.xml");
        }

        static public HtcMessage newMsg(int ch, int adr, int func, int Len = 0) // if Len is Not 0, It is Request Byte Count
        {
            HtcMessage msg = new HtcMessage();
            msg.sub = new HtcSub();
            msg.sub.Chanel = (byte)ch;
            msg.sub.Adr = (byte)adr;
            msg.sub.Func = (byte)func;
            msg.sub.Len = (short)Len;

            return msg;
        }

        static public HtcMessage newMsg(byte ch, byte adr, byte func, byte Len = 0)
        {
            HtcMessage msg = new HtcMessage();
            msg.sub = new HtcSub();
            msg.sub.Chanel = ch;
            msg.sub.Adr = adr;
            msg.sub.Func = func;
            msg.sub.Len = (short)Len;

            return msg;
        }
    }
    public static class LogHelper
    {
        public static void Info(string source, string msg)
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{source}] : {msg}");
        }

        public static void Error(string source, string msg)
        {
            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[{source}] : {msg}");
        }

        public static void Warn(string source, string msg)
        {
            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[{source}] : {msg}");
        }

        public static void Alarm(ALRAM_TYPE type, string name, string msg, bool isLogging = true)
        {
            Global.gMainPageVM.addLog((LOG_TYPE)type, $"[Alarm][{name}] : {msg}");
        }
    }
    public static class VerInfo
    {

        const string Program = "HTCRack-CXL";
        const string Date = "2025/06/18";
        const string Ver = "V1.0.0";
        const string Author = "uiop";
        public static string getVer() => Program + "    " + Date + " " + Ver;

        /*
         * 
         * const string Ver = "V1.0.0";
           CXL 용 작업 시작
         * 
         */
    }
}
