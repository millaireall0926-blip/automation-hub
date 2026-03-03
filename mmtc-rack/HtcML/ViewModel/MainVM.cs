using HtcML.Base;
using Microsoft.Win32;
using System;
using System.Runtime.InteropServices;
//using System.Windows.Controls;
using HandyControl.Controls;
using System.Collections.ObjectModel;
using System.IO;
using Tensorflow.NumPy;
using Tensorflow.Keras.Engine;

using static Tensorflow.KerasApi;
using PropertyChanged;
using Serilog.Core;

using Serilog;
using System.Windows.Markup;
using Tensorflow;
using System.Windows.Documents;
using System.Windows.Media;

using ScrollViewer =  System.Windows.Controls.ScrollViewer;
using System.Windows;
using System.Threading;
using System.Threading.Tasks;
using Tensorflow.Keras.Callbacks;
using System.Security.Cryptography;
using Tensorflow.Keras.Layers;
using PureHDF.Selections;
using Ookii.Dialogs.Wpf;
using System.Diagnostics;
using System.Windows.Threading;
using System.Linq;
using System.Data.Common;
using System.Printing;
using System.Text;
using System.Collections.Generic;
using HtcML.IPC;
using System.Windows.Media.Imaging;
using System.Security.Policy;

namespace HtcML.ViewModel
{


    public class MainVM : BaseVM
    {

        [DllImport("user32.dll", SetLastError = true)]
        static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint lpdwProcessId);

        public string Title { get;set; }
        public bool EnableCmd { get; set; } = true;

        public Paragraph TrainingParagraph { get; set; }
        public ScrollViewer TrainingScroll { get; set; }

        public Visibility _trainingLoading = Visibility.Hidden;
        public Visibility TrainingLoading { get => _trainingLoading; 
            set { _trainingLoading = value;
                if (_trainingLoading == Visibility.Hidden)
                    EnableCmd = true;
                else
                    EnableCmd = false;

            }
        } 
        

        public Paragraph PredictParagraph { get; set; }
        public ScrollViewer PredictScroll { get; set; }

        public Visibility _predictLoading = Visibility.Hidden;
        public Visibility PredictLoading
        {
            get => _predictLoading;
            set
            {
                _predictLoading = value;
                if (_predictLoading == Visibility.Hidden)
                    EnableCmd = true;
                else
                    EnableCmd = false;

            }
        }


        public ObservableCollection<string> InputFilss { get; set; } = new ObservableCollection<string>();
        public int LearnEpochs { get; set; } = 100;
        public string OutputPath { get; set; } = @"d:\hdf5";

        public string OutputName { get; set; } = "sampleMulti.h5";

        public string SamplePath { get; set; } = @"D:\htc-data\ml\Rack1_2024_02_05_2114.csv";
        public string PredictPath { get; set; } = @"d:\hdf5\sampleMulti.h5";

        public string IPCMode { get; set; } = "Start";
        public SolidColorBrush IPCColor { get; set; } = new SolidColorBrush(Colors.CadetBlue);


        BitmapImage ConnectIcon { get; set; } = new BitmapImage(new Uri(@"Resource/connect.png", UriKind.RelativeOrAbsolute));
        BitmapImage DisconnectIcon { get; set; } = new BitmapImage(new Uri(@"Resource/disconnect.png", UriKind.RelativeOrAbsolute));
        public BitmapImage IPCStatIcon { get; set; }




        public DelegateCommand CmdInput { get; set; }
        public DelegateCommand CmddOutput { get; set; }
        public DelegateCommand CmdLearning { get; set; }
        public DelegateCommand CmdHDF5 { get; set; }
        public DelegateCommand CmdIPC { get; set; }
        public DelegateCommand CmdSample { get; set; }
        public DelegateCommand CmdSimul { get; set; }
        public DelegateCommand CmdSimCancle { get; set; }


        [DoNotNotify]
        public Logger SVSerilog { get; set; } = null;

        // IPC 관련 
        IPCServer _ipc = new IPCServer();
        Thread _ipcThread;
        bool _quitThread = false;

        // Predict Model
        Sequential[] PredictModels = new Sequential[3];


        public MainVM() {
            CmdInput = new DelegateCommand(p => loadTrainingData());
            CmddOutput = new DelegateCommand(p => doOutputPath());
            CmdLearning = new DelegateCommand(p => learningData(p as string));

            CmdHDF5 = new DelegateCommand(p => doHDF5());
            CmdIPC = new DelegateCommand(p => doIPC());
            CmdSample = new DelegateCommand(p => loadSampleData());
            CmdSimul = new DelegateCommand(p => doSimul());
            CmdSimCancle = new DelegateCommand(p => { CancleSim = true; });

            Title = VerInfo.getVer();

            IPCStatIcon = DisconnectIcon;
            _ipc.notifyCon += setConnectIcon;
        }

        public void doHDF5()
        {
            VistaFolderBrowserDialog dlg = new VistaFolderBrowserDialog();
            dlg.SelectedPath = PredictPath;
            dlg.ShowNewFolderButton = true;

            if ((bool)dlg.ShowDialog() == false) return;

            PredictPath = dlg.SelectedPath;
        }

        public void doOutputPath()
        {
            VistaFolderBrowserDialog dlg = new VistaFolderBrowserDialog();
            dlg.SelectedPath = OutputPath;
            dlg.ShowNewFolderButton = true;

            if ((bool)dlg.ShowDialog() == false) return;

            OutputPath = dlg.SelectedPath;

         }

        public void setModels()
        {
            for (int i = 0; i < 3; i++)
            {
                string path = PredictPath + "\\" +
                    $"Box{i}"; // box index
                LoadOptions loadOptions = new LoadOptions();
                //loadOptions.

                PredictModels[i] = (Sequential)keras.models.load_model(path);

            }
        }



        public static void FuncIPCQue(object obj)
        {
            MainVM mv = (MainVM)obj;
            while (!mv._quitThread)
            {
                if (mv._ipc.getQueSize() > 0)
                {
                    var msg =  mv._ipc.popRecvQue();
                    mv.predictOne(msg);
                }else 
                    Thread.Sleep(10);


            }
            
        }

        public void setConnectIcon(bool isCon)
        {
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                () =>
                {
                    if (isCon)
                        IPCStatIcon = ConnectIcon;
                    else
                        IPCStatIcon = DisconnectIcon;
                });
        }

        public void doIPC()
        {
            if (IPCMode == "Start")
            {
                if (_ipcThread != null) return;

                IPCColor =  new SolidColorBrush(Colors.PaleVioletRed);
                _ipcThread = new Thread(FuncIPCQue);
                setModels();
                _quitThread = false;
                _ipcThread.Start(this);
                
                _ipc.startServer();

                IPCMode = "Stop";
            }else
            {
                IPCColor = new SolidColorBrush(Colors.CadetBlue);

                _ipc.stopServer();
                
                _quitThread = true;
                _ipcThread.Join();
                _ipcThread = null;

                IPCMode = "Start";
            }
            
        }


        public void addParagraphText(Paragraph para, ScrollViewer scroll, string str, int type = 0)
        {

            var run = new Run(str + "\r\n");
            if (type == 0)
                run.Foreground = new SolidColorBrush(Colors.White);
            else if (type == 1)
                run.Foreground = new SolidColorBrush(Colors.Red);

            para.Inlines.Add(run);
            if (para.Inlines.Count > 200)                            
                para.Inlines.Remove(para.Inlines.FirstInline);
            
            scroll.ScrollToBottom();
        }
        public void addTraningText(string str)
        {
            addParagraphText(TrainingParagraph, TrainingScroll, str);
        }

        public void addPredictText(string str)
        {
            addParagraphText(PredictParagraph, PredictScroll, str);
        }


        public void makeHdfFolder()
        {
            DirectoryInfo di = new DirectoryInfo(OutputPath + "\\" + OutputName);
            if (di.Exists == false)
                di.Create();
        }

        public Sequential makeModel(int type)
        {
            Sequential model = keras.Sequential();

            if (type == 1)
            {
                model.add(keras.layers.Dense(32, activation: "relu"));
                model.add(keras.layers.Dense(64, activation: "relu"));
                model.add(keras.layers.Dense(64, activation: "relu"));
                model.add(keras.layers.Dense(32, activation: "relu"));
                model.add(keras.layers.Dense(1, activation: "relu"));
            }else
            {
                model.add(keras.layers.Dense(32, activation: "relu"));
                model.add(keras.layers.Dense(64, activation: "relu"));
                model.add(keras.layers.Dense(64, activation: "relu"));
                model.add(keras.layers.Dense(32, activation: "relu"));
                model.add(keras.layers.Dense(1, activation: "relu"));
            }



            model.compile(optimizer: "adam",
            loss: " mean_squared_error",
            metrics: new string[] { "accuracy" });

            return model;
            //
        }




        public void predictOne(IPCMessage msg)
        {
            int bi = msg.BoxNo;
            int size = 6;
            if (bi == 1) size *= 2;
            int tsz = size * 2 + 1;
            float[,] XData = new float[1, tsz];
            for (int i = 0; i < tsz; i++)
                XData[0, i] = msg.Temps[i];

            NDArray xnp = np.array(XData);
            Tensor tensor = PredictModels[bi].predict(xnp);
            float[] val = tensor.ToArray<float>();
            _ipc.writeByte(msg.RackNo, bi, val);

            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                new Action(() => addPredictText($"Box{msg.BoxNo} - SV: {val[0].ToString("F2")}")));                    

        }

        #region Simunlation
        /// <summary>
        /// Simunlation
        /// </summary>

        bool CancleSim = false;

        class ThreadParam
        {
            public object parent;
            public object list;
            public int boxIndex;

            public List<int> firstLines;
            public ThreadParam(object o1, object o2, int no, List<int> firstLines = null)
            {
                this.parent = o1; this.list = o2; this.boxIndex = no;
                this.firstLines = firstLines;   
            }
        }        

       public static void FuncSimThread(object tp)
        {
            Thread.CurrentThread.Priority = ThreadPriority.Highest;
            ThreadParam? param = tp as ThreadParam;
            MainVM? mv = param.parent as MainVM;
            Collection<string>? lines = param.list as Collection<string>;

            Sequential[] models = new Sequential[3];//= makeModel(0);

            for (int i = 0; i < 3; i++)
            {
                string path = mv.PredictPath + "\\" +
                    $"Box{i}"; // box index
                LoadOptions loadOptions = new LoadOptions();
                //loadOptions.

                models[i] = (Sequential)keras.models.load_model(path);

            }
            
            int[] dimStarts = { 1, 
                                Global.gIs16Dimm ? 5:7,
                                Global.gIs16Dimm ? 13:19 };

            int[] dimEnds = {   Global.gIs16Dimm ? 4:6,
                                Global.gIs16Dimm ? 12:18, 
                                Global.gIs16Dimm ? 16:24 };
            
            int[] svPoss = {    Global.gIs16Dimm ? 17:25, 
                                Global.gIs16Dimm ? 18:26,
                                Global.gIs16Dimm ? 19:27 };

            float[][,] XDatas = new float[3][,];
            float[][] XPrevs = new float[3][];
            float[][] XDiffs = new float[3][];
            for (int i = 0; i < 3; i++)
            {
                int size = Global.gIs16Dimm ? 4:6;
                if (i == 1) size *= 2;
                int tsz = size * 2 + 1;

                XPrevs[i] = new float[size];
                XDiffs[i] = new float[size];
                XDatas[i] = new float[1, tsz];
            }

            float[][] predicts = new float[3][];

            float SetSV = 0;
            int linetotal = lines.Count;
            int lineIndex = 0;

            DateTime prevT = DateTime.Now;
            foreach (var line in lines)
            {
                string[] word = line.Split(new char[] { ',' });
                if (word.Length < 20) continue;

                
                for (int bi = 0; bi < 3; bi++)
                {
                    float[,] XData = XDatas[bi];
                    float[] XPrev = XPrevs[bi];
                    float[] XDiff = XDiffs[bi];
                    int dimStart = dimStarts[bi];
                    int dimEnd = dimEnds[bi];
                    int svPos = svPoss[bi];

                    int DimmCnt = Global.gIs16Dimm ? 4:6;
                    if (bi == 1) DimmCnt *= 2;

                    if (lineIndex == 0)
                    {
                        int x = 0;
                        for (int i = dimStart; i <= dimEnd; i++, x++)
                            XPrev[x] = float.Parse(word[i]);
                        SetSV = float.Parse(word[svPos]);
                    }

                    int xIndex = 0;
                    for (int i = dimStart; i <= dimEnd; i++, xIndex++)
                    {
                        XData[0, xIndex] = float.Parse(word[i]);
                        XDiff[0] = XData[0, xIndex] - XPrev[xIndex];
                        XPrev[0] = XData[0, xIndex];
                    }

                    for (int i = 0; i < DimmCnt; i++, xIndex++)
                    {
                        XData[0, xIndex] = XDiff[i];
                    }

                    XData[0, xIndex] = SetSV;

                    NDArray xnp = np.array(XData);
                    Tensor tensor = models[bi].predict(xnp);
                    predicts[bi] = tensor.ToArray<float>();                    
                }                    
                

                if (lineIndex % 10 == 0)
                {                    
                    TimeSpan t = DateTime.Now - prevT;
                    prevT = DateTime.Now;

                    Console.WriteLine($"Index {lineIndex}/{linetotal} - Duration {t.ToString("ss")}s");
                    //Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                        //new Action(() => mv.addPredictText($"{lineIndex}/{linetotal} {t.ToString("ss")}s")));                    
                }

                if (lineIndex % 100 == 0) Thread.Sleep(100);

                lineIndex++;

                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < word.Length; i++)
                {
                    bool isPredict = false;
                    float value = 0;
                    for (int j = 0; j < svPoss.Length; j++) { 
                        if (svPoss[j] == i)
                        {
                            isPredict = true;
                            value = predicts[j][0];
                            break;
                        }
                    }

                    if (isPredict)
                        sb.Append(value.ToString("F1"));
                    else 
                        sb.Append(word[i]);
                    if (i < word.Length - 1) 
                        sb.Append(',');                    
                }
                mv.SVSerilog.Information(sb.ToString());
                if (mv.CancleSim) break;
            }
            mv.PredictLoading = Visibility.Hidden;

            mv.SVSerilog.Dispose();
            mv.SVSerilog = null;

            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
            new Action(() => mv.addPredictText($"Start End {DateTime.Now.ToString()}")));
        }

        public async void doSimul()
        {
            
            CancleSim = false;

            string file = $"test.csv";

            if (System.IO.File.Exists(file))
                System.IO.File.Delete(file);

            SVSerilog = new LoggerConfiguration()
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite, outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();


            Collection<string> lines = new Collection<string>();
            try
            {
                FileStream logFileStream = new FileStream(SamplePath, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
                StreamReader logFileReader = new StreamReader(logFileStream);
                while (!logFileReader.EndOfStream)
                {
                    lines.Add(logFileReader.ReadLine());
                }
                logFileReader.Close();
                logFileStream.Close();
            }
            catch (Exception ex)
            {
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return;
            }

            PredictLoading = Visibility.Visible;
            addPredictText($"Start Simnulation {DateTime.Now.ToString()}");

            ThreadParam tp = new ThreadParam(this as object, lines as object, 0);
            Thread simThread = new Thread(FuncSimThread);
            //FuncSimThread(tp);
            simThread.Start(tp);
        }
        #endregion

        public void loadSampleData()
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "csv File(*.csv)|*.csv|All Files(*.*)|*.*";
            ofd.FilterIndex = 0;
            ofd.RestoreDirectory = true;
            if (ofd.ShowDialog() == false) return;

            SamplePath = ofd.FileName;
        }

        public void loadTrainingData ()
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "csv File(*.csv)|*.csv|All Files(*.*)|*.*";
            ofd.FilterIndex = 0;
            ofd.RestoreDirectory = true;
            ofd.Multiselect = true;
            if (ofd.ShowDialog() == false) return;

            InputFilss.Clear();

            foreach (string s in ofd.FileNames)
            {
                InputFilss.add(s);
            }

        }

        /// <summary>
        /// Learning
        /// </summary>
        /// 

        public void FuncLearn(object tp)
        {
            Thread.CurrentThread.Priority = ThreadPriority.Highest;
            ThreadParam? param = tp as ThreadParam;
            MainVM? mv = param.parent as MainVM;
            List<int> firstLines = param.firstLines;

            Collection<string>? lines = param.list as Collection<string>;
            Sequential model = mv.makeModel(param.boxIndex);


            int DimmCnt = Global.gIs16Dimm ? 4 : 6;
            
            if (param.boxIndex == 1)
                DimmCnt = Global.gIs16Dimm ? 8 : 12;

            int xColumn = (DimmCnt * 2) + 1;

            float[,] XData = new float[lines.Count, xColumn];
            float[] XPrev = new float[DimmCnt];
            float[] XDiff = new float[DimmCnt];
            float[] YData = new float[lines.Count];

            // Multi File 일때 sv 변경 부분
            // gDimmCount에 따른 start end 변경

            // param.boxIndex == 0

            int dimStart = 1;
            int dimEnd = Global.gIs16Dimm ? 4:6;
            int svPos = Global.gIs16Dimm ? 17:25; 
            if (param.boxIndex == 1)
            {
                dimStart = Global.gIs16Dimm ? 5:7;
                dimEnd = Global.gIs16Dimm ? 12:18;
                svPos = Global.gIs16Dimm ? 18:26;
            }
            else if (param.boxIndex == 2)
            {
                dimStart = Global.gIs16Dimm ? 13:19;
                dimEnd = Global.gIs16Dimm ? 16:24;
                svPos = Global.gIs16Dimm ? 19:27;
            }

            float SetSV = 0;
            int lineIndex = 0;
            foreach (var line in lines)
            {

                string[] word = line.Split(new char[] { ',' });  
                if (word.Length < 20) continue;
                if (word[1] == "") continue;
                
                // Line 0  Prev Data, SetSV
                if (firstLines.Contains(lineIndex))
                {
                    int x = 0;
                    for (int i = dimStart; i <= dimEnd; i++, x++)
                        XPrev[x] = float.Parse(word[i]);
                    SetSV = float.Parse(word[svPos]);
                }

                int xIndex = 0;

                //Console.WriteLine($"Line {lineIndex}");
                // Dimm Temp
                for (int i = dimStart; i <= dimEnd; i++, xIndex++)
                {
                    XData[lineIndex, xIndex] = float.Parse(word[i]);
                    XDiff[xIndex] = XData[lineIndex, xIndex] - XPrev[xIndex];
                    XPrev[xIndex] = XData[lineIndex, xIndex];

                    
                }

                // Dimm Diff
                for (int i = 0; i < DimmCnt; i++, xIndex++)
                {
                    XData[lineIndex, xIndex] = XDiff[i];
                }

                // SV
                XData[lineIndex, xIndex] = SetSV;

                // Label 
                YData[lineIndex] = float.Parse(word[svPos]);
                lineIndex++;
            }

            NDArray xnp = np.array(XData);
            NDArray ynp = np.array(YData);

            model.fit(
                xnp, ynp,
                epochs: mv.LearnEpochs,
                use_multiprocessing: true,
                workers: 6
            );

            string path = mv.OutputPath + "\\" + 
                mv.OutputName + "\\" + 
                $"Box{param.boxIndex}"; // box index
            model.save(path);

        }

        public void testDelay(object obj)
        {
            int sleep = (int)obj;
            Thread.Sleep(sleep * 1000);

        }

        

        
        public async void learningData(string Select)
        {

            if (InputFilss.Count == 0) return;  
            TrainingLoading = Visibility.Visible;
            addTraningText($"Traing Data :");
            string files = "";
            foreach (var s in InputFilss)
                files += s + " ";
            addTraningText(files);

            DateTime startTime = DateTime.Now;
            addTraningText($"Traing Start -> {startTime.ToString("MM-dd HH:mm:ss")}");

            

            Collection<string> lines = new Collection<string>();
            List<int> firstLines = new List<int>();
            try
            {
                
                foreach (var file in InputFilss)
                {
                    FileStream logFileStream = new FileStream(file, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
                    StreamReader logFileReader = new StreamReader(logFileStream);
                    
                    firstLines.add(lines.Count);
                    while (!logFileReader.EndOfStream)
                    {
                        lines.Add(logFileReader.ReadLine());
                        
                    }
                    logFileReader.Close();
                    logFileStream.Close();
                }
            }
            catch (Exception ex)
            {
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return;
            }


            makeHdfFolder();

//            Thread thread1 = new Thread(FuncLearnThread);
//            Thread thread2 = new Thread(FuncLearnThread);
//            Thread thread3 = new Thread(FuncLearnThread);
//
            ThreadParam p1 = new ThreadParam(this as object, lines as object, 0, firstLines);
            ThreadParam p2 = new ThreadParam(this as object, lines as object, 1, firstLines);
            ThreadParam p3 = new ThreadParam(this as object, lines as object, 2, firstLines);
            if (Select == "ALL")
            {
                await Task.Run(() => FuncLearn((object)p1));
                await Task.Run(() => FuncLearn((object)p2));
                await Task.Run(() => FuncLearn((object)p3));
            }else
            {
                ThreadParam? sel = null; ;
                if (Select == "A") sel = p1;
                else if (Select == "B") sel = p2;
                else if (Select =="C") sel = p3;
                await Task.Run(() => FuncLearn((object)sel));
            }


            //          thread1.Start(p1);
            //            thread1.Join();
            //            
            //            thread2.Start(p2);
            //            thread2.Join();
            //            
            //            thread3.Start(p3);
            //            thread3.Join();

            TrainingLoading = Visibility.Hidden;

            TimeSpan span = DateTime.Now - startTime;
            addTraningText($"Traing End Elapsed : {span.ToString()} ");
            addTraningText($"Compleate Save HDF5");
        }

    }
}
