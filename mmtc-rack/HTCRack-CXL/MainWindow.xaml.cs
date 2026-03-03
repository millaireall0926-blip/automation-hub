using HTCRack_CXL.ViewModel;
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
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace HTCRack_CXL
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            xTB_Title.Text = VerInfo.getVer();
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            Global.gConfigVM?.saveItems();
            Global.gMainVM?.quit();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
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

                MessageBoxResult res = HandyControl.Controls.MessageBox.Show($"Do You Want to Close System", "Quit", MessageBoxButton.YesNo, MessageBoxImage.Warning);
                if (res == MessageBoxResult.No) return;
                
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Application]: Close");

                this.Close();
            }
        }

        private void MoveTitle(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }

        private void SideMenuItem_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            Storyboard sb = (Storyboard)FindResource("storyViewChange");
            sb.Begin();

            HandyControl.Controls.SideMenuItem sd = sender as HandyControl.Controls.SideMenuItem;
            if (sd == null) return;

            Global.gMainVM.changeView(sd.Uid);
        }
    }
}
