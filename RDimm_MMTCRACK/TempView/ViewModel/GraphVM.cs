using ScottPlot.Plottable;
using ScottPlot;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TempView.Base;
using TempView.Model;
using HandyControl.Controls;
using System.IO;
using PropertyChanged;
using System.Reflection;
using System.Windows.Media;

namespace TempView.ViewModel
{
    public class GraphVM : BaseVM
    {

        public WpfPlot DimGraph;
        SignalPlot[] LPSigs;
        SignalPlot[] SVSigs;
        SignalPlot[] PVSigs;

        Collection<HLine> HLineGuids = new Collection<HLine>();
        Collection<VLine> VLineGuids = new Collection<VLine>();

        public ObservableCollection<DimDataModel> LPDatas { get; set; } = new ObservableCollection<DimDataModel>();

        public ObservableCollection<DimDataModel> SVDatas { get; set; } = new ObservableCollection<DimDataModel>();

        public ObservableCollection<DimDataModel> PVDatas { get; set; } = new ObservableCollection<DimDataModel>();

        public List<MarkerPlot> Markers = new List<MarkerPlot>();
        [DoNotNotify]
        public int Graphindex { get; set; } = 0;

        public GraphVM() {

            Global.gGraphVM = this;
        }

        public void initGraph()
        {
            LPSigs = new SignalPlot[Global.gMaxDimmCount];

            for (int i = 0; i < Global.gMaxDimmCount; i++)
            {
                LPSigs[i] = DimGraph.Plot.AddSignal(LPDatas[i].GraphData,
                    color: MConv.toMediaColor(LPDatas[i].ChanelColor.Color),
                    label: String.Format($"B{i + 1}"));
                LPSigs[i].LineWidth = 2;
                LPSigs[i].MarkerSize = 3;
                LPSigs[i].MaxRenderIndex = 0;

            }

            SVSigs = new SignalPlot[3];

            for (int i = 0; i < 3; i++)
            {

                SVSigs[i] = DimGraph.Plot.AddSignal(SVDatas[i].GraphData,
                    color: MConv.toMediaColor(LPDatas[i].ChanelColor.Color),
                    label: String.Format("PV" + i));
                SVSigs[i].LineWidth = 2;
                SVSigs[i].MarkerSize = 3;
                SVSigs[i].MaxRenderIndex = 0;
            }

            PVSigs = new SignalPlot[3];

            for (int i = 0; i < 3; i++)
            {

                PVSigs[i] = DimGraph.Plot.AddSignal(PVDatas[i].GraphData,
                    color: MConv.toMediaColor(LPDatas[i].ChanelColor.Color),
                    label: String.Format("PV" + i));
                PVSigs[i].LineWidth = 2;
                PVSigs[i].MarkerSize = 3;
                PVSigs[i].MaxRenderIndex = 0;
            }

            DimGraph.Refresh();
        }

        public void clearGuid()
        {
            while (VLineGuids.Count() > 0)
            {
                removeVLine();
            }
            while (HLineGuids.Count() > 0)
            {
                removeHLine();
            }
        }

        public void addVLine(double x)
        {
            var line = DimGraph.Plot.AddVerticalLine(0, color: System.Drawing.Color.Green, width: 1);
            line.X = x;
            line.DragEnabled = true;
            line.PositionLabel = true;
            line.LineStyle = LineStyle.Dash;
            VLineGuids.Add(line);
            DimGraph.Refresh();
        }

        public void removeVLine()
        {
            if (VLineGuids.Count > 0)
            {
                var line = VLineGuids[VLineGuids.Count - 1];
                DimGraph.Plot.Remove(line);
                VLineGuids.RemoveAt(VLineGuids.Count - 1);
            }
            DimGraph.Refresh();
        }

        public void addHLine(double y) 
        {
            if (y > 100) return;

            var line = DimGraph.Plot.AddHorizontalLine(0, color: System.Drawing.Color.Red, width: 1);
            line.Y = y;
            line.DragEnabled = true;
            line.PositionLabel = true;
            line.LineStyle = LineStyle.Dash;
            HLineGuids.Add(line);
            DimGraph.Refresh();
        }
        public void removeHLine()
        {
            if (HLineGuids.Count > 0)
            {
                var line = HLineGuids[HLineGuids.Count - 1];
                DimGraph.Plot.Remove(line);
                HLineGuids.RemoveAt(HLineGuids.Count - 1);
            }
            DimGraph.Refresh();
        }

        public void AddMarker(string name, double val, int index)
        {
            var maker = DimGraph.Plot.AddMarker(index, val);
            maker.Color = MConv.IntToColor(0xff, 0xff0000);
            maker.Text = name;
            Markers.Add(maker);
        }

        public void removeMakerAll()
        {
            if (Markers.Count == 0) return;
            foreach (var maker in Markers)
            {
                DimGraph.Plot.Remove(maker);
            }
            Markers.Clear();
        }

        public void setFile(string file)
        {
            removeMakerAll();
            Graphindex = 0;
            string name = Global.gMainVM.DataPath + "\\" + file + ".csv";
            Collection<string> lines = new Collection<string>();
            try
            {
                FileStream logFileStream = new FileStream(name, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
                StreamReader logFileReader = new StreamReader(logFileStream);
                while (!logFileReader.EndOfStream)
                {
                    lines.Add(logFileReader.ReadLine());
                }
                logFileReader.Close();
                logFileStream.Close();


            }
            catch (Exception ex)
            {
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return;
            }

            foreach (var line in lines)
            {
                string[] word = line.Split(new char[] { ',' });

                if (word.Length == 4) // marker 
                {
                    int index = Convert.ToInt32(word[1]);
                    double value = Convert.ToDouble(word[2]);
                    string dim = word[3];
                    AddMarker(dim, value, index);
                }
                else
                {
                    //word.Length = 17, 23
                    int dimTotal = Global.gMaxDimmCount;
                    int dimStart = 1;
                    if (word.Length < dimTotal + dimStart) continue;

                    for (int i = 0; i < dimTotal; i++)
                        LPDatas[i].GraphData[Graphindex] = Convert.ToDouble(word[i + dimStart]);

                    int svStart = dimTotal + dimStart;
                    if (word.Length < svStart + 2) continue;

                    for (int i = 0; i < 3; i++)
                    {
                        double val = Convert.ToDouble(word[i + svStart]);
                        SVDatas[i].GraphData[Graphindex] = val;
                    }

                    int pvStart = svStart + 3;
                    if (word.Length < pvStart + 2) continue;

                    for (int i = 0; i < 3; i++)
                    {
                        double val = Convert.ToDouble(word[i + pvStart]);
                        PVDatas[i].GraphData[Graphindex] = val;
                    }

                    Graphindex++;
                }

            }

            Console.WriteLine(Graphindex);

            foreach (var sig in LPSigs)
            {
                sig.MaxRenderIndex = Graphindex;
            }

            foreach (var sig in SVSigs)
            {
                sig.MaxRenderIndex = Graphindex;
            }

            foreach (var sig in PVSigs)
            {
                sig.MaxRenderIndex = Graphindex;
            }

            DimGraph.Plot.AxisAuto();
            setShow();

        }

        public void setShow()
        {
            for (int i = 0; i < Global.gMaxDimmCount; i++)
                LPSigs[i].IsVisible = LPDatas[i].Show;


            for (int i = 0; i < 3; i++)
            {
                SVSigs[i].IsVisible = SVDatas[i].Show;
                PVSigs[i].IsVisible = PVDatas[i].Show;
            }

            DimGraph.Refresh();
        }

    }
}
