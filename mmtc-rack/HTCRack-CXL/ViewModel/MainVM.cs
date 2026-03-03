using HTCRack_CXL.Base;
using HTCRack_CXL.IPC;
using HTCRack_CXL.Model;
using HTCRack_CXL.Net;
using HTCRack_CXL.View;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Windows.Threading;

namespace HTCRack_CXL.ViewModel
{
    enum VIEW_SEL : int
    {
        MAIN = 0,
        DIMM,
        ENGINEER,
        CONFIG,
        SETUP
    }
    public class MainVM :BaseVM
    {
        public string SelectViewName { get; set; }
        public string SelectViewDiscription { get; set; }
        public string SelectName { get; set; }
        public Visibility ShowMain { get; set; }
        public Visibility ShowDimm { get; set; }
        public Visibility ShowFan { get; set; }
        public Visibility ShowPower { get; set; }
        public Visibility ShowEngineer { get; set; }
        public Visibility ShowConfig { get; set; }
        public Visibility ShowBoard { get; set; }

        /////////////
        /* IPC 용 */
        /////////////
        public Visibility ShowIPCIcon { get; set; } = Visibility.Hidden;
        BitmapImage ConnectIcon { get; set; } = new BitmapImage(new Uri(@"/Resource/connect.png", UriKind.RelativeOrAbsolute));
        BitmapImage DisconnectIcon { get; set; } = new BitmapImage(new Uri(@"/Resource/disconnect.png", UriKind.RelativeOrAbsolute));
        public BitmapImage IPCStatIcon { get; set; }

        [DoNotNotify]
        public IPCClient IpcRecver { get; set; } = new IPCClient();

        /////////// 
        ///
        public string LogoPath { get; set; }

        [DoNotNotify]
        public ModbusLS PlcCtl { get; set; } = new ModbusLS();

        public Visibility IsUseSetup { get; set; } = Visibility.Hidden;

        public MainVM()
        {
            Global.gMainVM = this;

            changeView("MAIN");
            _conThread.Start(this);
            _netThread.Start(this);

            IPCStatIcon = DisconnectIcon;
            IpcRecver.notiCon += setConnectIcon;
            IpcRecver.notiRecv += recvIPCData;
        }
        public void changeView(string name)
        {
            SelectName = name;
            ShowMain = Visibility.Hidden;
            ShowDimm = Visibility.Hidden;
            ShowFan = Visibility.Hidden;
            ShowPower = Visibility.Hidden;
            ShowEngineer = Visibility.Hidden;
            ShowConfig = Visibility.Hidden;
            ShowBoard = Visibility.Hidden;


            if (name == VIEW_SEL.MAIN.ToString())
            {
                SelectViewName = "Main Control";
                SelectViewDiscription = "Operate The Chamber And Perform Test Functions";
                ShowMain = Visibility.Visible;
            }
            else if (name == VIEW_SEL.DIMM.ToString())
            {
                SelectViewName = "Dimm Temperature";
                SelectViewDiscription = "Output Real-Time DIMM Temperature And Graph";
                ShowDimm = Visibility.Visible;

                Global.gTempVM.SetBox();
            }
            else if (name == VIEW_SEL.ENGINEER.ToString())
            {

                bool show = doPasswordDialog();
                if (show)
                {
                    SelectViewName = "Engineer Mode";
                    SelectViewDiscription = "Set up Register Manunaly And Excute Command";
                    ShowEngineer = Visibility.Visible;
                    
                    Global.gEngineerVM.setRack();
                }
            }
            else if (name == VIEW_SEL.CONFIG.ToString())
            {
                bool show = doPasswordDialog();
                if (show)
                {
                    SelectViewName = "Program Configration";
                    SelectViewDiscription = "Check and change the settings required for the Prgram";
                    ShowConfig = Visibility.Visible;
                }
            }
            else if (name == VIEW_SEL.SETUP.ToString())
            {
                bool show = doPasswordDialog();
                if (show)
                {
                    SelectViewName = "Slot Board Setup";
                    SelectViewDiscription = "Board inspection and operational verification";
                    ShowBoard = Visibility.Visible;

                    Global.gBoardVM.setRack(false);
                }
            }
        }
        bool doPasswordDialog()
        {
            if (!Global.gConfigVM.IsUsePassword) return true;
            Global.gConfigVM.GetPassword = "";

            SelectViewName = "Password Page";
            SelectViewDiscription = "You'll need to enter a password to open this page";


            PasswordWindow pwDialog = new PasswordWindow();

            pwDialog.WindowStartupLocation = WindowStartupLocation.CenterOwner;
            pwDialog.ShowDialog();
            if (pwDialog.DialogResult.Value && pwDialog.DialogResult.HasValue)
                return true;
            
            return false;
        }
        public void setConnectIcon(bool isCon)
        {
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
            {
                if (isCon)
                    IPCStatIcon = ConnectIcon;
                else
                    IPCStatIcon = DisconnectIcon;
            }));
        }
        public void recvIPCData(int rack, int box, float[] sv)
        {
            var rm = Global.gMainPageVM.getRack(rack);
            if (rm == null) return;
            if (box >= rm.Boxs.Count) return;
            var bm = rm.Boxs[box];

            //todo
            /*
            foreach (SiteModel sm in bm.Sites)
            {
                sm.TargetSV = sv[0];
                bm.sendSV(sm, sm.TargetSV);
            }
            */
        }
        public void sendIPCData(IPCMessage msg)
        {
            IpcRecver.pushSendQue(msg);
        }
        public void quit()
        {
            _isCloseNet = true;
         
            if(_netThread.IsAlive)
                _netThread.Join();
            if(_conThread.IsAlive)
                _conThread.Join();
            if (_plcThread.IsAlive)
                _plcThread.Join();

            //DB 사용 안함
            /*
            if (_dbThread.IsAlive)
                _dbThread.Join();
            */

            foreach (RackModel rm in Global.gMainPageVM.Racks)
                rm._client.quit();

            IpcRecver.stop();

        }

        public bool _isCloseNet = false;

        public int _connectRetryProg = 0;
        public int _connectRetry = 5;
        public static Thread _conThread = new Thread(ConnectFunction);
        public static Thread _netThread = new Thread(NetworkFunction);
        //public static Thread _dbThread = new Thread(DBFunction);
        public static Thread _plcThread = new Thread(PlcFunction);

        static public void ConnectFunction(object parent)
        {
            MainVM mv = (MainVM)parent;
            while (!mv._isCloseNet)
            {
                if (Global.gMainPageVM == null)
                { Thread.Sleep(1000); continue; }

                foreach (RackModel rm in Global.gMainPageVM.Racks)
                {
                    if (!rm._client._isConnect)
                        rm._client.doConnect();
                }
                Thread.Sleep(10000);
            }

        }
        static public void NetworkFunction(object parent)
        {
            MainVM mv = (MainVM)parent;


            int ackWaitLimit = 800;

            while (!mv._isCloseNet)
            {
                if (Global.gMainPageVM == null)
                { Thread.Sleep(1000); continue; }


                foreach (RackModel rm in Global.gMainPageVM.Racks)
                {
                    if (!rm._client._isConnect)
                        continue;


                    if (rm._client._ackWaitCount == 0)
                    {
                        if (rm._client.prevPDOTime.Second == DateTime.Now.Second) continue;
                        if (rm._client._isUploading) continue;
                        if (rm._client._isPushing) continue;
                        HtcMessage msg = Global.newMsg(0xff, 0, 0x11, 334);
                        rm._client.pushSendQue(REG_TYPE.PDO, msg);

                        rm._client.prevPDOTime = DateTime.Now;
                    }
                    else
                    {
                        if (rm._client._ackWaitCount >= ackWaitLimit)
                        {
#if TEST_MODE
                            Console.WriteLine("AckWaitTime Out Pdo Force Send!");
#endif
                            rm._client._ackWaitCount = 0;
                            continue;
                        }
                    }
                    rm._client._ackWaitCount++;

                    if (mv.ShowEngineer == Visibility.Visible)
                    {
                        //msg = Global.newMsg(0xff, 0, 0x11, 15);                        
                        //rm._client.pushSendQue(REG_TYPE.FAN, msg);
                    }

                }
                Thread.Sleep(10);
            }

        }
        static public void PlcFunction(object parent)
        {
            MainVM vm = (MainVM)parent;
            vm.PlcCtl._ip = Global.gConfigVM.PlcIP;
            vm.PlcCtl._port = Global.gConfigVM.PlcPort;

            while (!vm._isCloseNet)
            {
                if (!vm.PlcCtl.IsConnected)
                {
                    vm.PlcCtl.doConnect();
                }
                else
                {
                    vm.PlcCtl.getPLCAll();
                }



                Thread.Sleep(5000);

            }
        }
        public void startPlcThread()
        {
            _plcThread.Start(this);
        }

    }
}
