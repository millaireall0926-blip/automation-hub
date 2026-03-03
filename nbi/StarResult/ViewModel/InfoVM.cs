using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ScottPlot;
using ScottPlot.Plottable;
using StarBase;

namespace StarResult.ViewModel
{
    public class InfoVM : BaseVM
    {

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

        string _BID = "-";
        public string BID
        {
            get { return _BID; }
            set { _BID = value; OnPropertyChanged("BID"); }
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
        #endregion
        public WpfPlot SCOT_Yield;
        RadialGaugePlot SCOT_Radia_Yield;
        public double[] _YeildsDounut = { 0, 0, 0, 0 };

        public InfoVM()
        {
            Global.gInfoVM = this;
        }

        public void initItems()
        {

            SCOT_Yield.Plot.Palette = ScottPlot.Palette.Nord;
            SCOT_Radia_Yield = SCOT_Yield.Plot.AddRadialGauge(_YeildsDounut);
            SCOT_Radia_Yield.Labels = new string[] { "FAIL", "PASS", "MOUNT", "TOTAL" };
            SCOT_Yield.Plot.Legend(true);
            SCOT_Radia_Yield.CircularBackground = false;
            SCOT_Radia_Yield.SpaceFraction = .05;
            SCOT_Radia_Yield.LabelPositionFraction = 0;
            SCOT_Yield.Plot.Style(ScottPlot.Style.Hazel);
            SCOT_Yield.Refresh();
        }

        public void updateYeild()
        {
            _YeildsDounut[3] = Convert.ToInt16(TotalCnt);
            _YeildsDounut[2] = Convert.ToInt16(MountCnt);
            _YeildsDounut[1] = Convert.ToInt16(PassCnt);
            _YeildsDounut[0] = Convert.ToInt16(FailCnt);
            SCOT_Yield.Refresh();
        }
    }
}
