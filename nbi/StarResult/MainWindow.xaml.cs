
using StarResult.View;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml;

namespace StarResult
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

            xTB_Title.Text = VerInfo.getVer();
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


        
        bool isDrag = false;
        Grid _dragItem = null;
        Image _dragImage = null;
        double _prevX;
        double _prevY;

        private void MoveStart_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Image img = e.Source as Image;

            switch (img.Name)
            {
                case "xImg_PWR":
                    _dragItem = xGrid_PWR; _dragImage = xImg_PWR;
                    break;
                case "xImg_Temp":
                    _dragItem = xGrid_Temp; _dragImage = xImg_Temp;
                    break;
                case "xImg_Info":
                    _dragItem = xGrid_Info; _dragImage = xImg_Info;
                    break;
                case "xImg_Log":
                    _dragItem = xGrid_Log; _dragImage = xImg_Log;
                    break;
                case "xImg_Step":
                    _dragItem = xGrid_Step; _dragImage = xImg_Step;
                    break;
                case "xImg_BIB":
                    _dragItem = xGrid_BIB; _dragImage = xImg_BIB;
                    break;
                case "xImg_Detail":
                    _dragItem = xGrid_Detail; _dragImage = xImg_Detail;
                    break;
            }

            if (_dragItem == null) return;

            isDrag = true;
            _prevX = Mouse.GetPosition(this).X;
            _prevY = Mouse.GetPosition(this).Y;

        }

        private void Image_MouseLeave(object sender, MouseEventArgs e)
        {
            this.Cursor = Cursors.Arrow;
        }

        private void Image_MouseEnter(object sender, MouseEventArgs e)
        {
            this.Cursor = Cursors.SizeNWSE;
        }

        private void MoveEnd_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            this.Cursor = Cursors.Arrow;
            isDrag = false;
        }

        private void Moving_MouseMove(object sender, MouseEventArgs e)
        {
            if (!isDrag) return;
            object o= this.Resources[_dragItem]; 
            if (_dragItem == null) return;
            
            double x = Mouse.GetPosition(this).X;
            double y = Mouse.GetPosition(this).Y;

            if ((_dragItem.Width + (x - _prevX)) <= 0 || _dragItem.Height + (y - _prevY) <= 0)
            {
                _dragItem.Width = 0; 
                _dragItem.Height = 0;
                this.Cursor = Cursors.Arrow;
                isDrag = false;
                return;
            }

            _dragItem.Width += x - _prevX;
            _dragItem.Height += y - _prevY;
            _dragImage.Margin = new Thickness(_dragItem.Width - 20, _dragItem.Height - 20, 0, 0);

            _prevX = x;
            _prevY = y;            
        }

        private void Show_Button_Click(object sender, RoutedEventArgs e)
        {
            Button bt = sender as Button;
            if (bt == null) return;
            TextBlock tb = bt.Content as TextBlock;
            if (tb == null) return;            

            switch (tb.Text)
            {
                case "PWR":
                    _dragItem = xGrid_PWR; _dragImage = xImg_PWR;
                    break;
                case "TEMP":
                    _dragItem = xGrid_Temp; _dragImage = xImg_Temp;
                    break;
                case "INFO":
                    _dragItem = xGrid_Info; _dragImage = xImg_Info;
                    break;
                case "LOG":
                    _dragItem = xGrid_Log; _dragImage = xImg_Log;
                    break;
                case "STEP":
                    _dragItem = xGrid_Step; _dragImage = xImg_Step;
                    break;
                case "BIB":
                    _dragItem = xGrid_BIB; _dragImage = xImg_BIB;
                    break;
                case "DETAIL":
                    _dragItem = xGrid_Detail; _dragImage = xImg_Detail;
                    break;
            }

            if (_dragItem == null) return;

            if (_dragItem.Width == 0)
            {
                _dragItem.Width = 400;
                _dragItem.Height = 300;
                _dragImage.Margin = new Thickness(_dragItem.Width - 20, _dragItem.Height - 20, 0, 0);

            }
            else
            {
                _dragItem.Width = 0;
                _dragItem.Height = 0;                
            }

        }

        private void ShowAll_Button_Click(object sender, RoutedEventArgs e)
        {
            Button bt = sender as Button;
            if (bt == null) return;
            TextBlock tb = bt.Content as TextBlock;
            if (tb == null) return;

            double defWidth = 400;
            double defHeight = 300;

            if (tb.Text == "CLEAR")
            {
                defWidth = 0;
                defHeight = 0;
            }

            xGrid_PWR.Width = defWidth; 
            xGrid_Temp.Width = defWidth;
            xGrid_Info.Width = defWidth;
            xGrid_Log.Width = defWidth;
            xGrid_Step.Width = defWidth;
            xGrid_BIB.Width = defWidth;
            xGrid_Detail.Width = defWidth;

            xGrid_PWR.Height = defHeight;
            xGrid_Temp.Height = defHeight;
            xGrid_Info.Height = defHeight;
            xGrid_Log.Height = defHeight;
            xGrid_Step.Height = defHeight;
            xGrid_BIB.Height = defHeight;
            xGrid_Detail.Height = defHeight;

            if (defWidth != 0)
            {
                xImg_PWR.Margin = new Thickness(defWidth - 20, defHeight - 20, 0, 0); ;
                xImg_Temp.Margin = new Thickness(defWidth - 20, defHeight - 20, 0, 0); ;
                xImg_Info.Margin = new Thickness(defWidth - 20, defHeight - 20, 0, 0); ;
                xImg_Log.Margin = new Thickness(defWidth - 20, defHeight - 20, 0, 0); ;
                xImg_Step.Margin = new Thickness(defWidth - 20, defHeight - 20, 0, 0); ;
                xImg_BIB.Margin = new Thickness(defWidth - 20, defHeight - 20, 0, 0); ;
            }

         }

        

        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            Global.gBIBVM.MultiChipWin.Close();
        }

        public void getMousePos(out double x, out double y)
        {
            x = Mouse.GetPosition(this).X;
            y = Mouse.GetPosition(this).Y;
        }

        private void CloseWarp_Click(object sender, RoutedEventArgs e)
        {
            Button bt = e.Source as Button;
            if (bt == null) return;
            Grid btGrid = bt.Parent as Grid;
            if (btGrid == null) return;
            Grid warpGrid = btGrid.Parent as Grid;
            if (warpGrid == null) return;   
            warpGrid.Height = 0; warpGrid.Width = 0;

        }

        private void DataPath_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.FolderBrowserDialog fdb = new System.Windows.Forms.FolderBrowserDialog();
            if (fdb.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                Global.gMainVM.DataPath = fdb.SelectedPath;
                Global.saveConfig();
            }
        }

        private void Load_Save_Click(object sender, RoutedEventArgs e)
        {
            Button bt = e.Source as Button;
            if (bt.Name == "xbt_LoadL")
                Global.gMainVM.LayoutMode = true;
            else
                Global.gMainVM.LayoutMode = false;

            if (Global.gMainVM.LayoutMode)
                Global.loadLayout(Global.gMainVM.LayoutNum);
            else 
                Global.saveLayout(Global.gMainVM.LayoutNum);
        }

        private void LayoutNum_Click(object sender, RoutedEventArgs e)
        {
            Button bt = e.Source as Button;
            switch (bt.Name)
            {
                case "xbt_Layout1":
                    Global.gMainVM.LayoutNum = 0;
                    break;
                case "xbt_Layout2":
                    Global.gMainVM.LayoutNum = 1;
                    break;
                case "xbt_Layout3":
                    Global.gMainVM.LayoutNum = 2;
                    break;
                case "xbt_Layout4":
                    Global.gMainVM.LayoutNum = 3;
                    break;
            }
        }
    }
}
 