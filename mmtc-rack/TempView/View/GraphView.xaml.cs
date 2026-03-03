using HandyControl.Tools.Extension;
using System;
using System.Collections.Generic;
using System.Linq;
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
using TempView.Base;
using TempView.Model;

namespace TempView.ViewModel
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
            Colors.SeaGreen,
            Colors.DarkOliveGreen
            };


        public GraphView()
        {
            InitializeComponent();

            for (int i = 0; i < Global.gMaxDimmCount; i++)
            {
                Global.gGraphVM.DimDatas.Add(new DimDataModel());
                Global.gGraphVM.DimDatas[i].Name = $"Dm{i + 1}";
                Global.gGraphVM.DimDatas[i].ChanelColor = new SolidColorBrush(GraphColors[i]);
                
            }

            int index = 0;
            
            for (int i = 0; i < Global.gSVList.Count; i++)
            {
                Global.gGraphVM.EtcDatas.Add(new DimDataModel());
                Global.gGraphVM.EtcDatas[index].Name = "SV" + Global.gSVList[i];
                int colVal = 0xff - (i*16);
                int rgb = colVal << 16 | 0xaaaa;

                Global.gGraphVM.EtcDatas[index].ChanelColor = new SolidColorBrush(MConv.IntToMediaColor(0xff, rgb));
                index++;
            }

            for (int i = 0; i < Global.gPVList.Count; i++)
            {
                Global.gGraphVM.EtcDatas.Add(new DimDataModel());
                Global.gGraphVM.EtcDatas[index].Name = "PV" + Global.gPVList[i];
                int colVal = 0xff - (i * 16);
                int rgb = colVal << 8 | 0xaa00aa;
                Global.gGraphVM.EtcDatas[index].ChanelColor = new SolidColorBrush(MConv.IntToMediaColor(0xff, rgb));
                index++;
            }

            
            Global.gGraphVM.DimGraph = xDimGraph;
            Global.gGraphVM.initGraph();
        }

        

        private void ToggleDimmAll_Click(object sender, RoutedEventArgs e)
        {
            ToggleButton tb = sender as ToggleButton;
            
            for (int i = 0; i < Global.gMaxDimmCount; i++)
                Global.gGraphVM.DimDatas[i].Show = (bool)tb.IsChecked;

            Global.gGraphVM.setShow();
        }

        private void ToggleEtcAll_Click(object sender, RoutedEventArgs e)
        {
            ToggleButton tb = sender as ToggleButton;

            for (int i = 0; i < Global.gSVList.Count + Global.gPVList.Count; i++)
                Global.gGraphVM.EtcDatas[i].Show = (bool)tb.IsChecked;


            Global.gGraphVM.setShow();


        }

        private void xBoxA_Click(object sender, RoutedEventArgs e)
        {
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
        }

        private void xDimGraph_MouseMove(object sender, MouseEventArgs e)
        {
            (double x, double y) = xDimGraph.GetMouseCoordinates();
            Console.WriteLine($"Mouse at ({x}, {y})");
        }
    }
}
