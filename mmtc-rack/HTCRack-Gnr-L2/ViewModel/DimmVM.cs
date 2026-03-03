using HTCRack.Base;
using HTCRack.Model;
using ScottPlot;
using ScottPlot.Plottable;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows;

namespace HTCRack.ViewModel
{

    public class BoxDataModel :BaseVM
    {
        public string Name { get; set; }
        public double BoxInTemp { get; set; } = 0;               
        public ObservableCollection<DimDataModel> DimmsInBox { get; set; } = new ObservableCollection<DimDataModel>();
        public double Left { get; set; }
        public double Top { get; set; }

        public string BoxSVC { get; set; }

        public string SVValue { get; set; }
    }

    public class ManualControl : BaseVM
    {
        public ManualControl(int index) {
            Index = index;
            DGCmdPlus = new DelegateCommand(p =>
            {
                if (IsAuto) return;
                string arg = p as string;
                double plus = Convert.ToDouble(arg);
                RackModel rack = Global.gMainPageVM.getRack(Global.gDimmVM.CurRack);
                CBoxModel box = rack.CBoxs[Index];
                foreach (CBoxSub sub in box.BoxSubs)
                {
                    sub.TargetSV += plus;
                }
            });

            DGCmdMinus = new DelegateCommand(p =>
            {
                if (IsAuto) return;
                string arg = p as string;
                double minus = Convert.ToDouble(arg);
                RackModel rack = Global.gMainPageVM.getRack(Global.gDimmVM.CurRack);
                CBoxModel box = rack.CBoxs[Index];
                foreach (CBoxSub sub in box.BoxSubs)
                {
                    sub.TargetSV -= minus;
                }
            });
        }
        public int Index { get; set; }
        public bool IsAuto { get; set; } = false;
        public DelegateCommand DGCmdPlus { get; set; }
        public DelegateCommand DGCmdMinus { get; set; }

        

    }


    public class DimmVM : BaseVM {

        int _curRack = 0;
        public int CurRack { 
            get => _curRack; 
            set { _curRack = value; CurName = $"FIX"+(_curRack+1).ToString();} 
        }

        public string EquipImg { get; set; } = "/Resource/equip_3x2.png";

        public bool AutoFit { get; set; } = true;
        public string CurName { get; set; } = "FIX1";
        public string BaseLineText { get; set; } = "";
        

        public WpfPlot DimGraph;
        SignalPlot[] DimSigs;
        HLine MinLine;
        HLine MaxLine;
        
        public ObservableCollection<DimDataModel> DimDatas { get; set; } = new ObservableCollection<DimDataModel>();        
        public ObservableCollection<BoxDataModel> BoxDatas { get; set; } = new ObservableCollection<BoxDataModel>();
        public ObservableCollection<ManualControl> ManualControls { get; set; } = new ObservableCollection<ManualControl>();
        public Visibility ShowManualContol { get; set; } = Visibility.Hidden;


        public List<MarkerPlot> Markers = new List<MarkerPlot>();


        public DimmVM() { 
            Global.gDimmVM = this;
            DimSigs = new SignalPlot[Global.gMainPageVM.MaxDimmCount];

            
            if (Global.gMainPageVM.LayoutColumnCount == 1)
                EquipImg = "/Resource/equip_col1.png";
            else if (Global.gMainPageVM.LayoutColumnCount == 2)
                EquipImg = "/Resource/equip_3x2.png";

            for (int i = 0; i < 3; i++)
                ManualControls.Add( new ManualControl(i) );
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
            // 현재 시간을 OADate 값으로 변환
            //double startTimestamp = DateTime.Now.ToOADate();

            for (int i = 0; i < Global.gMainPageVM.MaxDimmCount ; i++)
            {
                DimSigs[i] = DimGraph.Plot.AddSignal(DimDatas[i].GraphData,
                    color: MConv.toMediaColor(DimDatas[i].ChanelColor.Color),
                    label: String.Format($"DM{i + 1}"),
                    //sampleRate: 24 * 60 * 60);
                    sampleRate: 1); // 1초단위 누적
                DimSigs[i].LineWidth = 2;
                DimSigs[i].MarkerSize = 3;
                DimSigs[i].MaxRenderIndex = 0;
                DimSigs[i].OffsetX = 0;
                //DimSigs[i].OffsetX = 2;
                //DimSigs[i].OffsetX = startTimestamp;
            }

            MinLine = DimGraph.Plot.AddHorizontalLine(0, color: System.Drawing.Color.Red, width: 1);
            MaxLine = DimGraph.Plot.AddHorizontalLine(0, color: System.Drawing.Color.OrangeRed, width: 1);
            MinLine.DragEnabled = true;
            MaxLine.DragEnabled = true;
            MinLine.LineStyle = LineStyle.Dash;
            MaxLine.LineStyle = LineStyle.Dash;

            DimGraph.Plot.XAxis.TickLabelFormat(x =>
            {
                int totalSecconds = (int)Math.Round(x);
                int hours = totalSecconds / 3600;
                int minutes = (totalSecconds % 3600) / 60;
                int seconds = totalSecconds % 60;
                return $"{hours}:{minutes:D2}:{seconds:D2}";
            });
            //DimGraph.Plot.XAxis.TickLabelFormat("dD H:m:s", dateTimeFormat: true);
            //DimGraph.Plot.XAxis.TickLabelFormat("MM-dd HH:mm:ss", dateTimeFormat: true);
            DimGraph.Plot.AxisAuto();
            DimGraph.Refresh();
        }

        public void processRun(int rackindex)
        {
            RackModel rack = Global.gMainPageVM.getRack(CurRack);
            updateGraph(rack, rackindex);
            if (rack.CBoxs.Count != BoxDatas.Count) return;

            for (int i = 0; i< rack.CBoxs.Count; i++)
            {
                CBoxModel box = rack.CBoxs[i];
                BoxDataModel boxData = BoxDatas[i];
                boxData.BoxInTemp = box.InsideBoxTemp;                
                boxData.BoxSVC = "";
                boxData.SVValue = "";

                foreach (CBoxSub sub in box.BoxSubs)
                {
                    foreach (SVC svc in Enum.GetValues(typeof(SVC)))
                    {
                        if (box.checkSV(sub, svc))
                            boxData.BoxSVC += svc.ToString().Substring(0, 1) + " ";
                    }

                    boxData.SVValue += Math.Round(sub.TargetSV, 1).ToString() + " ";
                }
                if (box.DimModels.Count != boxData.DimmsInBox.Count) return;
                for (int j = 0; j< box.DimModels.Count; j++)
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
                boxData.Name = "Inside " +  box.Name;

                BoxDatas.Add(boxData);

                double dimTop = 0;
                double dimLeft = 0;
                int dimTotal = 0;

                foreach (CBoxSub sub in box.BoxSubs)
                    dimTotal += sub.DimCtrlList.Count;

                if (dimTotal < 7)
                    dimLeft = 7 * dimTotal;
                else
                    dimLeft = 4 * dimTotal;

                
                foreach (CBoxSub sub in box.BoxSubs)
                {
                    foreach (int index in sub.DimCtrlList)
                    {
                        boxData.DimmsInBox.Add(DimDatas[index]);
                        DimDatas[index].Left = dimLeft;
                        DimDatas[index].Top = dimTop;
                        DimDatas[index].Temp = 0;
                        DimDatas[index].TempLow = 0;
                        DimDatas[index].TempHigh = 0;

                        dimTop += 21;
                        dimLeft -= 5;
                    }
                }

                boxLeft += 300;
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

            setMinMax(rack.SetSV - 5, rack.SetSV + 5);

            removeMakerAll();
            foreach(var mm in rack.FailMakers)                            
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
                for (int i = rack.FailMakers.Count -1; i >= Markers.Count; i--)
                {
                    MarkerModel mm = rack.FailMakers[i];
                    AddMarker(mm.Name, mm.Y, mm.X);
                }
            }else if (rack.FailMakers.Count < Markers.Count)
            {
                removeMakerAll();
            }

                if (AutoFit)
                DimGraph.Plot.AxisAuto();
            DimGraph.Refresh();
        }

    }
}
