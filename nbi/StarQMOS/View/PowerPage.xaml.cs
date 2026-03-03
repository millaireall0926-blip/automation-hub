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
using StarQMOS.Model;

namespace StarQMOS.View
{
    /// <summary>
    /// PowerPage.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class PowerPage : Page
    {
        
        public PowerPage()
        {
            InitializeComponent();
            for (int i = 0; i < 5; i++)
                Global.gPowerVM.DPSs[i] = (DPSModel)this.Resources[string.Format($"xDPS{(i+1)}")];

            Global.gPowerVM.PEs[0] = (PEModel)this.Resources["xPE_VIH"];
            Global.gPowerVM.PEs[1] = (PEModel)this.Resources["xPE_VIL"];
            Global.gPowerVM.PEs[2] = (PEModel)this.Resources["xPE_VTH"];


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
