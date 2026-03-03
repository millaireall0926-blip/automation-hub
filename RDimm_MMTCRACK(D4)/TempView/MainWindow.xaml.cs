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

namespace TempView
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            Global.gMainView = this;

            xTB_Title.Text = VerInfo.getVer();
        }

        private void Window_Closed(object sender, EventArgs e)
        {

        }

        private void ClickTileButton(object sender, RoutedEventArgs e)
        {
            Button bt = e.Source as Button;
            if (bt == null) return;
            if (bt.Name == "xBTMin")
            {
                this.WindowState = WindowState.Minimized;
            }
            else if (bt.Name == "xBTRestore")
            {
                this.WindowState = (this.WindowState == WindowState.Normal) ? WindowState.Maximized : WindowState.Normal;
            }
            else if (bt.Name == "xBTClose")
            {
                this.Close();
            }
        }

        private void MoveTitle(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }
    }
}
