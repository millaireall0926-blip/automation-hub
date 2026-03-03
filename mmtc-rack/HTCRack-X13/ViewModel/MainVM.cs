using System;
using System.Collections.Generic;
using System.Configuration;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using System.Xml.Linq;
using HTCRack.Base;
using HTCRack.IPC;
using HTCRack.Model;
using HTCRack.Net;
using HTCRack.View;
using MongoAccess.Access;
using MongoAccess.Models;
using PropertyChanged;
using ScottPlot.Drawing.Colormaps;
using Application = System.Windows.Application;
using RackModel = HTCRack.Model.RackModel;

namespace HTCRack.ViewModel {

    enum VIEW_SEL : int 
    {
        MAIN = 0,
        DIMM,        
        ENGINEER,
        CONFIG,
        SETUP
    }
    public class MainVM :BaseVM  {
        
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

        public string LogoPath { get; set; }

        [DoNotNotify]
        public ModbusLS PlcCtl { get; set; } = new ModbusLS();
        [DoNotNotify]        
        public bool IsUsePassword { get; set; }

        public Visibility IsUseSetup { get; set; } = Visibility.Hidden;
        public MainVM() {
            Global.gMainVM = this;
            changeView("MAIN");
            _conThread.Start(this);
            _netThread.Start(this);

            IPCStatIcon = DisconnectIcon;
            IpcRecver.notiCon += setConnectIcon;
            IpcRecver.notiRecv += recvIPCData;
            
          //  PlcCtl.requestRX(new string[] {"%MW0"});
          //   PlcCtl.requestWX(new string[] { "%MW0" }, new byte[][] { new byte[] { 1 } });
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
            if (box >= rm.CBoxs.Count) return;
            var bm = rm.CBoxs[box];
            
            
            foreach (CBoxSub sub in bm.BoxSubs)
            {
                sub.TargetSV = sv[0];
                bm.sendSV(sub, sub.TargetSV);
            }
        }
        
        public void sendIPCData(IPCMessage msg)
        {
            IpcRecver.pushSendQue(msg);
        }


        public void changeView(string name) {
            SelectName = name;
            ShowMain = Visibility.Hidden;
            ShowDimm = Visibility.Hidden;
            ShowFan = Visibility.Hidden;
            ShowPower = Visibility.Hidden;
            ShowEngineer = Visibility.Hidden;
            ShowConfig = Visibility.Hidden;
            ShowBoard = Visibility.Hidden;
            
                
            if (name == VIEW_SEL.MAIN.ToString()) {
                SelectViewName = "Main Control";
                SelectViewDiscription = "Operate The Chamber And Perform Test Functions";
                ShowMain = Visibility.Visible;
            }
            else if (name == VIEW_SEL.DIMM.ToString()) {
                SelectViewName = "Dimm Temperature";
                SelectViewDiscription = "Output Real-Time DIMM Temperature And Graph";
                ShowDimm = Visibility.Visible;                
                Global.gDimmVM.setRack();

            }            
            else if (name == VIEW_SEL.ENGINEER.ToString()) {

                bool show = doPasswordDialog();
                if (show)
                {
                    SelectViewName = "Engineer Mode";
                    SelectViewDiscription = "Set up Register Manunaly And Excute Command";
                    ShowEngineer = Visibility.Visible;
                    Global.gEnginerVM.setRack();
                }
            }
            else if (name == VIEW_SEL.CONFIG.ToString()) {
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
        
        
        public void quit()
        {
            _isCloseNet = true;            
            _netThread.Join();
            _conThread.Join();
            if (_plcThread.IsAlive) 
                _plcThread.Join();
            if (_dbThread.IsAlive)
                _dbThread.Join();

            foreach (RackModel rm in Global.gMainPageVM.Racks)            
                rm._client.quit();

            IpcRecver.stop();
            
        }

        public bool _isCloseNet = false;

        public int _connectRetryProg = 0;
        public int _connectRetry = 5;
        public static Thread _conThread = new Thread(ConnectFunction);
        public static Thread _netThread = new Thread(NetworkFunction);
        public static Thread _dbThread = new Thread(DBFunction);
        public static Thread _plcThread = new Thread(PlcFunction);

        static public void ConnectFunction(object parent)
        {
            MainVM mv = (MainVM)parent;
            while (!mv._isCloseNet)
            {
                if (Global.gMainPageVM == null)
                {  Thread.Sleep(1000); continue;  }

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
            MainVM mv  = (MainVM)parent;
 

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
                        HtcMessage msg = Global.newMsg(0xff, 0, 0x11, 307);
                        rm._client.pushSendQue(REG_TYPE.PDO, msg);

                        rm._client.prevPDOTime = DateTime.Now;
                    }
                    else
                    {
                        if (rm._client._ackWaitCount >= ackWaitLimit)
                        {
                            Console.WriteLine("AckWaitTime Out Pdo Force Send!");
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


        #region DB Functions

        public EquipModel createDBEquip(DBAccess db)
        {
            var dbEquip = new EquipModel();
            dbEquip.Equip = Global.gConfigVM.UIName;
            dbEquip.Alive = 0;
            dbEquip.Racks = new List<MongoAccess.Models.RackModel>(Global.gMainPageVM.Racks.Count);


            foreach (var rack in Global.gMainPageVM.Racks)
            {
                MongoAccess.Models.RackModel dbRack = new MongoAccess.Models.RackModel();
                dbRack.Name = rack.Name;
                dbRack.IP = rack._client._ip;
                dbRack.Stat = (int)rack.RackStat;
                dbRack.Boxs = new List<BoxModel>(rack.CBoxs.Count);

                foreach (var box in rack.CBoxs)
                {
                    MongoAccess.Models.BoxModel dbBox = new MongoAccess.Models.BoxModel();
                    dbBox.DimmAvg = Convert.ToDouble(box.BoxTemp);
                    dbBox.InsideBox = box.InsideBoxTemp;

                    dbBox.Dimms = new List<double>(box.DimModels.Count);
                    foreach (var dimm in box.DimModels)
                        dbBox.Dimms.Add(dimm.DimTemp);
                    dbRack.Boxs.Add(dbBox);
                }

                dbRack.StartT = rack.TimeStart.ToString("yyyy-MM-dd HH:mm:ss");
                dbRack.EndT = rack.TimeEnd.ToString("yyyy-MM-dd HH:mm:ss");

                dbEquip.Racks.Add(dbRack);
            }
            db.createEquip(dbEquip);


            return dbEquip;
        }

        public bool updateDBData(EquipModel dbEquip)
        {
            dbEquip.Alive++;
            if (dbEquip.Racks.Count != Global.gMainPageVM.Racks.Count) return false;

            for  (int ri = 0; ri< Global.gMainPageVM.Racks.Count; ri++)
            {
                RackModel rack = Global.gMainPageVM.Racks[ri];
                MongoAccess.Models.RackModel dbRack = dbEquip.Racks[ri];
                dbRack.Stat = (int)rack.RackStat;

                if (rack.CBoxs.Count != dbRack.Boxs.Count) return false;

                for (int bi = 0; bi < rack.CBoxs.Count; bi++)
                {
                    CBoxModel cBox = rack.CBoxs[bi];
                    BoxModel dbBox = dbRack.Boxs[bi];

                    dbBox.DimmAvg =  Convert.ToDouble(cBox.BoxTemp);
                    dbBox.InsideBox = cBox.InsideBoxTemp;
                    
                    if (cBox.DimModels.Count != dbBox.Dimms.Count) return false;
                    for (int di = 0; di< cBox.DimModels.Count; di++)
                    {
                        dbBox.Dimms[di] = cBox.DimModels[di].DimTemp;
                    }
                }
            }

            return true;
        }


        static public void DBFunction(object parent)
        {
            MainVM vm = (MainVM)parent;
            DBAccess db = new DBAccess();
            db.setConStr(Global.gConfigVM.MongoIP, 
                        Global.gConfigVM.MongoPort,
                        Global.gConfigVM.MongoUser,
                        Global.gConfigVM.MongoPW,
                        Global.gConfigVM.MongoAuth);

            string myName = Global.gConfigVM.UIName;
            EquipModel dbEquip = null;

            while (!vm._isCloseNet)
            {                                
                if (db._isConnect == false)
                {
                    db.checkConnection();
                    if (db._isConnect)
                    {
                        dbEquip = db.getEquip(myName);
                        if (dbEquip == null && db._isConnect)
                            dbEquip = vm.createDBEquip(db);
                    }

                    Thread.Sleep(1000 * 10);
                    continue;
                }

                if (dbEquip == null) continue;

                RACK_STAT equipStat = RACK_STAT.STOP;
                
                foreach (var rack in Global.gMainPageVM.Racks)
                { 
                    if (rack.RackStat == RACK_STAT.ERROR)
                    {
                        equipStat = RACK_STAT.ERROR; break;
                    }else if (rack.RackStat == RACK_STAT.RUN || rack.RackStat == RACK_STAT.PREV_RUN)
                    {
                        equipStat = RACK_STAT.PREV_RUN; break;
                    }

                    dbEquip.Racks[Global.gMainPageVM.Racks.IndexOf(rack)].Stat = (int)rack.RackStat;
                }
                dbEquip.Alive++;
                db.replaceEquip(dbEquip);

                //if (equipStat == RACK_STAT.RUN)
                //{
                //    bool ret = vm.updateDBData(dbEquip);
                //    if (!ret)  // Differnt Box, Dim Setting. Replace data
                //    {
                //        db.deleteEquip(dbEquip);          
                //        db.createEquip(dbEquip);
                //    }else
                //    {
                //        db.replaceEquip(dbEquip);
                //    }
                //}else 
                //    db.updateAlive(dbEquip);
                //
                Thread.Sleep(1000 * 5);
            }
        }

        static public void PlcFunction(object parent)
        {
            MainVM vm = (MainVM)parent;
            vm.PlcCtl._ip = Global.gConfigVM.PlcIP;
            vm.PlcCtl._port = Global.gConfigVM.PlcPort;

            while(!vm._isCloseNet)
            {
                if (!vm.PlcCtl.IsConnected)
                {
                    vm.PlcCtl.doConnect();
                }else
                {
                    vm.PlcCtl.getPLCAll();
                }



                Thread.Sleep(5000);

            }
        }

        // Call Function after ConfigVM Loaded
        public void startDBThread()  
        {

            _dbThread.Start(this);
        }

        public void startPlcThread()
        {
            _plcThread.Start(this);
        }

        #endregion


    }
}
