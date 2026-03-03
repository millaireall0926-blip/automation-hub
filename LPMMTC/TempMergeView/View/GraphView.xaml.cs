using HandyControl.Tools.Extension;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
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
using TempMergeView.Base;
using TempMergeView.Model;

namespace TempMergeView.ViewModel
{
    /// <summary>
    /// GraphView.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class GraphView : Page
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


        public GraphView()
        {
            InitializeComponent();

            for (int i = 0; i < Global.gMaxDimmCount; i++)
            {
                Global.gGraphVM.LPDatas.Add(new DimDataModel());
                Global.gGraphVM.LPDatas[i].Name = $"LP{i + 1}";
                Global.gGraphVM.LPDatas[i].ChanelColor = new SolidColorBrush(GraphColors[i]);
                
            }

            for (int i = 0; i < Global.gMaxDimmCount; i++)
            {
                Global.gGraphVM.SVDatas.Add(new DimDataModel());
                Global.gGraphVM.SVDatas[i].Name = $"SV{i + 1}";
                int colVal = 0xff - (i*16);
                int rgb = colVal << 16 | 0xaaaa;

                Global.gGraphVM.SVDatas[i].ChanelColor = new SolidColorBrush(MConv.IntToMediaColor(0xff, rgb));

                Global.gGraphVM.PVDatas.Add(new DimDataModel());
                Global.gGraphVM.PVDatas[i].Name = $"PV{i + 1}";
                colVal = 0xff - (i * 16);
                rgb = colVal << 8 | 0xaa00aa;
                Global.gGraphVM.PVDatas[i].ChanelColor = new SolidColorBrush(MConv.IntToMediaColor(0xff, rgb));
            }

            Global.gGraphVM.DimGraph = xDimGraph;
            Global.gGraphVM.initGraph();
        }

        

        private void ToggleDimmAll_Click(object sender, RoutedEventArgs e)
        {
            ToggleButton tb = sender as ToggleButton;
            
            for (int i = 0; i < Global.gMaxDimmCount; i++)
                Global.gGraphVM.LPDatas[i].Show = (bool)tb.IsChecked;

            Global.gGraphVM.setShow();
        }

        private void ToggleSVAll_Click(object sender, RoutedEventArgs e)
        {
            ToggleButton tb = sender as ToggleButton;

            for (int i = 0; i < Global.gMaxDimmCount; i++)
            {
                Global.gGraphVM.SVDatas[i].Show = (bool)tb.IsChecked;
            }
            Global.gGraphVM.setShow();
        }

        private void TogglePVAll_Click(object sender, RoutedEventArgs e)
        {
            ToggleButton tb = sender as ToggleButton;

            for (int i = 0; i < Global.gMaxDimmCount; i++)
            {
                Global.gGraphVM.PVDatas[i].Show = (bool)tb.IsChecked;
            }
            Global.gGraphVM.setShow();
        }

        private void xBoxA_Click(object sender, RoutedEventArgs e)
        {
            /*
            ToggleButton btn = sender as ToggleButton;
            bool bShow = (bool)btn.IsChecked;
            int index = 0;
            if (btn.Name =="xBoxA") index = 0;
            else if (btn.Name == "xBoxB") index = 1;
            else if (btn.Name == "xBoxC") index = 2;


            foreach (var dim in Global.gBoxInDimmList[index])
            {
                Global.gGraphVM.DimDatas[dim].Show = bShow;
            }
            Global.gGraphVM.setShow();
            */
        }

        private void xDimGraph_MouseMove(object sender, MouseEventArgs e)
        {
            (double x, double y) = xDimGraph.GetMouseCoordinates();
            Console.WriteLine($"Mouse at ({x}, {y})");
        }
    }
}
