using HandyControl.Controls;
using HTCRack.Model;
using HTCRack.ViewModel;
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
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace HTCRack.View {
    /// <summary>
    /// ConfigPage.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class ConfigPage : Page {
        public ConfigPage() {
            InitializeComponent();

            Global.gConfigVM.ListTconResp = xList_TCon;

            Rect initRect = Global.gRackMagins[0];
            if (Global.gMainPageVM.LayoutColumnCount == 1)
            {
                initRect = Global.gRackMaginsCol1[0];
            }

            xRect_Select.Margin = new Thickness(initRect.X,
                                                initRect.Y,
                                                initRect.Width,
                                                initRect.Height);

            xRect_SelectTcon.Margin = xRect_Select.Margin;
        }


        private void Image_MouseLeftButtonDownTcon(object sender, MouseButtonEventArgs e)
        {
            Point pos = e.GetPosition(sender as Image);
            int index = 0;

            Rect[] rects = Global.gRackRects;
            Rect[] margins = Global.gRackMagins;
            if (Global.gMainPageVM.LayoutColumnCount == 1)
            {
                rects = Global.gRackRectsCol1;
                margins = Global.gRackMaginsCol1;
            }

            foreach (Rect rect in rects)
            {
                if (rect.Contains(pos))
                {
                    if (index >= Global.gMainPageVM.Racks.Count) return;
                    xRect_Select.Margin = new Thickness(margins[index].X,
                                                        margins[index].Y,
                                                        margins[index].Width,
                                                        margins[index].Height);
                    xRect_SelectTcon.Margin = xRect_Select.Margin;
                    Global.gConfigVM.CurRack = index;
                    Global.gConfigVM.clearTconResp();

                    break;
                }
                index++;
            }

        }


        private void Image_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point pos = e.GetPosition(sender as Image);
            Console.WriteLine($" x {pos.X} , y {pos.Y}");

            if (Global.gConfigVM.SendFlag != FPGA_FLAG.DONE )           
                if (System.Windows.MessageBox.Show("Now It is Sending File. Cancel?","Upload Cancel", MessageBoxButton.YesNo, MessageBoxImage.Question) == MessageBoxResult.No)                
                    return;
                
            

            int index = 0;

            Rect[] rects = Global.gRackRects;
            Rect[] margins = Global.gRackMagins;
            if (Global.gMainPageVM.LayoutColumnCount == 1)
            {
                rects = Global.gRackRectsCol1;
                margins = Global.gRackMaginsCol1;
            }

            foreach (Rect rect in rects)
            {
                if (rect.Contains(pos))
                {
                    if (index >= Global.gMainPageVM.Racks.Count) return;
                    xRect_Select.Margin = new Thickness(margins[index].X,
                                                        margins[index].Y,
                                                        margins[index].Width,
                                                        margins[index].Height);
                    xRect_SelectTcon.Margin = xRect_Select.Margin;
                    Global.gConfigVM.CurRack = index;
                    Global.gConfigVM.FpgaVer = "";
                  
                    
                    RackModel rack = Global.gMainPageVM.getRack(Global.gConfigVM.CurRack);
                    if (rack._client._isConnect)
                    {
                        var msg = Global.newMsg(0xff, 0, 0x10, 32);
                        rack._client.pushSendQue(REG_TYPE.PDO, msg);
                    }

                    break;
                }
                index++;
            }

        }

        private void HexOnlyInput(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9a-fA-F]+");
            e.Handled = regex.IsMatch(e.Text);
        }

        private void TabControl_Selected(object sender, SelectionChangedEventArgs e)
        {
            var tc = sender as HandyControl.Controls.TabControl;
            if (tc != null)
            {
                var tab = tc.SelectedItem as System.Windows.Controls.TabItem;
                string sel = tab.Header.ToString();

                if (sel == "Firmware")
                {
                    RackModel rack = Global.gMainPageVM.getRack(Global.gConfigVM.CurRack);
                    if (rack._client._isConnect)
                    {
                        var msg = Global.newMsg(0xff, 0, 0x10, 32);
                        rack._client.pushSendQue(REG_TYPE.PDO, msg);
                    }
                }
            }
        }

        private void ControlMode_Selected(object sender, SelectionChangedEventArgs e)
        {
            var cb = sender as System.Windows.Controls.ComboBox;
            int prevSel = Global.gConfigVM.ControlMode;
            int sel = cb.SelectedIndex;
            

            CONTROL_MODE mode = (CONTROL_MODE)sel;
            if (mode == CONTROL_MODE.ML)
            {
                Global.gMainVM.IpcRecver.start();
                Global.gMainVM.ShowIPCIcon = Visibility.Visible;
                
            }else 
            {
                Global.gMainVM.IpcRecver.stop();
                Global.gMainVM.ShowIPCIcon = Visibility.Hidden;                
            }

            if (mode == CONTROL_MODE.MANUAL)
                Global.gDimmVM.ShowManualContol = Visibility.Visible;
            else
                Global.gDimmVM.ShowManualContol = Visibility.Hidden;


        }

        private void Collection_Selected(object sender, SelectionChangedEventArgs e)
        {
            var cb = sender as System.Windows.Controls.ComboBox;
            int prevSel = Global.gConfigVM.ControlMode;
            int sel = cb.SelectedIndex;
            if (sel == 0) return;

            DIM_TYPE type = (DIM_TYPE)(sel -1);
            Global.gConfigVM.changeTempCollect(type);



        }
    }
}
