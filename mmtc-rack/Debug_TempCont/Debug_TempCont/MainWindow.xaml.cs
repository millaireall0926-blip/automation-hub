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
using Debug_TempCont.ViewModel;

namespace Debug_TempCont
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        GraphVM gvm = null;
        public MainWindow()
        {
            InitializeComponent();
            Global.gMainWindow = this;
            gvm = (GraphVM)App.Current.Services.GetService(typeof(GraphVM));
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if(Global.gSerialComm.sPort.IsOpen)
                Global.gSerialComm.Connect_Stop();

            if(Global.gGraphView.Graphtimer.IsEnabled)
                Global.gGraphView.Graphtimer.Stop();
        }

        private void Expander_Expanded(object sender, RoutedEventArgs e)
        {
            Global.gMainWindow.Width = 1600;
        }

        private void Expander_Collapsed(object sender, RoutedEventArgs e)
        {
            Global.gMainWindow.Width = 850;
        }
    }
}
