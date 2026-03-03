using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StarMaint.Common;
using StarMaint.Model;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Collections.ObjectModel;
using System.Threading;
using System.Xml;
using System.Windows.Input;
using System.Windows.Controls;
using System.Windows;
using StarMaint.View;
using System.Windows.Threading;
using System.IO;
using Microsoft.Win32;

namespace StarMaint.ViewModel
{
    public enum STAT_TYPE : int
    {
        STATUS_IDLE_NO_CONN = 0,
        STATUS_IDLE_CONN,
        STATUS_IDLE_BIBIN,
        //        STATUS_IDLE_LOAD,
        //        STATUS_IDLE_CONTACT,
        //        STATUS_IDLE_STOP,
        //        STATUS_IDLE_MASK,
        //        STATUS_OPER_LOADING,
        //        STATUS_OPER_CONTACTING,
        STATUS_OPER_RUNNING,
        //        STATUS_OPER_ABORTING,
        //        STATUS_ERR_ABORT,
        STATUS_ERR_ERROR,
        STATUS_OPER_DIAG,
        STATUS_TYPE_COUNT
    }
    public partial class MainVM :BaseVM
    {
        /// <summary>
        /// Command Binding
        /// </summary>
        public static CommandBindingCollection Commands = new CommandBindingCollection();
        public static RoutedCommand SerialNoCmd = new RoutedCommand("SerialNoCmd", typeof(ApplicationCommands));
        public static RoutedCommand DaemonCmd = new RoutedCommand("DaemonCmd", typeof(ApplicationCommands));

        void canExcute_Daemon(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }


        int _daemonUpdateFlag = 0;
        void exCute_Daemon(object send, ExecutedRoutedEventArgs e)
        {
             Button bt = e.Source as Button;
            if (bt == null) return;
            if (bt.Name == "xBT_update")
            {
                OpenFileDialog openFileDialog = new OpenFileDialog();
                openFileDialog.InitialDirectory = "d:\\nbi\\abin";
                openFileDialog.Filter = "Daemon files *.All files (*.*)|*.*";
                openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;
                if (openFileDialog.ShowDialog() == false) return;
                                    
                string filePath = openFileDialog.FileName;
                if (filePath.IndexOf("daemon") == -1)
                {
                    MessageBox.Show("Only Daemon File Enable");
                    return;
                }
                _daemonUpdateFlag = 1;
                _sendProcess.start(_selSlot, "/star/test/daemon", filePath);
                for (int i = 0; i < 5; i++) {
                    if (_daemonUpdateFlag == 2) break;
                    Thread.Sleep(1000);                    
                }
                
                MessageBox.Show((_daemonUpdateFlag == 2) ? "Upload Complete": "Upload Fail!");

                if (_daemonUpdateFlag == 2)
                {
                    string cmd = "<ctrl-cm cmd=\"sh sudo /star/test/daemon_cmd up\"/>";
                    _client.sendCmd((int)MSG_TYPE.DEST_DAEMON, _selSlot, "", cmd);
                }
                _daemonUpdateFlag = 0;


            }
            else if (bt.Name == "xBT_restart")
            {
                string cmd = "<ctrl-cm cmd=\"sh sudo /star/test/daemon_cmd\"/>";
                _client.sendCmd((int)MSG_TYPE.DEST_DAEMON, _selSlot, "", cmd);
            }
        }

        void canExcute_SerialNo(object sender, CanExecuteRoutedEventArgs e) 
        {
            SlotStat ss =  getSlot(_selSlot);
            
            e.CanExecute = true;
            
            if (ss != null)
            {
                if (ss._isOnline == false)
                {
                    e.CanExecute = false;
                }else
                    e.CanExecute = true;
            }else
                e.CanExecute = false;
//            string name = ((Button)e.Source).Name;
//            Console.WriteLine($"canExcute_SerialNo {name}");
        }

        void exCute_SerialNo(object send, ExecutedRoutedEventArgs e)
        {
            string name = ((Button)e.Source).Name;
            if (name == "xBT_Lock")
            {
                IsLock = !IsLock;
                _boardNoPage.xBT_Lock.Content = IsLock? "Lock" : "UnLock";
                foreach(DpsUserData dpsData in DpsDatas)
                {
                    dpsData.IsLock = IsLock;
                }

            }
            else if (name == "xBT_Push")
            {
                if (!IsLock)
                    sendRomData(true);
            }
        }


        /// <summary>
        /// Network
        /// </summary>
        public StarClient _client = new StarClient();
        SendFileProcess _sendProcess = new SendFileProcess();

        /// <summary>
        /// Thread 
        /// </summary>

        public Thread _msgQueThread = new Thread(threadMsgQue); // Connection Check 용
        public Thread _checkThread = new Thread(threadNetCheck); // Connection Check 용
        
        public static bool _isClose = false;

        public static void threadMsgQue(object parent)
        {
            MainVM vm = parent as MainVM;

            while (MainVM._isClose == false)
            {
                if (vm._client.getQueSize() > 0)
                {
                    StarMessage msg = vm._client.pop();
                    if (msg.type == MSG_TYPE.TYPE_REQFILE || msg.type == MSG_TYPE.TYPE_REQDONE)
                    {
                        vm.processRecvFile(msg.recvMsg);
                    }
                    else
                    {
                        vm.processResult(msg.command, msg.xmlNode);
                    }
                }
                else
                {
                    Thread.Sleep(1);
                }
            }   
                        
        }

        public static void threadNetCheck(object parent)
        {
            MainVM vm = parent as MainVM;

            while (MainVM._isClose == false)
            {
                if (vm._client.IsConnected == false)
                {
                    vm._client.doConnect();
                }
                Thread.Sleep(2000);
            }
        }

        /// <summary>
        /// Member
        /// </summary>
        public Collection<BitmapImage> StateBMIArr;
        public Collection<string> StatStrArr;
        public Collection<SolidColorBrush> StatColArr;
        public MainWindow _mainWin;
        public BoardNo _boardNoPage;
        public string _selSlot = "";

        bool _isLock = true;
        public bool IsLock { get => _isLock;
            set
            {
                _isLock = value;
                OnPropertyChanged(nameof(IsLock));
            }
        }

        /// <summary>
        /// SlotStat.cs
        /// </summary>
        ObservableCollection<SlotStat> _slots = null;
        public ObservableCollection<SlotStat> Slots
        {
            get
            {
                if (_slots == null)
                {
                    _slots = new ObservableCollection<SlotStat>();
                }
                return _slots;
            }
            set
            {
                _slots = value;
                OnPropertyChanged(nameof(Slots));
            }
        }
        /// <summary>
        /// DpsUserData.cs
        /// DpsNo.xaml
        /// </summary>
        string _tpcbno = "-";
        public string Tpcbno
        {
            get { return _tpcbno; }
            set
            {
                _tpcbno = value;
                OnPropertyChanged(nameof(Tpcbno));                                
            }
        }

        ObservableCollection<DpsUserData> _dpsDatas = new ObservableCollection<DpsUserData>();
        public ObservableCollection<DpsUserData> DpsDatas
        {
            get => _dpsDatas;
            set
            {
                _dpsDatas = value;
                OnPropertyChanged(nameof(DpsDatas));
            }
        }

        /// <summary>
        /// Function 
        /// </summary>

        void initResource()
        {
            StateBMIArr = new Collection<BitmapImage>();
            StateBMIArr.Add(new BitmapImage(new Uri(@"/Resources/st_dcon_32.bmp", UriKind.RelativeOrAbsolute)));
            StateBMIArr.Add(new BitmapImage(new Uri(@"/Resources/st_con_32.bmp", UriKind.RelativeOrAbsolute)));
            StateBMIArr.Add(new BitmapImage(new Uri(@"/Resources/st_bib_32.bmp", UriKind.RelativeOrAbsolute)));
            StateBMIArr.Add(new BitmapImage(new Uri(@"/Resources/st_run_32.bmp", UriKind.RelativeOrAbsolute)));
            StateBMIArr.Add(new BitmapImage(new Uri(@"/Resources/st_err_32.bmp", UriKind.RelativeOrAbsolute)));
            StateBMIArr.Add(new BitmapImage(new Uri(@"/Resources/diag.bmp", UriKind.RelativeOrAbsolute)));

            StatStrArr = new Collection<string>();
            StatStrArr.Add("Disconect");
            StatStrArr.Add("Connected");
            StatStrArr.Add("BIB IN");
            StatStrArr.Add("RUN");
            StatStrArr.Add("ERROR");
            StatStrArr.Add("DIAG");

            StatColArr = new Collection<SolidColorBrush>();
            StatColArr.Add(new SolidColorBrush(Colors.Gray));
            StatColArr.Add(new SolidColorBrush(Colors.White));
            StatColArr.Add(new SolidColorBrush(Colors.Yellow));
            StatColArr.Add(new SolidColorBrush(Colors.LightGreen));
            StatColArr.Add(new SolidColorBrush(Colors.IndianRed));
            StatColArr.Add(new SolidColorBrush(Colors.BlueViolet));

            for (int i = 0; i < 4; i++)
                _dpsDatas.Add(new DpsUserData(string.Format("DPS{0}", i+1)));
            
        }

        public void initCommand()
        {
            Commands.Add(new CommandBinding(MainVM.SerialNoCmd, exCute_SerialNo, canExcute_SerialNo));
            Commands.Add(new CommandBinding(MainVM.DaemonCmd, exCute_Daemon, canExcute_Daemon));
        }

        public MainVM()
        {
            initResource();                        
            
            _client.deleFunc += processResult;
            _client.recvFileFunc += processRecvFile;
            _sendProcess.Client = _client;

            startNetwork();

        }

        public void startNetwork()
        {
            _msgQueThread.Start(this);
            _checkThread.Start(this);
        }

        ~MainVM()
        {
            _isClose = true;
            _checkThread.Join();
            _client.Close();
            _msgQueThread.Join();
            
        }

        void addSlot(string id)
        {
            SlotStat st = new SlotStat();
            st.Name = id;
            Slots.Add(st);
            st._statType = STAT_TYPE.STATUS_IDLE_NO_CONN;
            st.Stat = StatStrArr[0];
            st.Icon = StateBMIArr[0];
            st.Statbr = StatColArr[0];
        }

        SlotStat getSlot(string id)
        {
            foreach (SlotStat st in Slots)
            {
                if (st.Name == id) return st;
            }
            return null;
        }

        int getSlotIndex(string id)
        {
            int index = 0;
            foreach (SlotStat st in Slots)
            {
                if (st.Name == id) break;
                index++;
            }
            return index;
        }

        /* 
        Recv
         * <star-p >
            * <rom>
            * <board val="ad1ee209"/>
            *   <dps id="0" val="805,2203,41"/>
            *   <dps id="1" val="805,2203,42"/>
            *   <dps id="2" val="805,2203,43"/>
            *   <dps id="3" val="805,2203,44"/>
            * </rom>
         * <star-p >
        
        Send - get
        <star-p >
            <rom cmd="get"/>
        <star-p >
        
        Send - set
        <star-p >
            <rom cmd="set" b="1234" d1="12" d2="22" .. />
        <star-p >
        
        </star-p>
        */
        public void sendRomData(bool set)
        {
            string msg = "";
            if (set)
            {
                msg = string.Format("<rom cmd=\"set\" ");
                msg += string.Format("b=\"{0}\" ", Tpcbno);
                for (int i = 0; i < 4; i++)
                {
                    msg += string.Format("d{0}=\"{1},{2},{3}\" ", i + 1,
                        DpsDatas[i].UserData1,
                        DpsDatas[i].UserData2,
                        DpsDatas[i].UserData3 );
                }
                msg += string.Format("/>");
            }
            else
            {
                msg = string.Format("<rom cmd=\"get\" />");
            }
            _client.sendCmd((int)MSG_TYPE.DEST_DAEMON, _selSlot, "", msg);
        }
    

        public void selectSlot(int idx)
        {
            if (Slots.Count == 0) return;
            _selSlot = Slots[idx].Name;
            if (Slots[idx]._isOnline == false) // Select Item 초기화
            {
                Tpcbno = "-";
                for (int i = 0; i < 4; i++)
                {
                    DpsDatas[i].UserData1 = "-";
                    DpsDatas[i].UserData2 = "-";
                    DpsDatas[i].UserData3 = "-";
                }

                Files.Clear();
            }
            else
            {
                if (Slots[idx]._statType == STAT_TYPE.STATUS_OPER_RUNNING 
                    || Slots[idx]._statType == STAT_TYPE.STATUS_OPER_DIAG)
                {
                    Tpcbno = "X";
                    for (int i = 0; i < 4; i++)
                    {
                        DpsDatas[i].UserData1 = "X";
                        DpsDatas[i].UserData2 = "X";
                        DpsDatas[i].UserData3 = "X";
                    }
                }
                else
                    sendRomData(false);
            }

            


        }

        void setSlot(string id, string bid, bool online, bool isPgm, bool isDiag)
        {
            int slotIndex = getSlotIndex(id);
            if (slotIndex >= Slots.Count) return;
          

            Slots[slotIndex]._isOnline = online;
            if (bid == "0x0") bid = "";

            Slots[slotIndex].BID = bid;
            if (online == false)
            {
                Slots[slotIndex]._statType = STAT_TYPE.STATUS_IDLE_NO_CONN;
                Slots[slotIndex].BID = "";
            }
            else
            {
                Slots[slotIndex]._statType = STAT_TYPE.STATUS_IDLE_CONN;
                if (bid != "")
                    Slots[slotIndex]._statType = STAT_TYPE.STATUS_IDLE_BIBIN;

                if (isPgm)
                    Slots[slotIndex]._statType = STAT_TYPE.STATUS_OPER_RUNNING;
                if (isDiag)
                    Slots[slotIndex]._statType = STAT_TYPE.STATUS_OPER_DIAG;

            }
            int sel = (int)Slots[slotIndex]._statType;
            Slots[slotIndex].Stat = StatStrArr[sel];
            Slots[slotIndex].Icon = StateBMIArr[sel];
            Slots[slotIndex].Statbr = StatColArr[sel];

        }

        void doCUList(XmlNode xml)
        {
            XmlNodeList listCU = xml.SelectNodes("result/cu-list/cu");

            int cnt = 0;
            bool isFirst = false;
            foreach (XmlNode nodeCU in listCU)
            {
                if (nodeCU == null) continue;
                string cuID = nodeCU.Attributes["id"].Value;
                string bid = nodeCU.Attributes["bid"].Value;
                bool isOnline = Convert.ToInt32(nodeCU.Attributes["online"].Value) == 1 ? true : false;
                bool isPGM = Convert.ToInt32(nodeCU.Attributes["pgm"].Value) == 1 ? true : false;
                bool isDiag = Convert.ToInt32(nodeCU.Attributes["diag"].Value) == 1 ? true : false;

                if (Slots.Count <= cnt)
                {
                    Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => addSlot(cuID)));
                    isFirst = true;
                }
                cnt++;


                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => setSlot(cuID, bid, isOnline, isPGM, isDiag)));
            }
            if (isFirst)
                _client.sendIdentify("StarMaint");
        }


        void doRom(XmlNode xml)
        {
            XmlNode nodeRom = xml.SelectSingleNode("result/rom/board");
            string val = nodeRom.Attributes["val"].Value;
            Tpcbno = val;

            XmlNodeList listDps = xml.SelectNodes("result/rom/dps");
            int cnt = 0;
            foreach (XmlNode nodeVal in listDps)
            {
                if (nodeVal == null) continue;
                string str = nodeVal.Attributes["val"].Value;
                string[] tokens = str.Split(',');
                if (tokens.Length < 3) continue;

                DpsDatas[cnt].UserData1 = tokens[0];
                DpsDatas[cnt].UserData2 = tokens[1];
                DpsDatas[cnt].UserData3 = tokens[2];

                cnt++;
            }
        }

        void doSlotStat(XmlNode xml)
        {            
            if (xml.Attributes["path"] == null) return;
            string cuID = xml.Attributes["path"].Value.Trim();

            XmlNode nodeStat = xml.SelectSingleNode("result/slot-stat");
            if(nodeStat == null) return;
            
            string bid = nodeStat.Attributes["bid"].Value;
            bool isPGM = Convert.ToInt32(nodeStat.Attributes["pgm"].Value) == 1 ? true : false;
            bool isDiag = Convert.ToInt32(nodeStat.Attributes["diag"].Value) == 1 ? true : false;
            setSlot(cuID, bid, true, isPGM, isDiag);
        }

        void doCmCtrl(XmlNode xml)
        {
            XmlNode nodeCm = xml.SelectSingleNode("result");
            if (nodeCm == null) return;

            string text = nodeCm.InnerText;
            string[] lines = text.Split('\n');

            foreach (var line in lines)
            {
                if (line.Length < 40) continue;
                int pos = line.IndexOf(":");
                if (pos == -1) continue;
                string name = line.Substring(pos + 3).Trim();

                if (name[name.Length - 1] == '/') // folder
                    continue;

                int type = 0;
                if (name.IndexOf(".xml") != -1)
                    type = 1;

                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Files.Add(
                        new FileModel { Name = name, Icon = type == 1 ? FileBmArr[1] : FileBmArr[0], Type = type }
                        ))                         
                );

                
            }
        }

        void doReqFile(XmlNode xml)
        {
            Console.WriteLine(xml.InnerText);
        }

        void processRecvFile(string xmlText)
        {
            int pos = xmlText.IndexOf("path=");
            string slotName = xmlText.Substring(pos + 6, 6);

            int st = xmlText.IndexOf("name=\"") + 6;
            int end = xmlText.IndexOf("\"", st) - st;
            string fileName = xmlText.Substring(st, end);
            
            st = xmlText.IndexOf("ofs", pos) + 5;
            end = xmlText.IndexOf("\"", st) - st;
            int ofs = Convert.ToInt32(xmlText.Substring(st, end));


            pos = xmlText.IndexOf("send-file");
            st = xmlText.IndexOf(">", pos) + 1;
            end = xmlText.IndexOf("</send-file>") - st;
            if (end < 0) return;

            string text = xmlText.Substring(st, end);


            if (ofs == -1)
            {
                // File Recv Complete                
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => loadingWin.Hide()));
            }
            else if (ofs == 0)
            {                
                //File.WriteAllText(fileName, text, Encoding.Default);
                FileStream fs = new FileStream(fileName, FileMode.Create);
                StreamWriter sw = new StreamWriter(fs);
                sw.Write(text);
                sw.Close();
                setSciText(text, false);
            }
            else
            {
                StreamWriter sw = File.AppendText(fileName);
                sw.Write(text);
                sw.Close();
                setSciText(text, true);
            }

            _client.sendResponse(slotName, "SEND-FILE", true);  

         }

        public void doSendFile(XmlNode xml)
        {
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => loadingWin.Hide()));

            if (!_sendProcess.isRunning)
            {
                if (_daemonUpdateFlag != 0) _daemonUpdateFlag = 2;
                return;
            }
            _sendProcess.next();

        }

        void doResult(RESULT_CMD type, XmlNode xml)
        {
          //  Console.WriteLine(type.ToString());
            try
            {
                switch (type)
                {
                    case RESULT_CMD.CMD_CU_LIST:
                    case RESULT_CMD.CMD_CU_LIST_AUTO: doCUList(xml); break;
                    case RESULT_CMD.CMD_STATUS: doSlotStat(xml); break;
                    case RESULT_CMD.CMD_ROM: doRom(xml); break;
                    case RESULT_CMD.CMD_CM_CTRL: doCmCtrl(xml); break;
                    case RESULT_CMD.CMD_REQ_FILE: doReqFile(xml); break;
                    case RESULT_CMD.CMD_SEND_FILE: doSendFile(xml); break;


                    default: break;
                }
            }
            catch (Exception ex)
            { Console.WriteLine(ex.ToString()); }
        }

        async void processResult(RESULT_CMD type, XmlNode xml)
        {
            /// -- User Thread 
//            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
//                    new Action(() => doResult(type, xml)));
            
            /// -- CLR Worker Thread
            Task t = Task.Run(() => doResult(type, xml));
            await t;

        }

    }
}
