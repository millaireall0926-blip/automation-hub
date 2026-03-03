using StarQMOS.Model;
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
using System.Windows.Shapes;
using System.Windows.Threading;

namespace StarQMOS.View
{
    /// <summary>
    /// Result.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class Result : Window
    {
        public Result()
        {
            InitializeComponent();
         
            Global.gResultVM.ShowDetail = Visibility.Hidden;
            Global.gResultVM.xSP_Bin = xSP_Bin;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {            

        }

        private void MoveResult(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }

        private void xBTClose_Click(object sender, RoutedEventArgs e)
        {
            Hide();
            if (Global.gResultVM.MultiChipWin.IsEnabled)
                Global.gResultVM.MultiChipWin.Hide();
        }

        private void xCanvas_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            Global.gResultVM._bibWidth = e.NewSize.Width - 10;
            Global.gResultVM._bibHeight = e.NewSize.Height - 10;
            if (Global.gResultVM._bibWidth < 0) Global.gResultVM._bibWidth = 0;
            if (Global.gResultVM._bibHeight < 0) Global.gResultVM._bibHeight = 0;

            //Global.gResultVM.setSize();
            Global.gMainVM.updateUIFlag((int)UI.BIB_SIZE);
        }

        private void ListView_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            var item = (sender as ListView).SelectedItem;
            if (Global.gResultVM.MultiChipWin.IsEnabled)
                Global.gResultVM.MultiChipWin.Hide();
            if (item != null)
            {
                StepModel step = item as StepModel;
                Global.gResultVM.setStep(step.Name);

            }
        }

        private void ListViewDetail_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            var ctrl = sender as ListView;            
            var item = (sender as ListView).SelectedItem;
            if (item == null || ctrl == null) return;
            if (ctrl.Name == "xListView_Block")
            {
                SubFDetailModel bm = item as SubFDetailModel;
                if (Global.gResultVM._curDno == 0) return;
                Global.gResultVM._showMulti = false;
                Global.gResultVM._blockAdr = bm.No;
                Global.gMainVM.updateUIFlag((int)UI.RESULT_SUB);                
            }
            else if (ctrl.Name == "xListView_Page")
            {
                SubFDetailModel bm = item as SubFDetailModel;
                if (Global.gResultVM._curDno == 0) return;
                Global.gResultVM.selectPage( bm.No);
            }

        }

            private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ComboBox currentComboBox = sender as ComboBox;
            if (currentComboBox == null) return;
            int idx = currentComboBox.SelectedIndex;
            if (Global.gResultVM.MultiChipWin.IsEnabled)
                Global.gResultVM.MultiChipWin.Hide();
            Global.gResultVM.setDutText();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            
            Button bt = e.Source as Button;
            Global.gResultVM.CurDno = Convert.ToInt32(bt.Uid);
            Global.gResultVM._showMulti = true;
            Global.gResultVM._blockAdr = -1;
            Global.gMainVM.updateUIFlag((int)UI.RESULT_SUB);
            //            Global.gResultVM.selectDut( Convert.ToInt32(bt.Uid), true);
        }

        private void xCanvas_MouseDown(object sender, MouseButtonEventArgs e)
        {
//            if (Global.gResultVM.MultiChipWin.IsEnabled)
//                Global.gResultVM.MultiChipWin.Hide();
        }
    }
}
