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
    /// Item2.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class Item2 : Page
    {
        public Item2()
        {
            InitializeComponent();
            DataContext = App.Current.Services.GetService(typeof(Item2ViewModel));
            Global.gItem2 = this;
        }

        private void TextBox_PreviewTextInput2(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9.]+");
            e.Handled = regex.IsMatch(e.Text);
        }
    }
}
