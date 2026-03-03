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
        public SignalPlot TempSigs;
        public ObservableCollection<TempDataModel> TempDatas { get; set; } = new ObservableCollection<TempDataModel>();
        public GraphVM()
        {
            Global.gGraphVM = this;
            TempSigs = new SignalPlot(); // LPCAMM BOX DATA 1
        }
        public void InitGraph()
        {
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
