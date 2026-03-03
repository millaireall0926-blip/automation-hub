using Debug_TempCont.Common;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace Debug_TempCont.ViewModel
{
    public class GraphVM : BaseVM
    {
        public SolidColorBrush[] Brushs;

        private ObservableCollection<GraphCH> graphCHs;
        public ObservableCollection<GraphCH> GraphCHs
        {
            get => graphCHs;
            set
            {
                graphCHs = value;
                OnPropertyChanged(nameof(GraphCHs));
            }
        }
        bool _autoFit = true;
        public bool AutoFit
        {
            get => _autoFit;
            set
            {
                _autoFit = value;
                OnPropertyChanged("AutoFit");
            }
        }
        public GraphVM() 
        {
            Brushs = new SolidColorBrush[4];
            Brushs[0] = new SolidColorBrush(Colors.Blue);
            Brushs[1] = new SolidColorBrush(Colors.ForestGreen);
            Brushs[2] = new SolidColorBrush(Colors.Orchid);
            Brushs[3] = new SolidColorBrush(Colors.Tomato);

            GraphCHs = new ObservableCollection<GraphCH>
            {
                new GraphCH("CH1", 0, 0, Brushs[0] ,"",false),
                new GraphCH("CH2", 0, 1, Brushs[1] ,"",false),
                new GraphCH("CH3", 1, 0, Brushs[2] ,"",false),
                new GraphCH("CH4", 1, 1, Brushs[3] ,"",false)
            };
        }
    }
    public class GraphCH : BaseVM
    {
        public double[] _GraphDataTemp;
        public string Grpahch { get; set; }
        public int GrpahRow { get; set; }
        public int GrpahCol { get; set; }
        SolidColorBrush _colorT;
        public SolidColorBrush ColorT
        {
            get { return _colorT; }
            set
            {
                _colorT = value;
                OnPropertyChanged("ColorT");
            }
        }
        string _getT;
        public string GetT
        {
            get { return _getT; }
            set
            {
                _getT = value;
                OnPropertyChanged("GetV");
            }
        }
        bool _show = false;
        public bool Show
        {
            get { return _show; }
            set { _show = value; OnPropertyChanged("Show"); }
        }
        public void setData(double t, int index)
        {
            _GraphDataTemp[index] = t;
        }
        public GraphCH(string ch, int gRow, int gCol, SolidColorBrush tcolor, string tget, bool show)
        {
            Grpahch = ch;
            GrpahRow = gRow;
            GrpahCol = gCol;
            ColorT = tcolor;
            GetT = tget;
            Show = show;
            _GraphDataTemp = new double[1_000_000];
        }
    }
}
