using HTCRack.Model;
using HTCRack.Net;
using HTCRack.ViewModel;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
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

namespace HTCRack.View
{
    /// <summary>
    /// EngineerPage.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class EngineerPage : Page
    {
        public EngineerPage()
        {
            InitializeComponent();

            Global.gEnginerVM.ListCmdLog = xList_Msg;
            Rect initRect = Global.gRackMagins[0];
            if (Global.gMainPageVM.LayoutColumnCount == 1)
            {
                initRect = Global.gRackMaginsCol1[0];
            }

            xRect_Select.Margin = new Thickness(initRect.X,
                                                initRect.Y,
                                                initRect.Width,
                                                initRect.Height);
        }

        private void Image_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point pos = e.GetPosition(sender as Image);
            Console.WriteLine($" x {pos.X} , y {pos.Y}");

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
                    if (Global.gEnginerVM.CurRack != index)
                    {
                        Global.gEnginerVM.CurRack = index;
                        Global.gEnginerVM.setRack();
                    }
                    break;
                }
                index++;
            }

        }

        public void changeRWVisible(ObservableCollection<RegModel> regs, Visibility isHex, Visibility isValue)
        {
            foreach (RegModel reg in regs)
            {
                if (!reg.Single) continue;
                if (reg.Kind == "0x") continue;
                reg.VisibleHex = isHex;
                reg.VisibleValue = isValue;
            }
        }

        private void Mode_Button_Click(object sender, RoutedEventArgs e)
        {
            Visibility isHex;
            Visibility isValue;
            if (Global.gEnginerVM.OutputMode == "Hex") {
                Global.gEnginerVM.OutputMode = "Value";
                isValue = Visibility.Visible;
                isHex = Visibility.Hidden;
            }
            else {
                Global.gEnginerVM.OutputMode = "Hex";
                isValue = Visibility.Hidden;
                isHex = Visibility.Visible;
            }

            EngineerVM vm = Global.gEnginerVM;
            changeRWVisible(vm.FanRegModels, isHex, isValue);
            changeRWVisible(vm.CurRegModels, isHex, isValue);
            changeRWVisible(vm.TempRegModels, isHex, isValue);
            changeRWVisible(vm.FPRegModels, isHex, isValue);
            changeRWVisible(vm.DimRegModels, isHex, isValue);
            changeRWVisible(vm.TConRegModels, isHex, isValue);
            changeRWVisible(vm.FanRegModels, isHex, isValue);

        }

        private void xTestBT_Click(object sender, RoutedEventArgs e)
        {
            
            int index = Global.gEnginerVM.CurRack;
            Global.gMainPageVM.Racks[index].reqFanDuty();
            Global.gMainPageVM.Racks[index].reqTConBS1();
        }

        private void Expand_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var tc = sender as TabControl;
            if (tc != null)
            {
                TabItem tab = tc.SelectedItem as TabItem;
                string sel = tab.Header.ToString();

                if (Global.gEnginerVM == null) return;
                EngineerVM vm = Global.gEnginerVM;

                vm.ExpandFan = false;
                vm.ExpandCur = false;
                vm.ExpandTemp = false;
                vm.ExpandFP = false;
                vm.ExpandDimm = false;
                vm.ExpandTCon = false;
                vm.ExpandGPIO = false;
                switch (sel)
                {
                    case "FAN":
                        vm.ExpandFan = true; break;
                    case "CUR":
                        vm.ExpandCur = true; break;
                    case "TEMP":
                        vm.ExpandTemp = true; break;
                    case "FP":
                        vm.ExpandFP = true; break;
                    case "DIMM":
                        vm.ExpandDimm = true; break;
                    case "TCON":
                        vm.ExpandTCon = true; break;
                    case "GPIO":
                        vm.ExpandGPIO = true; break;
                }
            }
        }

        
        private void msg_clear_Button_Click(object sender, RoutedEventArgs e)
        {
            Global.gEnginerVM.clearCmdLog();
        }


    }
}
