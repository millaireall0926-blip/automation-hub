using StarLauncher.Common;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StarLauncher.Model;
using System.Windows.Threading;
using System.Threading;
using System.Diagnostics;
using System.IO;

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
        public bool AliveOS
        {
            get => _aliveOS;
            set
            {
                _aliveOS = value; OnPropertyChanged(nameof(AliveOS));
            }
        }

        DispatcherTimer _aliveTimer = new DispatcherTimer();

        public MainVM()
        {
            ToolModels.Add(new ToolModel("StarOS.exe") { Title = "OS", Tip= "Star-Test Operation System"});
            ToolModels.Add(new ToolModel("StarIDE\\StarIDE.exe") { Title = "Edit", Tip = "Star-Test IDE(Integrated Development Environment)" });
            ToolModels.Add(new ToolModel("StarResult\\StarResult.exe") { Title = "View", Tip = "Star-Test Result Viewer" });
            //ToolModels.Add(new ToolModel("StarMaint.exe") { Title = "Maint", Tip = "Star-Test maintenance Program" });
            ToolModels.Add(new ToolModel("StarRouter\\StarRouter.exe") { Title = "Router", Tip = "Star-Test Slot Routing Program" });

            _aliveTimer.Interval = TimeSpan.FromSeconds(10);
            _aliveTimer.Tick += aliveTick;
            _aliveTimer.Start();
        }

        public bool CheckAlive(string name)
        {
            Mutex mutex = new Mutex(false, name, out bool createdNew);
            Console.WriteLine($"{name} {createdNew}");
            mutex.Close();
            if (createdNew)
                return false;

            return true;
        }

        public void aliveTick(object sender, EventArgs e)
        {
            if (!AliveOS) return;

            bool ret = CheckAlive("StarOS");
            if (!ret) Process.Start("StarOS.exe");

            string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "StarRouter", "StarRouter.exe");
            ret = CheckAlive("StarRouter");                        
            if (!ret) Process.Start(path);
        }

    }

    
}
