using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.Model;
using RDimm_MMTCRACK.View.Component;
using ScottPlot.Plottable;
using ScottPlot;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace RDimm_MMTCRACK.ViewModel
{
    public class BoxDataModel : BaseVM
    {
        public string Name { get; set; }
        public double BoxInTemp { get; set; } = 0;
        public ObservableCollection<DimDataModel> DimmsInBox { get; set; } = new ObservableCollection<DimDataModel>();
        public double Left { get; set; }
        public double Top { get; set; }

        public string BoxSVC { get; set; }

        public string SVValue { get; set; }
    }

    public class DimmVM : BaseVM
    {

        int _curRack = 0;
        public int CurRack
        {
            get => _curRack;
            set { _curRack = value; CurName = $"Rack" + (_curRack + 1).ToString(); }
        }

        public string EquipImg { get; set; } = "/Resource/equip.png";

        public bool AutoFit { get; set; } = true;
        public string CurName { get; set; } = "Rack1";
        public string BaseLineText { get; set; } = "";


        public WpfPlot DimGraph;
        SignalPlot[] DimSigs;
        HLine MinLine;
        HLine MaxLine;

        public ObservableCollection<DimDataModel> DimDatas { get; set; } = new ObservableCollection<DimDataModel>();
        public ObservableCollection<BoxDataModel> BoxDatas { get; set; } = new ObservableCollection<BoxDataModel>();
        //public ObservableCollection<ManualControl> ManualControls { get; set; } = new ObservableCollection<ManualControl>();
        public Visibility ShowManualContol { get; set; } = Visibility.Hidden;


        public List<MarkerPlot> Markers = new List<MarkerPlot>();


        public DimmVM()
        {
            Global.gDimmVM = this;
            DimSigs = new SignalPlot[Global.gMainPageVM.MaxDimmCount];


            if (Global.gMainPageVM.LayoutColumnCount == 1)
                EquipImg = "/Resource/equip_col1.png";
            else if (Global.gMainPageVM.LayoutColumnCount == 2)
                EquipImg = "/Resource/equip.png";

            //for (int i = 0; i < 3; i++)
            //    ManualControls.Add(new ManualControl(i));
        }

        public void setMinMax(double min, double max)
        {
            MinLine.Y = min;
            MaxLine.Y = max;
            //DimGraph.Plot.AddHorizontalLine(Global.gConfigVM.MaxDimm, color: System.Drawing.Color.Red, width: 1);
            //DimGraph.Plot.AddHorizontalLine(Global.gConfigVM.MinDimm, color: System.Drawing.Color.OrangeRed, width: 1);
            //DimGraph.Refresh();
        }

        public void initGraph()
        {
            for (int i = 0; i < Global.gMainPageVM.MaxDimmCount; i++)
            {
                DimSigs[i] = DimGraph.Plot.AddSignal(DimDatas[i].GraphData,
                    color: MConv.toMediaColor(DimDatas[i].ChanelColor.Color),
                    label: String.Format($"DM{i + 1}"));
                DimSigs[i].LineWidth = 2;
                DimSigs[i].MarkerSize = 3;
                DimSigs[i].MaxRenderIndex = 0;
            }

            MinLine = DimGraph.Plot.AddHorizontalLine(0, color: System.Drawing.Color.Red, width: 1);
            MaxLine = DimGraph.Plot.AddHorizontalLine(0, color: System.Drawing.Color.OrangeRed, width: 1);
            MinLine.DragEnabled = true;
            MaxLine.DragEnabled = true;
            MinLine.LineStyle = LineStyle.Dash;
            MaxLine.LineStyle = LineStyle.Dash;


            DimGraph.Refresh();
        }

        public void processRun(int rackindex)
        {
            RackModel rack = Global.gMainPageVM.getRack(CurRack);
            updateGraph(rack, rackindex);
            if (rack.CBoxs.Count != BoxDatas.Count) return;

            for (int i = 0; i < rack.CBoxs.Count; i++)
            {
                CBoxModel box = rack.CBoxs[i];
                BoxDataModel boxData = BoxDatas[i];
                boxData.BoxInTemp = box.InsideBoxTemp;
                boxData.BoxSVC = "";
                boxData.SVValue = "";


                    foreach (SVC svc in Enum.GetValues(typeof(SVC)))
                    {
                        if (box.checkSV(svc))
                            boxData.BoxSVC += svc.ToString().Substring(0, 1) + " ";
                    }

                    boxData.SVValue += Math.Round(box.TargetSV, 1).ToString() + " ";

                if (box.DimModels.Count != boxData.DimmsInBox.Count) return;
                for (int j = 0; j < box.DimModels.Count; j++)
                {
                    boxData.DimmsInBox[j].Temp = box.DimModels[j].DimTemp;
                    boxData.DimmsInBox[j].TempLow = box.DimModels[j].DimTempLow;
                    boxData.DimmsInBox[j].TempHigh = box.DimModels[j].DimTempHigh;

                    boxData.DimmsInBox[j].TempText = $"{box.DimModels[j].DimTempLow} | {box.DimModels[j].DimTempHigh}";

                }
            }

        }

        public void setRack()
        {
            RackModel rack = Global.gMainPageVM.getRack(CurRack);
            BaseLineText = "";
            BoxDatas.Clear();
            int boxLeft = 0;
            foreach (CBoxModel box in rack.CBoxs)
            {
                BoxDataModel boxData = new BoxDataModel();
                boxData.Left = boxLeft;
                boxData.Top = 24;
                boxData.Name = "Inside " + box.Name;

                BoxDatas.Add(boxData);

                double dimTop = 0;
                double dimLeft = 0;
                int dimTotal = 0;

                dimTotal += box.DimCtrlList.Count;

                if (dimTotal < 7)
                    dimLeft = 7 * dimTotal;
                else
                    dimLeft = 4 * dimTotal;



                foreach (int index in box.DimCtrlList)
                {
                    boxData.DimmsInBox.Add(DimDatas[index]);
                    DimDatas[index].Left = dimLeft;
                    DimDatas[index].Top = dimTop;
                    DimDatas[index].Temp = 0;
                    DimDatas[index].TempLow = 0;
                    DimDatas[index].TempHigh = 0;
                    DimDatas[index].TempText = "";
                    dimTop += 22;
                    dimLeft -= 5;
                }


                boxLeft += 260;
            }

            foreach (var sig in DimSigs)
            {
                sig.MaxRenderIndex = rack.GraphIndex;
            }

            if (rack.GraphIndex != 0)
            {
                for (int i = 0; i < Global.gMainPageVM.MaxDimmCount; i++)
                {
                    Array.Copy(rack.DimDatas[i].GraphData, DimDatas[i].GraphData, rack.GraphIndex);
                }
            }

            setMinMax(rack.RackSetSV - 5, rack.RackSetSV + 5);

            removeMakerAll();
            foreach (var mm in rack.FailMakers)
                AddMarker(mm.Name, mm.Y, mm.X);

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

        public void updateGraph(RackModel rack, int gindex)
        {
            int index = gindex - 1;
            if (index < 0) index = 0;

            for (int i = 0; i < Global.gMainPageVM.MaxDimmCount; i++)
            {
                DimDatas[i].Temp = rack.DimDatas[i].Temp;
                DimDatas[i].GraphData[index] = DimDatas[i].Temp;

                DimSigs[i].IsVisible = DimDatas[i].Show;
            }

            foreach (var sig in DimSigs)
            {
                sig.MaxRenderIndex = index;
            }

            if (rack.FailMakers.Count > Markers.Count)
            {
                for (int i = rack.FailMakers.Count - 1; i >= Markers.Count; i--)
                {
                    MarkerModel mm = rack.FailMakers[i];
                    AddMarker(mm.Name, mm.Y, mm.X);
                }
            }
            else if (rack.FailMakers.Count < Markers.Count)
            {
                removeMakerAll();
            }

            if (AutoFit)
                DimGraph.Plot.AxisAuto();
            DimGraph.Refresh();
        }
    }
}
