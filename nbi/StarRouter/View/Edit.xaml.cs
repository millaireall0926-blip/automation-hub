using StarRouter.ViewModel;
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

namespace StarRouter.View {
    /// <summary>
    /// Edit.xaml에 대한 상호 작용 논리
    /// </summary>
    /// 
    public partial class Edit : Window {
        MainVM _vm;
        public Edit() {
            InitializeComponent();
            _vm = App.Current.Services.GetService(typeof(MainVM)) as MainVM;
            this.DataContext = _vm;

        }

        private void Button_Click(object sender, RoutedEventArgs e) {
            Button bt = sender as Button;            
            if  (bt.Content as string == "OK") {
                DialogResult = true;
            }else 
                DialogResult = false;


        }

        private void Image_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.ChangedButton == MouseButton.Left)
                this.DragMove();
        }

        private void Window_KeyDown(object sender, KeyEventArgs e) {
            if (e.Key == Key.Return)
                this.DialogResult = true;
            else if (e.Key == Key.Escape)
                this.DialogResult = false;
        }
    }
}
