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
    /// Item1.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class Item1 : Page
    {
        public Item1()
        {
            InitializeComponent();
            DataContext = App.Current.Services.GetService(typeof(Item1ViewModel));
            Global.gItem1 = this;
        }
        private void TextBox_PreviewTextInput1(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9.]+");
            e.Handled = regex.IsMatch(e.Text);
        }
    }
}
