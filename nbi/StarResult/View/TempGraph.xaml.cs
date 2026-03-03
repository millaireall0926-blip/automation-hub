using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace StarResult.View
{
    /// <summary>
    /// TempGraph.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class TempGraph : Page
    {
        public TempGraph()
        {
            InitializeComponent();
            this.DataContext = Global.gGraphVM;

            Global.gGraphVM.xTempGraph = xTempGraph;
            Global.gGraphVM.initTemp();
        }

        private void Graph_Mode_Select(object sender, RoutedEventArgs e)
        {
            Global.gGraphVM.setTempMode();
        }
    }
}
