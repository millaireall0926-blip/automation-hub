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

namespace StarQMOS.View
{
    /// <summary>
    /// StepSelect.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class StepSelect : Window
    {
        public string _selects = "";
        public StepSelect()
        {
            InitializeComponent();
            this.DataContext = Global.gMainVM;
        }



        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Button bt =  e.Source as Button;
            if (bt.Content as string == "Run Step")
            {
                if (xCombo.SelectedItems.Count == 0) return;
                _selects = "";
                foreach (var item in xCombo.SelectedItems)
                {
                    _selects += item.ToString();
                    if (xCombo.SelectedItems[xCombo.SelectedItems.Count - 1] != item)
                        _selects += ",";
                }
                this.DialogResult = true;
            }else if (bt.Content as string == "Cancel")
            {
                this.DialogResult = false;
            }
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
                this.DialogResult = true;
            else if (e.Key == Key.Escape)
                this.DialogResult = false;

        }

        private void Rectangle_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
                this.DragMove();
        }
    }
}
