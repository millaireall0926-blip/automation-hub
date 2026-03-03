using StarOS.ViewModel;
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
    /// LotInfo.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class LotInfo : Window
    {
        public LotInfo()
        {
            InitializeComponent();
            
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
                this.DragMove();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            this.DataContext = Global.gMainVM;

            foreach (CommandBinding binding in MainVM.Commands)
            {
                CommandBindings.Add(binding);

            }
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)           
                Global.gMainVM.excute_CmdLot(xBT_LotInput, null);
            else if (e.Key == Key.Escape)
                Global.gMainVM.excute_CmdLot(xBT_LotCancel, null);
        }
    }
}
