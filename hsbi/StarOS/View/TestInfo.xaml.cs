using StarOS.Model;
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
using static System.Net.WebRequestMethods;

namespace StarOS.View
{
    /// <summary>
    /// TestInfo.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class TestInfo : Page
    {
        public TestInfo()
        {
            InitializeComponent();

            Global.gTestVM.SCOT_Yield = xSCOT_Yield;
            Global.gTestVM.initItems();
            Global.gTestVM.ListSlotLog = xList_SlotLog;
            Global.gTestVM.ListSysLog = xList_SysLog;
        }

        public void scrollList(int type)
        {
            if (type == 0)
                xList_SysLog.ScrollIntoView(xList_SysLog.Items.Count - 1);
            else
                xList_SlotLog.ScrollIntoView(xList_SlotLog.Items.Count - 1);
        }

        private void xList_SysLog_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            var item = (sender as ListView).SelectedItem;
            if (item != null)
            {
                SystemLog sl = item as SystemLog;                
                if (sl != null)
                {
                    if (sl.Message.IndexOf("[DIAG]") != -1)
                    {
                        SlotModel sm =  Global.gSlotVM.getSlot(sl.Uint);
                        if (sm == null) return;
                        if (sm._diagPath == "") return;
                        if (System.IO.File.Exists(sm._diagPath))
                        {
                            System.Diagnostics.Process.Start("Notepad.exe", sm._diagPath);
                        }
                        else
                        {
                            MessageBox.Show(
                                 $"Not Found DiagLog {sm._diagPath}. Wait File Recv", "Waiting File Recv"
                                , MessageBoxButton.YesNo, MessageBoxImage.Question);
                        }
                    } 
                
                }
                
            }
        }

        

        private void Button_Click_CloseInfo(object sender, RoutedEventArgs e)
        {
            Global.gTestVM.ShowReqInfo = Visibility.Hidden;
        }
    }
}
