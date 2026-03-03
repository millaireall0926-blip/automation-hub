using ScottPlot;
using Serilog;
using StarBase;
using StarBase.Net;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Xml;

using Logger = Serilog.Core.Logger;

namespace StarQMOS.ViewModel
{

    public enum STORY_MODE
    {
        NONE = 0,
        LOAD_DONE,
        TEST_DONE,

    }

    public enum INIT_MODE
    {
        NONE = 0,       
        DONE = 1
    }
    public class HandlerVM : BaseVM
    {
        public enum HANDLE_COL : int
        {
            DISCON = 0,
            OS,
            HANDLER,
            NOTYET,
            READY
            
        };

        public List<SolidColorBrush> _handlerColors = new List<SolidColorBrush>();

        SolidColorBrush _osColor;
        public SolidColorBrush OSColor
        {
            get => _osColor;
            set
            {
                _osColor = value;
                OnPropertyChanged(nameof(OSColor));
            }
        }

        SolidColorBrush _handlerColor;
        public SolidColorBrush HandlerColor
        {
            get => _handlerColor;
            set
            {
                _handlerColor = value;
                OnPropertyChanged(nameof(HandlerColor));
            }
        }

        SolidColorBrush _loadColor = new SolidColorBrush(Colors.Black);
        public SolidColorBrush LoadColor
        {
            get => _loadColor;
            set
            {
                _loadColor = value;
                OnPropertyChanged(nameof(LoadColor));
            }
        }

        SolidColorBrush _initColor = new SolidColorBrush(Colors.Black);
        public SolidColorBrush InitColor
        {
            get => _initColor;
            set
            {
                _initColor = value;
                OnPropertyChanged(nameof(InitColor));
            }
        }


        public int _totalCnt = 0;
        public int TotalCnt
        {
            get => _totalCnt;
            set { _totalCnt = value; OnPropertyChanged(nameof(TotalCnt)); }
        }

        public int _pssCnt = 0;
        public int PassCnt
        {
            get => _pssCnt;
            set { _pssCnt = value; OnPropertyChanged(nameof(PassCnt)); }
        }

        public int _failCnt = 0;
        public int FailCnt
        {
            get => _failCnt;
            set { _failCnt = value; OnPropertyChanged(nameof(FailCnt)); }
        }


        public SolidColorBrush getCol(HANDLE_COL type)
        {            
            return _handlerColors[(int)type];
        }

        public int DutMap = 0;

        public DelegateCommand DelCmdDevType { get; set; }

        public STORY_MODE StoryMode { get; set; } = STORY_MODE.NONE;

        public Logger BinSerilog { get; set; }

        public Logger PackSerilog { get; set; }

        public HandlerVM()
        {
            _handlerColors.Add(new SolidColorBrush(Colors.Gray));
            _handlerColors.Add(new SolidColorBrush(Colors.LightGreen));
            _handlerColors.Add(new SolidColorBrush(Colors.Orange));

            _handlerColors.Add(new SolidColorBrush(Colors.Black));
            _handlerColors.Add(new SolidColorBrush(Colors.DarkCyan));

            DelCmdDevType = new DelegateCommand(p => sendDevType());

            connected(false);
            loaded(false);
            Global.gHandlerVM = this;

            loadDevType();
            initBinLog();
        }

        public void addBinLog(string log)
        {
            BinSerilog.Information(log);
        }

        public void addPackLog(int type, string log)
        {
            if (type == 0)
                PackSerilog.Information("recv <- " + log);
            else if (type == 1)
                PackSerilog.Information("send -> " + log);
            else
                PackSerilog.Error("Err : " + log);
        }

        public void initBinLog()
        {
            BinSerilog = new LoggerConfiguration()
                .WriteTo.File(Global.gDirData + "\\" + "BIN_.csv", rollingInterval: RollingInterval.Day,
                outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();

            PackSerilog = new LoggerConfiguration()
                .WriteTo.File(Global.gDirData + "\\" + "Pack_.log", rollingInterval: RollingInterval.Day,
                outputTemplate: "{Timestamp:[yyyy-MM-dd HH:mm:ss]} [{Level:u3}]{Message:lj}{NewLine}")
                .CreateLogger();
        }

        public string DevTypeFolder = "";
        public Dictionary<int, string> devTypeDic = new Dictionary<int, string>();

        public void loadDevType()
        {
            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load("DevType.xml");
            }
            catch (Exception ex)
            {
                MessageBox.Show("DevType.xml Not Fount Can't Send DevType");
                return;
            }

            XmlNode root = doc.SelectSingleNode("/DevType");
            DevTypeFolder = root.Attributes["folder"].Value;

            XmlNodeList nodes = doc.SelectNodes("/DevType/item");
            foreach (XmlNode node in nodes )
            {
                int num = Convert.ToInt32(node.Attributes["num"].Value);
                string name = node.Attributes["name"].Value;
                devTypeDic.Add(num, name);
            }
        }

        public void sendDevType()
        {
            TotalCnt = 0;
            PassCnt = 0;
            FailCnt = 0;
            //byte[] binDatas = new byte[8];
            //for (int i = 0; i < 8; i++)
            //{
            //    binDatas[i] = 0x21;
            //    int bin = 0x21;
            //   // if (slot._dutDatas.Count > i)
            //        bin = 1;//Convert.ToInt32(1);
            //    binDatas[i] = (byte)bin;
            //}
            //Global.gNet._handler.send(0x10, binDatas);
            //
            //return;
            //
            //for (int i = 0; i< devTypeDic.Count; i++)
            //{
            //    string name = devTypeDic[i+1];
            //    byte[] bytes = MConv.StringToByte(name);
            //    byte[] datas = new byte[bytes.Length + 1];
            //    datas[0] = (byte)(i+1);
            //    Array.Copy(bytes, 0, datas, 1, bytes.Length);
            //    Global.gNet._handler.send((byte)HANLDER_TYPE.DEV_TYPE_CONFIG, datas);
            //    Thread.Sleep(50);
            //}
        }

        public void connected(bool bCon)
        {
            if (bCon)
            {
                OSColor = getCol(HANDLE_COL.OS);
                HandlerColor = getCol(HANDLE_COL.HANDLER);
                Global.gMainVM.HandlerText = "HANDLER INFO";
            }
            else
            {
                OSColor = getCol(HANDLE_COL.DISCON);
                HandlerColor = getCol(HANDLE_COL.DISCON);
                Global.gMainVM.HandlerText = "NO HANDLER";
            }
        }

        public void loaded(bool bLoad)
        {
            if (bLoad)            
                LoadColor = getCol(HANDLE_COL.READY);            
            else
                LoadColor = getCol(HANDLE_COL.NOTYET);
            TotalCnt = 0;
            PassCnt = 0;
            FailCnt = 0;
        }

        
        public INIT_MODE DpsInit = INIT_MODE.NONE;
        public void inited(bool bInit)
        {
            if (bInit)
            {
                InitColor = getCol(HANDLE_COL.READY);
                DpsInit = INIT_MODE.DONE;
            }
            else
            {
                InitColor = getCol(HANDLE_COL.NOTYET);
                DpsInit = INIT_MODE.NONE;
            }
            
        }
    }
}
