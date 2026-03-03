using System;
using System.Collections.Generic;
using System.Data;
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

namespace StarIDE.View
{
    /// <summary>
    /// BIBView.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class BIBView : Window
    {
        public BIBView()
        {
            InitializeComponent();
            DataContext = Global.gAssistVM;
            xDG_BIB.PreviewKeyDown += XDG_BIB_KeyDown;
            Global.gAssistVM._xDataGrid = xDG_BIB;
        }

        private void XDG_BIB_KeyDown(object sender, KeyEventArgs e)
        {
            bool IsShiftKey = (Keyboard.Modifiers & ModifierKeys.Shift) != 0;
            bool IsControlKey = (Keyboard.Modifiers & ModifierKeys.Control) != 0;

            if (e.Key == Key.Delete) // Save All
            {
                Global.gAssistVM.doBibButton("DEL");                
            }else if (IsControlKey &&  e.Key == Key.C)
            {
                Global.gAssistVM.doBibButton("COPY");
            }
            else if (IsControlKey && e.Key == Key.V)
            {
                Global.gAssistVM.doBibButton("PASTE");
            }
        }
        private void xDG_BIB_LoadingRow(object sender, DataGridRowEventArgs e)
        {
            TextBox tb = new TextBox();            
            tb.Text = "R" + (Global.gAssistVM.Row - e.Row.GetIndex()).ToString();
            tb.Background = new SolidColorBrush(Colors.Black);
            tb.IsReadOnly = true;
            tb.Width = 37;
            e.Row.Header = tb;            
        }

        private void MoveTitle(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
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
                Hide();
            }
            

        }

        private void xDG_BIB_SelectedCellsChanged(object sender, SelectedCellsChangedEventArgs e)
        {
            Global.gAssistVM._xCopyProc = 1;
            Global.gAssistVM._yCopyProc = 1;
        }
    }
}
