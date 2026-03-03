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
        SignalPlot LPSigs;
        SignalPlot SVSigs;
        SignalPlot PVSigs;

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
            LPSigs = new SignalPlot();

            LPSigs = DimGraph.Plot.AddSignal(LPDatas[0].GraphData, sampleRate:(24 * 60 * 60) / (Global.CaptureCycle + 0.115),
                    color: MConv.toMediaColor(LPDatas[0].ChanelColor.Color),
                    label: String.Format($"B{0 + 1}"));
            
            LPSigs.LineWidth = 2;
            LPSigs.MarkerSize = 3;
            LPSigs.MaxRenderIndex = 0;

            SVSigs = new SignalPlot();

            SVSigs = DimGraph.Plot.AddSignal(SVDatas[0].GraphData, sampleRate: (24 * 60 * 60) / (Global.CaptureCycle + 0.115),
                    color: MConv.toMediaColor(SVDatas[0].ChanelColor.Color),
                    label: String.Format("PV" + 1));
            SVSigs.LineWidth = 2;
            SVSigs.MarkerSize = 3;
            SVSigs.MaxRenderIndex = 0;

            PVSigs = new SignalPlot();

            PVSigs = DimGraph.Plot.AddSignal(PVDatas[0].GraphData, sampleRate: (24 * 60 * 60) / (Global.CaptureCycle + 0.115),
                    color: MConv.toMediaColor(PVDatas[0].ChanelColor.Color),
                    label: String.Format("PV" + 1));
            PVSigs.LineWidth = 2;
            PVSigs.MarkerSize = 3;
            PVSigs.MaxRenderIndex = 0;

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

                if (word.Length > 3)
                {
                    //word.Length = 4
                    DateTime timeValue;

                    bool parsedSuccessfully = false;

                    // 1. 먼저 새로운 형식(yyyy-MM-dd...)을 시도합니다.
                    if (DateTime.TryParseExact(word[0], "yyyy-MM-dd HH:mm:ss", null, System.Globalization.DateTimeStyles.None, out timeValue))
                    {
                        parsedSuccessfully = true;
                    }
                    // 2. 새로운 형식이 실패하면, 이전 형식(MM-dd...)을 시도합니다.
                    else if (DateTime.TryParseExact(word[0], "MM-dd HH:mm:ss", null, System.Globalization.DateTimeStyles.None, out timeValue))
                    {
                        parsedSuccessfully = true;
                    }

                    if (parsedSuccessfully)
                    {
                        LPDatas[0].TimeData[Graphindex] = timeValue;
                        LPDatas[0].GraphData[Graphindex] = Convert.ToDouble(word[1]);
                        SVDatas[0].GraphData[Graphindex] = Convert.ToDouble(word[2]);
                        PVDatas[0].GraphData[Graphindex] = Convert.ToDouble(word[3]);

                        Graphindex++;
                    }
                }
            }

            Console.WriteLine(Graphindex);

            if(Graphindex == 0)
            {
                // Plot.Clear()를 사용하지 않습니다.
                // 대신, 각 SignalPlot이 데이터를 렌더링하지 않도록 설정합니다.
                LPSigs.MaxRenderIndex = 0;
                SVSigs.MaxRenderIndex = 0;
                PVSigs.MaxRenderIndex = 0;

                // 이전에 그려진 마커가 있다면 모두 제거합니다.
                removeMakerAll();

                DimGraph.Plot.AxisAuto(); // 축 범위를 자동으로 다시 계산
                DimGraph.Render();
                Growl.InfoGlobal("선택한 파일에 유효한 데이터가 없습니다.");
                return;
            }
            
            LPSigs.MaxRenderIndex = Graphindex - 1;
            SVSigs.MaxRenderIndex = Graphindex - 1;
            PVSigs.MaxRenderIndex = Graphindex - 1;

            LPSigs.OffsetX = LPDatas[0].TimeData[0].ToOADate();
            SVSigs.OffsetX = LPDatas[0].TimeData[0].ToOADate();
            PVSigs.OffsetX = LPDatas[0].TimeData[0].ToOADate();
            DimGraph.Plot.XAxis.DateTimeFormat(true);
            DimGraph.Plot.AxisAuto();
            setShow();

        }

        public void setShow()
        {
            LPSigs.IsVisible = LPDatas[0].Show;
            SVSigs.IsVisible = SVDatas[0].Show;
            PVSigs.IsVisible = PVDatas[0].Show;

            DimGraph.Refresh();
        }

    }
}
