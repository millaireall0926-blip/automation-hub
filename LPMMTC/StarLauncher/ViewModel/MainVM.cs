using StarLauncher.Common;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StarLauncher.Model;
using System.Threading;
using System.Windows.Threading;
using System.Diagnostics;
using System.Windows.Controls.Primitives;

namespace StarLauncher.ViewModel
{
    public class MainVM : BaseVM
    {
        ObservableCollection<ToolModel> _toolModels = new ObservableCollection<ToolModel>();
        public ObservableCollection<ToolModel> ToolModels
        {
            get { return _toolModels; }
            set { _toolModels = value; OnPropertyChanged(nameof(ToolModels)); }
        }

        public bool _aliveOS = false;
        public bool AliveOS { 
            get => _aliveOS;
            set
            {
                _aliveOS = value; OnPropertyChanged(nameof(AliveOS));
            } 
        }

        DispatcherTimer _aliveTimer = new DispatcherTimer();

        public MainVM()
        {
            ToolModels.Add(new ToolModel("LP-MMTCRack.exe") { Title = "OS", Tip= "LPMMTCRack Operation System"});
            ToolModels.Add(new ToolModel("TempView\\TempView.exe") { Title = "Log Graph", Tip = "LPMMTCRack Log Graph View" });
            ToolModels.Add(new ToolModel("TempMergeView\\TempMergeView.exe") { Title = "Merge Log", Tip = "LPMMTCRACK Merge Log Graph" });

            _aliveTimer.Interval = TimeSpan.FromSeconds(6);
            _aliveTimer.Tick += aliveTick;
            _aliveTimer.Start();
        }


        public bool CheckAlive(string name)
        {                        
            Mutex mutex = new Mutex(false, name, out bool createdNew);
            Console.WriteLine($"createdNew {createdNew}");
            mutex.Close();
            if (createdNew)
                return false;
            
            return true;
        }

        public void aliveTick(object sender, EventArgs e)
        {
            if (AliveOS == false) return;
            string name = "LP-MMTCRack";
            bool ret = CheckAlive(name);
            if (!ret) {
                Process.Start("LP-MMTCRack.exe");
            }
        }

    }

    
}
