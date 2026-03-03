using MVVM_Loader.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
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

namespace MVVM_Loader.View
{
    /// <summary>
    /// Item3.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class Item3 : Page
    {
        public Item3()
        {
            InitializeComponent();
            DataContext = App.Current.Services.GetService(typeof(Item3ViewModel));
            Global.gItem3 = this;
        }
        private void TextBox_PreviewTextInput3(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9.]+");
            e.Handled = regex.IsMatch(e.Text);
        }
    }
}
