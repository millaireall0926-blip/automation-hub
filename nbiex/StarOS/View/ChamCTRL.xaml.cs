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

namespace StarOS.View
{
    /// <summary>
    /// ChamCTRL.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class ChamCTRL : Window
    {
        public ChamCTRL()
        {
            InitializeComponent();
        }

        private void Rectangle_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
                this.DragMove();
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
                this.Show();
            else if (e.Key == Key.Escape)
                this.Hide();
        }

        private void xBTClose_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Button bt = e.Source as Button;
            if (bt == null) return;
            Global.gChamberVM.excute_CmdCtrl(bt);
        }
    }
}
