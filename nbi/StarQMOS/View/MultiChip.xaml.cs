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

namespace StarQMOS.View
{
    /// <summary>
    /// MultiChip.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MultiChip : Window
    {
        public MultiChip()
        {
            InitializeComponent();

        }



        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
                this.Show();
            else if (e.Key == Key.Escape)
                this.Hide();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Button bt = e.Source as Button;
            Global.gResultVM.CurDno = Convert.ToInt32(bt.Uid);
            Global.gResultVM._showMulti = false;
            Global.gResultVM._blockAdr = -1;
            Global.gMainVM.updateUIFlag((int)UI.RESULT_SUB);
            //Global.gResultVM.selectDut(Convert.ToInt32(bt.Uid), false);
        }

        private void xCanvas_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            Global.gResultVM._mcWidth = e.NewSize.Width - 10;
            Global.gResultVM._mcHeight = e.NewSize.Height - 10;
            if (Global.gResultVM._mcWidth < 0) Global.gResultVM._bibWidth = 0;
            if (Global.gResultVM._mcHeight < 0) Global.gResultVM._bibHeight = 0;

            Global.gResultVM.setMcSize();
        }

        private void Image_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
                this.DragMove();
        }

        private void xBTClose_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();
        }
    }
}
