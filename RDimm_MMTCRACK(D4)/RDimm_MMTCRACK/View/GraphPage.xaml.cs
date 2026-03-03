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
    /// GraphPage.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class GraphPage : Page
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
                };
        public GraphPage()
        {
            InitializeComponent();
            /*
            for (int i = 0; i < 18; i++)
            {
                Global.gGraphVM.TempDatas.Add(new TempDataModel());
                Global.gGraphVM.TempDatas[i].Name = $"CH{i + 1}";
                Global.gGraphVM.TempDatas[i].ChannelColor = new SolidColorBrush(GraphColors[i]);
            }
            Global.gGraphVM.TempGraph = xTempGraph;
            Global.gGraphVM.InitGraph();
            */
        }

        private void ToggleChAll_Click(object sender, RoutedEventArgs e)
        {
            ToggleButton tb = sender as ToggleButton;

            for (int i = 0; i < 18; i++)
                Global.gGraphVM.TempDatas[i].Show = (bool)tb.IsChecked;
        }
    }
}
