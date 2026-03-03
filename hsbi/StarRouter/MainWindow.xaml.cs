using StarRouter.ViewModel;
using System;
using System.Collections.Generic;
using System.IO;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using WinForms = System.Windows.Forms;
using StarRouter.View;
using System.Collections;
using StarRouter.Comm;
using System.Windows.Controls.Primitives;
using System.Diagnostics;

namespace StarRouter {
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window {
        
        MainVM _vm;
        Config _config;
        WinForms.NotifyIcon _noti = new WinForms.NotifyIcon();


        public MainWindow() {
            InitializeComponent();
            // 최소 Service 개체 생성

            _config = App.Current.Services.GetService(typeof(Config)) as Config;
            _config.loadDoc();

            _vm = App.Current.Services.GetService(typeof(MainVM)) as MainVM;
            _vm.initServer();
            _vm.initTimers();
            _vm.setLogger(_config.LoggerPath);

            this.DataContext = _vm;

            xTB_Title.Text = "StarRouter" + "  " + _config.Version;


            // Notify ICON
            var uri = new Uri(@"/Resource/StarRouter.ico", UriKind.RelativeOrAbsolute);
            var iconStream = System.Windows.Application.GetResourceStream(uri).Stream;            
            _noti.Icon = new System.Drawing.Icon(iconStream);            
            _noti.Text = "StarRouter";

            _noti.DoubleClick += (object sender, EventArgs eventArgs) => setTrayStat(false) ;
            
            // Notify Menu
            System.Windows.Forms.ContextMenu menu = new System.Windows.Forms.ContextMenu(); 
            
            System.Windows.Forms.MenuItem item1 = new System.Windows.Forms.MenuItem();    // Menu 객체에 들어갈 각각의 menu
            item1.Index = 0;
            item1.Text = "Show";    // menu 이름
            item1.Click +=  (object click, EventArgs eClick) => setTrayStat(false);

            menu.MenuItems.Add(item1);
            _noti.ContextMenu = menu;
        }

        void setTrayStat(bool isTray) {
            if (isTray) {
                this.Hide();
                _noti.Visible = true;
            }else {
                this.Show();
                this.WindowState = WindowState.Normal;
                _noti.Visible = false;
            }
        }

        private void ClickTileButton(object sender, RoutedEventArgs e) {
            Button bt = e.Source as Button;
            if (bt == null) return;
            if (bt.Name == "xBTMin") {
                setTrayStat(true);

            }
            else if (bt.Name == "xBTRestore") {
                this.WindowState = (this.WindowState == WindowState.Normal) ? WindowState.Maximized : WindowState.Normal;
            }
            else if (bt.Name == "xBTClose") {
                MessageBoxResult ret = HandyControl.Controls.MessageBox.Show("Do You Want Fisnish Router?", "Quit", MessageBoxButton.YesNo, MessageBoxImage.Asterisk);
                if (ret == MessageBoxResult.Yes)
                    this.Close();
            }
        }

        private void Window_Closed(object sender, EventArgs e) {
            _noti.Dispose();
                
        }

        private void MoveTitle(object sender, MouseButtonEventArgs e) {
            this.DragMove();
        }

        private void SlotGrid_MouseDoubleClick(object sender, MouseButtonEventArgs e) {
            StarCM item = (e.Source as DataGrid).SelectedItem as StarCM;
            _vm.EditName = item.Name;
            _vm.EditIP = item.IP;
            _vm.EditPort = item.Port;
            Edit edit = new Edit();
            edit.ShowDialog();
            if (edit.DialogResult == true) {
                item.modifyItem(_vm.EditName, _vm.EditIP, _vm.EditPort);
            }
            
        }

        private void SlotGrid_MouseUp(object sender, MouseButtonEventArgs e) {

        }

        private void ToggleButton_Click(object sender, RoutedEventArgs e) {
            ToggleButton bt = sender as ToggleButton;

            if (bt.IsEnabled) _vm.LogLevel = LOG_TYPE.DEBUG;
            else _vm.LogLevel = LOG_TYPE.ERROR;
        }

        private void Log_Button_Click(object sender, RoutedEventArgs e) {
            Button btn = sender as Button;
            DateTime date = DateTime.Now;
            string name = "RouterDebug_";
            if (btn.Name == "xbt_Error")
                name = "RouterError_";
            string path = _config.LoggerPath + @"\" + name;
            

            string file = path + date.Year.ToString() 
                            + date.Month.ToString().PadLeft(2, '0') + ".log";
            try {
                using (FileStream fs = File.Open(file, FileMode.Open)) {
                    Process.Start("notepad.exe", file);
                }
            } catch {
                
            }
        }
    }
}
