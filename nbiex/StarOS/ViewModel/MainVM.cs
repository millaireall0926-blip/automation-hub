using HandyControl.Controls;
using Microsoft.Win32;
using MySql.Data.MySqlClient;
using StarBase;
using StarOS.Model;
using StarOS.View;
using System;
using System.Collections.ObjectModel;
using System.IO;
using System.Security.Cryptography;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Threading;

namespace StarOS.ViewModel
{



    public class Step :BaseVM
    {
        public Step(string name) => Name = name;
        public string name;
        public string Name
        {
            get { return name; }
            set { name = value; OnPropertyChanged(Name); }
        }
    }

    public class MainVM : BaseVM
    {
        public LotInfo LotWin;
        public DBLoader DBLoaderWin;
        public Result ResultWin;
        
        string _lotID;
        public string LotID
        {
            get { return _lotID; }
            set { _lotID = value; OnPropertyChanged("LotID"); }
        }
        #region DB Loader UI 관련
        bool _reTest;
        public bool ReTest
        {
            get { return _reTest; }
            set { _reTest = value; OnPropertyChanged("ReTest"); }
        }

        string _week;
        public string Week
        {
            get { return _week; }
            set { _week = value; OnPropertyChanged("Week"); }
        }
        string _inboudDate;
        public string InboudDate
        {
            get { return _inboudDate; }
            set { _inboudDate = value; OnPropertyChanged("InboudDate"); }
        }
        string _serialNumber;
        public string SerialNumber
        {
            get { return _serialNumber; }
            set { _serialNumber = value; OnPropertyChanged("SerialNumber"); }
        }
        public string _devType;
        public string DevType
        {
            get { return _devType; }
            set { _devType = value; OnPropertyChanged("DevType"); }
        }
        public string _chamberType;
        public string ChamberType
        {
            get { return _chamberType; }
            set { _chamberType = value; OnPropertyChanged("ChamberType"); }
        }
        public string _testType;
        public string TestType
        {
            get { return _testType; }
            set { _testType = value; OnPropertyChanged("TestType"); }
        }
        #endregion


        ObservableCollection<DBPgmModel> _pgmModels = new ObservableCollection<DBPgmModel>();
        public ObservableCollection<DBPgmModel> PgmModels
        {
            get { return _pgmModels; }
            set { _pgmModels = value; OnPropertyChanged("PgmModels"); }
        }   

        public Visibility _visibilityDBList = Visibility.Collapsed;
        public Visibility VisibilityDBList
        {
            get { return _visibilityDBList; }
            set { _visibilityDBList = value; OnPropertyChanged("VisibilityDBList"); }
        }

        ObservableCollection<string> _selSteps = new ObservableCollection<string>();
        public ObservableCollection<string> SelSteps
        {
            get { return _selSteps; }
            set
            {
                _selSteps = value;
                OnPropertyChanged("SelSteps");
            }
        }

        string _mainText = "NO ROUTER";
        public string MainText { 
            get=>_mainText; 
            set { _mainText = value; OnPropertyChanged(nameof(MainText)); } }

        ObservableCollection<Step> _steps = null;
        public ObservableCollection<Step> Steps
        {
            get { return _steps; }
            set
            {
                _steps = value;
                OnPropertyChanged("Steps");
            }
        }
        public StepBar xSteps;

        public MainVM()
        {
            Global.gMainVM = this;
            Steps = new ObservableCollection<Step>();

            Commands.Add(new CommandBinding(CmdLot, excute_CmdLot, canExcute_CmdLot));
            Commands.Add(new CommandBinding(CmdDB, excute_CmdDB, canExcute_CmdDB));

        }

        ~MainVM()
        {
            _uiThreadClose = true;
            _uiThread.Join();
        }

        #region UI Update 관련

        public bool checkCurSlot(string name)
        {
            if (Global.gSlotVM.CurSlot != null)
                if (Global.gSlotVM.CurSlot.Name == name)
                    return true;
            return false;

        }

        public void checkUpdateUI<T>(int type, string name, T org, T mod)
        {
            if (checkCurSlot(name))
                if (!org.Equals(mod))
                    Global.gMainVM.updateUIFlag(type);            
        }

        public void updateUIPriority1(int ui)
        {
            if (Global.gSlotVM.CurSlot == null)
            {
                _uiUpdateFlag = (int)UI.DONE;
                return;
            }

            if ((ui & (int)UI.INFO) != 0)
            {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gTestVM.updateUI()));
                _uiUpdateFlag &= ~(int)UI.INFO;
            }
            if ((ui & (int)UI.MENU) != 0)
            {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gMainWin.updateMenu()));
                _uiUpdateFlag &= ~(int)UI.MENU;
            }
            if ((ui & (int)UI.SLOTLOG) != 0)
            {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gTestVM.setSlotLog(Global.gSlotVM.CurSlot)));
                _uiUpdateFlag &= ~(int)UI.SLOTLOG;
            }
            if ((ui & (int)UI.STEPLIST) != 0)
            {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => setStepList()));
                _uiUpdateFlag &= ~(int)UI.STEPLIST;
            }
            if ((ui & (int)UI.POWER) != 0)
            {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gPowerVM.updatePower(Global.gSlotVM.CurSlot)));
                _uiUpdateFlag &= ~(int)UI.POWER;
            }

        
        }

        public void updateUIPriority2(int ui)
        {
            if (Global.gSlotVM.CurSlot == null)
            {
                _uiUpdateFlag = (int)UI.DONE;
                return;
            }

            if ((ui & (int)UI.BIB) != 0)
            {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gResultVM.setBIB(Global.gSlotVM.CurSlot)));
                _uiUpdateFlag &= ~(int)UI.BIB;
            }

            if ((ui & (int)UI.BIB_SIZE) != 0)
            {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gResultVM.setSize()));
                _uiUpdateFlag &= ~(int)UI.BIB_SIZE;
            }

            if ((ui & (int)UI.RESULT) != 0)
            {
                if (Global.gResultVM._busyResult) return;
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gResultVM.setSlot(Global.gSlotVM.CurSlot)));
                _uiUpdateFlag &= ~(int)UI.RESULT;
            }

            if ((ui & (int)UI.RESULT_SUB) != 0)
            {
                if (Global.gResultVM._busySub)
                {
//                    Console.WriteLine($"updateUIPriority2 Busy : {Global.gResultVM._busySub}");
                    return;
                }
//                Console.WriteLine("do loadSub");
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gResultVM.selectDut()));
                _uiUpdateFlag &= ~(int)UI.RESULT_SUB;
            }

            if ((ui & (int)UI.REGISTER) != 0)
            {
                if (Global.gTestVM.RegisterWin.IsVisible)
                {
                    Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                        new Action(() => Global.gTestVM.updateRegisterView()));
                }

                _uiUpdateFlag &= ~(int)UI.REGISTER;
            }

        }

        public void updateUIFlag(int ui)
        {            
            _uiUpdateFlag |= ui;
            //Console.WriteLine($"UI Update : {_uiUpdateFlag.ToString("X4")}");
        }

        public static void threadUI(object parent)
        {
            MainVM vm = parent as MainVM;

            bool doPriority2 = true;
            int flagPriority1 = 0;
            int flagPriority2 = 0;
            while (vm._uiThreadClose == false)
            {
              //  Console.WriteLine($"UI Release : {vm._uiUpdateFlag.ToString("X4")}");
                flagPriority1 = vm._uiUpdateFlag & 0xff;
                flagPriority2 = vm._uiUpdateFlag & 0xff00;

                if (flagPriority1 != 0)
                    vm.updateUIPriority1(flagPriority1);                                        
                if (doPriority2 & (flagPriority2 != 0))
                    vm.updateUIPriority2(flagPriority2);

                doPriority2 = !doPriority2;
                Thread.Sleep(400);
            }
        }

        public bool _uiThreadClose = false;
        public int _uiUpdateFlag = 0;
        public static Thread _uiThread = new Thread(threadUI);

        #endregion

        public void initItems()
        {

//            Steps.Add(new Step("0.Contact"));
//            for (int i = 0; i<16; i++)
//            {
//                Steps.Add((new Step(string.Format("{0}.Step", i))));
//            }
//            xSteps.StepIndex = 4;
//
            
            LotWin = new LotInfo();
            LotWin.Hide();

            DBLoaderWin = new DBLoader();
            DBLoaderWin.Hide();


            ResultWin = new Result();
            ResultWin.Hide();

            _uiThread.Start(this);
            //Commands.Add(new CommandBinding(MainVM.CmdMain, excute_cmdMain, canExcute_cmdMain));
        }

        public void setStepIndex()
        {
            SlotModel slot = Global.gSlotVM.CurSlot;
            if (slot == null) return;
            if (Steps.Count == 0) return;

            string step = slot.StepName;
            int pos =  slot.StepName.IndexOf("~RE");
            if (pos != -1)
                step = slot.StepName.Substring(0, pos);

            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                new Action(() =>
                {
                    int index = 0;
                    foreach (var item in Steps)
                    {
                        if (item.Name == step) break;
                        index++;
                    }

                    if (index >= Steps.Count)
                        index = Steps.Count - 1;
                    xSteps.StepIndex = index;

//                    string str = String.Format($"{step} {Steps[index].Name}, idx{index}");
//                    Console.WriteLine(str);
                }
            ));

        }

        public void setStepList()
        {
            SlotModel slot = Global.gSlotVM.CurSlot;
            if (slot == null) return;
            Steps.Clear();
            if (slot._stepList.Count > 0)
            {
                foreach (var step in slot._stepList)
                    Steps.Add(new Step(step));

                setStepIndex();
            }
        }




        #region Command Set



        public static CommandBindingCollection Commands = new CommandBindingCollection();

        public static RoutedCommand CmdDB = new RoutedCommand("CmdDB", typeof(ApplicationCommands));
        void canExcute_CmdDB(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        public void excute_CmdDB(object sender, ExecutedRoutedEventArgs e)
        {
            Button bt = sender as Button;
            PgmModels.Clear();

            if (bt == null)
                bt = e.Source as Button;
            if (bt == null) return;
            if (Global.gSlotVM.SelectSlots.Count == 0) return;

            if (bt.Name == "xBT_DBCancle")
            {
                DBLoaderWin.Hide();
                return;
            }

            if (LotID == "")
            {
                HandyControl.Controls.MessageBox.Show("Input Lot ID", "Lot ID Empty", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }
            else if (LotID.Length != 17)
            {
                HandyControl.Controls.MessageBox.Show("Input Lot ID", $"Lot ID Lenth 17, Now {LotID.Length}", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }
            LotID = LotID.ToUpper();

            if (ReTest)
            {
                char[] chars = LotID.ToCharArray();
                chars[13] = 'R';

                LotID = new string(chars);
            }
            Week = LotID.Substring(0, 2) + "Y";
            Week += LotID.Substring(2, 2) + "W";
            DevType = LotID.Substring(4, 3);
            string sub = LotID.Substring(7, 1);
            if (sub == "H") ChamberType = "HOT";
            else if (sub == "J") ChamberType = "HOT/COLD";
            else if (sub == "R") ChamberType = "Room Temp";
            else ChamberType = " - ";

            InboudDate = LotID.Substring(8, 2) + "M";
            InboudDate += LotID.Substring(10, 2) + "D";

            sub = LotID.Substring(12, 1);
            if (sub == "S") TestType = "Server";
            else if (sub == "N") TestType = "Normal";
            else if (sub == "A") TestType = "A-Test";
            else if (sub == "B") TestType = "B-Test";            
            else TestType = " - ";
            if (LotID.Substring(13, 1) == "R")
                TestType += "-Re";
            SerialNumber = LotID.Substring(14, 3);

            //string lotRegex = "...."+ LotID.Substring(4, 4) + "...." + LotID.Substring(12, 1) + "....";
            string query = $"select product from inventory where lot='{LotID}'";
            string productID = "";
            try
            {
                using (MySqlConnection mysql = new MySqlConnection(Global.gNet._dbCtrl._dbConCmd))
                {
                    mysql.Open();
                    MySqlCommand cmd = new MySqlCommand(query, mysql);
                    var table = cmd.ExecuteReader();

                    while (table.Read())
                    {
                        productID = table.GetString("product");
                        break;
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            if (productID == "")
            {
                HandyControl.Controls.MessageBox.Show("No Lot Data", "DB Empty", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            //string findPath = Global.gNet._dbCtrl.LoadPath + productID;
            //string findPath = @"\\192.168.50.36\home\H5ANAG8NCJR";
            string findPath = Path.Combine(Global.gNet._dbCtrl.LoadPath, productID);
            try
            {
                string[] subDirs = Directory.GetDirectories(findPath);
                foreach (string dir in subDirs)
                {
                    PgmModels.Add(new DBPgmModel()
                    {
                        LotID = LotID,
                        ProcutID = productID,
                        Pgm = Path.GetFileName(dir) + ".pgm",
                        Adr = Global.gNet._dbCtrl.LoadPath,
                        Path = Path.Combine(productID, Path.GetFileName(dir)),
                    });
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            if (PgmModels.Count == 0)
            {
                HandyControl.Controls.MessageBox.Show("No Data", "DB Empty", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }else 
                VisibilityDBList = Visibility.Visible;  

            

        }



        public static RoutedCommand CmdLot = new RoutedCommand("CmdLot", typeof(ApplicationCommands));
        void canExcute_CmdLot(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        public bool checkBeforeRun() {

            bool isErr = false;
            string msgErr = "";

            foreach (SlotModel slot in Global.gSlotVM.SelectSlots) {
                if (slot.StatType == STAT_TYPE.RUNNING || slot.StatType == STAT_TYPE.CONTACTING) {
                    isErr = true;
                    msgErr += $"{slot.Name} State Not Ready. Now {slot.StatType.ToString()}\r\n";
                }
                if (slot.BID == "") {
                    isErr = true;
                    msgErr += $"{slot.Name} Fail to Read BID. \r\n";
                }
                if (slot._fileSet == null) {
                    isErr = true;
                    msgErr += $"{slot.Name} is Not Ready for File. Load First\r\n";
                }

            }

            if (isErr)
                HandyControl.Controls.MessageBox.Show(msgErr, "Check Slot!", MessageBoxButton.OK, MessageBoxImage.Hand);

            return isErr;
        }
        public void excute_CmdLot(object sender, ExecutedRoutedEventArgs e)
        {
            Button bt = sender as Button;
            if (bt == null)
                bt = e.Source as Button;
            if (bt == null) return;
            if (Global.gSlotVM.SelectSlots.Count == 0) return;

            if (checkBeforeRun()) {
                LotWin.Hide();
                return;
            }

            if (bt.Name == "xBT_LotInput") //Contact 
            {
                string unit = "";
            
                foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
                {                    
                    if (slot.StatType == STAT_TYPE.RUNNING || slot.StatType == STAT_TYPE.CONTACTING)
                        continue;
                    
                    slot.initTest();
                    slot._timerRunCheck.Start();

                    slot.StatType = STAT_TYPE.CONTACTING;
                    string file = "/star/pgm/" + slot._fileSet._excute;
                    Global.gNet._client.sendAuthority(slot.Name, file);
                    Thread.Sleep(10);
                    string runCmd = string.Format($"{file} -net -con -lot={LotID}");
                    slot.LotID = LotID;                    
                    slot.StartTime = DateTime.Now.ToString();
                    slot.EndTime = "";
                    slot._contact = true;

                    slot.startPowerLog();
                    slot.endSlotLog();

                    Global.gNet._client.sendPgmRun(slot.Name, runCmd);                    
                    unit += slot._num.ToString() + " ";
                }
                Global.gTestVM.addSysLog(LOG_TYPE.INFO, "SLOTS", "[CONTACT] " + unit);
            }
            LotWin.Hide();

        }


        // Routed Comand  Canexcute 를 다른 Thread에서 Laoding시 갱신이 안됌... 
        // Direct Update로 변경..               
        ///public static RoutedCommand CmdMain = new RoutedCommand("CmdMain", typeof(ApplicationCommands));

        // CS = Check Stat        
        bool CS(int stat, STAT_TYPE type) => ((stat & (1 << (int)type)) != 0);
        public void enableMenuButton(Button bt, int st)
        {            

            bt.IsEnabled = false;

            if (CS(st, STAT_TYPE.DISCONNECT))
            {
                bt.IsEnabled = false;
                return;
            }

            if (CS(st, STAT_TYPE.CONNECT))
            {
                switch (bt.Name)
                {
                    case "xBT_Diag":
                        bt.IsEnabled = true;
                        break;
                }
                return;
            }


            if (CS(st, STAT_TYPE.INSERT))
            {
                switch (bt.Name)
                {
                    case "xBT_LoadDB":
                    case "xBT_Load":
                    case "xBT_Diag":
                        bt.IsEnabled = true;
                        break;
                }
                return;
            }

            if (CS(st, STAT_TYPE.LOAD) )
            {
                switch (bt.Name)
                {
                    case "xBT_LoadDB":
                    case "xBT_Load":
                    case "xBT_ReLoad":
                    case "xBT_Con":
                        bt.IsEnabled = true;
                        break;
                    default:
                        break;
                }
                return;
            }

            if (CS(st, STAT_TYPE.STOP) || CS(st, STAT_TYPE.ABORT))
            {
                switch (bt.Name)
                {
                    case "xBT_LoadDB":
                    case "xBT_Load":
                    case "xBT_ReLoad":
                    case "xBT_Con":
                    case "xBT_Test":
                    case "xBT_Step":
                        bt.IsEnabled = true;
                        break;
                    default:
                        break;
                }
                return;
            }

            if (CS(st, STAT_TYPE.CONTACT))
            {
                switch (bt.Name)
                {
                    case "xBT_LoadDB":
                    case "xBT_Load":
                    case "xBT_ReLoad":
                    case "xBT_Con":
                    case "xBT_Test":
                    case "xBT_Step":                    
                        bt.IsEnabled = true;
                        break;
                    default:
                        break;
                }
                return;
            }

            if (CS(st, STAT_TYPE.ERROR))
            {
                if (bt.Name == "xBT_Reset")
                    bt.IsEnabled = true;
                return;
            }

            if (CS(st, STAT_TYPE.RUNNING) || CS(st, STAT_TYPE.CONTACTING) 
                || CS(st, STAT_TYPE.LOADING) || CS(st, STAT_TYPE.DIAG))
            {
                switch (bt.Name)
                {
                    case "xBT_Stop":
                        bt.IsEnabled = true;
                        break;
                }
                return;
            }

        }

        public void loadFileSet(SlotModel slot)
        {
            if (!slot.isSendFileRemind()) return;

            slot._sendFileProcess.start ( 
                slot.Name,
                String.Format("/star/pgm/{0}", slot.getSendFile()),
                slot._fileSet._path + slot.getSendFile()
                );
            
            slot.startPowerLog();
            slot.endSlotLog();
            Global.gMainVM.updateUIFlag((int)UI.SLOTLOG);
            
        }

        public void loadByDB(DBPgmModel model)
        {
            foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
            {
                Global.gNet._client.sendPGMRemove(slot.Name);
            }

            DBLoaderWin.Hide();
            FileSet fs = new FileSet();
            fs._excute = model.Pgm;
            fs._path = Path.Combine(model.Adr, model.Path) + "\\";

            fs._files.Add(fs._excute);

            try
            {
                string[] datas = Directory.GetFiles(fs._path, "*.mpa", SearchOption.TopDirectoryOnly);
                foreach (string data in datas) fs._files.Add(Path.GetFileName(data));

                datas = Directory.GetFiles(fs._path, "*.vec", SearchOption.TopDirectoryOnly);
                foreach (string data in datas) fs._files.Add(data.Replace(fs._path, ""));
            }
            catch
            {
                HandyControl.Controls.MessageBox.Show("Check DB PGM Infomation. Fail Load", "Pgm loading Fail", MessageBoxButton.OK, MessageBoxImage.Information);
            }

            foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
            {
                slot.StatType = STAT_TYPE.LOADING;
                slot._fileSet = MConv.ObjectDeepCopy(fs);
                slot.LotID = model.LotID;
                slot._isDBLoad = true;
                // clear somthing

                loadFileSet(slot);
            }
        }

        public void stopedByChamber()
        {
            foreach (var slot in Global.gSlotVM.Slots)
            {
                if (!(slot.StatType == STAT_TYPE.RUNNING || slot.StatType == STAT_TYPE.CONTACTING))
                    continue;
                Global.gNet._client.sendPgmStop(slot.Name);
                slot.StatType = STAT_TYPE.ABORT;
                Global.gTestVM.addSlotLog(slot, "[Chamber-Alarm] Force Stop PGM", LOG_TYPE.WARNING);
            }

        }



        public void excute_cmdMain(Button bt)
        {
            
            if (bt == null) return;
            if (Global.gSlotVM.SelectSlots.Count == 0) return;

            string units = "";
            foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
            {
                units += slot._num.ToString() + " ";
            }

            if (bt.Name == "xBT_LoadDB")
            {
                PgmModels.Clear();
                
                VisibilityDBList = Visibility.Collapsed;
                DBLoaderWin.Left = Global.gMainWin.Left + 300;
                DBLoaderWin.Top = Global.gMainWin.Top + 500;
                DBLoaderWin.Show();
                

            }
            else  if (bt.Name == "xBT_Load")
            {
                //                FileInfo fie = new FileInfo($@"\\{Global.gSlotVM.CurSlot._ip}\star\result\result.xml");
                //                fie.Length;
                //                string fn = $@"\\{Global.gSlotVM.CurSlot._ip}\star\result\result.xml";
                //                File.Exists(fn);
                //                File.Copy(fn, @"d:\data\slot02.xml", overwrite:true);
                //                bool ret = File.Exists(@"d:\data\slot02.xml");


                OpenFileDialog ofd = new OpenFileDialog();
                ofd.Filter = "Pgm File(*.pgm)|*.pgm|ALL Files(*.*)|*.*";
                ofd.FilterIndex = 1;
                ofd.RestoreDirectory = true;
                if (ofd.ShowDialog() == false) return;

                foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
                {
                    Global.gNet._client.sendPGMRemove(slot.Name);
                }

                FileSet fs = new FileSet();
                fs._excute = ofd.SafeFileName;
                string full = ofd.FileName;
                fs._path = full.Replace(fs._excute, "");

                fs._files.Add(fs._excute);

                string[] datas = Directory.GetFiles(fs._path, "*.mpa", SearchOption.TopDirectoryOnly);
                foreach (string data in datas) fs._files.Add(data.Replace(fs._path, ""));

                datas = Directory.GetFiles(fs._path, "*.vec", SearchOption.TopDirectoryOnly);
                foreach (string data in datas) fs._files.Add(data.Replace(fs._path, ""));

                foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
                {
                    slot.StatType = STAT_TYPE.LOADING;
                    slot._fileSet = MConv.ObjectDeepCopy(fs);
                    slot._isDBLoad = false;
                    // clear somthing

                    loadFileSet(slot);
                }
			}
            else if (bt.Name == "xBT_ReLoad")
            {
                foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
                {

                    if (slot._fileSet == null) {
                        Global.gTestVM.addSysLog(LOG_TYPE.ERROR, slot.Name, "[RELOAD] Not Exist Reload File! Load First");
                        slot.StatType = STAT_TYPE.INSERT;
                        return;
                    }

                    slot.StatType = STAT_TYPE.LOADING;
                    slot._fileSet._sendIndex = 0;
                    loadFileSet(slot);
                    

                }
            }
            else if (bt.Name == "xBT_Con")
            {
                int conCount = 0;
                string unit = "";
                foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
                {
                    // db Load
                    if (slot._isDBLoad && slot.LotID != "")
                    {

                        if (slot.StatType == STAT_TYPE.RUNNING || slot.StatType == STAT_TYPE.CONTACTING)
                            continue;

                        slot.initTest();
                        slot._timerRunCheck.Start();

                        slot.StatType = STAT_TYPE.CONTACTING;
                        string file = "/star/pgm/" + slot._fileSet._excute;
                        Global.gNet._client.sendAuthority(slot.Name, file);
                        Thread.Sleep(10);
                        string runCmd = string.Format($"{file} -net -con -lot={LotID}");
                        
                        
                        slot.StartTime = DateTime.Now.ToString();
                        slot.EndTime = "";
                        slot._contact = true;

                        slot.startPowerLog();
                        slot.endSlotLog();

                        Global.gNet._client.sendPgmRun(slot.Name, runCmd);
                        unit += slot._num.ToString() + " ";
                        conCount++;

                    }
                }

                if (conCount == 0)
                {
                    LotID = DateTime.Now.ToString("yyyyMMdd_HHmmss");

                    LotWin.Left = Global.gMainWin.Left + 300;
                    LotWin.Top = Global.gMainWin.Top + 500;
                    LotWin.Show();
                }else
                {
                    Global.gTestVM.addSysLog(LOG_TYPE.INFO, "SLOTS", "[CONTACT] " + unit);
                    if (conCount != Global.gSlotVM.SelectSlots.Count)
                        Global.gTestVM.addSysLog(LOG_TYPE.WARNING, "SLOTS", "[CONTACT] Not All Contacted(Different loading Type)");
                }


                
            }
            else if (bt.Name == "xBT_Test")
            {
                if (checkBeforeRun()) return;

                foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
                {
                    if (slot.StatType == STAT_TYPE.RUNNING || slot.StatType == STAT_TYPE.CONTACTING)
                        continue;

                    slot._timerRunCheck.Start();
                    
                    slot._contact = false;
                    slot.StatType = STAT_TYPE.RUNNING;
                    string file = slot._fileSet._excute;
                    
                    string runCmd = string.Format($"/star/pgm/{file} -net -run");                    
                    Global.gNet._client.sendPgmRun(slot.Name, runCmd);
                }
                Global.gTestVM.addSysLog(LOG_TYPE.INFO, "SLOTS", "[TEST] " + units);

            }
            else if (bt.Name == "xBT_Stop")
            {
                foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
                {
                    if (slot.StatType == STAT_TYPE.LOADING)
                    {                        
                        slot._sendFileProcess.stop();
                        slot.StatType = STAT_TYPE.LOAD;
                    }else if (slot.StatType == STAT_TYPE.CONTACTING 
                        || slot.StatType == STAT_TYPE.RUNNING)
                    {
                        Global.gNet._client.sendPgmStop(slot.Name);
                        slot.StatType = STAT_TYPE.ABORT;
                        Global.gTestVM.addSlotLog(slot, "[User-Stop] PGM", LOG_TYPE.WARNING);

                    }else if (slot.StatType == STAT_TYPE.DIAG)
                    {
                        Global.gNet._client.sendDiagStop(slot.Name);                        

                    }

                }
            }
            else if (bt.Name == "xBT_Step")
            {
                if (Global.gSlotVM.CurSlot == null) return;
                SelSteps.Clear();
                if (Global.gSlotVM.CurSlot._stepList.Count == 0) return;
                foreach (string name in Global.gSlotVM.CurSlot._stepList)
                {
                    SelSteps.Add(name);
                }
                StepSelect dlg = new StepSelect();
                dlg.Left = Global.gMainWin.Left + 460;
                dlg.Top = Global.gMainWin.Top + 400;

                dlg.ShowDialog();
                if (dlg.DialogResult == false) return;

                foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
                {
                    slot._contact = false;
                    slot.StatType = STAT_TYPE.RUNNING;
                    string file = slot._fileSet._excute;

                    string runCmd = string.Format($"/star/pgm/{file} -net -run -single={dlg._selects}");
                    Global.gNet._client.sendPgmRun(slot.Name, runCmd);
                }
    
                Global.gTestVM.addSysLog(LOG_TYPE.INFO, "SLOTS", "[STEP] " + units);
            }
            else if (bt.Name == "xBT_Diag")
            {
                if (Global.gSlotVM.CurSlot == null) return;
                Diag dlg = new Diag();
                dlg.Left = Global.gMainWin.Left + 480;
                dlg.Top = Global.gMainWin.Top + 550;
                dlg.ShowDialog();
                if (dlg.DialogResult == false) return;

                foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
                {
                    slot._contact = false;
                    slot.StatType = STAT_TYPE.DIAG;
                    string runCmd = string.Format($"/star/diag/diag -diag -net -darg={Convert.ToString(dlg._diagFlag, 16)}");
                    Global.gNet._client.sendDiagRun(slot.Name, runCmd);
                }

                Global.gTestVM.addSysLog(LOG_TYPE.INFO, "SLOTS", "[DIAG] Excute " + units );
			}
            else if (bt.Name == "xBT_Reset")
            {
                foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
                {
                    if (slot.PrevStat == STAT_TYPE.CONTACTING)
                        slot.StatType = STAT_TYPE.LOAD;
                    else if (slot.PrevStat == STAT_TYPE.RUNNING)
                        slot.StatType = STAT_TYPE.CONTACT;
                    else
                        slot.StatType = STAT_TYPE.CONNECT;

                }
                    
            }

        }
        #endregion Command SET
    }
}
