using Serilog;
using Serilog.Core;
using StarBase;
using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using StarBase.Net;
using System.Collections.Generic;

namespace StarOS.Model {
    public enum STAT_TYPE : int
    {
        DISCONNECT = 0,
        CONNECT,
        INSERT,
        LOAD,
        CONTACT,
        STOP,
        MASK,
        LOADING,
        CONTACTING,
        RUNNING,
        ABORT,
        ERROR,
        DIAG,
        TYPE_COUNT
    }

    public enum STEP_TYPE : int
    {
        NORMAL = 0, 
        ALL_SCAN,
        SOAK_TIME,
        AGING
    }

    public class FileSet
    {
        public FileSet() { _sendIndex = 0; }


        public string   _excute;
        public string   _path;
        public Collection<string> _files = new Collection<string>();
        public int _sendIndex;
    }
        

    public class SlotDef
    {
        public SlotDef(BitmapImage image, String text, SolidColorBrush brush)
        {
            Image = image;
            Text = text;
            Brush = brush;
        }

        public BitmapImage  Image;
        public string       Text;
        public SolidColorBrush Brush;
    }

    public class SlotModel : BaseVM
    {
        /// <summary>
        /// ///////////////////////////////////////////////////////
        /// </summary>
        #region UI 연동 Property

        public bool _isOnline = false;
        public int _num = 0;
        public string _ip = "";

        BitmapImage _icon;
        public BitmapImage Icon
        {
            get => _icon;
            set
            {
                _icon = value;
                OnPropertyChanged("Icon");
            }
        }

        string _name;
        public string Name
        {
            get => _name;
            set
            {
                _name = value;
                OnPropertyChanged("Name");
            }
        }


        public bool _isParityOK = true;
        public bool IsParityOK
        {
            get => _isParityOK;
            set
            {
                _isParityOK = value;
                if (IsParityOK)
                    BibStatBrush = Global.gStatDefs[(int)STAT_TYPE.CONNECT].Brush;
                else
                    BibStatBrush = Global.gStatDefs[(int)STAT_TYPE.ERROR].Brush;
            }
        }


        public string _maskArg = "";
        public List<bool> _maskList = new List<bool>();

     
        
        public uint _numBid = 0;

        string _bid;
        public string BID
        {
            get => _bid;
            set
            {
                string sbid = value;

                if (!(sbid == "" || sbid == "0x0"))
                {
                    sbid = "";
                    _numBid = (uint)Convert.ToInt32(value, 16);

                    IsParityOK = true;
                    for (int i = 0; i < 4; i++) {
                        int sum = 0;
                        for (int j = 0; j < 8; j++) {
                            uint group = (_numBid >> (j * 4)) & 0xF;
                            if ((group & (1 << i)) != 0) sum++;
                        }
                        if (sum % 2 == 1) {
                            IsParityOK = false;
                            break;
                        }
                    }

                    uint nSerial = _numBid & 0xffff;
                    uint nBall = (_numBid >> 16) & 0xf;
                    uint nFunc = (_numBid >> 20) & 0xf;
                    uint nDevFam = (_numBid >> 24) & 0x7;
                    uint nEquip = (_numBid >> 27) & 0x1; // 0: ST805, 1: ST840
                    if (nDevFam == 7 && nFunc == 5 && nBall == 3) // 753  no parity check
                        IsParityOK = true;
                    else if (nSerial == 0x1234)
                        IsParityOK = true;


                    if (!IsParityOK)
                    {
                        sbid = string.Format("P-Err{0:X8}", _numBid);
                    } 
                    else 
                    {


                        if (Global.IsUseBibMap)
                        {

                            if (Global.BidFuncMap.ContainsKey((int)nFunc))
                                sbid += Global.BidFuncMap[(int)nFunc];
                            else
                                sbid += "---"; sbid += " ";

                            if (Global.BidDevMap.ContainsKey((int)nDevFam))
                                sbid += Global.BidDevMap[(int)nDevFam];
                            else
                                sbid += "---"; sbid += " ";

                            if (Global.BidBallMap.ContainsKey((int)nBall))
                                sbid += Global.BidBallMap[(int)nBall];
                            else
                                sbid += "---"; sbid += " ";

                            sbid += string.Format(": {0}", Convert.ToString(nSerial, 16).PadLeft(4, '0'));
                        }
                        else
                        {
                            sbid = string.Format("{0:X8}", _numBid);
                        }
                    }
                    if (StatType == STAT_TYPE.CONNECT)
                        StatType = STAT_TYPE.INSERT;
                } else
                {
                    if (StatType >= STAT_TYPE.INSERT )
                        StatType = STAT_TYPE.CONNECT;
                    sbid = "";
                }


                _bid = sbid;
                OnPropertyChanged("BID");
            }
        }


        string _stat;
        public string StatText
        {
            get => _stat;
            set
            {
                _stat = value;
                OnPropertyChanged("StatText");
            }
        }


        SolidColorBrush _bibStatBrush;
        public SolidColorBrush BibStatBrush
        {
            get => _bibStatBrush;
            set
            {
                _bibStatBrush = value;
                OnPropertyChanged("BibStatBrush");
            }
        }
        SolidColorBrush _statBrush;
        public SolidColorBrush StatBrush
        {
            get => _statBrush;
            set
            {
                _statBrush = value;
                OnPropertyChanged("StatBrush");
            }
        }

        SolidColorBrush _conBrush;
        public SolidColorBrush ConBrush
        {
            get => _conBrush;
            set
            {
                _conBrush = value;
                OnPropertyChanged("ConBrush");
            }
        }

        int _runPrint;
        public int RunPrint
        {
            get => _runPrint;
            set
            {
                _runPrint = value;
                OnPropertyChanged("RunPrint");
            }
        }

        public void initTest()
        {
            StartTime = "";
            EndTime = "";
            MeThod = "";  
            if (_isDBLoad == false)
                LotID = "";
            PgmName = "";
            StepName = "";
            _scanProc = 0;

            _steps.Clear();
            _stepList.Clear();           

            _dutModels.Clear();
            _dutDatas.Clear();

            _binGathering.Clear();

            if (Global.gMainVM.checkCurSlot(Name)) {
                Global.gMainVM.updateUIFlag((int)UI.INFO | (int)UI.SLOTLOG
                    | (int)UI.STEPLIST | (int)UI.POWER | (int)UI.RESULT);                
            }


             
        }


        public STAT_TYPE PrevStat = STAT_TYPE.DISCONNECT;         
        STAT_TYPE _statType;
        public STAT_TYPE StatType
        {
            get => _statType;
            set
            {
                PrevStat = _statType;
                
                Global.gMainVM.checkUpdateUI((int)UI.MENU, Name, _statType, value);

                _statType = value;

                if (_statType == STAT_TYPE.CONNECT || _statType == STAT_TYPE.INSERT)
                {                    
                    if (BID == "") _statType = STAT_TYPE.CONNECT;
                    else _statType = STAT_TYPE.INSERT;
                    Global.gMainVM.checkUpdateUI((int)UI.MENU, Name, PrevStat, _statType);
                }else if (_statType == STAT_TYPE.LOAD)
                {
                    initTest();
                }

                SlotDef def = Global.gStatDefs[(int)_statType];
                StatText = def.Text;
                StatBrush = def.Brush;
                Icon = def.Image;

                if (_statType == STAT_TYPE.RUNNING)
                {
                    RunPrint = 70;
                    ConBrush = def.Brush;
                }
                else if (_statType == STAT_TYPE.CONTACTING || _statType == STAT_TYPE.DIAG)
                {
                    RunPrint = 60;
                    ConBrush = def.Brush;
                }
                else
                {
                    if (_statType == STAT_TYPE.ABORT || _statType == STAT_TYPE.ERROR)
                        _timerAging.Stop();

                    RunPrint = 0;
                }

            }
        }

        public bool _isDBLoad = false;


        public string _lotID = "";
        public string LotID
        {
            get { return _lotID; }
            set
            {
                Global.gMainVM.checkUpdateUI((int)UI.INFO, Name, _lotID, value);
                _lotID = value;
            }
        }

        public string _startTime = "";
        public string StartTime
        {
            get { return _startTime; }
            set
            {
                Global.gMainVM.checkUpdateUI((int)UI.INFO, Name, _startTime, value);
                _startTime = value;
            }
        }

        public string _endTime = "";
        public string EndTime
        {
            get { return _endTime; }
            set
            {
                Global.gMainVM.checkUpdateUI((int)UI.INFO, Name, _endTime, value);
                _endTime = value;
            }
        }


        public string _pgmName = "";
        public string PgmName
        {
            get { return _pgmName; }
            set
            {
                Global.gMainVM.checkUpdateUI((int)UI.INFO, Name, _pgmName, value);
                _pgmName = value;
            }
        }

        public string _stepName = "";
        public string StepName
        {
            get { return _stepName; }
            set
            {
                Global.gMainVM.checkUpdateUI((int)UI.INFO, Name, _stepName, value);
                string prev = _stepName;
                _stepName = value;

                if (Global.gMainVM.checkCurSlot(Name))
                {
                    if (prev != value)
                        Global.gMainVM.setStepIndex();
                }
            }
        }

        public int _agingCnt;
        public int _agingTime;
        public DispatcherTimer _timerAging = new DispatcherTimer(); // Aging, Sock Progress Timer
        void agingTime_tick(object sender, EventArgs e)
        {
            if (!(StepType == STEP_TYPE.AGING ||
                StepType == STEP_TYPE.SOAK_TIME)) return;

            _agingCnt += 3;
            if (_agingTime <= 0 || _agingCnt >= _agingTime)
            {
                _timerAging.Stop();
                return;
            }

            _scanProc = _agingCnt * 100 / _agingTime;

            if (Global.gMainVM.checkCurSlot(Name))
                Global.gTestVM.ScanProg = _scanProc;
        }
        
        public STEP_TYPE _stepType = STEP_TYPE.NORMAL;
        public STEP_TYPE StepType
        {
            get { return _stepType; }
            set { 

                _stepType = value; 
                MeThod = _stepType.ToString();
            }
        }

        public string _method = "";
        public string MeThod
        {
            get { return _method; }
            set
            {
                Global.gMainVM.checkUpdateUI((int)UI.INFO, Name, _method, value);
                _method = value;
            }
        }

        #endregion UI 연동 Property

        /// <summary>
        #region     Slot Class 내부 Data 처리용
        /// </summary>

        public PLCModel _plcModel = null;
        public string _diagPath = "";

        public SlotModel(string id)
        {
            Name = id;
            _num = Convert.ToInt32(Name.Substring(4));
            StatType = STAT_TYPE.DISCONNECT;

            _sendFileProcess = new SendFileProcess();
            _sendFileProcess.Client = Global.gNet._client;
            _sendFileProcess.timeOver += sendFileTimeOver;
            _logStep = null;
            _logSlot = null;

            _timerRunCheck.Interval = TimeSpan.FromSeconds(10);
            _timerRunCheck.Tick += runChecktime_tick;

            _timerAging.Interval = TimeSpan.FromSeconds(3);
            _timerAging.Tick += agingTime_tick;

            BibStatBrush = Global.gStatDefs[(int)STAT_TYPE.CONNECT].Brush;

            initPowerData();

        }        

        public DispatcherTimer _timerRunCheck = new DispatcherTimer();
        void runChecktime_tick(object sender, EventArgs e)
        {
            StatType = STAT_TYPE.ERROR;            
            _timerRunCheck.Stop();
        }


        public FileSet _fileSet;
        public string getSendFile()
        {
            if (_fileSet._files.Count > _fileSet._sendIndex)
                return _fileSet._files[_fileSet._sendIndex];
            return "";
        }
        public bool isSendFileRemind()
        {
            return (_fileSet._files.Count > _fileSet._sendIndex);
        }

        public SendFileProcess _sendFileProcess;
        public void sendFileTimeOver()
        {
            _sendFileProcess.stop();
            StatType = STAT_TYPE.ERROR;
            string msg = "File Send Timer Error";
            
            if (_fileSet._files.Count > _fileSet._sendIndex)
                msg += string.Format(" \"{0}\"", _fileSet._files[_fileSet._sendIndex]);
            Global.gTestVM.addSysLog(LOG_TYPE.ERROR, Name, msg);
        }

        
        public bool _contact = false;  /// Test Type  0 = run, 1= contact
        public bool _endTest = true;   //  test 종료시 end-test msg받았는지 여부
        public int _scanProc = 0;      //  scan 진행 index



        // 화면 하단 출력용
        public Collection<string> _stepList = new Collection<string>();


        #endregion

        ///
        #region Result 처리
        public BIBModel _layout = new BIBModel();

        public ObservableCollection<DutData> _dutDatas = new ObservableCollection<DutData>();//data 요소
        public ObservableCollection<DutModel> _dutModels = new ObservableCollection<DutModel>(); // UI 연동 요소
        public ObservableCollection<StepModel> _steps = new ObservableCollection<StepModel>();

        public Dictionary<int, int> _binGathering = new Dictionary<int, int>();

        public StepModel findStep(string name)
        {
              return _steps.FirstOrDefault(a => a.Name == name); ;
        }
        
        #endregion
        ///

        /// <summary>
        #region Power 처리
        /// </summary>
        /// 
        public void  clearPowerTalble()
        {
            for (int i = 0; i< 5; i++)
            {                
                _DPSs[i].GetV = string.Format("{0,5}","-  V");;
                _DPSs[i].GetC = string.Format("{0,5}","-  A");;
                _DPSs[i].SetV = string.Format("{0,5}","-  V");;
                _DPSs[i].OC =   string.Format("{0,5}","-  A");;
            }


            for (int i = 0; i< 3; i++)
            {
                _PEs[i].IOV   = string.Format("{0,5}", "-  V");
                _PEs[i].CLKV  = string.Format("{0,5}", "-  V");
                _PEs[i].DRV   = string.Format("{0,5}", "-  V");
                _PEs[i].ScanV = string.Format("{0,5}", "-  V");
            }
        }
        
        void initPowerData()
        {
            for (int i = 0; i<5; i++)
                _DPSs[i] = new DPSModel();
            for (int i = 0; i < 3; i++)
                _PEs[i] = new PEModel();
            clearPowerTalble();
        }


        public bool _pwrMon = false;
        public bool PwrMon
        {
            get { return _pwrMon; }
            set {                 
                _pwrMon = value;
                if (_pwrMon == false) clearPowerTalble(); 
            }

        }
        public int _pwrGraphIndex = 0;        

        public DPSModel[] _DPSs = new DPSModel[5];
        public PEModel[]  _PEs = new PEModel[3];

        #endregion

        /// <summary>
        #region LOG 처리 
        /// </summary>


        public Logger _logStep = null;
        public Logger _logPower = null;
        public Logger _logSlot = null;

        public void startSlotLog()
        {
            if (_logSlot != null)
                _logSlot.Dispose();

            string file = String.Format($"{Global.gDirSlot}\\{LotID}_{Name}.log");
            _logSlot = new LoggerConfiguration()
                //.WriteTo.Console()                
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite,
                outputTemplate: "{Timestamp:[yyyy-MM-dd HH:mm:ss]}{Message:lj}{NewLine}")
                .CreateLogger();
        }

        public void addSlotLog(SlotLog log)
        {
            _logSlotQue.Add(log);
            if (_logSlotQue.Count > 100)
                _logSlotQue.RemoveAt(0);

            string inner = string.Format($"[{log.Step}]{log.Message}");
           
            if (_logSlot == null) startSlotLog();
            if (_logSlot == null) return;

            _logSlot.Information(inner);
        }

        public void endSlotLog()
        {
            if (_logSlot == null)
                return;
            _logSlot.Dispose();
            _logSlot = null;
            _logSlotQue.Clear();
//            Global.gMainVM.updateUIFlag((int)UI.SLOTLOG);
        }

        public void startPowerLog()
        {
            if (_logPower != null)
                _logPower.Dispose();

            if (LotID == "") return;

            //Console.WriteLine($"StartPowerLog {LotID}_{Name}.power");

            string file = String.Format($"{Global.gDirPower}\\{LotID}_{Name}.power");
            _logPower = new LoggerConfiguration()
                //.WriteTo.Console()                
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite,
                outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();
        }

        public void setPowerLogStep(string name, bool start = true)
        {
            if (_logPower == null) startPowerLog();
            if (_logPower == null) return;

            string msg = string.Format($"{(start ? "start" : "end")} step \'{name}\'");
            _logPower.Information(msg);
        }

        public void addPowerLog()
        {
            if (_logPower == null) startPowerLog();
            if (_logPower == null) return;
            

            string data = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
            data += "\t\t";
            for (int i = 0; i<5; i++)
            {                
                data += string.Format($"\t{_DPSs[i].SetV}\t{_DPSs[i].GetV}\t{_DPSs[i].OC}\t{_DPSs[i].GetC}");
            }

            for (int i = 0; i<3; i++)
            {
                data += string.Format($"\t{_PEs[i].IOV}\t{_PEs[i].CLKV}\t{_PEs[i].DRV}\t{_PEs[i].ScanV}");
            }

            double cur = 0;
            double target = 0;
            if (_plcModel != null) {
                cur = _plcModel.CurTemp;
                target = _plcModel.TargetTemp;  
            }

            data += string.Format("\t{0:F1}\t{1:F1}", cur, target);
            _logPower.Information(data);
        }

        public void endPowerLog()
        {
            if (_logPower == null)
                return;
            _logPower.Dispose();
            _logPower = null;
        }

        // TestInfo View ListView 출력용
        public Collection<SlotLog> _logSlotQue = new Collection<SlotLog>();
        

        #endregion
    }

}

