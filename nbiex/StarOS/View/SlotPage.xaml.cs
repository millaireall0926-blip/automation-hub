using StarOS.Model;
using System;
using System.Collections;
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
using System.Windows.Threading;

namespace StarOS.View
{
    /// <summary>
    /// SlotPage.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class SlotPage : Page
    {
        public SlotPage()
        {
            InitializeComponent();
        }

        private void SlotGrid_MouseUp(object sender, MouseButtonEventArgs e)
        {
            Global.gSlotVM.SelectSlots.Clear();
            IList items = (e.Source as DataGrid).SelectedItems;
            foreach (object item in items)
            {
                Global.gSlotVM.SelectSlots.Add((SlotModel)item);
            }

            if (Global.gSlotVM.SelectSlots.Count == 0) return;
                
            Global.gSlotVM.CurSlot = Global.gSlotVM.SelectSlots[0];
            
            Global.gPowerVM.setSlot(Global.gSlotVM.CurSlot);            

            Global.gMainVM.updateUIFlag((int)UI.MENU | (int)UI.INFO | (int)UI.SLOTLOG 
                | (int)UI.STEPLIST | (int)UI.POWER | (int)UI.RESULT | (int)UI.REGISTER);


            if (Global.gSlotVM.CurSlot._plcModel != Global.gChamberVM.CurPLC)
            {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() =>
                    {
                        Global.gChamberVM.CurPLC = Global.gSlotVM.CurSlot._plcModel;
                        Global.gChamberVM.setTempData();
                    }));
            }

        }

        private void SlotGrid_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            Global.gMainVM.ResultWin.Left = Global.gMainWin.Left + 400;
            Global.gMainVM.ResultWin.Top =  Global.gMainWin.Top + 200;

            
            //Global.gMainVM.ResultWin.Visibility = Visibility.Visible;
            //Global.gMainVM.ResultWin.WindowState = WindowState.Normal;
            Global.gMainVM.ResultWin.Topmost = true;
            //Global.gMainVM.ResultWin.WindowStartupLocation = WindowStartupLocation.CenterOwner;
            Global.gMainVM.ResultWin.Show();
            Global.gMainVM.ResultWin.Topmost = false;
        }
    }
}
