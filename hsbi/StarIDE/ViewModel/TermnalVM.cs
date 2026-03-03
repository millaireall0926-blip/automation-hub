using StarBase;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;
using StarIDE.Controls;
using System.Security.Permissions;
using System.Net.NetworkInformation;
using Microsoft.Xaml.Behaviors.Media;

namespace StarIDE.ViewModel
{
    public class TermnalVM : BaseVM
    {
        public enum CMD_TYPE : int
        {
            PING = 0,
            COPY = 1,
            EXECUTE = 2
        };

        public enum CMD_MODE
        {
            None = 0,
            Con = 1,
            Run,
            Diag,
            End
        }

        
        
        ObservableCollection<int> _slotIPs = new ObservableCollection<int>();
        public ObservableCollection<int> SlotIPs { get => _slotIPs; 
            set{ _slotIPs = value; OnPropertyChanged(nameof(SlotIPs));} }

        int _selectIP = 1;
        public int SelectIP { get => _selectIP; set { _selectIP = value; 
            OnPropertyChanged(nameof(SelectIP));} }

        public ObservableCollection<string> _lines = new ObservableCollection<string>();
        public ObservableCollection<string> Lines { get => _lines; 
            set { _lines = value; OnPropertyChanged(nameof(Lines)); } }
        public DelegateCommand CmdInput { get; set; }
        public DelegateCommand CmdRun { get; set; } 
        public DelegateCommand CmdCopy { get; set;}
        public DelegateCommand CmdPing { get; set; }
        public DelegateCommand CmdCancle { get; set; }

        public DelegateCommand CmdMode { get; set; }

        static CMD_MODE _cmdMode = CMD_MODE.None;
        public string _exeMode = _cmdMode.ToString();
        public string ExeMode
        {
            get => _exeMode; set { _exeMode = value; OnPropertyChanged(nameof(ExeMode)); }
        }

        public bool _useNet = false;
        public bool UseNet
        {
            get => _useNet; set { _useNet = value; OnPropertyChanged(nameof(UseNet)); }
        }

        public string _diagArg = "03";
        public string DiagArg
        {
            get => _diagArg; set { _diagArg = value; OnPropertyChanged(nameof(DiagArg)); }
        }

        public bool _enableCmd = true;
        public bool  EnableCmd
        {
            get => _enableCmd ; set { _enableCmd = value; OnPropertyChanged(nameof(EnableCmd)); }
        }

        public bool _enableCancle = true;
        public bool EnableCancle
        {
            get => _enableCancle; set { _enableCancle = value; CmdWait = Visibility.Visible;
                OnPropertyChanged(nameof(EnableCancle)); }
        }

        public Visibility _cmdWait = Visibility.Hidden;
        public Visibility CmdWait{  get => _cmdWait;
            set { 
                _cmdWait = value;
                if (_cmdWait == Visibility.Visible) EnableCmd = false;
                else EnableCmd = true;

                OnPropertyChanged(nameof(CmdWait));} 
        }


        private CmdControl _cmdCtrl = null;
        bool IsFirstSkip = false;


        public TermnalVM() {

            for (int i = 1; i<=50; i++)
            {
                SlotIPs.Add(i);
            }

            addLine("Terminal Start");

            CmdCancle = new DelegateCommand(async p =>
            {
                EnableCancle = false;
                stopCmdCtrl();                
                await Task.Delay(1000);
                startCmdCtrl();
            });

            CmdInput = new DelegateCommand(p =>
            {
                if (!EnableCancle) return;

                CmdWait = Visibility.Visible;
                string cmd = p as string;
                doCommand(cmd);

                
            });

            CmdCopy = new DelegateCommand(p =>
            {
                CmdWait = Visibility.Visible;
                string id = "pc";
                string pwd = "1111";
                string Ip = $"192.168.50.{SelectIP}";
                string path = "/star/pgm/";

                string[,] drive = { { "C:","c" }, { "D:", "d" }, { "E:", "e" },
                    { "F:","f" }, { "G:", "g" }, { "H:", "h" }, { "I:", "i" }
                };

                string folder = "";

                if (Global.gFilesVM._folderPath == null) return;
                for (int i = 0; i < drive.GetLength(0); i++)
                {
                    if (Global.gFilesVM._folderPath.IndexOf(drive[i, 0]) != -1)
                    {
                        string replace =  Global.gFilesVM._folderPath.Replace(drive[i, 0], drive[i, 1]);

                        folder = $"/mnt/" + replace.Replace('\\', '/');
                        break;
                    }
                }

                string copy = folder + "/*.pgm "
                             + folder + "/*.mpa";
                string copyCmd = $"wsl sshpass -p 1111 scp {copy} {id}@{Ip}:{path}";

                doCommand(copyCmd);                 
                

            });

            CmdRun = new DelegateCommand(p =>
            {
                CmdWait = Visibility.Visible;
                string id = "pc";
                string pwd = "1111";
                string Ip = $"192.168.50.{SelectIP}";
                string path = "/star/pgm/";

                string pgm = "";
                foreach (var model in Global.gFilesVM.MakedModels)
                {
                    if (model.Type == Model.MAKED_TYPE.PGM)
                    {
                        pgm = model.Name + "." + model.Type.ToString().ToLower();
                        break;
                    }
                }

                if (pgm == "")
                {
                    addLine("Not Found PGM File");
                    return;
                }

                string arg = UseNet ? "-net" : "";
                if (_cmdMode != CMD_MODE.None)
                {
                    arg += "-" + ExeMode.ToLower();
                    if (_cmdMode == CMD_MODE.Diag) arg += "-darg=" + DiagArg;
                }
                
                string runCmd = $"wsl sshpass -p {pwd} ssh {id}@{Ip} -o StrictHostKeyChecking=no \'sudo {path}{pgm} {arg}\'";
                doCommand(runCmd);

            });

            CmdPing = new DelegateCommand(p =>
            {
                CmdWait = Visibility.Visible;
                string pingCmd = $"ping -w 1 192.168.50.{SelectIP}";
                doCommand(pingCmd);
            });

            CmdMode = new DelegateCommand(p => {
                _cmdMode = _cmdMode + 1;
                if (_cmdMode == CMD_MODE.End) _cmdMode = CMD_MODE.None;
                ExeMode = _cmdMode.ToString();

            });

            startCmdCtrl();
        }

        public void createCmdCtrl()
        {
            _cmdCtrl = new CmdControl();

            _cmdCtrl.OnProcessOutput += OnCmdOut;
            _cmdCtrl.OnProcessError += OnCmdError;
            _cmdCtrl.OnProcessExit += OnCmdExit;

        }

        public void stopCmdCtrl()
        {
            _cmdCtrl.StopProcess();
            _cmdCtrl.Dispose();
            _cmdCtrl= null;
        }

        public void startCmdCtrl()
        {
            if (_cmdCtrl == null) { createCmdCtrl(); }

            ProcessStartInfo info = new ProcessStartInfo("cmd.exe", string.Empty);
            _cmdCtrl.StartProcess(info);

        }

        public void doCommand(string cmd)
        {
            _cmdCtrl.WriteInput(cmd);
            IsFirstSkip = true;
        }


         public void addLine(string line)
        {
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() =>
                    {
                        Lines.Add(line);
                        if (Lines.Count > 300) Lines.RemoveAt(0);
                    }));
        }

        void OnCmdOut(object sender, ProcessEventArgs args)
        {
            if (IsFirstSkip)
                IsFirstSkip = false;
            else
            {
                if (!EnableCancle) {
                    args.Content.IndexOf("Exit");
                    EnableCancle = true;
                    CmdWait = Visibility.Hidden;
                }else
                {
                    CmdWait = Visibility.Hidden;
                }

                string[] content = args.Content.Split('\n');  //args.Content.Replace(@"\n", @"\r\n");
                foreach (string line in content)
                {
                    addLine(line);
                }
            }
        }

        void OnCmdError(object sender, ProcessEventArgs args)
        {
            addLine("Error>"+ args.Content);
        }

        void OnCmdExit(object sender, ProcessEventArgs args)
        {
            addLine("Exit");
        }

    }
}
