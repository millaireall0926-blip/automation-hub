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
using StarQMOS.Model;
using System.Windows;
using System.Windows.Threading;
using System.Windows.Controls;
using System.Windows.Media;

namespace StarQMOS.ViewModel
{
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

        public ListView ListSlotLog;
        public ListView ListSysLog;
        public WpfPlot SCOT_Yield;
        RadialGaugePlot SCOT_Radia_Yield;
        public double[] _YeildsDounut = { 0,0,0,0};
        public TestInfoVM()
        {
            Global.gTestVM = this;
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
