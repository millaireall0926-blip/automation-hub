using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using StarBase;
using StarOS.Model;
using ScottPlot;
using ScottPlot.Plottable;
using ScottPlot.Renderable;

namespace StarOS.ViewModel
{
    public enum PE_TYPE : int
    { 
        VIH = 0,
        VIL = 1,
        VTH = 2
    }

    public class PowerVM : BaseVM
    {


        Color[] GraphColors = new Color[] {
            Colors.Coral,
            Colors.Tomato,
            Colors.OrangeRed,
            Colors.DarkOrange,
            Colors.Gold,
            Colors.DarkKhaki,
            Colors.Thistle,
            Colors.Plum,
            Colors.Violet,
            Colors.Orchid,

  
            Colors.DeepSkyBlue,
            Colors.DodgerBlue,
            Colors.RoyalBlue,
            Colors.Blue,
            Colors.GreenYellow,
            Colors.LawnGreen,
            Colors.Lime,
            Colors.LimeGreen,
            Colors.PaleGreen,
            Colors.SpringGreen,
            
            
            };

        string[] PENames = new string[]
        {
            "HDPS0","HDPS1",
            "VIHIO","VIHDR",            
            "VTTIO","VTTDR",
            "DUTIO","DUTDR","DUTRF",
            "VTH",

        };

        public static int DPS_SIZE = 20;
        public static int HALF_SIZE = DPS_SIZE / 2;
        public static int PE_SIZE = 10;

        public DPSModel[] DPS1s { get; set; }
        public DPSModel[] DPS2s { get; set; }
        public PEModel[] PEs { get; set; }
        public SolidColorBrush[] Brushs { get; set; }

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
            DPS1s = new DPSModel[HALF_SIZE];
            DPS2s = new DPSModel[HALF_SIZE];
            PEs = new PEModel[PE_SIZE];
            Brushs = new SolidColorBrush[DPS_SIZE*2];
            for (int i = 0; i < DPS_SIZE; i++) {
                Color col = GraphColors[i];
                if (col.R < 250)
                    col.R += 5;
                if (col.G < 250)
                    col.G += 5;
                if (col.B < 250)
                    col.B += 5;

                Brushs[i * 2] = new SolidColorBrush(GraphColors[i]);
                Brushs[i * 2 + 1] = new SolidColorBrush(col);
            }
            
            
        }

        public DPSModel makeModel (int index)
        {
            var model =  new DPSModel();

            model.Name = index.ToString();
            model.ColorV = Brushs[index*2];
            model.ColorC = Brushs[index*2+1]; 
            model.GetV = string.Format("{0,5}", "-");
            model.GetC = string.Format("{0,5}", "-");
            model.SetV = string.Format("{0,5}", "-");
            model.OC = string.Format("{0,5}", "-"); ;

            return model;
        }

        public DPSModel getModel(int index)
        {
            if (index >= DPS_SIZE) return null;

            if (index >= HALF_SIZE)
                return DPS2s[index % HALF_SIZE];

            return DPS1s[index % HALF_SIZE];
        }

        public DPSModel[] getModels(int index, out int modelIndex)
        {
            DPSModel[] models = DPS1s;
            if (index >= HALF_SIZE) models = DPS2s;
            modelIndex = index % HALF_SIZE;
            return models;
        }



        public void initItems()
        {
            for (int i = 0; i < DPS_SIZE ; i++)
            {
                var models = getModels(i, out int index);                
                models[index] =  makeModel(i);
                
            }
            DPS1s[0].Show = true;
            DPS2s[0].Show = true;


            for (int i = 0; i < PE_SIZE; i++)
            {
                PEs[i] = new PEModel(); 
                PEs[i].Name = PENames[i];
                PEs[i].SetV = string.Format("{0,5}", "-");                
                
                
            }

            PowerGraph.Plot.Title("Power");
            PowerGraph.Plot.YLabel("Level/Current");

            PowerGraph.Plot.Style(ScottPlot.Style.Gray2);
            PowerSigs = new SignalPlot[DPS_SIZE * 2];

            for (int i = 0; i < DPS_SIZE; i++)
            {
                DPSModel[] models = getModels(i, out int index);
                

                PowerSigs[i * 2]        = PowerGraph.Plot.AddSignal(models[index]._graphDataV, label: String.Format($"DPS{i}V"));
                PowerSigs[i * 2].LineWidth  = 3;
                PowerSigs[i * 2].MarkerSize = 0;
                PowerSigs[i * 2].MaxRenderIndex = 0;

                Color col = Brushs[i * 2].Color;
                PowerSigs[i * 2].Color = System.Drawing.Color.FromArgb(
                    col.A, col.R, col.G, col.B);
                    
                PowerSigs[(i * 2) + 1]  = PowerGraph.Plot.AddSignal(models[index]._graphDataC, label: String.Format($"DPS{i}C"));
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
                for (int i = 0; i < DPS_SIZE; i++)
                {
                    var models = getModels(i, out int index);
                    
                    Array.Copy(slot._DPSs[i]._graphDataV, models[index]._graphDataV, slot._pwrGraphIndex);
                    Array.Copy(slot._DPSs[i]._graphDataC, models[index]._graphDataC, slot._pwrGraphIndex);             
                }
            }
            PowerGraph.Refresh();

            updatePowerSet(slot);
        }

        public void updatePowerSet(SlotModel slot)
        {

            for (int i = 0; i < DPS_SIZE; i++)
            {
                var models = getModels(i, out int index);
                models[index].SetV = slot._DPSs[i].SetV;
                models[index].OC = slot._DPSs[i].OC;
            }

            for (int i = 0; i < PE_SIZE; i++)
            {
                PEs[i].SetV = slot._PEs[i].SetV;

            }
        }

        public void updatePower(SlotModel slot)
        {
            bool isData = true;
            //if (slot._DPSs[0].SetV == string.Format("{0,5}", "-"))
            //    isData = false;

            int graphIndex = slot._pwrGraphIndex - 1;
            if (graphIndex < 0) graphIndex = 0;

            for (int i = 0;i < DPS_SIZE;i++)
            {
                var models = getModels(i, out int index);
                
                models[index].GetV = slot._DPSs[i].GetV;
                models[index].GetC = slot._DPSs[i].GetC;
                
                PowerSigs[i * 2].IsVisible = models[index].Show;
                PowerSigs[(i * 2) + 1].IsVisible = models[index].Show;

                double v, c;
                double.TryParse(models[index].GetV, out v);
                double.TryParse(models[index].GetC, out c);

                if (isData) models[index].setData(v,c, graphIndex);                
            }

            
            foreach (var sig in PowerSigs)
            {
                sig.MaxRenderIndex = graphIndex;
            }

            if (AutoFit)
                PowerGraph.Plot.AxisAuto();
            PowerGraph.Refresh();

        }
        
    }
}
