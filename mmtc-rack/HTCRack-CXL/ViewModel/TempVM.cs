using HTCRack_CXL.Base;
using HTCRack_CXL.Model;
using ScottPlot.Plottable;
using ScottPlot;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HTCRack_CXL.ViewModel
{
    public class TempVM : BaseVM
    {
        public bool AutoFit { get; set; } = true;

        int _curRack = 0;
        public int CurRack
        {
            get => _curRack;
            set { _curRack = value; CurName = $"Fix " + (_curRack + 1).ToString(); }
        }
        public string CurName { get; set; } = "Fix 1";
        public string EquipImg { get; set; } = "/Resource/equip.png";

        public int GraphUniformGridCnt { get; set; } = 0;

        public WpfPlot TempGraph;
        public SignalPlot[] TempSigs;
        public ObservableCollection<TempDataModel> TempDatas { get; set; } = new ObservableCollection<TempDataModel>();

        public TempVM()
        {
            Global.gTempVM = this;
            TempSigs = new SignalPlot[4]; // CXL DATA 4
        }
        public void InitGraph()
        {
            for (int i = 0; i < 4; i++)
            {
                TempSigs[i] = TempGraph.Plot.AddSignal(TempDatas[i].PVData,
                    color: MConv.toMediaColor(TempDatas[i].ChannelColor.Color),
                    label: String.Format($"CH{i + 1}"));
                TempSigs[i].LineWidth = 2;
                TempSigs[i].MarkerSize = 3;
                TempSigs[i].MaxRenderIndex = 0;
            }
            TempGraph.Refresh();
        }
        public void SetBox()
        {
            int selectRack = CurRack;

            RackModel rm = Global.gMainPageVM.Racks[selectRack];

            for (int i = 0; i < 4; i++)
            {
                TempSigs[i].MaxRenderIndex = rm.GraphIndex;
                // 그래프 색상 변경
                TempSigs[i].Color = MConv.toMediaColor(Global.GraphColors[i]);

                // GraphIndex에 맞는 데이터 복사
                if (rm.GraphIndex != 0)
                {
                    Array.Copy(rm.TempDatas[i].PVData, TempDatas[i].PVData, rm.GraphIndex);
                    Array.Copy(rm.TempDatas[i].TimeData, TempDatas[i].TimeData, rm.GraphIndex);
                }
            }
            // 그래프 새로 고침
            TempGraph.Refresh();

        }
        public void UpdateGraph(int gindex, RackModel rm)
        {
            try
            {
                int index = gindex - 1;
                if (index < 0) index = 0;

                for (int i = 0; i < 4; i++)
                {
                    TempDatas[i].TempPV = rm.TempDatas[i].TempPV;
                    TempDatas[i].PVData[index] = rm.TempDatas[i].TempPV;
                    TempDatas[i].TimeData[index] = rm.TempDatas[i].nowTime; // 실시간 시간 업데이트

                    TempSigs[i].IsVisible = TempDatas[i].Show;
                    // MaxRenderIndex가 GraphData 배열 범위를 초과하지 않도록 처리
                    //TempSigs[i].MaxRenderIndex = Math.Min(rm.GraphIndex - 1, TempDatas[i].PVData.Length - 1);
                }

                foreach (var sig in TempSigs)
                    sig.MaxRenderIndex = index;

                // 실시간으로 OffsetX를 현재 시간으로 업데이트 (실시간 그래프를 위해)
                //TempSigs.OffsetX = TempDatas[0].TimeData[0].ToOADate(); // 첫 번째 데이터의 시간값을 기준으로

                // 그래프 자동 축소/확장
                if (AutoFit)
                    TempGraph.Plot.AxisAuto();

                // 그래프 새로 고침
                TempGraph.Refresh();
            }
            catch (Exception ex) { }
        }
    }
}
