using StarBase;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StarIDE.ViewModel
{
    public class TermnalVM : BaseVM
    {

        public ObservableCollection<string> _lines = new ObservableCollection<string>();
        public ObservableCollection<string> Lines { get => _lines; set { _lines = value; OnPropertyChanged(nameof(Lines)); } }
        public DelegateCommand CmdInput { get; set; }


        Process _proc = null;


        
        
        public TermnalVM() {
            openProc(); 

            addLine("Terminal Start");
            CmdInput = new DelegateCommand(p =>
            {
                string cmd = p as string;
                addLine(cmd);
                if (cmd != "")
                {
                    doProc(cmd);
                }
            });
                        
        }

        public void addLine(string line)
        {
            Lines.Add(line);
            if (Lines.Count > 300) Lines.RemoveAt(0);

        }

        public async void doProc(string cmd)
        {
            if (_proc == null) return;
            _proc.StandardInput.WriteLine(cmd);            
            await Task.Delay(100);
            _proc.StandardInput.Flush();
            //await _proc.StandardInput.FlushAsync();            
            _proc.StandardInput.Close();

            while (_proc.StandardOutput.Peek() > 0)
            {
                addLine($"{_proc.StandardOutput.ReadLine()}");                
            }
            _proc.WaitForExit();
        }

        public void recvProc()
        {

        }



        public void openProc()
        {
            if (_proc != null)
                closeProc();
            _proc = new Process();
            ProcessStartInfo info = new ProcessStartInfo()
            {
                FileName = @"cmd.exe",
                UseShellExecute = false,
                RedirectStandardOutput = true,
                RedirectStandardInput = true,
                RedirectStandardError = true,
                CreateNoWindow = true,
            };
            _proc.StartInfo = info;
            _proc.Start();


        }

        public void closeProc()
        {
            if (_proc == null) return;
            _proc.WaitForExit();
            _proc.Close();
            _proc = null;

        }



    }
}
