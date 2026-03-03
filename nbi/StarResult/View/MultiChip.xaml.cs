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

namespace StarResult.View
{
    /// <summary>
    /// Multichip.xaml에 대한 상호 작용 논리
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

        private void xCanvas_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            Global.gBIBVM._mcWidth = e.NewSize.Width - 10;
            Global.gBIBVM._mcHeight = e.NewSize.Height - 10;
            if (Global.gBIBVM._mcWidth < 0) Global.gBIBVM._bibWidth = 0;
            if (Global.gBIBVM._mcHeight < 0) Global.gBIBVM._bibHeight = 0;

            Global.gBIBVM.setMcSize();
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

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Button bt = e.Source as Button;
            Global.gBIBVM.CurDno = Convert.ToInt32(bt.Uid);            
            Global.gBIBVM._blockAdr = -1;
            Global.gBIBVM.selectDut(false);
        }
    }
}
