using RDimm_MMTCRACK.Model;
using RDimm_MMTCRACK.Net;
using RDimm_MMTCRACK.ViewModel;
using Serilog;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RDimm_MMTCRACK
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        //USB 연결, 해제 감지
        IntPtr WndProc(IntPtr hWnd, int nMsg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            UInt32 WM_DEVICE_CHANGE = 0x219;
            UInt32 DBT_DEVT_PORT = 0x03;
            UInt32 DBT_DEVICE_ARRIVAL = 0x8000;
            UInt32 DBT_DEVICE_REMOVE = 0x8004;

            if (((nMsg == WM_DEVICE_CHANGE) && (wParam.ToInt32() == DBT_DEVICE_ARRIVAL)) ||
                 ((nMsg == WM_DEVICE_CHANGE) && (wParam.ToInt32() == DBT_DEVICE_REMOVE))
               )
            {
                int devType = Marshal.ReadInt32(lParam, 4);
                if (devType == DBT_DEVT_PORT)
                {
                    string port_name = Marshal.PtrToStringAuto((IntPtr)((long)lParam + 12));
                    
                    //TCON Serial
                    if(Global.gTconCom.TconPortName == port_name)
                    {
                        if (wParam.ToInt32() == DBT_DEVICE_ARRIVAL)
                        {
                            string msg = "[WndProc] TCON Port Insert";
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
                            Global.gTconCom.TConConnect();
                            return IntPtr.Zero;
                        }
                        else
                        {
                            string msg = "[WndProc] TCON Port Remove";
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
                            //Global.gTconCom.TConDisConnet();
                            return IntPtr.Zero;
                        }
                    }
                    
                    int PortNum = 0;
                    //Site Serial
                    foreach (SiteCom scom in Global.gMainPageVM.sc)
                    {
                        if(scom.SitePortName == port_name)
                        {
                            if (wParam.ToInt32() == DBT_DEVICE_ARRIVAL)
                            {
                                string msg = "[WndProc] Site Port " + (PortNum + 1) + " Insert";
                                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
                                scom.SiteConnect(PortNum);
                                return IntPtr.Zero;
                            }
                            else
                            {
                                string msg = "[WndProc] Site Port " + (PortNum + 1) + " Remove";
                                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
                                Global.gMainPageVM.Racks[scom.SiteCh].MotorStatText = string.Empty;// 비우고 다시 받을지
                                Global.gMainPageVM.Racks[scom.SiteCh].MotorStatColor = Global.gMotorStatColors[1];
                                Global.gEngineerVM.ManualMotor[scom.SiteCh].eMotorStatColor = Global.gMotorStatColors[1];
                                //scom.SiteDisConnect();
                                return IntPtr.Zero;
                            }
                        }
                        PortNum++;
                    }
                    PortNum = 0;
                    //Capture Serial
                    foreach(CaptureCom ccom in Global.gMainPageVM.cc)
                    {
                        if(ccom.CaputurePortName == port_name)
                        {
                            if (wParam.ToInt32() == DBT_DEVICE_ARRIVAL)
                            {
                                string msg = "[WndProc] Caputre Port " + (PortNum + 1) + " Insert";
                                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
                                ccom.CaptureConnect(PortNum);
                                return IntPtr.Zero;
                            }
                            else
                            {
                                string msg = "[WndProc] Caputre Port " + (PortNum + 1) + " Remove";
                                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
                                foreach(CBoxModel bm in Global.gMainPageVM.Racks[ccom.CaputureCh].CBoxs)
                                {
                                    bm.BoxPV = 0;
                                    bm.BoxPVColor = Global.gWarningColors[0];
                                    bm.BoxPVText = string.Empty;
                                }
                                //ccom.CaptureDisConnect();
                                return IntPtr.Zero;
                            }
                        }
                        PortNum++;
                    }
                    //Alarm Serial
                    if(Global.gAlarmCom.AlarmPortName == port_name)
                    {
                        if(wParam.ToInt32() == DBT_DEVICE_ARRIVAL)
                        {
                            string msg = "[WndProc] Alarm Port Insert";
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
                            Global.gAlarmCom.AlarmConnect();
                            return IntPtr.Zero;
                        }
                        else
                        {
                            string msg = "[WndProc] Alarm Port Remove";
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
                            //Global.gAlarmCom.AlarmDisConnect();
                            return IntPtr.Zero;
                        }
                    }
                    //Flow Serial
                    if(Global.gFlowMeterCom.FlowMeterName == port_name)
                    {
                        if (wParam.ToInt32() == DBT_DEVICE_ARRIVAL)
                        {
                            string msg = "[WndProc] Flow Port Insert";
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
                            Global.gFlowMeterCom.FlowMeterConnect();
                            return IntPtr.Zero;
                        }
                        else
                        {
                            string msg = "[WndProc] Flow Port Remove";
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
                            //Global.gFlowMeterCom.FlowMeterDiConnect();
                            return IntPtr.Zero;
                        }
                    }
                }
            }
            return IntPtr.Zero;
        }
        public MainWindow()
        {
            InitializeComponent();
            xTB_Title.Text = VerInfo.getVer();
            Global.gMainWindow = this;
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            WindowInteropHelper helper = new WindowInteropHelper(this);
            HwndSource source = HwndSource.FromHwnd(helper.Handle);
            source.AddHook(new HwndSourceHook(this.WndProc));
        }
        private void Window_Closed(object sender, EventArgs e)
        {
            Global.gMainVM.quit();
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

                MessageBoxResult res = HandyControl.Controls.MessageBox.Show($"Do You Want to Close System", "Quit", MessageBoxButton.YesNo, MessageBoxImage.Warning);
                if (res == MessageBoxResult.No) return;


                this.Close();
            }
        }

        private void MoveTitle(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
        }

        private void SideMenuItem_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            Storyboard sb = (Storyboard)FindResource("storyViewChange");
            sb.Begin();

            HandyControl.Controls.SideMenuItem sd = sender as HandyControl.Controls.SideMenuItem;
            if (sd == null) return;

            Global.gMainVM.changeView(sd.Uid);
        }
    }
}
