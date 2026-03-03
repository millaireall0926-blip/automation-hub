using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Media;
using System.Windows.Shell;
using System.Windows.Threading;
using System.Xml.Linq;
using RedfishLib;
using RedfishTest.Base;
using RedfishTest.Model;
using Serilog;
using Serilog.Core;


namespace RedfishTest.ViewModel
{
    public class MainVM : BaseVM
    {
        
        ObservableCollection<DimmTemp> _dimmTemps = new ObservableCollection<DimmTemp>();
        public ObservableCollection<DimmTemp> DimmTemps
        {
            get { return _dimmTemps; }
            set { _dimmTemps = value; OnPropertyChanged(nameof(DimmTemps)); }
        }

        string _redfishIP = "192.168.50.144";
        public string RedfishIP
        {
            get { return _redfishIP; }
            set { _redfishIP = value; OnPropertyChanged(nameof(RedfishIP)); }
        }

        string _loginPath = "gsipw1234";
        public string LoginPath
        {
            get { return _loginPath; }
            set { _loginPath = value; OnPropertyChanged(nameof(LoginPath)); }
        }
        

        string _userID = "gsi7";
        public string UserID
        {
            get { return _userID; }
            set { _userID = value; OnPropertyChanged(nameof(UserID)); }
        }

        string _password = "gsipw1234";
        public string Password
        {
            get { return _password; }
            set { _password = value; OnPropertyChanged(nameof(Password)); }
        }


        Paragraph _respParagraph;
        public Paragraph RespParagraph
        {
            get { return _respParagraph; }
            set { _respParagraph = value; OnPropertyChanged(nameof(RespParagraph)); }
        }

        ScrollViewer _respScroll;
        public ScrollViewer RespScroll
        {
            get { return _respScroll; }
            set { _respScroll = value; OnPropertyChanged(nameof(RespScroll)); }
        }

        public bool _isGetEnable = true;
        public bool IsGetEnable
        {
            get { return _isGetEnable; }
            set {
                _isGetEnable = value;
                IsGetDisEnable = !_isGetEnable; 
                OnPropertyChanged(nameof(IsGetEnable));
            }
        }

        public bool _isGetDisEnable = false;
        public bool IsGetDisEnable
        {
            get { return _isGetDisEnable; }
            set { _isGetDisEnable = value; OnPropertyChanged(nameof(IsGetDisEnable)); }
        }

        public int _comboSelectedIndex = 0;
        public int ComboSelectedIndex
        {
            get { return _comboSelectedIndex; }
            set { 
                _comboSelectedIndex = value;
                _tempPaths = _comboSelectedIndex == 0 ? _tempPaths12 : _tempPaths24;
                TempList = "[Time] ";
                foreach (var path in _tempPaths)
                    TempList += path.Replace("_", "").Replace("PU", "") + " ";
                Console.WriteLine(TempList);

                OnPropertyChanged(nameof(ComboSelectedIndex)); }
        }

        public string _tempList = "";
        public string TempList
        {
            get { return _tempList; }
            set { _tempList = value; OnPropertyChanged(nameof(TempList)); }
        }


        public Logger _logTemp = null;
        public void startLog()
        {
            if (_logTemp != null)
                _logTemp.Dispose();

            DateTime now = DateTime.Now;
            string file = $"{DateTime.Now.ToString("MM_dd_HH_mm")}_DimmTemp.log";
            _logTemp = new LoggerConfiguration()                           
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite,
                outputTemplate: "{Timestamp:[yyyy-MM-dd HH:mm:ss:fff]}{Message:lj}{NewLine}")
                .CreateLogger();
        }

        public void addLog(string str)
        {
            if (_logTemp == null)
                startLog();
            _logTemp.Information(str);
        }

        public void endLog()
        {
            if (_logTemp == null) return;
            _logTemp.Dispose();
            _logTemp = null;
        }


        public string[] _tempPaths;
        public string[] _tempPaths24 = {
            "A1_CPU1", "A1_CPU2", "B1_CPU1", "B1_CPU2", "C1_CPU1", "C1_CPU2", "D1_CPU1", "D1_CPU2",
            "E1_CPU1", "E1_CPU2", "F1_CPU1", "F1_CPU2", "G1_CPU1", "G1_CPU2", "H1_CPU1", "H1_CPU2",
            "I1_CPU1", "I1_CPU2", "J1_CPU1", "J1_CPU2", "K1_CPU1", "K1_CPU2", "L1_CPU1", "L1_CPU2",
            "A2_CPU1", "A2_CPU2", "B2_CPU1", "B2_CPU2", "C2_CPU1", "C2_CPU2", "D2_CPU1", "D2_CPU2",
            "E2_CPU1", "E2_CPU2", "F2_CPU1", "F2_CPU2", "G2_CPU1", "G2_CPU2", "H2_CPU1", "H2_CPU2",
            "I2_CPU1", "I2_CPU2", "J2_CPU1", "J2_CPU2", "K2_CPU1", "K2_CPU2", "L2_CPU1", "L2_CPU2"
        };

        public string[] _tempPaths12 = {
            "A1_CPU1", "A1_CPU2", "B1_CPU1", "B1_CPU2", "C1_CPU1", "C1_CPU2", "D1_CPU1", "D1_CPU2",
            "E1_CPU1", "E1_CPU2", "F1_CPU1", "F1_CPU2", "G1_CPU1", "G1_CPU2", "H1_CPU1", "H1_CPU2",
            "I1_CPU1", "I1_CPU2", "J1_CPU1", "J1_CPU2", "K1_CPU1", "K1_CPU2", "L1_CPU1", "L1_CPU2",
            
        };

        public DelegateCommand GetCommandOne { get; set; }
        public DelegateCommand GetCommandContinue { get; set; }
        public MainVM()
        {            
            Global.gMainVM = this;
            
            
            addLog("Start Application");

            GetCommandOne = new DelegateCommand(async (e) =>
            {
                IsGetEnable = false;
                string sync = e as string;


                string res = DateTime.Now.ToString("[HH:mm:ss.fff]  ");
                Stopwatch stopwatch = Stopwatch.StartNew();
                if (sync == "r")
                {
					var stat = await RestBMC_Static.Reset(RedfishIP, UserID, Password);


					if (stat.Item1 != SESSION_STATUS.OK)
					{
						stopwatch.Stop();
						res = $"Redfish reset Fail {stat.Item2.ToString()} [{stopwatch.ElapsedMilliseconds}ms]";
						addParagraphText(res);
						return;
					}
				}
                else
                {
                    SESSION_STATUS stat = await RestBMC_Static.open(this.RedfishIP, this.UserID, this.Password);
                    if (stat != SESSION_STATUS.OK)
                    {
                        IsGetEnable = true;
                        stopwatch.Stop();
                        res = $"Redfish Login Fail {stat.ToString()} [{stopwatch.ElapsedMilliseconds}ms]";
                        addParagraphText(res);
                        return;
                    }
                }
                if (sync == "s") // sync
                {
                    
                    foreach (var path in _tempPaths)
                    {
                        double result = await RestBMC_Static.getTemp(path);
                        res += result.ToString("F1") + " ";
                    }
                }
                else if (sync == "a") // async
                {
                    Task<double>[] tasks = _tempPaths.Select(path => RestBMC_Static.getTemp(path)).ToArray();
                    double[] results = await Task.WhenAll(tasks);

                    foreach (var d in results)
                        res += d.ToString("F1") + " ";
                }
               
                stopwatch.Stop();
                res += $" [{stopwatch.ElapsedMilliseconds}ms]";
                addParagraphText(res);

                RestBMC_Static.close();
                IsGetEnable = true;
                                
            });

            GetCommandContinue = new DelegateCommand(async (e) =>
            {
                
                string mode = e as string;
                if (mode == "c") // continue
                {
                    IsGetEnable = false;
                    _isStop = false;
                    _getThread = new Thread(doGetThread);
                    _getThread.Start(this);
                }
                else if (mode == "t")//teminate
                {
                    IsGetEnable = true;
                    _isStop = true;
                    //_getThread.Interrupt();
                    _getThread.Join();
                
                }
                
            });
        }

        public Thread _getThread;
        public bool _isStop = true;
        public static async void doGetThread(object parent)
        {
            MainVM mainVM = parent as MainVM;
            
            string log = "";
            int index = 0;
            try
            {
                while (!mainVM._isStop)
                {
                    
                    string time = DateTime.Now.ToString("[HH:mm:ss.fff]  ");
                    Stopwatch stopwatch = Stopwatch.StartNew();
                    log = "";

                    SESSION_STATUS stat = await RestBMC_Static.open(mainVM.RedfishIP, mainVM.UserID, mainVM.Password);

                    if (stat != SESSION_STATUS.OK)
                    {
                        stopwatch.Stop();
                        log = $"Redfish Login Fail {stat.ToString()} [{stopwatch.ElapsedMilliseconds}ms]";
                        mainVM.addParagraphText(log);
                        continue;
                    }
                    
                    Task<double>[] tasks = mainVM._tempPaths.Select(path => RestBMC_Static.getTemp(path)).ToArray();
                    double[] results = await Task.WhenAll(tasks);

                    RestBMC_Static.close();
                    //double[] results = Task.Run(() => Task.WhenAll(tasks)).Result;
                    stopwatch.Stop();
                    foreach (var d in results)
                        log += d.ToString("F1") + " ";
                    log += $" [{stopwatch.ElapsedMilliseconds}ms]";
                    
                    await Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
                    {
                        mainVM.addParagraphText(time + log);
                        mainVM.addLog(log);
                    }));                                      
                }
            }
            catch (Exception ex)
            {                
                mainVM.addParagraphText("Thread Exception : " + ex.Message);
            }
            finally
            {                
                RestBMC_Static.close();                
                mainVM.IsGetEnable = true;
            }
        }

        public void addParagraphText(string str)
        {
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() =>
            {
                Paragraph para = RespParagraph;
                ScrollViewer scroll = RespScroll;

                var run = new Run(str + Environment.NewLine) { Foreground = new SolidColorBrush(Colors.White) }; // ✅ `Environment.NewLine` 사용
                para.Inlines.Add(run);

                if (para.Inlines.Count > 400)
                    para.Inlines.Remove(para.Inlines.FirstInline);

                scroll.ScrollToBottom();
            }));
        }

    }

}
