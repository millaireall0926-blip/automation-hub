using LP_MMTCRack.Model;
using LP_MMTCRack.ViewModel;
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

namespace LP_MMTCRack.View
{
    /// <summary>
    /// ConfigPage.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class ConfigPage : Page
    {
        public ConfigPage()
        {
            InitializeComponent();
        }

        private void TabControl_Selected(object sender, SelectionChangedEventArgs e)
        {
            var tc = sender as HandyControl.Controls.TabControl;
            if (tc != null)
            {
                var tab = tc.SelectedItem as System.Windows.Controls.TabItem;
                string sel = tab.Header.ToString();
            }
        }
	}
}
