using StarResult.Model;
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
    /// DutDetailView.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class DutDetailView : Page
    {
        public DutDetailView()
        {
            InitializeComponent();
            this.DataContext = Global.gBIBVM;
        }

        private void ListViewDetail_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            var ctrl = sender as ListView;
            var item = (sender as ListView).SelectedItem;
            if (item == null || ctrl == null) return;

            if (ctrl.Name == "xListView_Block")
            {
                return;
            }
            else if (ctrl.Name == "xListView_Page")
            {
                SubFDetailModel bm = item as SubFDetailModel;
                if (Global.gBIBVM.CurDno == 0) return;
                Global.gBIBVM.selectPage(bm.No);
            }
        }
    }
}
