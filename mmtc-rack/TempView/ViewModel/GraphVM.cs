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
using ScottPlot.Renderable;
using ScottPlot.Ticks;

namespace TempView.ViewModel
{
    public class GraphVM : BaseVM
    {

        public WpfPlot DimGraph;

        SignalPlot[] DimSigs;
        SignalPlot[] EtcSigs;


        Collection<HLine> HLineGuids = new Collection<HLine>();
        Collection<VLine> VLineGuids = new Collection<VLine>();

        public ObservableCollection<DimDataModel> DimDatas { get; set; } = new ObservableCollection<DimDataModel>();

        public ObservableCollection<DimDataModel> EtcDatas { get; set; } = new ObservableCollection<DimDataModel>();

        public List<MarkerPlot> Markers = new List<MarkerPlot>();
        [DoNotNotify]
        public int Graphindex { get; set; } = 0;

        public GraphVM() {

            Global.gGraphVM = this;
        }

        public void initGraph()
        {
            DimSigs = new SignalPlot[Global.gMaxDimmCount];

            for (int i = 0; i < Global.gMaxDimmCount; i++)
            {
                DimSigs[i] = DimGraph.Plot.AddSignal(DimDatas[i].GraphData,
                    color: MConv.toMediaColor(DimDatas[i].ChanelColor.Color),
                    label: String.Format($"DM{i + 1}"),
                    sampleRate: 1);
                                    //sampleRate: 24*60*60); //sampleRate =   //daily: sampleRate = 1
                                                                //hourly: sampleRate = 24
                                                                //minute: sampleRate = 24 * 60
                                                                //second: samleRate = 24 * 60 * 60
                                                                //millisecond: sampleRate = 24 * 60 * 60 * 1000
                DimSigs[i].LineWidth = 2;
                DimSigs[i].MarkerSize = 3;
                DimSigs[i].MaxRenderIndex = 0;
                DimSigs[i].OffsetX = 0;
                //DimSigs[i].OffsetX = 2;
            }

            EtcSigs = new SignalPlot[Global.gSVList.Count + Global.gPVList.Count];

            int index = 0;

            for (int i = 0; i < Global.gSVList.Count; i++)
            {

                EtcSigs[index] = DimGraph.Plot.AddSignal(EtcDatas[index].GraphData,
                    color: MConv.toMediaColor(DimDatas[index].ChanelColor.Color),
                    label: String.Format("PV" + Global.gSVList[i]),
                    sampleRate: 1);
                    //sampleRate: 24 * 60 * 60);
                EtcSigs[index].LineWidth = 2;
                EtcSigs[index].MarkerSize = 3;
                EtcSigs[index].MaxRenderIndex = 0;
                EtcSigs[index].OffsetX = 0;
                //EtcSigs[index].OffsetX = 2;
                index++;
            }

            for (int i = 0; i < Global.gPVList.Count; i++)
            {
                EtcSigs[index] = DimGraph.Plot.AddSignal(EtcDatas[index].GraphData,
                    color: MConv.toMediaColor(DimDatas[index].ChanelColor.Color),
                    label: String.Format("SV" + Global.gPVList[i]),
                    sampleRate: 1);
                //sampleRate: 24 * 60 * 60);
                EtcSigs[index].LineWidth = 2;
                EtcSigs[index].MarkerSize = 3;
                EtcSigs[index].MaxRenderIndex = 0;
                EtcSigs[index].OffsetX = 0;
                //EtcSigs[index].OffsetX = 2;
                index++;
            }

            // X축을 누적 HH:MM:SS로
            DimGraph.Plot.XAxis.TickLabelFormat(x => {
                int ts = (int)Math.Round(x);
                int h = ts / 3600; int m = (ts % 3600) / 60; int s = ts % 60;
                return $"{h}:{m:D2}:{s:D2}";
            });
            //DimGraph.Plot.XAxis.TickLabelFormat("dD H:m:s", dateTimeFormat: true);
            DimGraph.Plot.AxisAuto();
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

        //public string[] LabelData { get; set; } = new string[160_000];

        public async Task setFile(string file)
{
    removeMakerAll();
    string name = Path.Combine(Global.gMainVM.DataPath, file + ".csv");
    bool isWater = name.Contains("_Water");
    Global.gSelectVM._prevWater = isWater;

    try
    {
        // (선택) 파일 로딩 중임을 알리는 UI를 활성화합니다.
        // IsLoading = true;

        // 1. Task.Run을 사용하여 파일 읽기 및 파싱을 백그라운드 스레드에서 수행합니다.
        var loadResult = await Task.Run(() =>
        {
            var markers = new List<(string name, double value, int index)>();
            int lineIndex = 0; // 현재 라인(데이터 포인트)의 인덱스

            try
            {
                using (var logFileStream = new FileStream(name, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
                using (var logFileReader = new StreamReader(logFileStream))
                {
                    // 2. 파일을 한 줄씩 스트리밍 방식으로 읽습니다. 전체를 메모리에 올리지 않습니다.
                    string line;
                    while ((line = logFileReader.ReadLine()) != null)
                    {
                        // 배열 크기를 초과하지 않도록 방어 코드를 추가합니다.
                        if (lineIndex >= DimDatas[0].GraphData.Length)
                        {
                            // 필요하다면 여기서 로그를 남길 수 있습니다.
                            // Console.WriteLine("Warning: GraphData buffer is full. Skipping remaining lines.");
                            break; 
                        }

                        string[] word = line.Split(',');

                        // --- 기존 파싱 로직은 거의 그대로 사용 ---
                        if (isWater)
                        {
                            if (word.Length < 4) continue;
                            int index = 0;
                            for (; index < 4; index++)
                                EtcDatas[index].GraphData[lineIndex] = Convert.ToDouble(word[1 + index]);
                            
                            EtcDatas[index++].GraphData[lineIndex] = 0;
                            EtcDatas[index++].GraphData[lineIndex] = 0;
                            for (int i = 0; i < Global.gMaxDimmCount; i++) 
                                DimDatas[i].GraphData[lineIndex] = 0;
                        }
                        else if (word.Length == 4) // marker
                        {
                            markers.Add((word[3], Convert.ToDouble(word[2]), Convert.ToInt32(word[1])));
                        }
                        else
                        {
                            int dimTotal = Global.gMaxDimmCount;
                            int dimStart = 1;
                            int svStart = dimTotal + dimStart;
                            if (word.Length != svStart + Global.gSVList.Count + Global.gPVList.Count) continue;

                            // 3. 파싱된 데이터를 최종 배열(GraphData)에 직접 할당합니다.
                            for (int i = 0; i < dimTotal; i++)
                                DimDatas[i].GraphData[lineIndex] = Convert.ToDouble(word[i + dimStart]);

                            int index = 0;
                            for (int i = 0; i < Global.gSVList.Count; i++)
                                EtcDatas[index++].GraphData[lineIndex] = Convert.ToDouble(word[i + svStart]);

                            int pvStart = svStart + Global.gSVList.Count;
                            for (int i = 0; i < Global.gPVList.Count; i++)
                                EtcDatas[index++].GraphData[lineIndex] = Convert.ToDouble(word[i + pvStart]);
                        }
                        // ------------------------------------

                        lineIndex++; // 다음 데이터 포인트를 위해 인덱스를 증가시킵니다.
                    }
                }
                return (GraphIndex: lineIndex, Markers: markers, Error: (string)null);
            }
            catch (Exception ex)
            {
                return (GraphIndex: 0, Markers: (List<(string, double, int)>)null, Error: ex.Message);
            }
        });

        // 4. 백그라운드 작업 완료 후, UI 스레드에서 그래프 데이터 업데이트
        if (loadResult.Error != null)
        {
            Growl.ClearGlobal();
            Growl.InfoGlobal(loadResult.Error);
            return;
        }

        Graphindex = loadResult.GraphIndex;

        // 마커 추가
        foreach (var markerInfo in loadResult.Markers)
        {
            AddMarker(markerInfo.Item1, markerInfo.Item2, markerInfo.Item3);
        }

        // RenderLock을 사용하여 여러 Plot 변경 후 한 번만 렌더링
        DimGraph.Plot.RenderLock();
        try
        {
            // SignalPlot의 데이터 범위 업데이트
            foreach (var sig in DimSigs) sig.MaxRenderIndex = Graphindex - 1;
            foreach (var sig in EtcSigs) sig.MaxRenderIndex = Graphindex - 1;

            // 각 시리즈의 가시성 설정
            for (int i = 0; i < Global.gMaxDimmCount; i++)
                DimSigs[i].IsVisible = DimDatas[i].Show;
            for (int i = 0; i < Global.gSVList.Count + Global.gPVList.Count; i++)
                EtcSigs[i].IsVisible = EtcDatas[i].Show;

            DimGraph.Plot.AxisAuto();
        }
        finally
        {
            DimGraph.Plot.RenderUnlock();
            DimGraph.Refresh();
        }
    }
    catch (Exception ex)
    {
        Growl.ClearGlobal();
        Growl.InfoGlobal($"An unexpected error occurred: {ex.Message}");
    }
    finally
    {
        // (선택) 로딩 UI를 비활성화합니다.
        // IsLoading = false;
    }
}

        /*
        public void setFile(string file)
        {
            removeMakerAll();
            Graphindex = 0;
            string name = Global.gMainVM.DataPath + "\\" + file + ".csv";
            bool isWater = false;
            if (name.IndexOf("_Water") > 0)                            
                isWater = true;
            
            Global.gSelectVM._prevWater = isWater;

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

            ///// Find Year From File
            ///   날짜 / 시간으로 출력용
            //  string year = "2024";
            //  string token = file.ToLower();
            //  int pos = token.IndexOf("rack");
            //  if (pos == -1)
            //  {
            //      pos = token.IndexOf("fix");
            //  }
            //  if (pos != -1) {
            //      int pos2 = token.IndexOf("_", pos); pos2++;
            //      int pos3 = token.IndexOf('_', pos2);
            //      pos3 = pos3 - pos2;
            //      if (pos3 > 0)                
            //          year = token.Substring(pos2, pos3);                
            //  }
            /////

            foreach (var line in lines)
            {
                string[] word = line.Split(new char[] { ',' });
                bool isFirstLine = true;

                if (isWater)
                {
                    if (word.Length < 4) continue;

                    int dimTotal = Global.gMaxDimmCount;
                    for (int i = 0; i < dimTotal; i++)
                        DimDatas[i].GraphData[Graphindex] = 0;

                    int waterStart = 1;
                    int waterSize = 4;
                    int index = 0;
                    for (; index < waterSize; index++)
                    {
                        double val = Convert.ToDouble(word[waterStart + index]);
                        EtcDatas[index].GraphData[Graphindex] = val;
                    }
                    EtcDatas[index++].GraphData[Graphindex] = 0;
                    EtcDatas[index++].GraphData[Graphindex] = 0;

                    Graphindex++;
                    continue;
                }


                if (word.Length == 4) // marker 
                {
                    int index = Convert.ToInt32(word[1]);
                    double value = Convert.ToDouble(word[2]);
                    string dim = word[3];
                    AddMarker(dim, value, index);
                }
                else
                {
                    int dimTotal = Global.gMaxDimmCount;
                    int dimStart = 1;
                    if (word.Length < dimTotal + dimStart) continue;

                    int index = 0;
                    int svStart = dimTotal + dimStart;
                    if (word.Length != svStart + (Global.gSVList.Count + Global.gPVList.Count)) continue;
                    
                    for (int i = 0; i < dimTotal; i++)
                        DimDatas[i].GraphData[Graphindex] = Convert.ToDouble(word[i + dimStart]);
                    
                    for (int i = 0; i < Global.gSVList.Count; i++)
                    {
                        double val = Convert.ToDouble(word[i + svStart]);
                        EtcDatas[index].GraphData[Graphindex] = val;
                        index++;
                    }

                    int pvStart = svStart + Global.gSVList.Count;
                    for (int i = 0; i < Global.gPVList.Count; i++)
                    {
                        double val = Convert.ToDouble(word[i + pvStart]);
                        EtcDatas[index].GraphData[Graphindex] = val;
                        index++;
                    }

                    Graphindex++;
                }

            }
            
            foreach (var sig in DimSigs)
            {
                sig.MaxRenderIndex = Graphindex;
            }

            foreach (var sig in EtcSigs)
            {
                sig.MaxRenderIndex = Graphindex;
            }


            // 시간날짜 출력용
            //DimGraph.Plot.XAxis.DateTimeFormat(true);
            
            DimGraph.Plot.AxisAuto();
     
            setShow();

        }
        */
        public void setShow()
        {
            for (int i = 0; i < Global.gMaxDimmCount; i++)
            
                DimSigs[i].IsVisible = DimDatas[i].Show;
            
            
            for (int i = 0; i < Global.gSVList.Count + Global.gPVList.Count; i++)
                EtcSigs[i].IsVisible = EtcDatas[i].Show;

            DimGraph.Refresh();
        }

    }
}
