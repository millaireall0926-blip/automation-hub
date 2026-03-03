using HandyControl.Controls;
using StarQMOS.Model;
using StarQMOS.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
using System.Windows.Threading;

namespace StarQMOS
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : System.Windows.Window
    {
        public MainWindow()
        {
            InitializeComponent();
            Global.gMainWin = this;
            //this.Title = VerInfo.getVer();
            

            Global.gMainVM.xSteps = xSteps;
            Global.gMainVM.initItems();

            xTB_Title.Text = VerInfo.getVer();

            foreach (CommandBinding binding in MainVM.Commands)
            {
                CommandBindings.Add(binding);

            }
        }

        public void updateProperty()
        {
            CommandManager.InvalidateRequerySuggested();
            this.Dispatcher.Invoke(
            (ThreadStart)(() => { }), DispatcherPriority.ApplicationIdle);
        }

        public void updateMenu()
        {
            int st = 0;
            foreach (SlotModel sm in Global.gSlotVM.SelectSlots)
                st |= 1 << (int)sm.StatType;
            
            Global.gMainVM.enableMenuButton(xBT_Load, st);
            Global.gMainVM.enableMenuButton(xBT_ReLoad, st);
            Global.gMainVM.enableMenuButton(xBT_Con, st);
            Global.gMainVM.enableMenuButton(xBT_Test, st);
            Global.gMainVM.enableMenuButton(xBT_Stop, st);
            Global.gMainVM.enableMenuButton(xBT_Step, st);
            Global.gMainVM.enableMenuButton(xBT_Reset, st);

        }


        private void MoveTitle(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }


        private void ClickMenuButton(object sender, RoutedEventArgs e)
        {
            Global.gMainVM.excute_cmdMain(e.Source as Button);
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

        private void Window_Closed(object sender, EventArgs e)
        {
            Global.gNet.quit();            

            Global.gChamberVM._plcThreadClose = true; 
            Global.gMainVM._uiThreadClose = true;

            Global.gMainVM.LotWin.Close();
            Global.gMainVM.ResultWin.Close();
            Global.gChamberVM.ChamCtrlWin.Close();
            Global.gResultVM.MultiChipWin.Close();



        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Global.gNet = new Net();
        }
    }

    
    

}
