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
using StarOS.Model;

namespace StarOS.View
{
    /// <summary>
    /// PowerPage.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class PowerPage : Page
    {
        
        public PowerPage()
        {
            InitializeComponent();


            Global.gPowerVM.PowerGraph = xPowerGraph;
            Global.gPowerVM.initItems();
        }

        private void ToggleButton_Click(object sender, RoutedEventArgs e)
        {
            if (Global.gSlotVM.CurSlot == null) return;
            Global.gPowerVM.updatePower(Global.gSlotVM.CurSlot);
        }
    }
}
