using RDimm_MMTCRACK.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RDimm_MMTCRACK.View
{
    /// <summary>
    /// DimmPage.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class DimmPage : Page
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
    Colors.Fuchsia,
    Colors.MediumPurple,
    Colors.BlueViolet,
    Colors.DarkOrchid,
    Colors.Purple,
    Colors.Indigo,
    Colors.DarkSlateBlue,
    Colors.MediumSlateBlue,

    Colors.LightBlue,
    Colors.SkyBlue,
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
    Colors.SeaGreen
    };

        public DimmPage()
        {
            InitializeComponent();

            for (int i = 0; i < Global.gMainPageVM.MaxDimmCount; i++)
            {
                Global.gDimmVM.DimDatas.Add(new DimDataModel());
                Global.gDimmVM.DimDatas[i].Name = $"Dm{i + 1}";
                Global.gDimmVM.DimDatas[i].ChanelColor = new SolidColorBrush(GraphColors[i]);
            }


            Global.gDimmVM.DimGraph = xDimGraph;
            Global.gDimmVM.initGraph();

            Rect initRect = Global.gRackMagins[0];
            if (Global.gMainPageVM.LayoutColumnCount == 1)
            {
                initRect = Global.gRackMaginsCol1[0];
            }

            xRect_Select.Margin = new Thickness(initRect.X,
                                                initRect.Y,
                                                initRect.Width,
                                                initRect.Height);

            Global.gDimmVM.setRack();
        }
        private void Image_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point pos = e.GetPosition(sender as Image);
            Console.WriteLine($" x {pos.X} , y {pos.Y}");

            int index = 0;

            Rect[] rects = Global.gRackRects;
            Rect[] margins = Global.gRackMagins;
            if (Global.gMainPageVM.LayoutColumnCount == 1)
            {
                rects = Global.gRackRectsCol1;
                margins = Global.gRackMaginsCol1;
            }

            foreach (Rect rect in rects)
            {
                if (rect.Contains(pos))
                {
                    if (index >= Global.gMainPageVM.Racks.Count) return;
                    xRect_Select.Margin = new Thickness(margins[index].X,
                                                        margins[index].Y,
                                                        margins[index].Width,
                                                        margins[index].Height);
                    Global.gDimmVM.CurRack = index;
                    Global.gDimmVM.setRack();

                    break;
                }
                index++;
            }

        }

        private void ToggleDimmAll_Click(object sender, RoutedEventArgs e)
        {
            ToggleButton tb = sender as ToggleButton;

            for (int i = 0; i < Global.gMainPageVM.MaxDimmCount; i++)
                Global.gDimmVM.DimDatas[i].Show = (bool)tb.IsChecked;

        }

        private void xBoxA_Click(object sender, RoutedEventArgs e)
        {
            ToggleButton btn = sender as ToggleButton;
            bool bShow = (bool)btn.IsChecked;
            int index = 0;
            if (btn.Name == "xBoxA") index = 0;
            else if (btn.Name == "xBoxB") index = 1;
            else if (btn.Name == "xBoxC") index = 2;

            var rack = Global.gMainPageVM.getRack(Global.gDimmVM.CurRack);

            if (rack.CBoxs.Count <= index) return;

            foreach (int dim in rack.CBoxs[index].DimCtrlList)
            {
                if (Global.gDimmVM.DimDatas.Count <= dim) continue;
                Global.gDimmVM.DimDatas[dim].Show = bShow;
            }
        }
    }
}
