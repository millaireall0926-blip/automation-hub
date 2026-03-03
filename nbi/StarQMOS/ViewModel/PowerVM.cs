using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using StarBase;
using StarQMOS.Model;
using ScottPlot;
using ScottPlot.Plottable;
using ScottPlot.Renderable;

namespace StarQMOS.ViewModel
{
    public enum PE_TYPE : int
    { 
        VIH = 0,
        VIL = 1,
        VTH = 2
    }

    public class PowerVM : BaseVM
    {
        public DPSModel[] DPSs;
        public PEModel[] PEs;
        public SolidColorBrush[] Brushs;

        public WpfPlot PowerGraph;
        SignalPlot[] PowerSigs;

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

        public PowerVM()
        {
            Global.gPowerVM = this;            
            DPSs = new DPSModel[5];
            PEs = new PEModel[3];
            Brushs = new SolidColorBrush[10];

            // DPS1
            Brushs[0] = new SolidColorBrush(Colors.Blue);
            Brushs[1] = new SolidColorBrush(Colors.DarkBlue);
            Brushs[2] = new SolidColorBrush(Colors.Orchid);
            Brushs[3] = new SolidColorBrush(Colors.DarkOrchid);
            Brushs[4] = new SolidColorBrush(Colors.Green);
            Brushs[5] = new SolidColorBrush(Colors.DarkGreen);
            Brushs[6] = new SolidColorBrush(Colors.IndianRed);
            Brushs[7] = new SolidColorBrush(Colors.DarkRed);
            Brushs[8] = new SolidColorBrush(Colors.Orange);
            Brushs[9] = new SolidColorBrush(Colors.DarkOrange);
            
        }

        public void initItems()
        {
            for (int i = 0; i < 5; i++)
            {
                DPSs[i].ColorV = Brushs[i * 2];
                DPSs[i].ColorC = Brushs[(i * 2) + 1];
                DPSs[i].GetV = string.Format("{0,5}", "-  V");
                DPSs[i].GetC = string.Format("{0,5}", "-  A");
                DPSs[i].SetV = string.Format("{0,5}", "-  V");
                DPSs[i].OC = string.Format("{0,5}", "-  A"); ;
            }
            DPSs[0].Show = true;
            DPSs[1].Show = true;


            for (int i = 0; i < 3; i++)
            {
                PEs[i].IOV = string.Format("{0,5}", "-  V");
                PEs[i].CLKV = string.Format("{0,5}", "-  V");
                PEs[i].DRV = string.Format("{0,5}", "-  V");
                PEs[i].ScanV = string.Format("{0,5}", "-  V");
            }

            PowerGraph.Plot.Title("Power");
            PowerGraph.Plot.YLabel("Level/Current");

            PowerGraph.Plot.Style(ScottPlot.Style.Gray2);
            PowerSigs = new SignalPlot[10];

            for (int i = 0; i < 5; i++)
            {
                PowerSigs[i * 2]        = PowerGraph.Plot.AddSignal(DPSs[i]._graphDataV, label: String.Format($"DPS{i}V"));
                PowerSigs[i * 2].LineWidth  = 3;
                PowerSigs[i * 2].MarkerSize = 0;
                PowerSigs[i * 2].MaxRenderIndex = 0;

                Color col = Brushs[i * 2].Color;
                PowerSigs[i * 2].Color = System.Drawing.Color.FromArgb(
                    col.A, col.R, col.G, col.B);
                    
                PowerSigs[(i * 2) + 1]  = PowerGraph.Plot.AddSignal(DPSs[i]._graphDataC, label: String.Format($"DPS{i}C"));
                PowerSigs[(i * 2) + 1].LineWidth = 2;
                PowerSigs[(i * 2) + 1].MarkerSize = 3;
                PowerSigs[(i * 2) + 1].MaxRenderIndex = 0;
                col = Brushs[i * 2 + 1].Color;
                PowerSigs[(i * 2) + 1].Color = System.Drawing.Color.FromArgb(
                    col.A, col.R, col.G, col.B);
            }

            Legend ld = PowerGraph.Plot.Legend();
            ld.FillColor = System.Drawing.Color.DarkBlue;
            ld.FontColor = System.Drawing.Color.LightGray;
            ld.Location = Alignment.LowerRight;
            ld.IsVisible = false;            

            PowerGraph.Refresh();
        }   

        public void setSlot(SlotModel slot)
        {
            foreach(var sig in PowerSigs)
            {
                sig.MaxRenderIndex = slot._pwrGraphIndex;
            }

            if (slot._pwrGraphIndex != 0)
            {
                for (int i = 0; i < 5; i++)
                {
                    Array.Copy(slot._DPSs[i]._graphDataV, DPSs[i]._graphDataV, slot._pwrGraphIndex);
                    Array.Copy(slot._DPSs[i]._graphDataC, DPSs[i]._graphDataC, slot._pwrGraphIndex);
                }
            }
            PowerGraph.Refresh();
        }



        public void updatePower(SlotModel slot)
        {
            bool isData = true;
            if (slot._DPSs[0].SetV == string.Format("{0,5}", "-  V"))
                isData = false;

            int index = slot._pwrGraphIndex - 1;
            if (index < 0) index = 0;

            for (int i = 0;i < 5;i++)
            {
                DPSs[i].SetV = slot._DPSs[i].SetV;
                DPSs[i].GetV = slot._DPSs[i].GetV;
                DPSs[i].GetC = slot._DPSs[i].GetC;
                DPSs[i].OC = slot._DPSs[i].OC;

                PowerSigs[i * 2].IsVisible = DPSs[i].Show;
                PowerSigs[(i * 2) + 1].IsVisible = DPSs[i].Show;
                
                double v, c;
                double.TryParse(DPSs[i].GetV, out v);
                double.TryParse(DPSs[i].GetC, out c);

                if (isData) DPSs[i].setData(v, c, index);
            }

            for (int i = 0; i < 3; i++)
            {
                PEs[i].IOV = slot._PEs[i].IOV;
                PEs[i].CLKV = slot._PEs[i].CLKV;
                PEs[i].DRV = slot._PEs[i].DRV;
                PEs[i].ScanV = slot._PEs[i].ScanV;
            }
            foreach (var sig in PowerSigs)
            {
                sig.MaxRenderIndex = index;
            }

            if (AutoFit)
                PowerGraph.Plot.AxisAuto();
            PowerGraph.Refresh();

        }
        
    }
}
