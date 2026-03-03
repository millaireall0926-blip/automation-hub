using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.Model;
using RDimm_MMTCRACK.View.Component;
using ScottPlot;
using ScottPlot.Plottable;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace RDimm_MMTCRACK.ViewModel
{
    public class GraphVM
    {
        public bool AutoFit {  get; set; } = true;

        public WpfPlot TempGraph;
        SignalPlot[] TempSigs;
        public ObservableCollection<TempDataModel> TempDatas { get; set; } = new ObservableCollection<TempDataModel>();
        public GraphVM() 
        {
            Global.gGraphVM = this;
            TempSigs = new SignalPlot[18]; // Temp Count 18
        }
        public void InitGraph()
        {
            for(int i = 0; i < 18; i++)
            {
                TempSigs[i] = TempGraph.Plot.AddSignal(TempDatas[i].GraphData,
                    color: MConv.toMediaColor(TempDatas[i].ChannelColor.Color),
                    label: String.Format($"CH{i + 1}"));
                TempSigs[i].LineWidth = 2;
                TempSigs[i].MarkerSize = 3;
                TempSigs[i].MaxRenderIndex = 0;
            }
            TempGraph.Refresh();
        }
        // Update Graph 
        /*
        public void UpdateGraph(int gindex)
        {
            int index = gindex - 1;
            if(index < 0 ) index = 0;

            for(int i = 0; i < 6; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    //TempDatas[(i * 3) + j].Temp = Global.gMainPageVM.Racks[i].Boxes[j].GetPV; // LP Capture Temp
                    //TempDatas[(i * 3) + j].GraphData[index] = Global.gMainPageVM.Racks[i].Boxes[j].GetPV; // LP Capture Temp
                    
                    TempDatas[(i * 3) + j].Temp = Global.gMainPageVM.Racks[i].Boxes[j].BoxPV; // LP Capture Temp
                    TempDatas[(i * 3) + j].GraphData[index] = Global.gMainPageVM.Racks[i].Boxes[j].BoxPV; // LP Capture Temp

                    TempSigs[(i * 3) + j].IsVisible = TempDatas[(i * 3) + j].Show;
                }
            }
            foreach(var sig in TempSigs) 
            {
                sig.MaxRenderIndex = index;
            }
            if (AutoFit)
                TempGraph.Plot.AxisAuto();

            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
            {
                TempGraph.Refresh();
            }));
        }
        */
    }
}
