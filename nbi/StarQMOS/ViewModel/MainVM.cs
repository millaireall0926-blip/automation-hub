using HandyControl.Controls;
using Microsoft.Win32;
using StarBase;
using StarQMOS.Model;
using StarQMOS.View;
using System;
using System.Collections.ObjectModel;
using System.IO;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Threading;

namespace StarQMOS.ViewModel
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
        public Result ResultWin;
        public Visibility ShowLogo { get; set; } = Visibility.Visible;
        string _lotID;
        public string LotID
        {
            get { return _lotID; }
            set { _lotID = value; OnPropertyChanged("LotID"); }
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

        string _handlerText = "NO HANDLER";
        public string HandlerText
        {
            get => _handlerText;
            set { _handlerText = value; OnPropertyChanged(nameof(HandlerText)); }
        }

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
            if (Global.gShowLogo) { ShowLogo = Visibility.Visible; }
            else { ShowLogo = Visibility.Hidden; }
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
        }

            public void updateUIFlag(int ui)
        {
            _uiUpdateFlag |= ui;
        }

        public static void threadUI(object parent)
        {
            MainVM vm = parent as MainVM;

            bool doPriority2 = true;
            int flagPriority1 = 0;
            int flagPriority2 = 0;
            while (vm._uiThreadClose == false)
            {
                flagPriority1 = vm._uiUpdateFlag & 0xff;
                flagPriority2 = vm._uiUpdateFlag & 0xf00;

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
                    case "xBT_Load":
                    case "xBT_ReLoad":
                    case "xBT_Con":
                    case "xBT_Test":
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

        public void initDps()
        {
            if (Global.gSlotVM.Slots.Count == 0)
            {
                Global.gTestVM.addSysLog(LOG_TYPE.ERROR, "Router", $"Not Ready to Router. Check StarRouter Conncetion");
                return;
            }
            LotID = DateTime.Now.ToString("yyyyMMdd_HHmmss");
            SlotModel slot = Global.gSlotVM.Slots[0];
            if (slot.StatType == STAT_TYPE.RUNNING || slot.StatType == STAT_TYPE.CONTACTING)
                return;

            slot.initTest();
            slot._timerRunCheck.Start();

            slot.StatType = STAT_TYPE.CONTACTING;
            string file = "/star/pgm/" + slot._fileSet._excute;
            Global.gNet._client.sendAuthority(slot.Name, file);
            string runCmd = string.Format($"{file} -net -init");

            slot.LotID = LotID;
            slot.StartTime = DateTime.Now.ToString();
            slot.EndTime = "";
            slot._contact = true;

            slot.startPowerLog();
            slot.endSlotLog();

            Global.gNet._client.sendPgmRun(slot.Name, runCmd);

            Global.gHandlerVM.inited(true);

            
            Global.gTestVM.addSysLog(LOG_TYPE.INFO, "SLOTS", "[INIT-HANDLE] " + slot.Name);

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

            if (bt.Name == "xBT_Load")
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

                    // clear somthing

                    loadFileSet(slot);
                }
                Global.gHandlerVM.addBinLog($"{fs._excute}");
                Global.gHandlerVM.loaded(true);

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
                LotID = DateTime.Now.ToString("yyyyMMdd_HHmmss");
                foreach (SlotModel slot in Global.gSlotVM.SelectSlots)
                {
                    if (slot.StatType == STAT_TYPE.RUNNING || slot.StatType == STAT_TYPE.CONTACTING)
                        continue;

                    slot.initTest();
                    slot._timerRunCheck.Start();

                    slot.StatType = STAT_TYPE.CONTACTING;
                    string file = "/star/pgm/" + slot._fileSet._excute;
                    Global.gNet._client.sendAuthority(slot.Name, file);
                    string runCmd = string.Format($"{file} -net -init");

                    slot.LotID = LotID;
                    slot.StartTime = DateTime.Now.ToString();
                    slot.EndTime = "";
                    slot._contact = true;

                    slot.startPowerLog();
                    slot.endSlotLog();

                    Global.gNet._client.sendPgmRun(slot.Name, runCmd);

                    Global.gHandlerVM.inited(true);
                    
                }
                Global.gTestVM.addSysLog(LOG_TYPE.INFO, "SLOTS", "[INIT-User] " + units);
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
                    Global.gNet._client.sendAuthority(slot.Name, file);
                    string runCmd = string.Format($"/star/pgm/{file} -net -handler");

                    Global.gNet._client.sendPgmRun(slot.Name, runCmd);
                }
                Global.gTestVM.addSysLog(LOG_TYPE.INFO, "SLOTS", "[HANDLER] " + units);

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
                    Global.gNet._client.sendAuthority(slot.Name, file);
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
