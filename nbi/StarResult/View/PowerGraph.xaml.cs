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
    /// PowerGraph.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class PowerGraph : Page
    {
        public PowerGraph()
        {
            InitializeComponent();
            this.DataContext = Global.gGraphVM;

            Global.gGraphVM.xPowerGraph = xPowerGraph;
            Global.gGraphVM.initPower();
        }



        private void CheckBox_Click(object sender, RoutedEventArgs e)
        {
            if (Global.gGraphVM.xPowerGraph == null) return;
            CheckBox cb = sender as CheckBox;
            bool bCheck = (bool)cb.IsChecked;            
            Global.gGraphVM.setCheckBox(cb.Name, bCheck);
        }
    }
}
