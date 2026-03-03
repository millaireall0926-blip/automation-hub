using Debug_TempCont.ViewModel;
using ScottPlot;
using ScottPlot.Plottable;
using ScottPlot.Renderable;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace Debug_TempCont.View
{
    /// <summary>
    /// GraphView.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class GraphView : Page
    {
        public DispatcherTimer Graphtimer;
        public WpfPlot TempGraph;
        SignalPlot[] TempSigs;

        int Grpahcnt = 0;
        public bool UpdateFlag = false;
        int _tempGraphIndex = 0;

        GraphVM gvm = null;
        TempChVM tvm = null;
        public GraphView()
        {
            InitializeComponent();
            gvm = (GraphVM)App.Current.Services.GetService(typeof(GraphVM));
            tvm = (TempChVM)App.Current.Services.GetService(typeof(TempChVM));
            DataContext = App.Current.Services.GetService(typeof(GraphVM));

            TempGraph = xTempGraph;
            initItems();

            if (Graphtimer == null)
            {
                Graphtimer = new DispatcherTimer();
                Graphtimer.Interval = TimeSpan.FromSeconds(5);
                Graphtimer.Tick += Graphtimer_tick;
            }
            Graphtimer.Start();
            Global.gGraphView = this;
        }
        public void initItems()
        {
            //TempGraph.Plot.Title("Temp");
            TempGraph.Plot.XLabel("Time/Sec");
            TempGraph.Plot.YLabel("Temp/°C");

            TempGraph.Plot.Style(ScottPlot.Style.Gray2);
            TempSigs = new SignalPlot[4];

            for (int i = 0; i < 4; i++)
            {
                TempSigs[Grpahcnt] = TempGraph.Plot.AddSignal(gvm.GraphCHs[Grpahcnt]._GraphDataTemp);
                TempSigs[Grpahcnt].LineWidth = 3;
                TempSigs[Grpahcnt].MarkerSize = 0;
                TempSigs[Grpahcnt].MaxRenderIndex = 0;

                Color col = gvm.Brushs[Grpahcnt].Color;
                TempSigs[Grpahcnt].LineColor = System.Drawing.Color.FromArgb(col.A, col.R, col.G, col.B);
                Grpahcnt++;
            }

            Legend ld = TempGraph.Plot.Legend();
            ld.FillColor = System.Drawing.Color.DarkBlue;
            ld.FontColor = System.Drawing.Color.LightGray;
            ld.Location = Alignment.LowerRight;
            ld.IsVisible = true;

            TempGraph.Refresh();
        }
        public void Graphtimer_tick(object sender, EventArgs e)
        {
            //updateTemp();

            for (int i = 0; i < 4; i++)
            {

                if (UpdateFlag)
                    continue;
                if (tvm.TempDatas[i].PrevStat == STAT_TYPE.RUN)
                {
                    UpdateFlag = true;
                }
            }

            if(Global.gSerialComm.sPort.IsOpen) 
                updateTemp();
        }
        public void updateTemp()
        {
            int updatecnt = 0;
            for (int i = 0; i < 4; i++)
            {

                gvm.GraphCHs[updatecnt].GetT = tvm.TempDatas[i].TxtNowValue;

                TempSigs[updatecnt].IsVisible = gvm.GraphCHs[updatecnt].Show;

                gvm.GraphCHs[updatecnt].setData(Convert.ToDouble(tvm.TempDatas[i].TxtNowValue), _tempGraphIndex);

                if (UpdateFlag)
                {
                    //Global.gTemperatureLog.addTemperatureLog();
                    UpdateFlag = false;
                }
                updatecnt++;
            }
            foreach (var sig in TempSigs)
            {
                sig.MaxRenderIndex = _tempGraphIndex;
            }
            _tempGraphIndex++;
            if (gvm.AutoFit)
                TempGraph.Plot.AxisAuto();

            TempGraph.Refresh();
            if (_tempGraphIndex >= 1_000_000)
                _tempGraphIndex = 0;

        }
    }
}
