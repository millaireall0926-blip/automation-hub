using HandyControl.Controls;
using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.Model;
using Serilog;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;
using MessageBox = HandyControl.Controls.MessageBox;

namespace RDimm_MMTCRACK.ViewModel
{
    public class MotorLog : BaseVM
    {
        public string Time { get; set; }
        public string Message { get; set; }
        public MotorLog(string msg)
        {
            Time = DateTime.Now.ToString("yyyy-MM-dd H:mm:ss");
            Message = msg;
        }
    }
    public class ParaLog : BaseVM
    {
        public string Time { get; set; }
        public string Message { get; set; }
        public ParaLog(string msg)
        {
            Time = DateTime.Now.ToString("yyyy-MM-dd H:mm:ss");
            Message = msg;
        }
    }
    public class EngineerVM :BaseVM
    {
        ObservableCollection<EngineerModel> _funcdatas;
        public ObservableCollection<EngineerModel> Funcdatas
        {
            get => _funcdatas;
            set
            {
                _funcdatas = value;
                PC(nameof(Funcdatas));
            }
        }
        ObservableCollection<EngineerModel> _setpointdatas;
        public ObservableCollection<EngineerModel> SetPointdatas
        {
            get => _setpointdatas;
            set
            {
                _setpointdatas = value;
                PC(nameof(SetPointdatas));
            }
        }
        ObservableCollection<EngineerModel> _inoutdatas;
        public ObservableCollection<EngineerModel> InOutdatas
        {
            get => _inoutdatas;
            set
            {
                _inoutdatas = value;
                PC(nameof(InOutdatas));
            }
        }
        public int SelectedIndex { get; set; }
        public int uiPbint { get; set; } = 0;
        public ListView ListParaLog;
        ObservableCollection<ParaLog> _paraLogs = new ObservableCollection<ParaLog>();
        public ObservableCollection <ParaLog> ParaLogs { get { return _paraLogs; } set { _paraLogs = value; PC("ParaLogs"); } }
        public ListView ListMotorLog;
        ObservableCollection<MotorLog> _motorlogs = new ObservableCollection<MotorLog>();
        public ObservableCollection<MotorLog> MotorLogs { get { return _motorlogs; } set { _motorlogs = value; PC("MotorLogs"); } }
        ObservableCollection<ManMotorModel> _manualmotor = new ObservableCollection<ManMotorModel>();
        public ObservableCollection<ManMotorModel> ManualMotor { get { return _manualmotor; } set { _manualmotor = value; PC("ManualMotor"); } }
        public DelegateCommand ParaDeFaultSet { get; set; }
        public DelegateCommand ParaValueRead { get; set; }
        public DelegateCommand ParaValueWrite { get; set; }
        public BackgroundWorker _ReadParaWorker = null;
        public BackgroundWorker _WriteParaWorker = null;

        public int TconWriteCnt { get; set; } = 0;

        public string caputretime { get; set; } = string.Empty;
        public int TconReadCnt { get; set; } = 0;

        public EngineerVM()
        {
            foreach(RackModel rm in Global.gMainPageVM.Racks)
            {
                ManualMotor.Add(new ManMotorModel(rm.RackViewIndex, rm));
            }
            Global.gEngineerVM = this;

            Funcdatas = new ObservableCollection<EngineerModel>
            {
                new EngineerModel("운전,정지","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("파워모드","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("외부접점","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("표시상한","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("표시하한","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
            };
            SetPointdatas = new ObservableCollection<EngineerModel>
            {
                new EngineerModel("SP설정1","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("SP설정2","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("SP설정3","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("SP설정4","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("SP상한값","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("SP하한값","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
            };
            InOutdatas = new ObservableCollection<EngineerModel>
            {
                new EngineerModel("입력종류","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("입력범위상한","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("입력범위하한","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("입력구간보정1","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("입력구간보정2","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("입력구간보정3","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("PV치 보정값1","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("PV치 보정값2","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("PV치 보정값3","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("보조출력단자1","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("보조출력단자2","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
                new EngineerModel("보조출력단자3","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"),
            };
            ParaDeFaultSet = new DelegateCommand(p => { Task.Run(() => DeParaSetValue()); });
            ParaValueRead = new DelegateCommand(p => { Task.Run(() => PreReadParaValue()); });
            ParaValueWrite = new DelegateCommand(p => { Task.Run(() => PreWriteParaValue()); });
        }
        public void DeParaSetValue()
        {
            if (Global.gMainVM.AllRackSTAT != TOTALRACK_STAT.STOP)
                return;

            if (SelectedIndex == 0)
            {
                Funcdatas[0].ValueSet("1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1");
                Funcdatas[1].ValueSet("1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1");
                Funcdatas[2].ValueSet("1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1");
                Funcdatas[3].ValueSet("160.5", "160.5", "160.5", "160.5", "160.5", "160.5", "160.5", "160.5", "160.5", "160.5", "160.5", "160.5", "160.5", "160.5", "160.5", "160.5", "160.5", "160.5");
                Funcdatas[4].ValueSet("-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5", "-70.5");
            }
            else if(SelectedIndex == 1)
            {
                SetPointdatas[0].ValueSet("30", "30", "30", "30", "30", "30", "30", "30", "30", "30", "30", "30", "30", "30", "30", "30", "30", "30");
                SetPointdatas[1].ValueSet("-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60");
                SetPointdatas[2].ValueSet("-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60");
                SetPointdatas[3].ValueSet("-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60");
                SetPointdatas[4].ValueSet("150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150");
                SetPointdatas[5].ValueSet("-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60");
            }
            else if(SelectedIndex == 2)
            {
                InOutdatas[0].ValueSet("2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2");
                InOutdatas[1].ValueSet("150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150", "150");
                InOutdatas[2].ValueSet("-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60", "-60");
                InOutdatas[3].ValueSet("5", "5", "5", "5", "5", "5", "5", "5", "5", "5", "5", "5", "5", "5", "5", "5", "5", "5");
                InOutdatas[4].ValueSet("25", "25", "25", "25", "25", "25", "25", "25", "25", "25", "25", "25", "25", "25", "25", "25", "25", "25");
                InOutdatas[5].ValueSet("80", "80", "80", "80", "80", "80", "80", "80", "80", "80", "80", "80", "80", "80", "80", "80", "80", "80");
                InOutdatas[6].ValueSet("3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3");
                InOutdatas[7].ValueSet("0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0");
                InOutdatas[8].ValueSet("3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3");
                InOutdatas[9].ValueSet("6", "6", "6", "6", "6", "6", "6", "6", "6", "6", "6", "6", "6", "6", "6", "6", "6", "6");
                InOutdatas[10].ValueSet("2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2", "2");
                InOutdatas[11].ValueSet("3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3", "3");
            }
        }
        public void PreReadParaValue()
        {
            if (Global.gMainVM.AllRackSTAT != TOTALRACK_STAT.STOP)
                return;

            _ReadParaWorker = new BackgroundWorker();
            _ReadParaWorker.WorkerReportsProgress = true;
            _ReadParaWorker.WorkerSupportsCancellation = true;
            _ReadParaWorker.DoWork += ReadParaValue;
            _ReadParaWorker.ProgressChanged += _ReadWorker_Progresschanged;
            _ReadParaWorker.RunWorkerAsync();
        }
        private void _ReadWorker_Progresschanged(object sender, ProgressChangedEventArgs e)
        {
            uiPbint = e.ProgressPercentage;
        }
        public int writereadindex = 0;
        public void ReadParaValue(object sender, DoWorkEventArgs e)
        {
            if (Global.gMainVM.AllRackSTAT == TOTALRACK_STAT.STOP)
            {
                try
                {

                    _ReadParaWorker.ReportProgress(0);
                    Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.PARAMETER;
                    int i = 0;
                    writereadindex = SelectedIndex; //read Tap Index Check

                    Thread.Sleep(100);
                    if (SelectedIndex == 0)
                    {
                        for (i = 0; i < 5; i++)
                            Funcdatas[i].ValueClear();

                        for (i = 1; i < 19; i++)
                        {
                            Global.gTconCom.ParaReadMsg((byte)i, SelectedIndex);
                            _ReadParaWorker.ReportProgress(i * 5);
                            Thread.Sleep(100);
                        }
                    }
                    else if (SelectedIndex == 1)
                    {
                        for (i = 0; i < 6; i++)
                            SetPointdatas[i].ValueClear();

                        for (i = 1; i < 19; i++)
                        {
                            Global.gTconCom.ParaReadMsg((byte)i, SelectedIndex);
                            _ReadParaWorker.ReportProgress(i * 5);
                            Thread.Sleep(100);
                        }
                    }
                    else if (SelectedIndex == 2)
                    {
                        for (i = 0; i < 12; i++)
                            InOutdatas[i].ValueClear();

                        for (i = 1; i < 19; i++)
                        {
                            Global.gTconCom.ParaReadMsg((byte)i, SelectedIndex);
                            _ReadParaWorker.ReportProgress(i * 5);
                            Thread.Sleep(100);
                        }
                    }
                    Thread.Sleep(1000);
                    _ReadParaWorker.ReportProgress(100);
                    Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.STOP;
                }
                catch
                {
                    if (_ReadParaWorker != null)
                        _ReadParaWorker.CancelAsync();

                    addParaLog("Read Error Retry");
                }
            }
            else
            {
                MessageBox.Show("장비 RUN 중에는 파라메터 변경 금지");
                return;
            }
        }
        public void PreWriteParaValue()
        {
            if (Global.gMainVM.AllRackSTAT != TOTALRACK_STAT.STOP)
                return;
            _WriteParaWorker = new BackgroundWorker();
            _WriteParaWorker.WorkerReportsProgress = true;
            _WriteParaWorker.WorkerSupportsCancellation = true;
            _WriteParaWorker.DoWork += WriteParaValue;
            _WriteParaWorker.ProgressChanged += _WriteWorker_Progresschanged;
            _WriteParaWorker.RunWorkerAsync();
        }
        private void _WriteWorker_Progresschanged(object sender, ProgressChangedEventArgs e)
        {
            uiPbint = e.ProgressPercentage;
        }
        public void WriteParaValue(object sender, DoWorkEventArgs e)
        {
            string[,] getFuncParaVals = new string[5,18];
            string[,] getSetpointParaVals = new string[6, 18];
            string[,] getinoutParaVals = new string[12, 18];
            string[] getParaVal = new string[18];
            ArrayList PreParaSend = new ArrayList();

            if (Global.gMainVM.AllRackSTAT == TOTALRACK_STAT.STOP)
            {
                try
                {
                    _WriteParaWorker.ReportProgress(0);
                    Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.PARAMETER;
                    int i, j, k = 0;

                    writereadindex = SelectedIndex; //read Tap Index Check

                    Thread.Sleep(100);

                    string tmp = string.Empty;

                    if (SelectedIndex == 0)
                    {
                        for (i = 0; i < 5; i++)
                        {
                            getParaVal = Funcdatas[i].sendVal();
                            for (j = 0; j < 18; j++)
                            {
                                getFuncParaVals[i, j] = getParaVal[j];
                            }
                        }
                        for (j = 0; j < 18; j++)
                        {
                            for (i = 0; i < 5; i++)
                            {
                                if (i <= 2)
                                    tmp = MConv.makebyte(getFuncParaVals[i, j]);
                                else
                                    tmp = MConv.makebyte_dob(getFuncParaVals[i, j]);

                                foreach (string s in tmp.Split(' '))
                                {
                                    if (s != null && s != "")
                                        PreParaSend.Add(Convert.ToByte(s, 16));
                                }
                                if (i == 0)//28EA
                                {
                                    for (k = 0; k < 28; k++)
                                        PreParaSend.Add((byte)0x00);
                                }
                                else if (i == 1) //42EA
                                {
                                    for (k = 0; k < 11; k++)
                                        PreParaSend.Add((byte)0x00);

                                    PreParaSend.Add((byte)0x0A);

                                    for (k = 0; k < 4; k++)
                                        PreParaSend.Add((byte)0x00);
                                    for (k = 0; k < 2; k++)
                                        PreParaSend.Add((byte)0x01);
                                    for (k = 0; k < 15; k++)
                                        PreParaSend.Add((byte)0x00);

                                    PreParaSend.Add((byte)0x0F);

                                    for (k = 0; k < 8; k++)
                                        PreParaSend.Add((byte)0x00);
                                }
                            }
                            string SentCnt = PreParaSend.Count.ToString("X");
                            byte TotalCnt = Convert.ToByte(SentCnt, 16);

                            Global.gTconCom.ParaWriteMsg((byte)(j + 1), TotalCnt, PreParaSend, 0);
                            _WriteParaWorker.ReportProgress(j * 5);
                            Thread.Sleep(100);
                            PreParaSend.Clear();
                        }
                    }
                    else if (SelectedIndex == 1)
                    {
                        for (i = 0; i < 6; i++)
                        {
                            getParaVal = SetPointdatas[i].sendVal();
                            for (j = 0; j < 18; j++)
                            {
                                getSetpointParaVals[i, j] = getParaVal[j];
                            }
                        }
                        for (j = 0; j < 18; j++)
                        {
                            for (i = 0; i < 6; i++)
                            {
                                tmp = MConv.makebyte_dob(getSetpointParaVals[i, j]);

                                foreach (string s in tmp.Split(' '))
                                {
                                    if (s != null && s != "")
                                        PreParaSend.Add(Convert.ToByte(s, 16));
                                }
                                if (i == 3)
                                {
                                    for (int loop = 0; loop < 2; loop++)
                                    {
                                        PreParaSend.Add((byte)0xFD);
                                        PreParaSend.Add((byte)0xA8);

                                        for (k = 0; k < 4; k++)
                                            PreParaSend.Add((byte)0x00);
                                    }
                                }
                            }
                            string SentCnt = PreParaSend.Count.ToString("X");
                            byte TotalCnt = Convert.ToByte(SentCnt, 16);

                            Global.gTconCom.ParaWriteMsg((byte)(j + 1), TotalCnt, PreParaSend, 1);
                            _WriteParaWorker.ReportProgress(j * 5);
                            Thread.Sleep(100);
                            PreParaSend.Clear();
                        }
                    }
                    else if (SelectedIndex == 2)
                    {
                        for (i = 0; i < 12; i++)
                        {
                            getParaVal = InOutdatas[i].sendVal();
                            for (j = 0; j < 18; j++)
                            {
                                getinoutParaVals[i, j] = getParaVal[j];
                            }
                        }
                        for (j = 0; j < 18; j++)
                        {
                            for (i = 0; i < 12; i++)
                            {
                                if (i == 0 || i >= 9)
                                    tmp = MConv.makebyte(getinoutParaVals[i, j]);
                                else if (i > 0 && i <= 8)
                                    tmp = MConv.makebyte_dob(getinoutParaVals[i, j]);

                                foreach (string s in tmp.Split(' '))
                                {
                                    if (s != null && s != "")
                                        PreParaSend.Add(Convert.ToByte(s, 16));
                                }
                                if (i == 0)
                                {
                                    for (k = 0; k < 2; k++)
                                        PreParaSend.Add((byte)0x00);

                                }
                                else if (i == 2)
                                {
                                    PreParaSend.Add((byte)0x00);
                                    PreParaSend.Add((byte)0x01);
                                    PreParaSend.Add((byte)0x05);
                                    PreParaSend.Add((byte)0xDC);
                                    PreParaSend.Add((byte)0xFD);
                                    PreParaSend.Add((byte)0xA8);

                                    for (k = 0; k < 3; k++)
                                        PreParaSend.Add((byte)0x00);

                                    PreParaSend.Add((byte)0x01);
                                    PreParaSend.Add((byte)0x00);
                                    PreParaSend.Add((byte)0x01);
                                }
                                else if (i == 5)
                                {
                                    for (k = 0; k < 4; k++)
                                        PreParaSend.Add((byte)0x00);
                                }
                                else if (i == 8)
                                {
                                    for (k = 0; k < 11; k++)
                                        PreParaSend.Add((byte)0x00);

                                    PreParaSend.Add((byte)0x01);
                                    PreParaSend.Add((byte)0x00);
                                    PreParaSend.Add((byte)0x02);
                                    PreParaSend.Add((byte)0x00);
                                    PreParaSend.Add((byte)0x00);
                                }
                            }
                            string SentCnt = PreParaSend.Count.ToString("X");
                            byte TotalCnt = Convert.ToByte(SentCnt, 16);

                            Global.gTconCom.ParaWriteMsg((byte)(j + 1), TotalCnt, PreParaSend, 2);
                            _WriteParaWorker.ReportProgress(j * 5);
                            Thread.Sleep(100);
                            PreParaSend.Clear();
                        }
                    }
                    Thread.Sleep(1000);
                    _WriteParaWorker.ReportProgress(100);
                    Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.STOP;
                }
                catch
                {
                    if (_WriteParaWorker != null)
                        _WriteParaWorker.CancelAsync();

                    addParaLog("Write Error Retry");
                }
            }
            else
            {
                MessageBox.Show("장비 RUN 중에는 파라메터 변경 금지");
                return;
            }
        }
        public void addParaLog(string msg)
        {
            ParaLog plog = new ParaLog(msg);
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
            new Action(() =>
                {
                    ParaLogs.Add(plog);
                    if (ParaLogs.Count > 1000)
                        ParaLogs.RemoveAt(0);
                    ListParaLog.ScrollIntoView(plog);
                }));
        }
        public void addMotorLog(string msg)
        {
            MotorLog Mlog = new MotorLog(msg);
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
            new Action(() =>
            {
                MotorLogs.Add(Mlog);
                if (MotorLogs.Count > 1000)
                    MotorLogs.RemoveAt(0);
                ListMotorLog.ScrollIntoView(Mlog);
            }));
        }
    }
}
