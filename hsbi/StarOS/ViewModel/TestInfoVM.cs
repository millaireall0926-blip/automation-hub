using StarBase;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ScottPlot;
using ScottPlot.Plottable;
using ScottPlot.Renderable;
using System.Drawing;
using System.Collections.ObjectModel;
using StarOS.Model;
using System.Windows;
using System.Windows.Threading;
using System.Windows.Controls;
using System.Windows.Media;
using ScottPlot.Drawing.Colorsets;
using System.Runtime.InteropServices;
using Org.BouncyCastle.Utilities;
using StarOS.View;
using System.Reflection;

namespace StarOS.ViewModel
{
    [Serializable, StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Byte3
    {
        public byte byte1;
        public byte byte2;
        public byte byte3;

        public uint to_i()
        {
            return (uint)((byte3 << 16) | (byte2 << 8) | byte1);
        }

        public string to_s()
        {
            return $"{byte3.ToString("X2")} {byte2.ToString("X2")} {byte1.ToString("X2")}";
        }
    }

    [Serializable, StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct RegisterStruct
    {
        public Byte3 IDX0;
        public Byte3 IDX1;
        public Byte3 IDX2;
        public Byte3 IDX3;
        public Byte3 IDX4;
        public Byte3 IDX5;
        public Byte3 IDX6;
        public Byte3 IDX7;
        public Byte3 IDX8;
        public Byte3 IDX9;
        public Byte3 IDX10;
        public Byte3 IDX11;
        public Byte3 IDX12;
        public Byte3 IDX13;
        public Byte3 IDX14;
        public Byte3 IDX15;
        public Byte3 XMAX;
        public Byte3 YMAX;
        public byte Z_RESERVED; // no used
        public Byte3 LMAX;
        public Byte3 HMAX;
        public Byte3 XH;
        public Byte3 XT;
        public Byte3 YH;
        public Byte3 YT;
        public byte Z_RESERVED2; // no used
        public Byte3 D1A;
        public Byte3 D1B;
        public Byte3 D1C;
        public Byte3 D1D;
        public Byte3 D1E;
        public Byte3 D1F;
        public Byte3 D1G;
        public Byte3 D1H;
        public Byte3 D2A;
        public Byte3 D2B;
        public Byte3 D2C;
        public Byte3 D2D;
        public Byte3 D3B;
        public Byte3 D4B;
        public Byte3 TPH1;
        public Byte3 TPH1A;
        public Byte3 TPH1B;
        public Byte3 TPH1C;
        public Byte3 TPH1D;
        public Byte3 TPH2;
        public Byte3 TPH2A;
        public Byte3 TPH2B;
        public Byte3 TPH2C;
        public Byte3 TPH2D;
        public Byte3 D5;
        public Byte3 D6;
        public Byte3 D7;
        public Byte3 D8;
        public Byte3 XOS;
        public Byte3 YOS;
        public Byte3 DCMR;
        public Byte3 DCMR2;
        public ushort DBMAP;
        public Byte3 TIMER;
        public Byte3 PWAIT;
        public ushort STISP;
        public Byte3 REFTIMER;
        public ushort CFLG;
        public byte MOD;
        public Byte3 SCAN;
        public Byte3 VMPA;
        public ushort STATUS;
        public Byte3 XMASK;
        public Byte3 YMASK;
        public byte Z_RESERVED3; // no used
        public Byte3 CCD;
        public Byte3 RCD;
        public Byte3 DSD;
        public Byte3 DCMRH;
        public Byte3 DCMRL;
        public byte MODE; // no used
        public byte BL;
        public byte BT;
        public byte WL;
        public byte RL;
        public Byte3 XMIN;
        public Byte3 YMIN;
        public byte Z_RESERVED4; // no used
        public byte S1A;
        public byte MLOOP;
        public ushort ZMAX;
        public ushort ZH;
        public ushort ZMASK;
        public ushort ZMIN;
        public ushort RESERVED;
    }

    public class RegiserModel : BaseVM
    {        
        public string Name { get; set; }
        
        public string Value { get; set; }
        public SolidColorBrush Color { get; set; }
    }




    public class TestInfoVM : BaseVM
    {
        enum YIELD_TYPE : int
        {
            TOTAL = 0,
            CON = 1,
            FAIL = 2,
            PASS = 3,
        }

        #region UI Element
        string _pgm = "-";
        public string Pgm
        {
            get { return _pgm; }
            set { _pgm = value; OnPropertyChanged("Pgm"); }
        }

        string _lotID = "-";
        public string LotID
        {
            get { return _lotID; }
            set { _lotID = value; OnPropertyChanged("LotID"); }
        }

        string _step = "-";
        public string Step
        {
            get { return _step; }
            set { _step = value; OnPropertyChanged("Step"); }
        }

        string _method = "-";
        public string Method
        {
            get { return _method; }
            set { _method = value; OnPropertyChanged("Method"); }
        }

        string _start = "-";
        public string Start
        {
            get { return _start; }
            set { _start = value; OnPropertyChanged("Start"); }
        }

        string _end = "-";
        public string End
        {
            get { return _end; }
            set { _end = value; OnPropertyChanged("End"); }
        }

        string _failCnt = "-";
        public string FailCnt
        {
            get { return _failCnt; }
            set { _failCnt = value; OnPropertyChanged("FailCnt"); }
        }

        string _passCnt = "-";
        public string PassCnt
        {
            get { return _passCnt; }
            set { _passCnt = value; OnPropertyChanged("PassCnt"); }
        }

        string _mountCnt = "-";
        public string MountCnt
        {
            get { return _mountCnt; }
            set { _mountCnt = value; OnPropertyChanged("MountCnt"); }
        }

        string _totalCnt = "-";
        public string TotalCnt
        {
            get { return _totalCnt; }
            set { _totalCnt = value; OnPropertyChanged("TotalCnt"); }
        }


        int _scanProg = 0;
        public int ScanProg
        {
            get => _scanProg;
            set { _scanProg = value; OnPropertyChanged("ScanProg"); }
        }

        SolidColorBrush _scanBrush;
        public SolidColorBrush ScanBrush
        {
            get => _scanBrush;
            set { _scanBrush = value; OnPropertyChanged("ScanBrush"); }
        }


        public Visibility _showReqInfo = Visibility.Hidden;
        public Visibility ShowReqInfo
        {
            get => _showReqInfo;
            set { _showReqInfo = value; OnPropertyChanged("ReqInfo"); }
        }

        public string _reqMessage = "";
        public string ReqMessage
        {
            get => _reqMessage;
            set { _reqMessage = value; OnPropertyChanged("ReqMessage"); }
        }

        #endregion


        #region Register View
        string _selSlot;
        public string SelSlot
        {
            get => _selSlot;
            set
            {
                _selSlot = value; OnPropertyChanged("SelSlot");
            }
        }

        public DelegateCommand DGCMD_ShowRegView { get; set; }
        public DelegateCommand DGCMD_Toggle { get; set; }

        public RegisterView RegisterWin;

        public bool _showToggleSetOnly = false;
        public bool ShowToggleSetOnly
        {
            get => _showToggleSetOnly;
            set { _showToggleSetOnly = value; OnPropertyChanged("ShowToggleSetOnly"); }
        }

        public bool _showToggleHex = false;
        public bool ShowToggleHex
        {
            get => _showToggleHex;
            set { _showToggleHex = value; OnPropertyChanged("ShowToggleHex"); }
        }

        

        public void updateRegisterView()
        {
            RegModels.Clear();
            if (Global.gSlotVM.CurSlot == null) return;
            SlotModel slot = Global.gSlotVM.CurSlot;
            SelSlot = slot.Name;

            Type structType = typeof(RegisterStruct);

            // 필드 정보 가져오기
            FieldInfo[] fields = structType.GetFields();
            foreach (FieldInfo field in fields)
            {
                if (field.Name.Contains("RESERVED")) continue;
                bool isIndex = false;
                if (field.Name.Contains("IDX"))
                    isIndex = true;

                var fieldVal = field.GetValue(slot.RegSet);
                
                if (field.FieldType == typeof(Byte3))
                {
                    Byte3 tmp = (Byte3)fieldVal;
                    uint value = tmp.to_i();
                    if (value == 0 && ShowToggleSetOnly) continue;
                    if (isIndex && value != 0)
                        value--;


                    RegModels.Add(new RegiserModel() {
                        Name = field.Name,
                        Value = ShowToggleHex ? value.ToString("X") : value.ToString(),
                        Color = tmp.to_i() == 0 ? RegColors[0] : RegColors[1]
                    });
                    
                }
                else if (field.FieldType == typeof(ushort) ||
                    field.FieldType == typeof(byte))
                {
                    var tmp = field.FieldType == typeof(ushort) ?
                        (ushort)fieldVal : (byte)fieldVal;
                    if (tmp == 0 && ShowToggleSetOnly) continue;
                    RegModels.Add(new RegiserModel()
                    {
                        Name = field.Name,
                        Value = ShowToggleHex ? tmp.ToString("X") : tmp.ToString(),
                        Color = tmp == 0 ? RegColors[0] : RegColors[1]
                    });
                }
                
                
                
            }
           

            
        }

        public void clieckToggle()
        {
            Global.gMainVM.updateUIFlag((int)UI.REGISTER);
        }

        public void showRegisterView()
        {
            RegisterWin.Left = Global.gMainWin.Left + 500;
            RegisterWin.Top = Global.gMainWin.Top + 400;
            RegisterWin.Topmost = true;
            RegisterWin.Show();
            RegisterWin.Topmost = false;
        }

        public Collection<SolidColorBrush> RegColors = new Collection<SolidColorBrush>();

        ObservableCollection<RegiserModel> _regModels = new ObservableCollection<RegiserModel>();
        public ObservableCollection<RegiserModel> RegModels
        {
            get => _regModels;
            set { _regModels = value; OnPropertyChanged("RegModels"); }
        }   
        #endregion

        public ListView ListSlotLog;
        public ListView ListSysLog;
        public WpfPlot SCOT_Yield;
        RadialGaugePlot SCOT_Radia_Yield;
        public double[] _YeildsDounut = { 0,0,0,0};
        public TestInfoVM()
        {
            Global.gTestVM = this;
            RegColors.Add(new SolidColorBrush(Colors.DimGray));
            RegColors.Add(new SolidColorBrush(Colors.LightSeaGreen));
            RegisterWin = new RegisterView();
            DGCMD_ShowRegView = new DelegateCommand(_ => showRegisterView());
            DGCMD_Toggle = new DelegateCommand(_ => clieckToggle());
        }

        public void  initItems()
        {

            SCOT_Yield.Plot.Palette = ScottPlot.Palette.Nord;
            SCOT_Radia_Yield = SCOT_Yield.Plot.AddRadialGauge(_YeildsDounut);
            SCOT_Radia_Yield.Labels = new string[] { "FAIL", "PASS", "MOUNT", "TOTAL" };
            SCOT_Yield.Plot.Legend(true);
            SCOT_Radia_Yield.CircularBackground = false;
            SCOT_Radia_Yield.SpaceFraction = .05;
            SCOT_Radia_Yield.LabelPositionFraction = 0;
            SCOT_Yield.Plot.Style(ScottPlot.Style.Black);
            SCOT_Yield.Refresh();
        }

        public void addSysLog(LOG_TYPE type, string unit,  string msg)
        {
            SystemLog log = new SystemLog(type, unit, msg);
            
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                new Action(() =>
                {
                    SysLog.Add(log);
                    if (SysLog.Count > 100)
                        SysLog.RemoveAt(0);
                    ListSysLog.ScrollIntoView(log);
                    switch (type)
                    {
                        case LOG_TYPE.ERROR:
                            Global.gSystemLog.Error("[" + unit + "] " + msg);
                            break;
                        case LOG_TYPE.WARNING:
                            Global.gSystemLog.Warning("[" + unit + "] " + msg);
                            break;
                        case LOG_TYPE.INFO:
                            Global.gSystemLog.Information("[" + unit + "] " + msg);
                            break;
                        case LOG_TYPE.REQUEST_COMPLATE:
                        case LOG_TYPE.REQUEST:
                            Global.gSystemLog.Information("[" + unit + "] " + msg);
                            break;
                        

                    }        
                 }));
        }



        ObservableCollection<SystemLog> _sysLog = new ObservableCollection<SystemLog>();
        public ObservableCollection<SystemLog> SysLog
        {
            get { return _sysLog; }
            set { _sysLog = value; OnPropertyChanged("SysLog"); }
        }

        

        public void setSlotLog(SlotModel slot)
        {
            SlotLog.Clear();
            if (slot._logSlotQue.Count == 0)  return;
            foreach (SlotLog log in slot._logSlotQue)
            {
                SlotLog.Add(log);
            }
     
        }

        public void addSlotLog(SlotModel slot, string msg, LOG_TYPE type = LOG_TYPE.INFO)
        {
            SlotLog log = new SlotLog(slot.StepName, msg, type);
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                new Action(() => {
                    slot.addSlotLog(log);
                    if (Global.gMainVM.checkCurSlot(slot.Name)) {
              
                        SlotLog.Add(log);
                        if (SlotLog.Count > 100)
                            SlotLog.RemoveAt(0);
                        ListSlotLog.ScrollIntoView(log);
                    
                    }
                } ));
        }
                

        ObservableCollection<SlotLog> _slotLog = new ObservableCollection<SlotLog>();
        public ObservableCollection<SlotLog> SlotLog
        {
            get { return _slotLog; }
            set { _slotLog = value; OnPropertyChanged("SlotLog"); }
        }

        public void updateUI()
        {
            if (Global.gSlotVM.CurSlot == null) return;
            SlotModel slot = Global.gSlotVM.CurSlot;
            Pgm = slot.PgmName;
            LotID = slot.LotID;
            Step = slot.StepName;
            Method = slot.MeThod;
            Start = slot.StartTime;
            End = slot._endTime;

            ScanProg = slot._scanProc;
            //Console.WriteLine($"print scan {ScanProg}");
            if (slot.StepType == STEP_TYPE.AGING)
            {
                ScanBrush = Global.gStatDefs[(int)STAT_TYPE.ABORT].Brush;
            }
            else if (slot.StepType == STEP_TYPE.SOAK_TIME)
            {
                ScanBrush = Global.gStatDefs[(int)STAT_TYPE.LOAD].Brush;
            }
            else if (slot.StepType == STEP_TYPE.ALL_SCAN)
            {
                ScanBrush = Global.gStatDefs[(int)STAT_TYPE.DIAG].Brush;
            }
            else
            {
                if (slot._contact)
                    ScanBrush = Global.gStatDefs[(int)STAT_TYPE.CONTACTING].Brush;
                else
                    ScanBrush = Global.gStatDefs[(int)STAT_TYPE.RUNNING].Brush;
            }

            if (slot._steps.Count != 0) 
            {
                StepModel last = slot._steps.Last();

                string tmp = (last._testCnt - last._totalCnt).ToString();
                tmp = tmp.PadRight(5, ' ');
                tmp += string.Format("({0}%)", last._testCnt == 0 ? 0 : (last._testCnt - last._totalCnt) * 100 / last._testCnt);   // fail
                FailCnt = tmp;

                tmp = last._totalCnt.ToString();
                tmp = tmp.PadRight(5, ' ');
                tmp += string.Format("({0}%)", last._testCnt == 0 ? 0 : last._totalCnt * 100 / last._testCnt);
                PassCnt = tmp;

                MountCnt = last._testCnt.ToString();                   // mount 
                TotalCnt = slot._dutModels.Count.ToString();               // total

                _YeildsDounut[3] = slot._dutModels.Count;               // total
                _YeildsDounut[2] = last._testCnt;                   // mount 
                _YeildsDounut[1] = last._totalCnt;                  // pass
                _YeildsDounut[0] = last._testCnt - last._totalCnt;   // fail                 
            }else
            {
                TotalCnt = "-";
                MountCnt = "-";
                PassCnt = "-";
                FailCnt = "-";
                _YeildsDounut[3] = 0;
                _YeildsDounut[2] = 0;
                _YeildsDounut[1] = 0;
                _YeildsDounut[0] = 0;                
            }
            SCOT_Yield.Refresh();

        }   
    }
}
