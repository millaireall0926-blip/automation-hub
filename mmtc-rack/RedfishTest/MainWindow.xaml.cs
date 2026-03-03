using RedfishTest.ViewModel;
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

namespace RedfishTest
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            MainVM vm = DataContext as MainVM;
            vm.RespParagraph = new Paragraph();
            xRespTextBox.Document = new FlowDocument(vm.RespParagraph);
            vm.RespParagraph.LineHeight = 1;

            vm.RespScroll = xRespScroll;
            vm.ComboSelectedIndex = 0;
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            MainVM vm = DataContext as MainVM;
            if (vm != null)
            {
                if (vm._isStop == false)
                {
                    vm._isStop = true;
                    vm._getThread.Join();
                }
            }
        }
    }


}
