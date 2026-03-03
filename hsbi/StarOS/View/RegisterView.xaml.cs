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
    /// RegisterView.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class RegisterView : Window
    {
        public RegisterView()
        {
            InitializeComponent();
            this.DataContext = Global.gTestVM;
        }

        private void xBTClose_Click(object sender, RoutedEventArgs e)
        {
            Hide();
        }


        private void DragAndMove(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }
    }
}
