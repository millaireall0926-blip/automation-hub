using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

using System.Windows.Input;
using StarIDE.ViewModel;

namespace StarIDE
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            Global.gMainView = this;                      

            foreach (CommandBinding binding in MainVM.Commands)
                CommandBindings.Add(binding);

            this.KeyDown += MainWindow_KeyDown;
            Global.gMainVM.xRecent0 = xMenu_Recent0;
            Global.gMainVM.xRecent1 = xMenu_Recent1;
            Global.gMainVM.xRecent2 = xMenu_Recent2;
            Global.gMainVM.updateRecent();
        }

        private void MainWindow_KeyDown(object sender, KeyEventArgs e)
        {
            bool IsShiftKey = (Keyboard.Modifiers & ModifierKeys.Shift) != 0;
            bool IsControlKey = (Keyboard.Modifiers & ModifierKeys.Control) != 0;
            //Console.WriteLine($"s:{IsShiftKey}  c:{IsControlKey} {e.Key.ToString()}");
            if ((IsShiftKey & IsControlKey) && e.Key == Key.S) // Save All
            {
                Global.gMainVM.excuteMenu("SAVE ALL");
            } else if ((IsControlKey) && e.Key == Key.S)
            {
                Global.gMainVM.excuteMenu("SAVE FILE");
            }            
            else if (IsShiftKey && IsControlKey && e.Key == Key.B)
            {
                Global.gMainVM.excuteMenu("COMPILE ALL"); 
            }
            else if ((IsControlKey) && e.Key == Key.B)
            {
                Global.gMainVM.excuteMenu("COMPILE FILE");
            }

        }

        private void Window_Closed(object sender, EventArgs e)
        {
            Global.gMainVM.excuteMenu("CLOSE PROJECT");
            Global.gMainVM.saveRecent();
            Global.gAssistVM._bibView.Close();
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
                this.Close();
            }
        }

        private void MoveTitle(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }

        private void SideMenuItem_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            HandyControl.Controls.SideMenuItem sd = sender as HandyControl.Controls.SideMenuItem;
            if (sd == null) return;
            if (sd.Uid == "Recent")
                Global.gMainVM.openFolder(sd.ToolTip.ToString());
            else
                Global.gMainVM.excuteMenu(sd.Header.ToString());
        }
    }
}


