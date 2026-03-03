using LP_MMTCRack.Base;
using LP_MMTCRack.Model;
using LP_MMTCRack.View.Component;
using ScottPlot;
using ScottPlot.Plottable;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Threading;

namespace LP_MMTCRack.ViewModel
{
    public class GraphVM : BaseVM
    {
        public bool AutoFit { get; set; } = true;

        int _curBox = 0;
        public int CurBox
        {
            get => _curBox;
            set { _curBox = value; CurName = $"Box " + (_curBox + 1).ToString(); }
        }
        public string CurName { get; set; } = "Box 1";

        public int GraphUniformGridCnt { get; set; } = 0;

        public WpfPlot TempGraph;

        private ScatterPlot TempScatterPlot;

        private double[] displayX_data = new double[0];
        private double[] displayY_data = new double[0];

        //public SignalPlot TempSigs;
        public ObservableCollection<TempDataModel> TempDatas { get; set; } = new ObservableCollection<TempDataModel>();
        public GraphVM()
        {
            Global.gGraphVM = this;
            //TempSigs = new SignalPlot(); // LPCAMM BOX DATA 1
        }
        public void InitGraph()
        {
            double[] xData;
            double[] yData;

            // TempDatas 컬렉션이나 그 안의 데이터가 비어있을 수 있으므로 방어 코드 추가
            if (TempDatas.Any() && TempDatas[0].TimeData != null && TempDatas[0].GraphData != null)
            {
                xData = TempDatas[0].TimeData.Select(dt => dt.ToOADate()).ToArray();
                yData = TempDatas[0].GraphData;
            }
            else
            {
                // 데이터가 없으면 빈 배열로 초기화
                xData = new double[0];
                yData = new double[0];
            }

            // AddScatter는 빈 배열을 받아도 에러를 내지 않습니다.
            // 빈 데이터로 ScatterPlot을 생성하고, X축을 시간 축으로 설정
            TempScatterPlot = TempGraph.Plot.AddScatter(xData, yData,
        color: MConv.toMediaColor(TempDatas.FirstOrDefault()?.ChannelColor.Color ?? System.Windows.Media.Colors.DodgerBlue),
        label: String.Format($"CH{0 + 1}"));

            TempScatterPlot.LineWidth = 2;
            TempScatterPlot.MarkerSize = 0; // 실시간에서는 마커를 끄는 것이 성능에 유리

            TempGraph.Plot.XAxis.DateTimeFormat(true);

            if (xData.Any())
            {
                TempGraph.Plot.AxisAuto();
            }
            TempGraph.Refresh();

            /*
            // 첫 번째 온도 데이터를 사용하여 SignalPlot을 추가
            TempSigs = TempGraph.Plot.AddSignal(TempDatas[0].GraphData, sampleRate: (24 * 60 * 60) / (Global.gConfigVM.CaptureCycle + 0.115),
                color: MConv.toMediaColor(TempDatas[0].ChannelColor.Color),
                label: String.Format($"CH{0 + 1}"));

            TempSigs.LineWidth = 2;
            TempSigs.MarkerSize = 3;
            TempSigs.MaxRenderIndex = 0;

            // 실시간 시간으로 X축의 시작 값 설정 (현재 시간을 기준으로)
            TempSigs.OffsetX = DateTime.Now.ToOADate();
            TempGraph.Plot.XAxis.DateTimeFormat(true); // X축을 시간 형식으로 설정
            TempGraph.Plot.AxisAuto();
            TempGraph.Refresh();
            */
            /*
            TempSigs = TempGraph.Plot.AddSignal(TempDatas[0].GraphData, sampleRate: (24 * 60 * 60) / (Global.gConfigVM.CaptureCycle + 0.115),
                color: MConv.toMediaColor(TempDatas[0].ChannelColor.Color),
                label: String.Format($"CH{0 + 1}"));

            TempSigs.LineWidth = 2;
            TempSigs.MarkerSize = 3;
            TempSigs.MaxRenderIndex = 0;

            TempSigs.OffsetX = DateTime.Now.ToOADate();
            TempGraph.Plot.XAxis.DateTimeFormat(true);
            TempGraph.Plot.AxisAuto();
            TempGraph.Refresh();
            */
        }
        public void SetBox()
        {
            // 다른 Box를 선택했을 때 해당 Box의 데이터를 그래프에 표시하는 로직
            int selectRack = CurBox / GraphUniformGridCnt;
            int selectBox = CurBox % GraphUniformGridCnt;

            RackModel rm = Global.gMainPageVM.Racks[selectRack];
            BoxModel bm = rm.Boxes[selectBox];

            TempScatterPlot.Color = MConv.toMediaColor(Global.GraphColors[bm.BoxIndex]);

            // 유효한 데이터만큼 잘라서 복사
            double[] xData = bm.TempDatas[0].TimeData.Take(bm.GraphIndex).Select(dt => dt.ToOADate()).ToArray();
            double[] yData = bm.TempDatas[0].GraphData.Take(bm.GraphIndex).ToArray();

            // ScatterPlot의 데이터를 새 데이터로 업데이트
            TempScatterPlot.Update(xData, yData);

            if (xData.Any())
            {
                // 데이터가 있으면, 플롯을 보이게 하고 축을 자동으로 맞춥니다.
                TempScatterPlot.IsVisible = true;
                if (AutoFit)
                {
                    TempGraph.Plot.AxisAuto();
                }
            }
            else
            {
                // 데이터가 없으면, Refresh()가 에러를 내지 않도록 플롯을 숨깁니다.
                TempScatterPlot.IsVisible = false;
            }

            TempGraph.Refresh();

            /*
            int selectRack = CurBox / GraphUniformGridCnt;
            int selectBox = CurBox % GraphUniformGridCnt;

            RackModel rm = Global.gMainPageVM.Racks[selectRack];
            BoxModel bm = rm.Boxes[selectBox];

            TempSigs.MaxRenderIndex = bm.GraphIndex;
            // 그래프 색상 변경
            TempSigs.Color = MConv.toMediaColor(Global.GraphColors[bm.BoxIndex]);

            // GraphIndex에 맞는 데이터 복사
            if (bm.GraphIndex != 0)
            {
                Array.Copy(bm.TempDatas[0].GraphData, TempDatas[0].GraphData, bm.GraphIndex);
                Array.Copy(bm.TempDatas[0].TimeData, TempDatas[0].TimeData, bm.GraphIndex);
            }
            else
            {
                // 첫 번째 데이터일 경우 초기화 처리
                if (TempSigs.OffsetX == 0)
                {
                    TempGraph.Plot.XAxis.DateTimeFormat(true);
                    TempDatas[0].TimeData[0] = DateTime.Now;
                    TempSigs.OffsetX = TempDatas[0].TimeData[0].ToOADate();
                }
                else
                {
                    TempGraph.Plot.XAxis.DateTimeFormat(true);
                    TempSigs.OffsetX = TempDatas[0].TimeData[0].ToOADate();
                }
            }

            // 그래프 새로 고침
            TempGraph.Refresh();
            */
            /*
            int selectRack = CurBox / GraphUniformGridCnt;
            int selectBox = CurBox % GraphUniformGridCnt;

            //Console.WriteLine($" {selectRack} - {selectBox}");
            RackModel rm = Global.gMainPageVM.Racks[selectRack];
            BoxModel bm = rm.Boxes[selectBox];

            TempSigs.MaxRenderIndex = bm.GraphIndex;
            TempSigs.Color = MConv.toMediaColor(Global.GraphColors[bm.BoxIndex]);//MConv.toMediaColor(TempDatas[rm.RackIndex].ChannelColor.Color);

            if (bm.GraphIndex != 0) 
            {
                Array.Copy(bm.TempDatas[0].GraphData, TempDatas[0].GraphData, bm.GraphIndex);
                Array.Copy(bm.TempDatas[0].TimeData, TempDatas[0].TimeData, bm.GraphIndex);
            }
            else
            {
                if (TempSigs.OffsetX == 0)
                {
                    TempGraph.Plot.XAxis.DateTimeFormat(true);
                    TempDatas[0].TimeData[0] = DateTime.Now;
                    TempSigs.OffsetX = TempDatas[0].TimeData[0].ToOADate();
                }
                else
                {
                    TempGraph.Plot.XAxis.DateTimeFormat(true);
                    TempSigs.OffsetX = TempDatas[0].TimeData[0].ToOADate();
                }
                //Console.WriteLine($"SetBox() else: {Global.gGraphVM.TempSigs.OffsetX} , {TempDatas[0].TimeData[0].ToString()}");
            }

            

            //TempGraph.Plot.AxisAuto();

            TempGraph.Refresh();
            */
            /*
            if (rack.GraphIndex != 0)
            {
                for (int i = 0; i < Global.gMainPageVM.MaxDimmCount; i++)
                {
                    Array.Copy(rack.DimDatas[i].GraphData, DimDatas[i].GraphData, rack.GraphIndex);
                }
            }
            */
        }
        // Update Graph 
        public void UpdateGraph(int gindex, BoxModel bm)
        {
            // 현재 선택된 Box와 업데이트 대상 Box가 같을 때만 그래프를 갱신합니다.
            if (CurBox != bm.BoxIndex)
            {
                return;
            }

            // 유효한 데이터만 잘라내어 X, Y 배열 생성
            double[] xData = bm.TempDatas[0].TimeData.Take(gindex).Select(dt => dt.ToOADate()).ToArray();
            double[] yData = bm.TempDatas[0].GraphData.Take(gindex).ToArray();

            // ScatterPlot 데이터를 업데이트
            TempScatterPlot.Update(xData, yData);

            // 실시간으로 데이터가 들어왔으므로, 그래프가 반드시 보이도록 설정합니다.
            if (xData.Any())
            {
                TempScatterPlot.IsVisible = true;
                if (AutoFit)
                {
                    // 데이터가 있으니 축 자동 맞춤도 안전하게 실행됩니다.
                    TempGraph.Plot.AxisAuto();
                }
            }
            else
            {
                // (이 경우는 거의 없겠지만) 만약 빈 데이터로 업데이트 요청이 오면 숨깁니다.
                TempScatterPlot.IsVisible = false;
            }

            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
            {
                TempGraph.Refresh();
            }));
            /*
            int Getrm = bm.MasterRack.RackIndex;
            int Getbm = bm.EachBoxIndex;

            int index = gindex - 1;
            if (index < 0) index = 0;

            // 온도 데이터 갱신
            TempDatas[0].Temp = bm.TempDatas[0].Temp;
            TempDatas[0].GraphData[index] = bm.TempDatas[0].Temp;
            TempDatas[0].TimeData[index] = bm.TempDatas[0].nowTime; // 실시간 시간 업데이트

            // MaxRenderIndex가 GraphData 배열 범위를 초과하지 않도록 처리
            TempSigs.MaxRenderIndex = Math.Min(bm.GraphIndex - 1, TempDatas[0].GraphData.Length - 1);

            // 실시간으로 OffsetX를 현재 시간으로 업데이트 (실시간 그래프를 위해)
            TempSigs.OffsetX = TempDatas[0].TimeData[0].ToOADate(); // 첫 번째 데이터의 시간값을 기준으로

            // 그래프 자동 축소/확장
            if (AutoFit)
                TempGraph.Plot.AxisAuto();

            // 그래프 새로 고침
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
            {
                TempGraph.Refresh();
            }));
            */
            /*
            int index = gindex - 1;
            if(index < 0 ) index = 0;
            //TempSigs.OffsetX = bm.TempDatas[0].TimeData[0].ToOADate();

            TempDatas[0].Temp = bm.TempDatas[0].Temp;
            TempDatas[0].GraphData[index] = bm.TempDatas[0].Temp;

            TempDatas[0].TimeData[index] = bm.TempDatas[0].nowTime;

            

            //double[] timeDataAsOADate = TempDatas[0].TimeData.Select(x => x.ToOADate()).ToArray();
            //TempGraph.Plot.AddScatter(timeDataAsOADate, TempDatas[0].GraphData, color: MConv.toMediaColor(TempDatas[0].ChannelColor.Color));

            TempSigs.MaxRenderIndex = bm.GraphIndex - 1;

            if (AutoFit)
                TempGraph.Plot.AxisAuto();

            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
            {
                TempGraph.Refresh();
            }));
            */
        }

    }
}
